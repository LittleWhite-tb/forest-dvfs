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

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <math.h>
#include <vector>
#include <stdint.h>
#include <cassert>
#include <limits>

#include "DecisionMaker.h"
#include "Common.h"
#include "Topology.h"
#include "FreqSelector.h"
#include "Thread.h"
#include "Logger.h"

#ifdef REST_LOG
#include "Logger.h"
#include <iomanip>
#endif

namespace FoREST {

DecisionMaker::DecisionMaker (DVFSUnit *dvfsUnit, const Mode mode,
                              Config *cfg, std::vector<Thread*>& thr) :
   newEval (1),
   freqWindowCenter (0),
   addedFreqMax (false),
   unit (dvfsUnit),
   thread (thr),
   referenceL3misses (thr.size ()),
   IPC_EVAL_TIME(cfg->getInt("IPC_EVALUATION_TIME")),
   MIN_SLEEP_WIN(cfg->getInt("MIN_SLEEP_WIN")),
   MAX_SLEEP_WIN(cfg->getInt("MAX_SLEEP_WIN")),
   FREQ_WINDOW_SZ(cfg->getInt("FREQ_WINDOW_SZ")),
   STABILITY_WINDOW_SZ(cfg->getInt("STABILITY_WINDOW_SZ")),
   SYS_POWER(cfg->getFloat("SYS_POWER")),
   USER_PERF_REQ_PERF(cfg->getFloat("USER_PERF_REQ_PERF")),
   USER_PERF_REQ_ENERGY(cfg->getFloat("USER_PERF_REQ_ENERGY")),
   USER_PERF_REQ ((mode == MODE_PERFORMANCE ? USER_PERF_REQ_PERF : USER_PERF_REQ_ENERGY)),
   ACTIVITY_LEVEL(cfg->getFloat("ACTIVITY_LEVEL")),
   timeProfiler (),
   nbFreqs (dvfsUnit->getNbFreqs ()),
   oldMaxFreqId (0),
   totalSleepWin (DecisionMaker::MIN_SLEEP_WIN),
   freqSelector (dvfsUnit->getNbFreqs ()),
   skipSequenceComputation (false) 
{
   // setup an initial state for the decision
   lastSequence.step [STEP1].freqId = 0;
   lastSequence.step [STEP1].timeRatio = 0;
   lastSequence.step [STEP2].freqId = 0;
   lastSequence.step [STEP2].timeRatio = 0;
}

DecisionMaker::~DecisionMaker (void)
{}

bool DecisionMaker::getBestCouple (float d, FreqChunkCouple *bestCouple, float *coupleEnergy, bool *isSpecialCase)
{
   std::set<unsigned int> smallerIpc;
   std::set<unsigned int> greaterIpc;

   *isSpecialCase = false;
   unsigned int nbActiveCores = this->activeCores.size ();

   // The end frequency in the set of frequencies we have evaluated
   std::set<unsigned int>::iterator endFreq = this->freqsToEvaluate.end ();
   // The last frequency in the set of frequencies we have evaluated
   std::set<unsigned int>::iterator highestFreq = --this->freqsToEvaluate.end ();
   //std::set<unsigned int>::iterator realHighestFreq = highestFreq;
   //bool lowerHighestFreq = false, higherHighestFreq = false;
   
   // If the maximum frequency has been added only to have 
   // a reference frequency, then we don't take it as a possible
   // frequency to be applied at this iteration
   //
   // This avoids frequency to go suddenly up to max frequency and rather
   // goes up step by step (as fast as it goes down)
   //
   // If this statement is deactivated, it can cause higher energy consumption
   // because of volative high ipc values
   /*if (this->addedFreqMax) {
      --endFreq;
      --highestFreq;
   }*/

   // split frequencies depending on their IPCs
   for (std::set<unsigned int>::iterator freq = this->freqsToEvaluate.begin ();
        freq != this->freqsToEvaluate.end ();
        freq++)
   {
      bool isLower = true, isHigher = true;
      DLOG(INFO) << "Freq " << *freq << std::endl;

      // For each active thread
      for (std::set<Thread*>::iterator thread = this->activeThread.begin ();
           thread != this->activeThread.end ();
           thread++)
      {
         // Get the IPC at the requested freauency
         float threadIpc = (*thread)->getIPC (*freq);
         float maxIpc = (*thread)->getMaxIPC ();

         DLOG(INFO) << "Thread " << (*thread)->getId() << " ipc: " << threadIpc << " max: " << maxIpc <<  " degraded = " << maxIpc*d << std::endl;
         
         if (threadIpc < d * maxIpc)
         {
            isHigher = false;
         }
         else
         {
            isLower = false;
         }
      }

      /*if (realHighestFreq == freq) {
         if (isLower) lowerHighestFreq = true;
         if (isHigher) higherHighestFreq = true;
      } else {*/
         if (isLower)
         {
            smallerIpc.insert (*freq);
         }
         else if (isHigher)
         {
            greaterIpc.insert (*freq);
         }
      //}
   }

   // precompute t_i/t_ref * W_i/W_ref for every frequency i
   std::vector<float> e_ratios (this->nbFreqs);
   std::set<unsigned int>::iterator smallestFrequency = this->freqsToEvaluate.begin ();
   std::set<unsigned int>::iterator highestFrequency = --this->freqsToEvaluate.end ();
   const float Pref = this->unit->getPowerAt (*highestFrequency, nbActiveCores);
   const float Psys = DecisionMaker::SYS_POWER;
   e_ratios [*highestFrequency] = nbActiveCores;

   // Evalute energy ratios for each frequency except the highest one (which is the reference one)
   for (std::set<unsigned int>::iterator freq = this->freqsToEvaluate.begin ();
        freq != highestFrequency;
        freq++)
   {
      DLOG (INFO) << "computing ratio for freq " << *freq << std::endl;
      float Pi = this->unit->getPowerAt (*freq, nbActiveCores);

      e_ratios [*freq] = 0;

      for (std::set<Thread*>::iterator thr = this->activeThread.begin ();
           thr != this->activeThread.end ();
           thr++)
      {
         float IPCref = (*thr)->getIPC (*highestFrequency);
         float IPCi = (*thr)->getIPC (*freq);
         
         e_ratios [*freq] += (IPCref / IPCi) * ((Pi + Psys) / (Pref + Psys));
      }
   }

   // only greater freqs? IPC is ensured, whatever we choose, go to the smallest known frequency
   if (smallerIpc.size () == 0) {
      DLOG (INFO) << "special case : no smaller ipc" << std::endl;
      FreqChunkCouple couple = {{
         {*smallestFrequency, 1},
         {0, 0}
      }};
      *bestCouple = couple;
      if (coupleEnergy != NULL) {
         *coupleEnergy = e_ratios [*smallestFrequency];
      }

      return false;
   }
 
   // note: case with only smaller freqs is only due to mistakes in measurements
   // for safety we go to maximal freq here
   if (greaterIpc.size () == 0) {
      DLOG (INFO) << "special case : no greater ipc" << std::endl; 
      FreqChunkCouple couple = {{
         {*highestFreq, 1},
         {0, 0}
      }};
      *bestCouple = couple;
      if (coupleEnergy != NULL) {
         *coupleEnergy = e_ratios [*highestFreq];
      }
      *isSpecialCase = true;
      return true;
   }

   /*if (higherHighestFreq) {
      greaterIpc.insert (*realHighestFreq);
   } else if (lowerHighestFreq) {
      smallerIpc.insert (*realHighestFreq);
   }*/

   // we want min (r_i*e_i/e_ref+r_j*e_j/e_ref) among all couples (f_i, f_j)
   // with r_i and r_j the time ratios for f_i and f_j in the couple
   // and with e_i/e_ref = IPC_ref/IPC_i * W_i/W_ref
   // (as IPC_ref/IPC_i = CPI_i/CPI_ref and CPI_i/CPI_ref = t_i/t_ref)

   FreqChunkCouple bestC = {{{0, 0}, {0, 0}}};
   float bestCoupleE = std::numeric_limits<float>::max ();

   for (std::set<unsigned int>::iterator sit = smallerIpc.begin ();
        sit != smallerIpc.end ();
        sit++)
   {
      for (std::set<unsigned int>::iterator git = greaterIpc.begin ();
           git != greaterIpc.end ();
           git++)
      {
         FreqChunk step1, step2;

         unsigned int smaller = (*sit);
         unsigned int greater = (*git);

         DLOG (INFO) << "OK, comparing smaller (" << smaller << ") and greater (" << greater << ")" << std::endl;
         
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
            DLOG (INFO) << "greaterIpc = " << greaterIpc
                      << " smallerIpc = " << smallerIpc << std::endl;
            minRatio = rest_min (minRatio, 
                  (greaterIpc - d * maxIpc)
               / (greaterIpc - smallerIpc));
            DLOG (INFO) << "minRatio = " << minRatio << ", greater - smaller = " << (greaterIpc - smallerIpc) << std::endl;
         }

         step1.timeRatio = minRatio;
         step2.timeRatio = 1 - step1.timeRatio;

         DLOG (INFO) << "step1.tr = " << minRatio <<
                      "step2.tr = " << step2.timeRatio << std::endl;

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
            bestC = couple;
            bestCoupleE = coupleE;
         }
      }
   }

   assert (bestCoupleE < std::numeric_limits<float>::max ()); 

   if (coupleEnergy != NULL)
   {
      *coupleEnergy = bestCoupleE;
   }
   *bestCouple = bestC;

   return true;
}


void DecisionMaker::logFrequency (unsigned int freqId) const
{	
#ifdef REST_LOG
   assert (freqId < this->nbFreqs);
   Logger *log = Logger::getLog (this->unit->getId ());
   std::ostringstream logger;
   logger << freqId;
   log->logOut (logger);
#endif
}

void DecisionMaker::initEvaluation ()
{
   this->addedFreqMax = false;
   std::vector<Thread*>::iterator thr;

   // Reset the list of frequencies to evaluate
   this->freqsToEvaluate.clear ();

   // Computing the new freq window
   if (this->lastSequence.step [STEP1].timeRatio
       > this->lastSequence.step [STEP2].timeRatio)
   {
      this->freqWindowCenter = this->lastSequence.step [STEP1].freqId;
   }
   else 
   {
      this->freqWindowCenter = this->lastSequence.step [STEP2].freqId;
   }

   // request evaluation of all close frequencies
   unsigned int minFreqId = rest_max (0, 
      (int) this->freqWindowCenter - (int) DecisionMaker::FREQ_WINDOW_SZ);
   unsigned int maxFreqId = rest_min (this->nbFreqs - 1,
      this->freqWindowCenter + DecisionMaker::FREQ_WINDOW_SZ);

   for (unsigned int i = minFreqId; i <= maxFreqId; i++)
   { 
      this->freqsToEvaluate.insert (i);
   }

   // Consensus decision
   //this->freqsToEvaluate.insert ( (this->nbFreqs - 1 + this->freqWindowCenter) / 2);

   // also request evaluation for the maximal frequency as it must serve as a
   // reference point to compute the degradation rate
   if (maxFreqId < this->nbFreqs - 1)
   {
      this->freqsToEvaluate.insert (this->nbFreqs - 1);
      this->addedFreqMax = true;
   }

   /*std::cerr << "Evaluating frequencies: ";
   for (std::set<unsigned int>::iterator it = this->freqsToEvaluate.begin ();
        it != this->freqsToEvaluate.end ();
        it++) {
      std::cerr << *it << " ";
   }
   std::cerr << std::endl;*/

   // Reset the IPC for each thread
   for (thr = thread.begin (); thr != thread.end (); thr++) {
      (*thr)->resetIPC ();
   }

   // time the evaluation for debuging purposes
   //this->timeProfiler.evaluate (EVALUATION_INIT); 
}

void DecisionMaker::evaluateFrequency () {

   std::vector<Thread*>::iterator thr;

   // For each frequency in our window
   std::set<unsigned int>::iterator freq = freqsToEvaluate.begin ();
   while (freq != freqsToEvaluate.end ()) {    
      bool hwcPanic = false;
      
      // Apply the frequency
      this->unit->setFrequency (*freq);

      // Wait for the frequency to be applied
      nsleep (this->unit->getSwitchLatency ());

      // Reset all values for each thread
      for (thr = thread.begin (); thr != thread.end (); thr++) {
         (*thr)->reset (*freq);
      }

      // (Fo)rest for a while...
      nsleep (DecisionMaker::IPC_EVAL_TIME*1000);
      
      // Read all values for each thread
      for (thr = thread.begin (); thr != thread.end (); thr++) {
         (*thr)->read (*freq);
      }

      // Compute IPCs 
      for (thr = thread.begin (); thr != thread.end (); thr++) {
         hwcPanic = (*thr)->computeIPC (*freq);

        // If something went wrong, no need to go further ; restart the
        // whole evaluation for this frequency
         if (hwcPanic) {
            break;
         }
      }
      if (!hwcPanic) {
         freq++;
      }
   }

   // Debug information
   for (thr = thread.begin (); thr != thread.end (); thr++) {
      (*thr)->smoothIPC ();
      //std::cerr << "Thread #" << (*thr)->getId () <<  " ";
      //(*thr)->printIPC ();
   } 

   // Updates the usage of each thread
   // NOTE: Not necessarily updates it every time, the object decides
   // internally whether or not it should proceed to the update
   for (thr = thread.begin (); thr != thread.end (); thr++) { 
      (*thr)->computeUsage ();
   } 
 
   // Evaluate time spent in this evaluation step
   //this->timeProfiler.evaluate (FREQUENCY_EVALUATION);

}

bool DecisionMaker::computeSequence () 
{
   bool logFrequency = false;
   unsigned int maxRatioFreqId = 0;
   float bestE = std::numeric_limits<float>::max ();
   float bestD = 0;
   FreqChunkCouple bestCouple = {{{0, 0}, {0, 0}}};
   bool isSpecial = false, isSpecialCase;

   // active cpus
   this->activeThread.clear ();
   for (std::vector<Thread*>::iterator thr = thread.begin ();
        thr != thread.end ();
        thr++)
   {
      if ((*thr)->isActive (ACTIVITY_LEVEL))
      {
         this->activeThread.insert (*thr);
      }
   }
   this->activeCores.clear ();
   Topology::threadIdsToCoreIds (this->activeThread, this->activeCores);

   // compute max IPC per thread
   for (std::vector<Thread*>::iterator it = this->thread.begin ();
        it != this->thread.end ();
        it++) {
      (*it)->computeMaxIPC ();
   }

   // If there is no active cores, we take the smaller frequency available
   if (activeThread.size () == 0) {
      FreqChunkCouple zero = {{{0,1},{0,0}}};
      bestCouple = zero;
      bestE = 0;
   } else {
      bool continueLoop = true;
      // else, we test all perfmormance level by steps of 1%
      for (float d = 1; d >= USER_PERF_REQ && continueLoop; d -= 0.01)
      {
         DLOG (INFO) << "Degradation = " << d << std::endl;
         FreqChunkCouple couple;
         float coupleE;
   
         // If getBestCouple returns false, it means we already took the smallest
         // decision with respect to the current degradation ratio
         // Therefore it is unnecessary to continue the loop as we will always take
         // the same decision from now on
         if (!getBestCouple (d, &couple, &coupleE, &isSpecialCase)) {
            continueLoop = false;
         }
   
         DLOG (INFO)
            << "couple: ((" << couple.step [STEP1].freqId << "," << couple.step [STEP1].timeRatio << "),(" << couple.step [STEP2].freqId << "," << couple.step [STEP2].timeRatio 
            << ")) energy: " << coupleE << std::endl;
   
         if (coupleE < bestE)
         {
            bestCouple = couple;
            bestE = coupleE;
            bestD = d;
            isSpecial = isSpecialCase;
         }
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
   
   DLOG (INFO) << "totalsleepwin = " << this->totalSleepWin << std::endl;
   DLOG (INFO) << "maxRatioFreqId = " << maxRatioFreqId << std::endl;
	this->oldMaxFreqId = maxRatioFreqId;

   //this->timeProfiler.evaluate (SEQUENCE_COMPUTATION);
   
   return isSpecial;
}

bool DecisionMaker::executeSequence ()
{
   std::vector<Thread*>::iterator thr;
   
   //std::cerr << "Execute Sequence" << std::endl;

   // Resets the read of the ipc
   for (thr = thread.begin (); thr != thread.end (); thr++) {
         (*thr)->resetExec ();
   }
   
   // Apply steps
   for (unsigned int i = 0; i < 2; i++) {
      if (this->sequence.step [i].timeRatio > 0) {
         unsigned int freqId = this->sequence.step [i].freqId;
         this->unit->setFrequency (freqId);
   
         // Apply it for some time...
         unsigned int sleepWin = this->sequence.step [i].timeRatio
                           * this->totalSleepWin;
     
         //std::cerr << "sleepWin = " << sleepWin << std::endl;
         // Sleep now !
         nsleep (sleepWin*1000);
      }
   }
 
   // Read all values for each thread
   for (thr = thread.begin (); thr != thread.end (); thr++) {
      (*thr)->readExec ();
      (*thr)->computeL3MissRatio ();
      //std::cerr << "thr #" << (*thr)->getId () << ": " << (*thr)->getL3missesExec () << std::endl;
   }

   // Compute IPCs
   std::vector<float>::iterator refL3 = this->referenceL3misses.begin ();
   for (thr = thread.begin (); thr != thread.end (); thr++) { 
      if (this->newEval > 0) {
         (*refL3) = (*thr)->getL3MissRatioExec ();
         refL3++;
      }
   }

   if (this->activeCores.size () == 0) {
      return 1;
   }

   if (this->newEval == 0) {
      //std::cerr << "stable" << std::endl;
      // Check if it's stable
      std::vector<float>::iterator refL3 = this->referenceL3misses.begin ();
      this->totalSleepWin = rest_min (this->totalSleepWin*2, DecisionMaker::MAX_SLEEP_WIN);
      for (thr = thread.begin (); thr != thread.end (); thr++) {
         float min = rest_max (0, (*refL3) - 0.20);
         float max = (*refL3) + 0.20;
         float current = (*thr)->getL3MissRatioExec ();
         //std::cerr << "freq = " << this->oldMaxFreqId << " current = " << current << ", min = " << min << ", max = " << max << std::endl;
         if (current > 0.3 && (current < min
             || current > max)) {
            this->newEval = 1;
            //std::cerr << "BREAK" << std::endl;
            //this->totalSleepWin = DecisionMaker::MIN_SLEEP_WIN;
            break;
         }
         refL3++;
      }
   } else {
      //std::cerr << "newEval close to stable = " << this->newEval << std::endl;
      this->newEval--;
   }

   // Profiler, remind we leave execution
   //this->timeProfiler.evaluate (EXECUTION_SLOT);
   
   return this->newEval == 0;
}

} //namespace FoREST

