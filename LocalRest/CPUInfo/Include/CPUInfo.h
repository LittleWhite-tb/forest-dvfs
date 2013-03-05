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
#include "DataFileReader.h"
#include "PathBuilder.h"

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
       */
      CPUInfo ();

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

      /**
       * Converts a thread ID into a core ID.
       *
       * @param thId The thread ID to convert.
       * @return The core ID of the thread.
       */
      static unsigned int threadIdToCoreId (unsigned int thId)
      {
         std::map<unsigned int, unsigned int>::iterator it = 
            CPUInfo::threadToCore.find (thId);

         if (it == CPUInfo::threadToCore.end ())
         {
            unsigned int pkgId;
            DataFileReader reader (PathBuilder<PT_TOPOLOGY_CORE_ID,PathCache>::buildPath (thId));

            if (!reader.isOpen () || !reader.read (pkgId))
            {
               std::cerr << "Cannot detect which core own the thread " << thId << std::endl;

               exit (EXIT_FAILURE);
            }

            CPUInfo::threadToCore [thId] = pkgId;
            it = CPUInfo::threadToCore.find (thId);
         }

         return it->second;
      }

      /**
       * Converts a set of thread IDs into a set of core IDs
       *
       * @param thIds The set of thread Ids to convert
       * @param coreIds Ouput parameter, filled with the list of cores to which
       *  the threads are associated.
       */
      static void threadIdsToCoreIds (const std::set<unsigned int> &thIds,
                                     std::set<unsigned int> &coreIds)
      {
         for (std::set<unsigned int>::iterator it = thIds.begin ();
              it != thIds.end ();
              it++)
         {
            coreIds.insert (CPUInfo::threadIdToCoreId (*it));
         }
      }
      
   private:

      /**
       * Maps a core ID to every thread ID. Useful to determine the core of a
       * given thread.
       */
      static std::map<unsigned int, unsigned int> threadToCore;

      /**
       * Number of independant frequency-scalable computing units (thread or
       * core with independant DVFS capacity)
       */
      unsigned int nbDVFSUnits;

      /**
       * All the available DVFS-capable computing units.
       */
      std::vector<DVFSUnit *> DVFSUnits;

      /**
       * Fills the set of cores sharing the same frequency domain as the one
       * provided.
       *
       * @param cpuId ID of the core whose related cores are required.
       * @param related Output parameter, filled with the cores related to
       *  cpuId.
       */
      void getRelatedCores (unsigned int cpuId, std::set<unsigned int> &related);

};

#endif
