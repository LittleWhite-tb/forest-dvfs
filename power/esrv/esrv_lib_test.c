/**
*** INTEL CONFIDENTIAL
*** 
*** Copyright (March 2011) (March 2011) Intel Corporation All Rights Reserved. 
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

//-----------------------------------------------------------------------------
// Note:
//    This sample uses a configuration file. The name of the file is the 
//    argument of the esrv_lib_open function. The content of the file is a 
//    single command line. The command is the SAME as the command used
//    to run ESRV in standatd mode. For example, two configuration files 
//    could be created. One to be used with the ESRV simulated device and 
//    one with the Yokogawa WT210 power analyzer. See below the content of
//    these files
//    config_y210.txt:
//-----------------------------------------------------------------------------
// esrv --start --device y210 --device_options "items=all" --diagnostic --interface_options "com=0"
//-----------------------------------------------------------------------------
//    config_simulated.txt:
//-----------------------------------------------------------------------------
// esrv --start --library esrv_simulated_device.dll --diagnostic
//-----------------------------------------------------------------------------
// Note:
//    The --diagnostic option can be removed to silence debug messages.
// Note:
//    The library requires that the device has hardware integration of power
//    to compute energy. Software integration would not work since the time 
//    between calls is expected to be short.
// Note:
//    For similar reason as above, the Yokogawa WT210 driver doesn't check
//    for energy overlow conditions. This should never happen.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Headers inclusions
//-----------------------------------------------------------------------------
#include <assert.h>
#include <stdio.h> // for FILE *
#ifdef __PL_WINDOWS__
	#include <windows.h>
#endif // __PL_WINDOWS__
#include "esrv_lib.h"

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define LOOP_COUNT 40
#define HEADER "|Channel count|              TSC| Channel| Power (Watt)| Min (Watt)| Max (Watt)| Energy (Wh)|\n"
#define HEADER_UNDERSCORE "+-------------+-----------------+--------+-------------+-----------+-----------+------------+\n"
#define CONFIG_FILE_NAME "config_simulated.txt"

//-----------------------------------------------------------------------------
// Prototypes
//-----------------------------------------------------------------------------
void display_esrv_lib_data(PESRV_LIB_DATA);

/*-----------------------------------------------------------------------------
Function: main
Purpose : demo esrv lib usage
In      : none
Out     : none
Return  : status

History
-------------------------------------------------------------------------------
Date        : Author                  Modification
-------------------------------------------------------------------------------
03/08/2011    Jamel Tayeb             Creation.
*/
int main(int argc, char *argv[]) {

	int i = 0;
	char *configuration_file_name = NULL;
	char default_configuration_file_name[] = CONFIG_FILE_NAME;
	FILE *output = NULL;

	ESRV_LIB_DATA reading_1 = { 0 };
	ESRV_LIB_DATA reading_2 = { 0 };
	ESRV_STATUS ret = ESRV_FAILURE;

	//-------------------------------------------------------------------------
	// Open the device using either the default config file or the argument.
	//-------------------------------------------------------------------------
	if(argc != 2) {
		configuration_file_name = default_configuration_file_name;
	} else {
		configuration_file_name = argv[1];
	}
    fprintf (stderr, "Here %s\n", configuration_file_name);
	ret = esrv_lib_open(configuration_file_name);
	assert(ret == ESRV_SUCCESS);

	//-------------------------------------------------------------------------
	// Allocate channel data.
	// Note:
	//    Since devices may mave more than one channel - known only once the 
	//    device driver is loaded -, the memory required to store that channels
	//    data must be allocated after the esrv_lib_open call is performed.
	// Note:
	//    Use esrv_lib_free_channels to free memory allocated with the
	//    esrv_lib_allocate_channels calls.
	//-------------------------------------------------------------------------
	ret = esrv_lib_allocate_channels(&reading_1);
	assert(ret == ESRV_SUCCESS);
	ret = esrv_lib_allocate_channels(&reading_2);
	assert(ret == ESRV_SUCCESS);

	//-------------------------------------------------------------------------
	// Set the reading mode for each data structure.
	// Note:
	//    If non blocking reading mode is set for a data structure, then the 
	//    library will not wait until a new reading is available from the 
	//    device. The last reading is returned (but it is still read from the
	//    divice, so the driver execution time is always identical. This 
	//    behavior may change based on specific devive behavior though).
	// Note:
	//    By default, the blocking read mode is selected. This behavior can be
	//    changed by re-defining ESRV_LIB_DEFAULT_READ_MODE in esrv_lib.h.
	// Note:
	//    The reading mode applies to all channels of the device. It is not
	//    possible to set the reading mode per channel for a multi-channel
	//    device.
	//-------------------------------------------------------------------------
	reading_1.reading_mode = ESRV_LIB_BLOCKING_READ;
	reading_2.reading_mode = ESRV_LIB_NON_BLOCKING_READ;

	//-------------------------------------------------------------------------
	// Print a header.
	//-------------------------------------------------------------------------
	output = stdout;
	fprintf(
		output,
		HEADER_UNDERSCORE
	);
	fprintf(
		output,
		HEADER
	);
	fprintf(
		output,
		HEADER_UNDERSCORE
	);

	//-------------------------------------------------------------------------
	// Start the energy integration.
	//-------------------------------------------------------------------------
	ret = esrv_lib_start();
	assert(ret == ESRV_SUCCESS);

	//-------------------------------------------------------------------------
	// Reset the device and any running integration.
	// Note:
	//    In this sample, we want to measure power and energy accumulated at
	//    each loop trip. Since seting-up and starting a device may require
	//    lot of time - compared to a device read - we reset the device just
	//    before entering the main loop.
	//-------------------------------------------------------------------------
	ret = esrv_lib_reset();
	assert(ret == ESRV_SUCCESS);

	//-------------------------------------------------------------------------
	// Read the device to collect reference data.
	//-------------------------------------------------------------------------
	ret = esrv_lib_read(&reading_1);
	assert(ret == ESRV_SUCCESS);

	for(i = 0; i < LOOP_COUNT; i++) {

		//---------------------------------------------------------------------
		// Do something...
		//---------------------------------------------------------------------
		// TODO:

		//---------------------------------------------------------------------
		// Read the device to collect intermediate data and cumulative energy.
		//---------------------------------------------------------------------
		ret = esrv_lib_read(&reading_2);
		assert(ret == ESRV_SUCCESS);

		//---------------------------------------------------------------------
		// Print the intermediate data and cumulative energy.
		//---------------------------------------------------------------------
		display_esrv_lib_data(&reading_2);

	} // for i

	//-------------------------------------------------------------------------
	// Print the last intermediate data and cumulative energy (recap).
	//-------------------------------------------------------------------------
	fprintf(
		output,
		HEADER_UNDERSCORE
	);
	fprintf(
		output,
		HEADER
	);
	fprintf(
		output,
		HEADER_UNDERSCORE
	);
	display_esrv_lib_data(&reading_2);
	fprintf(
		output,
		HEADER_UNDERSCORE
	);

	//-------------------------------------------------------------------------
	// Compute the difference between the reference and the last intermediate
	// data. Difference is computed as reading_2 = reading_2 - reading_1. 
	// Note:
	//    Only TSC, power and energy are substracted. Min & max power are not. 
	//-------------------------------------------------------------------------
	ret = esrv_lib_diff(
		&reading_2, 
		&reading_1
	);
	assert(ret == ESRV_SUCCESS);

	//-------------------------------------------------------------------------
	// Print the difference.
	//-------------------------------------------------------------------------
	display_esrv_lib_data(&reading_2);
	fprintf(
		output,
		HEADER_UNDERSCORE
	);

	//-------------------------------------------------------------------------
	// Stop the energy integration.
	//-------------------------------------------------------------------------
	ret = esrv_lib_stop();
	assert(ret == ESRV_SUCCESS);

	//-------------------------------------------------------------------------
	// Close the device.
	//-------------------------------------------------------------------------
	ret = esrv_lib_close();
	assert(ret == ESRV_SUCCESS);

	//-------------------------------------------------------------------------
	// Free channel data.
	//-------------------------------------------------------------------------
	ret = esrv_lib_free_channels(&reading_1);
	assert(ret == ESRV_SUCCESS);
	ret = esrv_lib_free_channels(&reading_2);
	assert(ret == ESRV_SUCCESS);

	return(ESRV_SUCCESS);
}

/*-----------------------------------------------------------------------------
Function: display_esrv_lib_data
Purpose : display the members of an esrv lib data structure
In      : pointer to an esrv lib data structure
Out     : none
Return  : none

History
-------------------------------------------------------------------------------
Date        : Author                  Modification
-------------------------------------------------------------------------------
03/08/2011    Jamel Tayeb             Creation.
*/
void display_esrv_lib_data(PESRV_LIB_DATA reading) {

	size_t i = 0;
	FILE *f = NULL;

	assert(reading != NULL);
	assert(reading->channel_count > 0);

	f = stdout;

	fprintf(
		f,
		"|           %02u| %016llu|",
		(unsigned int)reading->channel_count,
		reading->tsc
	);

	for(i = 0; i < reading->channel_count; i++) {
		fprintf(
			f,
			"      %02u|   %+010.8g| %+010.8g| %+010.8g|  %+010.8Lg|\n",
			(unsigned int)(i + 1),
			reading->channels_data[i].power,
			reading->channels_data[i].min_power,
			reading->channels_data[i].max_power,
			reading->channels_data[i].energy
		);
	}
}
