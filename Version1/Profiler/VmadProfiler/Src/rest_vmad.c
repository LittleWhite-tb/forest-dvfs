#include <math.h>
#include <papi.h>
#include <stdio.h>
#include <stdlib.h>
#include <syscall.h>

#include "rest_vmad.h"
#include "Profilers.h"
#include "papiCounters.h"
#include "NaiveDM.h"




void camus_bind (camus_module_t module) 
{
	module->init = restInit;
	module->on = restOn;
	module->off = restOff;
	module->quit = restQuit;
	module->reset = restReset;
}


void restInit(camus_module_t module)
{
	void * theDmContext = NULL;
	int EventSet = PAPI_NULL;
	restData * restdata;
	
	xlog_start();
	xlog("restInit");
	xlog_add(" -> %s",module->header->name);
	
	STPContext * handle;
	handle= malloc (sizeof (*handle));
	assert ( handle != NULL );
	handle->killSig = NULL;
	handle->core=sched_getcpu ();
	handle->parent = -1;
	handle->myFuncs.initFunc = naiveDecisionInit;
	handle->myFuncs.destroyFunc = naiveDecisionDestruct;
	handle->myFuncs.reportFunc = naiveDecisionGiveReport;

	
	module->data = malloc(sizeof(restData));
	restdata = module->data;
	
	restdata->context.ProfContext = handle;
	
	initLibraryPapi();

	initThreadPapi ();

	restdata->context.ProfContext->parent = threadIdPapi();
	
	initPapiHelper (&EventSet, restdata->context.ProfContext);
	
	restdata->report.papiEventSet = EventSet;
	
	startPapi(restdata->report.papiEventSet);
	
	void * (* DmInit) (void)=restdata->context.ProfContext->myFuncs.initFunc;
	theDmContext = DmInit();
	restdata->context.DMcontext = theDmContext;

	xlog_end();
}

void restQuit(camus_module_t module)
{
	xlog_start();
	xlog("restQuit");
	xlog_add(" -> %s",module->header->name);
	restData * restdata = module->data;
	
	free(restdata->context.DMcontext); restdata->context.DMcontext = NULL;
	free(restdata->context.ProfContext); restdata->context.ProfContext = NULL;
	free(restdata); module->data = NULL;
	
	xlog_end();
}

void restOn(camus_module_t module)
{
	xlog_start();
	xlog("restOn");
	xlog_add(" -> %s",module->header->name);
	
	xlog_end();
	/*float privateBounded;
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
		   	// @todo make a switch statement to do some changes to the papi counters as the DM asked and change your prof_id
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
		
	return module;*/
}

void restOff(camus_module_t module)
{
	xlog_start();
	xlog("restOff");
	xlog_add(" -> %s",module->header->name);
	
	xlog_end();
}

void restReset(camus_module_t module)
{
	xlog_start();
	xlog("restReset");
	xlog_add(" -> %s",module->header->name);
	
	xlog_end();
}
