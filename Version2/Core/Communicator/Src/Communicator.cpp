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
  @file Communicator.cpp
  @brief The Communicator class is in this file
 */

#include "Communicator.h"
#include "MsgReader.h"
#include "IdMsg.h"

#include <assert.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <csignal>
#include <cstdlib>
#include <cerrno>
#include <cstdio>
#include <cstring>

#include <iostream>

/*
 * Creates a communicator which basically waits for incomming connections
 * and allows one to send messages.
 */
Communicator::Communicator ()
{
   this->sockets_out = std::map<unsigned int, int>();
   this->sockets_in = std::map<unsigned int, int>();
   pthread_mutex_init (&this->mutex_sockukn, NULL);
   this->connCallbacks = new std::map<CommConnectFn, void *>();

   // incoming connection socket
   this->fd_server = socket (AF_INET, SOCK_STREAM, 0);
   if (this->fd_server == -1)
   {
      std::perror ("Can't create server socket");
      exit (2);
   }

   // my address
   const struct sockaddr * addr = YellowPages::get_addr (YellowPages::get_id ());
   assert (addr != NULL);
   if (bind (this->fd_server, addr, YP_ADDRLEN (addr)) == -1)
   {
      std::perror ("Can't bind server socket");
      exit (2);
   }

   if (listen (this->fd_server, 32) == -1)
   {
      std::perror ("Can't set server socket in listen mode ");
      exit (2);
   }

   // the pipe is used to notify an accepted incoming connection
   if (pipe (this->new_conn) == -1)
   {
      std::perror ("Failed to create notification pipe");
      exit (2);
   }

   // start accepting connections in a thread
   if (pthread_create (&this->server_th, NULL, &Communicator::server_loop, this) != 0)
   {
      std::cerr << "Can't create accepting thread" << std::endl;
      exit (2);
   }
}

/*
 * Close all openned connections and free the allocated memory.
 */
Communicator::~Communicator ()
{
   // stop accepting connections
   pthread_cancel (this->server_th);
   pthread_join (this->server_th, NULL);

   // close listening socket
   close (this->fd_server);

   // cannot be notified of a new incoming connection anymore
   close (this->new_conn [0]);
   close (this->new_conn [1]);

   // close all outgoing connections
   std::map<unsigned int, int>::iterator it;
   for (it = this->sockets_out.begin ();
         it != this->sockets_out.end ();
         it++)
   {
      close (it->second);
   }

   // close all incoming connection
   // safe to do it without any lock as no more connections are accepted
   for (it = this->sockets_in.begin ();
         it != this->sockets_in.end ();
         it++)
   {
      close (it->second);
   }

   delete this->connCallbacks;
}

/*
 * Threaded function where the server indefinitely waits for new incoming
 * connections and accept them ASAP.
 */
void * Communicator::server_loop (void * arg)
{
   Communicator * obj = (Communicator *) arg;

   // I can be cancelled at checkpoint only
   pthread_setcancelstate (PTHREAD_CANCEL_ENABLE, NULL);
   pthread_setcanceltype (PTHREAD_CANCEL_DEFERRED, NULL);

   // while no-one wants me to die
   // terminates me with a pthread_cancel is ok
   while (true)
   {
      struct sockaddr_storage addr;
      socklen_t addr_len;
      int fd;

      // accept incoming connection
      addr_len = sizeof (addr);
      if ((fd = accept (obj->fd_server, (struct sockaddr *) &addr, &addr_len)) < 0)
      {
         std::perror ("Client connection error");
         continue;
      }

      // we have a new unidentified node
      pthread_mutex_lock (&obj->mutex_sockukn);
      obj->sockets_ukn.insert (fd);
      pthread_mutex_unlock (&obj->mutex_sockukn);

      std::cout << "Unkown node " << fd << " connected" << std::endl;

      // restart reading open connections
      write (obj->new_conn [1], &fd, sizeof (int));

      // ok I can be killed here
      pthread_testcancel ();
   }

   return NULL;
}

/*
 * Returns a socket to communicate with the given node. The socket
 * is ensured to be correctly openned.
 */
int Communicator::get_socket (unsigned int dest_id, bool create)
{
   std::map<unsigned int, int>::iterator it;

   // do we already have openned a socket?
   it = this->sockets_out.find (dest_id);
   if (it != this->sockets_out.end ())
   {
      return it->second;
   }

   // nothing found and we are not asked for open connections
   if (!create)
   {
      return -1;
   }

   // connect to destination
   int fd = socket (AF_INET, SOCK_STREAM, 0);
   if (fd == -1)
   {
      return -1;
   }

   const struct sockaddr * addr = YellowPages::get_addr (dest_id);
   assert (addr != NULL);
   if (connect (fd, addr, YP_ADDRLEN (addr)) < 0)
   {
      std::cerr <<  "Failed to connect to node " << dest_id << ": " << strerror (errno) << std::endl;
      close (fd);
      return -1;
   }

   // remember this socket
   this->sockets_out [dest_id] = fd;

   // tell the server who we are
   unsigned int my_id = YellowPages::get_id ();
   IdMsg idmsg (my_id, dest_id, my_id);
   this->send (idmsg);

   return fd;
}

/*
 * Send a message (destination is encoded in the message)
 */
void Communicator::send (const Message & msg)
{
   int fd = this->get_socket (msg.get_dest (), true);

   // no socket can be found for the destination
   if (fd != -1)
   {
      // try to send the message on the channel
      if (!msg.write_into (fd))
      {
         std::cerr << "Failure to write message in the channel" << std::endl;
         close (fd);
         this->sockets_out.erase (msg.get_dest ());
      }
   }
   else
   {
      std::cerr << "Failure to send message to " << msg.get_dest ()  << std::endl;
   }
}

/*
 * blocking reception function.
 */
Message * Communicator::recv (unsigned int * timeout)
{
   struct timeval tval;
   struct timeval * tvalp = NULL;
   fd_set fds;
   int maxfd;

   // restart after any new connection and until receiving a message or timeout
   while (true)
   {
      std::map<unsigned int, int>::iterator it_sockin;
      std::set<int>::iterator it_sockukn;
      Message * msg = NULL;   // received message
      int sres;               // result of select
      std::set<unsigned int> to_rm = std::set<unsigned int>(); // set of fd to remove
      std::set<unsigned int>::iterator it_rm;   // iterates over to_rm

      // initialize the maximal fd value
      maxfd = -1;

      // timeout given?
      if (timeout != NULL && *timeout != 0)
      {
         tvalp = &tval;
         tval.tv_sec = *timeout / 1000000;
         tval.tv_usec = *timeout % 1000000;
      }

      // wait for data on oppened and identified connections
      FD_ZERO (&fds);
      for (it_sockin = this->sockets_in.begin ();
            it_sockin != this->sockets_in.end ();
            it_sockin++)
      {
         FD_SET (it_sockin->second, &fds);
         maxfd = (it_sockin->second > maxfd ? it_sockin->second : maxfd);
      }

      // also watch for new connection
      FD_SET (this->new_conn [0], &fds);
      maxfd = (this->new_conn [0] > maxfd ? this->new_conn [0] : maxfd);

      // and finally consider unidentified sockets
      pthread_mutex_lock (&this->mutex_sockukn);

      for (it_sockukn = this->sockets_ukn.begin ();
            it_sockukn != this->sockets_ukn.end ();
            it_sockukn++)
      {
         FD_SET (*it_sockukn, &fds);
         maxfd = (*it_sockukn > maxfd ? *it_sockukn : maxfd);
      }

      pthread_mutex_unlock (&this->mutex_sockukn);

      // here is the actual wait
      sres = select (maxfd + 1, &fds, NULL, NULL, tvalp);
      if (sres > 0)  // some fd has received data
      {
         // update the timeout value
         if (timeout != NULL && *timeout != 0)
         {
            *timeout = tval.tv_sec + tval.tv_usec * 1000000;
         }

         // new connection? restart and start listening to it!
         if (FD_ISSET (this->new_conn [0], &fds))
         {
            int dummy;

            // consume the fd
            read (this->new_conn [0], &dummy, sizeof (int));

            continue;
         }

         // ok check for actual data
         for (it_sockin = this->sockets_in.begin ();
               it_sockin != this->sockets_in.end ();
               it_sockin++)
         {
            if (!FD_ISSET (it_sockin->second, &fds))
            {
               continue;
            }

            // read a message from the fd
            msg = MsgReader::read_msg (it_sockin->second, it_sockin->first, YellowPages::get_id ());

            // error while reading
            if (msg == NULL)
            {
               std::map<CommConnectFn, void *>::iterator it_fn;

               std::cout << "Connection with node " << it_sockin->first << " lost" << std::endl;
               close (it_sockin->second);
               to_rm.insert (it_sockin->first);

               // notify callbacks about the deconnection
               for (it_fn = this->connCallbacks->begin ();
                     it_fn != this->connCallbacks->end ();
                     it_fn++)
               {
                  it_fn->first (false, it_sockin->first, it_fn->second);
               }
            }
            else
            {
               // we have correctly read the message, it's time to return it
               // just cleanup closed connections before
               break;
            }
         }

         // cleanup closed sockets
         for (it_rm = to_rm.begin (); it_rm != to_rm.end (); it_rm++)
         {
            this->sockets_in.erase (*it_rm);
         }
         to_rm.clear ();

         // return the message read if any
         if (msg != NULL)
         {
            return msg;
         }

         // watch unidentified connections and check if they can be identified
         pthread_mutex_lock (&this->mutex_sockukn);

         for (it_sockukn = this->sockets_ukn.begin ();
               it_sockukn != this->sockets_ukn.end ();
               it_sockukn++)
         {
            std::map<CommConnectFn, void *>::iterator it_fn;

            if (!FD_ISSET (*it_sockukn, &fds))
            {
               continue;
            }

            // read the message here
            msg = MsgReader::read_msg (*it_sockukn, 0, 0);

            // failure while reading the message
            if (msg == NULL)
            {
               std::cout << "Connection with unknown node " << *it_sockukn << " lost" << std::endl;
               close (*it_sockukn);
               to_rm.insert (*it_sockukn);

               // do not notify callbacks as the unknown nodes are not considered as being connected
               // for callbacks functions

               continue;   // skip the rest of the message processing
            }

            // ignore unexpected messages
            if (msg->get_type () != Message::MSG_TP_ID)
            {
               std::cerr << "Skipping unexpected message from unidentified node " << *it_sockukn << std::endl;
               continue;
            }

            // connection is now identified, we can start regular communications
            std::cout << "Unkown node " << *it_sockukn << " identified as Node " << ((IdMsg *) msg)->get_id () << std::endl;

            this->sockets_in [( (IdMsg *) msg)->get_id ()] = *it_sockukn;
            to_rm.insert (*it_sockukn);

            // notify registered callbacks
            for (it_fn = this->connCallbacks->begin ();
                  it_fn != this->connCallbacks->end ();
                  it_fn++)
            {
               it_fn->first (true, ((IdMsg *) msg)->get_id (), it_fn->second);
            }
         }

         // cleanup closed/identified sockets
         for (it_rm = to_rm.begin (); it_rm != to_rm.end (); it_rm++)
         {
            this->sockets_ukn.erase (*it_rm);
         }

         pthread_mutex_unlock (&this->mutex_sockukn);
      }
      else if (sres < 0)
      {
         // error on select
         std::perror ("Error while waiting for data");
         return NULL;
      }
      else
      {
         // timeout
         return NULL;
      }
   }

   return NULL;
}

