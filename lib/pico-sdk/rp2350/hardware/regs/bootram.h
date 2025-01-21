// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
// =============================================================================
// Register block : BOOTRAM
// Version        : 1
// Bus type       : apb
// Description    : Additional registers mapped adjacent to the bootram, for use
//                  by the bootrom.
// =============================================================================
#ifndef _HARDWARE_REGS_BOOTRAM_H
#define _HARDWARE_REGS_BOOTRAM_H
// =============================================================================
// Register    : BOOTRAM_WRITE_ONCE0
// Description : This registers always ORs writes into its current contents.
//               Once a bit is set, it can only be cleared by a reset.
#define BOOTRAM_WRITE_ONCE0_OFFSET _u(0x00000800)
#define BOOTRAM_WRITE_ONCE0_BITS   _u(0xffffffff)
#define BOOTRAM_WRITE_ONCE0_RESET  _u(0x00000000)
#define BOOTRAM_WRITE_ONCE0_MSB    _u(31)
#define BOOTRAM_WRITE_ONCE0_LSB    _u(0)
#define BOOTRAM_WRITE_ONCE0_ACCESS "RW"
// =============================================================================
// Register    : BOOTRAM_WRITE_ONCE1
// Description : This registers always ORs writes into its current contents.
//               Once a bit is set, it can only be cleared by a reset.
#define BOOTRAM_WRITE_ONCE1_OFFSET _u(0x00000804)
#define BOOTRAM_WRITE_ONCE1_BITS   _u(0xffffffff)
#define BOOTRAM_WRITE_ONCE1_RESET  _u(0x00000000)
#define BOOTRAM_WRITE_ONCE1_MSB    _u(31)
#define BOOTRAM_WRITE_ONCE1_LSB    _u(0)
#define BOOTRAM_WRITE_ONCE1_ACCESS "RW"
// =============================================================================
// Register    : BOOTRAM_BOOTLOCK_STAT
// Description : Bootlock status register. 1=unclaimed, 0=claimed. These locks
//               function identically to the SIO spinlocks, but are reserved for
//               bootrom use.
#define BOOTRAM_BOOTLOCK_STAT_OFFSET _u(0x00000808)
#define BOOTRAM_BOOTLOCK_STAT_BITS   _u(0x000000ff)
#define BOOTRAM_BOOTLOCK_STAT_RESET  _u(0x000000ff)
#define BOOTRAM_BOOTLOCK_STAT_MSB    _u(7)
#define BOOTRAM_BOOTLOCK_STAT_LSB    _u(0)
#define BOOTRAM_BOOTLOCK_STAT_ACCESS "RW"
// =============================================================================
// Register    : BOOTRAM_BOOTLOCK0
// Description : Read to claim and check. Write to unclaim. The value returned
//               on successful claim is 1 << n, and on failed claim is zero.
#define BOOTRAM_BOOTLOCK0_OFFSET _u(0x0000080c)
#define BOOTRAM_BOOTLOCK0_BITS   _u(0xffffffff)
#define BOOTRAM_BOOTLOCK0_RESET  _u(0x00000000)
#define BOOTRAM_BOOTLOCK0_MSB    _u(31)
#define BOOTRAM_BOOTLOCK0_LSB    _u(0)
#define BOOTRAM_BOOTLOCK0_ACCESS "RW"
// =============================================================================
// Register    : BOOTRAM_BOOTLOCK1
// Description : Read to claim and check. Write to unclaim. The value returned
//               on successful claim is 1 << n, and on failed claim is zero.
#define BOOTRAM_BOOTLOCK1_OFFSET _u(0x00000810)
#define BOOTRAM_BOOTLOCK1_BITS   _u(0xffffffff)
#define BOOTRAM_BOOTLOCK1_RESET  _u(0x00000000)
#define BOOTRAM_BOOTLOCK1_MSB    _u(31)
#define BOOTRAM_BOOTLOCK1_LSB    _u(0)
#define BOOTRAM_BOOTLOCK1_ACCESS "RW"
// =============================================================================
// Register    : BOOTRAM_BOOTLOCK2
// Description : Read to claim and check. Write to unclaim. The value returned
//               on successful claim is 1 << n, and on failed claim is zero.
#define BOOTRAM_BOOTLOCK2_OFFSET _u(0x00000814)
#define BOOTRAM_BOOTLOCK2_BITS   _u(0xffffffff)
#define BOOTRAM_BOOTLOCK2_RESET  _u(0x00000000)
#define BOOTRAM_BOOTLOCK2_MSB    _u(31)
#define BOOTRAM_BOOTLOCK2_LSB    _u(0)
#define BOOTRAM_BOOTLOCK2_ACCESS "RW"
// =============================================================================
// Register    : BOOTRAM_BOOTLOCK3
// Description : Read to claim and check. Write to unclaim. The value returned
//               on successful claim is 1 << n, and on failed claim is zero.
#define BOOTRAM_BOOTLOCK3_OFFSET _u(0x00000818)
#define BOOTRAM_BOOTLOCK3_BITS   _u(0xffffffff)
#define BOOTRAM_BOOTLOCK3_RESET  _u(0x00000000)
#define BOOTRAM_BOOTLOCK3_MSB    _u(31)
#define BOOTRAM_BOOTLOCK3_LSB    _u(0)
#define BOOTRAM_BOOTLOCK3_ACCESS "RW"
// =============================================================================
// Register    : BOOTRAM_BOOTLOCK4
// Description : Read to claim and check. Write to unclaim. The value returned
//               on successful claim is 1 << n, and on failed claim is zero.
#define BOOTRAM_BOOTLOCK4_OFFSET _u(0x0000081c)
#define BOOTRAM_BOOTLOCK4_BITS   _u(0xffffffff)
#define BOOTRAM_BOOTLOCK4_RESET  _u(0x00000000)
#define BOOTRAM_BOOTLOCK4_MSB    _u(31)
#define BOOTRAM_BOOTLOCK4_LSB    _u(0)
#define BOOTRAM_BOOTLOCK4_ACCESS "RW"
// =============================================================================
// Register    : BOOTRAM_BOOTLOCK5
// Description : Read to claim and check. Write to unclaim. The value returned
//               on successful claim is 1 << n, and on failed claim is zero.
#define BOOTRAM_BOOTLOCK5_OFFSET _u(0x00000820)
#define BOOTRAM_BOOTLOCK5_BITS   _u(0xffffffff)
#define BOOTRAM_BOOTLOCK5_RESET  _u(0x00000000)
#define BOOTRAM_BOOTLOCK5_MSB    _u(31)
#define BOOTRAM_BOOTLOCK5_LSB    _u(0)
#define BOOTRAM_BOOTLOCK5_ACCESS "RW"
// =============================================================================
// Register    : BOOTRAM_BOOTLOCK6
// Description : Read to claim and check. Write to unclaim. The value returned
//               on successful claim is 1 << n, and on failed claim is zero.
#define BOOTRAM_BOOTLOCK6_OFFSET _u(0x00000824)
#define BOOTRAM_BOOTLOCK6_BITS   _u(0xffffffff)
#define BOOTRAM_BOOTLOCK6_RESET  _u(0x00000000)
#define BOOTRAM_BOOTLOCK6_MSB    _u(31)
#define BOOTRAM_BOOTLOCK6_LSB    _u(0)
#define BOOTRAM_BOOTLOCK6_ACCESS "RW"
// =============================================================================
// Register    : BOOTRAM_BOOTLOCK7
// Description : Read to claim and check. Write to unclaim. The value returned
//               on successful claim is 1 << n, and on failed claim is zero.
#define BOOTRAM_BOOTLOCK7_OFFSET _u(0x00000828)
#define BOOTRAM_BOOTLOCK7_BITS   _u(0xffffffff)
#define BOOTRAM_BOOTLOCK7_RESET  _u(0x00000000)
#define BOOTRAM_BOOTLOCK7_MSB    _u(31)
#define BOOTRAM_BOOTLOCK7_LSB    _u(0)
#define BOOTRAM_BOOTLOCK7_ACCESS "RW"
// =============================================================================
#endif // _HARDWARE_REGS_BOOTRAM_H

