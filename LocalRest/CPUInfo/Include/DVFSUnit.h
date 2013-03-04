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
 * The DVFSUnit class reprensents all the cores sharing the same frequency 
 * setting.
 */

#ifndef H_DVFSUNIT
#define H_DVFSUNIT

#include <algorithm>
#include <cassert>
#include <map>
#include <set>
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
       * @param id The unique ID for this unit. Must be the same as the id 
       *  used when the offline power information was generated.
       * @param cpuIds Set of CPU ids handled by this unit.
       */
      DVFSUnit (unsigned int id, const std::set<unsigned int> &cpuIds);

      /**
       * Destructor
       */
      ~DVFSUnit ();

      /**
       * Returns the id of this DVFS unit.
       *
       * @return The unique ID for this DVFS unit.
       */
      inline unsigned int getId() const
      {
         return this->id;
      }

      /**
       * Returns the set of CPUs under our control.
       *
       * @return The IDs of the threads this instance controls.
       */
      inline const std::set<unsigned int> getThreads() const
      {
         return this->cpuIds;
      }

      /**
       * Returns the number of available frequencies.
       *
       * @return How many different frequencies can be set for this unit.
       */
      inline unsigned int getNbFreqs () const
      {
         return this->freqs.size ();
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
         assert (freqId < this->freqs.size ());

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
      
      /**
       * Returns the current frequency used.
       *
       * @return The current frequency requested for the whole DVFS unit.
       */
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
       * Return the power measurement obtained during the offline step for the
       * given frequency with the given number of active cores.
       *
       * @param freqId The frequency whose power is requested for.
       * @param nbCoresOn How many physical CPU cores are active
       *
       * @return The power consumption of the offline benchmark in the
       * environment defined by the given parameters.
       */
      inline float getPowerAt (unsigned int freqId, unsigned int nbCoresOn) const
      {
         assert (freqId < this->freqs.size ());

         if (nbCoresOn == 0)
         {
            return 0;
         }
         
         return this->power [(nbCoresOn - 1) * this->freqs.size () + freqId];
      }

   private:

      /**
       * DVFS unit id.
       */
      unsigned int id;

      /**
       * The set of CPU ids under our control.
       */
      std::set<unsigned int> cpuIds;

      /**
       * All the possible frequencies this unit can use.
       * Sorted in increasing order.
       */
      std::vector<unsigned int> freqs;

      /**
       * Id of the current frequency in use.
       */
      unsigned int curFreqId;

      /**
       * Governor in use before we take control of the DVFS unit.
       */
      std::map<unsigned int, std::string> formerGov;

      /**
       * File where to write to set the frequency.
       */
      std::vector<std::ofstream*> freqFs;

      /**
       * Latency imposed to switch the frequency (nanoseconds).
       */
      unsigned int latency;

      /**
       * Flattened 2d array of power per given dvfsunit workload and per freq
       * 1st dimension : dvfsunit workload (in number of active physical cores)
       * 2nd dimension : available frequencies
       * 
       * To access the power consumption for A active cores at frequency F,
       * reach power[A * nbFreqs + F]
       */
      std::vector<float> power;

      /**
       * Initializes the given CPUs to begin controlling them.
       *
       * @param cpuIds The CPUs to control.
       */
      void takeControl (const std::set<unsigned int> &cpuIds);
};



#endif
