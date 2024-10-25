// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
// =============================================================================
// Register block : IO_QSPI
// Version        : 1
// Bus type       : apb
// =============================================================================
#ifndef _HARDWARE_REGS_IO_QSPI_H
#define _HARDWARE_REGS_IO_QSPI_H
// =============================================================================
// Register    : IO_QSPI_USBPHY_DP_STATUS
#define IO_QSPI_USBPHY_DP_STATUS_OFFSET _u(0x00000000)
#define IO_QSPI_USBPHY_DP_STATUS_BITS   _u(0x04022200)
#define IO_QSPI_USBPHY_DP_STATUS_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_USBPHY_DP_STATUS_IRQTOPROC
// Description : interrupt to processors, after override is applied
#define IO_QSPI_USBPHY_DP_STATUS_IRQTOPROC_RESET  _u(0x0)
#define IO_QSPI_USBPHY_DP_STATUS_IRQTOPROC_BITS   _u(0x04000000)
#define IO_QSPI_USBPHY_DP_STATUS_IRQTOPROC_MSB    _u(26)
#define IO_QSPI_USBPHY_DP_STATUS_IRQTOPROC_LSB    _u(26)
#define IO_QSPI_USBPHY_DP_STATUS_IRQTOPROC_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_USBPHY_DP_STATUS_INFROMPAD
// Description : input signal from pad, before filtering and override are
//               applied
#define IO_QSPI_USBPHY_DP_STATUS_INFROMPAD_RESET  _u(0x0)
#define IO_QSPI_USBPHY_DP_STATUS_INFROMPAD_BITS   _u(0x00020000)
#define IO_QSPI_USBPHY_DP_STATUS_INFROMPAD_MSB    _u(17)
#define IO_QSPI_USBPHY_DP_STATUS_INFROMPAD_LSB    _u(17)
#define IO_QSPI_USBPHY_DP_STATUS_INFROMPAD_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_USBPHY_DP_STATUS_OETOPAD
// Description : output enable to pad after register override is applied
#define IO_QSPI_USBPHY_DP_STATUS_OETOPAD_RESET  _u(0x0)
#define IO_QSPI_USBPHY_DP_STATUS_OETOPAD_BITS   _u(0x00002000)
#define IO_QSPI_USBPHY_DP_STATUS_OETOPAD_MSB    _u(13)
#define IO_QSPI_USBPHY_DP_STATUS_OETOPAD_LSB    _u(13)
#define IO_QSPI_USBPHY_DP_STATUS_OETOPAD_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_USBPHY_DP_STATUS_OUTTOPAD
// Description : output signal to pad after register override is applied
#define IO_QSPI_USBPHY_DP_STATUS_OUTTOPAD_RESET  _u(0x0)
#define IO_QSPI_USBPHY_DP_STATUS_OUTTOPAD_BITS   _u(0x00000200)
#define IO_QSPI_USBPHY_DP_STATUS_OUTTOPAD_MSB    _u(9)
#define IO_QSPI_USBPHY_DP_STATUS_OUTTOPAD_LSB    _u(9)
#define IO_QSPI_USBPHY_DP_STATUS_OUTTOPAD_ACCESS "RO"
// =============================================================================
// Register    : IO_QSPI_USBPHY_DP_CTRL
#define IO_QSPI_USBPHY_DP_CTRL_OFFSET _u(0x00000004)
#define IO_QSPI_USBPHY_DP_CTRL_BITS   _u(0x3003f01f)
#define IO_QSPI_USBPHY_DP_CTRL_RESET  _u(0x0000001f)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_USBPHY_DP_CTRL_IRQOVER
//               0x0 -> don't invert the interrupt
//               0x1 -> invert the interrupt
//               0x2 -> drive interrupt low
//               0x3 -> drive interrupt high
#define IO_QSPI_USBPHY_DP_CTRL_IRQOVER_RESET  _u(0x0)
#define IO_QSPI_USBPHY_DP_CTRL_IRQOVER_BITS   _u(0x30000000)
#define IO_QSPI_USBPHY_DP_CTRL_IRQOVER_MSB    _u(29)
#define IO_QSPI_USBPHY_DP_CTRL_IRQOVER_LSB    _u(28)
#define IO_QSPI_USBPHY_DP_CTRL_IRQOVER_ACCESS "RW"
#define IO_QSPI_USBPHY_DP_CTRL_IRQOVER_VALUE_NORMAL _u(0x0)
#define IO_QSPI_USBPHY_DP_CTRL_IRQOVER_VALUE_INVERT _u(0x1)
#define IO_QSPI_USBPHY_DP_CTRL_IRQOVER_VALUE_LOW _u(0x2)
#define IO_QSPI_USBPHY_DP_CTRL_IRQOVER_VALUE_HIGH _u(0x3)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_USBPHY_DP_CTRL_INOVER
//               0x0 -> don't invert the peri input
//               0x1 -> invert the peri input
//               0x2 -> drive peri input low
//               0x3 -> drive peri input high
#define IO_QSPI_USBPHY_DP_CTRL_INOVER_RESET  _u(0x0)
#define IO_QSPI_USBPHY_DP_CTRL_INOVER_BITS   _u(0x00030000)
#define IO_QSPI_USBPHY_DP_CTRL_INOVER_MSB    _u(17)
#define IO_QSPI_USBPHY_DP_CTRL_INOVER_LSB    _u(16)
#define IO_QSPI_USBPHY_DP_CTRL_INOVER_ACCESS "RW"
#define IO_QSPI_USBPHY_DP_CTRL_INOVER_VALUE_NORMAL _u(0x0)
#define IO_QSPI_USBPHY_DP_CTRL_INOVER_VALUE_INVERT _u(0x1)
#define IO_QSPI_USBPHY_DP_CTRL_INOVER_VALUE_LOW _u(0x2)
#define IO_QSPI_USBPHY_DP_CTRL_INOVER_VALUE_HIGH _u(0x3)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_USBPHY_DP_CTRL_OEOVER
//               0x0 -> drive output enable from peripheral signal selected by funcsel
//               0x1 -> drive output enable from inverse of peripheral signal selected by funcsel
//               0x2 -> disable output
//               0x3 -> enable output
#define IO_QSPI_USBPHY_DP_CTRL_OEOVER_RESET  _u(0x0)
#define IO_QSPI_USBPHY_DP_CTRL_OEOVER_BITS   _u(0x0000c000)
#define IO_QSPI_USBPHY_DP_CTRL_OEOVER_MSB    _u(15)
#define IO_QSPI_USBPHY_DP_CTRL_OEOVER_LSB    _u(14)
#define IO_QSPI_USBPHY_DP_CTRL_OEOVER_ACCESS "RW"
#define IO_QSPI_USBPHY_DP_CTRL_OEOVER_VALUE_NORMAL _u(0x0)
#define IO_QSPI_USBPHY_DP_CTRL_OEOVER_VALUE_INVERT _u(0x1)
#define IO_QSPI_USBPHY_DP_CTRL_OEOVER_VALUE_DISABLE _u(0x2)
#define IO_QSPI_USBPHY_DP_CTRL_OEOVER_VALUE_ENABLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_USBPHY_DP_CTRL_OUTOVER
//               0x0 -> drive output from peripheral signal selected by funcsel
//               0x1 -> drive output from inverse of peripheral signal selected by funcsel
//               0x2 -> drive output low
//               0x3 -> drive output high
#define IO_QSPI_USBPHY_DP_CTRL_OUTOVER_RESET  _u(0x0)
#define IO_QSPI_USBPHY_DP_CTRL_OUTOVER_BITS   _u(0x00003000)
#define IO_QSPI_USBPHY_DP_CTRL_OUTOVER_MSB    _u(13)
#define IO_QSPI_USBPHY_DP_CTRL_OUTOVER_LSB    _u(12)
#define IO_QSPI_USBPHY_DP_CTRL_OUTOVER_ACCESS "RW"
#define IO_QSPI_USBPHY_DP_CTRL_OUTOVER_VALUE_NORMAL _u(0x0)
#define IO_QSPI_USBPHY_DP_CTRL_OUTOVER_VALUE_INVERT _u(0x1)
#define IO_QSPI_USBPHY_DP_CTRL_OUTOVER_VALUE_LOW _u(0x2)
#define IO_QSPI_USBPHY_DP_CTRL_OUTOVER_VALUE_HIGH _u(0x3)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_USBPHY_DP_CTRL_FUNCSEL
// Description : 0-31 -> selects pin function according to the gpio table
//               31 == NULL
//               0x02 -> uart1_tx
//               0x03 -> i2c0_sda
//               0x05 -> siob_proc_56
//               0x1f -> null
#define IO_QSPI_USBPHY_DP_CTRL_FUNCSEL_RESET  _u(0x1f)
#define IO_QSPI_USBPHY_DP_CTRL_FUNCSEL_BITS   _u(0x0000001f)
#define IO_QSPI_USBPHY_DP_CTRL_FUNCSEL_MSB    _u(4)
#define IO_QSPI_USBPHY_DP_CTRL_FUNCSEL_LSB    _u(0)
#define IO_QSPI_USBPHY_DP_CTRL_FUNCSEL_ACCESS "RW"
#define IO_QSPI_USBPHY_DP_CTRL_FUNCSEL_VALUE_UART1_TX _u(0x02)
#define IO_QSPI_USBPHY_DP_CTRL_FUNCSEL_VALUE_I2C0_SDA _u(0x03)
#define IO_QSPI_USBPHY_DP_CTRL_FUNCSEL_VALUE_SIOB_PROC_56 _u(0x05)
#define IO_QSPI_USBPHY_DP_CTRL_FUNCSEL_VALUE_NULL _u(0x1f)
// =============================================================================
// Register    : IO_QSPI_USBPHY_DM_STATUS
#define IO_QSPI_USBPHY_DM_STATUS_OFFSET _u(0x00000008)
#define IO_QSPI_USBPHY_DM_STATUS_BITS   _u(0x04022200)
#define IO_QSPI_USBPHY_DM_STATUS_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_USBPHY_DM_STATUS_IRQTOPROC
// Description : interrupt to processors, after override is applied
#define IO_QSPI_USBPHY_DM_STATUS_IRQTOPROC_RESET  _u(0x0)
#define IO_QSPI_USBPHY_DM_STATUS_IRQTOPROC_BITS   _u(0x04000000)
#define IO_QSPI_USBPHY_DM_STATUS_IRQTOPROC_MSB    _u(26)
#define IO_QSPI_USBPHY_DM_STATUS_IRQTOPROC_LSB    _u(26)
#define IO_QSPI_USBPHY_DM_STATUS_IRQTOPROC_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_USBPHY_DM_STATUS_INFROMPAD
// Description : input signal from pad, before filtering and override are
//               applied
#define IO_QSPI_USBPHY_DM_STATUS_INFROMPAD_RESET  _u(0x0)
#define IO_QSPI_USBPHY_DM_STATUS_INFROMPAD_BITS   _u(0x00020000)
#define IO_QSPI_USBPHY_DM_STATUS_INFROMPAD_MSB    _u(17)
#define IO_QSPI_USBPHY_DM_STATUS_INFROMPAD_LSB    _u(17)
#define IO_QSPI_USBPHY_DM_STATUS_INFROMPAD_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_USBPHY_DM_STATUS_OETOPAD
// Description : output enable to pad after register override is applied
#define IO_QSPI_USBPHY_DM_STATUS_OETOPAD_RESET  _u(0x0)
#define IO_QSPI_USBPHY_DM_STATUS_OETOPAD_BITS   _u(0x00002000)
#define IO_QSPI_USBPHY_DM_STATUS_OETOPAD_MSB    _u(13)
#define IO_QSPI_USBPHY_DM_STATUS_OETOPAD_LSB    _u(13)
#define IO_QSPI_USBPHY_DM_STATUS_OETOPAD_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_USBPHY_DM_STATUS_OUTTOPAD
// Description : output signal to pad after register override is applied
#define IO_QSPI_USBPHY_DM_STATUS_OUTTOPAD_RESET  _u(0x0)
#define IO_QSPI_USBPHY_DM_STATUS_OUTTOPAD_BITS   _u(0x00000200)
#define IO_QSPI_USBPHY_DM_STATUS_OUTTOPAD_MSB    _u(9)
#define IO_QSPI_USBPHY_DM_STATUS_OUTTOPAD_LSB    _u(9)
#define IO_QSPI_USBPHY_DM_STATUS_OUTTOPAD_ACCESS "RO"
// =============================================================================
// Register    : IO_QSPI_USBPHY_DM_CTRL
#define IO_QSPI_USBPHY_DM_CTRL_OFFSET _u(0x0000000c)
#define IO_QSPI_USBPHY_DM_CTRL_BITS   _u(0x3003f01f)
#define IO_QSPI_USBPHY_DM_CTRL_RESET  _u(0x0000001f)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_USBPHY_DM_CTRL_IRQOVER
//               0x0 -> don't invert the interrupt
//               0x1 -> invert the interrupt
//               0x2 -> drive interrupt low
//               0x3 -> drive interrupt high
#define IO_QSPI_USBPHY_DM_CTRL_IRQOVER_RESET  _u(0x0)
#define IO_QSPI_USBPHY_DM_CTRL_IRQOVER_BITS   _u(0x30000000)
#define IO_QSPI_USBPHY_DM_CTRL_IRQOVER_MSB    _u(29)
#define IO_QSPI_USBPHY_DM_CTRL_IRQOVER_LSB    _u(28)
#define IO_QSPI_USBPHY_DM_CTRL_IRQOVER_ACCESS "RW"
#define IO_QSPI_USBPHY_DM_CTRL_IRQOVER_VALUE_NORMAL _u(0x0)
#define IO_QSPI_USBPHY_DM_CTRL_IRQOVER_VALUE_INVERT _u(0x1)
#define IO_QSPI_USBPHY_DM_CTRL_IRQOVER_VALUE_LOW _u(0x2)
#define IO_QSPI_USBPHY_DM_CTRL_IRQOVER_VALUE_HIGH _u(0x3)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_USBPHY_DM_CTRL_INOVER
//               0x0 -> don't invert the peri input
//               0x1 -> invert the peri input
//               0x2 -> drive peri input low
//               0x3 -> drive peri input high
#define IO_QSPI_USBPHY_DM_CTRL_INOVER_RESET  _u(0x0)
#define IO_QSPI_USBPHY_DM_CTRL_INOVER_BITS   _u(0x00030000)
#define IO_QSPI_USBPHY_DM_CTRL_INOVER_MSB    _u(17)
#define IO_QSPI_USBPHY_DM_CTRL_INOVER_LSB    _u(16)
#define IO_QSPI_USBPHY_DM_CTRL_INOVER_ACCESS "RW"
#define IO_QSPI_USBPHY_DM_CTRL_INOVER_VALUE_NORMAL _u(0x0)
#define IO_QSPI_USBPHY_DM_CTRL_INOVER_VALUE_INVERT _u(0x1)
#define IO_QSPI_USBPHY_DM_CTRL_INOVER_VALUE_LOW _u(0x2)
#define IO_QSPI_USBPHY_DM_CTRL_INOVER_VALUE_HIGH _u(0x3)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_USBPHY_DM_CTRL_OEOVER
//               0x0 -> drive output enable from peripheral signal selected by funcsel
//               0x1 -> drive output enable from inverse of peripheral signal selected by funcsel
//               0x2 -> disable output
//               0x3 -> enable output
#define IO_QSPI_USBPHY_DM_CTRL_OEOVER_RESET  _u(0x0)
#define IO_QSPI_USBPHY_DM_CTRL_OEOVER_BITS   _u(0x0000c000)
#define IO_QSPI_USBPHY_DM_CTRL_OEOVER_MSB    _u(15)
#define IO_QSPI_USBPHY_DM_CTRL_OEOVER_LSB    _u(14)
#define IO_QSPI_USBPHY_DM_CTRL_OEOVER_ACCESS "RW"
#define IO_QSPI_USBPHY_DM_CTRL_OEOVER_VALUE_NORMAL _u(0x0)
#define IO_QSPI_USBPHY_DM_CTRL_OEOVER_VALUE_INVERT _u(0x1)
#define IO_QSPI_USBPHY_DM_CTRL_OEOVER_VALUE_DISABLE _u(0x2)
#define IO_QSPI_USBPHY_DM_CTRL_OEOVER_VALUE_ENABLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_USBPHY_DM_CTRL_OUTOVER
//               0x0 -> drive output from peripheral signal selected by funcsel
//               0x1 -> drive output from inverse of peripheral signal selected by funcsel
//               0x2 -> drive output low
//               0x3 -> drive output high
#define IO_QSPI_USBPHY_DM_CTRL_OUTOVER_RESET  _u(0x0)
#define IO_QSPI_USBPHY_DM_CTRL_OUTOVER_BITS   _u(0x00003000)
#define IO_QSPI_USBPHY_DM_CTRL_OUTOVER_MSB    _u(13)
#define IO_QSPI_USBPHY_DM_CTRL_OUTOVER_LSB    _u(12)
#define IO_QSPI_USBPHY_DM_CTRL_OUTOVER_ACCESS "RW"
#define IO_QSPI_USBPHY_DM_CTRL_OUTOVER_VALUE_NORMAL _u(0x0)
#define IO_QSPI_USBPHY_DM_CTRL_OUTOVER_VALUE_INVERT _u(0x1)
#define IO_QSPI_USBPHY_DM_CTRL_OUTOVER_VALUE_LOW _u(0x2)
#define IO_QSPI_USBPHY_DM_CTRL_OUTOVER_VALUE_HIGH _u(0x3)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_USBPHY_DM_CTRL_FUNCSEL
// Description : 0-31 -> selects pin function according to the gpio table
//               31 == NULL
//               0x02 -> uart1_rx
//               0x03 -> i2c0_scl
//               0x05 -> siob_proc_57
//               0x1f -> null
#define IO_QSPI_USBPHY_DM_CTRL_FUNCSEL_RESET  _u(0x1f)
#define IO_QSPI_USBPHY_DM_CTRL_FUNCSEL_BITS   _u(0x0000001f)
#define IO_QSPI_USBPHY_DM_CTRL_FUNCSEL_MSB    _u(4)
#define IO_QSPI_USBPHY_DM_CTRL_FUNCSEL_LSB    _u(0)
#define IO_QSPI_USBPHY_DM_CTRL_FUNCSEL_ACCESS "RW"
#define IO_QSPI_USBPHY_DM_CTRL_FUNCSEL_VALUE_UART1_RX _u(0x02)
#define IO_QSPI_USBPHY_DM_CTRL_FUNCSEL_VALUE_I2C0_SCL _u(0x03)
#define IO_QSPI_USBPHY_DM_CTRL_FUNCSEL_VALUE_SIOB_PROC_57 _u(0x05)
#define IO_QSPI_USBPHY_DM_CTRL_FUNCSEL_VALUE_NULL _u(0x1f)
// =============================================================================
// Register    : IO_QSPI_GPIO_QSPI_SCLK_STATUS
#define IO_QSPI_GPIO_QSPI_SCLK_STATUS_OFFSET _u(0x00000010)
#define IO_QSPI_GPIO_QSPI_SCLK_STATUS_BITS   _u(0x04022200)
#define IO_QSPI_GPIO_QSPI_SCLK_STATUS_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_GPIO_QSPI_SCLK_STATUS_IRQTOPROC
// Description : interrupt to processors, after override is applied
#define IO_QSPI_GPIO_QSPI_SCLK_STATUS_IRQTOPROC_RESET  _u(0x0)
#define IO_QSPI_GPIO_QSPI_SCLK_STATUS_IRQTOPROC_BITS   _u(0x04000000)
#define IO_QSPI_GPIO_QSPI_SCLK_STATUS_IRQTOPROC_MSB    _u(26)
#define IO_QSPI_GPIO_QSPI_SCLK_STATUS_IRQTOPROC_LSB    _u(26)
#define IO_QSPI_GPIO_QSPI_SCLK_STATUS_IRQTOPROC_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_GPIO_QSPI_SCLK_STATUS_INFROMPAD
// Description : input signal from pad, before filtering and override are
//               applied
#define IO_QSPI_GPIO_QSPI_SCLK_STATUS_INFROMPAD_RESET  _u(0x0)
#define IO_QSPI_GPIO_QSPI_SCLK_STATUS_INFROMPAD_BITS   _u(0x00020000)
#define IO_QSPI_GPIO_QSPI_SCLK_STATUS_INFROMPAD_MSB    _u(17)
#define IO_QSPI_GPIO_QSPI_SCLK_STATUS_INFROMPAD_LSB    _u(17)
#define IO_QSPI_GPIO_QSPI_SCLK_STATUS_INFROMPAD_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_GPIO_QSPI_SCLK_STATUS_OETOPAD
// Description : output enable to pad after register override is applied
#define IO_QSPI_GPIO_QSPI_SCLK_STATUS_OETOPAD_RESET  _u(0x0)
#define IO_QSPI_GPIO_QSPI_SCLK_STATUS_OETOPAD_BITS   _u(0x00002000)
#define IO_QSPI_GPIO_QSPI_SCLK_STATUS_OETOPAD_MSB    _u(13)
#define IO_QSPI_GPIO_QSPI_SCLK_STATUS_OETOPAD_LSB    _u(13)
#define IO_QSPI_GPIO_QSPI_SCLK_STATUS_OETOPAD_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_GPIO_QSPI_SCLK_STATUS_OUTTOPAD
// Description : output signal to pad after register override is applied
#define IO_QSPI_GPIO_QSPI_SCLK_STATUS_OUTTOPAD_RESET  _u(0x0)
#define IO_QSPI_GPIO_QSPI_SCLK_STATUS_OUTTOPAD_BITS   _u(0x00000200)
#define IO_QSPI_GPIO_QSPI_SCLK_STATUS_OUTTOPAD_MSB    _u(9)
#define IO_QSPI_GPIO_QSPI_SCLK_STATUS_OUTTOPAD_LSB    _u(9)
#define IO_QSPI_GPIO_QSPI_SCLK_STATUS_OUTTOPAD_ACCESS "RO"
// =============================================================================
// Register    : IO_QSPI_GPIO_QSPI_SCLK_CTRL
#define IO_QSPI_GPIO_QSPI_SCLK_CTRL_OFFSET _u(0x00000014)
#define IO_QSPI_GPIO_QSPI_SCLK_CTRL_BITS   _u(0x3003f01f)
#define IO_QSPI_GPIO_QSPI_SCLK_CTRL_RESET  _u(0x0000001f)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_GPIO_QSPI_SCLK_CTRL_IRQOVER
//               0x0 -> don't invert the interrupt
//               0x1 -> invert the interrupt
//               0x2 -> drive interrupt low
//               0x3 -> drive interrupt high
#define IO_QSPI_GPIO_QSPI_SCLK_CTRL_IRQOVER_RESET  _u(0x0)
#define IO_QSPI_GPIO_QSPI_SCLK_CTRL_IRQOVER_BITS   _u(0x30000000)
#define IO_QSPI_GPIO_QSPI_SCLK_CTRL_IRQOVER_MSB    _u(29)
#define IO_QSPI_GPIO_QSPI_SCLK_CTRL_IRQOVER_LSB    _u(28)
#define IO_QSPI_GPIO_QSPI_SCLK_CTRL_IRQOVER_ACCESS "RW"
#define IO_QSPI_GPIO_QSPI_SCLK_CTRL_IRQOVER_VALUE_NORMAL _u(0x0)
#define IO_QSPI_GPIO_QSPI_SCLK_CTRL_IRQOVER_VALUE_INVERT _u(0x1)
#define IO_QSPI_GPIO_QSPI_SCLK_CTRL_IRQOVER_VALUE_LOW _u(0x2)
#define IO_QSPI_GPIO_QSPI_SCLK_CTRL_IRQOVER_VALUE_HIGH _u(0x3)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_GPIO_QSPI_SCLK_CTRL_INOVER
//               0x0 -> don't invert the peri input
//               0x1 -> invert the peri input
//               0x2 -> drive peri input low
//               0x3 -> drive peri input high
#define IO_QSPI_GPIO_QSPI_SCLK_CTRL_INOVER_RESET  _u(0x0)
#define IO_QSPI_GPIO_QSPI_SCLK_CTRL_INOVER_BITS   _u(0x00030000)
#define IO_QSPI_GPIO_QSPI_SCLK_CTRL_INOVER_MSB    _u(17)
#define IO_QSPI_GPIO_QSPI_SCLK_CTRL_INOVER_LSB    _u(16)
#define IO_QSPI_GPIO_QSPI_SCLK_CTRL_INOVER_ACCESS "RW"
#define IO_QSPI_GPIO_QSPI_SCLK_CTRL_INOVER_VALUE_NORMAL _u(0x0)
#define IO_QSPI_GPIO_QSPI_SCLK_CTRL_INOVER_VALUE_INVERT _u(0x1)
#define IO_QSPI_GPIO_QSPI_SCLK_CTRL_INOVER_VALUE_LOW _u(0x2)
#define IO_QSPI_GPIO_QSPI_SCLK_CTRL_INOVER_VALUE_HIGH _u(0x3)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_GPIO_QSPI_SCLK_CTRL_OEOVER
//               0x0 -> drive output enable from peripheral signal selected by funcsel
//               0x1 -> drive output enable from inverse of peripheral signal selected by funcsel
//               0x2 -> disable output
//               0x3 -> enable output
#define IO_QSPI_GPIO_QSPI_SCLK_CTRL_OEOVER_RESET  _u(0x0)
#define IO_QSPI_GPIO_QSPI_SCLK_CTRL_OEOVER_BITS   _u(0x0000c000)
#define IO_QSPI_GPIO_QSPI_SCLK_CTRL_OEOVER_MSB    _u(15)
#define IO_QSPI_GPIO_QSPI_SCLK_CTRL_OEOVER_LSB    _u(14)
#define IO_QSPI_GPIO_QSPI_SCLK_CTRL_OEOVER_ACCESS "RW"
#define IO_QSPI_GPIO_QSPI_SCLK_CTRL_OEOVER_VALUE_NORMAL _u(0x0)
#define IO_QSPI_GPIO_QSPI_SCLK_CTRL_OEOVER_VALUE_INVERT _u(0x1)
#define IO_QSPI_GPIO_QSPI_SCLK_CTRL_OEOVER_VALUE_DISABLE _u(0x2)
#define IO_QSPI_GPIO_QSPI_SCLK_CTRL_OEOVER_VALUE_ENABLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_GPIO_QSPI_SCLK_CTRL_OUTOVER
//               0x0 -> drive output from peripheral signal selected by funcsel
//               0x1 -> drive output from inverse of peripheral signal selected by funcsel
//               0x2 -> drive output low
//               0x3 -> drive output high
#define IO_QSPI_GPIO_QSPI_SCLK_CTRL_OUTOVER_RESET  _u(0x0)
#define IO_QSPI_GPIO_QSPI_SCLK_CTRL_OUTOVER_BITS   _u(0x00003000)
#define IO_QSPI_GPIO_QSPI_SCLK_CTRL_OUTOVER_MSB    _u(13)
#define IO_QSPI_GPIO_QSPI_SCLK_CTRL_OUTOVER_LSB    _u(12)
#define IO_QSPI_GPIO_QSPI_SCLK_CTRL_OUTOVER_ACCESS "RW"
#define IO_QSPI_GPIO_QSPI_SCLK_CTRL_OUTOVER_VALUE_NORMAL _u(0x0)
#define IO_QSPI_GPIO_QSPI_SCLK_CTRL_OUTOVER_VALUE_INVERT _u(0x1)
#define IO_QSPI_GPIO_QSPI_SCLK_CTRL_OUTOVER_VALUE_LOW _u(0x2)
#define IO_QSPI_GPIO_QSPI_SCLK_CTRL_OUTOVER_VALUE_HIGH _u(0x3)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_GPIO_QSPI_SCLK_CTRL_FUNCSEL
// Description : 0-31 -> selects pin function according to the gpio table
//               31 == NULL
//               0x00 -> xip_sclk
//               0x02 -> uart1_cts
//               0x03 -> i2c1_sda
//               0x05 -> siob_proc_58
//               0x0b -> uart1_tx
//               0x1f -> null
#define IO_QSPI_GPIO_QSPI_SCLK_CTRL_FUNCSEL_RESET  _u(0x1f)
#define IO_QSPI_GPIO_QSPI_SCLK_CTRL_FUNCSEL_BITS   _u(0x0000001f)
#define IO_QSPI_GPIO_QSPI_SCLK_CTRL_FUNCSEL_MSB    _u(4)
#define IO_QSPI_GPIO_QSPI_SCLK_CTRL_FUNCSEL_LSB    _u(0)
#define IO_QSPI_GPIO_QSPI_SCLK_CTRL_FUNCSEL_ACCESS "RW"
#define IO_QSPI_GPIO_QSPI_SCLK_CTRL_FUNCSEL_VALUE_XIP_SCLK _u(0x00)
#define IO_QSPI_GPIO_QSPI_SCLK_CTRL_FUNCSEL_VALUE_UART1_CTS _u(0x02)
#define IO_QSPI_GPIO_QSPI_SCLK_CTRL_FUNCSEL_VALUE_I2C1_SDA _u(0x03)
#define IO_QSPI_GPIO_QSPI_SCLK_CTRL_FUNCSEL_VALUE_SIOB_PROC_58 _u(0x05)
#define IO_QSPI_GPIO_QSPI_SCLK_CTRL_FUNCSEL_VALUE_UART1_TX _u(0x0b)
#define IO_QSPI_GPIO_QSPI_SCLK_CTRL_FUNCSEL_VALUE_NULL _u(0x1f)
// =============================================================================
// Register    : IO_QSPI_GPIO_QSPI_SS_STATUS
#define IO_QSPI_GPIO_QSPI_SS_STATUS_OFFSET _u(0x00000018)
#define IO_QSPI_GPIO_QSPI_SS_STATUS_BITS   _u(0x04022200)
#define IO_QSPI_GPIO_QSPI_SS_STATUS_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_GPIO_QSPI_SS_STATUS_IRQTOPROC
// Description : interrupt to processors, after override is applied
#define IO_QSPI_GPIO_QSPI_SS_STATUS_IRQTOPROC_RESET  _u(0x0)
#define IO_QSPI_GPIO_QSPI_SS_STATUS_IRQTOPROC_BITS   _u(0x04000000)
#define IO_QSPI_GPIO_QSPI_SS_STATUS_IRQTOPROC_MSB    _u(26)
#define IO_QSPI_GPIO_QSPI_SS_STATUS_IRQTOPROC_LSB    _u(26)
#define IO_QSPI_GPIO_QSPI_SS_STATUS_IRQTOPROC_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_GPIO_QSPI_SS_STATUS_INFROMPAD
// Description : input signal from pad, before filtering and override are
//               applied
#define IO_QSPI_GPIO_QSPI_SS_STATUS_INFROMPAD_RESET  _u(0x0)
#define IO_QSPI_GPIO_QSPI_SS_STATUS_INFROMPAD_BITS   _u(0x00020000)
#define IO_QSPI_GPIO_QSPI_SS_STATUS_INFROMPAD_MSB    _u(17)
#define IO_QSPI_GPIO_QSPI_SS_STATUS_INFROMPAD_LSB    _u(17)
#define IO_QSPI_GPIO_QSPI_SS_STATUS_INFROMPAD_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_GPIO_QSPI_SS_STATUS_OETOPAD
// Description : output enable to pad after register override is applied
#define IO_QSPI_GPIO_QSPI_SS_STATUS_OETOPAD_RESET  _u(0x0)
#define IO_QSPI_GPIO_QSPI_SS_STATUS_OETOPAD_BITS   _u(0x00002000)
#define IO_QSPI_GPIO_QSPI_SS_STATUS_OETOPAD_MSB    _u(13)
#define IO_QSPI_GPIO_QSPI_SS_STATUS_OETOPAD_LSB    _u(13)
#define IO_QSPI_GPIO_QSPI_SS_STATUS_OETOPAD_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_GPIO_QSPI_SS_STATUS_OUTTOPAD
// Description : output signal to pad after register override is applied
#define IO_QSPI_GPIO_QSPI_SS_STATUS_OUTTOPAD_RESET  _u(0x0)
#define IO_QSPI_GPIO_QSPI_SS_STATUS_OUTTOPAD_BITS   _u(0x00000200)
#define IO_QSPI_GPIO_QSPI_SS_STATUS_OUTTOPAD_MSB    _u(9)
#define IO_QSPI_GPIO_QSPI_SS_STATUS_OUTTOPAD_LSB    _u(9)
#define IO_QSPI_GPIO_QSPI_SS_STATUS_OUTTOPAD_ACCESS "RO"
// =============================================================================
// Register    : IO_QSPI_GPIO_QSPI_SS_CTRL
#define IO_QSPI_GPIO_QSPI_SS_CTRL_OFFSET _u(0x0000001c)
#define IO_QSPI_GPIO_QSPI_SS_CTRL_BITS   _u(0x3003f01f)
#define IO_QSPI_GPIO_QSPI_SS_CTRL_RESET  _u(0x0000001f)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_GPIO_QSPI_SS_CTRL_IRQOVER
//               0x0 -> don't invert the interrupt
//               0x1 -> invert the interrupt
//               0x2 -> drive interrupt low
//               0x3 -> drive interrupt high
#define IO_QSPI_GPIO_QSPI_SS_CTRL_IRQOVER_RESET  _u(0x0)
#define IO_QSPI_GPIO_QSPI_SS_CTRL_IRQOVER_BITS   _u(0x30000000)
#define IO_QSPI_GPIO_QSPI_SS_CTRL_IRQOVER_MSB    _u(29)
#define IO_QSPI_GPIO_QSPI_SS_CTRL_IRQOVER_LSB    _u(28)
#define IO_QSPI_GPIO_QSPI_SS_CTRL_IRQOVER_ACCESS "RW"
#define IO_QSPI_GPIO_QSPI_SS_CTRL_IRQOVER_VALUE_NORMAL _u(0x0)
#define IO_QSPI_GPIO_QSPI_SS_CTRL_IRQOVER_VALUE_INVERT _u(0x1)
#define IO_QSPI_GPIO_QSPI_SS_CTRL_IRQOVER_VALUE_LOW _u(0x2)
#define IO_QSPI_GPIO_QSPI_SS_CTRL_IRQOVER_VALUE_HIGH _u(0x3)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_GPIO_QSPI_SS_CTRL_INOVER
//               0x0 -> don't invert the peri input
//               0x1 -> invert the peri input
//               0x2 -> drive peri input low
//               0x3 -> drive peri input high
#define IO_QSPI_GPIO_QSPI_SS_CTRL_INOVER_RESET  _u(0x0)
#define IO_QSPI_GPIO_QSPI_SS_CTRL_INOVER_BITS   _u(0x00030000)
#define IO_QSPI_GPIO_QSPI_SS_CTRL_INOVER_MSB    _u(17)
#define IO_QSPI_GPIO_QSPI_SS_CTRL_INOVER_LSB    _u(16)
#define IO_QSPI_GPIO_QSPI_SS_CTRL_INOVER_ACCESS "RW"
#define IO_QSPI_GPIO_QSPI_SS_CTRL_INOVER_VALUE_NORMAL _u(0x0)
#define IO_QSPI_GPIO_QSPI_SS_CTRL_INOVER_VALUE_INVERT _u(0x1)
#define IO_QSPI_GPIO_QSPI_SS_CTRL_INOVER_VALUE_LOW _u(0x2)
#define IO_QSPI_GPIO_QSPI_SS_CTRL_INOVER_VALUE_HIGH _u(0x3)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_GPIO_QSPI_SS_CTRL_OEOVER
//               0x0 -> drive output enable from peripheral signal selected by funcsel
//               0x1 -> drive output enable from inverse of peripheral signal selected by funcsel
//               0x2 -> disable output
//               0x3 -> enable output
#define IO_QSPI_GPIO_QSPI_SS_CTRL_OEOVER_RESET  _u(0x0)
#define IO_QSPI_GPIO_QSPI_SS_CTRL_OEOVER_BITS   _u(0x0000c000)
#define IO_QSPI_GPIO_QSPI_SS_CTRL_OEOVER_MSB    _u(15)
#define IO_QSPI_GPIO_QSPI_SS_CTRL_OEOVER_LSB    _u(14)
#define IO_QSPI_GPIO_QSPI_SS_CTRL_OEOVER_ACCESS "RW"
#define IO_QSPI_GPIO_QSPI_SS_CTRL_OEOVER_VALUE_NORMAL _u(0x0)
#define IO_QSPI_GPIO_QSPI_SS_CTRL_OEOVER_VALUE_INVERT _u(0x1)
#define IO_QSPI_GPIO_QSPI_SS_CTRL_OEOVER_VALUE_DISABLE _u(0x2)
#define IO_QSPI_GPIO_QSPI_SS_CTRL_OEOVER_VALUE_ENABLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_GPIO_QSPI_SS_CTRL_OUTOVER
//               0x0 -> drive output from peripheral signal selected by funcsel
//               0x1 -> drive output from inverse of peripheral signal selected by funcsel
//               0x2 -> drive output low
//               0x3 -> drive output high
#define IO_QSPI_GPIO_QSPI_SS_CTRL_OUTOVER_RESET  _u(0x0)
#define IO_QSPI_GPIO_QSPI_SS_CTRL_OUTOVER_BITS   _u(0x00003000)
#define IO_QSPI_GPIO_QSPI_SS_CTRL_OUTOVER_MSB    _u(13)
#define IO_QSPI_GPIO_QSPI_SS_CTRL_OUTOVER_LSB    _u(12)
#define IO_QSPI_GPIO_QSPI_SS_CTRL_OUTOVER_ACCESS "RW"
#define IO_QSPI_GPIO_QSPI_SS_CTRL_OUTOVER_VALUE_NORMAL _u(0x0)
#define IO_QSPI_GPIO_QSPI_SS_CTRL_OUTOVER_VALUE_INVERT _u(0x1)
#define IO_QSPI_GPIO_QSPI_SS_CTRL_OUTOVER_VALUE_LOW _u(0x2)
#define IO_QSPI_GPIO_QSPI_SS_CTRL_OUTOVER_VALUE_HIGH _u(0x3)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_GPIO_QSPI_SS_CTRL_FUNCSEL
// Description : 0-31 -> selects pin function according to the gpio table
//               31 == NULL
//               0x00 -> xip_ss_n_0
//               0x02 -> uart1_rts
//               0x03 -> i2c1_scl
//               0x05 -> siob_proc_59
//               0x0b -> uart1_rx
//               0x1f -> null
#define IO_QSPI_GPIO_QSPI_SS_CTRL_FUNCSEL_RESET  _u(0x1f)
#define IO_QSPI_GPIO_QSPI_SS_CTRL_FUNCSEL_BITS   _u(0x0000001f)
#define IO_QSPI_GPIO_QSPI_SS_CTRL_FUNCSEL_MSB    _u(4)
#define IO_QSPI_GPIO_QSPI_SS_CTRL_FUNCSEL_LSB    _u(0)
#define IO_QSPI_GPIO_QSPI_SS_CTRL_FUNCSEL_ACCESS "RW"
#define IO_QSPI_GPIO_QSPI_SS_CTRL_FUNCSEL_VALUE_XIP_SS_N_0 _u(0x00)
#define IO_QSPI_GPIO_QSPI_SS_CTRL_FUNCSEL_VALUE_UART1_RTS _u(0x02)
#define IO_QSPI_GPIO_QSPI_SS_CTRL_FUNCSEL_VALUE_I2C1_SCL _u(0x03)
#define IO_QSPI_GPIO_QSPI_SS_CTRL_FUNCSEL_VALUE_SIOB_PROC_59 _u(0x05)
#define IO_QSPI_GPIO_QSPI_SS_CTRL_FUNCSEL_VALUE_UART1_RX _u(0x0b)
#define IO_QSPI_GPIO_QSPI_SS_CTRL_FUNCSEL_VALUE_NULL _u(0x1f)
// =============================================================================
// Register    : IO_QSPI_GPIO_QSPI_SD0_STATUS
#define IO_QSPI_GPIO_QSPI_SD0_STATUS_OFFSET _u(0x00000020)
#define IO_QSPI_GPIO_QSPI_SD0_STATUS_BITS   _u(0x04022200)
#define IO_QSPI_GPIO_QSPI_SD0_STATUS_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_GPIO_QSPI_SD0_STATUS_IRQTOPROC
// Description : interrupt to processors, after override is applied
#define IO_QSPI_GPIO_QSPI_SD0_STATUS_IRQTOPROC_RESET  _u(0x0)
#define IO_QSPI_GPIO_QSPI_SD0_STATUS_IRQTOPROC_BITS   _u(0x04000000)
#define IO_QSPI_GPIO_QSPI_SD0_STATUS_IRQTOPROC_MSB    _u(26)
#define IO_QSPI_GPIO_QSPI_SD0_STATUS_IRQTOPROC_LSB    _u(26)
#define IO_QSPI_GPIO_QSPI_SD0_STATUS_IRQTOPROC_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_GPIO_QSPI_SD0_STATUS_INFROMPAD
// Description : input signal from pad, before filtering and override are
//               applied
#define IO_QSPI_GPIO_QSPI_SD0_STATUS_INFROMPAD_RESET  _u(0x0)
#define IO_QSPI_GPIO_QSPI_SD0_STATUS_INFROMPAD_BITS   _u(0x00020000)
#define IO_QSPI_GPIO_QSPI_SD0_STATUS_INFROMPAD_MSB    _u(17)
#define IO_QSPI_GPIO_QSPI_SD0_STATUS_INFROMPAD_LSB    _u(17)
#define IO_QSPI_GPIO_QSPI_SD0_STATUS_INFROMPAD_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_GPIO_QSPI_SD0_STATUS_OETOPAD
// Description : output enable to pad after register override is applied
#define IO_QSPI_GPIO_QSPI_SD0_STATUS_OETOPAD_RESET  _u(0x0)
#define IO_QSPI_GPIO_QSPI_SD0_STATUS_OETOPAD_BITS   _u(0x00002000)
#define IO_QSPI_GPIO_QSPI_SD0_STATUS_OETOPAD_MSB    _u(13)
#define IO_QSPI_GPIO_QSPI_SD0_STATUS_OETOPAD_LSB    _u(13)
#define IO_QSPI_GPIO_QSPI_SD0_STATUS_OETOPAD_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_GPIO_QSPI_SD0_STATUS_OUTTOPAD
// Description : output signal to pad after register override is applied
#define IO_QSPI_GPIO_QSPI_SD0_STATUS_OUTTOPAD_RESET  _u(0x0)
#define IO_QSPI_GPIO_QSPI_SD0_STATUS_OUTTOPAD_BITS   _u(0x00000200)
#define IO_QSPI_GPIO_QSPI_SD0_STATUS_OUTTOPAD_MSB    _u(9)
#define IO_QSPI_GPIO_QSPI_SD0_STATUS_OUTTOPAD_LSB    _u(9)
#define IO_QSPI_GPIO_QSPI_SD0_STATUS_OUTTOPAD_ACCESS "RO"
// =============================================================================
// Register    : IO_QSPI_GPIO_QSPI_SD0_CTRL
#define IO_QSPI_GPIO_QSPI_SD0_CTRL_OFFSET _u(0x00000024)
#define IO_QSPI_GPIO_QSPI_SD0_CTRL_BITS   _u(0x3003f01f)
#define IO_QSPI_GPIO_QSPI_SD0_CTRL_RESET  _u(0x0000001f)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_GPIO_QSPI_SD0_CTRL_IRQOVER
//               0x0 -> don't invert the interrupt
//               0x1 -> invert the interrupt
//               0x2 -> drive interrupt low
//               0x3 -> drive interrupt high
#define IO_QSPI_GPIO_QSPI_SD0_CTRL_IRQOVER_RESET  _u(0x0)
#define IO_QSPI_GPIO_QSPI_SD0_CTRL_IRQOVER_BITS   _u(0x30000000)
#define IO_QSPI_GPIO_QSPI_SD0_CTRL_IRQOVER_MSB    _u(29)
#define IO_QSPI_GPIO_QSPI_SD0_CTRL_IRQOVER_LSB    _u(28)
#define IO_QSPI_GPIO_QSPI_SD0_CTRL_IRQOVER_ACCESS "RW"
#define IO_QSPI_GPIO_QSPI_SD0_CTRL_IRQOVER_VALUE_NORMAL _u(0x0)
#define IO_QSPI_GPIO_QSPI_SD0_CTRL_IRQOVER_VALUE_INVERT _u(0x1)
#define IO_QSPI_GPIO_QSPI_SD0_CTRL_IRQOVER_VALUE_LOW _u(0x2)
#define IO_QSPI_GPIO_QSPI_SD0_CTRL_IRQOVER_VALUE_HIGH _u(0x3)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_GPIO_QSPI_SD0_CTRL_INOVER
//               0x0 -> don't invert the peri input
//               0x1 -> invert the peri input
//               0x2 -> drive peri input low
//               0x3 -> drive peri input high
#define IO_QSPI_GPIO_QSPI_SD0_CTRL_INOVER_RESET  _u(0x0)
#define IO_QSPI_GPIO_QSPI_SD0_CTRL_INOVER_BITS   _u(0x00030000)
#define IO_QSPI_GPIO_QSPI_SD0_CTRL_INOVER_MSB    _u(17)
#define IO_QSPI_GPIO_QSPI_SD0_CTRL_INOVER_LSB    _u(16)
#define IO_QSPI_GPIO_QSPI_SD0_CTRL_INOVER_ACCESS "RW"
#define IO_QSPI_GPIO_QSPI_SD0_CTRL_INOVER_VALUE_NORMAL _u(0x0)
#define IO_QSPI_GPIO_QSPI_SD0_CTRL_INOVER_VALUE_INVERT _u(0x1)
#define IO_QSPI_GPIO_QSPI_SD0_CTRL_INOVER_VALUE_LOW _u(0x2)
#define IO_QSPI_GPIO_QSPI_SD0_CTRL_INOVER_VALUE_HIGH _u(0x3)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_GPIO_QSPI_SD0_CTRL_OEOVER
//               0x0 -> drive output enable from peripheral signal selected by funcsel
//               0x1 -> drive output enable from inverse of peripheral signal selected by funcsel
//               0x2 -> disable output
//               0x3 -> enable output
#define IO_QSPI_GPIO_QSPI_SD0_CTRL_OEOVER_RESET  _u(0x0)
#define IO_QSPI_GPIO_QSPI_SD0_CTRL_OEOVER_BITS   _u(0x0000c000)
#define IO_QSPI_GPIO_QSPI_SD0_CTRL_OEOVER_MSB    _u(15)
#define IO_QSPI_GPIO_QSPI_SD0_CTRL_OEOVER_LSB    _u(14)
#define IO_QSPI_GPIO_QSPI_SD0_CTRL_OEOVER_ACCESS "RW"
#define IO_QSPI_GPIO_QSPI_SD0_CTRL_OEOVER_VALUE_NORMAL _u(0x0)
#define IO_QSPI_GPIO_QSPI_SD0_CTRL_OEOVER_VALUE_INVERT _u(0x1)
#define IO_QSPI_GPIO_QSPI_SD0_CTRL_OEOVER_VALUE_DISABLE _u(0x2)
#define IO_QSPI_GPIO_QSPI_SD0_CTRL_OEOVER_VALUE_ENABLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_GPIO_QSPI_SD0_CTRL_OUTOVER
//               0x0 -> drive output from peripheral signal selected by funcsel
//               0x1 -> drive output from inverse of peripheral signal selected by funcsel
//               0x2 -> drive output low
//               0x3 -> drive output high
#define IO_QSPI_GPIO_QSPI_SD0_CTRL_OUTOVER_RESET  _u(0x0)
#define IO_QSPI_GPIO_QSPI_SD0_CTRL_OUTOVER_BITS   _u(0x00003000)
#define IO_QSPI_GPIO_QSPI_SD0_CTRL_OUTOVER_MSB    _u(13)
#define IO_QSPI_GPIO_QSPI_SD0_CTRL_OUTOVER_LSB    _u(12)
#define IO_QSPI_GPIO_QSPI_SD0_CTRL_OUTOVER_ACCESS "RW"
#define IO_QSPI_GPIO_QSPI_SD0_CTRL_OUTOVER_VALUE_NORMAL _u(0x0)
#define IO_QSPI_GPIO_QSPI_SD0_CTRL_OUTOVER_VALUE_INVERT _u(0x1)
#define IO_QSPI_GPIO_QSPI_SD0_CTRL_OUTOVER_VALUE_LOW _u(0x2)
#define IO_QSPI_GPIO_QSPI_SD0_CTRL_OUTOVER_VALUE_HIGH _u(0x3)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_GPIO_QSPI_SD0_CTRL_FUNCSEL
// Description : 0-31 -> selects pin function according to the gpio table
//               31 == NULL
//               0x00 -> xip_sd0
//               0x02 -> uart0_tx
//               0x03 -> i2c0_sda
//               0x05 -> siob_proc_60
//               0x1f -> null
#define IO_QSPI_GPIO_QSPI_SD0_CTRL_FUNCSEL_RESET  _u(0x1f)
#define IO_QSPI_GPIO_QSPI_SD0_CTRL_FUNCSEL_BITS   _u(0x0000001f)
#define IO_QSPI_GPIO_QSPI_SD0_CTRL_FUNCSEL_MSB    _u(4)
#define IO_QSPI_GPIO_QSPI_SD0_CTRL_FUNCSEL_LSB    _u(0)
#define IO_QSPI_GPIO_QSPI_SD0_CTRL_FUNCSEL_ACCESS "RW"
#define IO_QSPI_GPIO_QSPI_SD0_CTRL_FUNCSEL_VALUE_XIP_SD0 _u(0x00)
#define IO_QSPI_GPIO_QSPI_SD0_CTRL_FUNCSEL_VALUE_UART0_TX _u(0x02)
#define IO_QSPI_GPIO_QSPI_SD0_CTRL_FUNCSEL_VALUE_I2C0_SDA _u(0x03)
#define IO_QSPI_GPIO_QSPI_SD0_CTRL_FUNCSEL_VALUE_SIOB_PROC_60 _u(0x05)
#define IO_QSPI_GPIO_QSPI_SD0_CTRL_FUNCSEL_VALUE_NULL _u(0x1f)
// =============================================================================
// Register    : IO_QSPI_GPIO_QSPI_SD1_STATUS
#define IO_QSPI_GPIO_QSPI_SD1_STATUS_OFFSET _u(0x00000028)
#define IO_QSPI_GPIO_QSPI_SD1_STATUS_BITS   _u(0x04022200)
#define IO_QSPI_GPIO_QSPI_SD1_STATUS_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_GPIO_QSPI_SD1_STATUS_IRQTOPROC
// Description : interrupt to processors, after override is applied
#define IO_QSPI_GPIO_QSPI_SD1_STATUS_IRQTOPROC_RESET  _u(0x0)
#define IO_QSPI_GPIO_QSPI_SD1_STATUS_IRQTOPROC_BITS   _u(0x04000000)
#define IO_QSPI_GPIO_QSPI_SD1_STATUS_IRQTOPROC_MSB    _u(26)
#define IO_QSPI_GPIO_QSPI_SD1_STATUS_IRQTOPROC_LSB    _u(26)
#define IO_QSPI_GPIO_QSPI_SD1_STATUS_IRQTOPROC_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_GPIO_QSPI_SD1_STATUS_INFROMPAD
// Description : input signal from pad, before filtering and override are
//               applied
#define IO_QSPI_GPIO_QSPI_SD1_STATUS_INFROMPAD_RESET  _u(0x0)
#define IO_QSPI_GPIO_QSPI_SD1_STATUS_INFROMPAD_BITS   _u(0x00020000)
#define IO_QSPI_GPIO_QSPI_SD1_STATUS_INFROMPAD_MSB    _u(17)
#define IO_QSPI_GPIO_QSPI_SD1_STATUS_INFROMPAD_LSB    _u(17)
#define IO_QSPI_GPIO_QSPI_SD1_STATUS_INFROMPAD_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_GPIO_QSPI_SD1_STATUS_OETOPAD
// Description : output enable to pad after register override is applied
#define IO_QSPI_GPIO_QSPI_SD1_STATUS_OETOPAD_RESET  _u(0x0)
#define IO_QSPI_GPIO_QSPI_SD1_STATUS_OETOPAD_BITS   _u(0x00002000)
#define IO_QSPI_GPIO_QSPI_SD1_STATUS_OETOPAD_MSB    _u(13)
#define IO_QSPI_GPIO_QSPI_SD1_STATUS_OETOPAD_LSB    _u(13)
#define IO_QSPI_GPIO_QSPI_SD1_STATUS_OETOPAD_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_GPIO_QSPI_SD1_STATUS_OUTTOPAD
// Description : output signal to pad after register override is applied
#define IO_QSPI_GPIO_QSPI_SD1_STATUS_OUTTOPAD_RESET  _u(0x0)
#define IO_QSPI_GPIO_QSPI_SD1_STATUS_OUTTOPAD_BITS   _u(0x00000200)
#define IO_QSPI_GPIO_QSPI_SD1_STATUS_OUTTOPAD_MSB    _u(9)
#define IO_QSPI_GPIO_QSPI_SD1_STATUS_OUTTOPAD_LSB    _u(9)
#define IO_QSPI_GPIO_QSPI_SD1_STATUS_OUTTOPAD_ACCESS "RO"
// =============================================================================
// Register    : IO_QSPI_GPIO_QSPI_SD1_CTRL
#define IO_QSPI_GPIO_QSPI_SD1_CTRL_OFFSET _u(0x0000002c)
#define IO_QSPI_GPIO_QSPI_SD1_CTRL_BITS   _u(0x3003f01f)
#define IO_QSPI_GPIO_QSPI_SD1_CTRL_RESET  _u(0x0000001f)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_GPIO_QSPI_SD1_CTRL_IRQOVER
//               0x0 -> don't invert the interrupt
//               0x1 -> invert the interrupt
//               0x2 -> drive interrupt low
//               0x3 -> drive interrupt high
#define IO_QSPI_GPIO_QSPI_SD1_CTRL_IRQOVER_RESET  _u(0x0)
#define IO_QSPI_GPIO_QSPI_SD1_CTRL_IRQOVER_BITS   _u(0x30000000)
#define IO_QSPI_GPIO_QSPI_SD1_CTRL_IRQOVER_MSB    _u(29)
#define IO_QSPI_GPIO_QSPI_SD1_CTRL_IRQOVER_LSB    _u(28)
#define IO_QSPI_GPIO_QSPI_SD1_CTRL_IRQOVER_ACCESS "RW"
#define IO_QSPI_GPIO_QSPI_SD1_CTRL_IRQOVER_VALUE_NORMAL _u(0x0)
#define IO_QSPI_GPIO_QSPI_SD1_CTRL_IRQOVER_VALUE_INVERT _u(0x1)
#define IO_QSPI_GPIO_QSPI_SD1_CTRL_IRQOVER_VALUE_LOW _u(0x2)
#define IO_QSPI_GPIO_QSPI_SD1_CTRL_IRQOVER_VALUE_HIGH _u(0x3)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_GPIO_QSPI_SD1_CTRL_INOVER
//               0x0 -> don't invert the peri input
//               0x1 -> invert the peri input
//               0x2 -> drive peri input low
//               0x3 -> drive peri input high
#define IO_QSPI_GPIO_QSPI_SD1_CTRL_INOVER_RESET  _u(0x0)
#define IO_QSPI_GPIO_QSPI_SD1_CTRL_INOVER_BITS   _u(0x00030000)
#define IO_QSPI_GPIO_QSPI_SD1_CTRL_INOVER_MSB    _u(17)
#define IO_QSPI_GPIO_QSPI_SD1_CTRL_INOVER_LSB    _u(16)
#define IO_QSPI_GPIO_QSPI_SD1_CTRL_INOVER_ACCESS "RW"
#define IO_QSPI_GPIO_QSPI_SD1_CTRL_INOVER_VALUE_NORMAL _u(0x0)
#define IO_QSPI_GPIO_QSPI_SD1_CTRL_INOVER_VALUE_INVERT _u(0x1)
#define IO_QSPI_GPIO_QSPI_SD1_CTRL_INOVER_VALUE_LOW _u(0x2)
#define IO_QSPI_GPIO_QSPI_SD1_CTRL_INOVER_VALUE_HIGH _u(0x3)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_GPIO_QSPI_SD1_CTRL_OEOVER
//               0x0 -> drive output enable from peripheral signal selected by funcsel
//               0x1 -> drive output enable from inverse of peripheral signal selected by funcsel
//               0x2 -> disable output
//               0x3 -> enable output
#define IO_QSPI_GPIO_QSPI_SD1_CTRL_OEOVER_RESET  _u(0x0)
#define IO_QSPI_GPIO_QSPI_SD1_CTRL_OEOVER_BITS   _u(0x0000c000)
#define IO_QSPI_GPIO_QSPI_SD1_CTRL_OEOVER_MSB    _u(15)
#define IO_QSPI_GPIO_QSPI_SD1_CTRL_OEOVER_LSB    _u(14)
#define IO_QSPI_GPIO_QSPI_SD1_CTRL_OEOVER_ACCESS "RW"
#define IO_QSPI_GPIO_QSPI_SD1_CTRL_OEOVER_VALUE_NORMAL _u(0x0)
#define IO_QSPI_GPIO_QSPI_SD1_CTRL_OEOVER_VALUE_INVERT _u(0x1)
#define IO_QSPI_GPIO_QSPI_SD1_CTRL_OEOVER_VALUE_DISABLE _u(0x2)
#define IO_QSPI_GPIO_QSPI_SD1_CTRL_OEOVER_VALUE_ENABLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_GPIO_QSPI_SD1_CTRL_OUTOVER
//               0x0 -> drive output from peripheral signal selected by funcsel
//               0x1 -> drive output from inverse of peripheral signal selected by funcsel
//               0x2 -> drive output low
//               0x3 -> drive output high
#define IO_QSPI_GPIO_QSPI_SD1_CTRL_OUTOVER_RESET  _u(0x0)
#define IO_QSPI_GPIO_QSPI_SD1_CTRL_OUTOVER_BITS   _u(0x00003000)
#define IO_QSPI_GPIO_QSPI_SD1_CTRL_OUTOVER_MSB    _u(13)
#define IO_QSPI_GPIO_QSPI_SD1_CTRL_OUTOVER_LSB    _u(12)
#define IO_QSPI_GPIO_QSPI_SD1_CTRL_OUTOVER_ACCESS "RW"
#define IO_QSPI_GPIO_QSPI_SD1_CTRL_OUTOVER_VALUE_NORMAL _u(0x0)
#define IO_QSPI_GPIO_QSPI_SD1_CTRL_OUTOVER_VALUE_INVERT _u(0x1)
#define IO_QSPI_GPIO_QSPI_SD1_CTRL_OUTOVER_VALUE_LOW _u(0x2)
#define IO_QSPI_GPIO_QSPI_SD1_CTRL_OUTOVER_VALUE_HIGH _u(0x3)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_GPIO_QSPI_SD1_CTRL_FUNCSEL
// Description : 0-31 -> selects pin function according to the gpio table
//               31 == NULL
//               0x00 -> xip_sd1
//               0x02 -> uart0_rx
//               0x03 -> i2c0_scl
//               0x05 -> siob_proc_61
//               0x1f -> null
#define IO_QSPI_GPIO_QSPI_SD1_CTRL_FUNCSEL_RESET  _u(0x1f)
#define IO_QSPI_GPIO_QSPI_SD1_CTRL_FUNCSEL_BITS   _u(0x0000001f)
#define IO_QSPI_GPIO_QSPI_SD1_CTRL_FUNCSEL_MSB    _u(4)
#define IO_QSPI_GPIO_QSPI_SD1_CTRL_FUNCSEL_LSB    _u(0)
#define IO_QSPI_GPIO_QSPI_SD1_CTRL_FUNCSEL_ACCESS "RW"
#define IO_QSPI_GPIO_QSPI_SD1_CTRL_FUNCSEL_VALUE_XIP_SD1 _u(0x00)
#define IO_QSPI_GPIO_QSPI_SD1_CTRL_FUNCSEL_VALUE_UART0_RX _u(0x02)
#define IO_QSPI_GPIO_QSPI_SD1_CTRL_FUNCSEL_VALUE_I2C0_SCL _u(0x03)
#define IO_QSPI_GPIO_QSPI_SD1_CTRL_FUNCSEL_VALUE_SIOB_PROC_61 _u(0x05)
#define IO_QSPI_GPIO_QSPI_SD1_CTRL_FUNCSEL_VALUE_NULL _u(0x1f)
// =============================================================================
// Register    : IO_QSPI_GPIO_QSPI_SD2_STATUS
#define IO_QSPI_GPIO_QSPI_SD2_STATUS_OFFSET _u(0x00000030)
#define IO_QSPI_GPIO_QSPI_SD2_STATUS_BITS   _u(0x04022200)
#define IO_QSPI_GPIO_QSPI_SD2_STATUS_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_GPIO_QSPI_SD2_STATUS_IRQTOPROC
// Description : interrupt to processors, after override is applied
#define IO_QSPI_GPIO_QSPI_SD2_STATUS_IRQTOPROC_RESET  _u(0x0)
#define IO_QSPI_GPIO_QSPI_SD2_STATUS_IRQTOPROC_BITS   _u(0x04000000)
#define IO_QSPI_GPIO_QSPI_SD2_STATUS_IRQTOPROC_MSB    _u(26)
#define IO_QSPI_GPIO_QSPI_SD2_STATUS_IRQTOPROC_LSB    _u(26)
#define IO_QSPI_GPIO_QSPI_SD2_STATUS_IRQTOPROC_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_GPIO_QSPI_SD2_STATUS_INFROMPAD
// Description : input signal from pad, before filtering and override are
//               applied
#define IO_QSPI_GPIO_QSPI_SD2_STATUS_INFROMPAD_RESET  _u(0x0)
#define IO_QSPI_GPIO_QSPI_SD2_STATUS_INFROMPAD_BITS   _u(0x00020000)
#define IO_QSPI_GPIO_QSPI_SD2_STATUS_INFROMPAD_MSB    _u(17)
#define IO_QSPI_GPIO_QSPI_SD2_STATUS_INFROMPAD_LSB    _u(17)
#define IO_QSPI_GPIO_QSPI_SD2_STATUS_INFROMPAD_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_GPIO_QSPI_SD2_STATUS_OETOPAD
// Description : output enable to pad after register override is applied
#define IO_QSPI_GPIO_QSPI_SD2_STATUS_OETOPAD_RESET  _u(0x0)
#define IO_QSPI_GPIO_QSPI_SD2_STATUS_OETOPAD_BITS   _u(0x00002000)
#define IO_QSPI_GPIO_QSPI_SD2_STATUS_OETOPAD_MSB    _u(13)
#define IO_QSPI_GPIO_QSPI_SD2_STATUS_OETOPAD_LSB    _u(13)
#define IO_QSPI_GPIO_QSPI_SD2_STATUS_OETOPAD_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_GPIO_QSPI_SD2_STATUS_OUTTOPAD
// Description : output signal to pad after register override is applied
#define IO_QSPI_GPIO_QSPI_SD2_STATUS_OUTTOPAD_RESET  _u(0x0)
#define IO_QSPI_GPIO_QSPI_SD2_STATUS_OUTTOPAD_BITS   _u(0x00000200)
#define IO_QSPI_GPIO_QSPI_SD2_STATUS_OUTTOPAD_MSB    _u(9)
#define IO_QSPI_GPIO_QSPI_SD2_STATUS_OUTTOPAD_LSB    _u(9)
#define IO_QSPI_GPIO_QSPI_SD2_STATUS_OUTTOPAD_ACCESS "RO"
// =============================================================================
// Register    : IO_QSPI_GPIO_QSPI_SD2_CTRL
#define IO_QSPI_GPIO_QSPI_SD2_CTRL_OFFSET _u(0x00000034)
#define IO_QSPI_GPIO_QSPI_SD2_CTRL_BITS   _u(0x3003f01f)
#define IO_QSPI_GPIO_QSPI_SD2_CTRL_RESET  _u(0x0000001f)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_GPIO_QSPI_SD2_CTRL_IRQOVER
//               0x0 -> don't invert the interrupt
//               0x1 -> invert the interrupt
//               0x2 -> drive interrupt low
//               0x3 -> drive interrupt high
#define IO_QSPI_GPIO_QSPI_SD2_CTRL_IRQOVER_RESET  _u(0x0)
#define IO_QSPI_GPIO_QSPI_SD2_CTRL_IRQOVER_BITS   _u(0x30000000)
#define IO_QSPI_GPIO_QSPI_SD2_CTRL_IRQOVER_MSB    _u(29)
#define IO_QSPI_GPIO_QSPI_SD2_CTRL_IRQOVER_LSB    _u(28)
#define IO_QSPI_GPIO_QSPI_SD2_CTRL_IRQOVER_ACCESS "RW"
#define IO_QSPI_GPIO_QSPI_SD2_CTRL_IRQOVER_VALUE_NORMAL _u(0x0)
#define IO_QSPI_GPIO_QSPI_SD2_CTRL_IRQOVER_VALUE_INVERT _u(0x1)
#define IO_QSPI_GPIO_QSPI_SD2_CTRL_IRQOVER_VALUE_LOW _u(0x2)
#define IO_QSPI_GPIO_QSPI_SD2_CTRL_IRQOVER_VALUE_HIGH _u(0x3)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_GPIO_QSPI_SD2_CTRL_INOVER
//               0x0 -> don't invert the peri input
//               0x1 -> invert the peri input
//               0x2 -> drive peri input low
//               0x3 -> drive peri input high
#define IO_QSPI_GPIO_QSPI_SD2_CTRL_INOVER_RESET  _u(0x0)
#define IO_QSPI_GPIO_QSPI_SD2_CTRL_INOVER_BITS   _u(0x00030000)
#define IO_QSPI_GPIO_QSPI_SD2_CTRL_INOVER_MSB    _u(17)
#define IO_QSPI_GPIO_QSPI_SD2_CTRL_INOVER_LSB    _u(16)
#define IO_QSPI_GPIO_QSPI_SD2_CTRL_INOVER_ACCESS "RW"
#define IO_QSPI_GPIO_QSPI_SD2_CTRL_INOVER_VALUE_NORMAL _u(0x0)
#define IO_QSPI_GPIO_QSPI_SD2_CTRL_INOVER_VALUE_INVERT _u(0x1)
#define IO_QSPI_GPIO_QSPI_SD2_CTRL_INOVER_VALUE_LOW _u(0x2)
#define IO_QSPI_GPIO_QSPI_SD2_CTRL_INOVER_VALUE_HIGH _u(0x3)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_GPIO_QSPI_SD2_CTRL_OEOVER
//               0x0 -> drive output enable from peripheral signal selected by funcsel
//               0x1 -> drive output enable from inverse of peripheral signal selected by funcsel
//               0x2 -> disable output
//               0x3 -> enable output
#define IO_QSPI_GPIO_QSPI_SD2_CTRL_OEOVER_RESET  _u(0x0)
#define IO_QSPI_GPIO_QSPI_SD2_CTRL_OEOVER_BITS   _u(0x0000c000)
#define IO_QSPI_GPIO_QSPI_SD2_CTRL_OEOVER_MSB    _u(15)
#define IO_QSPI_GPIO_QSPI_SD2_CTRL_OEOVER_LSB    _u(14)
#define IO_QSPI_GPIO_QSPI_SD2_CTRL_OEOVER_ACCESS "RW"
#define IO_QSPI_GPIO_QSPI_SD2_CTRL_OEOVER_VALUE_NORMAL _u(0x0)
#define IO_QSPI_GPIO_QSPI_SD2_CTRL_OEOVER_VALUE_INVERT _u(0x1)
#define IO_QSPI_GPIO_QSPI_SD2_CTRL_OEOVER_VALUE_DISABLE _u(0x2)
#define IO_QSPI_GPIO_QSPI_SD2_CTRL_OEOVER_VALUE_ENABLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_GPIO_QSPI_SD2_CTRL_OUTOVER
//               0x0 -> drive output from peripheral signal selected by funcsel
//               0x1 -> drive output from inverse of peripheral signal selected by funcsel
//               0x2 -> drive output low
//               0x3 -> drive output high
#define IO_QSPI_GPIO_QSPI_SD2_CTRL_OUTOVER_RESET  _u(0x0)
#define IO_QSPI_GPIO_QSPI_SD2_CTRL_OUTOVER_BITS   _u(0x00003000)
#define IO_QSPI_GPIO_QSPI_SD2_CTRL_OUTOVER_MSB    _u(13)
#define IO_QSPI_GPIO_QSPI_SD2_CTRL_OUTOVER_LSB    _u(12)
#define IO_QSPI_GPIO_QSPI_SD2_CTRL_OUTOVER_ACCESS "RW"
#define IO_QSPI_GPIO_QSPI_SD2_CTRL_OUTOVER_VALUE_NORMAL _u(0x0)
#define IO_QSPI_GPIO_QSPI_SD2_CTRL_OUTOVER_VALUE_INVERT _u(0x1)
#define IO_QSPI_GPIO_QSPI_SD2_CTRL_OUTOVER_VALUE_LOW _u(0x2)
#define IO_QSPI_GPIO_QSPI_SD2_CTRL_OUTOVER_VALUE_HIGH _u(0x3)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_GPIO_QSPI_SD2_CTRL_FUNCSEL
// Description : 0-31 -> selects pin function according to the gpio table
//               31 == NULL
//               0x00 -> xip_sd2
//               0x02 -> uart0_cts
//               0x03 -> i2c1_sda
//               0x05 -> siob_proc_62
//               0x0b -> uart0_tx
//               0x1f -> null
#define IO_QSPI_GPIO_QSPI_SD2_CTRL_FUNCSEL_RESET  _u(0x1f)
#define IO_QSPI_GPIO_QSPI_SD2_CTRL_FUNCSEL_BITS   _u(0x0000001f)
#define IO_QSPI_GPIO_QSPI_SD2_CTRL_FUNCSEL_MSB    _u(4)
#define IO_QSPI_GPIO_QSPI_SD2_CTRL_FUNCSEL_LSB    _u(0)
#define IO_QSPI_GPIO_QSPI_SD2_CTRL_FUNCSEL_ACCESS "RW"
#define IO_QSPI_GPIO_QSPI_SD2_CTRL_FUNCSEL_VALUE_XIP_SD2 _u(0x00)
#define IO_QSPI_GPIO_QSPI_SD2_CTRL_FUNCSEL_VALUE_UART0_CTS _u(0x02)
#define IO_QSPI_GPIO_QSPI_SD2_CTRL_FUNCSEL_VALUE_I2C1_SDA _u(0x03)
#define IO_QSPI_GPIO_QSPI_SD2_CTRL_FUNCSEL_VALUE_SIOB_PROC_62 _u(0x05)
#define IO_QSPI_GPIO_QSPI_SD2_CTRL_FUNCSEL_VALUE_UART0_TX _u(0x0b)
#define IO_QSPI_GPIO_QSPI_SD2_CTRL_FUNCSEL_VALUE_NULL _u(0x1f)
// =============================================================================
// Register    : IO_QSPI_GPIO_QSPI_SD3_STATUS
#define IO_QSPI_GPIO_QSPI_SD3_STATUS_OFFSET _u(0x00000038)
#define IO_QSPI_GPIO_QSPI_SD3_STATUS_BITS   _u(0x04022200)
#define IO_QSPI_GPIO_QSPI_SD3_STATUS_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_GPIO_QSPI_SD3_STATUS_IRQTOPROC
// Description : interrupt to processors, after override is applied
#define IO_QSPI_GPIO_QSPI_SD3_STATUS_IRQTOPROC_RESET  _u(0x0)
#define IO_QSPI_GPIO_QSPI_SD3_STATUS_IRQTOPROC_BITS   _u(0x04000000)
#define IO_QSPI_GPIO_QSPI_SD3_STATUS_IRQTOPROC_MSB    _u(26)
#define IO_QSPI_GPIO_QSPI_SD3_STATUS_IRQTOPROC_LSB    _u(26)
#define IO_QSPI_GPIO_QSPI_SD3_STATUS_IRQTOPROC_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_GPIO_QSPI_SD3_STATUS_INFROMPAD
// Description : input signal from pad, before filtering and override are
//               applied
#define IO_QSPI_GPIO_QSPI_SD3_STATUS_INFROMPAD_RESET  _u(0x0)
#define IO_QSPI_GPIO_QSPI_SD3_STATUS_INFROMPAD_BITS   _u(0x00020000)
#define IO_QSPI_GPIO_QSPI_SD3_STATUS_INFROMPAD_MSB    _u(17)
#define IO_QSPI_GPIO_QSPI_SD3_STATUS_INFROMPAD_LSB    _u(17)
#define IO_QSPI_GPIO_QSPI_SD3_STATUS_INFROMPAD_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_GPIO_QSPI_SD3_STATUS_OETOPAD
// Description : output enable to pad after register override is applied
#define IO_QSPI_GPIO_QSPI_SD3_STATUS_OETOPAD_RESET  _u(0x0)
#define IO_QSPI_GPIO_QSPI_SD3_STATUS_OETOPAD_BITS   _u(0x00002000)
#define IO_QSPI_GPIO_QSPI_SD3_STATUS_OETOPAD_MSB    _u(13)
#define IO_QSPI_GPIO_QSPI_SD3_STATUS_OETOPAD_LSB    _u(13)
#define IO_QSPI_GPIO_QSPI_SD3_STATUS_OETOPAD_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_GPIO_QSPI_SD3_STATUS_OUTTOPAD
// Description : output signal to pad after register override is applied
#define IO_QSPI_GPIO_QSPI_SD3_STATUS_OUTTOPAD_RESET  _u(0x0)
#define IO_QSPI_GPIO_QSPI_SD3_STATUS_OUTTOPAD_BITS   _u(0x00000200)
#define IO_QSPI_GPIO_QSPI_SD3_STATUS_OUTTOPAD_MSB    _u(9)
#define IO_QSPI_GPIO_QSPI_SD3_STATUS_OUTTOPAD_LSB    _u(9)
#define IO_QSPI_GPIO_QSPI_SD3_STATUS_OUTTOPAD_ACCESS "RO"
// =============================================================================
// Register    : IO_QSPI_GPIO_QSPI_SD3_CTRL
#define IO_QSPI_GPIO_QSPI_SD3_CTRL_OFFSET _u(0x0000003c)
#define IO_QSPI_GPIO_QSPI_SD3_CTRL_BITS   _u(0x3003f01f)
#define IO_QSPI_GPIO_QSPI_SD3_CTRL_RESET  _u(0x0000001f)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_GPIO_QSPI_SD3_CTRL_IRQOVER
//               0x0 -> don't invert the interrupt
//               0x1 -> invert the interrupt
//               0x2 -> drive interrupt low
//               0x3 -> drive interrupt high
#define IO_QSPI_GPIO_QSPI_SD3_CTRL_IRQOVER_RESET  _u(0x0)
#define IO_QSPI_GPIO_QSPI_SD3_CTRL_IRQOVER_BITS   _u(0x30000000)
#define IO_QSPI_GPIO_QSPI_SD3_CTRL_IRQOVER_MSB    _u(29)
#define IO_QSPI_GPIO_QSPI_SD3_CTRL_IRQOVER_LSB    _u(28)
#define IO_QSPI_GPIO_QSPI_SD3_CTRL_IRQOVER_ACCESS "RW"
#define IO_QSPI_GPIO_QSPI_SD3_CTRL_IRQOVER_VALUE_NORMAL _u(0x0)
#define IO_QSPI_GPIO_QSPI_SD3_CTRL_IRQOVER_VALUE_INVERT _u(0x1)
#define IO_QSPI_GPIO_QSPI_SD3_CTRL_IRQOVER_VALUE_LOW _u(0x2)
#define IO_QSPI_GPIO_QSPI_SD3_CTRL_IRQOVER_VALUE_HIGH _u(0x3)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_GPIO_QSPI_SD3_CTRL_INOVER
//               0x0 -> don't invert the peri input
//               0x1 -> invert the peri input
//               0x2 -> drive peri input low
//               0x3 -> drive peri input high
#define IO_QSPI_GPIO_QSPI_SD3_CTRL_INOVER_RESET  _u(0x0)
#define IO_QSPI_GPIO_QSPI_SD3_CTRL_INOVER_BITS   _u(0x00030000)
#define IO_QSPI_GPIO_QSPI_SD3_CTRL_INOVER_MSB    _u(17)
#define IO_QSPI_GPIO_QSPI_SD3_CTRL_INOVER_LSB    _u(16)
#define IO_QSPI_GPIO_QSPI_SD3_CTRL_INOVER_ACCESS "RW"
#define IO_QSPI_GPIO_QSPI_SD3_CTRL_INOVER_VALUE_NORMAL _u(0x0)
#define IO_QSPI_GPIO_QSPI_SD3_CTRL_INOVER_VALUE_INVERT _u(0x1)
#define IO_QSPI_GPIO_QSPI_SD3_CTRL_INOVER_VALUE_LOW _u(0x2)
#define IO_QSPI_GPIO_QSPI_SD3_CTRL_INOVER_VALUE_HIGH _u(0x3)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_GPIO_QSPI_SD3_CTRL_OEOVER
//               0x0 -> drive output enable from peripheral signal selected by funcsel
//               0x1 -> drive output enable from inverse of peripheral signal selected by funcsel
//               0x2 -> disable output
//               0x3 -> enable output
#define IO_QSPI_GPIO_QSPI_SD3_CTRL_OEOVER_RESET  _u(0x0)
#define IO_QSPI_GPIO_QSPI_SD3_CTRL_OEOVER_BITS   _u(0x0000c000)
#define IO_QSPI_GPIO_QSPI_SD3_CTRL_OEOVER_MSB    _u(15)
#define IO_QSPI_GPIO_QSPI_SD3_CTRL_OEOVER_LSB    _u(14)
#define IO_QSPI_GPIO_QSPI_SD3_CTRL_OEOVER_ACCESS "RW"
#define IO_QSPI_GPIO_QSPI_SD3_CTRL_OEOVER_VALUE_NORMAL _u(0x0)
#define IO_QSPI_GPIO_QSPI_SD3_CTRL_OEOVER_VALUE_INVERT _u(0x1)
#define IO_QSPI_GPIO_QSPI_SD3_CTRL_OEOVER_VALUE_DISABLE _u(0x2)
#define IO_QSPI_GPIO_QSPI_SD3_CTRL_OEOVER_VALUE_ENABLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_GPIO_QSPI_SD3_CTRL_OUTOVER
//               0x0 -> drive output from peripheral signal selected by funcsel
//               0x1 -> drive output from inverse of peripheral signal selected by funcsel
//               0x2 -> drive output low
//               0x3 -> drive output high
#define IO_QSPI_GPIO_QSPI_SD3_CTRL_OUTOVER_RESET  _u(0x0)
#define IO_QSPI_GPIO_QSPI_SD3_CTRL_OUTOVER_BITS   _u(0x00003000)
#define IO_QSPI_GPIO_QSPI_SD3_CTRL_OUTOVER_MSB    _u(13)
#define IO_QSPI_GPIO_QSPI_SD3_CTRL_OUTOVER_LSB    _u(12)
#define IO_QSPI_GPIO_QSPI_SD3_CTRL_OUTOVER_ACCESS "RW"
#define IO_QSPI_GPIO_QSPI_SD3_CTRL_OUTOVER_VALUE_NORMAL _u(0x0)
#define IO_QSPI_GPIO_QSPI_SD3_CTRL_OUTOVER_VALUE_INVERT _u(0x1)
#define IO_QSPI_GPIO_QSPI_SD3_CTRL_OUTOVER_VALUE_LOW _u(0x2)
#define IO_QSPI_GPIO_QSPI_SD3_CTRL_OUTOVER_VALUE_HIGH _u(0x3)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_GPIO_QSPI_SD3_CTRL_FUNCSEL
// Description : 0-31 -> selects pin function according to the gpio table
//               31 == NULL
//               0x00 -> xip_sd3
//               0x02 -> uart0_rts
//               0x03 -> i2c1_scl
//               0x05 -> siob_proc_63
//               0x0b -> uart0_rx
//               0x1f -> null
#define IO_QSPI_GPIO_QSPI_SD3_CTRL_FUNCSEL_RESET  _u(0x1f)
#define IO_QSPI_GPIO_QSPI_SD3_CTRL_FUNCSEL_BITS   _u(0x0000001f)
#define IO_QSPI_GPIO_QSPI_SD3_CTRL_FUNCSEL_MSB    _u(4)
#define IO_QSPI_GPIO_QSPI_SD3_CTRL_FUNCSEL_LSB    _u(0)
#define IO_QSPI_GPIO_QSPI_SD3_CTRL_FUNCSEL_ACCESS "RW"
#define IO_QSPI_GPIO_QSPI_SD3_CTRL_FUNCSEL_VALUE_XIP_SD3 _u(0x00)
#define IO_QSPI_GPIO_QSPI_SD3_CTRL_FUNCSEL_VALUE_UART0_RTS _u(0x02)
#define IO_QSPI_GPIO_QSPI_SD3_CTRL_FUNCSEL_VALUE_I2C1_SCL _u(0x03)
#define IO_QSPI_GPIO_QSPI_SD3_CTRL_FUNCSEL_VALUE_SIOB_PROC_63 _u(0x05)
#define IO_QSPI_GPIO_QSPI_SD3_CTRL_FUNCSEL_VALUE_UART0_RX _u(0x0b)
#define IO_QSPI_GPIO_QSPI_SD3_CTRL_FUNCSEL_VALUE_NULL _u(0x1f)
// =============================================================================
// Register    : IO_QSPI_IRQSUMMARY_PROC0_SECURE
#define IO_QSPI_IRQSUMMARY_PROC0_SECURE_OFFSET _u(0x00000200)
#define IO_QSPI_IRQSUMMARY_PROC0_SECURE_BITS   _u(0x000000ff)
#define IO_QSPI_IRQSUMMARY_PROC0_SECURE_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_IRQSUMMARY_PROC0_SECURE_GPIO_QSPI_SD3
#define IO_QSPI_IRQSUMMARY_PROC0_SECURE_GPIO_QSPI_SD3_RESET  _u(0x0)
#define IO_QSPI_IRQSUMMARY_PROC0_SECURE_GPIO_QSPI_SD3_BITS   _u(0x00000080)
#define IO_QSPI_IRQSUMMARY_PROC0_SECURE_GPIO_QSPI_SD3_MSB    _u(7)
#define IO_QSPI_IRQSUMMARY_PROC0_SECURE_GPIO_QSPI_SD3_LSB    _u(7)
#define IO_QSPI_IRQSUMMARY_PROC0_SECURE_GPIO_QSPI_SD3_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_IRQSUMMARY_PROC0_SECURE_GPIO_QSPI_SD2
#define IO_QSPI_IRQSUMMARY_PROC0_SECURE_GPIO_QSPI_SD2_RESET  _u(0x0)
#define IO_QSPI_IRQSUMMARY_PROC0_SECURE_GPIO_QSPI_SD2_BITS   _u(0x00000040)
#define IO_QSPI_IRQSUMMARY_PROC0_SECURE_GPIO_QSPI_SD2_MSB    _u(6)
#define IO_QSPI_IRQSUMMARY_PROC0_SECURE_GPIO_QSPI_SD2_LSB    _u(6)
#define IO_QSPI_IRQSUMMARY_PROC0_SECURE_GPIO_QSPI_SD2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_IRQSUMMARY_PROC0_SECURE_GPIO_QSPI_SD1
#define IO_QSPI_IRQSUMMARY_PROC0_SECURE_GPIO_QSPI_SD1_RESET  _u(0x0)
#define IO_QSPI_IRQSUMMARY_PROC0_SECURE_GPIO_QSPI_SD1_BITS   _u(0x00000020)
#define IO_QSPI_IRQSUMMARY_PROC0_SECURE_GPIO_QSPI_SD1_MSB    _u(5)
#define IO_QSPI_IRQSUMMARY_PROC0_SECURE_GPIO_QSPI_SD1_LSB    _u(5)
#define IO_QSPI_IRQSUMMARY_PROC0_SECURE_GPIO_QSPI_SD1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_IRQSUMMARY_PROC0_SECURE_GPIO_QSPI_SD0
#define IO_QSPI_IRQSUMMARY_PROC0_SECURE_GPIO_QSPI_SD0_RESET  _u(0x0)
#define IO_QSPI_IRQSUMMARY_PROC0_SECURE_GPIO_QSPI_SD0_BITS   _u(0x00000010)
#define IO_QSPI_IRQSUMMARY_PROC0_SECURE_GPIO_QSPI_SD0_MSB    _u(4)
#define IO_QSPI_IRQSUMMARY_PROC0_SECURE_GPIO_QSPI_SD0_LSB    _u(4)
#define IO_QSPI_IRQSUMMARY_PROC0_SECURE_GPIO_QSPI_SD0_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_IRQSUMMARY_PROC0_SECURE_GPIO_QSPI_SS
#define IO_QSPI_IRQSUMMARY_PROC0_SECURE_GPIO_QSPI_SS_RESET  _u(0x0)
#define IO_QSPI_IRQSUMMARY_PROC0_SECURE_GPIO_QSPI_SS_BITS   _u(0x00000008)
#define IO_QSPI_IRQSUMMARY_PROC0_SECURE_GPIO_QSPI_SS_MSB    _u(3)
#define IO_QSPI_IRQSUMMARY_PROC0_SECURE_GPIO_QSPI_SS_LSB    _u(3)
#define IO_QSPI_IRQSUMMARY_PROC0_SECURE_GPIO_QSPI_SS_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_IRQSUMMARY_PROC0_SECURE_GPIO_QSPI_SCLK
#define IO_QSPI_IRQSUMMARY_PROC0_SECURE_GPIO_QSPI_SCLK_RESET  _u(0x0)
#define IO_QSPI_IRQSUMMARY_PROC0_SECURE_GPIO_QSPI_SCLK_BITS   _u(0x00000004)
#define IO_QSPI_IRQSUMMARY_PROC0_SECURE_GPIO_QSPI_SCLK_MSB    _u(2)
#define IO_QSPI_IRQSUMMARY_PROC0_SECURE_GPIO_QSPI_SCLK_LSB    _u(2)
#define IO_QSPI_IRQSUMMARY_PROC0_SECURE_GPIO_QSPI_SCLK_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_IRQSUMMARY_PROC0_SECURE_USBPHY_DM
#define IO_QSPI_IRQSUMMARY_PROC0_SECURE_USBPHY_DM_RESET  _u(0x0)
#define IO_QSPI_IRQSUMMARY_PROC0_SECURE_USBPHY_DM_BITS   _u(0x00000002)
#define IO_QSPI_IRQSUMMARY_PROC0_SECURE_USBPHY_DM_MSB    _u(1)
#define IO_QSPI_IRQSUMMARY_PROC0_SECURE_USBPHY_DM_LSB    _u(1)
#define IO_QSPI_IRQSUMMARY_PROC0_SECURE_USBPHY_DM_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_IRQSUMMARY_PROC0_SECURE_USBPHY_DP
#define IO_QSPI_IRQSUMMARY_PROC0_SECURE_USBPHY_DP_RESET  _u(0x0)
#define IO_QSPI_IRQSUMMARY_PROC0_SECURE_USBPHY_DP_BITS   _u(0x00000001)
#define IO_QSPI_IRQSUMMARY_PROC0_SECURE_USBPHY_DP_MSB    _u(0)
#define IO_QSPI_IRQSUMMARY_PROC0_SECURE_USBPHY_DP_LSB    _u(0)
#define IO_QSPI_IRQSUMMARY_PROC0_SECURE_USBPHY_DP_ACCESS "RO"
// =============================================================================
// Register    : IO_QSPI_IRQSUMMARY_PROC0_NONSECURE
#define IO_QSPI_IRQSUMMARY_PROC0_NONSECURE_OFFSET _u(0x00000204)
#define IO_QSPI_IRQSUMMARY_PROC0_NONSECURE_BITS   _u(0x000000ff)
#define IO_QSPI_IRQSUMMARY_PROC0_NONSECURE_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_IRQSUMMARY_PROC0_NONSECURE_GPIO_QSPI_SD3
#define IO_QSPI_IRQSUMMARY_PROC0_NONSECURE_GPIO_QSPI_SD3_RESET  _u(0x0)
#define IO_QSPI_IRQSUMMARY_PROC0_NONSECURE_GPIO_QSPI_SD3_BITS   _u(0x00000080)
#define IO_QSPI_IRQSUMMARY_PROC0_NONSECURE_GPIO_QSPI_SD3_MSB    _u(7)
#define IO_QSPI_IRQSUMMARY_PROC0_NONSECURE_GPIO_QSPI_SD3_LSB    _u(7)
#define IO_QSPI_IRQSUMMARY_PROC0_NONSECURE_GPIO_QSPI_SD3_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_IRQSUMMARY_PROC0_NONSECURE_GPIO_QSPI_SD2
#define IO_QSPI_IRQSUMMARY_PROC0_NONSECURE_GPIO_QSPI_SD2_RESET  _u(0x0)
#define IO_QSPI_IRQSUMMARY_PROC0_NONSECURE_GPIO_QSPI_SD2_BITS   _u(0x00000040)
#define IO_QSPI_IRQSUMMARY_PROC0_NONSECURE_GPIO_QSPI_SD2_MSB    _u(6)
#define IO_QSPI_IRQSUMMARY_PROC0_NONSECURE_GPIO_QSPI_SD2_LSB    _u(6)
#define IO_QSPI_IRQSUMMARY_PROC0_NONSECURE_GPIO_QSPI_SD2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_IRQSUMMARY_PROC0_NONSECURE_GPIO_QSPI_SD1
#define IO_QSPI_IRQSUMMARY_PROC0_NONSECURE_GPIO_QSPI_SD1_RESET  _u(0x0)
#define IO_QSPI_IRQSUMMARY_PROC0_NONSECURE_GPIO_QSPI_SD1_BITS   _u(0x00000020)
#define IO_QSPI_IRQSUMMARY_PROC0_NONSECURE_GPIO_QSPI_SD1_MSB    _u(5)
#define IO_QSPI_IRQSUMMARY_PROC0_NONSECURE_GPIO_QSPI_SD1_LSB    _u(5)
#define IO_QSPI_IRQSUMMARY_PROC0_NONSECURE_GPIO_QSPI_SD1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_IRQSUMMARY_PROC0_NONSECURE_GPIO_QSPI_SD0
#define IO_QSPI_IRQSUMMARY_PROC0_NONSECURE_GPIO_QSPI_SD0_RESET  _u(0x0)
#define IO_QSPI_IRQSUMMARY_PROC0_NONSECURE_GPIO_QSPI_SD0_BITS   _u(0x00000010)
#define IO_QSPI_IRQSUMMARY_PROC0_NONSECURE_GPIO_QSPI_SD0_MSB    _u(4)
#define IO_QSPI_IRQSUMMARY_PROC0_NONSECURE_GPIO_QSPI_SD0_LSB    _u(4)
#define IO_QSPI_IRQSUMMARY_PROC0_NONSECURE_GPIO_QSPI_SD0_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_IRQSUMMARY_PROC0_NONSECURE_GPIO_QSPI_SS
#define IO_QSPI_IRQSUMMARY_PROC0_NONSECURE_GPIO_QSPI_SS_RESET  _u(0x0)
#define IO_QSPI_IRQSUMMARY_PROC0_NONSECURE_GPIO_QSPI_SS_BITS   _u(0x00000008)
#define IO_QSPI_IRQSUMMARY_PROC0_NONSECURE_GPIO_QSPI_SS_MSB    _u(3)
#define IO_QSPI_IRQSUMMARY_PROC0_NONSECURE_GPIO_QSPI_SS_LSB    _u(3)
#define IO_QSPI_IRQSUMMARY_PROC0_NONSECURE_GPIO_QSPI_SS_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_IRQSUMMARY_PROC0_NONSECURE_GPIO_QSPI_SCLK
#define IO_QSPI_IRQSUMMARY_PROC0_NONSECURE_GPIO_QSPI_SCLK_RESET  _u(0x0)
#define IO_QSPI_IRQSUMMARY_PROC0_NONSECURE_GPIO_QSPI_SCLK_BITS   _u(0x00000004)
#define IO_QSPI_IRQSUMMARY_PROC0_NONSECURE_GPIO_QSPI_SCLK_MSB    _u(2)
#define IO_QSPI_IRQSUMMARY_PROC0_NONSECURE_GPIO_QSPI_SCLK_LSB    _u(2)
#define IO_QSPI_IRQSUMMARY_PROC0_NONSECURE_GPIO_QSPI_SCLK_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_IRQSUMMARY_PROC0_NONSECURE_USBPHY_DM
#define IO_QSPI_IRQSUMMARY_PROC0_NONSECURE_USBPHY_DM_RESET  _u(0x0)
#define IO_QSPI_IRQSUMMARY_PROC0_NONSECURE_USBPHY_DM_BITS   _u(0x00000002)
#define IO_QSPI_IRQSUMMARY_PROC0_NONSECURE_USBPHY_DM_MSB    _u(1)
#define IO_QSPI_IRQSUMMARY_PROC0_NONSECURE_USBPHY_DM_LSB    _u(1)
#define IO_QSPI_IRQSUMMARY_PROC0_NONSECURE_USBPHY_DM_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_IRQSUMMARY_PROC0_NONSECURE_USBPHY_DP
#define IO_QSPI_IRQSUMMARY_PROC0_NONSECURE_USBPHY_DP_RESET  _u(0x0)
#define IO_QSPI_IRQSUMMARY_PROC0_NONSECURE_USBPHY_DP_BITS   _u(0x00000001)
#define IO_QSPI_IRQSUMMARY_PROC0_NONSECURE_USBPHY_DP_MSB    _u(0)
#define IO_QSPI_IRQSUMMARY_PROC0_NONSECURE_USBPHY_DP_LSB    _u(0)
#define IO_QSPI_IRQSUMMARY_PROC0_NONSECURE_USBPHY_DP_ACCESS "RO"
// =============================================================================
// Register    : IO_QSPI_IRQSUMMARY_PROC1_SECURE
#define IO_QSPI_IRQSUMMARY_PROC1_SECURE_OFFSET _u(0x00000208)
#define IO_QSPI_IRQSUMMARY_PROC1_SECURE_BITS   _u(0x000000ff)
#define IO_QSPI_IRQSUMMARY_PROC1_SECURE_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_IRQSUMMARY_PROC1_SECURE_GPIO_QSPI_SD3
#define IO_QSPI_IRQSUMMARY_PROC1_SECURE_GPIO_QSPI_SD3_RESET  _u(0x0)
#define IO_QSPI_IRQSUMMARY_PROC1_SECURE_GPIO_QSPI_SD3_BITS   _u(0x00000080)
#define IO_QSPI_IRQSUMMARY_PROC1_SECURE_GPIO_QSPI_SD3_MSB    _u(7)
#define IO_QSPI_IRQSUMMARY_PROC1_SECURE_GPIO_QSPI_SD3_LSB    _u(7)
#define IO_QSPI_IRQSUMMARY_PROC1_SECURE_GPIO_QSPI_SD3_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_IRQSUMMARY_PROC1_SECURE_GPIO_QSPI_SD2
#define IO_QSPI_IRQSUMMARY_PROC1_SECURE_GPIO_QSPI_SD2_RESET  _u(0x0)
#define IO_QSPI_IRQSUMMARY_PROC1_SECURE_GPIO_QSPI_SD2_BITS   _u(0x00000040)
#define IO_QSPI_IRQSUMMARY_PROC1_SECURE_GPIO_QSPI_SD2_MSB    _u(6)
#define IO_QSPI_IRQSUMMARY_PROC1_SECURE_GPIO_QSPI_SD2_LSB    _u(6)
#define IO_QSPI_IRQSUMMARY_PROC1_SECURE_GPIO_QSPI_SD2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_IRQSUMMARY_PROC1_SECURE_GPIO_QSPI_SD1
#define IO_QSPI_IRQSUMMARY_PROC1_SECURE_GPIO_QSPI_SD1_RESET  _u(0x0)
#define IO_QSPI_IRQSUMMARY_PROC1_SECURE_GPIO_QSPI_SD1_BITS   _u(0x00000020)
#define IO_QSPI_IRQSUMMARY_PROC1_SECURE_GPIO_QSPI_SD1_MSB    _u(5)
#define IO_QSPI_IRQSUMMARY_PROC1_SECURE_GPIO_QSPI_SD1_LSB    _u(5)
#define IO_QSPI_IRQSUMMARY_PROC1_SECURE_GPIO_QSPI_SD1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_IRQSUMMARY_PROC1_SECURE_GPIO_QSPI_SD0
#define IO_QSPI_IRQSUMMARY_PROC1_SECURE_GPIO_QSPI_SD0_RESET  _u(0x0)
#define IO_QSPI_IRQSUMMARY_PROC1_SECURE_GPIO_QSPI_SD0_BITS   _u(0x00000010)
#define IO_QSPI_IRQSUMMARY_PROC1_SECURE_GPIO_QSPI_SD0_MSB    _u(4)
#define IO_QSPI_IRQSUMMARY_PROC1_SECURE_GPIO_QSPI_SD0_LSB    _u(4)
#define IO_QSPI_IRQSUMMARY_PROC1_SECURE_GPIO_QSPI_SD0_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_IRQSUMMARY_PROC1_SECURE_GPIO_QSPI_SS
#define IO_QSPI_IRQSUMMARY_PROC1_SECURE_GPIO_QSPI_SS_RESET  _u(0x0)
#define IO_QSPI_IRQSUMMARY_PROC1_SECURE_GPIO_QSPI_SS_BITS   _u(0x00000008)
#define IO_QSPI_IRQSUMMARY_PROC1_SECURE_GPIO_QSPI_SS_MSB    _u(3)
#define IO_QSPI_IRQSUMMARY_PROC1_SECURE_GPIO_QSPI_SS_LSB    _u(3)
#define IO_QSPI_IRQSUMMARY_PROC1_SECURE_GPIO_QSPI_SS_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_IRQSUMMARY_PROC1_SECURE_GPIO_QSPI_SCLK
#define IO_QSPI_IRQSUMMARY_PROC1_SECURE_GPIO_QSPI_SCLK_RESET  _u(0x0)
#define IO_QSPI_IRQSUMMARY_PROC1_SECURE_GPIO_QSPI_SCLK_BITS   _u(0x00000004)
#define IO_QSPI_IRQSUMMARY_PROC1_SECURE_GPIO_QSPI_SCLK_MSB    _u(2)
#define IO_QSPI_IRQSUMMARY_PROC1_SECURE_GPIO_QSPI_SCLK_LSB    _u(2)
#define IO_QSPI_IRQSUMMARY_PROC1_SECURE_GPIO_QSPI_SCLK_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_IRQSUMMARY_PROC1_SECURE_USBPHY_DM
#define IO_QSPI_IRQSUMMARY_PROC1_SECURE_USBPHY_DM_RESET  _u(0x0)
#define IO_QSPI_IRQSUMMARY_PROC1_SECURE_USBPHY_DM_BITS   _u(0x00000002)
#define IO_QSPI_IRQSUMMARY_PROC1_SECURE_USBPHY_DM_MSB    _u(1)
#define IO_QSPI_IRQSUMMARY_PROC1_SECURE_USBPHY_DM_LSB    _u(1)
#define IO_QSPI_IRQSUMMARY_PROC1_SECURE_USBPHY_DM_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_IRQSUMMARY_PROC1_SECURE_USBPHY_DP
#define IO_QSPI_IRQSUMMARY_PROC1_SECURE_USBPHY_DP_RESET  _u(0x0)
#define IO_QSPI_IRQSUMMARY_PROC1_SECURE_USBPHY_DP_BITS   _u(0x00000001)
#define IO_QSPI_IRQSUMMARY_PROC1_SECURE_USBPHY_DP_MSB    _u(0)
#define IO_QSPI_IRQSUMMARY_PROC1_SECURE_USBPHY_DP_LSB    _u(0)
#define IO_QSPI_IRQSUMMARY_PROC1_SECURE_USBPHY_DP_ACCESS "RO"
// =============================================================================
// Register    : IO_QSPI_IRQSUMMARY_PROC1_NONSECURE
#define IO_QSPI_IRQSUMMARY_PROC1_NONSECURE_OFFSET _u(0x0000020c)
#define IO_QSPI_IRQSUMMARY_PROC1_NONSECURE_BITS   _u(0x000000ff)
#define IO_QSPI_IRQSUMMARY_PROC1_NONSECURE_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_IRQSUMMARY_PROC1_NONSECURE_GPIO_QSPI_SD3
#define IO_QSPI_IRQSUMMARY_PROC1_NONSECURE_GPIO_QSPI_SD3_RESET  _u(0x0)
#define IO_QSPI_IRQSUMMARY_PROC1_NONSECURE_GPIO_QSPI_SD3_BITS   _u(0x00000080)
#define IO_QSPI_IRQSUMMARY_PROC1_NONSECURE_GPIO_QSPI_SD3_MSB    _u(7)
#define IO_QSPI_IRQSUMMARY_PROC1_NONSECURE_GPIO_QSPI_SD3_LSB    _u(7)
#define IO_QSPI_IRQSUMMARY_PROC1_NONSECURE_GPIO_QSPI_SD3_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_IRQSUMMARY_PROC1_NONSECURE_GPIO_QSPI_SD2
#define IO_QSPI_IRQSUMMARY_PROC1_NONSECURE_GPIO_QSPI_SD2_RESET  _u(0x0)
#define IO_QSPI_IRQSUMMARY_PROC1_NONSECURE_GPIO_QSPI_SD2_BITS   _u(0x00000040)
#define IO_QSPI_IRQSUMMARY_PROC1_NONSECURE_GPIO_QSPI_SD2_MSB    _u(6)
#define IO_QSPI_IRQSUMMARY_PROC1_NONSECURE_GPIO_QSPI_SD2_LSB    _u(6)
#define IO_QSPI_IRQSUMMARY_PROC1_NONSECURE_GPIO_QSPI_SD2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_IRQSUMMARY_PROC1_NONSECURE_GPIO_QSPI_SD1
#define IO_QSPI_IRQSUMMARY_PROC1_NONSECURE_GPIO_QSPI_SD1_RESET  _u(0x0)
#define IO_QSPI_IRQSUMMARY_PROC1_NONSECURE_GPIO_QSPI_SD1_BITS   _u(0x00000020)
#define IO_QSPI_IRQSUMMARY_PROC1_NONSECURE_GPIO_QSPI_SD1_MSB    _u(5)
#define IO_QSPI_IRQSUMMARY_PROC1_NONSECURE_GPIO_QSPI_SD1_LSB    _u(5)
#define IO_QSPI_IRQSUMMARY_PROC1_NONSECURE_GPIO_QSPI_SD1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_IRQSUMMARY_PROC1_NONSECURE_GPIO_QSPI_SD0
#define IO_QSPI_IRQSUMMARY_PROC1_NONSECURE_GPIO_QSPI_SD0_RESET  _u(0x0)
#define IO_QSPI_IRQSUMMARY_PROC1_NONSECURE_GPIO_QSPI_SD0_BITS   _u(0x00000010)
#define IO_QSPI_IRQSUMMARY_PROC1_NONSECURE_GPIO_QSPI_SD0_MSB    _u(4)
#define IO_QSPI_IRQSUMMARY_PROC1_NONSECURE_GPIO_QSPI_SD0_LSB    _u(4)
#define IO_QSPI_IRQSUMMARY_PROC1_NONSECURE_GPIO_QSPI_SD0_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_IRQSUMMARY_PROC1_NONSECURE_GPIO_QSPI_SS
#define IO_QSPI_IRQSUMMARY_PROC1_NONSECURE_GPIO_QSPI_SS_RESET  _u(0x0)
#define IO_QSPI_IRQSUMMARY_PROC1_NONSECURE_GPIO_QSPI_SS_BITS   _u(0x00000008)
#define IO_QSPI_IRQSUMMARY_PROC1_NONSECURE_GPIO_QSPI_SS_MSB    _u(3)
#define IO_QSPI_IRQSUMMARY_PROC1_NONSECURE_GPIO_QSPI_SS_LSB    _u(3)
#define IO_QSPI_IRQSUMMARY_PROC1_NONSECURE_GPIO_QSPI_SS_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_IRQSUMMARY_PROC1_NONSECURE_GPIO_QSPI_SCLK
#define IO_QSPI_IRQSUMMARY_PROC1_NONSECURE_GPIO_QSPI_SCLK_RESET  _u(0x0)
#define IO_QSPI_IRQSUMMARY_PROC1_NONSECURE_GPIO_QSPI_SCLK_BITS   _u(0x00000004)
#define IO_QSPI_IRQSUMMARY_PROC1_NONSECURE_GPIO_QSPI_SCLK_MSB    _u(2)
#define IO_QSPI_IRQSUMMARY_PROC1_NONSECURE_GPIO_QSPI_SCLK_LSB    _u(2)
#define IO_QSPI_IRQSUMMARY_PROC1_NONSECURE_GPIO_QSPI_SCLK_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_IRQSUMMARY_PROC1_NONSECURE_USBPHY_DM
#define IO_QSPI_IRQSUMMARY_PROC1_NONSECURE_USBPHY_DM_RESET  _u(0x0)
#define IO_QSPI_IRQSUMMARY_PROC1_NONSECURE_USBPHY_DM_BITS   _u(0x00000002)
#define IO_QSPI_IRQSUMMARY_PROC1_NONSECURE_USBPHY_DM_MSB    _u(1)
#define IO_QSPI_IRQSUMMARY_PROC1_NONSECURE_USBPHY_DM_LSB    _u(1)
#define IO_QSPI_IRQSUMMARY_PROC1_NONSECURE_USBPHY_DM_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_IRQSUMMARY_PROC1_NONSECURE_USBPHY_DP
#define IO_QSPI_IRQSUMMARY_PROC1_NONSECURE_USBPHY_DP_RESET  _u(0x0)
#define IO_QSPI_IRQSUMMARY_PROC1_NONSECURE_USBPHY_DP_BITS   _u(0x00000001)
#define IO_QSPI_IRQSUMMARY_PROC1_NONSECURE_USBPHY_DP_MSB    _u(0)
#define IO_QSPI_IRQSUMMARY_PROC1_NONSECURE_USBPHY_DP_LSB    _u(0)
#define IO_QSPI_IRQSUMMARY_PROC1_NONSECURE_USBPHY_DP_ACCESS "RO"
// =============================================================================
// Register    : IO_QSPI_IRQSUMMARY_DORMANT_WAKE_SECURE
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_SECURE_OFFSET _u(0x00000210)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_SECURE_BITS   _u(0x000000ff)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_SECURE_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_IRQSUMMARY_DORMANT_WAKE_SECURE_GPIO_QSPI_SD3
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_SECURE_GPIO_QSPI_SD3_RESET  _u(0x0)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_SECURE_GPIO_QSPI_SD3_BITS   _u(0x00000080)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_SECURE_GPIO_QSPI_SD3_MSB    _u(7)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_SECURE_GPIO_QSPI_SD3_LSB    _u(7)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_SECURE_GPIO_QSPI_SD3_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_IRQSUMMARY_DORMANT_WAKE_SECURE_GPIO_QSPI_SD2
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_SECURE_GPIO_QSPI_SD2_RESET  _u(0x0)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_SECURE_GPIO_QSPI_SD2_BITS   _u(0x00000040)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_SECURE_GPIO_QSPI_SD2_MSB    _u(6)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_SECURE_GPIO_QSPI_SD2_LSB    _u(6)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_SECURE_GPIO_QSPI_SD2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_IRQSUMMARY_DORMANT_WAKE_SECURE_GPIO_QSPI_SD1
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_SECURE_GPIO_QSPI_SD1_RESET  _u(0x0)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_SECURE_GPIO_QSPI_SD1_BITS   _u(0x00000020)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_SECURE_GPIO_QSPI_SD1_MSB    _u(5)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_SECURE_GPIO_QSPI_SD1_LSB    _u(5)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_SECURE_GPIO_QSPI_SD1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_IRQSUMMARY_DORMANT_WAKE_SECURE_GPIO_QSPI_SD0
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_SECURE_GPIO_QSPI_SD0_RESET  _u(0x0)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_SECURE_GPIO_QSPI_SD0_BITS   _u(0x00000010)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_SECURE_GPIO_QSPI_SD0_MSB    _u(4)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_SECURE_GPIO_QSPI_SD0_LSB    _u(4)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_SECURE_GPIO_QSPI_SD0_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_IRQSUMMARY_DORMANT_WAKE_SECURE_GPIO_QSPI_SS
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_SECURE_GPIO_QSPI_SS_RESET  _u(0x0)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_SECURE_GPIO_QSPI_SS_BITS   _u(0x00000008)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_SECURE_GPIO_QSPI_SS_MSB    _u(3)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_SECURE_GPIO_QSPI_SS_LSB    _u(3)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_SECURE_GPIO_QSPI_SS_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_IRQSUMMARY_DORMANT_WAKE_SECURE_GPIO_QSPI_SCLK
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_SECURE_GPIO_QSPI_SCLK_RESET  _u(0x0)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_SECURE_GPIO_QSPI_SCLK_BITS   _u(0x00000004)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_SECURE_GPIO_QSPI_SCLK_MSB    _u(2)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_SECURE_GPIO_QSPI_SCLK_LSB    _u(2)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_SECURE_GPIO_QSPI_SCLK_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_IRQSUMMARY_DORMANT_WAKE_SECURE_USBPHY_DM
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_SECURE_USBPHY_DM_RESET  _u(0x0)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_SECURE_USBPHY_DM_BITS   _u(0x00000002)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_SECURE_USBPHY_DM_MSB    _u(1)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_SECURE_USBPHY_DM_LSB    _u(1)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_SECURE_USBPHY_DM_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_IRQSUMMARY_DORMANT_WAKE_SECURE_USBPHY_DP
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_SECURE_USBPHY_DP_RESET  _u(0x0)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_SECURE_USBPHY_DP_BITS   _u(0x00000001)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_SECURE_USBPHY_DP_MSB    _u(0)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_SECURE_USBPHY_DP_LSB    _u(0)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_SECURE_USBPHY_DP_ACCESS "RO"
// =============================================================================
// Register    : IO_QSPI_IRQSUMMARY_DORMANT_WAKE_NONSECURE
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_NONSECURE_OFFSET _u(0x00000214)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_NONSECURE_BITS   _u(0x000000ff)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_NONSECURE_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_IRQSUMMARY_DORMANT_WAKE_NONSECURE_GPIO_QSPI_SD3
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_NONSECURE_GPIO_QSPI_SD3_RESET  _u(0x0)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_NONSECURE_GPIO_QSPI_SD3_BITS   _u(0x00000080)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_NONSECURE_GPIO_QSPI_SD3_MSB    _u(7)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_NONSECURE_GPIO_QSPI_SD3_LSB    _u(7)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_NONSECURE_GPIO_QSPI_SD3_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_IRQSUMMARY_DORMANT_WAKE_NONSECURE_GPIO_QSPI_SD2
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_NONSECURE_GPIO_QSPI_SD2_RESET  _u(0x0)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_NONSECURE_GPIO_QSPI_SD2_BITS   _u(0x00000040)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_NONSECURE_GPIO_QSPI_SD2_MSB    _u(6)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_NONSECURE_GPIO_QSPI_SD2_LSB    _u(6)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_NONSECURE_GPIO_QSPI_SD2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_IRQSUMMARY_DORMANT_WAKE_NONSECURE_GPIO_QSPI_SD1
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_NONSECURE_GPIO_QSPI_SD1_RESET  _u(0x0)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_NONSECURE_GPIO_QSPI_SD1_BITS   _u(0x00000020)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_NONSECURE_GPIO_QSPI_SD1_MSB    _u(5)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_NONSECURE_GPIO_QSPI_SD1_LSB    _u(5)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_NONSECURE_GPIO_QSPI_SD1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_IRQSUMMARY_DORMANT_WAKE_NONSECURE_GPIO_QSPI_SD0
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_NONSECURE_GPIO_QSPI_SD0_RESET  _u(0x0)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_NONSECURE_GPIO_QSPI_SD0_BITS   _u(0x00000010)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_NONSECURE_GPIO_QSPI_SD0_MSB    _u(4)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_NONSECURE_GPIO_QSPI_SD0_LSB    _u(4)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_NONSECURE_GPIO_QSPI_SD0_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_IRQSUMMARY_DORMANT_WAKE_NONSECURE_GPIO_QSPI_SS
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_NONSECURE_GPIO_QSPI_SS_RESET  _u(0x0)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_NONSECURE_GPIO_QSPI_SS_BITS   _u(0x00000008)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_NONSECURE_GPIO_QSPI_SS_MSB    _u(3)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_NONSECURE_GPIO_QSPI_SS_LSB    _u(3)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_NONSECURE_GPIO_QSPI_SS_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_IRQSUMMARY_DORMANT_WAKE_NONSECURE_GPIO_QSPI_SCLK
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_NONSECURE_GPIO_QSPI_SCLK_RESET  _u(0x0)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_NONSECURE_GPIO_QSPI_SCLK_BITS   _u(0x00000004)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_NONSECURE_GPIO_QSPI_SCLK_MSB    _u(2)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_NONSECURE_GPIO_QSPI_SCLK_LSB    _u(2)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_NONSECURE_GPIO_QSPI_SCLK_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_IRQSUMMARY_DORMANT_WAKE_NONSECURE_USBPHY_DM
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_NONSECURE_USBPHY_DM_RESET  _u(0x0)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_NONSECURE_USBPHY_DM_BITS   _u(0x00000002)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_NONSECURE_USBPHY_DM_MSB    _u(1)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_NONSECURE_USBPHY_DM_LSB    _u(1)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_NONSECURE_USBPHY_DM_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_IRQSUMMARY_DORMANT_WAKE_NONSECURE_USBPHY_DP
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_NONSECURE_USBPHY_DP_RESET  _u(0x0)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_NONSECURE_USBPHY_DP_BITS   _u(0x00000001)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_NONSECURE_USBPHY_DP_MSB    _u(0)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_NONSECURE_USBPHY_DP_LSB    _u(0)
#define IO_QSPI_IRQSUMMARY_DORMANT_WAKE_NONSECURE_USBPHY_DP_ACCESS "RO"
// =============================================================================
// Register    : IO_QSPI_INTR
// Description : Raw Interrupts
#define IO_QSPI_INTR_OFFSET _u(0x00000218)
#define IO_QSPI_INTR_BITS   _u(0xffffffff)
#define IO_QSPI_INTR_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_INTR_GPIO_QSPI_SD3_EDGE_HIGH
#define IO_QSPI_INTR_GPIO_QSPI_SD3_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_INTR_GPIO_QSPI_SD3_EDGE_HIGH_BITS   _u(0x80000000)
#define IO_QSPI_INTR_GPIO_QSPI_SD3_EDGE_HIGH_MSB    _u(31)
#define IO_QSPI_INTR_GPIO_QSPI_SD3_EDGE_HIGH_LSB    _u(31)
#define IO_QSPI_INTR_GPIO_QSPI_SD3_EDGE_HIGH_ACCESS "WC"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_INTR_GPIO_QSPI_SD3_EDGE_LOW
#define IO_QSPI_INTR_GPIO_QSPI_SD3_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_INTR_GPIO_QSPI_SD3_EDGE_LOW_BITS   _u(0x40000000)
#define IO_QSPI_INTR_GPIO_QSPI_SD3_EDGE_LOW_MSB    _u(30)
#define IO_QSPI_INTR_GPIO_QSPI_SD3_EDGE_LOW_LSB    _u(30)
#define IO_QSPI_INTR_GPIO_QSPI_SD3_EDGE_LOW_ACCESS "WC"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_INTR_GPIO_QSPI_SD3_LEVEL_HIGH
#define IO_QSPI_INTR_GPIO_QSPI_SD3_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_INTR_GPIO_QSPI_SD3_LEVEL_HIGH_BITS   _u(0x20000000)
#define IO_QSPI_INTR_GPIO_QSPI_SD3_LEVEL_HIGH_MSB    _u(29)
#define IO_QSPI_INTR_GPIO_QSPI_SD3_LEVEL_HIGH_LSB    _u(29)
#define IO_QSPI_INTR_GPIO_QSPI_SD3_LEVEL_HIGH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_INTR_GPIO_QSPI_SD3_LEVEL_LOW
#define IO_QSPI_INTR_GPIO_QSPI_SD3_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_INTR_GPIO_QSPI_SD3_LEVEL_LOW_BITS   _u(0x10000000)
#define IO_QSPI_INTR_GPIO_QSPI_SD3_LEVEL_LOW_MSB    _u(28)
#define IO_QSPI_INTR_GPIO_QSPI_SD3_LEVEL_LOW_LSB    _u(28)
#define IO_QSPI_INTR_GPIO_QSPI_SD3_LEVEL_LOW_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_INTR_GPIO_QSPI_SD2_EDGE_HIGH
#define IO_QSPI_INTR_GPIO_QSPI_SD2_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_INTR_GPIO_QSPI_SD2_EDGE_HIGH_BITS   _u(0x08000000)
#define IO_QSPI_INTR_GPIO_QSPI_SD2_EDGE_HIGH_MSB    _u(27)
#define IO_QSPI_INTR_GPIO_QSPI_SD2_EDGE_HIGH_LSB    _u(27)
#define IO_QSPI_INTR_GPIO_QSPI_SD2_EDGE_HIGH_ACCESS "WC"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_INTR_GPIO_QSPI_SD2_EDGE_LOW
#define IO_QSPI_INTR_GPIO_QSPI_SD2_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_INTR_GPIO_QSPI_SD2_EDGE_LOW_BITS   _u(0x04000000)
#define IO_QSPI_INTR_GPIO_QSPI_SD2_EDGE_LOW_MSB    _u(26)
#define IO_QSPI_INTR_GPIO_QSPI_SD2_EDGE_LOW_LSB    _u(26)
#define IO_QSPI_INTR_GPIO_QSPI_SD2_EDGE_LOW_ACCESS "WC"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_INTR_GPIO_QSPI_SD2_LEVEL_HIGH
#define IO_QSPI_INTR_GPIO_QSPI_SD2_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_INTR_GPIO_QSPI_SD2_LEVEL_HIGH_BITS   _u(0x02000000)
#define IO_QSPI_INTR_GPIO_QSPI_SD2_LEVEL_HIGH_MSB    _u(25)
#define IO_QSPI_INTR_GPIO_QSPI_SD2_LEVEL_HIGH_LSB    _u(25)
#define IO_QSPI_INTR_GPIO_QSPI_SD2_LEVEL_HIGH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_INTR_GPIO_QSPI_SD2_LEVEL_LOW
#define IO_QSPI_INTR_GPIO_QSPI_SD2_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_INTR_GPIO_QSPI_SD2_LEVEL_LOW_BITS   _u(0x01000000)
#define IO_QSPI_INTR_GPIO_QSPI_SD2_LEVEL_LOW_MSB    _u(24)
#define IO_QSPI_INTR_GPIO_QSPI_SD2_LEVEL_LOW_LSB    _u(24)
#define IO_QSPI_INTR_GPIO_QSPI_SD2_LEVEL_LOW_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_INTR_GPIO_QSPI_SD1_EDGE_HIGH
#define IO_QSPI_INTR_GPIO_QSPI_SD1_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_INTR_GPIO_QSPI_SD1_EDGE_HIGH_BITS   _u(0x00800000)
#define IO_QSPI_INTR_GPIO_QSPI_SD1_EDGE_HIGH_MSB    _u(23)
#define IO_QSPI_INTR_GPIO_QSPI_SD1_EDGE_HIGH_LSB    _u(23)
#define IO_QSPI_INTR_GPIO_QSPI_SD1_EDGE_HIGH_ACCESS "WC"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_INTR_GPIO_QSPI_SD1_EDGE_LOW
#define IO_QSPI_INTR_GPIO_QSPI_SD1_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_INTR_GPIO_QSPI_SD1_EDGE_LOW_BITS   _u(0x00400000)
#define IO_QSPI_INTR_GPIO_QSPI_SD1_EDGE_LOW_MSB    _u(22)
#define IO_QSPI_INTR_GPIO_QSPI_SD1_EDGE_LOW_LSB    _u(22)
#define IO_QSPI_INTR_GPIO_QSPI_SD1_EDGE_LOW_ACCESS "WC"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_INTR_GPIO_QSPI_SD1_LEVEL_HIGH
#define IO_QSPI_INTR_GPIO_QSPI_SD1_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_INTR_GPIO_QSPI_SD1_LEVEL_HIGH_BITS   _u(0x00200000)
#define IO_QSPI_INTR_GPIO_QSPI_SD1_LEVEL_HIGH_MSB    _u(21)
#define IO_QSPI_INTR_GPIO_QSPI_SD1_LEVEL_HIGH_LSB    _u(21)
#define IO_QSPI_INTR_GPIO_QSPI_SD1_LEVEL_HIGH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_INTR_GPIO_QSPI_SD1_LEVEL_LOW
#define IO_QSPI_INTR_GPIO_QSPI_SD1_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_INTR_GPIO_QSPI_SD1_LEVEL_LOW_BITS   _u(0x00100000)
#define IO_QSPI_INTR_GPIO_QSPI_SD1_LEVEL_LOW_MSB    _u(20)
#define IO_QSPI_INTR_GPIO_QSPI_SD1_LEVEL_LOW_LSB    _u(20)
#define IO_QSPI_INTR_GPIO_QSPI_SD1_LEVEL_LOW_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_INTR_GPIO_QSPI_SD0_EDGE_HIGH
#define IO_QSPI_INTR_GPIO_QSPI_SD0_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_INTR_GPIO_QSPI_SD0_EDGE_HIGH_BITS   _u(0x00080000)
#define IO_QSPI_INTR_GPIO_QSPI_SD0_EDGE_HIGH_MSB    _u(19)
#define IO_QSPI_INTR_GPIO_QSPI_SD0_EDGE_HIGH_LSB    _u(19)
#define IO_QSPI_INTR_GPIO_QSPI_SD0_EDGE_HIGH_ACCESS "WC"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_INTR_GPIO_QSPI_SD0_EDGE_LOW
#define IO_QSPI_INTR_GPIO_QSPI_SD0_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_INTR_GPIO_QSPI_SD0_EDGE_LOW_BITS   _u(0x00040000)
#define IO_QSPI_INTR_GPIO_QSPI_SD0_EDGE_LOW_MSB    _u(18)
#define IO_QSPI_INTR_GPIO_QSPI_SD0_EDGE_LOW_LSB    _u(18)
#define IO_QSPI_INTR_GPIO_QSPI_SD0_EDGE_LOW_ACCESS "WC"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_INTR_GPIO_QSPI_SD0_LEVEL_HIGH
#define IO_QSPI_INTR_GPIO_QSPI_SD0_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_INTR_GPIO_QSPI_SD0_LEVEL_HIGH_BITS   _u(0x00020000)
#define IO_QSPI_INTR_GPIO_QSPI_SD0_LEVEL_HIGH_MSB    _u(17)
#define IO_QSPI_INTR_GPIO_QSPI_SD0_LEVEL_HIGH_LSB    _u(17)
#define IO_QSPI_INTR_GPIO_QSPI_SD0_LEVEL_HIGH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_INTR_GPIO_QSPI_SD0_LEVEL_LOW
#define IO_QSPI_INTR_GPIO_QSPI_SD0_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_INTR_GPIO_QSPI_SD0_LEVEL_LOW_BITS   _u(0x00010000)
#define IO_QSPI_INTR_GPIO_QSPI_SD0_LEVEL_LOW_MSB    _u(16)
#define IO_QSPI_INTR_GPIO_QSPI_SD0_LEVEL_LOW_LSB    _u(16)
#define IO_QSPI_INTR_GPIO_QSPI_SD0_LEVEL_LOW_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_INTR_GPIO_QSPI_SS_EDGE_HIGH
#define IO_QSPI_INTR_GPIO_QSPI_SS_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_INTR_GPIO_QSPI_SS_EDGE_HIGH_BITS   _u(0x00008000)
#define IO_QSPI_INTR_GPIO_QSPI_SS_EDGE_HIGH_MSB    _u(15)
#define IO_QSPI_INTR_GPIO_QSPI_SS_EDGE_HIGH_LSB    _u(15)
#define IO_QSPI_INTR_GPIO_QSPI_SS_EDGE_HIGH_ACCESS "WC"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_INTR_GPIO_QSPI_SS_EDGE_LOW
#define IO_QSPI_INTR_GPIO_QSPI_SS_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_INTR_GPIO_QSPI_SS_EDGE_LOW_BITS   _u(0x00004000)
#define IO_QSPI_INTR_GPIO_QSPI_SS_EDGE_LOW_MSB    _u(14)
#define IO_QSPI_INTR_GPIO_QSPI_SS_EDGE_LOW_LSB    _u(14)
#define IO_QSPI_INTR_GPIO_QSPI_SS_EDGE_LOW_ACCESS "WC"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_INTR_GPIO_QSPI_SS_LEVEL_HIGH
#define IO_QSPI_INTR_GPIO_QSPI_SS_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_INTR_GPIO_QSPI_SS_LEVEL_HIGH_BITS   _u(0x00002000)
#define IO_QSPI_INTR_GPIO_QSPI_SS_LEVEL_HIGH_MSB    _u(13)
#define IO_QSPI_INTR_GPIO_QSPI_SS_LEVEL_HIGH_LSB    _u(13)
#define IO_QSPI_INTR_GPIO_QSPI_SS_LEVEL_HIGH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_INTR_GPIO_QSPI_SS_LEVEL_LOW
#define IO_QSPI_INTR_GPIO_QSPI_SS_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_INTR_GPIO_QSPI_SS_LEVEL_LOW_BITS   _u(0x00001000)
#define IO_QSPI_INTR_GPIO_QSPI_SS_LEVEL_LOW_MSB    _u(12)
#define IO_QSPI_INTR_GPIO_QSPI_SS_LEVEL_LOW_LSB    _u(12)
#define IO_QSPI_INTR_GPIO_QSPI_SS_LEVEL_LOW_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_INTR_GPIO_QSPI_SCLK_EDGE_HIGH
#define IO_QSPI_INTR_GPIO_QSPI_SCLK_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_INTR_GPIO_QSPI_SCLK_EDGE_HIGH_BITS   _u(0x00000800)
#define IO_QSPI_INTR_GPIO_QSPI_SCLK_EDGE_HIGH_MSB    _u(11)
#define IO_QSPI_INTR_GPIO_QSPI_SCLK_EDGE_HIGH_LSB    _u(11)
#define IO_QSPI_INTR_GPIO_QSPI_SCLK_EDGE_HIGH_ACCESS "WC"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_INTR_GPIO_QSPI_SCLK_EDGE_LOW
#define IO_QSPI_INTR_GPIO_QSPI_SCLK_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_INTR_GPIO_QSPI_SCLK_EDGE_LOW_BITS   _u(0x00000400)
#define IO_QSPI_INTR_GPIO_QSPI_SCLK_EDGE_LOW_MSB    _u(10)
#define IO_QSPI_INTR_GPIO_QSPI_SCLK_EDGE_LOW_LSB    _u(10)
#define IO_QSPI_INTR_GPIO_QSPI_SCLK_EDGE_LOW_ACCESS "WC"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_INTR_GPIO_QSPI_SCLK_LEVEL_HIGH
#define IO_QSPI_INTR_GPIO_QSPI_SCLK_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_INTR_GPIO_QSPI_SCLK_LEVEL_HIGH_BITS   _u(0x00000200)
#define IO_QSPI_INTR_GPIO_QSPI_SCLK_LEVEL_HIGH_MSB    _u(9)
#define IO_QSPI_INTR_GPIO_QSPI_SCLK_LEVEL_HIGH_LSB    _u(9)
#define IO_QSPI_INTR_GPIO_QSPI_SCLK_LEVEL_HIGH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_INTR_GPIO_QSPI_SCLK_LEVEL_LOW
#define IO_QSPI_INTR_GPIO_QSPI_SCLK_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_INTR_GPIO_QSPI_SCLK_LEVEL_LOW_BITS   _u(0x00000100)
#define IO_QSPI_INTR_GPIO_QSPI_SCLK_LEVEL_LOW_MSB    _u(8)
#define IO_QSPI_INTR_GPIO_QSPI_SCLK_LEVEL_LOW_LSB    _u(8)
#define IO_QSPI_INTR_GPIO_QSPI_SCLK_LEVEL_LOW_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_INTR_USBPHY_DM_EDGE_HIGH
#define IO_QSPI_INTR_USBPHY_DM_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_INTR_USBPHY_DM_EDGE_HIGH_BITS   _u(0x00000080)
#define IO_QSPI_INTR_USBPHY_DM_EDGE_HIGH_MSB    _u(7)
#define IO_QSPI_INTR_USBPHY_DM_EDGE_HIGH_LSB    _u(7)
#define IO_QSPI_INTR_USBPHY_DM_EDGE_HIGH_ACCESS "WC"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_INTR_USBPHY_DM_EDGE_LOW
#define IO_QSPI_INTR_USBPHY_DM_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_INTR_USBPHY_DM_EDGE_LOW_BITS   _u(0x00000040)
#define IO_QSPI_INTR_USBPHY_DM_EDGE_LOW_MSB    _u(6)
#define IO_QSPI_INTR_USBPHY_DM_EDGE_LOW_LSB    _u(6)
#define IO_QSPI_INTR_USBPHY_DM_EDGE_LOW_ACCESS "WC"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_INTR_USBPHY_DM_LEVEL_HIGH
#define IO_QSPI_INTR_USBPHY_DM_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_INTR_USBPHY_DM_LEVEL_HIGH_BITS   _u(0x00000020)
#define IO_QSPI_INTR_USBPHY_DM_LEVEL_HIGH_MSB    _u(5)
#define IO_QSPI_INTR_USBPHY_DM_LEVEL_HIGH_LSB    _u(5)
#define IO_QSPI_INTR_USBPHY_DM_LEVEL_HIGH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_INTR_USBPHY_DM_LEVEL_LOW
#define IO_QSPI_INTR_USBPHY_DM_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_INTR_USBPHY_DM_LEVEL_LOW_BITS   _u(0x00000010)
#define IO_QSPI_INTR_USBPHY_DM_LEVEL_LOW_MSB    _u(4)
#define IO_QSPI_INTR_USBPHY_DM_LEVEL_LOW_LSB    _u(4)
#define IO_QSPI_INTR_USBPHY_DM_LEVEL_LOW_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_INTR_USBPHY_DP_EDGE_HIGH
#define IO_QSPI_INTR_USBPHY_DP_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_INTR_USBPHY_DP_EDGE_HIGH_BITS   _u(0x00000008)
#define IO_QSPI_INTR_USBPHY_DP_EDGE_HIGH_MSB    _u(3)
#define IO_QSPI_INTR_USBPHY_DP_EDGE_HIGH_LSB    _u(3)
#define IO_QSPI_INTR_USBPHY_DP_EDGE_HIGH_ACCESS "WC"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_INTR_USBPHY_DP_EDGE_LOW
#define IO_QSPI_INTR_USBPHY_DP_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_INTR_USBPHY_DP_EDGE_LOW_BITS   _u(0x00000004)
#define IO_QSPI_INTR_USBPHY_DP_EDGE_LOW_MSB    _u(2)
#define IO_QSPI_INTR_USBPHY_DP_EDGE_LOW_LSB    _u(2)
#define IO_QSPI_INTR_USBPHY_DP_EDGE_LOW_ACCESS "WC"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_INTR_USBPHY_DP_LEVEL_HIGH
#define IO_QSPI_INTR_USBPHY_DP_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_INTR_USBPHY_DP_LEVEL_HIGH_BITS   _u(0x00000002)
#define IO_QSPI_INTR_USBPHY_DP_LEVEL_HIGH_MSB    _u(1)
#define IO_QSPI_INTR_USBPHY_DP_LEVEL_HIGH_LSB    _u(1)
#define IO_QSPI_INTR_USBPHY_DP_LEVEL_HIGH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_INTR_USBPHY_DP_LEVEL_LOW
#define IO_QSPI_INTR_USBPHY_DP_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_INTR_USBPHY_DP_LEVEL_LOW_BITS   _u(0x00000001)
#define IO_QSPI_INTR_USBPHY_DP_LEVEL_LOW_MSB    _u(0)
#define IO_QSPI_INTR_USBPHY_DP_LEVEL_LOW_LSB    _u(0)
#define IO_QSPI_INTR_USBPHY_DP_LEVEL_LOW_ACCESS "RO"
// =============================================================================
// Register    : IO_QSPI_PROC0_INTE
// Description : Interrupt Enable for proc0
#define IO_QSPI_PROC0_INTE_OFFSET _u(0x0000021c)
#define IO_QSPI_PROC0_INTE_BITS   _u(0xffffffff)
#define IO_QSPI_PROC0_INTE_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTE_GPIO_QSPI_SD3_EDGE_HIGH
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD3_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD3_EDGE_HIGH_BITS   _u(0x80000000)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD3_EDGE_HIGH_MSB    _u(31)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD3_EDGE_HIGH_LSB    _u(31)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD3_EDGE_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTE_GPIO_QSPI_SD3_EDGE_LOW
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD3_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD3_EDGE_LOW_BITS   _u(0x40000000)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD3_EDGE_LOW_MSB    _u(30)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD3_EDGE_LOW_LSB    _u(30)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD3_EDGE_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTE_GPIO_QSPI_SD3_LEVEL_HIGH
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD3_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD3_LEVEL_HIGH_BITS   _u(0x20000000)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD3_LEVEL_HIGH_MSB    _u(29)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD3_LEVEL_HIGH_LSB    _u(29)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD3_LEVEL_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTE_GPIO_QSPI_SD3_LEVEL_LOW
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD3_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD3_LEVEL_LOW_BITS   _u(0x10000000)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD3_LEVEL_LOW_MSB    _u(28)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD3_LEVEL_LOW_LSB    _u(28)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD3_LEVEL_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTE_GPIO_QSPI_SD2_EDGE_HIGH
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD2_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD2_EDGE_HIGH_BITS   _u(0x08000000)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD2_EDGE_HIGH_MSB    _u(27)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD2_EDGE_HIGH_LSB    _u(27)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD2_EDGE_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTE_GPIO_QSPI_SD2_EDGE_LOW
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD2_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD2_EDGE_LOW_BITS   _u(0x04000000)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD2_EDGE_LOW_MSB    _u(26)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD2_EDGE_LOW_LSB    _u(26)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD2_EDGE_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTE_GPIO_QSPI_SD2_LEVEL_HIGH
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD2_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD2_LEVEL_HIGH_BITS   _u(0x02000000)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD2_LEVEL_HIGH_MSB    _u(25)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD2_LEVEL_HIGH_LSB    _u(25)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD2_LEVEL_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTE_GPIO_QSPI_SD2_LEVEL_LOW
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD2_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD2_LEVEL_LOW_BITS   _u(0x01000000)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD2_LEVEL_LOW_MSB    _u(24)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD2_LEVEL_LOW_LSB    _u(24)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD2_LEVEL_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTE_GPIO_QSPI_SD1_EDGE_HIGH
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD1_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD1_EDGE_HIGH_BITS   _u(0x00800000)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD1_EDGE_HIGH_MSB    _u(23)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD1_EDGE_HIGH_LSB    _u(23)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD1_EDGE_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTE_GPIO_QSPI_SD1_EDGE_LOW
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD1_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD1_EDGE_LOW_BITS   _u(0x00400000)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD1_EDGE_LOW_MSB    _u(22)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD1_EDGE_LOW_LSB    _u(22)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD1_EDGE_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTE_GPIO_QSPI_SD1_LEVEL_HIGH
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD1_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD1_LEVEL_HIGH_BITS   _u(0x00200000)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD1_LEVEL_HIGH_MSB    _u(21)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD1_LEVEL_HIGH_LSB    _u(21)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD1_LEVEL_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTE_GPIO_QSPI_SD1_LEVEL_LOW
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD1_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD1_LEVEL_LOW_BITS   _u(0x00100000)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD1_LEVEL_LOW_MSB    _u(20)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD1_LEVEL_LOW_LSB    _u(20)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD1_LEVEL_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTE_GPIO_QSPI_SD0_EDGE_HIGH
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD0_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD0_EDGE_HIGH_BITS   _u(0x00080000)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD0_EDGE_HIGH_MSB    _u(19)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD0_EDGE_HIGH_LSB    _u(19)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD0_EDGE_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTE_GPIO_QSPI_SD0_EDGE_LOW
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD0_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD0_EDGE_LOW_BITS   _u(0x00040000)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD0_EDGE_LOW_MSB    _u(18)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD0_EDGE_LOW_LSB    _u(18)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD0_EDGE_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTE_GPIO_QSPI_SD0_LEVEL_HIGH
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD0_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD0_LEVEL_HIGH_BITS   _u(0x00020000)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD0_LEVEL_HIGH_MSB    _u(17)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD0_LEVEL_HIGH_LSB    _u(17)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD0_LEVEL_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTE_GPIO_QSPI_SD0_LEVEL_LOW
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD0_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD0_LEVEL_LOW_BITS   _u(0x00010000)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD0_LEVEL_LOW_MSB    _u(16)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD0_LEVEL_LOW_LSB    _u(16)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SD0_LEVEL_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTE_GPIO_QSPI_SS_EDGE_HIGH
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SS_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SS_EDGE_HIGH_BITS   _u(0x00008000)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SS_EDGE_HIGH_MSB    _u(15)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SS_EDGE_HIGH_LSB    _u(15)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SS_EDGE_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTE_GPIO_QSPI_SS_EDGE_LOW
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SS_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SS_EDGE_LOW_BITS   _u(0x00004000)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SS_EDGE_LOW_MSB    _u(14)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SS_EDGE_LOW_LSB    _u(14)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SS_EDGE_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTE_GPIO_QSPI_SS_LEVEL_HIGH
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SS_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SS_LEVEL_HIGH_BITS   _u(0x00002000)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SS_LEVEL_HIGH_MSB    _u(13)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SS_LEVEL_HIGH_LSB    _u(13)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SS_LEVEL_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTE_GPIO_QSPI_SS_LEVEL_LOW
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SS_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SS_LEVEL_LOW_BITS   _u(0x00001000)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SS_LEVEL_LOW_MSB    _u(12)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SS_LEVEL_LOW_LSB    _u(12)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SS_LEVEL_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTE_GPIO_QSPI_SCLK_EDGE_HIGH
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SCLK_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SCLK_EDGE_HIGH_BITS   _u(0x00000800)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SCLK_EDGE_HIGH_MSB    _u(11)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SCLK_EDGE_HIGH_LSB    _u(11)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SCLK_EDGE_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTE_GPIO_QSPI_SCLK_EDGE_LOW
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SCLK_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SCLK_EDGE_LOW_BITS   _u(0x00000400)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SCLK_EDGE_LOW_MSB    _u(10)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SCLK_EDGE_LOW_LSB    _u(10)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SCLK_EDGE_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTE_GPIO_QSPI_SCLK_LEVEL_HIGH
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SCLK_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SCLK_LEVEL_HIGH_BITS   _u(0x00000200)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SCLK_LEVEL_HIGH_MSB    _u(9)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SCLK_LEVEL_HIGH_LSB    _u(9)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SCLK_LEVEL_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTE_GPIO_QSPI_SCLK_LEVEL_LOW
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SCLK_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SCLK_LEVEL_LOW_BITS   _u(0x00000100)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SCLK_LEVEL_LOW_MSB    _u(8)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SCLK_LEVEL_LOW_LSB    _u(8)
#define IO_QSPI_PROC0_INTE_GPIO_QSPI_SCLK_LEVEL_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTE_USBPHY_DM_EDGE_HIGH
#define IO_QSPI_PROC0_INTE_USBPHY_DM_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTE_USBPHY_DM_EDGE_HIGH_BITS   _u(0x00000080)
#define IO_QSPI_PROC0_INTE_USBPHY_DM_EDGE_HIGH_MSB    _u(7)
#define IO_QSPI_PROC0_INTE_USBPHY_DM_EDGE_HIGH_LSB    _u(7)
#define IO_QSPI_PROC0_INTE_USBPHY_DM_EDGE_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTE_USBPHY_DM_EDGE_LOW
#define IO_QSPI_PROC0_INTE_USBPHY_DM_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTE_USBPHY_DM_EDGE_LOW_BITS   _u(0x00000040)
#define IO_QSPI_PROC0_INTE_USBPHY_DM_EDGE_LOW_MSB    _u(6)
#define IO_QSPI_PROC0_INTE_USBPHY_DM_EDGE_LOW_LSB    _u(6)
#define IO_QSPI_PROC0_INTE_USBPHY_DM_EDGE_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTE_USBPHY_DM_LEVEL_HIGH
#define IO_QSPI_PROC0_INTE_USBPHY_DM_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTE_USBPHY_DM_LEVEL_HIGH_BITS   _u(0x00000020)
#define IO_QSPI_PROC0_INTE_USBPHY_DM_LEVEL_HIGH_MSB    _u(5)
#define IO_QSPI_PROC0_INTE_USBPHY_DM_LEVEL_HIGH_LSB    _u(5)
#define IO_QSPI_PROC0_INTE_USBPHY_DM_LEVEL_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTE_USBPHY_DM_LEVEL_LOW
#define IO_QSPI_PROC0_INTE_USBPHY_DM_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTE_USBPHY_DM_LEVEL_LOW_BITS   _u(0x00000010)
#define IO_QSPI_PROC0_INTE_USBPHY_DM_LEVEL_LOW_MSB    _u(4)
#define IO_QSPI_PROC0_INTE_USBPHY_DM_LEVEL_LOW_LSB    _u(4)
#define IO_QSPI_PROC0_INTE_USBPHY_DM_LEVEL_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTE_USBPHY_DP_EDGE_HIGH
#define IO_QSPI_PROC0_INTE_USBPHY_DP_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTE_USBPHY_DP_EDGE_HIGH_BITS   _u(0x00000008)
#define IO_QSPI_PROC0_INTE_USBPHY_DP_EDGE_HIGH_MSB    _u(3)
#define IO_QSPI_PROC0_INTE_USBPHY_DP_EDGE_HIGH_LSB    _u(3)
#define IO_QSPI_PROC0_INTE_USBPHY_DP_EDGE_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTE_USBPHY_DP_EDGE_LOW
#define IO_QSPI_PROC0_INTE_USBPHY_DP_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTE_USBPHY_DP_EDGE_LOW_BITS   _u(0x00000004)
#define IO_QSPI_PROC0_INTE_USBPHY_DP_EDGE_LOW_MSB    _u(2)
#define IO_QSPI_PROC0_INTE_USBPHY_DP_EDGE_LOW_LSB    _u(2)
#define IO_QSPI_PROC0_INTE_USBPHY_DP_EDGE_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTE_USBPHY_DP_LEVEL_HIGH
#define IO_QSPI_PROC0_INTE_USBPHY_DP_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTE_USBPHY_DP_LEVEL_HIGH_BITS   _u(0x00000002)
#define IO_QSPI_PROC0_INTE_USBPHY_DP_LEVEL_HIGH_MSB    _u(1)
#define IO_QSPI_PROC0_INTE_USBPHY_DP_LEVEL_HIGH_LSB    _u(1)
#define IO_QSPI_PROC0_INTE_USBPHY_DP_LEVEL_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTE_USBPHY_DP_LEVEL_LOW
#define IO_QSPI_PROC0_INTE_USBPHY_DP_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTE_USBPHY_DP_LEVEL_LOW_BITS   _u(0x00000001)
#define IO_QSPI_PROC0_INTE_USBPHY_DP_LEVEL_LOW_MSB    _u(0)
#define IO_QSPI_PROC0_INTE_USBPHY_DP_LEVEL_LOW_LSB    _u(0)
#define IO_QSPI_PROC0_INTE_USBPHY_DP_LEVEL_LOW_ACCESS "RW"
// =============================================================================
// Register    : IO_QSPI_PROC0_INTF
// Description : Interrupt Force for proc0
#define IO_QSPI_PROC0_INTF_OFFSET _u(0x00000220)
#define IO_QSPI_PROC0_INTF_BITS   _u(0xffffffff)
#define IO_QSPI_PROC0_INTF_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTF_GPIO_QSPI_SD3_EDGE_HIGH
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD3_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD3_EDGE_HIGH_BITS   _u(0x80000000)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD3_EDGE_HIGH_MSB    _u(31)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD3_EDGE_HIGH_LSB    _u(31)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD3_EDGE_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTF_GPIO_QSPI_SD3_EDGE_LOW
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD3_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD3_EDGE_LOW_BITS   _u(0x40000000)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD3_EDGE_LOW_MSB    _u(30)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD3_EDGE_LOW_LSB    _u(30)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD3_EDGE_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTF_GPIO_QSPI_SD3_LEVEL_HIGH
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD3_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD3_LEVEL_HIGH_BITS   _u(0x20000000)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD3_LEVEL_HIGH_MSB    _u(29)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD3_LEVEL_HIGH_LSB    _u(29)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD3_LEVEL_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTF_GPIO_QSPI_SD3_LEVEL_LOW
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD3_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD3_LEVEL_LOW_BITS   _u(0x10000000)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD3_LEVEL_LOW_MSB    _u(28)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD3_LEVEL_LOW_LSB    _u(28)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD3_LEVEL_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTF_GPIO_QSPI_SD2_EDGE_HIGH
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD2_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD2_EDGE_HIGH_BITS   _u(0x08000000)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD2_EDGE_HIGH_MSB    _u(27)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD2_EDGE_HIGH_LSB    _u(27)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD2_EDGE_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTF_GPIO_QSPI_SD2_EDGE_LOW
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD2_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD2_EDGE_LOW_BITS   _u(0x04000000)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD2_EDGE_LOW_MSB    _u(26)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD2_EDGE_LOW_LSB    _u(26)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD2_EDGE_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTF_GPIO_QSPI_SD2_LEVEL_HIGH
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD2_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD2_LEVEL_HIGH_BITS   _u(0x02000000)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD2_LEVEL_HIGH_MSB    _u(25)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD2_LEVEL_HIGH_LSB    _u(25)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD2_LEVEL_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTF_GPIO_QSPI_SD2_LEVEL_LOW
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD2_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD2_LEVEL_LOW_BITS   _u(0x01000000)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD2_LEVEL_LOW_MSB    _u(24)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD2_LEVEL_LOW_LSB    _u(24)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD2_LEVEL_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTF_GPIO_QSPI_SD1_EDGE_HIGH
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD1_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD1_EDGE_HIGH_BITS   _u(0x00800000)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD1_EDGE_HIGH_MSB    _u(23)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD1_EDGE_HIGH_LSB    _u(23)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD1_EDGE_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTF_GPIO_QSPI_SD1_EDGE_LOW
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD1_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD1_EDGE_LOW_BITS   _u(0x00400000)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD1_EDGE_LOW_MSB    _u(22)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD1_EDGE_LOW_LSB    _u(22)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD1_EDGE_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTF_GPIO_QSPI_SD1_LEVEL_HIGH
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD1_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD1_LEVEL_HIGH_BITS   _u(0x00200000)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD1_LEVEL_HIGH_MSB    _u(21)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD1_LEVEL_HIGH_LSB    _u(21)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD1_LEVEL_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTF_GPIO_QSPI_SD1_LEVEL_LOW
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD1_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD1_LEVEL_LOW_BITS   _u(0x00100000)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD1_LEVEL_LOW_MSB    _u(20)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD1_LEVEL_LOW_LSB    _u(20)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD1_LEVEL_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTF_GPIO_QSPI_SD0_EDGE_HIGH
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD0_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD0_EDGE_HIGH_BITS   _u(0x00080000)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD0_EDGE_HIGH_MSB    _u(19)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD0_EDGE_HIGH_LSB    _u(19)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD0_EDGE_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTF_GPIO_QSPI_SD0_EDGE_LOW
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD0_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD0_EDGE_LOW_BITS   _u(0x00040000)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD0_EDGE_LOW_MSB    _u(18)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD0_EDGE_LOW_LSB    _u(18)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD0_EDGE_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTF_GPIO_QSPI_SD0_LEVEL_HIGH
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD0_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD0_LEVEL_HIGH_BITS   _u(0x00020000)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD0_LEVEL_HIGH_MSB    _u(17)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD0_LEVEL_HIGH_LSB    _u(17)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD0_LEVEL_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTF_GPIO_QSPI_SD0_LEVEL_LOW
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD0_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD0_LEVEL_LOW_BITS   _u(0x00010000)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD0_LEVEL_LOW_MSB    _u(16)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD0_LEVEL_LOW_LSB    _u(16)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SD0_LEVEL_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTF_GPIO_QSPI_SS_EDGE_HIGH
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SS_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SS_EDGE_HIGH_BITS   _u(0x00008000)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SS_EDGE_HIGH_MSB    _u(15)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SS_EDGE_HIGH_LSB    _u(15)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SS_EDGE_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTF_GPIO_QSPI_SS_EDGE_LOW
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SS_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SS_EDGE_LOW_BITS   _u(0x00004000)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SS_EDGE_LOW_MSB    _u(14)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SS_EDGE_LOW_LSB    _u(14)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SS_EDGE_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTF_GPIO_QSPI_SS_LEVEL_HIGH
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SS_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SS_LEVEL_HIGH_BITS   _u(0x00002000)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SS_LEVEL_HIGH_MSB    _u(13)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SS_LEVEL_HIGH_LSB    _u(13)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SS_LEVEL_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTF_GPIO_QSPI_SS_LEVEL_LOW
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SS_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SS_LEVEL_LOW_BITS   _u(0x00001000)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SS_LEVEL_LOW_MSB    _u(12)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SS_LEVEL_LOW_LSB    _u(12)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SS_LEVEL_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTF_GPIO_QSPI_SCLK_EDGE_HIGH
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SCLK_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SCLK_EDGE_HIGH_BITS   _u(0x00000800)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SCLK_EDGE_HIGH_MSB    _u(11)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SCLK_EDGE_HIGH_LSB    _u(11)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SCLK_EDGE_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTF_GPIO_QSPI_SCLK_EDGE_LOW
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SCLK_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SCLK_EDGE_LOW_BITS   _u(0x00000400)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SCLK_EDGE_LOW_MSB    _u(10)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SCLK_EDGE_LOW_LSB    _u(10)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SCLK_EDGE_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTF_GPIO_QSPI_SCLK_LEVEL_HIGH
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SCLK_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SCLK_LEVEL_HIGH_BITS   _u(0x00000200)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SCLK_LEVEL_HIGH_MSB    _u(9)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SCLK_LEVEL_HIGH_LSB    _u(9)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SCLK_LEVEL_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTF_GPIO_QSPI_SCLK_LEVEL_LOW
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SCLK_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SCLK_LEVEL_LOW_BITS   _u(0x00000100)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SCLK_LEVEL_LOW_MSB    _u(8)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SCLK_LEVEL_LOW_LSB    _u(8)
#define IO_QSPI_PROC0_INTF_GPIO_QSPI_SCLK_LEVEL_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTF_USBPHY_DM_EDGE_HIGH
#define IO_QSPI_PROC0_INTF_USBPHY_DM_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTF_USBPHY_DM_EDGE_HIGH_BITS   _u(0x00000080)
#define IO_QSPI_PROC0_INTF_USBPHY_DM_EDGE_HIGH_MSB    _u(7)
#define IO_QSPI_PROC0_INTF_USBPHY_DM_EDGE_HIGH_LSB    _u(7)
#define IO_QSPI_PROC0_INTF_USBPHY_DM_EDGE_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTF_USBPHY_DM_EDGE_LOW
#define IO_QSPI_PROC0_INTF_USBPHY_DM_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTF_USBPHY_DM_EDGE_LOW_BITS   _u(0x00000040)
#define IO_QSPI_PROC0_INTF_USBPHY_DM_EDGE_LOW_MSB    _u(6)
#define IO_QSPI_PROC0_INTF_USBPHY_DM_EDGE_LOW_LSB    _u(6)
#define IO_QSPI_PROC0_INTF_USBPHY_DM_EDGE_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTF_USBPHY_DM_LEVEL_HIGH
#define IO_QSPI_PROC0_INTF_USBPHY_DM_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTF_USBPHY_DM_LEVEL_HIGH_BITS   _u(0x00000020)
#define IO_QSPI_PROC0_INTF_USBPHY_DM_LEVEL_HIGH_MSB    _u(5)
#define IO_QSPI_PROC0_INTF_USBPHY_DM_LEVEL_HIGH_LSB    _u(5)
#define IO_QSPI_PROC0_INTF_USBPHY_DM_LEVEL_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTF_USBPHY_DM_LEVEL_LOW
#define IO_QSPI_PROC0_INTF_USBPHY_DM_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTF_USBPHY_DM_LEVEL_LOW_BITS   _u(0x00000010)
#define IO_QSPI_PROC0_INTF_USBPHY_DM_LEVEL_LOW_MSB    _u(4)
#define IO_QSPI_PROC0_INTF_USBPHY_DM_LEVEL_LOW_LSB    _u(4)
#define IO_QSPI_PROC0_INTF_USBPHY_DM_LEVEL_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTF_USBPHY_DP_EDGE_HIGH
#define IO_QSPI_PROC0_INTF_USBPHY_DP_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTF_USBPHY_DP_EDGE_HIGH_BITS   _u(0x00000008)
#define IO_QSPI_PROC0_INTF_USBPHY_DP_EDGE_HIGH_MSB    _u(3)
#define IO_QSPI_PROC0_INTF_USBPHY_DP_EDGE_HIGH_LSB    _u(3)
#define IO_QSPI_PROC0_INTF_USBPHY_DP_EDGE_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTF_USBPHY_DP_EDGE_LOW
#define IO_QSPI_PROC0_INTF_USBPHY_DP_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTF_USBPHY_DP_EDGE_LOW_BITS   _u(0x00000004)
#define IO_QSPI_PROC0_INTF_USBPHY_DP_EDGE_LOW_MSB    _u(2)
#define IO_QSPI_PROC0_INTF_USBPHY_DP_EDGE_LOW_LSB    _u(2)
#define IO_QSPI_PROC0_INTF_USBPHY_DP_EDGE_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTF_USBPHY_DP_LEVEL_HIGH
#define IO_QSPI_PROC0_INTF_USBPHY_DP_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTF_USBPHY_DP_LEVEL_HIGH_BITS   _u(0x00000002)
#define IO_QSPI_PROC0_INTF_USBPHY_DP_LEVEL_HIGH_MSB    _u(1)
#define IO_QSPI_PROC0_INTF_USBPHY_DP_LEVEL_HIGH_LSB    _u(1)
#define IO_QSPI_PROC0_INTF_USBPHY_DP_LEVEL_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTF_USBPHY_DP_LEVEL_LOW
#define IO_QSPI_PROC0_INTF_USBPHY_DP_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTF_USBPHY_DP_LEVEL_LOW_BITS   _u(0x00000001)
#define IO_QSPI_PROC0_INTF_USBPHY_DP_LEVEL_LOW_MSB    _u(0)
#define IO_QSPI_PROC0_INTF_USBPHY_DP_LEVEL_LOW_LSB    _u(0)
#define IO_QSPI_PROC0_INTF_USBPHY_DP_LEVEL_LOW_ACCESS "RW"
// =============================================================================
// Register    : IO_QSPI_PROC0_INTS
// Description : Interrupt status after masking & forcing for proc0
#define IO_QSPI_PROC0_INTS_OFFSET _u(0x00000224)
#define IO_QSPI_PROC0_INTS_BITS   _u(0xffffffff)
#define IO_QSPI_PROC0_INTS_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTS_GPIO_QSPI_SD3_EDGE_HIGH
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD3_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD3_EDGE_HIGH_BITS   _u(0x80000000)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD3_EDGE_HIGH_MSB    _u(31)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD3_EDGE_HIGH_LSB    _u(31)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD3_EDGE_HIGH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTS_GPIO_QSPI_SD3_EDGE_LOW
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD3_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD3_EDGE_LOW_BITS   _u(0x40000000)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD3_EDGE_LOW_MSB    _u(30)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD3_EDGE_LOW_LSB    _u(30)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD3_EDGE_LOW_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTS_GPIO_QSPI_SD3_LEVEL_HIGH
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD3_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD3_LEVEL_HIGH_BITS   _u(0x20000000)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD3_LEVEL_HIGH_MSB    _u(29)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD3_LEVEL_HIGH_LSB    _u(29)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD3_LEVEL_HIGH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTS_GPIO_QSPI_SD3_LEVEL_LOW
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD3_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD3_LEVEL_LOW_BITS   _u(0x10000000)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD3_LEVEL_LOW_MSB    _u(28)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD3_LEVEL_LOW_LSB    _u(28)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD3_LEVEL_LOW_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTS_GPIO_QSPI_SD2_EDGE_HIGH
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD2_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD2_EDGE_HIGH_BITS   _u(0x08000000)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD2_EDGE_HIGH_MSB    _u(27)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD2_EDGE_HIGH_LSB    _u(27)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD2_EDGE_HIGH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTS_GPIO_QSPI_SD2_EDGE_LOW
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD2_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD2_EDGE_LOW_BITS   _u(0x04000000)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD2_EDGE_LOW_MSB    _u(26)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD2_EDGE_LOW_LSB    _u(26)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD2_EDGE_LOW_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTS_GPIO_QSPI_SD2_LEVEL_HIGH
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD2_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD2_LEVEL_HIGH_BITS   _u(0x02000000)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD2_LEVEL_HIGH_MSB    _u(25)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD2_LEVEL_HIGH_LSB    _u(25)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD2_LEVEL_HIGH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTS_GPIO_QSPI_SD2_LEVEL_LOW
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD2_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD2_LEVEL_LOW_BITS   _u(0x01000000)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD2_LEVEL_LOW_MSB    _u(24)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD2_LEVEL_LOW_LSB    _u(24)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD2_LEVEL_LOW_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTS_GPIO_QSPI_SD1_EDGE_HIGH
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD1_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD1_EDGE_HIGH_BITS   _u(0x00800000)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD1_EDGE_HIGH_MSB    _u(23)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD1_EDGE_HIGH_LSB    _u(23)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD1_EDGE_HIGH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTS_GPIO_QSPI_SD1_EDGE_LOW
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD1_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD1_EDGE_LOW_BITS   _u(0x00400000)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD1_EDGE_LOW_MSB    _u(22)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD1_EDGE_LOW_LSB    _u(22)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD1_EDGE_LOW_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTS_GPIO_QSPI_SD1_LEVEL_HIGH
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD1_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD1_LEVEL_HIGH_BITS   _u(0x00200000)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD1_LEVEL_HIGH_MSB    _u(21)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD1_LEVEL_HIGH_LSB    _u(21)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD1_LEVEL_HIGH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTS_GPIO_QSPI_SD1_LEVEL_LOW
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD1_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD1_LEVEL_LOW_BITS   _u(0x00100000)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD1_LEVEL_LOW_MSB    _u(20)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD1_LEVEL_LOW_LSB    _u(20)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD1_LEVEL_LOW_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTS_GPIO_QSPI_SD0_EDGE_HIGH
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD0_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD0_EDGE_HIGH_BITS   _u(0x00080000)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD0_EDGE_HIGH_MSB    _u(19)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD0_EDGE_HIGH_LSB    _u(19)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD0_EDGE_HIGH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTS_GPIO_QSPI_SD0_EDGE_LOW
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD0_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD0_EDGE_LOW_BITS   _u(0x00040000)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD0_EDGE_LOW_MSB    _u(18)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD0_EDGE_LOW_LSB    _u(18)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD0_EDGE_LOW_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTS_GPIO_QSPI_SD0_LEVEL_HIGH
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD0_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD0_LEVEL_HIGH_BITS   _u(0x00020000)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD0_LEVEL_HIGH_MSB    _u(17)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD0_LEVEL_HIGH_LSB    _u(17)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD0_LEVEL_HIGH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTS_GPIO_QSPI_SD0_LEVEL_LOW
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD0_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD0_LEVEL_LOW_BITS   _u(0x00010000)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD0_LEVEL_LOW_MSB    _u(16)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD0_LEVEL_LOW_LSB    _u(16)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SD0_LEVEL_LOW_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTS_GPIO_QSPI_SS_EDGE_HIGH
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SS_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SS_EDGE_HIGH_BITS   _u(0x00008000)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SS_EDGE_HIGH_MSB    _u(15)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SS_EDGE_HIGH_LSB    _u(15)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SS_EDGE_HIGH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTS_GPIO_QSPI_SS_EDGE_LOW
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SS_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SS_EDGE_LOW_BITS   _u(0x00004000)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SS_EDGE_LOW_MSB    _u(14)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SS_EDGE_LOW_LSB    _u(14)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SS_EDGE_LOW_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTS_GPIO_QSPI_SS_LEVEL_HIGH
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SS_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SS_LEVEL_HIGH_BITS   _u(0x00002000)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SS_LEVEL_HIGH_MSB    _u(13)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SS_LEVEL_HIGH_LSB    _u(13)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SS_LEVEL_HIGH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTS_GPIO_QSPI_SS_LEVEL_LOW
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SS_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SS_LEVEL_LOW_BITS   _u(0x00001000)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SS_LEVEL_LOW_MSB    _u(12)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SS_LEVEL_LOW_LSB    _u(12)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SS_LEVEL_LOW_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTS_GPIO_QSPI_SCLK_EDGE_HIGH
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SCLK_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SCLK_EDGE_HIGH_BITS   _u(0x00000800)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SCLK_EDGE_HIGH_MSB    _u(11)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SCLK_EDGE_HIGH_LSB    _u(11)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SCLK_EDGE_HIGH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTS_GPIO_QSPI_SCLK_EDGE_LOW
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SCLK_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SCLK_EDGE_LOW_BITS   _u(0x00000400)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SCLK_EDGE_LOW_MSB    _u(10)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SCLK_EDGE_LOW_LSB    _u(10)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SCLK_EDGE_LOW_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTS_GPIO_QSPI_SCLK_LEVEL_HIGH
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SCLK_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SCLK_LEVEL_HIGH_BITS   _u(0x00000200)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SCLK_LEVEL_HIGH_MSB    _u(9)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SCLK_LEVEL_HIGH_LSB    _u(9)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SCLK_LEVEL_HIGH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTS_GPIO_QSPI_SCLK_LEVEL_LOW
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SCLK_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SCLK_LEVEL_LOW_BITS   _u(0x00000100)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SCLK_LEVEL_LOW_MSB    _u(8)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SCLK_LEVEL_LOW_LSB    _u(8)
#define IO_QSPI_PROC0_INTS_GPIO_QSPI_SCLK_LEVEL_LOW_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTS_USBPHY_DM_EDGE_HIGH
#define IO_QSPI_PROC0_INTS_USBPHY_DM_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTS_USBPHY_DM_EDGE_HIGH_BITS   _u(0x00000080)
#define IO_QSPI_PROC0_INTS_USBPHY_DM_EDGE_HIGH_MSB    _u(7)
#define IO_QSPI_PROC0_INTS_USBPHY_DM_EDGE_HIGH_LSB    _u(7)
#define IO_QSPI_PROC0_INTS_USBPHY_DM_EDGE_HIGH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTS_USBPHY_DM_EDGE_LOW
#define IO_QSPI_PROC0_INTS_USBPHY_DM_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTS_USBPHY_DM_EDGE_LOW_BITS   _u(0x00000040)
#define IO_QSPI_PROC0_INTS_USBPHY_DM_EDGE_LOW_MSB    _u(6)
#define IO_QSPI_PROC0_INTS_USBPHY_DM_EDGE_LOW_LSB    _u(6)
#define IO_QSPI_PROC0_INTS_USBPHY_DM_EDGE_LOW_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTS_USBPHY_DM_LEVEL_HIGH
#define IO_QSPI_PROC0_INTS_USBPHY_DM_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTS_USBPHY_DM_LEVEL_HIGH_BITS   _u(0x00000020)
#define IO_QSPI_PROC0_INTS_USBPHY_DM_LEVEL_HIGH_MSB    _u(5)
#define IO_QSPI_PROC0_INTS_USBPHY_DM_LEVEL_HIGH_LSB    _u(5)
#define IO_QSPI_PROC0_INTS_USBPHY_DM_LEVEL_HIGH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTS_USBPHY_DM_LEVEL_LOW
#define IO_QSPI_PROC0_INTS_USBPHY_DM_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTS_USBPHY_DM_LEVEL_LOW_BITS   _u(0x00000010)
#define IO_QSPI_PROC0_INTS_USBPHY_DM_LEVEL_LOW_MSB    _u(4)
#define IO_QSPI_PROC0_INTS_USBPHY_DM_LEVEL_LOW_LSB    _u(4)
#define IO_QSPI_PROC0_INTS_USBPHY_DM_LEVEL_LOW_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTS_USBPHY_DP_EDGE_HIGH
#define IO_QSPI_PROC0_INTS_USBPHY_DP_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTS_USBPHY_DP_EDGE_HIGH_BITS   _u(0x00000008)
#define IO_QSPI_PROC0_INTS_USBPHY_DP_EDGE_HIGH_MSB    _u(3)
#define IO_QSPI_PROC0_INTS_USBPHY_DP_EDGE_HIGH_LSB    _u(3)
#define IO_QSPI_PROC0_INTS_USBPHY_DP_EDGE_HIGH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTS_USBPHY_DP_EDGE_LOW
#define IO_QSPI_PROC0_INTS_USBPHY_DP_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTS_USBPHY_DP_EDGE_LOW_BITS   _u(0x00000004)
#define IO_QSPI_PROC0_INTS_USBPHY_DP_EDGE_LOW_MSB    _u(2)
#define IO_QSPI_PROC0_INTS_USBPHY_DP_EDGE_LOW_LSB    _u(2)
#define IO_QSPI_PROC0_INTS_USBPHY_DP_EDGE_LOW_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTS_USBPHY_DP_LEVEL_HIGH
#define IO_QSPI_PROC0_INTS_USBPHY_DP_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTS_USBPHY_DP_LEVEL_HIGH_BITS   _u(0x00000002)
#define IO_QSPI_PROC0_INTS_USBPHY_DP_LEVEL_HIGH_MSB    _u(1)
#define IO_QSPI_PROC0_INTS_USBPHY_DP_LEVEL_HIGH_LSB    _u(1)
#define IO_QSPI_PROC0_INTS_USBPHY_DP_LEVEL_HIGH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC0_INTS_USBPHY_DP_LEVEL_LOW
#define IO_QSPI_PROC0_INTS_USBPHY_DP_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC0_INTS_USBPHY_DP_LEVEL_LOW_BITS   _u(0x00000001)
#define IO_QSPI_PROC0_INTS_USBPHY_DP_LEVEL_LOW_MSB    _u(0)
#define IO_QSPI_PROC0_INTS_USBPHY_DP_LEVEL_LOW_LSB    _u(0)
#define IO_QSPI_PROC0_INTS_USBPHY_DP_LEVEL_LOW_ACCESS "RO"
// =============================================================================
// Register    : IO_QSPI_PROC1_INTE
// Description : Interrupt Enable for proc1
#define IO_QSPI_PROC1_INTE_OFFSET _u(0x00000228)
#define IO_QSPI_PROC1_INTE_BITS   _u(0xffffffff)
#define IO_QSPI_PROC1_INTE_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTE_GPIO_QSPI_SD3_EDGE_HIGH
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD3_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD3_EDGE_HIGH_BITS   _u(0x80000000)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD3_EDGE_HIGH_MSB    _u(31)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD3_EDGE_HIGH_LSB    _u(31)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD3_EDGE_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTE_GPIO_QSPI_SD3_EDGE_LOW
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD3_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD3_EDGE_LOW_BITS   _u(0x40000000)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD3_EDGE_LOW_MSB    _u(30)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD3_EDGE_LOW_LSB    _u(30)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD3_EDGE_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTE_GPIO_QSPI_SD3_LEVEL_HIGH
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD3_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD3_LEVEL_HIGH_BITS   _u(0x20000000)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD3_LEVEL_HIGH_MSB    _u(29)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD3_LEVEL_HIGH_LSB    _u(29)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD3_LEVEL_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTE_GPIO_QSPI_SD3_LEVEL_LOW
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD3_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD3_LEVEL_LOW_BITS   _u(0x10000000)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD3_LEVEL_LOW_MSB    _u(28)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD3_LEVEL_LOW_LSB    _u(28)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD3_LEVEL_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTE_GPIO_QSPI_SD2_EDGE_HIGH
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD2_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD2_EDGE_HIGH_BITS   _u(0x08000000)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD2_EDGE_HIGH_MSB    _u(27)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD2_EDGE_HIGH_LSB    _u(27)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD2_EDGE_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTE_GPIO_QSPI_SD2_EDGE_LOW
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD2_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD2_EDGE_LOW_BITS   _u(0x04000000)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD2_EDGE_LOW_MSB    _u(26)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD2_EDGE_LOW_LSB    _u(26)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD2_EDGE_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTE_GPIO_QSPI_SD2_LEVEL_HIGH
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD2_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD2_LEVEL_HIGH_BITS   _u(0x02000000)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD2_LEVEL_HIGH_MSB    _u(25)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD2_LEVEL_HIGH_LSB    _u(25)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD2_LEVEL_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTE_GPIO_QSPI_SD2_LEVEL_LOW
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD2_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD2_LEVEL_LOW_BITS   _u(0x01000000)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD2_LEVEL_LOW_MSB    _u(24)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD2_LEVEL_LOW_LSB    _u(24)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD2_LEVEL_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTE_GPIO_QSPI_SD1_EDGE_HIGH
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD1_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD1_EDGE_HIGH_BITS   _u(0x00800000)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD1_EDGE_HIGH_MSB    _u(23)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD1_EDGE_HIGH_LSB    _u(23)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD1_EDGE_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTE_GPIO_QSPI_SD1_EDGE_LOW
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD1_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD1_EDGE_LOW_BITS   _u(0x00400000)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD1_EDGE_LOW_MSB    _u(22)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD1_EDGE_LOW_LSB    _u(22)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD1_EDGE_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTE_GPIO_QSPI_SD1_LEVEL_HIGH
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD1_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD1_LEVEL_HIGH_BITS   _u(0x00200000)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD1_LEVEL_HIGH_MSB    _u(21)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD1_LEVEL_HIGH_LSB    _u(21)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD1_LEVEL_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTE_GPIO_QSPI_SD1_LEVEL_LOW
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD1_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD1_LEVEL_LOW_BITS   _u(0x00100000)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD1_LEVEL_LOW_MSB    _u(20)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD1_LEVEL_LOW_LSB    _u(20)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD1_LEVEL_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTE_GPIO_QSPI_SD0_EDGE_HIGH
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD0_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD0_EDGE_HIGH_BITS   _u(0x00080000)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD0_EDGE_HIGH_MSB    _u(19)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD0_EDGE_HIGH_LSB    _u(19)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD0_EDGE_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTE_GPIO_QSPI_SD0_EDGE_LOW
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD0_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD0_EDGE_LOW_BITS   _u(0x00040000)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD0_EDGE_LOW_MSB    _u(18)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD0_EDGE_LOW_LSB    _u(18)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD0_EDGE_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTE_GPIO_QSPI_SD0_LEVEL_HIGH
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD0_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD0_LEVEL_HIGH_BITS   _u(0x00020000)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD0_LEVEL_HIGH_MSB    _u(17)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD0_LEVEL_HIGH_LSB    _u(17)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD0_LEVEL_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTE_GPIO_QSPI_SD0_LEVEL_LOW
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD0_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD0_LEVEL_LOW_BITS   _u(0x00010000)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD0_LEVEL_LOW_MSB    _u(16)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD0_LEVEL_LOW_LSB    _u(16)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SD0_LEVEL_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTE_GPIO_QSPI_SS_EDGE_HIGH
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SS_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SS_EDGE_HIGH_BITS   _u(0x00008000)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SS_EDGE_HIGH_MSB    _u(15)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SS_EDGE_HIGH_LSB    _u(15)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SS_EDGE_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTE_GPIO_QSPI_SS_EDGE_LOW
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SS_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SS_EDGE_LOW_BITS   _u(0x00004000)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SS_EDGE_LOW_MSB    _u(14)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SS_EDGE_LOW_LSB    _u(14)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SS_EDGE_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTE_GPIO_QSPI_SS_LEVEL_HIGH
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SS_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SS_LEVEL_HIGH_BITS   _u(0x00002000)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SS_LEVEL_HIGH_MSB    _u(13)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SS_LEVEL_HIGH_LSB    _u(13)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SS_LEVEL_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTE_GPIO_QSPI_SS_LEVEL_LOW
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SS_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SS_LEVEL_LOW_BITS   _u(0x00001000)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SS_LEVEL_LOW_MSB    _u(12)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SS_LEVEL_LOW_LSB    _u(12)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SS_LEVEL_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTE_GPIO_QSPI_SCLK_EDGE_HIGH
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SCLK_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SCLK_EDGE_HIGH_BITS   _u(0x00000800)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SCLK_EDGE_HIGH_MSB    _u(11)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SCLK_EDGE_HIGH_LSB    _u(11)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SCLK_EDGE_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTE_GPIO_QSPI_SCLK_EDGE_LOW
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SCLK_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SCLK_EDGE_LOW_BITS   _u(0x00000400)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SCLK_EDGE_LOW_MSB    _u(10)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SCLK_EDGE_LOW_LSB    _u(10)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SCLK_EDGE_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTE_GPIO_QSPI_SCLK_LEVEL_HIGH
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SCLK_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SCLK_LEVEL_HIGH_BITS   _u(0x00000200)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SCLK_LEVEL_HIGH_MSB    _u(9)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SCLK_LEVEL_HIGH_LSB    _u(9)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SCLK_LEVEL_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTE_GPIO_QSPI_SCLK_LEVEL_LOW
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SCLK_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SCLK_LEVEL_LOW_BITS   _u(0x00000100)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SCLK_LEVEL_LOW_MSB    _u(8)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SCLK_LEVEL_LOW_LSB    _u(8)
#define IO_QSPI_PROC1_INTE_GPIO_QSPI_SCLK_LEVEL_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTE_USBPHY_DM_EDGE_HIGH
#define IO_QSPI_PROC1_INTE_USBPHY_DM_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTE_USBPHY_DM_EDGE_HIGH_BITS   _u(0x00000080)
#define IO_QSPI_PROC1_INTE_USBPHY_DM_EDGE_HIGH_MSB    _u(7)
#define IO_QSPI_PROC1_INTE_USBPHY_DM_EDGE_HIGH_LSB    _u(7)
#define IO_QSPI_PROC1_INTE_USBPHY_DM_EDGE_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTE_USBPHY_DM_EDGE_LOW
#define IO_QSPI_PROC1_INTE_USBPHY_DM_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTE_USBPHY_DM_EDGE_LOW_BITS   _u(0x00000040)
#define IO_QSPI_PROC1_INTE_USBPHY_DM_EDGE_LOW_MSB    _u(6)
#define IO_QSPI_PROC1_INTE_USBPHY_DM_EDGE_LOW_LSB    _u(6)
#define IO_QSPI_PROC1_INTE_USBPHY_DM_EDGE_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTE_USBPHY_DM_LEVEL_HIGH
#define IO_QSPI_PROC1_INTE_USBPHY_DM_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTE_USBPHY_DM_LEVEL_HIGH_BITS   _u(0x00000020)
#define IO_QSPI_PROC1_INTE_USBPHY_DM_LEVEL_HIGH_MSB    _u(5)
#define IO_QSPI_PROC1_INTE_USBPHY_DM_LEVEL_HIGH_LSB    _u(5)
#define IO_QSPI_PROC1_INTE_USBPHY_DM_LEVEL_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTE_USBPHY_DM_LEVEL_LOW
#define IO_QSPI_PROC1_INTE_USBPHY_DM_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTE_USBPHY_DM_LEVEL_LOW_BITS   _u(0x00000010)
#define IO_QSPI_PROC1_INTE_USBPHY_DM_LEVEL_LOW_MSB    _u(4)
#define IO_QSPI_PROC1_INTE_USBPHY_DM_LEVEL_LOW_LSB    _u(4)
#define IO_QSPI_PROC1_INTE_USBPHY_DM_LEVEL_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTE_USBPHY_DP_EDGE_HIGH
#define IO_QSPI_PROC1_INTE_USBPHY_DP_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTE_USBPHY_DP_EDGE_HIGH_BITS   _u(0x00000008)
#define IO_QSPI_PROC1_INTE_USBPHY_DP_EDGE_HIGH_MSB    _u(3)
#define IO_QSPI_PROC1_INTE_USBPHY_DP_EDGE_HIGH_LSB    _u(3)
#define IO_QSPI_PROC1_INTE_USBPHY_DP_EDGE_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTE_USBPHY_DP_EDGE_LOW
#define IO_QSPI_PROC1_INTE_USBPHY_DP_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTE_USBPHY_DP_EDGE_LOW_BITS   _u(0x00000004)
#define IO_QSPI_PROC1_INTE_USBPHY_DP_EDGE_LOW_MSB    _u(2)
#define IO_QSPI_PROC1_INTE_USBPHY_DP_EDGE_LOW_LSB    _u(2)
#define IO_QSPI_PROC1_INTE_USBPHY_DP_EDGE_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTE_USBPHY_DP_LEVEL_HIGH
#define IO_QSPI_PROC1_INTE_USBPHY_DP_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTE_USBPHY_DP_LEVEL_HIGH_BITS   _u(0x00000002)
#define IO_QSPI_PROC1_INTE_USBPHY_DP_LEVEL_HIGH_MSB    _u(1)
#define IO_QSPI_PROC1_INTE_USBPHY_DP_LEVEL_HIGH_LSB    _u(1)
#define IO_QSPI_PROC1_INTE_USBPHY_DP_LEVEL_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTE_USBPHY_DP_LEVEL_LOW
#define IO_QSPI_PROC1_INTE_USBPHY_DP_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTE_USBPHY_DP_LEVEL_LOW_BITS   _u(0x00000001)
#define IO_QSPI_PROC1_INTE_USBPHY_DP_LEVEL_LOW_MSB    _u(0)
#define IO_QSPI_PROC1_INTE_USBPHY_DP_LEVEL_LOW_LSB    _u(0)
#define IO_QSPI_PROC1_INTE_USBPHY_DP_LEVEL_LOW_ACCESS "RW"
// =============================================================================
// Register    : IO_QSPI_PROC1_INTF
// Description : Interrupt Force for proc1
#define IO_QSPI_PROC1_INTF_OFFSET _u(0x0000022c)
#define IO_QSPI_PROC1_INTF_BITS   _u(0xffffffff)
#define IO_QSPI_PROC1_INTF_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTF_GPIO_QSPI_SD3_EDGE_HIGH
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD3_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD3_EDGE_HIGH_BITS   _u(0x80000000)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD3_EDGE_HIGH_MSB    _u(31)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD3_EDGE_HIGH_LSB    _u(31)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD3_EDGE_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTF_GPIO_QSPI_SD3_EDGE_LOW
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD3_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD3_EDGE_LOW_BITS   _u(0x40000000)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD3_EDGE_LOW_MSB    _u(30)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD3_EDGE_LOW_LSB    _u(30)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD3_EDGE_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTF_GPIO_QSPI_SD3_LEVEL_HIGH
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD3_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD3_LEVEL_HIGH_BITS   _u(0x20000000)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD3_LEVEL_HIGH_MSB    _u(29)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD3_LEVEL_HIGH_LSB    _u(29)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD3_LEVEL_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTF_GPIO_QSPI_SD3_LEVEL_LOW
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD3_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD3_LEVEL_LOW_BITS   _u(0x10000000)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD3_LEVEL_LOW_MSB    _u(28)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD3_LEVEL_LOW_LSB    _u(28)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD3_LEVEL_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTF_GPIO_QSPI_SD2_EDGE_HIGH
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD2_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD2_EDGE_HIGH_BITS   _u(0x08000000)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD2_EDGE_HIGH_MSB    _u(27)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD2_EDGE_HIGH_LSB    _u(27)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD2_EDGE_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTF_GPIO_QSPI_SD2_EDGE_LOW
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD2_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD2_EDGE_LOW_BITS   _u(0x04000000)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD2_EDGE_LOW_MSB    _u(26)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD2_EDGE_LOW_LSB    _u(26)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD2_EDGE_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTF_GPIO_QSPI_SD2_LEVEL_HIGH
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD2_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD2_LEVEL_HIGH_BITS   _u(0x02000000)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD2_LEVEL_HIGH_MSB    _u(25)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD2_LEVEL_HIGH_LSB    _u(25)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD2_LEVEL_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTF_GPIO_QSPI_SD2_LEVEL_LOW
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD2_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD2_LEVEL_LOW_BITS   _u(0x01000000)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD2_LEVEL_LOW_MSB    _u(24)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD2_LEVEL_LOW_LSB    _u(24)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD2_LEVEL_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTF_GPIO_QSPI_SD1_EDGE_HIGH
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD1_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD1_EDGE_HIGH_BITS   _u(0x00800000)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD1_EDGE_HIGH_MSB    _u(23)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD1_EDGE_HIGH_LSB    _u(23)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD1_EDGE_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTF_GPIO_QSPI_SD1_EDGE_LOW
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD1_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD1_EDGE_LOW_BITS   _u(0x00400000)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD1_EDGE_LOW_MSB    _u(22)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD1_EDGE_LOW_LSB    _u(22)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD1_EDGE_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTF_GPIO_QSPI_SD1_LEVEL_HIGH
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD1_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD1_LEVEL_HIGH_BITS   _u(0x00200000)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD1_LEVEL_HIGH_MSB    _u(21)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD1_LEVEL_HIGH_LSB    _u(21)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD1_LEVEL_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTF_GPIO_QSPI_SD1_LEVEL_LOW
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD1_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD1_LEVEL_LOW_BITS   _u(0x00100000)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD1_LEVEL_LOW_MSB    _u(20)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD1_LEVEL_LOW_LSB    _u(20)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD1_LEVEL_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTF_GPIO_QSPI_SD0_EDGE_HIGH
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD0_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD0_EDGE_HIGH_BITS   _u(0x00080000)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD0_EDGE_HIGH_MSB    _u(19)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD0_EDGE_HIGH_LSB    _u(19)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD0_EDGE_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTF_GPIO_QSPI_SD0_EDGE_LOW
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD0_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD0_EDGE_LOW_BITS   _u(0x00040000)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD0_EDGE_LOW_MSB    _u(18)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD0_EDGE_LOW_LSB    _u(18)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD0_EDGE_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTF_GPIO_QSPI_SD0_LEVEL_HIGH
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD0_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD0_LEVEL_HIGH_BITS   _u(0x00020000)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD0_LEVEL_HIGH_MSB    _u(17)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD0_LEVEL_HIGH_LSB    _u(17)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD0_LEVEL_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTF_GPIO_QSPI_SD0_LEVEL_LOW
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD0_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD0_LEVEL_LOW_BITS   _u(0x00010000)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD0_LEVEL_LOW_MSB    _u(16)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD0_LEVEL_LOW_LSB    _u(16)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SD0_LEVEL_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTF_GPIO_QSPI_SS_EDGE_HIGH
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SS_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SS_EDGE_HIGH_BITS   _u(0x00008000)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SS_EDGE_HIGH_MSB    _u(15)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SS_EDGE_HIGH_LSB    _u(15)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SS_EDGE_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTF_GPIO_QSPI_SS_EDGE_LOW
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SS_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SS_EDGE_LOW_BITS   _u(0x00004000)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SS_EDGE_LOW_MSB    _u(14)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SS_EDGE_LOW_LSB    _u(14)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SS_EDGE_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTF_GPIO_QSPI_SS_LEVEL_HIGH
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SS_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SS_LEVEL_HIGH_BITS   _u(0x00002000)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SS_LEVEL_HIGH_MSB    _u(13)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SS_LEVEL_HIGH_LSB    _u(13)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SS_LEVEL_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTF_GPIO_QSPI_SS_LEVEL_LOW
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SS_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SS_LEVEL_LOW_BITS   _u(0x00001000)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SS_LEVEL_LOW_MSB    _u(12)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SS_LEVEL_LOW_LSB    _u(12)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SS_LEVEL_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTF_GPIO_QSPI_SCLK_EDGE_HIGH
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SCLK_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SCLK_EDGE_HIGH_BITS   _u(0x00000800)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SCLK_EDGE_HIGH_MSB    _u(11)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SCLK_EDGE_HIGH_LSB    _u(11)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SCLK_EDGE_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTF_GPIO_QSPI_SCLK_EDGE_LOW
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SCLK_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SCLK_EDGE_LOW_BITS   _u(0x00000400)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SCLK_EDGE_LOW_MSB    _u(10)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SCLK_EDGE_LOW_LSB    _u(10)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SCLK_EDGE_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTF_GPIO_QSPI_SCLK_LEVEL_HIGH
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SCLK_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SCLK_LEVEL_HIGH_BITS   _u(0x00000200)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SCLK_LEVEL_HIGH_MSB    _u(9)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SCLK_LEVEL_HIGH_LSB    _u(9)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SCLK_LEVEL_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTF_GPIO_QSPI_SCLK_LEVEL_LOW
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SCLK_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SCLK_LEVEL_LOW_BITS   _u(0x00000100)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SCLK_LEVEL_LOW_MSB    _u(8)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SCLK_LEVEL_LOW_LSB    _u(8)
#define IO_QSPI_PROC1_INTF_GPIO_QSPI_SCLK_LEVEL_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTF_USBPHY_DM_EDGE_HIGH
#define IO_QSPI_PROC1_INTF_USBPHY_DM_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTF_USBPHY_DM_EDGE_HIGH_BITS   _u(0x00000080)
#define IO_QSPI_PROC1_INTF_USBPHY_DM_EDGE_HIGH_MSB    _u(7)
#define IO_QSPI_PROC1_INTF_USBPHY_DM_EDGE_HIGH_LSB    _u(7)
#define IO_QSPI_PROC1_INTF_USBPHY_DM_EDGE_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTF_USBPHY_DM_EDGE_LOW
#define IO_QSPI_PROC1_INTF_USBPHY_DM_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTF_USBPHY_DM_EDGE_LOW_BITS   _u(0x00000040)
#define IO_QSPI_PROC1_INTF_USBPHY_DM_EDGE_LOW_MSB    _u(6)
#define IO_QSPI_PROC1_INTF_USBPHY_DM_EDGE_LOW_LSB    _u(6)
#define IO_QSPI_PROC1_INTF_USBPHY_DM_EDGE_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTF_USBPHY_DM_LEVEL_HIGH
#define IO_QSPI_PROC1_INTF_USBPHY_DM_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTF_USBPHY_DM_LEVEL_HIGH_BITS   _u(0x00000020)
#define IO_QSPI_PROC1_INTF_USBPHY_DM_LEVEL_HIGH_MSB    _u(5)
#define IO_QSPI_PROC1_INTF_USBPHY_DM_LEVEL_HIGH_LSB    _u(5)
#define IO_QSPI_PROC1_INTF_USBPHY_DM_LEVEL_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTF_USBPHY_DM_LEVEL_LOW
#define IO_QSPI_PROC1_INTF_USBPHY_DM_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTF_USBPHY_DM_LEVEL_LOW_BITS   _u(0x00000010)
#define IO_QSPI_PROC1_INTF_USBPHY_DM_LEVEL_LOW_MSB    _u(4)
#define IO_QSPI_PROC1_INTF_USBPHY_DM_LEVEL_LOW_LSB    _u(4)
#define IO_QSPI_PROC1_INTF_USBPHY_DM_LEVEL_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTF_USBPHY_DP_EDGE_HIGH
#define IO_QSPI_PROC1_INTF_USBPHY_DP_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTF_USBPHY_DP_EDGE_HIGH_BITS   _u(0x00000008)
#define IO_QSPI_PROC1_INTF_USBPHY_DP_EDGE_HIGH_MSB    _u(3)
#define IO_QSPI_PROC1_INTF_USBPHY_DP_EDGE_HIGH_LSB    _u(3)
#define IO_QSPI_PROC1_INTF_USBPHY_DP_EDGE_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTF_USBPHY_DP_EDGE_LOW
#define IO_QSPI_PROC1_INTF_USBPHY_DP_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTF_USBPHY_DP_EDGE_LOW_BITS   _u(0x00000004)
#define IO_QSPI_PROC1_INTF_USBPHY_DP_EDGE_LOW_MSB    _u(2)
#define IO_QSPI_PROC1_INTF_USBPHY_DP_EDGE_LOW_LSB    _u(2)
#define IO_QSPI_PROC1_INTF_USBPHY_DP_EDGE_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTF_USBPHY_DP_LEVEL_HIGH
#define IO_QSPI_PROC1_INTF_USBPHY_DP_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTF_USBPHY_DP_LEVEL_HIGH_BITS   _u(0x00000002)
#define IO_QSPI_PROC1_INTF_USBPHY_DP_LEVEL_HIGH_MSB    _u(1)
#define IO_QSPI_PROC1_INTF_USBPHY_DP_LEVEL_HIGH_LSB    _u(1)
#define IO_QSPI_PROC1_INTF_USBPHY_DP_LEVEL_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTF_USBPHY_DP_LEVEL_LOW
#define IO_QSPI_PROC1_INTF_USBPHY_DP_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTF_USBPHY_DP_LEVEL_LOW_BITS   _u(0x00000001)
#define IO_QSPI_PROC1_INTF_USBPHY_DP_LEVEL_LOW_MSB    _u(0)
#define IO_QSPI_PROC1_INTF_USBPHY_DP_LEVEL_LOW_LSB    _u(0)
#define IO_QSPI_PROC1_INTF_USBPHY_DP_LEVEL_LOW_ACCESS "RW"
// =============================================================================
// Register    : IO_QSPI_PROC1_INTS
// Description : Interrupt status after masking & forcing for proc1
#define IO_QSPI_PROC1_INTS_OFFSET _u(0x00000230)
#define IO_QSPI_PROC1_INTS_BITS   _u(0xffffffff)
#define IO_QSPI_PROC1_INTS_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTS_GPIO_QSPI_SD3_EDGE_HIGH
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD3_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD3_EDGE_HIGH_BITS   _u(0x80000000)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD3_EDGE_HIGH_MSB    _u(31)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD3_EDGE_HIGH_LSB    _u(31)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD3_EDGE_HIGH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTS_GPIO_QSPI_SD3_EDGE_LOW
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD3_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD3_EDGE_LOW_BITS   _u(0x40000000)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD3_EDGE_LOW_MSB    _u(30)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD3_EDGE_LOW_LSB    _u(30)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD3_EDGE_LOW_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTS_GPIO_QSPI_SD3_LEVEL_HIGH
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD3_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD3_LEVEL_HIGH_BITS   _u(0x20000000)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD3_LEVEL_HIGH_MSB    _u(29)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD3_LEVEL_HIGH_LSB    _u(29)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD3_LEVEL_HIGH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTS_GPIO_QSPI_SD3_LEVEL_LOW
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD3_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD3_LEVEL_LOW_BITS   _u(0x10000000)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD3_LEVEL_LOW_MSB    _u(28)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD3_LEVEL_LOW_LSB    _u(28)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD3_LEVEL_LOW_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTS_GPIO_QSPI_SD2_EDGE_HIGH
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD2_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD2_EDGE_HIGH_BITS   _u(0x08000000)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD2_EDGE_HIGH_MSB    _u(27)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD2_EDGE_HIGH_LSB    _u(27)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD2_EDGE_HIGH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTS_GPIO_QSPI_SD2_EDGE_LOW
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD2_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD2_EDGE_LOW_BITS   _u(0x04000000)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD2_EDGE_LOW_MSB    _u(26)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD2_EDGE_LOW_LSB    _u(26)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD2_EDGE_LOW_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTS_GPIO_QSPI_SD2_LEVEL_HIGH
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD2_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD2_LEVEL_HIGH_BITS   _u(0x02000000)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD2_LEVEL_HIGH_MSB    _u(25)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD2_LEVEL_HIGH_LSB    _u(25)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD2_LEVEL_HIGH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTS_GPIO_QSPI_SD2_LEVEL_LOW
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD2_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD2_LEVEL_LOW_BITS   _u(0x01000000)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD2_LEVEL_LOW_MSB    _u(24)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD2_LEVEL_LOW_LSB    _u(24)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD2_LEVEL_LOW_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTS_GPIO_QSPI_SD1_EDGE_HIGH
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD1_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD1_EDGE_HIGH_BITS   _u(0x00800000)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD1_EDGE_HIGH_MSB    _u(23)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD1_EDGE_HIGH_LSB    _u(23)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD1_EDGE_HIGH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTS_GPIO_QSPI_SD1_EDGE_LOW
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD1_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD1_EDGE_LOW_BITS   _u(0x00400000)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD1_EDGE_LOW_MSB    _u(22)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD1_EDGE_LOW_LSB    _u(22)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD1_EDGE_LOW_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTS_GPIO_QSPI_SD1_LEVEL_HIGH
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD1_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD1_LEVEL_HIGH_BITS   _u(0x00200000)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD1_LEVEL_HIGH_MSB    _u(21)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD1_LEVEL_HIGH_LSB    _u(21)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD1_LEVEL_HIGH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTS_GPIO_QSPI_SD1_LEVEL_LOW
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD1_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD1_LEVEL_LOW_BITS   _u(0x00100000)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD1_LEVEL_LOW_MSB    _u(20)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD1_LEVEL_LOW_LSB    _u(20)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD1_LEVEL_LOW_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTS_GPIO_QSPI_SD0_EDGE_HIGH
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD0_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD0_EDGE_HIGH_BITS   _u(0x00080000)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD0_EDGE_HIGH_MSB    _u(19)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD0_EDGE_HIGH_LSB    _u(19)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD0_EDGE_HIGH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTS_GPIO_QSPI_SD0_EDGE_LOW
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD0_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD0_EDGE_LOW_BITS   _u(0x00040000)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD0_EDGE_LOW_MSB    _u(18)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD0_EDGE_LOW_LSB    _u(18)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD0_EDGE_LOW_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTS_GPIO_QSPI_SD0_LEVEL_HIGH
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD0_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD0_LEVEL_HIGH_BITS   _u(0x00020000)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD0_LEVEL_HIGH_MSB    _u(17)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD0_LEVEL_HIGH_LSB    _u(17)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD0_LEVEL_HIGH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTS_GPIO_QSPI_SD0_LEVEL_LOW
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD0_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD0_LEVEL_LOW_BITS   _u(0x00010000)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD0_LEVEL_LOW_MSB    _u(16)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD0_LEVEL_LOW_LSB    _u(16)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SD0_LEVEL_LOW_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTS_GPIO_QSPI_SS_EDGE_HIGH
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SS_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SS_EDGE_HIGH_BITS   _u(0x00008000)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SS_EDGE_HIGH_MSB    _u(15)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SS_EDGE_HIGH_LSB    _u(15)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SS_EDGE_HIGH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTS_GPIO_QSPI_SS_EDGE_LOW
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SS_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SS_EDGE_LOW_BITS   _u(0x00004000)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SS_EDGE_LOW_MSB    _u(14)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SS_EDGE_LOW_LSB    _u(14)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SS_EDGE_LOW_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTS_GPIO_QSPI_SS_LEVEL_HIGH
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SS_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SS_LEVEL_HIGH_BITS   _u(0x00002000)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SS_LEVEL_HIGH_MSB    _u(13)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SS_LEVEL_HIGH_LSB    _u(13)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SS_LEVEL_HIGH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTS_GPIO_QSPI_SS_LEVEL_LOW
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SS_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SS_LEVEL_LOW_BITS   _u(0x00001000)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SS_LEVEL_LOW_MSB    _u(12)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SS_LEVEL_LOW_LSB    _u(12)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SS_LEVEL_LOW_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTS_GPIO_QSPI_SCLK_EDGE_HIGH
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SCLK_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SCLK_EDGE_HIGH_BITS   _u(0x00000800)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SCLK_EDGE_HIGH_MSB    _u(11)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SCLK_EDGE_HIGH_LSB    _u(11)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SCLK_EDGE_HIGH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTS_GPIO_QSPI_SCLK_EDGE_LOW
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SCLK_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SCLK_EDGE_LOW_BITS   _u(0x00000400)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SCLK_EDGE_LOW_MSB    _u(10)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SCLK_EDGE_LOW_LSB    _u(10)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SCLK_EDGE_LOW_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTS_GPIO_QSPI_SCLK_LEVEL_HIGH
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SCLK_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SCLK_LEVEL_HIGH_BITS   _u(0x00000200)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SCLK_LEVEL_HIGH_MSB    _u(9)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SCLK_LEVEL_HIGH_LSB    _u(9)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SCLK_LEVEL_HIGH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTS_GPIO_QSPI_SCLK_LEVEL_LOW
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SCLK_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SCLK_LEVEL_LOW_BITS   _u(0x00000100)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SCLK_LEVEL_LOW_MSB    _u(8)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SCLK_LEVEL_LOW_LSB    _u(8)
#define IO_QSPI_PROC1_INTS_GPIO_QSPI_SCLK_LEVEL_LOW_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTS_USBPHY_DM_EDGE_HIGH
#define IO_QSPI_PROC1_INTS_USBPHY_DM_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTS_USBPHY_DM_EDGE_HIGH_BITS   _u(0x00000080)
#define IO_QSPI_PROC1_INTS_USBPHY_DM_EDGE_HIGH_MSB    _u(7)
#define IO_QSPI_PROC1_INTS_USBPHY_DM_EDGE_HIGH_LSB    _u(7)
#define IO_QSPI_PROC1_INTS_USBPHY_DM_EDGE_HIGH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTS_USBPHY_DM_EDGE_LOW
#define IO_QSPI_PROC1_INTS_USBPHY_DM_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTS_USBPHY_DM_EDGE_LOW_BITS   _u(0x00000040)
#define IO_QSPI_PROC1_INTS_USBPHY_DM_EDGE_LOW_MSB    _u(6)
#define IO_QSPI_PROC1_INTS_USBPHY_DM_EDGE_LOW_LSB    _u(6)
#define IO_QSPI_PROC1_INTS_USBPHY_DM_EDGE_LOW_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTS_USBPHY_DM_LEVEL_HIGH
#define IO_QSPI_PROC1_INTS_USBPHY_DM_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTS_USBPHY_DM_LEVEL_HIGH_BITS   _u(0x00000020)
#define IO_QSPI_PROC1_INTS_USBPHY_DM_LEVEL_HIGH_MSB    _u(5)
#define IO_QSPI_PROC1_INTS_USBPHY_DM_LEVEL_HIGH_LSB    _u(5)
#define IO_QSPI_PROC1_INTS_USBPHY_DM_LEVEL_HIGH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTS_USBPHY_DM_LEVEL_LOW
#define IO_QSPI_PROC1_INTS_USBPHY_DM_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTS_USBPHY_DM_LEVEL_LOW_BITS   _u(0x00000010)
#define IO_QSPI_PROC1_INTS_USBPHY_DM_LEVEL_LOW_MSB    _u(4)
#define IO_QSPI_PROC1_INTS_USBPHY_DM_LEVEL_LOW_LSB    _u(4)
#define IO_QSPI_PROC1_INTS_USBPHY_DM_LEVEL_LOW_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTS_USBPHY_DP_EDGE_HIGH
#define IO_QSPI_PROC1_INTS_USBPHY_DP_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTS_USBPHY_DP_EDGE_HIGH_BITS   _u(0x00000008)
#define IO_QSPI_PROC1_INTS_USBPHY_DP_EDGE_HIGH_MSB    _u(3)
#define IO_QSPI_PROC1_INTS_USBPHY_DP_EDGE_HIGH_LSB    _u(3)
#define IO_QSPI_PROC1_INTS_USBPHY_DP_EDGE_HIGH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTS_USBPHY_DP_EDGE_LOW
#define IO_QSPI_PROC1_INTS_USBPHY_DP_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTS_USBPHY_DP_EDGE_LOW_BITS   _u(0x00000004)
#define IO_QSPI_PROC1_INTS_USBPHY_DP_EDGE_LOW_MSB    _u(2)
#define IO_QSPI_PROC1_INTS_USBPHY_DP_EDGE_LOW_LSB    _u(2)
#define IO_QSPI_PROC1_INTS_USBPHY_DP_EDGE_LOW_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTS_USBPHY_DP_LEVEL_HIGH
#define IO_QSPI_PROC1_INTS_USBPHY_DP_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTS_USBPHY_DP_LEVEL_HIGH_BITS   _u(0x00000002)
#define IO_QSPI_PROC1_INTS_USBPHY_DP_LEVEL_HIGH_MSB    _u(1)
#define IO_QSPI_PROC1_INTS_USBPHY_DP_LEVEL_HIGH_LSB    _u(1)
#define IO_QSPI_PROC1_INTS_USBPHY_DP_LEVEL_HIGH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_PROC1_INTS_USBPHY_DP_LEVEL_LOW
#define IO_QSPI_PROC1_INTS_USBPHY_DP_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_PROC1_INTS_USBPHY_DP_LEVEL_LOW_BITS   _u(0x00000001)
#define IO_QSPI_PROC1_INTS_USBPHY_DP_LEVEL_LOW_MSB    _u(0)
#define IO_QSPI_PROC1_INTS_USBPHY_DP_LEVEL_LOW_LSB    _u(0)
#define IO_QSPI_PROC1_INTS_USBPHY_DP_LEVEL_LOW_ACCESS "RO"
// =============================================================================
// Register    : IO_QSPI_DORMANT_WAKE_INTE
// Description : Interrupt Enable for dormant_wake
#define IO_QSPI_DORMANT_WAKE_INTE_OFFSET _u(0x00000234)
#define IO_QSPI_DORMANT_WAKE_INTE_BITS   _u(0xffffffff)
#define IO_QSPI_DORMANT_WAKE_INTE_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD3_EDGE_HIGH
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD3_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD3_EDGE_HIGH_BITS   _u(0x80000000)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD3_EDGE_HIGH_MSB    _u(31)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD3_EDGE_HIGH_LSB    _u(31)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD3_EDGE_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD3_EDGE_LOW
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD3_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD3_EDGE_LOW_BITS   _u(0x40000000)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD3_EDGE_LOW_MSB    _u(30)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD3_EDGE_LOW_LSB    _u(30)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD3_EDGE_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD3_LEVEL_HIGH
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD3_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD3_LEVEL_HIGH_BITS   _u(0x20000000)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD3_LEVEL_HIGH_MSB    _u(29)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD3_LEVEL_HIGH_LSB    _u(29)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD3_LEVEL_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD3_LEVEL_LOW
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD3_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD3_LEVEL_LOW_BITS   _u(0x10000000)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD3_LEVEL_LOW_MSB    _u(28)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD3_LEVEL_LOW_LSB    _u(28)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD3_LEVEL_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD2_EDGE_HIGH
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD2_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD2_EDGE_HIGH_BITS   _u(0x08000000)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD2_EDGE_HIGH_MSB    _u(27)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD2_EDGE_HIGH_LSB    _u(27)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD2_EDGE_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD2_EDGE_LOW
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD2_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD2_EDGE_LOW_BITS   _u(0x04000000)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD2_EDGE_LOW_MSB    _u(26)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD2_EDGE_LOW_LSB    _u(26)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD2_EDGE_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD2_LEVEL_HIGH
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD2_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD2_LEVEL_HIGH_BITS   _u(0x02000000)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD2_LEVEL_HIGH_MSB    _u(25)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD2_LEVEL_HIGH_LSB    _u(25)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD2_LEVEL_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD2_LEVEL_LOW
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD2_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD2_LEVEL_LOW_BITS   _u(0x01000000)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD2_LEVEL_LOW_MSB    _u(24)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD2_LEVEL_LOW_LSB    _u(24)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD2_LEVEL_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD1_EDGE_HIGH
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD1_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD1_EDGE_HIGH_BITS   _u(0x00800000)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD1_EDGE_HIGH_MSB    _u(23)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD1_EDGE_HIGH_LSB    _u(23)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD1_EDGE_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD1_EDGE_LOW
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD1_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD1_EDGE_LOW_BITS   _u(0x00400000)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD1_EDGE_LOW_MSB    _u(22)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD1_EDGE_LOW_LSB    _u(22)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD1_EDGE_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD1_LEVEL_HIGH
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD1_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD1_LEVEL_HIGH_BITS   _u(0x00200000)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD1_LEVEL_HIGH_MSB    _u(21)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD1_LEVEL_HIGH_LSB    _u(21)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD1_LEVEL_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD1_LEVEL_LOW
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD1_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD1_LEVEL_LOW_BITS   _u(0x00100000)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD1_LEVEL_LOW_MSB    _u(20)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD1_LEVEL_LOW_LSB    _u(20)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD1_LEVEL_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD0_EDGE_HIGH
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD0_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD0_EDGE_HIGH_BITS   _u(0x00080000)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD0_EDGE_HIGH_MSB    _u(19)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD0_EDGE_HIGH_LSB    _u(19)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD0_EDGE_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD0_EDGE_LOW
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD0_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD0_EDGE_LOW_BITS   _u(0x00040000)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD0_EDGE_LOW_MSB    _u(18)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD0_EDGE_LOW_LSB    _u(18)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD0_EDGE_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD0_LEVEL_HIGH
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD0_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD0_LEVEL_HIGH_BITS   _u(0x00020000)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD0_LEVEL_HIGH_MSB    _u(17)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD0_LEVEL_HIGH_LSB    _u(17)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD0_LEVEL_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD0_LEVEL_LOW
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD0_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD0_LEVEL_LOW_BITS   _u(0x00010000)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD0_LEVEL_LOW_MSB    _u(16)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD0_LEVEL_LOW_LSB    _u(16)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SD0_LEVEL_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SS_EDGE_HIGH
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SS_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SS_EDGE_HIGH_BITS   _u(0x00008000)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SS_EDGE_HIGH_MSB    _u(15)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SS_EDGE_HIGH_LSB    _u(15)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SS_EDGE_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SS_EDGE_LOW
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SS_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SS_EDGE_LOW_BITS   _u(0x00004000)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SS_EDGE_LOW_MSB    _u(14)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SS_EDGE_LOW_LSB    _u(14)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SS_EDGE_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SS_LEVEL_HIGH
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SS_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SS_LEVEL_HIGH_BITS   _u(0x00002000)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SS_LEVEL_HIGH_MSB    _u(13)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SS_LEVEL_HIGH_LSB    _u(13)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SS_LEVEL_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SS_LEVEL_LOW
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SS_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SS_LEVEL_LOW_BITS   _u(0x00001000)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SS_LEVEL_LOW_MSB    _u(12)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SS_LEVEL_LOW_LSB    _u(12)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SS_LEVEL_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SCLK_EDGE_HIGH
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SCLK_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SCLK_EDGE_HIGH_BITS   _u(0x00000800)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SCLK_EDGE_HIGH_MSB    _u(11)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SCLK_EDGE_HIGH_LSB    _u(11)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SCLK_EDGE_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SCLK_EDGE_LOW
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SCLK_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SCLK_EDGE_LOW_BITS   _u(0x00000400)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SCLK_EDGE_LOW_MSB    _u(10)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SCLK_EDGE_LOW_LSB    _u(10)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SCLK_EDGE_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SCLK_LEVEL_HIGH
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SCLK_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SCLK_LEVEL_HIGH_BITS   _u(0x00000200)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SCLK_LEVEL_HIGH_MSB    _u(9)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SCLK_LEVEL_HIGH_LSB    _u(9)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SCLK_LEVEL_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SCLK_LEVEL_LOW
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SCLK_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SCLK_LEVEL_LOW_BITS   _u(0x00000100)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SCLK_LEVEL_LOW_MSB    _u(8)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SCLK_LEVEL_LOW_LSB    _u(8)
#define IO_QSPI_DORMANT_WAKE_INTE_GPIO_QSPI_SCLK_LEVEL_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTE_USBPHY_DM_EDGE_HIGH
#define IO_QSPI_DORMANT_WAKE_INTE_USBPHY_DM_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTE_USBPHY_DM_EDGE_HIGH_BITS   _u(0x00000080)
#define IO_QSPI_DORMANT_WAKE_INTE_USBPHY_DM_EDGE_HIGH_MSB    _u(7)
#define IO_QSPI_DORMANT_WAKE_INTE_USBPHY_DM_EDGE_HIGH_LSB    _u(7)
#define IO_QSPI_DORMANT_WAKE_INTE_USBPHY_DM_EDGE_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTE_USBPHY_DM_EDGE_LOW
#define IO_QSPI_DORMANT_WAKE_INTE_USBPHY_DM_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTE_USBPHY_DM_EDGE_LOW_BITS   _u(0x00000040)
#define IO_QSPI_DORMANT_WAKE_INTE_USBPHY_DM_EDGE_LOW_MSB    _u(6)
#define IO_QSPI_DORMANT_WAKE_INTE_USBPHY_DM_EDGE_LOW_LSB    _u(6)
#define IO_QSPI_DORMANT_WAKE_INTE_USBPHY_DM_EDGE_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTE_USBPHY_DM_LEVEL_HIGH
#define IO_QSPI_DORMANT_WAKE_INTE_USBPHY_DM_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTE_USBPHY_DM_LEVEL_HIGH_BITS   _u(0x00000020)
#define IO_QSPI_DORMANT_WAKE_INTE_USBPHY_DM_LEVEL_HIGH_MSB    _u(5)
#define IO_QSPI_DORMANT_WAKE_INTE_USBPHY_DM_LEVEL_HIGH_LSB    _u(5)
#define IO_QSPI_DORMANT_WAKE_INTE_USBPHY_DM_LEVEL_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTE_USBPHY_DM_LEVEL_LOW
#define IO_QSPI_DORMANT_WAKE_INTE_USBPHY_DM_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTE_USBPHY_DM_LEVEL_LOW_BITS   _u(0x00000010)
#define IO_QSPI_DORMANT_WAKE_INTE_USBPHY_DM_LEVEL_LOW_MSB    _u(4)
#define IO_QSPI_DORMANT_WAKE_INTE_USBPHY_DM_LEVEL_LOW_LSB    _u(4)
#define IO_QSPI_DORMANT_WAKE_INTE_USBPHY_DM_LEVEL_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTE_USBPHY_DP_EDGE_HIGH
#define IO_QSPI_DORMANT_WAKE_INTE_USBPHY_DP_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTE_USBPHY_DP_EDGE_HIGH_BITS   _u(0x00000008)
#define IO_QSPI_DORMANT_WAKE_INTE_USBPHY_DP_EDGE_HIGH_MSB    _u(3)
#define IO_QSPI_DORMANT_WAKE_INTE_USBPHY_DP_EDGE_HIGH_LSB    _u(3)
#define IO_QSPI_DORMANT_WAKE_INTE_USBPHY_DP_EDGE_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTE_USBPHY_DP_EDGE_LOW
#define IO_QSPI_DORMANT_WAKE_INTE_USBPHY_DP_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTE_USBPHY_DP_EDGE_LOW_BITS   _u(0x00000004)
#define IO_QSPI_DORMANT_WAKE_INTE_USBPHY_DP_EDGE_LOW_MSB    _u(2)
#define IO_QSPI_DORMANT_WAKE_INTE_USBPHY_DP_EDGE_LOW_LSB    _u(2)
#define IO_QSPI_DORMANT_WAKE_INTE_USBPHY_DP_EDGE_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTE_USBPHY_DP_LEVEL_HIGH
#define IO_QSPI_DORMANT_WAKE_INTE_USBPHY_DP_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTE_USBPHY_DP_LEVEL_HIGH_BITS   _u(0x00000002)
#define IO_QSPI_DORMANT_WAKE_INTE_USBPHY_DP_LEVEL_HIGH_MSB    _u(1)
#define IO_QSPI_DORMANT_WAKE_INTE_USBPHY_DP_LEVEL_HIGH_LSB    _u(1)
#define IO_QSPI_DORMANT_WAKE_INTE_USBPHY_DP_LEVEL_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTE_USBPHY_DP_LEVEL_LOW
#define IO_QSPI_DORMANT_WAKE_INTE_USBPHY_DP_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTE_USBPHY_DP_LEVEL_LOW_BITS   _u(0x00000001)
#define IO_QSPI_DORMANT_WAKE_INTE_USBPHY_DP_LEVEL_LOW_MSB    _u(0)
#define IO_QSPI_DORMANT_WAKE_INTE_USBPHY_DP_LEVEL_LOW_LSB    _u(0)
#define IO_QSPI_DORMANT_WAKE_INTE_USBPHY_DP_LEVEL_LOW_ACCESS "RW"
// =============================================================================
// Register    : IO_QSPI_DORMANT_WAKE_INTF
// Description : Interrupt Force for dormant_wake
#define IO_QSPI_DORMANT_WAKE_INTF_OFFSET _u(0x00000238)
#define IO_QSPI_DORMANT_WAKE_INTF_BITS   _u(0xffffffff)
#define IO_QSPI_DORMANT_WAKE_INTF_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD3_EDGE_HIGH
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD3_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD3_EDGE_HIGH_BITS   _u(0x80000000)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD3_EDGE_HIGH_MSB    _u(31)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD3_EDGE_HIGH_LSB    _u(31)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD3_EDGE_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD3_EDGE_LOW
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD3_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD3_EDGE_LOW_BITS   _u(0x40000000)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD3_EDGE_LOW_MSB    _u(30)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD3_EDGE_LOW_LSB    _u(30)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD3_EDGE_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD3_LEVEL_HIGH
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD3_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD3_LEVEL_HIGH_BITS   _u(0x20000000)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD3_LEVEL_HIGH_MSB    _u(29)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD3_LEVEL_HIGH_LSB    _u(29)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD3_LEVEL_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD3_LEVEL_LOW
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD3_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD3_LEVEL_LOW_BITS   _u(0x10000000)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD3_LEVEL_LOW_MSB    _u(28)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD3_LEVEL_LOW_LSB    _u(28)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD3_LEVEL_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD2_EDGE_HIGH
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD2_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD2_EDGE_HIGH_BITS   _u(0x08000000)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD2_EDGE_HIGH_MSB    _u(27)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD2_EDGE_HIGH_LSB    _u(27)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD2_EDGE_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD2_EDGE_LOW
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD2_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD2_EDGE_LOW_BITS   _u(0x04000000)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD2_EDGE_LOW_MSB    _u(26)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD2_EDGE_LOW_LSB    _u(26)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD2_EDGE_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD2_LEVEL_HIGH
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD2_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD2_LEVEL_HIGH_BITS   _u(0x02000000)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD2_LEVEL_HIGH_MSB    _u(25)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD2_LEVEL_HIGH_LSB    _u(25)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD2_LEVEL_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD2_LEVEL_LOW
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD2_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD2_LEVEL_LOW_BITS   _u(0x01000000)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD2_LEVEL_LOW_MSB    _u(24)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD2_LEVEL_LOW_LSB    _u(24)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD2_LEVEL_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD1_EDGE_HIGH
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD1_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD1_EDGE_HIGH_BITS   _u(0x00800000)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD1_EDGE_HIGH_MSB    _u(23)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD1_EDGE_HIGH_LSB    _u(23)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD1_EDGE_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD1_EDGE_LOW
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD1_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD1_EDGE_LOW_BITS   _u(0x00400000)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD1_EDGE_LOW_MSB    _u(22)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD1_EDGE_LOW_LSB    _u(22)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD1_EDGE_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD1_LEVEL_HIGH
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD1_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD1_LEVEL_HIGH_BITS   _u(0x00200000)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD1_LEVEL_HIGH_MSB    _u(21)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD1_LEVEL_HIGH_LSB    _u(21)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD1_LEVEL_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD1_LEVEL_LOW
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD1_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD1_LEVEL_LOW_BITS   _u(0x00100000)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD1_LEVEL_LOW_MSB    _u(20)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD1_LEVEL_LOW_LSB    _u(20)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD1_LEVEL_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD0_EDGE_HIGH
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD0_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD0_EDGE_HIGH_BITS   _u(0x00080000)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD0_EDGE_HIGH_MSB    _u(19)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD0_EDGE_HIGH_LSB    _u(19)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD0_EDGE_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD0_EDGE_LOW
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD0_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD0_EDGE_LOW_BITS   _u(0x00040000)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD0_EDGE_LOW_MSB    _u(18)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD0_EDGE_LOW_LSB    _u(18)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD0_EDGE_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD0_LEVEL_HIGH
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD0_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD0_LEVEL_HIGH_BITS   _u(0x00020000)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD0_LEVEL_HIGH_MSB    _u(17)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD0_LEVEL_HIGH_LSB    _u(17)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD0_LEVEL_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD0_LEVEL_LOW
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD0_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD0_LEVEL_LOW_BITS   _u(0x00010000)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD0_LEVEL_LOW_MSB    _u(16)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD0_LEVEL_LOW_LSB    _u(16)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SD0_LEVEL_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SS_EDGE_HIGH
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SS_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SS_EDGE_HIGH_BITS   _u(0x00008000)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SS_EDGE_HIGH_MSB    _u(15)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SS_EDGE_HIGH_LSB    _u(15)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SS_EDGE_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SS_EDGE_LOW
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SS_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SS_EDGE_LOW_BITS   _u(0x00004000)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SS_EDGE_LOW_MSB    _u(14)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SS_EDGE_LOW_LSB    _u(14)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SS_EDGE_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SS_LEVEL_HIGH
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SS_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SS_LEVEL_HIGH_BITS   _u(0x00002000)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SS_LEVEL_HIGH_MSB    _u(13)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SS_LEVEL_HIGH_LSB    _u(13)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SS_LEVEL_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SS_LEVEL_LOW
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SS_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SS_LEVEL_LOW_BITS   _u(0x00001000)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SS_LEVEL_LOW_MSB    _u(12)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SS_LEVEL_LOW_LSB    _u(12)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SS_LEVEL_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SCLK_EDGE_HIGH
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SCLK_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SCLK_EDGE_HIGH_BITS   _u(0x00000800)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SCLK_EDGE_HIGH_MSB    _u(11)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SCLK_EDGE_HIGH_LSB    _u(11)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SCLK_EDGE_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SCLK_EDGE_LOW
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SCLK_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SCLK_EDGE_LOW_BITS   _u(0x00000400)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SCLK_EDGE_LOW_MSB    _u(10)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SCLK_EDGE_LOW_LSB    _u(10)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SCLK_EDGE_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SCLK_LEVEL_HIGH
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SCLK_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SCLK_LEVEL_HIGH_BITS   _u(0x00000200)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SCLK_LEVEL_HIGH_MSB    _u(9)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SCLK_LEVEL_HIGH_LSB    _u(9)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SCLK_LEVEL_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SCLK_LEVEL_LOW
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SCLK_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SCLK_LEVEL_LOW_BITS   _u(0x00000100)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SCLK_LEVEL_LOW_MSB    _u(8)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SCLK_LEVEL_LOW_LSB    _u(8)
#define IO_QSPI_DORMANT_WAKE_INTF_GPIO_QSPI_SCLK_LEVEL_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTF_USBPHY_DM_EDGE_HIGH
#define IO_QSPI_DORMANT_WAKE_INTF_USBPHY_DM_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTF_USBPHY_DM_EDGE_HIGH_BITS   _u(0x00000080)
#define IO_QSPI_DORMANT_WAKE_INTF_USBPHY_DM_EDGE_HIGH_MSB    _u(7)
#define IO_QSPI_DORMANT_WAKE_INTF_USBPHY_DM_EDGE_HIGH_LSB    _u(7)
#define IO_QSPI_DORMANT_WAKE_INTF_USBPHY_DM_EDGE_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTF_USBPHY_DM_EDGE_LOW
#define IO_QSPI_DORMANT_WAKE_INTF_USBPHY_DM_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTF_USBPHY_DM_EDGE_LOW_BITS   _u(0x00000040)
#define IO_QSPI_DORMANT_WAKE_INTF_USBPHY_DM_EDGE_LOW_MSB    _u(6)
#define IO_QSPI_DORMANT_WAKE_INTF_USBPHY_DM_EDGE_LOW_LSB    _u(6)
#define IO_QSPI_DORMANT_WAKE_INTF_USBPHY_DM_EDGE_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTF_USBPHY_DM_LEVEL_HIGH
#define IO_QSPI_DORMANT_WAKE_INTF_USBPHY_DM_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTF_USBPHY_DM_LEVEL_HIGH_BITS   _u(0x00000020)
#define IO_QSPI_DORMANT_WAKE_INTF_USBPHY_DM_LEVEL_HIGH_MSB    _u(5)
#define IO_QSPI_DORMANT_WAKE_INTF_USBPHY_DM_LEVEL_HIGH_LSB    _u(5)
#define IO_QSPI_DORMANT_WAKE_INTF_USBPHY_DM_LEVEL_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTF_USBPHY_DM_LEVEL_LOW
#define IO_QSPI_DORMANT_WAKE_INTF_USBPHY_DM_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTF_USBPHY_DM_LEVEL_LOW_BITS   _u(0x00000010)
#define IO_QSPI_DORMANT_WAKE_INTF_USBPHY_DM_LEVEL_LOW_MSB    _u(4)
#define IO_QSPI_DORMANT_WAKE_INTF_USBPHY_DM_LEVEL_LOW_LSB    _u(4)
#define IO_QSPI_DORMANT_WAKE_INTF_USBPHY_DM_LEVEL_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTF_USBPHY_DP_EDGE_HIGH
#define IO_QSPI_DORMANT_WAKE_INTF_USBPHY_DP_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTF_USBPHY_DP_EDGE_HIGH_BITS   _u(0x00000008)
#define IO_QSPI_DORMANT_WAKE_INTF_USBPHY_DP_EDGE_HIGH_MSB    _u(3)
#define IO_QSPI_DORMANT_WAKE_INTF_USBPHY_DP_EDGE_HIGH_LSB    _u(3)
#define IO_QSPI_DORMANT_WAKE_INTF_USBPHY_DP_EDGE_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTF_USBPHY_DP_EDGE_LOW
#define IO_QSPI_DORMANT_WAKE_INTF_USBPHY_DP_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTF_USBPHY_DP_EDGE_LOW_BITS   _u(0x00000004)
#define IO_QSPI_DORMANT_WAKE_INTF_USBPHY_DP_EDGE_LOW_MSB    _u(2)
#define IO_QSPI_DORMANT_WAKE_INTF_USBPHY_DP_EDGE_LOW_LSB    _u(2)
#define IO_QSPI_DORMANT_WAKE_INTF_USBPHY_DP_EDGE_LOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTF_USBPHY_DP_LEVEL_HIGH
#define IO_QSPI_DORMANT_WAKE_INTF_USBPHY_DP_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTF_USBPHY_DP_LEVEL_HIGH_BITS   _u(0x00000002)
#define IO_QSPI_DORMANT_WAKE_INTF_USBPHY_DP_LEVEL_HIGH_MSB    _u(1)
#define IO_QSPI_DORMANT_WAKE_INTF_USBPHY_DP_LEVEL_HIGH_LSB    _u(1)
#define IO_QSPI_DORMANT_WAKE_INTF_USBPHY_DP_LEVEL_HIGH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTF_USBPHY_DP_LEVEL_LOW
#define IO_QSPI_DORMANT_WAKE_INTF_USBPHY_DP_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTF_USBPHY_DP_LEVEL_LOW_BITS   _u(0x00000001)
#define IO_QSPI_DORMANT_WAKE_INTF_USBPHY_DP_LEVEL_LOW_MSB    _u(0)
#define IO_QSPI_DORMANT_WAKE_INTF_USBPHY_DP_LEVEL_LOW_LSB    _u(0)
#define IO_QSPI_DORMANT_WAKE_INTF_USBPHY_DP_LEVEL_LOW_ACCESS "RW"
// =============================================================================
// Register    : IO_QSPI_DORMANT_WAKE_INTS
// Description : Interrupt status after masking & forcing for dormant_wake
#define IO_QSPI_DORMANT_WAKE_INTS_OFFSET _u(0x0000023c)
#define IO_QSPI_DORMANT_WAKE_INTS_BITS   _u(0xffffffff)
#define IO_QSPI_DORMANT_WAKE_INTS_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD3_EDGE_HIGH
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD3_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD3_EDGE_HIGH_BITS   _u(0x80000000)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD3_EDGE_HIGH_MSB    _u(31)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD3_EDGE_HIGH_LSB    _u(31)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD3_EDGE_HIGH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD3_EDGE_LOW
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD3_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD3_EDGE_LOW_BITS   _u(0x40000000)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD3_EDGE_LOW_MSB    _u(30)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD3_EDGE_LOW_LSB    _u(30)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD3_EDGE_LOW_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD3_LEVEL_HIGH
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD3_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD3_LEVEL_HIGH_BITS   _u(0x20000000)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD3_LEVEL_HIGH_MSB    _u(29)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD3_LEVEL_HIGH_LSB    _u(29)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD3_LEVEL_HIGH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD3_LEVEL_LOW
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD3_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD3_LEVEL_LOW_BITS   _u(0x10000000)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD3_LEVEL_LOW_MSB    _u(28)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD3_LEVEL_LOW_LSB    _u(28)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD3_LEVEL_LOW_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD2_EDGE_HIGH
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD2_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD2_EDGE_HIGH_BITS   _u(0x08000000)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD2_EDGE_HIGH_MSB    _u(27)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD2_EDGE_HIGH_LSB    _u(27)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD2_EDGE_HIGH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD2_EDGE_LOW
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD2_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD2_EDGE_LOW_BITS   _u(0x04000000)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD2_EDGE_LOW_MSB    _u(26)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD2_EDGE_LOW_LSB    _u(26)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD2_EDGE_LOW_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD2_LEVEL_HIGH
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD2_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD2_LEVEL_HIGH_BITS   _u(0x02000000)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD2_LEVEL_HIGH_MSB    _u(25)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD2_LEVEL_HIGH_LSB    _u(25)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD2_LEVEL_HIGH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD2_LEVEL_LOW
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD2_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD2_LEVEL_LOW_BITS   _u(0x01000000)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD2_LEVEL_LOW_MSB    _u(24)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD2_LEVEL_LOW_LSB    _u(24)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD2_LEVEL_LOW_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD1_EDGE_HIGH
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD1_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD1_EDGE_HIGH_BITS   _u(0x00800000)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD1_EDGE_HIGH_MSB    _u(23)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD1_EDGE_HIGH_LSB    _u(23)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD1_EDGE_HIGH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD1_EDGE_LOW
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD1_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD1_EDGE_LOW_BITS   _u(0x00400000)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD1_EDGE_LOW_MSB    _u(22)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD1_EDGE_LOW_LSB    _u(22)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD1_EDGE_LOW_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD1_LEVEL_HIGH
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD1_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD1_LEVEL_HIGH_BITS   _u(0x00200000)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD1_LEVEL_HIGH_MSB    _u(21)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD1_LEVEL_HIGH_LSB    _u(21)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD1_LEVEL_HIGH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD1_LEVEL_LOW
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD1_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD1_LEVEL_LOW_BITS   _u(0x00100000)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD1_LEVEL_LOW_MSB    _u(20)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD1_LEVEL_LOW_LSB    _u(20)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD1_LEVEL_LOW_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD0_EDGE_HIGH
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD0_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD0_EDGE_HIGH_BITS   _u(0x00080000)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD0_EDGE_HIGH_MSB    _u(19)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD0_EDGE_HIGH_LSB    _u(19)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD0_EDGE_HIGH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD0_EDGE_LOW
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD0_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD0_EDGE_LOW_BITS   _u(0x00040000)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD0_EDGE_LOW_MSB    _u(18)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD0_EDGE_LOW_LSB    _u(18)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD0_EDGE_LOW_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD0_LEVEL_HIGH
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD0_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD0_LEVEL_HIGH_BITS   _u(0x00020000)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD0_LEVEL_HIGH_MSB    _u(17)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD0_LEVEL_HIGH_LSB    _u(17)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD0_LEVEL_HIGH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD0_LEVEL_LOW
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD0_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD0_LEVEL_LOW_BITS   _u(0x00010000)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD0_LEVEL_LOW_MSB    _u(16)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD0_LEVEL_LOW_LSB    _u(16)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SD0_LEVEL_LOW_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SS_EDGE_HIGH
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SS_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SS_EDGE_HIGH_BITS   _u(0x00008000)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SS_EDGE_HIGH_MSB    _u(15)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SS_EDGE_HIGH_LSB    _u(15)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SS_EDGE_HIGH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SS_EDGE_LOW
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SS_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SS_EDGE_LOW_BITS   _u(0x00004000)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SS_EDGE_LOW_MSB    _u(14)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SS_EDGE_LOW_LSB    _u(14)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SS_EDGE_LOW_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SS_LEVEL_HIGH
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SS_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SS_LEVEL_HIGH_BITS   _u(0x00002000)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SS_LEVEL_HIGH_MSB    _u(13)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SS_LEVEL_HIGH_LSB    _u(13)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SS_LEVEL_HIGH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SS_LEVEL_LOW
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SS_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SS_LEVEL_LOW_BITS   _u(0x00001000)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SS_LEVEL_LOW_MSB    _u(12)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SS_LEVEL_LOW_LSB    _u(12)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SS_LEVEL_LOW_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SCLK_EDGE_HIGH
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SCLK_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SCLK_EDGE_HIGH_BITS   _u(0x00000800)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SCLK_EDGE_HIGH_MSB    _u(11)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SCLK_EDGE_HIGH_LSB    _u(11)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SCLK_EDGE_HIGH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SCLK_EDGE_LOW
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SCLK_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SCLK_EDGE_LOW_BITS   _u(0x00000400)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SCLK_EDGE_LOW_MSB    _u(10)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SCLK_EDGE_LOW_LSB    _u(10)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SCLK_EDGE_LOW_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SCLK_LEVEL_HIGH
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SCLK_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SCLK_LEVEL_HIGH_BITS   _u(0x00000200)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SCLK_LEVEL_HIGH_MSB    _u(9)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SCLK_LEVEL_HIGH_LSB    _u(9)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SCLK_LEVEL_HIGH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SCLK_LEVEL_LOW
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SCLK_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SCLK_LEVEL_LOW_BITS   _u(0x00000100)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SCLK_LEVEL_LOW_MSB    _u(8)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SCLK_LEVEL_LOW_LSB    _u(8)
#define IO_QSPI_DORMANT_WAKE_INTS_GPIO_QSPI_SCLK_LEVEL_LOW_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTS_USBPHY_DM_EDGE_HIGH
#define IO_QSPI_DORMANT_WAKE_INTS_USBPHY_DM_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTS_USBPHY_DM_EDGE_HIGH_BITS   _u(0x00000080)
#define IO_QSPI_DORMANT_WAKE_INTS_USBPHY_DM_EDGE_HIGH_MSB    _u(7)
#define IO_QSPI_DORMANT_WAKE_INTS_USBPHY_DM_EDGE_HIGH_LSB    _u(7)
#define IO_QSPI_DORMANT_WAKE_INTS_USBPHY_DM_EDGE_HIGH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTS_USBPHY_DM_EDGE_LOW
#define IO_QSPI_DORMANT_WAKE_INTS_USBPHY_DM_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTS_USBPHY_DM_EDGE_LOW_BITS   _u(0x00000040)
#define IO_QSPI_DORMANT_WAKE_INTS_USBPHY_DM_EDGE_LOW_MSB    _u(6)
#define IO_QSPI_DORMANT_WAKE_INTS_USBPHY_DM_EDGE_LOW_LSB    _u(6)
#define IO_QSPI_DORMANT_WAKE_INTS_USBPHY_DM_EDGE_LOW_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTS_USBPHY_DM_LEVEL_HIGH
#define IO_QSPI_DORMANT_WAKE_INTS_USBPHY_DM_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTS_USBPHY_DM_LEVEL_HIGH_BITS   _u(0x00000020)
#define IO_QSPI_DORMANT_WAKE_INTS_USBPHY_DM_LEVEL_HIGH_MSB    _u(5)
#define IO_QSPI_DORMANT_WAKE_INTS_USBPHY_DM_LEVEL_HIGH_LSB    _u(5)
#define IO_QSPI_DORMANT_WAKE_INTS_USBPHY_DM_LEVEL_HIGH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTS_USBPHY_DM_LEVEL_LOW
#define IO_QSPI_DORMANT_WAKE_INTS_USBPHY_DM_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTS_USBPHY_DM_LEVEL_LOW_BITS   _u(0x00000010)
#define IO_QSPI_DORMANT_WAKE_INTS_USBPHY_DM_LEVEL_LOW_MSB    _u(4)
#define IO_QSPI_DORMANT_WAKE_INTS_USBPHY_DM_LEVEL_LOW_LSB    _u(4)
#define IO_QSPI_DORMANT_WAKE_INTS_USBPHY_DM_LEVEL_LOW_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTS_USBPHY_DP_EDGE_HIGH
#define IO_QSPI_DORMANT_WAKE_INTS_USBPHY_DP_EDGE_HIGH_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTS_USBPHY_DP_EDGE_HIGH_BITS   _u(0x00000008)
#define IO_QSPI_DORMANT_WAKE_INTS_USBPHY_DP_EDGE_HIGH_MSB    _u(3)
#define IO_QSPI_DORMANT_WAKE_INTS_USBPHY_DP_EDGE_HIGH_LSB    _u(3)
#define IO_QSPI_DORMANT_WAKE_INTS_USBPHY_DP_EDGE_HIGH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTS_USBPHY_DP_EDGE_LOW
#define IO_QSPI_DORMANT_WAKE_INTS_USBPHY_DP_EDGE_LOW_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTS_USBPHY_DP_EDGE_LOW_BITS   _u(0x00000004)
#define IO_QSPI_DORMANT_WAKE_INTS_USBPHY_DP_EDGE_LOW_MSB    _u(2)
#define IO_QSPI_DORMANT_WAKE_INTS_USBPHY_DP_EDGE_LOW_LSB    _u(2)
#define IO_QSPI_DORMANT_WAKE_INTS_USBPHY_DP_EDGE_LOW_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTS_USBPHY_DP_LEVEL_HIGH
#define IO_QSPI_DORMANT_WAKE_INTS_USBPHY_DP_LEVEL_HIGH_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTS_USBPHY_DP_LEVEL_HIGH_BITS   _u(0x00000002)
#define IO_QSPI_DORMANT_WAKE_INTS_USBPHY_DP_LEVEL_HIGH_MSB    _u(1)
#define IO_QSPI_DORMANT_WAKE_INTS_USBPHY_DP_LEVEL_HIGH_LSB    _u(1)
#define IO_QSPI_DORMANT_WAKE_INTS_USBPHY_DP_LEVEL_HIGH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : IO_QSPI_DORMANT_WAKE_INTS_USBPHY_DP_LEVEL_LOW
#define IO_QSPI_DORMANT_WAKE_INTS_USBPHY_DP_LEVEL_LOW_RESET  _u(0x0)
#define IO_QSPI_DORMANT_WAKE_INTS_USBPHY_DP_LEVEL_LOW_BITS   _u(0x00000001)
#define IO_QSPI_DORMANT_WAKE_INTS_USBPHY_DP_LEVEL_LOW_MSB    _u(0)
#define IO_QSPI_DORMANT_WAKE_INTS_USBPHY_DP_LEVEL_LOW_LSB    _u(0)
#define IO_QSPI_DORMANT_WAKE_INTS_USBPHY_DP_LEVEL_LOW_ACCESS "RO"
// =============================================================================
#endif // _HARDWARE_REGS_IO_QSPI_H

