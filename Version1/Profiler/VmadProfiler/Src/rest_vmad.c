#include <stdio.h>
#include <stdlib.h>
#include "rest_vmad.h"


void restBind(restModule *module)
{
	module->init = restInit(module);
	module->on = restOn (module);
	module->off = restOff (module);
	module->quit = restQuit (module);
	module->reset = restReset (module);
}


void *restInit(restModule *module)
{
	restOn(module);
	return NULL;
}

void *restQuit(restModule *module)
{
	//destroy la chaine rest
	return NULL;
}

void *restOn(restModule *module)
{
	//choper les compteurs papi
	//init la chaine de rest
	return NULL;
}
void *restOff(restModule *module)
{
	return NULL;
}
void *restReset(restModule *module)
{
	return NULL;
}
