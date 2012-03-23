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

#include <sstream>
#include <fstream>
#include <unistd.h>
#include <set>

#include "CPUInfo.h"

CPUInfo::CPUInfo (void)
{
   std::ostringstream oss;
   std::ifstream ifs;

   // the cores which are handled in a DVFS unit
   std::set<unsigned int> treatedCores;

   // number of plugged processors
   unsigned int nbCores = sysconf (_SC_NPROCESSORS_ONLN);

   // create the unique DVFS units
   for (unsigned int i = 0; i < nbCores; i++)
   {
      // a dvfs unit already handle this processor?
      if (treatedCores.find(i) != std::set::end)
      {
         continue;
      }

      // create the dvfs unit
      this->DVFSUnits.push_back(new DVFSUnit(i), false);

      // remember the processor numbers which are handled
      oss << "/sys/devices/system/cpu/cpu" << i << "/cpufreq/affected_cpus";
      ifs.open(oss.str().c_str());

      if (!ifs)
      {
         std::cerr << "Failed to open topology information for cpu " << i << std::endl;
         exit(-1);
      }

      while (ifs.good())
      {
         unsigned int cpuId;

         ifs >> cpuId;
         treatedCores.insert(cpuId);
      }

      ifs.close();

      // early exit if we have handled all the cores
      if (treatedCores.size() == nbCores)
      {
         break;
      }
   }

   this->nbDVFSUnits = this->DVFSUnits.size();
}

CoresInfo::~CoresInfo (void)
{
   for (unsigned int i = 0; i < this->nbDVFSUnits; i++)
   {
      delete this->DVFSUnits [i];
   }
}

