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

DVFSUnit::DVFSUnit (unsigned int id, unsigned int cpuid)
                // default under linux, if it may help
   :latency(10000),nbCpuIds(0)
{
   std::ifstream ifs;
   std::ofstream ofs;   

   // get the latency of this unit
   {
      DataFileReader reader(PathBuilder<PT_CPUINFO_TRANSITION_LATENCY,PathCache>::buildPath(id));
      if ( reader.isOpen() )
      {
         if ( reader.read(this->latency) == false )
         {
            std::cerr << "Failed to read latency for cpu " << cpuid << std::endl;
         }
      }
      else
      {
         std::cerr << "Failed to fetch the latency for cpu " << cpuid << std::endl;
      }
   }

   // retrieve the available frequencies
   {
      DataFileReader reader(PathBuilder<PT_POWER_CONFIG,PathCache>::buildPath(cpuid));
      if (!reader.isOpen()) 
      {
         std::cerr << "Error: Failed to fetch FoREST configuration file '" << PathBuilder<PT_POWER_CONFIG,PathCache>::buildPath(cpuid) << "' for cpuid #"
         << cpuid << ". You must run the offline script before using FoREST"
         << std::endl;
         exit (EXIT_FAILURE);
      } 

      reader.readLine<unsigned int>(this->freqs);
   }

   // Add the current cpu as the first cpu id in the DVFS list
	this->addCpuId (cpuid); 

   // Retrieve all the cores that are linked to the main cpuid by frequency
   {
      DataFileReader reader(PathBuilder<PT_CPUINFO_RELATED_CPU,PathCache>::buildPath(cpuid));
      if (!reader.isOpen()) {
         std::cerr << "Error: Failed to open topology information for cpu " << cpuid << std::endl;
         exit (EXIT_FAILURE);
      }

      unsigned int subCpuId;
      // Add all others cpu ids in the DVFS list as related cpus
      while (reader.read(subCpuId)) 
      {
         if (subCpuId != cpuid) 
         {
            this->addCpuId (subCpuId); 
         }
      }
   }

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
  
   {
      DataFileReader reader(PathBuilder<PT_POWER_CONFIG,PathCache>::buildPath(cpuid));
      if (!reader.isOpen()) 
      {
         std::cerr << "Error: Failed to fetch FoREST configuration file '" << PathBuilder<PT_POWER_CONFIG,PathCache>::buildPath(cpuid) << "' for cpuid #"
         << cpuid << ". You must run the offline script before using FoREST"
         << std::endl;
         exit (EXIT_FAILURE);
      } 

      std::vector<float> datas;
      reader.readLine(datas);
      
      float ratio;
      unsigned int i = 0;
      while (reader.read(ratio))
      {
         assert (i < this->nbPhysicalCores * this->freqs.size());
         this->power [i++] = ratio;
      }
      
      if ( i != (this->nbPhysicalCores * this->freqs.size()))
      {
         std::cerr << "Something goes wrong reading the FoREST configuration file" << std::endl;
         std::cerr << "Some values are missing" << std::endl;
      }
   }
	
   this->id = id;
}

DVFSUnit::~DVFSUnit ()
{
   std::ofstream ofs;

   // restore the former governor for each CPU
   size_t i, size = this->cpuIds.size ();
	for (i = 0; i < size; i++) {
      ofs.open (PathBuilder<PT_CPUINFO_SCALING_GOVERNOR,PathCache>::buildPath(this->cpuIds [i].logicalId).c_str ());

		if (ofs != 0 && ofs.is_open ()) {
         ofs << this->formerGov [i];
         ofs.flush ();
         ofs.close ();
		}

		this->freqFs [i]->close ();
		delete this->freqFs [i];
	}
}

const std::vector<CPUCouple>& DVFSUnit::getCpuIdList () const{
   return this->cpuIds;
}

void DVFSUnit::addCpuId (unsigned int cpuId) {
	assert (this->cpuIds.size () == this->formerGov.size ());
	
   std::string governor;
	std::ofstream ofs;

	// We also store the former governor of this processor to restore it when DVFSUnit object is destroyed
   {
      DataFileReader governorReader(PathBuilder<PT_CPUINFO_SCALING_GOVERNOR,PathCache>::buildPath(cpuId));

      if (!governorReader.isOpen()) {
         std::cerr << "Error: Cannot retrieve current governor for cpu id #" << cpuId << std::endl;
         exit (EXIT_FAILURE);
      }

      if ( governorReader.read<std::string>(governor) == false )
      {
         std::cerr << "Error: fail to read current governor for cpu id #" << cpuId << std::endl;
         exit (EXIT_FAILURE);
      }
   }

	// Push back in the list
	this->formerGov.push_back (governor);

	// Replace the current governor by userspace
	ofs.open (PathBuilder<PT_CPUINFO_SCALING_GOVERNOR,PathCache>::buildPath(cpuId).c_str ());
	ofs << "userspace";
	ofs.flush ();
	ofs.close ();

	// If it is the first cpu id to be added, then 
	// it represents the id we'll write into	
  // open the file in wich we have to write to set a frequency
	std::ofstream *freqFs = new std::ofstream ();
	handleAllocation (freqFs);
	freqFs->open (PathBuilder<PT_CPUINFO_SCALING_SETSPEED,PathCache>::buildPath(cpuId).c_str ());
	if (!freqFs->is_open ()) {
   	std::cerr << "Error: Cannot open frequency file setter. Are you root and running Linux ?" << std::endl;
   	std::cerr << "Technical Info:" << std::endl << "- DVFSunit id: " << cpuId << std::endl;
   	std::cerr << "- File path: " << PathBuilder<PT_CPUINFO_SCALING_SETSPEED,PathCache>::buildPath(cpuId).c_str() << std::endl;
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

