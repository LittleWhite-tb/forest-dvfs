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

#include "unistd.h"

Message * MsgReader::read_msg (int fd)
{
   Message::Type tp;

   if (read (fd, &tp, sizeof (tp)) < sizeof (tp))
   {
      return NULL;
   }

   switch (tp)
   {
      case Message::MSG_TP_SLEEP:
      case Message::MSG_TP_WAKE:
      case Message::MSG_TP_DIE:
         return new Message (tp);
         break;
      default:
         // UNSUPPORTED
         return NULL;
   }
}

