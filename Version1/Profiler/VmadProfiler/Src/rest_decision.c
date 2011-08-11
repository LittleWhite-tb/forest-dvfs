#include <math.h>
#include <papi.h>
#include <stdio.h>
#include <stdlib.h>
#include <syscall.h>

#include "vmad_rest_definition.h"
#include "camus_interpolation.h"
#include "camus_decision.h"
#include "camus_dep_tst.h"
#include "camus_decision_transaction.h"

#include "Profilers.h"
#include "papiCounters.h"
#include "NaiveDM.h"

#include "rest_decision.h"




void camus_bind (camus_module_t module) 
{
	module->init = restDecisionInit;
	module->on = restDecisionOn;
	module->off = restDecisionOff;
	module->quit = restDecisionQuit;
	module->reset = restDecisionReset;
}


void restDecisionInit(camus_module_t module)
{
	xlog_start();
	xlog("restDecisionInit");
	xlog_add(" -> %s",module->header->name);

	CAMUS_DECISION_PARAM (module);
	
	xlog("runtime_decision_block_symbol_name: %s", param->runtime_decision_block_symbol_name);
	xlog("chunk_instrumented_size: %li",  param->chunk_instrumented_size);
	xlog("chunk_initial_original_size: %li",  param->chunk_initial_original_size);
	xlog("chunk_initial_parallel_size: %li",  param->chunk_initial_parallel_size);
	xlog("number: %li",  param->number);
	
	
	CAMUS_DECISION_DATA (module);

	//Removed this line it bugs
	//data->report.Profreport.data.tp.window = param->chunk_instrumented_size;
	
	void* handle = dlopen(NULL, RTLD_LAZY);
	if (handle == NULL)
	{
		xlog("Error opening self image: %s", dlerror());
		exit(EXIT_FAILURE);
	}
	
	dlerror(); // Clear any existing error.

	char* error;
	void* sym = dlsym(handle, (const char*) param->runtime_decision_block_symbol_name);
	
	xlog ("Here");
	xlog_add (": %s\n", (const char*) param->runtime_decision_block_symbol_name);

	if ((error = dlerror()) != NULL)
	{
		xlog("Error loading symbol '%s': %s", (const char*)param->runtime_decision_block_symbol_name, error);
		exit(EXIT_FAILURE);
	}

	xlog ("Here:");
        xlog_add ("%p\n", sym);
	decision_t* d = (decision_t*) sym;
	d->decision_block = camus_decision_chunk;
	d->module = module;
	
	xlog_end();
}

void restDecisionQuit(camus_module_t module)
{
	xlog_start();
	xlog("restDecisionQuit");
	xlog_add(" -> %s",module->header->name);
	
	xlog_end();
}

void restDecisionOn(camus_module_t module)
{
	xlog_start();
	xlog("restDecisionOn");
	xlog_add(" -> %s",module->header->name);
	
	xlog_end();
}

void restDecisionOff(camus_module_t module)
{
	xlog_start();
	xlog("restDecisionOff");
	xlog_add(" -> %s",module->header->name);
	
	xlog_end();
}

void restDecisionReset(camus_module_t module)
{
	xlog_start();
	xlog("restDecisionReset");
	xlog_add(" -> %s",module->header->name);
	
	xlog_end();
}


int camus_decision_chunk(void* module, int* lower_bound, int* upper_bound)
{
	xlog_start();
	xlog("camus_decision_chunk");
	
	float privateBounded;
	int myWindow = -1;
	long_long values[3]= {0,0,0};
	unsigned long long currentTicks = 0;
	static unsigned long long oldTicks = 0;
	
	camus_module_t mymodule = module;

	restData * restdata = mymodule->data;
	camus_decision_param_t param = mymodule->header->param;
	
	currentTicks = getTicks ();
	restdata->report.Profreport.data.tp.ticks = currentTicks - oldTicks;
	oldTicks = currentTicks;
	
	accumPapi(restdata->report.papiEventSet, values);
	
	if(values[0]==0 || values[1]==0)//no divide by zeros!
	{
		privateBounded=0.0;
	}
	else
	{
		privateBounded=2*16*values[0]*values[2]/(values[1]*values[1]);
		privateBounded=(privateBounded>1.0)?1.0:privateBounded;
	}
	
	restdata->report.Profreport.data.tp.bounded=privateBounded;
	
	
	int (* DmReport) (void *, SProfReport *) = restdata->context.ProfContext->myFuncs.reportFunc;
	
	if(DmReport (restdata->context.DMcontext, &(restdata->report).Profreport))
	{
		myWindow = param->chunk_instrumented_size * restdata->report.Profreport.data.tp.nextWindow;
	}

	
	*lower_bound = *upper_bound;
	upper_bound += myWindow;
	
	camus_decision_log("CHUNK EXIT: version=1, lower_bound=%i, upper_bound=%i, chunk_size=%i", *lower_bound, *upper_bound, param->chunk_instrumented_size);
	
	xlog_end();
	
	return 1;
}
