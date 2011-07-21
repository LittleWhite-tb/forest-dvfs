/*
Copyright (C) 2011 Exascale Computing Research

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

#ifndef DECISIONPROCESS_H
#define DECISIONPROCESS_H

typedef int Core;

/**
 * @brief decide which algorithm we have to process
 **/ 
void decisionAlgorithm ();

/**
 * @brief compute the median in one core
 * @param core the core on which we want the median
 */
float compute_median_core(Core core);

#endif 
