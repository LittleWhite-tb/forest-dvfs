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
	volatile int killSignal=0;//kill signal used to have destructor function from other thread kill this thread
	STPContext * parentAddr= (STPContext *) ContextPtr;//handle to data needed from other thread
	void * myDM;//handle to Decision Maker context
	SProfReport myReport;
	PAPI_thread_id_t parentTid=parentAddr->parent;
	int EventSet = PAPI_NULL;



	int algorithm=0;
	int myWindow=FIRSTSLEEP;
	int mycore=parentAddr->core;
	float lastBoundedValue;
	float privateBounded;
	long_long values[2];

	unsigned long long startTime, endTime;
	

	#ifdef PRINT_DEBUG
	int myKHZ;
	FILE * fp;
	char freq_string[1024];
	
	fp = popen ("cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies | cut -f2 -d\" \"","r");
	fgets (freq_string, sizeof (freq_string), fp);
	myKHZ=atoi (freq_string);
	printf("My frequency is %d KHZ\n",myKHZ);

	#endif



	myReport.prof_id=THREADED_PROFILER;
	myReport.proc_id=mycore;

	//first we initialize our decision maker
	
	myDM=decisionInit ();

	/*@todo: add all the papi stuff*/
	
		
	/* Check to see if the preset, PAPI_L2_TCH and PAPI_L2_TCA exists */
	if ((PAPI_query_event (PAPI_L2_TCM) != PAPI_OK) && (PAPI_query_event (PAPI_L2_TCA) != PAPI_OK)) 
	{
		fprintf (stderr,"PAPI counters aren't sufficient to measure boundedness!\n");
		exit(1);
	}

	PAPI_create_eventset ( &EventSet );
	PAPI_add_event (EventSet,PAPI_L2_TCM);
	PAPI_add_event (EventSet,PAPI_L2_TCA);

	//This ensures that papi counters only reads the parent thread counters
	PAPI_attach (EventSet, parentTid);


	//we send back out kill signal address to the other thread to let it continue running
	parentAddr->killSig=&killSignal;

	
	PAPI_start(EventSet);
	startTime=getticks ();
	

	


	
	usleep (myWindow);
	while (killSignal==0)
	{
		

		PAPI_accum(EventSet,values);

		#ifdef PRINT_DEBUG
		
		printf ("L2 Cache misses is %lld and L2 Cache Accesses is %lld\n",values[0],values[1]);
		
		#endif		

		if(values[1]!=0)
		{
			privateBounded=(float)values[0]/(float)values[1];
		}
		else
		{
			//if it didn't hit the L2 at all it must be super compute bound
			privateBounded=0.0;
		}
		values[0]=0;
		values[1]=0;

		//fill in the report
		myReport.data.tp.bounded=privateBounded;
		endTime=getticks();	
		myReport.data.tp.ticks=endTime-startTime;


		//give the report
		#ifdef PRINT_DEBUG
		printf ("Debug: giving a report with bounded = %f, actual window=%f, expected the window to be %d\n",myReport.data.tp.bounded,
				(float)myReport.data.tp.ticks * 1000 / (float) myKHZ,myWindow);//this math puts it in microsecond since we are usleeping now
		#endif


		if (decisionGiveReport (myDM, &myReport))
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
	decisionDestruct (myDM);
}
	



	


STPContext * profilerInit (void)
{

	
	STPContext childContext;
	int current_cpu, retval;
	STPContext * handle;


	//make sure affinity is set high for the cpu we should run on

	current_cpu=sched_getcpu ();

	//we initialize papi
	retval=PAPI_library_init (PAPI_VER_CURRENT);
	if (retval != PAPI_VER_CURRENT) 
	{
		fprintf (stderr,"PAPI library init error!\n");
		exit (1);
	}
	// register the thread specificier
	PAPI_thread_init (pthread_self);

	handle= malloc(sizeof( * handle));
	assert ( handle != NULL );

	handle->killSig=NULL;
	handle->core=current_cpu;
	handle->parent=PAPI_thread_id();

	assert(pthread_create (&(handle->join_id),NULL,profilerThread,handle)==0);

	while (handle->killSig==NULL){}//wait for my child to init
	return handle;
}
	

void profilerDestroy (STPContext * prof)
{
	*(prof->killSig)=1;
	void* dummy;
	printf("Now waiting on my prof thread\n");
	pthread_join(prof->join_id,&dummy);//wait for profiler to die
	
	//clean up my context
	free( prof);

	printf("Waiting is over! Goodbye!\n");
}
	

