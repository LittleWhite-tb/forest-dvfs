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
 * PMU: nhm_unc (Intel Nehalem uncore)
 */

static const intel_x86_umask_t nhm_unc_unc_dram_open []=
{
   {
      .uname  = "CH0",
      .udesc  = "DRAM Channel 0 open comamnds issued for read or write",
      .ucode = 0x100,
   },
   {
      .uname  = "CH1",
      .udesc  = "DRAM Channel 1 open comamnds issued for read or write",
      .ucode = 0x200,
   },
   {
      .uname  = "CH2",
      .udesc  = "DRAM Channel 2 open comamnds issued for read or write",
      .ucode = 0x400,
   },
};

static const intel_x86_umask_t nhm_unc_unc_dram_page_close []=
{
   {
      .uname  = "CH0",
      .udesc  = "DRAM Channel 0 page close",
      .ucode = 0x100,
   },
   {
      .uname  = "CH1",
      .udesc  = "DRAM Channel 1 page close",
      .ucode = 0x200,
   },
   {
      .uname  = "CH2",
      .udesc  = "DRAM Channel 2 page close",
      .ucode = 0x400,
   },
};

static const intel_x86_umask_t nhm_unc_unc_dram_page_miss []=
{
   {
      .uname  = "CH0",
      .udesc  = "DRAM Channel 0 page miss",
      .ucode = 0x100,
   },
   {
      .uname  = "CH1",
      .udesc  = "DRAM Channel 1 page miss",
      .ucode = 0x200,
   },
   {
      .uname  = "CH2",
      .udesc  = "DRAM Channel 2 page miss",
      .ucode = 0x400,
   },
};

static const intel_x86_umask_t nhm_unc_unc_dram_pre_all []=
{
   {
      .uname  = "CH0",
      .udesc  = "DRAM Channel 0 precharge all commands",
      .ucode = 0x100,
   },
   {
      .uname  = "CH1",
      .udesc  = "DRAM Channel 1 precharge all commands",
      .ucode = 0x200,
   },
   {
      .uname  = "CH2",
      .udesc  = "DRAM Channel 2 precharge all commands",
      .ucode = 0x400,
   },
};

static const intel_x86_umask_t nhm_unc_unc_dram_read_cas []=
{
   {
      .uname  = "CH0",
      .udesc  = "DRAM Channel 0 read CAS commands",
      .ucode = 0x100,
   },
   {
      .uname  = "AUTOPRE_CH0",
      .udesc  = "DRAM Channel 0 read CAS auto page close commands",
      .ucode = 0x200,
   },
   {
      .uname  = "CH1",
      .udesc  = "DRAM Channel 1 read CAS commands",
      .ucode = 0x400,
   },
   {
      .uname  = "AUTOPRE_CH1",
      .udesc  = "DRAM Channel 1 read CAS auto page close commands",
      .ucode = 0x800,
   },
   {
      .uname  = "CH2",
      .udesc  = "DRAM Channel 2 read CAS commands",
      .ucode = 0x1000,
   },
   {
      .uname  = "AUTOPRE_CH2",
      .udesc  = "DRAM Channel 2 read CAS auto page close commands",
      .ucode = 0x2000,
   },
};

static const intel_x86_umask_t nhm_unc_unc_dram_refresh []=
{
   {
      .uname  = "CH0",
      .udesc  = "DRAM Channel 0 refresh commands",
      .ucode = 0x100,
   },
   {
      .uname  = "CH1",
      .udesc  = "DRAM Channel 1 refresh commands",
      .ucode = 0x200,
   },
   {
      .uname  = "CH2",
      .udesc  = "DRAM Channel 2 refresh commands",
      .ucode = 0x400,
   },
};

static const intel_x86_umask_t nhm_unc_unc_dram_write_cas []=
{
   {
      .uname  = "CH0",
      .udesc  = "DRAM Channel 0 write CAS commands",
      .ucode = 0x100,
   },
   {
      .uname  = "AUTOPRE_CH0",
      .udesc  = "DRAM Channel 0 write CAS auto page close commands",
      .ucode = 0x200,
   },
   {
      .uname  = "CH1",
      .udesc  = "DRAM Channel 1 write CAS commands",
      .ucode = 0x400,
   },
   {
      .uname  = "AUTOPRE_CH1",
      .udesc  = "DRAM Channel 1 write CAS auto page close commands",
      .ucode = 0x800,
   },
   {
      .uname  = "CH2",
      .udesc  = "DRAM Channel 2 write CAS commands",
      .ucode = 0x1000,
   },
   {
      .uname  = "AUTOPRE_CH2",
      .udesc  = "DRAM Channel 2 write CAS auto page close commands",
      .ucode = 0x2000,
   },
};

static const intel_x86_umask_t nhm_unc_unc_gq_alloc []=
{
   {
      .uname  = "READ_TRACKER",
      .udesc  = "GQ read tracker requests",
      .ucode = 0x100,
   },
   {
      .uname  = "RT_LLC_MISS",
      .udesc  = "GQ read tracker LLC misses",
      .ucode = 0x200,
   },
   {
      .uname  = "RT_TO_LLC_RESP",
      .udesc  = "GQ read tracker LLC requests",
      .ucode = 0x400,
   },
   {
      .uname  = "RT_TO_RTID_ACQUIRED",
      .udesc  = "GQ read tracker LLC miss to RTID acquired",
      .ucode = 0x800,
   },
   {
      .uname  = "WT_TO_RTID_ACQUIRED",
      .udesc  = "GQ write tracker LLC miss to RTID acquired",
      .ucode = 0x1000,
   },
   {
      .uname  = "WRITE_TRACKER",
      .udesc  = "GQ write tracker LLC misses",
      .ucode = 0x2000,
   },
   {
      .uname  = "PEER_PROBE_TRACKER",
      .udesc  = "GQ peer probe tracker requests",
      .ucode = 0x4000,
   },
};

static const intel_x86_umask_t nhm_unc_unc_gq_cycles_full []=
{
   {
      .uname  = "READ_TRACKER",
      .udesc  = "Cycles GQ read tracker is full.",
      .ucode = 0x100,
   },
   {
      .uname  = "WRITE_TRACKER",
      .udesc  = "Cycles GQ write tracker is full.",
      .ucode = 0x200,
   },
   {
      .uname  = "PEER_PROBE_TRACKER",
      .udesc  = "Cycles GQ peer probe tracker is full.",
      .ucode = 0x400,
   },
};

static const intel_x86_umask_t nhm_unc_unc_gq_cycles_not_empty []=
{
   {
      .uname  = "READ_TRACKER",
      .udesc  = "Cycles GQ read tracker is busy",
      .ucode = 0x100,
   },
   {
      .uname  = "WRITE_TRACKER",
      .udesc  = "Cycles GQ write tracker is busy",
      .ucode = 0x200,
   },
   {
      .uname  = "PEER_PROBE_TRACKER",
      .udesc  = "Cycles GQ peer probe tracker is busy",
      .ucode = 0x400,
   },
};

static const intel_x86_umask_t nhm_unc_unc_gq_data_from []=
{
   {
      .uname  = "QPI",
      .udesc  = "Cycles GQ data is imported from Quickpath interface",
      .ucode = 0x100,
   },
   {
      .uname  = "QMC",
      .udesc  = "Cycles GQ data is imported from Quickpath memory interface",
      .ucode = 0x200,
   },
   {
      .uname  = "LLC",
      .udesc  = "Cycles GQ data is imported from LLC",
      .ucode = 0x400,
   },
   {
      .uname  = "CORES_02",
      .udesc  = "Cycles GQ data is imported from Cores 0 and 2",
      .ucode = 0x800,
   },
   {
      .uname  = "CORES_13",
      .udesc  = "Cycles GQ data is imported from Cores 1 and 3",
      .ucode = 0x1000,
   },
};

static const intel_x86_umask_t nhm_unc_unc_gq_data_to []=
{
   {
      .uname  = "QPI_QMC",
      .udesc  = "Cycles GQ data sent to the QPI or QMC",
      .ucode = 0x100,
   },
   {
      .uname  = "LLC",
      .udesc  = "Cycles GQ data sent to LLC",
      .ucode = 0x200,
   },
   {
      .uname  = "CORES",
      .udesc  = "Cycles GQ data sent to cores",
      .ucode = 0x400,
   },
};

static const intel_x86_umask_t nhm_unc_unc_llc_hits []=
{
   {
      .uname  = "READ",
      .udesc  = "Number of LLC read hits",
      .ucode = 0x100,
   },
   {
      .uname  = "WRITE",
      .udesc  = "Number of LLC write hits",
      .ucode = 0x200,
   },
   {
      .uname  = "PROBE",
      .udesc  = "Number of LLC peer probe hits",
      .ucode = 0x400,
   },
   {
      .uname  = "ANY",
      .udesc  = "Number of LLC hits",
      .ucode = 0x300,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL,
   },
};

static const intel_x86_umask_t nhm_unc_unc_llc_lines_in []=
{
   {
      .uname  = "M_STATE",
      .udesc  = "LLC lines allocated in M state",
      .ucode = 0x100,
   },
   {
      .uname  = "E_STATE",
      .udesc  = "LLC lines allocated in E state",
      .ucode = 0x200,
   },
   {
      .uname  = "S_STATE",
      .udesc  = "LLC lines allocated in S state",
      .ucode = 0x400,
   },
   {
      .uname  = "F_STATE",
      .udesc  = "LLC lines allocated in F state",
      .ucode = 0x800,
   },
   {
      .uname  = "ANY",
      .udesc  = "LLC lines allocated",
      .ucode = 0xf00,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL,
   },
};

static const intel_x86_umask_t nhm_unc_unc_llc_lines_out []=
{
   {
      .uname  = "M_STATE",
      .udesc  = "LLC lines victimized in M state",
      .ucode = 0x100,
   },
   {
      .uname  = "E_STATE",
      .udesc  = "LLC lines victimized in E state",
      .ucode = 0x200,
   },
   {
      .uname  = "S_STATE",
      .udesc  = "LLC lines victimized in S state",
      .ucode = 0x400,
   },
   {
      .uname  = "I_STATE",
      .udesc  = "LLC lines victimized in I state",
      .ucode = 0x800,
   },
   {
      .uname  = "F_STATE",
      .udesc  = "LLC lines victimized in F state",
      .ucode = 0x1000,
   },
   {
      .uname  = "ANY",
      .udesc  = "LLC lines victimized",
      .ucode = 0x1f00,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL,
   },
};

static const intel_x86_umask_t nhm_unc_unc_llc_miss []=
{
   {
      .uname  = "READ",
      .udesc  = "Number of LLC read misses",
      .ucode = 0x100,
   },
   {
      .uname  = "WRITE",
      .udesc  = "Number of LLC write misses",
      .ucode = 0x200,
   },
   {
      .uname  = "PROBE",
      .udesc  = "Number of LLC peer probe misses",
      .ucode = 0x400,
   },
   {
      .uname  = "ANY",
      .udesc  = "Number of LLC misses",
      .ucode = 0x300,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL,
   },
};

static const intel_x86_umask_t nhm_unc_unc_qhl_address_conflicts []=
{
   {
      .uname  = "2WAY",
      .udesc  = "QHL 2 way address conflicts",
      .ucode = 0x200,
   },
   {
      .uname  = "3WAY",
      .udesc  = "QHL 3 way address conflicts",
      .ucode = 0x400,
   },
};

static const intel_x86_umask_t nhm_unc_unc_qhl_conflict_cycles []=
{
   {
      .uname  = "IOH",
      .udesc  = "QHL IOH Tracker conflict cycles",
      .ucode = 0x100,
   },
   {
      .uname  = "REMOTE",
      .udesc  = "QHL Remote Tracker conflict cycles",
      .ucode = 0x200,
   },
   {
      .uname  = "LOCAL",
      .udesc  = "QHL Local Tracker conflict cycles",
      .ucode = 0x400,
   },
};

static const intel_x86_umask_t nhm_unc_unc_qhl_cycles_full []=
{
   {
      .uname  = "REMOTE",
      .udesc  = "Cycles QHL  Remote Tracker is full",
      .ucode = 0x200,
   },
   {
      .uname  = "LOCAL",
      .udesc  = "Cycles QHL Local Tracker is full",
      .ucode = 0x400,
   },
   {
      .uname  = "IOH",
      .udesc  = "Cycles QHL IOH Tracker is full",
      .ucode = 0x100,
   },
};

static const intel_x86_umask_t nhm_unc_unc_qhl_cycles_not_empty []=
{
   {
      .uname  = "IOH",
      .udesc  = "Cycles QHL IOH is busy",
      .ucode = 0x100,
   },
   {
      .uname  = "REMOTE",
      .udesc  = "Cycles QHL Remote Tracker is busy",
      .ucode = 0x200,
   },
   {
      .uname  = "LOCAL",
      .udesc  = "Cycles QHL Local Tracker is busy",
      .ucode = 0x400,
   },
};

static const intel_x86_umask_t nhm_unc_unc_qhl_frc_ack_cnflts []=
{
   {
      .uname  = "LOCAL",
      .udesc  = "QHL FrcAckCnflts sent to local home",
      .ucode = 0x400,
      .uflags= INTEL_X86_DFL,
   },
};

static const intel_x86_umask_t nhm_unc_unc_qhl_occupancy []=
{
   {
      .uname  = "IOH",
      .udesc  = "Cycles QHL IOH Tracker Allocate to Deallocate Read Occupancy",
      .ucode = 0x100,
   },
   {
      .uname  = "REMOTE",
      .udesc  = "Cycles QHL Remote Tracker Allocate to Deallocate Read Occupancy",
      .ucode = 0x200,
   },
   {
      .uname  = "LOCAL",
      .udesc  = "Cycles QHL Local Tracker Allocate to Deallocate Read Occupancy",
      .ucode = 0x400,
   },
};

static const intel_x86_umask_t nhm_unc_unc_qhl_requests []=
{
   {
      .uname  = "LOCAL_READS",
      .udesc  = "Quickpath Home Logic local read requests",
      .ucode = 0x1000,
   },
   {
      .uname  = "LOCAL_WRITES",
      .udesc  = "Quickpath Home Logic local write requests",
      .ucode = 0x2000,
   },
   {
      .uname  = "REMOTE_READS",
      .udesc  = "Quickpath Home Logic remote read requests",
      .ucode = 0x400,
   },
   {
      .uname  = "IOH_READS",
      .udesc  = "Quickpath Home Logic IOH read requests",
      .ucode = 0x100,
   },
   {
      .uname  = "IOH_WRITES",
      .udesc  = "Quickpath Home Logic IOH write requests",
      .ucode = 0x200,
   },
   {
      .uname  = "REMOTE_WRITES",
      .udesc  = "Quickpath Home Logic remote write requests",
      .ucode = 0x800,
   },
};

static const intel_x86_umask_t nhm_unc_unc_qmc_busy []=
{
   {
      .uname  = "READ_CH0",
      .udesc  = "Cycles QMC channel 0 busy with a read request",
      .ucode = 0x100,
   },
   {
      .uname  = "READ_CH1",
      .udesc  = "Cycles QMC channel 1 busy with a read request",
      .ucode = 0x200,
   },
   {
      .uname  = "READ_CH2",
      .udesc  = "Cycles QMC channel 2 busy with a read request",
      .ucode = 0x400,
   },
   {
      .uname  = "WRITE_CH0",
      .udesc  = "Cycles QMC channel 0 busy with a write request",
      .ucode = 0x800,
   },
   {
      .uname  = "WRITE_CH1",
      .udesc  = "Cycles QMC channel 1 busy with a write request",
      .ucode = 0x1000,
   },
   {
      .uname  = "WRITE_CH2",
      .udesc  = "Cycles QMC channel 2 busy with a write request",
      .ucode = 0x2000,
   },
};

static const intel_x86_umask_t nhm_unc_unc_qmc_cancel []=
{
   {
      .uname  = "CH0",
      .udesc  = "QMC channel 0 cancels",
      .ucode = 0x100,
   },
   {
      .uname  = "CH1",
      .udesc  = "QMC channel 1 cancels",
      .ucode = 0x200,
   },
   {
      .uname  = "CH2",
      .udesc  = "QMC channel 2 cancels",
      .ucode = 0x400,
   },
   {
      .uname  = "ANY",
      .udesc  = "QMC cancels",
      .ucode = 0x700,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL,
   },
};

static const intel_x86_umask_t nhm_unc_unc_qmc_critical_priority_reads []=
{
   {
      .uname  = "CH0",
      .udesc  = "QMC channel 0 critical priority read requests",
      .ucode = 0x100,
   },
   {
      .uname  = "CH1",
      .udesc  = "QMC channel 1 critical priority read requests",
      .ucode = 0x200,
   },
   {
      .uname  = "CH2",
      .udesc  = "QMC channel 2 critical priority read requests",
      .ucode = 0x400,
   },
   {
      .uname  = "ANY",
      .udesc  = "QMC critical priority read requests",
      .ucode = 0x700,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL,
   },
};

static const intel_x86_umask_t nhm_unc_unc_qmc_high_priority_reads []=
{
   {
      .uname  = "CH0",
      .udesc  = "QMC channel 0 high priority read requests",
      .ucode = 0x100,
   },
   {
      .uname  = "CH1",
      .udesc  = "QMC channel 1 high priority read requests",
      .ucode = 0x200,
   },
   {
      .uname  = "CH2",
      .udesc  = "QMC channel 2 high priority read requests",
      .ucode = 0x400,
   },
   {
      .uname  = "ANY",
      .udesc  = "QMC high priority read requests",
      .ucode = 0x700,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL,
   },
};

static const intel_x86_umask_t nhm_unc_unc_qmc_isoc_full []=
{
   {
      .uname  = "READ_CH0",
      .udesc  = "Cycles DRAM channel 0 full with isochronous read requests",
      .ucode = 0x100,
   },
   {
      .uname  = "READ_CH1",
      .udesc  = "Cycles DRAM channel 1 full with isochronous read requests",
      .ucode = 0x200,
   },
   {
      .uname  = "READ_CH2",
      .udesc  = "Cycles DRAM channel 2 full with isochronous read requests",
      .ucode = 0x400,
   },
   {
      .uname  = "WRITE_CH0",
      .udesc  = "Cycles DRAM channel 0 full with isochronous write requests",
      .ucode = 0x800,
   },
   {
      .uname  = "WRITE_CH1",
      .udesc  = "Cycles DRAM channel 1 full with isochronous write requests",
      .ucode = 0x1000,
   },
   {
      .uname  = "WRITE_CH2",
      .udesc  = "Cycles DRAM channel 2 full with isochronous write requests",
      .ucode = 0x2000,
   },
};

static const intel_x86_umask_t nhm_unc_unc_imc_isoc_occupancy []=
{
   {
      .uname  = "CH0",
      .udesc  = "IMC channel 0 isochronous read request occupancy",
      .ucode = 0x100,
   },
   {
      .uname  = "CH1",
      .udesc  = "IMC channel 1 isochronous read request occupancy",
      .ucode = 0x200,
   },
   {
      .uname  = "CH2",
      .udesc  = "IMC channel 2 isochronous read request occupancy",
      .ucode = 0x400,
   },
   {
      .uname  = "ANY",
      .udesc  = "IMC isochronous read request occupancy",
      .ucode = 0x700,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL,
   },
};

static const intel_x86_umask_t nhm_unc_unc_qmc_normal_full []=
{
   {
      .uname  = "READ_CH0",
      .udesc  = "Cycles DRAM channel 0 full with normal read requests",
      .ucode = 0x100,
   },
   {
      .uname  = "READ_CH1",
      .udesc  = "Cycles DRAM channel 1 full with normal read requests",
      .ucode = 0x200,
   },
   {
      .uname  = "READ_CH2",
      .udesc  = "Cycles DRAM channel 2 full with normal read requests",
      .ucode = 0x400,
   },
   {
      .uname  = "WRITE_CH0",
      .udesc  = "Cycles DRAM channel 0 full with normal write requests",
      .ucode = 0x800,
   },
   {
      .uname  = "WRITE_CH1",
      .udesc  = "Cycles DRAM channel 1 full with normal write requests",
      .ucode = 0x1000,
   },
   {
      .uname  = "WRITE_CH2",
      .udesc  = "Cycles DRAM channel 2 full with normal write requests",
      .ucode = 0x2000,
   },
};

static const intel_x86_umask_t nhm_unc_unc_qmc_normal_reads []=
{
   {
      .uname  = "CH0",
      .udesc  = "QMC channel 0 normal read requests",
      .ucode = 0x100,
   },
   {
      .uname  = "CH1",
      .udesc  = "QMC channel 1 normal read requests",
      .ucode = 0x200,
   },
   {
      .uname  = "CH2",
      .udesc  = "QMC channel 2 normal read requests",
      .ucode = 0x400,
   },
   {
      .uname  = "ANY",
      .udesc  = "QMC normal read requests",
      .ucode = 0x700,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL,
   },
};

static const intel_x86_umask_t nhm_unc_unc_qmc_occupancy []=
{
   {
      .uname  = "CH0",
      .udesc  = "IMC channel 0 normal read request occupancy",
      .ucode = 0x100,
   },
   {
      .uname  = "CH1",
      .udesc  = "IMC channel 1 normal read request occupancy",
      .ucode = 0x200,
   },
   {
      .uname  = "CH2",
      .udesc  = "IMC channel 2 normal read request occupancy",
      .ucode = 0x400,
   },
};

static const intel_x86_umask_t nhm_unc_unc_qmc_priority_updates []=
{
   {
      .uname  = "CH0",
      .udesc  = "QMC channel 0 priority updates",
      .ucode = 0x100,
   },
   {
      .uname  = "CH1",
      .udesc  = "QMC channel 1 priority updates",
      .ucode = 0x200,
   },
   {
      .uname  = "CH2",
      .udesc  = "QMC channel 2 priority updates",
      .ucode = 0x400,
   },
   {
      .uname  = "ANY",
      .udesc  = "QMC priority updates",
      .ucode = 0x700,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL,
   },
};

static const intel_x86_umask_t nhm_unc_unc_qmc_writes []=
{
   {
      .uname  = "FULL_CH0",
      .udesc  = "QMC channel 0 full cache line writes",
      .ucode = 0x100,
      .grpid = 0,
   },
   {
      .uname  = "FULL_CH1",
      .udesc  = "QMC channel 1 full cache line writes",
      .ucode = 0x200,
      .grpid = 0,
   },
   {
      .uname  = "FULL_CH2",
      .udesc  = "QMC channel 2 full cache line writes",
      .ucode = 0x400,
      .grpid = 0,
   },
   {
      .uname  = "FULL_ANY",
      .udesc  = "QMC full cache line writes",
      .ucode = 0x700,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL,
      .grpid = 0,
   },
   {
      .uname  = "PARTIAL_CH0",
      .udesc  = "QMC channel 0 partial cache line writes",
      .ucode = 0x800,
      .grpid = 1,
   },
   {
      .uname  = "PARTIAL_CH1",
      .udesc  = "QMC channel 1 partial cache line writes",
      .ucode = 0x1000,
      .grpid = 1,
   },
   {
      .uname  = "PARTIAL_CH2",
      .udesc  = "QMC channel 2 partial cache line writes",
      .ucode = 0x2000,
      .grpid = 1,
   },
   {
      .uname  = "PARTIAL_ANY",
      .udesc  = "QMC partial cache line writes",
      .ucode = 0x3800,
      .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL,
      .grpid = 1,
   },
};

static const intel_x86_umask_t nhm_unc_unc_qpi_rx_no_ppt_credit []=
{
   {
      .uname  = "STALLS_LINK_0",
      .udesc  = "Link 0 snoop stalls due to no PPT entry",
      .ucode = 0x100,
   },
   {
      .uname  = "STALLS_LINK_1",
      .udesc  = "Link 1 snoop stalls due to no PPT entry",
      .ucode = 0x200,
   },
};

static const intel_x86_umask_t nhm_unc_unc_qpi_tx_header []=
{
   {
      .uname  = "BUSY_LINK_0",
      .udesc  = "Cycles link 0 outbound header busy",
      .ucode = 0x200,
   },
   {
      .uname  = "BUSY_LINK_1",
      .udesc  = "Cycles link 1 outbound header busy",
      .ucode = 0x800,
   },
};

static const intel_x86_umask_t nhm_unc_unc_qpi_tx_stalled_multi_flit []=
{
   {
      .uname  = "DRS_LINK_0",
      .udesc  = "Cycles QPI outbound link 0 DRS stalled",
      .ucode = 0x100,
   },
   {
      .uname  = "NCB_LINK_0",
      .udesc  = "Cycles QPI outbound link 0 NCB stalled",
      .ucode = 0x200,
   },
   {
      .uname  = "NCS_LINK_0",
      .udesc  = "Cycles QPI outbound link 0 NCS stalled",
      .ucode = 0x400,
   },
   {
      .uname  = "DRS_LINK_1",
      .udesc  = "Cycles QPI outbound link 1 DRS stalled",
      .ucode = 0x800,
   },
   {
      .uname  = "NCB_LINK_1",
      .udesc  = "Cycles QPI outbound link 1 NCB stalled",
      .ucode = 0x1000,
   },
   {
      .uname  = "NCS_LINK_1",
      .udesc  = "Cycles QPI outbound link 1 NCS stalled",
      .ucode = 0x2000,
   },
   {
      .uname  = "LINK_0",
      .udesc  = "Cycles QPI outbound link 0 multi flit stalled",
      .ucode = 0x700,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "LINK_1",
      .udesc  = "Cycles QPI outbound link 1 multi flit stalled",
      .ucode = 0x3800,
      .uflags= INTEL_X86_NCOMBO,
   },
};

static const intel_x86_umask_t nhm_unc_unc_qpi_tx_stalled_single_flit []=
{
   {
      .uname  = "HOME_LINK_0",
      .udesc  = "Cycles QPI outbound link 0 HOME stalled",
      .ucode = 0x100,
   },
   {
      .uname  = "SNOOP_LINK_0",
      .udesc  = "Cycles QPI outbound link 0 SNOOP stalled",
      .ucode = 0x200,
   },
   {
      .uname  = "NDR_LINK_0",
      .udesc  = "Cycles QPI outbound link 0 NDR stalled",
      .ucode = 0x400,
   },
   {
      .uname  = "HOME_LINK_1",
      .udesc  = "Cycles QPI outbound link 1 HOME stalled",
      .ucode = 0x800,
   },
   {
      .uname  = "SNOOP_LINK_1",
      .udesc  = "Cycles QPI outbound link 1 SNOOP stalled",
      .ucode = 0x1000,
   },
   {
      .uname  = "NDR_LINK_1",
      .udesc  = "Cycles QPI outbound link 1 NDR stalled",
      .ucode = 0x2000,
   },
   {
      .uname  = "LINK_0",
      .udesc  = "Cycles QPI outbound link 0 single flit stalled",
      .ucode = 0x700,
      .uflags= INTEL_X86_NCOMBO,
   },
   {
      .uname  = "LINK_1",
      .udesc  = "Cycles QPI outbound link 1 single flit stalled",
      .ucode = 0x3800,
      .uflags= INTEL_X86_NCOMBO,
   },
};

static const intel_x86_umask_t nhm_unc_unc_snp_resp_to_local_home []=
{
   {
      .uname  = "I_STATE",
      .udesc  = "Local home snoop response - LLC does not have cache line",
      .ucode = 0x100,
   },
   {
      .uname  = "S_STATE",
      .udesc  = "Local home snoop response - LLC has  cache line in S state",
      .ucode = 0x200,
   },
   {
      .uname  = "FWD_S_STATE",
      .udesc  = "Local home snoop response - LLC forwarding cache line in S state.",
      .ucode = 0x400,
   },
   {
      .uname  = "FWD_I_STATE",
      .udesc  = "Local home snoop response - LLC has forwarded a modified cache line",
      .ucode = 0x800,
   },
   {
      .uname  = "CONFLICT",
      .udesc  = "Local home conflict snoop response",
      .ucode = 0x1000,
   },
   {
      .uname  = "WB",
      .udesc  = "Local home snoop response - LLC has cache line in the M state",
      .ucode = 0x2000,
   },
};

static const intel_x86_umask_t nhm_unc_unc_snp_resp_to_remote_home []=
{
   {
      .uname  = "I_STATE",
      .udesc  = "Remote home snoop response - LLC does not have cache line",
      .ucode = 0x100,
   },
   {
      .uname  = "S_STATE",
      .udesc  = "Remote home snoop response - LLC has  cache line in S state",
      .ucode = 0x200,
   },
   {
      .uname  = "FWD_S_STATE",
      .udesc  = "Remote home snoop response - LLC forwarding cache line in S state.",
      .ucode = 0x400,
   },
   {
      .uname  = "FWD_I_STATE",
      .udesc  = "Remote home snoop response - LLC has forwarded a modified cache line",
      .ucode = 0x800,
   },
   {
      .uname  = "CONFLICT",
      .udesc  = "Remote home conflict snoop response",
      .ucode = 0x1000,
   },
   {
      .uname  = "WB",
      .udesc  = "Remote home snoop response - LLC has cache line in the M state",
      .ucode = 0x2000,
   },
   {
      .uname  = "HITM",
      .udesc  = "Remote home snoop response - LLC HITM",
      .ucode = 0x2400,
      .uflags= INTEL_X86_NCOMBO,
   },
};

static const intel_x86_entry_t intel_nhm_unc_pe []=
{
   {
      .name   = "UNC_CLK_UNHALTED",
      .desc   = "Uncore clockticks.",
      .modmsk =0x0,
      .cntmsk = 0x100000,
      .code = 0xff,
   },
   {
      .name   = "UNC_DRAM_OPEN",
      .desc   = "DRAM open comamnds issued for read or write",
      .modmsk = NHM_UNC_ATTRS,
      .cntmsk = 0x1fe00000,
      .code = 0x60,
      .numasks = LIBPFM_ARRAY_SIZE (nhm_unc_unc_dram_open),
      .ngrp = 1,
      .umasks = nhm_unc_unc_dram_open,
   },
   {
      .name   = "UNC_DRAM_PAGE_CLOSE",
      .desc   = "DRAM page close due to idle timer expiration",
      .modmsk = NHM_UNC_ATTRS,
      .cntmsk = 0x1fe00000,
      .code = 0x61,
      .numasks = LIBPFM_ARRAY_SIZE (nhm_unc_unc_dram_page_close),
      .ngrp = 1,
      .umasks = nhm_unc_unc_dram_page_close,
   },
   {
      .name   = "UNC_DRAM_PAGE_MISS",
      .desc   = "DRAM Channel 0 page miss",
      .modmsk = NHM_UNC_ATTRS,
      .cntmsk = 0x1fe00000,
      .code = 0x62,
      .numasks = LIBPFM_ARRAY_SIZE (nhm_unc_unc_dram_page_miss),
      .ngrp = 1,
      .umasks = nhm_unc_unc_dram_page_miss,
   },
   {
      .name   = "UNC_DRAM_PRE_ALL",
      .desc   = "DRAM Channel 0 precharge all commands",
      .modmsk = NHM_UNC_ATTRS,
      .cntmsk = 0x1fe00000,
      .code = 0x66,
      .numasks = LIBPFM_ARRAY_SIZE (nhm_unc_unc_dram_pre_all),
      .ngrp = 1,
      .umasks = nhm_unc_unc_dram_pre_all,
   },
   {
      .name   = "UNC_DRAM_READ_CAS",
      .desc   = "DRAM Channel 0 read CAS commands",
      .modmsk = NHM_UNC_ATTRS,
      .cntmsk = 0x1fe00000,
      .code = 0x63,
      .numasks = LIBPFM_ARRAY_SIZE (nhm_unc_unc_dram_read_cas),
      .ngrp = 1,
      .umasks = nhm_unc_unc_dram_read_cas,
   },
   {
      .name   = "UNC_DRAM_REFRESH",
      .desc   = "DRAM Channel 0 refresh commands",
      .modmsk = NHM_UNC_ATTRS,
      .cntmsk = 0x1fe00000,
      .code = 0x65,
      .numasks = LIBPFM_ARRAY_SIZE (nhm_unc_unc_dram_refresh),
      .ngrp = 1,
      .umasks = nhm_unc_unc_dram_refresh,
   },
   {
      .name   = "UNC_DRAM_WRITE_CAS",
      .desc   = "DRAM Channel 0 write CAS commands",
      .modmsk = NHM_UNC_ATTRS,
      .cntmsk = 0x1fe00000,
      .code = 0x64,
      .numasks = LIBPFM_ARRAY_SIZE (nhm_unc_unc_dram_write_cas),
      .ngrp = 1,
      .umasks = nhm_unc_unc_dram_write_cas,
   },
   {
      .name   = "UNC_GQ_ALLOC",
      .desc   = "GQ read tracker requests",
      .modmsk = NHM_UNC_ATTRS,
      .cntmsk = 0x1fe00000,
      .code = 0x3,
      .numasks = LIBPFM_ARRAY_SIZE (nhm_unc_unc_gq_alloc),
      .ngrp = 1,
      .umasks = nhm_unc_unc_gq_alloc,
   },
   {
      .name   = "UNC_GQ_CYCLES_FULL",
      .desc   = "Cycles GQ read tracker is full.",
      .modmsk = NHM_UNC_ATTRS,
      .cntmsk = 0x1fe00000,
      .code = 0x0,
      .numasks = LIBPFM_ARRAY_SIZE (nhm_unc_unc_gq_cycles_full),
      .ngrp = 1,
      .umasks = nhm_unc_unc_gq_cycles_full,
   },
   {
      .name   = "UNC_GQ_CYCLES_NOT_EMPTY",
      .desc   = "Cycles GQ read tracker is busy",
      .modmsk = NHM_UNC_ATTRS,
      .cntmsk = 0x1fe00000,
      .code = 0x1,
      .numasks = LIBPFM_ARRAY_SIZE (nhm_unc_unc_gq_cycles_not_empty),
      .ngrp = 1,
      .umasks = nhm_unc_unc_gq_cycles_not_empty,
   },
   {
      .name   = "UNC_GQ_DATA_FROM",
      .desc   = "Cycles GQ data is imported",
      .modmsk = NHM_UNC_ATTRS,
      .cntmsk = 0x1fe00000,
      .code = 0x4,
      .numasks = LIBPFM_ARRAY_SIZE (nhm_unc_unc_gq_data_from),
      .ngrp = 1,
      .umasks = nhm_unc_unc_gq_data_from,
   },
   {
      .name   = "UNC_GQ_DATA_TO",
      .desc   = "Cycles GQ data is exported",
      .modmsk = NHM_UNC_ATTRS,
      .cntmsk = 0x1fe00000,
      .code = 0x5,
      .numasks = LIBPFM_ARRAY_SIZE (nhm_unc_unc_gq_data_to),
      .ngrp = 1,
      .umasks = nhm_unc_unc_gq_data_to,
   },
   {
      .name   = "UNC_LLC_HITS",
      .desc   = "Number of LLC read hits",
      .modmsk = NHM_UNC_ATTRS,
      .cntmsk = 0x1fe00000,
      .code = 0x8,
      .numasks = LIBPFM_ARRAY_SIZE (nhm_unc_unc_llc_hits),
      .ngrp = 1,
      .umasks = nhm_unc_unc_llc_hits,
   },
   {
      .name   = "UNC_LLC_LINES_IN",
      .desc   = "LLC lines allocated in M state",
      .modmsk = NHM_UNC_ATTRS,
      .cntmsk = 0x1fe00000,
      .code = 0xa,
      .numasks = LIBPFM_ARRAY_SIZE (nhm_unc_unc_llc_lines_in),
      .ngrp = 1,
      .umasks = nhm_unc_unc_llc_lines_in,
   },
   {
      .name   = "UNC_LLC_LINES_OUT",
      .desc   = "LLC lines victimized in M state",
      .modmsk = NHM_UNC_ATTRS,
      .cntmsk = 0x1fe00000,
      .code = 0xb,
      .numasks = LIBPFM_ARRAY_SIZE (nhm_unc_unc_llc_lines_out),
      .ngrp = 1,
      .umasks = nhm_unc_unc_llc_lines_out,
   },
   {
      .name   = "UNC_LLC_MISS",
      .desc   = "Number of LLC read misses",
      .modmsk = NHM_UNC_ATTRS,
      .cntmsk = 0x1fe00000,
      .code = 0x9,
      .numasks = LIBPFM_ARRAY_SIZE (nhm_unc_unc_llc_miss),
      .ngrp = 1,
      .umasks = nhm_unc_unc_llc_miss,
   },
   {
      .name   = "UNC_QHL_ADDRESS_CONFLICTS",
      .desc   = "QHL 2 way address conflicts",
      .modmsk = NHM_UNC_ATTRS,
      .cntmsk = 0x1fe00000,
      .code = 0x24,
      .numasks = LIBPFM_ARRAY_SIZE (nhm_unc_unc_qhl_address_conflicts),
      .ngrp = 1,
      .umasks = nhm_unc_unc_qhl_address_conflicts,
   },
   {
      .name   = "UNC_QHL_CONFLICT_CYCLES",
      .desc   = "QHL IOH Tracker conflict cycles",
      .modmsk = NHM_UNC_ATTRS,
      .cntmsk = 0x1fe00000,
      .code = 0x25,
      .numasks = LIBPFM_ARRAY_SIZE (nhm_unc_unc_qhl_conflict_cycles),
      .ngrp = 1,
      .umasks = nhm_unc_unc_qhl_conflict_cycles,
   },
   {
      .name   = "UNC_QHL_CYCLES_FULL",
      .desc   = "Cycles QHL  Remote Tracker is full",
      .modmsk = NHM_UNC_ATTRS,
      .cntmsk = 0x1fe00000,
      .code = 0x21,
      .numasks = LIBPFM_ARRAY_SIZE (nhm_unc_unc_qhl_cycles_full),
      .ngrp = 1,
      .umasks = nhm_unc_unc_qhl_cycles_full,
   },
   {
      .name   = "UNC_QHL_CYCLES_NOT_EMPTY",
      .desc   = "Cycles QHL Tracker is not empty",
      .modmsk =0x0,
      .cntmsk = 0x1fe00000,
      .code = 0x22,
      .numasks = LIBPFM_ARRAY_SIZE (nhm_unc_unc_qhl_cycles_not_empty),
      .ngrp = 1,
      .umasks = nhm_unc_unc_qhl_cycles_not_empty,
   },
   {
      .name   = "UNC_QHL_FRC_ACK_CNFLTS",
      .desc   = "QHL FrcAckCnflts sent to local home",
      .modmsk = NHM_UNC_ATTRS,
      .cntmsk = 0x1fe00000,
      .code = 0x33,
      .numasks = LIBPFM_ARRAY_SIZE (nhm_unc_unc_qhl_frc_ack_cnflts),
      .ngrp = 1,
      .umasks = nhm_unc_unc_qhl_frc_ack_cnflts,
   },
   {
      .name   = "UNC_QHL_OCCUPANCY",
      .desc   = "Cycles QHL Tracker Allocate to Deallocate Read Occupancy",
      .modmsk = NHM_UNC_ATTRS,
      .cntmsk = 0x1fe00000,
      .code = 0x23,
      .numasks = LIBPFM_ARRAY_SIZE (nhm_unc_unc_qhl_occupancy),
      .ngrp = 1,
      .umasks = nhm_unc_unc_qhl_occupancy,
   },
   {
      .name   = "UNC_QHL_REQUESTS",
      .desc   = "Quickpath Home Logic local read requests",
      .modmsk = NHM_UNC_ATTRS,
      .cntmsk = 0x1fe00000,
      .code = 0x20,
      .numasks = LIBPFM_ARRAY_SIZE (nhm_unc_unc_qhl_requests),
      .ngrp = 1,
      .umasks = nhm_unc_unc_qhl_requests,
   },
   {
      .name   = "UNC_QHL_TO_QMC_BYPASS",
      .desc   = "Number of requests to QMC that bypass QHL",
      .modmsk = NHM_UNC_ATTRS,
      .cntmsk = 0x1fe00000,
      .code = 0x26,
   },
   {
      .name   = "UNC_QMC_BUSY",
      .desc   = "Cycles QMC busy with a read request",
      .modmsk = NHM_UNC_ATTRS,
      .cntmsk = 0x1fe00000,
      .code = 0x29,
      .numasks = LIBPFM_ARRAY_SIZE (nhm_unc_unc_qmc_busy),
      .ngrp = 1,
      .umasks = nhm_unc_unc_qmc_busy,
   },
   {
      .name   = "UNC_QMC_CANCEL",
      .desc   = "QMC cancels",
      .modmsk = NHM_UNC_ATTRS,
      .cntmsk = 0x1fe00000,
      .code = 0x30,
      .numasks = LIBPFM_ARRAY_SIZE (nhm_unc_unc_qmc_cancel),
      .ngrp = 1,
      .umasks = nhm_unc_unc_qmc_cancel,
   },
   {
      .name   = "UNC_QMC_CRITICAL_PRIORITY_READS",
      .desc   = "QMC critical priority read requests",
      .modmsk = NHM_UNC_ATTRS,
      .cntmsk = 0x1fe00000,
      .code = 0x2e,
      .numasks = LIBPFM_ARRAY_SIZE (nhm_unc_unc_qmc_critical_priority_reads),
      .ngrp = 1,
      .umasks = nhm_unc_unc_qmc_critical_priority_reads,
   },
   {
      .name   = "UNC_QMC_HIGH_PRIORITY_READS",
      .desc   = "QMC high priority read requests",
      .modmsk = NHM_UNC_ATTRS,
      .cntmsk = 0x1fe00000,
      .code = 0x2d,
      .numasks = LIBPFM_ARRAY_SIZE (nhm_unc_unc_qmc_high_priority_reads),
      .ngrp = 1,
      .umasks = nhm_unc_unc_qmc_high_priority_reads,
   },
   {
      .name   = "UNC_QMC_ISOC_FULL",
      .desc   = "Cycles DRAM full with isochronous (ISOC) read requests",
      .modmsk = NHM_UNC_ATTRS,
      .cntmsk = 0x1fe00000,
      .code = 0x28,
      .numasks = LIBPFM_ARRAY_SIZE (nhm_unc_unc_qmc_isoc_full),
      .ngrp = 1,
      .umasks = nhm_unc_unc_qmc_isoc_full,
   },
   {
      .name   = "UNC_IMC_ISOC_OCCUPANCY",
      .desc   = "IMC isochronous (ISOC) Read Occupancy",
      .modmsk = NHM_UNC_ATTRS,
      .cntmsk = 0x1fe00000,
      .code = 0x2b,
      .numasks = LIBPFM_ARRAY_SIZE (nhm_unc_unc_imc_isoc_occupancy),
      .ngrp = 1,
      .umasks = nhm_unc_unc_imc_isoc_occupancy,
   },
   {
      .name   = "UNC_QMC_NORMAL_FULL",
      .desc   = "Cycles DRAM full with normal read requests",
      .modmsk = NHM_UNC_ATTRS,
      .cntmsk = 0x1fe00000,
      .code = 0x27,
      .numasks = LIBPFM_ARRAY_SIZE (nhm_unc_unc_qmc_normal_full),
      .ngrp = 1,
      .umasks = nhm_unc_unc_qmc_normal_full,
   },
   {
      .name   = "UNC_QMC_NORMAL_READS",
      .desc   = "QMC normal read requests",
      .modmsk = NHM_UNC_ATTRS,
      .cntmsk = 0x1fe00000,
      .code = 0x2c,
      .numasks = LIBPFM_ARRAY_SIZE (nhm_unc_unc_qmc_normal_reads),
      .ngrp = 1,
      .umasks = nhm_unc_unc_qmc_normal_reads,
   },
   {
      .name   = "UNC_QMC_OCCUPANCY",
      .desc   = "QMC Occupancy",
      .modmsk = NHM_UNC_ATTRS,
      .cntmsk = 0x1fe00000,
      .code = 0x2a,
      .numasks = LIBPFM_ARRAY_SIZE (nhm_unc_unc_qmc_occupancy),
      .ngrp = 1,
      .umasks = nhm_unc_unc_qmc_occupancy,
   },
   {
      .name   = "UNC_QMC_PRIORITY_UPDATES",
      .desc   = "QMC priority updates",
      .modmsk = NHM_UNC_ATTRS,
      .cntmsk = 0x1fe00000,
      .code = 0x31,
      .numasks = LIBPFM_ARRAY_SIZE (nhm_unc_unc_qmc_priority_updates),
      .ngrp = 1,
      .umasks = nhm_unc_unc_qmc_priority_updates,
   },
   {
      .name   = "UNC_QMC_WRITES",
      .desc   = "QMC cache line writes",
      .modmsk = NHM_UNC_ATTRS,
      .cntmsk = 0x1fe00000,
      .code = 0x2f,
      .flags= INTEL_X86_GRP_EXCL,
      .numasks = LIBPFM_ARRAY_SIZE (nhm_unc_unc_qmc_writes),
      .ngrp = 2,
      .umasks = nhm_unc_unc_qmc_writes,
   },
   {
      .name   = "UNC_QPI_RX_NO_PPT_CREDIT",
      .desc   = "Link 0 snoop stalls due to no PPT entry",
      .modmsk = NHM_UNC_ATTRS,
      .cntmsk = 0x1fe00000,
      .code = 0x43,
      .numasks = LIBPFM_ARRAY_SIZE (nhm_unc_unc_qpi_rx_no_ppt_credit),
      .ngrp = 1,
      .umasks = nhm_unc_unc_qpi_rx_no_ppt_credit,
   },
   {
      .name   = "UNC_QPI_TX_HEADER",
      .desc   = "Cycles link 0 outbound header busy",
      .modmsk = NHM_UNC_ATTRS,
      .cntmsk = 0x1fe00000,
      .code = 0x42,
      .numasks = LIBPFM_ARRAY_SIZE (nhm_unc_unc_qpi_tx_header),
      .ngrp = 1,
      .umasks = nhm_unc_unc_qpi_tx_header,
   },
   {
      .name   = "UNC_QPI_TX_STALLED_MULTI_FLIT",
      .desc   = "Cycles QPI outbound stalls",
      .modmsk = NHM_UNC_ATTRS,
      .cntmsk = 0x1fe00000,
      .code = 0x41,
      .numasks = LIBPFM_ARRAY_SIZE (nhm_unc_unc_qpi_tx_stalled_multi_flit),
      .ngrp = 1,
      .umasks = nhm_unc_unc_qpi_tx_stalled_multi_flit,
   },
   {
      .name   = "UNC_QPI_TX_STALLED_SINGLE_FLIT",
      .desc   = "Cycles QPI outbound link stalls",
      .modmsk = NHM_UNC_ATTRS,
      .cntmsk = 0x1fe00000,
      .code = 0x40,
      .numasks = LIBPFM_ARRAY_SIZE (nhm_unc_unc_qpi_tx_stalled_single_flit),
      .ngrp = 1,
      .umasks = nhm_unc_unc_qpi_tx_stalled_single_flit,
   },
   {
      .name   = "UNC_SNP_RESP_TO_LOCAL_HOME",
      .desc   = "Local home snoop response",
      .modmsk = NHM_UNC_ATTRS,
      .cntmsk = 0x1fe00000,
      .code = 0x6,
      .numasks = LIBPFM_ARRAY_SIZE (nhm_unc_unc_snp_resp_to_local_home),
      .ngrp = 1,
      .umasks = nhm_unc_unc_snp_resp_to_local_home,
   },
   {
      .name   = "UNC_SNP_RESP_TO_REMOTE_HOME",
      .desc   = "Remote home snoop response",
      .modmsk = NHM_UNC_ATTRS,
      .cntmsk = 0x1fe00000,
      .code = 0x7,
      .numasks = LIBPFM_ARRAY_SIZE (nhm_unc_unc_snp_resp_to_remote_home),
      .ngrp = 1,
      .umasks = nhm_unc_unc_snp_resp_to_remote_home,
   },
};
