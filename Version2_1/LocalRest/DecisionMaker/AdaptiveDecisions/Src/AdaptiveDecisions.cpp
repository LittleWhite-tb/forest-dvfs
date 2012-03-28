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
 @file AdaptiveDecisions.cpp
 @brief The AdaptiveDecisions class is in this file
 */

#include <iostream>

#include "AdaptiveDecisions.h"
#include "Common.h"

AdaptiveDecisions::AdaptiveDecisions (DVFSUnit & unit) :
   DecisionMaker (unit)
{
   Decision defDec = this->defaultDecision ();

   // initialize internal data
   this->freqSel = new FreqSelector(unit.getNbFreqs ());
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

   return res;
}

Decision AdaptiveDecisions::takeDecision (const HWCounters & hwc)
{
   Decision res;

   unsigned int curSleepWin = this->formerSleepWin;
   unsigned int curFreqId = this->formerFreqId;

   float HWexp = this->getHWExploitationRatio (hwc);
   bool inEvalStep;

   // were we in the evaluation step
   inEvalStep = curSleepWin == AdaptiveDecisions::IPC_EVAL_TIME;

   if (inEvalStep)
   {
      // remember the IPC for this frequency
      this->ipcEval [curFreqId] = HWexp;

      // we have other frequencies to evaluate
      if (curFreqId != this->unit.getNbFreqs() - 1)
      {
         res.freqId = curFreqId + 1;
         res.sleepWin = AdaptiveDecisions::IPC_EVAL_TIME;
      }
      // this was the last evaluation
      else
      {
         // promote all frequencies at 10% of the top IPC
         float maxIPC = 0;
         for (unsigned int i = 0; i < this->unit.getNbFreqs(); i++)
         {
            maxIPC = rest_max (maxIPC, this->ipcEval [i]);
         }

         for (int i = this->unit.getNbFreqs() - 1; i >= 0; i--)
         {
            if (this->ipcEval [i] > 0.9 * maxIPC)
            {
               this->freqSel->promoteFrequency (i);
            }
         }

         // select the frequency and run it for a while
         res.freqId = this->freqSel->getBestFrequency ();

         // increase the sleeping window if we were previously using this freq
         if (this->formerExecFreqId == res.freqId)
         {
            res.sleepWin = rest_max (AdaptiveDecisions::MAX_SLEEP_WIN, this->formerExecSleepWin * 2);
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
      res.freqId = 0;
      res.sleepWin = AdaptiveDecisions::IPC_EVAL_TIME;
   }                        

   // remember some stuff to take better decisions afterwards
   this->formerFreqId = res.freqId;
   this->formerSleepWin = res.sleepWin;

   if (res.sleepWin != AdaptiveDecisions::IPC_EVAL_TIME)
   {
      this->formerExecFreqId = res.freqId;
      this->formerSleepWin = res.sleepWin;
   }

   return res;
}

