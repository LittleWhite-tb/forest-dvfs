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
 * @file FoREST.cpp
 * Entry point for FoREST. A thread will be started for each unit.
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

#include "Topology.h"
#include "DecisionMaker.h"
#include "Profiler.h"
#include "ThreadContext.h"
#include "FoREST.h"

#ifdef REST_LOG
	#include "Logger.h"
#endif

using namespace FoREST;

// Local Functions
static void *FoRESTthread (void * arg);
static void exitCleanup ();
static void sigHandler (int nsig);
static inline bool handleArguments (int argc, char *argv [], Mode& mode);
static inline bool launchThread (ThreadContext *context);

// The program context to keep track of important data structures at global level
static Context context;

int main (int argc, char *argv[]) {
   Mode mode = MODE_PERFORMANCE;

   // Google log library initialization
   google::InitGoogleLogging (argv[0]);
   google::LogToStderr (); 

   // Handle program arguments
   if (!handleArguments (argc, argv, mode)) {
      exit (EXIT_SUCCESS);
   }

   Config *config = new Config ();
   Topology *topo = new Topology (mode, config);
   unsigned int nbUnits = topo->getNbDVFSUnits ();
   
   // Keep track of the context at global scope
   context.topology = topo; 
   context.config = config;
 
   // Init FoREST log
#ifdef REST_LOG
   Logger::initLog (nbUnits);
#endif

   // For each unit in the topology, except from the first one
   // The first one will be launched on the current (main) thread
   for (unsigned int unitId = 1; unitId < nbUnits; unitId++) {
      ThreadContext *newContext = new ThreadContext ();
      context.threadContext.push_back (newContext);
      newContext->unit = topo->getDVFSUnit (unitId);
      launchThread (newContext);
   }

	// Handle USR1 signals in log if needed
#ifdef REST_LOG
	signal (SIGUSR1, sigHandler);
#endif

   // Handle Interrupt signal for proper cleanup
	signal (SIGINT, sigHandler);
	atexit (exitCleanup);

   // Launch the first unit's thread
   ThreadContext *newContext = new ThreadContext ();
   context.threadContext.push_back (newContext);
   newContext->unit = topo->getDVFSUnit (0);
   FoRESTthread (reinterpret_cast <void*> (newContext));

   return EXIT_SUCCESS;
}

/**
 * handleArguments
 * Gets which mode is selected by the user as the program arguments
 *
 * @param argc main argc argument
 * @param argv main argv argument
 * @param mode the mode reference to be set according to the given
 * program parameters
 */
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

/**
 * launchThread
 * Wrappers that spawns a new thread appropriately given a ThreadContext
 */
bool launchThread (ThreadContext *context) {
   if (pthread_create (&context->thr, NULL, FoRESTthread, context) != 0) {
      LOG (FATAL) << "Error: Failed to create FoREST thread" << std::endl;
      return false;
   }

   return true;
}

/**
 * Function to be executed by each thread
 */
static void *FoRESTthread (void *arg) {
   assert (arg != NULL);

   ThreadContext *context = reinterpret_cast<ThreadContext*> (arg);
   DecisionMaker* dm = context->unit->getDecisionMaker ();
   unsigned int id = context->unit->getId ();

	// only the main thread receives signals
	if (id != 0)
	{
		sigset_t set;

		sigemptyset (&set);
		sigaddset (&set, SIGINT);
		sigaddset (&set, SIGUSR1);
		pthread_sigmask (SIG_BLOCK, &set, NULL);
	}

   // do it as long as we are not getting killed by a signal
	while (true) {
      //std::cerr << "new eval" << std::endl;
      dm->initEvaluation ();
      dm->evaluateFrequency ();
      dm->computeSequence ();
      while (!dm->executeSequence ());
   }

	// pacify compiler but we never get out of while loop
	return NULL;
}

/**
 * sigHandler
 * Handles standard signals
 *
 * @param nsig the signal number to handle
 */
static void sigHandler (int nsig)
{
	switch (nsig) {
	case SIGINT:
			exit (EXIT_FAILURE);
			break;
	case SIGUSR1:
   // In this case we want to add a marker in each
   // thread log file
#ifdef REST_LOG
      for (std::vector<ThreadContext*>::iterator
           it = context.threadContext.begin ();
           it != context.threadContext.end ();
           it++) {
         Logger *log = Logger::getLog ((*it)->unit->getId ());
   		log->logOut ("*");
      }
#endif
	   break;
	default:
			LOG (INFO) << "Received signal #" << nsig << std::endl;
	};
}

/**
 * exitCleanup
 * Function called at program exit which takes advantage of a global program
 * context to join threads, free memory and exit the program properly
 */
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
   delete context.config;
   delete context.topology;
}

