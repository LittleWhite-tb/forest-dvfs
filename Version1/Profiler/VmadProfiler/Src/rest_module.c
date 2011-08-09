#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#define _GNU_SOURCE
#include <sched.h>

#include "rest_module.h"
#include "rest_vmad.h"
#include "NaiveDM.h"

void restModuleLoad (restModule **ptrmymodule)
{
	//module(s) loading
	
	STPContext * handle;
	handle= malloc (sizeof (*handle));
	assert ( handle != NULL );
	handle->killSig = NULL;
	handle->core=sched_getcpu ();
	handle->parent = -1;
	handle->myFuncs.initFunc = naiveDecisionInit;
	handle->myFuncs.destroyFunc = naiveDecisionDestruct;
	handle->myFuncs.reportFunc = naiveDecisionGiveReport;
	
	restModule * mymodule = NULL;
	mymodule = malloc (sizeof (*mymodule));
	assert( mymodule != NULL);
	mymodule->context.ProfContext = handle;
	mymodule->init = NULL;
	mymodule->off = NULL;
	mymodule->quit = NULL;
	mymodule->reset = NULL;
	
	mymodule->report.Profreport.prof_id=VMAD_PROFILER;
	mymodule->report.Profreport.proc_id=mymodule->context.ProfContext->core;
	mymodule->report.Profreport.data.tp.window=FIRSTSLEEPITERATION;
	
	//module(s) init
	restBind(mymodule);
	mymodule->init(mymodule);
	*ptrmymodule = mymodule;
}

void restModuleUnload (restModule *mymodule)
{
	//quit
	mymodule->quit(mymodule);
	mymodule->init = NULL;
	mymodule->on = NULL;
	mymodule->off = NULL;
	mymodule->quit = NULL;
	mymodule->reset = NULL;
	free(mymodule); mymodule = NULL;
}
