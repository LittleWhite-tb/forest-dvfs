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
 * @file Topology.h
 * The Topology class header is in this file
 */

#ifndef H_TOPOLOGY
#define H_TOPOLOGY

#include "Mode.h"
#include "DVFSUnit.h"
#include "FileUtils.h"
#include "ThreadArch.h"

#include "glog/logging.h"

#include <vector>

namespace FoREST {

/**
 * @class Topology
 * Information related to the processors on this system.
 */
class Topology
{
   public:
      /**
       * Constructor
       */
      Topology (const Mode mode, Config *config);

      /**
       * Destructor
       */
      ~Topology ();

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
      inline DVFSUnit *getDVFSUnit (unsigned int id) const
      {
         assert (id < this->nbDVFSUnits);

         return this->DVFSUnits [id];
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
            Topology::threadToCore.find (thId);

         if (it == Topology::threadToCore.end ())
         {
            unsigned int pkgId;
            std::vector<std::string> filenames;
            std::fstream ifs;
            std::ostringstream oss;
            oss << "/sys/devices/system/cpu/cpu" << thId << "/topology/core_id";
            filenames.push_back (oss.str ());

            FileUtils::tryToOpen (filenames, ifs, std::fstream::in);

            if (!(ifs >> pkgId))
            {
               LOG (FATAL) << "Cannot detect which core owns the thread "
                  << thId << std::endl;
            }

            Topology::threadToCore [thId] = pkgId;
            it = Topology::threadToCore.find (thId);
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
      static void threadIdsToCoreIds (const std::set<THREADCLASS*> &thIds,
                                     std::set<unsigned int> &coreIds)
      {
         for (std::set<THREADCLASS*>::const_iterator it = thIds.begin ();
              it != thIds.end ();
              it++)
         {
            unsigned int threadId = (*it)->getId ();
            coreIds.insert (Topology::threadIdToCoreId (threadId));
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
      std::vector<DVFSUnit*> DVFSUnits;

      /**
       * Fills the set of cores sharing the same frequency domain as the one
       * provided.
       *
       * @param cpuId ID of the core whose related cores are required.
       * @param related Output parameter, filled with the cores related to
       *  cpuId.
       */
      void getRelatedCores (unsigned int cpuId, std::set<unsigned int> &related) const;

};

} // namespace FoREST

#endif
