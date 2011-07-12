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

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include "Frequency_Mod.h"



//this function is implemented with a high overhead! So we Only read the frequency once and then store it somewhere.
int helperReadFreq (int procId)
{
	char bashCmd[1024]="";
	char core[3];
	
	strcat(bashCmd,"cat /sys/devices/system/cpu/cpu");
	sprintf(core,"%d",procId);
	strcat(bashCmd,core);
	strcat(bashCmd,"/cpufreq/cpuinfo_cur_freq");
	
	char currentFreq[10];
	int curFreq;
	FILE * fp;
	
  	fp = popen (bashCmd,"r");
  	if (fp==NULL)
	{
		printf("Failed to open cpufreq datafile\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		if(fgets(currentFreq, sizeof(currentFreq), fp))
		{
			curFreq = atoi( currentFreq);
		}
		else
		{
			printf("popen call failed somehow\n");
			exit(EXIT_FAILURE);
		}
		
	}
	pclose (fp);
	return curFreq;
	
}


SFreqData * init_cpufreq (void)
{

	SFreqData * handle;
	
	char num[128];
	int i;
	char char_buff[1024]="";
	int globalFrequency[NUM_STATIC_FREQ];//this is a place to temporarily store all the values until we alloc on the heap
	int num_frequency=0;//another temp holder
	int num_cores=0;
	

	//find all the frequencies and record them
	FILE * fp;
  	fp = popen ("cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies","r");
	
	if (fp==NULL)
	{
		printf("Failed to open cpufreq datafile\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		if(fgets(char_buff, sizeof(char_buff), fp)) 
		{
			char * pch;
			pch = strtok (char_buff," ");
			while (pch != NULL)
			{
				globalFrequency[num_frequency]=atoi(pch);
				
				if(globalFrequency[num_frequency]==0)
				{
					break;
				}
				num_frequency++;
				if(num_frequency == NUM_STATIC_FREQ)
				{
					printf("Ran out of allocation for frequencies, set NUM_STATIC_FREQ higher\n");
					exit(EXIT_FAILURE);
				}
				pch = strtok (NULL, " ");
			}
		}
		else
		{
			printf("popen call failed somehow\n");
			exit(EXIT_FAILURE);
		}
	}
	pclose(fp);

	//find number of cores
	fp=popen("cat /proc/cpuinfo | grep \"processor\" | wc -l", "r");
	if (fp==NULL)
	{
		printf("Failed to find number of cpus in /proc/cpuinfo\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		if(fgets(char_buff, sizeof(char_buff), fp))
		{
			char * pch;
			pch = strtok (char_buff," ");
			num_cores=atoi(pch);
			assert(num_cores>0);
			pclose(fp);	
		}	
		else
		{
			printf("popen call failed somehow\n");
			exit(EXIT_FAILURE);
		}	
	}
	/*now allocate a context with all my mallocs
	*/
	
	
	handle= malloc( sizeof(* handle));
	assert(handle!=NULL);
	handle->setFile=malloc(sizeof(*(handle->setFile))*num_cores);
	assert(handle->setFile!=NULL);
	handle->currentFreqs=malloc(sizeof(*(handle->currentFreqs))*num_cores);
	assert(handle->currentFreqs!=NULL);
	handle->availableFreqs=malloc (sizeof(*(handle->availableFreqs))*num_frequency );

	handle->numFreq=num_frequency;
	handle->numCores=num_cores;
	handle->freqMax=globalFrequency[0];
	handle->freqMin=globalFrequency[num_frequency-1];
	for(i=0; i<num_frequency;i++)
	{
		handle->availableFreqs[i]=globalFrequency[i];//copy frequencies into data structure
	}

	for(i=0; i<num_cores;i++)
	{
		//first set the govenor to userspace
		sprintf(char_buff,"echo userspace > /sys/devices/system/cpu/cpu");
		sprintf(num,"%d",i);
		strcat(char_buff,num);
		strcat(char_buff,"cpufreq/scaling_governor");
		system(char_buff);
		//now open the file descriptor
		sprintf(char_buff,"/sys/devices/system/cpu/cpu");
		sprintf(num,"%d",i);
		strcat(char_buff,num);
		strcat(char_buff,"/cpufreq/scaling_setspeed");
		fp = fopen (char_buff,"w");

		if(fp==NULL)
		{
			perror( "Error opening file" );
        	    	printf( "Error opening file: %s\n", strerror( errno ) );
			printf( "Perhaps you don't have sudo rights?\n");
		}
		handle->setFile[i]=fp;//file * opened so now we place it in the context
	}

	for(i=0; i<num_cores;i++)
	{
		handle->currentFreqs[i]=helperReadFreq(i);
	}	

	return handle;	
}


int readFreq (SFreqData * context, int procId)
{
	return context->currentFreqs[procId];
}


void changeFreq (SFreqData * context, int i, int core)
{

	if(core == -1)//this means change all the cores
	{
		int j;
		for(j=0;j<(context->numCores);j++)
		{	
			fseek(context->setFile[j],0,SEEK_SET);
			fprintf(context->setFile[j],"%d\n",context->availableFreqs[i]);
			fflush(context->setFile[j]);
			context->currentFreqs[j]=i;
		}
	}
	else//just the core the decision maker wants
	{	
		fseek(context->setFile[core],0,SEEK_SET);
		fprintf(context->setFile[core],"%d\n",context->availableFreqs[i]);
		fflush(context->setFile[core]);
		context->currentFreqs[core]=i;
	}
	
}

void destroy_cpufreq (SFreqData * context)
{
	int j;
	char char_buff[1024]="";
	char num[10]="";
	for(j=0;j<(context->numCores);j++)
	{	
		fclose(context->setFile[j]);//first close our file descriptors;
		//then set the govenor back to ondemand
		sprintf(char_buff,"echo ondemand > /sys/devices/system/cpu/cpu");
		sprintf(num,"%d",j);
		strcat(char_buff,num);
		strcat(char_buff,"cpufreq/scaling_governor");
		system(char_buff);
	}
	free(context->setFile);
	free(context->availableFreqs);
	free(context->currentFreqs);
	free(context);
}


