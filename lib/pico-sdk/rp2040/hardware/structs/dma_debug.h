// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _HARDWARE_STRUCTS_DMA_DEBUG_H
#define _HARDWARE_STRUCTS_DMA_DEBUG_H

/**
 * \file rp2040/dma_debug.h
 */

#include "hardware/address_mapped.h"
#include "hardware/regs/dma.h"

// Reference to datasheet: https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf#tab-registerlist_dma
//
// The _REG_ macro is intended to help make the register navigable in your IDE (for example, using the "Go to Definition" feature)
// _REG_(x) will link to the corresponding register in hardware/regs/dma.h.
//
// Bit-field descriptions are of the form:
// BITMASK [BITRANGE] FIELDNAME (RESETVALUE) DESCRIPTION

typedef struct {
    _REG_(DMA_CH0_DBG_CTDREQ_OFFSET) // DMA_CH0_DBG_CTDREQ
    // Read: get channel DREQ counter (i
    // 0x0000003f [5:0]   CH0_DBG_CTDREQ (0x00) 
    io_rw_32 dbg_ctdreq;
 
    _REG_(DMA_CH0_DBG_TCR_OFFSET) // DMA_CH0_DBG_TCR
    // Read to get channel TRANS_COUNT reload value, i
    // 0xffffffff [31:0]  CH0_DBG_TCR  (0x00000000) 
    io_ro_32 dbg_tcr;
 
    uint32_t _pad0[14];
} dma_debug_channel_hw_t;

typedef struct {
    dma_debug_channel_hw_t ch[12];
} dma_debug_hw_t;

#define dma_debug_hw ((dma_debug_hw_t *)(DMA_BASE + DMA_CH0_DBG_CTDREQ_OFFSET))

#endif // _HARDWARE_STRUCTS_DMA_DEBUG_H

