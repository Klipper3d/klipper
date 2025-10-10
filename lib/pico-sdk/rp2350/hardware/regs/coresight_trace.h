// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
// =============================================================================
// Register block : CORESIGHT_TRACE
// Version        : 1
// Bus type       : ahbl
// Description    : Coresight block - RP specific registers
// =============================================================================
#ifndef _HARDWARE_REGS_CORESIGHT_TRACE_H
#define _HARDWARE_REGS_CORESIGHT_TRACE_H
// =============================================================================
// Register    : CORESIGHT_TRACE_CTRL_STATUS
// Description : Control and status register
#define CORESIGHT_TRACE_CTRL_STATUS_OFFSET _u(0x00000000)
#define CORESIGHT_TRACE_CTRL_STATUS_BITS   _u(0x00000003)
#define CORESIGHT_TRACE_CTRL_STATUS_RESET  _u(0x00000001)
// -----------------------------------------------------------------------------
// Field       : CORESIGHT_TRACE_CTRL_STATUS_TRACE_CAPTURE_FIFO_OVERFLOW
// Description : This status flag is set high when trace data has been dropped
//               due to the FIFO being full at the point trace data was sampled.
//               Write 1 to acknowledge and clear the bit.
#define CORESIGHT_TRACE_CTRL_STATUS_TRACE_CAPTURE_FIFO_OVERFLOW_RESET  _u(0x0)
#define CORESIGHT_TRACE_CTRL_STATUS_TRACE_CAPTURE_FIFO_OVERFLOW_BITS   _u(0x00000002)
#define CORESIGHT_TRACE_CTRL_STATUS_TRACE_CAPTURE_FIFO_OVERFLOW_MSB    _u(1)
#define CORESIGHT_TRACE_CTRL_STATUS_TRACE_CAPTURE_FIFO_OVERFLOW_LSB    _u(1)
#define CORESIGHT_TRACE_CTRL_STATUS_TRACE_CAPTURE_FIFO_OVERFLOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : CORESIGHT_TRACE_CTRL_STATUS_TRACE_CAPTURE_FIFO_FLUSH
// Description : Set to 1 to continuously hold the trace FIFO in a flushed state
//               and prevent overflow.
//
//               Before clearing this flag, configure and start a DMA channel
//               with the correct DREQ for the TRACE_CAPTURE_FIFO register.
//
//               Clear this flag to begin sampling trace data, and set once
//               again once the trace capture buffer is full. You must configure
//               the TPIU in order to generate trace packets to be captured, as
//               well as components like the ETM further upstream to generate
//               the event stream propagated to the TPIU.
#define CORESIGHT_TRACE_CTRL_STATUS_TRACE_CAPTURE_FIFO_FLUSH_RESET  _u(0x1)
#define CORESIGHT_TRACE_CTRL_STATUS_TRACE_CAPTURE_FIFO_FLUSH_BITS   _u(0x00000001)
#define CORESIGHT_TRACE_CTRL_STATUS_TRACE_CAPTURE_FIFO_FLUSH_MSB    _u(0)
#define CORESIGHT_TRACE_CTRL_STATUS_TRACE_CAPTURE_FIFO_FLUSH_LSB    _u(0)
#define CORESIGHT_TRACE_CTRL_STATUS_TRACE_CAPTURE_FIFO_FLUSH_ACCESS "RW"
// =============================================================================
// Register    : CORESIGHT_TRACE_TRACE_CAPTURE_FIFO
// Description : FIFO for trace data captured from the TPIU
#define CORESIGHT_TRACE_TRACE_CAPTURE_FIFO_OFFSET _u(0x00000004)
#define CORESIGHT_TRACE_TRACE_CAPTURE_FIFO_BITS   _u(0xffffffff)
#define CORESIGHT_TRACE_TRACE_CAPTURE_FIFO_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : CORESIGHT_TRACE_TRACE_CAPTURE_FIFO_RDATA
// Description : Read from an 8 x 32-bit FIFO containing trace data captured
//               from the TPIU.
//
//               Hardware pushes to the FIFO on rising edges of clk_sys, when
//               either of the following is true:
//
//               * TPIU TRACECTL output is low (normal trace data)
//
//               * TPIU TRACETCL output is high, and TPIU TRACEDATA0 and
//               TRACEDATA1 are both low (trigger packet)
//
//               These conditions are in accordance with Arm Coresight
//               Architecture Spec v3.0 section D3.3.3: Decoding requirements
//               for Trace Capture Devices
//
//               The data captured into the FIFO is the full 32-bit TRACEDATA
//               bus output by the TPIU. Note that the TPIU is a DDR output at
//               half of clk_sys, therefore this interface can capture the full
//               32-bit TPIU DDR output bandwidth as it samples once per active
//               edge of the TPIU output clock.
#define CORESIGHT_TRACE_TRACE_CAPTURE_FIFO_RDATA_RESET  _u(0x00000000)
#define CORESIGHT_TRACE_TRACE_CAPTURE_FIFO_RDATA_BITS   _u(0xffffffff)
#define CORESIGHT_TRACE_TRACE_CAPTURE_FIFO_RDATA_MSB    _u(31)
#define CORESIGHT_TRACE_TRACE_CAPTURE_FIFO_RDATA_LSB    _u(0)
#define CORESIGHT_TRACE_TRACE_CAPTURE_FIFO_RDATA_ACCESS "RF"
// =============================================================================
#endif // _HARDWARE_REGS_CORESIGHT_TRACE_H

