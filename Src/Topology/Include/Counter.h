/*
 * FoREST - Reactive DVFS Control for Multicore Processors
 * Copyright (C) 2013 Universite de Versailles
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

#ifndef H_COUNTER
#define H_COUNTER

#include <stdint.h>

namespace FoREST {

/**
 * Structure containing old and current values of a counter
 */
struct CounterValues {
   uint64_t current;
   uint64_t old;
};

/**
 * Structure representing file descriptors, name and current
 * values of a hardware counter
 */
struct Counter {
   const char *name;
   int fd;
   CounterValues *values;

   Counter () :
      fd (0),
      values (NULL) {}

   Counter (const char *n, unsigned int nbFrequencies = 1) :
      name (n),
      fd (0),
      values (NULL)
   {
      setNbFrequencies (nbFrequencies);
   }

   ~Counter () {
      if (values != NULL) {
         delete [] values;
      }
   }

   void setNbFrequencies (unsigned int nb) {
      if (values != NULL) {
         delete [] values;
      }
      values = new CounterValues [nb];
      memset (values, 0, nb*sizeof (*values));
   }

   float getValue (unsigned int idx = 0) {
      return this->values [idx].current;
   }

   void setValue (unsigned int idx, uint64_t v) {
      CounterValues& values = this->values [idx];
      if (values.old <= v) {
         values.current = v - values.old;
      } else { // Overflow
         // TODO fix
         values.current = 0xFFFFFFFFFFFFFFFFUL - values.old + v;
      }

      // remember this value as the old one
      values.old = v;
   }
};

} // namespace FoREST

#endif
