/*
 * FoREST - Reactive DVFS Control for Multicore Processors
 * Copyright (C) 2013 Universite de Versailles
 * Copyright (C) 2011-2012 Exascale Research Center
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


/**
 * @file Common.h
 * Shared functions and properties.
 */

#ifndef H_COMMON
#define H_COMMON

#include <stdint.h>
#include <iostream>
#include <cstdlib>
#include <time.h>

/**
 * Value of the watched hardware counters.
 */
typedef union
{
   struct __attribute__ ((packed))
   {
      uint64_t retired;    // # of retired instructions
      uint64_t refCycles;  // unhalted cycles elapsed at the reference frequency
      uint64_t time;       // time as reported by rdtsc 
   };

   uint64_t values [3];
} HWCounters;

/**
 * How many counters have to be profiled. Does not include time.
 */
#define NB_HW_COUNTERS ((sizeof(HWCounters) / sizeof(uint64_t)) - 1)

/* Defines the runtime mode */
enum Mode {
   MODE_ENERGY,
   MODE_PERFORMANCE
};

/* Defines a cpu couple */
struct CPUCouple {
  unsigned int physicalId;
  unsigned int logicalId; 
};

// a few utility functions which may be useful at some point
#define rest_max(a, b) ((a) > (b) ? (a) : (b))
#define rest_min(a, b) ((a) < (b) ? (a) : (b))
#define rest_abs(a) ((a) < 0 ? -(a) : (a))


/**
 * Check if the pointer is not null
 * If it is, the program will kit with a error message
 * \param pPointer the pointer to check
 */
inline void handleAllocation (void *pPointer) {
	if (pPointer == 0) {
		std::cerr << "Error: One resource couldn't be allocated." << std::endl;
		exit (EXIT_FAILURE);
	}
}

#endif
