// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _HARDWARE_STRUCTS_SAU_H
#define _HARDWARE_STRUCTS_SAU_H

/**
 * \file rp2350/sau.h
 */

#include "hardware/address_mapped.h"
#include "hardware/regs/m33.h"

// Reference to datasheet: https://datasheets.raspberrypi.com/rp2350/rp2350-datasheet.pdf#tab-registerlist_m33
//
// The _REG_ macro is intended to help make the register navigable in your IDE (for example, using the "Go to Definition" feature)
// _REG_(x) will link to the corresponding register in hardware/regs/m33.h.
//
// Bit-field descriptions are of the form:
// BITMASK [BITRANGE] FIELDNAME (RESETVALUE) DESCRIPTION

#if defined(__riscv) && PICO_FORBID_ARM_HEADERS_ON_RISCV
#error "Arm header included in a RISC-V build with PICO_FORBID_ARM_HEADERS_ON_RISCV=1"
#endif

typedef struct {
    _REG_(M33_SAU_CTRL_OFFSET) // M33_SAU_CTRL
    // Allows enabling of the Security Attribution Unit
    // 0x00000002 [1]     ALLNS        (0) When SAU_CTRL
    // 0x00000001 [0]     ENABLE       (0) Enables the SAU
    io_rw_32 ctrl;
 
    _REG_(M33_SAU_TYPE_OFFSET) // M33_SAU_TYPE
    // Indicates the number of regions implemented by the Security Attribution Unit
    // 0x000000ff [7:0]   SREGION      (0x08) The number of implemented SAU regions
    io_ro_32 type;
 
    _REG_(M33_SAU_RNR_OFFSET) // M33_SAU_RNR
    // Selects the region currently accessed by SAU_RBAR and SAU_RLAR
    // 0x000000ff [7:0]   REGION       (0x00) Indicates the SAU region accessed by SAU_RBAR and SAU_RLAR
    io_rw_32 rnr;
 
    _REG_(M33_SAU_RBAR_OFFSET) // M33_SAU_RBAR
    // Provides indirect read and write access to the base address of the currently selected SAU region
    // 0xffffffe0 [31:5]  BADDR        (0x0000000) Holds bits [31:5] of the base address for the selected SAU region
    io_rw_32 rbar;
 
    _REG_(M33_SAU_RLAR_OFFSET) // M33_SAU_RLAR
    // Provides indirect read and write access to the limit address of the currently selected SAU region
    // 0xffffffe0 [31:5]  LADDR        (0x0000000) Holds bits [31:5] of the limit address for the selected...
    // 0x00000002 [1]     NSC          (0) Controls whether Non-secure state is permitted to...
    // 0x00000001 [0]     ENABLE       (0) SAU region enable
    io_rw_32 rlar;
} armv8m_sau_hw_t;

#define sau_hw ((armv8m_sau_hw_t *)(PPB_BASE + M33_SAU_CTRL_OFFSET))
#define sau_ns_hw ((armv8m_sau_hw_t *)(PPB_NONSEC_BASE + M33_SAU_CTRL_OFFSET))
static_assert(sizeof (armv8m_sau_hw_t) == 0x0014, "");

#endif // _HARDWARE_STRUCTS_SAU_H

