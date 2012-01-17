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
  @file ReportMsg.h
  @brief The ReportMsg class header is in this file
 */

#ifndef H_REPORTMSG
#define H_REPORTMSG

#include "Message.h"

class ReportMsg: public Message 
{
    public:
        /**
         * @brief New report message.
         * @param report The 3 values read from H/W.
         * @param sender The source of the message
         * @param dest The message destination
         */
        ReportMsg(unsigned int sender, unsigned int dest, long long report[3]);

        /** @brief Sole destructor. */
        ~ReportMsg();

        /**
         * @brief Returns the report content.
         * @return The report as an array of three values.
         */
        const long long *get_report() const;

      /**
       * @brief Dumps the message into the file descriptor.
       * @param fd The file descriptor where to write into.
       * @return False in case of failure, true otherwise.
       */
      bool write_into (int fd) const;


    private:
        long long data[3];
};

#endif

