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
  @file Message.cpp
  @brief The Message class is in this file
 */

#include "Message.h"

#include <unistd.h>

Message::Message (Message::Type tp, unsigned int sender, unsigned int dest)
{
   this->type = tp;
   this->sender = sender;
   this->dest = dest;
}

Message::~Message ()
{

}

Message::Type Message::get_type ()
{
   return this->type;
}

bool Message::write_into (int fd) const
{
   if (write (fd, &this->type, sizeof (this->type)) < (ssize_t) sizeof (this->type))
   {
      return false;
   }

   return true;
}


unsigned int Message::get_sender () const
{
   return this->sender;
}

unsigned int Message::get_dest () const
{
   return this->dest;
}
