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

#include "MsgReader.h"
#include "ReportMsg.h"

#include "unistd.h"

#include <iostream>

Message * MsgReader::read_msg (int fd, unsigned int src, unsigned int dst)
{
   Message::Type tp;

   if (read (fd, &tp, sizeof (tp)) < (ssize_t) sizeof (tp))
   {
      return NULL;
   }

   switch (tp)
   {
      case Message::MSG_TP_SLEEP:
      case Message::MSG_TP_WAKE:
      case Message::MSG_TP_DIE:
         return new Message (tp, src, dst);
         break;
      case Message::MSG_TP_REPORT:
         long long data[3];

         if (read (fd, data, sizeof (data)) < (ssize_t) sizeof (data))
         {
            std::cerr << "Incomplete report received" << std::endl;
            return NULL;
         }

         return new ReportMsg (src, dst, data);
      default:
         // UNSUPPORTED
         return NULL;
   }
}

