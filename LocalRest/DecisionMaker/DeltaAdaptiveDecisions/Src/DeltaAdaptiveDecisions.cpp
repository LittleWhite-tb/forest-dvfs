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
   
   for (unsigned int i = 0; i< this->unit.getNbFreqs (); i++)
   {
      this->mipsEval [i] = -1;
   }

   this->formerSleepWin = defDec.sleepWin;
   this->formerFreqId = defDec.freqId;
   this->curState = DeltaAdaptiveDecisions::WAITING_START;
}

DeltaAdaptiveDecisions::~DeltaAdaptiveDecisions (void)
{
   delete [] this->mipsEval;
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


void DeltaAdaptiveDecisions::freqSelStrategy1 (int *adjacentLowFreqId,int *adjacentHighFreqId, unsigned int virtualFreq)
{
	int maxFreqId = this->unit.getNbFreqs ();
	float maxMIPS = 0;
	*adjacentHighFreqId = -1;
	float minMIPS = 100;
	*adjacentLowFreqId = -1;
	
	for (int i = 0; i < maxFreqId; i++)
	{
		if(maxMIPS < this->mipsEval [i] && this->unit.getFrequency(i) >= virtualFreq )
		{
			maxMIPS = this->mipsEval [i];
			*adjacentHighFreqId = i;
		}
		if(minMIPS > this->mipsEval [i] && this->unit.getFrequency(i) <= virtualFreq)
		{
			minMIPS = this->mipsEval [i];
			*adjacentLowFreqId = i;
		}
	}
	
#ifdef REST_EXTRA_LOG
	std::stringstream ss (std::stringstream::out);
	ss << "[S1] maxMIPS [" << maxMIPS << "] adjacentHighFreqId [" << *adjacentHighFreqId << "]";
	ss << " [S1] minMIPS [" << minMIPS << "] adjacentLowFreqId [" << *adjacentLowFreqId << "]" << std::endl;
	Logger &log = Logger::getLog(this->unit.getOSId ());
	log.logOut(ss);
#endif
	
}


void DeltaAdaptiveDecisions::freqSelStrategy2 (int *adjacentLowFreqId,int *adjacentHighFreqId, unsigned int virtualFreq)
{
	int maxFreqId = this->unit.getNbFreqs ();
	*adjacentHighFreqId = maxFreqId - 1;
	*adjacentLowFreqId = -1;
	for (int i = 0; i < maxFreqId; i++)
	{
		if (this->unit.getFrequency (i) > virtualFreq)
		{
			*adjacentHighFreqId = i;
			break;
		}
	}
	*adjacentLowFreqId = rest_max (0, *adjacentHighFreqId - 1);
	
#ifdef REST_EXTRA_LOG
	std::stringstream ss (std::stringstream::out);
	ss << "[S2] adjacentHighFreqId [" << *adjacentHighFreqId << "]";
	ss << " [S2] adjacentLowFreqId [" << *adjacentLowFreqId << "]" << std::endl;
	Logger &log = Logger::getLog(this->unit.getOSId ());
	log.logOut(ss);
#endif
}

void DeltaAdaptiveDecisions::freqSelStrategy3 (int *adjacentLowFreqId,int *adjacentHighFreqId, unsigned int virtualFreq)
{
	int maxFreqId = this->unit.getNbFreqs ();
	float maxMIPS = 0;
	*adjacentHighFreqId = maxFreqId - 1;
	int tmpAdjacentHighFreqId = -1;
	*adjacentLowFreqId = -1;
	for (int i = 0; i < maxFreqId; i++)
	{
		if(maxMIPS < this->mipsEval [i] && this->unit.getFrequency(i) >= virtualFreq )
		{
			maxMIPS = this->mipsEval [i];
			*adjacentHighFreqId = i;
		}
		
		if (this->unit.getFrequency (i) > virtualFreq)
		{
			tmpAdjacentHighFreqId = i;
			break;
		}
	}
	*adjacentLowFreqId = rest_max (0, tmpAdjacentHighFreqId - 1);
	
#ifdef REST_EXTRA_LOG
	std::stringstream ss (std::stringstream::out);
	ss << "[S3]  maxMIPS [" << maxMIPS <<"] adjacentHighFreqId [" << *adjacentHighFreqId << "]";
	ss << " [S3] adjacentLowFreqId [" << *adjacentLowFreqId << "]" << std::endl;
	Logger &log = Logger::getLog(this->unit.getOSId ());
	log.logOut(ss);
#endif
}

void DeltaAdaptiveDecisions::freqSelStrategy4 (int *adjacentLowFreqId,int *adjacentHighFreqId, unsigned int virtualFreq)
{
	int maxFreqId = this->unit.getNbFreqs ();
	float minMIPS = 100;
	*adjacentHighFreqId = maxFreqId - 1;
	*adjacentLowFreqId = -1;
	for (int i = 0; i < maxFreqId; i++)
	{
		if(minMIPS > this->mipsEval [i] && this->unit.getFrequency(i) <= virtualFreq)
		{
			minMIPS = this->mipsEval [i];
			*adjacentLowFreqId = i;
		}
		
		if (this->unit.getFrequency (i) > virtualFreq)
		{
			*adjacentHighFreqId = i;
			break;
		}
	}
	
#ifdef REST_EXTRA_LOG
	std::stringstream ss (std::stringstream::out);
	ss << "[S4] adjacentHighFreqId [" << *adjacentHighFreqId << "]";
	ss << " [S4] minMIPS [" << minMIPS << "] adjacentLowFreqId [" << *adjacentLowFreqId << "]" << std::endl;
	Logger &log = Logger::getLog(this->unit.getOSId ());
	log.logOut(ss);
#endif
}

void DeltaAdaptiveDecisions::resetBetaState(Decision &res)
{
  //The degradation factor
   int deltaDegradation = 10;
   
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
   //here modification to be donne to select freq
   int adjacentLowFreqId = -1;
   int adjacentHighFreqId = -1;
   
   this->freqSelStrategy1(&adjacentLowFreqId,&adjacentHighFreqId,virualFreq);
   this->freqSelStrategy2(&adjacentLowFreqId,&adjacentHighFreqId,virualFreq);
   this->freqSelStrategy3(&adjacentLowFreqId,&adjacentHighFreqId,virualFreq);
   this->freqSelStrategy4(&adjacentLowFreqId,&adjacentHighFreqId,virualFreq);

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

      res.freqId = this->freqIdStp2;
      res.sleepWin = this->sleepWinStp2;
      res.preCntResetPause = this->unit.getSwitchLatency () / 1000;

      this->curState = DeltaAdaptiveDecisions::RUNNING_STP2;
   }
   else if (this->curState == DeltaAdaptiveDecisions::RUNNING_STP2)
   {
      this->mipsEval [this->formerFreqId] = Mips;
      
      this->resetBetaState (res);
   }

   // remember some stuff to take better decisions afterwards
   this->formerFreqId = res.freqId;
   this->formerSleepWin = res.sleepWin;

   return res;
}

