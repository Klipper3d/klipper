// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _HARDWARE_STRUCTS_DMA_H
#define _HARDWARE_STRUCTS_DMA_H

/**
 * \file rp2350/dma.h
 */

#include "hardware/address_mapped.h"
#include "hardware/regs/dma.h"
#include "hardware/structs/dma_debug.h"

// Reference to datasheet: https://datasheets.raspberrypi.com/rp2350/rp2350-datasheet.pdf#tab-registerlist_dma
//
// The _REG_ macro is intended to help make the register navigable in your IDE (for example, using the "Go to Definition" feature)
// _REG_(x) will link to the corresponding register in hardware/regs/dma.h.
//
// Bit-field descriptions are of the form:
// BITMASK [BITRANGE] FIELDNAME (RESETVALUE) DESCRIPTION

typedef struct {
    _REG_(DMA_CH0_READ_ADDR_OFFSET) // DMA_CH0_READ_ADDR
    // DMA Channel 0 Read Address pointer
    // 0xffffffff [31:0]  CH0_READ_ADDR (0x00000000) This register updates automatically each time a read completes
    io_rw_32 read_addr;
 
    _REG_(DMA_CH0_WRITE_ADDR_OFFSET) // DMA_CH0_WRITE_ADDR
    // DMA Channel 0 Write Address pointer
    // 0xffffffff [31:0]  CH0_WRITE_ADDR (0x00000000) This register updates automatically each time a write completes
    io_rw_32 write_addr;
 
    _REG_(DMA_CH0_TRANS_COUNT_OFFSET) // DMA_CH0_TRANS_COUNT
    // DMA Channel 0 Transfer Count
    // 0xf0000000 [31:28] MODE         (0x0) When MODE is 0x0, the transfer count decrements with...
    // 0x0fffffff [27:0]  COUNT        (0x0000000) 28-bit transfer count (256 million transfers maximum)
    io_rw_32 transfer_count;
 
    _REG_(DMA_CH0_CTRL_TRIG_OFFSET) // DMA_CH0_CTRL_TRIG
    // DMA Channel 0 Control and Status
    // 0x80000000 [31]    AHB_ERROR    (0) Logical OR of the READ_ERROR and WRITE_ERROR flags
    // 0x40000000 [30]    READ_ERROR   (0) If 1, the channel received a read bus error
    // 0x20000000 [29]    WRITE_ERROR  (0) If 1, the channel received a write bus error
    // 0x04000000 [26]    BUSY         (0) This flag goes high when the channel starts a new...
    // 0x02000000 [25]    SNIFF_EN     (0) If 1, this channel's data transfers are visible to the...
    // 0x01000000 [24]    BSWAP        (0) Apply byte-swap transformation to DMA data
    // 0x00800000 [23]    IRQ_QUIET    (0) In QUIET mode, the channel does not generate IRQs at the...
    // 0x007e0000 [22:17] TREQ_SEL     (0x00) Select a Transfer Request signal
    // 0x0001e000 [16:13] CHAIN_TO     (0x0) When this channel completes, it will trigger the channel...
    // 0x00001000 [12]    RING_SEL     (0) Select whether RING_SIZE applies to read or write addresses
    // 0x00000f00 [11:8]  RING_SIZE    (0x0) Size of address wrap region
    // 0x00000080 [7]     INCR_WRITE_REV (0) If 1, and INCR_WRITE is 1, the write address is...
    // 0x00000040 [6]     INCR_WRITE   (0) If 1, the write address increments with each transfer
    // 0x00000020 [5]     INCR_READ_REV (0) If 1, and INCR_READ is 1, the read address is...
    // 0x00000010 [4]     INCR_READ    (0) If 1, the read address increments with each transfer
    // 0x0000000c [3:2]   DATA_SIZE    (0x0) Set the size of each bus transfer (byte/halfword/word)
    // 0x00000002 [1]     HIGH_PRIORITY (0) HIGH_PRIORITY gives a channel preferential treatment in...
    // 0x00000001 [0]     EN           (0) DMA Channel Enable
    io_rw_32 ctrl_trig;
 
    _REG_(DMA_CH0_AL1_CTRL_OFFSET) // DMA_CH0_AL1_CTRL
    // Alias for channel 0 CTRL register
    // 0xffffffff [31:0]  CH0_AL1_CTRL (-) 
    io_rw_32 al1_ctrl;
 
    _REG_(DMA_CH0_AL1_READ_ADDR_OFFSET) // DMA_CH0_AL1_READ_ADDR
    // Alias for channel 0 READ_ADDR register
    // 0xffffffff [31:0]  CH0_AL1_READ_ADDR (-) 
    io_rw_32 al1_read_addr;
 
    _REG_(DMA_CH0_AL1_WRITE_ADDR_OFFSET) // DMA_CH0_AL1_WRITE_ADDR
    // Alias for channel 0 WRITE_ADDR register
    // 0xffffffff [31:0]  CH0_AL1_WRITE_ADDR (-) 
    io_rw_32 al1_write_addr;
 
    _REG_(DMA_CH0_AL1_TRANS_COUNT_TRIG_OFFSET) // DMA_CH0_AL1_TRANS_COUNT_TRIG
    // Alias for channel 0 TRANS_COUNT register +
    // 0xffffffff [31:0]  CH0_AL1_TRANS_COUNT_TRIG (-) 
    io_rw_32 al1_transfer_count_trig;
 
    _REG_(DMA_CH0_AL2_CTRL_OFFSET) // DMA_CH0_AL2_CTRL
    // Alias for channel 0 CTRL register
    // 0xffffffff [31:0]  CH0_AL2_CTRL (-) 
    io_rw_32 al2_ctrl;
 
    _REG_(DMA_CH0_AL2_TRANS_COUNT_OFFSET) // DMA_CH0_AL2_TRANS_COUNT
    // Alias for channel 0 TRANS_COUNT register
    // 0xffffffff [31:0]  CH0_AL2_TRANS_COUNT (-) 
    io_rw_32 al2_transfer_count;
 
    _REG_(DMA_CH0_AL2_READ_ADDR_OFFSET) // DMA_CH0_AL2_READ_ADDR
    // Alias for channel 0 READ_ADDR register
    // 0xffffffff [31:0]  CH0_AL2_READ_ADDR (-) 
    io_rw_32 al2_read_addr;
 
    _REG_(DMA_CH0_AL2_WRITE_ADDR_TRIG_OFFSET) // DMA_CH0_AL2_WRITE_ADDR_TRIG
    // Alias for channel 0 WRITE_ADDR register +
    // 0xffffffff [31:0]  CH0_AL2_WRITE_ADDR_TRIG (-) 
    io_rw_32 al2_write_addr_trig;
 
    _REG_(DMA_CH0_AL3_CTRL_OFFSET) // DMA_CH0_AL3_CTRL
    // Alias for channel 0 CTRL register
    // 0xffffffff [31:0]  CH0_AL3_CTRL (-) 
    io_rw_32 al3_ctrl;
 
    _REG_(DMA_CH0_AL3_WRITE_ADDR_OFFSET) // DMA_CH0_AL3_WRITE_ADDR
    // Alias for channel 0 WRITE_ADDR register
    // 0xffffffff [31:0]  CH0_AL3_WRITE_ADDR (-) 
    io_rw_32 al3_write_addr;
 
    _REG_(DMA_CH0_AL3_TRANS_COUNT_OFFSET) // DMA_CH0_AL3_TRANS_COUNT
    // Alias for channel 0 TRANS_COUNT register
    // 0xffffffff [31:0]  CH0_AL3_TRANS_COUNT (-) 
    io_rw_32 al3_transfer_count;
 
    _REG_(DMA_CH0_AL3_READ_ADDR_TRIG_OFFSET) // DMA_CH0_AL3_READ_ADDR_TRIG
    // Alias for channel 0 READ_ADDR register +
    // 0xffffffff [31:0]  CH0_AL3_READ_ADDR_TRIG (-) 
    io_rw_32 al3_read_addr_trig;
} dma_channel_hw_t;

typedef struct {
    _REG_(DMA_MPU_BAR0_OFFSET) // DMA_MPU_BAR0
    // Base address register for MPU region 0
    // 0xffffffe0 [31:5]  ADDR         (0x0000000) This MPU region matches addresses where addr[31:5] (the...
    io_rw_32 bar;
 
    _REG_(DMA_MPU_LAR0_OFFSET) // DMA_MPU_LAR0
    // Limit address register for MPU region 0
    // 0xffffffe0 [31:5]  ADDR         (0x0000000) Limit address bits 31:5
    // 0x00000004 [2]     S            (0) Determines the Secure/Non-secure (=1/0) status of...
    // 0x00000002 [1]     P            (0) Determines the Privileged/Unprivileged (=1/0) status of...
    // 0x00000001 [0]     EN           (0) Region enable
    io_rw_32 lar;
} dma_mpu_region_hw_t;

typedef struct {
    _REG_(DMA_INTR_OFFSET) // DMA_INTR
    // Interrupt Status (raw)
    // 0x0000ffff [15:0]  INTR         (0x0000) Raw interrupt status for DMA Channels 0
    io_rw_32 intr;
 
    _REG_(DMA_INTE0_OFFSET) // DMA_INTE0
    // Interrupt Enables for IRQ 0
    // 0x0000ffff [15:0]  INTE0        (0x0000) Set bit n to pass interrupts from channel n to DMA IRQ 0
    io_rw_32 inte;
 
    _REG_(DMA_INTF0_OFFSET) // DMA_INTF0
    // Force Interrupts
    // 0x0000ffff [15:0]  INTF0        (0x0000) Write 1s to force the corresponding bits in INTS0
    io_rw_32 intf;
 
    _REG_(DMA_INTS0_OFFSET) // DMA_INTS0
    // Interrupt Status for IRQ 0
    // 0x0000ffff [15:0]  INTS0        (0x0000) Indicates active channel interrupt requests which are...
    io_rw_32 ints;
} dma_irq_ctrl_hw_t;

typedef struct {
    dma_channel_hw_t ch[16];
 
    union {
        struct {
            _REG_(DMA_INTR_OFFSET) // DMA_INTR
            // Interrupt Status (raw)
            // 0x0000ffff [15:0]  INTR         (0x0000) Raw interrupt status for DMA Channels 0
            io_rw_32 intr;

            _REG_(DMA_INTE0_OFFSET) // DMA_INTE0
            // Interrupt Enables for IRQ 0
            // 0x0000ffff [15:0]  INTE0        (0x0000) Set bit n to pass interrupts from channel n to DMA IRQ 0
            io_rw_32 inte0;

            _REG_(DMA_INTF0_OFFSET) // DMA_INTF0
            // Force Interrupts
            // 0x0000ffff [15:0]  INTF0        (0x0000) Write 1s to force the corresponding bits in INTE0     
            io_rw_32 intf0;

            _REG_(DMA_INTS0_OFFSET) // DMA_INTS0
            // Interrupt Status for IRQ 0
            // 0x0000ffff [15:0]  INTS0        (0x0000) Indicates active channel interrupt requests which are...
            io_rw_32 ints0;

            uint32_t __pad0;

            _REG_(DMA_INTE1_OFFSET) // DMA_INTE1
            // Interrupt Enables for IRQ 1
            // 0x0000ffff [15:0]  INTE1        (0x0000) Set bit n to pass interrupts from channel n to DMA IRQ 1
            io_rw_32 inte1;

            _REG_(DMA_INTF1_OFFSET) // DMA_INTF1
            // Force Interrupts for IRQ 1
            // 0x0000ffff [15:0]  INTF1        (0x0000) Write 1s to force the corresponding bits in INTF1
            io_rw_32 intf1;

            _REG_(DMA_INTS1_OFFSET) // DMA_INTS1
            // Interrupt Status (masked) for IRQ 1
            // 0x0000ffff [15:0]  INTS1        (0x0000) Indicates active channel interrupt requests which are...
            io_rw_32 ints1;

            uint32_t __pad1;

            _REG_(DMA_INTE2_OFFSET) // DMA_INTE2
            // Interrupt Enables for IRQ 2
            // 0x0000ffff [15:0]  INTE2        (0x0000) Set bit n to pass interrupts from channel n to DMA IRQ 2
            io_rw_32 inte2;

            _REG_(DMA_INTF2_OFFSET) // DMA_INTF2
            // Force Interrupts for IRQ 2
            // 0x0000ffff [15:0]  INTF2        (0x0000) Set bit n to pass interrupts from channel n to DMA IRQ 2
            io_rw_32 intf2;

            _REG_(DMA_INTS2_OFFSET) // DMA_INTS2
            // Interrupt Status (masked) for IRQ 2
            // 0x0000ffff [15:0]  INTS2        (0x0000) Set bit n to pass interrupts from channel n to DMA IRQ 2
            io_rw_32 ints2;

            uint32_t __pad2;

            _REG_(DMA_INTE3_OFFSET) // DMA_INTE3
            // Interrupt Enables for IRQ 3
            // 0x0000ffff [15:0]  INTE3        (0x0000) Set bit n to pass interrupts from channel n to DMA IRQ 3
            io_rw_32 inte3;

            _REG_(DMA_INTF3_OFFSET) // DMA_INTF3
            // Force Interrupts for IRQ 3
            // 0x0000ffff [15:0]  INTF3        (0x0000) Set bit n to pass interrupts from channel n to DMA IRQ 3
            io_rw_32 intf3;

            _REG_(DMA_INTS3_OFFSET) // DMA_INTS3
            // Interrupt Status (masked) for IRQ 3
            // 0x0000ffff [15:0]  INTS3        (0x0000) Set bit n to pass interrupts from channel n to DMA IRQ 3
            io_rw_32 ints3;
        };
        dma_irq_ctrl_hw_t irq_ctrl[4];
    };
 
    // (Description copied from array index 0 register DMA_TIMER0 applies similarly to other array indexes)
    _REG_(DMA_TIMER0_OFFSET) // DMA_TIMER0
    // Pacing timer (generate periodic TREQs)
    // 0xffff0000 [31:16] X            (0x0000) Pacing Timer Dividend
    // 0x0000ffff [15:0]  Y            (0x0000) Pacing Timer Divisor
    io_rw_32 timer[4];
 
    _REG_(DMA_MULTI_CHAN_TRIGGER_OFFSET) // DMA_MULTI_CHAN_TRIGGER
    // Trigger one or more channels simultaneously
    // 0x0000ffff [15:0]  MULTI_CHAN_TRIGGER (0x0000) Each bit in this register corresponds to a DMA channel
    io_wo_32 multi_channel_trigger;
 
    _REG_(DMA_SNIFF_CTRL_OFFSET) // DMA_SNIFF_CTRL
    // Sniffer Control
    // 0x00000800 [11]    OUT_INV      (0) If set, the result appears inverted (bitwise complement)...
    // 0x00000400 [10]    OUT_REV      (0) If set, the result appears bit-reversed when read
    // 0x00000200 [9]     BSWAP        (0) Locally perform a byte reverse on the sniffed data,...
    // 0x000001e0 [8:5]   CALC         (0x0) 
    // 0x0000001e [4:1]   DMACH        (0x0) DMA channel for Sniffer to observe
    // 0x00000001 [0]     EN           (0) Enable sniffer
    io_rw_32 sniff_ctrl;
 
    _REG_(DMA_SNIFF_DATA_OFFSET) // DMA_SNIFF_DATA
    // Data accumulator for sniff hardware
    // 0xffffffff [31:0]  SNIFF_DATA   (0x00000000) Write an initial seed value here before starting a DMA...
    io_rw_32 sniff_data;
 
    uint32_t _pad0;
 
    _REG_(DMA_FIFO_LEVELS_OFFSET) // DMA_FIFO_LEVELS
    // Debug RAF, WAF, TDF levels
    // 0x00ff0000 [23:16] RAF_LVL      (0x00) Current Read-Address-FIFO fill level
    // 0x0000ff00 [15:8]  WAF_LVL      (0x00) Current Write-Address-FIFO fill level
    // 0x000000ff [7:0]   TDF_LVL      (0x00) Current Transfer-Data-FIFO fill level
    io_ro_32 fifo_levels;
 
    _REG_(DMA_CHAN_ABORT_OFFSET) // DMA_CHAN_ABORT
    // Abort an in-progress transfer sequence on one or more channels
    // 0x0000ffff [15:0]  CHAN_ABORT   (0x0000) Each bit corresponds to a channel
    io_wo_32 abort;
 
    _REG_(DMA_N_CHANNELS_OFFSET) // DMA_N_CHANNELS
    // The number of channels this DMA instance is equipped with
    // 0x0000001f [4:0]   N_CHANNELS   (-) 
    io_ro_32 n_channels;
 
    uint32_t _pad1[5];
 
    // (Description copied from array index 0 register DMA_SECCFG_CH0 applies similarly to other array indexes)
    _REG_(DMA_SECCFG_CH0_OFFSET) // DMA_SECCFG_CH0
    // Security level configuration for channel 0.
    // 0x00000004 [2]     LOCK         (0) LOCK is 0 at reset, and is set to 1 automatically upon a...
    // 0x00000002 [1]     S            (1) Secure channel
    // 0x00000001 [0]     P            (1) Privileged channel
    io_rw_32 seccfg_ch[16];
 
    // (Description copied from array index 0 register DMA_SECCFG_IRQ0 applies similarly to other array indexes)
    _REG_(DMA_SECCFG_IRQ0_OFFSET) // DMA_SECCFG_IRQ0
    // Security configuration for IRQ 0
    // 0x00000002 [1]     S            (1) Secure IRQ
    // 0x00000001 [0]     P            (1) Privileged IRQ
    io_rw_32 seccfg_irq[4];
 
    _REG_(DMA_SECCFG_MISC_OFFSET) // DMA_SECCFG_MISC
    // Miscellaneous security configuration
    // 0x00000200 [9]     TIMER3_S     (1) If 1, the TIMER3 register is only accessible from a...
    // 0x00000100 [8]     TIMER3_P     (1) If 1, the TIMER3 register is only accessible from a...
    // 0x00000080 [7]     TIMER2_S     (1) If 1, the TIMER2 register is only accessible from a...
    // 0x00000040 [6]     TIMER2_P     (1) If 1, the TIMER2 register is only accessible from a...
    // 0x00000020 [5]     TIMER1_S     (1) If 1, the TIMER1 register is only accessible from a...
    // 0x00000010 [4]     TIMER1_P     (1) If 1, the TIMER1 register is only accessible from a...
    // 0x00000008 [3]     TIMER0_S     (1) If 1, the TIMER0 register is only accessible from a...
    // 0x00000004 [2]     TIMER0_P     (1) If 1, the TIMER0 register is only accessible from a...
    // 0x00000002 [1]     SNIFF_S      (1) If 1, the sniffer can see data transfers from Secure...
    // 0x00000001 [0]     SNIFF_P      (1) If 1, the sniffer can see data transfers from Privileged...
    io_rw_32 seccfg_misc;
 
    uint32_t _pad2[11];
 
    _REG_(DMA_MPU_CTRL_OFFSET) // DMA_MPU_CTRL
    // Control register for DMA MPU
    // 0x00000008 [3]     NS_HIDE_ADDR (0) By default, when a region's S bit is clear,...
    // 0x00000004 [2]     S            (0) Determine whether an address not covered by an active...
    // 0x00000002 [1]     P            (0) Determine whether an address not covered by an active...
    io_rw_32 mpu_ctrl;
 
    dma_mpu_region_hw_t mpu_region[8];
} dma_hw_t;

#define dma_hw ((dma_hw_t *)DMA_BASE)
static_assert(sizeof (dma_hw_t) == 0x0544, "");

#endif // _HARDWARE_STRUCTS_DMA_H

