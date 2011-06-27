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

#include <stdio.h>
#include <stdlib.h>
#include <ThreadedProfiler.h>

#include <unistd.h>
#include <string.h>
#include <math.h>

#define MEM_BOUND_ITER (mem_iters)
#define MEM_BOUND_FOOTPRINT (4*1024*1024)

double BigVec[MEM_BOUND_FOOTPRINT];
long mem_iters=(11192);

int
main (void)
{
	STPContext *handle;
	SFuncsToUse profFuncs={decisionInit,decisionDestruct, decisionGiveReport};

    double DummyVec1[12]={1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0};
    double DummyVec2[12]={1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0};
    //a 1 gig vector definitely wont fit in cache

    int expected_loop_interval=1000;
    long updown_mem_product=5000;
    long cpu_2_mem_ratio=3793110;
	
	long long cpuBoundIter = 818822656;
	long long memBoundIter = 9217728;

	long long numUpDown = 25 ;
	unsigned long long memBoundFootprint = 4*1024*1024;
	
	double BigVec[4*1024*1024];
	expected_loop_interval = 1000;
	
	
	//calculate loop bounds
	numUpDown = (60*1000)/(expected_loop_interval*2);
	//once we have number of outer loops we use the product to find the memory loop bounds
	memBoundIter = round((float)updown_mem_product/(float)numUpDown);
	cpuBoundIter = memBoundIter*cpu_2_mem_ratio;

	//now start!
    printf ("REST Start\n");
    handle=profilerInit(profFuncs);

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


		for(j=0;j<memBoundIter;j++)
		{
			//memcpy(&BigVec[0],&BigVec[memBoundFootprint/2],memBoundFootprint/2*sizeof(*BigVec));
		}
		
	}
    

    profilerDestroy(handle);

 
    return EXIT_SUCCESS;
}

