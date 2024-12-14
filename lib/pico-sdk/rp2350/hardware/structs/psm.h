// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _HARDWARE_STRUCTS_PSM_H
#define _HARDWARE_STRUCTS_PSM_H

/**
 * \file rp2350/psm.h
 */

#include "hardware/address_mapped.h"
#include "hardware/regs/psm.h"

// Reference to datasheet: https://datasheets.raspberrypi.com/rp2350/rp2350-datasheet.pdf#tab-registerlist_psm
//
// The _REG_ macro is intended to help make the register navigable in your IDE (for example, using the "Go to Definition" feature)
// _REG_(x) will link to the corresponding register in hardware/regs/psm.h.
//
// Bit-field descriptions are of the form:
// BITMASK [BITRANGE] FIELDNAME (RESETVALUE) DESCRIPTION

typedef struct {
    _REG_(PSM_FRCE_ON_OFFSET) // PSM_FRCE_ON
    // Force block out of reset (i
    // 0x01000000 [24]    PROC1        (0) 
    // 0x00800000 [23]    PROC0        (0) 
    // 0x00400000 [22]    ACCESSCTRL   (0) 
    // 0x00200000 [21]    SIO          (0) 
    // 0x00100000 [20]    XIP          (0) 
    // 0x00080000 [19]    SRAM9        (0) 
    // 0x00040000 [18]    SRAM8        (0) 
    // 0x00020000 [17]    SRAM7        (0) 
    // 0x00010000 [16]    SRAM6        (0) 
    // 0x00008000 [15]    SRAM5        (0) 
    // 0x00004000 [14]    SRAM4        (0) 
    // 0x00002000 [13]    SRAM3        (0) 
    // 0x00001000 [12]    SRAM2        (0) 
    // 0x00000800 [11]    SRAM1        (0) 
    // 0x00000400 [10]    SRAM0        (0) 
    // 0x00000200 [9]     BOOTRAM      (0) 
    // 0x00000100 [8]     ROM          (0) 
    // 0x00000080 [7]     BUSFABRIC    (0) 
    // 0x00000040 [6]     PSM_READY    (0) 
    // 0x00000020 [5]     CLOCKS       (0) 
    // 0x00000010 [4]     RESETS       (0) 
    // 0x00000008 [3]     XOSC         (0) 
    // 0x00000004 [2]     ROSC         (0) 
    // 0x00000002 [1]     OTP          (0) 
    // 0x00000001 [0]     PROC_COLD    (0) 
    io_rw_32 frce_on;
 
    _REG_(PSM_FRCE_OFF_OFFSET) // PSM_FRCE_OFF
    // Force into reset (i
    // 0x01000000 [24]    PROC1        (0) 
    // 0x00800000 [23]    PROC0        (0) 
    // 0x00400000 [22]    ACCESSCTRL   (0) 
    // 0x00200000 [21]    SIO          (0) 
    // 0x00100000 [20]    XIP          (0) 
    // 0x00080000 [19]    SRAM9        (0) 
    // 0x00040000 [18]    SRAM8        (0) 
    // 0x00020000 [17]    SRAM7        (0) 
    // 0x00010000 [16]    SRAM6        (0) 
    // 0x00008000 [15]    SRAM5        (0) 
    // 0x00004000 [14]    SRAM4        (0) 
    // 0x00002000 [13]    SRAM3        (0) 
    // 0x00001000 [12]    SRAM2        (0) 
    // 0x00000800 [11]    SRAM1        (0) 
    // 0x00000400 [10]    SRAM0        (0) 
    // 0x00000200 [9]     BOOTRAM      (0) 
    // 0x00000100 [8]     ROM          (0) 
    // 0x00000080 [7]     BUSFABRIC    (0) 
    // 0x00000040 [6]     PSM_READY    (0) 
    // 0x00000020 [5]     CLOCKS       (0) 
    // 0x00000010 [4]     RESETS       (0) 
    // 0x00000008 [3]     XOSC         (0) 
    // 0x00000004 [2]     ROSC         (0) 
    // 0x00000002 [1]     OTP          (0) 
    // 0x00000001 [0]     PROC_COLD    (0) 
    io_rw_32 frce_off;
 
    _REG_(PSM_WDSEL_OFFSET) // PSM_WDSEL
    // Set to 1 if the watchdog should reset this
    // 0x01000000 [24]    PROC1        (0) 
    // 0x00800000 [23]    PROC0        (0) 
    // 0x00400000 [22]    ACCESSCTRL   (0) 
    // 0x00200000 [21]    SIO          (0) 
    // 0x00100000 [20]    XIP          (0) 
    // 0x00080000 [19]    SRAM9        (0) 
    // 0x00040000 [18]    SRAM8        (0) 
    // 0x00020000 [17]    SRAM7        (0) 
    // 0x00010000 [16]    SRAM6        (0) 
    // 0x00008000 [15]    SRAM5        (0) 
    // 0x00004000 [14]    SRAM4        (0) 
    // 0x00002000 [13]    SRAM3        (0) 
    // 0x00001000 [12]    SRAM2        (0) 
    // 0x00000800 [11]    SRAM1        (0) 
    // 0x00000400 [10]    SRAM0        (0) 
    // 0x00000200 [9]     BOOTRAM      (0) 
    // 0x00000100 [8]     ROM          (0) 
    // 0x00000080 [7]     BUSFABRIC    (0) 
    // 0x00000040 [6]     PSM_READY    (0) 
    // 0x00000020 [5]     CLOCKS       (0) 
    // 0x00000010 [4]     RESETS       (0) 
    // 0x00000008 [3]     XOSC         (0) 
    // 0x00000004 [2]     ROSC         (0) 
    // 0x00000002 [1]     OTP          (0) 
    // 0x00000001 [0]     PROC_COLD    (0) 
    io_rw_32 wdsel;
 
    _REG_(PSM_DONE_OFFSET) // PSM_DONE
    // Is the subsystem ready?
    // 0x01000000 [24]    PROC1        (0) 
    // 0x00800000 [23]    PROC0        (0) 
    // 0x00400000 [22]    ACCESSCTRL   (0) 
    // 0x00200000 [21]    SIO          (0) 
    // 0x00100000 [20]    XIP          (0) 
    // 0x00080000 [19]    SRAM9        (0) 
    // 0x00040000 [18]    SRAM8        (0) 
    // 0x00020000 [17]    SRAM7        (0) 
    // 0x00010000 [16]    SRAM6        (0) 
    // 0x00008000 [15]    SRAM5        (0) 
    // 0x00004000 [14]    SRAM4        (0) 
    // 0x00002000 [13]    SRAM3        (0) 
    // 0x00001000 [12]    SRAM2        (0) 
    // 0x00000800 [11]    SRAM1        (0) 
    // 0x00000400 [10]    SRAM0        (0) 
    // 0x00000200 [9]     BOOTRAM      (0) 
    // 0x00000100 [8]     ROM          (0) 
    // 0x00000080 [7]     BUSFABRIC    (0) 
    // 0x00000040 [6]     PSM_READY    (0) 
    // 0x00000020 [5]     CLOCKS       (0) 
    // 0x00000010 [4]     RESETS       (0) 
    // 0x00000008 [3]     XOSC         (0) 
    // 0x00000004 [2]     ROSC         (0) 
    // 0x00000002 [1]     OTP          (0) 
    // 0x00000001 [0]     PROC_COLD    (0) 
    io_ro_32 done;
} psm_hw_t;

#define psm_hw ((psm_hw_t *)PSM_BASE)
static_assert(sizeof (psm_hw_t) == 0x0010, "");

#endif // _HARDWARE_STRUCTS_PSM_H

