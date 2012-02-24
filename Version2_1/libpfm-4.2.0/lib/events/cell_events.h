/*
 * Copyright (c) 2007 TOSHIBA CORPORATION based on code from
 * Copyright (c) 2001-2006 Hewlett-Packard Development Company, L.P.
 * Contributed by Stephane Eranian <eranian@hpl.hp.com>
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
 */

static pme_cell_entry_t cell_pe [] =
{
   {
      .pme_name = "CYCLES",
      .pme_desc = "CPU cycles",
      .pme_code = 0x0, /* 0 */
      .pme_enable_word = WORD_NONE,
      .pme_freq = PFM_CELL_PME_FREQ_PPU_MFC,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "BRANCH_COMMIT_TH0",
      .pme_desc = "Branch instruction committed.",
      .pme_code = 0x834, /* 2100 */
      .pme_enable_word = WORD_0_AND_1,
      .pme_freq = PFM_CELL_PME_FREQ_PPU_MFC,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "BRANCH_FLUSH_TH0",
      .pme_desc = "Branch instruction that caused a misprediction flush is committed. Branch misprediction includes",
      .pme_code = 0x835, /* 2101 */
      .pme_enable_word = WORD_0_AND_1,
      .pme_freq = PFM_CELL_PME_FREQ_PPU_MFC,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "INST_BUFF_EMPTY_TH0",
      .pme_desc = "Instruction buffer empty.",
      .pme_code = 0x836, /* 2102 */
      .pme_enable_word = WORD_0_AND_1,
      .pme_freq = PFM_CELL_PME_FREQ_PPU_MFC,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "INST_ERAT_MISS_TH0",
      .pme_desc = "Instruction effective-address-to-real-address translation (I-ERAT) miss.",
      .pme_code = 0x837, /* 2103 */
      .pme_enable_word = WORD_0_AND_1,
      .pme_freq = PFM_CELL_PME_FREQ_PPU_MFC,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "L1_ICACHE_MISS_CYCLES_TH0",
      .pme_desc = "L1 Instruction cache miss cycles. Counts the cycles from the miss event until the returned instruction is dispatched or cancelled due to branch misprediction, completion restart, or exceptions.",
      .pme_code = 0x838, /* 2104 */
      .pme_enable_word = WORD_0_AND_1,
      .pme_freq = PFM_CELL_PME_FREQ_PPU_MFC,
      .pme_type = COUNT_TYPE_BOTH_TYPE,
   },
   {
      .pme_name = "DISPATCH_BLOCKED_TH0",
      .pme_desc = "Valid instruction available for dispatch, but dispatch is blocked.",
      .pme_code = 0x83a, /* 2106 */
      .pme_enable_word = WORD_0_AND_1,
      .pme_freq = PFM_CELL_PME_FREQ_PPU_MFC,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "INST_FLUSH_TH0",
      .pme_desc = "Instruction in pipeline stage EX7 causes a flush.",
      .pme_code = 0x83d, /* 2109 */
      .pme_enable_word = WORD_0_AND_1,
      .pme_freq = PFM_CELL_PME_FREQ_PPU_MFC,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "PPC_INST_COMMIT_TH0",
      .pme_desc = "Two PowerPC instructions committed. For microcode sequences, only the last microcode operation is counted. Committed instructions are counted two at a time. If only one instruction has committed for a given cycle, this event will not be raised until another instruction has been committed in a future cycle.",
      .pme_code = 0x83f, /* 2111 */
      .pme_enable_word = WORD_0_AND_1,
      .pme_freq = PFM_CELL_PME_FREQ_PPU_MFC,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "BRANCH_COMMIT_TH1",
      .pme_desc = "Branch instruction committed.",
      .pme_code = 0x847, /* 2119 */
      .pme_enable_word = WORD_0_AND_1,
      .pme_freq = PFM_CELL_PME_FREQ_PPU_MFC,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "BRANCH_FLUSH_TH1",
      .pme_desc = "Branch instruction that caused a misprediction flush is committed. Branch misprediction includes",
      .pme_code = 0x848, /* 2120 */
      .pme_enable_word = WORD_0_AND_1,
      .pme_freq = PFM_CELL_PME_FREQ_PPU_MFC,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "INST_BUFF_EMPTY_TH1",
      .pme_desc = "Instruction buffer empty.",
      .pme_code = 0x849, /* 2121 */
      .pme_enable_word = WORD_0_AND_1,
      .pme_freq = PFM_CELL_PME_FREQ_PPU_MFC,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "INST_ERAT_MISS_TH1",
      .pme_desc = "Instruction effective-address-to-real-address translation (I-ERAT) miss.",
      .pme_code = 0x84a, /* 2122 */
      .pme_enable_word = WORD_0_AND_1,
      .pme_freq = PFM_CELL_PME_FREQ_PPU_MFC,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "L1_ICACHE_MISS_CYCLES_TH1",
      .pme_desc = "L1 Instruction cache miss cycles. Counts the cycles from the miss event until the returned instruction is dispatched or cancelled due to branch misprediction, completion restart, or exceptions.",
      .pme_code = 0x84b, /* 2123 */
      .pme_enable_word = WORD_0_AND_1,
      .pme_freq = PFM_CELL_PME_FREQ_PPU_MFC,
      .pme_type = COUNT_TYPE_BOTH_TYPE,
   },
   {
      .pme_name = "DISPATCH_BLOCKED_TH1",
      .pme_desc = "Valid instruction available for dispatch, but dispatch is blocked.",
      .pme_code = 0x84d, /* 2125 */
      .pme_enable_word = WORD_0_AND_1,
      .pme_freq = PFM_CELL_PME_FREQ_PPU_MFC,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "INST_FLUSH_TH1",
      .pme_desc = "Instruction in pipeline stage EX7 causes a flush.",
      .pme_code = 0x850, /* 2128 */
      .pme_enable_word = WORD_0_AND_1,
      .pme_freq = PFM_CELL_PME_FREQ_PPU_MFC,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "PPC_INST_COMMIT_TH1",
      .pme_desc = "Two PowerPC instructions committed. For microcode sequences, only the last microcode operation is counted. Committed instructions are counted two at a time. If only one instruction has committed for a given cycle, this event will not be raised until another instruction has been committed in a future cycle.",
      .pme_code = 0x852, /* 2130 */
      .pme_enable_word = WORD_0_AND_1,
      .pme_freq = PFM_CELL_PME_FREQ_PPU_MFC,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "DATA_ERAT_MISS_TH0",
      .pme_desc = "Data effective-address-to-real-address translation (D-ERAT) miss. Not speculative.",
      .pme_code = 0x89a, /* 2202 */
      .pme_enable_word = WORD_0_AND_1,
      .pme_freq = PFM_CELL_PME_FREQ_PPU_MFC,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "ST_REQ_TH0",
      .pme_desc = "Store request counted at the L2 interface. Counts microcoded PPE sequences more than once. (Thread 0 and 1)",
      .pme_code = 0x89b, /* 2203 */
      .pme_enable_word = WORD_0_AND_1,
      .pme_freq = PFM_CELL_PME_FREQ_PPU_MFC,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "LD_VALID_TH0",
      .pme_desc = "Load valid at a particular pipe stage. Speculative, since flushed operations are counted as well. Counts microcoded PPE sequences more than once. Misaligned flushes might be counted the first time as well. Load operations include all loads that read data from the cache, dcbt and dcbtst. Does not include load Vector/SIMD multimedia extension pattern instructions.",
      .pme_code = 0x89c, /* 2204 */
      .pme_enable_word = WORD_0_AND_1,
      .pme_freq = PFM_CELL_PME_FREQ_PPU_MFC,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "L1_DCACHE_MISS_TH0",
      .pme_desc = "L1 D-cache load miss. Pulsed when there is a miss request that has a tag miss but not an ERAT miss. Speculative, since flushed operations are counted as well.",
      .pme_code = 0x89d, /* 2205 */
      .pme_enable_word = WORD_0_AND_1,
      .pme_freq = PFM_CELL_PME_FREQ_PPU_MFC,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "DATA_ERAT_MISS_TH1",
      .pme_desc = "Data effective-address-to-real-address translation (D-ERAT) miss. Not speculative.",
      .pme_code = 0x8aa, /* 2218 */
      .pme_enable_word = WORD_0_AND_1,
      .pme_freq = PFM_CELL_PME_FREQ_PPU_MFC,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "LD_VALID_TH1",
      .pme_desc = "Load valid at a particular pipe stage. Speculative, since flushed operations are counted as well. Counts microcoded PPE sequences more than once. Misaligned flushes might be counted the first time as well. Load operations include all loads that read data from the cache, dcbt and dcbtst. Does not include load Vector/SIMD multimedia extension pattern instructions.",
      .pme_code = 0x8ac, /* 2220 */
      .pme_enable_word = WORD_0_AND_1,
      .pme_freq = PFM_CELL_PME_FREQ_PPU_MFC,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "DATA_ERAT_MISS_TH1",
      .pme_desc = "L1 D-cache load miss. Pulsed when there is a miss request that has a tag miss but not an ERAT miss. Speculative, since flushed operations are counted as well.",
      .pme_code = 0x8ad, /* 2221 */
      .pme_enable_word = WORD_0_AND_1,
      .pme_freq = PFM_CELL_PME_FREQ_PPU_MFC,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "LD_MFC_MMIO",
      .pme_desc = "Load from MFC memory-mapped I/O (MMIO) space.",
      .pme_code = 0xc1c, /* 3100 */
      .pme_enable_word = WORD_2_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "ST_MFC_MMIO",
      .pme_desc = "Stores to MFC MMIO space.",
      .pme_code = 0xc1d, /* 3101 */
      .pme_enable_word = WORD_2_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "REQ_TOKEN_TYPE",
      .pme_desc = "Request token for even memory bank numbers 0-14.",
      .pme_code = 0xc22, /* 3106 */
      .pme_enable_word = WORD_2_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "RCV_8BEAT_DATA",
      .pme_desc = "Receive 8-beat data from the Element Interconnect Bus (EIB).",
      .pme_code = 0xc2b, /* 3115 */
      .pme_enable_word = WORD_2_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "SEND_8BEAT_DATA",
      .pme_desc = "Send 8-beat data to the EIB.",
      .pme_code = 0xc2c, /* 3116 */
      .pme_enable_word = WORD_2_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "SEND_CMD",
      .pme_desc = "Send a command to the EIB; includes retried commands.",
      .pme_code = 0xc2d, /* 3117 */
      .pme_enable_word = WORD_2_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "DATA_GRANT_CYCLES",
      .pme_desc = "Cycles between data request and data grant.",
      .pme_code = 0xc2e, /* 3118 */
      .pme_enable_word = WORD_2_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "NCU_ST_Q_NOT_EMPTY_CYCLES",
      .pme_desc = "The five-entry Non-Cacheable Unit (NCU) Store Command queue not empty.",
      .pme_code = 0xc33, /* 3123 */
      .pme_enable_word = WORD_2_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "L2_CACHE_HIT",
      .pme_desc = "Cache hit for core interface unit (CIU) loads and stores.",
      .pme_code = 0xc80, /* 3200 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "L2_CACHE_MISS",
      .pme_desc = "Cache miss for CIU loads and stores.",
      .pme_code = 0xc81, /* 3201 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "L2_LD_MISS",
      .pme_desc = "CIU load miss.",
      .pme_code = 0xc84, /* 3204 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "L2_ST_MISS",
      .pme_desc = "CIU store to Invalid state (miss).",
      .pme_code = 0xc85, /* 3205 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "L2_LWARX_LDARX_MISS_TH0",
      .pme_desc = "Load word and reserve indexed (lwarx/ldarx) for Thread 0 hits Invalid cache state",
      .pme_code = 0xc87, /* 3207 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "L2_STWCX_STDCX_MISS_TH0",
      .pme_desc = "Store word conditional indexed (stwcx/stdcx) for Thread 0 hits Invalid cache state when reservation is set.",
      .pme_code = 0xc8e, /* 3214 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "L2_ALL_SNOOP_SM_BUSY",
      .pme_desc = "All four snoop state machines busy.",
      .pme_code = 0xc99, /* 3225 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "L2_DCLAIM_GOOD",
      .pme_desc = "Data line claim (dclaim) that received good combined response; includes store/stcx/dcbz to Shared (S), Shared Last (SL),or Tagged (T) cache state; does not include dcbz to Invalid (I) cache state.",
      .pme_code = 0xce8, /* 3304 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "L2_DCLAIM_TO_RWITM",
      .pme_desc = "Dclaim converted into rwitm; may still not get to the bus if stcx is aborted .",
      .pme_code = 0xcef, /* 3311 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "L2_ST_TO_M_MU_E",
      .pme_desc = "Store to modified (M), modified unsolicited (MU), or exclusive (E) cache state.",
      .pme_code = 0xcf0, /* 3312 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "L2_ST_Q_FULL",
      .pme_desc = "8-entry store queue (STQ) full.",
      .pme_code = 0xcf1, /* 3313 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "L2_ST_TO_RC_ACKED",
      .pme_desc = "Store dispatched to RC machine is acknowledged.",
      .pme_code = 0xcf2, /* 3314 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "L2_GATHERABLE_ST",
      .pme_desc = "Gatherable store (type = 00000) received from CIU.",
      .pme_code = 0xcf3, /* 3315 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "L2_SNOOP_PUSH",
      .pme_desc = "Snoop push.",
      .pme_code = 0xcf6, /* 3318 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "L2_INTERVENTION_FROM_SL_E_SAME_MODE",
      .pme_desc = "Send intervention from (SL | E) cache state to a destination within the same CBE chip.",
      .pme_code = 0xcf7, /* 3319 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "L2_INTERVENTION_FROM_M_MU_SAME_MODE",
      .pme_desc = "Send intervention from (M | MU) cache state to a destination within the same CBE chip.",
      .pme_code = 0xcf8, /* 3320 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "L2_SNOOP_RETRY_CONFLICTS",
      .pme_desc = "Respond with Retry to a snooped request due to one of the following conflicts",
      .pme_code = 0xcfd, /* 3325 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "L2_SNOOP_RETRY_BUSY",
      .pme_desc = "Respond with Retry to a snooped request because all snoop machines are busy.",
      .pme_code = 0xcfe, /* 3326 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "L2_SNOOP_RESP_MMU_TO_EST",
      .pme_desc = "Snooped response causes a cache state transition from (M | MU) to (E | S | T).",
      .pme_code = 0xcff, /* 3327 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "L2_SNOOP_RESP_E_TO_S",
      .pme_desc = "Snooped response causes a cache state transition from E to S.",
      .pme_code = 0xd00, /* 3328 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "L2_SNOOP_RESP_ESLST_TO_I",
      .pme_desc = "Snooped response causes a cache state transition from (E | SL | S | T) to Invalid (I).",
      .pme_code = 0xd01, /* 3329 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "L2_SNOOP_RESP_MMU_TO_I",
      .pme_desc = "Snooped response causes a cache state transition from (M | MU) to I.",
      .pme_code = 0xd02, /* 3330 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "L2_LWARX_LDARX_MISS_TH1",
      .pme_desc = "Load and reserve indexed (lwarx/ldarx) for Thread 1 hits Invalid cache state.",
      .pme_code = 0xd54, /* 3412 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "L2_STWCX_STDCX_MISS_TH1",
      .pme_desc = "Store conditional indexed (stwcx/stdcx) for Thread 1 hits Invalid cache state.",
      .pme_code = 0xd5b, /* 3419 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "NCU_NON_CACHEABLE_ST_ALL",
      .pme_desc = "Non-cacheable store request received from CIU; includes all synchronization operations such as sync and eieio.",
      .pme_code = 0xdac, /* 3500 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "NCU_SYNC_REQ",
      .pme_desc = "sync received from CIU.",
      .pme_code = 0xdad, /* 3501 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "NCU_NON_CACHEABLE_ST",
      .pme_desc = "Non-cacheable store request received from CIU; includes only stores.",
      .pme_code = 0xdb0, /* 3504 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "NCU_EIEIO_REQ",
      .pme_desc = "eieio received from CIU.",
      .pme_code = 0xdb2, /* 3506 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "NCU_TLBIE_REQ",
      .pme_desc = "tlbie received from CIU.",
      .pme_code = 0xdb3, /* 3507 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "NCU_SYNC_WAIT",
      .pme_desc = "sync at the bottom of the store queue, while waiting on st_done signal from the Bus Interface Unit (BIU) and sync_done signal from L2.",
      .pme_code = 0xdb4, /* 3508 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "NCU_LWSYNC_WAIT",
      .pme_desc = "lwsync at the bottom of the store queue, while waiting for a sync_done signal from the L2.",
      .pme_code = 0xdb5, /* 3509 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "NCU_EIEIO_WAIT",
      .pme_desc = "eieio at the bottom of the store queue, while waiting for a st_done signal from the BIU and a sync_done signal from the L2.",
      .pme_code = 0xdb6, /* 3510 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "NCU_TLBIE_WAIT",
      .pme_desc = "tlbie at the bottom of the store queue, while waiting for a st_done signal from the BIU.",
      .pme_code = 0xdb7, /* 3511 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "NCU_COMBINED_NON_CACHEABLE_ST",
      .pme_desc = "Non-cacheable store combined with the previous non-cacheable store with a contiguous address.",
      .pme_code = 0xdb8, /* 3512 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "NCU_ALL_ST_GATHER_BUFFS_FULL",
      .pme_desc = "All four store-gather buffers full.",
      .pme_code = 0xdbb, /* 3515 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "NCU_LD_REQ",
      .pme_desc = "Non-cacheable load request received from CIU; includes instruction and data fetches.",
      .pme_code = 0xdbc, /* 3516 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "NCU_ST_Q_NOT_EMPTY",
      .pme_desc = "The four-deep store queue not empty.",
      .pme_code = 0xdbd, /* 3517 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "NCU_ST_Q_FULL",
      .pme_desc = "The four-deep store queue full.",
      .pme_code = 0xdbe, /* 3518 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "NCU_AT_LEAST_ONE_ST_GATHER_BUFF_NOT_EMPTY",
      .pme_desc = "At least one store gather buffer not empty.",
      .pme_code = 0xdbf, /* 3519 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "SPU_DUAL_INST_COMMITTED",
      .pme_desc = "A dual instruction is committed.",
      .pme_code = 0x1004, /* 4100 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_SPU,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "SPU_SINGLE_INST_COMMITTED",
      .pme_desc = "A single instruction is committed.",
      .pme_code = 0x1005, /* 4101 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_SPU,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "SPU_PIPE0_INST_COMMITTED",
      .pme_desc = "A pipeline 0 instruction is committed.",
      .pme_code = 0x1006, /* 4102 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_SPU,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "SPU_PIPE1_INST_COMMITTED",
      .pme_desc = "A pipeline 1 instruction is committed.",
      .pme_code = 0x1007, /* 4103 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_SPU,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "SPU_LS_BUSY",
      .pme_desc = "Local storage is busy.",
      .pme_code = 0x1009, /* 4105 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_SPU,
      .pme_type = COUNT_TYPE_BOTH_TYPE,
   },
   {
      .pme_name = "SPU_DMA_CONFLICT_LD_ST",
      .pme_desc = "A direct memory access (DMA) might conflict with a load or store.",
      .pme_code = 0x100a, /* 4106 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_SPU,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "SPU_LS_ST",
      .pme_desc = "A store instruction to local storage is issued.",
      .pme_code = 0x100b, /* 4107 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_SPU,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "SPU_LS_LD",
      .pme_desc = "A load instruction from local storage is issued.",
      .pme_code = 0x100c, /* 4108 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_SPU,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "SPU_FP_EXCEPTION",
      .pme_desc = "A floating-point unit exception occurred.",
      .pme_code = 0x100d, /* 4109 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_SPU,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "SPU_BRANCH_COMMIT",
      .pme_desc = "A branch instruction is committed.",
      .pme_code = 0x100e, /* 4110 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_SPU,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "SPU_NON_SEQ_PC",
      .pme_desc = "A nonsequential change of the SPU program counter has occurred. This can be caused by branch, asynchronous interrupt, stalled wait on channel, error-correction code (ECC) error, and so forth.",
      .pme_code = 0x100f, /* 4111 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_SPU,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "SPU_BRANCH_NOT_TAKEN",
      .pme_desc = "A branch was not taken.",
      .pme_code = 0x1010, /* 4112 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_SPU,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "SPU_BRANCH_MISS_PREDICTION",
      .pme_desc = "Branch miss prediction. This count is not exact. Certain other code sequences can cause additional pulses on this signal.",
      .pme_code = 0x1011, /* 4113 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_SPU,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "SPU_BRANCH_HINT_MISS_PREDICTION",
      .pme_desc = "Branch hint miss prediction. This count is not exact. Certain other code sequences can cause additional pulses on this signal.",
      .pme_code = 0x1012, /* 4114 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_SPU,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "SPU_INST_SEQ_ERROR",
      .pme_desc = "Instruction sequence error",
      .pme_code = 0x1013, /* 4115 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_SPU,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "SPU_STALL_CH_WRITE",
      .pme_desc = "Stalled waiting on any blocking channel write.",
      .pme_code = 0x1015, /* 4117 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_SPU,
      .pme_type = COUNT_TYPE_BOTH_TYPE,
   },
   {
      .pme_name = "SPU_STALL_EXTERNAL_EVENT_CH0",
      .pme_desc = "Stalled waiting on external event status (Channel 0).",
      .pme_code = 0x1016, /* 4118 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_SPU,
      .pme_type = COUNT_TYPE_BOTH_TYPE,
   },
   {
      .pme_name = "SPU_STALL_SIGNAL_1_CH3",
      .pme_desc = "Stalled waiting on SPU Signal Notification 1 (Channel 3).",
      .pme_code = 0x1017, /* 4119 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_SPU,
      .pme_type = COUNT_TYPE_BOTH_TYPE,
   },
   {
      .pme_name = "SPU_STALL_SIGNAL_2_CH4",
      .pme_desc = "Stalled waiting on SPU Signal Notification 2 (Channel 4).",
      .pme_code = 0x1018, /* 4120 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_SPU,
      .pme_type = COUNT_TYPE_BOTH_TYPE,
   },
   {
      .pme_name = "SPU_STALL_DMA_CH21",
      .pme_desc = "Stalled waiting on DMA Command Opcode or ClassID Register (Channel 21).",
      .pme_code = 0x1019, /* 4121 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_SPU,
      .pme_type = COUNT_TYPE_BOTH_TYPE,
   },
   {
      .pme_name = "SPU_STALL_MFC_READ_CH24",
      .pme_desc = "Stalled waiting on memory flow control (MFC) Read Tag-Group Status (Channel 24).",
      .pme_code = 0x101a, /* 4122 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_SPU,
      .pme_type = COUNT_TYPE_BOTH_TYPE,
   },
   {
      .pme_name = "SPU_STALL_MFC_READ_CH25",
      .pme_desc = "Stalled waiting on MFC Read List Stall-and-Notify Tag Status (Channel 25).",
      .pme_code = 0x101b, /* 4123 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_SPU,
      .pme_type = COUNT_TYPE_BOTH_TYPE,
   },
   {
      .pme_name = "SPU_STALL_OUTBOUND_MAILBOX_WRITE_CH28",
      .pme_desc = "Stalled waiting on SPU Write Outbound Mailbox (Channel 28).",
      .pme_code = 0x101c, /* 4124 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_SPU,
      .pme_type = COUNT_TYPE_BOTH_TYPE,
   },
   {
      .pme_name = "SPU_STALL_MAILBOX_CH29",
      .pme_desc = "Stalled waiting on SPU Mailbox (Channel 29).",
      .pme_code = 0x1022, /* 4130 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_SPU,
      .pme_type = COUNT_TYPE_BOTH_TYPE,
   },
   {
      .pme_name = "SPU_TR_STALL_CH",
      .pme_desc = "Stalled waiting on a channel operation.",
      .pme_code = 0x10a1, /* 4257 */
      .pme_enable_word = WORD_NONE,
      .pme_freq = PFM_CELL_PME_FREQ_SPU,
      .pme_type = COUNT_TYPE_BOTH_TYPE,
   },
   {
      .pme_name = "SPU_EV_INST_FETCH_STALL",
      .pme_desc = "Instruction fetch stall",
      .pme_code = 0x1107, /* 4359 */
      .pme_enable_word = WORD_NONE,
      .pme_freq = PFM_CELL_PME_FREQ_SPU,
      .pme_type = COUNT_TYPE_BOTH_TYPE,
   },
   {
      .pme_name = "SPU_EV_ADDR_TRACE",
      .pme_desc = "Serialized SPU address (program counter) trace.",
      .pme_code = 0x110b, /* 4363 */
      .pme_enable_word = 0xF,
      .pme_freq = PFM_CELL_PME_FREQ_SPU,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "MFC_ATOMIC_LD",
      .pme_desc = "An atomic load was received from direct memory access controller (DMAC).",
      .pme_code = 0x13ed, /* 5101 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "MFC_ATOMIC_DCLAIM",
      .pme_desc = "An atomic dclaim was sent to synergistic bus interface (SBI); includes retried requests.",
      .pme_code = 0x13ee, /* 5102 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "MFC_ATOMIC_RWITM",
      .pme_desc = "An atomic rwitm performed was sent to SBI; includes retried requests.",
      .pme_code = 0x13ef, /* 5103 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "MFC_ATOMIC_LD_CACHE_MISS_MU",
      .pme_desc = "An atomic load miss caused MU cache state.",
      .pme_code = 0x13f0, /* 5104 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "MFC_ATOMIC_LD_CACHE_MISS_E",
      .pme_desc = "An atomic load miss caused E cache state.",
      .pme_code = 0x13f1, /* 5105 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "MFC_ATOMIC_LD_CACHE_MISS_SL",
      .pme_desc = "An atomic load miss caused SL cache state.",
      .pme_code = 0x13f2, /* 5106 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "MFC_ATOMIC_LD_CACHE_HIT",
      .pme_desc = "An atomic load hits cache.",
      .pme_code = 0x13f3, /* 5107 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "MFC_ATOMIC_LD_CACHE_MISS_INTERVENTION",
      .pme_desc = "Atomic load misses cache with data intervention; sum of signals 4 and 6 in this group.",
      .pme_code = 0x13f4, /* 5108 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "MFC_ATOMIC_PUTLLXC_CACHE_MISS_WO_INTERVENTION",
      .pme_desc = "putllc or putlluc misses cache without data intervention; for putllc, counts only when reservation is set for the address.",
      .pme_code = 0x13fa, /* 5114 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "MFC_SNOOP_MACHINE_BUSY",
      .pme_desc = "Snoop machine busy.",
      .pme_code = 0x13fd, /* 5117 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "MFC_SNOOP_MMU_TO_I",
      .pme_desc = "A snoop caused cache transition from [M | MU] to I.",
      .pme_code = 0x13ff, /* 5119 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "MFC_SNOOP_ESSL_TO_I",
      .pme_desc = "A snoop caused cache transition from [E | S | SL] to I.",
      .pme_code = 0x1401, /* 5121 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "MFC_SNOOP_MU_TO_T",
      .pme_desc = "A snoop caused cache transition from MU to T cache state.",
      .pme_code = 0x1403, /* 5123 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "MFC_SENT_INTERVENTION_LOCAL",
      .pme_desc = "Sent modified data intervention to a destination within the same CBE chip.",
      .pme_code = 0x1407, /* 5127 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "MFC_ANY_DMA_GET",
      .pme_desc = "Any flavor of DMA get [] command issued to Synergistic Bus Interface (SBI); sum of signals 17-25 in this group.",
      .pme_code = 0x1450, /* 5200 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "MFC_ANY_DMA_PUT",
      .pme_desc = "Any flavor of DMA put [] command issued to SBI; sum of signals 2-16 in this group.",
      .pme_code = 0x1451, /* 5201 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "MFC_DMA_PUT",
      .pme_desc = "DMA put (put) is issued to SBI.",
      .pme_code = 0x1452, /* 5202 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "MFC_DMA_GET",
      .pme_desc = "DMA get data from effective address to local storage (get) issued to SBI.",
      .pme_code = 0x1461, /* 5217 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "MFC_LD_REQ",
      .pme_desc = "Load request sent to element interconnect bus (EIB); includes read, read atomic, rwitm, rwitm atomic, and retried commands.",
      .pme_code = 0x14b8, /* 5304 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "MFC_ST_REQ",
      .pme_desc = "Store request sent to EIB; includes wwf, wwc, wwk, dclaim, dclaim atomic, and retried commands.",
      .pme_code = 0x14b9, /* 5305 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "MFC_RECV_DATA",
      .pme_desc = "Received data from EIB, including partial cache line data.",
      .pme_code = 0x14ba, /* 5306 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "MFC_SENT_DATA",
      .pme_desc = "Sent data to EIB, both as a master and a snooper.",
      .pme_code = 0x14bb, /* 5307 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "MFC_SBI_Q_NOT_EMPTY",
      .pme_desc = "16-deep synergistic bus interface (SBI) queue with outgoing requests not empty; does not include atomic requests.",
      .pme_code = 0x14bc, /* 5308 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "MFC_SBI_Q_FULL",
      .pme_desc = "16-deep SBI queue with outgoing requests full; does not include atomic requests.",
      .pme_code = 0x14bd, /* 5309 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "MFC_SENT_REQ",
      .pme_desc = "Sent request to EIB.",
      .pme_code = 0x14be, /* 5310 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "MFC_RECV_DATA_BUS_GRANT",
      .pme_desc = "Received data bus grant; includes data sent for MMIO operations.",
      .pme_code = 0x14c0, /* 5312 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "MFC_WAIT_DATA_BUS_GRANT",
      .pme_desc = "Cycles between data bus request and data bus grant.",
      .pme_code = 0x14c1, /* 5313 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "MFC_CMD_O_MEM",
      .pme_desc = "Command (read or write) for an odd-numbered memory bank; valid only when resource allocation is turned on.",
      .pme_code = 0x14c2, /* 5314 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "MFC_CMD_E_MEM",
      .pme_desc = "Command (read or write) for an even-numbered memory bank; valid only when resource allocation is turned on.",
      .pme_code = 0x14c3, /* 5315 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "MFC_RECV_RETRY_RESP",
      .pme_desc = "Request gets the Retry response; includes local and global requests.",
      .pme_code = 0x14c6, /* 5318 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "MFC_SENT_DATA_BUS_REQ",
      .pme_desc = "Sent data bus request to EIB.",
      .pme_code = 0x14c7, /* 5319 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "MFC_TLB_MISS",
      .pme_desc = "Translation Lookaside Buffer (TLB) miss without parity or protection errors.",
      .pme_code = 0x1518, /* 5400 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "MFC_TLB_CYCLES",
      .pme_desc = "TLB miss (cycles).",
      .pme_code = 0x1519, /* 5401 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "MFC_TLB_HIT",
      .pme_desc = "TLB hit.",
      .pme_code = 0x151a, /* 5402 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_READ_RWITM_1",
      .pme_desc = "Number of read and rwitm commands (including atomic) AC1 to AC0. (Group 1)",
      .pme_code = 0x17d4, /* 6100 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_DCLAIM_1",
      .pme_desc = "Number of dclaim commands (including atomic) AC1 to AC0. (Group 1)",
      .pme_code = 0x17d5, /* 6101 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_WWK_WWC_WWF_1",
      .pme_desc = "Number of wwk, wwc, and wwf commands from AC1 to AC0. (Group 1)",
      .pme_code = 0x17d6, /* 6102 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_SYNC_TLBSYNC_EIEIO_1",
      .pme_desc = "Number of sync, tlbsync, and eieio commands from AC1 to AC0. (Group 1)",
      .pme_code = 0x17d7, /* 6103 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_TLBIE_1",
      .pme_desc = "Number of tlbie commands from AC1 to AC0. (Group 1)",
      .pme_code = 0x17d8, /* 6104 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_PAAM_CAM_HIT_1",
      .pme_desc = "Previous adjacent address match (PAAM) Content Addressable Memory (CAM) hit. (Group 1)",
      .pme_code = 0x17df, /* 6111 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_PAAM_CAM_MISS_1",
      .pme_desc = "PAAM CAM miss. (Group 1)",
      .pme_code = 0x17e0, /* 6112 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_CMD_REFLECTED_1",
      .pme_desc = "Command reflected. (Group 1)",
      .pme_code = 0x17e2, /* 6114 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_READ_RWITM_2",
      .pme_desc = "Number of read and rwitm commands (including atomic) AC1 to AC0. (Group 2)",
      .pme_code = 0x17e4, /* 6116 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_DCLAIM_2",
      .pme_desc = "Number of dclaim commands (including atomic) AC1 to AC0. (Group 2)",
      .pme_code = 0x17e5, /* 6117 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_WWK_WWC_WWF_2",
      .pme_desc = "Number of wwk, wwc, and wwf commands from AC1 to AC0. (Group 2)",
      .pme_code = 0x17e6, /* 6118 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_SYNC_TLBSYNC_EIEIO_2",
      .pme_desc = "Number of sync, tlbsync, and eieio commands from AC1 to AC0. (Group 2)",
      .pme_code = 0x17e7, /* 6119 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_TLBIE_2",
      .pme_desc = "Number of tlbie commands from AC1 to AC0. (Group 2)",
      .pme_code = 0x17e8, /* 6120 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_PAAM_CAM_HIT_2",
      .pme_desc = "PAAM CAM hit. (Group 2)",
      .pme_code = 0x17ef, /* 6127 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_PAAM_CAM_MISS_2",
      .pme_desc = "PAAM CAM miss. (Group 2)",
      .pme_code = 0x17f0, /* 6128 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_CMD_REFLECTED_2",
      .pme_desc = "Command reflected. (Group 2)",
      .pme_code = 0x17f2, /* 6130 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_LOCAL_CMD_FROM_SPE6",
      .pme_desc = "Local command from SPE 6.",
      .pme_code = 0x1839, /* 6201 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_LOCAL_CMD_FROM_SPE4",
      .pme_desc = "Local command from SPE 4.",
      .pme_code = 0x183a, /* 6202 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_LOCAL_CME_FROM_SPE2",
      .pme_desc = "Local command from SPE 2.",
      .pme_code = 0x183b, /* 6203 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_LOCAL_CMD_FROM_PPE",
      .pme_desc = "Local command from PPE.",
      .pme_code = 0x183d, /* 6205 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_LOCAL_CMD_FROM_SPE1",
      .pme_desc = "Local command from SPE 1.",
      .pme_code = 0x183e, /* 6206 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_LOCAL_CMD_FROM_SPE3",
      .pme_desc = "Local command from SPE 3.",
      .pme_code = 0x183f, /* 6207 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_LOCAL_CMD_FROM_SPE5",
      .pme_desc = "Local command from SPE 5.",
      .pme_code = 0x1840, /* 6208 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_LOCAL_CMD_FROM_SPE7",
      .pme_desc = "Local command from SPE 7.",
      .pme_code = 0x1841, /* 6209 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_GLOBAL_CMD_FROM_SPE6",
      .pme_desc = "AC1-to-AC0 global command from SPE 6.",
      .pme_code = 0x1844, /* 6212 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_GLOBAL_CMD_FROM_SPE4",
      .pme_desc = "AC1-to-AC0 global command from SPE 4.",
      .pme_code = 0x1845, /* 6213 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_GLOBAL_CMD_FROM_SPE2",
      .pme_desc = "AC1-to-AC0 global command from SPE 2.",
      .pme_code = 0x1846, /* 6214 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_GLOBAL_CMD_FROM_SPE0",
      .pme_desc = "AC1-to-AC0 global command from SPE 0.",
      .pme_code = 0x1847, /* 6215 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_GLOBAL_CMD_FROM_PPE",
      .pme_desc = "AC1-to-AC0 global command from PPE.",
      .pme_code = 0x1848, /* 6216 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_GLOBAL_CMD_FROM_SPE1",
      .pme_desc = "AC1-to-AC0 global command from SPE 1.",
      .pme_code = 0x1849, /* 6217 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_GLOBAL_CMD_FROM_SPE3",
      .pme_desc = "AC1-to-AC0 global command from SPE 3.",
      .pme_code = 0x184a, /* 6218 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_GLOBAL_CMD_FROM_SPE5",
      .pme_desc = "AC1-to-AC0 global command from SPE 5.",
      .pme_code = 0x184b, /* 6219 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_GLOBAL_CMD_FROM_SPE7",
      .pme_desc = "AC1-to-AC0 global command from SPE 7",
      .pme_code = 0x184c, /* 6220 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_AC1_REFLECTING_LOCAL_CMD",
      .pme_desc = "AC1 is reflecting any local command.",
      .pme_code = 0x184e, /* 6222 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_AC1_SEND_GLOBAL_CMD",
      .pme_desc = "AC1 sends a global command to AC0.",
      .pme_code = 0x184f, /* 6223 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_AC0_REFLECT_GLOBAL_CMD",
      .pme_desc = "AC0 reflects a global command back to AC1.",
      .pme_code = 0x1850, /* 6224 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_AC1_REFLECT_CMD_TO_BM",
      .pme_desc = "AC1 reflects a command back to the bus masters.",
      .pme_code = 0x1851, /* 6225 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_GRANT_DATA_RING0_1",
      .pme_desc = "Grant on data ring 0.",
      .pme_code = 0x189c, /* 6300 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_GRANT_DATA_RING1_1",
      .pme_desc = "Grant on data ring 1.",
      .pme_code = 0x189d, /* 6301 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_GRANT_DATA_RING2_1",
      .pme_desc = "Grant on data ring 2.",
      .pme_code = 0x189e, /* 6302 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_GRANT_DATA_RING3_1",
      .pme_desc = "Grant on data ring 3.",
      .pme_code = 0x189f, /* 6303 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_DATA_RING0_INUSE_1",
      .pme_desc = "Data ring 0 is in use.",
      .pme_code = 0x18a0, /* 6304 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_DATA_RING1_INUSE_1",
      .pme_desc = "Data ring 1 is in use.",
      .pme_code = 0x18a1, /* 6305 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_DATA_RING2_INUSE_1",
      .pme_desc = "Data ring 2 is in use.",
      .pme_code = 0x18a2, /* 6306 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_DATA_RING3_INUSE_1",
      .pme_desc = "Data ring 3 is in use.",
      .pme_code = 0x18a3, /* 6307 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_ALL_DATA_RINGS_IDLE_1",
      .pme_desc = "All data rings are idle.",
      .pme_code = 0x18a4, /* 6308 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_ONE_DATA_RING_BUSY_1",
      .pme_desc = "One data ring is busy.",
      .pme_code = 0x18a5, /* 6309 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_TWO_OR_THREE_DATA_RINGS_BUSY_1",
      .pme_desc = "Two or three data rings are busy.",
      .pme_code = 0x18a6, /* 6310 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_ALL_DATA_RINGS_BUSY_1",
      .pme_desc = "All data rings are busy.",
      .pme_code = 0x18a7, /* 6311 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_IOIF0_DATA_REQ_PENDING_1",
      .pme_desc = "BIC (IOIF0) data request pending.",
      .pme_code = 0x18a8, /* 6312 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_BOTH_TYPE,
   },
   {
      .pme_name = "EIB_SPE6_DATA_REQ_PENDING_1",
      .pme_desc = "SPE 6 data request pending.",
      .pme_code = 0x18a9, /* 6313 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_BOTH_TYPE,
   },
   {
      .pme_name = "EIB_SPE4_DATA_REQ_PENDING_1",
      .pme_desc = "SPE 4 data request pending.",
      .pme_code = 0x18aa, /* 6314 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_BOTH_TYPE,
   },
   {
      .pme_name = "EIB_SPE2_DATA_REQ_PENDING_1",
      .pme_desc = "SPE 2 data request pending.",
      .pme_code = 0x18ab, /* 6315 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_BOTH_TYPE,
   },
   {
      .pme_name = "EIB_SPE0_DATA_REQ_PENDING_1",
      .pme_desc = "SPE 0 data request pending.",
      .pme_code = 0x18ac, /* 6316 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_BOTH_TYPE,
   },
   {
      .pme_name = "EIB_MIC_DATA_REQ_PENDING_1",
      .pme_desc = "MIC data request pending.",
      .pme_code = 0x18ad, /* 6317 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_BOTH_TYPE,
   },
   {
      .pme_name = "EIB_PPE_DATA_REQ_PENDING_1",
      .pme_desc = "PPE data request pending.",
      .pme_code = 0x18ae, /* 6318 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_BOTH_TYPE,
   },
   {
      .pme_name = "EIB_SPE1_DATA_REQ_PENDING_1",
      .pme_desc = "SPE 1 data request pending.",
      .pme_code = 0x18af, /* 6319 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_BOTH_TYPE,
   },
   {
      .pme_name = "EIB_SPE3_DATA_REQ_PENDING_1",
      .pme_desc = "SPE 3 data request pending.",
      .pme_code = 0x18b0, /* 6320 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_BOTH_TYPE,
   },
   {
      .pme_name = "EIB_SPE5_DATA_REQ_PENDING_1",
      .pme_desc = "SPE 5 data request pending.",
      .pme_code = 0x18b1, /* 6321 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_BOTH_TYPE,
   },
   {
      .pme_name = "EIB_SPE7_DATA_REQ_PENDING_1",
      .pme_desc = "SPE 7 data request pending.",
      .pme_code = 0x18b2, /* 6322 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_BOTH_TYPE,
   },
   {
      .pme_name = "EIB_IOIF0_DATA_DEST_1",
      .pme_desc = "IOIF0 is data destination.",
      .pme_code = 0x18b4, /* 6324 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_SPE6_DATA_DEST_1",
      .pme_desc = "SPE 6 is data destination.",
      .pme_code = 0x18b5, /* 6325 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_SPE4_DATA_DEST_1",
      .pme_desc = "SPE 4 is data destination.",
      .pme_code = 0x18b6, /* 6326 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_SPE2_DATA_DEST_1",
      .pme_desc = "SPE 2 is data destination.",
      .pme_code = 0x18b7, /* 6327 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_SPE0_DATA_DEST_1",
      .pme_desc = "SPE 0 is data destination.",
      .pme_code = 0x18b8, /* 6328 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_MIC_DATA_DEST_1",
      .pme_desc = "MIC is data destination.",
      .pme_code = 0x18b9, /* 6329 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_PPE_DATA_DEST_1",
      .pme_desc = "PPE is data destination.",
      .pme_code = 0x18ba, /* 6330 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_SPE1_DATA_DEST_1",
      .pme_desc = "SPE 1 is data destination.",
      .pme_code = 0x18bb, /* 6331 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_IOIF0_DATA_REQ_PENDING_2",
      .pme_desc = "BIC (IOIF0) data request pending.",
      .pme_code = 0x1900, /* 6400 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_BOTH_TYPE,
   },
   {
      .pme_name = "EIB_SPE6_DATA_REQ_PENDING_2",
      .pme_desc = "SPE 6 data request pending.",
      .pme_code = 0x1901, /* 6401 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_BOTH_TYPE,
   },
   {
      .pme_name = "EIB_SPE4_DATA_REQ_PENDING_2",
      .pme_desc = "SPE 4 data request pending.",
      .pme_code = 0x1902, /* 6402 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_BOTH_TYPE,
   },
   {
      .pme_name = "EIB_SPE2_DATA_REQ_PENDING_2",
      .pme_desc = "SPE 2 data request pending.",
      .pme_code = 0x1903, /* 6403 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_BOTH_TYPE,
   },
   {
      .pme_name = "EIB_SPE0_DATA_REQ_PENDING_2",
      .pme_desc = "SPE 0 data request pending.",
      .pme_code = 0x1904, /* 6404 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_BOTH_TYPE,
   },
   {
      .pme_name = "EIB_MIC_DATA_REQ_PENDING_2",
      .pme_desc = "MIC data request pending.",
      .pme_code = 0x1905, /* 6405 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_BOTH_TYPE,
   },
   {
      .pme_name = "EIB_PPE_DATA_REQ_PENDING_2",
      .pme_desc = "PPE data request pending.",
      .pme_code = 0x1906, /* 6406 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_BOTH_TYPE,
   },
   {
      .pme_name = "EIB_SPE1_DATA_REQ_PENDING_2",
      .pme_desc = "SPE 1 data request pending.",
      .pme_code = 0x1907, /* 6407 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_BOTH_TYPE,
   },
   {
      .pme_name = "EIB_SPE3_DATA_REQ_PENDING_2",
      .pme_desc = "SPE 3 data request pending.",
      .pme_code = 0x1908, /* 6408 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_BOTH_TYPE,
   },
   {
      .pme_name = "EIB_SPE5_DATA_REQ_PENDING_2",
      .pme_desc = "SPE 5 data request pending.",
      .pme_code = 0x1909, /* 6409 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_BOTH_TYPE,
   },
   {
      .pme_name = "EIB_SPE7_DATA_REQ_PENDING_2",
      .pme_desc = "SPE 7 data request pending.",
      .pme_code = 0x190a, /* 6410 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_BOTH_TYPE,
   },
   {
      .pme_name = "EIB_IOIF1_DATA_REQ_PENDING_2",
      .pme_desc = "IOIF1 data request pending.",
      .pme_code = 0x190b, /* 6411 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_BOTH_TYPE,
   },
   {
      .pme_name = "EIB_IOIF0_DATA_DEST_2",
      .pme_desc = "IOIF0 is data destination.",
      .pme_code = 0x190c, /* 6412 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_SPE6_DATA_DEST_2",
      .pme_desc = "SPE 6 is data destination.",
      .pme_code = 0x190d, /* 6413 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_SPE4_DATA_DEST_2",
      .pme_desc = "SPE 4 is data destination.",
      .pme_code = 0x190e, /* 6414 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_SPE2_DATA_DEST_2",
      .pme_desc = "SPE 2 is data destination.",
      .pme_code = 0x190f, /* 6415 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_SPE0_DATA_DEST_2",
      .pme_desc = "SPE 0 is data destination.",
      .pme_code = 0x1910, /* 6416 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_MIC_DATA_DEST_2",
      .pme_desc = "MIC is data destination.",
      .pme_code = 0x1911, /* 6417 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_PPE_DATA_DEST_2",
      .pme_desc = "PPE is data destination.",
      .pme_code = 0x1912, /* 6418 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_SPE1_DATA_DEST_2",
      .pme_desc = "SPE 1 is data destination.",
      .pme_code = 0x1913, /* 6419 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_SPE3_DATA_DEST_2",
      .pme_desc = "SPE 3 is data destination.",
      .pme_code = 0x1914, /* 6420 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_SPE5_DATA_DEST_2",
      .pme_desc = "SPE 5 is data destination.",
      .pme_code = 0x1915, /* 6421 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_SPE7_DATA_DEST_2",
      .pme_desc = "SPE 7 is data destination.",
      .pme_code = 0x1916, /* 6422 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_IOIF1_DATA_DEST_2",
      .pme_desc = "IOIF1 is data destination.",
      .pme_code = 0x1917, /* 6423 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_GRANT_DATA_RING0_2",
      .pme_desc = "Grant on data ring 0.",
      .pme_code = 0x1918, /* 6424 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_GRANT_DATA_RING1_2",
      .pme_desc = "Grant on data ring 1.",
      .pme_code = 0x1919, /* 6425 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_GRANT_DATA_RING2_2",
      .pme_desc = "Grant on data ring 2.",
      .pme_code = 0x191a, /* 6426 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_GRANT_DATA_RING3_2",
      .pme_desc = "Grant on data ring 3.",
      .pme_code = 0x191b, /* 6427 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "EIB_ALL_DATA_RINGS_IDLE_2",
      .pme_desc = "All data rings are idle.",
      .pme_code = 0x191c, /* 6428 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_ONE_DATA_RING_BUSY_2",
      .pme_desc = "One data ring is busy.",
      .pme_code = 0x191d, /* 6429 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_TWO_OR_THREE_DATA_RINGS_BUSY_2",
      .pme_desc = "Two or three data rings are busy.",
      .pme_code = 0x191e, /* 6430 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_ALL_DATA_RINGS_BUSY_2",
      .pme_desc = "All four data rings are busy.",
      .pme_code = 0x191f, /* 6431 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG0_E_XIO_UNUSED",
      .pme_desc = "Even XIO token unused by RAG 0.",
      .pme_code = 0xfe4c, /* 65100 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG0_O_XIO_UNUSED",
      .pme_desc = "Odd XIO token unused by RAG 0.",
      .pme_code = 0xfe4d, /* 65101 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG0_E_BANK_UNUSED",
      .pme_desc = "Even bank token unused by RAG 0.",
      .pme_code = 0xfe4e, /* 65102 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG0_O_BANK_UNUSED",
      .pme_desc = "Odd bank token unused by RAG 0.",
      .pme_code = 0xfe4f, /* 65103 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_TOKEN_GRANTED_SPE0",
      .pme_desc = "Token granted for SPE 0.",
      .pme_code = 0xfe54, /* 65108 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_TOKEN_GRANTED_SPE1",
      .pme_desc = "Token granted for SPE 1.",
      .pme_code = 0xfe55, /* 65109 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_TOKEN_GRANTED_SPE2",
      .pme_desc = "Token granted for SPE 2.",
      .pme_code = 0xfe56, /* 65110 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_TOKEN_GRANTED_SPE3",
      .pme_desc = "Token granted for SPE 3.",
      .pme_code = 0xfe57, /* 65111 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_TOKEN_GRANTED_SPE4",
      .pme_desc = "Token granted for SPE 4.",
      .pme_code = 0xfe58, /* 65112 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_TOKEN_GRANTED_SPE5",
      .pme_desc = "Token granted for SPE 5.",
      .pme_code = 0xfe59, /* 65113 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_TOKEN_GRANTED_SPE6",
      .pme_desc = "Token granted for SPE 6.",
      .pme_code = 0xfe5a, /* 65114 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_TOKEN_GRANTED_SPE7",
      .pme_desc = "Token granted for SPE 7.",
      .pme_code = 0xfe5b, /* 65115 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG0_E_XIO_WASTED",
      .pme_desc = "Even XIO token wasted by RAG 0; valid only when Unused Enable (UE) = 1 in TKM_CR register.",
      .pme_code = 0xfeb0, /* 65200 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG0_O_XIO_WASTED",
      .pme_desc = "Odd XIO token wasted by RAG 0; valid only when Unused Enable (UE) = 1 in TKM_CR register.",
      .pme_code = 0xfeb1, /* 65201 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG0_E_BANK_WASTED",
      .pme_desc = "Even bank token wasted by RAG 0; valid only when Unused Enable (UE) = 1 in TKM_CR register.",
      .pme_code = 0xfeb2, /* 65202 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG0_O_BANK_WASTED",
      .pme_desc = "Odd bank token wasted by RAG 0; valid only when Unused Enable (UE) = 1 in TKM_CR register.",
      .pme_code = 0xfeb3, /* 65203 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAGU_E_XIO_WASTED",
      .pme_desc = "Even XIO token wasted by RAG U.",
      .pme_code = 0xfebc, /* 65212 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAGU_O_XIO_WASTED",
      .pme_desc = "Odd XIO token wasted by RAG U.",
      .pme_code = 0xfebd, /* 65213 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAGU_E_BANK_WASTED",
      .pme_desc = "Even bank token wasted by RAG U.",
      .pme_code = 0xfebe, /* 65214 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAGU_O_BANK_WASTED",
      .pme_desc = "Odd bank token wasted by RAG U.",
      .pme_code = 0xfebf, /* 65215 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG0_E_XIO_RAG1",
      .pme_desc = "Even XIO token from RAG 0 shared with RAG 1",
      .pme_code = 0xff14, /* 65300 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG0_E_XIO_RAG2",
      .pme_desc = "Even XIO token from RAG 0 shared with RAG 2",
      .pme_code = 0xff15, /* 65301 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG0_E_XIO_RAG3",
      .pme_desc = "Even XIO token from RAG 0 shared with RAG 3",
      .pme_code = 0xff16, /* 65302 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG0_O_XIO_RAG1",
      .pme_desc = "Odd XIO token from RAG 0 shared with RAG 1",
      .pme_code = 0xff17, /* 65303 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG0_O_XIO_RAG2",
      .pme_desc = "Odd XIO token from RAG 0 shared with RAG 2",
      .pme_code = 0xff18, /* 65304 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG0_O_XIO_RAG3",
      .pme_desc = "Odd XIO token from RAG 0 shared with RAG 3",
      .pme_code = 0xff19, /* 65305 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG0_E_BANK_RAG1",
      .pme_desc = "Even bank token from RAG 0 shared with RAG 1",
      .pme_code = 0xff1a, /* 65306 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG0_E_BANK_RAG2",
      .pme_desc = "Even bank token from RAG 0 shared with RAG 2",
      .pme_code = 0xff1b, /* 65307 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG0_E_BANK_RAG3",
      .pme_desc = "Even bank token from RAG 0 shared with RAG 3",
      .pme_code = 0xff1c, /* 65308 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG0_O_BANK_RAG1",
      .pme_desc = "Odd bank token from RAG 0 shared with RAG 1",
      .pme_code = 0xff1d, /* 65309 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG0_O_BANK_RAG2",
      .pme_desc = "Odd bank token from RAG 0 shared with RAG 2",
      .pme_code = 0xff1e, /* 65310 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG0_O_BANK_RAG3",
      .pme_desc = "Odd bank token from RAG 0 shared with RAG 3",
      .pme_code = 0xff1f, /* 65311 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG1_E_XIO_UNUSED",
      .pme_desc = "Even XIO token was unused by RAG 1.",
      .pme_code = 0xff88, /* 65416 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG1_O_XIO_UNUSED",
      .pme_desc = "Odd XIO token was unused by RAG 1.",
      .pme_code = 0xff89, /* 65417 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG1_E_BANK_UNUSED",
      .pme_desc = "Even bank token was unused by RAG 1.",
      .pme_code = 0xff8a, /* 65418 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG1_O_BANK_UNUSED",
      .pme_desc = "Odd bank token was unused by RAG 1.",
      .pme_code = 0xff8b, /* 65419 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_TOKEN_GRANTED_IOC0",
      .pme_desc = "Token was granted for IOC0.",
      .pme_code = 0xff91, /* 65425 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_TOKEN_GRANTED_IOC1",
      .pme_desc = "Token was granted for IOC1.",
      .pme_code = 0xff92, /* 65426 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG1_E_XIO_WASTED",
      .pme_desc = "Even XIO token was wasted by RAG 1. This is valid only when UE = 1 in TKM_CR.",
      .pme_code = 0xffec, /* 65516 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG1_O_XIO_WASTED",
      .pme_desc = "Odd XIO token was wasted by RAG 1. This is valid only when UE = 1 in TKM_CR.",
      .pme_code = 0xffed, /* 65517 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG1_E_BANK_WASTED",
      .pme_desc = "Even bank token was wasted by RAG 1. This is valid only when UE = 1 in TKM_CR.",
      .pme_code = 0xffee, /* 65518 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG1_O_BANK_WASTED",
      .pme_desc = "Odd bank token was wasted by RAG 1. This is valid only when UE = 1 in TKM_CR.",
      .pme_code = 0xffef, /* 65519 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG1_E_XIO_RAG0",
      .pme_desc = "Even XIO token from RAG 1 shared with RAG 0",
      .pme_code = 0x10050, /* 65616 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG1_E_XIO_RAG2",
      .pme_desc = "Even XIO token from RAG 1 shared with RAG 2",
      .pme_code = 0x10051, /* 65617 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG1_E_XIO_RAG3",
      .pme_desc = "Even XIO token from RAG 1 shared with RAG 3",
      .pme_code = 0x10052, /* 65618 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG1_O_XIO_RAG0",
      .pme_desc = "Odd XIO token from RAG 1 shared with RAG 0",
      .pme_code = 0x10053, /* 65619 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG1_O_XIO_RAG2",
      .pme_desc = "Odd XIO token from RAG 1 shared with RAG 2",
      .pme_code = 0x10054, /* 65620 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG1_O_XIO_RAG3",
      .pme_desc = "Odd XIO token from RAG 1 shared with RAG 3",
      .pme_code = 0x10055, /* 65621 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG1_E_BANK_RAG0",
      .pme_desc = "Even bank token from RAG 1 shared with RAG 0",
      .pme_code = 0x10056, /* 65622 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG1_E_BANK_RAG2",
      .pme_desc = "Even bank token from RAG 1 shared with RAG 2",
      .pme_code = 0x10057, /* 65623 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG1_E_BANK_RAG3",
      .pme_desc = "Even bank token from RAG 1 shared with RAG 3",
      .pme_code = 0x10058, /* 65624 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG1_O_BANK_RAG0",
      .pme_desc = "Odd bank token from RAG 1 shared with RAG 0",
      .pme_code = 0x10059, /* 65625 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG1_O_BANK_RAG2",
      .pme_desc = "Odd bank token from RAG 1 shared with RAG 2",
      .pme_code = 0x1005a, /* 65626 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG1_O_BANK_RAG3",
      .pme_desc = "Odd bank token from RAG 1 shared with RAG 3",
      .pme_code = 0x1005b, /* 65627 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAGU_E_XIO_RAG1",
      .pme_desc = "Even XIO token from RAG U shared with RAG 1",
      .pme_code = 0x1005c, /* 65628 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAGU_O_XIO_RAG1",
      .pme_desc = "Odd XIO token from RAG U shared with RAG 1",
      .pme_code = 0x1005d, /* 65629 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAGU_E_BANK_RAG1",
      .pme_desc = "Even bank token from RAG U shared with RAG 1",
      .pme_code = 0x1005e, /* 65630 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAGU_O_BANK_RAG1",
      .pme_desc = "Odd bank token from RAG U shared with RAG 1",
      .pme_code = 0x1005f, /* 65631 */
      .pme_enable_word = WORD_0_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG2_E_XIO_UNUSED",
      .pme_desc = "Even XIO token unused by RAG 2",
      .pme_code = 0x100e4, /* 65764 */
      .pme_enable_word = WORD_2_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG2_O_XIO_UNUSED",
      .pme_desc = "Odd XIO token unused by RAG 2",
      .pme_code = 0x100e5, /* 65765 */
      .pme_enable_word = WORD_2_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG2_E_BANK_UNUSED",
      .pme_desc = "Even bank token unused by RAG 2",
      .pme_code = 0x100e6, /* 65766 */
      .pme_enable_word = WORD_2_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG2_O_BANK_UNUSED",
      .pme_desc = "Odd bank token unused by RAG 2",
      .pme_code = 0x100e7, /* 65767 */
      .pme_enable_word = WORD_2_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG0_IOIF0_IN_TOKEN_UNUSED",
      .pme_desc = "IOIF0 In token unused by RAG 0",
      .pme_code = 0x100e8, /* 65768 */
      .pme_enable_word = WORD_2_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG0_IOIF0_OUT_TOKEN_UNUSED",
      .pme_desc = "IOIF0 Out token unused by RAG 0",
      .pme_code = 0x100e9, /* 65769 */
      .pme_enable_word = WORD_2_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG0_IOIF1_IN_TOKEN_UNUSED",
      .pme_desc = "IOIF1 In token unused by RAG 0",
      .pme_code = 0x100ea, /* 65770 */
      .pme_enable_word = WORD_2_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG0_IOIF1_OUT_TOKEN_UNUSED",
      .pme_desc = "IOIF1 Out token unused by RAG 0",
      .pme_code = 0x100eb, /* 65771 */
      .pme_enable_word = WORD_2_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG2_E_XIO_WASTED",
      .pme_desc = "Even XIO token wasted by RAG 2",
      .pme_code = 0x10148, /* 65864 */
      .pme_enable_word = WORD_2_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG2_O_XIO_WASTED",
      .pme_desc = "Odd XIO token wasted by RAG 2",
      .pme_code = 0x10149, /* 65865 */
      .pme_enable_word = WORD_2_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG2_E_BANK_WASTED",
      .pme_desc = "Even bank token wasted by RAG 2",
      .pme_code = 0x1014a, /* 65866 */
      .pme_enable_word = WORD_2_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG2_O_BANK_WASTED",
      .pme_desc = "Odd bank token wasted by RAG 2",
      .pme_code = 0x1014b, /* 65867 */
      .pme_enable_word = WORD_2_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG2_E_XIO_RAG0",
      .pme_desc = "Even XIO token from RAG 2 shared with RAG 0",
      .pme_code = 0x101ac, /* 65964 */
      .pme_enable_word = WORD_2_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG2_E_XIO_RAG1",
      .pme_desc = "Even XIO token from RAG 2 shared with RAG 1",
      .pme_code = 0x101ad, /* 65965 */
      .pme_enable_word = WORD_2_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG2_E_XIO_RAG3",
      .pme_desc = "Even XIO token from RAG 2 shared with RAG 3",
      .pme_code = 0x101ae, /* 65966 */
      .pme_enable_word = WORD_2_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG2_O_XIO_RAG0",
      .pme_desc = "Odd XIO token from RAG 2 shared with RAG 0",
      .pme_code = 0x101af, /* 65967 */
      .pme_enable_word = WORD_2_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG2_O_XIO_RAG1",
      .pme_desc = "Odd XIO token from RAG 2 shared with RAG 1",
      .pme_code = 0x101b0, /* 65968 */
      .pme_enable_word = WORD_2_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG2_O_XIO_RAG3",
      .pme_desc = "Odd XIO token from RAG 2 shared with RAG 3",
      .pme_code = 0x101b1, /* 65969 */
      .pme_enable_word = WORD_2_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG2_E_BANK_RAG0",
      .pme_desc = "Even bank token from RAG 2 shared with RAG 0",
      .pme_code = 0x101b2, /* 65970 */
      .pme_enable_word = WORD_2_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG2_E_BANK_RAG1",
      .pme_desc = "Even bank token from RAG 2 shared with RAG 1",
      .pme_code = 0x101b3, /* 65971 */
      .pme_enable_word = WORD_2_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG2_E_BANK_RAG3",
      .pme_desc = "Even bank token from RAG 2 shared with RAG 3",
      .pme_code = 0x101b4, /* 65972 */
      .pme_enable_word = WORD_2_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG2_O_BANK_RAG0",
      .pme_desc = "Odd bank token from RAG 2 shared with RAG 0",
      .pme_code = 0x101b5, /* 65973 */
      .pme_enable_word = WORD_2_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG2_O_BANK_RAG1",
      .pme_desc = "Odd bank token from RAG 2 shared with RAG 1",
      .pme_code = 0x101b6, /* 65974 */
      .pme_enable_word = WORD_2_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG2_O_BANK_RAG3",
      .pme_desc = "Odd bank token from RAG 2 shared with RAG 3",
      .pme_code = 0x101b7, /* 65975 */
      .pme_enable_word = WORD_2_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG0_IOIF0_IN_TOKEN_WASTED",
      .pme_desc = "IOIF0 In token wasted by RAG 0",
      .pme_code = 0x9ef38, /* 651064 */
      .pme_enable_word = WORD_2_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG0_IOIF0_OUT_TOKEN_WASTED",
      .pme_desc = "IOIF0 Out token wasted by RAG 0",
      .pme_code = 0x9ef39, /* 651065 */
      .pme_enable_word = WORD_2_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG0_IOIF1_IN_TOKEN_WASTED",
      .pme_desc = "IOIF1 In token wasted by RAG 0",
      .pme_code = 0x9ef3a, /* 651066 */
      .pme_enable_word = WORD_2_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG0_IOIF1_OUT_TOKEN_WASTED",
      .pme_desc = "IOIF1 Out token wasted by RAG 0",
      .pme_code = 0x9ef3b, /* 651067 */
      .pme_enable_word = WORD_2_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG3_E_XIO_UNUSED",
      .pme_desc = "Even XIO token was unused by RAG 3.",
      .pme_code = 0x9efac, /* 651180 */
      .pme_enable_word = WORD_2_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG3_O_XIO_UNUSED",
      .pme_desc = "Odd XIO token was unused by RAG 3.",
      .pme_code = 0x9efad, /* 651181 */
      .pme_enable_word = WORD_2_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG3_E_BANK_UNUSED",
      .pme_desc = "Even bank token was unused by RAG 3.",
      .pme_code = 0x9efae, /* 651182 */
      .pme_enable_word = WORD_2_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG3_O_BANK_UNUSED",
      .pme_desc = "Odd bank token was unused by RAG 3.",
      .pme_code = 0x9efaf, /* 651183 */
      .pme_enable_word = WORD_2_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG3_E_XIO_WASTED",
      .pme_desc = "Even XIO token wasted by RAG 3",
      .pme_code = 0x9f010, /* 651280 */
      .pme_enable_word = WORD_2_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG3_O_XIO_WASTED",
      .pme_desc = "Odd XIO token wasted by RAG 3",
      .pme_code = 0x9f011, /* 651281 */
      .pme_enable_word = WORD_2_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG3_E_BANK_WASTED",
      .pme_desc = "Even bank token wasted by RAG 3",
      .pme_code = 0x9f012, /* 651282 */
      .pme_enable_word = WORD_2_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG3_O_BANK_WASTED",
      .pme_desc = "Odd bank token wasted by RAG 3",
      .pme_code = 0x9f013, /* 651283 */
      .pme_enable_word = WORD_2_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG3_E_XIO_RAG0",
      .pme_desc = "Even XIO token from RAG 3 shared with RAG 0",
      .pme_code = 0x9f074, /* 651380 */
      .pme_enable_word = WORD_2_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG3_E_XIO_RAG1",
      .pme_desc = "Even XIO token from RAG 3 shared with RAG 1",
      .pme_code = 0x9f075, /* 651381 */
      .pme_enable_word = WORD_2_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG3_E_XIO_RAG2",
      .pme_desc = "Even XIO token from RAG 3 shared with RAG 2",
      .pme_code = 0x9f076, /* 651382 */
      .pme_enable_word = WORD_2_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG3_O_XIO_RAG0",
      .pme_desc = "Odd XIO token from RAG 3 shared with RAG 0",
      .pme_code = 0x9f077, /* 651383 */
      .pme_enable_word = WORD_2_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG3_O_XIO_RAG1",
      .pme_desc = "Odd XIO token from RAG 3 shared with RAG 1",
      .pme_code = 0x9f078, /* 651384 */
      .pme_enable_word = WORD_2_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG3_O_XIO_RAG2",
      .pme_desc = "Odd XIO token from RAG 3 shared with RAG 2",
      .pme_code = 0x9f079, /* 651385 */
      .pme_enable_word = WORD_2_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG3_E_BANK_RAG0",
      .pme_desc = "Even bank token from RAG 3 shared with RAG 0",
      .pme_code = 0x9f07a, /* 651386 */
      .pme_enable_word = WORD_2_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG3_E_BANK_RAG1",
      .pme_desc = "Even bank token from RAG 3 shared with RAG 1",
      .pme_code = 0x9f07b, /* 651387 */
      .pme_enable_word = WORD_2_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG3_E_BANK_RAG2",
      .pme_desc = "Even bank token from RAG 3 shared with RAG 2",
      .pme_code = 0x9f07c, /* 651388 */
      .pme_enable_word = WORD_2_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG3_O_BANK_RAG0",
      .pme_desc = "Odd bank token from RAG 3 shared with RAG 0",
      .pme_code = 0x9f07d, /* 651389 */
      .pme_enable_word = WORD_2_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG3_O_BANK_RAG1",
      .pme_desc = "Odd bank token from RAG 3 shared with RAG 1",
      .pme_code = 0x9f07e, /* 651390 */
      .pme_enable_word = WORD_2_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "EIB_RAG3_O_BANK_RAG2",
      .pme_desc = "Odd bank token from RAG 3 shared with RAG 2",
      .pme_code = 0x9f07f, /* 651391 */
      .pme_enable_word = WORD_2_ONLY,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "MIC_XIO1_READ_CMD_Q_EMPTY",
      .pme_desc = "XIO1 - Read command queue is empty.",
      .pme_code = 0x1bc5, /* 7109 */
      .pme_enable_word = 0xF,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "MIC_XIO1_WRITE_CMD_Q_EMPTY",
      .pme_desc = "XIO1 - Write command queue is empty.",
      .pme_code = 0x1bc6, /* 7110 */
      .pme_enable_word = 0xF,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "MIC_XIO1_READ_CMD_Q_FULL",
      .pme_desc = "XIO1 - Read command queue is full.",
      .pme_code = 0x1bc8, /* 7112 */
      .pme_enable_word = 0xF,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "MIC_XIO1_RESPONDS_READ_RETRY",
      .pme_desc = "XIO1 - MIC responds with a Retry for a read command because the read command queue is full.",
      .pme_code = 0x1bc9, /* 7113 */
      .pme_enable_word = 0xF,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "MIC_XIO1_WRITE_CMD_Q_FULL",
      .pme_desc = "XIO1 - Write command queue is full.",
      .pme_code = 0x1bca, /* 7114 */
      .pme_enable_word = 0xF,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "MIC_XIO1_RESPONDS_WRITE_RETRY",
      .pme_desc = "XIO1 - MIC responds with a Retry for a write command because the write command queue is full.",
      .pme_code = 0x1bcb, /* 7115 */
      .pme_enable_word = 0xF,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "MIC_XIO1_READ_CMD_DISPATCHED",
      .pme_desc = "XIO1 - Read command dispatched; includes high-priority and fast-path reads.",
      .pme_code = 0x1bde, /* 7134 */
      .pme_enable_word = 0xF,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "MIC_XIO1_WRITE_CMD_DISPATCHED",
      .pme_desc = "XIO1 - Write command dispatched.",
      .pme_code = 0x1bdf, /* 7135 */
      .pme_enable_word = 0xF,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "MIC_XIO1_READ_MOD_WRITE_CMD_DISPATCHED",
      .pme_desc = "XIO1 - Read-Modify-Write command (data size < 16 bytes) dispatched.",
      .pme_code = 0x1be0, /* 7136 */
      .pme_enable_word = 0xF,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "MIC_XIO1_REFRESH_DISPATCHED",
      .pme_desc = "XIO1 - Refresh dispatched.",
      .pme_code = 0x1be1, /* 7137 */
      .pme_enable_word = 0xF,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "MIC_XIO1_BYTE_MSK_WRITE_CMD_DISPATCHED",
      .pme_desc = "XIO1 - Byte-masking write command (data size >= 16 bytes) dispatched.",
      .pme_code = 0x1be3, /* 7139 */
      .pme_enable_word = 0xF,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "MIC_XIO1_WRITE_CMD_DISPATCHED_AFTER_READ",
      .pme_desc = "XIO1 - Write command dispatched after a read command was previously dispatched.",
      .pme_code = 0x1be5, /* 7141 */
      .pme_enable_word = 0xF,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "MIC_XIO1_READ_CMD_DISPATCHED_AFTER_WRITE",
      .pme_desc = "XIO1 - Read command dispatched after a write command was previously dispatched.",
      .pme_code = 0x1be6, /* 7142 */
      .pme_enable_word = 0xF,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "MIC_XIO0_READ_CMD_Q_EMPTY",
      .pme_desc = "XIO0 - Read command queue is empty.",
      .pme_code = 0x1c29, /* 7209 */
      .pme_enable_word = 0xF,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "MIC_XIO0_WRITE_CMD_Q_EMPTY",
      .pme_desc = "XIO0 - Write command queue is empty.",
      .pme_code = 0x1c2a, /* 7210 */
      .pme_enable_word = 0xF,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "MIC_XIO0_READ_CMD_Q_FULL",
      .pme_desc = "XIO0 - Read command queue is full.",
      .pme_code = 0x1c2c, /* 7212 */
      .pme_enable_word = 0xF,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "MIC_XIO0_RESPONDS_READ_RETRY",
      .pme_desc = "XIO0 - MIC responds with a Retry for a read command because the read command queue is full.",
      .pme_code = 0x1c2d, /* 7213 */
      .pme_enable_word = 0xF,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "MIC_XIO0_WRITE_CMD_Q_FULL",
      .pme_desc = "XIO0 - Write command queue is full.",
      .pme_code = 0x1c2e, /* 7214 */
      .pme_enable_word = 0xF,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "MIC_XIO0_RESPONDS_WRITE_RETRY",
      .pme_desc = "XIO0 - MIC responds with a Retry for a write command because the write command queue is full.",
      .pme_code = 0x1c2f, /* 7215 */
      .pme_enable_word = 0xF,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "MIC_XIO0_READ_CMD_DISPATCHED",
      .pme_desc = "XIO0 - Read command dispatched; includes high-priority and fast-path reads.",
      .pme_code = 0x1c42, /* 7234 */
      .pme_enable_word = 0xF,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "MIC_XIO0_WRITE_CMD_DISPATCHED",
      .pme_desc = "XIO0 - Write command dispatched.",
      .pme_code = 0x1c43, /* 7235 */
      .pme_enable_word = 0xF,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "MIC_XIO0_READ_MOD_WRITE_CMD_DISPATCHED",
      .pme_desc = "XIO0 - Read-Modify-Write command (data size < 16 bytes) dispatched.",
      .pme_code = 0x1c44, /* 7236 */
      .pme_enable_word = 0xF,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "MIC_XIO0_REFRESH_DISPATCHED",
      .pme_desc = "XIO0 - Refresh dispatched.",
      .pme_code = 0x1c45, /* 7237 */
      .pme_enable_word = 0xF,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "MIC_XIO0_WRITE_CMD_DISPATCHED_AFTER_READ",
      .pme_desc = "XIO0 - Write command dispatched after a read command was previously dispatched.",
      .pme_code = 0x1c49, /* 7241 */
      .pme_enable_word = 0xF,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "MIC_XIO0_READ_CMD_DISPATCHED_AFTER_WRITE",
      .pme_desc = "XIO0 - Read command dispatched after a write command was previously dispatched.",
      .pme_code = 0x1c4a, /* 7242 */
      .pme_enable_word = 0xF,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "MIC_XIO0_WRITE_CMD_DISPATCHED_2",
      .pme_desc = "XIO0 - Write command dispatched.",
      .pme_code = 0x1ca7, /* 7335 */
      .pme_enable_word = 0xF,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "MIC_XIO0_READ_MOD_WRITE_CMD_DISPATCHED_2",
      .pme_desc = "XIO0 - Read-Modify-Write command (data size < 16 bytes) dispatched.",
      .pme_code = 0x1ca8, /* 7336 */
      .pme_enable_word = 0xF,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "MIC_XIO0_REFRESH_DISPATCHED_2",
      .pme_desc = "XIO0 - Refresh dispatched.",
      .pme_code = 0x1ca9, /* 7337 */
      .pme_enable_word = 0xF,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "MIC_XIO0_BYTE_MSK_WRITE_CMD_DISPATCHED",
      .pme_desc = "XIO0 - Byte-masking write command (data size >= 16 bytes) dispatched.",
      .pme_code = 0x1cab, /* 7339 */
      .pme_enable_word = 0xF,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "BIF_IOIF0_TYPEA_DATA_PLG",
      .pme_desc = "Type A data physical layer group (PLG). Does not include header-only or credit-only data PLGs. In IOIF mode, counts I/O device read data; in BIF mode, counts all outbound data.",
      .pme_code = 0x1fb0, /* 8112 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "BIF_IOIF0_TYPEB_DATA_PLG",
      .pme_desc = "Type B data PLG. In IOIF mode, counts I/O device read data; in BIF mode, counts all outbound data.",
      .pme_code = 0x1fb1, /* 8113 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "BIF_IOIF0_IOIF_TYPEA_DATA_PLG",
      .pme_desc = "Type A data PLG. Does not include header-only or credit-only PLGs. In IOIF mode, counts CBE store data to I/O device. Does not apply in BIF mode.",
      .pme_code = 0x1fb2, /* 8114 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "BIF_IOIF0_IOIF_TYPEB_DATA_PLG",
      .pme_desc = "Type B data PLG. In IOIF mode, counts CBE store data to an I/O device. Does not apply in BIF mode.",
      .pme_code = 0x1fb3, /* 8115 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "BIF_IOIF0_DATA_PLG",
      .pme_desc = "Data PLG. Does not include header-only or credit-only PLGs.",
      .pme_code = 0x1fb4, /* 8116 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "BIF_IOIF0_CMD_PLG",
      .pme_desc = "Command PLG (no credit-only PLG). In IOIF mode, counts I/O command or reply PLGs. In BIF mode, counts command/ reflected command or snoop/combined responses.",
      .pme_code = 0x1fb5, /* 8117 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "BIF_IOIF0_TYPEA_TRANSFER",
      .pme_desc = "Type A data transfer regardless of length. Can also be used to count Type A data header PLGs (but not credit-only PLGs).",
      .pme_code = 0x1fb6, /* 8118 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "BIF_IOIF0_TYPEB_TRANSFER",
      .pme_desc = "Type B data transfer.",
      .pme_code = 0x1fb7, /* 8119 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "BIF_IOIF0_CMD_GREDIT_ONLY_PLG",
      .pme_desc = "Command-credit-only command PLG in either IOIF or BIF mode.",
      .pme_code = 0x1fb8, /* 8120 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "BIF_IOIF0_DATA_CREDIT_ONLY_PLG",
      .pme_desc = "Data-credit-only data PLG sent in either IOIF or BIF mode.",
      .pme_code = 0x1fb9, /* 8121 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "BIF_IOIF0_NON_NULL_ENVLP_SENT",
      .pme_desc = "Non-null envelope sent (does not include long envelopes).",
      .pme_code = 0x1fba, /* 8122 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "BIF_IOIF0_NULL_ENVLP_SENT",
      .pme_desc = "Null envelope sent.",
      .pme_code = 0x1fbc, /* 8124 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "BIF_IOIF0_NO_VALID_DATA_SENT",
      .pme_desc = "No valid data sent this cycle.",
      .pme_code = 0x1fbd, /* 8125 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "BIF_IOIF0_NORMAL_ENVLP_SENT",
      .pme_desc = "Normal envelope sent.",
      .pme_code = 0x1fbe, /* 8126 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "BIF_IOIF0_LONG_ENVLP_SENT",
      .pme_desc = "Long envelope sent.",
      .pme_code = 0x1fbf, /* 8127 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "BIF_IOIF0_NULL_PLG_INSERTED",
      .pme_desc = "A Null PLG inserted in an outgoing envelope.",
      .pme_code = 0x1fc0, /* 8128 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "BIF_IOIF0_OUTBOUND_ENV_ARRAY_FULL",
      .pme_desc = "Outbound envelope array is full.",
      .pme_code = 0x1fc1, /* 8129 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "BIF_IOIF1_TYPEB_TRANSFER",
      .pme_desc = "Type B data transfer.",
      .pme_code = 0x201b, /* 8219 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "BIF_IOIF0_NULL_ENVLP_RECV",
      .pme_desc = "Null envelope received.",
      .pme_code = 0x206d, /* 8301 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "BIF_IOIF0_CMD_PLG_2",
      .pme_desc = "Command PLG, but not credit-only PLG. In IOIF mode, counts I/O command or reply PLGs. In BIF mode, counts command/reflected command or snoop/combined responses.",
      .pme_code = 0x207a, /* 8314 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "BIF_IOIF0_CMD_GREDIT_ONLY_PLG_2",
      .pme_desc = "Command-credit-only command PLG.",
      .pme_code = 0x207b, /* 8315 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "BIF_IOIF0_NORMAL_ENVLP_RECV",
      .pme_desc = "Normal envelope received is good.",
      .pme_code = 0x2080, /* 8320 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "BIF_IOIF0_LONG_ENVLP_RECV",
      .pme_desc = "Long envelope received is good.",
      .pme_code = 0x2081, /* 8321 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "BIF_IOIF0_DATA_GREDIT_ONLY_PLG_2",
      .pme_desc = "Data-credit-only data PLG in either IOIF or BIF mode; will count a maximum of one per envelope.",
      .pme_code = 0x2082, /* 8322 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "BIF_IOIF0_NON_NULL_ENVLP",
      .pme_desc = "Non-null envelope; does not include long envelopes; includes retried envelopes.",
      .pme_code = 0x2083, /* 8323 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "BIF_IOIF0_DATA_GRANT_RECV",
      .pme_desc = "Data grant received.",
      .pme_code = 0x2084, /* 8324 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "BIF_IOIF0_DATA_PLG_2",
      .pme_desc = "Data PLG. Does not include header-only or credit-only PLGs.",
      .pme_code = 0x2088, /* 8328 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "BIF_IOIF0_TYPEA_TRANSFER_2",
      .pme_desc = "Type A data transfer regardless of length. Can also be used to count Type A data header PLGs, but not credit-only PLGs.",
      .pme_code = 0x2089, /* 8329 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "BIF_IOIF0_TYPEB_TRANSFER_2",
      .pme_desc = "Type B data transfer.",
      .pme_code = 0x208a, /* 8330 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "BIF_IOIF1_NULL_ENVLP_RECV",
      .pme_desc = "Null envelope received.",
      .pme_code = 0x20d1, /* 8401 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "BIF_IOIF1_CMD_PLG_2",
      .pme_desc = "Command PLG (no credit-only PLG). Counts I/O command or reply PLGs.",
      .pme_code = 0x20de, /* 8414 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "BIF_IOIF1_CMD_GREDIT_ONLY_PLG_2",
      .pme_desc = "Command-credit-only command PLG.",
      .pme_code = 0x20df, /* 8415 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "BIF_IOIF1_NORMAL_ENVLP_RECV",
      .pme_desc = "Normal envelope received is good.",
      .pme_code = 0x20e4, /* 8420 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "BIF_IOIF1_LONG_ENVLP_RECV",
      .pme_desc = "Long envelope received is good.",
      .pme_code = 0x20e5, /* 8421 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "BIF_IOIF1_DATA_GREDIT_ONLY_PLG_2",
      .pme_desc = "Data-credit-only data PLG received; will count a maximum of one per envelope.",
      .pme_code = 0x20e6, /* 8422 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "BIF_IOIF1_NON_NULL_ENVLP",
      .pme_desc = "Non-Null envelope received; does not include long envelopes; includes retried envelopes.",
      .pme_code = 0x20e7, /* 8423 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "BIF_IOIF1_DATA_GRANT_RECV",
      .pme_desc = "Data grant received.",
      .pme_code = 0x20e8, /* 8424 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "BIF_IOIF1_DATA_PLG_2",
      .pme_desc = "Data PLG received. Does not include header-only or credit-only PLGs.",
      .pme_code = 0x20ec, /* 8428 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "BIF_IOIF1_TYPEA_TRANSFER_2",
      .pme_desc = "Type I A data transfer regardless of length. Can also be used to count Type A data header PLGs (but not credit-only PLGs).",
      .pme_code = 0x20ed, /* 8429 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "BIF_IOIF1_TYPEB_TRANSFER_2",
      .pme_desc = "Type B data transfer received.",
      .pme_code = 0x20ee, /* 8430 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "IOC_MMIO_READ_IOIF1",
      .pme_desc = "Received MMIO read targeted to IOIF1.",
      .pme_code = 0x213c, /* 8508 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "IOC_MMIO_WRITE_IOIF1",
      .pme_desc = "Received MMIO write targeted to IOIF1.",
      .pme_code = 0x213d, /* 8509 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "IOC_MMIO_READ_IOIF0",
      .pme_desc = "Received MMIO read targeted to IOIF0.",
      .pme_code = 0x213e, /* 8510 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "IOC_MMIO_WRITE_IOIF0",
      .pme_desc = "Received MMIO write targeted to IOIF0.",
      .pme_code = 0x213f, /* 8511 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "IOC_CMD_TO_IOIF0",
      .pme_desc = "Sent command to IOIF0.",
      .pme_code = 0x2140, /* 8512 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "IOC_CMD_TO_IOIF1",
      .pme_desc = "Sent command to IOIF1.",
      .pme_code = 0x2141, /* 8513 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "IOC_IOIF0_MATRIX3_OCCUPIED",
      .pme_desc = "IOIF0 Dependency Matrix 3 is occupied by a dependent command.",
      .pme_code = 0x219d, /* 8605 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_BOTH_TYPE,
   },
   {
      .pme_name = "IOC_IOIF0_MATRIX4_OCCUPIED",
      .pme_desc = "IOIF0 Dependency Matrix 4 is occupied by a dependent command.",
      .pme_code = 0x219e, /* 8606 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_BOTH_TYPE,
   },
   {
      .pme_name = "IOC_IOIF0_MATRIX5_OCCUPIED",
      .pme_desc = "IOIF0 Dependency Matrix 5 is occupied by a dependent command.",
      .pme_code = 0x219f, /* 8607 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_BOTH_TYPE,
   },
   {
      .pme_name = "IOC_DMA_READ_IOIF0",
      .pme_desc = "Received read request from IOIF0.",
      .pme_code = 0x21a2, /* 8610 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "IOC_DMA_WRITE_IOIF0",
      .pme_desc = "Received write request from IOIF0.",
      .pme_code = 0x21a3, /* 8611 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "IOC_INTERRUPT_IOIF0",
      .pme_desc = "Received interrupt from the IOIF0.",
      .pme_code = 0x21a6, /* 8614 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "IOC_IOIF0_REQ_TOKEN_E_MEM",
      .pme_desc = "IOIF0 request for token for even memory banks 0-14.",
      .pme_code = 0x220c, /* 8716 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "IOC_IOIF0_REQ_TOKEN_O_MEM",
      .pme_desc = "IOIF0 request for token for odd memory banks 1-15.",
      .pme_code = 0x220d, /* 8717 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "IOC_IOIF0_REQ_TOKEN_1357",
      .pme_desc = "IOIF0 request for token type 1, 3, 5, or 7.",
      .pme_code = 0x220e, /* 8718 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "IOC_IOIF0_REQ_TOKEN_9111315",
      .pme_desc = "IOIF0 request for token type 9, 11, 13, or 15.",
      .pme_code = 0x220f, /* 8719 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "IOC_IOIF0_REQ_TOKEN_16",
      .pme_desc = "IOIF0 request for token type 16.",
      .pme_code = 0x2214, /* 8724 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "IOC_IOIF0_REQ_TOKEN_17",
      .pme_desc = "IOIF0 request for token type 17.",
      .pme_code = 0x2215, /* 8725 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "IOC_IOIF0_REQ_TOKEN_18",
      .pme_desc = "IOIF0 request for token type 18.",
      .pme_code = 0x2216, /* 8726 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "IOC_IOIF0_REQ_TOKEN_19",
      .pme_desc = "IOIF0 request for token type 19.",
      .pme_code = 0x2217, /* 8727 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_CUMULATIVE_LEN,
   },
   {
      .pme_name = "IOC_IOPT_CACHE_HIT",
      .pme_desc = "I/O page table cache hit for commands from IOIF.",
      .pme_code = 0x2260, /* 8800 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "IOC_IOPT_CACHE_MISS",
      .pme_desc = "I/O page table cache miss for commands from IOIF.",
      .pme_code = 0x2261, /* 8801 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "IOC_IOST_CACHE_HIT",
      .pme_desc = "I/O segment table cache hit.",
      .pme_code = 0x2263, /* 8803 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "IOC_IOST_CACHE_MISS",
      .pme_desc = "I/O segment table cache miss.",
      .pme_code = 0x2264, /* 8804 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "IOC_INTERRUPT_FROM_SPU",
      .pme_desc = "Interrupt received from any SPU (reflected cmd when IIC has sent ACK response).",
      .pme_code = 0x2278, /* 8824 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "IOC_IIC_INTERRUPT_TO_PPU_TH0",
      .pme_desc = "Internal interrupt controller (IIC) generated interrupt to PPU thread 0.",
      .pme_code = 0x2279, /* 8825 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "IOC_IIC_INTERRUPT_TO_PPU_TH1",
      .pme_desc = "IIC generated interrupt to PPU thread 1.",
      .pme_code = 0x227a, /* 8826 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "IOC_RECV_EXTERNAL_INTERRUPT_TO_TH0",
      .pme_desc = "Received external interrupt (using MMIO) from PPU to PPU thread 0.",
      .pme_code = 0x227b, /* 8827 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
   {
      .pme_name = "IOC_RECV_EXTERNAL_INTERRUPT_TO_TH1",
      .pme_desc = "Received external interrupt (using MMIO) from PPU to PPU thread 1.",
      .pme_code = 0x227c, /* 8828 */
      .pme_enable_word = WORD_0_AND_2,
      .pme_freq = PFM_CELL_PME_FREQ_HALF,
      .pme_type = COUNT_TYPE_OCCURRENCE,
   },
};
/*--- The number of events : 435 ---*/
#define PME_CELL_EVENT_COUNT 	(sizeof(cell_pe)/sizeof(pme_cell_entry_t))
