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
#include <pthread.h>
#include <unistd.h>

#include <dlfcn.h>


profilerHandle *restHandle = NULL;

int RestInitF ()
{
	return RestInit (REST_T_PROFILER, REST_NAIVE_DM, REST_FREQ_CHANGER);
}	

int RestInit (toolChainInit profilerArg, toolChainInit decisionMakerArg, toolChainInit freqChangerArg)
{
	toolChainInit profiler = profilerArg, decisionMaker = decisionMakerArg, freqChanger = freqChangerArg;
	int isRestEnvVar = 0;

	if((getenv("REST_PROFILER") !=NULL) || (getenv("REST_DM") !=NULL) || (getenv("REST_FREQ_CHANGER") !=NULL))
		isRestEnvVar = 1;

	if((restHandle == NULL) || ((restHandle != NULL) && !isRestEnvVar))
	{

		if((restHandle != NULL))
		{
			Log_output (-1,"Rest Already initialized overwriting previous parameters \n");
			RestDestroy();
		}
		else
		{
			Log_init();
		}

		//Choosing which profiler need to use
		if(getenv("REST_PROFILER") !=NULL && (strcmp(getenv("REST_PROFILER"), "vmad_profiler") == 0))
		{
				profiler = REST_VMAD_PROFILER;
		}

		//Choosing which algorithm need to use
		if(getenv("REST_DM") !=NULL)
		{
			if(strcmp(getenv("REST_DM"), "predictive_dm") == 0)
			{
				decisionMaker = REST_BRANCHPREDICT_DM;
			}
			
			if(strcmp(getenv("REST_DM"), "markov_dm") == 0)
			{
				decisionMaker = REST_MARKOVPREDICT_DM;
			}
			isRestEnvVar = 1;		
		}

		//Choosing which algorithm need to use
		if(getenv("REST_FREQ_CHANGER") !=NULL)
		{
			//Will be useful when we will have more than one freq changer
		}

		STPContext *(*profilerInitFunction) (SFuncsToUse funcPtrs) = NULL;	
		SFuncsToUse decisionFuncs;

		Log_output (-1, "Initializing REST...\n");

		
		if(isRestEnvVar == 1)		
		{
			Log_output (-1,"Rest will be initialized overwriting parameters with REST environement variables.\n");
		}				
		else
		{
			Log_output (-1,"Rest will be initialized with specified parameters inside the bench program.\n");
		}		

		Log_output (-1,"\nConfiguration:\n\tprofiler: %d \n\tdecision: %d \n\tfreqChanger: %d \n\tisRestEnvVar %d ",profiler, decisionMaker, freqChanger,isRestEnvVar);

		switch (profiler)
		{
			case REST_T_PROFILER :
				profilerInitFunction = threadedProfilerInit;
				break;
			case REST_VMAD_PROFILER :
				//Nothing to do here
				break;
			default :
				Log_output (-1,"Undefined profiler, the defined ones are : THREADED_PROFILER, VMAD_PROF_PROFILER\n");
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
				Log_output (-1,"Undefined decision maker, the defined ones are : NAIVE_DM, BRANCHPREDICT_DM, MARKOVPREDICT_DM\n");
				assert(0);
				break;
		}

		switch (freqChanger)
		{
			case REST_FREQ_CHANGER:
				break;
			default:
				Log_output (-1, "Undefined decision maker, the defined ones are : FREQ_CHANGER\n");
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
		restHandle->profiler=profiler;
		restHandle->decisionMaker=decisionMaker;
		restHandle->freqChanger=freqChanger;

		Log_output(-1, "Initializing REST Done!\n");

		return 0;
	}
	else
	{
	
		Log_output (-1,"Rest Already initialized...\n");
	}

	return 1;
}

void RestDestroy ( void )
{
	if(restHandle)
	{
    		STPContext *context = restHandle->ptr;
		void (*profilerDestroyFunction) (STPContext * prof);
	
		switch (restHandle->profiler)
		{
			case REST_T_PROFILER :
				profilerDestroyFunction = threadedProfilerDestroy;
				break;
			case REST_VMAD_PROFILER :
				break;
			default :
				Log_output (-1, "Undefined profiler, the defined ones are : THREADED_PROFILER, VMAD_PROF_PROFILER\n");
				assert(0);
				break;
		}
		
		profilerDestroyFunction(context);
		setenv ("LD_PRELOAD","", 1);

		restHandle=NULL;
		Log_output (-1, "Rest was correctly destroyed\n");
		Log_destroy ();
	}
}


