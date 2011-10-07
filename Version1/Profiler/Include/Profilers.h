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

#ifndef H_PROFILERS
#define H_PROFILERS
#include <papi.h>

//Enumerations
/** 
 * @brief enumeration for the different kinds of profilers that will feed data to the decision maker
*/
typedef enum eProfType 
{
	VMAD_PROFILER, /**< @brief this enumeration is for the VMAD profiler*/
	THREADED_PROFILER  /**< @brief this enumeration is for the threaded profiler*/
} EProfType;

//common define for all the profilers/decisions 
#define THRESHOLD (.05)
#define FIRSTSLEEP (6000)  
#define LONGESTSLEEP (128000)

#define FIRSTSLEEPITERATION (10)  
#define LONGESTSLEEPITERATION (100)


// Comon structure for all the profilers/decisions
/** 
 * @struct sFuncsToUse
 * @brief pointer table which is passed to the profiler to tell it who to initialize, report to, and destroy
 */
typedef struct sFuncsToUse
{
	void * initFunc;  /**< @brief function to use when you call the init*/
	void * destroyFunc;	/**< @brief function to use when you call the destroy */
	void * reportFunc; /**< @brief function to use when you should report*/
	
} SFuncsToUse; 

/** 
 * @struct sTPContext
 * @brief a context with all necessary information for PAPI implmentation and threading
*/ 
typedef struct sTPContext
{
	int  volatile * volatile killSig;  /**< @brief address of kill signal that we spin on*/
	int core;	/**< @brief physical core number to change its frequency */
	PAPI_thread_id_t parent; /**< @brief tid used by papi to access the counters*/
	pthread_t join_id;   /**<  @brief this tid is used for the join at the end of the program since stack allocated variables can be mangled*/
	SFuncsToUse myFuncs; /**<  @brief functions given by my parent for when I should init, destroy, or create*/
} STPContext; 




//Threaded Profiler structure
/** 
 * @struct sThreadProfiler
 * @brief structure that holds reporting and feedback information specifically for the threaded profiler implementation
*/
typedef struct sThreadProfiler
{
			float bounded;/**< @brief This variable is between 0 and 1 where 0 is compute bound and 1 is memory bound*/
			int ticks;/**< @brief This is the size of the report's window in cycles*/
			int window; /**< @brief this is the size of the report's window in usleep value*/
			int algorithm;/**< @brief This just tells us what our new ProfDesc is and also what we are instrumenting*/
			int nextWindow;/**< @brief This is the requested size of the next report's window in in usleep values*/
}SThreadProfiler;




//definition for the DeicionMaker.h
/** 
 * @brief union that could be any of many different future reporting structures... selected based on prof_id in SProfReport
*/
typedef union ProfilerInfo {
				 SThreadProfiler tp;/**< @brief the threaded profiler entry into the union*/
				} ProfilerInfo;
				

#endif
