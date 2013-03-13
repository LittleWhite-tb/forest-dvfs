/*
 * FoREST - Reactive DVFS Control for Multicore Processors
 * Copyright (C) 2013 Universite de Versailles
 * Copyright (C) 2011-2012 Exascale Research Center
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file LocalRest.cpp
 * Entry point for LocalRest. A thread will be started for each unit.
 * Main thread will handle the unit 0.
 */

#include <cstdlib>
#include <cassert>

#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sstream>

#include "glog/logging.h"

#include "Common.h"
#include "LocalRest.h"
#include "Topology.h"
#include "DecisionMaker.h"
#include "Profiler.h"
#include "ThreadContext.h"

#ifdef REST_LOG
	#include "Logger.h"
#endif

namespace FoREST {

// local functions
static void * FoREST (void * arg);
static void exitCleanup ();
static void sigHandler (int nsig);

struct Context {
   Topology *topology;
   std::vector<ThreadContext*> threadContext;
};

// the context
static Context context;

static inline bool handleArguments (int argc, char *argv[], Mode& mode) {
   if (argc != 2) {
      LOG (INFO) << "Usage: " << argv [0] << " {energy,performance}" << std::endl;
      return false;
   }

   std::string energy ("energy");
   std::string performance ("performance");

   if (energy.compare (argv [1]) == 0) {
      mode = MODE_ENERGY;
   }
   else if (performance.compare (argv [1]) == 0) {
      mode = MODE_PERFORMANCE;
   }
   else {
      LOG (FATAL) << "Error: Unknown/Unsupported runtime mode. Currently only \"performance\" and \"energy\" are supported." << std::endl;
      return false;
   }

   return true;
}

bool launchThread (unsigned int unitId) {
   DVFSUnit& unit = context.topology->getDVFSUnit (unitId);
   ThreadContext *context = unit.getContext ();
   ThreadArgs& args = context->args;
   pthread_t thr = context->thr;
   if (pthread_create (&thr, NULL, FoREST, &args) != 0) {
      LOG (FATAL) << "Error: Failed to create FoREST thread" << std::endl;
      return false;
   }

   return true;
}

static void *FoREST (void *arg) {
   assert (arg != NULL);

   ThreadArgs *args = reinterpret_cast<ThreadArgs*> (arg);
   DecisionMaker* dm = args->dm;

	// only the main thread receives signals
	if (args->id != 0)
	{
		sigset_t set;

		sigemptyset (&set);
		sigaddset (&set, SIGINT);
		sigaddset (&set, SIGUSR1);
		pthread_sigmask (SIG_BLOCK, &set, NULL);
	}

   // do it as long as we are not getting killed by a signal
	while (true) {
      dm->initEvaluation ();
      dm->evaluateFrequency ();
      dm->computeSequence ();
      dm->executeSequence ();
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
      for (std::vector<ThreadContext*>::iterator
           it = context.threadContext.begin ();
           it != context.threadContext.end ();
           it++) {
         Logger *log = Logger::getLog ((*it)->args.unit->getId ());
   		log->logOut ("*");
      }
#endif
	   break;
	default:
			LOG (INFO) << "Received signal #" << nsig << std::endl;
	};
}

static void exitCleanup () {
	// cancel all threads (0 = main process, not a thread)
	for (std::vector<ThreadContext*>::iterator
        it = context.threadContext.begin ();
        it != context.threadContext.end ();
        it++) {
		pthread_cancel ((*it)->thr);
		pthread_join ((*it)->thr, NULL);
	}

#ifdef REST_LOG
	Logger::destroyLog ();
#endif
   delete context.topology;
}

} // namespace FoREST

int main (int argc, char *argv[]) {
   FoREST::Mode mode = FoREST::MODE_PERFORMANCE;

   // Google log library initialization
   google::InitGoogleLogging (argv[0]);
   google::LogToStderr (); 

   // Handle program arguments
   if (!FoREST::handleArguments (argc, argv, mode)) {
      exit (EXIT_SUCCESS);
   }

   FoREST::Topology *topo = new FoREST::Topology (mode, FoREST::context.threadContext);
   unsigned int nbUnits = topo->getNbDVFSUnits ();
   FoREST::context.topology = topo; // Keep track of the topology in the context
 
   // Init FoREST log
#ifdef REST_LOG
   Logger::initLog (nbUnits);
#endif

   // For each unit in the topology, except from the first one
   // The first one will be launched on the current (main) thread
   for (unsigned int unitId = 1; unitId < nbUnits; unitId++) {
      FoREST::launchThread (unitId);
   }

   // Launch the first unit's thread
   FoREST::launchThread (0);

	// Handle USR1 signals in log if needed
#ifdef REST_LOG
	signal (SIGUSR1, FoREST::sigHandler);
#endif

   // Handle Interrupt signal for proper cleanup
	signal (SIGINT, FoREST::sigHandler);
	atexit (FoREST::exitCleanup);

   return EXIT_SUCCESS;
}
