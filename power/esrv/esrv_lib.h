/**
*** INTEL CONFIDENTIAL
*** 
*** Copyright (October 2008) (October 2008) Intel Corporation All Rights Reserved. 
*** The source code contained or described herein and all documents related to the
*** source code ("Material") are owned by Intel Corporation or its suppliers or 
*** licensors. Title to the Material remains with Intel Corporation or its 
*** suppliers and licensors. The Material contains trade secrets and proprietary 
*** and confidential information of Intel or its suppliers and licensors. 
*** The Material is protected by worldwide copyright and trade secret laws 
*** and treaty provisions. No part of the Material may be used, copied, 
*** reproduced, modified, published, uploaded, posted, transmitted, distributed,
*** or disclosed in any way without Intel's prior express written permission.
***
*** No license under any patent, copyright, trade secret or other intellectual
*** property right is granted to or conferred upon you by disclosure or delivery
*** of the Materials, either expressly, by implication, inducement, estoppel or
*** otherwise. Any license under such intellectual property rights must be 
*** express and approved by Intel in writing.
**/

#ifndef __INCLUDE_ESRV_LIB__
#define __INCLUDE_ESRV_LIB__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
/*--------------------------------------------------------------------------*/

//-----------------------------------------------------------------------------
// ESRV_API API definition
//-----------------------------------------------------------------------------
#ifdef __PL_WINDOWS__

	//-------------------------------------------------------------------------
	// All files compiled for a DLL must be with ESRV_LIB_EXPORTS symbol 
	// defined this symbol should not be defined on any project that uses this
	// DLL. 
	//-------------------------------------------------------------------------
	#ifdef ESRV_LIB_EXPORTS
		#ifdef __cplusplus 
			#define ESRV_LIB_API __declspec(dllexport) extern "C"
		#else // __cplusplus
			#define ESRV_LIB_API __declspec(dllexport) extern
		#endif // __cplusplus
	#else // ESRV_LIB_EXPORTS
		#ifdef __cplusplus 
			#define ESRV_LIB_API __declspec(dllimport) extern "C"
		#else // __cplusplus
			#define ESRV_LIB_API __declspec(dllimport) extern
		#endif // __cplusplus
	#endif // ESRV_LIB_EXPORTS
#endif // __PL_WINDOWS__
#if defined (__PL_LINUX__) || (__PL_SOLARIS__) || (__PL_MACOSX__)
	#define ESRV_LIB_API extern
#endif // __PL_LINUX__ || __PL_SOLARIS__ || __PL_MACOSX__

//-----------------------------------------------------------------------------
// Structures & enums.
//-----------------------------------------------------------------------------
#ifndef __INCLUDE_ESRV__

	typedef enum _esrv_status {

		ESRV_SUCCESS,
		ESRV_FAILURE

	} ESRV_STATUS, *PESRV_STATUS;

	typedef enum _esrv_failure {

		ESRV_NO_ERROR = 0x10001000,
		ESRV_CANNOT_SET_MEMORY_DEBUG,
		ESRV_INVALID_PARAMETERS,
		ESRV_INVALID_INTERFACE_PARAMETERS,
		ESRV_INVALID_EXTRA_DEVICE_DATA,
		ESRV_INVALID_DEVICE_OPTION,
		ESRV_OPEN_INTERFACE,
		ESRV_OPEN_DEVICE,
		ESRV_CLOSE_DEVICE,
		ESRV_CLOSE_INTERFACE,
		ESRV_UNKNOWN_COMMAND,
		ESRV_START_KERNEL,
		ESRV_CREATE_HEXIT,
		ESRV_INVALID_IPC,
		ESRV_INVALID_COMMAND,
		ESRV_INSTALL_EVENT_HANDLER,
		ESRV_UNINSTALL_EVENT_HANDLER,
		ESRV_DESTROY_HEXIT,
		ESRV_IPC_ERROR,
		ESRV_START_MESSAGE_LOOP,
		ESRV_INVALID_VIRTUAL_DEVICE_COUNT,
		ESRV_MAKE_WINDOW,
		ESRV_OPEN_WINDOW,
		ESRV_UNABLE_TO_LOAD_DEVICE,

		ESRV_WRONG_HW_CONFIGURATION,
		ESRV_HP_TIMER_SETUP,
		ESRV_HP_TIMER_READ,
		ESRV_KERNEL_PRIORITY_BOOST,
		ESRV_KERNEL_OUT_OF_MEMORY,
		ESRV_KERNEL_CREATE_SYNC_OBJECT,
		ESRV_KERNEL_DESTROY_SYNC_OBJECT,
		ESRV_KERNEL_MEASURE_TIME,

		// DAQ errors
		ESRV_DAQ_HP_TIMER_SETUP,
		ESRV_DAQ_HP_TIMER_READ,
		ESRV_DAQ_KERNEL_PRIORITY_BOOST,
		ESRV_DAQ_KERNEL_OUT_OF_MEMORY,
		ESRV_DAQ_KERNEL_BUILD_EXPRESSION,
		ESRV_DAQ_KERNEL_EVAL_EXPRESSION,
		ESRV_DAQ_KERNEL_INIT_STACKS,
		ESRV_DAQ_KERNEL_CREATE_SYNC_OBJECT,
		ESRV_DAQ_KERNEL_CREATE_MATH_THREAD,
		ESRV_DAQ_KERNEL_DESTROY_SYNC_OBJECT,
		ESRV_DAQ_KERNEL_MEASURE_TIME,
		ESRV_DAQ_KERNEL_MATH_THREAD_SYNCHRONIZATION,
		ESRV_DAQ_INVALID_CHANNEL_COUNT,

	} ESRV_ERROR, *PESRV_ERROR;

#endif // __INCLUDE_ESRV__

//-----------------------------------------------------------------------------
// Defines.
//-----------------------------------------------------------------------------
#define ESRV_LIB_BLOCKING_READ 0
#define ESRV_LIB_NON_BLOCKING_READ 1
#define ESRV_LIB_DEFAULT_READ_MODE ESRV_LIB_BLOCKING_READ

//-----------------------------------------------------------------------------
// Library channel data structure.
//-----------------------------------------------------------------------------
typedef struct _esrv_lib_channel_data {

	double power;
	double min_power;
	double max_power;
	long double energy;

} ESRV_LIB_CHANNEL_DATA, *PESRV_LIB_CHANNEL_DATA;

//-----------------------------------------------------------------------------
// Library data structure.
//-----------------------------------------------------------------------------
typedef struct _esrv_lib_data {

	int reading_mode;
	size_t channel_count;
	unsigned long long int tsc;
	PESRV_LIB_CHANNEL_DATA channels_data;

} ESRV_LIB_DATA, *PESRV_LIB_DATA;

//-----------------------------------------------------------------------------
// functions prototype
//-----------------------------------------------------------------------------
ESRV_LIB_API ESRV_STATUS esrv_lib_allocate_channels(PESRV_LIB_DATA);
ESRV_LIB_API ESRV_STATUS esrv_lib_free_channels(PESRV_LIB_DATA);
ESRV_LIB_API ESRV_STATUS esrv_lib_open(char *);
ESRV_LIB_API ESRV_STATUS esrv_lib_close(void);
ESRV_LIB_API ESRV_STATUS esrv_lib_start(void);
ESRV_LIB_API ESRV_STATUS esrv_lib_stop(void);
ESRV_LIB_API ESRV_STATUS esrv_lib_reset(void);
ESRV_LIB_API ESRV_STATUS esrv_lib_read(PESRV_LIB_DATA);
ESRV_LIB_API ESRV_STATUS esrv_lib_diff(PESRV_LIB_DATA, PESRV_LIB_DATA);

/*--------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __INCLUDE_ESRV_LIB__
