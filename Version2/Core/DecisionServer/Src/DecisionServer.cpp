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
#include <fstream>
#include <sstream>

#include "DecisionServer.h"
#include "Message.h"
#include "ReportMsg.h"
#include "YellowPages.h"

DecisionServer::DecisionServer (void)
{
   coresInfos = new CoresInfos ();
   freqchanger = new FreqChanger (coresInfos);
   naiveDecisions = new NaiveDecisions (coresInfos);

   this->comm = new Communicator();

   while (true)
   {
      Message * msg;
      Message::Type msgtp;

      msg = this->comm->recv();
      assert (msg != NULL);

      msgtp = msg->get_type();
      if (msgtp == Message::MSG_TP_REPORT)
      {
         int core = YellowPages::get_core_id (msg->get_sender());
         int freq = naiveDecisions->giveReport (core, ( (ReportMsg *) msg)->get_report());
         freqchanger->ChangeFreq (core, freq);
      }
      else
      {
         std::cerr << "Received a message with unexpected type: " << msg->get_type() << std::endl;
      }
   }
}

DecisionServer::~DecisionServer (void)
{
   delete (this->comm);
}

int main (int argc, char ** argv)
{
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " id config" << std::endl;
        return 1;
    }

    unsigned int id;
    std::istringstream iss(argv[1], std::istringstream::in);
    iss >> id;
    std::string confpath(argv[2]);
    YellowPages::init_from(id, confpath);

    DecisionServer *dc = new DecisionServer();
    delete dc;
}

