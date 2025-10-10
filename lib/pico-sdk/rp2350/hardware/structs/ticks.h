// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _HARDWARE_STRUCTS_TICKS_H
#define _HARDWARE_STRUCTS_TICKS_H

/**
 * \file rp2350/ticks.h
 */

#include "hardware/address_mapped.h"
#include "hardware/regs/ticks.h"

// Reference to datasheet: https://datasheets.raspberrypi.com/rp2350/rp2350-datasheet.pdf#tab-registerlist_ticks
//
// The _REG_ macro is intended to help make the register navigable in your IDE (for example, using the "Go to Definition" feature)
// _REG_(x) will link to the corresponding register in hardware/regs/ticks.h.
//
// Bit-field descriptions are of the form:
// BITMASK [BITRANGE] FIELDNAME (RESETVALUE) DESCRIPTION

/*! \brief Tick generator numbers on RP2350 (used as typedef \ref tick_gen_num_t) 
 *  \ingroup hardware_ticks
 */
typedef enum tick_gen_num_rp2350 {
    TICK_PROC0 = 0,
    TICK_PROC1 = 1,
    TICK_TIMER0 = 2,
    TICK_TIMER1 = 3,
    TICK_WATCHDOG = 4,
    TICK_RISCV = 5,
    TICK_COUNT
} tick_gen_num_t;

typedef struct {
    _REG_(TICKS_PROC0_CTRL_OFFSET) // TICKS_PROC0_CTRL
    // Controls the tick generator
    // 0x00000002 [1]     RUNNING      (-) Is the tick generator running?
    // 0x00000001 [0]     ENABLE       (0) start / stop tick generation
    io_rw_32 ctrl;
 
    _REG_(TICKS_PROC0_CYCLES_OFFSET) // TICKS_PROC0_CYCLES
    // 0x000001ff [8:0]   PROC0_CYCLES (0x000) Total number of clk_tick cycles before the next tick
    io_rw_32 cycles;
 
    _REG_(TICKS_PROC0_COUNT_OFFSET) // TICKS_PROC0_COUNT
    // 0x000001ff [8:0]   PROC0_COUNT  (-) Count down timer: the remaining number clk_tick cycles...
    io_ro_32 count;
} ticks_slice_hw_t;

typedef struct {
    ticks_slice_hw_t ticks[6];
} ticks_hw_t;

#define ticks_hw ((ticks_hw_t *)TICKS_BASE)
static_assert(sizeof (ticks_hw_t) == 0x0048, "");

#endif // _HARDWARE_STRUCTS_TICKS_H

