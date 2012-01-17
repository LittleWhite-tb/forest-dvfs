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

#include <unistd.h>
#include <cmath>
#include <iostream>

ThreadedProfiler::ThreadedProfiler (void)
{
    this->prof = new PapiCounters();
    this->prof->attach_to(LibProf::getTID());
    this->prof->start_counters();

    this->thid = pthread_create(&this->thid, NULL, ThreadedProfiler::profile_loop, this);
}

ThreadedProfiler::~ThreadedProfiler (void)
{
    delete(this->prof);
}

void *ThreadedProfiler::profile_loop(void *arg)
{
    ThreadedProfiler *obj = (ThreadedProfiler *) arg;
    unsigned int win_sz = 1;
    long long int values[3];
    unsigned int sleepScale = INIT_SLEEP_SCALE;

    // reset counters
    obj->prof->read(values);

    // continue profiling for a while...
    while (true) {
        usleep(std::pow(2, win_sz) * sleepScale);
        obj->prof->read(values);
        std::cout << values[0] << " " << values[1] << " " << values[2] << std::endl;
    };

    return NULL;
}
