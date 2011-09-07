/*
Copyright (C) 2011 Exascale Research Center

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
#include <dlfcn.h>

//#define _REENTRANT
#include <assert.h>
#include <stdio.h>

#include "NaiveDM.h"
#include "PredictiveDM.h"
#include "MarkovDM.h"
#include "Rest.h"
#include "ThreadedProfiler.h"
#include "Log.h"

#ifdef _VMAD
#include "camus_definitions.h"
#endif

#if 0
#include <pthread.h>
#include <unistd.h>

#define SPECTESTING
#define TESTTIME 5 //in seconds
#endif



//single global variable for the linker to hook us in... internally used only
static rest_main_t rest_original_main;
profilerHandle *restHandle = NULL;  //must be global so the atexit function can grab it
char ldPreload[256]="\0";
	
static void restAtExit( void )
{
	RestDestroy(restHandle);
}

//wrapper for the original function
static int rest_main(int argc, char** argv, char** env)
{
    
    toolChainInit profiler = REST_T_PROFILER, algorithm = REST_NAIVE_DM, freqChanger = REST_FREQ_CHANGER;
    
    //LD PRELOAD Could work but we are not sure
    char * tmp=getenv ("LD_PRELOAD");
    if(tmp!=NULL)
    {
    	strcpy (ldPreload, tmp);
    	setenv ("LD_PRELOAD"," ", 1);
    }
    //Choosing which profiler need to use
    if(getenv("REST_PROFILER") !=NULL)
    {
    	if(strcmp(getenv("REST_PROFILER"), "vmad_profiler") == 0)
    	{
    		profiler = REST_VMAD_PROFILER;
    	}
    }

    //Choosing which algorithm need to use
    if(getenv("REST_DM") !=NULL)
    {
    	if(strcmp(getenv("REST_DM"), "predictive_dm") == 0)
    	{
    		algorithm = REST_BRANCHPREDICT_DM;
    	}
    	else
    		if(strcmp(getenv("REST_DM"), "markov_dm") == 0)
    		{
    			algorithm = REST_MARKOVPREDICT_DM;
    		}
    }
    
    //Choosing which algorithm need to use
    if(getenv("REST_FREQ_CHANGER") !=NULL)
    {
    	//Will be useful when we will have more than one freq changer
    }
    
    Log_output (0,"Configuration:\n profiler: %d \n algorithm: %d \n freqChanger: %d \n ",profiler, algorithm, freqChanger);
    restHandle = RestInit(profiler, algorithm, freqChanger);
    restHandle->profiler = profiler;
    atexit(restAtExit);
    return rest_original_main(argc, argv, env);
 
}


// we redefine libc_start_main because at this point main is an arguement so we can store it in our symbol table and hook in
int __libc_start_main(rest_main_t main, int argc, char** ubp_av,
        rest_lsm_t init, rest_lsm_t fini, rest_lsm_t rtld_fini,
        void* stack_end)
{

    //reset main to our global variable so our wrapper can call it easily
    rest_original_main = main;
    //Initialisation :

    rest_libc_start_main_t real_start =
        (rest_libc_start_main_t)dlsym(RTLD_NEXT, "__libc_start_main");

    
    //call the wrapper with the real libc_start_main
    return real_start(rest_main, argc, ubp_av, init, fini, rtld_fini, stack_end);
}


#ifdef SPECTESTING

void waitThread (void* arg)
{
	sleep(TESTTIME);
	printf("You are in testing mode... undefine sPECTESTING in Rest.c for this\n");
	exit(5);
}
#endif



profilerHandle *RestInit (toolChainInit profiler, toolChainInit decisionMaker, toolChainInit freqChanger)
{
	if((restHandle == NULL) || ((restHandle != NULL) && (profiler != restHandle->profiler)))
	{
		if(restHandle != NULL)
		{
			Log_output (0,"Rest Already initialized overwriting previous profiler \n");
			RestDestroy(restHandle);
		}

		#ifdef SPECTESTING
		pthread_t dummy;
		pthread_create(&dummy ,NULL,waitThread,NULL);
		#endif
		STPContext *(*profilerInitFunction) (SFuncsToUse funcPtrs) = NULL;	
		SFuncsToUse decisionFuncs;

		Log_init();
		fprintf(stderr,"Initializing REST...\n");

		switch (profiler)
		{
			case REST_T_PROFILER :
				profilerInitFunction = threadedProfilerInit;
				break;
			case REST_VMAD_PROFILER :
				//Nothing to do here
				break;
			default :
				fprintf(stderr, "Undefined profiler, the defined ones are : THREADED_PROFILER, VMAD_PROF_PROFILER\n");
				assert(0);
				break;
		}
	
		switch (decisionMaker)
		{
			case REST_NAIVE_DM:
				decisionFuncs.initFunc =  naiveDecisionInit;
				decisionFuncs.destroyFunc = naiveDecisionDestruct;
				decisionFuncs.reportFunc =  naiveDecisionGiveReport;
				break;
			case REST_BRANCHPREDICT_DM:
				decisionFuncs.initFunc =  branchDecisionInit;
				decisionFuncs.destroyFunc = branchDecisionDestruct;
				decisionFuncs.reportFunc =  branchDecisionGiveReport;
				break;
			case REST_MARKOVPREDICT_DM:
				decisionFuncs.initFunc = markovDecisionInit;
				decisionFuncs.destroyFunc = markovDecisionDestruct;
				decisionFuncs.reportFunc = markovDecisionGiveReport;	
				break;
			default:
				fprintf(stderr, "Undefined decision maker, the defined ones are : NAIVE_DM, BRANCHPREDICT_DM, MARKOVPREDICT_DM\n");
				assert(0);
				break;
		}
	
		switch (freqChanger)
		{
			case REST_FREQ_CHANGER:
				break;
			default:
				fprintf(stderr, "Undefined decision maker, the defined ones are : FREQ_CHANGER\n");
				assert(0);
				break;
		}
	
		STPContext *handle = NULL;
		if (profilerInitFunction != NULL)
		{
			handle = profilerInitFunction (decisionFuncs);
		}
		else
		{
			handle = malloc (sizeof (*handle));
			assert (handle != NULL);
			memset (handle, 0, sizeof (*handle));
			handle->myFuncs = decisionFuncs;
		}

		restHandle = malloc(sizeof (*restHandle));
		restHandle->ptr = handle;
	}
	else
		Log_output (15,"Rest Already initialized same profiler detected, Rest continues. \n");

	return restHandle;
}

void RestDestroy (profilerHandle *handle)
{
    	STPContext *context = handle->ptr;
	void (*profilerDestroyFunction) (STPContext * prof);

	switch (handle->profiler)
	{
		case REST_T_PROFILER :
			profilerDestroyFunction = threadedProfilerDestroy;
			break;
		case REST_VMAD_PROFILER :
			break;
		default :
			fprintf(stderr, "Undefined profiler, the defined ones are : THREADED_PROFILER, VMAD_PROF_PROFILER\n");
			assert(0);
			break;
	}
	
	profilerDestroyFunction(context);
	
	setenv ("LD_PRELOAD",ldPreload, 1);
	Log_destroy ();
}

#ifdef _VMAD

void camus_empty (camus_module_t module)
{
}

void camus_bind (camus_module_t module) 
{
	module->init = camus_empty;
	module->on = camus_empty;
	module->off = camus_empty;
	module->quit = camus_empty;
	module->reset = camus_empty;
}
#endif

