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

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include <hwloc.h>

#include "DVFSUnit.h"
#include "PathBuilder.h"
#include "DataFileReader.h"

DVFSUnit::DVFSUnit (unsigned int id, const std::set<unsigned int> &cpuIds)
{
   this->cpuIds = cpuIds;
   this->id = id;

   // pick one arbitrary cpu id to represent the unit
   const unsigned int cpuID = *cpuIds.begin ();

   // get the latency of this unit
   {
      DataFileReader reader (PathBuilder<PT_CPUINFO_TRANSITION_LATENCY,PathCache>::buildPath (cpuID));
      if ( !reader.isOpen () || !reader.read (this->latency))
      {
         std::cerr << "Failed to fetch the latency for cpu " << cpuID
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
         std::cerr << "Error: Failed to read frequency list '"
         << PathBuilder<PT_CPUINFO_SCALING_AVAIL_FREQS,PathCache>::buildPath (cpuID)
         << "' for cpu #" << cpuID
         << std::endl;

         exit (EXIT_FAILURE);
      }

      reader.readLine<unsigned int>(this->freqs);
   }

   // initialize all the cores under our control
   this->takeControl (cpuIds);

   /* Now we want to take all the power economy ratio information from the
    * config file */
   {
      DataFileReader reader (PathBuilder<PT_POWER_CONFIG,PathCache>::buildPath (id));
      if (!reader.isOpen ()) 
      {
         std::cerr << "Error: Failed to fetch FoREST configuration file '" 
         << PathBuilder<PT_POWER_CONFIG,PathCache>::buildPath (id) 
         << "' for DVFS unit #" << id
         << ". You must run the offline script before using FoREST"
         << std::endl;

         exit (EXIT_FAILURE);
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
            std::cerr << "Corrupted power configuration file. Consider running again the offline power measurement"
               << std::endl;

            exit (EXIT_FAILURE);
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
   for (std::set<unsigned int>::iterator it = this->cpuIds.begin ();
        it != this->cpuIds.end ();
        it++)
   {
      ofs.open (PathBuilder<PT_CPUINFO_SCALING_GOVERNOR,PathCache>::buildPath (*it).c_str ());

		if (ofs != 0 && ofs.is_open ()) {
         ofs << this->formerGov [*it];
         ofs.flush ();
         ofs.close ();
		}
   }

   // close files
   for (unsigned int i = 0; i < this->cpuIds.size (); i++)
   {
		this->freqFs [i]->close ();
		delete this->freqFs [i];
	}
}

void DVFSUnit::takeControl (const std::set<unsigned int> &cpuIds)
{
   for (std::set<unsigned int>::iterator it = cpuIds.begin ();
        it != cpuIds.end ();
        it++)
   {
      std::string governor;
      std::ofstream ofs;

      // We store the former governor of this processor to restore it when
      // DVFSUnit object is destroyed
      {
         DataFileReader governorReader (PathBuilder<PT_CPUINFO_SCALING_GOVERNOR,PathCache>::buildPath (*it));

         if (!governorReader.isOpen () 
             || !governorReader.read<std::string>(governor))
         {
            std::cerr << "Error: fail to read current governor for cpu id #" 
               << *it << std::endl;

            governor = "ondemand";
         }
      }

      this->formerGov [*it] = governor;

	   // Replace the current governor by userspace
   	ofs.open (PathBuilder<PT_CPUINFO_SCALING_GOVERNOR,PathCache>::buildPath (*it).c_str ());
	   ofs << "userspace";
   	ofs.flush ();
	   ofs.close ();

      // open the file in wich we have to write to set a frequency
   	std::ofstream *freqFs = new std::ofstream ();
   	freqFs->open (PathBuilder<PT_CPUINFO_SCALING_SETSPEED,PathCache>::buildPath (*it).c_str ());

   	if (!freqFs->is_open ()) {
   	   std::cerr << "Error: Cannot open frequency file setter for writting." << std::endl;
      	std::cerr << "Technical Info:" << std::endl << "- DVFSunit id: "
            << *it << std::endl << "- File path: "
            << PathBuilder<PT_CPUINFO_SCALING_SETSPEED, PathCache>::buildPath (*it).c_str ()
            << std::endl;

   	   exit (EXIT_FAILURE);
      }

	   this->freqFs.push_back (freqFs);
   }
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

