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
  @file LibProf.cpp
  @brief The LibProf class is in this file
 */

#include "LibProf.h"

#include <unistd.h>
#include <sys/syscall.h>

LibProf::LibProf ()
{

}

LibProf::~LibProf ()
{

}

unsigned long long LibProf::getTicks ()
{
   unsigned long long res;
   rdtscll (res);
   return res;
}

unsigned long int LibProf::getTID()
{
#ifdef SYS_gettid
   return syscall (SYS_gettid);
#elif defined(__NR_gettid)
   return syscall (__NR_gettid);
#else
#error "Unable to implement gettid."
#endif
}
