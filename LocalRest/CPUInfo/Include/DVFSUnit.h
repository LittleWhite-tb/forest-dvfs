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
 * @file DVFSUnit.h
 * The DVFSUnit class header is in this file
 */

#ifndef H_DVFSUNIT
#define H_DVFSUNIT

#include <cassert>
#include <string>
#include <fstream>
#include <stdint.h>
#include <iostream>
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
      DVFSUnit (unsigned int id, unsigned int cpuid, bool useTB);

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
         return this->cpuIds[idx];
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
         return this->nbFreqs;
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
 	// std::cerr <<"["<< this->getOSId()<<"]" <<  "From DVFS freId : "<< freqId << " nbfreq : " << this->nbFreqs <<std::endl;
         assert (freqId < this->nbFreqs);

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

			/**
			 * Add a core id in the list of cpus of the dvfs unit
			 * Replaces its corresponding governor by userspace,
			 * saving its former governor to be able to restore it
			 * afterwards
			 */
      void addCpuId (unsigned int cpuId);	
   private:
	 		// TODO comment
	 		unsigned int id;
      /**
       * The number of frequencies which this unit can use.
       */
      unsigned int nbFreqs;

      /**
       * All the possible frequencies this thread can use. Array of size
       * nbFreqs, sorted in increasing order.
       */
      unsigned int * freqs;

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
      std::vector<unsigned int> cpuIds;
			unsigned int nbCpuIds;
};



#endif
