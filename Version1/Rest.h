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

#ifndef H_REST
#define H_REST

#include "DecisionMaker.h"
#include "ThreadedProfiler.h"

/**@brief this enumaration is used to specify the tool chain of REST
 * to be functionnal you have to choose one version of profiler in THREADED_PROFILER,WMAD_PROFILER 
 * then you have to chose a version of the decision maker to use in NAIVE_DM, BRANCHPREDICT_DM, MARKOVPREDICT_DM
 * and at last you have to choose the frequency changer driver in FREQ_CHANGER*/
typedef enum _toolChainInit
{	
	T_PROFILER, /**< @brief init the threaded profiler*/
	WMAD_PROFILER,     /**< @brief init the wmad profiler*/
	NAIVE_DM,		   /**< @brief init the naive decision maker*/
	BRANCHPREDICT_DM,  /**< @brief init the branch prediction like decision maker*/
	MARKOVPREDICT_DM,  /**< @brief init the markovian decision maker*/
	FREQ_CHANGER       /**< @brief init the frequency changer driver*/
}toolChainInit;


SFuncsToUse decisionFuncs; 
#endif
