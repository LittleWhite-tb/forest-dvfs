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
 * @file DecisionMaker.cpp
 * The DecisionMaker class is in this file
 */

#include <algorithm>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <cstdlib>
#include <math.h>
#include <vector>
#include <stdint.h>
#include <cassert>
#include <limits>
#include <time.h>

#include "DecisionMaker.h"
#include "Common.h"
#include "Topology.h"
#include "FreqSelector.h"
#include "Thread.h"

#ifdef REST_LOG
#include "Logger.h"
#include <iomanip>
#endif

namespace FoREST {

static inline void sleepForAWhile (unsigned long nanoseconds) {
   timespec req;
   req.tv_sec = 0;
   req.tv_nsec = nanoseconds*1000;
   nanosleep (&req, NULL);
}

DecisionMaker::DecisionMaker (DVFSUnit& dvfsUnit, const Mode mode,
                              Config &cfg) :
   unit (dvfsUnit),
   IPC_EVAL_TIME(cfg.getInt("IPC_EVALUATION_TIME")),
   MIN_SLEEP_WIN(cfg.getInt("MIN_SLEEP_WIN")),
   MAX_SLEEP_WIN(cfg.getInt("MAX_SLEEP_WIN")),
   FREQ_WINDOW_SZ(cfg.getInt("FREQ_WINDOW_SZ")),
   STABILITY_WINDOW_SZ(cfg.getInt("STABILITY_WINDOW_SZ")),
   SYS_POWER(cfg.getFloat("SYS_POWER")),
   USER_PERF_REQ_PERF(cfg.getFloat("USER_PERF_REQ_PERF")),
   USER_PERF_REQ_ENERGY(cfg.getFloat("USER_PERF_REQ_ENERGY")),
   USER_PERF_REQ ((mode == MODE_PERFORMANCE ? USER_PERF_REQ_PERF : USER_PERF_REQ_ENERGY)),
   ACTIVITY_LEVEL(cfg.getFloat("ACTIVITY_LEVEL")),
   timeProfiler (),
   oldMaxFreqId (0),
   totalSleepWin (DecisionMaker::MIN_SLEEP_WIN),
   freqSelector (dvfsUnit.getNbFreqs ()),
   skipSequenceComputation (false)
{
   // setup an initial state for the decision
   lastSequence.step [STEP1].freqId = 0;
   lastSequence.step [STEP1].timeRatio = 0;
   lastSequence.step [STEP2].freqId = 0;
   lastSequence.step [STEP2].timeRatio = 0;
}

void DecisionMaker::setupThreads (std::vector<Thread*>& threads) {
   this->thread = &threads; 
}

DecisionMaker::~DecisionMaker (void)
{}

FreqChunkCouple DecisionMaker::getBestCouple (float d, float *coupleEnergy)
{
   std::vector<unsigned int> smallerIpc;
   std::vector<unsigned int> greaterIpc;

   unsigned int nbActiveCores = this->activeCores.size ();

   DLOG (INFO) << "# active cores: " << nbActiveCores << std::endl;
   
   // no active cores?
   if (nbActiveCores == 0)
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

      //DLOG (INFO) << "All cores idle" << std::endl;
      return couple;
   }

   // compute max IPC per thread
   for (std::vector<Thread*>::iterator it = this->thread->begin ();
        it != this->thread->end ();
        it++) {
      (*it)->computeMaxIPC ();
   } 

   // split frequencies depending on their IPCs
   for (std::set<unsigned int>::iterator freq = this->freqsToEvaluate.begin ();
        freq != this->freqsToEvaluate.end ();
        freq++)
   {
      bool isLower = true, isHigher = true;

      // For each active thread
      for (std::set<Thread*>::iterator thread = this->activeThread.begin ();
           thread != this->activeThread.end ();
           thread++) {
         // Get the IPC at the requested freauency
         float threadIpc = (*thread)->getIPC (*freq);
         float maxIpc = (*thread)->getMaxIPC ();
         
         if (threadIpc < d * maxIpc) {
            isHigher = false;
         } else {
            isLower = false;
         }
      }

      if (isLower) {
         smallerIpc.push_back (*freq);
      } else if (isHigher) {
         greaterIpc.push_back (*freq);
      }
   }

   // precompute t_i/t_ref * W_i/W_ref for every frequency i
   std::vector<float> e_ratios (this->nbFreqs);
   const float Pref = this->unit.getPowerAt (*this->freqsToEvaluate.begin (), nbActiveCores);
   const float Psys = DecisionMaker::SYS_POWER;
   e_ratios [*this->freqsToEvaluate.begin ()] = nbActiveCores;

   std::set<unsigned int>::iterator freqIt = this->freqsToEvaluate.begin ();
   for (std::set<unsigned int>::iterator freq = ++freqIt;
        freq != this->freqsToEvaluate.end ();
        freq++)
   {
      float Pi = this->unit.getPowerAt (*freq, nbActiveCores);

      e_ratios [*freq] = 0;

      for (std::set<Thread*>::iterator thr = this->activeThread.begin ();
           thr != this->activeThread.end ();
           thr++)
      {
         float IPCref = (*thr)->getIPC (*this->freqsToEvaluate.begin ());
         float IPCi = (*thr)->getIPC (*freq);
         
         e_ratios [*freq] += (IPCref / IPCi) * ((Pi + Psys) / (Pref + Psys));
      }
   }

   // only greater freqs? IPC is ensured, whatever we choose, go to the smallest known frequency
   if (smallerIpc.size () == 0)
   {
      FreqChunk step1, step2;
      // sets are sorted highest from highest to lowest
      freqIt = this->freqsToEvaluate.end ();
      unsigned int minFreq = *(--freqIt);

      step1.freqId = minFreq;
      step2.freqId = 0;
      step1.timeRatio = 1;
      step2.timeRatio = 0;

      FreqChunkCouple couple;
      couple.step [STEP1] = step1;
      couple.step [STEP2] = step2;

      if (coupleEnergy != NULL)
      {
         *coupleEnergy = e_ratios [minFreq];
      }

      return couple;
   }

   // note: case with only smaller freqs is only due to mistakes in measurements
   // for safety we go to maximal freq here
   if (greaterIpc.size () == 0)
   {
      FreqChunk step1, step2;
      // sets are sorted
      unsigned int maxFreq = *this->freqsToEvaluate.begin ();

      step1.freqId = maxFreq;
      step2.freqId = 0;
      step1.timeRatio = 1;
      step2.timeRatio = 0;

      FreqChunkCouple couple;
      couple.step [STEP1] = step1;
      couple.step [STEP2] = step2;

      if (coupleEnergy != NULL)
      {
         *coupleEnergy = e_ratios [maxFreq];
      }

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
         for (std::set<Thread*>::iterator thread = this->activeThread.begin ();
              thread != this->activeThread.end ();
              thread++)
         {
            float greaterIpc = (*thread)->getIPC (greater);
            float smallerIpc = (*thread)->getIPC (smaller);
            float maxIpc = (*thread)->getMaxIPC ();
            minRatio = rest_min (minRatio, 
                  (greaterIpc - d * maxIpc)
               / (greaterIpc - smallerIpc));
         }

         step1.timeRatio = minRatio;
         step2.timeRatio = 1 - step1.timeRatio;

         assert (step1.timeRatio <= 1);
         assert (step2.timeRatio <= 1);

         FreqChunkCouple couple;
         couple.step [STEP1] = step1;
         couple.step [STEP2] = step2;

         float coupleE = step1.timeRatio * e_ratios [smaller] + step2.timeRatio * e_ratios [greater];

         DLOG (INFO) << "couple ((" << couple.step [STEP1].freqId << ","
            << couple.step [STEP1].timeRatio << "),(" 
            << couple.step [STEP2].freqId << "," 
            << couple.step [STEP2].timeRatio << ")) energy = "  << coupleE
            << std::endl;

         if (coupleE < bestCoupleE)
         {
            bestCouple = couple;
            bestCoupleE = coupleE;
         }
      }
   }

   assert (bestCoupleE < std::numeric_limits<float>::max ()); 

   if (coupleEnergy != NULL)
   {
      *coupleEnergy = bestCoupleE;
   }

   return bestCouple;
}


void DecisionMaker::logFrequency (unsigned int freqId) const
{	
#ifdef REST_LOG
   assert (freqId < this->nbFreqs);
   Logger *log = Logger::getLog (this->unit.getId ());
   std::ostringstream logger;
   logger << freqId;
   log->logOut (logger);
#endif
}

void DecisionMaker::initEvaluation ()
{ 
   unsigned int freqWindowCenter;

   // Reset the list of frequencies to evaluate
   this->freqsToEvaluate.clear ();

   // Computing the new freq window
   if (this->lastSequence.step [STEP1].timeRatio
       > this->lastSequence.step [STEP2].timeRatio)
   {
      freqWindowCenter = this->lastSequence.step [STEP1].freqId;
   }
   else 
   {
      freqWindowCenter = this->lastSequence.step [STEP2].freqId;
   }

   // request evaluation of all close frequencies
   unsigned int minFreqId = rest_max (0, 
      (int) freqWindowCenter - (int) DecisionMaker::FREQ_WINDOW_SZ);
   unsigned int maxFreqId = rest_min (this->nbFreqs - 1,
      freqWindowCenter + DecisionMaker::FREQ_WINDOW_SZ);

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

   // time the evaluation for debuging purposes
   this->timeProfiler.evaluate (EVALUATION_INIT); 
}

void DecisionMaker::evaluateFrequency () {
   bool hwcPanic = false;
   std::vector<Thread*>::iterator thr;

   // For each frequency in our window
   std::set<unsigned int>::iterator freq = freqsToEvaluate.begin ();
   while (freq != freqsToEvaluate.end ()) {
      // Apply the frequency
      this->unit.setFrequency (*freq);

      // Wait for the frequency to be applied
      sleepForAWhile (this->unit.getSwitchLatency ());

      // Reset all values for each thread
      for (thr = thread->begin (); thr != thread->end (); thr++) {
         (*thr)->read (*freq);
      }

      // (Fo)rest for a while...
      sleepForAWhile (DecisionMaker::IPC_EVAL_TIME);
      
      // Read all values for each thread
      for (thr = thread->begin (); thr != thread->end (); thr++) {
         (*thr)->read (*freq);
      }

      // Compute IPCs
      for (thr = thread->begin (); thr != thread->end (); thr++) {
         if (!(*thr)->computeIPC (*freq)) {
            hwcPanic = true; // There were a problem computing ipc, restarting
            break;
         }
      }
      if (!hwcPanic) {
         freq++;
      }
   }
   
   // Compute usage of each thread
   for (thr = thread->begin (); thr != thread->end (); thr++) {
      (*thr)->computeUsage ();
   }
  
   // Evaluate time spent in this evaluation step
   this->timeProfiler.evaluate (FREQUENCY_EVALUATION);

}

void DecisionMaker::computeSequence () 
{
   bool logFrequency = false;
   unsigned int maxRatioFreqId = 0;
   float bestE = std::numeric_limits<float>::max ();
   float bestD = 0;
   FreqChunkCouple bestCouple = {{{0, 0}, {0, 0}}};

   // active cpus
   for (std::vector<Thread*>::iterator thr = thread->begin ();
        thr != thread->end ();
        thr++)
   {
      if ((*thr)->isActive (ACTIVITY_LEVEL))
      {
         this->activeThread.insert (*thr);
      }
   }
   this->activeCores.clear ();
   Topology::threadIdsToCoreIds (this->activeThread, this->activeCores);

   // test all perfmormance level by steps of 1%
   for (float d = 1; d >= USER_PERF_REQ; d -= 0.01)
   {
      FreqChunkCouple couple;
      float coupleE;

      couple = getBestCouple (d, &coupleE);

      //DLOG (INFO) << "IPC: " << targetIPC
      //   << " couple: ((" << couple.step [STEP1].freqId << "," << couple.step [STEP1].timeRatio << "),(" << couple.step [STEP2].freqId << "," << couple.step [STEP2].timeRatio 
      //   << ")) energy: " << coupleE << std::endl;

      if (coupleE < bestE)
      {
         bestCouple = couple;
         bestE = coupleE;
         bestD = d;
      }
   }

   DLOG (INFO) << "d: " << bestD
      << " couple: ((" << bestCouple.step [STEP1].freqId << "," << bestCouple.step [STEP1].timeRatio << "),(" << bestCouple.step [STEP2].freqId << "," << bestCouple.step [STEP2].timeRatio 
      << ")) energy: " << bestE << std::endl;

   // Set the sequence to the best couple found
   this->sequence = bestCouple;

   // remember this for freq window update
   this->lastSequence = bestCouple;

   // Promote the frequencies
   this->freqSelector.promote (bestCouple.step [STEP1].freqId,
                               bestCouple.step [STEP1].timeRatio);
   this->freqSelector.promote (bestCouple.step [STEP2].freqId,
                               bestCouple.step [STEP2].timeRatio);

   // Set the dominant frequency in the couple (e.g the one having the maximum
   // time ratio)
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
		this->totalSleepWin = rest_min (DecisionMaker::MAX_SLEEP_WIN, this->totalSleepWin * 2);
	}
   else
   {
		unsigned int minFreqWindow = rest_max ((int) this->oldMaxFreqId -
                      (const int) DecisionMaker::STABILITY_WINDOW_SZ, 0);
		unsigned int maxFreqWindow =
         this->oldMaxFreqId + DecisionMaker::STABILITY_WINDOW_SZ;
		
      // If dominant freq changes by a step of +/- STABILITY_WINDOW_SZ
      // we still consider it as stable and we do not reset the totalSleepWin
		if (maxRatioFreqId < minFreqWindow || maxRatioFreqId > maxFreqWindow)
      {
			this->totalSleepWin = DecisionMaker::MIN_SLEEP_WIN;
		}
	}

   // Log the new frequency if necessary
	if (logFrequency) {
	 this->logFrequency (maxRatioFreqId);	
	}

	this->oldMaxFreqId = maxRatioFreqId;

   this->timeProfiler.evaluate (SEQUENCE_COMPUTATION);
}

void DecisionMaker::executeSequence ()
{
   // Apply steps
   for (unsigned int i = 0; i < 2; i++) {
      unsigned int freqId = this->sequence.step [i].freqId;
      this->unit.setFrequency (freqId);
   
      // Apply it for some time...
      unsigned int sleepWin = this->sequence.step [i].timeRatio
                        * this->totalSleepWin;
      sleepWin *= 1000; // Because we're sleeping in nanoseconds
      
      // Sleep now !
      sleepForAWhile (sleepWin);
   }
   // Profiler, remind we leave execution
   this->timeProfiler.evaluate (EXECUTION_SLOT);
}

} //namespace FoREST

