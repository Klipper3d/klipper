// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _HARDWARE_STRUCTS_HSTX_CTRL_H
#define _HARDWARE_STRUCTS_HSTX_CTRL_H

/**
 * \file rp2350/hstx_ctrl.h
 */

#include "hardware/address_mapped.h"
#include "hardware/regs/hstx_ctrl.h"

// Reference to datasheet: https://datasheets.raspberrypi.com/rp2350/rp2350-datasheet.pdf#tab-registerlist_hstx_ctrl
//
// The _REG_ macro is intended to help make the register navigable in your IDE (for example, using the "Go to Definition" feature)
// _REG_(x) will link to the corresponding register in hardware/regs/hstx_ctrl.h.
//
// Bit-field descriptions are of the form:
// BITMASK [BITRANGE] FIELDNAME (RESETVALUE) DESCRIPTION

typedef struct {
    _REG_(HSTX_CTRL_CSR_OFFSET) // HSTX_CTRL_CSR
    // 0xf0000000 [31:28] CLKDIV       (0x1) Clock period of the generated clock, measured in HSTX...
    // 0x0f000000 [27:24] CLKPHASE     (0x0) Set the initial phase of the generated clock
    // 0x001f0000 [20:16] N_SHIFTS     (0x05) Number of times to shift the shift register before...
    // 0x00001f00 [12:8]  SHIFT        (0x06) How many bits to right-rotate the shift register by each cycle
    // 0x00000060 [6:5]   COUPLED_SEL  (0x0) Select which PIO to use for coupled mode operation
    // 0x00000010 [4]     COUPLED_MODE (0) Enable the PIO-to-HSTX 1:1 connection
    // 0x00000002 [1]     EXPAND_EN    (0) Enable the command expander
    // 0x00000001 [0]     EN           (0) When EN is 1, the HSTX will shift out data as it appears...
    io_rw_32 csr;
 
    // (Description copied from array index 0 register HSTX_CTRL_BIT0 applies similarly to other array indexes)
    _REG_(HSTX_CTRL_BIT0_OFFSET) // HSTX_CTRL_BIT0
    // Data control register for output bit 0
    // 0x00020000 [17]    CLK          (0) Connect this output to the generated clock, rather than...
    // 0x00010000 [16]    INV          (0) Invert this data output (logical NOT)
    // 0x00001f00 [12:8]  SEL_N        (0x00) Shift register data bit select for the second half of...
    // 0x0000001f [4:0]   SEL_P        (0x00) Shift register data bit select for the first half of the...
    io_rw_32 bit[8];
 
    _REG_(HSTX_CTRL_EXPAND_SHIFT_OFFSET) // HSTX_CTRL_EXPAND_SHIFT
    // Configure the optional shifter inside the command expander
    // 0x1f000000 [28:24] ENC_N_SHIFTS (0x01) Number of times to consume from the shift register...
    // 0x001f0000 [20:16] ENC_SHIFT    (0x00) How many bits to right-rotate the shift register by each...
    // 0x00001f00 [12:8]  RAW_N_SHIFTS (0x01) Number of times to consume from the shift register...
    // 0x0000001f [4:0]   RAW_SHIFT    (0x00) How many bits to right-rotate the shift register by each...
    io_rw_32 expand_shift;
 
    _REG_(HSTX_CTRL_EXPAND_TMDS_OFFSET) // HSTX_CTRL_EXPAND_TMDS
    // Configure the optional TMDS encoder inside the command expander
    // 0x00e00000 [23:21] L2_NBITS     (0x0) Number of valid data bits for the lane 2 TMDS encoder,...
    // 0x001f0000 [20:16] L2_ROT       (0x00) Right-rotate applied to the current shifter data before...
    // 0x0000e000 [15:13] L1_NBITS     (0x0) Number of valid data bits for the lane 1 TMDS encoder,...
    // 0x00001f00 [12:8]  L1_ROT       (0x00) Right-rotate applied to the current shifter data before...
    // 0x000000e0 [7:5]   L0_NBITS     (0x0) Number of valid data bits for the lane 0 TMDS encoder,...
    // 0x0000001f [4:0]   L0_ROT       (0x00) Right-rotate applied to the current shifter data before...
    io_rw_32 expand_tmds;
} hstx_ctrl_hw_t;

#define hstx_ctrl_hw ((hstx_ctrl_hw_t *)HSTX_CTRL_BASE)
static_assert(sizeof (hstx_ctrl_hw_t) == 0x002c, "");

#endif // _HARDWARE_STRUCTS_HSTX_CTRL_H

