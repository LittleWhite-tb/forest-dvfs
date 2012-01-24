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

#include "MarkovDM.h"
#include "Frequency_Mod.h"
#include "Log.h"
#include "ThreadedProfiler.h"

#define DECISION_MAKER_SEARCH 0

void* branchDecisionInit (int coreId)
{
	int j;
	
	//allocation
	SaveData *savedData = (SaveData*) malloc (sizeof (savedData));
	
	//Just be paranoid
	if (savedData != NULL)
	{
		//initialized the freq data values
		savedData->sFreqData = initCpufreq (coreId);
		
		//initialize all the array of cores
		savedData->freqCounter = malloc (savedData->sFreqData->numFreq * sizeof (int));
		
		
		//i is the core and j the frequeny and we initialize them to 0
		
		for(j = 0; j < savedData->sFreqData->numFreq; j++)
		{
			savedData->freqCounter[j] = 0;
		}
	}
	else
	{
		//if the malloc failed we stop
		Log_output (0, "Bad allocation for SavedData structure in PredicitiveDM \n");
		exit (1);
	}
		
	//return the structure intialized
	return savedData;
	
}

int branchDecisionGiveReport (void *handle, SProfReport *report)
{		
		//Put in variables easier to use
		SaveData *savedData = handle;
		SFreqData *freqData = savedData->sFreqData;

		int currentCore = report->proc_id;
		assert(currentCore < freqData->numCores);
		
		
		//Take new information on the frequency that we should move to and the current date
		//to say that we wanted to move to it
		int newFrequency = round((int) (report->data.tp.bounded * freqData->numFreq));
		newFrequency=(report->data.tp.bounded == 0.0)?1:newFrequency;//unless it's prefectly compute bound, which it never will be, we won't use the turbo frequency
		newFrequency=(report->data.tp.bounded == 1.0)?freqData->numFreq-1:newFrequency;//if it's exactly 1.0 then we set it to the lowest frequency		
		
		assert (newFrequency>=0 && newFrequency<freqData->numFreq);
		
		int currentFreq = readFreq (freqData);
		int distance_frequecies = abs(newFrequency - currentFreq);
		
		//Increase the number of time that we call for this frequency
		savedData->freqCounter[newFrequency]++;
			
		// if the new frequency isn't equal to the old one
		// and if (the number of time that we have call this frequency) * (distance between each others)
		// is greater than (the number of time that we have called the curent frequency) then change
		if(newFrequency != currentFreq 
			&& (savedData->freqCounter[newFrequency]) * distance_frequecies
			> savedData->freqCounter[currentFreq])
		{
			//int i;
			
			//reset to 0 all freq countersi
			/*
			for(i = 0; i < freqData->numFreq; i ++)
				if(i != newFrequency)
					savedData->freqCounter[currentCore][i] = 0;
			*/
			//change the frequency
			//Log_output(0, "changing frequency %d\n", newFrequency);
			
			report->data.tp.nextWindow=1;
			changeFreq (freqData, newFrequency);
			freqData->officialWindowTrack = report->data.tp.nextWindow;
			freqData->localWindowTrack=1;
		}
		else
		{
			report->data.tp.nextWindow = ++report->data.tp.window;
			freqData->officialWindowTrack = report->data.tp.nextWindow;
			freqData->localWindowTrack++;
		}
	
	return 1;
}

void branchDecisionDestruct(void* handle)
{
	SaveData *savedData = handle;
	
	if (savedData != NULL)
	{
		free(savedData->freqCounter), savedData->freqCounter = NULL;
			
		destroyCpufreq(savedData->sFreqData);
	}
	
	handle = NULL;

}

