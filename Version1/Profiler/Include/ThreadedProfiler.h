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
#include <papi.h>


#define THRESHOLD (.05)
#define FIRSTSLEEP (600)  
#define LONGESTSLEEP (128000)


typedef struct sFuncsToUse
{
	void * initFunc;  /**< @brief function to use when you call the init*/
	void * destroyFunc;	/**< @brief function to use when you call the destroy */
	void * reportFunc; /**< @brief function to use when you should report*/
	
} SFuncsToUse;  

typedef struct sTPContext
{
	int  volatile * volatile killSig;  /**< @brief address of kill signal that we spin on*/
	int core;	/**< @brief physical core number to change its frequency */
	PAPI_thread_id_t parent; /**< @brief tid used by papi to access the counters*/
	pthread_t join_id;   /**<  @brief this tid is used for the join at the end of the program since stack allocated variables can be mangled*/
	SFuncsToUse myFuncs; /**<  @brief functions given by my parent for when I should init, destroy, or create*/
} STPContext;   

/**
 * @brief initializes the profiler through a pthread_create
* @return returns a pointer to the profiler thread's context
* @param funcPtrs a struct of function pointers that I want the profiler to use for its lifetime
 **/
STPContext * profilerInit (SFuncsToUse funcPtrs);


/**

 * @brief signals the profiler to destroy it's self
 * @param context takes the context of the profiling thread to send the kill signal
 **/
void profilerDestroy (STPContext *context);




#endif
