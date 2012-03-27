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
 * @fileProfiler.h
 * The Profiler class header is here.
 */
#ifndef H_PROFILER
#define H_PROFILER

#include "Common.h"
#include "DVFSUnit.h"

/**
 * @class Profiler
 * The root of all profiler implementations.
 */
class Profiler
{

   public:
      /**
       * Constructor
       *
       * @param unit The DVFS unit we are profiling.
       */
      Profiler (DVFSUnit & dvfsUnit);

      /**
       * Destructor
       */
      virtual ~Profiler (void);

      /**
       * Reads the counter values and "resets" them.
       *
       * @param hwc The hardware counter structure to fill with the results.
       */
      virtual void read (HWCounters & hwc) = 0;

   protected:

      /**
       * A copy of the DVFS unit we are profiling.
       */
      DVFSUnit & unit;
};

#endif
