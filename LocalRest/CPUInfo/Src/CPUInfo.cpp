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
 * @file CPUInfo.cpp
 * The CPUInfo class is in this file
 */

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <set>

#include "CPUInfo.h"

CPUInfo::CPUInfo ()
{
   std::ostringstream oss;
   std::ifstream ifs;

   // the cores which are handled in a DVFS unit
   std::set<unsigned int> treatedCores;
   std::set<unsigned int>::iterator it;
	 unsigned int cpuId;

   // number of plugged processors
   unsigned int nbCores = sysconf (_SC_NPROCESSORS_ONLN);

   // create the unique DVFS units
	 unsigned int j = 0;
   for (unsigned int i = 0; i < nbCores; i++)
   {
      // a dvfs unit already handle this processor?
      bool coreIsAlreadyHandled = false;
      // We loop through the structure containing all the cores already treated
      for (it = treatedCores.begin (); it != treatedCores.end (); it++) {
          if (*it == i) {
            coreIsAlreadyHandled = true;
            break;
          }
      }
      // Core already handled -> go to the next iteration
      if (coreIsAlreadyHandled) {
        continue;
      }

			//std::cerr << "Core #" << i << " is a DVFSUnit" << std::endl;

      // create the dvfs unit
      DVFSUnit *newDVFS = new DVFSUnit (j++, i, false);
      handleAllocation (newDVFS);
      this->DVFSUnits.push_back (newDVFS);

      // remember the processor numbers which are handled
      oss.str ("");
      oss << "/sys/devices/system/cpu/cpu" << i << "/cpufreq/related_cpus";
      ifs.open (oss.str ().c_str ());

      if (!ifs)
      {
         std::cerr << "Failed to open topology information for cpu " << i << std::endl;
         exit (-1);
      }

      while (ifs >> cpuId)
      {
         // We don't want to add every peer except from the dvfs id itself
         if (cpuId != i) {
						//std::cerr << "Core #" << i << " is handling cpu " << cpuId << std::endl;
            newDVFS->addCpuId (cpuId);
            treatedCores.insert (cpuId);
         }
      }

      ifs.close ();

      // early exit if we have handled all the cores
      if (treatedCores.size () == nbCores)
      {
         break;
      }
   }

   this->nbDVFSUnits = this->DVFSUnits.size ();
}

CPUInfo::~CPUInfo ()
{
   for (unsigned int i = 0; i < this->nbDVFSUnits; i++)
   {
      delete this->DVFSUnits [i];
   }
}

