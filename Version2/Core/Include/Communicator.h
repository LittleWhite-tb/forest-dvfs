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
  @file Communicator.h
  @brief The Communicator class header is in this file 
 */
 
#ifndef H_COMMUNICATOR
#define H_COMMUNICATOR

#include "DecisionServer.h"

/**
 * @class Communicator
 * @brief Use to communicate with childs instance of rest on an lesser level
 */
class Communicator: public DecisionServer
{
	public:
		/**
		 * @brief Constructor
		 */
		Communicator (void);
		
		/**
		 * @brief Destructor
		 */
		virtual ~Communicator (void);
		
		/**
		 * @brief bind the communication param to be defined
		 */
		virtual void bindCom (void);
		 
		/**
		 * @brief listen the father's socket param to be defined
 		 * @return return true if success
		 */
		virtual bool listen (void);
		
		/**
		 * @brief send an order to on of its child param to be defined
 		 * @return return true if success
		 */
		virtual bool sendCom (void);
		
		/**
		 * @brief receive a communication from a father instance param to be defined
		 */
		virtual void receiveCom (void);
		
		/**
		 * @brief close a communication established param to be defined
		 */
		virtual void closeCom (void);
		
		/**
		 * @brief gives informations concerning the current state return must be set
		 */
		virtual void getInformation (void);
};

#endif
