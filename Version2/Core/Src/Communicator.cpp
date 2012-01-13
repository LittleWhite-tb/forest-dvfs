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

#include <assert.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

Communicator::Communicator (unsigned int local_id)
{
   this->local_id = local_id;
   this->sockets_out = std::map<unsigned int, int>();
   this->sockets_in = std::map<unsigned int, int>();
   pthread_mutex_init (&this->mutex_sockin, NULL);

   // incoming connection socket
   this->fd_server = socket (PF_UNIX, SOCK_STREAM, 0);
   if (this->fd_server == -1)
   {
      perror ("Can't create server socket ");
      exit (2);
   }

   struct sockaddr * addr = YellowPages::get_addr (local_id);
   assert (addr != NULL);
   if (bind (this->fd_server, (struct sockaddr *) &addr, YP_ADDRLEN (addr)) == -1)
   {
      perror ("Can't bind server socket ");
      exit (2);
   }

   if (listen (this->fd_server, 32) == -1)
   {
      perror ("Can't set server socket in listen mode ");
      exit (2);
   }

   // start accepting connections
   if (pthread_create (&this->server_th, NULL, &Communicator::server_loop, this) != 0)
   {
      printf ("Can't create accepting thread\n");
      exit (2);
   }
}

Communicator::~Communicator()
{
   pthread_cancel (this->server_th);
   pthread_join (this->server_th, NULL);

   close (this->fd_server);

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
      struct sockaddr addr;
      int fd;
      unsigned int src_id;

      if ( (fd = accept (obj->fd_server, &addr, NULL)) < 0)
      {
         perror ("Client connection error ");
         continue;
      }

      pthread_mutex_lock (&obj->mutex_sockin);
      src_id = YellowPages::get_id (&addr);
      obj->sockets_in[src_id] = fd;
      pthread_mutex_unlock (&obj->mutex_sockin);
   }

   return NULL;
}

int Communicator::get_socket (unsigned int dest_id, bool create)
{
   std::map<unsigned int, int>::iterator it;

   it = this->sockets_out.find (dest_id);
   if (it != this->sockets_out.end())
   {
      return (*it).second;
   }

   if (!create)
   {
      return -1;
   }

   // connect to destination
   int fd = socket (AF_UNIX, SOCK_STREAM, 0);
   if (fd == -1)
   {
      return -1;
   }

   struct sockaddr * addr = YellowPages::get_addr (dest_id);
   assert (addr != NULL);
   if (connect (fd, addr, YP_ADDRLEN (addr)) < 0)
   {
      printf ("Failed to connect to node %u\n", dest_id);
      close (fd);
      return -1;
   }

   this->sockets_out[dest_id] = fd;
   return fd;
}

void Communicator::send_to (unsigned int dest_id, const Message & msg)
{
   int fd = this->get_socket (dest_id, true);

   if (fd != -1)
   {
      if (!msg.write_into (fd))
      {
         close (fd);
         this->sockets_out.erase (dest_id);
      }
   }
   else
   {
      printf ("Failure to send message to %u\n", dest_id);
   }
}

Message * Communicator::recv (unsigned int * timeout, unsigned int sender_id)
{
   struct timeval tval;
   struct timeval * tvalp = NULL;
   fd_set fds;
   int maxfd = -1;

   if (*timeout != 0)
   {
      tvalp = &tval;
      tval.tv_sec = *timeout / 1000000;
      tval.tv_usec = *timeout % 1000000;
   }

   // get file descriptors to watch
   FD_ZERO (&fds);
   if (sender_id != UINT_MAX)
   {
      pthread_mutex_lock (&this->mutex_sockin);

      std::map<unsigned int, int>::iterator it;
      it = this->sockets_in.find (sender_id);
      assert (it != this->sockets_in.end());

      FD_SET (it->second, &fds);
      maxfd = it->second;

      pthread_mutex_unlock (&this->mutex_sockin);
   }
   else
   {
      pthread_mutex_lock (&this->mutex_sockin);

      std::map<unsigned int, int>::iterator it;
      for (it = this->sockets_in.begin();
            it != this->sockets_in.end();
            it++)
      {
         FD_SET (it->second, &fds);
         maxfd = (it->second > maxfd ? it->second : maxfd);
      }

      pthread_mutex_unlock (&this->mutex_sockin);
   }

   int sres = select (maxfd + 1, &fds, NULL, NULL, &tval);
   if (sres > 0)
   {
      // update the timeval value
      *timeout = tval.tv_sec + tval.tv_usec * 1000000;

      if (sender_id != UINT_MAX)
      {
         // single sender
         pthread_mutex_lock (&this->mutex_sockin);

         std::map<unsigned int, int>::iterator it;
         it = this->sockets_in.find (sender_id);
         assert (it != this->sockets_in.end());
         Message * msg = MsgReader::read_msg (it->second);

         if (msg == NULL)
         {
            close (it->second);
            this->sockets_in.erase (it);
         }

         pthread_mutex_unlock (&this->mutex_sockin);

         return msg;
      }
      else
      {
         // multiple connections
         Message * msg = NULL;

         pthread_mutex_lock (&this->mutex_sockin);

         std::set<int> to_rm = std::set<int>();

         std::map<unsigned int, int>::iterator it;
         for (it = this->sockets_in.begin(); it != this->sockets_in.end(); it++)
         {
            if (!FD_ISSET (it->second, &fds))
            {
               continue;
            }

            msg = MsgReader::read_msg (it->second);

            if (msg == NULL)
            {
               close (it->second);
               to_rm.insert (it->first);
            }
         }

         // cleanup closed sockets
         std::set<int>::iterator it_rm;
         for (it_rm = to_rm.begin(); it_rm != to_rm.end(); it_rm++)
         {
            close (this->sockets_in[*it_rm]);
            this->sockets_in.erase (*it_rm);
         }

         pthread_mutex_unlock (&this->mutex_sockin);

         return msg;
      }
   }

   return NULL;
}

void Communicator::getInformation (void)
{

}

