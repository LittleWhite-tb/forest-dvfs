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
  @file ThreadedProfiler.cpp
  @brief The ThreadedProfiler class header is in this file
 */

#include "ThreadedProfiler.h"
#include "PapiCounters.h"
#include "Message.h"
#include "ReportMsg.h"
#include "YellowPages.h"

#include <cmath>
#include <iostream>
#include <cstdio>

ThreadedProfiler::ThreadedProfiler (void) : Profiler()
{
   this->prof = new PapiCounters();
   this->prof->attach_to (LibProf::getTID());
   this->prof->start_counters();

   pthread_create (&this->thid, NULL, ThreadedProfiler::profile_loop, this);
}

ThreadedProfiler::~ThreadedProfiler (void)
{
   pthread_cancel (this->thid);
   if (pthread_join (this->thid, NULL))
   {
      std::perror ("Failed to wait for profiler thread termination");
   }
   delete (this->prof);
}

void * ThreadedProfiler::profile_loop (void * arg)
{
   ThreadedProfiler * obj = (ThreadedProfiler *) arg;
   unsigned int win_sz = 1;
   long long int values[3];
   unsigned int sleep_scale = INIT_SLEEP_SCALE;
   unsigned int sleep_time = std::pow (2, win_sz) * sleep_scale;

   unsigned int my_id = YellowPages::get_id();
   unsigned int server_id = YellowPages::get_server_id();

   pthread_setcancelstate (PTHREAD_CANCEL_ENABLE, NULL);
   pthread_setcanceltype (PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

   // reset counters
   obj->prof->read (values);

   // continue profiling for a while...
   while (true)
   {
      Message * msg;
      unsigned int timeout = sleep_time;
      ReportMsg * report;

      do
      {
         msg = obj->comm->recv (&timeout);

         // most probably timeout (can be error too...)
         if (msg == NULL)
         {
            break;
         }

      }
      while (msg != NULL);

      obj->prof->read (values);

      // send report to the decision server
      report = new ReportMsg (my_id, server_id, values);
      obj->comm->send (*report);
      delete report;

      std::cout << values[0] << " " << values[1] << " " << values[2] << std::endl;
   };

   return NULL;
}
