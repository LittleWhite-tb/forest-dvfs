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
 * @file AdaptiveDecisions.cpp
 * The AdaptiveDecisions class is in this file
 */

#include <iostream>

#include "AdaptiveDecisions.h"
#include "Common.h"

AdaptiveDecisions::AdaptiveDecisions (DVFSUnit & unit) :
   DecisionMaker (unit)
{
   Decision defDec = this->defaultDecision ();

   // initialize internal data
   this->freqSel = new FreqSelector (unit.getNbFreqs ());
   this->ipcEval = new float [this->unit.getNbFreqs ()];

   this->formerSleepWin = defDec.sleepWin;
   this->formerFreqId = defDec.freqId;
   this->formerExecSleepWin = defDec.sleepWin;
   this->formerExecFreqId = defDec.freqId;
}

AdaptiveDecisions::~AdaptiveDecisions (void)
{
   delete this->freqSel;
   delete [] this->ipcEval;
}

Decision AdaptiveDecisions::defaultDecision ()
{
   Decision res;

   res.sleepWin = AdaptiveDecisions::MIN_SLEEP_WIN;
   res.freqId = 0;
   res.preCntResetPause = 0;

   return res;
}

Decision AdaptiveDecisions::takeDecision (const HWCounters & hwc)
{
	Decision dec = this->takeDecision (hwc, false);
	return dec;
}

Decision AdaptiveDecisions::takeDecision (const HWCounters & hwc, bool delayedStart)
{
   Decision res;
   if(delayedStart == true)
   {
      res = this->defaultDecision();
      return res;
   }
   unsigned int curSleepWin = this->formerSleepWin;
   unsigned int curFreqId = this->formerFreqId;

   float HWexp = this->getHWExploitationRatio (hwc);
   bool inEvalStep;

   //define the freq to test
   unsigned int minEvalFreqId = rest_max (0,
                                          (int) this->formerExecFreqId - (int) AdaptiveDecisions::NB_EVAL_NEAR_FREQS);
   unsigned int maxEvalFreqId = rest_min (this->unit.getNbFreqs () - 1,
                                          this->formerExecFreqId + AdaptiveDecisions::NB_EVAL_NEAR_FREQS);

   // were we in the evaluation step
   // 1 if the sleeping window = 20 (we test freq) 0 elsewhere.
   inEvalStep = curSleepWin == AdaptiveDecisions::IPC_EVAL_TIME;

   if (inEvalStep)
   {
      // remember the IPC for this frequency
      this->ipcEval [curFreqId] = HWexp;

      // we have other frequencies to evaluate
      if (curFreqId != maxEvalFreqId)
      {
         res.freqId = curFreqId + 1;
         res.sleepWin = AdaptiveDecisions::IPC_EVAL_TIME;
      }
      // this was the last evaluation
      else
      {
         // DEBUG
         if (AdaptiveDecisions::VERBOSE && this->unit.getOSId () == 0)
         {
            std::cout << "Evaluation result [" << minEvalFreqId << '-' << maxEvalFreqId << "]: ";
            for (unsigned int i = minEvalFreqId; i <= maxEvalFreqId; i++)
            {
               std::cout << this->ipcEval [i] << " ";
            }
            std::cout << std::endl;
         }

         // promote all frequencies at 10% of the top IPC
         float maxIPC = 0;
         for (unsigned int i = minEvalFreqId; i <= maxEvalFreqId; i++)
         {
            maxIPC = rest_max (maxIPC, this->ipcEval [i]);
         }

         for (unsigned int i = minEvalFreqId; i <= maxEvalFreqId; i++)
         {
            if (this->ipcEval [i] > 0.9 * maxIPC)
            {
               this->freqSel->promoteFrequency (i);
            }
         }

         // select the frequency and run it for a while
         res.freqId = this->freqSel->getBestFrequency ();

         if (AdaptiveDecisions::VERBOSE && this->unit.getOSId () == 0)
         {
            std::cout << "Elected freq: " << res.freqId << std::endl;
         }

         // increase the sleeping window if we were previously using this freq
         if (this->formerExecFreqId == res.freqId)
         {
            res.sleepWin = rest_min (AdaptiveDecisions::MAX_SLEEP_WIN, this->formerExecSleepWin * 2);
         }
         else
         {
            res.sleepWin = AdaptiveDecisions::MIN_SLEEP_WIN;
         }
      }
   }
   // enterring the evaluation step
   else
   {
      res.freqId = minEvalFreqId;
      res.sleepWin = AdaptiveDecisions::IPC_EVAL_TIME;

      if (AdaptiveDecisions::VERBOSE && this->unit.getOSId () == 0)
      {
         std::cout << "boundness: " << (hwc.l2miss * 1.) / hwc.cycles << std::endl;
      }
   }

   // remember some stuff to take better decisions afterwards
   this->formerFreqId = res.freqId;
   this->formerSleepWin = res.sleepWin;

   if (res.sleepWin != AdaptiveDecisions::IPC_EVAL_TIME)
   {
#ifdef REST_EXTRA_LOG
      if (res.freqId != this->formerExecFreqId)
      {
         struct timespec ts;
         clock_gettime (CLOCK_MONOTONIC, &ts);
         this->switchOFS << ts.tv_nsec + ts.tv_sec * 1000000000 << " " << res.freqId << std::endl;
      }
#endif

      // we don't care about when the frequency will be set
      res.preCntResetPause = 0;

      // remember what we have decided
      this->formerExecFreqId = res.freqId;
      this->formerExecSleepWin = res.sleepWin;
   }
   else
   {
      // we are in an evaluation step, wait before reseting the counters: the
      // latency has to be actually set before we can measure anything relevant
      res.preCntResetPause = this->unit.getSwitchLatency () / 1000;
   }

   return res;
}

