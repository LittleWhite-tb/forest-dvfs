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
  @file SetWinMsg.h
  @brief The SetWinMsg class header is in this file
 */

#ifndef H_SETWINMSG
#define H_SETWINMSG

#include "Message.h"

class SetWinMsg : public Message
{
   public:
      /**
       * @brief Sole constructor.
       * @param sender The sender node's id
       * @param dest The destination node's id
       * @param win_sz The window size carried by the message.
       */
      SetWinMsg (unsigned int sender, unsigned int dest, unsigned int win_sz);

      /**
       * @brief Sole destructor.
       */
      ~SetWinMsg();

      /**
         * @brief Dumps the message into the file descriptor. The message MUST
         * start by the message type.
         *
         * @param fd The file descriptor where to write into.
         * @return False in case of failure, true otherwise.
         */
      bool write_into (int fd) const;


      /**
       * @brief Returns the size of the window in the message.
       */
      inline unsigned int get_window_size()
      {
         return this->win_sz;
      }

   private:
      unsigned int win_sz;
};

#endif
