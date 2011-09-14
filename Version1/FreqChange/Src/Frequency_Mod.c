/*
Copyright (C) 2011 Exascale Research Center

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include <assert.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#include "rdtsc.h"
#include "Frequency_Mod.h"
#include "Log.h"

static __inline__ unsigned long long getTicks (void)
{
   unsigned long long ret;
   rdtscll(ret);
   return ret;
}



//this function is implemented with a high overhead! So we Only read the frequency once and then store it somewhere.
static int helperReadFreq (SFreqData * context, int procId)
{
	char curFreqString[1024]="";
	char core[1024];
	
	strcat (curFreqString,"/sys/devices/system/cpu/cpu");
	sprintf (core,"%d",procId);
	strcat (curFreqString,core);
	strcat (curFreqString,"/cpufreq/cpuinfo_cur_freq");
	
	char currentFreq[512];
	int curFreq,i;
	FILE * fp;
	
  	fp = fopen (curFreqString,"r");
  	if (fp==NULL)
	{
		printf ("Failed to open cpufreq datafile\n");
		exit (EXIT_FAILURE);
	}
	else
	{
		if (fgets (currentFreq, sizeof (currentFreq), fp))
		{
			curFreq = atoi ( currentFreq);
		}
		else
		{
			printf ("fopen call failed somehow 1\n");
			exit (EXIT_FAILURE);
		}
		
	}
	fclose (fp);

	for (i=0; i < context->numFreq ; i++)
	{
		if (curFreq==context->availableFreqs[i])
			return i;	
	}
	Log_output (0,"Error: no match between available frequencies and current frequency of processor %d\n",procId);
	exit (1);
	return -1;
	
}


SFreqData * initCpufreq (void)
{
	SFreqData * handle;
	
	int i;
	char char_buff[1024]="";
	int globalFrequency[NUM_STATIC_FREQ];//this is a place to temporarily store all the values until we alloc on the heap
	int num_frequency=0;//another temp holder
	int num_cores=0;
	

	//find all the frequencies and record them
	FILE * fp;
  	fp = fopen ("/sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies","r");
	
	if (fp==NULL)
	{
		printf ("Failed to open cpufreq datafile\n");
		exit (EXIT_FAILURE);
	}
	else
	{
		if (fgets (char_buff, sizeof (char_buff), fp)) 
		{
			char * pch;
			pch = strtok (char_buff," ");
			while (pch != NULL)
			{
				globalFrequency[num_frequency]=atoi(pch);
				
				if (globalFrequency[num_frequency]==0)
				{
					break;
				}
				num_frequency++;
				if (num_frequency == NUM_STATIC_FREQ)
				{
					printf ("Ran out of allocation for frequencies, set NUM_STATIC_FREQ higher\n");
					exit (EXIT_FAILURE);
				}
				pch = strtok (NULL, " ");
			}
		}
		else
		{
			printf ("fopen call failed somehow 2\n");
			exit (EXIT_FAILURE);
		}
	}
	fclose (fp);

	//find number of cores
	fp= fopen ("/proc/cpuinfo", "r");
	if (fp==NULL)
	{
		printf ("Failed to find number of cpus in /proc/cpuinfo\n");
		exit (EXIT_FAILURE);
	}
	else
	{
		while (fgets (char_buff, sizeof (char_buff), fp))
		{
			//Check if there is something
			if (strstr (char_buff, "processor") != NULL)
			{
				num_cores++;
			}
		}	
		fclose (fp);
	}

	assert (num_cores > 0);
	/*now allocate a context with all my mallocs
	*/
	
	handle= malloc ( sizeof(* handle));
	assert (handle!=NULL);
	handle->setFile=malloc (sizeof (*(handle->setFile))*num_cores);
	assert (handle->setFile!=NULL);
	handle->currentFreqs=malloc (sizeof (*(handle->currentFreqs))*num_cores);
	assert (handle->currentFreqs!=NULL);
	handle->availableFreqs=malloc (sizeof (*(handle->availableFreqs))*num_frequency );
	

	handle->numFreq=num_frequency;
	handle->numCores=num_cores;
	handle->freqMax=globalFrequency[0];
	handle->freqMin=globalFrequency[num_frequency-1];
	handle->thisSample=0;
	
	for (i=0; i<num_frequency;i++)
	{
		handle->availableFreqs[i]=globalFrequency[i];//copy frequencies into data structure
	}

	for (i=0; i<num_cores;i++)
	{
		//first set the govenor to userspace
		sprintf (char_buff,"/sys/devices/system/cpu/cpu%d/cpufreq/scaling_governor", i);

		FILE *f = fopen (char_buff, "w");

		if (fp==NULL)
		{
			perror ( "Error opening file" );
        	    	printf ( "Error opening file: %s - %s\n", char_buff, strerror( errno ) );
			printf ( "Perhaps you don't have sudo rights?\n");
		}
		assert (f != NULL);

		fprintf (f, "userspace");

		fclose (f), f = NULL;

		//now open the file descriptor
		sprintf (char_buff,"/sys/devices/system/cpu/cpu%d/cpufreq/scaling_setspeed", i);
		fp = fopen (char_buff,"w");

		if (fp==NULL)
		{
			perror ( "Error opening file" );
        	    	printf ( "Error opening file: %s\n", strerror( errno ) );
			printf ( "Perhaps you don't have sudo rights?\n");
		}

		handle->setFile[i]=fp;//file * opened so now we place it in the context
	}

	for (i=0; i<num_cores;i++)
	{
		handle->currentFreqs[i]=helperReadFreq (handle, i);
	}	

	return handle;	
}


int readFreq (SFreqData * context, int procId)
{
	return context->currentFreqs[procId];
}


void changeFreq (SFreqData * context, int core, int i)
{

	if(core == -1)//this means change all the cores
	{
		int j;
		for (j=0;j<(context->numCores);j++)
		{	
			context->sampler[context->thisSample].time=getTicks ();
			context->sampler[context->thisSample].freq=i;
			context->sampler[context->thisSample].core=j;
			context->thisSample++;
			if (context->thisSample > NUMSAMPLES)
			{
				context->thisSample=NUMSAMPLES;
			}

			fseek (context->setFile[j],0,SEEK_SET);
			fprintf (context->setFile[j],"%d\n",context->availableFreqs[i]);
			fflush (context->setFile[j]);
			context->currentFreqs[j]=i;
		}
	}
	else//just the core the decision maker wants
	{	
		context->sampler[context->thisSample].time=getTicks ();
		context->sampler[context->thisSample].freq=i;
		context->sampler[context->thisSample].core=core;
		context->thisSample++;
		
		if (context->thisSample > NUMSAMPLES)
		{
			context->thisSample=NUMSAMPLES;
		}



		fseek (context->setFile[core],0,SEEK_SET);
		fprintf (context->setFile[core],"%d\n",context->availableFreqs[i]);
		fflush (context->setFile[core]);
		context->currentFreqs[core]=i;
	}
	
}

void destroyCpufreq (SFreqData * context)
{
	int j;
	int ret;
	char char_buff[1024]="";
	char num[10]="";
	FILE * dumpfile;
	int i;

	for (j=0;j<(context->numCores);j++)
	{	
		fclose (context->setFile[j]);//first close our file descriptors;
		//then set the govenor back to ondemand
		sprintf (char_buff,"echo ondemand > /sys/devices/system/cpu/cpu");
		sprintf (num,"%d",j);
		strcat (char_buff,num);
		strcat (char_buff,"/cpufreq/scaling_governor");
		ret = system (char_buff);

		if (WIFSIGNALED(ret) && (WTERMSIG(ret) == SIGINT || WTERMSIG(ret) == SIGQUIT))
		{
			Log_output (0,"System call failed! WHAT!?\n");
			exit (1);		
		}

		//dump our samples per core to a file
		sprintf (char_buff,"frequency_dump");
		strcat (char_buff,num);
		strcat (char_buff,".txt");
		dumpfile=fopen (char_buff,"a");
	
		fprintf (dumpfile,"Time, Core, Frequency\n");
		for (i=0;i<context->thisSample;i++ )
		{
			if (context->sampler[i].core==j)
			{		
				fprintf (dumpfile,"%lld, %d, %d\n",context->sampler[i].time,context->sampler[i].core,context->sampler[i].freq);	
			}		
		}
		fclose (dumpfile);
		

	}

	//free up our memory
	free (context->setFile);
	free (context->availableFreqs);
	free (context->currentFreqs);
	free (context);

	


}


