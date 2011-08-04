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
#include <stdlib.h>

#include "DecisionStructures.h"
#include "Log.h"
#include "MarkovDM.h"
#include "ThreadedProfiler.h"

#define DECISION_MAKER_SEARCH 0


void* markovDecisionInit (void)
{

	SaveData *savedData = malloc (sizeof (savedData));
	savedData->sFreqData = initCpufreq ();
	savedData->predictor = Markov_Initialize ();
	Markov_Set(savedData->predictor, 1 , M_PREFDIST);
	Markov_Set(savedData->predictor, 100, M_ERRMAX);
	Markov_Set(savedData->predictor, 100, M_CONSTTL);
	savedData->duration=0;
	return savedData;
}

int markovDecisionGiveReport (void *handle, SProfReport *report)
{
	SFreqData *freqData = handle;
	int newFrequency = round ((int) (report->data.tp.bounded * freqData->numFreq));
	int currentCore = report->proc_id;
	
	if (report->prof_id == THREADED_PROFILER)
	{
		//Too change: 0 only for now
		if(newFrequency != readFreq(freqData, 0))
		{
			Log_output (0, "changing frequency %d\n", newFrequency);
			changeFreq (freqData, -1, newFrequency);
			//changeFreq (freqData, currentCore, newFrequency);
			report->data.tp.nextWindow=FIRSTSLEEP;
			return 1;
		}
		else
		{
			report->data.tp.nextWindow=report->data.tp.window*2;
			report->data.tp.nextWindow=(report->data.tp.nextWindow>LONGESTSLEEP)?report->data.tp.nextWindow:LONGESTSLEEP;
			return 1;
		}
	}
	
	return 0;
}

void markovDecisionDestruct(void* handle)
{
	SaveData *savedData = handle;
	
	if (savedData != NULL)
	{
		Markov_Clear(savedData->predictor);
		destroyCpufreq (savedData->sFreqData);
	}
	free(savedData);
	handle = NULL;
}
