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
#include <stdint.h>
#include <string.h>

#include "Profiler.h"
#include "perfmon/perf_event.h"
#include "perfmon/pfmlib_perf_event.h"
#include "glog/logging.h"
#include "Counter.h"

namespace FoREST {

// definition of the static fields
pthread_mutex_t Profiler::pfmInitMtx = PTHREAD_MUTEX_INITIALIZER;
unsigned int Profiler::nbPfmInit = 0;

Profiler::Profiler () {
   // libpfm init
   Profiler::pfmInit (); 
}

void Profiler::open (Counter& counter, unsigned int threadId) {
   struct perf_event_attr attr;
   pfm_perf_encode_arg_t arg;
   int res;

   // initialize the structure
   memset (&attr, 0, sizeof (attr));
   memset (&arg, 0, sizeof (arg));
   arg.attr = &attr;
   arg.fstr = 0;
   arg.size = sizeof (arg);
   // encode the counter
   res = pfm_get_os_event_encoding (counter.name,  PFM_PLM0 | PFM_PLM1 |
                                                   PFM_PLM2 | PFM_PLM3,
                                    PFM_OS_PERF_EVENT_EXT, &arg);
   
   /* Check for errors when calling libpfm */
   if (res != PFM_SUCCESS) {
      std::cerr << "Error: ";
      switch (res) {
      case PFM_ERR_TOOSMALL:
         std::cerr << "The code argument is too small for the encoding." << std::endl;
         break;
      case PFM_ERR_INVAL:
         std::cerr << "The code or count argument is NULL." << std::endl;
         break;
      case PFM_ERR_NOMEM:
         std::cerr << "Not enough memory." << std::endl;
         break; 
      case PFM_ERR_NOTFOUND:
         std::cerr << "Event not found." << std::endl;
         break;
      case PFM_ERR_ATTR:
         std::cerr << "Invalid event attribute (unit mask or modifier)" << std::endl;
         break; 
      case PFM_ERR_ATTR_VAL:
         std::cerr << "Invalid modifier value." << std::endl;
         break; 
      case PFM_ERR_ATTR_SET:
         std::cerr << "Attribute already set, cannot be changed." << std::endl;
         break;
      }
   } 
   
   CHECK (res == PFM_SUCCESS) << "Failed to get counter " << counter.name
      << " on cpu " << threadId << std::endl;

   // request scaling in case of shared counters
   arg.attr->read_format = PERF_FORMAT_TOTAL_TIME_ENABLED | PERF_FORMAT_TOTAL_TIME_RUNNING;
   
   /**
    * libpfm Xeon Phi Hack:
    * set the exclude_guest flag to 0 in order to allow perf to open counters
   */
   arg.attr->exclude_guest = 0;

   // open the corresponding file on the system
   counter.fd = perf_event_open (arg.attr, -1, threadId, -1, 0);
   if (counter.fd  == -1) {
      perror ("Error while opening counter");
   }
   CHECK (counter.fd != -1) << "Cannot open counter "
      << counter.name << " on cpu " << threadId << std::endl;

   // Activate the counter
   CHECK (ioctl (counter.fd, PERF_EVENT_IOC_ENABLE, 0) == 0)
      << "Cannot enable event " << counter.name << " on cpu " << threadId
      << std::endl;
}

void Profiler::close (Counter& counter) {
   ::close (counter.fd);
}

Profiler::~Profiler ()
{
   // close pfm
   Profiler::pfmTerminate ();
}

bool Profiler::read (Counter& counter, unsigned int frequencyId)
{
   int res;

   //DLOG (INFO) << "reading hwc for cpu " << cpu << std::endl;
   struct read_format {
      uint64_t nr;
      uint64_t time_enabled;
      uint64_t time_running;
   } fmt;
   uint64_t value;

   res = ::read (counter.fd, &fmt, sizeof (fmt));
   if (res != sizeof (fmt))
   {
      LOG (ERROR) << "Failed to read counter " << counter.name << std::endl;
      return false;
   }

   // handle scaling to allow PMU sharing
   value = (uint64_t)((double) fmt.nr * (double) fmt.time_enabled / fmt.time_running);
   
   counter.setValue (frequencyId, value);

   return true;
}

} // namespace FoREST

