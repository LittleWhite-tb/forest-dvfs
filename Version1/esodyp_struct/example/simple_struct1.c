/*
  This code builds a framework that helps model a program's behaviour. It is programmed
  in such a way that is should seem to the user as being a black-box. Not a lot of functions
  are needed to be able to make it run.

  For any problem of installation or usage, please email me at: beyler@dpt-info.u-strasbg.fr

  Copyright (C) 2006 Beyler Jean Christophe

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

#include <stdio.h>
#include <stdlib.h>

#include "../Markov_struct.h"

#define N 20
#define M 1000

int access[10] = {
		 2,4,1,5,3,
		 12,23,4,6,17};
		
int main(void)
{
int i,idx;

SGlobalAdd t[N];

//Initialize memory
SMarkov* m = Markov_Initialize();

//Init table
for(i=0;i<N;i++)
   {
   //Set low
   t[i].low = 1024+(i%2)*32;
   //Set high
   t[i].high = 1;
   }

for(i=0,idx=0;i<M;i++)
   {
   idx += access[i%10];
   idx %= N;
   
   m->fct(m,&(t[idx]));
   }
   
printf("Finished simulation\n");
//Free memory
Markov_Clear(m);

return EXIT_SUCCESS;
}
