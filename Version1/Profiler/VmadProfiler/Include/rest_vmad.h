#ifndef REST_VMAD
#define REST_VMAD

#include "rest_module.h"

void restBind(restModule *module);


void *restInit(restModule *module);
void *restQuit(restModule *module);
void *restOn(restModule *module);
void *restOff(restModule *module);
void *restReset(restModule *module);

#endif


