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

#ifndef H_DECISIONMAKER
#define H_DECISIONMAKER

#include "Profilers.h"

/**< @brief profiler report that it will fill*/
typedef struct sProfReport
{
	ProfDesc prof_id; /**< @brief the id of the profiler*/
	int proc_id; /**< @brief the id of the processor*/
	ProfilerInfo data; /**< @brief struct of data relative to the profiler*/
} SProfReport;

/**
 * @brief init the decision maker
 **/
void * naiveDecisionInit (void);

/**
 * @brief free all that has to be free
 */
void naiveDecisionDestruct (void *);

/**
 * @brief function return 0 if no additional action is required by the profiler and 1 if context needs to be examined and what the frequency changer should do
 * @param profiler the report to give
 * @param data a handle to the Decision Maker context
 **/
int naiveDecisionGiveReport (void *data, SProfReport *profiler);

#endif 
