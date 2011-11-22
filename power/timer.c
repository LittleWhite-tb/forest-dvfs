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


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "esrv/esrv_lib.h"
#include "timer.h"

#define CONFIGURATION_NAME "Libraries/power/esrv/config_y210.txt"

static ESRV_LIB_DATA reading;

void display_esrv_lib_data(PESRV_LIB_DATA reading);

void *evaluationInit (void)     
{
	char buf[256];
    ESRV_STATUS ret = ESRV_FAILURE;
    
    snprintf (buf, sizeof (buf), "%s/%s", MLDIR, CONFIGURATION_NAME);

    //Open library
    fprintf (stderr, "Opening configuration file: %s\n", buf);
    ret = esrv_lib_open (buf);
    assert (ret == ESRV_SUCCESS);

    //Allocate channel
    ret = esrv_lib_allocate_channels(&reading);
    assert(ret == ESRV_SUCCESS);

    ret = esrv_lib_start();
    assert(ret == ESRV_SUCCESS);

    return NULL; 
}

int evaluationClose (void *data) 
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

	(void) data;
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

double evaluationStart (void *data)
{
	(void) data;
    return timer_read ();
}

double evaluationStop (void *data) 
{
	(void) data;
    return timer_read ();
}
