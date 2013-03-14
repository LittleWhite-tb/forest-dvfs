/*
 * FoREST - Reactive DVFS Control for Multicore Processors
 * Copyright (C) 2013 Universite de Versailles
 * Copyright (C) 2011-2012 Exascale Research Center
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file FreqSelector.cpp
 * The FreqSelector class is in this file
 */

#include "FreqSelector.h"

namespace FoREST {

FreqSelector::FreqSelector (unsigned int nbFreqs)
{
   this->nbFreqs = nbFreqs;
   this->coeffs = new float [nbFreqs];

   for (unsigned int i = 0; i < nbFreqs; i++)
   {
      this->coeffs [i] = 0.;
   }
}

FreqSelector::~FreqSelector ()
{
   delete [] this->coeffs;
}

} // namespace FoREST

