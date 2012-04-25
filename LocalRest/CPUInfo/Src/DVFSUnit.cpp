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
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

#include "DVFSUnit.h"

#ifdef REST_EXTRA_LOG
#include "Logger.h"
#endif

DVFSUnit::DVFSUnit (unsigned int id, bool useTB)
{
   std::ostringstream oss;
   std::ifstream ifs;
   std::ofstream ofs;
   bool hasTB;    // has TurboBoost?
   unsigned int tmp;  // multi-purpose int
   std::vector<int> tmpAllFreqs; // to temporarily store frequencies

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

   // get the latency of this unit
   oss.str (std::string (""));
   oss << "/sys/devices/system/cpu/cpu" << id << "/cpufreq/cpuinfo_transition_latency";
   ifs.open (oss.str ().c_str ());
   if (!ifs)
   {
      std::cerr << "Failed to fetch the latency for cpu " << id << std::endl;
      // default under linux, if it may help
      this->latency = 10000;
   }
   ifs >> this->latency;
   ifs.close ();

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

   // transfert the frequencies into an array
   this->nbFreqs = tmpAllFreqs.size () - ((hasTB && !useTB) ? 1 : 0);
   this->freqs = new unsigned int [this->nbFreqs];

   for (unsigned int i = 0; i < this->nbFreqs; i++)
   {
      this->freqs [i] = tmpAllFreqs [i];
   }

   // open the file in wich we have to write to set a frequency
   oss.str (std::string (""));
   oss << "/sys/devices/system/cpu/cpu" << id << "/cpufreq/scaling_setspeed";
   this->freqFs.open (oss.str ().c_str ());
   if (!this->freqFs)
   {
      std::cerr << "Failed to open frequency setter for DVFS unit " << id << std::endl;
   }

   // initialize the frequency to the minimal freq
   this->curFreqId = 1;   // hack to ensure that the frequency will really be set
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
}

void DVFSUnit::setFrequency (unsigned int freqId)
{
   assert (freqId < this->nbFreqs);

   // nothing to do?
   if (freqId == this->curFreqId)
   {
      return;
   }
   #ifdef REST_EXTRA_LOG
   Logger & log = Logger::getLog(this->procId);
   std::stringstream ss (std::stringstream::out);
   ss << "[DVFSUnit::setFrequency] FreqId : "<<freqId<<" curFreqId : "<<this->curFreqId;
   log.logOut(ss);
   #endif
  
   // write the correct frequency in the file
   this->freqFs.seekp (0, std::ios::beg);
   this->freqFs << this->freqs [freqId];
   this->freqFs.flush ();

   this->curFreqId = freqId;
}

