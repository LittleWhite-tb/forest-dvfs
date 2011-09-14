#include <stdio.h>
#include "likwidCounters.h"

#define NBTHREADS 4
q#define SIZE 10000

int main (int argc, char *argv[])
{
	unsigned currentThread, currentCounter, i;
	unsigned nbCounters;
	double A[SIZE], B[SIZE], C[SIZE];
	double sum = 0.;
	
	for (i = 0; i < SIZE; i += 1)
	{
		A[i] = B[i] = C[i] = i * 3.14;
	}
	
	Likwid_startLibrary (NBTHREADS);
	
	/* Starts the counters for thread 0 */
	Likwid_startCountersThread (0);
	
	/* Computation stuff */
	for (i = 0; i < SIZE-1; i += 1)
	{
		A[i] = B[i] * C[i] + B[i+1];
		sum += A[i];
	}
	fprintf (stderr, "sum = %e\n", sum);
	
	/* Stops the counters for thread 0 */
	Likwid_stopCountersThread (0);
	
	Likwid_readCountersThread (0);
	
	nbCounters = Likwid_getCountersNumber ();
	
	/* Print things */
	fprintf (stderr, "************* THREAD 0\n");
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
	
	/* Computation stuff */
	for (i = 0; i < SIZE-1; i += 1)
	{
		A[i] = B[i] * C[i] + B[i+1];
		sum += A[i];
	}
	fprintf (stderr, "sum = %e\n", sum);
	Likwid_stopCounters ();
	
	Likwid_readCounters ();
	
	/* Print things */
	fprintf (stderr, "\n\n************* ALL THREADS\n");
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
	
	Likwid_stopLibrary ();
	
	(void) argc;
	(void) argv;
	return 0;
}

