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
 * @file DVFSUnit.h
 * The DVFSUnit class header is in this file
 */

#ifndef H_DVFSUNIT
#define H_DVFSUNIT

#include <cassert>
#include <vector>

#include "Common.h"

/**
 * @class DVFSUnit
 * A core or a thread whose frequency can be set independantly from any other
 * DVFSUnit
 */
class DVFSUnit
{
   public:
      /**
       * Constructor
       *
       * @param id The id of the processor for the OS
       * @param useTB Do we consider the TurboBoost frequency or ignore it.
       */
      DVFSUnit (unsigned int id, unsigned int cpuid);

      /**
       * Destructor
       */
      ~DVFSUnit ();

      /**
       * Returns the unit identifier (name) for the OS. This is typically a
       * logical processor number.
       *
       * @return The unit number defined by the underlying OS.
       */
      inline unsigned int getOSId (unsigned idx = 0) const
      {
         assert (idx < this->cpuIds.size ());
         return this->cpuIds [idx].logicalId;
      }

      inline unsigned int getId () const {
         return id;
      }

      inline unsigned int getNbCpuIds () const{
            return this->cpuIds.size ();
      }

      /**
       * Returns the number of available frequencies.
       *
       * @return How many different frequencies can be set for this unit.
       */
      inline unsigned int getNbFreqs () const
      {
         return this->freqs.size();
      }

      /**
       * Returns what is the frequency with the given id.
       *
       * @param freqId A number between 0 and the number of frequency that
       * uniquely identifies the frequency. It is guaranteed that a lower id
       * corresponds to a lower frequency.
       */
      inline unsigned int getFrequency (unsigned int freqId) const
      {
         assert (freqId < this->freqs.size());

         return this->freqs [freqId];
      }

      /**
       * Returns the current frequency used for the unit.
       *
       * @return The id of the last required frequency.
       */
      inline unsigned int getCurFreqId () const
      {
         return this->curFreqId;
      }
      
      inline unsigned int getCurFreq () const{
      	return this->freqs [this->curFreqId];
      }

      /**
       * Sets the DVFS unit to use the frequency with the given id.
       *
       * @param freqId The frequency to use.
       */
      void setFrequency (unsigned int freqId);

      /**
       * Returns the latency required to switch the frequency.
       *
       * @return The latency (in ns) required by the unit to actually
       * change its frequency.
       */
      inline unsigned int getSwitchLatency () const
      {
         return this->latency;
      }

      // TODO comment
      const std::vector<CPUCouple>& getCpuIdList () const;

      inline unsigned int getNbPhysicalCores () const {
         return this->nbPhysicalCores;
      }

      /**
       * Return the power measurement obtained during the offline step for the
       * given frequency with the given number of active cores.
       *
       * @param freqId The frequency whose power is requested for.
       * @param nbCoresOn The number of active cores.
       *
       * @return The power consumption of the offline benchmark in the
       * environment defined by the given parameters.
       */
      inline float getPowerAt (unsigned int freqId, unsigned int nbCoresOn) const
      {
         assert (freqId < this->freqs.size());
         assert (nbCoresOn <= this->nbPhysicalCores);

         if (nbCoresOn == 0)
         {
            return 0;
         }

         return this->power [(nbCoresOn - 1) * this->freqs.size() + freqId];
      }

   private:
      /**
       * Add a core id in the list of cpus of the dvfs unit
       * Replaces its corresponding governor by userspace,
       * saving its former governor to be able to restore it
       * afterwards
       */
      void addCpuId (unsigned int cpuId);
     
     /**
      * Identifier for this unit
      */
      unsigned int id;
      
      /**
       * All the possible frequencies this thread can use. Array of size
       * nbFreqs, sorted in increasing order.
       */
      std::vector<unsigned int> freqs;

      /**
       * Id of the current frequency in use.
       */
      unsigned int curFreqId;

      /**
       * Governor in use before we take control of the DVFS unit.
       */
      std::vector<std::string> formerGov;

      /**
       * File where to write to set the frequency.
       */
      std::vector<std::ofstream*> freqFs;

      /**
       * Latency imposed to switch the frequency (nanoseconds).
       */
      unsigned int latency;

      /**
       * List of cores that are linked to the DVFS Unit by frequency
       * and handled by it 
       */
      std::vector<CPUCouple> cpuIds;
		unsigned int nbCpuIds;
      unsigned int nbPhysicalCores;
      
      /**
       * Flattened 2d array of power per given dvfsunit workload and per freq
       * 1st dimension : dvfsunit workload (in number of actives physical cores)
       * 2nd dimension : available frequencies
       */
      std::vector<float> power;
};



#endif
