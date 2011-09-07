#include <stdio.h>
#include "likwidCounters.h"

#define NBTHREADS 4

int main (int argc, char *argv[])
{
	unsigned currentThread, currentCounter;
	unsigned nbCounters;
	
	Likwid_startLibrary (NBTHREADS);
	
	/* Starts the counters for thread 0 */
	Likwid_startCountersThread (0);
	// Do computation stuff
	Likwid_startCountersThread (0);
	
	Likwid_readCountersThread (0);
	
	nbCounters = Likwid_getCountersNumber ();
	
	for (currentThread = 0; currentThread < NBTHREADS; currentThread += 1)
	{
		fprintf (stderr, "Results for thread : %u\n", currentThread);
		for (currentCounter = 0; currentCounter < nbCounters; currentCounter += 1)
		{
			fprintf (stderr, "\t - Counter %u: %s, %llu\n",
				currentCounter,
				Likwid_getCounterName (currentCounter),
				(unsigned long long) Likwid_getResult (currentThread, currentCounter));
		}
		
	}
	
	/* Starts all the counters */
	Likwid_startCounters ();
	// Do computation stuff
	
	Likwid_stopLibrary ();
	
	(void) argc;
	(void) argv;
	return 0;
}

