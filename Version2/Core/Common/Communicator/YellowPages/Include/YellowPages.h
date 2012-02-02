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

#include "Config.h"
#include "Rest.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <string>
#include <map>

#include <netinet/in.h>
#define YP_ADDRLEN (a)(sizeof (struct sockaddr_in))

class YellowPages
{
   public:
      /**
       * @brief Configures the yellow pages using the given config file.
       * Lines in the configuration file must follow the following format:
       * ID HOST PORT SERVER_ID PROC_CORE
       * where the space character is used as separator between the fields.
       *
       * @param local_id The local_id to use
       * @param fpath The path to the configuration file.
       */
      static void init_from (unsigned int local_id, Config & cfg);

      /**
       * @brief Cleanups the memory used by the yellow pages and reset
       * the configuration to an empty state.
       */
      static void reset ();

      /**
       * @brief Returns the address of the node with the given id.
       * @param id The node id.
       * @return The the address of the node with the given ID. Usable
       * only until next call to get_addr. Can be NULL.
       */
      static const struct sockaddr * get_addr (unsigned int id);

      /**
       * @brief Returns the local id of the calling process.
       * @return The id of the process.
       */
      static inline unsigned int get_id ()
      {
         return YellowPages::id;
      }

      /**
       * @brief Returns the id of the server associated with me.
       * @return The id of my decision server.
       */
      static inline unsigned int get_server_id ()
      {
         return YellowPages::serverId;
      }

      /**
       * @brief Returns the processor core, the given profiler is in charge of.
       * @return The id of the proc core watched by the profiler.
       */
      static int get_core_id (unsigned int id);

   private:
      /** @brief My id */
      static unsigned int id;

      /** @brief The id of my server. */
      static unsigned int serverId;

      /** @brief map id -> address */
      static std::map<unsigned int, const struct sockaddr *> yp;

      /** @brief map id -> core_id */
      static std::map<unsigned int, int> yp_core;

};

#endif
