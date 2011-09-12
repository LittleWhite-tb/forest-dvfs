//This define and include must come first for the macros CPU_ZERO and CPU_SET to be defined properly  
#define _GNU_SOURCE
#include <sched.h>


#include "Frequency_Mod.h"
#include "Microbench1.h"
#include "Rest.h"
#include "../power/timer.h"
#include "rdtsc.h"

#include <assert.h>
#include <ctype.h>
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//#define ORACLE_MODE

//#define CHANGETOGETHER




/*====================Hacked to get timing info===========*/

#define NUMSAMP 20000

struct timestamp
{
	long long time;
	int bound; //1= switching to compute bound 2= switching to memory bound
};



//class __attribute__((aligned(16))) 
double DummyVec1[12]={1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0};
double DummyVec2[12]={1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0};
//a 1 gig vector definitely wont fit in cache
double BigVec[MEM_BOUND_FOOTPRINT*32];
static unsigned int g_seed; 


static struct option option_list[] = {
{"r", 0, 0, 'r'},
{"p", 0, 0, 'p'},
{"m", 0, 0, 'm'},
{"c", 1, 0, 'c'},
{"i", 1, 0, 'i'},
{"d", 1, 0, 'd'},
{0,0,0,0}

};


static __inline__ unsigned long long getTicks(void)
{
   unsigned long long ret;
   rdtscll(ret);
   return ret;
}

   
static __inline__ void fastSrand() 
    
{ 
    
	g_seed = 14545265746874; 
    
} 
  
   
static __inline__ int fastRand() 
    
{ 
    
	g_seed = (214013*g_seed+2531011); 
    
	return (g_seed>>16)&0x7FFF; 
    
} 




int main(int argc,char ** argv)
{


	int doing_rest=1;
	int mem_bound_only=0;
	int proc_bound_only=0;
	unsigned long long start_time;
	unsigned long long end_time;
	unsigned long long memtime=0;
	unsigned long long cputime=0;

	//used for recording when we transition between compute and memory bound
	struct timestamp samples[NUMSAMP];
	int mysample=0;

	//used for manually changing frequencies
	SFreqData * FreqStuff;

	int assigned_cpu=0;
	int choose_dm=1;
	float expected_loop_interval=1000;//this variable is for anticapted ms for each loops interval
	int numcores;
	int opt;

	//used by power meter to get functions dynamically and use them
	double energy_start,energy_stop;
	void * dlinit_func;
	void * dlclose_func;
	void * dlstart_func;
	void * dlstop_func;


	//These ratios are guedron specific... the first is the ratio of the loop bounds.. the second is the product of inner and outer loops
	long cpu_2_mem_ratio=3793110/85;
	long updown_mem_product=5000*32;


	long outer_loop_bound;
	long cpu_iters=(268435456); //these are the old default values
	long mem_iters=(92);
	
   //parse options
       int c;
     
       opterr = 0;
     
       while ((c = getopt_long (argc, argv, "", option_list, NULL)) != -1)
         switch (c)
           {
           case 'c':
             assigned_cpu = atoi(optarg);
             break;
	    case 'i':
             expected_loop_interval = atof(optarg);
             break;
           case 'r':
             doing_rest = 0;
             break;
	   case 'm':
             mem_bound_only = 1;
             break;
	   case 'p':
             proc_bound_only = 1;
             break; 
       case 'd':
             choose_dm = atoi(optarg);
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


    //Set logger
    Log_setOutput (stderr);
	
    //now start!
    Log_output (0, "REST Start\n");


	//make sure affinity is set high for the cpu we should run on

	cpu_set_t set_affinity;
	CPU_ZERO(&set_affinity);
	CPU_SET(assigned_cpu,&set_affinity);
	sched_setaffinity(getpid(),sizeof(cpu_set_t),&set_affinity);
	
	int current_cpu;
	current_cpu=sched_getcpu();

	assert (assigned_cpu==current_cpu);
	//link in the power meter functions if you're cpu 0
/*	if(assigned_cpu==0)
	{
		
		void * dl;	
	
		dl = dlopen ("../../../power/timer.so", RTLD_NOW);
		if (dl == NULL)
		{
		    printf("The power meter dynamic library could not be linked... please locate timer.so\n");
		    fprintf (stderr, "%s\n", dlerror());
		    exit(EXIT_FAILURE);
		}
	
		dlinit_func=dlsym(dl,"timer_init");
		dlstart_func=dlsym(dl,"timer_start");
		dlstop_func=dlsym(dl,"timer_stop");
		dlclose_func=dlsym(dl,"timer_close");
		unsigned long long (* init_func)(void)=dlinit_func;
		
		double (* start_func)(void)=dlstart_func;
		
		init_func();
		energy_start=start_func();
		start_time=getTicks();
		
	}
*/
	

	//touch the file so the script spawns the children
	if(assigned_cpu==0)
	{
		FILE * fp;
		fp=fopen("/tmp/rest","w");
		fclose(fp);
	}

	#ifndef ORACLE_MODE


	if(doing_rest)    
	{
		if(choose_dm==1)
		{
			
			RestInit(REST_T_PROFILER, REST_NAIVE_DM, REST_FREQ_CHANGER);
		}
		else if (choose_dm==2)
		{
			RestInit(REST_T_PROFILER, REST_BRANCHPREDICT_DM, REST_FREQ_CHANGER);
		}
		else if (choose_dm==3)
		{
			RestInit(REST_T_PROFILER, REST_MARKOVPREDICT_DM, REST_FREQ_CHANGER);
		}
		else
		{
			Log_output (0, "-d option was incorrect choose 1 or 2\n");
			exit (1);
		}
	}	
	#else
	FreqStuff= initCpufreq();
	#endif
	
	//we seed random number generator
	fastSrand();
 
	//calculate loop bounds
	NUM_UP_DOWN=(60*1000)/(expected_loop_interval*2);//60 second in ms divded by the expected loop interval (mult by 2 because we do compute and memory loops)
	//once we have number of outer loops we use the product to find the memory loop bounds
	MEM_BOUND_ITER=round((float)updown_mem_product/(float)NUM_UP_DOWN);
	CPU_BOUND_ITER=MEM_BOUND_ITER*cpu_2_mem_ratio;
	//now start!


	int i;
	for(i=0;i<NUM_UP_DOWN; i++)

	{
		//fprintf (stderr, "Iter %d out of %d\n", i, NUM_UP_DOWN);

		//Now we do something cpu bound
		
		
		if(!mem_bound_only)
		{	

			
			#ifdef ORACLE_MODE			
			if(doing_rest)
			{
				#ifdef CHANGETOGETHER
				if(assigned_cpu==0)
				{
				   changeFreq(FreqStuff,-1,1);
	
				}	
				#else
				changeFreq(FreqStuff,assigned_cpu,1);
				#endif
			}
			#endif
			
			samples[mysample].time=getTicks();
			samples[mysample].bound=1;
			mysample++;
			mysample=(mysample>=NUMSAMP)?NUMSAMP-1:mysample;

			
			cputime-=getTicks();
			int j;	
			double temp[4];
			//force everything to stay in registers and unroll
			temp[0]=DummyVec1[0];
			temp[1]=DummyVec1[1];
			temp[2]=DummyVec1[2];
			temp[3]=DummyVec1[3];
			for(j=0;j<CPU_BOUND_ITER;j++)
			{
				temp[0]=temp[0]*temp[0];
				temp[1]=temp[1]*temp[1];
				temp[2]=temp[2]*temp[2];
				temp[3]=temp[3]*temp[3];
				
			}
			DummyVec2[0]=temp[0];
			DummyVec2[1]=temp[1];
			DummyVec2[2]=temp[2];
			DummyVec2[3]=temp[3];

		cputime+=getTicks();
		}
		//printf("Switching to Memory Bound on CPU %d\n",current_cpu);
		//Now we do something mem bound
		
		
		
		if(!proc_bound_only)
		{
			
			#ifdef ORACLE_MODE			
			if(doing_rest)
			{
				#ifdef CHANGETOGETHER
				if(assigned_cpu==0)
				{
				   changeFreq(FreqStuff,-1,FreqStuff->numFreq-1);
	
				}	
				#else
				changeFreq(FreqStuff,assigned_cpu,FreqStuff->numFreq-1);
				#endif
			}
			#endif
		
			samples[mysample].time=getTicks();
			samples[mysample].bound=2;
			mysample++;
			mysample=(mysample>=NUMSAMP)?NUMSAMP-1:mysample;
			
			memtime-=getTicks();
			int j;
			int index;
			int k;
			double temp[8];
			for(j=0;j<MEM_BOUND_ITER;j++)
			{
				
				memcpy(&BigVec[fastRand()%(MEM_BOUND_FOOTPRINT*31)],&BigVec[fastRand()%(MEM_BOUND_FOOTPRINT*31)],
					MEM_BOUND_FOOTPRINT/2*sizeof(*BigVec));
				
			}
			memtime+=getTicks();
		}
		
	}

/*
	if(assigned_cpu==0)
	{

		end_time=getTicks();
		unsigned long long (* close_func)(void)=dlclose_func;
		double (* stop_func)(void)=dlstop_func;	
		energy_stop=stop_func();
		close_func();
		//printf("Cpu ticks is %llu, memticks is %llu, total is %llu\n",cpuend-cpustart,memend-memstart,end_time-start_time);
		printf("=============\nTotal energy: %f Wh\nTotal time: %f seconds\n\t%4f percent: Processor Bounded time\n\t"
				"%4f percent: Memory Bounded time\n",energy_stop-energy_start,
			((float)(end_time-start_time))/(((float)2660000)*1000.0),
			(cputime)/((float)(end_time-start_time))*100.0, (memtime)/((float)(end_time-start_time))*100.0 );
		
		printf("Average loop latency for Memory was %f ms and CPU was %f ms\n Overhead for changing frequencies and misc:"
				" %f percent or %f ms per frequency change\n For this given ms interval we did %f MBs of data transfers and %f synthetic Giga Floating Ops (not per second)\nWe report this because the benchmark varies this amount of work since loop bounds must be discrete\n=================\n",
				(float)memtime/(float)(2660000*NUM_UP_DOWN),(float)cputime/(float)(2660000*NUM_UP_DOWN),
				((float)(end_time-start_time)-(memtime+cputime))*100 /(float)(end_time-start_time),
				 (float)((end_time-start_time)-(memtime+cputime))/(float)(2*NUM_UP_DOWN*2660000),
				 (NUM_UP_DOWN*MEM_BOUND_ITER*MEM_BOUND_FOOTPRINT*.5*8*12/1048576.0),
				  //copy second half to first half... 8 bytes in a double... 12 cpus on Guedron... and divide by megabytes
				 (NUM_UP_DOWN*CPU_BOUND_ITER*4.0*12/1000000000));
		
		//energy_stop=(*dlstop_func)();
		//(*dlclose_func)();
	}
*/

	    #ifndef ORACLE_MODE
	    if(doing_rest)
	    {    	
	    	RestDestroy ();
	    }
	    #else
	    destroyCpufreq(FreqStuff);
	    #endif


   //dump my sample for analysis
   char filename[512]="timing_dump_core";
   char temp[16];
    sprintf(temp,"%d",assigned_cpu);
    strcat(filename,temp);
    strcat(filename, ".txt");
   FILE* dump=fopen(filename,"w");
   
    int x;
    for(x=0;x<mysample;x++)
    {
	if(samples[x].bound==1)
	{fprintf(dump,"%lld, compute bound, %d\n",samples[x].time,assigned_cpu);}
	else if (samples[x].bound==2)
	{fprintf(dump,"%lld, memory bound, %d\n",samples[x].time,assigned_cpu);}
	else
	{
		printf("Something Screwed up!\n");
	}
    }
    fclose(dump);
    


    Log_output (0, "REST Stop\n");
 
    return EXIT_SUCCESS;
	
	

}	
	

	
	
