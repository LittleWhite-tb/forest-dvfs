#ifndef DECISIONMAKER_H
#define DECISIONMAKER_H

//#include <Profilers.h>






typedef struct sProfReport{
	ProfDesc prof_id;
	int proc_id;
	ProfilerInfo data;
	} SProfReport;


//function return 0 if no additional action is required by the profiler and 1 if context needs to be examined
int Decision_give_report( SProfReport *);

#endif 
