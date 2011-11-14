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
  @file Likwid.h
  @brief The Likwid class header is in this file 
 */
#ifndef H_LIKWID
#define H_LIKWID

#include "LibProf.h"

/**
 * @class Likwid
 * @brief The Likwid profiler
 */
class Likwid:public LibProf
{
	
	public:
		/**
		 * @brief Constructor
		 */
		Likwid (void);
		
		/**
		 * @brief Destructor
		 */
		virtual ~Likwid (void);
		
		/**
		 * @brief Create the counter listener to profile
 		 * @return return true if success
		 */
		virtual bool creatCounters (void);
		
		/**
		 * @brief accumulate values readden
		 */
		virtual void accumulators (void);
		 
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
};

#endif
