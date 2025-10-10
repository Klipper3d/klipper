// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
// =============================================================================
// Register block : PSM
// Version        : 1
// Bus type       : apb
// =============================================================================
#ifndef _HARDWARE_REGS_PSM_H
#define _HARDWARE_REGS_PSM_H
// =============================================================================
// Register    : PSM_FRCE_ON
// Description : Force block out of reset (i.e. power it on)
#define PSM_FRCE_ON_OFFSET _u(0x00000000)
#define PSM_FRCE_ON_BITS   _u(0x01ffffff)
#define PSM_FRCE_ON_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : PSM_FRCE_ON_PROC1
#define PSM_FRCE_ON_PROC1_RESET  _u(0x0)
#define PSM_FRCE_ON_PROC1_BITS   _u(0x01000000)
#define PSM_FRCE_ON_PROC1_MSB    _u(24)
#define PSM_FRCE_ON_PROC1_LSB    _u(24)
#define PSM_FRCE_ON_PROC1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_FRCE_ON_PROC0
#define PSM_FRCE_ON_PROC0_RESET  _u(0x0)
#define PSM_FRCE_ON_PROC0_BITS   _u(0x00800000)
#define PSM_FRCE_ON_PROC0_MSB    _u(23)
#define PSM_FRCE_ON_PROC0_LSB    _u(23)
#define PSM_FRCE_ON_PROC0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_FRCE_ON_ACCESSCTRL
#define PSM_FRCE_ON_ACCESSCTRL_RESET  _u(0x0)
#define PSM_FRCE_ON_ACCESSCTRL_BITS   _u(0x00400000)
#define PSM_FRCE_ON_ACCESSCTRL_MSB    _u(22)
#define PSM_FRCE_ON_ACCESSCTRL_LSB    _u(22)
#define PSM_FRCE_ON_ACCESSCTRL_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_FRCE_ON_SIO
#define PSM_FRCE_ON_SIO_RESET  _u(0x0)
#define PSM_FRCE_ON_SIO_BITS   _u(0x00200000)
#define PSM_FRCE_ON_SIO_MSB    _u(21)
#define PSM_FRCE_ON_SIO_LSB    _u(21)
#define PSM_FRCE_ON_SIO_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_FRCE_ON_XIP
#define PSM_FRCE_ON_XIP_RESET  _u(0x0)
#define PSM_FRCE_ON_XIP_BITS   _u(0x00100000)
#define PSM_FRCE_ON_XIP_MSB    _u(20)
#define PSM_FRCE_ON_XIP_LSB    _u(20)
#define PSM_FRCE_ON_XIP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_FRCE_ON_SRAM9
#define PSM_FRCE_ON_SRAM9_RESET  _u(0x0)
#define PSM_FRCE_ON_SRAM9_BITS   _u(0x00080000)
#define PSM_FRCE_ON_SRAM9_MSB    _u(19)
#define PSM_FRCE_ON_SRAM9_LSB    _u(19)
#define PSM_FRCE_ON_SRAM9_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_FRCE_ON_SRAM8
#define PSM_FRCE_ON_SRAM8_RESET  _u(0x0)
#define PSM_FRCE_ON_SRAM8_BITS   _u(0x00040000)
#define PSM_FRCE_ON_SRAM8_MSB    _u(18)
#define PSM_FRCE_ON_SRAM8_LSB    _u(18)
#define PSM_FRCE_ON_SRAM8_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_FRCE_ON_SRAM7
#define PSM_FRCE_ON_SRAM7_RESET  _u(0x0)
#define PSM_FRCE_ON_SRAM7_BITS   _u(0x00020000)
#define PSM_FRCE_ON_SRAM7_MSB    _u(17)
#define PSM_FRCE_ON_SRAM7_LSB    _u(17)
#define PSM_FRCE_ON_SRAM7_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_FRCE_ON_SRAM6
#define PSM_FRCE_ON_SRAM6_RESET  _u(0x0)
#define PSM_FRCE_ON_SRAM6_BITS   _u(0x00010000)
#define PSM_FRCE_ON_SRAM6_MSB    _u(16)
#define PSM_FRCE_ON_SRAM6_LSB    _u(16)
#define PSM_FRCE_ON_SRAM6_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_FRCE_ON_SRAM5
#define PSM_FRCE_ON_SRAM5_RESET  _u(0x0)
#define PSM_FRCE_ON_SRAM5_BITS   _u(0x00008000)
#define PSM_FRCE_ON_SRAM5_MSB    _u(15)
#define PSM_FRCE_ON_SRAM5_LSB    _u(15)
#define PSM_FRCE_ON_SRAM5_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_FRCE_ON_SRAM4
#define PSM_FRCE_ON_SRAM4_RESET  _u(0x0)
#define PSM_FRCE_ON_SRAM4_BITS   _u(0x00004000)
#define PSM_FRCE_ON_SRAM4_MSB    _u(14)
#define PSM_FRCE_ON_SRAM4_LSB    _u(14)
#define PSM_FRCE_ON_SRAM4_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_FRCE_ON_SRAM3
#define PSM_FRCE_ON_SRAM3_RESET  _u(0x0)
#define PSM_FRCE_ON_SRAM3_BITS   _u(0x00002000)
#define PSM_FRCE_ON_SRAM3_MSB    _u(13)
#define PSM_FRCE_ON_SRAM3_LSB    _u(13)
#define PSM_FRCE_ON_SRAM3_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_FRCE_ON_SRAM2
#define PSM_FRCE_ON_SRAM2_RESET  _u(0x0)
#define PSM_FRCE_ON_SRAM2_BITS   _u(0x00001000)
#define PSM_FRCE_ON_SRAM2_MSB    _u(12)
#define PSM_FRCE_ON_SRAM2_LSB    _u(12)
#define PSM_FRCE_ON_SRAM2_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_FRCE_ON_SRAM1
#define PSM_FRCE_ON_SRAM1_RESET  _u(0x0)
#define PSM_FRCE_ON_SRAM1_BITS   _u(0x00000800)
#define PSM_FRCE_ON_SRAM1_MSB    _u(11)
#define PSM_FRCE_ON_SRAM1_LSB    _u(11)
#define PSM_FRCE_ON_SRAM1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_FRCE_ON_SRAM0
#define PSM_FRCE_ON_SRAM0_RESET  _u(0x0)
#define PSM_FRCE_ON_SRAM0_BITS   _u(0x00000400)
#define PSM_FRCE_ON_SRAM0_MSB    _u(10)
#define PSM_FRCE_ON_SRAM0_LSB    _u(10)
#define PSM_FRCE_ON_SRAM0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_FRCE_ON_BOOTRAM
#define PSM_FRCE_ON_BOOTRAM_RESET  _u(0x0)
#define PSM_FRCE_ON_BOOTRAM_BITS   _u(0x00000200)
#define PSM_FRCE_ON_BOOTRAM_MSB    _u(9)
#define PSM_FRCE_ON_BOOTRAM_LSB    _u(9)
#define PSM_FRCE_ON_BOOTRAM_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_FRCE_ON_ROM
#define PSM_FRCE_ON_ROM_RESET  _u(0x0)
#define PSM_FRCE_ON_ROM_BITS   _u(0x00000100)
#define PSM_FRCE_ON_ROM_MSB    _u(8)
#define PSM_FRCE_ON_ROM_LSB    _u(8)
#define PSM_FRCE_ON_ROM_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_FRCE_ON_BUSFABRIC
#define PSM_FRCE_ON_BUSFABRIC_RESET  _u(0x0)
#define PSM_FRCE_ON_BUSFABRIC_BITS   _u(0x00000080)
#define PSM_FRCE_ON_BUSFABRIC_MSB    _u(7)
#define PSM_FRCE_ON_BUSFABRIC_LSB    _u(7)
#define PSM_FRCE_ON_BUSFABRIC_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_FRCE_ON_PSM_READY
#define PSM_FRCE_ON_PSM_READY_RESET  _u(0x0)
#define PSM_FRCE_ON_PSM_READY_BITS   _u(0x00000040)
#define PSM_FRCE_ON_PSM_READY_MSB    _u(6)
#define PSM_FRCE_ON_PSM_READY_LSB    _u(6)
#define PSM_FRCE_ON_PSM_READY_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_FRCE_ON_CLOCKS
#define PSM_FRCE_ON_CLOCKS_RESET  _u(0x0)
#define PSM_FRCE_ON_CLOCKS_BITS   _u(0x00000020)
#define PSM_FRCE_ON_CLOCKS_MSB    _u(5)
#define PSM_FRCE_ON_CLOCKS_LSB    _u(5)
#define PSM_FRCE_ON_CLOCKS_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_FRCE_ON_RESETS
#define PSM_FRCE_ON_RESETS_RESET  _u(0x0)
#define PSM_FRCE_ON_RESETS_BITS   _u(0x00000010)
#define PSM_FRCE_ON_RESETS_MSB    _u(4)
#define PSM_FRCE_ON_RESETS_LSB    _u(4)
#define PSM_FRCE_ON_RESETS_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_FRCE_ON_XOSC
#define PSM_FRCE_ON_XOSC_RESET  _u(0x0)
#define PSM_FRCE_ON_XOSC_BITS   _u(0x00000008)
#define PSM_FRCE_ON_XOSC_MSB    _u(3)
#define PSM_FRCE_ON_XOSC_LSB    _u(3)
#define PSM_FRCE_ON_XOSC_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_FRCE_ON_ROSC
#define PSM_FRCE_ON_ROSC_RESET  _u(0x0)
#define PSM_FRCE_ON_ROSC_BITS   _u(0x00000004)
#define PSM_FRCE_ON_ROSC_MSB    _u(2)
#define PSM_FRCE_ON_ROSC_LSB    _u(2)
#define PSM_FRCE_ON_ROSC_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_FRCE_ON_OTP
#define PSM_FRCE_ON_OTP_RESET  _u(0x0)
#define PSM_FRCE_ON_OTP_BITS   _u(0x00000002)
#define PSM_FRCE_ON_OTP_MSB    _u(1)
#define PSM_FRCE_ON_OTP_LSB    _u(1)
#define PSM_FRCE_ON_OTP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_FRCE_ON_PROC_COLD
#define PSM_FRCE_ON_PROC_COLD_RESET  _u(0x0)
#define PSM_FRCE_ON_PROC_COLD_BITS   _u(0x00000001)
#define PSM_FRCE_ON_PROC_COLD_MSB    _u(0)
#define PSM_FRCE_ON_PROC_COLD_LSB    _u(0)
#define PSM_FRCE_ON_PROC_COLD_ACCESS "RW"
// =============================================================================
// Register    : PSM_FRCE_OFF
// Description : Force into reset (i.e. power it off)
#define PSM_FRCE_OFF_OFFSET _u(0x00000004)
#define PSM_FRCE_OFF_BITS   _u(0x01ffffff)
#define PSM_FRCE_OFF_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : PSM_FRCE_OFF_PROC1
#define PSM_FRCE_OFF_PROC1_RESET  _u(0x0)
#define PSM_FRCE_OFF_PROC1_BITS   _u(0x01000000)
#define PSM_FRCE_OFF_PROC1_MSB    _u(24)
#define PSM_FRCE_OFF_PROC1_LSB    _u(24)
#define PSM_FRCE_OFF_PROC1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_FRCE_OFF_PROC0
#define PSM_FRCE_OFF_PROC0_RESET  _u(0x0)
#define PSM_FRCE_OFF_PROC0_BITS   _u(0x00800000)
#define PSM_FRCE_OFF_PROC0_MSB    _u(23)
#define PSM_FRCE_OFF_PROC0_LSB    _u(23)
#define PSM_FRCE_OFF_PROC0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_FRCE_OFF_ACCESSCTRL
#define PSM_FRCE_OFF_ACCESSCTRL_RESET  _u(0x0)
#define PSM_FRCE_OFF_ACCESSCTRL_BITS   _u(0x00400000)
#define PSM_FRCE_OFF_ACCESSCTRL_MSB    _u(22)
#define PSM_FRCE_OFF_ACCESSCTRL_LSB    _u(22)
#define PSM_FRCE_OFF_ACCESSCTRL_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_FRCE_OFF_SIO
#define PSM_FRCE_OFF_SIO_RESET  _u(0x0)
#define PSM_FRCE_OFF_SIO_BITS   _u(0x00200000)
#define PSM_FRCE_OFF_SIO_MSB    _u(21)
#define PSM_FRCE_OFF_SIO_LSB    _u(21)
#define PSM_FRCE_OFF_SIO_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_FRCE_OFF_XIP
#define PSM_FRCE_OFF_XIP_RESET  _u(0x0)
#define PSM_FRCE_OFF_XIP_BITS   _u(0x00100000)
#define PSM_FRCE_OFF_XIP_MSB    _u(20)
#define PSM_FRCE_OFF_XIP_LSB    _u(20)
#define PSM_FRCE_OFF_XIP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_FRCE_OFF_SRAM9
#define PSM_FRCE_OFF_SRAM9_RESET  _u(0x0)
#define PSM_FRCE_OFF_SRAM9_BITS   _u(0x00080000)
#define PSM_FRCE_OFF_SRAM9_MSB    _u(19)
#define PSM_FRCE_OFF_SRAM9_LSB    _u(19)
#define PSM_FRCE_OFF_SRAM9_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_FRCE_OFF_SRAM8
#define PSM_FRCE_OFF_SRAM8_RESET  _u(0x0)
#define PSM_FRCE_OFF_SRAM8_BITS   _u(0x00040000)
#define PSM_FRCE_OFF_SRAM8_MSB    _u(18)
#define PSM_FRCE_OFF_SRAM8_LSB    _u(18)
#define PSM_FRCE_OFF_SRAM8_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_FRCE_OFF_SRAM7
#define PSM_FRCE_OFF_SRAM7_RESET  _u(0x0)
#define PSM_FRCE_OFF_SRAM7_BITS   _u(0x00020000)
#define PSM_FRCE_OFF_SRAM7_MSB    _u(17)
#define PSM_FRCE_OFF_SRAM7_LSB    _u(17)
#define PSM_FRCE_OFF_SRAM7_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_FRCE_OFF_SRAM6
#define PSM_FRCE_OFF_SRAM6_RESET  _u(0x0)
#define PSM_FRCE_OFF_SRAM6_BITS   _u(0x00010000)
#define PSM_FRCE_OFF_SRAM6_MSB    _u(16)
#define PSM_FRCE_OFF_SRAM6_LSB    _u(16)
#define PSM_FRCE_OFF_SRAM6_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_FRCE_OFF_SRAM5
#define PSM_FRCE_OFF_SRAM5_RESET  _u(0x0)
#define PSM_FRCE_OFF_SRAM5_BITS   _u(0x00008000)
#define PSM_FRCE_OFF_SRAM5_MSB    _u(15)
#define PSM_FRCE_OFF_SRAM5_LSB    _u(15)
#define PSM_FRCE_OFF_SRAM5_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_FRCE_OFF_SRAM4
#define PSM_FRCE_OFF_SRAM4_RESET  _u(0x0)
#define PSM_FRCE_OFF_SRAM4_BITS   _u(0x00004000)
#define PSM_FRCE_OFF_SRAM4_MSB    _u(14)
#define PSM_FRCE_OFF_SRAM4_LSB    _u(14)
#define PSM_FRCE_OFF_SRAM4_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_FRCE_OFF_SRAM3
#define PSM_FRCE_OFF_SRAM3_RESET  _u(0x0)
#define PSM_FRCE_OFF_SRAM3_BITS   _u(0x00002000)
#define PSM_FRCE_OFF_SRAM3_MSB    _u(13)
#define PSM_FRCE_OFF_SRAM3_LSB    _u(13)
#define PSM_FRCE_OFF_SRAM3_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_FRCE_OFF_SRAM2
#define PSM_FRCE_OFF_SRAM2_RESET  _u(0x0)
#define PSM_FRCE_OFF_SRAM2_BITS   _u(0x00001000)
#define PSM_FRCE_OFF_SRAM2_MSB    _u(12)
#define PSM_FRCE_OFF_SRAM2_LSB    _u(12)
#define PSM_FRCE_OFF_SRAM2_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_FRCE_OFF_SRAM1
#define PSM_FRCE_OFF_SRAM1_RESET  _u(0x0)
#define PSM_FRCE_OFF_SRAM1_BITS   _u(0x00000800)
#define PSM_FRCE_OFF_SRAM1_MSB    _u(11)
#define PSM_FRCE_OFF_SRAM1_LSB    _u(11)
#define PSM_FRCE_OFF_SRAM1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_FRCE_OFF_SRAM0
#define PSM_FRCE_OFF_SRAM0_RESET  _u(0x0)
#define PSM_FRCE_OFF_SRAM0_BITS   _u(0x00000400)
#define PSM_FRCE_OFF_SRAM0_MSB    _u(10)
#define PSM_FRCE_OFF_SRAM0_LSB    _u(10)
#define PSM_FRCE_OFF_SRAM0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_FRCE_OFF_BOOTRAM
#define PSM_FRCE_OFF_BOOTRAM_RESET  _u(0x0)
#define PSM_FRCE_OFF_BOOTRAM_BITS   _u(0x00000200)
#define PSM_FRCE_OFF_BOOTRAM_MSB    _u(9)
#define PSM_FRCE_OFF_BOOTRAM_LSB    _u(9)
#define PSM_FRCE_OFF_BOOTRAM_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_FRCE_OFF_ROM
#define PSM_FRCE_OFF_ROM_RESET  _u(0x0)
#define PSM_FRCE_OFF_ROM_BITS   _u(0x00000100)
#define PSM_FRCE_OFF_ROM_MSB    _u(8)
#define PSM_FRCE_OFF_ROM_LSB    _u(8)
#define PSM_FRCE_OFF_ROM_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_FRCE_OFF_BUSFABRIC
#define PSM_FRCE_OFF_BUSFABRIC_RESET  _u(0x0)
#define PSM_FRCE_OFF_BUSFABRIC_BITS   _u(0x00000080)
#define PSM_FRCE_OFF_BUSFABRIC_MSB    _u(7)
#define PSM_FRCE_OFF_BUSFABRIC_LSB    _u(7)
#define PSM_FRCE_OFF_BUSFABRIC_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_FRCE_OFF_PSM_READY
#define PSM_FRCE_OFF_PSM_READY_RESET  _u(0x0)
#define PSM_FRCE_OFF_PSM_READY_BITS   _u(0x00000040)
#define PSM_FRCE_OFF_PSM_READY_MSB    _u(6)
#define PSM_FRCE_OFF_PSM_READY_LSB    _u(6)
#define PSM_FRCE_OFF_PSM_READY_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_FRCE_OFF_CLOCKS
#define PSM_FRCE_OFF_CLOCKS_RESET  _u(0x0)
#define PSM_FRCE_OFF_CLOCKS_BITS   _u(0x00000020)
#define PSM_FRCE_OFF_CLOCKS_MSB    _u(5)
#define PSM_FRCE_OFF_CLOCKS_LSB    _u(5)
#define PSM_FRCE_OFF_CLOCKS_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_FRCE_OFF_RESETS
#define PSM_FRCE_OFF_RESETS_RESET  _u(0x0)
#define PSM_FRCE_OFF_RESETS_BITS   _u(0x00000010)
#define PSM_FRCE_OFF_RESETS_MSB    _u(4)
#define PSM_FRCE_OFF_RESETS_LSB    _u(4)
#define PSM_FRCE_OFF_RESETS_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_FRCE_OFF_XOSC
#define PSM_FRCE_OFF_XOSC_RESET  _u(0x0)
#define PSM_FRCE_OFF_XOSC_BITS   _u(0x00000008)
#define PSM_FRCE_OFF_XOSC_MSB    _u(3)
#define PSM_FRCE_OFF_XOSC_LSB    _u(3)
#define PSM_FRCE_OFF_XOSC_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_FRCE_OFF_ROSC
#define PSM_FRCE_OFF_ROSC_RESET  _u(0x0)
#define PSM_FRCE_OFF_ROSC_BITS   _u(0x00000004)
#define PSM_FRCE_OFF_ROSC_MSB    _u(2)
#define PSM_FRCE_OFF_ROSC_LSB    _u(2)
#define PSM_FRCE_OFF_ROSC_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_FRCE_OFF_OTP
#define PSM_FRCE_OFF_OTP_RESET  _u(0x0)
#define PSM_FRCE_OFF_OTP_BITS   _u(0x00000002)
#define PSM_FRCE_OFF_OTP_MSB    _u(1)
#define PSM_FRCE_OFF_OTP_LSB    _u(1)
#define PSM_FRCE_OFF_OTP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_FRCE_OFF_PROC_COLD
#define PSM_FRCE_OFF_PROC_COLD_RESET  _u(0x0)
#define PSM_FRCE_OFF_PROC_COLD_BITS   _u(0x00000001)
#define PSM_FRCE_OFF_PROC_COLD_MSB    _u(0)
#define PSM_FRCE_OFF_PROC_COLD_LSB    _u(0)
#define PSM_FRCE_OFF_PROC_COLD_ACCESS "RW"
// =============================================================================
// Register    : PSM_WDSEL
// Description : Set to 1 if the watchdog should reset this
#define PSM_WDSEL_OFFSET _u(0x00000008)
#define PSM_WDSEL_BITS   _u(0x01ffffff)
#define PSM_WDSEL_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : PSM_WDSEL_PROC1
#define PSM_WDSEL_PROC1_RESET  _u(0x0)
#define PSM_WDSEL_PROC1_BITS   _u(0x01000000)
#define PSM_WDSEL_PROC1_MSB    _u(24)
#define PSM_WDSEL_PROC1_LSB    _u(24)
#define PSM_WDSEL_PROC1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_WDSEL_PROC0
#define PSM_WDSEL_PROC0_RESET  _u(0x0)
#define PSM_WDSEL_PROC0_BITS   _u(0x00800000)
#define PSM_WDSEL_PROC0_MSB    _u(23)
#define PSM_WDSEL_PROC0_LSB    _u(23)
#define PSM_WDSEL_PROC0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_WDSEL_ACCESSCTRL
#define PSM_WDSEL_ACCESSCTRL_RESET  _u(0x0)
#define PSM_WDSEL_ACCESSCTRL_BITS   _u(0x00400000)
#define PSM_WDSEL_ACCESSCTRL_MSB    _u(22)
#define PSM_WDSEL_ACCESSCTRL_LSB    _u(22)
#define PSM_WDSEL_ACCESSCTRL_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_WDSEL_SIO
#define PSM_WDSEL_SIO_RESET  _u(0x0)
#define PSM_WDSEL_SIO_BITS   _u(0x00200000)
#define PSM_WDSEL_SIO_MSB    _u(21)
#define PSM_WDSEL_SIO_LSB    _u(21)
#define PSM_WDSEL_SIO_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_WDSEL_XIP
#define PSM_WDSEL_XIP_RESET  _u(0x0)
#define PSM_WDSEL_XIP_BITS   _u(0x00100000)
#define PSM_WDSEL_XIP_MSB    _u(20)
#define PSM_WDSEL_XIP_LSB    _u(20)
#define PSM_WDSEL_XIP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_WDSEL_SRAM9
#define PSM_WDSEL_SRAM9_RESET  _u(0x0)
#define PSM_WDSEL_SRAM9_BITS   _u(0x00080000)
#define PSM_WDSEL_SRAM9_MSB    _u(19)
#define PSM_WDSEL_SRAM9_LSB    _u(19)
#define PSM_WDSEL_SRAM9_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_WDSEL_SRAM8
#define PSM_WDSEL_SRAM8_RESET  _u(0x0)
#define PSM_WDSEL_SRAM8_BITS   _u(0x00040000)
#define PSM_WDSEL_SRAM8_MSB    _u(18)
#define PSM_WDSEL_SRAM8_LSB    _u(18)
#define PSM_WDSEL_SRAM8_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_WDSEL_SRAM7
#define PSM_WDSEL_SRAM7_RESET  _u(0x0)
#define PSM_WDSEL_SRAM7_BITS   _u(0x00020000)
#define PSM_WDSEL_SRAM7_MSB    _u(17)
#define PSM_WDSEL_SRAM7_LSB    _u(17)
#define PSM_WDSEL_SRAM7_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_WDSEL_SRAM6
#define PSM_WDSEL_SRAM6_RESET  _u(0x0)
#define PSM_WDSEL_SRAM6_BITS   _u(0x00010000)
#define PSM_WDSEL_SRAM6_MSB    _u(16)
#define PSM_WDSEL_SRAM6_LSB    _u(16)
#define PSM_WDSEL_SRAM6_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_WDSEL_SRAM5
#define PSM_WDSEL_SRAM5_RESET  _u(0x0)
#define PSM_WDSEL_SRAM5_BITS   _u(0x00008000)
#define PSM_WDSEL_SRAM5_MSB    _u(15)
#define PSM_WDSEL_SRAM5_LSB    _u(15)
#define PSM_WDSEL_SRAM5_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_WDSEL_SRAM4
#define PSM_WDSEL_SRAM4_RESET  _u(0x0)
#define PSM_WDSEL_SRAM4_BITS   _u(0x00004000)
#define PSM_WDSEL_SRAM4_MSB    _u(14)
#define PSM_WDSEL_SRAM4_LSB    _u(14)
#define PSM_WDSEL_SRAM4_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_WDSEL_SRAM3
#define PSM_WDSEL_SRAM3_RESET  _u(0x0)
#define PSM_WDSEL_SRAM3_BITS   _u(0x00002000)
#define PSM_WDSEL_SRAM3_MSB    _u(13)
#define PSM_WDSEL_SRAM3_LSB    _u(13)
#define PSM_WDSEL_SRAM3_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_WDSEL_SRAM2
#define PSM_WDSEL_SRAM2_RESET  _u(0x0)
#define PSM_WDSEL_SRAM2_BITS   _u(0x00001000)
#define PSM_WDSEL_SRAM2_MSB    _u(12)
#define PSM_WDSEL_SRAM2_LSB    _u(12)
#define PSM_WDSEL_SRAM2_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_WDSEL_SRAM1
#define PSM_WDSEL_SRAM1_RESET  _u(0x0)
#define PSM_WDSEL_SRAM1_BITS   _u(0x00000800)
#define PSM_WDSEL_SRAM1_MSB    _u(11)
#define PSM_WDSEL_SRAM1_LSB    _u(11)
#define PSM_WDSEL_SRAM1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_WDSEL_SRAM0
#define PSM_WDSEL_SRAM0_RESET  _u(0x0)
#define PSM_WDSEL_SRAM0_BITS   _u(0x00000400)
#define PSM_WDSEL_SRAM0_MSB    _u(10)
#define PSM_WDSEL_SRAM0_LSB    _u(10)
#define PSM_WDSEL_SRAM0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_WDSEL_BOOTRAM
#define PSM_WDSEL_BOOTRAM_RESET  _u(0x0)
#define PSM_WDSEL_BOOTRAM_BITS   _u(0x00000200)
#define PSM_WDSEL_BOOTRAM_MSB    _u(9)
#define PSM_WDSEL_BOOTRAM_LSB    _u(9)
#define PSM_WDSEL_BOOTRAM_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_WDSEL_ROM
#define PSM_WDSEL_ROM_RESET  _u(0x0)
#define PSM_WDSEL_ROM_BITS   _u(0x00000100)
#define PSM_WDSEL_ROM_MSB    _u(8)
#define PSM_WDSEL_ROM_LSB    _u(8)
#define PSM_WDSEL_ROM_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_WDSEL_BUSFABRIC
#define PSM_WDSEL_BUSFABRIC_RESET  _u(0x0)
#define PSM_WDSEL_BUSFABRIC_BITS   _u(0x00000080)
#define PSM_WDSEL_BUSFABRIC_MSB    _u(7)
#define PSM_WDSEL_BUSFABRIC_LSB    _u(7)
#define PSM_WDSEL_BUSFABRIC_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_WDSEL_PSM_READY
#define PSM_WDSEL_PSM_READY_RESET  _u(0x0)
#define PSM_WDSEL_PSM_READY_BITS   _u(0x00000040)
#define PSM_WDSEL_PSM_READY_MSB    _u(6)
#define PSM_WDSEL_PSM_READY_LSB    _u(6)
#define PSM_WDSEL_PSM_READY_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_WDSEL_CLOCKS
#define PSM_WDSEL_CLOCKS_RESET  _u(0x0)
#define PSM_WDSEL_CLOCKS_BITS   _u(0x00000020)
#define PSM_WDSEL_CLOCKS_MSB    _u(5)
#define PSM_WDSEL_CLOCKS_LSB    _u(5)
#define PSM_WDSEL_CLOCKS_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_WDSEL_RESETS
#define PSM_WDSEL_RESETS_RESET  _u(0x0)
#define PSM_WDSEL_RESETS_BITS   _u(0x00000010)
#define PSM_WDSEL_RESETS_MSB    _u(4)
#define PSM_WDSEL_RESETS_LSB    _u(4)
#define PSM_WDSEL_RESETS_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_WDSEL_XOSC
#define PSM_WDSEL_XOSC_RESET  _u(0x0)
#define PSM_WDSEL_XOSC_BITS   _u(0x00000008)
#define PSM_WDSEL_XOSC_MSB    _u(3)
#define PSM_WDSEL_XOSC_LSB    _u(3)
#define PSM_WDSEL_XOSC_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_WDSEL_ROSC
#define PSM_WDSEL_ROSC_RESET  _u(0x0)
#define PSM_WDSEL_ROSC_BITS   _u(0x00000004)
#define PSM_WDSEL_ROSC_MSB    _u(2)
#define PSM_WDSEL_ROSC_LSB    _u(2)
#define PSM_WDSEL_ROSC_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_WDSEL_OTP
#define PSM_WDSEL_OTP_RESET  _u(0x0)
#define PSM_WDSEL_OTP_BITS   _u(0x00000002)
#define PSM_WDSEL_OTP_MSB    _u(1)
#define PSM_WDSEL_OTP_LSB    _u(1)
#define PSM_WDSEL_OTP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PSM_WDSEL_PROC_COLD
#define PSM_WDSEL_PROC_COLD_RESET  _u(0x0)
#define PSM_WDSEL_PROC_COLD_BITS   _u(0x00000001)
#define PSM_WDSEL_PROC_COLD_MSB    _u(0)
#define PSM_WDSEL_PROC_COLD_LSB    _u(0)
#define PSM_WDSEL_PROC_COLD_ACCESS "RW"
// =============================================================================
// Register    : PSM_DONE
// Description : Is the subsystem ready?
#define PSM_DONE_OFFSET _u(0x0000000c)
#define PSM_DONE_BITS   _u(0x01ffffff)
#define PSM_DONE_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : PSM_DONE_PROC1
#define PSM_DONE_PROC1_RESET  _u(0x0)
#define PSM_DONE_PROC1_BITS   _u(0x01000000)
#define PSM_DONE_PROC1_MSB    _u(24)
#define PSM_DONE_PROC1_LSB    _u(24)
#define PSM_DONE_PROC1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : PSM_DONE_PROC0
#define PSM_DONE_PROC0_RESET  _u(0x0)
#define PSM_DONE_PROC0_BITS   _u(0x00800000)
#define PSM_DONE_PROC0_MSB    _u(23)
#define PSM_DONE_PROC0_LSB    _u(23)
#define PSM_DONE_PROC0_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : PSM_DONE_ACCESSCTRL
#define PSM_DONE_ACCESSCTRL_RESET  _u(0x0)
#define PSM_DONE_ACCESSCTRL_BITS   _u(0x00400000)
#define PSM_DONE_ACCESSCTRL_MSB    _u(22)
#define PSM_DONE_ACCESSCTRL_LSB    _u(22)
#define PSM_DONE_ACCESSCTRL_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : PSM_DONE_SIO
#define PSM_DONE_SIO_RESET  _u(0x0)
#define PSM_DONE_SIO_BITS   _u(0x00200000)
#define PSM_DONE_SIO_MSB    _u(21)
#define PSM_DONE_SIO_LSB    _u(21)
#define PSM_DONE_SIO_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : PSM_DONE_XIP
#define PSM_DONE_XIP_RESET  _u(0x0)
#define PSM_DONE_XIP_BITS   _u(0x00100000)
#define PSM_DONE_XIP_MSB    _u(20)
#define PSM_DONE_XIP_LSB    _u(20)
#define PSM_DONE_XIP_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : PSM_DONE_SRAM9
#define PSM_DONE_SRAM9_RESET  _u(0x0)
#define PSM_DONE_SRAM9_BITS   _u(0x00080000)
#define PSM_DONE_SRAM9_MSB    _u(19)
#define PSM_DONE_SRAM9_LSB    _u(19)
#define PSM_DONE_SRAM9_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : PSM_DONE_SRAM8
#define PSM_DONE_SRAM8_RESET  _u(0x0)
#define PSM_DONE_SRAM8_BITS   _u(0x00040000)
#define PSM_DONE_SRAM8_MSB    _u(18)
#define PSM_DONE_SRAM8_LSB    _u(18)
#define PSM_DONE_SRAM8_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : PSM_DONE_SRAM7
#define PSM_DONE_SRAM7_RESET  _u(0x0)
#define PSM_DONE_SRAM7_BITS   _u(0x00020000)
#define PSM_DONE_SRAM7_MSB    _u(17)
#define PSM_DONE_SRAM7_LSB    _u(17)
#define PSM_DONE_SRAM7_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : PSM_DONE_SRAM6
#define PSM_DONE_SRAM6_RESET  _u(0x0)
#define PSM_DONE_SRAM6_BITS   _u(0x00010000)
#define PSM_DONE_SRAM6_MSB    _u(16)
#define PSM_DONE_SRAM6_LSB    _u(16)
#define PSM_DONE_SRAM6_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : PSM_DONE_SRAM5
#define PSM_DONE_SRAM5_RESET  _u(0x0)
#define PSM_DONE_SRAM5_BITS   _u(0x00008000)
#define PSM_DONE_SRAM5_MSB    _u(15)
#define PSM_DONE_SRAM5_LSB    _u(15)
#define PSM_DONE_SRAM5_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : PSM_DONE_SRAM4
#define PSM_DONE_SRAM4_RESET  _u(0x0)
#define PSM_DONE_SRAM4_BITS   _u(0x00004000)
#define PSM_DONE_SRAM4_MSB    _u(14)
#define PSM_DONE_SRAM4_LSB    _u(14)
#define PSM_DONE_SRAM4_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : PSM_DONE_SRAM3
#define PSM_DONE_SRAM3_RESET  _u(0x0)
#define PSM_DONE_SRAM3_BITS   _u(0x00002000)
#define PSM_DONE_SRAM3_MSB    _u(13)
#define PSM_DONE_SRAM3_LSB    _u(13)
#define PSM_DONE_SRAM3_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : PSM_DONE_SRAM2
#define PSM_DONE_SRAM2_RESET  _u(0x0)
#define PSM_DONE_SRAM2_BITS   _u(0x00001000)
#define PSM_DONE_SRAM2_MSB    _u(12)
#define PSM_DONE_SRAM2_LSB    _u(12)
#define PSM_DONE_SRAM2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : PSM_DONE_SRAM1
#define PSM_DONE_SRAM1_RESET  _u(0x0)
#define PSM_DONE_SRAM1_BITS   _u(0x00000800)
#define PSM_DONE_SRAM1_MSB    _u(11)
#define PSM_DONE_SRAM1_LSB    _u(11)
#define PSM_DONE_SRAM1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : PSM_DONE_SRAM0
#define PSM_DONE_SRAM0_RESET  _u(0x0)
#define PSM_DONE_SRAM0_BITS   _u(0x00000400)
#define PSM_DONE_SRAM0_MSB    _u(10)
#define PSM_DONE_SRAM0_LSB    _u(10)
#define PSM_DONE_SRAM0_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : PSM_DONE_BOOTRAM
#define PSM_DONE_BOOTRAM_RESET  _u(0x0)
#define PSM_DONE_BOOTRAM_BITS   _u(0x00000200)
#define PSM_DONE_BOOTRAM_MSB    _u(9)
#define PSM_DONE_BOOTRAM_LSB    _u(9)
#define PSM_DONE_BOOTRAM_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : PSM_DONE_ROM
#define PSM_DONE_ROM_RESET  _u(0x0)
#define PSM_DONE_ROM_BITS   _u(0x00000100)
#define PSM_DONE_ROM_MSB    _u(8)
#define PSM_DONE_ROM_LSB    _u(8)
#define PSM_DONE_ROM_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : PSM_DONE_BUSFABRIC
#define PSM_DONE_BUSFABRIC_RESET  _u(0x0)
#define PSM_DONE_BUSFABRIC_BITS   _u(0x00000080)
#define PSM_DONE_BUSFABRIC_MSB    _u(7)
#define PSM_DONE_BUSFABRIC_LSB    _u(7)
#define PSM_DONE_BUSFABRIC_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : PSM_DONE_PSM_READY
#define PSM_DONE_PSM_READY_RESET  _u(0x0)
#define PSM_DONE_PSM_READY_BITS   _u(0x00000040)
#define PSM_DONE_PSM_READY_MSB    _u(6)
#define PSM_DONE_PSM_READY_LSB    _u(6)
#define PSM_DONE_PSM_READY_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : PSM_DONE_CLOCKS
#define PSM_DONE_CLOCKS_RESET  _u(0x0)
#define PSM_DONE_CLOCKS_BITS   _u(0x00000020)
#define PSM_DONE_CLOCKS_MSB    _u(5)
#define PSM_DONE_CLOCKS_LSB    _u(5)
#define PSM_DONE_CLOCKS_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : PSM_DONE_RESETS
#define PSM_DONE_RESETS_RESET  _u(0x0)
#define PSM_DONE_RESETS_BITS   _u(0x00000010)
#define PSM_DONE_RESETS_MSB    _u(4)
#define PSM_DONE_RESETS_LSB    _u(4)
#define PSM_DONE_RESETS_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : PSM_DONE_XOSC
#define PSM_DONE_XOSC_RESET  _u(0x0)
#define PSM_DONE_XOSC_BITS   _u(0x00000008)
#define PSM_DONE_XOSC_MSB    _u(3)
#define PSM_DONE_XOSC_LSB    _u(3)
#define PSM_DONE_XOSC_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : PSM_DONE_ROSC
#define PSM_DONE_ROSC_RESET  _u(0x0)
#define PSM_DONE_ROSC_BITS   _u(0x00000004)
#define PSM_DONE_ROSC_MSB    _u(2)
#define PSM_DONE_ROSC_LSB    _u(2)
#define PSM_DONE_ROSC_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : PSM_DONE_OTP
#define PSM_DONE_OTP_RESET  _u(0x0)
#define PSM_DONE_OTP_BITS   _u(0x00000002)
#define PSM_DONE_OTP_MSB    _u(1)
#define PSM_DONE_OTP_LSB    _u(1)
#define PSM_DONE_OTP_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : PSM_DONE_PROC_COLD
#define PSM_DONE_PROC_COLD_RESET  _u(0x0)
#define PSM_DONE_PROC_COLD_BITS   _u(0x00000001)
#define PSM_DONE_PROC_COLD_MSB    _u(0)
#define PSM_DONE_PROC_COLD_LSB    _u(0)
#define PSM_DONE_PROC_COLD_ACCESS "RO"
// =============================================================================
#endif // _HARDWARE_REGS_PSM_H

