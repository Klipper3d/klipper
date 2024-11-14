// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _HARDWARE_STRUCTS_SYSCFG_H
#define _HARDWARE_STRUCTS_SYSCFG_H

/**
 * \file rp2350/syscfg.h
 */

#include "hardware/address_mapped.h"
#include "hardware/regs/syscfg.h"

// Reference to datasheet: https://datasheets.raspberrypi.com/rp2350/rp2350-datasheet.pdf#tab-registerlist_syscfg
//
// The _REG_ macro is intended to help make the register navigable in your IDE (for example, using the "Go to Definition" feature)
// _REG_(x) will link to the corresponding register in hardware/regs/syscfg.h.
//
// Bit-field descriptions are of the form:
// BITMASK [BITRANGE] FIELDNAME (RESETVALUE) DESCRIPTION

typedef struct {
    _REG_(SYSCFG_PROC_CONFIG_OFFSET) // SYSCFG_PROC_CONFIG
    // Configuration for processors
    // 0x00000002 [1]     PROC1_HALTED (0) Indication that proc1 has halted
    // 0x00000001 [0]     PROC0_HALTED (0) Indication that proc0 has halted
    io_ro_32 proc_config;
 
    _REG_(SYSCFG_PROC_IN_SYNC_BYPASS_OFFSET) // SYSCFG_PROC_IN_SYNC_BYPASS
    // For each bit, if 1, bypass the input synchronizer between that GPIO +
    // 0xffffffff [31:0]  GPIO         (0x00000000) 
    io_rw_32 proc_in_sync_bypass;
 
    _REG_(SYSCFG_PROC_IN_SYNC_BYPASS_HI_OFFSET) // SYSCFG_PROC_IN_SYNC_BYPASS_HI
    // For each bit, if 1, bypass the input synchronizer between that GPIO +
    // 0xf0000000 [31:28] QSPI_SD      (0x0) 
    // 0x08000000 [27]    QSPI_CSN     (0) 
    // 0x04000000 [26]    QSPI_SCK     (0) 
    // 0x02000000 [25]    USB_DM       (0) 
    // 0x01000000 [24]    USB_DP       (0) 
    // 0x0000ffff [15:0]  GPIO         (0x0000) 
    io_rw_32 proc_in_sync_bypass_hi;
 
    _REG_(SYSCFG_DBGFORCE_OFFSET) // SYSCFG_DBGFORCE
    // Directly control the chip SWD debug port
    // 0x00000008 [3]     ATTACH       (0) Attach chip debug port to syscfg controls, and...
    // 0x00000004 [2]     SWCLK        (1) Directly drive SWCLK, if ATTACH is set
    // 0x00000002 [1]     SWDI         (1) Directly drive SWDIO input, if ATTACH is set
    // 0x00000001 [0]     SWDO         (-) Observe the value of SWDIO output
    io_rw_32 dbgforce;
 
    _REG_(SYSCFG_MEMPOWERDOWN_OFFSET) // SYSCFG_MEMPOWERDOWN
    // Control PD pins to memories
    // 0x00001000 [12]    BOOTRAM      (0) 
    // 0x00000800 [11]    ROM          (0) 
    // 0x00000400 [10]    USB          (0) 
    // 0x00000200 [9]     SRAM9        (0) 
    // 0x00000100 [8]     SRAM8        (0) 
    // 0x00000080 [7]     SRAM7        (0) 
    // 0x00000040 [6]     SRAM6        (0) 
    // 0x00000020 [5]     SRAM5        (0) 
    // 0x00000010 [4]     SRAM4        (0) 
    // 0x00000008 [3]     SRAM3        (0) 
    // 0x00000004 [2]     SRAM2        (0) 
    // 0x00000002 [1]     SRAM1        (0) 
    // 0x00000001 [0]     SRAM0        (0) 
    io_rw_32 mempowerdown;
 
    _REG_(SYSCFG_AUXCTRL_OFFSET) // SYSCFG_AUXCTRL
    // Auxiliary system control register
    // 0x000000ff [7:0]   AUXCTRL      (0x00) * Bits 7:2: Reserved
    io_rw_32 auxctrl;
} syscfg_hw_t;

#define syscfg_hw ((syscfg_hw_t *)SYSCFG_BASE)
static_assert(sizeof (syscfg_hw_t) == 0x0018, "");

#endif // _HARDWARE_STRUCTS_SYSCFG_H

