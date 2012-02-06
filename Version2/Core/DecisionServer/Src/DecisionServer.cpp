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
#include "Log.h"
#include "Config.h"

static void cleanup_fn ();
static void sighandler (int ns);

static DecisionServer * dc = NULL; // ok, I know...

DecisionServer::DecisionServer (void)
{
   this->coresInfos = new CoresInfos ();
   this->freqchanger = new FreqChanger (coresInfos);
   this->naiveDecisions = new NaiveDecisions (coresInfos);
   this->nbProfs = 0;

   int numCores = coresInfos->numCores;
   int numFreqs = coresInfos->numFreqs;

   freqTracker = new int*[numCores];

   for (int i = 0; i < numCores; ++i)
   {
      freqTracker [i] = new int [numFreqs];

      for (int j = 0; j < numFreqs; ++j)
      {
         freqTracker [i][j] = 0;
      }
   }

   this->comm = new Communicator ();

   // register callback
   this->comm->registerConnCallback (DecisionServer::connectionCallback, this);

   // FIXME: the communicator should only start listening here !! (and not before)
}


DecisionServer::~DecisionServer (void)
{
   //Outputting core/frequency changes
   std::ofstream file ("core_frequency_count.csv", std::ios::out | std::ios::trunc);

   if (!file.good ())
   {
      std::cerr << "Failed to open server output" << std::endl;
      return;
   }

   file << "\"Core\";\"Frequency\";\"Freqency changes\"" << std::endl;

   for (unsigned int i = 0; i < this->coresInfos->numCores; i++)
   {
      for (unsigned int j = 0; j < this->coresInfos->numFreqs; j++)
      {
         file << i << ";" << j << ";" << this->freqTracker [i][j] << std::endl;
      }
   }

   file.flush ();
   file.close ();

   // cleanup memory
   delete this->comm;
   delete this->naiveDecisions;
   delete this->freqchanger;
   delete this->coresInfos;
}

// main server loop in which the messages are handled
void DecisionServer::server_loop ()
{
   // while we do not die
   while (true)
   {
      Message * msg;
      Message::Type msgtp;

      // actual reception
      msg = this->comm->recv ();
      assert (msg != NULL);

      msgtp = msg->get_type ();

      // report received
      if (msgtp == Message::MSG_TP_REPORT)
      {
         // send the report to the decision maker
         int core = YellowPages::getCoreId (msg->get_sender ());
         int freq = naiveDecisions->giveReport (core,
                                                ( (ReportMsg *) msg)->get_report ());

         // requested to switch the frequency
         if (freq != -1)
         {
            // some bookkeeping...
            ++freqTracker [core][freq];

            // actually change the frequency here
            freqchanger->ChangeFreq (core, freq);

            // reset profiler's sleep windows
            this->sleep_windows [msg->get_sender () ] = INIT_SLEEP_WIN;

            // send this to the profiler
            Message * swmsg = new SetWinMsg (msg->get_dest (), msg->get_sender (), INIT_SLEEP_WIN);
            this->comm->send (*swmsg);
            delete swmsg;
         }
         else  // keep the same frequency
         {
            unsigned int win = this->sleep_windows [msg->get_sender () ];

            // avoid un-necessary set window messages
            if (win >= LONGEST_SLEEP_WIN)
            {
               continue;
            }

            // expand profiler window
            win *= 2;
            win = win > LONGEST_SLEEP_WIN ? LONGEST_SLEEP_WIN : win;
            this->sleep_windows [msg->get_sender () ] = win;

            // send the set window message to the profiler
            Message * swmsg = new SetWinMsg (msg->get_dest (), msg->get_sender (), win);
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


//
// BELLOW IS THE STATIC CODE THAT LAUNCHES THE SERVER
//

int main (int argc, char ** argv)
{
   if (argc != 3)
   {
      std::cerr << "Usage: " << argv [0] << " id config" << std::endl;
      return 1;
   }

   atexit (cleanup_fn);

   signal (SIGTERM, sighandler);
   signal (SIGINT, sighandler);

   unsigned int id;
   std::istringstream iss (argv [1], std::istringstream::in);
   iss >> id;
   Config cfg = Config (argv [2]);
   YellowPages::initFrom (id, cfg);

   dc = new DecisionServer ();
   dc->server_loop ();
}

// cleans the static decision server at its exit
static void cleanup_fn ()
{
   YellowPages::reset ();
   delete (dc);
}

// signal handler for SIGINT, SIGTERM
static void sighandler (int ns)
{
   (void) ns;

   // will indirectly call cleanup
   exit (0);
}

// connection and deconnection callback
void DecisionServer::connectionCallback (bool conn, unsigned int id, void * arg)
{
   (void) (conn);
   (void) (id);

   DecisionServer * obj = (DecisionServer *) arg;

   if (conn)
   {
      obj->nbProfs++;
      obj->sleep_windows [id] = INIT_SLEEP_WIN;
   }
   else
   {
      obj->nbProfs--;
      obj->sleep_windows.erase (id);
   }
}

