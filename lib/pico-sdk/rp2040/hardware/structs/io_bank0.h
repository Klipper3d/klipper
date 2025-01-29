// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _HARDWARE_STRUCTS_IO_BANK0_H
#define _HARDWARE_STRUCTS_IO_BANK0_H

/**
 * \file rp2040/io_bank0.h
 */

#include "hardware/address_mapped.h"
#include "hardware/regs/io_bank0.h"

// Reference to datasheet: https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf#tab-registerlist_io_bank0
//
// The _REG_ macro is intended to help make the register navigable in your IDE (for example, using the "Go to Definition" feature)
// _REG_(x) will link to the corresponding register in hardware/regs/io_bank0.h.
//
// Bit-field descriptions are of the form:
// BITMASK [BITRANGE] FIELDNAME (RESETVALUE) DESCRIPTION

/**
 * \brief GPIO pin function selectors on RP2040 (used as typedef \ref gpio_function_t) 
 * \ingroup hardware_gpio
 */
typedef enum gpio_function_rp2040 {
    GPIO_FUNC_XIP = 0, ///< Select XIP as GPIO pin function
    GPIO_FUNC_SPI = 1, ///< Select SPI as GPIO pin function
    GPIO_FUNC_UART = 2, ///< Select UART as GPIO pin function
    GPIO_FUNC_I2C = 3, ///< Select I2C as GPIO pin function
    GPIO_FUNC_PWM = 4, ///< Select PWM as GPIO pin function
    GPIO_FUNC_SIO = 5, ///< Select SIO as GPIO pin function
    GPIO_FUNC_PIO0 = 6, ///< Select PIO0 as GPIO pin function
    GPIO_FUNC_PIO1 = 7, ///< Select PIO1 as GPIO pin function
    GPIO_FUNC_GPCK = 8, ///< Select GPCK as GPIO pin function
    GPIO_FUNC_USB = 9, ///< Select USB as GPIO pin function
    GPIO_FUNC_NULL = 0x1f, ///< Select NULL as GPIO pin function
} gpio_function_t;

typedef struct {
    _REG_(IO_BANK0_GPIO0_STATUS_OFFSET) // IO_BANK0_GPIO0_STATUS
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
 
    _REG_(IO_BANK0_GPIO0_CTRL_OFFSET) // IO_BANK0_GPIO0_CTRL
    // GPIO control including function select and overrides
    // 0x30000000 [29:28] IRQOVER      (0x0) 
    // 0x00030000 [17:16] INOVER       (0x0) 
    // 0x00003000 [13:12] OEOVER       (0x0) 
    // 0x00000300 [9:8]   OUTOVER      (0x0) 
    // 0x0000001f [4:0]   FUNCSEL      (0x1f) 0-31 -> selects pin function according to the gpio table +
    io_rw_32 ctrl;
} io_bank0_status_ctrl_hw_t;

typedef struct {
    // (Description copied from array index 0 register IO_BANK0_PROC0_INTE0 applies similarly to other array indexes)
    _REG_(IO_BANK0_PROC0_INTE0_OFFSET) // IO_BANK0_PROC0_INTE0
    // Interrupt Enable for proc0
    // 0x80000000 [31]    GPIO7_EDGE_HIGH (0) 
    // 0x40000000 [30]    GPIO7_EDGE_LOW (0) 
    // 0x20000000 [29]    GPIO7_LEVEL_HIGH (0) 
    // 0x10000000 [28]    GPIO7_LEVEL_LOW (0) 
    // 0x08000000 [27]    GPIO6_EDGE_HIGH (0) 
    // 0x04000000 [26]    GPIO6_EDGE_LOW (0) 
    // 0x02000000 [25]    GPIO6_LEVEL_HIGH (0) 
    // 0x01000000 [24]    GPIO6_LEVEL_LOW (0) 
    // 0x00800000 [23]    GPIO5_EDGE_HIGH (0) 
    // 0x00400000 [22]    GPIO5_EDGE_LOW (0) 
    // 0x00200000 [21]    GPIO5_LEVEL_HIGH (0) 
    // 0x00100000 [20]    GPIO5_LEVEL_LOW (0) 
    // 0x00080000 [19]    GPIO4_EDGE_HIGH (0) 
    // 0x00040000 [18]    GPIO4_EDGE_LOW (0) 
    // 0x00020000 [17]    GPIO4_LEVEL_HIGH (0) 
    // 0x00010000 [16]    GPIO4_LEVEL_LOW (0) 
    // 0x00008000 [15]    GPIO3_EDGE_HIGH (0) 
    // 0x00004000 [14]    GPIO3_EDGE_LOW (0) 
    // 0x00002000 [13]    GPIO3_LEVEL_HIGH (0) 
    // 0x00001000 [12]    GPIO3_LEVEL_LOW (0) 
    // 0x00000800 [11]    GPIO2_EDGE_HIGH (0) 
    // 0x00000400 [10]    GPIO2_EDGE_LOW (0) 
    // 0x00000200 [9]     GPIO2_LEVEL_HIGH (0) 
    // 0x00000100 [8]     GPIO2_LEVEL_LOW (0) 
    // 0x00000080 [7]     GPIO1_EDGE_HIGH (0) 
    // 0x00000040 [6]     GPIO1_EDGE_LOW (0) 
    // 0x00000020 [5]     GPIO1_LEVEL_HIGH (0) 
    // 0x00000010 [4]     GPIO1_LEVEL_LOW (0) 
    // 0x00000008 [3]     GPIO0_EDGE_HIGH (0) 
    // 0x00000004 [2]     GPIO0_EDGE_LOW (0) 
    // 0x00000002 [1]     GPIO0_LEVEL_HIGH (0) 
    // 0x00000001 [0]     GPIO0_LEVEL_LOW (0) 
    io_rw_32 inte[4];
 
    // (Description copied from array index 0 register IO_BANK0_PROC0_INTF0 applies similarly to other array indexes)
    _REG_(IO_BANK0_PROC0_INTF0_OFFSET) // IO_BANK0_PROC0_INTF0
    // Interrupt Force for proc0
    // 0x80000000 [31]    GPIO7_EDGE_HIGH (0) 
    // 0x40000000 [30]    GPIO7_EDGE_LOW (0) 
    // 0x20000000 [29]    GPIO7_LEVEL_HIGH (0) 
    // 0x10000000 [28]    GPIO7_LEVEL_LOW (0) 
    // 0x08000000 [27]    GPIO6_EDGE_HIGH (0) 
    // 0x04000000 [26]    GPIO6_EDGE_LOW (0) 
    // 0x02000000 [25]    GPIO6_LEVEL_HIGH (0) 
    // 0x01000000 [24]    GPIO6_LEVEL_LOW (0) 
    // 0x00800000 [23]    GPIO5_EDGE_HIGH (0) 
    // 0x00400000 [22]    GPIO5_EDGE_LOW (0) 
    // 0x00200000 [21]    GPIO5_LEVEL_HIGH (0) 
    // 0x00100000 [20]    GPIO5_LEVEL_LOW (0) 
    // 0x00080000 [19]    GPIO4_EDGE_HIGH (0) 
    // 0x00040000 [18]    GPIO4_EDGE_LOW (0) 
    // 0x00020000 [17]    GPIO4_LEVEL_HIGH (0) 
    // 0x00010000 [16]    GPIO4_LEVEL_LOW (0) 
    // 0x00008000 [15]    GPIO3_EDGE_HIGH (0) 
    // 0x00004000 [14]    GPIO3_EDGE_LOW (0) 
    // 0x00002000 [13]    GPIO3_LEVEL_HIGH (0) 
    // 0x00001000 [12]    GPIO3_LEVEL_LOW (0) 
    // 0x00000800 [11]    GPIO2_EDGE_HIGH (0) 
    // 0x00000400 [10]    GPIO2_EDGE_LOW (0) 
    // 0x00000200 [9]     GPIO2_LEVEL_HIGH (0) 
    // 0x00000100 [8]     GPIO2_LEVEL_LOW (0) 
    // 0x00000080 [7]     GPIO1_EDGE_HIGH (0) 
    // 0x00000040 [6]     GPIO1_EDGE_LOW (0) 
    // 0x00000020 [5]     GPIO1_LEVEL_HIGH (0) 
    // 0x00000010 [4]     GPIO1_LEVEL_LOW (0) 
    // 0x00000008 [3]     GPIO0_EDGE_HIGH (0) 
    // 0x00000004 [2]     GPIO0_EDGE_LOW (0) 
    // 0x00000002 [1]     GPIO0_LEVEL_HIGH (0) 
    // 0x00000001 [0]     GPIO0_LEVEL_LOW (0) 
    io_rw_32 intf[4];
 
    // (Description copied from array index 0 register IO_BANK0_PROC0_INTS0 applies similarly to other array indexes)
    _REG_(IO_BANK0_PROC0_INTS0_OFFSET) // IO_BANK0_PROC0_INTS0
    // Interrupt status after masking & forcing for proc0
    // 0x80000000 [31]    GPIO7_EDGE_HIGH (0) 
    // 0x40000000 [30]    GPIO7_EDGE_LOW (0) 
    // 0x20000000 [29]    GPIO7_LEVEL_HIGH (0) 
    // 0x10000000 [28]    GPIO7_LEVEL_LOW (0) 
    // 0x08000000 [27]    GPIO6_EDGE_HIGH (0) 
    // 0x04000000 [26]    GPIO6_EDGE_LOW (0) 
    // 0x02000000 [25]    GPIO6_LEVEL_HIGH (0) 
    // 0x01000000 [24]    GPIO6_LEVEL_LOW (0) 
    // 0x00800000 [23]    GPIO5_EDGE_HIGH (0) 
    // 0x00400000 [22]    GPIO5_EDGE_LOW (0) 
    // 0x00200000 [21]    GPIO5_LEVEL_HIGH (0) 
    // 0x00100000 [20]    GPIO5_LEVEL_LOW (0) 
    // 0x00080000 [19]    GPIO4_EDGE_HIGH (0) 
    // 0x00040000 [18]    GPIO4_EDGE_LOW (0) 
    // 0x00020000 [17]    GPIO4_LEVEL_HIGH (0) 
    // 0x00010000 [16]    GPIO4_LEVEL_LOW (0) 
    // 0x00008000 [15]    GPIO3_EDGE_HIGH (0) 
    // 0x00004000 [14]    GPIO3_EDGE_LOW (0) 
    // 0x00002000 [13]    GPIO3_LEVEL_HIGH (0) 
    // 0x00001000 [12]    GPIO3_LEVEL_LOW (0) 
    // 0x00000800 [11]    GPIO2_EDGE_HIGH (0) 
    // 0x00000400 [10]    GPIO2_EDGE_LOW (0) 
    // 0x00000200 [9]     GPIO2_LEVEL_HIGH (0) 
    // 0x00000100 [8]     GPIO2_LEVEL_LOW (0) 
    // 0x00000080 [7]     GPIO1_EDGE_HIGH (0) 
    // 0x00000040 [6]     GPIO1_EDGE_LOW (0) 
    // 0x00000020 [5]     GPIO1_LEVEL_HIGH (0) 
    // 0x00000010 [4]     GPIO1_LEVEL_LOW (0) 
    // 0x00000008 [3]     GPIO0_EDGE_HIGH (0) 
    // 0x00000004 [2]     GPIO0_EDGE_LOW (0) 
    // 0x00000002 [1]     GPIO0_LEVEL_HIGH (0) 
    // 0x00000001 [0]     GPIO0_LEVEL_LOW (0) 
    io_ro_32 ints[4];
} io_bank0_irq_ctrl_hw_t;

/// \tag::io_bank0_hw[]
typedef struct {
    io_bank0_status_ctrl_hw_t io[30];
 
    // (Description copied from array index 0 register IO_BANK0_INTR0 applies similarly to other array indexes)
    _REG_(IO_BANK0_INTR0_OFFSET) // IO_BANK0_INTR0
    // Raw Interrupts
    // 0x80000000 [31]    GPIO7_EDGE_HIGH (0) 
    // 0x40000000 [30]    GPIO7_EDGE_LOW (0) 
    // 0x20000000 [29]    GPIO7_LEVEL_HIGH (0) 
    // 0x10000000 [28]    GPIO7_LEVEL_LOW (0) 
    // 0x08000000 [27]    GPIO6_EDGE_HIGH (0) 
    // 0x04000000 [26]    GPIO6_EDGE_LOW (0) 
    // 0x02000000 [25]    GPIO6_LEVEL_HIGH (0) 
    // 0x01000000 [24]    GPIO6_LEVEL_LOW (0) 
    // 0x00800000 [23]    GPIO5_EDGE_HIGH (0) 
    // 0x00400000 [22]    GPIO5_EDGE_LOW (0) 
    // 0x00200000 [21]    GPIO5_LEVEL_HIGH (0) 
    // 0x00100000 [20]    GPIO5_LEVEL_LOW (0) 
    // 0x00080000 [19]    GPIO4_EDGE_HIGH (0) 
    // 0x00040000 [18]    GPIO4_EDGE_LOW (0) 
    // 0x00020000 [17]    GPIO4_LEVEL_HIGH (0) 
    // 0x00010000 [16]    GPIO4_LEVEL_LOW (0) 
    // 0x00008000 [15]    GPIO3_EDGE_HIGH (0) 
    // 0x00004000 [14]    GPIO3_EDGE_LOW (0) 
    // 0x00002000 [13]    GPIO3_LEVEL_HIGH (0) 
    // 0x00001000 [12]    GPIO3_LEVEL_LOW (0) 
    // 0x00000800 [11]    GPIO2_EDGE_HIGH (0) 
    // 0x00000400 [10]    GPIO2_EDGE_LOW (0) 
    // 0x00000200 [9]     GPIO2_LEVEL_HIGH (0) 
    // 0x00000100 [8]     GPIO2_LEVEL_LOW (0) 
    // 0x00000080 [7]     GPIO1_EDGE_HIGH (0) 
    // 0x00000040 [6]     GPIO1_EDGE_LOW (0) 
    // 0x00000020 [5]     GPIO1_LEVEL_HIGH (0) 
    // 0x00000010 [4]     GPIO1_LEVEL_LOW (0) 
    // 0x00000008 [3]     GPIO0_EDGE_HIGH (0) 
    // 0x00000004 [2]     GPIO0_EDGE_LOW (0) 
    // 0x00000002 [1]     GPIO0_LEVEL_HIGH (0) 
    // 0x00000001 [0]     GPIO0_LEVEL_LOW (0) 
    io_rw_32 intr[4];
 
    union {
        struct {
            io_bank0_irq_ctrl_hw_t proc0_irq_ctrl;
            io_bank0_irq_ctrl_hw_t proc1_irq_ctrl;
            io_bank0_irq_ctrl_hw_t dormant_wake_irq_ctrl;
        };
        io_bank0_irq_ctrl_hw_t irq_ctrl[3];
    };
} io_bank0_hw_t;
/// \end::io_bank0_hw[]

#define io_bank0_hw ((io_bank0_hw_t *)IO_BANK0_BASE)
static_assert(sizeof (io_bank0_hw_t) == 0x0190, "");

#endif // _HARDWARE_STRUCTS_IO_BANK0_H

