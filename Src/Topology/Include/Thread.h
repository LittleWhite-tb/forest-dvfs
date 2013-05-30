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
#include <stdint.h>
#include <iomanip>

#include "Profiler.h"
#include "Common.h"
#include "rdtsc.h"

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
    * Counter monitoring the UNHALTED_REFERENCE_CYCLES hwc
    */
   Counter refCycles;

   /**
    * Counter monitoring the INST_RETIRED:ANY_P hwc
    */
   Counter retired;

   /**
    * Values of time (array for each frequency)
    */
   CounterValues *time;

   Counter execL3misses;
   CounterValues execTime;
   float execL3MissRatio;

   /**
    * Time when the last thread usage computation occured
    */
   uint64_t lastUsageComputation;
   
   /**
    * Difference between the current time and lastUsageComputation
    * It is used to know how old is the last usage computation
    */
   uint64_t diff;

   /**
    * Threshold under which usage computation is not considered as critical
    */
   const uint64_t TIME_THRESHOLD;

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
    * CPU usage (between 0 and 1)
    */
   float usage_;

public:
   /**
    * Constructor
    *
    * @param id the logical OS id of the thread
    * @param nbFrequencies number of available frequencies on the processor
    * @param profiler The profiler that will be used to monitor the thread
    */
   Thread (unsigned int id, unsigned int nbFrequencies, Profiler& profiler, uint64_t threshold);

   ~Thread ();
   
   /**
    * Resets HW Counters for a specific frequencyId
    *
    * @param frequencyId the id corresponding to the frequency currently running on the system
    */
   bool reset (unsigned int frequencyId) {
      Profiler::readTsc (this->time [frequencyId]);
      return this->profiler_.read (this->retired, frequencyId); 
   }

   bool resetExec () {
      Profiler::readTsc (this->execTime);
      return this->profiler_.read (this->execL3misses, 0);
   }
   
   /**
    * Reads HW Counters for a specific frequencyId
    *
    * @param frequencyId the id corresponding to the frequency currently running on the system
    */
   bool read (unsigned int frequencyId) { 
      Profiler::readTsc (this->time [frequencyId]); 
      return this->profiler_.read (this->retired, frequencyId);
   }

   bool readExec () {
      Profiler::readTsc (this->execTime);
      return this->profiler_.read (this->execL3misses, 0);
   }

   /**
    * Returns the id of the current thread given during object
    * construction
    */
   inline unsigned int getId () const{
      return this->id_;
   }

   /**
    * Returns whether the usage computation has to be updated
    * e.g usage computation is too old
    */
   bool hasToUpdateUsage () {
      this->diff = rdtsc () - this->lastUsageComputation;
      return this->diff > TIME_THRESHOLD;
   }

   /**
    * Computes and returns the CPU usage with the measurements done with the highest frequency
    * The update is done only if a time threshold is reached
    * Otherwise the method does nothing
    */
   inline void computeUsage () {
      if (this->hasToUpdateUsage ()) { // Only updates the usage if necessary
         float res;

         // Gather refCycles data
         this->profiler_.read (refCycles, 0); 
         uint64_t ref = this->refCycles.values [0].current;
         
         // NOTE: RDTSC and refCycles run at the same freq
         res = ref / (1. * this->diff);
         
         // Rationalize the usage if it goes beyond a ratio of 1
         this->usage_ = rest_min (res, 1);

         // Update lastUpdate to new time reference
         this->lastUsageComputation = rdtsc ();
      }
   }

   /**
    * Returns the CPU usage of the current thread
    */
   inline float getUsage () const{
      return this->usage_;
   }

   /**
    * Returns whether the thread is considered as active regarding of the given
    * threshold value
    *
    * @param the threshold to compare the current thread activity to
    */
   inline bool isActive (float threshold) {
      return this->usage_ > threshold;
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
      uint64_t retired = this->retired.values [frequencyId].current;
      uint64_t time = this->time [frequencyId].current;
      bool hwcPanic = false;

      if (time == 0) {
         LOG (WARNING) << "no time elapsed since last measurement" << std::endl;
         return false;
      }

      // Computes ipc value
      float ipc = retired / (1. * time); 
      this->ipc_ [frequencyId] = ipc;

      // Handle irrational ipc values
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
      return this->ipc_ [frequencyId];
   }

   inline void computeL3MissRatio () {
      uint64_t l3misses = this->execL3misses.values [0].current;
      uint64_t time = this->execTime.current;

      if (time == 0) {
         DLOG (WARNING) << "No time elapsed since last measurement" << std::endl;
         return;
      }

      float ratio = l3misses*1000 / (1. * time);
      this->execL3MissRatio = ratio;
   }

   inline float getL3MissRatioExec () const{
      return this->execL3MissRatio;
   }

   /**
    * Smooth the IPC to delete measurement noise
    */
   inline void smoothIPC () {
      unsigned int j = this->nbFrequencies_ - 1;
      for (unsigned int i = this->nbFrequencies_ - 1; i != 0; i--) {
         if (this->ipc_ [i] != 0) {
            if (this->ipc_ [i] > this->ipc_ [j]) {
               this->ipc_ [i] = this->ipc_ [j];
               j = i;
            }
         }
      }
   }

   inline void printIPC () {
      for (unsigned int i = 0; i < this->nbFrequencies_; i++) {
         std::cerr << std::setw (5) << this->ipc_ [i] <<  " ";
      }
      std::cerr << std::endl;
   }

   /**
    * Reset the ipc array at the beginning of the evaluation
    */
   inline void resetIPC () {
      memset (this->ipc_, 0, this->nbFrequencies_*sizeof (*this->ipc_));
   }

   /**
    * Computes the maximum IPC among all the different frequencies
    * 
    * This function MUST be called after the IPC has been computed
    */
   inline void computeMaxIPC () {
      this->maxIpc_ = this->ipc_ [0];
      for (unsigned int i = 1; i < this->nbFrequencies_; i++) {
         if (this->ipc_ [this->maxIpc_] < this->ipc_ [i]) {
            this->maxIpc_ = i;
         }
      }
   }

   /**
    * Get the maximum IPC among all the different frequencies
    *
    * This function MUST be called after the Max IPC has been computed
    */
   inline float getMaxIPC () const{
      return this->ipc_ [this->maxIpc_];
   }
};

} // namespace FoREST

#endif

