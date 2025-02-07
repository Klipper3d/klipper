// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
// =============================================================================
// Register block : XIP_AUX
// Version        : 1
// Bus type       : ahb
// Description    : Auxiliary DMA access to XIP FIFOs, via fast AHB bus access
// =============================================================================
#ifndef _HARDWARE_REGS_XIP_AUX_H
#define _HARDWARE_REGS_XIP_AUX_H
// =============================================================================
// Register    : XIP_AUX_STREAM
// Description : Read the XIP stream FIFO (fast bus access to
//               XIP_CTRL_STREAM_FIFO)
#define XIP_AUX_STREAM_OFFSET _u(0x00000000)
#define XIP_AUX_STREAM_BITS   _u(0xffffffff)
#define XIP_AUX_STREAM_RESET  _u(0x00000000)
#define XIP_AUX_STREAM_MSB    _u(31)
#define XIP_AUX_STREAM_LSB    _u(0)
#define XIP_AUX_STREAM_ACCESS "RF"
// =============================================================================
// Register    : XIP_AUX_QMI_DIRECT_TX
// Description : Write to the QMI direct-mode TX FIFO (fast bus access to
//               QMI_DIRECT_TX)
#define XIP_AUX_QMI_DIRECT_TX_OFFSET _u(0x00000004)
#define XIP_AUX_QMI_DIRECT_TX_BITS   _u(0x001fffff)
#define XIP_AUX_QMI_DIRECT_TX_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : XIP_AUX_QMI_DIRECT_TX_NOPUSH
// Description : Inhibit the RX FIFO push that would correspond to this TX FIFO
//               entry.
//
//               Useful to avoid garbage appearing in the RX FIFO when pushing
//               the command at the beginning of a SPI transfer.
#define XIP_AUX_QMI_DIRECT_TX_NOPUSH_RESET  _u(0x0)
#define XIP_AUX_QMI_DIRECT_TX_NOPUSH_BITS   _u(0x00100000)
#define XIP_AUX_QMI_DIRECT_TX_NOPUSH_MSB    _u(20)
#define XIP_AUX_QMI_DIRECT_TX_NOPUSH_LSB    _u(20)
#define XIP_AUX_QMI_DIRECT_TX_NOPUSH_ACCESS "WF"
// -----------------------------------------------------------------------------
// Field       : XIP_AUX_QMI_DIRECT_TX_OE
// Description : Output enable (active-high). For single width (SPI), this field
//               is ignored, and SD0 is always set to output, with SD1 always
//               set to input.
//
//               For dual and quad width (DSPI/QSPI), this sets whether the
//               relevant SDx pads are set to output whilst transferring this
//               FIFO record. In this case the command/address should have OE
//               set, and the data transfer should have OE set or clear
//               depending on the direction of the transfer.
#define XIP_AUX_QMI_DIRECT_TX_OE_RESET  _u(0x0)
#define XIP_AUX_QMI_DIRECT_TX_OE_BITS   _u(0x00080000)
#define XIP_AUX_QMI_DIRECT_TX_OE_MSB    _u(19)
#define XIP_AUX_QMI_DIRECT_TX_OE_LSB    _u(19)
#define XIP_AUX_QMI_DIRECT_TX_OE_ACCESS "WF"
// -----------------------------------------------------------------------------
// Field       : XIP_AUX_QMI_DIRECT_TX_DWIDTH
// Description : Data width. If 0, hardware will transmit the 8 LSBs of the
//               DIRECT_TX DATA field, and return an 8-bit value in the 8 LSBs
//               of DIRECT_RX. If 1, the full 16-bit width is used. 8-bit and
//               16-bit transfers can be mixed freely.
#define XIP_AUX_QMI_DIRECT_TX_DWIDTH_RESET  _u(0x0)
#define XIP_AUX_QMI_DIRECT_TX_DWIDTH_BITS   _u(0x00040000)
#define XIP_AUX_QMI_DIRECT_TX_DWIDTH_MSB    _u(18)
#define XIP_AUX_QMI_DIRECT_TX_DWIDTH_LSB    _u(18)
#define XIP_AUX_QMI_DIRECT_TX_DWIDTH_ACCESS "WF"
// -----------------------------------------------------------------------------
// Field       : XIP_AUX_QMI_DIRECT_TX_IWIDTH
// Description : Configure whether this FIFO record is transferred with
//               single/dual/quad interface width (0/1/2). Different widths can
//               be mixed freely.
//               0x0 -> Single width
//               0x1 -> Dual width
//               0x2 -> Quad width
#define XIP_AUX_QMI_DIRECT_TX_IWIDTH_RESET  _u(0x0)
#define XIP_AUX_QMI_DIRECT_TX_IWIDTH_BITS   _u(0x00030000)
#define XIP_AUX_QMI_DIRECT_TX_IWIDTH_MSB    _u(17)
#define XIP_AUX_QMI_DIRECT_TX_IWIDTH_LSB    _u(16)
#define XIP_AUX_QMI_DIRECT_TX_IWIDTH_ACCESS "WF"
#define XIP_AUX_QMI_DIRECT_TX_IWIDTH_VALUE_S _u(0x0)
#define XIP_AUX_QMI_DIRECT_TX_IWIDTH_VALUE_D _u(0x1)
#define XIP_AUX_QMI_DIRECT_TX_IWIDTH_VALUE_Q _u(0x2)
// -----------------------------------------------------------------------------
// Field       : XIP_AUX_QMI_DIRECT_TX_DATA
// Description : Data pushed here will be clocked out falling edges of SCK (or
//               before the very first rising edge of SCK, if this is the first
//               pulse). For each byte clocked out, the interface will
//               simultaneously sample one byte, on rising edges of SCK, and
//               push this to the DIRECT_RX FIFO.
//
//               For 16-bit data, the least-significant byte is transmitted
//               first.
#define XIP_AUX_QMI_DIRECT_TX_DATA_RESET  _u(0x0000)
#define XIP_AUX_QMI_DIRECT_TX_DATA_BITS   _u(0x0000ffff)
#define XIP_AUX_QMI_DIRECT_TX_DATA_MSB    _u(15)
#define XIP_AUX_QMI_DIRECT_TX_DATA_LSB    _u(0)
#define XIP_AUX_QMI_DIRECT_TX_DATA_ACCESS "WF"
// =============================================================================
// Register    : XIP_AUX_QMI_DIRECT_RX
// Description : Read from the QMI direct-mode RX FIFO (fast bus access to
//               QMI_DIRECT_RX)
//               With each byte clocked out on the serial interface, one byte
//               will simultaneously be clocked in, and will appear in this
//               FIFO. The serial interface will stall when this FIFO is full,
//               to avoid dropping data.
//
//               When 16-bit data is pushed into the TX FIFO, the corresponding
//               RX FIFO push will also contain 16 bits of data. The least-
//               significant byte is the first one received.
#define XIP_AUX_QMI_DIRECT_RX_OFFSET _u(0x00000008)
#define XIP_AUX_QMI_DIRECT_RX_BITS   _u(0x0000ffff)
#define XIP_AUX_QMI_DIRECT_RX_RESET  _u(0x00000000)
#define XIP_AUX_QMI_DIRECT_RX_MSB    _u(15)
#define XIP_AUX_QMI_DIRECT_RX_LSB    _u(0)
#define XIP_AUX_QMI_DIRECT_RX_ACCESS "RF"
// =============================================================================
#endif // _HARDWARE_REGS_XIP_AUX_H

