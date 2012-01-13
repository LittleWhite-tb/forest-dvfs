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
  @file Communicator.h
  @brief The Communicator class header is in this file
 */

#ifndef H_COMMUNICATOR
#define H_COMMUNICATOR

#include "Rest.h"
#include "YellowPages.h"
#include "Message.h"

#include <limits.h>
#include <pthread.h>
#include <map>
#include <set>

/**
 * @brief Handles all incoming and outgoing connections.
 */
class Communicator : public Rest
{
   public:
      /** @brief Creates and initialize the underlying network layer. */
      Communicator (unsigned int local_id);

      /** @brief Destructor. */
      ~Communicator();

      /**
       * @brief Sends a message to anoter peer with the given id. Connects to
       * this peer if no connection already exists.
       * @param dest_id The ID of the destination node.
       * @param msg The message to send.
       */
      void send_to (unsigned int dest_id, const Message & msg);

      /**
       * @brief Blocking receive operation.
       * @return One message received. Can be NULL.
       * To be deallocated with delete.
       */
      inline Message * recv()
      {
         return recv (0, UINT_MAX);
      };

      /**
       * @brief Blocking receive operation with a timeout.
       * @param timeout The maximal waiting period in µs. 0 is unlimited.
       * Updated with the remaining time in case of message reception.
       * @return The received message. Can be NULL.
       * To be deallocated with delete.
       */
      inline Message * recv (unsigned int * timeout)
      {
         return recv (timeout, UINT_MAX);
      }

      /**
       * @brief Blocking receive operation bounded in time for a specific
       * sender.
       * @param timeout The maximal waiting period in µs. 0 is unlimited.
       * Updated with the remaining time in case of message reception.
       * @param sender_id The id of the sender node. UINT_MAX is anyone.
       * @return A received message or NULL. To be deallocated with delete.
       */
      Message * recv (unsigned int * timeout, unsigned int sender_id);

      /**
      		 * @brief Gives information concerning the current state.
      		 */
      void getInformation (void);

   private:

      /** @brief server local id. */
      unsigned int local_id;
      /** @brief fd where to accept incomming connections. */
      int fd_server;

      /** @brief Server thread id. */
      pthread_t server_th;
      /** @brief mutex to access sockets_in */
      pthread_mutex_t mutex_sockin;
      /** @brief Openned socket connections */
      std::map<unsigned int, int> sockets_in;

      /** @brief Outgoing connections. */
      std::map<unsigned int, int> sockets_out;

      /**
       * @brief Creates or returns the file descriptor to communicate with
       * the given node.
       * @param dest_id the id of the destination node.
       * @param create Determine wether the connection must be created if
       * none exists.
       * @return The socket fd where data can be written into. Can
       * return -1.
       */
      int get_socket (unsigned int dest_id, bool create);


      /**
       * @brief Reading loop. Accepts incomming connections.
       * @param obj This.
       * @return NULL.
       */
      static void * server_loop (void * obj);
};

#endif

