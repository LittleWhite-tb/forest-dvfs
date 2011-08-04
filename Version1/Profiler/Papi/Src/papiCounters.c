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
 
#include <stdio.h>
#include <stdlib.h>
#include <papi.h>
#define _GNU_SOURCE
#include <syscall.h>
#include <sys/types.h>

#include "Log.h"
#include "ThreadedProfiler.h"
#include "papiCounters.h"
 
static __inline__ pid_t getTid (void)
{
	pid_t answer =  syscall (__NR_gettid);
	return answer;
}
 
 
 static void initPapiHelper ( int * EventSet, STPContext * handle)
{
	int codes[3];
	int ret_code;
	
	if (PAPI_event_name_to_code ("SQ_FULL_STALL_CYCLES", &codes[0]) != PAPI_OK) 
	{
		Log_output (0,"PAPI even_name_to_code failed!\n");
		exit (1);
	}

	if (PAPI_event_name_to_code ("UNHALTED_CORE_CYCLES", &codes[1]) != PAPI_OK) 
	{
		Log_output (0,"PAPI even_name_to_code failed!\n");
		exit (1);
	}
	if (PAPI_event_name_to_code ("L2_RQSTS:MISS", &codes[2]) != PAPI_OK) 
	{
		Log_output (0,"PAPI even_name_to_code failed!\n");
		exit (1);
	}

	/* Check to see if the PAPI natives are available */
	if ((PAPI_query_event (codes[0]) != PAPI_OK) || (PAPI_query_event (codes[1]) != PAPI_OK) || (PAPI_query_event (codes[2]) != PAPI_OK)) 
	{
		Log_output (0,"PAPI counters aren't sufficient to measure boundedness!\n");
		exit (1);
	}

	ret_code=PAPI_create_eventset ( EventSet );
	if(PAPI_OK != ret_code)
	{
		Log_output (0,"Creating the PAPI create eventset failed:%d %s\n",ret_code, PAPI_strerror(ret_code));
		exit (1);

	}
	ret_code=PAPI_add_events (*EventSet,codes, 3);
	if(PAPI_OK != ret_code)
	{
		Log_output (0,"Adding the PAPI add eventset failed: %d %s\n",ret_code, PAPI_strerror(ret_code));
		exit (1);
	}

	//This ensures that papi counters only reads the parent thread counters
	ret_code=PAPI_attach (*EventSet, handle->parent);
	if(PAPI_OK != ret_code)
	{
		Log_output (0,"Attaching the PAPI eventset failed: %d %s\n",ret_code, PAPI_strerror(ret_code));
		exit (1);
	}
	return;
}


static void startPapi (int EventSet)
{
	int ret_code=PAPI_start (EventSet);
	if(PAPI_OK != ret_code)
	{
		Log_output (5," PAPI start failed: %s\n",PAPI_strerror(ret_code));
		exit (1);
	
	}	
}
	
static void accumPapi (int EventSet, long_long *values)
{	
	int ret_code = PAPI_accum (EventSet,values);
	if(PAPI_OK != ret_code)
	{
		Log_output (5," PAPI accum failed: %s\n",PAPI_strerror(ret_code));
		exit (1);
	}
}	
		
static void initLibraryPapi ()
{
	int retval=PAPI_library_init (PAPI_VER_CURRENT);
	if (retval != PAPI_VER_CURRENT) 
	{
		Log_output (0,"PAPI library init error!\n");
		exit (1);
	}
}

static void initThreadPapi (pid_t getTid)
{
	if (PAPI_thread_init (getTid) != PAPI_OK)
	{
		Log_output (0,"Thread init function didn't register properly\n");
	}
}

static PAPI_thread_id_t threadIdPapi ()
{
	return PAPI_thread_id ();
}
