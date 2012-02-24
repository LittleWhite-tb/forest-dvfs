static const sparc_entry_t ultra3i_pe [] =
{
   /* These two must always be first.  */
   {
      .name = "Cycle_cnt",
      .desc = "Accumulated cycles",
      .ctrl = PME_CTRL_S0 | PME_CTRL_S1,
      .code = 0x0,
   },
   {
      .name = "Instr_cnt",
      .desc = "Number of instructions completed",
      .ctrl = PME_CTRL_S0 | PME_CTRL_S1,
      .code = 0x1,
   },

   /* PIC0 events common to all UltraSPARC processors */
   {
      .name = "Dispatch0_IC_miss",
      .desc = "I-buffer is empty from I-Cache miss",
      .ctrl = PME_CTRL_S0,
      .code = 0x2,
   },
   {
      .name = "IC_ref",
      .desc = "I-cache refrences",
      .ctrl = PME_CTRL_S0,
      .code = 0x8,
   },
   {
      .name = "DC_rd",
      .desc = "D-cache read references (including accesses that subsequently trap)",
      .ctrl = PME_CTRL_S0,
      .code = 0x9,
   },
   {
      .name = "DC_wr",
      .desc = "D-cache store accesses (including cacheable stores that subsequently trap)",
      .ctrl = PME_CTRL_S0,
      .code = 0xa,
   },
   {
      .name = "EC_ref",
      .desc = "E-cache references",
      .ctrl = PME_CTRL_S0,
      .code = 0xc,
   },
   {
      .name = "EC_snoop_inv",
      .desc = "L2-cache invalidates generated from a snoop by a remote processor",
      .ctrl = PME_CTRL_S0,
      .code = 0xe,
   },

   /* PIC1 events common to all UltraSPARC processors */
   {
      .name = "Dispatch0_mispred",
      .desc = "I-buffer is empty from Branch misprediction",
      .ctrl = PME_CTRL_S1,
      .code = 0x2,
   },
   {
      .name = "EC_wb",
      .desc = "Dirty sub-blocks that produce writebacks due to L2-cache miss events",
      .ctrl = PME_CTRL_S1,
      .code = 0xd,
   },
   {
      .name = "EC_snoop_cb",
      .desc = "L2-cache copybacks generated from a snoop by a remote processor",
      .ctrl = PME_CTRL_S1,
      .code = 0xe,
   },

   /* PIC0 events common to all UltraSPARC-III/III+/IIIi processors  */
   {
      .name = "Dispatch0_br_target",
      .desc = "I-buffer is empty due to a branch target address calculation",
      .ctrl = PME_CTRL_S0,
      .code = 0x3,
   },
   {
      .name = "Dispatch0_2nd_br",
      .desc = "Stall cycles due to having two branch instructions line-up in one 4-instruction group causing the second branch in the group to be re-fetched, delaying it's entrance into the I-buffer",
      .ctrl = PME_CTRL_S0,
      .code = 0x4,
   },
   {
      .name = "Rstall_storeQ",
      .desc = "R-stage stall for a store instruction which is the next instruction to be executed, but it stailled due to the store queue being full",
      .ctrl = PME_CTRL_S0,
      .code = 0x5,
   },
   {
      .name = "Rstall_IU_use",
      .desc = "R-stage stall for an event that the next instruction to be executed depends on the result of a preceeding integer instruction in the pipeline that is not yet available",
      .ctrl = PME_CTRL_S0,
      .code = 0x6,
   },
   {
      .name = "EC_write_hit_RTO",
      .desc = "W-cache exclusive requests that hit L2-cache in S, O, or Os state and thus, do a read-to-own bus transaction",
      .ctrl = PME_CTRL_S0,
      .code = 0xd,
   },
   {
      .name = "EC_rd_miss",
      .desc = "L2-cache miss events (including atomics) from D-cache events",
      .ctrl = PME_CTRL_S0,
      .code = 0xf,
   },
   {
      .name = "PC_port0_rd",
      .desc = "P-cache cacheable FP loads to the first port (general purpose load path to D-cache and P-cache via MS pipeline)",
      .ctrl = PME_CTRL_S0,
      .code = 0x10,
   },
   {
      .name = "SI_snoop",
      .desc = "Counts snoops from remote processor (s) including RTS, RTSR, RTO, RTOR, RS, RSR, RTSM, and WS",
      .ctrl = PME_CTRL_S0,
      .code = 0x11,
   },
   {
      .name = "SI_ciq_flow",
      .desc = "Counts system clock cycles when the flow control (PauseOut) signal is asserted",
      .ctrl = PME_CTRL_S0,
      .code = 0x12,
   },
   {
      .name = "SI_owned",
      .desc = "Counts events where owned_in is asserted on bus requests from the local processor",
      .ctrl = PME_CTRL_S0,
      .code = 0x13,
   },
   {
      .name = "SW_count0",
      .desc = "Counts software-generated occurrences of 'sethi %hi (0xfc000), %g0' instruction",
      .ctrl = PME_CTRL_S0,
      .code = 0x14,
   },
   {
      .name = "IU_Stat_Br_miss_taken",
      .desc = "Retired branches that were predicted to be taken, but in fact were not taken",
      .ctrl = PME_CTRL_S0,
      .code = 0x15,
   },
   {
      .name = "IU_Stat_Br_Count_taken",
      .desc = "Retired taken branches",
      .ctrl = PME_CTRL_S0,
      .code = 0x16,
   },
   {
      .name = "Dispatch0_rs_mispred",
      .desc = "I-buffer is empty due to a Return Address Stack misprediction",
      .ctrl = PME_CTRL_S0,
      .code = 0x4,
   },
   {
      .name = "FA_pipe_completion",
      .desc = "Instructions that complete execution on the FPG ALU pipelines",
      .ctrl = PME_CTRL_S0,
      .code = 0x18,
   },

   /* PIC1 events common to all UltraSPARC-III/III+/IIIi processors  */
   {
      .name = "IC_miss_cancelled",
      .desc = "I-cache misses cancelled due to mis-speculation, recycle, or other events",
      .ctrl = PME_CTRL_S1,
      .code = 0x3,
   },
   {
      .name = "Re_FPU_bypass",
      .desc = "Stall due to recirculation when an FPU bypass condition that does not have a direct bypass path occurs",
      .ctrl = PME_CTRL_S1,
      .code = 0x5,
   },
   {
      .name = "Re_DC_miss",
      .desc = "Stall due to loads that miss D-cache and get recirculated",
      .ctrl = PME_CTRL_S1,
      .code = 0x6,
   },
   {
      .name = "Re_EC_miss",
      .desc = "Stall due to loads that miss L2-cache and get recirculated",
      .ctrl = PME_CTRL_S1,
      .code = 0x7,
   },
   {
      .name = "IC_miss",
      .desc = "I-cache misses, including fetches from mis-speculated execution paths which are later cancelled",
      .ctrl = PME_CTRL_S1,
      .code = 0x8,
   },
   {
      .name = "DC_rd_miss",
      .desc = "Recirculated loads that miss the D-cache",
      .ctrl = PME_CTRL_S1,
      .code = 0x9,
   },
   {
      .name = "DC_wr_miss",
      .desc = "D-cache store accesses that miss D-cache",
      .ctrl = PME_CTRL_S1,
      .code = 0xa,
   },
   {
      .name = "Rstall_FP_use",
      .desc = "R-stage stall for an event that the next instruction to be executed depends on the result of a preceeding floating-point instruction in the pipeline that is not yet available",
      .ctrl = PME_CTRL_S1,
      .code = 0xb,
   },
   {
      .name = "EC_misses",
      .desc = "E-cache misses",
      .ctrl = PME_CTRL_S1,
      .code = 0xc,
   },
   {
      .name = "EC_ic_miss",
      .desc = "L2-cache read misses from I-cache requests",
      .ctrl = PME_CTRL_S1,
      .code = 0xf,
   },
   {
      .name = "Re_PC_miss",
      .desc = "Stall due to recirculation when a prefetch cache miss occurs on a prefetch predicted second load",
      .ctrl = PME_CTRL_S1,
      .code = 0x10,
   },
   {
      .name = "ITLB_miss",
      .desc = "I-TLB miss traps taken",
      .ctrl = PME_CTRL_S1,
      .code = 0x11,
   },
   {
      .name = "DTLB_miss",
      .desc = "Memory reference instructions which trap due to D-TLB miss",
      .ctrl = PME_CTRL_S1,
      .code = 0x12,
   },
   {
      .name = "WC_miss",
      .desc = "W-cache misses",
      .ctrl = PME_CTRL_S1,
      .code = 0x13,
   },
   {
      .name = "WC_snoop_cb",
      .desc = "W-cache copybacks generated by a snoop from a remote processor",
      .ctrl = PME_CTRL_S1,
      .code = 0x14,
   },
   {
      .name = "WC_scrubbed",
      .desc = "W-cache hits to clean lines",
      .ctrl = PME_CTRL_S1,
      .code = 0x15,
   },
   {
      .name = "WC_wb_wo_read",
      .desc = "W-cache writebacks not requiring a read",
      .ctrl = PME_CTRL_S1,
      .code = 0x16,
   },
   {
      .name = "PC_soft_hit",
      .desc = "FP loads that hit a P-cache line that was prefetched by a software-prefetch instruction",
      .ctrl = PME_CTRL_S1,
      .code = 0x18,
   },
   {
      .name = "PC_snoop_inv",
      .desc = "P-cache invalidates that were generated by a snoop from a remote processor and stores by a local processor",
      .ctrl = PME_CTRL_S1,
      .code = 0x19,
   },
   {
      .name = "PC_hard_hit",
      .desc = "FP loads that hit a P-cache line that was prefetched by a hardware prefetch",
      .ctrl = PME_CTRL_S1,
      .code = 0x1a,
   },
   {
      .name = "PC_port1_rd",
      .desc = "P-cache cacheable FP loads to the second port (memory and out-of-pipeline instruction execution loads via the A0 and A1 pipelines)",
      .ctrl = PME_CTRL_S1,
      .code = 0x1b,
   },
   {
      .name = "SW_count1",
      .desc = "Counts software-generated occurrences of 'sethi %hi (0xfc000), %g0' instruction",
      .ctrl = PME_CTRL_S1,
      .code = 0x1c,
   },
   {
      .name = "IU_Stat_Br_miss_untaken",
      .desc = "Retired branches that were predicted to be untaken, but in fact were taken",
      .ctrl = PME_CTRL_S1,
      .code = 0x1d,
   },
   {
      .name = "IU_Stat_Br_Count_untaken",
      .desc = "Retired untaken branches",
      .ctrl = PME_CTRL_S1,
      .code = 0x1e,
   },
   {
      .name = "PC_MS_miss",
      .desc = "FP loads through the MS pipeline that miss P-cache",
      .ctrl = PME_CTRL_S1,
      .code = 0x1f,
   },
   {
      .name = "Re_RAW_miss",
      .desc = "Stall due to recirculation when there is a load in the E-stage which has a non-bypassable read-after-write hazard with an earlier store instruction",
      .ctrl = PME_CTRL_S1,
      .code = 0x26,
   },
   {
      .name = "FM_pipe_completion",
      .desc = "Instructions that complete execution on the FPG Multiply pipelines",
      .ctrl = PME_CTRL_S0,
      .code = 0x27,
   },


   /* PIC0 memory controller events specific to UltraSPARC-IIIi processors */
   {
      .name = "MC_read_dispatched",
      .desc = "DDR 64-byte reads dispatched by the MIU",
      .ctrl = PME_CTRL_S0,
      .code = 0x20,
   },
   {
      .name = "MC_write_dispatched",
      .desc = "DDR 64-byte writes dispatched by the MIU",
      .ctrl = PME_CTRL_S0,
      .code = 0x21,
   },
   {
      .name = "MC_read_returned_to_JBU",
      .desc = "64-byte reads that return data to JBU",
      .ctrl = PME_CTRL_S0,
      .code = 0x22,
   },
   {
      .name = "MC_msl_busy_stall",
      .desc = "Stall cycles due to msl_busy",
      .ctrl = PME_CTRL_S0,
      .code = 0x23,
   },
   {
      .name = "MC_mdb_overflow_stall",
      .desc = "Stall cycles due to potential memory data buffer overflow",
      .ctrl = PME_CTRL_S0,
      .code = 0x24,
   },
   {
      .name = "MC_miu_spec_request",
      .desc = "Speculative requests accepted by MIU",
      .ctrl = PME_CTRL_S0,
      .code = 0x25,
   },

   /* PIC1 memory controller events specific to UltraSPARC-IIIi processors */
   {
      .name = "MC_reads",
      .desc = "64-byte reads by the MSL",
      .ctrl = PME_CTRL_S1,
      .code = 0x20,
   },
   {
      .name = "MC_writes",
      .desc = "64-byte writes by the MSL",
      .ctrl = PME_CTRL_S1,
      .code = 0x21,
   },
   {
      .name = "MC_page_close_stall",
      .desc = "DDR page conflicts",
      .ctrl = PME_CTRL_S1,
      .code = 0x22,
   },

   /* PIC1 events specific to UltraSPARC-III+/IIIi */
   {
      .name = "Re_DC_missovhd",
      .desc = "Used to measure D-cache stall counts seperatedly for L2-cache hits and misses.  This counter is used with the recirculation and cache access events to seperately calculate the D-cache loads that hit and miss the L2-cache",
      .ctrl = PME_CTRL_S1,
      .code = 0x4,
   },
};
#define PME_SPARC_ULTRA3I_EVENT_COUNT	   (sizeof(ultra3i_pe)/sizeof(sparc_entry_t))
