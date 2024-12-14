// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _HARDWARE_STRUCTS_CLOCKS_H
#define _HARDWARE_STRUCTS_CLOCKS_H

/**
 * \file rp2040/clocks.h
 */

#include "hardware/address_mapped.h"
#include "hardware/regs/clocks.h"

// Reference to datasheet: https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf#tab-registerlist_clocks
//
// The _REG_ macro is intended to help make the register navigable in your IDE (for example, using the "Go to Definition" feature)
// _REG_(x) will link to the corresponding register in hardware/regs/clocks.h.
//
// Bit-field descriptions are of the form:
// BITMASK [BITRANGE] FIELDNAME (RESETVALUE) DESCRIPTION

/** \brief Clock numbers on RP2040 (used as typedef \ref clock_num_t) 
 *  \ingroup hardware_clocks
 */
/// \tag::clkenum[]
typedef enum clock_num_rp2040 {
    clk_gpout0 = 0, ///< Select CLK_GPOUT0 as clock source
    clk_gpout1 = 1, ///< Select CLK_GPOUT1 as clock source
    clk_gpout2 = 2, ///< Select CLK_GPOUT2 as clock source
    clk_gpout3 = 3, ///< Select CLK_GPOUT3 as clock source
    clk_ref = 4, ///< Select CLK_REF as clock source
    clk_sys = 5, ///< Select CLK_SYS as clock source
    clk_peri = 6, ///< Select CLK_PERI as clock source
    clk_usb = 7, ///< Select CLK_USB as clock source
    clk_adc = 8, ///< Select CLK_ADC as clock source
    clk_rtc = 9, ///< Select CLK_RTC as clock source
    CLK_COUNT
} clock_num_t;
/// \end::clkenum[]

/** \brief  Clock destination numbers on RP2040 (used as typedef \ref clock_dest_num_t)
 *  \ingroup hardware_clocks
 */
typedef enum clock_dest_num_rp2040 {
    CLK_DEST_SYS_CLOCKS = 0, ///< Select SYS_CLOCKS as clock destination
    CLK_DEST_ADC_ADC = 1, ///< Select ADC_ADC as clock destination
    CLK_DEST_SYS_ADC = 2, ///< Select SYS_ADC as clock destination
    CLK_DEST_SYS_BUSCTRL = 3, ///< Select SYS_BUSCTRL as clock destination
    CLK_DEST_SYS_BUSFABRIC = 4, ///< Select SYS_BUSFABRIC as clock destination
    CLK_DEST_SYS_DMA = 5, ///< Select SYS_DMA as clock destination
    CLK_DEST_SYS_I2C0 = 6, ///< Select SYS_I2C0 as clock destination
    CLK_DEST_SYS_I2C1 = 7, ///< Select SYS_I2C1 as clock destination
    CLK_DEST_SYS_IO = 8, ///< Select SYS_IO as clock destination
    CLK_DEST_SYS_JTAG = 9, ///< Select SYS_JTAG as clock destination
    CLK_DEST_SYS_VREG_AND_CHIP_RESET = 10, ///< Select SYS_VREG_AND_CHIP_RESET as clock destination
    CLK_DEST_SYS_PADS = 11, ///< Select SYS_PADS as clock destination
    CLK_DEST_SYS_PIO0 = 12, ///< Select SYS_PIO0 as clock destination
    CLK_DEST_SYS_PIO1 = 13, ///< Select SYS_PIO1 as clock destination
    CLK_DEST_SYS_PLL_SYS = 14, ///< Select SYS_PLL_SYS as clock destination
    CLK_DEST_SYS_PLL_USB = 15, ///< Select SYS_PLL_USB as clock destination
    CLK_DEST_SYS_PSM = 16, ///< Select SYS_PSM as clock destination
    CLK_DEST_SYS_PWM = 17, ///< Select SYS_PWM as clock destination
    CLK_DEST_SYS_RESETS = 18, ///< Select SYS_RESETS as clock destination
    CLK_DEST_SYS_ROM = 19, ///< Select SYS_ROM as clock destination
    CLK_DEST_SYS_ROSC = 20, ///< Select SYS_ROSC as clock destination
    CLK_DEST_RTC_RTC = 21, ///< Select RTC_RTC as clock destination
    CLK_DEST_SYS_RTC = 22, ///< Select SYS_RTC as clock destination
    CLK_DEST_SYS_SIO = 23, ///< Select SYS_SIO as clock destination
    CLK_DEST_PERI_SPI0 = 24, ///< Select PERI_SPI0 as clock destination
    CLK_DEST_SYS_SPI0 = 25, ///< Select SYS_SPI0 as clock destination
    CLK_DEST_PERI_SPI1 = 26, ///< Select PERI_SPI1 as clock destination
    CLK_DEST_SYS_SPI1 = 27, ///< Select SYS_SPI1 as clock destination
    CLK_DEST_SYS_SRAM0 = 28, ///< Select SYS_SRAM0 as clock destination
    CLK_DEST_SYS_SRAM1 = 29, ///< Select SYS_SRAM1 as clock destination
    CLK_DEST_SYS_SRAM2 = 30, ///< Select SYS_SRAM2 as clock destination
    CLK_DEST_SYS_SRAM3 = 31, ///< Select SYS_SRAM3 as clock destination
    CLK_DEST_SYS_SRAM4 = 32, ///< Select SYS_SRAM4 as clock destination
    CLK_DEST_SYS_SRAM5 = 33, ///< Select SYS_SRAM5 as clock destination
    CLK_DEST_SYS_SYSCFG = 34, ///< Select SYS_SYSCFG as clock destination
    CLK_DEST_SYS_SYSINFO = 35, ///< Select SYS_SYSINFO as clock destination
    CLK_DEST_SYS_TBMAN = 36, ///< Select SYS_TBMAN as clock destination
    CLK_DEST_SYS_TIMER = 37, ///< Select SYS_TIMER as clock destination
    CLK_DEST_PERI_UART0 = 38, ///< Select PERI_UART0 as clock destination
    CLK_DEST_SYS_UART0 = 39, ///< Select SYS_UART0 as clock destination
    CLK_DEST_PERI_UART1 = 40, ///< Select PERI_UART1 as clock destination
    CLK_DEST_SYS_UART1 = 41, ///< Select SYS_UART1 as clock destination
    CLK_DEST_SYS_USBCTRL = 42, ///< Select SYS_USBCTRL as clock destination
    CLK_DEST_USB_USBCTRL = 43, ///< Select USB_USBCTRL as clock destination
    CLK_DEST_SYS_WATCHDOG = 44, ///< Select SYS_WATCHDOG as clock destination
    CLK_DEST_SYS_XIP = 45, ///< Select SYS_XIP as clock destination
    CLK_DEST_SYS_XOSC = 46, ///< Select SYS_XOSC as clock destination
    NUM_CLOCK_DESTINATIONS
} clock_dest_num_t;

/// \tag::clock_hw[]
typedef struct {
    _REG_(CLOCKS_CLK_GPOUT0_CTRL_OFFSET) // CLOCKS_CLK_GPOUT0_CTRL
    // Clock control, can be changed on-the-fly (except for auxsrc)
    // 0x00100000 [20]    NUDGE        (0) An edge on this signal shifts the phase of the output by...
    // 0x00030000 [17:16] PHASE        (0x0) This delays the enable signal by up to 3 cycles of the...
    // 0x00001000 [12]    DC50         (0) Enables duty cycle correction for odd divisors
    // 0x00000800 [11]    ENABLE       (0) Starts and stops the clock generator cleanly
    // 0x00000400 [10]    KILL         (0) Asynchronously kills the clock generator
    // 0x000001e0 [8:5]   AUXSRC       (0x0) Selects the auxiliary clock source, will glitch when switching
    io_rw_32 ctrl;
 
    _REG_(CLOCKS_CLK_GPOUT0_DIV_OFFSET) // CLOCKS_CLK_GPOUT0_DIV
    // Clock divisor, can be changed on-the-fly
    // 0xffffff00 [31:8]  INT          (0x000001) Integer component of the divisor, 0 -> divide by 2^16
    // 0x000000ff [7:0]   FRAC         (0x00) Fractional component of the divisor
    io_rw_32 div;
 
    _REG_(CLOCKS_CLK_GPOUT0_SELECTED_OFFSET) // CLOCKS_CLK_GPOUT0_SELECTED
    // Indicates which SRC is currently selected by the glitchless mux (one-hot)
    // 0xffffffff [31:0]  CLK_GPOUT0_SELECTED (0x00000001) This slice does not have a glitchless mux (only the...
    io_ro_32 selected;
} clock_hw_t;
/// \end::clock_hw[]

typedef struct {
    _REG_(CLOCKS_CLK_SYS_RESUS_CTRL_OFFSET) // CLOCKS_CLK_SYS_RESUS_CTRL
    // 0x00010000 [16]    CLEAR        (0) For clearing the resus after the fault that triggered it...
    // 0x00001000 [12]    FRCE         (0) Force a resus, for test purposes only
    // 0x00000100 [8]     ENABLE       (0) Enable resus
    // 0x000000ff [7:0]   TIMEOUT      (0xff) This is expressed as a number of clk_ref cycles +
    io_rw_32 ctrl;
 
    _REG_(CLOCKS_CLK_SYS_RESUS_STATUS_OFFSET) // CLOCKS_CLK_SYS_RESUS_STATUS
    // 0x00000001 [0]     RESUSSED     (0) Clock has been resuscitated, correct the error then send...
    io_ro_32 status;
} clock_resus_hw_t;

typedef struct {
    _REG_(CLOCKS_FC0_REF_KHZ_OFFSET) // CLOCKS_FC0_REF_KHZ
    // Reference clock frequency in kHz
    // 0x000fffff [19:0]  FC0_REF_KHZ  (0x00000) 
    io_rw_32 ref_khz;
 
    _REG_(CLOCKS_FC0_MIN_KHZ_OFFSET) // CLOCKS_FC0_MIN_KHZ
    // Minimum pass frequency in kHz
    // 0x01ffffff [24:0]  FC0_MIN_KHZ  (0x0000000) 
    io_rw_32 min_khz;
 
    _REG_(CLOCKS_FC0_MAX_KHZ_OFFSET) // CLOCKS_FC0_MAX_KHZ
    // Maximum pass frequency in kHz
    // 0x01ffffff [24:0]  FC0_MAX_KHZ  (0x1ffffff) 
    io_rw_32 max_khz;
 
    _REG_(CLOCKS_FC0_DELAY_OFFSET) // CLOCKS_FC0_DELAY
    // Delays the start of frequency counting to allow the mux to settle +
    // 0x00000007 [2:0]   FC0_DELAY    (0x1) 
    io_rw_32 delay;
 
    _REG_(CLOCKS_FC0_INTERVAL_OFFSET) // CLOCKS_FC0_INTERVAL
    // The test interval is 0
    // 0x0000000f [3:0]   FC0_INTERVAL (0x8) 
    io_rw_32 interval;
 
    _REG_(CLOCKS_FC0_SRC_OFFSET) // CLOCKS_FC0_SRC
    // Clock sent to frequency counter, set to 0 when not required +
    // 0x000000ff [7:0]   FC0_SRC      (0x00) 
    io_rw_32 src;
 
    _REG_(CLOCKS_FC0_STATUS_OFFSET) // CLOCKS_FC0_STATUS
    // Frequency counter status
    // 0x10000000 [28]    DIED         (0) Test clock stopped during test
    // 0x01000000 [24]    FAST         (0) Test clock faster than expected, only valid when status_done=1
    // 0x00100000 [20]    SLOW         (0) Test clock slower than expected, only valid when status_done=1
    // 0x00010000 [16]    FAIL         (0) Test failed
    // 0x00001000 [12]    WAITING      (0) Waiting for test clock to start
    // 0x00000100 [8]     RUNNING      (0) Test running
    // 0x00000010 [4]     DONE         (0) Test complete
    // 0x00000001 [0]     PASS         (0) Test passed
    io_ro_32 status;
 
    _REG_(CLOCKS_FC0_RESULT_OFFSET) // CLOCKS_FC0_RESULT
    // Result of frequency measurement, only valid when status_done=1
    // 0x3fffffe0 [29:5]  KHZ          (0x0000000) 
    // 0x0000001f [4:0]   FRAC         (0x00) 
    io_ro_32 result;
} fc_hw_t;

typedef struct {
    clock_hw_t clk[10];
 
    clock_resus_hw_t resus;
 
    fc_hw_t fc0;
 
    union {
        struct {
            _REG_(CLOCKS_WAKE_EN0_OFFSET) // CLOCKS_WAKE_EN0
            // enable clock in wake mode
            // 0x80000000 [31]    CLK_SYS_SRAM3 (1) 
            // 0x40000000 [30]    CLK_SYS_SRAM2 (1) 
            // 0x20000000 [29]    CLK_SYS_SRAM1 (1) 
            // 0x10000000 [28]    CLK_SYS_SRAM0 (1) 
            // 0x08000000 [27]    CLK_SYS_SPI1 (1) 
            // 0x04000000 [26]    CLK_PERI_SPI1 (1) 
            // 0x02000000 [25]    CLK_SYS_SPI0 (1) 
            // 0x01000000 [24]    CLK_PERI_SPI0 (1) 
            // 0x00800000 [23]    CLK_SYS_SIOB (1) 
            // 0x00400000 [22]    CLK_SYS_RTC  (1) 
            // 0x00200000 [21]    CLK_RTC_RTC  (1) 
            // 0x00100000 [20]    CLK_SYS_ROSC (1) 
            // 0x00080000 [19]    CLK_SYS_ROM  (1) 
            // 0x00040000 [18]    CLK_SYS_RESETS (1) 
            // 0x00020000 [17]    CLK_SYS_PWM  (1) 
            // 0x00010000 [16]    CLK_SYS_POWER (1) 
            // 0x00008000 [15]    CLK_SYS_PLL_USB (1) 
            // 0x00004000 [14]    CLK_SYS_PLL_SYS (1) 
            // 0x00002000 [13]    CLK_SYS_PIO1 (1) 
            // 0x00001000 [12]    CLK_SYS_PIO0 (1) 
            // 0x00000800 [11]    CLK_SYS_PADS (1) 
            // 0x00000400 [10]    CLK_SYS_LDO_POR (1) 
            // 0x00000200 [9]     CLK_SYS_JTAG (1) 
            // 0x00000100 [8]     CLK_SYS_IO   (1) 
            // 0x00000080 [7]     CLK_SYS_I2C1 (1) 
            // 0x00000040 [6]     CLK_SYS_I2C0 (1) 
            // 0x00000020 [5]     CLK_SYS_DMA  (1) 
            // 0x00000010 [4]     CLK_SYS_BUSFABRIC (1) 
            // 0x00000008 [3]     CLK_SYS_BUSCTRL (1) 
            // 0x00000004 [2]     CLK_SYS_ADC0 (1) 
            // 0x00000002 [1]     CLK_ADC_ADC0 (1) 
            // 0x00000001 [0]     CLK_SYS_CLOCKS_BANK_DEFAULT (1) 
            io_rw_32 wake_en0; 

            _REG_(CLOCKS_WAKE_EN1_OFFSET) // CLOCKS_WAKE_EN1
            // enable clock in wake mode
            // 0x00004000 [14]    CLK_SYS_XOSC (1) 
            // 0x00002000 [13]    CLK_SYS_XIP  (1) 
            // 0x00001000 [12]    CLK_SYS_WATCHDOG (1) 
            // 0x00000800 [11]    CLK_USB_USBCTRL (1) 
            // 0x00000400 [10]    CLK_SYS_USBCTRL (1) 
            // 0x00000200 [9]     CLK_SYS_UART1 (1) 
            // 0x00000100 [8]     CLK_PERI_UART1 (1) 
            // 0x00000080 [7]     CLK_SYS_UART0 (1) 
            // 0x00000040 [6]     CLK_PERI_UART0 (1) 
            // 0x00000020 [5]     CLK_SYS_TIMER (1) 
            // 0x00000010 [4]     CLK_SYS_TBMAN (1) 
            // 0x00000008 [3]     CLK_SYS_SYSINFO (1) 
            // 0x00000004 [2]     CLK_SYS_SYSCFG (1) 
            // 0x00000002 [1]     CLK_SYS_SRAM5 (1) 
            // 0x00000001 [0]     CLK_SYS_SRAM4 (1) 
            io_rw_32 wake_en1; 
        };
        // (Description copied from array index 0 register CLOCKS_WAKE_EN0 applies similarly to other array indexes)
        _REG_(CLOCKS_WAKE_EN0_OFFSET) // CLOCKS_WAKE_EN0
        // enable clock in wake mode
        // 0x80000000 [31]    CLK_SYS_SRAM3 (1) 
        // 0x40000000 [30]    CLK_SYS_SRAM2 (1) 
        // 0x20000000 [29]    CLK_SYS_SRAM1 (1) 
        // 0x10000000 [28]    CLK_SYS_SRAM0 (1) 
        // 0x08000000 [27]    CLK_SYS_SPI1 (1) 
        // 0x04000000 [26]    CLK_PERI_SPI1 (1) 
        // 0x02000000 [25]    CLK_SYS_SPI0 (1) 
        // 0x01000000 [24]    CLK_PERI_SPI0 (1) 
        // 0x00800000 [23]    CLK_SYS_SIO  (1) 
        // 0x00400000 [22]    CLK_SYS_RTC  (1) 
        // 0x00200000 [21]    CLK_RTC_RTC  (1) 
        // 0x00100000 [20]    CLK_SYS_ROSC (1) 
        // 0x00080000 [19]    CLK_SYS_ROM  (1) 
        // 0x00040000 [18]    CLK_SYS_RESETS (1) 
        // 0x00020000 [17]    CLK_SYS_PWM  (1) 
        // 0x00010000 [16]    CLK_SYS_PSM  (1) 
        // 0x00008000 [15]    CLK_SYS_PLL_USB (1) 
        // 0x00004000 [14]    CLK_SYS_PLL_SYS (1) 
        // 0x00002000 [13]    CLK_SYS_PIO1 (1) 
        // 0x00001000 [12]    CLK_SYS_PIO0 (1) 
        // 0x00000800 [11]    CLK_SYS_PADS (1) 
        // 0x00000400 [10]    CLK_SYS_VREG_AND_CHIP_RESET (1) 
        // 0x00000200 [9]     CLK_SYS_JTAG (1) 
        // 0x00000100 [8]     CLK_SYS_IO   (1) 
        // 0x00000080 [7]     CLK_SYS_I2C1 (1) 
        // 0x00000040 [6]     CLK_SYS_I2C0 (1) 
        // 0x00000020 [5]     CLK_SYS_DMA  (1) 
        // 0x00000010 [4]     CLK_SYS_BUSFABRIC (1) 
        // 0x00000008 [3]     CLK_SYS_BUSCTRL (1) 
        // 0x00000004 [2]     CLK_SYS_ADC  (1) 
        // 0x00000002 [1]     CLK_ADC_ADC  (1) 
        // 0x00000001 [0]     CLK_SYS_CLOCKS (1) 
        io_rw_32 wake_en[2];
    };
 
    union {
        struct {
            _REG_(CLOCKS_SLEEP_EN0_OFFSET) // CLOCKS_SLEEP_EN0
            // enable clock in sleep mode
            // 0x80000000 [31]    CLK_SYS_SRAM3 (1) 
            // 0x40000000 [30]    CLK_SYS_SRAM2 (1) 
            // 0x20000000 [29]    CLK_SYS_SRAM1 (1) 
            // 0x10000000 [28]    CLK_SYS_SRAM0 (1) 
            // 0x08000000 [27]    CLK_SYS_SPI1 (1) 
            // 0x04000000 [26]    CLK_PERI_SPI1 (1) 
            // 0x02000000 [25]    CLK_SYS_SPI0 (1) 
            // 0x01000000 [24]    CLK_PERI_SPI0 (1) 
            // 0x00800000 [23]    CLK_SYS_SIOB (1) 
            // 0x00400000 [22]    CLK_SYS_RTC  (1) 
            // 0x00200000 [21]    CLK_RTC_RTC  (1) 
            // 0x00100000 [20]    CLK_SYS_ROSC (1) 
            // 0x00080000 [19]    CLK_SYS_ROM  (1) 
            // 0x00040000 [18]    CLK_SYS_RESETS (1) 
            // 0x00020000 [17]    CLK_SYS_PWM  (1) 
            // 0x00010000 [16]    CLK_SYS_POWER (1) 
            // 0x00008000 [15]    CLK_SYS_PLL_USB (1) 
            // 0x00004000 [14]    CLK_SYS_PLL_SYS (1) 
            // 0x00002000 [13]    CLK_SYS_PIO1 (1) 
            // 0x00001000 [12]    CLK_SYS_PIO0 (1) 
            // 0x00000800 [11]    CLK_SYS_PADS (1) 
            // 0x00000400 [10]    CLK_SYS_LDO_POR (1) 
            // 0x00000200 [9]     CLK_SYS_JTAG (1) 
            // 0x00000100 [8]     CLK_SYS_IO   (1) 
            // 0x00000080 [7]     CLK_SYS_I2C1 (1) 
            // 0x00000040 [6]     CLK_SYS_I2C0 (1) 
            // 0x00000020 [5]     CLK_SYS_DMA  (1) 
            // 0x00000010 [4]     CLK_SYS_BUSFABRIC (1) 
            // 0x00000008 [3]     CLK_SYS_BUSCTRL (1) 
            // 0x00000004 [2]     CLK_SYS_ADC0 (1) 
            // 0x00000002 [1]     CLK_ADC_ADC0 (1) 
            // 0x00000001 [0]     CLK_SYS_CLOCKS_BANK_DEFAULT (1) 
            io_rw_32 sleep_en0; 

            _REG_(CLOCKS_SLEEP_EN1_OFFSET) // CLOCKS_SLEEP_EN1
            // enable clock in sleep mode
            // 0x00004000 [14]    CLK_SYS_XOSC (1) 
            // 0x00002000 [13]    CLK_SYS_XIP  (1) 
            // 0x00001000 [12]    CLK_SYS_WATCHDOG (1) 
            // 0x00000800 [11]    CLK_USB_USBCTRL (1) 
            // 0x00000400 [10]    CLK_SYS_USBCTRL (1) 
            // 0x00000200 [9]     CLK_SYS_UART1 (1) 
            // 0x00000100 [8]     CLK_PERI_UART1 (1) 
            // 0x00000080 [7]     CLK_SYS_UART0 (1) 
            // 0x00000040 [6]     CLK_PERI_UART0 (1) 
            // 0x00000020 [5]     CLK_SYS_TIMER (1) 
            // 0x00000010 [4]     CLK_SYS_TBMAN (1) 
            // 0x00000008 [3]     CLK_SYS_SYSINFO (1) 
            // 0x00000004 [2]     CLK_SYS_SYSCFG (1) 
            // 0x00000002 [1]     CLK_SYS_SRAM5 (1) 
            // 0x00000001 [0]     CLK_SYS_SRAM4 (1) 
            io_rw_32 sleep_en1; 
        };
        // (Description copied from array index 0 register CLOCKS_SLEEP_EN0 applies similarly to other array indexes)
        _REG_(CLOCKS_SLEEP_EN0_OFFSET) // CLOCKS_SLEEP_EN0
        // enable clock in sleep mode
        // 0x80000000 [31]    CLK_SYS_SRAM3 (1) 
        // 0x40000000 [30]    CLK_SYS_SRAM2 (1) 
        // 0x20000000 [29]    CLK_SYS_SRAM1 (1) 
        // 0x10000000 [28]    CLK_SYS_SRAM0 (1) 
        // 0x08000000 [27]    CLK_SYS_SPI1 (1) 
        // 0x04000000 [26]    CLK_PERI_SPI1 (1) 
        // 0x02000000 [25]    CLK_SYS_SPI0 (1) 
        // 0x01000000 [24]    CLK_PERI_SPI0 (1) 
        // 0x00800000 [23]    CLK_SYS_SIO  (1) 
        // 0x00400000 [22]    CLK_SYS_RTC  (1) 
        // 0x00200000 [21]    CLK_RTC_RTC  (1) 
        // 0x00100000 [20]    CLK_SYS_ROSC (1) 
        // 0x00080000 [19]    CLK_SYS_ROM  (1) 
        // 0x00040000 [18]    CLK_SYS_RESETS (1) 
        // 0x00020000 [17]    CLK_SYS_PWM  (1) 
        // 0x00010000 [16]    CLK_SYS_PSM  (1) 
        // 0x00008000 [15]    CLK_SYS_PLL_USB (1) 
        // 0x00004000 [14]    CLK_SYS_PLL_SYS (1) 
        // 0x00002000 [13]    CLK_SYS_PIO1 (1) 
        // 0x00001000 [12]    CLK_SYS_PIO0 (1) 
        // 0x00000800 [11]    CLK_SYS_PADS (1) 
        // 0x00000400 [10]    CLK_SYS_VREG_AND_CHIP_RESET (1) 
        // 0x00000200 [9]     CLK_SYS_JTAG (1) 
        // 0x00000100 [8]     CLK_SYS_IO   (1) 
        // 0x00000080 [7]     CLK_SYS_I2C1 (1) 
        // 0x00000040 [6]     CLK_SYS_I2C0 (1) 
        // 0x00000020 [5]     CLK_SYS_DMA  (1) 
        // 0x00000010 [4]     CLK_SYS_BUSFABRIC (1) 
        // 0x00000008 [3]     CLK_SYS_BUSCTRL (1) 
        // 0x00000004 [2]     CLK_SYS_ADC  (1) 
        // 0x00000002 [1]     CLK_ADC_ADC  (1) 
        // 0x00000001 [0]     CLK_SYS_CLOCKS (1) 
        io_rw_32 sleep_en[2];
    };
 
    union {
        struct {
            _REG_(CLOCKS_ENABLED0_OFFSET) // CLOCKS_ENABLED0
            // indicates the state of the clock enable
            // 0x80000000 [31]    CLK_SYS_SRAM3 (0) 
            // 0x40000000 [30]    CLK_SYS_SRAM2 (0) 
            // 0x20000000 [29]    CLK_SYS_SRAM1 (0) 
            // 0x10000000 [28]    CLK_SYS_SRAM0 (0) 
            // 0x08000000 [27]    CLK_SYS_SPI1 (0) 
            // 0x04000000 [26]    CLK_PERI_SPI1 (0) 
            // 0x02000000 [25]    CLK_SYS_SPI0 (0) 
            // 0x01000000 [24]    CLK_PERI_SPI0 (0) 
            // 0x00800000 [23]    CLK_SYS_SIOB (0) 
            // 0x00400000 [22]    CLK_SYS_RTC  (0) 
            // 0x00200000 [21]    CLK_RTC_RTC  (0) 
            // 0x00100000 [20]    CLK_SYS_ROSC (0) 
            // 0x00080000 [19]    CLK_SYS_ROM  (0) 
            // 0x00040000 [18]    CLK_SYS_RESETS (0) 
            // 0x00020000 [17]    CLK_SYS_PWM  (0) 
            // 0x00010000 [16]    CLK_SYS_POWER (0) 
            // 0x00008000 [15]    CLK_SYS_PLL_USB (0) 
            // 0x00004000 [14]    CLK_SYS_PLL_SYS (0) 
            // 0x00002000 [13]    CLK_SYS_PIO1 (0) 
            // 0x00001000 [12]    CLK_SYS_PIO0 (0) 
            // 0x00000800 [11]    CLK_SYS_PADS (0) 
            // 0x00000400 [10]    CLK_SYS_LDO_POR (0) 
            // 0x00000200 [9]     CLK_SYS_JTAG (0) 
            // 0x00000100 [8]     CLK_SYS_IO   (0) 
            // 0x00000080 [7]     CLK_SYS_I2C1 (0) 
            // 0x00000040 [6]     CLK_SYS_I2C0 (0) 
            // 0x00000020 [5]     CLK_SYS_DMA  (0) 
            // 0x00000010 [4]     CLK_SYS_BUSFABRIC (0) 
            // 0x00000008 [3]     CLK_SYS_BUSCTRL (0) 
            // 0x00000004 [2]     CLK_SYS_ADC0 (0) 
            // 0x00000002 [1]     CLK_ADC_ADC0 (0) 
            // 0x00000001 [0]     CLK_SYS_CLOCKS_BANK_DEFAULT (0) 
            io_ro_32 enabled0; 

            _REG_(CLOCKS_ENABLED1_OFFSET) // CLOCKS_ENABLED1
            // indicates the state of the clock enable
            // 0x00004000 [14]    CLK_SYS_XOSC (0) 
            // 0x00002000 [13]    CLK_SYS_XIP  (0) 
            // 0x00001000 [12]    CLK_SYS_WATCHDOG (0) 
            // 0x00000800 [11]    CLK_USB_USBCTRL (0) 
            // 0x00000400 [10]    CLK_SYS_USBCTRL (0) 
            // 0x00000200 [9]     CLK_SYS_UART1 (0) 
            // 0x00000100 [8]     CLK_PERI_UART1 (0) 
            // 0x00000080 [7]     CLK_SYS_UART0 (0) 
            // 0x00000040 [6]     CLK_PERI_UART0 (0) 
            // 0x00000020 [5]     CLK_SYS_TIMER (0) 
            // 0x00000010 [4]     CLK_SYS_TBMAN (0) 
            // 0x00000008 [3]     CLK_SYS_SYSINFO (0) 
            // 0x00000004 [2]     CLK_SYS_SYSCFG (0) 
            // 0x00000002 [1]     CLK_SYS_SRAM5 (0) 
            // 0x00000001 [0]     CLK_SYS_SRAM4 (0) 
            io_ro_32 enabled1; 
        };
        // (Description copied from array index 0 register CLOCKS_ENABLED0 applies similarly to other array indexes)
        _REG_(CLOCKS_ENABLED0_OFFSET) // CLOCKS_ENABLED0
        // indicates the state of the clock enable
        // 0x80000000 [31]    CLK_SYS_SRAM3 (0) 
        // 0x40000000 [30]    CLK_SYS_SRAM2 (0) 
        // 0x20000000 [29]    CLK_SYS_SRAM1 (0) 
        // 0x10000000 [28]    CLK_SYS_SRAM0 (0) 
        // 0x08000000 [27]    CLK_SYS_SPI1 (0) 
        // 0x04000000 [26]    CLK_PERI_SPI1 (0) 
        // 0x02000000 [25]    CLK_SYS_SPI0 (0) 
        // 0x01000000 [24]    CLK_PERI_SPI0 (0) 
        // 0x00800000 [23]    CLK_SYS_SIO  (0) 
        // 0x00400000 [22]    CLK_SYS_RTC  (0) 
        // 0x00200000 [21]    CLK_RTC_RTC  (0) 
        // 0x00100000 [20]    CLK_SYS_ROSC (0) 
        // 0x00080000 [19]    CLK_SYS_ROM  (0) 
        // 0x00040000 [18]    CLK_SYS_RESETS (0) 
        // 0x00020000 [17]    CLK_SYS_PWM  (0) 
        // 0x00010000 [16]    CLK_SYS_PSM  (0) 
        // 0x00008000 [15]    CLK_SYS_PLL_USB (0) 
        // 0x00004000 [14]    CLK_SYS_PLL_SYS (0) 
        // 0x00002000 [13]    CLK_SYS_PIO1 (0) 
        // 0x00001000 [12]    CLK_SYS_PIO0 (0) 
        // 0x00000800 [11]    CLK_SYS_PADS (0) 
        // 0x00000400 [10]    CLK_SYS_VREG_AND_CHIP_RESET (0) 
        // 0x00000200 [9]     CLK_SYS_JTAG (0) 
        // 0x00000100 [8]     CLK_SYS_IO   (0) 
        // 0x00000080 [7]     CLK_SYS_I2C1 (0) 
        // 0x00000040 [6]     CLK_SYS_I2C0 (0) 
        // 0x00000020 [5]     CLK_SYS_DMA  (0) 
        // 0x00000010 [4]     CLK_SYS_BUSFABRIC (0) 
        // 0x00000008 [3]     CLK_SYS_BUSCTRL (0) 
        // 0x00000004 [2]     CLK_SYS_ADC  (0) 
        // 0x00000002 [1]     CLK_ADC_ADC  (0) 
        // 0x00000001 [0]     CLK_SYS_CLOCKS (0) 
        io_ro_32 enabled[2];
    };
 
    _REG_(CLOCKS_INTR_OFFSET) // CLOCKS_INTR
    // Raw Interrupts
    // 0x00000001 [0]     CLK_SYS_RESUS (0) 
    io_ro_32 intr;
 
    _REG_(CLOCKS_INTE_OFFSET) // CLOCKS_INTE
    // Interrupt Enable
    // 0x00000001 [0]     CLK_SYS_RESUS (0) 
    io_rw_32 inte;
 
    _REG_(CLOCKS_INTF_OFFSET) // CLOCKS_INTF
    // Interrupt Force
    // 0x00000001 [0]     CLK_SYS_RESUS (0) 
    io_rw_32 intf;
 
    _REG_(CLOCKS_INTS_OFFSET) // CLOCKS_INTS
    // Interrupt status after masking & forcing
    // 0x00000001 [0]     CLK_SYS_RESUS (0) 
    io_ro_32 ints;
} clocks_hw_t;

#define clocks_hw ((clocks_hw_t *)CLOCKS_BASE)
static_assert(sizeof (clocks_hw_t) == 0x00c8, "");

#endif // _HARDWARE_STRUCTS_CLOCKS_H

