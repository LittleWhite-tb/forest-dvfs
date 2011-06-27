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
	
	return freqAvaible;
}

int decisionGiveReport(void *handle, SProfReport *report)
{
	FreqData *freqAvaible = handle;
	
	int *avaibleFreq =  freqAvaible->availablefreqs;
	
	printf("%d\n", report->proc_id);
	
	if(report->prof_id == THREADED_PROFILER)
	{
		(void)avaibleFreq[(int)(report->data.tp.bounded * freqAvaible->numberOfFreq)];
	}
	
	
	return 0;
}

void decisionDestruct(void* handle)
{
	if(handle != NULL)
		free(handle);
}
