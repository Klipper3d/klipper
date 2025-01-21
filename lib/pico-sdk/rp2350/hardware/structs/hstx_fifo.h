// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _HARDWARE_STRUCTS_HSTX_FIFO_H
#define _HARDWARE_STRUCTS_HSTX_FIFO_H

/**
 * \file rp2350/hstx_fifo.h
 */

#include "hardware/address_mapped.h"
#include "hardware/regs/hstx_fifo.h"

// Reference to datasheet: https://datasheets.raspberrypi.com/rp2350/rp2350-datasheet.pdf#tab-registerlist_hstx_fifo
//
// The _REG_ macro is intended to help make the register navigable in your IDE (for example, using the "Go to Definition" feature)
// _REG_(x) will link to the corresponding register in hardware/regs/hstx_fifo.h.
//
// Bit-field descriptions are of the form:
// BITMASK [BITRANGE] FIELDNAME (RESETVALUE) DESCRIPTION

typedef struct {
    _REG_(HSTX_FIFO_STAT_OFFSET) // HSTX_FIFO_STAT
    // FIFO status
    // 0x00000400 [10]    WOF          (0) FIFO was written when full
    // 0x00000200 [9]     EMPTY        (-) 
    // 0x00000100 [8]     FULL         (-) 
    // 0x000000ff [7:0]   LEVEL        (0x00) 
    io_rw_32 stat;
 
    _REG_(HSTX_FIFO_FIFO_OFFSET) // HSTX_FIFO_FIFO
    // Write access to FIFO
    // 0xffffffff [31:0]  FIFO         (0x00000000) 
    io_wo_32 fifo;
} hstx_fifo_hw_t;

#define hstx_fifo_hw ((hstx_fifo_hw_t *)HSTX_FIFO_BASE)
static_assert(sizeof (hstx_fifo_hw_t) == 0x0008, "");

#endif // _HARDWARE_STRUCTS_HSTX_FIFO_H

