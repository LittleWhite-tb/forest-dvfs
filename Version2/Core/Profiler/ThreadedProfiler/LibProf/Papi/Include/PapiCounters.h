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
  @file PapiCounters.h
  @brief The PapiCounters class header is in this file
 */
#ifndef H_PAPICOUNTERS
#define H_PAPICOUNTERS

#include <papi.h>
#include <sys/types.h>
#include <unistd.h>

#include "LibProf.h"

/**
 * @class PapiCounters
 * @brief The PapiCounters for hardware counters
 */
class PapiCounters: public LibProf
{

   public:
      /**
       * @brief Constructor
       */
      PapiCounters (void);

      /**
       * @brief Destructor
       */
      virtual ~PapiCounters (void);

      /**
       * @brief Tells the profiler to watch counter for the given thread.
       * @param tid The TID of the concerned thread.
       */
      void attach_to(unsigned long int tid);

      /**
       * @brief Starts watching the counters.
       */
      void start_counters();

      /**
       * @brief Stop watching the counters.
       */
      void stop_counters();

      /**
       * @brief Reads the counter values and reset them.
       * @param values Where to write the couter values.
       */
      void read(long long *values);

    private:

      /**
       * @brief EventSet for papi.
       */
      int ev_set;

};

#endif
