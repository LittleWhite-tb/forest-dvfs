/*
Copyright (C) 2011 Exascale Compûting Research

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



#define _GNU_SOURCE
#include <sched.h>

#include "DecisionStructures.h"
#include "Log.h"
#include "rdtsc.h"
#include "papiCounters.h"
#include "ThreadedProfiler.h"

#include <assert.h>
#include <math.h>
#include <papi.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syscall.h>
#include <sys/types.h>
#include <unistd.h>


#if 0
#define PRINT_DEBUG 
#endif

#define STATS




//internal declarations
//static void initPapiHelper ( int * EventSet, STPContext * handle);
//static __inline__ pid_t getTid (void);
//static __inline__ unsigned long long getTicks ( void );


/*
static __inline__ unsigned long long getTicks ( void )
{
   unsigned long long ret;
   rdtscll (ret);
   return ret;
}

static __inline__ pid_t getTid (void)
{
	pid_t answer =  syscall (__NR_gettid);
	return answer;
}*/

/*static void initPapiHelper ( int * EventSet, STPContext * handle)
{

	int codes[3];
	int ret_code;
	
	if (PAPI_event_name_to_code ("SQ_FULL_STALL_CYCLES", &codes[0]) != PAPI_OK) 
	{
		Log_output (0,"PAPI even_name_to_code failed!\n");
		exit (1);
	}

	if (PAPI_event_name_to_code ("UNHALTED_CORE_CYCLES", &codes[1]) != PAPI_OK) 
	{
		Log_output (0,"PAPI even_name_to_code failed!\n");
		exit (1);
	}
	if (PAPI_event_name_to_code ("L2_RQSTS:MISS", &codes[2]) != PAPI_OK) 
	{
		Log_output (0,"PAPI even_name_to_code failed!\n");
		exit (1);
	}

	 //Check to see if the PAPI natives are available 
	if ((PAPI_query_event (codes[0]) != PAPI_OK) || (PAPI_query_event (codes[1]) != PAPI_OK) || (PAPI_query_event (codes[2]) != PAPI_OK)) 
	{
		Log_output (0,"PAPI counters aren't sufficient to measure boundedness!\n");
		exit (1);
	}

	ret_code=PAPI_create_eventset ( EventSet );
	if(PAPI_OK != ret_code)
	{
		Log_output (0,"Creating the PAPI create eventset failed:%d %s\n",ret_code, PAPI_strerror(ret_code));
		exit (1);

	}
	ret_code=PAPI_add_events (*EventSet,codes, 3);
	if(PAPI_OK != ret_code)
	{
		Log_output (0,"Adding the PAPI add eventset failed: %d %s\n",ret_code, PAPI_strerror(ret_code));
		exit (1);
	}

	//This ensures that papi counters only reads the parent thread counters
	ret_code=PAPI_attach (*EventSet, handle->parent);
	if(PAPI_OK != ret_code)
	{
		Log_output (0,"Attaching the PAPI eventset failed: %d %s\n",ret_code, PAPI_strerror(ret_code));
		exit (1);
	}
	return;
}*/




void * profilerThread (void * ContextPtr)
{

	/*Contexts and signal variables*/
	volatile int killSignal=0;//kill signal used to have destructor function from other thread kill this thread
	STPContext * temp =  ContextPtr;//handle to data needed from other thread
	STPContext * myHandle= temp;
	void * myDM;//handle to Decision Maker context
	SProfReport myReport;
	int mycore=myHandle->core;

	/*Papi related variables*/
	int EventSet = PAPI_NULL;
	long_long values[3]= {0,0,0};
	int ret_code=0;

	/*Algorithm related variables*/
	int algorithm=0;
	int myWindow=1;
	float lastBoundedValue=.5;//just initializing to .5 so it's valid
	float privateBounded;
	unsigned long long startTime, endTime;

	#ifdef STATS
	/*Log related data*/
	int failureBackoff=0;	
	int numGoodSamples=0;
	int numBadSamples=0;
	float totalOfBounded=0.0;
	float minValue=1.0;
	float maxValue=0.0;
	#endif
	
	

	#ifdef PRINT_DEBUG
	int myKHZ;
	FILE * fp;
	char * dummy;
	char freq_string[1024];
	//get the second highest frequency
	fp = popen ("cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies | cut -f2 -d\" \"","r");
	dummy=fgets (freq_string, sizeof (freq_string), fp);
	//make it an int	
	myKHZ=atoi (freq_string);
	printf ("My frequency is %d KHZ\n",myKHZ);
	#endif

	//first we initialize our decision maker functions and call the init
	int (* myReporter) (void *, SProfReport *)=myHandle->myFuncs.reportFunc;
	void * (* myInit) (void)=myHandle->myFuncs.initFunc;
	myDM=myInit ();


	//call the papi helper to init stuff
	initPapiHelper (  &EventSet, myHandle);

	//initialize my report
	myReport.prof_id=THREADED_PROFILER;
	myReport.proc_id=mycore;

	//we send back out kill signal address to the other thread to let it continue running
	myHandle->killSig=&killSignal;

	ret_code=PAPI_start (EventSet);
	if(PAPI_OK != ret_code)
	{
		Log_output (5," PAPI start failed: %s\n",PAPI_strerror(ret_code));
	}
	startTime=getTicks ();

	
	usleep (FIRSTSLEEP);

	while (killSignal==0)
	{


		float val1, val2, val3;

		//get PAPI info from counters		
		if(PAPI_OK != PAPI_accum (EventSet,values))
		{
			Log_output (0, " PAPI accum failed: %s\n",PAPI_strerror(ret_code));
			
			#ifdef STATS			
			numBadSamples++;

			if(numBadSamples>MINSAMPLESTOFAIL && numBadSamples > (numBadSamples+numGoodSamples)*FAILURERATE)
			{
				Log_output (10, " PAPI failures exceeded failure rate and sample threshold, program terminating\n");
				exit(4);
			}
			failureBackoff++;
			if (failureBackoff>4)
			{
				myWindow++;
				myWindow= (myWindow>log2(LONGESTSLEEP/FIRSTSLEEP))?log2(LONGESTSLEEP/FIRSTSLEEP):myWindow;
			}
			#endif
			
		}
		
		else if(values [1] != 0)
		//generate the bounded variable
		{
			
			Log_output (1,"PAPI accum succeeded!!\n");
					
			val1=values[0];
			val2=values[1];	
			val3=values[2];
			privateBounded=2*16*val1*val3/(val2*val2);
			privateBounded=(privateBounded>1.0)?1.0:privateBounded;
			
			#ifdef STATS
			//Doing statistics
			failureBackoff=0;
			minValue=(minValue>privateBounded)?privateBounded:minValue;
			maxValue=(maxValue>privateBounded)?maxValue:privateBounded;
			totalOfBounded+=privateBounded;
			numGoodSamples++;

			#endif
		
			//fill in the report
			myReport.data.tp.bounded=privateBounded;
			endTime=getTicks ();	
			myReport.data.tp.ticks=endTime-startTime;
			myReport.data.tp.window=myWindow;
			
			#ifdef PRINT_DEBUG
				printf( "From core %d:\n",mycore);
				printf ("Total Cycles is %lld\n",values[1]);
				printf ("Super Queue was full %lld\n",values[0]);
				printf ("L2 Misses is %lld\n",values[2]);
				printf ("Debug: giving a report with bounded = %f, actual window=%f, expected the window to be %d\n",myReport.data.tp.bounded,
					(float)myReport.data.tp.ticks * 1000 / (float) myKHZ,myWindow);//this math puts it in microsecond since we are usleeping now
				fflush (stdout);
			#endif
	
			//reset my counters
			values[0]=0;
			values[1]=0;
			values[2]=0;
	
			//give the report
			if (myReporter (myDM, &myReport))
			{
			  	myWindow=myReport.data.tp.nextWindow;
				myWindow= (myWindow>log2(LONGESTSLEEP/FIRSTSLEEP))?log2(LONGESTSLEEP/FIRSTSLEEP):myWindow;	
			   	algorithm=myReport.data.tp.algorithm;
			   	/* @todo make a switch statement to do some changes to the papi counters as the DM asked and change your prof_id*/		
			}
			else
			{
				//self regulate
				if(fabs (lastBoundedValue - privateBounded)>THRESHOLD) 
				{
					myWindow=1;
				}
				else
				{
					myWindow++;
					myWindow= (myWindow>log2(LONGESTSLEEP/FIRSTSLEEP))?log2(LONGESTSLEEP/FIRSTSLEEP):myWindow;
				}	
			}
			lastBoundedValue=privateBounded;
			startTime=getTicks ();
		}		
		else
		{
			Log_output (6, "Silently failing PAPI accum!!\n");
			#ifdef STATS			
			numBadSamples++;

			if(numBadSamples>MINSAMPLESTOFAIL && numBadSamples > (numBadSamples+numGoodSamples)*FAILURERATE)
			{
				Log_output (10, " PAPI failures exceeded failure rate and sample threshold, program terminating\n");
				exit(4);
			}
			failureBackoff++;
			if (failureBackoff>4)
			{
				myWindow++;
				myWindow= (myWindow>log2(LONGESTSLEEP/FIRSTSLEEP))?log2(LONGESTSLEEP/FIRSTSLEEP):myWindow;
			}
			#endif
		}
		assert(pow(2,myWindow)*FIRSTSLEEP!=0);
		usleep (pow(2,myWindow)*FIRSTSLEEP);
	}

	#ifdef STATS
	float goodSamp=	numGoodSamples;
	float badSamp=numBadSamples;
	
	//dump profiler stats to a file
	char char_buff[256]="\0";
	char numCoreString[10];
	if(getenv("REST_OUTPUT")!=NULL)
	{
		strcat (char_buff,getenv("REST_OUTPUT"));
		strcat (char_buff,"/");
	}
	strcat(char_buff,"ProfilerStats");
	sprintf (numCoreString,"%d",mycore);
	strcat(char_buff,numCoreString);
	strcat(char_buff,".txt");
		fprintf (stderr, "%s: is the file name we are trying to appending to\n", char_buff);
	
	FILE * dumpfile=fopen (char_buff,"a");
	if(dumpfile!=NULL)
	{
		fprintf(dumpfile,"PAPI Failure Rate: %f  Good Samples: %d  Bad Samples: %d \n",badSamp/(goodSamp+badSamp),numGoodSamples,numBadSamples);
		fprintf(dumpfile,"Minimum Boundedness: %f\n",minValue);
		fprintf(dumpfile,"Maximum Boundedness: %f\n",maxValue);
		fprintf(dumpfile,"Average Boundedness: %f\n",totalOfBounded/goodSamp);	
	}
	else
	{
		Log_output (2000,"Unable to open file for dumping profiler data\n");
	}
	fclose(dumpfile);
	#endif
	
	void (* myDestroyer)(void *)=myHandle->myFuncs.destroyFunc;
	myDestroyer (myDM);
	return NULL;
}
	

STPContext * threadedProfilerInit (SFuncsToUse funcPtrs)
{
	int current_cpu, retval;
	STPContext * handle;

	//we initialize papi
	retval = PAPI_is_initialized();
	if (retval == PAPI_LOW_LEVEL_INITED)
	{
		Log_output (20,"PAPI library already initialized error!\n");
                exit (1);
	}
	retval=PAPI_library_init (PAPI_VER_CURRENT);
	if (retval != PAPI_VER_CURRENT) 
	{
		Log_output (20,"PAPI library init error!\n");
		exit (1);
	}

	
	//get our cpu... it should be pinned
	current_cpu=sched_getcpu ();

	// register the thread specificier
	 if (PAPI_thread_init ((unsigned long (*)(void)) getpid) != PAPI_OK)
	{
		Log_output (0,"Thread init function didn't register properly\n");
	}

	//allocate our context on the heap, check that we succeeded, and initialize it
	handle= malloc (sizeof ( * handle));
	assert ( handle != NULL );
	handle->killSig=NULL;
	handle->core=current_cpu;
	handle->parent=PAPI_thread_id ();
	handle->myFuncs=funcPtrs;
	assert(pthread_create (&(handle->join_id),NULL,profilerThread,handle)==0);
	//spin waiting for my child to init
	while (handle->killSig==NULL){}
	return handle;
}
	

void threadedProfilerDestroy (STPContext * prof)
{
	void* dummy;
	
	//send the kill signal to the profiling thread
	*(prof->killSig)=1;
	
	//wait for profiler to die
	pthread_join (prof->join_id,&dummy);
	
	//clean up my context
	free (prof);

}
	

