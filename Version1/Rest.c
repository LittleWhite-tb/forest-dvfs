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

#include "Rest.h"

void *RestInit (toolChainInit profiler, toolChainInit decisionMaker, toolChainInit freqChanger)
{
	STPContext *(*profilerInitFunction) (SFuncsToUse funcPtrs);	
    SFuncsToUse decisionFuncs; 
	
	switch (profiler)
	{
		case T_PROFILER :
			profilerInitFunction = threadedProfilerInit;
			break;
		case WMAD_PROFILER :
			fprintf(stderr, "Not yet implemented for now, please choose the THREADED_PROFILER\n");
			assert(0);
			break;
		default :
			fprintf(stderr, "Undefined profiler, the defined ones are : THREADED_PROFILER, WMAD_PROFILER\n");
			assert(0);
			break;
	}
	
	
	switch (decisionMaker)
	{
		case NAIVE_DM:
			decisionFuncs.initFunc =  naiveDecisionInit;
			decisionFuncs.destroyFunc = naiveDecisionDestruct;
			decisionFuncs.reportFunc =  naiveDecisionGiveReport;
			break;
		case BRANCHPREDICT_DM:
			fprintf(stderr, "Not yet implemented for now, please choose the NAIVE_DM \n");
			assert(0);
			break;
		case MARKOVPREDICT_DM:
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
		case FREQ_CHANGER:
			break;
		default:
			fprintf(stderr, "Undefined decision maker, the defined ones are : FREQ_CHANGER\n");
			assert(0);
			break;
	}
	
	STPContext *handle = profilerInitFunction (decisionFuncs);
    return handle;
}

void RestDestroy (toolChainInit profiler, void *ptr)
{
    STPContext *handle = ptr;
	void (*profilerDestroyFunction) (STPContext * prof);

	switch (profiler)
	{
		case T_PROFILER :
			profilerDestroyFunction = threadedProfilerDestroy;
			break;
		case WMAD_PROFILER :
			fprintf(stderr, "Not yet implemented for now, please choose the THREADED_PROFILER\n");
			assert(0);
			break;
		default :
			fprintf(stderr, "Undefined profiler, the defined ones are : THREADED_PROFILER, WMAD_PROFILER\n");
			assert(0);
			break;
	}
	
	profilerDestroyFunction(handle);
}
