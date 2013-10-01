/*
 * FoREST - Reactive DVFS Control for Multicore Processors
 * Copyright (C) 2013 Universite de Versailles
 * Copyright (C) 2011-2012 Exascale Research Center
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file DVFSUnit.cpp
 * The DVFSUnit class is in this file
 */

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "glog/logging.h"
#include "ThreadArch.h"
#include "DVFSUnit.h"
#include "ThreadContext.h"
#include "FileUtils.h"
#include "Common.h"
#include "Config.h"


namespace FoREST {

DVFSUnit::DVFSUnit (unsigned int id, const std::set<unsigned int> &cpuIds,
                    const Mode mode, Config *config) :
   profiler (),
   freqFs (cpuIds.size ())
{
   this->id = id;
   std::fstream ifs;
   std::ostringstream folder;

   // pick one arbitrary cpu id to represent the unit
   const unsigned int cpuID = *cpuIds.begin ();
   folder << "/sys/devices/system/cpu/cpu" << cpuID << "/cpufreq/";

   // get the latency of this unit
   {
      std::vector<std::string> filenames;
      filenames.push_back (folder.str () + "cpuinfo_transition_latency");

      FileUtils::tryToOpen (filenames, ifs, std::fstream::in);

      if (!(ifs >> this->latency))
      {
         LOG (WARNING) << "Failed to fetch the latency for cpu " << cpuID
            << std::endl;

         // default under linux, if it may help
         this->latency = 10000;
      }
      ifs.close ();
   }

   // retrieve the available frequencies
   {
      std::vector<std::string> filenames;
      filenames.push_back (folder.str () + "scaling_available_frequencies");
      unsigned int val;

      FileUtils::tryToOpen (filenames, ifs, std::fstream::in);
     
      while (ifs >> val) {
         this->freqs.push_back (val);
      }
      std::sort (this->freqs.begin (), this->freqs.end ());

      ifs.close ();
   }

   unsigned int i = 0;
   unsigned int nbFreqs = this->freqs.size ();
   for (std::set<unsigned int>::const_iterator it = cpuIds.begin ();
        it != cpuIds.end ();
        it++) {
      // Number of cycles needed to achieve ~100ms
      uint64_t cyclesFor100ms = this->freqs [nbFreqs - 1] * 100;
      THREADCLASS *newThread = new THREADCLASS (i++, nbFreqs, profiler, cyclesFor100ms);
      this->takeControl (newThread->getId ());
      this->thread.push_back (newThread);
   }

   this->decisionMaker = new DecisionMaker (this, mode, config, thread);

   /* Now we want to take all the power economy ratio information from the
    * config file */
   {
      std::vector<std::string> filenames;
      std::ostringstream oss;
      oss << "offline/power_" << cpuID << ".cfg";
      filenames.push_back (oss.str ());

      FileUtils::tryToOpen (filenames, ifs, std::fstream::in);

      // skip the first line
      {
         std::string dummy;
         std::getline (ifs, dummy, '\n');
      }

      std::vector<float> data;
      while (FileUtils::readLine<float> (data, ifs))
      {
         // skip empty lines
         if (data.size () == 0)
         {
            continue;
         }

         for (std::vector<float>::iterator it = data.begin ();
              it != data.end ();
              it++)
         {
            this->power.push_back (*it);
         }

         if (data.size () != this->freqs.size ())
         {
            LOG (FATAL) << "Corrupted power configuration file. Consider running again the offline power measurement"
               << std::endl;
         }

         data.clear ();
      }

      ifs.close ();
   }

   // init the status of the unit
   this->curFreqId = 0;
   this->setFrequency (0);
}

void DVFSUnit::handOver (unsigned int threadId) {
   std::fstream fs;
   std::vector<std::string> filenames;
   std::ostringstream oss;

   oss << "/sys/devices/system/cpu/cpu" << threadId << "/cpufreq/scaling_governor";
   filenames.push_back (oss.str ());

   FileUtils::tryToOpen (filenames, fs, std::fstream::out);
  
   assert (fs.is_open ());
   fs << this->formerGov [threadId];
   fs.flush ();
   fs.close ();
}

DVFSUnit::~DVFSUnit ()
{
   // restore the former governor for each CPU
   for (std::vector<THREADCLASS*>::iterator it = this->thread.begin ();
        it != this->thread.end ();
        it++)
   {
      // Restore previous data to the thread
      std::cerr << "I HAND OVER TO " << this->formerGov [(*it)->getId ()] << std::endl;
      this->handOver ((*it)->getId ());
      delete *it;
   }

   delete this->decisionMaker;

   // close files
   for (std::vector<std::fstream*>::iterator it = this->freqFs.begin ();
        it != this->freqFs.end ();
        it++) {
   	(*it)->close ();
		delete *it;
	}
}

void DVFSUnit::takeControl (unsigned int threadId)
{
   std::string governor;
   std::fstream fs;


   // We store the former governor of this processor to restore it when
   // DVFSUnit object is destroyed
   {
      std::vector<std::string> filenames;
      std::ostringstream file;
      file << "/sys/devices/system/cpu/cpu" << threadId << "/cpufreq/scaling_governor";
      filenames.push_back (file.str ());
      
      FileUtils::tryToOpen (filenames, fs, std::fstream::in);

      std::vector<std::string> v;
      FileUtils::readLine <std::string> (v, fs); 
      this->formerGov [threadId] = v[0];

      fs.close ();
   } 

   {
      std::vector<std::string> filenames;
      std::ostringstream oss;
      oss << "/sys/devices/system/cpu/cpu" << threadId << "/cpufreq/scaling_governor";
      filenames.push_back (oss.str ());
      FileUtils::tryToOpen (filenames, fs, std::fstream::out);

      // Replace the current governor by userspace
      fs << "userspace";
	   fs.flush ();
      fs.close ();
   }

   // open the file in wich we have to write to set a frequency
   {
      std::vector<std::string> filenames;
      std::ostringstream oss;
      oss << "/sys/devices/system/cpu/cpu" << threadId << "/cpufreq/scaling_setspeed";
      filenames.push_back (oss.str ());
      std::fstream *freqFs = new std::fstream ();
      
      FileUtils::tryToOpen (filenames, *freqFs, std::fstream::out);
      this->freqFs [threadId] = freqFs;
      assert (this->freqFs [threadId]->is_open ());
   }
}

void DVFSUnit::setFrequency (unsigned int freqId)
{ 
   assert (freqId < this->freqs.size ());

   // nothing to do?
   if (freqId == this->curFreqId) {
      return;
   }

	for (std::vector<std::fstream *>::iterator it = this->freqFs.begin ();
        it != this->freqFs.end ();
        it++)
   {
		// write the correct frequency in the file
  	   *(*it) << this->freqs [freqId];
		(*it)->flush ();

		assert (!(*it)->fail () && !(*it)->bad ());
	}

	this->curFreqId = freqId;
}

} // namespace FoREST

