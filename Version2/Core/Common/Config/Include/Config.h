/*   Copyright (C) 2011 Exascale Research Center   This program is free software; you can redistribute it and/or   modify it under the terms of the GNU General Public License   as published by the Free Software Foundation; either version 2   of the License, or (at your option) any later version.   This program is distributed in the hope that it will be useful,   but WITHOUT ANY WARRANTY; without even the implied warranty of   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the   GNU General Public License for more details.   You should have received a copy of the GNU General Public License   along with this program; if not, write to the Free Software   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA. */

/**  @file Config.h  @brief The Config class header is in this file */
#ifndef H_CONFIG
#define H_CONFIG

#define CONF_HNAME_SZ 512

#include <string>
#include <map>
/**
 * Structure to store a network entry in the configuration file.
 */
typedef struct
{
   char hostname [CONF_HNAME_SZ];
   unsigned short port;
   unsigned int serverId;
   unsigned int procNum;
} NetConfEntry;
class Config
{
   public:

      /**
       * @brief Sole constructor.
       * @param fpath The configuration file to read.
       */
      Config (const char * fpath);

      /**
       * @brief Sole destructor.
       */
      ~Config ();

      /**
       * @brief Returns the map where each node ID is mapped to a network entry.
       *
       * @return The map of network entries.
       */
      inline const std::map<unsigned int, NetConfEntry> &getNetEntries () const
      {
         return this->netEntries;
      }

   private:
      /**
       * @brief Network configuration entries.
       */
      std::map<unsigned int, NetConfEntry> netEntries;
};

#endif
