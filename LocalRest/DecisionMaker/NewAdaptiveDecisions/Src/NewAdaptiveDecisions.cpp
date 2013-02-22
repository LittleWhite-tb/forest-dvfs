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

#include <algorithm>
#include <iostream>
#include <cstring>
#include <unistd.h>
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

struct CoupleInfo {
   float ipc;
   unsigned int freqId;
};


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
   this->skipSequenceComputation = false;
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
   this->usage = new float [this->nbCpuIds];

   // Check that allocation was successful
   handleAllocation (this->ipcEval);
   handleAllocation (this->usage);

   // Reserve the critical vectors for performance reasons
   this->sequence.reserve (this->nbCpuIds+1);

   // setup an initial state for the decision
   this->lastSequence.step [STEP1].freqId = 0;
   this->lastSequence.step [STEP1].timeRatio = 0;
   this->lastSequence.step [STEP2].freqId = 0;
   this->lastSequence.step [STEP2].timeRatio = 0;
}

NewAdaptiveDecisions::~NewAdaptiveDecisions (void)
{
   delete [] this->ipcEval;
   delete [] this->usage;
}

void NewAdaptiveDecisions::getAvgIPCPerFreq (float *avgIPC)
{
   /*std::cout << "Activity level: ";
   for (unsigned int c = 0; c < this->nbCpuIds; c++)
   {
      std::cout << this->usage [c] << " ";
   }
   std::cout << std::endl;*/


   for (std::set<unsigned int>::iterator it = this->freqsToEvaluate.begin ();
      it != this->freqsToEvaluate.end ();
      it++)
   {
      unsigned int activeCpus = 0;
      float ipc = 0;

      for (unsigned int c = 0; c < this->nbCpuIds; c++)
      {
         if (this->usage [c] >= ACTIVITY_LEVEL)
         {
            ipc += this->ipcEval [*it * this->nbCpuIds + c];
            activeCpus++;
         }
      }

      if (activeCpus == 0)
      {
         avgIPC [*it] = 0;
         continue;
      }

      ipc = ipc / activeCpus;
      avgIPC [*it] = ipc;

      std::cout << "IPC for freq " << *it << " = " << ipc << std::endl;
   }
}

void NewAdaptiveDecisions::getMaxIPCs (float *IPCs, std::vector<float> & maxIPCs)
{
   maxIPCs.clear();

   for (unsigned int c = 0; c < this->nbCpuIds; c++)
   {
      float maxIPC = 0;

      for (std::set<unsigned int>::iterator it = this->freqsToEvaluate.begin ();
         it != this->freqsToEvaluate.end ();
         it++)
      {
         maxIPC = rest_max (maxIPC, IPCs [*it * this->nbCpuIds + c]);
      }

      maxIPCs.push_back(maxIPC);
   }
}

FreqChunkCouple NewAdaptiveDecisions::getBestCouple (float *IPCs, float d, float *coupleEnergy)
{
   std::vector<unsigned int> smallerIpc;
   std::vector<unsigned int> greaterIpc;
   std::vector<float> maxIPCs;

   // how many cores are active?
   unsigned int activeCpus = 0;
   for (unsigned int c = 0; c < this->nbCpuIds; c++)
   {
      if (this->usage [c] >= ACTIVITY_LEVEL)
      {
         activeCpus++;
      }
   }

   // no active cores?
   if (activeCpus == 0)
   {
      FreqChunk step1, step2;

      step1.freqId = 0;
      step2.freqId = 0;
      step1.timeRatio = 1;
      step2.timeRatio = 0;

      FreqChunkCouple couple;
      couple.step [STEP1] = step1;
      couple.step [STEP2] = step2;

      if (coupleEnergy != NULL)
      {
         *coupleEnergy = 0;
      }

      //std::cout << "All cores idle" << std::endl;
      return couple;
   }

   // compute max IPC per core
   getMaxIPCs(IPCs, maxIPCs);

   // split frequencies depending on their IPCs
   for (std::set<unsigned int>::iterator it = this->freqsToEvaluate.begin ();
        it != this->freqsToEvaluate.end ();
        it++)
   {
      bool isLower = true, isHigher = true;

      //std::cout << "Freq " << *it << ": ";

      for (unsigned int c = 0; c < this->nbCpuIds; c++)
      {
         if (this->usage [c] >= ACTIVITY_LEVEL)
         {
            //std::cout << IPCs [*it * this->nbCpuIds + c] << " ";

            if (IPCs [*it * this->nbCpuIds + c] < d * maxIPCs[c])
            {
               isHigher = false;
            }
            else
            {
               isLower = false;
            }
         }
      }

      //std::cout << std::endl;

      if (isLower)
      {
         //std::cout << "smaller freq: " << *it <<  std::endl;
         smallerIpc.push_back (*it);
      }
      else if (isHigher)
      {
         //std::cout << "greater freq: " << *it << std::endl;
         greaterIpc.push_back (*it);
      }
   }

   // precompute t_i/t_ref * W_i/W_ref for every frequency i
   float *e_ratios = new float [this->nbFreqs];
   float Pref = this->unit.getPowerAt (*this->freqsToEvaluate.begin (), activeCpus);
   e_ratios [*this->freqsToEvaluate.begin ()] = activeCpus;

   for (std::set<unsigned int>::iterator it = this->freqsToEvaluate.begin ()++;
        it != this->freqsToEvaluate.end ();
        it++)
   {
      float Pi = this->unit.getPowerAt (*it, activeCpus);

      e_ratios [*it] = 0;

      for (unsigned int c = 0; c < this->nbCpuIds; c++)
      {
         if (this->usage [c] >= ACTIVITY_LEVEL)
         {
            float IPCref = IPCs [*this->freqsToEvaluate.begin () * this->nbCpuIds + c];
            float IPCi = IPCs [*it * this->nbCpuIds + c];

            e_ratios [*it] += (IPCref / IPCi) * (Pi / Pref);
         }
      }
   }

   // only greater freqs? IPC is ensured, whatever we chose, go to the smallest known frequency
   if (smallerIpc.size () == 0)
   {
      FreqChunk step1, step2;
      // sets are sorted
      unsigned int minFreq = *this->freqsToEvaluate.begin ();

      step1.freqId = minFreq;
      step2.freqId = 0;
      step1.timeRatio = 1;
      step2.timeRatio = 0;

      FreqChunkCouple couple;
      couple.step [STEP1] = step1;
      couple.step [STEP2] = step2;

      if (coupleEnergy != NULL)
      {
         *coupleEnergy = e_ratios[minFreq];
      }

      //std::cout << "No smaller freqs, choosing " << minFreq << std::endl;

      delete [] e_ratios;
      return couple;
   }

   // note: case with only smaller freqs is only due to mistakes in measurements
   // for safety we go to maximal freq here
   if (greaterIpc.size () == 0)
   {
      FreqChunk step1, step2;
      // sets are sorted
      unsigned int maxFreq = *--this->freqsToEvaluate.end ();

      step1.freqId = maxFreq;
      step2.freqId = 0;
      step1.timeRatio = 1;
      step2.timeRatio = 0;

      FreqChunkCouple couple;
      couple.step [STEP1] = step1;
      couple.step [STEP2] = step2;

      if (coupleEnergy != NULL)
      {
         *coupleEnergy = e_ratios[maxFreq];
      }

      //std::cout << "No greater freqs, choosing " << maxFreq << std::endl;

      delete [] e_ratios;
      return couple;
   }


   // we want min (r_i*e_i/e_ref+r_j*e_j/e_ref) among all couples (f_i, f_j)
   // with r_i and r_j the time ratios for f_i and f_j in the couple
   // and with e_i/e_ref = IPC_ref/IPC_i * W_i/W_ref
   // (as IPC_ref/IPC_i = CPI_i/CPI_ref and CPI_i/CPI_ref = t_i/t_ref)

   FreqChunkCouple bestCouple = {{{0, 0}, {0, 0}}};
   float bestCoupleE = std::numeric_limits<float>::max ();

   for (std::vector<unsigned int>::iterator sit = smallerIpc.begin ();
        sit != smallerIpc.end ();
        sit++)
   {
      for (std::vector<unsigned int>::iterator git = greaterIpc.begin ();
           git != greaterIpc.end ();
           git++)
      {
         FreqChunk step1, step2;

         unsigned int smaller = (*sit);
         unsigned int greater = (*git);
         
         step1.freqId = smaller;
         step2.freqId = greater;

         // apply the highest frequency as much as possible, find the smallest
         // time ratio for the smallest freq
         float minRatio = 2;
         for (unsigned int c = 0; c < this->nbCpuIds; c++)
         {
            if (this->usage [c] >= ACTIVITY_LEVEL)
            {
               minRatio = rest_min(minRatio, 
                  (IPCs[greater * this->nbCpuIds + c] - d * maxIPCs[c])
                  / (IPCs[greater * this->nbCpuIds + c] - IPCs[smaller * this->nbCpuIds + c]));
            }
         }

         step1.timeRatio = minRatio;
         step2.timeRatio = 1 - step1.timeRatio;

         assert (step1.timeRatio <= 1);
         assert (step2.timeRatio <= 1);

         FreqChunkCouple couple;
         couple.step [STEP1] = step1;
         couple.step [STEP2] = step2;

         float coupleE = step1.timeRatio * e_ratios [smaller] + step2.timeRatio * e_ratios [greater];

         //std::cout << "couple ((" << couple.step [STEP1].freqId << "," << couple.step [STEP1].timeRatio 
         //   << "),(" << couple.step [STEP2].freqId << "," << couple.step [STEP2].timeRatio << ")) energy = "  << coupleE << std::endl;

         if (coupleE < bestCoupleE)
         {
            bestCouple = couple;
            bestCoupleE = coupleE;
         }
      }
   }

   assert (bestCoupleE < std::numeric_limits<float>::max ());

   delete [] e_ratios;

   if (coupleEnergy != NULL)
   {
      *coupleEnergy = bestCoupleE;
   }

   return bestCouple;
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
   logger << "\tStep1 {" << this->lastSequence.step [STEP1].freqId << ", " << this->lastSequence.step [STEP1].timeRatio << "}" << std::endl;
   logger << "\tStep2 {" << this->lastSequence.step [STEP2].freqId << ", " << this->lastSequence.step [STEP2].timeRatio << "}" << std::endl;
#endif

   this->log->logOut (logger);
#endif
}

Decision NewAdaptiveDecisions::initEvaluation ()
{
   Decision res;
   unsigned int freqWindowCenter;

   // Reset the list of frequencies to evaluate
   this->freqsToEvaluate.clear ();

   // Computing the new freq window
   if (this->lastSequence.step [STEP1].timeRatio > this->lastSequence.step [STEP2].timeRatio)
   {
      freqWindowCenter = this->lastSequence.step [STEP1].freqId;
   }
   else 
   {
      freqWindowCenter = this->lastSequence.step [STEP2].freqId;
   }

   // request evaluation of all close frequencies
   unsigned int minFreqId = rest_max (0, 
      (int) freqWindowCenter - (int) NewAdaptiveDecisions::NB_EVAL_NEAR_FREQS);
   unsigned int maxFreqId = rest_min (this->nbFreqs - 1,
      freqWindowCenter + NewAdaptiveDecisions::NB_EVAL_NEAR_FREQS);

   for (unsigned int i = minFreqId; i <= maxFreqId; i++)
   {
      this->freqsToEvaluate.insert (i);
   }

   // also request evaluation for the maximal frequency as it must serve as a
   // reference point to compute the degradation rate
   if (maxFreqId < this->nbFreqs - 1)
   {
      this->freqsToEvaluate.insert (this->nbFreqs - 1);
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
      if (HWexp < 0 || isnan (HWexp))
      {
         hwcPanic = true;
      }
      else
      {
         this->ipcEval [currentFreq*this->nbCpuIds + i] = HWexp;
      }

      // also update cpu usage
      this->usage [i] = this->getCPUUsage (hwc);
   }

   // in case of panic, restart evaluation
   if (!hwcPanic) {
      this->currentEvalFreqId++;
   }

   // last evaluation step?
   if (!hwcPanic && this->currentEvalFreqId == this->freqsToEvaluate.end ()) {
      this->curEvalState = SEQUENCE_COMPUTATION;

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

void NewAdaptiveDecisions::computeSequence () 
{
   bool logFrequency = false;
   unsigned int maxRatioFreqId = 0;
   float bestE = std::numeric_limits<float>::max ();
   float bestD = 0;
   FreqChunkCouple bestCouple = {{{0, 0}, {0, 0}}};

   // test all perfmormance level by steps of 1%
   for (float d = 1; d >= USER_PERF_REQ; d -= 0.01)
   {
      FreqChunkCouple couple;
      float coupleE;

      couple = getBestCouple (this->ipcEval, d, &coupleE);

      //std::cout << "IPC: " << targetIPC
      //   << " couple: ((" << couple.step [STEP1].freqId << "," << couple.step [STEP1].timeRatio << "),(" << couple.step [STEP2].freqId << "," << couple.step [STEP2].timeRatio 
      //   << ")) energy: " << coupleE << std::endl;

      if (coupleE < bestE)
      {
         bestCouple = couple;
         bestE = coupleE;
         bestD = d;
      }
   }

   std::cout << "d: " << bestD
      << " couple: ((" << bestCouple.step [STEP1].freqId << "," << bestCouple.step [STEP1].timeRatio << "),(" << bestCouple.step [STEP2].freqId << "," << bestCouple.step [STEP2].timeRatio 
      << ")) energy: " << bestE << std::endl;

   this->sequence.clear (); 
   this->sequence.push_back (bestCouple.step [STEP1]);
   this->sequence.push_back (bestCouple.step [STEP2]);

   this->freqSelector.promote (bestCouple.step [STEP1].freqId,
                               bestCouple.step [STEP1].timeRatio);
   this->freqSelector.promote (bestCouple.step [STEP2].freqId,
                               bestCouple.step [STEP2].timeRatio);

   // remember this for freq window update
   this->lastSequence = bestCouple;

   if (bestCouple.step [STEP1].timeRatio > bestCouple.step [STEP2].timeRatio)
   {
      maxRatioFreqId = bestCouple.step [STEP1].freqId;
   }
   else
   {
      maxRatioFreqId = bestCouple.step [STEP2].freqId;
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

   //std::cout << " tsw = " << this->totalSleepWin << ", " << maxRatioFreqId << ", " << this->oldMaxFreqId << std::endl; 
   
	if (logFrequency) {
	 this->logFrequency (maxRatioFreqId);	
	}

	this->oldMaxFreqId = maxRatioFreqId; 
}

Decision NewAdaptiveDecisions::evaluate ()
{
	switch (this->curEvalState)
   {
		case EVALUATION_INIT:
			return this->initEvaluation ();

		case FREQUENCY_EVALUATION:
			return this->evaluateFrequency ();

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

      // reset IPC related counters before starting sequence
      if (this->currentSeqChunk == 0)
      {
         this->readCounters ();
      }

		return res;	
   }

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

void NewAdaptiveDecisions::readCounters ()
{
   assert (this->prof != 0);

   for (unsigned int c = 0; c < this->nbCpuIds; c++)
   {
      HWCounters hwc;
      this->prof->read (hwc, c);
   }
}
