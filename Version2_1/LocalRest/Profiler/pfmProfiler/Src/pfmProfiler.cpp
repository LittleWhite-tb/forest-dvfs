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
 * @file PfmProfiler.cpp
 * The PfmProfiler class is in this file
 */

#include <unistd.h>
#include <iostream>
#include <sys/ioctl.h>
#include <stdint.h>
#include <string.h>

#include "pfmProfiler.h"
#include "perfmon/perf_event.h"
#include "perfmon/pfmlib_perf_event.h"

// definition of the static fields
pthread_mutex_t PfmProfiler::pfmInitMtx = PTHREAD_MUTEX_INITIALIZER;
unsigned int PfmProfiler::nbPfmInit = 0;

PfmProfiler::PfmProfiler (DVFSUnit & unit) : Profiler(unit)
{
#ifdef ARCH_SNB
   const char * counters [] = {
      "LAST_LEVEL_CACHE_REFERENCES",
      "UNHALTED_CORE_CYCLES",
      "CPU_CLK_UNHALTED:REF_P"
   };
#else
   const char * counters [] = {
      "L2_RQSTS:MISS",
      "UNHALTED_CORE_CYCLES",
      "CPU_CLK_UNHALTED:REF_P"
   };
#endif
   int res;

   // libpfm init
   PfmProfiler::pfmInit();

   // initialize counters
   for (unsigned int i = 0; i < NB_HW_COUNTERS; i++)
   {
      pfm_perf_encode_arg_t arg;

      // initialize the structure
      arg.attr = new perf_event_attr ();
      arg.fstr = NULL;
      arg.size = sizeof (arg);

      // encode the counter
      res = pfm_get_os_event_encoding (counters [i], PFM_PLM0 | PFM_PLM3, PFM_OS_PERF_EVENT_EXT, &arg);
      if (res != PFM_SUCCESS)
      {
         std::cerr << "Failed to get counter " << counters [i] << std::endl;
      }

      // request scaling in case of shared counters
      arg.attr->read_format = PERF_FORMAT_TOTAL_TIME_ENABLED | PERF_FORMAT_TOTAL_TIME_RUNNING;

      // open the corresponding file on the system
      this->pfmFds [i] = perf_event_open (arg.attr, -1, this->unit.getOSId(), -1, 0);
      if (this->pfmFds [i] == -1)
      {
         std::cerr << "Cannot open counter " << counters [i] << " on cpu " << this->unit.getOSId() << std::endl;
      }

      // Activate the counter
      if (ioctl (this->pfmFds [i], PERF_EVENT_IOC_ENABLE, 0))
      {
         std::cerr << "Cannot enable event " << counters [i] << " on cpu " << this->unit.getOSId() << std::endl;
      }

      // initially, old values are 0
      memset(&this->formerMeasurement, 0, sizeof(this->formerMeasurement));

      // free some mem
      delete arg.attr;
   }
}

PfmProfiler::~PfmProfiler ()
{
   // free memory
   for (unsigned int i = 0; i < NB_HW_COUNTERS; i++)
   {
      close (this->pfmFds [i]);
   }

   // close pfm
   PfmProfiler::pfmTerminate();
}

void PfmProfiler::read (HWCounters & hwc)
{
   int res;

   for (unsigned int i = 0; i < NB_HW_COUNTERS; i++)
   {
      uint64_t buf [3];
      uint64_t value;

      res = ::read (this->pfmFds [i], buf, sizeof (buf));
      if (res != sizeof (buf))
      {
         std::cerr << "Failed to read counter #" << i << std::endl;
      }

      // handle scaling to allow PMU sharing
      value = (uint64_t)( (double) buf [0] * buf [1] / buf [2]);
      if (this->formerMeasurement.values [i] <= value)
      {
         hwc.values [i] = value - this->formerMeasurement.values [i];
      }
      else  // overflow
      {
         hwc.values [i] = 0xFFFFFFFFFFFFFFFFUL - this->formerMeasurement.values [i] + value;
      }
      
      // remember this value
      this->formerMeasurement.values [i] = hwc.values[i];
   }
}

