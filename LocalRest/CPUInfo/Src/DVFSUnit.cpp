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

DVFSUnit::DVFSUnit (unsigned int id, unsigned int cpuid)
{
   std::ostringstream oss;
   std::ifstream ifs;
   std::ofstream ofs;   
   unsigned int tmp;  // multi-purpose int

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
   oss.str(""); // Remove previous use
   oss << "offline/power_" << cpuid << ".cfg";
   
   ifs.open (oss.str ().c_str ());
   if (!ifs) {
      std::cerr << "Error: Failed to fetch FoREST configuration file '" << oss.str() << "' for cpuid #"
      << cpuid << ". You must run the offline script before using FoREST"
      << std::endl;
      exit (EXIT_FAILURE);
   } 

   /* Read config file first line */
   std::string line;
   std::getline (ifs, line, '\n');
   std::istringstream iss(line);
   while (iss >> tmp) {
      assert (!iss.fail ());
      this->freqs.push_back (tmp);
   } 

   // Add the current cpu as the first cpu id in the DVFS list
	this->addCpuId (cpuid); 

  // Retrieve all the cores that are linked to the main cpuid by frequency
   std::ostringstream rel;
   rel << "/sys/devices/system/cpu/cpu" << cpuid << "/cpufreq/related_cpus";
   std::ifstream relIfs;
   relIfs.open (rel.str ().c_str ());
   if (!relIfs) {
      std::cerr << "Error: Failed to open topology information for cpu " << cpuid << std::endl;
      exit (EXIT_FAILURE);
   }

   unsigned int subCpuId;
   // Add all others cpu ids in the DVFS list as related cpus
   while (relIfs >> subCpuId) {
      if (subCpuId != cpuid) {
         this->addCpuId (subCpuId); 
      }
   }
   relIfs.close ();

   /* Go fetch physical core information and store it in this->cores eventually */
   hwloc_topology_t topology;
   hwloc_obj_t obj;
   
   /* Initialize hwloc library */
   if (hwloc_topology_init (&topology) == -1) {
      std::cerr << "Error: Cannot initialize Hwloc topology" << std::endl;
      exit (EXIT_FAILURE);
   }

   hwloc_topology_ignore_type (topology, HWLOC_OBJ_CACHE);

   if (hwloc_topology_load (topology) == -1) {
      std::cerr << "Error: Cannot load Hwloc topology" << std::endl;
      exit (EXIT_FAILURE);
   }
 
   /* We should have a certain kind of hwloc topology tree... */
   assert (hwloc_get_depth_type (topology, 1) == HWLOC_OBJ_SOCKET);
   assert (hwloc_get_depth_type (topology, 2) == HWLOC_OBJ_CORE);

   this->nbPhysicalCores = hwloc_get_nbobjs_by_depth (topology, 2);
   /* For each physical core, we're going to see their children (eg the logical units)
    * and see which logical id corresponds to which physical id
    */
   for (unsigned int j = 0; j < this->nbPhysicalCores; j++) { 
      obj = hwloc_get_obj_by_depth (topology, 2, j);
      for (unsigned k = 0; k < obj->arity; k++) {
         for (unsigned int i = 0; i < this->nbCpuIds; i++) {
            if (obj->children [k]->os_index == this->cpuIds [i].logicalId) {
               this->cpuIds [i].physicalId = obj->os_index;
            }
         }
      }
   }

   // We don't need hwloc anymore, bybye
   hwloc_topology_destroy (topology);

   /* Now we want to take all the power economy ratio information from the config file */
   this->power.resize(this->nbPhysicalCores * this->freqs.size(),0);
  
   // Go through all the other lines
   float ratio;
   unsigned int i = 0;
   while (ifs >> ratio) { 
      assert (i < this->nbPhysicalCores * this->freqs.size());
      this->power [i++] = ratio;
      assert (!ifs.fail ());
   }
   ifs.close ();
	
   this->id = id;
}

DVFSUnit::~DVFSUnit ()
{
   std::ostringstream oss;
   std::ofstream ofs;

   // restore the former governor for each CPU
   size_t i, size = this->cpuIds.size ();
	for (i = 0; i < size; i++) {
		oss << "/sys/devices/system/cpu/cpu" << this->cpuIds [i].logicalId << "/cpufreq/scaling_governor";
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
}

const std::vector<CPUCouple>& DVFSUnit::getCpuIdList () const{
   return this->cpuIds;
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

   assert (this->freqs.size() != 0);
	*freqFs << this->freqs [0];
	freqFs->flush ();
	this->curFreqId = 0;
   CPUCouple cpuc = {0, cpuId};
	this->cpuIds.push_back (cpuc);
	this->nbCpuIds++;
	this->freqFs.push_back (freqFs);
}

void DVFSUnit::setFrequency (unsigned int freqId)
{ 
   assert (freqId < this->freqs.size());

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

