// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _HARDWARE_STRUCTS_XOSC_H
#define _HARDWARE_STRUCTS_XOSC_H

/**
 * \file rp2350/xosc.h
 */

#include "hardware/address_mapped.h"
#include "hardware/regs/xosc.h"

// Reference to datasheet: https://datasheets.raspberrypi.com/rp2350/rp2350-datasheet.pdf#tab-registerlist_xosc
//
// The _REG_ macro is intended to help make the register navigable in your IDE (for example, using the "Go to Definition" feature)
// _REG_(x) will link to the corresponding register in hardware/regs/xosc.h.
//
// Bit-field descriptions are of the form:
// BITMASK [BITRANGE] FIELDNAME (RESETVALUE) DESCRIPTION

/// \tag::xosc_hw[]
typedef struct {
    _REG_(XOSC_CTRL_OFFSET) // XOSC_CTRL
    // Crystal Oscillator Control
    // 0x00fff000 [23:12] ENABLE       (-) On power-up this field is initialised to DISABLE and the...
    // 0x00000fff [11:0]  FREQ_RANGE   (-) The 12-bit code is intended to give some protection...
    io_rw_32 ctrl;
 
    _REG_(XOSC_STATUS_OFFSET) // XOSC_STATUS
    // Crystal Oscillator Status
    // 0x80000000 [31]    STABLE       (0) Oscillator is running and stable
    // 0x01000000 [24]    BADWRITE     (0) An invalid value has been written to CTRL_ENABLE or...
    // 0x00001000 [12]    ENABLED      (-) Oscillator is enabled but not necessarily running and...
    // 0x00000003 [1:0]   FREQ_RANGE   (-) The current frequency range setting
    io_rw_32 status;
 
    _REG_(XOSC_DORMANT_OFFSET) // XOSC_DORMANT
    // Crystal Oscillator pause control
    // 0xffffffff [31:0]  DORMANT      (-) This is used to save power by pausing the XOSC +
    io_rw_32 dormant;
 
    _REG_(XOSC_STARTUP_OFFSET) // XOSC_STARTUP
    // Controls the startup delay
    // 0x00100000 [20]    X4           (-) Multiplies the startup_delay by 4, just in case
    // 0x00003fff [13:0]  DELAY        (-) in multiples of 256*xtal_period
    io_rw_32 startup;
 
    _REG_(XOSC_COUNT_OFFSET) // XOSC_COUNT
    // A down counter running at the XOSC frequency which counts to zero and stops.
    // 0x0000ffff [15:0]  COUNT        (0x0000) 
    io_rw_32 count;
} xosc_hw_t;
/// \end::xosc_hw[]

#define xosc_hw ((xosc_hw_t *)XOSC_BASE)
static_assert(sizeof (xosc_hw_t) == 0x0014, "");

#endif // _HARDWARE_STRUCTS_XOSC_H

