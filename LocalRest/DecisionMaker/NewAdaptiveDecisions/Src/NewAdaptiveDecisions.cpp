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
 * @file NewAdaptiveDecisions.cpp
 * The NewAdaptiveDecisions class is in this file
 */

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <stdint.h>
#include <cassert>
#include <limits>

#include "NewAdaptiveDecisions.h"
#include "Common.h"
#include <math.h>
#include "FreqSelector.h"

#ifdef REST_LOG
	#include "Logger.h"
	#include <iomanip>
#endif

NewAdaptiveDecisions::NewAdaptiveDecisions (const DVFSUnit& unit) :
   DecisionMaker (unit),
	 timeProfiler (),
	 freqSelector (unit.getNbFreqs ())
{
	this->curRuntimeState = EVALUATION;
	this->curEvalState = EVALUATION_INIT;
	this->currentSeqChunk = 0;
	this->totalSleepWin = NewAdaptiveDecisions::MIN_SLEEP_WIN;
	this->oldMaxFreqId = 0;
	this->prof = NULL;
#ifdef REST_LOG
	this->log = Logger::getLog (this->unit.getId ());
#endif

	this->nbCpuIds = unit.getNbCpuIds ();
	this->nbFreqs = unit.getNbFreqs ();
	assert (this->nbCpuIds != 0);
	assert (this->nbFreqs != 0);

	this->ipcEvalSize = this->nbFreqs * this->nbCpuIds;

	// Allocate data
	this->ipcEval = new float [this->ipcEvalSize];
	this->cpuDecs = new FreqChunkCouple [this->nbCpuIds];
   this->cpuUsageInfo = new unsigned int [2*this->nbCpuIds];

	// Check that allocation was successful
	handleAllocation (this->ipcEval);
	handleAllocation (this->cpuDecs); 

	// Reserve the critical vectors for performance reasons
	this->sequence.reserve (this->nbCpuIds+1);

   // setup an initial state for former decisions
   for (unsigned int i = 0; i < this->nbCpuIds; i++) {
      this->cpuDecs [i].step [STEP1].freqId = 0;
      this->cpuDecs [i].step [STEP1].timeRatio = 0;

      this->cpuDecs [i].step [STEP2].freqId = 0;
      this->cpuDecs [i].step [STEP2].timeRatio = 0;
   }
}

NewAdaptiveDecisions::~NewAdaptiveDecisions (void)
{
	delete [] this->ipcEval;
	delete [] this->cpuDecs;
   delete [] this->cpuUsageInfo;
}

FreqChunkCouple NewAdaptiveDecisions::getVirtualFreq (float degradedIPC,
   unsigned int cpu) const
{
   assert (degradedIPC >= 0);
	assert (cpu < this->nbCpuIds);

   FreqChunkCouple res;

	FreqChunk &Step1 = res.step [STEP1];
	FreqChunk &Step2 = res.step [STEP2];

	float max = -1;
	float min = std::numeric_limits<float>::max ();
	
	int tmpfreqId1 = -1;
	int tmpfreqId2 = -1;

	Step1.freqId = 0;
	Step2.freqId = 0;
	Step1.timeRatio = 0.0f;
	Step2.timeRatio = 0.0f;

	// find the two consecutive IPC just above and just below the degradedIPC
   for (std::set<unsigned int>::iterator it = this->freqsToEvaluate.begin ();
        it != this->freqsToEvaluate.end ();
        it++)
   {
		unsigned int idx = *it * this->nbCpuIds + cpu;

		if (max < this->ipcEval [idx] && this->ipcEval [idx] <= degradedIPC)
      {
			max = this->ipcEval [idx];
			tmpfreqId1 = *it;	
		}

		if (min > this->ipcEval [idx] && this->ipcEval [idx] > degradedIPC)
      {
			min = this->ipcEval [idx];
			tmpfreqId2 = *it;
		}
	}

   // no freq below thredshold?
	if (tmpfreqId1 == -1)
   {
      // no freq above threshold?
		if (tmpfreqId2 == -1)
      {
         // no freq is above and none is below the threshold
         // we should never reach that point!
         std::cerr << "The impossible happened!" << std::endl;
         exit (EXIT_FAILURE);
		}

      // only use freq2
      Step2.freqId = tmpfreqId2;
		Step2.timeRatio = 1;
	} 
   else
   {
		assert (tmpfreqId2 != -1);
      
		//once the freq are found, compute the time to spend in each
		Step2.freqId = tmpfreqId2;
		Step2.timeRatio = (degradedIPC - max) / (min - max);

		Step1.freqId = tmpfreqId1;
		Step1.timeRatio = 1 - Step2.timeRatio;	
	}

	assert (Step1.freqId < this->nbFreqs);
	assert (Step2.freqId < this->nbFreqs);
	assert (Step1.timeRatio <= 1);
	assert (Step2.timeRatio <= 1);
   assert (Step1.timeRatio >= 0);
	assert (Step2.timeRatio >= 0);

   return res;
}

void NewAdaptiveDecisions::logFrequency (unsigned int freqId) const
{	
	assert (freqId < this->nbFreqs);
   
   (void) freqId;

#ifdef REST_LOG
   std::ostringstream logger;
   logger << freqId;

#if 0
	float totTimeRatio = 0.;
	for (unsigned int i = 0; i < this->sequence.size (); i++) {
		logger << "{" << this->sequence [i].freqId << ", " << this->sequence [i].timeRatio << "}, ";
		totTimeRatio += this->sequence [i].timeRatio;
	}
	std::cerr << logger.str () << std::endl;
	assert (totTimeRatio == 1);
	logger << "}" << std::endl;	


		float ipcMax = 0.;
		unsigned int max = 0;

		for (unsigned int i = 1; i < this->ipcEvalSize; i++) {
			ipcMax = rest_max (ipcMax, this->ipcEval [i]);
			if (this->ipcEval [i] > this->ipcEval [max]) {
				max = i;
			}
		}
		/* modulo : core
		 * division : freq selection (ou inversement)
		 */
		logger << " " << this->ipcEval [max] << std::endl;

	logger << "IPC Eval array :" << std::endl;	
	logger << std::endl;
	for (std::set<unsigned int>::iterator it = this->freqsToEvaluate.begin ();
		it != this->freqsToEvaluate.end ();
		it++) {
			unsigned int baseIdx = *it*this->nbCpuIds;
			logger << *it << " ";
			for (unsigned int j = 0; j < this->nbCpuIds; j++) {
				logger << std::setw (10) << this->ipcEval [baseIdx+j] << " ";
			}
			logger << std::endl;
	}


	// Debug print
	for (unsigned int i = 0; i < this->nbCpuIds; i++) {
		logger << "Core #" << i << ":" << std::endl;
		logger << "\tStep1 {" << this->cpuDecs [i].step [STEP1].freqId << ", " << this->cpuDecs [i].step [STEP1].timeRatio << "}" << std::endl;
		logger << "\tStep2 {" << this->cpuDecs [i].step [STEP2].freqId << ", " << this->cpuDecs [i].step [STEP2].timeRatio << "}" << std::endl;
	}
#endif

	this->log->logOut (logger);
#endif
}

unsigned int NewAdaptiveDecisions::getCurrentCpuUsage () const{
   unsigned activeCpus = 0;
   std::ifstream ifs;
   std::ostringstream oss;
   std::string str;
   unsigned int cpuIdx = 0;

   ifs.open ("/proc/stat");
   if (!ifs) {
      std::cerr << "Error: Cannot open /proc/stat for cpu usage info. Are you running on a Linux machine ?" << std::endl;
      exit (EXIT_FAILURE);
   }

   /* For each line */ 
   while (!std::getline (ifs, str, '\n').eof ()) {
      /* We want to get the lines starting with cpu{0-9)+ */
      if (!str.compare (0, 3, "cpu")) {
         const char *cstr = str.c_str ();
         
         /* Eliminate the non numbered "cpu" lines */
         if (cstr[3] != ' ') {
            unsigned int i = 3;
            /* get to the next space */
            while (cstr [i] != ' ') i++;
            std::istringstream iss (cstr + i);

            /* go through the first 4 entries */
            unsigned int totalTime = 0, idleTime = 0;
            unsigned int value;
            
            /* Get total awake time with first 3 values */
            //std::cerr << "cpu #" << cpuIdx;
            for (unsigned int nb = 0; nb < 4; nb++) {
               iss >> value;
               //std::cerr << " " << value;
               assert (!iss.bad () && !iss.fail ());
               if (nb == 3) {
                  idleTime = value;
               }
               totalTime += value;
            } 

            unsigned int diffIdle = rest_max (0, (int)(idleTime - this->cpuUsageInfo [cpuIdx * 2 + 1]));
            unsigned int diffTotal = rest_max (0, (int)(totalTime - this->cpuUsageInfo [cpuIdx * 2]));

            float ratio;
            if (diffIdle == 0) {
               ratio = 100.;
            } else {
               ratio = (abs(diffTotal - diffIdle)) / (float)diffIdle * 100;
            }
            ratio = rest_min (100, ratio);

            std::cerr << "#" << cpuIdx << ": " << std::setw (10) << ratio << "%, ";
            (void) ratio;
            this->cpuUsageInfo [cpuIdx * 2] = totalTime;
            this->cpuUsageInfo [cpuIdx * 2 + 1] = idleTime;

            cpuIdx++;
         }
      }
   } 
   std::cerr << "         \r";
   //exit (EXIT_FAILURE);
   
   ifs.close ();

   return activeCpus;
}

Decision NewAdaptiveDecisions::initEvaluation ()
{
	Decision res;

	// Reset the list of frequencies to evaluate
	this->freqsToEvaluate.clear ();
	
   // Computing the freq window for every core
   for (unsigned int i = 0; i < this->nbCpuIds; i++) {
      unsigned int freqWindowCenter;

      // what is the main frequency applied at previous step?
      if (this->cpuDecs [i].step [STEP1].timeRatio < this->cpuDecs [i].step [STEP2].timeRatio)
      {
         freqWindowCenter = this->cpuDecs [i].step [STEP1].freqId;
      }
      else 
      {
         freqWindowCenter = this->cpuDecs [i].step [STEP2].freqId;
      }

      // request evaluation of all close frequencies
      unsigned int minFreqId = rest_max (0, 
         (int) freqWindowCenter - (int) NewAdaptiveDecisions::NB_EVAL_NEAR_FREQS);
      unsigned int maxFreqId = rest_min (this->nbFreqs - 1,
         freqWindowCenter + NewAdaptiveDecisions::NB_EVAL_NEAR_FREQS);

      for (unsigned int i = minFreqId; i <= maxFreqId; i++) {
         this->freqsToEvaluate.insert (i);
      }
   }
	
	assert (this->freqsToEvaluate.size () <= this->nbFreqs);
	assert (this->freqsToEvaluate.size () > 0);
	
	// We're all set! Let's evaluate the frequencies!
	this->curEvalState = FREQUENCY_EVALUATION;
	
	// Prepare the first frequency to be the minimum of our freshly computed frequency window
	this->currentEvalFreqId = this->freqsToEvaluate.begin ();
	res.freqId = *this->currentEvalFreqId;
   res.cpuId = this->unit.getOSId ();
	res.sleepWin = NewAdaptiveDecisions::IPC_EVAL_TIME;	
	res.freqApplyDelay = this->unit.getSwitchLatency () / 1000;

   assert (res.freqId < this->nbFreqs);

   // time the evaluation for debuging purposes
   this->timeProfiler.evaluate (EVALUATION_INIT);

	return res;
}

Decision NewAdaptiveDecisions::evaluateFrequency () {
	assert (this->prof != NULL);

   Decision res;

	unsigned int currentFreq = *this->currentEvalFreqId;
	bool hwcPanic = false; // Decides whether the HWC values are reasonable
	
   // read counters
	for (unsigned int i = 0; i < this->nbCpuIds; i++) {
		HWCounters hwc;

		this->prof->read (hwc, i);
		float HWexp = this->getHWExploitationRatio (hwc);
		
      //std::cerr << "#" << i << ", freq #" << currentFreq << " IPC = " << HWexp << std::endl;
		if (HWexp < 0 || isnan (HWexp)) {
			hwcPanic = true;
		} else {
			this->ipcEval [currentFreq*this->nbCpuIds + i] = HWexp;
		}
   }

	// in case of panic, restart evaluation
   if (!hwcPanic) {
      this->currentEvalFreqId++;
   }
	
   // last evaluation step?
	if (!hwcPanic && this->currentEvalFreqId == this->freqsToEvaluate.end ()) {
		this->curEvalState = STEPS_COMPUTATION;

		this->timeProfiler.evaluate (FREQUENCY_EVALUATION);
		
      // do nothing here and immediately get back to the next step
      res.freqId = 0;
      res.cpuId = this->unit.getOSId ();
      res.sleepWin = 0;
      res.freqApplyDelay = 0;

		return res;
	}

   // evaluate the next frequency
   res.freqId = *this->currentEvalFreqId;
   res.cpuId = this->unit.getOSId ();
   res.sleepWin = NewAdaptiveDecisions::IPC_EVAL_TIME;
   res.freqApplyDelay = this->unit.getSwitchLatency () / 1000;
		
   assert (res.freqId < this->nbFreqs);

   return res;
}


void NewAdaptiveDecisions::computeSteps ()
{
#if 0
	std::cerr << "IPC Eval array :" << std::endl;
	for (unsigned int i = 0; i < this->nbFreqs; i++) {
		if (this->evaluateFreq [i]) {
						std::cerr << "{ ";
		}
		for (unsigned int j = 0; j < this->nbCpuIds; j++) {
						std::cerr << this->ipcEval [i*this->nbCpuIds+j] << " ";
		}
		if (this->evaluateFreq [i]) {
						std::cerr << " }";
		}
		std::cerr << std::endl;
	}
#endif

    this->getCurrentCpuUsage ();

   //std::cerr << activeCpus << std::endl;

   // make a decision per cpu
	for (unsigned int cpu = 0; cpu < this->nbCpuIds; cpu++)
   {
		float maxIPC = 0;

      for (std::set<unsigned int>::iterator it = this->freqsToEvaluate.begin ();
           it != this->freqsToEvaluate.end ();
           it++)
      {
			maxIPC = rest_max (maxIPC, this->ipcEval [*it * this->nbCpuIds + cpu]);
		}

		float degradedIPC = USER_PERF_REQ * maxIPC;
		cpuDecs [cpu] = this->getVirtualFreq (degradedIPC, cpu);
	}

   // go to next step
	this->curEvalState = SEQUENCE_COMPUTATION;

	this->timeProfiler.evaluate (STEPS_COMPUTATION);
}


//#if 0
void NewAdaptiveDecisions::computeSequence () 
{
   float *timeRatios = new float [this->nbFreqs];
   memset (timeRatios, 0, this->nbFreqs * sizeof (*timeRatios));
	 bool logFrequency = false;
	
	// store the maximal time ratio per frequency
   for (unsigned int i = 0; i < this->nbCpuIds; i++) {
		FreqChunk & step1 = this->cpuDecs [i].step [STEP1];
		FreqChunk & step2 = this->cpuDecs [i].step [STEP2];

		timeRatios [step1.freqId] = rest_max (timeRatios [step1.freqId], step1.timeRatio);
		timeRatios [step2.freqId] = rest_max (timeRatios [step2.freqId], step2.timeRatio);
	}

   // convert the time ratio of the steps into sequence time ratios:
   // decide among the frequencies requested by all the cores
   // currently we favor high frequencies
   // 1 | 0.2 | 0.5 | 0.7 -> 0 | 0 | 0.3 | 0.7
   float totTimeRatio = 0;
	 for (int i = this->nbFreqs - 1; i >= 0; i--)
   {
      timeRatios [i] = rest_min (1 - totTimeRatio, timeRatios [i]);	
      totTimeRatio = rest_min (1, totTimeRatio + timeRatios [i]);
   }
	 assert (totTimeRatio <= 1);

	// Search for max time ratio and its associated frequency
   // also promote the frequencies
	unsigned int maxRatioFreqId = 0;
	for (unsigned int i = 1; i < this->nbFreqs; i++)
   {
      this->freqSelector.promote (i, timeRatios [i]);

		if (timeRatios [i] > timeRatios [maxRatioFreqId]) {
			maxRatioFreqId = i;
		}
	}

#ifdef REST_LOG
   if (this->oldMaxFreqId != maxRatioFreqId)
   {
	 		logFrequency = true; 
   }
#endif

	// Lengthen sleeping time if necessary
	if (this->freqSelector.isFreqStable (maxRatioFreqId))
   {
		this->totalSleepWin = rest_min (NewAdaptiveDecisions::MAX_SLEEP_WIN, this->totalSleepWin * 2);
	}
   else
   {
		unsigned int minFreqWindow = rest_max ((int) this->oldMaxFreqId -
         (const int) NewAdaptiveDecisions::MAX_FREQ_WINDOW, 0);
		unsigned int maxFreqWindow =
         this->oldMaxFreqId + NewAdaptiveDecisions::MAX_FREQ_WINDOW;
		
		if (maxRatioFreqId < minFreqWindow || maxRatioFreqId > maxFreqWindow)
      {
			this->totalSleepWin = NewAdaptiveDecisions::MIN_SLEEP_WIN;
		}
	}

	// Generate the new sequence
	this->sequence.clear ();

   float totalTimeRatio = 0.0f;
   for (unsigned int i = 0; i < this->nbFreqs; i++)
   {
		if (timeRatios [i] * this->totalSleepWin < NewAdaptiveDecisions::MIN_SLEEP_WIN)
      {
         continue;
      }

      FreqChunk freqChunk = {i, timeRatios [i]};
      totalTimeRatio += timeRatios [i];

      this->sequence.push_back (freqChunk);
	}
	
	// most of the freqs have too small execution times
	if (totalTimeRatio < 1)
   {
      if (this->sequence.size () > 0)
      {
         // scale the time ratio of every frequency to reach a total of 1 
         for (std::vector<FreqChunk>::iterator it = this->sequence.begin ();
              it != this->sequence.end ();
              it++)
         {
            it->timeRatio = it->timeRatio / totalTimeRatio;
         }
      }
      // the sequence is empty
      else
      {
   		FreqChunk freqChunk = { maxRatioFreqId, 1};
		   this->sequence.push_back (freqChunk);
      }
	}

	if (logFrequency) {
	 this->logFrequency (maxRatioFreqId);	
	}

#if 0
	std::cerr << "#" << this->unit.getId () << ", st = " << this->totalSleepWin << " {";
	for (unsigned int i = 0; i < this->sequence.size (); i++) {
		std::cerr << "{" << this->sequence [i].freqId << ", " << this->sequence [i].timeRatio << " / " << this->freqSelector.getFrequencyMark (this->sequence [i].freqId) << " }, ";	
	}
	std::cerr << "}" << std::endl;
#endif

	this->oldMaxFreqId = maxRatioFreqId;

   delete [] timeRatios;
}
//#endif

Decision NewAdaptiveDecisions::evaluate ()
{
	switch (this->curEvalState)
   {
		case EVALUATION_INIT:
			return this->initEvaluation ();

		case FREQUENCY_EVALUATION:
			return this->evaluateFrequency ();

		case STEPS_COMPUTATION:
			this->computeSteps ();
			// No break here on purpose
		case SEQUENCE_COMPUTATION:
			Decision res;

			this->computeSequence ();
			
         this->curRuntimeState = SEQUENCE_EXECUTION;
			this->curEvalState = EVALUATION_INIT; // For the next evaluation
			
         this->timeProfiler.evaluate (SEQUENCE_COMPUTATION);
			
         // do nothing and go to the next step
         res.freqId = 0;
         res.cpuId = this->unit.getOSId ();
         res.sleepWin = 0;
         res.freqApplyDelay = 0;

         return res;
			break;
		
		default:
			std::cerr << "Error: Unknown Evaluation State "
								<< this->curRuntimeState << " not handled" << std::endl;
			exit (EXIT_FAILURE);
	};	
}

Decision NewAdaptiveDecisions::executeSequence ()
{
	size_t seqSize = this->sequence.size ();

	if (this->currentSeqChunk < seqSize) {
      Decision res;

		res.freqId = this->sequence [this->currentSeqChunk].freqId;
		res.cpuId = this->unit.getOSId ();
      res.sleepWin =	this->sequence [this->currentSeqChunk].timeRatio * this->totalSleepWin;
      res.freqApplyDelay = 0;
      
		assert (res.sleepWin <= this->totalSleepWin);

		this->currentSeqChunk++;

		return res;	
   }
   else
   {
      // this assert is not required, isn't it?
      //assert (this->currentSeqChunk > 0);

		// Reset the current sequence chunk that will be executed in the
      // execution state
		this->currentSeqChunk = 0;
	
		// Now, we loop back to the evaluation
		this->curRuntimeState = EVALUATION;

		// Profiler, remind we leave execution
		this->timeProfiler.evaluate (EXECUTION_SLOT);
		
		/*#ifdef REST_LOG
			std::ostringstream str;
			str << std::setw (10) << this->totalsleepWin << " ";
			this->timeProfiler.print (str);
			this->log->logOut (str);
		#endif*/

		Decision res;

      // do nothing and go to the next step
      res.freqId = 0;
      res.cpuId = this->unit.getOSId ();
      res.sleepWin = 0;
      res.freqApplyDelay = 0;

		return res;
	}
}

// New
Decision NewAdaptiveDecisions::takeDecision ()
{
	Decision res;
	
	switch (this->curRuntimeState)
   {
		case EVALUATION:
			res = this->evaluate ();
			break;

		case SEQUENCE_EXECUTION:
			res = this->executeSequence ();
			break;

		default:
			std::cerr << "Error: Unknown Runtime State " << this->curRuntimeState << " not handled" << std::endl;
			exit (EXIT_FAILURE);
	}
	
	return res;
}

void NewAdaptiveDecisions::readCounters () {
	assert (this->prof != 0);

	// We read only if necessary
	if (this->curRuntimeState == EVALUATION) {
		for (unsigned int i = 0; i < this->nbCpuIds; i++)
      {
			HWCounters hwc;
			this->prof->read (hwc, i);
		}
	}
}
