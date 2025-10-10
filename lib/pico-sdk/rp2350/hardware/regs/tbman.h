// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
// =============================================================================
// Register block : TBMAN
// Version        : 1
// Bus type       : apb
// Description    : For managing simulation testbenches
// =============================================================================
#ifndef _HARDWARE_REGS_TBMAN_H
#define _HARDWARE_REGS_TBMAN_H
// =============================================================================
// Register    : TBMAN_PLATFORM
// Description : Indicates the type of platform in use
#define TBMAN_PLATFORM_OFFSET _u(0x00000000)
#define TBMAN_PLATFORM_BITS   _u(0x00000007)
#define TBMAN_PLATFORM_RESET  _u(0x00000001)
// -----------------------------------------------------------------------------
// Field       : TBMAN_PLATFORM_HDLSIM
// Description : Indicates the platform is a simulation
#define TBMAN_PLATFORM_HDLSIM_RESET  _u(0x0)
#define TBMAN_PLATFORM_HDLSIM_BITS   _u(0x00000004)
#define TBMAN_PLATFORM_HDLSIM_MSB    _u(2)
#define TBMAN_PLATFORM_HDLSIM_LSB    _u(2)
#define TBMAN_PLATFORM_HDLSIM_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : TBMAN_PLATFORM_FPGA
// Description : Indicates the platform is an FPGA
#define TBMAN_PLATFORM_FPGA_RESET  _u(0x0)
#define TBMAN_PLATFORM_FPGA_BITS   _u(0x00000002)
#define TBMAN_PLATFORM_FPGA_MSB    _u(1)
#define TBMAN_PLATFORM_FPGA_LSB    _u(1)
#define TBMAN_PLATFORM_FPGA_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : TBMAN_PLATFORM_ASIC
// Description : Indicates the platform is an ASIC
#define TBMAN_PLATFORM_ASIC_RESET  _u(0x1)
#define TBMAN_PLATFORM_ASIC_BITS   _u(0x00000001)
#define TBMAN_PLATFORM_ASIC_MSB    _u(0)
#define TBMAN_PLATFORM_ASIC_LSB    _u(0)
#define TBMAN_PLATFORM_ASIC_ACCESS "RO"
// =============================================================================
#endif // _HARDWARE_REGS_TBMAN_H

