/*  This benchmark tries to characterize the latency to switch frequencies... it uses system() calls to change the frequency*/
#define _GNU_SOURCE
#include <sched.h>

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <getopt.h>
#include <math.h>
#include "rdtsc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//used to get all the p-states
int globalFrequency[20];
int num_frequency=0;
int assigned_cpu=0;
int num_repeats=1000;
FILE * freq_fp;

int * BigFatTable;

int numcores;
int opt;





static struct option option_list[] = {
{"c", 1, 0, 'c'},
{0,0,0,0}

};


static __inline__ unsigned long long getticks(void)
{
   unsigned long long ret;
   rdtscll(ret);
   return ret;
}


void setgov()
{
	char govstring[1024]="";
	sprintf(govstring,"echo userspace > /sys/devices/system/cpu/cpu%d/cpufreq/scaling_governor",assigned_cpu);
	system(govstring);
}
	
void Init_cpufreq(int core)
{
	char Freq[1024]="";
	char num[128];
	
	
	strcat(Freq,"/sys/devices/system/cpu/cpu");
	sprintf(num,"%d",core);
	strcat(Freq,num);
	strcat(Freq,"/cpufreq/scaling_setspeed");
	//printf("Trying to open file: %s\n",Freq);
	freq_fp = fopen (Freq,"w");
	if(freq_fp==NULL)
	{
		perror( "Error opening file" );
            	printf( "Error opening file: %s\n", strerror( errno ) );
		printf( "Perhaps you don't have sudo rights?\n");
	}
}

void Destroy_cpufreq()
{
	fclose(freq_fp);
}

	
	
void changeFreq(int i)
{
	fseek(freq_fp,0,SEEK_SET);
	fprintf(freq_fp,"%d\n",globalFrequency[i]);
	fflush(freq_fp);
	//printf("Frequency has changed to %d\n",globalFrequency[i]);
	
}


//Will use this to get the frequencies available from the kernel
void readFreq()
{
	
	
	char freq_available[1024] ;
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
		  if(assigned_cpu==0)
		  {
		  	//printf("Discovered P-state%d to be frequency %d\n",num_frequency,globalFrequency[num_frequency]);
		  }
		  num_frequency++;
    		  pch = strtok (NULL, " ");
  		 }

	}
	    	
	pclose (fp);
	
}



int main(int argc,char ** argv)
{


	
	unsigned long long start_time;
	unsigned long long end_time;
	unsigned long long memtime=0;
	unsigned long long cputime=0;
	float * VarianceTable;
	float clock;

	
   //parse options
       int c;
     
       opterr = 0;
     
       while ((c = getopt_long (argc, argv, "", option_list, NULL)) != -1)
         switch (c)
           {
           case 'c':
             assigned_cpu = atoi(optarg);
             break;
           case '?':
             if (optopt == 'c')
               fprintf (stderr, "Option -%c requires an argument.\n", optopt);
             else if (isprint (optopt))
               fprintf (stderr, "Unknown option `-%c'.\n", optopt);
             else
               fprintf (stderr,
                        "Unknown option character `\\x%x'.\n",
                        optopt);
             return 1;
           default:
             printf("Running on default CPU0\n");
           }


	
	//get the frequencies available from cpufreq
	readFreq();

	clock=globalFrequency[1];
	//make sure affinity is set high for the cpu we should run on

	cpu_set_t set_affinity;
	CPU_ZERO(&set_affinity);
	CPU_SET(assigned_cpu,&set_affinity);
	sched_setaffinity(getpid(),sizeof(cpu_set_t),&set_affinity);
	
	int current_cpu;
	current_cpu=sched_getcpu();

	Init_cpufreq(assigned_cpu);

	assert(assigned_cpu==current_cpu);

	//set govenor to userspace
	setgov();
	
	//allocate some tables
	BigFatTable=malloc(num_frequency*num_frequency*num_repeats*sizeof(* BigFatTable));
	VarianceTable=malloc(num_frequency*num_frequency*sizeof(* VarianceTable));

	int i,j,k;
	for(i=0;i<num_frequency; i++)//frequency to start from

	{
		for(j=0;j<num_frequency; j++)//frequency to go to
		{
			for(k=0;k<num_repeats; k++)//repititions to get a statistical average
			{
				changeFreq(i);
				start_time=getticks();
				changeFreq(j);
				end_time=getticks();
				BigFatTable[i*num_frequency*num_repeats+j*num_repeats+k]=end_time-start_time;
			}
		}
	}

	//now we process the tables
	printf("======================================\nLatency Table for Changing Frequencies in milliseconds\nGoing to\t");
	for(i=0;i<num_frequency; i++)//frequency to start from

	{
		
		printf("%d\t\t",globalFrequency[i]);
	}
	printf("\nComing from\n");
	for(i=0;i<num_frequency; i++)//frequency to start from

	{
		printf("%d\t\t",globalFrequency[i]);
		for(j=0;j<num_frequency; j++)//frequency to go to
		{
			float sum=0.0;
			float average=0;
			float squared;
			for(k=0;k<num_repeats; k++)//repititions to get a statistical average
			{
				sum+=BigFatTable[i*num_frequency*num_repeats+j*num_repeats+k];
			}
			average=(float)sum/(float)num_repeats;
			printf("%f,\t", average/clock);
			sum=0;
			for(k=0;k<num_repeats; k++)//now lets find the standard deviation while its in cache
			{
				squared=BigFatTable[i*num_frequency*num_repeats+j*num_repeats+k]-average;
				squared*=squared;
				sum+=squared;
			}
			VarianceTable[i*num_frequency+j]=sqrt(sum/num_repeats);

			
		}
		printf("\n");
	}

	//now we print out the table that we saved in Variance table now that latency table is printed
	printf("======================================\nStandard Deviation Table for Changing Frequencies in milliseconds\nGoing to\t");
	for(i=0;i<num_frequency; i++)//frequency to start from

	{
		
		printf("%d\t\t",globalFrequency[i]);
	}
	printf("\nStarting from\n");
	for(i=0;i<num_frequency; i++)//frequency to start from

	{
		printf("%d\t\t",globalFrequency[i]);
		for(j=0;j<num_frequency; j++)//frequency to go to
		{
			
			printf("%f,\t",VarianceTable[i*num_frequency+j]/clock);

			
		}
		printf("\n");
	}

		
	
	
	

}	
	

	
	
