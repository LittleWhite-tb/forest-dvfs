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
 @file DecisionMaker.cpp
 @brief The  DecisionMaker class is in this file
 */

#include  <assert.h>

#include "DecisionMaker.h"


DecisionMaker::DecisionMaker (CoresInfos * coresInfos)
{
   this->coresInfo = coresInfos;
}

DecisionMaker::~DecisionMaker (void)
{

}

float DecisionMaker::computeBoundness (long long sqFullStall, long long unhaltedCore, long long l2Miss)
{
   float res;

   if (unhaltedCore == 0)
   {
      return 0.5;
   }

   res = (4.*16*sqFullStall*l2Miss) / (unhaltedCore*unhaltedCore);
   res = res > 1 ? 1 : res;

   return res;
}
