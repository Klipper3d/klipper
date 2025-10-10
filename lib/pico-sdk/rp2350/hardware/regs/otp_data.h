// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
// =============================================================================
// Register block : OTP_DATA
// Version        : 1
// Bus type       : apb
// Description    : Predefined OTP data layout for RP2350
// =============================================================================
#ifndef _HARDWARE_REGS_OTP_DATA_H
#define _HARDWARE_REGS_OTP_DATA_H
// =============================================================================
// Register    : OTP_DATA_CHIPID0
// Description : Bits 15:0 of public device ID. (ECC)
//
//               The CHIPID0..3 rows contain a 64-bit random identifier for this
//               chip, which can be read from the USB bootloader PICOBOOT
//               interface or from the get_sys_info ROM API.
//
//               The number of random bits makes the occurrence of twins
//               exceedingly unlikely: for example, a fleet of a hundred million
//               devices has a 99.97% probability of no twinned IDs. This is
//               estimated to be lower than the occurrence of process errors in
//               the assignment of sequential random IDs, and for practical
//               purposes CHIPID may be treated as unique.
#define OTP_DATA_CHIPID0_ROW _u(0x00000000)
#define OTP_DATA_CHIPID0_BITS   _u(0x0000ffff)
#define OTP_DATA_CHIPID0_RESET  "-"
#define OTP_DATA_CHIPID0_WIDTH  _u(16)
#define OTP_DATA_CHIPID0_MSB    _u(15)
#define OTP_DATA_CHIPID0_LSB    _u(0)
#define OTP_DATA_CHIPID0_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_CHIPID1
// Description : Bits 31:16 of public device ID (ECC)
#define OTP_DATA_CHIPID1_ROW _u(0x00000001)
#define OTP_DATA_CHIPID1_BITS   _u(0x0000ffff)
#define OTP_DATA_CHIPID1_RESET  "-"
#define OTP_DATA_CHIPID1_WIDTH  _u(16)
#define OTP_DATA_CHIPID1_MSB    _u(15)
#define OTP_DATA_CHIPID1_LSB    _u(0)
#define OTP_DATA_CHIPID1_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_CHIPID2
// Description : Bits 47:32 of public device ID (ECC)
#define OTP_DATA_CHIPID2_ROW _u(0x00000002)
#define OTP_DATA_CHIPID2_BITS   _u(0x0000ffff)
#define OTP_DATA_CHIPID2_RESET  "-"
#define OTP_DATA_CHIPID2_WIDTH  _u(16)
#define OTP_DATA_CHIPID2_MSB    _u(15)
#define OTP_DATA_CHIPID2_LSB    _u(0)
#define OTP_DATA_CHIPID2_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_CHIPID3
// Description : Bits 63:48 of public device ID (ECC)
#define OTP_DATA_CHIPID3_ROW _u(0x00000003)
#define OTP_DATA_CHIPID3_BITS   _u(0x0000ffff)
#define OTP_DATA_CHIPID3_RESET  "-"
#define OTP_DATA_CHIPID3_WIDTH  _u(16)
#define OTP_DATA_CHIPID3_MSB    _u(15)
#define OTP_DATA_CHIPID3_LSB    _u(0)
#define OTP_DATA_CHIPID3_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_RANDID0
// Description : Bits 15:0 of private per-device random number (ECC)
//
//               The RANDID0..7 rows form a 128-bit random number generated
//               during device test.
//
//               This ID is not exposed through the USB PICOBOOT GET_INFO
//               command or the ROM `get_sys_info()` API. However note that the
//               USB PICOBOOT OTP access point can read the entirety of page 0,
//               so this value is not meaningfully private unless the USB
//               PICOBOOT interface is disabled via the
//               DISABLE_BOOTSEL_USB_PICOBOOT_IFC flag in BOOT_FLAGS0.
#define OTP_DATA_RANDID0_ROW _u(0x00000004)
#define OTP_DATA_RANDID0_BITS   _u(0x0000ffff)
#define OTP_DATA_RANDID0_RESET  "-"
#define OTP_DATA_RANDID0_WIDTH  _u(16)
#define OTP_DATA_RANDID0_MSB    _u(15)
#define OTP_DATA_RANDID0_LSB    _u(0)
#define OTP_DATA_RANDID0_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_RANDID1
// Description : Bits 31:16 of private per-device random number (ECC)
#define OTP_DATA_RANDID1_ROW _u(0x00000005)
#define OTP_DATA_RANDID1_BITS   _u(0x0000ffff)
#define OTP_DATA_RANDID1_RESET  "-"
#define OTP_DATA_RANDID1_WIDTH  _u(16)
#define OTP_DATA_RANDID1_MSB    _u(15)
#define OTP_DATA_RANDID1_LSB    _u(0)
#define OTP_DATA_RANDID1_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_RANDID2
// Description : Bits 47:32 of private per-device random number (ECC)
#define OTP_DATA_RANDID2_ROW _u(0x00000006)
#define OTP_DATA_RANDID2_BITS   _u(0x0000ffff)
#define OTP_DATA_RANDID2_RESET  "-"
#define OTP_DATA_RANDID2_WIDTH  _u(16)
#define OTP_DATA_RANDID2_MSB    _u(15)
#define OTP_DATA_RANDID2_LSB    _u(0)
#define OTP_DATA_RANDID2_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_RANDID3
// Description : Bits 63:48 of private per-device random number (ECC)
#define OTP_DATA_RANDID3_ROW _u(0x00000007)
#define OTP_DATA_RANDID3_BITS   _u(0x0000ffff)
#define OTP_DATA_RANDID3_RESET  "-"
#define OTP_DATA_RANDID3_WIDTH  _u(16)
#define OTP_DATA_RANDID3_MSB    _u(15)
#define OTP_DATA_RANDID3_LSB    _u(0)
#define OTP_DATA_RANDID3_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_RANDID4
// Description : Bits 79:64 of private per-device random number (ECC)
#define OTP_DATA_RANDID4_ROW _u(0x00000008)
#define OTP_DATA_RANDID4_BITS   _u(0x0000ffff)
#define OTP_DATA_RANDID4_RESET  "-"
#define OTP_DATA_RANDID4_WIDTH  _u(16)
#define OTP_DATA_RANDID4_MSB    _u(15)
#define OTP_DATA_RANDID4_LSB    _u(0)
#define OTP_DATA_RANDID4_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_RANDID5
// Description : Bits 95:80 of private per-device random number (ECC)
#define OTP_DATA_RANDID5_ROW _u(0x00000009)
#define OTP_DATA_RANDID5_BITS   _u(0x0000ffff)
#define OTP_DATA_RANDID5_RESET  "-"
#define OTP_DATA_RANDID5_WIDTH  _u(16)
#define OTP_DATA_RANDID5_MSB    _u(15)
#define OTP_DATA_RANDID5_LSB    _u(0)
#define OTP_DATA_RANDID5_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_RANDID6
// Description : Bits 111:96 of private per-device random number (ECC)
#define OTP_DATA_RANDID6_ROW _u(0x0000000a)
#define OTP_DATA_RANDID6_BITS   _u(0x0000ffff)
#define OTP_DATA_RANDID6_RESET  "-"
#define OTP_DATA_RANDID6_WIDTH  _u(16)
#define OTP_DATA_RANDID6_MSB    _u(15)
#define OTP_DATA_RANDID6_LSB    _u(0)
#define OTP_DATA_RANDID6_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_RANDID7
// Description : Bits 127:112 of private per-device random number (ECC)
#define OTP_DATA_RANDID7_ROW _u(0x0000000b)
#define OTP_DATA_RANDID7_BITS   _u(0x0000ffff)
#define OTP_DATA_RANDID7_RESET  "-"
#define OTP_DATA_RANDID7_WIDTH  _u(16)
#define OTP_DATA_RANDID7_MSB    _u(15)
#define OTP_DATA_RANDID7_LSB    _u(0)
#define OTP_DATA_RANDID7_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_ROSC_CALIB
// Description : Ring oscillator frequency in kHz, measured during manufacturing
//               (ECC)
//
//               This is measured at 1.1 V, at room temperature, with the ROSC
//               configuration registers in their reset state.
#define OTP_DATA_ROSC_CALIB_ROW _u(0x00000010)
#define OTP_DATA_ROSC_CALIB_BITS   _u(0x0000ffff)
#define OTP_DATA_ROSC_CALIB_RESET  "-"
#define OTP_DATA_ROSC_CALIB_WIDTH  _u(16)
#define OTP_DATA_ROSC_CALIB_MSB    _u(15)
#define OTP_DATA_ROSC_CALIB_LSB    _u(0)
#define OTP_DATA_ROSC_CALIB_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_LPOSC_CALIB
// Description : Low-power oscillator frequency in Hz, measured during
//               manufacturing (ECC)
//
//               This is measured at 1.1V, at room temperature, with the LPOSC
//               trim register in its reset state.
#define OTP_DATA_LPOSC_CALIB_ROW _u(0x00000011)
#define OTP_DATA_LPOSC_CALIB_BITS   _u(0x0000ffff)
#define OTP_DATA_LPOSC_CALIB_RESET  "-"
#define OTP_DATA_LPOSC_CALIB_WIDTH  _u(16)
#define OTP_DATA_LPOSC_CALIB_MSB    _u(15)
#define OTP_DATA_LPOSC_CALIB_LSB    _u(0)
#define OTP_DATA_LPOSC_CALIB_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_NUM_GPIOS
// Description : The number of main user GPIOs (bank 0). Should read 48 in the
//               QFN80 package, and 30 in the QFN60 package. (ECC)
#define OTP_DATA_NUM_GPIOS_ROW _u(0x00000018)
#define OTP_DATA_NUM_GPIOS_BITS   _u(0x000000ff)
#define OTP_DATA_NUM_GPIOS_RESET  "-"
#define OTP_DATA_NUM_GPIOS_WIDTH  _u(16)
#define OTP_DATA_NUM_GPIOS_MSB    _u(7)
#define OTP_DATA_NUM_GPIOS_LSB    _u(0)
#define OTP_DATA_NUM_GPIOS_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_INFO_CRC0
// Description : Lower 16 bits of CRC32 of OTP addresses 0x00 through 0x6b
//               (polynomial 0x4c11db7, input reflected, output reflected, seed
//               all-ones, final XOR all-ones) (ECC)
#define OTP_DATA_INFO_CRC0_ROW _u(0x00000036)
#define OTP_DATA_INFO_CRC0_BITS   _u(0x0000ffff)
#define OTP_DATA_INFO_CRC0_RESET  "-"
#define OTP_DATA_INFO_CRC0_WIDTH  _u(16)
#define OTP_DATA_INFO_CRC0_MSB    _u(15)
#define OTP_DATA_INFO_CRC0_LSB    _u(0)
#define OTP_DATA_INFO_CRC0_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_INFO_CRC1
// Description : Upper 16 bits of CRC32 of OTP addresses 0x00 through 0x6b (ECC)
#define OTP_DATA_INFO_CRC1_ROW _u(0x00000037)
#define OTP_DATA_INFO_CRC1_BITS   _u(0x0000ffff)
#define OTP_DATA_INFO_CRC1_RESET  "-"
#define OTP_DATA_INFO_CRC1_WIDTH  _u(16)
#define OTP_DATA_INFO_CRC1_MSB    _u(15)
#define OTP_DATA_INFO_CRC1_LSB    _u(0)
#define OTP_DATA_INFO_CRC1_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_CRIT0
// Description : Page 0 critical boot flags (RBIT-8)
#define OTP_DATA_CRIT0_ROW _u(0x00000038)
#define OTP_DATA_CRIT0_BITS   _u(0x00000003)
#define OTP_DATA_CRIT0_RESET  _u(0x00000000)
#define OTP_DATA_CRIT0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_CRIT0_RISCV_DISABLE
// Description : Permanently disable RISC-V processors (Hazard3)
#define OTP_DATA_CRIT0_RISCV_DISABLE_RESET  "-"
#define OTP_DATA_CRIT0_RISCV_DISABLE_BITS   _u(0x00000002)
#define OTP_DATA_CRIT0_RISCV_DISABLE_MSB    _u(1)
#define OTP_DATA_CRIT0_RISCV_DISABLE_LSB    _u(1)
#define OTP_DATA_CRIT0_RISCV_DISABLE_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_CRIT0_ARM_DISABLE
// Description : Permanently disable ARM processors (Cortex-M33)
#define OTP_DATA_CRIT0_ARM_DISABLE_RESET  "-"
#define OTP_DATA_CRIT0_ARM_DISABLE_BITS   _u(0x00000001)
#define OTP_DATA_CRIT0_ARM_DISABLE_MSB    _u(0)
#define OTP_DATA_CRIT0_ARM_DISABLE_LSB    _u(0)
#define OTP_DATA_CRIT0_ARM_DISABLE_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_CRIT0_R1
// Description : Redundant copy of CRIT0
#define OTP_DATA_CRIT0_R1_ROW _u(0x00000039)
#define OTP_DATA_CRIT0_R1_BITS   _u(0x00ffffff)
#define OTP_DATA_CRIT0_R1_RESET  "-"
#define OTP_DATA_CRIT0_R1_WIDTH  _u(24)
#define OTP_DATA_CRIT0_R1_MSB    _u(23)
#define OTP_DATA_CRIT0_R1_LSB    _u(0)
#define OTP_DATA_CRIT0_R1_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_CRIT0_R2
// Description : Redundant copy of CRIT0
#define OTP_DATA_CRIT0_R2_ROW _u(0x0000003a)
#define OTP_DATA_CRIT0_R2_BITS   _u(0x00ffffff)
#define OTP_DATA_CRIT0_R2_RESET  "-"
#define OTP_DATA_CRIT0_R2_WIDTH  _u(24)
#define OTP_DATA_CRIT0_R2_MSB    _u(23)
#define OTP_DATA_CRIT0_R2_LSB    _u(0)
#define OTP_DATA_CRIT0_R2_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_CRIT0_R3
// Description : Redundant copy of CRIT0
#define OTP_DATA_CRIT0_R3_ROW _u(0x0000003b)
#define OTP_DATA_CRIT0_R3_BITS   _u(0x00ffffff)
#define OTP_DATA_CRIT0_R3_RESET  "-"
#define OTP_DATA_CRIT0_R3_WIDTH  _u(24)
#define OTP_DATA_CRIT0_R3_MSB    _u(23)
#define OTP_DATA_CRIT0_R3_LSB    _u(0)
#define OTP_DATA_CRIT0_R3_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_CRIT0_R4
// Description : Redundant copy of CRIT0
#define OTP_DATA_CRIT0_R4_ROW _u(0x0000003c)
#define OTP_DATA_CRIT0_R4_BITS   _u(0x00ffffff)
#define OTP_DATA_CRIT0_R4_RESET  "-"
#define OTP_DATA_CRIT0_R4_WIDTH  _u(24)
#define OTP_DATA_CRIT0_R4_MSB    _u(23)
#define OTP_DATA_CRIT0_R4_LSB    _u(0)
#define OTP_DATA_CRIT0_R4_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_CRIT0_R5
// Description : Redundant copy of CRIT0
#define OTP_DATA_CRIT0_R5_ROW _u(0x0000003d)
#define OTP_DATA_CRIT0_R5_BITS   _u(0x00ffffff)
#define OTP_DATA_CRIT0_R5_RESET  "-"
#define OTP_DATA_CRIT0_R5_WIDTH  _u(24)
#define OTP_DATA_CRIT0_R5_MSB    _u(23)
#define OTP_DATA_CRIT0_R5_LSB    _u(0)
#define OTP_DATA_CRIT0_R5_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_CRIT0_R6
// Description : Redundant copy of CRIT0
#define OTP_DATA_CRIT0_R6_ROW _u(0x0000003e)
#define OTP_DATA_CRIT0_R6_BITS   _u(0x00ffffff)
#define OTP_DATA_CRIT0_R6_RESET  "-"
#define OTP_DATA_CRIT0_R6_WIDTH  _u(24)
#define OTP_DATA_CRIT0_R6_MSB    _u(23)
#define OTP_DATA_CRIT0_R6_LSB    _u(0)
#define OTP_DATA_CRIT0_R6_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_CRIT0_R7
// Description : Redundant copy of CRIT0
#define OTP_DATA_CRIT0_R7_ROW _u(0x0000003f)
#define OTP_DATA_CRIT0_R7_BITS   _u(0x00ffffff)
#define OTP_DATA_CRIT0_R7_RESET  "-"
#define OTP_DATA_CRIT0_R7_WIDTH  _u(24)
#define OTP_DATA_CRIT0_R7_MSB    _u(23)
#define OTP_DATA_CRIT0_R7_LSB    _u(0)
#define OTP_DATA_CRIT0_R7_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_CRIT1
// Description : Page 1 critical boot flags (RBIT-8)
#define OTP_DATA_CRIT1_ROW _u(0x00000040)
#define OTP_DATA_CRIT1_BITS   _u(0x0000007f)
#define OTP_DATA_CRIT1_RESET  _u(0x00000000)
#define OTP_DATA_CRIT1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_CRIT1_GLITCH_DETECTOR_SENS
// Description : Increase the sensitivity of the glitch detectors from their
//               default.
#define OTP_DATA_CRIT1_GLITCH_DETECTOR_SENS_RESET  "-"
#define OTP_DATA_CRIT1_GLITCH_DETECTOR_SENS_BITS   _u(0x00000060)
#define OTP_DATA_CRIT1_GLITCH_DETECTOR_SENS_MSB    _u(6)
#define OTP_DATA_CRIT1_GLITCH_DETECTOR_SENS_LSB    _u(5)
#define OTP_DATA_CRIT1_GLITCH_DETECTOR_SENS_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_CRIT1_GLITCH_DETECTOR_ENABLE
// Description : Arm the glitch detectors to reset the system if an abnormal
//               clock/power event is observed.
#define OTP_DATA_CRIT1_GLITCH_DETECTOR_ENABLE_RESET  "-"
#define OTP_DATA_CRIT1_GLITCH_DETECTOR_ENABLE_BITS   _u(0x00000010)
#define OTP_DATA_CRIT1_GLITCH_DETECTOR_ENABLE_MSB    _u(4)
#define OTP_DATA_CRIT1_GLITCH_DETECTOR_ENABLE_LSB    _u(4)
#define OTP_DATA_CRIT1_GLITCH_DETECTOR_ENABLE_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_CRIT1_BOOT_ARCH
// Description : Set the default boot architecture, 0=ARM 1=RISC-V. Ignored if
//               ARM_DISABLE, RISCV_DISABLE or SECURE_BOOT_ENABLE is set.
#define OTP_DATA_CRIT1_BOOT_ARCH_RESET  "-"
#define OTP_DATA_CRIT1_BOOT_ARCH_BITS   _u(0x00000008)
#define OTP_DATA_CRIT1_BOOT_ARCH_MSB    _u(3)
#define OTP_DATA_CRIT1_BOOT_ARCH_LSB    _u(3)
#define OTP_DATA_CRIT1_BOOT_ARCH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_CRIT1_DEBUG_DISABLE
// Description : Disable all debug access
#define OTP_DATA_CRIT1_DEBUG_DISABLE_RESET  "-"
#define OTP_DATA_CRIT1_DEBUG_DISABLE_BITS   _u(0x00000004)
#define OTP_DATA_CRIT1_DEBUG_DISABLE_MSB    _u(2)
#define OTP_DATA_CRIT1_DEBUG_DISABLE_LSB    _u(2)
#define OTP_DATA_CRIT1_DEBUG_DISABLE_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_CRIT1_SECURE_DEBUG_DISABLE
// Description : Disable Secure debug access
#define OTP_DATA_CRIT1_SECURE_DEBUG_DISABLE_RESET  "-"
#define OTP_DATA_CRIT1_SECURE_DEBUG_DISABLE_BITS   _u(0x00000002)
#define OTP_DATA_CRIT1_SECURE_DEBUG_DISABLE_MSB    _u(1)
#define OTP_DATA_CRIT1_SECURE_DEBUG_DISABLE_LSB    _u(1)
#define OTP_DATA_CRIT1_SECURE_DEBUG_DISABLE_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_CRIT1_SECURE_BOOT_ENABLE
// Description : Enable boot signature enforcement, and permanently disable the
//               RISC-V cores.
#define OTP_DATA_CRIT1_SECURE_BOOT_ENABLE_RESET  "-"
#define OTP_DATA_CRIT1_SECURE_BOOT_ENABLE_BITS   _u(0x00000001)
#define OTP_DATA_CRIT1_SECURE_BOOT_ENABLE_MSB    _u(0)
#define OTP_DATA_CRIT1_SECURE_BOOT_ENABLE_LSB    _u(0)
#define OTP_DATA_CRIT1_SECURE_BOOT_ENABLE_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_CRIT1_R1
// Description : Redundant copy of CRIT1
#define OTP_DATA_CRIT1_R1_ROW _u(0x00000041)
#define OTP_DATA_CRIT1_R1_BITS   _u(0x00ffffff)
#define OTP_DATA_CRIT1_R1_RESET  "-"
#define OTP_DATA_CRIT1_R1_WIDTH  _u(24)
#define OTP_DATA_CRIT1_R1_MSB    _u(23)
#define OTP_DATA_CRIT1_R1_LSB    _u(0)
#define OTP_DATA_CRIT1_R1_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_CRIT1_R2
// Description : Redundant copy of CRIT1
#define OTP_DATA_CRIT1_R2_ROW _u(0x00000042)
#define OTP_DATA_CRIT1_R2_BITS   _u(0x00ffffff)
#define OTP_DATA_CRIT1_R2_RESET  "-"
#define OTP_DATA_CRIT1_R2_WIDTH  _u(24)
#define OTP_DATA_CRIT1_R2_MSB    _u(23)
#define OTP_DATA_CRIT1_R2_LSB    _u(0)
#define OTP_DATA_CRIT1_R2_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_CRIT1_R3
// Description : Redundant copy of CRIT1
#define OTP_DATA_CRIT1_R3_ROW _u(0x00000043)
#define OTP_DATA_CRIT1_R3_BITS   _u(0x00ffffff)
#define OTP_DATA_CRIT1_R3_RESET  "-"
#define OTP_DATA_CRIT1_R3_WIDTH  _u(24)
#define OTP_DATA_CRIT1_R3_MSB    _u(23)
#define OTP_DATA_CRIT1_R3_LSB    _u(0)
#define OTP_DATA_CRIT1_R3_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_CRIT1_R4
// Description : Redundant copy of CRIT1
#define OTP_DATA_CRIT1_R4_ROW _u(0x00000044)
#define OTP_DATA_CRIT1_R4_BITS   _u(0x00ffffff)
#define OTP_DATA_CRIT1_R4_RESET  "-"
#define OTP_DATA_CRIT1_R4_WIDTH  _u(24)
#define OTP_DATA_CRIT1_R4_MSB    _u(23)
#define OTP_DATA_CRIT1_R4_LSB    _u(0)
#define OTP_DATA_CRIT1_R4_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_CRIT1_R5
// Description : Redundant copy of CRIT1
#define OTP_DATA_CRIT1_R5_ROW _u(0x00000045)
#define OTP_DATA_CRIT1_R5_BITS   _u(0x00ffffff)
#define OTP_DATA_CRIT1_R5_RESET  "-"
#define OTP_DATA_CRIT1_R5_WIDTH  _u(24)
#define OTP_DATA_CRIT1_R5_MSB    _u(23)
#define OTP_DATA_CRIT1_R5_LSB    _u(0)
#define OTP_DATA_CRIT1_R5_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_CRIT1_R6
// Description : Redundant copy of CRIT1
#define OTP_DATA_CRIT1_R6_ROW _u(0x00000046)
#define OTP_DATA_CRIT1_R6_BITS   _u(0x00ffffff)
#define OTP_DATA_CRIT1_R6_RESET  "-"
#define OTP_DATA_CRIT1_R6_WIDTH  _u(24)
#define OTP_DATA_CRIT1_R6_MSB    _u(23)
#define OTP_DATA_CRIT1_R6_LSB    _u(0)
#define OTP_DATA_CRIT1_R6_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_CRIT1_R7
// Description : Redundant copy of CRIT1
#define OTP_DATA_CRIT1_R7_ROW _u(0x00000047)
#define OTP_DATA_CRIT1_R7_BITS   _u(0x00ffffff)
#define OTP_DATA_CRIT1_R7_RESET  "-"
#define OTP_DATA_CRIT1_R7_WIDTH  _u(24)
#define OTP_DATA_CRIT1_R7_MSB    _u(23)
#define OTP_DATA_CRIT1_R7_LSB    _u(0)
#define OTP_DATA_CRIT1_R7_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOT_FLAGS0
// Description : Disable/Enable boot paths/features in the RP2350 mask ROM.
//               Disables always supersede enables. Enables are provided where
//               there are other configurations in OTP that must be valid.
//               (RBIT-3)
#define OTP_DATA_BOOT_FLAGS0_ROW _u(0x00000048)
#define OTP_DATA_BOOT_FLAGS0_BITS   _u(0x003fffff)
#define OTP_DATA_BOOT_FLAGS0_RESET  _u(0x00000000)
#define OTP_DATA_BOOT_FLAGS0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_BOOT_FLAGS0_DISABLE_SRAM_WINDOW_BOOT
#define OTP_DATA_BOOT_FLAGS0_DISABLE_SRAM_WINDOW_BOOT_RESET  "-"
#define OTP_DATA_BOOT_FLAGS0_DISABLE_SRAM_WINDOW_BOOT_BITS   _u(0x00200000)
#define OTP_DATA_BOOT_FLAGS0_DISABLE_SRAM_WINDOW_BOOT_MSB    _u(21)
#define OTP_DATA_BOOT_FLAGS0_DISABLE_SRAM_WINDOW_BOOT_LSB    _u(21)
#define OTP_DATA_BOOT_FLAGS0_DISABLE_SRAM_WINDOW_BOOT_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_BOOT_FLAGS0_DISABLE_XIP_ACCESS_ON_SRAM_ENTRY
// Description : Disable all access to XIP after entering an SRAM binary.
//
//               Note that this will cause bootrom APIs that access XIP to fail,
//               including APIs that interact with the partition table.
#define OTP_DATA_BOOT_FLAGS0_DISABLE_XIP_ACCESS_ON_SRAM_ENTRY_RESET  "-"
#define OTP_DATA_BOOT_FLAGS0_DISABLE_XIP_ACCESS_ON_SRAM_ENTRY_BITS   _u(0x00100000)
#define OTP_DATA_BOOT_FLAGS0_DISABLE_XIP_ACCESS_ON_SRAM_ENTRY_MSB    _u(20)
#define OTP_DATA_BOOT_FLAGS0_DISABLE_XIP_ACCESS_ON_SRAM_ENTRY_LSB    _u(20)
#define OTP_DATA_BOOT_FLAGS0_DISABLE_XIP_ACCESS_ON_SRAM_ENTRY_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_BOOT_FLAGS0_DISABLE_BOOTSEL_UART_BOOT
#define OTP_DATA_BOOT_FLAGS0_DISABLE_BOOTSEL_UART_BOOT_RESET  "-"
#define OTP_DATA_BOOT_FLAGS0_DISABLE_BOOTSEL_UART_BOOT_BITS   _u(0x00080000)
#define OTP_DATA_BOOT_FLAGS0_DISABLE_BOOTSEL_UART_BOOT_MSB    _u(19)
#define OTP_DATA_BOOT_FLAGS0_DISABLE_BOOTSEL_UART_BOOT_LSB    _u(19)
#define OTP_DATA_BOOT_FLAGS0_DISABLE_BOOTSEL_UART_BOOT_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_BOOT_FLAGS0_DISABLE_BOOTSEL_USB_PICOBOOT_IFC
#define OTP_DATA_BOOT_FLAGS0_DISABLE_BOOTSEL_USB_PICOBOOT_IFC_RESET  "-"
#define OTP_DATA_BOOT_FLAGS0_DISABLE_BOOTSEL_USB_PICOBOOT_IFC_BITS   _u(0x00040000)
#define OTP_DATA_BOOT_FLAGS0_DISABLE_BOOTSEL_USB_PICOBOOT_IFC_MSB    _u(18)
#define OTP_DATA_BOOT_FLAGS0_DISABLE_BOOTSEL_USB_PICOBOOT_IFC_LSB    _u(18)
#define OTP_DATA_BOOT_FLAGS0_DISABLE_BOOTSEL_USB_PICOBOOT_IFC_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_BOOT_FLAGS0_DISABLE_BOOTSEL_USB_MSD_IFC
#define OTP_DATA_BOOT_FLAGS0_DISABLE_BOOTSEL_USB_MSD_IFC_RESET  "-"
#define OTP_DATA_BOOT_FLAGS0_DISABLE_BOOTSEL_USB_MSD_IFC_BITS   _u(0x00020000)
#define OTP_DATA_BOOT_FLAGS0_DISABLE_BOOTSEL_USB_MSD_IFC_MSB    _u(17)
#define OTP_DATA_BOOT_FLAGS0_DISABLE_BOOTSEL_USB_MSD_IFC_LSB    _u(17)
#define OTP_DATA_BOOT_FLAGS0_DISABLE_BOOTSEL_USB_MSD_IFC_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_BOOT_FLAGS0_DISABLE_WATCHDOG_SCRATCH
#define OTP_DATA_BOOT_FLAGS0_DISABLE_WATCHDOG_SCRATCH_RESET  "-"
#define OTP_DATA_BOOT_FLAGS0_DISABLE_WATCHDOG_SCRATCH_BITS   _u(0x00010000)
#define OTP_DATA_BOOT_FLAGS0_DISABLE_WATCHDOG_SCRATCH_MSB    _u(16)
#define OTP_DATA_BOOT_FLAGS0_DISABLE_WATCHDOG_SCRATCH_LSB    _u(16)
#define OTP_DATA_BOOT_FLAGS0_DISABLE_WATCHDOG_SCRATCH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_BOOT_FLAGS0_DISABLE_POWER_SCRATCH
#define OTP_DATA_BOOT_FLAGS0_DISABLE_POWER_SCRATCH_RESET  "-"
#define OTP_DATA_BOOT_FLAGS0_DISABLE_POWER_SCRATCH_BITS   _u(0x00008000)
#define OTP_DATA_BOOT_FLAGS0_DISABLE_POWER_SCRATCH_MSB    _u(15)
#define OTP_DATA_BOOT_FLAGS0_DISABLE_POWER_SCRATCH_LSB    _u(15)
#define OTP_DATA_BOOT_FLAGS0_DISABLE_POWER_SCRATCH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_BOOT_FLAGS0_ENABLE_OTP_BOOT
// Description : Enable OTP boot. A number of OTP rows specified by OTPBOOT_LEN
//               will be loaded, starting from OTPBOOT_SRC, into the SRAM
//               location specified by OTPBOOT_DST1 and OTPBOOT_DST0.
//
//               The loaded program image is stored with ECC, 16 bits per row,
//               and must contain a valid IMAGE_DEF. Do not set this bit without
//               first programming an image into OTP and configuring
//               OTPBOOT_LEN, OTPBOOT_SRC, OTPBOOT_DST0 and OTPBOOT_DST1.
//
//               Note that OTPBOOT_LEN and OTPBOOT_SRC must be even numbers of
//               OTP rows. Equivalently, the image must be a multiple of 32 bits
//               in size, and must start at a 32-bit-aligned address in the ECC
//               read data address window.
#define OTP_DATA_BOOT_FLAGS0_ENABLE_OTP_BOOT_RESET  "-"
#define OTP_DATA_BOOT_FLAGS0_ENABLE_OTP_BOOT_BITS   _u(0x00004000)
#define OTP_DATA_BOOT_FLAGS0_ENABLE_OTP_BOOT_MSB    _u(14)
#define OTP_DATA_BOOT_FLAGS0_ENABLE_OTP_BOOT_LSB    _u(14)
#define OTP_DATA_BOOT_FLAGS0_ENABLE_OTP_BOOT_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_BOOT_FLAGS0_DISABLE_OTP_BOOT
// Description : Takes precedence over ENABLE_OTP_BOOT.
#define OTP_DATA_BOOT_FLAGS0_DISABLE_OTP_BOOT_RESET  "-"
#define OTP_DATA_BOOT_FLAGS0_DISABLE_OTP_BOOT_BITS   _u(0x00002000)
#define OTP_DATA_BOOT_FLAGS0_DISABLE_OTP_BOOT_MSB    _u(13)
#define OTP_DATA_BOOT_FLAGS0_DISABLE_OTP_BOOT_LSB    _u(13)
#define OTP_DATA_BOOT_FLAGS0_DISABLE_OTP_BOOT_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_BOOT_FLAGS0_DISABLE_FLASH_BOOT
#define OTP_DATA_BOOT_FLAGS0_DISABLE_FLASH_BOOT_RESET  "-"
#define OTP_DATA_BOOT_FLAGS0_DISABLE_FLASH_BOOT_BITS   _u(0x00001000)
#define OTP_DATA_BOOT_FLAGS0_DISABLE_FLASH_BOOT_MSB    _u(12)
#define OTP_DATA_BOOT_FLAGS0_DISABLE_FLASH_BOOT_LSB    _u(12)
#define OTP_DATA_BOOT_FLAGS0_DISABLE_FLASH_BOOT_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_BOOT_FLAGS0_ROLLBACK_REQUIRED
// Description : Require binaries to have a rollback version. Set automatically
//               the first time a binary with a rollback version is booted.
#define OTP_DATA_BOOT_FLAGS0_ROLLBACK_REQUIRED_RESET  "-"
#define OTP_DATA_BOOT_FLAGS0_ROLLBACK_REQUIRED_BITS   _u(0x00000800)
#define OTP_DATA_BOOT_FLAGS0_ROLLBACK_REQUIRED_MSB    _u(11)
#define OTP_DATA_BOOT_FLAGS0_ROLLBACK_REQUIRED_LSB    _u(11)
#define OTP_DATA_BOOT_FLAGS0_ROLLBACK_REQUIRED_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_BOOT_FLAGS0_HASHED_PARTITION_TABLE
// Description : Require a partition table to be hashed (if not signed)
#define OTP_DATA_BOOT_FLAGS0_HASHED_PARTITION_TABLE_RESET  "-"
#define OTP_DATA_BOOT_FLAGS0_HASHED_PARTITION_TABLE_BITS   _u(0x00000400)
#define OTP_DATA_BOOT_FLAGS0_HASHED_PARTITION_TABLE_MSB    _u(10)
#define OTP_DATA_BOOT_FLAGS0_HASHED_PARTITION_TABLE_LSB    _u(10)
#define OTP_DATA_BOOT_FLAGS0_HASHED_PARTITION_TABLE_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_BOOT_FLAGS0_SECURE_PARTITION_TABLE
// Description : Require a partition table to be signed
#define OTP_DATA_BOOT_FLAGS0_SECURE_PARTITION_TABLE_RESET  "-"
#define OTP_DATA_BOOT_FLAGS0_SECURE_PARTITION_TABLE_BITS   _u(0x00000200)
#define OTP_DATA_BOOT_FLAGS0_SECURE_PARTITION_TABLE_MSB    _u(9)
#define OTP_DATA_BOOT_FLAGS0_SECURE_PARTITION_TABLE_LSB    _u(9)
#define OTP_DATA_BOOT_FLAGS0_SECURE_PARTITION_TABLE_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_BOOT_FLAGS0_DISABLE_AUTO_SWITCH_ARCH
// Description : Disable auto-switch of CPU architecture on boot when the (only)
//               binary to be booted is for the other Arm/RISC-V architecture
//               and both architectures are enabled
#define OTP_DATA_BOOT_FLAGS0_DISABLE_AUTO_SWITCH_ARCH_RESET  "-"
#define OTP_DATA_BOOT_FLAGS0_DISABLE_AUTO_SWITCH_ARCH_BITS   _u(0x00000100)
#define OTP_DATA_BOOT_FLAGS0_DISABLE_AUTO_SWITCH_ARCH_MSB    _u(8)
#define OTP_DATA_BOOT_FLAGS0_DISABLE_AUTO_SWITCH_ARCH_LSB    _u(8)
#define OTP_DATA_BOOT_FLAGS0_DISABLE_AUTO_SWITCH_ARCH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_BOOT_FLAGS0_SINGLE_FLASH_BINARY
// Description : Restrict flash boot path to use of a single binary at the start
//               of flash
#define OTP_DATA_BOOT_FLAGS0_SINGLE_FLASH_BINARY_RESET  "-"
#define OTP_DATA_BOOT_FLAGS0_SINGLE_FLASH_BINARY_BITS   _u(0x00000080)
#define OTP_DATA_BOOT_FLAGS0_SINGLE_FLASH_BINARY_MSB    _u(7)
#define OTP_DATA_BOOT_FLAGS0_SINGLE_FLASH_BINARY_LSB    _u(7)
#define OTP_DATA_BOOT_FLAGS0_SINGLE_FLASH_BINARY_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_BOOT_FLAGS0_OVERRIDE_FLASH_PARTITION_SLOT_SIZE
// Description : Override the limit for default flash metadata scanning.
//
//               The value is specified in FLASH_PARTITION_SLOT_SIZE. Make sure
//               FLASH_PARTITION_SLOT_SIZE is valid before setting this bit
#define OTP_DATA_BOOT_FLAGS0_OVERRIDE_FLASH_PARTITION_SLOT_SIZE_RESET  "-"
#define OTP_DATA_BOOT_FLAGS0_OVERRIDE_FLASH_PARTITION_SLOT_SIZE_BITS   _u(0x00000040)
#define OTP_DATA_BOOT_FLAGS0_OVERRIDE_FLASH_PARTITION_SLOT_SIZE_MSB    _u(6)
#define OTP_DATA_BOOT_FLAGS0_OVERRIDE_FLASH_PARTITION_SLOT_SIZE_LSB    _u(6)
#define OTP_DATA_BOOT_FLAGS0_OVERRIDE_FLASH_PARTITION_SLOT_SIZE_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_BOOT_FLAGS0_FLASH_DEVINFO_ENABLE
// Description : Mark FLASH_DEVINFO as containing valid, ECC'd data which
//               describes external flash devices.
#define OTP_DATA_BOOT_FLAGS0_FLASH_DEVINFO_ENABLE_RESET  "-"
#define OTP_DATA_BOOT_FLAGS0_FLASH_DEVINFO_ENABLE_BITS   _u(0x00000020)
#define OTP_DATA_BOOT_FLAGS0_FLASH_DEVINFO_ENABLE_MSB    _u(5)
#define OTP_DATA_BOOT_FLAGS0_FLASH_DEVINFO_ENABLE_LSB    _u(5)
#define OTP_DATA_BOOT_FLAGS0_FLASH_DEVINFO_ENABLE_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_BOOT_FLAGS0_FAST_SIGCHECK_ROSC_DIV
// Description : Enable quartering of ROSC divisor during signature check, to
//               reduce secure boot time
#define OTP_DATA_BOOT_FLAGS0_FAST_SIGCHECK_ROSC_DIV_RESET  "-"
#define OTP_DATA_BOOT_FLAGS0_FAST_SIGCHECK_ROSC_DIV_BITS   _u(0x00000010)
#define OTP_DATA_BOOT_FLAGS0_FAST_SIGCHECK_ROSC_DIV_MSB    _u(4)
#define OTP_DATA_BOOT_FLAGS0_FAST_SIGCHECK_ROSC_DIV_LSB    _u(4)
#define OTP_DATA_BOOT_FLAGS0_FAST_SIGCHECK_ROSC_DIV_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_BOOT_FLAGS0_FLASH_IO_VOLTAGE_1V8
// Description : If 1, configure the QSPI pads for 1.8 V operation when
//               accessing flash for the first time from the bootrom, using the
//               VOLTAGE_SELECT register for the QSPI pads bank. This slightly
//               improves the input timing of the pads at low voltages, but does
//               not affect their output characteristics.
//
//               If 0, leave VOLTAGE_SELECT in its reset state (suitable for
//               operation at and above 2.5 V)
#define OTP_DATA_BOOT_FLAGS0_FLASH_IO_VOLTAGE_1V8_RESET  "-"
#define OTP_DATA_BOOT_FLAGS0_FLASH_IO_VOLTAGE_1V8_BITS   _u(0x00000008)
#define OTP_DATA_BOOT_FLAGS0_FLASH_IO_VOLTAGE_1V8_MSB    _u(3)
#define OTP_DATA_BOOT_FLAGS0_FLASH_IO_VOLTAGE_1V8_LSB    _u(3)
#define OTP_DATA_BOOT_FLAGS0_FLASH_IO_VOLTAGE_1V8_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_BOOT_FLAGS0_ENABLE_BOOTSEL_NON_DEFAULT_PLL_XOSC_CFG
// Description : Enable loading of the non-default XOSC and PLL configuration
//               before entering BOOTSEL mode.
//
//               Ensure that BOOTSEL_XOSC_CFG and BOOTSEL_PLL_CFG are correctly
//               programmed before setting this bit.
//
//               If this bit is set, user software may use the contents of
//               BOOTSEL_PLL_CFG to calculated the expected XOSC frequency based
//               on the fixed USB boot frequency of 48 MHz.
#define OTP_DATA_BOOT_FLAGS0_ENABLE_BOOTSEL_NON_DEFAULT_PLL_XOSC_CFG_RESET  "-"
#define OTP_DATA_BOOT_FLAGS0_ENABLE_BOOTSEL_NON_DEFAULT_PLL_XOSC_CFG_BITS   _u(0x00000004)
#define OTP_DATA_BOOT_FLAGS0_ENABLE_BOOTSEL_NON_DEFAULT_PLL_XOSC_CFG_MSB    _u(2)
#define OTP_DATA_BOOT_FLAGS0_ENABLE_BOOTSEL_NON_DEFAULT_PLL_XOSC_CFG_LSB    _u(2)
#define OTP_DATA_BOOT_FLAGS0_ENABLE_BOOTSEL_NON_DEFAULT_PLL_XOSC_CFG_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_BOOT_FLAGS0_ENABLE_BOOTSEL_LED
// Description : Enable bootloader activity LED. If set, bootsel_led_cfg is
//               assumed to be valid
#define OTP_DATA_BOOT_FLAGS0_ENABLE_BOOTSEL_LED_RESET  "-"
#define OTP_DATA_BOOT_FLAGS0_ENABLE_BOOTSEL_LED_BITS   _u(0x00000002)
#define OTP_DATA_BOOT_FLAGS0_ENABLE_BOOTSEL_LED_MSB    _u(1)
#define OTP_DATA_BOOT_FLAGS0_ENABLE_BOOTSEL_LED_LSB    _u(1)
#define OTP_DATA_BOOT_FLAGS0_ENABLE_BOOTSEL_LED_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_BOOT_FLAGS0_DISABLE_BOOTSEL_EXEC2
#define OTP_DATA_BOOT_FLAGS0_DISABLE_BOOTSEL_EXEC2_RESET  "-"
#define OTP_DATA_BOOT_FLAGS0_DISABLE_BOOTSEL_EXEC2_BITS   _u(0x00000001)
#define OTP_DATA_BOOT_FLAGS0_DISABLE_BOOTSEL_EXEC2_MSB    _u(0)
#define OTP_DATA_BOOT_FLAGS0_DISABLE_BOOTSEL_EXEC2_LSB    _u(0)
#define OTP_DATA_BOOT_FLAGS0_DISABLE_BOOTSEL_EXEC2_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOT_FLAGS0_R1
// Description : Redundant copy of BOOT_FLAGS0
#define OTP_DATA_BOOT_FLAGS0_R1_ROW _u(0x00000049)
#define OTP_DATA_BOOT_FLAGS0_R1_BITS   _u(0x00ffffff)
#define OTP_DATA_BOOT_FLAGS0_R1_RESET  "-"
#define OTP_DATA_BOOT_FLAGS0_R1_WIDTH  _u(24)
#define OTP_DATA_BOOT_FLAGS0_R1_MSB    _u(23)
#define OTP_DATA_BOOT_FLAGS0_R1_LSB    _u(0)
#define OTP_DATA_BOOT_FLAGS0_R1_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOT_FLAGS0_R2
// Description : Redundant copy of BOOT_FLAGS0
#define OTP_DATA_BOOT_FLAGS0_R2_ROW _u(0x0000004a)
#define OTP_DATA_BOOT_FLAGS0_R2_BITS   _u(0x00ffffff)
#define OTP_DATA_BOOT_FLAGS0_R2_RESET  "-"
#define OTP_DATA_BOOT_FLAGS0_R2_WIDTH  _u(24)
#define OTP_DATA_BOOT_FLAGS0_R2_MSB    _u(23)
#define OTP_DATA_BOOT_FLAGS0_R2_LSB    _u(0)
#define OTP_DATA_BOOT_FLAGS0_R2_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOT_FLAGS1
// Description : Disable/Enable boot paths/features in the RP2350 mask ROM.
//               Disables always supersede enables. Enables are provided where
//               there are other configurations in OTP that must be valid.
//               (RBIT-3)
#define OTP_DATA_BOOT_FLAGS1_ROW _u(0x0000004b)
#define OTP_DATA_BOOT_FLAGS1_BITS   _u(0x000f0f0f)
#define OTP_DATA_BOOT_FLAGS1_RESET  _u(0x00000000)
#define OTP_DATA_BOOT_FLAGS1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_BOOT_FLAGS1_DOUBLE_TAP
// Description : Enable entering BOOTSEL mode via double-tap of the RUN/RSTn
//               pin. Adds a significant delay to boot time, as configured by
//               DOUBLE_TAP_DELAY.
//
//               This functions by waiting at startup (i.e. following a reset)
//               to see if a second reset is applied soon afterward. The second
//               reset is detected by the bootrom with help of the
//               POWMAN_CHIP_RESET_DOUBLE_TAP flag, which is not reset by the
//               external reset pin, and the bootrom enters BOOTSEL mode
//               (NSBOOT) to await further instruction over USB or UART.
#define OTP_DATA_BOOT_FLAGS1_DOUBLE_TAP_RESET  "-"
#define OTP_DATA_BOOT_FLAGS1_DOUBLE_TAP_BITS   _u(0x00080000)
#define OTP_DATA_BOOT_FLAGS1_DOUBLE_TAP_MSB    _u(19)
#define OTP_DATA_BOOT_FLAGS1_DOUBLE_TAP_LSB    _u(19)
#define OTP_DATA_BOOT_FLAGS1_DOUBLE_TAP_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_BOOT_FLAGS1_DOUBLE_TAP_DELAY
// Description : Adjust how long to wait for a second reset when double tap
//               BOOTSEL mode is enabled via DOUBLE_TAP. The minimum is 50
//               milliseconds, and each unit of this field adds an additional 50
//               milliseconds.
//
//               For example, settings this field to its maximum value of 7 will
//               cause the chip to wait for 400 milliseconds at boot to check
//               for a second reset which requests entry to BOOTSEL mode.
//
//               200 milliseconds (DOUBLE_TAP_DELAY=3) is a good intermediate
//               value.
#define OTP_DATA_BOOT_FLAGS1_DOUBLE_TAP_DELAY_RESET  "-"
#define OTP_DATA_BOOT_FLAGS1_DOUBLE_TAP_DELAY_BITS   _u(0x00070000)
#define OTP_DATA_BOOT_FLAGS1_DOUBLE_TAP_DELAY_MSB    _u(18)
#define OTP_DATA_BOOT_FLAGS1_DOUBLE_TAP_DELAY_LSB    _u(16)
#define OTP_DATA_BOOT_FLAGS1_DOUBLE_TAP_DELAY_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_BOOT_FLAGS1_KEY_INVALID
// Description : Mark a boot key as invalid, or prevent it from ever becoming
//               valid. The bootrom will ignore any boot key marked as invalid
//               during secure boot signature checks.
//
//               Each bit in this field corresponds to one of the four 256-bit
//               boot key hashes that may be stored in page 2 of the OTP.
//
//               When provisioning boot keys, it's recommended to mark any boot
//               key slots you don't intend to use as KEY_INVALID, so that
//               spurious keys can not be installed at a later time.
#define OTP_DATA_BOOT_FLAGS1_KEY_INVALID_RESET  "-"
#define OTP_DATA_BOOT_FLAGS1_KEY_INVALID_BITS   _u(0x00000f00)
#define OTP_DATA_BOOT_FLAGS1_KEY_INVALID_MSB    _u(11)
#define OTP_DATA_BOOT_FLAGS1_KEY_INVALID_LSB    _u(8)
#define OTP_DATA_BOOT_FLAGS1_KEY_INVALID_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_BOOT_FLAGS1_KEY_VALID
// Description : Mark each of the possible boot keys as valid. The bootrom will
//               check signatures against all valid boot keys, and ignore
//               invalid boot keys.
//
//               Each bit in this field corresponds to one of the four 256-bit
//               boot key hashes that may be stored in page 2 of the OTP.
//
//               A KEY_VALID bit is ignored if the corresponding KEY_INVALID bit
//               is set. Boot keys are considered valid only when KEY_VALID is
//               set and KEY_INVALID is clear.
//
//               Do not mark a boot key as KEY_VALID if it does not contain a
//               valid SHA-256 hash of your secp256k1 public key. Verify keys
//               after programming, before setting the KEY_VALID bits -- a boot
//               key with uncorrectable ECC faults will render your device
//               unbootable if secure boot is enabled.
//
//               Do not enable secure boot without first installing a valid key.
//               This will render your device unbootable.
#define OTP_DATA_BOOT_FLAGS1_KEY_VALID_RESET  "-"
#define OTP_DATA_BOOT_FLAGS1_KEY_VALID_BITS   _u(0x0000000f)
#define OTP_DATA_BOOT_FLAGS1_KEY_VALID_MSB    _u(3)
#define OTP_DATA_BOOT_FLAGS1_KEY_VALID_LSB    _u(0)
#define OTP_DATA_BOOT_FLAGS1_KEY_VALID_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOT_FLAGS1_R1
// Description : Redundant copy of BOOT_FLAGS1
#define OTP_DATA_BOOT_FLAGS1_R1_ROW _u(0x0000004c)
#define OTP_DATA_BOOT_FLAGS1_R1_BITS   _u(0x00ffffff)
#define OTP_DATA_BOOT_FLAGS1_R1_RESET  "-"
#define OTP_DATA_BOOT_FLAGS1_R1_WIDTH  _u(24)
#define OTP_DATA_BOOT_FLAGS1_R1_MSB    _u(23)
#define OTP_DATA_BOOT_FLAGS1_R1_LSB    _u(0)
#define OTP_DATA_BOOT_FLAGS1_R1_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOT_FLAGS1_R2
// Description : Redundant copy of BOOT_FLAGS1
#define OTP_DATA_BOOT_FLAGS1_R2_ROW _u(0x0000004d)
#define OTP_DATA_BOOT_FLAGS1_R2_BITS   _u(0x00ffffff)
#define OTP_DATA_BOOT_FLAGS1_R2_RESET  "-"
#define OTP_DATA_BOOT_FLAGS1_R2_WIDTH  _u(24)
#define OTP_DATA_BOOT_FLAGS1_R2_MSB    _u(23)
#define OTP_DATA_BOOT_FLAGS1_R2_LSB    _u(0)
#define OTP_DATA_BOOT_FLAGS1_R2_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_DEFAULT_BOOT_VERSION0
// Description : Default boot version thermometer counter, bits 23:0 (RBIT-3)
#define OTP_DATA_DEFAULT_BOOT_VERSION0_ROW _u(0x0000004e)
#define OTP_DATA_DEFAULT_BOOT_VERSION0_BITS   _u(0x00ffffff)
#define OTP_DATA_DEFAULT_BOOT_VERSION0_RESET  "-"
#define OTP_DATA_DEFAULT_BOOT_VERSION0_WIDTH  _u(24)
#define OTP_DATA_DEFAULT_BOOT_VERSION0_MSB    _u(23)
#define OTP_DATA_DEFAULT_BOOT_VERSION0_LSB    _u(0)
#define OTP_DATA_DEFAULT_BOOT_VERSION0_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_DEFAULT_BOOT_VERSION0_R1
// Description : Redundant copy of DEFAULT_BOOT_VERSION0
#define OTP_DATA_DEFAULT_BOOT_VERSION0_R1_ROW _u(0x0000004f)
#define OTP_DATA_DEFAULT_BOOT_VERSION0_R1_BITS   _u(0x00ffffff)
#define OTP_DATA_DEFAULT_BOOT_VERSION0_R1_RESET  "-"
#define OTP_DATA_DEFAULT_BOOT_VERSION0_R1_WIDTH  _u(24)
#define OTP_DATA_DEFAULT_BOOT_VERSION0_R1_MSB    _u(23)
#define OTP_DATA_DEFAULT_BOOT_VERSION0_R1_LSB    _u(0)
#define OTP_DATA_DEFAULT_BOOT_VERSION0_R1_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_DEFAULT_BOOT_VERSION0_R2
// Description : Redundant copy of DEFAULT_BOOT_VERSION0
#define OTP_DATA_DEFAULT_BOOT_VERSION0_R2_ROW _u(0x00000050)
#define OTP_DATA_DEFAULT_BOOT_VERSION0_R2_BITS   _u(0x00ffffff)
#define OTP_DATA_DEFAULT_BOOT_VERSION0_R2_RESET  "-"
#define OTP_DATA_DEFAULT_BOOT_VERSION0_R2_WIDTH  _u(24)
#define OTP_DATA_DEFAULT_BOOT_VERSION0_R2_MSB    _u(23)
#define OTP_DATA_DEFAULT_BOOT_VERSION0_R2_LSB    _u(0)
#define OTP_DATA_DEFAULT_BOOT_VERSION0_R2_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_DEFAULT_BOOT_VERSION1
// Description : Default boot version thermometer counter, bits 47:24 (RBIT-3)
#define OTP_DATA_DEFAULT_BOOT_VERSION1_ROW _u(0x00000051)
#define OTP_DATA_DEFAULT_BOOT_VERSION1_BITS   _u(0x00ffffff)
#define OTP_DATA_DEFAULT_BOOT_VERSION1_RESET  "-"
#define OTP_DATA_DEFAULT_BOOT_VERSION1_WIDTH  _u(24)
#define OTP_DATA_DEFAULT_BOOT_VERSION1_MSB    _u(23)
#define OTP_DATA_DEFAULT_BOOT_VERSION1_LSB    _u(0)
#define OTP_DATA_DEFAULT_BOOT_VERSION1_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_DEFAULT_BOOT_VERSION1_R1
// Description : Redundant copy of DEFAULT_BOOT_VERSION1
#define OTP_DATA_DEFAULT_BOOT_VERSION1_R1_ROW _u(0x00000052)
#define OTP_DATA_DEFAULT_BOOT_VERSION1_R1_BITS   _u(0x00ffffff)
#define OTP_DATA_DEFAULT_BOOT_VERSION1_R1_RESET  "-"
#define OTP_DATA_DEFAULT_BOOT_VERSION1_R1_WIDTH  _u(24)
#define OTP_DATA_DEFAULT_BOOT_VERSION1_R1_MSB    _u(23)
#define OTP_DATA_DEFAULT_BOOT_VERSION1_R1_LSB    _u(0)
#define OTP_DATA_DEFAULT_BOOT_VERSION1_R1_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_DEFAULT_BOOT_VERSION1_R2
// Description : Redundant copy of DEFAULT_BOOT_VERSION1
#define OTP_DATA_DEFAULT_BOOT_VERSION1_R2_ROW _u(0x00000053)
#define OTP_DATA_DEFAULT_BOOT_VERSION1_R2_BITS   _u(0x00ffffff)
#define OTP_DATA_DEFAULT_BOOT_VERSION1_R2_RESET  "-"
#define OTP_DATA_DEFAULT_BOOT_VERSION1_R2_WIDTH  _u(24)
#define OTP_DATA_DEFAULT_BOOT_VERSION1_R2_MSB    _u(23)
#define OTP_DATA_DEFAULT_BOOT_VERSION1_R2_LSB    _u(0)
#define OTP_DATA_DEFAULT_BOOT_VERSION1_R2_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_FLASH_DEVINFO
// Description : Stores information about external flash device(s). (ECC)
//
//               Assumed to be valid if BOOT_FLAGS0_FLASH_DEVINFO_ENABLE is set.
#define OTP_DATA_FLASH_DEVINFO_ROW _u(0x00000054)
#define OTP_DATA_FLASH_DEVINFO_BITS   _u(0x0000ffbf)
#define OTP_DATA_FLASH_DEVINFO_RESET  _u(0x00000000)
#define OTP_DATA_FLASH_DEVINFO_WIDTH  _u(16)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_FLASH_DEVINFO_CS1_SIZE
// Description : The size of the flash/PSRAM device on chip select 1
//               (addressable at 0x11000000 through 0x11ffffff).
//
//               A value of zero is decoded as a size of zero (no device).
//               Nonzero values are decoded as 4kiB << CS1_SIZE. For example,
//               four megabytes is encoded with a CS1_SIZE value of 10, and 16
//               megabytes is encoded with a CS1_SIZE value of 12.
//
//               When BOOT_FLAGS0_FLASH_DEVINFO_ENABLE is not set, a default of
//               zero is used.
//               0x0 -> NONE
//               0x1 -> 8K
//               0x2 -> 16K
//               0x3 -> 32K
//               0x4 -> 64k
//               0x5 -> 128K
//               0x6 -> 256K
//               0x7 -> 512K
//               0x8 -> 1M
//               0x9 -> 2M
//               0xa -> 4M
//               0xb -> 8M
//               0xc -> 16M
#define OTP_DATA_FLASH_DEVINFO_CS1_SIZE_RESET  "-"
#define OTP_DATA_FLASH_DEVINFO_CS1_SIZE_BITS   _u(0x0000f000)
#define OTP_DATA_FLASH_DEVINFO_CS1_SIZE_MSB    _u(15)
#define OTP_DATA_FLASH_DEVINFO_CS1_SIZE_LSB    _u(12)
#define OTP_DATA_FLASH_DEVINFO_CS1_SIZE_ACCESS "RO"
#define OTP_DATA_FLASH_DEVINFO_CS1_SIZE_VALUE_NONE _u(0x0)
#define OTP_DATA_FLASH_DEVINFO_CS1_SIZE_VALUE_8K _u(0x1)
#define OTP_DATA_FLASH_DEVINFO_CS1_SIZE_VALUE_16K _u(0x2)
#define OTP_DATA_FLASH_DEVINFO_CS1_SIZE_VALUE_32K _u(0x3)
#define OTP_DATA_FLASH_DEVINFO_CS1_SIZE_VALUE_64K _u(0x4)
#define OTP_DATA_FLASH_DEVINFO_CS1_SIZE_VALUE_128K _u(0x5)
#define OTP_DATA_FLASH_DEVINFO_CS1_SIZE_VALUE_256K _u(0x6)
#define OTP_DATA_FLASH_DEVINFO_CS1_SIZE_VALUE_512K _u(0x7)
#define OTP_DATA_FLASH_DEVINFO_CS1_SIZE_VALUE_1M _u(0x8)
#define OTP_DATA_FLASH_DEVINFO_CS1_SIZE_VALUE_2M _u(0x9)
#define OTP_DATA_FLASH_DEVINFO_CS1_SIZE_VALUE_4M _u(0xa)
#define OTP_DATA_FLASH_DEVINFO_CS1_SIZE_VALUE_8M _u(0xb)
#define OTP_DATA_FLASH_DEVINFO_CS1_SIZE_VALUE_16M _u(0xc)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_FLASH_DEVINFO_CS0_SIZE
// Description : The size of the flash/PSRAM device on chip select 0
//               (addressable at 0x10000000 through 0x10ffffff).
//
//               A value of zero is decoded as a size of zero (no device).
//               Nonzero values are decoded as 4kiB << CS0_SIZE. For example,
//               four megabytes is encoded with a CS0_SIZE value of 10, and 16
//               megabytes is encoded with a CS0_SIZE value of 12.
//
//               When BOOT_FLAGS0_FLASH_DEVINFO_ENABLE is not set, a default of
//               12 (16 MiB) is used.
//               0x0 -> NONE
//               0x1 -> 8K
//               0x2 -> 16K
//               0x3 -> 32K
//               0x4 -> 64k
//               0x5 -> 128K
//               0x6 -> 256K
//               0x7 -> 512K
//               0x8 -> 1M
//               0x9 -> 2M
//               0xa -> 4M
//               0xb -> 8M
//               0xc -> 16M
#define OTP_DATA_FLASH_DEVINFO_CS0_SIZE_RESET  "-"
#define OTP_DATA_FLASH_DEVINFO_CS0_SIZE_BITS   _u(0x00000f00)
#define OTP_DATA_FLASH_DEVINFO_CS0_SIZE_MSB    _u(11)
#define OTP_DATA_FLASH_DEVINFO_CS0_SIZE_LSB    _u(8)
#define OTP_DATA_FLASH_DEVINFO_CS0_SIZE_ACCESS "RO"
#define OTP_DATA_FLASH_DEVINFO_CS0_SIZE_VALUE_NONE _u(0x0)
#define OTP_DATA_FLASH_DEVINFO_CS0_SIZE_VALUE_8K _u(0x1)
#define OTP_DATA_FLASH_DEVINFO_CS0_SIZE_VALUE_16K _u(0x2)
#define OTP_DATA_FLASH_DEVINFO_CS0_SIZE_VALUE_32K _u(0x3)
#define OTP_DATA_FLASH_DEVINFO_CS0_SIZE_VALUE_64K _u(0x4)
#define OTP_DATA_FLASH_DEVINFO_CS0_SIZE_VALUE_128K _u(0x5)
#define OTP_DATA_FLASH_DEVINFO_CS0_SIZE_VALUE_256K _u(0x6)
#define OTP_DATA_FLASH_DEVINFO_CS0_SIZE_VALUE_512K _u(0x7)
#define OTP_DATA_FLASH_DEVINFO_CS0_SIZE_VALUE_1M _u(0x8)
#define OTP_DATA_FLASH_DEVINFO_CS0_SIZE_VALUE_2M _u(0x9)
#define OTP_DATA_FLASH_DEVINFO_CS0_SIZE_VALUE_4M _u(0xa)
#define OTP_DATA_FLASH_DEVINFO_CS0_SIZE_VALUE_8M _u(0xb)
#define OTP_DATA_FLASH_DEVINFO_CS0_SIZE_VALUE_16M _u(0xc)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_FLASH_DEVINFO_D8H_ERASE_SUPPORTED
// Description : If true, all attached devices are assumed to support (or
//               ignore, in the case of PSRAM) a block erase command with a
//               command prefix of D8h, an erase size of 64 kiB, and a 24-bit
//               address. Almost all 25-series flash devices support this
//               command.
//
//               If set, the bootrom will use the D8h erase command where it is
//               able, to accelerate bulk erase operations. This makes flash
//               programming faster.
//
//               When BOOT_FLAGS0_FLASH_DEVINFO_ENABLE is not set, this field
//               defaults to false.
#define OTP_DATA_FLASH_DEVINFO_D8H_ERASE_SUPPORTED_RESET  "-"
#define OTP_DATA_FLASH_DEVINFO_D8H_ERASE_SUPPORTED_BITS   _u(0x00000080)
#define OTP_DATA_FLASH_DEVINFO_D8H_ERASE_SUPPORTED_MSB    _u(7)
#define OTP_DATA_FLASH_DEVINFO_D8H_ERASE_SUPPORTED_LSB    _u(7)
#define OTP_DATA_FLASH_DEVINFO_D8H_ERASE_SUPPORTED_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_FLASH_DEVINFO_CS1_GPIO
// Description : Indicate a GPIO number to be used for the secondary flash chip
//               select (CS1), which selects the external QSPI device mapped at
//               system addresses 0x11000000 through 0x11ffffff. There is no
//               such configuration for CS0, as the primary chip select has a
//               dedicated pin.
//
//               On RP2350 the permissible GPIO numbers are 0, 8, 19 and 47.
//
//               Ignored if CS1_size is zero. If CS1_SIZE is nonzero, the
//               bootrom will automatically configure this GPIO as a second chip
//               select upon entering the flash boot path, or entering any other
//               path that may use the QSPI flash interface, such as BOOTSEL
//               mode (nsboot).
#define OTP_DATA_FLASH_DEVINFO_CS1_GPIO_RESET  "-"
#define OTP_DATA_FLASH_DEVINFO_CS1_GPIO_BITS   _u(0x0000003f)
#define OTP_DATA_FLASH_DEVINFO_CS1_GPIO_MSB    _u(5)
#define OTP_DATA_FLASH_DEVINFO_CS1_GPIO_LSB    _u(0)
#define OTP_DATA_FLASH_DEVINFO_CS1_GPIO_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_FLASH_PARTITION_SLOT_SIZE
// Description : Gap between partition table slot 0 and slot 1 at the start of
//               flash (the default size is 4096 bytes) (ECC) Enabled by the
//               OVERRIDE_FLASH_PARTITION_SLOT_SIZE bit in BOOT_FLAGS, the size
//               is 4096 * (value + 1)
#define OTP_DATA_FLASH_PARTITION_SLOT_SIZE_ROW _u(0x00000055)
#define OTP_DATA_FLASH_PARTITION_SLOT_SIZE_BITS   _u(0x0000ffff)
#define OTP_DATA_FLASH_PARTITION_SLOT_SIZE_RESET  "-"
#define OTP_DATA_FLASH_PARTITION_SLOT_SIZE_WIDTH  _u(16)
#define OTP_DATA_FLASH_PARTITION_SLOT_SIZE_MSB    _u(15)
#define OTP_DATA_FLASH_PARTITION_SLOT_SIZE_LSB    _u(0)
#define OTP_DATA_FLASH_PARTITION_SLOT_SIZE_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTSEL_LED_CFG
// Description : Pin configuration for LED status, used by USB bootloader. (ECC)
//               Must be valid if BOOT_FLAGS0_ENABLE_BOOTSEL_LED is set.
#define OTP_DATA_BOOTSEL_LED_CFG_ROW _u(0x00000056)
#define OTP_DATA_BOOTSEL_LED_CFG_BITS   _u(0x0000013f)
#define OTP_DATA_BOOTSEL_LED_CFG_RESET  _u(0x00000000)
#define OTP_DATA_BOOTSEL_LED_CFG_WIDTH  _u(16)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_BOOTSEL_LED_CFG_ACTIVELOW
// Description : LED is active-low. (Default: active-high.)
#define OTP_DATA_BOOTSEL_LED_CFG_ACTIVELOW_RESET  "-"
#define OTP_DATA_BOOTSEL_LED_CFG_ACTIVELOW_BITS   _u(0x00000100)
#define OTP_DATA_BOOTSEL_LED_CFG_ACTIVELOW_MSB    _u(8)
#define OTP_DATA_BOOTSEL_LED_CFG_ACTIVELOW_LSB    _u(8)
#define OTP_DATA_BOOTSEL_LED_CFG_ACTIVELOW_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_BOOTSEL_LED_CFG_PIN
// Description : GPIO index to use for bootloader activity LED.
#define OTP_DATA_BOOTSEL_LED_CFG_PIN_RESET  "-"
#define OTP_DATA_BOOTSEL_LED_CFG_PIN_BITS   _u(0x0000003f)
#define OTP_DATA_BOOTSEL_LED_CFG_PIN_MSB    _u(5)
#define OTP_DATA_BOOTSEL_LED_CFG_PIN_LSB    _u(0)
#define OTP_DATA_BOOTSEL_LED_CFG_PIN_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTSEL_PLL_CFG
// Description : Optional PLL configuration for BOOTSEL mode. (ECC)
//
//               This should be configured to produce an exact 48 MHz based on
//               the crystal oscillator frequency. User mode software may also
//               use this value to calculate the expected crystal frequency
//               based on an assumed 48 MHz PLL output.
//
//               If no configuration is given, the crystal is assumed to be 12
//               MHz.
//
//               The PLL frequency can be calculated as:
//
//               PLL out = (XOSC frequency / (REFDIV+1)) x FBDIV / (POSTDIV1 x
//               POSTDIV2)
//
//               Conversely the crystal frequency can be calculated as:
//
//               XOSC frequency = 48 MHz x (REFDIV+1) x (POSTDIV1 x POSTDIV2) /
//               FBDIV
//
//               (Note the  +1 on REFDIV is because the value stored in this OTP
//               location is the actual divisor value minus one.)
//
//               Used if and only if ENABLE_BOOTSEL_NON_DEFAULT_PLL_XOSC_CFG is
//               set in BOOT_FLAGS0. That bit should be set only after this row
//               and BOOTSEL_XOSC_CFG are both correctly programmed.
#define OTP_DATA_BOOTSEL_PLL_CFG_ROW _u(0x00000057)
#define OTP_DATA_BOOTSEL_PLL_CFG_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTSEL_PLL_CFG_RESET  _u(0x00000000)
#define OTP_DATA_BOOTSEL_PLL_CFG_WIDTH  _u(16)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_BOOTSEL_PLL_CFG_REFDIV
// Description : PLL reference divisor, minus one.
//
//               Programming a value of 0 means a reference divisor of 1.
//               Programming a value of 1 means a reference divisor of 2 (for
//               exceptionally fast XIN inputs)
#define OTP_DATA_BOOTSEL_PLL_CFG_REFDIV_RESET  "-"
#define OTP_DATA_BOOTSEL_PLL_CFG_REFDIV_BITS   _u(0x00008000)
#define OTP_DATA_BOOTSEL_PLL_CFG_REFDIV_MSB    _u(15)
#define OTP_DATA_BOOTSEL_PLL_CFG_REFDIV_LSB    _u(15)
#define OTP_DATA_BOOTSEL_PLL_CFG_REFDIV_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_BOOTSEL_PLL_CFG_POSTDIV2
// Description : PLL post-divide 2 divisor, in the range 1..7 inclusive.
#define OTP_DATA_BOOTSEL_PLL_CFG_POSTDIV2_RESET  "-"
#define OTP_DATA_BOOTSEL_PLL_CFG_POSTDIV2_BITS   _u(0x00007000)
#define OTP_DATA_BOOTSEL_PLL_CFG_POSTDIV2_MSB    _u(14)
#define OTP_DATA_BOOTSEL_PLL_CFG_POSTDIV2_LSB    _u(12)
#define OTP_DATA_BOOTSEL_PLL_CFG_POSTDIV2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_BOOTSEL_PLL_CFG_POSTDIV1
// Description : PLL post-divide 1 divisor, in the range 1..7 inclusive.
#define OTP_DATA_BOOTSEL_PLL_CFG_POSTDIV1_RESET  "-"
#define OTP_DATA_BOOTSEL_PLL_CFG_POSTDIV1_BITS   _u(0x00000e00)
#define OTP_DATA_BOOTSEL_PLL_CFG_POSTDIV1_MSB    _u(11)
#define OTP_DATA_BOOTSEL_PLL_CFG_POSTDIV1_LSB    _u(9)
#define OTP_DATA_BOOTSEL_PLL_CFG_POSTDIV1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_BOOTSEL_PLL_CFG_FBDIV
// Description : PLL feedback divisor, in the range 16..320 inclusive.
#define OTP_DATA_BOOTSEL_PLL_CFG_FBDIV_RESET  "-"
#define OTP_DATA_BOOTSEL_PLL_CFG_FBDIV_BITS   _u(0x000001ff)
#define OTP_DATA_BOOTSEL_PLL_CFG_FBDIV_MSB    _u(8)
#define OTP_DATA_BOOTSEL_PLL_CFG_FBDIV_LSB    _u(0)
#define OTP_DATA_BOOTSEL_PLL_CFG_FBDIV_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTSEL_XOSC_CFG
// Description : Non-default crystal oscillator configuration for the USB
//               bootloader. (ECC)
//
//               These values may also be used by user code configuring the
//               crystal oscillator.
//
//               Used if and only if ENABLE_BOOTSEL_NON_DEFAULT_PLL_XOSC_CFG is
//               set in BOOT_FLAGS0. That bit should be set only after this row
//               and BOOTSEL_PLL_CFG are both correctly programmed.
#define OTP_DATA_BOOTSEL_XOSC_CFG_ROW _u(0x00000058)
#define OTP_DATA_BOOTSEL_XOSC_CFG_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTSEL_XOSC_CFG_RESET  _u(0x00000000)
#define OTP_DATA_BOOTSEL_XOSC_CFG_WIDTH  _u(16)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_BOOTSEL_XOSC_CFG_RANGE
// Description : Value of the XOSC_CTRL_FREQ_RANGE register.
//               0x0 -> 1_15MHZ
//               0x1 -> 10_30MHZ
//               0x2 -> 25_60MHZ
//               0x3 -> 40_100MHZ
#define OTP_DATA_BOOTSEL_XOSC_CFG_RANGE_RESET  "-"
#define OTP_DATA_BOOTSEL_XOSC_CFG_RANGE_BITS   _u(0x0000c000)
#define OTP_DATA_BOOTSEL_XOSC_CFG_RANGE_MSB    _u(15)
#define OTP_DATA_BOOTSEL_XOSC_CFG_RANGE_LSB    _u(14)
#define OTP_DATA_BOOTSEL_XOSC_CFG_RANGE_ACCESS "RO"
#define OTP_DATA_BOOTSEL_XOSC_CFG_RANGE_VALUE_1_15MHZ _u(0x0)
#define OTP_DATA_BOOTSEL_XOSC_CFG_RANGE_VALUE_10_30MHZ _u(0x1)
#define OTP_DATA_BOOTSEL_XOSC_CFG_RANGE_VALUE_25_60MHZ _u(0x2)
#define OTP_DATA_BOOTSEL_XOSC_CFG_RANGE_VALUE_40_100MHZ _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_BOOTSEL_XOSC_CFG_STARTUP
// Description : Value of the XOSC_STARTUP register
#define OTP_DATA_BOOTSEL_XOSC_CFG_STARTUP_RESET  "-"
#define OTP_DATA_BOOTSEL_XOSC_CFG_STARTUP_BITS   _u(0x00003fff)
#define OTP_DATA_BOOTSEL_XOSC_CFG_STARTUP_MSB    _u(13)
#define OTP_DATA_BOOTSEL_XOSC_CFG_STARTUP_LSB    _u(0)
#define OTP_DATA_BOOTSEL_XOSC_CFG_STARTUP_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_USB_BOOT_FLAGS
// Description : USB boot specific feature flags (RBIT-3)
#define OTP_DATA_USB_BOOT_FLAGS_ROW _u(0x00000059)
#define OTP_DATA_USB_BOOT_FLAGS_BITS   _u(0x00c0ffff)
#define OTP_DATA_USB_BOOT_FLAGS_RESET  _u(0x00000000)
#define OTP_DATA_USB_BOOT_FLAGS_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_USB_BOOT_FLAGS_DP_DM_SWAP
// Description : Swap DM/DP during USB boot, to support board layouts with
//               mirrored USB routing (deliberate or accidental).
#define OTP_DATA_USB_BOOT_FLAGS_DP_DM_SWAP_RESET  "-"
#define OTP_DATA_USB_BOOT_FLAGS_DP_DM_SWAP_BITS   _u(0x00800000)
#define OTP_DATA_USB_BOOT_FLAGS_DP_DM_SWAP_MSB    _u(23)
#define OTP_DATA_USB_BOOT_FLAGS_DP_DM_SWAP_LSB    _u(23)
#define OTP_DATA_USB_BOOT_FLAGS_DP_DM_SWAP_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_USB_BOOT_FLAGS_WHITE_LABEL_ADDR_VALID
// Description : valid flag for INFO_UF2_TXT_BOARD_ID_STRDEF entry of the
//               USB_WHITE_LABEL struct (index 15)
#define OTP_DATA_USB_BOOT_FLAGS_WHITE_LABEL_ADDR_VALID_RESET  "-"
#define OTP_DATA_USB_BOOT_FLAGS_WHITE_LABEL_ADDR_VALID_BITS   _u(0x00400000)
#define OTP_DATA_USB_BOOT_FLAGS_WHITE_LABEL_ADDR_VALID_MSB    _u(22)
#define OTP_DATA_USB_BOOT_FLAGS_WHITE_LABEL_ADDR_VALID_LSB    _u(22)
#define OTP_DATA_USB_BOOT_FLAGS_WHITE_LABEL_ADDR_VALID_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_USB_BOOT_FLAGS_WL_INFO_UF2_TXT_BOARD_ID_STRDEF_VALID
// Description : valid flag for the USB_WHITE_LABEL_ADDR field
#define OTP_DATA_USB_BOOT_FLAGS_WL_INFO_UF2_TXT_BOARD_ID_STRDEF_VALID_RESET  "-"
#define OTP_DATA_USB_BOOT_FLAGS_WL_INFO_UF2_TXT_BOARD_ID_STRDEF_VALID_BITS   _u(0x00008000)
#define OTP_DATA_USB_BOOT_FLAGS_WL_INFO_UF2_TXT_BOARD_ID_STRDEF_VALID_MSB    _u(15)
#define OTP_DATA_USB_BOOT_FLAGS_WL_INFO_UF2_TXT_BOARD_ID_STRDEF_VALID_LSB    _u(15)
#define OTP_DATA_USB_BOOT_FLAGS_WL_INFO_UF2_TXT_BOARD_ID_STRDEF_VALID_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_USB_BOOT_FLAGS_WL_INFO_UF2_TXT_MODEL_STRDEF_VALID
// Description : valid flag for INFO_UF2_TXT_MODEL_STRDEF entry of the
//               USB_WHITE_LABEL struct (index 14)
#define OTP_DATA_USB_BOOT_FLAGS_WL_INFO_UF2_TXT_MODEL_STRDEF_VALID_RESET  "-"
#define OTP_DATA_USB_BOOT_FLAGS_WL_INFO_UF2_TXT_MODEL_STRDEF_VALID_BITS   _u(0x00004000)
#define OTP_DATA_USB_BOOT_FLAGS_WL_INFO_UF2_TXT_MODEL_STRDEF_VALID_MSB    _u(14)
#define OTP_DATA_USB_BOOT_FLAGS_WL_INFO_UF2_TXT_MODEL_STRDEF_VALID_LSB    _u(14)
#define OTP_DATA_USB_BOOT_FLAGS_WL_INFO_UF2_TXT_MODEL_STRDEF_VALID_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_USB_BOOT_FLAGS_WL_INDEX_HTM_REDIRECT_NAME_STRDEF_VALID
// Description : valid flag for INDEX_HTM_REDIRECT_NAME_STRDEF entry of the
//               USB_WHITE_LABEL struct (index 13)
#define OTP_DATA_USB_BOOT_FLAGS_WL_INDEX_HTM_REDIRECT_NAME_STRDEF_VALID_RESET  "-"
#define OTP_DATA_USB_BOOT_FLAGS_WL_INDEX_HTM_REDIRECT_NAME_STRDEF_VALID_BITS   _u(0x00002000)
#define OTP_DATA_USB_BOOT_FLAGS_WL_INDEX_HTM_REDIRECT_NAME_STRDEF_VALID_MSB    _u(13)
#define OTP_DATA_USB_BOOT_FLAGS_WL_INDEX_HTM_REDIRECT_NAME_STRDEF_VALID_LSB    _u(13)
#define OTP_DATA_USB_BOOT_FLAGS_WL_INDEX_HTM_REDIRECT_NAME_STRDEF_VALID_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_USB_BOOT_FLAGS_WL_INDEX_HTM_REDIRECT_URL_STRDEF_VALID
// Description : valid flag for INDEX_HTM_REDIRECT_URL_STRDEF entry of the
//               USB_WHITE_LABEL struct (index 12)
#define OTP_DATA_USB_BOOT_FLAGS_WL_INDEX_HTM_REDIRECT_URL_STRDEF_VALID_RESET  "-"
#define OTP_DATA_USB_BOOT_FLAGS_WL_INDEX_HTM_REDIRECT_URL_STRDEF_VALID_BITS   _u(0x00001000)
#define OTP_DATA_USB_BOOT_FLAGS_WL_INDEX_HTM_REDIRECT_URL_STRDEF_VALID_MSB    _u(12)
#define OTP_DATA_USB_BOOT_FLAGS_WL_INDEX_HTM_REDIRECT_URL_STRDEF_VALID_LSB    _u(12)
#define OTP_DATA_USB_BOOT_FLAGS_WL_INDEX_HTM_REDIRECT_URL_STRDEF_VALID_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_USB_BOOT_FLAGS_WL_SCSI_INQUIRY_VERSION_STRDEF_VALID
// Description : valid flag for SCSI_INQUIRY_VERSION_STRDEF entry of the
//               USB_WHITE_LABEL struct (index 11)
#define OTP_DATA_USB_BOOT_FLAGS_WL_SCSI_INQUIRY_VERSION_STRDEF_VALID_RESET  "-"
#define OTP_DATA_USB_BOOT_FLAGS_WL_SCSI_INQUIRY_VERSION_STRDEF_VALID_BITS   _u(0x00000800)
#define OTP_DATA_USB_BOOT_FLAGS_WL_SCSI_INQUIRY_VERSION_STRDEF_VALID_MSB    _u(11)
#define OTP_DATA_USB_BOOT_FLAGS_WL_SCSI_INQUIRY_VERSION_STRDEF_VALID_LSB    _u(11)
#define OTP_DATA_USB_BOOT_FLAGS_WL_SCSI_INQUIRY_VERSION_STRDEF_VALID_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_USB_BOOT_FLAGS_WL_SCSI_INQUIRY_PRODUCT_STRDEF_VALID
// Description : valid flag for SCSI_INQUIRY_PRODUCT_STRDEF entry of the
//               USB_WHITE_LABEL struct (index 10)
#define OTP_DATA_USB_BOOT_FLAGS_WL_SCSI_INQUIRY_PRODUCT_STRDEF_VALID_RESET  "-"
#define OTP_DATA_USB_BOOT_FLAGS_WL_SCSI_INQUIRY_PRODUCT_STRDEF_VALID_BITS   _u(0x00000400)
#define OTP_DATA_USB_BOOT_FLAGS_WL_SCSI_INQUIRY_PRODUCT_STRDEF_VALID_MSB    _u(10)
#define OTP_DATA_USB_BOOT_FLAGS_WL_SCSI_INQUIRY_PRODUCT_STRDEF_VALID_LSB    _u(10)
#define OTP_DATA_USB_BOOT_FLAGS_WL_SCSI_INQUIRY_PRODUCT_STRDEF_VALID_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_USB_BOOT_FLAGS_WL_SCSI_INQUIRY_VENDOR_STRDEF_VALID
// Description : valid flag for SCSI_INQUIRY_VENDOR_STRDEF entry of the
//               USB_WHITE_LABEL struct (index 9)
#define OTP_DATA_USB_BOOT_FLAGS_WL_SCSI_INQUIRY_VENDOR_STRDEF_VALID_RESET  "-"
#define OTP_DATA_USB_BOOT_FLAGS_WL_SCSI_INQUIRY_VENDOR_STRDEF_VALID_BITS   _u(0x00000200)
#define OTP_DATA_USB_BOOT_FLAGS_WL_SCSI_INQUIRY_VENDOR_STRDEF_VALID_MSB    _u(9)
#define OTP_DATA_USB_BOOT_FLAGS_WL_SCSI_INQUIRY_VENDOR_STRDEF_VALID_LSB    _u(9)
#define OTP_DATA_USB_BOOT_FLAGS_WL_SCSI_INQUIRY_VENDOR_STRDEF_VALID_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_USB_BOOT_FLAGS_WL_VOLUME_LABEL_STRDEF_VALID
// Description : valid flag for VOLUME_LABEL_STRDEF entry of the USB_WHITE_LABEL
//               struct (index 8)
#define OTP_DATA_USB_BOOT_FLAGS_WL_VOLUME_LABEL_STRDEF_VALID_RESET  "-"
#define OTP_DATA_USB_BOOT_FLAGS_WL_VOLUME_LABEL_STRDEF_VALID_BITS   _u(0x00000100)
#define OTP_DATA_USB_BOOT_FLAGS_WL_VOLUME_LABEL_STRDEF_VALID_MSB    _u(8)
#define OTP_DATA_USB_BOOT_FLAGS_WL_VOLUME_LABEL_STRDEF_VALID_LSB    _u(8)
#define OTP_DATA_USB_BOOT_FLAGS_WL_VOLUME_LABEL_STRDEF_VALID_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_USB_BOOT_FLAGS_WL_USB_CONFIG_ATTRIBUTES_MAX_POWER_VALUES_VALID
// Description : valid flag for USB_CONFIG_ATTRIBUTES_MAX_POWER_VALUES entry of
//               the USB_WHITE_LABEL struct (index 7)
#define OTP_DATA_USB_BOOT_FLAGS_WL_USB_CONFIG_ATTRIBUTES_MAX_POWER_VALUES_VALID_RESET  "-"
#define OTP_DATA_USB_BOOT_FLAGS_WL_USB_CONFIG_ATTRIBUTES_MAX_POWER_VALUES_VALID_BITS   _u(0x00000080)
#define OTP_DATA_USB_BOOT_FLAGS_WL_USB_CONFIG_ATTRIBUTES_MAX_POWER_VALUES_VALID_MSB    _u(7)
#define OTP_DATA_USB_BOOT_FLAGS_WL_USB_CONFIG_ATTRIBUTES_MAX_POWER_VALUES_VALID_LSB    _u(7)
#define OTP_DATA_USB_BOOT_FLAGS_WL_USB_CONFIG_ATTRIBUTES_MAX_POWER_VALUES_VALID_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_USB_BOOT_FLAGS_WL_USB_DEVICE_SERIAL_NUMBER_STRDEF_VALID
// Description : valid flag for USB_DEVICE_SERIAL_NUMBER_STRDEF entry of the
//               USB_WHITE_LABEL struct (index 6)
#define OTP_DATA_USB_BOOT_FLAGS_WL_USB_DEVICE_SERIAL_NUMBER_STRDEF_VALID_RESET  "-"
#define OTP_DATA_USB_BOOT_FLAGS_WL_USB_DEVICE_SERIAL_NUMBER_STRDEF_VALID_BITS   _u(0x00000040)
#define OTP_DATA_USB_BOOT_FLAGS_WL_USB_DEVICE_SERIAL_NUMBER_STRDEF_VALID_MSB    _u(6)
#define OTP_DATA_USB_BOOT_FLAGS_WL_USB_DEVICE_SERIAL_NUMBER_STRDEF_VALID_LSB    _u(6)
#define OTP_DATA_USB_BOOT_FLAGS_WL_USB_DEVICE_SERIAL_NUMBER_STRDEF_VALID_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_USB_BOOT_FLAGS_WL_USB_DEVICE_PRODUCT_STRDEF_VALID
// Description : valid flag for USB_DEVICE_PRODUCT_STRDEF entry of the
//               USB_WHITE_LABEL struct (index 5)
#define OTP_DATA_USB_BOOT_FLAGS_WL_USB_DEVICE_PRODUCT_STRDEF_VALID_RESET  "-"
#define OTP_DATA_USB_BOOT_FLAGS_WL_USB_DEVICE_PRODUCT_STRDEF_VALID_BITS   _u(0x00000020)
#define OTP_DATA_USB_BOOT_FLAGS_WL_USB_DEVICE_PRODUCT_STRDEF_VALID_MSB    _u(5)
#define OTP_DATA_USB_BOOT_FLAGS_WL_USB_DEVICE_PRODUCT_STRDEF_VALID_LSB    _u(5)
#define OTP_DATA_USB_BOOT_FLAGS_WL_USB_DEVICE_PRODUCT_STRDEF_VALID_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_USB_BOOT_FLAGS_WL_USB_DEVICE_MANUFACTURER_STRDEF_VALID
// Description : valid flag for USB_DEVICE_MANUFACTURER_STRDEF entry of the
//               USB_WHITE_LABEL struct (index 4)
#define OTP_DATA_USB_BOOT_FLAGS_WL_USB_DEVICE_MANUFACTURER_STRDEF_VALID_RESET  "-"
#define OTP_DATA_USB_BOOT_FLAGS_WL_USB_DEVICE_MANUFACTURER_STRDEF_VALID_BITS   _u(0x00000010)
#define OTP_DATA_USB_BOOT_FLAGS_WL_USB_DEVICE_MANUFACTURER_STRDEF_VALID_MSB    _u(4)
#define OTP_DATA_USB_BOOT_FLAGS_WL_USB_DEVICE_MANUFACTURER_STRDEF_VALID_LSB    _u(4)
#define OTP_DATA_USB_BOOT_FLAGS_WL_USB_DEVICE_MANUFACTURER_STRDEF_VALID_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_USB_BOOT_FLAGS_WL_USB_DEVICE_LANG_ID_VALUE_VALID
// Description : valid flag for USB_DEVICE_LANG_ID_VALUE entry of the
//               USB_WHITE_LABEL struct (index 3)
#define OTP_DATA_USB_BOOT_FLAGS_WL_USB_DEVICE_LANG_ID_VALUE_VALID_RESET  "-"
#define OTP_DATA_USB_BOOT_FLAGS_WL_USB_DEVICE_LANG_ID_VALUE_VALID_BITS   _u(0x00000008)
#define OTP_DATA_USB_BOOT_FLAGS_WL_USB_DEVICE_LANG_ID_VALUE_VALID_MSB    _u(3)
#define OTP_DATA_USB_BOOT_FLAGS_WL_USB_DEVICE_LANG_ID_VALUE_VALID_LSB    _u(3)
#define OTP_DATA_USB_BOOT_FLAGS_WL_USB_DEVICE_LANG_ID_VALUE_VALID_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_USB_BOOT_FLAGS_WL_USB_DEVICE_SERIAL_NUMBER_VALUE_VALID
// Description : valid flag for USB_DEVICE_BCD_DEVICEVALUE entry of the
//               USB_WHITE_LABEL struct (index 2)
#define OTP_DATA_USB_BOOT_FLAGS_WL_USB_DEVICE_SERIAL_NUMBER_VALUE_VALID_RESET  "-"
#define OTP_DATA_USB_BOOT_FLAGS_WL_USB_DEVICE_SERIAL_NUMBER_VALUE_VALID_BITS   _u(0x00000004)
#define OTP_DATA_USB_BOOT_FLAGS_WL_USB_DEVICE_SERIAL_NUMBER_VALUE_VALID_MSB    _u(2)
#define OTP_DATA_USB_BOOT_FLAGS_WL_USB_DEVICE_SERIAL_NUMBER_VALUE_VALID_LSB    _u(2)
#define OTP_DATA_USB_BOOT_FLAGS_WL_USB_DEVICE_SERIAL_NUMBER_VALUE_VALID_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_USB_BOOT_FLAGS_WL_USB_DEVICE_PID_VALUE_VALID
// Description : valid flag for USB_DEVICE_PID_VALUE entry of the
//               USB_WHITE_LABEL struct (index 1)
#define OTP_DATA_USB_BOOT_FLAGS_WL_USB_DEVICE_PID_VALUE_VALID_RESET  "-"
#define OTP_DATA_USB_BOOT_FLAGS_WL_USB_DEVICE_PID_VALUE_VALID_BITS   _u(0x00000002)
#define OTP_DATA_USB_BOOT_FLAGS_WL_USB_DEVICE_PID_VALUE_VALID_MSB    _u(1)
#define OTP_DATA_USB_BOOT_FLAGS_WL_USB_DEVICE_PID_VALUE_VALID_LSB    _u(1)
#define OTP_DATA_USB_BOOT_FLAGS_WL_USB_DEVICE_PID_VALUE_VALID_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_USB_BOOT_FLAGS_WL_USB_DEVICE_VID_VALUE_VALID
// Description : valid flag for USB_DEVICE_VID_VALUE entry of the
//               USB_WHITE_LABEL struct (index 0)
#define OTP_DATA_USB_BOOT_FLAGS_WL_USB_DEVICE_VID_VALUE_VALID_RESET  "-"
#define OTP_DATA_USB_BOOT_FLAGS_WL_USB_DEVICE_VID_VALUE_VALID_BITS   _u(0x00000001)
#define OTP_DATA_USB_BOOT_FLAGS_WL_USB_DEVICE_VID_VALUE_VALID_MSB    _u(0)
#define OTP_DATA_USB_BOOT_FLAGS_WL_USB_DEVICE_VID_VALUE_VALID_LSB    _u(0)
#define OTP_DATA_USB_BOOT_FLAGS_WL_USB_DEVICE_VID_VALUE_VALID_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_USB_BOOT_FLAGS_R1
// Description : Redundant copy of USB_BOOT_FLAGS
#define OTP_DATA_USB_BOOT_FLAGS_R1_ROW _u(0x0000005a)
#define OTP_DATA_USB_BOOT_FLAGS_R1_BITS   _u(0x00ffffff)
#define OTP_DATA_USB_BOOT_FLAGS_R1_RESET  "-"
#define OTP_DATA_USB_BOOT_FLAGS_R1_WIDTH  _u(24)
#define OTP_DATA_USB_BOOT_FLAGS_R1_MSB    _u(23)
#define OTP_DATA_USB_BOOT_FLAGS_R1_LSB    _u(0)
#define OTP_DATA_USB_BOOT_FLAGS_R1_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_USB_BOOT_FLAGS_R2
// Description : Redundant copy of USB_BOOT_FLAGS
#define OTP_DATA_USB_BOOT_FLAGS_R2_ROW _u(0x0000005b)
#define OTP_DATA_USB_BOOT_FLAGS_R2_BITS   _u(0x00ffffff)
#define OTP_DATA_USB_BOOT_FLAGS_R2_RESET  "-"
#define OTP_DATA_USB_BOOT_FLAGS_R2_WIDTH  _u(24)
#define OTP_DATA_USB_BOOT_FLAGS_R2_MSB    _u(23)
#define OTP_DATA_USB_BOOT_FLAGS_R2_LSB    _u(0)
#define OTP_DATA_USB_BOOT_FLAGS_R2_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_USB_WHITE_LABEL_ADDR
// Description : Row index of the USB_WHITE_LABEL structure within OTP (ECC)
//
//               The table has 16 rows, each of which are also ECC and marked
//               valid by the corresponding valid bit in USB_BOOT_FLAGS (ECC).
//
//               The entries are either _VALUEs where the 16 bit value is used
//               as is, or _STRDEFs which acts as a pointers to a string value.
//
//               The value stored in a _STRDEF is two separate bytes: The low
//               seven bits of the first (LSB) byte indicates the number of
//               characters in the string, and the top bit of the first (LSB)
//               byte if set to indicate that each character in the string is
//               two bytes (Unicode) versus one byte if unset. The second (MSB)
//               byte represents the location of the string data, and is encoded
//               as the number of rows from this USB_WHITE_LABEL_ADDR; i.e. the
//               row of the start of the string is USB_WHITE_LABEL_ADDR value +
//               msb_byte.
//
//               In each case, the corresponding valid bit enables replacing the
//               default value for the corresponding item provided by the boot
//               rom.
//
//               Note that Unicode _STRDEFs are only supported for
//               USB_DEVICE_PRODUCT_STRDEF, USB_DEVICE_SERIAL_NUMBER_STRDEF and
//               USB_DEVICE_MANUFACTURER_STRDEF. Unicode values will be ignored
//               if specified for other fields, and non-unicode values for these
//               three items will be converted to Unicode characters by setting
//               the upper 8 bits to zero.
//
//               Note that if the USB_WHITE_LABEL structure or the corresponding
//               strings are not readable by BOOTSEL mode based on OTP
//               permissions, or if alignment requirements are not met, then the
//               corresponding default values are used.
//
//               The index values indicate where each field is located (row
//               USB_WHITE_LABEL_ADDR value + index):
//               0x0000 -> INDEX_USB_DEVICE_VID_VALUE
//               0x0001 -> INDEX_USB_DEVICE_PID_VALUE
//               0x0002 -> INDEX_USB_DEVICE_BCD_DEVICE_VALUE
//               0x0003 -> INDEX_USB_DEVICE_LANG_ID_VALUE
//               0x0004 -> INDEX_USB_DEVICE_MANUFACTURER_STRDEF
//               0x0005 -> INDEX_USB_DEVICE_PRODUCT_STRDEF
//               0x0006 -> INDEX_USB_DEVICE_SERIAL_NUMBER_STRDEF
//               0x0007 -> INDEX_USB_CONFIG_ATTRIBUTES_MAX_POWER_VALUES
//               0x0008 -> INDEX_VOLUME_LABEL_STRDEF
//               0x0009 -> INDEX_SCSI_INQUIRY_VENDOR_STRDEF
//               0x000a -> INDEX_SCSI_INQUIRY_PRODUCT_STRDEF
//               0x000b -> INDEX_SCSI_INQUIRY_VERSION_STRDEF
//               0x000c -> INDEX_INDEX_HTM_REDIRECT_URL_STRDEF
//               0x000d -> INDEX_INDEX_HTM_REDIRECT_NAME_STRDEF
//               0x000e -> INDEX_INFO_UF2_TXT_MODEL_STRDEF
//               0x000f -> INDEX_INFO_UF2_TXT_BOARD_ID_STRDEF
#define OTP_DATA_USB_WHITE_LABEL_ADDR_ROW _u(0x0000005c)
#define OTP_DATA_USB_WHITE_LABEL_ADDR_BITS   _u(0x0000ffff)
#define OTP_DATA_USB_WHITE_LABEL_ADDR_RESET  "-"
#define OTP_DATA_USB_WHITE_LABEL_ADDR_WIDTH  _u(16)
#define OTP_DATA_USB_WHITE_LABEL_ADDR_MSB    _u(15)
#define OTP_DATA_USB_WHITE_LABEL_ADDR_LSB    _u(0)
#define OTP_DATA_USB_WHITE_LABEL_ADDR_ACCESS "RO"
#define OTP_DATA_USB_WHITE_LABEL_ADDR_VALUE_INDEX_USB_DEVICE_VID_VALUE _u(0x0000)
#define OTP_DATA_USB_WHITE_LABEL_ADDR_VALUE_INDEX_USB_DEVICE_PID_VALUE _u(0x0001)
#define OTP_DATA_USB_WHITE_LABEL_ADDR_VALUE_INDEX_USB_DEVICE_BCD_DEVICE_VALUE _u(0x0002)
#define OTP_DATA_USB_WHITE_LABEL_ADDR_VALUE_INDEX_USB_DEVICE_LANG_ID_VALUE _u(0x0003)
#define OTP_DATA_USB_WHITE_LABEL_ADDR_VALUE_INDEX_USB_DEVICE_MANUFACTURER_STRDEF _u(0x0004)
#define OTP_DATA_USB_WHITE_LABEL_ADDR_VALUE_INDEX_USB_DEVICE_PRODUCT_STRDEF _u(0x0005)
#define OTP_DATA_USB_WHITE_LABEL_ADDR_VALUE_INDEX_USB_DEVICE_SERIAL_NUMBER_STRDEF _u(0x0006)
#define OTP_DATA_USB_WHITE_LABEL_ADDR_VALUE_INDEX_USB_CONFIG_ATTRIBUTES_MAX_POWER_VALUES _u(0x0007)
#define OTP_DATA_USB_WHITE_LABEL_ADDR_VALUE_INDEX_VOLUME_LABEL_STRDEF _u(0x0008)
#define OTP_DATA_USB_WHITE_LABEL_ADDR_VALUE_INDEX_SCSI_INQUIRY_VENDOR_STRDEF _u(0x0009)
#define OTP_DATA_USB_WHITE_LABEL_ADDR_VALUE_INDEX_SCSI_INQUIRY_PRODUCT_STRDEF _u(0x000a)
#define OTP_DATA_USB_WHITE_LABEL_ADDR_VALUE_INDEX_SCSI_INQUIRY_VERSION_STRDEF _u(0x000b)
#define OTP_DATA_USB_WHITE_LABEL_ADDR_VALUE_INDEX_INDEX_HTM_REDIRECT_URL_STRDEF _u(0x000c)
#define OTP_DATA_USB_WHITE_LABEL_ADDR_VALUE_INDEX_INDEX_HTM_REDIRECT_NAME_STRDEF _u(0x000d)
#define OTP_DATA_USB_WHITE_LABEL_ADDR_VALUE_INDEX_INFO_UF2_TXT_MODEL_STRDEF _u(0x000e)
#define OTP_DATA_USB_WHITE_LABEL_ADDR_VALUE_INDEX_INFO_UF2_TXT_BOARD_ID_STRDEF _u(0x000f)
// =============================================================================
// Register    : OTP_DATA_OTPBOOT_SRC
// Description : OTP start row for the OTP boot image. (ECC)
//
//               If OTP boot is enabled, the bootrom will load from this
//               location into SRAM and then directly enter the loaded image.
//               Note that the image must be signed if SECURE_BOOT_ENABLE is
//               set. The image itself is assumed to be ECC-protected.
//
//               This must be an even number. Equivalently, the OTP boot image
//               must start at a word-aligned location in the ECC read data
//               address window.
#define OTP_DATA_OTPBOOT_SRC_ROW _u(0x0000005e)
#define OTP_DATA_OTPBOOT_SRC_BITS   _u(0x0000ffff)
#define OTP_DATA_OTPBOOT_SRC_RESET  "-"
#define OTP_DATA_OTPBOOT_SRC_WIDTH  _u(16)
#define OTP_DATA_OTPBOOT_SRC_MSB    _u(15)
#define OTP_DATA_OTPBOOT_SRC_LSB    _u(0)
#define OTP_DATA_OTPBOOT_SRC_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_OTPBOOT_LEN
// Description : Length in rows of the OTP boot image. (ECC)
//
//               OTPBOOT_LEN must be even. The total image size must be a
//               multiple of 4 bytes (32 bits).
#define OTP_DATA_OTPBOOT_LEN_ROW _u(0x0000005f)
#define OTP_DATA_OTPBOOT_LEN_BITS   _u(0x0000ffff)
#define OTP_DATA_OTPBOOT_LEN_RESET  "-"
#define OTP_DATA_OTPBOOT_LEN_WIDTH  _u(16)
#define OTP_DATA_OTPBOOT_LEN_MSB    _u(15)
#define OTP_DATA_OTPBOOT_LEN_LSB    _u(0)
#define OTP_DATA_OTPBOOT_LEN_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_OTPBOOT_DST0
// Description : Bits 15:0 of the OTP boot image load destination (and entry
//               point). (ECC)
//
//               This must be a location in main SRAM (main SRAM is addresses
//               0x20000000 through 0x20082000) and must be word-aligned.
#define OTP_DATA_OTPBOOT_DST0_ROW _u(0x00000060)
#define OTP_DATA_OTPBOOT_DST0_BITS   _u(0x0000ffff)
#define OTP_DATA_OTPBOOT_DST0_RESET  "-"
#define OTP_DATA_OTPBOOT_DST0_WIDTH  _u(16)
#define OTP_DATA_OTPBOOT_DST0_MSB    _u(15)
#define OTP_DATA_OTPBOOT_DST0_LSB    _u(0)
#define OTP_DATA_OTPBOOT_DST0_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_OTPBOOT_DST1
// Description : Bits 31:16 of the OTP boot image load destination (and entry
//               point). (ECC)
//
//               This must be a location in main SRAM (main SRAM is addresses
//               0x20000000 through 0x20082000) and must be word-aligned.
#define OTP_DATA_OTPBOOT_DST1_ROW _u(0x00000061)
#define OTP_DATA_OTPBOOT_DST1_BITS   _u(0x0000ffff)
#define OTP_DATA_OTPBOOT_DST1_RESET  "-"
#define OTP_DATA_OTPBOOT_DST1_WIDTH  _u(16)
#define OTP_DATA_OTPBOOT_DST1_MSB    _u(15)
#define OTP_DATA_OTPBOOT_DST1_LSB    _u(0)
#define OTP_DATA_OTPBOOT_DST1_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY0_0
// Description : Bits 15:0 of SHA-256 hash of boot key 0 (ECC)
#define OTP_DATA_BOOTKEY0_0_ROW _u(0x00000080)
#define OTP_DATA_BOOTKEY0_0_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY0_0_RESET  "-"
#define OTP_DATA_BOOTKEY0_0_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY0_0_MSB    _u(15)
#define OTP_DATA_BOOTKEY0_0_LSB    _u(0)
#define OTP_DATA_BOOTKEY0_0_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY0_1
// Description : Bits 31:16 of SHA-256 hash of boot key 0 (ECC)
#define OTP_DATA_BOOTKEY0_1_ROW _u(0x00000081)
#define OTP_DATA_BOOTKEY0_1_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY0_1_RESET  "-"
#define OTP_DATA_BOOTKEY0_1_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY0_1_MSB    _u(15)
#define OTP_DATA_BOOTKEY0_1_LSB    _u(0)
#define OTP_DATA_BOOTKEY0_1_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY0_2
// Description : Bits 47:32 of SHA-256 hash of boot key 0 (ECC)
#define OTP_DATA_BOOTKEY0_2_ROW _u(0x00000082)
#define OTP_DATA_BOOTKEY0_2_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY0_2_RESET  "-"
#define OTP_DATA_BOOTKEY0_2_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY0_2_MSB    _u(15)
#define OTP_DATA_BOOTKEY0_2_LSB    _u(0)
#define OTP_DATA_BOOTKEY0_2_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY0_3
// Description : Bits 63:48 of SHA-256 hash of boot key 0 (ECC)
#define OTP_DATA_BOOTKEY0_3_ROW _u(0x00000083)
#define OTP_DATA_BOOTKEY0_3_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY0_3_RESET  "-"
#define OTP_DATA_BOOTKEY0_3_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY0_3_MSB    _u(15)
#define OTP_DATA_BOOTKEY0_3_LSB    _u(0)
#define OTP_DATA_BOOTKEY0_3_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY0_4
// Description : Bits 79:64 of SHA-256 hash of boot key 0 (ECC)
#define OTP_DATA_BOOTKEY0_4_ROW _u(0x00000084)
#define OTP_DATA_BOOTKEY0_4_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY0_4_RESET  "-"
#define OTP_DATA_BOOTKEY0_4_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY0_4_MSB    _u(15)
#define OTP_DATA_BOOTKEY0_4_LSB    _u(0)
#define OTP_DATA_BOOTKEY0_4_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY0_5
// Description : Bits 95:80 of SHA-256 hash of boot key 0 (ECC)
#define OTP_DATA_BOOTKEY0_5_ROW _u(0x00000085)
#define OTP_DATA_BOOTKEY0_5_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY0_5_RESET  "-"
#define OTP_DATA_BOOTKEY0_5_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY0_5_MSB    _u(15)
#define OTP_DATA_BOOTKEY0_5_LSB    _u(0)
#define OTP_DATA_BOOTKEY0_5_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY0_6
// Description : Bits 111:96 of SHA-256 hash of boot key 0 (ECC)
#define OTP_DATA_BOOTKEY0_6_ROW _u(0x00000086)
#define OTP_DATA_BOOTKEY0_6_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY0_6_RESET  "-"
#define OTP_DATA_BOOTKEY0_6_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY0_6_MSB    _u(15)
#define OTP_DATA_BOOTKEY0_6_LSB    _u(0)
#define OTP_DATA_BOOTKEY0_6_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY0_7
// Description : Bits 127:112 of SHA-256 hash of boot key 0 (ECC)
#define OTP_DATA_BOOTKEY0_7_ROW _u(0x00000087)
#define OTP_DATA_BOOTKEY0_7_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY0_7_RESET  "-"
#define OTP_DATA_BOOTKEY0_7_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY0_7_MSB    _u(15)
#define OTP_DATA_BOOTKEY0_7_LSB    _u(0)
#define OTP_DATA_BOOTKEY0_7_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY0_8
// Description : Bits 143:128 of SHA-256 hash of boot key 0 (ECC)
#define OTP_DATA_BOOTKEY0_8_ROW _u(0x00000088)
#define OTP_DATA_BOOTKEY0_8_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY0_8_RESET  "-"
#define OTP_DATA_BOOTKEY0_8_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY0_8_MSB    _u(15)
#define OTP_DATA_BOOTKEY0_8_LSB    _u(0)
#define OTP_DATA_BOOTKEY0_8_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY0_9
// Description : Bits 159:144 of SHA-256 hash of boot key 0 (ECC)
#define OTP_DATA_BOOTKEY0_9_ROW _u(0x00000089)
#define OTP_DATA_BOOTKEY0_9_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY0_9_RESET  "-"
#define OTP_DATA_BOOTKEY0_9_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY0_9_MSB    _u(15)
#define OTP_DATA_BOOTKEY0_9_LSB    _u(0)
#define OTP_DATA_BOOTKEY0_9_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY0_10
// Description : Bits 175:160 of SHA-256 hash of boot key 0 (ECC)
#define OTP_DATA_BOOTKEY0_10_ROW _u(0x0000008a)
#define OTP_DATA_BOOTKEY0_10_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY0_10_RESET  "-"
#define OTP_DATA_BOOTKEY0_10_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY0_10_MSB    _u(15)
#define OTP_DATA_BOOTKEY0_10_LSB    _u(0)
#define OTP_DATA_BOOTKEY0_10_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY0_11
// Description : Bits 191:176 of SHA-256 hash of boot key 0 (ECC)
#define OTP_DATA_BOOTKEY0_11_ROW _u(0x0000008b)
#define OTP_DATA_BOOTKEY0_11_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY0_11_RESET  "-"
#define OTP_DATA_BOOTKEY0_11_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY0_11_MSB    _u(15)
#define OTP_DATA_BOOTKEY0_11_LSB    _u(0)
#define OTP_DATA_BOOTKEY0_11_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY0_12
// Description : Bits 207:192 of SHA-256 hash of boot key 0 (ECC)
#define OTP_DATA_BOOTKEY0_12_ROW _u(0x0000008c)
#define OTP_DATA_BOOTKEY0_12_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY0_12_RESET  "-"
#define OTP_DATA_BOOTKEY0_12_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY0_12_MSB    _u(15)
#define OTP_DATA_BOOTKEY0_12_LSB    _u(0)
#define OTP_DATA_BOOTKEY0_12_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY0_13
// Description : Bits 223:208 of SHA-256 hash of boot key 0 (ECC)
#define OTP_DATA_BOOTKEY0_13_ROW _u(0x0000008d)
#define OTP_DATA_BOOTKEY0_13_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY0_13_RESET  "-"
#define OTP_DATA_BOOTKEY0_13_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY0_13_MSB    _u(15)
#define OTP_DATA_BOOTKEY0_13_LSB    _u(0)
#define OTP_DATA_BOOTKEY0_13_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY0_14
// Description : Bits 239:224 of SHA-256 hash of boot key 0 (ECC)
#define OTP_DATA_BOOTKEY0_14_ROW _u(0x0000008e)
#define OTP_DATA_BOOTKEY0_14_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY0_14_RESET  "-"
#define OTP_DATA_BOOTKEY0_14_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY0_14_MSB    _u(15)
#define OTP_DATA_BOOTKEY0_14_LSB    _u(0)
#define OTP_DATA_BOOTKEY0_14_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY0_15
// Description : Bits 255:240 of SHA-256 hash of boot key 0 (ECC)
#define OTP_DATA_BOOTKEY0_15_ROW _u(0x0000008f)
#define OTP_DATA_BOOTKEY0_15_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY0_15_RESET  "-"
#define OTP_DATA_BOOTKEY0_15_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY0_15_MSB    _u(15)
#define OTP_DATA_BOOTKEY0_15_LSB    _u(0)
#define OTP_DATA_BOOTKEY0_15_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY1_0
// Description : Bits 15:0 of SHA-256 hash of boot key 1 (ECC)
#define OTP_DATA_BOOTKEY1_0_ROW _u(0x00000090)
#define OTP_DATA_BOOTKEY1_0_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY1_0_RESET  "-"
#define OTP_DATA_BOOTKEY1_0_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY1_0_MSB    _u(15)
#define OTP_DATA_BOOTKEY1_0_LSB    _u(0)
#define OTP_DATA_BOOTKEY1_0_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY1_1
// Description : Bits 31:16 of SHA-256 hash of boot key 1 (ECC)
#define OTP_DATA_BOOTKEY1_1_ROW _u(0x00000091)
#define OTP_DATA_BOOTKEY1_1_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY1_1_RESET  "-"
#define OTP_DATA_BOOTKEY1_1_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY1_1_MSB    _u(15)
#define OTP_DATA_BOOTKEY1_1_LSB    _u(0)
#define OTP_DATA_BOOTKEY1_1_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY1_2
// Description : Bits 47:32 of SHA-256 hash of boot key 1 (ECC)
#define OTP_DATA_BOOTKEY1_2_ROW _u(0x00000092)
#define OTP_DATA_BOOTKEY1_2_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY1_2_RESET  "-"
#define OTP_DATA_BOOTKEY1_2_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY1_2_MSB    _u(15)
#define OTP_DATA_BOOTKEY1_2_LSB    _u(0)
#define OTP_DATA_BOOTKEY1_2_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY1_3
// Description : Bits 63:48 of SHA-256 hash of boot key 1 (ECC)
#define OTP_DATA_BOOTKEY1_3_ROW _u(0x00000093)
#define OTP_DATA_BOOTKEY1_3_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY1_3_RESET  "-"
#define OTP_DATA_BOOTKEY1_3_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY1_3_MSB    _u(15)
#define OTP_DATA_BOOTKEY1_3_LSB    _u(0)
#define OTP_DATA_BOOTKEY1_3_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY1_4
// Description : Bits 79:64 of SHA-256 hash of boot key 1 (ECC)
#define OTP_DATA_BOOTKEY1_4_ROW _u(0x00000094)
#define OTP_DATA_BOOTKEY1_4_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY1_4_RESET  "-"
#define OTP_DATA_BOOTKEY1_4_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY1_4_MSB    _u(15)
#define OTP_DATA_BOOTKEY1_4_LSB    _u(0)
#define OTP_DATA_BOOTKEY1_4_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY1_5
// Description : Bits 95:80 of SHA-256 hash of boot key 1 (ECC)
#define OTP_DATA_BOOTKEY1_5_ROW _u(0x00000095)
#define OTP_DATA_BOOTKEY1_5_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY1_5_RESET  "-"
#define OTP_DATA_BOOTKEY1_5_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY1_5_MSB    _u(15)
#define OTP_DATA_BOOTKEY1_5_LSB    _u(0)
#define OTP_DATA_BOOTKEY1_5_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY1_6
// Description : Bits 111:96 of SHA-256 hash of boot key 1 (ECC)
#define OTP_DATA_BOOTKEY1_6_ROW _u(0x00000096)
#define OTP_DATA_BOOTKEY1_6_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY1_6_RESET  "-"
#define OTP_DATA_BOOTKEY1_6_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY1_6_MSB    _u(15)
#define OTP_DATA_BOOTKEY1_6_LSB    _u(0)
#define OTP_DATA_BOOTKEY1_6_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY1_7
// Description : Bits 127:112 of SHA-256 hash of boot key 1 (ECC)
#define OTP_DATA_BOOTKEY1_7_ROW _u(0x00000097)
#define OTP_DATA_BOOTKEY1_7_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY1_7_RESET  "-"
#define OTP_DATA_BOOTKEY1_7_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY1_7_MSB    _u(15)
#define OTP_DATA_BOOTKEY1_7_LSB    _u(0)
#define OTP_DATA_BOOTKEY1_7_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY1_8
// Description : Bits 143:128 of SHA-256 hash of boot key 1 (ECC)
#define OTP_DATA_BOOTKEY1_8_ROW _u(0x00000098)
#define OTP_DATA_BOOTKEY1_8_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY1_8_RESET  "-"
#define OTP_DATA_BOOTKEY1_8_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY1_8_MSB    _u(15)
#define OTP_DATA_BOOTKEY1_8_LSB    _u(0)
#define OTP_DATA_BOOTKEY1_8_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY1_9
// Description : Bits 159:144 of SHA-256 hash of boot key 1 (ECC)
#define OTP_DATA_BOOTKEY1_9_ROW _u(0x00000099)
#define OTP_DATA_BOOTKEY1_9_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY1_9_RESET  "-"
#define OTP_DATA_BOOTKEY1_9_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY1_9_MSB    _u(15)
#define OTP_DATA_BOOTKEY1_9_LSB    _u(0)
#define OTP_DATA_BOOTKEY1_9_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY1_10
// Description : Bits 175:160 of SHA-256 hash of boot key 1 (ECC)
#define OTP_DATA_BOOTKEY1_10_ROW _u(0x0000009a)
#define OTP_DATA_BOOTKEY1_10_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY1_10_RESET  "-"
#define OTP_DATA_BOOTKEY1_10_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY1_10_MSB    _u(15)
#define OTP_DATA_BOOTKEY1_10_LSB    _u(0)
#define OTP_DATA_BOOTKEY1_10_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY1_11
// Description : Bits 191:176 of SHA-256 hash of boot key 1 (ECC)
#define OTP_DATA_BOOTKEY1_11_ROW _u(0x0000009b)
#define OTP_DATA_BOOTKEY1_11_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY1_11_RESET  "-"
#define OTP_DATA_BOOTKEY1_11_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY1_11_MSB    _u(15)
#define OTP_DATA_BOOTKEY1_11_LSB    _u(0)
#define OTP_DATA_BOOTKEY1_11_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY1_12
// Description : Bits 207:192 of SHA-256 hash of boot key 1 (ECC)
#define OTP_DATA_BOOTKEY1_12_ROW _u(0x0000009c)
#define OTP_DATA_BOOTKEY1_12_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY1_12_RESET  "-"
#define OTP_DATA_BOOTKEY1_12_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY1_12_MSB    _u(15)
#define OTP_DATA_BOOTKEY1_12_LSB    _u(0)
#define OTP_DATA_BOOTKEY1_12_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY1_13
// Description : Bits 223:208 of SHA-256 hash of boot key 1 (ECC)
#define OTP_DATA_BOOTKEY1_13_ROW _u(0x0000009d)
#define OTP_DATA_BOOTKEY1_13_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY1_13_RESET  "-"
#define OTP_DATA_BOOTKEY1_13_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY1_13_MSB    _u(15)
#define OTP_DATA_BOOTKEY1_13_LSB    _u(0)
#define OTP_DATA_BOOTKEY1_13_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY1_14
// Description : Bits 239:224 of SHA-256 hash of boot key 1 (ECC)
#define OTP_DATA_BOOTKEY1_14_ROW _u(0x0000009e)
#define OTP_DATA_BOOTKEY1_14_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY1_14_RESET  "-"
#define OTP_DATA_BOOTKEY1_14_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY1_14_MSB    _u(15)
#define OTP_DATA_BOOTKEY1_14_LSB    _u(0)
#define OTP_DATA_BOOTKEY1_14_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY1_15
// Description : Bits 255:240 of SHA-256 hash of boot key 1 (ECC)
#define OTP_DATA_BOOTKEY1_15_ROW _u(0x0000009f)
#define OTP_DATA_BOOTKEY1_15_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY1_15_RESET  "-"
#define OTP_DATA_BOOTKEY1_15_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY1_15_MSB    _u(15)
#define OTP_DATA_BOOTKEY1_15_LSB    _u(0)
#define OTP_DATA_BOOTKEY1_15_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY2_0
// Description : Bits 15:0 of SHA-256 hash of boot key 2 (ECC)
#define OTP_DATA_BOOTKEY2_0_ROW _u(0x000000a0)
#define OTP_DATA_BOOTKEY2_0_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY2_0_RESET  "-"
#define OTP_DATA_BOOTKEY2_0_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY2_0_MSB    _u(15)
#define OTP_DATA_BOOTKEY2_0_LSB    _u(0)
#define OTP_DATA_BOOTKEY2_0_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY2_1
// Description : Bits 31:16 of SHA-256 hash of boot key 2 (ECC)
#define OTP_DATA_BOOTKEY2_1_ROW _u(0x000000a1)
#define OTP_DATA_BOOTKEY2_1_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY2_1_RESET  "-"
#define OTP_DATA_BOOTKEY2_1_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY2_1_MSB    _u(15)
#define OTP_DATA_BOOTKEY2_1_LSB    _u(0)
#define OTP_DATA_BOOTKEY2_1_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY2_2
// Description : Bits 47:32 of SHA-256 hash of boot key 2 (ECC)
#define OTP_DATA_BOOTKEY2_2_ROW _u(0x000000a2)
#define OTP_DATA_BOOTKEY2_2_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY2_2_RESET  "-"
#define OTP_DATA_BOOTKEY2_2_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY2_2_MSB    _u(15)
#define OTP_DATA_BOOTKEY2_2_LSB    _u(0)
#define OTP_DATA_BOOTKEY2_2_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY2_3
// Description : Bits 63:48 of SHA-256 hash of boot key 2 (ECC)
#define OTP_DATA_BOOTKEY2_3_ROW _u(0x000000a3)
#define OTP_DATA_BOOTKEY2_3_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY2_3_RESET  "-"
#define OTP_DATA_BOOTKEY2_3_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY2_3_MSB    _u(15)
#define OTP_DATA_BOOTKEY2_3_LSB    _u(0)
#define OTP_DATA_BOOTKEY2_3_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY2_4
// Description : Bits 79:64 of SHA-256 hash of boot key 2 (ECC)
#define OTP_DATA_BOOTKEY2_4_ROW _u(0x000000a4)
#define OTP_DATA_BOOTKEY2_4_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY2_4_RESET  "-"
#define OTP_DATA_BOOTKEY2_4_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY2_4_MSB    _u(15)
#define OTP_DATA_BOOTKEY2_4_LSB    _u(0)
#define OTP_DATA_BOOTKEY2_4_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY2_5
// Description : Bits 95:80 of SHA-256 hash of boot key 2 (ECC)
#define OTP_DATA_BOOTKEY2_5_ROW _u(0x000000a5)
#define OTP_DATA_BOOTKEY2_5_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY2_5_RESET  "-"
#define OTP_DATA_BOOTKEY2_5_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY2_5_MSB    _u(15)
#define OTP_DATA_BOOTKEY2_5_LSB    _u(0)
#define OTP_DATA_BOOTKEY2_5_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY2_6
// Description : Bits 111:96 of SHA-256 hash of boot key 2 (ECC)
#define OTP_DATA_BOOTKEY2_6_ROW _u(0x000000a6)
#define OTP_DATA_BOOTKEY2_6_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY2_6_RESET  "-"
#define OTP_DATA_BOOTKEY2_6_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY2_6_MSB    _u(15)
#define OTP_DATA_BOOTKEY2_6_LSB    _u(0)
#define OTP_DATA_BOOTKEY2_6_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY2_7
// Description : Bits 127:112 of SHA-256 hash of boot key 2 (ECC)
#define OTP_DATA_BOOTKEY2_7_ROW _u(0x000000a7)
#define OTP_DATA_BOOTKEY2_7_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY2_7_RESET  "-"
#define OTP_DATA_BOOTKEY2_7_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY2_7_MSB    _u(15)
#define OTP_DATA_BOOTKEY2_7_LSB    _u(0)
#define OTP_DATA_BOOTKEY2_7_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY2_8
// Description : Bits 143:128 of SHA-256 hash of boot key 2 (ECC)
#define OTP_DATA_BOOTKEY2_8_ROW _u(0x000000a8)
#define OTP_DATA_BOOTKEY2_8_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY2_8_RESET  "-"
#define OTP_DATA_BOOTKEY2_8_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY2_8_MSB    _u(15)
#define OTP_DATA_BOOTKEY2_8_LSB    _u(0)
#define OTP_DATA_BOOTKEY2_8_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY2_9
// Description : Bits 159:144 of SHA-256 hash of boot key 2 (ECC)
#define OTP_DATA_BOOTKEY2_9_ROW _u(0x000000a9)
#define OTP_DATA_BOOTKEY2_9_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY2_9_RESET  "-"
#define OTP_DATA_BOOTKEY2_9_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY2_9_MSB    _u(15)
#define OTP_DATA_BOOTKEY2_9_LSB    _u(0)
#define OTP_DATA_BOOTKEY2_9_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY2_10
// Description : Bits 175:160 of SHA-256 hash of boot key 2 (ECC)
#define OTP_DATA_BOOTKEY2_10_ROW _u(0x000000aa)
#define OTP_DATA_BOOTKEY2_10_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY2_10_RESET  "-"
#define OTP_DATA_BOOTKEY2_10_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY2_10_MSB    _u(15)
#define OTP_DATA_BOOTKEY2_10_LSB    _u(0)
#define OTP_DATA_BOOTKEY2_10_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY2_11
// Description : Bits 191:176 of SHA-256 hash of boot key 2 (ECC)
#define OTP_DATA_BOOTKEY2_11_ROW _u(0x000000ab)
#define OTP_DATA_BOOTKEY2_11_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY2_11_RESET  "-"
#define OTP_DATA_BOOTKEY2_11_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY2_11_MSB    _u(15)
#define OTP_DATA_BOOTKEY2_11_LSB    _u(0)
#define OTP_DATA_BOOTKEY2_11_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY2_12
// Description : Bits 207:192 of SHA-256 hash of boot key 2 (ECC)
#define OTP_DATA_BOOTKEY2_12_ROW _u(0x000000ac)
#define OTP_DATA_BOOTKEY2_12_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY2_12_RESET  "-"
#define OTP_DATA_BOOTKEY2_12_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY2_12_MSB    _u(15)
#define OTP_DATA_BOOTKEY2_12_LSB    _u(0)
#define OTP_DATA_BOOTKEY2_12_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY2_13
// Description : Bits 223:208 of SHA-256 hash of boot key 2 (ECC)
#define OTP_DATA_BOOTKEY2_13_ROW _u(0x000000ad)
#define OTP_DATA_BOOTKEY2_13_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY2_13_RESET  "-"
#define OTP_DATA_BOOTKEY2_13_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY2_13_MSB    _u(15)
#define OTP_DATA_BOOTKEY2_13_LSB    _u(0)
#define OTP_DATA_BOOTKEY2_13_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY2_14
// Description : Bits 239:224 of SHA-256 hash of boot key 2 (ECC)
#define OTP_DATA_BOOTKEY2_14_ROW _u(0x000000ae)
#define OTP_DATA_BOOTKEY2_14_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY2_14_RESET  "-"
#define OTP_DATA_BOOTKEY2_14_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY2_14_MSB    _u(15)
#define OTP_DATA_BOOTKEY2_14_LSB    _u(0)
#define OTP_DATA_BOOTKEY2_14_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY2_15
// Description : Bits 255:240 of SHA-256 hash of boot key 2 (ECC)
#define OTP_DATA_BOOTKEY2_15_ROW _u(0x000000af)
#define OTP_DATA_BOOTKEY2_15_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY2_15_RESET  "-"
#define OTP_DATA_BOOTKEY2_15_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY2_15_MSB    _u(15)
#define OTP_DATA_BOOTKEY2_15_LSB    _u(0)
#define OTP_DATA_BOOTKEY2_15_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY3_0
// Description : Bits 15:0 of SHA-256 hash of boot key 3 (ECC)
#define OTP_DATA_BOOTKEY3_0_ROW _u(0x000000b0)
#define OTP_DATA_BOOTKEY3_0_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY3_0_RESET  "-"
#define OTP_DATA_BOOTKEY3_0_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY3_0_MSB    _u(15)
#define OTP_DATA_BOOTKEY3_0_LSB    _u(0)
#define OTP_DATA_BOOTKEY3_0_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY3_1
// Description : Bits 31:16 of SHA-256 hash of boot key 3 (ECC)
#define OTP_DATA_BOOTKEY3_1_ROW _u(0x000000b1)
#define OTP_DATA_BOOTKEY3_1_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY3_1_RESET  "-"
#define OTP_DATA_BOOTKEY3_1_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY3_1_MSB    _u(15)
#define OTP_DATA_BOOTKEY3_1_LSB    _u(0)
#define OTP_DATA_BOOTKEY3_1_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY3_2
// Description : Bits 47:32 of SHA-256 hash of boot key 3 (ECC)
#define OTP_DATA_BOOTKEY3_2_ROW _u(0x000000b2)
#define OTP_DATA_BOOTKEY3_2_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY3_2_RESET  "-"
#define OTP_DATA_BOOTKEY3_2_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY3_2_MSB    _u(15)
#define OTP_DATA_BOOTKEY3_2_LSB    _u(0)
#define OTP_DATA_BOOTKEY3_2_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY3_3
// Description : Bits 63:48 of SHA-256 hash of boot key 3 (ECC)
#define OTP_DATA_BOOTKEY3_3_ROW _u(0x000000b3)
#define OTP_DATA_BOOTKEY3_3_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY3_3_RESET  "-"
#define OTP_DATA_BOOTKEY3_3_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY3_3_MSB    _u(15)
#define OTP_DATA_BOOTKEY3_3_LSB    _u(0)
#define OTP_DATA_BOOTKEY3_3_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY3_4
// Description : Bits 79:64 of SHA-256 hash of boot key 3 (ECC)
#define OTP_DATA_BOOTKEY3_4_ROW _u(0x000000b4)
#define OTP_DATA_BOOTKEY3_4_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY3_4_RESET  "-"
#define OTP_DATA_BOOTKEY3_4_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY3_4_MSB    _u(15)
#define OTP_DATA_BOOTKEY3_4_LSB    _u(0)
#define OTP_DATA_BOOTKEY3_4_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY3_5
// Description : Bits 95:80 of SHA-256 hash of boot key 3 (ECC)
#define OTP_DATA_BOOTKEY3_5_ROW _u(0x000000b5)
#define OTP_DATA_BOOTKEY3_5_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY3_5_RESET  "-"
#define OTP_DATA_BOOTKEY3_5_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY3_5_MSB    _u(15)
#define OTP_DATA_BOOTKEY3_5_LSB    _u(0)
#define OTP_DATA_BOOTKEY3_5_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY3_6
// Description : Bits 111:96 of SHA-256 hash of boot key 3 (ECC)
#define OTP_DATA_BOOTKEY3_6_ROW _u(0x000000b6)
#define OTP_DATA_BOOTKEY3_6_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY3_6_RESET  "-"
#define OTP_DATA_BOOTKEY3_6_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY3_6_MSB    _u(15)
#define OTP_DATA_BOOTKEY3_6_LSB    _u(0)
#define OTP_DATA_BOOTKEY3_6_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY3_7
// Description : Bits 127:112 of SHA-256 hash of boot key 3 (ECC)
#define OTP_DATA_BOOTKEY3_7_ROW _u(0x000000b7)
#define OTP_DATA_BOOTKEY3_7_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY3_7_RESET  "-"
#define OTP_DATA_BOOTKEY3_7_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY3_7_MSB    _u(15)
#define OTP_DATA_BOOTKEY3_7_LSB    _u(0)
#define OTP_DATA_BOOTKEY3_7_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY3_8
// Description : Bits 143:128 of SHA-256 hash of boot key 3 (ECC)
#define OTP_DATA_BOOTKEY3_8_ROW _u(0x000000b8)
#define OTP_DATA_BOOTKEY3_8_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY3_8_RESET  "-"
#define OTP_DATA_BOOTKEY3_8_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY3_8_MSB    _u(15)
#define OTP_DATA_BOOTKEY3_8_LSB    _u(0)
#define OTP_DATA_BOOTKEY3_8_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY3_9
// Description : Bits 159:144 of SHA-256 hash of boot key 3 (ECC)
#define OTP_DATA_BOOTKEY3_9_ROW _u(0x000000b9)
#define OTP_DATA_BOOTKEY3_9_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY3_9_RESET  "-"
#define OTP_DATA_BOOTKEY3_9_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY3_9_MSB    _u(15)
#define OTP_DATA_BOOTKEY3_9_LSB    _u(0)
#define OTP_DATA_BOOTKEY3_9_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY3_10
// Description : Bits 175:160 of SHA-256 hash of boot key 3 (ECC)
#define OTP_DATA_BOOTKEY3_10_ROW _u(0x000000ba)
#define OTP_DATA_BOOTKEY3_10_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY3_10_RESET  "-"
#define OTP_DATA_BOOTKEY3_10_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY3_10_MSB    _u(15)
#define OTP_DATA_BOOTKEY3_10_LSB    _u(0)
#define OTP_DATA_BOOTKEY3_10_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY3_11
// Description : Bits 191:176 of SHA-256 hash of boot key 3 (ECC)
#define OTP_DATA_BOOTKEY3_11_ROW _u(0x000000bb)
#define OTP_DATA_BOOTKEY3_11_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY3_11_RESET  "-"
#define OTP_DATA_BOOTKEY3_11_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY3_11_MSB    _u(15)
#define OTP_DATA_BOOTKEY3_11_LSB    _u(0)
#define OTP_DATA_BOOTKEY3_11_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY3_12
// Description : Bits 207:192 of SHA-256 hash of boot key 3 (ECC)
#define OTP_DATA_BOOTKEY3_12_ROW _u(0x000000bc)
#define OTP_DATA_BOOTKEY3_12_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY3_12_RESET  "-"
#define OTP_DATA_BOOTKEY3_12_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY3_12_MSB    _u(15)
#define OTP_DATA_BOOTKEY3_12_LSB    _u(0)
#define OTP_DATA_BOOTKEY3_12_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY3_13
// Description : Bits 223:208 of SHA-256 hash of boot key 3 (ECC)
#define OTP_DATA_BOOTKEY3_13_ROW _u(0x000000bd)
#define OTP_DATA_BOOTKEY3_13_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY3_13_RESET  "-"
#define OTP_DATA_BOOTKEY3_13_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY3_13_MSB    _u(15)
#define OTP_DATA_BOOTKEY3_13_LSB    _u(0)
#define OTP_DATA_BOOTKEY3_13_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY3_14
// Description : Bits 239:224 of SHA-256 hash of boot key 3 (ECC)
#define OTP_DATA_BOOTKEY3_14_ROW _u(0x000000be)
#define OTP_DATA_BOOTKEY3_14_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY3_14_RESET  "-"
#define OTP_DATA_BOOTKEY3_14_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY3_14_MSB    _u(15)
#define OTP_DATA_BOOTKEY3_14_LSB    _u(0)
#define OTP_DATA_BOOTKEY3_14_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_BOOTKEY3_15
// Description : Bits 255:240 of SHA-256 hash of boot key 3 (ECC)
#define OTP_DATA_BOOTKEY3_15_ROW _u(0x000000bf)
#define OTP_DATA_BOOTKEY3_15_BITS   _u(0x0000ffff)
#define OTP_DATA_BOOTKEY3_15_RESET  "-"
#define OTP_DATA_BOOTKEY3_15_WIDTH  _u(16)
#define OTP_DATA_BOOTKEY3_15_MSB    _u(15)
#define OTP_DATA_BOOTKEY3_15_LSB    _u(0)
#define OTP_DATA_BOOTKEY3_15_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_KEY1_0
// Description : Bits 15:0 of OTP access key 1 (ECC)
#define OTP_DATA_KEY1_0_ROW _u(0x00000f48)
#define OTP_DATA_KEY1_0_BITS   _u(0x0000ffff)
#define OTP_DATA_KEY1_0_RESET  "-"
#define OTP_DATA_KEY1_0_WIDTH  _u(16)
#define OTP_DATA_KEY1_0_MSB    _u(15)
#define OTP_DATA_KEY1_0_LSB    _u(0)
#define OTP_DATA_KEY1_0_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_KEY1_1
// Description : Bits 31:16 of OTP access key 1 (ECC)
#define OTP_DATA_KEY1_1_ROW _u(0x00000f49)
#define OTP_DATA_KEY1_1_BITS   _u(0x0000ffff)
#define OTP_DATA_KEY1_1_RESET  "-"
#define OTP_DATA_KEY1_1_WIDTH  _u(16)
#define OTP_DATA_KEY1_1_MSB    _u(15)
#define OTP_DATA_KEY1_1_LSB    _u(0)
#define OTP_DATA_KEY1_1_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_KEY1_2
// Description : Bits 47:32 of OTP access key 1 (ECC)
#define OTP_DATA_KEY1_2_ROW _u(0x00000f4a)
#define OTP_DATA_KEY1_2_BITS   _u(0x0000ffff)
#define OTP_DATA_KEY1_2_RESET  "-"
#define OTP_DATA_KEY1_2_WIDTH  _u(16)
#define OTP_DATA_KEY1_2_MSB    _u(15)
#define OTP_DATA_KEY1_2_LSB    _u(0)
#define OTP_DATA_KEY1_2_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_KEY1_3
// Description : Bits 63:48 of OTP access key 1 (ECC)
#define OTP_DATA_KEY1_3_ROW _u(0x00000f4b)
#define OTP_DATA_KEY1_3_BITS   _u(0x0000ffff)
#define OTP_DATA_KEY1_3_RESET  "-"
#define OTP_DATA_KEY1_3_WIDTH  _u(16)
#define OTP_DATA_KEY1_3_MSB    _u(15)
#define OTP_DATA_KEY1_3_LSB    _u(0)
#define OTP_DATA_KEY1_3_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_KEY1_4
// Description : Bits 79:64 of OTP access key 1 (ECC)
#define OTP_DATA_KEY1_4_ROW _u(0x00000f4c)
#define OTP_DATA_KEY1_4_BITS   _u(0x0000ffff)
#define OTP_DATA_KEY1_4_RESET  "-"
#define OTP_DATA_KEY1_4_WIDTH  _u(16)
#define OTP_DATA_KEY1_4_MSB    _u(15)
#define OTP_DATA_KEY1_4_LSB    _u(0)
#define OTP_DATA_KEY1_4_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_KEY1_5
// Description : Bits 95:80 of OTP access key 1 (ECC)
#define OTP_DATA_KEY1_5_ROW _u(0x00000f4d)
#define OTP_DATA_KEY1_5_BITS   _u(0x0000ffff)
#define OTP_DATA_KEY1_5_RESET  "-"
#define OTP_DATA_KEY1_5_WIDTH  _u(16)
#define OTP_DATA_KEY1_5_MSB    _u(15)
#define OTP_DATA_KEY1_5_LSB    _u(0)
#define OTP_DATA_KEY1_5_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_KEY1_6
// Description : Bits 111:96 of OTP access key 1 (ECC)
#define OTP_DATA_KEY1_6_ROW _u(0x00000f4e)
#define OTP_DATA_KEY1_6_BITS   _u(0x0000ffff)
#define OTP_DATA_KEY1_6_RESET  "-"
#define OTP_DATA_KEY1_6_WIDTH  _u(16)
#define OTP_DATA_KEY1_6_MSB    _u(15)
#define OTP_DATA_KEY1_6_LSB    _u(0)
#define OTP_DATA_KEY1_6_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_KEY1_7
// Description : Bits 127:112 of OTP access key 1 (ECC)
#define OTP_DATA_KEY1_7_ROW _u(0x00000f4f)
#define OTP_DATA_KEY1_7_BITS   _u(0x0000ffff)
#define OTP_DATA_KEY1_7_RESET  "-"
#define OTP_DATA_KEY1_7_WIDTH  _u(16)
#define OTP_DATA_KEY1_7_MSB    _u(15)
#define OTP_DATA_KEY1_7_LSB    _u(0)
#define OTP_DATA_KEY1_7_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_KEY2_0
// Description : Bits 15:0 of OTP access key 2 (ECC)
#define OTP_DATA_KEY2_0_ROW _u(0x00000f50)
#define OTP_DATA_KEY2_0_BITS   _u(0x0000ffff)
#define OTP_DATA_KEY2_0_RESET  "-"
#define OTP_DATA_KEY2_0_WIDTH  _u(16)
#define OTP_DATA_KEY2_0_MSB    _u(15)
#define OTP_DATA_KEY2_0_LSB    _u(0)
#define OTP_DATA_KEY2_0_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_KEY2_1
// Description : Bits 31:16 of OTP access key 2 (ECC)
#define OTP_DATA_KEY2_1_ROW _u(0x00000f51)
#define OTP_DATA_KEY2_1_BITS   _u(0x0000ffff)
#define OTP_DATA_KEY2_1_RESET  "-"
#define OTP_DATA_KEY2_1_WIDTH  _u(16)
#define OTP_DATA_KEY2_1_MSB    _u(15)
#define OTP_DATA_KEY2_1_LSB    _u(0)
#define OTP_DATA_KEY2_1_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_KEY2_2
// Description : Bits 47:32 of OTP access key 2 (ECC)
#define OTP_DATA_KEY2_2_ROW _u(0x00000f52)
#define OTP_DATA_KEY2_2_BITS   _u(0x0000ffff)
#define OTP_DATA_KEY2_2_RESET  "-"
#define OTP_DATA_KEY2_2_WIDTH  _u(16)
#define OTP_DATA_KEY2_2_MSB    _u(15)
#define OTP_DATA_KEY2_2_LSB    _u(0)
#define OTP_DATA_KEY2_2_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_KEY2_3
// Description : Bits 63:48 of OTP access key 2 (ECC)
#define OTP_DATA_KEY2_3_ROW _u(0x00000f53)
#define OTP_DATA_KEY2_3_BITS   _u(0x0000ffff)
#define OTP_DATA_KEY2_3_RESET  "-"
#define OTP_DATA_KEY2_3_WIDTH  _u(16)
#define OTP_DATA_KEY2_3_MSB    _u(15)
#define OTP_DATA_KEY2_3_LSB    _u(0)
#define OTP_DATA_KEY2_3_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_KEY2_4
// Description : Bits 79:64 of OTP access key 2 (ECC)
#define OTP_DATA_KEY2_4_ROW _u(0x00000f54)
#define OTP_DATA_KEY2_4_BITS   _u(0x0000ffff)
#define OTP_DATA_KEY2_4_RESET  "-"
#define OTP_DATA_KEY2_4_WIDTH  _u(16)
#define OTP_DATA_KEY2_4_MSB    _u(15)
#define OTP_DATA_KEY2_4_LSB    _u(0)
#define OTP_DATA_KEY2_4_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_KEY2_5
// Description : Bits 95:80 of OTP access key 2 (ECC)
#define OTP_DATA_KEY2_5_ROW _u(0x00000f55)
#define OTP_DATA_KEY2_5_BITS   _u(0x0000ffff)
#define OTP_DATA_KEY2_5_RESET  "-"
#define OTP_DATA_KEY2_5_WIDTH  _u(16)
#define OTP_DATA_KEY2_5_MSB    _u(15)
#define OTP_DATA_KEY2_5_LSB    _u(0)
#define OTP_DATA_KEY2_5_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_KEY2_6
// Description : Bits 111:96 of OTP access key 2 (ECC)
#define OTP_DATA_KEY2_6_ROW _u(0x00000f56)
#define OTP_DATA_KEY2_6_BITS   _u(0x0000ffff)
#define OTP_DATA_KEY2_6_RESET  "-"
#define OTP_DATA_KEY2_6_WIDTH  _u(16)
#define OTP_DATA_KEY2_6_MSB    _u(15)
#define OTP_DATA_KEY2_6_LSB    _u(0)
#define OTP_DATA_KEY2_6_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_KEY2_7
// Description : Bits 127:112 of OTP access key 2 (ECC)
#define OTP_DATA_KEY2_7_ROW _u(0x00000f57)
#define OTP_DATA_KEY2_7_BITS   _u(0x0000ffff)
#define OTP_DATA_KEY2_7_RESET  "-"
#define OTP_DATA_KEY2_7_WIDTH  _u(16)
#define OTP_DATA_KEY2_7_MSB    _u(15)
#define OTP_DATA_KEY2_7_LSB    _u(0)
#define OTP_DATA_KEY2_7_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_KEY3_0
// Description : Bits 15:0 of OTP access key 3 (ECC)
#define OTP_DATA_KEY3_0_ROW _u(0x00000f58)
#define OTP_DATA_KEY3_0_BITS   _u(0x0000ffff)
#define OTP_DATA_KEY3_0_RESET  "-"
#define OTP_DATA_KEY3_0_WIDTH  _u(16)
#define OTP_DATA_KEY3_0_MSB    _u(15)
#define OTP_DATA_KEY3_0_LSB    _u(0)
#define OTP_DATA_KEY3_0_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_KEY3_1
// Description : Bits 31:16 of OTP access key 3 (ECC)
#define OTP_DATA_KEY3_1_ROW _u(0x00000f59)
#define OTP_DATA_KEY3_1_BITS   _u(0x0000ffff)
#define OTP_DATA_KEY3_1_RESET  "-"
#define OTP_DATA_KEY3_1_WIDTH  _u(16)
#define OTP_DATA_KEY3_1_MSB    _u(15)
#define OTP_DATA_KEY3_1_LSB    _u(0)
#define OTP_DATA_KEY3_1_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_KEY3_2
// Description : Bits 47:32 of OTP access key 3 (ECC)
#define OTP_DATA_KEY3_2_ROW _u(0x00000f5a)
#define OTP_DATA_KEY3_2_BITS   _u(0x0000ffff)
#define OTP_DATA_KEY3_2_RESET  "-"
#define OTP_DATA_KEY3_2_WIDTH  _u(16)
#define OTP_DATA_KEY3_2_MSB    _u(15)
#define OTP_DATA_KEY3_2_LSB    _u(0)
#define OTP_DATA_KEY3_2_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_KEY3_3
// Description : Bits 63:48 of OTP access key 3 (ECC)
#define OTP_DATA_KEY3_3_ROW _u(0x00000f5b)
#define OTP_DATA_KEY3_3_BITS   _u(0x0000ffff)
#define OTP_DATA_KEY3_3_RESET  "-"
#define OTP_DATA_KEY3_3_WIDTH  _u(16)
#define OTP_DATA_KEY3_3_MSB    _u(15)
#define OTP_DATA_KEY3_3_LSB    _u(0)
#define OTP_DATA_KEY3_3_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_KEY3_4
// Description : Bits 79:64 of OTP access key 3 (ECC)
#define OTP_DATA_KEY3_4_ROW _u(0x00000f5c)
#define OTP_DATA_KEY3_4_BITS   _u(0x0000ffff)
#define OTP_DATA_KEY3_4_RESET  "-"
#define OTP_DATA_KEY3_4_WIDTH  _u(16)
#define OTP_DATA_KEY3_4_MSB    _u(15)
#define OTP_DATA_KEY3_4_LSB    _u(0)
#define OTP_DATA_KEY3_4_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_KEY3_5
// Description : Bits 95:80 of OTP access key 3 (ECC)
#define OTP_DATA_KEY3_5_ROW _u(0x00000f5d)
#define OTP_DATA_KEY3_5_BITS   _u(0x0000ffff)
#define OTP_DATA_KEY3_5_RESET  "-"
#define OTP_DATA_KEY3_5_WIDTH  _u(16)
#define OTP_DATA_KEY3_5_MSB    _u(15)
#define OTP_DATA_KEY3_5_LSB    _u(0)
#define OTP_DATA_KEY3_5_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_KEY3_6
// Description : Bits 111:96 of OTP access key 3 (ECC)
#define OTP_DATA_KEY3_6_ROW _u(0x00000f5e)
#define OTP_DATA_KEY3_6_BITS   _u(0x0000ffff)
#define OTP_DATA_KEY3_6_RESET  "-"
#define OTP_DATA_KEY3_6_WIDTH  _u(16)
#define OTP_DATA_KEY3_6_MSB    _u(15)
#define OTP_DATA_KEY3_6_LSB    _u(0)
#define OTP_DATA_KEY3_6_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_KEY3_7
// Description : Bits 127:112 of OTP access key 3 (ECC)
#define OTP_DATA_KEY3_7_ROW _u(0x00000f5f)
#define OTP_DATA_KEY3_7_BITS   _u(0x0000ffff)
#define OTP_DATA_KEY3_7_RESET  "-"
#define OTP_DATA_KEY3_7_WIDTH  _u(16)
#define OTP_DATA_KEY3_7_MSB    _u(15)
#define OTP_DATA_KEY3_7_LSB    _u(0)
#define OTP_DATA_KEY3_7_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_KEY4_0
// Description : Bits 15:0 of OTP access key 4 (ECC)
#define OTP_DATA_KEY4_0_ROW _u(0x00000f60)
#define OTP_DATA_KEY4_0_BITS   _u(0x0000ffff)
#define OTP_DATA_KEY4_0_RESET  "-"
#define OTP_DATA_KEY4_0_WIDTH  _u(16)
#define OTP_DATA_KEY4_0_MSB    _u(15)
#define OTP_DATA_KEY4_0_LSB    _u(0)
#define OTP_DATA_KEY4_0_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_KEY4_1
// Description : Bits 31:16 of OTP access key 4 (ECC)
#define OTP_DATA_KEY4_1_ROW _u(0x00000f61)
#define OTP_DATA_KEY4_1_BITS   _u(0x0000ffff)
#define OTP_DATA_KEY4_1_RESET  "-"
#define OTP_DATA_KEY4_1_WIDTH  _u(16)
#define OTP_DATA_KEY4_1_MSB    _u(15)
#define OTP_DATA_KEY4_1_LSB    _u(0)
#define OTP_DATA_KEY4_1_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_KEY4_2
// Description : Bits 47:32 of OTP access key 4 (ECC)
#define OTP_DATA_KEY4_2_ROW _u(0x00000f62)
#define OTP_DATA_KEY4_2_BITS   _u(0x0000ffff)
#define OTP_DATA_KEY4_2_RESET  "-"
#define OTP_DATA_KEY4_2_WIDTH  _u(16)
#define OTP_DATA_KEY4_2_MSB    _u(15)
#define OTP_DATA_KEY4_2_LSB    _u(0)
#define OTP_DATA_KEY4_2_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_KEY4_3
// Description : Bits 63:48 of OTP access key 4 (ECC)
#define OTP_DATA_KEY4_3_ROW _u(0x00000f63)
#define OTP_DATA_KEY4_3_BITS   _u(0x0000ffff)
#define OTP_DATA_KEY4_3_RESET  "-"
#define OTP_DATA_KEY4_3_WIDTH  _u(16)
#define OTP_DATA_KEY4_3_MSB    _u(15)
#define OTP_DATA_KEY4_3_LSB    _u(0)
#define OTP_DATA_KEY4_3_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_KEY4_4
// Description : Bits 79:64 of OTP access key 4 (ECC)
#define OTP_DATA_KEY4_4_ROW _u(0x00000f64)
#define OTP_DATA_KEY4_4_BITS   _u(0x0000ffff)
#define OTP_DATA_KEY4_4_RESET  "-"
#define OTP_DATA_KEY4_4_WIDTH  _u(16)
#define OTP_DATA_KEY4_4_MSB    _u(15)
#define OTP_DATA_KEY4_4_LSB    _u(0)
#define OTP_DATA_KEY4_4_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_KEY4_5
// Description : Bits 95:80 of OTP access key 4 (ECC)
#define OTP_DATA_KEY4_5_ROW _u(0x00000f65)
#define OTP_DATA_KEY4_5_BITS   _u(0x0000ffff)
#define OTP_DATA_KEY4_5_RESET  "-"
#define OTP_DATA_KEY4_5_WIDTH  _u(16)
#define OTP_DATA_KEY4_5_MSB    _u(15)
#define OTP_DATA_KEY4_5_LSB    _u(0)
#define OTP_DATA_KEY4_5_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_KEY4_6
// Description : Bits 111:96 of OTP access key 4 (ECC)
#define OTP_DATA_KEY4_6_ROW _u(0x00000f66)
#define OTP_DATA_KEY4_6_BITS   _u(0x0000ffff)
#define OTP_DATA_KEY4_6_RESET  "-"
#define OTP_DATA_KEY4_6_WIDTH  _u(16)
#define OTP_DATA_KEY4_6_MSB    _u(15)
#define OTP_DATA_KEY4_6_LSB    _u(0)
#define OTP_DATA_KEY4_6_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_KEY4_7
// Description : Bits 127:112 of OTP access key 4 (ECC)
#define OTP_DATA_KEY4_7_ROW _u(0x00000f67)
#define OTP_DATA_KEY4_7_BITS   _u(0x0000ffff)
#define OTP_DATA_KEY4_7_RESET  "-"
#define OTP_DATA_KEY4_7_WIDTH  _u(16)
#define OTP_DATA_KEY4_7_MSB    _u(15)
#define OTP_DATA_KEY4_7_LSB    _u(0)
#define OTP_DATA_KEY4_7_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_KEY5_0
// Description : Bits 15:0 of OTP access key 5 (ECC)
#define OTP_DATA_KEY5_0_ROW _u(0x00000f68)
#define OTP_DATA_KEY5_0_BITS   _u(0x0000ffff)
#define OTP_DATA_KEY5_0_RESET  "-"
#define OTP_DATA_KEY5_0_WIDTH  _u(16)
#define OTP_DATA_KEY5_0_MSB    _u(15)
#define OTP_DATA_KEY5_0_LSB    _u(0)
#define OTP_DATA_KEY5_0_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_KEY5_1
// Description : Bits 31:16 of OTP access key 5 (ECC)
#define OTP_DATA_KEY5_1_ROW _u(0x00000f69)
#define OTP_DATA_KEY5_1_BITS   _u(0x0000ffff)
#define OTP_DATA_KEY5_1_RESET  "-"
#define OTP_DATA_KEY5_1_WIDTH  _u(16)
#define OTP_DATA_KEY5_1_MSB    _u(15)
#define OTP_DATA_KEY5_1_LSB    _u(0)
#define OTP_DATA_KEY5_1_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_KEY5_2
// Description : Bits 47:32 of OTP access key 5 (ECC)
#define OTP_DATA_KEY5_2_ROW _u(0x00000f6a)
#define OTP_DATA_KEY5_2_BITS   _u(0x0000ffff)
#define OTP_DATA_KEY5_2_RESET  "-"
#define OTP_DATA_KEY5_2_WIDTH  _u(16)
#define OTP_DATA_KEY5_2_MSB    _u(15)
#define OTP_DATA_KEY5_2_LSB    _u(0)
#define OTP_DATA_KEY5_2_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_KEY5_3
// Description : Bits 63:48 of OTP access key 5 (ECC)
#define OTP_DATA_KEY5_3_ROW _u(0x00000f6b)
#define OTP_DATA_KEY5_3_BITS   _u(0x0000ffff)
#define OTP_DATA_KEY5_3_RESET  "-"
#define OTP_DATA_KEY5_3_WIDTH  _u(16)
#define OTP_DATA_KEY5_3_MSB    _u(15)
#define OTP_DATA_KEY5_3_LSB    _u(0)
#define OTP_DATA_KEY5_3_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_KEY5_4
// Description : Bits 79:64 of OTP access key 5 (ECC)
#define OTP_DATA_KEY5_4_ROW _u(0x00000f6c)
#define OTP_DATA_KEY5_4_BITS   _u(0x0000ffff)
#define OTP_DATA_KEY5_4_RESET  "-"
#define OTP_DATA_KEY5_4_WIDTH  _u(16)
#define OTP_DATA_KEY5_4_MSB    _u(15)
#define OTP_DATA_KEY5_4_LSB    _u(0)
#define OTP_DATA_KEY5_4_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_KEY5_5
// Description : Bits 95:80 of OTP access key 5 (ECC)
#define OTP_DATA_KEY5_5_ROW _u(0x00000f6d)
#define OTP_DATA_KEY5_5_BITS   _u(0x0000ffff)
#define OTP_DATA_KEY5_5_RESET  "-"
#define OTP_DATA_KEY5_5_WIDTH  _u(16)
#define OTP_DATA_KEY5_5_MSB    _u(15)
#define OTP_DATA_KEY5_5_LSB    _u(0)
#define OTP_DATA_KEY5_5_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_KEY5_6
// Description : Bits 111:96 of OTP access key 5 (ECC)
#define OTP_DATA_KEY5_6_ROW _u(0x00000f6e)
#define OTP_DATA_KEY5_6_BITS   _u(0x0000ffff)
#define OTP_DATA_KEY5_6_RESET  "-"
#define OTP_DATA_KEY5_6_WIDTH  _u(16)
#define OTP_DATA_KEY5_6_MSB    _u(15)
#define OTP_DATA_KEY5_6_LSB    _u(0)
#define OTP_DATA_KEY5_6_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_KEY5_7
// Description : Bits 127:112 of OTP access key 5 (ECC)
#define OTP_DATA_KEY5_7_ROW _u(0x00000f6f)
#define OTP_DATA_KEY5_7_BITS   _u(0x0000ffff)
#define OTP_DATA_KEY5_7_RESET  "-"
#define OTP_DATA_KEY5_7_WIDTH  _u(16)
#define OTP_DATA_KEY5_7_MSB    _u(15)
#define OTP_DATA_KEY5_7_LSB    _u(0)
#define OTP_DATA_KEY5_7_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_KEY6_0
// Description : Bits 15:0 of OTP access key 6 (ECC)
#define OTP_DATA_KEY6_0_ROW _u(0x00000f70)
#define OTP_DATA_KEY6_0_BITS   _u(0x0000ffff)
#define OTP_DATA_KEY6_0_RESET  "-"
#define OTP_DATA_KEY6_0_WIDTH  _u(16)
#define OTP_DATA_KEY6_0_MSB    _u(15)
#define OTP_DATA_KEY6_0_LSB    _u(0)
#define OTP_DATA_KEY6_0_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_KEY6_1
// Description : Bits 31:16 of OTP access key 6 (ECC)
#define OTP_DATA_KEY6_1_ROW _u(0x00000f71)
#define OTP_DATA_KEY6_1_BITS   _u(0x0000ffff)
#define OTP_DATA_KEY6_1_RESET  "-"
#define OTP_DATA_KEY6_1_WIDTH  _u(16)
#define OTP_DATA_KEY6_1_MSB    _u(15)
#define OTP_DATA_KEY6_1_LSB    _u(0)
#define OTP_DATA_KEY6_1_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_KEY6_2
// Description : Bits 47:32 of OTP access key 6 (ECC)
#define OTP_DATA_KEY6_2_ROW _u(0x00000f72)
#define OTP_DATA_KEY6_2_BITS   _u(0x0000ffff)
#define OTP_DATA_KEY6_2_RESET  "-"
#define OTP_DATA_KEY6_2_WIDTH  _u(16)
#define OTP_DATA_KEY6_2_MSB    _u(15)
#define OTP_DATA_KEY6_2_LSB    _u(0)
#define OTP_DATA_KEY6_2_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_KEY6_3
// Description : Bits 63:48 of OTP access key 6 (ECC)
#define OTP_DATA_KEY6_3_ROW _u(0x00000f73)
#define OTP_DATA_KEY6_3_BITS   _u(0x0000ffff)
#define OTP_DATA_KEY6_3_RESET  "-"
#define OTP_DATA_KEY6_3_WIDTH  _u(16)
#define OTP_DATA_KEY6_3_MSB    _u(15)
#define OTP_DATA_KEY6_3_LSB    _u(0)
#define OTP_DATA_KEY6_3_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_KEY6_4
// Description : Bits 79:64 of OTP access key 6 (ECC)
#define OTP_DATA_KEY6_4_ROW _u(0x00000f74)
#define OTP_DATA_KEY6_4_BITS   _u(0x0000ffff)
#define OTP_DATA_KEY6_4_RESET  "-"
#define OTP_DATA_KEY6_4_WIDTH  _u(16)
#define OTP_DATA_KEY6_4_MSB    _u(15)
#define OTP_DATA_KEY6_4_LSB    _u(0)
#define OTP_DATA_KEY6_4_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_KEY6_5
// Description : Bits 95:80 of OTP access key 6 (ECC)
#define OTP_DATA_KEY6_5_ROW _u(0x00000f75)
#define OTP_DATA_KEY6_5_BITS   _u(0x0000ffff)
#define OTP_DATA_KEY6_5_RESET  "-"
#define OTP_DATA_KEY6_5_WIDTH  _u(16)
#define OTP_DATA_KEY6_5_MSB    _u(15)
#define OTP_DATA_KEY6_5_LSB    _u(0)
#define OTP_DATA_KEY6_5_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_KEY6_6
// Description : Bits 111:96 of OTP access key 6 (ECC)
#define OTP_DATA_KEY6_6_ROW _u(0x00000f76)
#define OTP_DATA_KEY6_6_BITS   _u(0x0000ffff)
#define OTP_DATA_KEY6_6_RESET  "-"
#define OTP_DATA_KEY6_6_WIDTH  _u(16)
#define OTP_DATA_KEY6_6_MSB    _u(15)
#define OTP_DATA_KEY6_6_LSB    _u(0)
#define OTP_DATA_KEY6_6_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_KEY6_7
// Description : Bits 127:112 of OTP access key 6 (ECC)
#define OTP_DATA_KEY6_7_ROW _u(0x00000f77)
#define OTP_DATA_KEY6_7_BITS   _u(0x0000ffff)
#define OTP_DATA_KEY6_7_RESET  "-"
#define OTP_DATA_KEY6_7_WIDTH  _u(16)
#define OTP_DATA_KEY6_7_MSB    _u(15)
#define OTP_DATA_KEY6_7_LSB    _u(0)
#define OTP_DATA_KEY6_7_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_KEY1_VALID
// Description : Valid flag for key 1. Once the valid flag is set, the key can
//               no longer be read or written, and becomes a valid fixed key for
//               protecting OTP pages.
#define OTP_DATA_KEY1_VALID_ROW _u(0x00000f79)
#define OTP_DATA_KEY1_VALID_BITS   _u(0x00010101)
#define OTP_DATA_KEY1_VALID_RESET  _u(0x00000000)
#define OTP_DATA_KEY1_VALID_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_KEY1_VALID_VALID_R2
// Description : Redundant copy of VALID, with 3-way majority vote
#define OTP_DATA_KEY1_VALID_VALID_R2_RESET  "-"
#define OTP_DATA_KEY1_VALID_VALID_R2_BITS   _u(0x00010000)
#define OTP_DATA_KEY1_VALID_VALID_R2_MSB    _u(16)
#define OTP_DATA_KEY1_VALID_VALID_R2_LSB    _u(16)
#define OTP_DATA_KEY1_VALID_VALID_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_KEY1_VALID_VALID_R1
// Description : Redundant copy of VALID, with 3-way majority vote
#define OTP_DATA_KEY1_VALID_VALID_R1_RESET  "-"
#define OTP_DATA_KEY1_VALID_VALID_R1_BITS   _u(0x00000100)
#define OTP_DATA_KEY1_VALID_VALID_R1_MSB    _u(8)
#define OTP_DATA_KEY1_VALID_VALID_R1_LSB    _u(8)
#define OTP_DATA_KEY1_VALID_VALID_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_KEY1_VALID_VALID
#define OTP_DATA_KEY1_VALID_VALID_RESET  "-"
#define OTP_DATA_KEY1_VALID_VALID_BITS   _u(0x00000001)
#define OTP_DATA_KEY1_VALID_VALID_MSB    _u(0)
#define OTP_DATA_KEY1_VALID_VALID_LSB    _u(0)
#define OTP_DATA_KEY1_VALID_VALID_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_KEY2_VALID
// Description : Valid flag for key 2. Once the valid flag is set, the key can
//               no longer be read or written, and becomes a valid fixed key for
//               protecting OTP pages.
#define OTP_DATA_KEY2_VALID_ROW _u(0x00000f7a)
#define OTP_DATA_KEY2_VALID_BITS   _u(0x00010101)
#define OTP_DATA_KEY2_VALID_RESET  _u(0x00000000)
#define OTP_DATA_KEY2_VALID_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_KEY2_VALID_VALID_R2
// Description : Redundant copy of VALID, with 3-way majority vote
#define OTP_DATA_KEY2_VALID_VALID_R2_RESET  "-"
#define OTP_DATA_KEY2_VALID_VALID_R2_BITS   _u(0x00010000)
#define OTP_DATA_KEY2_VALID_VALID_R2_MSB    _u(16)
#define OTP_DATA_KEY2_VALID_VALID_R2_LSB    _u(16)
#define OTP_DATA_KEY2_VALID_VALID_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_KEY2_VALID_VALID_R1
// Description : Redundant copy of VALID, with 3-way majority vote
#define OTP_DATA_KEY2_VALID_VALID_R1_RESET  "-"
#define OTP_DATA_KEY2_VALID_VALID_R1_BITS   _u(0x00000100)
#define OTP_DATA_KEY2_VALID_VALID_R1_MSB    _u(8)
#define OTP_DATA_KEY2_VALID_VALID_R1_LSB    _u(8)
#define OTP_DATA_KEY2_VALID_VALID_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_KEY2_VALID_VALID
#define OTP_DATA_KEY2_VALID_VALID_RESET  "-"
#define OTP_DATA_KEY2_VALID_VALID_BITS   _u(0x00000001)
#define OTP_DATA_KEY2_VALID_VALID_MSB    _u(0)
#define OTP_DATA_KEY2_VALID_VALID_LSB    _u(0)
#define OTP_DATA_KEY2_VALID_VALID_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_KEY3_VALID
// Description : Valid flag for key 3. Once the valid flag is set, the key can
//               no longer be read or written, and becomes a valid fixed key for
//               protecting OTP pages.
#define OTP_DATA_KEY3_VALID_ROW _u(0x00000f7b)
#define OTP_DATA_KEY3_VALID_BITS   _u(0x00010101)
#define OTP_DATA_KEY3_VALID_RESET  _u(0x00000000)
#define OTP_DATA_KEY3_VALID_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_KEY3_VALID_VALID_R2
// Description : Redundant copy of VALID, with 3-way majority vote
#define OTP_DATA_KEY3_VALID_VALID_R2_RESET  "-"
#define OTP_DATA_KEY3_VALID_VALID_R2_BITS   _u(0x00010000)
#define OTP_DATA_KEY3_VALID_VALID_R2_MSB    _u(16)
#define OTP_DATA_KEY3_VALID_VALID_R2_LSB    _u(16)
#define OTP_DATA_KEY3_VALID_VALID_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_KEY3_VALID_VALID_R1
// Description : Redundant copy of VALID, with 3-way majority vote
#define OTP_DATA_KEY3_VALID_VALID_R1_RESET  "-"
#define OTP_DATA_KEY3_VALID_VALID_R1_BITS   _u(0x00000100)
#define OTP_DATA_KEY3_VALID_VALID_R1_MSB    _u(8)
#define OTP_DATA_KEY3_VALID_VALID_R1_LSB    _u(8)
#define OTP_DATA_KEY3_VALID_VALID_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_KEY3_VALID_VALID
#define OTP_DATA_KEY3_VALID_VALID_RESET  "-"
#define OTP_DATA_KEY3_VALID_VALID_BITS   _u(0x00000001)
#define OTP_DATA_KEY3_VALID_VALID_MSB    _u(0)
#define OTP_DATA_KEY3_VALID_VALID_LSB    _u(0)
#define OTP_DATA_KEY3_VALID_VALID_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_KEY4_VALID
// Description : Valid flag for key 4. Once the valid flag is set, the key can
//               no longer be read or written, and becomes a valid fixed key for
//               protecting OTP pages.
#define OTP_DATA_KEY4_VALID_ROW _u(0x00000f7c)
#define OTP_DATA_KEY4_VALID_BITS   _u(0x00010101)
#define OTP_DATA_KEY4_VALID_RESET  _u(0x00000000)
#define OTP_DATA_KEY4_VALID_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_KEY4_VALID_VALID_R2
// Description : Redundant copy of VALID, with 3-way majority vote
#define OTP_DATA_KEY4_VALID_VALID_R2_RESET  "-"
#define OTP_DATA_KEY4_VALID_VALID_R2_BITS   _u(0x00010000)
#define OTP_DATA_KEY4_VALID_VALID_R2_MSB    _u(16)
#define OTP_DATA_KEY4_VALID_VALID_R2_LSB    _u(16)
#define OTP_DATA_KEY4_VALID_VALID_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_KEY4_VALID_VALID_R1
// Description : Redundant copy of VALID, with 3-way majority vote
#define OTP_DATA_KEY4_VALID_VALID_R1_RESET  "-"
#define OTP_DATA_KEY4_VALID_VALID_R1_BITS   _u(0x00000100)
#define OTP_DATA_KEY4_VALID_VALID_R1_MSB    _u(8)
#define OTP_DATA_KEY4_VALID_VALID_R1_LSB    _u(8)
#define OTP_DATA_KEY4_VALID_VALID_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_KEY4_VALID_VALID
#define OTP_DATA_KEY4_VALID_VALID_RESET  "-"
#define OTP_DATA_KEY4_VALID_VALID_BITS   _u(0x00000001)
#define OTP_DATA_KEY4_VALID_VALID_MSB    _u(0)
#define OTP_DATA_KEY4_VALID_VALID_LSB    _u(0)
#define OTP_DATA_KEY4_VALID_VALID_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_KEY5_VALID
// Description : Valid flag for key 5. Once the valid flag is set, the key can
//               no longer be read or written, and becomes a valid fixed key for
//               protecting OTP pages.
#define OTP_DATA_KEY5_VALID_ROW _u(0x00000f7d)
#define OTP_DATA_KEY5_VALID_BITS   _u(0x00010101)
#define OTP_DATA_KEY5_VALID_RESET  _u(0x00000000)
#define OTP_DATA_KEY5_VALID_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_KEY5_VALID_VALID_R2
// Description : Redundant copy of VALID, with 3-way majority vote
#define OTP_DATA_KEY5_VALID_VALID_R2_RESET  "-"
#define OTP_DATA_KEY5_VALID_VALID_R2_BITS   _u(0x00010000)
#define OTP_DATA_KEY5_VALID_VALID_R2_MSB    _u(16)
#define OTP_DATA_KEY5_VALID_VALID_R2_LSB    _u(16)
#define OTP_DATA_KEY5_VALID_VALID_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_KEY5_VALID_VALID_R1
// Description : Redundant copy of VALID, with 3-way majority vote
#define OTP_DATA_KEY5_VALID_VALID_R1_RESET  "-"
#define OTP_DATA_KEY5_VALID_VALID_R1_BITS   _u(0x00000100)
#define OTP_DATA_KEY5_VALID_VALID_R1_MSB    _u(8)
#define OTP_DATA_KEY5_VALID_VALID_R1_LSB    _u(8)
#define OTP_DATA_KEY5_VALID_VALID_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_KEY5_VALID_VALID
#define OTP_DATA_KEY5_VALID_VALID_RESET  "-"
#define OTP_DATA_KEY5_VALID_VALID_BITS   _u(0x00000001)
#define OTP_DATA_KEY5_VALID_VALID_MSB    _u(0)
#define OTP_DATA_KEY5_VALID_VALID_LSB    _u(0)
#define OTP_DATA_KEY5_VALID_VALID_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_KEY6_VALID
// Description : Valid flag for key 6. Once the valid flag is set, the key can
//               no longer be read or written, and becomes a valid fixed key for
//               protecting OTP pages.
#define OTP_DATA_KEY6_VALID_ROW _u(0x00000f7e)
#define OTP_DATA_KEY6_VALID_BITS   _u(0x00010101)
#define OTP_DATA_KEY6_VALID_RESET  _u(0x00000000)
#define OTP_DATA_KEY6_VALID_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_KEY6_VALID_VALID_R2
// Description : Redundant copy of VALID, with 3-way majority vote
#define OTP_DATA_KEY6_VALID_VALID_R2_RESET  "-"
#define OTP_DATA_KEY6_VALID_VALID_R2_BITS   _u(0x00010000)
#define OTP_DATA_KEY6_VALID_VALID_R2_MSB    _u(16)
#define OTP_DATA_KEY6_VALID_VALID_R2_LSB    _u(16)
#define OTP_DATA_KEY6_VALID_VALID_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_KEY6_VALID_VALID_R1
// Description : Redundant copy of VALID, with 3-way majority vote
#define OTP_DATA_KEY6_VALID_VALID_R1_RESET  "-"
#define OTP_DATA_KEY6_VALID_VALID_R1_BITS   _u(0x00000100)
#define OTP_DATA_KEY6_VALID_VALID_R1_MSB    _u(8)
#define OTP_DATA_KEY6_VALID_VALID_R1_LSB    _u(8)
#define OTP_DATA_KEY6_VALID_VALID_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_KEY6_VALID_VALID
#define OTP_DATA_KEY6_VALID_VALID_RESET  "-"
#define OTP_DATA_KEY6_VALID_VALID_BITS   _u(0x00000001)
#define OTP_DATA_KEY6_VALID_VALID_MSB    _u(0)
#define OTP_DATA_KEY6_VALID_VALID_LSB    _u(0)
#define OTP_DATA_KEY6_VALID_VALID_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE0_LOCK0
// Description : Lock configuration LSBs for page 0 (rows 0x0 through 0x3f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE0_LOCK0_ROW _u(0x00000f80)
#define OTP_DATA_PAGE0_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE0_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE0_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE0_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE0_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE0_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE0_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE0_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE0_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE0_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE0_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE0_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE0_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE0_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE0_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE0_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE0_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE0_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE0_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE0_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE0_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE0_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE0_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE0_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE0_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE0_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE0_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE0_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE0_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE0_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE0_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE0_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE0_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE0_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE0_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE0_LOCK1
// Description : Lock configuration MSBs for page 0 (rows 0x0 through 0x3f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE0_LOCK1_ROW _u(0x00000f81)
#define OTP_DATA_PAGE0_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE0_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE0_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE0_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE0_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE0_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE0_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE0_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE0_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE0_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE0_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE0_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE0_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE0_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE0_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE0_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE0_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE0_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE0_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE0_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE0_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE0_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE0_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE0_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE0_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE0_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE0_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE0_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE0_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE0_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE0_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE0_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE0_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE0_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE0_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE0_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE0_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE0_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE0_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE0_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE0_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE0_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE0_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE0_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE0_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE1_LOCK0
// Description : Lock configuration LSBs for page 1 (rows 0x40 through 0x7f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE1_LOCK0_ROW _u(0x00000f82)
#define OTP_DATA_PAGE1_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE1_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE1_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE1_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE1_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE1_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE1_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE1_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE1_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE1_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE1_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE1_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE1_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE1_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE1_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE1_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE1_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE1_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE1_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE1_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE1_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE1_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE1_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE1_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE1_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE1_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE1_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE1_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE1_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE1_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE1_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE1_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE1_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE1_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE1_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE1_LOCK1
// Description : Lock configuration MSBs for page 1 (rows 0x40 through 0x7f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE1_LOCK1_ROW _u(0x00000f83)
#define OTP_DATA_PAGE1_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE1_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE1_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE1_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE1_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE1_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE1_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE1_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE1_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE1_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE1_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE1_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE1_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE1_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE1_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE1_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE1_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE1_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE1_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE1_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE1_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE1_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE1_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE1_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE1_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE1_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE1_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE1_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE1_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE1_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE1_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE1_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE1_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE1_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE1_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE1_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE1_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE1_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE1_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE1_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE1_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE1_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE1_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE1_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE1_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE2_LOCK0
// Description : Lock configuration LSBs for page 2 (rows 0x80 through 0xbf).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE2_LOCK0_ROW _u(0x00000f84)
#define OTP_DATA_PAGE2_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE2_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE2_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE2_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE2_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE2_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE2_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE2_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE2_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE2_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE2_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE2_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE2_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE2_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE2_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE2_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE2_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE2_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE2_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE2_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE2_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE2_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE2_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE2_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE2_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE2_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE2_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE2_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE2_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE2_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE2_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE2_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE2_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE2_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE2_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE2_LOCK1
// Description : Lock configuration MSBs for page 2 (rows 0x80 through 0xbf).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE2_LOCK1_ROW _u(0x00000f85)
#define OTP_DATA_PAGE2_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE2_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE2_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE2_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE2_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE2_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE2_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE2_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE2_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE2_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE2_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE2_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE2_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE2_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE2_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE2_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE2_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE2_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE2_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE2_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE2_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE2_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE2_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE2_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE2_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE2_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE2_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE2_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE2_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE2_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE2_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE2_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE2_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE2_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE2_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE2_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE2_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE2_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE2_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE2_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE2_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE2_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE2_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE2_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE2_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE3_LOCK0
// Description : Lock configuration LSBs for page 3 (rows 0xc0 through 0xff).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE3_LOCK0_ROW _u(0x00000f86)
#define OTP_DATA_PAGE3_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE3_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE3_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE3_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE3_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE3_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE3_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE3_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE3_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE3_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE3_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE3_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE3_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE3_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE3_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE3_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE3_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE3_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE3_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE3_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE3_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE3_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE3_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE3_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE3_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE3_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE3_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE3_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE3_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE3_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE3_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE3_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE3_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE3_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE3_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE3_LOCK1
// Description : Lock configuration MSBs for page 3 (rows 0xc0 through 0xff).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE3_LOCK1_ROW _u(0x00000f87)
#define OTP_DATA_PAGE3_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE3_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE3_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE3_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE3_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE3_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE3_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE3_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE3_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE3_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE3_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE3_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE3_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE3_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE3_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE3_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE3_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE3_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE3_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE3_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE3_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE3_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE3_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE3_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE3_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE3_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE3_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE3_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE3_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE3_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE3_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE3_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE3_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE3_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE3_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE3_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE3_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE3_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE3_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE3_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE3_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE3_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE3_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE3_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE3_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE4_LOCK0
// Description : Lock configuration LSBs for page 4 (rows 0x100 through 0x13f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE4_LOCK0_ROW _u(0x00000f88)
#define OTP_DATA_PAGE4_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE4_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE4_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE4_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE4_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE4_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE4_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE4_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE4_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE4_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE4_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE4_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE4_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE4_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE4_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE4_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE4_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE4_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE4_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE4_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE4_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE4_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE4_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE4_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE4_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE4_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE4_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE4_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE4_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE4_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE4_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE4_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE4_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE4_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE4_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE4_LOCK1
// Description : Lock configuration MSBs for page 4 (rows 0x100 through 0x13f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE4_LOCK1_ROW _u(0x00000f89)
#define OTP_DATA_PAGE4_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE4_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE4_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE4_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE4_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE4_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE4_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE4_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE4_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE4_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE4_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE4_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE4_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE4_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE4_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE4_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE4_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE4_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE4_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE4_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE4_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE4_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE4_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE4_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE4_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE4_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE4_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE4_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE4_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE4_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE4_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE4_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE4_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE4_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE4_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE4_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE4_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE4_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE4_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE4_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE4_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE4_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE4_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE4_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE4_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE5_LOCK0
// Description : Lock configuration LSBs for page 5 (rows 0x140 through 0x17f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE5_LOCK0_ROW _u(0x00000f8a)
#define OTP_DATA_PAGE5_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE5_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE5_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE5_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE5_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE5_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE5_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE5_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE5_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE5_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE5_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE5_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE5_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE5_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE5_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE5_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE5_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE5_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE5_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE5_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE5_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE5_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE5_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE5_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE5_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE5_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE5_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE5_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE5_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE5_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE5_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE5_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE5_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE5_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE5_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE5_LOCK1
// Description : Lock configuration MSBs for page 5 (rows 0x140 through 0x17f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE5_LOCK1_ROW _u(0x00000f8b)
#define OTP_DATA_PAGE5_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE5_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE5_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE5_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE5_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE5_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE5_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE5_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE5_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE5_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE5_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE5_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE5_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE5_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE5_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE5_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE5_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE5_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE5_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE5_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE5_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE5_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE5_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE5_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE5_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE5_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE5_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE5_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE5_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE5_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE5_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE5_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE5_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE5_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE5_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE5_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE5_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE5_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE5_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE5_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE5_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE5_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE5_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE5_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE5_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE6_LOCK0
// Description : Lock configuration LSBs for page 6 (rows 0x180 through 0x1bf).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE6_LOCK0_ROW _u(0x00000f8c)
#define OTP_DATA_PAGE6_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE6_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE6_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE6_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE6_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE6_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE6_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE6_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE6_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE6_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE6_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE6_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE6_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE6_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE6_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE6_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE6_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE6_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE6_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE6_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE6_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE6_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE6_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE6_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE6_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE6_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE6_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE6_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE6_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE6_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE6_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE6_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE6_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE6_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE6_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE6_LOCK1
// Description : Lock configuration MSBs for page 6 (rows 0x180 through 0x1bf).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE6_LOCK1_ROW _u(0x00000f8d)
#define OTP_DATA_PAGE6_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE6_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE6_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE6_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE6_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE6_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE6_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE6_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE6_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE6_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE6_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE6_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE6_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE6_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE6_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE6_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE6_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE6_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE6_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE6_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE6_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE6_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE6_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE6_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE6_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE6_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE6_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE6_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE6_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE6_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE6_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE6_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE6_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE6_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE6_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE6_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE6_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE6_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE6_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE6_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE6_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE6_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE6_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE6_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE6_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE7_LOCK0
// Description : Lock configuration LSBs for page 7 (rows 0x1c0 through 0x1ff).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE7_LOCK0_ROW _u(0x00000f8e)
#define OTP_DATA_PAGE7_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE7_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE7_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE7_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE7_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE7_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE7_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE7_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE7_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE7_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE7_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE7_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE7_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE7_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE7_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE7_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE7_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE7_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE7_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE7_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE7_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE7_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE7_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE7_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE7_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE7_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE7_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE7_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE7_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE7_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE7_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE7_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE7_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE7_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE7_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE7_LOCK1
// Description : Lock configuration MSBs for page 7 (rows 0x1c0 through 0x1ff).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE7_LOCK1_ROW _u(0x00000f8f)
#define OTP_DATA_PAGE7_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE7_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE7_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE7_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE7_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE7_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE7_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE7_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE7_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE7_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE7_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE7_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE7_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE7_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE7_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE7_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE7_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE7_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE7_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE7_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE7_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE7_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE7_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE7_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE7_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE7_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE7_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE7_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE7_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE7_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE7_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE7_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE7_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE7_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE7_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE7_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE7_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE7_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE7_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE7_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE7_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE7_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE7_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE7_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE7_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE8_LOCK0
// Description : Lock configuration LSBs for page 8 (rows 0x200 through 0x23f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE8_LOCK0_ROW _u(0x00000f90)
#define OTP_DATA_PAGE8_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE8_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE8_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE8_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE8_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE8_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE8_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE8_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE8_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE8_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE8_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE8_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE8_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE8_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE8_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE8_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE8_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE8_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE8_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE8_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE8_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE8_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE8_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE8_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE8_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE8_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE8_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE8_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE8_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE8_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE8_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE8_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE8_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE8_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE8_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE8_LOCK1
// Description : Lock configuration MSBs for page 8 (rows 0x200 through 0x23f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE8_LOCK1_ROW _u(0x00000f91)
#define OTP_DATA_PAGE8_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE8_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE8_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE8_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE8_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE8_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE8_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE8_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE8_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE8_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE8_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE8_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE8_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE8_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE8_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE8_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE8_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE8_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE8_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE8_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE8_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE8_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE8_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE8_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE8_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE8_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE8_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE8_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE8_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE8_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE8_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE8_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE8_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE8_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE8_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE8_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE8_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE8_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE8_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE8_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE8_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE8_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE8_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE8_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE8_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE9_LOCK0
// Description : Lock configuration LSBs for page 9 (rows 0x240 through 0x27f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE9_LOCK0_ROW _u(0x00000f92)
#define OTP_DATA_PAGE9_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE9_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE9_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE9_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE9_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE9_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE9_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE9_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE9_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE9_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE9_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE9_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE9_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE9_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE9_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE9_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE9_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE9_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE9_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE9_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE9_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE9_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE9_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE9_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE9_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE9_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE9_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE9_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE9_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE9_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE9_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE9_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE9_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE9_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE9_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE9_LOCK1
// Description : Lock configuration MSBs for page 9 (rows 0x240 through 0x27f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE9_LOCK1_ROW _u(0x00000f93)
#define OTP_DATA_PAGE9_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE9_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE9_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE9_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE9_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE9_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE9_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE9_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE9_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE9_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE9_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE9_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE9_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE9_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE9_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE9_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE9_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE9_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE9_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE9_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE9_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE9_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE9_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE9_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE9_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE9_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE9_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE9_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE9_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE9_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE9_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE9_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE9_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE9_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE9_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE9_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE9_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE9_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE9_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE9_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE9_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE9_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE9_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE9_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE9_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE10_LOCK0
// Description : Lock configuration LSBs for page 10 (rows 0x280 through 0x2bf).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE10_LOCK0_ROW _u(0x00000f94)
#define OTP_DATA_PAGE10_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE10_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE10_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE10_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE10_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE10_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE10_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE10_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE10_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE10_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE10_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE10_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE10_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE10_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE10_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE10_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE10_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE10_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE10_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE10_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE10_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE10_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE10_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE10_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE10_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE10_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE10_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE10_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE10_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE10_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE10_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE10_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE10_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE10_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE10_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE10_LOCK1
// Description : Lock configuration MSBs for page 10 (rows 0x280 through 0x2bf).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE10_LOCK1_ROW _u(0x00000f95)
#define OTP_DATA_PAGE10_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE10_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE10_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE10_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE10_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE10_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE10_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE10_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE10_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE10_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE10_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE10_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE10_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE10_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE10_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE10_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE10_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE10_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE10_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE10_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE10_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE10_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE10_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE10_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE10_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE10_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE10_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE10_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE10_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE10_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE10_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE10_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE10_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE10_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE10_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE10_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE10_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE10_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE10_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE10_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE10_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE10_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE10_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE10_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE10_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE11_LOCK0
// Description : Lock configuration LSBs for page 11 (rows 0x2c0 through 0x2ff).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE11_LOCK0_ROW _u(0x00000f96)
#define OTP_DATA_PAGE11_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE11_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE11_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE11_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE11_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE11_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE11_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE11_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE11_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE11_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE11_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE11_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE11_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE11_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE11_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE11_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE11_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE11_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE11_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE11_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE11_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE11_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE11_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE11_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE11_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE11_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE11_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE11_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE11_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE11_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE11_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE11_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE11_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE11_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE11_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE11_LOCK1
// Description : Lock configuration MSBs for page 11 (rows 0x2c0 through 0x2ff).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE11_LOCK1_ROW _u(0x00000f97)
#define OTP_DATA_PAGE11_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE11_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE11_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE11_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE11_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE11_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE11_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE11_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE11_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE11_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE11_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE11_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE11_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE11_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE11_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE11_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE11_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE11_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE11_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE11_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE11_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE11_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE11_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE11_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE11_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE11_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE11_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE11_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE11_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE11_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE11_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE11_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE11_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE11_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE11_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE11_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE11_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE11_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE11_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE11_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE11_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE11_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE11_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE11_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE11_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE12_LOCK0
// Description : Lock configuration LSBs for page 12 (rows 0x300 through 0x33f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE12_LOCK0_ROW _u(0x00000f98)
#define OTP_DATA_PAGE12_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE12_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE12_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE12_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE12_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE12_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE12_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE12_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE12_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE12_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE12_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE12_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE12_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE12_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE12_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE12_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE12_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE12_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE12_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE12_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE12_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE12_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE12_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE12_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE12_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE12_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE12_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE12_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE12_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE12_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE12_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE12_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE12_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE12_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE12_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE12_LOCK1
// Description : Lock configuration MSBs for page 12 (rows 0x300 through 0x33f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE12_LOCK1_ROW _u(0x00000f99)
#define OTP_DATA_PAGE12_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE12_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE12_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE12_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE12_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE12_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE12_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE12_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE12_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE12_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE12_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE12_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE12_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE12_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE12_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE12_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE12_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE12_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE12_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE12_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE12_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE12_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE12_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE12_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE12_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE12_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE12_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE12_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE12_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE12_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE12_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE12_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE12_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE12_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE12_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE12_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE12_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE12_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE12_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE12_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE12_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE12_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE12_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE12_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE12_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE13_LOCK0
// Description : Lock configuration LSBs for page 13 (rows 0x340 through 0x37f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE13_LOCK0_ROW _u(0x00000f9a)
#define OTP_DATA_PAGE13_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE13_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE13_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE13_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE13_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE13_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE13_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE13_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE13_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE13_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE13_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE13_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE13_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE13_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE13_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE13_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE13_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE13_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE13_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE13_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE13_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE13_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE13_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE13_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE13_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE13_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE13_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE13_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE13_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE13_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE13_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE13_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE13_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE13_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE13_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE13_LOCK1
// Description : Lock configuration MSBs for page 13 (rows 0x340 through 0x37f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE13_LOCK1_ROW _u(0x00000f9b)
#define OTP_DATA_PAGE13_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE13_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE13_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE13_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE13_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE13_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE13_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE13_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE13_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE13_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE13_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE13_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE13_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE13_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE13_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE13_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE13_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE13_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE13_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE13_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE13_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE13_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE13_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE13_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE13_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE13_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE13_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE13_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE13_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE13_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE13_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE13_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE13_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE13_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE13_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE13_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE13_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE13_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE13_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE13_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE13_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE13_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE13_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE13_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE13_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE14_LOCK0
// Description : Lock configuration LSBs for page 14 (rows 0x380 through 0x3bf).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE14_LOCK0_ROW _u(0x00000f9c)
#define OTP_DATA_PAGE14_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE14_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE14_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE14_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE14_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE14_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE14_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE14_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE14_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE14_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE14_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE14_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE14_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE14_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE14_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE14_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE14_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE14_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE14_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE14_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE14_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE14_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE14_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE14_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE14_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE14_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE14_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE14_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE14_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE14_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE14_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE14_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE14_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE14_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE14_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE14_LOCK1
// Description : Lock configuration MSBs for page 14 (rows 0x380 through 0x3bf).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE14_LOCK1_ROW _u(0x00000f9d)
#define OTP_DATA_PAGE14_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE14_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE14_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE14_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE14_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE14_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE14_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE14_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE14_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE14_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE14_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE14_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE14_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE14_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE14_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE14_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE14_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE14_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE14_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE14_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE14_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE14_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE14_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE14_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE14_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE14_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE14_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE14_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE14_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE14_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE14_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE14_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE14_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE14_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE14_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE14_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE14_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE14_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE14_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE14_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE14_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE14_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE14_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE14_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE14_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE15_LOCK0
// Description : Lock configuration LSBs for page 15 (rows 0x3c0 through 0x3ff).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE15_LOCK0_ROW _u(0x00000f9e)
#define OTP_DATA_PAGE15_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE15_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE15_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE15_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE15_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE15_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE15_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE15_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE15_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE15_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE15_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE15_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE15_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE15_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE15_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE15_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE15_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE15_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE15_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE15_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE15_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE15_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE15_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE15_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE15_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE15_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE15_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE15_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE15_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE15_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE15_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE15_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE15_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE15_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE15_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE15_LOCK1
// Description : Lock configuration MSBs for page 15 (rows 0x3c0 through 0x3ff).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE15_LOCK1_ROW _u(0x00000f9f)
#define OTP_DATA_PAGE15_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE15_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE15_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE15_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE15_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE15_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE15_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE15_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE15_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE15_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE15_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE15_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE15_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE15_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE15_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE15_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE15_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE15_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE15_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE15_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE15_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE15_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE15_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE15_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE15_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE15_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE15_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE15_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE15_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE15_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE15_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE15_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE15_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE15_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE15_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE15_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE15_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE15_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE15_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE15_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE15_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE15_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE15_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE15_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE15_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE16_LOCK0
// Description : Lock configuration LSBs for page 16 (rows 0x400 through 0x43f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE16_LOCK0_ROW _u(0x00000fa0)
#define OTP_DATA_PAGE16_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE16_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE16_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE16_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE16_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE16_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE16_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE16_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE16_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE16_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE16_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE16_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE16_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE16_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE16_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE16_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE16_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE16_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE16_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE16_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE16_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE16_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE16_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE16_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE16_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE16_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE16_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE16_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE16_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE16_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE16_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE16_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE16_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE16_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE16_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE16_LOCK1
// Description : Lock configuration MSBs for page 16 (rows 0x400 through 0x43f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE16_LOCK1_ROW _u(0x00000fa1)
#define OTP_DATA_PAGE16_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE16_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE16_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE16_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE16_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE16_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE16_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE16_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE16_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE16_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE16_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE16_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE16_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE16_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE16_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE16_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE16_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE16_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE16_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE16_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE16_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE16_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE16_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE16_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE16_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE16_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE16_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE16_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE16_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE16_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE16_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE16_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE16_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE16_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE16_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE16_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE16_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE16_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE16_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE16_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE16_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE16_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE16_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE16_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE16_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE17_LOCK0
// Description : Lock configuration LSBs for page 17 (rows 0x440 through 0x47f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE17_LOCK0_ROW _u(0x00000fa2)
#define OTP_DATA_PAGE17_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE17_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE17_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE17_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE17_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE17_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE17_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE17_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE17_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE17_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE17_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE17_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE17_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE17_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE17_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE17_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE17_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE17_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE17_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE17_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE17_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE17_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE17_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE17_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE17_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE17_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE17_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE17_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE17_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE17_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE17_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE17_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE17_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE17_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE17_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE17_LOCK1
// Description : Lock configuration MSBs for page 17 (rows 0x440 through 0x47f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE17_LOCK1_ROW _u(0x00000fa3)
#define OTP_DATA_PAGE17_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE17_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE17_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE17_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE17_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE17_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE17_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE17_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE17_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE17_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE17_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE17_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE17_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE17_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE17_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE17_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE17_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE17_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE17_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE17_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE17_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE17_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE17_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE17_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE17_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE17_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE17_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE17_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE17_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE17_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE17_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE17_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE17_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE17_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE17_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE17_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE17_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE17_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE17_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE17_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE17_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE17_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE17_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE17_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE17_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE18_LOCK0
// Description : Lock configuration LSBs for page 18 (rows 0x480 through 0x4bf).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE18_LOCK0_ROW _u(0x00000fa4)
#define OTP_DATA_PAGE18_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE18_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE18_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE18_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE18_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE18_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE18_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE18_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE18_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE18_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE18_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE18_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE18_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE18_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE18_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE18_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE18_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE18_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE18_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE18_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE18_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE18_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE18_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE18_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE18_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE18_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE18_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE18_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE18_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE18_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE18_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE18_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE18_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE18_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE18_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE18_LOCK1
// Description : Lock configuration MSBs for page 18 (rows 0x480 through 0x4bf).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE18_LOCK1_ROW _u(0x00000fa5)
#define OTP_DATA_PAGE18_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE18_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE18_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE18_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE18_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE18_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE18_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE18_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE18_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE18_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE18_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE18_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE18_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE18_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE18_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE18_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE18_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE18_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE18_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE18_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE18_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE18_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE18_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE18_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE18_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE18_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE18_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE18_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE18_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE18_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE18_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE18_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE18_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE18_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE18_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE18_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE18_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE18_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE18_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE18_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE18_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE18_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE18_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE18_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE18_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE19_LOCK0
// Description : Lock configuration LSBs for page 19 (rows 0x4c0 through 0x4ff).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE19_LOCK0_ROW _u(0x00000fa6)
#define OTP_DATA_PAGE19_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE19_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE19_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE19_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE19_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE19_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE19_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE19_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE19_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE19_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE19_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE19_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE19_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE19_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE19_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE19_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE19_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE19_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE19_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE19_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE19_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE19_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE19_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE19_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE19_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE19_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE19_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE19_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE19_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE19_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE19_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE19_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE19_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE19_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE19_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE19_LOCK1
// Description : Lock configuration MSBs for page 19 (rows 0x4c0 through 0x4ff).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE19_LOCK1_ROW _u(0x00000fa7)
#define OTP_DATA_PAGE19_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE19_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE19_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE19_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE19_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE19_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE19_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE19_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE19_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE19_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE19_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE19_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE19_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE19_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE19_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE19_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE19_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE19_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE19_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE19_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE19_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE19_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE19_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE19_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE19_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE19_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE19_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE19_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE19_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE19_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE19_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE19_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE19_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE19_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE19_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE19_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE19_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE19_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE19_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE19_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE19_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE19_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE19_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE19_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE19_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE20_LOCK0
// Description : Lock configuration LSBs for page 20 (rows 0x500 through 0x53f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE20_LOCK0_ROW _u(0x00000fa8)
#define OTP_DATA_PAGE20_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE20_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE20_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE20_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE20_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE20_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE20_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE20_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE20_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE20_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE20_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE20_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE20_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE20_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE20_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE20_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE20_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE20_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE20_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE20_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE20_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE20_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE20_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE20_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE20_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE20_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE20_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE20_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE20_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE20_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE20_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE20_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE20_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE20_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE20_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE20_LOCK1
// Description : Lock configuration MSBs for page 20 (rows 0x500 through 0x53f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE20_LOCK1_ROW _u(0x00000fa9)
#define OTP_DATA_PAGE20_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE20_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE20_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE20_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE20_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE20_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE20_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE20_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE20_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE20_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE20_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE20_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE20_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE20_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE20_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE20_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE20_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE20_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE20_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE20_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE20_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE20_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE20_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE20_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE20_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE20_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE20_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE20_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE20_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE20_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE20_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE20_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE20_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE20_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE20_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE20_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE20_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE20_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE20_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE20_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE20_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE20_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE20_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE20_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE20_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE21_LOCK0
// Description : Lock configuration LSBs for page 21 (rows 0x540 through 0x57f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE21_LOCK0_ROW _u(0x00000faa)
#define OTP_DATA_PAGE21_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE21_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE21_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE21_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE21_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE21_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE21_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE21_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE21_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE21_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE21_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE21_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE21_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE21_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE21_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE21_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE21_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE21_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE21_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE21_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE21_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE21_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE21_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE21_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE21_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE21_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE21_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE21_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE21_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE21_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE21_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE21_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE21_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE21_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE21_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE21_LOCK1
// Description : Lock configuration MSBs for page 21 (rows 0x540 through 0x57f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE21_LOCK1_ROW _u(0x00000fab)
#define OTP_DATA_PAGE21_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE21_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE21_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE21_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE21_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE21_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE21_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE21_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE21_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE21_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE21_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE21_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE21_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE21_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE21_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE21_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE21_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE21_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE21_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE21_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE21_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE21_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE21_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE21_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE21_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE21_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE21_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE21_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE21_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE21_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE21_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE21_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE21_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE21_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE21_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE21_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE21_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE21_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE21_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE21_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE21_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE21_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE21_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE21_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE21_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE22_LOCK0
// Description : Lock configuration LSBs for page 22 (rows 0x580 through 0x5bf).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE22_LOCK0_ROW _u(0x00000fac)
#define OTP_DATA_PAGE22_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE22_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE22_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE22_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE22_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE22_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE22_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE22_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE22_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE22_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE22_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE22_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE22_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE22_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE22_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE22_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE22_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE22_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE22_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE22_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE22_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE22_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE22_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE22_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE22_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE22_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE22_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE22_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE22_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE22_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE22_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE22_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE22_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE22_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE22_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE22_LOCK1
// Description : Lock configuration MSBs for page 22 (rows 0x580 through 0x5bf).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE22_LOCK1_ROW _u(0x00000fad)
#define OTP_DATA_PAGE22_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE22_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE22_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE22_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE22_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE22_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE22_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE22_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE22_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE22_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE22_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE22_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE22_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE22_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE22_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE22_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE22_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE22_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE22_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE22_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE22_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE22_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE22_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE22_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE22_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE22_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE22_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE22_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE22_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE22_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE22_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE22_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE22_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE22_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE22_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE22_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE22_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE22_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE22_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE22_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE22_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE22_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE22_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE22_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE22_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE23_LOCK0
// Description : Lock configuration LSBs for page 23 (rows 0x5c0 through 0x5ff).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE23_LOCK0_ROW _u(0x00000fae)
#define OTP_DATA_PAGE23_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE23_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE23_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE23_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE23_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE23_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE23_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE23_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE23_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE23_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE23_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE23_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE23_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE23_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE23_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE23_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE23_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE23_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE23_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE23_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE23_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE23_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE23_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE23_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE23_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE23_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE23_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE23_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE23_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE23_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE23_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE23_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE23_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE23_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE23_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE23_LOCK1
// Description : Lock configuration MSBs for page 23 (rows 0x5c0 through 0x5ff).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE23_LOCK1_ROW _u(0x00000faf)
#define OTP_DATA_PAGE23_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE23_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE23_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE23_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE23_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE23_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE23_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE23_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE23_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE23_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE23_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE23_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE23_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE23_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE23_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE23_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE23_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE23_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE23_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE23_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE23_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE23_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE23_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE23_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE23_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE23_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE23_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE23_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE23_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE23_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE23_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE23_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE23_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE23_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE23_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE23_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE23_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE23_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE23_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE23_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE23_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE23_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE23_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE23_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE23_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE24_LOCK0
// Description : Lock configuration LSBs for page 24 (rows 0x600 through 0x63f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE24_LOCK0_ROW _u(0x00000fb0)
#define OTP_DATA_PAGE24_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE24_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE24_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE24_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE24_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE24_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE24_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE24_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE24_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE24_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE24_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE24_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE24_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE24_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE24_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE24_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE24_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE24_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE24_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE24_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE24_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE24_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE24_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE24_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE24_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE24_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE24_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE24_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE24_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE24_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE24_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE24_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE24_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE24_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE24_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE24_LOCK1
// Description : Lock configuration MSBs for page 24 (rows 0x600 through 0x63f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE24_LOCK1_ROW _u(0x00000fb1)
#define OTP_DATA_PAGE24_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE24_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE24_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE24_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE24_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE24_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE24_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE24_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE24_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE24_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE24_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE24_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE24_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE24_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE24_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE24_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE24_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE24_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE24_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE24_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE24_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE24_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE24_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE24_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE24_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE24_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE24_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE24_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE24_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE24_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE24_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE24_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE24_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE24_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE24_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE24_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE24_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE24_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE24_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE24_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE24_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE24_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE24_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE24_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE24_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE25_LOCK0
// Description : Lock configuration LSBs for page 25 (rows 0x640 through 0x67f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE25_LOCK0_ROW _u(0x00000fb2)
#define OTP_DATA_PAGE25_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE25_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE25_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE25_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE25_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE25_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE25_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE25_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE25_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE25_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE25_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE25_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE25_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE25_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE25_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE25_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE25_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE25_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE25_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE25_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE25_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE25_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE25_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE25_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE25_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE25_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE25_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE25_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE25_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE25_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE25_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE25_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE25_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE25_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE25_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE25_LOCK1
// Description : Lock configuration MSBs for page 25 (rows 0x640 through 0x67f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE25_LOCK1_ROW _u(0x00000fb3)
#define OTP_DATA_PAGE25_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE25_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE25_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE25_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE25_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE25_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE25_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE25_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE25_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE25_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE25_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE25_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE25_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE25_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE25_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE25_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE25_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE25_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE25_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE25_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE25_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE25_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE25_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE25_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE25_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE25_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE25_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE25_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE25_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE25_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE25_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE25_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE25_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE25_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE25_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE25_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE25_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE25_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE25_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE25_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE25_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE25_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE25_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE25_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE25_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE26_LOCK0
// Description : Lock configuration LSBs for page 26 (rows 0x680 through 0x6bf).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE26_LOCK0_ROW _u(0x00000fb4)
#define OTP_DATA_PAGE26_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE26_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE26_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE26_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE26_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE26_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE26_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE26_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE26_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE26_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE26_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE26_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE26_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE26_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE26_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE26_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE26_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE26_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE26_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE26_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE26_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE26_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE26_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE26_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE26_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE26_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE26_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE26_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE26_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE26_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE26_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE26_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE26_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE26_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE26_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE26_LOCK1
// Description : Lock configuration MSBs for page 26 (rows 0x680 through 0x6bf).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE26_LOCK1_ROW _u(0x00000fb5)
#define OTP_DATA_PAGE26_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE26_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE26_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE26_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE26_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE26_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE26_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE26_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE26_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE26_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE26_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE26_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE26_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE26_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE26_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE26_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE26_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE26_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE26_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE26_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE26_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE26_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE26_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE26_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE26_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE26_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE26_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE26_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE26_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE26_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE26_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE26_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE26_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE26_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE26_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE26_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE26_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE26_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE26_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE26_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE26_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE26_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE26_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE26_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE26_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE27_LOCK0
// Description : Lock configuration LSBs for page 27 (rows 0x6c0 through 0x6ff).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE27_LOCK0_ROW _u(0x00000fb6)
#define OTP_DATA_PAGE27_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE27_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE27_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE27_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE27_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE27_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE27_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE27_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE27_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE27_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE27_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE27_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE27_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE27_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE27_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE27_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE27_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE27_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE27_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE27_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE27_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE27_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE27_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE27_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE27_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE27_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE27_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE27_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE27_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE27_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE27_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE27_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE27_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE27_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE27_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE27_LOCK1
// Description : Lock configuration MSBs for page 27 (rows 0x6c0 through 0x6ff).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE27_LOCK1_ROW _u(0x00000fb7)
#define OTP_DATA_PAGE27_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE27_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE27_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE27_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE27_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE27_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE27_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE27_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE27_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE27_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE27_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE27_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE27_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE27_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE27_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE27_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE27_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE27_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE27_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE27_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE27_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE27_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE27_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE27_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE27_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE27_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE27_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE27_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE27_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE27_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE27_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE27_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE27_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE27_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE27_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE27_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE27_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE27_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE27_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE27_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE27_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE27_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE27_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE27_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE27_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE28_LOCK0
// Description : Lock configuration LSBs for page 28 (rows 0x700 through 0x73f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE28_LOCK0_ROW _u(0x00000fb8)
#define OTP_DATA_PAGE28_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE28_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE28_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE28_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE28_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE28_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE28_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE28_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE28_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE28_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE28_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE28_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE28_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE28_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE28_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE28_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE28_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE28_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE28_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE28_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE28_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE28_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE28_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE28_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE28_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE28_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE28_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE28_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE28_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE28_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE28_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE28_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE28_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE28_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE28_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE28_LOCK1
// Description : Lock configuration MSBs for page 28 (rows 0x700 through 0x73f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE28_LOCK1_ROW _u(0x00000fb9)
#define OTP_DATA_PAGE28_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE28_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE28_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE28_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE28_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE28_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE28_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE28_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE28_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE28_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE28_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE28_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE28_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE28_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE28_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE28_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE28_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE28_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE28_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE28_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE28_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE28_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE28_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE28_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE28_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE28_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE28_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE28_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE28_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE28_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE28_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE28_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE28_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE28_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE28_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE28_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE28_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE28_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE28_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE28_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE28_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE28_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE28_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE28_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE28_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE29_LOCK0
// Description : Lock configuration LSBs for page 29 (rows 0x740 through 0x77f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE29_LOCK0_ROW _u(0x00000fba)
#define OTP_DATA_PAGE29_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE29_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE29_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE29_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE29_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE29_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE29_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE29_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE29_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE29_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE29_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE29_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE29_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE29_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE29_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE29_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE29_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE29_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE29_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE29_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE29_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE29_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE29_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE29_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE29_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE29_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE29_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE29_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE29_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE29_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE29_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE29_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE29_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE29_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE29_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE29_LOCK1
// Description : Lock configuration MSBs for page 29 (rows 0x740 through 0x77f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE29_LOCK1_ROW _u(0x00000fbb)
#define OTP_DATA_PAGE29_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE29_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE29_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE29_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE29_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE29_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE29_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE29_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE29_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE29_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE29_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE29_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE29_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE29_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE29_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE29_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE29_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE29_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE29_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE29_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE29_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE29_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE29_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE29_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE29_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE29_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE29_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE29_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE29_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE29_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE29_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE29_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE29_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE29_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE29_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE29_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE29_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE29_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE29_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE29_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE29_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE29_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE29_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE29_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE29_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE30_LOCK0
// Description : Lock configuration LSBs for page 30 (rows 0x780 through 0x7bf).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE30_LOCK0_ROW _u(0x00000fbc)
#define OTP_DATA_PAGE30_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE30_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE30_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE30_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE30_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE30_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE30_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE30_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE30_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE30_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE30_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE30_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE30_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE30_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE30_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE30_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE30_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE30_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE30_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE30_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE30_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE30_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE30_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE30_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE30_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE30_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE30_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE30_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE30_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE30_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE30_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE30_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE30_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE30_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE30_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE30_LOCK1
// Description : Lock configuration MSBs for page 30 (rows 0x780 through 0x7bf).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE30_LOCK1_ROW _u(0x00000fbd)
#define OTP_DATA_PAGE30_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE30_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE30_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE30_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE30_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE30_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE30_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE30_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE30_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE30_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE30_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE30_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE30_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE30_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE30_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE30_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE30_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE30_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE30_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE30_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE30_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE30_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE30_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE30_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE30_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE30_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE30_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE30_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE30_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE30_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE30_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE30_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE30_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE30_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE30_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE30_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE30_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE30_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE30_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE30_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE30_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE30_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE30_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE30_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE30_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE31_LOCK0
// Description : Lock configuration LSBs for page 31 (rows 0x7c0 through 0x7ff).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE31_LOCK0_ROW _u(0x00000fbe)
#define OTP_DATA_PAGE31_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE31_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE31_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE31_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE31_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE31_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE31_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE31_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE31_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE31_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE31_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE31_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE31_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE31_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE31_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE31_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE31_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE31_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE31_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE31_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE31_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE31_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE31_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE31_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE31_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE31_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE31_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE31_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE31_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE31_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE31_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE31_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE31_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE31_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE31_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE31_LOCK1
// Description : Lock configuration MSBs for page 31 (rows 0x7c0 through 0x7ff).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE31_LOCK1_ROW _u(0x00000fbf)
#define OTP_DATA_PAGE31_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE31_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE31_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE31_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE31_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE31_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE31_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE31_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE31_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE31_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE31_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE31_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE31_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE31_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE31_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE31_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE31_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE31_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE31_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE31_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE31_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE31_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE31_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE31_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE31_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE31_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE31_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE31_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE31_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE31_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE31_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE31_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE31_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE31_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE31_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE31_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE31_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE31_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE31_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE31_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE31_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE31_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE31_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE31_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE31_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE32_LOCK0
// Description : Lock configuration LSBs for page 32 (rows 0x800 through 0x83f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE32_LOCK0_ROW _u(0x00000fc0)
#define OTP_DATA_PAGE32_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE32_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE32_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE32_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE32_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE32_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE32_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE32_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE32_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE32_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE32_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE32_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE32_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE32_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE32_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE32_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE32_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE32_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE32_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE32_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE32_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE32_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE32_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE32_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE32_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE32_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE32_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE32_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE32_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE32_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE32_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE32_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE32_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE32_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE32_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE32_LOCK1
// Description : Lock configuration MSBs for page 32 (rows 0x800 through 0x83f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE32_LOCK1_ROW _u(0x00000fc1)
#define OTP_DATA_PAGE32_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE32_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE32_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE32_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE32_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE32_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE32_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE32_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE32_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE32_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE32_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE32_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE32_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE32_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE32_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE32_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE32_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE32_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE32_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE32_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE32_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE32_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE32_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE32_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE32_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE32_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE32_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE32_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE32_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE32_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE32_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE32_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE32_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE32_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE32_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE32_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE32_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE32_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE32_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE32_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE32_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE32_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE32_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE32_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE32_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE33_LOCK0
// Description : Lock configuration LSBs for page 33 (rows 0x840 through 0x87f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE33_LOCK0_ROW _u(0x00000fc2)
#define OTP_DATA_PAGE33_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE33_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE33_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE33_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE33_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE33_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE33_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE33_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE33_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE33_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE33_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE33_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE33_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE33_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE33_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE33_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE33_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE33_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE33_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE33_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE33_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE33_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE33_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE33_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE33_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE33_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE33_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE33_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE33_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE33_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE33_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE33_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE33_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE33_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE33_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE33_LOCK1
// Description : Lock configuration MSBs for page 33 (rows 0x840 through 0x87f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE33_LOCK1_ROW _u(0x00000fc3)
#define OTP_DATA_PAGE33_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE33_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE33_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE33_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE33_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE33_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE33_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE33_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE33_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE33_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE33_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE33_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE33_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE33_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE33_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE33_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE33_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE33_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE33_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE33_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE33_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE33_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE33_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE33_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE33_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE33_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE33_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE33_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE33_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE33_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE33_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE33_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE33_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE33_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE33_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE33_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE33_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE33_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE33_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE33_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE33_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE33_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE33_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE33_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE33_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE34_LOCK0
// Description : Lock configuration LSBs for page 34 (rows 0x880 through 0x8bf).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE34_LOCK0_ROW _u(0x00000fc4)
#define OTP_DATA_PAGE34_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE34_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE34_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE34_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE34_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE34_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE34_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE34_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE34_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE34_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE34_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE34_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE34_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE34_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE34_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE34_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE34_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE34_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE34_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE34_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE34_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE34_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE34_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE34_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE34_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE34_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE34_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE34_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE34_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE34_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE34_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE34_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE34_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE34_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE34_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE34_LOCK1
// Description : Lock configuration MSBs for page 34 (rows 0x880 through 0x8bf).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE34_LOCK1_ROW _u(0x00000fc5)
#define OTP_DATA_PAGE34_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE34_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE34_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE34_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE34_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE34_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE34_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE34_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE34_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE34_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE34_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE34_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE34_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE34_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE34_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE34_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE34_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE34_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE34_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE34_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE34_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE34_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE34_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE34_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE34_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE34_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE34_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE34_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE34_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE34_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE34_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE34_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE34_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE34_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE34_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE34_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE34_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE34_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE34_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE34_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE34_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE34_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE34_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE34_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE34_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE35_LOCK0
// Description : Lock configuration LSBs for page 35 (rows 0x8c0 through 0x8ff).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE35_LOCK0_ROW _u(0x00000fc6)
#define OTP_DATA_PAGE35_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE35_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE35_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE35_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE35_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE35_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE35_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE35_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE35_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE35_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE35_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE35_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE35_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE35_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE35_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE35_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE35_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE35_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE35_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE35_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE35_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE35_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE35_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE35_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE35_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE35_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE35_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE35_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE35_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE35_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE35_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE35_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE35_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE35_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE35_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE35_LOCK1
// Description : Lock configuration MSBs for page 35 (rows 0x8c0 through 0x8ff).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE35_LOCK1_ROW _u(0x00000fc7)
#define OTP_DATA_PAGE35_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE35_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE35_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE35_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE35_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE35_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE35_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE35_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE35_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE35_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE35_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE35_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE35_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE35_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE35_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE35_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE35_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE35_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE35_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE35_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE35_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE35_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE35_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE35_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE35_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE35_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE35_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE35_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE35_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE35_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE35_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE35_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE35_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE35_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE35_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE35_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE35_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE35_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE35_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE35_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE35_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE35_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE35_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE35_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE35_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE36_LOCK0
// Description : Lock configuration LSBs for page 36 (rows 0x900 through 0x93f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE36_LOCK0_ROW _u(0x00000fc8)
#define OTP_DATA_PAGE36_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE36_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE36_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE36_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE36_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE36_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE36_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE36_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE36_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE36_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE36_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE36_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE36_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE36_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE36_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE36_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE36_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE36_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE36_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE36_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE36_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE36_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE36_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE36_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE36_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE36_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE36_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE36_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE36_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE36_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE36_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE36_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE36_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE36_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE36_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE36_LOCK1
// Description : Lock configuration MSBs for page 36 (rows 0x900 through 0x93f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE36_LOCK1_ROW _u(0x00000fc9)
#define OTP_DATA_PAGE36_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE36_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE36_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE36_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE36_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE36_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE36_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE36_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE36_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE36_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE36_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE36_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE36_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE36_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE36_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE36_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE36_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE36_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE36_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE36_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE36_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE36_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE36_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE36_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE36_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE36_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE36_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE36_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE36_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE36_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE36_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE36_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE36_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE36_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE36_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE36_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE36_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE36_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE36_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE36_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE36_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE36_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE36_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE36_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE36_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE37_LOCK0
// Description : Lock configuration LSBs for page 37 (rows 0x940 through 0x97f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE37_LOCK0_ROW _u(0x00000fca)
#define OTP_DATA_PAGE37_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE37_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE37_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE37_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE37_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE37_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE37_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE37_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE37_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE37_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE37_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE37_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE37_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE37_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE37_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE37_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE37_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE37_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE37_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE37_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE37_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE37_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE37_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE37_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE37_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE37_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE37_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE37_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE37_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE37_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE37_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE37_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE37_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE37_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE37_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE37_LOCK1
// Description : Lock configuration MSBs for page 37 (rows 0x940 through 0x97f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE37_LOCK1_ROW _u(0x00000fcb)
#define OTP_DATA_PAGE37_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE37_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE37_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE37_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE37_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE37_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE37_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE37_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE37_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE37_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE37_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE37_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE37_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE37_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE37_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE37_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE37_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE37_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE37_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE37_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE37_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE37_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE37_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE37_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE37_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE37_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE37_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE37_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE37_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE37_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE37_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE37_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE37_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE37_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE37_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE37_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE37_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE37_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE37_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE37_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE37_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE37_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE37_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE37_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE37_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE38_LOCK0
// Description : Lock configuration LSBs for page 38 (rows 0x980 through 0x9bf).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE38_LOCK0_ROW _u(0x00000fcc)
#define OTP_DATA_PAGE38_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE38_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE38_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE38_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE38_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE38_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE38_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE38_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE38_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE38_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE38_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE38_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE38_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE38_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE38_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE38_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE38_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE38_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE38_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE38_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE38_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE38_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE38_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE38_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE38_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE38_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE38_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE38_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE38_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE38_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE38_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE38_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE38_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE38_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE38_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE38_LOCK1
// Description : Lock configuration MSBs for page 38 (rows 0x980 through 0x9bf).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE38_LOCK1_ROW _u(0x00000fcd)
#define OTP_DATA_PAGE38_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE38_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE38_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE38_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE38_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE38_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE38_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE38_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE38_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE38_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE38_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE38_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE38_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE38_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE38_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE38_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE38_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE38_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE38_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE38_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE38_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE38_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE38_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE38_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE38_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE38_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE38_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE38_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE38_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE38_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE38_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE38_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE38_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE38_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE38_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE38_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE38_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE38_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE38_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE38_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE38_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE38_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE38_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE38_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE38_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE39_LOCK0
// Description : Lock configuration LSBs for page 39 (rows 0x9c0 through 0x9ff).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE39_LOCK0_ROW _u(0x00000fce)
#define OTP_DATA_PAGE39_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE39_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE39_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE39_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE39_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE39_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE39_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE39_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE39_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE39_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE39_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE39_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE39_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE39_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE39_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE39_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE39_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE39_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE39_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE39_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE39_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE39_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE39_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE39_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE39_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE39_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE39_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE39_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE39_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE39_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE39_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE39_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE39_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE39_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE39_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE39_LOCK1
// Description : Lock configuration MSBs for page 39 (rows 0x9c0 through 0x9ff).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE39_LOCK1_ROW _u(0x00000fcf)
#define OTP_DATA_PAGE39_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE39_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE39_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE39_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE39_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE39_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE39_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE39_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE39_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE39_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE39_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE39_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE39_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE39_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE39_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE39_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE39_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE39_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE39_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE39_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE39_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE39_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE39_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE39_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE39_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE39_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE39_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE39_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE39_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE39_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE39_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE39_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE39_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE39_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE39_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE39_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE39_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE39_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE39_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE39_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE39_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE39_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE39_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE39_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE39_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE40_LOCK0
// Description : Lock configuration LSBs for page 40 (rows 0xa00 through 0xa3f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE40_LOCK0_ROW _u(0x00000fd0)
#define OTP_DATA_PAGE40_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE40_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE40_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE40_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE40_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE40_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE40_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE40_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE40_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE40_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE40_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE40_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE40_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE40_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE40_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE40_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE40_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE40_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE40_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE40_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE40_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE40_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE40_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE40_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE40_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE40_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE40_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE40_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE40_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE40_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE40_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE40_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE40_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE40_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE40_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE40_LOCK1
// Description : Lock configuration MSBs for page 40 (rows 0xa00 through 0xa3f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE40_LOCK1_ROW _u(0x00000fd1)
#define OTP_DATA_PAGE40_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE40_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE40_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE40_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE40_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE40_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE40_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE40_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE40_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE40_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE40_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE40_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE40_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE40_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE40_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE40_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE40_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE40_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE40_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE40_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE40_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE40_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE40_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE40_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE40_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE40_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE40_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE40_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE40_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE40_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE40_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE40_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE40_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE40_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE40_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE40_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE40_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE40_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE40_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE40_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE40_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE40_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE40_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE40_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE40_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE41_LOCK0
// Description : Lock configuration LSBs for page 41 (rows 0xa40 through 0xa7f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE41_LOCK0_ROW _u(0x00000fd2)
#define OTP_DATA_PAGE41_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE41_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE41_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE41_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE41_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE41_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE41_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE41_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE41_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE41_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE41_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE41_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE41_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE41_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE41_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE41_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE41_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE41_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE41_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE41_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE41_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE41_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE41_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE41_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE41_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE41_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE41_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE41_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE41_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE41_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE41_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE41_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE41_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE41_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE41_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE41_LOCK1
// Description : Lock configuration MSBs for page 41 (rows 0xa40 through 0xa7f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE41_LOCK1_ROW _u(0x00000fd3)
#define OTP_DATA_PAGE41_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE41_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE41_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE41_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE41_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE41_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE41_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE41_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE41_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE41_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE41_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE41_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE41_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE41_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE41_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE41_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE41_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE41_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE41_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE41_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE41_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE41_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE41_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE41_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE41_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE41_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE41_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE41_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE41_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE41_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE41_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE41_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE41_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE41_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE41_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE41_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE41_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE41_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE41_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE41_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE41_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE41_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE41_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE41_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE41_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE42_LOCK0
// Description : Lock configuration LSBs for page 42 (rows 0xa80 through 0xabf).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE42_LOCK0_ROW _u(0x00000fd4)
#define OTP_DATA_PAGE42_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE42_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE42_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE42_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE42_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE42_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE42_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE42_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE42_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE42_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE42_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE42_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE42_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE42_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE42_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE42_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE42_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE42_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE42_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE42_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE42_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE42_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE42_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE42_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE42_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE42_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE42_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE42_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE42_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE42_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE42_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE42_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE42_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE42_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE42_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE42_LOCK1
// Description : Lock configuration MSBs for page 42 (rows 0xa80 through 0xabf).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE42_LOCK1_ROW _u(0x00000fd5)
#define OTP_DATA_PAGE42_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE42_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE42_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE42_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE42_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE42_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE42_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE42_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE42_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE42_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE42_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE42_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE42_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE42_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE42_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE42_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE42_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE42_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE42_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE42_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE42_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE42_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE42_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE42_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE42_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE42_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE42_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE42_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE42_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE42_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE42_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE42_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE42_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE42_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE42_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE42_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE42_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE42_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE42_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE42_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE42_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE42_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE42_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE42_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE42_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE43_LOCK0
// Description : Lock configuration LSBs for page 43 (rows 0xac0 through 0xaff).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE43_LOCK0_ROW _u(0x00000fd6)
#define OTP_DATA_PAGE43_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE43_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE43_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE43_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE43_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE43_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE43_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE43_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE43_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE43_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE43_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE43_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE43_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE43_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE43_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE43_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE43_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE43_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE43_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE43_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE43_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE43_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE43_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE43_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE43_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE43_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE43_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE43_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE43_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE43_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE43_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE43_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE43_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE43_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE43_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE43_LOCK1
// Description : Lock configuration MSBs for page 43 (rows 0xac0 through 0xaff).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE43_LOCK1_ROW _u(0x00000fd7)
#define OTP_DATA_PAGE43_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE43_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE43_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE43_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE43_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE43_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE43_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE43_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE43_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE43_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE43_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE43_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE43_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE43_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE43_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE43_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE43_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE43_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE43_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE43_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE43_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE43_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE43_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE43_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE43_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE43_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE43_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE43_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE43_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE43_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE43_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE43_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE43_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE43_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE43_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE43_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE43_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE43_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE43_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE43_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE43_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE43_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE43_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE43_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE43_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE44_LOCK0
// Description : Lock configuration LSBs for page 44 (rows 0xb00 through 0xb3f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE44_LOCK0_ROW _u(0x00000fd8)
#define OTP_DATA_PAGE44_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE44_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE44_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE44_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE44_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE44_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE44_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE44_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE44_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE44_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE44_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE44_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE44_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE44_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE44_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE44_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE44_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE44_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE44_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE44_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE44_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE44_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE44_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE44_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE44_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE44_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE44_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE44_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE44_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE44_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE44_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE44_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE44_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE44_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE44_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE44_LOCK1
// Description : Lock configuration MSBs for page 44 (rows 0xb00 through 0xb3f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE44_LOCK1_ROW _u(0x00000fd9)
#define OTP_DATA_PAGE44_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE44_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE44_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE44_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE44_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE44_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE44_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE44_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE44_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE44_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE44_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE44_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE44_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE44_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE44_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE44_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE44_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE44_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE44_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE44_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE44_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE44_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE44_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE44_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE44_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE44_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE44_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE44_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE44_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE44_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE44_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE44_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE44_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE44_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE44_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE44_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE44_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE44_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE44_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE44_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE44_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE44_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE44_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE44_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE44_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE45_LOCK0
// Description : Lock configuration LSBs for page 45 (rows 0xb40 through 0xb7f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE45_LOCK0_ROW _u(0x00000fda)
#define OTP_DATA_PAGE45_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE45_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE45_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE45_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE45_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE45_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE45_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE45_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE45_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE45_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE45_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE45_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE45_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE45_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE45_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE45_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE45_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE45_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE45_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE45_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE45_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE45_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE45_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE45_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE45_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE45_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE45_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE45_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE45_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE45_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE45_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE45_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE45_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE45_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE45_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE45_LOCK1
// Description : Lock configuration MSBs for page 45 (rows 0xb40 through 0xb7f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE45_LOCK1_ROW _u(0x00000fdb)
#define OTP_DATA_PAGE45_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE45_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE45_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE45_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE45_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE45_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE45_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE45_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE45_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE45_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE45_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE45_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE45_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE45_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE45_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE45_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE45_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE45_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE45_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE45_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE45_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE45_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE45_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE45_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE45_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE45_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE45_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE45_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE45_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE45_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE45_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE45_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE45_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE45_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE45_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE45_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE45_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE45_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE45_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE45_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE45_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE45_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE45_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE45_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE45_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE46_LOCK0
// Description : Lock configuration LSBs for page 46 (rows 0xb80 through 0xbbf).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE46_LOCK0_ROW _u(0x00000fdc)
#define OTP_DATA_PAGE46_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE46_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE46_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE46_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE46_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE46_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE46_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE46_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE46_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE46_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE46_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE46_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE46_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE46_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE46_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE46_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE46_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE46_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE46_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE46_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE46_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE46_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE46_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE46_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE46_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE46_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE46_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE46_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE46_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE46_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE46_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE46_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE46_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE46_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE46_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE46_LOCK1
// Description : Lock configuration MSBs for page 46 (rows 0xb80 through 0xbbf).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE46_LOCK1_ROW _u(0x00000fdd)
#define OTP_DATA_PAGE46_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE46_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE46_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE46_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE46_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE46_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE46_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE46_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE46_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE46_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE46_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE46_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE46_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE46_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE46_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE46_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE46_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE46_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE46_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE46_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE46_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE46_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE46_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE46_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE46_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE46_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE46_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE46_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE46_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE46_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE46_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE46_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE46_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE46_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE46_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE46_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE46_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE46_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE46_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE46_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE46_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE46_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE46_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE46_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE46_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE47_LOCK0
// Description : Lock configuration LSBs for page 47 (rows 0xbc0 through 0xbff).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE47_LOCK0_ROW _u(0x00000fde)
#define OTP_DATA_PAGE47_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE47_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE47_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE47_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE47_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE47_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE47_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE47_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE47_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE47_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE47_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE47_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE47_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE47_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE47_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE47_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE47_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE47_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE47_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE47_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE47_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE47_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE47_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE47_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE47_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE47_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE47_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE47_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE47_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE47_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE47_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE47_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE47_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE47_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE47_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE47_LOCK1
// Description : Lock configuration MSBs for page 47 (rows 0xbc0 through 0xbff).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE47_LOCK1_ROW _u(0x00000fdf)
#define OTP_DATA_PAGE47_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE47_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE47_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE47_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE47_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE47_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE47_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE47_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE47_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE47_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE47_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE47_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE47_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE47_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE47_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE47_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE47_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE47_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE47_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE47_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE47_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE47_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE47_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE47_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE47_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE47_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE47_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE47_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE47_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE47_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE47_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE47_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE47_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE47_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE47_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE47_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE47_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE47_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE47_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE47_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE47_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE47_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE47_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE47_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE47_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE48_LOCK0
// Description : Lock configuration LSBs for page 48 (rows 0xc00 through 0xc3f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE48_LOCK0_ROW _u(0x00000fe0)
#define OTP_DATA_PAGE48_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE48_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE48_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE48_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE48_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE48_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE48_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE48_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE48_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE48_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE48_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE48_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE48_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE48_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE48_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE48_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE48_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE48_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE48_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE48_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE48_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE48_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE48_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE48_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE48_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE48_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE48_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE48_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE48_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE48_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE48_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE48_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE48_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE48_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE48_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE48_LOCK1
// Description : Lock configuration MSBs for page 48 (rows 0xc00 through 0xc3f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE48_LOCK1_ROW _u(0x00000fe1)
#define OTP_DATA_PAGE48_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE48_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE48_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE48_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE48_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE48_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE48_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE48_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE48_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE48_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE48_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE48_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE48_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE48_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE48_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE48_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE48_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE48_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE48_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE48_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE48_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE48_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE48_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE48_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE48_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE48_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE48_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE48_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE48_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE48_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE48_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE48_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE48_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE48_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE48_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE48_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE48_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE48_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE48_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE48_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE48_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE48_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE48_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE48_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE48_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE49_LOCK0
// Description : Lock configuration LSBs for page 49 (rows 0xc40 through 0xc7f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE49_LOCK0_ROW _u(0x00000fe2)
#define OTP_DATA_PAGE49_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE49_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE49_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE49_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE49_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE49_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE49_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE49_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE49_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE49_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE49_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE49_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE49_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE49_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE49_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE49_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE49_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE49_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE49_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE49_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE49_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE49_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE49_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE49_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE49_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE49_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE49_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE49_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE49_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE49_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE49_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE49_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE49_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE49_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE49_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE49_LOCK1
// Description : Lock configuration MSBs for page 49 (rows 0xc40 through 0xc7f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE49_LOCK1_ROW _u(0x00000fe3)
#define OTP_DATA_PAGE49_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE49_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE49_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE49_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE49_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE49_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE49_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE49_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE49_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE49_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE49_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE49_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE49_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE49_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE49_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE49_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE49_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE49_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE49_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE49_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE49_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE49_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE49_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE49_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE49_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE49_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE49_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE49_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE49_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE49_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE49_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE49_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE49_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE49_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE49_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE49_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE49_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE49_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE49_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE49_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE49_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE49_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE49_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE49_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE49_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE50_LOCK0
// Description : Lock configuration LSBs for page 50 (rows 0xc80 through 0xcbf).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE50_LOCK0_ROW _u(0x00000fe4)
#define OTP_DATA_PAGE50_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE50_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE50_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE50_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE50_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE50_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE50_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE50_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE50_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE50_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE50_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE50_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE50_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE50_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE50_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE50_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE50_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE50_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE50_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE50_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE50_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE50_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE50_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE50_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE50_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE50_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE50_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE50_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE50_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE50_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE50_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE50_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE50_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE50_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE50_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE50_LOCK1
// Description : Lock configuration MSBs for page 50 (rows 0xc80 through 0xcbf).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE50_LOCK1_ROW _u(0x00000fe5)
#define OTP_DATA_PAGE50_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE50_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE50_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE50_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE50_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE50_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE50_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE50_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE50_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE50_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE50_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE50_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE50_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE50_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE50_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE50_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE50_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE50_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE50_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE50_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE50_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE50_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE50_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE50_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE50_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE50_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE50_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE50_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE50_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE50_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE50_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE50_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE50_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE50_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE50_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE50_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE50_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE50_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE50_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE50_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE50_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE50_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE50_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE50_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE50_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE51_LOCK0
// Description : Lock configuration LSBs for page 51 (rows 0xcc0 through 0xcff).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE51_LOCK0_ROW _u(0x00000fe6)
#define OTP_DATA_PAGE51_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE51_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE51_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE51_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE51_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE51_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE51_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE51_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE51_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE51_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE51_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE51_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE51_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE51_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE51_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE51_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE51_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE51_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE51_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE51_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE51_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE51_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE51_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE51_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE51_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE51_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE51_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE51_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE51_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE51_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE51_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE51_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE51_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE51_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE51_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE51_LOCK1
// Description : Lock configuration MSBs for page 51 (rows 0xcc0 through 0xcff).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE51_LOCK1_ROW _u(0x00000fe7)
#define OTP_DATA_PAGE51_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE51_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE51_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE51_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE51_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE51_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE51_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE51_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE51_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE51_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE51_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE51_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE51_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE51_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE51_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE51_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE51_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE51_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE51_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE51_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE51_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE51_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE51_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE51_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE51_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE51_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE51_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE51_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE51_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE51_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE51_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE51_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE51_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE51_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE51_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE51_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE51_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE51_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE51_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE51_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE51_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE51_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE51_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE51_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE51_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE52_LOCK0
// Description : Lock configuration LSBs for page 52 (rows 0xd00 through 0xd3f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE52_LOCK0_ROW _u(0x00000fe8)
#define OTP_DATA_PAGE52_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE52_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE52_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE52_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE52_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE52_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE52_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE52_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE52_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE52_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE52_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE52_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE52_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE52_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE52_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE52_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE52_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE52_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE52_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE52_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE52_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE52_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE52_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE52_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE52_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE52_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE52_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE52_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE52_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE52_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE52_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE52_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE52_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE52_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE52_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE52_LOCK1
// Description : Lock configuration MSBs for page 52 (rows 0xd00 through 0xd3f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE52_LOCK1_ROW _u(0x00000fe9)
#define OTP_DATA_PAGE52_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE52_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE52_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE52_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE52_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE52_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE52_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE52_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE52_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE52_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE52_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE52_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE52_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE52_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE52_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE52_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE52_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE52_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE52_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE52_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE52_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE52_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE52_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE52_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE52_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE52_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE52_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE52_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE52_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE52_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE52_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE52_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE52_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE52_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE52_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE52_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE52_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE52_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE52_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE52_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE52_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE52_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE52_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE52_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE52_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE53_LOCK0
// Description : Lock configuration LSBs for page 53 (rows 0xd40 through 0xd7f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE53_LOCK0_ROW _u(0x00000fea)
#define OTP_DATA_PAGE53_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE53_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE53_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE53_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE53_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE53_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE53_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE53_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE53_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE53_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE53_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE53_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE53_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE53_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE53_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE53_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE53_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE53_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE53_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE53_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE53_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE53_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE53_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE53_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE53_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE53_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE53_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE53_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE53_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE53_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE53_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE53_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE53_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE53_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE53_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE53_LOCK1
// Description : Lock configuration MSBs for page 53 (rows 0xd40 through 0xd7f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE53_LOCK1_ROW _u(0x00000feb)
#define OTP_DATA_PAGE53_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE53_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE53_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE53_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE53_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE53_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE53_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE53_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE53_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE53_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE53_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE53_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE53_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE53_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE53_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE53_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE53_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE53_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE53_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE53_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE53_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE53_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE53_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE53_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE53_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE53_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE53_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE53_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE53_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE53_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE53_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE53_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE53_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE53_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE53_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE53_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE53_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE53_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE53_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE53_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE53_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE53_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE53_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE53_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE53_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE54_LOCK0
// Description : Lock configuration LSBs for page 54 (rows 0xd80 through 0xdbf).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE54_LOCK0_ROW _u(0x00000fec)
#define OTP_DATA_PAGE54_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE54_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE54_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE54_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE54_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE54_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE54_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE54_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE54_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE54_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE54_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE54_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE54_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE54_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE54_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE54_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE54_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE54_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE54_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE54_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE54_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE54_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE54_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE54_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE54_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE54_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE54_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE54_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE54_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE54_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE54_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE54_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE54_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE54_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE54_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE54_LOCK1
// Description : Lock configuration MSBs for page 54 (rows 0xd80 through 0xdbf).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE54_LOCK1_ROW _u(0x00000fed)
#define OTP_DATA_PAGE54_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE54_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE54_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE54_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE54_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE54_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE54_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE54_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE54_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE54_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE54_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE54_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE54_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE54_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE54_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE54_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE54_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE54_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE54_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE54_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE54_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE54_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE54_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE54_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE54_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE54_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE54_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE54_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE54_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE54_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE54_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE54_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE54_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE54_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE54_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE54_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE54_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE54_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE54_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE54_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE54_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE54_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE54_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE54_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE54_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE55_LOCK0
// Description : Lock configuration LSBs for page 55 (rows 0xdc0 through 0xdff).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE55_LOCK0_ROW _u(0x00000fee)
#define OTP_DATA_PAGE55_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE55_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE55_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE55_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE55_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE55_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE55_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE55_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE55_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE55_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE55_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE55_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE55_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE55_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE55_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE55_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE55_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE55_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE55_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE55_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE55_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE55_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE55_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE55_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE55_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE55_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE55_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE55_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE55_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE55_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE55_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE55_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE55_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE55_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE55_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE55_LOCK1
// Description : Lock configuration MSBs for page 55 (rows 0xdc0 through 0xdff).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE55_LOCK1_ROW _u(0x00000fef)
#define OTP_DATA_PAGE55_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE55_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE55_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE55_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE55_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE55_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE55_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE55_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE55_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE55_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE55_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE55_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE55_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE55_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE55_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE55_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE55_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE55_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE55_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE55_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE55_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE55_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE55_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE55_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE55_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE55_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE55_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE55_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE55_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE55_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE55_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE55_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE55_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE55_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE55_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE55_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE55_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE55_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE55_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE55_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE55_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE55_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE55_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE55_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE55_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE56_LOCK0
// Description : Lock configuration LSBs for page 56 (rows 0xe00 through 0xe3f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE56_LOCK0_ROW _u(0x00000ff0)
#define OTP_DATA_PAGE56_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE56_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE56_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE56_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE56_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE56_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE56_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE56_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE56_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE56_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE56_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE56_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE56_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE56_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE56_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE56_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE56_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE56_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE56_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE56_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE56_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE56_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE56_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE56_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE56_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE56_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE56_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE56_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE56_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE56_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE56_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE56_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE56_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE56_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE56_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE56_LOCK1
// Description : Lock configuration MSBs for page 56 (rows 0xe00 through 0xe3f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE56_LOCK1_ROW _u(0x00000ff1)
#define OTP_DATA_PAGE56_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE56_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE56_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE56_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE56_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE56_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE56_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE56_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE56_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE56_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE56_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE56_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE56_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE56_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE56_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE56_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE56_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE56_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE56_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE56_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE56_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE56_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE56_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE56_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE56_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE56_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE56_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE56_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE56_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE56_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE56_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE56_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE56_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE56_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE56_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE56_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE56_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE56_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE56_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE56_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE56_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE56_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE56_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE56_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE56_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE57_LOCK0
// Description : Lock configuration LSBs for page 57 (rows 0xe40 through 0xe7f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE57_LOCK0_ROW _u(0x00000ff2)
#define OTP_DATA_PAGE57_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE57_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE57_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE57_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE57_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE57_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE57_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE57_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE57_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE57_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE57_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE57_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE57_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE57_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE57_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE57_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE57_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE57_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE57_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE57_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE57_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE57_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE57_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE57_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE57_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE57_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE57_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE57_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE57_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE57_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE57_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE57_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE57_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE57_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE57_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE57_LOCK1
// Description : Lock configuration MSBs for page 57 (rows 0xe40 through 0xe7f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE57_LOCK1_ROW _u(0x00000ff3)
#define OTP_DATA_PAGE57_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE57_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE57_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE57_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE57_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE57_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE57_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE57_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE57_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE57_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE57_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE57_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE57_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE57_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE57_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE57_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE57_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE57_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE57_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE57_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE57_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE57_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE57_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE57_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE57_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE57_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE57_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE57_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE57_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE57_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE57_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE57_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE57_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE57_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE57_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE57_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE57_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE57_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE57_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE57_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE57_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE57_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE57_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE57_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE57_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE58_LOCK0
// Description : Lock configuration LSBs for page 58 (rows 0xe80 through 0xebf).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE58_LOCK0_ROW _u(0x00000ff4)
#define OTP_DATA_PAGE58_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE58_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE58_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE58_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE58_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE58_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE58_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE58_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE58_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE58_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE58_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE58_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE58_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE58_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE58_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE58_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE58_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE58_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE58_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE58_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE58_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE58_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE58_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE58_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE58_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE58_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE58_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE58_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE58_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE58_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE58_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE58_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE58_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE58_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE58_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE58_LOCK1
// Description : Lock configuration MSBs for page 58 (rows 0xe80 through 0xebf).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE58_LOCK1_ROW _u(0x00000ff5)
#define OTP_DATA_PAGE58_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE58_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE58_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE58_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE58_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE58_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE58_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE58_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE58_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE58_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE58_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE58_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE58_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE58_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE58_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE58_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE58_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE58_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE58_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE58_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE58_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE58_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE58_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE58_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE58_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE58_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE58_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE58_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE58_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE58_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE58_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE58_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE58_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE58_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE58_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE58_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE58_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE58_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE58_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE58_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE58_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE58_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE58_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE58_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE58_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE59_LOCK0
// Description : Lock configuration LSBs for page 59 (rows 0xec0 through 0xeff).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE59_LOCK0_ROW _u(0x00000ff6)
#define OTP_DATA_PAGE59_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE59_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE59_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE59_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE59_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE59_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE59_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE59_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE59_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE59_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE59_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE59_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE59_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE59_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE59_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE59_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE59_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE59_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE59_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE59_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE59_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE59_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE59_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE59_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE59_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE59_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE59_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE59_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE59_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE59_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE59_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE59_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE59_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE59_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE59_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE59_LOCK1
// Description : Lock configuration MSBs for page 59 (rows 0xec0 through 0xeff).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE59_LOCK1_ROW _u(0x00000ff7)
#define OTP_DATA_PAGE59_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE59_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE59_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE59_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE59_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE59_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE59_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE59_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE59_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE59_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE59_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE59_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE59_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE59_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE59_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE59_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE59_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE59_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE59_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE59_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE59_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE59_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE59_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE59_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE59_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE59_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE59_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE59_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE59_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE59_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE59_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE59_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE59_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE59_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE59_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE59_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE59_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE59_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE59_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE59_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE59_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE59_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE59_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE59_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE59_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE60_LOCK0
// Description : Lock configuration LSBs for page 60 (rows 0xf00 through 0xf3f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE60_LOCK0_ROW _u(0x00000ff8)
#define OTP_DATA_PAGE60_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE60_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE60_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE60_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE60_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE60_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE60_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE60_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE60_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE60_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE60_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE60_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE60_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE60_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE60_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE60_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE60_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE60_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE60_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE60_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE60_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE60_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE60_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE60_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE60_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE60_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE60_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE60_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE60_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE60_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE60_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE60_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE60_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE60_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE60_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE60_LOCK1
// Description : Lock configuration MSBs for page 60 (rows 0xf00 through 0xf3f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE60_LOCK1_ROW _u(0x00000ff9)
#define OTP_DATA_PAGE60_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE60_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE60_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE60_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE60_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE60_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE60_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE60_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE60_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE60_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE60_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE60_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE60_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE60_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE60_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE60_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE60_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE60_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE60_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE60_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE60_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE60_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE60_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE60_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE60_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE60_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE60_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE60_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE60_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE60_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE60_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE60_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE60_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE60_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE60_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE60_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE60_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE60_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE60_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE60_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE60_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE60_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE60_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE60_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE60_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE61_LOCK0
// Description : Lock configuration LSBs for page 61 (rows 0xf40 through 0xf7f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE61_LOCK0_ROW _u(0x00000ffa)
#define OTP_DATA_PAGE61_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE61_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE61_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE61_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE61_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE61_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE61_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE61_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE61_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE61_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE61_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE61_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE61_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE61_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE61_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE61_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE61_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE61_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE61_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE61_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE61_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE61_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE61_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE61_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE61_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE61_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE61_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE61_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE61_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE61_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE61_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE61_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE61_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE61_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE61_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE61_LOCK1
// Description : Lock configuration MSBs for page 61 (rows 0xf40 through 0xf7f).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE61_LOCK1_ROW _u(0x00000ffb)
#define OTP_DATA_PAGE61_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE61_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE61_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE61_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE61_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE61_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE61_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE61_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE61_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE61_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE61_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE61_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE61_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE61_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE61_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE61_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE61_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE61_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE61_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE61_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE61_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE61_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE61_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE61_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE61_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE61_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE61_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE61_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE61_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE61_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE61_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE61_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE61_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE61_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE61_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE61_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE61_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE61_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE61_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE61_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE61_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE61_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE61_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE61_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE61_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE62_LOCK0
// Description : Lock configuration LSBs for page 62 (rows 0xf80 through 0xfbf).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE62_LOCK0_ROW _u(0x00000ffc)
#define OTP_DATA_PAGE62_LOCK0_BITS   _u(0x00ffff7f)
#define OTP_DATA_PAGE62_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE62_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE62_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE62_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE62_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE62_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE62_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE62_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE62_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE62_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE62_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE62_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE62_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE62_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE62_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE62_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE62_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE62_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE62_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE62_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE62_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE62_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE62_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE62_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE62_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE62_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE62_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE62_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE62_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE62_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE62_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE62_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE62_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE62_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE62_LOCK1
// Description : Lock configuration MSBs for page 62 (rows 0xf80 through 0xfbf).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE62_LOCK1_ROW _u(0x00000ffd)
#define OTP_DATA_PAGE62_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE62_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE62_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE62_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE62_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE62_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE62_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE62_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE62_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE62_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE62_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE62_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE62_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE62_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE62_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE62_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE62_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE62_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE62_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE62_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE62_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE62_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE62_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE62_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE62_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE62_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE62_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE62_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE62_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE62_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE62_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE62_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE62_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE62_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE62_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE62_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE62_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE62_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE62_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE62_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE62_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE62_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE62_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE62_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE62_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
// Register    : OTP_DATA_PAGE63_LOCK0
// Description : Lock configuration LSBs for page 63 (rows 0xfc0 through 0xfff).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE63_LOCK0_ROW _u(0x00000ffe)
#define OTP_DATA_PAGE63_LOCK0_BITS   _u(0x00ffffff)
#define OTP_DATA_PAGE63_LOCK0_RESET  _u(0x00000000)
#define OTP_DATA_PAGE63_LOCK0_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE63_LOCK0_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE63_LOCK0_R2_RESET  "-"
#define OTP_DATA_PAGE63_LOCK0_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE63_LOCK0_R2_MSB    _u(23)
#define OTP_DATA_PAGE63_LOCK0_R2_LSB    _u(16)
#define OTP_DATA_PAGE63_LOCK0_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE63_LOCK0_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE63_LOCK0_R1_RESET  "-"
#define OTP_DATA_PAGE63_LOCK0_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE63_LOCK0_R1_MSB    _u(15)
#define OTP_DATA_PAGE63_LOCK0_R1_LSB    _u(8)
#define OTP_DATA_PAGE63_LOCK0_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE63_LOCK0_RMA
// Description : Decommission for RMA of a suspected faulty device. This re-
//               enables the factory test JTAG interface, and makes pages 3
//               through 61 of the OTP permanently inaccessible.
#define OTP_DATA_PAGE63_LOCK0_RMA_RESET  "-"
#define OTP_DATA_PAGE63_LOCK0_RMA_BITS   _u(0x00000080)
#define OTP_DATA_PAGE63_LOCK0_RMA_MSB    _u(7)
#define OTP_DATA_PAGE63_LOCK0_RMA_LSB    _u(7)
#define OTP_DATA_PAGE63_LOCK0_RMA_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE63_LOCK0_NO_KEY_STATE
// Description : State when at least one key is registered for this page and no
//               matching key has been entered.
//               0x0 -> read_only
//               0x1 -> inaccessible
#define OTP_DATA_PAGE63_LOCK0_NO_KEY_STATE_RESET  "-"
#define OTP_DATA_PAGE63_LOCK0_NO_KEY_STATE_BITS   _u(0x00000040)
#define OTP_DATA_PAGE63_LOCK0_NO_KEY_STATE_MSB    _u(6)
#define OTP_DATA_PAGE63_LOCK0_NO_KEY_STATE_LSB    _u(6)
#define OTP_DATA_PAGE63_LOCK0_NO_KEY_STATE_ACCESS "RO"
#define OTP_DATA_PAGE63_LOCK0_NO_KEY_STATE_VALUE_READ_ONLY _u(0x0)
#define OTP_DATA_PAGE63_LOCK0_NO_KEY_STATE_VALUE_INACCESSIBLE _u(0x1)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE63_LOCK0_KEY_R
// Description : Index 1-6 of a hardware key which must be entered to grant read
//               access, or 0 if no such key is required.
#define OTP_DATA_PAGE63_LOCK0_KEY_R_RESET  "-"
#define OTP_DATA_PAGE63_LOCK0_KEY_R_BITS   _u(0x00000038)
#define OTP_DATA_PAGE63_LOCK0_KEY_R_MSB    _u(5)
#define OTP_DATA_PAGE63_LOCK0_KEY_R_LSB    _u(3)
#define OTP_DATA_PAGE63_LOCK0_KEY_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE63_LOCK0_KEY_W
// Description : Index 1-6 of a hardware key which must be entered to grant
//               write access, or 0 if no such key is required.
#define OTP_DATA_PAGE63_LOCK0_KEY_W_RESET  "-"
#define OTP_DATA_PAGE63_LOCK0_KEY_W_BITS   _u(0x00000007)
#define OTP_DATA_PAGE63_LOCK0_KEY_W_MSB    _u(2)
#define OTP_DATA_PAGE63_LOCK0_KEY_W_LSB    _u(0)
#define OTP_DATA_PAGE63_LOCK0_KEY_W_ACCESS "RO"
// =============================================================================
// Register    : OTP_DATA_PAGE63_LOCK1
// Description : Lock configuration MSBs for page 63 (rows 0xfc0 through 0xfff).
//               Locks are stored with 3-way majority vote encoding, so that
//               bits can be set independently.
//
//               This OTP location is always readable, and is write-protected by
//               its own permissions.
#define OTP_DATA_PAGE63_LOCK1_ROW _u(0x00000fff)
#define OTP_DATA_PAGE63_LOCK1_BITS   _u(0x00ffff3f)
#define OTP_DATA_PAGE63_LOCK1_RESET  _u(0x00000000)
#define OTP_DATA_PAGE63_LOCK1_WIDTH  _u(24)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE63_LOCK1_R2
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE63_LOCK1_R2_RESET  "-"
#define OTP_DATA_PAGE63_LOCK1_R2_BITS   _u(0x00ff0000)
#define OTP_DATA_PAGE63_LOCK1_R2_MSB    _u(23)
#define OTP_DATA_PAGE63_LOCK1_R2_LSB    _u(16)
#define OTP_DATA_PAGE63_LOCK1_R2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE63_LOCK1_R1
// Description : Redundant copy of bits 7:0
#define OTP_DATA_PAGE63_LOCK1_R1_RESET  "-"
#define OTP_DATA_PAGE63_LOCK1_R1_BITS   _u(0x0000ff00)
#define OTP_DATA_PAGE63_LOCK1_R1_MSB    _u(15)
#define OTP_DATA_PAGE63_LOCK1_R1_LSB    _u(8)
#define OTP_DATA_PAGE63_LOCK1_R1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE63_LOCK1_LOCK_BL
// Description : Dummy lock bits reserved for bootloaders (including the RP2350
//               USB bootloader) to store their own OTP access permissions. No
//               hardware effect, and no corresponding SW_LOCKx registers.
//               0x0 -> Bootloader permits user reads and writes to this page
//               0x1 -> Bootloader permits user reads of this page
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE
//               0x3 -> Bootloader does not permit user access to this page
#define OTP_DATA_PAGE63_LOCK1_LOCK_BL_RESET  "-"
#define OTP_DATA_PAGE63_LOCK1_LOCK_BL_BITS   _u(0x00000030)
#define OTP_DATA_PAGE63_LOCK1_LOCK_BL_MSB    _u(5)
#define OTP_DATA_PAGE63_LOCK1_LOCK_BL_LSB    _u(4)
#define OTP_DATA_PAGE63_LOCK1_LOCK_BL_ACCESS "RO"
#define OTP_DATA_PAGE63_LOCK1_LOCK_BL_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE63_LOCK1_LOCK_BL_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE63_LOCK1_LOCK_BL_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE63_LOCK1_LOCK_BL_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE63_LOCK1_LOCK_NS
// Description : Lock state for Non-secure accesses to this page. Thermometer-
//               coded, so lock state can be advanced permanently from any state
//               to any less-permissive state by programming OTP. Software can
//               also advance the lock state temporarily (until next OTP reset)
//               using the SW_LOCKx registers.
//
//               Note that READ_WRITE and READ_ONLY are equivalent in hardware,
//               as the SBPI programming interface is not accessible to Non-
//               secure software. However, Secure software may check these bits
//               to apply write permissions to a Non-secure OTP programming API.
//               0x0 -> Page can be read by Non-secure software, and Secure software may permit Non-secure writes.
//               0x1 -> Page can be read by Non-secure software
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Non-secure software.
#define OTP_DATA_PAGE63_LOCK1_LOCK_NS_RESET  "-"
#define OTP_DATA_PAGE63_LOCK1_LOCK_NS_BITS   _u(0x0000000c)
#define OTP_DATA_PAGE63_LOCK1_LOCK_NS_MSB    _u(3)
#define OTP_DATA_PAGE63_LOCK1_LOCK_NS_LSB    _u(2)
#define OTP_DATA_PAGE63_LOCK1_LOCK_NS_ACCESS "RO"
#define OTP_DATA_PAGE63_LOCK1_LOCK_NS_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE63_LOCK1_LOCK_NS_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE63_LOCK1_LOCK_NS_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE63_LOCK1_LOCK_NS_VALUE_INACCESSIBLE _u(0x3)
// -----------------------------------------------------------------------------
// Field       : OTP_DATA_PAGE63_LOCK1_LOCK_S
// Description : Lock state for Secure accesses to this page. Thermometer-coded,
//               so lock state can be advanced permanently from any state to any
//               less-permissive state by programming OTP. Software can also
//               advance the lock state temporarily (until next OTP reset) using
//               the SW_LOCKx registers.
//               0x0 -> Page is fully accessible by Secure software.
//               0x1 -> Page can be read by Secure software, but can not be written.
//               0x2 -> Do not use. Behaves the same as INACCESSIBLE.
//               0x3 -> Page can not be accessed by Secure software.
#define OTP_DATA_PAGE63_LOCK1_LOCK_S_RESET  "-"
#define OTP_DATA_PAGE63_LOCK1_LOCK_S_BITS   _u(0x00000003)
#define OTP_DATA_PAGE63_LOCK1_LOCK_S_MSB    _u(1)
#define OTP_DATA_PAGE63_LOCK1_LOCK_S_LSB    _u(0)
#define OTP_DATA_PAGE63_LOCK1_LOCK_S_ACCESS "RO"
#define OTP_DATA_PAGE63_LOCK1_LOCK_S_VALUE_READ_WRITE _u(0x0)
#define OTP_DATA_PAGE63_LOCK1_LOCK_S_VALUE_READ_ONLY _u(0x1)
#define OTP_DATA_PAGE63_LOCK1_LOCK_S_VALUE_RESERVED _u(0x2)
#define OTP_DATA_PAGE63_LOCK1_LOCK_S_VALUE_INACCESSIBLE _u(0x3)
// =============================================================================
#endif // _HARDWARE_REGS_OTP_DATA_H

