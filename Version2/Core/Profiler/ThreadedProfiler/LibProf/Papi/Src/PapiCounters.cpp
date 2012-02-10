/*
   Copyright (C) 2011 Exascale Research Center

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

/**
  @file PapiCounters.cpp
  @brief The PapiCounters class is in this file
 */

#include "PapiCounters.h"
#include <papi.h>
#include <iostream>
#include <cstdlib>
#include <cassert>

PapiCounters::PapiCounters (void)
{
   // library init
   int retval = PAPI_library_init (PAPI_VER_CURRENT);
   if (retval != PAPI_VER_CURRENT)
   {
      std::cerr << "PAPI library init error!" << std::endl;
      exit (1);
   }
   retval = PAPI_is_initialized ();
   if (retval != PAPI_LOW_LEVEL_INITED)
   {
      std::cerr << "Failed to init PAPI sub-system." << std::endl;
      exit (1);
   }

   // init the threading
   if (PAPI_thread_init (LibProf::getTID) != PAPI_OK)
   {
      std::cerr << "Thread init function didn't register properly" << std::endl;
      exit (1);
   }

   // initialize the event set
   int ev_codes [3] = {0, 0, 0};

   if (PAPI_event_name_to_code (const_cast<char *> ("SQ_FULL_STALL_CYCLES"),
                                &ev_codes [0]) != PAPI_OK)
   {
      std::cerr << "PAPI even_name_to_code failed!" << std::endl;
      exit (1);
   }

   if (PAPI_event_name_to_code (const_cast<char *> ("UNHALTED_CORE_CYCLES"),
                                &ev_codes [1]) != PAPI_OK)
   {
      std::cerr << "PAPI even_name_to_code failed!" << std::endl;
      exit (1);
   }

   if (PAPI_event_name_to_code (const_cast<char *> ("L2_RQSTS:MISS"),
                                &ev_codes [2]) != PAPI_OK)
   {
      std::cerr << "PAPI even_name_to_code failed!" << std::endl;
      exit (1);
   }

   // check for the events availability
   if ((PAPI_query_event (ev_codes [0]) != PAPI_OK)
         || (PAPI_query_event (ev_codes [1]) != PAPI_OK)
         || (PAPI_query_event (ev_codes [2]) != PAPI_OK))
   {
      std::cerr << "Insufficient hardware counters support." << std::endl;
      exit (1);
   }

   // create the event set
   this->ev_set = PAPI_NULL;
   retval = PAPI_create_eventset (&this->ev_set);
   if (PAPI_OK != retval)
   {
      std::cerr << "Creating the PAPI create eventset failed: " << PAPI_strerror (retval) << std::endl;
      exit (1);
   }

   retval = PAPI_add_events (this->ev_set, ev_codes, 3);
   if (PAPI_OK != retval)
   {
      std::cerr << "Adding the PAPI add eventset failed: " <<  PAPI_strerror (retval) << std::endl;
      exit (1);
   }
}

PapiCounters::~PapiCounters (void)
{
   PAPI_shutdown ();
}

void PapiCounters::attach_to (unsigned long int tid)
{
   if (PAPI_attach (this->ev_set, tid) != PAPI_OK)
   {
      std::cerr << "Failed to observe the events for the thread #" << tid << std::endl;
   }
}

void PapiCounters::start_counters ()
{
   PAPI_start (this->ev_set);
}

void PapiCounters::stop_counters ()
{
   long long dummy [3];

   PAPI_stop (this->ev_set, dummy);
}

void PapiCounters::read (long long * values)
{
   assert (values != NULL);

   values [0] = 0;
   values [1] = 0;
   values [2] = 0;

   if (PAPI_accum (this->ev_set, values) != PAPI_OK)
   {
      std::cerr << "Counter accumulation failed." << std::endl;
   }
}
