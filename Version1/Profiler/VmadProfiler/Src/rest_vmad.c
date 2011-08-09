#include <math.h>
#include <papi.h>
#include <stdio.h>
#include <stdlib.h>
#include <syscall.h>

#include "rest_vmad.h"
#include "Profilers.h"
#include "papiCounters.h"




void restBind(restModule *module) 
{
	module->init = restInit;
	module->on = restOn;
	module->off = restOff;
	module->quit = restQuit;
	module->reset = restReset;
}


void *restInit(restModule *module)
{
	/**Papi initialisation, almost like the threaded profiler*/
	void * theDmContext = NULL;
	int EventSet = PAPI_NULL;
	
	initLibraryPapi();

	initThreadPapi ();

	module->context.ProfContext->parent = threadIdPapi();
	
	//call the papi helper to init stuff
	initPapiHelper (&EventSet, module->context.ProfContext);
	
	module->report.papiEventSet = EventSet;
	
	startPapi(module->report.papiEventSet);
	
	void * (* DmInit) (void)=module->context.ProfContext->myFuncs.initFunc;
	theDmContext = DmInit();
	module->context.DMcontext = theDmContext;
	
	return NULL;
}

void *restQuit(restModule *module)
{
	free(module->context.DMcontext); module->context.DMcontext = NULL;
	free(module->context.ProfContext); module->context.ProfContext = NULL;
	return NULL;
}

void *restOn(restModule *module)
{
	float privateBounded;
	static float lastBoundedValue = 0.5;
	static int myWindow = FIRSTSLEEPITERATION;
	long_long values[3]= {0,0,0};
	unsigned long long currentTicks = 0;
	static unsigned long long oldTicks = 0;
	
	currentTicks = getTicks ();
	module->report.Profreport.data.tp.ticks = currentTicks - oldTicks;
	oldTicks = currentTicks;
	
	accumPapi(module->report.papiEventSet, values);
	
	if(values[0]==0 || values[1]==0)//no divide by zeros!
	{
		privateBounded=0.0;
	}
	else
	{
		privateBounded=2*16*values[0]*values[2]/(values[1]*values[1]);
		privateBounded=(privateBounded>1.0)?1.0:privateBounded;
	}
	
	module->report.Profreport.data.tp.bounded=privateBounded;
	module->report.Profreport.data.tp.window=myWindow;
	
	int (* DmReport) (void *, SProfReport *) = module->context.ProfContext->myFuncs.reportFunc;
	
	if (DmReport (module->context.DMcontext, &(module->report).Profreport))
		{
		  	myWindow=module->report.Profreport.data.tp.nextWindow;
		   	//algorithm=module->report.Profreport.data.tp.algorithm;
		   	/* @todo make a switch statement to do some changes to the papi counters as the DM asked and change your prof_id*/		
		}
		else
		{
			//self regulate
			
			if(fabsf (lastBoundedValue - privateBounded)> (float)THRESHOLD) 
			{
				myWindow=FIRSTSLEEPITERATION;
			}
			else
			{
				myWindow*=2;
				myWindow= (myWindow>LONGESTSLEEPITERATION)?LONGESTSLEEPITERATION:myWindow;
			}
		}
		lastBoundedValue=privateBounded;
		
		module->report.Profreport.data.tp.window=myWindow;
		
	return module;
}

void *restOff(restModule *module)
{
	(void) module;
	return NULL;
}

void *restReset(restModule *module)
{
	(void) module;
	return NULL;
}
