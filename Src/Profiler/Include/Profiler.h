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

#include "perfmon/pfmlib.h"
#include "glog/logging.h"
#include "Counter.h"
#include "rdtsc.h"

namespace FoREST {

/**
 * @class Profiler
 * Profiler implementation based on libpfm.
 */
class Profiler
{

   public:
      /**
       * Constructor
       */
      Profiler ();

      /**
       * Destructor
       */
      virtual ~Profiler ();

      /**
       * Reads the counter values
       *
       * @param counter the counter to read. The counter is assumed as opened.
       * This means the open method has to be called before the read method
       * @param frequencyId the frequencyId to know where to store the results
       */
      bool read (Counter& counter, unsigned int frequencyId = 0); 

      /**
       * Opens the HW counter file descriptors for the given thread id
       *
       * @param counter the counter to open
       * @param threadId the id of the thread to monitor
       */
      void open (Counter& counter, unsigned int threadId);

      /**
       * Closes the HW counter file descriptor
       *
       * @param counter the counter to close. The counter is assumed
       * as opened when this method is called
       */
      void close (Counter& counter);

      /**
       * Reads the timestamp counter value
       *
       * @time the values to update
       */
      static inline void readTsc (CounterValues& time) {
         uint64_t val = rdtsc ();
         time.current = val - time.old;
         time.old = val;
      }

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

      /**
       * Mutex to access the pfm initialization variables.
       */
      static pthread_mutex_t pfmInitMtx;

      /**
       * How many threads have initialized the library ?
       */
      static unsigned int nbPfmInit;
};

}

#endif
