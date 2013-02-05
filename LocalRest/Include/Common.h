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
 * How many counters have to be profiled. Does not account for the time.
 */
#define NB_HW_COUNTERS ((sizeof(HWCounters) - 1) / sizeof(uint64_t))

/* Defines the runtime mode */
enum {ENERGY_SAVER = 0, PERFORMANCE};

/* Defines a cpu couple */
struct CPUCouple {
  unsigned int physicalId;
  unsigned int logicalId; 
};

// a few utility functions which may be useful at some point
#define rest_max(a, b) ((a) > (b) ? (a) : (b))
#define rest_min(a, b) ((a) < (b) ? (a) : (b))
#define rest_abs(a) ((a) < 0 ? -(a) : (a))


// TODO comment
inline void handleAllocation (void *toTest) {
	if (toTest == 0) {
		std::cerr << "Error: One resource couldn't be allocated." << std::endl;
		exit (EXIT_FAILURE);
	}
}

#endif
