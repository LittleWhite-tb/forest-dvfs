/*
Copyright (C) 2011 Exascale Research Center

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "Log.h"
#include "ThreadedProfiler.h"

#define MEM_BOUND_FOOTPRINT (4*1024*1024)

/** 
 * @brief Perform the benchmark 
 */
static void 
benchmark (void)
{
    //Dummy arrays
    double DummyVec1[12]={1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0};
    double DummyVec2[12]={1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0};
 
    //Lots of random variables to make it all happy
    long updown_mem_product=5000;
    long cpu_2_mem_ratio=3793110;

    //Set this to 1, it was: (60*1000)/(expected_loop_interval*2);
    //No longer needed for the moment: int expected_loop_interval=1000;
	long long numUpDown = 1;

    unsigned long long memBoundFootprint = 4*1024*1024;
	long long memBoundIter = round((float)updown_mem_product/(float)numUpDown);
	long long cpuBoundIter = memBoundIter*cpu_2_mem_ratio;

    //Our memory array
	double *BigVec = NULL;
    BigVec = malloc (MEM_BOUND_FOOTPRINT * sizeof (*BigVec));
    assert (BigVec != NULL);

	//calculate loop bounds
	//once we have number of outer loops we use the product to find the memory loop bounds

	int i;
	for(i=0; i < numUpDown; i++)
	{
		//Now we do something cpu bound
		int j;	
		double temp[4];
		
        //force everything to stay in registers and unroll
		temp[0]=DummyVec1[0];
		temp[1]=DummyVec1[1];
		temp[2]=DummyVec1[2];
		temp[3]=DummyVec1[3];
		
        //Do CPU
        for(j=0;j<cpuBoundIter;j++)
		{
			temp[0]=temp[0]*temp[0];
			temp[1]=temp[1]*temp[1];
			temp[2]=temp[2]*temp[2];
			temp[3]=temp[3]*temp[3];
			
		}

		DummyVec2[0]=temp[0];
		DummyVec2[1]=temp[1];
		DummyVec2[2]=temp[2];
		DummyVec2[3]=temp[3];

        //Do Memory
		for(j=0;j<memBoundIter;j++)
		{
			memcpy(&BigVec[0],&BigVec[memBoundFootprint/2],memBoundFootprint/2*sizeof(*BigVec));
		}
	}

    //Delete array
    free (BigVec), BigVec = NULL;
}

int
main (int argc, char **argv)
{
	STPContext *handle;
	SFuncsToUse profFuncs={decisionInit,decisionDestruct, decisionGiveReport};

    //Set logger
    Log_setOutput (stderr);
	
    if (argc > 1)
        Log_setVerbosity (50000);

    //now start!
    Log_output (0, "REST Start\n");

    handle = profilerInit (profFuncs);

    benchmark ();

    profilerDestroy (handle), handle = NULL;

    Log_output (0, "REST Stop\n");
 
    return EXIT_SUCCESS;
}

