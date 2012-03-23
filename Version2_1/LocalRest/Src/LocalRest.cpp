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
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>


#include "LocalRest.h"
#include "CoresInfo.h"
#include "AdaptiveDecisions.h"
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
static void pipeDebug ();


// variables shared among the threads
static pthread_t * thIds;
static CoresInfo * cnfo;
static DecisionMaker * dec;
static FreqChanger * freq;
static Profiler * prof;
static thOptions * thopts;
static int pipeFD = -1;

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
   dec = new AdaptiveDecisions (cnfo);
   freq = new FreqChanger (cnfo);
   prof = new PfmProfiler ();

   // # of processors
   unsigned int numCores = cnfo->numCores;
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

   // handle debug request
   if (mkfifo (NAMEPIPE, 0644) != 0)
   {
      std::cerr << "Error when creating debug pipe: " << strerror (errno) << std::endl;
      exit (EXIT_FAILURE);
   }

   signal (SIGUSR1, sigHandler);


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
   unsigned long long int counters [6];    // sq_full_cycles, unhalted_core_cycles, l2_misses
   unsigned int curFreqId = (unsigned int) -1;

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
      Decision resDec;

      // wait for a while
      usleep (sleepWin);

      // adapt to what's happening
      prof->read (opt->cpu, counters);
      resDec = dec->takeDecision (opt->cpu, counters);

//      if (curFreqId != resDec.freqId) {
      freq->changeFreq (opt->cpu, resDec.freqId);
//         curFreqId = resDec.freqId;
//      }

      sleepWin = resDec.sleepWin;
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
   else
      // SIGUSER1
      if (nsig == SIGUSR1)
      {
         pipeDebug ();
      }
}

static void pipeDebug ()
{
   std::string stats = freq->debug ().c_str ();
   size_t size = stats.size ();

   if (pipeFD == -1)
   {
      pipeFD = open (NAMEPIPE, O_WRONLY);
   }

   if (write (pipeFD, stats.c_str (), size) == -1)
   {
      std::cerr << "Debug Failed" << std::endl;
   }
}


static void exitCleanup ()
{
   unsigned int i;
   unsigned int numCores = sysconf (_SC_NPROCESSORS_ONLN);

   // cancel all threads (0 = main process, not a thread)
   for (i = 1; i < numCores; i++)
   {
      pthread_cancel (thIds [i]);
      pthread_join (thIds [i], NULL);
   }

   close (pipeFD);
   unlink (NAMEPIPE);

   // free all memory
   delete prof;
   delete freq;
   delete dec;
   delete cnfo;

   delete [] thopts;
   delete [] thIds;
}

