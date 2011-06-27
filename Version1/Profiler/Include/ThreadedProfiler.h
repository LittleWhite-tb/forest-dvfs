/*
Copyright (C) 2011 Exascale Compûting Research

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

#ifndef H_THREADEDPROFILER
#define H_THREADEDPROFILER

#include <pthread.h>


#define LOWERTHRESHOLD (.9)
#define UPPERTHRESHOLD (1.1)
#define FIRSTSLEEP (200)  
#define LONGESTSLEEP (1000000)



typedef struct sTPContext
{
	int  * volatile killSig;  /**< @brief address of kill signal that we spin on*/
	pthread_t parent; /**< @brief tid used by papi to access the counters*/
} STPContext;   

/**
 * @brief initializes the profiler through a pthread_create
* @return returns a pointer to the profile thread's context
 **/
STPContext profilerInit (void);


/**

 * @brief signals the profiler to destroy it's self
 * @parameter takes the context of the profiling thread to send the kill signal
 **/
void profilerDestroy (STPContext);




#endif
