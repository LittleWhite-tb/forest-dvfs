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
 @file CoresInfos.cpp
 @brief The  CoresInfos class is in this file
 */

#include <assert.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <cstdlib>

#include "CoresInfo.h"

CoresInfo::CoresInfo (void)
{
   initCpuData ();

   //Initilize array of data per core
   this->allCoreData = new CoreData [this->numCores];

   for (unsigned int i = 0; i < this->numCores; i++)
   {
      initCoreData (i, this->allCoreData [i]);
   }
}

CoresInfo::~CoresInfo (void)
{
   for (unsigned int i = 0; i < this->numCores; i++)
   {
      this->freeCoreData (i);
   }

   delete [] this->allCoreData;
}

void CoresInfo::initCpuData ()
{
   std::vector<int> tmpAllFreqs;    //this is a place to temporarily store all the values until we alloc on the heap
   std::ostringstream oss;          // Useful for converting string to int
   std::ifstream fp;                //File descriptor for reading

   // Fetch the number of available cores
   this->numCores = sysconf (_SC_NPROCESSORS_ONLN);
   assert (this->numCores > 0);

   // TurboBoost ?
   if (system ("[ `cat /proc/cpuinfo | grep ida | wc -l` -eq 0 ]") == 0)
   {
      this->hasTurboBoost = false;
   }
   else
   {
      this->hasTurboBoost = true;
   }

   // Find all the valid frequencies
   this->numFreqs = 0;
   fp.open ("/sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies");
   if (!fp)
   {
      std::cerr << "Failed to open cpufreq datafile\n" << std::endl;
   }
   else
   {
      unsigned int curFreq = 0; //Freq that we will be reading

      while (fp >> curFreq)
      {
         tmpAllFreqs.insert (tmpAllFreqs.begin(), curFreq); //Saving the freq
      }
   }
   fp.close ();
   fp.clear ();

   //Initialize tracker and available frequencies
   this->numFreqs = tmpAllFreqs.size ();
   this->availableFreqs = new unsigned int [this->numFreqs];
   for (unsigned int i = 0; i < this->numFreqs; i++)
   {
      this->availableFreqs [i] = tmpAllFreqs [i]; //Copy frequencies into data structure
   }

   this->freqMax = this->availableFreqs [0];
   this->freqMin = this->availableFreqs [this->numFreqs - 1];
}

void CoresInfo::initCoreData (unsigned int coreId, CoreData & data)
{
   std::ifstream fp; //File descriptor R
   std::fstream f; //File descriptor R/W
   std::ostringstream oss; // Usefull for converting string to int
   std::stringbuf sb;

   data.freqTrack = new unsigned long int [this->numFreqs];

   //Initialize tracker and available frequencies
   for (unsigned int i = 0; i < this->numFreqs; i++)
   {
      data.freqTrack [i] = 0;
   }

   //remember the current governor
   oss << "/sys/devices/system/cpu/cpu" << coreId << "/cpufreq/scaling_governor";
   fp.open (oss.str ().c_str ());
   if (fp.fail ())
   {
      std::cerr << "Error opening file for reading: " << oss.str ().c_str ()
                << ": ";
      std::perror ("");
   }
   else
   {
      // remember former governor
      fp >> &sb;
      data.oldGov = sb.str ();

      if (fp.fail ())
      {
         std::cerr << "Failed to read current governor" << std::endl;
      }
   }
   fp.close ();

   //set the governor to userspace
   f.open (oss.str ().c_str ());
   if (f.fail ())
   {
      std::cerr << "Error opening file for writting: " << oss.str ().c_str ()
                << ": ";
      std::perror ("");
   }
   else
   {
      //Setting to user space
      f << "userspace";
      f.flush ();

      if (f.fail ())
      {
         std::cerr << "Failed to set userspace governor" << std::endl;
      }
   }
   f.close ();

   //Open and Check the file descriptor to set the frequency
   oss.str ("");
   oss << "/sys/devices/system/cpu/cpu" << coreId
       << "/cpufreq/scaling_setspeed";

   data.freqFd = new std::ofstream ();
   data.freqFd->open (oss.str ().c_str ());

   //Paranoid
   if (data.freqFd->fail ())
   {
      std::cerr << "Error opening file: " << oss.str ().c_str () << ": ";
      std::perror ("");
   }

   // No frequency change request so far
   data.requestedFreq = 0;
}

void CoresInfo::freeCoreData (unsigned int coreId)
{
   CoreData * cd;
   std::ostringstream oss;
   std::ofstream f;

   if (coreId >= this->numCores)
   {
      return;
   }

   cd = &this->allCoreData [coreId];

   delete [] cd->freqTrack;

   cd->freqFd->close ();
   delete cd->freqFd;

   // restore the original governor
   oss << "/sys/devices/system/cpu/cpu" << coreId
       << "/cpufreq/scaling_governor";
   f.open (oss.str ().c_str ());
   if (f.fail ())
   {
      std::cerr << "Error opening file: " << oss.str ().c_str () << ": ";
      std::perror ("");
   }
   else
   {
      f << cd->oldGov;
      f.flush ();
   }
   f.close ();
}

