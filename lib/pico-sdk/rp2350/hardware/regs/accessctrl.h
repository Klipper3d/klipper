// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
// =============================================================================
// Register block : ACCESSCTRL
// Version        : 1
// Bus type       : apb
// Description    : Hardware access control registers
// =============================================================================
#ifndef _HARDWARE_REGS_ACCESSCTRL_H
#define _HARDWARE_REGS_ACCESSCTRL_H
// =============================================================================
// Register    : ACCESSCTRL_LOCK
// Description : Once a LOCK bit is written to 1, ACCESSCTRL silently ignores
//               writes from that master. LOCK is writable only by a Secure,
//               Privileged processor or debugger.
//
//               LOCK bits are only writable when their value is zero. Once set,
//               they can never be cleared, except by a full reset of ACCESSCTRL
//
//               Setting the LOCK bit does not affect whether an access raises a
//               bus error. Unprivileged writes, or writes from the DMA, will
//               continue to raise bus errors. All other accesses will continue
//               not to.
#define ACCESSCTRL_LOCK_OFFSET _u(0x00000000)
#define ACCESSCTRL_LOCK_BITS   _u(0x0000000f)
#define ACCESSCTRL_LOCK_RESET  _u(0x00000004)
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_LOCK_DEBUG
#define ACCESSCTRL_LOCK_DEBUG_RESET  _u(0x0)
#define ACCESSCTRL_LOCK_DEBUG_BITS   _u(0x00000008)
#define ACCESSCTRL_LOCK_DEBUG_MSB    _u(3)
#define ACCESSCTRL_LOCK_DEBUG_LSB    _u(3)
#define ACCESSCTRL_LOCK_DEBUG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_LOCK_DMA
#define ACCESSCTRL_LOCK_DMA_RESET  _u(0x1)
#define ACCESSCTRL_LOCK_DMA_BITS   _u(0x00000004)
#define ACCESSCTRL_LOCK_DMA_MSB    _u(2)
#define ACCESSCTRL_LOCK_DMA_LSB    _u(2)
#define ACCESSCTRL_LOCK_DMA_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_LOCK_CORE1
#define ACCESSCTRL_LOCK_CORE1_RESET  _u(0x0)
#define ACCESSCTRL_LOCK_CORE1_BITS   _u(0x00000002)
#define ACCESSCTRL_LOCK_CORE1_MSB    _u(1)
#define ACCESSCTRL_LOCK_CORE1_LSB    _u(1)
#define ACCESSCTRL_LOCK_CORE1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_LOCK_CORE0
#define ACCESSCTRL_LOCK_CORE0_RESET  _u(0x0)
#define ACCESSCTRL_LOCK_CORE0_BITS   _u(0x00000001)
#define ACCESSCTRL_LOCK_CORE0_MSB    _u(0)
#define ACCESSCTRL_LOCK_CORE0_LSB    _u(0)
#define ACCESSCTRL_LOCK_CORE0_ACCESS "RW"
// =============================================================================
// Register    : ACCESSCTRL_FORCE_CORE_NS
// Description : Force core 1's bus accesses to always be Non-secure, no matter
//               the core's internal state.
//
//               Useful for schemes where one core is designated as the Non-
//               secure core, since some peripherals may filter individual
//               registers internally based on security state but not on master
//               ID.
#define ACCESSCTRL_FORCE_CORE_NS_OFFSET _u(0x00000004)
#define ACCESSCTRL_FORCE_CORE_NS_BITS   _u(0x00000002)
#define ACCESSCTRL_FORCE_CORE_NS_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_FORCE_CORE_NS_CORE1
#define ACCESSCTRL_FORCE_CORE_NS_CORE1_RESET  _u(0x0)
#define ACCESSCTRL_FORCE_CORE_NS_CORE1_BITS   _u(0x00000002)
#define ACCESSCTRL_FORCE_CORE_NS_CORE1_MSB    _u(1)
#define ACCESSCTRL_FORCE_CORE_NS_CORE1_LSB    _u(1)
#define ACCESSCTRL_FORCE_CORE_NS_CORE1_ACCESS "RW"
// =============================================================================
// Register    : ACCESSCTRL_CFGRESET
// Description : Write 1 to reset all ACCESSCTRL configuration, except for the
//               LOCK and FORCE_CORE_NS registers.
//
//               This bit is used in the RP2350 bootrom to quickly restore
//               ACCESSCTRL to a known state during the boot path.
//
//               Note that, like all registers in ACCESSCTRL, this register is
//               not writable when the writer's corresponding LOCK bit is set,
//               therefore a master which has been locked out of ACCESSCTRL can
//               not use the CFGRESET register to disturb its contents.
#define ACCESSCTRL_CFGRESET_OFFSET _u(0x00000008)
#define ACCESSCTRL_CFGRESET_BITS   _u(0x00000001)
#define ACCESSCTRL_CFGRESET_RESET  _u(0x00000000)
#define ACCESSCTRL_CFGRESET_MSB    _u(0)
#define ACCESSCTRL_CFGRESET_LSB    _u(0)
#define ACCESSCTRL_CFGRESET_ACCESS "SC"
// =============================================================================
// Register    : ACCESSCTRL_GPIO_NSMASK0
// Description : Control whether GPIO0...31 are accessible to Non-secure code.
//               Writable only by a Secure, Privileged processor or debugger.
//
//               0 -> Secure access only
//
//               1 -> Secure + Non-secure access
#define ACCESSCTRL_GPIO_NSMASK0_OFFSET _u(0x0000000c)
#define ACCESSCTRL_GPIO_NSMASK0_BITS   _u(0xffffffff)
#define ACCESSCTRL_GPIO_NSMASK0_RESET  _u(0x00000000)
#define ACCESSCTRL_GPIO_NSMASK0_MSB    _u(31)
#define ACCESSCTRL_GPIO_NSMASK0_LSB    _u(0)
#define ACCESSCTRL_GPIO_NSMASK0_ACCESS "RW"
// =============================================================================
// Register    : ACCESSCTRL_GPIO_NSMASK1
// Description : Control whether GPIO32..47 are accessible to Non-secure code,
//               and whether QSPI and USB bitbang are accessible through the
//               Non-secure SIO. Writable only by a Secure, Privileged processor
//               or debugger.
#define ACCESSCTRL_GPIO_NSMASK1_OFFSET _u(0x00000010)
#define ACCESSCTRL_GPIO_NSMASK1_BITS   _u(0xff00ffff)
#define ACCESSCTRL_GPIO_NSMASK1_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_GPIO_NSMASK1_QSPI_SD
#define ACCESSCTRL_GPIO_NSMASK1_QSPI_SD_RESET  _u(0x0)
#define ACCESSCTRL_GPIO_NSMASK1_QSPI_SD_BITS   _u(0xf0000000)
#define ACCESSCTRL_GPIO_NSMASK1_QSPI_SD_MSB    _u(31)
#define ACCESSCTRL_GPIO_NSMASK1_QSPI_SD_LSB    _u(28)
#define ACCESSCTRL_GPIO_NSMASK1_QSPI_SD_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_GPIO_NSMASK1_QSPI_CSN
#define ACCESSCTRL_GPIO_NSMASK1_QSPI_CSN_RESET  _u(0x0)
#define ACCESSCTRL_GPIO_NSMASK1_QSPI_CSN_BITS   _u(0x08000000)
#define ACCESSCTRL_GPIO_NSMASK1_QSPI_CSN_MSB    _u(27)
#define ACCESSCTRL_GPIO_NSMASK1_QSPI_CSN_LSB    _u(27)
#define ACCESSCTRL_GPIO_NSMASK1_QSPI_CSN_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_GPIO_NSMASK1_QSPI_SCK
#define ACCESSCTRL_GPIO_NSMASK1_QSPI_SCK_RESET  _u(0x0)
#define ACCESSCTRL_GPIO_NSMASK1_QSPI_SCK_BITS   _u(0x04000000)
#define ACCESSCTRL_GPIO_NSMASK1_QSPI_SCK_MSB    _u(26)
#define ACCESSCTRL_GPIO_NSMASK1_QSPI_SCK_LSB    _u(26)
#define ACCESSCTRL_GPIO_NSMASK1_QSPI_SCK_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_GPIO_NSMASK1_USB_DM
#define ACCESSCTRL_GPIO_NSMASK1_USB_DM_RESET  _u(0x0)
#define ACCESSCTRL_GPIO_NSMASK1_USB_DM_BITS   _u(0x02000000)
#define ACCESSCTRL_GPIO_NSMASK1_USB_DM_MSB    _u(25)
#define ACCESSCTRL_GPIO_NSMASK1_USB_DM_LSB    _u(25)
#define ACCESSCTRL_GPIO_NSMASK1_USB_DM_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_GPIO_NSMASK1_USB_DP
#define ACCESSCTRL_GPIO_NSMASK1_USB_DP_RESET  _u(0x0)
#define ACCESSCTRL_GPIO_NSMASK1_USB_DP_BITS   _u(0x01000000)
#define ACCESSCTRL_GPIO_NSMASK1_USB_DP_MSB    _u(24)
#define ACCESSCTRL_GPIO_NSMASK1_USB_DP_LSB    _u(24)
#define ACCESSCTRL_GPIO_NSMASK1_USB_DP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_GPIO_NSMASK1_GPIO
#define ACCESSCTRL_GPIO_NSMASK1_GPIO_RESET  _u(0x0000)
#define ACCESSCTRL_GPIO_NSMASK1_GPIO_BITS   _u(0x0000ffff)
#define ACCESSCTRL_GPIO_NSMASK1_GPIO_MSB    _u(15)
#define ACCESSCTRL_GPIO_NSMASK1_GPIO_LSB    _u(0)
#define ACCESSCTRL_GPIO_NSMASK1_GPIO_ACCESS "RW"
// =============================================================================
// Register    : ACCESSCTRL_ROM
// Description : Control whether debugger, DMA, core 0 and core 1 can access
//               ROM, and at what security/privilege levels they can do so.
//
//               Defaults to fully open access.
//
//               This register is writable only from a Secure, Privileged
//               processor or debugger, with the exception of the NSU bit, which
//               becomes Non-secure-Privileged-writable when the NSP bit is set.
#define ACCESSCTRL_ROM_OFFSET _u(0x00000014)
#define ACCESSCTRL_ROM_BITS   _u(0x000000ff)
#define ACCESSCTRL_ROM_RESET  _u(0x000000ff)
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_ROM_DBG
// Description : If 1, ROM can be accessed by the debugger, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_ROM_DBG_RESET  _u(0x1)
#define ACCESSCTRL_ROM_DBG_BITS   _u(0x00000080)
#define ACCESSCTRL_ROM_DBG_MSB    _u(7)
#define ACCESSCTRL_ROM_DBG_LSB    _u(7)
#define ACCESSCTRL_ROM_DBG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_ROM_DMA
// Description : If 1, ROM can be accessed by the DMA, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_ROM_DMA_RESET  _u(0x1)
#define ACCESSCTRL_ROM_DMA_BITS   _u(0x00000040)
#define ACCESSCTRL_ROM_DMA_MSB    _u(6)
#define ACCESSCTRL_ROM_DMA_LSB    _u(6)
#define ACCESSCTRL_ROM_DMA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_ROM_CORE1
// Description : If 1, ROM can be accessed by core 1, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_ROM_CORE1_RESET  _u(0x1)
#define ACCESSCTRL_ROM_CORE1_BITS   _u(0x00000020)
#define ACCESSCTRL_ROM_CORE1_MSB    _u(5)
#define ACCESSCTRL_ROM_CORE1_LSB    _u(5)
#define ACCESSCTRL_ROM_CORE1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_ROM_CORE0
// Description : If 1, ROM can be accessed by core 0, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_ROM_CORE0_RESET  _u(0x1)
#define ACCESSCTRL_ROM_CORE0_BITS   _u(0x00000010)
#define ACCESSCTRL_ROM_CORE0_MSB    _u(4)
#define ACCESSCTRL_ROM_CORE0_LSB    _u(4)
#define ACCESSCTRL_ROM_CORE0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_ROM_SP
// Description : If 1, ROM can be accessed from a Secure, Privileged context.
#define ACCESSCTRL_ROM_SP_RESET  _u(0x1)
#define ACCESSCTRL_ROM_SP_BITS   _u(0x00000008)
#define ACCESSCTRL_ROM_SP_MSB    _u(3)
#define ACCESSCTRL_ROM_SP_LSB    _u(3)
#define ACCESSCTRL_ROM_SP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_ROM_SU
// Description : If 1, and SP is also set, ROM can be accessed from a Secure,
//               Unprivileged context.
#define ACCESSCTRL_ROM_SU_RESET  _u(0x1)
#define ACCESSCTRL_ROM_SU_BITS   _u(0x00000004)
#define ACCESSCTRL_ROM_SU_MSB    _u(2)
#define ACCESSCTRL_ROM_SU_LSB    _u(2)
#define ACCESSCTRL_ROM_SU_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_ROM_NSP
// Description : If 1, ROM can be accessed from a Non-secure, Privileged
//               context.
#define ACCESSCTRL_ROM_NSP_RESET  _u(0x1)
#define ACCESSCTRL_ROM_NSP_BITS   _u(0x00000002)
#define ACCESSCTRL_ROM_NSP_MSB    _u(1)
#define ACCESSCTRL_ROM_NSP_LSB    _u(1)
#define ACCESSCTRL_ROM_NSP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_ROM_NSU
// Description : If 1, and NSP is also set, ROM can be accessed from a Non-
//               secure, Unprivileged context.
//
//               This bit is writable from a Non-secure, Privileged context, if
//               and only if the NSP bit is set.
#define ACCESSCTRL_ROM_NSU_RESET  _u(0x1)
#define ACCESSCTRL_ROM_NSU_BITS   _u(0x00000001)
#define ACCESSCTRL_ROM_NSU_MSB    _u(0)
#define ACCESSCTRL_ROM_NSU_LSB    _u(0)
#define ACCESSCTRL_ROM_NSU_ACCESS "RW"
// =============================================================================
// Register    : ACCESSCTRL_XIP_MAIN
// Description : Control whether debugger, DMA, core 0 and core 1 can access
//               XIP_MAIN, and at what security/privilege levels they can do so.
//
//               Defaults to fully open access.
//
//               This register is writable only from a Secure, Privileged
//               processor or debugger, with the exception of the NSU bit, which
//               becomes Non-secure-Privileged-writable when the NSP bit is set.
#define ACCESSCTRL_XIP_MAIN_OFFSET _u(0x00000018)
#define ACCESSCTRL_XIP_MAIN_BITS   _u(0x000000ff)
#define ACCESSCTRL_XIP_MAIN_RESET  _u(0x000000ff)
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_XIP_MAIN_DBG
// Description : If 1, XIP_MAIN can be accessed by the debugger, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_XIP_MAIN_DBG_RESET  _u(0x1)
#define ACCESSCTRL_XIP_MAIN_DBG_BITS   _u(0x00000080)
#define ACCESSCTRL_XIP_MAIN_DBG_MSB    _u(7)
#define ACCESSCTRL_XIP_MAIN_DBG_LSB    _u(7)
#define ACCESSCTRL_XIP_MAIN_DBG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_XIP_MAIN_DMA
// Description : If 1, XIP_MAIN can be accessed by the DMA, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_XIP_MAIN_DMA_RESET  _u(0x1)
#define ACCESSCTRL_XIP_MAIN_DMA_BITS   _u(0x00000040)
#define ACCESSCTRL_XIP_MAIN_DMA_MSB    _u(6)
#define ACCESSCTRL_XIP_MAIN_DMA_LSB    _u(6)
#define ACCESSCTRL_XIP_MAIN_DMA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_XIP_MAIN_CORE1
// Description : If 1, XIP_MAIN can be accessed by core 1, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_XIP_MAIN_CORE1_RESET  _u(0x1)
#define ACCESSCTRL_XIP_MAIN_CORE1_BITS   _u(0x00000020)
#define ACCESSCTRL_XIP_MAIN_CORE1_MSB    _u(5)
#define ACCESSCTRL_XIP_MAIN_CORE1_LSB    _u(5)
#define ACCESSCTRL_XIP_MAIN_CORE1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_XIP_MAIN_CORE0
// Description : If 1, XIP_MAIN can be accessed by core 0, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_XIP_MAIN_CORE0_RESET  _u(0x1)
#define ACCESSCTRL_XIP_MAIN_CORE0_BITS   _u(0x00000010)
#define ACCESSCTRL_XIP_MAIN_CORE0_MSB    _u(4)
#define ACCESSCTRL_XIP_MAIN_CORE0_LSB    _u(4)
#define ACCESSCTRL_XIP_MAIN_CORE0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_XIP_MAIN_SP
// Description : If 1, XIP_MAIN can be accessed from a Secure, Privileged
//               context.
#define ACCESSCTRL_XIP_MAIN_SP_RESET  _u(0x1)
#define ACCESSCTRL_XIP_MAIN_SP_BITS   _u(0x00000008)
#define ACCESSCTRL_XIP_MAIN_SP_MSB    _u(3)
#define ACCESSCTRL_XIP_MAIN_SP_LSB    _u(3)
#define ACCESSCTRL_XIP_MAIN_SP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_XIP_MAIN_SU
// Description : If 1, and SP is also set, XIP_MAIN can be accessed from a
//               Secure, Unprivileged context.
#define ACCESSCTRL_XIP_MAIN_SU_RESET  _u(0x1)
#define ACCESSCTRL_XIP_MAIN_SU_BITS   _u(0x00000004)
#define ACCESSCTRL_XIP_MAIN_SU_MSB    _u(2)
#define ACCESSCTRL_XIP_MAIN_SU_LSB    _u(2)
#define ACCESSCTRL_XIP_MAIN_SU_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_XIP_MAIN_NSP
// Description : If 1, XIP_MAIN can be accessed from a Non-secure, Privileged
//               context.
#define ACCESSCTRL_XIP_MAIN_NSP_RESET  _u(0x1)
#define ACCESSCTRL_XIP_MAIN_NSP_BITS   _u(0x00000002)
#define ACCESSCTRL_XIP_MAIN_NSP_MSB    _u(1)
#define ACCESSCTRL_XIP_MAIN_NSP_LSB    _u(1)
#define ACCESSCTRL_XIP_MAIN_NSP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_XIP_MAIN_NSU
// Description : If 1, and NSP is also set, XIP_MAIN can be accessed from a Non-
//               secure, Unprivileged context.
//
//               This bit is writable from a Non-secure, Privileged context, if
//               and only if the NSP bit is set.
#define ACCESSCTRL_XIP_MAIN_NSU_RESET  _u(0x1)
#define ACCESSCTRL_XIP_MAIN_NSU_BITS   _u(0x00000001)
#define ACCESSCTRL_XIP_MAIN_NSU_MSB    _u(0)
#define ACCESSCTRL_XIP_MAIN_NSU_LSB    _u(0)
#define ACCESSCTRL_XIP_MAIN_NSU_ACCESS "RW"
// =============================================================================
// Register    : ACCESSCTRL_SRAM0
// Description : Control whether debugger, DMA, core 0 and core 1 can access
//               SRAM0, and at what security/privilege levels they can do so.
//
//               Defaults to fully open access.
//
//               This register is writable only from a Secure, Privileged
//               processor or debugger, with the exception of the NSU bit, which
//               becomes Non-secure-Privileged-writable when the NSP bit is set.
#define ACCESSCTRL_SRAM0_OFFSET _u(0x0000001c)
#define ACCESSCTRL_SRAM0_BITS   _u(0x000000ff)
#define ACCESSCTRL_SRAM0_RESET  _u(0x000000ff)
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM0_DBG
// Description : If 1, SRAM0 can be accessed by the debugger, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_SRAM0_DBG_RESET  _u(0x1)
#define ACCESSCTRL_SRAM0_DBG_BITS   _u(0x00000080)
#define ACCESSCTRL_SRAM0_DBG_MSB    _u(7)
#define ACCESSCTRL_SRAM0_DBG_LSB    _u(7)
#define ACCESSCTRL_SRAM0_DBG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM0_DMA
// Description : If 1, SRAM0 can be accessed by the DMA, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_SRAM0_DMA_RESET  _u(0x1)
#define ACCESSCTRL_SRAM0_DMA_BITS   _u(0x00000040)
#define ACCESSCTRL_SRAM0_DMA_MSB    _u(6)
#define ACCESSCTRL_SRAM0_DMA_LSB    _u(6)
#define ACCESSCTRL_SRAM0_DMA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM0_CORE1
// Description : If 1, SRAM0 can be accessed by core 1, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_SRAM0_CORE1_RESET  _u(0x1)
#define ACCESSCTRL_SRAM0_CORE1_BITS   _u(0x00000020)
#define ACCESSCTRL_SRAM0_CORE1_MSB    _u(5)
#define ACCESSCTRL_SRAM0_CORE1_LSB    _u(5)
#define ACCESSCTRL_SRAM0_CORE1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM0_CORE0
// Description : If 1, SRAM0 can be accessed by core 0, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_SRAM0_CORE0_RESET  _u(0x1)
#define ACCESSCTRL_SRAM0_CORE0_BITS   _u(0x00000010)
#define ACCESSCTRL_SRAM0_CORE0_MSB    _u(4)
#define ACCESSCTRL_SRAM0_CORE0_LSB    _u(4)
#define ACCESSCTRL_SRAM0_CORE0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM0_SP
// Description : If 1, SRAM0 can be accessed from a Secure, Privileged context.
#define ACCESSCTRL_SRAM0_SP_RESET  _u(0x1)
#define ACCESSCTRL_SRAM0_SP_BITS   _u(0x00000008)
#define ACCESSCTRL_SRAM0_SP_MSB    _u(3)
#define ACCESSCTRL_SRAM0_SP_LSB    _u(3)
#define ACCESSCTRL_SRAM0_SP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM0_SU
// Description : If 1, and SP is also set, SRAM0 can be accessed from a Secure,
//               Unprivileged context.
#define ACCESSCTRL_SRAM0_SU_RESET  _u(0x1)
#define ACCESSCTRL_SRAM0_SU_BITS   _u(0x00000004)
#define ACCESSCTRL_SRAM0_SU_MSB    _u(2)
#define ACCESSCTRL_SRAM0_SU_LSB    _u(2)
#define ACCESSCTRL_SRAM0_SU_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM0_NSP
// Description : If 1, SRAM0 can be accessed from a Non-secure, Privileged
//               context.
#define ACCESSCTRL_SRAM0_NSP_RESET  _u(0x1)
#define ACCESSCTRL_SRAM0_NSP_BITS   _u(0x00000002)
#define ACCESSCTRL_SRAM0_NSP_MSB    _u(1)
#define ACCESSCTRL_SRAM0_NSP_LSB    _u(1)
#define ACCESSCTRL_SRAM0_NSP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM0_NSU
// Description : If 1, and NSP is also set, SRAM0 can be accessed from a Non-
//               secure, Unprivileged context.
//
//               This bit is writable from a Non-secure, Privileged context, if
//               and only if the NSP bit is set.
#define ACCESSCTRL_SRAM0_NSU_RESET  _u(0x1)
#define ACCESSCTRL_SRAM0_NSU_BITS   _u(0x00000001)
#define ACCESSCTRL_SRAM0_NSU_MSB    _u(0)
#define ACCESSCTRL_SRAM0_NSU_LSB    _u(0)
#define ACCESSCTRL_SRAM0_NSU_ACCESS "RW"
// =============================================================================
// Register    : ACCESSCTRL_SRAM1
// Description : Control whether debugger, DMA, core 0 and core 1 can access
//               SRAM1, and at what security/privilege levels they can do so.
//
//               Defaults to fully open access.
//
//               This register is writable only from a Secure, Privileged
//               processor or debugger, with the exception of the NSU bit, which
//               becomes Non-secure-Privileged-writable when the NSP bit is set.
#define ACCESSCTRL_SRAM1_OFFSET _u(0x00000020)
#define ACCESSCTRL_SRAM1_BITS   _u(0x000000ff)
#define ACCESSCTRL_SRAM1_RESET  _u(0x000000ff)
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM1_DBG
// Description : If 1, SRAM1 can be accessed by the debugger, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_SRAM1_DBG_RESET  _u(0x1)
#define ACCESSCTRL_SRAM1_DBG_BITS   _u(0x00000080)
#define ACCESSCTRL_SRAM1_DBG_MSB    _u(7)
#define ACCESSCTRL_SRAM1_DBG_LSB    _u(7)
#define ACCESSCTRL_SRAM1_DBG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM1_DMA
// Description : If 1, SRAM1 can be accessed by the DMA, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_SRAM1_DMA_RESET  _u(0x1)
#define ACCESSCTRL_SRAM1_DMA_BITS   _u(0x00000040)
#define ACCESSCTRL_SRAM1_DMA_MSB    _u(6)
#define ACCESSCTRL_SRAM1_DMA_LSB    _u(6)
#define ACCESSCTRL_SRAM1_DMA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM1_CORE1
// Description : If 1, SRAM1 can be accessed by core 1, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_SRAM1_CORE1_RESET  _u(0x1)
#define ACCESSCTRL_SRAM1_CORE1_BITS   _u(0x00000020)
#define ACCESSCTRL_SRAM1_CORE1_MSB    _u(5)
#define ACCESSCTRL_SRAM1_CORE1_LSB    _u(5)
#define ACCESSCTRL_SRAM1_CORE1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM1_CORE0
// Description : If 1, SRAM1 can be accessed by core 0, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_SRAM1_CORE0_RESET  _u(0x1)
#define ACCESSCTRL_SRAM1_CORE0_BITS   _u(0x00000010)
#define ACCESSCTRL_SRAM1_CORE0_MSB    _u(4)
#define ACCESSCTRL_SRAM1_CORE0_LSB    _u(4)
#define ACCESSCTRL_SRAM1_CORE0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM1_SP
// Description : If 1, SRAM1 can be accessed from a Secure, Privileged context.
#define ACCESSCTRL_SRAM1_SP_RESET  _u(0x1)
#define ACCESSCTRL_SRAM1_SP_BITS   _u(0x00000008)
#define ACCESSCTRL_SRAM1_SP_MSB    _u(3)
#define ACCESSCTRL_SRAM1_SP_LSB    _u(3)
#define ACCESSCTRL_SRAM1_SP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM1_SU
// Description : If 1, and SP is also set, SRAM1 can be accessed from a Secure,
//               Unprivileged context.
#define ACCESSCTRL_SRAM1_SU_RESET  _u(0x1)
#define ACCESSCTRL_SRAM1_SU_BITS   _u(0x00000004)
#define ACCESSCTRL_SRAM1_SU_MSB    _u(2)
#define ACCESSCTRL_SRAM1_SU_LSB    _u(2)
#define ACCESSCTRL_SRAM1_SU_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM1_NSP
// Description : If 1, SRAM1 can be accessed from a Non-secure, Privileged
//               context.
#define ACCESSCTRL_SRAM1_NSP_RESET  _u(0x1)
#define ACCESSCTRL_SRAM1_NSP_BITS   _u(0x00000002)
#define ACCESSCTRL_SRAM1_NSP_MSB    _u(1)
#define ACCESSCTRL_SRAM1_NSP_LSB    _u(1)
#define ACCESSCTRL_SRAM1_NSP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM1_NSU
// Description : If 1, and NSP is also set, SRAM1 can be accessed from a Non-
//               secure, Unprivileged context.
//
//               This bit is writable from a Non-secure, Privileged context, if
//               and only if the NSP bit is set.
#define ACCESSCTRL_SRAM1_NSU_RESET  _u(0x1)
#define ACCESSCTRL_SRAM1_NSU_BITS   _u(0x00000001)
#define ACCESSCTRL_SRAM1_NSU_MSB    _u(0)
#define ACCESSCTRL_SRAM1_NSU_LSB    _u(0)
#define ACCESSCTRL_SRAM1_NSU_ACCESS "RW"
// =============================================================================
// Register    : ACCESSCTRL_SRAM2
// Description : Control whether debugger, DMA, core 0 and core 1 can access
//               SRAM2, and at what security/privilege levels they can do so.
//
//               Defaults to fully open access.
//
//               This register is writable only from a Secure, Privileged
//               processor or debugger, with the exception of the NSU bit, which
//               becomes Non-secure-Privileged-writable when the NSP bit is set.
#define ACCESSCTRL_SRAM2_OFFSET _u(0x00000024)
#define ACCESSCTRL_SRAM2_BITS   _u(0x000000ff)
#define ACCESSCTRL_SRAM2_RESET  _u(0x000000ff)
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM2_DBG
// Description : If 1, SRAM2 can be accessed by the debugger, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_SRAM2_DBG_RESET  _u(0x1)
#define ACCESSCTRL_SRAM2_DBG_BITS   _u(0x00000080)
#define ACCESSCTRL_SRAM2_DBG_MSB    _u(7)
#define ACCESSCTRL_SRAM2_DBG_LSB    _u(7)
#define ACCESSCTRL_SRAM2_DBG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM2_DMA
// Description : If 1, SRAM2 can be accessed by the DMA, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_SRAM2_DMA_RESET  _u(0x1)
#define ACCESSCTRL_SRAM2_DMA_BITS   _u(0x00000040)
#define ACCESSCTRL_SRAM2_DMA_MSB    _u(6)
#define ACCESSCTRL_SRAM2_DMA_LSB    _u(6)
#define ACCESSCTRL_SRAM2_DMA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM2_CORE1
// Description : If 1, SRAM2 can be accessed by core 1, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_SRAM2_CORE1_RESET  _u(0x1)
#define ACCESSCTRL_SRAM2_CORE1_BITS   _u(0x00000020)
#define ACCESSCTRL_SRAM2_CORE1_MSB    _u(5)
#define ACCESSCTRL_SRAM2_CORE1_LSB    _u(5)
#define ACCESSCTRL_SRAM2_CORE1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM2_CORE0
// Description : If 1, SRAM2 can be accessed by core 0, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_SRAM2_CORE0_RESET  _u(0x1)
#define ACCESSCTRL_SRAM2_CORE0_BITS   _u(0x00000010)
#define ACCESSCTRL_SRAM2_CORE0_MSB    _u(4)
#define ACCESSCTRL_SRAM2_CORE0_LSB    _u(4)
#define ACCESSCTRL_SRAM2_CORE0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM2_SP
// Description : If 1, SRAM2 can be accessed from a Secure, Privileged context.
#define ACCESSCTRL_SRAM2_SP_RESET  _u(0x1)
#define ACCESSCTRL_SRAM2_SP_BITS   _u(0x00000008)
#define ACCESSCTRL_SRAM2_SP_MSB    _u(3)
#define ACCESSCTRL_SRAM2_SP_LSB    _u(3)
#define ACCESSCTRL_SRAM2_SP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM2_SU
// Description : If 1, and SP is also set, SRAM2 can be accessed from a Secure,
//               Unprivileged context.
#define ACCESSCTRL_SRAM2_SU_RESET  _u(0x1)
#define ACCESSCTRL_SRAM2_SU_BITS   _u(0x00000004)
#define ACCESSCTRL_SRAM2_SU_MSB    _u(2)
#define ACCESSCTRL_SRAM2_SU_LSB    _u(2)
#define ACCESSCTRL_SRAM2_SU_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM2_NSP
// Description : If 1, SRAM2 can be accessed from a Non-secure, Privileged
//               context.
#define ACCESSCTRL_SRAM2_NSP_RESET  _u(0x1)
#define ACCESSCTRL_SRAM2_NSP_BITS   _u(0x00000002)
#define ACCESSCTRL_SRAM2_NSP_MSB    _u(1)
#define ACCESSCTRL_SRAM2_NSP_LSB    _u(1)
#define ACCESSCTRL_SRAM2_NSP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM2_NSU
// Description : If 1, and NSP is also set, SRAM2 can be accessed from a Non-
//               secure, Unprivileged context.
//
//               This bit is writable from a Non-secure, Privileged context, if
//               and only if the NSP bit is set.
#define ACCESSCTRL_SRAM2_NSU_RESET  _u(0x1)
#define ACCESSCTRL_SRAM2_NSU_BITS   _u(0x00000001)
#define ACCESSCTRL_SRAM2_NSU_MSB    _u(0)
#define ACCESSCTRL_SRAM2_NSU_LSB    _u(0)
#define ACCESSCTRL_SRAM2_NSU_ACCESS "RW"
// =============================================================================
// Register    : ACCESSCTRL_SRAM3
// Description : Control whether debugger, DMA, core 0 and core 1 can access
//               SRAM3, and at what security/privilege levels they can do so.
//
//               Defaults to fully open access.
//
//               This register is writable only from a Secure, Privileged
//               processor or debugger, with the exception of the NSU bit, which
//               becomes Non-secure-Privileged-writable when the NSP bit is set.
#define ACCESSCTRL_SRAM3_OFFSET _u(0x00000028)
#define ACCESSCTRL_SRAM3_BITS   _u(0x000000ff)
#define ACCESSCTRL_SRAM3_RESET  _u(0x000000ff)
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM3_DBG
// Description : If 1, SRAM3 can be accessed by the debugger, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_SRAM3_DBG_RESET  _u(0x1)
#define ACCESSCTRL_SRAM3_DBG_BITS   _u(0x00000080)
#define ACCESSCTRL_SRAM3_DBG_MSB    _u(7)
#define ACCESSCTRL_SRAM3_DBG_LSB    _u(7)
#define ACCESSCTRL_SRAM3_DBG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM3_DMA
// Description : If 1, SRAM3 can be accessed by the DMA, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_SRAM3_DMA_RESET  _u(0x1)
#define ACCESSCTRL_SRAM3_DMA_BITS   _u(0x00000040)
#define ACCESSCTRL_SRAM3_DMA_MSB    _u(6)
#define ACCESSCTRL_SRAM3_DMA_LSB    _u(6)
#define ACCESSCTRL_SRAM3_DMA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM3_CORE1
// Description : If 1, SRAM3 can be accessed by core 1, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_SRAM3_CORE1_RESET  _u(0x1)
#define ACCESSCTRL_SRAM3_CORE1_BITS   _u(0x00000020)
#define ACCESSCTRL_SRAM3_CORE1_MSB    _u(5)
#define ACCESSCTRL_SRAM3_CORE1_LSB    _u(5)
#define ACCESSCTRL_SRAM3_CORE1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM3_CORE0
// Description : If 1, SRAM3 can be accessed by core 0, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_SRAM3_CORE0_RESET  _u(0x1)
#define ACCESSCTRL_SRAM3_CORE0_BITS   _u(0x00000010)
#define ACCESSCTRL_SRAM3_CORE0_MSB    _u(4)
#define ACCESSCTRL_SRAM3_CORE0_LSB    _u(4)
#define ACCESSCTRL_SRAM3_CORE0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM3_SP
// Description : If 1, SRAM3 can be accessed from a Secure, Privileged context.
#define ACCESSCTRL_SRAM3_SP_RESET  _u(0x1)
#define ACCESSCTRL_SRAM3_SP_BITS   _u(0x00000008)
#define ACCESSCTRL_SRAM3_SP_MSB    _u(3)
#define ACCESSCTRL_SRAM3_SP_LSB    _u(3)
#define ACCESSCTRL_SRAM3_SP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM3_SU
// Description : If 1, and SP is also set, SRAM3 can be accessed from a Secure,
//               Unprivileged context.
#define ACCESSCTRL_SRAM3_SU_RESET  _u(0x1)
#define ACCESSCTRL_SRAM3_SU_BITS   _u(0x00000004)
#define ACCESSCTRL_SRAM3_SU_MSB    _u(2)
#define ACCESSCTRL_SRAM3_SU_LSB    _u(2)
#define ACCESSCTRL_SRAM3_SU_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM3_NSP
// Description : If 1, SRAM3 can be accessed from a Non-secure, Privileged
//               context.
#define ACCESSCTRL_SRAM3_NSP_RESET  _u(0x1)
#define ACCESSCTRL_SRAM3_NSP_BITS   _u(0x00000002)
#define ACCESSCTRL_SRAM3_NSP_MSB    _u(1)
#define ACCESSCTRL_SRAM3_NSP_LSB    _u(1)
#define ACCESSCTRL_SRAM3_NSP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM3_NSU
// Description : If 1, and NSP is also set, SRAM3 can be accessed from a Non-
//               secure, Unprivileged context.
//
//               This bit is writable from a Non-secure, Privileged context, if
//               and only if the NSP bit is set.
#define ACCESSCTRL_SRAM3_NSU_RESET  _u(0x1)
#define ACCESSCTRL_SRAM3_NSU_BITS   _u(0x00000001)
#define ACCESSCTRL_SRAM3_NSU_MSB    _u(0)
#define ACCESSCTRL_SRAM3_NSU_LSB    _u(0)
#define ACCESSCTRL_SRAM3_NSU_ACCESS "RW"
// =============================================================================
// Register    : ACCESSCTRL_SRAM4
// Description : Control whether debugger, DMA, core 0 and core 1 can access
//               SRAM4, and at what security/privilege levels they can do so.
//
//               Defaults to fully open access.
//
//               This register is writable only from a Secure, Privileged
//               processor or debugger, with the exception of the NSU bit, which
//               becomes Non-secure-Privileged-writable when the NSP bit is set.
#define ACCESSCTRL_SRAM4_OFFSET _u(0x0000002c)
#define ACCESSCTRL_SRAM4_BITS   _u(0x000000ff)
#define ACCESSCTRL_SRAM4_RESET  _u(0x000000ff)
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM4_DBG
// Description : If 1, SRAM4 can be accessed by the debugger, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_SRAM4_DBG_RESET  _u(0x1)
#define ACCESSCTRL_SRAM4_DBG_BITS   _u(0x00000080)
#define ACCESSCTRL_SRAM4_DBG_MSB    _u(7)
#define ACCESSCTRL_SRAM4_DBG_LSB    _u(7)
#define ACCESSCTRL_SRAM4_DBG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM4_DMA
// Description : If 1, SRAM4 can be accessed by the DMA, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_SRAM4_DMA_RESET  _u(0x1)
#define ACCESSCTRL_SRAM4_DMA_BITS   _u(0x00000040)
#define ACCESSCTRL_SRAM4_DMA_MSB    _u(6)
#define ACCESSCTRL_SRAM4_DMA_LSB    _u(6)
#define ACCESSCTRL_SRAM4_DMA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM4_CORE1
// Description : If 1, SRAM4 can be accessed by core 1, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_SRAM4_CORE1_RESET  _u(0x1)
#define ACCESSCTRL_SRAM4_CORE1_BITS   _u(0x00000020)
#define ACCESSCTRL_SRAM4_CORE1_MSB    _u(5)
#define ACCESSCTRL_SRAM4_CORE1_LSB    _u(5)
#define ACCESSCTRL_SRAM4_CORE1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM4_CORE0
// Description : If 1, SRAM4 can be accessed by core 0, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_SRAM4_CORE0_RESET  _u(0x1)
#define ACCESSCTRL_SRAM4_CORE0_BITS   _u(0x00000010)
#define ACCESSCTRL_SRAM4_CORE0_MSB    _u(4)
#define ACCESSCTRL_SRAM4_CORE0_LSB    _u(4)
#define ACCESSCTRL_SRAM4_CORE0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM4_SP
// Description : If 1, SRAM4 can be accessed from a Secure, Privileged context.
#define ACCESSCTRL_SRAM4_SP_RESET  _u(0x1)
#define ACCESSCTRL_SRAM4_SP_BITS   _u(0x00000008)
#define ACCESSCTRL_SRAM4_SP_MSB    _u(3)
#define ACCESSCTRL_SRAM4_SP_LSB    _u(3)
#define ACCESSCTRL_SRAM4_SP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM4_SU
// Description : If 1, and SP is also set, SRAM4 can be accessed from a Secure,
//               Unprivileged context.
#define ACCESSCTRL_SRAM4_SU_RESET  _u(0x1)
#define ACCESSCTRL_SRAM4_SU_BITS   _u(0x00000004)
#define ACCESSCTRL_SRAM4_SU_MSB    _u(2)
#define ACCESSCTRL_SRAM4_SU_LSB    _u(2)
#define ACCESSCTRL_SRAM4_SU_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM4_NSP
// Description : If 1, SRAM4 can be accessed from a Non-secure, Privileged
//               context.
#define ACCESSCTRL_SRAM4_NSP_RESET  _u(0x1)
#define ACCESSCTRL_SRAM4_NSP_BITS   _u(0x00000002)
#define ACCESSCTRL_SRAM4_NSP_MSB    _u(1)
#define ACCESSCTRL_SRAM4_NSP_LSB    _u(1)
#define ACCESSCTRL_SRAM4_NSP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM4_NSU
// Description : If 1, and NSP is also set, SRAM4 can be accessed from a Non-
//               secure, Unprivileged context.
//
//               This bit is writable from a Non-secure, Privileged context, if
//               and only if the NSP bit is set.
#define ACCESSCTRL_SRAM4_NSU_RESET  _u(0x1)
#define ACCESSCTRL_SRAM4_NSU_BITS   _u(0x00000001)
#define ACCESSCTRL_SRAM4_NSU_MSB    _u(0)
#define ACCESSCTRL_SRAM4_NSU_LSB    _u(0)
#define ACCESSCTRL_SRAM4_NSU_ACCESS "RW"
// =============================================================================
// Register    : ACCESSCTRL_SRAM5
// Description : Control whether debugger, DMA, core 0 and core 1 can access
//               SRAM5, and at what security/privilege levels they can do so.
//
//               Defaults to fully open access.
//
//               This register is writable only from a Secure, Privileged
//               processor or debugger, with the exception of the NSU bit, which
//               becomes Non-secure-Privileged-writable when the NSP bit is set.
#define ACCESSCTRL_SRAM5_OFFSET _u(0x00000030)
#define ACCESSCTRL_SRAM5_BITS   _u(0x000000ff)
#define ACCESSCTRL_SRAM5_RESET  _u(0x000000ff)
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM5_DBG
// Description : If 1, SRAM5 can be accessed by the debugger, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_SRAM5_DBG_RESET  _u(0x1)
#define ACCESSCTRL_SRAM5_DBG_BITS   _u(0x00000080)
#define ACCESSCTRL_SRAM5_DBG_MSB    _u(7)
#define ACCESSCTRL_SRAM5_DBG_LSB    _u(7)
#define ACCESSCTRL_SRAM5_DBG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM5_DMA
// Description : If 1, SRAM5 can be accessed by the DMA, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_SRAM5_DMA_RESET  _u(0x1)
#define ACCESSCTRL_SRAM5_DMA_BITS   _u(0x00000040)
#define ACCESSCTRL_SRAM5_DMA_MSB    _u(6)
#define ACCESSCTRL_SRAM5_DMA_LSB    _u(6)
#define ACCESSCTRL_SRAM5_DMA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM5_CORE1
// Description : If 1, SRAM5 can be accessed by core 1, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_SRAM5_CORE1_RESET  _u(0x1)
#define ACCESSCTRL_SRAM5_CORE1_BITS   _u(0x00000020)
#define ACCESSCTRL_SRAM5_CORE1_MSB    _u(5)
#define ACCESSCTRL_SRAM5_CORE1_LSB    _u(5)
#define ACCESSCTRL_SRAM5_CORE1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM5_CORE0
// Description : If 1, SRAM5 can be accessed by core 0, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_SRAM5_CORE0_RESET  _u(0x1)
#define ACCESSCTRL_SRAM5_CORE0_BITS   _u(0x00000010)
#define ACCESSCTRL_SRAM5_CORE0_MSB    _u(4)
#define ACCESSCTRL_SRAM5_CORE0_LSB    _u(4)
#define ACCESSCTRL_SRAM5_CORE0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM5_SP
// Description : If 1, SRAM5 can be accessed from a Secure, Privileged context.
#define ACCESSCTRL_SRAM5_SP_RESET  _u(0x1)
#define ACCESSCTRL_SRAM5_SP_BITS   _u(0x00000008)
#define ACCESSCTRL_SRAM5_SP_MSB    _u(3)
#define ACCESSCTRL_SRAM5_SP_LSB    _u(3)
#define ACCESSCTRL_SRAM5_SP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM5_SU
// Description : If 1, and SP is also set, SRAM5 can be accessed from a Secure,
//               Unprivileged context.
#define ACCESSCTRL_SRAM5_SU_RESET  _u(0x1)
#define ACCESSCTRL_SRAM5_SU_BITS   _u(0x00000004)
#define ACCESSCTRL_SRAM5_SU_MSB    _u(2)
#define ACCESSCTRL_SRAM5_SU_LSB    _u(2)
#define ACCESSCTRL_SRAM5_SU_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM5_NSP
// Description : If 1, SRAM5 can be accessed from a Non-secure, Privileged
//               context.
#define ACCESSCTRL_SRAM5_NSP_RESET  _u(0x1)
#define ACCESSCTRL_SRAM5_NSP_BITS   _u(0x00000002)
#define ACCESSCTRL_SRAM5_NSP_MSB    _u(1)
#define ACCESSCTRL_SRAM5_NSP_LSB    _u(1)
#define ACCESSCTRL_SRAM5_NSP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM5_NSU
// Description : If 1, and NSP is also set, SRAM5 can be accessed from a Non-
//               secure, Unprivileged context.
//
//               This bit is writable from a Non-secure, Privileged context, if
//               and only if the NSP bit is set.
#define ACCESSCTRL_SRAM5_NSU_RESET  _u(0x1)
#define ACCESSCTRL_SRAM5_NSU_BITS   _u(0x00000001)
#define ACCESSCTRL_SRAM5_NSU_MSB    _u(0)
#define ACCESSCTRL_SRAM5_NSU_LSB    _u(0)
#define ACCESSCTRL_SRAM5_NSU_ACCESS "RW"
// =============================================================================
// Register    : ACCESSCTRL_SRAM6
// Description : Control whether debugger, DMA, core 0 and core 1 can access
//               SRAM6, and at what security/privilege levels they can do so.
//
//               Defaults to fully open access.
//
//               This register is writable only from a Secure, Privileged
//               processor or debugger, with the exception of the NSU bit, which
//               becomes Non-secure-Privileged-writable when the NSP bit is set.
#define ACCESSCTRL_SRAM6_OFFSET _u(0x00000034)
#define ACCESSCTRL_SRAM6_BITS   _u(0x000000ff)
#define ACCESSCTRL_SRAM6_RESET  _u(0x000000ff)
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM6_DBG
// Description : If 1, SRAM6 can be accessed by the debugger, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_SRAM6_DBG_RESET  _u(0x1)
#define ACCESSCTRL_SRAM6_DBG_BITS   _u(0x00000080)
#define ACCESSCTRL_SRAM6_DBG_MSB    _u(7)
#define ACCESSCTRL_SRAM6_DBG_LSB    _u(7)
#define ACCESSCTRL_SRAM6_DBG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM6_DMA
// Description : If 1, SRAM6 can be accessed by the DMA, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_SRAM6_DMA_RESET  _u(0x1)
#define ACCESSCTRL_SRAM6_DMA_BITS   _u(0x00000040)
#define ACCESSCTRL_SRAM6_DMA_MSB    _u(6)
#define ACCESSCTRL_SRAM6_DMA_LSB    _u(6)
#define ACCESSCTRL_SRAM6_DMA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM6_CORE1
// Description : If 1, SRAM6 can be accessed by core 1, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_SRAM6_CORE1_RESET  _u(0x1)
#define ACCESSCTRL_SRAM6_CORE1_BITS   _u(0x00000020)
#define ACCESSCTRL_SRAM6_CORE1_MSB    _u(5)
#define ACCESSCTRL_SRAM6_CORE1_LSB    _u(5)
#define ACCESSCTRL_SRAM6_CORE1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM6_CORE0
// Description : If 1, SRAM6 can be accessed by core 0, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_SRAM6_CORE0_RESET  _u(0x1)
#define ACCESSCTRL_SRAM6_CORE0_BITS   _u(0x00000010)
#define ACCESSCTRL_SRAM6_CORE0_MSB    _u(4)
#define ACCESSCTRL_SRAM6_CORE0_LSB    _u(4)
#define ACCESSCTRL_SRAM6_CORE0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM6_SP
// Description : If 1, SRAM6 can be accessed from a Secure, Privileged context.
#define ACCESSCTRL_SRAM6_SP_RESET  _u(0x1)
#define ACCESSCTRL_SRAM6_SP_BITS   _u(0x00000008)
#define ACCESSCTRL_SRAM6_SP_MSB    _u(3)
#define ACCESSCTRL_SRAM6_SP_LSB    _u(3)
#define ACCESSCTRL_SRAM6_SP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM6_SU
// Description : If 1, and SP is also set, SRAM6 can be accessed from a Secure,
//               Unprivileged context.
#define ACCESSCTRL_SRAM6_SU_RESET  _u(0x1)
#define ACCESSCTRL_SRAM6_SU_BITS   _u(0x00000004)
#define ACCESSCTRL_SRAM6_SU_MSB    _u(2)
#define ACCESSCTRL_SRAM6_SU_LSB    _u(2)
#define ACCESSCTRL_SRAM6_SU_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM6_NSP
// Description : If 1, SRAM6 can be accessed from a Non-secure, Privileged
//               context.
#define ACCESSCTRL_SRAM6_NSP_RESET  _u(0x1)
#define ACCESSCTRL_SRAM6_NSP_BITS   _u(0x00000002)
#define ACCESSCTRL_SRAM6_NSP_MSB    _u(1)
#define ACCESSCTRL_SRAM6_NSP_LSB    _u(1)
#define ACCESSCTRL_SRAM6_NSP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM6_NSU
// Description : If 1, and NSP is also set, SRAM6 can be accessed from a Non-
//               secure, Unprivileged context.
//
//               This bit is writable from a Non-secure, Privileged context, if
//               and only if the NSP bit is set.
#define ACCESSCTRL_SRAM6_NSU_RESET  _u(0x1)
#define ACCESSCTRL_SRAM6_NSU_BITS   _u(0x00000001)
#define ACCESSCTRL_SRAM6_NSU_MSB    _u(0)
#define ACCESSCTRL_SRAM6_NSU_LSB    _u(0)
#define ACCESSCTRL_SRAM6_NSU_ACCESS "RW"
// =============================================================================
// Register    : ACCESSCTRL_SRAM7
// Description : Control whether debugger, DMA, core 0 and core 1 can access
//               SRAM7, and at what security/privilege levels they can do so.
//
//               Defaults to fully open access.
//
//               This register is writable only from a Secure, Privileged
//               processor or debugger, with the exception of the NSU bit, which
//               becomes Non-secure-Privileged-writable when the NSP bit is set.
#define ACCESSCTRL_SRAM7_OFFSET _u(0x00000038)
#define ACCESSCTRL_SRAM7_BITS   _u(0x000000ff)
#define ACCESSCTRL_SRAM7_RESET  _u(0x000000ff)
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM7_DBG
// Description : If 1, SRAM7 can be accessed by the debugger, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_SRAM7_DBG_RESET  _u(0x1)
#define ACCESSCTRL_SRAM7_DBG_BITS   _u(0x00000080)
#define ACCESSCTRL_SRAM7_DBG_MSB    _u(7)
#define ACCESSCTRL_SRAM7_DBG_LSB    _u(7)
#define ACCESSCTRL_SRAM7_DBG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM7_DMA
// Description : If 1, SRAM7 can be accessed by the DMA, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_SRAM7_DMA_RESET  _u(0x1)
#define ACCESSCTRL_SRAM7_DMA_BITS   _u(0x00000040)
#define ACCESSCTRL_SRAM7_DMA_MSB    _u(6)
#define ACCESSCTRL_SRAM7_DMA_LSB    _u(6)
#define ACCESSCTRL_SRAM7_DMA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM7_CORE1
// Description : If 1, SRAM7 can be accessed by core 1, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_SRAM7_CORE1_RESET  _u(0x1)
#define ACCESSCTRL_SRAM7_CORE1_BITS   _u(0x00000020)
#define ACCESSCTRL_SRAM7_CORE1_MSB    _u(5)
#define ACCESSCTRL_SRAM7_CORE1_LSB    _u(5)
#define ACCESSCTRL_SRAM7_CORE1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM7_CORE0
// Description : If 1, SRAM7 can be accessed by core 0, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_SRAM7_CORE0_RESET  _u(0x1)
#define ACCESSCTRL_SRAM7_CORE0_BITS   _u(0x00000010)
#define ACCESSCTRL_SRAM7_CORE0_MSB    _u(4)
#define ACCESSCTRL_SRAM7_CORE0_LSB    _u(4)
#define ACCESSCTRL_SRAM7_CORE0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM7_SP
// Description : If 1, SRAM7 can be accessed from a Secure, Privileged context.
#define ACCESSCTRL_SRAM7_SP_RESET  _u(0x1)
#define ACCESSCTRL_SRAM7_SP_BITS   _u(0x00000008)
#define ACCESSCTRL_SRAM7_SP_MSB    _u(3)
#define ACCESSCTRL_SRAM7_SP_LSB    _u(3)
#define ACCESSCTRL_SRAM7_SP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM7_SU
// Description : If 1, and SP is also set, SRAM7 can be accessed from a Secure,
//               Unprivileged context.
#define ACCESSCTRL_SRAM7_SU_RESET  _u(0x1)
#define ACCESSCTRL_SRAM7_SU_BITS   _u(0x00000004)
#define ACCESSCTRL_SRAM7_SU_MSB    _u(2)
#define ACCESSCTRL_SRAM7_SU_LSB    _u(2)
#define ACCESSCTRL_SRAM7_SU_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM7_NSP
// Description : If 1, SRAM7 can be accessed from a Non-secure, Privileged
//               context.
#define ACCESSCTRL_SRAM7_NSP_RESET  _u(0x1)
#define ACCESSCTRL_SRAM7_NSP_BITS   _u(0x00000002)
#define ACCESSCTRL_SRAM7_NSP_MSB    _u(1)
#define ACCESSCTRL_SRAM7_NSP_LSB    _u(1)
#define ACCESSCTRL_SRAM7_NSP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM7_NSU
// Description : If 1, and NSP is also set, SRAM7 can be accessed from a Non-
//               secure, Unprivileged context.
//
//               This bit is writable from a Non-secure, Privileged context, if
//               and only if the NSP bit is set.
#define ACCESSCTRL_SRAM7_NSU_RESET  _u(0x1)
#define ACCESSCTRL_SRAM7_NSU_BITS   _u(0x00000001)
#define ACCESSCTRL_SRAM7_NSU_MSB    _u(0)
#define ACCESSCTRL_SRAM7_NSU_LSB    _u(0)
#define ACCESSCTRL_SRAM7_NSU_ACCESS "RW"
// =============================================================================
// Register    : ACCESSCTRL_SRAM8
// Description : Control whether debugger, DMA, core 0 and core 1 can access
//               SRAM8, and at what security/privilege levels they can do so.
//
//               Defaults to fully open access.
//
//               This register is writable only from a Secure, Privileged
//               processor or debugger, with the exception of the NSU bit, which
//               becomes Non-secure-Privileged-writable when the NSP bit is set.
#define ACCESSCTRL_SRAM8_OFFSET _u(0x0000003c)
#define ACCESSCTRL_SRAM8_BITS   _u(0x000000ff)
#define ACCESSCTRL_SRAM8_RESET  _u(0x000000ff)
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM8_DBG
// Description : If 1, SRAM8 can be accessed by the debugger, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_SRAM8_DBG_RESET  _u(0x1)
#define ACCESSCTRL_SRAM8_DBG_BITS   _u(0x00000080)
#define ACCESSCTRL_SRAM8_DBG_MSB    _u(7)
#define ACCESSCTRL_SRAM8_DBG_LSB    _u(7)
#define ACCESSCTRL_SRAM8_DBG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM8_DMA
// Description : If 1, SRAM8 can be accessed by the DMA, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_SRAM8_DMA_RESET  _u(0x1)
#define ACCESSCTRL_SRAM8_DMA_BITS   _u(0x00000040)
#define ACCESSCTRL_SRAM8_DMA_MSB    _u(6)
#define ACCESSCTRL_SRAM8_DMA_LSB    _u(6)
#define ACCESSCTRL_SRAM8_DMA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM8_CORE1
// Description : If 1, SRAM8 can be accessed by core 1, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_SRAM8_CORE1_RESET  _u(0x1)
#define ACCESSCTRL_SRAM8_CORE1_BITS   _u(0x00000020)
#define ACCESSCTRL_SRAM8_CORE1_MSB    _u(5)
#define ACCESSCTRL_SRAM8_CORE1_LSB    _u(5)
#define ACCESSCTRL_SRAM8_CORE1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM8_CORE0
// Description : If 1, SRAM8 can be accessed by core 0, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_SRAM8_CORE0_RESET  _u(0x1)
#define ACCESSCTRL_SRAM8_CORE0_BITS   _u(0x00000010)
#define ACCESSCTRL_SRAM8_CORE0_MSB    _u(4)
#define ACCESSCTRL_SRAM8_CORE0_LSB    _u(4)
#define ACCESSCTRL_SRAM8_CORE0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM8_SP
// Description : If 1, SRAM8 can be accessed from a Secure, Privileged context.
#define ACCESSCTRL_SRAM8_SP_RESET  _u(0x1)
#define ACCESSCTRL_SRAM8_SP_BITS   _u(0x00000008)
#define ACCESSCTRL_SRAM8_SP_MSB    _u(3)
#define ACCESSCTRL_SRAM8_SP_LSB    _u(3)
#define ACCESSCTRL_SRAM8_SP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM8_SU
// Description : If 1, and SP is also set, SRAM8 can be accessed from a Secure,
//               Unprivileged context.
#define ACCESSCTRL_SRAM8_SU_RESET  _u(0x1)
#define ACCESSCTRL_SRAM8_SU_BITS   _u(0x00000004)
#define ACCESSCTRL_SRAM8_SU_MSB    _u(2)
#define ACCESSCTRL_SRAM8_SU_LSB    _u(2)
#define ACCESSCTRL_SRAM8_SU_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM8_NSP
// Description : If 1, SRAM8 can be accessed from a Non-secure, Privileged
//               context.
#define ACCESSCTRL_SRAM8_NSP_RESET  _u(0x1)
#define ACCESSCTRL_SRAM8_NSP_BITS   _u(0x00000002)
#define ACCESSCTRL_SRAM8_NSP_MSB    _u(1)
#define ACCESSCTRL_SRAM8_NSP_LSB    _u(1)
#define ACCESSCTRL_SRAM8_NSP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM8_NSU
// Description : If 1, and NSP is also set, SRAM8 can be accessed from a Non-
//               secure, Unprivileged context.
//
//               This bit is writable from a Non-secure, Privileged context, if
//               and only if the NSP bit is set.
#define ACCESSCTRL_SRAM8_NSU_RESET  _u(0x1)
#define ACCESSCTRL_SRAM8_NSU_BITS   _u(0x00000001)
#define ACCESSCTRL_SRAM8_NSU_MSB    _u(0)
#define ACCESSCTRL_SRAM8_NSU_LSB    _u(0)
#define ACCESSCTRL_SRAM8_NSU_ACCESS "RW"
// =============================================================================
// Register    : ACCESSCTRL_SRAM9
// Description : Control whether debugger, DMA, core 0 and core 1 can access
//               SRAM9, and at what security/privilege levels they can do so.
//
//               Defaults to fully open access.
//
//               This register is writable only from a Secure, Privileged
//               processor or debugger, with the exception of the NSU bit, which
//               becomes Non-secure-Privileged-writable when the NSP bit is set.
#define ACCESSCTRL_SRAM9_OFFSET _u(0x00000040)
#define ACCESSCTRL_SRAM9_BITS   _u(0x000000ff)
#define ACCESSCTRL_SRAM9_RESET  _u(0x000000ff)
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM9_DBG
// Description : If 1, SRAM9 can be accessed by the debugger, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_SRAM9_DBG_RESET  _u(0x1)
#define ACCESSCTRL_SRAM9_DBG_BITS   _u(0x00000080)
#define ACCESSCTRL_SRAM9_DBG_MSB    _u(7)
#define ACCESSCTRL_SRAM9_DBG_LSB    _u(7)
#define ACCESSCTRL_SRAM9_DBG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM9_DMA
// Description : If 1, SRAM9 can be accessed by the DMA, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_SRAM9_DMA_RESET  _u(0x1)
#define ACCESSCTRL_SRAM9_DMA_BITS   _u(0x00000040)
#define ACCESSCTRL_SRAM9_DMA_MSB    _u(6)
#define ACCESSCTRL_SRAM9_DMA_LSB    _u(6)
#define ACCESSCTRL_SRAM9_DMA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM9_CORE1
// Description : If 1, SRAM9 can be accessed by core 1, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_SRAM9_CORE1_RESET  _u(0x1)
#define ACCESSCTRL_SRAM9_CORE1_BITS   _u(0x00000020)
#define ACCESSCTRL_SRAM9_CORE1_MSB    _u(5)
#define ACCESSCTRL_SRAM9_CORE1_LSB    _u(5)
#define ACCESSCTRL_SRAM9_CORE1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM9_CORE0
// Description : If 1, SRAM9 can be accessed by core 0, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_SRAM9_CORE0_RESET  _u(0x1)
#define ACCESSCTRL_SRAM9_CORE0_BITS   _u(0x00000010)
#define ACCESSCTRL_SRAM9_CORE0_MSB    _u(4)
#define ACCESSCTRL_SRAM9_CORE0_LSB    _u(4)
#define ACCESSCTRL_SRAM9_CORE0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM9_SP
// Description : If 1, SRAM9 can be accessed from a Secure, Privileged context.
#define ACCESSCTRL_SRAM9_SP_RESET  _u(0x1)
#define ACCESSCTRL_SRAM9_SP_BITS   _u(0x00000008)
#define ACCESSCTRL_SRAM9_SP_MSB    _u(3)
#define ACCESSCTRL_SRAM9_SP_LSB    _u(3)
#define ACCESSCTRL_SRAM9_SP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM9_SU
// Description : If 1, and SP is also set, SRAM9 can be accessed from a Secure,
//               Unprivileged context.
#define ACCESSCTRL_SRAM9_SU_RESET  _u(0x1)
#define ACCESSCTRL_SRAM9_SU_BITS   _u(0x00000004)
#define ACCESSCTRL_SRAM9_SU_MSB    _u(2)
#define ACCESSCTRL_SRAM9_SU_LSB    _u(2)
#define ACCESSCTRL_SRAM9_SU_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM9_NSP
// Description : If 1, SRAM9 can be accessed from a Non-secure, Privileged
//               context.
#define ACCESSCTRL_SRAM9_NSP_RESET  _u(0x1)
#define ACCESSCTRL_SRAM9_NSP_BITS   _u(0x00000002)
#define ACCESSCTRL_SRAM9_NSP_MSB    _u(1)
#define ACCESSCTRL_SRAM9_NSP_LSB    _u(1)
#define ACCESSCTRL_SRAM9_NSP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SRAM9_NSU
// Description : If 1, and NSP is also set, SRAM9 can be accessed from a Non-
//               secure, Unprivileged context.
//
//               This bit is writable from a Non-secure, Privileged context, if
//               and only if the NSP bit is set.
#define ACCESSCTRL_SRAM9_NSU_RESET  _u(0x1)
#define ACCESSCTRL_SRAM9_NSU_BITS   _u(0x00000001)
#define ACCESSCTRL_SRAM9_NSU_MSB    _u(0)
#define ACCESSCTRL_SRAM9_NSU_LSB    _u(0)
#define ACCESSCTRL_SRAM9_NSU_ACCESS "RW"
// =============================================================================
// Register    : ACCESSCTRL_DMA
// Description : Control whether debugger, DMA, core 0 and core 1 can access
//               DMA, and at what security/privilege levels they can do so.
//
//               Defaults to Secure access from any master.
//
//               This register is writable only from a Secure, Privileged
//               processor or debugger, with the exception of the NSU bit, which
//               becomes Non-secure-Privileged-writable when the NSP bit is set.
#define ACCESSCTRL_DMA_OFFSET _u(0x00000044)
#define ACCESSCTRL_DMA_BITS   _u(0x000000ff)
#define ACCESSCTRL_DMA_RESET  _u(0x000000fc)
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_DMA_DBG
// Description : If 1, DMA can be accessed by the debugger, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_DMA_DBG_RESET  _u(0x1)
#define ACCESSCTRL_DMA_DBG_BITS   _u(0x00000080)
#define ACCESSCTRL_DMA_DBG_MSB    _u(7)
#define ACCESSCTRL_DMA_DBG_LSB    _u(7)
#define ACCESSCTRL_DMA_DBG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_DMA_DMA
// Description : If 1, DMA can be accessed by the DMA, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_DMA_DMA_RESET  _u(0x1)
#define ACCESSCTRL_DMA_DMA_BITS   _u(0x00000040)
#define ACCESSCTRL_DMA_DMA_MSB    _u(6)
#define ACCESSCTRL_DMA_DMA_LSB    _u(6)
#define ACCESSCTRL_DMA_DMA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_DMA_CORE1
// Description : If 1, DMA can be accessed by core 1, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_DMA_CORE1_RESET  _u(0x1)
#define ACCESSCTRL_DMA_CORE1_BITS   _u(0x00000020)
#define ACCESSCTRL_DMA_CORE1_MSB    _u(5)
#define ACCESSCTRL_DMA_CORE1_LSB    _u(5)
#define ACCESSCTRL_DMA_CORE1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_DMA_CORE0
// Description : If 1, DMA can be accessed by core 0, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_DMA_CORE0_RESET  _u(0x1)
#define ACCESSCTRL_DMA_CORE0_BITS   _u(0x00000010)
#define ACCESSCTRL_DMA_CORE0_MSB    _u(4)
#define ACCESSCTRL_DMA_CORE0_LSB    _u(4)
#define ACCESSCTRL_DMA_CORE0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_DMA_SP
// Description : If 1, DMA can be accessed from a Secure, Privileged context.
#define ACCESSCTRL_DMA_SP_RESET  _u(0x1)
#define ACCESSCTRL_DMA_SP_BITS   _u(0x00000008)
#define ACCESSCTRL_DMA_SP_MSB    _u(3)
#define ACCESSCTRL_DMA_SP_LSB    _u(3)
#define ACCESSCTRL_DMA_SP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_DMA_SU
// Description : If 1, and SP is also set, DMA can be accessed from a Secure,
//               Unprivileged context.
#define ACCESSCTRL_DMA_SU_RESET  _u(0x1)
#define ACCESSCTRL_DMA_SU_BITS   _u(0x00000004)
#define ACCESSCTRL_DMA_SU_MSB    _u(2)
#define ACCESSCTRL_DMA_SU_LSB    _u(2)
#define ACCESSCTRL_DMA_SU_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_DMA_NSP
// Description : If 1, DMA can be accessed from a Non-secure, Privileged
//               context.
#define ACCESSCTRL_DMA_NSP_RESET  _u(0x0)
#define ACCESSCTRL_DMA_NSP_BITS   _u(0x00000002)
#define ACCESSCTRL_DMA_NSP_MSB    _u(1)
#define ACCESSCTRL_DMA_NSP_LSB    _u(1)
#define ACCESSCTRL_DMA_NSP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_DMA_NSU
// Description : If 1, and NSP is also set, DMA can be accessed from a Non-
//               secure, Unprivileged context.
//
//               This bit is writable from a Non-secure, Privileged context, if
//               and only if the NSP bit is set.
#define ACCESSCTRL_DMA_NSU_RESET  _u(0x0)
#define ACCESSCTRL_DMA_NSU_BITS   _u(0x00000001)
#define ACCESSCTRL_DMA_NSU_MSB    _u(0)
#define ACCESSCTRL_DMA_NSU_LSB    _u(0)
#define ACCESSCTRL_DMA_NSU_ACCESS "RW"
// =============================================================================
// Register    : ACCESSCTRL_USBCTRL
// Description : Control whether debugger, DMA, core 0 and core 1 can access
//               USBCTRL, and at what security/privilege levels they can do so.
//
//               Defaults to Secure access from any master.
//
//               This register is writable only from a Secure, Privileged
//               processor or debugger, with the exception of the NSU bit, which
//               becomes Non-secure-Privileged-writable when the NSP bit is set.
#define ACCESSCTRL_USBCTRL_OFFSET _u(0x00000048)
#define ACCESSCTRL_USBCTRL_BITS   _u(0x000000ff)
#define ACCESSCTRL_USBCTRL_RESET  _u(0x000000fc)
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_USBCTRL_DBG
// Description : If 1, USBCTRL can be accessed by the debugger, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_USBCTRL_DBG_RESET  _u(0x1)
#define ACCESSCTRL_USBCTRL_DBG_BITS   _u(0x00000080)
#define ACCESSCTRL_USBCTRL_DBG_MSB    _u(7)
#define ACCESSCTRL_USBCTRL_DBG_LSB    _u(7)
#define ACCESSCTRL_USBCTRL_DBG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_USBCTRL_DMA
// Description : If 1, USBCTRL can be accessed by the DMA, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_USBCTRL_DMA_RESET  _u(0x1)
#define ACCESSCTRL_USBCTRL_DMA_BITS   _u(0x00000040)
#define ACCESSCTRL_USBCTRL_DMA_MSB    _u(6)
#define ACCESSCTRL_USBCTRL_DMA_LSB    _u(6)
#define ACCESSCTRL_USBCTRL_DMA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_USBCTRL_CORE1
// Description : If 1, USBCTRL can be accessed by core 1, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_USBCTRL_CORE1_RESET  _u(0x1)
#define ACCESSCTRL_USBCTRL_CORE1_BITS   _u(0x00000020)
#define ACCESSCTRL_USBCTRL_CORE1_MSB    _u(5)
#define ACCESSCTRL_USBCTRL_CORE1_LSB    _u(5)
#define ACCESSCTRL_USBCTRL_CORE1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_USBCTRL_CORE0
// Description : If 1, USBCTRL can be accessed by core 0, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_USBCTRL_CORE0_RESET  _u(0x1)
#define ACCESSCTRL_USBCTRL_CORE0_BITS   _u(0x00000010)
#define ACCESSCTRL_USBCTRL_CORE0_MSB    _u(4)
#define ACCESSCTRL_USBCTRL_CORE0_LSB    _u(4)
#define ACCESSCTRL_USBCTRL_CORE0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_USBCTRL_SP
// Description : If 1, USBCTRL can be accessed from a Secure, Privileged
//               context.
#define ACCESSCTRL_USBCTRL_SP_RESET  _u(0x1)
#define ACCESSCTRL_USBCTRL_SP_BITS   _u(0x00000008)
#define ACCESSCTRL_USBCTRL_SP_MSB    _u(3)
#define ACCESSCTRL_USBCTRL_SP_LSB    _u(3)
#define ACCESSCTRL_USBCTRL_SP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_USBCTRL_SU
// Description : If 1, and SP is also set, USBCTRL can be accessed from a
//               Secure, Unprivileged context.
#define ACCESSCTRL_USBCTRL_SU_RESET  _u(0x1)
#define ACCESSCTRL_USBCTRL_SU_BITS   _u(0x00000004)
#define ACCESSCTRL_USBCTRL_SU_MSB    _u(2)
#define ACCESSCTRL_USBCTRL_SU_LSB    _u(2)
#define ACCESSCTRL_USBCTRL_SU_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_USBCTRL_NSP
// Description : If 1, USBCTRL can be accessed from a Non-secure, Privileged
//               context.
#define ACCESSCTRL_USBCTRL_NSP_RESET  _u(0x0)
#define ACCESSCTRL_USBCTRL_NSP_BITS   _u(0x00000002)
#define ACCESSCTRL_USBCTRL_NSP_MSB    _u(1)
#define ACCESSCTRL_USBCTRL_NSP_LSB    _u(1)
#define ACCESSCTRL_USBCTRL_NSP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_USBCTRL_NSU
// Description : If 1, and NSP is also set, USBCTRL can be accessed from a Non-
//               secure, Unprivileged context.
//
//               This bit is writable from a Non-secure, Privileged context, if
//               and only if the NSP bit is set.
#define ACCESSCTRL_USBCTRL_NSU_RESET  _u(0x0)
#define ACCESSCTRL_USBCTRL_NSU_BITS   _u(0x00000001)
#define ACCESSCTRL_USBCTRL_NSU_MSB    _u(0)
#define ACCESSCTRL_USBCTRL_NSU_LSB    _u(0)
#define ACCESSCTRL_USBCTRL_NSU_ACCESS "RW"
// =============================================================================
// Register    : ACCESSCTRL_PIO0
// Description : Control whether debugger, DMA, core 0 and core 1 can access
//               PIO0, and at what security/privilege levels they can do so.
//
//               Defaults to Secure access from any master.
//
//               This register is writable only from a Secure, Privileged
//               processor or debugger, with the exception of the NSU bit, which
//               becomes Non-secure-Privileged-writable when the NSP bit is set.
#define ACCESSCTRL_PIO0_OFFSET _u(0x0000004c)
#define ACCESSCTRL_PIO0_BITS   _u(0x000000ff)
#define ACCESSCTRL_PIO0_RESET  _u(0x000000fc)
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PIO0_DBG
// Description : If 1, PIO0 can be accessed by the debugger, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_PIO0_DBG_RESET  _u(0x1)
#define ACCESSCTRL_PIO0_DBG_BITS   _u(0x00000080)
#define ACCESSCTRL_PIO0_DBG_MSB    _u(7)
#define ACCESSCTRL_PIO0_DBG_LSB    _u(7)
#define ACCESSCTRL_PIO0_DBG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PIO0_DMA
// Description : If 1, PIO0 can be accessed by the DMA, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_PIO0_DMA_RESET  _u(0x1)
#define ACCESSCTRL_PIO0_DMA_BITS   _u(0x00000040)
#define ACCESSCTRL_PIO0_DMA_MSB    _u(6)
#define ACCESSCTRL_PIO0_DMA_LSB    _u(6)
#define ACCESSCTRL_PIO0_DMA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PIO0_CORE1
// Description : If 1, PIO0 can be accessed by core 1, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_PIO0_CORE1_RESET  _u(0x1)
#define ACCESSCTRL_PIO0_CORE1_BITS   _u(0x00000020)
#define ACCESSCTRL_PIO0_CORE1_MSB    _u(5)
#define ACCESSCTRL_PIO0_CORE1_LSB    _u(5)
#define ACCESSCTRL_PIO0_CORE1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PIO0_CORE0
// Description : If 1, PIO0 can be accessed by core 0, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_PIO0_CORE0_RESET  _u(0x1)
#define ACCESSCTRL_PIO0_CORE0_BITS   _u(0x00000010)
#define ACCESSCTRL_PIO0_CORE0_MSB    _u(4)
#define ACCESSCTRL_PIO0_CORE0_LSB    _u(4)
#define ACCESSCTRL_PIO0_CORE0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PIO0_SP
// Description : If 1, PIO0 can be accessed from a Secure, Privileged context.
#define ACCESSCTRL_PIO0_SP_RESET  _u(0x1)
#define ACCESSCTRL_PIO0_SP_BITS   _u(0x00000008)
#define ACCESSCTRL_PIO0_SP_MSB    _u(3)
#define ACCESSCTRL_PIO0_SP_LSB    _u(3)
#define ACCESSCTRL_PIO0_SP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PIO0_SU
// Description : If 1, and SP is also set, PIO0 can be accessed from a Secure,
//               Unprivileged context.
#define ACCESSCTRL_PIO0_SU_RESET  _u(0x1)
#define ACCESSCTRL_PIO0_SU_BITS   _u(0x00000004)
#define ACCESSCTRL_PIO0_SU_MSB    _u(2)
#define ACCESSCTRL_PIO0_SU_LSB    _u(2)
#define ACCESSCTRL_PIO0_SU_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PIO0_NSP
// Description : If 1, PIO0 can be accessed from a Non-secure, Privileged
//               context.
#define ACCESSCTRL_PIO0_NSP_RESET  _u(0x0)
#define ACCESSCTRL_PIO0_NSP_BITS   _u(0x00000002)
#define ACCESSCTRL_PIO0_NSP_MSB    _u(1)
#define ACCESSCTRL_PIO0_NSP_LSB    _u(1)
#define ACCESSCTRL_PIO0_NSP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PIO0_NSU
// Description : If 1, and NSP is also set, PIO0 can be accessed from a Non-
//               secure, Unprivileged context.
//
//               This bit is writable from a Non-secure, Privileged context, if
//               and only if the NSP bit is set.
#define ACCESSCTRL_PIO0_NSU_RESET  _u(0x0)
#define ACCESSCTRL_PIO0_NSU_BITS   _u(0x00000001)
#define ACCESSCTRL_PIO0_NSU_MSB    _u(0)
#define ACCESSCTRL_PIO0_NSU_LSB    _u(0)
#define ACCESSCTRL_PIO0_NSU_ACCESS "RW"
// =============================================================================
// Register    : ACCESSCTRL_PIO1
// Description : Control whether debugger, DMA, core 0 and core 1 can access
//               PIO1, and at what security/privilege levels they can do so.
//
//               Defaults to Secure access from any master.
//
//               This register is writable only from a Secure, Privileged
//               processor or debugger, with the exception of the NSU bit, which
//               becomes Non-secure-Privileged-writable when the NSP bit is set.
#define ACCESSCTRL_PIO1_OFFSET _u(0x00000050)
#define ACCESSCTRL_PIO1_BITS   _u(0x000000ff)
#define ACCESSCTRL_PIO1_RESET  _u(0x000000fc)
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PIO1_DBG
// Description : If 1, PIO1 can be accessed by the debugger, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_PIO1_DBG_RESET  _u(0x1)
#define ACCESSCTRL_PIO1_DBG_BITS   _u(0x00000080)
#define ACCESSCTRL_PIO1_DBG_MSB    _u(7)
#define ACCESSCTRL_PIO1_DBG_LSB    _u(7)
#define ACCESSCTRL_PIO1_DBG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PIO1_DMA
// Description : If 1, PIO1 can be accessed by the DMA, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_PIO1_DMA_RESET  _u(0x1)
#define ACCESSCTRL_PIO1_DMA_BITS   _u(0x00000040)
#define ACCESSCTRL_PIO1_DMA_MSB    _u(6)
#define ACCESSCTRL_PIO1_DMA_LSB    _u(6)
#define ACCESSCTRL_PIO1_DMA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PIO1_CORE1
// Description : If 1, PIO1 can be accessed by core 1, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_PIO1_CORE1_RESET  _u(0x1)
#define ACCESSCTRL_PIO1_CORE1_BITS   _u(0x00000020)
#define ACCESSCTRL_PIO1_CORE1_MSB    _u(5)
#define ACCESSCTRL_PIO1_CORE1_LSB    _u(5)
#define ACCESSCTRL_PIO1_CORE1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PIO1_CORE0
// Description : If 1, PIO1 can be accessed by core 0, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_PIO1_CORE0_RESET  _u(0x1)
#define ACCESSCTRL_PIO1_CORE0_BITS   _u(0x00000010)
#define ACCESSCTRL_PIO1_CORE0_MSB    _u(4)
#define ACCESSCTRL_PIO1_CORE0_LSB    _u(4)
#define ACCESSCTRL_PIO1_CORE0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PIO1_SP
// Description : If 1, PIO1 can be accessed from a Secure, Privileged context.
#define ACCESSCTRL_PIO1_SP_RESET  _u(0x1)
#define ACCESSCTRL_PIO1_SP_BITS   _u(0x00000008)
#define ACCESSCTRL_PIO1_SP_MSB    _u(3)
#define ACCESSCTRL_PIO1_SP_LSB    _u(3)
#define ACCESSCTRL_PIO1_SP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PIO1_SU
// Description : If 1, and SP is also set, PIO1 can be accessed from a Secure,
//               Unprivileged context.
#define ACCESSCTRL_PIO1_SU_RESET  _u(0x1)
#define ACCESSCTRL_PIO1_SU_BITS   _u(0x00000004)
#define ACCESSCTRL_PIO1_SU_MSB    _u(2)
#define ACCESSCTRL_PIO1_SU_LSB    _u(2)
#define ACCESSCTRL_PIO1_SU_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PIO1_NSP
// Description : If 1, PIO1 can be accessed from a Non-secure, Privileged
//               context.
#define ACCESSCTRL_PIO1_NSP_RESET  _u(0x0)
#define ACCESSCTRL_PIO1_NSP_BITS   _u(0x00000002)
#define ACCESSCTRL_PIO1_NSP_MSB    _u(1)
#define ACCESSCTRL_PIO1_NSP_LSB    _u(1)
#define ACCESSCTRL_PIO1_NSP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PIO1_NSU
// Description : If 1, and NSP is also set, PIO1 can be accessed from a Non-
//               secure, Unprivileged context.
//
//               This bit is writable from a Non-secure, Privileged context, if
//               and only if the NSP bit is set.
#define ACCESSCTRL_PIO1_NSU_RESET  _u(0x0)
#define ACCESSCTRL_PIO1_NSU_BITS   _u(0x00000001)
#define ACCESSCTRL_PIO1_NSU_MSB    _u(0)
#define ACCESSCTRL_PIO1_NSU_LSB    _u(0)
#define ACCESSCTRL_PIO1_NSU_ACCESS "RW"
// =============================================================================
// Register    : ACCESSCTRL_PIO2
// Description : Control whether debugger, DMA, core 0 and core 1 can access
//               PIO2, and at what security/privilege levels they can do so.
//
//               Defaults to Secure access from any master.
//
//               This register is writable only from a Secure, Privileged
//               processor or debugger, with the exception of the NSU bit, which
//               becomes Non-secure-Privileged-writable when the NSP bit is set.
#define ACCESSCTRL_PIO2_OFFSET _u(0x00000054)
#define ACCESSCTRL_PIO2_BITS   _u(0x000000ff)
#define ACCESSCTRL_PIO2_RESET  _u(0x000000fc)
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PIO2_DBG
// Description : If 1, PIO2 can be accessed by the debugger, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_PIO2_DBG_RESET  _u(0x1)
#define ACCESSCTRL_PIO2_DBG_BITS   _u(0x00000080)
#define ACCESSCTRL_PIO2_DBG_MSB    _u(7)
#define ACCESSCTRL_PIO2_DBG_LSB    _u(7)
#define ACCESSCTRL_PIO2_DBG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PIO2_DMA
// Description : If 1, PIO2 can be accessed by the DMA, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_PIO2_DMA_RESET  _u(0x1)
#define ACCESSCTRL_PIO2_DMA_BITS   _u(0x00000040)
#define ACCESSCTRL_PIO2_DMA_MSB    _u(6)
#define ACCESSCTRL_PIO2_DMA_LSB    _u(6)
#define ACCESSCTRL_PIO2_DMA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PIO2_CORE1
// Description : If 1, PIO2 can be accessed by core 1, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_PIO2_CORE1_RESET  _u(0x1)
#define ACCESSCTRL_PIO2_CORE1_BITS   _u(0x00000020)
#define ACCESSCTRL_PIO2_CORE1_MSB    _u(5)
#define ACCESSCTRL_PIO2_CORE1_LSB    _u(5)
#define ACCESSCTRL_PIO2_CORE1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PIO2_CORE0
// Description : If 1, PIO2 can be accessed by core 0, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_PIO2_CORE0_RESET  _u(0x1)
#define ACCESSCTRL_PIO2_CORE0_BITS   _u(0x00000010)
#define ACCESSCTRL_PIO2_CORE0_MSB    _u(4)
#define ACCESSCTRL_PIO2_CORE0_LSB    _u(4)
#define ACCESSCTRL_PIO2_CORE0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PIO2_SP
// Description : If 1, PIO2 can be accessed from a Secure, Privileged context.
#define ACCESSCTRL_PIO2_SP_RESET  _u(0x1)
#define ACCESSCTRL_PIO2_SP_BITS   _u(0x00000008)
#define ACCESSCTRL_PIO2_SP_MSB    _u(3)
#define ACCESSCTRL_PIO2_SP_LSB    _u(3)
#define ACCESSCTRL_PIO2_SP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PIO2_SU
// Description : If 1, and SP is also set, PIO2 can be accessed from a Secure,
//               Unprivileged context.
#define ACCESSCTRL_PIO2_SU_RESET  _u(0x1)
#define ACCESSCTRL_PIO2_SU_BITS   _u(0x00000004)
#define ACCESSCTRL_PIO2_SU_MSB    _u(2)
#define ACCESSCTRL_PIO2_SU_LSB    _u(2)
#define ACCESSCTRL_PIO2_SU_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PIO2_NSP
// Description : If 1, PIO2 can be accessed from a Non-secure, Privileged
//               context.
#define ACCESSCTRL_PIO2_NSP_RESET  _u(0x0)
#define ACCESSCTRL_PIO2_NSP_BITS   _u(0x00000002)
#define ACCESSCTRL_PIO2_NSP_MSB    _u(1)
#define ACCESSCTRL_PIO2_NSP_LSB    _u(1)
#define ACCESSCTRL_PIO2_NSP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PIO2_NSU
// Description : If 1, and NSP is also set, PIO2 can be accessed from a Non-
//               secure, Unprivileged context.
//
//               This bit is writable from a Non-secure, Privileged context, if
//               and only if the NSP bit is set.
#define ACCESSCTRL_PIO2_NSU_RESET  _u(0x0)
#define ACCESSCTRL_PIO2_NSU_BITS   _u(0x00000001)
#define ACCESSCTRL_PIO2_NSU_MSB    _u(0)
#define ACCESSCTRL_PIO2_NSU_LSB    _u(0)
#define ACCESSCTRL_PIO2_NSU_ACCESS "RW"
// =============================================================================
// Register    : ACCESSCTRL_CORESIGHT_TRACE
// Description : Control whether debugger, DMA, core 0 and core 1 can access
//               CORESIGHT_TRACE, and at what security/privilege levels they can
//               do so.
//
//               Defaults to Secure, Privileged processor or debug access only.
//
//               This register is writable only from a Secure, Privileged
//               processor or debugger, with the exception of the NSU bit, which
//               becomes Non-secure-Privileged-writable when the NSP bit is set.
#define ACCESSCTRL_CORESIGHT_TRACE_OFFSET _u(0x00000058)
#define ACCESSCTRL_CORESIGHT_TRACE_BITS   _u(0x000000ff)
#define ACCESSCTRL_CORESIGHT_TRACE_RESET  _u(0x000000b8)
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_CORESIGHT_TRACE_DBG
// Description : If 1, CORESIGHT_TRACE can be accessed by the debugger, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_CORESIGHT_TRACE_DBG_RESET  _u(0x1)
#define ACCESSCTRL_CORESIGHT_TRACE_DBG_BITS   _u(0x00000080)
#define ACCESSCTRL_CORESIGHT_TRACE_DBG_MSB    _u(7)
#define ACCESSCTRL_CORESIGHT_TRACE_DBG_LSB    _u(7)
#define ACCESSCTRL_CORESIGHT_TRACE_DBG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_CORESIGHT_TRACE_DMA
// Description : If 1, CORESIGHT_TRACE can be accessed by the DMA, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_CORESIGHT_TRACE_DMA_RESET  _u(0x0)
#define ACCESSCTRL_CORESIGHT_TRACE_DMA_BITS   _u(0x00000040)
#define ACCESSCTRL_CORESIGHT_TRACE_DMA_MSB    _u(6)
#define ACCESSCTRL_CORESIGHT_TRACE_DMA_LSB    _u(6)
#define ACCESSCTRL_CORESIGHT_TRACE_DMA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_CORESIGHT_TRACE_CORE1
// Description : If 1, CORESIGHT_TRACE can be accessed by core 1, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_CORESIGHT_TRACE_CORE1_RESET  _u(0x1)
#define ACCESSCTRL_CORESIGHT_TRACE_CORE1_BITS   _u(0x00000020)
#define ACCESSCTRL_CORESIGHT_TRACE_CORE1_MSB    _u(5)
#define ACCESSCTRL_CORESIGHT_TRACE_CORE1_LSB    _u(5)
#define ACCESSCTRL_CORESIGHT_TRACE_CORE1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_CORESIGHT_TRACE_CORE0
// Description : If 1, CORESIGHT_TRACE can be accessed by core 0, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_CORESIGHT_TRACE_CORE0_RESET  _u(0x1)
#define ACCESSCTRL_CORESIGHT_TRACE_CORE0_BITS   _u(0x00000010)
#define ACCESSCTRL_CORESIGHT_TRACE_CORE0_MSB    _u(4)
#define ACCESSCTRL_CORESIGHT_TRACE_CORE0_LSB    _u(4)
#define ACCESSCTRL_CORESIGHT_TRACE_CORE0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_CORESIGHT_TRACE_SP
// Description : If 1, CORESIGHT_TRACE can be accessed from a Secure, Privileged
//               context.
#define ACCESSCTRL_CORESIGHT_TRACE_SP_RESET  _u(0x1)
#define ACCESSCTRL_CORESIGHT_TRACE_SP_BITS   _u(0x00000008)
#define ACCESSCTRL_CORESIGHT_TRACE_SP_MSB    _u(3)
#define ACCESSCTRL_CORESIGHT_TRACE_SP_LSB    _u(3)
#define ACCESSCTRL_CORESIGHT_TRACE_SP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_CORESIGHT_TRACE_SU
// Description : If 1, and SP is also set, CORESIGHT_TRACE can be accessed from
//               a Secure, Unprivileged context.
#define ACCESSCTRL_CORESIGHT_TRACE_SU_RESET  _u(0x0)
#define ACCESSCTRL_CORESIGHT_TRACE_SU_BITS   _u(0x00000004)
#define ACCESSCTRL_CORESIGHT_TRACE_SU_MSB    _u(2)
#define ACCESSCTRL_CORESIGHT_TRACE_SU_LSB    _u(2)
#define ACCESSCTRL_CORESIGHT_TRACE_SU_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_CORESIGHT_TRACE_NSP
// Description : If 1, CORESIGHT_TRACE can be accessed from a Non-secure,
//               Privileged context.
#define ACCESSCTRL_CORESIGHT_TRACE_NSP_RESET  _u(0x0)
#define ACCESSCTRL_CORESIGHT_TRACE_NSP_BITS   _u(0x00000002)
#define ACCESSCTRL_CORESIGHT_TRACE_NSP_MSB    _u(1)
#define ACCESSCTRL_CORESIGHT_TRACE_NSP_LSB    _u(1)
#define ACCESSCTRL_CORESIGHT_TRACE_NSP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_CORESIGHT_TRACE_NSU
// Description : If 1, and NSP is also set, CORESIGHT_TRACE can be accessed from
//               a Non-secure, Unprivileged context.
//
//               This bit is writable from a Non-secure, Privileged context, if
//               and only if the NSP bit is set.
#define ACCESSCTRL_CORESIGHT_TRACE_NSU_RESET  _u(0x0)
#define ACCESSCTRL_CORESIGHT_TRACE_NSU_BITS   _u(0x00000001)
#define ACCESSCTRL_CORESIGHT_TRACE_NSU_MSB    _u(0)
#define ACCESSCTRL_CORESIGHT_TRACE_NSU_LSB    _u(0)
#define ACCESSCTRL_CORESIGHT_TRACE_NSU_ACCESS "RW"
// =============================================================================
// Register    : ACCESSCTRL_CORESIGHT_PERIPH
// Description : Control whether debugger, DMA, core 0 and core 1 can access
//               CORESIGHT_PERIPH, and at what security/privilege levels they
//               can do so.
//
//               Defaults to Secure, Privileged processor or debug access only.
//
//               This register is writable only from a Secure, Privileged
//               processor or debugger, with the exception of the NSU bit, which
//               becomes Non-secure-Privileged-writable when the NSP bit is set.
#define ACCESSCTRL_CORESIGHT_PERIPH_OFFSET _u(0x0000005c)
#define ACCESSCTRL_CORESIGHT_PERIPH_BITS   _u(0x000000ff)
#define ACCESSCTRL_CORESIGHT_PERIPH_RESET  _u(0x000000b8)
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_CORESIGHT_PERIPH_DBG
// Description : If 1, CORESIGHT_PERIPH can be accessed by the debugger, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_CORESIGHT_PERIPH_DBG_RESET  _u(0x1)
#define ACCESSCTRL_CORESIGHT_PERIPH_DBG_BITS   _u(0x00000080)
#define ACCESSCTRL_CORESIGHT_PERIPH_DBG_MSB    _u(7)
#define ACCESSCTRL_CORESIGHT_PERIPH_DBG_LSB    _u(7)
#define ACCESSCTRL_CORESIGHT_PERIPH_DBG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_CORESIGHT_PERIPH_DMA
// Description : If 1, CORESIGHT_PERIPH can be accessed by the DMA, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_CORESIGHT_PERIPH_DMA_RESET  _u(0x0)
#define ACCESSCTRL_CORESIGHT_PERIPH_DMA_BITS   _u(0x00000040)
#define ACCESSCTRL_CORESIGHT_PERIPH_DMA_MSB    _u(6)
#define ACCESSCTRL_CORESIGHT_PERIPH_DMA_LSB    _u(6)
#define ACCESSCTRL_CORESIGHT_PERIPH_DMA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_CORESIGHT_PERIPH_CORE1
// Description : If 1, CORESIGHT_PERIPH can be accessed by core 1, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_CORESIGHT_PERIPH_CORE1_RESET  _u(0x1)
#define ACCESSCTRL_CORESIGHT_PERIPH_CORE1_BITS   _u(0x00000020)
#define ACCESSCTRL_CORESIGHT_PERIPH_CORE1_MSB    _u(5)
#define ACCESSCTRL_CORESIGHT_PERIPH_CORE1_LSB    _u(5)
#define ACCESSCTRL_CORESIGHT_PERIPH_CORE1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_CORESIGHT_PERIPH_CORE0
// Description : If 1, CORESIGHT_PERIPH can be accessed by core 0, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_CORESIGHT_PERIPH_CORE0_RESET  _u(0x1)
#define ACCESSCTRL_CORESIGHT_PERIPH_CORE0_BITS   _u(0x00000010)
#define ACCESSCTRL_CORESIGHT_PERIPH_CORE0_MSB    _u(4)
#define ACCESSCTRL_CORESIGHT_PERIPH_CORE0_LSB    _u(4)
#define ACCESSCTRL_CORESIGHT_PERIPH_CORE0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_CORESIGHT_PERIPH_SP
// Description : If 1, CORESIGHT_PERIPH can be accessed from a Secure,
//               Privileged context.
#define ACCESSCTRL_CORESIGHT_PERIPH_SP_RESET  _u(0x1)
#define ACCESSCTRL_CORESIGHT_PERIPH_SP_BITS   _u(0x00000008)
#define ACCESSCTRL_CORESIGHT_PERIPH_SP_MSB    _u(3)
#define ACCESSCTRL_CORESIGHT_PERIPH_SP_LSB    _u(3)
#define ACCESSCTRL_CORESIGHT_PERIPH_SP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_CORESIGHT_PERIPH_SU
// Description : If 1, and SP is also set, CORESIGHT_PERIPH can be accessed from
//               a Secure, Unprivileged context.
#define ACCESSCTRL_CORESIGHT_PERIPH_SU_RESET  _u(0x0)
#define ACCESSCTRL_CORESIGHT_PERIPH_SU_BITS   _u(0x00000004)
#define ACCESSCTRL_CORESIGHT_PERIPH_SU_MSB    _u(2)
#define ACCESSCTRL_CORESIGHT_PERIPH_SU_LSB    _u(2)
#define ACCESSCTRL_CORESIGHT_PERIPH_SU_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_CORESIGHT_PERIPH_NSP
// Description : If 1, CORESIGHT_PERIPH can be accessed from a Non-secure,
//               Privileged context.
#define ACCESSCTRL_CORESIGHT_PERIPH_NSP_RESET  _u(0x0)
#define ACCESSCTRL_CORESIGHT_PERIPH_NSP_BITS   _u(0x00000002)
#define ACCESSCTRL_CORESIGHT_PERIPH_NSP_MSB    _u(1)
#define ACCESSCTRL_CORESIGHT_PERIPH_NSP_LSB    _u(1)
#define ACCESSCTRL_CORESIGHT_PERIPH_NSP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_CORESIGHT_PERIPH_NSU
// Description : If 1, and NSP is also set, CORESIGHT_PERIPH can be accessed
//               from a Non-secure, Unprivileged context.
//
//               This bit is writable from a Non-secure, Privileged context, if
//               and only if the NSP bit is set.
#define ACCESSCTRL_CORESIGHT_PERIPH_NSU_RESET  _u(0x0)
#define ACCESSCTRL_CORESIGHT_PERIPH_NSU_BITS   _u(0x00000001)
#define ACCESSCTRL_CORESIGHT_PERIPH_NSU_MSB    _u(0)
#define ACCESSCTRL_CORESIGHT_PERIPH_NSU_LSB    _u(0)
#define ACCESSCTRL_CORESIGHT_PERIPH_NSU_ACCESS "RW"
// =============================================================================
// Register    : ACCESSCTRL_SYSINFO
// Description : Control whether debugger, DMA, core 0 and core 1 can access
//               SYSINFO, and at what security/privilege levels they can do so.
//
//               Defaults to fully open access.
//
//               This register is writable only from a Secure, Privileged
//               processor or debugger, with the exception of the NSU bit, which
//               becomes Non-secure-Privileged-writable when the NSP bit is set.
#define ACCESSCTRL_SYSINFO_OFFSET _u(0x00000060)
#define ACCESSCTRL_SYSINFO_BITS   _u(0x000000ff)
#define ACCESSCTRL_SYSINFO_RESET  _u(0x000000ff)
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SYSINFO_DBG
// Description : If 1, SYSINFO can be accessed by the debugger, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_SYSINFO_DBG_RESET  _u(0x1)
#define ACCESSCTRL_SYSINFO_DBG_BITS   _u(0x00000080)
#define ACCESSCTRL_SYSINFO_DBG_MSB    _u(7)
#define ACCESSCTRL_SYSINFO_DBG_LSB    _u(7)
#define ACCESSCTRL_SYSINFO_DBG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SYSINFO_DMA
// Description : If 1, SYSINFO can be accessed by the DMA, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_SYSINFO_DMA_RESET  _u(0x1)
#define ACCESSCTRL_SYSINFO_DMA_BITS   _u(0x00000040)
#define ACCESSCTRL_SYSINFO_DMA_MSB    _u(6)
#define ACCESSCTRL_SYSINFO_DMA_LSB    _u(6)
#define ACCESSCTRL_SYSINFO_DMA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SYSINFO_CORE1
// Description : If 1, SYSINFO can be accessed by core 1, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_SYSINFO_CORE1_RESET  _u(0x1)
#define ACCESSCTRL_SYSINFO_CORE1_BITS   _u(0x00000020)
#define ACCESSCTRL_SYSINFO_CORE1_MSB    _u(5)
#define ACCESSCTRL_SYSINFO_CORE1_LSB    _u(5)
#define ACCESSCTRL_SYSINFO_CORE1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SYSINFO_CORE0
// Description : If 1, SYSINFO can be accessed by core 0, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_SYSINFO_CORE0_RESET  _u(0x1)
#define ACCESSCTRL_SYSINFO_CORE0_BITS   _u(0x00000010)
#define ACCESSCTRL_SYSINFO_CORE0_MSB    _u(4)
#define ACCESSCTRL_SYSINFO_CORE0_LSB    _u(4)
#define ACCESSCTRL_SYSINFO_CORE0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SYSINFO_SP
// Description : If 1, SYSINFO can be accessed from a Secure, Privileged
//               context.
#define ACCESSCTRL_SYSINFO_SP_RESET  _u(0x1)
#define ACCESSCTRL_SYSINFO_SP_BITS   _u(0x00000008)
#define ACCESSCTRL_SYSINFO_SP_MSB    _u(3)
#define ACCESSCTRL_SYSINFO_SP_LSB    _u(3)
#define ACCESSCTRL_SYSINFO_SP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SYSINFO_SU
// Description : If 1, and SP is also set, SYSINFO can be accessed from a
//               Secure, Unprivileged context.
#define ACCESSCTRL_SYSINFO_SU_RESET  _u(0x1)
#define ACCESSCTRL_SYSINFO_SU_BITS   _u(0x00000004)
#define ACCESSCTRL_SYSINFO_SU_MSB    _u(2)
#define ACCESSCTRL_SYSINFO_SU_LSB    _u(2)
#define ACCESSCTRL_SYSINFO_SU_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SYSINFO_NSP
// Description : If 1, SYSINFO can be accessed from a Non-secure, Privileged
//               context.
#define ACCESSCTRL_SYSINFO_NSP_RESET  _u(0x1)
#define ACCESSCTRL_SYSINFO_NSP_BITS   _u(0x00000002)
#define ACCESSCTRL_SYSINFO_NSP_MSB    _u(1)
#define ACCESSCTRL_SYSINFO_NSP_LSB    _u(1)
#define ACCESSCTRL_SYSINFO_NSP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SYSINFO_NSU
// Description : If 1, and NSP is also set, SYSINFO can be accessed from a Non-
//               secure, Unprivileged context.
//
//               This bit is writable from a Non-secure, Privileged context, if
//               and only if the NSP bit is set.
#define ACCESSCTRL_SYSINFO_NSU_RESET  _u(0x1)
#define ACCESSCTRL_SYSINFO_NSU_BITS   _u(0x00000001)
#define ACCESSCTRL_SYSINFO_NSU_MSB    _u(0)
#define ACCESSCTRL_SYSINFO_NSU_LSB    _u(0)
#define ACCESSCTRL_SYSINFO_NSU_ACCESS "RW"
// =============================================================================
// Register    : ACCESSCTRL_RESETS
// Description : Control whether debugger, DMA, core 0 and core 1 can access
//               RESETS, and at what security/privilege levels they can do so.
//
//               Defaults to Secure access from any master.
//
//               This register is writable only from a Secure, Privileged
//               processor or debugger, with the exception of the NSU bit, which
//               becomes Non-secure-Privileged-writable when the NSP bit is set.
#define ACCESSCTRL_RESETS_OFFSET _u(0x00000064)
#define ACCESSCTRL_RESETS_BITS   _u(0x000000ff)
#define ACCESSCTRL_RESETS_RESET  _u(0x000000fc)
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_RESETS_DBG
// Description : If 1, RESETS can be accessed by the debugger, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_RESETS_DBG_RESET  _u(0x1)
#define ACCESSCTRL_RESETS_DBG_BITS   _u(0x00000080)
#define ACCESSCTRL_RESETS_DBG_MSB    _u(7)
#define ACCESSCTRL_RESETS_DBG_LSB    _u(7)
#define ACCESSCTRL_RESETS_DBG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_RESETS_DMA
// Description : If 1, RESETS can be accessed by the DMA, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_RESETS_DMA_RESET  _u(0x1)
#define ACCESSCTRL_RESETS_DMA_BITS   _u(0x00000040)
#define ACCESSCTRL_RESETS_DMA_MSB    _u(6)
#define ACCESSCTRL_RESETS_DMA_LSB    _u(6)
#define ACCESSCTRL_RESETS_DMA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_RESETS_CORE1
// Description : If 1, RESETS can be accessed by core 1, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_RESETS_CORE1_RESET  _u(0x1)
#define ACCESSCTRL_RESETS_CORE1_BITS   _u(0x00000020)
#define ACCESSCTRL_RESETS_CORE1_MSB    _u(5)
#define ACCESSCTRL_RESETS_CORE1_LSB    _u(5)
#define ACCESSCTRL_RESETS_CORE1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_RESETS_CORE0
// Description : If 1, RESETS can be accessed by core 0, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_RESETS_CORE0_RESET  _u(0x1)
#define ACCESSCTRL_RESETS_CORE0_BITS   _u(0x00000010)
#define ACCESSCTRL_RESETS_CORE0_MSB    _u(4)
#define ACCESSCTRL_RESETS_CORE0_LSB    _u(4)
#define ACCESSCTRL_RESETS_CORE0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_RESETS_SP
// Description : If 1, RESETS can be accessed from a Secure, Privileged context.
#define ACCESSCTRL_RESETS_SP_RESET  _u(0x1)
#define ACCESSCTRL_RESETS_SP_BITS   _u(0x00000008)
#define ACCESSCTRL_RESETS_SP_MSB    _u(3)
#define ACCESSCTRL_RESETS_SP_LSB    _u(3)
#define ACCESSCTRL_RESETS_SP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_RESETS_SU
// Description : If 1, and SP is also set, RESETS can be accessed from a Secure,
//               Unprivileged context.
#define ACCESSCTRL_RESETS_SU_RESET  _u(0x1)
#define ACCESSCTRL_RESETS_SU_BITS   _u(0x00000004)
#define ACCESSCTRL_RESETS_SU_MSB    _u(2)
#define ACCESSCTRL_RESETS_SU_LSB    _u(2)
#define ACCESSCTRL_RESETS_SU_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_RESETS_NSP
// Description : If 1, RESETS can be accessed from a Non-secure, Privileged
//               context.
#define ACCESSCTRL_RESETS_NSP_RESET  _u(0x0)
#define ACCESSCTRL_RESETS_NSP_BITS   _u(0x00000002)
#define ACCESSCTRL_RESETS_NSP_MSB    _u(1)
#define ACCESSCTRL_RESETS_NSP_LSB    _u(1)
#define ACCESSCTRL_RESETS_NSP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_RESETS_NSU
// Description : If 1, and NSP is also set, RESETS can be accessed from a Non-
//               secure, Unprivileged context.
//
//               This bit is writable from a Non-secure, Privileged context, if
//               and only if the NSP bit is set.
#define ACCESSCTRL_RESETS_NSU_RESET  _u(0x0)
#define ACCESSCTRL_RESETS_NSU_BITS   _u(0x00000001)
#define ACCESSCTRL_RESETS_NSU_MSB    _u(0)
#define ACCESSCTRL_RESETS_NSU_LSB    _u(0)
#define ACCESSCTRL_RESETS_NSU_ACCESS "RW"
// =============================================================================
// Register    : ACCESSCTRL_IO_BANK0
// Description : Control whether debugger, DMA, core 0 and core 1 can access
//               IO_BANK0, and at what security/privilege levels they can do so.
//
//               Defaults to Secure access from any master.
//
//               This register is writable only from a Secure, Privileged
//               processor or debugger, with the exception of the NSU bit, which
//               becomes Non-secure-Privileged-writable when the NSP bit is set.
#define ACCESSCTRL_IO_BANK0_OFFSET _u(0x00000068)
#define ACCESSCTRL_IO_BANK0_BITS   _u(0x000000ff)
#define ACCESSCTRL_IO_BANK0_RESET  _u(0x000000fc)
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_IO_BANK0_DBG
// Description : If 1, IO_BANK0 can be accessed by the debugger, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_IO_BANK0_DBG_RESET  _u(0x1)
#define ACCESSCTRL_IO_BANK0_DBG_BITS   _u(0x00000080)
#define ACCESSCTRL_IO_BANK0_DBG_MSB    _u(7)
#define ACCESSCTRL_IO_BANK0_DBG_LSB    _u(7)
#define ACCESSCTRL_IO_BANK0_DBG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_IO_BANK0_DMA
// Description : If 1, IO_BANK0 can be accessed by the DMA, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_IO_BANK0_DMA_RESET  _u(0x1)
#define ACCESSCTRL_IO_BANK0_DMA_BITS   _u(0x00000040)
#define ACCESSCTRL_IO_BANK0_DMA_MSB    _u(6)
#define ACCESSCTRL_IO_BANK0_DMA_LSB    _u(6)
#define ACCESSCTRL_IO_BANK0_DMA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_IO_BANK0_CORE1
// Description : If 1, IO_BANK0 can be accessed by core 1, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_IO_BANK0_CORE1_RESET  _u(0x1)
#define ACCESSCTRL_IO_BANK0_CORE1_BITS   _u(0x00000020)
#define ACCESSCTRL_IO_BANK0_CORE1_MSB    _u(5)
#define ACCESSCTRL_IO_BANK0_CORE1_LSB    _u(5)
#define ACCESSCTRL_IO_BANK0_CORE1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_IO_BANK0_CORE0
// Description : If 1, IO_BANK0 can be accessed by core 0, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_IO_BANK0_CORE0_RESET  _u(0x1)
#define ACCESSCTRL_IO_BANK0_CORE0_BITS   _u(0x00000010)
#define ACCESSCTRL_IO_BANK0_CORE0_MSB    _u(4)
#define ACCESSCTRL_IO_BANK0_CORE0_LSB    _u(4)
#define ACCESSCTRL_IO_BANK0_CORE0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_IO_BANK0_SP
// Description : If 1, IO_BANK0 can be accessed from a Secure, Privileged
//               context.
#define ACCESSCTRL_IO_BANK0_SP_RESET  _u(0x1)
#define ACCESSCTRL_IO_BANK0_SP_BITS   _u(0x00000008)
#define ACCESSCTRL_IO_BANK0_SP_MSB    _u(3)
#define ACCESSCTRL_IO_BANK0_SP_LSB    _u(3)
#define ACCESSCTRL_IO_BANK0_SP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_IO_BANK0_SU
// Description : If 1, and SP is also set, IO_BANK0 can be accessed from a
//               Secure, Unprivileged context.
#define ACCESSCTRL_IO_BANK0_SU_RESET  _u(0x1)
#define ACCESSCTRL_IO_BANK0_SU_BITS   _u(0x00000004)
#define ACCESSCTRL_IO_BANK0_SU_MSB    _u(2)
#define ACCESSCTRL_IO_BANK0_SU_LSB    _u(2)
#define ACCESSCTRL_IO_BANK0_SU_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_IO_BANK0_NSP
// Description : If 1, IO_BANK0 can be accessed from a Non-secure, Privileged
//               context.
#define ACCESSCTRL_IO_BANK0_NSP_RESET  _u(0x0)
#define ACCESSCTRL_IO_BANK0_NSP_BITS   _u(0x00000002)
#define ACCESSCTRL_IO_BANK0_NSP_MSB    _u(1)
#define ACCESSCTRL_IO_BANK0_NSP_LSB    _u(1)
#define ACCESSCTRL_IO_BANK0_NSP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_IO_BANK0_NSU
// Description : If 1, and NSP is also set, IO_BANK0 can be accessed from a Non-
//               secure, Unprivileged context.
//
//               This bit is writable from a Non-secure, Privileged context, if
//               and only if the NSP bit is set.
#define ACCESSCTRL_IO_BANK0_NSU_RESET  _u(0x0)
#define ACCESSCTRL_IO_BANK0_NSU_BITS   _u(0x00000001)
#define ACCESSCTRL_IO_BANK0_NSU_MSB    _u(0)
#define ACCESSCTRL_IO_BANK0_NSU_LSB    _u(0)
#define ACCESSCTRL_IO_BANK0_NSU_ACCESS "RW"
// =============================================================================
// Register    : ACCESSCTRL_IO_BANK1
// Description : Control whether debugger, DMA, core 0 and core 1 can access
//               IO_BANK1, and at what security/privilege levels they can do so.
//
//               Defaults to Secure access from any master.
//
//               This register is writable only from a Secure, Privileged
//               processor or debugger, with the exception of the NSU bit, which
//               becomes Non-secure-Privileged-writable when the NSP bit is set.
#define ACCESSCTRL_IO_BANK1_OFFSET _u(0x0000006c)
#define ACCESSCTRL_IO_BANK1_BITS   _u(0x000000ff)
#define ACCESSCTRL_IO_BANK1_RESET  _u(0x000000fc)
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_IO_BANK1_DBG
// Description : If 1, IO_BANK1 can be accessed by the debugger, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_IO_BANK1_DBG_RESET  _u(0x1)
#define ACCESSCTRL_IO_BANK1_DBG_BITS   _u(0x00000080)
#define ACCESSCTRL_IO_BANK1_DBG_MSB    _u(7)
#define ACCESSCTRL_IO_BANK1_DBG_LSB    _u(7)
#define ACCESSCTRL_IO_BANK1_DBG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_IO_BANK1_DMA
// Description : If 1, IO_BANK1 can be accessed by the DMA, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_IO_BANK1_DMA_RESET  _u(0x1)
#define ACCESSCTRL_IO_BANK1_DMA_BITS   _u(0x00000040)
#define ACCESSCTRL_IO_BANK1_DMA_MSB    _u(6)
#define ACCESSCTRL_IO_BANK1_DMA_LSB    _u(6)
#define ACCESSCTRL_IO_BANK1_DMA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_IO_BANK1_CORE1
// Description : If 1, IO_BANK1 can be accessed by core 1, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_IO_BANK1_CORE1_RESET  _u(0x1)
#define ACCESSCTRL_IO_BANK1_CORE1_BITS   _u(0x00000020)
#define ACCESSCTRL_IO_BANK1_CORE1_MSB    _u(5)
#define ACCESSCTRL_IO_BANK1_CORE1_LSB    _u(5)
#define ACCESSCTRL_IO_BANK1_CORE1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_IO_BANK1_CORE0
// Description : If 1, IO_BANK1 can be accessed by core 0, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_IO_BANK1_CORE0_RESET  _u(0x1)
#define ACCESSCTRL_IO_BANK1_CORE0_BITS   _u(0x00000010)
#define ACCESSCTRL_IO_BANK1_CORE0_MSB    _u(4)
#define ACCESSCTRL_IO_BANK1_CORE0_LSB    _u(4)
#define ACCESSCTRL_IO_BANK1_CORE0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_IO_BANK1_SP
// Description : If 1, IO_BANK1 can be accessed from a Secure, Privileged
//               context.
#define ACCESSCTRL_IO_BANK1_SP_RESET  _u(0x1)
#define ACCESSCTRL_IO_BANK1_SP_BITS   _u(0x00000008)
#define ACCESSCTRL_IO_BANK1_SP_MSB    _u(3)
#define ACCESSCTRL_IO_BANK1_SP_LSB    _u(3)
#define ACCESSCTRL_IO_BANK1_SP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_IO_BANK1_SU
// Description : If 1, and SP is also set, IO_BANK1 can be accessed from a
//               Secure, Unprivileged context.
#define ACCESSCTRL_IO_BANK1_SU_RESET  _u(0x1)
#define ACCESSCTRL_IO_BANK1_SU_BITS   _u(0x00000004)
#define ACCESSCTRL_IO_BANK1_SU_MSB    _u(2)
#define ACCESSCTRL_IO_BANK1_SU_LSB    _u(2)
#define ACCESSCTRL_IO_BANK1_SU_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_IO_BANK1_NSP
// Description : If 1, IO_BANK1 can be accessed from a Non-secure, Privileged
//               context.
#define ACCESSCTRL_IO_BANK1_NSP_RESET  _u(0x0)
#define ACCESSCTRL_IO_BANK1_NSP_BITS   _u(0x00000002)
#define ACCESSCTRL_IO_BANK1_NSP_MSB    _u(1)
#define ACCESSCTRL_IO_BANK1_NSP_LSB    _u(1)
#define ACCESSCTRL_IO_BANK1_NSP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_IO_BANK1_NSU
// Description : If 1, and NSP is also set, IO_BANK1 can be accessed from a Non-
//               secure, Unprivileged context.
//
//               This bit is writable from a Non-secure, Privileged context, if
//               and only if the NSP bit is set.
#define ACCESSCTRL_IO_BANK1_NSU_RESET  _u(0x0)
#define ACCESSCTRL_IO_BANK1_NSU_BITS   _u(0x00000001)
#define ACCESSCTRL_IO_BANK1_NSU_MSB    _u(0)
#define ACCESSCTRL_IO_BANK1_NSU_LSB    _u(0)
#define ACCESSCTRL_IO_BANK1_NSU_ACCESS "RW"
// =============================================================================
// Register    : ACCESSCTRL_PADS_BANK0
// Description : Control whether debugger, DMA, core 0 and core 1 can access
//               PADS_BANK0, and at what security/privilege levels they can do
//               so.
//
//               Defaults to Secure access from any master.
//
//               This register is writable only from a Secure, Privileged
//               processor or debugger, with the exception of the NSU bit, which
//               becomes Non-secure-Privileged-writable when the NSP bit is set.
#define ACCESSCTRL_PADS_BANK0_OFFSET _u(0x00000070)
#define ACCESSCTRL_PADS_BANK0_BITS   _u(0x000000ff)
#define ACCESSCTRL_PADS_BANK0_RESET  _u(0x000000fc)
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PADS_BANK0_DBG
// Description : If 1, PADS_BANK0 can be accessed by the debugger, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_PADS_BANK0_DBG_RESET  _u(0x1)
#define ACCESSCTRL_PADS_BANK0_DBG_BITS   _u(0x00000080)
#define ACCESSCTRL_PADS_BANK0_DBG_MSB    _u(7)
#define ACCESSCTRL_PADS_BANK0_DBG_LSB    _u(7)
#define ACCESSCTRL_PADS_BANK0_DBG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PADS_BANK0_DMA
// Description : If 1, PADS_BANK0 can be accessed by the DMA, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_PADS_BANK0_DMA_RESET  _u(0x1)
#define ACCESSCTRL_PADS_BANK0_DMA_BITS   _u(0x00000040)
#define ACCESSCTRL_PADS_BANK0_DMA_MSB    _u(6)
#define ACCESSCTRL_PADS_BANK0_DMA_LSB    _u(6)
#define ACCESSCTRL_PADS_BANK0_DMA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PADS_BANK0_CORE1
// Description : If 1, PADS_BANK0 can be accessed by core 1, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_PADS_BANK0_CORE1_RESET  _u(0x1)
#define ACCESSCTRL_PADS_BANK0_CORE1_BITS   _u(0x00000020)
#define ACCESSCTRL_PADS_BANK0_CORE1_MSB    _u(5)
#define ACCESSCTRL_PADS_BANK0_CORE1_LSB    _u(5)
#define ACCESSCTRL_PADS_BANK0_CORE1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PADS_BANK0_CORE0
// Description : If 1, PADS_BANK0 can be accessed by core 0, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_PADS_BANK0_CORE0_RESET  _u(0x1)
#define ACCESSCTRL_PADS_BANK0_CORE0_BITS   _u(0x00000010)
#define ACCESSCTRL_PADS_BANK0_CORE0_MSB    _u(4)
#define ACCESSCTRL_PADS_BANK0_CORE0_LSB    _u(4)
#define ACCESSCTRL_PADS_BANK0_CORE0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PADS_BANK0_SP
// Description : If 1, PADS_BANK0 can be accessed from a Secure, Privileged
//               context.
#define ACCESSCTRL_PADS_BANK0_SP_RESET  _u(0x1)
#define ACCESSCTRL_PADS_BANK0_SP_BITS   _u(0x00000008)
#define ACCESSCTRL_PADS_BANK0_SP_MSB    _u(3)
#define ACCESSCTRL_PADS_BANK0_SP_LSB    _u(3)
#define ACCESSCTRL_PADS_BANK0_SP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PADS_BANK0_SU
// Description : If 1, and SP is also set, PADS_BANK0 can be accessed from a
//               Secure, Unprivileged context.
#define ACCESSCTRL_PADS_BANK0_SU_RESET  _u(0x1)
#define ACCESSCTRL_PADS_BANK0_SU_BITS   _u(0x00000004)
#define ACCESSCTRL_PADS_BANK0_SU_MSB    _u(2)
#define ACCESSCTRL_PADS_BANK0_SU_LSB    _u(2)
#define ACCESSCTRL_PADS_BANK0_SU_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PADS_BANK0_NSP
// Description : If 1, PADS_BANK0 can be accessed from a Non-secure, Privileged
//               context.
#define ACCESSCTRL_PADS_BANK0_NSP_RESET  _u(0x0)
#define ACCESSCTRL_PADS_BANK0_NSP_BITS   _u(0x00000002)
#define ACCESSCTRL_PADS_BANK0_NSP_MSB    _u(1)
#define ACCESSCTRL_PADS_BANK0_NSP_LSB    _u(1)
#define ACCESSCTRL_PADS_BANK0_NSP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PADS_BANK0_NSU
// Description : If 1, and NSP is also set, PADS_BANK0 can be accessed from a
//               Non-secure, Unprivileged context.
//
//               This bit is writable from a Non-secure, Privileged context, if
//               and only if the NSP bit is set.
#define ACCESSCTRL_PADS_BANK0_NSU_RESET  _u(0x0)
#define ACCESSCTRL_PADS_BANK0_NSU_BITS   _u(0x00000001)
#define ACCESSCTRL_PADS_BANK0_NSU_MSB    _u(0)
#define ACCESSCTRL_PADS_BANK0_NSU_LSB    _u(0)
#define ACCESSCTRL_PADS_BANK0_NSU_ACCESS "RW"
// =============================================================================
// Register    : ACCESSCTRL_PADS_QSPI
// Description : Control whether debugger, DMA, core 0 and core 1 can access
//               PADS_QSPI, and at what security/privilege levels they can do
//               so.
//
//               Defaults to Secure access from any master.
//
//               This register is writable only from a Secure, Privileged
//               processor or debugger, with the exception of the NSU bit, which
//               becomes Non-secure-Privileged-writable when the NSP bit is set.
#define ACCESSCTRL_PADS_QSPI_OFFSET _u(0x00000074)
#define ACCESSCTRL_PADS_QSPI_BITS   _u(0x000000ff)
#define ACCESSCTRL_PADS_QSPI_RESET  _u(0x000000fc)
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PADS_QSPI_DBG
// Description : If 1, PADS_QSPI can be accessed by the debugger, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_PADS_QSPI_DBG_RESET  _u(0x1)
#define ACCESSCTRL_PADS_QSPI_DBG_BITS   _u(0x00000080)
#define ACCESSCTRL_PADS_QSPI_DBG_MSB    _u(7)
#define ACCESSCTRL_PADS_QSPI_DBG_LSB    _u(7)
#define ACCESSCTRL_PADS_QSPI_DBG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PADS_QSPI_DMA
// Description : If 1, PADS_QSPI can be accessed by the DMA, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_PADS_QSPI_DMA_RESET  _u(0x1)
#define ACCESSCTRL_PADS_QSPI_DMA_BITS   _u(0x00000040)
#define ACCESSCTRL_PADS_QSPI_DMA_MSB    _u(6)
#define ACCESSCTRL_PADS_QSPI_DMA_LSB    _u(6)
#define ACCESSCTRL_PADS_QSPI_DMA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PADS_QSPI_CORE1
// Description : If 1, PADS_QSPI can be accessed by core 1, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_PADS_QSPI_CORE1_RESET  _u(0x1)
#define ACCESSCTRL_PADS_QSPI_CORE1_BITS   _u(0x00000020)
#define ACCESSCTRL_PADS_QSPI_CORE1_MSB    _u(5)
#define ACCESSCTRL_PADS_QSPI_CORE1_LSB    _u(5)
#define ACCESSCTRL_PADS_QSPI_CORE1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PADS_QSPI_CORE0
// Description : If 1, PADS_QSPI can be accessed by core 0, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_PADS_QSPI_CORE0_RESET  _u(0x1)
#define ACCESSCTRL_PADS_QSPI_CORE0_BITS   _u(0x00000010)
#define ACCESSCTRL_PADS_QSPI_CORE0_MSB    _u(4)
#define ACCESSCTRL_PADS_QSPI_CORE0_LSB    _u(4)
#define ACCESSCTRL_PADS_QSPI_CORE0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PADS_QSPI_SP
// Description : If 1, PADS_QSPI can be accessed from a Secure, Privileged
//               context.
#define ACCESSCTRL_PADS_QSPI_SP_RESET  _u(0x1)
#define ACCESSCTRL_PADS_QSPI_SP_BITS   _u(0x00000008)
#define ACCESSCTRL_PADS_QSPI_SP_MSB    _u(3)
#define ACCESSCTRL_PADS_QSPI_SP_LSB    _u(3)
#define ACCESSCTRL_PADS_QSPI_SP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PADS_QSPI_SU
// Description : If 1, and SP is also set, PADS_QSPI can be accessed from a
//               Secure, Unprivileged context.
#define ACCESSCTRL_PADS_QSPI_SU_RESET  _u(0x1)
#define ACCESSCTRL_PADS_QSPI_SU_BITS   _u(0x00000004)
#define ACCESSCTRL_PADS_QSPI_SU_MSB    _u(2)
#define ACCESSCTRL_PADS_QSPI_SU_LSB    _u(2)
#define ACCESSCTRL_PADS_QSPI_SU_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PADS_QSPI_NSP
// Description : If 1, PADS_QSPI can be accessed from a Non-secure, Privileged
//               context.
#define ACCESSCTRL_PADS_QSPI_NSP_RESET  _u(0x0)
#define ACCESSCTRL_PADS_QSPI_NSP_BITS   _u(0x00000002)
#define ACCESSCTRL_PADS_QSPI_NSP_MSB    _u(1)
#define ACCESSCTRL_PADS_QSPI_NSP_LSB    _u(1)
#define ACCESSCTRL_PADS_QSPI_NSP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PADS_QSPI_NSU
// Description : If 1, and NSP is also set, PADS_QSPI can be accessed from a
//               Non-secure, Unprivileged context.
//
//               This bit is writable from a Non-secure, Privileged context, if
//               and only if the NSP bit is set.
#define ACCESSCTRL_PADS_QSPI_NSU_RESET  _u(0x0)
#define ACCESSCTRL_PADS_QSPI_NSU_BITS   _u(0x00000001)
#define ACCESSCTRL_PADS_QSPI_NSU_MSB    _u(0)
#define ACCESSCTRL_PADS_QSPI_NSU_LSB    _u(0)
#define ACCESSCTRL_PADS_QSPI_NSU_ACCESS "RW"
// =============================================================================
// Register    : ACCESSCTRL_BUSCTRL
// Description : Control whether debugger, DMA, core 0 and core 1 can access
//               BUSCTRL, and at what security/privilege levels they can do so.
//
//               Defaults to Secure access from any master.
//
//               This register is writable only from a Secure, Privileged
//               processor or debugger, with the exception of the NSU bit, which
//               becomes Non-secure-Privileged-writable when the NSP bit is set.
#define ACCESSCTRL_BUSCTRL_OFFSET _u(0x00000078)
#define ACCESSCTRL_BUSCTRL_BITS   _u(0x000000ff)
#define ACCESSCTRL_BUSCTRL_RESET  _u(0x000000fc)
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_BUSCTRL_DBG
// Description : If 1, BUSCTRL can be accessed by the debugger, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_BUSCTRL_DBG_RESET  _u(0x1)
#define ACCESSCTRL_BUSCTRL_DBG_BITS   _u(0x00000080)
#define ACCESSCTRL_BUSCTRL_DBG_MSB    _u(7)
#define ACCESSCTRL_BUSCTRL_DBG_LSB    _u(7)
#define ACCESSCTRL_BUSCTRL_DBG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_BUSCTRL_DMA
// Description : If 1, BUSCTRL can be accessed by the DMA, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_BUSCTRL_DMA_RESET  _u(0x1)
#define ACCESSCTRL_BUSCTRL_DMA_BITS   _u(0x00000040)
#define ACCESSCTRL_BUSCTRL_DMA_MSB    _u(6)
#define ACCESSCTRL_BUSCTRL_DMA_LSB    _u(6)
#define ACCESSCTRL_BUSCTRL_DMA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_BUSCTRL_CORE1
// Description : If 1, BUSCTRL can be accessed by core 1, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_BUSCTRL_CORE1_RESET  _u(0x1)
#define ACCESSCTRL_BUSCTRL_CORE1_BITS   _u(0x00000020)
#define ACCESSCTRL_BUSCTRL_CORE1_MSB    _u(5)
#define ACCESSCTRL_BUSCTRL_CORE1_LSB    _u(5)
#define ACCESSCTRL_BUSCTRL_CORE1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_BUSCTRL_CORE0
// Description : If 1, BUSCTRL can be accessed by core 0, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_BUSCTRL_CORE0_RESET  _u(0x1)
#define ACCESSCTRL_BUSCTRL_CORE0_BITS   _u(0x00000010)
#define ACCESSCTRL_BUSCTRL_CORE0_MSB    _u(4)
#define ACCESSCTRL_BUSCTRL_CORE0_LSB    _u(4)
#define ACCESSCTRL_BUSCTRL_CORE0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_BUSCTRL_SP
// Description : If 1, BUSCTRL can be accessed from a Secure, Privileged
//               context.
#define ACCESSCTRL_BUSCTRL_SP_RESET  _u(0x1)
#define ACCESSCTRL_BUSCTRL_SP_BITS   _u(0x00000008)
#define ACCESSCTRL_BUSCTRL_SP_MSB    _u(3)
#define ACCESSCTRL_BUSCTRL_SP_LSB    _u(3)
#define ACCESSCTRL_BUSCTRL_SP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_BUSCTRL_SU
// Description : If 1, and SP is also set, BUSCTRL can be accessed from a
//               Secure, Unprivileged context.
#define ACCESSCTRL_BUSCTRL_SU_RESET  _u(0x1)
#define ACCESSCTRL_BUSCTRL_SU_BITS   _u(0x00000004)
#define ACCESSCTRL_BUSCTRL_SU_MSB    _u(2)
#define ACCESSCTRL_BUSCTRL_SU_LSB    _u(2)
#define ACCESSCTRL_BUSCTRL_SU_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_BUSCTRL_NSP
// Description : If 1, BUSCTRL can be accessed from a Non-secure, Privileged
//               context.
#define ACCESSCTRL_BUSCTRL_NSP_RESET  _u(0x0)
#define ACCESSCTRL_BUSCTRL_NSP_BITS   _u(0x00000002)
#define ACCESSCTRL_BUSCTRL_NSP_MSB    _u(1)
#define ACCESSCTRL_BUSCTRL_NSP_LSB    _u(1)
#define ACCESSCTRL_BUSCTRL_NSP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_BUSCTRL_NSU
// Description : If 1, and NSP is also set, BUSCTRL can be accessed from a Non-
//               secure, Unprivileged context.
//
//               This bit is writable from a Non-secure, Privileged context, if
//               and only if the NSP bit is set.
#define ACCESSCTRL_BUSCTRL_NSU_RESET  _u(0x0)
#define ACCESSCTRL_BUSCTRL_NSU_BITS   _u(0x00000001)
#define ACCESSCTRL_BUSCTRL_NSU_MSB    _u(0)
#define ACCESSCTRL_BUSCTRL_NSU_LSB    _u(0)
#define ACCESSCTRL_BUSCTRL_NSU_ACCESS "RW"
// =============================================================================
// Register    : ACCESSCTRL_ADC0
// Description : Control whether debugger, DMA, core 0 and core 1 can access
//               ADC0, and at what security/privilege levels they can do so.
//
//               Defaults to Secure access from any master.
//
//               This register is writable only from a Secure, Privileged
//               processor or debugger, with the exception of the NSU bit, which
//               becomes Non-secure-Privileged-writable when the NSP bit is set.
#define ACCESSCTRL_ADC0_OFFSET _u(0x0000007c)
#define ACCESSCTRL_ADC0_BITS   _u(0x000000ff)
#define ACCESSCTRL_ADC0_RESET  _u(0x000000fc)
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_ADC0_DBG
// Description : If 1, ADC0 can be accessed by the debugger, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_ADC0_DBG_RESET  _u(0x1)
#define ACCESSCTRL_ADC0_DBG_BITS   _u(0x00000080)
#define ACCESSCTRL_ADC0_DBG_MSB    _u(7)
#define ACCESSCTRL_ADC0_DBG_LSB    _u(7)
#define ACCESSCTRL_ADC0_DBG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_ADC0_DMA
// Description : If 1, ADC0 can be accessed by the DMA, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_ADC0_DMA_RESET  _u(0x1)
#define ACCESSCTRL_ADC0_DMA_BITS   _u(0x00000040)
#define ACCESSCTRL_ADC0_DMA_MSB    _u(6)
#define ACCESSCTRL_ADC0_DMA_LSB    _u(6)
#define ACCESSCTRL_ADC0_DMA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_ADC0_CORE1
// Description : If 1, ADC0 can be accessed by core 1, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_ADC0_CORE1_RESET  _u(0x1)
#define ACCESSCTRL_ADC0_CORE1_BITS   _u(0x00000020)
#define ACCESSCTRL_ADC0_CORE1_MSB    _u(5)
#define ACCESSCTRL_ADC0_CORE1_LSB    _u(5)
#define ACCESSCTRL_ADC0_CORE1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_ADC0_CORE0
// Description : If 1, ADC0 can be accessed by core 0, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_ADC0_CORE0_RESET  _u(0x1)
#define ACCESSCTRL_ADC0_CORE0_BITS   _u(0x00000010)
#define ACCESSCTRL_ADC0_CORE0_MSB    _u(4)
#define ACCESSCTRL_ADC0_CORE0_LSB    _u(4)
#define ACCESSCTRL_ADC0_CORE0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_ADC0_SP
// Description : If 1, ADC0 can be accessed from a Secure, Privileged context.
#define ACCESSCTRL_ADC0_SP_RESET  _u(0x1)
#define ACCESSCTRL_ADC0_SP_BITS   _u(0x00000008)
#define ACCESSCTRL_ADC0_SP_MSB    _u(3)
#define ACCESSCTRL_ADC0_SP_LSB    _u(3)
#define ACCESSCTRL_ADC0_SP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_ADC0_SU
// Description : If 1, and SP is also set, ADC0 can be accessed from a Secure,
//               Unprivileged context.
#define ACCESSCTRL_ADC0_SU_RESET  _u(0x1)
#define ACCESSCTRL_ADC0_SU_BITS   _u(0x00000004)
#define ACCESSCTRL_ADC0_SU_MSB    _u(2)
#define ACCESSCTRL_ADC0_SU_LSB    _u(2)
#define ACCESSCTRL_ADC0_SU_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_ADC0_NSP
// Description : If 1, ADC0 can be accessed from a Non-secure, Privileged
//               context.
#define ACCESSCTRL_ADC0_NSP_RESET  _u(0x0)
#define ACCESSCTRL_ADC0_NSP_BITS   _u(0x00000002)
#define ACCESSCTRL_ADC0_NSP_MSB    _u(1)
#define ACCESSCTRL_ADC0_NSP_LSB    _u(1)
#define ACCESSCTRL_ADC0_NSP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_ADC0_NSU
// Description : If 1, and NSP is also set, ADC0 can be accessed from a Non-
//               secure, Unprivileged context.
//
//               This bit is writable from a Non-secure, Privileged context, if
//               and only if the NSP bit is set.
#define ACCESSCTRL_ADC0_NSU_RESET  _u(0x0)
#define ACCESSCTRL_ADC0_NSU_BITS   _u(0x00000001)
#define ACCESSCTRL_ADC0_NSU_MSB    _u(0)
#define ACCESSCTRL_ADC0_NSU_LSB    _u(0)
#define ACCESSCTRL_ADC0_NSU_ACCESS "RW"
// =============================================================================
// Register    : ACCESSCTRL_HSTX
// Description : Control whether debugger, DMA, core 0 and core 1 can access
//               HSTX, and at what security/privilege levels they can do so.
//
//               Defaults to Secure access from any master.
//
//               This register is writable only from a Secure, Privileged
//               processor or debugger, with the exception of the NSU bit, which
//               becomes Non-secure-Privileged-writable when the NSP bit is set.
#define ACCESSCTRL_HSTX_OFFSET _u(0x00000080)
#define ACCESSCTRL_HSTX_BITS   _u(0x000000ff)
#define ACCESSCTRL_HSTX_RESET  _u(0x000000fc)
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_HSTX_DBG
// Description : If 1, HSTX can be accessed by the debugger, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_HSTX_DBG_RESET  _u(0x1)
#define ACCESSCTRL_HSTX_DBG_BITS   _u(0x00000080)
#define ACCESSCTRL_HSTX_DBG_MSB    _u(7)
#define ACCESSCTRL_HSTX_DBG_LSB    _u(7)
#define ACCESSCTRL_HSTX_DBG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_HSTX_DMA
// Description : If 1, HSTX can be accessed by the DMA, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_HSTX_DMA_RESET  _u(0x1)
#define ACCESSCTRL_HSTX_DMA_BITS   _u(0x00000040)
#define ACCESSCTRL_HSTX_DMA_MSB    _u(6)
#define ACCESSCTRL_HSTX_DMA_LSB    _u(6)
#define ACCESSCTRL_HSTX_DMA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_HSTX_CORE1
// Description : If 1, HSTX can be accessed by core 1, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_HSTX_CORE1_RESET  _u(0x1)
#define ACCESSCTRL_HSTX_CORE1_BITS   _u(0x00000020)
#define ACCESSCTRL_HSTX_CORE1_MSB    _u(5)
#define ACCESSCTRL_HSTX_CORE1_LSB    _u(5)
#define ACCESSCTRL_HSTX_CORE1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_HSTX_CORE0
// Description : If 1, HSTX can be accessed by core 0, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_HSTX_CORE0_RESET  _u(0x1)
#define ACCESSCTRL_HSTX_CORE0_BITS   _u(0x00000010)
#define ACCESSCTRL_HSTX_CORE0_MSB    _u(4)
#define ACCESSCTRL_HSTX_CORE0_LSB    _u(4)
#define ACCESSCTRL_HSTX_CORE0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_HSTX_SP
// Description : If 1, HSTX can be accessed from a Secure, Privileged context.
#define ACCESSCTRL_HSTX_SP_RESET  _u(0x1)
#define ACCESSCTRL_HSTX_SP_BITS   _u(0x00000008)
#define ACCESSCTRL_HSTX_SP_MSB    _u(3)
#define ACCESSCTRL_HSTX_SP_LSB    _u(3)
#define ACCESSCTRL_HSTX_SP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_HSTX_SU
// Description : If 1, and SP is also set, HSTX can be accessed from a Secure,
//               Unprivileged context.
#define ACCESSCTRL_HSTX_SU_RESET  _u(0x1)
#define ACCESSCTRL_HSTX_SU_BITS   _u(0x00000004)
#define ACCESSCTRL_HSTX_SU_MSB    _u(2)
#define ACCESSCTRL_HSTX_SU_LSB    _u(2)
#define ACCESSCTRL_HSTX_SU_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_HSTX_NSP
// Description : If 1, HSTX can be accessed from a Non-secure, Privileged
//               context.
#define ACCESSCTRL_HSTX_NSP_RESET  _u(0x0)
#define ACCESSCTRL_HSTX_NSP_BITS   _u(0x00000002)
#define ACCESSCTRL_HSTX_NSP_MSB    _u(1)
#define ACCESSCTRL_HSTX_NSP_LSB    _u(1)
#define ACCESSCTRL_HSTX_NSP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_HSTX_NSU
// Description : If 1, and NSP is also set, HSTX can be accessed from a Non-
//               secure, Unprivileged context.
//
//               This bit is writable from a Non-secure, Privileged context, if
//               and only if the NSP bit is set.
#define ACCESSCTRL_HSTX_NSU_RESET  _u(0x0)
#define ACCESSCTRL_HSTX_NSU_BITS   _u(0x00000001)
#define ACCESSCTRL_HSTX_NSU_MSB    _u(0)
#define ACCESSCTRL_HSTX_NSU_LSB    _u(0)
#define ACCESSCTRL_HSTX_NSU_ACCESS "RW"
// =============================================================================
// Register    : ACCESSCTRL_I2C0
// Description : Control whether debugger, DMA, core 0 and core 1 can access
//               I2C0, and at what security/privilege levels they can do so.
//
//               Defaults to Secure access from any master.
//
//               This register is writable only from a Secure, Privileged
//               processor or debugger, with the exception of the NSU bit, which
//               becomes Non-secure-Privileged-writable when the NSP bit is set.
#define ACCESSCTRL_I2C0_OFFSET _u(0x00000084)
#define ACCESSCTRL_I2C0_BITS   _u(0x000000ff)
#define ACCESSCTRL_I2C0_RESET  _u(0x000000fc)
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_I2C0_DBG
// Description : If 1, I2C0 can be accessed by the debugger, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_I2C0_DBG_RESET  _u(0x1)
#define ACCESSCTRL_I2C0_DBG_BITS   _u(0x00000080)
#define ACCESSCTRL_I2C0_DBG_MSB    _u(7)
#define ACCESSCTRL_I2C0_DBG_LSB    _u(7)
#define ACCESSCTRL_I2C0_DBG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_I2C0_DMA
// Description : If 1, I2C0 can be accessed by the DMA, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_I2C0_DMA_RESET  _u(0x1)
#define ACCESSCTRL_I2C0_DMA_BITS   _u(0x00000040)
#define ACCESSCTRL_I2C0_DMA_MSB    _u(6)
#define ACCESSCTRL_I2C0_DMA_LSB    _u(6)
#define ACCESSCTRL_I2C0_DMA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_I2C0_CORE1
// Description : If 1, I2C0 can be accessed by core 1, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_I2C0_CORE1_RESET  _u(0x1)
#define ACCESSCTRL_I2C0_CORE1_BITS   _u(0x00000020)
#define ACCESSCTRL_I2C0_CORE1_MSB    _u(5)
#define ACCESSCTRL_I2C0_CORE1_LSB    _u(5)
#define ACCESSCTRL_I2C0_CORE1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_I2C0_CORE0
// Description : If 1, I2C0 can be accessed by core 0, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_I2C0_CORE0_RESET  _u(0x1)
#define ACCESSCTRL_I2C0_CORE0_BITS   _u(0x00000010)
#define ACCESSCTRL_I2C0_CORE0_MSB    _u(4)
#define ACCESSCTRL_I2C0_CORE0_LSB    _u(4)
#define ACCESSCTRL_I2C0_CORE0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_I2C0_SP
// Description : If 1, I2C0 can be accessed from a Secure, Privileged context.
#define ACCESSCTRL_I2C0_SP_RESET  _u(0x1)
#define ACCESSCTRL_I2C0_SP_BITS   _u(0x00000008)
#define ACCESSCTRL_I2C0_SP_MSB    _u(3)
#define ACCESSCTRL_I2C0_SP_LSB    _u(3)
#define ACCESSCTRL_I2C0_SP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_I2C0_SU
// Description : If 1, and SP is also set, I2C0 can be accessed from a Secure,
//               Unprivileged context.
#define ACCESSCTRL_I2C0_SU_RESET  _u(0x1)
#define ACCESSCTRL_I2C0_SU_BITS   _u(0x00000004)
#define ACCESSCTRL_I2C0_SU_MSB    _u(2)
#define ACCESSCTRL_I2C0_SU_LSB    _u(2)
#define ACCESSCTRL_I2C0_SU_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_I2C0_NSP
// Description : If 1, I2C0 can be accessed from a Non-secure, Privileged
//               context.
#define ACCESSCTRL_I2C0_NSP_RESET  _u(0x0)
#define ACCESSCTRL_I2C0_NSP_BITS   _u(0x00000002)
#define ACCESSCTRL_I2C0_NSP_MSB    _u(1)
#define ACCESSCTRL_I2C0_NSP_LSB    _u(1)
#define ACCESSCTRL_I2C0_NSP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_I2C0_NSU
// Description : If 1, and NSP is also set, I2C0 can be accessed from a Non-
//               secure, Unprivileged context.
//
//               This bit is writable from a Non-secure, Privileged context, if
//               and only if the NSP bit is set.
#define ACCESSCTRL_I2C0_NSU_RESET  _u(0x0)
#define ACCESSCTRL_I2C0_NSU_BITS   _u(0x00000001)
#define ACCESSCTRL_I2C0_NSU_MSB    _u(0)
#define ACCESSCTRL_I2C0_NSU_LSB    _u(0)
#define ACCESSCTRL_I2C0_NSU_ACCESS "RW"
// =============================================================================
// Register    : ACCESSCTRL_I2C1
// Description : Control whether debugger, DMA, core 0 and core 1 can access
//               I2C1, and at what security/privilege levels they can do so.
//
//               Defaults to Secure access from any master.
//
//               This register is writable only from a Secure, Privileged
//               processor or debugger, with the exception of the NSU bit, which
//               becomes Non-secure-Privileged-writable when the NSP bit is set.
#define ACCESSCTRL_I2C1_OFFSET _u(0x00000088)
#define ACCESSCTRL_I2C1_BITS   _u(0x000000ff)
#define ACCESSCTRL_I2C1_RESET  _u(0x000000fc)
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_I2C1_DBG
// Description : If 1, I2C1 can be accessed by the debugger, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_I2C1_DBG_RESET  _u(0x1)
#define ACCESSCTRL_I2C1_DBG_BITS   _u(0x00000080)
#define ACCESSCTRL_I2C1_DBG_MSB    _u(7)
#define ACCESSCTRL_I2C1_DBG_LSB    _u(7)
#define ACCESSCTRL_I2C1_DBG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_I2C1_DMA
// Description : If 1, I2C1 can be accessed by the DMA, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_I2C1_DMA_RESET  _u(0x1)
#define ACCESSCTRL_I2C1_DMA_BITS   _u(0x00000040)
#define ACCESSCTRL_I2C1_DMA_MSB    _u(6)
#define ACCESSCTRL_I2C1_DMA_LSB    _u(6)
#define ACCESSCTRL_I2C1_DMA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_I2C1_CORE1
// Description : If 1, I2C1 can be accessed by core 1, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_I2C1_CORE1_RESET  _u(0x1)
#define ACCESSCTRL_I2C1_CORE1_BITS   _u(0x00000020)
#define ACCESSCTRL_I2C1_CORE1_MSB    _u(5)
#define ACCESSCTRL_I2C1_CORE1_LSB    _u(5)
#define ACCESSCTRL_I2C1_CORE1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_I2C1_CORE0
// Description : If 1, I2C1 can be accessed by core 0, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_I2C1_CORE0_RESET  _u(0x1)
#define ACCESSCTRL_I2C1_CORE0_BITS   _u(0x00000010)
#define ACCESSCTRL_I2C1_CORE0_MSB    _u(4)
#define ACCESSCTRL_I2C1_CORE0_LSB    _u(4)
#define ACCESSCTRL_I2C1_CORE0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_I2C1_SP
// Description : If 1, I2C1 can be accessed from a Secure, Privileged context.
#define ACCESSCTRL_I2C1_SP_RESET  _u(0x1)
#define ACCESSCTRL_I2C1_SP_BITS   _u(0x00000008)
#define ACCESSCTRL_I2C1_SP_MSB    _u(3)
#define ACCESSCTRL_I2C1_SP_LSB    _u(3)
#define ACCESSCTRL_I2C1_SP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_I2C1_SU
// Description : If 1, and SP is also set, I2C1 can be accessed from a Secure,
//               Unprivileged context.
#define ACCESSCTRL_I2C1_SU_RESET  _u(0x1)
#define ACCESSCTRL_I2C1_SU_BITS   _u(0x00000004)
#define ACCESSCTRL_I2C1_SU_MSB    _u(2)
#define ACCESSCTRL_I2C1_SU_LSB    _u(2)
#define ACCESSCTRL_I2C1_SU_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_I2C1_NSP
// Description : If 1, I2C1 can be accessed from a Non-secure, Privileged
//               context.
#define ACCESSCTRL_I2C1_NSP_RESET  _u(0x0)
#define ACCESSCTRL_I2C1_NSP_BITS   _u(0x00000002)
#define ACCESSCTRL_I2C1_NSP_MSB    _u(1)
#define ACCESSCTRL_I2C1_NSP_LSB    _u(1)
#define ACCESSCTRL_I2C1_NSP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_I2C1_NSU
// Description : If 1, and NSP is also set, I2C1 can be accessed from a Non-
//               secure, Unprivileged context.
//
//               This bit is writable from a Non-secure, Privileged context, if
//               and only if the NSP bit is set.
#define ACCESSCTRL_I2C1_NSU_RESET  _u(0x0)
#define ACCESSCTRL_I2C1_NSU_BITS   _u(0x00000001)
#define ACCESSCTRL_I2C1_NSU_MSB    _u(0)
#define ACCESSCTRL_I2C1_NSU_LSB    _u(0)
#define ACCESSCTRL_I2C1_NSU_ACCESS "RW"
// =============================================================================
// Register    : ACCESSCTRL_PWM
// Description : Control whether debugger, DMA, core 0 and core 1 can access
//               PWM, and at what security/privilege levels they can do so.
//
//               Defaults to Secure access from any master.
//
//               This register is writable only from a Secure, Privileged
//               processor or debugger, with the exception of the NSU bit, which
//               becomes Non-secure-Privileged-writable when the NSP bit is set.
#define ACCESSCTRL_PWM_OFFSET _u(0x0000008c)
#define ACCESSCTRL_PWM_BITS   _u(0x000000ff)
#define ACCESSCTRL_PWM_RESET  _u(0x000000fc)
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PWM_DBG
// Description : If 1, PWM can be accessed by the debugger, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_PWM_DBG_RESET  _u(0x1)
#define ACCESSCTRL_PWM_DBG_BITS   _u(0x00000080)
#define ACCESSCTRL_PWM_DBG_MSB    _u(7)
#define ACCESSCTRL_PWM_DBG_LSB    _u(7)
#define ACCESSCTRL_PWM_DBG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PWM_DMA
// Description : If 1, PWM can be accessed by the DMA, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_PWM_DMA_RESET  _u(0x1)
#define ACCESSCTRL_PWM_DMA_BITS   _u(0x00000040)
#define ACCESSCTRL_PWM_DMA_MSB    _u(6)
#define ACCESSCTRL_PWM_DMA_LSB    _u(6)
#define ACCESSCTRL_PWM_DMA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PWM_CORE1
// Description : If 1, PWM can be accessed by core 1, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_PWM_CORE1_RESET  _u(0x1)
#define ACCESSCTRL_PWM_CORE1_BITS   _u(0x00000020)
#define ACCESSCTRL_PWM_CORE1_MSB    _u(5)
#define ACCESSCTRL_PWM_CORE1_LSB    _u(5)
#define ACCESSCTRL_PWM_CORE1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PWM_CORE0
// Description : If 1, PWM can be accessed by core 0, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_PWM_CORE0_RESET  _u(0x1)
#define ACCESSCTRL_PWM_CORE0_BITS   _u(0x00000010)
#define ACCESSCTRL_PWM_CORE0_MSB    _u(4)
#define ACCESSCTRL_PWM_CORE0_LSB    _u(4)
#define ACCESSCTRL_PWM_CORE0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PWM_SP
// Description : If 1, PWM can be accessed from a Secure, Privileged context.
#define ACCESSCTRL_PWM_SP_RESET  _u(0x1)
#define ACCESSCTRL_PWM_SP_BITS   _u(0x00000008)
#define ACCESSCTRL_PWM_SP_MSB    _u(3)
#define ACCESSCTRL_PWM_SP_LSB    _u(3)
#define ACCESSCTRL_PWM_SP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PWM_SU
// Description : If 1, and SP is also set, PWM can be accessed from a Secure,
//               Unprivileged context.
#define ACCESSCTRL_PWM_SU_RESET  _u(0x1)
#define ACCESSCTRL_PWM_SU_BITS   _u(0x00000004)
#define ACCESSCTRL_PWM_SU_MSB    _u(2)
#define ACCESSCTRL_PWM_SU_LSB    _u(2)
#define ACCESSCTRL_PWM_SU_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PWM_NSP
// Description : If 1, PWM can be accessed from a Non-secure, Privileged
//               context.
#define ACCESSCTRL_PWM_NSP_RESET  _u(0x0)
#define ACCESSCTRL_PWM_NSP_BITS   _u(0x00000002)
#define ACCESSCTRL_PWM_NSP_MSB    _u(1)
#define ACCESSCTRL_PWM_NSP_LSB    _u(1)
#define ACCESSCTRL_PWM_NSP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PWM_NSU
// Description : If 1, and NSP is also set, PWM can be accessed from a Non-
//               secure, Unprivileged context.
//
//               This bit is writable from a Non-secure, Privileged context, if
//               and only if the NSP bit is set.
#define ACCESSCTRL_PWM_NSU_RESET  _u(0x0)
#define ACCESSCTRL_PWM_NSU_BITS   _u(0x00000001)
#define ACCESSCTRL_PWM_NSU_MSB    _u(0)
#define ACCESSCTRL_PWM_NSU_LSB    _u(0)
#define ACCESSCTRL_PWM_NSU_ACCESS "RW"
// =============================================================================
// Register    : ACCESSCTRL_SPI0
// Description : Control whether debugger, DMA, core 0 and core 1 can access
//               SPI0, and at what security/privilege levels they can do so.
//
//               Defaults to Secure access from any master.
//
//               This register is writable only from a Secure, Privileged
//               processor or debugger, with the exception of the NSU bit, which
//               becomes Non-secure-Privileged-writable when the NSP bit is set.
#define ACCESSCTRL_SPI0_OFFSET _u(0x00000090)
#define ACCESSCTRL_SPI0_BITS   _u(0x000000ff)
#define ACCESSCTRL_SPI0_RESET  _u(0x000000fc)
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SPI0_DBG
// Description : If 1, SPI0 can be accessed by the debugger, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_SPI0_DBG_RESET  _u(0x1)
#define ACCESSCTRL_SPI0_DBG_BITS   _u(0x00000080)
#define ACCESSCTRL_SPI0_DBG_MSB    _u(7)
#define ACCESSCTRL_SPI0_DBG_LSB    _u(7)
#define ACCESSCTRL_SPI0_DBG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SPI0_DMA
// Description : If 1, SPI0 can be accessed by the DMA, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_SPI0_DMA_RESET  _u(0x1)
#define ACCESSCTRL_SPI0_DMA_BITS   _u(0x00000040)
#define ACCESSCTRL_SPI0_DMA_MSB    _u(6)
#define ACCESSCTRL_SPI0_DMA_LSB    _u(6)
#define ACCESSCTRL_SPI0_DMA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SPI0_CORE1
// Description : If 1, SPI0 can be accessed by core 1, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_SPI0_CORE1_RESET  _u(0x1)
#define ACCESSCTRL_SPI0_CORE1_BITS   _u(0x00000020)
#define ACCESSCTRL_SPI0_CORE1_MSB    _u(5)
#define ACCESSCTRL_SPI0_CORE1_LSB    _u(5)
#define ACCESSCTRL_SPI0_CORE1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SPI0_CORE0
// Description : If 1, SPI0 can be accessed by core 0, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_SPI0_CORE0_RESET  _u(0x1)
#define ACCESSCTRL_SPI0_CORE0_BITS   _u(0x00000010)
#define ACCESSCTRL_SPI0_CORE0_MSB    _u(4)
#define ACCESSCTRL_SPI0_CORE0_LSB    _u(4)
#define ACCESSCTRL_SPI0_CORE0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SPI0_SP
// Description : If 1, SPI0 can be accessed from a Secure, Privileged context.
#define ACCESSCTRL_SPI0_SP_RESET  _u(0x1)
#define ACCESSCTRL_SPI0_SP_BITS   _u(0x00000008)
#define ACCESSCTRL_SPI0_SP_MSB    _u(3)
#define ACCESSCTRL_SPI0_SP_LSB    _u(3)
#define ACCESSCTRL_SPI0_SP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SPI0_SU
// Description : If 1, and SP is also set, SPI0 can be accessed from a Secure,
//               Unprivileged context.
#define ACCESSCTRL_SPI0_SU_RESET  _u(0x1)
#define ACCESSCTRL_SPI0_SU_BITS   _u(0x00000004)
#define ACCESSCTRL_SPI0_SU_MSB    _u(2)
#define ACCESSCTRL_SPI0_SU_LSB    _u(2)
#define ACCESSCTRL_SPI0_SU_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SPI0_NSP
// Description : If 1, SPI0 can be accessed from a Non-secure, Privileged
//               context.
#define ACCESSCTRL_SPI0_NSP_RESET  _u(0x0)
#define ACCESSCTRL_SPI0_NSP_BITS   _u(0x00000002)
#define ACCESSCTRL_SPI0_NSP_MSB    _u(1)
#define ACCESSCTRL_SPI0_NSP_LSB    _u(1)
#define ACCESSCTRL_SPI0_NSP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SPI0_NSU
// Description : If 1, and NSP is also set, SPI0 can be accessed from a Non-
//               secure, Unprivileged context.
//
//               This bit is writable from a Non-secure, Privileged context, if
//               and only if the NSP bit is set.
#define ACCESSCTRL_SPI0_NSU_RESET  _u(0x0)
#define ACCESSCTRL_SPI0_NSU_BITS   _u(0x00000001)
#define ACCESSCTRL_SPI0_NSU_MSB    _u(0)
#define ACCESSCTRL_SPI0_NSU_LSB    _u(0)
#define ACCESSCTRL_SPI0_NSU_ACCESS "RW"
// =============================================================================
// Register    : ACCESSCTRL_SPI1
// Description : Control whether debugger, DMA, core 0 and core 1 can access
//               SPI1, and at what security/privilege levels they can do so.
//
//               Defaults to Secure access from any master.
//
//               This register is writable only from a Secure, Privileged
//               processor or debugger, with the exception of the NSU bit, which
//               becomes Non-secure-Privileged-writable when the NSP bit is set.
#define ACCESSCTRL_SPI1_OFFSET _u(0x00000094)
#define ACCESSCTRL_SPI1_BITS   _u(0x000000ff)
#define ACCESSCTRL_SPI1_RESET  _u(0x000000fc)
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SPI1_DBG
// Description : If 1, SPI1 can be accessed by the debugger, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_SPI1_DBG_RESET  _u(0x1)
#define ACCESSCTRL_SPI1_DBG_BITS   _u(0x00000080)
#define ACCESSCTRL_SPI1_DBG_MSB    _u(7)
#define ACCESSCTRL_SPI1_DBG_LSB    _u(7)
#define ACCESSCTRL_SPI1_DBG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SPI1_DMA
// Description : If 1, SPI1 can be accessed by the DMA, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_SPI1_DMA_RESET  _u(0x1)
#define ACCESSCTRL_SPI1_DMA_BITS   _u(0x00000040)
#define ACCESSCTRL_SPI1_DMA_MSB    _u(6)
#define ACCESSCTRL_SPI1_DMA_LSB    _u(6)
#define ACCESSCTRL_SPI1_DMA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SPI1_CORE1
// Description : If 1, SPI1 can be accessed by core 1, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_SPI1_CORE1_RESET  _u(0x1)
#define ACCESSCTRL_SPI1_CORE1_BITS   _u(0x00000020)
#define ACCESSCTRL_SPI1_CORE1_MSB    _u(5)
#define ACCESSCTRL_SPI1_CORE1_LSB    _u(5)
#define ACCESSCTRL_SPI1_CORE1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SPI1_CORE0
// Description : If 1, SPI1 can be accessed by core 0, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_SPI1_CORE0_RESET  _u(0x1)
#define ACCESSCTRL_SPI1_CORE0_BITS   _u(0x00000010)
#define ACCESSCTRL_SPI1_CORE0_MSB    _u(4)
#define ACCESSCTRL_SPI1_CORE0_LSB    _u(4)
#define ACCESSCTRL_SPI1_CORE0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SPI1_SP
// Description : If 1, SPI1 can be accessed from a Secure, Privileged context.
#define ACCESSCTRL_SPI1_SP_RESET  _u(0x1)
#define ACCESSCTRL_SPI1_SP_BITS   _u(0x00000008)
#define ACCESSCTRL_SPI1_SP_MSB    _u(3)
#define ACCESSCTRL_SPI1_SP_LSB    _u(3)
#define ACCESSCTRL_SPI1_SP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SPI1_SU
// Description : If 1, and SP is also set, SPI1 can be accessed from a Secure,
//               Unprivileged context.
#define ACCESSCTRL_SPI1_SU_RESET  _u(0x1)
#define ACCESSCTRL_SPI1_SU_BITS   _u(0x00000004)
#define ACCESSCTRL_SPI1_SU_MSB    _u(2)
#define ACCESSCTRL_SPI1_SU_LSB    _u(2)
#define ACCESSCTRL_SPI1_SU_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SPI1_NSP
// Description : If 1, SPI1 can be accessed from a Non-secure, Privileged
//               context.
#define ACCESSCTRL_SPI1_NSP_RESET  _u(0x0)
#define ACCESSCTRL_SPI1_NSP_BITS   _u(0x00000002)
#define ACCESSCTRL_SPI1_NSP_MSB    _u(1)
#define ACCESSCTRL_SPI1_NSP_LSB    _u(1)
#define ACCESSCTRL_SPI1_NSP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SPI1_NSU
// Description : If 1, and NSP is also set, SPI1 can be accessed from a Non-
//               secure, Unprivileged context.
//
//               This bit is writable from a Non-secure, Privileged context, if
//               and only if the NSP bit is set.
#define ACCESSCTRL_SPI1_NSU_RESET  _u(0x0)
#define ACCESSCTRL_SPI1_NSU_BITS   _u(0x00000001)
#define ACCESSCTRL_SPI1_NSU_MSB    _u(0)
#define ACCESSCTRL_SPI1_NSU_LSB    _u(0)
#define ACCESSCTRL_SPI1_NSU_ACCESS "RW"
// =============================================================================
// Register    : ACCESSCTRL_TIMER0
// Description : Control whether debugger, DMA, core 0 and core 1 can access
//               TIMER0, and at what security/privilege levels they can do so.
//
//               Defaults to Secure access from any master.
//
//               This register is writable only from a Secure, Privileged
//               processor or debugger, with the exception of the NSU bit, which
//               becomes Non-secure-Privileged-writable when the NSP bit is set.
#define ACCESSCTRL_TIMER0_OFFSET _u(0x00000098)
#define ACCESSCTRL_TIMER0_BITS   _u(0x000000ff)
#define ACCESSCTRL_TIMER0_RESET  _u(0x000000fc)
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_TIMER0_DBG
// Description : If 1, TIMER0 can be accessed by the debugger, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_TIMER0_DBG_RESET  _u(0x1)
#define ACCESSCTRL_TIMER0_DBG_BITS   _u(0x00000080)
#define ACCESSCTRL_TIMER0_DBG_MSB    _u(7)
#define ACCESSCTRL_TIMER0_DBG_LSB    _u(7)
#define ACCESSCTRL_TIMER0_DBG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_TIMER0_DMA
// Description : If 1, TIMER0 can be accessed by the DMA, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_TIMER0_DMA_RESET  _u(0x1)
#define ACCESSCTRL_TIMER0_DMA_BITS   _u(0x00000040)
#define ACCESSCTRL_TIMER0_DMA_MSB    _u(6)
#define ACCESSCTRL_TIMER0_DMA_LSB    _u(6)
#define ACCESSCTRL_TIMER0_DMA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_TIMER0_CORE1
// Description : If 1, TIMER0 can be accessed by core 1, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_TIMER0_CORE1_RESET  _u(0x1)
#define ACCESSCTRL_TIMER0_CORE1_BITS   _u(0x00000020)
#define ACCESSCTRL_TIMER0_CORE1_MSB    _u(5)
#define ACCESSCTRL_TIMER0_CORE1_LSB    _u(5)
#define ACCESSCTRL_TIMER0_CORE1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_TIMER0_CORE0
// Description : If 1, TIMER0 can be accessed by core 0, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_TIMER0_CORE0_RESET  _u(0x1)
#define ACCESSCTRL_TIMER0_CORE0_BITS   _u(0x00000010)
#define ACCESSCTRL_TIMER0_CORE0_MSB    _u(4)
#define ACCESSCTRL_TIMER0_CORE0_LSB    _u(4)
#define ACCESSCTRL_TIMER0_CORE0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_TIMER0_SP
// Description : If 1, TIMER0 can be accessed from a Secure, Privileged context.
#define ACCESSCTRL_TIMER0_SP_RESET  _u(0x1)
#define ACCESSCTRL_TIMER0_SP_BITS   _u(0x00000008)
#define ACCESSCTRL_TIMER0_SP_MSB    _u(3)
#define ACCESSCTRL_TIMER0_SP_LSB    _u(3)
#define ACCESSCTRL_TIMER0_SP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_TIMER0_SU
// Description : If 1, and SP is also set, TIMER0 can be accessed from a Secure,
//               Unprivileged context.
#define ACCESSCTRL_TIMER0_SU_RESET  _u(0x1)
#define ACCESSCTRL_TIMER0_SU_BITS   _u(0x00000004)
#define ACCESSCTRL_TIMER0_SU_MSB    _u(2)
#define ACCESSCTRL_TIMER0_SU_LSB    _u(2)
#define ACCESSCTRL_TIMER0_SU_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_TIMER0_NSP
// Description : If 1, TIMER0 can be accessed from a Non-secure, Privileged
//               context.
#define ACCESSCTRL_TIMER0_NSP_RESET  _u(0x0)
#define ACCESSCTRL_TIMER0_NSP_BITS   _u(0x00000002)
#define ACCESSCTRL_TIMER0_NSP_MSB    _u(1)
#define ACCESSCTRL_TIMER0_NSP_LSB    _u(1)
#define ACCESSCTRL_TIMER0_NSP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_TIMER0_NSU
// Description : If 1, and NSP is also set, TIMER0 can be accessed from a Non-
//               secure, Unprivileged context.
//
//               This bit is writable from a Non-secure, Privileged context, if
//               and only if the NSP bit is set.
#define ACCESSCTRL_TIMER0_NSU_RESET  _u(0x0)
#define ACCESSCTRL_TIMER0_NSU_BITS   _u(0x00000001)
#define ACCESSCTRL_TIMER0_NSU_MSB    _u(0)
#define ACCESSCTRL_TIMER0_NSU_LSB    _u(0)
#define ACCESSCTRL_TIMER0_NSU_ACCESS "RW"
// =============================================================================
// Register    : ACCESSCTRL_TIMER1
// Description : Control whether debugger, DMA, core 0 and core 1 can access
//               TIMER1, and at what security/privilege levels they can do so.
//
//               Defaults to Secure access from any master.
//
//               This register is writable only from a Secure, Privileged
//               processor or debugger, with the exception of the NSU bit, which
//               becomes Non-secure-Privileged-writable when the NSP bit is set.
#define ACCESSCTRL_TIMER1_OFFSET _u(0x0000009c)
#define ACCESSCTRL_TIMER1_BITS   _u(0x000000ff)
#define ACCESSCTRL_TIMER1_RESET  _u(0x000000fc)
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_TIMER1_DBG
// Description : If 1, TIMER1 can be accessed by the debugger, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_TIMER1_DBG_RESET  _u(0x1)
#define ACCESSCTRL_TIMER1_DBG_BITS   _u(0x00000080)
#define ACCESSCTRL_TIMER1_DBG_MSB    _u(7)
#define ACCESSCTRL_TIMER1_DBG_LSB    _u(7)
#define ACCESSCTRL_TIMER1_DBG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_TIMER1_DMA
// Description : If 1, TIMER1 can be accessed by the DMA, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_TIMER1_DMA_RESET  _u(0x1)
#define ACCESSCTRL_TIMER1_DMA_BITS   _u(0x00000040)
#define ACCESSCTRL_TIMER1_DMA_MSB    _u(6)
#define ACCESSCTRL_TIMER1_DMA_LSB    _u(6)
#define ACCESSCTRL_TIMER1_DMA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_TIMER1_CORE1
// Description : If 1, TIMER1 can be accessed by core 1, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_TIMER1_CORE1_RESET  _u(0x1)
#define ACCESSCTRL_TIMER1_CORE1_BITS   _u(0x00000020)
#define ACCESSCTRL_TIMER1_CORE1_MSB    _u(5)
#define ACCESSCTRL_TIMER1_CORE1_LSB    _u(5)
#define ACCESSCTRL_TIMER1_CORE1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_TIMER1_CORE0
// Description : If 1, TIMER1 can be accessed by core 0, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_TIMER1_CORE0_RESET  _u(0x1)
#define ACCESSCTRL_TIMER1_CORE0_BITS   _u(0x00000010)
#define ACCESSCTRL_TIMER1_CORE0_MSB    _u(4)
#define ACCESSCTRL_TIMER1_CORE0_LSB    _u(4)
#define ACCESSCTRL_TIMER1_CORE0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_TIMER1_SP
// Description : If 1, TIMER1 can be accessed from a Secure, Privileged context.
#define ACCESSCTRL_TIMER1_SP_RESET  _u(0x1)
#define ACCESSCTRL_TIMER1_SP_BITS   _u(0x00000008)
#define ACCESSCTRL_TIMER1_SP_MSB    _u(3)
#define ACCESSCTRL_TIMER1_SP_LSB    _u(3)
#define ACCESSCTRL_TIMER1_SP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_TIMER1_SU
// Description : If 1, and SP is also set, TIMER1 can be accessed from a Secure,
//               Unprivileged context.
#define ACCESSCTRL_TIMER1_SU_RESET  _u(0x1)
#define ACCESSCTRL_TIMER1_SU_BITS   _u(0x00000004)
#define ACCESSCTRL_TIMER1_SU_MSB    _u(2)
#define ACCESSCTRL_TIMER1_SU_LSB    _u(2)
#define ACCESSCTRL_TIMER1_SU_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_TIMER1_NSP
// Description : If 1, TIMER1 can be accessed from a Non-secure, Privileged
//               context.
#define ACCESSCTRL_TIMER1_NSP_RESET  _u(0x0)
#define ACCESSCTRL_TIMER1_NSP_BITS   _u(0x00000002)
#define ACCESSCTRL_TIMER1_NSP_MSB    _u(1)
#define ACCESSCTRL_TIMER1_NSP_LSB    _u(1)
#define ACCESSCTRL_TIMER1_NSP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_TIMER1_NSU
// Description : If 1, and NSP is also set, TIMER1 can be accessed from a Non-
//               secure, Unprivileged context.
//
//               This bit is writable from a Non-secure, Privileged context, if
//               and only if the NSP bit is set.
#define ACCESSCTRL_TIMER1_NSU_RESET  _u(0x0)
#define ACCESSCTRL_TIMER1_NSU_BITS   _u(0x00000001)
#define ACCESSCTRL_TIMER1_NSU_MSB    _u(0)
#define ACCESSCTRL_TIMER1_NSU_LSB    _u(0)
#define ACCESSCTRL_TIMER1_NSU_ACCESS "RW"
// =============================================================================
// Register    : ACCESSCTRL_UART0
// Description : Control whether debugger, DMA, core 0 and core 1 can access
//               UART0, and at what security/privilege levels they can do so.
//
//               Defaults to Secure access from any master.
//
//               This register is writable only from a Secure, Privileged
//               processor or debugger, with the exception of the NSU bit, which
//               becomes Non-secure-Privileged-writable when the NSP bit is set.
#define ACCESSCTRL_UART0_OFFSET _u(0x000000a0)
#define ACCESSCTRL_UART0_BITS   _u(0x000000ff)
#define ACCESSCTRL_UART0_RESET  _u(0x000000fc)
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_UART0_DBG
// Description : If 1, UART0 can be accessed by the debugger, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_UART0_DBG_RESET  _u(0x1)
#define ACCESSCTRL_UART0_DBG_BITS   _u(0x00000080)
#define ACCESSCTRL_UART0_DBG_MSB    _u(7)
#define ACCESSCTRL_UART0_DBG_LSB    _u(7)
#define ACCESSCTRL_UART0_DBG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_UART0_DMA
// Description : If 1, UART0 can be accessed by the DMA, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_UART0_DMA_RESET  _u(0x1)
#define ACCESSCTRL_UART0_DMA_BITS   _u(0x00000040)
#define ACCESSCTRL_UART0_DMA_MSB    _u(6)
#define ACCESSCTRL_UART0_DMA_LSB    _u(6)
#define ACCESSCTRL_UART0_DMA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_UART0_CORE1
// Description : If 1, UART0 can be accessed by core 1, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_UART0_CORE1_RESET  _u(0x1)
#define ACCESSCTRL_UART0_CORE1_BITS   _u(0x00000020)
#define ACCESSCTRL_UART0_CORE1_MSB    _u(5)
#define ACCESSCTRL_UART0_CORE1_LSB    _u(5)
#define ACCESSCTRL_UART0_CORE1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_UART0_CORE0
// Description : If 1, UART0 can be accessed by core 0, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_UART0_CORE0_RESET  _u(0x1)
#define ACCESSCTRL_UART0_CORE0_BITS   _u(0x00000010)
#define ACCESSCTRL_UART0_CORE0_MSB    _u(4)
#define ACCESSCTRL_UART0_CORE0_LSB    _u(4)
#define ACCESSCTRL_UART0_CORE0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_UART0_SP
// Description : If 1, UART0 can be accessed from a Secure, Privileged context.
#define ACCESSCTRL_UART0_SP_RESET  _u(0x1)
#define ACCESSCTRL_UART0_SP_BITS   _u(0x00000008)
#define ACCESSCTRL_UART0_SP_MSB    _u(3)
#define ACCESSCTRL_UART0_SP_LSB    _u(3)
#define ACCESSCTRL_UART0_SP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_UART0_SU
// Description : If 1, and SP is also set, UART0 can be accessed from a Secure,
//               Unprivileged context.
#define ACCESSCTRL_UART0_SU_RESET  _u(0x1)
#define ACCESSCTRL_UART0_SU_BITS   _u(0x00000004)
#define ACCESSCTRL_UART0_SU_MSB    _u(2)
#define ACCESSCTRL_UART0_SU_LSB    _u(2)
#define ACCESSCTRL_UART0_SU_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_UART0_NSP
// Description : If 1, UART0 can be accessed from a Non-secure, Privileged
//               context.
#define ACCESSCTRL_UART0_NSP_RESET  _u(0x0)
#define ACCESSCTRL_UART0_NSP_BITS   _u(0x00000002)
#define ACCESSCTRL_UART0_NSP_MSB    _u(1)
#define ACCESSCTRL_UART0_NSP_LSB    _u(1)
#define ACCESSCTRL_UART0_NSP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_UART0_NSU
// Description : If 1, and NSP is also set, UART0 can be accessed from a Non-
//               secure, Unprivileged context.
//
//               This bit is writable from a Non-secure, Privileged context, if
//               and only if the NSP bit is set.
#define ACCESSCTRL_UART0_NSU_RESET  _u(0x0)
#define ACCESSCTRL_UART0_NSU_BITS   _u(0x00000001)
#define ACCESSCTRL_UART0_NSU_MSB    _u(0)
#define ACCESSCTRL_UART0_NSU_LSB    _u(0)
#define ACCESSCTRL_UART0_NSU_ACCESS "RW"
// =============================================================================
// Register    : ACCESSCTRL_UART1
// Description : Control whether debugger, DMA, core 0 and core 1 can access
//               UART1, and at what security/privilege levels they can do so.
//
//               Defaults to Secure access from any master.
//
//               This register is writable only from a Secure, Privileged
//               processor or debugger, with the exception of the NSU bit, which
//               becomes Non-secure-Privileged-writable when the NSP bit is set.
#define ACCESSCTRL_UART1_OFFSET _u(0x000000a4)
#define ACCESSCTRL_UART1_BITS   _u(0x000000ff)
#define ACCESSCTRL_UART1_RESET  _u(0x000000fc)
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_UART1_DBG
// Description : If 1, UART1 can be accessed by the debugger, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_UART1_DBG_RESET  _u(0x1)
#define ACCESSCTRL_UART1_DBG_BITS   _u(0x00000080)
#define ACCESSCTRL_UART1_DBG_MSB    _u(7)
#define ACCESSCTRL_UART1_DBG_LSB    _u(7)
#define ACCESSCTRL_UART1_DBG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_UART1_DMA
// Description : If 1, UART1 can be accessed by the DMA, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_UART1_DMA_RESET  _u(0x1)
#define ACCESSCTRL_UART1_DMA_BITS   _u(0x00000040)
#define ACCESSCTRL_UART1_DMA_MSB    _u(6)
#define ACCESSCTRL_UART1_DMA_LSB    _u(6)
#define ACCESSCTRL_UART1_DMA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_UART1_CORE1
// Description : If 1, UART1 can be accessed by core 1, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_UART1_CORE1_RESET  _u(0x1)
#define ACCESSCTRL_UART1_CORE1_BITS   _u(0x00000020)
#define ACCESSCTRL_UART1_CORE1_MSB    _u(5)
#define ACCESSCTRL_UART1_CORE1_LSB    _u(5)
#define ACCESSCTRL_UART1_CORE1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_UART1_CORE0
// Description : If 1, UART1 can be accessed by core 0, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_UART1_CORE0_RESET  _u(0x1)
#define ACCESSCTRL_UART1_CORE0_BITS   _u(0x00000010)
#define ACCESSCTRL_UART1_CORE0_MSB    _u(4)
#define ACCESSCTRL_UART1_CORE0_LSB    _u(4)
#define ACCESSCTRL_UART1_CORE0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_UART1_SP
// Description : If 1, UART1 can be accessed from a Secure, Privileged context.
#define ACCESSCTRL_UART1_SP_RESET  _u(0x1)
#define ACCESSCTRL_UART1_SP_BITS   _u(0x00000008)
#define ACCESSCTRL_UART1_SP_MSB    _u(3)
#define ACCESSCTRL_UART1_SP_LSB    _u(3)
#define ACCESSCTRL_UART1_SP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_UART1_SU
// Description : If 1, and SP is also set, UART1 can be accessed from a Secure,
//               Unprivileged context.
#define ACCESSCTRL_UART1_SU_RESET  _u(0x1)
#define ACCESSCTRL_UART1_SU_BITS   _u(0x00000004)
#define ACCESSCTRL_UART1_SU_MSB    _u(2)
#define ACCESSCTRL_UART1_SU_LSB    _u(2)
#define ACCESSCTRL_UART1_SU_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_UART1_NSP
// Description : If 1, UART1 can be accessed from a Non-secure, Privileged
//               context.
#define ACCESSCTRL_UART1_NSP_RESET  _u(0x0)
#define ACCESSCTRL_UART1_NSP_BITS   _u(0x00000002)
#define ACCESSCTRL_UART1_NSP_MSB    _u(1)
#define ACCESSCTRL_UART1_NSP_LSB    _u(1)
#define ACCESSCTRL_UART1_NSP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_UART1_NSU
// Description : If 1, and NSP is also set, UART1 can be accessed from a Non-
//               secure, Unprivileged context.
//
//               This bit is writable from a Non-secure, Privileged context, if
//               and only if the NSP bit is set.
#define ACCESSCTRL_UART1_NSU_RESET  _u(0x0)
#define ACCESSCTRL_UART1_NSU_BITS   _u(0x00000001)
#define ACCESSCTRL_UART1_NSU_MSB    _u(0)
#define ACCESSCTRL_UART1_NSU_LSB    _u(0)
#define ACCESSCTRL_UART1_NSU_ACCESS "RW"
// =============================================================================
// Register    : ACCESSCTRL_OTP
// Description : Control whether debugger, DMA, core 0 and core 1 can access
//               OTP, and at what security/privilege levels they can do so.
//
//               Defaults to Secure access from any master.
//
//               This register is writable only from a Secure, Privileged
//               processor or debugger, with the exception of the NSU bit, which
//               becomes Non-secure-Privileged-writable when the NSP bit is set.
#define ACCESSCTRL_OTP_OFFSET _u(0x000000a8)
#define ACCESSCTRL_OTP_BITS   _u(0x000000ff)
#define ACCESSCTRL_OTP_RESET  _u(0x000000fc)
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_OTP_DBG
// Description : If 1, OTP can be accessed by the debugger, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_OTP_DBG_RESET  _u(0x1)
#define ACCESSCTRL_OTP_DBG_BITS   _u(0x00000080)
#define ACCESSCTRL_OTP_DBG_MSB    _u(7)
#define ACCESSCTRL_OTP_DBG_LSB    _u(7)
#define ACCESSCTRL_OTP_DBG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_OTP_DMA
// Description : If 1, OTP can be accessed by the DMA, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_OTP_DMA_RESET  _u(0x1)
#define ACCESSCTRL_OTP_DMA_BITS   _u(0x00000040)
#define ACCESSCTRL_OTP_DMA_MSB    _u(6)
#define ACCESSCTRL_OTP_DMA_LSB    _u(6)
#define ACCESSCTRL_OTP_DMA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_OTP_CORE1
// Description : If 1, OTP can be accessed by core 1, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_OTP_CORE1_RESET  _u(0x1)
#define ACCESSCTRL_OTP_CORE1_BITS   _u(0x00000020)
#define ACCESSCTRL_OTP_CORE1_MSB    _u(5)
#define ACCESSCTRL_OTP_CORE1_LSB    _u(5)
#define ACCESSCTRL_OTP_CORE1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_OTP_CORE0
// Description : If 1, OTP can be accessed by core 0, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_OTP_CORE0_RESET  _u(0x1)
#define ACCESSCTRL_OTP_CORE0_BITS   _u(0x00000010)
#define ACCESSCTRL_OTP_CORE0_MSB    _u(4)
#define ACCESSCTRL_OTP_CORE0_LSB    _u(4)
#define ACCESSCTRL_OTP_CORE0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_OTP_SP
// Description : If 1, OTP can be accessed from a Secure, Privileged context.
#define ACCESSCTRL_OTP_SP_RESET  _u(0x1)
#define ACCESSCTRL_OTP_SP_BITS   _u(0x00000008)
#define ACCESSCTRL_OTP_SP_MSB    _u(3)
#define ACCESSCTRL_OTP_SP_LSB    _u(3)
#define ACCESSCTRL_OTP_SP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_OTP_SU
// Description : If 1, and SP is also set, OTP can be accessed from a Secure,
//               Unprivileged context.
#define ACCESSCTRL_OTP_SU_RESET  _u(0x1)
#define ACCESSCTRL_OTP_SU_BITS   _u(0x00000004)
#define ACCESSCTRL_OTP_SU_MSB    _u(2)
#define ACCESSCTRL_OTP_SU_LSB    _u(2)
#define ACCESSCTRL_OTP_SU_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_OTP_NSP
// Description : If 1, OTP can be accessed from a Non-secure, Privileged
//               context.
#define ACCESSCTRL_OTP_NSP_RESET  _u(0x0)
#define ACCESSCTRL_OTP_NSP_BITS   _u(0x00000002)
#define ACCESSCTRL_OTP_NSP_MSB    _u(1)
#define ACCESSCTRL_OTP_NSP_LSB    _u(1)
#define ACCESSCTRL_OTP_NSP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_OTP_NSU
// Description : If 1, and NSP is also set, OTP can be accessed from a Non-
//               secure, Unprivileged context.
//
//               This bit is writable from a Non-secure, Privileged context, if
//               and only if the NSP bit is set.
#define ACCESSCTRL_OTP_NSU_RESET  _u(0x0)
#define ACCESSCTRL_OTP_NSU_BITS   _u(0x00000001)
#define ACCESSCTRL_OTP_NSU_MSB    _u(0)
#define ACCESSCTRL_OTP_NSU_LSB    _u(0)
#define ACCESSCTRL_OTP_NSU_ACCESS "RW"
// =============================================================================
// Register    : ACCESSCTRL_TBMAN
// Description : Control whether debugger, DMA, core 0 and core 1 can access
//               TBMAN, and at what security/privilege levels they can do so.
//
//               Defaults to Secure access from any master.
//
//               This register is writable only from a Secure, Privileged
//               processor or debugger, with the exception of the NSU bit, which
//               becomes Non-secure-Privileged-writable when the NSP bit is set.
#define ACCESSCTRL_TBMAN_OFFSET _u(0x000000ac)
#define ACCESSCTRL_TBMAN_BITS   _u(0x000000ff)
#define ACCESSCTRL_TBMAN_RESET  _u(0x000000fc)
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_TBMAN_DBG
// Description : If 1, TBMAN can be accessed by the debugger, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_TBMAN_DBG_RESET  _u(0x1)
#define ACCESSCTRL_TBMAN_DBG_BITS   _u(0x00000080)
#define ACCESSCTRL_TBMAN_DBG_MSB    _u(7)
#define ACCESSCTRL_TBMAN_DBG_LSB    _u(7)
#define ACCESSCTRL_TBMAN_DBG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_TBMAN_DMA
// Description : If 1, TBMAN can be accessed by the DMA, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_TBMAN_DMA_RESET  _u(0x1)
#define ACCESSCTRL_TBMAN_DMA_BITS   _u(0x00000040)
#define ACCESSCTRL_TBMAN_DMA_MSB    _u(6)
#define ACCESSCTRL_TBMAN_DMA_LSB    _u(6)
#define ACCESSCTRL_TBMAN_DMA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_TBMAN_CORE1
// Description : If 1, TBMAN can be accessed by core 1, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_TBMAN_CORE1_RESET  _u(0x1)
#define ACCESSCTRL_TBMAN_CORE1_BITS   _u(0x00000020)
#define ACCESSCTRL_TBMAN_CORE1_MSB    _u(5)
#define ACCESSCTRL_TBMAN_CORE1_LSB    _u(5)
#define ACCESSCTRL_TBMAN_CORE1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_TBMAN_CORE0
// Description : If 1, TBMAN can be accessed by core 0, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_TBMAN_CORE0_RESET  _u(0x1)
#define ACCESSCTRL_TBMAN_CORE0_BITS   _u(0x00000010)
#define ACCESSCTRL_TBMAN_CORE0_MSB    _u(4)
#define ACCESSCTRL_TBMAN_CORE0_LSB    _u(4)
#define ACCESSCTRL_TBMAN_CORE0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_TBMAN_SP
// Description : If 1, TBMAN can be accessed from a Secure, Privileged context.
#define ACCESSCTRL_TBMAN_SP_RESET  _u(0x1)
#define ACCESSCTRL_TBMAN_SP_BITS   _u(0x00000008)
#define ACCESSCTRL_TBMAN_SP_MSB    _u(3)
#define ACCESSCTRL_TBMAN_SP_LSB    _u(3)
#define ACCESSCTRL_TBMAN_SP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_TBMAN_SU
// Description : If 1, and SP is also set, TBMAN can be accessed from a Secure,
//               Unprivileged context.
#define ACCESSCTRL_TBMAN_SU_RESET  _u(0x1)
#define ACCESSCTRL_TBMAN_SU_BITS   _u(0x00000004)
#define ACCESSCTRL_TBMAN_SU_MSB    _u(2)
#define ACCESSCTRL_TBMAN_SU_LSB    _u(2)
#define ACCESSCTRL_TBMAN_SU_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_TBMAN_NSP
// Description : If 1, TBMAN can be accessed from a Non-secure, Privileged
//               context.
#define ACCESSCTRL_TBMAN_NSP_RESET  _u(0x0)
#define ACCESSCTRL_TBMAN_NSP_BITS   _u(0x00000002)
#define ACCESSCTRL_TBMAN_NSP_MSB    _u(1)
#define ACCESSCTRL_TBMAN_NSP_LSB    _u(1)
#define ACCESSCTRL_TBMAN_NSP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_TBMAN_NSU
// Description : If 1, and NSP is also set, TBMAN can be accessed from a Non-
//               secure, Unprivileged context.
//
//               This bit is writable from a Non-secure, Privileged context, if
//               and only if the NSP bit is set.
#define ACCESSCTRL_TBMAN_NSU_RESET  _u(0x0)
#define ACCESSCTRL_TBMAN_NSU_BITS   _u(0x00000001)
#define ACCESSCTRL_TBMAN_NSU_MSB    _u(0)
#define ACCESSCTRL_TBMAN_NSU_LSB    _u(0)
#define ACCESSCTRL_TBMAN_NSU_ACCESS "RW"
// =============================================================================
// Register    : ACCESSCTRL_POWMAN
// Description : Control whether debugger, DMA, core 0 and core 1 can access
//               POWMAN, and at what security/privilege levels they can do so.
//
//               Defaults to Secure, Privileged processor or debug access only.
//
//               This register is writable only from a Secure, Privileged
//               processor or debugger, with the exception of the NSU bit, which
//               becomes Non-secure-Privileged-writable when the NSP bit is set.
#define ACCESSCTRL_POWMAN_OFFSET _u(0x000000b0)
#define ACCESSCTRL_POWMAN_BITS   _u(0x000000ff)
#define ACCESSCTRL_POWMAN_RESET  _u(0x000000b8)
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_POWMAN_DBG
// Description : If 1, POWMAN can be accessed by the debugger, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_POWMAN_DBG_RESET  _u(0x1)
#define ACCESSCTRL_POWMAN_DBG_BITS   _u(0x00000080)
#define ACCESSCTRL_POWMAN_DBG_MSB    _u(7)
#define ACCESSCTRL_POWMAN_DBG_LSB    _u(7)
#define ACCESSCTRL_POWMAN_DBG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_POWMAN_DMA
// Description : If 1, POWMAN can be accessed by the DMA, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_POWMAN_DMA_RESET  _u(0x0)
#define ACCESSCTRL_POWMAN_DMA_BITS   _u(0x00000040)
#define ACCESSCTRL_POWMAN_DMA_MSB    _u(6)
#define ACCESSCTRL_POWMAN_DMA_LSB    _u(6)
#define ACCESSCTRL_POWMAN_DMA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_POWMAN_CORE1
// Description : If 1, POWMAN can be accessed by core 1, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_POWMAN_CORE1_RESET  _u(0x1)
#define ACCESSCTRL_POWMAN_CORE1_BITS   _u(0x00000020)
#define ACCESSCTRL_POWMAN_CORE1_MSB    _u(5)
#define ACCESSCTRL_POWMAN_CORE1_LSB    _u(5)
#define ACCESSCTRL_POWMAN_CORE1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_POWMAN_CORE0
// Description : If 1, POWMAN can be accessed by core 0, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_POWMAN_CORE0_RESET  _u(0x1)
#define ACCESSCTRL_POWMAN_CORE0_BITS   _u(0x00000010)
#define ACCESSCTRL_POWMAN_CORE0_MSB    _u(4)
#define ACCESSCTRL_POWMAN_CORE0_LSB    _u(4)
#define ACCESSCTRL_POWMAN_CORE0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_POWMAN_SP
// Description : If 1, POWMAN can be accessed from a Secure, Privileged context.
#define ACCESSCTRL_POWMAN_SP_RESET  _u(0x1)
#define ACCESSCTRL_POWMAN_SP_BITS   _u(0x00000008)
#define ACCESSCTRL_POWMAN_SP_MSB    _u(3)
#define ACCESSCTRL_POWMAN_SP_LSB    _u(3)
#define ACCESSCTRL_POWMAN_SP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_POWMAN_SU
// Description : If 1, and SP is also set, POWMAN can be accessed from a Secure,
//               Unprivileged context.
#define ACCESSCTRL_POWMAN_SU_RESET  _u(0x0)
#define ACCESSCTRL_POWMAN_SU_BITS   _u(0x00000004)
#define ACCESSCTRL_POWMAN_SU_MSB    _u(2)
#define ACCESSCTRL_POWMAN_SU_LSB    _u(2)
#define ACCESSCTRL_POWMAN_SU_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_POWMAN_NSP
// Description : If 1, POWMAN can be accessed from a Non-secure, Privileged
//               context.
#define ACCESSCTRL_POWMAN_NSP_RESET  _u(0x0)
#define ACCESSCTRL_POWMAN_NSP_BITS   _u(0x00000002)
#define ACCESSCTRL_POWMAN_NSP_MSB    _u(1)
#define ACCESSCTRL_POWMAN_NSP_LSB    _u(1)
#define ACCESSCTRL_POWMAN_NSP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_POWMAN_NSU
// Description : If 1, and NSP is also set, POWMAN can be accessed from a Non-
//               secure, Unprivileged context.
//
//               This bit is writable from a Non-secure, Privileged context, if
//               and only if the NSP bit is set.
#define ACCESSCTRL_POWMAN_NSU_RESET  _u(0x0)
#define ACCESSCTRL_POWMAN_NSU_BITS   _u(0x00000001)
#define ACCESSCTRL_POWMAN_NSU_MSB    _u(0)
#define ACCESSCTRL_POWMAN_NSU_LSB    _u(0)
#define ACCESSCTRL_POWMAN_NSU_ACCESS "RW"
// =============================================================================
// Register    : ACCESSCTRL_TRNG
// Description : Control whether debugger, DMA, core 0 and core 1 can access
//               TRNG, and at what security/privilege levels they can do so.
//
//               Defaults to Secure, Privileged processor or debug access only.
//
//               This register is writable only from a Secure, Privileged
//               processor or debugger, with the exception of the NSU bit, which
//               becomes Non-secure-Privileged-writable when the NSP bit is set.
#define ACCESSCTRL_TRNG_OFFSET _u(0x000000b4)
#define ACCESSCTRL_TRNG_BITS   _u(0x000000ff)
#define ACCESSCTRL_TRNG_RESET  _u(0x000000b8)
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_TRNG_DBG
// Description : If 1, TRNG can be accessed by the debugger, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_TRNG_DBG_RESET  _u(0x1)
#define ACCESSCTRL_TRNG_DBG_BITS   _u(0x00000080)
#define ACCESSCTRL_TRNG_DBG_MSB    _u(7)
#define ACCESSCTRL_TRNG_DBG_LSB    _u(7)
#define ACCESSCTRL_TRNG_DBG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_TRNG_DMA
// Description : If 1, TRNG can be accessed by the DMA, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_TRNG_DMA_RESET  _u(0x0)
#define ACCESSCTRL_TRNG_DMA_BITS   _u(0x00000040)
#define ACCESSCTRL_TRNG_DMA_MSB    _u(6)
#define ACCESSCTRL_TRNG_DMA_LSB    _u(6)
#define ACCESSCTRL_TRNG_DMA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_TRNG_CORE1
// Description : If 1, TRNG can be accessed by core 1, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_TRNG_CORE1_RESET  _u(0x1)
#define ACCESSCTRL_TRNG_CORE1_BITS   _u(0x00000020)
#define ACCESSCTRL_TRNG_CORE1_MSB    _u(5)
#define ACCESSCTRL_TRNG_CORE1_LSB    _u(5)
#define ACCESSCTRL_TRNG_CORE1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_TRNG_CORE0
// Description : If 1, TRNG can be accessed by core 0, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_TRNG_CORE0_RESET  _u(0x1)
#define ACCESSCTRL_TRNG_CORE0_BITS   _u(0x00000010)
#define ACCESSCTRL_TRNG_CORE0_MSB    _u(4)
#define ACCESSCTRL_TRNG_CORE0_LSB    _u(4)
#define ACCESSCTRL_TRNG_CORE0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_TRNG_SP
// Description : If 1, TRNG can be accessed from a Secure, Privileged context.
#define ACCESSCTRL_TRNG_SP_RESET  _u(0x1)
#define ACCESSCTRL_TRNG_SP_BITS   _u(0x00000008)
#define ACCESSCTRL_TRNG_SP_MSB    _u(3)
#define ACCESSCTRL_TRNG_SP_LSB    _u(3)
#define ACCESSCTRL_TRNG_SP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_TRNG_SU
// Description : If 1, and SP is also set, TRNG can be accessed from a Secure,
//               Unprivileged context.
#define ACCESSCTRL_TRNG_SU_RESET  _u(0x0)
#define ACCESSCTRL_TRNG_SU_BITS   _u(0x00000004)
#define ACCESSCTRL_TRNG_SU_MSB    _u(2)
#define ACCESSCTRL_TRNG_SU_LSB    _u(2)
#define ACCESSCTRL_TRNG_SU_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_TRNG_NSP
// Description : If 1, TRNG can be accessed from a Non-secure, Privileged
//               context.
#define ACCESSCTRL_TRNG_NSP_RESET  _u(0x0)
#define ACCESSCTRL_TRNG_NSP_BITS   _u(0x00000002)
#define ACCESSCTRL_TRNG_NSP_MSB    _u(1)
#define ACCESSCTRL_TRNG_NSP_LSB    _u(1)
#define ACCESSCTRL_TRNG_NSP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_TRNG_NSU
// Description : If 1, and NSP is also set, TRNG can be accessed from a Non-
//               secure, Unprivileged context.
//
//               This bit is writable from a Non-secure, Privileged context, if
//               and only if the NSP bit is set.
#define ACCESSCTRL_TRNG_NSU_RESET  _u(0x0)
#define ACCESSCTRL_TRNG_NSU_BITS   _u(0x00000001)
#define ACCESSCTRL_TRNG_NSU_MSB    _u(0)
#define ACCESSCTRL_TRNG_NSU_LSB    _u(0)
#define ACCESSCTRL_TRNG_NSU_ACCESS "RW"
// =============================================================================
// Register    : ACCESSCTRL_SHA256
// Description : Control whether debugger, DMA, core 0 and core 1 can access
//               SHA256, and at what security/privilege levels they can do so.
//
//               Defaults to Secure, Privileged access only.
//
//               This register is writable only from a Secure, Privileged
//               processor or debugger, with the exception of the NSU bit, which
//               becomes Non-secure-Privileged-writable when the NSP bit is set.
#define ACCESSCTRL_SHA256_OFFSET _u(0x000000b8)
#define ACCESSCTRL_SHA256_BITS   _u(0x000000ff)
#define ACCESSCTRL_SHA256_RESET  _u(0x000000f8)
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SHA256_DBG
// Description : If 1, SHA256 can be accessed by the debugger, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_SHA256_DBG_RESET  _u(0x1)
#define ACCESSCTRL_SHA256_DBG_BITS   _u(0x00000080)
#define ACCESSCTRL_SHA256_DBG_MSB    _u(7)
#define ACCESSCTRL_SHA256_DBG_LSB    _u(7)
#define ACCESSCTRL_SHA256_DBG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SHA256_DMA
// Description : If 1, SHA256 can be accessed by the DMA, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_SHA256_DMA_RESET  _u(0x1)
#define ACCESSCTRL_SHA256_DMA_BITS   _u(0x00000040)
#define ACCESSCTRL_SHA256_DMA_MSB    _u(6)
#define ACCESSCTRL_SHA256_DMA_LSB    _u(6)
#define ACCESSCTRL_SHA256_DMA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SHA256_CORE1
// Description : If 1, SHA256 can be accessed by core 1, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_SHA256_CORE1_RESET  _u(0x1)
#define ACCESSCTRL_SHA256_CORE1_BITS   _u(0x00000020)
#define ACCESSCTRL_SHA256_CORE1_MSB    _u(5)
#define ACCESSCTRL_SHA256_CORE1_LSB    _u(5)
#define ACCESSCTRL_SHA256_CORE1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SHA256_CORE0
// Description : If 1, SHA256 can be accessed by core 0, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_SHA256_CORE0_RESET  _u(0x1)
#define ACCESSCTRL_SHA256_CORE0_BITS   _u(0x00000010)
#define ACCESSCTRL_SHA256_CORE0_MSB    _u(4)
#define ACCESSCTRL_SHA256_CORE0_LSB    _u(4)
#define ACCESSCTRL_SHA256_CORE0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SHA256_SP
// Description : If 1, SHA256 can be accessed from a Secure, Privileged context.
#define ACCESSCTRL_SHA256_SP_RESET  _u(0x1)
#define ACCESSCTRL_SHA256_SP_BITS   _u(0x00000008)
#define ACCESSCTRL_SHA256_SP_MSB    _u(3)
#define ACCESSCTRL_SHA256_SP_LSB    _u(3)
#define ACCESSCTRL_SHA256_SP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SHA256_SU
// Description : If 1, and SP is also set, SHA256 can be accessed from a Secure,
//               Unprivileged context.
#define ACCESSCTRL_SHA256_SU_RESET  _u(0x0)
#define ACCESSCTRL_SHA256_SU_BITS   _u(0x00000004)
#define ACCESSCTRL_SHA256_SU_MSB    _u(2)
#define ACCESSCTRL_SHA256_SU_LSB    _u(2)
#define ACCESSCTRL_SHA256_SU_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SHA256_NSP
// Description : If 1, SHA256 can be accessed from a Non-secure, Privileged
//               context.
#define ACCESSCTRL_SHA256_NSP_RESET  _u(0x0)
#define ACCESSCTRL_SHA256_NSP_BITS   _u(0x00000002)
#define ACCESSCTRL_SHA256_NSP_MSB    _u(1)
#define ACCESSCTRL_SHA256_NSP_LSB    _u(1)
#define ACCESSCTRL_SHA256_NSP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SHA256_NSU
// Description : If 1, and NSP is also set, SHA256 can be accessed from a Non-
//               secure, Unprivileged context.
//
//               This bit is writable from a Non-secure, Privileged context, if
//               and only if the NSP bit is set.
#define ACCESSCTRL_SHA256_NSU_RESET  _u(0x0)
#define ACCESSCTRL_SHA256_NSU_BITS   _u(0x00000001)
#define ACCESSCTRL_SHA256_NSU_MSB    _u(0)
#define ACCESSCTRL_SHA256_NSU_LSB    _u(0)
#define ACCESSCTRL_SHA256_NSU_ACCESS "RW"
// =============================================================================
// Register    : ACCESSCTRL_SYSCFG
// Description : Control whether debugger, DMA, core 0 and core 1 can access
//               SYSCFG, and at what security/privilege levels they can do so.
//
//               Defaults to Secure, Privileged processor or debug access only.
//
//               This register is writable only from a Secure, Privileged
//               processor or debugger, with the exception of the NSU bit, which
//               becomes Non-secure-Privileged-writable when the NSP bit is set.
#define ACCESSCTRL_SYSCFG_OFFSET _u(0x000000bc)
#define ACCESSCTRL_SYSCFG_BITS   _u(0x000000ff)
#define ACCESSCTRL_SYSCFG_RESET  _u(0x000000b8)
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SYSCFG_DBG
// Description : If 1, SYSCFG can be accessed by the debugger, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_SYSCFG_DBG_RESET  _u(0x1)
#define ACCESSCTRL_SYSCFG_DBG_BITS   _u(0x00000080)
#define ACCESSCTRL_SYSCFG_DBG_MSB    _u(7)
#define ACCESSCTRL_SYSCFG_DBG_LSB    _u(7)
#define ACCESSCTRL_SYSCFG_DBG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SYSCFG_DMA
// Description : If 1, SYSCFG can be accessed by the DMA, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_SYSCFG_DMA_RESET  _u(0x0)
#define ACCESSCTRL_SYSCFG_DMA_BITS   _u(0x00000040)
#define ACCESSCTRL_SYSCFG_DMA_MSB    _u(6)
#define ACCESSCTRL_SYSCFG_DMA_LSB    _u(6)
#define ACCESSCTRL_SYSCFG_DMA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SYSCFG_CORE1
// Description : If 1, SYSCFG can be accessed by core 1, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_SYSCFG_CORE1_RESET  _u(0x1)
#define ACCESSCTRL_SYSCFG_CORE1_BITS   _u(0x00000020)
#define ACCESSCTRL_SYSCFG_CORE1_MSB    _u(5)
#define ACCESSCTRL_SYSCFG_CORE1_LSB    _u(5)
#define ACCESSCTRL_SYSCFG_CORE1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SYSCFG_CORE0
// Description : If 1, SYSCFG can be accessed by core 0, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_SYSCFG_CORE0_RESET  _u(0x1)
#define ACCESSCTRL_SYSCFG_CORE0_BITS   _u(0x00000010)
#define ACCESSCTRL_SYSCFG_CORE0_MSB    _u(4)
#define ACCESSCTRL_SYSCFG_CORE0_LSB    _u(4)
#define ACCESSCTRL_SYSCFG_CORE0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SYSCFG_SP
// Description : If 1, SYSCFG can be accessed from a Secure, Privileged context.
#define ACCESSCTRL_SYSCFG_SP_RESET  _u(0x1)
#define ACCESSCTRL_SYSCFG_SP_BITS   _u(0x00000008)
#define ACCESSCTRL_SYSCFG_SP_MSB    _u(3)
#define ACCESSCTRL_SYSCFG_SP_LSB    _u(3)
#define ACCESSCTRL_SYSCFG_SP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SYSCFG_SU
// Description : If 1, and SP is also set, SYSCFG can be accessed from a Secure,
//               Unprivileged context.
#define ACCESSCTRL_SYSCFG_SU_RESET  _u(0x0)
#define ACCESSCTRL_SYSCFG_SU_BITS   _u(0x00000004)
#define ACCESSCTRL_SYSCFG_SU_MSB    _u(2)
#define ACCESSCTRL_SYSCFG_SU_LSB    _u(2)
#define ACCESSCTRL_SYSCFG_SU_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SYSCFG_NSP
// Description : If 1, SYSCFG can be accessed from a Non-secure, Privileged
//               context.
#define ACCESSCTRL_SYSCFG_NSP_RESET  _u(0x0)
#define ACCESSCTRL_SYSCFG_NSP_BITS   _u(0x00000002)
#define ACCESSCTRL_SYSCFG_NSP_MSB    _u(1)
#define ACCESSCTRL_SYSCFG_NSP_LSB    _u(1)
#define ACCESSCTRL_SYSCFG_NSP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_SYSCFG_NSU
// Description : If 1, and NSP is also set, SYSCFG can be accessed from a Non-
//               secure, Unprivileged context.
//
//               This bit is writable from a Non-secure, Privileged context, if
//               and only if the NSP bit is set.
#define ACCESSCTRL_SYSCFG_NSU_RESET  _u(0x0)
#define ACCESSCTRL_SYSCFG_NSU_BITS   _u(0x00000001)
#define ACCESSCTRL_SYSCFG_NSU_MSB    _u(0)
#define ACCESSCTRL_SYSCFG_NSU_LSB    _u(0)
#define ACCESSCTRL_SYSCFG_NSU_ACCESS "RW"
// =============================================================================
// Register    : ACCESSCTRL_CLOCKS
// Description : Control whether debugger, DMA, core 0 and core 1 can access
//               CLOCKS, and at what security/privilege levels they can do so.
//
//               Defaults to Secure, Privileged processor or debug access only.
//
//               This register is writable only from a Secure, Privileged
//               processor or debugger, with the exception of the NSU bit, which
//               becomes Non-secure-Privileged-writable when the NSP bit is set.
#define ACCESSCTRL_CLOCKS_OFFSET _u(0x000000c0)
#define ACCESSCTRL_CLOCKS_BITS   _u(0x000000ff)
#define ACCESSCTRL_CLOCKS_RESET  _u(0x000000b8)
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_CLOCKS_DBG
// Description : If 1, CLOCKS can be accessed by the debugger, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_CLOCKS_DBG_RESET  _u(0x1)
#define ACCESSCTRL_CLOCKS_DBG_BITS   _u(0x00000080)
#define ACCESSCTRL_CLOCKS_DBG_MSB    _u(7)
#define ACCESSCTRL_CLOCKS_DBG_LSB    _u(7)
#define ACCESSCTRL_CLOCKS_DBG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_CLOCKS_DMA
// Description : If 1, CLOCKS can be accessed by the DMA, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_CLOCKS_DMA_RESET  _u(0x0)
#define ACCESSCTRL_CLOCKS_DMA_BITS   _u(0x00000040)
#define ACCESSCTRL_CLOCKS_DMA_MSB    _u(6)
#define ACCESSCTRL_CLOCKS_DMA_LSB    _u(6)
#define ACCESSCTRL_CLOCKS_DMA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_CLOCKS_CORE1
// Description : If 1, CLOCKS can be accessed by core 1, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_CLOCKS_CORE1_RESET  _u(0x1)
#define ACCESSCTRL_CLOCKS_CORE1_BITS   _u(0x00000020)
#define ACCESSCTRL_CLOCKS_CORE1_MSB    _u(5)
#define ACCESSCTRL_CLOCKS_CORE1_LSB    _u(5)
#define ACCESSCTRL_CLOCKS_CORE1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_CLOCKS_CORE0
// Description : If 1, CLOCKS can be accessed by core 0, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_CLOCKS_CORE0_RESET  _u(0x1)
#define ACCESSCTRL_CLOCKS_CORE0_BITS   _u(0x00000010)
#define ACCESSCTRL_CLOCKS_CORE0_MSB    _u(4)
#define ACCESSCTRL_CLOCKS_CORE0_LSB    _u(4)
#define ACCESSCTRL_CLOCKS_CORE0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_CLOCKS_SP
// Description : If 1, CLOCKS can be accessed from a Secure, Privileged context.
#define ACCESSCTRL_CLOCKS_SP_RESET  _u(0x1)
#define ACCESSCTRL_CLOCKS_SP_BITS   _u(0x00000008)
#define ACCESSCTRL_CLOCKS_SP_MSB    _u(3)
#define ACCESSCTRL_CLOCKS_SP_LSB    _u(3)
#define ACCESSCTRL_CLOCKS_SP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_CLOCKS_SU
// Description : If 1, and SP is also set, CLOCKS can be accessed from a Secure,
//               Unprivileged context.
#define ACCESSCTRL_CLOCKS_SU_RESET  _u(0x0)
#define ACCESSCTRL_CLOCKS_SU_BITS   _u(0x00000004)
#define ACCESSCTRL_CLOCKS_SU_MSB    _u(2)
#define ACCESSCTRL_CLOCKS_SU_LSB    _u(2)
#define ACCESSCTRL_CLOCKS_SU_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_CLOCKS_NSP
// Description : If 1, CLOCKS can be accessed from a Non-secure, Privileged
//               context.
#define ACCESSCTRL_CLOCKS_NSP_RESET  _u(0x0)
#define ACCESSCTRL_CLOCKS_NSP_BITS   _u(0x00000002)
#define ACCESSCTRL_CLOCKS_NSP_MSB    _u(1)
#define ACCESSCTRL_CLOCKS_NSP_LSB    _u(1)
#define ACCESSCTRL_CLOCKS_NSP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_CLOCKS_NSU
// Description : If 1, and NSP is also set, CLOCKS can be accessed from a Non-
//               secure, Unprivileged context.
//
//               This bit is writable from a Non-secure, Privileged context, if
//               and only if the NSP bit is set.
#define ACCESSCTRL_CLOCKS_NSU_RESET  _u(0x0)
#define ACCESSCTRL_CLOCKS_NSU_BITS   _u(0x00000001)
#define ACCESSCTRL_CLOCKS_NSU_MSB    _u(0)
#define ACCESSCTRL_CLOCKS_NSU_LSB    _u(0)
#define ACCESSCTRL_CLOCKS_NSU_ACCESS "RW"
// =============================================================================
// Register    : ACCESSCTRL_XOSC
// Description : Control whether debugger, DMA, core 0 and core 1 can access
//               XOSC, and at what security/privilege levels they can do so.
//
//               Defaults to Secure, Privileged processor or debug access only.
//
//               This register is writable only from a Secure, Privileged
//               processor or debugger, with the exception of the NSU bit, which
//               becomes Non-secure-Privileged-writable when the NSP bit is set.
#define ACCESSCTRL_XOSC_OFFSET _u(0x000000c4)
#define ACCESSCTRL_XOSC_BITS   _u(0x000000ff)
#define ACCESSCTRL_XOSC_RESET  _u(0x000000b8)
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_XOSC_DBG
// Description : If 1, XOSC can be accessed by the debugger, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_XOSC_DBG_RESET  _u(0x1)
#define ACCESSCTRL_XOSC_DBG_BITS   _u(0x00000080)
#define ACCESSCTRL_XOSC_DBG_MSB    _u(7)
#define ACCESSCTRL_XOSC_DBG_LSB    _u(7)
#define ACCESSCTRL_XOSC_DBG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_XOSC_DMA
// Description : If 1, XOSC can be accessed by the DMA, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_XOSC_DMA_RESET  _u(0x0)
#define ACCESSCTRL_XOSC_DMA_BITS   _u(0x00000040)
#define ACCESSCTRL_XOSC_DMA_MSB    _u(6)
#define ACCESSCTRL_XOSC_DMA_LSB    _u(6)
#define ACCESSCTRL_XOSC_DMA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_XOSC_CORE1
// Description : If 1, XOSC can be accessed by core 1, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_XOSC_CORE1_RESET  _u(0x1)
#define ACCESSCTRL_XOSC_CORE1_BITS   _u(0x00000020)
#define ACCESSCTRL_XOSC_CORE1_MSB    _u(5)
#define ACCESSCTRL_XOSC_CORE1_LSB    _u(5)
#define ACCESSCTRL_XOSC_CORE1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_XOSC_CORE0
// Description : If 1, XOSC can be accessed by core 0, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_XOSC_CORE0_RESET  _u(0x1)
#define ACCESSCTRL_XOSC_CORE0_BITS   _u(0x00000010)
#define ACCESSCTRL_XOSC_CORE0_MSB    _u(4)
#define ACCESSCTRL_XOSC_CORE0_LSB    _u(4)
#define ACCESSCTRL_XOSC_CORE0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_XOSC_SP
// Description : If 1, XOSC can be accessed from a Secure, Privileged context.
#define ACCESSCTRL_XOSC_SP_RESET  _u(0x1)
#define ACCESSCTRL_XOSC_SP_BITS   _u(0x00000008)
#define ACCESSCTRL_XOSC_SP_MSB    _u(3)
#define ACCESSCTRL_XOSC_SP_LSB    _u(3)
#define ACCESSCTRL_XOSC_SP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_XOSC_SU
// Description : If 1, and SP is also set, XOSC can be accessed from a Secure,
//               Unprivileged context.
#define ACCESSCTRL_XOSC_SU_RESET  _u(0x0)
#define ACCESSCTRL_XOSC_SU_BITS   _u(0x00000004)
#define ACCESSCTRL_XOSC_SU_MSB    _u(2)
#define ACCESSCTRL_XOSC_SU_LSB    _u(2)
#define ACCESSCTRL_XOSC_SU_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_XOSC_NSP
// Description : If 1, XOSC can be accessed from a Non-secure, Privileged
//               context.
#define ACCESSCTRL_XOSC_NSP_RESET  _u(0x0)
#define ACCESSCTRL_XOSC_NSP_BITS   _u(0x00000002)
#define ACCESSCTRL_XOSC_NSP_MSB    _u(1)
#define ACCESSCTRL_XOSC_NSP_LSB    _u(1)
#define ACCESSCTRL_XOSC_NSP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_XOSC_NSU
// Description : If 1, and NSP is also set, XOSC can be accessed from a Non-
//               secure, Unprivileged context.
//
//               This bit is writable from a Non-secure, Privileged context, if
//               and only if the NSP bit is set.
#define ACCESSCTRL_XOSC_NSU_RESET  _u(0x0)
#define ACCESSCTRL_XOSC_NSU_BITS   _u(0x00000001)
#define ACCESSCTRL_XOSC_NSU_MSB    _u(0)
#define ACCESSCTRL_XOSC_NSU_LSB    _u(0)
#define ACCESSCTRL_XOSC_NSU_ACCESS "RW"
// =============================================================================
// Register    : ACCESSCTRL_ROSC
// Description : Control whether debugger, DMA, core 0 and core 1 can access
//               ROSC, and at what security/privilege levels they can do so.
//
//               Defaults to Secure, Privileged processor or debug access only.
//
//               This register is writable only from a Secure, Privileged
//               processor or debugger, with the exception of the NSU bit, which
//               becomes Non-secure-Privileged-writable when the NSP bit is set.
#define ACCESSCTRL_ROSC_OFFSET _u(0x000000c8)
#define ACCESSCTRL_ROSC_BITS   _u(0x000000ff)
#define ACCESSCTRL_ROSC_RESET  _u(0x000000b8)
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_ROSC_DBG
// Description : If 1, ROSC can be accessed by the debugger, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_ROSC_DBG_RESET  _u(0x1)
#define ACCESSCTRL_ROSC_DBG_BITS   _u(0x00000080)
#define ACCESSCTRL_ROSC_DBG_MSB    _u(7)
#define ACCESSCTRL_ROSC_DBG_LSB    _u(7)
#define ACCESSCTRL_ROSC_DBG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_ROSC_DMA
// Description : If 1, ROSC can be accessed by the DMA, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_ROSC_DMA_RESET  _u(0x0)
#define ACCESSCTRL_ROSC_DMA_BITS   _u(0x00000040)
#define ACCESSCTRL_ROSC_DMA_MSB    _u(6)
#define ACCESSCTRL_ROSC_DMA_LSB    _u(6)
#define ACCESSCTRL_ROSC_DMA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_ROSC_CORE1
// Description : If 1, ROSC can be accessed by core 1, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_ROSC_CORE1_RESET  _u(0x1)
#define ACCESSCTRL_ROSC_CORE1_BITS   _u(0x00000020)
#define ACCESSCTRL_ROSC_CORE1_MSB    _u(5)
#define ACCESSCTRL_ROSC_CORE1_LSB    _u(5)
#define ACCESSCTRL_ROSC_CORE1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_ROSC_CORE0
// Description : If 1, ROSC can be accessed by core 0, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_ROSC_CORE0_RESET  _u(0x1)
#define ACCESSCTRL_ROSC_CORE0_BITS   _u(0x00000010)
#define ACCESSCTRL_ROSC_CORE0_MSB    _u(4)
#define ACCESSCTRL_ROSC_CORE0_LSB    _u(4)
#define ACCESSCTRL_ROSC_CORE0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_ROSC_SP
// Description : If 1, ROSC can be accessed from a Secure, Privileged context.
#define ACCESSCTRL_ROSC_SP_RESET  _u(0x1)
#define ACCESSCTRL_ROSC_SP_BITS   _u(0x00000008)
#define ACCESSCTRL_ROSC_SP_MSB    _u(3)
#define ACCESSCTRL_ROSC_SP_LSB    _u(3)
#define ACCESSCTRL_ROSC_SP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_ROSC_SU
// Description : If 1, and SP is also set, ROSC can be accessed from a Secure,
//               Unprivileged context.
#define ACCESSCTRL_ROSC_SU_RESET  _u(0x0)
#define ACCESSCTRL_ROSC_SU_BITS   _u(0x00000004)
#define ACCESSCTRL_ROSC_SU_MSB    _u(2)
#define ACCESSCTRL_ROSC_SU_LSB    _u(2)
#define ACCESSCTRL_ROSC_SU_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_ROSC_NSP
// Description : If 1, ROSC can be accessed from a Non-secure, Privileged
//               context.
#define ACCESSCTRL_ROSC_NSP_RESET  _u(0x0)
#define ACCESSCTRL_ROSC_NSP_BITS   _u(0x00000002)
#define ACCESSCTRL_ROSC_NSP_MSB    _u(1)
#define ACCESSCTRL_ROSC_NSP_LSB    _u(1)
#define ACCESSCTRL_ROSC_NSP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_ROSC_NSU
// Description : If 1, and NSP is also set, ROSC can be accessed from a Non-
//               secure, Unprivileged context.
//
//               This bit is writable from a Non-secure, Privileged context, if
//               and only if the NSP bit is set.
#define ACCESSCTRL_ROSC_NSU_RESET  _u(0x0)
#define ACCESSCTRL_ROSC_NSU_BITS   _u(0x00000001)
#define ACCESSCTRL_ROSC_NSU_MSB    _u(0)
#define ACCESSCTRL_ROSC_NSU_LSB    _u(0)
#define ACCESSCTRL_ROSC_NSU_ACCESS "RW"
// =============================================================================
// Register    : ACCESSCTRL_PLL_SYS
// Description : Control whether debugger, DMA, core 0 and core 1 can access
//               PLL_SYS, and at what security/privilege levels they can do so.
//
//               Defaults to Secure, Privileged processor or debug access only.
//
//               This register is writable only from a Secure, Privileged
//               processor or debugger, with the exception of the NSU bit, which
//               becomes Non-secure-Privileged-writable when the NSP bit is set.
#define ACCESSCTRL_PLL_SYS_OFFSET _u(0x000000cc)
#define ACCESSCTRL_PLL_SYS_BITS   _u(0x000000ff)
#define ACCESSCTRL_PLL_SYS_RESET  _u(0x000000b8)
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PLL_SYS_DBG
// Description : If 1, PLL_SYS can be accessed by the debugger, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_PLL_SYS_DBG_RESET  _u(0x1)
#define ACCESSCTRL_PLL_SYS_DBG_BITS   _u(0x00000080)
#define ACCESSCTRL_PLL_SYS_DBG_MSB    _u(7)
#define ACCESSCTRL_PLL_SYS_DBG_LSB    _u(7)
#define ACCESSCTRL_PLL_SYS_DBG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PLL_SYS_DMA
// Description : If 1, PLL_SYS can be accessed by the DMA, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_PLL_SYS_DMA_RESET  _u(0x0)
#define ACCESSCTRL_PLL_SYS_DMA_BITS   _u(0x00000040)
#define ACCESSCTRL_PLL_SYS_DMA_MSB    _u(6)
#define ACCESSCTRL_PLL_SYS_DMA_LSB    _u(6)
#define ACCESSCTRL_PLL_SYS_DMA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PLL_SYS_CORE1
// Description : If 1, PLL_SYS can be accessed by core 1, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_PLL_SYS_CORE1_RESET  _u(0x1)
#define ACCESSCTRL_PLL_SYS_CORE1_BITS   _u(0x00000020)
#define ACCESSCTRL_PLL_SYS_CORE1_MSB    _u(5)
#define ACCESSCTRL_PLL_SYS_CORE1_LSB    _u(5)
#define ACCESSCTRL_PLL_SYS_CORE1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PLL_SYS_CORE0
// Description : If 1, PLL_SYS can be accessed by core 0, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_PLL_SYS_CORE0_RESET  _u(0x1)
#define ACCESSCTRL_PLL_SYS_CORE0_BITS   _u(0x00000010)
#define ACCESSCTRL_PLL_SYS_CORE0_MSB    _u(4)
#define ACCESSCTRL_PLL_SYS_CORE0_LSB    _u(4)
#define ACCESSCTRL_PLL_SYS_CORE0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PLL_SYS_SP
// Description : If 1, PLL_SYS can be accessed from a Secure, Privileged
//               context.
#define ACCESSCTRL_PLL_SYS_SP_RESET  _u(0x1)
#define ACCESSCTRL_PLL_SYS_SP_BITS   _u(0x00000008)
#define ACCESSCTRL_PLL_SYS_SP_MSB    _u(3)
#define ACCESSCTRL_PLL_SYS_SP_LSB    _u(3)
#define ACCESSCTRL_PLL_SYS_SP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PLL_SYS_SU
// Description : If 1, and SP is also set, PLL_SYS can be accessed from a
//               Secure, Unprivileged context.
#define ACCESSCTRL_PLL_SYS_SU_RESET  _u(0x0)
#define ACCESSCTRL_PLL_SYS_SU_BITS   _u(0x00000004)
#define ACCESSCTRL_PLL_SYS_SU_MSB    _u(2)
#define ACCESSCTRL_PLL_SYS_SU_LSB    _u(2)
#define ACCESSCTRL_PLL_SYS_SU_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PLL_SYS_NSP
// Description : If 1, PLL_SYS can be accessed from a Non-secure, Privileged
//               context.
#define ACCESSCTRL_PLL_SYS_NSP_RESET  _u(0x0)
#define ACCESSCTRL_PLL_SYS_NSP_BITS   _u(0x00000002)
#define ACCESSCTRL_PLL_SYS_NSP_MSB    _u(1)
#define ACCESSCTRL_PLL_SYS_NSP_LSB    _u(1)
#define ACCESSCTRL_PLL_SYS_NSP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PLL_SYS_NSU
// Description : If 1, and NSP is also set, PLL_SYS can be accessed from a Non-
//               secure, Unprivileged context.
//
//               This bit is writable from a Non-secure, Privileged context, if
//               and only if the NSP bit is set.
#define ACCESSCTRL_PLL_SYS_NSU_RESET  _u(0x0)
#define ACCESSCTRL_PLL_SYS_NSU_BITS   _u(0x00000001)
#define ACCESSCTRL_PLL_SYS_NSU_MSB    _u(0)
#define ACCESSCTRL_PLL_SYS_NSU_LSB    _u(0)
#define ACCESSCTRL_PLL_SYS_NSU_ACCESS "RW"
// =============================================================================
// Register    : ACCESSCTRL_PLL_USB
// Description : Control whether debugger, DMA, core 0 and core 1 can access
//               PLL_USB, and at what security/privilege levels they can do so.
//
//               Defaults to Secure, Privileged processor or debug access only.
//
//               This register is writable only from a Secure, Privileged
//               processor or debugger, with the exception of the NSU bit, which
//               becomes Non-secure-Privileged-writable when the NSP bit is set.
#define ACCESSCTRL_PLL_USB_OFFSET _u(0x000000d0)
#define ACCESSCTRL_PLL_USB_BITS   _u(0x000000ff)
#define ACCESSCTRL_PLL_USB_RESET  _u(0x000000b8)
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PLL_USB_DBG
// Description : If 1, PLL_USB can be accessed by the debugger, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_PLL_USB_DBG_RESET  _u(0x1)
#define ACCESSCTRL_PLL_USB_DBG_BITS   _u(0x00000080)
#define ACCESSCTRL_PLL_USB_DBG_MSB    _u(7)
#define ACCESSCTRL_PLL_USB_DBG_LSB    _u(7)
#define ACCESSCTRL_PLL_USB_DBG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PLL_USB_DMA
// Description : If 1, PLL_USB can be accessed by the DMA, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_PLL_USB_DMA_RESET  _u(0x0)
#define ACCESSCTRL_PLL_USB_DMA_BITS   _u(0x00000040)
#define ACCESSCTRL_PLL_USB_DMA_MSB    _u(6)
#define ACCESSCTRL_PLL_USB_DMA_LSB    _u(6)
#define ACCESSCTRL_PLL_USB_DMA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PLL_USB_CORE1
// Description : If 1, PLL_USB can be accessed by core 1, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_PLL_USB_CORE1_RESET  _u(0x1)
#define ACCESSCTRL_PLL_USB_CORE1_BITS   _u(0x00000020)
#define ACCESSCTRL_PLL_USB_CORE1_MSB    _u(5)
#define ACCESSCTRL_PLL_USB_CORE1_LSB    _u(5)
#define ACCESSCTRL_PLL_USB_CORE1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PLL_USB_CORE0
// Description : If 1, PLL_USB can be accessed by core 0, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_PLL_USB_CORE0_RESET  _u(0x1)
#define ACCESSCTRL_PLL_USB_CORE0_BITS   _u(0x00000010)
#define ACCESSCTRL_PLL_USB_CORE0_MSB    _u(4)
#define ACCESSCTRL_PLL_USB_CORE0_LSB    _u(4)
#define ACCESSCTRL_PLL_USB_CORE0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PLL_USB_SP
// Description : If 1, PLL_USB can be accessed from a Secure, Privileged
//               context.
#define ACCESSCTRL_PLL_USB_SP_RESET  _u(0x1)
#define ACCESSCTRL_PLL_USB_SP_BITS   _u(0x00000008)
#define ACCESSCTRL_PLL_USB_SP_MSB    _u(3)
#define ACCESSCTRL_PLL_USB_SP_LSB    _u(3)
#define ACCESSCTRL_PLL_USB_SP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PLL_USB_SU
// Description : If 1, and SP is also set, PLL_USB can be accessed from a
//               Secure, Unprivileged context.
#define ACCESSCTRL_PLL_USB_SU_RESET  _u(0x0)
#define ACCESSCTRL_PLL_USB_SU_BITS   _u(0x00000004)
#define ACCESSCTRL_PLL_USB_SU_MSB    _u(2)
#define ACCESSCTRL_PLL_USB_SU_LSB    _u(2)
#define ACCESSCTRL_PLL_USB_SU_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PLL_USB_NSP
// Description : If 1, PLL_USB can be accessed from a Non-secure, Privileged
//               context.
#define ACCESSCTRL_PLL_USB_NSP_RESET  _u(0x0)
#define ACCESSCTRL_PLL_USB_NSP_BITS   _u(0x00000002)
#define ACCESSCTRL_PLL_USB_NSP_MSB    _u(1)
#define ACCESSCTRL_PLL_USB_NSP_LSB    _u(1)
#define ACCESSCTRL_PLL_USB_NSP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_PLL_USB_NSU
// Description : If 1, and NSP is also set, PLL_USB can be accessed from a Non-
//               secure, Unprivileged context.
//
//               This bit is writable from a Non-secure, Privileged context, if
//               and only if the NSP bit is set.
#define ACCESSCTRL_PLL_USB_NSU_RESET  _u(0x0)
#define ACCESSCTRL_PLL_USB_NSU_BITS   _u(0x00000001)
#define ACCESSCTRL_PLL_USB_NSU_MSB    _u(0)
#define ACCESSCTRL_PLL_USB_NSU_LSB    _u(0)
#define ACCESSCTRL_PLL_USB_NSU_ACCESS "RW"
// =============================================================================
// Register    : ACCESSCTRL_TICKS
// Description : Control whether debugger, DMA, core 0 and core 1 can access
//               TICKS, and at what security/privilege levels they can do so.
//
//               Defaults to Secure, Privileged processor or debug access only.
//
//               This register is writable only from a Secure, Privileged
//               processor or debugger, with the exception of the NSU bit, which
//               becomes Non-secure-Privileged-writable when the NSP bit is set.
#define ACCESSCTRL_TICKS_OFFSET _u(0x000000d4)
#define ACCESSCTRL_TICKS_BITS   _u(0x000000ff)
#define ACCESSCTRL_TICKS_RESET  _u(0x000000b8)
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_TICKS_DBG
// Description : If 1, TICKS can be accessed by the debugger, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_TICKS_DBG_RESET  _u(0x1)
#define ACCESSCTRL_TICKS_DBG_BITS   _u(0x00000080)
#define ACCESSCTRL_TICKS_DBG_MSB    _u(7)
#define ACCESSCTRL_TICKS_DBG_LSB    _u(7)
#define ACCESSCTRL_TICKS_DBG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_TICKS_DMA
// Description : If 1, TICKS can be accessed by the DMA, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_TICKS_DMA_RESET  _u(0x0)
#define ACCESSCTRL_TICKS_DMA_BITS   _u(0x00000040)
#define ACCESSCTRL_TICKS_DMA_MSB    _u(6)
#define ACCESSCTRL_TICKS_DMA_LSB    _u(6)
#define ACCESSCTRL_TICKS_DMA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_TICKS_CORE1
// Description : If 1, TICKS can be accessed by core 1, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_TICKS_CORE1_RESET  _u(0x1)
#define ACCESSCTRL_TICKS_CORE1_BITS   _u(0x00000020)
#define ACCESSCTRL_TICKS_CORE1_MSB    _u(5)
#define ACCESSCTRL_TICKS_CORE1_LSB    _u(5)
#define ACCESSCTRL_TICKS_CORE1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_TICKS_CORE0
// Description : If 1, TICKS can be accessed by core 0, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_TICKS_CORE0_RESET  _u(0x1)
#define ACCESSCTRL_TICKS_CORE0_BITS   _u(0x00000010)
#define ACCESSCTRL_TICKS_CORE0_MSB    _u(4)
#define ACCESSCTRL_TICKS_CORE0_LSB    _u(4)
#define ACCESSCTRL_TICKS_CORE0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_TICKS_SP
// Description : If 1, TICKS can be accessed from a Secure, Privileged context.
#define ACCESSCTRL_TICKS_SP_RESET  _u(0x1)
#define ACCESSCTRL_TICKS_SP_BITS   _u(0x00000008)
#define ACCESSCTRL_TICKS_SP_MSB    _u(3)
#define ACCESSCTRL_TICKS_SP_LSB    _u(3)
#define ACCESSCTRL_TICKS_SP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_TICKS_SU
// Description : If 1, and SP is also set, TICKS can be accessed from a Secure,
//               Unprivileged context.
#define ACCESSCTRL_TICKS_SU_RESET  _u(0x0)
#define ACCESSCTRL_TICKS_SU_BITS   _u(0x00000004)
#define ACCESSCTRL_TICKS_SU_MSB    _u(2)
#define ACCESSCTRL_TICKS_SU_LSB    _u(2)
#define ACCESSCTRL_TICKS_SU_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_TICKS_NSP
// Description : If 1, TICKS can be accessed from a Non-secure, Privileged
//               context.
#define ACCESSCTRL_TICKS_NSP_RESET  _u(0x0)
#define ACCESSCTRL_TICKS_NSP_BITS   _u(0x00000002)
#define ACCESSCTRL_TICKS_NSP_MSB    _u(1)
#define ACCESSCTRL_TICKS_NSP_LSB    _u(1)
#define ACCESSCTRL_TICKS_NSP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_TICKS_NSU
// Description : If 1, and NSP is also set, TICKS can be accessed from a Non-
//               secure, Unprivileged context.
//
//               This bit is writable from a Non-secure, Privileged context, if
//               and only if the NSP bit is set.
#define ACCESSCTRL_TICKS_NSU_RESET  _u(0x0)
#define ACCESSCTRL_TICKS_NSU_BITS   _u(0x00000001)
#define ACCESSCTRL_TICKS_NSU_MSB    _u(0)
#define ACCESSCTRL_TICKS_NSU_LSB    _u(0)
#define ACCESSCTRL_TICKS_NSU_ACCESS "RW"
// =============================================================================
// Register    : ACCESSCTRL_WATCHDOG
// Description : Control whether debugger, DMA, core 0 and core 1 can access
//               WATCHDOG, and at what security/privilege levels they can do so.
//
//               Defaults to Secure, Privileged processor or debug access only.
//
//               This register is writable only from a Secure, Privileged
//               processor or debugger, with the exception of the NSU bit, which
//               becomes Non-secure-Privileged-writable when the NSP bit is set.
#define ACCESSCTRL_WATCHDOG_OFFSET _u(0x000000d8)
#define ACCESSCTRL_WATCHDOG_BITS   _u(0x000000ff)
#define ACCESSCTRL_WATCHDOG_RESET  _u(0x000000b8)
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_WATCHDOG_DBG
// Description : If 1, WATCHDOG can be accessed by the debugger, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_WATCHDOG_DBG_RESET  _u(0x1)
#define ACCESSCTRL_WATCHDOG_DBG_BITS   _u(0x00000080)
#define ACCESSCTRL_WATCHDOG_DBG_MSB    _u(7)
#define ACCESSCTRL_WATCHDOG_DBG_LSB    _u(7)
#define ACCESSCTRL_WATCHDOG_DBG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_WATCHDOG_DMA
// Description : If 1, WATCHDOG can be accessed by the DMA, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_WATCHDOG_DMA_RESET  _u(0x0)
#define ACCESSCTRL_WATCHDOG_DMA_BITS   _u(0x00000040)
#define ACCESSCTRL_WATCHDOG_DMA_MSB    _u(6)
#define ACCESSCTRL_WATCHDOG_DMA_LSB    _u(6)
#define ACCESSCTRL_WATCHDOG_DMA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_WATCHDOG_CORE1
// Description : If 1, WATCHDOG can be accessed by core 1, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_WATCHDOG_CORE1_RESET  _u(0x1)
#define ACCESSCTRL_WATCHDOG_CORE1_BITS   _u(0x00000020)
#define ACCESSCTRL_WATCHDOG_CORE1_MSB    _u(5)
#define ACCESSCTRL_WATCHDOG_CORE1_LSB    _u(5)
#define ACCESSCTRL_WATCHDOG_CORE1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_WATCHDOG_CORE0
// Description : If 1, WATCHDOG can be accessed by core 0, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_WATCHDOG_CORE0_RESET  _u(0x1)
#define ACCESSCTRL_WATCHDOG_CORE0_BITS   _u(0x00000010)
#define ACCESSCTRL_WATCHDOG_CORE0_MSB    _u(4)
#define ACCESSCTRL_WATCHDOG_CORE0_LSB    _u(4)
#define ACCESSCTRL_WATCHDOG_CORE0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_WATCHDOG_SP
// Description : If 1, WATCHDOG can be accessed from a Secure, Privileged
//               context.
#define ACCESSCTRL_WATCHDOG_SP_RESET  _u(0x1)
#define ACCESSCTRL_WATCHDOG_SP_BITS   _u(0x00000008)
#define ACCESSCTRL_WATCHDOG_SP_MSB    _u(3)
#define ACCESSCTRL_WATCHDOG_SP_LSB    _u(3)
#define ACCESSCTRL_WATCHDOG_SP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_WATCHDOG_SU
// Description : If 1, and SP is also set, WATCHDOG can be accessed from a
//               Secure, Unprivileged context.
#define ACCESSCTRL_WATCHDOG_SU_RESET  _u(0x0)
#define ACCESSCTRL_WATCHDOG_SU_BITS   _u(0x00000004)
#define ACCESSCTRL_WATCHDOG_SU_MSB    _u(2)
#define ACCESSCTRL_WATCHDOG_SU_LSB    _u(2)
#define ACCESSCTRL_WATCHDOG_SU_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_WATCHDOG_NSP
// Description : If 1, WATCHDOG can be accessed from a Non-secure, Privileged
//               context.
#define ACCESSCTRL_WATCHDOG_NSP_RESET  _u(0x0)
#define ACCESSCTRL_WATCHDOG_NSP_BITS   _u(0x00000002)
#define ACCESSCTRL_WATCHDOG_NSP_MSB    _u(1)
#define ACCESSCTRL_WATCHDOG_NSP_LSB    _u(1)
#define ACCESSCTRL_WATCHDOG_NSP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_WATCHDOG_NSU
// Description : If 1, and NSP is also set, WATCHDOG can be accessed from a Non-
//               secure, Unprivileged context.
//
//               This bit is writable from a Non-secure, Privileged context, if
//               and only if the NSP bit is set.
#define ACCESSCTRL_WATCHDOG_NSU_RESET  _u(0x0)
#define ACCESSCTRL_WATCHDOG_NSU_BITS   _u(0x00000001)
#define ACCESSCTRL_WATCHDOG_NSU_MSB    _u(0)
#define ACCESSCTRL_WATCHDOG_NSU_LSB    _u(0)
#define ACCESSCTRL_WATCHDOG_NSU_ACCESS "RW"
// =============================================================================
// Register    : ACCESSCTRL_RSM
// Description : Control whether debugger, DMA, core 0 and core 1 can access
//               RSM, and at what security/privilege levels they can do so.
//
//               Defaults to Secure, Privileged processor or debug access only.
//
//               This register is writable only from a Secure, Privileged
//               processor or debugger, with the exception of the NSU bit, which
//               becomes Non-secure-Privileged-writable when the NSP bit is set.
#define ACCESSCTRL_RSM_OFFSET _u(0x000000dc)
#define ACCESSCTRL_RSM_BITS   _u(0x000000ff)
#define ACCESSCTRL_RSM_RESET  _u(0x000000b8)
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_RSM_DBG
// Description : If 1, RSM can be accessed by the debugger, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_RSM_DBG_RESET  _u(0x1)
#define ACCESSCTRL_RSM_DBG_BITS   _u(0x00000080)
#define ACCESSCTRL_RSM_DBG_MSB    _u(7)
#define ACCESSCTRL_RSM_DBG_LSB    _u(7)
#define ACCESSCTRL_RSM_DBG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_RSM_DMA
// Description : If 1, RSM can be accessed by the DMA, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_RSM_DMA_RESET  _u(0x0)
#define ACCESSCTRL_RSM_DMA_BITS   _u(0x00000040)
#define ACCESSCTRL_RSM_DMA_MSB    _u(6)
#define ACCESSCTRL_RSM_DMA_LSB    _u(6)
#define ACCESSCTRL_RSM_DMA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_RSM_CORE1
// Description : If 1, RSM can be accessed by core 1, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_RSM_CORE1_RESET  _u(0x1)
#define ACCESSCTRL_RSM_CORE1_BITS   _u(0x00000020)
#define ACCESSCTRL_RSM_CORE1_MSB    _u(5)
#define ACCESSCTRL_RSM_CORE1_LSB    _u(5)
#define ACCESSCTRL_RSM_CORE1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_RSM_CORE0
// Description : If 1, RSM can be accessed by core 0, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_RSM_CORE0_RESET  _u(0x1)
#define ACCESSCTRL_RSM_CORE0_BITS   _u(0x00000010)
#define ACCESSCTRL_RSM_CORE0_MSB    _u(4)
#define ACCESSCTRL_RSM_CORE0_LSB    _u(4)
#define ACCESSCTRL_RSM_CORE0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_RSM_SP
// Description : If 1, RSM can be accessed from a Secure, Privileged context.
#define ACCESSCTRL_RSM_SP_RESET  _u(0x1)
#define ACCESSCTRL_RSM_SP_BITS   _u(0x00000008)
#define ACCESSCTRL_RSM_SP_MSB    _u(3)
#define ACCESSCTRL_RSM_SP_LSB    _u(3)
#define ACCESSCTRL_RSM_SP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_RSM_SU
// Description : If 1, and SP is also set, RSM can be accessed from a Secure,
//               Unprivileged context.
#define ACCESSCTRL_RSM_SU_RESET  _u(0x0)
#define ACCESSCTRL_RSM_SU_BITS   _u(0x00000004)
#define ACCESSCTRL_RSM_SU_MSB    _u(2)
#define ACCESSCTRL_RSM_SU_LSB    _u(2)
#define ACCESSCTRL_RSM_SU_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_RSM_NSP
// Description : If 1, RSM can be accessed from a Non-secure, Privileged
//               context.
#define ACCESSCTRL_RSM_NSP_RESET  _u(0x0)
#define ACCESSCTRL_RSM_NSP_BITS   _u(0x00000002)
#define ACCESSCTRL_RSM_NSP_MSB    _u(1)
#define ACCESSCTRL_RSM_NSP_LSB    _u(1)
#define ACCESSCTRL_RSM_NSP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_RSM_NSU
// Description : If 1, and NSP is also set, RSM can be accessed from a Non-
//               secure, Unprivileged context.
//
//               This bit is writable from a Non-secure, Privileged context, if
//               and only if the NSP bit is set.
#define ACCESSCTRL_RSM_NSU_RESET  _u(0x0)
#define ACCESSCTRL_RSM_NSU_BITS   _u(0x00000001)
#define ACCESSCTRL_RSM_NSU_MSB    _u(0)
#define ACCESSCTRL_RSM_NSU_LSB    _u(0)
#define ACCESSCTRL_RSM_NSU_ACCESS "RW"
// =============================================================================
// Register    : ACCESSCTRL_XIP_CTRL
// Description : Control whether debugger, DMA, core 0 and core 1 can access
//               XIP_CTRL, and at what security/privilege levels they can do so.
//
//               Defaults to Secure, Privileged processor or debug access only.
//
//               This register is writable only from a Secure, Privileged
//               processor or debugger, with the exception of the NSU bit, which
//               becomes Non-secure-Privileged-writable when the NSP bit is set.
#define ACCESSCTRL_XIP_CTRL_OFFSET _u(0x000000e0)
#define ACCESSCTRL_XIP_CTRL_BITS   _u(0x000000ff)
#define ACCESSCTRL_XIP_CTRL_RESET  _u(0x000000b8)
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_XIP_CTRL_DBG
// Description : If 1, XIP_CTRL can be accessed by the debugger, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_XIP_CTRL_DBG_RESET  _u(0x1)
#define ACCESSCTRL_XIP_CTRL_DBG_BITS   _u(0x00000080)
#define ACCESSCTRL_XIP_CTRL_DBG_MSB    _u(7)
#define ACCESSCTRL_XIP_CTRL_DBG_LSB    _u(7)
#define ACCESSCTRL_XIP_CTRL_DBG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_XIP_CTRL_DMA
// Description : If 1, XIP_CTRL can be accessed by the DMA, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_XIP_CTRL_DMA_RESET  _u(0x0)
#define ACCESSCTRL_XIP_CTRL_DMA_BITS   _u(0x00000040)
#define ACCESSCTRL_XIP_CTRL_DMA_MSB    _u(6)
#define ACCESSCTRL_XIP_CTRL_DMA_LSB    _u(6)
#define ACCESSCTRL_XIP_CTRL_DMA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_XIP_CTRL_CORE1
// Description : If 1, XIP_CTRL can be accessed by core 1, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_XIP_CTRL_CORE1_RESET  _u(0x1)
#define ACCESSCTRL_XIP_CTRL_CORE1_BITS   _u(0x00000020)
#define ACCESSCTRL_XIP_CTRL_CORE1_MSB    _u(5)
#define ACCESSCTRL_XIP_CTRL_CORE1_LSB    _u(5)
#define ACCESSCTRL_XIP_CTRL_CORE1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_XIP_CTRL_CORE0
// Description : If 1, XIP_CTRL can be accessed by core 0, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_XIP_CTRL_CORE0_RESET  _u(0x1)
#define ACCESSCTRL_XIP_CTRL_CORE0_BITS   _u(0x00000010)
#define ACCESSCTRL_XIP_CTRL_CORE0_MSB    _u(4)
#define ACCESSCTRL_XIP_CTRL_CORE0_LSB    _u(4)
#define ACCESSCTRL_XIP_CTRL_CORE0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_XIP_CTRL_SP
// Description : If 1, XIP_CTRL can be accessed from a Secure, Privileged
//               context.
#define ACCESSCTRL_XIP_CTRL_SP_RESET  _u(0x1)
#define ACCESSCTRL_XIP_CTRL_SP_BITS   _u(0x00000008)
#define ACCESSCTRL_XIP_CTRL_SP_MSB    _u(3)
#define ACCESSCTRL_XIP_CTRL_SP_LSB    _u(3)
#define ACCESSCTRL_XIP_CTRL_SP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_XIP_CTRL_SU
// Description : If 1, and SP is also set, XIP_CTRL can be accessed from a
//               Secure, Unprivileged context.
#define ACCESSCTRL_XIP_CTRL_SU_RESET  _u(0x0)
#define ACCESSCTRL_XIP_CTRL_SU_BITS   _u(0x00000004)
#define ACCESSCTRL_XIP_CTRL_SU_MSB    _u(2)
#define ACCESSCTRL_XIP_CTRL_SU_LSB    _u(2)
#define ACCESSCTRL_XIP_CTRL_SU_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_XIP_CTRL_NSP
// Description : If 1, XIP_CTRL can be accessed from a Non-secure, Privileged
//               context.
#define ACCESSCTRL_XIP_CTRL_NSP_RESET  _u(0x0)
#define ACCESSCTRL_XIP_CTRL_NSP_BITS   _u(0x00000002)
#define ACCESSCTRL_XIP_CTRL_NSP_MSB    _u(1)
#define ACCESSCTRL_XIP_CTRL_NSP_LSB    _u(1)
#define ACCESSCTRL_XIP_CTRL_NSP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_XIP_CTRL_NSU
// Description : If 1, and NSP is also set, XIP_CTRL can be accessed from a Non-
//               secure, Unprivileged context.
//
//               This bit is writable from a Non-secure, Privileged context, if
//               and only if the NSP bit is set.
#define ACCESSCTRL_XIP_CTRL_NSU_RESET  _u(0x0)
#define ACCESSCTRL_XIP_CTRL_NSU_BITS   _u(0x00000001)
#define ACCESSCTRL_XIP_CTRL_NSU_MSB    _u(0)
#define ACCESSCTRL_XIP_CTRL_NSU_LSB    _u(0)
#define ACCESSCTRL_XIP_CTRL_NSU_ACCESS "RW"
// =============================================================================
// Register    : ACCESSCTRL_XIP_QMI
// Description : Control whether debugger, DMA, core 0 and core 1 can access
//               XIP_QMI, and at what security/privilege levels they can do so.
//
//               Defaults to Secure, Privileged processor or debug access only.
//
//               This register is writable only from a Secure, Privileged
//               processor or debugger, with the exception of the NSU bit, which
//               becomes Non-secure-Privileged-writable when the NSP bit is set.
#define ACCESSCTRL_XIP_QMI_OFFSET _u(0x000000e4)
#define ACCESSCTRL_XIP_QMI_BITS   _u(0x000000ff)
#define ACCESSCTRL_XIP_QMI_RESET  _u(0x000000b8)
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_XIP_QMI_DBG
// Description : If 1, XIP_QMI can be accessed by the debugger, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_XIP_QMI_DBG_RESET  _u(0x1)
#define ACCESSCTRL_XIP_QMI_DBG_BITS   _u(0x00000080)
#define ACCESSCTRL_XIP_QMI_DBG_MSB    _u(7)
#define ACCESSCTRL_XIP_QMI_DBG_LSB    _u(7)
#define ACCESSCTRL_XIP_QMI_DBG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_XIP_QMI_DMA
// Description : If 1, XIP_QMI can be accessed by the DMA, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_XIP_QMI_DMA_RESET  _u(0x0)
#define ACCESSCTRL_XIP_QMI_DMA_BITS   _u(0x00000040)
#define ACCESSCTRL_XIP_QMI_DMA_MSB    _u(6)
#define ACCESSCTRL_XIP_QMI_DMA_LSB    _u(6)
#define ACCESSCTRL_XIP_QMI_DMA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_XIP_QMI_CORE1
// Description : If 1, XIP_QMI can be accessed by core 1, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_XIP_QMI_CORE1_RESET  _u(0x1)
#define ACCESSCTRL_XIP_QMI_CORE1_BITS   _u(0x00000020)
#define ACCESSCTRL_XIP_QMI_CORE1_MSB    _u(5)
#define ACCESSCTRL_XIP_QMI_CORE1_LSB    _u(5)
#define ACCESSCTRL_XIP_QMI_CORE1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_XIP_QMI_CORE0
// Description : If 1, XIP_QMI can be accessed by core 0, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_XIP_QMI_CORE0_RESET  _u(0x1)
#define ACCESSCTRL_XIP_QMI_CORE0_BITS   _u(0x00000010)
#define ACCESSCTRL_XIP_QMI_CORE0_MSB    _u(4)
#define ACCESSCTRL_XIP_QMI_CORE0_LSB    _u(4)
#define ACCESSCTRL_XIP_QMI_CORE0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_XIP_QMI_SP
// Description : If 1, XIP_QMI can be accessed from a Secure, Privileged
//               context.
#define ACCESSCTRL_XIP_QMI_SP_RESET  _u(0x1)
#define ACCESSCTRL_XIP_QMI_SP_BITS   _u(0x00000008)
#define ACCESSCTRL_XIP_QMI_SP_MSB    _u(3)
#define ACCESSCTRL_XIP_QMI_SP_LSB    _u(3)
#define ACCESSCTRL_XIP_QMI_SP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_XIP_QMI_SU
// Description : If 1, and SP is also set, XIP_QMI can be accessed from a
//               Secure, Unprivileged context.
#define ACCESSCTRL_XIP_QMI_SU_RESET  _u(0x0)
#define ACCESSCTRL_XIP_QMI_SU_BITS   _u(0x00000004)
#define ACCESSCTRL_XIP_QMI_SU_MSB    _u(2)
#define ACCESSCTRL_XIP_QMI_SU_LSB    _u(2)
#define ACCESSCTRL_XIP_QMI_SU_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_XIP_QMI_NSP
// Description : If 1, XIP_QMI can be accessed from a Non-secure, Privileged
//               context.
#define ACCESSCTRL_XIP_QMI_NSP_RESET  _u(0x0)
#define ACCESSCTRL_XIP_QMI_NSP_BITS   _u(0x00000002)
#define ACCESSCTRL_XIP_QMI_NSP_MSB    _u(1)
#define ACCESSCTRL_XIP_QMI_NSP_LSB    _u(1)
#define ACCESSCTRL_XIP_QMI_NSP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_XIP_QMI_NSU
// Description : If 1, and NSP is also set, XIP_QMI can be accessed from a Non-
//               secure, Unprivileged context.
//
//               This bit is writable from a Non-secure, Privileged context, if
//               and only if the NSP bit is set.
#define ACCESSCTRL_XIP_QMI_NSU_RESET  _u(0x0)
#define ACCESSCTRL_XIP_QMI_NSU_BITS   _u(0x00000001)
#define ACCESSCTRL_XIP_QMI_NSU_MSB    _u(0)
#define ACCESSCTRL_XIP_QMI_NSU_LSB    _u(0)
#define ACCESSCTRL_XIP_QMI_NSU_ACCESS "RW"
// =============================================================================
// Register    : ACCESSCTRL_XIP_AUX
// Description : Control whether debugger, DMA, core 0 and core 1 can access
//               XIP_AUX, and at what security/privilege levels they can do so.
//
//               Defaults to Secure, Privileged access only.
//
//               This register is writable only from a Secure, Privileged
//               processor or debugger, with the exception of the NSU bit, which
//               becomes Non-secure-Privileged-writable when the NSP bit is set.
#define ACCESSCTRL_XIP_AUX_OFFSET _u(0x000000e8)
#define ACCESSCTRL_XIP_AUX_BITS   _u(0x000000ff)
#define ACCESSCTRL_XIP_AUX_RESET  _u(0x000000f8)
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_XIP_AUX_DBG
// Description : If 1, XIP_AUX can be accessed by the debugger, at
//               security/privilege levels permitted by SP/NSP/SU/NSU in this
//               register.
#define ACCESSCTRL_XIP_AUX_DBG_RESET  _u(0x1)
#define ACCESSCTRL_XIP_AUX_DBG_BITS   _u(0x00000080)
#define ACCESSCTRL_XIP_AUX_DBG_MSB    _u(7)
#define ACCESSCTRL_XIP_AUX_DBG_LSB    _u(7)
#define ACCESSCTRL_XIP_AUX_DBG_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_XIP_AUX_DMA
// Description : If 1, XIP_AUX can be accessed by the DMA, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_XIP_AUX_DMA_RESET  _u(0x1)
#define ACCESSCTRL_XIP_AUX_DMA_BITS   _u(0x00000040)
#define ACCESSCTRL_XIP_AUX_DMA_MSB    _u(6)
#define ACCESSCTRL_XIP_AUX_DMA_LSB    _u(6)
#define ACCESSCTRL_XIP_AUX_DMA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_XIP_AUX_CORE1
// Description : If 1, XIP_AUX can be accessed by core 1, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_XIP_AUX_CORE1_RESET  _u(0x1)
#define ACCESSCTRL_XIP_AUX_CORE1_BITS   _u(0x00000020)
#define ACCESSCTRL_XIP_AUX_CORE1_MSB    _u(5)
#define ACCESSCTRL_XIP_AUX_CORE1_LSB    _u(5)
#define ACCESSCTRL_XIP_AUX_CORE1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_XIP_AUX_CORE0
// Description : If 1, XIP_AUX can be accessed by core 0, at security/privilege
//               levels permitted by SP/NSP/SU/NSU in this register.
#define ACCESSCTRL_XIP_AUX_CORE0_RESET  _u(0x1)
#define ACCESSCTRL_XIP_AUX_CORE0_BITS   _u(0x00000010)
#define ACCESSCTRL_XIP_AUX_CORE0_MSB    _u(4)
#define ACCESSCTRL_XIP_AUX_CORE0_LSB    _u(4)
#define ACCESSCTRL_XIP_AUX_CORE0_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_XIP_AUX_SP
// Description : If 1, XIP_AUX can be accessed from a Secure, Privileged
//               context.
#define ACCESSCTRL_XIP_AUX_SP_RESET  _u(0x1)
#define ACCESSCTRL_XIP_AUX_SP_BITS   _u(0x00000008)
#define ACCESSCTRL_XIP_AUX_SP_MSB    _u(3)
#define ACCESSCTRL_XIP_AUX_SP_LSB    _u(3)
#define ACCESSCTRL_XIP_AUX_SP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_XIP_AUX_SU
// Description : If 1, and SP is also set, XIP_AUX can be accessed from a
//               Secure, Unprivileged context.
#define ACCESSCTRL_XIP_AUX_SU_RESET  _u(0x0)
#define ACCESSCTRL_XIP_AUX_SU_BITS   _u(0x00000004)
#define ACCESSCTRL_XIP_AUX_SU_MSB    _u(2)
#define ACCESSCTRL_XIP_AUX_SU_LSB    _u(2)
#define ACCESSCTRL_XIP_AUX_SU_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_XIP_AUX_NSP
// Description : If 1, XIP_AUX can be accessed from a Non-secure, Privileged
//               context.
#define ACCESSCTRL_XIP_AUX_NSP_RESET  _u(0x0)
#define ACCESSCTRL_XIP_AUX_NSP_BITS   _u(0x00000002)
#define ACCESSCTRL_XIP_AUX_NSP_MSB    _u(1)
#define ACCESSCTRL_XIP_AUX_NSP_LSB    _u(1)
#define ACCESSCTRL_XIP_AUX_NSP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : ACCESSCTRL_XIP_AUX_NSU
// Description : If 1, and NSP is also set, XIP_AUX can be accessed from a Non-
//               secure, Unprivileged context.
//
//               This bit is writable from a Non-secure, Privileged context, if
//               and only if the NSP bit is set.
#define ACCESSCTRL_XIP_AUX_NSU_RESET  _u(0x0)
#define ACCESSCTRL_XIP_AUX_NSU_BITS   _u(0x00000001)
#define ACCESSCTRL_XIP_AUX_NSU_MSB    _u(0)
#define ACCESSCTRL_XIP_AUX_NSU_LSB    _u(0)
#define ACCESSCTRL_XIP_AUX_NSU_ACCESS "RW"
// =============================================================================
#endif // _HARDWARE_REGS_ACCESSCTRL_H

