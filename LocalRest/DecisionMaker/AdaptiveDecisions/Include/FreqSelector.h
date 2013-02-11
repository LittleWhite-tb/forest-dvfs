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
#include <iostream>
#include <iomanip>

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
      inline void promote (unsigned int freqId, float ratio)
      {
         std::cout << "promote " << freqId << std::endl;
         assert (freqId < this->nbFreqs);

         if (this->coeffs [freqId] + ratio >= FreqSelector::MAX_PROMS)
         {
            // maximal mark? now decrease the other freq marks
            float dec = ratio - (FreqSelector::MAX_PROMS - this->coeffs [freqId]);
            for (unsigned int i = 0; i < this->nbFreqs; i++)
            {
               if (i != freqId)
               {
                  this->coeffs [i] = rest_max (0, this->coeffs [i] - dec);
               }
            }

            // useful when reaching the max for the first time
            this->coeffs [freqId] = FreqSelector::MAX_PROMS;
         }
         else
         {
            this->coeffs [freqId] += ratio;
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

      /**
       * Returns true if the given frequency can be safely considered as stable.
       * It is stable if its mark is above 75% of the maximal possible mark.
       *
       * @param freqId The id of the frequency to test.
       *
       * @return True if the frequency can be considered as stable.
       */
      inline bool isFreqStable (unsigned int freqId) {
         assert (freqId < this->nbFreqs);

         return this->coeffs [freqId] >= 0.75f * MAX_PROMS;
      }

      /**
       * Returns the current mark for a given frequency.
       *
       * @param freqId The id of the frequency
       *
       * @return The current mark of the frequency in the selector.
       */
      inline float getFrequencyMark (unsigned freqId) {
         assert (freqId < this->nbFreqs);

         return this->coeffs [freqId];
      }

      /**
       * Debug print function.
       */
      inline void print () {
         for (unsigned int i = 0; i < this->nbFreqs; i++) {
            std::cerr << std::setw (10) << this->coeffs [i] << " ";
         }
         std::cerr << std::endl;
      }

   private:

      /**
       * Maximal number of successive promotions that a frequency has to have
       * before being selected.
       */
      static const float MAX_PROMS = 2;

      /**
       * Number of frequencies in the coeff array.
       */
      unsigned int nbFreqs;

      /**
       * Current coefficient applied to every frequency.
       */
      float * coeffs;
};

#endif
