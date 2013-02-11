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
       * Threshold under which a logical processing unit is considered as not active
       */
      static const unsigned int ACTIVE_THRESHOLD = 30;

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
       * A CPU is active if its usage is above this.
       */
      static const float ACTIVITY_LEVEL = 0.3;

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
       * Converts IPC per core into IPC per processor. Actually averages the 
       * IPCs per core. The result is outputed in the argument.
       *
       * @param avgIPC Output parameter. Must be an allocated array able to
       * contain at least as many entries as the number of frequencies.
       */
      void getAvgIPCPerFreq (float *avgIPC);

      /**
       * Returns the maximal IPC in the array of IPCs per frequency.
       *
       * @param IPCs an array initialized with the IPC per frequency (one entry
       * per frequency.
       *
       * @return The maximal IPC value in the array among the frequencies tested
       * in the evaluation step.
       */
      float getMaxIPC (float *IPCs);

      /**
       * Computes the best frequency couple for achieving the target IPC with
       * the frequencies whose IPC is provided in IPCs.
       *
       * @param IPCs an array of IPC per frequency. On entry per frequency.
       * @param targetIPC The IPC the couple must achieve.
       * @param coupleEnergy Output parameter filled with the couple energy
       * estimation (not in J.) if non NULL.
       *
       * @return The frequency couple leading to the minimal energy consumption
       * and achieving the targetIPC.
       */
      FreqChunkCouple getBestCouple (float *IPCs, float targetIPC, float *coupleEnergy);

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

      /**
       * Outputs the frequency in the log file
       */
      void logFrequency (unsigned int freqId) const;

      /**
       * Debug the str param on stderr (see debug (const char *str))
       */
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
      inline float getHWExploitationRatio (const HWCounters & hwc) const
      {
         if (hwc.time == 0)
         {
            std::cerr << "no time elapsed since last measurement" << std::endl;
            return 0;
         }

         /*std::cerr << "hwc.cycles = " << hwc.cycles
           << " hwc.retired = " << hwc.retired << std::endl;
           std::cerr << "getFreq (0) = " << this->unit.getFrequency (0)
           << " getCurFreq () = " << this->unit.getCurFreq () << std::endl;*/

         return hwc.retired / (1. * hwc.time);
      }

      /**
       * Returns the CPU usage corresponding to the given hardware counter
       * measurement.
       *
       * @param hwc The hardware counter values.
       *
       * @return a usage ratio between 0 and 1 representing the CPU usage.
       */
      inline float getCPUUsage (HWCounters &hwc) const 
      {
         float res;

         if (hwc.time == 0)
         {
            std::cerr << "no time elapsed since last measurement" << std::endl;
            return 0;
         }

         // NOTE: RDTSC and refCycles run at the same freq
         res = hwc.refCycles / (1. * hwc.time);

         return rest_min (res, 1);
      }

      /**
       * Computes the couple of freq Id and its associated sleepWindows to
       *  emulate a frequency that would lead to the provided degraded IPC
       * 
       * @param degradedIPC the target degraded IPC
       * @param cpu The cpu for which the virtual freq is computed
       */
      FreqChunkCouple getVirtualFreq (float degradedIPC, unsigned int cpu, unsigned int activeCpus) const;

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
       * Last executed sequence.
       */
      FreqChunkCouple lastSequence;

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
       * IPC evaluation result. There are as many frequencies as the number of
       * frequencies times the number of cpu on the current DVFS unit. For a two
       * cores machines, ipcEval [1] is the IPC for freq 0 on core 1, ipcEval [2]
       * is the IPC for freq 1 on core 0.
       */
      float * ipcEval;

      /**
       * Size of the ipcEval array
       */
      size_t ipcEvalSize;

      /**
       * Last measured CPU activity per core. As many entries as the number of
       * cpu cores. Usage between 0 and 1.
       */
      float *usage;

      /**
       * Helps to compute stability accross settings.
       */
      FreqSelector freqSelector;

      /**
       * States whether we can skip the frequency sequence computation
       * This is typically true if no physical core is active
       */
      bool skipSequenceComputation;

#ifdef REST_LOG
      /**
       * Log reference to trace the runtime decisions and important information
       */
      Logger *log;
#endif

};

#endif
