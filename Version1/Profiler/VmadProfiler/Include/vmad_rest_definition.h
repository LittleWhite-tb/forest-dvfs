
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
	long long iteration;
	
}restData;
