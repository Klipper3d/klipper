// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _HARDWARE_STRUCTS_TIMER_H
#define _HARDWARE_STRUCTS_TIMER_H

/**
 * \file rp2350/timer.h
 */

#include "hardware/address_mapped.h"
#include "hardware/regs/timer.h"

// Reference to datasheet: https://datasheets.raspberrypi.com/rp2350/rp2350-datasheet.pdf#tab-registerlist_timer
//
// The _REG_ macro is intended to help make the register navigable in your IDE (for example, using the "Go to Definition" feature)
// _REG_(x) will link to the corresponding register in hardware/regs/timer.h.
//
// Bit-field descriptions are of the form:
// BITMASK [BITRANGE] FIELDNAME (RESETVALUE) DESCRIPTION

typedef struct {
    _REG_(TIMER_TIMEHW_OFFSET) // TIMER_TIMEHW
    // Write to bits 63:32 of time always write timelw before timehw
    // 0xffffffff [31:0]  TIMEHW       (0x00000000) 
    io_wo_32 timehw;
 
    _REG_(TIMER_TIMELW_OFFSET) // TIMER_TIMELW
    // Write to bits 31:0 of time writes do not get copied to time until timehw is written
    // 0xffffffff [31:0]  TIMELW       (0x00000000) 
    io_wo_32 timelw;
 
    _REG_(TIMER_TIMEHR_OFFSET) // TIMER_TIMEHR
    // Read from bits 63:32 of time always read timelr before timehr
    // 0xffffffff [31:0]  TIMEHR       (0x00000000) 
    io_ro_32 timehr;
 
    _REG_(TIMER_TIMELR_OFFSET) // TIMER_TIMELR
    // Read from bits 31:0 of time
    // 0xffffffff [31:0]  TIMELR       (0x00000000) 
    io_ro_32 timelr;
 
    // (Description copied from array index 0 register TIMER_ALARM0 applies similarly to other array indexes)
    _REG_(TIMER_ALARM0_OFFSET) // TIMER_ALARM0
    // Arm alarm 0, and configure the time it will fire
    // 0xffffffff [31:0]  ALARM0       (0x00000000) 
    io_rw_32 alarm[4];
 
    _REG_(TIMER_ARMED_OFFSET) // TIMER_ARMED
    // Indicates the armed/disarmed status of each alarm
    // 0x0000000f [3:0]   ARMED        (0x0) 
    io_rw_32 armed;
 
    _REG_(TIMER_TIMERAWH_OFFSET) // TIMER_TIMERAWH
    // Raw read from bits 63:32 of time (no side effects)
    // 0xffffffff [31:0]  TIMERAWH     (0x00000000) 
    io_ro_32 timerawh;
 
    _REG_(TIMER_TIMERAWL_OFFSET) // TIMER_TIMERAWL
    // Raw read from bits 31:0 of time (no side effects)
    // 0xffffffff [31:0]  TIMERAWL     (0x00000000) 
    io_ro_32 timerawl;
 
    _REG_(TIMER_DBGPAUSE_OFFSET) // TIMER_DBGPAUSE
    // Set bits high to enable pause when the corresponding debug ports are active
    // 0x00000004 [2]     DBG1         (1) Pause when processor 1 is in debug mode
    // 0x00000002 [1]     DBG0         (1) Pause when processor 0 is in debug mode
    io_rw_32 dbgpause;
 
    _REG_(TIMER_PAUSE_OFFSET) // TIMER_PAUSE
    // Set high to pause the timer
    // 0x00000001 [0]     PAUSE        (0) 
    io_rw_32 pause;
 
    _REG_(TIMER_LOCKED_OFFSET) // TIMER_LOCKED
    // Set locked bit to disable write access to timer Once set, cannot be cleared (without a reset)
    // 0x00000001 [0]     LOCKED       (0) 
    io_rw_32 locked;
 
    _REG_(TIMER_SOURCE_OFFSET) // TIMER_SOURCE
    // Selects the source for the timer
    // 0x00000001 [0]     CLK_SYS      (0) 
    io_rw_32 source;
 
    _REG_(TIMER_INTR_OFFSET) // TIMER_INTR
    // Raw Interrupts
    // 0x00000008 [3]     ALARM_3      (0) 
    // 0x00000004 [2]     ALARM_2      (0) 
    // 0x00000002 [1]     ALARM_1      (0) 
    // 0x00000001 [0]     ALARM_0      (0) 
    io_rw_32 intr;
 
    _REG_(TIMER_INTE_OFFSET) // TIMER_INTE
    // Interrupt Enable
    // 0x00000008 [3]     ALARM_3      (0) 
    // 0x00000004 [2]     ALARM_2      (0) 
    // 0x00000002 [1]     ALARM_1      (0) 
    // 0x00000001 [0]     ALARM_0      (0) 
    io_rw_32 inte;
 
    _REG_(TIMER_INTF_OFFSET) // TIMER_INTF
    // Interrupt Force
    // 0x00000008 [3]     ALARM_3      (0) 
    // 0x00000004 [2]     ALARM_2      (0) 
    // 0x00000002 [1]     ALARM_1      (0) 
    // 0x00000001 [0]     ALARM_0      (0) 
    io_rw_32 intf;
 
    _REG_(TIMER_INTS_OFFSET) // TIMER_INTS
    // Interrupt status after masking & forcing
    // 0x00000008 [3]     ALARM_3      (0) 
    // 0x00000004 [2]     ALARM_2      (0) 
    // 0x00000002 [1]     ALARM_1      (0) 
    // 0x00000001 [0]     ALARM_0      (0) 
    io_ro_32 ints;
} timer_hw_t;

#define timer0_hw ((timer_hw_t *)TIMER0_BASE)
#define timer1_hw ((timer_hw_t *)TIMER1_BASE)
static_assert(sizeof (timer_hw_t) == 0x004c, "");

#endif // _HARDWARE_STRUCTS_TIMER_H

