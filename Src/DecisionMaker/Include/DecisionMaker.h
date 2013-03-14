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
 * The AdaptiveDecisions class header is in this file
 */

#ifndef H_DECISIONMAKER
#define H_DECISIONMAKER

#include <set>
#include <sstream>
#include <time.h>

#include "glog/logging.h"

#include "Common.h"
#include "Config.h"
#include "Profiler.h"
#include "TimeProfiler.h"
#include "FreqSelector.h"
#include "Mode.h"
#include "Thread.h"

#ifdef REST_LOG
#include <vector>
#include "Logger.h"
#endif

namespace FoREST {
class DVFSUnit;
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
 * @class DecisionMaker
 *
 * The decision is taken adaptively here, trying to maximize the ressource usage
 * and exploiting the memory boundness ratio as a hint.
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
       * @param cfg The current configuration.
       */
      DecisionMaker (DVFSUnit& dvfsUnit, const Mode mode,
                     Config *config);

      /**
       * Destructor
       */
      ~DecisionMaker ();

      /**
       * Set the DecisionMaker a reference to its DVFSUnit counterpart threads
       */
      void setupThreads (std::vector<Thread*>& thread);

      /**
       * Initializes the evaluation, does several steps : 
       * - Compute the center of the frequency window (see computeFreqWindowCenter)
       * - Compute the frequency Window (see computeFreqWindow)
       * - Reset some values
       */
      void initEvaluation ();

      /**
       * Call iteratively the getHWExploitationRatio method to evaluate each
       * frequency in the window computed before
       *
       * @return The decision corresponding to the next frequency to be evaluated (or a zeroDecision if it was the last frequency)
       */
      void evaluateFrequency ();

      /**
       * Compute the step1 and step2 for each core
       */
      void computeSteps ();

      /**
       * Compute the sequence corresponding to the aggregation of the 2-steps
       * computation of all the cores previously computed
       */
      void computeSequence ();

      /**
       * Executes the sequence of frequency/timeRatio couples in the runtime
       */
      void executeSequence ();

   private:

      /**
       * Which DVFS unit we are handling.
      */
      DVFSUnit& unit;
      
      /**
       * Threads the handled DVFS unit is holding
       */
      std::vector<Thread*>* thread;

      /**
       * List of threads active for the current evaluation process
       */
      std::set<Thread*> activeThread;
      /**
       * Time required to evaluate the IPC for one frequency (us).
       */
      const unsigned int IPC_EVAL_TIME;

      /**
       * Minimal execution time once a frequency is chosen (us).
       */
      const unsigned int MIN_SLEEP_WIN;

      /**
       * Maximal execution time before re-evaluating which frequency to use
       * (us).
       */
      const unsigned int MAX_SLEEP_WIN;

      /**
       * Number of frequencies we consider bellow and above the current
       * frequency during the evaluation of frequencies. 2 here means that
       * at most five frequencies will be evaluated: two just bellow the current
       * one and two just above.
       */
      const unsigned int FREQ_WINDOW_SZ;

      /**
       * Number of frequencies to consider as similar regarding stability.
       * If this value is 1 for instance, the new frequency
       * applied can be the most representative frequency among those applied
       * during the last sequence, the one above, or the one below while we
       * still consider the situation as stable, i.e. the sleep window is 
       * increased.
       */
      const unsigned int STABILITY_WINDOW_SZ;

      /**
       * Approximate system power.
       */
      const float SYS_POWER;

      /**
       * Minimal performance requested by the user (in % of the max performance)
       * in the "performance" mode.
       */
      const float USER_PERF_REQ_PERF;

      /**
       * Minimal performance requested by the user (in % of the max performance)
       * in the "energy" mode.
       */
      const float USER_PERF_REQ_ENERGY;
      
      /**
       * Allowed performance slowdown currently requested by the user.
       */
      const float USER_PERF_REQ;

      /**
       * A CPU is active if its usage is above this.
       *
       * NOTE: on SandyBridge, inactive cores incorrectly report high number
       * of unhalted core cycles (up to 50% reported activity for an idle core)
       */
      const float ACTIVITY_LEVEL;

      /**
       * Computes the best frequency couple for achieving the target IPC with
       * the frequencies whose IPC is provided in IPCs.
       *
       * @param IPCs an array of IPC per frequency. On entry per frequency.
       * @param d Current degradation ratio to consider.
       * @param coupleEnergy Output parameter filled with the couple energy
       * estimation (not in J.) if non NULL.
       *
       * @return The frequency couple leading to the minimal energy consumption
       * and achieving the targetIPC.
       */
      FreqChunkCouple getBestCouple (float d, float *coupleEnergy);

      /**
       * Outputs the frequency in the log file
       */
      void logFrequency (unsigned int freqId) const; 
 
      /**
       * Computes the couple of freq Id and its associated sleepWindows to
       *  emulate a frequency that would lead to the provided degraded IPC
       * 
       * @param degradedIPC the target degraded IPC
       * @param cpu The cpu for which the virtual freq is computed
       */
      FreqChunkCouple getVirtualFreq (float degradedIPC, unsigned int cpu, unsigned int activeCpus) const;

      /**
       * The timeProfiler is a way to keep track of the overhead of the
       * different steps in our evaluation and execution processes.
       */
      TimeProfiler timeProfiler;

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
       * The current sequence
       */
      FreqChunkCouple sequence;

      /**
       * Maximum freq id used in the former sequence for stability evaluation.
       */
      unsigned int oldMaxFreqId;

      /**
       * The sleep window consumed by the whole sequence
       */
      unsigned int totalSleepWin;

      /**
       * Helps to compute stability accross settings.
       */
      FreqSelector freqSelector;

      /**
       * States whether we can skip the frequency sequence computation
       * This is typically true if no physical core is active
       */
      bool skipSequenceComputation;

      /**
       * Set of active cores.
       * An active CPU is a core having an activity higher than threshold \a ACTIVITY_LEVEL
       */
      std::set<unsigned int>activeCores;
};

} // namespace FoREST

#endif
