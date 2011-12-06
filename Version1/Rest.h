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


//prototypes

#ifdef __cplusplus
extern "C" {
#endif


typedef int (*rest_main_t)(int argc, char** argv, char** env);

typedef void (*rest_lsm_t)(void);

typedef int (*rest_libc_start_main_t)(
        rest_main_t main, int argc, char** ubp_av,
        rest_lsm_t init, rest_lsm_t fini, rest_lsm_t rtld_fini,
        void* stack_end);






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
 * @struct profilerHandle
 * @brief this structure handle the profiler and the STPContext that allow rest to work with the parameters given
 * by the user
 **/
 typedef struct _profilerHandle
 {
 	void *ptr;
 	toolChainInit profiler;
	toolChainInit decisionMaker;
	toolChainInit freqChanger;
 }profilerHandle;


/**
 * @brief initialize the rest system with the components you wish to use
 * @param profiler specify the profiler you wish to use
 * @param decisionMaker specify the decisionmaker you wish to use
 * @param freqChanger specify the frequency changer you wish to use
 **/
int RestInit (toolChainInit profiler, toolChainInit decisionMaker, toolChainInit freqChanger);

/**
 * @brief destroys the context of the rest runtime, returns threads back used for profiling, and dumps all statistics to files
 **/
void RestDestroy ( void );

#ifdef __cplusplus
}
#endif

 

#endif
