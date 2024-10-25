// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _HARDWARE_STRUCTS_PWM_H
#define _HARDWARE_STRUCTS_PWM_H

/**
 * \file rp2350/pwm.h
 */

#include "hardware/address_mapped.h"
#include "hardware/regs/pwm.h"

// Reference to datasheet: https://datasheets.raspberrypi.com/rp2350/rp2350-datasheet.pdf#tab-registerlist_pwm
//
// The _REG_ macro is intended to help make the register navigable in your IDE (for example, using the "Go to Definition" feature)
// _REG_(x) will link to the corresponding register in hardware/regs/pwm.h.
//
// Bit-field descriptions are of the form:
// BITMASK [BITRANGE] FIELDNAME (RESETVALUE) DESCRIPTION

typedef struct {
    _REG_(PWM_CH0_CSR_OFFSET) // PWM_CH0_CSR
    // Control and status register
    // 0x00000080 [7]     PH_ADV       (0) Advance the phase of the counter by 1 count, while it is running
    // 0x00000040 [6]     PH_RET       (0) Retard the phase of the counter by 1 count, while it is running
    // 0x00000030 [5:4]   DIVMODE      (0x0) 
    // 0x00000008 [3]     B_INV        (0) Invert output B
    // 0x00000004 [2]     A_INV        (0) Invert output A
    // 0x00000002 [1]     PH_CORRECT   (0) 1: Enable phase-correct modulation
    // 0x00000001 [0]     EN           (0) Enable the PWM channel
    io_rw_32 csr;
 
    _REG_(PWM_CH0_DIV_OFFSET) // PWM_CH0_DIV
    // INT and FRAC form a fixed-point fractional number
    // 0x00000ff0 [11:4]  INT          (0x01) 
    // 0x0000000f [3:0]   FRAC         (0x0) 
    io_rw_32 div;
 
    _REG_(PWM_CH0_CTR_OFFSET) // PWM_CH0_CTR
    // Direct access to the PWM counter
    // 0x0000ffff [15:0]  CH0_CTR      (0x0000) 
    io_rw_32 ctr;
 
    _REG_(PWM_CH0_CC_OFFSET) // PWM_CH0_CC
    // Counter compare values
    // 0xffff0000 [31:16] B            (0x0000) 
    // 0x0000ffff [15:0]  A            (0x0000) 
    io_rw_32 cc;
 
    _REG_(PWM_CH0_TOP_OFFSET) // PWM_CH0_TOP
    // Counter wrap value
    // 0x0000ffff [15:0]  CH0_TOP      (0xffff) 
    io_rw_32 top;
} pwm_slice_hw_t;

typedef struct {
    _REG_(PWM_IRQ0_INTE_OFFSET) // PWM_IRQ0_INTE
    // Interrupt Enable for irq0
    // 0x00000800 [11]    CH11         (0) 
    // 0x00000400 [10]    CH10         (0) 
    // 0x00000200 [9]     CH9          (0) 
    // 0x00000100 [8]     CH8          (0) 
    // 0x00000080 [7]     CH7          (0) 
    // 0x00000040 [6]     CH6          (0) 
    // 0x00000020 [5]     CH5          (0) 
    // 0x00000010 [4]     CH4          (0) 
    // 0x00000008 [3]     CH3          (0) 
    // 0x00000004 [2]     CH2          (0) 
    // 0x00000002 [1]     CH1          (0) 
    // 0x00000001 [0]     CH0          (0) 
    io_rw_32 inte;
 
    _REG_(PWM_IRQ0_INTF_OFFSET) // PWM_IRQ0_INTF
    // Interrupt Force for irq0
    // 0x00000800 [11]    CH11         (0) 
    // 0x00000400 [10]    CH10         (0) 
    // 0x00000200 [9]     CH9          (0) 
    // 0x00000100 [8]     CH8          (0) 
    // 0x00000080 [7]     CH7          (0) 
    // 0x00000040 [6]     CH6          (0) 
    // 0x00000020 [5]     CH5          (0) 
    // 0x00000010 [4]     CH4          (0) 
    // 0x00000008 [3]     CH3          (0) 
    // 0x00000004 [2]     CH2          (0) 
    // 0x00000002 [1]     CH1          (0) 
    // 0x00000001 [0]     CH0          (0) 
    io_rw_32 intf;
 
    _REG_(PWM_IRQ0_INTS_OFFSET) // PWM_IRQ0_INTS
    // Interrupt status after masking & forcing for irq0
    // 0x00000800 [11]    CH11         (0) 
    // 0x00000400 [10]    CH10         (0) 
    // 0x00000200 [9]     CH9          (0) 
    // 0x00000100 [8]     CH8          (0) 
    // 0x00000080 [7]     CH7          (0) 
    // 0x00000040 [6]     CH6          (0) 
    // 0x00000020 [5]     CH5          (0) 
    // 0x00000010 [4]     CH4          (0) 
    // 0x00000008 [3]     CH3          (0) 
    // 0x00000004 [2]     CH2          (0) 
    // 0x00000002 [1]     CH1          (0) 
    // 0x00000001 [0]     CH0          (0) 
    io_ro_32 ints;
} pwm_irq_ctrl_hw_t;

typedef struct {
    pwm_slice_hw_t slice[12];
 
    _REG_(PWM_EN_OFFSET) // PWM_EN
    // This register aliases the CSR_EN bits for all channels
    // 0x00000800 [11]    CH11         (0) 
    // 0x00000400 [10]    CH10         (0) 
    // 0x00000200 [9]     CH9          (0) 
    // 0x00000100 [8]     CH8          (0) 
    // 0x00000080 [7]     CH7          (0) 
    // 0x00000040 [6]     CH6          (0) 
    // 0x00000020 [5]     CH5          (0) 
    // 0x00000010 [4]     CH4          (0) 
    // 0x00000008 [3]     CH3          (0) 
    // 0x00000004 [2]     CH2          (0) 
    // 0x00000002 [1]     CH1          (0) 
    // 0x00000001 [0]     CH0          (0) 
    io_rw_32 en;
 
    _REG_(PWM_INTR_OFFSET) // PWM_INTR
    // Raw Interrupts
    // 0x00000800 [11]    CH11         (0) 
    // 0x00000400 [10]    CH10         (0) 
    // 0x00000200 [9]     CH9          (0) 
    // 0x00000100 [8]     CH8          (0) 
    // 0x00000080 [7]     CH7          (0) 
    // 0x00000040 [6]     CH6          (0) 
    // 0x00000020 [5]     CH5          (0) 
    // 0x00000010 [4]     CH4          (0) 
    // 0x00000008 [3]     CH3          (0) 
    // 0x00000004 [2]     CH2          (0) 
    // 0x00000002 [1]     CH1          (0) 
    // 0x00000001 [0]     CH0          (0) 
    io_rw_32 intr;
 
    union {
        struct {
            _REG_(PWM_IRQ0_INTE_OFFSET) // PWM_IRQ0_INTE
            // Interrupt Enable for irq0
            // 0x00000800 [11]    CH11         (0) 
            // 0x00000400 [10]    CH10         (0) 
            // 0x00000200 [9]     CH9          (0) 
            // 0x00000100 [8]     CH8          (0) 
            // 0x00000080 [7]     CH7          (0) 
            // 0x00000040 [6]     CH6          (0) 
            // 0x00000020 [5]     CH5          (0) 
            // 0x00000010 [4]     CH4          (0) 
            // 0x00000008 [3]     CH3          (0) 
            // 0x00000004 [2]     CH2          (0) 
            // 0x00000002 [1]     CH1          (0) 
            // 0x00000001 [0]     CH0          (0) 
            io_rw_32 inte;

            _REG_(PWM_IRQ0_INTF_OFFSET) // PWM_IRQ0_INTF
            // Interrupt Force for irq0
            // 0x00000800 [11]    CH11         (0) 
            // 0x00000400 [10]    CH10         (0) 
            // 0x00000200 [9]     CH9          (0) 
            // 0x00000100 [8]     CH8          (0) 
            // 0x00000080 [7]     CH7          (0) 
            // 0x00000040 [6]     CH6          (0) 
            // 0x00000020 [5]     CH5          (0) 
            // 0x00000010 [4]     CH4          (0) 
            // 0x00000008 [3]     CH3          (0) 
            // 0x00000004 [2]     CH2          (0) 
            // 0x00000002 [1]     CH1          (0) 
            // 0x00000001 [0]     CH0          (0) 
            io_rw_32 intf;

            _REG_(PWM_IRQ0_INTS_OFFSET) // PWM_IRQ0_INTS
            // Interrupt status after masking & forcing for irq0
            // 0x00000800 [11]    CH11         (0) 
            // 0x00000400 [10]    CH10         (0) 
            // 0x00000200 [9]     CH9          (0) 
            // 0x00000100 [8]     CH8          (0) 
            // 0x00000080 [7]     CH7          (0) 
            // 0x00000040 [6]     CH6          (0) 
            // 0x00000020 [5]     CH5          (0) 
            // 0x00000010 [4]     CH4          (0) 
            // 0x00000008 [3]     CH3          (0) 
            // 0x00000004 [2]     CH2          (0) 
            // 0x00000002 [1]     CH1          (0) 
            // 0x00000001 [0]     CH0          (0) 
            io_rw_32 ints;

            _REG_(PWM_IRQ1_INTE_OFFSET) // PWM_IRQ1_INTE
            // Interrupt Enable for irq1
            // 0x00000800 [11]    CH11         (0) 
            // 0x00000400 [10]    CH10         (0) 
            // 0x00000200 [9]     CH9          (0) 
            // 0x00000100 [8]     CH8          (0) 
            // 0x00000080 [7]     CH7          (0) 
            // 0x00000040 [6]     CH6          (0) 
            // 0x00000020 [5]     CH5          (0) 
            // 0x00000010 [4]     CH4          (0) 
            // 0x00000008 [3]     CH3          (0) 
            // 0x00000004 [2]     CH2          (0) 
            // 0x00000002 [1]     CH1          (0) 
            // 0x00000001 [0]     CH0          (0) 
            io_rw_32 inte1;

            _REG_(PWM_IRQ1_INTF_OFFSET) // PWM_IRQ1_INTF
            // Interrupt Force for irq1
            // 0x00000800 [11]    CH11         (0) 
            // 0x00000400 [10]    CH10         (0) 
            // 0x00000200 [9]     CH9          (0) 
            // 0x00000100 [8]     CH8          (0) 
            // 0x00000080 [7]     CH7          (0) 
            // 0x00000040 [6]     CH6          (0) 
            // 0x00000020 [5]     CH5          (0) 
            // 0x00000010 [4]     CH4          (0) 
            // 0x00000008 [3]     CH3          (0) 
            // 0x00000004 [2]     CH2          (0) 
            // 0x00000002 [1]     CH1          (0) 
            // 0x00000001 [0]     CH0          (0) 
            io_rw_32 intf1;

            _REG_(PWM_IRQ1_INTS_OFFSET) // PWM_IRQ1_INTS
            // Interrupt status after masking & forcing for irq1
            // 0x00000800 [11]    CH11         (0) 
            // 0x00000400 [10]    CH10         (0) 
            // 0x00000200 [9]     CH9          (0) 
            // 0x00000100 [8]     CH8          (0) 
            // 0x00000080 [7]     CH7          (0) 
            // 0x00000040 [6]     CH6          (0) 
            // 0x00000020 [5]     CH5          (0) 
            // 0x00000010 [4]     CH4          (0) 
            // 0x00000008 [3]     CH3          (0) 
            // 0x00000004 [2]     CH2          (0) 
            // 0x00000002 [1]     CH1          (0) 
            // 0x00000001 [0]     CH0          (0)                   
            io_rw_32 ints1;
        };
        pwm_irq_ctrl_hw_t irq_ctrl[2];
    };
} pwm_hw_t;

#define pwm_hw ((pwm_hw_t *)PWM_BASE)
static_assert(sizeof (pwm_hw_t) == 0x0110, "");

#endif // _HARDWARE_STRUCTS_PWM_H

