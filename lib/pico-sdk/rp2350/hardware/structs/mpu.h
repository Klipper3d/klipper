// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _HARDWARE_STRUCTS_MPU_H
#define _HARDWARE_STRUCTS_MPU_H

/**
 * \file rp2350/mpu.h
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
    _REG_(M33_MPU_TYPE_OFFSET) // M33_MPU_TYPE
    // The MPU Type Register indicates how many regions the MPU `FTSSS supports
    // 0x0000ff00 [15:8]  DREGION      (0x08) Number of regions supported by the MPU
    // 0x00000001 [0]     SEPARATE     (0) Indicates support for separate instructions and data...
    io_ro_32 type;
 
    _REG_(M33_MPU_CTRL_OFFSET) // M33_MPU_CTRL
    // Enables the MPU and, when the MPU is enabled, controls whether the default memory map is enabled...
    // 0x00000004 [2]     PRIVDEFENA   (0) Controls whether the default memory map is enabled for...
    // 0x00000002 [1]     HFNMIENA     (0) Controls whether handlers executing with priority less...
    // 0x00000001 [0]     ENABLE       (0) Enables the MPU
    io_rw_32 ctrl;
 
    _REG_(M33_MPU_RNR_OFFSET) // M33_MPU_RNR
    // Selects the region currently accessed by MPU_RBAR and MPU_RLAR
    // 0x00000007 [2:0]   REGION       (0x0) Indicates the memory region accessed by MPU_RBAR and MPU_RLAR
    io_rw_32 rnr;
 
    _REG_(M33_MPU_RBAR_OFFSET) // M33_MPU_RBAR
    // Provides indirect read and write access to the base address of the currently selected MPU region `FTSSS
    // 0xffffffe0 [31:5]  BASE         (0x0000000) Contains bits [31:5] of the lower inclusive limit of the...
    // 0x00000018 [4:3]   SH           (0x0) Defines the Shareability domain of this region for Normal memory
    // 0x00000006 [2:1]   AP           (0x0) Defines the access permissions for this region
    // 0x00000001 [0]     XN           (0) Defines whether code can be executed from this region
    io_rw_32 rbar;
 
    _REG_(M33_MPU_RLAR_OFFSET) // M33_MPU_RLAR
    // Provides indirect read and write access to the limit address of the currently selected MPU region `FTSSS
    // 0xffffffe0 [31:5]  LIMIT        (0x0000000) Contains bits [31:5] of the upper inclusive limit of the...
    // 0x0000000e [3:1]   ATTRINDX     (0x0) Associates a set of attributes in the MPU_MAIR0 and...
    // 0x00000001 [0]     EN           (0) Region enable
    io_rw_32 rlar;
 
    _REG_(M33_MPU_RBAR_A1_OFFSET) // M33_MPU_RBAR_A1
    // Provides indirect read and write access to the base address of the MPU region selected by...
    // 0xffffffe0 [31:5]  BASE         (0x0000000) Contains bits [31:5] of the lower inclusive limit of the...
    // 0x00000018 [4:3]   SH           (0x0) Defines the Shareability domain of this region for Normal memory
    // 0x00000006 [2:1]   AP           (0x0) Defines the access permissions for this region
    // 0x00000001 [0]     XN           (0) Defines whether code can be executed from this region
    io_rw_32 rbar_a1;
 
    _REG_(M33_MPU_RLAR_A1_OFFSET) // M33_MPU_RLAR_A1
    // Provides indirect read and write access to the limit address of the currently selected MPU...
    // 0xffffffe0 [31:5]  LIMIT        (0x0000000) Contains bits [31:5] of the upper inclusive limit of the...
    // 0x0000000e [3:1]   ATTRINDX     (0x0) Associates a set of attributes in the MPU_MAIR0 and...
    // 0x00000001 [0]     EN           (0) Region enable
    io_rw_32 rlar_a1;
 
    _REG_(M33_MPU_RBAR_A2_OFFSET) // M33_MPU_RBAR_A2
    // Provides indirect read and write access to the base address of the MPU region selected by...
    // 0xffffffe0 [31:5]  BASE         (0x0000000) Contains bits [31:5] of the lower inclusive limit of the...
    // 0x00000018 [4:3]   SH           (0x0) Defines the Shareability domain of this region for Normal memory
    // 0x00000006 [2:1]   AP           (0x0) Defines the access permissions for this region
    // 0x00000001 [0]     XN           (0) Defines whether code can be executed from this region
    io_rw_32 rbar_a2;
 
    _REG_(M33_MPU_RLAR_A2_OFFSET) // M33_MPU_RLAR_A2
    // Provides indirect read and write access to the limit address of the currently selected MPU...
    // 0xffffffe0 [31:5]  LIMIT        (0x0000000) Contains bits [31:5] of the upper inclusive limit of the...
    // 0x0000000e [3:1]   ATTRINDX     (0x0) Associates a set of attributes in the MPU_MAIR0 and...
    // 0x00000001 [0]     EN           (0) Region enable
    io_rw_32 rlar_a2;
 
    _REG_(M33_MPU_RBAR_A3_OFFSET) // M33_MPU_RBAR_A3
    // Provides indirect read and write access to the base address of the MPU region selected by...
    // 0xffffffe0 [31:5]  BASE         (0x0000000) Contains bits [31:5] of the lower inclusive limit of the...
    // 0x00000018 [4:3]   SH           (0x0) Defines the Shareability domain of this region for Normal memory
    // 0x00000006 [2:1]   AP           (0x0) Defines the access permissions for this region
    // 0x00000001 [0]     XN           (0) Defines whether code can be executed from this region
    io_rw_32 rbar_a3;
 
    _REG_(M33_MPU_RLAR_A3_OFFSET) // M33_MPU_RLAR_A3
    // Provides indirect read and write access to the limit address of the currently selected MPU...
    // 0xffffffe0 [31:5]  LIMIT        (0x0000000) Contains bits [31:5] of the upper inclusive limit of the...
    // 0x0000000e [3:1]   ATTRINDX     (0x0) Associates a set of attributes in the MPU_MAIR0 and...
    // 0x00000001 [0]     EN           (0) Region enable
    io_rw_32 rlar_a3;
 
    uint32_t _pad0;
 
    // (Description copied from array index 0 register M33_MPU_MAIR0 applies similarly to other array indexes)
    _REG_(M33_MPU_MAIR0_OFFSET) // M33_MPU_MAIR0
    // Along with MPU_MAIR1, provides the memory attribute encodings corresponding to the AttrIndex values
    // 0xff000000 [31:24] ATTR3        (0x00) Memory attribute encoding for MPU regions with an AttrIndex of 3
    // 0x00ff0000 [23:16] ATTR2        (0x00) Memory attribute encoding for MPU regions with an AttrIndex of 2
    // 0x0000ff00 [15:8]  ATTR1        (0x00) Memory attribute encoding for MPU regions with an AttrIndex of 1
    // 0x000000ff [7:0]   ATTR0        (0x00) Memory attribute encoding for MPU regions with an AttrIndex of 0
    io_rw_32 mair[2];
} mpu_hw_t;

#define mpu_hw ((mpu_hw_t *)(PPB_BASE + M33_MPU_TYPE_OFFSET))
#define mpu_ns_hw ((mpu_hw_t *)(PPB_NONSEC_BASE + M33_MPU_TYPE_OFFSET))
static_assert(sizeof (mpu_hw_t) == 0x0038, "");

#endif // _HARDWARE_STRUCTS_MPU_H

