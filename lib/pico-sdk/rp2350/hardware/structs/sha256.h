// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _HARDWARE_STRUCTS_SHA256_H
#define _HARDWARE_STRUCTS_SHA256_H

/**
 * \file rp2350/sha256.h
 */

#include "hardware/address_mapped.h"
#include "hardware/regs/sha256.h"

// Reference to datasheet: https://datasheets.raspberrypi.com/rp2350/rp2350-datasheet.pdf#tab-registerlist_sha256
//
// The _REG_ macro is intended to help make the register navigable in your IDE (for example, using the "Go to Definition" feature)
// _REG_(x) will link to the corresponding register in hardware/regs/sha256.h.
//
// Bit-field descriptions are of the form:
// BITMASK [BITRANGE] FIELDNAME (RESETVALUE) DESCRIPTION

typedef struct {
    _REG_(SHA256_CSR_OFFSET) // SHA256_CSR
    // Control and status register
    // 0x00001000 [12]    BSWAP        (1) Enable byte swapping of 32-bit values at the point they...
    // 0x00000300 [9:8]   DMA_SIZE     (0x2) Configure DREQ logic for the correct DMA data size
    // 0x00000010 [4]     ERR_WDATA_NOT_RDY (0) Set when a write occurs whilst the SHA-256 core is not...
    // 0x00000004 [2]     SUM_VLD      (1) If 1, the SHA-256 checksum presented in registers SUM0...
    // 0x00000002 [1]     WDATA_RDY    (1) If 1, the SHA-256 core is ready to accept more data...
    // 0x00000001 [0]     START        (0) Write 1 to prepare the SHA-256 core for a new checksum
    io_rw_32 csr;
 
    _REG_(SHA256_WDATA_OFFSET) // SHA256_WDATA
    // Write data register
    // 0xffffffff [31:0]  WDATA        (0x00000000) After pulsing START and writing 16 words of data to this...
    io_wo_32 wdata;
 
    // (Description copied from array index 0 register SHA256_SUM0 applies similarly to other array indexes)
    _REG_(SHA256_SUM0_OFFSET) // SHA256_SUM0
    // 256-bit checksum result
    // 0xffffffff [31:0]  SUM0         (0x00000000) 
    io_ro_32 sum[8];
} sha256_hw_t;

#define sha256_hw ((sha256_hw_t *)SHA256_BASE)
static_assert(sizeof (sha256_hw_t) == 0x0028, "");

#endif // _HARDWARE_STRUCTS_SHA256_H

