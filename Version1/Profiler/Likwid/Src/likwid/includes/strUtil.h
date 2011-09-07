/*
 * ===========================================================================
 *
 *      Filename:  strUtil.h
 *
 *      Description:  Header File strUtil Module. 
 *                    Helper routines for bstrlib and command line parsing
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

#ifndef STRUTIL_H
#define STRUTIL_H

#include <bstrlib.h>
#include <types.h>
#include <time.h>

extern int str2int(const char* str);
extern uint32_t bstr_to_cpuset_physical(uint32_t* threads,  const_bstring q);
extern int bstr_to_cpuset(int* threads,  const_bstring str);
extern void bstr_to_eventset(StrUtilEventSet* set, const_bstring str);
extern bstring bSecureInput (int maxlen, char* vgcCtx);
extern int bJustifyCenter (bstring b, int width);
extern void bstr_to_workgroup(Workgroup* threads,  const_bstring str, DataType type, int numberOfStreams);
extern FILE* bstr_to_outstream(const_bstring argString, bstring filter);
extern int bstr_to_doubleSize(const_bstring str, DataType type);
extern void bstr_to_interval(const_bstring str, struct timespec* interval);

#endif /*STRUTIL_H*/
