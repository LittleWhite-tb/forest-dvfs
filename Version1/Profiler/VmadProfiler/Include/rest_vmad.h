#ifndef REST_VMAD
#define REST_VMAD

#include "camus_definitions.h"
#include "vmad_rest_definition.h"

#include "Profilers.h"
#include "DecisionStructures.h"





void camus_bind (camus_module_t module);


void restInit(camus_module_t module);
void restQuit(camus_module_t module);
void restOn(camus_module_t module);
void restOff(camus_module_t module);
void restReset(camus_module_t module);

#endif


