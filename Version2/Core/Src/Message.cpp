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

Message::Message(Type tp)
{
    this->type = tp;
}

Message::~Message()
{

}

bool Message::write_into(int fd) const
{
    if (write(fd, &this->type, sizeof(this->type)) < sizeof(this->type)) {
        return false;
    }

    return true;
}

void Message::getInformation(void) {

}
