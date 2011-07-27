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
#include <stdlib.h>
#include <math.h>

#include "Frequency_Mod.h"
#include "define.h"
#include "Log.h"
#include "DecisionProcess.h"
#include "DecisionMaker.h"

#define DECISION_MAKER_SEARCH 0

void* naiveDecisionInit (void)
{
	int i, j;
	
	//allocation
	SaveData *savedData = malloc(sizeof(savedData));
	savedData->sFreqData = initCpufreq ();
	
	savedData->freqCounter = (int **)malloc(savedData->sFreqData->numCores * sizeof(int *));
	
	for(i = 0; i < savedData->sFreqData->numCores; i++)
	{
		savedData->freqCounter[i] = (int *)malloc(savedData->sFreqData->numFreq * sizeof(int));
	}
	
	
	for(i = 0; i < savedData->sFreqData->numCores; i++)
	{
		for(j = 0; j < savedData->sFreqData->numFreq; j++)
		{
			savedData->freqCounter[i][j] = 0;
		}
	}
	
	
	//return the structure intialized
	return savedData;
}

int naiveDecisionGiveReport (void *handle, SProfReport *report)
{
	if (report->prof_id == THREADED_PROFILER)
	{
		int currentCore = report->proc_id;
		
		//Put in variables easier to use
		SaveData *savedData = handle;
		SFreqData *freqData = savedData->sFreqData;
		
		//Take new information on the frequency that we should move to and the current date
		//to say that we wanted to move to it
		int newFrequency = round((int) (report->data.tp.bounded * freqData->numFreq));
		int distance_frequecies = abs(newFrequency - freqData->currentFreqs[currentCore]);
		
		//Increase the number of time that we call for this frequency
		savedData->freqCounter[currentCore][newFrequency]++;
			
		// if the new frequency isn't equal to the old one
		// and if (the number of time that we have call this frequency) * (distance between each others)
		// is greater than (the number of time that we have called the curent frequency) then change
		if(newFrequency != readFreq (freqData, currentCore) 
			&& (savedData->freqCounter[currentCore][newFrequency]) * distance_frequecies
			> savedData->freqCounter[currentCore][readFreq (freqData, currentCore)])
		{
			int i;
			
			//reset to 0 all freq counters
			for(i = 0; i < freqData->numFreq; i ++)
				if(i != newFrequency)
					savedData->freqCounter[currentCore][i] = 0;
			
			//change the frequency
			Log_output(0, "changing frequency %d\n", newFrequency);
			changeFreq (freqData, currentCore, newFrequency);
		}
	}
	
	return 0;
}

void naiveDecisionDestruct(void* handle)
{
	SaveData *savedData = handle;
	
	if (handle != NULL)
	{
		int i;
		
		for(i = 0; i < savedData->sFreqData->numCores; i++)
		{
			free(savedData->freqCounter[i]);
		}
		free(savedData->freqCounter);
		
		savedData->freqCounter = NULL;
		
		destroyCpufreq(savedData->sFreqData);
	}
	
	handle = NULL;
}
