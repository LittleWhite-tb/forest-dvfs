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
 @file DecisionMaker.cpp
 @brief The  DecisionMaker class is in this file
 */

#include  <assert.h>

#include "DecisionMaker.h"


DecisionMaker::DecisionMaker (CoresInfos *coresInfos)
{
	this->coresInfo = coresInfo;
}

DecisionMaker::~DecisionMaker (void)
{
	
}

int DecisionMaker::giveReport(int core, long long HWCounters[3])
{
	assert(1);
	//Log(1,"This is a virtual method why did you call it? Instead use it in NaiveDecisions");
	return 0;
}

float DecisionMaker::computeBoundness(long long sqFullStall, long long unhaltedCore, long long l2Miss)
{
	return (4*16*sqFullStall*unhaltedCore)/(l2Miss*l2Miss);
}
