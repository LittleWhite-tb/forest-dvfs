#include "Rest.h"
void * RestHandle;

void restinitfortranwrapper_()
{
	#ifdef THREAD_REST
	RestHandle=RestInit(T_PROFILER, NAIVE_DM, FREQ_CHANGER);
	#endif
}



restdestroyfortranwrapper_()
{
	#ifdef THREAD_REST
	RestDestroy(T_PROFILER,RestHandle);
	#endif
}
