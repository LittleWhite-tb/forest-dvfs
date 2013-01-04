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

#ifdef REST_LOG
#include <iostream>
#include <fstream>
#include <time.h>
#endif

#include "Common.h"
#include "DVFSUnit.h"
#include "Profiler.h"

/**
 * Decision taken by the decision maker.
 */
typedef struct
{
   unsigned int freqId;    // new frequency to use
   unsigned int cpuId;			// the current cpu to apply the decision to
   unsigned int sleepWin;  // new sleep window to use (usec)
   float timeRatio; //the portion of the totalSleepWin ossiciated to the selected freq, sleepWin = timeRatio * totalSleepWin
   unsigned int freqApplyDelay;   // amount of time to wait before reseting the counters (in usec)
   bool skip;
} Decision;

#define DECISION_DEFAULT_INITIALIZER {0,0,500,0,0,false}
#define DECISION_ZERO_INITIALIZER {0,0,0,0,0,true}

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
      DecisionMaker (const DVFSUnit& dvfsUnit);

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
      virtual Decision takeDecision () = 0;

			/**
			 * Read the HW counters of all the cores in the Decision Maker's DVFS Unit
			 */
			virtual void readCounters () = 0;

			// TODO Comment
			virtual void setProfiler (Profiler *prof) = 0;

   protected:
      /**
       * Which DVFS unit we are handling.
       */
      const DVFSUnit& unit;
};
#endif
