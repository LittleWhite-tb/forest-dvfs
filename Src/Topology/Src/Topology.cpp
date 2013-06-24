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
 * @file Topology.cpp
 * The Topology class is in this file
 */

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <set>

#include "Topology.h"
#include "Common.h"
#include "FileUtils.h"
#include "glog/logging.h"

namespace FoREST {

std::map<unsigned int, unsigned int> Topology::threadToCore;

Topology::Topology (const Mode mode, Config *config) {
   unsigned int DVFSid = 0;
   std::ostringstream oss;

   // the cores which are handled in a DVFS unit
   std::set<unsigned int> allCores;
   std::set<unsigned int>::iterator it;

   // number of plugged processors
   unsigned int nbCores = sysconf (_SC_NPROCESSORS_ONLN);

   // set of all the cores
   for (unsigned int i = 0; i < nbCores; i++)
   {
      allCores.insert (i);
   }

   // until we manage all the cores
   while (allCores.size () > 0)
   {
      std::set<unsigned int> related;
      unsigned int cpuId = *allCores.begin ();

      this->getRelatedCores (cpuId, related);

      // create the unique DVFS unit 
      DVFSUnit *unit = new DVFSUnit (DVFSid++, related, mode, config);
      this->DVFSUnits.push_back (unit);

      // remove the related cores from the list of cores
      for (std::set<unsigned int>::iterator it = related.begin ();
           it != related.end ();
           it++)
      {
         allCores.erase (*it);
      }
   }

   this->nbDVFSUnits = this->DVFSUnits.size ();
}

Topology::~Topology ()
{
   for (std::vector<DVFSUnit *>::iterator it = this->DVFSUnits.begin ();
         it != this->DVFSUnits.end ();
         it++)
   {
      delete *it;
   }
}

void Topology::getRelatedCores (unsigned int cpuId, std::set<unsigned int> &related) const{
   unsigned int val;
   std::vector<std::string> filenames;
   std::fstream ifs;
   std::ostringstream folder;

   folder << "/sys/devices/system/cpu/cpu" << cpuId << "/cpufreq/";
   filenames.push_back (folder.str () + "domain_cpus");
   filenames.push_back (folder.str () + "related_cpus");

   FileUtils::tryToOpen (filenames, ifs, std::fstream::in);

   while (ifs >> val) {
      related.insert (val);
   }

   ifs.close ();
}

} // namespace FoREST

