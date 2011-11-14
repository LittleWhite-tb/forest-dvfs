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
  @file DecisionServer.h
  @brief The DecisionServer class header is in this file 
 */
#ifndef H_DECISIONSERVER
#define H_DECISIONSERVER

#include "Rest.h"

/**
 * @class DecisionServer
 * @brief Communicate actions with Rest children of this instance
 */
class DecisionServer: public Rest
{
	public:
		/**
		 * @brief Constructor
		 */
		DecisionServer (void);
		
		/**
		 * @brief Destructor
		 */
		virtual ~DecisionServer (void);
		
		/**
		 * @brief gives informations concerning the current state return must be set
		 */
		virtual void getInformation (void);
};

#endif
