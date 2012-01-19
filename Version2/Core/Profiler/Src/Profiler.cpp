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
  @file Profiler.cpp
  @brief The Profiler header is in this file
 */

#include "Profiler.h"
#include "ThreadedProfiler.h"
#include "YellowPages.h"

#include <unistd.h>
#include <iostream>
#include <sstream>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <string>


Profiler::Profiler (void)
{
   this->comm = new Communicator();
}

Profiler::~Profiler (void)
{
   delete this->comm;
}

#define MEM_BOUND_FOOTPRINT (4*1024*1024 / 32)
static double BigVec[MEM_BOUND_FOOTPRINT*32];
static double DummyVec1[12]= {5.0,3.0,9.0,1.5,5.0,19.0,1.3,1.3,1.4,7.0,9.0,1.4};
static double DummyVec2[12]= {5.0,8.0,1.4,1.2,8.0,9.0,1.4,10.9,1.2,4.0,1.5,1.3};

static inline int fastRand()
{
   static int g_seed = 14545265746874;
   g_seed = (214013*g_seed+2531011);

   return (g_seed>>16) &0x7FFF;
}

int main (int argc, char ** argv)
{
   int i, j;

   if (argc != 3)
   {
      std::cerr << "Usage: " << argv[0] << " id config" << std::endl;
      return 1;
   }

   unsigned int id;
   std::istringstream iss (argv[1], std::istringstream::in);
   iss >> id;
   std::string confpath (argv[2]);
   YellowPages::init_from (id, confpath);

   //calculate loop bounds
   float expected_loop_interval=1000;
   long updown_mem_product=5000*32;
   long cpu_2_mem_ratio=3793110/85;
   int NUM_UP_DOWN= (60*1000) / (expected_loop_interval*2);
   int MEM_BOUND_ITER=round ( (float) updown_mem_product/ (float) NUM_UP_DOWN);
   int CPU_BOUND_ITER=MEM_BOUND_ITER*cpu_2_mem_ratio;

NUM_UP_DOWN=1;

   ThreadedProfiler * p = new ThreadedProfiler();

   for (i=0; i<NUM_UP_DOWN; i++)
   {
      double temp[8];

      //std::cout << "cpu" << std::endl;

      // cpu
      temp[0]=DummyVec1[0];
      temp[1]=DummyVec1[1];
      temp[2]=DummyVec1[2];
      temp[3]=DummyVec1[3];

      for (j=0; j<CPU_BOUND_ITER; j++)
      {
         temp[0]=temp[0]*temp[0];
         temp[1]=temp[1]*temp[1];
         temp[2]=temp[2]*temp[2];
         temp[3]=temp[3]*temp[3];
      }

      DummyVec2[0]=temp[0];
      DummyVec2[1]=temp[1];
      DummyVec2[2]=temp[2];
      DummyVec2[3]=temp[3];

      //std::cout << DummyVec2[0] << std::endl;
      //std::cout << "mem" << std::endl;

      // memory
      for (j=0; j<MEM_BOUND_ITER; j++)
      {
         memcpy (&BigVec[fastRand() % (MEM_BOUND_FOOTPRINT*31) ],&BigVec[fastRand() % (MEM_BOUND_FOOTPRINT*31) ],
                 MEM_BOUND_FOOTPRINT/2*sizeof (*BigVec));
      }
   }

   std::cout << BigVec[50] << DummyVec2[3] << std::endl;

   delete p;

   return 0;
}

