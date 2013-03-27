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
 @file FoREST.h
 @brief The FoREST class header is in this file
 */

#ifndef FOREST_H
#define FOREST_H

#include "Topology.h"
#include "ThreadContext.h"
#include "Config.h"

namespace FoREST {

/**
 * Context structure keeping globally track of the main program structures
 * Allow the program to exit properly when atexit handler or signal handlers
 * are called
 */
struct Context {
   /**
    * A structure containing the machine topology, but also holds DVFS Units, 
    * and corresponding decision makers / profilers
    */
   Topology *topology;

   /**
    * A vector of thread contexts to keep track of correspnding DVFSUnit structures
    * Each DVFSUnit profiles, decides and executes with its own separate thread, its own DecisionMaker and its own Profiler
    */
   std::vector<ThreadContext*> threadContext;

   /**
    * The Config structure represents the program tuning parameters, as provided in config.cfg
    */
   Config *config;
};

} // namespace FoREST

#endif
