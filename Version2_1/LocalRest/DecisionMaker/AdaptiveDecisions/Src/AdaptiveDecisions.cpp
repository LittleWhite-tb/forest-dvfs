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
   defDec = this->defaultDecision ();

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

Decision AdaptiveDecisions::takeDecision (HWCounters & hwc)
{
   Decision res;

   float sqFull = HWCounters [0];
   float cycles = HWCounters [1];
   float L2Misses = HWCounters [2];
   float refCycles = HWCounters [3];

   unsigned int curFreqId = this->decisions [core].freqId;
   unsigned int curSleepWin = this->decisions [core].sleepWin;

   float locMaxBoundness = this->maxBoundness [core];

   // compute boundness and put it into our internal form:
   // between 0 and 1, where 1 = CPU bound, 0 = mem bound
   float boundness = this->computeBoundness (sqFull, cycles, L2Misses);

   locMaxBoundness = max (locMaxBoundness, boundness);
   this->maxBoundness [core] = locMaxBoundness;

   boundness = 1 - (boundness / locMaxBoundness);

   // handle this special case
   if (cycles == 0)
   {
      boundness = 0;
   }

   if (core == 0)
   {
      std::cout << "sqFull " << sqFull << " refCycles " << refCycles << " L2Misses " << L2Misses << std::endl;
   }

   float perfIdx = 1 - (this->computeBoundness (sqFull, refCycles, L2Misses) / locMaxBoundness);

   //
   // decision algo starts here
   //

   unsigned int boundFreqPrediction = min (boundness * this->nbFreqs, this->nbFreqs - 1);

   // default: do not do anything
   res.freqId = curFreqId;

   // large freq change requested by boundness prediction
   if (boundFreqPrediction < min (curFreqId, curFreqId - this->freqOffset [core])
         || boundFreqPrediction > max (curFreqId + 1, curFreqId + 1 - this->freqOffset [core]))
   {
      res.freqId = boundFreqPrediction;
      this->freqOffset [core] = 0;
   }
   else
   {
      // intense use of the resources, try to increase the frequency a bit
      if (perfIdx > 0.9 && curFreqId < this->nbFreqs - 1 && this->freqOffset [core] >= 0)
      {
         unsigned int off = min (2, this->nbFreqs - 1 - curFreqId);

         res.freqId += off;
         this->freqOffset [core] += off;
      }
      else
      {
         // resources poorly exploited, decrease the frequency a bit
         if (perfIdx < 0.8 && curFreqId > 0 && this->freqOffset [core] <= 0)
         {
            unsigned int off = min (2, curFreqId);

            res.freqId -= off;
            this->freqOffset [core] -= off;
         }
      }
   }

   // adapt the sleep window to stable phases
   if (res.freqId != curFreqId)
   {
      res.sleepWin = MIN_SLEEP_WIN;
   }
   else
   {
      res.sleepWin = min (MAX_SLEEP_WIN, curSleepWin * 2);
   }

   //
   // end of decision algo
   //

   if (core == 0)
   {
      std::cout << "Bnd: " << boundness << " Freq: " << curFreqId << " Pfx: " << perfIdx << std::endl;
   }

   // remember some stuff to take better decisions afterwards
   this->decisions [core] = res;

   return res;
}
