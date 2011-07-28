#include <assert.h>
#include <stdio.h>
#include "Rest.h"


void RestInit (toolChainInit profiler, toolChainInit decisionMaker, toolChainInit freqChanger)
{
	STPContext *(*profilerInitFunction)(SFuncsToUse funcPtrs);	
	void (*profilerDestroyFunction)(STPContext * prof);

	
	switch (profiler)
	{
		case T_PROFILER :
			profilerInitFunction = threadedProfilerInit;
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
	
	
	switch (decisionMaker)
	{
		case NAIVE_DM:
			decisionFuncs.initFunc =  naiveDecisionInit;
			decisionFuncs.destroyFunc = naiveDecisionDestruct;
			decisionFuncs.reportFunc =  naiveDecisionGiveReport;
			break;
		case BRANCHPREDICT_DM:
			decisionFuncs.initFunc =  branchDecisionInit;
			decisionFuncs.destroyFunc = branchDecisionDestruct;
			decisionFuncs.reportFunc =  branchDecisionGiveReport;
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
	profilerDestroyFunction(handle);
	
}

/*@todo add the destroy function*/
