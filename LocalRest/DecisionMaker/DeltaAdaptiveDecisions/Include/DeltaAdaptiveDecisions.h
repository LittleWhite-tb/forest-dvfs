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
 * @file DeltaAdaptiveDecisions.h
 * The AdaptiveDecisions class header is in this file
 */

#ifndef H_DELTAADAPTIVEDECISIONS
#define H_DELTAADAPTIVEDECISIONS

#include "DecisionMaker.h"
#include "FreqSelector.h"
#include <iostream>
#include <stack>

/**
 * @class AdaptiveDecisions
 *
 * The decision is taken adaptively here, trying to maximize the ressource usage
 * and exploiting the memory boundness ratio as a hint.
 */
class DeltaAdaptiveDecisions : public DecisionMaker
{
   public:

      /**
       * Constructor
       */
      DeltaAdaptiveDecisions (DVFSUnit & dvfsUnit);

      /**
       * Destructor
       */
      ~DeltaAdaptiveDecisions ();

      /**
       * Decides what to do considering the last measurements.
       *
       * @param hwc The hardware counters values measured
       *
       * @return A decision object where a new core frequency and sleeping
       * window is given.
       */
      Decision takeDecision (const HWCounters & hwc);
      Decision takeDecision (const HWCounters & hwc, bool delayedStart);
      /**
       * Gives an initialization decision which defines a default sleep window
       * and frequency to use.
       *
       * @return A default decision object.
       */
      Decision defaultDecision ();

   private:

      typedef enum
      {
         WAITING_START,
         MIPS_EVALUATION,
         RUNNING_STP1,
         RUNNING_STP2
      } BetaState;

      /**
       * Debug flag.
       */
      static const bool VERBOSE = true;

      /**
      * The fixed time for evaluation.
      */
      static const unsigned int STATIC_EVAL_TIME = 1000000;

      /**
      * Computes the hardware exploitation ratio from the hardware counters.
      *
      * @param hwc The hardware counter values.
      *
      * @return The mips ratio
      */
      inline float getMipsRatio (const HWCounters & hwc) const
      {
         return (( (hwc.retired / (1.0 * hwc.cycles)) * this->unit.getFrequency (this->unit.getFrequency ())) /1000000);
      }
      
      /**
      * Computes the hardware exploitation ratio from the hardware counters.
      *
      * @param hwc The hardware counter values.
      *
      * @return The ipc ratio
      */
      inline float getHWExploitationRatio (const HWCounters & hwc) const
      {
         uint64_t swRefCycles = hwc.cycles * ((double) this->unit.getFrequency (0) / this->unit.getFrequency (this->unit.getFrequency ()));

         return hwc.retired / (1. * swRefCycles);
      }
      
      /**
       * Compute the beta coefficient, r ratio and setup the given decision
       * to start a new execution sequence.
       *
       * @param res A decision to update in order to start the new execution
       * sequence.
       */
      void resetBetaState(Decision &res);

	  /**
	   * Use the maxIPC and minIPC to emulate the virtual freq
	   * @param res A decision to update in order to start the new execution
       * sequence.
       * */
	  void decStrategy1 (Decision &res,int deltaDegradation);
	  
	  /**
	   * Use the Beta coefficient to compute the virtual freq and using adjacent frequnecy to emulate it
	   * @param res A decision to update in order to start the new execution
       * sequence.
       * */
	  void decStrategy2 (Decision &res,int deltaDegradation);
	  
	  /**
	   * Use the Beta coefficient to compute the virtual freq, and the maxIPC and the low adjacent frequency to emulate it
	   * @param res A decision to update in order to start the new execution
       * sequence.
       * */
	  void decStrategy3 (Decision &res, int deltaDegradation);
	  
	  /**
	   * Use the Beta coefficient to compute the virtual freq, and the minIPC and the high adjacent frequency to emulate it
	   * @param res A decision to update in order to start the new execution
       * sequence.
       * */
	  void decStrategy4 (Decision &res);
      /**
       * State in which the decision maker currently is.
       */
      BetaState curState;

      /**
      * MIPS evaluation result.
      */
      float * mipsEval;

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
       * Frequency selected for the second step of execution.
       */
      unsigned int freqIdStp2;

      /**
       * Selected sleep window for execution step 2.
       */
      unsigned int sleepWinStp2;
};

#endif
