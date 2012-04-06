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
 * PMU: amd64_fam15h (AMD64 Fam15h Interlagos)
 *
 * Based on libpfm patch by Robert Richter <robert.richter@amd.com>:
 * Family 15h Microarchitecture performance monitor events
 *
 * History:
 *
 * Apr 29 2011 -- Robert Richter, robert.richter@amd.com:
 * Source: BKDG for AMD Family 15h Models 00h-0Fh Processors,
 * 42301, Rev 1.15, April 18, 2011
 *
 * Dec 09 2010 -- Robert Richter, robert.richter@amd.com:
 * Source: BIOS and Kernel Developer's Guide for the AMD Family 15h
 * Processors, Rev 0.90, May 18, 2010
 */

static const amd64_umask_t amd64_fam15h_dispatched_fpu_ops []=
{
   {
      .uname  = "OPS_PIPE0",
      .udesc  = "Total number uops assigned to Pipe 0",
      .ucode = 0x1,
   },
   {
      .uname  = "OPS_PIPE1",
      .udesc  = "Total number uops assigned to Pipe 1",
      .ucode = 0x2,
   },
   {
      .uname  = "OPS_PIPE2",
      .udesc  = "Total number uops assigned to Pipe 2",
      .ucode = 0x4,
   },
   {
      .uname  = "OPS_PIPE3",
      .udesc  = "Total number uops assigned to Pipe 3",
      .ucode = 0x8,
   },
   {
      .uname  = "OPS_DUAL_PIPE0",
      .udesc  = "Total number dual-pipe uops assigned to Pipe 0",
      .ucode = 0x10,
   },
   {
      .uname  = "OPS_DUAL_PIPE1",
      .udesc  = "Total number dual-pipe uops assigned to Pipe 1",
      .ucode = 0x20,
   },
   {
      .uname  = "OPS_DUAL_PIPE2",
      .udesc  = "Total number dual-pipe uops assigned to Pipe 2",
      .ucode = 0x40,
   },
   {
      .uname  = "OPS_DUAL_PIPE3",
      .udesc  = "Total number dual-pipe uops assigned to Pipe 3",
      .ucode = 0x80,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0xff,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam15h_retired_sse_ops []=
{
   {
      .uname  = "SINGLE_ADD_SUB_OPS",
      .udesc  = "Single-precision add/subtract FLOPS",
      .ucode = 0x1,
   },
   {
      .uname  = "SINGLE_MUL_OPS",
      .udesc  = "Single-precision multiply FLOPS",
      .ucode = 0x2,
   },
   {
      .uname  = "SINGLE_DIV_OPS",
      .udesc  = "Single-precision divide/square root FLOPS",
      .ucode = 0x4,
   },
   {
      .uname  = "SINGLE_MUL_ADD_OPS",
      .udesc  = "Single precision multiply-add FLOPS. Multiply-add counts as 2 FLOPS",
      .ucode = 0x8,
   },
   {
      .uname  = "DOUBLE_ADD_SUB_OPS",
      .udesc  = "Double precision add/subtract FLOPS",
      .ucode = 0x10,
   },
   {
      .uname  = "DOUBLE_MUL_OPS",
      .udesc  = "Double precision multiply FLOPS",
      .ucode = 0x20,
   },
   {
      .uname  = "DOUBLE_DIV_OPS",
      .udesc  = "Double precision divide/square root FLOPS",
      .ucode = 0x40,
   },
   {
      .uname  = "DOUBLE_MUL_ADD_OPS",
      .udesc  = "Double precision multiply-add FLOPS. Multiply-add counts as 2 FLOPS",
      .ucode = 0x80,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0xff,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam15h_move_scalar_optimization []=
{
   {
      .uname  = "SSE_MOVE_OPS",
      .udesc  = "Number of SSE Move Ops",
      .ucode = 0x1,
   },
   {
      .uname  = "SSE_MOVE_OPS_ELIM",
      .udesc  = "Number of SSE Move Ops eliminated",
      .ucode = 0x2,
   },
   {
      .uname  = "OPT_CAND",
      .udesc  = "Number of Ops that are candidates for optimization (Z-bit set or pass)",
      .ucode = 0x4,
   },
   {
      .uname  = "SCALAR_OPS_OPTIMIZED",
      .udesc  = "Number of Scalar ops optimized",
      .ucode = 0x8,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0xf,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam15h_retired_serializing_ops []=
{
   {
      .uname  = "SSE_RETIRED",
      .udesc  = "SSE bottom-executing uops retired",
      .ucode = 0x1,
   },
   {
      .uname  = "SSE_MISPREDICTED",
      .udesc  = "SSE control word mispredict traps due to mispredictions",
      .ucode = 0x2,
   },
   {
      .uname  = "X87_RETIRED",
      .udesc  = "X87 bottom-executing uops retired",
      .ucode = 0x4,
   },
   {
      .uname  = "X87_MISPREDICTED",
      .udesc  = "X87 control word mispredict traps due to mispredictions",
      .ucode = 0x8,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0xf,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam15h_segment_register_loads []=
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

static const amd64_umask_t amd64_fam15h_load_q_store_q_full []=
{
   {
      .uname  = "LOAD_QUEUE",
      .udesc  = "The number of cycles that the load buffer is full",
      .ucode = 0x1,
   },
   {
      .uname  = "STORE_QUEUE",
      .udesc  = "The number of cycles that the store buffer is full",
      .ucode = 0x2,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0x3,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam15h_locked_ops []=
{
   {
      .uname  = "EXECUTED",
      .udesc  = "Number of locked instructions executed",
      .ucode = 0x1,
   },
   {
      .uname  = "CYCLES_NON_SPECULATIVE_PHASE",
      .udesc  = "Number of cycles spent in non-speculative phase, excluding cache miss penalty",
      .ucode = 0x4,
   },
   {
      .uname  = "CYCLES_WAITING",
      .udesc  = "Number of cycles spent in non-speculative phase, including the cache miss penalty",
      .ucode = 0x8,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0xd,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam15h_cancelled_store_to_load []=
{
   {
      .uname  = "SIZE_ADDRESS_MISMATCHES",
      .udesc  = "Store is smaller than load or different starting byte but partial overlap",
      .ucode = 0x1,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0x1,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam15h_data_cache_misses []=
{
   {
      .uname  = "DC_MISS_STREAMING_STORE",
      .udesc  = "First data cache miss or streaming store to a 64B cache line",
      .ucode = 0x1,
   },
   {
      .uname  = "STREAMING_STORE",
      .udesc  = "First streaming store to a 64B cache line",
      .ucode = 0x2,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0x3,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam15h_data_cache_refills_from_l2_or_northbridge []=
{
   {
      .uname  = "GOOD",
      .udesc  = "Fill with good data. (Final valid status is valid)",
      .ucode = 0x1,
   },
   {
      .uname  = "INVALID",
      .udesc  = "Early valid status turned out to be invalid",
      .ucode = 0x2,
   },
   {
      .uname  = "POISON",
      .udesc  = "Fill with poison data",
      .ucode = 0x4,
   },
   {
      .uname  = "READ_ERROR",
      .udesc  = "Fill with read data error",
      .ucode = 0x8,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0xf,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam15h_unified_tlb_hit []=
{
   {
      .uname  = "4K_DATA",
      .udesc  = "4 KB unified TLB hit for data",
      .ucode = 0x1,
   },
   {
      .uname  = "2M_DATA",
      .udesc  = "2 MB unified TLB hit for data",
      .ucode = 0x2,
   },
   {
      .uname  = "1G_DATA",
      .udesc  = "1 GB unified TLB hit for data",
      .ucode = 0x4,
   },
   {
      .uname  = "4K_INST",
      .udesc  = "4 KB unified TLB hit for instruction",
      .ucode = 0x10,
   },
   {
      .uname  = "2M_INST",
      .udesc  = "2 MB unified TLB hit for instruction",
      .ucode = 0x20,
   },
   {
      .uname  = "1G_INST",
      .udesc  = "1 GB unified TLB hit for instruction",
      .ucode = 0x40,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0x77,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam15h_unified_tlb_miss []=
{
   {
      .uname  = "4K_DATA",
      .udesc  = "4 KB unified TLB miss for data",
      .ucode = 0x1,
   },
   {
      .uname  = "2M_DATA",
      .udesc  = "2 MB unified TLB miss for data",
      .ucode = 0x2,
   },
   {
      .uname  = "1GB_DATA",
      .udesc  = "1 GB unified TLB miss for data",
      .ucode = 0x4,
   },
   {
      .uname  = "4K_INST",
      .udesc  = "4 KB unified TLB miss for instruction",
      .ucode = 0x10,
   },
   {
      .uname  = "2M_INST",
      .udesc  = "2 MB unified TLB miss for instruction",
      .ucode = 0x20,
   },
   {
      .uname  = "1G_INST",
      .udesc  = "1 GB unified TLB miss for instruction",
      .ucode = 0x40,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0x77,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam15h_prefetch_instructions_dispatched []=
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

static const amd64_umask_t amd64_fam15h_ineffective_sw_prefetches []=
{
   {
      .uname  = "SW_PREFETCH_HIT_IN_L1",
      .udesc  = "Software prefetch hit in the L1",
      .ucode = 0x1,
   },
   {
      .uname  = "SW_PREFETCH_HIT_IN_L2",
      .udesc  = "Software prefetch hit in the L2",
      .ucode = 0x8,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0x9,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam15h_memory_requests []=
{
   {
      .uname  = "NON_CACHEABLE",
      .udesc  = "Requests to non-cacheable (UC) memory",
      .ucode = 0x1,
   },
   {
      .uname  = "WRITE_COMBINING",
      .udesc  = "Requests to non-cacheable (WC, but not WC+/SS) memory",
      .ucode = 0x2,
   },
   {
      .uname  = "STREAMING_STORE",
      .udesc  = "Requests to non-cacheable (WC+/SS, but not WC) memory",
      .ucode = 0x80,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0x83,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam15h_data_prefetcher []=
{
   {
      .uname  = "ATTEMPTED",
      .udesc  = "Prefetch attempts",
      .ucode = 0x2,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0x2,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam15h_mab_reqs []=
{
   {
      .uname  = "BUFFER_BIT_0",
      .udesc  = "Buffer entry index bit 0",
      .ucode = 0x1,
   },
   {
      .uname  = "BUFFER_BIT_1",
      .udesc  = "Buffer entry index bit 1",
      .ucode = 0x2,
   },
   {
      .uname  = "BUFFER_BIT_2",
      .udesc  = "Buffer entry index bit 2",
      .ucode = 0x4,
   },
   {
      .uname  = "BUFFER_BIT_3",
      .udesc  = "Buffer entry index bit 3",
      .ucode = 0x8,
   },
   {
      .uname  = "BUFFER_BIT_4",
      .udesc  = "Buffer entry index bit 4",
      .ucode = 0x10,
   },
   {
      .uname  = "BUFFER_BIT_5",
      .udesc  = "Buffer entry index bit 5",
      .ucode = 0x20,
   },
   {
      .uname  = "BUFFER_BIT_6",
      .udesc  = "Buffer entry index bit 6",
      .ucode = 0x40,
   },
   {
      .uname  = "BUFFER_BIT_7",
      .udesc  = "Buffer entry index bit 7",
      .ucode = 0x80,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0xff,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam15h_system_read_responses []=
{
   {
      .uname  = "EXCLUSIVE",
      .udesc  = "Exclusive",
      .ucode = 0x1,
   },
   {
      .uname  = "MODIFIED",
      .udesc  = "Modified (D18F0x68[ATMModeEn]==0), Modified written (D18F0x68[ATMModeEn]==1)",
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
      .uname  = "MODIFIED_UNWRITTEN",
      .udesc  = "Modified unwritten",
      .ucode = 0x20,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0x3f,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam15h_octword_write_transfers []=
{
   {
      .uname  = "OCTWORD_WRITE_TRANSFER",
      .udesc  = "OW write transfer",
      .ucode = 0x1,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0x1,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam15h_requests_to_l2[]=
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
      .uname  = "TLB_WALK",
      .udesc  = "TLB fill (page table walks)",
      .ucode = 0x4,
   },
   {
      .uname  = "SNOOP",
      .udesc  = "NB probe request",
      .ucode = 0x8,
   },
   {
      .uname  = "CANCELLED",
      .udesc  = "Canceled request",
      .ucode = 0x10,
   },
   {
      .uname  = "PREFETCHER",
      .udesc  = "L2 cache prefetcher request",
      .ucode = 0x40,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0x5f,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam15h_l2_cache_miss []=
{
   {
      .uname  = "INSTRUCTIONS",
      .udesc  = "IC fill",
      .ucode = 0x1,
   },
   {
      .uname  = "DATA",
      .udesc  = "DC fill (includes possible replays, whereas PMCx041 does not)",
      .ucode = 0x2,
   },
   {
      .uname  = "TLB_WALK",
      .udesc  = "TLB page table walk",
      .ucode = 0x4,
   },
   {
      .uname  = "PREFETCHER",
      .udesc  = "L2 Cache Prefetcher request",
      .ucode = 0x10,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0x17,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam15h_l2_cache_fill_writeback []=
{
   {
      .uname  = "L2_FILLS",
      .udesc  = "L2 fills from system",
      .ucode = 0x1,
   },
   {
      .uname  = "L2_WRITEBACKS",
      .udesc  = "L2 Writebacks to system (Clean and Dirty)",
      .ucode = 0x2,
   },
   {
      .uname  = "L2_WRITEBACKS_CLEAN",
      .udesc  = "L2 Clean Writebacks to system",
      .ucode = 0x4,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0x7,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam15h_page_splintering []=
{
   {
      .uname  = "GUEST_LARGER",
      .udesc  = "Guest page size is larger than host page size when nested paging is enabled",
      .ucode = 0x1,
   },
   {
      .uname  = "MTRR_MISMATCH",
      .udesc  = "Splintering due to MTRRs, IORRs, APIC, TOMs or other special address region",
      .ucode = 0x2,
   },
   {
      .uname  = "HOST_LARGER",
      .udesc  = "Host page size is larger than the guest page size",
      .ucode = 0x4,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0x7,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam15h_l1_itlb_miss_and_l2_itlb_miss []=
{
   {
      .uname  = "4K_PAGE_FETCHES",
      .udesc  = "Instruction fetches to a 4 KB page",
      .ucode = 0x1,
   },
   {
      .uname  = "2M_PAGE_FETCHES",
      .udesc  = "Instruction fetches to a 2 MB page",
      .ucode = 0x2,
   },
   {
      .uname  = "1G_PAGE_FETCHES",
      .udesc  = "Instruction fetches to a 1 GB page",
      .ucode = 0x4,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0x7,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam15h_instruction_cache_invalidated []=
{
   {
      .uname  = "NON_SMC_PROBE_MISS",
      .udesc  = "Non-SMC invalidating probe that missed on in-flight instructions",
      .ucode = 0x1,
   },
   {
      .uname  = "NON_SMC_PROBE_HIT",
      .udesc  = "Non-SMC invalidating probe that hit on in-flight instructions",
      .ucode = 0x2,
   },
   {
      .uname  = "SMC_PROBE_MISS",
      .udesc  = "SMC invalidating probe that missed on in-flight instructions",
      .ucode = 0x4,
   },
   {
      .uname  = "SMC_PROBE_HIT",
      .udesc  = "SMC invalidating probe that hit on in-flight instructions",
      .ucode = 0x8,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0xf,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam15h_retired_mmx_fp_instructions []=
{
   {
      .uname  = "X87",
      .udesc  = "X87 instructions",
      .ucode = 0x1,
   },
   {
      .uname  = "MMX",
      .udesc  = "MMX (tm) instructions",
      .ucode = 0x2,
   },
   {
      .uname  = "SSE",
      .udesc  = "SSE instructions (SSE,SSE2,SSE3,SSSE3,SSE4A,SSE4.1,SSE4.2,AVX,XOP,FMA4)",
      .ucode = 0x4,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0x7,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam15h_fpu_exceptions []=
{
   {
      .uname  = "TOTAL_FAULTS",
      .udesc  = "Total microfaults",
      .ucode = 0x1,
   },
   {
      .uname  = "TOTAL_TRAPS",
      .udesc  = "Total microtraps",
      .ucode = 0x2,
   },
   {
      .uname  = "INT2EXT_FAULTS",
      .udesc  = "Int2Ext faults",
      .ucode = 0x4,
   },
   {
      .uname  = "EXT2INT_FAULTS",
      .udesc  = "Ext2Int faults",
      .ucode = 0x8,
   },
   {
      .uname  = "BYPASS_FAULTS",
      .udesc  = "Bypass faults",
      .ucode = 0x10,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0x1f,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam15h_ibs_ops_tagged []=
{
   {
      .uname  = "TAGGED",
      .udesc  = "Number of ops tagged by IBS",
      .ucode = 0x1,
   },
   {
      .uname  = "RETIRED",
      .udesc  = "Number of ops tagged by IBS that retired",
      .ucode = 0x2,
   },
   {
      .uname  = "IGNORED",
      .udesc  = "Number of times an op could not be tagged by IBS because of a previous tagged op that has not retired",
      .ucode = 0x4,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0x7,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam15h_ls_dispatch []=
{
   {
      .uname  = "LOADS",
      .udesc  = "Loads",
      .ucode = 0x1,
   },
   {
      .uname  = "STORES",
      .udesc  = "Stores",
      .ucode = 0x2,
   },
   {
      .uname  = "LOAD_OP_STORES",
      .udesc  = "Load-op-Stores",
      .ucode = 0x4,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0x7,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_umask_t amd64_fam15h_l2_prefetcher_trigger_events []=
{
   {
      .uname  = "LOAD_L1_MISS_SEEN_BY_PREFETCHER",
      .udesc  = "Load L1 miss seen by prefetcher",
      .ucode = 0x1,
   },
   {
      .uname  = "STORE_L1_MISS_SEEN_BY_PREFETCHER",
      .udesc  = "Store L1 miss seen by prefetcher",
      .ucode = 0x2,
   },
   {
      .uname  = "ALL",
      .udesc  = "All sub-events selected",
      .ucode = 0x3,
      .uflags= AMD64_FL_NCOMBO | AMD64_FL_DFL,
   },
};

static const amd64_entry_t amd64_fam15h_pe []=
{
   {
      .name    = "DISPATCHED_FPU_OPS",
      .desc    = "FPU Pipe Assignment",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0x0,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam15h_dispatched_fpu_ops),
      .ngrp    = 1,
      .umasks  = amd64_fam15h_dispatched_fpu_ops,
   },
   {
      .name    = "CYCLES_FPU_EMPTY",
      .desc    = "FP Scheduler Empty",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0x1,
   },
   {
      .name    = "RETIRED_SSE_OPS",
      .desc    = "Retired SSE/BNI Ops",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0x3,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam15h_retired_sse_ops),
      .ngrp    = 1,
      .umasks  = amd64_fam15h_retired_sse_ops,
   },
   {
      .name    = "MOVE_SCALAR_OPTIMIZATION",
      .desc    = "Number of Move Elimination and Scalar Op Optimization",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0x4,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam15h_move_scalar_optimization),
      .ngrp    = 1,
      .umasks  = amd64_fam15h_move_scalar_optimization,
   },
   {
      .name    = "RETIRED_SERIALIZING_OPS",
      .desc    = "Retired Serializing Ops",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0x5,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam15h_retired_serializing_ops),
      .ngrp    = 1,
      .umasks  = amd64_fam15h_retired_serializing_ops,
   },
   {
      .name    = "BOTTOM_EXECUTE_OP",
      .desc    = "Number of Cycles that a Bottom-Execute uop is in the FP Scheduler",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0x6,
   },
   {
      .name    = "SEGMENT_REGISTER_LOADS",
      .desc    = "Segment Register Loads",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0x20,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam15h_segment_register_loads),
      .ngrp    = 1,
      .umasks  = amd64_fam15h_segment_register_loads,
   },
   {
      .name    = "PIPELINE_RESTART_DUE_TO_SELF_MODIFYING_CODE",
      .desc    = "Pipeline Restart Due to Self-Modifying Code",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0x21,
   },
   {
      .name    = "PIPELINE_RESTART_DUE_TO_PROBE_HIT",
      .desc    = "Pipeline Restart Due to Probe Hit",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0x22,
   },
   {
      .name    = "LOAD_Q_STORE_Q_FULL",
      .desc    = "Load Queue/Store Queue Full",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0x23,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam15h_load_q_store_q_full),
      .ngrp    = 1,
      .umasks  = amd64_fam15h_load_q_store_q_full,
   },
   {
      .name    = "LOCKED_OPS",
      .desc    = "Locked Operations",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0x24,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam15h_locked_ops),
      .ngrp    = 1,
      .umasks  = amd64_fam15h_locked_ops,
   },
   {
      .name    = "RETIRED_CLFLUSH_INSTRUCTIONS",
      .desc    = "Retired CLFLUSH Instructions",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0x26,
   },
   {
      .name    = "RETIRED_CPUID_INSTRUCTIONS",
      .desc    = "Retired CPUID Instructions",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0x27,
   },
   {
      .name    = "CANCELLED_STORE_TO_LOAD",
      .desc    = "Canceled Store to Load Forward Operations",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0x2a,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam15h_cancelled_store_to_load),
      .ngrp    = 1,
      .umasks  = amd64_fam15h_cancelled_store_to_load,
   },
   {
      .name    = "SMIS_RECEIVED",
      .desc    = "SMIs Received",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0x2b,
   },
   {
      .name    = "DATA_CACHE_ACCESSES",
      .desc    = "Data Cache Accesses",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0x40,
   },
   {
      .name    = "DATA_CACHE_MISSES",
      .desc    = "Data Cache Misses",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0x41,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam15h_data_cache_misses),
      .ngrp    = 1,
      .umasks  = amd64_fam15h_data_cache_misses,
   },
   {
      .name    = "DATA_CACHE_REFILLS_FROM_L2_OR_NORTHBRIDGE",
      .desc    = "Data Cache Refills from L2 or System",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0x42,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam15h_data_cache_refills_from_l2_or_northbridge),
      .ngrp    = 1,
      .umasks  = amd64_fam15h_data_cache_refills_from_l2_or_northbridge,
   },
   {
      .name    = "DATA_CACHE_REFILLS_FROM_NORTHBRIDGE",
      .desc    = "Data Cache Refills from System",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0x43,
   },
   {
      .name    = "UNIFIED_TLB_HIT",
      .desc    = "Unified TLB Hit",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0x45,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam15h_unified_tlb_hit),
      .ngrp    = 1,
      .umasks  = amd64_fam15h_unified_tlb_hit,
   },
   {
      .name    = "UNIFIED_TLB_MISS",
      .desc    = "Unified TLB Miss",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0x46,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam15h_unified_tlb_miss),
      .ngrp    = 1,
      .umasks  = amd64_fam15h_unified_tlb_miss,
   },
   {
      .name    = "MISALIGNED_ACCESSES",
      .desc    = "Misaligned Accesses",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0x47,
   },
   {
      .name    = "PREFETCH_INSTRUCTIONS_DISPATCHED",
      .desc    = "Prefetch Instructions Dispatched",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0x4b,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam15h_prefetch_instructions_dispatched),
      .ngrp    = 1,
      .umasks  = amd64_fam15h_prefetch_instructions_dispatched,
   },
   {
      .name    = "INEFFECTIVE_SW_PREFETCHES",
      .desc    = "Ineffective Software Prefetches",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0x52,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam15h_ineffective_sw_prefetches),
      .ngrp    = 1,
      .umasks  = amd64_fam15h_ineffective_sw_prefetches,
   },
   {
      .name    = "MEMORY_REQUESTS",
      .desc    = "Memory Requests by Type",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0x65,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam15h_memory_requests),
      .ngrp    = 1,
      .umasks  = amd64_fam15h_memory_requests,
   },
   {
      .name    = "DATA_PREFETCHER",
      .desc    = "Data Prefetcher",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0x67,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam15h_data_prefetcher),
      .ngrp    = 1,
      .umasks  = amd64_fam15h_data_prefetcher,
   },
   {
      .name    = "MAB_REQS",
      .desc    = "MAB Requests",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0x68,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam15h_mab_reqs),
      .ngrp    = 1,
      .umasks  = amd64_fam15h_mab_reqs,
   },
   {
      .name    = "MAB_WAIT",
      .desc    = "MAB Wait Cycles",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0x69,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam15h_mab_reqs),
      .ngrp    = 1,
      .umasks  = amd64_fam15h_mab_reqs, /* identical to actual umasks list for this event */
   },
   {
      .name    = "SYSTEM_READ_RESPONSES",
      .desc    = "Response From System on Cache Refills",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0x6c,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam15h_system_read_responses),
      .ngrp    = 1,
      .umasks  = amd64_fam15h_system_read_responses,
   },
   {
      .name    = "OCTWORD_WRITE_TRANSFERS",
      .desc    = "Octwords Written to System",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0x6d,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam15h_octword_write_transfers),
      .ngrp    = 1,
      .umasks  = amd64_fam15h_octword_write_transfers,
   },
   {
      .name    = "CPU_CLK_UNHALTED",
      .desc    = "CPU Clocks not Halted",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0x76,
   },
   {
      .name    = "REQUESTS_TO_L2",
      .desc    = "Requests to L2 Cache",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0x7d,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam15h_requests_to_l2),
      .ngrp    = 1,
      .umasks  = amd64_fam15h_requests_to_l2,
   },
   {
      .name    = "L2_CACHE_MISS",
      .desc    = "L2 Cache Misses",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0x7e,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam15h_l2_cache_miss),
      .ngrp    = 1,
      .umasks  = amd64_fam15h_l2_cache_miss,
   },
   {
      .name    = "L2_CACHE_FILL_WRITEBACK",
      .desc    = "L2 Fill/Writeback",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0x7f,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam15h_l2_cache_fill_writeback),
      .ngrp    = 1,
      .umasks  = amd64_fam15h_l2_cache_fill_writeback,
   },
   {
      .name    = "PAGE_SPLINTERING",
      .desc    = "Page Splintering",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0x165,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam15h_page_splintering),
      .ngrp    = 1,
      .umasks  = amd64_fam15h_page_splintering,
   },
   {
      .name    = "INSTRUCTION_CACHE_FETCHES",
      .desc    = "Instruction Cache Fetches",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0x80,
   },
   {
      .name    = "INSTRUCTION_CACHE_MISSES",
      .desc    = "Instruction Cache Misses",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0x81,
   },
   {
      .name    = "INSTRUCTION_CACHE_REFILLS_FROM_L2",
      .desc    = "Instruction Cache Refills from L2",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0x82,
   },
   {
      .name    = "INSTRUCTION_CACHE_REFILLS_FROM_SYSTEM",
      .desc    = "Instruction Cache Refills from System",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0x83,
   },
   {
      .name    = "L1_ITLB_MISS_AND_L2_ITLB_HIT",
      .desc    = "L1 ITLB Miss, L2 ITLB Hit",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0x84,
   },
   {
      .name    = "L1_ITLB_MISS_AND_L2_ITLB_MISS",
      .desc    = "L1 ITLB Miss, L2 ITLB Miss",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0x85,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam15h_l1_itlb_miss_and_l2_itlb_miss),
      .ngrp    = 1,
      .umasks  = amd64_fam15h_l1_itlb_miss_and_l2_itlb_miss,
   },
   {
      .name    = "PIPELINE_RESTART_DUE_TO_INSTRUCTION_STREAM_PROBE",
      .desc    = "Pipeline Restart Due to Instruction Stream Probe",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0x86,
   },
   {
      .name    = "INSTRUCTION_FETCH_STALL",
      .desc    = "Instruction Fetch Stall",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0x87,
   },
   {
      .name    = "RETURN_STACK_HITS",
      .desc    = "Return Stack Hits",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0x88,
   },
   {
      .name    = "RETURN_STACK_OVERFLOWS",
      .desc    = "Return Stack Overflows",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0x89,
   },
   {
      .name    = "INSTRUCTION_CACHE_VICTIMS",
      .desc    = "Instruction Cache Victims",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0x8b,
   },
   {
      .name    = "INSTRUCTION_CACHE_INVALIDATED",
      .desc    = "Instruction Cache Lines Invalidated",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0x8c,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam15h_instruction_cache_invalidated),
      .ngrp    = 1,
      .umasks  = amd64_fam15h_instruction_cache_invalidated,
   },
   {
      .name    = "ITLB_RELOADS",
      .desc    = "ITLB Reloads",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0x99,
   },
   {
      .name    = "ITLB_RELOADS_ABORTED",
      .desc    = "ITLB Reloads Aborted",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0x9a,
   },
   {
      .name    = "RETIRED_INSTRUCTIONS",
      .desc    = "Retired Instructions",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0xc0,
   },
   {
      .name    = "RETIRED_UOPS",
      .desc    = "Retired uops",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0xc1,
   },
   {
      .name    = "RETIRED_BRANCH_INSTRUCTIONS",
      .desc    = "Retired Branch Instructions",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0xc2,
   },
   {
      .name    = "RETIRED_MISPREDICTED_BRANCH_INSTRUCTIONS",
      .desc    = "Retired Mispredicted Branch Instructions",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0xc3,
   },
   {
      .name    = "RETIRED_TAKEN_BRANCH_INSTRUCTIONS",
      .desc    = "Retired Taken Branch Instructions",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0xc4,
   },
   {
      .name    = "RETIRED_TAKEN_BRANCH_INSTRUCTIONS_MISPREDICTED",
      .desc    = "Retired Taken Branch Instructions Mispredicted",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0xc5,
   },
   {
      .name    = "RETIRED_FAR_CONTROL_TRANSFERS",
      .desc    = "Retired Far Control Transfers",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0xc6,
   },
   {
      .name    = "RETIRED_BRANCH_RESYNCS",
      .desc    = "Retired Branch Resyncs",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0xc7,
   },
   {
      .name    = "RETIRED_NEAR_RETURNS",
      .desc    = "Retired Near Returns",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0xc8,
   },
   {
      .name    = "RETIRED_NEAR_RETURNS_MISPREDICTED",
      .desc    = "Retired Near Returns Mispredicted",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0xc9,
   },
   {
      .name    = "RETIRED_INDIRECT_BRANCHES_MISPREDICTED",
      .desc    = "Retired Indirect Branches Mispredicted",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0xca,
   },
   {
      .name    = "RETIRED_MMX_FP_INSTRUCTIONS",
      .desc    = "Retired MMX/FP Instructions",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0xcb,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam15h_retired_mmx_fp_instructions),
      .ngrp    = 1,
      .umasks  = amd64_fam15h_retired_mmx_fp_instructions,
   },
   {
      .name    = "INTERRUPTS_MASKED_CYCLES",
      .desc    = "Interrupts-Masked Cycles",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0xcd,
   },
   {
      .name    = "INTERRUPTS_MASKED_CYCLES_WITH_INTERRUPT_PENDING",
      .desc    = "Interrupts-Masked Cycles with Interrupt Pending",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0xce,
   },
   {
      .name    = "INTERRUPTS_TAKEN",
      .desc    = "Interrupts Taken",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0xcf,
   },
   {
      .name    = "DECODER_EMPTY",
      .desc    = "Decoder Empty",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0xd0,
   },
   {
      .name    = "DISPATCH_STALLS",
      .desc    = "Dispatch Stalls",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0xd1,
   },
   {
      .name    = "DISPATCH_STALL_FOR_SERIALIZATION",
      .desc    = "Microsequencer Stall due to Serialization",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0xd3,
   },
   {
      .name    = "DISPATCH_STALL_FOR_RETIRE_QUEUE_FULL",
      .desc    = "Dispatch Stall for Instruction Retire Q Full",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0xd5,
   },
   {
      .name    = "DISPATCH_STALL_FOR_INT_SCHED_QUEUE_FULL",
      .desc    = "Dispatch Stall for Integer Scheduler Queue Full",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0xd6,
   },
   {
      .name    = "DISPATCH_STALL_FOR_FPU_FULL",
      .desc    = "Dispatch Stall for FP Scheduler Queue Full",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0xd7,
   },
   {
      .name    = "DISPATCH_STALL_FOR_LDQ_FULL",
      .desc    = "Dispatch Stall for LDQ Full",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0xd8,
   },
   {
      .name    = "MICROSEQ_STALL_WAITING_FOR_ALL_QUIET",
      .desc    = "Microsequencer Stall Waiting for All Quiet",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0xd9,
   },
   {
      .name    = "FPU_EXCEPTIONS",
      .desc    = "FPU Exceptions",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0xdb,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam15h_fpu_exceptions),
      .ngrp    = 1,
      .umasks  = amd64_fam15h_fpu_exceptions,
   },
   {
      .name    = "DR0_BREAKPOINTS",
      .desc    = "DR0 Breakpoint Match",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0xdc,
   },
   {
      .name    = "DR1_BREAKPOINTS",
      .desc    = "DR1 Breakpoint Match",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0xdd,
   },
   {
      .name    = "DR2_BREAKPOINTS",
      .desc    = "DR2 Breakpoint Match",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0xde,
   },
   {
      .name    = "DR3_BREAKPOINTS",
      .desc    = "DR3 Breakpoint Match",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0xdf,
   },
   {
      .name    = "IBS_OPS_TAGGED",
      .desc    = "Tagged IBS Ops",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0x1cf,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam15h_ibs_ops_tagged),
      .ngrp    = 1,
      .umasks  = amd64_fam15h_ibs_ops_tagged,
   },
   {
      .name    = "LS_DISPATCH",
      .desc    = "LS Dispatch",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0x29,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam15h_ls_dispatch),
      .ngrp    = 1,
      .umasks  = amd64_fam15h_ls_dispatch,
   },
   {
      .name    = "EXECUTED_CLFLUSH_INSTRUCTIONS",
      .desc    = "Executed CLFLUSH Instructions",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0x30,
   },
   {
      .name    = "L2_PREFETCHER_TRIGGER_EVENTS",
      .desc    = "L2 Prefetcher Trigger Events",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0x16c,
      .numasks = LIBPFM_ARRAY_SIZE (amd64_fam15h_l2_prefetcher_trigger_events),
      .ngrp    = 1,
      .umasks  = amd64_fam15h_l2_prefetcher_trigger_events,
   },
   {
      .name    = "DISPATCH_STALL_FOR_STQ_FULL",
      .desc    = "Dispatch Stall for STQ Full",
      .modmsk  = AMD64_FAM15H_ATTRS,
      .code    = 0x1d8,
   },
};