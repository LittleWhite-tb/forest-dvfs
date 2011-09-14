 /*
Copyright (C) 2011 Exascale Computing Research

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
 
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <papi.h>
#define _GNU_SOURCE
#include <unistd.h>
#include <syscall.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#include "ThreadedProfiler.h"
#include "likwidCounters.h"
#include "rdtsc.h"
#include "likwid.h"
#include "perfmon.h"
#include "timer.h"
#include "cpuid.h"
#include "msr.h"

static SLikwidData *m_likwidData = NULL;

/**
	Counter: 0, name: INSTR_RETIRED_ANY
	Counter: 1, name: CPU_CLK_UNHALTED_CORE
	Counter: 2, name: SQ_FULL_STALL_CYCLES
	Counter: 3, name: L2_RQSTS_MISS
	Number of metrics: 4
	
	If you want to add one, then go to the startLibrary function and change
	the perfmon_setupEventSetC string parameter.
*/

void Likwid_startLibrary (unsigned nbThreads)
{
	char hardCountersBuf[256];
	
	assert (nbThreads != 0 && m_likwidData == NULL);
	m_likwidData = malloc (sizeof (*m_likwidData));
	assert (m_likwidData != NULL);
	memset (m_likwidData, 0, sizeof (*m_likwidData));
	
	m_likwidData->nbThreads = nbThreads;
	
	m_likwidData->threads = malloc ( nbThreads * sizeof (*m_likwidData->threads));
	assert (m_likwidData->threads != NULL);
	memset (m_likwidData->threads, 0, nbThreads * sizeof (*m_likwidData->threads));
	
	cpuid_init ();
	msr_init ();
	perfmon_init (m_likwidData->nbThreads, m_likwidData->threads, stderr);
	
	switch (cpuid_info.model)
	{
		 case NEHALEM_WESTMERE_M:
		 case NEHALEM_WESTMERE:
		 case NEHALEM_BLOOMFIELD:
         case NEHALEM_LYNNFIELD:
         case NEHALEM_EX:
			snprintf (hardCountersBuf, sizeof (hardCountersBuf), "INSTR_RETIRED_ANY:FIXC0,CPU_CLK_UNHALTED_CORE:FIXC1,SQ_FULL_STALL_CYCLES:PMC0,L2_RQSTS_MISS:PMC1");
			break;
		 case SANDYBRIDGE:
         case XEON_SANDYBRIDGE:
         	snprintf (hardCountersBuf, sizeof (hardCountersBuf), "INSTR_RETIRED_ANY:FIXC0,CPU_CLK_UNHALTED_CORE:FIXC1,OFFCORE_REQUESTS_BUFFER_SQ_FULL:PMC0,L2_RQSTS_MISS:PMC1");
			break;
		default:
			fprintf (stderr, "Unknown processor with code %p\n", cpuid_info.model);
			assert (0);
			break;
	}
	m_likwidData->nbCounters = perfmon_setupEventSetC (hardCountersBuf, &m_likwidData->hardwareCountersNames);
}

void Likwid_startCounters (void)
{
	perfmon_startCounters ();
}

void Likwid_startCountersThread (unsigned threadId)
{
	perfmon_startCountersOneThread (threadId);
}

void Likwid_stopCounters (void)
{
	perfmon_stopCounters ();
}

void Likwid_stopCountersThread (unsigned threadId)
{
	perfmon_stopCountersOneThread (threadId);
}

void Likwid_readCounters (void)
{
	perfmon_readCounters ();
}

void Likwid_readCountersThread (unsigned threadId)
{
	perfmon_readCountersOneThread (threadId);
}

uint64_t Likwid_getResult (unsigned threadId, unsigned counterId)
{
	return perfmon_getEventResult (threadId, counterId);
}

unsigned Likwid_getCountersNumber (void)
{
	assert (m_likwidData != NULL);
	return m_likwidData->nbCounters;
}

char *Likwid_getCounterName (unsigned counterId)
{
	assert (m_likwidData != NULL);
	assert (counterId < m_likwidData->nbCounters);
	return m_likwidData->hardwareCountersNames[counterId];
}

void Likwid_stopLibrary (void)
{
	assert (m_likwidData != NULL);
	
	perfmon_finalize();
	
	free (m_likwidData->threads), m_likwidData->threads = NULL;
	free (m_likwidData), m_likwidData = NULL;
}
