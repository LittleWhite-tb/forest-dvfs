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

#include "DecisionMaker.h"

/**@todo make it less dirty*/

void* decisionInit (void)
{
	FreqData *freqAvaible = getAllAvailableFreq();
	SavedData *savedData = malloc(sizeof(SavedData));
	
	savedData->freqAvaible.nbFreq = freqAvaible->numberOfFreq;
	savedData->freqAvaible.freq = freqAvaible->availablefreqs;
	
	return savedData;
}

int decisionGiveReport (void *handle, SProfReport *report)
{
	SavedData *savedData = handle;
	int newFrequency = (int) (report->data.tp.bounded * savedData->freqAvaible.nbFreq);
	
	if (report->prof_id == THREADED_PROFILER)
	{
		if(newFrequency != savedData->currentFreq)
		{
			Log_output(0, "changing frequency %d\n", newFrequency);
			changeFreq ( -1, newFrequency);
			savedData->currentFreq = newFrequency;
		}
	}
	
	return 0;
}

void decisionDestruct(void* handle)
{
	SavedData *savedData = handle;
	
	if (handle != NULL)
	{
		free(savedData->freqAvaible.freq);
		savedData->freqAvaible.freq = NULL;
		free(handle);
	}
	
	handle = NULL;
}
