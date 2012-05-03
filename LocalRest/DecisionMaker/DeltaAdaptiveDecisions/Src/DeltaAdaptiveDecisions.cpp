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


#include "DeltaAdaptiveDecisions.h"
#include "Common.h"

#ifdef REST_EXTRA_LOG
#include <sstream>
#include <vector>
#include "Logger.h"
#endif

DeltaAdaptiveDecisions::DeltaAdaptiveDecisions (DVFSUnit & unit) :
   DecisionMaker (unit)
{
   Decision defDec = this->defaultDecision ();

   // initialize internal data
   this->mipsEval = new float [this->unit.getNbFreqs ()];
   this->ipcEval = new float [this->unit.getNbFreqs ()];
   
   for (unsigned int i = 0; i< this->unit.getNbFreqs (); i++)
   {
      this->mipsEval [i] = -1;
      this->ipcEval [i] = -1;
   }

   this->formerSleepWin = defDec.sleepWin;
   this->formerFreqId = defDec.freqId;
   this->curState = DeltaAdaptiveDecisions::WAITING_START;
}

DeltaAdaptiveDecisions::~DeltaAdaptiveDecisions (void)
{
   delete [] this->mipsEval;
   delete [] this->ipcEval;
}

Decision DeltaAdaptiveDecisions::defaultDecision ()
{
   Decision res;

   res.sleepWin = DeltaAdaptiveDecisions::STATIC_EVAL_TIME;
   res.freqId = 0;
   res.preCntResetPause = 0;

   return res;
}

Decision DeltaAdaptiveDecisions::takeDecision (const HWCounters & hwc)
{
   Decision dec = this->takeDecision (hwc, false);
   return dec;
}

void DeltaAdaptiveDecisions::decStrategy1 (Decision &res, int deltaDegradation)
{
	unsigned int maxFreqId = this->unit.getNbFreqs () - 1;
	
	
	float maxIPC = 0;
	int adjacentHighFreqId = -1;
	float minIPC = 10;
	int adjacentLowFreqId = -1;
	for (unsigned int i = 0; i < maxFreqId; i++)
	{
		if(maxIPC < this->ipcEval [i])
		{
			maxIPC = this->ipcEval [i];
			adjacentHighFreqId = i;
		}
		if(minIPC > this->ipcEval [i])
		{
			minIPC = this->ipcEval [i];
			adjacentLowFreqId = i;
		}
	}
	float degradedIPC = ((100 - deltaDegradation)/100.0) * maxIPC;
	double rFactor = (degradedIPC - maxIPC) / (minIPC - maxIPC);
	
	
	this->freqIdStp2 = adjacentHighFreqId;
	this->sleepWinStp2 = (1 - rFactor) * DeltaAdaptiveDecisions::STATIC_EVAL_TIME;

	res.freqId = adjacentLowFreqId;
	res.sleepWin = rFactor * DeltaAdaptiveDecisions::STATIC_EVAL_TIME;
	res.preCntResetPause = this->unit.getSwitchLatency () / 1000;

#ifdef REST_EXTRA_LOG
	/*std::stringstream ss (std::stringstream::out);
	ss << "maxIPC [" << maxIPC << "] associated freq [" << adjacentHighFreqId << "]";
	ss << " minIPC [" << minIPC << "] associated freq [" << adjacentLowFreqId << "]";
	ss << " degradedIPC [" << degradedIPC << "]";
	ss << " rfactor [" << rFactor << "]";
	ss << " adjacentHighFreqId  [" << adjacentHighFreqId << "] adjacentLowFreqId [" << adjacentLowFreqId << "]" << std::endl;
	Logger &log = Logger::getLog(this->unit.getOSId ());
	log.logOut(ss);*/
#endif
	this->curState = DeltaAdaptiveDecisions::RUNNING_STP1;
	
}


void DeltaAdaptiveDecisions::decStrategy2 (Decision &res, int deltaDegradation)
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
   unsigned int maxVirtFreq = maxFreq / (1 + (deltaDegradation / betaCoef));
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
   double rFactor = ((1. + (deltaDegradation / betaCoef)) / maxFreq) - (1. / this->unit.getFrequency (adjacentHighFreqId));
   rFactor /= (1. / this->unit.getFrequency (adjacentLowFreqId)) - (1. / this->unit.getFrequency (adjacentHighFreqId));
   rFactor = rest_max(0, rFactor);
   rFactor = rest_min(1, rFactor);

   this->freqIdStp2 = adjacentHighFreqId;
   this->sleepWinStp2 = (1 - rFactor) * DeltaAdaptiveDecisions::STATIC_EVAL_TIME;

   res.freqId = adjacentLowFreqId;
   res.sleepWin = rFactor * DeltaAdaptiveDecisions::STATIC_EVAL_TIME;
   res.preCntResetPause = this->unit.getSwitchLatency () / 1000;

   this->curState = DeltaAdaptiveDecisions::RUNNING_STP1;
}

void DeltaAdaptiveDecisions::resetBetaState(Decision &res)
{
  //The degradation factor
   int deltaDegradation = 10;
   
   //the Strategy to use
   int decstrat = 2;
      
	switch (decstrat)
	{
		case 1:
			this->decStrategy1(res,deltaDegradation); 
			break;
		case 2: 
			this->decStrategy2(res,deltaDegradation);
			break;
		case 3:
			break;
		case 4:
		break;
	}
   
}

Decision DeltaAdaptiveDecisions::takeDecision (const HWCounters & hwc, bool delayedStart)
{
   Decision res = this->defaultDecision ();

   if (delayedStart)
   {
      return res;
   }

   unsigned int curFreqId = this->formerFreqId;
   unsigned int maxFreqId = this->unit.getNbFreqs () - 1;

   float Mips = this->getMipsRatio (hwc);
   float Ipc = this->getHWExploitationRatio (hwc);

   //we are in the mips evaluation
   if (this->curState == DeltaAdaptiveDecisions::WAITING_START)
   {
      this->curState = DeltaAdaptiveDecisions::MIPS_EVALUATION;

      res.freqId = 0;
      res.sleepWin = DeltaAdaptiveDecisions::STATIC_EVAL_TIME;
      res.preCntResetPause = this->unit.getSwitchLatency () / 1000;
   }
   else if (this->curState == DeltaAdaptiveDecisions::MIPS_EVALUATION)
   {
      // remember the IPC and MIPS for this frequency
      this->mipsEval [curFreqId] = Mips;
      this->ipcEval [curFreqId] = Ipc;

      // we have other frequencies to evaluate
      if (curFreqId != maxFreqId)
      {
         res.freqId = curFreqId + 1;
         res.sleepWin = DeltaAdaptiveDecisions::STATIC_EVAL_TIME;
         res.preCntResetPause = this->unit.getSwitchLatency () / 1000;
      }
      else //we have the mips for all freq
      {
         this->resetBetaState (res);
      }
   }
   else if (this->curState == DeltaAdaptiveDecisions::RUNNING_STP1)
   {
      this->mipsEval [this->formerFreqId] = Mips;
      this->ipcEval [this->formerFreqId] = Ipc;

      res.freqId = this->freqIdStp2;
      res.sleepWin = this->sleepWinStp2;
      res.preCntResetPause = this->unit.getSwitchLatency () / 1000;

      this->curState = DeltaAdaptiveDecisions::RUNNING_STP2;
   }
   else if (this->curState == DeltaAdaptiveDecisions::RUNNING_STP2)
   {
      this->mipsEval [this->formerFreqId] = Mips;
      this->ipcEval [this->formerFreqId] = Ipc;
      
      this->resetBetaState (res);
   }

   // remember some stuff to take better decisions afterwards
   this->formerFreqId = res.freqId;
   this->formerSleepWin = res.sleepWin;

   return res;
}

