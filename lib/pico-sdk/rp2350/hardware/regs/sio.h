// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
// =============================================================================
// Register block : SIO
// Version        : 1
// Bus type       : apb
// Description    : Single-cycle IO block
//                  Provides core-local and inter-core hardware for the two
//                  processors, with single-cycle access.
// =============================================================================
#ifndef _HARDWARE_REGS_SIO_H
#define _HARDWARE_REGS_SIO_H
// =============================================================================
// Register    : SIO_CPUID
// Description : Processor core identifier
//               Value is 0 when read from processor core 0, and 1 when read
//               from processor core 1.
#define SIO_CPUID_OFFSET _u(0x00000000)
#define SIO_CPUID_BITS   _u(0xffffffff)
#define SIO_CPUID_RESET  "-"
#define SIO_CPUID_MSB    _u(31)
#define SIO_CPUID_LSB    _u(0)
#define SIO_CPUID_ACCESS "RO"
// =============================================================================
// Register    : SIO_GPIO_IN
// Description : Input value for GPIO0...31.
//
//               In the Non-secure SIO, Secure-only GPIOs (as per ACCESSCTRL)
//               appear as zero.
#define SIO_GPIO_IN_OFFSET _u(0x00000004)
#define SIO_GPIO_IN_BITS   _u(0xffffffff)
#define SIO_GPIO_IN_RESET  _u(0x00000000)
#define SIO_GPIO_IN_MSB    _u(31)
#define SIO_GPIO_IN_LSB    _u(0)
#define SIO_GPIO_IN_ACCESS "RO"
// =============================================================================
// Register    : SIO_GPIO_HI_IN
// Description : Input value on GPIO32...47, QSPI IOs and USB pins
//
//               In the Non-secure SIO, Secure-only GPIOs (as per ACCESSCTRL)
//               appear as zero.
#define SIO_GPIO_HI_IN_OFFSET _u(0x00000008)
#define SIO_GPIO_HI_IN_BITS   _u(0xff00ffff)
#define SIO_GPIO_HI_IN_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : SIO_GPIO_HI_IN_QSPI_SD
// Description : Input value on QSPI SD0 (MOSI), SD1 (MISO), SD2 and SD3 pins
#define SIO_GPIO_HI_IN_QSPI_SD_RESET  _u(0x0)
#define SIO_GPIO_HI_IN_QSPI_SD_BITS   _u(0xf0000000)
#define SIO_GPIO_HI_IN_QSPI_SD_MSB    _u(31)
#define SIO_GPIO_HI_IN_QSPI_SD_LSB    _u(28)
#define SIO_GPIO_HI_IN_QSPI_SD_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : SIO_GPIO_HI_IN_QSPI_CSN
// Description : Input value on QSPI CSn pin
#define SIO_GPIO_HI_IN_QSPI_CSN_RESET  _u(0x0)
#define SIO_GPIO_HI_IN_QSPI_CSN_BITS   _u(0x08000000)
#define SIO_GPIO_HI_IN_QSPI_CSN_MSB    _u(27)
#define SIO_GPIO_HI_IN_QSPI_CSN_LSB    _u(27)
#define SIO_GPIO_HI_IN_QSPI_CSN_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : SIO_GPIO_HI_IN_QSPI_SCK
// Description : Input value on QSPI SCK pin
#define SIO_GPIO_HI_IN_QSPI_SCK_RESET  _u(0x0)
#define SIO_GPIO_HI_IN_QSPI_SCK_BITS   _u(0x04000000)
#define SIO_GPIO_HI_IN_QSPI_SCK_MSB    _u(26)
#define SIO_GPIO_HI_IN_QSPI_SCK_LSB    _u(26)
#define SIO_GPIO_HI_IN_QSPI_SCK_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : SIO_GPIO_HI_IN_USB_DM
// Description : Input value on USB D- pin
#define SIO_GPIO_HI_IN_USB_DM_RESET  _u(0x0)
#define SIO_GPIO_HI_IN_USB_DM_BITS   _u(0x02000000)
#define SIO_GPIO_HI_IN_USB_DM_MSB    _u(25)
#define SIO_GPIO_HI_IN_USB_DM_LSB    _u(25)
#define SIO_GPIO_HI_IN_USB_DM_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : SIO_GPIO_HI_IN_USB_DP
// Description : Input value on USB D+ pin
#define SIO_GPIO_HI_IN_USB_DP_RESET  _u(0x0)
#define SIO_GPIO_HI_IN_USB_DP_BITS   _u(0x01000000)
#define SIO_GPIO_HI_IN_USB_DP_MSB    _u(24)
#define SIO_GPIO_HI_IN_USB_DP_LSB    _u(24)
#define SIO_GPIO_HI_IN_USB_DP_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : SIO_GPIO_HI_IN_GPIO
// Description : Input value on GPIO32...47
#define SIO_GPIO_HI_IN_GPIO_RESET  _u(0x0000)
#define SIO_GPIO_HI_IN_GPIO_BITS   _u(0x0000ffff)
#define SIO_GPIO_HI_IN_GPIO_MSB    _u(15)
#define SIO_GPIO_HI_IN_GPIO_LSB    _u(0)
#define SIO_GPIO_HI_IN_GPIO_ACCESS "RO"
// =============================================================================
// Register    : SIO_GPIO_OUT
// Description : GPIO0...31 output value
//               Set output level (1/0 -> high/low) for GPIO0...31. Reading back
//               gives the last value written, NOT the input value from the
//               pins.
//
//               If core 0 and core 1 both write to GPIO_OUT simultaneously (or
//               to a SET/CLR/XOR alias), the result is as though the write from
//               core 0 took place first, and the write from core 1 was then
//               applied to that intermediate result.
//
//               In the Non-secure SIO, Secure-only GPIOs (as per ACCESSCTRL)
//               ignore writes, and their output status reads back as zero. This
//               is also true for SET/CLR/XOR aliases of this register.
#define SIO_GPIO_OUT_OFFSET _u(0x00000010)
#define SIO_GPIO_OUT_BITS   _u(0xffffffff)
#define SIO_GPIO_OUT_RESET  _u(0x00000000)
#define SIO_GPIO_OUT_MSB    _u(31)
#define SIO_GPIO_OUT_LSB    _u(0)
#define SIO_GPIO_OUT_ACCESS "RW"
// =============================================================================
// Register    : SIO_GPIO_HI_OUT
// Description : Output value for GPIO32...47, QSPI IOs and USB pins.
//
//               Write to set output level (1/0 -> high/low). Reading back gives
//               the last value written, NOT the input value from the pins. If
//               core 0 and core 1 both write to GPIO_HI_OUT simultaneously (or
//               to a SET/CLR/XOR alias), the result is as though the write from
//               core 0 took place first, and the write from core 1 was then
//               applied to that intermediate result.
//
//               In the Non-secure SIO, Secure-only GPIOs (as per ACCESSCTRL)
//               ignore writes, and their output status reads back as zero. This
//               is also true for SET/CLR/XOR aliases of this register.
#define SIO_GPIO_HI_OUT_OFFSET _u(0x00000014)
#define SIO_GPIO_HI_OUT_BITS   _u(0xff00ffff)
#define SIO_GPIO_HI_OUT_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : SIO_GPIO_HI_OUT_QSPI_SD
// Description : Output value for QSPI SD0 (MOSI), SD1 (MISO), SD2 and SD3 pins
#define SIO_GPIO_HI_OUT_QSPI_SD_RESET  _u(0x0)
#define SIO_GPIO_HI_OUT_QSPI_SD_BITS   _u(0xf0000000)
#define SIO_GPIO_HI_OUT_QSPI_SD_MSB    _u(31)
#define SIO_GPIO_HI_OUT_QSPI_SD_LSB    _u(28)
#define SIO_GPIO_HI_OUT_QSPI_SD_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SIO_GPIO_HI_OUT_QSPI_CSN
// Description : Output value for QSPI CSn pin
#define SIO_GPIO_HI_OUT_QSPI_CSN_RESET  _u(0x0)
#define SIO_GPIO_HI_OUT_QSPI_CSN_BITS   _u(0x08000000)
#define SIO_GPIO_HI_OUT_QSPI_CSN_MSB    _u(27)
#define SIO_GPIO_HI_OUT_QSPI_CSN_LSB    _u(27)
#define SIO_GPIO_HI_OUT_QSPI_CSN_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SIO_GPIO_HI_OUT_QSPI_SCK
// Description : Output value for QSPI SCK pin
#define SIO_GPIO_HI_OUT_QSPI_SCK_RESET  _u(0x0)
#define SIO_GPIO_HI_OUT_QSPI_SCK_BITS   _u(0x04000000)
#define SIO_GPIO_HI_OUT_QSPI_SCK_MSB    _u(26)
#define SIO_GPIO_HI_OUT_QSPI_SCK_LSB    _u(26)
#define SIO_GPIO_HI_OUT_QSPI_SCK_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SIO_GPIO_HI_OUT_USB_DM
// Description : Output value for USB D- pin
#define SIO_GPIO_HI_OUT_USB_DM_RESET  _u(0x0)
#define SIO_GPIO_HI_OUT_USB_DM_BITS   _u(0x02000000)
#define SIO_GPIO_HI_OUT_USB_DM_MSB    _u(25)
#define SIO_GPIO_HI_OUT_USB_DM_LSB    _u(25)
#define SIO_GPIO_HI_OUT_USB_DM_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SIO_GPIO_HI_OUT_USB_DP
// Description : Output value for USB D+ pin
#define SIO_GPIO_HI_OUT_USB_DP_RESET  _u(0x0)
#define SIO_GPIO_HI_OUT_USB_DP_BITS   _u(0x01000000)
#define SIO_GPIO_HI_OUT_USB_DP_MSB    _u(24)
#define SIO_GPIO_HI_OUT_USB_DP_LSB    _u(24)
#define SIO_GPIO_HI_OUT_USB_DP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SIO_GPIO_HI_OUT_GPIO
// Description : Output value for GPIO32...47
#define SIO_GPIO_HI_OUT_GPIO_RESET  _u(0x0000)
#define SIO_GPIO_HI_OUT_GPIO_BITS   _u(0x0000ffff)
#define SIO_GPIO_HI_OUT_GPIO_MSB    _u(15)
#define SIO_GPIO_HI_OUT_GPIO_LSB    _u(0)
#define SIO_GPIO_HI_OUT_GPIO_ACCESS "RW"
// =============================================================================
// Register    : SIO_GPIO_OUT_SET
// Description : GPIO0...31 output value set
//               Perform an atomic bit-set on GPIO_OUT, i.e. `GPIO_OUT |= wdata`
#define SIO_GPIO_OUT_SET_OFFSET _u(0x00000018)
#define SIO_GPIO_OUT_SET_BITS   _u(0xffffffff)
#define SIO_GPIO_OUT_SET_RESET  _u(0x00000000)
#define SIO_GPIO_OUT_SET_MSB    _u(31)
#define SIO_GPIO_OUT_SET_LSB    _u(0)
#define SIO_GPIO_OUT_SET_ACCESS "WO"
// =============================================================================
// Register    : SIO_GPIO_HI_OUT_SET
// Description : Output value set for GPIO32..47, QSPI IOs and USB pins.
//               Perform an atomic bit-set on GPIO_HI_OUT, i.e. `GPIO_HI_OUT |=
//               wdata`
#define SIO_GPIO_HI_OUT_SET_OFFSET _u(0x0000001c)
#define SIO_GPIO_HI_OUT_SET_BITS   _u(0xff00ffff)
#define SIO_GPIO_HI_OUT_SET_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : SIO_GPIO_HI_OUT_SET_QSPI_SD
#define SIO_GPIO_HI_OUT_SET_QSPI_SD_RESET  _u(0x0)
#define SIO_GPIO_HI_OUT_SET_QSPI_SD_BITS   _u(0xf0000000)
#define SIO_GPIO_HI_OUT_SET_QSPI_SD_MSB    _u(31)
#define SIO_GPIO_HI_OUT_SET_QSPI_SD_LSB    _u(28)
#define SIO_GPIO_HI_OUT_SET_QSPI_SD_ACCESS "WO"
// -----------------------------------------------------------------------------
// Field       : SIO_GPIO_HI_OUT_SET_QSPI_CSN
#define SIO_GPIO_HI_OUT_SET_QSPI_CSN_RESET  _u(0x0)
#define SIO_GPIO_HI_OUT_SET_QSPI_CSN_BITS   _u(0x08000000)
#define SIO_GPIO_HI_OUT_SET_QSPI_CSN_MSB    _u(27)
#define SIO_GPIO_HI_OUT_SET_QSPI_CSN_LSB    _u(27)
#define SIO_GPIO_HI_OUT_SET_QSPI_CSN_ACCESS "WO"
// -----------------------------------------------------------------------------
// Field       : SIO_GPIO_HI_OUT_SET_QSPI_SCK
#define SIO_GPIO_HI_OUT_SET_QSPI_SCK_RESET  _u(0x0)
#define SIO_GPIO_HI_OUT_SET_QSPI_SCK_BITS   _u(0x04000000)
#define SIO_GPIO_HI_OUT_SET_QSPI_SCK_MSB    _u(26)
#define SIO_GPIO_HI_OUT_SET_QSPI_SCK_LSB    _u(26)
#define SIO_GPIO_HI_OUT_SET_QSPI_SCK_ACCESS "WO"
// -----------------------------------------------------------------------------
// Field       : SIO_GPIO_HI_OUT_SET_USB_DM
#define SIO_GPIO_HI_OUT_SET_USB_DM_RESET  _u(0x0)
#define SIO_GPIO_HI_OUT_SET_USB_DM_BITS   _u(0x02000000)
#define SIO_GPIO_HI_OUT_SET_USB_DM_MSB    _u(25)
#define SIO_GPIO_HI_OUT_SET_USB_DM_LSB    _u(25)
#define SIO_GPIO_HI_OUT_SET_USB_DM_ACCESS "WO"
// -----------------------------------------------------------------------------
// Field       : SIO_GPIO_HI_OUT_SET_USB_DP
#define SIO_GPIO_HI_OUT_SET_USB_DP_RESET  _u(0x0)
#define SIO_GPIO_HI_OUT_SET_USB_DP_BITS   _u(0x01000000)
#define SIO_GPIO_HI_OUT_SET_USB_DP_MSB    _u(24)
#define SIO_GPIO_HI_OUT_SET_USB_DP_LSB    _u(24)
#define SIO_GPIO_HI_OUT_SET_USB_DP_ACCESS "WO"
// -----------------------------------------------------------------------------
// Field       : SIO_GPIO_HI_OUT_SET_GPIO
#define SIO_GPIO_HI_OUT_SET_GPIO_RESET  _u(0x0000)
#define SIO_GPIO_HI_OUT_SET_GPIO_BITS   _u(0x0000ffff)
#define SIO_GPIO_HI_OUT_SET_GPIO_MSB    _u(15)
#define SIO_GPIO_HI_OUT_SET_GPIO_LSB    _u(0)
#define SIO_GPIO_HI_OUT_SET_GPIO_ACCESS "WO"
// =============================================================================
// Register    : SIO_GPIO_OUT_CLR
// Description : GPIO0...31 output value clear
//               Perform an atomic bit-clear on GPIO_OUT, i.e. `GPIO_OUT &=
//               ~wdata`
#define SIO_GPIO_OUT_CLR_OFFSET _u(0x00000020)
#define SIO_GPIO_OUT_CLR_BITS   _u(0xffffffff)
#define SIO_GPIO_OUT_CLR_RESET  _u(0x00000000)
#define SIO_GPIO_OUT_CLR_MSB    _u(31)
#define SIO_GPIO_OUT_CLR_LSB    _u(0)
#define SIO_GPIO_OUT_CLR_ACCESS "WO"
// =============================================================================
// Register    : SIO_GPIO_HI_OUT_CLR
// Description : Output value clear for GPIO32..47, QSPI IOs and USB pins.
//               Perform an atomic bit-clear on GPIO_HI_OUT, i.e. `GPIO_HI_OUT
//               &= ~wdata`
#define SIO_GPIO_HI_OUT_CLR_OFFSET _u(0x00000024)
#define SIO_GPIO_HI_OUT_CLR_BITS   _u(0xff00ffff)
#define SIO_GPIO_HI_OUT_CLR_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : SIO_GPIO_HI_OUT_CLR_QSPI_SD
#define SIO_GPIO_HI_OUT_CLR_QSPI_SD_RESET  _u(0x0)
#define SIO_GPIO_HI_OUT_CLR_QSPI_SD_BITS   _u(0xf0000000)
#define SIO_GPIO_HI_OUT_CLR_QSPI_SD_MSB    _u(31)
#define SIO_GPIO_HI_OUT_CLR_QSPI_SD_LSB    _u(28)
#define SIO_GPIO_HI_OUT_CLR_QSPI_SD_ACCESS "WO"
// -----------------------------------------------------------------------------
// Field       : SIO_GPIO_HI_OUT_CLR_QSPI_CSN
#define SIO_GPIO_HI_OUT_CLR_QSPI_CSN_RESET  _u(0x0)
#define SIO_GPIO_HI_OUT_CLR_QSPI_CSN_BITS   _u(0x08000000)
#define SIO_GPIO_HI_OUT_CLR_QSPI_CSN_MSB    _u(27)
#define SIO_GPIO_HI_OUT_CLR_QSPI_CSN_LSB    _u(27)
#define SIO_GPIO_HI_OUT_CLR_QSPI_CSN_ACCESS "WO"
// -----------------------------------------------------------------------------
// Field       : SIO_GPIO_HI_OUT_CLR_QSPI_SCK
#define SIO_GPIO_HI_OUT_CLR_QSPI_SCK_RESET  _u(0x0)
#define SIO_GPIO_HI_OUT_CLR_QSPI_SCK_BITS   _u(0x04000000)
#define SIO_GPIO_HI_OUT_CLR_QSPI_SCK_MSB    _u(26)
#define SIO_GPIO_HI_OUT_CLR_QSPI_SCK_LSB    _u(26)
#define SIO_GPIO_HI_OUT_CLR_QSPI_SCK_ACCESS "WO"
// -----------------------------------------------------------------------------
// Field       : SIO_GPIO_HI_OUT_CLR_USB_DM
#define SIO_GPIO_HI_OUT_CLR_USB_DM_RESET  _u(0x0)
#define SIO_GPIO_HI_OUT_CLR_USB_DM_BITS   _u(0x02000000)
#define SIO_GPIO_HI_OUT_CLR_USB_DM_MSB    _u(25)
#define SIO_GPIO_HI_OUT_CLR_USB_DM_LSB    _u(25)
#define SIO_GPIO_HI_OUT_CLR_USB_DM_ACCESS "WO"
// -----------------------------------------------------------------------------
// Field       : SIO_GPIO_HI_OUT_CLR_USB_DP
#define SIO_GPIO_HI_OUT_CLR_USB_DP_RESET  _u(0x0)
#define SIO_GPIO_HI_OUT_CLR_USB_DP_BITS   _u(0x01000000)
#define SIO_GPIO_HI_OUT_CLR_USB_DP_MSB    _u(24)
#define SIO_GPIO_HI_OUT_CLR_USB_DP_LSB    _u(24)
#define SIO_GPIO_HI_OUT_CLR_USB_DP_ACCESS "WO"
// -----------------------------------------------------------------------------
// Field       : SIO_GPIO_HI_OUT_CLR_GPIO
#define SIO_GPIO_HI_OUT_CLR_GPIO_RESET  _u(0x0000)
#define SIO_GPIO_HI_OUT_CLR_GPIO_BITS   _u(0x0000ffff)
#define SIO_GPIO_HI_OUT_CLR_GPIO_MSB    _u(15)
#define SIO_GPIO_HI_OUT_CLR_GPIO_LSB    _u(0)
#define SIO_GPIO_HI_OUT_CLR_GPIO_ACCESS "WO"
// =============================================================================
// Register    : SIO_GPIO_OUT_XOR
// Description : GPIO0...31 output value XOR
//               Perform an atomic bitwise XOR on GPIO_OUT, i.e. `GPIO_OUT ^=
//               wdata`
#define SIO_GPIO_OUT_XOR_OFFSET _u(0x00000028)
#define SIO_GPIO_OUT_XOR_BITS   _u(0xffffffff)
#define SIO_GPIO_OUT_XOR_RESET  _u(0x00000000)
#define SIO_GPIO_OUT_XOR_MSB    _u(31)
#define SIO_GPIO_OUT_XOR_LSB    _u(0)
#define SIO_GPIO_OUT_XOR_ACCESS "WO"
// =============================================================================
// Register    : SIO_GPIO_HI_OUT_XOR
// Description : Output value XOR for GPIO32..47, QSPI IOs and USB pins.
//               Perform an atomic bitwise XOR on GPIO_HI_OUT, i.e. `GPIO_HI_OUT
//               ^= wdata`
#define SIO_GPIO_HI_OUT_XOR_OFFSET _u(0x0000002c)
#define SIO_GPIO_HI_OUT_XOR_BITS   _u(0xff00ffff)
#define SIO_GPIO_HI_OUT_XOR_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : SIO_GPIO_HI_OUT_XOR_QSPI_SD
#define SIO_GPIO_HI_OUT_XOR_QSPI_SD_RESET  _u(0x0)
#define SIO_GPIO_HI_OUT_XOR_QSPI_SD_BITS   _u(0xf0000000)
#define SIO_GPIO_HI_OUT_XOR_QSPI_SD_MSB    _u(31)
#define SIO_GPIO_HI_OUT_XOR_QSPI_SD_LSB    _u(28)
#define SIO_GPIO_HI_OUT_XOR_QSPI_SD_ACCESS "WO"
// -----------------------------------------------------------------------------
// Field       : SIO_GPIO_HI_OUT_XOR_QSPI_CSN
#define SIO_GPIO_HI_OUT_XOR_QSPI_CSN_RESET  _u(0x0)
#define SIO_GPIO_HI_OUT_XOR_QSPI_CSN_BITS   _u(0x08000000)
#define SIO_GPIO_HI_OUT_XOR_QSPI_CSN_MSB    _u(27)
#define SIO_GPIO_HI_OUT_XOR_QSPI_CSN_LSB    _u(27)
#define SIO_GPIO_HI_OUT_XOR_QSPI_CSN_ACCESS "WO"
// -----------------------------------------------------------------------------
// Field       : SIO_GPIO_HI_OUT_XOR_QSPI_SCK
#define SIO_GPIO_HI_OUT_XOR_QSPI_SCK_RESET  _u(0x0)
#define SIO_GPIO_HI_OUT_XOR_QSPI_SCK_BITS   _u(0x04000000)
#define SIO_GPIO_HI_OUT_XOR_QSPI_SCK_MSB    _u(26)
#define SIO_GPIO_HI_OUT_XOR_QSPI_SCK_LSB    _u(26)
#define SIO_GPIO_HI_OUT_XOR_QSPI_SCK_ACCESS "WO"
// -----------------------------------------------------------------------------
// Field       : SIO_GPIO_HI_OUT_XOR_USB_DM
#define SIO_GPIO_HI_OUT_XOR_USB_DM_RESET  _u(0x0)
#define SIO_GPIO_HI_OUT_XOR_USB_DM_BITS   _u(0x02000000)
#define SIO_GPIO_HI_OUT_XOR_USB_DM_MSB    _u(25)
#define SIO_GPIO_HI_OUT_XOR_USB_DM_LSB    _u(25)
#define SIO_GPIO_HI_OUT_XOR_USB_DM_ACCESS "WO"
// -----------------------------------------------------------------------------
// Field       : SIO_GPIO_HI_OUT_XOR_USB_DP
#define SIO_GPIO_HI_OUT_XOR_USB_DP_RESET  _u(0x0)
#define SIO_GPIO_HI_OUT_XOR_USB_DP_BITS   _u(0x01000000)
#define SIO_GPIO_HI_OUT_XOR_USB_DP_MSB    _u(24)
#define SIO_GPIO_HI_OUT_XOR_USB_DP_LSB    _u(24)
#define SIO_GPIO_HI_OUT_XOR_USB_DP_ACCESS "WO"
// -----------------------------------------------------------------------------
// Field       : SIO_GPIO_HI_OUT_XOR_GPIO
#define SIO_GPIO_HI_OUT_XOR_GPIO_RESET  _u(0x0000)
#define SIO_GPIO_HI_OUT_XOR_GPIO_BITS   _u(0x0000ffff)
#define SIO_GPIO_HI_OUT_XOR_GPIO_MSB    _u(15)
#define SIO_GPIO_HI_OUT_XOR_GPIO_LSB    _u(0)
#define SIO_GPIO_HI_OUT_XOR_GPIO_ACCESS "WO"
// =============================================================================
// Register    : SIO_GPIO_OE
// Description : GPIO0...31 output enable
//               Set output enable (1/0 -> output/input) for GPIO0...31. Reading
//               back gives the last value written.
//
//               If core 0 and core 1 both write to GPIO_OE simultaneously (or
//               to a SET/CLR/XOR alias), the result is as though the write from
//               core 0 took place first, and the write from core 1 was then
//               applied to that intermediate result.
//
//               In the Non-secure SIO, Secure-only GPIOs (as per ACCESSCTRL)
//               ignore writes, and their output status reads back as zero. This
//               is also true for SET/CLR/XOR aliases of this register.
#define SIO_GPIO_OE_OFFSET _u(0x00000030)
#define SIO_GPIO_OE_BITS   _u(0xffffffff)
#define SIO_GPIO_OE_RESET  _u(0x00000000)
#define SIO_GPIO_OE_MSB    _u(31)
#define SIO_GPIO_OE_LSB    _u(0)
#define SIO_GPIO_OE_ACCESS "RW"
// =============================================================================
// Register    : SIO_GPIO_HI_OE
// Description : Output enable value for GPIO32...47, QSPI IOs and USB pins.
//
//               Write output enable (1/0 -> output/input). Reading back gives
//               the last value written. If core 0 and core 1 both write to
//               GPIO_HI_OE simultaneously (or to a SET/CLR/XOR alias), the
//               result is as though the write from core 0 took place first, and
//               the write from core 1 was then applied to that intermediate
//               result.
//
//               In the Non-secure SIO, Secure-only GPIOs (as per ACCESSCTRL)
//               ignore writes, and their output status reads back as zero. This
//               is also true for SET/CLR/XOR aliases of this register.
#define SIO_GPIO_HI_OE_OFFSET _u(0x00000034)
#define SIO_GPIO_HI_OE_BITS   _u(0xff00ffff)
#define SIO_GPIO_HI_OE_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : SIO_GPIO_HI_OE_QSPI_SD
// Description : Output enable value for QSPI SD0 (MOSI), SD1 (MISO), SD2 and
//               SD3 pins
#define SIO_GPIO_HI_OE_QSPI_SD_RESET  _u(0x0)
#define SIO_GPIO_HI_OE_QSPI_SD_BITS   _u(0xf0000000)
#define SIO_GPIO_HI_OE_QSPI_SD_MSB    _u(31)
#define SIO_GPIO_HI_OE_QSPI_SD_LSB    _u(28)
#define SIO_GPIO_HI_OE_QSPI_SD_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SIO_GPIO_HI_OE_QSPI_CSN
// Description : Output enable value for QSPI CSn pin
#define SIO_GPIO_HI_OE_QSPI_CSN_RESET  _u(0x0)
#define SIO_GPIO_HI_OE_QSPI_CSN_BITS   _u(0x08000000)
#define SIO_GPIO_HI_OE_QSPI_CSN_MSB    _u(27)
#define SIO_GPIO_HI_OE_QSPI_CSN_LSB    _u(27)
#define SIO_GPIO_HI_OE_QSPI_CSN_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SIO_GPIO_HI_OE_QSPI_SCK
// Description : Output enable value for QSPI SCK pin
#define SIO_GPIO_HI_OE_QSPI_SCK_RESET  _u(0x0)
#define SIO_GPIO_HI_OE_QSPI_SCK_BITS   _u(0x04000000)
#define SIO_GPIO_HI_OE_QSPI_SCK_MSB    _u(26)
#define SIO_GPIO_HI_OE_QSPI_SCK_LSB    _u(26)
#define SIO_GPIO_HI_OE_QSPI_SCK_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SIO_GPIO_HI_OE_USB_DM
// Description : Output enable value for USB D- pin
#define SIO_GPIO_HI_OE_USB_DM_RESET  _u(0x0)
#define SIO_GPIO_HI_OE_USB_DM_BITS   _u(0x02000000)
#define SIO_GPIO_HI_OE_USB_DM_MSB    _u(25)
#define SIO_GPIO_HI_OE_USB_DM_LSB    _u(25)
#define SIO_GPIO_HI_OE_USB_DM_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SIO_GPIO_HI_OE_USB_DP
// Description : Output enable value for USB D+ pin
#define SIO_GPIO_HI_OE_USB_DP_RESET  _u(0x0)
#define SIO_GPIO_HI_OE_USB_DP_BITS   _u(0x01000000)
#define SIO_GPIO_HI_OE_USB_DP_MSB    _u(24)
#define SIO_GPIO_HI_OE_USB_DP_LSB    _u(24)
#define SIO_GPIO_HI_OE_USB_DP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SIO_GPIO_HI_OE_GPIO
// Description : Output enable value for GPIO32...47
#define SIO_GPIO_HI_OE_GPIO_RESET  _u(0x0000)
#define SIO_GPIO_HI_OE_GPIO_BITS   _u(0x0000ffff)
#define SIO_GPIO_HI_OE_GPIO_MSB    _u(15)
#define SIO_GPIO_HI_OE_GPIO_LSB    _u(0)
#define SIO_GPIO_HI_OE_GPIO_ACCESS "RW"
// =============================================================================
// Register    : SIO_GPIO_OE_SET
// Description : GPIO0...31 output enable set
//               Perform an atomic bit-set on GPIO_OE, i.e. `GPIO_OE |= wdata`
#define SIO_GPIO_OE_SET_OFFSET _u(0x00000038)
#define SIO_GPIO_OE_SET_BITS   _u(0xffffffff)
#define SIO_GPIO_OE_SET_RESET  _u(0x00000000)
#define SIO_GPIO_OE_SET_MSB    _u(31)
#define SIO_GPIO_OE_SET_LSB    _u(0)
#define SIO_GPIO_OE_SET_ACCESS "WO"
// =============================================================================
// Register    : SIO_GPIO_HI_OE_SET
// Description : Output enable set for GPIO32...47, QSPI IOs and USB pins.
//               Perform an atomic bit-set on GPIO_HI_OE, i.e. `GPIO_HI_OE |=
//               wdata`
#define SIO_GPIO_HI_OE_SET_OFFSET _u(0x0000003c)
#define SIO_GPIO_HI_OE_SET_BITS   _u(0xff00ffff)
#define SIO_GPIO_HI_OE_SET_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : SIO_GPIO_HI_OE_SET_QSPI_SD
#define SIO_GPIO_HI_OE_SET_QSPI_SD_RESET  _u(0x0)
#define SIO_GPIO_HI_OE_SET_QSPI_SD_BITS   _u(0xf0000000)
#define SIO_GPIO_HI_OE_SET_QSPI_SD_MSB    _u(31)
#define SIO_GPIO_HI_OE_SET_QSPI_SD_LSB    _u(28)
#define SIO_GPIO_HI_OE_SET_QSPI_SD_ACCESS "WO"
// -----------------------------------------------------------------------------
// Field       : SIO_GPIO_HI_OE_SET_QSPI_CSN
#define SIO_GPIO_HI_OE_SET_QSPI_CSN_RESET  _u(0x0)
#define SIO_GPIO_HI_OE_SET_QSPI_CSN_BITS   _u(0x08000000)
#define SIO_GPIO_HI_OE_SET_QSPI_CSN_MSB    _u(27)
#define SIO_GPIO_HI_OE_SET_QSPI_CSN_LSB    _u(27)
#define SIO_GPIO_HI_OE_SET_QSPI_CSN_ACCESS "WO"
// -----------------------------------------------------------------------------
// Field       : SIO_GPIO_HI_OE_SET_QSPI_SCK
#define SIO_GPIO_HI_OE_SET_QSPI_SCK_RESET  _u(0x0)
#define SIO_GPIO_HI_OE_SET_QSPI_SCK_BITS   _u(0x04000000)
#define SIO_GPIO_HI_OE_SET_QSPI_SCK_MSB    _u(26)
#define SIO_GPIO_HI_OE_SET_QSPI_SCK_LSB    _u(26)
#define SIO_GPIO_HI_OE_SET_QSPI_SCK_ACCESS "WO"
// -----------------------------------------------------------------------------
// Field       : SIO_GPIO_HI_OE_SET_USB_DM
#define SIO_GPIO_HI_OE_SET_USB_DM_RESET  _u(0x0)
#define SIO_GPIO_HI_OE_SET_USB_DM_BITS   _u(0x02000000)
#define SIO_GPIO_HI_OE_SET_USB_DM_MSB    _u(25)
#define SIO_GPIO_HI_OE_SET_USB_DM_LSB    _u(25)
#define SIO_GPIO_HI_OE_SET_USB_DM_ACCESS "WO"
// -----------------------------------------------------------------------------
// Field       : SIO_GPIO_HI_OE_SET_USB_DP
#define SIO_GPIO_HI_OE_SET_USB_DP_RESET  _u(0x0)
#define SIO_GPIO_HI_OE_SET_USB_DP_BITS   _u(0x01000000)
#define SIO_GPIO_HI_OE_SET_USB_DP_MSB    _u(24)
#define SIO_GPIO_HI_OE_SET_USB_DP_LSB    _u(24)
#define SIO_GPIO_HI_OE_SET_USB_DP_ACCESS "WO"
// -----------------------------------------------------------------------------
// Field       : SIO_GPIO_HI_OE_SET_GPIO
#define SIO_GPIO_HI_OE_SET_GPIO_RESET  _u(0x0000)
#define SIO_GPIO_HI_OE_SET_GPIO_BITS   _u(0x0000ffff)
#define SIO_GPIO_HI_OE_SET_GPIO_MSB    _u(15)
#define SIO_GPIO_HI_OE_SET_GPIO_LSB    _u(0)
#define SIO_GPIO_HI_OE_SET_GPIO_ACCESS "WO"
// =============================================================================
// Register    : SIO_GPIO_OE_CLR
// Description : GPIO0...31 output enable clear
//               Perform an atomic bit-clear on GPIO_OE, i.e. `GPIO_OE &=
//               ~wdata`
#define SIO_GPIO_OE_CLR_OFFSET _u(0x00000040)
#define SIO_GPIO_OE_CLR_BITS   _u(0xffffffff)
#define SIO_GPIO_OE_CLR_RESET  _u(0x00000000)
#define SIO_GPIO_OE_CLR_MSB    _u(31)
#define SIO_GPIO_OE_CLR_LSB    _u(0)
#define SIO_GPIO_OE_CLR_ACCESS "WO"
// =============================================================================
// Register    : SIO_GPIO_HI_OE_CLR
// Description : Output enable clear for GPIO32...47, QSPI IOs and USB pins.
//               Perform an atomic bit-clear on GPIO_HI_OE, i.e. `GPIO_HI_OE &=
//               ~wdata`
#define SIO_GPIO_HI_OE_CLR_OFFSET _u(0x00000044)
#define SIO_GPIO_HI_OE_CLR_BITS   _u(0xff00ffff)
#define SIO_GPIO_HI_OE_CLR_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : SIO_GPIO_HI_OE_CLR_QSPI_SD
#define SIO_GPIO_HI_OE_CLR_QSPI_SD_RESET  _u(0x0)
#define SIO_GPIO_HI_OE_CLR_QSPI_SD_BITS   _u(0xf0000000)
#define SIO_GPIO_HI_OE_CLR_QSPI_SD_MSB    _u(31)
#define SIO_GPIO_HI_OE_CLR_QSPI_SD_LSB    _u(28)
#define SIO_GPIO_HI_OE_CLR_QSPI_SD_ACCESS "WO"
// -----------------------------------------------------------------------------
// Field       : SIO_GPIO_HI_OE_CLR_QSPI_CSN
#define SIO_GPIO_HI_OE_CLR_QSPI_CSN_RESET  _u(0x0)
#define SIO_GPIO_HI_OE_CLR_QSPI_CSN_BITS   _u(0x08000000)
#define SIO_GPIO_HI_OE_CLR_QSPI_CSN_MSB    _u(27)
#define SIO_GPIO_HI_OE_CLR_QSPI_CSN_LSB    _u(27)
#define SIO_GPIO_HI_OE_CLR_QSPI_CSN_ACCESS "WO"
// -----------------------------------------------------------------------------
// Field       : SIO_GPIO_HI_OE_CLR_QSPI_SCK
#define SIO_GPIO_HI_OE_CLR_QSPI_SCK_RESET  _u(0x0)
#define SIO_GPIO_HI_OE_CLR_QSPI_SCK_BITS   _u(0x04000000)
#define SIO_GPIO_HI_OE_CLR_QSPI_SCK_MSB    _u(26)
#define SIO_GPIO_HI_OE_CLR_QSPI_SCK_LSB    _u(26)
#define SIO_GPIO_HI_OE_CLR_QSPI_SCK_ACCESS "WO"
// -----------------------------------------------------------------------------
// Field       : SIO_GPIO_HI_OE_CLR_USB_DM
#define SIO_GPIO_HI_OE_CLR_USB_DM_RESET  _u(0x0)
#define SIO_GPIO_HI_OE_CLR_USB_DM_BITS   _u(0x02000000)
#define SIO_GPIO_HI_OE_CLR_USB_DM_MSB    _u(25)
#define SIO_GPIO_HI_OE_CLR_USB_DM_LSB    _u(25)
#define SIO_GPIO_HI_OE_CLR_USB_DM_ACCESS "WO"
// -----------------------------------------------------------------------------
// Field       : SIO_GPIO_HI_OE_CLR_USB_DP
#define SIO_GPIO_HI_OE_CLR_USB_DP_RESET  _u(0x0)
#define SIO_GPIO_HI_OE_CLR_USB_DP_BITS   _u(0x01000000)
#define SIO_GPIO_HI_OE_CLR_USB_DP_MSB    _u(24)
#define SIO_GPIO_HI_OE_CLR_USB_DP_LSB    _u(24)
#define SIO_GPIO_HI_OE_CLR_USB_DP_ACCESS "WO"
// -----------------------------------------------------------------------------
// Field       : SIO_GPIO_HI_OE_CLR_GPIO
#define SIO_GPIO_HI_OE_CLR_GPIO_RESET  _u(0x0000)
#define SIO_GPIO_HI_OE_CLR_GPIO_BITS   _u(0x0000ffff)
#define SIO_GPIO_HI_OE_CLR_GPIO_MSB    _u(15)
#define SIO_GPIO_HI_OE_CLR_GPIO_LSB    _u(0)
#define SIO_GPIO_HI_OE_CLR_GPIO_ACCESS "WO"
// =============================================================================
// Register    : SIO_GPIO_OE_XOR
// Description : GPIO0...31 output enable XOR
//               Perform an atomic bitwise XOR on GPIO_OE, i.e. `GPIO_OE ^=
//               wdata`
#define SIO_GPIO_OE_XOR_OFFSET _u(0x00000048)
#define SIO_GPIO_OE_XOR_BITS   _u(0xffffffff)
#define SIO_GPIO_OE_XOR_RESET  _u(0x00000000)
#define SIO_GPIO_OE_XOR_MSB    _u(31)
#define SIO_GPIO_OE_XOR_LSB    _u(0)
#define SIO_GPIO_OE_XOR_ACCESS "WO"
// =============================================================================
// Register    : SIO_GPIO_HI_OE_XOR
// Description : Output enable XOR for GPIO32...47, QSPI IOs and USB pins.
//               Perform an atomic bitwise XOR on GPIO_HI_OE, i.e. `GPIO_HI_OE
//               ^= wdata`
#define SIO_GPIO_HI_OE_XOR_OFFSET _u(0x0000004c)
#define SIO_GPIO_HI_OE_XOR_BITS   _u(0xff00ffff)
#define SIO_GPIO_HI_OE_XOR_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : SIO_GPIO_HI_OE_XOR_QSPI_SD
#define SIO_GPIO_HI_OE_XOR_QSPI_SD_RESET  _u(0x0)
#define SIO_GPIO_HI_OE_XOR_QSPI_SD_BITS   _u(0xf0000000)
#define SIO_GPIO_HI_OE_XOR_QSPI_SD_MSB    _u(31)
#define SIO_GPIO_HI_OE_XOR_QSPI_SD_LSB    _u(28)
#define SIO_GPIO_HI_OE_XOR_QSPI_SD_ACCESS "WO"
// -----------------------------------------------------------------------------
// Field       : SIO_GPIO_HI_OE_XOR_QSPI_CSN
#define SIO_GPIO_HI_OE_XOR_QSPI_CSN_RESET  _u(0x0)
#define SIO_GPIO_HI_OE_XOR_QSPI_CSN_BITS   _u(0x08000000)
#define SIO_GPIO_HI_OE_XOR_QSPI_CSN_MSB    _u(27)
#define SIO_GPIO_HI_OE_XOR_QSPI_CSN_LSB    _u(27)
#define SIO_GPIO_HI_OE_XOR_QSPI_CSN_ACCESS "WO"
// -----------------------------------------------------------------------------
// Field       : SIO_GPIO_HI_OE_XOR_QSPI_SCK
#define SIO_GPIO_HI_OE_XOR_QSPI_SCK_RESET  _u(0x0)
#define SIO_GPIO_HI_OE_XOR_QSPI_SCK_BITS   _u(0x04000000)
#define SIO_GPIO_HI_OE_XOR_QSPI_SCK_MSB    _u(26)
#define SIO_GPIO_HI_OE_XOR_QSPI_SCK_LSB    _u(26)
#define SIO_GPIO_HI_OE_XOR_QSPI_SCK_ACCESS "WO"
// -----------------------------------------------------------------------------
// Field       : SIO_GPIO_HI_OE_XOR_USB_DM
#define SIO_GPIO_HI_OE_XOR_USB_DM_RESET  _u(0x0)
#define SIO_GPIO_HI_OE_XOR_USB_DM_BITS   _u(0x02000000)
#define SIO_GPIO_HI_OE_XOR_USB_DM_MSB    _u(25)
#define SIO_GPIO_HI_OE_XOR_USB_DM_LSB    _u(25)
#define SIO_GPIO_HI_OE_XOR_USB_DM_ACCESS "WO"
// -----------------------------------------------------------------------------
// Field       : SIO_GPIO_HI_OE_XOR_USB_DP
#define SIO_GPIO_HI_OE_XOR_USB_DP_RESET  _u(0x0)
#define SIO_GPIO_HI_OE_XOR_USB_DP_BITS   _u(0x01000000)
#define SIO_GPIO_HI_OE_XOR_USB_DP_MSB    _u(24)
#define SIO_GPIO_HI_OE_XOR_USB_DP_LSB    _u(24)
#define SIO_GPIO_HI_OE_XOR_USB_DP_ACCESS "WO"
// -----------------------------------------------------------------------------
// Field       : SIO_GPIO_HI_OE_XOR_GPIO
#define SIO_GPIO_HI_OE_XOR_GPIO_RESET  _u(0x0000)
#define SIO_GPIO_HI_OE_XOR_GPIO_BITS   _u(0x0000ffff)
#define SIO_GPIO_HI_OE_XOR_GPIO_MSB    _u(15)
#define SIO_GPIO_HI_OE_XOR_GPIO_LSB    _u(0)
#define SIO_GPIO_HI_OE_XOR_GPIO_ACCESS "WO"
// =============================================================================
// Register    : SIO_FIFO_ST
// Description : Status register for inter-core FIFOs (mailboxes).
//               There is one FIFO in the core 0 -> core 1 direction, and one
//               core 1 -> core 0. Both are 32 bits wide and 8 words deep.
//               Core 0 can see the read side of the 1->0 FIFO (RX), and the
//               write side of 0->1 FIFO (TX).
//               Core 1 can see the read side of the 0->1 FIFO (RX), and the
//               write side of 1->0 FIFO (TX).
//               The SIO IRQ for each core is the logical OR of the VLD, WOF and
//               ROE fields of its FIFO_ST register.
#define SIO_FIFO_ST_OFFSET _u(0x00000050)
#define SIO_FIFO_ST_BITS   _u(0x0000000f)
#define SIO_FIFO_ST_RESET  _u(0x00000002)
// -----------------------------------------------------------------------------
// Field       : SIO_FIFO_ST_ROE
// Description : Sticky flag indicating the RX FIFO was read when empty. This
//               read was ignored by the FIFO.
#define SIO_FIFO_ST_ROE_RESET  _u(0x0)
#define SIO_FIFO_ST_ROE_BITS   _u(0x00000008)
#define SIO_FIFO_ST_ROE_MSB    _u(3)
#define SIO_FIFO_ST_ROE_LSB    _u(3)
#define SIO_FIFO_ST_ROE_ACCESS "WC"
// -----------------------------------------------------------------------------
// Field       : SIO_FIFO_ST_WOF
// Description : Sticky flag indicating the TX FIFO was written when full. This
//               write was ignored by the FIFO.
#define SIO_FIFO_ST_WOF_RESET  _u(0x0)
#define SIO_FIFO_ST_WOF_BITS   _u(0x00000004)
#define SIO_FIFO_ST_WOF_MSB    _u(2)
#define SIO_FIFO_ST_WOF_LSB    _u(2)
#define SIO_FIFO_ST_WOF_ACCESS "WC"
// -----------------------------------------------------------------------------
// Field       : SIO_FIFO_ST_RDY
// Description : Value is 1 if this core's TX FIFO is not full (i.e. if FIFO_WR
//               is ready for more data)
#define SIO_FIFO_ST_RDY_RESET  _u(0x1)
#define SIO_FIFO_ST_RDY_BITS   _u(0x00000002)
#define SIO_FIFO_ST_RDY_MSB    _u(1)
#define SIO_FIFO_ST_RDY_LSB    _u(1)
#define SIO_FIFO_ST_RDY_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : SIO_FIFO_ST_VLD
// Description : Value is 1 if this core's RX FIFO is not empty (i.e. if FIFO_RD
//               is valid)
#define SIO_FIFO_ST_VLD_RESET  _u(0x0)
#define SIO_FIFO_ST_VLD_BITS   _u(0x00000001)
#define SIO_FIFO_ST_VLD_MSB    _u(0)
#define SIO_FIFO_ST_VLD_LSB    _u(0)
#define SIO_FIFO_ST_VLD_ACCESS "RO"
// =============================================================================
// Register    : SIO_FIFO_WR
// Description : Write access to this core's TX FIFO
#define SIO_FIFO_WR_OFFSET _u(0x00000054)
#define SIO_FIFO_WR_BITS   _u(0xffffffff)
#define SIO_FIFO_WR_RESET  _u(0x00000000)
#define SIO_FIFO_WR_MSB    _u(31)
#define SIO_FIFO_WR_LSB    _u(0)
#define SIO_FIFO_WR_ACCESS "WF"
// =============================================================================
// Register    : SIO_FIFO_RD
// Description : Read access to this core's RX FIFO
#define SIO_FIFO_RD_OFFSET _u(0x00000058)
#define SIO_FIFO_RD_BITS   _u(0xffffffff)
#define SIO_FIFO_RD_RESET  "-"
#define SIO_FIFO_RD_MSB    _u(31)
#define SIO_FIFO_RD_LSB    _u(0)
#define SIO_FIFO_RD_ACCESS "RF"
// =============================================================================
// Register    : SIO_SPINLOCK_ST
// Description : Spinlock state
//               A bitmap containing the state of all 32 spinlocks (1=locked).
//               Mainly intended for debugging.
#define SIO_SPINLOCK_ST_OFFSET _u(0x0000005c)
#define SIO_SPINLOCK_ST_BITS   _u(0xffffffff)
#define SIO_SPINLOCK_ST_RESET  _u(0x00000000)
#define SIO_SPINLOCK_ST_MSB    _u(31)
#define SIO_SPINLOCK_ST_LSB    _u(0)
#define SIO_SPINLOCK_ST_ACCESS "RO"
// =============================================================================
// Register    : SIO_INTERP0_ACCUM0
// Description : Read/write access to accumulator 0
#define SIO_INTERP0_ACCUM0_OFFSET _u(0x00000080)
#define SIO_INTERP0_ACCUM0_BITS   _u(0xffffffff)
#define SIO_INTERP0_ACCUM0_RESET  _u(0x00000000)
#define SIO_INTERP0_ACCUM0_MSB    _u(31)
#define SIO_INTERP0_ACCUM0_LSB    _u(0)
#define SIO_INTERP0_ACCUM0_ACCESS "RW"
// =============================================================================
// Register    : SIO_INTERP0_ACCUM1
// Description : Read/write access to accumulator 1
#define SIO_INTERP0_ACCUM1_OFFSET _u(0x00000084)
#define SIO_INTERP0_ACCUM1_BITS   _u(0xffffffff)
#define SIO_INTERP0_ACCUM1_RESET  _u(0x00000000)
#define SIO_INTERP0_ACCUM1_MSB    _u(31)
#define SIO_INTERP0_ACCUM1_LSB    _u(0)
#define SIO_INTERP0_ACCUM1_ACCESS "RW"
// =============================================================================
// Register    : SIO_INTERP0_BASE0
// Description : Read/write access to BASE0 register.
#define SIO_INTERP0_BASE0_OFFSET _u(0x00000088)
#define SIO_INTERP0_BASE0_BITS   _u(0xffffffff)
#define SIO_INTERP0_BASE0_RESET  _u(0x00000000)
#define SIO_INTERP0_BASE0_MSB    _u(31)
#define SIO_INTERP0_BASE0_LSB    _u(0)
#define SIO_INTERP0_BASE0_ACCESS "RW"
// =============================================================================
// Register    : SIO_INTERP0_BASE1
// Description : Read/write access to BASE1 register.
#define SIO_INTERP0_BASE1_OFFSET _u(0x0000008c)
#define SIO_INTERP0_BASE1_BITS   _u(0xffffffff)
#define SIO_INTERP0_BASE1_RESET  _u(0x00000000)
#define SIO_INTERP0_BASE1_MSB    _u(31)
#define SIO_INTERP0_BASE1_LSB    _u(0)
#define SIO_INTERP0_BASE1_ACCESS "RW"
// =============================================================================
// Register    : SIO_INTERP0_BASE2
// Description : Read/write access to BASE2 register.
#define SIO_INTERP0_BASE2_OFFSET _u(0x00000090)
#define SIO_INTERP0_BASE2_BITS   _u(0xffffffff)
#define SIO_INTERP0_BASE2_RESET  _u(0x00000000)
#define SIO_INTERP0_BASE2_MSB    _u(31)
#define SIO_INTERP0_BASE2_LSB    _u(0)
#define SIO_INTERP0_BASE2_ACCESS "RW"
// =============================================================================
// Register    : SIO_INTERP0_POP_LANE0
// Description : Read LANE0 result, and simultaneously write lane results to
//               both accumulators (POP).
#define SIO_INTERP0_POP_LANE0_OFFSET _u(0x00000094)
#define SIO_INTERP0_POP_LANE0_BITS   _u(0xffffffff)
#define SIO_INTERP0_POP_LANE0_RESET  _u(0x00000000)
#define SIO_INTERP0_POP_LANE0_MSB    _u(31)
#define SIO_INTERP0_POP_LANE0_LSB    _u(0)
#define SIO_INTERP0_POP_LANE0_ACCESS "RO"
// =============================================================================
// Register    : SIO_INTERP0_POP_LANE1
// Description : Read LANE1 result, and simultaneously write lane results to
//               both accumulators (POP).
#define SIO_INTERP0_POP_LANE1_OFFSET _u(0x00000098)
#define SIO_INTERP0_POP_LANE1_BITS   _u(0xffffffff)
#define SIO_INTERP0_POP_LANE1_RESET  _u(0x00000000)
#define SIO_INTERP0_POP_LANE1_MSB    _u(31)
#define SIO_INTERP0_POP_LANE1_LSB    _u(0)
#define SIO_INTERP0_POP_LANE1_ACCESS "RO"
// =============================================================================
// Register    : SIO_INTERP0_POP_FULL
// Description : Read FULL result, and simultaneously write lane results to both
//               accumulators (POP).
#define SIO_INTERP0_POP_FULL_OFFSET _u(0x0000009c)
#define SIO_INTERP0_POP_FULL_BITS   _u(0xffffffff)
#define SIO_INTERP0_POP_FULL_RESET  _u(0x00000000)
#define SIO_INTERP0_POP_FULL_MSB    _u(31)
#define SIO_INTERP0_POP_FULL_LSB    _u(0)
#define SIO_INTERP0_POP_FULL_ACCESS "RO"
// =============================================================================
// Register    : SIO_INTERP0_PEEK_LANE0
// Description : Read LANE0 result, without altering any internal state (PEEK).
#define SIO_INTERP0_PEEK_LANE0_OFFSET _u(0x000000a0)
#define SIO_INTERP0_PEEK_LANE0_BITS   _u(0xffffffff)
#define SIO_INTERP0_PEEK_LANE0_RESET  _u(0x00000000)
#define SIO_INTERP0_PEEK_LANE0_MSB    _u(31)
#define SIO_INTERP0_PEEK_LANE0_LSB    _u(0)
#define SIO_INTERP0_PEEK_LANE0_ACCESS "RO"
// =============================================================================
// Register    : SIO_INTERP0_PEEK_LANE1
// Description : Read LANE1 result, without altering any internal state (PEEK).
#define SIO_INTERP0_PEEK_LANE1_OFFSET _u(0x000000a4)
#define SIO_INTERP0_PEEK_LANE1_BITS   _u(0xffffffff)
#define SIO_INTERP0_PEEK_LANE1_RESET  _u(0x00000000)
#define SIO_INTERP0_PEEK_LANE1_MSB    _u(31)
#define SIO_INTERP0_PEEK_LANE1_LSB    _u(0)
#define SIO_INTERP0_PEEK_LANE1_ACCESS "RO"
// =============================================================================
// Register    : SIO_INTERP0_PEEK_FULL
// Description : Read FULL result, without altering any internal state (PEEK).
#define SIO_INTERP0_PEEK_FULL_OFFSET _u(0x000000a8)
#define SIO_INTERP0_PEEK_FULL_BITS   _u(0xffffffff)
#define SIO_INTERP0_PEEK_FULL_RESET  _u(0x00000000)
#define SIO_INTERP0_PEEK_FULL_MSB    _u(31)
#define SIO_INTERP0_PEEK_FULL_LSB    _u(0)
#define SIO_INTERP0_PEEK_FULL_ACCESS "RO"
// =============================================================================
// Register    : SIO_INTERP0_CTRL_LANE0
// Description : Control register for lane 0
#define SIO_INTERP0_CTRL_LANE0_OFFSET _u(0x000000ac)
#define SIO_INTERP0_CTRL_LANE0_BITS   _u(0x03bfffff)
#define SIO_INTERP0_CTRL_LANE0_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : SIO_INTERP0_CTRL_LANE0_OVERF
// Description : Set if either OVERF0 or OVERF1 is set.
#define SIO_INTERP0_CTRL_LANE0_OVERF_RESET  _u(0x0)
#define SIO_INTERP0_CTRL_LANE0_OVERF_BITS   _u(0x02000000)
#define SIO_INTERP0_CTRL_LANE0_OVERF_MSB    _u(25)
#define SIO_INTERP0_CTRL_LANE0_OVERF_LSB    _u(25)
#define SIO_INTERP0_CTRL_LANE0_OVERF_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : SIO_INTERP0_CTRL_LANE0_OVERF1
// Description : Indicates if any masked-off MSBs in ACCUM1 are set.
#define SIO_INTERP0_CTRL_LANE0_OVERF1_RESET  _u(0x0)
#define SIO_INTERP0_CTRL_LANE0_OVERF1_BITS   _u(0x01000000)
#define SIO_INTERP0_CTRL_LANE0_OVERF1_MSB    _u(24)
#define SIO_INTERP0_CTRL_LANE0_OVERF1_LSB    _u(24)
#define SIO_INTERP0_CTRL_LANE0_OVERF1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : SIO_INTERP0_CTRL_LANE0_OVERF0
// Description : Indicates if any masked-off MSBs in ACCUM0 are set.
#define SIO_INTERP0_CTRL_LANE0_OVERF0_RESET  _u(0x0)
#define SIO_INTERP0_CTRL_LANE0_OVERF0_BITS   _u(0x00800000)
#define SIO_INTERP0_CTRL_LANE0_OVERF0_MSB    _u(23)
#define SIO_INTERP0_CTRL_LANE0_OVERF0_LSB    _u(23)
#define SIO_INTERP0_CTRL_LANE0_OVERF0_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : SIO_INTERP0_CTRL_LANE0_BLEND
// Description : Only present on INTERP0 on each core. If BLEND mode is enabled:
//               - LANE1 result is a linear interpolation between BASE0 and
//               BASE1, controlled
//               by the 8 LSBs of lane 1 shift and mask value (a fractional
//               number between
//               0 and 255/256ths)
//               - LANE0 result does not have BASE0 added (yields only the 8
//               LSBs of lane 1 shift+mask value)
//               - FULL result does not have lane 1 shift+mask value added
//               (BASE2 + lane 0 shift+mask)
//               LANE1 SIGNED flag controls whether the interpolation is signed
//               or unsigned.
#define SIO_INTERP0_CTRL_LANE0_BLEND_RESET  _u(0x0)
#define SIO_INTERP0_CTRL_LANE0_BLEND_BITS   _u(0x00200000)
#define SIO_INTERP0_CTRL_LANE0_BLEND_MSB    _u(21)
#define SIO_INTERP0_CTRL_LANE0_BLEND_LSB    _u(21)
#define SIO_INTERP0_CTRL_LANE0_BLEND_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SIO_INTERP0_CTRL_LANE0_FORCE_MSB
// Description : ORed into bits 29:28 of the lane result presented to the
//               processor on the bus.
//               No effect on the internal 32-bit datapath. Handy for using a
//               lane to generate sequence
//               of pointers into flash or SRAM.
#define SIO_INTERP0_CTRL_LANE0_FORCE_MSB_RESET  _u(0x0)
#define SIO_INTERP0_CTRL_LANE0_FORCE_MSB_BITS   _u(0x00180000)
#define SIO_INTERP0_CTRL_LANE0_FORCE_MSB_MSB    _u(20)
#define SIO_INTERP0_CTRL_LANE0_FORCE_MSB_LSB    _u(19)
#define SIO_INTERP0_CTRL_LANE0_FORCE_MSB_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SIO_INTERP0_CTRL_LANE0_ADD_RAW
// Description : If 1, mask + shift is bypassed for LANE0 result. This does not
//               affect FULL result.
#define SIO_INTERP0_CTRL_LANE0_ADD_RAW_RESET  _u(0x0)
#define SIO_INTERP0_CTRL_LANE0_ADD_RAW_BITS   _u(0x00040000)
#define SIO_INTERP0_CTRL_LANE0_ADD_RAW_MSB    _u(18)
#define SIO_INTERP0_CTRL_LANE0_ADD_RAW_LSB    _u(18)
#define SIO_INTERP0_CTRL_LANE0_ADD_RAW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SIO_INTERP0_CTRL_LANE0_CROSS_RESULT
// Description : If 1, feed the opposite lane's result into this lane's
//               accumulator on POP.
#define SIO_INTERP0_CTRL_LANE0_CROSS_RESULT_RESET  _u(0x0)
#define SIO_INTERP0_CTRL_LANE0_CROSS_RESULT_BITS   _u(0x00020000)
#define SIO_INTERP0_CTRL_LANE0_CROSS_RESULT_MSB    _u(17)
#define SIO_INTERP0_CTRL_LANE0_CROSS_RESULT_LSB    _u(17)
#define SIO_INTERP0_CTRL_LANE0_CROSS_RESULT_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SIO_INTERP0_CTRL_LANE0_CROSS_INPUT
// Description : If 1, feed the opposite lane's accumulator into this lane's
//               shift + mask hardware.
//               Takes effect even if ADD_RAW is set (the CROSS_INPUT mux is
//               before the shift+mask bypass)
#define SIO_INTERP0_CTRL_LANE0_CROSS_INPUT_RESET  _u(0x0)
#define SIO_INTERP0_CTRL_LANE0_CROSS_INPUT_BITS   _u(0x00010000)
#define SIO_INTERP0_CTRL_LANE0_CROSS_INPUT_MSB    _u(16)
#define SIO_INTERP0_CTRL_LANE0_CROSS_INPUT_LSB    _u(16)
#define SIO_INTERP0_CTRL_LANE0_CROSS_INPUT_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SIO_INTERP0_CTRL_LANE0_SIGNED
// Description : If SIGNED is set, the shifted and masked accumulator value is
//               sign-extended to 32 bits
//               before adding to BASE0, and LANE0 PEEK/POP appear extended to
//               32 bits when read by processor.
#define SIO_INTERP0_CTRL_LANE0_SIGNED_RESET  _u(0x0)
#define SIO_INTERP0_CTRL_LANE0_SIGNED_BITS   _u(0x00008000)
#define SIO_INTERP0_CTRL_LANE0_SIGNED_MSB    _u(15)
#define SIO_INTERP0_CTRL_LANE0_SIGNED_LSB    _u(15)
#define SIO_INTERP0_CTRL_LANE0_SIGNED_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SIO_INTERP0_CTRL_LANE0_MASK_MSB
// Description : The most-significant bit allowed to pass by the mask
//               (inclusive)
//               Setting MSB < LSB may cause chip to turn inside-out
#define SIO_INTERP0_CTRL_LANE0_MASK_MSB_RESET  _u(0x00)
#define SIO_INTERP0_CTRL_LANE0_MASK_MSB_BITS   _u(0x00007c00)
#define SIO_INTERP0_CTRL_LANE0_MASK_MSB_MSB    _u(14)
#define SIO_INTERP0_CTRL_LANE0_MASK_MSB_LSB    _u(10)
#define SIO_INTERP0_CTRL_LANE0_MASK_MSB_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SIO_INTERP0_CTRL_LANE0_MASK_LSB
// Description : The least-significant bit allowed to pass by the mask
//               (inclusive)
#define SIO_INTERP0_CTRL_LANE0_MASK_LSB_RESET  _u(0x00)
#define SIO_INTERP0_CTRL_LANE0_MASK_LSB_BITS   _u(0x000003e0)
#define SIO_INTERP0_CTRL_LANE0_MASK_LSB_MSB    _u(9)
#define SIO_INTERP0_CTRL_LANE0_MASK_LSB_LSB    _u(5)
#define SIO_INTERP0_CTRL_LANE0_MASK_LSB_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SIO_INTERP0_CTRL_LANE0_SHIFT
// Description : Right-rotate applied to accumulator before masking. By
//               appropriately configuring the masks, left and right shifts can
//               be synthesised.
#define SIO_INTERP0_CTRL_LANE0_SHIFT_RESET  _u(0x00)
#define SIO_INTERP0_CTRL_LANE0_SHIFT_BITS   _u(0x0000001f)
#define SIO_INTERP0_CTRL_LANE0_SHIFT_MSB    _u(4)
#define SIO_INTERP0_CTRL_LANE0_SHIFT_LSB    _u(0)
#define SIO_INTERP0_CTRL_LANE0_SHIFT_ACCESS "RW"
// =============================================================================
// Register    : SIO_INTERP0_CTRL_LANE1
// Description : Control register for lane 1
#define SIO_INTERP0_CTRL_LANE1_OFFSET _u(0x000000b0)
#define SIO_INTERP0_CTRL_LANE1_BITS   _u(0x001fffff)
#define SIO_INTERP0_CTRL_LANE1_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : SIO_INTERP0_CTRL_LANE1_FORCE_MSB
// Description : ORed into bits 29:28 of the lane result presented to the
//               processor on the bus.
//               No effect on the internal 32-bit datapath. Handy for using a
//               lane to generate sequence
//               of pointers into flash or SRAM.
#define SIO_INTERP0_CTRL_LANE1_FORCE_MSB_RESET  _u(0x0)
#define SIO_INTERP0_CTRL_LANE1_FORCE_MSB_BITS   _u(0x00180000)
#define SIO_INTERP0_CTRL_LANE1_FORCE_MSB_MSB    _u(20)
#define SIO_INTERP0_CTRL_LANE1_FORCE_MSB_LSB    _u(19)
#define SIO_INTERP0_CTRL_LANE1_FORCE_MSB_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SIO_INTERP0_CTRL_LANE1_ADD_RAW
// Description : If 1, mask + shift is bypassed for LANE1 result. This does not
//               affect FULL result.
#define SIO_INTERP0_CTRL_LANE1_ADD_RAW_RESET  _u(0x0)
#define SIO_INTERP0_CTRL_LANE1_ADD_RAW_BITS   _u(0x00040000)
#define SIO_INTERP0_CTRL_LANE1_ADD_RAW_MSB    _u(18)
#define SIO_INTERP0_CTRL_LANE1_ADD_RAW_LSB    _u(18)
#define SIO_INTERP0_CTRL_LANE1_ADD_RAW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SIO_INTERP0_CTRL_LANE1_CROSS_RESULT
// Description : If 1, feed the opposite lane's result into this lane's
//               accumulator on POP.
#define SIO_INTERP0_CTRL_LANE1_CROSS_RESULT_RESET  _u(0x0)
#define SIO_INTERP0_CTRL_LANE1_CROSS_RESULT_BITS   _u(0x00020000)
#define SIO_INTERP0_CTRL_LANE1_CROSS_RESULT_MSB    _u(17)
#define SIO_INTERP0_CTRL_LANE1_CROSS_RESULT_LSB    _u(17)
#define SIO_INTERP0_CTRL_LANE1_CROSS_RESULT_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SIO_INTERP0_CTRL_LANE1_CROSS_INPUT
// Description : If 1, feed the opposite lane's accumulator into this lane's
//               shift + mask hardware.
//               Takes effect even if ADD_RAW is set (the CROSS_INPUT mux is
//               before the shift+mask bypass)
#define SIO_INTERP0_CTRL_LANE1_CROSS_INPUT_RESET  _u(0x0)
#define SIO_INTERP0_CTRL_LANE1_CROSS_INPUT_BITS   _u(0x00010000)
#define SIO_INTERP0_CTRL_LANE1_CROSS_INPUT_MSB    _u(16)
#define SIO_INTERP0_CTRL_LANE1_CROSS_INPUT_LSB    _u(16)
#define SIO_INTERP0_CTRL_LANE1_CROSS_INPUT_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SIO_INTERP0_CTRL_LANE1_SIGNED
// Description : If SIGNED is set, the shifted and masked accumulator value is
//               sign-extended to 32 bits
//               before adding to BASE1, and LANE1 PEEK/POP appear extended to
//               32 bits when read by processor.
#define SIO_INTERP0_CTRL_LANE1_SIGNED_RESET  _u(0x0)
#define SIO_INTERP0_CTRL_LANE1_SIGNED_BITS   _u(0x00008000)
#define SIO_INTERP0_CTRL_LANE1_SIGNED_MSB    _u(15)
#define SIO_INTERP0_CTRL_LANE1_SIGNED_LSB    _u(15)
#define SIO_INTERP0_CTRL_LANE1_SIGNED_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SIO_INTERP0_CTRL_LANE1_MASK_MSB
// Description : The most-significant bit allowed to pass by the mask
//               (inclusive)
//               Setting MSB < LSB may cause chip to turn inside-out
#define SIO_INTERP0_CTRL_LANE1_MASK_MSB_RESET  _u(0x00)
#define SIO_INTERP0_CTRL_LANE1_MASK_MSB_BITS   _u(0x00007c00)
#define SIO_INTERP0_CTRL_LANE1_MASK_MSB_MSB    _u(14)
#define SIO_INTERP0_CTRL_LANE1_MASK_MSB_LSB    _u(10)
#define SIO_INTERP0_CTRL_LANE1_MASK_MSB_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SIO_INTERP0_CTRL_LANE1_MASK_LSB
// Description : The least-significant bit allowed to pass by the mask
//               (inclusive)
#define SIO_INTERP0_CTRL_LANE1_MASK_LSB_RESET  _u(0x00)
#define SIO_INTERP0_CTRL_LANE1_MASK_LSB_BITS   _u(0x000003e0)
#define SIO_INTERP0_CTRL_LANE1_MASK_LSB_MSB    _u(9)
#define SIO_INTERP0_CTRL_LANE1_MASK_LSB_LSB    _u(5)
#define SIO_INTERP0_CTRL_LANE1_MASK_LSB_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SIO_INTERP0_CTRL_LANE1_SHIFT
// Description : Right-rotate applied to accumulator before masking. By
//               appropriately configuring the masks, left and right shifts can
//               be synthesised.
#define SIO_INTERP0_CTRL_LANE1_SHIFT_RESET  _u(0x00)
#define SIO_INTERP0_CTRL_LANE1_SHIFT_BITS   _u(0x0000001f)
#define SIO_INTERP0_CTRL_LANE1_SHIFT_MSB    _u(4)
#define SIO_INTERP0_CTRL_LANE1_SHIFT_LSB    _u(0)
#define SIO_INTERP0_CTRL_LANE1_SHIFT_ACCESS "RW"
// =============================================================================
// Register    : SIO_INTERP0_ACCUM0_ADD
// Description : Values written here are atomically added to ACCUM0
//               Reading yields lane 0's raw shift and mask value (BASE0 not
//               added).
#define SIO_INTERP0_ACCUM0_ADD_OFFSET _u(0x000000b4)
#define SIO_INTERP0_ACCUM0_ADD_BITS   _u(0x00ffffff)
#define SIO_INTERP0_ACCUM0_ADD_RESET  _u(0x00000000)
#define SIO_INTERP0_ACCUM0_ADD_MSB    _u(23)
#define SIO_INTERP0_ACCUM0_ADD_LSB    _u(0)
#define SIO_INTERP0_ACCUM0_ADD_ACCESS "RW"
// =============================================================================
// Register    : SIO_INTERP0_ACCUM1_ADD
// Description : Values written here are atomically added to ACCUM1
//               Reading yields lane 1's raw shift and mask value (BASE1 not
//               added).
#define SIO_INTERP0_ACCUM1_ADD_OFFSET _u(0x000000b8)
#define SIO_INTERP0_ACCUM1_ADD_BITS   _u(0x00ffffff)
#define SIO_INTERP0_ACCUM1_ADD_RESET  _u(0x00000000)
#define SIO_INTERP0_ACCUM1_ADD_MSB    _u(23)
#define SIO_INTERP0_ACCUM1_ADD_LSB    _u(0)
#define SIO_INTERP0_ACCUM1_ADD_ACCESS "RW"
// =============================================================================
// Register    : SIO_INTERP0_BASE_1AND0
// Description : On write, the lower 16 bits go to BASE0, upper bits to BASE1
//               simultaneously.
//               Each half is sign-extended to 32 bits if that lane's SIGNED
//               flag is set.
#define SIO_INTERP0_BASE_1AND0_OFFSET _u(0x000000bc)
#define SIO_INTERP0_BASE_1AND0_BITS   _u(0xffffffff)
#define SIO_INTERP0_BASE_1AND0_RESET  _u(0x00000000)
#define SIO_INTERP0_BASE_1AND0_MSB    _u(31)
#define SIO_INTERP0_BASE_1AND0_LSB    _u(0)
#define SIO_INTERP0_BASE_1AND0_ACCESS "WO"
// =============================================================================
// Register    : SIO_INTERP1_ACCUM0
// Description : Read/write access to accumulator 0
#define SIO_INTERP1_ACCUM0_OFFSET _u(0x000000c0)
#define SIO_INTERP1_ACCUM0_BITS   _u(0xffffffff)
#define SIO_INTERP1_ACCUM0_RESET  _u(0x00000000)
#define SIO_INTERP1_ACCUM0_MSB    _u(31)
#define SIO_INTERP1_ACCUM0_LSB    _u(0)
#define SIO_INTERP1_ACCUM0_ACCESS "RW"
// =============================================================================
// Register    : SIO_INTERP1_ACCUM1
// Description : Read/write access to accumulator 1
#define SIO_INTERP1_ACCUM1_OFFSET _u(0x000000c4)
#define SIO_INTERP1_ACCUM1_BITS   _u(0xffffffff)
#define SIO_INTERP1_ACCUM1_RESET  _u(0x00000000)
#define SIO_INTERP1_ACCUM1_MSB    _u(31)
#define SIO_INTERP1_ACCUM1_LSB    _u(0)
#define SIO_INTERP1_ACCUM1_ACCESS "RW"
// =============================================================================
// Register    : SIO_INTERP1_BASE0
// Description : Read/write access to BASE0 register.
#define SIO_INTERP1_BASE0_OFFSET _u(0x000000c8)
#define SIO_INTERP1_BASE0_BITS   _u(0xffffffff)
#define SIO_INTERP1_BASE0_RESET  _u(0x00000000)
#define SIO_INTERP1_BASE0_MSB    _u(31)
#define SIO_INTERP1_BASE0_LSB    _u(0)
#define SIO_INTERP1_BASE0_ACCESS "RW"
// =============================================================================
// Register    : SIO_INTERP1_BASE1
// Description : Read/write access to BASE1 register.
#define SIO_INTERP1_BASE1_OFFSET _u(0x000000cc)
#define SIO_INTERP1_BASE1_BITS   _u(0xffffffff)
#define SIO_INTERP1_BASE1_RESET  _u(0x00000000)
#define SIO_INTERP1_BASE1_MSB    _u(31)
#define SIO_INTERP1_BASE1_LSB    _u(0)
#define SIO_INTERP1_BASE1_ACCESS "RW"
// =============================================================================
// Register    : SIO_INTERP1_BASE2
// Description : Read/write access to BASE2 register.
#define SIO_INTERP1_BASE2_OFFSET _u(0x000000d0)
#define SIO_INTERP1_BASE2_BITS   _u(0xffffffff)
#define SIO_INTERP1_BASE2_RESET  _u(0x00000000)
#define SIO_INTERP1_BASE2_MSB    _u(31)
#define SIO_INTERP1_BASE2_LSB    _u(0)
#define SIO_INTERP1_BASE2_ACCESS "RW"
// =============================================================================
// Register    : SIO_INTERP1_POP_LANE0
// Description : Read LANE0 result, and simultaneously write lane results to
//               both accumulators (POP).
#define SIO_INTERP1_POP_LANE0_OFFSET _u(0x000000d4)
#define SIO_INTERP1_POP_LANE0_BITS   _u(0xffffffff)
#define SIO_INTERP1_POP_LANE0_RESET  _u(0x00000000)
#define SIO_INTERP1_POP_LANE0_MSB    _u(31)
#define SIO_INTERP1_POP_LANE0_LSB    _u(0)
#define SIO_INTERP1_POP_LANE0_ACCESS "RO"
// =============================================================================
// Register    : SIO_INTERP1_POP_LANE1
// Description : Read LANE1 result, and simultaneously write lane results to
//               both accumulators (POP).
#define SIO_INTERP1_POP_LANE1_OFFSET _u(0x000000d8)
#define SIO_INTERP1_POP_LANE1_BITS   _u(0xffffffff)
#define SIO_INTERP1_POP_LANE1_RESET  _u(0x00000000)
#define SIO_INTERP1_POP_LANE1_MSB    _u(31)
#define SIO_INTERP1_POP_LANE1_LSB    _u(0)
#define SIO_INTERP1_POP_LANE1_ACCESS "RO"
// =============================================================================
// Register    : SIO_INTERP1_POP_FULL
// Description : Read FULL result, and simultaneously write lane results to both
//               accumulators (POP).
#define SIO_INTERP1_POP_FULL_OFFSET _u(0x000000dc)
#define SIO_INTERP1_POP_FULL_BITS   _u(0xffffffff)
#define SIO_INTERP1_POP_FULL_RESET  _u(0x00000000)
#define SIO_INTERP1_POP_FULL_MSB    _u(31)
#define SIO_INTERP1_POP_FULL_LSB    _u(0)
#define SIO_INTERP1_POP_FULL_ACCESS "RO"
// =============================================================================
// Register    : SIO_INTERP1_PEEK_LANE0
// Description : Read LANE0 result, without altering any internal state (PEEK).
#define SIO_INTERP1_PEEK_LANE0_OFFSET _u(0x000000e0)
#define SIO_INTERP1_PEEK_LANE0_BITS   _u(0xffffffff)
#define SIO_INTERP1_PEEK_LANE0_RESET  _u(0x00000000)
#define SIO_INTERP1_PEEK_LANE0_MSB    _u(31)
#define SIO_INTERP1_PEEK_LANE0_LSB    _u(0)
#define SIO_INTERP1_PEEK_LANE0_ACCESS "RO"
// =============================================================================
// Register    : SIO_INTERP1_PEEK_LANE1
// Description : Read LANE1 result, without altering any internal state (PEEK).
#define SIO_INTERP1_PEEK_LANE1_OFFSET _u(0x000000e4)
#define SIO_INTERP1_PEEK_LANE1_BITS   _u(0xffffffff)
#define SIO_INTERP1_PEEK_LANE1_RESET  _u(0x00000000)
#define SIO_INTERP1_PEEK_LANE1_MSB    _u(31)
#define SIO_INTERP1_PEEK_LANE1_LSB    _u(0)
#define SIO_INTERP1_PEEK_LANE1_ACCESS "RO"
// =============================================================================
// Register    : SIO_INTERP1_PEEK_FULL
// Description : Read FULL result, without altering any internal state (PEEK).
#define SIO_INTERP1_PEEK_FULL_OFFSET _u(0x000000e8)
#define SIO_INTERP1_PEEK_FULL_BITS   _u(0xffffffff)
#define SIO_INTERP1_PEEK_FULL_RESET  _u(0x00000000)
#define SIO_INTERP1_PEEK_FULL_MSB    _u(31)
#define SIO_INTERP1_PEEK_FULL_LSB    _u(0)
#define SIO_INTERP1_PEEK_FULL_ACCESS "RO"
// =============================================================================
// Register    : SIO_INTERP1_CTRL_LANE0
// Description : Control register for lane 0
#define SIO_INTERP1_CTRL_LANE0_OFFSET _u(0x000000ec)
#define SIO_INTERP1_CTRL_LANE0_BITS   _u(0x03dfffff)
#define SIO_INTERP1_CTRL_LANE0_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : SIO_INTERP1_CTRL_LANE0_OVERF
// Description : Set if either OVERF0 or OVERF1 is set.
#define SIO_INTERP1_CTRL_LANE0_OVERF_RESET  _u(0x0)
#define SIO_INTERP1_CTRL_LANE0_OVERF_BITS   _u(0x02000000)
#define SIO_INTERP1_CTRL_LANE0_OVERF_MSB    _u(25)
#define SIO_INTERP1_CTRL_LANE0_OVERF_LSB    _u(25)
#define SIO_INTERP1_CTRL_LANE0_OVERF_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : SIO_INTERP1_CTRL_LANE0_OVERF1
// Description : Indicates if any masked-off MSBs in ACCUM1 are set.
#define SIO_INTERP1_CTRL_LANE0_OVERF1_RESET  _u(0x0)
#define SIO_INTERP1_CTRL_LANE0_OVERF1_BITS   _u(0x01000000)
#define SIO_INTERP1_CTRL_LANE0_OVERF1_MSB    _u(24)
#define SIO_INTERP1_CTRL_LANE0_OVERF1_LSB    _u(24)
#define SIO_INTERP1_CTRL_LANE0_OVERF1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : SIO_INTERP1_CTRL_LANE0_OVERF0
// Description : Indicates if any masked-off MSBs in ACCUM0 are set.
#define SIO_INTERP1_CTRL_LANE0_OVERF0_RESET  _u(0x0)
#define SIO_INTERP1_CTRL_LANE0_OVERF0_BITS   _u(0x00800000)
#define SIO_INTERP1_CTRL_LANE0_OVERF0_MSB    _u(23)
#define SIO_INTERP1_CTRL_LANE0_OVERF0_LSB    _u(23)
#define SIO_INTERP1_CTRL_LANE0_OVERF0_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : SIO_INTERP1_CTRL_LANE0_CLAMP
// Description : Only present on INTERP1 on each core. If CLAMP mode is enabled:
//               - LANE0 result is shifted and masked ACCUM0, clamped by a lower
//               bound of
//               BASE0 and an upper bound of BASE1.
//               - Signedness of these comparisons is determined by
//               LANE0_CTRL_SIGNED
#define SIO_INTERP1_CTRL_LANE0_CLAMP_RESET  _u(0x0)
#define SIO_INTERP1_CTRL_LANE0_CLAMP_BITS   _u(0x00400000)
#define SIO_INTERP1_CTRL_LANE0_CLAMP_MSB    _u(22)
#define SIO_INTERP1_CTRL_LANE0_CLAMP_LSB    _u(22)
#define SIO_INTERP1_CTRL_LANE0_CLAMP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SIO_INTERP1_CTRL_LANE0_FORCE_MSB
// Description : ORed into bits 29:28 of the lane result presented to the
//               processor on the bus.
//               No effect on the internal 32-bit datapath. Handy for using a
//               lane to generate sequence
//               of pointers into flash or SRAM.
#define SIO_INTERP1_CTRL_LANE0_FORCE_MSB_RESET  _u(0x0)
#define SIO_INTERP1_CTRL_LANE0_FORCE_MSB_BITS   _u(0x00180000)
#define SIO_INTERP1_CTRL_LANE0_FORCE_MSB_MSB    _u(20)
#define SIO_INTERP1_CTRL_LANE0_FORCE_MSB_LSB    _u(19)
#define SIO_INTERP1_CTRL_LANE0_FORCE_MSB_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SIO_INTERP1_CTRL_LANE0_ADD_RAW
// Description : If 1, mask + shift is bypassed for LANE0 result. This does not
//               affect FULL result.
#define SIO_INTERP1_CTRL_LANE0_ADD_RAW_RESET  _u(0x0)
#define SIO_INTERP1_CTRL_LANE0_ADD_RAW_BITS   _u(0x00040000)
#define SIO_INTERP1_CTRL_LANE0_ADD_RAW_MSB    _u(18)
#define SIO_INTERP1_CTRL_LANE0_ADD_RAW_LSB    _u(18)
#define SIO_INTERP1_CTRL_LANE0_ADD_RAW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SIO_INTERP1_CTRL_LANE0_CROSS_RESULT
// Description : If 1, feed the opposite lane's result into this lane's
//               accumulator on POP.
#define SIO_INTERP1_CTRL_LANE0_CROSS_RESULT_RESET  _u(0x0)
#define SIO_INTERP1_CTRL_LANE0_CROSS_RESULT_BITS   _u(0x00020000)
#define SIO_INTERP1_CTRL_LANE0_CROSS_RESULT_MSB    _u(17)
#define SIO_INTERP1_CTRL_LANE0_CROSS_RESULT_LSB    _u(17)
#define SIO_INTERP1_CTRL_LANE0_CROSS_RESULT_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SIO_INTERP1_CTRL_LANE0_CROSS_INPUT
// Description : If 1, feed the opposite lane's accumulator into this lane's
//               shift + mask hardware.
//               Takes effect even if ADD_RAW is set (the CROSS_INPUT mux is
//               before the shift+mask bypass)
#define SIO_INTERP1_CTRL_LANE0_CROSS_INPUT_RESET  _u(0x0)
#define SIO_INTERP1_CTRL_LANE0_CROSS_INPUT_BITS   _u(0x00010000)
#define SIO_INTERP1_CTRL_LANE0_CROSS_INPUT_MSB    _u(16)
#define SIO_INTERP1_CTRL_LANE0_CROSS_INPUT_LSB    _u(16)
#define SIO_INTERP1_CTRL_LANE0_CROSS_INPUT_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SIO_INTERP1_CTRL_LANE0_SIGNED
// Description : If SIGNED is set, the shifted and masked accumulator value is
//               sign-extended to 32 bits
//               before adding to BASE0, and LANE0 PEEK/POP appear extended to
//               32 bits when read by processor.
#define SIO_INTERP1_CTRL_LANE0_SIGNED_RESET  _u(0x0)
#define SIO_INTERP1_CTRL_LANE0_SIGNED_BITS   _u(0x00008000)
#define SIO_INTERP1_CTRL_LANE0_SIGNED_MSB    _u(15)
#define SIO_INTERP1_CTRL_LANE0_SIGNED_LSB    _u(15)
#define SIO_INTERP1_CTRL_LANE0_SIGNED_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SIO_INTERP1_CTRL_LANE0_MASK_MSB
// Description : The most-significant bit allowed to pass by the mask
//               (inclusive)
//               Setting MSB < LSB may cause chip to turn inside-out
#define SIO_INTERP1_CTRL_LANE0_MASK_MSB_RESET  _u(0x00)
#define SIO_INTERP1_CTRL_LANE0_MASK_MSB_BITS   _u(0x00007c00)
#define SIO_INTERP1_CTRL_LANE0_MASK_MSB_MSB    _u(14)
#define SIO_INTERP1_CTRL_LANE0_MASK_MSB_LSB    _u(10)
#define SIO_INTERP1_CTRL_LANE0_MASK_MSB_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SIO_INTERP1_CTRL_LANE0_MASK_LSB
// Description : The least-significant bit allowed to pass by the mask
//               (inclusive)
#define SIO_INTERP1_CTRL_LANE0_MASK_LSB_RESET  _u(0x00)
#define SIO_INTERP1_CTRL_LANE0_MASK_LSB_BITS   _u(0x000003e0)
#define SIO_INTERP1_CTRL_LANE0_MASK_LSB_MSB    _u(9)
#define SIO_INTERP1_CTRL_LANE0_MASK_LSB_LSB    _u(5)
#define SIO_INTERP1_CTRL_LANE0_MASK_LSB_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SIO_INTERP1_CTRL_LANE0_SHIFT
// Description : Right-rotate applied to accumulator before masking. By
//               appropriately configuring the masks, left and right shifts can
//               be synthesised.
#define SIO_INTERP1_CTRL_LANE0_SHIFT_RESET  _u(0x00)
#define SIO_INTERP1_CTRL_LANE0_SHIFT_BITS   _u(0x0000001f)
#define SIO_INTERP1_CTRL_LANE0_SHIFT_MSB    _u(4)
#define SIO_INTERP1_CTRL_LANE0_SHIFT_LSB    _u(0)
#define SIO_INTERP1_CTRL_LANE0_SHIFT_ACCESS "RW"
// =============================================================================
// Register    : SIO_INTERP1_CTRL_LANE1
// Description : Control register for lane 1
#define SIO_INTERP1_CTRL_LANE1_OFFSET _u(0x000000f0)
#define SIO_INTERP1_CTRL_LANE1_BITS   _u(0x001fffff)
#define SIO_INTERP1_CTRL_LANE1_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : SIO_INTERP1_CTRL_LANE1_FORCE_MSB
// Description : ORed into bits 29:28 of the lane result presented to the
//               processor on the bus.
//               No effect on the internal 32-bit datapath. Handy for using a
//               lane to generate sequence
//               of pointers into flash or SRAM.
#define SIO_INTERP1_CTRL_LANE1_FORCE_MSB_RESET  _u(0x0)
#define SIO_INTERP1_CTRL_LANE1_FORCE_MSB_BITS   _u(0x00180000)
#define SIO_INTERP1_CTRL_LANE1_FORCE_MSB_MSB    _u(20)
#define SIO_INTERP1_CTRL_LANE1_FORCE_MSB_LSB    _u(19)
#define SIO_INTERP1_CTRL_LANE1_FORCE_MSB_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SIO_INTERP1_CTRL_LANE1_ADD_RAW
// Description : If 1, mask + shift is bypassed for LANE1 result. This does not
//               affect FULL result.
#define SIO_INTERP1_CTRL_LANE1_ADD_RAW_RESET  _u(0x0)
#define SIO_INTERP1_CTRL_LANE1_ADD_RAW_BITS   _u(0x00040000)
#define SIO_INTERP1_CTRL_LANE1_ADD_RAW_MSB    _u(18)
#define SIO_INTERP1_CTRL_LANE1_ADD_RAW_LSB    _u(18)
#define SIO_INTERP1_CTRL_LANE1_ADD_RAW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SIO_INTERP1_CTRL_LANE1_CROSS_RESULT
// Description : If 1, feed the opposite lane's result into this lane's
//               accumulator on POP.
#define SIO_INTERP1_CTRL_LANE1_CROSS_RESULT_RESET  _u(0x0)
#define SIO_INTERP1_CTRL_LANE1_CROSS_RESULT_BITS   _u(0x00020000)
#define SIO_INTERP1_CTRL_LANE1_CROSS_RESULT_MSB    _u(17)
#define SIO_INTERP1_CTRL_LANE1_CROSS_RESULT_LSB    _u(17)
#define SIO_INTERP1_CTRL_LANE1_CROSS_RESULT_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SIO_INTERP1_CTRL_LANE1_CROSS_INPUT
// Description : If 1, feed the opposite lane's accumulator into this lane's
//               shift + mask hardware.
//               Takes effect even if ADD_RAW is set (the CROSS_INPUT mux is
//               before the shift+mask bypass)
#define SIO_INTERP1_CTRL_LANE1_CROSS_INPUT_RESET  _u(0x0)
#define SIO_INTERP1_CTRL_LANE1_CROSS_INPUT_BITS   _u(0x00010000)
#define SIO_INTERP1_CTRL_LANE1_CROSS_INPUT_MSB    _u(16)
#define SIO_INTERP1_CTRL_LANE1_CROSS_INPUT_LSB    _u(16)
#define SIO_INTERP1_CTRL_LANE1_CROSS_INPUT_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SIO_INTERP1_CTRL_LANE1_SIGNED
// Description : If SIGNED is set, the shifted and masked accumulator value is
//               sign-extended to 32 bits
//               before adding to BASE1, and LANE1 PEEK/POP appear extended to
//               32 bits when read by processor.
#define SIO_INTERP1_CTRL_LANE1_SIGNED_RESET  _u(0x0)
#define SIO_INTERP1_CTRL_LANE1_SIGNED_BITS   _u(0x00008000)
#define SIO_INTERP1_CTRL_LANE1_SIGNED_MSB    _u(15)
#define SIO_INTERP1_CTRL_LANE1_SIGNED_LSB    _u(15)
#define SIO_INTERP1_CTRL_LANE1_SIGNED_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SIO_INTERP1_CTRL_LANE1_MASK_MSB
// Description : The most-significant bit allowed to pass by the mask
//               (inclusive)
//               Setting MSB < LSB may cause chip to turn inside-out
#define SIO_INTERP1_CTRL_LANE1_MASK_MSB_RESET  _u(0x00)
#define SIO_INTERP1_CTRL_LANE1_MASK_MSB_BITS   _u(0x00007c00)
#define SIO_INTERP1_CTRL_LANE1_MASK_MSB_MSB    _u(14)
#define SIO_INTERP1_CTRL_LANE1_MASK_MSB_LSB    _u(10)
#define SIO_INTERP1_CTRL_LANE1_MASK_MSB_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SIO_INTERP1_CTRL_LANE1_MASK_LSB
// Description : The least-significant bit allowed to pass by the mask
//               (inclusive)
#define SIO_INTERP1_CTRL_LANE1_MASK_LSB_RESET  _u(0x00)
#define SIO_INTERP1_CTRL_LANE1_MASK_LSB_BITS   _u(0x000003e0)
#define SIO_INTERP1_CTRL_LANE1_MASK_LSB_MSB    _u(9)
#define SIO_INTERP1_CTRL_LANE1_MASK_LSB_LSB    _u(5)
#define SIO_INTERP1_CTRL_LANE1_MASK_LSB_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SIO_INTERP1_CTRL_LANE1_SHIFT
// Description : Right-rotate applied to accumulator before masking. By
//               appropriately configuring the masks, left and right shifts can
//               be synthesised.
#define SIO_INTERP1_CTRL_LANE1_SHIFT_RESET  _u(0x00)
#define SIO_INTERP1_CTRL_LANE1_SHIFT_BITS   _u(0x0000001f)
#define SIO_INTERP1_CTRL_LANE1_SHIFT_MSB    _u(4)
#define SIO_INTERP1_CTRL_LANE1_SHIFT_LSB    _u(0)
#define SIO_INTERP1_CTRL_LANE1_SHIFT_ACCESS "RW"
// =============================================================================
// Register    : SIO_INTERP1_ACCUM0_ADD
// Description : Values written here are atomically added to ACCUM0
//               Reading yields lane 0's raw shift and mask value (BASE0 not
//               added).
#define SIO_INTERP1_ACCUM0_ADD_OFFSET _u(0x000000f4)
#define SIO_INTERP1_ACCUM0_ADD_BITS   _u(0x00ffffff)
#define SIO_INTERP1_ACCUM0_ADD_RESET  _u(0x00000000)
#define SIO_INTERP1_ACCUM0_ADD_MSB    _u(23)
#define SIO_INTERP1_ACCUM0_ADD_LSB    _u(0)
#define SIO_INTERP1_ACCUM0_ADD_ACCESS "RW"
// =============================================================================
// Register    : SIO_INTERP1_ACCUM1_ADD
// Description : Values written here are atomically added to ACCUM1
//               Reading yields lane 1's raw shift and mask value (BASE1 not
//               added).
#define SIO_INTERP1_ACCUM1_ADD_OFFSET _u(0x000000f8)
#define SIO_INTERP1_ACCUM1_ADD_BITS   _u(0x00ffffff)
#define SIO_INTERP1_ACCUM1_ADD_RESET  _u(0x00000000)
#define SIO_INTERP1_ACCUM1_ADD_MSB    _u(23)
#define SIO_INTERP1_ACCUM1_ADD_LSB    _u(0)
#define SIO_INTERP1_ACCUM1_ADD_ACCESS "RW"
// =============================================================================
// Register    : SIO_INTERP1_BASE_1AND0
// Description : On write, the lower 16 bits go to BASE0, upper bits to BASE1
//               simultaneously.
//               Each half is sign-extended to 32 bits if that lane's SIGNED
//               flag is set.
#define SIO_INTERP1_BASE_1AND0_OFFSET _u(0x000000fc)
#define SIO_INTERP1_BASE_1AND0_BITS   _u(0xffffffff)
#define SIO_INTERP1_BASE_1AND0_RESET  _u(0x00000000)
#define SIO_INTERP1_BASE_1AND0_MSB    _u(31)
#define SIO_INTERP1_BASE_1AND0_LSB    _u(0)
#define SIO_INTERP1_BASE_1AND0_ACCESS "WO"
// =============================================================================
// Register    : SIO_SPINLOCK0
// Description : Reading from a spinlock address will:
//               - Return 0 if lock is already locked
//               - Otherwise return nonzero, and simultaneously claim the lock
//
//               Writing (any value) releases the lock.
//               If core 0 and core 1 attempt to claim the same lock
//               simultaneously, core 0 wins.
//               The value returned on success is 0x1 << lock number.
#define SIO_SPINLOCK0_OFFSET _u(0x00000100)
#define SIO_SPINLOCK0_BITS   _u(0xffffffff)
#define SIO_SPINLOCK0_RESET  _u(0x00000000)
#define SIO_SPINLOCK0_MSB    _u(31)
#define SIO_SPINLOCK0_LSB    _u(0)
#define SIO_SPINLOCK0_ACCESS "RW"
// =============================================================================
// Register    : SIO_SPINLOCK1
// Description : Reading from a spinlock address will:
//               - Return 0 if lock is already locked
//               - Otherwise return nonzero, and simultaneously claim the lock
//
//               Writing (any value) releases the lock.
//               If core 0 and core 1 attempt to claim the same lock
//               simultaneously, core 0 wins.
//               The value returned on success is 0x1 << lock number.
#define SIO_SPINLOCK1_OFFSET _u(0x00000104)
#define SIO_SPINLOCK1_BITS   _u(0xffffffff)
#define SIO_SPINLOCK1_RESET  _u(0x00000000)
#define SIO_SPINLOCK1_MSB    _u(31)
#define SIO_SPINLOCK1_LSB    _u(0)
#define SIO_SPINLOCK1_ACCESS "RW"
// =============================================================================
// Register    : SIO_SPINLOCK2
// Description : Reading from a spinlock address will:
//               - Return 0 if lock is already locked
//               - Otherwise return nonzero, and simultaneously claim the lock
//
//               Writing (any value) releases the lock.
//               If core 0 and core 1 attempt to claim the same lock
//               simultaneously, core 0 wins.
//               The value returned on success is 0x1 << lock number.
#define SIO_SPINLOCK2_OFFSET _u(0x00000108)
#define SIO_SPINLOCK2_BITS   _u(0xffffffff)
#define SIO_SPINLOCK2_RESET  _u(0x00000000)
#define SIO_SPINLOCK2_MSB    _u(31)
#define SIO_SPINLOCK2_LSB    _u(0)
#define SIO_SPINLOCK2_ACCESS "RW"
// =============================================================================
// Register    : SIO_SPINLOCK3
// Description : Reading from a spinlock address will:
//               - Return 0 if lock is already locked
//               - Otherwise return nonzero, and simultaneously claim the lock
//
//               Writing (any value) releases the lock.
//               If core 0 and core 1 attempt to claim the same lock
//               simultaneously, core 0 wins.
//               The value returned on success is 0x1 << lock number.
#define SIO_SPINLOCK3_OFFSET _u(0x0000010c)
#define SIO_SPINLOCK3_BITS   _u(0xffffffff)
#define SIO_SPINLOCK3_RESET  _u(0x00000000)
#define SIO_SPINLOCK3_MSB    _u(31)
#define SIO_SPINLOCK3_LSB    _u(0)
#define SIO_SPINLOCK3_ACCESS "RW"
// =============================================================================
// Register    : SIO_SPINLOCK4
// Description : Reading from a spinlock address will:
//               - Return 0 if lock is already locked
//               - Otherwise return nonzero, and simultaneously claim the lock
//
//               Writing (any value) releases the lock.
//               If core 0 and core 1 attempt to claim the same lock
//               simultaneously, core 0 wins.
//               The value returned on success is 0x1 << lock number.
#define SIO_SPINLOCK4_OFFSET _u(0x00000110)
#define SIO_SPINLOCK4_BITS   _u(0xffffffff)
#define SIO_SPINLOCK4_RESET  _u(0x00000000)
#define SIO_SPINLOCK4_MSB    _u(31)
#define SIO_SPINLOCK4_LSB    _u(0)
#define SIO_SPINLOCK4_ACCESS "RW"
// =============================================================================
// Register    : SIO_SPINLOCK5
// Description : Reading from a spinlock address will:
//               - Return 0 if lock is already locked
//               - Otherwise return nonzero, and simultaneously claim the lock
//
//               Writing (any value) releases the lock.
//               If core 0 and core 1 attempt to claim the same lock
//               simultaneously, core 0 wins.
//               The value returned on success is 0x1 << lock number.
#define SIO_SPINLOCK5_OFFSET _u(0x00000114)
#define SIO_SPINLOCK5_BITS   _u(0xffffffff)
#define SIO_SPINLOCK5_RESET  _u(0x00000000)
#define SIO_SPINLOCK5_MSB    _u(31)
#define SIO_SPINLOCK5_LSB    _u(0)
#define SIO_SPINLOCK5_ACCESS "RW"
// =============================================================================
// Register    : SIO_SPINLOCK6
// Description : Reading from a spinlock address will:
//               - Return 0 if lock is already locked
//               - Otherwise return nonzero, and simultaneously claim the lock
//
//               Writing (any value) releases the lock.
//               If core 0 and core 1 attempt to claim the same lock
//               simultaneously, core 0 wins.
//               The value returned on success is 0x1 << lock number.
#define SIO_SPINLOCK6_OFFSET _u(0x00000118)
#define SIO_SPINLOCK6_BITS   _u(0xffffffff)
#define SIO_SPINLOCK6_RESET  _u(0x00000000)
#define SIO_SPINLOCK6_MSB    _u(31)
#define SIO_SPINLOCK6_LSB    _u(0)
#define SIO_SPINLOCK6_ACCESS "RW"
// =============================================================================
// Register    : SIO_SPINLOCK7
// Description : Reading from a spinlock address will:
//               - Return 0 if lock is already locked
//               - Otherwise return nonzero, and simultaneously claim the lock
//
//               Writing (any value) releases the lock.
//               If core 0 and core 1 attempt to claim the same lock
//               simultaneously, core 0 wins.
//               The value returned on success is 0x1 << lock number.
#define SIO_SPINLOCK7_OFFSET _u(0x0000011c)
#define SIO_SPINLOCK7_BITS   _u(0xffffffff)
#define SIO_SPINLOCK7_RESET  _u(0x00000000)
#define SIO_SPINLOCK7_MSB    _u(31)
#define SIO_SPINLOCK7_LSB    _u(0)
#define SIO_SPINLOCK7_ACCESS "RW"
// =============================================================================
// Register    : SIO_SPINLOCK8
// Description : Reading from a spinlock address will:
//               - Return 0 if lock is already locked
//               - Otherwise return nonzero, and simultaneously claim the lock
//
//               Writing (any value) releases the lock.
//               If core 0 and core 1 attempt to claim the same lock
//               simultaneously, core 0 wins.
//               The value returned on success is 0x1 << lock number.
#define SIO_SPINLOCK8_OFFSET _u(0x00000120)
#define SIO_SPINLOCK8_BITS   _u(0xffffffff)
#define SIO_SPINLOCK8_RESET  _u(0x00000000)
#define SIO_SPINLOCK8_MSB    _u(31)
#define SIO_SPINLOCK8_LSB    _u(0)
#define SIO_SPINLOCK8_ACCESS "RW"
// =============================================================================
// Register    : SIO_SPINLOCK9
// Description : Reading from a spinlock address will:
//               - Return 0 if lock is already locked
//               - Otherwise return nonzero, and simultaneously claim the lock
//
//               Writing (any value) releases the lock.
//               If core 0 and core 1 attempt to claim the same lock
//               simultaneously, core 0 wins.
//               The value returned on success is 0x1 << lock number.
#define SIO_SPINLOCK9_OFFSET _u(0x00000124)
#define SIO_SPINLOCK9_BITS   _u(0xffffffff)
#define SIO_SPINLOCK9_RESET  _u(0x00000000)
#define SIO_SPINLOCK9_MSB    _u(31)
#define SIO_SPINLOCK9_LSB    _u(0)
#define SIO_SPINLOCK9_ACCESS "RW"
// =============================================================================
// Register    : SIO_SPINLOCK10
// Description : Reading from a spinlock address will:
//               - Return 0 if lock is already locked
//               - Otherwise return nonzero, and simultaneously claim the lock
//
//               Writing (any value) releases the lock.
//               If core 0 and core 1 attempt to claim the same lock
//               simultaneously, core 0 wins.
//               The value returned on success is 0x1 << lock number.
#define SIO_SPINLOCK10_OFFSET _u(0x00000128)
#define SIO_SPINLOCK10_BITS   _u(0xffffffff)
#define SIO_SPINLOCK10_RESET  _u(0x00000000)
#define SIO_SPINLOCK10_MSB    _u(31)
#define SIO_SPINLOCK10_LSB    _u(0)
#define SIO_SPINLOCK10_ACCESS "RW"
// =============================================================================
// Register    : SIO_SPINLOCK11
// Description : Reading from a spinlock address will:
//               - Return 0 if lock is already locked
//               - Otherwise return nonzero, and simultaneously claim the lock
//
//               Writing (any value) releases the lock.
//               If core 0 and core 1 attempt to claim the same lock
//               simultaneously, core 0 wins.
//               The value returned on success is 0x1 << lock number.
#define SIO_SPINLOCK11_OFFSET _u(0x0000012c)
#define SIO_SPINLOCK11_BITS   _u(0xffffffff)
#define SIO_SPINLOCK11_RESET  _u(0x00000000)
#define SIO_SPINLOCK11_MSB    _u(31)
#define SIO_SPINLOCK11_LSB    _u(0)
#define SIO_SPINLOCK11_ACCESS "RW"
// =============================================================================
// Register    : SIO_SPINLOCK12
// Description : Reading from a spinlock address will:
//               - Return 0 if lock is already locked
//               - Otherwise return nonzero, and simultaneously claim the lock
//
//               Writing (any value) releases the lock.
//               If core 0 and core 1 attempt to claim the same lock
//               simultaneously, core 0 wins.
//               The value returned on success is 0x1 << lock number.
#define SIO_SPINLOCK12_OFFSET _u(0x00000130)
#define SIO_SPINLOCK12_BITS   _u(0xffffffff)
#define SIO_SPINLOCK12_RESET  _u(0x00000000)
#define SIO_SPINLOCK12_MSB    _u(31)
#define SIO_SPINLOCK12_LSB    _u(0)
#define SIO_SPINLOCK12_ACCESS "RW"
// =============================================================================
// Register    : SIO_SPINLOCK13
// Description : Reading from a spinlock address will:
//               - Return 0 if lock is already locked
//               - Otherwise return nonzero, and simultaneously claim the lock
//
//               Writing (any value) releases the lock.
//               If core 0 and core 1 attempt to claim the same lock
//               simultaneously, core 0 wins.
//               The value returned on success is 0x1 << lock number.
#define SIO_SPINLOCK13_OFFSET _u(0x00000134)
#define SIO_SPINLOCK13_BITS   _u(0xffffffff)
#define SIO_SPINLOCK13_RESET  _u(0x00000000)
#define SIO_SPINLOCK13_MSB    _u(31)
#define SIO_SPINLOCK13_LSB    _u(0)
#define SIO_SPINLOCK13_ACCESS "RW"
// =============================================================================
// Register    : SIO_SPINLOCK14
// Description : Reading from a spinlock address will:
//               - Return 0 if lock is already locked
//               - Otherwise return nonzero, and simultaneously claim the lock
//
//               Writing (any value) releases the lock.
//               If core 0 and core 1 attempt to claim the same lock
//               simultaneously, core 0 wins.
//               The value returned on success is 0x1 << lock number.
#define SIO_SPINLOCK14_OFFSET _u(0x00000138)
#define SIO_SPINLOCK14_BITS   _u(0xffffffff)
#define SIO_SPINLOCK14_RESET  _u(0x00000000)
#define SIO_SPINLOCK14_MSB    _u(31)
#define SIO_SPINLOCK14_LSB    _u(0)
#define SIO_SPINLOCK14_ACCESS "RW"
// =============================================================================
// Register    : SIO_SPINLOCK15
// Description : Reading from a spinlock address will:
//               - Return 0 if lock is already locked
//               - Otherwise return nonzero, and simultaneously claim the lock
//
//               Writing (any value) releases the lock.
//               If core 0 and core 1 attempt to claim the same lock
//               simultaneously, core 0 wins.
//               The value returned on success is 0x1 << lock number.
#define SIO_SPINLOCK15_OFFSET _u(0x0000013c)
#define SIO_SPINLOCK15_BITS   _u(0xffffffff)
#define SIO_SPINLOCK15_RESET  _u(0x00000000)
#define SIO_SPINLOCK15_MSB    _u(31)
#define SIO_SPINLOCK15_LSB    _u(0)
#define SIO_SPINLOCK15_ACCESS "RW"
// =============================================================================
// Register    : SIO_SPINLOCK16
// Description : Reading from a spinlock address will:
//               - Return 0 if lock is already locked
//               - Otherwise return nonzero, and simultaneously claim the lock
//
//               Writing (any value) releases the lock.
//               If core 0 and core 1 attempt to claim the same lock
//               simultaneously, core 0 wins.
//               The value returned on success is 0x1 << lock number.
#define SIO_SPINLOCK16_OFFSET _u(0x00000140)
#define SIO_SPINLOCK16_BITS   _u(0xffffffff)
#define SIO_SPINLOCK16_RESET  _u(0x00000000)
#define SIO_SPINLOCK16_MSB    _u(31)
#define SIO_SPINLOCK16_LSB    _u(0)
#define SIO_SPINLOCK16_ACCESS "RW"
// =============================================================================
// Register    : SIO_SPINLOCK17
// Description : Reading from a spinlock address will:
//               - Return 0 if lock is already locked
//               - Otherwise return nonzero, and simultaneously claim the lock
//
//               Writing (any value) releases the lock.
//               If core 0 and core 1 attempt to claim the same lock
//               simultaneously, core 0 wins.
//               The value returned on success is 0x1 << lock number.
#define SIO_SPINLOCK17_OFFSET _u(0x00000144)
#define SIO_SPINLOCK17_BITS   _u(0xffffffff)
#define SIO_SPINLOCK17_RESET  _u(0x00000000)
#define SIO_SPINLOCK17_MSB    _u(31)
#define SIO_SPINLOCK17_LSB    _u(0)
#define SIO_SPINLOCK17_ACCESS "RW"
// =============================================================================
// Register    : SIO_SPINLOCK18
// Description : Reading from a spinlock address will:
//               - Return 0 if lock is already locked
//               - Otherwise return nonzero, and simultaneously claim the lock
//
//               Writing (any value) releases the lock.
//               If core 0 and core 1 attempt to claim the same lock
//               simultaneously, core 0 wins.
//               The value returned on success is 0x1 << lock number.
#define SIO_SPINLOCK18_OFFSET _u(0x00000148)
#define SIO_SPINLOCK18_BITS   _u(0xffffffff)
#define SIO_SPINLOCK18_RESET  _u(0x00000000)
#define SIO_SPINLOCK18_MSB    _u(31)
#define SIO_SPINLOCK18_LSB    _u(0)
#define SIO_SPINLOCK18_ACCESS "RW"
// =============================================================================
// Register    : SIO_SPINLOCK19
// Description : Reading from a spinlock address will:
//               - Return 0 if lock is already locked
//               - Otherwise return nonzero, and simultaneously claim the lock
//
//               Writing (any value) releases the lock.
//               If core 0 and core 1 attempt to claim the same lock
//               simultaneously, core 0 wins.
//               The value returned on success is 0x1 << lock number.
#define SIO_SPINLOCK19_OFFSET _u(0x0000014c)
#define SIO_SPINLOCK19_BITS   _u(0xffffffff)
#define SIO_SPINLOCK19_RESET  _u(0x00000000)
#define SIO_SPINLOCK19_MSB    _u(31)
#define SIO_SPINLOCK19_LSB    _u(0)
#define SIO_SPINLOCK19_ACCESS "RW"
// =============================================================================
// Register    : SIO_SPINLOCK20
// Description : Reading from a spinlock address will:
//               - Return 0 if lock is already locked
//               - Otherwise return nonzero, and simultaneously claim the lock
//
//               Writing (any value) releases the lock.
//               If core 0 and core 1 attempt to claim the same lock
//               simultaneously, core 0 wins.
//               The value returned on success is 0x1 << lock number.
#define SIO_SPINLOCK20_OFFSET _u(0x00000150)
#define SIO_SPINLOCK20_BITS   _u(0xffffffff)
#define SIO_SPINLOCK20_RESET  _u(0x00000000)
#define SIO_SPINLOCK20_MSB    _u(31)
#define SIO_SPINLOCK20_LSB    _u(0)
#define SIO_SPINLOCK20_ACCESS "RW"
// =============================================================================
// Register    : SIO_SPINLOCK21
// Description : Reading from a spinlock address will:
//               - Return 0 if lock is already locked
//               - Otherwise return nonzero, and simultaneously claim the lock
//
//               Writing (any value) releases the lock.
//               If core 0 and core 1 attempt to claim the same lock
//               simultaneously, core 0 wins.
//               The value returned on success is 0x1 << lock number.
#define SIO_SPINLOCK21_OFFSET _u(0x00000154)
#define SIO_SPINLOCK21_BITS   _u(0xffffffff)
#define SIO_SPINLOCK21_RESET  _u(0x00000000)
#define SIO_SPINLOCK21_MSB    _u(31)
#define SIO_SPINLOCK21_LSB    _u(0)
#define SIO_SPINLOCK21_ACCESS "RW"
// =============================================================================
// Register    : SIO_SPINLOCK22
// Description : Reading from a spinlock address will:
//               - Return 0 if lock is already locked
//               - Otherwise return nonzero, and simultaneously claim the lock
//
//               Writing (any value) releases the lock.
//               If core 0 and core 1 attempt to claim the same lock
//               simultaneously, core 0 wins.
//               The value returned on success is 0x1 << lock number.
#define SIO_SPINLOCK22_OFFSET _u(0x00000158)
#define SIO_SPINLOCK22_BITS   _u(0xffffffff)
#define SIO_SPINLOCK22_RESET  _u(0x00000000)
#define SIO_SPINLOCK22_MSB    _u(31)
#define SIO_SPINLOCK22_LSB    _u(0)
#define SIO_SPINLOCK22_ACCESS "RW"
// =============================================================================
// Register    : SIO_SPINLOCK23
// Description : Reading from a spinlock address will:
//               - Return 0 if lock is already locked
//               - Otherwise return nonzero, and simultaneously claim the lock
//
//               Writing (any value) releases the lock.
//               If core 0 and core 1 attempt to claim the same lock
//               simultaneously, core 0 wins.
//               The value returned on success is 0x1 << lock number.
#define SIO_SPINLOCK23_OFFSET _u(0x0000015c)
#define SIO_SPINLOCK23_BITS   _u(0xffffffff)
#define SIO_SPINLOCK23_RESET  _u(0x00000000)
#define SIO_SPINLOCK23_MSB    _u(31)
#define SIO_SPINLOCK23_LSB    _u(0)
#define SIO_SPINLOCK23_ACCESS "RW"
// =============================================================================
// Register    : SIO_SPINLOCK24
// Description : Reading from a spinlock address will:
//               - Return 0 if lock is already locked
//               - Otherwise return nonzero, and simultaneously claim the lock
//
//               Writing (any value) releases the lock.
//               If core 0 and core 1 attempt to claim the same lock
//               simultaneously, core 0 wins.
//               The value returned on success is 0x1 << lock number.
#define SIO_SPINLOCK24_OFFSET _u(0x00000160)
#define SIO_SPINLOCK24_BITS   _u(0xffffffff)
#define SIO_SPINLOCK24_RESET  _u(0x00000000)
#define SIO_SPINLOCK24_MSB    _u(31)
#define SIO_SPINLOCK24_LSB    _u(0)
#define SIO_SPINLOCK24_ACCESS "RW"
// =============================================================================
// Register    : SIO_SPINLOCK25
// Description : Reading from a spinlock address will:
//               - Return 0 if lock is already locked
//               - Otherwise return nonzero, and simultaneously claim the lock
//
//               Writing (any value) releases the lock.
//               If core 0 and core 1 attempt to claim the same lock
//               simultaneously, core 0 wins.
//               The value returned on success is 0x1 << lock number.
#define SIO_SPINLOCK25_OFFSET _u(0x00000164)
#define SIO_SPINLOCK25_BITS   _u(0xffffffff)
#define SIO_SPINLOCK25_RESET  _u(0x00000000)
#define SIO_SPINLOCK25_MSB    _u(31)
#define SIO_SPINLOCK25_LSB    _u(0)
#define SIO_SPINLOCK25_ACCESS "RW"
// =============================================================================
// Register    : SIO_SPINLOCK26
// Description : Reading from a spinlock address will:
//               - Return 0 if lock is already locked
//               - Otherwise return nonzero, and simultaneously claim the lock
//
//               Writing (any value) releases the lock.
//               If core 0 and core 1 attempt to claim the same lock
//               simultaneously, core 0 wins.
//               The value returned on success is 0x1 << lock number.
#define SIO_SPINLOCK26_OFFSET _u(0x00000168)
#define SIO_SPINLOCK26_BITS   _u(0xffffffff)
#define SIO_SPINLOCK26_RESET  _u(0x00000000)
#define SIO_SPINLOCK26_MSB    _u(31)
#define SIO_SPINLOCK26_LSB    _u(0)
#define SIO_SPINLOCK26_ACCESS "RW"
// =============================================================================
// Register    : SIO_SPINLOCK27
// Description : Reading from a spinlock address will:
//               - Return 0 if lock is already locked
//               - Otherwise return nonzero, and simultaneously claim the lock
//
//               Writing (any value) releases the lock.
//               If core 0 and core 1 attempt to claim the same lock
//               simultaneously, core 0 wins.
//               The value returned on success is 0x1 << lock number.
#define SIO_SPINLOCK27_OFFSET _u(0x0000016c)
#define SIO_SPINLOCK27_BITS   _u(0xffffffff)
#define SIO_SPINLOCK27_RESET  _u(0x00000000)
#define SIO_SPINLOCK27_MSB    _u(31)
#define SIO_SPINLOCK27_LSB    _u(0)
#define SIO_SPINLOCK27_ACCESS "RW"
// =============================================================================
// Register    : SIO_SPINLOCK28
// Description : Reading from a spinlock address will:
//               - Return 0 if lock is already locked
//               - Otherwise return nonzero, and simultaneously claim the lock
//
//               Writing (any value) releases the lock.
//               If core 0 and core 1 attempt to claim the same lock
//               simultaneously, core 0 wins.
//               The value returned on success is 0x1 << lock number.
#define SIO_SPINLOCK28_OFFSET _u(0x00000170)
#define SIO_SPINLOCK28_BITS   _u(0xffffffff)
#define SIO_SPINLOCK28_RESET  _u(0x00000000)
#define SIO_SPINLOCK28_MSB    _u(31)
#define SIO_SPINLOCK28_LSB    _u(0)
#define SIO_SPINLOCK28_ACCESS "RW"
// =============================================================================
// Register    : SIO_SPINLOCK29
// Description : Reading from a spinlock address will:
//               - Return 0 if lock is already locked
//               - Otherwise return nonzero, and simultaneously claim the lock
//
//               Writing (any value) releases the lock.
//               If core 0 and core 1 attempt to claim the same lock
//               simultaneously, core 0 wins.
//               The value returned on success is 0x1 << lock number.
#define SIO_SPINLOCK29_OFFSET _u(0x00000174)
#define SIO_SPINLOCK29_BITS   _u(0xffffffff)
#define SIO_SPINLOCK29_RESET  _u(0x00000000)
#define SIO_SPINLOCK29_MSB    _u(31)
#define SIO_SPINLOCK29_LSB    _u(0)
#define SIO_SPINLOCK29_ACCESS "RW"
// =============================================================================
// Register    : SIO_SPINLOCK30
// Description : Reading from a spinlock address will:
//               - Return 0 if lock is already locked
//               - Otherwise return nonzero, and simultaneously claim the lock
//
//               Writing (any value) releases the lock.
//               If core 0 and core 1 attempt to claim the same lock
//               simultaneously, core 0 wins.
//               The value returned on success is 0x1 << lock number.
#define SIO_SPINLOCK30_OFFSET _u(0x00000178)
#define SIO_SPINLOCK30_BITS   _u(0xffffffff)
#define SIO_SPINLOCK30_RESET  _u(0x00000000)
#define SIO_SPINLOCK30_MSB    _u(31)
#define SIO_SPINLOCK30_LSB    _u(0)
#define SIO_SPINLOCK30_ACCESS "RW"
// =============================================================================
// Register    : SIO_SPINLOCK31
// Description : Reading from a spinlock address will:
//               - Return 0 if lock is already locked
//               - Otherwise return nonzero, and simultaneously claim the lock
//
//               Writing (any value) releases the lock.
//               If core 0 and core 1 attempt to claim the same lock
//               simultaneously, core 0 wins.
//               The value returned on success is 0x1 << lock number.
#define SIO_SPINLOCK31_OFFSET _u(0x0000017c)
#define SIO_SPINLOCK31_BITS   _u(0xffffffff)
#define SIO_SPINLOCK31_RESET  _u(0x00000000)
#define SIO_SPINLOCK31_MSB    _u(31)
#define SIO_SPINLOCK31_LSB    _u(0)
#define SIO_SPINLOCK31_ACCESS "RW"
// =============================================================================
// Register    : SIO_DOORBELL_OUT_SET
// Description : Trigger a doorbell interrupt on the opposite core.
//
//               Write 1 to a bit to set the corresponding bit in DOORBELL_IN on
//               the opposite core. This raises the opposite core's doorbell
//               interrupt.
//
//               Read to get the status of the doorbells currently asserted on
//               the opposite core. This is equivalent to that core reading its
//               own DOORBELL_IN status.
#define SIO_DOORBELL_OUT_SET_OFFSET _u(0x00000180)
#define SIO_DOORBELL_OUT_SET_BITS   _u(0x000000ff)
#define SIO_DOORBELL_OUT_SET_RESET  _u(0x00000000)
#define SIO_DOORBELL_OUT_SET_MSB    _u(7)
#define SIO_DOORBELL_OUT_SET_LSB    _u(0)
#define SIO_DOORBELL_OUT_SET_ACCESS "RW"
// =============================================================================
// Register    : SIO_DOORBELL_OUT_CLR
// Description : Clear doorbells which have been posted to the opposite core.
//               This register is intended for debugging and initialisation
//               purposes.
//
//               Writing 1 to a bit in DOORBELL_OUT_CLR clears the corresponding
//               bit in DOORBELL_IN on the opposite core. Clearing all bits will
//               cause that core's doorbell interrupt to deassert. Since the
//               usual order of events is for software to send events using
//               DOORBELL_OUT_SET, and acknowledge incoming events by writing to
//               DOORBELL_IN_CLR, this register should be used with caution to
//               avoid race conditions.
//
//               Reading returns the status of the doorbells currently asserted
//               on the other core, i.e. is equivalent to that core reading its
//               own DOORBELL_IN status.
#define SIO_DOORBELL_OUT_CLR_OFFSET _u(0x00000184)
#define SIO_DOORBELL_OUT_CLR_BITS   _u(0x000000ff)
#define SIO_DOORBELL_OUT_CLR_RESET  _u(0x00000000)
#define SIO_DOORBELL_OUT_CLR_MSB    _u(7)
#define SIO_DOORBELL_OUT_CLR_LSB    _u(0)
#define SIO_DOORBELL_OUT_CLR_ACCESS "WC"
// =============================================================================
// Register    : SIO_DOORBELL_IN_SET
// Description : Write 1s to trigger doorbell interrupts on this core. Read to
//               get status of doorbells currently asserted on this core.
#define SIO_DOORBELL_IN_SET_OFFSET _u(0x00000188)
#define SIO_DOORBELL_IN_SET_BITS   _u(0x000000ff)
#define SIO_DOORBELL_IN_SET_RESET  _u(0x00000000)
#define SIO_DOORBELL_IN_SET_MSB    _u(7)
#define SIO_DOORBELL_IN_SET_LSB    _u(0)
#define SIO_DOORBELL_IN_SET_ACCESS "RW"
// =============================================================================
// Register    : SIO_DOORBELL_IN_CLR
// Description : Check and acknowledge doorbells posted to this core. This
//               core's doorbell interrupt is asserted when any bit in this
//               register is 1.
//
//               Write 1 to each bit to clear that bit. The doorbell interrupt
//               deasserts once all bits are cleared. Read to get status of
//               doorbells currently asserted on this core.
#define SIO_DOORBELL_IN_CLR_OFFSET _u(0x0000018c)
#define SIO_DOORBELL_IN_CLR_BITS   _u(0x000000ff)
#define SIO_DOORBELL_IN_CLR_RESET  _u(0x00000000)
#define SIO_DOORBELL_IN_CLR_MSB    _u(7)
#define SIO_DOORBELL_IN_CLR_LSB    _u(0)
#define SIO_DOORBELL_IN_CLR_ACCESS "WC"
// =============================================================================
// Register    : SIO_PERI_NONSEC
// Description : Detach certain core-local peripherals from Secure SIO, and
//               attach them to Non-secure SIO, so that Non-secure software can
//               use them. Attempting to access one of these peripherals from
//               the Secure SIO when it is attached to the Non-secure SIO, or
//               vice versa, will generate a bus error.
//
//               This register is per-core, and is only present on the Secure
//               SIO.
//
//               Most SIO hardware is duplicated across the Secure and Non-
//               secure SIO, so is not listed in this register.
#define SIO_PERI_NONSEC_OFFSET _u(0x00000190)
#define SIO_PERI_NONSEC_BITS   _u(0x00000023)
#define SIO_PERI_NONSEC_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : SIO_PERI_NONSEC_TMDS
// Description : IF 1, detach TMDS encoder (of this core) from the Secure SIO,
//               and attach to the Non-secure SIO.
#define SIO_PERI_NONSEC_TMDS_RESET  _u(0x0)
#define SIO_PERI_NONSEC_TMDS_BITS   _u(0x00000020)
#define SIO_PERI_NONSEC_TMDS_MSB    _u(5)
#define SIO_PERI_NONSEC_TMDS_LSB    _u(5)
#define SIO_PERI_NONSEC_TMDS_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SIO_PERI_NONSEC_INTERP1
// Description : If 1, detach interpolator 1 (of this core) from the Secure SIO,
//               and attach to the Non-secure SIO.
#define SIO_PERI_NONSEC_INTERP1_RESET  _u(0x0)
#define SIO_PERI_NONSEC_INTERP1_BITS   _u(0x00000002)
#define SIO_PERI_NONSEC_INTERP1_MSB    _u(1)
#define SIO_PERI_NONSEC_INTERP1_LSB    _u(1)
#define SIO_PERI_NONSEC_INTERP1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SIO_PERI_NONSEC_INTERP0
// Description : If 1, detach interpolator 0 (of this core) from the Secure SIO,
//               and attach to the Non-secure SIO.
#define SIO_PERI_NONSEC_INTERP0_RESET  _u(0x0)
#define SIO_PERI_NONSEC_INTERP0_BITS   _u(0x00000001)
#define SIO_PERI_NONSEC_INTERP0_MSB    _u(0)
#define SIO_PERI_NONSEC_INTERP0_LSB    _u(0)
#define SIO_PERI_NONSEC_INTERP0_ACCESS "RW"
// =============================================================================
// Register    : SIO_RISCV_SOFTIRQ
// Description : Control the assertion of the standard software interrupt
//               (MIP.MSIP) on the RISC-V cores.
//
//               Unlike the RISC-V timer, this interrupt is not routed to a
//               normal system-level interrupt line, so can not be used by the
//               Arm cores.
//
//               It is safe for both cores to write to this register on the same
//               cycle. The set/clear effect is accumulated across both cores,
//               and then applied. If a flag is both set and cleared on the same
//               cycle, only the set takes effect.
#define SIO_RISCV_SOFTIRQ_OFFSET _u(0x000001a0)
#define SIO_RISCV_SOFTIRQ_BITS   _u(0x00000303)
#define SIO_RISCV_SOFTIRQ_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : SIO_RISCV_SOFTIRQ_CORE1_CLR
// Description : Write 1 to atomically clear the core 1 software interrupt flag.
//               Read to get the status of this flag.
#define SIO_RISCV_SOFTIRQ_CORE1_CLR_RESET  _u(0x0)
#define SIO_RISCV_SOFTIRQ_CORE1_CLR_BITS   _u(0x00000200)
#define SIO_RISCV_SOFTIRQ_CORE1_CLR_MSB    _u(9)
#define SIO_RISCV_SOFTIRQ_CORE1_CLR_LSB    _u(9)
#define SIO_RISCV_SOFTIRQ_CORE1_CLR_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SIO_RISCV_SOFTIRQ_CORE0_CLR
// Description : Write 1 to atomically clear the core 0 software interrupt flag.
//               Read to get the status of this flag.
#define SIO_RISCV_SOFTIRQ_CORE0_CLR_RESET  _u(0x0)
#define SIO_RISCV_SOFTIRQ_CORE0_CLR_BITS   _u(0x00000100)
#define SIO_RISCV_SOFTIRQ_CORE0_CLR_MSB    _u(8)
#define SIO_RISCV_SOFTIRQ_CORE0_CLR_LSB    _u(8)
#define SIO_RISCV_SOFTIRQ_CORE0_CLR_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SIO_RISCV_SOFTIRQ_CORE1_SET
// Description : Write 1 to atomically set the core 1 software interrupt flag.
//               Read to get the status of this flag.
#define SIO_RISCV_SOFTIRQ_CORE1_SET_RESET  _u(0x0)
#define SIO_RISCV_SOFTIRQ_CORE1_SET_BITS   _u(0x00000002)
#define SIO_RISCV_SOFTIRQ_CORE1_SET_MSB    _u(1)
#define SIO_RISCV_SOFTIRQ_CORE1_SET_LSB    _u(1)
#define SIO_RISCV_SOFTIRQ_CORE1_SET_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SIO_RISCV_SOFTIRQ_CORE0_SET
// Description : Write 1 to atomically set the core 0 software interrupt flag.
//               Read to get the status of this flag.
#define SIO_RISCV_SOFTIRQ_CORE0_SET_RESET  _u(0x0)
#define SIO_RISCV_SOFTIRQ_CORE0_SET_BITS   _u(0x00000001)
#define SIO_RISCV_SOFTIRQ_CORE0_SET_MSB    _u(0)
#define SIO_RISCV_SOFTIRQ_CORE0_SET_LSB    _u(0)
#define SIO_RISCV_SOFTIRQ_CORE0_SET_ACCESS "RW"
// =============================================================================
// Register    : SIO_MTIME_CTRL
// Description : Control register for the RISC-V 64-bit Machine-mode timer. This
//               timer is only present in the Secure SIO, so is only accessible
//               to an Arm core in Secure mode or a RISC-V core in Machine mode.
//
//               Note whilst this timer follows the RISC-V privileged
//               specification, it is equally usable by the Arm cores. The
//               interrupts are routed to normal system-level interrupt lines as
//               well as to the MIP.MTIP inputs on the RISC-V cores.
#define SIO_MTIME_CTRL_OFFSET _u(0x000001a4)
#define SIO_MTIME_CTRL_BITS   _u(0x0000000f)
#define SIO_MTIME_CTRL_RESET  _u(0x0000000d)
// -----------------------------------------------------------------------------
// Field       : SIO_MTIME_CTRL_DBGPAUSE_CORE1
// Description : If 1, the timer pauses when core 1 is in the debug halt state.
#define SIO_MTIME_CTRL_DBGPAUSE_CORE1_RESET  _u(0x1)
#define SIO_MTIME_CTRL_DBGPAUSE_CORE1_BITS   _u(0x00000008)
#define SIO_MTIME_CTRL_DBGPAUSE_CORE1_MSB    _u(3)
#define SIO_MTIME_CTRL_DBGPAUSE_CORE1_LSB    _u(3)
#define SIO_MTIME_CTRL_DBGPAUSE_CORE1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SIO_MTIME_CTRL_DBGPAUSE_CORE0
// Description : If 1, the timer pauses when core 0 is in the debug halt state.
#define SIO_MTIME_CTRL_DBGPAUSE_CORE0_RESET  _u(0x1)
#define SIO_MTIME_CTRL_DBGPAUSE_CORE0_BITS   _u(0x00000004)
#define SIO_MTIME_CTRL_DBGPAUSE_CORE0_MSB    _u(2)
#define SIO_MTIME_CTRL_DBGPAUSE_CORE0_LSB    _u(2)
#define SIO_MTIME_CTRL_DBGPAUSE_CORE0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SIO_MTIME_CTRL_FULLSPEED
// Description : If 1, increment the timer every cycle (i.e. run directly from
//               the system clock), rather than incrementing on the system-level
//               timer tick input.
#define SIO_MTIME_CTRL_FULLSPEED_RESET  _u(0x0)
#define SIO_MTIME_CTRL_FULLSPEED_BITS   _u(0x00000002)
#define SIO_MTIME_CTRL_FULLSPEED_MSB    _u(1)
#define SIO_MTIME_CTRL_FULLSPEED_LSB    _u(1)
#define SIO_MTIME_CTRL_FULLSPEED_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SIO_MTIME_CTRL_EN
// Description : Timer enable bit. When 0, the timer will not increment
//               automatically.
#define SIO_MTIME_CTRL_EN_RESET  _u(0x1)
#define SIO_MTIME_CTRL_EN_BITS   _u(0x00000001)
#define SIO_MTIME_CTRL_EN_MSB    _u(0)
#define SIO_MTIME_CTRL_EN_LSB    _u(0)
#define SIO_MTIME_CTRL_EN_ACCESS "RW"
// =============================================================================
// Register    : SIO_MTIME
// Description : Read/write access to the high half of RISC-V Machine-mode
//               timer. This register is shared between both cores. If both
//               cores write on the same cycle, core 1 takes precedence.
#define SIO_MTIME_OFFSET _u(0x000001b0)
#define SIO_MTIME_BITS   _u(0xffffffff)
#define SIO_MTIME_RESET  _u(0x00000000)
#define SIO_MTIME_MSB    _u(31)
#define SIO_MTIME_LSB    _u(0)
#define SIO_MTIME_ACCESS "RW"
// =============================================================================
// Register    : SIO_MTIMEH
// Description : Read/write access to the high half of RISC-V Machine-mode
//               timer. This register is shared between both cores. If both
//               cores write on the same cycle, core 1 takes precedence.
#define SIO_MTIMEH_OFFSET _u(0x000001b4)
#define SIO_MTIMEH_BITS   _u(0xffffffff)
#define SIO_MTIMEH_RESET  _u(0x00000000)
#define SIO_MTIMEH_MSB    _u(31)
#define SIO_MTIMEH_LSB    _u(0)
#define SIO_MTIMEH_ACCESS "RW"
// =============================================================================
// Register    : SIO_MTIMECMP
// Description : Low half of RISC-V Machine-mode timer comparator. This register
//               is core-local, i.e., each core gets a copy of this register,
//               with the comparison result routed to its own interrupt line.
//
//               The timer interrupt is asserted whenever MTIME is greater than
//               or equal to MTIMECMP. This comparison is unsigned, and
//               performed on the full 64-bit values.
#define SIO_MTIMECMP_OFFSET _u(0x000001b8)
#define SIO_MTIMECMP_BITS   _u(0xffffffff)
#define SIO_MTIMECMP_RESET  _u(0xffffffff)
#define SIO_MTIMECMP_MSB    _u(31)
#define SIO_MTIMECMP_LSB    _u(0)
#define SIO_MTIMECMP_ACCESS "RW"
// =============================================================================
// Register    : SIO_MTIMECMPH
// Description : High half of RISC-V Machine-mode timer comparator. This
//               register is core-local.
//
//               The timer interrupt is asserted whenever MTIME is greater than
//               or equal to MTIMECMP. This comparison is unsigned, and
//               performed on the full 64-bit values.
#define SIO_MTIMECMPH_OFFSET _u(0x000001bc)
#define SIO_MTIMECMPH_BITS   _u(0xffffffff)
#define SIO_MTIMECMPH_RESET  _u(0xffffffff)
#define SIO_MTIMECMPH_MSB    _u(31)
#define SIO_MTIMECMPH_LSB    _u(0)
#define SIO_MTIMECMPH_ACCESS "RW"
// =============================================================================
// Register    : SIO_TMDS_CTRL
// Description : Control register for TMDS encoder.
#define SIO_TMDS_CTRL_OFFSET _u(0x000001c0)
#define SIO_TMDS_CTRL_BITS   _u(0x1f9fffff)
#define SIO_TMDS_CTRL_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : SIO_TMDS_CTRL_CLEAR_BALANCE
// Description : Clear the running DC balance state of the TMDS encoders. This
//               bit should be written once at the beginning of each scanline.
#define SIO_TMDS_CTRL_CLEAR_BALANCE_RESET  _u(0x0)
#define SIO_TMDS_CTRL_CLEAR_BALANCE_BITS   _u(0x10000000)
#define SIO_TMDS_CTRL_CLEAR_BALANCE_MSB    _u(28)
#define SIO_TMDS_CTRL_CLEAR_BALANCE_LSB    _u(28)
#define SIO_TMDS_CTRL_CLEAR_BALANCE_ACCESS "SC"
// -----------------------------------------------------------------------------
// Field       : SIO_TMDS_CTRL_PIX2_NOSHIFT
// Description : When encoding two pixels's worth of symbols in one cycle (a
//               read of a PEEK/POP_DOUBLE register), the second encoder sees a
//               shifted version of the colour data register.
//
//               This control disables that shift, so that both encoder layers
//               see the same pixel data. This is used for pixel doubling.
#define SIO_TMDS_CTRL_PIX2_NOSHIFT_RESET  _u(0x0)
#define SIO_TMDS_CTRL_PIX2_NOSHIFT_BITS   _u(0x08000000)
#define SIO_TMDS_CTRL_PIX2_NOSHIFT_MSB    _u(27)
#define SIO_TMDS_CTRL_PIX2_NOSHIFT_LSB    _u(27)
#define SIO_TMDS_CTRL_PIX2_NOSHIFT_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SIO_TMDS_CTRL_PIX_SHIFT
// Description : Shift applied to the colour data register with each read of a
//               POP alias register.
//
//               Reading from the POP_SINGLE register, or reading from the
//               POP_DOUBLE register with PIX2_NOSHIFT set (for pixel doubling),
//               shifts by the indicated amount.
//
//               Reading from a POP_DOUBLE register when PIX2_NOSHIFT is clear
//               will shift by double the indicated amount. (Shift by 32 means
//               no shift.)
//               0x0 -> Do not shift the colour data register.
//               0x1 -> Shift the colour data register by 1 bit
//               0x2 -> Shift the colour data register by 2 bits
//               0x3 -> Shift the colour data register by 4 bits
//               0x4 -> Shift the colour data register by 8 bits
//               0x5 -> Shift the colour data register by 16 bits
#define SIO_TMDS_CTRL_PIX_SHIFT_RESET  _u(0x0)
#define SIO_TMDS_CTRL_PIX_SHIFT_BITS   _u(0x07000000)
#define SIO_TMDS_CTRL_PIX_SHIFT_MSB    _u(26)
#define SIO_TMDS_CTRL_PIX_SHIFT_LSB    _u(24)
#define SIO_TMDS_CTRL_PIX_SHIFT_ACCESS "RW"
#define SIO_TMDS_CTRL_PIX_SHIFT_VALUE_0 _u(0x0)
#define SIO_TMDS_CTRL_PIX_SHIFT_VALUE_1 _u(0x1)
#define SIO_TMDS_CTRL_PIX_SHIFT_VALUE_2 _u(0x2)
#define SIO_TMDS_CTRL_PIX_SHIFT_VALUE_4 _u(0x3)
#define SIO_TMDS_CTRL_PIX_SHIFT_VALUE_8 _u(0x4)
#define SIO_TMDS_CTRL_PIX_SHIFT_VALUE_16 _u(0x5)
// -----------------------------------------------------------------------------
// Field       : SIO_TMDS_CTRL_INTERLEAVE
// Description : Enable lane interleaving for reads of PEEK_SINGLE/POP_SINGLE.
//
//               When interleaving is disabled, each of the 3 symbols appears as
//               a contiguous 10-bit field, with lane 0 being the least-
//               significant and starting at bit 0 of the register.
//
//               When interleaving is enabled, the symbols are packed into 5
//               chunks of 3 lanes times 2 bits (30 bits total). Each chunk
//               contains two bits of a TMDS symbol per lane, with lane 0 being
//               the least significant.
#define SIO_TMDS_CTRL_INTERLEAVE_RESET  _u(0x0)
#define SIO_TMDS_CTRL_INTERLEAVE_BITS   _u(0x00800000)
#define SIO_TMDS_CTRL_INTERLEAVE_MSB    _u(23)
#define SIO_TMDS_CTRL_INTERLEAVE_LSB    _u(23)
#define SIO_TMDS_CTRL_INTERLEAVE_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SIO_TMDS_CTRL_L2_NBITS
// Description : Number of valid colour MSBs for lane 2 (1-8 bits, encoded as 0
//               through 7). Remaining LSBs are masked to 0 after the rotate.
#define SIO_TMDS_CTRL_L2_NBITS_RESET  _u(0x0)
#define SIO_TMDS_CTRL_L2_NBITS_BITS   _u(0x001c0000)
#define SIO_TMDS_CTRL_L2_NBITS_MSB    _u(20)
#define SIO_TMDS_CTRL_L2_NBITS_LSB    _u(18)
#define SIO_TMDS_CTRL_L2_NBITS_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SIO_TMDS_CTRL_L1_NBITS
// Description : Number of valid colour MSBs for lane 1 (1-8 bits, encoded as 0
//               through 7). Remaining LSBs are masked to 0 after the rotate.
#define SIO_TMDS_CTRL_L1_NBITS_RESET  _u(0x0)
#define SIO_TMDS_CTRL_L1_NBITS_BITS   _u(0x00038000)
#define SIO_TMDS_CTRL_L1_NBITS_MSB    _u(17)
#define SIO_TMDS_CTRL_L1_NBITS_LSB    _u(15)
#define SIO_TMDS_CTRL_L1_NBITS_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SIO_TMDS_CTRL_L0_NBITS
// Description : Number of valid colour MSBs for lane 0 (1-8 bits, encoded as 0
//               through 7). Remaining LSBs are masked to 0 after the rotate.
#define SIO_TMDS_CTRL_L0_NBITS_RESET  _u(0x0)
#define SIO_TMDS_CTRL_L0_NBITS_BITS   _u(0x00007000)
#define SIO_TMDS_CTRL_L0_NBITS_MSB    _u(14)
#define SIO_TMDS_CTRL_L0_NBITS_LSB    _u(12)
#define SIO_TMDS_CTRL_L0_NBITS_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SIO_TMDS_CTRL_L2_ROT
// Description : Right-rotate the 16 LSBs of the colour accumulator by 0-15
//               bits, in order to get the MSB of the lane 2 (red) colour data
//               aligned with the MSB of the 8-bit encoder input.
//
//               For example, for RGB565 (red most significant), red is bits
//               15:11, so should be right-rotated by 8 bits to align with bits
//               7:3 of the encoder input.
#define SIO_TMDS_CTRL_L2_ROT_RESET  _u(0x0)
#define SIO_TMDS_CTRL_L2_ROT_BITS   _u(0x00000f00)
#define SIO_TMDS_CTRL_L2_ROT_MSB    _u(11)
#define SIO_TMDS_CTRL_L2_ROT_LSB    _u(8)
#define SIO_TMDS_CTRL_L2_ROT_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SIO_TMDS_CTRL_L1_ROT
// Description : Right-rotate the 16 LSBs of the colour accumulator by 0-15
//               bits, in order to get the MSB of the lane 1 (green) colour data
//               aligned with the MSB of the 8-bit encoder input.
//
//               For example, for RGB565, green is bits 10:5, so should be
//               right-rotated by 3 bits to align with bits 7:2 of the encoder
//               input.
#define SIO_TMDS_CTRL_L1_ROT_RESET  _u(0x0)
#define SIO_TMDS_CTRL_L1_ROT_BITS   _u(0x000000f0)
#define SIO_TMDS_CTRL_L1_ROT_MSB    _u(7)
#define SIO_TMDS_CTRL_L1_ROT_LSB    _u(4)
#define SIO_TMDS_CTRL_L1_ROT_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : SIO_TMDS_CTRL_L0_ROT
// Description : Right-rotate the 16 LSBs of the colour accumulator by 0-15
//               bits, in order to get the MSB of the lane 0 (blue) colour data
//               aligned with the MSB of the 8-bit encoder input.
//
//               For example, for RGB565 (red most significant), blue is bits
//               4:0, so should be right-rotated by 13 to align with bits 7:3 of
//               the encoder input.
#define SIO_TMDS_CTRL_L0_ROT_RESET  _u(0x0)
#define SIO_TMDS_CTRL_L0_ROT_BITS   _u(0x0000000f)
#define SIO_TMDS_CTRL_L0_ROT_MSB    _u(3)
#define SIO_TMDS_CTRL_L0_ROT_LSB    _u(0)
#define SIO_TMDS_CTRL_L0_ROT_ACCESS "RW"
// =============================================================================
// Register    : SIO_TMDS_WDATA
// Description : Write-only access to the TMDS colour data register.
#define SIO_TMDS_WDATA_OFFSET _u(0x000001c4)
#define SIO_TMDS_WDATA_BITS   _u(0xffffffff)
#define SIO_TMDS_WDATA_RESET  _u(0x00000000)
#define SIO_TMDS_WDATA_MSB    _u(31)
#define SIO_TMDS_WDATA_LSB    _u(0)
#define SIO_TMDS_WDATA_ACCESS "WO"
// =============================================================================
// Register    : SIO_TMDS_PEEK_SINGLE
// Description : Get the encoding of one pixel's worth of colour data, packed
//               into a 32-bit value (3x10-bit symbols).
//
//               The PEEK alias does not shift the colour register when read,
//               but still advances the running DC balance state of each
//               encoder. This is useful for pixel doubling.
#define SIO_TMDS_PEEK_SINGLE_OFFSET _u(0x000001c8)
#define SIO_TMDS_PEEK_SINGLE_BITS   _u(0xffffffff)
#define SIO_TMDS_PEEK_SINGLE_RESET  _u(0x00000000)
#define SIO_TMDS_PEEK_SINGLE_MSB    _u(31)
#define SIO_TMDS_PEEK_SINGLE_LSB    _u(0)
#define SIO_TMDS_PEEK_SINGLE_ACCESS "RF"
// =============================================================================
// Register    : SIO_TMDS_POP_SINGLE
// Description : Get the encoding of one pixel's worth of colour data, packed
//               into a 32-bit value. The packing is 5 chunks of 3 lanes times 2
//               bits (30 bits total). Each chunk contains two bits of a TMDS
//               symbol per lane. This format is intended for shifting out with
//               the HSTX peripheral on RP2350.
//
//               The POP alias shifts the colour register when read, as well as
//               advancing the running DC balance state of each encoder.
#define SIO_TMDS_POP_SINGLE_OFFSET _u(0x000001cc)
#define SIO_TMDS_POP_SINGLE_BITS   _u(0xffffffff)
#define SIO_TMDS_POP_SINGLE_RESET  _u(0x00000000)
#define SIO_TMDS_POP_SINGLE_MSB    _u(31)
#define SIO_TMDS_POP_SINGLE_LSB    _u(0)
#define SIO_TMDS_POP_SINGLE_ACCESS "RF"
// =============================================================================
// Register    : SIO_TMDS_PEEK_DOUBLE_L0
// Description : Get lane 0 of the encoding of two pixels' worth of colour data.
//               Two 10-bit TMDS symbols are packed at the bottom of a 32-bit
//               word.
//
//               The PEEK alias does not shift the colour register when read,
//               but still advances the lane 0 DC balance state. This is useful
//               if all 3 lanes' worth of encode are to be read at once, rather
//               than processing the entire scanline for one lane before moving
//               to the next lane.
#define SIO_TMDS_PEEK_DOUBLE_L0_OFFSET _u(0x000001d0)
#define SIO_TMDS_PEEK_DOUBLE_L0_BITS   _u(0xffffffff)
#define SIO_TMDS_PEEK_DOUBLE_L0_RESET  _u(0x00000000)
#define SIO_TMDS_PEEK_DOUBLE_L0_MSB    _u(31)
#define SIO_TMDS_PEEK_DOUBLE_L0_LSB    _u(0)
#define SIO_TMDS_PEEK_DOUBLE_L0_ACCESS "RF"
// =============================================================================
// Register    : SIO_TMDS_POP_DOUBLE_L0
// Description : Get lane 0 of the encoding of two pixels' worth of colour data.
//               Two 10-bit TMDS symbols are packed at the bottom of a 32-bit
//               word.
//
//               The POP alias shifts the colour register when read, according
//               to the values of PIX_SHIFT and PIX2_NOSHIFT.
#define SIO_TMDS_POP_DOUBLE_L0_OFFSET _u(0x000001d4)
#define SIO_TMDS_POP_DOUBLE_L0_BITS   _u(0xffffffff)
#define SIO_TMDS_POP_DOUBLE_L0_RESET  _u(0x00000000)
#define SIO_TMDS_POP_DOUBLE_L0_MSB    _u(31)
#define SIO_TMDS_POP_DOUBLE_L0_LSB    _u(0)
#define SIO_TMDS_POP_DOUBLE_L0_ACCESS "RF"
// =============================================================================
// Register    : SIO_TMDS_PEEK_DOUBLE_L1
// Description : Get lane 1 of the encoding of two pixels' worth of colour data.
//               Two 10-bit TMDS symbols are packed at the bottom of a 32-bit
//               word.
//
//               The PEEK alias does not shift the colour register when read,
//               but still advances the lane 1 DC balance state. This is useful
//               if all 3 lanes' worth of encode are to be read at once, rather
//               than processing the entire scanline for one lane before moving
//               to the next lane.
#define SIO_TMDS_PEEK_DOUBLE_L1_OFFSET _u(0x000001d8)
#define SIO_TMDS_PEEK_DOUBLE_L1_BITS   _u(0xffffffff)
#define SIO_TMDS_PEEK_DOUBLE_L1_RESET  _u(0x00000000)
#define SIO_TMDS_PEEK_DOUBLE_L1_MSB    _u(31)
#define SIO_TMDS_PEEK_DOUBLE_L1_LSB    _u(0)
#define SIO_TMDS_PEEK_DOUBLE_L1_ACCESS "RF"
// =============================================================================
// Register    : SIO_TMDS_POP_DOUBLE_L1
// Description : Get lane 1 of the encoding of two pixels' worth of colour data.
//               Two 10-bit TMDS symbols are packed at the bottom of a 32-bit
//               word.
//
//               The POP alias shifts the colour register when read, according
//               to the values of PIX_SHIFT and PIX2_NOSHIFT.
#define SIO_TMDS_POP_DOUBLE_L1_OFFSET _u(0x000001dc)
#define SIO_TMDS_POP_DOUBLE_L1_BITS   _u(0xffffffff)
#define SIO_TMDS_POP_DOUBLE_L1_RESET  _u(0x00000000)
#define SIO_TMDS_POP_DOUBLE_L1_MSB    _u(31)
#define SIO_TMDS_POP_DOUBLE_L1_LSB    _u(0)
#define SIO_TMDS_POP_DOUBLE_L1_ACCESS "RF"
// =============================================================================
// Register    : SIO_TMDS_PEEK_DOUBLE_L2
// Description : Get lane 2 of the encoding of two pixels' worth of colour data.
//               Two 10-bit TMDS symbols are packed at the bottom of a 32-bit
//               word.
//
//               The PEEK alias does not shift the colour register when read,
//               but still advances the lane 2 DC balance state. This is useful
//               if all 3 lanes' worth of encode are to be read at once, rather
//               than processing the entire scanline for one lane before moving
//               to the next lane.
#define SIO_TMDS_PEEK_DOUBLE_L2_OFFSET _u(0x000001e0)
#define SIO_TMDS_PEEK_DOUBLE_L2_BITS   _u(0xffffffff)
#define SIO_TMDS_PEEK_DOUBLE_L2_RESET  _u(0x00000000)
#define SIO_TMDS_PEEK_DOUBLE_L2_MSB    _u(31)
#define SIO_TMDS_PEEK_DOUBLE_L2_LSB    _u(0)
#define SIO_TMDS_PEEK_DOUBLE_L2_ACCESS "RF"
// =============================================================================
// Register    : SIO_TMDS_POP_DOUBLE_L2
// Description : Get lane 2 of the encoding of two pixels' worth of colour data.
//               Two 10-bit TMDS symbols are packed at the bottom of a 32-bit
//               word.
//
//               The POP alias shifts the colour register when read, according
//               to the values of PIX_SHIFT and PIX2_NOSHIFT.
#define SIO_TMDS_POP_DOUBLE_L2_OFFSET _u(0x000001e4)
#define SIO_TMDS_POP_DOUBLE_L2_BITS   _u(0xffffffff)
#define SIO_TMDS_POP_DOUBLE_L2_RESET  _u(0x00000000)
#define SIO_TMDS_POP_DOUBLE_L2_MSB    _u(31)
#define SIO_TMDS_POP_DOUBLE_L2_LSB    _u(0)
#define SIO_TMDS_POP_DOUBLE_L2_ACCESS "RF"
// =============================================================================
#endif // _HARDWARE_REGS_SIO_H

