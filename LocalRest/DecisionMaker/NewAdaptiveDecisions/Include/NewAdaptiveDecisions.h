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

struct FreqChunk {
	unsigned int freqId;
	float timeRatio;
};

// Frequency sequence data structure
enum { STEP1 = 0, STEP2 };
struct DecisionCouple {
	Decision step[2];
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
      
			// TODO Comment
			void setProfiler (Profiler *prof) {
				this->prof = prof;
			}
      
   private:
			// TODO Comment
			TimeProfiler timeProfiler;

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
      static const unsigned int IPC_EVAL_TIME = 20;

      /**
       * Minimal execution time once a frequency is chosen (us).
       */
      static const unsigned int MIN_SLEEP_WIN = 500;

			// TODO comment
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
			 * Compute the center of the frequency window for the current cpu
			 * that is being evaluated, according to the two previous steps
			 */
      inline void computeFreqWindowCenter (unsigned int id) {
				assert (id < this->nbCpuIds);

				if(this->dec[id].step[STEP1].timeRatio < this->dec[id].step[STEP2].timeRatio) {
					this->freqWindowCenter = this->dec[id].step[STEP1].freqId;
				}	else {
					this->freqWindowCenter = this->dec[id].step[STEP2].freqId;
				}
			}

			/**
			 * Compute the window of frequencies that will be evaluated by the runtime
			 */
      inline void computeFreqWindow () {
				unsigned int minFreqId = 
					rest_max (0
							,
								(int) this->freqWindowCenter
							- (int) NewAdaptiveDecisions::NB_EVAL_NEAR_FREQS
					);
				unsigned int maxFreqId =
					rest_min (this->nbFreqs - 1
							,
								this->freqWindowCenter
							+ NewAdaptiveDecisions::NB_EVAL_NEAR_FREQS
					);


				//std::cerr << "nbFreqs = " << this->nbFreqs << ", min = " << minFreqId << ", max = " << maxFreqId << std::endl;
				
				for (unsigned int i = minFreqId; i <= maxFreqId; i++) {
					//std::cerr << "enabling " << i << std::endl;
					this->evaluateFreq[i] = true;
				}
			}

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
			 * Decides wheter we can increase the sleeping time of the runtime
			 * considering the current and the previous generated sequences.
			 * If they are similar enough, we expand the sleeping time,
			 * else we reset it
			 */
      void lengthenSleepTime ();
 
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
					uint64_t swRefCycles = hwc.cycles * ((double) this->unit.getFrequency (0) / this->unit.getCurFreq ());
					/*std::cerr << "hwc.cycles = " << hwc.cycles
										<< " hwc.retired = " << hwc.retired << std::endl;
					std::cerr << "getFreq(0) = " << this->unit.getFrequency(0)
										<< " getCurFreq () = " << this->unit.getCurFreq() << std::endl;*/
					return hwc.retired / (1. * swRefCycles);
				}

      /**
       * Computes the couples of FreqId and their associated sleepWindows to emulate a frequency associated to the 5% degraded IPC
       * @param	degradedIPC the 5% degraded IPC
       * @param minFreqId the Lowest bound of the frequency window
       * @param maxFreqId the Highesr bound of the frequency window
       * @param Step1 the first member of the couple
       * @param Step2 the second member of the couple
       */
      void getVirtualFreq (float degradedIPC, unsigned int cpu);

			// TODO comment
			void readCounters ();
      
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
			 * Size of the ipcEval array
			 */
      size_t ipcEvalSize;

			/**
			 * Number of cpuIds that are handled by the Decision Maker
			 */
			size_t nbCpuIds;

			/**
			 * Number of frequencies that are available on the processor
			 */
			size_t nbFreqs;

			/**
			 * Table of step1/step2 couples that are generated in computeSteps during the evaluation
			 */
			DecisionCouple *dec;
			float *timeRatios;

			/**
			 * TODO comment
			 */
  		bool *evaluateFreq;
			std::vector<unsigned int> freqsToEvaluate;
			
			/**
			 * The old and current sequences that are being exeucuted in the execution step
			 */
			std::vector<FreqChunk> sequence;
			unsigned int oldMaxFreqId;
			
			/**
			 * Array containing the states of the steps only for the computeSequence method.
			 * It's been added as a class variable to allocate memory only once instead of
			 * reallocating the same space each time computeSequence is called (optimization)
			 */
			short int *stepState;
			
			/**
       * The center of the freq window
       */
      unsigned int freqWindowCenter;

      /**
       * The Total SleepWindow, sleepWinStep1+sleelWinStep2
       */
      unsigned int totalsleepWin;
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
       * Current Frequency
       */
      unsigned int currentFreqId;

			#ifdef REST_LOG
			/**
			 * Log reference to trace the runtime decisions and important information
			 */
			Logger *log;
			#endif

			// TODO Comment
			Profiler *prof;
			FreqSelector freqSelector;
};

#endif
