// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _HARDWARE_STRUCTS_TBMAN_H
#define _HARDWARE_STRUCTS_TBMAN_H

/**
 * \file rp2040/tbman.h
 */

#include "hardware/address_mapped.h"
#include "hardware/regs/tbman.h"

// Reference to datasheet: https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf#tab-registerlist_tbman
//
// The _REG_ macro is intended to help make the register navigable in your IDE (for example, using the "Go to Definition" feature)
// _REG_(x) will link to the corresponding register in hardware/regs/tbman.h.
//
// Bit-field descriptions are of the form:
// BITMASK [BITRANGE] FIELDNAME (RESETVALUE) DESCRIPTION

typedef struct {
    _REG_(TBMAN_PLATFORM_OFFSET) // TBMAN_PLATFORM
    // Indicates the type of platform in use
    // 0x00000002 [1]     FPGA         (0) Indicates the platform is an FPGA
    // 0x00000001 [0]     ASIC         (1) Indicates the platform is an ASIC
    io_ro_32 platform;
} tbman_hw_t;

#define tbman_hw ((tbman_hw_t *)TBMAN_BASE)
static_assert(sizeof (tbman_hw_t) == 0x0004, "");

#endif // _HARDWARE_STRUCTS_TBMAN_H

