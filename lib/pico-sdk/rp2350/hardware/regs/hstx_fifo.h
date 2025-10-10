// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
// =============================================================================
// Register block : HSTX_FIFO
// Version        : 1
// Bus type       : ahbl
// Description    : FIFO status and write access for HSTX
// =============================================================================
#ifndef _HARDWARE_REGS_HSTX_FIFO_H
#define _HARDWARE_REGS_HSTX_FIFO_H
// =============================================================================
// Register    : HSTX_FIFO_STAT
// Description : FIFO status
#define HSTX_FIFO_STAT_OFFSET _u(0x00000000)
#define HSTX_FIFO_STAT_BITS   _u(0x000007ff)
#define HSTX_FIFO_STAT_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : HSTX_FIFO_STAT_WOF
// Description : FIFO was written when full. Write 1 to clear.
#define HSTX_FIFO_STAT_WOF_RESET  _u(0x0)
#define HSTX_FIFO_STAT_WOF_BITS   _u(0x00000400)
#define HSTX_FIFO_STAT_WOF_MSB    _u(10)
#define HSTX_FIFO_STAT_WOF_LSB    _u(10)
#define HSTX_FIFO_STAT_WOF_ACCESS "WC"
// -----------------------------------------------------------------------------
// Field       : HSTX_FIFO_STAT_EMPTY
#define HSTX_FIFO_STAT_EMPTY_RESET  "-"
#define HSTX_FIFO_STAT_EMPTY_BITS   _u(0x00000200)
#define HSTX_FIFO_STAT_EMPTY_MSB    _u(9)
#define HSTX_FIFO_STAT_EMPTY_LSB    _u(9)
#define HSTX_FIFO_STAT_EMPTY_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : HSTX_FIFO_STAT_FULL
#define HSTX_FIFO_STAT_FULL_RESET  "-"
#define HSTX_FIFO_STAT_FULL_BITS   _u(0x00000100)
#define HSTX_FIFO_STAT_FULL_MSB    _u(8)
#define HSTX_FIFO_STAT_FULL_LSB    _u(8)
#define HSTX_FIFO_STAT_FULL_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : HSTX_FIFO_STAT_LEVEL
#define HSTX_FIFO_STAT_LEVEL_RESET  _u(0x00)
#define HSTX_FIFO_STAT_LEVEL_BITS   _u(0x000000ff)
#define HSTX_FIFO_STAT_LEVEL_MSB    _u(7)
#define HSTX_FIFO_STAT_LEVEL_LSB    _u(0)
#define HSTX_FIFO_STAT_LEVEL_ACCESS "RO"
// =============================================================================
// Register    : HSTX_FIFO_FIFO
// Description : Write access to FIFO
#define HSTX_FIFO_FIFO_OFFSET _u(0x00000004)
#define HSTX_FIFO_FIFO_BITS   _u(0xffffffff)
#define HSTX_FIFO_FIFO_RESET  _u(0x00000000)
#define HSTX_FIFO_FIFO_MSB    _u(31)
#define HSTX_FIFO_FIFO_LSB    _u(0)
#define HSTX_FIFO_FIFO_ACCESS "WF"
// =============================================================================
#endif // _HARDWARE_REGS_HSTX_FIFO_H

