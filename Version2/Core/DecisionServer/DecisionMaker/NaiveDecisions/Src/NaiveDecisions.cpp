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

//#define BINARYMODE

NaiveDecisions::NaiveDecisions (CoresInfos * coresInfos) :
   DecisionMaker (coresInfos)
{

}

NaiveDecisions::~NaiveDecisions (void)
{

}

int NaiveDecisions::giveReport (unsigned int core, const long long HWCounters[3])
{
   float boundness = computeBoundness (HWCounters[0], HWCounters[1],
                                       HWCounters[2]);
#ifdef BINARYMODE
   int newFrequency = (boundness < 0.5) ?1:coresInfo->numFreqs-1;
#else
   int newFrequency = round ((int) (boundness * coresInfo->numFreqs));
   newFrequency = (boundness == 0.0) ? 1 : newFrequency; //unless it's prefectly compute bound, which it never will be, we won't use the turbo frequency
   newFrequency = (boundness == 1.0) ? coresInfo->numFreqs - 1 : newFrequency; //if it's exactly 1.0 then we set it to the lowest frequency
#endif

   //Trick to adjuste the right freq
   if (boundness <= 0.1)
   {
      newFrequency = 1;
   }

   int currentFreq = this->coresInfo->all_core_data[core].currentFreq;

   int FreqDistance = std::abs ( (float) (newFrequency - currentFreq));

   if (FreqDistance > 1 || (FreqDistance <= 1 && newFrequency > currentFreq))
   {
      return newFrequency;
   }
   else
   {
      return -1;
   }
}
