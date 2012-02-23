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
 @file DecisionMaker.h
 @brief The DecisionMaker class header is in this file
 */

#ifndef H_DECISIONMAKER
#define H_DECISIONMAKER

#include "CoresInfo.h"

/**
 * @class DecisionMaker
 * @brief This object take rights decision were ever it is pinned (on a CPU, on a server etc...)
 * in the way to save energy. This is an abstract class see Decision algorithms as NaiveDecisions,
 * PredictiveDecisions, and Markovs for implementations.
 */
class DecisionMaker
{
	public:
		/**
		 * @brief Constructor
		 */
		DecisionMaker (CoresInfo * coresInfo);

		/**
		 * @brief Destructor
		 */
		virtual ~DecisionMaker (void);

		/**
		 * @brief Gives a core number and the new frequency to set
		 * @param core the core ID
		 * @param HWCounters integer array the three hardware counters given by the profiler
		 * @return the frequency to move to
		 */
		virtual int giveReport (unsigned int core,
				const unsigned long long HWCounters[3]) = 0;

		/**
		 * @brief compute the boundness of a program at a certain time, values shall
		 * be given by the profiler
		 * @param sqFullStall Counts cycles the Super Queue is full.
		 * Neither of the threads on this core will be able to access the uncore
		 * @param unhaltedCore elapsed cycles, correlation to time not maintained with
		 * time when frequency scaling operates
		 * @param l2Miss L2 cache misses
		 * @return the boundness of the sample
		 */
		float computeBoundness (unsigned long long sqFullStall,
            unsigned long long unhaltedCore, unsigned long long l2Miss) const;

   protected:

		//Variables
		CoresInfo * coresInfo;
};
#endif
