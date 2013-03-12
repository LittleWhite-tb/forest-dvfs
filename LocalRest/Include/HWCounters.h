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
 * @file HWCounters.h
 * HW counters structure file
 */

#ifndef H_HWCOUNTERS
#define H_HWCOUNTERS

#include <cstdlib>
#include <stdint.h>

namespace FoREST {

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


} // namespace FoREST

#endif
