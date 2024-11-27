// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _HARDWARE_STRUCTS_MPU_H
#define _HARDWARE_STRUCTS_MPU_H

/**
 * \file rp2040/mpu.h
 */

#include "hardware/address_mapped.h"
#include "hardware/regs/m0plus.h"

// Reference to datasheet: https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf#tab-registerlist_m0plus
//
// The _REG_ macro is intended to help make the register navigable in your IDE (for example, using the "Go to Definition" feature)
// _REG_(x) will link to the corresponding register in hardware/regs/m0plus.h.
//
// Bit-field descriptions are of the form:
// BITMASK [BITRANGE] FIELDNAME (RESETVALUE) DESCRIPTION

typedef struct {
    _REG_(M0PLUS_MPU_TYPE_OFFSET) // M0PLUS_MPU_TYPE
    // MPU Type Register
    // 0x00ff0000 [23:16] IREGION      (0x00) Instruction region
    // 0x0000ff00 [15:8]  DREGION      (0x08) Number of regions supported by the MPU
    // 0x00000001 [0]     SEPARATE     (0) Indicates support for separate instruction and data address maps
    io_ro_32 type;
 
    _REG_(M0PLUS_MPU_CTRL_OFFSET) // M0PLUS_MPU_CTRL
    // MPU Control Register
    // 0x00000004 [2]     PRIVDEFENA   (0) Controls whether the default memory map is enabled as a...
    // 0x00000002 [1]     HFNMIENA     (0) Controls the use of the MPU for HardFaults and NMIs
    // 0x00000001 [0]     ENABLE       (0) Enables the MPU
    io_rw_32 ctrl;
 
    _REG_(M0PLUS_MPU_RNR_OFFSET) // M0PLUS_MPU_RNR
    // MPU Region Number Register
    // 0x0000000f [3:0]   REGION       (0x0) Indicates the MPU region referenced by the MPU_RBAR and...
    io_rw_32 rnr;
 
    _REG_(M0PLUS_MPU_RBAR_OFFSET) // M0PLUS_MPU_RBAR
    // MPU Region Base Address Register
    // 0xffffff00 [31:8]  ADDR         (0x000000) Base address of the region
    // 0x00000010 [4]     VALID        (0) On writes, indicates whether the write must update the...
    // 0x0000000f [3:0]   REGION       (0x0) On writes, specifies the number of the region whose base...
    io_rw_32 rbar;
 
    _REG_(M0PLUS_MPU_RASR_OFFSET) // M0PLUS_MPU_RASR
    // MPU Region Attribute and Size Register
    // 0xffff0000 [31:16] ATTRS        (0x0000) The MPU Region Attribute field
    // 0x0000ff00 [15:8]  SRD          (0x00) Subregion Disable
    // 0x0000003e [5:1]   SIZE         (0x00) Indicates the region size
    // 0x00000001 [0]     ENABLE       (0) Enables the region
    io_rw_32 rasr;
} mpu_hw_t;

#define mpu_hw ((mpu_hw_t *)(PPB_BASE + M0PLUS_MPU_TYPE_OFFSET))
static_assert(sizeof (mpu_hw_t) == 0x0014, "");

#endif // _HARDWARE_STRUCTS_MPU_H

