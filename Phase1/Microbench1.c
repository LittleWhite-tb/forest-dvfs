#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CPU_BOUND_ITER (818822656)

//corresponds to 1 gig of memory for the matrix scalar add
#define MEM_BOUND_ITER 4217728

//used to get all the p-states
int globalFrequency[20];
int numcores;
int opt;

double DummyVec[4]={1.0,1.0,1.0,1.0};
//a 1 gig vector definitely wont fit in cache
double BigVec[MEM_BOUND_ITER];

void changeFreq(int core, int i)
{
	char Freq[1024]="";
	char num[128];
	sprintf(num,"%d",globalFrequency[i]);
	strcat(Freq,"echo ");
	strcat(Freq, num);
	strcat(Freq," > /sys/devices/system/cpu/cpu");
	sprintf(num,"%d",core);
	strcat(Freq,num);
	strcat(Freq,"/cpufreq/scaling_setspeed");
	system(Freq);
	printf("Changing my frequency on core %d to %d\n",core,globalFrequency[i]);
}


//Will use this to get the frequencies available... right now we'll hardcode the array for guedron
void readFreq()
{
	
	/* For guedron 
	globalFrequency[0]=2661000;
	globalFrequency[1]=2660000;
	globalFrequency[2]=2527000;
	globalFrequency[3]=2394000;
	globalFrequency[4]=2261000;
	globalFrequency[5]=2128000;
	globalFrequency[6]=1995000;
	globalFrequency[7]=1862000;
	globalFrequency[8]=1729000;
	globalFrequency[9]=1596000;

	*/

	/*For my laptop*/
	globalFrequency[0]=2533000;            
	globalFrequency[1]=2399000;
	globalFrequency[2]=2266000;
	globalFrequency[3]=2133000;
	globalFrequency[4]=1999000;
	globalFrequency[5]=1866000;
	globalFrequency[6]=1733000;
	globalFrequency[7]=1599000;
	globalFrequency[8]=1466000;
	globalFrequency[9]=1333000;
	globalFrequency[10]=1199000;
	globalFrequency[11]=1066000;
	globalFrequency[12]=933000;

	/*
	*/


	
	
	/*
	char freq_available[1024] ;
	FILE * fp;
	size_t len = 0;
	ssize_t read;
  	fp = fopen ("/sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequuencies","r");
  	if (fp)
 	{
		printf("Failed to open cpufreq datafile\n");
		exit(EXIT_FAILURE);
	}
	
    	else
	{
		//read = fgets(freq_available, sizeof(freq_available), fp); 
        	printf("Retrieved line of length %zu :\n", read);
        	//printf("%s", freq_available);
	}
	    	
	fclose (fp);
	*/
}



int main(int argc,char ** argv)
{


	int ondemand=0;
	int assigned_cpu=0;
	char Rest_root_dir[1024]="../";
	readFreq();

	
   //parse options
       int c;
     
       opterr = 0;
     
       while ((c = getopt (argc, argv, "oc:")) != -1)
         switch (c)
           {
           case 'c':
             assigned_cpu = atoi(optarg);
             break;
           case 'o':
             ondemand = 1;
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


	//make sure affinity is set high for the cpu we should run on
	
	//
	cpu_set_t set_affinity;
	CPU_ZERO(&set_affinity);
	CPU_SET(assigned_cpu,&set_affinity);
	sched_setaffinity(getpid(),sizeof(cpu_set_t),&set_affinity);
	
	int current_cpu;
	current_cpu=sched_getcpu();
	
	

	printf("Starting up benchmark on CPU %d\n",current_cpu);

	//Now we do something cpu bound
	if(!ondemand)
	{
		changeFreq(current_cpu,0);
	}	
	int i;	
	for(i=0;i<CPU_BOUND_ITER;i++)
	{
		DummyVec[0]=DummyVec[0]*DummyVec[0];
		DummyVec[1]=DummyVec[1]*DummyVec[1];
		DummyVec[2]=DummyVec[2]*DummyVec[2];
		DummyVec[3]=DummyVec[3]*DummyVec[3];
	}

	printf("Switching to Memory Bound on CPU %d\n",current_cpu);
	//Now we do something mem bound
	
	if(!ondemand)
	{ 
		changeFreq(current_cpu,6);
	}

	
	int j;
	for(j=0;j<10;j++)
	{
		for(i=0;i<MEM_BOUND_ITER;i++)
		{
			BigVec[i]=BigVec[i]+1.0;
		}
	}
	
	

}	
	

	
	
