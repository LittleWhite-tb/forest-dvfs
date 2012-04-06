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
 @file NaiveDecisions.cpp
 @brief The  NaiveDecisions class is in this file
 */

#include <assert.h>
#include <cmath>
#include <iostream>

#include "NaiveDecisions.h"

NaiveDecisions::NaiveDecisions (DVFSUnit & unit) :
   DecisionMaker (unit)
{

}

NaiveDecisions::~NaiveDecisions (void)
{

}

#if FALSE
int NaiveDecisions::giveReport (unsigned int core,
                                const unsigned long long * HWCounters)
const
{
   float boundness = computeBoundness (HWCounters [0], HWCounters [1], HWCounters [2]);

   unsigned int newFrequency = round ((unsigned int)
                                       ((1 - boundness) * (coresInfo->numFreqs - 1)));

   // do not go into TurboBoost freq
   if (this->coresInfo->hasTurboBoost)
   {
      newFrequency = (newFrequency == this->coresInfo->numFreqs - 1) ?
                     this->coresInfo->numFreqs - 2 : newFrequency;
   }

   unsigned int currentFreq = this->coresInfo->allCoreData [core].requestedFreq;
   unsigned int FreqDistance = (unsigned int) std::abs ((float)(newFrequency - currentFreq));

   // only change freq when large increase (more than one jump)
   // or decreasing the frequence
   if (FreqDistance > 1 || newFrequency < currentFreq)
   {
      return newFrequency;
   }
   else
   {
      return -1;
   }
}
#endif