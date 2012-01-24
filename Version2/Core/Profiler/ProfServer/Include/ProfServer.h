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
  @file ProfServer.h
  @brief The ProfServer class header is in this file 
 */
 
#ifndef H_PROFSERVER
#define H_PROFSERVER

#include "Profiler.h"

/**
 * @class ProfServer
 * @brief Use to handle external and internal Profiler informations
 */
class ProfServer: public Profiler
{
	public:
		/**
		 * @brief Constructor
		 */
		ProfServer (void);
		
		/**
		 * @brief Destructor
		 */
		virtual ~ProfServer (void);	
};

#endif