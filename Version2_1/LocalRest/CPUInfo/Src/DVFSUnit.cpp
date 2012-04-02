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
 * @file DVFSUnit.cpp
 * The DVFSUnit class is in this file
 */

#include <cassert>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

#include "DVFSUnit.h"

DVFSUnit::DVFSUnit (unsigned int id, bool useTB)
{
   std::ostringstream oss;
   std::ifstream ifs;
   std::ofstream ofs;
   bool hasTB;    // has TurboBoost?
   unsigned int tmp;  // multi-purpose int
   std::vector<int> tmpAllFreqs; // to temporarily store frequencies
   unsigned int nbAvailFreqs;	      // # of frequencies exposed by the hardware
   unsigned int nbAvailNoTBFreqs;   // # of frequencies exposed except TB
   unsigned int nbRegHanFreqs;	   // # of regular frequencies we can handle (no TB)
   unsigned int nbHanFreqs;	      // # of frequencies we handle (including turboboost)
   float tmpf;	// temporary float

   this->procId = id;

   // retrieve the current governor
   oss << "/sys/devices/system/cpu/cpu" << id << "/cpufreq/scaling_governor";
   ifs.open (oss.str ().c_str ());

   if (!ifs)
   {
      std::cerr << "Failed to retrieve current governor for cpu " << id << std::endl;
      exit (-1);
   }

   ifs >> this->formerGov;
   ifs.close ();

   // set the userspace governor
   ofs.open (oss.str ().c_str ());
   ofs << "userspace";
   ofs.flush ();
   ofs.close ();

   // detect turboboost availability
   if (system ("[ `cat /proc/cpuinfo | grep ida | wc -l` -eq 0 ]") == 0)
   {
      hasTB = false;
   }
   else
   {
      hasTB = true;
   }

   // retrieve the available frequencies
   oss.str (std::string (""));
   oss << "/sys/devices/system/cpu/cpu" << id << "/cpufreq/scaling_available_frequencies";
   ifs.open (oss.str ().c_str ());
   if (!ifs)
   {
      std::cerr << "Failed to fetch the available frequencies for cpu " << id << std::endl;
      exit (-1);
   }

   while (ifs >> tmp)
   {
      tmpAllFreqs.insert (tmpAllFreqs.begin (), tmp); //Saving the freq
   }
   ifs.close ();

   // bound the number of frequencies
   nbAvailFreqs = tmpAllFreqs.size ();

   if (hasTB)
   {
      nbAvailNoTBFreqs = nbAvailFreqs - 1;
      nbRegHanFreqs = rest_min (nbAvailFreqs - 1, DVFSUnit::MAX_NB_FREQS);
      nbHanFreqs = nbRegHanFreqs + (useTB ? 1 : 0);
   }
   else
   {
      nbAvailNoTBFreqs = nbAvailFreqs;
      nbRegHanFreqs = rest_min (nbAvailFreqs, DVFSUnit::MAX_NB_FREQS);
      nbHanFreqs = nbRegHanFreqs;
   }

   // transfert the required amount of frequencies into an array
   this->nbFreqs = nbHanFreqs;
   this->freqs = new unsigned int [nbHanFreqs];

   tmpf = 0;
   for (unsigned int i = 0;
         i < nbRegHanFreqs;
         i++)
   {
      this->freqs [i] = tmpAllFreqs[(unsigned int) roundf(tmpf)];

      tmpf += (float) nbAvailNoTBFreqs / DVFSUnit::MAX_NB_FREQS;
   }

   // always consider the specific turboboost frequency
   if (hasTB && useTB)
   {
      this->freqs [this->nbFreqs - 1] = tmpAllFreqs [nbAvailFreqs - 1];
   }

   // initialize the frequency tracking
   this->freqSwitch = new unsigned long int [this->nbFreqs];

   // open the file in wich we have to write to set a frequency
   oss.str (std::string (""));
   oss << "/sys/devices/system/cpu/cpu" << id << "/cpufreq/scaling_setspeed";
   this->freqFs.open (oss.str ().c_str ());

   // initialize the frequency to the minimal freq
   this->curFreqId = 1;   // hack to ensure that the frequency will be set
   this->setFrequency (0);
}

DVFSUnit::~DVFSUnit ()
{
   std::ostringstream oss;
   std::ofstream ofs;

   // restore the former governor
   oss << "/sys/devices/system/cpu/cpu" << this->procId << "/cpufreq/scaling_governor";
   ofs.open (oss.str ().c_str ());
   ofs << this->formerGov;
   ofs.flush ();
   ofs.close ();

   // close the frequency file
   this->freqFs.close ();

   // cleanup memory
   delete [] this->freqs;
   delete [] this->freqSwitch;
}

void DVFSUnit::setFrequency (unsigned int freqId)
{
   assert (freqId < this->nbFreqs);

   // nothing to do?
   if (freqId == this->curFreqId)
   {
      return;
   }

   // write the correct frequency in the file
   this->freqFs.seekp (0, std::ios::beg);
   this->freqFs << this->freqs [freqId];
   this->freqFs.flush ();

   this->curFreqId = freqId;

   // track the frequency switch
   // this is performed without any synchronization, assuming that the elements
   // of the array are aligned at a 8 bytes boundary which is always the case
   // considering the data type and the memory allocation requirements.
   // We exploit here the fact that x86(_64) cpus provide atomic operations on
   // memory under this constraint.
   this->freqSwitch [freqId] ++;
}

