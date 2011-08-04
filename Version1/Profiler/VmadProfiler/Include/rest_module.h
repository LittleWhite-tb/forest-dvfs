#ifndef REST_MODULE
#define REST_MODULE

#include "Profilers.h"

void restModuleLoad ();
void restModuleUnload ();

typedef struct _restModule
{
	void * (* init) (struct _restModule *module);
	void * (* quit) (struct _restModule *module);
	void * (* reset) (struct _restModule *module);
	void * (* on) (struct _restModule *module);
	void * (* off) (struct _restModule *module);
	STPContext *context;
	
}restModule;


#endif

