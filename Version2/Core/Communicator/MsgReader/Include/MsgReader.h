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
  @file MsgReader.h
  @brief The MsgReader class header is in this file
 */

#ifndef H_MSGREADER
#define H_MSGREADER

#include "Rest.h"
#include "Message.h"

class MsgReader
{
   public:
      /**
       * @brief Utility function to read a message.
       * @param fd The file descriptor where to read from.
       * @return A pointer to the read message or NULL. Free with delete.
       */
      static Message * read_msg (int fd);
};

#endif
