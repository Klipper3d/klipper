// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _HARDWARE_STRUCTS_XIP_AUX_H
#define _HARDWARE_STRUCTS_XIP_AUX_H

/**
 * \file rp2350/xip_aux.h
 */

#include "hardware/address_mapped.h"
#include "hardware/regs/xip_aux.h"

// Reference to datasheet: https://datasheets.raspberrypi.com/rp2350/rp2350-datasheet.pdf#tab-registerlist_xip_aux
//
// The _REG_ macro is intended to help make the register navigable in your IDE (for example, using the "Go to Definition" feature)
// _REG_(x) will link to the corresponding register in hardware/regs/xip_aux.h.
//
// Bit-field descriptions are of the form:
// BITMASK [BITRANGE] FIELDNAME (RESETVALUE) DESCRIPTION

typedef struct {
    _REG_(XIP_AUX_STREAM_OFFSET) // XIP_AUX_STREAM
    // Read the XIP stream FIFO (fast bus access to XIP_CTRL_STREAM_FIFO)
    // 0xffffffff [31:0]  STREAM       (0x00000000) 
    io_ro_32 stream;
 
    _REG_(XIP_AUX_QMI_DIRECT_TX_OFFSET) // XIP_AUX_QMI_DIRECT_TX
    // Write to the QMI direct-mode TX FIFO (fast bus access to QMI_DIRECT_TX)
    // 0x00100000 [20]    NOPUSH       (0) Inhibit the RX FIFO push that would correspond to this...
    // 0x00080000 [19]    OE           (0) Output enable (active-high)
    // 0x00040000 [18]    DWIDTH       (0) Data width
    // 0x00030000 [17:16] IWIDTH       (0x0) Configure whether this FIFO record is transferred with...
    // 0x0000ffff [15:0]  DATA         (0x0000) Data pushed here will be clocked out falling edges of...
    io_wo_32 qmi_direct_tx;
 
    _REG_(XIP_AUX_QMI_DIRECT_RX_OFFSET) // XIP_AUX_QMI_DIRECT_RX
    // Read from the QMI direct-mode RX FIFO (fast bus access to QMI_DIRECT_RX)
    // 0x0000ffff [15:0]  QMI_DIRECT_RX (0x0000) With each byte clocked out on the serial interface, one...
    io_ro_32 qmi_direct_rx;
} xip_aux_hw_t;

#define xip_aux_hw ((xip_aux_hw_t *)XIP_AUX_BASE)
static_assert(sizeof (xip_aux_hw_t) == 0x000c, "");

#endif // _HARDWARE_STRUCTS_XIP_AUX_H

