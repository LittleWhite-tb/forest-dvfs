/*
 * Copyright (c) 2011 Google, Inc
 * Contributed by Stephane Eranian <eranian@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * This file is part of libpfm, a performance monitoring support library for
 * applications on Linux.
 *
 * This file has been automatically generated.
 *
 * PMU: coreduo (Intel Core Duo/Core Solo)
 */

static const intel_x86_umask_t coreduo_sse_prefetch []=
{
   {
      .uname  = "NTA",
      .udesc  = "Streaming SIMD Extensions (SSE) Prefetch NTA instructions executed",
      .ucode = 0x0,
   },
   {
      .uname  = "T1",
      .udesc  = "SSE software prefetch instruction PREFE0xTCT1 retired",
      .ucode = 0x100,
   },
   {
      .uname  = "T2",
      .udesc  = "SSE software prefetch instruction PREFE0xTCT2 retired",
      .ucode = 0x200,
   },
};

static const intel_x86_umask_t coreduo_l2_ads []=
{
   {
      .uname  = "SELF",
      .udesc  = "This core",
      .ucode = 0x4000,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL,
   },
   {
      .uname  = "BOTH_CORES",
      .udesc  = "Both cores",
      .ucode = 0xc000,
      .uflags= INTEL_X86_NCOMBO,
   },
};

static const intel_x86_umask_t coreduo_l2_lines_in []=
{
   {
      .uname  = "SELF",
      .udesc  = "This core",
      .ucode = 0x4000,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL,
      .grpid = 0,
   },
   {
      .uname  = "BOTH_CORES",
      .udesc  = "Both cores",
      .ucode = 0xc000,
      .uflags= INTEL_X86_NCOMBO,
      .grpid = 0,
   },
   {
      .uname  = "ANY",
      .udesc  = "All inclusive",
      .ucode = 0x3000,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL,
      .grpid = 1,
   },
   {
      .uname  = "PREFETCH",
      .udesc  = "Hardware prefetch only",
      .ucode = 0x1000,
      .uflags= INTEL_X86_NCOMBO,
      .grpid = 1,
   },
};

static const intel_x86_umask_t coreduo_l2_ifetch []=
{
   {
      .uname  = "MESI",
      .udesc  = "Any cacheline access",
      .ucode = 0xf00,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL,
      .grpid = 0,
   },
   {
      .uname  = "I_STATE",
      .udesc  = "Invalid cacheline",
      .ucode = 0x100,
      .grpid = 0,
   },
   {
      .uname  = "S_STATE",
      .udesc  = "Shared cacheline",
      .ucode = 0x200,
      .grpid = 0,
   },
   {
      .uname  = "E_STATE",
      .udesc  = "Exclusive cacheline",
      .ucode = 0x400,
      .grpid = 0,
   },
   {
      .uname  = "M_STATE",
      .udesc  = "Modified cacheline",
      .ucode = 0x800,
      .grpid = 0,
   },
   {
      .uname  = "SELF",
      .udesc  = "This core",
      .ucode = 0x4000,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL,
      .grpid = 1,
   },
   {
      .uname  = "BOTH_CORES",
      .udesc  = "Both cores",
      .ucode = 0xc000,
      .uflags= INTEL_X86_NCOMBO,
      .grpid = 1,
   },
};

static const intel_x86_umask_t coreduo_l2_rqsts []=
{
   {
      .uname  = "MESI",
      .udesc  = "Any cacheline access",
      .ucode = 0xf00,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL,
      .grpid = 0,
   },
   {
      .uname  = "I_STATE",
      .udesc  = "Invalid cacheline",
      .ucode = 0x100,
      .grpid = 0,
   },
   {
      .uname  = "S_STATE",
      .udesc  = "Shared cacheline",
      .ucode = 0x200,
      .grpid = 0,
   },
   {
      .uname  = "E_STATE",
      .udesc  = "Exclusive cacheline",
      .ucode = 0x400,
      .grpid = 0,
   },
   {
      .uname  = "M_STATE",
      .udesc  = "Modified cacheline",
      .ucode = 0x800,
      .grpid = 0,
   },
   {
      .uname  = "SELF",
      .udesc  = "This core",
      .ucode = 0x4000,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL,
      .grpid = 1,
   },
   {
      .uname  = "BOTH_CORES",
      .udesc  = "Both cores",
      .ucode = 0xc000,
      .uflags= INTEL_X86_NCOMBO,
      .grpid = 1,
   },
   {
      .uname  = "ANY",
      .udesc  = "All inclusive",
      .ucode = 0x3000,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL,
      .grpid = 2,
   },
   {
      .uname  = "PREFETCH",
      .udesc  = "Hardware prefetch only",
      .ucode = 0x1000,
      .uflags= INTEL_X86_NCOMBO,
      .grpid = 2,
   },
};

static const intel_x86_umask_t coreduo_thermal_trip []=
{
   {
      .uname  = "CYCLES",
      .udesc  = "Duration in a thermal trip based on the current core clock",
      .ucode = 0xc000,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "TRIPS",
      .udesc  = "Number of thermal trips",
      .ucode = 0xc000 | INTEL_X86_MOD_EDGE,
      .modhw = _INTEL_X86_ATTR_E,
      .uflags= INTEL_X86_NCOMBO,
   },
};

static const intel_x86_umask_t coreduo_cpu_clk_unhalted []=
{
   {
      .uname  = "CORE_P",
      .udesc  = "Unhalted core cycles",
      .ucode = 0x0,
   },
   {
      .uname  = "NONHLT_REF_CYCLES",
      .udesc  = "Non-halted bus cycles",
      .ucode = 0x100,
   },
   {
      .uname  = "SERIAL_EXECUTION_CYCLES",
      .udesc  = "Non-halted bus cycles of this core executing code while the other core is halted",
      .ucode = 0x200,
   },
};

static const intel_x86_umask_t coreduo_dcache_cache_ld []=
{
   {
      .uname  = "MESI",
      .udesc  = "Any cacheline access",
      .ucode = 0xf00,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL,
   },
   {
      .uname  = "I_STATE",
      .udesc  = "Invalid cacheline",
      .ucode = 0x100,
   },
   {
      .uname  = "S_STATE",
      .udesc  = "Shared cacheline",
      .ucode = 0x200,
   },
   {
      .uname  = "E_STATE",
      .udesc  = "Exclusive cacheline",
      .ucode = 0x400,
   },
   {
      .uname  = "M_STATE",
      .udesc  = "Modified cacheline",
      .ucode = 0x800,
   },
};

static const intel_x86_umask_t coreduo_sse_pre_miss []=
{
   {
      .uname  = "NTA_MISS",
      .udesc  = "PREFETCHNTA missed all caches",
      .ucode = 0x0,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "T1_MISS",
      .udesc  = "PREFETCHT1 missed all caches",
      .ucode = 0x100,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "T2_MISS",
      .udesc  = "PREFETCHT2 missed all caches",
      .ucode = 0x200,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "STORES_MISS",
      .udesc  = "SSE streaming store instruction missed all caches",
      .ucode = 0x300,
      .uflags= INTEL_X86_NCOMBO,
   },
};

static const intel_x86_umask_t coreduo_bus_drdy_clocks []=
{
   {
      .uname  = "THIS_AGENT",
      .udesc  = "This agent",
      .ucode = 0x0,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL,
   },
   {
      .uname  = "ALL_AGENTS",
      .udesc  = "Any agent on the bus",
      .ucode = 0x2000,
      .uflags= INTEL_X86_NCOMBO,
   },
};

static const intel_x86_umask_t coreduo_simd_int_instructions []=
{
   {
      .uname  = "MUL",
      .udesc  = "Number of SIMD Integer packed multiply instructions executed",
      .ucode = 0x100,
   },
   {
      .uname  = "SHIFT",
      .udesc  = "Number of SIMD Integer packed shift instructions executed",
      .ucode = 0x200,
   },
   {
      .uname  = "PACK",
      .udesc  = "Number of SIMD Integer pack operations instruction executed",
      .ucode = 0x400,
   },
   {
      .uname  = "UNPACK",
      .udesc  = "Number of SIMD Integer unpack instructions executed",
      .ucode = 0x800,
   },
   {
      .uname  = "LOGICAL",
      .udesc  = "Number of SIMD Integer packed logical instructions executed",
      .ucode = 0x1000,
   },
   {
      .uname  = "ARITHMETIC",
      .udesc  = "Number of SIMD Integer packed arithmetic instructions executed",
      .ucode = 0x2000,
   },
};

static const intel_x86_umask_t coreduo_mmx_fp_trans []=
{
   {
      .uname  = "TO_FP",
      .udesc  = "Number of transitions from MMX to X87",
      .ucode = 0x0,
   },
   {
      .uname  = "TO_MMX",
      .udesc  = "Number of transitions from X87 to MMX",
      .ucode = 0x100,
   },
};

static const intel_x86_umask_t coreduo_sse_instructions_retired []=
{
   {
      .uname  = "SINGLE",
      .udesc  = "Number of SSE/SSE2 single precision instructions retired (packed and scalar)",
      .ucode = 0x0,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "SCALAR_SINGLE",
      .udesc  = "Number of SSE/SSE2 scalar single precision instructions retired",
      .ucode = 0x100,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "PACKED_DOUBLE",
      .udesc  = "Number of SSE/SSE2 packed double percision instructions retired",
      .ucode = 0x200,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "DOUBLE",
      .udesc  = "Number of SSE/SSE2 scalar double percision instructions retired",
      .ucode = 0x300,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "INT_128",
      .udesc  = "Number of SSE2 128 bit integer  instructions retired",
      .ucode = 0x400,
      .uflags= INTEL_X86_NCOMBO,
   },
};

static const intel_x86_umask_t coreduo_sse_comp_instructions_retired []=
{
   {
      .uname  = "PACKED_SINGLE",
      .udesc  = "Number of SSE/SSE2 packed single precision compute instructions retired (does not include AND, OR, XOR)",
      .ucode = 0x0,
   },
   {
      .uname  = "SCALAR_SINGLE",
      .udesc  = "Number of SSE/SSE2 scalar single precision compute instructions retired (does not include AND, OR, XOR)",
      .ucode = 0x100,
   },
   {
      .uname  = "PACKED_DOUBLE",
      .udesc  = "Number of SSE/SSE2 packed double precision compute instructions retired (does not include AND, OR, XOR)",
      .ucode = 0x200,
   },
   {
      .uname  = "SCALAR_DOUBLE",
      .udesc  = "Number of SSE/SSE2 scalar double precision compute instructions retired (does not include AND, OR, XOR)",
      .ucode = 0x300,
      .uflags= INTEL_X86_NCOMBO,
   },
};

static const intel_x86_umask_t coreduo_fused_uops []=
{
   {
      .uname  = "ALL",
      .udesc  = "All fused uops retired",
      .ucode = 0x0,
   },
   {
      .uname  = "LOADS",
      .udesc  = "Fused load uops retired",
      .ucode = 0x100,
   },
   {
      .uname  = "STORES",
      .udesc  = "Fused load uops retired",
      .ucode = 0x200,
   },
};

static const intel_x86_entry_t intel_coreduo_pe []=
{
   {
      .name   = "UNHALTED_CORE_CYCLES",
      .desc   = "Unhalted core cycles",
      .modmsk = INTEL_X86_ATTRS,
      .equiv = "CPU_CLK_UNHALTED:CORE_P",
      .cntmsk = 0x3,
      .code = 0x3c,
   },
   {
      .name   = "UNHALTED_REFERENCE_CYCLES",
      .desc   = "Unhalted reference cycles. Measures bus cycles",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x13c,
   },
   {
      .name   = "INSTRUCTION_RETIRED",
      .desc   = "Instructions retired",
      .modmsk = INTEL_X86_ATTRS,
      .equiv = "INSTR_RET",
      .cntmsk = 0x3,
      .code = 0xc0,
   },
   {
      .name   = "INSTRUCTIONS_RETIRED",
      .desc   = "This is an alias for INSTRUCTION_RETIRED",
      .modmsk = INTEL_X86_ATTRS,
      .equiv = "INSTRUCTION_RETIRED",
      .cntmsk = 0x3,
      .code = 0xc0,
   },
   {
      .name   = "LLC_REFERENCES",
      .desc   = "Last level of cache references",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x4f2e,
   },
   {
      .name   = "LAST_LEVEL_CACHE_REFERENCES",
      .desc   = "This is an alias for LLC_REFERENCES",
      .modmsk = INTEL_X86_ATTRS,
      .equiv = "LLC_REFERENCES",
      .cntmsk = 0x3,
      .code = 0x4f2e,
   },
   {
      .name   = "LLC_MISSES",
      .desc   = "Last level of cache misses",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x412e,
   },
   {
      .name   = "LAST_LEVEL_CACHE_MISSES",
      .desc   = "This is an alias for LLC_MISSES",
      .modmsk = INTEL_X86_ATTRS,
      .equiv = "LLC_MISSES",
      .cntmsk = 0x3,
      .code = 0x412e,
   },
   {
      .name   = "BRANCH_INSTRUCTIONS_RETIRED",
      .desc   = "Branch instructions retired",
      .modmsk = INTEL_X86_ATTRS,
      .equiv = "BR_INSTR_RET",
      .cntmsk = 0x3,
      .code = 0xc4,
   },
   {
      .name   = "MISPREDICTED_BRANCH_RETIRED",
      .desc   = "Mispredicted branch instruction retired",
      .modmsk = INTEL_X86_ATTRS,
      .equiv = "BR_MISPRED_RET",
      .cntmsk = 0x3,
      .code = 0xc5,
   },
   {
      .name   = "LD_BLOCKS",
      .desc   = "Load operations delayed due to store buffer blocks. The preceding store may be blocked due to unknown address, unknown data, or conflict due to partial overlap between the load and store.",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x3,
   },
   {
      .name   = "SD_DRAINS",
      .desc   = "Cycles while draining store buffers",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x4,
   },
   {
      .name   = "MISALIGN_MEM_REF",
      .desc   = "Misaligned data memory references (MOB splits of loads and stores).",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x5,
   },
   {
      .name   = "SEG_REG_LOADS",
      .desc   = "Segment register loads",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x6,
   },
   {
      .name   = "SSE_PREFETCH",
      .desc   = "Streaming SIMD Extensions (SSE) Prefetch instructions executed",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x7,
      .numasks = LIBPFM_ARRAY_SIZE (coreduo_sse_prefetch),
      .ngrp = 1,
      .umasks = coreduo_sse_prefetch,
   },
   {
      .name   = "SSE_NTSTORES_RET",
      .desc   = "SSE streaming store instruction retired",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x307,
   },
   {
      .name   = "FP_COMPS_OP_EXE",
      .desc   = "FP computational Instruction executed. FADD, FSUB, FCOM, FMULs, MUL, IMUL, FDIVs, DIV, IDIV, FPREMs, FSQRT are included; but exclude FADD or FMUL used in the middle of a transcendental instruction.",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x10,
   },
   {
      .name   = "FP_ASSIST",
      .desc   = "FP exceptions experienced microcode assists",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x2,
      .code = 0x11,
   },
   {
      .name   = "MUL",
      .desc   = "Multiply operations (a speculative count, including FP and integer multiplies).",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x2,
      .code = 0x12,
   },
   {
      .name   = "DIV",
      .desc   = "Divide operations (a speculative count, including FP and integer multiplies). ",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x2,
      .code = 0x13,
   },
   {
      .name   = "CYCLES_DIV_BUSY",
      .desc   = "Cycles the divider is busy ",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x1,
      .code = 0x14,
   },
   {
      .name   = "L2_ADS",
      .desc   = "L2 Address strobes ",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x21,
      .numasks = LIBPFM_ARRAY_SIZE (coreduo_l2_ads),
      .ngrp = 1,
      .umasks = coreduo_l2_ads,
   },
   {
      .name   = "DBUS_BUSY",
      .desc   = "Core cycle during which data buswas busy (increments by 4)",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x22,
      .numasks = LIBPFM_ARRAY_SIZE (coreduo_l2_ads),
      .ngrp = 1,
      .umasks = coreduo_l2_ads, /* identical to actual umasks list for this event */
   },
   {
      .name   = "DBUS_BUSY_RD",
      .desc   = "Cycles data bus is busy transferring data to a core (increments by 4) ",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x23,
      .numasks = LIBPFM_ARRAY_SIZE (coreduo_l2_ads),
      .ngrp = 1,
      .umasks = coreduo_l2_ads, /* identical to actual umasks list for this event */
   },
   {
      .name   = "L2_LINES_IN",
      .desc   = "L2 cache lines allocated",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x24,
      .numasks = LIBPFM_ARRAY_SIZE (coreduo_l2_lines_in),
      .ngrp = 2,
      .umasks = coreduo_l2_lines_in,
   },
   {
      .name   = "L2_M_LINES_IN",
      .desc   = "L2 Modified-state cache lines allocated",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x25,
      .numasks = LIBPFM_ARRAY_SIZE (coreduo_l2_ads),
      .ngrp = 1,
      .umasks = coreduo_l2_ads, /* identical to actual umasks list for this event */
   },
   {
      .name   = "L2_LINES_OUT",
      .desc   = "L2 cache lines evicted ",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x26,
      .numasks = LIBPFM_ARRAY_SIZE (coreduo_l2_lines_in),
      .ngrp = 2,
      .umasks = coreduo_l2_lines_in, /* identical to actual umasks list for this event */
   },
   {
      .name   = "L2_M_LINES_OUT",
      .desc   = "L2 Modified-state cache lines evicted ",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x27,
      .numasks = LIBPFM_ARRAY_SIZE (coreduo_l2_lines_in),
      .ngrp = 2,
      .umasks = coreduo_l2_lines_in, /* identical to actual umasks list for this event */
   },
   {
      .name   = "L2_IFETCH",
      .desc   = "L2 instruction fetches from nstruction fetch unit (includes speculative fetches) ",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x28,
      .numasks = LIBPFM_ARRAY_SIZE (coreduo_l2_ifetch),
      .ngrp = 2,
      .umasks = coreduo_l2_ifetch,
   },
   {
      .name   = "L2_LD",
      .desc   = "L2 cache reads (includes speculation) ",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x29,
      .numasks = LIBPFM_ARRAY_SIZE (coreduo_l2_ifetch),
      .ngrp = 2,
      .umasks = coreduo_l2_ifetch, /* identical to actual umasks list for this event */
   },
   {
      .name   = "L2_ST",
      .desc   = "L2 cache writes (includes speculation)",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x2a,
      .numasks = LIBPFM_ARRAY_SIZE (coreduo_l2_ifetch),
      .ngrp = 2,
      .umasks = coreduo_l2_ifetch, /* identical to actual umasks list for this event */
   },
   {
      .name   = "L2_RQSTS",
      .desc   = "L2 cache reference requests ",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x2e,
      .numasks = LIBPFM_ARRAY_SIZE (coreduo_l2_rqsts),
      .ngrp = 3,
      .umasks = coreduo_l2_rqsts,
   },
   {
      .name   = "L2_REJECT_CYCLES",
      .desc   = "Cycles L2 is busy and rejecting new requests.",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x30,
      .numasks = LIBPFM_ARRAY_SIZE (coreduo_l2_rqsts),
      .ngrp = 3,
      .umasks = coreduo_l2_rqsts, /* identical to actual umasks list for this event */
   },
   {
      .name   = "L2_NO_REQUEST_CYCLES",
      .desc   = "Cycles there is no request to access L2.",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x32,
      .numasks = LIBPFM_ARRAY_SIZE (coreduo_l2_rqsts),
      .ngrp = 3,
      .umasks = coreduo_l2_rqsts, /* identical to actual umasks list for this event */
   },
   {
      .name   = "EST_TRANS_ALL",
      .desc   = "Any Intel Enhanced SpeedStep (R) Technology transitions",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x3a,
   },
   {
      .name   = "EST_TRANS_ALL",
      .desc   = "Intel Enhanced SpeedStep Technology frequency transitions",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x103a,
   },
   {
      .name   = "THERMAL_TRIP",
      .desc   = "Duration in a thermal trip based on the current core clock ",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x3b,
      .numasks = LIBPFM_ARRAY_SIZE (coreduo_thermal_trip),
      .ngrp = 1,
      .umasks = coreduo_thermal_trip,
   },
   {
      .name   = "CPU_CLK_UNHALTED",
      .desc   = "Core cycles when core is not halted",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x3c,
      .numasks = LIBPFM_ARRAY_SIZE (coreduo_cpu_clk_unhalted),
      .ngrp = 1,
      .umasks = coreduo_cpu_clk_unhalted,
   },
   {
      .name   = "DCACHE_CACHE_LD",
      .desc   = "L1 cacheable data read operations",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x40,
      .numasks = LIBPFM_ARRAY_SIZE (coreduo_dcache_cache_ld),
      .ngrp = 1,
      .umasks = coreduo_dcache_cache_ld,
   },
   {
      .name   = "DCACHE_CACHE_ST",
      .desc   = "L1 cacheable data write operations",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x41,
      .numasks = LIBPFM_ARRAY_SIZE (coreduo_dcache_cache_ld),
      .ngrp = 1,
      .umasks = coreduo_dcache_cache_ld, /* identical to actual umasks list for this event */
   },
   {
      .name   = "DCACHE_CACHE_LOCK",
      .desc   = "L1 cacheable lock read operations to invalid state",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x42,
      .numasks = LIBPFM_ARRAY_SIZE (coreduo_dcache_cache_ld),
      .ngrp = 1,
      .umasks = coreduo_dcache_cache_ld, /* identical to actual umasks list for this event */
   },
   {
      .name   = "DATA_MEM_REF",
      .desc   = "L1 data read and writes of cacheable and non-cacheable types",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x143,
   },
   {
      .name   = "DATA_MEM_CACHE_REF",
      .desc   = "L1 data cacheable read and write operations.",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x244,
   },
   {
      .name   = "DCACHE_REPL",
      .desc   = "L1 data cache line replacements",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0xf45,
   },
   {
      .name   = "DCACHE_M_REPL",
      .desc   = "L1 data M-state cache line  allocated",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x46,
   },
   {
      .name   = "DCACHE_M_EVICT",
      .desc   = "L1 data M-state cache line evicted",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x47,
   },
   {
      .name   = "DCACHE_PEND_MISS",
      .desc   = "Weighted cycles of L1 miss outstanding",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x48,
   },
   {
      .name   = "DTLB_MISS",
      .desc   = "Data references that missed TLB",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x49,
   },
   {
      .name   = "SSE_PRE_MISS",
      .desc   = "Streaming SIMD Extensions (SSE) instructions missing all cache levels",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x4b,
      .numasks = LIBPFM_ARRAY_SIZE (coreduo_sse_pre_miss),
      .ngrp = 1,
      .umasks = coreduo_sse_pre_miss,
   },
   {
      .name   = "L1_PREF_REQ",
      .desc   = "L1 prefetch requests due to DCU cache misses",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x4f,
   },
   {
      .name   = "BUS_REQ_OUTSTANDING",
      .desc   = "Weighted cycles of cacheable bus data read requests. This event counts full-line read request from DCU or HW prefetcher, but not RFO, write, instruction fetches, or others.",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x60,
      .numasks = LIBPFM_ARRAY_SIZE (coreduo_l2_lines_in),
      .ngrp = 2,
      .umasks = coreduo_l2_lines_in, /* identical to actual umasks list for this event */
   },
   {
      .name   = "BUS_BNR_CLOCKS",
      .desc   = "External bus cycles while BNR asserted",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x61,
   },
   {
      .name   = "BUS_DRDY_CLOCKS",
      .desc   = "External bus cycles while DRDY asserted",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x62,
      .numasks = LIBPFM_ARRAY_SIZE (coreduo_bus_drdy_clocks),
      .ngrp = 1,
      .umasks = coreduo_bus_drdy_clocks,
   },
   {
      .name   = "BUS_LOCKS_CLOCKS",
      .desc   = "External bus cycles while bus lock signal asserted",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x63,
      .numasks = LIBPFM_ARRAY_SIZE (coreduo_l2_ads),
      .ngrp = 1,
      .umasks = coreduo_l2_ads, /* identical to actual umasks list for this event */
   },
   {
      .name   = "BUS_DATA_RCV",
      .desc   = "External bus cycles while bus lock signal asserted",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x4064,
   },
   {
      .name   = "BUS_TRANS_BRD",
      .desc   = "Burst read bus transactions (data or code)",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x65,
      .numasks = LIBPFM_ARRAY_SIZE (coreduo_l2_ads),
      .ngrp = 1,
      .umasks = coreduo_l2_ads, /* identical to actual umasks list for this event */
   },
   {
      .name   = "BUS_TRANS_RFO",
      .desc   = "Completed read for ownership ",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x66,
      .numasks = LIBPFM_ARRAY_SIZE (coreduo_l2_lines_in),
      .ngrp = 2,
      .umasks = coreduo_l2_lines_in, /* identical to actual umasks list for this event */
   },
   {
      .name   = "BUS_TRANS_IFETCH",
      .desc   = "Completed instruction fetch transactions",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x68,
      .numasks = LIBPFM_ARRAY_SIZE (coreduo_l2_lines_in),
      .ngrp = 2,
      .umasks = coreduo_l2_lines_in, /* identical to actual umasks list for this event */
   },
   {
      .name   = "BUS_TRANS_INVAL",
      .desc   = "Completed invalidate transactions",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x69,
      .numasks = LIBPFM_ARRAY_SIZE (coreduo_l2_lines_in),
      .ngrp = 2,
      .umasks = coreduo_l2_lines_in, /* identical to actual umasks list for this event */
   },
   {
      .name   = "BUS_TRANS_PWR",
      .desc   = "Completed partial write transactions",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x6a,
      .numasks = LIBPFM_ARRAY_SIZE (coreduo_l2_lines_in),
      .ngrp = 2,
      .umasks = coreduo_l2_lines_in, /* identical to actual umasks list for this event */
   },
   {
      .name   = "BUS_TRANS_P",
      .desc   = "Completed partial transactions (include partial read + partial write + line write)",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x6b,
      .numasks = LIBPFM_ARRAY_SIZE (coreduo_l2_lines_in),
      .ngrp = 2,
      .umasks = coreduo_l2_lines_in, /* identical to actual umasks list for this event */
   },
   {
      .name   = "BUS_TRANS_IO",
      .desc   = "Completed I/O transactions (read and write)",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x6c,
      .numasks = LIBPFM_ARRAY_SIZE (coreduo_l2_lines_in),
      .ngrp = 2,
      .umasks = coreduo_l2_lines_in, /* identical to actual umasks list for this event */
   },
   {
      .name   = "BUS_TRANS_DEF",
      .desc   = "Completed defer transactions ",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x206d,
      .numasks = LIBPFM_ARRAY_SIZE (coreduo_l2_ads),
      .ngrp = 1,
      .umasks = coreduo_l2_ads, /* identical to actual umasks list for this event */
   },
   {
      .name   = "BUS_TRANS_WB",
      .desc   = "Completed writeback transactions from DCU (does not include L2 writebacks)",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0xc067,
      .numasks = LIBPFM_ARRAY_SIZE (coreduo_bus_drdy_clocks),
      .ngrp = 1,
      .umasks = coreduo_bus_drdy_clocks, /* identical to actual umasks list for this event */
   },
   {
      .name   = "BUS_TRANS_BURST",
      .desc   = "Completed burst transactions (full line transactions include reads, write, RFO, and writebacks) ",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0xc06e,
      .numasks = LIBPFM_ARRAY_SIZE (coreduo_bus_drdy_clocks),
      .ngrp = 1,
      .umasks = coreduo_bus_drdy_clocks, /* identical to actual umasks list for this event */
   },
   {
      .name   = "BUS_TRANS_MEM",
      .desc   = "Completed memory transactions. This includes Bus_Trans_Burst + Bus_Trans_P + Bus_Trans_Inval.",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0xc06f,
      .numasks = LIBPFM_ARRAY_SIZE (coreduo_bus_drdy_clocks),
      .ngrp = 1,
      .umasks = coreduo_bus_drdy_clocks, /* identical to actual umasks list for this event */
   },
   {
      .name   = "BUS_TRANS_ANY",
      .desc   = "Any completed bus transactions",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0xc070,
      .numasks = LIBPFM_ARRAY_SIZE (coreduo_bus_drdy_clocks),
      .ngrp = 1,
      .umasks = coreduo_bus_drdy_clocks, /* identical to actual umasks list for this event */
   },
   {
      .name   = "BUS_SNOOPS",
      .desc   = "External bus cycles while bus lock signal asserted",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x77,
      .numasks = LIBPFM_ARRAY_SIZE (coreduo_l2_ifetch),
      .ngrp = 2,
      .umasks = coreduo_l2_ifetch, /* identical to actual umasks list for this event */
   },
   {
      .name   = "DCU_SNOOP_TO_SHARE",
      .desc   = "DCU snoops to share-state L1 cache line due to L1 misses ",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x178,
      .numasks = LIBPFM_ARRAY_SIZE (coreduo_l2_ads),
      .ngrp = 1,
      .umasks = coreduo_l2_ads, /* identical to actual umasks list for this event */
   },
   {
      .name   = "BUS_NOT_IN_USE",
      .desc   = "Number of cycles there is no transaction from the core",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x7d,
      .numasks = LIBPFM_ARRAY_SIZE (coreduo_l2_ads),
      .ngrp = 1,
      .umasks = coreduo_l2_ads, /* identical to actual umasks list for this event */
   },
   {
      .name   = "BUS_SNOOP_STALL",
      .desc   = "Number of bus cycles while bus snoop is stalled",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x7e,
   },
   {
      .name   = "ICACHE_READS",
      .desc   = "Number of instruction fetches from ICache, streaming buffers (both cacheable and uncacheable fetches)",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x80,
   },
   {
      .name   = "ICACHE_MISSES",
      .desc   = "Number of instruction fetch misses from ICache, streaming buffers.",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x81,
   },
   {
      .name   = "ITLB_MISSES",
      .desc   = "Number of iITLB misses",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x85,
   },
   {
      .name   = "IFU_MEM_STALL",
      .desc   = "Cycles IFU is stalled while waiting for data from memory",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x86,
   },
   {
      .name   = "ILD_STALL",
      .desc   = "Number of instruction length decoder stalls (Counts number of LCP stalls)",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x87,
   },
   {
      .name   = "BR_INST_EXEC",
      .desc   = "Branch instruction executed (includes speculation).",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x88,
   },
   {
      .name   = "BR_MISSP_EXEC",
      .desc   = "Branch instructions executed and mispredicted at execution  (includes branches that do not have prediction or mispredicted)",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x89,
   },
   {
      .name   = "BR_BAC_MISSP_EXEC",
      .desc   = "Branch instructions executed that were mispredicted at front end",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x8a,
   },
   {
      .name   = "BR_CND_EXEC",
      .desc   = "Conditional branch instructions executed",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x8b,
   },
   {
      .name   = "BR_CND_MISSP_EXEC",
      .desc   = "Conditional branch instructions executed that were mispredicted",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x8c,
   },
   {
      .name   = "BR_IND_EXEC",
      .desc   = "Indirect branch instructions executed",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x8d,
   },
   {
      .name   = "BR_IND_MISSP_EXEC",
      .desc   = "Indirect branch instructions executed that were mispredicted",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x8e,
   },
   {
      .name   = "BR_RET_EXEC",
      .desc   = "Return branch instructions executed",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x8f,
   },
   {
      .name   = "BR_RET_MISSP_EXEC",
      .desc   = "Return branch instructions executed that were mispredicted",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x90,
   },
   {
      .name   = "BR_RET_BAC_MISSP_EXEC",
      .desc   = "Return branch instructions executed that were mispredicted at the front end",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x91,
   },
   {
      .name   = "BR_CALL_EXEC",
      .desc   = "Return call instructions executed",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x92,
   },
   {
      .name   = "BR_CALL_MISSP_EXEC",
      .desc   = "Return call instructions executed that were mispredicted",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x93,
   },
   {
      .name   = "BR_IND_CALL_EXEC",
      .desc   = "Indirect call branch instructions executed",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0x94,
   },
   {
      .name   = "RESOURCE_STALL",
      .desc   = "Cycles while there is a resource related stall (renaming, buffer entries) as seen by allocator",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0xa2,
   },
   {
      .name   = "MMX_INSTR_EXEC",
      .desc   = "Number of MMX instructions executed (does not include MOVQ and MOVD stores)",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0xb0,
   },
   {
      .name   = "SIMD_INT_SAT_EXEC",
      .desc   = "Number of SIMD Integer saturating instructions executed",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0xb1,
   },
   {
      .name   = "SIMD_INT_INSTRUCTIONS",
      .desc   = "Number of SIMD Integer instructions executed",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0xb3,
      .numasks = LIBPFM_ARRAY_SIZE (coreduo_simd_int_instructions),
      .ngrp = 1,
      .umasks = coreduo_simd_int_instructions,
   },
   {
      .name   = "INSTR_RET",
      .desc   = "Number of instruction retired (Macro fused instruction count as 2)",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0xc0,
   },
   {
      .name   = "FP_COMP_INSTR_RET",
      .desc   = "Number of FP compute instructions retired (X87 instruction or instruction that contain X87 operations)",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x1,
      .code = 0xc1,
   },
   {
      .name   = "UOPS_RET",
      .desc   = "Number of micro-ops retired (include fused uops)",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0xc2,
   },
   {
      .name   = "SMC_DETECTED",
      .desc   = "Number of times self-modifying code condition detected",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0xc3,
   },
   {
      .name   = "BR_INSTR_RET",
      .desc   = "Number of branch instructions retired",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0xc4,
   },
   {
      .name   = "BR_MISPRED_RET",
      .desc   = "Number of mispredicted branch instructions retired",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0xc5,
   },
   {
      .name   = "CYCLES_INT_MASKED",
      .desc   = "Cycles while interrupt is disabled",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0xc6,
   },
   {
      .name   = "CYCLES_INT_PEDNING_MASKED",
      .desc   = "Cycles while interrupt is disabled and interrupts are pending",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0xc7,
   },
   {
      .name   = "HW_INT_RX",
      .desc   = "Number of hardware interrupts received",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0xc8,
   },
   {
      .name   = "BR_TAKEN_RET",
      .desc   = "Number of taken branch instruction retired",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0xc9,
   },
   {
      .name   = "BR_MISPRED_TAKEN_RET",
      .desc   = "Number of taken and mispredicted branch instructions retired",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0xca,
   },
   {
      .name   = "MMX_FP_TRANS",
      .desc   = "Transitions from MMX (TM) Instructions to Floating Point Instructions",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0xcc,
      .numasks = LIBPFM_ARRAY_SIZE (coreduo_mmx_fp_trans),
      .ngrp = 1,
      .umasks = coreduo_mmx_fp_trans,
   },
   {
      .name   = "MMX_ASSIST",
      .desc   = "Number of EMMS executed",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0xcd,
   },
   {
      .name   = "MMX_INSTR_RET",
      .desc   = "Number of MMX instruction retired",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0xce,
   },
   {
      .name   = "INSTR_DECODED",
      .desc   = "Number of instruction decoded",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0xd0,
   },
   {
      .name   = "ESP_UOPS",
      .desc   = "Number of ESP folding instruction decoded",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0xd7,
   },
   {
      .name   = "SSE_INSTRUCTIONS_RETIRED",
      .desc   = "Number of SSE/SSE2 instructions retired (packed and scalar)",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0xd8,
      .numasks = LIBPFM_ARRAY_SIZE (coreduo_sse_instructions_retired),
      .ngrp = 1,
      .umasks = coreduo_sse_instructions_retired,
   },
   {
      .name   = "SSE_COMP_INSTRUCTIONS_RETIRED",
      .desc   = "Number of computational SSE/SSE2 instructions retired (does not include AND, OR, XOR)",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0xd9,
      .numasks = LIBPFM_ARRAY_SIZE (coreduo_sse_comp_instructions_retired),
      .ngrp = 1,
      .umasks = coreduo_sse_comp_instructions_retired,
   },
   {
      .name   = "FUSED_UOPS",
      .desc   = "Fused uops retired",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0xda,
      .numasks = LIBPFM_ARRAY_SIZE (coreduo_fused_uops),
      .ngrp = 1,
      .umasks = coreduo_fused_uops,
   },
   {
      .name   = "UNFUSION",
      .desc   = "Number of unfusion events in the ROB (due to exception)",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0xdb,
   },
   {
      .name   = "BR_INSTR_DECODED",
      .desc   = "Branch instructions decoded",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0xe0,
   },
   {
      .name   = "BTB_MISSES",
      .desc   = "Number of branches the BTB did not produce a prediction",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0xe2,
   },
   {
      .name   = "BR_BOGUS",
      .desc   = "Number of bogus branches",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0xe4,
   },
   {
      .name   = "BACLEARS",
      .desc   = "Number of BAClears asserted",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0xe6,
   },
   {
      .name   = "PREF_RQSTS_UP",
      .desc   = "Number of hardware prefetch requests issued in forward streams",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0xf0,
   },
   {
      .name   = "PREF_RQSTS_DN",
      .desc   = "Number of hardware prefetch requests issued in backward streams",
      .modmsk = INTEL_X86_ATTRS,
      .cntmsk = 0x3,
      .code = 0xf8,
   },
};
