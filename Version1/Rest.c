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

#include <assert.h>
#include <stdio.h>

#include "camus_definitions.h"
#include "NaiveDM.h"
#include "PredictiveDM.h"
#include "MarkovDM.h"
#include "Rest.h"
#include "ThreadedProfiler.h"


void *RestInit (toolChainInit profiler, toolChainInit decisionMaker, toolChainInit freqChanger)
{
	STPContext *(*profilerInitFunction) (SFuncsToUse funcPtrs) = NULL;	
	SFuncsToUse decisionFuncs; 
	
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
			fprintf(stderr, "Not yet implemented for now, please choose the NAIVE_DM \n");
			assert(0);
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
	return handle;
}

void RestDestroy (toolChainInit profiler, void *ptr)
{
    STPContext *handle = ptr;
	void (*profilerDestroyFunction) (STPContext * prof);

	switch (profiler)
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
	
	profilerDestroyFunction(handle);
}


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


/*@todo add the destroy function*/
