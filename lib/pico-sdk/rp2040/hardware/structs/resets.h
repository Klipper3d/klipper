// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _HARDWARE_STRUCTS_RESETS_H
#define _HARDWARE_STRUCTS_RESETS_H

/**
 * \file rp2040/resets.h
 */

#include "hardware/address_mapped.h"
#include "hardware/regs/resets.h"

// Reference to datasheet: https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf#tab-registerlist_resets
//
// The _REG_ macro is intended to help make the register navigable in your IDE (for example, using the "Go to Definition" feature)
// _REG_(x) will link to the corresponding register in hardware/regs/resets.h.
//
// Bit-field descriptions are of the form:
// BITMASK [BITRANGE] FIELDNAME (RESETVALUE) DESCRIPTION

/** \brief Resettable component numbers on RP2040 (used as typedef \ref reset_num_t)
 *  \ingroup hardware_resets
 */
typedef enum reset_num_rp2040 {
    RESET_ADC = 0, ///< Select ADC to be reset
    RESET_BUSCTRL = 1, ///< Select BUSCTRL to be reset
    RESET_DMA = 2, ///< Select DMA to be reset
    RESET_I2C0 = 3, ///< Select I2C0 to be reset
    RESET_I2C1 = 4, ///< Select I2C1 to be reset
    RESET_IO_BANK0 = 5, ///< Select IO_BANK0 to be reset
    RESET_IO_QSPI = 6, ///< Select IO_QSPI to be reset
    RESET_JTAG = 7, ///< Select JTAG to be reset
    RESET_PADS_BANK0 = 8, ///< Select PADS_BANK0 to be reset
    RESET_PADS_QSPI = 9, ///< Select PADS_QSPI to be reset
    RESET_PIO0 = 10, ///< Select PIO0 to be reset
    RESET_PIO1 = 11, ///< Select PIO1 to be reset
    RESET_PLL_SYS = 12, ///< Select PLL_SYS to be reset
    RESET_PLL_USB = 13, ///< Select PLL_USB to be reset
    RESET_PWM = 14, ///< Select PWM to be reset
    RESET_RTC = 15, ///< Select RTC to be reset
    RESET_SPI0 = 16, ///< Select SPI0 to be reset
    RESET_SPI1 = 17, ///< Select SPI1 to be reset
    RESET_SYSCFG = 18, ///< Select SYSCFG to be reset
    RESET_SYSINFO = 19, ///< Select SYSINFO to be reset
    RESET_TBMAN = 20, ///< Select TBMAN to be reset
    RESET_TIMER = 21, ///< Select TIMER to be reset
    RESET_UART0 = 22, ///< Select UART0 to be reset
    RESET_UART1 = 23, ///< Select UART1 to be reset
    RESET_USBCTRL = 24, ///< Select USBCTRL to be reset
    RESET_COUNT
} reset_num_t;

/// \tag::resets_hw[]
typedef struct {
    _REG_(RESETS_RESET_OFFSET) // RESETS_RESET
    // Reset control.
    // 0x01000000 [24]    USBCTRL      (1) 
    // 0x00800000 [23]    UART1        (1) 
    // 0x00400000 [22]    UART0        (1) 
    // 0x00200000 [21]    TIMER        (1) 
    // 0x00100000 [20]    TBMAN        (1) 
    // 0x00080000 [19]    SYSINFO      (1) 
    // 0x00040000 [18]    SYSCFG       (1) 
    // 0x00020000 [17]    SPI1         (1) 
    // 0x00010000 [16]    SPI0         (1) 
    // 0x00008000 [15]    RTC          (1) 
    // 0x00004000 [14]    PWM          (1) 
    // 0x00002000 [13]    PLL_USB      (1) 
    // 0x00001000 [12]    PLL_SYS      (1) 
    // 0x00000800 [11]    PIO1         (1) 
    // 0x00000400 [10]    PIO0         (1) 
    // 0x00000200 [9]     PADS_QSPI    (1) 
    // 0x00000100 [8]     PADS_BANK0   (1) 
    // 0x00000080 [7]     JTAG         (1) 
    // 0x00000040 [6]     IO_QSPI      (1) 
    // 0x00000020 [5]     IO_BANK0     (1) 
    // 0x00000010 [4]     I2C1         (1) 
    // 0x00000008 [3]     I2C0         (1) 
    // 0x00000004 [2]     DMA          (1) 
    // 0x00000002 [1]     BUSCTRL      (1) 
    // 0x00000001 [0]     ADC          (1) 
    io_rw_32 reset;
 
    _REG_(RESETS_WDSEL_OFFSET) // RESETS_WDSEL
    // Watchdog select.
    // 0x01000000 [24]    USBCTRL      (0) 
    // 0x00800000 [23]    UART1        (0) 
    // 0x00400000 [22]    UART0        (0) 
    // 0x00200000 [21]    TIMER        (0) 
    // 0x00100000 [20]    TBMAN        (0) 
    // 0x00080000 [19]    SYSINFO      (0) 
    // 0x00040000 [18]    SYSCFG       (0) 
    // 0x00020000 [17]    SPI1         (0) 
    // 0x00010000 [16]    SPI0         (0) 
    // 0x00008000 [15]    RTC          (0) 
    // 0x00004000 [14]    PWM          (0) 
    // 0x00002000 [13]    PLL_USB      (0) 
    // 0x00001000 [12]    PLL_SYS      (0) 
    // 0x00000800 [11]    PIO1         (0) 
    // 0x00000400 [10]    PIO0         (0) 
    // 0x00000200 [9]     PADS_QSPI    (0) 
    // 0x00000100 [8]     PADS_BANK0   (0) 
    // 0x00000080 [7]     JTAG         (0) 
    // 0x00000040 [6]     IO_QSPI      (0) 
    // 0x00000020 [5]     IO_BANK0     (0) 
    // 0x00000010 [4]     I2C1         (0) 
    // 0x00000008 [3]     I2C0         (0) 
    // 0x00000004 [2]     DMA          (0) 
    // 0x00000002 [1]     BUSCTRL      (0) 
    // 0x00000001 [0]     ADC          (0) 
    io_rw_32 wdsel;
 
    _REG_(RESETS_RESET_DONE_OFFSET) // RESETS_RESET_DONE
    // Reset done.
    // 0x01000000 [24]    USBCTRL      (0) 
    // 0x00800000 [23]    UART1        (0) 
    // 0x00400000 [22]    UART0        (0) 
    // 0x00200000 [21]    TIMER        (0) 
    // 0x00100000 [20]    TBMAN        (0) 
    // 0x00080000 [19]    SYSINFO      (0) 
    // 0x00040000 [18]    SYSCFG       (0) 
    // 0x00020000 [17]    SPI1         (0) 
    // 0x00010000 [16]    SPI0         (0) 
    // 0x00008000 [15]    RTC          (0) 
    // 0x00004000 [14]    PWM          (0) 
    // 0x00002000 [13]    PLL_USB      (0) 
    // 0x00001000 [12]    PLL_SYS      (0) 
    // 0x00000800 [11]    PIO1         (0) 
    // 0x00000400 [10]    PIO0         (0) 
    // 0x00000200 [9]     PADS_QSPI    (0) 
    // 0x00000100 [8]     PADS_BANK0   (0) 
    // 0x00000080 [7]     JTAG         (0) 
    // 0x00000040 [6]     IO_QSPI      (0) 
    // 0x00000020 [5]     IO_BANK0     (0) 
    // 0x00000010 [4]     I2C1         (0) 
    // 0x00000008 [3]     I2C0         (0) 
    // 0x00000004 [2]     DMA          (0) 
    // 0x00000002 [1]     BUSCTRL      (0) 
    // 0x00000001 [0]     ADC          (0) 
    io_ro_32 reset_done;
} resets_hw_t;
/// \end::resets_hw[]

#define resets_hw ((resets_hw_t *)RESETS_BASE)
static_assert(sizeof (resets_hw_t) == 0x000c, "");

#endif // _HARDWARE_STRUCTS_RESETS_H

