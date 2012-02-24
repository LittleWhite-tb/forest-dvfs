static const sparc_entry_t niagara1_pe [] =
{
   /* PIC1 Niagara-1 events */
   {
      .name = "Instr_cnt",
      .desc = "Number of instructions completed",
      .ctrl = PME_CTRL_S1,
      .code = 0x0,
   },

   /* PIC0 Niagara-1 events */
   {
      .name = "SB_full",
      .desc = "Store-buffer full",
      .ctrl = PME_CTRL_S0,
      .code = 0x0,
   },
   {
      .name = "FP_instr_cnt",
      .desc = "FPU instructions",
      .ctrl = PME_CTRL_S0,
      .code = 0x1,
   },
   {
      .name = "IC_miss",
      .desc = "I-cache miss",
      .ctrl = PME_CTRL_S0,
      .code = 0x2,
   },
   {
      .name = "DC_miss",
      .desc = "D-cache miss",
      .ctrl = PME_CTRL_S0,
      .code = 0x3,
   },
   {
      .name = "ITLB_miss",
      .desc = "I-TLB miss",
      .ctrl = PME_CTRL_S0,
      .code = 0x4,
   },
   {
      .name = "DTLB_miss",
      .desc = "D-TLB miss",
      .ctrl = PME_CTRL_S0,
      .code = 0x5,
   },
   {
      .name = "L2_imiss",
      .desc = "E-cache instruction fetch miss",
      .ctrl = PME_CTRL_S0,
      .code = 0x6,
   },
   {
      .name = "L2_dmiss_ld",
      .desc = "E-cache data load miss",
      .ctrl = PME_CTRL_S0,
      .code = 0x7,
   },

};
#define PME_SPARC_NIAGARA1_EVENT_COUNT	   (sizeof(niagara1_pe)/sizeof(sparc_entry_t))
