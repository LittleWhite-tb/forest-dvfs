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

#include "Rest.h"
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
	int EventSet = PAPI_NULL;

	xlog_start();
	xlog("restDecisionInit");
	xlog_add(" -> %s",module->header->name);

	CAMUS_DECISION_PARAM (module);
	
	xlog("runtime_decision_block_symbol_name: %s", param->runtime_decision_block_symbol_name);
	xlog("chunk_instrumented_size: %li",  param->chunk_instrumented_size);
	xlog("chunk_initial_original_size: %li",  param->chunk_initial_original_size);
	xlog("chunk_initial_parallel_size: %li",  param->chunk_initial_parallel_size);
	xlog("number: %li",  param->number);
	
	module->data = malloc (sizeof (restData));
	assert (module->data != NULL);
	memset (module->data, 0, sizeof (restData));

	restData *data = module->data;

	//Removed this line it bugs
	data->report.Profreport.data.tp.window = param->chunk_instrumented_size;
	data->context.ProfContext = RestInit (REST_VMAD_PROFILER, REST_NAIVE_DM, REST_FREQ_CHANGER);

	//Sanity check
	int (* DmReport) (void *, SProfReport *) = data->context.ProfContext->myFuncs.reportFunc;

	//Init the DecisionMaker
	void * (*myInit) (void)= data->context.ProfContext->myFuncs.initFunc;
	data->context.DMcontext = myInit ();
	xlog ("DM Report");
	xlog_add (" %p %p %p", DmReport, data, data->context.DMcontext);
	
	//Handle Papi
	initLibraryPapi();

	initThreadPapi ();

	data->context.ProfContext->parent = threadIdPapi();
	
	initPapiHelper (&EventSet, data->context.ProfContext);
	
	data->report.papiEventSet = EventSet;
	
	startPapi(data->report.papiEventSet);

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

	xlog ("Getting data");

	restData * restdata = mymodule->data;
	camus_decision_param_t param = mymodule->header->param;
	
	currentTicks = getTicks ();
	restdata->report.Profreport.data.tp.ticks = currentTicks - oldTicks;
	oldTicks = currentTicks;

	xlog ("Checking papi -");
	
	accumPapi (restdata->report.papiEventSet, values);

	xlog ("Checked papi");
	
	if(values[0]==0 || values[1]==0)//no divide by zeros!
	{
		privateBounded=0.0;
	}
	else
	{
		float value0 = values[0];
		float value1 = values[1];
		float value2 = values[2];
		privateBounded=2.0*16.0*value0*value2 / (value1*value1 + 0.0f);
		privateBounded=(privateBounded>1.0)?1.0:privateBounded;
	}
	fprintf (stderr, "\n\n\n%lld %lld %lld %f\n\n\n", values[0], values[1], values[2], privateBounded);
	
	restdata->report.Profreport.data.tp.bounded=privateBounded;

	xlog ("Updated bounded");
	
	int (* DmReport) (void *, SProfReport *) = restdata->context.ProfContext->myFuncs.reportFunc;
	
	xlog ("Reporting to DM");
	xlog_add (" %p %p %p", DmReport, restdata, restdata->context.DMcontext);

	int res = DmReport (restdata->context.DMcontext, &(restdata->report.Profreport));

	if(res != 0)
	{
		xlog ("Got information: ");
		xlog_add ("chunksize %d, nextWindow %d\n", param->chunk_instrumented_size,
			  				   restdata->report.Profreport.data.tp.nextWindow);

		myWindow = param->chunk_instrumented_size * restdata->report.Profreport.data.tp.nextWindow;
	}
	xlog ("Reported to DM: ");
	xlog_add ("%d -> %d", res, myWindow);

	*lower_bound = restdata->iteration;
	restdata->iteration = *lower_bound + myWindow;
	*upper_bound = restdata->iteration;
	
	camus_decision_log("CHUNK EXIT: version=1, lower_bound=%i, upper_bound=%i, chunk_size=%i", *lower_bound, *upper_bound, param->chunk_instrumented_size);
	
	xlog_end();
	
	return 1;
}
