// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _HARDWARE_STRUCTS_M33_H
#define _HARDWARE_STRUCTS_M33_H

/**
 * \file rp2350/m33.h
 */

#include "hardware/address_mapped.h"
#include "hardware/regs/m33.h"

// Reference to datasheet: https://datasheets.raspberrypi.com/rp2350/rp2350-datasheet.pdf#tab-registerlist_m33
//
// The _REG_ macro is intended to help make the register navigable in your IDE (for example, using the "Go to Definition" feature)
// _REG_(x) will link to the corresponding register in hardware/regs/m33.h.
//
// Bit-field descriptions are of the form:
// BITMASK [BITRANGE] FIELDNAME (RESETVALUE) DESCRIPTION

#if defined(__riscv) && PICO_FORBID_ARM_HEADERS_ON_RISCV
#error "Arm header included in a RISC-V build with PICO_FORBID_ARM_HEADERS_ON_RISCV=1"
#endif

typedef struct {
    // (Description copied from array index 0 register M33_ITM_STIM0 applies similarly to other array indexes)
    _REG_(M33_ITM_STIM0_OFFSET) // M33_ITM_STIM0
    // ITM Stimulus Port Register 0
    // 0xffffffff [31:0]  STIMULUS     (0x00000000) Data to write to the Stimulus Port FIFO, for forwarding...
    io_rw_32 itm_stim[32];
 
    uint32_t _pad0[864];
 
    _REG_(M33_ITM_TER0_OFFSET) // M33_ITM_TER0
    // Provide an individual enable bit for each ITM_STIM register
    // 0xffffffff [31:0]  STIMENA      (0x00000000) For STIMENA[m] in ITM_TER*n, controls whether...
    io_rw_32 itm_ter0;
 
    uint32_t _pad1[15];
 
    _REG_(M33_ITM_TPR_OFFSET) // M33_ITM_TPR
    // Controls which stimulus ports can be accessed by unprivileged code
    // 0x0000000f [3:0]   PRIVMASK     (0x0) Bit mask to enable tracing on ITM stimulus ports
    io_rw_32 itm_tpr;
 
    uint32_t _pad2[15];
 
    _REG_(M33_ITM_TCR_OFFSET) // M33_ITM_TCR
    // Configures and controls transfers through the ITM interface
    // 0x00800000 [23]    BUSY         (0) Indicates whether the ITM is currently processing events
    // 0x007f0000 [22:16] TRACEBUSID   (0x00) Identifier for multi-source trace stream formatting
    // 0x00000c00 [11:10] GTSFREQ      (0x0) Defines how often the ITM generates a global timestamp,...
    // 0x00000300 [9:8]   TSPRESCALE   (0x0) Local timestamp prescaler, used with the trace packet...
    // 0x00000020 [5]     STALLENA     (0) Stall the PE to guarantee delivery of Data Trace packets
    // 0x00000010 [4]     SWOENA       (0) Enables asynchronous clocking of the timestamp counter
    // 0x00000008 [3]     TXENA        (0) Enables forwarding of hardware event packet from the DWT...
    // 0x00000004 [2]     SYNCENA      (0) Enables Synchronization packet transmission for a...
    // 0x00000002 [1]     TSENA        (0) Enables Local timestamp generation
    // 0x00000001 [0]     ITMENA       (0) Enables the ITM
    io_rw_32 itm_tcr;
 
    uint32_t _pad3[27];
 
    _REG_(M33_INT_ATREADY_OFFSET) // M33_INT_ATREADY
    // Integration Mode: Read ATB Ready
    // 0x00000002 [1]     AFVALID      (0) A read of this bit returns the value of AFVALID
    // 0x00000001 [0]     ATREADY      (0) A read of this bit returns the value of ATREADY
    io_ro_32 int_atready;
 
    uint32_t _pad4;
 
    _REG_(M33_INT_ATVALID_OFFSET) // M33_INT_ATVALID
    // Integration Mode: Write ATB Valid
    // 0x00000002 [1]     AFREADY      (0) A write to this bit gives the value of AFREADY
    // 0x00000001 [0]     ATREADY      (0) A write to this bit gives the value of ATVALID
    io_rw_32 int_atvalid;
 
    uint32_t _pad5;
 
    _REG_(M33_ITM_ITCTRL_OFFSET) // M33_ITM_ITCTRL
    // Integration Mode Control Register
    // 0x00000001 [0]     IME          (0) Integration mode enable bit - The possible values are: ...
    io_rw_32 itm_itctrl;
 
    uint32_t _pad6[46];
 
    _REG_(M33_ITM_DEVARCH_OFFSET) // M33_ITM_DEVARCH
    // Provides CoreSight discovery information for the ITM
    // 0xffe00000 [31:21] ARCHITECT    (0x23b) Defines the architect of the component
    // 0x00100000 [20]    PRESENT      (1) Defines that the DEVARCH register is present
    // 0x000f0000 [19:16] REVISION     (0x0) Defines the architecture revision of the component
    // 0x0000f000 [15:12] ARCHVER      (0x1) Defines the architecture version of the component
    // 0x00000fff [11:0]  ARCHPART     (0xa01) Defines the architecture of the component
    io_ro_32 itm_devarch;
 
    uint32_t _pad7[3];
 
    _REG_(M33_ITM_DEVTYPE_OFFSET) // M33_ITM_DEVTYPE
    // Provides CoreSight discovery information for the ITM
    // 0x000000f0 [7:4]   SUB          (0x4) Component sub-type
    // 0x0000000f [3:0]   MAJOR        (0x3) Component major type
    io_ro_32 itm_devtype;
 
    _REG_(M33_ITM_PIDR4_OFFSET) // M33_ITM_PIDR4
    // Provides CoreSight discovery information for the ITM
    // 0x000000f0 [7:4]   SIZE         (0x0) See CoreSight Architecture Specification
    // 0x0000000f [3:0]   DES_2        (0x4) See CoreSight Architecture Specification
    io_ro_32 itm_pidr4;
 
    _REG_(M33_ITM_PIDR5_OFFSET) // M33_ITM_PIDR5
    // Provides CoreSight discovery information for the ITM
    // 0x00000000 [31:0]  ITM_PIDR5    (0x00000000) 
    io_rw_32 itm_pidr5;
 
    _REG_(M33_ITM_PIDR6_OFFSET) // M33_ITM_PIDR6
    // Provides CoreSight discovery information for the ITM
    // 0x00000000 [31:0]  ITM_PIDR6    (0x00000000) 
    io_rw_32 itm_pidr6;
 
    _REG_(M33_ITM_PIDR7_OFFSET) // M33_ITM_PIDR7
    // Provides CoreSight discovery information for the ITM
    // 0x00000000 [31:0]  ITM_PIDR7    (0x00000000) 
    io_rw_32 itm_pidr7;
 
    _REG_(M33_ITM_PIDR0_OFFSET) // M33_ITM_PIDR0
    // Provides CoreSight discovery information for the ITM
    // 0x000000ff [7:0]   PART_0       (0x21) See CoreSight Architecture Specification
    io_ro_32 itm_pidr0;
 
    _REG_(M33_ITM_PIDR1_OFFSET) // M33_ITM_PIDR1
    // Provides CoreSight discovery information for the ITM
    // 0x000000f0 [7:4]   DES_0        (0xb) See CoreSight Architecture Specification
    // 0x0000000f [3:0]   PART_1       (0xd) See CoreSight Architecture Specification
    io_ro_32 itm_pidr1;
 
    _REG_(M33_ITM_PIDR2_OFFSET) // M33_ITM_PIDR2
    // Provides CoreSight discovery information for the ITM
    // 0x000000f0 [7:4]   REVISION     (0x0) See CoreSight Architecture Specification
    // 0x00000008 [3]     JEDEC        (1) See CoreSight Architecture Specification
    // 0x00000007 [2:0]   DES_1        (0x3) See CoreSight Architecture Specification
    io_ro_32 itm_pidr2;
 
    _REG_(M33_ITM_PIDR3_OFFSET) // M33_ITM_PIDR3
    // Provides CoreSight discovery information for the ITM
    // 0x000000f0 [7:4]   REVAND       (0x0) See CoreSight Architecture Specification
    // 0x0000000f [3:0]   CMOD         (0x0) See CoreSight Architecture Specification
    io_ro_32 itm_pidr3;
 
    // (Description copied from array index 0 register M33_ITM_CIDR0 applies similarly to other array indexes)
    _REG_(M33_ITM_CIDR0_OFFSET) // M33_ITM_CIDR0
    // Provides CoreSight discovery information for the ITM
    // 0x000000ff [7:0]   PRMBL_0      (0x0d) See CoreSight Architecture Specification
    io_ro_32 itm_cidr[4];
 
    _REG_(M33_DWT_CTRL_OFFSET) // M33_DWT_CTRL
    // Provides configuration and status information for the DWT unit, and used to control features of the unit
    // 0xf0000000 [31:28] NUMCOMP      (0x7) Number of DWT comparators implemented
    // 0x08000000 [27]    NOTRCPKT     (0) Indicates whether the implementation does not support trace
    // 0x04000000 [26]    NOEXTTRIG    (0) Reserved, RAZ
    // 0x02000000 [25]    NOCYCCNT     (1) Indicates whether the implementation does not include a...
    // 0x01000000 [24]    NOPRFCNT     (1) Indicates whether the implementation does not include...
    // 0x00800000 [23]    CYCDISS      (0) Controls whether the cycle counter is disabled in Secure state
    // 0x00400000 [22]    CYCEVTENA    (1) Enables Event Counter packet generation on POSTCNT underflow
    // 0x00200000 [21]    FOLDEVTENA   (1) Enables DWT_FOLDCNT counter
    // 0x00100000 [20]    LSUEVTENA    (1) Enables DWT_LSUCNT counter
    // 0x00080000 [19]    SLEEPEVTENA  (0) Enable DWT_SLEEPCNT counter
    // 0x00040000 [18]    EXCEVTENA    (1) Enables DWT_EXCCNT counter
    // 0x00020000 [17]    CPIEVTENA    (0) Enables DWT_CPICNT counter
    // 0x00010000 [16]    EXTTRCENA    (0) Enables generation of Exception Trace packets
    // 0x00001000 [12]    PCSAMPLENA   (1) Enables use of POSTCNT counter as a timer for Periodic...
    // 0x00000c00 [11:10] SYNCTAP      (0x2) Selects the position of the synchronization packet...
    // 0x00000200 [9]     CYCTAP       (0) Selects the position of the POSTCNT tap on the CYCCNT counter
    // 0x000001e0 [8:5]   POSTINIT     (0x1) Initial value for the POSTCNT counter
    // 0x0000001e [4:1]   POSTPRESET   (0x2) Reload value for the POSTCNT counter
    // 0x00000001 [0]     CYCCNTENA    (0) Enables CYCCNT
    io_rw_32 dwt_ctrl;
 
    _REG_(M33_DWT_CYCCNT_OFFSET) // M33_DWT_CYCCNT
    // Shows or sets the value of the processor cycle counter, CYCCNT
    // 0xffffffff [31:0]  CYCCNT       (0x00000000) Increments one on each processor clock cycle when DWT_CTRL
    io_rw_32 dwt_cyccnt;
 
    uint32_t _pad8;
 
    _REG_(M33_DWT_EXCCNT_OFFSET) // M33_DWT_EXCCNT
    // Counts the total cycles spent in exception processing
    // 0x000000ff [7:0]   EXCCNT       (0x00) Counts one on each cycle when all of the following are...
    io_rw_32 dwt_exccnt;
 
    uint32_t _pad9;
 
    _REG_(M33_DWT_LSUCNT_OFFSET) // M33_DWT_LSUCNT
    // Increments on the additional cycles required to execute all load or store instructions
    // 0x000000ff [7:0]   LSUCNT       (0x00) Counts one on each cycle when all of the following are...
    io_rw_32 dwt_lsucnt;
 
    _REG_(M33_DWT_FOLDCNT_OFFSET) // M33_DWT_FOLDCNT
    // Increments on the additional cycles required to execute all load or store instructions
    // 0x000000ff [7:0]   FOLDCNT      (0x00) Counts on each cycle when all of the following are true:...
    io_rw_32 dwt_foldcnt;
 
    uint32_t _pad10;
 
    _REG_(M33_DWT_COMP0_OFFSET) // M33_DWT_COMP0
    // Provides a reference value for use by watchpoint comparator 0
    // 0xffffffff [31:0]  DWT_COMP0    (0x00000000) 
    io_rw_32 dwt_comp0;
 
    uint32_t _pad11;
 
    _REG_(M33_DWT_FUNCTION0_OFFSET) // M33_DWT_FUNCTION0
    // Controls the operation of watchpoint comparator 0
    // 0xf8000000 [31:27] ID           (0x0b) Identifies the capabilities for MATCH for comparator *n
    // 0x01000000 [24]    MATCHED      (0) Set to 1 when the comparator matches
    // 0x00000c00 [11:10] DATAVSIZE    (0x0) Defines the size of the object being watched for by Data...
    // 0x00000030 [5:4]   ACTION       (0x0) Defines the action on a match
    // 0x0000000f [3:0]   MATCH        (0x0) Controls the type of match generated by this comparator
    io_rw_32 dwt_function0;
 
    uint32_t _pad12;
 
    _REG_(M33_DWT_COMP1_OFFSET) // M33_DWT_COMP1
    // Provides a reference value for use by watchpoint comparator 1
    // 0xffffffff [31:0]  DWT_COMP1    (0x00000000) 
    io_rw_32 dwt_comp1;
 
    uint32_t _pad13;
 
    _REG_(M33_DWT_FUNCTION1_OFFSET) // M33_DWT_FUNCTION1
    // Controls the operation of watchpoint comparator 1
    // 0xf8000000 [31:27] ID           (0x11) Identifies the capabilities for MATCH for comparator *n
    // 0x01000000 [24]    MATCHED      (1) Set to 1 when the comparator matches
    // 0x00000c00 [11:10] DATAVSIZE    (0x2) Defines the size of the object being watched for by Data...
    // 0x00000030 [5:4]   ACTION       (0x2) Defines the action on a match
    // 0x0000000f [3:0]   MATCH        (0x8) Controls the type of match generated by this comparator
    io_rw_32 dwt_function1;
 
    uint32_t _pad14;
 
    _REG_(M33_DWT_COMP2_OFFSET) // M33_DWT_COMP2
    // Provides a reference value for use by watchpoint comparator 2
    // 0xffffffff [31:0]  DWT_COMP2    (0x00000000) 
    io_rw_32 dwt_comp2;
 
    uint32_t _pad15;
 
    _REG_(M33_DWT_FUNCTION2_OFFSET) // M33_DWT_FUNCTION2
    // Controls the operation of watchpoint comparator 2
    // 0xf8000000 [31:27] ID           (0x0a) Identifies the capabilities for MATCH for comparator *n
    // 0x01000000 [24]    MATCHED      (0) Set to 1 when the comparator matches
    // 0x00000c00 [11:10] DATAVSIZE    (0x0) Defines the size of the object being watched for by Data...
    // 0x00000030 [5:4]   ACTION       (0x0) Defines the action on a match
    // 0x0000000f [3:0]   MATCH        (0x0) Controls the type of match generated by this comparator
    io_rw_32 dwt_function2;
 
    uint32_t _pad16;
 
    _REG_(M33_DWT_COMP3_OFFSET) // M33_DWT_COMP3
    // Provides a reference value for use by watchpoint comparator 3
    // 0xffffffff [31:0]  DWT_COMP3    (0x00000000) 
    io_rw_32 dwt_comp3;
 
    uint32_t _pad17;
 
    _REG_(M33_DWT_FUNCTION3_OFFSET) // M33_DWT_FUNCTION3
    // Controls the operation of watchpoint comparator 3
    // 0xf8000000 [31:27] ID           (0x04) Identifies the capabilities for MATCH for comparator *n
    // 0x01000000 [24]    MATCHED      (0) Set to 1 when the comparator matches
    // 0x00000c00 [11:10] DATAVSIZE    (0x2) Defines the size of the object being watched for by Data...
    // 0x00000030 [5:4]   ACTION       (0x0) Defines the action on a match
    // 0x0000000f [3:0]   MATCH        (0x0) Controls the type of match generated by this comparator
    io_rw_32 dwt_function3;
 
    uint32_t _pad18[984];
 
    _REG_(M33_DWT_DEVARCH_OFFSET) // M33_DWT_DEVARCH
    // Provides CoreSight discovery information for the DWT
    // 0xffe00000 [31:21] ARCHITECT    (0x23b) Defines the architect of the component
    // 0x00100000 [20]    PRESENT      (1) Defines that the DEVARCH register is present
    // 0x000f0000 [19:16] REVISION     (0x0) Defines the architecture revision of the component
    // 0x0000f000 [15:12] ARCHVER      (0x1) Defines the architecture version of the component
    // 0x00000fff [11:0]  ARCHPART     (0xa02) Defines the architecture of the component
    io_ro_32 dwt_devarch;
 
    uint32_t _pad19[3];
 
    _REG_(M33_DWT_DEVTYPE_OFFSET) // M33_DWT_DEVTYPE
    // Provides CoreSight discovery information for the DWT
    // 0x000000f0 [7:4]   SUB          (0x0) Component sub-type
    // 0x0000000f [3:0]   MAJOR        (0x0) Component major type
    io_ro_32 dwt_devtype;
 
    _REG_(M33_DWT_PIDR4_OFFSET) // M33_DWT_PIDR4
    // Provides CoreSight discovery information for the DWT
    // 0x000000f0 [7:4]   SIZE         (0x0) See CoreSight Architecture Specification
    // 0x0000000f [3:0]   DES_2        (0x4) See CoreSight Architecture Specification
    io_ro_32 dwt_pidr4;
 
    _REG_(M33_DWT_PIDR5_OFFSET) // M33_DWT_PIDR5
    // Provides CoreSight discovery information for the DWT
    // 0x00000000 [31:0]  DWT_PIDR5    (0x00000000) 
    io_rw_32 dwt_pidr5;
 
    _REG_(M33_DWT_PIDR6_OFFSET) // M33_DWT_PIDR6
    // Provides CoreSight discovery information for the DWT
    // 0x00000000 [31:0]  DWT_PIDR6    (0x00000000) 
    io_rw_32 dwt_pidr6;
 
    _REG_(M33_DWT_PIDR7_OFFSET) // M33_DWT_PIDR7
    // Provides CoreSight discovery information for the DWT
    // 0x00000000 [31:0]  DWT_PIDR7    (0x00000000) 
    io_rw_32 dwt_pidr7;
 
    _REG_(M33_DWT_PIDR0_OFFSET) // M33_DWT_PIDR0
    // Provides CoreSight discovery information for the DWT
    // 0x000000ff [7:0]   PART_0       (0x21) See CoreSight Architecture Specification
    io_ro_32 dwt_pidr0;
 
    _REG_(M33_DWT_PIDR1_OFFSET) // M33_DWT_PIDR1
    // Provides CoreSight discovery information for the DWT
    // 0x000000f0 [7:4]   DES_0        (0xb) See CoreSight Architecture Specification
    // 0x0000000f [3:0]   PART_1       (0xd) See CoreSight Architecture Specification
    io_ro_32 dwt_pidr1;
 
    _REG_(M33_DWT_PIDR2_OFFSET) // M33_DWT_PIDR2
    // Provides CoreSight discovery information for the DWT
    // 0x000000f0 [7:4]   REVISION     (0x0) See CoreSight Architecture Specification
    // 0x00000008 [3]     JEDEC        (1) See CoreSight Architecture Specification
    // 0x00000007 [2:0]   DES_1        (0x3) See CoreSight Architecture Specification
    io_ro_32 dwt_pidr2;
 
    _REG_(M33_DWT_PIDR3_OFFSET) // M33_DWT_PIDR3
    // Provides CoreSight discovery information for the DWT
    // 0x000000f0 [7:4]   REVAND       (0x0) See CoreSight Architecture Specification
    // 0x0000000f [3:0]   CMOD         (0x0) See CoreSight Architecture Specification
    io_ro_32 dwt_pidr3;
 
    // (Description copied from array index 0 register M33_DWT_CIDR0 applies similarly to other array indexes)
    _REG_(M33_DWT_CIDR0_OFFSET) // M33_DWT_CIDR0
    // Provides CoreSight discovery information for the DWT
    // 0x000000ff [7:0]   PRMBL_0      (0x0d) See CoreSight Architecture Specification
    io_ro_32 dwt_cidr[4];
 
    _REG_(M33_FP_CTRL_OFFSET) // M33_FP_CTRL
    // Provides FPB implementation information, and the global enable for the FPB unit
    // 0xf0000000 [31:28] REV          (0x6) Flash Patch and Breakpoint Unit architecture revision
    // 0x00007000 [14:12] NUM_CODE_14_12_ (0x5) Indicates the number of implemented instruction address...
    // 0x00000f00 [11:8]  NUM_LIT      (0x5) Indicates the number of implemented literal address comparators
    // 0x000000f0 [7:4]   NUM_CODE_7_4_ (0x8) Indicates the number of implemented instruction address...
    // 0x00000002 [1]     KEY          (0) Writes to the FP_CTRL are ignored unless KEY is...
    // 0x00000001 [0]     ENABLE       (0) Enables the FPB
    io_rw_32 fp_ctrl;
 
    _REG_(M33_FP_REMAP_OFFSET) // M33_FP_REMAP
    // Indicates whether the implementation supports Flash Patch remap and, if it does, holds the...
    // 0x20000000 [29]    RMPSPT       (0) Indicates whether the FPB unit supports the Flash Patch...
    // 0x1fffffe0 [28:5]  REMAP        (0x000000) Holds the bits[28:5] of the Flash Patch remap address
    io_ro_32 fp_remap;
 
    // (Description copied from array index 0 register M33_FP_COMP0 applies similarly to other array indexes)
    _REG_(M33_FP_COMP0_OFFSET) // M33_FP_COMP0
    // Holds an address for comparison
    // 0x00000001 [0]     BE           (0) Selects between flashpatch and breakpoint functionality
    io_rw_32 fp_comp[8];
 
    uint32_t _pad20[997];
 
    _REG_(M33_FP_DEVARCH_OFFSET) // M33_FP_DEVARCH
    // Provides CoreSight discovery information for the FPB
    // 0xffe00000 [31:21] ARCHITECT    (0x23b) Defines the architect of the component
    // 0x00100000 [20]    PRESENT      (1) Defines that the DEVARCH register is present
    // 0x000f0000 [19:16] REVISION     (0x0) Defines the architecture revision of the component
    // 0x0000f000 [15:12] ARCHVER      (0x1) Defines the architecture version of the component
    // 0x00000fff [11:0]  ARCHPART     (0xa03) Defines the architecture of the component
    io_ro_32 fp_devarch;
 
    uint32_t _pad21[3];
 
    _REG_(M33_FP_DEVTYPE_OFFSET) // M33_FP_DEVTYPE
    // Provides CoreSight discovery information for the FPB
    // 0x000000f0 [7:4]   SUB          (0x0) Component sub-type
    // 0x0000000f [3:0]   MAJOR        (0x0) Component major type
    io_ro_32 fp_devtype;
 
    _REG_(M33_FP_PIDR4_OFFSET) // M33_FP_PIDR4
    // Provides CoreSight discovery information for the FP
    // 0x000000f0 [7:4]   SIZE         (0x0) See CoreSight Architecture Specification
    // 0x0000000f [3:0]   DES_2        (0x4) See CoreSight Architecture Specification
    io_ro_32 fp_pidr4;
 
    _REG_(M33_FP_PIDR5_OFFSET) // M33_FP_PIDR5
    // Provides CoreSight discovery information for the FP
    // 0x00000000 [31:0]  FP_PIDR5     (0x00000000) 
    io_rw_32 fp_pidr5;
 
    _REG_(M33_FP_PIDR6_OFFSET) // M33_FP_PIDR6
    // Provides CoreSight discovery information for the FP
    // 0x00000000 [31:0]  FP_PIDR6     (0x00000000) 
    io_rw_32 fp_pidr6;
 
    _REG_(M33_FP_PIDR7_OFFSET) // M33_FP_PIDR7
    // Provides CoreSight discovery information for the FP
    // 0x00000000 [31:0]  FP_PIDR7     (0x00000000) 
    io_rw_32 fp_pidr7;
 
    _REG_(M33_FP_PIDR0_OFFSET) // M33_FP_PIDR0
    // Provides CoreSight discovery information for the FP
    // 0x000000ff [7:0]   PART_0       (0x21) See CoreSight Architecture Specification
    io_ro_32 fp_pidr0;
 
    _REG_(M33_FP_PIDR1_OFFSET) // M33_FP_PIDR1
    // Provides CoreSight discovery information for the FP
    // 0x000000f0 [7:4]   DES_0        (0xb) See CoreSight Architecture Specification
    // 0x0000000f [3:0]   PART_1       (0xd) See CoreSight Architecture Specification
    io_ro_32 fp_pidr1;
 
    _REG_(M33_FP_PIDR2_OFFSET) // M33_FP_PIDR2
    // Provides CoreSight discovery information for the FP
    // 0x000000f0 [7:4]   REVISION     (0x0) See CoreSight Architecture Specification
    // 0x00000008 [3]     JEDEC        (1) See CoreSight Architecture Specification
    // 0x00000007 [2:0]   DES_1        (0x3) See CoreSight Architecture Specification
    io_ro_32 fp_pidr2;
 
    _REG_(M33_FP_PIDR3_OFFSET) // M33_FP_PIDR3
    // Provides CoreSight discovery information for the FP
    // 0x000000f0 [7:4]   REVAND       (0x0) See CoreSight Architecture Specification
    // 0x0000000f [3:0]   CMOD         (0x0) See CoreSight Architecture Specification
    io_ro_32 fp_pidr3;
 
    // (Description copied from array index 0 register M33_FP_CIDR0 applies similarly to other array indexes)
    _REG_(M33_FP_CIDR0_OFFSET) // M33_FP_CIDR0
    // Provides CoreSight discovery information for the FP
    // 0x000000ff [7:0]   PRMBL_0      (0x0d) See CoreSight Architecture Specification
    io_ro_32 fp_cidr[4];
 
    uint32_t _pad22[11265];
 
    _REG_(M33_ICTR_OFFSET) // M33_ICTR
    // Provides information about the interrupt controller
    // 0x0000000f [3:0]   INTLINESNUM  (0x1) Indicates the number of the highest implemented register...
    io_ro_32 ictr;
 
    _REG_(M33_ACTLR_OFFSET) // M33_ACTLR
    // Provides IMPLEMENTATION DEFINED configuration and control options
    // 0x20000000 [29]    EXTEXCLALL   (0) External Exclusives Allowed with no MPU
    // 0x00001000 [12]    DISITMATBFLUSH (0) Disable ATB Flush
    // 0x00000400 [10]    FPEXCODIS    (0) Disable FPU exception outputs
    // 0x00000200 [9]     DISOOFP      (0) Disable out-of-order FP instruction completion
    // 0x00000004 [2]     DISFOLD      (0) Disable dual-issue
    // 0x00000001 [0]     DISMCYCINT   (0) Disable dual-issue
    io_rw_32 actlr;
 
    uint32_t _pad23;
 
    _REG_(M33_SYST_CSR_OFFSET) // M33_SYST_CSR
    // SysTick Control and Status Register
    // 0x00010000 [16]    COUNTFLAG    (0) Returns 1 if timer counted to 0 since last time this was read
    // 0x00000004 [2]     CLKSOURCE    (0) SysTick clock source
    // 0x00000002 [1]     TICKINT      (0) Enables SysTick exception request: +
    // 0x00000001 [0]     ENABLE       (0) Enable SysTick counter: +
    io_rw_32 syst_csr;
 
    _REG_(M33_SYST_RVR_OFFSET) // M33_SYST_RVR
    // SysTick Reload Value Register
    // 0x00ffffff [23:0]  RELOAD       (0x000000) Value to load into the SysTick Current Value Register...
    io_rw_32 syst_rvr;
 
    _REG_(M33_SYST_CVR_OFFSET) // M33_SYST_CVR
    // SysTick Current Value Register
    // 0x00ffffff [23:0]  CURRENT      (0x000000) Reads return the current value of the SysTick counter
    io_rw_32 syst_cvr;
 
    _REG_(M33_SYST_CALIB_OFFSET) // M33_SYST_CALIB
    // SysTick Calibration Value Register
    // 0x80000000 [31]    NOREF        (0) If reads as 1, the Reference clock is not provided - the...
    // 0x40000000 [30]    SKEW         (0) If reads as 1, the calibration value for 10ms is inexact...
    // 0x00ffffff [23:0]  TENMS        (0x000000) An optional Reload value to be used for 10ms (100Hz)...
    io_ro_32 syst_calib;
 
    uint32_t _pad24[56];
 
    // (Description copied from array index 0 register M33_NVIC_ISER0 applies similarly to other array indexes)
    _REG_(M33_NVIC_ISER0_OFFSET) // M33_NVIC_ISER0
    // Enables or reads the enabled state of each group of 32 interrupts
    // 0xffffffff [31:0]  SETENA       (0x00000000) For SETENA[m] in NVIC_ISER*n, indicates whether...
    io_rw_32 nvic_iser[2];
 
    uint32_t _pad25[30];
 
    // (Description copied from array index 0 register M33_NVIC_ICER0 applies similarly to other array indexes)
    _REG_(M33_NVIC_ICER0_OFFSET) // M33_NVIC_ICER0
    // Clears or reads the enabled state of each group of 32 interrupts
    // 0xffffffff [31:0]  CLRENA       (0x00000000) For CLRENA[m] in NVIC_ICER*n, indicates whether...
    io_rw_32 nvic_icer[2];
 
    uint32_t _pad26[30];
 
    // (Description copied from array index 0 register M33_NVIC_ISPR0 applies similarly to other array indexes)
    _REG_(M33_NVIC_ISPR0_OFFSET) // M33_NVIC_ISPR0
    // Enables or reads the pending state of each group of 32 interrupts
    // 0xffffffff [31:0]  SETPEND      (0x00000000) For SETPEND[m] in NVIC_ISPR*n, indicates whether...
    io_rw_32 nvic_ispr[2];
 
    uint32_t _pad27[30];
 
    // (Description copied from array index 0 register M33_NVIC_ICPR0 applies similarly to other array indexes)
    _REG_(M33_NVIC_ICPR0_OFFSET) // M33_NVIC_ICPR0
    // Clears or reads the pending state of each group of 32 interrupts
    // 0xffffffff [31:0]  CLRPEND      (0x00000000) For CLRPEND[m] in NVIC_ICPR*n, indicates whether...
    io_rw_32 nvic_icpr[2];
 
    uint32_t _pad28[30];
 
    // (Description copied from array index 0 register M33_NVIC_IABR0 applies similarly to other array indexes)
    _REG_(M33_NVIC_IABR0_OFFSET) // M33_NVIC_IABR0
    // For each group of 32 interrupts, shows the active state of each interrupt
    // 0xffffffff [31:0]  ACTIVE       (0x00000000) For ACTIVE[m] in NVIC_IABR*n, indicates the active state...
    io_rw_32 nvic_iabr[2];
 
    uint32_t _pad29[30];
 
    // (Description copied from array index 0 register M33_NVIC_ITNS0 applies similarly to other array indexes)
    _REG_(M33_NVIC_ITNS0_OFFSET) // M33_NVIC_ITNS0
    // For each group of 32 interrupts, determines whether each interrupt targets Non-secure or Secure state
    // 0xffffffff [31:0]  ITNS         (0x00000000) For ITNS[m] in NVIC_ITNS*n, `IAAMO the target Security...
    io_rw_32 nvic_itns[2];
 
    uint32_t _pad30[30];
 
    // (Description copied from array index 0 register M33_NVIC_IPR0 applies similarly to other array indexes)
    _REG_(M33_NVIC_IPR0_OFFSET) // M33_NVIC_IPR0
    // Sets or reads interrupt priorities
    // 0xf0000000 [31:28] PRI_N3       (0x0) For register NVIC_IPRn, the priority of interrupt number...
    // 0x00f00000 [23:20] PRI_N2       (0x0) For register NVIC_IPRn, the priority of interrupt number...
    // 0x0000f000 [15:12] PRI_N1       (0x0) For register NVIC_IPRn, the priority of interrupt number...
    // 0x000000f0 [7:4]   PRI_N0       (0x0) For register NVIC_IPRn, the priority of interrupt number...
    io_rw_32 nvic_ipr[16];
 
    uint32_t _pad31[560];
 
    _REG_(M33_CPUID_OFFSET) // M33_CPUID
    // Provides identification information for the PE, including an implementer code for the device and...
    // 0xff000000 [31:24] IMPLEMENTER  (0x41) This field must hold an implementer code that has been...
    // 0x00f00000 [23:20] VARIANT      (0x1) IMPLEMENTATION DEFINED variant number
    // 0x000f0000 [19:16] ARCHITECTURE (0xf) Defines the Architecture implemented by the PE
    // 0x0000fff0 [15:4]  PARTNO       (0xd21) IMPLEMENTATION DEFINED primary part number for the device
    // 0x0000000f [3:0]   REVISION     (0x0) IMPLEMENTATION DEFINED revision number for the device
    io_ro_32 cpuid;
 
    _REG_(M33_ICSR_OFFSET) // M33_ICSR
    // Controls and provides status information for NMI, PendSV, SysTick and interrupts
    // 0x80000000 [31]    PENDNMISET   (0) Indicates whether the NMI exception is pending
    // 0x40000000 [30]    PENDNMICLR   (0) Allows the NMI exception pend state to be cleared
    // 0x10000000 [28]    PENDSVSET    (0) Indicates whether the PendSV `FTSSS exception is pending
    // 0x08000000 [27]    PENDSVCLR    (0) Allows the PendSV exception pend state to be cleared `FTSSS
    // 0x04000000 [26]    PENDSTSET    (0) Indicates whether the SysTick `FTSSS exception is pending
    // 0x02000000 [25]    PENDSTCLR    (0) Allows the SysTick exception pend state to be cleared `FTSSS
    // 0x01000000 [24]    STTNS        (0) Controls whether in a single SysTick implementation, the...
    // 0x00800000 [23]    ISRPREEMPT   (0) Indicates whether a pending exception will be serviced...
    // 0x00400000 [22]    ISRPENDING   (0) Indicates whether an external interrupt, generated by...
    // 0x001ff000 [20:12] VECTPENDING  (0x000) The exception number of the highest priority pending and...
    // 0x00000800 [11]    RETTOBASE    (0) In Handler mode, indicates whether there is more than...
    // 0x000001ff [8:0]   VECTACTIVE   (0x000) The exception number of the current executing exception
    io_rw_32 icsr;
 
    _REG_(M33_VTOR_OFFSET) // M33_VTOR
    // Vector Table Offset Register
    // 0xffffff80 [31:7]  TBLOFF       (0x0000000) Vector table base offset field
    io_rw_32 vtor;
 
    _REG_(M33_AIRCR_OFFSET) // M33_AIRCR
    // Application Interrupt and Reset Control Register
    // 0xffff0000 [31:16] VECTKEY      (0x0000) Register key: +
    // 0x00008000 [15]    ENDIANESS    (0) Data endianness implemented: +
    // 0x00004000 [14]    PRIS         (0) Prioritize Secure exceptions
    // 0x00002000 [13]    BFHFNMINS    (0) BusFault, HardFault, and NMI Non-secure enable
    // 0x00000700 [10:8]  PRIGROUP     (0x0) Interrupt priority grouping field
    // 0x00000008 [3]     SYSRESETREQS (0) System reset request, Secure state only
    // 0x00000004 [2]     SYSRESETREQ  (0) Writing 1 to this bit causes the SYSRESETREQ signal to...
    // 0x00000002 [1]     VECTCLRACTIVE (0) Clears all active state information for fixed and...
    io_rw_32 aircr;
 
    _REG_(M33_SCR_OFFSET) // M33_SCR
    // System Control Register
    // 0x00000010 [4]     SEVONPEND    (0) Send Event on Pending bit: +
    // 0x00000008 [3]     SLEEPDEEPS   (0) 0 SLEEPDEEP is available to both security states +
    // 0x00000004 [2]     SLEEPDEEP    (0) Controls whether the processor uses sleep or deep sleep...
    // 0x00000002 [1]     SLEEPONEXIT  (0) Indicates sleep-on-exit when returning from Handler mode...
    io_rw_32 scr;
 
    _REG_(M33_CCR_OFFSET) // M33_CCR
    // Sets or returns configuration and control data
    // 0x00040000 [18]    BP           (0) Enables program flow prediction `FTSSS
    // 0x00020000 [17]    IC           (0) This is a global enable bit for instruction caches in...
    // 0x00010000 [16]    DC           (0) Enables data caching of all data accesses to Normal memory `FTSSS
    // 0x00000400 [10]    STKOFHFNMIGN (0) Controls the effect of a stack limit violation while...
    // 0x00000200 [9]     RES1         (1) Reserved, RES1
    // 0x00000100 [8]     BFHFNMIGN    (0) Determines the effect of precise BusFaults on handlers...
    // 0x00000010 [4]     DIV_0_TRP    (0) Controls the generation of a DIVBYZERO UsageFault when...
    // 0x00000008 [3]     UNALIGN_TRP  (0) Controls the trapping of unaligned word or halfword accesses
    // 0x00000002 [1]     USERSETMPEND (0) Determines whether unprivileged accesses are permitted...
    // 0x00000001 [0]     RES1_1       (1) Reserved, RES1
    io_rw_32 ccr;
 
    // (Description copied from array index 0 register M33_SHPR1 applies similarly to other array indexes)
    _REG_(M33_SHPR1_OFFSET) // M33_SHPR1
    // Sets or returns priority for system handlers 4 - 7
    // 0xe0000000 [31:29] PRI_7_3      (0x0) Priority of system handler 7, SecureFault
    // 0x00e00000 [23:21] PRI_6_3      (0x0) Priority of system handler 6, SecureFault
    // 0x0000e000 [15:13] PRI_5_3      (0x0) Priority of system handler 5, SecureFault
    // 0x000000e0 [7:5]   PRI_4_3      (0x0) Priority of system handler 4, SecureFault
    io_rw_32 shpr[3];
 
    _REG_(M33_SHCSR_OFFSET) // M33_SHCSR
    // Provides access to the active and pending status of system exceptions
    // 0x00200000 [21]    HARDFAULTPENDED (0) `IAAMO the pending state of the HardFault exception `CTTSSS
    // 0x00100000 [20]    SECUREFAULTPENDED (0) `IAAMO the pending state of the SecureFault exception
    // 0x00080000 [19]    SECUREFAULTENA (0) `DW the SecureFault exception is enabled
    // 0x00040000 [18]    USGFAULTENA  (0) `DW the UsageFault exception is enabled `FTSSS
    // 0x00020000 [17]    BUSFAULTENA  (0) `DW the BusFault exception is enabled
    // 0x00010000 [16]    MEMFAULTENA  (0) `DW the MemManage exception is enabled `FTSSS
    // 0x00008000 [15]    SVCALLPENDED (0) `IAAMO the pending state of the SVCall exception `FTSSS
    // 0x00004000 [14]    BUSFAULTPENDED (0) `IAAMO the pending state of the BusFault exception
    // 0x00002000 [13]    MEMFAULTPENDED (0) `IAAMO the pending state of the MemManage exception `FTSSS
    // 0x00001000 [12]    USGFAULTPENDED (0) The UsageFault exception is banked between Security...
    // 0x00000800 [11]    SYSTICKACT   (0) `IAAMO the active state of the SysTick exception `FTSSS
    // 0x00000400 [10]    PENDSVACT    (0) `IAAMO the active state of the PendSV exception `FTSSS
    // 0x00000100 [8]     MONITORACT   (0) `IAAMO the active state of the DebugMonitor exception
    // 0x00000080 [7]     SVCALLACT    (0) `IAAMO the active state of the SVCall exception `FTSSS
    // 0x00000020 [5]     NMIACT       (0) `IAAMO the active state of the NMI exception
    // 0x00000010 [4]     SECUREFAULTACT (0) `IAAMO the active state of the SecureFault exception
    // 0x00000008 [3]     USGFAULTACT  (0) `IAAMO the active state of the UsageFault exception `FTSSS
    // 0x00000004 [2]     HARDFAULTACT (0) Indicates and allows limited modification of the active...
    // 0x00000002 [1]     BUSFAULTACT  (0) `IAAMO the active state of the BusFault exception
    // 0x00000001 [0]     MEMFAULTACT  (0) `IAAMO the active state of the MemManage exception `FTSSS
    io_rw_32 shcsr;
 
    _REG_(M33_CFSR_OFFSET) // M33_CFSR
    // Contains the three Configurable Fault Status Registers
    // 0x02000000 [25]    UFSR_DIVBYZERO (0) Sticky flag indicating whether an integer division by...
    // 0x01000000 [24]    UFSR_UNALIGNED (0) Sticky flag indicating whether an unaligned access error...
    // 0x00100000 [20]    UFSR_STKOF   (0) Sticky flag indicating whether a stack overflow error...
    // 0x00080000 [19]    UFSR_NOCP    (0) Sticky flag indicating whether a coprocessor disabled or...
    // 0x00040000 [18]    UFSR_INVPC   (0) Sticky flag indicating whether an integrity check error...
    // 0x00020000 [17]    UFSR_INVSTATE (0) Sticky flag indicating whether an EPSR
    // 0x00010000 [16]    UFSR_UNDEFINSTR (0) Sticky flag indicating whether an undefined instruction...
    // 0x00008000 [15]    BFSR_BFARVALID (0) Indicates validity of the contents of the BFAR register
    // 0x00002000 [13]    BFSR_LSPERR  (0) Records whether a BusFault occurred during FP lazy state...
    // 0x00001000 [12]    BFSR_STKERR  (0) Records whether a derived BusFault occurred during...
    // 0x00000800 [11]    BFSR_UNSTKERR (0) Records whether a derived BusFault occurred during...
    // 0x00000400 [10]    BFSR_IMPRECISERR (0) Records whether an imprecise data access error has occurred
    // 0x00000200 [9]     BFSR_PRECISERR (0) Records whether a precise data access error has occurred
    // 0x00000100 [8]     BFSR_IBUSERR (0) Records whether a BusFault on an instruction prefetch...
    // 0x000000ff [7:0]   MMFSR        (0x00) Provides information on MemManage exceptions
    io_rw_32 cfsr;
 
    _REG_(M33_HFSR_OFFSET) // M33_HFSR
    // Shows the cause of any HardFaults
    // 0x80000000 [31]    DEBUGEVT     (0) Indicates when a Debug event has occurred
    // 0x40000000 [30]    FORCED       (0) Indicates that a fault with configurable priority has...
    // 0x00000002 [1]     VECTTBL      (0) Indicates when a fault has occurred because of a vector...
    io_rw_32 hfsr;
 
    _REG_(M33_DFSR_OFFSET) // M33_DFSR
    // Shows which debug event occurred
    // 0x00000010 [4]     EXTERNAL     (0) Sticky flag indicating whether an External debug request...
    // 0x00000008 [3]     VCATCH       (0) Sticky flag indicating whether a Vector catch debug...
    // 0x00000004 [2]     DWTTRAP      (0) Sticky flag indicating whether a Watchpoint debug event...
    // 0x00000002 [1]     BKPT         (0) Sticky flag indicating whether a Breakpoint debug event...
    // 0x00000001 [0]     HALTED       (0) Sticky flag indicating that a Halt request debug event...
    io_rw_32 dfsr;
 
    _REG_(M33_MMFAR_OFFSET) // M33_MMFAR
    // Shows the address of the memory location that caused an MPU fault
    // 0xffffffff [31:0]  ADDRESS      (0x00000000) This register is updated with the address of a location...
    io_rw_32 mmfar;
 
    _REG_(M33_BFAR_OFFSET) // M33_BFAR
    // Shows the address associated with a precise data access BusFault
    // 0xffffffff [31:0]  ADDRESS      (0x00000000) This register is updated with the address of a location...
    io_rw_32 bfar;
 
    uint32_t _pad32;
 
    // (Description copied from array index 0 register M33_ID_PFR0 applies similarly to other array indexes)
    _REG_(M33_ID_PFR0_OFFSET) // M33_ID_PFR0
    // Gives top-level information about the instruction set supported by the PE
    // 0x000000f0 [7:4]   STATE1       (0x3) T32 instruction set support
    // 0x0000000f [3:0]   STATE0       (0x0) A32 instruction set support
    io_ro_32 id_pfr[2];
 
    _REG_(M33_ID_DFR0_OFFSET) // M33_ID_DFR0
    // Provides top level information about the debug system
    // 0x00f00000 [23:20] MPROFDBG     (0x2) Indicates the supported M-profile debug architecture
    io_ro_32 id_dfr0;
 
    _REG_(M33_ID_AFR0_OFFSET) // M33_ID_AFR0
    // Provides information about the IMPLEMENTATION DEFINED features of the PE
    // 0x0000f000 [15:12] IMPDEF3      (0x0) IMPLEMENTATION DEFINED meaning
    // 0x00000f00 [11:8]  IMPDEF2      (0x0) IMPLEMENTATION DEFINED meaning
    // 0x000000f0 [7:4]   IMPDEF1      (0x0) IMPLEMENTATION DEFINED meaning
    // 0x0000000f [3:0]   IMPDEF0      (0x0) IMPLEMENTATION DEFINED meaning
    io_ro_32 id_afr0;
 
    // (Description copied from array index 0 register M33_ID_MMFR0 applies similarly to other array indexes)
    _REG_(M33_ID_MMFR0_OFFSET) // M33_ID_MMFR0
    // Provides information about the implemented memory model and memory management support
    // 0x00f00000 [23:20] AUXREG       (0x1) Indicates support for Auxiliary Control Registers
    // 0x000f0000 [19:16] TCM          (0x0) Indicates support for tightly coupled memories (TCMs)
    // 0x0000f000 [15:12] SHARELVL     (0x1) Indicates the number of shareability levels implemented
    // 0x00000f00 [11:8]  OUTERSHR     (0xf) Indicates the outermost shareability domain implemented
    // 0x000000f0 [7:4]   PMSA         (0x4) Indicates support for the protected memory system...
    io_ro_32 id_mmfr[4];
 
    // (Description copied from array index 0 register M33_ID_ISAR0 applies similarly to other array indexes)
    _REG_(M33_ID_ISAR0_OFFSET) // M33_ID_ISAR0
    // Provides information about the instruction set implemented by the PE
    // 0x0f000000 [27:24] DIVIDE       (0x8) Indicates the supported Divide instructions
    // 0x00f00000 [23:20] DEBUG        (0x0) Indicates the implemented Debug instructions
    // 0x000f0000 [19:16] COPROC       (0x9) Indicates the supported Coprocessor instructions
    // 0x0000f000 [15:12] CMPBRANCH    (0x2) Indicates the supported combined Compare and Branch instructions
    // 0x00000f00 [11:8]  BITFIELD     (0x3) Indicates the supported bit field instructions
    // 0x000000f0 [7:4]   BITCOUNT     (0x0) Indicates the supported bit count instructions
    io_ro_32 id_isar[6];
 
    uint32_t _pad33;
 
    _REG_(M33_CTR_OFFSET) // M33_CTR
    // Provides information about the architecture of the caches
    // 0x80000000 [31]    RES1         (1) Reserved, RES1
    // 0x0f000000 [27:24] CWG          (0x0) Log2 of the number of words of the maximum size of...
    // 0x00f00000 [23:20] ERG          (0x0) Log2 of the number of words of the maximum size of the...
    // 0x000f0000 [19:16] DMINLINE     (0x0) Log2 of the number of words in the smallest cache line...
    // 0x0000c000 [15:14] RES1_1       (0x3) Reserved, RES1
    // 0x0000000f [3:0]   IMINLINE     (0x0) Log2 of the number of words in the smallest cache line...
    io_ro_32 ctr;
 
    uint32_t _pad34[2];
 
    _REG_(M33_CPACR_OFFSET) // M33_CPACR
    // Specifies the access privileges for coprocessors and the FP Extension
    // 0x00c00000 [23:22] CP11         (0x0) The value in this field is ignored
    // 0x00300000 [21:20] CP10         (0x0) Defines the access rights for the floating-point functionality
    // 0x0000c000 [15:14] CP7          (0x0) Controls access privileges for coprocessor 7
    // 0x00003000 [13:12] CP6          (0x0) Controls access privileges for coprocessor 6
    // 0x00000c00 [11:10] CP5          (0x0) Controls access privileges for coprocessor 5
    // 0x00000300 [9:8]   CP4          (0x0) Controls access privileges for coprocessor 4
    // 0x000000c0 [7:6]   CP3          (0x0) Controls access privileges for coprocessor 3
    // 0x00000030 [5:4]   CP2          (0x0) Controls access privileges for coprocessor 2
    // 0x0000000c [3:2]   CP1          (0x0) Controls access privileges for coprocessor 1
    // 0x00000003 [1:0]   CP0          (0x0) Controls access privileges for coprocessor 0
    io_rw_32 cpacr;
 
    _REG_(M33_NSACR_OFFSET) // M33_NSACR
    // Defines the Non-secure access permissions for both the FP Extension and coprocessors CP0 to CP7
    // 0x00000800 [11]    CP11         (0) Enables Non-secure access to the Floating-point Extension
    // 0x00000400 [10]    CP10         (0) Enables Non-secure access to the Floating-point Extension
    // 0x00000080 [7]     CP7          (0) Enables Non-secure access to coprocessor CP7
    // 0x00000040 [6]     CP6          (0) Enables Non-secure access to coprocessor CP6
    // 0x00000020 [5]     CP5          (0) Enables Non-secure access to coprocessor CP5
    // 0x00000010 [4]     CP4          (0) Enables Non-secure access to coprocessor CP4
    // 0x00000008 [3]     CP3          (0) Enables Non-secure access to coprocessor CP3
    // 0x00000004 [2]     CP2          (0) Enables Non-secure access to coprocessor CP2
    // 0x00000002 [1]     CP1          (0) Enables Non-secure access to coprocessor CP1
    // 0x00000001 [0]     CP0          (0) Enables Non-secure access to coprocessor CP0
    io_rw_32 nsacr;
 
    _REG_(M33_MPU_TYPE_OFFSET) // M33_MPU_TYPE
    // The MPU Type Register indicates how many regions the MPU `FTSSS supports
    // 0x0000ff00 [15:8]  DREGION      (0x08) Number of regions supported by the MPU
    // 0x00000001 [0]     SEPARATE     (0) Indicates support for separate instructions and data...
    io_ro_32 mpu_type;
 
    _REG_(M33_MPU_CTRL_OFFSET) // M33_MPU_CTRL
    // Enables the MPU and, when the MPU is enabled, controls whether the default memory map is enabled...
    // 0x00000004 [2]     PRIVDEFENA   (0) Controls whether the default memory map is enabled for...
    // 0x00000002 [1]     HFNMIENA     (0) Controls whether handlers executing with priority less...
    // 0x00000001 [0]     ENABLE       (0) Enables the MPU
    io_rw_32 mpu_ctrl;
 
    _REG_(M33_MPU_RNR_OFFSET) // M33_MPU_RNR
    // Selects the region currently accessed by MPU_RBAR and MPU_RLAR
    // 0x00000007 [2:0]   REGION       (0x0) Indicates the memory region accessed by MPU_RBAR and MPU_RLAR
    io_rw_32 mpu_rnr;
 
    _REG_(M33_MPU_RBAR_OFFSET) // M33_MPU_RBAR
    // Provides indirect read and write access to the base address of the currently selected MPU region `FTSSS
    // 0xffffffe0 [31:5]  BASE         (0x0000000) Contains bits [31:5] of the lower inclusive limit of the...
    // 0x00000018 [4:3]   SH           (0x0) Defines the Shareability domain of this region for Normal memory
    // 0x00000006 [2:1]   AP           (0x0) Defines the access permissions for this region
    // 0x00000001 [0]     XN           (0) Defines whether code can be executed from this region
    io_rw_32 mpu_rbar;
 
    _REG_(M33_MPU_RLAR_OFFSET) // M33_MPU_RLAR
    // Provides indirect read and write access to the limit address of the currently selected MPU region `FTSSS
    // 0xffffffe0 [31:5]  LIMIT        (0x0000000) Contains bits [31:5] of the upper inclusive limit of the...
    // 0x0000000e [3:1]   ATTRINDX     (0x0) Associates a set of attributes in the MPU_MAIR0 and...
    // 0x00000001 [0]     EN           (0) Region enable
    io_rw_32 mpu_rlar;
 
    _REG_(M33_MPU_RBAR_A1_OFFSET) // M33_MPU_RBAR_A1
    // Provides indirect read and write access to the base address of the MPU region selected by...
    // 0xffffffe0 [31:5]  BASE         (0x0000000) Contains bits [31:5] of the lower inclusive limit of the...
    // 0x00000018 [4:3]   SH           (0x0) Defines the Shareability domain of this region for Normal memory
    // 0x00000006 [2:1]   AP           (0x0) Defines the access permissions for this region
    // 0x00000001 [0]     XN           (0) Defines whether code can be executed from this region
    io_rw_32 mpu_rbar_a1;
 
    _REG_(M33_MPU_RLAR_A1_OFFSET) // M33_MPU_RLAR_A1
    // Provides indirect read and write access to the limit address of the currently selected MPU...
    // 0xffffffe0 [31:5]  LIMIT        (0x0000000) Contains bits [31:5] of the upper inclusive limit of the...
    // 0x0000000e [3:1]   ATTRINDX     (0x0) Associates a set of attributes in the MPU_MAIR0 and...
    // 0x00000001 [0]     EN           (0) Region enable
    io_rw_32 mpu_rlar_a1;
 
    _REG_(M33_MPU_RBAR_A2_OFFSET) // M33_MPU_RBAR_A2
    // Provides indirect read and write access to the base address of the MPU region selected by...
    // 0xffffffe0 [31:5]  BASE         (0x0000000) Contains bits [31:5] of the lower inclusive limit of the...
    // 0x00000018 [4:3]   SH           (0x0) Defines the Shareability domain of this region for Normal memory
    // 0x00000006 [2:1]   AP           (0x0) Defines the access permissions for this region
    // 0x00000001 [0]     XN           (0) Defines whether code can be executed from this region
    io_rw_32 mpu_rbar_a2;
 
    _REG_(M33_MPU_RLAR_A2_OFFSET) // M33_MPU_RLAR_A2
    // Provides indirect read and write access to the limit address of the currently selected MPU...
    // 0xffffffe0 [31:5]  LIMIT        (0x0000000) Contains bits [31:5] of the upper inclusive limit of the...
    // 0x0000000e [3:1]   ATTRINDX     (0x0) Associates a set of attributes in the MPU_MAIR0 and...
    // 0x00000001 [0]     EN           (0) Region enable
    io_rw_32 mpu_rlar_a2;
 
    _REG_(M33_MPU_RBAR_A3_OFFSET) // M33_MPU_RBAR_A3
    // Provides indirect read and write access to the base address of the MPU region selected by...
    // 0xffffffe0 [31:5]  BASE         (0x0000000) Contains bits [31:5] of the lower inclusive limit of the...
    // 0x00000018 [4:3]   SH           (0x0) Defines the Shareability domain of this region for Normal memory
    // 0x00000006 [2:1]   AP           (0x0) Defines the access permissions for this region
    // 0x00000001 [0]     XN           (0) Defines whether code can be executed from this region
    io_rw_32 mpu_rbar_a3;
 
    _REG_(M33_MPU_RLAR_A3_OFFSET) // M33_MPU_RLAR_A3
    // Provides indirect read and write access to the limit address of the currently selected MPU...
    // 0xffffffe0 [31:5]  LIMIT        (0x0000000) Contains bits [31:5] of the upper inclusive limit of the...
    // 0x0000000e [3:1]   ATTRINDX     (0x0) Associates a set of attributes in the MPU_MAIR0 and...
    // 0x00000001 [0]     EN           (0) Region enable
    io_rw_32 mpu_rlar_a3;
 
    uint32_t _pad35;
 
    // (Description copied from array index 0 register M33_MPU_MAIR0 applies similarly to other array indexes)
    _REG_(M33_MPU_MAIR0_OFFSET) // M33_MPU_MAIR0
    // Along with MPU_MAIR1, provides the memory attribute encodings corresponding to the AttrIndex values
    // 0xff000000 [31:24] ATTR3        (0x00) Memory attribute encoding for MPU regions with an AttrIndex of 3
    // 0x00ff0000 [23:16] ATTR2        (0x00) Memory attribute encoding for MPU regions with an AttrIndex of 2
    // 0x0000ff00 [15:8]  ATTR1        (0x00) Memory attribute encoding for MPU regions with an AttrIndex of 1
    // 0x000000ff [7:0]   ATTR0        (0x00) Memory attribute encoding for MPU regions with an AttrIndex of 0
    io_rw_32 mpu_mair[2];
 
    uint32_t _pad36[2];
 
    _REG_(M33_SAU_CTRL_OFFSET) // M33_SAU_CTRL
    // Allows enabling of the Security Attribution Unit
    // 0x00000002 [1]     ALLNS        (0) When SAU_CTRL
    // 0x00000001 [0]     ENABLE       (0) Enables the SAU
    io_rw_32 sau_ctrl;
 
    _REG_(M33_SAU_TYPE_OFFSET) // M33_SAU_TYPE
    // Indicates the number of regions implemented by the Security Attribution Unit
    // 0x000000ff [7:0]   SREGION      (0x08) The number of implemented SAU regions
    io_ro_32 sau_type;
 
    _REG_(M33_SAU_RNR_OFFSET) // M33_SAU_RNR
    // Selects the region currently accessed by SAU_RBAR and SAU_RLAR
    // 0x000000ff [7:0]   REGION       (0x00) Indicates the SAU region accessed by SAU_RBAR and SAU_RLAR
    io_rw_32 sau_rnr;
 
    _REG_(M33_SAU_RBAR_OFFSET) // M33_SAU_RBAR
    // Provides indirect read and write access to the base address of the currently selected SAU region
    // 0xffffffe0 [31:5]  BADDR        (0x0000000) Holds bits [31:5] of the base address for the selected SAU region
    io_rw_32 sau_rbar;
 
    _REG_(M33_SAU_RLAR_OFFSET) // M33_SAU_RLAR
    // Provides indirect read and write access to the limit address of the currently selected SAU region
    // 0xffffffe0 [31:5]  LADDR        (0x0000000) Holds bits [31:5] of the limit address for the selected...
    // 0x00000002 [1]     NSC          (0) Controls whether Non-secure state is permitted to...
    // 0x00000001 [0]     ENABLE       (0) SAU region enable
    io_rw_32 sau_rlar;
 
    _REG_(M33_SFSR_OFFSET) // M33_SFSR
    // Provides information about any security related faults
    // 0x00000080 [7]     LSERR        (0) Sticky flag indicating that an error occurred during...
    // 0x00000040 [6]     SFARVALID    (0) This bit is set when the SFAR register contains a valid value
    // 0x00000020 [5]     LSPERR       (0) Stick flag indicating that an SAU or IDAU violation...
    // 0x00000010 [4]     INVTRAN      (0) Sticky flag indicating that an exception was raised due...
    // 0x00000008 [3]     AUVIOL       (0) Sticky flag indicating that an attempt was made to...
    // 0x00000004 [2]     INVER        (0) This can be caused by EXC_RETURN
    // 0x00000002 [1]     INVIS        (0) This bit is set if the integrity signature in an...
    // 0x00000001 [0]     INVEP        (0) This bit is set if a function call from the Non-secure...
    io_rw_32 sfsr;
 
    _REG_(M33_SFAR_OFFSET) // M33_SFAR
    // Shows the address of the memory location that caused a Security violation
    // 0xffffffff [31:0]  ADDRESS      (0x00000000) The address of an access that caused a attribution unit violation
    io_rw_32 sfar;
 
    uint32_t _pad37;
 
    _REG_(M33_DHCSR_OFFSET) // M33_DHCSR
    // Controls halting debug
    // 0x04000000 [26]    S_RESTART_ST (0) Indicates the PE has processed a request to clear DHCSR
    // 0x02000000 [25]    S_RESET_ST   (0) Indicates whether the PE has been reset since the last...
    // 0x01000000 [24]    S_RETIRE_ST  (0) Set to 1 every time the PE retires one of more instructions
    // 0x00100000 [20]    S_SDE        (0) Indicates whether Secure invasive debug is allowed
    // 0x00080000 [19]    S_LOCKUP     (0) Indicates whether the PE is in Lockup state
    // 0x00040000 [18]    S_SLEEP      (0) Indicates whether the PE is sleeping
    // 0x00020000 [17]    S_HALT       (0) Indicates whether the PE is in Debug state
    // 0x00010000 [16]    S_REGRDY     (0) Handshake flag to transfers through the DCRDR
    // 0x00000020 [5]     C_SNAPSTALL  (0) Allow imprecise entry to Debug state
    // 0x00000008 [3]     C_MASKINTS   (0) When debug is enabled, the debugger can write to this...
    // 0x00000004 [2]     C_STEP       (0) Enable single instruction step
    // 0x00000002 [1]     C_HALT       (0) PE enter Debug state halt request
    // 0x00000001 [0]     C_DEBUGEN    (0) Enable Halting debug
    io_rw_32 dhcsr;
 
    _REG_(M33_DCRSR_OFFSET) // M33_DCRSR
    // With the DCRDR, provides debug access to the general-purpose registers, special-purpose...
    // 0x00010000 [16]    REGWNR       (0) Specifies the access type for the transfer
    // 0x0000007f [6:0]   REGSEL       (0x00) Specifies the general-purpose register, special-purpose...
    io_rw_32 dcrsr;
 
    _REG_(M33_DCRDR_OFFSET) // M33_DCRDR
    // With the DCRSR, provides debug access to the general-purpose registers, special-purpose...
    // 0xffffffff [31:0]  DBGTMP       (0x00000000) Provides debug access for reading and writing the...
    io_rw_32 dcrdr;
 
    _REG_(M33_DEMCR_OFFSET) // M33_DEMCR
    // Manages vector catch behavior and DebugMonitor handling when debugging
    // 0x01000000 [24]    TRCENA       (0) Global enable for all DWT and ITM features
    // 0x00100000 [20]    SDME         (0) Indicates whether the DebugMonitor targets the Secure or...
    // 0x00080000 [19]    MON_REQ      (0) DebugMonitor semaphore bit
    // 0x00040000 [18]    MON_STEP     (0) Enable DebugMonitor stepping
    // 0x00020000 [17]    MON_PEND     (0) Sets or clears the pending state of the DebugMonitor exception
    // 0x00010000 [16]    MON_EN       (0) Enable the DebugMonitor exception
    // 0x00000800 [11]    VC_SFERR     (0) SecureFault exception halting debug vector catch enable
    // 0x00000400 [10]    VC_HARDERR   (0) HardFault exception halting debug vector catch enable
    // 0x00000200 [9]     VC_INTERR    (0) Enable halting debug vector catch for faults during...
    // 0x00000100 [8]     VC_BUSERR    (0) BusFault exception halting debug vector catch enable
    // 0x00000080 [7]     VC_STATERR   (0) Enable halting debug trap on a UsageFault exception...
    // 0x00000040 [6]     VC_CHKERR    (0) Enable halting debug trap on a UsageFault exception...
    // 0x00000020 [5]     VC_NOCPERR   (0) Enable halting debug trap on a UsageFault caused by an...
    // 0x00000010 [4]     VC_MMERR     (0) Enable halting debug trap on a MemManage exception
    // 0x00000001 [0]     VC_CORERESET (0) Enable Reset Vector Catch
    io_rw_32 demcr;
 
    uint32_t _pad38[2];
 
    _REG_(M33_DSCSR_OFFSET) // M33_DSCSR
    // Provides control and status information for Secure debug
    // 0x00020000 [17]    CDSKEY       (0) Writes to the CDS bit are ignored unless CDSKEY is...
    // 0x00010000 [16]    CDS          (0) This field indicates the current Security state of the processor
    // 0x00000002 [1]     SBRSEL       (0) If SBRSELEN is 1 this bit selects whether the Non-secure...
    // 0x00000001 [0]     SBRSELEN     (0) Controls whether the SBRSEL field or the current...
    io_rw_32 dscsr;
 
    uint32_t _pad39[61];
 
    _REG_(M33_STIR_OFFSET) // M33_STIR
    // Provides a mechanism for software to generate an interrupt
    // 0x000001ff [8:0]   INTID        (0x000) Indicates the interrupt to be pended
    io_rw_32 stir;
 
    uint32_t _pad40[12];
 
    _REG_(M33_FPCCR_OFFSET) // M33_FPCCR
    // Holds control data for the Floating-point extension
    // 0x80000000 [31]    ASPEN        (0) When this bit is set to 1, execution of a floating-point...
    // 0x40000000 [30]    LSPEN        (0) Enables lazy context save of floating-point state
    // 0x20000000 [29]    LSPENS       (1) This bit controls whether the LSPEN bit is writeable...
    // 0x10000000 [28]    CLRONRET     (0) Clear floating-point caller saved registers on exception return
    // 0x08000000 [27]    CLRONRETS    (0) This bit controls whether the CLRONRET bit is writeable...
    // 0x04000000 [26]    TS           (0) Treat floating-point registers as Secure enable
    // 0x00000400 [10]    UFRDY        (1) Indicates whether the software executing when the PE...
    // 0x00000200 [9]     SPLIMVIOL    (0) This bit is banked between the Security states and...
    // 0x00000100 [8]     MONRDY       (0) Indicates whether the software executing when the PE...
    // 0x00000080 [7]     SFRDY        (0) Indicates whether the software executing when the PE...
    // 0x00000040 [6]     BFRDY        (1) Indicates whether the software executing when the PE...
    // 0x00000020 [5]     MMRDY        (1) Indicates whether the software executing when the PE...
    // 0x00000010 [4]     HFRDY        (1) Indicates whether the software executing when the PE...
    // 0x00000008 [3]     THREAD       (0) Indicates the PE mode when it allocated the...
    // 0x00000004 [2]     S            (0) Security status of the floating-point context
    // 0x00000002 [1]     USER         (1) Indicates the privilege level of the software executing...
    // 0x00000001 [0]     LSPACT       (0) Indicates whether lazy preservation of the...
    io_rw_32 fpccr;
 
    _REG_(M33_FPCAR_OFFSET) // M33_FPCAR
    // Holds the location of the unpopulated floating-point register space allocated on an exception stack frame
    // 0xfffffff8 [31:3]  ADDRESS      (0x00000000) The location of the unpopulated floating-point register...
    io_rw_32 fpcar;
 
    _REG_(M33_FPDSCR_OFFSET) // M33_FPDSCR
    // Holds the default values for the floating-point status control data that the PE assigns to the...
    // 0x04000000 [26]    AHP          (0) Default value for FPSCR
    // 0x02000000 [25]    DN           (0) Default value for FPSCR
    // 0x01000000 [24]    FZ           (0) Default value for FPSCR
    // 0x00c00000 [23:22] RMODE        (0x0) Default value for FPSCR
    io_rw_32 fpdscr;
 
    // (Description copied from array index 0 register M33_MVFR0 applies similarly to other array indexes)
    _REG_(M33_MVFR0_OFFSET) // M33_MVFR0
    // Describes the features provided by the Floating-point Extension
    // 0xf0000000 [31:28] FPROUND      (0x6) Indicates the rounding modes supported by the FP Extension
    // 0x00f00000 [23:20] FPSQRT       (0x5) Indicates the support for FP square root operations
    // 0x000f0000 [19:16] FPDIVIDE     (0x4) Indicates the support for FP divide operations
    // 0x00000f00 [11:8]  FPDP         (0x6) Indicates support for FP double-precision operations
    // 0x000000f0 [7:4]   FPSP         (0x0) Indicates support for FP single-precision operations
    // 0x0000000f [3:0]   SIMDREG      (0x1) Indicates size of FP register file
    io_ro_32 mvfr[3];
 
    uint32_t _pad41[28];
 
    _REG_(M33_DDEVARCH_OFFSET) // M33_DDEVARCH
    // Provides CoreSight discovery information for the SCS
    // 0xffe00000 [31:21] ARCHITECT    (0x23b) Defines the architect of the component
    // 0x00100000 [20]    PRESENT      (1) Defines that the DEVARCH register is present
    // 0x000f0000 [19:16] REVISION     (0x0) Defines the architecture revision of the component
    // 0x0000f000 [15:12] ARCHVER      (0x2) Defines the architecture version of the component
    // 0x00000fff [11:0]  ARCHPART     (0xa04) Defines the architecture of the component
    io_ro_32 ddevarch;
 
    uint32_t _pad42[3];
 
    _REG_(M33_DDEVTYPE_OFFSET) // M33_DDEVTYPE
    // Provides CoreSight discovery information for the SCS
    // 0x000000f0 [7:4]   SUB          (0x0) Component sub-type
    // 0x0000000f [3:0]   MAJOR        (0x0) CoreSight major type
    io_ro_32 ddevtype;
 
    _REG_(M33_DPIDR4_OFFSET) // M33_DPIDR4
    // Provides CoreSight discovery information for the SCS
    // 0x000000f0 [7:4]   SIZE         (0x0) See CoreSight Architecture Specification
    // 0x0000000f [3:0]   DES_2        (0x4) See CoreSight Architecture Specification
    io_ro_32 dpidr4;
 
    _REG_(M33_DPIDR5_OFFSET) // M33_DPIDR5
    // Provides CoreSight discovery information for the SCS
    // 0x00000000 [31:0]  DPIDR5       (0x00000000) 
    io_rw_32 dpidr5;
 
    _REG_(M33_DPIDR6_OFFSET) // M33_DPIDR6
    // Provides CoreSight discovery information for the SCS
    // 0x00000000 [31:0]  DPIDR6       (0x00000000) 
    io_rw_32 dpidr6;
 
    _REG_(M33_DPIDR7_OFFSET) // M33_DPIDR7
    // Provides CoreSight discovery information for the SCS
    // 0x00000000 [31:0]  DPIDR7       (0x00000000) 
    io_rw_32 dpidr7;
 
    _REG_(M33_DPIDR0_OFFSET) // M33_DPIDR0
    // Provides CoreSight discovery information for the SCS
    // 0x000000ff [7:0]   PART_0       (0x21) See CoreSight Architecture Specification
    io_ro_32 dpidr0;
 
    _REG_(M33_DPIDR1_OFFSET) // M33_DPIDR1
    // Provides CoreSight discovery information for the SCS
    // 0x000000f0 [7:4]   DES_0        (0xb) See CoreSight Architecture Specification
    // 0x0000000f [3:0]   PART_1       (0xd) See CoreSight Architecture Specification
    io_ro_32 dpidr1;
 
    _REG_(M33_DPIDR2_OFFSET) // M33_DPIDR2
    // Provides CoreSight discovery information for the SCS
    // 0x000000f0 [7:4]   REVISION     (0x0) See CoreSight Architecture Specification
    // 0x00000008 [3]     JEDEC        (1) See CoreSight Architecture Specification
    // 0x00000007 [2:0]   DES_1        (0x3) See CoreSight Architecture Specification
    io_ro_32 dpidr2;
 
    _REG_(M33_DPIDR3_OFFSET) // M33_DPIDR3
    // Provides CoreSight discovery information for the SCS
    // 0x000000f0 [7:4]   REVAND       (0x0) See CoreSight Architecture Specification
    // 0x0000000f [3:0]   CMOD         (0x0) See CoreSight Architecture Specification
    io_ro_32 dpidr3;
 
    // (Description copied from array index 0 register M33_DCIDR0 applies similarly to other array indexes)
    _REG_(M33_DCIDR0_OFFSET) // M33_DCIDR0
    // Provides CoreSight discovery information for the SCS
    // 0x000000ff [7:0]   PRMBL_0      (0x0d) See CoreSight Architecture Specification
    io_ro_32 dcidr[4];
 
    uint32_t _pad43[51201];
 
    _REG_(M33_TRCPRGCTLR_OFFSET) // M33_TRCPRGCTLR
    // Programming Control Register
    // 0x00000001 [0]     EN           (0) Trace Unit Enable
    io_rw_32 trcprgctlr;
 
    uint32_t _pad44;
 
    _REG_(M33_TRCSTATR_OFFSET) // M33_TRCSTATR
    // The TRCSTATR indicates the ETM-Teal status
    // 0x00000002 [1]     PMSTABLE     (0) Indicates whether the ETM-Teal registers are stable and...
    // 0x00000001 [0]     IDLE         (0) Indicates that the trace unit is inactive
    io_ro_32 trcstatr;
 
    _REG_(M33_TRCCONFIGR_OFFSET) // M33_TRCCONFIGR
    // The TRCCONFIGR sets the basic tracing options for the trace unit
    // 0x00001000 [12]    RS           (0) Return stack enable
    // 0x00000800 [11]    TS           (0) Global timestamp tracing
    // 0x000007e0 [10:5]  COND         (0x00) Conditional instruction tracing
    // 0x00000010 [4]     CCI          (0) Cycle counting in instruction trace
    // 0x00000008 [3]     BB           (0) Branch broadcast mode
    io_rw_32 trcconfigr;
 
    uint32_t _pad45[3];
 
    _REG_(M33_TRCEVENTCTL0R_OFFSET) // M33_TRCEVENTCTL0R
    // The TRCEVENTCTL0R controls the tracing of events in the trace stream
    // 0x00008000 [15]    TYPE1        (0) Selects the resource type for event 1
    // 0x00000700 [10:8]  SEL1         (0x0) Selects the resource number, based on the value of...
    // 0x00000080 [7]     TYPE0        (0) Selects the resource type for event 0
    // 0x00000007 [2:0]   SEL0         (0x0) Selects the resource number, based on the value of...
    io_rw_32 trceventctl0r;
 
    _REG_(M33_TRCEVENTCTL1R_OFFSET) // M33_TRCEVENTCTL1R
    // The TRCEVENTCTL1R controls how the events selected by TRCEVENTCTL0R behave
    // 0x00001000 [12]    LPOVERRIDE   (0) Low power state behavior override
    // 0x00000800 [11]    ATB          (0) ATB enabled
    // 0x00000002 [1]     INSTEN1      (0) One bit per event, to enable generation of an event...
    // 0x00000001 [0]     INSTEN0      (0) One bit per event, to enable generation of an event...
    io_rw_32 trceventctl1r;
 
    uint32_t _pad46;
 
    _REG_(M33_TRCSTALLCTLR_OFFSET) // M33_TRCSTALLCTLR
    // The TRCSTALLCTLR enables ETM-Teal to stall the processor if the ETM-Teal FIFO goes over the...
    // 0x00000400 [10]    INSTPRIORITY (0) Reserved, RES0
    // 0x00000100 [8]     ISTALL       (0) Stall processor based on instruction trace buffer space
    // 0x0000000c [3:2]   LEVEL        (0x0) Threshold at which stalling becomes active
    io_rw_32 trcstallctlr;
 
    _REG_(M33_TRCTSCTLR_OFFSET) // M33_TRCTSCTLR
    // The TRCTSCTLR controls the insertion of global timestamps into the trace stream
    // 0x00000080 [7]     TYPE0        (0) Selects the resource type for event 0
    // 0x00000003 [1:0]   SEL0         (0x0) Selects the resource number, based on the value of...
    io_rw_32 trctsctlr;
 
    _REG_(M33_TRCSYNCPR_OFFSET) // M33_TRCSYNCPR
    // The TRCSYNCPR specifies the period of trace synchronization of the trace streams
    // 0x0000001f [4:0]   PERIOD       (0x0a) Defines the number of bytes of trace between trace...
    io_ro_32 trcsyncpr;
 
    _REG_(M33_TRCCCCTLR_OFFSET) // M33_TRCCCCTLR
    // The TRCCCCTLR sets the threshold value for instruction trace cycle counting
    // 0x00000fff [11:0]  THRESHOLD    (0x000) Instruction trace cycle count threshold
    io_rw_32 trcccctlr;
 
    uint32_t _pad47[17];
 
    _REG_(M33_TRCVICTLR_OFFSET) // M33_TRCVICTLR
    // The TRCVICTLR controls instruction trace filtering
    // 0x00080000 [19]    EXLEVEL_S3   (0) In Secure state, each bit controls whether instruction...
    // 0x00010000 [16]    EXLEVEL_S0   (0) In Secure state, each bit controls whether instruction...
    // 0x00000800 [11]    TRCERR       (0) Selects whether a system error exception must always be traced
    // 0x00000400 [10]    TRCRESET     (0) Selects whether a reset exception must always be traced
    // 0x00000200 [9]     SSSTATUS     (0) Indicates the current status of the start/stop logic
    // 0x00000080 [7]     TYPE0        (0) Selects the resource type for event 0
    // 0x00000003 [1:0]   SEL0         (0x0) Selects the resource number, based on the value of...
    io_rw_32 trcvictlr;
 
    uint32_t _pad48[47];
 
    _REG_(M33_TRCCNTRLDVR0_OFFSET) // M33_TRCCNTRLDVR0
    // The TRCCNTRLDVR defines the reload value for the reduced function counter
    // 0x0000ffff [15:0]  VALUE        (0x0000) Defines the reload value for the counter
    io_rw_32 trccntrldvr0;
 
    uint32_t _pad49[15];
 
    _REG_(M33_TRCIDR8_OFFSET) // M33_TRCIDR8
    // TRCIDR8
    // 0xffffffff [31:0]  MAXSPEC      (0x00000000) reads as `ImpDef
    io_ro_32 trcidr8;
 
    _REG_(M33_TRCIDR9_OFFSET) // M33_TRCIDR9
    // TRCIDR9
    // 0xffffffff [31:0]  NUMP0KEY     (0x00000000) reads as `ImpDef
    io_ro_32 trcidr9;
 
    _REG_(M33_TRCIDR10_OFFSET) // M33_TRCIDR10
    // TRCIDR10
    // 0xffffffff [31:0]  NUMP1KEY     (0x00000000) reads as `ImpDef
    io_ro_32 trcidr10;
 
    _REG_(M33_TRCIDR11_OFFSET) // M33_TRCIDR11
    // TRCIDR11
    // 0xffffffff [31:0]  NUMP1SPC     (0x00000000) reads as `ImpDef
    io_ro_32 trcidr11;
 
    _REG_(M33_TRCIDR12_OFFSET) // M33_TRCIDR12
    // TRCIDR12
    // 0xffffffff [31:0]  NUMCONDKEY   (0x00000001) reads as `ImpDef
    io_ro_32 trcidr12;
 
    _REG_(M33_TRCIDR13_OFFSET) // M33_TRCIDR13
    // TRCIDR13
    // 0xffffffff [31:0]  NUMCONDSPC   (0x00000000) reads as `ImpDef
    io_ro_32 trcidr13;
 
    uint32_t _pad50[10];
 
    _REG_(M33_TRCIMSPEC_OFFSET) // M33_TRCIMSPEC
    // The TRCIMSPEC shows the presence of any IMPLEMENTATION SPECIFIC features, and enables any...
    // 0x0000000f [3:0]   SUPPORT      (0x0) Reserved, RES0
    io_ro_32 trcimspec;
 
    uint32_t _pad51[7];
 
    _REG_(M33_TRCIDR0_OFFSET) // M33_TRCIDR0
    // TRCIDR0
    // 0x20000000 [29]    COMMOPT      (1) reads as `ImpDef
    // 0x1f000000 [28:24] TSSIZE       (0x08) reads as `ImpDef
    // 0x00020000 [17]    TRCEXDATA    (0) reads as `ImpDef
    // 0x00018000 [16:15] QSUPP        (0x0) reads as `ImpDef
    // 0x00004000 [14]    QFILT        (0) reads as `ImpDef
    // 0x00003000 [13:12] CONDTYPE     (0x0) reads as `ImpDef
    // 0x00000c00 [11:10] NUMEVENT     (0x1) reads as `ImpDef
    // 0x00000200 [9]     RETSTACK     (1) reads as `ImpDef
    // 0x00000080 [7]     TRCCCI       (1) reads as `ImpDef
    // 0x00000040 [6]     TRCCOND      (1) reads as `ImpDef
    // 0x00000020 [5]     TRCBB        (1) reads as `ImpDef
    // 0x00000018 [4:3]   TRCDATA      (0x0) reads as `ImpDef
    // 0x00000006 [2:1]   INSTP0       (0x0) reads as `ImpDef
    // 0x00000001 [0]     RES1         (1) Reserved, RES1
    io_ro_32 trcidr0;
 
    _REG_(M33_TRCIDR1_OFFSET) // M33_TRCIDR1
    // TRCIDR1
    // 0xff000000 [31:24] DESIGNER     (0x41) reads as `ImpDef
    // 0x0000f000 [15:12] RES1         (0xf) Reserved, RES1
    // 0x00000f00 [11:8]  TRCARCHMAJ   (0x4) reads as 0b0100
    // 0x000000f0 [7:4]   TRCARCHMIN   (0x2) reads as 0b0000
    // 0x0000000f [3:0]   REVISION     (0x1) reads as `ImpDef
    io_ro_32 trcidr1;
 
    _REG_(M33_TRCIDR2_OFFSET) // M33_TRCIDR2
    // TRCIDR2
    // 0x1e000000 [28:25] CCSIZE       (0x0) reads as `ImpDef
    // 0x01f00000 [24:20] DVSIZE       (0x00) reads as `ImpDef
    // 0x000f8000 [19:15] DASIZE       (0x00) reads as `ImpDef
    // 0x00007c00 [14:10] VMIDSIZE     (0x00) reads as `ImpDef
    // 0x000003e0 [9:5]   CIDSIZE      (0x00) reads as `ImpDef
    // 0x0000001f [4:0]   IASIZE       (0x04) reads as `ImpDef
    io_ro_32 trcidr2;
 
    _REG_(M33_TRCIDR3_OFFSET) // M33_TRCIDR3
    // TRCIDR3
    // 0x80000000 [31]    NOOVERFLOW   (0) reads as `ImpDef
    // 0x70000000 [30:28] NUMPROC      (0x0) reads as `ImpDef
    // 0x08000000 [27]    SYSSTALL     (1) reads as `ImpDef
    // 0x04000000 [26]    STALLCTL     (1) reads as `ImpDef
    // 0x02000000 [25]    SYNCPR       (1) reads as `ImpDef
    // 0x01000000 [24]    TRCERR       (1) reads as `ImpDef
    // 0x00f00000 [23:20] EXLEVEL_NS   (0x0) reads as `ImpDef
    // 0x000f0000 [19:16] EXLEVEL_S    (0x9) reads as `ImpDef
    // 0x00000fff [11:0]  CCITMIN      (0x004) reads as `ImpDef
    io_ro_32 trcidr3;
 
    _REG_(M33_TRCIDR4_OFFSET) // M33_TRCIDR4
    // TRCIDR4
    // 0xf0000000 [31:28] NUMVMIDC     (0x0) reads as `ImpDef
    // 0x0f000000 [27:24] NUMCIDC      (0x0) reads as `ImpDef
    // 0x00f00000 [23:20] NUMSSCC      (0x1) reads as `ImpDef
    // 0x000f0000 [19:16] NUMRSPAIR    (0x1) reads as `ImpDef
    // 0x0000f000 [15:12] NUMPC        (0x4) reads as `ImpDef
    // 0x00000100 [8]     SUPPDAC      (0) reads as `ImpDef
    // 0x000000f0 [7:4]   NUMDVC       (0x0) reads as `ImpDef
    // 0x0000000f [3:0]   NUMACPAIRS   (0x0) reads as `ImpDef
    io_ro_32 trcidr4;
 
    _REG_(M33_TRCIDR5_OFFSET) // M33_TRCIDR5
    // TRCIDR5
    // 0x80000000 [31]    REDFUNCNTR   (1) reads as `ImpDef
    // 0x70000000 [30:28] NUMCNTR      (0x1) reads as `ImpDef
    // 0x0e000000 [27:25] NUMSEQSTATE  (0x0) reads as `ImpDef
    // 0x00800000 [23]    LPOVERRIDE   (1) reads as `ImpDef
    // 0x00400000 [22]    ATBTRIG      (1) reads as `ImpDef
    // 0x003f0000 [21:16] TRACEIDSIZE  (0x07) reads as 0x07
    // 0x00000e00 [11:9]  NUMEXTINSEL  (0x0) reads as `ImpDef
    // 0x000001ff [8:0]   NUMEXTIN     (0x004) reads as `ImpDef
    io_ro_32 trcidr5;
 
    _REG_(M33_TRCIDR6_OFFSET) // M33_TRCIDR6
    // TRCIDR6
    // 0x00000000 [31:0]  TRCIDR6      (0x00000000) 
    io_rw_32 trcidr6;
 
    _REG_(M33_TRCIDR7_OFFSET) // M33_TRCIDR7
    // TRCIDR7
    // 0x00000000 [31:0]  TRCIDR7      (0x00000000) 
    io_rw_32 trcidr7;
 
    uint32_t _pad52[2];
 
    // (Description copied from array index 0 register M33_TRCRSCTLR2 applies similarly to other array indexes)
    _REG_(M33_TRCRSCTLR2_OFFSET) // M33_TRCRSCTLR2
    // The TRCRSCTLR controls the trace resources
    // 0x00200000 [21]    PAIRINV      (0) Inverts the result of a combined pair of resources
    // 0x00100000 [20]    INV          (0) Inverts the selected resources
    // 0x00070000 [18:16] GROUP        (0x0) Selects a group of resource
    // 0x000000ff [7:0]   SELECT       (0x00) Selects one or more resources from the wanted group
    io_rw_32 trcrsctlr[2];
 
    uint32_t _pad53[36];
 
    _REG_(M33_TRCSSCSR_OFFSET) // M33_TRCSSCSR
    // Controls the corresponding single-shot comparator resource
    // 0x80000000 [31]    STATUS       (0) Single-shot status bit
    // 0x00000008 [3]     PC           (0) Reserved, RES1
    // 0x00000004 [2]     DV           (0) Reserved, RES0
    // 0x00000002 [1]     DA           (0) Reserved, RES0
    // 0x00000001 [0]     INST         (0) Reserved, RES0
    io_rw_32 trcsscsr;
 
    uint32_t _pad54[7];
 
    _REG_(M33_TRCSSPCICR_OFFSET) // M33_TRCSSPCICR
    // Selects the PE comparator inputs for Single-shot control
    // 0x0000000f [3:0]   PC           (0x0) Selects one or more PE comparator inputs for Single-shot control
    io_rw_32 trcsspcicr;
 
    uint32_t _pad55[19];
 
    _REG_(M33_TRCPDCR_OFFSET) // M33_TRCPDCR
    // Requests the system to provide power to the trace unit
    // 0x00000008 [3]     PU           (0) Powerup request bit:
    io_rw_32 trcpdcr;
 
    _REG_(M33_TRCPDSR_OFFSET) // M33_TRCPDSR
    // Returns the following information about the trace unit: - OS Lock status
    // 0x00000020 [5]     OSLK         (0) OS Lock status bit:
    // 0x00000002 [1]     STICKYPD     (1) Sticky powerdown status bit
    // 0x00000001 [0]     POWER        (1) Power status bit:
    io_ro_32 trcpdsr;
 
    uint32_t _pad56[755];
 
    _REG_(M33_TRCITATBIDR_OFFSET) // M33_TRCITATBIDR
    // Trace Integration ATB Identification Register
    // 0x0000007f [6:0]   ID           (0x00) Trace ID
    io_rw_32 trcitatbidr;
 
    uint32_t _pad57[3];
 
    _REG_(M33_TRCITIATBINR_OFFSET) // M33_TRCITIATBINR
    // Trace Integration Instruction ATB In Register
    // 0x00000002 [1]     AFVALIDM     (0) Integration Mode instruction AFVALIDM in
    // 0x00000001 [0]     ATREADYM     (0) Integration Mode instruction ATREADYM in
    io_rw_32 trcitiatbinr;
 
    uint32_t _pad58;
 
    _REG_(M33_TRCITIATBOUTR_OFFSET) // M33_TRCITIATBOUTR
    // Trace Integration Instruction ATB Out Register
    // 0x00000002 [1]     AFREADY      (0) Integration Mode instruction AFREADY out
    // 0x00000001 [0]     ATVALID      (0) Integration Mode instruction ATVALID out
    io_rw_32 trcitiatboutr;
 
    uint32_t _pad59[40];
 
    _REG_(M33_TRCCLAIMSET_OFFSET) // M33_TRCCLAIMSET
    // Claim Tag Set Register
    // 0x00000008 [3]     SET3         (1) When a write to one of these bits occurs, with the value:
    // 0x00000004 [2]     SET2         (1) When a write to one of these bits occurs, with the value:
    // 0x00000002 [1]     SET1         (1) When a write to one of these bits occurs, with the value:
    // 0x00000001 [0]     SET0         (1) When a write to one of these bits occurs, with the value:
    io_rw_32 trcclaimset;
 
    _REG_(M33_TRCCLAIMCLR_OFFSET) // M33_TRCCLAIMCLR
    // Claim Tag Clear Register
    // 0x00000008 [3]     CLR3         (0) When a write to one of these bits occurs, with the value:
    // 0x00000004 [2]     CLR2         (0) When a write to one of these bits occurs, with the value:
    // 0x00000002 [1]     CLR1         (0) When a write to one of these bits occurs, with the value:
    // 0x00000001 [0]     CLR0         (0) When a write to one of these bits occurs, with the value:
    io_rw_32 trcclaimclr;
 
    uint32_t _pad60[4];
 
    _REG_(M33_TRCAUTHSTATUS_OFFSET) // M33_TRCAUTHSTATUS
    // Returns the level of tracing that the trace unit can support
    // 0x000000c0 [7:6]   SNID         (0x0) Indicates whether the system enables the trace unit to...
    // 0x00000030 [5:4]   SID          (0x0) Indicates whether the trace unit supports Secure invasive debug:
    // 0x0000000c [3:2]   NSNID        (0x0) Indicates whether the system enables the trace unit to...
    // 0x00000003 [1:0]   NSID         (0x0) Indicates whether the trace unit supports Non-secure...
    io_ro_32 trcauthstatus;
 
    _REG_(M33_TRCDEVARCH_OFFSET) // M33_TRCDEVARCH
    // TRCDEVARCH
    // 0xffe00000 [31:21] ARCHITECT    (0x23b) reads as 0b01000111011
    // 0x00100000 [20]    PRESENT      (1) reads as 0b1
    // 0x000f0000 [19:16] REVISION     (0x2) reads as 0b0000
    // 0x0000ffff [15:0]  ARCHID       (0x4a13) reads as 0b0100101000010011
    io_ro_32 trcdevarch;
 
    uint32_t _pad61[2];
 
    _REG_(M33_TRCDEVID_OFFSET) // M33_TRCDEVID
    // TRCDEVID
    // 0x00000000 [31:0]  TRCDEVID     (0x00000000) 
    io_rw_32 trcdevid;
 
    _REG_(M33_TRCDEVTYPE_OFFSET) // M33_TRCDEVTYPE
    // TRCDEVTYPE
    // 0x000000f0 [7:4]   SUB          (0x1) reads as 0b0001
    // 0x0000000f [3:0]   MAJOR        (0x3) reads as 0b0011
    io_ro_32 trcdevtype;
 
    _REG_(M33_TRCPIDR4_OFFSET) // M33_TRCPIDR4
    // TRCPIDR4
    // 0x000000f0 [7:4]   SIZE         (0x0) reads as `ImpDef
    // 0x0000000f [3:0]   DES_2        (0x4) reads as `ImpDef
    io_ro_32 trcpidr4;
 
    _REG_(M33_TRCPIDR5_OFFSET) // M33_TRCPIDR5
    // TRCPIDR5
    // 0x00000000 [31:0]  TRCPIDR5     (0x00000000) 
    io_rw_32 trcpidr5;
 
    _REG_(M33_TRCPIDR6_OFFSET) // M33_TRCPIDR6
    // TRCPIDR6
    // 0x00000000 [31:0]  TRCPIDR6     (0x00000000) 
    io_rw_32 trcpidr6;
 
    _REG_(M33_TRCPIDR7_OFFSET) // M33_TRCPIDR7
    // TRCPIDR7
    // 0x00000000 [31:0]  TRCPIDR7     (0x00000000) 
    io_rw_32 trcpidr7;
 
    _REG_(M33_TRCPIDR0_OFFSET) // M33_TRCPIDR0
    // TRCPIDR0
    // 0x000000ff [7:0]   PART_0       (0x21) reads as `ImpDef
    io_ro_32 trcpidr0;
 
    _REG_(M33_TRCPIDR1_OFFSET) // M33_TRCPIDR1
    // TRCPIDR1
    // 0x000000f0 [7:4]   DES_0        (0xb) reads as `ImpDef
    // 0x0000000f [3:0]   PART_0       (0xd) reads as `ImpDef
    io_ro_32 trcpidr1;
 
    _REG_(M33_TRCPIDR2_OFFSET) // M33_TRCPIDR2
    // TRCPIDR2
    // 0x000000f0 [7:4]   REVISION     (0x2) reads as `ImpDef
    // 0x00000008 [3]     JEDEC        (1) reads as 0b1
    // 0x00000007 [2:0]   DES_0        (0x3) reads as `ImpDef
    io_ro_32 trcpidr2;
 
    _REG_(M33_TRCPIDR3_OFFSET) // M33_TRCPIDR3
    // TRCPIDR3
    // 0x000000f0 [7:4]   REVAND       (0x0) reads as `ImpDef
    // 0x0000000f [3:0]   CMOD         (0x0) reads as `ImpDef
    io_ro_32 trcpidr3;
 
    // (Description copied from array index 0 register M33_TRCCIDR0 applies similarly to other array indexes)
    _REG_(M33_TRCCIDR0_OFFSET) // M33_TRCCIDR0
    // TRCCIDR0
    // 0x000000ff [7:0]   PRMBL_0      (0x0d) reads as 0b00001101
    io_ro_32 trccidr[4];
 
    _REG_(M33_CTICONTROL_OFFSET) // M33_CTICONTROL
    // CTI Control Register
    // 0x00000001 [0]     GLBEN        (0) Enables or disables the CTI
    io_rw_32 cticontrol;
 
    uint32_t _pad62[3];
 
    _REG_(M33_CTIINTACK_OFFSET) // M33_CTIINTACK
    // CTI Interrupt Acknowledge Register
    // 0x000000ff [7:0]   INTACK       (0x00) Acknowledges the corresponding ctitrigout output
    io_rw_32 ctiintack;
 
    _REG_(M33_CTIAPPSET_OFFSET) // M33_CTIAPPSET
    // CTI Application Trigger Set Register
    // 0x0000000f [3:0]   APPSET       (0x0) Setting a bit HIGH generates a channel event for the...
    io_rw_32 ctiappset;
 
    _REG_(M33_CTIAPPCLEAR_OFFSET) // M33_CTIAPPCLEAR
    // CTI Application Trigger Clear Register
    // 0x0000000f [3:0]   APPCLEAR     (0x0) Sets the corresponding bits in the CTIAPPSET to 0
    io_rw_32 ctiappclear;
 
    _REG_(M33_CTIAPPPULSE_OFFSET) // M33_CTIAPPPULSE
    // CTI Application Pulse Register
    // 0x0000000f [3:0]   APPULSE      (0x0) Setting a bit HIGH generates a channel event pulse for...
    io_rw_32 ctiapppulse;
 
    // (Description copied from array index 0 register M33_CTIINEN0 applies similarly to other array indexes)
    _REG_(M33_CTIINEN0_OFFSET) // M33_CTIINEN0
    // CTI Trigger to Channel Enable Registers
    // 0x0000000f [3:0]   TRIGINEN     (0x0) Enables a cross trigger event to the corresponding...
    io_rw_32 ctiinen[8];
 
    uint32_t _pad63[24];
 
    // (Description copied from array index 0 register M33_CTIOUTEN0 applies similarly to other array indexes)
    _REG_(M33_CTIOUTEN0_OFFSET) // M33_CTIOUTEN0
    // CTI Trigger to Channel Enable Registers
    // 0x0000000f [3:0]   TRIGOUTEN    (0x0) Enables a cross trigger event to ctitrigout when the...
    io_rw_32 ctiouten[8];
 
    uint32_t _pad64[28];
 
    _REG_(M33_CTITRIGINSTATUS_OFFSET) // M33_CTITRIGINSTATUS
    // CTI Trigger to Channel Enable Registers
    // 0x000000ff [7:0]   TRIGINSTATUS (0x00) Shows the status of the ctitrigin inputs
    io_ro_32 ctitriginstatus;
 
    _REG_(M33_CTITRIGOUTSTATUS_OFFSET) // M33_CTITRIGOUTSTATUS
    // CTI Trigger In Status Register
    // 0x000000ff [7:0]   TRIGOUTSTATUS (0x00) Shows the status of the ctitrigout outputs
    io_ro_32 ctitrigoutstatus;
 
    _REG_(M33_CTICHINSTATUS_OFFSET) // M33_CTICHINSTATUS
    // CTI Channel In Status Register
    // 0x0000000f [3:0]   CTICHOUTSTATUS (0x0) Shows the status of the ctichout outputs
    io_ro_32 ctichinstatus;
 
    uint32_t _pad65;
 
    _REG_(M33_CTIGATE_OFFSET) // M33_CTIGATE
    // Enable CTI Channel Gate register
    // 0x00000008 [3]     CTIGATEEN3   (1) Enable ctichout3
    // 0x00000004 [2]     CTIGATEEN2   (1) Enable ctichout2
    // 0x00000002 [1]     CTIGATEEN1   (1) Enable ctichout1
    // 0x00000001 [0]     CTIGATEEN0   (1) Enable ctichout0
    io_rw_32 ctigate;
 
    _REG_(M33_ASICCTL_OFFSET) // M33_ASICCTL
    // External Multiplexer Control register
    // 0x00000000 [31:0]  ASICCTL      (0x00000000) 
    io_rw_32 asicctl;
 
    uint32_t _pad66[871];
 
    _REG_(M33_ITCHOUT_OFFSET) // M33_ITCHOUT
    // Integration Test Channel Output register
    // 0x0000000f [3:0]   CTCHOUT      (0x0) Sets the value of the ctichout outputs
    io_rw_32 itchout;
 
    _REG_(M33_ITTRIGOUT_OFFSET) // M33_ITTRIGOUT
    // Integration Test Trigger Output register
    // 0x000000ff [7:0]   CTTRIGOUT    (0x00) Sets the value of the ctitrigout outputs
    io_rw_32 ittrigout;
 
    uint32_t _pad67[2];
 
    _REG_(M33_ITCHIN_OFFSET) // M33_ITCHIN
    // Integration Test Channel Input register
    // 0x0000000f [3:0]   CTCHIN       (0x0) Reads the value of the ctichin inputs
    io_ro_32 itchin;
 
    uint32_t _pad68[2];
 
    _REG_(M33_ITCTRL_OFFSET) // M33_ITCTRL
    // Integration Mode Control register
    // 0x00000001 [0]     IME          (0) Integration Mode Enable
    io_rw_32 itctrl;
 
    uint32_t _pad69[46];
 
    _REG_(M33_DEVARCH_OFFSET) // M33_DEVARCH
    // Device Architecture register
    // 0xffe00000 [31:21] ARCHITECT    (0x23b) Indicates the component architect
    // 0x00100000 [20]    PRESENT      (1) Indicates whether the DEVARCH register is present
    // 0x000f0000 [19:16] REVISION     (0x0) Indicates the architecture revision
    // 0x0000ffff [15:0]  ARCHID       (0x1a14) Indicates the component
    io_ro_32 devarch;
 
    uint32_t _pad70[2];
 
    _REG_(M33_DEVID_OFFSET) // M33_DEVID
    // Device Configuration register
    // 0x000f0000 [19:16] NUMCH        (0x4) Number of ECT channels available
    // 0x0000ff00 [15:8]  NUMTRIG      (0x08) Number of ECT triggers available
    // 0x0000001f [4:0]   EXTMUXNUM    (0x00) Indicates the number of multiplexers available on...
    io_ro_32 devid;
 
    _REG_(M33_DEVTYPE_OFFSET) // M33_DEVTYPE
    // Device Type Identifier register
    // 0x000000f0 [7:4]   SUB          (0x1) Sub-classification of the type of the debug component as...
    // 0x0000000f [3:0]   MAJOR        (0x4) Major classification of the type of the debug component...
    io_ro_32 devtype;
 
    _REG_(M33_PIDR4_OFFSET) // M33_PIDR4
    // CoreSight Peripheral ID4
    // 0x000000f0 [7:4]   SIZE         (0x0) Always 0b0000
    // 0x0000000f [3:0]   DES_2        (0x4) Together, PIDR1
    io_ro_32 pidr4;
 
    _REG_(M33_PIDR5_OFFSET) // M33_PIDR5
    // CoreSight Peripheral ID5
    // 0x00000000 [31:0]  PIDR5        (0x00000000) 
    io_rw_32 pidr5;
 
    _REG_(M33_PIDR6_OFFSET) // M33_PIDR6
    // CoreSight Peripheral ID6
    // 0x00000000 [31:0]  PIDR6        (0x00000000) 
    io_rw_32 pidr6;
 
    _REG_(M33_PIDR7_OFFSET) // M33_PIDR7
    // CoreSight Peripheral ID7
    // 0x00000000 [31:0]  PIDR7        (0x00000000) 
    io_rw_32 pidr7;
 
    _REG_(M33_PIDR0_OFFSET) // M33_PIDR0
    // CoreSight Peripheral ID0
    // 0x000000ff [7:0]   PART_0       (0x21) Bits[7:0] of the 12-bit part number of the component
    io_ro_32 pidr0;
 
    _REG_(M33_PIDR1_OFFSET) // M33_PIDR1
    // CoreSight Peripheral ID1
    // 0x000000f0 [7:4]   DES_0        (0xb) Together, PIDR1
    // 0x0000000f [3:0]   PART_1       (0xd) Bits[11:8] of the 12-bit part number of the component
    io_ro_32 pidr1;
 
    _REG_(M33_PIDR2_OFFSET) // M33_PIDR2
    // CoreSight Peripheral ID2
    // 0x000000f0 [7:4]   REVISION     (0x0) This device is at r1p0
    // 0x00000008 [3]     JEDEC        (1) Always 1
    // 0x00000007 [2:0]   DES_1        (0x3) Together, PIDR1
    io_ro_32 pidr2;
 
    _REG_(M33_PIDR3_OFFSET) // M33_PIDR3
    // CoreSight Peripheral ID3
    // 0x000000f0 [7:4]   REVAND       (0x0) Indicates minor errata fixes specific to the revision of...
    // 0x0000000f [3:0]   CMOD         (0x0) Customer Modified
    io_ro_32 pidr3;
 
    // (Description copied from array index 0 register M33_CIDR0 applies similarly to other array indexes)
    _REG_(M33_CIDR0_OFFSET) // M33_CIDR0
    // CoreSight Component ID0
    // 0x000000ff [7:0]   PRMBL_0      (0x0d) Preamble[0]
    io_ro_32 cidr[4];
} m33_hw_t;

#define m33_hw ((m33_hw_t *)PPB_BASE)
#define m33_ns_hw ((m33_hw_t *)PPB_NONSEC_BASE)
static_assert(sizeof (m33_hw_t) == 0x43000, "");

#endif // _HARDWARE_STRUCTS_M33_H

