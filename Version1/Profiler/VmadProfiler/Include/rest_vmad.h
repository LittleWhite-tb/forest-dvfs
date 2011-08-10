#ifndef REST_VMAD
#define REST_VMAD

#include "camus_definitions.h"


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

typedef struct _restData
{
	RESTcontext context;
	Report report;
	
}restData;


void camus_bind (camus_module_t module);


void restInit(camus_module_t module);
void restQuit(camus_module_t module);
void restOn(camus_module_t module);
void restOff(camus_module_t module);
void restReset(camus_module_t module);

#endif


