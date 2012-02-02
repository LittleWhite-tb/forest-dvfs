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
  @file SetWinMsg.cpp
  @brief The SetWinMsg class is in this file
 */

#include <unistd.h>

#include "SetWinMsg.h"

SetWinMsg::SetWinMsg (unsigned int sender, unsigned int dest, unsigned int win_sz)
   : Message (Message::MSG_TP_SETWIN, sender, dest)
{
   this->win_sz = win_sz;
}

SetWinMsg::~SetWinMsg ()
{

}

bool SetWinMsg::write_into (int fd) const
{
   return this->Message::write_into (fd)
          && write (fd, &this->win_sz, sizeof (this->win_sz)) == sizeof (this->win_sz);
}


