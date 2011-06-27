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


#define MEM_BOUND_ITER (mem_iters)
#define MEM_BOUND_FOOTPRINT (4*1024*1024)

double BigVec[MEM_BOUND_FOOTPRINT];
long mem_iters=(11192);

int
main (void)
{
    printf ("REST Start\n");

    STPContext * handle;

    SFuncsToUse profFuncs={decisionInit,decisionDestruct, decisionGiveReport};

    handle=profilerInit(profFuncs);

   int j;
   int index;
   int k;
   double temp[8];
   for(j=0;j<MEM_BOUND_ITER;j++)
   {
	memcpy(&BigVec[0],&BigVec[MEM_BOUND_FOOTPRINT/2],MEM_BOUND_FOOTPRINT/2*sizeof(*BigVec));
				
    }
    profilerDestroy(handle);

    
    return EXIT_SUCCESS;
}
