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
 * PMU: amd64_fam14h (AMD64 Fam14h)
 */

static const amd64_umask_t amd64_fam14h_dispatched_fpu []=
{
   {
      .uname  = "PIPE0",
      .udesc  = "Pipe 0 (fadd, imul, mmx) ops",
      .ucode = 0x1,
   },
   {
      .uname  = "PIPE1",
      .udesc  = "Pipe 1 (fmul, store, mmx) ops",
      .ucode = 0x2,
   },
   {
      .uname  = "ANY",
      .udesc  = "Pipe 1 and Pipe 0 ops",
      .ucode = 0x3,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam14h_retired_sse_operations []=
{
   {
      .uname  = "SINGLE_ADD_SUB_OPS",
      .udesc  = "Single precision add/subtract ops",
      .ucode = 0x1,
   },
   {
      .uname  = "SINGLE_MUL_OPS",
      .udesc  = "Single precision multiply ops",
      .ucode = 0x2,
   },
   {
      .uname  = "SINGLE_DIV_OPS",
      .udesc  = "Single precision divide/square root ops",
      .ucode = 0x4,
   },
   {
      .uname  = "DOUBLE_ADD_SUB_OPS",
      .udesc  = "Double precision add/subtract ops",
      .ucode = 0x8,
   },
   {
      .uname  = "DOUBLE_MUL_OPS",
      .udesc  = "Double precision multiply ops",
      .ucode = 0x10,
   },
   {
      .uname  = "DOUBLE_DIV_OPS",
      .udesc  = "Double precision divide/square root ops",
      .ucode = 0x20,
   },
   {
      .uname  = "OP_TYPE",
      .udesc  = "Op type: 0=uops. 1=FLOPS",
      .ucode = 0x40,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0x7f,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam14h_retired_move_ops []=
{
   {
      .uname  = "ALL_OTHER_MERGING_MOVE_UOPS",
      .udesc  = "All other merging move uops",
      .ucode = 0x4,
   },
   {
      .uname  = "ALL_OTHER_MOVE_UOPS",
      .udesc  = "All other move uops",
      .ucode = 0x8,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0xc,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam14h_retired_serializing_ops []=
{
   {
      .uname  = "SSE_BOTTOM_EXECUTING_UOPS",
      .udesc  = "SSE bottom-executing uops retired",
      .ucode = 0x1,
   },
   {
      .uname  = "SSE_BOTTOM_SERIALIZING_UOPS",
      .udesc  = "SSE bottom-serializing uops retired",
      .ucode = 0x2,
   },
   {
      .uname  = "X87_BOTTOM_EXECUTING_UOPS",
      .udesc  = "X87 bottom-executing uops retired",
      .ucode = 0x4,
   },
   {
      .uname  = "X87_BOTTOM_SERIALIZING_UOPS",
      .udesc  = "X87 bottom-serializing uops retired",
      .ucode = 0x8,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0xf,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam14h_retired_x87_fpu_ops []=
{
   {
      .uname  = "ADD_SUB_OPS",
      .udesc  = "Add/substract ops",
      .ucode = 0x1,
   },
   {
      .uname  = "MULT_OPS",
      .udesc  = "Multiply ops",
      .ucode = 0x2,
   },
   {
      .uname  = "DIV_FSQRT_OPS",
      .udesc  = "Divide and fqsrt ops",
      .ucode = 0x4,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0x7,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam14h_segment_register_loads []=
{
   {
      .uname  = "ES",
      .udesc  = "ES",
      .ucode = 0x1,
   },
   {
      .uname  = "CS",
      .udesc  = "CS",
      .ucode = 0x2,
   },
   {
      .uname  = "SS",
      .udesc  = "SS",
      .ucode = 0x4,
   },
   {
      .uname  = "DS",
      .udesc  = "DS",
      .ucode = 0x8,
   },
   {
      .uname  = "FS",
      .udesc  = "FS",
      .ucode = 0x10,
   },
   {
      .uname  = "GS",
      .udesc  = "GS",
      .ucode = 0x20,
   },
   {
      .uname  = "HS",
      .udesc  = "HS",
      .ucode = 0x40,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0x7f,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam14h_locked_ops []=
{
   {
      .uname  = "EXECUTED",
      .udesc  = "Number of locked instructions executed",
      .ucode = 0x1,
   },
   {
      .uname  = "BUS_LOCK",
      .udesc  = "Number of cycles to acquire bus lock",
      .ucode = 0x2,
   },
   {
      .uname  = "UNLOCK_LINE",
      .udesc  = "Number of cycles to unlock line (not including cache miss)",
      .ucode = 0x4,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0x7,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam14h_cancelled_store_to_load_forward_operations []=
{
   {
      .uname  = "ADDRESS_MISMATCHES",
      .udesc  = "Address mismatches (starting byte not the same).",
      .ucode = 0x1,
   },
   {
      .uname  = "STORE_IS_SMALLER_THAN_LOAD",
      .udesc  = "Store is smaller than load.",
      .ucode = 0x2,
   },
   {
      .uname  = "MISALIGNED",
      .udesc  = "Misaligned.",
      .ucode = 0x4,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0x7,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam14h_data_cache_refills []=
{
   {
      .uname  = "UNCACHEABLE",
      .udesc  = "From non-cacheable data",
      .ucode = 0x1,
   },
   {
      .uname  = "SHARED",
      .udesc  = "From shared lines",
      .ucode = 0x2,
   },
   {
      .uname  = "EXCLUSIVE",
      .udesc  = "From exclusive lines",
      .ucode = 0x4,
   },
   {
      .uname  = "OWNED",
      .udesc  = "From owned lines",
      .ucode = 0x8,
   },
   {
      .uname  = "MODIFIED",
      .udesc  = "From modified lines",
      .ucode = 0x10,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0x1f,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam14h_data_cache_refills_from_nb []=
{
   {
      .uname  = "UNCACHEABLE",
      .udesc  = "Uncacheable data",
      .ucode = 0x1,
   },
   {
      .uname  = "SHARED",
      .udesc  = "Shared",
      .ucode = 0x2,
   },
   {
      .uname  = "EXCLUSIVE",
      .udesc  = "Exclusive",
      .ucode = 0x4,
   },
   {
      .uname  = "OWNED",
      .udesc  = "Owned",
      .ucode = 0x8,
   },
   {
      .uname  = "MODIFIED",
      .udesc  = "Modified",
      .ucode = 0x10,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0x1f,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam14h_data_cache_lines_evicted []=
{
   {
      .uname  = "PROBE",
      .udesc  = "Eviction from probe",
      .ucode = 0x1,
   },
   {
      .uname  = "SHARED",
      .udesc  = "Shared eviction",
      .ucode = 0x2,
   },
   {
      .uname  = "EXCLUSIVE",
      .udesc  = "Exclusive eviction",
      .ucode = 0x4,
   },
   {
      .uname  = "OWNED",
      .udesc  = "Owned eviction",
      .ucode = 0x8,
   },
   {
      .uname  = "MODIFIED",
      .udesc  = "Modified eviction",
      .ucode = 0x10,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0x1f,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam14h_dtlb_miss []=
{
   {
      .uname  = "STORES_L1TLB_MISS",
      .udesc  = "Stores that miss L1TLB",
      .ucode = 0x1,
   },
   {
      .uname  = "LOADS_L1TLB_MISS",
      .udesc  = "Loads that miss L1TLB",
      .ucode = 0x2,
   },
   {
      .uname  = "STORES_L2TLB_MISS",
      .udesc  = "Stores that miss L2TLB",
      .ucode = 0x4,
   },
   {
      .uname  = "LOADS_L2TLB_MISS",
      .udesc  = "Loads that miss L2TLB",
      .ucode = 0x8,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0xf,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam14h_prefetch_instructions_dispatched []=
{
   {
      .uname  = "LOAD",
      .udesc  = "Load (Prefetch, PrefetchT0/T1/T2)",
      .ucode = 0x1,
   },
   {
      .uname  = "STORE",
      .udesc  = "Store (PrefetchW)",
      .ucode = 0x2,
   },
   {
      .uname  = "NTA",
      .udesc  = "NTA (PrefetchNTA)",
      .ucode = 0x4,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0x7,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam14h_l1_dtlb_hit []=
{
   {
      .uname  = "L1_4K_TLB_HIT",
      .udesc  = "L1 4K TLB hit",
      .ucode = 0x1,
   },
   {
      .uname  = "L1_2M_TLB_HIT",
      .udesc  = "L1 2M TLB hit",
      .ucode = 0x2,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0x3,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam14h_dcache_sw_prefetches []=
{
   {
      .uname  = "HIT",
      .udesc  = "SW prefetch hit in the data cache",
      .ucode = 0x1,
   },
   {
      .uname  = "PENDING_FILL",
      .udesc  = "SW prefetch hit a pending fill",
      .ucode = 0x2,
   },
   {
      .uname  = "NO_MAB",
      .udesc  = "SW prefetch does not get a MAB",
      .ucode = 0x4,
   },
   {
      .uname  = "L2_HIT",
      .udesc  = "SW prefetch hits L2",
      .ucode = 0x8,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0xf,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam14h_memory_requests []=
{
   {
      .uname  = "NON_CACHEABLE",
      .udesc  = "Requests to non-cacheable (UC) memory",
      .ucode = 0x1,
   },
   {
      .uname  = "WRITE_COMBINING",
      .udesc  = "Requests to write-combining (WC) memory or WC buffer flushes to WB memory",
      .ucode = 0x2,
   },
   {
      .uname  = "STREAMING_STORE",
      .udesc  = "Streaming store (SS) requests",
      .ucode = 0x80,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0x83,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam14h_mab_requests []=
{
   {
      .uname  = "DC_BUFFER_0",
      .udesc  = "Data cache buffer 0",
      .ucode = 0x0,
      .uflags= AMD64_FL_NCOMBO,
   },
   {
      .uname  = "DC_BUFFER_1",
      .udesc  = "Data cache buffer 1",
      .ucode = 0x1,
      .uflags= AMD64_FL_NCOMBO,
   },
   {
      .uname  = "DC_BUFFER_2",
      .udesc  = "Data cache buffer 2",
      .ucode = 0x2,
      .uflags= AMD64_FL_NCOMBO,
   },
   {
      .uname  = "DC_BUFFER_3",
      .udesc  = "Data cache buffer 3",
      .ucode = 0x3,
      .uflags= AMD64_FL_NCOMBO,
   },
   {
      .uname  = "DC_BUFFER_4",
      .udesc  = "Data cache buffer 4",
      .ucode = 0x4,
      .uflags= AMD64_FL_NCOMBO,
   },
   {
      .uname  = "DC_BUFFER_5",
      .udesc  = "Data cache buffer 5",
      .ucode = 0x5,
      .uflags= AMD64_FL_NCOMBO,
   },
   {
      .uname  = "DC_BUFFER_6",
      .udesc  = "Data cache buffer 6",
      .ucode = 0x6,
      .uflags= AMD64_FL_NCOMBO,
   },
   {
      .uname  = "DC_BUFFER_7",
      .udesc  = "Data cache buffer 7",
      .ucode = 0x7,
      .uflags= AMD64_FL_NCOMBO,
   },
   {
      .uname  = "IC_BUFFER_0",
      .udesc  = "Instruction cache Buffer 1",
      .ucode = 0x8,
      .uflags= AMD64_FL_NCOMBO,
   },
   {
      .uname  = "IC_BUFFER_1",
      .udesc  = "Instructions cache buffer 1",
      .ucode = 0x9,
      .uflags= AMD64_FL_NCOMBO,
   },
   {
      .uname  = "ANY_IC_BUFFER",
      .udesc  = "Any instruction cache buffer",
      .ucode = 0xa,
      .uflags= AMD64_FL_NCOMBO,
   },
   {
      .uname  = "ANY_DC_BUFFER",
      .udesc  = "Any data cache buffer",
      .ucode = 0xb,
      .uflags= AMD64_FL_NCOMBO,
   },
};

static const amd64_umask_t amd64_fam14h_system_read_responses []=
{
   {
      .uname  = "EXCLUSIVE",
      .udesc  = "Exclusive",
      .ucode = 0x1,
   },
   {
      .uname  = "MODIFIED",
      .udesc  = "Modified",
      .ucode = 0x2,
   },
   {
      .uname  = "SHARED",
      .udesc  = "Shared",
      .ucode = 0x4,
   },
   {
      .uname  = "OWNED",
      .udesc  = "Owned",
      .ucode = 0x8,
   },
   {
      .uname  = "DATA_ERROR",
      .udesc  = "Data Error",
      .ucode = 0x10,
   },
   {
      .uname  = "DIRTY_SUCCESS",
      .udesc  = "Change-to-dirty success",
      .ucode = 0x20,
   },
   {
      .uname  = "UNCACHEABLE",
      .udesc  = "Uncacheable",
      .ucode = 0x40,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0x7f,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam14h_requests_to_l2[]=
{
   {
      .uname  = "INSTRUCTIONS",
      .udesc  = "IC fill",
      .ucode = 0x1,
   },
   {
      .uname  = "DATA",
      .udesc  = "DC fill",
      .ucode = 0x2,
   },
   {
      .uname  = "SNOOP",
      .udesc  = "Tag snoop request",
      .ucode = 0x8,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0xb,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam14h_l2_cache_miss []=
{
   {
      .uname  = "INSTRUCTIONS",
      .udesc  = "IC fill",
      .ucode = 0x1,
   },
   {
      .uname  = "DATA",
      .udesc  = "DC fill (includes possible replays, whereas EventSelect 041h does not)",
      .ucode = 0x2,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0x3,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam14h_l2_fill_writeback []=
{
   {
      .uname  = "L2_FILLS",
      .udesc  = "L2 fills (victims from L1 caches, TLB page table walks and data prefetches)",
      .ucode = 0x1,
   },
   {
      .uname  = "L2_WRITEBACKS",
      .udesc  = "L2 Writebacks to system.",
      .ucode = 0x2,
   },
   {
      .uname  = "IC_ATTR_WRITES_L2_ACCESS",
      .udesc  = "Ic attribute writes which access the L2",
      .ucode = 0x4,
   },
   {
      .uname  = "IC_ATTR_WRITES_L2_WRITES",
      .udesc  = "Ic attribute writes which store into the L2",
      .ucode = 0x8,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0xf,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam14h_l1_itlb_miss_and_l2_itlb_miss []=
{
   {
      .uname  = "4K_PAGE_FETCHES",
      .udesc  = "Instruction fetches to a 4K page.",
      .ucode = 0x1,
   },
   {
      .uname  = "2M_PAGE_FETCHES",
      .udesc  = "Instruction fetches to a 2M page.",
      .ucode = 0x2,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0x3,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam14h_instruction_cache_lines_invalidated []=
{
   {
      .uname  = "INVALIDATING_LS_PROBE",
      .udesc  = "IC invalidate due to an LS probe",
      .ucode = 0x1,
   },
   {
      .uname  = "INVALIDATING_BU_PROBE",
      .udesc  = "IC invalidate due to a BU probe",
      .ucode = 0x2,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0x3,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam14h_retired_floating_point_instructions []=
{
   {
      .uname  = "X87",
      .udesc  = "X87 or MMX instructions",
      .ucode = 0x1,
   },
   {
      .uname  = "SSE",
      .udesc  = "SSE (SSE, SSE2, SSE3, MNI) instructions",
      .ucode = 0x2,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0x3,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam14h_fpu_exceptions []=
{
   {
      .uname  = "X87_RECLASS_MICROFAULTS",
      .udesc  = "X87 reclass microfaults",
      .ucode = 0x1,
   },
   {
      .uname  = "SSE_RETYPE_MICROFAULTS",
      .udesc  = "SSE retype microfaults",
      .ucode = 0x2,
   },
   {
      .uname  = "SSE_RECLASS_MICROFAULTS",
      .udesc  = "SSE reclass microfaults",
      .ucode = 0x4,
   },
   {
      .uname  = "SSE_AND_X87_MICROTRAPS",
      .udesc  = "SSE and x87 microtraps",
      .ucode = 0x8,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0xf,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam14h_dram_accesses_page []=
{
   {
      .uname  = "HIT",
      .udesc  = "DCT0 Page hit",
      .ucode = 0x1,
   },
   {
      .uname  = "MISS",
      .udesc  = "DCT0 Page Miss",
      .ucode = 0x2,
   },
   {
      .uname  = "CONFLICT",
      .udesc  = "DCT0 Page Conflict",
      .ucode = 0x4,
   },
   {
      .uname  = "WRITE_REQUEST",
      .udesc  = "Write request",
      .ucode = 0x40,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0x47,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam14h_memory_controller_page_table []=
{
   {
      .uname  = "DCT0_PAGE_TABLE_OVERFLOW",
      .udesc  = "DCT0 Page Table Overflow",
      .ucode = 0x1,
   },
   {
      .uname  = "DCT0_PAGE_TABLE_STALE_HIT",
      .udesc  = "DCT0 number of stale table entry hits (hit on a page closed too soon)",
      .ucode = 0x2,
   },
   {
      .uname  = "DCT0_PAGE_TABLE_IDLE_INC",
      .udesc  = "DCT0 page table idle cycle limit incremented",
      .ucode = 0x4,
   },
   {
      .uname  = "DCT0_PAGE_TABLE_IDLE_DEC",
      .udesc  = "DCT0 page table idle cycle limit decremented",
      .ucode = 0x8,
   },
   {
      .uname  = "DCT0_PAGE_TABLE_CLOSED",
      .udesc  = "DCT0 page table is closed due to row inactivity",
      .ucode = 0x10,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0x1f,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam14h_memory_controller_slot_misses []=
{
   {
      .uname  = "DCT0_RBD",
      .udesc  = "DCT0 RBD",
      .ucode = 0x10,
   },
   {
      .uname  = "DCT0_PREFETCH",
      .udesc  = "DCT0 prefetch",
      .ucode = 0x40,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0x50,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam14h_memory_controller_rbd_queue_events []=
{
   {
      .uname  = "DCQ_BYPASS_MAX",
      .udesc  = "DCQ_BYPASS_MAX counter reached",
      .ucode = 0x4,
   },
   {
      .uname  = "BANK_CLOSED",
      .udesc  = "Bank is closed due to bank conflict with an outstanding request in the RBD queue",
      .ucode = 0x8,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0xc,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam14h_thermal_status []=
{
   {
      .uname  = "MEMHOT_L",
      .udesc  = "MEMHOT_L assertions",
      .ucode = 0x1,
   },
   {
      .uname  = "HTC_TRANSITION",
      .udesc  = "Number of times HTC transitions from inactive to active",
      .ucode = 0x4,
   },
   {
      .uname  = "CLOCKS_HTC_P_STATE_INACTIVE",
      .udesc  = "Number of clocks HTC P-state is inactive.",
      .ucode = 0x20,
   },
   {
      .uname  = "CLOCKS_HTC_P_STATE_ACTIVE",
      .udesc  = "Number of clocks HTC P-state is active",
      .ucode = 0x40,
   },
   {
      .uname  = "PROCHOT_L",
      .udesc  = "PROCHOT_L asserted by an external source and the assertion causes a P-state change",
      .ucode = 0x80,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0xc5,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam14h_cpu_io_requests_to_memory_io []=
{
   {
      .uname  = "I_O_TO_I_O",
      .udesc  = "IO to IO",
      .ucode = 0x1,
   },
   {
      .uname  = "I_O_TO_MEM",
      .udesc  = "IO to Mem",
      .ucode = 0x2,
   },
   {
      .uname  = "CPU_TO_I_O",
      .udesc  = "CPU to IO",
      .ucode = 0x4,
   },
   {
      .uname  = "CPU_TO_MEM",
      .udesc  = "CPU to Mem",
      .ucode = 0x8,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0xf,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam14h_cache_block []=
{
   {
      .uname  = "VICTIM_WRITEBACK",
      .udesc  = "Victim Block (Writeback)",
      .ucode = 0x1,
   },
   {
      .uname  = "DCACHE_LOAD_MISS",
      .udesc  = "Read Block (Dcache load miss refill)",
      .ucode = 0x4,
   },
   {
      .uname  = "SHARED_ICACHE_REFILL",
      .udesc  = "Read Block Shared (Icache refill)",
      .ucode = 0x8,
   },
   {
      .uname  = "READ_BLOCK_MODIFIED",
      .udesc  = "Read Block Modified (Dcache store miss refill)",
      .ucode = 0x10,
   },
   {
      .uname  = "CHANGE_TO_DIRTY",
      .udesc  = "Change-to-Dirty (first store to clean block already in cache)",
      .ucode = 0x20,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0x3d,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam14h_sized_commands []=
{
   {
      .uname  = "NON_POSTED_WRITE_BYTE",
      .udesc  = "Non-Posted SzWr Byte (1-32 bytes) Legacy or mapped IO, typically 1-4 bytes",
      .ucode = 0x1,
   },
   {
      .uname  = "NON_POSTED_WRITE_DWORD",
      .udesc  = "Non-Posted SzWr DW (1-16 dwords) Legacy or mapped IO, typically 1 DWORD",
      .ucode = 0x2,
   },
   {
      .uname  = "POSTED_WRITE_BYTE",
      .udesc  = "Posted SzWr Byte (1-32 bytes) Subcache-line DMA writes, size varies; also flushes of partially-filled Write Combining buffer",
      .ucode = 0x4,
   },
   {
      .uname  = "POSTED_WRITE_DWORD",
      .udesc  = "Posted SzWr DW (1-16 dwords) Block-oriented DMA writes, often cache-line sized; also processor Write Combining buffer flushes",
      .ucode = 0x8,
   },
   {
      .uname  = "READ_BYTE_4_BYTES",
      .udesc  = "SzRd Byte (4 bytes) Legacy or mapped IO",
      .ucode = 0x10,
   },
   {
      .uname  = "READ_DWORD_1_16_DWORDS",
      .udesc  = "SzRd DW (1-16 dwords) Block-oriented DMA reads, typically cache-line size",
      .ucode = 0x20,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0x3f,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam14h_probe []=
{
   {
      .uname  = "MISS",
      .udesc  = "Probe miss",
      .ucode = 0x1,
   },
   {
      .uname  = "HIT_CLEAN",
      .udesc  = "Probe hit clean",
      .ucode = 0x2,
   },
   {
      .uname  = "HIT_DIRTY_NO_MEMORY_CANCEL",
      .udesc  = "Probe hit dirty without memory cancel (probed by Sized Write or Change2Dirty)",
      .ucode = 0x4,
   },
   {
      .uname  = "HIT_DIRTY_WITH_MEMORY_CANCEL",
      .udesc  = "Probe hit dirty with memory cancel (probed by DMA read or cache refill request)",
      .ucode = 0x8,
   },
   {
      .uname  = "UPSTREAM_HIGH_PRIO_READS",
      .udesc  = "Upstream high priority reads",
      .ucode = 0x10,
   },
   {
      .uname  = "UPSTREAM_LOW_PRIO_READS",
      .udesc  = "Upstream low priority reads",
      .ucode = 0x20,
   },
   {
      .uname  = "UPSTREAM_LOW_PRIO_WRITES",
      .udesc  = "Upstream non-ISOC writes",
      .ucode = 0x80,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0xbf,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam14h_dev_events []=
{
   {
      .uname  = "HIT",
      .udesc  = "DEV hit",
      .ucode = 0x10,
   },
   {
      .uname  = "MISS",
      .udesc  = "DEV miss",
      .ucode = 0x20,
   },
   {
      .uname  = "ERROR",
      .udesc  = "DEV error",
      .ucode = 0x40,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0x70,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam14h_memory_controller_requests []=
{
   {
      .uname  = "32_BYTES_WRITES",
      .udesc  = "32 Bytes Sized Writes",
      .ucode = 0x8,
   },
   {
      .uname  = "64_BYTES_WRITES",
      .udesc  = "64 Bytes Sized Writes",
      .ucode = 0x10,
   },
   {
      .uname  = "32_BYTES_READS",
      .udesc  = "32 Bytes Sized Reads",
      .ucode = 0x20,
   },
   {
      .uname  = "64_BYTES_READS",
      .udesc  = "64 Byte Sized Reads",
      .ucode = 0x40,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0x78,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam14h_sideband_signals_special_signals []=
{
   {
      .uname  = "STOPGRANT",
      .udesc  = "Stopgrant",
      .ucode = 0x2,
   },
   {
      .uname  = "SHUTDOWN",
      .udesc  = "Shutdown",
      .ucode = 0x4,
   },
   {
      .uname  = "WBINVD",
      .udesc  = "Wbinvd",
      .ucode = 0x8,
   },
   {
      .uname  = "INVD",
      .udesc  = "Invd",
      .ucode = 0x10,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0x1c,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam14h_interrupt_events []=
{
   {
      .uname  = "FIXED_AND_LPA",
      .udesc  = "Fixed and LPA",
      .ucode = 0x1,
   },
   {
      .uname  = "LPA",
      .udesc  = "LPA",
      .ucode = 0x2,
   },
   {
      .uname  = "SMI",
      .udesc  = "SMI",
      .ucode = 0x4,
   },
   {
      .uname  = "NMI",
      .udesc  = "NMI",
      .ucode = 0x8,
   },
   {
      .uname  = "INIT",
      .udesc  = "INIT",
      .ucode = 0x10,
   },
   {
      .uname  = "STARTUP",
      .udesc  = "STARTUP",
      .ucode = 0x20,
   },
   {
      .uname  = "INT",
      .udesc  = "INT",
      .ucode = 0x40,
   },
   {
      .uname  = "EOI",
      .udesc  = "EOI",
      .ucode = 0x80,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0xff,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam14h_pdc_miss []=
{
   {
      .uname  = "HOST_PDE_LEVEL",
      .udesc  = "Host PDE level",
      .ucode = 0x1,
   },
   {
      .uname  = "HOST_PDPE_LEVEL",
      .udesc  = "Host PDPE level",
      .ucode = 0x2,
   },
   {
      .uname  = "HOST_PML4E_LEVEL",
      .udesc  = "Host PML4E level",
      .ucode = 0x4,
   },
   {
      .uname  = "GUEST_PDE_LEVEL",
      .udesc  = "Guest PDE level",
      .ucode = 0x10,
   },
   {
      .uname  = "GUEST_PDPE_LEVEL",
      .udesc  = "Guest PDPE level",
      .ucode = 0x20,
   },
   {
      .uname  = "GUEST_PML4E_LEVEL",
      .udesc  = "Guest PML4E level",
      .ucode = 0x40,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0x67,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_entry_t amd64_fam14h_pe []=
{
   {
      .name    = "DISPATCHED_FPU",
      .desc    = "Number of uops dispatched to FPU execution pipelines",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0x0,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam14h_dispatched_fpu),
      .ngrp    = 1,
      .umasks  = amd64_fam14h_dispatched_fpu,
   },
   {
      .name    = "CYCLES_NO_FPU_OPS_RETIRED",
      .desc    = "Cycles in which the FPU is Empty",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0x1,
   },
   {
      .name    = "DISPATCHED_FPU_OPS_FAST_FLAG",
      .desc    = "Dispatched Fast Flag FPU Operations",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0x2,
   },
   {
      .name    = "RETIRED_SSE_OPERATIONS",
      .desc    = "Retired SSE Operations",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0x3,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam14h_retired_sse_operations),
      .ngrp    = 1,
      .umasks  = amd64_fam14h_retired_sse_operations,
   },
   {
      .name    = "RETIRED_MOVE_OPS",
      .desc    = "Retired Move Ops",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0x4,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam14h_retired_move_ops),
      .ngrp    = 1,
      .umasks  = amd64_fam14h_retired_move_ops,
   },
   {
      .name    = "RETIRED_SERIALIZING_OPS",
      .desc    = "Retired Serializing Ops",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0x5,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam14h_retired_serializing_ops),
      .ngrp    = 1,
      .umasks  = amd64_fam14h_retired_serializing_ops,
   },
   {
      .name    = "RETIRED_X87_FPU_OPS",
      .desc    = "Number of x87 floating points ops that have retired",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0x11,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam14h_retired_x87_fpu_ops),
      .ngrp    = 1,
      .umasks  = amd64_fam14h_retired_x87_fpu_ops,
   },
   {
      .name    = "SEGMENT_REGISTER_LOADS",
      .desc    = "Segment Register Loads",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0x20,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam14h_segment_register_loads),
      .ngrp    = 1,
      .umasks  = amd64_fam14h_segment_register_loads,
   },
   {
      .name    = "PIPELINE_RESTART_DUE_TO_SELF_MODIFYING_CODE",
      .desc    = "Pipeline Restart Due to Self-Modifying Code",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0x21,
   },
   {
      .name    = "PIPELINE_RESTART_DUE_TO_PROBE_HIT",
      .desc    = "Pipeline Restart Due to Probe Hit",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0x22,
   },
   {
      .name    = "RSQ_FULL",
      .desc    = "Number of cycles that the RSQ holds retired stores. This buffer holds the stores waiting to retired as well as requests that missed the data cacge and waiting on a refill",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0x23,
   },
   {
      .name    = "LOCKED_OPS",
      .desc    = "Locked Operations",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0x24,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam14h_locked_ops),
      .ngrp    = 1,
      .umasks  = amd64_fam14h_locked_ops,
   },
   {
      .name    = "RETIRED_CLFLUSH_INSTRUCTIONS",
      .desc    = "Retired CLFLUSH Instructions",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0x26,
   },
   {
      .name    = "RETIRED_CPUID_INSTRUCTIONS",
      .desc    = "Retired CPUID Instructions",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0x27,
   },
   {
      .name    = "CANCELLED_STORE_TO_LOAD_FORWARD_OPERATIONS",
      .desc    = "Cancelled Store to Load Forward Operations",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0x2a,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam14h_cancelled_store_to_load_forward_operations),
      .ngrp    = 1,
      .umasks  = amd64_fam14h_cancelled_store_to_load_forward_operations,
   },
   {
      .name    = "DATA_CACHE_ACCESSES",
      .desc    = "Data Cache Accesses",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0x40,
   },
   {
      .name    = "DATA_CACHE_MISSES",
      .desc    = "Data Cache Misses",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0x41,
   },
   {
      .name    = "DATA_CACHE_REFILLS",
      .desc    = "Data Cache Refills from L2 or Northbridge",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0x42,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam14h_data_cache_refills),
      .ngrp    = 1,
      .umasks  = amd64_fam14h_data_cache_refills,
   },
   {
      .name    = "DATA_CACHE_REFILLS_FROM_NB",
      .desc    = "Data Cache Refills from the Northbridge",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0x43,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam14h_data_cache_refills_from_nb),
      .ngrp    = 1,
      .umasks  = amd64_fam14h_data_cache_refills_from_nb,
   },
   {
      .name    = "DATA_CACHE_LINES_EVICTED",
      .desc    = "Data Cache Lines Evicted",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0x44,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam14h_data_cache_lines_evicted),
      .ngrp    = 1,
      .umasks  = amd64_fam14h_data_cache_lines_evicted,
   },
   {
      .name    = "L1_DTLB_MISS_AND_L2_DTLB_HIT",
      .desc    = "Number of data cache accesses that miss in the L1 DTLB and hit the L2 DTLB. This is a speculative event",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0x45,
   },
   {
      .name    = "DTLB_MISS",
      .desc    = "L1 DTLB and L2 DTLB Miss",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0x46,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam14h_dtlb_miss),
      .ngrp    = 1,
      .umasks  = amd64_fam14h_dtlb_miss,
   },
   {
      .name    = "MISALIGNED_ACCESSES",
      .desc    = "Misaligned Accesses",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0x47,
   },
   {
      .name    = "PREFETCH_INSTRUCTIONS_DISPATCHED",
      .desc    = "Prefetch Instructions Dispatched",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0x4b,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam14h_prefetch_instructions_dispatched),
      .ngrp    = 1,
      .umasks  = amd64_fam14h_prefetch_instructions_dispatched,
   },
   {
      .name    = "DCACHE_MISSES_BY_LOCKED_INSTRUCTIONS",
      .desc    = "DCACHE Misses by Locked Instructions",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0x4c,
   },
   {
      .name    = "L1_DTLB_HIT",
      .desc    = "L1 DTLB Hit",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0x4d,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam14h_l1_dtlb_hit),
      .ngrp    = 1,
      .umasks  = amd64_fam14h_l1_dtlb_hit,
   },
   {
      .name    = "DCACHE_SW_PREFETCHES",
      .desc    = "Number of software prefetches that do not cuase an actual data cache refill",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0x52,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam14h_dcache_sw_prefetches),
      .ngrp    = 1,
      .umasks  = amd64_fam14h_dcache_sw_prefetches,
   },
   {
      .name    = "GLOBAL_TLB_FLUSHES",
      .desc    = "Global TLB Flushes",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0x54,
   },
   {
      .name    = "MEMORY_REQUESTS",
      .desc    = "Memory Requests by Type",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0x65,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam14h_memory_requests),
      .ngrp    = 1,
      .umasks  = amd64_fam14h_memory_requests,
   },
   {
      .name    = "MAB_REQUESTS",
      .desc    = "Number of L1 I-cache and D-cache misses per buffer. Average latency by combining with MAB_WAIT_CYCLES.",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0x68,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam14h_mab_requests),
      .ngrp    = 1,
      .umasks  = amd64_fam14h_mab_requests,
   },
   {
      .name    = "MAB_WAIT_CYCLES",
      .desc    = "Latency of L1 I-cache and D-cache misses per buffer. Average latency by combining with MAB_REQUESTS.",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0x69,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam14h_mab_requests),
      .ngrp    = 1,
      .umasks  = amd64_fam14h_mab_requests, /* identical to actual umasks list for this event */
   },
   {
      .name    = "SYSTEM_READ_RESPONSES",
      .desc    = "Northbridge Read Responses by Coherency State",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0x6c,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam14h_system_read_responses),
      .ngrp    = 1,
      .umasks  = amd64_fam14h_system_read_responses,
   },
   {
      .name    = "CPU_CLK_UNHALTED",
      .desc    = "CPU Clocks not Halted",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0x76,
   },
   {
      .name    = "REQUESTS_TO_L2",
      .desc    = "Requests to L2 Cache",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0x7d,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam14h_requests_to_l2),
      .ngrp    = 1,
      .umasks  = amd64_fam14h_requests_to_l2,
   },
   {
      .name    = "L2_CACHE_MISS",
      .desc    = "L2 Cache Misses",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0x7e,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam14h_l2_cache_miss),
      .ngrp    = 1,
      .umasks  = amd64_fam14h_l2_cache_miss,
   },
   {
      .name    = "L2_FILL_WRITEBACK",
      .desc    = "L2 Fill/Writeback",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0x7f,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam14h_l2_fill_writeback),
      .ngrp    = 1,
      .umasks  = amd64_fam14h_l2_fill_writeback,
   },
   {
      .name    = "INSTRUCTION_CACHE_FETCHES",
      .desc    = "Instruction Cache Fetches",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0x80,
   },
   {
      .name    = "INSTRUCTION_CACHE_MISSES",
      .desc    = "Instruction Cache Misses",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0x81,
   },
   {
      .name    = "INSTRUCTION_CACHE_REFILLS_FROM_L2",
      .desc    = "Instruction Cache Refills from L2",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0x82,
   },
   {
      .name    = "INSTRUCTION_CACHE_REFILLS_FROM_SYSTEM",
      .desc    = "Instruction Cache Refills from System",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0x83,
   },
   {
      .name    = "L1_ITLB_MISS_AND_L2_ITLB_MISS",
      .desc    = "L1 ITLB Miss and L2 ITLB Miss",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0x85,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam14h_l1_itlb_miss_and_l2_itlb_miss),
      .ngrp    = 1,
      .umasks  = amd64_fam14h_l1_itlb_miss_and_l2_itlb_miss,
   },
   {
      .name    = "INSTRUCTION_FETCH_STALL",
      .desc    = "Instruction Fetch Stall",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0x87,
   },
   {
      .name    = "RETURN_STACK_HITS",
      .desc    = "Return Stack Hits",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0x88,
   },
   {
      .name    = "RETURN_STACK_OVERFLOWS",
      .desc    = "Return Stack Overflows",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0x89,
   },
   {
      .name    = "INSTRUCTION_CACHE_VICTIMS",
      .desc    = "Instruction Cache Victims",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0x8b,
   },
   {
      .name    = "INSTRUCTION_CACHE_LINES_INVALIDATED",
      .desc    = "Instruction Cache Lines Invalidated",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0x8c,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam14h_instruction_cache_lines_invalidated),
      .ngrp    = 1,
      .umasks  = amd64_fam14h_instruction_cache_lines_invalidated,
   },
   {
      .name    = "ITLB_RELOADS",
      .desc    = "ITLB Reloads",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0x99,
   },
   {
      .name    = "ITLB_RELOADS_ABORTED",
      .desc    = "ITLB Reloads Aborted",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0x9a,
   },
   {
      .name    = "RETIRED_INSTRUCTIONS",
      .desc    = "Retired Instructions",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0xc0,
   },
   {
      .name    = "RETIRED_UOPS",
      .desc    = "Retired uops",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0xc1,
   },
   {
      .name    = "RETIRED_BRANCH_INSTRUCTIONS",
      .desc    = "Retired Branch Instructions",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0xc2,
   },
   {
      .name    = "RETIRED_MISPREDICTED_BRANCH_INSTRUCTIONS",
      .desc    = "Retired Mispredicted Branch Instructions",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0xc3,
   },
   {
      .name    = "RETIRED_TAKEN_BRANCH_INSTRUCTIONS",
      .desc    = "Retired Taken Branch Instructions",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0xc4,
   },
   {
      .name    = "RETIRED_TAKEN_BRANCH_INSTRUCTIONS_MISPREDICTED",
      .desc    = "Retired Taken Branch Instructions Mispredicted",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0xc5,
   },
   {
      .name    = "RETIRED_FAR_CONTROL_TRANSFERS",
      .desc    = "Retired Far Control Transfers",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0xc6,
   },
   {
      .name    = "RETIRED_BRANCH_RESYNCS",
      .desc    = "Retired Branch Resyncs",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0xc7,
   },
   {
      .name    = "RETIRED_NEAR_RETURNS",
      .desc    = "Retired Near Returns",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0xc8,
   },
   {
      .name    = "RETIRED_NEAR_RETURNS_MISPREDICTED",
      .desc    = "Retired Near Returns Mispredicted",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0xc9,
   },
   {
      .name    = "RETIRED_INDIRECT_BRANCHES_MISPREDICTED",
      .desc    = "Retired Indirect Branches Mispredicted",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0xca,
   },
   {
      .name    = "RETIRED_FLOATING_POINT_INSTRUCTIONS",
      .desc    = "Retired SSE/MMX/FP Instructions",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0xcb,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam14h_retired_floating_point_instructions),
      .ngrp    = 1,
      .umasks  = amd64_fam14h_retired_floating_point_instructions,
   },
   {
      .name    = "INTERRUPTS_MASKED_CYCLES",
      .desc    = "Interrupts-Masked Cycles",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0xcd,
   },
   {
      .name    = "INTERRUPTS_MASKED_CYCLES_WITH_INTERRUPT_PENDING",
      .desc    = "Interrupts-Masked Cycles with Interrupt Pending",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0xce,
   },
   {
      .name    = "INTERRUPTS_TAKEN",
      .desc    = "Interrupts Taken",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0xcf,
   },
   {
      .name    = "FPU_EXCEPTIONS",
      .desc    = "FPU Exceptions",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0xdb,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam14h_fpu_exceptions),
      .ngrp    = 1,
      .umasks  = amd64_fam14h_fpu_exceptions,
   },
   {
      .name    = "DR0_BREAKPOINT_MATCHES",
      .desc    = "DR0 Breakpoint Matches",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0xdc,
   },
   {
      .name    = "DR1_BREAKPOINT_MATCHES",
      .desc    = "DR1 Breakpoint Matches",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0xdd,
   },
   {
      .name    = "DR2_BREAKPOINT_MATCHES",
      .desc    = "DR2 Breakpoint Matches",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0xde,
   },
   {
      .name    = "DR3_BREAKPOINT_MATCHES",
      .desc    = "DR3 Breakpoint Matches",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0xdf,
   },
   {
      .name    = "DRAM_ACCESSES_PAGE",
      .desc    = "DRAM Accesses",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0xe0,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam14h_dram_accesses_page),
      .ngrp    = 1,
      .umasks  = amd64_fam14h_dram_accesses_page,
   },
   {
      .name    = "MEMORY_CONTROLLER_PAGE_TABLE",
      .desc    = "Number of page table events in the local DRAM controller",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0xe1,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam14h_memory_controller_page_table),
      .ngrp    = 1,
      .umasks  = amd64_fam14h_memory_controller_page_table,
   },
   {
      .name    = "MEMORY_CONTROLLER_SLOT_MISSES",
      .desc    = "Memory Controller DRAM Command Slots Missed",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0xe2,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam14h_memory_controller_slot_misses),
      .ngrp    = 1,
      .umasks  = amd64_fam14h_memory_controller_slot_misses,
   },
   {
      .name    = "MEMORY_CONTROLLER_RBD_QUEUE_EVENTS",
      .desc    = "Memory Controller Bypass Counter Saturation",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0xe4,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam14h_memory_controller_rbd_queue_events),
      .ngrp    = 1,
      .umasks  = amd64_fam14h_memory_controller_rbd_queue_events,
   },
   {
      .name    = "THERMAL_STATUS",
      .desc    = "Thermal Status",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0xe8,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam14h_thermal_status),
      .ngrp    = 1,
      .umasks  = amd64_fam14h_thermal_status,
   },
   {
      .name    = "CPU_IO_REQUESTS_TO_MEMORY_IO",
      .desc    = "CPU/IO Requests to Memory/IO",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0xe9,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam14h_cpu_io_requests_to_memory_io),
      .ngrp    = 1,
      .umasks  = amd64_fam14h_cpu_io_requests_to_memory_io,
   },
   {
      .name    = "CACHE_BLOCK",
      .desc    = "Cache Block Commands",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0xea,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam14h_cache_block),
      .ngrp    = 1,
      .umasks  = amd64_fam14h_cache_block,
   },
   {
      .name    = "SIZED_COMMANDS",
      .desc    = "Sized Commands",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0xeb,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam14h_sized_commands),
      .ngrp    = 1,
      .umasks  = amd64_fam14h_sized_commands,
   },
   {
      .name    = "PROBE",
      .desc    = "Probe Responses and Upstream Requests",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0xec,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam14h_probe),
      .ngrp    = 1,
      .umasks  = amd64_fam14h_probe,
   },
   {
      .name    = "DEV_EVENTS",
      .desc    = "DEV Events",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0xee,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam14h_dev_events),
      .ngrp    = 1,
      .umasks  = amd64_fam14h_dev_events,
   },
   {
      .name    = "MEMORY_CONTROLLER_REQUESTS",
      .desc    = "Memory Controller Requests",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0x1f0,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam14h_memory_controller_requests),
      .ngrp    = 1,
      .umasks  = amd64_fam14h_memory_controller_requests,
   },
   {
      .name    = "SIDEBAND_SIGNALS_SPECIAL_SIGNALS",
      .desc    = "Sideband signals and special cycles",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0x1e9,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam14h_sideband_signals_special_signals),
      .ngrp    = 1,
      .umasks  = amd64_fam14h_sideband_signals_special_signals,
   },
   {
      .name    = "INTERRUPT_EVENTS",
      .desc    = "Interrupt events",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0x1ea,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam14h_interrupt_events),
      .ngrp    = 1,
      .umasks  = amd64_fam14h_interrupt_events,
   },
   {
      .name    = "PDC_MISS",
      .desc    = "PDC miss",
      .modmsk  = AMD64_FAM10H_ATTRS,
      .code    = 0x162,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam14h_pdc_miss),
      .ngrp    = 1,
      .umasks  = amd64_fam14h_pdc_miss,
   },
};
