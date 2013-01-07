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

DVFSUnit::DVFSUnit (unsigned int id, unsigned int cpuid, bool useTB)
{
   std::ostringstream oss;
   std::ifstream ifs;
   std::ofstream ofs;
   bool hasTB;    // has TurboBoost?
   unsigned int tmp;  // multi-purpose int
   std::vector<int> tmpAllFreqs; // to temporarily store frequencies 
   
	 // detect turboboost availability
   if (system ("[ `cat /proc/cpuinfo | grep ida | wc -l` -eq 0 ]") == 0) {
      hasTB = false;
   }
   else {
      hasTB = true;
   }

   // get the latency of this unit
   oss.str (std::string (""));
   oss << "/sys/devices/system/cpu/cpu" << cpuid << "/cpufreq/cpuinfo_transition_latency";
   ifs.open (oss.str ().c_str ());
   if (!ifs)
   {
      std::cerr << "Failed to fetch the latency for cpu " << cpuid << std::endl;
      // default under linux, if it may help
      this->latency = 10000;
   }
   ifs >> this->latency;
   ifs.close ();

   // retrieve the available frequencies
   oss.str (std::string (""));
   oss << "/sys/devices/system/cpu/cpu" << cpuid << "/cpufreq/scaling_available_frequencies";
   ifs.open (oss.str ().c_str ());
   if (!ifs)
   {
      std::cerr << "Failed to fetch the available frequencies for cpu " << cpuid << std::endl;
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

	this->addCpuId (cpuid);
	this->id = id;
}

DVFSUnit::~DVFSUnit ()
{
   std::ostringstream oss;
   std::ofstream ofs;

   // restore the former governor for each CPU
	 size_t i, size = this->cpuIds.size ();
	for (i = 0; i < size; i++) {
		oss << "/sys/devices/system/cpu/cpu" << this->cpuIds [i] << "/cpufreq/scaling_governor";
  	ofs.open (oss.str ().c_str ());

		if (ofs != 0 && ofs.is_open ()) {
  		ofs << this->formerGov [i];
 	 		ofs.flush ();
			ofs.close ();
		}
		oss.str (std::string (""));

		this->freqFs [i]->close ();
		delete this->freqFs [i];
	}
   // cleanup memory
   delete [] this->freqs;
}


void DVFSUnit::addCpuId (unsigned int cpuId) {
	assert (this->cpuIds.size () == this->formerGov.size ());
	std::ostringstream oss;
	std::ifstream ifs;
	std::ofstream ofs;

	// We also store the former governor of this processor to restore it when DVFSUnit object is destroyed
	oss << "/sys/devices/system/cpu/cpu" << cpuId << "/cpufreq/scaling_governor";

	ifs.open (oss.str ().c_str ());

	if (ifs == 0 || !ifs.is_open ()) {
		std::cerr << "Error: Cannot retrieve current governor for cpu id #" << cpuId << std::endl;
		exit (EXIT_FAILURE);
	}

	std::string governor;
	ifs >> governor;

	// Push back in the list
	this->formerGov.push_back (governor);

	// Replace the current governor by userspace
	ofs.open (oss.str ().c_str ());
	ofs << "userspace";
	ofs.flush ();
	ofs.close ();

	// If it is the first cpu id to be added, then 
	// it represents the id we'll write into	
  // open the file in wich we have to write to set a frequency
	oss.str (std::string (""));
	oss << "/sys/devices/system/cpu/cpu" << cpuId << "/cpufreq/scaling_setspeed";
	std::ofstream *freqFs = new std::ofstream ();
	handleAllocation (freqFs);
	freqFs->open (oss.str ().c_str ());
	if (!freqFs->is_open ()) {
   	std::cerr << "Error: Cannot open frequency file setter. Are you root and running Linux ?" << std::endl;
   	std::cerr << "Technical Info:" << std::endl << "- DVFSunit id: " << cpuId << std::endl;
   	std::cerr << "- File path: " << oss.str () << std::endl;
   	exit (EXIT_FAILURE);
	}

	*freqFs << this->freqs [0];
	freqFs->flush ();
	this->curFreqId = 0;
	this->cpuIds.push_back (cpuId);
	this->nbCpuIds++;
	this->freqFs.push_back (freqFs);
}

void DVFSUnit::setFrequency (unsigned int freqId)
{ 
   assert (freqId < this->nbFreqs);

   //std::ostringstream oss;
		
	//std::cerr << "Setting frequency id #" << freqId << std::endl;

   // nothing to do?
   if (freqId == this->curFreqId) {
      return;
   }

	size_t i, size = this->nbCpuIds;
	for (i = 0; i < size; i++) {
		// write the correct frequency in the file
		//this->freqFs [i]->seekp (0, std::ios::beg);
  	*(this->freqFs [i]) << this->freqs [freqId];
		this->freqFs [i]->flush ();
		assert (!this->freqFs [i]->fail () && !this->freqFs [i]->bad ());
		//std::cerr << "Writing " << this->freqs [freqId] << std::endl;
	}

	this->curFreqId = freqId;
}

