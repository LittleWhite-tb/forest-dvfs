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


#include "BetaAdaptiveDecisions.h"
#include "Common.h"

#ifdef REST_EXTRA_LOG
#include <sstream>
#include <vector>
#include "Logger.h"
#endif

BetaAdaptiveDecisions::BetaAdaptiveDecisions (DVFSUnit & unit) :
   DecisionMaker (unit)
{
   Decision defDec = this->defaultDecision ();

   // initialize internal data
   this->mipsEval = new float [this->unit.getNbFreqs ()];
   for (unsigned int i = 0; i< this->unit.getNbFreqs (); i++)
   {
      this->mipsEval [i] = -1;
   }

   this->formerSleepWin = defDec.sleepWin;
   this->formerFreqId = defDec.freqId;
   this->curState = BetaAdaptiveDecisions::WAITING_START;
}

BetaAdaptiveDecisions::~BetaAdaptiveDecisions (void)
{
   delete [] this->mipsEval;
}

Decision BetaAdaptiveDecisions::defaultDecision ()
{
   Decision res;

   res.sleepWin = BetaAdaptiveDecisions::STATIC_EVAL_TIME;
   res.freqId = 0;
   res.preCntResetPause = 0;

   return res;
}

Decision BetaAdaptiveDecisions::takeDecision (const HWCounters & hwc)
{
   Decision dec = this->takeDecision (hwc, false);
   return dec;
}

void BetaAdaptiveDecisions::resetBetaState(Decision &res)
{
   unsigned int minFreq = this->unit.getFrequency (0);
   unsigned int maxFreqId = this->unit.getNbFreqs () - 1;
   unsigned int maxFreq = this->unit.getFrequency (maxFreqId);

   //Compute Beta
   double tmpNumerator = 0;
   double tmpDenominator = 0;

   for (unsigned int i = 0; i < this->unit.getNbFreqs (); i++)
   {
      unsigned int iFreq = this->unit.getFrequency (i);
      tmpNumerator += ((maxFreq / (1.0 * iFreq)) - 1) * ((this->mipsEval [maxFreqId] / (1.0 * this->mipsEval [i])) - 1);
      tmpDenominator += ((maxFreq / (1.0 * iFreq)) - 1) * ((maxFreq / (1.0 * iFreq)) - 1);
   }
   double betaCoef = tmpNumerator / tmpDenominator;

   //Find the virtual freq
   unsigned int maxVirtFreq = maxFreq / (1 + (0.05 / betaCoef));
   unsigned int virualFreq = rest_max (minFreq, maxVirtFreq);

   //select the real adjacent freq of the virutal one
   int adjacentHighFreqId = maxFreqId;
   int adjacentLowFreqId = -1;
   for (unsigned int i = 0; i < this->unit.getNbFreqs (); i++)
   {
      if (this->unit.getFrequency (i) > virualFreq)
      {
         adjacentHighFreqId = i;
         break;
      }
   }
   adjacentLowFreqId = rest_max (0, adjacentHighFreqId - 1);

   //compute the runing time for each
   double rFactor = ((1. + (0.05 / betaCoef)) / maxFreq) - (1. / this->unit.getFrequency (adjacentHighFreqId));
   rFactor /= (1. / this->unit.getFrequency (adjacentLowFreqId)) - (1. / this->unit.getFrequency (adjacentHighFreqId));
   rFactor = rest_max(0, rFactor);
   rFactor = rest_min(1, rFactor);

   this->freqIdStp2 = adjacentHighFreqId;
   this->sleepWinStp2 = (1 - rFactor) * BetaAdaptiveDecisions::STATIC_EVAL_TIME;

   res.freqId = adjacentLowFreqId;
   res.sleepWin = rFactor * BetaAdaptiveDecisions::STATIC_EVAL_TIME;
   res.preCntResetPause = this->unit.getSwitchLatency () / 1000;

   this->curState = BetaAdaptiveDecisions::RUNNING_STP1;
}

Decision BetaAdaptiveDecisions::takeDecision (const HWCounters & hwc, bool delayedStart)
{
   Decision res = this->defaultDecision ();

   if (delayedStart)
   {

#ifdef REST_EXTRA_LOG
      Logger & log = Logger::getLog (this->unit.getOSId ());
      std::stringstream ss (std::stringstream::out);
      ss << "[BetaAdaptiveDecisions::takeDecision] delayed ...";
      log.logOut (ss);
#endif

      return res;
   }

   unsigned int curFreqId = this->formerFreqId;
   unsigned int maxFreqId = this->unit.getNbFreqs () - 1;

   float Mips = this->getMipsRatio (hwc);

   //we are in the mips evaluation
   if (this->curState == BetaAdaptiveDecisions::WAITING_START)
   {
      this->curState = BetaAdaptiveDecisions::MIPS_EVALUATION;

      res.freqId = 0;
      res.sleepWin = BetaAdaptiveDecisions::STATIC_EVAL_TIME;
      res.preCntResetPause = this->unit.getSwitchLatency () / 1000;
   }
   else if (this->curState == BetaAdaptiveDecisions::MIPS_EVALUATION)
   {
      // remember the IPC for this frequency
      this->mipsEval [curFreqId] = Mips;

      // we have other frequencies to evaluate
      if (curFreqId != maxFreqId)
      {
         res.freqId = curFreqId + 1;
         res.sleepWin = BetaAdaptiveDecisions::STATIC_EVAL_TIME;
         res.preCntResetPause = this->unit.getSwitchLatency () / 1000;
      }
      else //we have the mips for all freq
      {
         this->resetBetaState (res);
      }
   }
   else if (this->curState == BetaAdaptiveDecisions::RUNNING_STP1)
   {
      this->mipsEval [this->formerFreqId] = Mips;

      res.freqId = this->freqIdStp2;
      res.sleepWin = this->sleepWinStp2;
      res.preCntResetPause = this->unit.getSwitchLatency () / 1000;

      this->curState = BetaAdaptiveDecisions::RUNNING_STP2;
   }
   else if (this->curState == BetaAdaptiveDecisions::RUNNING_STP2)
   {
      this->mipsEval [this->formerFreqId] = Mips;
      
      this->resetBetaState (res);
   }

   // remember some stuff to take better decisions afterwards
   this->formerFreqId = res.freqId;
   this->formerSleepWin = res.sleepWin;

   return res;
}

