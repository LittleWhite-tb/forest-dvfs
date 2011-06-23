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

#include <pthread.h>
#include <unistd.h>
#include <papi.h>
#include <rdtsc.h>

static __inline__ unsigned long long getticks(void)
{
   unsigned long long ret;
   rdtscll(ret);
   return ret;
}


/**
 * @brief core runtime routine
 **/

void * profilerThread(void * ContextPtr)
{
	volatile int killSignal=0;
	STPContext * parentAddr= (STPContext *) ContextPtr;
	DMContext myDM;
	SProfReport myReport;
	pthread_t parentTid=parentAddr->parent;
	int algorithm=0;
	int myWindow=FIRSTSLEEP;
	float lastBoundedValue;
	float privateBounded;

	unsigned long long startTime, endTime;
	

	myReport.prof_id=THREADED_PROFILER;
	myReport.proc_id=parentTid;

	//first we initialize our decision maker
	
	myDM=decisionInit();

	//we initialize papi
	PAPI_library_init(PAPI_VER_CURRENT);

	//we send back out context to the other thread to let it continue running
	parentAddr->killSig=&killSignal;
	
	startTime=getticks();

	/*@todo: add all the papi stuff*/
	
	usleep(myWindow);
	while(killSignal==0)
	{
		/*@todo do some papi stuff for now it's statically set at .4*/

		privateBounded=.4;

		//fill in the report
		myReport.data.tp.bounded=privateBounded;
		endTime=getticks();	
		myReport.data.tp.ticks=endTime-startTime;
		//give the report
		printf("Debug: giving a report with bounded = %f window=%d\n",myReport.data.tp.bounded,myReport.data.tp.ticks);
		if(!decisionGiveReport (myDM, &myReport))
		{
		  	myWindow=myReport.data.tp.nextTicks;
		   	algorithm=myReport.data.tp.algorithm;
		   	//switch(algorithm){
			//	case default:/* @todo do some changes to the papi counters as the DM asked and change your prof_id*/
			//	};
		}
		else
		{
		    //self regulate
		    if((lastBoundedValue/privateBounded)<LOWERTHRESHOLD || (lastBoundedValue/privateBounded)>UPPERTHRESHOLD)
		    {
			myWindow=FIRSTSLEEP;
		    }
		    else
		    {
			myWindow*=2;
			myWindow=(myWindow>LONGESTSLEEP)?LONGESTSLEEP:myWindow;
		    }
		}
		startTime=getticks();
		usleep(myWindow);
	}
	decisionDestruct(myDM);
	killSignal=0;
}
	



	


STPContext profilerInit (void)
{

	pthread_t dummy;
	STPContext childContext;
	childContext.killSig=NULL;
	childContext.parent=pthread_self();
	pthread_create(&dummy,NULL,profilerThread,&childContext);
	while (childContext.killSig==NULL){};//wait for my child to init
	return childContext;
}
	

void profilerDestroy (STPContext prof)
{
	*(prof.killSig)=1;
	printf("Now waiting on my prof thread\n");
	while(*(prof.killSig)!=0){}//wait for profiler to die
	printf("Waiting is over! Goodbye!\n");
}
	

