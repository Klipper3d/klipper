// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _HARDWARE_STRUCTS_TMDS_ENCODE_H
#define _HARDWARE_STRUCTS_TMDS_ENCODE_H

/**
 * \file rp2350/tmds_encode.h
 */

#include "hardware/address_mapped.h"
#include "hardware/regs/sio.h"

// Reference to datasheet: https://datasheets.raspberrypi.com/rp2350/rp2350-datasheet.pdf#tab-registerlist_sio
//
// The _REG_ macro is intended to help make the register navigable in your IDE (for example, using the "Go to Definition" feature)
// _REG_(x) will link to the corresponding register in hardware/regs/sio.h.
//
// Bit-field descriptions are of the form:
// BITMASK [BITRANGE] FIELDNAME (RESETVALUE) DESCRIPTION

typedef struct {
    _REG_(SIO_TMDS_CTRL_OFFSET) // SIO_TMDS_CTRL
    // Control register for TMDS encoder
    // 0x10000000 [28]    CLEAR_BALANCE (0) Clear the running DC balance state of the TMDS encoders
    // 0x08000000 [27]    PIX2_NOSHIFT (0) When encoding two pixels's worth of symbols in one cycle...
    // 0x07000000 [26:24] PIX_SHIFT    (0x0) Shift applied to the colour data register with each read...
    // 0x00800000 [23]    INTERLEAVE   (0) Enable lane interleaving for reads of PEEK_SINGLE/POP_SINGLE
    // 0x001c0000 [20:18] L2_NBITS     (0x0) Number of valid colour MSBs for lane 2 (1-8 bits,...
    // 0x00038000 [17:15] L1_NBITS     (0x0) Number of valid colour MSBs for lane 1 (1-8 bits,...
    // 0x00007000 [14:12] L0_NBITS     (0x0) Number of valid colour MSBs for lane 0 (1-8 bits,...
    // 0x00000f00 [11:8]  L2_ROT       (0x0) Right-rotate the 16 LSBs of the colour accumulator by...
    // 0x000000f0 [7:4]   L1_ROT       (0x0) Right-rotate the 16 LSBs of the colour accumulator by...
    // 0x0000000f [3:0]   L0_ROT       (0x0) Right-rotate the 16 LSBs of the colour accumulator by...
    io_rw_32 tmds_ctrl;
 
    _REG_(SIO_TMDS_WDATA_OFFSET) // SIO_TMDS_WDATA
    // Write-only access to the TMDS colour data register
    // 0xffffffff [31:0]  TMDS_WDATA   (0x00000000) 
    io_wo_32 tmds_wdata;
 
    _REG_(SIO_TMDS_PEEK_SINGLE_OFFSET) // SIO_TMDS_PEEK_SINGLE
    // Get the encoding of one pixel's worth of colour data, packed into a 32-bit value (3x10-bit symbols)
    // 0xffffffff [31:0]  TMDS_PEEK_SINGLE (0x00000000) 
    io_ro_32 tmds_peek_single;
 
    _REG_(SIO_TMDS_POP_SINGLE_OFFSET) // SIO_TMDS_POP_SINGLE
    // Get the encoding of one pixel's worth of colour data, packed into a 32-bit value
    // 0xffffffff [31:0]  TMDS_POP_SINGLE (0x00000000) 
    io_ro_32 tmds_pop_single;
 
    _REG_(SIO_TMDS_PEEK_DOUBLE_L0_OFFSET) // SIO_TMDS_PEEK_DOUBLE_L0
    // Get lane 0 of the encoding of two pixels' worth of colour data
    // 0xffffffff [31:0]  TMDS_PEEK_DOUBLE_L0 (0x00000000) 
    io_ro_32 tmds_peek_double_l0;
 
    _REG_(SIO_TMDS_POP_DOUBLE_L0_OFFSET) // SIO_TMDS_POP_DOUBLE_L0
    // Get lane 0 of the encoding of two pixels' worth of colour data
    // 0xffffffff [31:0]  TMDS_POP_DOUBLE_L0 (0x00000000) 
    io_ro_32 tmds_pop_double_l0;
 
    _REG_(SIO_TMDS_PEEK_DOUBLE_L1_OFFSET) // SIO_TMDS_PEEK_DOUBLE_L1
    // Get lane 1 of the encoding of two pixels' worth of colour data
    // 0xffffffff [31:0]  TMDS_PEEK_DOUBLE_L1 (0x00000000) 
    io_ro_32 tmds_peek_double_l1;
 
    _REG_(SIO_TMDS_POP_DOUBLE_L1_OFFSET) // SIO_TMDS_POP_DOUBLE_L1
    // Get lane 1 of the encoding of two pixels' worth of colour data
    // 0xffffffff [31:0]  TMDS_POP_DOUBLE_L1 (0x00000000) 
    io_ro_32 tmds_pop_double_l1;
 
    _REG_(SIO_TMDS_PEEK_DOUBLE_L2_OFFSET) // SIO_TMDS_PEEK_DOUBLE_L2
    // Get lane 2 of the encoding of two pixels' worth of colour data
    // 0xffffffff [31:0]  TMDS_PEEK_DOUBLE_L2 (0x00000000) 
    io_ro_32 tmds_peek_double_l2;
 
    _REG_(SIO_TMDS_POP_DOUBLE_L2_OFFSET) // SIO_TMDS_POP_DOUBLE_L2
    // Get lane 2 of the encoding of two pixels' worth of colour data
    // 0xffffffff [31:0]  TMDS_POP_DOUBLE_L2 (0x00000000) 
    io_ro_32 tmds_pop_double_l2;
} tmds_encode_hw_t;

#define tmds_encode_hw ((tmds_encode_hw_t *)(SIO_BASE + SIO_TMDS_CTRL_OFFSET))
#define tmds_encode_ns_hw ((tmds_encode_hw_t *)(SIO_NONSEC_BASE + SIO_TMDS_CTRL_OFFSET))
static_assert(sizeof (tmds_encode_hw_t) == 0x0028, "");

#endif // _HARDWARE_STRUCTS_TMDS_ENCODE_H

