#ifndef MICROBENCH1_H
#define MICROBENCH1_H

#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <dlfcn.h>
#include <assert.h>
#include "rdtsc.h"
#include "../power/timer.h"

#define CPU_BOUND_ITER (cpu_iters)

#define MEM_BOUND_ITER (mem_iters)

#define MEM_BOUND_FOOTPRINT (4*1024*1024 / 32)

#define NUM_UP_DOWN (outer_loop_bound)

#define round(x) ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))


#endif
