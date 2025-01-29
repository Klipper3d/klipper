// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _HARDWARE_STRUCTS_M0PLUS_H
#define _HARDWARE_STRUCTS_M0PLUS_H

/**
 * \file rp2040/m0plus.h
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
    uint32_t _pad0[14340];
 
    _REG_(M0PLUS_SYST_CSR_OFFSET) // M0PLUS_SYST_CSR
    // SysTick Control and Status Register
    // 0x00010000 [16]    COUNTFLAG    (0) Returns 1 if timer counted to 0 since last time this was read
    // 0x00000004 [2]     CLKSOURCE    (0) SysTick clock source
    // 0x00000002 [1]     TICKINT      (0) Enables SysTick exception request: +
    // 0x00000001 [0]     ENABLE       (0) Enable SysTick counter: +
    io_rw_32 syst_csr;
 
    _REG_(M0PLUS_SYST_RVR_OFFSET) // M0PLUS_SYST_RVR
    // SysTick Reload Value Register
    // 0x00ffffff [23:0]  RELOAD       (0x000000) Value to load into the SysTick Current Value Register...
    io_rw_32 syst_rvr;
 
    _REG_(M0PLUS_SYST_CVR_OFFSET) // M0PLUS_SYST_CVR
    // SysTick Current Value Register
    // 0x00ffffff [23:0]  CURRENT      (0x000000) Reads return the current value of the SysTick counter
    io_rw_32 syst_cvr;
 
    _REG_(M0PLUS_SYST_CALIB_OFFSET) // M0PLUS_SYST_CALIB
    // SysTick Calibration Value Register
    // 0x80000000 [31]    NOREF        (0) If reads as 1, the Reference clock is not provided - the...
    // 0x40000000 [30]    SKEW         (0) If reads as 1, the calibration value for 10ms is inexact...
    // 0x00ffffff [23:0]  TENMS        (0x000000) An optional Reload value to be used for 10ms (100Hz)...
    io_ro_32 syst_calib;
 
    uint32_t _pad1[56];
 
    _REG_(M0PLUS_NVIC_ISER_OFFSET) // M0PLUS_NVIC_ISER
    // Interrupt Set-Enable Register
    // 0xffffffff [31:0]  SETENA       (0x00000000) Interrupt set-enable bits
    io_rw_32 nvic_iser;
 
    uint32_t _pad2[31];
 
    _REG_(M0PLUS_NVIC_ICER_OFFSET) // M0PLUS_NVIC_ICER
    // Interrupt Clear-Enable Register
    // 0xffffffff [31:0]  CLRENA       (0x00000000) Interrupt clear-enable bits
    io_rw_32 nvic_icer;
 
    uint32_t _pad3[31];
 
    _REG_(M0PLUS_NVIC_ISPR_OFFSET) // M0PLUS_NVIC_ISPR
    // Interrupt Set-Pending Register
    // 0xffffffff [31:0]  SETPEND      (0x00000000) Interrupt set-pending bits
    io_rw_32 nvic_ispr;
 
    uint32_t _pad4[31];
 
    _REG_(M0PLUS_NVIC_ICPR_OFFSET) // M0PLUS_NVIC_ICPR
    // Interrupt Clear-Pending Register
    // 0xffffffff [31:0]  CLRPEND      (0x00000000) Interrupt clear-pending bits
    io_rw_32 nvic_icpr;
 
    uint32_t _pad5[95];
 
    // (Description copied from array index 0 register M0PLUS_NVIC_IPR0 applies similarly to other array indexes)
    _REG_(M0PLUS_NVIC_IPR0_OFFSET) // M0PLUS_NVIC_IPR0
    // Interrupt Priority Register 0
    // 0xc0000000 [31:30] IP_3         (0x0) Priority of interrupt 3
    // 0x00c00000 [23:22] IP_2         (0x0) Priority of interrupt 2
    // 0x0000c000 [15:14] IP_1         (0x0) Priority of interrupt 1
    // 0x000000c0 [7:6]   IP_0         (0x0) Priority of interrupt 0
    io_rw_32 nvic_ipr[8];
 
    uint32_t _pad6[568];
 
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
 
    _REG_(M0PLUS_CCR_OFFSET) // M0PLUS_CCR
    // Configuration and Control Register
    // 0x00000200 [9]     STKALIGN     (0) Always reads as one, indicates 8-byte stack alignment on...
    // 0x00000008 [3]     UNALIGN_TRP  (0) Always reads as one, indicates that all unaligned...
    io_ro_32 ccr;
 
    uint32_t _pad7;
 
    // (Description copied from array index 0 register M0PLUS_SHPR2 applies similarly to other array indexes)
    _REG_(M0PLUS_SHPR2_OFFSET) // M0PLUS_SHPR2
    // System Handler Priority Register 2
    // 0xc0000000 [31:30] PRI_11       (0x0) Priority of system handler 11, SVCall
    io_rw_32 shpr[2];
 
    _REG_(M0PLUS_SHCSR_OFFSET) // M0PLUS_SHCSR
    // System Handler Control and State Register
    // 0x00008000 [15]    SVCALLPENDED (0) Reads as 1 if SVCall is Pending
    io_rw_32 shcsr;
 
    uint32_t _pad8[26];
 
    _REG_(M0PLUS_MPU_TYPE_OFFSET) // M0PLUS_MPU_TYPE
    // MPU Type Register
    // 0x00ff0000 [23:16] IREGION      (0x00) Instruction region
    // 0x0000ff00 [15:8]  DREGION      (0x08) Number of regions supported by the MPU
    // 0x00000001 [0]     SEPARATE     (0) Indicates support for separate instruction and data address maps
    io_ro_32 mpu_type;
 
    _REG_(M0PLUS_MPU_CTRL_OFFSET) // M0PLUS_MPU_CTRL
    // MPU Control Register
    // 0x00000004 [2]     PRIVDEFENA   (0) Controls whether the default memory map is enabled as a...
    // 0x00000002 [1]     HFNMIENA     (0) Controls the use of the MPU for HardFaults and NMIs
    // 0x00000001 [0]     ENABLE       (0) Enables the MPU
    io_rw_32 mpu_ctrl;
 
    _REG_(M0PLUS_MPU_RNR_OFFSET) // M0PLUS_MPU_RNR
    // MPU Region Number Register
    // 0x0000000f [3:0]   REGION       (0x0) Indicates the MPU region referenced by the MPU_RBAR and...
    io_rw_32 mpu_rnr;
 
    _REG_(M0PLUS_MPU_RBAR_OFFSET) // M0PLUS_MPU_RBAR
    // MPU Region Base Address Register
    // 0xffffff00 [31:8]  ADDR         (0x000000) Base address of the region
    // 0x00000010 [4]     VALID        (0) On writes, indicates whether the write must update the...
    // 0x0000000f [3:0]   REGION       (0x0) On writes, specifies the number of the region whose base...
    io_rw_32 mpu_rbar;
 
    _REG_(M0PLUS_MPU_RASR_OFFSET) // M0PLUS_MPU_RASR
    // MPU Region Attribute and Size Register
    // 0xffff0000 [31:16] ATTRS        (0x0000) The MPU Region Attribute field
    // 0x0000ff00 [15:8]  SRD          (0x00) Subregion Disable
    // 0x0000003e [5:1]   SIZE         (0x00) Indicates the region size
    // 0x00000001 [0]     ENABLE       (0) Enables the region
    io_rw_32 mpu_rasr;
} m0plus_hw_t;

#define ppb_hw ((m0plus_hw_t *)PPB_BASE)
static_assert(sizeof (m0plus_hw_t) == 0xeda4, "");

#endif // _HARDWARE_STRUCTS_M0PLUS_H

