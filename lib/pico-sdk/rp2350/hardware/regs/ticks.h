// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
// =============================================================================
// Register block : TICKS
// Version        : 1
// Bus type       : apb
// =============================================================================
#ifndef _HARDWARE_REGS_TICKS_H
#define _HARDWARE_REGS_TICKS_H
// =============================================================================
// Register    : TICKS_PROC0_CTRL
// Description : Controls the tick generator
#define TICKS_PROC0_CTRL_OFFSET _u(0x00000000)
#define TICKS_PROC0_CTRL_BITS   _u(0x00000003)
#define TICKS_PROC0_CTRL_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : TICKS_PROC0_CTRL_RUNNING
// Description : Is the tick generator running?
#define TICKS_PROC0_CTRL_RUNNING_RESET  "-"
#define TICKS_PROC0_CTRL_RUNNING_BITS   _u(0x00000002)
#define TICKS_PROC0_CTRL_RUNNING_MSB    _u(1)
#define TICKS_PROC0_CTRL_RUNNING_LSB    _u(1)
#define TICKS_PROC0_CTRL_RUNNING_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : TICKS_PROC0_CTRL_ENABLE
// Description : start / stop tick generation
#define TICKS_PROC0_CTRL_ENABLE_RESET  _u(0x0)
#define TICKS_PROC0_CTRL_ENABLE_BITS   _u(0x00000001)
#define TICKS_PROC0_CTRL_ENABLE_MSB    _u(0)
#define TICKS_PROC0_CTRL_ENABLE_LSB    _u(0)
#define TICKS_PROC0_CTRL_ENABLE_ACCESS "RW"
// =============================================================================
// Register    : TICKS_PROC0_CYCLES
// Description : None
//               Total number of clk_tick cycles before the next tick.
#define TICKS_PROC0_CYCLES_OFFSET _u(0x00000004)
#define TICKS_PROC0_CYCLES_BITS   _u(0x000001ff)
#define TICKS_PROC0_CYCLES_RESET  _u(0x00000000)
#define TICKS_PROC0_CYCLES_MSB    _u(8)
#define TICKS_PROC0_CYCLES_LSB    _u(0)
#define TICKS_PROC0_CYCLES_ACCESS "RW"
// =============================================================================
// Register    : TICKS_PROC0_COUNT
// Description : None
//               Count down timer: the remaining number clk_tick cycles before
//               the next tick is generated.
#define TICKS_PROC0_COUNT_OFFSET _u(0x00000008)
#define TICKS_PROC0_COUNT_BITS   _u(0x000001ff)
#define TICKS_PROC0_COUNT_RESET  "-"
#define TICKS_PROC0_COUNT_MSB    _u(8)
#define TICKS_PROC0_COUNT_LSB    _u(0)
#define TICKS_PROC0_COUNT_ACCESS "RO"
// =============================================================================
// Register    : TICKS_PROC1_CTRL
// Description : Controls the tick generator
#define TICKS_PROC1_CTRL_OFFSET _u(0x0000000c)
#define TICKS_PROC1_CTRL_BITS   _u(0x00000003)
#define TICKS_PROC1_CTRL_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : TICKS_PROC1_CTRL_RUNNING
// Description : Is the tick generator running?
#define TICKS_PROC1_CTRL_RUNNING_RESET  "-"
#define TICKS_PROC1_CTRL_RUNNING_BITS   _u(0x00000002)
#define TICKS_PROC1_CTRL_RUNNING_MSB    _u(1)
#define TICKS_PROC1_CTRL_RUNNING_LSB    _u(1)
#define TICKS_PROC1_CTRL_RUNNING_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : TICKS_PROC1_CTRL_ENABLE
// Description : start / stop tick generation
#define TICKS_PROC1_CTRL_ENABLE_RESET  _u(0x0)
#define TICKS_PROC1_CTRL_ENABLE_BITS   _u(0x00000001)
#define TICKS_PROC1_CTRL_ENABLE_MSB    _u(0)
#define TICKS_PROC1_CTRL_ENABLE_LSB    _u(0)
#define TICKS_PROC1_CTRL_ENABLE_ACCESS "RW"
// =============================================================================
// Register    : TICKS_PROC1_CYCLES
// Description : None
//               Total number of clk_tick cycles before the next tick.
#define TICKS_PROC1_CYCLES_OFFSET _u(0x00000010)
#define TICKS_PROC1_CYCLES_BITS   _u(0x000001ff)
#define TICKS_PROC1_CYCLES_RESET  _u(0x00000000)
#define TICKS_PROC1_CYCLES_MSB    _u(8)
#define TICKS_PROC1_CYCLES_LSB    _u(0)
#define TICKS_PROC1_CYCLES_ACCESS "RW"
// =============================================================================
// Register    : TICKS_PROC1_COUNT
// Description : None
//               Count down timer: the remaining number clk_tick cycles before
//               the next tick is generated.
#define TICKS_PROC1_COUNT_OFFSET _u(0x00000014)
#define TICKS_PROC1_COUNT_BITS   _u(0x000001ff)
#define TICKS_PROC1_COUNT_RESET  "-"
#define TICKS_PROC1_COUNT_MSB    _u(8)
#define TICKS_PROC1_COUNT_LSB    _u(0)
#define TICKS_PROC1_COUNT_ACCESS "RO"
// =============================================================================
// Register    : TICKS_TIMER0_CTRL
// Description : Controls the tick generator
#define TICKS_TIMER0_CTRL_OFFSET _u(0x00000018)
#define TICKS_TIMER0_CTRL_BITS   _u(0x00000003)
#define TICKS_TIMER0_CTRL_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : TICKS_TIMER0_CTRL_RUNNING
// Description : Is the tick generator running?
#define TICKS_TIMER0_CTRL_RUNNING_RESET  "-"
#define TICKS_TIMER0_CTRL_RUNNING_BITS   _u(0x00000002)
#define TICKS_TIMER0_CTRL_RUNNING_MSB    _u(1)
#define TICKS_TIMER0_CTRL_RUNNING_LSB    _u(1)
#define TICKS_TIMER0_CTRL_RUNNING_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : TICKS_TIMER0_CTRL_ENABLE
// Description : start / stop tick generation
#define TICKS_TIMER0_CTRL_ENABLE_RESET  _u(0x0)
#define TICKS_TIMER0_CTRL_ENABLE_BITS   _u(0x00000001)
#define TICKS_TIMER0_CTRL_ENABLE_MSB    _u(0)
#define TICKS_TIMER0_CTRL_ENABLE_LSB    _u(0)
#define TICKS_TIMER0_CTRL_ENABLE_ACCESS "RW"
// =============================================================================
// Register    : TICKS_TIMER0_CYCLES
// Description : None
//               Total number of clk_tick cycles before the next tick.
#define TICKS_TIMER0_CYCLES_OFFSET _u(0x0000001c)
#define TICKS_TIMER0_CYCLES_BITS   _u(0x000001ff)
#define TICKS_TIMER0_CYCLES_RESET  _u(0x00000000)
#define TICKS_TIMER0_CYCLES_MSB    _u(8)
#define TICKS_TIMER0_CYCLES_LSB    _u(0)
#define TICKS_TIMER0_CYCLES_ACCESS "RW"
// =============================================================================
// Register    : TICKS_TIMER0_COUNT
// Description : None
//               Count down timer: the remaining number clk_tick cycles before
//               the next tick is generated.
#define TICKS_TIMER0_COUNT_OFFSET _u(0x00000020)
#define TICKS_TIMER0_COUNT_BITS   _u(0x000001ff)
#define TICKS_TIMER0_COUNT_RESET  "-"
#define TICKS_TIMER0_COUNT_MSB    _u(8)
#define TICKS_TIMER0_COUNT_LSB    _u(0)
#define TICKS_TIMER0_COUNT_ACCESS "RO"
// =============================================================================
// Register    : TICKS_TIMER1_CTRL
// Description : Controls the tick generator
#define TICKS_TIMER1_CTRL_OFFSET _u(0x00000024)
#define TICKS_TIMER1_CTRL_BITS   _u(0x00000003)
#define TICKS_TIMER1_CTRL_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : TICKS_TIMER1_CTRL_RUNNING
// Description : Is the tick generator running?
#define TICKS_TIMER1_CTRL_RUNNING_RESET  "-"
#define TICKS_TIMER1_CTRL_RUNNING_BITS   _u(0x00000002)
#define TICKS_TIMER1_CTRL_RUNNING_MSB    _u(1)
#define TICKS_TIMER1_CTRL_RUNNING_LSB    _u(1)
#define TICKS_TIMER1_CTRL_RUNNING_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : TICKS_TIMER1_CTRL_ENABLE
// Description : start / stop tick generation
#define TICKS_TIMER1_CTRL_ENABLE_RESET  _u(0x0)
#define TICKS_TIMER1_CTRL_ENABLE_BITS   _u(0x00000001)
#define TICKS_TIMER1_CTRL_ENABLE_MSB    _u(0)
#define TICKS_TIMER1_CTRL_ENABLE_LSB    _u(0)
#define TICKS_TIMER1_CTRL_ENABLE_ACCESS "RW"
// =============================================================================
// Register    : TICKS_TIMER1_CYCLES
// Description : None
//               Total number of clk_tick cycles before the next tick.
#define TICKS_TIMER1_CYCLES_OFFSET _u(0x00000028)
#define TICKS_TIMER1_CYCLES_BITS   _u(0x000001ff)
#define TICKS_TIMER1_CYCLES_RESET  _u(0x00000000)
#define TICKS_TIMER1_CYCLES_MSB    _u(8)
#define TICKS_TIMER1_CYCLES_LSB    _u(0)
#define TICKS_TIMER1_CYCLES_ACCESS "RW"
// =============================================================================
// Register    : TICKS_TIMER1_COUNT
// Description : None
//               Count down timer: the remaining number clk_tick cycles before
//               the next tick is generated.
#define TICKS_TIMER1_COUNT_OFFSET _u(0x0000002c)
#define TICKS_TIMER1_COUNT_BITS   _u(0x000001ff)
#define TICKS_TIMER1_COUNT_RESET  "-"
#define TICKS_TIMER1_COUNT_MSB    _u(8)
#define TICKS_TIMER1_COUNT_LSB    _u(0)
#define TICKS_TIMER1_COUNT_ACCESS "RO"
// =============================================================================
// Register    : TICKS_WATCHDOG_CTRL
// Description : Controls the tick generator
#define TICKS_WATCHDOG_CTRL_OFFSET _u(0x00000030)
#define TICKS_WATCHDOG_CTRL_BITS   _u(0x00000003)
#define TICKS_WATCHDOG_CTRL_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : TICKS_WATCHDOG_CTRL_RUNNING
// Description : Is the tick generator running?
#define TICKS_WATCHDOG_CTRL_RUNNING_RESET  "-"
#define TICKS_WATCHDOG_CTRL_RUNNING_BITS   _u(0x00000002)
#define TICKS_WATCHDOG_CTRL_RUNNING_MSB    _u(1)
#define TICKS_WATCHDOG_CTRL_RUNNING_LSB    _u(1)
#define TICKS_WATCHDOG_CTRL_RUNNING_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : TICKS_WATCHDOG_CTRL_ENABLE
// Description : start / stop tick generation
#define TICKS_WATCHDOG_CTRL_ENABLE_RESET  _u(0x0)
#define TICKS_WATCHDOG_CTRL_ENABLE_BITS   _u(0x00000001)
#define TICKS_WATCHDOG_CTRL_ENABLE_MSB    _u(0)
#define TICKS_WATCHDOG_CTRL_ENABLE_LSB    _u(0)
#define TICKS_WATCHDOG_CTRL_ENABLE_ACCESS "RW"
// =============================================================================
// Register    : TICKS_WATCHDOG_CYCLES
// Description : None
//               Total number of clk_tick cycles before the next tick.
#define TICKS_WATCHDOG_CYCLES_OFFSET _u(0x00000034)
#define TICKS_WATCHDOG_CYCLES_BITS   _u(0x000001ff)
#define TICKS_WATCHDOG_CYCLES_RESET  _u(0x00000000)
#define TICKS_WATCHDOG_CYCLES_MSB    _u(8)
#define TICKS_WATCHDOG_CYCLES_LSB    _u(0)
#define TICKS_WATCHDOG_CYCLES_ACCESS "RW"
// =============================================================================
// Register    : TICKS_WATCHDOG_COUNT
// Description : None
//               Count down timer: the remaining number clk_tick cycles before
//               the next tick is generated.
#define TICKS_WATCHDOG_COUNT_OFFSET _u(0x00000038)
#define TICKS_WATCHDOG_COUNT_BITS   _u(0x000001ff)
#define TICKS_WATCHDOG_COUNT_RESET  "-"
#define TICKS_WATCHDOG_COUNT_MSB    _u(8)
#define TICKS_WATCHDOG_COUNT_LSB    _u(0)
#define TICKS_WATCHDOG_COUNT_ACCESS "RO"
// =============================================================================
// Register    : TICKS_RISCV_CTRL
// Description : Controls the tick generator
#define TICKS_RISCV_CTRL_OFFSET _u(0x0000003c)
#define TICKS_RISCV_CTRL_BITS   _u(0x00000003)
#define TICKS_RISCV_CTRL_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : TICKS_RISCV_CTRL_RUNNING
// Description : Is the tick generator running?
#define TICKS_RISCV_CTRL_RUNNING_RESET  "-"
#define TICKS_RISCV_CTRL_RUNNING_BITS   _u(0x00000002)
#define TICKS_RISCV_CTRL_RUNNING_MSB    _u(1)
#define TICKS_RISCV_CTRL_RUNNING_LSB    _u(1)
#define TICKS_RISCV_CTRL_RUNNING_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : TICKS_RISCV_CTRL_ENABLE
// Description : start / stop tick generation
#define TICKS_RISCV_CTRL_ENABLE_RESET  _u(0x0)
#define TICKS_RISCV_CTRL_ENABLE_BITS   _u(0x00000001)
#define TICKS_RISCV_CTRL_ENABLE_MSB    _u(0)
#define TICKS_RISCV_CTRL_ENABLE_LSB    _u(0)
#define TICKS_RISCV_CTRL_ENABLE_ACCESS "RW"
// =============================================================================
// Register    : TICKS_RISCV_CYCLES
// Description : None
//               Total number of clk_tick cycles before the next tick.
#define TICKS_RISCV_CYCLES_OFFSET _u(0x00000040)
#define TICKS_RISCV_CYCLES_BITS   _u(0x000001ff)
#define TICKS_RISCV_CYCLES_RESET  _u(0x00000000)
#define TICKS_RISCV_CYCLES_MSB    _u(8)
#define TICKS_RISCV_CYCLES_LSB    _u(0)
#define TICKS_RISCV_CYCLES_ACCESS "RW"
// =============================================================================
// Register    : TICKS_RISCV_COUNT
// Description : None
//               Count down timer: the remaining number clk_tick cycles before
//               the next tick is generated.
#define TICKS_RISCV_COUNT_OFFSET _u(0x00000044)
#define TICKS_RISCV_COUNT_BITS   _u(0x000001ff)
#define TICKS_RISCV_COUNT_RESET  "-"
#define TICKS_RISCV_COUNT_MSB    _u(8)
#define TICKS_RISCV_COUNT_LSB    _u(0)
#define TICKS_RISCV_COUNT_ACCESS "RO"
// =============================================================================
#endif // _HARDWARE_REGS_TICKS_H

