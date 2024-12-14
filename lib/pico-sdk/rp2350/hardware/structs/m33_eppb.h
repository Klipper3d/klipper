// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _HARDWARE_STRUCTS_M33_EPPB_H
#define _HARDWARE_STRUCTS_M33_EPPB_H

/**
 * \file rp2350/m33_eppb.h
 */

#include "hardware/address_mapped.h"
#include "hardware/regs/m33_eppb.h"

// Reference to datasheet: https://datasheets.raspberrypi.com/rp2350/rp2350-datasheet.pdf#tab-registerlist_m33_eppb
//
// The _REG_ macro is intended to help make the register navigable in your IDE (for example, using the "Go to Definition" feature)
// _REG_(x) will link to the corresponding register in hardware/regs/m33_eppb.h.
//
// Bit-field descriptions are of the form:
// BITMASK [BITRANGE] FIELDNAME (RESETVALUE) DESCRIPTION

#if defined(__riscv) && PICO_FORBID_ARM_HEADERS_ON_RISCV
#error "Arm header included in a RISC-V build with PICO_FORBID_ARM_HEADERS_ON_RISCV=1"
#endif
  

typedef struct {
    // (Description copied from array index 0 register M33_EPPB_NMI_MASK0 applies similarly to other array indexes)
    _REG_(M33_EPPB_NMI_MASK0_OFFSET) // M33_EPPB_NMI_MASK0
    // NMI mask for IRQs 0 through 31
    // 0xffffffff [31:0]  NMI_MASK0    (0x00000000) 
    io_rw_32 nmi_mask[2];
 
    _REG_(M33_EPPB_SLEEPCTRL_OFFSET) // M33_EPPB_SLEEPCTRL
    // Nonstandard sleep control register
    // 0x00000004 [2]     WICENACK     (0) Status signal from the processor's interrupt controller
    // 0x00000002 [1]     WICENREQ     (1) Request that the next processor deep sleep is a WIC sleep
    // 0x00000001 [0]     LIGHT_SLEEP  (0) By default, any processor sleep will deassert the...
    io_rw_32 sleepctrl;
} m33_eppb_hw_t;

#define eppb_hw ((m33_eppb_hw_t *)EPPB_BASE)
static_assert(sizeof (m33_eppb_hw_t) == 0x000c, "");

#endif // _HARDWARE_STRUCTS_M33_EPPB_H

