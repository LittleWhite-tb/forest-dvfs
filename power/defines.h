/*============================================================================
   PROJECT : Kernel_Mem_Benchmark (kerbe)
   SOURCE  : defines.h
   MODULE  : Contient tous les defines de l'exp�ience
   AUTHOR  : Christophe Lemuet
   UPDATED : 02/05/2003 (DD/MM/YYYY)
============================================================================*/

#ifndef _DEFINES_H_
#define _DEFINES_H_

#ifdef X86
	#define PROC_STRING "X86"
#else
    #error "Only supporting X86"
#endif

#define ALIGNEMENT ALIGNED

#ifdef ELTCHAR
#  define ELT char
#  define SZELT "char"
#  define FMT "%c "
#elif ELTSHORT
#  define ELT short
#  define SZELT "short"
#  define FMT "%d "
#elif  ELTINT
#  define ELT int
#  define SZELT "int"
#  define FMT "%d "
#elif  ELTFLOAT
#  define ELT float
#  define SZELT "float"
#  define FMT "%f "
#elif  ELTDOUBLE
#  define ELT double
#  define SZELT "double"
#  define FMT "%lf "
#elif  ELTLONG
#  define ELT long
#  define SZELT "long"
#  define FMT "%ld "
#endif           

/**
 * @todo Move this to a separate .h
 */
#include <stdint.h>


/**
 * @brief struct sBenchResult contains the results of a benchmark program run
 */
typedef struct sBenchResult 
{
	double *time;            /**< @brief Time spent by the benchmark program */
    uint64_t *iterations;    /**< @brief Number of iterations */
} BenchResult;

#endif


