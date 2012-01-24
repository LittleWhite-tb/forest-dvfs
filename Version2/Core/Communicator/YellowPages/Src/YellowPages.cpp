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
  @file YellowPages.cpp
  @brief The YellowPages class is in this file
 */

#include "YellowPages.h"

#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <unistd.h>

#include <iostream>
#include <fstream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>


unsigned int YellowPages::id;
unsigned int YellowPages::server_id;
std::map<unsigned int, const struct sockaddr *> YellowPages::yp;
std::map<unsigned int, int> YellowPages::yp_core;


void YellowPages::init_from (unsigned int local_id, std::string & fpath)
{
   YellowPages::id = local_id;

   std::ifstream ifs (fpath.c_str(), std::ifstream::in);

   struct addrinfo addrhint;
   addrhint.ai_flags = 0;
   addrhint.ai_family = AF_INET;
   addrhint.ai_socktype = SOCK_STREAM;
   addrhint.ai_protocol = 0;
   addrhint.ai_addrlen = 0;
   addrhint.ai_addr = NULL;
   addrhint.ai_canonname = NULL;
   addrhint.ai_next = NULL;

   // read all the data provided
   while (true)
   {
      unsigned int id;
      char host[512];
      char port[64];
      unsigned int sid;
      int core_id;
      struct sockaddr_in * saddr;
      struct addrinfo * res_addr;

      ifs >> id;
      if (ifs.bad() || ifs.eof())
      {
         break;
      }

      ifs.width (512);
      ifs >> host;
      if (ifs.bad() || ifs.eof())
      {
         break;
      }

      ifs.width (64);
      ifs >> port;
      if (ifs.bad() || ifs.eof())
      {
         break;
      }

      ifs >> sid;
      if (ifs.bad() || ifs.eof())
      {
         break;
      }

      ifs >> core_id;

      if (id == local_id)
      {
         YellowPages::server_id = sid;
      }

      getaddrinfo (host, port, &addrhint, &res_addr);
      saddr = new struct sockaddr_in();
      memcpy (saddr, res_addr->ai_addr, res_addr->ai_addrlen);
      freeaddrinfo (res_addr);

      YellowPages::yp[id] = (const struct sockaddr *) saddr;
      YellowPages::yp_core[id] = core_id;

      if (ifs.bad() || ifs.eof())
      {
         break;
      }
   }
}

void YellowPages::reset()
{
   std::map<unsigned int, const struct sockaddr *>::iterator it;

   for (it = YellowPages::yp.begin();
         it != YellowPages::yp.end();
         it++)
   {
      delete it->second;
   }

   YellowPages::yp.clear();
   YellowPages::yp_core.clear();
   YellowPages::id = UINT_MAX;
   YellowPages::server_id = UINT_MAX;
}

const struct sockaddr * YellowPages::get_addr (unsigned int id)
{
   if (id == UINT_MAX)
   {
      return NULL;
   }

   std::map<unsigned int, const struct sockaddr *>::iterator it;
   it = YellowPages::yp.find (id);

   if (it != YellowPages::yp.end())
   {
      return it->second;
   }
   else
   {
      return NULL;
   }
}

int YellowPages::get_core_id (unsigned int id)
{
   if (id == UINT_MAX)
   {
      return INT_MAX;
   }

   std::map<unsigned int, int>::iterator it;
   it = YellowPages::yp_core.find (id);

   if (it != YellowPages::yp_core.end())
   {
      return it->second;
   }
   else
   {
      return INT_MAX;
   }
}

