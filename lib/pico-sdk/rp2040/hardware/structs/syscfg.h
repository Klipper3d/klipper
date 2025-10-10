// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _HARDWARE_STRUCTS_SYSCFG_H
#define _HARDWARE_STRUCTS_SYSCFG_H

/**
 * \file rp2040/syscfg.h
 */

#include "hardware/address_mapped.h"
#include "hardware/regs/syscfg.h"

// Reference to datasheet: https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf#tab-registerlist_syscfg
//
// The _REG_ macro is intended to help make the register navigable in your IDE (for example, using the "Go to Definition" feature)
// _REG_(x) will link to the corresponding register in hardware/regs/syscfg.h.
//
// Bit-field descriptions are of the form:
// BITMASK [BITRANGE] FIELDNAME (RESETVALUE) DESCRIPTION

typedef struct {
    _REG_(SYSCFG_PROC0_NMI_MASK_OFFSET) // SYSCFG_PROC0_NMI_MASK
    // Processor core 0 NMI source mask
    // 0xffffffff [31:0]  PROC0_NMI_MASK (0x00000000) Set a bit high to enable NMI from that IRQ
    io_rw_32 proc0_nmi_mask;
 
    _REG_(SYSCFG_PROC1_NMI_MASK_OFFSET) // SYSCFG_PROC1_NMI_MASK
    // Processor core 1 NMI source mask
    // 0xffffffff [31:0]  PROC1_NMI_MASK (0x00000000) Set a bit high to enable NMI from that IRQ
    io_rw_32 proc1_nmi_mask;
 
    _REG_(SYSCFG_PROC_CONFIG_OFFSET) // SYSCFG_PROC_CONFIG
    // Configuration for processors
    // 0xf0000000 [31:28] PROC1_DAP_INSTID (0x1) Configure proc1 DAP instance ID
    // 0x0f000000 [27:24] PROC0_DAP_INSTID (0x0) Configure proc0 DAP instance ID
    // 0x00000002 [1]     PROC1_HALTED (0) Indication that proc1 has halted
    // 0x00000001 [0]     PROC0_HALTED (0) Indication that proc0 has halted
    io_rw_32 proc_config;
 
    _REG_(SYSCFG_PROC_IN_SYNC_BYPASS_OFFSET) // SYSCFG_PROC_IN_SYNC_BYPASS
    // For each bit, if 1, bypass the input synchronizer between that GPIO +
    // 0x3fffffff [29:0]  PROC_IN_SYNC_BYPASS (0x00000000) 
    io_rw_32 proc_in_sync_bypass;
 
    _REG_(SYSCFG_PROC_IN_SYNC_BYPASS_HI_OFFSET) // SYSCFG_PROC_IN_SYNC_BYPASS_HI
    // For each bit, if 1, bypass the input synchronizer between that GPIO +
    // 0x0000003f [5:0]   PROC_IN_SYNC_BYPASS_HI (0x00) 
    io_rw_32 proc_in_sync_bypass_hi;
 
    _REG_(SYSCFG_DBGFORCE_OFFSET) // SYSCFG_DBGFORCE
    // Directly control the SWD debug port of either processor
    // 0x00000080 [7]     PROC1_ATTACH (0) Attach processor 1 debug port to syscfg controls, and...
    // 0x00000040 [6]     PROC1_SWCLK  (1) Directly drive processor 1 SWCLK, if PROC1_ATTACH is set
    // 0x00000020 [5]     PROC1_SWDI   (1) Directly drive processor 1 SWDIO input, if PROC1_ATTACH is set
    // 0x00000010 [4]     PROC1_SWDO   (-) Observe the value of processor 1 SWDIO output
    // 0x00000008 [3]     PROC0_ATTACH (0) Attach processor 0 debug port to syscfg controls, and...
    // 0x00000004 [2]     PROC0_SWCLK  (1) Directly drive processor 0 SWCLK, if PROC0_ATTACH is set
    // 0x00000002 [1]     PROC0_SWDI   (1) Directly drive processor 0 SWDIO input, if PROC0_ATTACH is set
    // 0x00000001 [0]     PROC0_SWDO   (-) Observe the value of processor 0 SWDIO output
    io_rw_32 dbgforce;
 
    _REG_(SYSCFG_MEMPOWERDOWN_OFFSET) // SYSCFG_MEMPOWERDOWN
    // Control power downs to memories
    // 0x00000080 [7]     ROM          (0) 
    // 0x00000040 [6]     USB          (0) 
    // 0x00000020 [5]     SRAM5        (0) 
    // 0x00000010 [4]     SRAM4        (0) 
    // 0x00000008 [3]     SRAM3        (0) 
    // 0x00000004 [2]     SRAM2        (0) 
    // 0x00000002 [1]     SRAM1        (0) 
    // 0x00000001 [0]     SRAM0        (0) 
    io_rw_32 mempowerdown;
} syscfg_hw_t;

#define syscfg_hw ((syscfg_hw_t *)SYSCFG_BASE)
static_assert(sizeof (syscfg_hw_t) == 0x001c, "");

#endif // _HARDWARE_STRUCTS_SYSCFG_H

