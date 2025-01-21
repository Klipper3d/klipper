// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
// =============================================================================
// Register block : RESETS
// Version        : 1
// Bus type       : apb
// =============================================================================
#ifndef _HARDWARE_REGS_RESETS_H
#define _HARDWARE_REGS_RESETS_H
// =============================================================================
// Register    : RESETS_RESET
#define RESETS_RESET_OFFSET _u(0x00000000)
#define RESETS_RESET_BITS   _u(0x1fffffff)
#define RESETS_RESET_RESET  _u(0x1fffffff)
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_USBCTRL
#define RESETS_RESET_USBCTRL_RESET  _u(0x1)
#define RESETS_RESET_USBCTRL_BITS   _u(0x10000000)
#define RESETS_RESET_USBCTRL_MSB    _u(28)
#define RESETS_RESET_USBCTRL_LSB    _u(28)
#define RESETS_RESET_USBCTRL_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_UART1
#define RESETS_RESET_UART1_RESET  _u(0x1)
#define RESETS_RESET_UART1_BITS   _u(0x08000000)
#define RESETS_RESET_UART1_MSB    _u(27)
#define RESETS_RESET_UART1_LSB    _u(27)
#define RESETS_RESET_UART1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_UART0
#define RESETS_RESET_UART0_RESET  _u(0x1)
#define RESETS_RESET_UART0_BITS   _u(0x04000000)
#define RESETS_RESET_UART0_MSB    _u(26)
#define RESETS_RESET_UART0_LSB    _u(26)
#define RESETS_RESET_UART0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_TRNG
#define RESETS_RESET_TRNG_RESET  _u(0x1)
#define RESETS_RESET_TRNG_BITS   _u(0x02000000)
#define RESETS_RESET_TRNG_MSB    _u(25)
#define RESETS_RESET_TRNG_LSB    _u(25)
#define RESETS_RESET_TRNG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_TIMER1
#define RESETS_RESET_TIMER1_RESET  _u(0x1)
#define RESETS_RESET_TIMER1_BITS   _u(0x01000000)
#define RESETS_RESET_TIMER1_MSB    _u(24)
#define RESETS_RESET_TIMER1_LSB    _u(24)
#define RESETS_RESET_TIMER1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_TIMER0
#define RESETS_RESET_TIMER0_RESET  _u(0x1)
#define RESETS_RESET_TIMER0_BITS   _u(0x00800000)
#define RESETS_RESET_TIMER0_MSB    _u(23)
#define RESETS_RESET_TIMER0_LSB    _u(23)
#define RESETS_RESET_TIMER0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_TBMAN
#define RESETS_RESET_TBMAN_RESET  _u(0x1)
#define RESETS_RESET_TBMAN_BITS   _u(0x00400000)
#define RESETS_RESET_TBMAN_MSB    _u(22)
#define RESETS_RESET_TBMAN_LSB    _u(22)
#define RESETS_RESET_TBMAN_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_SYSINFO
#define RESETS_RESET_SYSINFO_RESET  _u(0x1)
#define RESETS_RESET_SYSINFO_BITS   _u(0x00200000)
#define RESETS_RESET_SYSINFO_MSB    _u(21)
#define RESETS_RESET_SYSINFO_LSB    _u(21)
#define RESETS_RESET_SYSINFO_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_SYSCFG
#define RESETS_RESET_SYSCFG_RESET  _u(0x1)
#define RESETS_RESET_SYSCFG_BITS   _u(0x00100000)
#define RESETS_RESET_SYSCFG_MSB    _u(20)
#define RESETS_RESET_SYSCFG_LSB    _u(20)
#define RESETS_RESET_SYSCFG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_SPI1
#define RESETS_RESET_SPI1_RESET  _u(0x1)
#define RESETS_RESET_SPI1_BITS   _u(0x00080000)
#define RESETS_RESET_SPI1_MSB    _u(19)
#define RESETS_RESET_SPI1_LSB    _u(19)
#define RESETS_RESET_SPI1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_SPI0
#define RESETS_RESET_SPI0_RESET  _u(0x1)
#define RESETS_RESET_SPI0_BITS   _u(0x00040000)
#define RESETS_RESET_SPI0_MSB    _u(18)
#define RESETS_RESET_SPI0_LSB    _u(18)
#define RESETS_RESET_SPI0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_SHA256
#define RESETS_RESET_SHA256_RESET  _u(0x1)
#define RESETS_RESET_SHA256_BITS   _u(0x00020000)
#define RESETS_RESET_SHA256_MSB    _u(17)
#define RESETS_RESET_SHA256_LSB    _u(17)
#define RESETS_RESET_SHA256_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_PWM
#define RESETS_RESET_PWM_RESET  _u(0x1)
#define RESETS_RESET_PWM_BITS   _u(0x00010000)
#define RESETS_RESET_PWM_MSB    _u(16)
#define RESETS_RESET_PWM_LSB    _u(16)
#define RESETS_RESET_PWM_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_PLL_USB
#define RESETS_RESET_PLL_USB_RESET  _u(0x1)
#define RESETS_RESET_PLL_USB_BITS   _u(0x00008000)
#define RESETS_RESET_PLL_USB_MSB    _u(15)
#define RESETS_RESET_PLL_USB_LSB    _u(15)
#define RESETS_RESET_PLL_USB_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_PLL_SYS
#define RESETS_RESET_PLL_SYS_RESET  _u(0x1)
#define RESETS_RESET_PLL_SYS_BITS   _u(0x00004000)
#define RESETS_RESET_PLL_SYS_MSB    _u(14)
#define RESETS_RESET_PLL_SYS_LSB    _u(14)
#define RESETS_RESET_PLL_SYS_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_PIO2
#define RESETS_RESET_PIO2_RESET  _u(0x1)
#define RESETS_RESET_PIO2_BITS   _u(0x00002000)
#define RESETS_RESET_PIO2_MSB    _u(13)
#define RESETS_RESET_PIO2_LSB    _u(13)
#define RESETS_RESET_PIO2_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_PIO1
#define RESETS_RESET_PIO1_RESET  _u(0x1)
#define RESETS_RESET_PIO1_BITS   _u(0x00001000)
#define RESETS_RESET_PIO1_MSB    _u(12)
#define RESETS_RESET_PIO1_LSB    _u(12)
#define RESETS_RESET_PIO1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_PIO0
#define RESETS_RESET_PIO0_RESET  _u(0x1)
#define RESETS_RESET_PIO0_BITS   _u(0x00000800)
#define RESETS_RESET_PIO0_MSB    _u(11)
#define RESETS_RESET_PIO0_LSB    _u(11)
#define RESETS_RESET_PIO0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_PADS_QSPI
#define RESETS_RESET_PADS_QSPI_RESET  _u(0x1)
#define RESETS_RESET_PADS_QSPI_BITS   _u(0x00000400)
#define RESETS_RESET_PADS_QSPI_MSB    _u(10)
#define RESETS_RESET_PADS_QSPI_LSB    _u(10)
#define RESETS_RESET_PADS_QSPI_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_PADS_BANK0
#define RESETS_RESET_PADS_BANK0_RESET  _u(0x1)
#define RESETS_RESET_PADS_BANK0_BITS   _u(0x00000200)
#define RESETS_RESET_PADS_BANK0_MSB    _u(9)
#define RESETS_RESET_PADS_BANK0_LSB    _u(9)
#define RESETS_RESET_PADS_BANK0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_JTAG
#define RESETS_RESET_JTAG_RESET  _u(0x1)
#define RESETS_RESET_JTAG_BITS   _u(0x00000100)
#define RESETS_RESET_JTAG_MSB    _u(8)
#define RESETS_RESET_JTAG_LSB    _u(8)
#define RESETS_RESET_JTAG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_IO_QSPI
#define RESETS_RESET_IO_QSPI_RESET  _u(0x1)
#define RESETS_RESET_IO_QSPI_BITS   _u(0x00000080)
#define RESETS_RESET_IO_QSPI_MSB    _u(7)
#define RESETS_RESET_IO_QSPI_LSB    _u(7)
#define RESETS_RESET_IO_QSPI_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_IO_BANK0
#define RESETS_RESET_IO_BANK0_RESET  _u(0x1)
#define RESETS_RESET_IO_BANK0_BITS   _u(0x00000040)
#define RESETS_RESET_IO_BANK0_MSB    _u(6)
#define RESETS_RESET_IO_BANK0_LSB    _u(6)
#define RESETS_RESET_IO_BANK0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_I2C1
#define RESETS_RESET_I2C1_RESET  _u(0x1)
#define RESETS_RESET_I2C1_BITS   _u(0x00000020)
#define RESETS_RESET_I2C1_MSB    _u(5)
#define RESETS_RESET_I2C1_LSB    _u(5)
#define RESETS_RESET_I2C1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_I2C0
#define RESETS_RESET_I2C0_RESET  _u(0x1)
#define RESETS_RESET_I2C0_BITS   _u(0x00000010)
#define RESETS_RESET_I2C0_MSB    _u(4)
#define RESETS_RESET_I2C0_LSB    _u(4)
#define RESETS_RESET_I2C0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_HSTX
#define RESETS_RESET_HSTX_RESET  _u(0x1)
#define RESETS_RESET_HSTX_BITS   _u(0x00000008)
#define RESETS_RESET_HSTX_MSB    _u(3)
#define RESETS_RESET_HSTX_LSB    _u(3)
#define RESETS_RESET_HSTX_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_DMA
#define RESETS_RESET_DMA_RESET  _u(0x1)
#define RESETS_RESET_DMA_BITS   _u(0x00000004)
#define RESETS_RESET_DMA_MSB    _u(2)
#define RESETS_RESET_DMA_LSB    _u(2)
#define RESETS_RESET_DMA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_BUSCTRL
#define RESETS_RESET_BUSCTRL_RESET  _u(0x1)
#define RESETS_RESET_BUSCTRL_BITS   _u(0x00000002)
#define RESETS_RESET_BUSCTRL_MSB    _u(1)
#define RESETS_RESET_BUSCTRL_LSB    _u(1)
#define RESETS_RESET_BUSCTRL_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_ADC
#define RESETS_RESET_ADC_RESET  _u(0x1)
#define RESETS_RESET_ADC_BITS   _u(0x00000001)
#define RESETS_RESET_ADC_MSB    _u(0)
#define RESETS_RESET_ADC_LSB    _u(0)
#define RESETS_RESET_ADC_ACCESS "RW"
// =============================================================================
// Register    : RESETS_WDSEL
#define RESETS_WDSEL_OFFSET _u(0x00000004)
#define RESETS_WDSEL_BITS   _u(0x1fffffff)
#define RESETS_WDSEL_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : RESETS_WDSEL_USBCTRL
#define RESETS_WDSEL_USBCTRL_RESET  _u(0x0)
#define RESETS_WDSEL_USBCTRL_BITS   _u(0x10000000)
#define RESETS_WDSEL_USBCTRL_MSB    _u(28)
#define RESETS_WDSEL_USBCTRL_LSB    _u(28)
#define RESETS_WDSEL_USBCTRL_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RESETS_WDSEL_UART1
#define RESETS_WDSEL_UART1_RESET  _u(0x0)
#define RESETS_WDSEL_UART1_BITS   _u(0x08000000)
#define RESETS_WDSEL_UART1_MSB    _u(27)
#define RESETS_WDSEL_UART1_LSB    _u(27)
#define RESETS_WDSEL_UART1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RESETS_WDSEL_UART0
#define RESETS_WDSEL_UART0_RESET  _u(0x0)
#define RESETS_WDSEL_UART0_BITS   _u(0x04000000)
#define RESETS_WDSEL_UART0_MSB    _u(26)
#define RESETS_WDSEL_UART0_LSB    _u(26)
#define RESETS_WDSEL_UART0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RESETS_WDSEL_TRNG
#define RESETS_WDSEL_TRNG_RESET  _u(0x0)
#define RESETS_WDSEL_TRNG_BITS   _u(0x02000000)
#define RESETS_WDSEL_TRNG_MSB    _u(25)
#define RESETS_WDSEL_TRNG_LSB    _u(25)
#define RESETS_WDSEL_TRNG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RESETS_WDSEL_TIMER1
#define RESETS_WDSEL_TIMER1_RESET  _u(0x0)
#define RESETS_WDSEL_TIMER1_BITS   _u(0x01000000)
#define RESETS_WDSEL_TIMER1_MSB    _u(24)
#define RESETS_WDSEL_TIMER1_LSB    _u(24)
#define RESETS_WDSEL_TIMER1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RESETS_WDSEL_TIMER0
#define RESETS_WDSEL_TIMER0_RESET  _u(0x0)
#define RESETS_WDSEL_TIMER0_BITS   _u(0x00800000)
#define RESETS_WDSEL_TIMER0_MSB    _u(23)
#define RESETS_WDSEL_TIMER0_LSB    _u(23)
#define RESETS_WDSEL_TIMER0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RESETS_WDSEL_TBMAN
#define RESETS_WDSEL_TBMAN_RESET  _u(0x0)
#define RESETS_WDSEL_TBMAN_BITS   _u(0x00400000)
#define RESETS_WDSEL_TBMAN_MSB    _u(22)
#define RESETS_WDSEL_TBMAN_LSB    _u(22)
#define RESETS_WDSEL_TBMAN_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RESETS_WDSEL_SYSINFO
#define RESETS_WDSEL_SYSINFO_RESET  _u(0x0)
#define RESETS_WDSEL_SYSINFO_BITS   _u(0x00200000)
#define RESETS_WDSEL_SYSINFO_MSB    _u(21)
#define RESETS_WDSEL_SYSINFO_LSB    _u(21)
#define RESETS_WDSEL_SYSINFO_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RESETS_WDSEL_SYSCFG
#define RESETS_WDSEL_SYSCFG_RESET  _u(0x0)
#define RESETS_WDSEL_SYSCFG_BITS   _u(0x00100000)
#define RESETS_WDSEL_SYSCFG_MSB    _u(20)
#define RESETS_WDSEL_SYSCFG_LSB    _u(20)
#define RESETS_WDSEL_SYSCFG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RESETS_WDSEL_SPI1
#define RESETS_WDSEL_SPI1_RESET  _u(0x0)
#define RESETS_WDSEL_SPI1_BITS   _u(0x00080000)
#define RESETS_WDSEL_SPI1_MSB    _u(19)
#define RESETS_WDSEL_SPI1_LSB    _u(19)
#define RESETS_WDSEL_SPI1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RESETS_WDSEL_SPI0
#define RESETS_WDSEL_SPI0_RESET  _u(0x0)
#define RESETS_WDSEL_SPI0_BITS   _u(0x00040000)
#define RESETS_WDSEL_SPI0_MSB    _u(18)
#define RESETS_WDSEL_SPI0_LSB    _u(18)
#define RESETS_WDSEL_SPI0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RESETS_WDSEL_SHA256
#define RESETS_WDSEL_SHA256_RESET  _u(0x0)
#define RESETS_WDSEL_SHA256_BITS   _u(0x00020000)
#define RESETS_WDSEL_SHA256_MSB    _u(17)
#define RESETS_WDSEL_SHA256_LSB    _u(17)
#define RESETS_WDSEL_SHA256_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RESETS_WDSEL_PWM
#define RESETS_WDSEL_PWM_RESET  _u(0x0)
#define RESETS_WDSEL_PWM_BITS   _u(0x00010000)
#define RESETS_WDSEL_PWM_MSB    _u(16)
#define RESETS_WDSEL_PWM_LSB    _u(16)
#define RESETS_WDSEL_PWM_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RESETS_WDSEL_PLL_USB
#define RESETS_WDSEL_PLL_USB_RESET  _u(0x0)
#define RESETS_WDSEL_PLL_USB_BITS   _u(0x00008000)
#define RESETS_WDSEL_PLL_USB_MSB    _u(15)
#define RESETS_WDSEL_PLL_USB_LSB    _u(15)
#define RESETS_WDSEL_PLL_USB_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RESETS_WDSEL_PLL_SYS
#define RESETS_WDSEL_PLL_SYS_RESET  _u(0x0)
#define RESETS_WDSEL_PLL_SYS_BITS   _u(0x00004000)
#define RESETS_WDSEL_PLL_SYS_MSB    _u(14)
#define RESETS_WDSEL_PLL_SYS_LSB    _u(14)
#define RESETS_WDSEL_PLL_SYS_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RESETS_WDSEL_PIO2
#define RESETS_WDSEL_PIO2_RESET  _u(0x0)
#define RESETS_WDSEL_PIO2_BITS   _u(0x00002000)
#define RESETS_WDSEL_PIO2_MSB    _u(13)
#define RESETS_WDSEL_PIO2_LSB    _u(13)
#define RESETS_WDSEL_PIO2_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RESETS_WDSEL_PIO1
#define RESETS_WDSEL_PIO1_RESET  _u(0x0)
#define RESETS_WDSEL_PIO1_BITS   _u(0x00001000)
#define RESETS_WDSEL_PIO1_MSB    _u(12)
#define RESETS_WDSEL_PIO1_LSB    _u(12)
#define RESETS_WDSEL_PIO1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RESETS_WDSEL_PIO0
#define RESETS_WDSEL_PIO0_RESET  _u(0x0)
#define RESETS_WDSEL_PIO0_BITS   _u(0x00000800)
#define RESETS_WDSEL_PIO0_MSB    _u(11)
#define RESETS_WDSEL_PIO0_LSB    _u(11)
#define RESETS_WDSEL_PIO0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RESETS_WDSEL_PADS_QSPI
#define RESETS_WDSEL_PADS_QSPI_RESET  _u(0x0)
#define RESETS_WDSEL_PADS_QSPI_BITS   _u(0x00000400)
#define RESETS_WDSEL_PADS_QSPI_MSB    _u(10)
#define RESETS_WDSEL_PADS_QSPI_LSB    _u(10)
#define RESETS_WDSEL_PADS_QSPI_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RESETS_WDSEL_PADS_BANK0
#define RESETS_WDSEL_PADS_BANK0_RESET  _u(0x0)
#define RESETS_WDSEL_PADS_BANK0_BITS   _u(0x00000200)
#define RESETS_WDSEL_PADS_BANK0_MSB    _u(9)
#define RESETS_WDSEL_PADS_BANK0_LSB    _u(9)
#define RESETS_WDSEL_PADS_BANK0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RESETS_WDSEL_JTAG
#define RESETS_WDSEL_JTAG_RESET  _u(0x0)
#define RESETS_WDSEL_JTAG_BITS   _u(0x00000100)
#define RESETS_WDSEL_JTAG_MSB    _u(8)
#define RESETS_WDSEL_JTAG_LSB    _u(8)
#define RESETS_WDSEL_JTAG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RESETS_WDSEL_IO_QSPI
#define RESETS_WDSEL_IO_QSPI_RESET  _u(0x0)
#define RESETS_WDSEL_IO_QSPI_BITS   _u(0x00000080)
#define RESETS_WDSEL_IO_QSPI_MSB    _u(7)
#define RESETS_WDSEL_IO_QSPI_LSB    _u(7)
#define RESETS_WDSEL_IO_QSPI_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RESETS_WDSEL_IO_BANK0
#define RESETS_WDSEL_IO_BANK0_RESET  _u(0x0)
#define RESETS_WDSEL_IO_BANK0_BITS   _u(0x00000040)
#define RESETS_WDSEL_IO_BANK0_MSB    _u(6)
#define RESETS_WDSEL_IO_BANK0_LSB    _u(6)
#define RESETS_WDSEL_IO_BANK0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RESETS_WDSEL_I2C1
#define RESETS_WDSEL_I2C1_RESET  _u(0x0)
#define RESETS_WDSEL_I2C1_BITS   _u(0x00000020)
#define RESETS_WDSEL_I2C1_MSB    _u(5)
#define RESETS_WDSEL_I2C1_LSB    _u(5)
#define RESETS_WDSEL_I2C1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RESETS_WDSEL_I2C0
#define RESETS_WDSEL_I2C0_RESET  _u(0x0)
#define RESETS_WDSEL_I2C0_BITS   _u(0x00000010)
#define RESETS_WDSEL_I2C0_MSB    _u(4)
#define RESETS_WDSEL_I2C0_LSB    _u(4)
#define RESETS_WDSEL_I2C0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RESETS_WDSEL_HSTX
#define RESETS_WDSEL_HSTX_RESET  _u(0x0)
#define RESETS_WDSEL_HSTX_BITS   _u(0x00000008)
#define RESETS_WDSEL_HSTX_MSB    _u(3)
#define RESETS_WDSEL_HSTX_LSB    _u(3)
#define RESETS_WDSEL_HSTX_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RESETS_WDSEL_DMA
#define RESETS_WDSEL_DMA_RESET  _u(0x0)
#define RESETS_WDSEL_DMA_BITS   _u(0x00000004)
#define RESETS_WDSEL_DMA_MSB    _u(2)
#define RESETS_WDSEL_DMA_LSB    _u(2)
#define RESETS_WDSEL_DMA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RESETS_WDSEL_BUSCTRL
#define RESETS_WDSEL_BUSCTRL_RESET  _u(0x0)
#define RESETS_WDSEL_BUSCTRL_BITS   _u(0x00000002)
#define RESETS_WDSEL_BUSCTRL_MSB    _u(1)
#define RESETS_WDSEL_BUSCTRL_LSB    _u(1)
#define RESETS_WDSEL_BUSCTRL_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RESETS_WDSEL_ADC
#define RESETS_WDSEL_ADC_RESET  _u(0x0)
#define RESETS_WDSEL_ADC_BITS   _u(0x00000001)
#define RESETS_WDSEL_ADC_MSB    _u(0)
#define RESETS_WDSEL_ADC_LSB    _u(0)
#define RESETS_WDSEL_ADC_ACCESS "RW"
// =============================================================================
// Register    : RESETS_RESET_DONE
#define RESETS_RESET_DONE_OFFSET _u(0x00000008)
#define RESETS_RESET_DONE_BITS   _u(0x1fffffff)
#define RESETS_RESET_DONE_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_DONE_USBCTRL
#define RESETS_RESET_DONE_USBCTRL_RESET  _u(0x0)
#define RESETS_RESET_DONE_USBCTRL_BITS   _u(0x10000000)
#define RESETS_RESET_DONE_USBCTRL_MSB    _u(28)
#define RESETS_RESET_DONE_USBCTRL_LSB    _u(28)
#define RESETS_RESET_DONE_USBCTRL_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_DONE_UART1
#define RESETS_RESET_DONE_UART1_RESET  _u(0x0)
#define RESETS_RESET_DONE_UART1_BITS   _u(0x08000000)
#define RESETS_RESET_DONE_UART1_MSB    _u(27)
#define RESETS_RESET_DONE_UART1_LSB    _u(27)
#define RESETS_RESET_DONE_UART1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_DONE_UART0
#define RESETS_RESET_DONE_UART0_RESET  _u(0x0)
#define RESETS_RESET_DONE_UART0_BITS   _u(0x04000000)
#define RESETS_RESET_DONE_UART0_MSB    _u(26)
#define RESETS_RESET_DONE_UART0_LSB    _u(26)
#define RESETS_RESET_DONE_UART0_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_DONE_TRNG
#define RESETS_RESET_DONE_TRNG_RESET  _u(0x0)
#define RESETS_RESET_DONE_TRNG_BITS   _u(0x02000000)
#define RESETS_RESET_DONE_TRNG_MSB    _u(25)
#define RESETS_RESET_DONE_TRNG_LSB    _u(25)
#define RESETS_RESET_DONE_TRNG_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_DONE_TIMER1
#define RESETS_RESET_DONE_TIMER1_RESET  _u(0x0)
#define RESETS_RESET_DONE_TIMER1_BITS   _u(0x01000000)
#define RESETS_RESET_DONE_TIMER1_MSB    _u(24)
#define RESETS_RESET_DONE_TIMER1_LSB    _u(24)
#define RESETS_RESET_DONE_TIMER1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_DONE_TIMER0
#define RESETS_RESET_DONE_TIMER0_RESET  _u(0x0)
#define RESETS_RESET_DONE_TIMER0_BITS   _u(0x00800000)
#define RESETS_RESET_DONE_TIMER0_MSB    _u(23)
#define RESETS_RESET_DONE_TIMER0_LSB    _u(23)
#define RESETS_RESET_DONE_TIMER0_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_DONE_TBMAN
#define RESETS_RESET_DONE_TBMAN_RESET  _u(0x0)
#define RESETS_RESET_DONE_TBMAN_BITS   _u(0x00400000)
#define RESETS_RESET_DONE_TBMAN_MSB    _u(22)
#define RESETS_RESET_DONE_TBMAN_LSB    _u(22)
#define RESETS_RESET_DONE_TBMAN_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_DONE_SYSINFO
#define RESETS_RESET_DONE_SYSINFO_RESET  _u(0x0)
#define RESETS_RESET_DONE_SYSINFO_BITS   _u(0x00200000)
#define RESETS_RESET_DONE_SYSINFO_MSB    _u(21)
#define RESETS_RESET_DONE_SYSINFO_LSB    _u(21)
#define RESETS_RESET_DONE_SYSINFO_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_DONE_SYSCFG
#define RESETS_RESET_DONE_SYSCFG_RESET  _u(0x0)
#define RESETS_RESET_DONE_SYSCFG_BITS   _u(0x00100000)
#define RESETS_RESET_DONE_SYSCFG_MSB    _u(20)
#define RESETS_RESET_DONE_SYSCFG_LSB    _u(20)
#define RESETS_RESET_DONE_SYSCFG_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_DONE_SPI1
#define RESETS_RESET_DONE_SPI1_RESET  _u(0x0)
#define RESETS_RESET_DONE_SPI1_BITS   _u(0x00080000)
#define RESETS_RESET_DONE_SPI1_MSB    _u(19)
#define RESETS_RESET_DONE_SPI1_LSB    _u(19)
#define RESETS_RESET_DONE_SPI1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_DONE_SPI0
#define RESETS_RESET_DONE_SPI0_RESET  _u(0x0)
#define RESETS_RESET_DONE_SPI0_BITS   _u(0x00040000)
#define RESETS_RESET_DONE_SPI0_MSB    _u(18)
#define RESETS_RESET_DONE_SPI0_LSB    _u(18)
#define RESETS_RESET_DONE_SPI0_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_DONE_SHA256
#define RESETS_RESET_DONE_SHA256_RESET  _u(0x0)
#define RESETS_RESET_DONE_SHA256_BITS   _u(0x00020000)
#define RESETS_RESET_DONE_SHA256_MSB    _u(17)
#define RESETS_RESET_DONE_SHA256_LSB    _u(17)
#define RESETS_RESET_DONE_SHA256_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_DONE_PWM
#define RESETS_RESET_DONE_PWM_RESET  _u(0x0)
#define RESETS_RESET_DONE_PWM_BITS   _u(0x00010000)
#define RESETS_RESET_DONE_PWM_MSB    _u(16)
#define RESETS_RESET_DONE_PWM_LSB    _u(16)
#define RESETS_RESET_DONE_PWM_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_DONE_PLL_USB
#define RESETS_RESET_DONE_PLL_USB_RESET  _u(0x0)
#define RESETS_RESET_DONE_PLL_USB_BITS   _u(0x00008000)
#define RESETS_RESET_DONE_PLL_USB_MSB    _u(15)
#define RESETS_RESET_DONE_PLL_USB_LSB    _u(15)
#define RESETS_RESET_DONE_PLL_USB_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_DONE_PLL_SYS
#define RESETS_RESET_DONE_PLL_SYS_RESET  _u(0x0)
#define RESETS_RESET_DONE_PLL_SYS_BITS   _u(0x00004000)
#define RESETS_RESET_DONE_PLL_SYS_MSB    _u(14)
#define RESETS_RESET_DONE_PLL_SYS_LSB    _u(14)
#define RESETS_RESET_DONE_PLL_SYS_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_DONE_PIO2
#define RESETS_RESET_DONE_PIO2_RESET  _u(0x0)
#define RESETS_RESET_DONE_PIO2_BITS   _u(0x00002000)
#define RESETS_RESET_DONE_PIO2_MSB    _u(13)
#define RESETS_RESET_DONE_PIO2_LSB    _u(13)
#define RESETS_RESET_DONE_PIO2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_DONE_PIO1
#define RESETS_RESET_DONE_PIO1_RESET  _u(0x0)
#define RESETS_RESET_DONE_PIO1_BITS   _u(0x00001000)
#define RESETS_RESET_DONE_PIO1_MSB    _u(12)
#define RESETS_RESET_DONE_PIO1_LSB    _u(12)
#define RESETS_RESET_DONE_PIO1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_DONE_PIO0
#define RESETS_RESET_DONE_PIO0_RESET  _u(0x0)
#define RESETS_RESET_DONE_PIO0_BITS   _u(0x00000800)
#define RESETS_RESET_DONE_PIO0_MSB    _u(11)
#define RESETS_RESET_DONE_PIO0_LSB    _u(11)
#define RESETS_RESET_DONE_PIO0_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_DONE_PADS_QSPI
#define RESETS_RESET_DONE_PADS_QSPI_RESET  _u(0x0)
#define RESETS_RESET_DONE_PADS_QSPI_BITS   _u(0x00000400)
#define RESETS_RESET_DONE_PADS_QSPI_MSB    _u(10)
#define RESETS_RESET_DONE_PADS_QSPI_LSB    _u(10)
#define RESETS_RESET_DONE_PADS_QSPI_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_DONE_PADS_BANK0
#define RESETS_RESET_DONE_PADS_BANK0_RESET  _u(0x0)
#define RESETS_RESET_DONE_PADS_BANK0_BITS   _u(0x00000200)
#define RESETS_RESET_DONE_PADS_BANK0_MSB    _u(9)
#define RESETS_RESET_DONE_PADS_BANK0_LSB    _u(9)
#define RESETS_RESET_DONE_PADS_BANK0_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_DONE_JTAG
#define RESETS_RESET_DONE_JTAG_RESET  _u(0x0)
#define RESETS_RESET_DONE_JTAG_BITS   _u(0x00000100)
#define RESETS_RESET_DONE_JTAG_MSB    _u(8)
#define RESETS_RESET_DONE_JTAG_LSB    _u(8)
#define RESETS_RESET_DONE_JTAG_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_DONE_IO_QSPI
#define RESETS_RESET_DONE_IO_QSPI_RESET  _u(0x0)
#define RESETS_RESET_DONE_IO_QSPI_BITS   _u(0x00000080)
#define RESETS_RESET_DONE_IO_QSPI_MSB    _u(7)
#define RESETS_RESET_DONE_IO_QSPI_LSB    _u(7)
#define RESETS_RESET_DONE_IO_QSPI_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_DONE_IO_BANK0
#define RESETS_RESET_DONE_IO_BANK0_RESET  _u(0x0)
#define RESETS_RESET_DONE_IO_BANK0_BITS   _u(0x00000040)
#define RESETS_RESET_DONE_IO_BANK0_MSB    _u(6)
#define RESETS_RESET_DONE_IO_BANK0_LSB    _u(6)
#define RESETS_RESET_DONE_IO_BANK0_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_DONE_I2C1
#define RESETS_RESET_DONE_I2C1_RESET  _u(0x0)
#define RESETS_RESET_DONE_I2C1_BITS   _u(0x00000020)
#define RESETS_RESET_DONE_I2C1_MSB    _u(5)
#define RESETS_RESET_DONE_I2C1_LSB    _u(5)
#define RESETS_RESET_DONE_I2C1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_DONE_I2C0
#define RESETS_RESET_DONE_I2C0_RESET  _u(0x0)
#define RESETS_RESET_DONE_I2C0_BITS   _u(0x00000010)
#define RESETS_RESET_DONE_I2C0_MSB    _u(4)
#define RESETS_RESET_DONE_I2C0_LSB    _u(4)
#define RESETS_RESET_DONE_I2C0_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_DONE_HSTX
#define RESETS_RESET_DONE_HSTX_RESET  _u(0x0)
#define RESETS_RESET_DONE_HSTX_BITS   _u(0x00000008)
#define RESETS_RESET_DONE_HSTX_MSB    _u(3)
#define RESETS_RESET_DONE_HSTX_LSB    _u(3)
#define RESETS_RESET_DONE_HSTX_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_DONE_DMA
#define RESETS_RESET_DONE_DMA_RESET  _u(0x0)
#define RESETS_RESET_DONE_DMA_BITS   _u(0x00000004)
#define RESETS_RESET_DONE_DMA_MSB    _u(2)
#define RESETS_RESET_DONE_DMA_LSB    _u(2)
#define RESETS_RESET_DONE_DMA_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_DONE_BUSCTRL
#define RESETS_RESET_DONE_BUSCTRL_RESET  _u(0x0)
#define RESETS_RESET_DONE_BUSCTRL_BITS   _u(0x00000002)
#define RESETS_RESET_DONE_BUSCTRL_MSB    _u(1)
#define RESETS_RESET_DONE_BUSCTRL_LSB    _u(1)
#define RESETS_RESET_DONE_BUSCTRL_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RESETS_RESET_DONE_ADC
#define RESETS_RESET_DONE_ADC_RESET  _u(0x0)
#define RESETS_RESET_DONE_ADC_BITS   _u(0x00000001)
#define RESETS_RESET_DONE_ADC_MSB    _u(0)
#define RESETS_RESET_DONE_ADC_LSB    _u(0)
#define RESETS_RESET_DONE_ADC_ACCESS "RO"
// =============================================================================
#endif // _HARDWARE_REGS_RESETS_H

