// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _HARDWARE_STRUCTS_RESETS_H
#define _HARDWARE_STRUCTS_RESETS_H

/**
 * \file rp2350/resets.h
 */

#include "hardware/address_mapped.h"
#include "hardware/regs/resets.h"

// Reference to datasheet: https://datasheets.raspberrypi.com/rp2350/rp2350-datasheet.pdf#tab-registerlist_resets
//
// The _REG_ macro is intended to help make the register navigable in your IDE (for example, using the "Go to Definition" feature)
// _REG_(x) will link to the corresponding register in hardware/regs/resets.h.
//
// Bit-field descriptions are of the form:
// BITMASK [BITRANGE] FIELDNAME (RESETVALUE) DESCRIPTION

/** \brief Resettable component numbers on RP2350 (used as typedef \ref reset_num_t)
 *  \ingroup hardware_resets
 */
typedef enum reset_num_rp2350 {
    RESET_ADC = 0, ///< Select ADC to be reset
    RESET_BUSCTRL = 1, ///< Select BUSCTRL to be reset
    RESET_DMA = 2, ///< Select DMA to be reset
    RESET_HSTX = 3, ///< Select HSTX to be reset
    RESET_I2C0 = 4, ///< Select I2C0 to be reset
    RESET_I2C1 = 5, ///< Select I2C1 to be reset
    RESET_IO_BANK0 = 6, ///< Select IO_BANK0 to be reset
    RESET_IO_QSPI = 7, ///< Select IO_QSPI to be reset
    RESET_JTAG = 8, ///< Select JTAG to be reset
    RESET_PADS_BANK0 = 9, ///< Select PADS_BANK0 to be reset
    RESET_PADS_QSPI = 10, ///< Select PADS_QSPI to be reset
    RESET_PIO0 = 11, ///< Select PIO0 to be reset
    RESET_PIO1 = 12, ///< Select PIO1 to be reset
    RESET_PIO2 = 13, ///< Select PIO2 to be reset
    RESET_PLL_SYS = 14, ///< Select PLL_SYS to be reset
    RESET_PLL_USB = 15, ///< Select PLL_USB to be reset
    RESET_PWM = 16, ///< Select PWM to be reset
    RESET_SHA256 = 17, ///< Select SHA256 to be reset
    RESET_SPI0 = 18, ///< Select SPI0 to be reset
    RESET_SPI1 = 19, ///< Select SPI1 to be reset
    RESET_SYSCFG = 20, ///< Select SYSCFG to be reset
    RESET_SYSINFO = 21, ///< Select SYSINFO to be reset
    RESET_TBMAN = 22, ///< Select TBMAN to be reset
    RESET_TIMER0 = 23, ///< Select TIMER0 to be reset
    RESET_TIMER1 = 24, ///< Select TIMER1 to be reset
    RESET_TRNG = 25, ///< Select TRNG to be reset
    RESET_UART0 = 26, ///< Select UART0 to be reset
    RESET_UART1 = 27, ///< Select UART1 to be reset
    RESET_USBCTRL = 28, ///< Select USBCTRL to be reset
    RESET_COUNT
} reset_num_t;

/// \tag::resets_hw[]
typedef struct {
    _REG_(RESETS_RESET_OFFSET) // RESETS_RESET
    // 0x10000000 [28]    USBCTRL      (1) 
    // 0x08000000 [27]    UART1        (1) 
    // 0x04000000 [26]    UART0        (1) 
    // 0x02000000 [25]    TRNG         (1) 
    // 0x01000000 [24]    TIMER1       (1) 
    // 0x00800000 [23]    TIMER0       (1) 
    // 0x00400000 [22]    TBMAN        (1) 
    // 0x00200000 [21]    SYSINFO      (1) 
    // 0x00100000 [20]    SYSCFG       (1) 
    // 0x00080000 [19]    SPI1         (1) 
    // 0x00040000 [18]    SPI0         (1) 
    // 0x00020000 [17]    SHA256       (1) 
    // 0x00010000 [16]    PWM          (1) 
    // 0x00008000 [15]    PLL_USB      (1) 
    // 0x00004000 [14]    PLL_SYS      (1) 
    // 0x00002000 [13]    PIO2         (1) 
    // 0x00001000 [12]    PIO1         (1) 
    // 0x00000800 [11]    PIO0         (1) 
    // 0x00000400 [10]    PADS_QSPI    (1) 
    // 0x00000200 [9]     PADS_BANK0   (1) 
    // 0x00000100 [8]     JTAG         (1) 
    // 0x00000080 [7]     IO_QSPI      (1) 
    // 0x00000040 [6]     IO_BANK0     (1) 
    // 0x00000020 [5]     I2C1         (1) 
    // 0x00000010 [4]     I2C0         (1) 
    // 0x00000008 [3]     HSTX         (1) 
    // 0x00000004 [2]     DMA          (1) 
    // 0x00000002 [1]     BUSCTRL      (1) 
    // 0x00000001 [0]     ADC          (1) 
    io_rw_32 reset;
 
    _REG_(RESETS_WDSEL_OFFSET) // RESETS_WDSEL
    // 0x10000000 [28]    USBCTRL      (0) 
    // 0x08000000 [27]    UART1        (0) 
    // 0x04000000 [26]    UART0        (0) 
    // 0x02000000 [25]    TRNG         (0) 
    // 0x01000000 [24]    TIMER1       (0) 
    // 0x00800000 [23]    TIMER0       (0) 
    // 0x00400000 [22]    TBMAN        (0) 
    // 0x00200000 [21]    SYSINFO      (0) 
    // 0x00100000 [20]    SYSCFG       (0) 
    // 0x00080000 [19]    SPI1         (0) 
    // 0x00040000 [18]    SPI0         (0) 
    // 0x00020000 [17]    SHA256       (0) 
    // 0x00010000 [16]    PWM          (0) 
    // 0x00008000 [15]    PLL_USB      (0) 
    // 0x00004000 [14]    PLL_SYS      (0) 
    // 0x00002000 [13]    PIO2         (0) 
    // 0x00001000 [12]    PIO1         (0) 
    // 0x00000800 [11]    PIO0         (0) 
    // 0x00000400 [10]    PADS_QSPI    (0) 
    // 0x00000200 [9]     PADS_BANK0   (0) 
    // 0x00000100 [8]     JTAG         (0) 
    // 0x00000080 [7]     IO_QSPI      (0) 
    // 0x00000040 [6]     IO_BANK0     (0) 
    // 0x00000020 [5]     I2C1         (0) 
    // 0x00000010 [4]     I2C0         (0) 
    // 0x00000008 [3]     HSTX         (0) 
    // 0x00000004 [2]     DMA          (0) 
    // 0x00000002 [1]     BUSCTRL      (0) 
    // 0x00000001 [0]     ADC          (0) 
    io_rw_32 wdsel;
 
    _REG_(RESETS_RESET_DONE_OFFSET) // RESETS_RESET_DONE
    // 0x10000000 [28]    USBCTRL      (0) 
    // 0x08000000 [27]    UART1        (0) 
    // 0x04000000 [26]    UART0        (0) 
    // 0x02000000 [25]    TRNG         (0) 
    // 0x01000000 [24]    TIMER1       (0) 
    // 0x00800000 [23]    TIMER0       (0) 
    // 0x00400000 [22]    TBMAN        (0) 
    // 0x00200000 [21]    SYSINFO      (0) 
    // 0x00100000 [20]    SYSCFG       (0) 
    // 0x00080000 [19]    SPI1         (0) 
    // 0x00040000 [18]    SPI0         (0) 
    // 0x00020000 [17]    SHA256       (0) 
    // 0x00010000 [16]    PWM          (0) 
    // 0x00008000 [15]    PLL_USB      (0) 
    // 0x00004000 [14]    PLL_SYS      (0) 
    // 0x00002000 [13]    PIO2         (0) 
    // 0x00001000 [12]    PIO1         (0) 
    // 0x00000800 [11]    PIO0         (0) 
    // 0x00000400 [10]    PADS_QSPI    (0) 
    // 0x00000200 [9]     PADS_BANK0   (0) 
    // 0x00000100 [8]     JTAG         (0) 
    // 0x00000080 [7]     IO_QSPI      (0) 
    // 0x00000040 [6]     IO_BANK0     (0) 
    // 0x00000020 [5]     I2C1         (0) 
    // 0x00000010 [4]     I2C0         (0) 
    // 0x00000008 [3]     HSTX         (0) 
    // 0x00000004 [2]     DMA          (0) 
    // 0x00000002 [1]     BUSCTRL      (0) 
    // 0x00000001 [0]     ADC          (0) 
    io_ro_32 reset_done;
} resets_hw_t;
/// \end::resets_hw[]

#define resets_hw ((resets_hw_t *)RESETS_BASE)
static_assert(sizeof (resets_hw_t) == 0x000c, "");

#endif // _HARDWARE_STRUCTS_RESETS_H

