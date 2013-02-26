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
 * @file CPUInfo.h
 * The CPUInfo class header is in this file
 */

#ifndef H_CPUINFO
#define H_CPUINFO

#include "DVFSUnit.h"

#include <cassert>
#include <vector>

/**
 * @class CPUInfo
 * Information related to the processors on this system.
 */
class CPUInfo
{
   public:
      /**
       * Constructor
       * The mode represents the type of algorithm the runtime is going to use
       */
      CPUInfo (unsigned mode);

      /**
       * Destructor
       */
      ~CPUInfo ();

      /**
       * Get the number of DVFS units available.
       */
      inline unsigned int getNbDVFSUnits () const
      {
         return this->nbDVFSUnits;
      }

      /**
       * Returns the DVFS unit at the given position.
       *
       * @param id The identifier of the DVFS unit (< nbDVFSUnits)
       *
       * @return The DVFS unit with the given id.
       */
      inline DVFSUnit & getDVFSUnit (unsigned int id) const
      {
         assert (id < this->nbDVFSUnits);

         return *this->DVFSUnits [id];
      }

   private:
      /**
       * Number of independant frequency-scalable computing units (thread or
       * core with independant DVFS capacity)
       */
      unsigned int nbDVFSUnits;

      /**
       * All the available DVFS-capable computing units. Entries aligned at
       * the cache line boundary to avoid false sharing when entries are used
       * in parallel.
       */
      std::vector<DVFSUnit *> DVFSUnits;

};

#endif
