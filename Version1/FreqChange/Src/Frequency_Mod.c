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
#include <sched.h>
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



/**
 * @brief Read the available frequencies in the file and check the validity of the current frequency
 * @param context context the context we wish to use
 * @param procId id of the the core*/
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
		return -1;
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
			return -1;
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
	
	int i, j;
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
		return NULL;
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
					return NULL;
				}
				pch = strtok (NULL, " ");
			}
		}
		else
		{
			printf ("fopen call failed somehow 2\n");
			return NULL;
		}
	}
	fclose (fp);


	/*now allocate a context with all my mallocs */

	handle= malloc ( sizeof(* handle));
	assert (handle!=NULL);
	handle->setFile=malloc (sizeof (*(handle->setFile)));
	assert (handle->setFile!=NULL);
	handle->currentFreqs=malloc (sizeof (*(handle->currentFreqs)));
	assert (handle->currentFreqs!=NULL);
	handle->availableFreqs=malloc (sizeof (*(handle->availableFreqs))*num_frequency );
	

	handle->numFreq=num_frequency;
	handle->idCore=sched_getcpu ();
	handle->freqMax=globalFrequency[0];
	handle->freqMin=globalFrequency[num_frequency-1];
	handle->thisSample=0;

	handle->freqTrack = malloc (num_frequency * sizeof (handle->freqTrack));

	for(i = 0; i < num_frequency; i++)
	{
		handle->freqTrack[i] = 0;
	}

	for(i=0; i<num_frequency;i++)
	{
		handle->availableFreqs[i]=globalFrequency[i];//copy frequencies into data structure
	}

	//first set the govenor to userspace
	sprintf (char_buff,"/sys/devices/system/cpu/cpu%d/cpufreq/scaling_governor", handle->idCore);

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
	sprintf (char_buff,"/sys/devices/system/cpu/cpu%d/cpufreq/scaling_setspeed", handle->idCore);
	fp = fopen (char_buff,"w");

	if (fp==NULL)
	{
		perror ( "Error opening file" );
		printf ( "Error opening file: %s\n", strerror( errno ) );
		printf ( "Perhaps you don't have sudo rights?\n");
	}

	handle->setFile=fp;//file * opened so now we place it in the context



	handle->currentFreqs = helperReadFreq (handle, handle->idCore);

	return handle;	
}


int readFreq (SFreqData * context)
{
	return context->currentFreqs;
}


void changeFreq (SFreqData * context, int i)
{
	context->sampler[context->thisSample].time=getTicks ();
	context->sampler[context->thisSample].freq=i;
	context->sampler[context->thisSample].window=context->windowTrack;
	context->thisSample++;

	if (context->thisSample > NUMSAMPLES)
	{
		context->thisSample=NUMSAMPLES;
	}

	context->freqTrack[i]++;

	fseek (context->setFile,0,SEEK_SET);
	fprintf (context->setFile,"%d\n",context->availableFreqs[i]);
	fflush (context->setFile);
	context->currentFreqs=i;

	
}

void destroyCpufreq (SFreqData * context)
{
	int j;
	int ret;
	char char_buff[1024]="\0";
	char num[10]="\0";
	char pid[10]="\0";
	FILE * dumpfile;
	int i;
	

	fclose (context->setFile);//first close our file descriptors;
	//then set the govenor back to ondemand
	strcpy (char_buff,"echo ondemand > /sys/devices/system/cpu/cpu");
	sprintf (num,"%d",context->idCore);
	strcat (char_buff,num);
	strcat (char_buff,"/cpufreq/scaling_governor");
	ret = system (char_buff);

	if (WIFSIGNALED(ret) && (WTERMSIG(ret) == SIGINT || WTERMSIG(ret) == SIGQUIT))
	{
		Log_output (0,"System call failed! WHAT!?\n");
		exit (1);
	}

	strcpy(char_buff,"");
	if(getenv("REST_OUTPUT") != NULL)
	{
		strcpy(char_buff,getenv("REST_OUTPUT"));
		strcat(char_buff, "/");
	}

	//dump our samples per core to a file
	strcat (char_buff,"frequency_dump");
	strcat (char_buff,num);
	strcat (char_buff,".txt");
	dumpfile=fopen (char_buff,"a");

	if(dumpfile != NULL)
	{
		fprintf (dumpfile,"Time\t\t\tCore\tFreq\tWindow\n");

		for (i=0;i<context->thisSample;i++ )
		{
			fprintf (dumpfile,"%lld\t%d\t%d\t%d\n",context->sampler[i].time,context->idCore,context->sampler[i].freq,context->sampler[i].window);
		}
		fclose (dumpfile);
	}
	else
	{
		Log_output(15, "REST_OUTPUT: failed to open directory: %s sampler stats output aborted\n",char_buff );
	}
	
	strcpy(char_buff,"");

	if(getenv("REST_OUTPUT") != NULL)
	{
		strcpy (char_buff, getenv("REST_OUTPUT"));
		strcat(char_buff, "/");
	}
	//dump our frequencies per core to a file
	strcat (char_buff,"core_frequency_count");
	strcat (char_buff,num);
	dumpfile=fopen (char_buff,"a");

	if(dumpfile != NULL)
	{
		
		fprintf (dumpfile,"Core\tFreq\tFreq changes\n");

		for (i=0;i<context->numFreq; i++ )
		{
			fprintf (dumpfile,"%d\t%d\t%ld\n",context->idCore,i,context->freqTrack[i]);
		}
		fclose (dumpfile);
	}
	else
	{
		Log_output(15, "REST_OUTPUT: failed to open directory: %s frequency per core output aborted\n",char_buff);
	}


	//free up our memory
	free(context->freqTrack), context->freqTrack = NULL;
	free (context->setFile); context->setFile=NULL;
	free (context->availableFreqs); context->availableFreqs=NULL;
	free (context->currentFreqs); context->currentFreqs=NULL;
	free (context); context=NULL;

}


