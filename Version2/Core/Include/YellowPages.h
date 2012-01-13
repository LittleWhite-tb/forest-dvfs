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
  @file YellowPages.h
  @brief The YellowPages class header is in this file 
 */

#ifndef H_YELLOWPAGES
#define H_YELLOWPAGES

#include "Rest.h"

#include <sys/types.h>
#include <sys/socket.h>

#define YP_ADDRLEN(a) sizeof(a)

class YellowPages
{
    public:
        /**
         * @brief Returns the id of the node with the given address.
         * @param addr The network address of the node.
         * @return The ID of the node with the given address.
         */
        static unsigned int get_id(struct sockaddr *addr);

        /**
         * @brief Returns the address of the node with the given id.
         * @param id The node id.
         * @return The the address of the node with the given ID. Usable
         * only until next call to get_addr. Can be NULL.
         */
        static struct sockaddr *get_addr(unsigned int id);
};

#endif
