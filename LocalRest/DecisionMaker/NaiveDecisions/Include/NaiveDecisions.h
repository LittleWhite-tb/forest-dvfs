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
 * @file NaiveDecisions.h
 * The NaiveDecisions class header is in this file
 */

#ifndef H_NAIVEDECISIONS
#define H_NAIVEDECISIONS

#include "DecisionMaker.h"

/**
 * @class NaiveDecisions
 *
 * Simple decision making algorithm which directly selects the frequency to use
 * according to a memory boundness ratio deduced from the hardware counters.
 */
class __attribute__ ((deprecated)) NaiveDecisions : public DecisionMaker
{
   public:
      /**
       * @brief Constructor
       */
      NaiveDecisions (const DVFSUnit & unit, const Mode mode);

      /**
       * @brief Destructor
       */
      ~NaiveDecisions ();

#if FALSE
      /**
       * @brief Gives a core number and the new frequency to set
       * @param core the core ID
       * @param HWCounters integer array the three hardware counters given by the profiler
       * @param currentFreq the frequency currently set
       * @return the frequency to move to
       */
      int giveReport (unsigned int core, const unsigned long long HWCounters [3]) const;
#endif

};

#endif
