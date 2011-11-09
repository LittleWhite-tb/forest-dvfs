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
  @file Papi.h
  @brief The Papi class header is in this file 
 */
#ifndef H_PAPI
#define H_PAPI

/**
 * @class Papi
 * @brief The Papi for hardware counters
 */
class Papi
{
	
	public:
		/**
		 * @brief Constructor
		 */
		Papi (void);
		
		/**
		 * @brief Destructor
		 */
		virtual ~Papi (void);
		
		/**
		 * @brief getTid gives the id of a thread
		 * @return pid_t the thread id
		 */
		virtual pid_t getTid ( void );
		
		/**
		 * @brief getTicks gives the clock's tick
		 * @return unsigned long long tick of the clock
		 */
		unsigned long long getTicks ( void );
		
		/**
		 * @brief initPapiHelper Check to see if the PAPI natives are available
		 * @param int EventSet allows to creat the events corresponding to papi counters
		 */
		void initPapiHelper ( int * EventSet);
		
		/**
		 * @brief start listening to papi counters
		 * @param int EventSet the event related
		 */
		void start (int EventSet);
		
		/**
		 * @brief accumulator This function adds the value you pass to the readings in the hardware counter.
		 * @param int EventSet the event related		 
		 * @param long_long *values values to add in the accumulator
		 */
		void accumulator (int EventSet, long_long *values);
		
		/**
		 * @brief initLibraryPapi initialize the papi library
		 */
		void initLibraryPapi ();
		
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
