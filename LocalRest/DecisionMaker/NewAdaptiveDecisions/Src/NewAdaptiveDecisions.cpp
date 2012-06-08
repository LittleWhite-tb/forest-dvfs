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
 * @file NewNewAdaptiveDecisions.cpp
 * The NewNewAdaptiveDecisions class is in this file
 */

#include <iostream>

#include "NewAdaptiveDecisions.h"
#include "Common.h"

#ifdef REST_EXTRA_LOG
#include <sstream>
#include <vector>
#include "Logger.h"
#endif

NewAdaptiveDecisions::NewAdaptiveDecisions (DVFSUnit & unit) :
   DecisionMaker (unit)
{
   Decision defDec = this->defaultDecision ();

   // initialize internal data
   this->ipcEval = new float [this->unit.getNbFreqs ()];

   this->formerSleepWinStep1 = defDec.sleepWin;
   this->formerFreqIdStep1 = defDec.freqId;
   this->formerSleepWinStep2 = defDec.sleepWin;
   this->formerFreqIdStep2 = defDec.freqId;
   this->curState = NewAdaptiveDecisions::START;
   this->totalsleepWin = NewAdaptiveDecisions::MIN_SLEEP_WIN;

}

NewAdaptiveDecisions::~NewAdaptiveDecisions (void)
{
   delete [] this->ipcEval;
}

Decision NewAdaptiveDecisions::defaultDecision ()
{
   Decision res;
   res.sleepWin = NewAdaptiveDecisions::MIN_SLEEP_WIN;
   res.freqId = 0;
   res.preCntResetPause = 0;

   return res;
}

void NewAdaptiveDecisions::getVirtualFreq(float degradedIPC,unsigned int minFreqId,unsigned int maxFreqId,Decision &Step1, Decision &Step2)
{
	float max=0;
	float min=100;
	
	int tmpfreqId1 = -1; 
	int tmpfreqId2 = -1;

	Step1.freqId = 0;
	Step2.freqId = 0; 
	Step1.timeRatio = 0.0;
	Step2.timeRatio = 0.0;


#ifdef REST_EXTRA_LOG
        std::stringstream ss (std::stringstream::out);
        ss << "degradedIPC = " << degradedIPC << " | minFreqId = " << minFreqId << " | maxFreqId = " << maxFreqId;
        Logger &log = Logger::getLog(this->unit.getOSId ());
        log.logOut(ss);
#endif
	
	//find the two consecutive IPC that majorate and minorate the degradedIPC
	for (unsigned int i = minFreqId; i <= maxFreqId; i++)
	{
		if(max < this->ipcEval[i] && this->ipcEval[i] < degradedIPC )
		{
			max = this->ipcEval[i];
			tmpfreqId1 = i;
#ifdef REST_EXTRA_LOG
        std::stringstream ss (std::stringstream::out);
        ss << "max = " << max << " | tmpfreqId1 = " << tmpfreqId1;
        Logger &log = Logger::getLog(this->unit.getOSId ());
        log.logOut(ss);
#endif

		}
		if(min > this->ipcEval[i] && this->ipcEval[i] > degradedIPC )
		{
			min = this->ipcEval[i];
			tmpfreqId2 = i;
#ifdef REST_EXTRA_LOG
        std::stringstream ss (std::stringstream::out);
        ss << "min " << min << " | tmpfreqId2 " << tmpfreqId2;
        Logger &log = Logger::getLog(this->unit.getOSId ());
        log.logOut(ss);
#endif

		}

	}
	if(max == 0 && tmpfreqId1 == -1)
	{
		Step2.timeRatio = 1;
		Step2.freqId = tmpfreqId2;

		Step1.timeRatio = 1 - Step2.timeRatio;
		Step1.freqId = 0;
	}
	else
	{	
		//once the freq are found, compute the time to spend in each
		Step1.freqId = tmpfreqId1;
		Step1.timeRatio = (degradedIPC - max)/(min-max);
		Step2.freqId = tmpfreqId2;
		Step2.timeRatio = 1 - Step1.timeRatio;
	}
	
}


Decision NewAdaptiveDecisions::takeDecision (const HWCounters & hwc)
{
	Decision dec = this->takeDecision (hwc, false);
	return dec;
}

Decision NewAdaptiveDecisions::takeDecision (const HWCounters & hwc, bool delayedStart)
{
   Decision res;
   Decision Step1;
   Decision Step2;

   res = this->defaultDecision();

   if(delayedStart == true)
   {
       return res;
   }
   unsigned int curSleepWin = this->formerSleepWin;
   unsigned int curFreqId = this->formerFreqId;

   float HWexp = this->getHWExploitationRatio (hwc);
   // get the correct center of the frequency window
   if(this->formerSleepWinStep1 > this->formerSleepWinStep2)
   {
	this->freqWindowCenter = this->formerFreqIdStep1;
   }
   else if (this->formerSleepWinStep1 == this->formerSleepWinStep2)
   {
	this->freqWindowCenter = this->formerFreqIdStep1;
	if(this->formerFreqIdStep1 > this->formerFreqIdStep2)
	{
		this->freqWindowCenter = this->formerFreqIdStep2;
	}
   }
   else
   {
	this->freqWindowCenter = this->formerFreqIdStep2;
   }

   //define the freq to test
   unsigned int minEvalFreqId = rest_max (0,
                                          (int) this->freqWindowCenter - (int) NewAdaptiveDecisions::NB_EVAL_NEAR_FREQS);
   unsigned int maxEvalFreqId = rest_min (this->unit.getNbFreqs () - 1,
                                          this->freqWindowCenter + NewAdaptiveDecisions::NB_EVAL_NEAR_FREQS);

   // were we in the evaluation step
   // 1 if the sleeping window = 20 (we test freq) 0 elsewhere.
  if ( curSleepWin == NewAdaptiveDecisions::IPC_EVAL_TIME )
  {
	this->curState = NewAdaptiveDecisions::EVAL_STP;
  }
 

   if ( this->curState == NewAdaptiveDecisions::EVAL_STP )
   {
      // remember the IPC for this frequency
      this->ipcEval [curFreqId] = HWexp;
#ifdef REST_EXTRA_LOG
	std::stringstream ss (std::stringstream::out);
        ss << "IN EVAL STEP";
        Logger &log = Logger::getLog(this->unit.getOSId ());
        log.logOut(ss);	
#endif

      // we have other frequencies to evaluate
      if (curFreqId != maxEvalFreqId)
      {
         res.freqId = curFreqId + 1;
         res.sleepWin = NewAdaptiveDecisions::IPC_EVAL_TIME;
      }
      // this was the last evaluation
      else
      {
#ifdef REST_EXTRA_LOG
        std::stringstream ss (std::stringstream::out);
	ss << "END OF THE EVAL PROCESS, START DECISION ";
	Logger & log = Logger::getLog(this->unit.getOSId ());
	log.logOut(ss);
	log.endBlock();
#endif

         // get the maxIPC from the tested IPC
         float maxIPC = 0;
         for (unsigned int i = minEvalFreqId; i <= maxEvalFreqId; i++)
         {
            maxIPC = rest_max (maxIPC, this->ipcEval [i]);
         }
	 
	 //get the frequency couple to fit the 5% degradation
	 float degradedIPC = 0.95*maxIPC;	
         this->getVirtualFreq (degradedIPC, minEvalFreqId, maxEvalFreqId, Step1, Step2);
         
         // increase the sleeping window if we were previously using this freq
         if (this->formerFreqIdStep1 == Step1.freqId && this->formerFreqIdStep2 == Step2.freqId)
         {
            this->totalsleepWin = rest_min (NewAdaptiveDecisions::MAX_SLEEP_WIN, this->totalsleepWin * 2);
         }
         else
         {
            this->totalsleepWin = NewAdaptiveDecisions::MIN_SLEEP_WIN;
         }
#ifdef REST_EXTRA_LOG
        {
	std::stringstream ss (std::stringstream::out);
        ss << "-- ["<<Step1.timeRatio<<"]Step1.freqId = " << Step1.freqId << " ["<<Step2.timeRatio<<"]Step2.freqId = " << Step2.freqId <<" --";
        Logger &log = Logger::getLog(this->unit.getOSId ());
        log.logOut(ss);
	}
#endif
	 //apply the first step of the decsion
         res.freqId = Step1.freqId;
	 res.sleepWin = Step1.timeRatio * this->totalsleepWin;

         //remember the second step to be applied
         this->freqIdStep2 = Step2.freqId;
	 this->sleepWinStep2 = Step2.timeRatio * this->totalsleepWin; 

	//remeber the two steps of the decision
	this->formerFreqIdStep1 = Step1.freqId;
	this->formerSleepWinStep1 = Step1.sleepWin;
	this->formerFreqIdStep2 = Step2.freqId;
	this->formerSleepWinStep2 = Step2.sleepWin;
#ifdef REST_EXTRA_LOG
	{
	std::stringstream ss (std::stringstream::out);
        ss << "SET FIRST STEP, WAITNG FOR STEP 2";
        Logger &log = Logger::getLog(this->unit.getOSId ());
        log.logOut(ss);	
	}
#endif
	curState = NewAdaptiveDecisions::GOTO_RUN_STP2;
      }
   }
   else
   {
      //do we have to run the next step ?
      if(this->curState == NewAdaptiveDecisions::GOTO_RUN_STP2)
      {
#ifdef REST_EXTRA_LOG
	{
	std::stringstream ss (std::stringstream::out);
        ss << "FIRST STEP ENDED,SET STEP 2";
        Logger &log = Logger::getLog(this->unit.getOSId ());
        log.logOut(ss);	
	}
#endif
 
         res.freqId = this->freqIdStep2;
	 res.sleepWin = this->sleepWinStep2;
	 this->curState = NewAdaptiveDecisions::RUN_STP2_DONE;
      }
      else
      {
#ifdef REST_EXTRA_LOG
	{
	std::stringstream ss (std::stringstream::out);
        ss << "ALL STEP DONE, GOING IN EVAL STEP";
        Logger &log = Logger::getLog(this->unit.getOSId ());
        log.logOut(ss);	
	log.endBlock();
	}
#endif

         //then go in the eval process
         res.freqId = minEvalFreqId;
         res.sleepWin = NewAdaptiveDecisions::IPC_EVAL_TIME;

         if (NewAdaptiveDecisions::VERBOSE && this->unit.getOSId () == 0)
         {
            std::cout << "boundness: " << (hwc.l2miss * 1.) / hwc.cycles << std::endl;
         
	 }
      }
   }

   // remember some stuff to take better decisions afterwards
   this->formerFreqId = res.freqId;
   this->formerSleepWin = res.sleepWin;

   if (res.sleepWin != NewAdaptiveDecisions::IPC_EVAL_TIME)
   {
      // we don't care about when the frequency will be set
      res.preCntResetPause = 0;
   }
   else
   {
      // we are in an evaluation step, wait before reseting the counters: the
      // latency has to be actually set before we can measure anything relevant
      res.preCntResetPause = this->unit.getSwitchLatency () / 1000;
   }

   return res;
}

