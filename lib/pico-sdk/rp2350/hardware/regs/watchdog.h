// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
// =============================================================================
// Register block : WATCHDOG
// Version        : 1
// Bus type       : apb
// =============================================================================
#ifndef _HARDWARE_REGS_WATCHDOG_H
#define _HARDWARE_REGS_WATCHDOG_H
// =============================================================================
// Register    : WATCHDOG_CTRL
// Description : Watchdog control
//               The rst_wdsel register determines which subsystems are reset
//               when the watchdog is triggered.
//               The watchdog can be triggered in software.
#define WATCHDOG_CTRL_OFFSET _u(0x00000000)
#define WATCHDOG_CTRL_BITS   _u(0xc7ffffff)
#define WATCHDOG_CTRL_RESET  _u(0x07000000)
// -----------------------------------------------------------------------------
// Field       : WATCHDOG_CTRL_TRIGGER
// Description : Trigger a watchdog reset
#define WATCHDOG_CTRL_TRIGGER_RESET  _u(0x0)
#define WATCHDOG_CTRL_TRIGGER_BITS   _u(0x80000000)
#define WATCHDOG_CTRL_TRIGGER_MSB    _u(31)
#define WATCHDOG_CTRL_TRIGGER_LSB    _u(31)
#define WATCHDOG_CTRL_TRIGGER_ACCESS "SC"
// -----------------------------------------------------------------------------
// Field       : WATCHDOG_CTRL_ENABLE
// Description : When not enabled the watchdog timer is paused
#define WATCHDOG_CTRL_ENABLE_RESET  _u(0x0)
#define WATCHDOG_CTRL_ENABLE_BITS   _u(0x40000000)
#define WATCHDOG_CTRL_ENABLE_MSB    _u(30)
#define WATCHDOG_CTRL_ENABLE_LSB    _u(30)
#define WATCHDOG_CTRL_ENABLE_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : WATCHDOG_CTRL_PAUSE_DBG1
// Description : Pause the watchdog timer when processor 1 is in debug mode
#define WATCHDOG_CTRL_PAUSE_DBG1_RESET  _u(0x1)
#define WATCHDOG_CTRL_PAUSE_DBG1_BITS   _u(0x04000000)
#define WATCHDOG_CTRL_PAUSE_DBG1_MSB    _u(26)
#define WATCHDOG_CTRL_PAUSE_DBG1_LSB    _u(26)
#define WATCHDOG_CTRL_PAUSE_DBG1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : WATCHDOG_CTRL_PAUSE_DBG0
// Description : Pause the watchdog timer when processor 0 is in debug mode
#define WATCHDOG_CTRL_PAUSE_DBG0_RESET  _u(0x1)
#define WATCHDOG_CTRL_PAUSE_DBG0_BITS   _u(0x02000000)
#define WATCHDOG_CTRL_PAUSE_DBG0_MSB    _u(25)
#define WATCHDOG_CTRL_PAUSE_DBG0_LSB    _u(25)
#define WATCHDOG_CTRL_PAUSE_DBG0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : WATCHDOG_CTRL_PAUSE_JTAG
// Description : Pause the watchdog timer when JTAG is accessing the bus fabric
#define WATCHDOG_CTRL_PAUSE_JTAG_RESET  _u(0x1)
#define WATCHDOG_CTRL_PAUSE_JTAG_BITS   _u(0x01000000)
#define WATCHDOG_CTRL_PAUSE_JTAG_MSB    _u(24)
#define WATCHDOG_CTRL_PAUSE_JTAG_LSB    _u(24)
#define WATCHDOG_CTRL_PAUSE_JTAG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : WATCHDOG_CTRL_TIME
// Description : Indicates the time in usec before a watchdog reset will be
//               triggered
#define WATCHDOG_CTRL_TIME_RESET  _u(0x000000)
#define WATCHDOG_CTRL_TIME_BITS   _u(0x00ffffff)
#define WATCHDOG_CTRL_TIME_MSB    _u(23)
#define WATCHDOG_CTRL_TIME_LSB    _u(0)
#define WATCHDOG_CTRL_TIME_ACCESS "RO"
// =============================================================================
// Register    : WATCHDOG_LOAD
// Description : Load the watchdog timer. The maximum setting is 0xffffff which
//               corresponds to approximately 16 seconds.
#define WATCHDOG_LOAD_OFFSET _u(0x00000004)
#define WATCHDOG_LOAD_BITS   _u(0x00ffffff)
#define WATCHDOG_LOAD_RESET  _u(0x00000000)
#define WATCHDOG_LOAD_MSB    _u(23)
#define WATCHDOG_LOAD_LSB    _u(0)
#define WATCHDOG_LOAD_ACCESS "WF"
// =============================================================================
// Register    : WATCHDOG_REASON
// Description : Logs the reason for the last reset. Both bits are zero for the
//               case of a hardware reset.
//
//               Additionally, as of RP2350, a debugger warm reset of either
//               core (SYSRESETREQ or hartreset) will also clear the watchdog
//               reason register, so that software loaded under the debugger
//               following a watchdog timeout will not continue to see the
//               timeout condition.
#define WATCHDOG_REASON_OFFSET _u(0x00000008)
#define WATCHDOG_REASON_BITS   _u(0x00000003)
#define WATCHDOG_REASON_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : WATCHDOG_REASON_FORCE
#define WATCHDOG_REASON_FORCE_RESET  _u(0x0)
#define WATCHDOG_REASON_FORCE_BITS   _u(0x00000002)
#define WATCHDOG_REASON_FORCE_MSB    _u(1)
#define WATCHDOG_REASON_FORCE_LSB    _u(1)
#define WATCHDOG_REASON_FORCE_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : WATCHDOG_REASON_TIMER
#define WATCHDOG_REASON_TIMER_RESET  _u(0x0)
#define WATCHDOG_REASON_TIMER_BITS   _u(0x00000001)
#define WATCHDOG_REASON_TIMER_MSB    _u(0)
#define WATCHDOG_REASON_TIMER_LSB    _u(0)
#define WATCHDOG_REASON_TIMER_ACCESS "RO"
// =============================================================================
// Register    : WATCHDOG_SCRATCH0
// Description : Scratch register. Information persists through soft reset of
//               the chip.
#define WATCHDOG_SCRATCH0_OFFSET _u(0x0000000c)
#define WATCHDOG_SCRATCH0_BITS   _u(0xffffffff)
#define WATCHDOG_SCRATCH0_RESET  _u(0x00000000)
#define WATCHDOG_SCRATCH0_MSB    _u(31)
#define WATCHDOG_SCRATCH0_LSB    _u(0)
#define WATCHDOG_SCRATCH0_ACCESS "RW"
// =============================================================================
// Register    : WATCHDOG_SCRATCH1
// Description : Scratch register. Information persists through soft reset of
//               the chip.
#define WATCHDOG_SCRATCH1_OFFSET _u(0x00000010)
#define WATCHDOG_SCRATCH1_BITS   _u(0xffffffff)
#define WATCHDOG_SCRATCH1_RESET  _u(0x00000000)
#define WATCHDOG_SCRATCH1_MSB    _u(31)
#define WATCHDOG_SCRATCH1_LSB    _u(0)
#define WATCHDOG_SCRATCH1_ACCESS "RW"
// =============================================================================
// Register    : WATCHDOG_SCRATCH2
// Description : Scratch register. Information persists through soft reset of
//               the chip.
#define WATCHDOG_SCRATCH2_OFFSET _u(0x00000014)
#define WATCHDOG_SCRATCH2_BITS   _u(0xffffffff)
#define WATCHDOG_SCRATCH2_RESET  _u(0x00000000)
#define WATCHDOG_SCRATCH2_MSB    _u(31)
#define WATCHDOG_SCRATCH2_LSB    _u(0)
#define WATCHDOG_SCRATCH2_ACCESS "RW"
// =============================================================================
// Register    : WATCHDOG_SCRATCH3
// Description : Scratch register. Information persists through soft reset of
//               the chip.
#define WATCHDOG_SCRATCH3_OFFSET _u(0x00000018)
#define WATCHDOG_SCRATCH3_BITS   _u(0xffffffff)
#define WATCHDOG_SCRATCH3_RESET  _u(0x00000000)
#define WATCHDOG_SCRATCH3_MSB    _u(31)
#define WATCHDOG_SCRATCH3_LSB    _u(0)
#define WATCHDOG_SCRATCH3_ACCESS "RW"
// =============================================================================
// Register    : WATCHDOG_SCRATCH4
// Description : Scratch register. Information persists through soft reset of
//               the chip.
#define WATCHDOG_SCRATCH4_OFFSET _u(0x0000001c)
#define WATCHDOG_SCRATCH4_BITS   _u(0xffffffff)
#define WATCHDOG_SCRATCH4_RESET  _u(0x00000000)
#define WATCHDOG_SCRATCH4_MSB    _u(31)
#define WATCHDOG_SCRATCH4_LSB    _u(0)
#define WATCHDOG_SCRATCH4_ACCESS "RW"
// =============================================================================
// Register    : WATCHDOG_SCRATCH5
// Description : Scratch register. Information persists through soft reset of
//               the chip.
#define WATCHDOG_SCRATCH5_OFFSET _u(0x00000020)
#define WATCHDOG_SCRATCH5_BITS   _u(0xffffffff)
#define WATCHDOG_SCRATCH5_RESET  _u(0x00000000)
#define WATCHDOG_SCRATCH5_MSB    _u(31)
#define WATCHDOG_SCRATCH5_LSB    _u(0)
#define WATCHDOG_SCRATCH5_ACCESS "RW"
// =============================================================================
// Register    : WATCHDOG_SCRATCH6
// Description : Scratch register. Information persists through soft reset of
//               the chip.
#define WATCHDOG_SCRATCH6_OFFSET _u(0x00000024)
#define WATCHDOG_SCRATCH6_BITS   _u(0xffffffff)
#define WATCHDOG_SCRATCH6_RESET  _u(0x00000000)
#define WATCHDOG_SCRATCH6_MSB    _u(31)
#define WATCHDOG_SCRATCH6_LSB    _u(0)
#define WATCHDOG_SCRATCH6_ACCESS "RW"
// =============================================================================
// Register    : WATCHDOG_SCRATCH7
// Description : Scratch register. Information persists through soft reset of
//               the chip.
#define WATCHDOG_SCRATCH7_OFFSET _u(0x00000028)
#define WATCHDOG_SCRATCH7_BITS   _u(0xffffffff)
#define WATCHDOG_SCRATCH7_RESET  _u(0x00000000)
#define WATCHDOG_SCRATCH7_MSB    _u(31)
#define WATCHDOG_SCRATCH7_LSB    _u(0)
#define WATCHDOG_SCRATCH7_ACCESS "RW"
// =============================================================================
#endif // _HARDWARE_REGS_WATCHDOG_H

