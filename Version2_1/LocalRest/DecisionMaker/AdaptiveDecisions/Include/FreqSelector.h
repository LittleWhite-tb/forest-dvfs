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
 * @file FreqSelector.h
 * The FreqSelector class header is in this file
 */

#ifndef H_FREQSELECTOR
#define H_FREQSELECTOR

#include <cassert>

#include "Common.h"

/**
 * @class FreqSelector
 *
 * The helper allows one to be less sensitive to measurement noise. It consists
 * in a state based frequency selector.
 */

class FreqSelector
{
   public:
      /**
       * Constructor.
       *
       * @param nbFreqs The number of frequencies to handle.
       */
      FreqSelector (unsigned int nbFreqs);

      /**
       * Destructor.
       */
      ~FreqSelector ();

      /**
       * Add some value to a given frequency, leading it to have more chances to
       * be selected.
       *
       * @param freqId The identifier of the frequency to promote.
       */
      inline void promoteFrequency (unsigned int freqId)
      {
         assert (freqId < this->nbFreqs);

         if (this->coeffs [freqId] == FreqSelector::MAX_PROMS)
         {
            for (unsigned int i = 0; i < this->nbFreqs; i++)
            {
               if (i != freqId)
               {
                  this->coeffs [i]--;
               }
            }
         }
         else
         {
           this->coeffs [freqId]++;
         }
      }

      /**
       * Returns the best frequency to use.
       *
       * @return The best frequency to use according to the current state.
       */
      inline unsigned int getBestFrequency ()
      {
         unsigned int i;
         unsigned int max_coeff = 0;

         for (i = 0; i < this->nbFreqs; i++)
         {
            max_coeff = rest_max (this->coeffs [i], max_coeff);
         }

         for (i = 0; i < this->nbFreqs; i++)
         {
            if (this->coeffs [i] == max_coeff)
            {
               return i;
            }
         }

         return 0;
      }

   private:

      /**
       * Maximal number of successive promotions that a frequency has to have 
       * before being selected.
       */
      static const unsigned int MAX_PROMS = 2;

      /**
       * Number of frequencies in the coeff array.
       */
      unsigned int nbFreqs;

      /**
       * Current coefficient applied to every frequency.
       */
      unsigned int * coeffs;
};

#endif
