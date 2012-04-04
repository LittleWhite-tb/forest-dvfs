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
  @file IdMsg.cpp
  @brief The IdMsg class is in this file
 */

#include "IdMsg.h"

#include <unistd.h>

IdMsg::IdMsg (unsigned int sender, unsigned int dest, unsigned int id)
   : Message (Message::MSG_TP_ID, sender, dest)
{
   this->id = id;
}

IdMsg::~IdMsg ()
{

}

bool IdMsg::write_into (int fd) const
{
   return this->Message::write_into (fd) && write (fd, &this->id, sizeof (this->id)) == sizeof (this->id);
}