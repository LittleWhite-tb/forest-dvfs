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
  @file Log.h
  @brief The Log class header is in this file
 */

#ifndef H_LOG
#define H_LOG

#include <cstdio>
#include <cstdarg>
#include <map>

#include <sys/types.h>
#include <unistd.h>
#define LOG_ID() getpid ()

#define LOG(verb, fmt, ...) log_extended(__FILE__, __LINE__, __FUNCTION__,\
                                         verb, fmt, __VA_ARGS__)

class Log
{
   public:
      typedef enum
      {
         VERB_DBG,
         VERB_NFO,
         VERB_WARN,
         VERB_ERR
      } Verbosity;

      /** @brief Destructor. */
      ~Log ();

      /**
       * @brief Returns the log to use for the entity with the given id.
       * @param id The entity id for which the log is wanted. Consider using
       * the LOG_ID macro here.
       *
       * @return A log that can be used by the entity with the given id.
       */
      static Log * get_log (int id);

      /**
       * @brief Clears the memory used by the logs opened so far.
       */
      static void close_logs ();

      /**
       * @brief Sets the verbosity for all opened log
       *
       * @param min_verb The minimal verbosity required to display the message.
       */
      static void set_general_verb (Verbosity min_verb);

      /**
       * @brief Logs a message. Do not use directly, use the LOG macro
       * instead.
       *
       * @param file The source filename
       * @param lno Line in the source file
       * @param func The function name in which this is called
       * @param verb The level of the message
       * @param fmt Message as for printf
       */
      void log_extended (const char * filename, unsigned int lno,
                         const char * func, Verbosity verb, const char * fmt, ...) const;

   private:
      /**
       * @brief Constructor.
       * @param id The id of the concerned entity. Can be a pid, tid, ...
       */
      Log (int id);

      /** @brief File in which we write. */
      FILE * fd;

      /** @brief Current verbosity. */
      Verbosity verbosity;

      static inline const char * get_verb_str (Verbosity v)
      {
         if (v == VERB_DBG)
         {
            return "DEBUG";
         }
         else if (v == VERB_NFO)
         {
            return "INFO ";
         }
         else if (v == VERB_WARN)
         {
            return "WARN ";
         }
         else if (v == VERB_ERR)
         {
            return "ERROR";
         }
         else
         {
            return " ??? ";
         }
      }

      /**
       * @brief All logs openned so far.
       */
      static std::map<int, Log *> all_logs;

      /**
       * @brief Current general verbosity.
       */
      static Verbosity general_verbosity;

};

#endif

