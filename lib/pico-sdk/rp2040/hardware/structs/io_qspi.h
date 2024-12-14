// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _HARDWARE_STRUCTS_IO_QSPI_H
#define _HARDWARE_STRUCTS_IO_QSPI_H

/**
 * \file rp2040/io_qspi.h
 */

#include "hardware/address_mapped.h"
#include "hardware/regs/io_qspi.h"

// Reference to datasheet: https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf#tab-registerlist_io_qspi
//
// The _REG_ macro is intended to help make the register navigable in your IDE (for example, using the "Go to Definition" feature)
// _REG_(x) will link to the corresponding register in hardware/regs/io_qspi.h.
//
// Bit-field descriptions are of the form:
// BITMASK [BITRANGE] FIELDNAME (RESETVALUE) DESCRIPTION

/**
 * \brief QSPI pin function selectors on RP2040 (used as typedef \ref gpio_function1_t) 
 */
typedef enum gpio_function1_rp2040 {
    GPIO_FUNC1_XIP = 0, ///< Select XIP as QSPI pin function
    GPIO_FUNC1_SIO = 5, ///< Select SIO as QSPI pin function
    GPIO_FUNC1_NULL = 0x1f, ///< Select NULL as QSPI pin function
} gpio_function1_t;

typedef struct {
    _REG_(IO_QSPI_GPIO_QSPI_SCLK_STATUS_OFFSET) // IO_QSPI_GPIO_QSPI_SCLK_STATUS
    // GPIO status
    // 0x04000000 [26]    IRQTOPROC    (0) interrupt to processors, after override is applied
    // 0x01000000 [24]    IRQFROMPAD   (0) interrupt from pad before override is applied
    // 0x00080000 [19]    INTOPERI     (0) input signal to peripheral, after override is applied
    // 0x00020000 [17]    INFROMPAD    (0) input signal from pad, before override is applied
    // 0x00002000 [13]    OETOPAD      (0) output enable to pad after register override is applied
    // 0x00001000 [12]    OEFROMPERI   (0) output enable from selected peripheral, before register...
    // 0x00000200 [9]     OUTTOPAD     (0) output signal to pad after register override is applied
    // 0x00000100 [8]     OUTFROMPERI  (0) output signal from selected peripheral, before register...
    io_ro_32 status;
 
    _REG_(IO_QSPI_GPIO_QSPI_SCLK_CTRL_OFFSET) // IO_QSPI_GPIO_QSPI_SCLK_CTRL
    // GPIO control including function select and overrides
    // 0x30000000 [29:28] IRQOVER      (0x0) 
    // 0x00030000 [17:16] INOVER       (0x0) 
    // 0x00003000 [13:12] OEOVER       (0x0) 
    // 0x00000300 [9:8]   OUTOVER      (0x0) 
    // 0x0000001f [4:0]   FUNCSEL      (0x1f) 0-31 -> selects pin function according to the gpio table +
    io_rw_32 ctrl;
} io_qspi_status_ctrl_hw_t;

typedef struct {
    _REG_(IO_QSPI_PROC0_INTE_OFFSET) // IO_QSPI_PROC0_INTE
    // Interrupt Enable for proc0
    // 0x00800000 [23]    GPIO_QSPI_SD3_EDGE_HIGH (0) 
    // 0x00400000 [22]    GPIO_QSPI_SD3_EDGE_LOW (0) 
    // 0x00200000 [21]    GPIO_QSPI_SD3_LEVEL_HIGH (0) 
    // 0x00100000 [20]    GPIO_QSPI_SD3_LEVEL_LOW (0) 
    // 0x00080000 [19]    GPIO_QSPI_SD2_EDGE_HIGH (0) 
    // 0x00040000 [18]    GPIO_QSPI_SD2_EDGE_LOW (0) 
    // 0x00020000 [17]    GPIO_QSPI_SD2_LEVEL_HIGH (0) 
    // 0x00010000 [16]    GPIO_QSPI_SD2_LEVEL_LOW (0) 
    // 0x00008000 [15]    GPIO_QSPI_SD1_EDGE_HIGH (0) 
    // 0x00004000 [14]    GPIO_QSPI_SD1_EDGE_LOW (0) 
    // 0x00002000 [13]    GPIO_QSPI_SD1_LEVEL_HIGH (0) 
    // 0x00001000 [12]    GPIO_QSPI_SD1_LEVEL_LOW (0) 
    // 0x00000800 [11]    GPIO_QSPI_SD0_EDGE_HIGH (0) 
    // 0x00000400 [10]    GPIO_QSPI_SD0_EDGE_LOW (0) 
    // 0x00000200 [9]     GPIO_QSPI_SD0_LEVEL_HIGH (0) 
    // 0x00000100 [8]     GPIO_QSPI_SD0_LEVEL_LOW (0) 
    // 0x00000080 [7]     GPIO_QSPI_SS_EDGE_HIGH (0) 
    // 0x00000040 [6]     GPIO_QSPI_SS_EDGE_LOW (0) 
    // 0x00000020 [5]     GPIO_QSPI_SS_LEVEL_HIGH (0) 
    // 0x00000010 [4]     GPIO_QSPI_SS_LEVEL_LOW (0) 
    // 0x00000008 [3]     GPIO_QSPI_SCLK_EDGE_HIGH (0) 
    // 0x00000004 [2]     GPIO_QSPI_SCLK_EDGE_LOW (0) 
    // 0x00000002 [1]     GPIO_QSPI_SCLK_LEVEL_HIGH (0) 
    // 0x00000001 [0]     GPIO_QSPI_SCLK_LEVEL_LOW (0) 
    io_rw_32 inte;
 
    _REG_(IO_QSPI_PROC0_INTF_OFFSET) // IO_QSPI_PROC0_INTF
    // Interrupt Force for proc0
    // 0x00800000 [23]    GPIO_QSPI_SD3_EDGE_HIGH (0) 
    // 0x00400000 [22]    GPIO_QSPI_SD3_EDGE_LOW (0) 
    // 0x00200000 [21]    GPIO_QSPI_SD3_LEVEL_HIGH (0) 
    // 0x00100000 [20]    GPIO_QSPI_SD3_LEVEL_LOW (0) 
    // 0x00080000 [19]    GPIO_QSPI_SD2_EDGE_HIGH (0) 
    // 0x00040000 [18]    GPIO_QSPI_SD2_EDGE_LOW (0) 
    // 0x00020000 [17]    GPIO_QSPI_SD2_LEVEL_HIGH (0) 
    // 0x00010000 [16]    GPIO_QSPI_SD2_LEVEL_LOW (0) 
    // 0x00008000 [15]    GPIO_QSPI_SD1_EDGE_HIGH (0) 
    // 0x00004000 [14]    GPIO_QSPI_SD1_EDGE_LOW (0) 
    // 0x00002000 [13]    GPIO_QSPI_SD1_LEVEL_HIGH (0) 
    // 0x00001000 [12]    GPIO_QSPI_SD1_LEVEL_LOW (0) 
    // 0x00000800 [11]    GPIO_QSPI_SD0_EDGE_HIGH (0) 
    // 0x00000400 [10]    GPIO_QSPI_SD0_EDGE_LOW (0) 
    // 0x00000200 [9]     GPIO_QSPI_SD0_LEVEL_HIGH (0) 
    // 0x00000100 [8]     GPIO_QSPI_SD0_LEVEL_LOW (0) 
    // 0x00000080 [7]     GPIO_QSPI_SS_EDGE_HIGH (0) 
    // 0x00000040 [6]     GPIO_QSPI_SS_EDGE_LOW (0) 
    // 0x00000020 [5]     GPIO_QSPI_SS_LEVEL_HIGH (0) 
    // 0x00000010 [4]     GPIO_QSPI_SS_LEVEL_LOW (0) 
    // 0x00000008 [3]     GPIO_QSPI_SCLK_EDGE_HIGH (0) 
    // 0x00000004 [2]     GPIO_QSPI_SCLK_EDGE_LOW (0) 
    // 0x00000002 [1]     GPIO_QSPI_SCLK_LEVEL_HIGH (0) 
    // 0x00000001 [0]     GPIO_QSPI_SCLK_LEVEL_LOW (0) 
    io_rw_32 intf;
 
    _REG_(IO_QSPI_PROC0_INTS_OFFSET) // IO_QSPI_PROC0_INTS
    // Interrupt status after masking & forcing for proc0
    // 0x00800000 [23]    GPIO_QSPI_SD3_EDGE_HIGH (0) 
    // 0x00400000 [22]    GPIO_QSPI_SD3_EDGE_LOW (0) 
    // 0x00200000 [21]    GPIO_QSPI_SD3_LEVEL_HIGH (0) 
    // 0x00100000 [20]    GPIO_QSPI_SD3_LEVEL_LOW (0) 
    // 0x00080000 [19]    GPIO_QSPI_SD2_EDGE_HIGH (0) 
    // 0x00040000 [18]    GPIO_QSPI_SD2_EDGE_LOW (0) 
    // 0x00020000 [17]    GPIO_QSPI_SD2_LEVEL_HIGH (0) 
    // 0x00010000 [16]    GPIO_QSPI_SD2_LEVEL_LOW (0) 
    // 0x00008000 [15]    GPIO_QSPI_SD1_EDGE_HIGH (0) 
    // 0x00004000 [14]    GPIO_QSPI_SD1_EDGE_LOW (0) 
    // 0x00002000 [13]    GPIO_QSPI_SD1_LEVEL_HIGH (0) 
    // 0x00001000 [12]    GPIO_QSPI_SD1_LEVEL_LOW (0) 
    // 0x00000800 [11]    GPIO_QSPI_SD0_EDGE_HIGH (0) 
    // 0x00000400 [10]    GPIO_QSPI_SD0_EDGE_LOW (0) 
    // 0x00000200 [9]     GPIO_QSPI_SD0_LEVEL_HIGH (0) 
    // 0x00000100 [8]     GPIO_QSPI_SD0_LEVEL_LOW (0) 
    // 0x00000080 [7]     GPIO_QSPI_SS_EDGE_HIGH (0) 
    // 0x00000040 [6]     GPIO_QSPI_SS_EDGE_LOW (0) 
    // 0x00000020 [5]     GPIO_QSPI_SS_LEVEL_HIGH (0) 
    // 0x00000010 [4]     GPIO_QSPI_SS_LEVEL_LOW (0) 
    // 0x00000008 [3]     GPIO_QSPI_SCLK_EDGE_HIGH (0) 
    // 0x00000004 [2]     GPIO_QSPI_SCLK_EDGE_LOW (0) 
    // 0x00000002 [1]     GPIO_QSPI_SCLK_LEVEL_HIGH (0) 
    // 0x00000001 [0]     GPIO_QSPI_SCLK_LEVEL_LOW (0) 
    io_ro_32 ints;
} io_qspi_irq_ctrl_hw_t;

typedef struct {
    io_qspi_status_ctrl_hw_t io[6];
 
    _REG_(IO_QSPI_INTR_OFFSET) // IO_QSPI_INTR
    // Raw Interrupts
    // 0x00800000 [23]    GPIO_QSPI_SD3_EDGE_HIGH (0) 
    // 0x00400000 [22]    GPIO_QSPI_SD3_EDGE_LOW (0) 
    // 0x00200000 [21]    GPIO_QSPI_SD3_LEVEL_HIGH (0) 
    // 0x00100000 [20]    GPIO_QSPI_SD3_LEVEL_LOW (0) 
    // 0x00080000 [19]    GPIO_QSPI_SD2_EDGE_HIGH (0) 
    // 0x00040000 [18]    GPIO_QSPI_SD2_EDGE_LOW (0) 
    // 0x00020000 [17]    GPIO_QSPI_SD2_LEVEL_HIGH (0) 
    // 0x00010000 [16]    GPIO_QSPI_SD2_LEVEL_LOW (0) 
    // 0x00008000 [15]    GPIO_QSPI_SD1_EDGE_HIGH (0) 
    // 0x00004000 [14]    GPIO_QSPI_SD1_EDGE_LOW (0) 
    // 0x00002000 [13]    GPIO_QSPI_SD1_LEVEL_HIGH (0) 
    // 0x00001000 [12]    GPIO_QSPI_SD1_LEVEL_LOW (0) 
    // 0x00000800 [11]    GPIO_QSPI_SD0_EDGE_HIGH (0) 
    // 0x00000400 [10]    GPIO_QSPI_SD0_EDGE_LOW (0) 
    // 0x00000200 [9]     GPIO_QSPI_SD0_LEVEL_HIGH (0) 
    // 0x00000100 [8]     GPIO_QSPI_SD0_LEVEL_LOW (0) 
    // 0x00000080 [7]     GPIO_QSPI_SS_EDGE_HIGH (0) 
    // 0x00000040 [6]     GPIO_QSPI_SS_EDGE_LOW (0) 
    // 0x00000020 [5]     GPIO_QSPI_SS_LEVEL_HIGH (0) 
    // 0x00000010 [4]     GPIO_QSPI_SS_LEVEL_LOW (0) 
    // 0x00000008 [3]     GPIO_QSPI_SCLK_EDGE_HIGH (0) 
    // 0x00000004 [2]     GPIO_QSPI_SCLK_EDGE_LOW (0) 
    // 0x00000002 [1]     GPIO_QSPI_SCLK_LEVEL_HIGH (0) 
    // 0x00000001 [0]     GPIO_QSPI_SCLK_LEVEL_LOW (0) 
    io_rw_32 intr;
 
    union {
        struct {
            io_qspi_irq_ctrl_hw_t proc0_irq_ctrl;
            io_qspi_irq_ctrl_hw_t proc1_irq_ctrl;
            io_qspi_irq_ctrl_hw_t dormant_wake_irq_ctrl;
        };
        io_qspi_irq_ctrl_hw_t irq_ctrl[3];
    };
} io_qspi_hw_t;

#define io_qspi_hw ((io_qspi_hw_t *)IO_QSPI_BASE)
static_assert(sizeof (io_qspi_hw_t) == 0x0058, "");

#endif // _HARDWARE_STRUCTS_IO_QSPI_H

