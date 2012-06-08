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
 * The DeltaAdaptiveDecisions class header is in this file
 */

#ifndef H_DELTAADAPTIVEDECISIONS
#define H_DELTAADAPTIVEDECISIONS

#include "DecisionMaker.h"
#include <iostream>
#include <stack>
#ifdef REST_EXTRA_LOG
#include <sstream>
#include <vector>
#include "Logger.h"
#endif

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
#ifdef REST_EXTRA_LOG
	std::stringstream ss (std::stringstream::out);
	ss << "IPC["<< this->unit.getFrequency() <<"]=" << (hwc.retired / (1.0 * hwc.cycles)) << " | retired = "<< hwc.retired << " | cycles = "<< hwc.cycles ;	
	ss << " | MIPS["<< this->unit.getFrequency() <<"]=" << (( (hwc.retired / (1.0 * hwc.cycles)) * this->unit.getFrequency (this->unit.getFrequency ())) /1000 );
	Logger & log = Logger::getLog(this->unit.getOSId ());	
	log.logOut(ss);
#endif

         return (( (hwc.retired / (1.0 * hwc.cycles)) * this->unit.getFrequency (this->unit.getFrequency ())) /1000);
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
	  void freqSelStrategy1 (int *adjacentLowFreqId,int *adjacentHighFreqId, unsigned int virtualFreq);
	  
	  /**
	   * Use the adjacent frequnecies to emulate it
	   * @param res A decision to update in order to start the new execution
       * sequence.
       * */
	  void freqSelStrategy2 (int *adjacentLowFreqId,int *adjacentHighFreqId, unsigned int virtualFreq);
	  
	  /**
	   * Use the maxIPC and the low adjacent frequency to emulate it
	   * @param res A decision to update in order to start the new execution
       * sequence.
       * */
	  void freqSelStrategy3 (int *adjacentLowFreqId,int *adjacentHighFreqId, unsigned int virtualFreq);
	  
	  /**
	   * Use the minIPC and the high adjacent frequency to emulate it
	   * @param res A decision to update in order to start the new execution
       * sequence.
       * */
	  void freqSelStrategy4 (int *adjacentLowFreqId,int *adjacentHighFreqId, unsigned int virtualFreq);


      /**
       * State in which the decision maker currently is.
       */
      BetaState curState;

      /**
      * MIPS evaluation result.
      */
      float * mipsEval;

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
