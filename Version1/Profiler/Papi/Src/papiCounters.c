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
 
#define _GNU_SOURCE
#include <sched.h>

#include <stdio.h>
#include <stdlib.h>
#include <papi.h>
#include <unistd.h>
#include <syscall.h>
#include <sys/types.h>

#include "Log.h"
#include "ThreadedProfiler.h"
#include "papiCounters.h"
#include "rdtsc.h"
 
pid_t getTid (void)
{
	#ifdef SYS_gettid
		return (pid_t)syscall(SYS_gettid);
	#elif defined(__NR_gettid)
		return (pid_t)syscall(__NR_gettid);
	#else
		#error "Unable to implement gettid."
	#endif


//	pid_t answer =  syscall (__NR_gettid);
//	return answer;
}
unsigned long long getTicks ( void )
{
   unsigned long long ret;
   rdtscll (ret);
   return ret;
}
 
 
void initPapiHelper ( int * EventSet, STPContext * handle)
{
	int codes[3] = {0,0,0};
	int ret_code;
	
	if (PAPI_event_name_to_code ("SQ_FULL_STALL_CYCLES", &codes[0]) != PAPI_OK) 
	{
		fprintf (stderr,"PAPI even_name_to_code failed!\n");
		exit (1);
	}

	if (PAPI_event_name_to_code ("UNHALTED_CORE_CYCLES", &codes[1]) != PAPI_OK) 
	{
		fprintf (stderr,"PAPI even_name_to_code failed!\n");
		exit (1);
	}
	if (PAPI_event_name_to_code ("L2_RQSTS:MISS", &codes[2]) != PAPI_OK) 
	{
		fprintf (stderr,"PAPI even_name_to_code failed!\n");
		exit (1);
	}
	
	/* Check to see if the PAPI natives are available */
	if ((PAPI_query_event (codes[0]) != PAPI_OK) || (PAPI_query_event (codes[1]) != PAPI_OK) || (PAPI_query_event (codes[2]) != PAPI_OK)) 
	{
		fprintf (stderr,"PAPI counters aren't sufficient to measure boundedness!\n");
		exit (1);
	}

	ret_code=PAPI_create_eventset ( EventSet );
	if(PAPI_OK != ret_code)
	{
		fprintf (stderr,"Creating the PAPI create eventset failed:%d %s\n",ret_code, PAPI_strerror(ret_code));
		exit (1);

	}
	ret_code=PAPI_add_events (*EventSet,codes, 3);
	if(PAPI_OK != ret_code)
	{
		fprintf (stderr,"Adding the PAPI add eventset failed: %d %s\n",ret_code, PAPI_strerror(ret_code));
		exit (1);
	}

	//This ensures that papi counters only reads the parent thread counters
	ret_code=PAPI_attach (*EventSet, handle->parent);
	if(PAPI_OK != ret_code)
	{
		fprintf (stderr,"Attaching the PAPI eventset failed: %d %s\n",ret_code, PAPI_strerror(ret_code));
		exit (1);
	}
	return;
}


void startPapi (int EventSet)
{
	int ret_code=PAPI_start (EventSet);
	if(PAPI_OK != ret_code)
	{
		fprintf (stderr," PAPI start failed: %s\n",PAPI_strerror(ret_code));
		exit (1);
	
	}	
}
	
void accumPapi (int EventSet, long_long *values)
{	
	int ret_code = PAPI_accum (EventSet,values);
	if(PAPI_OK != ret_code)
	{
		fprintf (stderr," PAPI accum failed: %s\n",PAPI_strerror(ret_code));
		exit (1);
	}
}	
		
void initLibraryPapi ()
{
	int retval=PAPI_library_init (PAPI_VER_CURRENT);
	if (retval != PAPI_VER_CURRENT) 
	{
		fprintf (stderr,"PAPI library init error!\n");
		exit (1);
	}
}

void initThreadPapi (void)
{

	if (PAPI_thread_init ((unsigned long (*)(void)) getTid) != PAPI_OK)
	{
		fprintf (stderr,"Thread init function didn't register properly\n");
		exit(1);
	}
}

PAPI_thread_id_t threadIdPapi ()
{
	return PAPI_thread_id ();
}
