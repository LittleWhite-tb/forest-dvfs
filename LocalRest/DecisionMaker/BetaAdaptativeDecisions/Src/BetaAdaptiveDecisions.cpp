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
   //this->freqSel = new BetaFreqSelector (unit.getNbFreqs ());
   this->ipcEval = new float [this->unit.getNbFreqs ()];
   this->mipsEval = new float [this->unit.getNbFreqs ()];
	for (unsigned int i = 0; i< this->unit.getNbFreqs (); i++)
	{
		this->mipsEval[i] = -1;
	}

   this->formerSleepWin = defDec.sleepWin;
   this->formerFreqId = defDec.freqId;
   this->formerExecSleepWin = defDec.sleepWin;
   this->formerExecFreqId = defDec.freqId;
   this->inEvalStep = false;
   this->lastFreqEval = false;
   this->jumpToNextBetaComputation = false;
   this->updateMips = false;
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
	Decision dec = this->takeDecision(hwc, false);
	return dec;
}

Decision BetaAdaptiveDecisions::takeDecision (const HWCounters & hwc, bool delayedStart)
{
   Decision res = this->defaultDecision();
   if(delayedStart == true)
   {	
	
	#ifdef REST_EXTRA_LOG
		Logger & log = Logger::getLog(this->unit.getOSId());
		std::stringstream ss (std::stringstream::out);
		ss << "[BetaAdaptiveDecisions::takeDecision] delayed ...";
		log.logOut(ss);
	#endif
	
	return res;
   }
   unsigned int curSleepWin = this->formerSleepWin;
   unsigned int curFreqId = this->formerFreqId;

   float HWexp = this->getHWExploitationRatio (hwc);
   float Mips = this->getMipsRatio (hwc);

   //#ifdef REST_EXTRA_LOG
	//this->switchOFS <<"["<< this->unit.getOSId()<< "] =====> eval["<<this->unit.getFrequency()<<"/"<<(this->unit.getNbFreqs()-1)<<"]"<<std::endl;
	//this->switchOFS <<"  hwc.retired : "<< hwc.retired <<" hwc.cycles : "<< hwc.cycles << std::endl;
	//this->switchOFS <<"  freq unsed : " <<(this->unit.getFrequency (this->unit.getFrequency ())) <<std::endl;
	//this->switchOFS <<"  IPC : " << ( hwc.retired /(1.0 * hwc.cycles) ) << std::endl;
	//this->switchOFS <<"  freq * IPC : " <<(( hwc.retired /(1.0 * hwc.cycles) ) * this->unit.getFrequency (this->unit.getFrequency ())) << std::endl;
	//this->switchOFS <<"  MIPS : "<< ((( hwc.retired /(1.0 * hwc.cycles) ) * this->unit.getFrequency (this->unit.getFrequency ()))/1000000) << std::endl << std::endl;
   //#endif
   
   //define the freq to test
   unsigned int minEvalFreqId = 0; //rest_max (0, (int) this->formerExecFreqId - (int) BetaAdaptiveDecisions::NB_EVAL_NEAR_FREQS);
   unsigned int maxEvalFreqId = this->unit.getNbFreqs()-1; //rest_min (this->unit.getNbFreqs () - 1, this->formerExecFreqId + BetaAdaptiveDecisions::NB_EVAL_NEAR_FREQS);

   //determine if we are either in the mips evaluation or in the the exec of virtual freq
   inEvalStep = curSleepWin == BetaAdaptiveDecisions::STATIC_EVAL_TIME;
   
    
	//we are in the mips evaluation
	if (this->inEvalStep && !this->lastFreqEval)
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
			res.freqId = 0;
			res.sleepWin = BetaAdaptiveDecisions::STATIC_EVAL_TIME;
		}
	}
	// The evaluation of all freq is finished, we are now computing virtual frequencies
	else
	{
		if(this->updateMips)
		{
			//#ifdef REST_EXTRA_LOG
			//this->switchOFS << "[" << this->unit.getOSId()<< "]" << " UPDATE : previous MIPS[" << this->formerFreqId<<"] = " << this->mipsEval[this->formerFreqId]; 
			this->mipsEval[this->formerFreqId] = Mips; 
			//this->switchOFS << "[" << this->unit.getOSId()<< "]" << " updated MIPS[" << this->formerFreqId<<"] = " << this->mipsEval[this->formerFreqId]<<std::endl; 
			//#endif
		}

		if(!this->jumpToNextBetaComputation)
		{
			this->updateMips = false;
			//Compute Beta
			double tmpNumerator = 0,tmpDenominator = 0;
			for (unsigned int i = 0; i < this->unit.getNbFreqs(); i++)
			{
				tmpNumerator+=((this->unit.getFrequency(this->unit.getNbFreqs()-1)/(1.0 * this->unit.getFrequency(i)))-1)*(((this->mipsEval[this->unit.getNbFreqs()-1])/(1.0 * (this->mipsEval[i])))-1);
				tmpDenominator+=((this->unit.getFrequency(this->unit.getNbFreqs()-1)/(1.0 * this->unit.getFrequency(i)))-1)*((this->unit.getFrequency(this->unit.getNbFreqs()-1)/(1.0 * this->unit.getFrequency(i)))-1);
				
				//#ifdef REST_EXTRA_LOG
					//this->switchOFS <<"["<<this->unit.getOSId() <<"] " << "Fmax : "<< this->unit.getFrequency(this->unit.getNbFreqs()-1);
					//this->switchOFS <<" | F["<<i<<"] : "<< this->unit.getFrequency(i);
					//this->switchOFS <<" | MIPS [Fmax] : " << this->mipsEval[this->unit.getNbFreqs()-1];
					//this->switchOFS <<" | MIPS ["<<i<<"] : " << this->mipsEval[i];

					//this->switchOFS <<" | Numerator[" << i <<"] : "<<  tmpNumerator;
					//this->switchOFS <<" | Denominator[" << i <<"] : "<<  tmpDenominator << std::endl;
				//#endif
			}
			double betaCoef = tmpNumerator / tmpDenominator;


			// DEBUG
			//#ifdef REST_EXTRA_LOG		
				//this->switchOFS <<std::endl<<"["<<this->unit.getOSId() <<"] " << "Numerator : "<<  tmpNumerator;
				//this->switchOFS <<" | Denominator : "<<  tmpDenominator;
				//this->switchOFS <<" | betaCoef : "<<  betaCoef << std::endl;
			//#endif

		
			//Find the virtual freq
			unsigned int minVirtFreq = this->unit.getFrequency(0);
			unsigned int maxVirtFreq = (this->unit.getFrequency(this->unit.getNbFreqs()-1))/(1+(0.05/betaCoef));
			unsigned int virualFreq = rest_max(minVirtFreq,maxVirtFreq);

			// DEBUG
			//#ifdef REST_EXTRA_LOG
				//this->switchOFS <<"["<<this->unit.getOSId() <<"]" << " minVirtFreq : " << minVirtFreq << " | maxVirtFreq : " << maxVirtFreq << " | virualFreq : " << virualFreq << std::endl;
			//#endif

			//select the real adjacent freq of the virutal one
			int adjacentHighFreqId = -1,adjacentLowFreqId = -1;
			for(unsigned int i=0; i < this->unit.getNbFreqs(); i++)
			{
				if(this->unit.getFrequency(i) > virualFreq)
				{
					adjacentHighFreqId = i;
				}
			}
			if(adjacentHighFreqId == -1)
			{
				adjacentHighFreqId = this->unit.getNbFreqs()-1;
			}
			adjacentLowFreqId = adjacentHighFreqId - 1;

			// DEBUG
			//#ifdef REST_EXTRA_LOG
				//this->switchOFS <<"["<<this->unit.getOSId()<<"]" << " adjacentHighFreqId : " << adjacentHighFreqId << " | adjacentLowFreqId : " << adjacentLowFreqId << std::endl;
			//#endif

			//compute the runing time for each
			double rFactor = ((1+(0.05/betaCoef))/(1.0 *(this->unit.getFrequency(this->unit.getNbFreqs()-1)))) - (1/(1.0 * this->unit.getFrequency(adjacentHighFreqId)));
			//#ifdef REST_EXTRA_LOG
				//this->switchOFS <<"["<<this->unit.getOSId()<<"]" << " Rfactor Numerator : " << rFactor;
			//#endif
		

			rFactor /= ((1/(1.0 * this->unit.getFrequency(adjacentLowFreqId)))-(1/(1.0 * this->unit.getFrequency(adjacentHighFreqId))));
		 	if(rFactor < 0) { rFactor = 0; }
			if(rFactor > 1) { rFactor = 1; }	
			//#ifdef REST_EXTRA_LOG
				//this->switchOFS << " | rfactor Denominator : " <<  ((1/(1.0 * this->unit.getFrequency(adjacentLowFreqId)))-(1/(1.0 * this->unit.getFrequency(adjacentHighFreqId))));
				//this->switchOFS << " | rfactor : " <<  rFactor << std::endl;
			//#endif
			

			this->nextVirtualFreqId = adjacentHighFreqId;
			this->nextVirtualSleepWin = (1 - rFactor) * BetaAdaptiveDecisions::STATIC_EVAL_TIME;

			res.freqId=adjacentLowFreqId;
			res.sleepWin = rFactor * BetaAdaptiveDecisions::STATIC_EVAL_TIME;
			res.preCntResetPause = (res.sleepWin < (this->unit.getSwitchLatency () / 1000)) ? (this->unit.getSwitchLatency()/1000) : 0;
			
			this->updateMips = true;
			
			//if(BetaAdaptiveDecisions::VERBOSE)
			//{
			//#ifdef REST_EXTRA_LOG

				//this->switchOFS <<"["<<this->unit.getOSId()<<"]" <<" [applied]first choice => freq : " << res.freqId << " | timeslice : " << res.sleepWin << " | resetPause : " << res.preCntResetPause << std::endl;
			//#endif
			//}
			
			jumpToNextBetaComputation = true;

		}
		else
		{
			res.freqId = this->nextVirtualFreqId;
			res.sleepWin = this->nextVirtualSleepWin;
			res.preCntResetPause = (res.sleepWin < (this->unit.getSwitchLatency () / 1000)) ? (this->unit.getSwitchLatency()/1000) : 0;
			//if(BetaAdaptiveDecisions::VERBOSE)
			//{
			//#ifdef REST_EXTRA_LOG
				//this->switchOFS <<"["<<this->unit.getOSId()<<"]" << "[applied]second choice => freq : " << res.freqId << " | timeslice : " << res.sleepWin << " | resetPause : " << res.preCntResetPause << std::endl;
			//#endif
			//}
			jumpToNextBetaComputation = false;
		}
	}
   // remember some stuff to take better decisions afterwards
   this->formerFreqId = res.freqId;
   this->formerSleepWin = res.sleepWin;
	
   return res;
}

