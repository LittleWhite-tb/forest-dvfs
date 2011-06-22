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

#ifndef FREQUENCY_MOD_H
#define FREQUENCY_MOD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "define.h"

#define NUM_STATIC_FREQ 20

/**
 * @brief change the frequence of a specified core
 * @param core the core id we wish to use
 * @param i is the frequence index we wish to set*/
void changeFreq(int core, int i);

/**
 * @brief get all the frequences available on the system
 * @return the frequences available on the system as some other information (frequences max, min and the number of available frequences*/
freqData *getAllAvailableFreq();

/**
 * @brief return the frequences value of the specified core
 * @param the proc id we wish to use
 * @return the current frequence*/
int getTheFreqById(int procId);



extern int globalFrequency[ NUM_STATIC_FREQ];
extern int num_frequency;

#endif
