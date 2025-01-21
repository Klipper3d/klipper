// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _HARDWARE_STRUCTS_PLL_H
#define _HARDWARE_STRUCTS_PLL_H

/**
 * \file rp2350/pll.h
 */

#include "hardware/address_mapped.h"
#include "hardware/regs/pll.h"

// Reference to datasheet: https://datasheets.raspberrypi.com/rp2350/rp2350-datasheet.pdf#tab-registerlist_pll
//
// The _REG_ macro is intended to help make the register navigable in your IDE (for example, using the "Go to Definition" feature)
// _REG_(x) will link to the corresponding register in hardware/regs/pll.h.
//
// Bit-field descriptions are of the form:
// BITMASK [BITRANGE] FIELDNAME (RESETVALUE) DESCRIPTION

/// \tag::pll_hw[]
typedef struct {
    _REG_(PLL_CS_OFFSET) // PLL_CS
    // Control and Status
    // 0x80000000 [31]    LOCK         (0) PLL is locked
    // 0x40000000 [30]    LOCK_N       (0) PLL is not locked +
    // 0x00000100 [8]     BYPASS       (0) Passes the reference clock to the output instead of the...
    // 0x0000003f [5:0]   REFDIV       (0x01) Divides the PLL input reference clock
    io_rw_32 cs;
 
    _REG_(PLL_PWR_OFFSET) // PLL_PWR
    // Controls the PLL power modes
    // 0x00000020 [5]     VCOPD        (1) PLL VCO powerdown +
    // 0x00000008 [3]     POSTDIVPD    (1) PLL post divider powerdown +
    // 0x00000004 [2]     DSMPD        (1) PLL DSM powerdown +
    // 0x00000001 [0]     PD           (1) PLL powerdown +
    io_rw_32 pwr;
 
    _REG_(PLL_FBDIV_INT_OFFSET) // PLL_FBDIV_INT
    // Feedback divisor
    // 0x00000fff [11:0]  FBDIV_INT    (0x000) see ctrl reg description for constraints
    io_rw_32 fbdiv_int;
 
    _REG_(PLL_PRIM_OFFSET) // PLL_PRIM
    // Controls the PLL post dividers for the primary output
    // 0x00070000 [18:16] POSTDIV1     (0x7) divide by 1-7
    // 0x00007000 [14:12] POSTDIV2     (0x7) divide by 1-7
    io_rw_32 prim;
 
    _REG_(PLL_INTR_OFFSET) // PLL_INTR
    // Raw Interrupts
    // 0x00000001 [0]     LOCK_N_STICKY (0) 
    io_rw_32 intr;
 
    _REG_(PLL_INTE_OFFSET) // PLL_INTE
    // Interrupt Enable
    // 0x00000001 [0]     LOCK_N_STICKY (0) 
    io_rw_32 inte;
 
    _REG_(PLL_INTF_OFFSET) // PLL_INTF
    // Interrupt Force
    // 0x00000001 [0]     LOCK_N_STICKY (0) 
    io_rw_32 intf;
 
    _REG_(PLL_INTS_OFFSET) // PLL_INTS
    // Interrupt status after masking & forcing
    // 0x00000001 [0]     LOCK_N_STICKY (0) 
    io_ro_32 ints;
} pll_hw_t;
/// \end::pll_hw[]

#define pll_sys_hw ((pll_hw_t *)PLL_SYS_BASE)
#define pll_usb_hw ((pll_hw_t *)PLL_USB_BASE)
static_assert(sizeof (pll_hw_t) == 0x0020, "");

#endif // _HARDWARE_STRUCTS_PLL_H

