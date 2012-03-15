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

AdaptiveDecisions::AdaptiveDecisions (CoresInfo * coresInfo) :
   DecisionMaker (coresInfo)
{
   // do not go into turboboost
   if (coresInfo->hasTurboBoost)
   {
      this->nbFreqs = coresInfo->numFreqs - 1;
   }
   else
   {
      this->nbFreqs = coresInfo->numFreqs;
   }

   // build and initialize the decision table to a linear decision function
   float boundStep = 1.0f / this->nbFreqs;
   this->decTable = new float * [this->coresInfo->numCores];
   for (unsigned int i = 0; i < this->coresInfo->numCores; i++)
   {
      this->decTable [i] = new float [this->nbFreqs];
      for (unsigned int j = 0; j < this->nbFreqs; j++)
      {
         this->decTable [i][j] = j * boundStep;
      }
   }

   // initialize internal structures
   this->maxBoundness = new float [coresInfo->numCores];
   this->formerPerfIdx = new float [coresInfo->numCores];
   this->formerBoundness = new float [coresInfo->numCores];
   for (unsigned int i = 0; i < coresInfo->numCores; i++)
   {
      // default maximal value for boundness
      this->maxBoundness [i] = 0.0f;

      // < 0 to ignore this initialization value later
      this->formerPerfIdx [i] = -1;
      this->formerBoundness [i] = -1;
   }
}

AdaptiveDecisions::~AdaptiveDecisions (void)
{
   for (unsigned int i = 0; i < this->coresInfo->numCores; i++)
   {
      delete [] this->decTable [i];
   }

   delete [] this->decTable;

   delete [] this->maxBoundness;
   delete [] this->formerPerfIdx;
   delete [] this->formerBoundness;
}

Decision AdaptiveDecisions::takeDecision (unsigned int core,
      const unsigned long long * HWCounters)
{
   Decision res;

   res.freqId = this->nbFreqs - 1;
   res.sleepWin = INIT_SLEEP_WIN;

   float sqFull = HWCounters [0];
   float cycles = HWCounters [1];
   float L2Misses = HWCounters [2];
   float retiredIns = HWCounters [3];

   float oldPerfIdx = this->formerPerfIdx [core];
   float oldBoundness = this->formerBoundness [core];
   unsigned int oldFreqId = this->decisions [core].freqId;
   unsigned int oldSleepWin = this->decisions [core].sleepWin;
   float locMaxBoundness = this->maxBoundness [core];

   // compute boundness and put it into our internal form:
   // between 0 and 1, where 1 = CPU bound, 0 = mem bound
   float boundness = this->computeBoundness (sqFull, cycles, L2Misses);

   if (locMaxBoundness < boundness)
   {
      locMaxBoundness = boundness;
      this->maxBoundness [core] = boundness;
   }

   boundness = 1 - (boundness / locMaxBoundness);

   if (core == 0)
   {
      std::cout << "boundness " << boundness << " maxBoundness " << locMaxBoundness << std::endl;
   }


   // retired ins / sec
   float perfIdx = retiredIns / cycles;

   // handle this special case
   if (cycles == 0)
   {
      perfIdx = oldPerfIdx;
      boundness = oldBoundness;
   }

   float perfEvo = (perfIdx - oldPerfIdx) / oldPerfIdx;

#if FALSE
   // update the decision table by evaluating the relevance of the last decision
   if (oldPerfIdx >= 0)
   {

      // >10% of performance degradation -> next time, be a bit more gentle
      if (perfEvo <= -0.10 && oldFreqId < this->nbFreqs - 1
            && oldBoundness < this->decTable [core][oldFreqId + 1])
      {
         this->decTable [core][oldFreqId + 1] = oldBoundness;

         // ensure consistency over the table
         for (int i = oldFreqId; i >= 0; i--)
         {
            if (this->decTable [core][i] >= this->decTable [core][i + 1])
            {
               this->decTable [core][i] = this->decTable [core][i + 1] - 0.001;
            }
         }

         for (int i = oldFreqId; i >= 0; i--)
         {
            if (this->decTable [core][i] < 0)
            {
               this->decTable [core][i] = 0;
            }
         }
      }
      else
      {
         // <2% of perf degradation -> be more aggressive next time
         if (perfEvo >= -0.02 && oldFreqId > 0
               && oldBoundness + 0.001 > this->decTable [core][oldFreqId])
         {
            this->decTable [core][oldFreqId] = oldBoundness + 0.001;

            // ensure consistency of the table
            for (unsigned int i = oldFreqId + 1;
                  i < this->nbFreqs;
                  i++)
            {
               if (this->decTable [core][i] <= this->decTable [core][i - 1])
               {
                  this->decTable [core][i] = this->decTable [core][i - 1] + 0.001;
               }
            }

            for (unsigned int i = oldFreqId;
                  i < this->nbFreqs;
                  i++)
            {
               if (this->decTable [core][i] > 1)
               {
                  this->decTable [core][i] = 1;
               }
            }
         }
      }
   }
#endif

   // chose a frequency and a sleep window
   for (int i = this->nbFreqs - 1; i >= 0; i--)
   {
      if (boundness >= this->decTable [core][i])
      {
         res.freqId = i;
         break;
      }
   }

   // only increase frequency when big hops are required for stability reasons
   if ((int) res.freqId - (int) oldFreqId == 1 && oldFreqId > 0)
   {
      res.freqId = oldFreqId;
   }

   if (res.freqId != oldFreqId)
   {
      if (core == 0)
      {
         std::cout << "*** Switching to freq " << res.freqId << std::endl;
      }

      res.sleepWin = INIT_SLEEP_WIN;
   }
   else
   {
      res.sleepWin = 2 * oldSleepWin;
      if (res.sleepWin > LONGEST_SLEEP_WIN)
      {
         res.sleepWin = LONGEST_SLEEP_WIN;
      }
   }

   if (core == 0)
   {
      std::cout << "Bnd: " << boundness << " Freq: " << res.freqId << " PerfIdx: " << perfIdx << std::endl;
      for (unsigned int i = 0; i < this->nbFreqs; i++)
      {
         std::cout << this->decTable [core][i] << " ";
      }
      std::cout << std::endl;
   }

   // remember some stuff to take better decisions afterwards
   this->formerBoundness [core] = boundness;
   this->formerPerfIdx [core] = perfIdx;
   this->decisions [core] = res;

   return res;
}
