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

#include "CoresInfos.h"

CoresInfos::CoresInfos (void)
{
   initCpuDatas ();

   //Initilize array of data per core
   this->allCoreDatas = new CoreData[this->numCores];

   for (unsigned int i = 0; i < this->numCores; i++)
   {
      this->allCoreDatas[i] = initCoreDatas (i);
   }
}

CoresInfos::~CoresInfos (void)
{
   for (unsigned int i = 0; i < this->numCores; i++)
   {
      this->freeCoreDatas (i);
   }

   delete[] this->allCoreDatas;
}

void CoresInfos::initCpuDatas ()
{
   int i, num_core = 0, num_frequency = 0;
   std::string char_buff; //Temp string for reading
   std::vector<int> globalFrequency; //this is a place to temporarily store all the values until we alloc on the heap
   std::ostringstream oss; // Useful for converting string to int
   std::ifstream fp; //File descriptor R

   //Finding all the frequencies and record them
   fp.open (
      "/sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies");

   if (fp == NULL)
   {
      std::cerr << "Failed to open cpufreq datafile\n" << std::endl;
      return;
   }
   else
   {
      int pch = 0; //Freq that we will be reading

      while (fp >> pch)
      {
         globalFrequency.push_back (pch); //Saving the freq

         num_frequency++; //Increasing the number of frequencies
      }
   }
   fp.close ();
   fp.clear ();

   //Now find all proc available on the machine
   fp.open ("/proc/cpuinfo");

   if (fp == NULL)
   {
      std::cerr << "Failed to open the cpuinfo file\n" << std::endl;
      assert (true);
   }
   else
   {
      //if there is a proc line it means that a processor is available
      while (getline (fp, char_buff))
      {
         if (char_buff.find ("processor") != std::string::npos)
         {
            num_core++;
         }
      }
      fp.close ();
      fp.clear ();
   }

   //Paranoid
   assert (num_core > 0);

   //We have all informations so save them
   numFreqs = num_frequency;
   numCores = num_core;

   freqMax = globalFrequency[0];
   freqMin = globalFrequency[num_frequency - 1];

   //Initialize tracker and available frequencies
   this->availableFreqs = new int[num_frequency];
   for (i = 0; i < num_frequency; i++)
   {
      availableFreqs[i] = globalFrequency[i]; //Copy frequencies into data structure
   }
}

void CoresInfos::freeCoreDatas (unsigned int coreId)
{
   CoreData * cd;
   std::ostringstream oss;
   std::ofstream f;

   if (coreId >= this->numCores)
   {
      return;
   }

   cd = &this->allCoreDatas[coreId];

   delete[] cd->freqTrack;

   cd->freq_fd->close();
   delete cd->freq_fd;

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
      f << cd->old_gov;
      f.flush ();
   }
   f.close ();
}

CoreData CoresInfos::initCoreDatas (unsigned int coreId)
{
   std::ifstream fp; //File descriptor R
   std::fstream f; //File descriptor R/W
   std::ostringstream oss; // Usefull for converting string to int
   std::stringbuf sb;

   CoreData res;
   res.idCore = coreId;
   res.freqTrack = new unsigned long int[this->numFreqs];

   //Initialize tracker and available frequencies
   for (unsigned int i = 0; i < this->numFreqs; i++)
   {
      res.freqTrack[i] = 0;
   }

   //remember the current governor
   oss << "/sys/devices/system/cpu/cpu" << res.idCore
       << "/cpufreq/scaling_governor";
   f.open (oss.str ().c_str ());
   if (f.fail ())
   {
      std::cerr << "Error opening file for reading: " << oss.str ().c_str () << ": ";
      std::perror ("");
   }
   else
   {
      // remember former governor
      f >> &sb;
      res.old_gov = sb.str();

      if (f.fail())
      {
         std::cerr << "Failed to read current governor" << std::endl;
      }
   }
   f.close ();

   //set the governor to userspace
   f.open (oss.str ().c_str ());
   if (f.fail ())
   {
      std::cerr << "Error opening file for writting: " << oss.str ().c_str () << ": ";
      std::perror ("");
   }
   else
   {
      //Setting to user space
      f << "userspace";
      f.flush ();

      if (f.fail())
      {
         std::cerr << "Failed to set userspace governor" << std::endl;
      }
   }

   //Open and Check the file descriptor
   oss.str ("");
   oss << "/sys/devices/system/cpu/cpu" << res.idCore	<< "/cpufreq/scaling_setspeed";

   res.freq_fd = new std::ofstream();
   res.freq_fd->open (oss.str().c_str());

   //Paranoid
   if (res.freq_fd->fail())
   {
      std::cerr << "Error opening file: " << oss.str().c_str () << ": ";
      std::perror ("");
   }

   //We can't know for now current frequency
   res.currentFreq = -1;

   return res;
}
