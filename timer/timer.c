/*============================================================================
   PROJECT : Kernel_Mem_Benchmark (kerbe)
   SOURCE  : timer.c
   MODULE  : Performance Counters
   AUTHOR  : Christophe Lemuet
   UPDATED : 09/09/2003 (DD/MM/YYYY)
   UPDATED : 20/09/2005 (DD/MM/YYYY) JT.Acquaviva. Add X86 timer with rdtsc() function
============================================================================*/
#include <stdlib.h>
#include "timer.h"
#include "rdtsc.h"
       
static __inline__ unsigned long long getticks(void)
{
   unsigned long long ret;
   rdtscll(ret);
   return ret;
}

unsigned long long timer_init()     { return EXIT_SUCCESS; }
unsigned long long timer_close()     { return EXIT_SUCCESS; }
double timer_start(){ return  getticks(); }
double timer_stop() { return  getticks(); }
