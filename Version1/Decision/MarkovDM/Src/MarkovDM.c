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

void* markovDecisionInit (int coreId)
{
	//Initilization of data to save
	SaveData *savedData = malloc (sizeof(*savedData));
	savedData->sFreqData = initCpufreq (coreId);
	savedData->predictor = malloc (sizeof(*(savedData->predictor)));
	savedData->predictor = NULL; //first we make all the allocations null since we do per core markov modeling
	savedData->duration = 0;

	return savedData;
}

int markovDecisionGiveReport (void *handle, SProfReport *report)
{
	//Getting back informations
	SaveData *savedData = handle;
	SFreqData *freqData = savedData->sFreqData;
	
	//Finding the new frequency to move to
	int newFrequency = round (
			(int) (report->data.tp.bounded * freqData->numFreq));
			
	//unless it's prefectly compute bound we won't use the turbo frequency		
	newFrequency = (report->data.tp.bounded == 0.0) ? 1 : newFrequency; 
	//if it's exactly 1.0 then we set it to the lowest frequency
	newFrequency =
			(report->data.tp.bounded == 1.0) ?
					freqData->numFreq - 1 : newFrequency;
					
	//Just to be safe that the frequency is not over max
	assert(newFrequency>-1 && newFrequency<freqData->numFreq);

	//init the markov on this core if not already done
	if (savedData->predictor == NULL)
	{
		savedData->predictor = Markov_Initialize ();
		Markov_Set (savedData->predictor, 1, M_PREFDIST);
		Markov_Set (savedData->predictor, 100, M_ERRMAX);
		Markov_Set (savedData->predictor, 100, M_CONSTTL);
	}
	
	if (report->prof_id == THREADED_PROFILER)
	{

		savedData->duration += pow (2, report->data.tp.window) * FIRSTSLEEP;

		if (newFrequency == readFreq (freqData)) //it's the same frequency suggestion
		{
			report->data.tp.nextWindow = report->data.tp.window + 1;
			return 1;
		}
		else  //we have a transition!
		{
			SGlobalAdd nodeToAdd;
			SGlobalAdd * markovPredictPtr;

			changeFreq (freqData, newFrequency);

			nodeToAdd.boundedness = report->data.tp.bounded;
			nodeToAdd.duration = savedData->duration;
			markovPredictPtr = savedData->predictor->fct (savedData->predictor,
					&nodeToAdd);
			
			if (markovPredictPtr)
			{
				report->data.tp.nextWindow = markovPredictPtr->duration;
				
				freqData->officialWindowTrack = report->data.tp.nextWindow;
				freqData->localWindowTrack=1;
				
				return 1;
			}
			else
			{
				report->data.tp.nextWindow = 1;
				
				freqData->officialWindowTrack = report->data.tp.nextWindow;
				freqData->localWindowTrack++;
				
				return 1;
			}
		}
	}

	return 0;
}

void markovDecisionDestruct (void* handle)
{
	SaveData *savedData = handle;

	if (savedData != NULL)
	{
		Markov_Clear (savedData->predictor);
		free (savedData->predictor);
		destroyCpufreq (savedData->sFreqData);
	}
	free (savedData);
	handle = NULL;
}
