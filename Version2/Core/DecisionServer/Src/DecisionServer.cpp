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
 @file DecisionServer.cpp
 @brief The DecisionServer class is in this file
 */

#include <assert.h>
#include <iostream>

#include <sstream>

#include "DecisionServer.h"

DecisionServer::DecisionServer (void)
{
	coresInfos = new CoresInfos ();
	freqchanger = new FreqChanger (coresInfos);
	naiveDecisions = new NaiveDecisions (coresInfos);
}

DecisionServer::~DecisionServer (void)
{

}

int main(int argc, char **argv)
{

	std::istringstream iss; //Convert string to int
	iss.str(argv[1]);
	int curFreq = 0;
	iss >> curFreq;

	DecisionServer *decisionServer = new DecisionServer();
	std::cout << "Before changing freq " << decisionServer->freqchanger->ReadCurrentFreq(0) << std::endl;
	decisionServer->freqchanger->ChangeFreq(0, curFreq);
	std::cout << "After  changing freq " << decisionServer->freqchanger->ReadCurrentFreq(0) << std::endl;

    
}
