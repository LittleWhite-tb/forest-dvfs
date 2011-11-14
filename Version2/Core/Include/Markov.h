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
  @file Markov.h
  @brief The Markov class header is in this file 
 */
#ifndef H_MARKOV
#define H_MARKOV

#include "Decision.h"
#include "Profiler.h"

/**
 * @class Markov
 * @brief Markov algorithm to give the action to do depending on the boundness of the program
 */
class Markov:public Decision
{
	public:
		/**
		 * @brief Constructor
		 */
		Markov (void);
		
		/**
		 * @brief Destructor
		 */
		virtual ~Markov (void);
		
		/**
		 * @brief function return 0 if no additional action is required by the profiler and 1 
		 * if context needs to be examined and what the frequency changer should do
		 * @param profiler the report to give
		 * @param data a handle to the Decision Maker context
		 **/
		virtual int decisionGiveReport (void *data, Profiler *profiler);
};

#endif
