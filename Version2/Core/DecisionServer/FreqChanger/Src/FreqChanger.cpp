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
 @file FreqChanger.cpp
 @brief The  FreqChanger class is in this file
 */

#include <assert.h>
#include <sstream>
#include <iostream>
#include <fstream>

#include "FreqChanger.h"

FreqChanger::FreqChanger (CoresInfos *coresInfos)
{
	//Saving data core' references
	this->coresInfos = coresInfos;

	assert(this->coresInfos);

	//Initializing the current freq
	for (int i = 0; i < coresInfos->numCores; i++)
	{
		coresInfos->coreDatasArray->at (i).currentFreq = ReadCurrentFreq (i);
	}
}

FreqChanger::~FreqChanger (void)
{

}

int FreqChanger::ReadCurrentFreq (int coreId)
{
	std::string curFreqString = ""; //Temp string for the current frequency
	std::istringstream iss; //Convert string to int
	std::ostringstream oss; //Convert int to string
	std::ifstream fp; //File descriptor
	int curFreq = 0;

	//Setting and Opening path leading to the cpuinfo data
	oss << "/sys/devices/system/cpu/cpu" << coreId
			<< "/cpufreq/cpuinfo_cur_freq";
	fp.open (oss.str ().c_str ());

	//Reading the current frequency
	//Paranoid
	if (fp.bad () == true)
	{
		std::cerr << "Failed to open cpufreq datafile" << std::endl;
		return -1;
	}
	else
	{
		//Reading the current frequency
		if (fp.bad () == false)
		{
			getline (fp, curFreqString);
			iss.str (curFreqString);
			iss >> curFreq;
		}
		//Paranoid
		else
		{
			std::cerr << "file open call failed somehow" << std::endl;
			return -1;
		}
	}
	fp.close ();
	fp.clear();

	return curFreq;
}

int FreqChanger::ReadFreq (int coreId)
{
	return coresInfos->coreDatasArray->at (coreId).currentFreq;
}

void FreqChanger::ChangeFreq (int coreId, int freqId)
{
	//long long int time = getTicks();
	std::ostringstream oss; //Convert string to int
	std::ofstream f;
	CoreDatas *coreDatas = &coresInfos->coreDatasArray->at (coreId);

	coreDatas->freqTrack[freqId]++;

	f.open (coreDatas->setFile.c_str ());

	f.seekp (0, std::ios::beg);
	oss << coresInfos->availableFreqs[freqId];
	f << oss.str ();
	coreDatas->currentFreq = coresInfos->availableFreqs[freqId];
	f.flush();

	f.close ();
	f.clear ();
}
