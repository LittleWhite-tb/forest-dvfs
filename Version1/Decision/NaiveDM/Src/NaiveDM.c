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

#include "NaiveDM.h"
#include "Frequency_Mod.h"
#include "Log.h"
#include "ThreadedProfiler.h"


//#define BINARYMODE

#define DECISION_MAKER_SEARCH 0

void* naiveDecisionInit (int idCore)
{
	SFreqData *freqData = initCpufreq (idCore);
	return freqData;
}

int naiveDecisionGiveReport (void *handle, SProfReport *report)
{
	SFreqData *freqData = handle;

	#ifdef BINARYMODE
	int newFrequency = (report->data.tp.bounded < 0.5)?1:freqData->numFreq-1;
	#else
	int newFrequency = round ((int) (report->data.tp.bounded * freqData->numFreq));
	newFrequency=(report->data.tp.bounded == 0.0)?1:newFrequency;//unless it's prefectly compute bound, which it never will be, we won't use the turbo frequency
	newFrequency=(report->data.tp.bounded == 1.0)?freqData->numFreq-1:newFrequency;//if it's exactly 1.0 then we set it to the lowest frequency
	#endif

	//Trick to adjuste the right freq
	if(report->data.tp.bounded <= 0.1)
		newFrequency=1;
	//else
	//	newFrequency=freqData->numFreq-1;
	
	//int currentCore = report->proc_id;

	//Too change: 0 only for now
	//
	int currentFreq = readFreq (freqData);

	int FreqDistance = abs(newFrequency - currentFreq);

	
	if(newFrequency != currentFreq)
	{
		if(FreqDistance > 2 || (FreqDistance <= 1 && newFrequency > currentFreq))
		{
			report->data.tp.nextWindow=1;
			changeFreq (freqData, newFrequency);
			freqData->officialWindowTrack = report->data.tp.nextWindow;
			freqData->localWindowTrack=1;
			return 1;
		}
		else 
		{
			report->data.tp.nextWindow = ++report->data.tp.window;
			freqData->officialWindowTrack = report->data.tp.nextWindow;
			freqData->localWindowTrack++;
			return 1;
		}
	}
	else
	{

		report->data.tp.nextWindow = ++report->data.tp.window;
		freqData->officialWindowTrack = report->data.tp.nextWindow;
		freqData->localWindowTrack++;
		return 1;
	}
	return 0;
}

void naiveDecisionDestruct(void* handle)
{
	SFreqData *sFreqData = handle;
	
	if (handle != NULL)
	{
		destroyCpufreq (sFreqData);
	}
	
	handle = NULL;
}

