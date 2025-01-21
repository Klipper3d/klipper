// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
// =============================================================================
// Register block : M33_EPPB
// Version        : 1
// Bus type       : apb
// Description    : Cortex-M33 EPPB vendor register block for RP2350
// =============================================================================
#ifndef _HARDWARE_REGS_M33_EPPB_H
#define _HARDWARE_REGS_M33_EPPB_H
// =============================================================================
// Register    : M33_EPPB_NMI_MASK0
// Description : NMI mask for IRQs 0 through 31. This register is core-local,
//               and is reset by a processor warm reset.
#define M33_EPPB_NMI_MASK0_OFFSET _u(0x00000000)
#define M33_EPPB_NMI_MASK0_BITS   _u(0xffffffff)
#define M33_EPPB_NMI_MASK0_RESET  _u(0x00000000)
#define M33_EPPB_NMI_MASK0_MSB    _u(31)
#define M33_EPPB_NMI_MASK0_LSB    _u(0)
#define M33_EPPB_NMI_MASK0_ACCESS "RW"
// =============================================================================
// Register    : M33_EPPB_NMI_MASK1
// Description : NMI mask for IRQs 0 though 51. This register is core-local, and
//               is reset by a processor warm reset.
#define M33_EPPB_NMI_MASK1_OFFSET _u(0x00000004)
#define M33_EPPB_NMI_MASK1_BITS   _u(0x000fffff)
#define M33_EPPB_NMI_MASK1_RESET  _u(0x00000000)
#define M33_EPPB_NMI_MASK1_MSB    _u(19)
#define M33_EPPB_NMI_MASK1_LSB    _u(0)
#define M33_EPPB_NMI_MASK1_ACCESS "RW"
// =============================================================================
// Register    : M33_EPPB_SLEEPCTRL
// Description : Nonstandard sleep control register
#define M33_EPPB_SLEEPCTRL_OFFSET _u(0x00000008)
#define M33_EPPB_SLEEPCTRL_BITS   _u(0x00000007)
#define M33_EPPB_SLEEPCTRL_RESET  _u(0x00000002)
// -----------------------------------------------------------------------------
// Field       : M33_EPPB_SLEEPCTRL_WICENACK
// Description : Status signal from the processor's interrupt controller.
//               Changes to WICENREQ are eventually reflected in WICENACK.
#define M33_EPPB_SLEEPCTRL_WICENACK_RESET  _u(0x0)
#define M33_EPPB_SLEEPCTRL_WICENACK_BITS   _u(0x00000004)
#define M33_EPPB_SLEEPCTRL_WICENACK_MSB    _u(2)
#define M33_EPPB_SLEEPCTRL_WICENACK_LSB    _u(2)
#define M33_EPPB_SLEEPCTRL_WICENACK_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : M33_EPPB_SLEEPCTRL_WICENREQ
// Description : Request that the next processor deep sleep is a WIC sleep.
//               After setting this bit, before sleeping, poll WICENACK to
//               ensure the processor interrupt controller has acknowledged the
//               change.
#define M33_EPPB_SLEEPCTRL_WICENREQ_RESET  _u(0x1)
#define M33_EPPB_SLEEPCTRL_WICENREQ_BITS   _u(0x00000002)
#define M33_EPPB_SLEEPCTRL_WICENREQ_MSB    _u(1)
#define M33_EPPB_SLEEPCTRL_WICENREQ_LSB    _u(1)
#define M33_EPPB_SLEEPCTRL_WICENREQ_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : M33_EPPB_SLEEPCTRL_LIGHT_SLEEP
// Description : By default, any processor sleep will deassert the system-level
//               clock request. Reenabling the clocks incurs 5 cycles of
//               additional latency on wakeup.
//
//               Setting LIGHT_SLEEP to 1 keeps the clock request asserted
//               during a normal sleep (Arm SCR.SLEEPDEEP = 0), for faster
//               wakeup. Processor deep sleep (Arm SCR.SLEEPDEEP = 1) is not
//               affected, and will always deassert the system-level clock
//               request.
#define M33_EPPB_SLEEPCTRL_LIGHT_SLEEP_RESET  _u(0x0)
#define M33_EPPB_SLEEPCTRL_LIGHT_SLEEP_BITS   _u(0x00000001)
#define M33_EPPB_SLEEPCTRL_LIGHT_SLEEP_MSB    _u(0)
#define M33_EPPB_SLEEPCTRL_LIGHT_SLEEP_LSB    _u(0)
#define M33_EPPB_SLEEPCTRL_LIGHT_SLEEP_ACCESS "RW"
// =============================================================================
#endif // _HARDWARE_REGS_M33_EPPB_H

