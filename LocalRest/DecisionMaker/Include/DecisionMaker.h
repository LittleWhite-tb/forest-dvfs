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
   unsigned int sleepWin;  // new sleep window to use (usec)
   unsigned int freqApplyDelay;   // amount of time to wait before reseting the counters (in usec)
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
       * Genenerates a new decision maker in charge of the given
       * dvfs unit and targeting the given mode.
       *
       * @param dvfsUnit The DVFS unit this decision maker is in charge of.
       * @param mode Run mode (energy or performance).
       */
      DecisionMaker (const DVFSUnit& dvfsUnit, const Mode runMode);

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

      /**
       * Sets the hardware counter profiler to prof.
       *
       * @param prof The new profiler to use.
       */
      virtual void setProfiler (Profiler *prof)
      {
         this->prof = prof;
      }

   protected:

      /**
       * Which DVFS unit we are handling.
       */
      const DVFSUnit& unit;

      /**
       * What mode are we currently achieving.
       */
      const Mode mode;

      /**
       * Profiler used to read  
       */
      Profiler *prof;
};
#endif
