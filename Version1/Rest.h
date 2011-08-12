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

/**
 * @enum toolChainInit
 * @brief this enumaration is used to specify the tool chain of REST
 * to be functionnal you have to choose one version of profiler in THREADED_PROFILER,WMAD_PROFILER 
 * then you have to chose a version of the decision maker to use in NAIVE_DM, BRANCHPREDICT_DM, MARKOVPREDICT_DM
 * and at last you have to choose the frequency changer driver in FREQ_CHANGER*/
typedef enum _toolChainInit
{	
	REST_T_PROFILER, /**< @brief init the threaded profiler*/
	REST_VMAD_PROFILER,     /**< @brief init the wmad profiler*/
	REST_NAIVE_DM,		   /**< @brief init the naive decision maker*/
	REST_BRANCHPREDICT_DM,  /**< @brief init the branch prediction like decision maker*/
	REST_MARKOVPREDICT_DM,  /**< @brief init the markovian decision maker*/
	REST_FREQ_CHANGER       /**< @brief init the frequency changer driver*/
}toolChainInit;


/**
 * @brief initialize the rest system with the components you wish to use
 * @param profiler specify the profiler you wish to use
 * @param decisionMaker specify the decisionmaker you wish to use
 * @param freqChanger specify the frequency changer you wish to use
 * @return a handle to the rest runtime context
 **/
void * RestInit (toolChainInit profiler, toolChainInit decisionMaker, toolChainInit freqChanger);

/**
 * @brief destroys the context of the rest runtime and returns the threads used for profiling
 * @param profiler the profiler used to init the runtime
 * @param ptr the handle used for the destroy
 **/
void RestDestroy (toolChainInit profiler, void *ptr);

 

#endif
