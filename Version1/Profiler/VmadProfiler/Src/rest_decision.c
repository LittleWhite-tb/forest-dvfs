#include <math.h>
#include <papi.h>
#include <stdio.h>
#include <stdlib.h>
#include <syscall.h>

#include "vmad_rest_definition.h"
#include "camus_interpolation.h"
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
	
	xlog("runtime_decision_block_symbol_name: %s", module->header->param->runtime_decision_block_symbol_name);
	xlog("chunk_instrumented_size: %li",  module->header->param->chunk_instrumented_size);
	xlog("chunk_initial_original_size: %li",  module->header->param->chunk_initial_original_size);
	xlog("chunk_initial_parallel_size: %li",  module->header->param->chunk_initial_parallel_size);
	xlog("number: %li",  module->header->param->number);
	
	module->data->report.Profreport.data.tp.window = module->header->param->chunk_instrumented_size;
	
	
	void* handle = dlopen(NULL, RTLD_LAZY);
	if (handle == NULL)
	{
		xlog("Error opening self image: %s", dlerror());
		exit(EXIT_FAILURE);
	}
	
	dlerror(); // Clear any existing error.

	char* error;
	void* sym = dlsym(handle, (const char*) module->header->param->runtime_decision_block_symbol_name);

	if ((error = dlerror()) != NULL)
	{
		xlog("Error loading symbol '%s': %s", (const char*)module->header->param->runtime_decision_block_symbol_name, error);
		exit(EXIT_FAILURE);
	}

	decision_t* d = (decision_t*) sym;
	d->decision_block = camus_decision_chunk;
	d->module = self;
	
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
	static float lastBoundedValue = 0.5;
	int myWindow;
	long_long values[3]= {0,0,0};
	unsigned long long currentTicks = 0;
	static unsigned long long oldTicks = 0;
	
	currentTicks = getTicks ();
	module->data->report.Profreport.data.tp.ticks = currentTicks - oldTicks;
	oldTicks = currentTicks;
	
	accumPapi(module->data->report.papiEventSet, values);
	
	if(values[0]==0 || values[1]==0)//no divide by zeros!
	{
		privateBounded=0.0;
	}
	else
	{
		privateBounded=2*16*values[0]*values[2]/(values[1]*values[1]);
		privateBounded=(privateBounded>1.0)?1.0:privateBounded;
	}
	
	module->data->report.Profreport.data.tp.bounded=privateBounded;
	
	
	int (* DmReport) (void *, SProfReport *) = module->data->context.ProfContext->myFuncs.reportFunc;
	
	if(DmReport (module->context.DMcontext, &(module->data->report).Profreport))
	{
		myWindow = module->header->param->chunk_instrumented_size * module->report.Profreport.data.tp.nextWindow;
	}

	lastBoundedValue=privateBounded;
	
	*lower_bound = *upper_bound;
	upper_bound += myWindow;
	
	camus_decision_log("CHUNK EXIT: version=%i, lower_bound=%i, upper_bound=%i, chunk_size=%i",version, *lower_bound, *upper_bound, data->chunk_size);
	
	xlog_end();
	
	return 1;
}
