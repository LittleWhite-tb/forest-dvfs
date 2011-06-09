
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "esrv/esrv_lib.h"
#include "timer.h"

#define CONFIGURATION_NAME "power/esrv/config_y210.txt"

static ESRV_LIB_DATA reading;

void display_esrv_lib_data(PESRV_LIB_DATA reading);

unsigned long long timer_init (void)     
{
    ESRV_STATUS ret = ESRV_FAILURE;

    //Open library
    fprintf (stderr, "Opening configuration file: %s\n", CONFIGURATION_NAME);
    ret = esrv_lib_open (CONFIGURATION_NAME);
    assert (ret == ESRV_SUCCESS);

    //Allocate channel
    ret = esrv_lib_allocate_channels(&reading);
    assert(ret == ESRV_SUCCESS);

    ret = esrv_lib_start();
    assert(ret == ESRV_SUCCESS);

    return EXIT_SUCCESS; 
}

unsigned long long timer_close (void) 
{
    ESRV_STATUS ret = ESRV_FAILURE;

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
    ret = esrv_lib_free_channels(&reading);
    assert(ret == ESRV_SUCCESS);

    return EXIT_SUCCESS;
}

unsigned long long timer_reset (void)
{
    ESRV_STATUS ret = ESRV_FAILURE;

    //Open library
    ret = esrv_lib_reset ();
    assert (ret == ESRV_SUCCESS);

    return EXIT_SUCCESS;
}

static double timer_read (void)
{
    ESRV_STATUS ret = ESRV_FAILURE;

    ret = esrv_lib_read(&reading);
    assert(ret == ESRV_SUCCESS);

    return reading.channels_data[0].energy*3600;
}

double timer_start (void)
{ 
    return timer_read ();
}

double timer_stop (void) 
{ 
    return timer_read ();
}
