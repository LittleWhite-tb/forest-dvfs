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



#include <Profilers.h>
#include <ThreadedProfiler.h>
#include <DecisionMaker.h>

#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <papi.h>
#include <assert.h>
#include <rdtsc.h>

#define PRINT_DEBUG

static __inline__ unsigned long long getticks ( void )
{
   unsigned long long ret;
   rdtscll (ret);
   return ret;
}


/**
 * @brief core runtime routine
 * @param ContextPtr pointer to my parents data
 **/

void * profilerThread (void * ContextPtr)
{

	/*Contexts and signal variables*/
	volatile int killSignal=0;//kill signal used to have destructor function from other thread kill this thread
	STPContext * myHandle= (STPContext *) ContextPtr;//handle to data needed from other thread
	void * myDM;//handle to Decision Maker context
	SProfReport myReport;
	int mycore=myHandle->core;

	/*Papi related variables*/
	PAPI_thread_id_t parentTid=myHandle->parent;//thread id used to pin
	int EventSet = PAPI_NULL;
	long_long values[4];

	/*Algorithm related variables*/
	int algorithm=0;
	int myWindow=FIRSTSLEEP;
	float lastBoundedValue=.5;
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
	printf("My frequency is %d KHZ\n",myKHZ);
	#endif

	//first we initialize our decision maker
	void * (* myInit)(void)=myHandle->myFuncs.initFunc;
	myDM=myInit();

	/* Check to see if the PAPI presets are available */
	if ((PAPI_query_event (PAPI_L2_DCM) != PAPI_OK) && (PAPI_query_event (PAPI_L2_DCA) != PAPI_OK) && 
		(PAPI_query_event (PAPI_MEM_SCY) != PAPI_OK) && (PAPI_query_event (PAPI_TOT_CYC) != PAPI_OK)) 
	{
		fprintf (stderr,"PAPI counters aren't sufficient to measure boundedness!\n");
		exit(1);
	}

	PAPI_create_eventset ( &EventSet );
	PAPI_add_event (EventSet,PAPI_MEM_SCY);
	PAPI_add_event (EventSet,PAPI_TOT_CYC);
	PAPI_add_event (EventSet,PAPI_L2_DCM);
	PAPI_add_event (EventSet,PAPI_L2_DCA);

	//This ensures that papi counters only reads the parent thread counters
	PAPI_attach (EventSet, parentTid);

	//initialize my report
	myReport.prof_id=THREADED_PROFILER;
	myReport.proc_id=mycore;

	//we send back out kill signal address to the other thread to let it continue running
	myHandle->killSig=&killSignal;

	
	PAPI_start(EventSet);
	startTime=getticks ();

	
	usleep (myWindow);
	while (killSignal==0)
	{
		
		int (* myReporter) (void *, SProfReport *)=myHandle->myFuncs.reportFunc;
		float val1, val2;

		//get PAPI info from counters		
		PAPI_accum(EventSet,values);

		#ifdef PRINT_DEBUG
			printf ("Cycles stalled on memory accesses is %lld and Total Cycles is %lld\n",values[0],values[1]);
			printf ("L2 Cache misses is %lld and L2 Cache Accesses is %lld\n",values[2],values[3]);
		#endif		

		//generate the bounded variable
		val1=values[0];
		val2=values[1];
		privateBounded=val1/val2;
		
		//reset my counters
		values[0]=0;
		values[1]=0;
		values[2]=0;
		values[3]=0;

		//fill in the report
		myReport.data.tp.bounded=privateBounded;
		endTime=getticks();	
		myReport.data.tp.ticks=endTime-startTime;

		//give the report
		#ifdef PRINT_DEBUG
			printf ("Debug: giving a report with bounded = %f, actual window=%f, expected the window to be %d\n",myReport.data.tp.bounded,
				(float)myReport.data.tp.ticks * 1000 / (float) myKHZ,myWindow);//this math puts it in microsecond since we are usleeping now
		#endif


		if (myReporter (myDM, &myReport))
		{
		  	myWindow=myReport.data.tp.nextTicks;
		   	algorithm=myReport.data.tp.algorithm;
		   	/* @todo make a switch statement to do some changes to the papi counters as the DM asked and change your prof_id*/		
		}
		else
		{
			//self regulate
			if( (lastBoundedValue/privateBounded)<LOWERTHRESHOLD || (lastBoundedValue/privateBounded)>UPPERTHRESHOLD)
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
		startTime=getticks ();
		usleep (myWindow);
	}
	void (* myDestroyer)(void *)=myHandle->myFuncs.destroyFunc;
	myDestroyer (myDM);
	return NULL;
}
	

STPContext * profilerInit (SFuncsToUse funcPtrs)
{
	int current_cpu, retval;
	STPContext * handle;

	//we initialize papi
	retval=PAPI_library_init (PAPI_VER_CURRENT);
	if (retval != PAPI_VER_CURRENT) 
	{
		fprintf (stderr,"PAPI library init error!\n");
		exit (1);
	}

		
	//get our cpu... it should be pinned
	current_cpu=sched_getcpu ();

	// register the thread specificier
	PAPI_thread_init (pthread_self);
	//allocate our context on the heap, check that we succeeded, and initialize it
	handle= malloc(sizeof( * handle));
	assert ( handle != NULL );
	handle->killSig=NULL;
	handle->core=current_cpu;
	handle->parent=PAPI_thread_id();
	handle->myFuncs=funcPtrs;
	assert(pthread_create (&(handle->join_id),NULL,profilerThread,handle)==0);
	//spin waiting for my child to init
	while (handle->killSig==NULL){}
	return handle;
}
	

void profilerDestroy (STPContext * prof)
{
	void* dummy;
	
	//send the kill signal to the profiling thread
	*(prof->killSig)=1;
	
	//wait for profiler to die
	pthread_join(prof->join_id,&dummy);
	
	//clean up my context
	free( prof);

}
	

