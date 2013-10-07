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

#ifndef H_FREQSELECTOR
#define H_FREQSELECTOR

#include <cassert>
#include <iostream>
#include <iomanip>

#include "Common.h"

namespace FoREST {

/**
 * @class FreqSelector
 *
 * The FreqSelector class is a helper class that weighs each frequency regarding
 * of the total time it has been applied on the machine. It helps making sure
 * we can extend FoREST's execution time depending on how often the considered
 * frequency has been chosen
 */
class FreqSelector
{
   public:
      /**
       * Constructor
       *
       * @param nbFreqs The number of frequencies to handle.
       */
      FreqSelector (unsigned int nbFreqs);
      ~FreqSelector ();

      /**
       * Add some value to a given frequency, leading it to have more chances to
       * be considered as stable. 
       *
       * @param freqId The identifier of the frequency to promote.
       * @param ratio Weight to add to the corresponding freqId 
       */
      inline void promote (unsigned int freqId, float ratio)
      {
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


}// namespace FoREST

#endif
