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
  @file Message.h
  @brief The Message class header is in this file
 */

/*
 * WARNING!
 *
 * IF YOU PLAN TO IMPLEMENT A MESSAGE SUBCLASS, DO NOT FORGET TO ALSO
 * IMPLEMENT THE SPECIALZED READING PROCEDURE IN MsgReader.
 *
 */

#ifndef H_MESSAGE
#define H_MESSAGE

#include "Rest.h"

class Message
{
   public:
      /** @brief Message type */
      typedef enum
      {
         MSG_TP_SLEEP,
         MSG_TP_WAKE,
         MSG_TP_SETWIN,
         MSG_TP_DIE,
         MSG_TP_REPORT,
         MSG_TP_ID
      } Type;

      /**
       * @brief Creates a simple message only carrying type info.
       * @param tp The message type.
       * @param sender The sender node id
       * @param dest The destination node id
       */
      Message (Type tp, unsigned int sender, unsigned int dest);

      /**
       * @brief Returns the type id of the message.
       * @return The type of the message.
       */
      Type get_type ();

      /** @brief Message destructor. */
      virtual ~Message ();

      /**
       * @brief Dumps the message into the file descriptor. The message MUST
       * start by the message type.
       *
       * @param fd The file descriptor where to write into.
       * @return False in case of failure, true otherwise.
       */
      virtual bool write_into (int fd) const;

      /**
       * @brief Returns the sender of this message.
       * @return The message sender.
       */
      unsigned int get_sender () const;

      /**
       * @brief Returns the destination for this message.
       * @return The id of the (desired) destination of the message.
       */
      unsigned int get_dest () const;


   private:
      Type type;
      unsigned int sender;
      unsigned int dest;
};

#endif
