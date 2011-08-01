#ifndef MICROBENCH1_H
#define MICROBENCH1_H


#define CPU_BOUND_ITER (cpu_iters)

#define MEM_BOUND_ITER (mem_iters)

#define MEM_BOUND_FOOTPRINT (4*1024*1024 / 32)

#define NUM_UP_DOWN (outer_loop_bound)

#define round(x) ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))


#endif
