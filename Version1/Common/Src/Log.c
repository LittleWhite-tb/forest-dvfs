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
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

#include "Log.h"

//Static variables
static int logVerbosity = 5;     /**implicit declaration of function ‘Log_init’< @brief Log verbosity, used to check if we log something or not */
static FILE *logOutput = NULL;   /**< @brief Log output */

void Log_setVerbosity (int value)
{
	logVerbosity = value;
}

void Log_setOutput (FILE *f)
{
	logOutput = f;
}

void Log_output (int verbosity, char *fmt, ...)
{
	char buf[1024];

	if (fmt == NULL)
	{
		return;
	}
	
	snprintf(buf, 24, "#%d: ", getpid());
	strncat(buf, fmt, 1000);
    
	//We accept anything under 0 or it has to be above logVerbosity
	if (verbosity < 0 || verbosity >= logVerbosity)
	{
		if (logOutput != NULL)
		{
			//Get vararg and call vfprintf
			va_list ap;
			va_start (ap, fmt);
			vfprintf (logOutput, buf, ap);
			va_end (ap);
		}
	}
}

void Log_init()
{
	if(getenv("REST_OUTPUT") !=NULL && strcmp(getenv("REST_OUTPUT"), " ") == 0)
    	{
		char path[256];
		sprintf(path, "%s", getenv("REST_OUTPUT"));
		
		if(path[(strlen(path)-1)] != '/')
			sprintf(path, "/");
		
		strcat(path, "Log");

		logOutput = fopen(path, "w+");
		if(logOutput == NULL)
		{
			logOutput = stderr;
			Log_output(15, "REST_OUTPUT: Wrong directory: %s output will be redirected on stderr.\n", path);
		}
	}
	else
	{
		logOutput = stderr;	
		Log_output(15, "REST_OUTPUT: output will be redirected on stderr.\n");
	}
}

void Log_destroy()
{
	if(getenv("REST_OUTPUT") !=NULL && strcmp(getenv("REST_OUTPUT"), " ") == 0)
    		fclose (logOutput);
}
