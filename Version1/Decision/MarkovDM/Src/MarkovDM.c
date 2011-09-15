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
	int i;
	SaveData *savedData = malloc (sizeof (*savedData));
	savedData->sFreqData = initCpufreq ();
	savedData->predictor = malloc (savedData->sFreqData->numCores * sizeof(* (savedData->predictor)));

	for(i=0; i< savedData->sFreqData->numCores; i++)
	{
		savedData->predictor[i]=NULL;//first we make all the allocations null since we do per core markov modeling
	}
	savedData->duration=0;
	return savedData;
}

int markovDecisionGiveReport (void *handle, SProfReport *report)
{

	SaveData *savedData = handle;
	SFreqData *freqData =savedData->sFreqData;
	int newFrequency = round ((int) (report->data.tp.bounded * freqData->numFreq));
	newFrequency=(report->data.tp.bounded == 0.0)?1:newFrequency;//unless it's prefectly compute bound we won't use the turbo frequency
	newFrequency=(report->data.tp.bounded == 1.0)?freqData->numFreq-1:newFrequency;//if it's exactly 1.0 then we set it to the lowest frequency
	int currentCore = report->proc_id;
	
	assert (newFrequency>-1 && newFrequency<12);
	assert(currentCore<freqData->numCores);
	//init the markov on this core if not already done
	if(savedData->predictor[currentCore]==NULL)
	{
		savedData->predictor[currentCore]=Markov_Initialize ();
		Markov_Set (savedData->predictor[currentCore], 1 , M_PREFDIST);
		Markov_Set (savedData->predictor[currentCore], 100, M_ERRMAX);
		Markov_Set (savedData->predictor[currentCore], 100, M_CONSTTL);
	}
	if (report->prof_id == THREADED_PROFILER)
	{
		
		savedData->duration+=pow(2,report->data.tp.window)*FIRSTSLEEP;
		if(newFrequency == readFreq(freqData, currentCore))//it's the same frequency suggestion
		{
			report->data.tp.nextWindow=report->data.tp.window+1;
			return 1;
		}
		else  //we have a transition!
		{
			SGlobalAdd nodeToAdd;
			SGlobalAdd * markovPredictPtr;

			
			changeFreq (freqData, currentCore, newFrequency);
			
			nodeToAdd.boundedness=report->data.tp.bounded;
			nodeToAdd.duration=savedData->duration;
			markovPredictPtr=savedData->predictor[currentCore]->fct(savedData->predictor[currentCore],&nodeToAdd);
			if(markovPredictPtr)
			{
				//printf("We had a prediction!!\n");
				report->data.tp.nextWindow=markovPredictPtr->duration;
				return 1;
			}
			else
			{
				report->data.tp.nextWindow=1;
				return 1;
			}
		}
	}

return 0;
}
			
	
void markovDecisionDestruct(void* handle)
{
	SaveData *savedData = handle;
	
	if (savedData != NULL)
	{
		int i;
		for(i=0;i< savedData->sFreqData->numCores;i++)
		{
			Markov_Clear(savedData->predictor[i]);
		}
		free(savedData->predictor);
		destroyCpufreq (savedData->sFreqData);
	}
	free(savedData);
	handle = NULL;
}
