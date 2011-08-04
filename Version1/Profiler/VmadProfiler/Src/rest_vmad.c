#include <stdio.h>
#include <stdlib.h>
#include <papi.h>
#include <syscall.h>

#include "rest_vmad.h"
#include "Profilers.h"
#include "papiCounters.h"




void restBind(restModule *module)
{
	module->init = restInit(module);
	module->on = restOn (module);
	module->off = restOff (module);
	module->quit = restQuit (module);
	module->reset = restReset (module);
}


void *restInit(restModule *module)
{
	/**Papi initialisation, almost like the threaded profiler*/
	int EventSet = PAPI_NULL;
	STPContext * handle;
	long_long values[3]= {0,0,0};
	
	initLibraryPapi();
	initThreadPapi (getTid);
	
	//get our cpu... it should be pinned
	int current_cpu=sched_getcpu ();
	
	handle= malloc (sizeof ( * handle));
	assert ( handle != NULL );
	handle->killSig=NULL;
	handle->core=current_cpu;
	handle->parent=threadIdPapi ();
	handle->myFuncs=module->context->myFuncs;
	
	//call the papi helper to init stuff
	initPapiHelper (&EventSet, handle);
	startPapi(EventSet);
	
	
	
	
	restOn (module);
	return NULL;
}

void *restQuit(restModule *module)
{
	//destroy la chaine rest
	return NULL;
}

void *restOn(restModule *module)
{
	//choper les compteurs papi

	//init la chaine de rest
	return NULL;
}
void *restOff(restModule *module)
{
	return NULL;
}
void *restReset(restModule *module)
{
	return NULL;
}
