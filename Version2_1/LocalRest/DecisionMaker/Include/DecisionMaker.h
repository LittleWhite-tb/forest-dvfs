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
 * @file DecisionMaker.h
 * The DecisionMaker class header is in this file
 */

#ifndef H_DECISIONMAKER
#define H_DECISIONMAKER

#ifdef REST_EXTRA_LOG
#include <iostream>
#include <fstream>
#include <time.h>
#endif

#include "Common.h"
#include "DVFSUnit.h"

/**
 * Decision taken by the decision maker.
 */
typedef struct
{
   unsigned int freqId;    // new frequency to use
   unsigned int sleepWin;  // new sleep window to use (usec)
} Decision;

/**
 * @class DecisionMaker
 * Generic decision maker to decide the next frequency and sleep window to use.
 * There is one decision maker instancer per DVFS unit.
 */
class DecisionMaker
{
   public:

      /**
       * Constructor
       */
      DecisionMaker (DVFSUnit & dvfsUnit);

      /**
       * Destructor
       */
      virtual ~DecisionMaker ();

      /**
       * Decides what to do considering the last measurements.
       *
       * @param hwc The hardware counters values measured
       *
       * @return A decision object where a new core frequency and sleeping
       * window is given.
       */
      virtual Decision takeDecision (const HWCounters & hwc) = 0;

      /**
       * Gives an initialization decision which defines a default sleep window
       * and frequency to use.
       *
       * @return A default decision object.
       */
      virtual Decision defaultDecision () = 0;

#ifdef REST_EXTRA_LOG
      /**
       * Print a marker in the log file.
       */
      inline void logMarker ()
      {
         struct timespec ts;
         clock_gettime (CLOCK_MONOTONIC, &ts);
         this->switchOFS << ts.tv_nsec + ts.tv_sec * 1000000000 << " #" << std::endl;
      }
#endif

   protected:

      /**
       * Which DVFS unit we are handling.
       */
      DVFSUnit & unit;

#ifdef REST_EXTRA_LOG
      /**
       * File where to output the frequency switches.
       */
      std::ofstream switchOFS;
#endif

};
#endif
