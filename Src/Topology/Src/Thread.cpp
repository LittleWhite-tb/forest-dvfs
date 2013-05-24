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
 * @file Thread.cpp
 * The Thread class is in this file
 */

#include <cstring>

#include "Common.h"
#include "Thread.h"
#include "Counter.h"
#include "Profiler.h"

namespace FoREST {

Thread::Thread (unsigned id, unsigned int nbFrequencies, Profiler& profiler, uint64_t threshold) :
id_ (id),
lastUsageComputation (0),
TIME_THRESHOLD (threshold),
maxIpc_ (0),
nbFrequencies_ (nbFrequencies),
profiler_ (profiler),
usage_ (0) {
   // Initialize the counter structures
   retired.name = "INST_RETIRED:ANY_P";
   refCycles.name = "UNHALTED_REFERENCE_CYCLES";

   // Allocate data
   ipc_ = new float [nbFrequencies_];
   retired.values = new CounterValues [2*nbFrequencies_+1];
   execRetired.values = new CounterValues;

   time = retired.values + nbFrequencies_;
   refCycles.values = time + nbFrequencies_;

   // reset the values 
   memset (ipc_, 0, sizeof (*ipc_)*nbFrequencies_);
   memset (retired.values, 0, sizeof (*retired.values)*(2*nbFrequencies_+1));
   memset (execRetired.values, 0, sizeof (*execRetired.values));
   memset (&execTime, 0, sizeof (execTime));

   // Open the HW counters file descriptors
   profiler_.open (retired, id_);
   profiler_.open (refCycles, id_);

   execRetired.fd = retired.fd;
}

Thread::~Thread () {
   delete [] ipc_;
   delete [] retired.values;
   profiler_.close (retired);
   profiler_.close (refCycles); 
}

} // namespace FoREST

