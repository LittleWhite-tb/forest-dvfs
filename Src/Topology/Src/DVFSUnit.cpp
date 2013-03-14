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
#include <sstream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include <hwloc.h>

#include "glog/logging.h"

#include "DVFSUnit.h"
#include "ThreadContext.h"
#include "PathBuilder.h"
#include "DataFileReader.h"
#include "Common.h"
#include "Config.h"

namespace FoREST {

DVFSUnit::DVFSUnit (unsigned int id, const std::set<unsigned int> &cpuIds,
                    const Mode mode, Config *config) :
   decisionMaker (*this, mode, config),
   profiler (*this) 
{
   this->id = id;

   // pick one arbitrary cpu id to represent the unit
   const unsigned int cpuID = *cpuIds.begin ();

   // get the latency of this unit
   {
      DataFileReader reader (PathBuilder<PT_CPUINFO_TRANSITION_LATENCY,PathCache>::buildPath (cpuID));
      if ( !reader.isOpen () || !reader.read (this->latency))
      {
         LOG (WARNING) << "Failed to fetch the latency for cpu " << cpuID
            << std::endl;

         // default under linux, if it may help
         this->latency = 10000;
      }
   }

   // retrieve the available frequencies
   {
      DataFileReader reader (PathBuilder<PT_CPUINFO_SCALING_AVAIL_FREQS, PathCache>::buildPath (cpuID));
      if (!reader.isOpen ()) 
      {
         LOG (FATAL) << "Failed to read frequency list '"
         << PathBuilder<PT_CPUINFO_SCALING_AVAIL_FREQS,PathCache>::buildPath (cpuID)
         << "' for cpu #" << cpuID
         << std::endl;
      }

      reader.readLine<unsigned int>(this->freqs);
      std::sort (this->freqs.begin (), this->freqs.end ());
   }

   unsigned int i = 0;
   unsigned int nbFreqs = this->freqs.size ();
   for (std::set<unsigned int>::const_iterator it = cpuIds.begin ();
        it != cpuIds.end ();
        it++) {
      Thread *newThread = new Thread (i++, nbFreqs, profiler);
      this->takeControl (newThread);
      this->thread.push_back (newThread);
   }

   this->decisionMaker.setupThreads (thread);

   /* Now we want to take all the power economy ratio information from the
    * config file */
   {
      DataFileReader reader (PathBuilder<PT_POWER_CONFIG,PathCache>::buildPath (id));
      if (!reader.isOpen ()) 
      {
         LOG (FATAL) << "Failed to fetch FoREST configuration file '" 
         << PathBuilder<PT_POWER_CONFIG,PathCache>::buildPath (id) 
         << "' for DVFS unit #" << id
         << ". You must run the offline script before using FoREST"
         << std::endl;
      } 

      // skip the first line
      std::vector<float> data;
      reader.readLine (data);
      data.clear ();
      
      while (reader.readLine (data))
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
   }

   // init the status of the unit
   this->curFreqId = 0;
   this->setFrequency (0);
}

DVFSUnit::~DVFSUnit ()
{
   std::ofstream ofs;

   // restore the former governor for each CPU
   for (std::vector<Thread*>::iterator it = this->thread.begin ();
        it != this->thread.end ();
        it++)
   {
      Thread* thread = *it;
      unsigned int threadId = thread->getId ();
      ofs.open (PathBuilder<PT_CPUINFO_SCALING_GOVERNOR,PathCache>::buildPath (threadId).c_str ());

		if (ofs != 0 && ofs.is_open ()) {
         ofs << this->formerGov [threadId];
         ofs.flush ();
         ofs.close ();
		}
      delete thread;
   }

   // close files
   for (std::vector<std::ofstream*>::iterator it = this->freqFs.begin ();
        it != this->freqFs.end ();
        it++) {
   	(*it)->close ();
		delete *it;
	}
}

void DVFSUnit::takeControl (const Thread *thread)
{
   assert (thread != NULL);
   unsigned int threadId = thread->getId ();

   std::string governor;
   std::ofstream ofs;

   // We store the former governor of this processor to restore it when
   // DVFSUnit object is destroyed
   {
      DataFileReader governorReader (PathBuilder<PT_CPUINFO_SCALING_GOVERNOR,PathCache>::buildPath (threadId));

      if (!governorReader.isOpen () 
          || !governorReader.read<std::string>(governor))
      {
         LOG (WARNING) << "Failed to read current governor for cpu id #" 
            << threadId << std::endl;

         governor = "ondemand";
      }
   }

   this->formerGov [threadId] = governor;

   // Replace the current governor by userspace
	ofs.open (PathBuilder<PT_CPUINFO_SCALING_GOVERNOR,PathCache>::buildPath (threadId).c_str ());
   ofs << "userspace";
	ofs.flush ();
   ofs.close ();

   // open the file in wich we have to write to set a frequency
	std::ofstream *freqFs = new std::ofstream ();
	freqFs->open (PathBuilder<PT_CPUINFO_SCALING_SETSPEED,PathCache>::buildPath (threadId).c_str ());

	if (!freqFs->is_open ()) {
	   LOG (FATAL) << "Error: Cannot open frequency file setter for writting."
         << std::endl
         << "Technical Info:" << std::endl << "- DVFSunit id: "
         << id << std::endl << "- File path: "
         << PathBuilder<PT_CPUINFO_SCALING_SETSPEED, PathCache>::buildPath (threadId).c_str ()
         << std::endl;
   }

   this->freqFs.push_back (freqFs);
}

void DVFSUnit::setFrequency (unsigned int freqId)
{ 
   assert (freqId < this->freqs.size ());

   // nothing to do?
   if (freqId == this->curFreqId) {
      return;
   }

	for (std::vector<std::ofstream *>::iterator it = this->freqFs.begin ();
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

