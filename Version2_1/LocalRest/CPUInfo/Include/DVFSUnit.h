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
      DVFSUnit (unsigned int id, bool useTB);

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
      inline unsigned int getOSId () const
      {
         return this->procId;
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
         assert (freqId < this->nbFreqs);

         return this->freqs [freqId];
      }

      /**
       * Returns the current frequency used for the unit.
       *
       * @return The id of the last required frequency.
       */
      inline unsigned int getFrequency () const
      {
         return this->curFreqId;
      }

      /**
       * Sets the DVFS unit to use the frequency with the given id.
       *
       * @param freqId The frequency to use.
       */
      void setFrequency (unsigned int freqId);

   private:

      /**
       * The id of the processor we are handling.
       */
      unsigned int procId;

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
      std::string formerGov;

      /**
       * File where to write to set the frequency.
       */
      std::ofstream freqFs;
};



#endif
