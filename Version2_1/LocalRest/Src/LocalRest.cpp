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
#include <signal.h>

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
   unsigned int cpu;    // which cpu they watch
   bool main;           // main thread
} thOptions;


// local functions
static void * thProf (void * opts);
static void exitCleanup ();
static void sigHandler (int nsig);


// variables shared among the threads
static pthread_t * thIds;
static CoresInfo * cnfo;
static DecisionMaker * dec;
static FreqChanger * freq;
static Profiler * prof;
static thOptions * thopts;

/**
 * @brief Rest entry point.
 *
 */
int main (int argc, char ** argv)
{
   (void)(argc);
   (void)(argv);

   // initialize helpers
   cnfo = new CoresInfo ();
   dec = new NaiveDecisions (cnfo);
   freq = new FreqChanger (cnfo);
   prof = new PfmProfiler ();

   // # of processors
   unsigned int numCores = sysconf (_SC_NPROCESSORS_ONLN);
   thopts = new thOptions [numCores];
   thIds = new pthread_t [numCores];

   // launch threads
   for (unsigned int i = 1; i < numCores; i++)
   {
      // initialize options
      thopts [i].cpu = i;
      thopts [i].main = false;

      // run thread
      if (pthread_create (&thIds [i], NULL, thProf, &thopts [i]) != 0)
      {
         std::cerr << "Failed to create profiler thread" << std::endl;
         return EXIT_FAILURE;
      }
   }

   // cleanup stuff when exiting
   signal (SIGINT, sigHandler);
   atexit (exitCleanup);

   // the main process is the main profiler
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

   // only the main thread receives signals
   if (!opt->main)
   {
      sigset_t set;

      sigemptyset (&set);
      sigaddset (&set, SIGINT);
      pthread_sigmask (SIG_BLOCK, &set, NULL);
   }

   // reset counters
   prof->read (opt->cpu, counters);

   // do it as long as we are not getting killed by a signal
   while (true)
   {
      // wait for a while
      usleep (sleepWin);

      // adapt to what's happening
      prof->read (opt->cpu, counters);
      nfreq = dec->giveReport (opt->cpu, counters);

      // frequency change requested
      if (nfreq != -1)
      {
         // reset sleep window size
         sleepWin = INIT_SLEEP_WIN;

         // change frequency
         freq->changeFreq (opt->cpu, nfreq);
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

static void sigHandler (int nsig)
{
   // Ctrl + C
   if (nsig == SIGINT)
   {
      exit (EXIT_FAILURE);
   }
}

static void exitCleanup ()
{
   unsigned int i;
   unsigned int numCores = sysconf (_SC_NPROCESSORS_ONLN);

   std::cout << "atexit" << std::endl;

   // cancel all threads (0 = main process, not a thread)
   for (i = 1; i < numCores; i++)
   {
      pthread_cancel (thIds [i]);
      pthread_join (thIds [i], NULL);
   }

   // free all memory
   delete prof;
   delete freq;
   delete dec;
   delete cnfo;

   delete [] thopts;
   delete [] thIds;
}

