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
 @file CoresInfos.cpp
 @brief The  CoresInfos class is in this file
 */

#include <assert.h>
#include <sstream>
#include <iostream>
#include <fstream>

#include "CoresInfos.h"

CoresInfos::CoresInfos (void)
{
	initCpuDatas ();

	//Initilize array of data per core
	coreDatasArray = new std::vector<CoreDatas> ();

	for (int i = 0; i < numCores; i++)
	{
		coreDatasArray->push_back (initCoreDatas (i));
	}
}

CoresInfos::~CoresInfos (void)
{

}

void CoresInfos::initCpuDatas ()
{
	int i, num_core = 0, num_frequency = 0;
	std::string char_buff; //Temp string for reading
	std::vector<int> globalFrequency; //this is a place to temporarily store all the values until we alloc on the heap
	std::ostringstream oss; // Useful for converting string to int
	std::ifstream fp; //File descriptor R

	//Finding all the frequencies and record them
	fp.open (
			"/sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies");

	if (fp == NULL)
	{
		std::cerr << "Failed to open cpufreq datafile\n" << std::endl;
		return;
	}
	else
	{
		int pch = 0; //Freq that we will be reading

		while (fp >> pch)
		{
			globalFrequency.push_back (pch); //Saving the freq

			num_frequency++; //Increasing the number of frequencies
		}
	}
	fp.close ();
	fp.clear ();

	//Now find all proc available on the machine
	fp.open ("/proc/cpuinfo");

	if (fp == NULL)
	{
		std::cerr << "Failed to open the cpuinfo file\n" << std::endl;
		assert(true);
	}
	else
	{
		//if there is a proc line it means that a processor is available
		while (getline (fp, char_buff))
		{
			if (char_buff.find ("processor") != std::string::npos)
			{
				num_core++;
			}
		}
		fp.close ();
		fp.clear ();
	}

	//Paranoid
	assert(num_core > 0);

	//We have all informations so save them
	numFreqs = num_frequency;
	numCores = num_core;

	freqMax = globalFrequency[0];
	freqMin = globalFrequency[num_frequency - 1];

	//Initialize tracker and available frequencies
	for (i = 0; i < num_frequency; i++)
	{
		availableFreqs.push_back (globalFrequency[i]); //Copy frequencies into data structure
	}
}

CoreDatas CoresInfos::initCoreDatas (int coreId)
{
	std::ifstream fp; //File descriptor R
	std::ofstream f; //File descriptor R/W
	std::ostringstream oss; // Usefull for converting string to int

	CoreDatas coreDatas;
	coreDatas.idCore = coreId;

	//Initialize tracker and available frequencies
	for (int i = 0; i < numFreqs; i++)
	{
		coreDatas.freqTrack.push_back (numFreqs); //Initializing the frequencies tracker
	}

	//first set the governor to userspace
	oss << "/sys/devices/system/cpu/cpu" << coreDatas.idCore
			<< "/cpufreq/scaling_governor";
	f.open (oss.str ().c_str ());

	if (f == NULL && f.bad () == true)
	{
		std::cerr << "Error opening file: " << oss.str ().c_str () << std::endl;
		std::cerr << "Perhaps you don't have sudo rights?" << std::endl;
		assert(f.bad());
	}

	//Setting to user space
	f << "userspace";
	f.flush ();
	f.close ();
	f.clear ();

	//Open and Check the file descriptor
	oss.str ("");
	oss << "/sys/devices/system/cpu/cpu" << coreDatas.idCore
			<< "/cpufreq/scaling_setspeed";
	coreDatas.setFile.assign (oss.str ().c_str ());

	fp.open (coreDatas.setFile.c_str ());

	//Paranoid
	if (fp.bad () == true)
	{
		std::cerr << "Error opening file: " << coreDatas.setFile.c_str ()
				<< std::endl;
		std::cerr << "Perhaps you don't have sudo rights?" << std::endl;
		assert(fp.bad() == true);
	}

	fp.close ();
	fp.clear ();

	//We can't know for now current frequency
	coreDatas.currentFreq = -1;

	return coreDatas;
}
