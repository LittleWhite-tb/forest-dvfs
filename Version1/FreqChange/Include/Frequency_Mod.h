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

#ifndef H_FREQUENCY_MOD
#define H_FREQUENCY_MOD

#include <stdio.h>

#define NUM_STATIC_FREQ 30
#define NUMSAMPLES 100000


/**
 * @struct samples
 * @brief structure which hold frequency, core, and time information
 */
struct samples
{
	long long time;/**@brief the time in ticks from RDTSC*/
	int freq;/**@brief frequency given as an index in the index table*/
	int core;/**@brief core id between 0 and numCores*/
};


/**
 * @struct sfreqData
 * @brief data structure containing all the needed information for changing and tracking the frequencies and cores of the system
 */ 
typedef struct sfreqData
{
	int freqMax;         /**< @brief the highest available frequence*/
	int freqMin;         /**< @brief the lowest available frequence*/
	int numFreq;    /**< @brief the number of available frequences*/
	int numCores;	    /**< @brief the number of available cores*/
	FILE **setFile;	/**< @brief pointer to an array of File descriptors of size numCores used to change frequency */
	int *currentFreqs;/**< @brief pointer to an array of size numCores of ints that holds the currentFrequency for */
	int *availableFreqs; /**< @brief the available frequences on the system of size numFreq*/
	int thisSample;	/**< @brief tracks where we are in our sample array*/
	struct samples sampler[NUMSAMPLES]; /**< @brief a sample array for profiling the frequency changer's actions*/
	

	long int **freqTrack; /**< @brief track the frequencies values by core*/
		
}SFreqData;



/**
 * @brief allocates contexts, queries the system for number of frequncies and number of cores, opens up descriptors for 
 * @return handle to all information the frequency changer will need to change frequencies*/
SFreqData * initCpufreq ( void );


/**
 * @brief change the frequence of a specified core
 * @param context with data needed to make change
 * @param core the core id we wish to use
 * @param i is the frequence index we wish to set*/
void changeFreq (SFreqData * context, int core, int i);

/**
 * @brief change the frequence of a specified core
 * @return index in the frequency table which holds the frequency
 * @param context with data needed to read freq
 * @param core the core id we wish to use*/

int readFreq (SFreqData * context, int core);


/**
 * @brief frees contexts and closes all files used by frequency changer
 * @return handle to all information the frequency changer will need to change frequencies*/
void destroyCpufreq (SFreqData * context);




#endif
