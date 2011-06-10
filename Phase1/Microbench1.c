

#include "Microbench1.h"

//used to get all the p-states
int globalFrequency[20];
int num_frequency=0;
int assigned_cpu=0;
int numcores;
int opt;
double energy_start,energy_stop;
void * dlinit_func;
void * dlclose_func;
void * dlstart_func;
void * dlstop_func;

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
		  	printf("Discovered P-state%d to be frequency %d\n",num_frequency,globalFrequency[num_frequency]);
		  }
		  num_frequency++;
    		  pch = strtok (NULL, " ");
  		 }

	}
	    	
	pclose (fp);
	
}



int main(int argc,char ** argv)
{


	int ondemand=0;
	

	
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


	//get the frequencies available from cpufreq
	readFreq();

	//make sure affinity is set high for the cpu we should run on

	cpu_set_t set_affinity;
	CPU_ZERO(&set_affinity);
	CPU_SET(assigned_cpu,&set_affinity);
	sched_setaffinity(getpid(),sizeof(cpu_set_t),&set_affinity);
	
	int current_cpu;
	current_cpu=sched_getcpu();


	//link in the power meter functions if you're cpu 1

	if(assigned_cpu==0)
	{
		
		void * dl;	
	
		dl = dlopen ("../power/timer.so", RTLD_NOW);
		if (dl == NULL)
		{
		    printf("The power meter dynamic library could not be linked... please locate timer.so\n");
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
		
	}
	
	

	

	int i;
	for(i=0;i<NUM_UP_DOWN; i++)

	{
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
			changeFreq(current_cpu,num_frequency-1);//we switch to the lowest frequency
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


	if(assigned_cpu==0)
	{

		unsigned long long (* close_func)(void)=dlclose_func;
		double (* stop_func)(void)=dlstop_func;	
		energy_stop=stop_func();
		close_func();
		//energy_stop=(*dlstop_func)();
		//(*dlclose_func)();
	}

	//set the policy back to make the cpu responsive again
	if(!ondemand)
	{
		char policy[1024]="";
		sprintf(policy,"echo ondemand > /sys/devices/system/cpu/cpu%d/cpufreq/scaling_governor",current_cpu);
		system(policy);
	}
	
	

}	
	

	
	
