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
 * @file AdaptiveDecisions.h
 * The AdaptiveDecisions class header is in this file
 */

#ifndef H_ADAPTIVEDECISIONS
#define H_ADAPTIVEDECISIONS

#include "DecisionMaker.h"
#include "FreqSelector.h"

/**
 * @class AdaptiveDecisions
 *
 * The decision is taken adaptively here, trying to maximize the ressource usage
 * and exploiting the memory boundness ratio as a hint.
 */
class AdaptiveDecisions : public DecisionMaker
{
   public:

      /**
       * Constructor
       */
      AdaptiveDecisions (DVFSUnit & dvfsUnit);

      /**
       * Destructor
       */
      ~AdaptiveDecisions ();

      /**
       * Decides what to do considering the last measurements.
       *
       * @param hwc The hardware counters values measured
       *
       * @return A decision object where a new core frequency and sleeping
       * window is given.
       */
      Decision takeDecision (const HWCounters & hwc);

      /**
       * Gives an initialization decision which defines a default sleep window
       * and frequency to use.
       *
       * @return A default decision object.
       */
      Decision defaultDecision ();

   private:

      /**
       * Time required to evaluate the IPC for one frequency (us).
       */
      static const unsigned int IPC_EVAL_TIME = 20;

      /**
       * Minimal execution time once a frequency is chosen (us).
       */
      static const unsigned int MIN_SLEEP_WIN = 500;

      /**
       * Maximal execution time before re-evaluating which frequency to use
       * (us).
       */
      static const unsigned int MAX_SLEEP_WIN = 150000;

      /**
       * Computes the hardware exploitation ratio from the hardware counters.
       *
       * @param hwc The hardware counter values.
       *
       * @return A number which evaluates how much the cpu is used compared to
       * the memory. Zero means that the memory is not at all the bottleneck,
       * whereas large values means that the cpu is often paused and waits for
       * the memory.
       */
      inline float getHWExploitationRatio (const HWCounters & hwc) const
      {
         uint64_t swRefCycles = hwc.cycles * ((double) this->unit.getFrequency(0) / this->unit.getFrequency(this->unit.getFrequency()));

         return hwc.retired / (1. * swRefCycles);
      }

      /**
       * Helper to maintain the evaluation results.
       */
      FreqSelector * freqSel;

      /**
       * IPC evaluation result.
       */
      float * ipcEval;

      /**
       * The last sleep window we have used.
       */
      unsigned int formerSleepWin;

      /**
       * Previously chosen frequency.
       */
      unsigned int formerFreqId;

      /**
       * The frequency we have used for execution (excluding freq evaluation).
       */
      unsigned int formerExecFreqId;

      /**
       * The last sleep window we have used for execution.
       */
      unsigned int formerExecSleepWin;
};

#endif
