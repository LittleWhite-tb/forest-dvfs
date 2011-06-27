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


#include "Frequency_Mod.h"


//used to store all the p-states
int globalFrequency[NUM_STATIC_FREQ];

int num_frequency=0;

void changeFreq(int core, int i)
{
	char Freq[1024]="";
	char userSpace[1024]="";
	char num[128];
	getAllAvailableFreq();
	if(core == -1)
	{
		int k,j;
		for(j=0;j<12;j++)
		{
			for( k=0;k<1024;k++)
			{
				Freq[k]='\0';
				userSpace[k]='\0';
			}
			strcat(userSpace,"echo userspace > /sys/devices/system/cpu/cpu");
			sprintf(num,"%d",j);
			strcat(userSpace,num);
			strcat(userSpace,"/cpufreq/scaling_governor");
			
			system(userSpace);
			
			sprintf(num,"%d",globalFrequency[i]);
			strcat(Freq,"echo ");
			strcat(Freq, num);
			strcat(Freq," > /sys/devices/system/cpu/cpu");
			sprintf(num,"%d",j);
			strcat(Freq,num);
			strcat(Freq,"/cpufreq/scaling_setspeed");
			
			system(Freq);
			
			printf("Changing my freq on core (%d) to (%d)\n",j,globalFrequency[i]);
		}
	}
	else
	{	
		strcat(userSpace,"echo userspace > /sys/devices/system/cpu/cpu");
		sprintf(num,"%d",core);
		strcat(userSpace,num);
		strcat(userSpace,"/cpufreq/scaling_governor");
		
		system(userSpace);
		
		sprintf(num,"%d",globalFrequency[i]);
		strcat(Freq,"echo ");
		strcat(Freq, num);
		strcat(Freq," > /sys/devices/system/cpu/cpu");
		sprintf(num,"%d",core);
		strcat(Freq,num);
		strcat(Freq,"/cpufreq/scaling_setspeed");
		
		system(Freq);
		
		printf("Changing my freq on core (%d) to (%d)\n",core,globalFrequency[i]);
	}
}

//Will use this to get the frequencies available from the kernel
FreqData *getAllAvailableFreq()
{
	int i;
	
	
	char freq_available[1024]="";
	
	FILE * fp;
  	fp = popen ("cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies","r");
	
	if (fp==NULL)
	{
		printf("Failed to open cpufreq datafile\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		fgets(freq_available, sizeof(freq_available), fp); 
		char * pch;
		pch = strtok (freq_available," ");
		while (pch != NULL)
		{
			globalFrequency[num_frequency]=atoi(pch);
			
			if(globalFrequency[num_frequency]==0)
			{
				break;
			}
			num_frequency++;
			pch = strtok (NULL, " ");
		}
	}
	FreqData *tmp = malloc(sizeof(FreqData));
	tmp->availablefreqs = (int*) malloc(num_frequency * sizeof(int));
	for(i=0;i<num_frequency;i++)
	{
		tmp->availablefreqs[i] = globalFrequency[i];
		printf("Availeble[%d] = %d\n",i,tmp->availablefreqs[i]);
	}
	
	tmp->freqMax = tmp->availablefreqs[0];
	tmp->freqMin = tmp->availablefreqs[num_frequency-1];
	tmp->numberOfFreq = num_frequency;
	pclose (fp);
	return tmp;
	
}

int getTheFreqById(int procId)
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
		fgets(currentFreq, sizeof(currentFreq), fp);
		curFreq = atoi( currentFreq);
		printf("current freq (%d) on core (%d)\n",curFreq,procId);
	}
	pclose (fp);
	return curFreq;
}

int main()
{
	changeFreq(-1, 3);
	return 0;
}
