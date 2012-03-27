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

#define MIN_SLEEP_WIN 500
#define MAX_SLEEP_WIN 150000

AdaptiveDecisions::AdaptiveDecisions (DVFSUnit & unit) :
   DecisionMaker (unit)
{
   Decision defDec = this->defaultDecision ();

   // initialize internal data
   this->formerSleepWin = defDec.sleepWin;
   this->formerFreqId = defDec.freqId;
   this->maxBoundness = 0;
   this->maxHWExploitation = 0;
   this->freqOffset = 0;
}

AdaptiveDecisions::~AdaptiveDecisions (void)
{

}

Decision AdaptiveDecisions::defaultDecision ()
{
   Decision res;

   res.sleepWin = MIN_SLEEP_WIN;
   res.freqId = 0;

   return res;
}

Decision AdaptiveDecisions::takeDecision (const HWCounters & hwc)
{
   Decision res;

   unsigned int curSleepWin = this->formerSleepWin;
   unsigned int curFreqId = this->formerFreqId;

   // compute boundness and put it into our internal form:
   // between 0 and 1, where 1 = CPU bound, 0 = mem bound
   float boundness = this->getBoundnessRatio (hwc);   
   this->maxBoundness =  rest_max (this->maxBoundness, boundness);
   boundness = 1 - (boundness / this->maxBoundness);

   float HWexp = this->getHWExploitationRatio (hwc);
   this->maxHWExploitation = rest_max (this->maxHWExploitation, HWexp);
   HWexp = 1 - (HWexp / this->maxHWExploitation);

   // scan frequencies periodically (DEBUG decision)
   if (curFreqId == this->unit.getNbFreqs() - 1)
   {
      res.freqId = 0;
      res.sleepWin = MAX_SLEEP_WIN;
   }
   else
   {
      res.freqId = curFreqId + 1;
      res.sleepWin = MIN_SLEEP_WIN;
   }

   if (this->unit.getOSId () == 0)
   {
      std::cout << " Freq: " << curFreqId << "Bnd: " << boundness << " HWe: " << HWexp << std::endl;
   }

   // remember some stuff to take better decisions afterwards
   this->formerFreqId = res.freqId;
   this->formerSleepWin = res.sleepWin;

   return res;
}

