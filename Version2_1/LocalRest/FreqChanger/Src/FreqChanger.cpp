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
 @file FreqChanger.cpp
 @brief The  FreqChanger class is in this file
 */

#include <cassert>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cstdio>

#include "FreqChanger.h"

FreqChanger::FreqChanger (CoresInfo * coresInfo)
{
   //Saving data core' references
   this->coresInfo = coresInfo;
   assert (this->coresInfo != NULL);

   //Initializing the requested freq
   for (unsigned int i = 0; i < coresInfo->numCores; i++)
   {
      coresInfo->allCoreData [i].requestedFreq = this->readCurrentFreq (i);
   }
}

FreqChanger::~FreqChanger (void)
{

}

unsigned int FreqChanger::readCurrentFreq (unsigned int coreId)
{
   std::stringbuf sb;
   std::istringstream iss; //Convert string to int
   std::ostringstream oss; //Convert int to string
   std::ifstream fp; //File descriptor
   unsigned int curFreq = 0;

   //Setting and Opening path leading to the cpuinfo data
   oss << "/sys/devices/system/cpu/cpu" << coreId
       << "/cpufreq/scaling_cur_freq";
   fp.open (oss.str ().c_str ());

   //Reading the current frequency
   if (fp.fail ())
   {
      std::perror ("Failed to open cpufreq datafile");
      return -1;
   }
   else
   {
      fp >> &sb;
      iss.str (sb.str ());
      iss >> curFreq;
   }
   fp.close ();

   return curFreq;
}

unsigned int FreqChanger::readFreq (unsigned int coreId)
{
   assert (coreId < this->coresInfo->numCores);

   return this->coresInfo->allCoreData [coreId].requestedFreq;
}

void FreqChanger::changeFreq (unsigned int coreId, int freqId)
{
   std::ostringstream oss;

   assert (coreId < this->coresInfo->numCores);
   assert (freqId >= 0 && (unsigned int) freqId < this->coresInfo->numFreqs);

   CoreData * cd = &this->coresInfo->allCoreData [coreId];
   if (cd->requestedFreq == (unsigned int) freqId)
   {
      return;
   }

   cd->freqTrack [freqId]++;

   cd->freqFd->seekp (0, std::ios::beg);
   oss << this->coresInfo->availableFreqs [freqId];
   *cd->freqFd << oss.str ();
   cd->freqFd->flush ();

   cd->requestedFreq = this->coresInfo->availableFreqs [freqId];
}
