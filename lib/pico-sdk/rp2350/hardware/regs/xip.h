// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
// =============================================================================
// Register block : XIP
// Version        : 1
// Bus type       : ahb
// Description    : QSPI flash execute-in-place block
// =============================================================================
#ifndef _HARDWARE_REGS_XIP_H
#define _HARDWARE_REGS_XIP_H
// =============================================================================
// Register    : XIP_CTRL
// Description : Cache control register. Read-only from a Non-secure context.
#define XIP_CTRL_OFFSET _u(0x00000000)
#define XIP_CTRL_BITS   _u(0x00000ffb)
#define XIP_CTRL_RESET  _u(0x00000083)
// -----------------------------------------------------------------------------
// Field       : XIP_CTRL_WRITABLE_M1
// Description : If 1, enable writes to XIP memory window 1 (addresses
//               0x11000000 through 0x11ffffff, and their uncached mirrors). If
//               0, this region is read-only.
//
//               XIP memory is *read-only by default*. This bit must be set to
//               enable writes if a RAM device is attached on QSPI chip select
//               1.
//
//               The default read-only behaviour avoids two issues with writing
//               to a read-only QSPI device (e.g. flash). First, a write will
//               initially appear to succeed due to caching, but the data will
//               eventually be lost when the written line is evicted, causing
//               unpredictable behaviour.
//
//               Second, when a written line is evicted, it will cause a write
//               command to be issued to the flash, which can break the flash
//               out of its continuous read mode. After this point, flash reads
//               will return garbage. This is a security concern, as it allows
//               Non-secure software to break Secure flash reads if it has
//               permission to write to any flash address.
//
//               Note the read-only behaviour is implemented by downgrading
//               writes to reads, so writes will still cause allocation of an
//               address, but have no other effect.
#define XIP_CTRL_WRITABLE_M1_RESET  _u(0x0)
#define XIP_CTRL_WRITABLE_M1_BITS   _u(0x00000800)
#define XIP_CTRL_WRITABLE_M1_MSB    _u(11)
#define XIP_CTRL_WRITABLE_M1_LSB    _u(11)
#define XIP_CTRL_WRITABLE_M1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : XIP_CTRL_WRITABLE_M0
// Description : If 1, enable writes to XIP memory window 0 (addresses
//               0x10000000 through 0x10ffffff, and their uncached mirrors). If
//               0, this region is read-only.
//
//               XIP memory is *read-only by default*. This bit must be set to
//               enable writes if a RAM device is attached on QSPI chip select
//               0.
//
//               The default read-only behaviour avoids two issues with writing
//               to a read-only QSPI device (e.g. flash). First, a write will
//               initially appear to succeed due to caching, but the data will
//               eventually be lost when the written line is evicted, causing
//               unpredictable behaviour.
//
//               Second, when a written line is evicted, it will cause a write
//               command to be issued to the flash, which can break the flash
//               out of its continuous read mode. After this point, flash reads
//               will return garbage. This is a security concern, as it allows
//               Non-secure software to break Secure flash reads if it has
//               permission to write to any flash address.
//
//               Note the read-only behaviour is implemented by downgrading
//               writes to reads, so writes will still cause allocation of an
//               address, but have no other effect.
#define XIP_CTRL_WRITABLE_M0_RESET  _u(0x0)
#define XIP_CTRL_WRITABLE_M0_BITS   _u(0x00000400)
#define XIP_CTRL_WRITABLE_M0_MSB    _u(10)
#define XIP_CTRL_WRITABLE_M0_LSB    _u(10)
#define XIP_CTRL_WRITABLE_M0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : XIP_CTRL_SPLIT_WAYS
// Description : When 1, route all cached+Secure accesses to way 0 of the cache,
//               and route all cached+Non-secure accesses to way 1 of the cache.
//
//               This partitions the cache into two half-sized direct-mapped
//               regions, such that Non-secure code can not observe cache line
//               state changes caused by Secure execution.
//
//               A full cache flush is required when changing the value of
//               SPLIT_WAYS. The flush should be performed whilst SPLIT_WAYS is
//               0, so that both cache ways are accessible for invalidation.
#define XIP_CTRL_SPLIT_WAYS_RESET  _u(0x0)
#define XIP_CTRL_SPLIT_WAYS_BITS   _u(0x00000200)
#define XIP_CTRL_SPLIT_WAYS_MSB    _u(9)
#define XIP_CTRL_SPLIT_WAYS_LSB    _u(9)
#define XIP_CTRL_SPLIT_WAYS_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : XIP_CTRL_MAINT_NONSEC
// Description : When 0, Non-secure accesses to the cache maintenance address
//               window (addr[27] == 1, addr[26] == 0) will generate a bus
//               error. When 1, Non-secure accesses can perform cache
//               maintenance operations by writing to the cache maintenance
//               address window.
//
//               Cache maintenance operations may be used to corrupt Secure data
//               by invalidating cache lines inappropriately, or map Secure
//               content into a Non-secure region by pinning cache lines.
//               Therefore this bit should generally be set to 0, unless Secure
//               code is not using the cache.
//
//               Care should also be taken to clear the cache data memory and
//               tag memory before granting maintenance operations to Non-secure
//               code.
#define XIP_CTRL_MAINT_NONSEC_RESET  _u(0x0)
#define XIP_CTRL_MAINT_NONSEC_BITS   _u(0x00000100)
#define XIP_CTRL_MAINT_NONSEC_MSB    _u(8)
#define XIP_CTRL_MAINT_NONSEC_LSB    _u(8)
#define XIP_CTRL_MAINT_NONSEC_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : XIP_CTRL_NO_UNTRANSLATED_NONSEC
// Description : When 1, Non-secure accesses to the uncached, untranslated
//               window (addr[27:26] == 3) will generate a bus error.
#define XIP_CTRL_NO_UNTRANSLATED_NONSEC_RESET  _u(0x1)
#define XIP_CTRL_NO_UNTRANSLATED_NONSEC_BITS   _u(0x00000080)
#define XIP_CTRL_NO_UNTRANSLATED_NONSEC_MSB    _u(7)
#define XIP_CTRL_NO_UNTRANSLATED_NONSEC_LSB    _u(7)
#define XIP_CTRL_NO_UNTRANSLATED_NONSEC_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : XIP_CTRL_NO_UNTRANSLATED_SEC
// Description : When 1, Secure accesses to the uncached, untranslated window
//               (addr[27:26] == 3) will generate a bus error.
#define XIP_CTRL_NO_UNTRANSLATED_SEC_RESET  _u(0x0)
#define XIP_CTRL_NO_UNTRANSLATED_SEC_BITS   _u(0x00000040)
#define XIP_CTRL_NO_UNTRANSLATED_SEC_MSB    _u(6)
#define XIP_CTRL_NO_UNTRANSLATED_SEC_LSB    _u(6)
#define XIP_CTRL_NO_UNTRANSLATED_SEC_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : XIP_CTRL_NO_UNCACHED_NONSEC
// Description : When 1, Non-secure accesses to the uncached window (addr[27:26]
//               == 1) will generate a bus error. This may reduce the number of
//               SAU/MPU/PMP regions required to protect flash contents.
//
//               Note this does not disable access to the uncached, untranslated
//               window -- see NO_UNTRANSLATED_SEC.
#define XIP_CTRL_NO_UNCACHED_NONSEC_RESET  _u(0x0)
#define XIP_CTRL_NO_UNCACHED_NONSEC_BITS   _u(0x00000020)
#define XIP_CTRL_NO_UNCACHED_NONSEC_MSB    _u(5)
#define XIP_CTRL_NO_UNCACHED_NONSEC_LSB    _u(5)
#define XIP_CTRL_NO_UNCACHED_NONSEC_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : XIP_CTRL_NO_UNCACHED_SEC
// Description : When 1, Secure accesses to the uncached window (addr[27:26] ==
//               1) will generate a bus error. This may reduce the number of
//               SAU/MPU/PMP regions required to protect flash contents.
//
//               Note this does not disable access to the uncached, untranslated
//               window -- see NO_UNTRANSLATED_SEC.
#define XIP_CTRL_NO_UNCACHED_SEC_RESET  _u(0x0)
#define XIP_CTRL_NO_UNCACHED_SEC_BITS   _u(0x00000010)
#define XIP_CTRL_NO_UNCACHED_SEC_MSB    _u(4)
#define XIP_CTRL_NO_UNCACHED_SEC_LSB    _u(4)
#define XIP_CTRL_NO_UNCACHED_SEC_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : XIP_CTRL_POWER_DOWN
// Description : When 1, the cache memories are powered down. They retain state,
//               but can not be accessed. This reduces static power dissipation.
//               Writing 1 to this bit forces CTRL_EN_SECURE and
//               CTRL_EN_NONSECURE to 0, i.e. the cache cannot be enabled when
//               powered down.
#define XIP_CTRL_POWER_DOWN_RESET  _u(0x0)
#define XIP_CTRL_POWER_DOWN_BITS   _u(0x00000008)
#define XIP_CTRL_POWER_DOWN_MSB    _u(3)
#define XIP_CTRL_POWER_DOWN_LSB    _u(3)
#define XIP_CTRL_POWER_DOWN_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : XIP_CTRL_EN_NONSECURE
// Description : When 1, enable the cache for Non-secure accesses. When enabled,
//               Non-secure XIP accesses to the cached (addr[26] == 0) window
//               will query the cache, and QSPI accesses are performed only if
//               the requested data is not present. When disabled, Secure access
//               ignore the cache contents, and always access the QSPI
//               interface.
//
//               Accesses to the uncached (addr[26] == 1) window will never
//               query the cache, irrespective of this bit.
#define XIP_CTRL_EN_NONSECURE_RESET  _u(0x1)
#define XIP_CTRL_EN_NONSECURE_BITS   _u(0x00000002)
#define XIP_CTRL_EN_NONSECURE_MSB    _u(1)
#define XIP_CTRL_EN_NONSECURE_LSB    _u(1)
#define XIP_CTRL_EN_NONSECURE_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : XIP_CTRL_EN_SECURE
// Description : When 1, enable the cache for Secure accesses. When enabled,
//               Secure XIP accesses to the cached (addr[26] == 0) window will
//               query the cache, and QSPI accesses are performed only if the
//               requested data is not present. When disabled, Secure access
//               ignore the cache contents, and always access the QSPI
//               interface.
//
//               Accesses to the uncached (addr[26] == 1) window will never
//               query the cache, irrespective of this bit.
//
//               There is no cache-as-SRAM address window. Cache lines are
//               allocated for SRAM-like use by individually pinning them, and
//               keeping the cache enabled.
#define XIP_CTRL_EN_SECURE_RESET  _u(0x1)
#define XIP_CTRL_EN_SECURE_BITS   _u(0x00000001)
#define XIP_CTRL_EN_SECURE_MSB    _u(0)
#define XIP_CTRL_EN_SECURE_LSB    _u(0)
#define XIP_CTRL_EN_SECURE_ACCESS "RW"
// =============================================================================
// Register    : XIP_STAT
#define XIP_STAT_OFFSET _u(0x00000008)
#define XIP_STAT_BITS   _u(0x00000006)
#define XIP_STAT_RESET  _u(0x00000002)
// -----------------------------------------------------------------------------
// Field       : XIP_STAT_FIFO_FULL
// Description : When 1, indicates the XIP streaming FIFO is completely full.
//               The streaming FIFO is 2 entries deep, so the full and empty
//               flag allow its level to be ascertained.
#define XIP_STAT_FIFO_FULL_RESET  _u(0x0)
#define XIP_STAT_FIFO_FULL_BITS   _u(0x00000004)
#define XIP_STAT_FIFO_FULL_MSB    _u(2)
#define XIP_STAT_FIFO_FULL_LSB    _u(2)
#define XIP_STAT_FIFO_FULL_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : XIP_STAT_FIFO_EMPTY
// Description : When 1, indicates the XIP streaming FIFO is completely empty.
#define XIP_STAT_FIFO_EMPTY_RESET  _u(0x1)
#define XIP_STAT_FIFO_EMPTY_BITS   _u(0x00000002)
#define XIP_STAT_FIFO_EMPTY_MSB    _u(1)
#define XIP_STAT_FIFO_EMPTY_LSB    _u(1)
#define XIP_STAT_FIFO_EMPTY_ACCESS "RO"
// =============================================================================
// Register    : XIP_CTR_HIT
// Description : Cache Hit counter
//               A 32 bit saturating counter that increments upon each cache
//               hit,
//               i.e. when an XIP access is serviced directly from cached data.
//               Write any value to clear.
#define XIP_CTR_HIT_OFFSET _u(0x0000000c)
#define XIP_CTR_HIT_BITS   _u(0xffffffff)
#define XIP_CTR_HIT_RESET  _u(0x00000000)
#define XIP_CTR_HIT_MSB    _u(31)
#define XIP_CTR_HIT_LSB    _u(0)
#define XIP_CTR_HIT_ACCESS "WC"
// =============================================================================
// Register    : XIP_CTR_ACC
// Description : Cache Access counter
//               A 32 bit saturating counter that increments upon each XIP
//               access,
//               whether the cache is hit or not. This includes noncacheable
//               accesses.
//               Write any value to clear.
#define XIP_CTR_ACC_OFFSET _u(0x00000010)
#define XIP_CTR_ACC_BITS   _u(0xffffffff)
#define XIP_CTR_ACC_RESET  _u(0x00000000)
#define XIP_CTR_ACC_MSB    _u(31)
#define XIP_CTR_ACC_LSB    _u(0)
#define XIP_CTR_ACC_ACCESS "WC"
// =============================================================================
// Register    : XIP_STREAM_ADDR
// Description : FIFO stream address
//               The address of the next word to be streamed from flash to the
//               streaming FIFO.
//               Increments automatically after each flash access.
//               Write the initial access address here before starting a
//               streaming read.
#define XIP_STREAM_ADDR_OFFSET _u(0x00000014)
#define XIP_STREAM_ADDR_BITS   _u(0xfffffffc)
#define XIP_STREAM_ADDR_RESET  _u(0x00000000)
#define XIP_STREAM_ADDR_MSB    _u(31)
#define XIP_STREAM_ADDR_LSB    _u(2)
#define XIP_STREAM_ADDR_ACCESS "RW"
// =============================================================================
// Register    : XIP_STREAM_CTR
// Description : FIFO stream control
//               Write a nonzero value to start a streaming read. This will then
//               progress in the background, using flash idle cycles to transfer
//               a linear data block from flash to the streaming FIFO.
//               Decrements automatically (1 at a time) as the stream
//               progresses, and halts on reaching 0.
//               Write 0 to halt an in-progress stream, and discard any in-
//               flight
//               read, so that a new stream can immediately be started (after
//               draining the FIFO and reinitialising STREAM_ADDR)
#define XIP_STREAM_CTR_OFFSET _u(0x00000018)
#define XIP_STREAM_CTR_BITS   _u(0x003fffff)
#define XIP_STREAM_CTR_RESET  _u(0x00000000)
#define XIP_STREAM_CTR_MSB    _u(21)
#define XIP_STREAM_CTR_LSB    _u(0)
#define XIP_STREAM_CTR_ACCESS "RW"
// =============================================================================
// Register    : XIP_STREAM_FIFO
// Description : FIFO stream data
//               Streamed data is buffered here, for retrieval by the system
//               DMA.
//               This FIFO can also be accessed via the XIP_AUX slave, to avoid
//               exposing
//               the DMA to bus stalls caused by other XIP traffic.
#define XIP_STREAM_FIFO_OFFSET _u(0x0000001c)
#define XIP_STREAM_FIFO_BITS   _u(0xffffffff)
#define XIP_STREAM_FIFO_RESET  _u(0x00000000)
#define XIP_STREAM_FIFO_MSB    _u(31)
#define XIP_STREAM_FIFO_LSB    _u(0)
#define XIP_STREAM_FIFO_ACCESS "RF"
// =============================================================================
#endif // _HARDWARE_REGS_XIP_H

