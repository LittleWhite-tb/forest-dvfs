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
#include "Common.h"

CPUInfo::CPUInfo ()
{
   std::ostringstream oss;

   // the cores which are handled in a DVFS unit
   std::set<unsigned int> treatedCores;
   std::set<unsigned int>::iterator it;

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

      // create the dvfs unit 
      DVFSUnit *newDVFS = new DVFSUnit (j++, i);
      handleAllocation (newDVFS);
      this->DVFSUnits.push_back (newDVFS);

      // Get all the cpu handled by the new dvfs unit to avoid creating other useless ones
      const std::vector<CPUCouple>& cpuIds = newDVFS->getCpuIdList ();
      std::vector<CPUCouple>::const_iterator it; 

      // Insert all of them in the treatedCores vector
      for (it = cpuIds.begin (); it != cpuIds.end (); it++) {
         unsigned int cpuid = (*it).logicalId;
         treatedCores.insert (cpuid);
      }

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

