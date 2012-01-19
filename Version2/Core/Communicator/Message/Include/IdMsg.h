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
  @file IdMsg.h
  @brief The IsMsg class header is in this file
 */

#ifndef H_IDMSG
#define H_IDMSG

#include "Message.h"

class IdMsg : public Message
{
   public:
      /**
       * @brief Creates a simple message to identify which node is at the
       * beginning of the communication channel.
       *
       * @param tp The message type.
             * @param sender The sender node id.
             * @param dest The destination node id.
             * @param id The id carried in the message.
       */
      IdMsg (unsigned int sender, unsigned int dest, unsigned int id);

      /**
       * @brief Sole destructor.
       */
      ~IdMsg();


      /**
       * @brief Dumps the message into the file descriptor.
       * @param fd The file descriptor where to write into.
       * @return False in case of failure, true otherwise.
       */
      bool write_into (int fd) const;

      /**
       * @brief Returns the id carried by the message.
       *
       * @return The id in the message.
       */
      inline unsigned int get_id()
      {
         return this->id;
      }

   private:

      /**
       * The id carried by the message.
       */
      unsigned int id;

};

#endif

