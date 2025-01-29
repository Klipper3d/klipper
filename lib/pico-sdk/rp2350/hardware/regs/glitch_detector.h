// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
// =============================================================================
// Register block : GLITCH_DETECTOR
// Version        : 1
// Bus type       : apb
// Description    : Glitch detector controls
// =============================================================================
#ifndef _HARDWARE_REGS_GLITCH_DETECTOR_H
#define _HARDWARE_REGS_GLITCH_DETECTOR_H
// =============================================================================
// Register    : GLITCH_DETECTOR_ARM
// Description : Forcibly arm the glitch detectors, if they are not already
//               armed by OTP. When armed, any individual detector trigger will
//               cause a restart of the switched core power domain's power-on
//               reset state machine.
//
//               Glitch detector triggers are recorded accumulatively in
//               TRIG_STATUS. If the system is reset by a glitch detector
//               trigger, this is recorded in POWMAN_CHIP_RESET.
//
//               This register is Secure read/write only.
//               0x5bad -> Do not force the glitch detectors to be armed
//               0x0000 -> Force the glitch detectors to be armed. (Any value other than ARM_NO counts as YES)
#define GLITCH_DETECTOR_ARM_OFFSET _u(0x00000000)
#define GLITCH_DETECTOR_ARM_BITS   _u(0x0000ffff)
#define GLITCH_DETECTOR_ARM_RESET  _u(0x00005bad)
#define GLITCH_DETECTOR_ARM_MSB    _u(15)
#define GLITCH_DETECTOR_ARM_LSB    _u(0)
#define GLITCH_DETECTOR_ARM_ACCESS "RW"
#define GLITCH_DETECTOR_ARM_VALUE_NO _u(0x5bad)
#define GLITCH_DETECTOR_ARM_VALUE_YES _u(0x0000)
// =============================================================================
// Register    : GLITCH_DETECTOR_DISARM
// Description : None
//               Forcibly disarm the glitch detectors, if they are armed by OTP.
//               Ignored if ARM is YES.
//
//               This register is Secure read/write only.
//               0x0000 -> Do not disarm the glitch detectors. (Any value other than DISARM_YES counts as NO)
//               0xdcaf -> Disarm the glitch detectors
#define GLITCH_DETECTOR_DISARM_OFFSET _u(0x00000004)
#define GLITCH_DETECTOR_DISARM_BITS   _u(0x0000ffff)
#define GLITCH_DETECTOR_DISARM_RESET  _u(0x00000000)
#define GLITCH_DETECTOR_DISARM_MSB    _u(15)
#define GLITCH_DETECTOR_DISARM_LSB    _u(0)
#define GLITCH_DETECTOR_DISARM_ACCESS "RW"
#define GLITCH_DETECTOR_DISARM_VALUE_NO _u(0x0000)
#define GLITCH_DETECTOR_DISARM_VALUE_YES _u(0xdcaf)
// =============================================================================
// Register    : GLITCH_DETECTOR_SENSITIVITY
// Description : Adjust the sensitivity of glitch detectors to values other than
//               their OTP-provided defaults.
//
//               This register is Secure read/write only.
#define GLITCH_DETECTOR_SENSITIVITY_OFFSET _u(0x00000008)
#define GLITCH_DETECTOR_SENSITIVITY_BITS   _u(0xff00ffff)
#define GLITCH_DETECTOR_SENSITIVITY_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : GLITCH_DETECTOR_SENSITIVITY_DEFAULT
//               0x00 -> Use the default sensitivity configured in OTP for all detectors. (Any value other than DEFAULT_NO counts as YES)
//               0xde -> Do not use the default sensitivity configured in OTP. Instead use the value from this register.
#define GLITCH_DETECTOR_SENSITIVITY_DEFAULT_RESET  _u(0x00)
#define GLITCH_DETECTOR_SENSITIVITY_DEFAULT_BITS   _u(0xff000000)
#define GLITCH_DETECTOR_SENSITIVITY_DEFAULT_MSB    _u(31)
#define GLITCH_DETECTOR_SENSITIVITY_DEFAULT_LSB    _u(24)
#define GLITCH_DETECTOR_SENSITIVITY_DEFAULT_ACCESS "RW"
#define GLITCH_DETECTOR_SENSITIVITY_DEFAULT_VALUE_YES _u(0x00)
#define GLITCH_DETECTOR_SENSITIVITY_DEFAULT_VALUE_NO _u(0xde)
// -----------------------------------------------------------------------------
// Field       : GLITCH_DETECTOR_SENSITIVITY_DET3_INV
// Description : Must be the inverse of DET3, else the default value is used.
#define GLITCH_DETECTOR_SENSITIVITY_DET3_INV_RESET  _u(0x0)
#define GLITCH_DETECTOR_SENSITIVITY_DET3_INV_BITS   _u(0x0000c000)
#define GLITCH_DETECTOR_SENSITIVITY_DET3_INV_MSB    _u(15)
#define GLITCH_DETECTOR_SENSITIVITY_DET3_INV_LSB    _u(14)
#define GLITCH_DETECTOR_SENSITIVITY_DET3_INV_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : GLITCH_DETECTOR_SENSITIVITY_DET2_INV
// Description : Must be the inverse of DET2, else the default value is used.
#define GLITCH_DETECTOR_SENSITIVITY_DET2_INV_RESET  _u(0x0)
#define GLITCH_DETECTOR_SENSITIVITY_DET2_INV_BITS   _u(0x00003000)
#define GLITCH_DETECTOR_SENSITIVITY_DET2_INV_MSB    _u(13)
#define GLITCH_DETECTOR_SENSITIVITY_DET2_INV_LSB    _u(12)
#define GLITCH_DETECTOR_SENSITIVITY_DET2_INV_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : GLITCH_DETECTOR_SENSITIVITY_DET1_INV
// Description : Must be the inverse of DET1, else the default value is used.
#define GLITCH_DETECTOR_SENSITIVITY_DET1_INV_RESET  _u(0x0)
#define GLITCH_DETECTOR_SENSITIVITY_DET1_INV_BITS   _u(0x00000c00)
#define GLITCH_DETECTOR_SENSITIVITY_DET1_INV_MSB    _u(11)
#define GLITCH_DETECTOR_SENSITIVITY_DET1_INV_LSB    _u(10)
#define GLITCH_DETECTOR_SENSITIVITY_DET1_INV_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : GLITCH_DETECTOR_SENSITIVITY_DET0_INV
// Description : Must be the inverse of DET0, else the default value is used.
#define GLITCH_DETECTOR_SENSITIVITY_DET0_INV_RESET  _u(0x0)
#define GLITCH_DETECTOR_SENSITIVITY_DET0_INV_BITS   _u(0x00000300)
#define GLITCH_DETECTOR_SENSITIVITY_DET0_INV_MSB    _u(9)
#define GLITCH_DETECTOR_SENSITIVITY_DET0_INV_LSB    _u(8)
#define GLITCH_DETECTOR_SENSITIVITY_DET0_INV_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : GLITCH_DETECTOR_SENSITIVITY_DET3
// Description : Set sensitivity for detector 3. Higher values are more
//               sensitive.
#define GLITCH_DETECTOR_SENSITIVITY_DET3_RESET  _u(0x0)
#define GLITCH_DETECTOR_SENSITIVITY_DET3_BITS   _u(0x000000c0)
#define GLITCH_DETECTOR_SENSITIVITY_DET3_MSB    _u(7)
#define GLITCH_DETECTOR_SENSITIVITY_DET3_LSB    _u(6)
#define GLITCH_DETECTOR_SENSITIVITY_DET3_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : GLITCH_DETECTOR_SENSITIVITY_DET2
// Description : Set sensitivity for detector 2. Higher values are more
//               sensitive.
#define GLITCH_DETECTOR_SENSITIVITY_DET2_RESET  _u(0x0)
#define GLITCH_DETECTOR_SENSITIVITY_DET2_BITS   _u(0x00000030)
#define GLITCH_DETECTOR_SENSITIVITY_DET2_MSB    _u(5)
#define GLITCH_DETECTOR_SENSITIVITY_DET2_LSB    _u(4)
#define GLITCH_DETECTOR_SENSITIVITY_DET2_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : GLITCH_DETECTOR_SENSITIVITY_DET1
// Description : Set sensitivity for detector 1. Higher values are more
//               sensitive.
#define GLITCH_DETECTOR_SENSITIVITY_DET1_RESET  _u(0x0)
#define GLITCH_DETECTOR_SENSITIVITY_DET1_BITS   _u(0x0000000c)
#define GLITCH_DETECTOR_SENSITIVITY_DET1_MSB    _u(3)
#define GLITCH_DETECTOR_SENSITIVITY_DET1_LSB    _u(2)
#define GLITCH_DETECTOR_SENSITIVITY_DET1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : GLITCH_DETECTOR_SENSITIVITY_DET0
// Description : Set sensitivity for detector 0. Higher values are more
//               sensitive.
#define GLITCH_DETECTOR_SENSITIVITY_DET0_RESET  _u(0x0)
#define GLITCH_DETECTOR_SENSITIVITY_DET0_BITS   _u(0x00000003)
#define GLITCH_DETECTOR_SENSITIVITY_DET0_MSB    _u(1)
#define GLITCH_DETECTOR_SENSITIVITY_DET0_LSB    _u(0)
#define GLITCH_DETECTOR_SENSITIVITY_DET0_ACCESS "RW"
// =============================================================================
// Register    : GLITCH_DETECTOR_LOCK
// Description : None
//               Write any nonzero value to disable writes to ARM, DISARM,
//               SENSITIVITY and LOCK. This register is Secure read/write only.
#define GLITCH_DETECTOR_LOCK_OFFSET _u(0x0000000c)
#define GLITCH_DETECTOR_LOCK_BITS   _u(0x000000ff)
#define GLITCH_DETECTOR_LOCK_RESET  _u(0x00000000)
#define GLITCH_DETECTOR_LOCK_MSB    _u(7)
#define GLITCH_DETECTOR_LOCK_LSB    _u(0)
#define GLITCH_DETECTOR_LOCK_ACCESS "RW"
// =============================================================================
// Register    : GLITCH_DETECTOR_TRIG_STATUS
// Description : Set when a detector output triggers. Write-1-clear.
//
//               (May immediately return high if the detector remains in a
//               failed state. Detectors can only be cleared by a full reset of
//               the switched core power domain.)
//
//               This register is Secure read/write only.
#define GLITCH_DETECTOR_TRIG_STATUS_OFFSET _u(0x00000010)
#define GLITCH_DETECTOR_TRIG_STATUS_BITS   _u(0x0000000f)
#define GLITCH_DETECTOR_TRIG_STATUS_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : GLITCH_DETECTOR_TRIG_STATUS_DET3
#define GLITCH_DETECTOR_TRIG_STATUS_DET3_RESET  _u(0x0)
#define GLITCH_DETECTOR_TRIG_STATUS_DET3_BITS   _u(0x00000008)
#define GLITCH_DETECTOR_TRIG_STATUS_DET3_MSB    _u(3)
#define GLITCH_DETECTOR_TRIG_STATUS_DET3_LSB    _u(3)
#define GLITCH_DETECTOR_TRIG_STATUS_DET3_ACCESS "WC"
// -----------------------------------------------------------------------------
// Field       : GLITCH_DETECTOR_TRIG_STATUS_DET2
#define GLITCH_DETECTOR_TRIG_STATUS_DET2_RESET  _u(0x0)
#define GLITCH_DETECTOR_TRIG_STATUS_DET2_BITS   _u(0x00000004)
#define GLITCH_DETECTOR_TRIG_STATUS_DET2_MSB    _u(2)
#define GLITCH_DETECTOR_TRIG_STATUS_DET2_LSB    _u(2)
#define GLITCH_DETECTOR_TRIG_STATUS_DET2_ACCESS "WC"
// -----------------------------------------------------------------------------
// Field       : GLITCH_DETECTOR_TRIG_STATUS_DET1
#define GLITCH_DETECTOR_TRIG_STATUS_DET1_RESET  _u(0x0)
#define GLITCH_DETECTOR_TRIG_STATUS_DET1_BITS   _u(0x00000002)
#define GLITCH_DETECTOR_TRIG_STATUS_DET1_MSB    _u(1)
#define GLITCH_DETECTOR_TRIG_STATUS_DET1_LSB    _u(1)
#define GLITCH_DETECTOR_TRIG_STATUS_DET1_ACCESS "WC"
// -----------------------------------------------------------------------------
// Field       : GLITCH_DETECTOR_TRIG_STATUS_DET0
#define GLITCH_DETECTOR_TRIG_STATUS_DET0_RESET  _u(0x0)
#define GLITCH_DETECTOR_TRIG_STATUS_DET0_BITS   _u(0x00000001)
#define GLITCH_DETECTOR_TRIG_STATUS_DET0_MSB    _u(0)
#define GLITCH_DETECTOR_TRIG_STATUS_DET0_LSB    _u(0)
#define GLITCH_DETECTOR_TRIG_STATUS_DET0_ACCESS "WC"
// =============================================================================
// Register    : GLITCH_DETECTOR_TRIG_FORCE
// Description : Simulate the firing of one or more detectors. Writing ones to
//               this register will set the matching bits in STATUS_TRIG.
//
//               If the glitch detectors are currently armed, writing ones will
//               also immediately reset the switched core power domain, and set
//               the reset reason latches in POWMAN_CHIP_RESET to indicate a
//               glitch detector resets.
//
//               This register is Secure read/write only.
#define GLITCH_DETECTOR_TRIG_FORCE_OFFSET _u(0x00000014)
#define GLITCH_DETECTOR_TRIG_FORCE_BITS   _u(0x0000000f)
#define GLITCH_DETECTOR_TRIG_FORCE_RESET  _u(0x00000000)
#define GLITCH_DETECTOR_TRIG_FORCE_MSB    _u(3)
#define GLITCH_DETECTOR_TRIG_FORCE_LSB    _u(0)
#define GLITCH_DETECTOR_TRIG_FORCE_ACCESS "SC"
// =============================================================================
#endif // _HARDWARE_REGS_GLITCH_DETECTOR_H

