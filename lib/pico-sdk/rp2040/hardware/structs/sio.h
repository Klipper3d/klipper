// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _HARDWARE_STRUCTS_SIO_H
#define _HARDWARE_STRUCTS_SIO_H

/**
 * \file rp2040/sio.h
 */

#include "hardware/address_mapped.h"
#include "hardware/regs/sio.h"
#include "hardware/structs/interp.h"

// Reference to datasheet: https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf#tab-registerlist_sio
//
// The _REG_ macro is intended to help make the register navigable in your IDE (for example, using the "Go to Definition" feature)
// _REG_(x) will link to the corresponding register in hardware/regs/sio.h.
//
// Bit-field descriptions are of the form:
// BITMASK [BITRANGE] FIELDNAME (RESETVALUE) DESCRIPTION


typedef struct {
    _REG_(SIO_CPUID_OFFSET) // SIO_CPUID
    // Processor core identifier
    // 0xffffffff [31:0]  CPUID        (-) Value is 0 when read from processor core 0, and 1 when...
    io_ro_32 cpuid;
 
    _REG_(SIO_GPIO_IN_OFFSET) // SIO_GPIO_IN
    // Input value for GPIO pins
    // 0x3fffffff [29:0]  GPIO_IN      (0x00000000) Input value for GPIO0
    io_ro_32 gpio_in;
 
    _REG_(SIO_GPIO_HI_IN_OFFSET) // SIO_GPIO_HI_IN
    // Input value for QSPI pins
    // 0x0000003f [5:0]   GPIO_HI_IN   (0x00) Input value on QSPI IO in order 0
    io_ro_32 gpio_hi_in;
 
    uint32_t _pad0;
 
    _REG_(SIO_GPIO_OUT_OFFSET) // SIO_GPIO_OUT
    // GPIO output value
    // 0x3fffffff [29:0]  GPIO_OUT     (0x00000000) Set output level (1/0 -> high/low) for GPIO0
    io_rw_32 gpio_out;
 
    _REG_(SIO_GPIO_OUT_SET_OFFSET) // SIO_GPIO_OUT_SET
    // GPIO output value set
    // 0x3fffffff [29:0]  GPIO_OUT_SET (0x00000000) Perform an atomic bit-set on GPIO_OUT, i
    io_wo_32 gpio_set;
 
    _REG_(SIO_GPIO_OUT_CLR_OFFSET) // SIO_GPIO_OUT_CLR
    // GPIO output value clear
    // 0x3fffffff [29:0]  GPIO_OUT_CLR (0x00000000) Perform an atomic bit-clear on GPIO_OUT, i
    io_wo_32 gpio_clr;
 
    _REG_(SIO_GPIO_OUT_XOR_OFFSET) // SIO_GPIO_OUT_XOR
    // GPIO output value XOR
    // 0x3fffffff [29:0]  GPIO_OUT_XOR (0x00000000) Perform an atomic bitwise XOR on GPIO_OUT, i
    io_wo_32 gpio_togl;
 
    _REG_(SIO_GPIO_OE_OFFSET) // SIO_GPIO_OE
    // GPIO output enable
    // 0x3fffffff [29:0]  GPIO_OE      (0x00000000) Set output enable (1/0 -> output/input) for GPIO0
    io_rw_32 gpio_oe;
 
    _REG_(SIO_GPIO_OE_SET_OFFSET) // SIO_GPIO_OE_SET
    // GPIO output enable set
    // 0x3fffffff [29:0]  GPIO_OE_SET  (0x00000000) Perform an atomic bit-set on GPIO_OE, i
    io_wo_32 gpio_oe_set;
 
    _REG_(SIO_GPIO_OE_CLR_OFFSET) // SIO_GPIO_OE_CLR
    // GPIO output enable clear
    // 0x3fffffff [29:0]  GPIO_OE_CLR  (0x00000000) Perform an atomic bit-clear on GPIO_OE, i
    io_wo_32 gpio_oe_clr;
 
    _REG_(SIO_GPIO_OE_XOR_OFFSET) // SIO_GPIO_OE_XOR
    // GPIO output enable XOR
    // 0x3fffffff [29:0]  GPIO_OE_XOR  (0x00000000) Perform an atomic bitwise XOR on GPIO_OE, i
    io_wo_32 gpio_oe_togl;
 
    _REG_(SIO_GPIO_HI_OUT_OFFSET) // SIO_GPIO_HI_OUT
    // QSPI output value
    // 0x0000003f [5:0]   GPIO_HI_OUT  (0x00) Set output level (1/0 -> high/low) for QSPI IO0
    io_rw_32 gpio_hi_out;
 
    _REG_(SIO_GPIO_HI_OUT_SET_OFFSET) // SIO_GPIO_HI_OUT_SET
    // QSPI output value set
    // 0x0000003f [5:0]   GPIO_HI_OUT_SET (0x00) Perform an atomic bit-set on GPIO_HI_OUT, i
    io_wo_32 gpio_hi_set;
 
    _REG_(SIO_GPIO_HI_OUT_CLR_OFFSET) // SIO_GPIO_HI_OUT_CLR
    // QSPI output value clear
    // 0x0000003f [5:0]   GPIO_HI_OUT_CLR (0x00) Perform an atomic bit-clear on GPIO_HI_OUT, i
    io_wo_32 gpio_hi_clr;
 
    _REG_(SIO_GPIO_HI_OUT_XOR_OFFSET) // SIO_GPIO_HI_OUT_XOR
    // QSPI output value XOR
    // 0x0000003f [5:0]   GPIO_HI_OUT_XOR (0x00) Perform an atomic bitwise XOR on GPIO_HI_OUT, i
    io_wo_32 gpio_hi_togl;
 
    _REG_(SIO_GPIO_HI_OE_OFFSET) // SIO_GPIO_HI_OE
    // QSPI output enable
    // 0x0000003f [5:0]   GPIO_HI_OE   (0x00) Set output enable (1/0 -> output/input) for QSPI IO0
    io_rw_32 gpio_hi_oe;
 
    _REG_(SIO_GPIO_HI_OE_SET_OFFSET) // SIO_GPIO_HI_OE_SET
    // QSPI output enable set
    // 0x0000003f [5:0]   GPIO_HI_OE_SET (0x00) Perform an atomic bit-set on GPIO_HI_OE, i
    io_wo_32 gpio_hi_oe_set;
 
    _REG_(SIO_GPIO_HI_OE_CLR_OFFSET) // SIO_GPIO_HI_OE_CLR
    // QSPI output enable clear
    // 0x0000003f [5:0]   GPIO_HI_OE_CLR (0x00) Perform an atomic bit-clear on GPIO_HI_OE, i
    io_wo_32 gpio_hi_oe_clr;
 
    _REG_(SIO_GPIO_HI_OE_XOR_OFFSET) // SIO_GPIO_HI_OE_XOR
    // QSPI output enable XOR
    // 0x0000003f [5:0]   GPIO_HI_OE_XOR (0x00) Perform an atomic bitwise XOR on GPIO_HI_OE, i
    io_wo_32 gpio_hi_oe_togl;
 
    _REG_(SIO_FIFO_ST_OFFSET) // SIO_FIFO_ST
    // Status register for inter-core FIFOs (mailboxes).
    // 0x00000008 [3]     ROE          (0) Sticky flag indicating the RX FIFO was read when empty
    // 0x00000004 [2]     WOF          (0) Sticky flag indicating the TX FIFO was written when full
    // 0x00000002 [1]     RDY          (1) Value is 1 if this core's TX FIFO is not full (i
    // 0x00000001 [0]     VLD          (0) Value is 1 if this core's RX FIFO is not empty (i
    io_rw_32 fifo_st;
 
    _REG_(SIO_FIFO_WR_OFFSET) // SIO_FIFO_WR
    // Write access to this core's TX FIFO
    // 0xffffffff [31:0]  FIFO_WR      (0x00000000) 
    io_wo_32 fifo_wr;
 
    _REG_(SIO_FIFO_RD_OFFSET) // SIO_FIFO_RD
    // Read access to this core's RX FIFO
    // 0xffffffff [31:0]  FIFO_RD      (-) 
    io_ro_32 fifo_rd;
 
    _REG_(SIO_SPINLOCK_ST_OFFSET) // SIO_SPINLOCK_ST
    // Spinlock state
    // 0xffffffff [31:0]  SPINLOCK_ST  (0x00000000) 
    io_ro_32 spinlock_st;
 
    _REG_(SIO_DIV_UDIVIDEND_OFFSET) // SIO_DIV_UDIVIDEND
    // Divider unsigned dividend
    // 0xffffffff [31:0]  DIV_UDIVIDEND (0x00000000) 
    io_rw_32 div_udividend;
 
    _REG_(SIO_DIV_UDIVISOR_OFFSET) // SIO_DIV_UDIVISOR
    // Divider unsigned divisor
    // 0xffffffff [31:0]  DIV_UDIVISOR (0x00000000) 
    io_rw_32 div_udivisor;
 
    _REG_(SIO_DIV_SDIVIDEND_OFFSET) // SIO_DIV_SDIVIDEND
    // Divider signed dividend
    // 0xffffffff [31:0]  DIV_SDIVIDEND (0x00000000) 
    io_rw_32 div_sdividend;
 
    _REG_(SIO_DIV_SDIVISOR_OFFSET) // SIO_DIV_SDIVISOR
    // Divider signed divisor
    // 0xffffffff [31:0]  DIV_SDIVISOR (0x00000000) 
    io_rw_32 div_sdivisor;
 
    _REG_(SIO_DIV_QUOTIENT_OFFSET) // SIO_DIV_QUOTIENT
    // Divider result quotient
    // 0xffffffff [31:0]  DIV_QUOTIENT (0x00000000) 
    io_rw_32 div_quotient;
 
    _REG_(SIO_DIV_REMAINDER_OFFSET) // SIO_DIV_REMAINDER
    // Divider result remainder
    // 0xffffffff [31:0]  DIV_REMAINDER (0x00000000) 
    io_rw_32 div_remainder;
 
    _REG_(SIO_DIV_CSR_OFFSET) // SIO_DIV_CSR
    // Control and status register for divider
    // 0x00000002 [1]     DIRTY        (0) Changes to 1 when any register is written, and back to 0...
    // 0x00000001 [0]     READY        (1) Reads as 0 when a calculation is in progress, 1 otherwise
    io_ro_32 div_csr;
 
    uint32_t _pad1;
 
    interp_hw_t interp[2];
 
    // (Description copied from array index 0 register SIO_SPINLOCK0 applies similarly to other array indexes)
    _REG_(SIO_SPINLOCK0_OFFSET) // SIO_SPINLOCK0
    // Spinlock register 0
    // 0xffffffff [31:0]  SPINLOCK0    (0x00000000) 
    io_rw_32 spinlock[32];
} sio_hw_t;

#define sio_hw ((sio_hw_t *)SIO_BASE)
static_assert(sizeof (sio_hw_t) == 0x0180, "");

#endif // _HARDWARE_STRUCTS_SIO_H

