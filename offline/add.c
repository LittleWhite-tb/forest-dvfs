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

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
   long unsigned int nbIteration = 1;
   
   if ( argc != 2 )
   {
      fprintf(stderr,"This program need on argument : the number of iteration\n");
      return EXIT_FAILURE;
   }
   
   if ( sscanf(argv[1],"%lu",&nbIteration) != 1 )
   {
      fprintf(stderr,"Fail to get the number of iteration from argument\n");
      return EXIT_FAILURE;
   }
   
   long unsigned int i = 0;
   for ( i = 0 ; i < nbIteration ; i++ )
   {
      asm volatile(\
      "add %%r8, %%r8\n\t"\
      "add %%r9, %%r9\n\t"\
      "add %%r10, %%r10\n\t"\
      "add %%r11, %%r11\n\t"\
      "add %%r12, %%r12\n\t"\
      "add %%r13, %%r13\n\t"\
      "add %%r14, %%r14\n\t"\
      "add %%r15, %%r15\n\t"\
      :\
      :\
      :"%r15","%r14","%r13","%r12","%r11","%r10","%r9","%r8"\
      );
   }
   
   return EXIT_SUCCESS;
}
