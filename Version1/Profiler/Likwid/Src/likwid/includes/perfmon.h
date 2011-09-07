/*
 * ===========================================================================
 *
 *      Filename:  perfmon.h
 *
 *      Description:  Header File of perfmon module.
 *                    Configures and reads out performance counters
 *                    on x86 based architectures. Supports multi threading.
 *
 *      Version:  2.2
 *      Created:  14.06.2011
 *
 *      Author:  Jan Treibig (jt), jan.treibig@gmail.com
 *      Company:  RRZE Erlangen
 *      Project:  likwid
 *      Copyright:  Copyright (c) 2010, Jan Treibig
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License, v2, as
 *      published by the Free Software Foundation
 *     
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *     
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * ===========================================================================
 */


/** \file perfmon.h
 * Header of perfmon module
 *
 */
#ifndef PERFMON_H
#define PERFMON_H

#include <stdint.h>
#include <bstrlib.h>
#include <types.h>

extern int perfmon_verbose;
extern PerfmonThread* perfmon_threadData;
extern int perfmon_numThreads;

void perfmon_readCountersOneThread (unsigned idX);
void perfmon_startCountersOneThread (unsigned idX);
void perfmon_stopCountersOneThread (unsigned idX);

extern int  (*perfmon_getIndex) (bstring reg, PerfmonCounterIndex* index);
extern void (*perfmon_setupCounterThread) (int thread_id,
       PerfmonEvent* event , PerfmonCounterIndex index);
//extern void (*perfmon_setupReport) (MultiplexCollections* collections);
//extern void (*perfmon_printReport) (MultiplexCollections* collections);

extern void perfmon_initEventSet(StrUtilEventSet* eventSetConfig, PerfmonEventSet* set);
extern void perfmon_printAvailableGroups(void);
extern void perfmon_printGroupHelp(bstring group);
extern void perfmon_init(int numThreads, int threads[],FILE* outstream);
extern void perfmon_finalize(void);
extern void perfmon_setupEventSet(bstring eventString);
extern uint64_t perfmon_getEventResult(int thread, int index);
extern int perfmon_setupEventSetC(char* eventCString, char*** eventnames);
extern void perfmon_setupCounters(void);
extern void perfmon_startCounters(void);
extern void perfmon_stopCounters(void);
extern void perfmon_readCounters(void);
extern double perfmon_getResult(int threadId, char* counterString);
extern void perfmon_printMarkerResults(bstring filepath);
extern void perfmon_logCounterResults(double time);
extern void perfmon_printCounterResults(void);
extern void perfmon_printCounters(void);
extern void perfmon_printEvents(void);

#endif /*PERFMON_H*/
