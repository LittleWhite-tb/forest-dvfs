#include "Rest.h"
#include <stdio.h>
void * RestHandle;

void restinitfortranwrapper_()
{
	RestHandle=RestInit(REST_T_PROFILER, REST_NAIVE_DM, REST_FREQ_CHANGER);
}



void restdestroyfortranwrapper_()
{
	RestDestroy();
}
