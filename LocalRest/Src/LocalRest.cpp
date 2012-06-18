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
 * The LocalRest class is in this file. This is the main file for the local rest
 * server.
 */

#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <cstring>
#include <sstream>

#include "Common.h"
#include "LocalRest.h"
#include "CPUInfo.h"
#include "AdaptiveDecisions.h"
//#include "NewAdaptiveDecisions.h"
#include "pfmProfiler.h"

// local functions
static void * thProf (void * arg);
static void exitCleanup ();
static void sigHandler (int nsig);

// options used to create the threads
typedef struct
{
   unsigned int id;
   DecisionMaker * dec;
   Profiler * prof;
   DVFSUnit * unit;
} thOpts;

// variables shared among the threads
typedef struct
{
   CPUInfo * cnfo;
   pthread_t * thIds;
   thOpts ** allOpts;
} RESTContext;


// the context
static RESTContext restCtx;


/**
 * Rest entry point.
 */
int main (int argc, char ** argv)
{
   (void)(argc);
   (void)(argv);


   // initialize the general context
   restCtx.cnfo = new CPUInfo ();

   unsigned int nbDVFSUnits = restCtx.cnfo->getNbDVFSUnits ();
   restCtx.thIds = new pthread_t [nbDVFSUnits];
   restCtx.allOpts = new thOpts * [nbDVFSUnits];


   // launch threads
   for (unsigned int i = 1; i < nbDVFSUnits; i++)
   {
      DVFSUnit & unit = restCtx.cnfo->getDVFSUnit (i);
      // initialize options
      restCtx.allOpts [i] = new thOpts ();
      thOpts * opts = restCtx.allOpts [i];
      opts->id = i;
      //opts->dec = new NewAdaptiveDecisions (unit);
      opts->dec = new AdaptiveDecisions (unit);
      opts->prof = new PfmProfiler (unit);
      opts->unit = &unit;

      // run thread
      if (pthread_create (&restCtx.thIds [i], NULL, thProf, opts) != 0)
      {
         std::cerr << "Failed to create profiler thread" << std::endl;
         return EXIT_FAILURE;
      }
   }

   // the main process is the main profiler
   DVFSUnit & unit0 = restCtx.cnfo->getDVFSUnit (0);

   restCtx.allOpts [0] = new thOpts ();
   thOpts * opts = restCtx.allOpts [0];
   opts->id = 0;
   //opts->dec = new NewAdaptiveDecisions (unit0);
   opts->dec = new AdaptiveDecisions (unit0);
   opts->prof = new PfmProfiler (unit0);
   opts->unit = &unit0;

   // cleanup stuff when exiting
#ifdef REST_EXTRA_LOG
   signal (SIGUSR1, sigHandler);
#endif
   signal (SIGINT, sigHandler);
   atexit (exitCleanup);

   thProf (opts);

   // never reached
   return EXIT_SUCCESS;
}

static void * thProf (void * arg)
{
   HWCounters hwc;
   thOpts * opts = (thOpts *) arg;
   Decision lastDec;

   // only the main thread receives signals
   if (opts->id != 0)
   {
      sigset_t set;

      sigemptyset (&set);
      sigaddset (&set, SIGINT);
      sigaddset (&set, SIGUSR1);
      pthread_sigmask (SIG_BLOCK, &set, NULL);
   }

   // reset counters and get initialization data
   opts->prof->read (hwc);
   lastDec = opts->dec->defaultDecision ();

   // do it as long as we are not getting killed by a signal
   while (true)
   {
      // wait for a while
      usleep (lastDec.sleepWin);

      // check what's going on
      	opts->prof->read (hwc);
      //if(hwc.cycles > 1000)
      //{
      	//lastDec = opts->dec->takeDecision (hwc);
      //}
      	// switch frequency
      	opts->unit->setFrequency (lastDec.freqId);
      

      // if needed, wait a bit for the freq to be applied
      if (lastDec.preCntResetPause != 0)
      {
         usleep (lastDec.preCntResetPause);

         // reset the counters
         opts->prof->read (hwc);
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
   else
   {
      // SIGUSER1
      if (nsig == SIGUSR1)
      {
#ifdef REST_EXTRA_LOG
         // log a marker in all the logs
         for (unsigned int i = 0; i < restCtx.cnfo->getNbDVFSUnits (); i++)
         {
            restCtx.allOpts [i]->dec->logMarker ();
         }
#endif
      }
   }
}

static void exitCleanup ()
{
   unsigned int i;
   unsigned int nbUnits = restCtx.cnfo->getNbDVFSUnits ();

   // cancel all threads (0 = main process, not a thread)
   for (i = 1; i < nbUnits; i++)
   {
      pthread_cancel (restCtx.thIds [i]);
      pthread_join (restCtx.thIds [i], NULL);
   }

   // clean the memory used by all the threads
   for (i = 0; i < nbUnits; i++)
   {
      delete restCtx.allOpts [i]->prof;
      delete restCtx.allOpts [i]->dec;
      delete restCtx.allOpts [i];
   }
   delete restCtx.cnfo;
   delete [] restCtx.thIds;
   delete [] restCtx.allOpts;
}

