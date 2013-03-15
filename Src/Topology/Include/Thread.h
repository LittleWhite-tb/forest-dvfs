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

#include <math.h>

#include "Profiler.h"
#include "Common.h"
#include "HWCounters.h"

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
    * Array storing ipc values regarding of previous hw counters computation
    */
   float *ipc_;

   /**
    * Frequency id in the ipc table that refers the max IPC
    */
   unsigned int maxIpc_;

   /**
    * Number of available frequencies
    */

   unsigned int nbFrequencies_;
   /**
    * The profiler it is attached to
    */
   Profiler& profiler_;

   /**
    * File descriptors of the hw counters to be read by the profiler
    */
   int fd [NB_HW_COUNTERS];

   /**
    * CPU usage (between 0 and 1)
    */
   float usage_;

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
    * Reset HW Counters for a specific frequencyId
    *
    * @param frequencyId the id corresponding to the frequency currently running on the system
    */
   bool reset (unsigned int frequencyId) {
      return read (frequencyId);
   }
   
   /**
    * Get HW Counters at a specific frequency id
    *
    * @param frequencyId the id corresponding to the frequency currently running on the system
    */
   inline HWCounters& getHWCounters (unsigned int frequencyId) const{
      return hwc_ [frequencyId];
   }

   inline unsigned int getId () const{
      return id_;
   }

   /**
    * Computes and returns the CPU usage with the measurements done with the highest frequency
    */
   inline float computeUsage () {
      float res;
      // Take the highest frequency as the reference for computing thread usage
      HWCounters& hwc = hwc_ [nbFrequencies_ - 1];
      
      if (hwc.time == 0)
      {
         LOG (WARNING) << "no time elapsed since last measurement" << std::endl;
         return 0;
      }
      
      DLOG (INFO) << "active cycles: " << hwc.refCycles << " rdtsc: "
      << hwc.time << std::endl;
      
      // NOTE: RDTSC and refCycles run at the same freq
      res = hwc.refCycles / (1. * hwc.time);
      
      usage_ = rest_min (res, 1);
      //std::cerr << "Usage : " << usage_ << std::endl;
      return usage_;
   }

   /**
    * Returns the CPU usage of the current thread
    */
   inline float getUsage () const{
      return usage_;
   }

   inline bool isActive (float activityThreshold) const{
      return usage_ > activityThreshold;
   }

   /**
       * Computes the thread IPC from the hardware counters at a given frequency.
       * A number which evaluates how much the cpu is used compared to
       * the memory. Zero means that the memory is not at all the bottleneck,
       * whereas large values means that the cpu is often paused and waits for
       * the memory.
       *
       * This function MUST be called AFTER the values has been first read
       *
       * @param frequencyId The frequency id to know which hw counters values to use
       *
       * @return Whether HW counters returned irrational values (nan or 0 values)
       */
   inline bool computeIPC (unsigned int frequencyId) const{
      HWCounters& hwc = hwc_ [frequencyId];
      bool hwcPanic = false;

      if (hwc.time == 0) {
         LOG (WARNING) << "no time elapsed since last measurement" << std::endl;
         return 0;
      }
      //std::cerr << "#" << id_ << ": retired = " << hwc.retired << ", time = " << hwc.time << std::endl;
      float ipc = hwc.retired / (1. * hwc.time); 
      ipc_ [frequencyId] = ipc;
      if (ipc < 0 || isnan (ipc)) {
         hwcPanic = true;
      }
      return hwcPanic;
   }

   /**
    * Get an IPC for a specific frequency
    *
    * This function MUST be called after IPC has been computed
    *
    * @param frequencyId the wanted frequency id
    */
   inline float getIPC (unsigned int frequencyId) const{
      return ipc_ [frequencyId];
   }

   /**
    * Computes the maximum IPC among all the different frequencies
    * 
    * This function MUST be called after the IPC has been computed
    */
   inline void computeMaxIPC () {
      maxIpc_ = ipc_ [0];
      for (unsigned int i = 1; i < nbFrequencies_; i++) {
         if (maxIpc_ < ipc_ [i]) {
            maxIpc_ = i;
         }
      }
   }

   /**
    * Get the maximum IPC among all the different frequencies
    *
    * This function MUST be called after the Max IPC has been computed
    */
   inline float getMaxIPC () const{
      return ipc_ [maxIpc_];
   }
};

} // namespace FoREST

#endif

