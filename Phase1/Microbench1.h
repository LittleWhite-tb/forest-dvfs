#ifndef MICROBENCH1_H

#define MICROBENCH1_H

#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <dlfcn.h>
#include "../power/timer.h"

#define CPU_BOUND_ITER (818822656)

#define MEM_BOUND_ITER 9217728

#define NUM_UP_DOWN 25

#endif
