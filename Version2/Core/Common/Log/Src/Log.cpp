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
  @file Log.cpp
  @brief The Log class is in this file
 */

#include "Log.h"
#include <sstream>
#include <cstdio>

std::map<int, Log *> Log::all_logs;
Log::Verbosity general_verbosity = Log::VERB_DBG;;

Log::Log (int id)
{
   std::ostringstream oss;

   oss << "log_" << id;
   this->fd = fopen (oss.str ().c_str (), "w");

   this->verbosity = VERB_DBG;
}

Log::~Log ()
{
   fflush (this->fd);
   fclose (this->fd);
}

Log * Log::get_log (int id)
{
   std::map<int, Log *>::iterator it;

   it = Log::all_logs.find (id);
   if (it == Log::all_logs.end ())
   {
      Log * res = new Log (id);
      Log::all_logs [id] = res;
   }

   return Log::all_logs [id];
}

void Log::close_logs ()
{
   std::map<int, Log *>::iterator it;
   for (it = Log::all_logs.begin ();
         it != Log::all_logs.end ();
         it++)
   {
      delete it->second;
   }

   Log::all_logs.clear ();
}

void Log::set_general_verb (Log::Verbosity min_verb)
{
   std::map<int, Log *>::iterator it;
   for (it = Log::all_logs.begin ();
         it != Log::all_logs.end ();
         it++)
   {
      it->second->verbosity = min_verb;
   }
}

void Log::log_extended (const char * filename, unsigned int lno,
                        const char * func, Verbosity verb, const char * fmt, ...) const
{
   std::ostringstream oss;
   va_list args;

   if (verb < this->verbosity)
   {
      return;
   }

   oss << "[" << func << ":" << filename << ":" << lno << "]" << std::endl
       << Log::get_verb_str (verb) << ": ";
   fwrite (oss.str ().c_str (), 1, oss.str ().size (), this->fd);

   va_start (args, fmt);
   vfprintf (this->fd, fmt, args);
   va_end (args);
}


