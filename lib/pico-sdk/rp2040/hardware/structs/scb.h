// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _HARDWARE_STRUCTS_SCB_H
#define _HARDWARE_STRUCTS_SCB_H

/**
 * \file rp2040/scb.h
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
    _REG_(M0PLUS_CPUID_OFFSET) // M0PLUS_CPUID
    // CPUID Base Register
    // 0xff000000 [31:24] IMPLEMENTER  (0x41) Implementor code: 0x41 = ARM
    // 0x00f00000 [23:20] VARIANT      (0x0) Major revision number n in the rnpm revision status: +
    // 0x000f0000 [19:16] ARCHITECTURE (0xc) Constant that defines the architecture of the processor: +
    // 0x0000fff0 [15:4]  PARTNO       (0xc60) Number of processor within family: 0xC60 = Cortex-M0+
    // 0x0000000f [3:0]   REVISION     (0x1) Minor revision number m in the rnpm revision status: +
    io_ro_32 cpuid;
 
    _REG_(M0PLUS_ICSR_OFFSET) // M0PLUS_ICSR
    // Interrupt Control and State Register
    // 0x80000000 [31]    NMIPENDSET   (0) Setting this bit will activate an NMI
    // 0x10000000 [28]    PENDSVSET    (0) PendSV set-pending bit
    // 0x08000000 [27]    PENDSVCLR    (0) PendSV clear-pending bit
    // 0x04000000 [26]    PENDSTSET    (0) SysTick exception set-pending bit
    // 0x02000000 [25]    PENDSTCLR    (0) SysTick exception clear-pending bit
    // 0x00800000 [23]    ISRPREEMPT   (0) The system can only access this bit when the core is halted
    // 0x00400000 [22]    ISRPENDING   (0) External interrupt pending flag
    // 0x001ff000 [20:12] VECTPENDING  (0x000) Indicates the exception number for the highest priority...
    // 0x000001ff [8:0]   VECTACTIVE   (0x000) Active exception number field
    io_rw_32 icsr;
 
    _REG_(M0PLUS_VTOR_OFFSET) // M0PLUS_VTOR
    // Vector Table Offset Register
    // 0xffffff00 [31:8]  TBLOFF       (0x000000) Bits [31:8] of the indicate the vector table offset address
    io_rw_32 vtor;
 
    _REG_(M0PLUS_AIRCR_OFFSET) // M0PLUS_AIRCR
    // Application Interrupt and Reset Control Register
    // 0xffff0000 [31:16] VECTKEY      (0x0000) Register key: +
    // 0x00008000 [15]    ENDIANESS    (0) Data endianness implemented: +
    // 0x00000004 [2]     SYSRESETREQ  (0) Writing 1 to this bit causes the SYSRESETREQ signal to...
    // 0x00000002 [1]     VECTCLRACTIVE (0) Clears all active state information for fixed and...
    io_rw_32 aircr;
 
    _REG_(M0PLUS_SCR_OFFSET) // M0PLUS_SCR
    // System Control Register
    // 0x00000010 [4]     SEVONPEND    (0) Send Event on Pending bit: +
    // 0x00000004 [2]     SLEEPDEEP    (0) Controls whether the processor uses sleep or deep sleep...
    // 0x00000002 [1]     SLEEPONEXIT  (0) Indicates sleep-on-exit when returning from Handler mode...
    io_rw_32 scr;
} armv6m_scb_hw_t;

#define scb_hw ((armv6m_scb_hw_t *)(PPB_BASE + M0PLUS_CPUID_OFFSET))
static_assert(sizeof (armv6m_scb_hw_t) == 0x0014, "");

#endif // _HARDWARE_STRUCTS_SCB_H

