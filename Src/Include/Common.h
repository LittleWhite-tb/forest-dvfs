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

#ifndef H_FORESTCOMMON
#define H_FORESTCOMMON

#include <stdint.h>
#include <cstdlib>
#include <time.h>

namespace FoREST {

inline void nsleep (unsigned long nanoseconds) {
   timespec req;
   req.tv_sec = 0;
   req.tv_nsec = nanoseconds*1000;
   nanosleep (&req, NULL);
}

// a few utility functions which may be useful at some point
#define rest_max(a, b) ((a) > (b) ? (a) : (b))
#define rest_min(a, b) ((a) < (b) ? (a) : (b))
#define rest_abs(a) ((a) < 0 ? -(a) : (a))

} // namespace FoREST

#endif
