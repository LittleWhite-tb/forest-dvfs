/*
Copyright (C) 2011 Exascale Computing Research

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

#ifndef H_PREDICTIVEDM
#define H_PREDICTIVEDM

#include "DecisionStructures.h"

/**
 * @brief init the decision maker which uses branch prediction algorithms
 **/
void * branchDecisionInit (void);

/**
 * @brief free all that has to be free
 */
void branchDecisionDestruct (void *);

/**
 * @brief function return 0 if no additional action is required by the profiler and 1 if context needs to be examined and what the frequency changer should do
 * @param profiler the report to give
 * @param data a handle to the Decision Maker context
 * @return 0 if it has to self regulate
 **/
int branchDecisionGiveReport (void *data, SProfReport *profiler);


#endif 
