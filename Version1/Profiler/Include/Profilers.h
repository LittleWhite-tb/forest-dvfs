#ifndef PROFILERS_H
#define PROFILERS_H

//Enumerations

#define INSTRUMENTED_PROFILER 2
#define THREADED_PROFILER 4

typedef ProfDesc int;//should use the Enumerations above



//Threaded Profiler structures
typedef struct sThreadedProfContext{
			int algorithm;//This just tells us what our new ProfDesc is and also what we are instrumenting
			int next_ticks;//This is the requested size of the next report's window in cycles
			}SThreadedProfContext;


typedef struct sThread_Profiler{
			float bounded;//This variable is between 0 and 1 where 0 is compute bound and 1 is memory bound
			int ticks;//This is the size of the report's window in cycles
			SThreadedProfContext * ptr;//This provides an allocation in any of the profilers to get feedback from the decisionmaker
			}SThread_Profiler;




//definition for the DeicionMaker.h

typedef union ProfilerInfo {
				 SThread_Profiler tp;
				} ProfilerInfo;
				

#endif
