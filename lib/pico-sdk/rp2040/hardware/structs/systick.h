// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _HARDWARE_STRUCTS_SYSTICK_H
#define _HARDWARE_STRUCTS_SYSTICK_H

/**
 * \file rp2040/systick.h
 */

#include "hardware/address_mapped.h"
#include "hardware/regs/m0plus.h"

// Reference to datasheet: https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf#tab-registerlist_m0plus
//
// The _REG_ macro is intended to help make the register navigable in your IDE (for example, using the "Go to Definition" feature)
// _REG_(x) will link to the corresponding register in hardware/regs/m0plus.h.
//
// Bit-field descriptions are of the form:
// BITMASK [BITRANGE] FIELDNAME (RESETVALUE) DESCRIPTION

typedef struct {
    _REG_(M0PLUS_SYST_CSR_OFFSET) // M0PLUS_SYST_CSR
    // SysTick Control and Status Register
    // 0x00010000 [16]    COUNTFLAG    (0) Returns 1 if timer counted to 0 since last time this was read
    // 0x00000004 [2]     CLKSOURCE    (0) SysTick clock source
    // 0x00000002 [1]     TICKINT      (0) Enables SysTick exception request: +
    // 0x00000001 [0]     ENABLE       (0) Enable SysTick counter: +
    io_rw_32 csr;
 
    _REG_(M0PLUS_SYST_RVR_OFFSET) // M0PLUS_SYST_RVR
    // SysTick Reload Value Register
    // 0x00ffffff [23:0]  RELOAD       (0x000000) Value to load into the SysTick Current Value Register...
    io_rw_32 rvr;
 
    _REG_(M0PLUS_SYST_CVR_OFFSET) // M0PLUS_SYST_CVR
    // SysTick Current Value Register
    // 0x00ffffff [23:0]  CURRENT      (0x000000) Reads return the current value of the SysTick counter
    io_rw_32 cvr;
 
    _REG_(M0PLUS_SYST_CALIB_OFFSET) // M0PLUS_SYST_CALIB
    // SysTick Calibration Value Register
    // 0x80000000 [31]    NOREF        (0) If reads as 1, the Reference clock is not provided - the...
    // 0x40000000 [30]    SKEW         (0) If reads as 1, the calibration value for 10ms is inexact...
    // 0x00ffffff [23:0]  TENMS        (0x000000) An optional Reload value to be used for 10ms (100Hz)...
    io_ro_32 calib;
} systick_hw_t;

#define systick_hw ((systick_hw_t *)(PPB_BASE + M0PLUS_SYST_CSR_OFFSET))
static_assert(sizeof (systick_hw_t) == 0x0010, "");

#endif // _HARDWARE_STRUCTS_SYSTICK_H

