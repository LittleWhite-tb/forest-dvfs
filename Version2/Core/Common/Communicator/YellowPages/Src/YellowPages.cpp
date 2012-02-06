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

#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <iostream>
#include <sstream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

#include "YellowPages.h"


// definition of static fields
unsigned int YellowPages::id;
unsigned int YellowPages::serverId;
std::map<unsigned int, const struct sockaddr *> YellowPages::yp;
std::map<unsigned int, int> YellowPages::yp_core;


void YellowPages::initFrom (unsigned int local_id, Config & cfg)
{
   struct addrinfo addrhint;
   struct sockaddr_in * saddr;   // network address
   struct addrinfo * res_addr;   // name resolution request
   std::map<unsigned int, NetConfEntry>::const_iterator nmit;

   // hint for the name resolution
   // avoid junk results
   addrhint.ai_flags = 0;
   addrhint.ai_family = AF_INET;
   addrhint.ai_socktype = SOCK_STREAM;
   addrhint.ai_protocol = 0;
   addrhint.ai_addrlen = 0;
   addrhint.ai_addr = NULL;
   addrhint.ai_canonname = NULL;
   addrhint.ai_next = NULL;

   // remember our id
   YellowPages::id = local_id;

   // solve network names
   const std::map<unsigned int, NetConfEntry> &netMap = cfg.getNetEntries ();
   for (nmit = netMap.begin (); nmit != netMap.end (); nmit++)
   {
      std::ostringstream oss;

      // name resolution
      oss << nmit->second.port;
      getaddrinfo (nmit->second.hostname, oss.str ().c_str (), &addrhint, &res_addr);
      saddr = new struct sockaddr_in ();
      memcpy (saddr, res_addr->ai_addr, res_addr->ai_addrlen);
      freeaddrinfo (res_addr);

      // save info
      YellowPages::yp [nmit->first] = (const struct sockaddr *) saddr;
      YellowPages::yp_core [nmit->first] = nmit->second.procNum;

      if (local_id == nmit->first)
      {
         YellowPages::serverId = nmit->second.serverId;
      }
   }
}

void YellowPages::reset ()
{
   std::map<unsigned int, const struct sockaddr *>::iterator it;

   for (it = YellowPages::yp.begin ();
         it != YellowPages::yp.end ();
         it++)
   {
      delete it->second;
   }

   YellowPages::yp.clear ();
   YellowPages::yp_core.clear ();
   YellowPages::id = UINT_MAX;
   YellowPages::serverId = UINT_MAX;
}

const struct sockaddr * YellowPages::getAddr (unsigned int id)
{
   if (id == UINT_MAX)
   {
      return NULL;
   }

   std::map<unsigned int, const struct sockaddr *>::iterator it;
   it = YellowPages::yp.find (id);

   if (it != YellowPages::yp.end ())
   {
      return it->second;
   }
   else
   {
      return NULL;
   }
}

int YellowPages::getCoreId (unsigned int id)
{
   if (id == UINT_MAX)
   {
      return INT_MAX;
   }

   std::map<unsigned int, int>::iterator it;
   it = YellowPages::yp_core.find (id);

   if (it != YellowPages::yp_core.end ())
   {
      return it->second;
   }
   else
   {
      return INT_MAX;
   }
}

