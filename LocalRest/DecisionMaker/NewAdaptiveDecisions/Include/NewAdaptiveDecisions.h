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
 * @file NewAdaptiveDecisions.h
 * The AdaptiveDecisions class header is in this file
 */

#ifndef H_NEWADAPTIVEDECISIONS
#define H_NEWADAPTIVEDECISIONS

#include <set>
#include <sstream>
#include <time.h>

#include "DecisionMaker.h"
#include "Profiler.h"
#include "TimeProfiler.h"
#include "FreqSelector.h"

#ifdef REST_LOG
#include <vector>
#include "Logger.h"
#endif


// Frequency sequence data structure
enum { STEP1 = 0, STEP2 };

/**
 * A frequency and an associated weight. The time ratio is a relative execution
 * time required for the frequency.
 */
struct FreqChunk {
   unsigned int freqId;
   float timeRatio;
};

/**
 * A couple of freq chunk.
 */
struct FreqChunkCouple
{
   FreqChunk step [2];
};


/**
 * A couple of decisions
 */
struct DecisionCouple
{
   Decision step [2];
};

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
      NewAdaptiveDecisions (const DVFSUnit& dvfsUnit);
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
      Decision takeDecision ();



   private:

      /**
       * Debug flag.
       */
#ifdef NDEBUG
      static const bool VERBOSE = false;
#else
      static const bool VERBOSE = true;
#endif

      /**
       * Time required to evaluate the IPC for one frequency (us).
       */
      static const unsigned int IPC_EVAL_TIME = 75;

      /**
       * Minimal execution time once a frequency is chosen (us).
       */
      static const unsigned int MIN_SLEEP_WIN = 1000;

      /**
       * Number of frequencies to consider when computing the stability of the
       * successive choices. If this value is 1 for instance, the new frequency
       * applied can be the most representative frequency among those applied
       * during the last sequence, the one above, or the one below while we
       * still consider the situation as stable, i.e. the sleep window is 
       * increased.
       */
      static const unsigned int MAX_FREQ_WINDOW = 1;

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
       * Number representing the minimum time ratio that can be added in the sequence
       * If the time ratio is less than this number, then the frequency chunk is not added to the sequence
       * and the time ratio is distributed to the last element in the sequence
       */
      static const float MIN_TIME_RATIO = 0.10; 

      /**
       * Maximal performance degradation allowed by the user (in % of the
       * max performance).
       */
      static const float USER_PERF_REQ = 0.95;

      /**
       * Generic method called for the evaluation step
       * Does several steps:
       * - Evaluation Init (see initEvaluation)
       * - Frequency Evaluation (see evaluateFrequency)
       * - 2 Steps computation (see computeSteps)
       * - Sequence computation (see computeSequence)
       * 
       * @return The decision of the evaluation
       */
      Decision evaluate ();

      /**
       * Initializes the evaluation, does several steps : 
       * - Compute the center of the frequency window (see computeFreqWindowCenter)
       * - Compute the frequency Window (see computeFreqWindow)
       * - Reset some values
       */
      Decision initEvaluation ();

      /**
       * Call iteratively the getHWExploitationRatio method to evaluate each
       * frequency in the window computed before
       *
       * @return The decision corresponding to the next frequency to be evaluated (or a zeroDecision if it was the last frequency)
       */
      Decision evaluateFrequency ();

      /**
       * Compute the step1 and step2 for each core
       */
      void computeSteps ();

      /**
       * Compute the sequence corresponding to the aggregation of the 2-steps computation of all the cores previously computed
       *
       */
      void computeSequence ();

      /**
       * Executes the sequence of frequency/timeRatio couples in the runtime
       */
      Decision executeSequence ();

      /**
       * Is useful to check whether we have to read the counters or not (it's not necessary in the execution state)
       *
       * @return Whether the runtime is currently evaluating or executing the frequency sequence
       */
      inline bool isEvaluating () const{
         return this->curRuntimeState == EVALUATION;
      }


      /**
       * Method for printing debug information
       */
      inline void debug (const char *str) {
#ifndef NDEBUG
         if (NewAdaptiveDecisions::VERBOSE && this->unit.getOSId () == 0) {
            std::cerr << "DEBUG:: " << str << std::endl;
         }
#endif
         (void) str;
      }

      // TODO comment
      void logFrequency (unsigned int freqId) const;

      inline void debug (std::ostringstream& str) {
         this->debug (str.str ().c_str ());
      }

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
      inline float getHWExploitationRatio (const HWCounters & hwc) const{
         // CPU off -> IPC = 0
         if (hwc.cycles == 0)
         {
            std::cerr << "idle core" << std::endl;
            return 0;
         }

         uint64_t swRefCycles = hwc.cycles * ((double) this->unit.getFrequency (0) / this->unit.getCurFreq ());

         /*std::cerr << "hwc.cycles = " << hwc.cycles
           << " hwc.retired = " << hwc.retired << std::endl;
           std::cerr << "getFreq (0) = " << this->unit.getFrequency (0)
           << " getCurFreq () = " << this->unit.getCurFreq () << std::endl;*/
         
         return hwc.retired / (1. * swRefCycles);
      }

      /**
       * Computes the couple of freq Id and its associated sleepWindows to
       *  emulate a frequency that would lead to the provided degraded IPC
       * 
       * @param degradedIPC the target degraded IPC
       * @param cpu The cpu for which the virtual freq is computed
       */
      FreqChunkCouple getVirtualFreq (float degradedIPC, unsigned int cpu) const;

      // TODO comment
      void readCounters ();

      // TODO Comment
      TimeProfiler timeProfiler;

      /**
       * The current state of the decision maker
       */
      State curRuntimeState;
      /**
       * The current evaluation state of the decision maker (because several evaluation steps)
       */
      EvalState curEvalState;

      /**
       * The current frequency chunk we are executing (in the runtime execution step)
       */
      unsigned int currentSeqChunk;

      /**
       * Number of cpuIds that are handled by the Decision Maker
       */
      size_t nbCpuIds;

      /**
       * Number of frequencies that are available on the processor
       */
      size_t nbFreqs;

      /**
       * The decision couples computed by every core.
       */
      FreqChunkCouple *cpuDecs;

      /**
       * List of all the frequencies to evaluate
       */
      std::set<unsigned int> freqsToEvaluate;

      /**
       * Currently evaluated frequency. Related to freqsToEvaluate.
       */
      std::set<unsigned int>::iterator currentEvalFreqId;

      /**
       * The current sequence
       */
      std::vector<FreqChunk> sequence;

      /**
       * Maximum freq id used in the former sequence for stability evaluation.
       */
      unsigned int oldMaxFreqId;

      /**
       * The sleep window consumed by the whole sequence
       */
      unsigned int totalSleepWin;

      /**
       * IPC evaluation result.
       */
      float * ipcEval;

      /**
       * Size of the ipcEval array
       */
      size_t ipcEvalSize;

      /**
       * Helps to compute stability accross settings.
       */
      FreqSelector freqSelector;

#ifdef REST_LOG
      /**
       * Log reference to trace the runtime decisions and important information
       */
      Logger *log;
#endif

};

#endif
