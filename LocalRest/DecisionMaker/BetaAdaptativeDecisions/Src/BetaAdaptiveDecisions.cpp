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

#include "BetaAdaptiveDecisions.h"
#include "Common.h"


/**
* Represent the run time of a specific freq for the emulation of the virtual freq
*/
typedef struct
{
	int runTime;
	int freqId;
}virtualFreqSlice;

/**
* The acumalation of freq and associated runtime to emulate a specific virtual freq
*/
std::stack<virtualFreqSlice> virtualFreqComposition;


BetaAdaptiveDecisions::BetaAdaptiveDecisions (DVFSUnit & unit) :
   DecisionMaker (unit)
{
   Decision defDec = this->defaultDecision ();

   // initialize internal data
   //this->freqSel = new BetaFreqSelector (unit.getNbFreqs ());
   this->ipcEval = new float [this->unit.getNbFreqs ()];
   this->mipsEval = new float [this->unit.getNbFreqs ()];


   this->formerSleepWin = defDec.sleepWin;
   this->formerFreqId = defDec.freqId;
   this->formerExecSleepWin = defDec.sleepWin;
   this->formerExecFreqId = defDec.freqId;
}

BetaAdaptiveDecisions::~BetaAdaptiveDecisions (void)
{
   //delete this->freqSel;
   delete [] this->ipcEval;
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
   Decision res = this->defaultDecision();

   unsigned int curSleepWin = this->formerSleepWin;
   unsigned int curFreqId = this->formerFreqId;

   float HWexp = this->getHWExploitationRatio (hwc);
   float Mips = this->getMipsRatio (hwc);

   static bool inEvalStep = false;
   static bool lastFreqEval = false;
   static bool jumpToNextBetaComputation = false;

   //define the freq to test
   unsigned int minEvalFreqId = 0; //rest_max (0, (int) this->formerExecFreqId - (int) BetaAdaptiveDecisions::NB_EVAL_NEAR_FREQS);
   unsigned int maxEvalFreqId = this->unit.getNbFreqs(); //rest_min (this->unit.getNbFreqs () - 1, this->formerExecFreqId + BetaAdaptiveDecisions::NB_EVAL_NEAR_FREQS);

   //determine if we are either in the mips evaluation or in the the exec of virtual freq
   inEvalStep = curSleepWin == BetaAdaptiveDecisions::STATIC_EVAL_TIME;

   //we are in the mips evaluation
   if (inEvalStep && !lastFreqEval)
   {
      // remember the IPC for this frequency
      this->ipcEval [curFreqId] = HWexp;
      this->mipsEval [curFreqId] = Mips;

      // we have other frequencies to evaluate
      if (curFreqId != maxEvalFreqId)
      {
         res.freqId = curFreqId + 1;
         res.sleepWin = BetaAdaptiveDecisions::STATIC_EVAL_TIME;
      }
      else //we have the mips for all freq
      {
      	lastFreqEval = true;
      }
   }
	// The evaluation of all freq is finished, we are now computing virtual frequencies
	else
	{
		// DEBUG
		if (BetaAdaptiveDecisions::VERBOSE && this->unit.getOSId () == 0)
		{
			std::cout << "Evaluation result [" << minEvalFreqId << '-' << maxEvalFreqId << "]: ";
			for (unsigned int i = minEvalFreqId; i <= maxEvalFreqId; i++)
			{
				std::cout << this->ipcEval [i] << " ";
			}
			std::cout << std::endl;
			for (unsigned int i = minEvalFreqId; i <= maxEvalFreqId; i++)
			{
				std::cout << this->mipsEval [i] << " ";
			}
			std::cout << std::endl;
		}
		if(!jumpToNextBetaComputation)
		{
			//Compute Beta
			int tmpNumerator = 0,tmpDenominator = 0;
			for (unsigned int i = 0; i < this->unit.getNbFreqs(); i++)
			{
				tmpNumerator+=((this->unit.getFrequency(this->unit.getNbFreqs()-1)/this->unit.getFrequency(i))-1)*((this->mipsEval[this->unit.getNbFreqs()-1])/(this->mipsEval[i])-1);
				tmpDenominator+=((this->unit.getFrequency(this->unit.getNbFreqs()-1)/this->unit.getFrequency(i))-1)*((this->unit.getFrequency(this->unit.getNbFreqs()-1)/this->unit.getFrequency(i))-1);
			}
			int betaCoef = tmpNumerator / tmpDenominator;

			// DEBUG
			if (BetaAdaptiveDecisions::VERBOSE && this->unit.getOSId () == 0)
			{
				std::cout << "Numerator : " << tmpNumerator << " Denominator : " << tmpDenominator << " betaCoef : " << betaCoef << std::endl;
			}

			//Find the virtual freq
			unsigned int minVirtFreq = this->unit.getFrequency(0);
			unsigned int maxVirtFreq = (this->unit.getFrequency(this->unit.getNbFreqs()-1))/(1+(0.05/betaCoef));
			unsigned int virualFreq = rest_max(minVirtFreq,maxVirtFreq);

			// DEBUG
			if (BetaAdaptiveDecisions::VERBOSE && this->unit.getOSId () == 0)
			{
				std::cout << "minVirtFreq : " << minVirtFreq << " maxVirtFreq : " << maxVirtFreq << " virualFreq : " << virualFreq << std::endl;
			}

			//select the real adjacent freq of the virutal one
			int adjacentHighFreqId = -1,adjacentLowFreqId = -1;
			for(unsigned int i=0; i < this->unit.getNbFreqs(); i++)
			{
				if(this->unit.getFrequency(i) > virualFreq)
				{
					adjacentHighFreqId = this->unit.getFrequency(i);
				}
			}
			adjacentLowFreqId = adjacentHighFreqId - 1;

			// DEBUG
			if (BetaAdaptiveDecisions::VERBOSE && this->unit.getOSId () == 0)
			{
				std::cout << "adjacentHighFreqId : " << adjacentHighFreqId << " adjacentLowFreqId : " << adjacentLowFreqId << std::endl;
			}
			//compute the runing time for each
			int rFactor = ((1+(0.05/betaCoef))/(this->unit.getFrequency(this->unit.getNbFreqs()-1))) - (1/this->unit.getFrequency(adjacentHighFreqId));
			rFactor /= ((1/this->unit.getFrequency(adjacentLowFreqId))-(1/this->unit.getFrequency(adjacentHighFreqId)));
			//push on a stack the two time and freq to be used

			virtualFreqSlice virtualFreq;
			virtualFreq.freqId=adjacentLowFreqId;
			virtualFreq.runTime =(1 - rFactor) * BetaAdaptiveDecisions::STATIC_EVAL_TIME;
			virtualFreqComposition.push(virtualFreq);

			virtualFreq.freqId=adjacentLowFreqId;
			virtualFreq.runTime = rFactor * BetaAdaptiveDecisions::STATIC_EVAL_TIME;
			virtualFreqComposition.push(virtualFreq);

			jumpToNextBetaComputation = true;

		}
		else
		{
			virtualFreqComposition.pop();
			virtualFreqSlice virtualFreq = virtualFreqComposition.top();
			res.freqId = virtualFreq.freqId;
			res.sleepWin = virtualFreq.runTime;
			res.preCntResetPause = 0;
			if(virtualFreqComposition.size() == 0)
			{
				jumpToNextBetaComputation = false;
			}
		}
	}

   // remember some stuff to take better decisions afterwards
   this->formerFreqId = res.freqId;
   this->formerSleepWin = res.sleepWin;


#ifdef REST_EXTRA_LOG
#endif

            // we are in an evaluation step, wait before reseting the counters: the
      // latency has to be actually set before we can measure anything relevant
    //  res.preCntResetPause = this->unit.getSwitchLatency () / 1000;


   return res;
}

