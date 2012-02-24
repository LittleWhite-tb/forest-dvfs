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
 * PMU: wsm (Intel Westmere (single-socket))
 */

static const intel_x86_umask_t wsm_uops_decoded []=
{
   {
      .uname  = "ESP_FOLDING",
      .udesc  = "Stack pointer instructions decoded",
      .ucode = 0x400,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "ESP_SYNC",
      .udesc  = "Stack pointer sync operations",
      .ucode = 0x800,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "MS",
      .udesc  = "Counts the number of uops decoded by the Microcode Sequencer (MS). The MS delivers uops when the instruction is more than 4 uops long or a microcode assist is occurring.",
      .ucode = 0x200,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "MS_CYCLES_ACTIVE",
      .udesc  = "Uops decoded by Microcode Sequencer",
      .uequiv = "MS:c=1",
      .ucode = 0x200 | (0x1 << INTEL_X86_CMASK_BIT),
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "STALL_CYCLES",
      .udesc  = "Cycles no Uops are decoded",
      .ucode = 0x100 | INTEL_X86_MOD_INV | (0x1 << INTEL_X86_CMASK_BIT),
      .uflags= INTEL_X86_NCOMBO,
   },
};

static const intel_x86_umask_t wsm_bpu_clears []=
{
   {
      .uname  = "EARLY",
      .udesc  = "Early Branch Prediction Unit clears",
      .ucode = 0x100,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "LATE",
      .udesc  = "Late Branch Prediction Unit clears",
      .ucode = 0x200,
      .uflags= INTEL_X86_NCOMBO,
   },
};

static const intel_x86_umask_t wsm_uops_retired []=
{
   {
      .uname  = "ANY",
      .udesc  = "Uops retired (Precise Event)",
      .ucode = 0x100,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS | INTEL_X86_DFL,
   },
   {
      .uname  = "MACRO_FUSED",
      .udesc  = "Macro-fused Uops retired (Precise Event)",
      .ucode = 0x400,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
   },
   {
      .uname  = "RETIRE_SLOTS",
      .udesc  = "Retirement slots used (Precise Event)",
      .ucode = 0x200,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
   },
   {
      .uname  = "STALL_CYCLES",
      .udesc  = "Cycles Uops are not retiring (Precise Event)",
      .uequiv = "ANY:c=1:i=1",
      .ucode = 0x100 | INTEL_X86_MOD_INV | (0x1 << INTEL_X86_CMASK_BIT),
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
   },
   {
      .uname  = "TOTAL_CYCLES",
      .udesc  = "Total cycles using precise uop retired event (Precise Event)",
      .uequiv = "ANY:c=16:i=1",
      .ucode = 0x100 | INTEL_X86_MOD_INV | (0x10 << INTEL_X86_CMASK_BIT),
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
   },
   {
      .uname  = "ACTIVE_CYCLES",
      .udesc  = "Alias for TOTAL_CYCLES (Precise Event)",
      .uequiv = "ANY:c=16:i=1",
      .ucode = 0x100 | INTEL_X86_MOD_INV | (0x10 << INTEL_X86_CMASK_BIT),
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
   },
};

static const intel_x86_umask_t wsm_br_misp_retired []=
{
   {
      .uname  = "ALL_BRANCHES",
      .udesc  = "Mispredicted retired branch instructions (Precise Event)",
      .ucode = 0x0,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
   },
   {
      .uname  = "NEAR_CALL",
      .udesc  = "Mispredicted near retired calls (Precise Event)",
      .ucode = 0x200,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
   },
   {
      .uname  = "CONDITIONAL",
      .udesc  = "Mispredicted conditional branches retired (Precise Event)",
      .ucode = 0x100,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
   },
};

static const intel_x86_umask_t wsm_ept []=
{
   {
      .uname  = "WALK_CYCLES",
      .udesc  = "Extended Page Table walk cycles",
      .ucode = 0x1000,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL,
   },
};

static const intel_x86_umask_t wsm_uops_executed []=
{
   {
      .uname  = "PORT0",
      .udesc  = "Uops executed on port 0 (integer arithmetic, SIMD and FP add uops)",
      .ucode = 0x100,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "PORT1",
      .udesc  = "Uops executed on port 1 (integer arithmetic, SIMD, integer shift, FP multiply, FP divide uops)",
      .ucode = 0x200,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "PORT2_CORE",
      .udesc  = "Uops executed on port 2 on any thread (load uops)(core count only)",
      .ucode = 0x400 | INTEL_X86_MOD_ANY,
      .modhw = _INTEL_X86_ATTR_T,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "PORT3_CORE",
      .udesc  = "Uops executed on port 3 on any thread (store uops)(core count only)",
      .ucode = 0x800 | INTEL_X86_MOD_ANY,
      .modhw = _INTEL_X86_ATTR_T,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "PORT4_CORE",
      .udesc  = "Uops executed on port 4 on any thread (handle store values for stores on port 3)(core count only)",
      .ucode = 0x1000 | INTEL_X86_MOD_ANY,
      .modhw = _INTEL_X86_ATTR_T,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "PORT5",
      .udesc  = "Uops executed on port 5",
      .ucode = 0x2000,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "PORT015",
      .udesc  = "Uops issued on ports 0, 1 or 5",
      .ucode = 0x4000,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "PORT234_CORE",
      .udesc  = "Uops issued on ports 2, 3 or 4 on any thread (core count only)",
      .ucode = 0x8000 | INTEL_X86_MOD_ANY,
      .modhw = _INTEL_X86_ATTR_T,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "PORT015_STALL_CYCLES",
      .udesc  = "Cycles no Uops issued on ports 0, 1 or 5",
      .uequiv = "PORT015:c=1:i=1",
      .ucode = 0x4000 | INTEL_X86_MOD_INV | (0x1 << INTEL_X86_CMASK_BIT),
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "CORE_ACTIVE_CYCLES_NO_PORT5",
      .udesc  = "Cycles in which uops are executed only on port0-4 on any thread (core count only)",
      .ucode = 0x1f00 | INTEL_X86_MOD_ANY | (0x1 << INTEL_X86_CMASK_BIT),
      .modhw = _INTEL_X86_ATTR_C | _INTEL_X86_ATTR_T,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "CORE_ACTIVE_CYCLES",
      .udesc  = "Cycles in which uops are executed on any port any thread (core count only)",
      .ucode = 0x3f00 | INTEL_X86_MOD_ANY | (0x1 << INTEL_X86_CMASK_BIT),
      .modhw = _INTEL_X86_ATTR_C | _INTEL_X86_ATTR_T,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "CORE_STALL_CYCLES",
      .udesc  = "Cycles in which no uops are executed on any port any thread (core count only)",
      .ucode = 0x3f00 | INTEL_X86_MOD_ANY | INTEL_X86_MOD_INV | (0x1 << INTEL_X86_CMASK_BIT),
      .modhw = _INTEL_X86_ATTR_I | _INTEL_X86_ATTR_C | _INTEL_X86_ATTR_T,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "CORE_STALL_CYCLES_NO_PORT5",
      .udesc  = "Cycles in which no uops are executed on any port0-4 on any thread (core count only)",
      .ucode = 0x1f00 | INTEL_X86_MOD_ANY | INTEL_X86_MOD_INV | (0x1 << INTEL_X86_CMASK_BIT),
      .modhw = _INTEL_X86_ATTR_I | _INTEL_X86_ATTR_C | _INTEL_X86_ATTR_T,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "CORE_STALL_COUNT",
      .udesc  = "Number of transitions from stalled to uops to execute on any port any thread (core count only)",
      .uequiv = "CORE_STALL_CYCLES:e=1",
      .ucode = 0x3f00 | INTEL_X86_MOD_EDGE | INTEL_X86_MOD_ANY | INTEL_X86_MOD_INV | (0x1 << INTEL_X86_CMASK_BIT),
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "CORE_STALL_COUNT_NO_PORT5",
      .udesc  = "Number of transitions from stalled to uops to execute on port0-4 on any thread (core count only)",
      .uequiv = "CORE_STALL_CYCLES_NO_PORT5:e=1",
      .ucode = 0x1f00 | INTEL_X86_MOD_EDGE | INTEL_X86_MOD_ANY | INTEL_X86_MOD_INV | (0x1 << INTEL_X86_CMASK_BIT),
      .uflags= INTEL_X86_NCOMBO,
   },
};

static const intel_x86_umask_t wsm_inst_retired []=
{
   {
      .uname  = "ANY_P",
      .udesc  = "Instructions Retired (Precise Event)",
      .ucode = 0x0,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS | INTEL_X86_DFL,
   },
   {
      .uname  = "X87",
      .udesc  = "Retired floating-point operations (Precise Event)",
      .ucode = 0x200,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
   },
   {
      .uname  = "MMX",
      .udesc  = "Retired MMX instructions (Precise Event)",
      .ucode = 0x400,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
   },
   {
      .uname  = "TOTAL_CYCLES",
      .udesc  = "Total cycles (Precise Event)",
      .uequiv = "ANY_P:c=16:i=1",
      .ucode = 0x100 | INTEL_X86_MOD_INV | (0x10 << INTEL_X86_CMASK_BIT),
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
   },
};

static const intel_x86_umask_t wsm_ild_stall []=
{
   {
      .uname  = "ANY",
      .udesc  = "Any Instruction Length Decoder stall cycles",
      .uequiv = "IQ_FULL:LCP:MRU:REGEN",
      .ucode = 0xf00,
      .uflags= INTEL_X86_DFL,
   },
   {
      .uname  = "IQ_FULL",
      .udesc  = "Instruction Queue full stall cycles",
      .ucode = 0x400,
   },
   {
      .uname  = "LCP",
      .udesc  = "Length Change Prefix stall cycles",
      .ucode = 0x100,
   },
   {
      .uname  = "MRU",
      .udesc  = "Stall cycles due to BPU MRU bypass",
      .ucode = 0x200,
   },
   {
      .uname  = "REGEN",
      .udesc  = "Regen stall cycles",
      .ucode = 0x800,
   },
};

static const intel_x86_umask_t wsm_dtlb_load_misses []=
{
   {
      .uname  = "ANY",
      .udesc  = "DTLB load misses",
      .ucode = 0x100,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL,
   },
   {
      .uname  = "PDE_MISS",
      .udesc  = "DTLB load miss caused by low part of address",
      .ucode = 0x2000,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "STLB_HIT",
      .udesc  = "DTLB second level hit",
      .ucode = 0x1000,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "WALK_COMPLETED",
      .udesc  = "DTLB load miss page walks complete",
      .ucode = 0x200,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "WALK_CYCLES",
      .udesc  = "DTLB load miss page walk cycles",
      .ucode = 0x400,
      .uflags= INTEL_X86_NCOMBO,
   },
};

static const intel_x86_umask_t wsm_l2_lines_in []=
{
   {
      .uname  = "ANY",
      .udesc  = "L2 lines alloacated",
      .ucode = 0x700,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL,
   },
   {
      .uname  = "E_STATE",
      .udesc  = "L2 lines allocated in the E state",
      .ucode = 0x400,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "S_STATE",
      .udesc  = "L2 lines allocated in the S state",
      .ucode = 0x200,
      .uflags= INTEL_X86_NCOMBO,
   },
};

static const intel_x86_umask_t wsm_ssex_uops_retired []=
{
   {
      .uname  = "PACKED_DOUBLE",
      .udesc  = "SIMD Packed-Double Uops retired (Precise Event)",
      .ucode = 0x400,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
   },
   {
      .uname  = "PACKED_SINGLE",
      .udesc  = "SIMD Packed-Single Uops retired (Precise Event)",
      .ucode = 0x100,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
   },
   {
      .uname  = "SCALAR_DOUBLE",
      .udesc  = "SIMD Scalar-Double Uops retired (Precise Event)",
      .ucode = 0x800,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
   },
   {
      .uname  = "SCALAR_SINGLE",
      .udesc  = "SIMD Scalar-Single Uops retired (Precise Event)",
      .ucode = 0x200,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
   },
   {
      .uname  = "VECTOR_INTEGER",
      .udesc  = "SIMD Vector Integer Uops retired (Precise Event)",
      .ucode = 0x1000,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
   },
};

static const intel_x86_umask_t wsm_store_blocks []=
{
   {
      .uname  = "AT_RET",
      .udesc  = "Loads delayed with at-Retirement block code",
      .ucode = 0x400,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "L1D_BLOCK",
      .udesc  = "Cacheable loads delayed with L1D block code",
      .ucode = 0x800,
      .uflags= INTEL_X86_NCOMBO,
   },
};

static const intel_x86_umask_t wsm_fp_mmx_trans []=
{
   {
      .uname  = "ANY",
      .udesc  = "All Floating Point to and from MMX transitions",
      .ucode = 0x300,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL,
   },
   {
      .uname  = "TO_FP",
      .udesc  = "Transitions from MMX to Floating Point instructions",
      .ucode = 0x100,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "TO_MMX",
      .udesc  = "Transitions from Floating Point to MMX instructions",
      .ucode = 0x200,
      .uflags= INTEL_X86_NCOMBO,
   },
};

static const intel_x86_umask_t wsm_cache_lock_cycles []=
{
   {
      .uname  = "L1D",
      .udesc  = "Cycles L1D locked",
      .ucode = 0x200,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "L1D_L2",
      .udesc  = "Cycles L1D and L2 locked",
      .ucode = 0x100,
      .uflags= INTEL_X86_NCOMBO,
   },
};

static const intel_x86_umask_t wsm_l3_lat_cache []=
{
   {
      .uname  = "MISS",
      .udesc  = "Last level cache miss",
      .ucode = 0x100,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "REFERENCE",
      .udesc  = "Last level cache reference",
      .ucode = 0x200,
      .uflags= INTEL_X86_NCOMBO,
   },
};

static const intel_x86_umask_t wsm_simd_int_64[]=
{
   {
      .uname  = "PACK",
      .udesc  = "SIMD integer 64 bit pack operations",
      .ucode = 0x400,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "PACKED_ARITH",
      .udesc  = "SIMD integer 64 bit arithmetic operations",
      .ucode = 0x2000,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "PACKED_LOGICAL",
      .udesc  = "SIMD integer 64 bit logical operations",
      .ucode = 0x1000,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "PACKED_MPY",
      .udesc  = "SIMD integer 64 bit packed multiply operations",
      .ucode = 0x100,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "PACKED_SHIFT",
      .udesc  = "SIMD integer 64 bit shift operations",
      .ucode = 0x200,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "SHUFFLE_MOVE",
      .udesc  = "SIMD integer 64 bit shuffle/move operations",
      .ucode = 0x4000,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "UNPACK",
      .udesc  = "SIMD integer 64 bit unpack operations",
      .ucode = 0x800,
      .uflags= INTEL_X86_NCOMBO,
   },
};

static const intel_x86_umask_t wsm_br_misp_exec []=
{
   {
      .uname  = "ANY",
      .udesc  = "Mispredicted branches executed",
      .ucode = 0x7f00,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "COND",
      .udesc  = "Mispredicted conditional branches executed",
      .ucode = 0x100,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "DIRECT",
      .udesc  = "Mispredicted unconditional branches executed",
      .ucode = 0x200,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "DIRECT_NEAR_CALL",
      .udesc  = "Mispredicted non call branches executed",
      .ucode = 0x1000,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "INDIRECT_NEAR_CALL",
      .udesc  = "Mispredicted indirect call branches executed",
      .ucode = 0x2000,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "INDIRECT_NON_CALL",
      .udesc  = "Mispredicted indirect non call branches executed",
      .ucode = 0x400,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "NEAR_CALLS",
      .udesc  = "Mispredicted call branches executed",
      .ucode = 0x3000,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "NON_CALLS",
      .udesc  = "Mispredicted non call branches executed",
      .ucode = 0x700,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "RETURN_NEAR",
      .udesc  = "Mispredicted return branches executed",
      .ucode = 0x800,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "TAKEN",
      .udesc  = "Mispredicted taken branches executed",
      .ucode = 0x4000,
      .uflags= INTEL_X86_NCOMBO,
   },
};

static const intel_x86_umask_t wsm_baclear []=
{
   {
      .uname  = "BAD_TARGET",
      .udesc  = "BACLEAR asserted with bad target address",
      .ucode = 0x200,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "CLEAR",
      .udesc  = "BACLEAR asserted, regardless of cause",
      .ucode = 0x100,
      .uflags= INTEL_X86_NCOMBO,
   },
};

static const intel_x86_umask_t wsm_dtlb_misses []=
{
   {
      .uname  = "ANY",
      .udesc  = "DTLB misses",
      .ucode = 0x100,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL,
   },
   {
      .uname  = "LARGE_WALK_COMPLETED",
      .udesc  = "DTLB miss large page walks",
      .ucode = 0x8000,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "STLB_HIT",
      .udesc  = "DTLB first level misses but second level hit",
      .ucode = 0x1000,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "WALK_COMPLETED",
      .udesc  = "DTLB miss page walks",
      .ucode = 0x200,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "WALK_CYCLES",
      .udesc  = "DTLB miss page walk cycles",
      .ucode = 0x400,
      .uflags= INTEL_X86_NCOMBO,
   },
};

static const intel_x86_umask_t wsm_mem_inst_retired []=
{
   {
      .uname  = "LATENCY_ABOVE_THRESHOLD",
      .udesc  = "Memory instructions retired above programmed clocks, minimum value threshold is 4, requires PEBS (Precise Event)",
      .ucode = 0x1000,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
   },
   {
      .uname  = "LOADS",
      .udesc  = "Instructions retired which contains a load (Precise Event)",
      .ucode = 0x100,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
   },
   {
      .uname  = "STORES",
      .udesc  = "Instructions retired which contains a store (Precise Event)",
      .ucode = 0x200,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
   },
};

static const intel_x86_umask_t wsm_uops_issued []=
{
   {
      .uname  = "ANY",
      .udesc  = "Uops issued",
      .ucode = 0x100,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL,
   },
   {
      .uname  = "STALL_CYCLES",
      .udesc  = "Cycles stalled no issued uops",
      .uequiv = "ANY:c=1:i=1",
      .ucode = 0x100 | INTEL_X86_MOD_INV | (0x1 << INTEL_X86_CMASK_BIT),
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "FUSED",
      .udesc  = "Fused Uops issued",
      .ucode = 0x200,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "CYCLES_ALL_THREADS",
      .udesc  = "Cycles uops issued on either threads (core count)",
      .uequiv = "ANY:c=1:t=1",
      .ucode = 0x100 | INTEL_X86_MOD_ANY | (0x1 << INTEL_X86_CMASK_BIT),
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "CORE_STALL_CYCLES",
      .udesc  = "Cycles no uops issued on any threads (core count)",
      .uequiv = "ANY:c=1:i=1:t=1",
      .ucode = 0x100 | INTEL_X86_MOD_ANY | INTEL_X86_MOD_INV | (0x1 << INTEL_X86_CMASK_BIT),
      .uflags= INTEL_X86_NCOMBO,
   },
};

static const intel_x86_umask_t wsm_l2_rqsts []=
{
   {
      .uname  = "IFETCH_HIT",
      .udesc  = "L2 instruction fetch hits",
      .ucode = 0x1000,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "IFETCH_MISS",
      .udesc  = "L2 instruction fetch misses",
      .ucode = 0x2000,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "IFETCHES",
      .udesc  = "L2 instruction fetches",
      .ucode = 0x3000,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "LD_HIT",
      .udesc  = "L2 load hits",
      .ucode = 0x100,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "LD_MISS",
      .udesc  = "L2 load misses",
      .ucode = 0x200,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "LOADS",
      .udesc  = "L2 requests",
      .ucode = 0x300,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "MISS",
      .udesc  = "All L2 misses",
      .ucode = 0xaa00,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "PREFETCH_HIT",
      .udesc  = "L2 prefetch hits",
      .ucode = 0x4000,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "PREFETCH_MISS",
      .udesc  = "L2 prefetch misses",
      .ucode = 0x8000,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "PREFETCHES",
      .udesc  = "All L2 prefetches",
      .ucode = 0xc000,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "REFERENCES",
      .udesc  = "All L2 requests",
      .ucode = 0xff00,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "RFO_HIT",
      .udesc  = "L2 RFO hits",
      .ucode = 0x400,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "RFO_MISS",
      .udesc  = "L2 RFO misses",
      .ucode = 0x800,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "RFOS",
      .udesc  = "L2 RFO requests",
      .ucode = 0xc00,
      .uflags= INTEL_X86_NCOMBO,
   },
};

static const intel_x86_umask_t wsm_load_dispatch []=
{
   {
      .uname  = "ANY",
      .udesc  = "All loads dispatched",
      .ucode = 0x700,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL,
   },
   {
      .uname  = "RS",
      .udesc  = "Number of loads dispatched from the Reservation Station (RS) that bypass the Memory Order Buffer",
      .ucode = 0x100,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "RS_DELAYED",
      .udesc  = "Number of delayed RS dispatches at the stage latch",
      .ucode = 0x200,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "MOB",
      .udesc  = "Number of loads dispatched from Reservation Station (RS)",
      .ucode = 0x400,
      .uflags= INTEL_X86_NCOMBO,
   },
};

static const intel_x86_umask_t wsm_snoopq_requests []=
{
   {
      .uname  = "CODE",
      .udesc  = "Snoop code requests",
      .ucode = 0x400,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "DATA",
      .udesc  = "Snoop data requests",
      .ucode = 0x100,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "INVALIDATE",
      .udesc  = "Snoop invalidate requests",
      .ucode = 0x200,
      .uflags= INTEL_X86_NCOMBO,
   },
};

static const intel_x86_umask_t wsm_offcore_requests []=
{
   {
      .uname  = "ANY",
      .udesc  = "All offcore requests",
      .ucode = 0x8000,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL,
   },
   {
      .uname  = "ANY_READ",
      .udesc  = "Offcore read requests",
      .ucode = 0x800,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "ANY_RFO",
      .udesc  = "Offcore RFO requests",
      .ucode = 0x1000,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "DEMAND_READ_CODE",
      .udesc  = "Offcore demand code read requests",
      .ucode = 0x200,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "DEMAND_READ_DATA",
      .udesc  = "Offcore demand data read requests",
      .ucode = 0x100,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "DEMAND_RFO",
      .udesc  = "Offcore demand RFO requests",
      .ucode = 0x400,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "L1D_WRITEBACK",
      .udesc  = "Offcore L1 data cache writebacks",
      .ucode = 0x4000,
      .uflags= INTEL_X86_NCOMBO,
   },
};

static const intel_x86_umask_t wsm_load_block []=
{
   {
      .uname  = "OVERLAP_STORE",
      .udesc  = "Loads that partially overlap an earlier store",
      .ucode = 0x200,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL,
   },
};

static const intel_x86_umask_t wsm_misalign_memory []=
{
   {
      .uname  = "STORE",
      .udesc  = "Store referenced with misaligned address",
      .ucode = 0x200,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL,
   },
};

static const intel_x86_umask_t wsm_machine_clears []=
{
   {
      .uname  = "MEM_ORDER",
      .udesc  = "Execution pipeline restart due to Memory ordering conflicts ",
      .ucode = 0x200,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "CYCLES",
      .udesc  = "Cycles machine clear is asserted",
      .ucode = 0x100,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "SMC",
      .udesc  = "Self-modifying code detected",
      .ucode = 0x400,
      .uflags= INTEL_X86_NCOMBO,
   },
};

static const intel_x86_umask_t wsm_fp_comp_ops_exe []=
{
   {
      .uname  = "MMX",
      .udesc  = "MMX Uops",
      .ucode = 0x200,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "SSE_DOUBLE_PRECISION",
      .udesc  = "SSE FP double precision Uops",
      .ucode = 0x8000,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "SSE_FP",
      .udesc  = "SSE and SSE2 FP Uops",
      .ucode = 0x400,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "SSE_FP_PACKED",
      .udesc  = "SSE FP packed Uops",
      .ucode = 0x1000,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "SSE_FP_SCALAR",
      .udesc  = "SSE FP scalar Uops",
      .ucode = 0x2000,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "SSE_SINGLE_PRECISION",
      .udesc  = "SSE FP single precision Uops",
      .ucode = 0x4000,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "SSE2_INTEGER",
      .udesc  = "SSE2 integer Uops",
      .ucode = 0x800,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "X87",
      .udesc  = "Computational floating-point operations executed",
      .ucode = 0x100,
      .uflags= INTEL_X86_NCOMBO,
   },
};

static const intel_x86_umask_t wsm_br_inst_retired []=
{
   {
      .uname  = "ALL_BRANCHES",
      .udesc  = "Retired branch instructions (Precise Event)",
      .ucode = 0x0,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS | INTEL_X86_DFL,
   },
   {
      .uname  = "CONDITIONAL",
      .udesc  = "Retired conditional branch instructions (Precise Event)",
      .ucode = 0x100,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
   },
   {
      .uname  = "NEAR_CALL",
      .udesc  = "Retired near call instructions (Precise Event)",
      .ucode = 0x200,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
   },
};

static const intel_x86_umask_t wsm_large_itlb []=
{
   {
      .uname  = "HIT",
      .udesc  = "Large ITLB hit",
      .ucode = 0x100,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL,
   },
};

static const intel_x86_umask_t wsm_lsd []=
{
   {
      .uname  = "UOPS",
      .udesc  = "Counts the number of micro-ops delivered by LSD",
      .ucode = 0x100,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "ACTIVE",
      .udesc  = "Cycles is which at least one micro-op delivered by LSD",
      .uequiv = "UOPS:c=1",
      .ucode = 0x100 | (0x1 << INTEL_X86_CMASK_BIT),
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "INACTIVE",
      .udesc  = "Cycles is which no micro-op is delivered by LSD",
      .uequiv = "UOPS:c=1:i=1",
      .ucode = 0x100 | INTEL_X86_MOD_INV | (0x1 << INTEL_X86_CMASK_BIT),
      .uflags= INTEL_X86_NCOMBO,
   },
};

static const intel_x86_umask_t wsm_l2_lines_out []=
{
   {
      .uname  = "ANY",
      .udesc  = "L2 lines evicted",
      .ucode = 0xf00,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL,
   },
   {
      .uname  = "DEMAND_CLEAN",
      .udesc  = "L2 lines evicted by a demand request",
      .ucode = 0x100,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "DEMAND_DIRTY",
      .udesc  = "L2 modified lines evicted by a demand request",
      .ucode = 0x200,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "PREFETCH_CLEAN",
      .udesc  = "L2 lines evicted by a prefetch request",
      .ucode = 0x400,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "PREFETCH_DIRTY",
      .udesc  = "L2 modified lines evicted by a prefetch request",
      .ucode = 0x800,
      .uflags= INTEL_X86_NCOMBO,
   },
};

static const intel_x86_umask_t wsm_itlb_misses []=
{
   {
      .uname  = "ANY",
      .udesc  = "ITLB miss",
      .ucode = 0x100,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL,
   },
   {
      .uname  = "WALK_COMPLETED",
      .udesc  = "ITLB miss page walks",
      .ucode = 0x200,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "WALK_CYCLES",
      .udesc  = "ITLB miss page walk cycles",
      .ucode = 0x400,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "LARGE_WALK_COMPLETED",
      .udesc  = "Number of completed large page walks due to misses in the STLB",
      .ucode = 0x8000,
      .uflags= INTEL_X86_NCOMBO,
   },
};

static const intel_x86_umask_t wsm_l1d_prefetch []=
{
   {
      .uname  = "MISS",
      .udesc  = "L1D hardware prefetch misses",
      .ucode = 0x200,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "REQUESTS",
      .udesc  = "L1D hardware prefetch requests",
      .ucode = 0x100,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "TRIGGERS",
      .udesc  = "L1D hardware prefetch requests triggered",
      .ucode = 0x400,
      .uflags= INTEL_X86_NCOMBO,
   },
};

static const intel_x86_umask_t wsm_sq_misc []=
{
   {
      .uname  = "LRU_HINTS",
      .udesc  = "Super Queue LRU hints sent to LLC",
      .ucode = 0x400,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "SPLIT_LOCK",
      .udesc  = "Super Queue lock splits across a cache line",
      .ucode = 0x1000,
      .uflags= INTEL_X86_NCOMBO,
   },
};

static const intel_x86_umask_t wsm_fp_assist []=
{
   {
      .uname  = "ALL",
      .udesc  = "All X87 Floating point assists (Precise Event)",
      .ucode = 0x100,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS | INTEL_X86_DFL,
   },
   {
      .uname  = "INPUT",
      .udesc  = "X87 Floating point assists for invalid input value (Precise Event)",
      .ucode = 0x400,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
   },
   {
      .uname  = "OUTPUT",
      .udesc  = "X87 Floating point assists for invalid output value (Precise Event)",
      .ucode = 0x200,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
   },
};

static const intel_x86_umask_t wsm_simd_int_128[]=
{
   {
      .uname  = "PACK",
      .udesc  = "128 bit SIMD integer pack operations",
      .ucode = 0x400,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "PACKED_ARITH",
      .udesc  = "128 bit SIMD integer arithmetic operations",
      .ucode = 0x2000,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "PACKED_LOGICAL",
      .udesc  = "128 bit SIMD integer logical operations",
      .ucode = 0x1000,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "PACKED_MPY",
      .udesc  = "128 bit SIMD integer multiply operations",
      .ucode = 0x100,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "PACKED_SHIFT",
      .udesc  = "128 bit SIMD integer shift operations",
      .ucode = 0x200,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "SHUFFLE_MOVE",
      .udesc  = "128 bit SIMD integer shuffle/move operations",
      .ucode = 0x4000,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "UNPACK",
      .udesc  = "128 bit SIMD integer unpack operations",
      .ucode = 0x800,
      .uflags= INTEL_X86_NCOMBO,
   },
};

static const intel_x86_umask_t wsm_offcore_requests_outstanding []=
{
   {
      .uname  = "ANY_READ",
      .udesc  = "Outstanding offcore reads",
      .ucode = 0x800,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "DEMAND_READ_CODE",
      .udesc  = "Outstanding offcore demand code reads",
      .ucode = 0x200,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "DEMAND_READ_DATA",
      .udesc  = "Outstanding offcore demand data reads",
      .ucode = 0x100,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "DEMAND_RFO",
      .udesc  = "Outstanding offcore demand RFOs",
      .ucode = 0x400,
      .uflags= INTEL_X86_NCOMBO,
   },
};

static const intel_x86_umask_t wsm_mem_store_retired []=
{
   {
      .uname  = "DTLB_MISS",
      .udesc  = "Retired stores that miss the DTLB (Precise Event)",
      .ucode = 0x100,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS | INTEL_X86_DFL,
   },
};

static const intel_x86_umask_t wsm_inst_decoded []=
{
   {
      .uname  = "DEC0",
      .udesc  = "Instructions that must be decoded by decoder 0",
      .ucode = 0x100,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL,
   },
};

static const intel_x86_umask_t wsm_macro_insts []=
{
   {
      .uname  = "DECODED",
      .udesc  = "Instructions decoded",
      .ucode = 0x100,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL,
   },
};

static const intel_x86_umask_t wsm_arith []=
{
   {
      .uname  = "CYCLES_DIV_BUSY",
      .udesc  = "Counts the number of cycles the divider is busy executing divide or square root operations. The divide can be integer, X87 or Streaming SIMD Extensions (SSE). The square root operation can be either X87 or SSE. Count may be incorrect when HT is on",
      .ucode = 0x100,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "DIV",
      .udesc  = "Counts the number of divide or square root operations. The divide can be integer, X87 or Streaming SIMD Extensions (SSE). The square root operation can be either X87 or SSE. Count may be incorrect when HT is on",
      .uequiv = "CYCLES_DIV_BUSY:c=1:i=1:e=1",
      .ucode = 0x100 | INTEL_X86_MOD_EDGE | INTEL_X86_MOD_INV | (0x1 << INTEL_X86_CMASK_BIT),
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "MUL",
      .udesc  = "Counts the number of multiply operations executed. This includes integer as well as floating point multiply operations but excludes DPPS mul and MPSAD. Count may be incorrect when HT is on",
      .ucode = 0x200,
      .uflags= INTEL_X86_NCOMBO,
   },
};

static const intel_x86_umask_t wsm_l2_transactions []=
{
   {
      .uname  = "ANY",
      .udesc  = "All L2 transactions",
      .ucode = 0x8000,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL,
   },
   {
      .uname  = "FILL",
      .udesc  = "L2 fill transactions",
      .ucode = 0x2000,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "IFETCH",
      .udesc  = "L2 instruction fetch transactions",
      .ucode = 0x400,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "L1D_WB",
      .udesc  = "L1D writeback to L2 transactions",
      .ucode = 0x1000,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "LOAD",
      .udesc  = "L2 Load transactions",
      .ucode = 0x100,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "PREFETCH",
      .udesc  = "L2 prefetch transactions",
      .ucode = 0x800,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "RFO",
      .udesc  = "L2 RFO transactions",
      .ucode = 0x200,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "WB",
      .udesc  = "L2 writeback to LLC transactions",
      .ucode = 0x4000,
      .uflags= INTEL_X86_NCOMBO,
   },
};

static const intel_x86_umask_t wsm_sb_drain []=
{
   {
      .uname  = "ANY",
      .udesc  = "All Store buffer stall cycles",
      .ucode = 0x700,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL,
   },
};

static const intel_x86_umask_t wsm_mem_uncore_retired []=
{
   {
      .uname  = "LOCAL_HITM",
      .udesc  = "Load instructions retired that HIT modified data in sibling core (Precise Event)",
      .ucode = 0x200,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
      .umodel = PFM_PMU_INTEL_WSM_DP,
   },
   {
      .uname  = "LOCAL_DRAM_AND_REMOTE_CACHE_HIT",
      .udesc  = "Load instructions retired local dram and remote cache HIT data sources (Precise Event)",
      .ucode = 0x800,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
      .umodel = PFM_PMU_INTEL_WSM_DP,
   },
   {
      .uname  = "REMOTE_DRAM",
      .udesc  = "Load instructions retired remote DRAM and remote home-remote cache HITM (Precise Event)",
      .ucode = 0x1000,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
      .umodel = PFM_PMU_INTEL_WSM_DP,
   },
   {
      .uname  = "UNCACHEABLE",
      .udesc  = "Load instructions retired IO (Precise Event)",
      .ucode = 0x8000,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
   },
   {
      .uname  = "REMOTE_HITM",
      .udesc  = "Retired loads that hit remote socket in modified state (Precise Event)",
      .ucode = 0x400,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
      .umodel = PFM_PMU_INTEL_WSM_DP,
   },
   {
      .uname  = "OTHER_LLC_MISS",
      .udesc  = "Load instructions retired other LLC miss (Precise Event)",
      .ucode = 0x2000,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
      .umodel = PFM_PMU_INTEL_WSM_DP,
   },
   {
      .uname  = "UNKNOWN_SOURCE",
      .udesc  = "Load instructions retired unknown LLC miss (Precise Event)",
      .ucode = 0x100,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
      .umodel = PFM_PMU_INTEL_WSM_DP,
   },
   {
      .uname  = "LOCAL_DRAM",
      .udesc  = "Retired loads with a data source of local DRAM or locally homed remote cache HITM (Precise Event)",
      .ucode = 0x1000,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
      .umodel = PFM_PMU_INTEL_WSM,
   },
   {
      .uname  = "OTHER_CORE_L2_HITM",
      .udesc  = "Retired loads instruction that hit modified data in sibling core (Precise Event)",
      .ucode = 0x200,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
      .umodel = PFM_PMU_INTEL_WSM,
   },
   {
      .uname  = "REMOTE_CACHE_LOCAL_HOME_HIT",
      .udesc  = "Retired loads instruction that hit remote cache hit data source (Precise Event)",
      .ucode = 0x800,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
      .umodel = PFM_PMU_INTEL_WSM,
   },
   {
      .uname  = "REMOTE_DRAM",
      .udesc  = "Retired loads instruction remote DRAM and remote home-remote cache HITM (Precise Event)",
      .ucode = 0x2000,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
      .umodel = PFM_PMU_INTEL_WSM,
   },
};

static const intel_x86_umask_t wsm_l2_data_rqsts []=
{
   {
      .uname  = "ANY",
      .udesc  = "All L2 data requests",
      .ucode = 0xff00,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL,
   },
   {
      .uname  = "DEMAND_E_STATE",
      .udesc  = "L2 data demand loads in E state",
      .ucode = 0x400,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "DEMAND_I_STATE",
      .udesc  = "L2 data demand loads in I state (misses)",
      .ucode = 0x100,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "DEMAND_M_STATE",
      .udesc  = "L2 data demand loads in M state",
      .ucode = 0x800,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "DEMAND_MESI",
      .udesc  = "L2 data demand requests",
      .ucode = 0xf00,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "DEMAND_S_STATE",
      .udesc  = "L2 data demand loads in S state",
      .ucode = 0x200,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "PREFETCH_E_STATE",
      .udesc  = "L2 data prefetches in E state",
      .ucode = 0x4000,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "PREFETCH_I_STATE",
      .udesc  = "L2 data prefetches in the I state (misses)",
      .ucode = 0x1000,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "PREFETCH_M_STATE",
      .udesc  = "L2 data prefetches in M state",
      .ucode = 0x8000,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "PREFETCH_MESI",
      .udesc  = "All L2 data prefetches",
      .ucode = 0xf000,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "PREFETCH_S_STATE",
      .udesc  = "L2 data prefetches in the S state",
      .ucode = 0x2000,
      .uflags= INTEL_X86_NCOMBO,
   },
};

static const intel_x86_umask_t wsm_br_inst_exec []=
{
   {
      .uname  = "ANY",
      .udesc  = "Branch instructions executed",
      .ucode = 0x7f00,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL,
   },
   {
      .uname  = "COND",
      .udesc  = "Conditional branch instructions executed",
      .ucode = 0x100,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "DIRECT",
      .udesc  = "Unconditional branches executed",
      .ucode = 0x200,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "DIRECT_NEAR_CALL",
      .udesc  = "Unconditional call branches executed",
      .ucode = 0x1000,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "INDIRECT_NEAR_CALL",
      .udesc  = "Indirect call branches executed",
      .ucode = 0x2000,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "INDIRECT_NON_CALL",
      .udesc  = "Indirect non call branches executed",
      .ucode = 0x400,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "NEAR_CALLS",
      .udesc  = "Call branches executed",
      .ucode = 0x3000,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "NON_CALLS",
      .udesc  = "All non call branches executed",
      .ucode = 0x700,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "RETURN_NEAR",
      .udesc  = "Indirect return branches executed",
      .ucode = 0x800,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "TAKEN",
      .udesc  = "Taken branches executed",
      .ucode = 0x4000,
      .uflags= INTEL_X86_NCOMBO,
   },
};

static const intel_x86_umask_t wsm_snoopq_requests_outstanding []=
{
   {
      .uname  = "CODE",
      .udesc  = "Outstanding snoop code requests",
      .ucode = 0x400,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "CODE_NOT_EMPTY",
      .udesc  = "Cycles snoop code requests queue not empty",
      .uequiv = "CODE:c=1",
      .ucode = 0x400 | (0x1 << INTEL_X86_CMASK_BIT),
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "DATA",
      .udesc  = "Outstanding snoop data requests",
      .ucode = 0x100,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "DATA_NOT_EMPTY",
      .udesc  = "Cycles snoop data requests queue not empty",
      .uequiv = "DATA:c=1",
      .ucode = 0x100 | (0x1 << INTEL_X86_CMASK_BIT),
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "INVALIDATE",
      .udesc  = "Outstanding snoop invalidate requests",
      .ucode = 0x200,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "INVALIDATE_NOT_EMPTY",
      .udesc  = "Cycles snoop invalidate requests queue not empty",
      .uequiv = "INVALIDATE:c=1",
      .ucode = 0x200 | (0x1 << INTEL_X86_CMASK_BIT),
      .uflags= INTEL_X86_NCOMBO,
   },
};

static const intel_x86_umask_t wsm_mem_load_retired []=
{
   {
      .uname  = "DTLB_MISS",
      .udesc  = "Retired loads that miss the DTLB (Precise Event)",
      .ucode = 0x8000,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
   },
   {
      .uname  = "HIT_LFB",
      .udesc  = "Retired loads that miss L1D and hit an previously allocated LFB (Precise Event)",
      .ucode = 0x4000,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
   },
   {
      .uname  = "L1D_HIT",
      .udesc  = "Retired loads that hit the L1 data cache (Precise Event)",
      .ucode = 0x100,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
   },
   {
      .uname  = "L2_HIT",
      .udesc  = "Retired loads that hit the L2 cache (Precise Event)",
      .ucode = 0x200,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
   },
   {
      .uname  = "L3_MISS",
      .udesc  = "Retired loads that miss the LLC cache (Precise Event)",
      .ucode = 0x1000,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
   },
   {
      .uname  = "LLC_MISS",
      .udesc  = "This is an alias for L3_MISS",
      .uequiv = "L3_MISS",
      .ucode = 0x1000,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
   },
   {
      .uname  = "L3_UNSHARED_HIT",
      .udesc  = "Retired loads that hit valid versions in the LLC cache (Precise Event)",
      .ucode = 0x400,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
   },
   {
      .uname  = "LLC_UNSHARED_HIT",
      .udesc  = "This is an alias for L3_UNSHARED_HIT",
      .uequiv = "L3_UNSHARED_HIT",
      .ucode = 0x400,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
   },
   {
      .uname  = "OTHER_CORE_L2_HIT_HITM",
      .udesc  = "Retired loads that hit sibling core's L2 in modified or unmodified states (Precise Event)",
      .ucode = 0x800,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
   },
};

static const intel_x86_umask_t wsm_l1i []=
{
   {
      .uname  = "CYCLES_STALLED",
      .udesc  = "L1I instruction fetch stall cycles",
      .ucode = 0x400,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "HITS",
      .udesc  = "L1I instruction fetch hits",
      .ucode = 0x100,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "MISSES",
      .udesc  = "L1I instruction fetch misses",
      .ucode = 0x200,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "READS",
      .udesc  = "L1I Instruction fetches",
      .ucode = 0x300,
      .uflags= INTEL_X86_NCOMBO,
   },
};

static const intel_x86_umask_t wsm_l2_write []=
{
   {
      .uname  = "LOCK_E_STATE",
      .udesc  = "L2 demand lock RFOs in E state",
      .ucode = 0x4000,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "LOCK_HIT",
      .udesc  = "All demand L2 lock RFOs that hit the cache",
      .ucode = 0xe000,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "LOCK_I_STATE",
      .udesc  = "L2 demand lock RFOs in I state (misses)",
      .ucode = 0x1000,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "LOCK_M_STATE",
      .udesc  = "L2 demand lock RFOs in M state",
      .ucode = 0x8000,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "LOCK_MESI",
      .udesc  = "All demand L2 lock RFOs",
      .ucode = 0xf000,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "LOCK_S_STATE",
      .udesc  = "L2 demand lock RFOs in S state",
      .ucode = 0x2000,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "RFO_HIT",
      .udesc  = "All L2 demand store RFOs that hit the cache",
      .ucode = 0xe00,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "RFO_I_STATE",
      .udesc  = "L2 demand store RFOs in I state (misses)",
      .ucode = 0x100,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "RFO_M_STATE",
      .udesc  = "L2 demand store RFOs in M state",
      .ucode = 0x800,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "RFO_MESI",
      .udesc  = "All L2 demand store RFOs",
      .ucode = 0xf00,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "RFO_S_STATE",
      .udesc  = "L2 demand store RFOs in S state",
      .ucode = 0x200,
      .uflags= INTEL_X86_NCOMBO,
   },
};

static const intel_x86_umask_t wsm_snoop_response []=
{
   {
      .uname  = "HIT",
      .udesc  = "Thread responded HIT to snoop",
      .ucode = 0x100,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "HITE",
      .udesc  = "Thread responded HITE to snoop",
      .ucode = 0x200,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "HITM",
      .udesc  = "Thread responded HITM to snoop",
      .ucode = 0x400,
      .uflags= INTEL_X86_NCOMBO,
   },
};

static const intel_x86_umask_t wsm_l1d []=
{
   {
      .uname  = "M_EVICT",
      .udesc  = "L1D cache lines replaced in M state ",
      .ucode = 0x400,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "M_REPL",
      .udesc  = "L1D cache lines allocated in the M state",
      .ucode = 0x200,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "M_SNOOP_EVICT",
      .udesc  = "L1D snoop eviction of cache lines in M state",
      .ucode = 0x800,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "REPL",
      .udesc  = "L1 data cache lines allocated",
      .ucode = 0x100,
      .uflags= INTEL_X86_NCOMBO,
   },
};

static const intel_x86_umask_t wsm_resource_stalls []=
{
   {
      .uname  = "ANY",
      .udesc  = "Resource related stall cycles",
      .ucode = 0x100,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL,
   },
   {
      .uname  = "FPCW",
      .udesc  = "FPU control word write stall cycles",
      .ucode = 0x2000,
   },
   {
      .uname  = "LOAD",
      .udesc  = "Load buffer stall cycles",
      .ucode = 0x200,
   },
   {
      .uname  = "MXCSR",
      .udesc  = "MXCSR rename stall cycles",
      .ucode = 0x4000,
   },
   {
      .uname  = "OTHER",
      .udesc  = "Other Resource related stall cycles",
      .ucode = 0x8000,
   },
   {
      .uname  = "ROB_FULL",
      .udesc  = "ROB full stall cycles",
      .ucode = 0x1000,
   },
   {
      .uname  = "RS_FULL",
      .udesc  = "Reservation Station full stall cycles",
      .ucode = 0x400,
   },
   {
      .uname  = "STORE",
      .udesc  = "Store buffer stall cycles",
      .ucode = 0x800,
   },
};

static const intel_x86_umask_t wsm_rat_stalls []=
{
   {
      .uname  = "ANY",
      .udesc  = "All RAT stall cycles",
      .uequiv = "FLAGS:REGISTERS:ROB_READ_PORT:SCOREBOARD",
      .ucode = 0xf00,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL,
   },
   {
      .uname  = "FLAGS",
      .udesc  = "Flag stall cycles",
      .ucode = 0x100,
   },
   {
      .uname  = "REGISTERS",
      .udesc  = "Partial register stall cycles",
      .ucode = 0x200,
   },
   {
      .uname  = "ROB_READ_PORT",
      .udesc  = "ROB read port stalls cycles",
      .ucode = 0x400,
   },
   {
      .uname  = "SCOREBOARD",
      .udesc  = "Scoreboard stall cycles",
      .ucode = 0x800,
   },
};

static const intel_x86_umask_t wsm_cpu_clk_unhalted []=
{
   {
      .uname  = "THREAD_P",
      .udesc  = "Cycles when thread is not halted (programmable counter)",
      .ucode = 0x0,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "REF_P",
      .udesc  = "Reference base clock (133 Mhz) cycles when thread is not halted",
      .ucode = 0x100,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "TOTAL_CYCLES",
      .udesc  = "Total number of elapsed cycles. Does not work when C-state enabled",
      .uequiv = "THREAD_P:c=2:i=1",
      .ucode = 0x0 | INTEL_X86_MOD_INV | (0x2 << INTEL_X86_CMASK_BIT),
      .uflags= INTEL_X86_NCOMBO,
   },
};

static const intel_x86_umask_t wsm_l1d_wb_l2[]=
{
   {
      .uname  = "E_STATE",
      .udesc  = "L1 writebacks to L2 in E state",
      .ucode = 0x400,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "I_STATE",
      .udesc  = "L1 writebacks to L2 in I state (misses)",
      .ucode = 0x100,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "M_STATE",
      .udesc  = "L1 writebacks to L2 in M state",
      .ucode = 0x800,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "MESI",
      .udesc  = "All L1 writebacks to L2",
      .ucode = 0xf00,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "S_STATE",
      .udesc  = "L1 writebacks to L2 in S state",
      .ucode = 0x200,
      .uflags= INTEL_X86_NCOMBO,
   },
};

static const intel_x86_umask_t wsm_offcore_response_0[]=
{
   {
      .uname  = "DMND_DATA_RD",
      .udesc  = "Request: counts the number of demand and DCU prefetch data reads of full and partial cachelines as well as demand data page table entry cacheline reads. Does not count L2 data read prefetches or instruction fetches",
      .ucode = 0x100,
      .grpid = 0,
   },
   {
      .uname  = "DMND_RFO",
      .udesc  = "Request: counts the number of demand and DCU prefetch reads for ownership (RFO) requests generated by a write to data cacheline. Does not count L2 RFO",
      .ucode = 0x200,
      .grpid = 0,
   },
   {
      .uname  = "DMND_IFETCH",
      .udesc  = "Request: counts the number of demand and DCU prefetch instruction cacheline reads. Does not count L2 code read prefetches",
      .ucode = 0x400,
      .grpid = 0,
   },
   {
      .uname  = "WB",
      .udesc  = "Request: counts the number of writeback (modified to exclusive) transactions",
      .ucode = 0x800,
      .grpid = 0,
   },
   {
      .uname  = "PF_DATA_RD",
      .udesc  = "Request: counts the number of data cacheline reads generated by L2 prefetchers",
      .ucode = 0x1000,
      .grpid = 0,
   },
   {
      .uname  = "PF_RFO",
      .udesc  = "Request: counts the number of RFO requests generated by L2 prefetchers",
      .ucode = 0x2000,
      .grpid = 0,
   },
   {
      .uname  = "PF_IFETCH",
      .udesc  = "Request: counts the number of code reads generated by L2 prefetchers",
      .ucode = 0x4000,
      .grpid = 0,
   },
   {
      .uname  = "OTHER",
      .udesc  = "Request: counts one of the following transaction types, including L3 invalidate, I/O, full or partial writes, WC or non-temporal stores, CLFLUSH, Fences, lock, unlock, split lock",
      .ucode = 0x8000,
      .grpid = 0,
   },
   {
      .uname  = "ANY_IFETCH",
      .udesc  = "Request: combination of PF_IFETCH | DMND_IFETCH",
      .uequiv = "PF_IFETCH:DMND_IFETCH",
      .ucode = 0x4400,
      .grpid = 0,
   },
   {
      .uname  = "ANY_REQUEST",
      .udesc  = "Request: combination of all requests umasks",
      .uequiv = "DMND_DATA_RD:DMND_RFO:DMND_IFETCH:WB:PF_DATA_RD:PF_RFO:PF_IFETCH:OTHER",
      .ucode = 0xff00,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL,
      .grpid = 0,
   },
   {
      .uname  = "ANY_DATA",
      .udesc  = "Request: any data read/write request",
      .uequiv = "DMND_DATA_RD:PF_DATA_RD:DMND_RFO:PF_RFO",
      .ucode = 0x3300,
      .grpid = 0,
   },
   {
      .uname  = "ANY_DATA_RD",
      .udesc  = "Request: any data read in request",
      .uequiv = "DMND_DATA_RD:PF_DATA_RD",
      .ucode = 0x1100,
      .grpid = 0,
   },
   {
      .uname  = "ANY_RFO",
      .udesc  = "Request: combination of DMND_RFO | PF_RFO",
      .uequiv = "DMND_RFO:PF_RFO",
      .ucode = 0x2200,
      .grpid = 0,
   },

   {
      .uname  = "UNCORE_HIT",
      .udesc  = "Response: counts L3 Hit: local or remote home requests that hit L3 cache in the uncore with no coherency actions required (snooping)",
      .ucode = 0x10000,
      .grpid = 1,
   },
   {
      .uname  = "OTHER_CORE_HIT_SNP",
      .udesc  = "Response: counts L3 Hit: local or remote home requests that hit L3 cache in the uncore and was serviced by another core with a cross core snoop where no modified copies were found (clean)",
      .ucode = 0x20000,
      .grpid = 1,
   },
   {
      .uname  = "OTHER_CORE_HITM",
      .udesc  = "Response: counts L3 Hit: local or remote home requests that hit L3 cache in the uncore and was serviced by another core with a cross core snoop where modified copies were found (HITM)",
      .ucode = 0x40000,
      .grpid = 1,
   },
   {
      .uname  = "REMOTE_CACHE_HITM",
      .udesc  = "Response: counts L3 Hit: local or remote home requests that hit a remote L3 cacheline in modified (HITM) state",
      .ucode = 0x80000,
      .grpid = 1,
   },

   {
      .uname  = "REMOTE_CACHE_FWD",
      .udesc  = "Response: counts L3 Miss: local homed requests that missed the L3 cache and was serviced by forwarded data following a cross package snoop where no modified copies found. (Remote home requests are not counted)",
      .ucode = 0x100000,
      .grpid = 1,
      .umodel = PFM_PMU_INTEL_WSM,
   },
   {
      .uname  = "LOCAL_DRAM_AND_REMOTE_CACHE_HIT",
      .udesc  = "Response: counts L3 Miss: local home requests that missed the L3 cache and were serviced by local DRAM or a remote cache",
      .ucode = 0x100000,
      .grpid = 1,
      .umodel = PFM_PMU_INTEL_WSM_DP,
   },

   {
      .uname  = "REMOTE_DRAM",
      .udesc  = "Response: counts L3 Miss: remote home requests that missed the L3 cache and were serviced by remote DRAM",
      .ucode = 0x200000,
      .grpid = 1,
      .umodel = PFM_PMU_INTEL_WSM_DP,
   },
   {
      .uname  = "LOCAL_DRAM",
      .udesc  = "Response: counts L3 Miss: local home requests that missed the L3 cache and were serviced by local DRAM",
      .ucode = 0x200000,
      .grpid = 1,
      .umodel = PFM_PMU_INTEL_WSM,
   },

   {
      .uname  = "REMOTE_DRAM",
      .udesc  = "Response: counts L3 Miss: remote home requests that missed the L3 cache and were serviced by remote DRAM",
      .ucode = 0x400000,
      .grpid = 1,
      .umodel = PFM_PMU_INTEL_WSM,
   },
   {
      .uname  = "OTHER_LLC_MISS",
      .udesc  = "Response: counts L3 Miss: remote home requests that missed the L3 cache",
      .ucode = 0x400000,
      .grpid = 1,
      .umodel = PFM_PMU_INTEL_WSM_DP,
   },

   {
      .uname  = "NON_DRAM",
      .udesc  = "Response: Non-DRAM requests that were serviced by IOH",
      .ucode = 0x800000,
      .grpid = 1,
   },

   {
      .uname  = "ANY_CACHE_DRAM",
      .udesc  = "Response: requests serviced by any source but IOH",
      .uequiv = "UNCORE_HIT:OTHER_CORE_HIT_SNP:OTHER_CORE_HITM:REMOTE_CACHE_FWD:REMOTE_CACHE_HITM:REMOTE_DRAM:LOCAL_DRAM",
      .ucode  = 0x7f0000,
      .grpid  = 1,
      .umodel = PFM_PMU_INTEL_WSM,
   },
   {
      .uname  = "ANY_CACHE_DRAM",
      .udesc  = "Response: requests serviced by any source but IOH",
      .uequiv = "UNCORE_HIT:OTHER_CORE_HIT_SNP:OTHER_CORE_HITM:REMOTE_CACHE_HITM:OTHER_LLC_MISS:REMOTE_DRAM:LOCAL_DRAM_AND_REMOTE_CACHE_HIT",
      .ucode  = 0x7f0000,
      .grpid  = 1,
      .umodel = PFM_PMU_INTEL_WSM_DP,
   },

   {
      .uname  = "ANY_DRAM",
      .udesc  = "Response: requests serviced by local or remote DRAM",
      .uequiv = "REMOTE_DRAM:LOCAL_DRAM",
      .ucode  = 0x600000,
      .umodel = PFM_PMU_INTEL_WSM,
      .grpid  = 1,
   },
   {
      .uname  = "ANY_LLC_MISS",
      .udesc  = "Response: requests that missed in L3",
      .uequiv = "REMOTE_CACHE_HITM:REMOTE_CACHE_FWD:REMOTE_DRAM:LOCAL_DRAM:NON_DRAM",
      .ucode  = 0xf80000,
      .grpid  = 1,
      .umodel = PFM_PMU_INTEL_WSM,
   },
   {
      .uname  = "ANY_LLC_MISS",
      .udesc  = "Response: requests that missed in L3",
      .uequiv = "REMOTE_CACHE_HITM:REMOTE_DRAM:OTHER_LLC_MISS:LOCAL_DRAM_AND_REMOTE_CACHE_HIT:NON_DRAM",
      .ucode  = 0xf80000,
      .grpid  = 1,
      .umodel = PFM_PMU_INTEL_WSM_DP,
   },

   {
      .uname  = "LOCAL_CACHE_DRAM",
      .udesc  = "Response: requests hit local core or uncore caches or local DRAM",
      .uequiv = "UNCORE_HIT:OTHER_CORE_HIT_SNP:OTHER_CORE_HITM:LOCAL_DRAM",
      .ucode  = 0x270000,
      .umodel = PFM_PMU_INTEL_WSM,
      .grpid  = 1,
   },
   {
      .uname  = "REMOTE_CACHE_DRAM",
      .udesc  = "Response: requests that miss L3 and hit remote caches or DRAM",
      .uequiv = "REMOTE_CACHE_HITM:REMOTE_CACHE_FWD:REMOTE_DRAM",
      .ucode  = 0x580000,
      .grpid  = 1,
      .umodel = PFM_PMU_INTEL_WSM,
   },

   {
      .uname  = "LOCAL_CACHE",
      .udesc  = "Response: any local (core and socket) caches",
      .uequiv = "UNCORE_HIT:OTHER_CORE_HIT_SNP:OTHER_CORE_HITM",
      .ucode  = 0x70000,
      .grpid  = 1,
   },

   {
      .uname  = "ANY_RESPONSE",
      .udesc  = "Response: combination of all response umasks",
      .uequiv = "UNCORE_HIT:OTHER_CORE_HIT_SNP:OTHER_CORE_HITM:REMOTE_CACHE_HITM:REMOTE_CACHE_FWD:REMOTE_DRAM:LOCAL_DRAM:NON_DRAM",
      .ucode = 0xff0000,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL,
      .grpid = 1,
      .umodel = PFM_PMU_INTEL_WSM,
   },

   {
      .uname  = "ANY_RESPONSE",
      .udesc  = "Response: combination of all response umasks",
      .uequiv = "UNCORE_HIT:OTHER_CORE_HIT_SNP:OTHER_CORE_HITM:REMOTE_CACHE_HITM:REMOTE_DRAM:OTHER_LLC_MISS:LOCAL_DRAM_AND_REMOTE_CACHE_HIT:NON_DRAM",
      .ucode = 0xff0000,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL,
      .grpid = 1,
      .umodel = PFM_PMU_INTEL_WSM_DP,
   },
};

static const intel_x86_entry_t intel_wsm_pe []=
{
   {
      .name   = "UNHALTED_CORE_CYCLES",
      .desc   = "Count core clock cycles whenever the clock signal on the specific core is running (not halted).",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0x20000000full,
      .code = 0x3c,
   },
   {
      .name   = "INSTRUCTION_RETIRED",
      .desc   = "Count the number of instructions at retirement.",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0x10000000full,
      .code = 0xc0,
   },
   {
      .name   = "INSTRUCTIONS_RETIRED",
      .desc   = "This is an alias for INSTRUCTION_RETIRED",
      .modmsk = INTEL_V3_ATTRS,
      .equiv = "INSTRUCTION_RETIRED",
      .cntmsk = 0x10000000full,
      .code = 0xc0,
   },
   {
      .name   = "UNHALTED_REFERENCE_CYCLES",
      .desc   = "Unhalted reference cycles",
      .modmsk = INTEL_FIXED3_ATTRS,
      .cntmsk = 0x400000000ull,
      .code = 0x0300, /* pseudo encoding */
   },
   {
      .name   = "LLC_REFERENCES",
      .desc   = "Count each request originating from the core to reference a cache line in the last level cache. The count may include speculation, but excludes cache line fills due to hardware prefetch.",
      .modmsk = INTEL_V3_ATTRS,
      .equiv = "L3_LAT_CACHE:REFERENCE",
      .cntmsk = 0xf,
      .code = 0x4f2e,
   },
   {
      .name   = "LAST_LEVEL_CACHE_REFERENCES",
      .desc   = "This is an alias for LLC_REFERENCES",
      .modmsk = INTEL_V3_ATTRS,
      .equiv = "L3_LAT_CACHE:REFERENCE",
      .cntmsk = 0xf,
      .code = 0x4f2e,
   },
   {
      .name   = "LLC_MISSES",
      .desc   = "Count each cache miss condition for references to the last level cache. The event count may include speculation, but excludes cache line fills due to hardware prefetch.",
      .modmsk = INTEL_V3_ATTRS,
      .equiv = "L3_LAT_CACHE:MISS",
      .cntmsk = 0xf,
      .code = 0x412e,
   },
   {
      .name   = "LAST_LEVEL_CACHE_MISSES",
      .desc   = "This is an alias for LLC_MISSES",
      .modmsk = INTEL_V3_ATTRS,
      .equiv = "L3_LAT_CACHE:MISS",
      .cntmsk = 0xf,
      .code = 0x412e,
   },
   {
      .name   = "BRANCH_INSTRUCTIONS_RETIRED",
      .desc   = "Count branch instructions at retirement. Specifically, this event counts the retirement of the last micro-op of a branch instruction.",
      .modmsk = INTEL_V3_ATTRS,
      .equiv = "BR_INST_RETIRED:ALL_BRANCHES",
      .cntmsk = 0xf,
      .code = 0x4c4,
   },
   {
      .name   = "UOPS_DECODED",
      .desc   = "Micro-ops decoded",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0xd1,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_uops_decoded),
      .ngrp = 1,
      .umasks = wsm_uops_decoded,
   },
   {
      .name   = "L1D_CACHE_LOCK_FB_HIT",
      .desc   = "L1D cacheable load lock speculated or retired accepted into the fill buffer",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0x152,
   },
   {
      .name   = "BPU_CLEARS",
      .desc   = "Branch Prediction Unit clears",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0xe8,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_bpu_clears),
      .ngrp = 1,
      .umasks = wsm_bpu_clears,
   },
   {
      .name   = "UOPS_RETIRED",
      .desc   = "Cycles Uops are being retired (Precise Event)",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0xc2,
      .flags= INTEL_X86_PEBS,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_uops_retired),
      .ngrp = 1,
      .umasks = wsm_uops_retired,
   },
   {
      .name   = "BR_MISP_RETIRED",
      .desc   = "Mispredicted retired branches (Precise Event)",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0xc5,
      .flags= INTEL_X86_PEBS,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_br_misp_retired),
      .ngrp = 1,
      .umasks = wsm_br_misp_retired,
   },
   {
      .name   = "EPT",
      .desc   = "Extended Page Table",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0x4f,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_ept),
      .ngrp = 1,
      .umasks = wsm_ept,
   },
   {
      .name   = "UOPS_EXECUTED",
      .desc   = "Micro-ops executed",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0xb1,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_uops_executed),
      .ngrp = 1,
      .umasks = wsm_uops_executed,
   },
   {
      .name   = "IO_TRANSACTIONS",
      .desc   = "I/O transactions",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0x16c,
   },
   {
      .name   = "ES_REG_RENAMES",
      .desc   = "ES segment renames",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0x1d5,
   },
   {
      .name   = "INST_RETIRED",
      .desc   = "Instructions retired (Precise Event)",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0xc0,
      .flags= INTEL_X86_PEBS,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_inst_retired),
      .ngrp = 1,
      .umasks = wsm_inst_retired,
   },
   {
      .name   = "ILD_STALL",
      .desc   = "Instruction Length Decoder stalls",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0x87,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_ild_stall),
      .ngrp = 1,
      .umasks = wsm_ild_stall,
   },
   {
      .name   = "DTLB_LOAD_MISSES",
      .desc   = "DTLB load misses",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0x8,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_dtlb_load_misses),
      .ngrp = 1,
      .umasks = wsm_dtlb_load_misses,
   },
   {
      .name   = "L2_LINES_IN",
      .desc   = "L2 lines alloacated",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0xf1,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_l2_lines_in),
      .ngrp = 1,
      .umasks = wsm_l2_lines_in,
   },
   {
      .name   = "SSEX_UOPS_RETIRED",
      .desc   = "SIMD micro-ops retired (Precise Event)",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0xc7,
      .flags= INTEL_X86_PEBS,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_ssex_uops_retired),
      .ngrp = 1,
      .umasks = wsm_ssex_uops_retired,
   },
   {
      .name   = "STORE_BLOCKS",
      .desc   = "Load delayed by block code",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0x6,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_store_blocks),
      .ngrp = 1,
      .umasks = wsm_store_blocks,
   },
   {
      .name   = "FP_MMX_TRANS",
      .desc   = "Floating Point to and from MMX transitions",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0xcc,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_fp_mmx_trans),
      .ngrp = 1,
      .umasks = wsm_fp_mmx_trans,
   },
   {
      .name   = "CACHE_LOCK_CYCLES",
      .desc   = "Cache locked",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0x3,
      .code = 0x63,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_cache_lock_cycles),
      .ngrp = 1,
      .umasks = wsm_cache_lock_cycles,
   },
   {
      .name   = "OFFCORE_REQUESTS_SQ_FULL",
      .desc   = "Offcore requests blocked due to Super Queue full",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0x1b2,
   },
   {
      .name   = "L3_LAT_CACHE",
      .desc   = "Last level cache accesses",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0x2e,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_l3_lat_cache),
      .ngrp = 1,
      .umasks = wsm_l3_lat_cache,
   },
   {
      .name   = "SIMD_INT_64",
      .desc   = "SIMD 64-bit integer operations",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0xfd,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_simd_int_64),
      .ngrp = 1,
      .umasks = wsm_simd_int_64,
   },
   {
      .name   = "BR_INST_DECODED",
      .desc   = "Branch instructions decoded",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0x1e0,
   },
   {
      .name   = "BR_MISP_EXEC",
      .desc   = "Mispredicted branches executed",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0x89,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_br_misp_exec),
      .ngrp = 1,
      .umasks = wsm_br_misp_exec,
   },
   {
      .name   = "SQ_FULL_STALL_CYCLES",
      .desc   = "Super Queue full stall cycles",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0x1f6,
   },
   {
      .name   = "BACLEAR",
      .desc   = "Branch address calculator clears",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0xe6,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_baclear),
      .ngrp = 1,
      .umasks = wsm_baclear,
   },
   {
      .name   = "DTLB_MISSES",
      .desc   = "Data TLB misses",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0x49,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_dtlb_misses),
      .ngrp = 1,
      .umasks = wsm_dtlb_misses,
   },
   {
      .name   = "MEM_INST_RETIRED",
      .desc   = "Memory instructions retired (Precise Event)",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0xb,
      .flags= INTEL_X86_PEBS,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_mem_inst_retired),
      .ngrp = 1,
      .umasks = wsm_mem_inst_retired,
   },
   {
      .name   = "UOPS_ISSUED",
      .desc   = "Uops issued",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0xe,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_uops_issued),
      .ngrp = 1,
      .umasks = wsm_uops_issued,
   },
   {
      .name   = "L2_RQSTS",
      .desc   = "L2 requests",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0x24,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_l2_rqsts),
      .ngrp = 1,
      .umasks = wsm_l2_rqsts,
   },
   {
      .name   = "TWO_UOP_INSTS_DECODED",
      .desc   = "Two Uop instructions decoded",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0x119,
   },
   {
      .name   = "LOAD_DISPATCH",
      .desc   = "Loads dispatched",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0x13,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_load_dispatch),
      .ngrp = 1,
      .umasks = wsm_load_dispatch,
   },
   {
      .name   = "BACLEAR_FORCE_IQ",
      .desc   = "BACLEAR forced by Instruction queue",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0x1a7,
   },
   {
      .name   = "SNOOPQ_REQUESTS",
      .desc   = "Snoopq requests",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0xb4,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_snoopq_requests),
      .ngrp = 1,
      .umasks = wsm_snoopq_requests,
   },
   {
      .name   = "OFFCORE_REQUESTS",
      .desc   = "Offcore requests",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0xb0,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_offcore_requests),
      .ngrp = 1,
      .umasks = wsm_offcore_requests,
   },
   {
      .name   = "LOAD_BLOCK",
      .desc   = "Loads blocked",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0x3,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_load_block),
      .ngrp = 1,
      .umasks = wsm_load_block,
   },
   {
      .name   = "MISALIGN_MEMORY",
      .desc   = "Misaligned accesses",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0x5,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_misalign_memory),
      .ngrp = 1,
      .umasks = wsm_misalign_memory,
   },
   {
      .name   = "INST_QUEUE_WRITE_CYCLES",
      .desc   = "Cycles instructions are written to the instruction queue",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0x11e,
   },
   {
      .name   = "LSD_OVERFLOW",
      .desc   = "Number of loops that cannot stream from the instruction queue.",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0x120,
   },
   {
      .name   = "MACHINE_CLEARS",
      .desc   = "Machine clear asserted",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0xc3,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_machine_clears),
      .ngrp = 1,
      .umasks = wsm_machine_clears,
   },
   {
      .name   = "FP_COMP_OPS_EXE",
      .desc   = "SSE/MMX micro-ops",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0x10,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_fp_comp_ops_exe),
      .ngrp = 1,
      .umasks = wsm_fp_comp_ops_exe,
   },
   {
      .name   = "ITLB_FLUSH",
      .desc   = "ITLB flushes",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0x1ae,
   },
   {
      .name   = "BR_INST_RETIRED",
      .desc   = "Retired branch instructions (Precise Event)",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0xc4,
      .flags= INTEL_X86_PEBS,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_br_inst_retired),
      .ngrp = 1,
      .umasks = wsm_br_inst_retired,
   },
   {
      .name   = "L1D_CACHE_PREFETCH_LOCK_FB_HIT",
      .desc   = "L1D prefetch load lock accepted in fill buffer",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0x3,
      .code = 0x152,
   },
   {
      .name   = "LARGE_ITLB",
      .desc   = "Large ITLB accesses",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0x82,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_large_itlb),
      .ngrp = 1,
      .umasks = wsm_large_itlb,
   },
   {
      .name   = "LSD",
      .desc   = "Loop stream detector",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0xa8,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_lsd),
      .ngrp = 1,
      .umasks = wsm_lsd,
   },
   {
      .name   = "L2_LINES_OUT",
      .desc   = "L2 lines evicted",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0xf2,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_l2_lines_out),
      .ngrp = 1,
      .umasks = wsm_l2_lines_out,
   },
   {
      .name   = "ITLB_MISSES",
      .desc   = "ITLB miss",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0x85,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_itlb_misses),
      .ngrp = 1,
      .umasks = wsm_itlb_misses,
   },
   {
      .name   = "L1D_PREFETCH",
      .desc   = "L1D hardware prefetch",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0x3,
      .code = 0x4e,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_l1d_prefetch),
      .ngrp = 1,
      .umasks = wsm_l1d_prefetch,
   },
   {
      .name   = "SQ_MISC",
      .desc   = "Super Queue miscellaneous",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0xf4,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_sq_misc),
      .ngrp = 1,
      .umasks = wsm_sq_misc,
   },
   {
      .name   = "SEG_RENAME_STALLS",
      .desc   = "Segment rename stall cycles",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0x1d4,
   },
   {
      .name   = "FP_ASSIST",
      .desc   = "X87 Floating point assists (Precise Event)",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0xf7,
      .flags= INTEL_X86_PEBS,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_fp_assist),
      .ngrp = 1,
      .umasks = wsm_fp_assist,
   },
   {
      .name   = "SIMD_INT_128",
      .desc   = "128 bit SIMD operations",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0x12,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_simd_int_128),
      .ngrp = 1,
      .umasks = wsm_simd_int_128,
   },
   {
      .name   = "OFFCORE_REQUESTS_OUTSTANDING",
      .desc   = "Outstanding offcore requests",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0x1,
      .code = 0x60,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_offcore_requests_outstanding),
      .ngrp = 1,
      .umasks = wsm_offcore_requests_outstanding,
   },
   {
      .name   = "MEM_STORE_RETIRED",
      .desc   = "Retired stores",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0xc,
      .flags= INTEL_X86_PEBS,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_mem_store_retired),
      .ngrp = 1,
      .umasks = wsm_mem_store_retired,
   },
   {
      .name   = "INST_DECODED",
      .desc   = "Instructions decoded",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0x18,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_inst_decoded),
      .ngrp = 1,
      .umasks = wsm_inst_decoded,
   },
   {
      .name   = "MACRO_INSTS_FUSIONS_DECODED",
      .desc   = "Count the number of instructions decoded that are macros-fused but not necessarily executed or retired",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0x1a6,
   },
   {
      .name   = "MACRO_INSTS",
      .desc   = "Macro-instructions",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0xd0,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_macro_insts),
      .ngrp = 1,
      .umasks = wsm_macro_insts,
   },
   {
      .name   = "PARTIAL_ADDRESS_ALIAS",
      .desc   = "False dependencies due to partial address aliasing",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0x107,
   },
   {
      .name   = "ARITH",
      .desc   = "Counts arithmetic multiply and divide operations",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0x14,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_arith),
      .ngrp = 1,
      .umasks = wsm_arith,
   },
   {
      .name   = "L2_TRANSACTIONS",
      .desc   = "L2 transactions",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0xf0,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_l2_transactions),
      .ngrp = 1,
      .umasks = wsm_l2_transactions,
   },
   {
      .name   = "INST_QUEUE_WRITES",
      .desc   = "Instructions written to instruction queue.",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0x117,
   },
   {
      .name   = "SB_DRAIN",
      .desc   = "Store buffer",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0x4,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_sb_drain),
      .ngrp = 1,
      .umasks = wsm_sb_drain,
   },
   {
      .name   = "LOAD_HIT_PRE",
      .desc   = "Load operations conflicting with software prefetches",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0x3,
      .code = 0x14c,
   },
   {
      .name   = "MEM_UNCORE_RETIRED",
      .desc   = "Load instructions retired (Precise Event)",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0xf,
      .flags= INTEL_X86_PEBS,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_mem_uncore_retired),
      .ngrp = 1,
      .umasks = wsm_mem_uncore_retired,
   },
   {
      .name   = "L2_DATA_RQSTS",
      .desc   = "All L2 data requests",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0x26,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_l2_data_rqsts),
      .ngrp = 1,
      .umasks = wsm_l2_data_rqsts,
   },
   {
      .name   = "BR_INST_EXEC",
      .desc   = "Branch instructions executed",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0x88,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_br_inst_exec),
      .ngrp = 1,
      .umasks = wsm_br_inst_exec,
   },
   {
      .name   = "ITLB_MISS_RETIRED",
      .desc   = "Retired instructions that missed the ITLB (Precise Event)",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0x20c8,
      .flags= INTEL_X86_PEBS,
   },
   {
      .name   = "BPU_MISSED_CALL_RET",
      .desc   = "Branch prediction unit missed call or return",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0x1e5,
   },
   {
      .name   = "SNOOPQ_REQUESTS_OUTSTANDING",
      .desc   = "Outstanding snoop requests",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0x1,
      .code = 0xb3,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_snoopq_requests_outstanding),
      .ngrp = 1,
      .umasks = wsm_snoopq_requests_outstanding,
   },
   {
      .name   = "MEM_LOAD_RETIRED",
      .desc   = "Memory loads retired (Precise Event)",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0xcb,
      .flags= INTEL_X86_PEBS,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_mem_load_retired),
      .ngrp = 1,
      .umasks = wsm_mem_load_retired,
   },
   {
      .name   = "L1I",
      .desc   = "L1I instruction fetch",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0x80,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_l1i),
      .ngrp = 1,
      .umasks = wsm_l1i,
   },
   {
      .name   = "L2_WRITE",
      .desc   = "L2 demand lock/store RFO",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0x27,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_l2_write),
      .ngrp = 1,
      .umasks = wsm_l2_write,
   },
   {
      .name   = "SNOOP_RESPONSE",
      .desc   = "Snoop",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0xb8,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_snoop_response),
      .ngrp = 1,
      .umasks = wsm_snoop_response,
   },
   {
      .name   = "L1D",
      .desc   = "L1D cache",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0x3,
      .code = 0x51,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_l1d),
      .ngrp = 1,
      .umasks = wsm_l1d,
   },
   {
      .name   = "RESOURCE_STALLS",
      .desc   = "Resource related stall cycles",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0xa2,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_resource_stalls),
      .ngrp = 1,
      .umasks = wsm_resource_stalls,
   },
   {
      .name   = "RAT_STALLS",
      .desc   = "All RAT stall cycles",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0xd2,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_rat_stalls),
      .ngrp = 1,
      .umasks = wsm_rat_stalls,
   },
   {
      .name   = "CPU_CLK_UNHALTED",
      .desc   = "Cycles when processor is not in halted state",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0x3c,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_cpu_clk_unhalted),
      .ngrp = 1,
      .umasks = wsm_cpu_clk_unhalted,
   },
   {
      .name   = "L1D_WB_L2",
      .desc   = "L1D writebacks to L2",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0x28,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_l1d_wb_l2),
      .ngrp = 1,
      .umasks = wsm_l1d_wb_l2,
   },
   {
      .name   = "MISPREDICTED_BRANCH_RETIRED",
      .desc   = "Count mispredicted branch instructions at retirement. Specifically, this event counts at retirement of the last micro-op of a branch instruction in the architectural path of the execution and experienced misprediction in the branch prediction hardware",
      .modmsk = INTEL_V3_ATTRS,
      .equiv = "BR_MISP_RETIRED:ALL_BRANCHES",
      .cntmsk = 0xf,
      .code = 0xc5,
   },
   {
      .name   = "THREAD_ACTIVE",
      .desc   = "Cycles thread is active",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0x1ec,
   },
   {
      .name   = "UOP_UNFUSION",
      .desc   = "Counts unfusion events due to floating point exception to a fused uop",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0x1db,
   },
   {
      .name   = "OFFCORE_RESPONSE_0",
      .desc   = "Offcore response 0 (must provide at least one request and one response umasks)",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0x1b7,
      .flags= INTEL_X86_NHM_OFFCORE,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_offcore_response_0),
      .ngrp = 2,
      .umasks = wsm_offcore_response_0,
   },
   {
      .name   = "OFFCORE_RESPONSE_1",
      .desc   = "Offcore response 1 (must provide at least one request and one response umasks)",
      .modmsk = INTEL_V3_ATTRS,
      .cntmsk = 0xf,
      .code = 0x1bb,
      .flags= INTEL_X86_NHM_OFFCORE,
      .numasks = LIBPFM_ARRAY_SIZE (wsm_offcore_response_0),
      .ngrp = 2,
      .umasks = wsm_offcore_response_0, /* identical to actual umasks list for this event */
   },
};
