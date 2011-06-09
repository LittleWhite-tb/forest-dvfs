/*============================================================================
   PROJECT : Kernel_Mem_Benchmark (kerbe)
   SOURCE  : timer.h
   MODULE  : Performance Counters
   AUTHOR  : Christophe Lemuet
   UPDATED : 09/09/2003 (DD/MM/YYYY)
============================================================================*/

#ifndef H_TIMER
#define H_TIMER

#include "defines.h"

unsigned long long timer_init (void);
unsigned long long timer_close (void);
double  timer_start (void);
double  timer_stop (void);
 

#define TIMER_START timer_start();
#define TIMER_STOP  timer_stop();

#endif
