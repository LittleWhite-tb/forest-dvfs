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

#include <limits.h>
#include <pthread.h>
#include <map>
#include <set>

#include "YellowPages.h"
#include "Message.h"

/**
 * @brief Callback functions called when a client connects or disconnects.
 * @param conn true when the client connected, false otherwise.
 * @param id The id of the node involved
 * @param arg User-defined value set when registering the callback.
 */
typedef void (*CommConnectFn) (bool conn, unsigned int id, void * arg);


/**
 * @brief Handles all incoming and outgoing connections.
 */
class Communicator
{
   public:
      /** @brief Creates and initialize the underlying network layer. */
      Communicator ();

      /** @brief Destructor. */
      ~Communicator();

      /**
       * @brief Sends a message to anoter peer with the given id. Connects to
       * this peer if no connection already exists.
       * @param dest_id The ID of the destination node.
       * @param msg The message to send.
       */
      void send (const Message & msg);

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
       * @brief Registers a callback function which will be called when a
       * new client will connect or disconnect.
       * @param fn A pointer to the callback function.
       * @param arg An extra argument passed to the function when called.
       */
      inline void registerConnCallback (CommConnectFn fn, void * arg)
      {
         (*this->connCallbacks) [fn] = arg;
      }

      /**
       * @brief Unregisters a connection/disconnectino callback function.
       * @param fn The function to unregister.
       */
      inline void unregisterConnCallback (CommConnectFn & fn)
      {
         this->connCallbacks->erase (fn);
      }


   private:

      /** @brief fd where to accept incomming connections. */
      int fd_server;

      /** @brief Server thread id. */
      pthread_t server_th;
      /** @brief Openned socket connections */
      std::map<unsigned int, int> sockets_in;

      /** @brief Outgoing connections. */
      std::map<unsigned int, int> sockets_out;

      /** @brief mutex to access sockets_ukn */
      pthread_mutex_t mutex_sockukn;
      /**
       * @brief list of unidentified sockets. Those are people that connected
       * to us but we have no id what is their id.
       */
      std::set<int> sockets_ukn;

      /**
       * @brief Registered (dis)connection callbacks.
       */
      std::map<CommConnectFn, void *> *connCallbacks;


      /**
       * @brief fds to notify new connections. new_conn[0] is the read end,
       * new_conn[1] is the write end of the pipe.
       */
      int new_conn[2];

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

