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

#include "Profiler.h"

/**
 * @class PfmProfiler
 * @brief Profiler implementation based on libpfm.
 */
class PfmProfiler : Profiler
{

   public:
      /**
       * @brief Constructor
       */
      PfmProfiler (void);

      /**
       * @brief Destructor
       */
      virtual ~PfmProfiler (void);

      /**
       * @brief Reads the counter values and "resets" them. 
       * @param coreId The involved processor core's number.
       * @param values Where to write the differences of counter values compared
       * to the last measurement.
       */
      void read (unsigned int coreId, unsigned long long * values);

   private:

      unsigned int nbCores;   /**< @brief Number of cpus */

      /**
       * @brief List of fds for counters per cpu. Access with 
       * pfmFds[cpu_num][counter_num]
       */
      int **pfmFds;

      /**
       * @brief List of number of fds per cpu. Related to pfmFds.
       */
      unsigned int nbFds;

      /**
       * @brief Former measurement to allow difference computations.
       * One per counter for each cpu.
       */
      uint64_t **oldValues;
};

#endif
