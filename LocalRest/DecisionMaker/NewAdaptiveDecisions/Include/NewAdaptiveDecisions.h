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

#ifndef H_NEWADAPTIVEDECISIONS
#define H_NEWADAPTIVEDECISIONS

#include "DecisionMaker.h"

#ifdef REST_EXTRA_LOG
#include <sstream>
#include <vector>
#include "Logger.h"
#endif


/**
 * @class NewAdaptiveDecisions
 *
 * The decision is taken adaptively here, trying to maximize the ressource usage
 * and exploiting the memory boundness ratio as a hint.
 */
class NewAdaptiveDecisions : public DecisionMaker
{
   public:

      /**
       * Constructor
       */
      NewAdaptiveDecisions (DVFSUnit & dvfsUnit);

      /**
       * Destructor
       */
      ~NewAdaptiveDecisions ();

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
         START,
	 EVAL_STP,
         GOTO_RUN_STP2,
	 RUN_STP2_DONE
      } State;

      /**
       * Debug flag.
       */
      static const bool VERBOSE = false;

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
      static const unsigned int MAX_SLEEP_WIN = 200000;

      /**
       * Number of frequencies we consider bellow and above the current
       * frequency during the evaluation of frequencies. 2 here means that
       * at most four frequencies will be evaluated: two just bellow the current
       * one and two just above.
       */
      static const unsigned int NB_EVAL_NEAR_FREQS = 2;

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
	uint64_t swRefCycles = hwc.cycles * ((double) this->unit.getFrequency (0) / this->unit.getFrequency (this->unit.getFrequency ()));

#ifdef REST_EXTRA_LOG
        std::stringstream ss (std::stringstream::out);
        ss <<  "hwc.cycles = " << hwc.cycles << " | hwc.retired = " << hwc.retired << " | IPC ["<< this->unit.getFrequency() <<"] = " <<  hwc.retired / (1. * swRefCycles);;
        Logger & log = Logger::getLog(this->unit.getOSId ());
        log.logOut(ss);
#endif
         
         return hwc.retired / (1. * swRefCycles);
      }

      /**
       * Computes the couples of FreqId and their associated sleepWindows to emulate a frequency associated to the 5% degraded IPC
       * @param	degradedIPC the 5% degraded IPC
       * @param minFreqId the Lowest bound of the frequency window
       * @param maxFreqId the Highesr bound of the frequency window
       * @param Step1 the first member of the couple
       * @param Step2 the second memeber of the couple
       */
      void getVirtualFreq(float degradedIPC,unsigned int minFreqId,unsigned int maxFreqId, Decision &Step1, Decision &Step2);

      /**
       * The center of the freq window
       */
      unsigned int freqWindowCenter;

      /**
       * The Total SleepWindow, sleepWinStep1+sleelWinStep2
       */
      unsigned int totalsleepWin; 
      
      /**
       * The state the decision maker currently is
       */
      State curState;
      /**
       * IPC evaluation result.
       */
      float * ipcEval;

      /**
       * The freqId associated to the seconf part of the decision to apply
       */
      unsigned int freqIdStep2;
	
      /**
       * The sleepWin associated to the second part of the decision to apply
       */
      unsigned int sleepWinStep2;

      /**
       * The last sleep window we have used.
       */
      unsigned int formerSleepWin;

      /**
       * Previously chosen frequency.
       */
      unsigned int formerFreqId;

      /**
       * The frequency we have used for the first step of the decision
       */
      unsigned int formerFreqIdStep1;

      /**
       * The sleep window we have used for the first step of the decision
       */
      unsigned int formerSleepWinStep1;

	/**
       * The frequency we have used for the second step of the decision
       */
      unsigned int formerFreqIdStep2;

      /**
       * The sleep window we have used for the second step of the decision
       */
      unsigned int formerSleepWinStep2;

};

#endif
