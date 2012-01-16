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

/**
  @file PapiCounters.h
  @brief The PapiCounters class header is in this file 
 */
#ifndef H_PAPICOUNTERS
#define H_PAPICOUNTERS

#include <papi.h>
#include <sys/types.h>
#include <unistd.h>

#include "LibProf.h"

/**
 * @class PapiCounters
 * @brief The PapiCounters for hardware counters
 */
class PapiCounters:public LibProf
{
	
	public:
		/**
		 * @brief Constructor
		 */
		PapiCounters (void);
		
		/**
		 * @brief Destructor
		 */
		virtual ~PapiCounters (void);
		
		/**
		 * @brief getTid gives the id of a thread
		 * @return pid_t the thread id
		 */
		virtual pid_t getTid ( void );
		
		/**
		 * @brief initPapiHelper Check to see if the PAPI natives are available
		 * @param EventSet allows to creat the events corresponding to papi counters
		 */
		void initPapiHelper ( int * EventSet);
		
		/**
		 * @brief start listening to papi counters
		 * @param EventSet the event related
		 */
		virtual void startCounters (int EventSet);
		
		/**
		 * @brief accumulator This function adds the value you pass to the readings in the hardware counter.
		 * @param EventSet the event related		 
		 * @param values values to add in the accumulator
		 */
		virtual void accumulator (int EventSet, long_long *values);
		
		/**
		 * @brief closing the listen of counters
		 * @return return true if success
		 */
		virtual bool closeCounters (void);
		
		/**
		 * @brief getTicks gives the clock's tick
		 * @return unsigned long long tick of the clock
		 */
		virtual unsigned long long getTicks ( void );
		
		/**
		 * @brief startLibrary initialize the profiler's library
		 */
		virtual void startLibrary (...);
		
		/**
		 * @brief stopLibrary terminate the profiler's library
		 */
		virtual void stopLibrary (void);
		
		/**
		 * @brief intiThreadPapi verify that the init function registered well 
		 */
		void initThreadPapi (void);
		
		/**
		 * Return the thread's papi ID
		 */
		PAPI_thread_id_t threadIdPapi ();
};

#endif
