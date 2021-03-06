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
class DVFSUnit;

/**
 * @class Thread
 * Represents a CPU logical unit
 * Holds the HW counter values of the logical unit it represents for each available frequency
 * Is contained in the Topology and referenced in DVFS units
 */
class Thread {
protected:
   /**
    * Logical thread id
    */
   unsigned int id_;

   /**
    * Values of time (array for each frequency)
    */
   CounterValues *time_;
 
   // Last-level-of-cache ratio
   float llcRatio_;

   /**
    * Frequency sequence execution time
    */
   CounterValues execTime_;

   
   /**
    * Threshold under which thread usage is considered as neglictible
    */
   const uint64_t TIME_THRESHOLD;

   /**
    * Array storing ipc values considering previous hw counters computation
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
    * The DVFS Unit it is attached to
    */
   DVFSUnit& unit_;

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
   Thread (unsigned int id, unsigned int nbFrequencies, Profiler& profiler, DVFSUnit& unit, uint64_t threshold) :
   id_ (id), 
   TIME_THRESHOLD (threshold),
   maxIpc_ (0),
   nbFrequencies_ (nbFrequencies),
   profiler_ (profiler),
   unit_ (unit),
   usage_ (0) {
      ipc_ = new float [nbFrequencies_];
      time_ = new CounterValues [nbFrequencies_];
   }

   virtual ~Thread () {
      delete [] ipc_;
      delete [] time_;
   }
   
   /**
    * Resets HW Counters for a specific frequencyId
    *
    * @param frequencyId the id corresponding to the frequency currently running on the system
    */
   virtual bool reset (unsigned int frequencyId) = 0;

   /**
    * Reads HW Counters for a specific frequencyId
    *
    * @param frequencyId the id corresponding to the frequency currently running on the system
    */
   virtual bool read (unsigned int frequencyId) = 0;

   /**
    * Resets the thread counter values
    */
   virtual bool resetExec ()  = 0;

   /**
    * Read the thread counter values
    */
   virtual bool readExec () = 0; 

   /**
    * Returns the id of the current thread given during object
    * construction
    */
   virtual unsigned int getId () const{
      return this->id_;
   } 

   /**
    * Computes and returns the CPU usage with the measurements done with the highest frequency
    * The update is done only if a time threshold is reached
    * Otherwise the method does nothing
    */
   virtual void computeUsage () = 0;

   /**
    * Returns the CPU usage of the current thread
    */
   virtual float getUsage () const{
      return this->usage_;
   }

   /**
    * Returns whether the thread is considered as active regarding of the given
    * threshold value
    *
    * @param the threshold to compare the current thread activity to
    */
   virtual bool isActive (float threshold) {
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
   virtual bool computeIPC (unsigned int frequencyId) = 0;

   /**
    * Get an IPC for a specific frequency
    *
    * This function MUST be called after IPC has been computed
    *
    * @param frequencyId the wanted frequency id
    */
   virtual float getIPC (unsigned int frequencyId) const{
      return this->ipc_ [frequencyId];
   }
   
   /**
    * Computes the last-level-of-cache ratio
    */
   virtual void computeLLCRatio () = 0;

   /**
    * @return The Last-level-of-cache ratio
    *
    * \sa Thread::llcRatio_, Thread::computeLLCRatio
    */
   virtual float getLLCRatio () {
      return this->llcRatio_;
   }

   /**
    * Smooth the IPC to delete measurement noise
    */
   virtual void smoothIPC () {
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

   virtual void printIPC () {
      for (unsigned int i = 0; i < this->nbFrequencies_; i++) {
         std::cerr << std::setw (5) << this->ipc_ [i] <<  " ";
      }
      std::cerr << std::endl;
   }

   /**
    * Reset the ipc array at the beginning of the evaluation
    */
   virtual void resetIPC () {
      memset (this->ipc_, 0, this->nbFrequencies_*sizeof (*this->ipc_));
   }

   /**
    * Computes the maximum IPC among all the different frequencies
    * 
    * This function MUST be called after the IPC has been computed
    */
   virtual void computeMaxIPC () {
      this->maxIpc_ = 0;
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
   virtual float getMaxIPC () const{
      return this->ipc_ [this->maxIpc_];
   }
};

} // namespace FoREST

#endif

