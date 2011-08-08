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




#include "DecisionStructures.h"
#include "Log.h"
#include "rdtsc.h"
#include "papiCounters.h"

#include <assert.h>
#include <papi.h>
#include <pthread.h>
#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <syscall.h>
#include <sys/types.h>
#include <unistd.h>


#if 0
#define PRINT_DEBUG
#endif


//internal declarations
//static void initPapiHelper ( int * EventSet, STPContext * handle);
//static __inline__ pid_t getTid (void);
//static __inline__ unsigned long long getTicks ( void );



/*static __inline__ unsigned long long getTicks ( void )
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
	int myWindow=FIRSTSLEEP;
	float lastBoundedValue=.5;//just initializing to .5 so it's valid
	float privateBounded;
	unsigned long long startTime, endTime;
	

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

	
	usleep (myWindow);
	while (killSignal==0)
	{
		float val1, val2, val3;

		//get PAPI info from counters		
		if(PAPI_OK != PAPI_accum (EventSet,values))
		{
			Log_output (5," PAPI accum failed: %s\n",PAPI_strerror(ret_code));
		}	

		//generate the bounded variable
		if(values[0]==0 || values[1]==0)//no divide by zeros!
		{
			privateBounded=0.0;
		}
		else
		{
			val1=values[0];
			val2=values[1];	
			val3=values[2];
			privateBounded=2*16*val1*val3/(val2*val2);
			privateBounded=(privateBounded>1.0)?1.0:privateBounded;
		}

		

		//fill in the report
		myReport.data.tp.bounded=privateBounded;
		endTime=getTicks ();	
		myReport.data.tp.ticks=endTime-startTime;
		myReport.data.tp.window=myWindow;

		
		#ifdef PRINT_DEBUG
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
		   	algorithm=myReport.data.tp.algorithm;
		   	/* @todo make a switch statement to do some changes to the papi counters as the DM asked and change your prof_id*/		
		}
		else
		{
			//self regulate
			if(abs (lastBoundedValue - privateBounded)>THRESHOLD) 
			{
				myWindow=FIRSTSLEEP;
			}
			else
			{
				myWindow*=2;
				myWindow= (myWindow>LONGESTSLEEP)?LONGESTSLEEP:myWindow;
			}
		}
		lastBoundedValue=privateBounded;
		startTime=getTicks ();
		usleep (myWindow);
	}

	/* @todo destroy papi stuff properly*/
	void (* myDestroyer)(void *)=myHandle->myFuncs.destroyFunc;
	myDestroyer (myDM);
	return NULL;
}
	

STPContext * threadedProfilerInit (SFuncsToUse funcPtrs)
{
	int current_cpu, retval;
	STPContext * handle;

	//we initialize papi
	retval=PAPI_library_init (PAPI_VER_CURRENT);
	if (retval != PAPI_VER_CURRENT) 
	{
		Log_output (0,"PAPI library init error!\n");
		exit (1);
	}

	
	//get our cpu... it should be pinned
	current_cpu=sched_getcpu ();

	// register the thread specificier
	 if (PAPI_thread_init (getTid) != PAPI_OK)
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
	

