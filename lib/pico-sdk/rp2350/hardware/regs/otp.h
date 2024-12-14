// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
// =============================================================================
// Register block : OTP
// Version        : 1
// Bus type       : apb
// Description    : SNPS OTP control IF (SBPI and RPi wrapper control)
// =============================================================================
#ifndef _HARDWARE_REGS_OTP_H
#define _HARDWARE_REGS_OTP_H
// =============================================================================
// Register    : OTP_SW_LOCK0
// Description : Software lock register for page 0.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK0_OFFSET _u(0x00000000)
#define OTP_SW_LOCK0_BITS   _u(0x0000000f)
#define OTP_SW_LOCK0_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK0_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK0_NSEC_RESET  "-"
#define OTP_SW_LOCK0_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK0_NSEC_MSB    _u(3)
#define OTP_SW_LOCK0_NSEC_LSB    _u(2)
#define OTP_SW_LOCK0_NSEC_ACCESS "RW"
#define OTP_SW_LOCK0_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK0_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK0_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK0_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK0_SEC_RESET  "-"
#define OTP_SW_LOCK0_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK0_SEC_MSB    _u(1)
#define OTP_SW_LOCK0_SEC_LSB    _u(0)
#define OTP_SW_LOCK0_SEC_ACCESS "RW"
#define OTP_SW_LOCK0_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK0_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK0_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK1
// Description : Software lock register for page 1.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK1_OFFSET _u(0x00000004)
#define OTP_SW_LOCK1_BITS   _u(0x0000000f)
#define OTP_SW_LOCK1_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK1_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK1_NSEC_RESET  "-"
#define OTP_SW_LOCK1_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK1_NSEC_MSB    _u(3)
#define OTP_SW_LOCK1_NSEC_LSB    _u(2)
#define OTP_SW_LOCK1_NSEC_ACCESS "RW"
#define OTP_SW_LOCK1_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK1_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK1_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK1_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK1_SEC_RESET  "-"
#define OTP_SW_LOCK1_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK1_SEC_MSB    _u(1)
#define OTP_SW_LOCK1_SEC_LSB    _u(0)
#define OTP_SW_LOCK1_SEC_ACCESS "RW"
#define OTP_SW_LOCK1_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK1_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK1_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK2
// Description : Software lock register for page 2.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK2_OFFSET _u(0x00000008)
#define OTP_SW_LOCK2_BITS   _u(0x0000000f)
#define OTP_SW_LOCK2_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK2_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK2_NSEC_RESET  "-"
#define OTP_SW_LOCK2_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK2_NSEC_MSB    _u(3)
#define OTP_SW_LOCK2_NSEC_LSB    _u(2)
#define OTP_SW_LOCK2_NSEC_ACCESS "RW"
#define OTP_SW_LOCK2_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK2_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK2_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK2_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK2_SEC_RESET  "-"
#define OTP_SW_LOCK2_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK2_SEC_MSB    _u(1)
#define OTP_SW_LOCK2_SEC_LSB    _u(0)
#define OTP_SW_LOCK2_SEC_ACCESS "RW"
#define OTP_SW_LOCK2_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK2_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK2_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK3
// Description : Software lock register for page 3.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK3_OFFSET _u(0x0000000c)
#define OTP_SW_LOCK3_BITS   _u(0x0000000f)
#define OTP_SW_LOCK3_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK3_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK3_NSEC_RESET  "-"
#define OTP_SW_LOCK3_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK3_NSEC_MSB    _u(3)
#define OTP_SW_LOCK3_NSEC_LSB    _u(2)
#define OTP_SW_LOCK3_NSEC_ACCESS "RW"
#define OTP_SW_LOCK3_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK3_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK3_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK3_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK3_SEC_RESET  "-"
#define OTP_SW_LOCK3_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK3_SEC_MSB    _u(1)
#define OTP_SW_LOCK3_SEC_LSB    _u(0)
#define OTP_SW_LOCK3_SEC_ACCESS "RW"
#define OTP_SW_LOCK3_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK3_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK3_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK4
// Description : Software lock register for page 4.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK4_OFFSET _u(0x00000010)
#define OTP_SW_LOCK4_BITS   _u(0x0000000f)
#define OTP_SW_LOCK4_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK4_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK4_NSEC_RESET  "-"
#define OTP_SW_LOCK4_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK4_NSEC_MSB    _u(3)
#define OTP_SW_LOCK4_NSEC_LSB    _u(2)
#define OTP_SW_LOCK4_NSEC_ACCESS "RW"
#define OTP_SW_LOCK4_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK4_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK4_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK4_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK4_SEC_RESET  "-"
#define OTP_SW_LOCK4_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK4_SEC_MSB    _u(1)
#define OTP_SW_LOCK4_SEC_LSB    _u(0)
#define OTP_SW_LOCK4_SEC_ACCESS "RW"
#define OTP_SW_LOCK4_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK4_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK4_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK5
// Description : Software lock register for page 5.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK5_OFFSET _u(0x00000014)
#define OTP_SW_LOCK5_BITS   _u(0x0000000f)
#define OTP_SW_LOCK5_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK5_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK5_NSEC_RESET  "-"
#define OTP_SW_LOCK5_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK5_NSEC_MSB    _u(3)
#define OTP_SW_LOCK5_NSEC_LSB    _u(2)
#define OTP_SW_LOCK5_NSEC_ACCESS "RW"
#define OTP_SW_LOCK5_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK5_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK5_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK5_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK5_SEC_RESET  "-"
#define OTP_SW_LOCK5_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK5_SEC_MSB    _u(1)
#define OTP_SW_LOCK5_SEC_LSB    _u(0)
#define OTP_SW_LOCK5_SEC_ACCESS "RW"
#define OTP_SW_LOCK5_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK5_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK5_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK6
// Description : Software lock register for page 6.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK6_OFFSET _u(0x00000018)
#define OTP_SW_LOCK6_BITS   _u(0x0000000f)
#define OTP_SW_LOCK6_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK6_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK6_NSEC_RESET  "-"
#define OTP_SW_LOCK6_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK6_NSEC_MSB    _u(3)
#define OTP_SW_LOCK6_NSEC_LSB    _u(2)
#define OTP_SW_LOCK6_NSEC_ACCESS "RW"
#define OTP_SW_LOCK6_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK6_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK6_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK6_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK6_SEC_RESET  "-"
#define OTP_SW_LOCK6_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK6_SEC_MSB    _u(1)
#define OTP_SW_LOCK6_SEC_LSB    _u(0)
#define OTP_SW_LOCK6_SEC_ACCESS "RW"
#define OTP_SW_LOCK6_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK6_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK6_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK7
// Description : Software lock register for page 7.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK7_OFFSET _u(0x0000001c)
#define OTP_SW_LOCK7_BITS   _u(0x0000000f)
#define OTP_SW_LOCK7_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK7_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK7_NSEC_RESET  "-"
#define OTP_SW_LOCK7_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK7_NSEC_MSB    _u(3)
#define OTP_SW_LOCK7_NSEC_LSB    _u(2)
#define OTP_SW_LOCK7_NSEC_ACCESS "RW"
#define OTP_SW_LOCK7_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK7_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK7_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK7_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK7_SEC_RESET  "-"
#define OTP_SW_LOCK7_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK7_SEC_MSB    _u(1)
#define OTP_SW_LOCK7_SEC_LSB    _u(0)
#define OTP_SW_LOCK7_SEC_ACCESS "RW"
#define OTP_SW_LOCK7_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK7_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK7_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK8
// Description : Software lock register for page 8.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK8_OFFSET _u(0x00000020)
#define OTP_SW_LOCK8_BITS   _u(0x0000000f)
#define OTP_SW_LOCK8_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK8_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK8_NSEC_RESET  "-"
#define OTP_SW_LOCK8_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK8_NSEC_MSB    _u(3)
#define OTP_SW_LOCK8_NSEC_LSB    _u(2)
#define OTP_SW_LOCK8_NSEC_ACCESS "RW"
#define OTP_SW_LOCK8_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK8_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK8_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK8_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK8_SEC_RESET  "-"
#define OTP_SW_LOCK8_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK8_SEC_MSB    _u(1)
#define OTP_SW_LOCK8_SEC_LSB    _u(0)
#define OTP_SW_LOCK8_SEC_ACCESS "RW"
#define OTP_SW_LOCK8_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK8_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK8_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK9
// Description : Software lock register for page 9.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK9_OFFSET _u(0x00000024)
#define OTP_SW_LOCK9_BITS   _u(0x0000000f)
#define OTP_SW_LOCK9_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK9_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK9_NSEC_RESET  "-"
#define OTP_SW_LOCK9_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK9_NSEC_MSB    _u(3)
#define OTP_SW_LOCK9_NSEC_LSB    _u(2)
#define OTP_SW_LOCK9_NSEC_ACCESS "RW"
#define OTP_SW_LOCK9_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK9_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK9_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK9_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK9_SEC_RESET  "-"
#define OTP_SW_LOCK9_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK9_SEC_MSB    _u(1)
#define OTP_SW_LOCK9_SEC_LSB    _u(0)
#define OTP_SW_LOCK9_SEC_ACCESS "RW"
#define OTP_SW_LOCK9_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK9_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK9_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK10
// Description : Software lock register for page 10.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK10_OFFSET _u(0x00000028)
#define OTP_SW_LOCK10_BITS   _u(0x0000000f)
#define OTP_SW_LOCK10_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK10_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK10_NSEC_RESET  "-"
#define OTP_SW_LOCK10_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK10_NSEC_MSB    _u(3)
#define OTP_SW_LOCK10_NSEC_LSB    _u(2)
#define OTP_SW_LOCK10_NSEC_ACCESS "RW"
#define OTP_SW_LOCK10_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK10_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK10_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK10_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK10_SEC_RESET  "-"
#define OTP_SW_LOCK10_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK10_SEC_MSB    _u(1)
#define OTP_SW_LOCK10_SEC_LSB    _u(0)
#define OTP_SW_LOCK10_SEC_ACCESS "RW"
#define OTP_SW_LOCK10_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK10_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK10_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK11
// Description : Software lock register for page 11.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK11_OFFSET _u(0x0000002c)
#define OTP_SW_LOCK11_BITS   _u(0x0000000f)
#define OTP_SW_LOCK11_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK11_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK11_NSEC_RESET  "-"
#define OTP_SW_LOCK11_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK11_NSEC_MSB    _u(3)
#define OTP_SW_LOCK11_NSEC_LSB    _u(2)
#define OTP_SW_LOCK11_NSEC_ACCESS "RW"
#define OTP_SW_LOCK11_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK11_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK11_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK11_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK11_SEC_RESET  "-"
#define OTP_SW_LOCK11_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK11_SEC_MSB    _u(1)
#define OTP_SW_LOCK11_SEC_LSB    _u(0)
#define OTP_SW_LOCK11_SEC_ACCESS "RW"
#define OTP_SW_LOCK11_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK11_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK11_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK12
// Description : Software lock register for page 12.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK12_OFFSET _u(0x00000030)
#define OTP_SW_LOCK12_BITS   _u(0x0000000f)
#define OTP_SW_LOCK12_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK12_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK12_NSEC_RESET  "-"
#define OTP_SW_LOCK12_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK12_NSEC_MSB    _u(3)
#define OTP_SW_LOCK12_NSEC_LSB    _u(2)
#define OTP_SW_LOCK12_NSEC_ACCESS "RW"
#define OTP_SW_LOCK12_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK12_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK12_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK12_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK12_SEC_RESET  "-"
#define OTP_SW_LOCK12_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK12_SEC_MSB    _u(1)
#define OTP_SW_LOCK12_SEC_LSB    _u(0)
#define OTP_SW_LOCK12_SEC_ACCESS "RW"
#define OTP_SW_LOCK12_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK12_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK12_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK13
// Description : Software lock register for page 13.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK13_OFFSET _u(0x00000034)
#define OTP_SW_LOCK13_BITS   _u(0x0000000f)
#define OTP_SW_LOCK13_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK13_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK13_NSEC_RESET  "-"
#define OTP_SW_LOCK13_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK13_NSEC_MSB    _u(3)
#define OTP_SW_LOCK13_NSEC_LSB    _u(2)
#define OTP_SW_LOCK13_NSEC_ACCESS "RW"
#define OTP_SW_LOCK13_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK13_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK13_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK13_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK13_SEC_RESET  "-"
#define OTP_SW_LOCK13_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK13_SEC_MSB    _u(1)
#define OTP_SW_LOCK13_SEC_LSB    _u(0)
#define OTP_SW_LOCK13_SEC_ACCESS "RW"
#define OTP_SW_LOCK13_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK13_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK13_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK14
// Description : Software lock register for page 14.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK14_OFFSET _u(0x00000038)
#define OTP_SW_LOCK14_BITS   _u(0x0000000f)
#define OTP_SW_LOCK14_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK14_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK14_NSEC_RESET  "-"
#define OTP_SW_LOCK14_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK14_NSEC_MSB    _u(3)
#define OTP_SW_LOCK14_NSEC_LSB    _u(2)
#define OTP_SW_LOCK14_NSEC_ACCESS "RW"
#define OTP_SW_LOCK14_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK14_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK14_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK14_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK14_SEC_RESET  "-"
#define OTP_SW_LOCK14_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK14_SEC_MSB    _u(1)
#define OTP_SW_LOCK14_SEC_LSB    _u(0)
#define OTP_SW_LOCK14_SEC_ACCESS "RW"
#define OTP_SW_LOCK14_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK14_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK14_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK15
// Description : Software lock register for page 15.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK15_OFFSET _u(0x0000003c)
#define OTP_SW_LOCK15_BITS   _u(0x0000000f)
#define OTP_SW_LOCK15_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK15_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK15_NSEC_RESET  "-"
#define OTP_SW_LOCK15_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK15_NSEC_MSB    _u(3)
#define OTP_SW_LOCK15_NSEC_LSB    _u(2)
#define OTP_SW_LOCK15_NSEC_ACCESS "RW"
#define OTP_SW_LOCK15_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK15_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK15_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK15_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK15_SEC_RESET  "-"
#define OTP_SW_LOCK15_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK15_SEC_MSB    _u(1)
#define OTP_SW_LOCK15_SEC_LSB    _u(0)
#define OTP_SW_LOCK15_SEC_ACCESS "RW"
#define OTP_SW_LOCK15_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK15_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK15_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK16
// Description : Software lock register for page 16.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK16_OFFSET _u(0x00000040)
#define OTP_SW_LOCK16_BITS   _u(0x0000000f)
#define OTP_SW_LOCK16_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK16_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK16_NSEC_RESET  "-"
#define OTP_SW_LOCK16_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK16_NSEC_MSB    _u(3)
#define OTP_SW_LOCK16_NSEC_LSB    _u(2)
#define OTP_SW_LOCK16_NSEC_ACCESS "RW"
#define OTP_SW_LOCK16_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK16_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK16_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK16_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK16_SEC_RESET  "-"
#define OTP_SW_LOCK16_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK16_SEC_MSB    _u(1)
#define OTP_SW_LOCK16_SEC_LSB    _u(0)
#define OTP_SW_LOCK16_SEC_ACCESS "RW"
#define OTP_SW_LOCK16_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK16_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK16_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK17
// Description : Software lock register for page 17.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK17_OFFSET _u(0x00000044)
#define OTP_SW_LOCK17_BITS   _u(0x0000000f)
#define OTP_SW_LOCK17_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK17_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK17_NSEC_RESET  "-"
#define OTP_SW_LOCK17_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK17_NSEC_MSB    _u(3)
#define OTP_SW_LOCK17_NSEC_LSB    _u(2)
#define OTP_SW_LOCK17_NSEC_ACCESS "RW"
#define OTP_SW_LOCK17_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK17_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK17_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK17_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK17_SEC_RESET  "-"
#define OTP_SW_LOCK17_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK17_SEC_MSB    _u(1)
#define OTP_SW_LOCK17_SEC_LSB    _u(0)
#define OTP_SW_LOCK17_SEC_ACCESS "RW"
#define OTP_SW_LOCK17_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK17_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK17_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK18
// Description : Software lock register for page 18.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK18_OFFSET _u(0x00000048)
#define OTP_SW_LOCK18_BITS   _u(0x0000000f)
#define OTP_SW_LOCK18_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK18_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK18_NSEC_RESET  "-"
#define OTP_SW_LOCK18_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK18_NSEC_MSB    _u(3)
#define OTP_SW_LOCK18_NSEC_LSB    _u(2)
#define OTP_SW_LOCK18_NSEC_ACCESS "RW"
#define OTP_SW_LOCK18_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK18_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK18_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK18_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK18_SEC_RESET  "-"
#define OTP_SW_LOCK18_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK18_SEC_MSB    _u(1)
#define OTP_SW_LOCK18_SEC_LSB    _u(0)
#define OTP_SW_LOCK18_SEC_ACCESS "RW"
#define OTP_SW_LOCK18_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK18_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK18_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK19
// Description : Software lock register for page 19.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK19_OFFSET _u(0x0000004c)
#define OTP_SW_LOCK19_BITS   _u(0x0000000f)
#define OTP_SW_LOCK19_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK19_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK19_NSEC_RESET  "-"
#define OTP_SW_LOCK19_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK19_NSEC_MSB    _u(3)
#define OTP_SW_LOCK19_NSEC_LSB    _u(2)
#define OTP_SW_LOCK19_NSEC_ACCESS "RW"
#define OTP_SW_LOCK19_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK19_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK19_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK19_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK19_SEC_RESET  "-"
#define OTP_SW_LOCK19_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK19_SEC_MSB    _u(1)
#define OTP_SW_LOCK19_SEC_LSB    _u(0)
#define OTP_SW_LOCK19_SEC_ACCESS "RW"
#define OTP_SW_LOCK19_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK19_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK19_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK20
// Description : Software lock register for page 20.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK20_OFFSET _u(0x00000050)
#define OTP_SW_LOCK20_BITS   _u(0x0000000f)
#define OTP_SW_LOCK20_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK20_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK20_NSEC_RESET  "-"
#define OTP_SW_LOCK20_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK20_NSEC_MSB    _u(3)
#define OTP_SW_LOCK20_NSEC_LSB    _u(2)
#define OTP_SW_LOCK20_NSEC_ACCESS "RW"
#define OTP_SW_LOCK20_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK20_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK20_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK20_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK20_SEC_RESET  "-"
#define OTP_SW_LOCK20_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK20_SEC_MSB    _u(1)
#define OTP_SW_LOCK20_SEC_LSB    _u(0)
#define OTP_SW_LOCK20_SEC_ACCESS "RW"
#define OTP_SW_LOCK20_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK20_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK20_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK21
// Description : Software lock register for page 21.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK21_OFFSET _u(0x00000054)
#define OTP_SW_LOCK21_BITS   _u(0x0000000f)
#define OTP_SW_LOCK21_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK21_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK21_NSEC_RESET  "-"
#define OTP_SW_LOCK21_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK21_NSEC_MSB    _u(3)
#define OTP_SW_LOCK21_NSEC_LSB    _u(2)
#define OTP_SW_LOCK21_NSEC_ACCESS "RW"
#define OTP_SW_LOCK21_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK21_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK21_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK21_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK21_SEC_RESET  "-"
#define OTP_SW_LOCK21_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK21_SEC_MSB    _u(1)
#define OTP_SW_LOCK21_SEC_LSB    _u(0)
#define OTP_SW_LOCK21_SEC_ACCESS "RW"
#define OTP_SW_LOCK21_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK21_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK21_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK22
// Description : Software lock register for page 22.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK22_OFFSET _u(0x00000058)
#define OTP_SW_LOCK22_BITS   _u(0x0000000f)
#define OTP_SW_LOCK22_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK22_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK22_NSEC_RESET  "-"
#define OTP_SW_LOCK22_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK22_NSEC_MSB    _u(3)
#define OTP_SW_LOCK22_NSEC_LSB    _u(2)
#define OTP_SW_LOCK22_NSEC_ACCESS "RW"
#define OTP_SW_LOCK22_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK22_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK22_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK22_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK22_SEC_RESET  "-"
#define OTP_SW_LOCK22_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK22_SEC_MSB    _u(1)
#define OTP_SW_LOCK22_SEC_LSB    _u(0)
#define OTP_SW_LOCK22_SEC_ACCESS "RW"
#define OTP_SW_LOCK22_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK22_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK22_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK23
// Description : Software lock register for page 23.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK23_OFFSET _u(0x0000005c)
#define OTP_SW_LOCK23_BITS   _u(0x0000000f)
#define OTP_SW_LOCK23_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK23_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK23_NSEC_RESET  "-"
#define OTP_SW_LOCK23_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK23_NSEC_MSB    _u(3)
#define OTP_SW_LOCK23_NSEC_LSB    _u(2)
#define OTP_SW_LOCK23_NSEC_ACCESS "RW"
#define OTP_SW_LOCK23_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK23_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK23_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK23_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK23_SEC_RESET  "-"
#define OTP_SW_LOCK23_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK23_SEC_MSB    _u(1)
#define OTP_SW_LOCK23_SEC_LSB    _u(0)
#define OTP_SW_LOCK23_SEC_ACCESS "RW"
#define OTP_SW_LOCK23_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK23_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK23_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK24
// Description : Software lock register for page 24.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK24_OFFSET _u(0x00000060)
#define OTP_SW_LOCK24_BITS   _u(0x0000000f)
#define OTP_SW_LOCK24_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK24_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK24_NSEC_RESET  "-"
#define OTP_SW_LOCK24_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK24_NSEC_MSB    _u(3)
#define OTP_SW_LOCK24_NSEC_LSB    _u(2)
#define OTP_SW_LOCK24_NSEC_ACCESS "RW"
#define OTP_SW_LOCK24_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK24_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK24_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK24_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK24_SEC_RESET  "-"
#define OTP_SW_LOCK24_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK24_SEC_MSB    _u(1)
#define OTP_SW_LOCK24_SEC_LSB    _u(0)
#define OTP_SW_LOCK24_SEC_ACCESS "RW"
#define OTP_SW_LOCK24_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK24_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK24_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK25
// Description : Software lock register for page 25.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK25_OFFSET _u(0x00000064)
#define OTP_SW_LOCK25_BITS   _u(0x0000000f)
#define OTP_SW_LOCK25_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK25_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK25_NSEC_RESET  "-"
#define OTP_SW_LOCK25_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK25_NSEC_MSB    _u(3)
#define OTP_SW_LOCK25_NSEC_LSB    _u(2)
#define OTP_SW_LOCK25_NSEC_ACCESS "RW"
#define OTP_SW_LOCK25_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK25_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK25_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK25_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK25_SEC_RESET  "-"
#define OTP_SW_LOCK25_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK25_SEC_MSB    _u(1)
#define OTP_SW_LOCK25_SEC_LSB    _u(0)
#define OTP_SW_LOCK25_SEC_ACCESS "RW"
#define OTP_SW_LOCK25_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK25_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK25_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK26
// Description : Software lock register for page 26.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK26_OFFSET _u(0x00000068)
#define OTP_SW_LOCK26_BITS   _u(0x0000000f)
#define OTP_SW_LOCK26_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK26_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK26_NSEC_RESET  "-"
#define OTP_SW_LOCK26_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK26_NSEC_MSB    _u(3)
#define OTP_SW_LOCK26_NSEC_LSB    _u(2)
#define OTP_SW_LOCK26_NSEC_ACCESS "RW"
#define OTP_SW_LOCK26_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK26_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK26_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK26_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK26_SEC_RESET  "-"
#define OTP_SW_LOCK26_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK26_SEC_MSB    _u(1)
#define OTP_SW_LOCK26_SEC_LSB    _u(0)
#define OTP_SW_LOCK26_SEC_ACCESS "RW"
#define OTP_SW_LOCK26_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK26_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK26_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK27
// Description : Software lock register for page 27.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK27_OFFSET _u(0x0000006c)
#define OTP_SW_LOCK27_BITS   _u(0x0000000f)
#define OTP_SW_LOCK27_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK27_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK27_NSEC_RESET  "-"
#define OTP_SW_LOCK27_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK27_NSEC_MSB    _u(3)
#define OTP_SW_LOCK27_NSEC_LSB    _u(2)
#define OTP_SW_LOCK27_NSEC_ACCESS "RW"
#define OTP_SW_LOCK27_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK27_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK27_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK27_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK27_SEC_RESET  "-"
#define OTP_SW_LOCK27_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK27_SEC_MSB    _u(1)
#define OTP_SW_LOCK27_SEC_LSB    _u(0)
#define OTP_SW_LOCK27_SEC_ACCESS "RW"
#define OTP_SW_LOCK27_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK27_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK27_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK28
// Description : Software lock register for page 28.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK28_OFFSET _u(0x00000070)
#define OTP_SW_LOCK28_BITS   _u(0x0000000f)
#define OTP_SW_LOCK28_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK28_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK28_NSEC_RESET  "-"
#define OTP_SW_LOCK28_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK28_NSEC_MSB    _u(3)
#define OTP_SW_LOCK28_NSEC_LSB    _u(2)
#define OTP_SW_LOCK28_NSEC_ACCESS "RW"
#define OTP_SW_LOCK28_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK28_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK28_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK28_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK28_SEC_RESET  "-"
#define OTP_SW_LOCK28_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK28_SEC_MSB    _u(1)
#define OTP_SW_LOCK28_SEC_LSB    _u(0)
#define OTP_SW_LOCK28_SEC_ACCESS "RW"
#define OTP_SW_LOCK28_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK28_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK28_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK29
// Description : Software lock register for page 29.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK29_OFFSET _u(0x00000074)
#define OTP_SW_LOCK29_BITS   _u(0x0000000f)
#define OTP_SW_LOCK29_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK29_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK29_NSEC_RESET  "-"
#define OTP_SW_LOCK29_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK29_NSEC_MSB    _u(3)
#define OTP_SW_LOCK29_NSEC_LSB    _u(2)
#define OTP_SW_LOCK29_NSEC_ACCESS "RW"
#define OTP_SW_LOCK29_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK29_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK29_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK29_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK29_SEC_RESET  "-"
#define OTP_SW_LOCK29_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK29_SEC_MSB    _u(1)
#define OTP_SW_LOCK29_SEC_LSB    _u(0)
#define OTP_SW_LOCK29_SEC_ACCESS "RW"
#define OTP_SW_LOCK29_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK29_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK29_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK30
// Description : Software lock register for page 30.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK30_OFFSET _u(0x00000078)
#define OTP_SW_LOCK30_BITS   _u(0x0000000f)
#define OTP_SW_LOCK30_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK30_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK30_NSEC_RESET  "-"
#define OTP_SW_LOCK30_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK30_NSEC_MSB    _u(3)
#define OTP_SW_LOCK30_NSEC_LSB    _u(2)
#define OTP_SW_LOCK30_NSEC_ACCESS "RW"
#define OTP_SW_LOCK30_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK30_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK30_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK30_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK30_SEC_RESET  "-"
#define OTP_SW_LOCK30_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK30_SEC_MSB    _u(1)
#define OTP_SW_LOCK30_SEC_LSB    _u(0)
#define OTP_SW_LOCK30_SEC_ACCESS "RW"
#define OTP_SW_LOCK30_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK30_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK30_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK31
// Description : Software lock register for page 31.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK31_OFFSET _u(0x0000007c)
#define OTP_SW_LOCK31_BITS   _u(0x0000000f)
#define OTP_SW_LOCK31_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK31_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK31_NSEC_RESET  "-"
#define OTP_SW_LOCK31_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK31_NSEC_MSB    _u(3)
#define OTP_SW_LOCK31_NSEC_LSB    _u(2)
#define OTP_SW_LOCK31_NSEC_ACCESS "RW"
#define OTP_SW_LOCK31_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK31_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK31_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK31_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK31_SEC_RESET  "-"
#define OTP_SW_LOCK31_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK31_SEC_MSB    _u(1)
#define OTP_SW_LOCK31_SEC_LSB    _u(0)
#define OTP_SW_LOCK31_SEC_ACCESS "RW"
#define OTP_SW_LOCK31_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK31_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK31_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK32
// Description : Software lock register for page 32.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK32_OFFSET _u(0x00000080)
#define OTP_SW_LOCK32_BITS   _u(0x0000000f)
#define OTP_SW_LOCK32_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK32_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK32_NSEC_RESET  "-"
#define OTP_SW_LOCK32_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK32_NSEC_MSB    _u(3)
#define OTP_SW_LOCK32_NSEC_LSB    _u(2)
#define OTP_SW_LOCK32_NSEC_ACCESS "RW"
#define OTP_SW_LOCK32_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK32_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK32_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK32_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK32_SEC_RESET  "-"
#define OTP_SW_LOCK32_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK32_SEC_MSB    _u(1)
#define OTP_SW_LOCK32_SEC_LSB    _u(0)
#define OTP_SW_LOCK32_SEC_ACCESS "RW"
#define OTP_SW_LOCK32_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK32_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK32_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK33
// Description : Software lock register for page 33.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK33_OFFSET _u(0x00000084)
#define OTP_SW_LOCK33_BITS   _u(0x0000000f)
#define OTP_SW_LOCK33_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK33_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK33_NSEC_RESET  "-"
#define OTP_SW_LOCK33_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK33_NSEC_MSB    _u(3)
#define OTP_SW_LOCK33_NSEC_LSB    _u(2)
#define OTP_SW_LOCK33_NSEC_ACCESS "RW"
#define OTP_SW_LOCK33_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK33_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK33_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK33_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK33_SEC_RESET  "-"
#define OTP_SW_LOCK33_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK33_SEC_MSB    _u(1)
#define OTP_SW_LOCK33_SEC_LSB    _u(0)
#define OTP_SW_LOCK33_SEC_ACCESS "RW"
#define OTP_SW_LOCK33_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK33_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK33_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK34
// Description : Software lock register for page 34.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK34_OFFSET _u(0x00000088)
#define OTP_SW_LOCK34_BITS   _u(0x0000000f)
#define OTP_SW_LOCK34_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK34_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK34_NSEC_RESET  "-"
#define OTP_SW_LOCK34_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK34_NSEC_MSB    _u(3)
#define OTP_SW_LOCK34_NSEC_LSB    _u(2)
#define OTP_SW_LOCK34_NSEC_ACCESS "RW"
#define OTP_SW_LOCK34_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK34_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK34_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK34_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK34_SEC_RESET  "-"
#define OTP_SW_LOCK34_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK34_SEC_MSB    _u(1)
#define OTP_SW_LOCK34_SEC_LSB    _u(0)
#define OTP_SW_LOCK34_SEC_ACCESS "RW"
#define OTP_SW_LOCK34_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK34_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK34_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK35
// Description : Software lock register for page 35.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK35_OFFSET _u(0x0000008c)
#define OTP_SW_LOCK35_BITS   _u(0x0000000f)
#define OTP_SW_LOCK35_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK35_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK35_NSEC_RESET  "-"
#define OTP_SW_LOCK35_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK35_NSEC_MSB    _u(3)
#define OTP_SW_LOCK35_NSEC_LSB    _u(2)
#define OTP_SW_LOCK35_NSEC_ACCESS "RW"
#define OTP_SW_LOCK35_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK35_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK35_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK35_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK35_SEC_RESET  "-"
#define OTP_SW_LOCK35_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK35_SEC_MSB    _u(1)
#define OTP_SW_LOCK35_SEC_LSB    _u(0)
#define OTP_SW_LOCK35_SEC_ACCESS "RW"
#define OTP_SW_LOCK35_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK35_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK35_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK36
// Description : Software lock register for page 36.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK36_OFFSET _u(0x00000090)
#define OTP_SW_LOCK36_BITS   _u(0x0000000f)
#define OTP_SW_LOCK36_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK36_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK36_NSEC_RESET  "-"
#define OTP_SW_LOCK36_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK36_NSEC_MSB    _u(3)
#define OTP_SW_LOCK36_NSEC_LSB    _u(2)
#define OTP_SW_LOCK36_NSEC_ACCESS "RW"
#define OTP_SW_LOCK36_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK36_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK36_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK36_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK36_SEC_RESET  "-"
#define OTP_SW_LOCK36_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK36_SEC_MSB    _u(1)
#define OTP_SW_LOCK36_SEC_LSB    _u(0)
#define OTP_SW_LOCK36_SEC_ACCESS "RW"
#define OTP_SW_LOCK36_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK36_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK36_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK37
// Description : Software lock register for page 37.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK37_OFFSET _u(0x00000094)
#define OTP_SW_LOCK37_BITS   _u(0x0000000f)
#define OTP_SW_LOCK37_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK37_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK37_NSEC_RESET  "-"
#define OTP_SW_LOCK37_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK37_NSEC_MSB    _u(3)
#define OTP_SW_LOCK37_NSEC_LSB    _u(2)
#define OTP_SW_LOCK37_NSEC_ACCESS "RW"
#define OTP_SW_LOCK37_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK37_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK37_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK37_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK37_SEC_RESET  "-"
#define OTP_SW_LOCK37_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK37_SEC_MSB    _u(1)
#define OTP_SW_LOCK37_SEC_LSB    _u(0)
#define OTP_SW_LOCK37_SEC_ACCESS "RW"
#define OTP_SW_LOCK37_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK37_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK37_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK38
// Description : Software lock register for page 38.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK38_OFFSET _u(0x00000098)
#define OTP_SW_LOCK38_BITS   _u(0x0000000f)
#define OTP_SW_LOCK38_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK38_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK38_NSEC_RESET  "-"
#define OTP_SW_LOCK38_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK38_NSEC_MSB    _u(3)
#define OTP_SW_LOCK38_NSEC_LSB    _u(2)
#define OTP_SW_LOCK38_NSEC_ACCESS "RW"
#define OTP_SW_LOCK38_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK38_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK38_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK38_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK38_SEC_RESET  "-"
#define OTP_SW_LOCK38_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK38_SEC_MSB    _u(1)
#define OTP_SW_LOCK38_SEC_LSB    _u(0)
#define OTP_SW_LOCK38_SEC_ACCESS "RW"
#define OTP_SW_LOCK38_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK38_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK38_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK39
// Description : Software lock register for page 39.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK39_OFFSET _u(0x0000009c)
#define OTP_SW_LOCK39_BITS   _u(0x0000000f)
#define OTP_SW_LOCK39_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK39_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK39_NSEC_RESET  "-"
#define OTP_SW_LOCK39_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK39_NSEC_MSB    _u(3)
#define OTP_SW_LOCK39_NSEC_LSB    _u(2)
#define OTP_SW_LOCK39_NSEC_ACCESS "RW"
#define OTP_SW_LOCK39_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK39_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK39_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK39_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK39_SEC_RESET  "-"
#define OTP_SW_LOCK39_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK39_SEC_MSB    _u(1)
#define OTP_SW_LOCK39_SEC_LSB    _u(0)
#define OTP_SW_LOCK39_SEC_ACCESS "RW"
#define OTP_SW_LOCK39_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK39_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK39_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK40
// Description : Software lock register for page 40.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK40_OFFSET _u(0x000000a0)
#define OTP_SW_LOCK40_BITS   _u(0x0000000f)
#define OTP_SW_LOCK40_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK40_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK40_NSEC_RESET  "-"
#define OTP_SW_LOCK40_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK40_NSEC_MSB    _u(3)
#define OTP_SW_LOCK40_NSEC_LSB    _u(2)
#define OTP_SW_LOCK40_NSEC_ACCESS "RW"
#define OTP_SW_LOCK40_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK40_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK40_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK40_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK40_SEC_RESET  "-"
#define OTP_SW_LOCK40_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK40_SEC_MSB    _u(1)
#define OTP_SW_LOCK40_SEC_LSB    _u(0)
#define OTP_SW_LOCK40_SEC_ACCESS "RW"
#define OTP_SW_LOCK40_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK40_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK40_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK41
// Description : Software lock register for page 41.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK41_OFFSET _u(0x000000a4)
#define OTP_SW_LOCK41_BITS   _u(0x0000000f)
#define OTP_SW_LOCK41_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK41_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK41_NSEC_RESET  "-"
#define OTP_SW_LOCK41_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK41_NSEC_MSB    _u(3)
#define OTP_SW_LOCK41_NSEC_LSB    _u(2)
#define OTP_SW_LOCK41_NSEC_ACCESS "RW"
#define OTP_SW_LOCK41_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK41_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK41_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK41_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK41_SEC_RESET  "-"
#define OTP_SW_LOCK41_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK41_SEC_MSB    _u(1)
#define OTP_SW_LOCK41_SEC_LSB    _u(0)
#define OTP_SW_LOCK41_SEC_ACCESS "RW"
#define OTP_SW_LOCK41_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK41_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK41_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK42
// Description : Software lock register for page 42.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK42_OFFSET _u(0x000000a8)
#define OTP_SW_LOCK42_BITS   _u(0x0000000f)
#define OTP_SW_LOCK42_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK42_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK42_NSEC_RESET  "-"
#define OTP_SW_LOCK42_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK42_NSEC_MSB    _u(3)
#define OTP_SW_LOCK42_NSEC_LSB    _u(2)
#define OTP_SW_LOCK42_NSEC_ACCESS "RW"
#define OTP_SW_LOCK42_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK42_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK42_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK42_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK42_SEC_RESET  "-"
#define OTP_SW_LOCK42_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK42_SEC_MSB    _u(1)
#define OTP_SW_LOCK42_SEC_LSB    _u(0)
#define OTP_SW_LOCK42_SEC_ACCESS "RW"
#define OTP_SW_LOCK42_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK42_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK42_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK43
// Description : Software lock register for page 43.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK43_OFFSET _u(0x000000ac)
#define OTP_SW_LOCK43_BITS   _u(0x0000000f)
#define OTP_SW_LOCK43_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK43_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK43_NSEC_RESET  "-"
#define OTP_SW_LOCK43_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK43_NSEC_MSB    _u(3)
#define OTP_SW_LOCK43_NSEC_LSB    _u(2)
#define OTP_SW_LOCK43_NSEC_ACCESS "RW"
#define OTP_SW_LOCK43_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK43_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK43_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK43_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK43_SEC_RESET  "-"
#define OTP_SW_LOCK43_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK43_SEC_MSB    _u(1)
#define OTP_SW_LOCK43_SEC_LSB    _u(0)
#define OTP_SW_LOCK43_SEC_ACCESS "RW"
#define OTP_SW_LOCK43_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK43_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK43_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK44
// Description : Software lock register for page 44.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK44_OFFSET _u(0x000000b0)
#define OTP_SW_LOCK44_BITS   _u(0x0000000f)
#define OTP_SW_LOCK44_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK44_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK44_NSEC_RESET  "-"
#define OTP_SW_LOCK44_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK44_NSEC_MSB    _u(3)
#define OTP_SW_LOCK44_NSEC_LSB    _u(2)
#define OTP_SW_LOCK44_NSEC_ACCESS "RW"
#define OTP_SW_LOCK44_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK44_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK44_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK44_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK44_SEC_RESET  "-"
#define OTP_SW_LOCK44_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK44_SEC_MSB    _u(1)
#define OTP_SW_LOCK44_SEC_LSB    _u(0)
#define OTP_SW_LOCK44_SEC_ACCESS "RW"
#define OTP_SW_LOCK44_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK44_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK44_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK45
// Description : Software lock register for page 45.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK45_OFFSET _u(0x000000b4)
#define OTP_SW_LOCK45_BITS   _u(0x0000000f)
#define OTP_SW_LOCK45_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK45_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK45_NSEC_RESET  "-"
#define OTP_SW_LOCK45_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK45_NSEC_MSB    _u(3)
#define OTP_SW_LOCK45_NSEC_LSB    _u(2)
#define OTP_SW_LOCK45_NSEC_ACCESS "RW"
#define OTP_SW_LOCK45_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK45_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK45_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK45_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK45_SEC_RESET  "-"
#define OTP_SW_LOCK45_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK45_SEC_MSB    _u(1)
#define OTP_SW_LOCK45_SEC_LSB    _u(0)
#define OTP_SW_LOCK45_SEC_ACCESS "RW"
#define OTP_SW_LOCK45_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK45_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK45_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK46
// Description : Software lock register for page 46.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK46_OFFSET _u(0x000000b8)
#define OTP_SW_LOCK46_BITS   _u(0x0000000f)
#define OTP_SW_LOCK46_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK46_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK46_NSEC_RESET  "-"
#define OTP_SW_LOCK46_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK46_NSEC_MSB    _u(3)
#define OTP_SW_LOCK46_NSEC_LSB    _u(2)
#define OTP_SW_LOCK46_NSEC_ACCESS "RW"
#define OTP_SW_LOCK46_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK46_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK46_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK46_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK46_SEC_RESET  "-"
#define OTP_SW_LOCK46_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK46_SEC_MSB    _u(1)
#define OTP_SW_LOCK46_SEC_LSB    _u(0)
#define OTP_SW_LOCK46_SEC_ACCESS "RW"
#define OTP_SW_LOCK46_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK46_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK46_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK47
// Description : Software lock register for page 47.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK47_OFFSET _u(0x000000bc)
#define OTP_SW_LOCK47_BITS   _u(0x0000000f)
#define OTP_SW_LOCK47_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK47_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK47_NSEC_RESET  "-"
#define OTP_SW_LOCK47_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK47_NSEC_MSB    _u(3)
#define OTP_SW_LOCK47_NSEC_LSB    _u(2)
#define OTP_SW_LOCK47_NSEC_ACCESS "RW"
#define OTP_SW_LOCK47_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK47_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK47_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK47_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK47_SEC_RESET  "-"
#define OTP_SW_LOCK47_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK47_SEC_MSB    _u(1)
#define OTP_SW_LOCK47_SEC_LSB    _u(0)
#define OTP_SW_LOCK47_SEC_ACCESS "RW"
#define OTP_SW_LOCK47_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK47_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK47_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK48
// Description : Software lock register for page 48.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK48_OFFSET _u(0x000000c0)
#define OTP_SW_LOCK48_BITS   _u(0x0000000f)
#define OTP_SW_LOCK48_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK48_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK48_NSEC_RESET  "-"
#define OTP_SW_LOCK48_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK48_NSEC_MSB    _u(3)
#define OTP_SW_LOCK48_NSEC_LSB    _u(2)
#define OTP_SW_LOCK48_NSEC_ACCESS "RW"
#define OTP_SW_LOCK48_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK48_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK48_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK48_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK48_SEC_RESET  "-"
#define OTP_SW_LOCK48_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK48_SEC_MSB    _u(1)
#define OTP_SW_LOCK48_SEC_LSB    _u(0)
#define OTP_SW_LOCK48_SEC_ACCESS "RW"
#define OTP_SW_LOCK48_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK48_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK48_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK49
// Description : Software lock register for page 49.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK49_OFFSET _u(0x000000c4)
#define OTP_SW_LOCK49_BITS   _u(0x0000000f)
#define OTP_SW_LOCK49_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK49_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK49_NSEC_RESET  "-"
#define OTP_SW_LOCK49_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK49_NSEC_MSB    _u(3)
#define OTP_SW_LOCK49_NSEC_LSB    _u(2)
#define OTP_SW_LOCK49_NSEC_ACCESS "RW"
#define OTP_SW_LOCK49_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK49_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK49_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK49_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK49_SEC_RESET  "-"
#define OTP_SW_LOCK49_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK49_SEC_MSB    _u(1)
#define OTP_SW_LOCK49_SEC_LSB    _u(0)
#define OTP_SW_LOCK49_SEC_ACCESS "RW"
#define OTP_SW_LOCK49_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK49_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK49_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK50
// Description : Software lock register for page 50.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK50_OFFSET _u(0x000000c8)
#define OTP_SW_LOCK50_BITS   _u(0x0000000f)
#define OTP_SW_LOCK50_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK50_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK50_NSEC_RESET  "-"
#define OTP_SW_LOCK50_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK50_NSEC_MSB    _u(3)
#define OTP_SW_LOCK50_NSEC_LSB    _u(2)
#define OTP_SW_LOCK50_NSEC_ACCESS "RW"
#define OTP_SW_LOCK50_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK50_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK50_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK50_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK50_SEC_RESET  "-"
#define OTP_SW_LOCK50_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK50_SEC_MSB    _u(1)
#define OTP_SW_LOCK50_SEC_LSB    _u(0)
#define OTP_SW_LOCK50_SEC_ACCESS "RW"
#define OTP_SW_LOCK50_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK50_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK50_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK51
// Description : Software lock register for page 51.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK51_OFFSET _u(0x000000cc)
#define OTP_SW_LOCK51_BITS   _u(0x0000000f)
#define OTP_SW_LOCK51_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK51_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK51_NSEC_RESET  "-"
#define OTP_SW_LOCK51_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK51_NSEC_MSB    _u(3)
#define OTP_SW_LOCK51_NSEC_LSB    _u(2)
#define OTP_SW_LOCK51_NSEC_ACCESS "RW"
#define OTP_SW_LOCK51_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK51_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK51_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK51_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK51_SEC_RESET  "-"
#define OTP_SW_LOCK51_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK51_SEC_MSB    _u(1)
#define OTP_SW_LOCK51_SEC_LSB    _u(0)
#define OTP_SW_LOCK51_SEC_ACCESS "RW"
#define OTP_SW_LOCK51_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK51_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK51_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK52
// Description : Software lock register for page 52.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK52_OFFSET _u(0x000000d0)
#define OTP_SW_LOCK52_BITS   _u(0x0000000f)
#define OTP_SW_LOCK52_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK52_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK52_NSEC_RESET  "-"
#define OTP_SW_LOCK52_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK52_NSEC_MSB    _u(3)
#define OTP_SW_LOCK52_NSEC_LSB    _u(2)
#define OTP_SW_LOCK52_NSEC_ACCESS "RW"
#define OTP_SW_LOCK52_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK52_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK52_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK52_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK52_SEC_RESET  "-"
#define OTP_SW_LOCK52_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK52_SEC_MSB    _u(1)
#define OTP_SW_LOCK52_SEC_LSB    _u(0)
#define OTP_SW_LOCK52_SEC_ACCESS "RW"
#define OTP_SW_LOCK52_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK52_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK52_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK53
// Description : Software lock register for page 53.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK53_OFFSET _u(0x000000d4)
#define OTP_SW_LOCK53_BITS   _u(0x0000000f)
#define OTP_SW_LOCK53_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK53_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK53_NSEC_RESET  "-"
#define OTP_SW_LOCK53_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK53_NSEC_MSB    _u(3)
#define OTP_SW_LOCK53_NSEC_LSB    _u(2)
#define OTP_SW_LOCK53_NSEC_ACCESS "RW"
#define OTP_SW_LOCK53_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK53_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK53_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK53_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK53_SEC_RESET  "-"
#define OTP_SW_LOCK53_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK53_SEC_MSB    _u(1)
#define OTP_SW_LOCK53_SEC_LSB    _u(0)
#define OTP_SW_LOCK53_SEC_ACCESS "RW"
#define OTP_SW_LOCK53_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK53_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK53_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK54
// Description : Software lock register for page 54.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK54_OFFSET _u(0x000000d8)
#define OTP_SW_LOCK54_BITS   _u(0x0000000f)
#define OTP_SW_LOCK54_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK54_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK54_NSEC_RESET  "-"
#define OTP_SW_LOCK54_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK54_NSEC_MSB    _u(3)
#define OTP_SW_LOCK54_NSEC_LSB    _u(2)
#define OTP_SW_LOCK54_NSEC_ACCESS "RW"
#define OTP_SW_LOCK54_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK54_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK54_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK54_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK54_SEC_RESET  "-"
#define OTP_SW_LOCK54_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK54_SEC_MSB    _u(1)
#define OTP_SW_LOCK54_SEC_LSB    _u(0)
#define OTP_SW_LOCK54_SEC_ACCESS "RW"
#define OTP_SW_LOCK54_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK54_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK54_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK55
// Description : Software lock register for page 55.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK55_OFFSET _u(0x000000dc)
#define OTP_SW_LOCK55_BITS   _u(0x0000000f)
#define OTP_SW_LOCK55_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK55_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK55_NSEC_RESET  "-"
#define OTP_SW_LOCK55_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK55_NSEC_MSB    _u(3)
#define OTP_SW_LOCK55_NSEC_LSB    _u(2)
#define OTP_SW_LOCK55_NSEC_ACCESS "RW"
#define OTP_SW_LOCK55_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK55_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK55_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK55_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK55_SEC_RESET  "-"
#define OTP_SW_LOCK55_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK55_SEC_MSB    _u(1)
#define OTP_SW_LOCK55_SEC_LSB    _u(0)
#define OTP_SW_LOCK55_SEC_ACCESS "RW"
#define OTP_SW_LOCK55_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK55_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK55_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK56
// Description : Software lock register for page 56.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK56_OFFSET _u(0x000000e0)
#define OTP_SW_LOCK56_BITS   _u(0x0000000f)
#define OTP_SW_LOCK56_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK56_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK56_NSEC_RESET  "-"
#define OTP_SW_LOCK56_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK56_NSEC_MSB    _u(3)
#define OTP_SW_LOCK56_NSEC_LSB    _u(2)
#define OTP_SW_LOCK56_NSEC_ACCESS "RW"
#define OTP_SW_LOCK56_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK56_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK56_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK56_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK56_SEC_RESET  "-"
#define OTP_SW_LOCK56_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK56_SEC_MSB    _u(1)
#define OTP_SW_LOCK56_SEC_LSB    _u(0)
#define OTP_SW_LOCK56_SEC_ACCESS "RW"
#define OTP_SW_LOCK56_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK56_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK56_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK57
// Description : Software lock register for page 57.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK57_OFFSET _u(0x000000e4)
#define OTP_SW_LOCK57_BITS   _u(0x0000000f)
#define OTP_SW_LOCK57_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK57_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK57_NSEC_RESET  "-"
#define OTP_SW_LOCK57_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK57_NSEC_MSB    _u(3)
#define OTP_SW_LOCK57_NSEC_LSB    _u(2)
#define OTP_SW_LOCK57_NSEC_ACCESS "RW"
#define OTP_SW_LOCK57_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK57_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK57_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK57_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK57_SEC_RESET  "-"
#define OTP_SW_LOCK57_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK57_SEC_MSB    _u(1)
#define OTP_SW_LOCK57_SEC_LSB    _u(0)
#define OTP_SW_LOCK57_SEC_ACCESS "RW"
#define OTP_SW_LOCK57_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK57_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK57_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK58
// Description : Software lock register for page 58.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK58_OFFSET _u(0x000000e8)
#define OTP_SW_LOCK58_BITS   _u(0x0000000f)
#define OTP_SW_LOCK58_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK58_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK58_NSEC_RESET  "-"
#define OTP_SW_LOCK58_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK58_NSEC_MSB    _u(3)
#define OTP_SW_LOCK58_NSEC_LSB    _u(2)
#define OTP_SW_LOCK58_NSEC_ACCESS "RW"
#define OTP_SW_LOCK58_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK58_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK58_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK58_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK58_SEC_RESET  "-"
#define OTP_SW_LOCK58_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK58_SEC_MSB    _u(1)
#define OTP_SW_LOCK58_SEC_LSB    _u(0)
#define OTP_SW_LOCK58_SEC_ACCESS "RW"
#define OTP_SW_LOCK58_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK58_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK58_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK59
// Description : Software lock register for page 59.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK59_OFFSET _u(0x000000ec)
#define OTP_SW_LOCK59_BITS   _u(0x0000000f)
#define OTP_SW_LOCK59_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK59_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK59_NSEC_RESET  "-"
#define OTP_SW_LOCK59_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK59_NSEC_MSB    _u(3)
#define OTP_SW_LOCK59_NSEC_LSB    _u(2)
#define OTP_SW_LOCK59_NSEC_ACCESS "RW"
#define OTP_SW_LOCK59_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK59_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK59_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK59_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK59_SEC_RESET  "-"
#define OTP_SW_LOCK59_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK59_SEC_MSB    _u(1)
#define OTP_SW_LOCK59_SEC_LSB    _u(0)
#define OTP_SW_LOCK59_SEC_ACCESS "RW"
#define OTP_SW_LOCK59_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK59_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK59_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK60
// Description : Software lock register for page 60.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK60_OFFSET _u(0x000000f0)
#define OTP_SW_LOCK60_BITS   _u(0x0000000f)
#define OTP_SW_LOCK60_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK60_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK60_NSEC_RESET  "-"
#define OTP_SW_LOCK60_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK60_NSEC_MSB    _u(3)
#define OTP_SW_LOCK60_NSEC_LSB    _u(2)
#define OTP_SW_LOCK60_NSEC_ACCESS "RW"
#define OTP_SW_LOCK60_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK60_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK60_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK60_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK60_SEC_RESET  "-"
#define OTP_SW_LOCK60_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK60_SEC_MSB    _u(1)
#define OTP_SW_LOCK60_SEC_LSB    _u(0)
#define OTP_SW_LOCK60_SEC_ACCESS "RW"
#define OTP_SW_LOCK60_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK60_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK60_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK61
// Description : Software lock register for page 61.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK61_OFFSET _u(0x000000f4)
#define OTP_SW_LOCK61_BITS   _u(0x0000000f)
#define OTP_SW_LOCK61_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK61_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK61_NSEC_RESET  "-"
#define OTP_SW_LOCK61_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK61_NSEC_MSB    _u(3)
#define OTP_SW_LOCK61_NSEC_LSB    _u(2)
#define OTP_SW_LOCK61_NSEC_ACCESS "RW"
#define OTP_SW_LOCK61_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK61_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK61_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK61_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK61_SEC_RESET  "-"
#define OTP_SW_LOCK61_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK61_SEC_MSB    _u(1)
#define OTP_SW_LOCK61_SEC_LSB    _u(0)
#define OTP_SW_LOCK61_SEC_ACCESS "RW"
#define OTP_SW_LOCK61_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK61_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK61_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK62
// Description : Software lock register for page 62.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK62_OFFSET _u(0x000000f8)
#define OTP_SW_LOCK62_BITS   _u(0x0000000f)
#define OTP_SW_LOCK62_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK62_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK62_NSEC_RESET  "-"
#define OTP_SW_LOCK62_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK62_NSEC_MSB    _u(3)
#define OTP_SW_LOCK62_NSEC_LSB    _u(2)
#define OTP_SW_LOCK62_NSEC_ACCESS "RW"
#define OTP_SW_LOCK62_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK62_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK62_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK62_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK62_SEC_RESET  "-"
#define OTP_SW_LOCK62_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK62_SEC_MSB    _u(1)
#define OTP_SW_LOCK62_SEC_LSB    _u(0)
#define OTP_SW_LOCK62_SEC_ACCESS "RW"
#define OTP_SW_LOCK62_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK62_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK62_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SW_LOCK63
// Description : Software lock register for page 63.
//
//               Locks are initialised from the OTP lock pages at reset. This
//               register can be written to further advance the lock state of
//               each page (until next reset), and read to check the current
//               lock state of a page.
#define OTP_SW_LOCK63_OFFSET _u(0x000000fc)
#define OTP_SW_LOCK63_BITS   _u(0x0000000f)
#define OTP_SW_LOCK63_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK63_NSEC
// Description : Non-secure lock status. Writes are OR'd with the current value.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK63_NSEC_RESET  "-"
#define OTP_SW_LOCK63_NSEC_BITS   _u(0x0000000c)
#define OTP_SW_LOCK63_NSEC_MSB    _u(3)
#define OTP_SW_LOCK63_NSEC_LSB    _u(2)
#define OTP_SW_LOCK63_NSEC_ACCESS "RW"
#define OTP_SW_LOCK63_NSEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK63_NSEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK63_NSEC_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_SW_LOCK63_SEC
// Description : Secure lock status. Writes are OR'd with the current value.
//               This field is read-only to Non-secure code.
//               0x0 -> read_write
//               0x1 -> read_only
//               0x3 -> inaccessible
#define OTP_SW_LOCK63_SEC_RESET  "-"
#define OTP_SW_LOCK63_SEC_BITS   _u(0x00000003)
#define OTP_SW_LOCK63_SEC_MSB    _u(1)
#define OTP_SW_LOCK63_SEC_LSB    _u(0)
#define OTP_SW_LOCK63_SEC_ACCESS "RW"
#define OTP_SW_LOCK63_SEC_VALUE_READ_WRITE _u(0x0)
#define OTP_SW_LOCK63_SEC_VALUE_READ_ONLY _u(0x1)
#define OTP_SW_LOCK63_SEC_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_SBPI_INSTR
// Description : Dispatch instructions to the SBPI interface, used for
//               programming the OTP fuses.
#define OTP_SBPI_INSTR_OFFSET _u(0x00000100)
#define OTP_SBPI_INSTR_BITS   _u(0x7fffffff)
#define OTP_SBPI_INSTR_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SBPI_INSTR_EXEC
// Description : Execute instruction
#define OTP_SBPI_INSTR_EXEC_RESET  _u(0x0)
#define OTP_SBPI_INSTR_EXEC_BITS   _u(0x40000000)
#define OTP_SBPI_INSTR_EXEC_MSB    _u(30)
#define OTP_SBPI_INSTR_EXEC_LSB    _u(30)
#define OTP_SBPI_INSTR_EXEC_ACCESS "SC"
// -----------------------------------------------------------------------------
// Field       : OTP_SBPI_INSTR_IS_WR
// Description : Payload type is write
#define OTP_SBPI_INSTR_IS_WR_RESET  _u(0x0)
#define OTP_SBPI_INSTR_IS_WR_BITS   _u(0x20000000)
#define OTP_SBPI_INSTR_IS_WR_MSB    _u(29)
#define OTP_SBPI_INSTR_IS_WR_LSB    _u(29)
#define OTP_SBPI_INSTR_IS_WR_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : OTP_SBPI_INSTR_HAS_PAYLOAD
// Description : Instruction has payload (data to be written or to be read)
#define OTP_SBPI_INSTR_HAS_PAYLOAD_RESET  _u(0x0)
#define OTP_SBPI_INSTR_HAS_PAYLOAD_BITS   _u(0x10000000)
#define OTP_SBPI_INSTR_HAS_PAYLOAD_MSB    _u(28)
#define OTP_SBPI_INSTR_HAS_PAYLOAD_LSB    _u(28)
#define OTP_SBPI_INSTR_HAS_PAYLOAD_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : OTP_SBPI_INSTR_PAYLOAD_SIZE_M1
// Description : Instruction payload size in bytes minus 1
#define OTP_SBPI_INSTR_PAYLOAD_SIZE_M1_RESET  _u(0x0)
#define OTP_SBPI_INSTR_PAYLOAD_SIZE_M1_BITS   _u(0x0f000000)
#define OTP_SBPI_INSTR_PAYLOAD_SIZE_M1_MSB    _u(27)
#define OTP_SBPI_INSTR_PAYLOAD_SIZE_M1_LSB    _u(24)
#define OTP_SBPI_INSTR_PAYLOAD_SIZE_M1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : OTP_SBPI_INSTR_TARGET
// Description : Instruction target, it can be PMC (0x3a) or DAP (0x02)
#define OTP_SBPI_INSTR_TARGET_RESET  _u(0x00)
#define OTP_SBPI_INSTR_TARGET_BITS   _u(0x00ff0000)
#define OTP_SBPI_INSTR_TARGET_MSB    _u(23)
#define OTP_SBPI_INSTR_TARGET_LSB    _u(16)
#define OTP_SBPI_INSTR_TARGET_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : OTP_SBPI_INSTR_CMD
#define OTP_SBPI_INSTR_CMD_RESET  _u(0x00)
#define OTP_SBPI_INSTR_CMD_BITS   _u(0x0000ff00)
#define OTP_SBPI_INSTR_CMD_MSB    _u(15)
#define OTP_SBPI_INSTR_CMD_LSB    _u(8)
#define OTP_SBPI_INSTR_CMD_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : OTP_SBPI_INSTR_SHORT_WDATA
// Description : wdata to be used only when payload_size_m1=0
#define OTP_SBPI_INSTR_SHORT_WDATA_RESET  _u(0x00)
#define OTP_SBPI_INSTR_SHORT_WDATA_BITS   _u(0x000000ff)
#define OTP_SBPI_INSTR_SHORT_WDATA_MSB    _u(7)
#define OTP_SBPI_INSTR_SHORT_WDATA_LSB    _u(0)
#define OTP_SBPI_INSTR_SHORT_WDATA_ACCESS "RW"
// =============================================================================
// Register    : OTP_SBPI_WDATA_0
// Description : SBPI write payload bytes 3..0
#define OTP_SBPI_WDATA_0_OFFSET _u(0x00000104)
#define OTP_SBPI_WDATA_0_BITS   _u(0xffffffff)
#define OTP_SBPI_WDATA_0_RESET  _u(0x00000000)
#define OTP_SBPI_WDATA_0_MSB    _u(31)
#define OTP_SBPI_WDATA_0_LSB    _u(0)
#define OTP_SBPI_WDATA_0_ACCESS "RW"
// =============================================================================
// Register    : OTP_SBPI_WDATA_1
// Description : SBPI write payload bytes 7..4
#define OTP_SBPI_WDATA_1_OFFSET _u(0x00000108)
#define OTP_SBPI_WDATA_1_BITS   _u(0xffffffff)
#define OTP_SBPI_WDATA_1_RESET  _u(0x00000000)
#define OTP_SBPI_WDATA_1_MSB    _u(31)
#define OTP_SBPI_WDATA_1_LSB    _u(0)
#define OTP_SBPI_WDATA_1_ACCESS "RW"
// =============================================================================
// Register    : OTP_SBPI_WDATA_2
// Description : SBPI write payload bytes 11..8
#define OTP_SBPI_WDATA_2_OFFSET _u(0x0000010c)
#define OTP_SBPI_WDATA_2_BITS   _u(0xffffffff)
#define OTP_SBPI_WDATA_2_RESET  _u(0x00000000)
#define OTP_SBPI_WDATA_2_MSB    _u(31)
#define OTP_SBPI_WDATA_2_LSB    _u(0)
#define OTP_SBPI_WDATA_2_ACCESS "RW"
// =============================================================================
// Register    : OTP_SBPI_WDATA_3
// Description : SBPI write payload bytes 15..12
#define OTP_SBPI_WDATA_3_OFFSET _u(0x00000110)
#define OTP_SBPI_WDATA_3_BITS   _u(0xffffffff)
#define OTP_SBPI_WDATA_3_RESET  _u(0x00000000)
#define OTP_SBPI_WDATA_3_MSB    _u(31)
#define OTP_SBPI_WDATA_3_LSB    _u(0)
#define OTP_SBPI_WDATA_3_ACCESS "RW"
// =============================================================================
// Register    : OTP_SBPI_RDATA_0
// Description : Read payload bytes 3..0. Once read, the data in the register
//               will automatically clear to 0.
#define OTP_SBPI_RDATA_0_OFFSET _u(0x00000114)
#define OTP_SBPI_RDATA_0_BITS   _u(0xffffffff)
#define OTP_SBPI_RDATA_0_RESET  _u(0x00000000)
#define OTP_SBPI_RDATA_0_MSB    _u(31)
#define OTP_SBPI_RDATA_0_LSB    _u(0)
#define OTP_SBPI_RDATA_0_ACCESS "RO"
// =============================================================================
// Register    : OTP_SBPI_RDATA_1
// Description : Read payload bytes 7..4. Once read, the data in the register
//               will automatically clear to 0.
#define OTP_SBPI_RDATA_1_OFFSET _u(0x00000118)
#define OTP_SBPI_RDATA_1_BITS   _u(0xffffffff)
#define OTP_SBPI_RDATA_1_RESET  _u(0x00000000)
#define OTP_SBPI_RDATA_1_MSB    _u(31)
#define OTP_SBPI_RDATA_1_LSB    _u(0)
#define OTP_SBPI_RDATA_1_ACCESS "RO"
// =============================================================================
// Register    : OTP_SBPI_RDATA_2
// Description : Read payload bytes 11..8. Once read, the data in the register
//               will automatically clear to 0.
#define OTP_SBPI_RDATA_2_OFFSET _u(0x0000011c)
#define OTP_SBPI_RDATA_2_BITS   _u(0xffffffff)
#define OTP_SBPI_RDATA_2_RESET  _u(0x00000000)
#define OTP_SBPI_RDATA_2_MSB    _u(31)
#define OTP_SBPI_RDATA_2_LSB    _u(0)
#define OTP_SBPI_RDATA_2_ACCESS "RO"
// =============================================================================
// Register    : OTP_SBPI_RDATA_3
// Description : Read payload bytes 15..12. Once read, the data in the register
//               will automatically clear to 0.
#define OTP_SBPI_RDATA_3_OFFSET _u(0x00000120)
#define OTP_SBPI_RDATA_3_BITS   _u(0xffffffff)
#define OTP_SBPI_RDATA_3_RESET  _u(0x00000000)
#define OTP_SBPI_RDATA_3_MSB    _u(31)
#define OTP_SBPI_RDATA_3_LSB    _u(0)
#define OTP_SBPI_RDATA_3_ACCESS "RO"
// =============================================================================
// Register    : OTP_SBPI_STATUS
#define OTP_SBPI_STATUS_OFFSET _u(0x00000124)
#define OTP_SBPI_STATUS_BITS   _u(0x00ff1111)
#define OTP_SBPI_STATUS_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_SBPI_STATUS_MISO
// Description : SBPI MISO (master in - slave out): response from SBPI
#define OTP_SBPI_STATUS_MISO_RESET  "-"
#define OTP_SBPI_STATUS_MISO_BITS   _u(0x00ff0000)
#define OTP_SBPI_STATUS_MISO_MSB    _u(23)
#define OTP_SBPI_STATUS_MISO_LSB    _u(16)
#define OTP_SBPI_STATUS_MISO_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_SBPI_STATUS_FLAG
// Description : SBPI flag
#define OTP_SBPI_STATUS_FLAG_RESET  "-"
#define OTP_SBPI_STATUS_FLAG_BITS   _u(0x00001000)
#define OTP_SBPI_STATUS_FLAG_MSB    _u(12)
#define OTP_SBPI_STATUS_FLAG_LSB    _u(12)
#define OTP_SBPI_STATUS_FLAG_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_SBPI_STATUS_INSTR_MISS
// Description : Last instruction missed (dropped), as the previous has not
//               finished running
#define OTP_SBPI_STATUS_INSTR_MISS_RESET  _u(0x0)
#define OTP_SBPI_STATUS_INSTR_MISS_BITS   _u(0x00000100)
#define OTP_SBPI_STATUS_INSTR_MISS_MSB    _u(8)
#define OTP_SBPI_STATUS_INSTR_MISS_LSB    _u(8)
#define OTP_SBPI_STATUS_INSTR_MISS_ACCESS "WC"
// -----------------------------------------------------------------------------
// Field       : OTP_SBPI_STATUS_INSTR_DONE
// Description : Last instruction done
#define OTP_SBPI_STATUS_INSTR_DONE_RESET  _u(0x0)
#define OTP_SBPI_STATUS_INSTR_DONE_BITS   _u(0x00000010)
#define OTP_SBPI_STATUS_INSTR_DONE_MSB    _u(4)
#define OTP_SBPI_STATUS_INSTR_DONE_LSB    _u(4)
#define OTP_SBPI_STATUS_INSTR_DONE_ACCESS "WC"
// -----------------------------------------------------------------------------
// Field       : OTP_SBPI_STATUS_RDATA_VLD
// Description : Read command has returned data
#define OTP_SBPI_STATUS_RDATA_VLD_RESET  _u(0x0)
#define OTP_SBPI_STATUS_RDATA_VLD_BITS   _u(0x00000001)
#define OTP_SBPI_STATUS_RDATA_VLD_MSB    _u(0)
#define OTP_SBPI_STATUS_RDATA_VLD_LSB    _u(0)
#define OTP_SBPI_STATUS_RDATA_VLD_ACCESS "WC"
// =============================================================================
// Register    : OTP_USR
// Description : Controls for APB data read interface (USER interface)
#define OTP_USR_OFFSET _u(0x00000128)
#define OTP_USR_BITS   _u(0x00000011)
#define OTP_USR_RESET  _u(0x00000001)
// -----------------------------------------------------------------------------
// Field       : OTP_USR_PD
// Description : Power-down; 1 disables current reference. Must be 0 to read
//               data from the OTP.
#define OTP_USR_PD_RESET  _u(0x0)
#define OTP_USR_PD_BITS   _u(0x00000010)
#define OTP_USR_PD_MSB    _u(4)
#define OTP_USR_PD_LSB    _u(4)
#define OTP_USR_PD_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : OTP_USR_DCTRL
// Description : 1 enables USER interface; 0 disables USER interface (enables
//               SBPI).
//
//               This bit must be cleared before performing any SBPI access,
//               such as when programming the OTP. The APB data read interface
//               (USER interface) will be inaccessible during this time, and
//               will return a bus error if any read is attempted.
#define OTP_USR_DCTRL_RESET  _u(0x1)
#define OTP_USR_DCTRL_BITS   _u(0x00000001)
#define OTP_USR_DCTRL_MSB    _u(0)
#define OTP_USR_DCTRL_LSB    _u(0)
#define OTP_USR_DCTRL_ACCESS "RW"
// =============================================================================
// Register    : OTP_DBG
// Description : Debug for OTP power-on state machine
#define OTP_DBG_OFFSET _u(0x0000012c)
#define OTP_DBG_BITS   _u(0x000010ff)
#define OTP_DBG_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_DBG_CUSTOMER_RMA_FLAG
// Description : The chip is in RMA mode
#define OTP_DBG_CUSTOMER_RMA_FLAG_RESET  "-"
#define OTP_DBG_CUSTOMER_RMA_FLAG_BITS   _u(0x00001000)
#define OTP_DBG_CUSTOMER_RMA_FLAG_MSB    _u(12)
#define OTP_DBG_CUSTOMER_RMA_FLAG_LSB    _u(12)
#define OTP_DBG_CUSTOMER_RMA_FLAG_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DBG_PSM_STATE
// Description : Monitor the PSM FSM's state
#define OTP_DBG_PSM_STATE_RESET  "-"
#define OTP_DBG_PSM_STATE_BITS   _u(0x000000f0)
#define OTP_DBG_PSM_STATE_MSB    _u(7)
#define OTP_DBG_PSM_STATE_LSB    _u(4)
#define OTP_DBG_PSM_STATE_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DBG_ROSC_UP
// Description : Ring oscillator is up and running
#define OTP_DBG_ROSC_UP_RESET  "-"
#define OTP_DBG_ROSC_UP_BITS   _u(0x00000008)
#define OTP_DBG_ROSC_UP_MSB    _u(3)
#define OTP_DBG_ROSC_UP_LSB    _u(3)
#define OTP_DBG_ROSC_UP_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DBG_ROSC_UP_SEEN
// Description : Ring oscillator was seen up and running
#define OTP_DBG_ROSC_UP_SEEN_RESET  _u(0x0)
#define OTP_DBG_ROSC_UP_SEEN_BITS   _u(0x00000004)
#define OTP_DBG_ROSC_UP_SEEN_MSB    _u(2)
#define OTP_DBG_ROSC_UP_SEEN_LSB    _u(2)
#define OTP_DBG_ROSC_UP_SEEN_ACCESS "WC"
// -----------------------------------------------------------------------------
// Field       : OTP_DBG_BOOT_DONE
// Description : PSM boot done status flag
#define OTP_DBG_BOOT_DONE_RESET  "-"
#define OTP_DBG_BOOT_DONE_BITS   _u(0x00000002)
#define OTP_DBG_BOOT_DONE_MSB    _u(1)
#define OTP_DBG_BOOT_DONE_LSB    _u(1)
#define OTP_DBG_BOOT_DONE_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DBG_PSM_DONE
// Description : PSM done status flag
#define OTP_DBG_PSM_DONE_RESET  "-"
#define OTP_DBG_PSM_DONE_BITS   _u(0x00000001)
#define OTP_DBG_PSM_DONE_MSB    _u(0)
#define OTP_DBG_PSM_DONE_LSB    _u(0)
#define OTP_DBG_PSM_DONE_ACCESS "RO"
// =============================================================================
// Register    : OTP_BIST
// Description : During BIST, count address locations that have at least one
//               leaky bit
#define OTP_BIST_OFFSET _u(0x00000134)
#define OTP_BIST_BITS   _u(0x7fff1fff)
#define OTP_BIST_RESET  _u(0x0fff0000)
// -----------------------------------------------------------------------------
// Field       : OTP_BIST_CNT_FAIL
// Description : Flag if the count of address locations with at least one leaky
//               bit exceeds cnt_max
#define OTP_BIST_CNT_FAIL_RESET  "-"
#define OTP_BIST_CNT_FAIL_BITS   _u(0x40000000)
#define OTP_BIST_CNT_FAIL_MSB    _u(30)
#define OTP_BIST_CNT_FAIL_LSB    _u(30)
#define OTP_BIST_CNT_FAIL_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_BIST_CNT_CLR
// Description : Clear counter before use
#define OTP_BIST_CNT_CLR_RESET  _u(0x0)
#define OTP_BIST_CNT_CLR_BITS   _u(0x20000000)
#define OTP_BIST_CNT_CLR_MSB    _u(29)
#define OTP_BIST_CNT_CLR_LSB    _u(29)
#define OTP_BIST_CNT_CLR_ACCESS "SC"
// -----------------------------------------------------------------------------
// Field       : OTP_BIST_CNT_ENA
// Description : Enable the counter before the BIST function is initiated
#define OTP_BIST_CNT_ENA_RESET  _u(0x0)
#define OTP_BIST_CNT_ENA_BITS   _u(0x10000000)
#define OTP_BIST_CNT_ENA_MSB    _u(28)
#define OTP_BIST_CNT_ENA_LSB    _u(28)
#define OTP_BIST_CNT_ENA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : OTP_BIST_CNT_MAX
// Description : The cnt_fail flag will be set if the number of leaky locations
//               exceeds this number
#define OTP_BIST_CNT_MAX_RESET  _u(0xfff)
#define OTP_BIST_CNT_MAX_BITS   _u(0x0fff0000)
#define OTP_BIST_CNT_MAX_MSB    _u(27)
#define OTP_BIST_CNT_MAX_LSB    _u(16)
#define OTP_BIST_CNT_MAX_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : OTP_BIST_CNT
// Description : Number of locations that have at least one leaky bit. Note:
//               This count is true only if the BIST was initiated without the
//               fix option.
#define OTP_BIST_CNT_RESET  "-"
#define OTP_BIST_CNT_BITS   _u(0x00001fff)
#define OTP_BIST_CNT_MSB    _u(12)
#define OTP_BIST_CNT_LSB    _u(0)
#define OTP_BIST_CNT_ACCESS "RO"
// =============================================================================
// Register    : OTP_CRT_KEY_W0
// Description : Word 0 (bits 31..0) of the key. Write only, read returns 0x0
#define OTP_CRT_KEY_W0_OFFSET _u(0x00000138)
#define OTP_CRT_KEY_W0_BITS   _u(0xffffffff)
#define OTP_CRT_KEY_W0_RESET  _u(0x00000000)
#define OTP_CRT_KEY_W0_MSB    _u(31)
#define OTP_CRT_KEY_W0_LSB    _u(0)
#define OTP_CRT_KEY_W0_ACCESS "WO"
// =============================================================================
// Register    : OTP_CRT_KEY_W1
// Description : Word 1 (bits 63..32) of the key. Write only, read returns 0x0
#define OTP_CRT_KEY_W1_OFFSET _u(0x0000013c)
#define OTP_CRT_KEY_W1_BITS   _u(0xffffffff)
#define OTP_CRT_KEY_W1_RESET  _u(0x00000000)
#define OTP_CRT_KEY_W1_MSB    _u(31)
#define OTP_CRT_KEY_W1_LSB    _u(0)
#define OTP_CRT_KEY_W1_ACCESS "WO"
// =============================================================================
// Register    : OTP_CRT_KEY_W2
// Description : Word 2 (bits 95..64) of the key. Write only, read returns 0x0
#define OTP_CRT_KEY_W2_OFFSET _u(0x00000140)
#define OTP_CRT_KEY_W2_BITS   _u(0xffffffff)
#define OTP_CRT_KEY_W2_RESET  _u(0x00000000)
#define OTP_CRT_KEY_W2_MSB    _u(31)
#define OTP_CRT_KEY_W2_LSB    _u(0)
#define OTP_CRT_KEY_W2_ACCESS "WO"
// =============================================================================
// Register    : OTP_CRT_KEY_W3
// Description : Word 3 (bits 127..96) of the key. Write only, read returns 0x0
#define OTP_CRT_KEY_W3_OFFSET _u(0x00000144)
#define OTP_CRT_KEY_W3_BITS   _u(0xffffffff)
#define OTP_CRT_KEY_W3_RESET  _u(0x00000000)
#define OTP_CRT_KEY_W3_MSB    _u(31)
#define OTP_CRT_KEY_W3_LSB    _u(0)
#define OTP_CRT_KEY_W3_ACCESS "WO"
// =============================================================================
// Register    : OTP_CRITICAL
// Description : Quickly check values of critical flags read during boot up
#define OTP_CRITICAL_OFFSET _u(0x00000148)
#define OTP_CRITICAL_BITS   _u(0x0003007f)
#define OTP_CRITICAL_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_CRITICAL_RISCV_DISABLE
#define OTP_CRITICAL_RISCV_DISABLE_RESET  _u(0x0)
#define OTP_CRITICAL_RISCV_DISABLE_BITS   _u(0x00020000)
#define OTP_CRITICAL_RISCV_DISABLE_MSB    _u(17)
#define OTP_CRITICAL_RISCV_DISABLE_LSB    _u(17)
#define OTP_CRITICAL_RISCV_DISABLE_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_CRITICAL_ARM_DISABLE
#define OTP_CRITICAL_ARM_DISABLE_RESET  _u(0x0)
#define OTP_CRITICAL_ARM_DISABLE_BITS   _u(0x00010000)
#define OTP_CRITICAL_ARM_DISABLE_MSB    _u(16)
#define OTP_CRITICAL_ARM_DISABLE_LSB    _u(16)
#define OTP_CRITICAL_ARM_DISABLE_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_CRITICAL_GLITCH_DETECTOR_SENS
#define OTP_CRITICAL_GLITCH_DETECTOR_SENS_RESET  _u(0x0)
#define OTP_CRITICAL_GLITCH_DETECTOR_SENS_BITS   _u(0x00000060)
#define OTP_CRITICAL_GLITCH_DETECTOR_SENS_MSB    _u(6)
#define OTP_CRITICAL_GLITCH_DETECTOR_SENS_LSB    _u(5)
#define OTP_CRITICAL_GLITCH_DETECTOR_SENS_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_CRITICAL_GLITCH_DETECTOR_ENABLE
#define OTP_CRITICAL_GLITCH_DETECTOR_ENABLE_RESET  _u(0x0)
#define OTP_CRITICAL_GLITCH_DETECTOR_ENABLE_BITS   _u(0x00000010)
#define OTP_CRITICAL_GLITCH_DETECTOR_ENABLE_MSB    _u(4)
#define OTP_CRITICAL_GLITCH_DETECTOR_ENABLE_LSB    _u(4)
#define OTP_CRITICAL_GLITCH_DETECTOR_ENABLE_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_CRITICAL_DEFAULT_ARCHSEL
#define OTP_CRITICAL_DEFAULT_ARCHSEL_RESET  _u(0x0)
#define OTP_CRITICAL_DEFAULT_ARCHSEL_BITS   _u(0x00000008)
#define OTP_CRITICAL_DEFAULT_ARCHSEL_MSB    _u(3)
#define OTP_CRITICAL_DEFAULT_ARCHSEL_LSB    _u(3)
#define OTP_CRITICAL_DEFAULT_ARCHSEL_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_CRITICAL_DEBUG_DISABLE
#define OTP_CRITICAL_DEBUG_DISABLE_RESET  _u(0x0)
#define OTP_CRITICAL_DEBUG_DISABLE_BITS   _u(0x00000004)
#define OTP_CRITICAL_DEBUG_DISABLE_MSB    _u(2)
#define OTP_CRITICAL_DEBUG_DISABLE_LSB    _u(2)
#define OTP_CRITICAL_DEBUG_DISABLE_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_CRITICAL_SECURE_DEBUG_DISABLE
#define OTP_CRITICAL_SECURE_DEBUG_DISABLE_RESET  _u(0x0)
#define OTP_CRITICAL_SECURE_DEBUG_DISABLE_BITS   _u(0x00000002)
#define OTP_CRITICAL_SECURE_DEBUG_DISABLE_MSB    _u(1)
#define OTP_CRITICAL_SECURE_DEBUG_DISABLE_LSB    _u(1)
#define OTP_CRITICAL_SECURE_DEBUG_DISABLE_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_CRITICAL_SECURE_BOOT_ENABLE
#define OTP_CRITICAL_SECURE_BOOT_ENABLE_RESET  _u(0x0)
#define OTP_CRITICAL_SECURE_BOOT_ENABLE_BITS   _u(0x00000001)
#define OTP_CRITICAL_SECURE_BOOT_ENABLE_MSB    _u(0)
#define OTP_CRITICAL_SECURE_BOOT_ENABLE_LSB    _u(0)
#define OTP_CRITICAL_SECURE_BOOT_ENABLE_ACCESS "RO"
// =============================================================================
// Register    : OTP_KEY_VALID
// Description : Which keys were valid (enrolled) at boot time
#define OTP_KEY_VALID_OFFSET _u(0x0000014c)
#define OTP_KEY_VALID_BITS   _u(0x000000ff)
#define OTP_KEY_VALID_RESET  _u(0x00000000)
#define OTP_KEY_VALID_MSB    _u(7)
#define OTP_KEY_VALID_LSB    _u(0)
#define OTP_KEY_VALID_ACCESS "RO"
// =============================================================================
// Register    : OTP_DEBUGEN
// Description : Enable a debug feature that has been disabled. Debug features
//               are disabled if one of the relevant critical boot flags is set
//               in OTP (DEBUG_DISABLE or SECURE_DEBUG_DISABLE), OR if a debug
//               key is marked valid in OTP, and the matching key value has not
//               been supplied over SWD.
//
//               Specifically:
//
//               - The DEBUG_DISABLE flag disables all debug features. This can
//               be fully overridden by setting all bits of this register.
//
//               - The SECURE_DEBUG_DISABLE flag disables secure processor
//               debug. This can be fully overridden by setting the PROC0_SECURE
//               and PROC1_SECURE bits of this register.
//
//               - If a single debug key has been registered, and no matching
//               key value has been supplied over SWD, then all debug features
//               are disabled. This can be fully overridden by setting all bits
//               of this register.
//
//               - If both debug keys have been registered, and the Non-secure
//               key's value (key 6) has been supplied over SWD, secure
//               processor debug is disabled. This can be fully overridden by
//               setting the PROC0_SECURE and PROC1_SECURE bits of this
//               register.
//
//               - If both debug keys have been registered, and the Secure key's
//               value (key 5) has been supplied over SWD, then no debug
//               features are disabled by the key mechanism. However, note that
//               in this case debug features may still be disabled by the
//               critical boot flags.
#define OTP_DEBUGEN_OFFSET _u(0x00000150)
#define OTP_DEBUGEN_BITS   _u(0x0000010f)
#define OTP_DEBUGEN_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_DEBUGEN_MISC
// Description : Enable other debug components. Specifically, the CTI, and the
//               APB-AP used to access the RISC-V Debug Module.
//
//               These components are disabled by default if either of the debug
//               disable critical flags is set, or if at least one debug key has
//               been enrolled and the least secure of these enrolled key values
//               has not been provided over SWD.
#define OTP_DEBUGEN_MISC_RESET  _u(0x0)
#define OTP_DEBUGEN_MISC_BITS   _u(0x00000100)
#define OTP_DEBUGEN_MISC_MSB    _u(8)
#define OTP_DEBUGEN_MISC_LSB    _u(8)
#define OTP_DEBUGEN_MISC_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : OTP_DEBUGEN_PROC1_SECURE
// Description : Permit core 1's Mem-AP to generate Secure accesses, assuming it
//               is enabled at all. Also enable secure debug of core 1 (SPIDEN
//               and SPNIDEN).
//
//               Secure debug of core 1 is disabled by default if the secure
//               debug disable critical flag is set, or if at least one debug
//               key has been enrolled and the most secure of these enrolled key
//               values not yet provided over SWD.
#define OTP_DEBUGEN_PROC1_SECURE_RESET  _u(0x0)
#define OTP_DEBUGEN_PROC1_SECURE_BITS   _u(0x00000008)
#define OTP_DEBUGEN_PROC1_SECURE_MSB    _u(3)
#define OTP_DEBUGEN_PROC1_SECURE_LSB    _u(3)
#define OTP_DEBUGEN_PROC1_SECURE_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : OTP_DEBUGEN_PROC1
// Description : Enable core 1's Mem-AP if it is currently disabled.
//
//               The Mem-AP is disabled by default if either of the debug
//               disable critical flags is set, or if at least one debug key has
//               been enrolled and the least secure of these enrolled key values
//               has not been provided over SWD.
#define OTP_DEBUGEN_PROC1_RESET  _u(0x0)
#define OTP_DEBUGEN_PROC1_BITS   _u(0x00000004)
#define OTP_DEBUGEN_PROC1_MSB    _u(2)
#define OTP_DEBUGEN_PROC1_LSB    _u(2)
#define OTP_DEBUGEN_PROC1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : OTP_DEBUGEN_PROC0_SECURE
// Description : Permit core 0's Mem-AP to generate Secure accesses, assuming it
//               is enabled at all. Also enable secure debug of core 0 (SPIDEN
//               and SPNIDEN).
//
//               Secure debug of core 0 is disabled by default if the secure
//               debug disable critical flag is set, or if at least one debug
//               key has been enrolled and the most secure of these enrolled key
//               values not yet provided over SWD.
//
//               Note also that core Mem-APs are unconditionally disabled when a
//               core is switched to RISC-V mode (by setting the ARCHSEL bit and
//               performing a warm reset of the core).
#define OTP_DEBUGEN_PROC0_SECURE_RESET  _u(0x0)
#define OTP_DEBUGEN_PROC0_SECURE_BITS   _u(0x00000002)
#define OTP_DEBUGEN_PROC0_SECURE_MSB    _u(1)
#define OTP_DEBUGEN_PROC0_SECURE_LSB    _u(1)
#define OTP_DEBUGEN_PROC0_SECURE_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : OTP_DEBUGEN_PROC0
// Description : Enable core 0's Mem-AP if it is currently disabled.
//
//               The Mem-AP is disabled by default if either of the debug
//               disable critical flags is set, or if at least one debug key has
//               been enrolled and the least secure of these enrolled key values
//               has not been provided over SWD.
//
//               Note also that core Mem-APs are unconditionally disabled when a
//               core is switched to RISC-V mode (by setting the ARCHSEL bit and
//               performing a warm reset of the core).
#define OTP_DEBUGEN_PROC0_RESET  _u(0x0)
#define OTP_DEBUGEN_PROC0_BITS   _u(0x00000001)
#define OTP_DEBUGEN_PROC0_MSB    _u(0)
#define OTP_DEBUGEN_PROC0_LSB    _u(0)
#define OTP_DEBUGEN_PROC0_ACCESS "RW"
// =============================================================================
// Register    : OTP_DEBUGEN_LOCK
// Description : Write 1s to lock corresponding bits in DEBUGEN. This register
//               is reset by the processor cold reset.
#define OTP_DEBUGEN_LOCK_OFFSET _u(0x00000154)
#define OTP_DEBUGEN_LOCK_BITS   _u(0x0000010f)
#define OTP_DEBUGEN_LOCK_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_DEBUGEN_LOCK_MISC
// Description : Write 1 to lock the MISC bit of DEBUGEN. Can't be cleared once
//               set.
#define OTP_DEBUGEN_LOCK_MISC_RESET  _u(0x0)
#define OTP_DEBUGEN_LOCK_MISC_BITS   _u(0x00000100)
#define OTP_DEBUGEN_LOCK_MISC_MSB    _u(8)
#define OTP_DEBUGEN_LOCK_MISC_LSB    _u(8)
#define OTP_DEBUGEN_LOCK_MISC_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : OTP_DEBUGEN_LOCK_PROC1_SECURE
// Description : Write 1 to lock the PROC1_SECURE bit of DEBUGEN. Can't be
//               cleared once set.
#define OTP_DEBUGEN_LOCK_PROC1_SECURE_RESET  _u(0x0)
#define OTP_DEBUGEN_LOCK_PROC1_SECURE_BITS   _u(0x00000008)
#define OTP_DEBUGEN_LOCK_PROC1_SECURE_MSB    _u(3)
#define OTP_DEBUGEN_LOCK_PROC1_SECURE_LSB    _u(3)
#define OTP_DEBUGEN_LOCK_PROC1_SECURE_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : OTP_DEBUGEN_LOCK_PROC1
// Description : Write 1 to lock the PROC1 bit of DEBUGEN. Can't be cleared once
//               set.
#define OTP_DEBUGEN_LOCK_PROC1_RESET  _u(0x0)
#define OTP_DEBUGEN_LOCK_PROC1_BITS   _u(0x00000004)
#define OTP_DEBUGEN_LOCK_PROC1_MSB    _u(2)
#define OTP_DEBUGEN_LOCK_PROC1_LSB    _u(2)
#define OTP_DEBUGEN_LOCK_PROC1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : OTP_DEBUGEN_LOCK_PROC0_SECURE
// Description : Write 1 to lock the PROC0_SECURE bit of DEBUGEN. Can't be
//               cleared once set.
#define OTP_DEBUGEN_LOCK_PROC0_SECURE_RESET  _u(0x0)
#define OTP_DEBUGEN_LOCK_PROC0_SECURE_BITS   _u(0x00000002)
#define OTP_DEBUGEN_LOCK_PROC0_SECURE_MSB    _u(1)
#define OTP_DEBUGEN_LOCK_PROC0_SECURE_LSB    _u(1)
#define OTP_DEBUGEN_LOCK_PROC0_SECURE_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : OTP_DEBUGEN_LOCK_PROC0
// Description : Write 1 to lock the PROC0 bit of DEBUGEN. Can't be cleared once
//               set.
#define OTP_DEBUGEN_LOCK_PROC0_RESET  _u(0x0)
#define OTP_DEBUGEN_LOCK_PROC0_BITS   _u(0x00000001)
#define OTP_DEBUGEN_LOCK_PROC0_MSB    _u(0)
#define OTP_DEBUGEN_LOCK_PROC0_LSB    _u(0)
#define OTP_DEBUGEN_LOCK_PROC0_ACCESS "RW"
// =============================================================================
// Register    : OTP_ARCHSEL
// Description : Architecture select (Arm/RISC-V). The default and allowable
//               values of this register are constrained by the critical boot
//               flags.
//
//               This register is reset by the earliest reset in the switched
//               core power domain (before a processor cold reset).
//
//               Cores sample their architecture select signal on a warm reset.
//               The source of the warm reset could be the system power-up state
//               machine, the watchdog timer, Arm SYSRESETREQ or from RISC-V
//               hartresetreq.
//
//               Note that when an Arm core is deselected, its cold reset domain
//               is also held in reset, since in particular the SYSRESETREQ bit
//               becomes inaccessible once the core is deselected. Note also the
//               RISC-V cores do not have a cold reset domain, since their
//               corresponding controls are located in the Debug Module.
#define OTP_ARCHSEL_OFFSET _u(0x00000158)
#define OTP_ARCHSEL_BITS   _u(0x00000003)
#define OTP_ARCHSEL_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_ARCHSEL_CORE1
// Description : Select architecture for core 1.
//               0x0 -> Switch core 1 to Arm (Cortex-M33)
//               0x1 -> Switch core 1 to RISC-V (Hazard3)
#define OTP_ARCHSEL_CORE1_RESET  _u(0x0)
#define OTP_ARCHSEL_CORE1_BITS   _u(0x00000002)
#define OTP_ARCHSEL_CORE1_MSB    _u(1)
#define OTP_ARCHSEL_CORE1_LSB    _u(1)
#define OTP_ARCHSEL_CORE1_ACCESS "RW"
#define OTP_ARCHSEL_CORE1_VALUE_ARM _u(0x0)
#define OTP_ARCHSEL_CORE1_VALUE_RISCV _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_ARCHSEL_CORE0
// Description : Select architecture for core 0.
//               0x0 -> Switch core 0 to Arm (Cortex-M33)
//               0x1 -> Switch core 0 to RISC-V (Hazard3)
#define OTP_ARCHSEL_CORE0_RESET  _u(0x0)
#define OTP_ARCHSEL_CORE0_BITS   _u(0x00000001)
#define OTP_ARCHSEL_CORE0_MSB    _u(0)
#define OTP_ARCHSEL_CORE0_LSB    _u(0)
#define OTP_ARCHSEL_CORE0_ACCESS "RW"
#define OTP_ARCHSEL_CORE0_VALUE_ARM _u(0x0)
#define OTP_ARCHSEL_CORE0_VALUE_RISCV _u(0x1)
// =============================================================================
// Register    : OTP_ARCHSEL_STATUS
// Description : Get the current architecture select state of each core. Cores
//               sample the current value of the ARCHSEL register when their
//               warm reset is released, at which point the corresponding bit in
//               this register will also update.
#define OTP_ARCHSEL_STATUS_OFFSET _u(0x0000015c)
#define OTP_ARCHSEL_STATUS_BITS   _u(0x00000003)
#define OTP_ARCHSEL_STATUS_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_ARCHSEL_STATUS_CORE1
// Description : Current architecture for core 0. Updated on processor warm
//               reset.
//               0x0 -> Core 1 is currently Arm (Cortex-M33)
//               0x1 -> Core 1 is currently RISC-V (Hazard3)
#define OTP_ARCHSEL_STATUS_CORE1_RESET  _u(0x0)
#define OTP_ARCHSEL_STATUS_CORE1_BITS   _u(0x00000002)
#define OTP_ARCHSEL_STATUS_CORE1_MSB    _u(1)
#define OTP_ARCHSEL_STATUS_CORE1_LSB    _u(1)
#define OTP_ARCHSEL_STATUS_CORE1_ACCESS "RO"
#define OTP_ARCHSEL_STATUS_CORE1_VALUE_ARM _u(0x0)
#define OTP_ARCHSEL_STATUS_CORE1_VALUE_RISCV _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_ARCHSEL_STATUS_CORE0
// Description : Current architecture for core 0. Updated on processor warm
//               reset.
//               0x0 -> Core 0 is currently Arm (Cortex-M33)
//               0x1 -> Core 0 is currently RISC-V (Hazard3)
#define OTP_ARCHSEL_STATUS_CORE0_RESET  _u(0x0)
#define OTP_ARCHSEL_STATUS_CORE0_BITS   _u(0x00000001)
#define OTP_ARCHSEL_STATUS_CORE0_MSB    _u(0)
#define OTP_ARCHSEL_STATUS_CORE0_LSB    _u(0)
#define OTP_ARCHSEL_STATUS_CORE0_ACCESS "RO"
#define OTP_ARCHSEL_STATUS_CORE0_VALUE_ARM _u(0x0)
#define OTP_ARCHSEL_STATUS_CORE0_VALUE_RISCV _u(0x1)
// =============================================================================
// Register    : OTP_BOOTDIS
// Description : Tell the bootrom to ignore scratch register boot vectors (both
//               power manager and watchdog) on the next power up.
//
//               If an early boot stage has soft-locked some OTP pages in order
//               to protect their contents from later stages, there is a risk
//               that Secure code running at a later stage can unlock the pages
//               by performing a watchdog reset that resets the OTP.
//
//               This register can be used to ensure that the bootloader runs as
//               normal on the next power up, preventing Secure code at a later
//               stage from accessing OTP in its unlocked state.
//
//               Should be used in conjunction with the power manager BOOTDIS
//               register.
#define OTP_BOOTDIS_OFFSET _u(0x00000160)
#define OTP_BOOTDIS_BITS   _u(0x00000003)
#define OTP_BOOTDIS_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_BOOTDIS_NEXT
// Description : This flag always ORs writes into its current contents. It can
//               be set but not cleared by software.
//
//               The BOOTDIS_NEXT bit is OR'd into the BOOTDIS_NOW bit when the
//               core is powered down. Simultaneously, the BOOTDIS_NEXT bit is
//               cleared. Setting this bit means that the boot scratch registers
//               will be ignored following the next core power down.
//
//               This flag should be set by an early boot stage that has soft-
//               locked OTP pages, to prevent later stages from unlocking it via
//               watchdog reset.
#define OTP_BOOTDIS_NEXT_RESET  _u(0x0)
#define OTP_BOOTDIS_NEXT_BITS   _u(0x00000002)
#define OTP_BOOTDIS_NEXT_MSB    _u(1)
#define OTP_BOOTDIS_NEXT_LSB    _u(1)
#define OTP_BOOTDIS_NEXT_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : OTP_BOOTDIS_NOW
// Description : When the core is powered down, the current value of
//               BOOTDIS_NEXT is OR'd into BOOTDIS_NOW, and BOOTDIS_NEXT is
//               cleared.
//
//               The bootrom checks this flag before reading the boot scratch
//               registers. If it is set, the bootrom clears it, and ignores the
//               BOOT registers. This prevents Secure software from diverting
//               the boot path before a bootloader has had the chance to soft
//               lock OTP pages containing sensitive data.
#define OTP_BOOTDIS_NOW_RESET  _u(0x0)
#define OTP_BOOTDIS_NOW_BITS   _u(0x00000001)
#define OTP_BOOTDIS_NOW_MSB    _u(0)
#define OTP_BOOTDIS_NOW_LSB    _u(0)
#define OTP_BOOTDIS_NOW_ACCESS "WC"
// =============================================================================
// Register    : OTP_INTR
// Description : Raw Interrupts
#define OTP_INTR_OFFSET _u(0x00000164)
#define OTP_INTR_BITS   _u(0x0000001f)
#define OTP_INTR_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_INTR_APB_RD_NSEC_FAIL
#define OTP_INTR_APB_RD_NSEC_FAIL_RESET  _u(0x0)
#define OTP_INTR_APB_RD_NSEC_FAIL_BITS   _u(0x00000010)
#define OTP_INTR_APB_RD_NSEC_FAIL_MSB    _u(4)
#define OTP_INTR_APB_RD_NSEC_FAIL_LSB    _u(4)
#define OTP_INTR_APB_RD_NSEC_FAIL_ACCESS "WC"
// -----------------------------------------------------------------------------
// Field       : OTP_INTR_APB_RD_SEC_FAIL
#define OTP_INTR_APB_RD_SEC_FAIL_RESET  _u(0x0)
#define OTP_INTR_APB_RD_SEC_FAIL_BITS   _u(0x00000008)
#define OTP_INTR_APB_RD_SEC_FAIL_MSB    _u(3)
#define OTP_INTR_APB_RD_SEC_FAIL_LSB    _u(3)
#define OTP_INTR_APB_RD_SEC_FAIL_ACCESS "WC"
// -----------------------------------------------------------------------------
// Field       : OTP_INTR_APB_DCTRL_FAIL
#define OTP_INTR_APB_DCTRL_FAIL_RESET  _u(0x0)
#define OTP_INTR_APB_DCTRL_FAIL_BITS   _u(0x00000004)
#define OTP_INTR_APB_DCTRL_FAIL_MSB    _u(2)
#define OTP_INTR_APB_DCTRL_FAIL_LSB    _u(2)
#define OTP_INTR_APB_DCTRL_FAIL_ACCESS "WC"
// -----------------------------------------------------------------------------
// Field       : OTP_INTR_SBPI_WR_FAIL
#define OTP_INTR_SBPI_WR_FAIL_RESET  _u(0x0)
#define OTP_INTR_SBPI_WR_FAIL_BITS   _u(0x00000002)
#define OTP_INTR_SBPI_WR_FAIL_MSB    _u(1)
#define OTP_INTR_SBPI_WR_FAIL_LSB    _u(1)
#define OTP_INTR_SBPI_WR_FAIL_ACCESS "WC"
// -----------------------------------------------------------------------------
// Field       : OTP_INTR_SBPI_FLAG_N
#define OTP_INTR_SBPI_FLAG_N_RESET  _u(0x0)
#define OTP_INTR_SBPI_FLAG_N_BITS   _u(0x00000001)
#define OTP_INTR_SBPI_FLAG_N_MSB    _u(0)
#define OTP_INTR_SBPI_FLAG_N_LSB    _u(0)
#define OTP_INTR_SBPI_FLAG_N_ACCESS "RO"
// =============================================================================
// Register    : OTP_INTE
// Description : Interrupt Enable
#define OTP_INTE_OFFSET _u(0x00000168)
#define OTP_INTE_BITS   _u(0x0000001f)
#define OTP_INTE_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_INTE_APB_RD_NSEC_FAIL
#define OTP_INTE_APB_RD_NSEC_FAIL_RESET  _u(0x0)
#define OTP_INTE_APB_RD_NSEC_FAIL_BITS   _u(0x00000010)
#define OTP_INTE_APB_RD_NSEC_FAIL_MSB    _u(4)
#define OTP_INTE_APB_RD_NSEC_FAIL_LSB    _u(4)
#define OTP_INTE_APB_RD_NSEC_FAIL_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : OTP_INTE_APB_RD_SEC_FAIL
#define OTP_INTE_APB_RD_SEC_FAIL_RESET  _u(0x0)
#define OTP_INTE_APB_RD_SEC_FAIL_BITS   _u(0x00000008)
#define OTP_INTE_APB_RD_SEC_FAIL_MSB    _u(3)
#define OTP_INTE_APB_RD_SEC_FAIL_LSB    _u(3)
#define OTP_INTE_APB_RD_SEC_FAIL_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : OTP_INTE_APB_DCTRL_FAIL
#define OTP_INTE_APB_DCTRL_FAIL_RESET  _u(0x0)
#define OTP_INTE_APB_DCTRL_FAIL_BITS   _u(0x00000004)
#define OTP_INTE_APB_DCTRL_FAIL_MSB    _u(2)
#define OTP_INTE_APB_DCTRL_FAIL_LSB    _u(2)
#define OTP_INTE_APB_DCTRL_FAIL_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : OTP_INTE_SBPI_WR_FAIL
#define OTP_INTE_SBPI_WR_FAIL_RESET  _u(0x0)
#define OTP_INTE_SBPI_WR_FAIL_BITS   _u(0x00000002)
#define OTP_INTE_SBPI_WR_FAIL_MSB    _u(1)
#define OTP_INTE_SBPI_WR_FAIL_LSB    _u(1)
#define OTP_INTE_SBPI_WR_FAIL_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : OTP_INTE_SBPI_FLAG_N
#define OTP_INTE_SBPI_FLAG_N_RESET  _u(0x0)
#define OTP_INTE_SBPI_FLAG_N_BITS   _u(0x00000001)
#define OTP_INTE_SBPI_FLAG_N_MSB    _u(0)
#define OTP_INTE_SBPI_FLAG_N_LSB    _u(0)
#define OTP_INTE_SBPI_FLAG_N_ACCESS "RW"
// =============================================================================
// Register    : OTP_INTF
// Description : Interrupt Force
#define OTP_INTF_OFFSET _u(0x0000016c)
#define OTP_INTF_BITS   _u(0x0000001f)
#define OTP_INTF_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_INTF_APB_RD_NSEC_FAIL
#define OTP_INTF_APB_RD_NSEC_FAIL_RESET  _u(0x0)
#define OTP_INTF_APB_RD_NSEC_FAIL_BITS   _u(0x00000010)
#define OTP_INTF_APB_RD_NSEC_FAIL_MSB    _u(4)
#define OTP_INTF_APB_RD_NSEC_FAIL_LSB    _u(4)
#define OTP_INTF_APB_RD_NSEC_FAIL_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : OTP_INTF_APB_RD_SEC_FAIL
#define OTP_INTF_APB_RD_SEC_FAIL_RESET  _u(0x0)
#define OTP_INTF_APB_RD_SEC_FAIL_BITS   _u(0x00000008)
#define OTP_INTF_APB_RD_SEC_FAIL_MSB    _u(3)
#define OTP_INTF_APB_RD_SEC_FAIL_LSB    _u(3)
#define OTP_INTF_APB_RD_SEC_FAIL_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : OTP_INTF_APB_DCTRL_FAIL
#define OTP_INTF_APB_DCTRL_FAIL_RESET  _u(0x0)
#define OTP_INTF_APB_DCTRL_FAIL_BITS   _u(0x00000004)
#define OTP_INTF_APB_DCTRL_FAIL_MSB    _u(2)
#define OTP_INTF_APB_DCTRL_FAIL_LSB    _u(2)
#define OTP_INTF_APB_DCTRL_FAIL_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : OTP_INTF_SBPI_WR_FAIL
#define OTP_INTF_SBPI_WR_FAIL_RESET  _u(0x0)
#define OTP_INTF_SBPI_WR_FAIL_BITS   _u(0x00000002)
#define OTP_INTF_SBPI_WR_FAIL_MSB    _u(1)
#define OTP_INTF_SBPI_WR_FAIL_LSB    _u(1)
#define OTP_INTF_SBPI_WR_FAIL_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : OTP_INTF_SBPI_FLAG_N
#define OTP_INTF_SBPI_FLAG_N_RESET  _u(0x0)
#define OTP_INTF_SBPI_FLAG_N_BITS   _u(0x00000001)
#define OTP_INTF_SBPI_FLAG_N_MSB    _u(0)
#define OTP_INTF_SBPI_FLAG_N_LSB    _u(0)
#define OTP_INTF_SBPI_FLAG_N_ACCESS "RW"
// =============================================================================
// Register    : OTP_INTS
// Description : Interrupt status after masking & forcing
#define OTP_INTS_OFFSET _u(0x00000170)
#define OTP_INTS_BITS   _u(0x0000001f)
#define OTP_INTS_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : OTP_INTS_APB_RD_NSEC_FAIL
#define OTP_INTS_APB_RD_NSEC_FAIL_RESET  _u(0x0)
#define OTP_INTS_APB_RD_NSEC_FAIL_BITS   _u(0x00000010)
#define OTP_INTS_APB_RD_NSEC_FAIL_MSB    _u(4)
#define OTP_INTS_APB_RD_NSEC_FAIL_LSB    _u(4)
#define OTP_INTS_APB_RD_NSEC_FAIL_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_INTS_APB_RD_SEC_FAIL
#define OTP_INTS_APB_RD_SEC_FAIL_RESET  _u(0x0)
#define OTP_INTS_APB_RD_SEC_FAIL_BITS   _u(0x00000008)
#define OTP_INTS_APB_RD_SEC_FAIL_MSB    _u(3)
#define OTP_INTS_APB_RD_SEC_FAIL_LSB    _u(3)
#define OTP_INTS_APB_RD_SEC_FAIL_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_INTS_APB_DCTRL_FAIL
#define OTP_INTS_APB_DCTRL_FAIL_RESET  _u(0x0)
#define OTP_INTS_APB_DCTRL_FAIL_BITS   _u(0x00000004)
#define OTP_INTS_APB_DCTRL_FAIL_MSB    _u(2)
#define OTP_INTS_APB_DCTRL_FAIL_LSB    _u(2)
#define OTP_INTS_APB_DCTRL_FAIL_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_INTS_SBPI_WR_FAIL
#define OTP_INTS_SBPI_WR_FAIL_RESET  _u(0x0)
#define OTP_INTS_SBPI_WR_FAIL_BITS   _u(0x00000002)
#define OTP_INTS_SBPI_WR_FAIL_MSB    _u(1)
#define OTP_INTS_SBPI_WR_FAIL_LSB    _u(1)
#define OTP_INTS_SBPI_WR_FAIL_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_INTS_SBPI_FLAG_N
#define OTP_INTS_SBPI_FLAG_N_RESET  _u(0x0)
#define OTP_INTS_SBPI_FLAG_N_BITS   _u(0x00000001)
#define OTP_INTS_SBPI_FLAG_N_MSB    _u(0)
#define OTP_INTS_SBPI_FLAG_N_LSB    _u(0)
#define OTP_INTS_SBPI_FLAG_N_ACCESS "RO"
// =============================================================================
#endif // _HARDWARE_REGS_OTP_H

