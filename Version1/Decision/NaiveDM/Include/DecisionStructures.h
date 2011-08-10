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

#ifndef H_DECISION_STRUCTURES
#define H_DECISION_STRUCTURES

#include "Frequency_Mod.h"
#include "Profilers.h"
#include "Markov.h"

/**< @brief profiler report that it will fill*/
typedef struct sProfReport
{
	EProfType prof_id; /**< @brief the id of the profiler*/
	int proc_id; /**< @brief the id of the processor*/
	ProfilerInfo data; /**< @brief struct of data relative to the profiler*/
} SProfReport;

/**
 * @struct saveData
 * @brief data to keep betweens calls
**/
typedef struct saveData
{
	SFreqData *sFreqData;/** @brief frequency's data**/
	int **freqCounter;/** @brief the number of time we call each frequency for each core**/
	int duration;/** @brief the duration we have been at the current frequency**/
	SMarkov ** predictor; /** @brief handle to my markov modeling system**/
} SaveData;


/**
 * @brief decide which algorithm we have to process
 **/ 
void decisionAlgorithm ();


#endif 