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
#include <sys/un.h>

#include <iostream>
#include <fstream>


unsigned int YellowPages::id;
unsigned int YellowPages::server_id;
std::map<unsigned int, std::string> YellowPages::yp;
std::map<std::string, unsigned int> YellowPages::ryp;
std::map<unsigned int, int> YellowPages::yp_core;


void YellowPages::init_from (unsigned int local_id, std::string & fpath)
{
   YellowPages::id = local_id;

   std::ifstream ifs (fpath.c_str(), std::ifstream::in);

   // read all the data provided
   while (true)
   {
      unsigned int id;
      char addr[256];
      unsigned int sid;
      int core_id;

      ifs >> id;
      if (ifs.bad() || ifs.eof())
      {
         break;
      }

      ifs.width (256);
      ifs >> addr;
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

      YellowPages::yp[id] = addr;
      YellowPages::ryp[addr] = id;
      YellowPages::yp_core[id] = core_id;

      if (ifs.bad() || ifs.eof())
      {
         break;
      }
   }
}

unsigned int YellowPages::get_id (struct sockaddr * saddr)
{
   assert (saddr->sa_family == AF_LOCAL || saddr->sa_family == AF_UNIX);

   struct sockaddr_un * addr = (struct sockaddr_un *) saddr;

   std::map<std::string, unsigned int>::iterator it;
   it = YellowPages::ryp.find (addr->sun_path);

   if (it != YellowPages::ryp.end())
   {
      return it->second;
   }
   else
   {
      return UINT_MAX;
   }
}

struct sockaddr * YellowPages::get_addr (unsigned int id)
{
   static struct sockaddr_un addr;

   if (id == UINT_MAX)
   {
      return NULL;
   }

   std::map<unsigned int, std::string>::iterator it;
   it = YellowPages::yp.find (id);

   if (it != YellowPages::yp.end())
   {
      addr.sun_family = AF_LOCAL;
      it->second.copy (addr.sun_path, it->second.size(), 0);
      addr.sun_path[it->second.size() ] = '\0';

      return (sockaddr *) &addr;
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

