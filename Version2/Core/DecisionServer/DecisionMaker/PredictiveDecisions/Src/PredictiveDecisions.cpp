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
 @file PredictiveDecisions.cpp
 @brief The PredictiveDecisions class is in this file
 */

#include <assert.h>
#include <cmath>

#include "PredictiveDecisions.h"

PredictiveDecisions::PredictiveDecisions (CoresInfos *coresInfos) :
		DecisionMaker (coresInfos)
{

}

PredictiveDecisions::~PredictiveDecisions (void)
{

}

int PredictiveDecisions::giveReport (int core, long long HWCounters[3])
{
	assert(coresInfo != NULL);

	//Compute the boundness of the program
	float boundness = computeBoundness (HWCounters[0], HWCounters[1],
			HWCounters[2]);

	//Choose to which frequency we want to go
	int newFrequency = round ((int) (boundness * (coresInfo->numFreqs - 1)));
	newFrequency = (boundness == 0.0) ? 1 : newFrequency; //unless it's prefectly compute bound, which it never will be, we won't use the turbo frequency

	//Datas which belong to a core
	CoreData *coreadata = &coresInfo->allCoreDatas[core];

	//Compute distance between current frequency to the new one
	int currentFreq = coreadata->currentFreq;
	int distance_frequecies = std::abs ((float) (newFrequency - currentFreq));

	//Increase the number of time that we call for this frequency
	coreadata->freqTrack[newFrequency]++;

	// if the new frequency isn't equal to the old one
	// and if (the number of time that we have call this frequency) * (distance between each others)
	// is greater than (the number of time that we have called the curent frequency) then change
	if (newFrequency != currentFreq
			&& (coreadata->freqTrack[newFrequency]) * distance_frequecies
					> coreadata->freqTrack[currentFreq])
	{
		return newFrequency;
	}

	assert(true);
	return 1;
}

