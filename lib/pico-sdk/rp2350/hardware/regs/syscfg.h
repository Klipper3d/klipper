// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
// =============================================================================
// Register block : SYSCFG
// Version        : 1
// Bus type       : apb
// Description    : Register block for various chip control signals
// =============================================================================
#ifndef _HARDWARE_REGS_SYSCFG_H
#define _HARDWARE_REGS_SYSCFG_H
// =============================================================================
// Register    : SYSCFG_PROC_CONFIG
// Description : Configuration for processors
#define SYSCFG_PROC_CONFIG_OFFSET _u(0x00000000)
#define SYSCFG_PROC_CONFIG_BITS   _u(0x00000003)
#define SYSCFG_PROC_CONFIG_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : SYSCFG_PROC_CONFIG_PROC1_HALTED
// Description : Indication that proc1 has halted
#define SYSCFG_PROC_CONFIG_PROC1_HALTED_RESET  _u(0x0)
#define SYSCFG_PROC_CONFIG_PROC1_HALTED_BITS   _u(0x00000002)
#define SYSCFG_PROC_CONFIG_PROC1_HALTED_MSB    _u(1)
#define SYSCFG_PROC_CONFIG_PROC1_HALTED_LSB    _u(1)
#define SYSCFG_PROC_CONFIG_PROC1_HALTED_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : SYSCFG_PROC_CONFIG_PROC0_HALTED
// Description : Indication that proc0 has halted
#define SYSCFG_PROC_CONFIG_PROC0_HALTED_RESET  _u(0x0)
#define SYSCFG_PROC_CONFIG_PROC0_HALTED_BITS   _u(0x00000001)
#define SYSCFG_PROC_CONFIG_PROC0_HALTED_MSB    _u(0)
#define SYSCFG_PROC_CONFIG_PROC0_HALTED_LSB    _u(0)
#define SYSCFG_PROC_CONFIG_PROC0_HALTED_ACCESS "RO"
// =============================================================================
// Register    : SYSCFG_PROC_IN_SYNC_BYPASS
// Description : For each bit, if 1, bypass the input synchronizer between that
//               GPIO
//               and the GPIO input register in the SIO. The input synchronizers
//               should
//               generally be unbypassed, to avoid injecting metastabilities
//               into processors.
//               If you're feeling brave, you can bypass to save two cycles of
//               input
//               latency. This register applies to GPIO 0...31.
#define SYSCFG_PROC_IN_SYNC_BYPASS_OFFSET _u(0x00000004)
#define SYSCFG_PROC_IN_SYNC_BYPASS_BITS   _u(0xffffffff)
#define SYSCFG_PROC_IN_SYNC_BYPASS_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : SYSCFG_PROC_IN_SYNC_BYPASS_GPIO
#define SYSCFG_PROC_IN_SYNC_BYPASS_GPIO_RESET  _u(0x00000000)
#define SYSCFG_PROC_IN_SYNC_BYPASS_GPIO_BITS   _u(0xffffffff)
#define SYSCFG_PROC_IN_SYNC_BYPASS_GPIO_MSB    _u(31)
#define SYSCFG_PROC_IN_SYNC_BYPASS_GPIO_LSB    _u(0)
#define SYSCFG_PROC_IN_SYNC_BYPASS_GPIO_ACCESS "RW"
// =============================================================================
// Register    : SYSCFG_PROC_IN_SYNC_BYPASS_HI
// Description : For each bit, if 1, bypass the input synchronizer between that
//               GPIO
//               and the GPIO input register in the SIO. The input synchronizers
//               should
//               generally be unbypassed, to avoid injecting metastabilities
//               into processors.
//               If you're feeling brave, you can bypass to save two cycles of
//               input
//               latency. This register applies to GPIO 32...47. USB GPIO 56..57
//               QSPI GPIO 58..63
#define SYSCFG_PROC_IN_SYNC_BYPASS_HI_OFFSET _u(0x00000008)
#define SYSCFG_PROC_IN_SYNC_BYPASS_HI_BITS   _u(0xff00ffff)
#define SYSCFG_PROC_IN_SYNC_BYPASS_HI_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : SYSCFG_PROC_IN_SYNC_BYPASS_HI_QSPI_SD
#define SYSCFG_PROC_IN_SYNC_BYPASS_HI_QSPI_SD_RESET  _u(0x0)
#define SYSCFG_PROC_IN_SYNC_BYPASS_HI_QSPI_SD_BITS   _u(0xf0000000)
#define SYSCFG_PROC_IN_SYNC_BYPASS_HI_QSPI_SD_MSB    _u(31)
#define SYSCFG_PROC_IN_SYNC_BYPASS_HI_QSPI_SD_LSB    _u(28)
#define SYSCFG_PROC_IN_SYNC_BYPASS_HI_QSPI_SD_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SYSCFG_PROC_IN_SYNC_BYPASS_HI_QSPI_CSN
#define SYSCFG_PROC_IN_SYNC_BYPASS_HI_QSPI_CSN_RESET  _u(0x0)
#define SYSCFG_PROC_IN_SYNC_BYPASS_HI_QSPI_CSN_BITS   _u(0x08000000)
#define SYSCFG_PROC_IN_SYNC_BYPASS_HI_QSPI_CSN_MSB    _u(27)
#define SYSCFG_PROC_IN_SYNC_BYPASS_HI_QSPI_CSN_LSB    _u(27)
#define SYSCFG_PROC_IN_SYNC_BYPASS_HI_QSPI_CSN_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SYSCFG_PROC_IN_SYNC_BYPASS_HI_QSPI_SCK
#define SYSCFG_PROC_IN_SYNC_BYPASS_HI_QSPI_SCK_RESET  _u(0x0)
#define SYSCFG_PROC_IN_SYNC_BYPASS_HI_QSPI_SCK_BITS   _u(0x04000000)
#define SYSCFG_PROC_IN_SYNC_BYPASS_HI_QSPI_SCK_MSB    _u(26)
#define SYSCFG_PROC_IN_SYNC_BYPASS_HI_QSPI_SCK_LSB    _u(26)
#define SYSCFG_PROC_IN_SYNC_BYPASS_HI_QSPI_SCK_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SYSCFG_PROC_IN_SYNC_BYPASS_HI_USB_DM
#define SYSCFG_PROC_IN_SYNC_BYPASS_HI_USB_DM_RESET  _u(0x0)
#define SYSCFG_PROC_IN_SYNC_BYPASS_HI_USB_DM_BITS   _u(0x02000000)
#define SYSCFG_PROC_IN_SYNC_BYPASS_HI_USB_DM_MSB    _u(25)
#define SYSCFG_PROC_IN_SYNC_BYPASS_HI_USB_DM_LSB    _u(25)
#define SYSCFG_PROC_IN_SYNC_BYPASS_HI_USB_DM_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SYSCFG_PROC_IN_SYNC_BYPASS_HI_USB_DP
#define SYSCFG_PROC_IN_SYNC_BYPASS_HI_USB_DP_RESET  _u(0x0)
#define SYSCFG_PROC_IN_SYNC_BYPASS_HI_USB_DP_BITS   _u(0x01000000)
#define SYSCFG_PROC_IN_SYNC_BYPASS_HI_USB_DP_MSB    _u(24)
#define SYSCFG_PROC_IN_SYNC_BYPASS_HI_USB_DP_LSB    _u(24)
#define SYSCFG_PROC_IN_SYNC_BYPASS_HI_USB_DP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SYSCFG_PROC_IN_SYNC_BYPASS_HI_GPIO
#define SYSCFG_PROC_IN_SYNC_BYPASS_HI_GPIO_RESET  _u(0x0000)
#define SYSCFG_PROC_IN_SYNC_BYPASS_HI_GPIO_BITS   _u(0x0000ffff)
#define SYSCFG_PROC_IN_SYNC_BYPASS_HI_GPIO_MSB    _u(15)
#define SYSCFG_PROC_IN_SYNC_BYPASS_HI_GPIO_LSB    _u(0)
#define SYSCFG_PROC_IN_SYNC_BYPASS_HI_GPIO_ACCESS "RW"
// =============================================================================
// Register    : SYSCFG_DBGFORCE
// Description : Directly control the chip SWD debug port
#define SYSCFG_DBGFORCE_OFFSET _u(0x0000000c)
#define SYSCFG_DBGFORCE_BITS   _u(0x0000000f)
#define SYSCFG_DBGFORCE_RESET  _u(0x00000006)
// -----------------------------------------------------------------------------
// Field       : SYSCFG_DBGFORCE_ATTACH
// Description : Attach chip debug port to syscfg controls, and disconnect it
//               from external SWD pads.
#define SYSCFG_DBGFORCE_ATTACH_RESET  _u(0x0)
#define SYSCFG_DBGFORCE_ATTACH_BITS   _u(0x00000008)
#define SYSCFG_DBGFORCE_ATTACH_MSB    _u(3)
#define SYSCFG_DBGFORCE_ATTACH_LSB    _u(3)
#define SYSCFG_DBGFORCE_ATTACH_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SYSCFG_DBGFORCE_SWCLK
// Description : Directly drive SWCLK, if ATTACH is set
#define SYSCFG_DBGFORCE_SWCLK_RESET  _u(0x1)
#define SYSCFG_DBGFORCE_SWCLK_BITS   _u(0x00000004)
#define SYSCFG_DBGFORCE_SWCLK_MSB    _u(2)
#define SYSCFG_DBGFORCE_SWCLK_LSB    _u(2)
#define SYSCFG_DBGFORCE_SWCLK_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SYSCFG_DBGFORCE_SWDI
// Description : Directly drive SWDIO input, if ATTACH is set
#define SYSCFG_DBGFORCE_SWDI_RESET  _u(0x1)
#define SYSCFG_DBGFORCE_SWDI_BITS   _u(0x00000002)
#define SYSCFG_DBGFORCE_SWDI_MSB    _u(1)
#define SYSCFG_DBGFORCE_SWDI_LSB    _u(1)
#define SYSCFG_DBGFORCE_SWDI_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SYSCFG_DBGFORCE_SWDO
// Description : Observe the value of SWDIO output.
#define SYSCFG_DBGFORCE_SWDO_RESET  "-"
#define SYSCFG_DBGFORCE_SWDO_BITS   _u(0x00000001)
#define SYSCFG_DBGFORCE_SWDO_MSB    _u(0)
#define SYSCFG_DBGFORCE_SWDO_LSB    _u(0)
#define SYSCFG_DBGFORCE_SWDO_ACCESS "RO"
// =============================================================================
// Register    : SYSCFG_MEMPOWERDOWN
// Description : Control PD pins to memories.
//               Set high to put memories to a low power state. In this state
//               the memories will retain contents but not be accessible
//               Use with caution
#define SYSCFG_MEMPOWERDOWN_OFFSET _u(0x00000010)
#define SYSCFG_MEMPOWERDOWN_BITS   _u(0x00001fff)
#define SYSCFG_MEMPOWERDOWN_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : SYSCFG_MEMPOWERDOWN_BOOTRAM
#define SYSCFG_MEMPOWERDOWN_BOOTRAM_RESET  _u(0x0)
#define SYSCFG_MEMPOWERDOWN_BOOTRAM_BITS   _u(0x00001000)
#define SYSCFG_MEMPOWERDOWN_BOOTRAM_MSB    _u(12)
#define SYSCFG_MEMPOWERDOWN_BOOTRAM_LSB    _u(12)
#define SYSCFG_MEMPOWERDOWN_BOOTRAM_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SYSCFG_MEMPOWERDOWN_ROM
#define SYSCFG_MEMPOWERDOWN_ROM_RESET  _u(0x0)
#define SYSCFG_MEMPOWERDOWN_ROM_BITS   _u(0x00000800)
#define SYSCFG_MEMPOWERDOWN_ROM_MSB    _u(11)
#define SYSCFG_MEMPOWERDOWN_ROM_LSB    _u(11)
#define SYSCFG_MEMPOWERDOWN_ROM_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SYSCFG_MEMPOWERDOWN_USB
#define SYSCFG_MEMPOWERDOWN_USB_RESET  _u(0x0)
#define SYSCFG_MEMPOWERDOWN_USB_BITS   _u(0x00000400)
#define SYSCFG_MEMPOWERDOWN_USB_MSB    _u(10)
#define SYSCFG_MEMPOWERDOWN_USB_LSB    _u(10)
#define SYSCFG_MEMPOWERDOWN_USB_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SYSCFG_MEMPOWERDOWN_SRAM9
#define SYSCFG_MEMPOWERDOWN_SRAM9_RESET  _u(0x0)
#define SYSCFG_MEMPOWERDOWN_SRAM9_BITS   _u(0x00000200)
#define SYSCFG_MEMPOWERDOWN_SRAM9_MSB    _u(9)
#define SYSCFG_MEMPOWERDOWN_SRAM9_LSB    _u(9)
#define SYSCFG_MEMPOWERDOWN_SRAM9_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SYSCFG_MEMPOWERDOWN_SRAM8
#define SYSCFG_MEMPOWERDOWN_SRAM8_RESET  _u(0x0)
#define SYSCFG_MEMPOWERDOWN_SRAM8_BITS   _u(0x00000100)
#define SYSCFG_MEMPOWERDOWN_SRAM8_MSB    _u(8)
#define SYSCFG_MEMPOWERDOWN_SRAM8_LSB    _u(8)
#define SYSCFG_MEMPOWERDOWN_SRAM8_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SYSCFG_MEMPOWERDOWN_SRAM7
#define SYSCFG_MEMPOWERDOWN_SRAM7_RESET  _u(0x0)
#define SYSCFG_MEMPOWERDOWN_SRAM7_BITS   _u(0x00000080)
#define SYSCFG_MEMPOWERDOWN_SRAM7_MSB    _u(7)
#define SYSCFG_MEMPOWERDOWN_SRAM7_LSB    _u(7)
#define SYSCFG_MEMPOWERDOWN_SRAM7_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SYSCFG_MEMPOWERDOWN_SRAM6
#define SYSCFG_MEMPOWERDOWN_SRAM6_RESET  _u(0x0)
#define SYSCFG_MEMPOWERDOWN_SRAM6_BITS   _u(0x00000040)
#define SYSCFG_MEMPOWERDOWN_SRAM6_MSB    _u(6)
#define SYSCFG_MEMPOWERDOWN_SRAM6_LSB    _u(6)
#define SYSCFG_MEMPOWERDOWN_SRAM6_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SYSCFG_MEMPOWERDOWN_SRAM5
#define SYSCFG_MEMPOWERDOWN_SRAM5_RESET  _u(0x0)
#define SYSCFG_MEMPOWERDOWN_SRAM5_BITS   _u(0x00000020)
#define SYSCFG_MEMPOWERDOWN_SRAM5_MSB    _u(5)
#define SYSCFG_MEMPOWERDOWN_SRAM5_LSB    _u(5)
#define SYSCFG_MEMPOWERDOWN_SRAM5_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SYSCFG_MEMPOWERDOWN_SRAM4
#define SYSCFG_MEMPOWERDOWN_SRAM4_RESET  _u(0x0)
#define SYSCFG_MEMPOWERDOWN_SRAM4_BITS   _u(0x00000010)
#define SYSCFG_MEMPOWERDOWN_SRAM4_MSB    _u(4)
#define SYSCFG_MEMPOWERDOWN_SRAM4_LSB    _u(4)
#define SYSCFG_MEMPOWERDOWN_SRAM4_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SYSCFG_MEMPOWERDOWN_SRAM3
#define SYSCFG_MEMPOWERDOWN_SRAM3_RESET  _u(0x0)
#define SYSCFG_MEMPOWERDOWN_SRAM3_BITS   _u(0x00000008)
#define SYSCFG_MEMPOWERDOWN_SRAM3_MSB    _u(3)
#define SYSCFG_MEMPOWERDOWN_SRAM3_LSB    _u(3)
#define SYSCFG_MEMPOWERDOWN_SRAM3_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SYSCFG_MEMPOWERDOWN_SRAM2
#define SYSCFG_MEMPOWERDOWN_SRAM2_RESET  _u(0x0)
#define SYSCFG_MEMPOWERDOWN_SRAM2_BITS   _u(0x00000004)
#define SYSCFG_MEMPOWERDOWN_SRAM2_MSB    _u(2)
#define SYSCFG_MEMPOWERDOWN_SRAM2_LSB    _u(2)
#define SYSCFG_MEMPOWERDOWN_SRAM2_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SYSCFG_MEMPOWERDOWN_SRAM1
#define SYSCFG_MEMPOWERDOWN_SRAM1_RESET  _u(0x0)
#define SYSCFG_MEMPOWERDOWN_SRAM1_BITS   _u(0x00000002)
#define SYSCFG_MEMPOWERDOWN_SRAM1_MSB    _u(1)
#define SYSCFG_MEMPOWERDOWN_SRAM1_LSB    _u(1)
#define SYSCFG_MEMPOWERDOWN_SRAM1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SYSCFG_MEMPOWERDOWN_SRAM0
#define SYSCFG_MEMPOWERDOWN_SRAM0_RESET  _u(0x0)
#define SYSCFG_MEMPOWERDOWN_SRAM0_BITS   _u(0x00000001)
#define SYSCFG_MEMPOWERDOWN_SRAM0_MSB    _u(0)
#define SYSCFG_MEMPOWERDOWN_SRAM0_LSB    _u(0)
#define SYSCFG_MEMPOWERDOWN_SRAM0_ACCESS "RW"
// =============================================================================
// Register    : SYSCFG_AUXCTRL
// Description : Auxiliary system control register
//               * Bits 7:2: Reserved
//
//               * Bit 1: When clear, the LPOSC output is XORed into the TRNG
//               ROSC output as an additional, uncorrelated entropy source. When
//               set, this behaviour is disabled.
//
//               * Bit 0: Force POWMAN clock to switch to LPOSC, by asserting
//               its WDRESET input. This must be set before initiating a
//               watchdog reset of the RSM from a stage that includes CLOCKS, if
//               POWMAN is running from clk_ref at the point that the watchdog
//               reset takes place. Otherwise, the short pulse generated on
//               clk_ref by the reset of the CLOCKS block may affect POWMAN
//               register state.
#define SYSCFG_AUXCTRL_OFFSET _u(0x00000014)
#define SYSCFG_AUXCTRL_BITS   _u(0x000000ff)
#define SYSCFG_AUXCTRL_RESET  _u(0x00000000)
#define SYSCFG_AUXCTRL_MSB    _u(7)
#define SYSCFG_AUXCTRL_LSB    _u(0)
#define SYSCFG_AUXCTRL_ACCESS "RW"
// =============================================================================
#endif // _HARDWARE_REGS_SYSCFG_H

