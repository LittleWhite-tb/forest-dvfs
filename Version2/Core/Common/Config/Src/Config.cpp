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
  @file Config.cpp
  @brief The Config class is in this file
 */

#include <fstream>
#include <iostream>

#include "Config.h"

// locally used functions
static bool skipJunk (std::ifstream & ifs);

Config::Config (const char * fpath)
{
   std::ifstream ifs (fpath);

   if (!ifs.good ())
   {
      std::cerr << "Cannot open configuration file '" << fpath << "'" << std::endl;
      return;
   }

   // read all the network entries
   while (ifs.good ())
   {
      unsigned int id;
      NetConfEntry nce;

      skipJunk (ifs);

      // read the node id
      ifs >> id;
      skipJunk (ifs);

      if (!ifs.good ())
      {
         break;
      }

      // read the host
      ifs.width (CONF_HNAME_SZ);
      ifs >> nce.hostname;
      skipJunk (ifs);

      if (!ifs.good ())
      {
         break;
      }

      // read the port
      ifs >> nce.port;
      skipJunk (ifs);

      if (!ifs.good ())
      {
         break;
      }

      // read the server ID
      ifs >> nce.serverId;
      skipJunk (ifs);

      if (!ifs.good ())
      {
         break;
      }

      // read the processor code ID
      ifs >> nce.procNum;

      if (ifs.fail ())   // special: eof is not a failure here
      {
         break;
      }

      this->netEntries [id] = nce;
   }

   ifs.close ();
}

Config::~Config ()
{

}

/**
 * @brief Skip all whitespace and comments in the stream.
 * @return False if some error happened in the stream.
 */
static bool skipJunk (std::ifstream & ifs)
{
   int tmpc;

   // skip whitespaces
   while (ifs.good () && isspace (ifs.get ()));
   ifs.unget ();

   if (!ifs.good ())
   {
      return false;
   }

   // check if this is a comment
   if (ifs.peek () == '#')
   {
      // skip the rest of the line
      do
      {
         tmpc = ifs.get ();
      }
      while (ifs.good () && tmpc != '\n');
   }

   if (!ifs.good ())
   {
      return false;
   }

   return true;
}

