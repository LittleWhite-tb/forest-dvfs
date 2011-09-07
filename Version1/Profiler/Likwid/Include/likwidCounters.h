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

#ifndef H_LIKWIDCOUNTER
#define H_LIKWIDCOUNTER 

#include <stdint.h>

typedef struct sLikwidData {
	char **hardwareCountersNames;
	unsigned nbCounters;
	int *threads;
	unsigned nbThreads;
} SLikwidData;

void Likwid_startLibrary (unsigned nbThreads);
void Likwid_stopLibrary (void);
void Likwid_startCounters (void);
void Likwid_startCountersThread (unsigned threadId);
void Likwid_stopCounters (void);
void Likwid_stopCountersThread (unsigned threadId);
void Likwid_readCounters (void);
void Likwid_readCountersThread (unsigned threadId);
uint64_t Likwid_getResult (unsigned threadId, unsigned counterId);
unsigned Likwid_getCountersNumber ();
char *Likwid_getCounterName (unsigned counterId);

#endif
