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

Communicator::Communicator ()
{
   this->sockets_out = std::map<unsigned int, int>();
   this->sockets_in = std::map<unsigned int, int>();
   pthread_mutex_init (&this->mutex_sockukn, NULL);

   // incoming connection socket
   this->fd_server = socket (AF_INET, SOCK_STREAM, 0);
   if (this->fd_server == -1)
   {
      std::perror ("Can't create server socket");
      exit (2);
   }

   const struct sockaddr * addr = YellowPages::get_addr (YellowPages::get_id());
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

   if (pipe (this->new_conn) == -1)
   {
      std::perror ("Failed to create notification pipe");
      exit (2);
   }

   // start accepting connections
   if (pthread_create (&this->server_th, NULL, &Communicator::server_loop, this) != 0)
   {
      std::cerr << "Can't create accepting thread" << std::endl;
      exit (2);
   }
}

Communicator::~Communicator()
{
   pthread_cancel (this->server_th);
   pthread_join (this->server_th, NULL);

   close (this->fd_server);

   close (this->new_conn[0]);
   close (this->new_conn[1]);

   std::map<unsigned int, int>::iterator it;
   for (it = this->sockets_out.begin();
         it != this->sockets_out.end();
         it++)
   {
      close (it->second);
   }

   // safe to do it without any lock as no more connections are accepted
   for (it = this->sockets_in.begin();
         it != this->sockets_in.end();
         it++)
   {
      close (it->second);
   }
}

void * Communicator::server_loop (void * arg)
{
   Communicator * obj = (Communicator *) arg;

   pthread_setcancelstate (PTHREAD_CANCEL_ENABLE, NULL);
   pthread_setcanceltype (PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

   while (true)
   {
      struct sockaddr_storage addr;
      socklen_t addr_len;
      int fd;

      addr_len = sizeof (addr);
      if ( (fd = accept (obj->fd_server, (struct sockaddr *) &addr, &addr_len)) < 0)
      {
         std::perror ("Client connection error");
         continue;
      }

      pthread_mutex_lock (&obj->mutex_sockukn);
      obj->sockets_ukn.insert (fd);
      pthread_mutex_unlock (&obj->mutex_sockukn);

      std::cout << "Unkown node " << fd << " connected" << std::endl;

      // restart reading open connections
      write (obj->new_conn[1], &fd, sizeof (int));
   }

   return NULL;
}

int Communicator::get_socket (unsigned int dest_id, bool create)
{
   std::map<unsigned int, int>::iterator it;

   it = this->sockets_out.find (dest_id);
   if (it != this->sockets_out.end())
   {
      return it->second;
   }

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

   this->sockets_out[dest_id] = fd;

   // tell the server who we are
   unsigned int my_id = YellowPages::get_id();
   IdMsg idmsg (my_id, dest_id, my_id);
   this->send (idmsg);

   return fd;
}

void Communicator::send (const Message & msg)
{
   int fd = this->get_socket (msg.get_dest(), true);

   if (fd != -1)
   {
      if (!msg.write_into (fd))
      {
         std::cerr << "Failure to write message in the channel" << std::endl;
         close (fd);
         this->sockets_out.erase (msg.get_dest());
      }
   }
   else
   {
      std::cerr << "Failure to send message to " << msg.get_dest()  << std::endl;
   }
}

Message * Communicator::recv (unsigned int * timeout, unsigned int sender_id)
{
   struct timeval tval;
   struct timeval * tvalp;
   fd_set fds;
   int maxfd;

   // restart after any new connection and until receiving a message or timeout
   while (true)
   {
      maxfd = -1;

      if (timeout != NULL && *timeout != 0)
      {
         tvalp = &tval;
         tval.tv_sec = *timeout / 1000000;
         tval.tv_usec = *timeout % 1000000;
      }

      // get file descriptors to watch
      FD_ZERO (&fds);
      if (sender_id != UINT_MAX)
      {
         std::map<unsigned int, int>::iterator it;
         it = this->sockets_in.find (sender_id);
         assert (it != this->sockets_in.end());

         FD_SET (it->second, &fds);
         maxfd = it->second;
      }
      else
      {
         std::map<unsigned int, int>::iterator it;
         for (it = this->sockets_in.begin();
               it != this->sockets_in.end();
               it++)
         {
            FD_SET (it->second, &fds);
            maxfd = (it->second > maxfd ? it->second : maxfd);
         }
      }

      // also watch for new connection
      FD_SET (this->new_conn[0], &fds);
      maxfd = (this->new_conn[0] > maxfd ? this->new_conn[0] : maxfd);

      // and finally consider unidentified sockets
      pthread_mutex_lock (&this->mutex_sockukn);
      std::set<int>::iterator it;

      for (it = this->sockets_ukn.begin();
            it != this->sockets_ukn.end();
            it++)
      {
         FD_SET (*it, &fds);
         maxfd = (*it > maxfd ? *it : maxfd);
      }

      pthread_mutex_unlock (&this->mutex_sockukn);

      int sres = select (maxfd + 1, &fds, NULL, NULL, &tval);
      if (sres > 0)
      {
         // update the timeval value
         if (timeout != NULL && *timeout != 0)
         {
            *timeout = tval.tv_sec + tval.tv_usec * 1000000;
         }

         // new connection ? restart !
         if (FD_ISSET (this->new_conn[0], &fds))
         {
            int dummy;
            read (this->new_conn[0], &dummy, sizeof (int));
            continue;
         }

         if (sender_id != UINT_MAX)
         {
            // single sender
            std::map<unsigned int, int>::iterator it;
            it = this->sockets_in.find (sender_id);
            assert (it != this->sockets_in.end());
            Message * msg = MsgReader::read_msg (it->second, sender_id, YellowPages::get_id());

            if (msg == NULL)
            {
               std::cout << "Connection with node " << it->first << " lost" << std::endl;
               close (it->second);
               this->sockets_in.erase (it);
            }

            return msg;
         }
         else
         {
            // multiple connections
            Message * msg = NULL;

            std::set<int> to_rm = std::set<int>();

            std::map<unsigned int, int>::iterator it;
            for (it = this->sockets_in.begin(); it != this->sockets_in.end(); it++)
            {
               if (!FD_ISSET (it->second, &fds))
               {
                  continue;
               }

               msg = MsgReader::read_msg (it->second, it->first, YellowPages::get_id());

               if (msg == NULL)
               {
                  std::cout << "Connection with node " << it->first << " lost" << std::endl;
                  close (it->second);
                  to_rm.insert (it->first);
                  continue;
               }

               break;
            }

            // cleanup closed sockets
            std::set<int>::iterator it_rm;
            for (it_rm = to_rm.begin(); it_rm != to_rm.end(); it_rm++)
            {
               this->sockets_in.erase (*it_rm);
            }

            if (msg != NULL)
            {
               return msg;
            }

            // watch unidentified connections and check if they can be identified
            to_rm.clear();

            pthread_mutex_lock (&this->mutex_sockukn);

            std::set<int>::iterator its;
            for (its = this->sockets_ukn.begin();
                  its != this->sockets_ukn.end();
                  its++)
            {
               if (!FD_ISSET (*its, &fds))
               {
                  continue;
               }

               msg = MsgReader::read_msg (*its, 0, 0);

               if (msg == NULL)
               {
                  std::cout << "Connection with unknown node " << *its << " lost" << std::endl;
                  close (*its);
                  to_rm.insert (*its);
                  continue;
               }

               if (msg->get_type() != Message::MSG_TP_ID)
               {
                  std::cerr << "Skipping unexpected message from unidentified node " << *its << std::endl;
                  continue;
               }

               // connection is now identified, we can start regular communications

               std::cout << "Unkown node " << *its << " identified as Node " << ( (IdMsg *) msg)->get_id() << std::endl;

               this->sockets_in[ ( (IdMsg *) msg)->get_id() ] = *its;
               to_rm.insert (*its);
            }

            // cleanup closed/identified sockets
            for (it_rm = to_rm.begin(); it_rm != to_rm.end(); it_rm++)
            {
               this->sockets_ukn.erase (*it_rm);
            }

            pthread_mutex_unlock (&this->mutex_sockukn);
         }
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

