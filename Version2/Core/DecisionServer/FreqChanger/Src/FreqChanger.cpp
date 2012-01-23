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

#include <assert.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cstdio>

#include "FreqChanger.h"

FreqChanger::FreqChanger (CoresInfos * coresInfos)
{
   //Saving data core' references
   this->coresInfos = coresInfos;

   assert (this->coresInfos);

   //Initializing the current freq
   for (unsigned int i = 0; i < coresInfos->numCores; i++)
   {
      coresInfos->allCoreDatas[i].currentFreq = ReadCurrentFreq (i);
   }
}

FreqChanger::~FreqChanger (void)
{

}

int FreqChanger::ReadCurrentFreq (unsigned int coreId)
{
   std::string curFreqString = ""; //Temp string for the current frequency
   std::istringstream iss; //Convert string to int
   std::ostringstream oss; //Convert int to string
   std::ifstream fp; //File descriptor
   int curFreq = 0;

   //Setting and Opening path leading to the cpuinfo data
   oss << "/sys/devices/system/cpu/cpu" << coreId
       << "/cpufreq/scaling_cur_freq";
   fp.open (oss.str ().c_str ());

   //Reading the current frequency
   //Paranoid
   if (fp.fail())
   {
      std::perror ("Failed to open cpufreq datafile");
      return -1;
   }
   else
   {
      //Reading the current frequency
      if (fp.good())
      {
         getline (fp, curFreqString);
         iss.str (curFreqString);
         iss >> curFreq;
      }
      //Paranoid
      else
      {
         std::cerr << "Failed to read cpufreq data" << std::endl;
         return -1;
      }
   }
   fp.close ();

   return curFreq;
}

int FreqChanger::ReadFreq (unsigned int coreId)
{
   if (coreId >= this->coresInfos->numCores)
   {
      return -1;
   }

   return this->coresInfos->allCoreDatas[coreId].currentFreq;
}

void FreqChanger::ChangeFreq (unsigned int coreId, int freqId)
{
   std::ostringstream oss;

   if (coreId >= this->coresInfos->numCores)
   {
      std::cerr << "Invalid core num for freq chande: " << coreId << std::endl;
      return;
   }

   CoreData * cd = &this->coresInfos->allCoreDatas[coreId];
   if (cd->currentFreq == freqId)
   {
      return;
   }

   if (freqId < 0 || freqId > (int) this->coresInfos->numFreqs)
   {
      std::cerr << "Invalid frequence requested: " << freqId << std::endl;
      return;
   }

   cd->freqTrack[freqId]++;

   cd->freq_fd->seekp (0, std::ios::beg);
   oss << this->coresInfos->availableFreqs[freqId];
   *cd->freq_fd << oss.str ();
   cd->freq_fd->flush();

   cd->currentFreq = this->coresInfos->availableFreqs[freqId];
}
