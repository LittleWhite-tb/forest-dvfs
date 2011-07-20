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

#include <assert.h>
#include <math.h>
#include <Profilers.h>
#include <Frequency_Mod.h>
#include <define.h>
#include <Log.h>

#include "DecisionMaker.h"

void* decisionInit (void)
{
	SFreqData *freqData = init_cpufreq ();
	return freqData;
}

int decisionGiveReport (void *handle, SProfReport *report)
{
	SFreqData *freqData = handle;
	int newFrequency = round((int) (report->data.tp.bounded * (freqData->numFreq-1)));
	
	if (report->prof_id == THREADED_PROFILER)
	{
		//Too change: 0 only for now
		if(newFrequency != readFreq(freqData, 0))
		{
			Log_output(0, "changing frequency %d\n", newFrequency);
			changeFreq (freqData, -1, newFrequency);
		}
	}
	
	return 0;
}

void decisionDestruct(void* handle)
{
	SFreqData *sFreqData = handle;
	
	if (handle != NULL)
	{
		destroy_cpufreq(sFreqData);
	}
	
	handle = NULL;
}
