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
 * @file Profiler.cpp
 * The Profiler class is in this file
 */

#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <sys/ioctl.h>
#include <set>
#include <stdint.h>
#include <string.h>
#include <cassert>

#include "Profiler.h"
#include "DVFSUnit.h"
#include "perfmon/perf_event.h"
#include "perfmon/pfmlib_perf_event.h"
#include "glog/logging.h"

namespace FoREST {

// definition of the static fields
pthread_mutex_t Profiler::pfmInitMtx = PTHREAD_MUTEX_INITIALIZER;
unsigned int Profiler::nbPfmInit = 0;

Profiler::Profiler (DVFSUnit & dvfsUnit) : unit (dvfsUnit) {
   // libpfm init
   Profiler::pfmInit (); 
}

void Profiler::open (unsigned int threadId, int fd[NB_HW_COUNTERS]) { 
  const char * counters [] =
   {
      "INST_RETIRED:ANY_P",
#if defined (ARCH_SNB) || defined(ARCH_IVB)
      "UNHALTED_REFERENCE_CYCLES"
#else
#warning "Unknown micro-architecture! CPU cycle counting may be wrong."
      "UNHALTED_REFERENCE_CYCLES"
#endif
   };

   int res;
   
   // initialize counters for the current cpu
   for (unsigned int i = 0; i < NB_HW_COUNTERS; i++)
   {
      struct perf_event_attr attr;
      pfm_perf_encode_arg_t arg;

      // initialize the structure
      memset (&attr, 0, sizeof (attr));
      memset (&arg, 0, sizeof (arg));
      arg.attr = &attr;
      arg.fstr = 0;
      arg.size = sizeof (arg);

      // encode the counter
      res = pfm_get_os_event_encoding (counters [i],  PFM_PLM0 | PFM_PLM1 |
                                                      PFM_PLM2 | PFM_PLM3,
                                       PFM_OS_PERF_EVENT_EXT, &arg);
      CHECK (res == PFM_SUCCESS) << "Failed to get counter " << counters [i]
         << " on cpu " << threadId << std::endl;

      // request scaling in case of shared counters
      arg.attr->read_format = PERF_FORMAT_TOTAL_TIME_ENABLED | PERF_FORMAT_TOTAL_TIME_RUNNING;

      // open the corresponding file on the system
      fd [i] = perf_event_open (arg.attr, -1, threadId, -1, 0);
      CHECK (fd [i] != -1) << "Cannot open counter "
         << counters [i] << " on cpu " << threadId << std::endl;

      // Activate the counter
      CHECK (ioctl (fd [i], PERF_EVENT_IOC_ENABLE, 0) == 0)
         << "Cannot enable event " << counters [i] << " on cpu " << threadId
         << std::endl;
   }
}

Profiler::~Profiler ()
{
   // close pfm
   Profiler::pfmTerminate ();
}

bool Profiler::read (int fd [NB_HW_COUNTERS], HWCounters & hwc, HWCounters& old)
{
   int res;

   //DLOG (INFO) << "reading hwc for cpu " << cpu << std::endl;
   
   // specific case of the time stamp counter
   uint32_t tsa, tsd;
   uint64_t val;
   asm volatile ("rdtsc" : "=a" (tsa), "=d" (tsd));
   val = ((uint64_t) tsa) | (((uint64_t) tsd) << 32);
   hwc.time = val - old.time;
   old.time = val;

   // for each hw counter of this cpu
   for (unsigned int i = 0; i < NB_HW_COUNTERS; i++)
   {
      uint64_t buf [3];
      uint64_t value;

      res = ::read (fd [i], buf, sizeof (buf));
      if (res != sizeof (buf))
      {
         LOG (ERROR) << "Failed to read counter #" << i << std::endl;
         return false;
      }

      // handle scaling to allow PMU sharing
      value = (uint64_t)((double) buf [0] * (double) buf [1] / buf [2]);
      if (old.values [i] <= value)
      {
         hwc.values [i] = value - old.values [i];
      }
      else  // overflow
      {
         hwc.values [i] = 0xFFFFFFFFFFFFFFFFUL - old.values [i] + value;
      }

      // remember this value
      old.values [i] = value;
   }

   return true;
}

} // namespace FoREST

