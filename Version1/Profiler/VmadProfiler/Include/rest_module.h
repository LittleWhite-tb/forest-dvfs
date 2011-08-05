#ifndef REST_MODULE
#define REST_MODULE

#include "Profilers.h"
#include "DecisionStructures.h"


typedef struct _RESTcontext
{
	STPContext *ProfContext;
	void * DMcontext;
}RESTcontext;


typedef struct _Report
{
	SProfReport Profreport;
	int papiEventSet;
}Report;

typedef struct _restModule
{
	void * (* init) (struct _restModule *module);
	void * (* quit) (struct _restModule *module);
	void * (* reset) (struct _restModule *module);
	void * (* on) (struct _restModule *module);
	void * (* off) (struct _restModule *module);
	RESTcontext context;
	Report report;
	
}restModule;

void restModuleLoad (restModule *mymodule);
void restModuleUnload (restModule *mymodule);

#endif

