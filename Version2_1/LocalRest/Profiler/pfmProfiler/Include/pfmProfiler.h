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
  @file pfmProfiler.h
  @brief The PfmProfiler class header is here.
 */
#ifndef H_PFMPROFILER
#define H_PFMPROFILER

#include <iostream>
#include <pthread.h>
#include "perfmon/pfmlib.h"

#include "Profiler.h"

/**
 * @class PfmProfiler
 * Profiler implementation based on libpfm.
 */
class PfmProfiler : public Profiler
{

   public:
      /**
       * Constructor
       *
       * @param unit The DVFS unit we are profiling.
       */
      PfmProfiler (DVFSUnit & unit);

      /**
       * Destructor
       */
      virtual ~PfmProfiler ();

      /**
       * Reads the counter values and "resets" them.
       *
       * @param hwc The hardware counter structure to fill with the results.
       */
      void read (HWCounters & hwc);

   private:

      /**
       * Pfm initialization routine.
       */
      static void pfmInit ()
      {
         int res;

         pthread_mutex_lock(&PfmProfiler::pfmInitMtx);

         if (PfmProfiler::nbPfmInit == 0)
         {
            if ((res = pfm_initialize ()) != PFM_SUCCESS)
            {
               std::cerr << "Failed to initialize libpfm: " << pfm_strerror (res) << std::endl;
            }
         }

         PfmProfiler::nbPfmInit++;

         pthread_mutex_unlock(&PfmProfiler::pfmInitMtx);
      }

      /**
       * Pfm closing routine.
       */
      static void pfmTerminate ()
      {
         pthread_mutex_lock(&PfmProfiler::pfmInitMtx);

         if (PfmProfiler::nbPfmInit-- == 0)
         {
            pfm_terminate ();
         }

         pthread_mutex_unlock(&PfmProfiler::pfmInitMtx);
      }

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
      int pfmFds [NB_HW_COUNTERS];

      /**
       * Former result to allow difference computations.
       */
      HWCounters formerMeasurement;
};

#endif
