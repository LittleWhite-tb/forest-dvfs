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
  @file Profiler.h
  @brief The Profiler class header is in this file 
 */
#ifndef H_PROFILER
#define H_PROFILER

/**
 * @class Profiler
 * @brief The Profiler of ThreadedProfiler and ProfilerServer 
 */
class Profiler
{
	
	public:
		/**
		 * @brief Constructor
		 */
		Profiler (void);
		
		/**
		 * @brief Destructor
		 */
		virtual ~Profiler (void);
		
		/**
		 * @brief compute the ratio of boudness and cpuness
		 * @return the boundeness computed
		 */
		virtual float ComputeBoudness (void);
};

#endif
