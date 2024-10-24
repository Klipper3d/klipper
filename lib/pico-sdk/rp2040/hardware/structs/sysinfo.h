// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _HARDWARE_STRUCTS_SYSINFO_H
#define _HARDWARE_STRUCTS_SYSINFO_H

/**
 * \file rp2040/sysinfo.h
 */

#include "hardware/address_mapped.h"
#include "hardware/regs/sysinfo.h"

// Reference to datasheet: https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf#tab-registerlist_sysinfo
//
// The _REG_ macro is intended to help make the register navigable in your IDE (for example, using the "Go to Definition" feature)
// _REG_(x) will link to the corresponding register in hardware/regs/sysinfo.h.
//
// Bit-field descriptions are of the form:
// BITMASK [BITRANGE] FIELDNAME (RESETVALUE) DESCRIPTION

typedef struct {
    _REG_(SYSINFO_CHIP_ID_OFFSET) // SYSINFO_CHIP_ID
    // JEDEC JEP-106 compliant chip identifier
    // 0xf0000000 [31:28] REVISION     (-) 
    // 0x0ffff000 [27:12] PART         (-) 
    // 0x00000fff [11:0]  MANUFACTURER (-) 
    io_ro_32 chip_id;
 
    _REG_(SYSINFO_PLATFORM_OFFSET) // SYSINFO_PLATFORM
    // Platform register
    // 0x00000002 [1]     ASIC         (0) 
    // 0x00000001 [0]     FPGA         (0) 
    io_ro_32 platform;
 
    uint32_t _pad0[2];
 
    _REG_(SYSINFO_GITREF_RP2040_OFFSET) // SYSINFO_GITREF_RP2040
    // Git hash of the chip source
    // 0xffffffff [31:0]  GITREF_RP2040 (-) 
    io_ro_32 gitref_rp2040;
} sysinfo_hw_t;

#define sysinfo_hw ((sysinfo_hw_t *)SYSINFO_BASE)
static_assert(sizeof (sysinfo_hw_t) == 0x0014, "");

#endif // _HARDWARE_STRUCTS_SYSINFO_H

