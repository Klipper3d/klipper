// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _HARDWARE_STRUCTS_CORESIGHT_TRACE_H
#define _HARDWARE_STRUCTS_CORESIGHT_TRACE_H

/**
 * \file rp2350/coresight_trace.h
 */

#include "hardware/address_mapped.h"
#include "hardware/regs/coresight_trace.h"

// Reference to datasheet: https://datasheets.raspberrypi.com/rp2350/rp2350-datasheet.pdf#tab-registerlist_coresight_trace
//
// The _REG_ macro is intended to help make the register navigable in your IDE (for example, using the "Go to Definition" feature)
// _REG_(x) will link to the corresponding register in hardware/regs/coresight_trace.h.
//
// Bit-field descriptions are of the form:
// BITMASK [BITRANGE] FIELDNAME (RESETVALUE) DESCRIPTION

typedef struct {
    _REG_(CORESIGHT_TRACE_CTRL_STATUS_OFFSET) // CORESIGHT_TRACE_CTRL_STATUS
    // Control and status register
    // 0x00000002 [1]     TRACE_CAPTURE_FIFO_OVERFLOW (0) This status flag is set high when trace data has been...
    // 0x00000001 [0]     TRACE_CAPTURE_FIFO_FLUSH (1) Set to 1 to continuously hold the trace FIFO in a...
    io_rw_32 ctrl_status;
 
    _REG_(CORESIGHT_TRACE_TRACE_CAPTURE_FIFO_OFFSET) // CORESIGHT_TRACE_TRACE_CAPTURE_FIFO
    // FIFO for trace data captured from the TPIU
    // 0xffffffff [31:0]  RDATA        (0x00000000) Read from an 8 x 32-bit FIFO containing trace data...
    io_ro_32 trace_capture_fifo;
} coresight_trace_hw_t;

#define coresight_trace_hw ((coresight_trace_hw_t *)CORESIGHT_TRACE_BASE)
static_assert(sizeof (coresight_trace_hw_t) == 0x0008, "");

#endif // _HARDWARE_STRUCTS_CORESIGHT_TRACE_H

