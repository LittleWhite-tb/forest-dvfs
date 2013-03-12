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
  @file Profiler.h
  @brief The Profiler class header is here.
 */
#ifndef H_PFMPROFILER
#define H_PFMPROFILER

#include <iostream>
#include <pthread.h>

#include "HWCounters.h"
#include "perfmon/pfmlib.h"
#include "glog/logging.h"

namespace FoREST {
class DVFSUnit;
/**
 * @class Profiler
 * Profiler implementation based on libpfm.
 */
class Profiler
{

   public:
      /**
       * Constructor
       *
       * @param unit The DVFS unit we are profiling.
       */
      Profiler (DVFSUnit & unit);

      /**
       * Destructor
       */
      virtual ~Profiler ();

      /**
       * Reads the counter values and "resets" them.
       *
       * @param hwc The hardware counter structure to fill with the results.
       * @param cpu The id of the cpu in the internal profiler table
       */
      bool read (int fd [NB_HW_COUNTERS], HWCounters & hwc, HWCounters& cpu);

      /**
       * Opens the HW counters file descriptors for the given thread id
       *
       * @param given thread id
       * @param fd the array of file descriptors (must be NB_HW_COUNTERS wide)
       */
      void open (unsigned int threadId, int fd [NB_HW_COUNTERS]);

   private:

      /**
       * Pfm initialization routine.
       */
      static void pfmInit ()
      {
         int res;

         pthread_mutex_lock (&Profiler::pfmInitMtx);

         if (Profiler::nbPfmInit == 0)
         {
            if ((res = pfm_initialize ()) != PFM_SUCCESS)
            {
               LOG (FATAL) << "Failed to initialize libpfm: " << pfm_strerror (res) << std::endl;
            }
         }

         Profiler::nbPfmInit++;

         pthread_mutex_unlock (&Profiler::pfmInitMtx);
      }

      /**
       * Pfm closing routine.
       */
      static void pfmTerminate ()
      {
         pthread_mutex_lock (&Profiler::pfmInitMtx);

         if (Profiler::nbPfmInit-- == 0)
         {
            pfm_terminate ();
         }

         pthread_mutex_unlock (&Profiler::pfmInitMtx);
      }

      inline unsigned int getNbCpuIds () const{
          return this->nbCpuIds;
      }

      /**
       * Number of cpu handled by the Profiler
       */
      unsigned int nbCpuIds;

      /**
       * Mutex to access the pfm initialization variables.
       */
      static pthread_mutex_t pfmInitMtx;

      /**
       * How many threads have initialized the library?
       */
      static unsigned int nbPfmInit;

      /**
       * List of fds opened to read the HW counters.
       */
      int *pfmFds;

      /**
       * Former result to allow difference computations.
       */
      HWCounters *formerMeasurement;

      DVFSUnit& unit;
};

}

#endif
