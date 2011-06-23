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

#ifndef _DEFINE_H_
#define _DEFINE_H_

/**@brief data structure containing all the needed information for the decision system*/
typedef struct _freqData
{
	int *availablefreqs; /**< @brief the available frequences on the system*/
	int freqMax;         /**< @brief the highest available frequence*/
	int freqMin;         /**< @brief the lowest available frequence*/
	int numberOfFreq;    /**< @brief the number of available frequences*/
}freqData;

typedef struct core
{
	int core_nb;
	int* threads;
	int nb_thread;
	float core_median_value;
}Core;

#endif
