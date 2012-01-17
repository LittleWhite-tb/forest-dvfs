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
 @file ReportMsg.cpp
 @brief The ReportMsg class is in this file
 */

#include "ReportMsg.h"
#include <unistd.h>

ReportMsg::ReportMsg(unsigned int sender, unsigned int dest, long long report[3]) : Message(Message::MSG_TP_REPORT, sender, dest) {
    this->data[0] = report[0];
    this->data[1] = report[1];
    this->data[2] = report[2];
}

ReportMsg::~ReportMsg() {

}

const long long *ReportMsg::get_report() const {
    return this->data;
}

bool ReportMsg::write_into (int fd) const {
    return write(fd, this->data, sizeof(this->data)) == sizeof(this->data);
}

