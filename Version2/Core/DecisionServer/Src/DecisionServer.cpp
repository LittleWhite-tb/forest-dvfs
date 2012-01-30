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
#include <cstdlib>
#include <signal.h>

#include "DecisionServer.h"
#include "Message.h"
#include "ReportMsg.h"
#include "YellowPages.h"
#include "Profiler.h"
#include "SetWinMsg.h"

static void
cleanup_fn ();
static void
sighandler (int ns);

static DecisionServer * dc = NULL; // ok, I know...

DecisionServer::DecisionServer (void)
{
   this->coresInfos = new CoresInfos ();
   this->freqchanger = new FreqChanger (coresInfos);
   this->naiveDecisions = new NaiveDecisions (coresInfos);
   int numCores = coresInfos->numCores;
   int numFreqs = coresInfos->numFreqs;

   this->sleep_windows = new unsigned int[numCores];
   freqTracker = new int*[numCores];

   for (int i = 0; i < numCores; ++i)
   {
      this->sleep_windows[i] = INIT_SLEEP_WIN;
      freqTracker[i] = new int[numFreqs];

      for (int j = 0; j < numFreqs; ++j)
      {

         freqTracker[i][j] = 0;
      }
   }

   this->comm = new Communicator ();
}

DecisionServer::~DecisionServer (void)
{
   delete this->comm;
   delete this->naiveDecisions;
   delete this->freqchanger;
   delete this->coresInfos;
}

void DecisionServer::server_loop ()
{
   while (true)
   {
      Message * msg;
      Message::Type msgtp;

      msg = this->comm->recv ();
      assert (msg != NULL);

      msgtp = msg->get_type ();
      if (msgtp == Message::MSG_TP_REPORT)
      {
         int core = YellowPages::get_core_id (msg->get_sender ());
         int freq = naiveDecisions->giveReport (core,
                                                ( (ReportMsg *) msg)->get_report ());

         if (freq != -1)
         {
            ++freqTracker[core][freq];
            freqchanger->ChangeFreq (core, freq);

            // reset profiler's sleep windows
            this->sleep_windows[msg->get_sender() ] = INIT_SLEEP_WIN;
            Message * swmsg = new SetWinMsg (msg->get_dest(), msg->get_sender(), INIT_SLEEP_WIN);
            this->comm->send (*swmsg);
            delete swmsg;
         }
         else
         {
            // expand profiler window
            unsigned int win = this->sleep_windows[msg->get_sender() ];

            // avoid un-necessary set window messages
            if (win == LONGEST_SLEEP_WIN)
            {
               continue;
            }

            win *= 2;
            win = win > LONGEST_SLEEP_WIN ? LONGEST_SLEEP_WIN : win;
            this->sleep_windows[msg->get_sender() ] = win;

            Message * swmsg = new SetWinMsg (msg->get_dest(), msg->get_sender(), win);
            this->comm->send (*swmsg);
            delete swmsg;
         }
      }
      else
      {
         std::cerr << "Received a message with unexpected type: "
                   << msg->get_type () << std::endl;
      }
   }
}

int main (int argc, char ** argv)
{
   if (argc != 3)
   {
      std::cerr << "Usage: " << argv[0] << " id config" << std::endl;
      return 1;
   }

   signal (SIGTERM, sighandler);
   signal (SIGINT, sighandler);

   atexit (cleanup_fn);

   unsigned int id;
   std::istringstream iss (argv[1], std::istringstream::in);
   iss >> id;
   std::string confpath (argv[2]);
   YellowPages::init_from (id, confpath);

   dc = new DecisionServer ();
   dc->server_loop ();
}

static void cleanup_fn ()
{
   YellowPages::reset ();
   delete (dc);
}

static void sighandler (int ns)
{
   (void) ns;

   // will indirectly call cleanup
   exit (0);
}
