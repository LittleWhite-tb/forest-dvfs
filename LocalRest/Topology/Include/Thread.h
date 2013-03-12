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
 * @file Thread.h
 * The Thread class header is in this file
 */

#ifndef H_TOPOTHREAD
#define H_TOPOTHREAD

#include "Profiler.h"

namespace FoREST {

/**
 * @class Thread
 * Represents a CPU logical unit
 * Holds the HW counter values of the logical unit it represents for each available frequency
 * Is contained in the Topology and referenced in DVFS units
 */
class Thread {
private:
   /**
    * Logical thread id
    */
   unsigned int id_;
   
   /**
    * HW Counters array representing current hwc values for each available frequency
    */
   HWCounters *hwc_;

   /**
    * HW Counters array representing previous hwc values for each available frequency
    */
   HWCounters *oldHwc_;

   /**
    * Number of available frequencies
    */

   unsigned int nbFrequencies_;
   /**
    * The profiler it is attached to
    */
   Profiler& profiler_;

   int fd [NB_HW_COUNTERS];
public:
   /**
    * Constructor
    *
    * @param Number of available frequencies on the processor
    */
   Thread (unsigned int id, unsigned int nbFrequencies, Profiler& profiler);

   ~Thread ();
   /**
    * Read HW Counters for a specific frequencyId
    *
    * @param frequencyId the id corresponding to the frequency currently running on the system
    */
   bool read (unsigned int frequencyId); 
   
   /**
    * Get HW Counters at a specific frequency id
    *
    * @param frequencyId the id corresponding to the frequency currently running on the system
    */
   inline HWCounters& getHWCounters (unsigned int frequencyId) const{
      return hwc_ [frequencyId];
   }

   inline unsigned int getId () const {
      return id_;
   }
};

} // namespace FoREST

#endif

