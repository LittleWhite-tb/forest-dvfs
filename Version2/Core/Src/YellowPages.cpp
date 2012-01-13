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

unsigned int YellowPages::get_id (struct sockaddr * saddr)
{
   assert (saddr->sa_family == AF_LOCAL || saddr->sa_family == AF_UNIX);

   struct sockaddr_un * addr = (struct sockaddr_un *) saddr;

   unsigned int id;
   if (sscanf (addr->sun_path, "/tmp/rest%u", &id) == 1)
   {
      return id;
   }

   return UINT_MAX;
}

struct sockaddr * YellowPages::get_addr (unsigned int id)
{
   static struct sockaddr_un addr;

   if (id == UINT_MAX)
   {
      return NULL;
   }

   sprintf (addr.sun_path, "/tmp/rest%u", id);

   addr.sun_family = AF_LOCAL;

   return (sockaddr *) &addr;
}


