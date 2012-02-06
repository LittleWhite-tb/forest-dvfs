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
  @file Profiler.cpp
  @brief The Profiler class is in this file
 */

#include <unistd.h>
#include <iostream>
#include <sstream>
#include <math.h>
#include <cstdlib>
#include <string.h>
#include <assert.h>
#include <string>
#include <dlfcn.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "Profiler.h"
#include "ThreadedProfiler.h"
#include "YellowPages.h"
#include "Log.h"
#include "Config.h"

Profiler::Profiler (void)
{
   this->comm = new Communicator ();
}

Profiler::~Profiler (void)
{
   delete this->comm;
}

//
// Bellow is the profiler starting point
//

// locally used functions
static void profiler_cleanup ();
static int prof_main (int argc, char ** argv, char ** env);

// crappy global variables used by the profiler
static int (*original_main) (int argc, char ** argv, char ** env);
static ThreadedProfiler * tp;
static Log * logger;

static bool mainProf = true;
static unsigned int nbProcs = 0;
static pid_t * sons = NULL;

// timer and power librairies symbols
typedef double (*evalGet) (void * data);
typedef void * (*evalInit) (void);
typedef int (*evalClose) (void * data);

void * dlPower;
void * dlTimer;

static evalInit timer_init;
static evalGet timer_start;
static evalGet timer_stop;
static evalClose timer_close;

static evalInit power_init;
static evalGet power_start;
static evalGet power_stop;
static evalClose power_close;

static double timer_begvalue;
static double timer_endvalue;
static double power_begvalue;
static double power_endvalue;


/**
 * Performs a clean termination of the profiler.
 * Called thanks to atexit.
 */
static void profiler_cleanup ()
{
   // the main profiler waits for its sons if they forked
   if (mainProf && sons != NULL)
   {
      for (unsigned int i = 0; i < nbProcs; i++)
      {
         waitpid (sons[i], NULL, 0);
      }

      delete[] sons;
   }

   if (dlPower != NULL)
   {
      power_endvalue = power_stop (NULL);
      power_close (NULL);
      dlclose (dlPower);
      logger->LOG (Log::VERB_NFO, "Power consumed: %f\n", power_endvalue - power_begvalue);
   }

   if (dlTimer != NULL)
   {
      timer_endvalue = timer_stop (NULL);
      timer_close (NULL);
      dlclose (dlTimer);
      logger->LOG (Log::VERB_NFO, "Time consumed : %f\n", timer_endvalue - timer_begvalue);
   }

   // cleanup stuff
   delete tp;
   YellowPages::reset ();
   Log::close_logs ();
}


/**
 * Fake main called instead of the original program main.
 */
static int prof_main (int argc, char ** argv, char ** env)
{
   std::ostringstream oss;

   // checks arguments
   if (argc < 3)
   {
      std::cerr << "Usage: " << argv [0] << " id config [program options]" << std::endl;
      return 1;
   }

   // do not spawn on all the subprocess we will launch later
   unsetenv ("LD_PRELOAD");

   // parse arguments
   unsigned int id;
   std::istringstream iss (argv [1], std::istringstream::in);
   iss >> id;

   // load configuration
   Config cfg = Config (argv [2]);

   // handle the profiler fork option
   if (cfg.profFork())
   {
      nbProcs = sysconf (_SC_NPROCESSORS_ONLN);
      sons = new pid_t[nbProcs];

      for (unsigned int i = 0; i < nbProcs - 1; i++)
      {
         if ( (sons[i] = fork()) == 0)
         {
            mainProf = false;

            // we do not need that
            delete[] sons;
            sons = NULL;

            // increment our id
            id += i + 1;

            std::cout << "forking profiler " << id << std::endl;

            break;
         }
      }
   }

   YellowPages::initFrom (id, cfg);
   logger = Log::get_log (LOG_ID());

   // ensure that I'm pinned to my core
   std::cout << oss.str().c_str() << std::endl;
   if (system (oss.str().c_str()) == -1)
   {
      std::cerr << "Failed to pin profiler " << id << " to core " << YellowPages::getCoreId (id) << std::endl;
   }

   // start the profiler
   tp = new ThreadedProfiler ();

   // what we have to do in case of exit
   atexit (profiler_cleanup);

   // load and start power and timer libraries
   dlPower = NULL;
   power_init = NULL;
   power_start = NULL;
   power_stop = NULL;
   power_close = NULL;

   if (mainProf)
   {
      dlPower = dlopen ("/opt/rest_modifications/power/timer.so", RTLD_NOW);

      // skip if the lib cannot be found
      if (dlPower == NULL)
      {
         std::cerr << "Cannot open power library: " << dlerror () << std::endl;
      }
      else
      {
         power_init = (evalInit) dlsym (dlPower, "evaluationInit");
         assert (power_init != NULL);
         power_start = (evalGet) dlsym (dlPower, "evaluationStart");
         assert (power_start != NULL);
         power_stop = (evalGet) dlsym (dlPower, "evaluationStop");
         assert (power_stop != NULL);
         power_close = (evalClose) dlsym (dlPower, "evaluationClose");
         assert (power_close != NULL);
      }

      dlTimer = dlopen ("/opt/rest_modifications/timer/timer.so", RTLD_NOW);

      // skip if the lib cannot be found
      if (dlTimer == NULL)
      {
         std::cerr << "Cannot open timer library: " << dlerror () << std::endl;
      }
      else
      {
         timer_init = (evalInit) dlsym (dlTimer, "evaluationInit");
         assert (timer_init != NULL);
         timer_start = (evalGet) dlsym (dlTimer, "evaluationStart");
         assert (timer_start != NULL);
         timer_stop = (evalGet) dlsym (dlTimer, "evaluationStop");
         assert (timer_stop != NULL);
         timer_close = (evalClose) dlsym (dlTimer, "evaluationClose");
         assert (timer_close != NULL);
      }
   }

   // start measuring power
   if (dlPower != NULL)
   {
      power_init ();
      power_begvalue = power_start (NULL);
   }

   // start measuring time
   if (dlTimer != NULL)
   {
      timer_init ();
      timer_begvalue = timer_start (NULL);
   }

   // call the main and skip our arguments
   argv [2] = argv [0];
   return original_main (argc - 2, argv + 2, env);
}

// we redefine libc_start_main because at this point main is an arguement so we
// can store it in our symbol table and hook in
extern "C" int __libc_start_main (int (*main) (int, char **, char **), int argc,
                                  char ** ubp_av, void (*init) (), void (*fini) (), void (*rtld_fini) (),
                                  void * stack_end)
{
   // remember where is the original main function
   original_main = main;

   // find the actual libc_start_main
   int (*real_start) (int (*main) (int, char **, char **), int argc,
                      char ** ubp_av, void (*init) (), void (*fini) (), void (*rtld_fini) (),
                      void* stack_end) = (int ( *) (int ( *) (int, char **, char **), int, char **,
                                          void ( *) (), void ( *) (), void ( *) (), void *))
                                         dlsym (RTLD_NEXT, "__libc_start_main");

   // call the real libc_start_main such as it calls our main
   return real_start (prof_main, argc, ubp_av, init, fini, rtld_fini, stack_end);
}
