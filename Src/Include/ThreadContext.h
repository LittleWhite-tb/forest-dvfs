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
 * @file ThreadContext.h
 * Thread structures file
 */

#ifndef H_THREADCONTEXT
#define H_THREADCONTEXT

namespace FoREST {
class DecisionMaker;
class Profiler;
class DVFSUnit;
   
// options used to create the threads
struct ThreadArgs
{
	unsigned int id;
	DecisionMaker *dm;
	DVFSUnit *unit;
};

struct ThreadContext {
	pthread_t thr;
   ThreadArgs args;
};

} // namespace FoREST

#endif
