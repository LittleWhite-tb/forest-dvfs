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
  @file MsgReader.cpp
  @brief The MsgReader class is in this file
 */


#include <unistd.h>
#include <iostream>
#include <stdint.h>

#include "MsgReader.h"
#include "ReportMsg.h"
#include "IdMsg.h"
#include "SetWinMsg.h"

/**
 * Robust reading procedure where we try as much as possible to read the
 * requested amount of data.
 *
 * @param fd The socket where to read the data
 * @param buf The buffer in which writting this data
 * @param bufsz The size of the data to read.
 * @return true if the reading procedure succeed.
 */
static bool robust_read (int fd, void * buf, unsigned int bufsz)
{
   unsigned int done = 0;

   do
   {
      unsigned int tmp_done;

      tmp_done = read (fd, ((uint8_t *) buf) + done, bufsz - done);

      if (tmp_done <= 0)
      {
         return false;
      }

      done += tmp_done;
   }
   while (done < bufsz);

   return true;
}

Message * MsgReader::read_msg (int fd, unsigned int src, unsigned int dst)
{
   Message::Type tp;

   if (!robust_read (fd, &tp, sizeof (tp)))
   {
      return NULL;
   }

   switch (tp)
   {
      case Message::MSG_TP_SLEEP:
      case Message::MSG_TP_WAKE:
      case Message::MSG_TP_DIE:
         return new Message (tp, src, dst);

      case Message::MSG_TP_REPORT:
         long long data [3];

         if (!robust_read (fd, data, sizeof (data)))
         {
            std::cerr << "Incomplete report received" << std::endl;
            return NULL;
         }

         return new ReportMsg (src, dst, data);

      case Message::MSG_TP_ID:
         unsigned int id;

         if (!robust_read (fd, &id, sizeof (id)))
         {
            std::cerr << "Incomplete ID message received" << std::endl;
            return NULL;
         }

         return new IdMsg (src, dst, id);

      case Message::MSG_TP_SETWIN:
         unsigned int win_sz;

         if (!robust_read (fd, &win_sz, sizeof (win_sz)))
         {
            std::cerr << "Incomplete set window message received" << std::endl;
            return NULL;
         }

         return new SetWinMsg (src, dst, win_sz);

      default:
         // UNSUPPORTED
         return NULL;
   }
}

