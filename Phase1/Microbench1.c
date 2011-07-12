
#include "Microbench1.h"

//used to get all the p-states
int globalFrequency[20];
int num_frequency=0;
int assigned_cpu=0;
int expected_loop_interval=1000;//this variable is for anticapted ms for each loops interval
int numcores;
int opt;
double energy_start,energy_stop;
void * dlinit_func;
void * dlclose_func;
void * dlstart_func;
void * dlstop_func;

//class __attribute__((aligned(16))) 
double DummyVec1[12]={1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0};
double DummyVec2[12]={1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0};
//a 1 gig vector definitely wont fit in cache
double BigVec[MEM_BOUND_FOOTPRINT];
static unsigned int g_seed; 


//These ratios are guedron specific... the first is the ratio of the loop bounds.. the second is the product of inner and outer loops
long cpu_2_mem_ratio=3793110;
long updown_mem_product=5000;


long outer_loop_bound;
long cpu_iters=(268435456); //these are the old default values
long mem_iters=(92);

static struct option option_list[] = {
{"r", 0, 0, 'r'},
{"p", 0, 0, 'p'},
{"m", 0, 0, 'm'},
{"c", 1, 0, 'c'},
{"i", 1, 0, 'i'},
{0,0,0,0}

};


static __inline__ unsigned long long getticks(void)
{
   unsigned long long ret;
   rdtscll(ret);
   return ret;
}


   
inline void fast_srand() 
    
{ 
    
	g_seed = 14545265746874; 
    
} 
  
   
inline int fastrand() 
    
{ 
    
	g_seed = (214013*g_seed+2531011); 
    
	return (g_seed>>16)&0x7FFF; 
    
} 


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
		  num_frequency++;
    		  pch = strtok (NULL, " ");
  		}

	}
	    	
	pclose (fp);
	
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
             expected_loop_interval = atoi(optarg);
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

	//make sure affinity is set high for the cpu we should run on

	cpu_set_t set_affinity;
	CPU_ZERO(&set_affinity);
	CPU_SET(assigned_cpu,&set_affinity);
	sched_setaffinity(getpid(),sizeof(cpu_set_t),&set_affinity);
	
	int current_cpu;
	current_cpu=sched_getcpu();

	assert(assigned_cpu==current_cpu);
	//link in the power meter functions if you're cpu 0

	if(assigned_cpu==0)
	{
		
		void * dl;	
	
		dl = dlopen ("../power/timer.so", RTLD_NOW);
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
		start_time=getticks();
		
	}
	
	//signal(SIGINT,safe_die);
	//fast_srand();//was used when membound loop needed random variables
	 
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
			if(doing_rest)
			{
				changeFreq(current_cpu,1);
			}
			cputime-=getticks();
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

		cputime+=getticks();
		}
		//printf("Switching to Memory Bound on CPU %d\n",current_cpu);
		//Now we do something mem bound
		
		
		
		if(!proc_bound_only)
		{
			if(doing_rest)
			{ 
				changeFreq(current_cpu,num_frequency-1);//we switch to the lowest frequency
			}
			memtime-=getticks();
			int j;
			int index;
			int k;
			double temp[8];
			for(j=0;j<MEM_BOUND_ITER;j++)
			{
				memcpy(&BigVec[0],&BigVec[MEM_BOUND_FOOTPRINT/2],MEM_BOUND_FOOTPRINT/2*sizeof(*BigVec));
				
			}
			memtime+=getticks();
		}
		
	}
	

	if(assigned_cpu==0)
	{

		end_time=getticks();
		unsigned long long (* close_func)(void)=dlclose_func;
		double (* stop_func)(void)=dlstop_func;	
		energy_stop=stop_func();
		close_func();
		//printf("Cpu ticks is %llu, memticks is %llu, total is %llu\n",cpuend-cpustart,memend-memstart,end_time-start_time);
		printf("=============\nTotal energy: %f Wh\nTotal time: %f seconds\n\t%4f percent: Processor Bounded time\n\t"
				"%4f percent: Memory Bounded time\n",energy_stop-energy_start,
			((float)(end_time-start_time))/(((float)globalFrequency[1])*1000.0),
			(cputime)/((float)(end_time-start_time))*100.0, (memtime)/((float)(end_time-start_time))*100.0 );
		
		printf("Average loop latency for Memory was %f ms and CPU was %f ms\n Overhead for changing frequencies and misc:"
				" %f percent or %f ms per frequency change\n For this given ms interval we did %f MBs of data transfers and %f synthetic Giga Floating Ops (not per second)\nWe report this because the benchmark varies this amount of work since loop bounds must be discrete\n=================\n",
				(float)memtime/(float)(globalFrequency[1]*NUM_UP_DOWN),(float)cputime/(float)(globalFrequency[1]*NUM_UP_DOWN),
				((float)(end_time-start_time)-(memtime+cputime))*100 /(float)(end_time-start_time),
				 (float)((end_time-start_time)-(memtime+cputime))/(float)(2*NUM_UP_DOWN*globalFrequency[1]),
				 (NUM_UP_DOWN*MEM_BOUND_ITER*MEM_BOUND_FOOTPRINT*.5*8*12/1048576.0),
				  //copy second half to first half... 8 bytes in a double... 12 cpus on Guedron... and divide by megabytes
				 (NUM_UP_DOWN*CPU_BOUND_ITER*4.0*12/1000000000));
		
		//energy_stop=(*dlstop_func)();
		//(*dlclose_func)();
	}

	
	
	

}	
	

	
	
