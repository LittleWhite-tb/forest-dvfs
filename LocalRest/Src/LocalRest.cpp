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
//#include "AdaptiveDecisions.h"
#include "NewAdaptiveDecisions.h"
#include "pfmProfiler.h"

#ifdef REST_LOG
	#include "Logger.h"
#endif

// local functions
static void * thProf (void * arg);
static void exitCleanup ();
static void sigHandler (int nsig);

// options used to create the threads
typedef struct
{
	unsigned int id;
	DecisionMaker *dec;
	Profiler *prof;
	DVFSUnit *unit;
} thOpts;

struct ThreadContext {
	pthread_t pid;
	thOpts opts;
};

// variables shared among the threads
struct RESTContext
{
	CPUInfo cnfo;
	ThreadContext *thdCtx;
};

// the context
static RESTContext restCtx;

/**
 * Rest entry point.
 */
int main (int argc, char ** argv)
{
	(void)(argc);
	(void)(argv);

	unsigned int nbDVFSUnits = restCtx.cnfo.getNbDVFSUnits ();
#ifdef REST_LOG	
	Logger::initLog (nbDVFSUnits);
#endif
	restCtx.thdCtx = new ThreadContext [nbDVFSUnits];
	handleAllocation (restCtx.thdCtx);
	
	// launch threads
	for (unsigned int i = 1; i < nbDVFSUnits; i++)
	{
		DVFSUnit& unit = restCtx.cnfo.getDVFSUnit (i);
		// initialize options	
		thOpts& opts = restCtx.thdCtx [i].opts;
		opts.id = i;
		opts.dec = new NewAdaptiveDecisions (unit);
		handleAllocation (opts.dec);
		opts.prof = new PfmProfiler (unit);
		handleAllocation (opts.dec);
		opts.unit = &unit;

		// run thread
		if (pthread_create (&restCtx.thdCtx [i].pid, NULL, thProf, &opts) != 0)
		{
			std::cerr << "Error: Failed to create profiler thread" << std::endl;
			return EXIT_FAILURE;
		}
	}

	// the main process is the main profiler
	DVFSUnit& unit0 = restCtx.cnfo.getDVFSUnit (0);
	thOpts& opts = restCtx.thdCtx [0].opts;
	opts.id = 0;
	opts.dec = new NewAdaptiveDecisions (unit0);
	handleAllocation (opts.dec);
	opts.prof = new PfmProfiler (unit0);
	handleAllocation (opts.dec);
	opts.unit = &unit0;

	// cleanup stuff when exiting
#ifdef REST_LOG
	signal (SIGUSR1, sigHandler);
#endif
	signal (SIGINT, sigHandler);
	atexit (exitCleanup);

	// Launch the function for main thread
	thProf (&opts);

	// never reached
	return EXIT_SUCCESS;
}

static void * thProf (void * arg)
{
	thOpts * opts = (thOpts *) arg;
	Decision dec;

   // default initialization for the first decision
   dec.freqId = 0;
   dec.cpuId = opts->unit->getOSId ();
   dec.sleepWin = 0;
   dec.freqApplyDelay = 0;

	opts->dec->setProfiler (opts->prof);

	// only the main thread receives signals
	if (opts->id != 0)
	{
		sigset_t set;

		sigemptyset (&set);
		sigaddset (&set, SIGINT);
		sigaddset (&set, SIGUSR1);
		pthread_sigmask (SIG_BLOCK, &set, NULL);
	}

   // do it as long as we are not getting killed by a signal
   while (true) {
      // If the runtime's decision is to skip the sleeping time, we obey it !!
      // wait for a while	
      if (dec.sleepWin > 0)
      {
         usleep (dec.sleepWin);
      }

      dec = opts->dec->takeDecision ();

      // switch frequency	
      if (dec.sleepWin > 0) 
      {
         opts->unit->setFrequency (dec.freqId);
      }

      // if needed, wait a bit for the freq to be applied	
      if (dec.freqApplyDelay != 0) {
         usleep (dec.freqApplyDelay);
         opts->dec->readCounters (); // Reset the counters
      }
   }

	// pacify compiler but we never get out of while loop
	return NULL;
}

static void sigHandler (int nsig)
{
	switch (nsig) {
	case SIGINT:	
			exit (EXIT_FAILURE);
			break;
	case SIGUSR1:
			#ifdef REST_LOG
				for (unsigned int i = 0; i < restCtx.cnfo.getNbDVFSUnits (); i++) {
					Logger *log = Logger::getLog (restCtx.cnfo.getDVFSUnit (i).getId ());
					log->logOut ("*");
				}
			#endif
			break;
	default:
			std::cerr << "Received signal #" << nsig << std::endl;
	};
}

static void exitCleanup ()
{
	unsigned int i;
	unsigned int nbUnits = restCtx.cnfo.getNbDVFSUnits ();

	// cancel all threads (0 = main process, not a thread)
	for (i = 1; i < nbUnits; i++)
	{
		pthread_cancel (restCtx.thdCtx [i].pid);
		pthread_join (restCtx.thdCtx [i].pid, NULL);
	}

	// clean the memory used by all the threads
	for (i = 0; i < nbUnits; i++)
	{
		delete restCtx.thdCtx [i].opts.prof;
		delete restCtx.thdCtx [i].opts.dec;
	}
	delete [] restCtx.thdCtx;

#ifdef REST_LOG
	Logger::destroyLog ();
#endif
}

