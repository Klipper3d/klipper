// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _HARDWARE_STRUCTS_QMI_H
#define _HARDWARE_STRUCTS_QMI_H

/**
 * \file rp2350/qmi.h
 */

#include "hardware/address_mapped.h"
#include "hardware/regs/qmi.h"

// Reference to datasheet: https://datasheets.raspberrypi.com/rp2350/rp2350-datasheet.pdf#tab-registerlist_qmi
//
// The _REG_ macro is intended to help make the register navigable in your IDE (for example, using the "Go to Definition" feature)
// _REG_(x) will link to the corresponding register in hardware/regs/qmi.h.
//
// Bit-field descriptions are of the form:
// BITMASK [BITRANGE] FIELDNAME (RESETVALUE) DESCRIPTION

typedef struct {
    _REG_(QMI_M0_TIMING_OFFSET) // QMI_M0_TIMING
    // Timing configuration register for memory address window 0
    // 0xc0000000 [31:30] COOLDOWN     (0x1) Chip select cooldown period
    // 0x30000000 [29:28] PAGEBREAK    (0x0) When page break is enabled, chip select will...
    // 0x02000000 [25]    SELECT_SETUP (0) Add up to one additional system clock cycle of setup...
    // 0x01800000 [24:23] SELECT_HOLD  (0x0) Add up to three additional system clock cycles of active...
    // 0x007e0000 [22:17] MAX_SELECT   (0x00) Enforce a maximum assertion duration for this window's...
    // 0x0001f000 [16:12] MIN_DESELECT (0x00) After this window's chip select is deasserted, it...
    // 0x00000700 [10:8]  RXDELAY      (0x0) Delay the read data sample timing, in units of one half...
    // 0x000000ff [7:0]   CLKDIV       (0x04) Clock divisor
    io_rw_32 timing;
 
    _REG_(QMI_M0_RFMT_OFFSET) // QMI_M0_RFMT
    // Read transfer format configuration for memory address window 0.
    // 0x10000000 [28]    DTR          (0) Enable double transfer rate (DTR) for read commands:...
    // 0x00070000 [18:16] DUMMY_LEN    (0x0) Length of dummy phase between command suffix and data...
    // 0x0000c000 [15:14] SUFFIX_LEN   (0x0) Length of post-address command suffix, in units of 4 bits
    // 0x00001000 [12]    PREFIX_LEN   (1) Length of command prefix, in units of 8 bits
    // 0x00000300 [9:8]   DATA_WIDTH   (0x0) The width used for the data transfer
    // 0x000000c0 [7:6]   DUMMY_WIDTH  (0x0) The width used for the dummy phase, if any
    // 0x00000030 [5:4]   SUFFIX_WIDTH (0x0) The width used for the post-address command suffix, if any
    // 0x0000000c [3:2]   ADDR_WIDTH   (0x0) The transfer width used for the address
    // 0x00000003 [1:0]   PREFIX_WIDTH (0x0) The transfer width used for the command prefix, if any
    io_rw_32 rfmt;
 
    _REG_(QMI_M0_RCMD_OFFSET) // QMI_M0_RCMD
    // Command constants used for reads from memory address window 0.
    // 0x0000ff00 [15:8]  SUFFIX       (0xa0) The command suffix bits following the address, if...
    // 0x000000ff [7:0]   PREFIX       (0x03) The command prefix bits to prepend on each new transfer,...
    io_rw_32 rcmd;
 
    _REG_(QMI_M0_WFMT_OFFSET) // QMI_M0_WFMT
    // Write transfer format configuration for memory address window 0.
    // 0x10000000 [28]    DTR          (0) Enable double transfer rate (DTR) for write commands:...
    // 0x00070000 [18:16] DUMMY_LEN    (0x0) Length of dummy phase between command suffix and data...
    // 0x0000c000 [15:14] SUFFIX_LEN   (0x0) Length of post-address command suffix, in units of 4 bits
    // 0x00001000 [12]    PREFIX_LEN   (1) Length of command prefix, in units of 8 bits
    // 0x00000300 [9:8]   DATA_WIDTH   (0x0) The width used for the data transfer
    // 0x000000c0 [7:6]   DUMMY_WIDTH  (0x0) The width used for the dummy phase, if any
    // 0x00000030 [5:4]   SUFFIX_WIDTH (0x0) The width used for the post-address command suffix, if any
    // 0x0000000c [3:2]   ADDR_WIDTH   (0x0) The transfer width used for the address
    // 0x00000003 [1:0]   PREFIX_WIDTH (0x0) The transfer width used for the command prefix, if any
    io_rw_32 wfmt;
 
    _REG_(QMI_M0_WCMD_OFFSET) // QMI_M0_WCMD
    // Command constants used for writes to memory address window 0.
    // 0x0000ff00 [15:8]  SUFFIX       (0xa0) The command suffix bits following the address, if...
    // 0x000000ff [7:0]   PREFIX       (0x02) The command prefix bits to prepend on each new transfer,...
    io_rw_32 wcmd;
} qmi_mem_hw_t;

typedef struct {
    _REG_(QMI_DIRECT_CSR_OFFSET) // QMI_DIRECT_CSR
    // Control and status for direct serial mode
    // 0xc0000000 [31:30] RXDELAY      (0x0) Delay the read data sample timing, in units of one half...
    // 0x3fc00000 [29:22] CLKDIV       (0x06) Clock divisor for direct serial mode
    // 0x001c0000 [20:18] RXLEVEL      (0x0) Current level of DIRECT_RX FIFO
    // 0x00020000 [17]    RXFULL       (0) When 1, the DIRECT_RX FIFO is currently full
    // 0x00010000 [16]    RXEMPTY      (0) When 1, the DIRECT_RX FIFO is currently empty
    // 0x00007000 [14:12] TXLEVEL      (0x0) Current level of DIRECT_TX FIFO
    // 0x00000800 [11]    TXEMPTY      (0) When 1, the DIRECT_TX FIFO is currently empty
    // 0x00000400 [10]    TXFULL       (0) When 1, the DIRECT_TX FIFO is currently full
    // 0x00000080 [7]     AUTO_CS1N    (0) When 1, automatically assert the CS1n chip select line...
    // 0x00000040 [6]     AUTO_CS0N    (0) When 1, automatically assert the CS0n chip select line...
    // 0x00000008 [3]     ASSERT_CS1N  (0) When 1, assert (i
    // 0x00000004 [2]     ASSERT_CS0N  (0) When 1, assert (i
    // 0x00000002 [1]     BUSY         (0) Direct mode busy flag
    // 0x00000001 [0]     EN           (0) Enable direct mode
    io_rw_32 direct_csr;
 
    _REG_(QMI_DIRECT_TX_OFFSET) // QMI_DIRECT_TX
    // Transmit FIFO for direct mode
    // 0x00100000 [20]    NOPUSH       (0) Inhibit the RX FIFO push that would correspond to this...
    // 0x00080000 [19]    OE           (0) Output enable (active-high)
    // 0x00040000 [18]    DWIDTH       (0) Data width
    // 0x00030000 [17:16] IWIDTH       (0x0) Configure whether this FIFO record is transferred with...
    // 0x0000ffff [15:0]  DATA         (0x0000) Data pushed here will be clocked out falling edges of...
    io_wo_32 direct_tx;
 
    _REG_(QMI_DIRECT_RX_OFFSET) // QMI_DIRECT_RX
    // Receive FIFO for direct mode
    // 0x0000ffff [15:0]  DIRECT_RX    (0x0000) With each byte clocked out on the serial interface, one...
    io_ro_32 direct_rx;
 
    qmi_mem_hw_t m[2];
 
    // (Description copied from array index 0 register QMI_ATRANS0 applies similarly to other array indexes)
    _REG_(QMI_ATRANS0_OFFSET) // QMI_ATRANS0
    // Configure address translation for XIP virtual addresses 0x000000 through 0x3fffff (a 4 MiB window starting at +0 MiB).
    // 0x07ff0000 [26:16] SIZE         (0x400) Translation aperture size for this virtual address...
    // 0x00000fff [11:0]  BASE         (0x000) Physical address base for this virtual address range, in...
    io_rw_32 atrans[8];
} qmi_hw_t;

#define qmi_hw ((qmi_hw_t *)XIP_QMI_BASE)
static_assert(sizeof (qmi_hw_t) == 0x0054, "");

#endif // _HARDWARE_STRUCTS_QMI_H

