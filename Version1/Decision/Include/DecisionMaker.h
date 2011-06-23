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

#ifndef DECISIONMAKER_H
#define DECISIONMAKER_H

#include <Profilers.h>
#include <Frequency_Mod.h>
#include <define.h>

/**TODO doc*/
typedef struct sProfReport
{
	ProfDesc prof_id;
	int proc_id;
	ProfilerInfo data;
} SProfReport;


/**
 * @brief init the decision maker
 **/
void* decisionInit (void);

/**
 * @brief free all that has to be free
 */
void decisionDestruct(void);

/**
 * @brief function return 0 if no additional action is required by the profiler and 1 if context needs to be examined and what the frequency changer should do
 * @param profiler fill the report to know what we are talking about
 **/
int DecisionGiveReport (void *data, SProfReport *profiler);

#endif 
