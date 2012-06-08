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
 * @file Logger.cpp
 * The Logger class is int this file
 */


#include <sstream>
#include <iostream>
#include <fstream>
#include <time.h>
#include <cassert>
#include <vector>
#include "Logger.h"

std::vector<Logger *>Logger::logList;

void Logger::initLog (unsigned int nbTh)
{
	for(unsigned int i=0;i<nbTh;i++)
	{
		Logger::logList.push_back(new Logger(i));
	}
}
void Logger::destroyLog (void)
{
	for(int i=0;i<(int)Logger::logList.size();i++)
	{
		delete Logger::logList[i];
	}
	
}
Logger & Logger::getLog(unsigned int id)
{
	assert (id < (unsigned int)Logger::logList.size());

	return *Logger::logList [id];

}

Logger::Logger (unsigned int id)
{
	std::ostringstream oss;
	this->thId =id;
	oss.str (std::string (""));

	oss << "RESTlog" << this->thId;
	this->switchOFS.open (oss.str ().c_str (), std::ofstream::out | std::ofstream::trunc);
	if (!this->switchOFS)
	{
		std::cerr << "Failed to open the log file for DVFS unit " << this->thId << std::endl;
	}
}

Logger::~Logger (void)
{
	this->switchOFS.flush ();
	this->switchOFS.close ();
}

void Logger::logOut (std::stringstream &logString)
{
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	this->switchOFS << "[" << (ts.tv_nsec + ts.tv_sec * 1000000000)/ 1000000000.0 << "]*["<<this->thId<<"] >> " << logString.str() << std::endl;
}


void Logger::endBlock ()
{
	this->switchOFS << std::endl;
}
