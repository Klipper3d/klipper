// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _HARDWARE_STRUCTS_OTP_H
#define _HARDWARE_STRUCTS_OTP_H

/**
 * \file rp2350/otp.h
 */

#include "hardware/address_mapped.h"
#include "hardware/regs/otp.h"

// Reference to datasheet: https://datasheets.raspberrypi.com/rp2350/rp2350-datasheet.pdf#tab-registerlist_otp
//
// The _REG_ macro is intended to help make the register navigable in your IDE (for example, using the "Go to Definition" feature)
// _REG_(x) will link to the corresponding register in hardware/regs/otp.h.
//
// Bit-field descriptions are of the form:
// BITMASK [BITRANGE] FIELDNAME (RESETVALUE) DESCRIPTION

typedef struct {
    // (Description copied from array index 0 register OTP_SW_LOCK0 applies similarly to other array indexes)
    _REG_(OTP_SW_LOCK0_OFFSET) // OTP_SW_LOCK0
    // Software lock register for page 0.
    // 0x0000000c [3:2]   NSEC         (-) Non-secure lock status
    // 0x00000003 [1:0]   SEC          (-) Secure lock status
    io_rw_32 sw_lock[64];
 
    _REG_(OTP_SBPI_INSTR_OFFSET) // OTP_SBPI_INSTR
    // Dispatch instructions to the SBPI interface, used for programming the OTP fuses
    // 0x40000000 [30]    EXEC         (0) Execute instruction
    // 0x20000000 [29]    IS_WR        (0) Payload type is write
    // 0x10000000 [28]    HAS_PAYLOAD  (0) Instruction has payload (data to be written or to be read)
    // 0x0f000000 [27:24] PAYLOAD_SIZE_M1 (0x0) Instruction payload size in bytes minus 1
    // 0x00ff0000 [23:16] TARGET       (0x00) Instruction target, it can be PMC (0x3a) or DAP (0x02)
    // 0x0000ff00 [15:8]  CMD          (0x00) 
    // 0x000000ff [7:0]   SHORT_WDATA  (0x00) wdata to be used only when payload_size_m1=0
    io_rw_32 sbpi_instr;
 
    // (Description copied from array index 0 register OTP_SBPI_WDATA_0 applies similarly to other array indexes)
    _REG_(OTP_SBPI_WDATA_0_OFFSET) // OTP_SBPI_WDATA_0
    // SBPI write payload bytes 3
    // 0xffffffff [31:0]  SBPI_WDATA_0 (0x00000000) 
    io_rw_32 sbpi_wdata[4];
 
    // (Description copied from array index 0 register OTP_SBPI_RDATA_0 applies similarly to other array indexes)
    _REG_(OTP_SBPI_RDATA_0_OFFSET) // OTP_SBPI_RDATA_0
    // Read payload bytes 3
    // 0xffffffff [31:0]  SBPI_RDATA_0 (0x00000000) 
    io_ro_32 sbpi_rdata[4];
 
    _REG_(OTP_SBPI_STATUS_OFFSET) // OTP_SBPI_STATUS
    // 0x00ff0000 [23:16] MISO         (-) SBPI MISO (master in - slave out): response from SBPI
    // 0x00001000 [12]    FLAG         (-) SBPI flag
    // 0x00000100 [8]     INSTR_MISS   (0) Last instruction missed (dropped), as the previous has...
    // 0x00000010 [4]     INSTR_DONE   (0) Last instruction done
    // 0x00000001 [0]     RDATA_VLD    (0) Read command has returned data
    io_rw_32 sbpi_status;
 
    _REG_(OTP_USR_OFFSET) // OTP_USR
    // Controls for APB data read interface (USER interface)
    // 0x00000010 [4]     PD           (0) Power-down; 1 disables current reference
    // 0x00000001 [0]     DCTRL        (1) 1 enables USER interface; 0 disables USER interface...
    io_rw_32 usr;
 
    _REG_(OTP_DBG_OFFSET) // OTP_DBG
    // Debug for OTP power-on state machine
    // 0x00001000 [12]    CUSTOMER_RMA_FLAG (-) The chip is in RMA mode
    // 0x000000f0 [7:4]   PSM_STATE    (-) Monitor the PSM FSM's state
    // 0x00000008 [3]     ROSC_UP      (-) Ring oscillator is up and running
    // 0x00000004 [2]     ROSC_UP_SEEN (0) Ring oscillator was seen up and running
    // 0x00000002 [1]     BOOT_DONE    (-) PSM boot done status flag
    // 0x00000001 [0]     PSM_DONE     (-) PSM done status flag
    io_rw_32 dbg;
 
    uint32_t _pad0;
 
    _REG_(OTP_BIST_OFFSET) // OTP_BIST
    // During BIST, count address locations that have at least one leaky bit
    // 0x40000000 [30]    CNT_FAIL     (-) Flag if the count of address locations with at least one...
    // 0x20000000 [29]    CNT_CLR      (0) Clear counter before use
    // 0x10000000 [28]    CNT_ENA      (0) Enable the counter before the BIST function is initiated
    // 0x0fff0000 [27:16] CNT_MAX      (0xfff) The cnt_fail flag will be set if the number of leaky...
    // 0x00001fff [12:0]  CNT          (-) Number of locations that have at least one leaky bit
    io_rw_32 bist;
 
    // (Description copied from array index 0 register OTP_CRT_KEY_W0 applies similarly to other array indexes)
    _REG_(OTP_CRT_KEY_W0_OFFSET) // OTP_CRT_KEY_W0
    // Word 0 (bits 31
    // 0xffffffff [31:0]  CRT_KEY_W0   (0x00000000) 
    io_wo_32 crt_key_w[4];
 
    _REG_(OTP_CRITICAL_OFFSET) // OTP_CRITICAL
    // Quickly check values of critical flags read during boot up
    // 0x00020000 [17]    RISCV_DISABLE (0) 
    // 0x00010000 [16]    ARM_DISABLE  (0) 
    // 0x00000060 [6:5]   GLITCH_DETECTOR_SENS (0x0) 
    // 0x00000010 [4]     GLITCH_DETECTOR_ENABLE (0) 
    // 0x00000008 [3]     DEFAULT_ARCHSEL (0) 
    // 0x00000004 [2]     DEBUG_DISABLE (0) 
    // 0x00000002 [1]     SECURE_DEBUG_DISABLE (0) 
    // 0x00000001 [0]     SECURE_BOOT_ENABLE (0) 
    io_ro_32 critical;
 
    _REG_(OTP_KEY_VALID_OFFSET) // OTP_KEY_VALID
    // Which keys were valid (enrolled) at boot time
    // 0x000000ff [7:0]   KEY_VALID    (0x00) 
    io_ro_32 key_valid;
 
    _REG_(OTP_DEBUGEN_OFFSET) // OTP_DEBUGEN
    // Enable a debug feature that has been disabled. Debug features are disabled if one of the relevant critical boot flags is set in OTP (DEBUG_DISABLE or SECURE_DEBUG_DISABLE), OR if a debug key is marked valid in OTP, and the matching key value has not been supplied over SWD.
    // 0x00000100 [8]     MISC         (0) Enable other debug components
    // 0x00000008 [3]     PROC1_SECURE (0) Permit core 1's Mem-AP to generate Secure accesses,...
    // 0x00000004 [2]     PROC1        (0) Enable core 1's Mem-AP if it is currently disabled
    // 0x00000002 [1]     PROC0_SECURE (0) Permit core 0's Mem-AP to generate Secure accesses,...
    // 0x00000001 [0]     PROC0        (0) Enable core 0's Mem-AP if it is currently disabled
    io_rw_32 debugen;
 
    _REG_(OTP_DEBUGEN_LOCK_OFFSET) // OTP_DEBUGEN_LOCK
    // Write 1s to lock corresponding bits in DEBUGEN
    // 0x00000100 [8]     MISC         (0) Write 1 to lock the MISC bit of DEBUGEN
    // 0x00000008 [3]     PROC1_SECURE (0) Write 1 to lock the PROC1_SECURE bit of DEBUGEN
    // 0x00000004 [2]     PROC1        (0) Write 1 to lock the PROC1 bit of DEBUGEN
    // 0x00000002 [1]     PROC0_SECURE (0) Write 1 to lock the PROC0_SECURE bit of DEBUGEN
    // 0x00000001 [0]     PROC0        (0) Write 1 to lock the PROC0 bit of DEBUGEN
    io_rw_32 debugen_lock;
 
    _REG_(OTP_ARCHSEL_OFFSET) // OTP_ARCHSEL
    // Architecture select (Arm/RISC-V), applied on next processor reset. The default and allowable values of this register are constrained by the critical boot flags.
    // 0x00000002 [1]     CORE1        (0) Select architecture for core 1
    // 0x00000001 [0]     CORE0        (0) Select architecture for core 0
    io_rw_32 archsel;
 
    _REG_(OTP_ARCHSEL_STATUS_OFFSET) // OTP_ARCHSEL_STATUS
    // Get the current architecture select state of each core
    // 0x00000002 [1]     CORE1        (0) Current architecture for core 0
    // 0x00000001 [0]     CORE0        (0) Current architecture for core 0
    io_ro_32 archsel_status;
 
    _REG_(OTP_BOOTDIS_OFFSET) // OTP_BOOTDIS
    // Tell the bootrom to ignore scratch register boot vectors (both power manager and watchdog) on the next power up.
    // 0x00000002 [1]     NEXT         (0) This flag always ORs writes into its current contents
    // 0x00000001 [0]     NOW          (0) When the core is powered down, the current value of...
    io_rw_32 bootdis;
 
    _REG_(OTP_INTR_OFFSET) // OTP_INTR
    // Raw Interrupts
    // 0x00000010 [4]     APB_RD_NSEC_FAIL (0) 
    // 0x00000008 [3]     APB_RD_SEC_FAIL (0) 
    // 0x00000004 [2]     APB_DCTRL_FAIL (0) 
    // 0x00000002 [1]     SBPI_WR_FAIL (0) 
    // 0x00000001 [0]     SBPI_FLAG_N  (0) 
    io_rw_32 intr;
 
    _REG_(OTP_INTE_OFFSET) // OTP_INTE
    // Interrupt Enable
    // 0x00000010 [4]     APB_RD_NSEC_FAIL (0) 
    // 0x00000008 [3]     APB_RD_SEC_FAIL (0) 
    // 0x00000004 [2]     APB_DCTRL_FAIL (0) 
    // 0x00000002 [1]     SBPI_WR_FAIL (0) 
    // 0x00000001 [0]     SBPI_FLAG_N  (0) 
    io_rw_32 inte;
 
    _REG_(OTP_INTF_OFFSET) // OTP_INTF
    // Interrupt Force
    // 0x00000010 [4]     APB_RD_NSEC_FAIL (0) 
    // 0x00000008 [3]     APB_RD_SEC_FAIL (0) 
    // 0x00000004 [2]     APB_DCTRL_FAIL (0) 
    // 0x00000002 [1]     SBPI_WR_FAIL (0) 
    // 0x00000001 [0]     SBPI_FLAG_N  (0) 
    io_rw_32 intf;
 
    _REG_(OTP_INTS_OFFSET) // OTP_INTS
    // Interrupt status after masking & forcing
    // 0x00000010 [4]     APB_RD_NSEC_FAIL (0) 
    // 0x00000008 [3]     APB_RD_SEC_FAIL (0) 
    // 0x00000004 [2]     APB_DCTRL_FAIL (0) 
    // 0x00000002 [1]     SBPI_WR_FAIL (0) 
    // 0x00000001 [0]     SBPI_FLAG_N  (0) 
    io_ro_32 ints;
} otp_hw_t;

#define otp_hw ((otp_hw_t *)OTP_BASE)
static_assert(sizeof (otp_hw_t) == 0x0174, "");

#endif // _HARDWARE_STRUCTS_OTP_H

