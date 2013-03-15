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
#include "HWCounters.h"
#include "Profiler.h"

namespace FoREST {

Thread::Thread (unsigned id, unsigned int nbFrequencies, Profiler& profiler) :
id_ (id),
nbFrequencies_ (nbFrequencies),
profiler_ (profiler) {
   // Allocate data
   hwc_ = new HWCounters [2*nbFrequencies_];
   oldHwc_ = hwc_ + nbFrequencies_;
   ipc_ = new float [nbFrequencies_];

   // reset the values
   memset (hwc_, 0, sizeof (*hwc_)*2*nbFrequencies_);
   memset (ipc_, 0, sizeof (*ipc_)*nbFrequencies_);

   // Open the HW counters file descriptors
   profiler_.open (id_, fd);
}

Thread::~Thread () {
   delete [] hwc_;
   delete [] ipc_;
   for (unsigned int i = 0; i < NB_HW_COUNTERS; i++) {
      close (fd[i]);
   }
}

bool Thread::read (unsigned int frequencyId) {
   if (nbFrequencies_ < frequencyId) {
      return false;
   }
   HWCounters& hwc = hwc_ [frequencyId];
   HWCounters& oldHwc = oldHwc_ [frequencyId];
   bool ret = profiler_.read (fd, hwc, oldHwc);
  /* 
   std::cerr << "HWC #" << id_ << ": {(" << NB_HW_COUNTERS << ") " << hwc.time << ", ";
   for (unsigned int i = 0; i < NB_HW_COUNTERS; i++) {
      std::cerr << hwc.values [i] << ", ";
   }
   std::cerr << "}" << std::endl;*/

   return ret;
}

} // namespace FoREST

