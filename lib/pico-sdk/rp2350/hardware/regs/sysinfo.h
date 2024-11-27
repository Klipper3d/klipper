// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
// =============================================================================
// Register block : SYSINFO
// Version        : 1
// Bus type       : apb
// =============================================================================
#ifndef _HARDWARE_REGS_SYSINFO_H
#define _HARDWARE_REGS_SYSINFO_H
// =============================================================================
// Register    : SYSINFO_CHIP_ID
// Description : JEDEC JEP-106 compliant chip identifier.
#define SYSINFO_CHIP_ID_OFFSET _u(0x00000000)
#define SYSINFO_CHIP_ID_BITS   _u(0xffffffff)
#define SYSINFO_CHIP_ID_RESET  _u(0x00000001)
// -----------------------------------------------------------------------------
// Field       : SYSINFO_CHIP_ID_REVISION
#define SYSINFO_CHIP_ID_REVISION_RESET  "-"
#define SYSINFO_CHIP_ID_REVISION_BITS   _u(0xf0000000)
#define SYSINFO_CHIP_ID_REVISION_MSB    _u(31)
#define SYSINFO_CHIP_ID_REVISION_LSB    _u(28)
#define SYSINFO_CHIP_ID_REVISION_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : SYSINFO_CHIP_ID_PART
#define SYSINFO_CHIP_ID_PART_RESET  "-"
#define SYSINFO_CHIP_ID_PART_BITS   _u(0x0ffff000)
#define SYSINFO_CHIP_ID_PART_MSB    _u(27)
#define SYSINFO_CHIP_ID_PART_LSB    _u(12)
#define SYSINFO_CHIP_ID_PART_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : SYSINFO_CHIP_ID_MANUFACTURER
#define SYSINFO_CHIP_ID_MANUFACTURER_RESET  "-"
#define SYSINFO_CHIP_ID_MANUFACTURER_BITS   _u(0x00000ffe)
#define SYSINFO_CHIP_ID_MANUFACTURER_MSB    _u(11)
#define SYSINFO_CHIP_ID_MANUFACTURER_LSB    _u(1)
#define SYSINFO_CHIP_ID_MANUFACTURER_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : SYSINFO_CHIP_ID_STOP_BIT
#define SYSINFO_CHIP_ID_STOP_BIT_RESET  _u(0x1)
#define SYSINFO_CHIP_ID_STOP_BIT_BITS   _u(0x00000001)
#define SYSINFO_CHIP_ID_STOP_BIT_MSB    _u(0)
#define SYSINFO_CHIP_ID_STOP_BIT_LSB    _u(0)
#define SYSINFO_CHIP_ID_STOP_BIT_ACCESS "RO"
// =============================================================================
// Register    : SYSINFO_PACKAGE_SEL
#define SYSINFO_PACKAGE_SEL_OFFSET _u(0x00000004)
#define SYSINFO_PACKAGE_SEL_BITS   _u(0x00000001)
#define SYSINFO_PACKAGE_SEL_RESET  _u(0x00000000)
#define SYSINFO_PACKAGE_SEL_MSB    _u(0)
#define SYSINFO_PACKAGE_SEL_LSB    _u(0)
#define SYSINFO_PACKAGE_SEL_ACCESS "RO"
// =============================================================================
// Register    : SYSINFO_PLATFORM
// Description : Platform register. Allows software to know what environment it
//               is running in during pre-production development. Post-
//               production, the PLATFORM is always ASIC, non-SIM.
#define SYSINFO_PLATFORM_OFFSET _u(0x00000008)
#define SYSINFO_PLATFORM_BITS   _u(0x0000001f)
#define SYSINFO_PLATFORM_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : SYSINFO_PLATFORM_GATESIM
#define SYSINFO_PLATFORM_GATESIM_RESET  "-"
#define SYSINFO_PLATFORM_GATESIM_BITS   _u(0x00000010)
#define SYSINFO_PLATFORM_GATESIM_MSB    _u(4)
#define SYSINFO_PLATFORM_GATESIM_LSB    _u(4)
#define SYSINFO_PLATFORM_GATESIM_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : SYSINFO_PLATFORM_BATCHSIM
#define SYSINFO_PLATFORM_BATCHSIM_RESET  "-"
#define SYSINFO_PLATFORM_BATCHSIM_BITS   _u(0x00000008)
#define SYSINFO_PLATFORM_BATCHSIM_MSB    _u(3)
#define SYSINFO_PLATFORM_BATCHSIM_LSB    _u(3)
#define SYSINFO_PLATFORM_BATCHSIM_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : SYSINFO_PLATFORM_HDLSIM
#define SYSINFO_PLATFORM_HDLSIM_RESET  "-"
#define SYSINFO_PLATFORM_HDLSIM_BITS   _u(0x00000004)
#define SYSINFO_PLATFORM_HDLSIM_MSB    _u(2)
#define SYSINFO_PLATFORM_HDLSIM_LSB    _u(2)
#define SYSINFO_PLATFORM_HDLSIM_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : SYSINFO_PLATFORM_ASIC
#define SYSINFO_PLATFORM_ASIC_RESET  "-"
#define SYSINFO_PLATFORM_ASIC_BITS   _u(0x00000002)
#define SYSINFO_PLATFORM_ASIC_MSB    _u(1)
#define SYSINFO_PLATFORM_ASIC_LSB    _u(1)
#define SYSINFO_PLATFORM_ASIC_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : SYSINFO_PLATFORM_FPGA
#define SYSINFO_PLATFORM_FPGA_RESET  "-"
#define SYSINFO_PLATFORM_FPGA_BITS   _u(0x00000001)
#define SYSINFO_PLATFORM_FPGA_MSB    _u(0)
#define SYSINFO_PLATFORM_FPGA_LSB    _u(0)
#define SYSINFO_PLATFORM_FPGA_ACCESS "RO"
// =============================================================================
// Register    : SYSINFO_GITREF_RP2350
// Description : Git hash of the chip source. Used to identify chip version.
#define SYSINFO_GITREF_RP2350_OFFSET _u(0x00000014)
#define SYSINFO_GITREF_RP2350_BITS   _u(0xffffffff)
#define SYSINFO_GITREF_RP2350_RESET  "-"
#define SYSINFO_GITREF_RP2350_MSB    _u(31)
#define SYSINFO_GITREF_RP2350_LSB    _u(0)
#define SYSINFO_GITREF_RP2350_ACCESS "RO"
// =============================================================================
#endif // _HARDWARE_REGS_SYSINFO_H

