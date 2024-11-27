// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _HARDWARE_STRUCTS_GLITCH_DETECTOR_H
#define _HARDWARE_STRUCTS_GLITCH_DETECTOR_H

/**
 * \file rp2350/glitch_detector.h
 */

#include "hardware/address_mapped.h"
#include "hardware/regs/glitch_detector.h"

// Reference to datasheet: https://datasheets.raspberrypi.com/rp2350/rp2350-datasheet.pdf#tab-registerlist_glitch_detector
//
// The _REG_ macro is intended to help make the register navigable in your IDE (for example, using the "Go to Definition" feature)
// _REG_(x) will link to the corresponding register in hardware/regs/glitch_detector.h.
//
// Bit-field descriptions are of the form:
// BITMASK [BITRANGE] FIELDNAME (RESETVALUE) DESCRIPTION

typedef struct {
    _REG_(GLITCH_DETECTOR_ARM_OFFSET) // GLITCH_DETECTOR_ARM
    // Forcibly arm the glitch detectors, if they are not already armed by OTP
    // 0x0000ffff [15:0]  ARM          (0x5bad) 
    io_rw_32 arm;
 
    _REG_(GLITCH_DETECTOR_DISARM_OFFSET) // GLITCH_DETECTOR_DISARM
    // 0x0000ffff [15:0]  DISARM       (0x0000) Forcibly disarm the glitch detectors, if they are armed by OTP
    io_rw_32 disarm;
 
    _REG_(GLITCH_DETECTOR_SENSITIVITY_OFFSET) // GLITCH_DETECTOR_SENSITIVITY
    // Adjust the sensitivity of glitch detectors to values other than their OTP-provided defaults
    // 0xff000000 [31:24] DEFAULT      (0x00) 
    // 0x0000c000 [15:14] DET3_INV     (0x0) Must be the inverse of DET3, else the default value is used
    // 0x00003000 [13:12] DET2_INV     (0x0) Must be the inverse of DET2, else the default value is used
    // 0x00000c00 [11:10] DET1_INV     (0x0) Must be the inverse of DET1, else the default value is used
    // 0x00000300 [9:8]   DET0_INV     (0x0) Must be the inverse of DET0, else the default value is used
    // 0x000000c0 [7:6]   DET3         (0x0) Set sensitivity for detector 3
    // 0x00000030 [5:4]   DET2         (0x0) Set sensitivity for detector 2
    // 0x0000000c [3:2]   DET1         (0x0) Set sensitivity for detector 1
    // 0x00000003 [1:0]   DET0         (0x0) Set sensitivity for detector 0
    io_rw_32 sensitivity;
 
    _REG_(GLITCH_DETECTOR_LOCK_OFFSET) // GLITCH_DETECTOR_LOCK
    // 0x000000ff [7:0]   LOCK         (0x00) Write any nonzero value to disable writes to ARM,...
    io_rw_32 lock;
 
    _REG_(GLITCH_DETECTOR_TRIG_STATUS_OFFSET) // GLITCH_DETECTOR_TRIG_STATUS
    // Set when a detector output triggers
    // 0x00000008 [3]     DET3         (0) 
    // 0x00000004 [2]     DET2         (0) 
    // 0x00000002 [1]     DET1         (0) 
    // 0x00000001 [0]     DET0         (0) 
    io_rw_32 trig_status;
 
    _REG_(GLITCH_DETECTOR_TRIG_FORCE_OFFSET) // GLITCH_DETECTOR_TRIG_FORCE
    // Simulate the firing of one or more detectors
    // 0x0000000f [3:0]   TRIG_FORCE   (0x0) 
    io_wo_32 trig_force;
} glitch_detector_hw_t;

#define glitch_detector_hw ((glitch_detector_hw_t *)GLITCH_DETECTOR_BASE)
static_assert(sizeof (glitch_detector_hw_t) == 0x0018, "");

#endif // _HARDWARE_STRUCTS_GLITCH_DETECTOR_H

