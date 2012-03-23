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

/**
 * Value of the watched hardware counters.
 */
typedef struct
{
   uint64_t l2miss;  // # of data L2 cache misses
   uint64_t cycles;  // cycles elapsed, considering the current freq
   uint64_t refCycles;  // cycles elapsed at a reference frequency
} HWCounters;

// a few utility functions which may be usefull at some point
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))
#define abs(a) ((a) < 0 ? -(a) : (a))

#endif
