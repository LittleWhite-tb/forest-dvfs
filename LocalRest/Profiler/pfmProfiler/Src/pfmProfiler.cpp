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
 * @file PfmProfiler.cpp
 * The PfmProfiler class is in this file
 */

#include <cstdlib>
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

PfmProfiler::PfmProfiler (DVFSUnit & unit) : Profiler (unit)
{
   const char * counters [] =
   {
      "INST_RETIRED:ANY_P",
#ifdef ARCH_SNB
      "UNHALTED_REFERENCE_CYCLES"
#elif ARCH_IVB
      "UNHALTED_REFERENCE_CYCLES"
//      "CPU_CLK_UNHALTED:REF_P"
//      "UNHALTED_CORE_CYCLES"
#else
#warning "Unknown micro-architecture! CPU cycle counting may be wrong."
      "UNHALTED_REFERENCE_CYCLES"
#endif
   };

   int res;

   // libpfm init
   PfmProfiler::pfmInit (); 
   this->nbCpuIds = this->unit.getNbCpuIds ();

   //   std::cerr << "nbCpuIds = " << this->nbCpuIds << std::endl;

   this->pfmFds = new int [NB_HW_COUNTERS*nbCpuIds];
   handleAllocation (this->pfmFds);

   this->formerMeasurement = new HWCounters [nbCpuIds];
   handleAllocation (this->formerMeasurement);

   // For each CPU in the DVFSUnit
   for (unsigned int cpu = 0; cpu < nbCpuIds; cpu++) {
      unsigned int cpuId = this->unit.getOSId (cpu);

      // Offset in the pfmFds array because flattened 2D array
      unsigned baseIdx = cpu*NB_HW_COUNTERS;

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
         res = pfm_get_os_event_encoding (counters [i],  PFM_PLM0 | PFM_PLM1 | PFM_PLM2 | PFM_PLM3, PFM_OS_PERF_EVENT_EXT, &arg);
         if (res != PFM_SUCCESS)
         {
            std::cerr << "Failed to get counter " << counters [i] << " on cpu " << cpu << std::endl;
            exit (EXIT_FAILURE);
         }

         // request scaling in case of shared counters
         arg.attr->read_format = PERF_FORMAT_TOTAL_TIME_ENABLED | PERF_FORMAT_TOTAL_TIME_RUNNING;

         // open the corresponding file on the system
         this->pfmFds [baseIdx + i] = perf_event_open (arg.attr, -1, cpuId, -1, 0);
         if (this->pfmFds [baseIdx + i] == -1)
         {
            std::cerr << "Cannot open counter " << counters [i] << " on cpu " << cpuId << std::endl;
            exit (EXIT_FAILURE);
         }

         // Activate the counter
         if (ioctl (this->pfmFds [baseIdx + i], PERF_EVENT_IOC_ENABLE, 0))
         {
            std::cerr << "Cannot enable event " << counters [i] << " on cpu " << cpuId << std::endl;
            exit (EXIT_FAILURE);
         }
      }
   }

   // initially, old values are 0
   memset (this->formerMeasurement, 0, this->nbCpuIds * sizeof (*this->formerMeasurement)); 
}

PfmProfiler::~PfmProfiler ()
{
   // free memory
   for (unsigned int i = 0; i < nbCpuIds*NB_HW_COUNTERS; i++)
   {
      close (this->pfmFds [i]);
   }

   delete [] pfmFds, pfmFds = 0;
   delete [] formerMeasurement, formerMeasurement = 0;

   // close pfm
   PfmProfiler::pfmTerminate ();
}

void PfmProfiler::read (HWCounters & hwc, unsigned int cpu)
{
   assert (cpu < this->nbCpuIds);
   int res;

   //std::cerr << "reading hwc for cpu " << cpu << std::endl;
   
   // specific case of the time stamp counter
   uint32_t tsa, tsd;
   uint64_t val;
   asm volatile ("rdtsc" : "=a" (tsa), "=d" (tsd));
   val = ((uint64_t) tsa) | (((uint64_t) tsd) << 32);
   hwc.time = val - this->formerMeasurement [cpu].time;
   this->formerMeasurement [cpu].time = val;

   // Base index of the file descriptors array (because it's a flattened 2d array)
   unsigned baseIdx = cpu*NB_HW_COUNTERS;

   // for each hw counter of this cpu
   for (unsigned int i = 0; i < NB_HW_COUNTERS; i++)
   {
      uint64_t buf [3];
      uint64_t value;

      res = ::read (this->pfmFds [baseIdx + i], buf, sizeof (buf));
      if (res != sizeof (buf))
      {
         std::cerr << "Failed to read counter #" << i << std::endl;
      }

      // handle scaling to allow PMU sharing
      value = (uint64_t)((double) buf [0] * (double) buf [1] / buf [2]);
      //if (i == 1) std::cout << "value: " << buf [0] << " former: " << this->formerMeasurement [cpu].values [i] <<  std::endl;
      if (this->formerMeasurement [cpu].values [i] <= value)
      {
         hwc.values [i] = value - this->formerMeasurement [cpu].values [i];
      }
      else  // overflow
      {
         hwc.values [i] = 0xFFFFFFFFFFFFFFFFUL - this->formerMeasurement [cpu].values [i] + value;
      }

      // remember this value
      this->formerMeasurement [cpu].values [i] = value;
   }
}

