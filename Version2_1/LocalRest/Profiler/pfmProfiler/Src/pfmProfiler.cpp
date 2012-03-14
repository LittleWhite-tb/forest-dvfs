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
  @file PfmProfiler.cpp
  @brief The PfmProfiler class is in this file
 */

#include <unistd.h>
#include <iostream>
#include <sys/ioctl.h>
#include <stdint.h>

#include "pfmProfiler.h"
#include "perfmon/pfmlib.h"
#include "perfmon/perf_event.h"
#include "perfmon/pfmlib_perf_event.h"


PfmProfiler::PfmProfiler (void)
{
   int res;
#ifdef ARCH_SNB
   const char * counters [] =
   { "OFFCORE_REQUESTS_BUFFER:SQ_FULL", "UNHALTED_REFERENCE_CYCLES", "LAST_LEVEL_CACHE_REFERENCES", "INSTRUCTION_RETIRED" };
#else
   const char * counters [] =
   { "SQ_FULL_STALL_CYCLES", "UNHALTED_REFERENCE_CYCLES", "L2_RQSTS:MISS", "INSTRUCTION_RETIRED" };
#endif
   const unsigned int nbCounters = sizeof (counters) / sizeof (*counters);

   // libpfm init
   if ((res = pfm_initialize ()) != PFM_SUCCESS)
   {
      std::cerr << "Failed to initialize libpfm: " << pfm_strerror (res) << std::endl;
   }

   // fetch number of processors and initialize a few things
   this->nbCores = sysconf (_SC_NPROCESSORS_ONLN);
   this->nbFds = nbCounters;

   this->pfmFds = new int *[this->nbCores];
   this->oldValues = new uint64_t *[this->nbCores];
   for (unsigned int i = 0; i < this->nbCores; i++)
   {
      this->pfmFds [i] = new int [nbCounters];
      this->oldValues [i] = new uint64_t [nbCounters];
   }


   // initialize counters
   for (unsigned int i = 0; i < nbCounters; i++)
   {
      pfm_perf_encode_arg_t arg;

      // initialize the structure
      arg.attr = new perf_event_attr ();
      arg.fstr = NULL;
      arg.size = sizeof (arg);

      // encode the counter
      res = pfm_get_os_event_encoding (counters [i], PFM_PLM0 | PFM_PLM3,
      PFM_OS_PERF_EVENT_EXT, &arg);
      if (res != PFM_SUCCESS)
      {
         std::cerr << "Failed to get counter " << counters [i] << std::endl;
      }

      // request scaling in case of shared counters
      arg.attr->read_format = PERF_FORMAT_TOTAL_TIME_ENABLED
      | PERF_FORMAT_TOTAL_TIME_RUNNING;

      // convert that into a fd for every cpu
      for (unsigned int cpu = 0; cpu < this->nbCores; cpu++)
      {
         // open the file descriptor
         this->pfmFds [cpu][i] = perf_event_open (arg.attr, -1, cpu, -1, 0);
         if (this->pfmFds [cpu][i] == -1)
         {
            std::cerr << "Cannot open counter " << counters [i] << " on cpu " << cpu << std::endl;
         }

         // Activate the counter
         if (ioctl (this->pfmFds [cpu][i], PERF_EVENT_IOC_ENABLE, 0))
         {
            std::cerr << "Cannot enable event " << counters [i] << " on cpu " << cpu << std::endl;
         }

         // initially, old values are 0
         this->oldValues [cpu][i] = 0;
      }

      // free some mem
      delete arg.attr;
   }
}

PfmProfiler::~PfmProfiler (void)
{
   // free memory
   for (unsigned int cpu = 0; cpu < this->nbCores; cpu++)
   {
      for (unsigned int i = 0; i < this->nbFds; i++)
      {
         close (this->pfmFds [cpu][i]);
      }

      delete [] this->pfmFds [cpu];
      delete [] this->oldValues [cpu];
   }

   delete [] this->oldValues;
   delete [] this->pfmFds;

   // close pfm
   pfm_terminate ();
}

void PfmProfiler::read (unsigned int coreId, unsigned long long * values)
{
   int res;

   for (unsigned int i = 0; i < this->nbFds; i++)
   {
      uint64_t buf [3];
      uint64_t value;

      res = ::read (this->pfmFds [coreId][i], buf, sizeof (buf));
      if (res != sizeof (buf))
      {
         std::cerr << "Failed to read counter #" << i << " on cpu " << coreId << std::endl;
      }

      // handle scaling to allow PMU sharing
      value = (uint64_t)( (double) buf [0] * buf [1] / buf [2]);
      if (oldValues [coreId][i] <= value)
      {
         values [i] = value - oldValues [coreId][i];
      }
      else  // overflow
      {
         values [i] = 0xFFFFFFFFFFFFFFFFUL - oldValues [coreId][i] + value;
      }
      oldValues [coreId][i] = value;
   }
}

