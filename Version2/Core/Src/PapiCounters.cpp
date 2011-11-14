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
  @file PapiCounters.cpp
  @brief The PapiCounters class is in this file 
 */
 
#include "PapiCounters.h"

PapiCounters::PapiCounters (void)
{

}
		
PapiCounters::~PapiCounters (void)
{

}

pid_t PapiCounters::getTid ( void )
{
	return 0;
}

void PapiCounters::initPapiHelper ( int * EventSet)
{
	(void) EventSet;
}

void PapiCounters::startCounters (int EventSet)
{
	(void) EventSet;
}

void PapiCounters::accumulator (int EventSet, long_long *values)
{
	(void) EventSet;
	(void) values;
}


bool PapiCounters::closeCounters (void)
{
	return false;
}

unsigned long long PapiCounters::getTicks ( void )
{
	return 0;
}

void PapiCounters::startLibrary (...)
{

}

void PapiCounters::initThreadPapi (void)
{
	
}

PAPI_thread_id_t PapiCounters::threadIdPapi ()
{
	return 0;
}

