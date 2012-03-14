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
 @file AdaptiveDecisions.h
 @brief The AdaptiveDecisions class header is in this file
 */

#ifndef H_ADAPTIVEDECISIONS
#define H_ADAPTIVEDECISIONS

#include "DecisionMaker.h"

/**
 * @class AdaptiveDecisions
 * @brief The decision is taken adaptively here, according to the performance
 * degradation that occurs when switching the frequency. It tries to enforce a
 * maximal performance degradation.
 */
class AdaptiveDecisions : public DecisionMaker
{
   public:
      /**
       * @brief Constructor
       */
      AdaptiveDecisions (CoresInfo * coresInfo);

      /**
       * @brief Destructor
       */
      ~AdaptiveDecisions (void);

      /**
       * @brief Decides what to do for the given processor core.
       * @param core the core ID
       * @param HWCounters integer array the three hardware counters given by the profiler
       * @return A decision object where a new core frequency and sleeping 
       * window is given.
       */
      Decision takeDecision  (unsigned int core, 
                              const unsigned long long *HWCounters) const;

   private:

      /**
       * @brief The decision table maps each frequency to a range of boundness
       * values. nbFreqs entries per core.
       *
       * [[0.0, 0.7]] means that the first frequency is selected when
       * 0 <= boudness < 0.7 and the second one when 0.7 <= boundness < 1.0
       * for core 0
       */
      float **decTable;

      /**
       * @brief Number of frequencies in the table.
       */
      unsigned int nbFreqs;

      /**
       * @brief The last performance index per core (instructions retired per
       * second).
       */
      float * formerPerfIdx;

      /**
       * @brief The last boundness computed for each core.
       */
      float * formerBoundness;

};

#endif
