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

//Enumerations
/** 
 * @brief enumeration for the different kinds of profilers that will feed data to the decision maker
*/
typedef enum eProfType 
{
	VMAD_PROFILER, /**< @brief this enumeration is for the VMAD profiler*/
	THREADED_PROFILER  /**< @brief this enumeration is for the threaded profiler*/
} EProfType;


//Threaded Profiler structure
/** 
 * @struct sThreadProfiler
 * @brief structure that holds reporting and feedback information specifically for the threaded profiler implementation
*/
typedef struct sThreadProfiler
{
			float bounded;/**< @brief This variable is between 0 and 1 where 0 is compute bound and 1 is memory bound*/
			int ticks;/**< @brief This is the size of the report's window in cycles*/
			int algorithm;/**< @brief This just tells us what our new ProfDesc is and also what we are instrumenting*/
			int nextTicks;/**< @brief This is the requested size of the next report's window in cycles*/
}SThreadProfiler;




//definition for the DeicionMaker.h
/** 
 * @brief union that could be any of many different future reporting structures... selected based on prof_id in SProfReport
*/
typedef union ProfilerInfo {
				 SThreadProfiler tp;/**< @brief the threaded profiler entry into the union*/
				} ProfilerInfo;
				

#endif
