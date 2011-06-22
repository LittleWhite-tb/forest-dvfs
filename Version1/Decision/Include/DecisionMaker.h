/*
Copyright (C) 2011 Exascale Compûting Research

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

typedef struct sProfReport
{
	ProfDesc prof_id;
	int proc_id;
	ProfilerInfo data;
} SProfReport;

typedef struct tabFreq
{
	int* allFreq;
	int nbFreq;
} TabFreq;

/**
 * @brief init the decision maker
 * @return the avaibles frequencies
 **/
TabFreq* init();

/**
 * @brief function return 0 if no additional action is required by the profiler and 1 if context needs to be examined
 **/
int Decision_give_report( SProfReport *);

/**
 * @brief decide witch algorithm we have to process
 **/ 
void decideAlgorithm();


#endif 
