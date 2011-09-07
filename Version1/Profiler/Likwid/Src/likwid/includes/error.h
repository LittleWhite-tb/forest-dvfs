/*
 * ===========================================================================
 *
 *      Filename:  error.h
 *
 *      Description:  Central error handling macros
 *
 *      Version:  2.2
 *      Created:  14.06.2011
 *
 *      Author:  Jan Treibig (jt), jan.treibig@gmail.com
 *      Company:  RRZE Erlangen
 *      Project:  likwid
 *      Copyright:  Copyright (c) 2010, Jan Treibig
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License, v2, as
 *      published by the Free Software Foundation
 *     
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *     
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * ===========================================================================
 */

#ifndef ERROR_H
#define ERROR_H

#include <errno.h>
#include <string.h>
#include <msr.h>

#define str(x) #x

#define FINALIZE  msr_finalize();

#ifdef WARN
#define WARNING(msg) \
    fprintf(stderr, "WARNING - [%s:%d] " str(msg) "\n", __FILE__, __LINE__)
#else
#define WARNING(msg)
#endif

#define ERROR_PRINT fprintf(stderr, "ERROR - [%s:%d] %s\n", __FILE__, __LINE__, strerror(errno))

#define ERROR  \
    ERROR_PRINT; \
    FINALIZE \
    exit(EXIT_FAILURE)

#define ERROR_MSG(msg)  \
    fprintf(stderr, "ERROR - [%s:%d] " str(msg) "\n", __FILE__, __LINE__); \
    FINALIZE \
    exit(EXIT_FAILURE)

#define ERROR_PMSG(msg,var)  \
    fprintf(stderr, "ERROR - [%s:%d] " str(msg) "\n", __FILE__, __LINE__, var); \
    FINALIZE \
    exit(EXIT_FAILURE)

#define CHECK_ERROR(func, msg)  \
    if ((func) < 0) { fprintf(stderr, "ERROR - [%s:%d] " str(msg) " - %s \n", __FILE__, __LINE__, strerror(errno));}

#define EXIT_IF_ERROR(func, msg)  \
    if ((func) < 0) { fprintf(stderr,"ERROR - [%s:%d] " str(msg) " - %s \n", __FILE__, __LINE__, strerror(errno)); exit(EXIT_FAILURE); }

#ifndef DEBUGLEV
#define DEBUGLEV 0
#endif

#define DEBUGPRINT(lev, fmt, ...) \
    if (DEBUGLEV > lev) { \
        printf(fmt, __VA_ARGS__); \
        fflush(stdout); \
    }

#endif /*ERROR_H*/
