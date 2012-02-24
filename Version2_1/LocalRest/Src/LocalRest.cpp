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
 * @file LocalRest.cpp
 * @brief The LocalRest class is in this file. This is the main file for the
 * local rest server.
 */

#include <cstdlib>
#include <pthread.h>
#include <unistd.h>
#include <iostream>

#include "LocalRest.h"
#include "CoresInfo.h"
#include "DecisionMaker.h"
#include "NaiveDecisions.h"
#include "Profiler.h"
#include "pfmProfiler.h"
#include "FreqChanger.h"

// Options for threads
typedef struct
{
   CoresInfo * cnfo;    // shared cores info
   DecisionMaker * dec; // shared decision maker
   FreqChanger * freq;  // shared freq changer
   Profiler * prof;     // shared profiler
   unsigned int cpu;    // which cpu they watch
   bool main;           // main thread
} thOptions;

// thread function
static void * thProf (void * opts);

/**
 * @brief Rest entry point.
 *
 */
int main (int argc, char ** argv)
{
   (void)(argc);
   (void)(argv);

   CoresInfo * cnfo = new CoresInfo ();
   DecisionMaker * dec = new NaiveDecisions (cnfo);
   FreqChanger * freq = new FreqChanger (cnfo);
   Profiler * prof = new PfmProfiler ();

   // # of processors
   unsigned int numCores = sysconf (_SC_NPROCESSORS_ONLN);
   thOptions * thopts = new thOptions [numCores];

   // launch threads
   for (unsigned int i = 1; i < numCores; i++)
   {
      pthread_t thid;

      // initialize options
      thopts [i].cnfo = cnfo;
      thopts [i].dec = dec;
      thopts [i].freq = freq;
      thopts [i].prof = prof;
      thopts [i].cpu = i;
      thopts [i].main = false;

      // run thread
      if (pthread_create (&thid, NULL, thProf, &thopts [i]) != 0)
      {
         std::cerr << "Failed to create profiler thread" << std::endl;
      }
   }

   delete [] thopts;

   // the main process is the main profiler
   thopts [0].cnfo = cnfo;
   thopts [0].dec = dec;
   thopts [0].freq = freq;
   thopts [0].prof = prof;
   thopts [0].cpu = 0;
   thopts [0].main = true;

   thProf (&thopts [0]);

   // never reached
   return EXIT_SUCCESS;
}

static void * thProf (void * arg)
{
   thOptions * opt = (thOptions *) arg;
   unsigned int sleepWin = INIT_SLEEP_WIN;
   unsigned long long int counters [3];    // sq_full_cycles, unhalted_core_cycles, l2_misses
   int nfreq;

   // reset counters
   opt->prof->read (opt->cpu, counters);

   // do it as long as we are not getting killed by a signal
   while (true)
   {
      // wait for a while
      usleep (sleepWin);

      // adapt to what's happening
      opt->prof->read (opt->cpu, counters);
      nfreq = opt->dec->giveReport (opt->cpu, counters);

      // frequency change requested
      if (nfreq != -1)
      {
         // reset sleep window size
         sleepWin = INIT_SLEEP_WIN;

         // change frequency
         opt->freq->changeFreq (opt->cpu, nfreq);
      }
      else  // no frequency modification requested
      {
         // increase sleep window size
         sleepWin *= 2;
         sleepWin = sleepWin > LONGEST_SLEEP_WIN ? LONGEST_SLEEP_WIN : sleepWin;
      }
   }

   // pacify compiler but we never get out of while loop
   return NULL;
}

