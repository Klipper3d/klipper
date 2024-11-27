// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
// =============================================================================
// Register block : RVCSR
// Version        : 1
// Bus type       : apb
// Description    : CSR listing for Hazard3
// =============================================================================
#ifndef _HARDWARE_REGS_RVCSR_H
#define _HARDWARE_REGS_RVCSR_H
// =============================================================================
// Register    : RVCSR_MSTATUS
// Description : Machine status register
#define RVCSR_MSTATUS_OFFSET _u(0x00000300)
#define RVCSR_MSTATUS_BITS   _u(0x00221888)
#define RVCSR_MSTATUS_RESET  _u(0x00001800)
// -----------------------------------------------------------------------------
// Field       : RVCSR_MSTATUS_TW
// Description : Timeout wait. When 1, attempting to execute a WFI instruction
//               in U-mode will instantly cause an illegal instruction
//               exception.
#define RVCSR_MSTATUS_TW_RESET  _u(0x0)
#define RVCSR_MSTATUS_TW_BITS   _u(0x00200000)
#define RVCSR_MSTATUS_TW_MSB    _u(21)
#define RVCSR_MSTATUS_TW_LSB    _u(21)
#define RVCSR_MSTATUS_TW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_MSTATUS_MPRV
// Description : Modify privilege. If 1, loads and stores behave as though the
//               current privilege level were `mpp`. This includes physical
//               memory protection checks, and the privilege level asserted on
//               the system bus alongside the load/store address.
#define RVCSR_MSTATUS_MPRV_RESET  _u(0x0)
#define RVCSR_MSTATUS_MPRV_BITS   _u(0x00020000)
#define RVCSR_MSTATUS_MPRV_MSB    _u(17)
#define RVCSR_MSTATUS_MPRV_LSB    _u(17)
#define RVCSR_MSTATUS_MPRV_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_MSTATUS_MPP
// Description : Previous privilege level. Can store the values 3 (M-mode) or 0
//               (U-mode). If another value is written, hardware rounds to the
//               nearest supported mode.
#define RVCSR_MSTATUS_MPP_RESET  _u(0x3)
#define RVCSR_MSTATUS_MPP_BITS   _u(0x00001800)
#define RVCSR_MSTATUS_MPP_MSB    _u(12)
#define RVCSR_MSTATUS_MPP_LSB    _u(11)
#define RVCSR_MSTATUS_MPP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_MSTATUS_MPIE
// Description : Previous interrupt enable. Readable and writable. Is set to the
//               current value of `mstatus.mie` on trap entry. Is set to 1 on
//               trap return.
#define RVCSR_MSTATUS_MPIE_RESET  _u(0x0)
#define RVCSR_MSTATUS_MPIE_BITS   _u(0x00000080)
#define RVCSR_MSTATUS_MPIE_MSB    _u(7)
#define RVCSR_MSTATUS_MPIE_LSB    _u(7)
#define RVCSR_MSTATUS_MPIE_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_MSTATUS_MIE
// Description : Interrupt enable. Readable and writable. Is set to 0 on trap
//               entry. Is set to the current value of `mstatus.mpie` on trap
//               return.
#define RVCSR_MSTATUS_MIE_RESET  _u(0x0)
#define RVCSR_MSTATUS_MIE_BITS   _u(0x00000008)
#define RVCSR_MSTATUS_MIE_MSB    _u(3)
#define RVCSR_MSTATUS_MIE_LSB    _u(3)
#define RVCSR_MSTATUS_MIE_ACCESS "RW"
// =============================================================================
// Register    : RVCSR_MISA
// Description : Summary of ISA extension support
//
//               On RP2350, Hazard3's full `-march` string is:
//               `rv32ima_zicsr_zifencei_zba_zbb_zbs_zbkb_zca_zcb_zcmp`
//
//               Note Zca is equivalent to the C extension in this case; all
//               instructions from the RISC-V C extension relevant to a 32-bit
//               non-floating-point processor are supported. On older toolchains
//               which do not support the Zc extensions, the appropriate
//               `-march` string is: `rv32imac_zicsr_zifencei_zba_zbb_zbs_zbkb`
//
//               In addition the following custom extensions are configured:
//               Xh3bm, Xh3power, Xh3irq, Xh3pmpm
#define RVCSR_MISA_OFFSET _u(0x00000301)
#define RVCSR_MISA_BITS   _u(0xc0901107)
#define RVCSR_MISA_RESET  _u(0x40901105)
// -----------------------------------------------------------------------------
// Field       : RVCSR_MISA_MXL
// Description : Value of 0x1 indicates this is a 32-bit processor.
#define RVCSR_MISA_MXL_RESET  _u(0x1)
#define RVCSR_MISA_MXL_BITS   _u(0xc0000000)
#define RVCSR_MISA_MXL_MSB    _u(31)
#define RVCSR_MISA_MXL_LSB    _u(30)
#define RVCSR_MISA_MXL_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RVCSR_MISA_X
// Description : Value of 1 indicates nonstandard extensions are present. (Xh3b
//               bit manipulation, and custom sleep and interrupt control CSRs)
#define RVCSR_MISA_X_RESET  _u(0x1)
#define RVCSR_MISA_X_BITS   _u(0x00800000)
#define RVCSR_MISA_X_MSB    _u(23)
#define RVCSR_MISA_X_LSB    _u(23)
#define RVCSR_MISA_X_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RVCSR_MISA_U
// Description : Value of 1 indicates U-mode is implemented.
#define RVCSR_MISA_U_RESET  _u(0x1)
#define RVCSR_MISA_U_BITS   _u(0x00100000)
#define RVCSR_MISA_U_MSB    _u(20)
#define RVCSR_MISA_U_LSB    _u(20)
#define RVCSR_MISA_U_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RVCSR_MISA_M
// Description : Value of 1 indicates the M extension (integer multiply/divide)
//               is implemented.
#define RVCSR_MISA_M_RESET  _u(0x1)
#define RVCSR_MISA_M_BITS   _u(0x00001000)
#define RVCSR_MISA_M_MSB    _u(12)
#define RVCSR_MISA_M_LSB    _u(12)
#define RVCSR_MISA_M_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RVCSR_MISA_I
// Description : Value of 1 indicates the RVI base ISA is implemented (as
//               opposed to RVE)
#define RVCSR_MISA_I_RESET  _u(0x1)
#define RVCSR_MISA_I_BITS   _u(0x00000100)
#define RVCSR_MISA_I_MSB    _u(8)
#define RVCSR_MISA_I_LSB    _u(8)
#define RVCSR_MISA_I_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RVCSR_MISA_C
// Description : Value of 1 indicates the C extension (compressed instructions)
//               is implemented.
#define RVCSR_MISA_C_RESET  _u(0x1)
#define RVCSR_MISA_C_BITS   _u(0x00000004)
#define RVCSR_MISA_C_MSB    _u(2)
#define RVCSR_MISA_C_LSB    _u(2)
#define RVCSR_MISA_C_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RVCSR_MISA_B
// Description : Value of 1 indicates the B extension (bit manipulation) is
//               implemented. B is the combination of Zba, Zbb and Zbs.
//
//               Hazard3 implements all of these extensions, but the definition
//               of B as ZbaZbbZbs did not exist at the point this version of
//               Hazard3 was taped out. This bit was reserved-0 at that point.
//               Therefore this bit reads as 0.
#define RVCSR_MISA_B_RESET  _u(0x0)
#define RVCSR_MISA_B_BITS   _u(0x00000002)
#define RVCSR_MISA_B_MSB    _u(1)
#define RVCSR_MISA_B_LSB    _u(1)
#define RVCSR_MISA_B_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RVCSR_MISA_A
// Description : Value of 1 indicates the A extension (atomics) is implemented.
#define RVCSR_MISA_A_RESET  _u(0x1)
#define RVCSR_MISA_A_BITS   _u(0x00000001)
#define RVCSR_MISA_A_MSB    _u(0)
#define RVCSR_MISA_A_LSB    _u(0)
#define RVCSR_MISA_A_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MEDELEG
// Description : Machine exception delegation register. Not implemented, as no
//               S-mode support.
#define RVCSR_MEDELEG_OFFSET _u(0x00000302)
#define RVCSR_MEDELEG_BITS   _u(0xffffffff)
#define RVCSR_MEDELEG_RESET  "-"
#define RVCSR_MEDELEG_MSB    _u(31)
#define RVCSR_MEDELEG_LSB    _u(0)
#define RVCSR_MEDELEG_ACCESS "RW"
// =============================================================================
// Register    : RVCSR_MIDELEG
// Description : Machine interrupt delegation register. Not implemented, as no
//               S-mode support.
#define RVCSR_MIDELEG_OFFSET _u(0x00000303)
#define RVCSR_MIDELEG_BITS   _u(0xffffffff)
#define RVCSR_MIDELEG_RESET  "-"
#define RVCSR_MIDELEG_MSB    _u(31)
#define RVCSR_MIDELEG_LSB    _u(0)
#define RVCSR_MIDELEG_ACCESS "RW"
// =============================================================================
// Register    : RVCSR_MIE
// Description : Machine interrupt enable register
#define RVCSR_MIE_OFFSET _u(0x00000304)
#define RVCSR_MIE_BITS   _u(0x00000888)
#define RVCSR_MIE_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : RVCSR_MIE_MEIE
// Description : External interrupt enable. The processor transfers to the
//               external interrupt vector when `mie.meie`, `mip.meip` and
//               `mstatus.mie` are all 1.
//
//               Hazard3 has internal registers to individually filter external
//               interrupts (see `meiea`), but this standard control can be used
//               to mask all external interrupts at once.
#define RVCSR_MIE_MEIE_RESET  _u(0x0)
#define RVCSR_MIE_MEIE_BITS   _u(0x00000800)
#define RVCSR_MIE_MEIE_MSB    _u(11)
#define RVCSR_MIE_MEIE_LSB    _u(11)
#define RVCSR_MIE_MEIE_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_MIE_MTIE
// Description : Timer interrupt enable. The processor transfers to the timer
//               interrupt vector when `mie.mtie`, `mip.mtip` and `mstatus.mie`
//               are all 1, unless a software or external interrupt request is
//               also valid at this time.
#define RVCSR_MIE_MTIE_RESET  _u(0x0)
#define RVCSR_MIE_MTIE_BITS   _u(0x00000080)
#define RVCSR_MIE_MTIE_MSB    _u(7)
#define RVCSR_MIE_MTIE_LSB    _u(7)
#define RVCSR_MIE_MTIE_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_MIE_MSIE
// Description : Software interrupt enable. The processor transfers to the
//               software interrupt vector `mie.msie`, `mip.msip` and
//               `mstatus.mie` are all 1, unless an external interrupt request
//               is also valid at this time.
#define RVCSR_MIE_MSIE_RESET  _u(0x0)
#define RVCSR_MIE_MSIE_BITS   _u(0x00000008)
#define RVCSR_MIE_MSIE_MSB    _u(3)
#define RVCSR_MIE_MSIE_LSB    _u(3)
#define RVCSR_MIE_MSIE_ACCESS "RW"
// =============================================================================
// Register    : RVCSR_MTVEC
// Description : Machine trap handler base address.
#define RVCSR_MTVEC_OFFSET _u(0x00000305)
#define RVCSR_MTVEC_BITS   _u(0xffffffff)
#define RVCSR_MTVEC_RESET  _u(0x00007ffc)
// -----------------------------------------------------------------------------
// Field       : RVCSR_MTVEC_BASE
// Description : The upper 30 bits of the trap vector address (2 LSBs are
//               implicitly 0). Must be 64-byte-aligned if vectoring is enabled.
//               Otherwise, must be 4-byte-aligned.
#define RVCSR_MTVEC_BASE_RESET  _u(0x00001fff)
#define RVCSR_MTVEC_BASE_BITS   _u(0xfffffffc)
#define RVCSR_MTVEC_BASE_MSB    _u(31)
#define RVCSR_MTVEC_BASE_LSB    _u(2)
#define RVCSR_MTVEC_BASE_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_MTVEC_MODE
// Description : If 0 (direct mode), all traps set pc to the trap vector base.
//               If 1 (vectored), exceptions set pc to the trap vector base, and
//               interrupts set pc to 4 times the interrupt cause (3=soft IRQ,
//               7=timer IRQ, 11=external IRQ).
//
//               The upper bit is hardwired to zero, so attempting to set mode
//               to 2 or 3 will result in a value of 0 or 1 respectively.
//               0x0 -> Direct entry to mtvec
//               0x1 -> Vectored entry to a 16-entry jump table starting at mtvec
#define RVCSR_MTVEC_MODE_RESET  _u(0x0)
#define RVCSR_MTVEC_MODE_BITS   _u(0x00000003)
#define RVCSR_MTVEC_MODE_MSB    _u(1)
#define RVCSR_MTVEC_MODE_LSB    _u(0)
#define RVCSR_MTVEC_MODE_ACCESS "RW"
#define RVCSR_MTVEC_MODE_VALUE_DIRECT _u(0x0)
#define RVCSR_MTVEC_MODE_VALUE_VECTORED _u(0x1)
// =============================================================================
// Register    : RVCSR_MCOUNTEREN
// Description : Counter enable. Control access to counters from U-mode. Not to
//               be confused with mcountinhibit.
#define RVCSR_MCOUNTEREN_OFFSET _u(0x00000306)
#define RVCSR_MCOUNTEREN_BITS   _u(0x00000007)
#define RVCSR_MCOUNTEREN_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : RVCSR_MCOUNTEREN_IR
// Description : If 1, U-mode is permitted to access the `instret`/`instreth`
//               instruction retire counter CSRs. Otherwise, U-mode accesses to
//               these CSRs will trap.
#define RVCSR_MCOUNTEREN_IR_RESET  _u(0x0)
#define RVCSR_MCOUNTEREN_IR_BITS   _u(0x00000004)
#define RVCSR_MCOUNTEREN_IR_MSB    _u(2)
#define RVCSR_MCOUNTEREN_IR_LSB    _u(2)
#define RVCSR_MCOUNTEREN_IR_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_MCOUNTEREN_TM
// Description : No hardware effect, as the `time`/`timeh` CSRs are not
//               implemented. However, this field still exists, as M-mode
//               software can use it to track whether it should emulate U-mode
//               attempts to access those CSRs.
#define RVCSR_MCOUNTEREN_TM_RESET  _u(0x0)
#define RVCSR_MCOUNTEREN_TM_BITS   _u(0x00000002)
#define RVCSR_MCOUNTEREN_TM_MSB    _u(1)
#define RVCSR_MCOUNTEREN_TM_LSB    _u(1)
#define RVCSR_MCOUNTEREN_TM_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_MCOUNTEREN_CY
// Description : If 1, U-mode is permitted to access the `cycle`/`cycleh` cycle
//               counter CSRs. Otherwise, U-mode accesses to these CSRs will
//               trap.
#define RVCSR_MCOUNTEREN_CY_RESET  _u(0x0)
#define RVCSR_MCOUNTEREN_CY_BITS   _u(0x00000001)
#define RVCSR_MCOUNTEREN_CY_MSB    _u(0)
#define RVCSR_MCOUNTEREN_CY_LSB    _u(0)
#define RVCSR_MCOUNTEREN_CY_ACCESS "RW"
// =============================================================================
// Register    : RVCSR_MENVCFG
// Description : Machine environment configuration register, low half
#define RVCSR_MENVCFG_OFFSET _u(0x0000030a)
#define RVCSR_MENVCFG_BITS   _u(0x00000001)
#define RVCSR_MENVCFG_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : RVCSR_MENVCFG_FIOM
// Description : When set, fence instructions in modes less privileged than
//               M-mode which specify that IO memory accesses are ordered will
//               also cause ordering of main memory accesses.
//
//               FIOM is hardwired to zero on Hazard3, because S-mode is not
//               supported, and because fence instructions execute as NOPs (with
//               the exception of `fence.i`)
#define RVCSR_MENVCFG_FIOM_RESET  _u(0x0)
#define RVCSR_MENVCFG_FIOM_BITS   _u(0x00000001)
#define RVCSR_MENVCFG_FIOM_MSB    _u(0)
#define RVCSR_MENVCFG_FIOM_LSB    _u(0)
#define RVCSR_MENVCFG_FIOM_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MSTATUSH
// Description : High half of mstatus, hardwired to 0.
#define RVCSR_MSTATUSH_OFFSET _u(0x00000310)
#define RVCSR_MSTATUSH_BITS   _u(0xffffffff)
#define RVCSR_MSTATUSH_RESET  _u(0x00000000)
#define RVCSR_MSTATUSH_MSB    _u(31)
#define RVCSR_MSTATUSH_LSB    _u(0)
#define RVCSR_MSTATUSH_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MENVCFGH
// Description : Machine environment configuration register, high half
//
//               This register is fully reserved, as Hazard3 does not implement
//               the relevant extensions. It is implemented as hardwired-0.
#define RVCSR_MENVCFGH_OFFSET _u(0x0000031a)
#define RVCSR_MENVCFGH_BITS   _u(0x00000000)
#define RVCSR_MENVCFGH_RESET  _u(0x00000000)
#define RVCSR_MENVCFGH_MSB    _u(31)
#define RVCSR_MENVCFGH_LSB    _u(0)
#define RVCSR_MENVCFGH_ACCESS "RW"
// =============================================================================
// Register    : RVCSR_MCOUNTINHIBIT
// Description : Count inhibit register for `mcycle`/`minstret`
#define RVCSR_MCOUNTINHIBIT_OFFSET _u(0x00000320)
#define RVCSR_MCOUNTINHIBIT_BITS   _u(0x00000005)
#define RVCSR_MCOUNTINHIBIT_RESET  _u(0x00000005)
// -----------------------------------------------------------------------------
// Field       : RVCSR_MCOUNTINHIBIT_IR
// Description : Inhibit counting of the `minstret` and `minstreth` registers.
//               Set by default to save power.
#define RVCSR_MCOUNTINHIBIT_IR_RESET  _u(0x1)
#define RVCSR_MCOUNTINHIBIT_IR_BITS   _u(0x00000004)
#define RVCSR_MCOUNTINHIBIT_IR_MSB    _u(2)
#define RVCSR_MCOUNTINHIBIT_IR_LSB    _u(2)
#define RVCSR_MCOUNTINHIBIT_IR_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_MCOUNTINHIBIT_CY
// Description : Inhibit counting of the `mcycle` and `mcycleh` registers. Set
//               by default to save power.
#define RVCSR_MCOUNTINHIBIT_CY_RESET  _u(0x1)
#define RVCSR_MCOUNTINHIBIT_CY_BITS   _u(0x00000001)
#define RVCSR_MCOUNTINHIBIT_CY_MSB    _u(0)
#define RVCSR_MCOUNTINHIBIT_CY_LSB    _u(0)
#define RVCSR_MCOUNTINHIBIT_CY_ACCESS "RW"
// =============================================================================
// Register    : RVCSR_MHPMEVENT3
// Description : Extended performance event selector, hardwired to 0.
#define RVCSR_MHPMEVENT3_OFFSET _u(0x00000323)
#define RVCSR_MHPMEVENT3_BITS   _u(0xffffffff)
#define RVCSR_MHPMEVENT3_RESET  _u(0x00000000)
#define RVCSR_MHPMEVENT3_MSB    _u(31)
#define RVCSR_MHPMEVENT3_LSB    _u(0)
#define RVCSR_MHPMEVENT3_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMEVENT4
// Description : Extended performance event selector, hardwired to 0.
#define RVCSR_MHPMEVENT4_OFFSET _u(0x00000324)
#define RVCSR_MHPMEVENT4_BITS   _u(0xffffffff)
#define RVCSR_MHPMEVENT4_RESET  _u(0x00000000)
#define RVCSR_MHPMEVENT4_MSB    _u(31)
#define RVCSR_MHPMEVENT4_LSB    _u(0)
#define RVCSR_MHPMEVENT4_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMEVENT5
// Description : Extended performance event selector, hardwired to 0.
#define RVCSR_MHPMEVENT5_OFFSET _u(0x00000325)
#define RVCSR_MHPMEVENT5_BITS   _u(0xffffffff)
#define RVCSR_MHPMEVENT5_RESET  _u(0x00000000)
#define RVCSR_MHPMEVENT5_MSB    _u(31)
#define RVCSR_MHPMEVENT5_LSB    _u(0)
#define RVCSR_MHPMEVENT5_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMEVENT6
// Description : Extended performance event selector, hardwired to 0.
#define RVCSR_MHPMEVENT6_OFFSET _u(0x00000326)
#define RVCSR_MHPMEVENT6_BITS   _u(0xffffffff)
#define RVCSR_MHPMEVENT6_RESET  _u(0x00000000)
#define RVCSR_MHPMEVENT6_MSB    _u(31)
#define RVCSR_MHPMEVENT6_LSB    _u(0)
#define RVCSR_MHPMEVENT6_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMEVENT7
// Description : Extended performance event selector, hardwired to 0.
#define RVCSR_MHPMEVENT7_OFFSET _u(0x00000327)
#define RVCSR_MHPMEVENT7_BITS   _u(0xffffffff)
#define RVCSR_MHPMEVENT7_RESET  _u(0x00000000)
#define RVCSR_MHPMEVENT7_MSB    _u(31)
#define RVCSR_MHPMEVENT7_LSB    _u(0)
#define RVCSR_MHPMEVENT7_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMEVENT8
// Description : Extended performance event selector, hardwired to 0.
#define RVCSR_MHPMEVENT8_OFFSET _u(0x00000328)
#define RVCSR_MHPMEVENT8_BITS   _u(0xffffffff)
#define RVCSR_MHPMEVENT8_RESET  _u(0x00000000)
#define RVCSR_MHPMEVENT8_MSB    _u(31)
#define RVCSR_MHPMEVENT8_LSB    _u(0)
#define RVCSR_MHPMEVENT8_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMEVENT9
// Description : Extended performance event selector, hardwired to 0.
#define RVCSR_MHPMEVENT9_OFFSET _u(0x00000329)
#define RVCSR_MHPMEVENT9_BITS   _u(0xffffffff)
#define RVCSR_MHPMEVENT9_RESET  _u(0x00000000)
#define RVCSR_MHPMEVENT9_MSB    _u(31)
#define RVCSR_MHPMEVENT9_LSB    _u(0)
#define RVCSR_MHPMEVENT9_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMEVENT10
// Description : Extended performance event selector, hardwired to 0.
#define RVCSR_MHPMEVENT10_OFFSET _u(0x0000032a)
#define RVCSR_MHPMEVENT10_BITS   _u(0xffffffff)
#define RVCSR_MHPMEVENT10_RESET  _u(0x00000000)
#define RVCSR_MHPMEVENT10_MSB    _u(31)
#define RVCSR_MHPMEVENT10_LSB    _u(0)
#define RVCSR_MHPMEVENT10_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMEVENT11
// Description : Extended performance event selector, hardwired to 0.
#define RVCSR_MHPMEVENT11_OFFSET _u(0x0000032b)
#define RVCSR_MHPMEVENT11_BITS   _u(0xffffffff)
#define RVCSR_MHPMEVENT11_RESET  _u(0x00000000)
#define RVCSR_MHPMEVENT11_MSB    _u(31)
#define RVCSR_MHPMEVENT11_LSB    _u(0)
#define RVCSR_MHPMEVENT11_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMEVENT12
// Description : Extended performance event selector, hardwired to 0.
#define RVCSR_MHPMEVENT12_OFFSET _u(0x0000032c)
#define RVCSR_MHPMEVENT12_BITS   _u(0xffffffff)
#define RVCSR_MHPMEVENT12_RESET  _u(0x00000000)
#define RVCSR_MHPMEVENT12_MSB    _u(31)
#define RVCSR_MHPMEVENT12_LSB    _u(0)
#define RVCSR_MHPMEVENT12_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMEVENT13
// Description : Extended performance event selector, hardwired to 0.
#define RVCSR_MHPMEVENT13_OFFSET _u(0x0000032d)
#define RVCSR_MHPMEVENT13_BITS   _u(0xffffffff)
#define RVCSR_MHPMEVENT13_RESET  _u(0x00000000)
#define RVCSR_MHPMEVENT13_MSB    _u(31)
#define RVCSR_MHPMEVENT13_LSB    _u(0)
#define RVCSR_MHPMEVENT13_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMEVENT14
// Description : Extended performance event selector, hardwired to 0.
#define RVCSR_MHPMEVENT14_OFFSET _u(0x0000032e)
#define RVCSR_MHPMEVENT14_BITS   _u(0xffffffff)
#define RVCSR_MHPMEVENT14_RESET  _u(0x00000000)
#define RVCSR_MHPMEVENT14_MSB    _u(31)
#define RVCSR_MHPMEVENT14_LSB    _u(0)
#define RVCSR_MHPMEVENT14_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMEVENT15
// Description : Extended performance event selector, hardwired to 0.
#define RVCSR_MHPMEVENT15_OFFSET _u(0x0000032f)
#define RVCSR_MHPMEVENT15_BITS   _u(0xffffffff)
#define RVCSR_MHPMEVENT15_RESET  _u(0x00000000)
#define RVCSR_MHPMEVENT15_MSB    _u(31)
#define RVCSR_MHPMEVENT15_LSB    _u(0)
#define RVCSR_MHPMEVENT15_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMEVENT16
// Description : Extended performance event selector, hardwired to 0.
#define RVCSR_MHPMEVENT16_OFFSET _u(0x00000330)
#define RVCSR_MHPMEVENT16_BITS   _u(0xffffffff)
#define RVCSR_MHPMEVENT16_RESET  _u(0x00000000)
#define RVCSR_MHPMEVENT16_MSB    _u(31)
#define RVCSR_MHPMEVENT16_LSB    _u(0)
#define RVCSR_MHPMEVENT16_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMEVENT17
// Description : Extended performance event selector, hardwired to 0.
#define RVCSR_MHPMEVENT17_OFFSET _u(0x00000331)
#define RVCSR_MHPMEVENT17_BITS   _u(0xffffffff)
#define RVCSR_MHPMEVENT17_RESET  _u(0x00000000)
#define RVCSR_MHPMEVENT17_MSB    _u(31)
#define RVCSR_MHPMEVENT17_LSB    _u(0)
#define RVCSR_MHPMEVENT17_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMEVENT18
// Description : Extended performance event selector, hardwired to 0.
#define RVCSR_MHPMEVENT18_OFFSET _u(0x00000332)
#define RVCSR_MHPMEVENT18_BITS   _u(0xffffffff)
#define RVCSR_MHPMEVENT18_RESET  _u(0x00000000)
#define RVCSR_MHPMEVENT18_MSB    _u(31)
#define RVCSR_MHPMEVENT18_LSB    _u(0)
#define RVCSR_MHPMEVENT18_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMEVENT19
// Description : Extended performance event selector, hardwired to 0.
#define RVCSR_MHPMEVENT19_OFFSET _u(0x00000333)
#define RVCSR_MHPMEVENT19_BITS   _u(0xffffffff)
#define RVCSR_MHPMEVENT19_RESET  _u(0x00000000)
#define RVCSR_MHPMEVENT19_MSB    _u(31)
#define RVCSR_MHPMEVENT19_LSB    _u(0)
#define RVCSR_MHPMEVENT19_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMEVENT20
// Description : Extended performance event selector, hardwired to 0.
#define RVCSR_MHPMEVENT20_OFFSET _u(0x00000334)
#define RVCSR_MHPMEVENT20_BITS   _u(0xffffffff)
#define RVCSR_MHPMEVENT20_RESET  _u(0x00000000)
#define RVCSR_MHPMEVENT20_MSB    _u(31)
#define RVCSR_MHPMEVENT20_LSB    _u(0)
#define RVCSR_MHPMEVENT20_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMEVENT21
// Description : Extended performance event selector, hardwired to 0.
#define RVCSR_MHPMEVENT21_OFFSET _u(0x00000335)
#define RVCSR_MHPMEVENT21_BITS   _u(0xffffffff)
#define RVCSR_MHPMEVENT21_RESET  _u(0x00000000)
#define RVCSR_MHPMEVENT21_MSB    _u(31)
#define RVCSR_MHPMEVENT21_LSB    _u(0)
#define RVCSR_MHPMEVENT21_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMEVENT22
// Description : Extended performance event selector, hardwired to 0.
#define RVCSR_MHPMEVENT22_OFFSET _u(0x00000336)
#define RVCSR_MHPMEVENT22_BITS   _u(0xffffffff)
#define RVCSR_MHPMEVENT22_RESET  _u(0x00000000)
#define RVCSR_MHPMEVENT22_MSB    _u(31)
#define RVCSR_MHPMEVENT22_LSB    _u(0)
#define RVCSR_MHPMEVENT22_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMEVENT23
// Description : Extended performance event selector, hardwired to 0.
#define RVCSR_MHPMEVENT23_OFFSET _u(0x00000337)
#define RVCSR_MHPMEVENT23_BITS   _u(0xffffffff)
#define RVCSR_MHPMEVENT23_RESET  _u(0x00000000)
#define RVCSR_MHPMEVENT23_MSB    _u(31)
#define RVCSR_MHPMEVENT23_LSB    _u(0)
#define RVCSR_MHPMEVENT23_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMEVENT24
// Description : Extended performance event selector, hardwired to 0.
#define RVCSR_MHPMEVENT24_OFFSET _u(0x00000338)
#define RVCSR_MHPMEVENT24_BITS   _u(0xffffffff)
#define RVCSR_MHPMEVENT24_RESET  _u(0x00000000)
#define RVCSR_MHPMEVENT24_MSB    _u(31)
#define RVCSR_MHPMEVENT24_LSB    _u(0)
#define RVCSR_MHPMEVENT24_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMEVENT25
// Description : Extended performance event selector, hardwired to 0.
#define RVCSR_MHPMEVENT25_OFFSET _u(0x00000339)
#define RVCSR_MHPMEVENT25_BITS   _u(0xffffffff)
#define RVCSR_MHPMEVENT25_RESET  _u(0x00000000)
#define RVCSR_MHPMEVENT25_MSB    _u(31)
#define RVCSR_MHPMEVENT25_LSB    _u(0)
#define RVCSR_MHPMEVENT25_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMEVENT26
// Description : Extended performance event selector, hardwired to 0.
#define RVCSR_MHPMEVENT26_OFFSET _u(0x0000033a)
#define RVCSR_MHPMEVENT26_BITS   _u(0xffffffff)
#define RVCSR_MHPMEVENT26_RESET  _u(0x00000000)
#define RVCSR_MHPMEVENT26_MSB    _u(31)
#define RVCSR_MHPMEVENT26_LSB    _u(0)
#define RVCSR_MHPMEVENT26_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMEVENT27
// Description : Extended performance event selector, hardwired to 0.
#define RVCSR_MHPMEVENT27_OFFSET _u(0x0000033b)
#define RVCSR_MHPMEVENT27_BITS   _u(0xffffffff)
#define RVCSR_MHPMEVENT27_RESET  _u(0x00000000)
#define RVCSR_MHPMEVENT27_MSB    _u(31)
#define RVCSR_MHPMEVENT27_LSB    _u(0)
#define RVCSR_MHPMEVENT27_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMEVENT28
// Description : Extended performance event selector, hardwired to 0.
#define RVCSR_MHPMEVENT28_OFFSET _u(0x0000033c)
#define RVCSR_MHPMEVENT28_BITS   _u(0xffffffff)
#define RVCSR_MHPMEVENT28_RESET  _u(0x00000000)
#define RVCSR_MHPMEVENT28_MSB    _u(31)
#define RVCSR_MHPMEVENT28_LSB    _u(0)
#define RVCSR_MHPMEVENT28_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMEVENT29
// Description : Extended performance event selector, hardwired to 0.
#define RVCSR_MHPMEVENT29_OFFSET _u(0x0000033d)
#define RVCSR_MHPMEVENT29_BITS   _u(0xffffffff)
#define RVCSR_MHPMEVENT29_RESET  _u(0x00000000)
#define RVCSR_MHPMEVENT29_MSB    _u(31)
#define RVCSR_MHPMEVENT29_LSB    _u(0)
#define RVCSR_MHPMEVENT29_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMEVENT30
// Description : Extended performance event selector, hardwired to 0.
#define RVCSR_MHPMEVENT30_OFFSET _u(0x0000033e)
#define RVCSR_MHPMEVENT30_BITS   _u(0xffffffff)
#define RVCSR_MHPMEVENT30_RESET  _u(0x00000000)
#define RVCSR_MHPMEVENT30_MSB    _u(31)
#define RVCSR_MHPMEVENT30_LSB    _u(0)
#define RVCSR_MHPMEVENT30_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMEVENT31
// Description : Extended performance event selector, hardwired to 0.
#define RVCSR_MHPMEVENT31_OFFSET _u(0x0000033f)
#define RVCSR_MHPMEVENT31_BITS   _u(0xffffffff)
#define RVCSR_MHPMEVENT31_RESET  _u(0x00000000)
#define RVCSR_MHPMEVENT31_MSB    _u(31)
#define RVCSR_MHPMEVENT31_LSB    _u(0)
#define RVCSR_MHPMEVENT31_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MSCRATCH
// Description : Scratch register for machine trap handlers.
//
//               32-bit read/write register with no specific hardware function.
//               Software may use this to do a fast save/restore of a core
//               register in a trap handler.
#define RVCSR_MSCRATCH_OFFSET _u(0x00000340)
#define RVCSR_MSCRATCH_BITS   _u(0xffffffff)
#define RVCSR_MSCRATCH_RESET  _u(0x00000000)
#define RVCSR_MSCRATCH_MSB    _u(31)
#define RVCSR_MSCRATCH_LSB    _u(0)
#define RVCSR_MSCRATCH_ACCESS "RW"
// =============================================================================
// Register    : RVCSR_MEPC
// Description : Machine exception program counter.
//
//               When entering a trap, the current value of the program counter
//               is recorded here. When executing an `mret`, the processor jumps
//               to `mepc`. Can also be read and written by software.
#define RVCSR_MEPC_OFFSET _u(0x00000341)
#define RVCSR_MEPC_BITS   _u(0xfffffffc)
#define RVCSR_MEPC_RESET  _u(0x00000000)
#define RVCSR_MEPC_MSB    _u(31)
#define RVCSR_MEPC_LSB    _u(2)
#define RVCSR_MEPC_ACCESS "RW"
// =============================================================================
// Register    : RVCSR_MCAUSE
// Description : Machine trap cause. Set when entering a trap to indicate the
//               reason for the trap. Readable and writable by software.
#define RVCSR_MCAUSE_OFFSET _u(0x00000342)
#define RVCSR_MCAUSE_BITS   _u(0x8000000f)
#define RVCSR_MCAUSE_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : RVCSR_MCAUSE_INTERRUPT
// Description : If 1, the trap was caused by an interrupt. If 0, it was caused
//               by an exception.
#define RVCSR_MCAUSE_INTERRUPT_RESET  _u(0x0)
#define RVCSR_MCAUSE_INTERRUPT_BITS   _u(0x80000000)
#define RVCSR_MCAUSE_INTERRUPT_MSB    _u(31)
#define RVCSR_MCAUSE_INTERRUPT_LSB    _u(31)
#define RVCSR_MCAUSE_INTERRUPT_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_MCAUSE_CODE
// Description : If `interrupt` is set, `code` indicates the index of the bit in
//               mip that caused the trap (3=soft IRQ, 7=timer IRQ, 11=external
//               IRQ). Otherwise, `code` is set according to the cause of the
//               exception.
//               0x0 -> Instruction fetch was misaligned. Will never fire on RP2350, since the C extension is enabled.
//               0x1 -> Instruction access fault. Instruction fetch failed a PMP check, or encountered a downstream bus fault, and then passed the point of no speculation.
//               0x2 -> Illegal instruction was executed (including illegal CSR accesses)
//               0x3 -> Breakpoint. An ebreak instruction was executed when the relevant dcsr.ebreak bit was clear.
//               0x4 -> Load address misaligned. Hazard3 requires natural alignment of all accesses.
//               0x5 -> Load access fault. A load failed a PMP check, or encountered a downstream bus error.
//               0x6 -> Store/AMO address misaligned. Hazard3 requires natural alignment of all accesses.
//               0x7 -> Store/AMO access fault. A store/AMO failed a PMP check, or encountered a downstream bus error. Also set if an AMO is attempted on a region that does not support atomics (on RP2350, anything but SRAM).
//               0x8 -> Environment call from U-mode.
//               0xb -> Environment call from M-mode.
#define RVCSR_MCAUSE_CODE_RESET  _u(0x0)
#define RVCSR_MCAUSE_CODE_BITS   _u(0x0000000f)
#define RVCSR_MCAUSE_CODE_MSB    _u(3)
#define RVCSR_MCAUSE_CODE_LSB    _u(0)
#define RVCSR_MCAUSE_CODE_ACCESS "RW"
#define RVCSR_MCAUSE_CODE_VALUE_INSTR_ALIGN _u(0x0)
#define RVCSR_MCAUSE_CODE_VALUE_INSTR_FAULT _u(0x1)
#define RVCSR_MCAUSE_CODE_VALUE_ILLEGAL_INSTR _u(0x2)
#define RVCSR_MCAUSE_CODE_VALUE_BREAKPOINT _u(0x3)
#define RVCSR_MCAUSE_CODE_VALUE_LOAD_ALIGN _u(0x4)
#define RVCSR_MCAUSE_CODE_VALUE_LOAD_FAULT _u(0x5)
#define RVCSR_MCAUSE_CODE_VALUE_STORE_ALIGN _u(0x6)
#define RVCSR_MCAUSE_CODE_VALUE_STORE_FAULT _u(0x7)
#define RVCSR_MCAUSE_CODE_VALUE_U_ECALL _u(0x8)
#define RVCSR_MCAUSE_CODE_VALUE_M_ECALL _u(0xb)
// =============================================================================
// Register    : RVCSR_MTVAL
// Description : Machine bad address or instruction. Hardwired to zero.
#define RVCSR_MTVAL_OFFSET _u(0x00000343)
#define RVCSR_MTVAL_BITS   _u(0xffffffff)
#define RVCSR_MTVAL_RESET  _u(0x00000000)
#define RVCSR_MTVAL_MSB    _u(31)
#define RVCSR_MTVAL_LSB    _u(0)
#define RVCSR_MTVAL_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MIP
// Description : Machine interrupt pending
#define RVCSR_MIP_OFFSET _u(0x00000344)
#define RVCSR_MIP_BITS   _u(0x00000888)
#define RVCSR_MIP_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : RVCSR_MIP_MEIP
// Description : External interrupt pending. The processor transfers to the
//               external interrupt vector when `mie.meie`, `mip.meip` and
//               `mstatus.mie` are all 1.
//
//               Hazard3 has internal registers to individually filter which
//               external IRQs appear in `meip`. When `meip` is 1, this
//               indicates there is at least one external interrupt which is
//               asserted (hence pending in `mieipa`), enabled in `meiea`, and
//               of priority greater than or equal to the current preemption
//               level in `meicontext.preempt`.
#define RVCSR_MIP_MEIP_RESET  _u(0x0)
#define RVCSR_MIP_MEIP_BITS   _u(0x00000800)
#define RVCSR_MIP_MEIP_MSB    _u(11)
#define RVCSR_MIP_MEIP_LSB    _u(11)
#define RVCSR_MIP_MEIP_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RVCSR_MIP_MTIP
// Description : Timer interrupt pending. The processor transfers to the timer
//               interrupt vector when `mie.mtie`, `mip.mtip` and `mstatus.mie`
//               are all 1, unless a software or external interrupt request is
//               also valid at this time.
#define RVCSR_MIP_MTIP_RESET  _u(0x0)
#define RVCSR_MIP_MTIP_BITS   _u(0x00000080)
#define RVCSR_MIP_MTIP_MSB    _u(7)
#define RVCSR_MIP_MTIP_LSB    _u(7)
#define RVCSR_MIP_MTIP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_MIP_MSIP
// Description : Software interrupt pending. The processor transfers to the
//               software interrupt vector `mie.msie`, `mip.msip` and
//               `mstatus.mie` are all 1, unless an external interrupt request
//               is also valid at this time.
#define RVCSR_MIP_MSIP_RESET  _u(0x0)
#define RVCSR_MIP_MSIP_BITS   _u(0x00000008)
#define RVCSR_MIP_MSIP_MSB    _u(3)
#define RVCSR_MIP_MSIP_LSB    _u(3)
#define RVCSR_MIP_MSIP_ACCESS "RW"
// =============================================================================
// Register    : RVCSR_PMPCFG0
// Description : Physical memory protection configuration for regions 0 through
//               3
#define RVCSR_PMPCFG0_OFFSET _u(0x000003a0)
#define RVCSR_PMPCFG0_BITS   _u(0x9f9f9f9f)
#define RVCSR_PMPCFG0_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG0_R3_L
// Description : Lock region 3, and apply it to M-mode as well as U-mode.
#define RVCSR_PMPCFG0_R3_L_RESET  _u(0x0)
#define RVCSR_PMPCFG0_R3_L_BITS   _u(0x80000000)
#define RVCSR_PMPCFG0_R3_L_MSB    _u(31)
#define RVCSR_PMPCFG0_R3_L_LSB    _u(31)
#define RVCSR_PMPCFG0_R3_L_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG0_R3_A
// Description : Address matching type for region 3. Writing an unsupported
//               value (TOR) will set the region to OFF.
//               0x0 -> Disable region
//               0x2 -> Naturally aligned 4-byte
//               0x3 -> Naturally aligned power-of-two (8 bytes to 4 GiB)
#define RVCSR_PMPCFG0_R3_A_RESET  _u(0x0)
#define RVCSR_PMPCFG0_R3_A_BITS   _u(0x18000000)
#define RVCSR_PMPCFG0_R3_A_MSB    _u(28)
#define RVCSR_PMPCFG0_R3_A_LSB    _u(27)
#define RVCSR_PMPCFG0_R3_A_ACCESS "RW"
#define RVCSR_PMPCFG0_R3_A_VALUE_OFF _u(0x0)
#define RVCSR_PMPCFG0_R3_A_VALUE_NA4 _u(0x2)
#define RVCSR_PMPCFG0_R3_A_VALUE_NAPOT _u(0x3)
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG0_R3_R
// Description : Read permission for region 3. Note R and X are transposed from
//               the standard bit order due to erratum RP2350-E6.
#define RVCSR_PMPCFG0_R3_R_RESET  _u(0x0)
#define RVCSR_PMPCFG0_R3_R_BITS   _u(0x04000000)
#define RVCSR_PMPCFG0_R3_R_MSB    _u(26)
#define RVCSR_PMPCFG0_R3_R_LSB    _u(26)
#define RVCSR_PMPCFG0_R3_R_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG0_R3_W
// Description : Write permission for region 3
#define RVCSR_PMPCFG0_R3_W_RESET  _u(0x0)
#define RVCSR_PMPCFG0_R3_W_BITS   _u(0x02000000)
#define RVCSR_PMPCFG0_R3_W_MSB    _u(25)
#define RVCSR_PMPCFG0_R3_W_LSB    _u(25)
#define RVCSR_PMPCFG0_R3_W_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG0_R3_X
// Description : Execute permission for region 3. Note R and X are transposed
//               from the standard bit order due to erratum RP2350-E6.
#define RVCSR_PMPCFG0_R3_X_RESET  _u(0x0)
#define RVCSR_PMPCFG0_R3_X_BITS   _u(0x01000000)
#define RVCSR_PMPCFG0_R3_X_MSB    _u(24)
#define RVCSR_PMPCFG0_R3_X_LSB    _u(24)
#define RVCSR_PMPCFG0_R3_X_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG0_R2_L
// Description : Lock region 2, and apply it to M-mode as well as U-mode.
#define RVCSR_PMPCFG0_R2_L_RESET  _u(0x0)
#define RVCSR_PMPCFG0_R2_L_BITS   _u(0x00800000)
#define RVCSR_PMPCFG0_R2_L_MSB    _u(23)
#define RVCSR_PMPCFG0_R2_L_LSB    _u(23)
#define RVCSR_PMPCFG0_R2_L_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG0_R2_A
// Description : Address matching type for region 2. Writing an unsupported
//               value (TOR) will set the region to OFF.
//               0x0 -> Disable region
//               0x2 -> Naturally aligned 4-byte
//               0x3 -> Naturally aligned power-of-two (8 bytes to 4 GiB)
#define RVCSR_PMPCFG0_R2_A_RESET  _u(0x0)
#define RVCSR_PMPCFG0_R2_A_BITS   _u(0x00180000)
#define RVCSR_PMPCFG0_R2_A_MSB    _u(20)
#define RVCSR_PMPCFG0_R2_A_LSB    _u(19)
#define RVCSR_PMPCFG0_R2_A_ACCESS "RW"
#define RVCSR_PMPCFG0_R2_A_VALUE_OFF _u(0x0)
#define RVCSR_PMPCFG0_R2_A_VALUE_NA4 _u(0x2)
#define RVCSR_PMPCFG0_R2_A_VALUE_NAPOT _u(0x3)
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG0_R2_R
// Description : Read permission for region 2. Note R and X are transposed from
//               the standard bit order due to erratum RP2350-E6.
#define RVCSR_PMPCFG0_R2_R_RESET  _u(0x0)
#define RVCSR_PMPCFG0_R2_R_BITS   _u(0x00040000)
#define RVCSR_PMPCFG0_R2_R_MSB    _u(18)
#define RVCSR_PMPCFG0_R2_R_LSB    _u(18)
#define RVCSR_PMPCFG0_R2_R_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG0_R2_W
// Description : Write permission for region 2
#define RVCSR_PMPCFG0_R2_W_RESET  _u(0x0)
#define RVCSR_PMPCFG0_R2_W_BITS   _u(0x00020000)
#define RVCSR_PMPCFG0_R2_W_MSB    _u(17)
#define RVCSR_PMPCFG0_R2_W_LSB    _u(17)
#define RVCSR_PMPCFG0_R2_W_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG0_R2_X
// Description : Execute permission for region 2. Note R and X are transposed
//               from the standard bit order due to erratum RP2350-E6.
#define RVCSR_PMPCFG0_R2_X_RESET  _u(0x0)
#define RVCSR_PMPCFG0_R2_X_BITS   _u(0x00010000)
#define RVCSR_PMPCFG0_R2_X_MSB    _u(16)
#define RVCSR_PMPCFG0_R2_X_LSB    _u(16)
#define RVCSR_PMPCFG0_R2_X_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG0_R1_L
// Description : Lock region 1, and apply it to M-mode as well as U-mode.
#define RVCSR_PMPCFG0_R1_L_RESET  _u(0x0)
#define RVCSR_PMPCFG0_R1_L_BITS   _u(0x00008000)
#define RVCSR_PMPCFG0_R1_L_MSB    _u(15)
#define RVCSR_PMPCFG0_R1_L_LSB    _u(15)
#define RVCSR_PMPCFG0_R1_L_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG0_R1_A
// Description : Address matching type for region 1. Writing an unsupported
//               value (TOR) will set the region to OFF.
//               0x0 -> Disable region
//               0x2 -> Naturally aligned 4-byte
//               0x3 -> Naturally aligned power-of-two (8 bytes to 4 GiB)
#define RVCSR_PMPCFG0_R1_A_RESET  _u(0x0)
#define RVCSR_PMPCFG0_R1_A_BITS   _u(0x00001800)
#define RVCSR_PMPCFG0_R1_A_MSB    _u(12)
#define RVCSR_PMPCFG0_R1_A_LSB    _u(11)
#define RVCSR_PMPCFG0_R1_A_ACCESS "RW"
#define RVCSR_PMPCFG0_R1_A_VALUE_OFF _u(0x0)
#define RVCSR_PMPCFG0_R1_A_VALUE_NA4 _u(0x2)
#define RVCSR_PMPCFG0_R1_A_VALUE_NAPOT _u(0x3)
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG0_R1_R
// Description : Read permission for region 1. Note R and X are transposed from
//               the standard bit order due to erratum RP2350-E6.
#define RVCSR_PMPCFG0_R1_R_RESET  _u(0x0)
#define RVCSR_PMPCFG0_R1_R_BITS   _u(0x00000400)
#define RVCSR_PMPCFG0_R1_R_MSB    _u(10)
#define RVCSR_PMPCFG0_R1_R_LSB    _u(10)
#define RVCSR_PMPCFG0_R1_R_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG0_R1_W
// Description : Write permission for region 1
#define RVCSR_PMPCFG0_R1_W_RESET  _u(0x0)
#define RVCSR_PMPCFG0_R1_W_BITS   _u(0x00000200)
#define RVCSR_PMPCFG0_R1_W_MSB    _u(9)
#define RVCSR_PMPCFG0_R1_W_LSB    _u(9)
#define RVCSR_PMPCFG0_R1_W_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG0_R1_X
// Description : Execute permission for region 1. Note R and X are transposed
//               from the standard bit order due to erratum RP2350-E6.
#define RVCSR_PMPCFG0_R1_X_RESET  _u(0x0)
#define RVCSR_PMPCFG0_R1_X_BITS   _u(0x00000100)
#define RVCSR_PMPCFG0_R1_X_MSB    _u(8)
#define RVCSR_PMPCFG0_R1_X_LSB    _u(8)
#define RVCSR_PMPCFG0_R1_X_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG0_R0_L
// Description : Lock region 0, and apply it to M-mode as well as U-mode.
#define RVCSR_PMPCFG0_R0_L_RESET  _u(0x0)
#define RVCSR_PMPCFG0_R0_L_BITS   _u(0x00000080)
#define RVCSR_PMPCFG0_R0_L_MSB    _u(7)
#define RVCSR_PMPCFG0_R0_L_LSB    _u(7)
#define RVCSR_PMPCFG0_R0_L_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG0_R0_A
// Description : Address matching type for region 0. Writing an unsupported
//               value (TOR) will set the region to OFF.
//               0x0 -> Disable region
//               0x2 -> Naturally aligned 4-byte
//               0x3 -> Naturally aligned power-of-two (8 bytes to 4 GiB)
#define RVCSR_PMPCFG0_R0_A_RESET  _u(0x0)
#define RVCSR_PMPCFG0_R0_A_BITS   _u(0x00000018)
#define RVCSR_PMPCFG0_R0_A_MSB    _u(4)
#define RVCSR_PMPCFG0_R0_A_LSB    _u(3)
#define RVCSR_PMPCFG0_R0_A_ACCESS "RW"
#define RVCSR_PMPCFG0_R0_A_VALUE_OFF _u(0x0)
#define RVCSR_PMPCFG0_R0_A_VALUE_NA4 _u(0x2)
#define RVCSR_PMPCFG0_R0_A_VALUE_NAPOT _u(0x3)
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG0_R0_R
// Description : Read permission for region 0. Note R and X are transposed from
//               the standard bit order due to erratum RP2350-E6.
#define RVCSR_PMPCFG0_R0_R_RESET  _u(0x0)
#define RVCSR_PMPCFG0_R0_R_BITS   _u(0x00000004)
#define RVCSR_PMPCFG0_R0_R_MSB    _u(2)
#define RVCSR_PMPCFG0_R0_R_LSB    _u(2)
#define RVCSR_PMPCFG0_R0_R_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG0_R0_W
// Description : Write permission for region 0
#define RVCSR_PMPCFG0_R0_W_RESET  _u(0x0)
#define RVCSR_PMPCFG0_R0_W_BITS   _u(0x00000002)
#define RVCSR_PMPCFG0_R0_W_MSB    _u(1)
#define RVCSR_PMPCFG0_R0_W_LSB    _u(1)
#define RVCSR_PMPCFG0_R0_W_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG0_R0_X
// Description : Execute permission for region 0. Note R and X are transposed
//               from the standard bit order due to erratum RP2350-E6.
#define RVCSR_PMPCFG0_R0_X_RESET  _u(0x0)
#define RVCSR_PMPCFG0_R0_X_BITS   _u(0x00000001)
#define RVCSR_PMPCFG0_R0_X_MSB    _u(0)
#define RVCSR_PMPCFG0_R0_X_LSB    _u(0)
#define RVCSR_PMPCFG0_R0_X_ACCESS "RW"
// =============================================================================
// Register    : RVCSR_PMPCFG1
// Description : Physical memory protection configuration for regions 4 through
//               7
#define RVCSR_PMPCFG1_OFFSET _u(0x000003a1)
#define RVCSR_PMPCFG1_BITS   _u(0x9f9f9f9f)
#define RVCSR_PMPCFG1_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG1_R7_L
// Description : Lock region 7, and apply it to M-mode as well as U-mode.
#define RVCSR_PMPCFG1_R7_L_RESET  _u(0x0)
#define RVCSR_PMPCFG1_R7_L_BITS   _u(0x80000000)
#define RVCSR_PMPCFG1_R7_L_MSB    _u(31)
#define RVCSR_PMPCFG1_R7_L_LSB    _u(31)
#define RVCSR_PMPCFG1_R7_L_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG1_R7_A
// Description : Address matching type for region 7. Writing an unsupported
//               value (TOR) will set the region to OFF.
//               0x0 -> Disable region
//               0x2 -> Naturally aligned 4-byte
//               0x3 -> Naturally aligned power-of-two (8 bytes to 4 GiB)
#define RVCSR_PMPCFG1_R7_A_RESET  _u(0x0)
#define RVCSR_PMPCFG1_R7_A_BITS   _u(0x18000000)
#define RVCSR_PMPCFG1_R7_A_MSB    _u(28)
#define RVCSR_PMPCFG1_R7_A_LSB    _u(27)
#define RVCSR_PMPCFG1_R7_A_ACCESS "RW"
#define RVCSR_PMPCFG1_R7_A_VALUE_OFF _u(0x0)
#define RVCSR_PMPCFG1_R7_A_VALUE_NA4 _u(0x2)
#define RVCSR_PMPCFG1_R7_A_VALUE_NAPOT _u(0x3)
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG1_R7_R
// Description : Read permission for region 7. Note R and X are transposed from
//               the standard bit order due to erratum RP2350-E6.
#define RVCSR_PMPCFG1_R7_R_RESET  _u(0x0)
#define RVCSR_PMPCFG1_R7_R_BITS   _u(0x04000000)
#define RVCSR_PMPCFG1_R7_R_MSB    _u(26)
#define RVCSR_PMPCFG1_R7_R_LSB    _u(26)
#define RVCSR_PMPCFG1_R7_R_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG1_R7_W
// Description : Write permission for region 7
#define RVCSR_PMPCFG1_R7_W_RESET  _u(0x0)
#define RVCSR_PMPCFG1_R7_W_BITS   _u(0x02000000)
#define RVCSR_PMPCFG1_R7_W_MSB    _u(25)
#define RVCSR_PMPCFG1_R7_W_LSB    _u(25)
#define RVCSR_PMPCFG1_R7_W_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG1_R7_X
// Description : Execute permission for region 7. Note R and X are transposed
//               from the standard bit order due to erratum RP2350-E6.
#define RVCSR_PMPCFG1_R7_X_RESET  _u(0x0)
#define RVCSR_PMPCFG1_R7_X_BITS   _u(0x01000000)
#define RVCSR_PMPCFG1_R7_X_MSB    _u(24)
#define RVCSR_PMPCFG1_R7_X_LSB    _u(24)
#define RVCSR_PMPCFG1_R7_X_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG1_R6_L
// Description : Lock region 6, and apply it to M-mode as well as U-mode.
#define RVCSR_PMPCFG1_R6_L_RESET  _u(0x0)
#define RVCSR_PMPCFG1_R6_L_BITS   _u(0x00800000)
#define RVCSR_PMPCFG1_R6_L_MSB    _u(23)
#define RVCSR_PMPCFG1_R6_L_LSB    _u(23)
#define RVCSR_PMPCFG1_R6_L_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG1_R6_A
// Description : Address matching type for region 6. Writing an unsupported
//               value (TOR) will set the region to OFF.
//               0x0 -> Disable region
//               0x2 -> Naturally aligned 4-byte
//               0x3 -> Naturally aligned power-of-two (8 bytes to 4 GiB)
#define RVCSR_PMPCFG1_R6_A_RESET  _u(0x0)
#define RVCSR_PMPCFG1_R6_A_BITS   _u(0x00180000)
#define RVCSR_PMPCFG1_R6_A_MSB    _u(20)
#define RVCSR_PMPCFG1_R6_A_LSB    _u(19)
#define RVCSR_PMPCFG1_R6_A_ACCESS "RW"
#define RVCSR_PMPCFG1_R6_A_VALUE_OFF _u(0x0)
#define RVCSR_PMPCFG1_R6_A_VALUE_NA4 _u(0x2)
#define RVCSR_PMPCFG1_R6_A_VALUE_NAPOT _u(0x3)
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG1_R6_R
// Description : Read permission for region 6. Note R and X are transposed from
//               the standard bit order due to erratum RP2350-E6.
#define RVCSR_PMPCFG1_R6_R_RESET  _u(0x0)
#define RVCSR_PMPCFG1_R6_R_BITS   _u(0x00040000)
#define RVCSR_PMPCFG1_R6_R_MSB    _u(18)
#define RVCSR_PMPCFG1_R6_R_LSB    _u(18)
#define RVCSR_PMPCFG1_R6_R_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG1_R6_W
// Description : Write permission for region 6
#define RVCSR_PMPCFG1_R6_W_RESET  _u(0x0)
#define RVCSR_PMPCFG1_R6_W_BITS   _u(0x00020000)
#define RVCSR_PMPCFG1_R6_W_MSB    _u(17)
#define RVCSR_PMPCFG1_R6_W_LSB    _u(17)
#define RVCSR_PMPCFG1_R6_W_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG1_R6_X
// Description : Execute permission for region 6. Note R and X are transposed
//               from the standard bit order due to erratum RP2350-E6.
#define RVCSR_PMPCFG1_R6_X_RESET  _u(0x0)
#define RVCSR_PMPCFG1_R6_X_BITS   _u(0x00010000)
#define RVCSR_PMPCFG1_R6_X_MSB    _u(16)
#define RVCSR_PMPCFG1_R6_X_LSB    _u(16)
#define RVCSR_PMPCFG1_R6_X_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG1_R5_L
// Description : Lock region 5, and apply it to M-mode as well as U-mode.
#define RVCSR_PMPCFG1_R5_L_RESET  _u(0x0)
#define RVCSR_PMPCFG1_R5_L_BITS   _u(0x00008000)
#define RVCSR_PMPCFG1_R5_L_MSB    _u(15)
#define RVCSR_PMPCFG1_R5_L_LSB    _u(15)
#define RVCSR_PMPCFG1_R5_L_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG1_R5_A
// Description : Address matching type for region 5. Writing an unsupported
//               value (TOR) will set the region to OFF.
//               0x0 -> Disable region
//               0x2 -> Naturally aligned 4-byte
//               0x3 -> Naturally aligned power-of-two (8 bytes to 4 GiB)
#define RVCSR_PMPCFG1_R5_A_RESET  _u(0x0)
#define RVCSR_PMPCFG1_R5_A_BITS   _u(0x00001800)
#define RVCSR_PMPCFG1_R5_A_MSB    _u(12)
#define RVCSR_PMPCFG1_R5_A_LSB    _u(11)
#define RVCSR_PMPCFG1_R5_A_ACCESS "RW"
#define RVCSR_PMPCFG1_R5_A_VALUE_OFF _u(0x0)
#define RVCSR_PMPCFG1_R5_A_VALUE_NA4 _u(0x2)
#define RVCSR_PMPCFG1_R5_A_VALUE_NAPOT _u(0x3)
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG1_R5_R
// Description : Read permission for region 5. Note R and X are transposed from
//               the standard bit order due to erratum RP2350-E6.
#define RVCSR_PMPCFG1_R5_R_RESET  _u(0x0)
#define RVCSR_PMPCFG1_R5_R_BITS   _u(0x00000400)
#define RVCSR_PMPCFG1_R5_R_MSB    _u(10)
#define RVCSR_PMPCFG1_R5_R_LSB    _u(10)
#define RVCSR_PMPCFG1_R5_R_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG1_R5_W
// Description : Write permission for region 5
#define RVCSR_PMPCFG1_R5_W_RESET  _u(0x0)
#define RVCSR_PMPCFG1_R5_W_BITS   _u(0x00000200)
#define RVCSR_PMPCFG1_R5_W_MSB    _u(9)
#define RVCSR_PMPCFG1_R5_W_LSB    _u(9)
#define RVCSR_PMPCFG1_R5_W_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG1_R5_X
// Description : Execute permission for region 5. Note R and X are transposed
//               from the standard bit order due to erratum RP2350-E6.
#define RVCSR_PMPCFG1_R5_X_RESET  _u(0x0)
#define RVCSR_PMPCFG1_R5_X_BITS   _u(0x00000100)
#define RVCSR_PMPCFG1_R5_X_MSB    _u(8)
#define RVCSR_PMPCFG1_R5_X_LSB    _u(8)
#define RVCSR_PMPCFG1_R5_X_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG1_R4_L
// Description : Lock region 4, and apply it to M-mode as well as U-mode.
#define RVCSR_PMPCFG1_R4_L_RESET  _u(0x0)
#define RVCSR_PMPCFG1_R4_L_BITS   _u(0x00000080)
#define RVCSR_PMPCFG1_R4_L_MSB    _u(7)
#define RVCSR_PMPCFG1_R4_L_LSB    _u(7)
#define RVCSR_PMPCFG1_R4_L_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG1_R4_A
// Description : Address matching type for region 4. Writing an unsupported
//               value (TOR) will set the region to OFF.
//               0x0 -> Disable region
//               0x2 -> Naturally aligned 4-byte
//               0x3 -> Naturally aligned power-of-two (8 bytes to 4 GiB)
#define RVCSR_PMPCFG1_R4_A_RESET  _u(0x0)
#define RVCSR_PMPCFG1_R4_A_BITS   _u(0x00000018)
#define RVCSR_PMPCFG1_R4_A_MSB    _u(4)
#define RVCSR_PMPCFG1_R4_A_LSB    _u(3)
#define RVCSR_PMPCFG1_R4_A_ACCESS "RW"
#define RVCSR_PMPCFG1_R4_A_VALUE_OFF _u(0x0)
#define RVCSR_PMPCFG1_R4_A_VALUE_NA4 _u(0x2)
#define RVCSR_PMPCFG1_R4_A_VALUE_NAPOT _u(0x3)
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG1_R4_R
// Description : Read permission for region 4. Note R and X are transposed from
//               the standard bit order due to erratum RP2350-E6.
#define RVCSR_PMPCFG1_R4_R_RESET  _u(0x0)
#define RVCSR_PMPCFG1_R4_R_BITS   _u(0x00000004)
#define RVCSR_PMPCFG1_R4_R_MSB    _u(2)
#define RVCSR_PMPCFG1_R4_R_LSB    _u(2)
#define RVCSR_PMPCFG1_R4_R_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG1_R4_W
// Description : Write permission for region 4
#define RVCSR_PMPCFG1_R4_W_RESET  _u(0x0)
#define RVCSR_PMPCFG1_R4_W_BITS   _u(0x00000002)
#define RVCSR_PMPCFG1_R4_W_MSB    _u(1)
#define RVCSR_PMPCFG1_R4_W_LSB    _u(1)
#define RVCSR_PMPCFG1_R4_W_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG1_R4_X
// Description : Execute permission for region 4. Note R and X are transposed
//               from the standard bit order due to erratum RP2350-E6.
#define RVCSR_PMPCFG1_R4_X_RESET  _u(0x0)
#define RVCSR_PMPCFG1_R4_X_BITS   _u(0x00000001)
#define RVCSR_PMPCFG1_R4_X_MSB    _u(0)
#define RVCSR_PMPCFG1_R4_X_LSB    _u(0)
#define RVCSR_PMPCFG1_R4_X_ACCESS "RW"
// =============================================================================
// Register    : RVCSR_PMPCFG2
// Description : Physical memory protection configuration for regions 8 through
//               11
#define RVCSR_PMPCFG2_OFFSET _u(0x000003a2)
#define RVCSR_PMPCFG2_BITS   _u(0x9f9f9f9f)
#define RVCSR_PMPCFG2_RESET  _u(0x001f1f1f)
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG2_R11_L
// Description : Lock region 11, and apply it to M-mode as well as U-mode.
#define RVCSR_PMPCFG2_R11_L_RESET  _u(0x0)
#define RVCSR_PMPCFG2_R11_L_BITS   _u(0x80000000)
#define RVCSR_PMPCFG2_R11_L_MSB    _u(31)
#define RVCSR_PMPCFG2_R11_L_LSB    _u(31)
#define RVCSR_PMPCFG2_R11_L_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG2_R11_A
// Description : Address matching type for region 11. Writing an unsupported
//               value (TOR) will set the region to OFF.
//               0x0 -> Disable region
//               0x2 -> Naturally aligned 4-byte
//               0x3 -> Naturally aligned power-of-two (8 bytes to 4 GiB)
#define RVCSR_PMPCFG2_R11_A_RESET  _u(0x0)
#define RVCSR_PMPCFG2_R11_A_BITS   _u(0x18000000)
#define RVCSR_PMPCFG2_R11_A_MSB    _u(28)
#define RVCSR_PMPCFG2_R11_A_LSB    _u(27)
#define RVCSR_PMPCFG2_R11_A_ACCESS "RO"
#define RVCSR_PMPCFG2_R11_A_VALUE_OFF _u(0x0)
#define RVCSR_PMPCFG2_R11_A_VALUE_NA4 _u(0x2)
#define RVCSR_PMPCFG2_R11_A_VALUE_NAPOT _u(0x3)
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG2_R11_R
// Description : Read permission for region 11. Note R and X are transposed from
//               the standard bit order due to erratum RP2350-E6.
#define RVCSR_PMPCFG2_R11_R_RESET  _u(0x0)
#define RVCSR_PMPCFG2_R11_R_BITS   _u(0x04000000)
#define RVCSR_PMPCFG2_R11_R_MSB    _u(26)
#define RVCSR_PMPCFG2_R11_R_LSB    _u(26)
#define RVCSR_PMPCFG2_R11_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG2_R11_W
// Description : Write permission for region 11
#define RVCSR_PMPCFG2_R11_W_RESET  _u(0x0)
#define RVCSR_PMPCFG2_R11_W_BITS   _u(0x02000000)
#define RVCSR_PMPCFG2_R11_W_MSB    _u(25)
#define RVCSR_PMPCFG2_R11_W_LSB    _u(25)
#define RVCSR_PMPCFG2_R11_W_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG2_R11_X
// Description : Execute permission for region 11. Note R and X are transposed
//               from the standard bit order due to erratum RP2350-E6.
#define RVCSR_PMPCFG2_R11_X_RESET  _u(0x0)
#define RVCSR_PMPCFG2_R11_X_BITS   _u(0x01000000)
#define RVCSR_PMPCFG2_R11_X_MSB    _u(24)
#define RVCSR_PMPCFG2_R11_X_LSB    _u(24)
#define RVCSR_PMPCFG2_R11_X_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG2_R10_L
// Description : Lock region 10, and apply it to M-mode as well as U-mode.
#define RVCSR_PMPCFG2_R10_L_RESET  _u(0x0)
#define RVCSR_PMPCFG2_R10_L_BITS   _u(0x00800000)
#define RVCSR_PMPCFG2_R10_L_MSB    _u(23)
#define RVCSR_PMPCFG2_R10_L_LSB    _u(23)
#define RVCSR_PMPCFG2_R10_L_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG2_R10_A
// Description : Address matching type for region 10. Writing an unsupported
//               value (TOR) will set the region to OFF.
//               0x0 -> Disable region
//               0x2 -> Naturally aligned 4-byte
//               0x3 -> Naturally aligned power-of-two (8 bytes to 4 GiB)
#define RVCSR_PMPCFG2_R10_A_RESET  _u(0x3)
#define RVCSR_PMPCFG2_R10_A_BITS   _u(0x00180000)
#define RVCSR_PMPCFG2_R10_A_MSB    _u(20)
#define RVCSR_PMPCFG2_R10_A_LSB    _u(19)
#define RVCSR_PMPCFG2_R10_A_ACCESS "RO"
#define RVCSR_PMPCFG2_R10_A_VALUE_OFF _u(0x0)
#define RVCSR_PMPCFG2_R10_A_VALUE_NA4 _u(0x2)
#define RVCSR_PMPCFG2_R10_A_VALUE_NAPOT _u(0x3)
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG2_R10_R
// Description : Read permission for region 10. Note R and X are transposed from
//               the standard bit order due to erratum RP2350-E6.
#define RVCSR_PMPCFG2_R10_R_RESET  _u(0x1)
#define RVCSR_PMPCFG2_R10_R_BITS   _u(0x00040000)
#define RVCSR_PMPCFG2_R10_R_MSB    _u(18)
#define RVCSR_PMPCFG2_R10_R_LSB    _u(18)
#define RVCSR_PMPCFG2_R10_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG2_R10_W
// Description : Write permission for region 10
#define RVCSR_PMPCFG2_R10_W_RESET  _u(0x1)
#define RVCSR_PMPCFG2_R10_W_BITS   _u(0x00020000)
#define RVCSR_PMPCFG2_R10_W_MSB    _u(17)
#define RVCSR_PMPCFG2_R10_W_LSB    _u(17)
#define RVCSR_PMPCFG2_R10_W_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG2_R10_X
// Description : Execute permission for region 10. Note R and X are transposed
//               from the standard bit order due to erratum RP2350-E6.
#define RVCSR_PMPCFG2_R10_X_RESET  _u(0x1)
#define RVCSR_PMPCFG2_R10_X_BITS   _u(0x00010000)
#define RVCSR_PMPCFG2_R10_X_MSB    _u(16)
#define RVCSR_PMPCFG2_R10_X_LSB    _u(16)
#define RVCSR_PMPCFG2_R10_X_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG2_R9_L
// Description : Lock region 9, and apply it to M-mode as well as U-mode.
#define RVCSR_PMPCFG2_R9_L_RESET  _u(0x0)
#define RVCSR_PMPCFG2_R9_L_BITS   _u(0x00008000)
#define RVCSR_PMPCFG2_R9_L_MSB    _u(15)
#define RVCSR_PMPCFG2_R9_L_LSB    _u(15)
#define RVCSR_PMPCFG2_R9_L_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG2_R9_A
// Description : Address matching type for region 9. Writing an unsupported
//               value (TOR) will set the region to OFF.
//               0x0 -> Disable region
//               0x2 -> Naturally aligned 4-byte
//               0x3 -> Naturally aligned power-of-two (8 bytes to 4 GiB)
#define RVCSR_PMPCFG2_R9_A_RESET  _u(0x3)
#define RVCSR_PMPCFG2_R9_A_BITS   _u(0x00001800)
#define RVCSR_PMPCFG2_R9_A_MSB    _u(12)
#define RVCSR_PMPCFG2_R9_A_LSB    _u(11)
#define RVCSR_PMPCFG2_R9_A_ACCESS "RO"
#define RVCSR_PMPCFG2_R9_A_VALUE_OFF _u(0x0)
#define RVCSR_PMPCFG2_R9_A_VALUE_NA4 _u(0x2)
#define RVCSR_PMPCFG2_R9_A_VALUE_NAPOT _u(0x3)
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG2_R9_R
// Description : Read permission for region 9. Note R and X are transposed from
//               the standard bit order due to erratum RP2350-E6.
#define RVCSR_PMPCFG2_R9_R_RESET  _u(0x1)
#define RVCSR_PMPCFG2_R9_R_BITS   _u(0x00000400)
#define RVCSR_PMPCFG2_R9_R_MSB    _u(10)
#define RVCSR_PMPCFG2_R9_R_LSB    _u(10)
#define RVCSR_PMPCFG2_R9_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG2_R9_W
// Description : Write permission for region 9
#define RVCSR_PMPCFG2_R9_W_RESET  _u(0x1)
#define RVCSR_PMPCFG2_R9_W_BITS   _u(0x00000200)
#define RVCSR_PMPCFG2_R9_W_MSB    _u(9)
#define RVCSR_PMPCFG2_R9_W_LSB    _u(9)
#define RVCSR_PMPCFG2_R9_W_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG2_R9_X
// Description : Execute permission for region 9. Note R and X are transposed
//               from the standard bit order due to erratum RP2350-E6.
#define RVCSR_PMPCFG2_R9_X_RESET  _u(0x1)
#define RVCSR_PMPCFG2_R9_X_BITS   _u(0x00000100)
#define RVCSR_PMPCFG2_R9_X_MSB    _u(8)
#define RVCSR_PMPCFG2_R9_X_LSB    _u(8)
#define RVCSR_PMPCFG2_R9_X_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG2_R8_L
// Description : Lock region 8, and apply it to M-mode as well as U-mode.
#define RVCSR_PMPCFG2_R8_L_RESET  _u(0x0)
#define RVCSR_PMPCFG2_R8_L_BITS   _u(0x00000080)
#define RVCSR_PMPCFG2_R8_L_MSB    _u(7)
#define RVCSR_PMPCFG2_R8_L_LSB    _u(7)
#define RVCSR_PMPCFG2_R8_L_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG2_R8_A
// Description : Address matching type for region 8. Writing an unsupported
//               value (TOR) will set the region to OFF.
//               0x0 -> Disable region
//               0x2 -> Naturally aligned 4-byte
//               0x3 -> Naturally aligned power-of-two (8 bytes to 4 GiB)
#define RVCSR_PMPCFG2_R8_A_RESET  _u(0x3)
#define RVCSR_PMPCFG2_R8_A_BITS   _u(0x00000018)
#define RVCSR_PMPCFG2_R8_A_MSB    _u(4)
#define RVCSR_PMPCFG2_R8_A_LSB    _u(3)
#define RVCSR_PMPCFG2_R8_A_ACCESS "RO"
#define RVCSR_PMPCFG2_R8_A_VALUE_OFF _u(0x0)
#define RVCSR_PMPCFG2_R8_A_VALUE_NA4 _u(0x2)
#define RVCSR_PMPCFG2_R8_A_VALUE_NAPOT _u(0x3)
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG2_R8_R
// Description : Read permission for region 8. Note R and X are transposed from
//               the standard bit order due to erratum RP2350-E6.
#define RVCSR_PMPCFG2_R8_R_RESET  _u(0x1)
#define RVCSR_PMPCFG2_R8_R_BITS   _u(0x00000004)
#define RVCSR_PMPCFG2_R8_R_MSB    _u(2)
#define RVCSR_PMPCFG2_R8_R_LSB    _u(2)
#define RVCSR_PMPCFG2_R8_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG2_R8_W
// Description : Write permission for region 8
#define RVCSR_PMPCFG2_R8_W_RESET  _u(0x1)
#define RVCSR_PMPCFG2_R8_W_BITS   _u(0x00000002)
#define RVCSR_PMPCFG2_R8_W_MSB    _u(1)
#define RVCSR_PMPCFG2_R8_W_LSB    _u(1)
#define RVCSR_PMPCFG2_R8_W_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG2_R8_X
// Description : Execute permission for region 8. Note R and X are transposed
//               from the standard bit order due to erratum RP2350-E6.
#define RVCSR_PMPCFG2_R8_X_RESET  _u(0x1)
#define RVCSR_PMPCFG2_R8_X_BITS   _u(0x00000001)
#define RVCSR_PMPCFG2_R8_X_MSB    _u(0)
#define RVCSR_PMPCFG2_R8_X_LSB    _u(0)
#define RVCSR_PMPCFG2_R8_X_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_PMPCFG3
// Description : Physical memory protection configuration for regions 12 through
//               15
#define RVCSR_PMPCFG3_OFFSET _u(0x000003a3)
#define RVCSR_PMPCFG3_BITS   _u(0x9f9f9f9f)
#define RVCSR_PMPCFG3_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG3_R15_L
// Description : Lock region 15, and apply it to M-mode as well as U-mode.
#define RVCSR_PMPCFG3_R15_L_RESET  _u(0x0)
#define RVCSR_PMPCFG3_R15_L_BITS   _u(0x80000000)
#define RVCSR_PMPCFG3_R15_L_MSB    _u(31)
#define RVCSR_PMPCFG3_R15_L_LSB    _u(31)
#define RVCSR_PMPCFG3_R15_L_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG3_R15_A
// Description : Address matching type for region 15. Writing an unsupported
//               value (TOR) will set the region to OFF.
//               0x0 -> Disable region
//               0x2 -> Naturally aligned 4-byte
//               0x3 -> Naturally aligned power-of-two (8 bytes to 4 GiB)
#define RVCSR_PMPCFG3_R15_A_RESET  _u(0x0)
#define RVCSR_PMPCFG3_R15_A_BITS   _u(0x18000000)
#define RVCSR_PMPCFG3_R15_A_MSB    _u(28)
#define RVCSR_PMPCFG3_R15_A_LSB    _u(27)
#define RVCSR_PMPCFG3_R15_A_ACCESS "RO"
#define RVCSR_PMPCFG3_R15_A_VALUE_OFF _u(0x0)
#define RVCSR_PMPCFG3_R15_A_VALUE_NA4 _u(0x2)
#define RVCSR_PMPCFG3_R15_A_VALUE_NAPOT _u(0x3)
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG3_R15_R
// Description : Read permission for region 15. Note R and X are transposed from
//               the standard bit order due to erratum RP2350-E6.
#define RVCSR_PMPCFG3_R15_R_RESET  _u(0x0)
#define RVCSR_PMPCFG3_R15_R_BITS   _u(0x04000000)
#define RVCSR_PMPCFG3_R15_R_MSB    _u(26)
#define RVCSR_PMPCFG3_R15_R_LSB    _u(26)
#define RVCSR_PMPCFG3_R15_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG3_R15_W
// Description : Write permission for region 15
#define RVCSR_PMPCFG3_R15_W_RESET  _u(0x0)
#define RVCSR_PMPCFG3_R15_W_BITS   _u(0x02000000)
#define RVCSR_PMPCFG3_R15_W_MSB    _u(25)
#define RVCSR_PMPCFG3_R15_W_LSB    _u(25)
#define RVCSR_PMPCFG3_R15_W_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG3_R15_X
// Description : Execute permission for region 15. Note R and X are transposed
//               from the standard bit order due to erratum RP2350-E6.
#define RVCSR_PMPCFG3_R15_X_RESET  _u(0x0)
#define RVCSR_PMPCFG3_R15_X_BITS   _u(0x01000000)
#define RVCSR_PMPCFG3_R15_X_MSB    _u(24)
#define RVCSR_PMPCFG3_R15_X_LSB    _u(24)
#define RVCSR_PMPCFG3_R15_X_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG3_R14_L
// Description : Lock region 14, and apply it to M-mode as well as U-mode.
#define RVCSR_PMPCFG3_R14_L_RESET  _u(0x0)
#define RVCSR_PMPCFG3_R14_L_BITS   _u(0x00800000)
#define RVCSR_PMPCFG3_R14_L_MSB    _u(23)
#define RVCSR_PMPCFG3_R14_L_LSB    _u(23)
#define RVCSR_PMPCFG3_R14_L_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG3_R14_A
// Description : Address matching type for region 14. Writing an unsupported
//               value (TOR) will set the region to OFF.
//               0x0 -> Disable region
//               0x2 -> Naturally aligned 4-byte
//               0x3 -> Naturally aligned power-of-two (8 bytes to 4 GiB)
#define RVCSR_PMPCFG3_R14_A_RESET  _u(0x0)
#define RVCSR_PMPCFG3_R14_A_BITS   _u(0x00180000)
#define RVCSR_PMPCFG3_R14_A_MSB    _u(20)
#define RVCSR_PMPCFG3_R14_A_LSB    _u(19)
#define RVCSR_PMPCFG3_R14_A_ACCESS "RO"
#define RVCSR_PMPCFG3_R14_A_VALUE_OFF _u(0x0)
#define RVCSR_PMPCFG3_R14_A_VALUE_NA4 _u(0x2)
#define RVCSR_PMPCFG3_R14_A_VALUE_NAPOT _u(0x3)
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG3_R14_R
// Description : Read permission for region 14. Note R and X are transposed from
//               the standard bit order due to erratum RP2350-E6.
#define RVCSR_PMPCFG3_R14_R_RESET  _u(0x0)
#define RVCSR_PMPCFG3_R14_R_BITS   _u(0x00040000)
#define RVCSR_PMPCFG3_R14_R_MSB    _u(18)
#define RVCSR_PMPCFG3_R14_R_LSB    _u(18)
#define RVCSR_PMPCFG3_R14_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG3_R14_W
// Description : Write permission for region 14
#define RVCSR_PMPCFG3_R14_W_RESET  _u(0x0)
#define RVCSR_PMPCFG3_R14_W_BITS   _u(0x00020000)
#define RVCSR_PMPCFG3_R14_W_MSB    _u(17)
#define RVCSR_PMPCFG3_R14_W_LSB    _u(17)
#define RVCSR_PMPCFG3_R14_W_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG3_R14_X
// Description : Execute permission for region 14. Note R and X are transposed
//               from the standard bit order due to erratum RP2350-E6.
#define RVCSR_PMPCFG3_R14_X_RESET  _u(0x0)
#define RVCSR_PMPCFG3_R14_X_BITS   _u(0x00010000)
#define RVCSR_PMPCFG3_R14_X_MSB    _u(16)
#define RVCSR_PMPCFG3_R14_X_LSB    _u(16)
#define RVCSR_PMPCFG3_R14_X_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG3_R13_L
// Description : Lock region 13, and apply it to M-mode as well as U-mode.
#define RVCSR_PMPCFG3_R13_L_RESET  _u(0x0)
#define RVCSR_PMPCFG3_R13_L_BITS   _u(0x00008000)
#define RVCSR_PMPCFG3_R13_L_MSB    _u(15)
#define RVCSR_PMPCFG3_R13_L_LSB    _u(15)
#define RVCSR_PMPCFG3_R13_L_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG3_R13_A
// Description : Address matching type for region 13. Writing an unsupported
//               value (TOR) will set the region to OFF.
//               0x0 -> Disable region
//               0x2 -> Naturally aligned 4-byte
//               0x3 -> Naturally aligned power-of-two (8 bytes to 4 GiB)
#define RVCSR_PMPCFG3_R13_A_RESET  _u(0x0)
#define RVCSR_PMPCFG3_R13_A_BITS   _u(0x00001800)
#define RVCSR_PMPCFG3_R13_A_MSB    _u(12)
#define RVCSR_PMPCFG3_R13_A_LSB    _u(11)
#define RVCSR_PMPCFG3_R13_A_ACCESS "RO"
#define RVCSR_PMPCFG3_R13_A_VALUE_OFF _u(0x0)
#define RVCSR_PMPCFG3_R13_A_VALUE_NA4 _u(0x2)
#define RVCSR_PMPCFG3_R13_A_VALUE_NAPOT _u(0x3)
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG3_R13_R
// Description : Read permission for region 13. Note R and X are transposed from
//               the standard bit order due to erratum RP2350-E6.
#define RVCSR_PMPCFG3_R13_R_RESET  _u(0x0)
#define RVCSR_PMPCFG3_R13_R_BITS   _u(0x00000400)
#define RVCSR_PMPCFG3_R13_R_MSB    _u(10)
#define RVCSR_PMPCFG3_R13_R_LSB    _u(10)
#define RVCSR_PMPCFG3_R13_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG3_R13_W
// Description : Write permission for region 13
#define RVCSR_PMPCFG3_R13_W_RESET  _u(0x0)
#define RVCSR_PMPCFG3_R13_W_BITS   _u(0x00000200)
#define RVCSR_PMPCFG3_R13_W_MSB    _u(9)
#define RVCSR_PMPCFG3_R13_W_LSB    _u(9)
#define RVCSR_PMPCFG3_R13_W_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG3_R13_X
// Description : Execute permission for region 13. Note R and X are transposed
//               from the standard bit order due to erratum RP2350-E6.
#define RVCSR_PMPCFG3_R13_X_RESET  _u(0x0)
#define RVCSR_PMPCFG3_R13_X_BITS   _u(0x00000100)
#define RVCSR_PMPCFG3_R13_X_MSB    _u(8)
#define RVCSR_PMPCFG3_R13_X_LSB    _u(8)
#define RVCSR_PMPCFG3_R13_X_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG3_R12_L
// Description : Lock region 12, and apply it to M-mode as well as U-mode.
#define RVCSR_PMPCFG3_R12_L_RESET  _u(0x0)
#define RVCSR_PMPCFG3_R12_L_BITS   _u(0x00000080)
#define RVCSR_PMPCFG3_R12_L_MSB    _u(7)
#define RVCSR_PMPCFG3_R12_L_LSB    _u(7)
#define RVCSR_PMPCFG3_R12_L_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG3_R12_A
// Description : Address matching type for region 12. Writing an unsupported
//               value (TOR) will set the region to OFF.
//               0x0 -> Disable region
//               0x2 -> Naturally aligned 4-byte
//               0x3 -> Naturally aligned power-of-two (8 bytes to 4 GiB)
#define RVCSR_PMPCFG3_R12_A_RESET  _u(0x0)
#define RVCSR_PMPCFG3_R12_A_BITS   _u(0x00000018)
#define RVCSR_PMPCFG3_R12_A_MSB    _u(4)
#define RVCSR_PMPCFG3_R12_A_LSB    _u(3)
#define RVCSR_PMPCFG3_R12_A_ACCESS "RO"
#define RVCSR_PMPCFG3_R12_A_VALUE_OFF _u(0x0)
#define RVCSR_PMPCFG3_R12_A_VALUE_NA4 _u(0x2)
#define RVCSR_PMPCFG3_R12_A_VALUE_NAPOT _u(0x3)
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG3_R12_R
// Description : Read permission for region 12. Note R and X are transposed from
//               the standard bit order due to erratum RP2350-E6.
#define RVCSR_PMPCFG3_R12_R_RESET  _u(0x0)
#define RVCSR_PMPCFG3_R12_R_BITS   _u(0x00000004)
#define RVCSR_PMPCFG3_R12_R_MSB    _u(2)
#define RVCSR_PMPCFG3_R12_R_LSB    _u(2)
#define RVCSR_PMPCFG3_R12_R_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG3_R12_W
// Description : Write permission for region 12
#define RVCSR_PMPCFG3_R12_W_RESET  _u(0x0)
#define RVCSR_PMPCFG3_R12_W_BITS   _u(0x00000002)
#define RVCSR_PMPCFG3_R12_W_MSB    _u(1)
#define RVCSR_PMPCFG3_R12_W_LSB    _u(1)
#define RVCSR_PMPCFG3_R12_W_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RVCSR_PMPCFG3_R12_X
// Description : Execute permission for region 12. Note R and X are transposed
//               from the standard bit order due to erratum RP2350-E6.
#define RVCSR_PMPCFG3_R12_X_RESET  _u(0x0)
#define RVCSR_PMPCFG3_R12_X_BITS   _u(0x00000001)
#define RVCSR_PMPCFG3_R12_X_MSB    _u(0)
#define RVCSR_PMPCFG3_R12_X_LSB    _u(0)
#define RVCSR_PMPCFG3_R12_X_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_PMPADDR0
// Description : Physical memory protection address for region 0. Note all PMP
//               addresses are in units of four bytes.
#define RVCSR_PMPADDR0_OFFSET _u(0x000003b0)
#define RVCSR_PMPADDR0_BITS   _u(0x3fffffff)
#define RVCSR_PMPADDR0_RESET  _u(0x00000000)
#define RVCSR_PMPADDR0_MSB    _u(29)
#define RVCSR_PMPADDR0_LSB    _u(0)
#define RVCSR_PMPADDR0_ACCESS "RW"
// =============================================================================
// Register    : RVCSR_PMPADDR1
// Description : Physical memory protection address for region 1. Note all PMP
//               addresses are in units of four bytes.
#define RVCSR_PMPADDR1_OFFSET _u(0x000003b1)
#define RVCSR_PMPADDR1_BITS   _u(0x3fffffff)
#define RVCSR_PMPADDR1_RESET  _u(0x00000000)
#define RVCSR_PMPADDR1_MSB    _u(29)
#define RVCSR_PMPADDR1_LSB    _u(0)
#define RVCSR_PMPADDR1_ACCESS "RW"
// =============================================================================
// Register    : RVCSR_PMPADDR2
// Description : Physical memory protection address for region 2. Note all PMP
//               addresses are in units of four bytes.
#define RVCSR_PMPADDR2_OFFSET _u(0x000003b2)
#define RVCSR_PMPADDR2_BITS   _u(0x3fffffff)
#define RVCSR_PMPADDR2_RESET  _u(0x00000000)
#define RVCSR_PMPADDR2_MSB    _u(29)
#define RVCSR_PMPADDR2_LSB    _u(0)
#define RVCSR_PMPADDR2_ACCESS "RW"
// =============================================================================
// Register    : RVCSR_PMPADDR3
// Description : Physical memory protection address for region 3. Note all PMP
//               addresses are in units of four bytes.
#define RVCSR_PMPADDR3_OFFSET _u(0x000003b3)
#define RVCSR_PMPADDR3_BITS   _u(0x3fffffff)
#define RVCSR_PMPADDR3_RESET  _u(0x00000000)
#define RVCSR_PMPADDR3_MSB    _u(29)
#define RVCSR_PMPADDR3_LSB    _u(0)
#define RVCSR_PMPADDR3_ACCESS "RW"
// =============================================================================
// Register    : RVCSR_PMPADDR4
// Description : Physical memory protection address for region 4. Note all PMP
//               addresses are in units of four bytes.
#define RVCSR_PMPADDR4_OFFSET _u(0x000003b4)
#define RVCSR_PMPADDR4_BITS   _u(0x3fffffff)
#define RVCSR_PMPADDR4_RESET  _u(0x00000000)
#define RVCSR_PMPADDR4_MSB    _u(29)
#define RVCSR_PMPADDR4_LSB    _u(0)
#define RVCSR_PMPADDR4_ACCESS "RW"
// =============================================================================
// Register    : RVCSR_PMPADDR5
// Description : Physical memory protection address for region 5. Note all PMP
//               addresses are in units of four bytes.
#define RVCSR_PMPADDR5_OFFSET _u(0x000003b5)
#define RVCSR_PMPADDR5_BITS   _u(0x3fffffff)
#define RVCSR_PMPADDR5_RESET  _u(0x00000000)
#define RVCSR_PMPADDR5_MSB    _u(29)
#define RVCSR_PMPADDR5_LSB    _u(0)
#define RVCSR_PMPADDR5_ACCESS "RW"
// =============================================================================
// Register    : RVCSR_PMPADDR6
// Description : Physical memory protection address for region 6. Note all PMP
//               addresses are in units of four bytes.
#define RVCSR_PMPADDR6_OFFSET _u(0x000003b6)
#define RVCSR_PMPADDR6_BITS   _u(0x3fffffff)
#define RVCSR_PMPADDR6_RESET  _u(0x00000000)
#define RVCSR_PMPADDR6_MSB    _u(29)
#define RVCSR_PMPADDR6_LSB    _u(0)
#define RVCSR_PMPADDR6_ACCESS "RW"
// =============================================================================
// Register    : RVCSR_PMPADDR7
// Description : Physical memory protection address for region 7. Note all PMP
//               addresses are in units of four bytes.
#define RVCSR_PMPADDR7_OFFSET _u(0x000003b7)
#define RVCSR_PMPADDR7_BITS   _u(0x3fffffff)
#define RVCSR_PMPADDR7_RESET  _u(0x00000000)
#define RVCSR_PMPADDR7_MSB    _u(29)
#define RVCSR_PMPADDR7_LSB    _u(0)
#define RVCSR_PMPADDR7_ACCESS "RW"
// =============================================================================
// Register    : RVCSR_PMPADDR8
// Description : Physical memory protection address for region 8. Note all PMP
//               addresses are in units of four bytes.
//
//               Hardwired to the address range `0x00000000` through
//               `0x0fffffff`, which contains the boot ROM. This range is made
//               accessible to User mode by default. User mode access to this
//               range can be disabled using one of the dynamically configurable
//               PMP regions, or using the permission registers in ACCESSCTRL.
#define RVCSR_PMPADDR8_OFFSET _u(0x000003b8)
#define RVCSR_PMPADDR8_BITS   _u(0x3fffffff)
#define RVCSR_PMPADDR8_RESET  _u(0x01ffffff)
#define RVCSR_PMPADDR8_MSB    _u(29)
#define RVCSR_PMPADDR8_LSB    _u(0)
#define RVCSR_PMPADDR8_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_PMPADDR9
// Description : Physical memory protection address for region 9. Note all PMP
//               addresses are in units of four bytes.
//
//               Hardwired to the address range `0x40000000` through
//               `0x5fffffff`, which contains the system peripherals. This range
//               is made accessible to User mode by default. User mode access to
//               this range can be disabled using one of the dynamically
//               configurable PMP regions, or using the permission registers in
//               ACCESSCTRL.
#define RVCSR_PMPADDR9_OFFSET _u(0x000003b9)
#define RVCSR_PMPADDR9_BITS   _u(0x3fffffff)
#define RVCSR_PMPADDR9_RESET  _u(0x13ffffff)
#define RVCSR_PMPADDR9_MSB    _u(29)
#define RVCSR_PMPADDR9_LSB    _u(0)
#define RVCSR_PMPADDR9_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_PMPADDR10
// Description : Physical memory protection address for region 10. Note all PMP
//               addresses are in units of four bytes.
//
//               Hardwired to the address range `0xd0000000` through
//               `0xdfffffff`, which contains the core-local peripherals (SIO).
//               This range is made accessible to User mode by default. User
//               mode access to this range can be disabled using one of the
//               dynamically configurable PMP regions, or using the permission
//               registers in ACCESSCTRL.
#define RVCSR_PMPADDR10_OFFSET _u(0x000003ba)
#define RVCSR_PMPADDR10_BITS   _u(0x3fffffff)
#define RVCSR_PMPADDR10_RESET  _u(0x35ffffff)
#define RVCSR_PMPADDR10_MSB    _u(29)
#define RVCSR_PMPADDR10_LSB    _u(0)
#define RVCSR_PMPADDR10_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_PMPADDR11
// Description : Physical memory protection address for region 11. Note all PMP
//               addresses are in units of four bytes.
//
//               Hardwired to all-zeroes. This region is not implemented.
#define RVCSR_PMPADDR11_OFFSET _u(0x000003bb)
#define RVCSR_PMPADDR11_BITS   _u(0x3fffffff)
#define RVCSR_PMPADDR11_RESET  _u(0x00000000)
#define RVCSR_PMPADDR11_MSB    _u(29)
#define RVCSR_PMPADDR11_LSB    _u(0)
#define RVCSR_PMPADDR11_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_PMPADDR12
// Description : Physical memory protection address for region 12. Note all PMP
//               addresses are in units of four bytes.
//
//               Hardwired to all-zeroes. This region is not implemented.
#define RVCSR_PMPADDR12_OFFSET _u(0x000003bc)
#define RVCSR_PMPADDR12_BITS   _u(0x3fffffff)
#define RVCSR_PMPADDR12_RESET  _u(0x00000000)
#define RVCSR_PMPADDR12_MSB    _u(29)
#define RVCSR_PMPADDR12_LSB    _u(0)
#define RVCSR_PMPADDR12_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_PMPADDR13
// Description : Physical memory protection address for region 13. Note all PMP
//               addresses are in units of four bytes.
//
//               Hardwired to all-zeroes. This region is not implemented.
#define RVCSR_PMPADDR13_OFFSET _u(0x000003bd)
#define RVCSR_PMPADDR13_BITS   _u(0x3fffffff)
#define RVCSR_PMPADDR13_RESET  _u(0x00000000)
#define RVCSR_PMPADDR13_MSB    _u(29)
#define RVCSR_PMPADDR13_LSB    _u(0)
#define RVCSR_PMPADDR13_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_PMPADDR14
// Description : Physical memory protection address for region 14. Note all PMP
//               addresses are in units of four bytes.
//
//               Hardwired to all-zeroes. This region is not implemented.
#define RVCSR_PMPADDR14_OFFSET _u(0x000003be)
#define RVCSR_PMPADDR14_BITS   _u(0x3fffffff)
#define RVCSR_PMPADDR14_RESET  _u(0x00000000)
#define RVCSR_PMPADDR14_MSB    _u(29)
#define RVCSR_PMPADDR14_LSB    _u(0)
#define RVCSR_PMPADDR14_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_PMPADDR15
// Description : Physical memory protection address for region 15. Note all PMP
//               addresses are in units of four bytes.
//
//               Hardwired to all-zeroes. This region is not implemented.
#define RVCSR_PMPADDR15_OFFSET _u(0x000003bf)
#define RVCSR_PMPADDR15_BITS   _u(0x3fffffff)
#define RVCSR_PMPADDR15_RESET  _u(0x00000000)
#define RVCSR_PMPADDR15_MSB    _u(29)
#define RVCSR_PMPADDR15_LSB    _u(0)
#define RVCSR_PMPADDR15_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_TSELECT
// Description : Select trigger to be configured via `tdata1`/`tdata2`
//
//               On RP2350, four instruction address triggers are implemented,
//               so only the two LSBs of this register are writable.
#define RVCSR_TSELECT_OFFSET _u(0x000007a0)
#define RVCSR_TSELECT_BITS   _u(0x00000003)
#define RVCSR_TSELECT_RESET  _u(0x00000000)
#define RVCSR_TSELECT_MSB    _u(1)
#define RVCSR_TSELECT_LSB    _u(0)
#define RVCSR_TSELECT_ACCESS "RW"
// =============================================================================
// Register    : RVCSR_TDATA1
// Description : Trigger configuration data 1
//
//               Hazard 3 only supports address/data match triggers (type=2) so
//               this register description includes the `mcontrol` fields for
//               this type.
//
//               More precisely, Hazard3 only supports exact instruction address
//               match triggers (hardware breakpoints) so many of this
//               register's fields are hardwired.
#define RVCSR_TDATA1_OFFSET _u(0x000007a1)
#define RVCSR_TDATA1_BITS   _u(0xffffffcf)
#define RVCSR_TDATA1_RESET  _u(0x20000000)
// -----------------------------------------------------------------------------
// Field       : RVCSR_TDATA1_TYPE
// Description : Trigger type. Hardwired to type=2, meaning an address/data
//               match trigger
#define RVCSR_TDATA1_TYPE_RESET  _u(0x2)
#define RVCSR_TDATA1_TYPE_BITS   _u(0xf0000000)
#define RVCSR_TDATA1_TYPE_MSB    _u(31)
#define RVCSR_TDATA1_TYPE_LSB    _u(28)
#define RVCSR_TDATA1_TYPE_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RVCSR_TDATA1_DMODE
// Description : If 0, both Debug and M-mode can write the `tdata` registers at
//               the selected `tselect`.
//
//               If 1, only Debug Mode can write the `tdata` registers at the
//               selected `tselect`. Writes from other modes are ignored.
//
//               This bit is only writable from Debug Mode
#define RVCSR_TDATA1_DMODE_RESET  _u(0x0)
#define RVCSR_TDATA1_DMODE_BITS   _u(0x08000000)
#define RVCSR_TDATA1_DMODE_MSB    _u(27)
#define RVCSR_TDATA1_DMODE_LSB    _u(27)
#define RVCSR_TDATA1_DMODE_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_TDATA1_MASKMAX
// Description : Value of 0 indicates only exact address matches are supported
#define RVCSR_TDATA1_MASKMAX_RESET  _u(0x00)
#define RVCSR_TDATA1_MASKMAX_BITS   _u(0x07e00000)
#define RVCSR_TDATA1_MASKMAX_MSB    _u(26)
#define RVCSR_TDATA1_MASKMAX_LSB    _u(21)
#define RVCSR_TDATA1_MASKMAX_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RVCSR_TDATA1_HIT
// Description : Trigger hit flag. Not implemented, hardwired to 0.
#define RVCSR_TDATA1_HIT_RESET  _u(0x0)
#define RVCSR_TDATA1_HIT_BITS   _u(0x00100000)
#define RVCSR_TDATA1_HIT_MSB    _u(20)
#define RVCSR_TDATA1_HIT_LSB    _u(20)
#define RVCSR_TDATA1_HIT_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RVCSR_TDATA1_SELECT
// Description : Hardwired value of 0 indicates that only address matches are
//               supported, not data matches
#define RVCSR_TDATA1_SELECT_RESET  _u(0x0)
#define RVCSR_TDATA1_SELECT_BITS   _u(0x00080000)
#define RVCSR_TDATA1_SELECT_MSB    _u(19)
#define RVCSR_TDATA1_SELECT_LSB    _u(19)
#define RVCSR_TDATA1_SELECT_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RVCSR_TDATA1_TIMING
// Description : Hardwired value of 0 indicates that trigger fires before the
//               triggering instruction executes, not afterward
#define RVCSR_TDATA1_TIMING_RESET  _u(0x0)
#define RVCSR_TDATA1_TIMING_BITS   _u(0x00040000)
#define RVCSR_TDATA1_TIMING_MSB    _u(18)
#define RVCSR_TDATA1_TIMING_LSB    _u(18)
#define RVCSR_TDATA1_TIMING_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RVCSR_TDATA1_SIZELO
// Description : Hardwired value of 0 indicates that access size matching is not
//               supported
#define RVCSR_TDATA1_SIZELO_RESET  _u(0x0)
#define RVCSR_TDATA1_SIZELO_BITS   _u(0x00030000)
#define RVCSR_TDATA1_SIZELO_MSB    _u(17)
#define RVCSR_TDATA1_SIZELO_LSB    _u(16)
#define RVCSR_TDATA1_SIZELO_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RVCSR_TDATA1_ACTION
// Description : Select action to be taken when the trigger fires.
//               0x0 -> Raise a breakpoint exception, which can be handled by the M-mode exception handler
//               0x1 -> Enter debug mode. This action is only selectable when `tdata1.dmode` is 1.
#define RVCSR_TDATA1_ACTION_RESET  _u(0x0)
#define RVCSR_TDATA1_ACTION_BITS   _u(0x0000f000)
#define RVCSR_TDATA1_ACTION_MSB    _u(15)
#define RVCSR_TDATA1_ACTION_LSB    _u(12)
#define RVCSR_TDATA1_ACTION_ACCESS "RW"
#define RVCSR_TDATA1_ACTION_VALUE_EBREAK _u(0x0)
#define RVCSR_TDATA1_ACTION_VALUE_DEBUG _u(0x1)
// -----------------------------------------------------------------------------
// Field       : RVCSR_TDATA1_CHAIN
// Description : Hardwired to 0 to indicate trigger chaining is not supported.
#define RVCSR_TDATA1_CHAIN_RESET  _u(0x0)
#define RVCSR_TDATA1_CHAIN_BITS   _u(0x00000800)
#define RVCSR_TDATA1_CHAIN_MSB    _u(11)
#define RVCSR_TDATA1_CHAIN_LSB    _u(11)
#define RVCSR_TDATA1_CHAIN_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RVCSR_TDATA1_MATCH
// Description : Hardwired to 0 to indicate match is always on the full address
//               specified by `tdata2`
#define RVCSR_TDATA1_MATCH_RESET  _u(0x0)
#define RVCSR_TDATA1_MATCH_BITS   _u(0x00000780)
#define RVCSR_TDATA1_MATCH_MSB    _u(10)
#define RVCSR_TDATA1_MATCH_LSB    _u(7)
#define RVCSR_TDATA1_MATCH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RVCSR_TDATA1_M
// Description : When set, enable this trigger in M-mode
#define RVCSR_TDATA1_M_RESET  _u(0x0)
#define RVCSR_TDATA1_M_BITS   _u(0x00000040)
#define RVCSR_TDATA1_M_MSB    _u(6)
#define RVCSR_TDATA1_M_LSB    _u(6)
#define RVCSR_TDATA1_M_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_TDATA1_U
// Description : When set, enable this trigger in U-mode
#define RVCSR_TDATA1_U_RESET  _u(0x0)
#define RVCSR_TDATA1_U_BITS   _u(0x00000008)
#define RVCSR_TDATA1_U_MSB    _u(3)
#define RVCSR_TDATA1_U_LSB    _u(3)
#define RVCSR_TDATA1_U_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_TDATA1_EXECUTE
// Description : When set, the trigger fires on the address of an instruction
//               that is executed.
#define RVCSR_TDATA1_EXECUTE_RESET  _u(0x0)
#define RVCSR_TDATA1_EXECUTE_BITS   _u(0x00000004)
#define RVCSR_TDATA1_EXECUTE_MSB    _u(2)
#define RVCSR_TDATA1_EXECUTE_LSB    _u(2)
#define RVCSR_TDATA1_EXECUTE_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_TDATA1_STORE
// Description : Hardwired to 0 to indicate store address/data triggers are not
//               supported
#define RVCSR_TDATA1_STORE_RESET  _u(0x0)
#define RVCSR_TDATA1_STORE_BITS   _u(0x00000002)
#define RVCSR_TDATA1_STORE_MSB    _u(1)
#define RVCSR_TDATA1_STORE_LSB    _u(1)
#define RVCSR_TDATA1_STORE_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RVCSR_TDATA1_LOAD
// Description : Hardwired to 0 to indicate load address/data triggers are not
//               supported
#define RVCSR_TDATA1_LOAD_RESET  _u(0x0)
#define RVCSR_TDATA1_LOAD_BITS   _u(0x00000001)
#define RVCSR_TDATA1_LOAD_MSB    _u(0)
#define RVCSR_TDATA1_LOAD_LSB    _u(0)
#define RVCSR_TDATA1_LOAD_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_TDATA2
// Description : Trigger configuration data 2
//
//               Contains the address for instruction address triggers (hardware
//               breakpoints)
#define RVCSR_TDATA2_OFFSET _u(0x000007a2)
#define RVCSR_TDATA2_BITS   _u(0xffffffff)
#define RVCSR_TDATA2_RESET  _u(0x00000000)
#define RVCSR_TDATA2_MSB    _u(31)
#define RVCSR_TDATA2_LSB    _u(0)
#define RVCSR_TDATA2_ACCESS "RW"
// =============================================================================
// Register    : RVCSR_DCSR
// Description : Debug control and status register. Access outside of Debug Mode
//               will cause an illegal instruction exception.
#define RVCSR_DCSR_OFFSET _u(0x000007b0)
#define RVCSR_DCSR_BITS   _u(0xf0009fc7)
#define RVCSR_DCSR_RESET  _u(0x40000603)
// -----------------------------------------------------------------------------
// Field       : RVCSR_DCSR_XDEBUGVER
// Description : Hardwired to 4: external debug support as per RISC-V 0.13.2
//               debug specification.
#define RVCSR_DCSR_XDEBUGVER_RESET  _u(0x4)
#define RVCSR_DCSR_XDEBUGVER_BITS   _u(0xf0000000)
#define RVCSR_DCSR_XDEBUGVER_MSB    _u(31)
#define RVCSR_DCSR_XDEBUGVER_LSB    _u(28)
#define RVCSR_DCSR_XDEBUGVER_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RVCSR_DCSR_EBREAKM
// Description : When 1, `ebreak` instructions executed in M-mode will break to
//               Debug Mode instead of trapping
#define RVCSR_DCSR_EBREAKM_RESET  _u(0x0)
#define RVCSR_DCSR_EBREAKM_BITS   _u(0x00008000)
#define RVCSR_DCSR_EBREAKM_MSB    _u(15)
#define RVCSR_DCSR_EBREAKM_LSB    _u(15)
#define RVCSR_DCSR_EBREAKM_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_DCSR_EBREAKU
// Description : When 1, `ebreak` instructions executed in U-mode will break to
//               Debug Mode instead of trapping.
#define RVCSR_DCSR_EBREAKU_RESET  _u(0x0)
#define RVCSR_DCSR_EBREAKU_BITS   _u(0x00001000)
#define RVCSR_DCSR_EBREAKU_MSB    _u(12)
#define RVCSR_DCSR_EBREAKU_LSB    _u(12)
#define RVCSR_DCSR_EBREAKU_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_DCSR_STEPIE
// Description : Hardwired to 0: no interrupts are taken during hardware single-
//               stepping.
#define RVCSR_DCSR_STEPIE_RESET  _u(0x0)
#define RVCSR_DCSR_STEPIE_BITS   _u(0x00000800)
#define RVCSR_DCSR_STEPIE_MSB    _u(11)
#define RVCSR_DCSR_STEPIE_LSB    _u(11)
#define RVCSR_DCSR_STEPIE_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RVCSR_DCSR_STOPCOUNT
// Description : Hardwired to 1: `mcycle`/`mcycleh` and `minstret`/`minstreth`
//               do not increment in Debug Mode.
#define RVCSR_DCSR_STOPCOUNT_RESET  _u(0x1)
#define RVCSR_DCSR_STOPCOUNT_BITS   _u(0x00000400)
#define RVCSR_DCSR_STOPCOUNT_MSB    _u(10)
#define RVCSR_DCSR_STOPCOUNT_LSB    _u(10)
#define RVCSR_DCSR_STOPCOUNT_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RVCSR_DCSR_STOPTIME
// Description : Hardwired to 1: core-local timers don't increment in debug
//               mode. External timers (e.g. hart-shared) may be configured to
//               ignore this.
#define RVCSR_DCSR_STOPTIME_RESET  _u(0x1)
#define RVCSR_DCSR_STOPTIME_BITS   _u(0x00000200)
#define RVCSR_DCSR_STOPTIME_MSB    _u(9)
#define RVCSR_DCSR_STOPTIME_LSB    _u(9)
#define RVCSR_DCSR_STOPTIME_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RVCSR_DCSR_CAUSE
// Description : Set by hardware when entering debug mode.
//               0x1 -> An ebreak instruction was executed when the relevant `dcsr.ebreakx` bit was set.
//               0x2 -> The trigger module caused a breakpoint exception.
//               0x3 -> Processor entered Debug Mode due to a halt request, or a reset-halt request present when the core reset was released.
//               0x4 -> Processor entered Debug Mode after executing one instruction with single-stepping enabled.
#define RVCSR_DCSR_CAUSE_RESET  _u(0x0)
#define RVCSR_DCSR_CAUSE_BITS   _u(0x000001c0)
#define RVCSR_DCSR_CAUSE_MSB    _u(8)
#define RVCSR_DCSR_CAUSE_LSB    _u(6)
#define RVCSR_DCSR_CAUSE_ACCESS "RO"
#define RVCSR_DCSR_CAUSE_VALUE_EBREAK _u(0x1)
#define RVCSR_DCSR_CAUSE_VALUE_TRIGGER _u(0x2)
#define RVCSR_DCSR_CAUSE_VALUE_HALTREQ _u(0x3)
#define RVCSR_DCSR_CAUSE_VALUE_STEP _u(0x4)
// -----------------------------------------------------------------------------
// Field       : RVCSR_DCSR_STEP
// Description : When 1, re-enter Debug Mode after each instruction executed in
//               M-mode or U-mode.
#define RVCSR_DCSR_STEP_RESET  _u(0x0)
#define RVCSR_DCSR_STEP_BITS   _u(0x00000004)
#define RVCSR_DCSR_STEP_MSB    _u(2)
#define RVCSR_DCSR_STEP_LSB    _u(2)
#define RVCSR_DCSR_STEP_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_DCSR_PRV
// Description : Read the privilege mode the core was in when entering Debug
//               Mode, and set the privilege mode the core will execute in when
//               returning from Debug Mode.
#define RVCSR_DCSR_PRV_RESET  _u(0x3)
#define RVCSR_DCSR_PRV_BITS   _u(0x00000003)
#define RVCSR_DCSR_PRV_MSB    _u(1)
#define RVCSR_DCSR_PRV_LSB    _u(0)
#define RVCSR_DCSR_PRV_ACCESS "RW"
// =============================================================================
// Register    : RVCSR_DPC
// Description : Debug program counter. When entering Debug Mode, `dpc` samples
//               the current program counter, e.g. the address of an `ebreak`
//               which caused Debug Mode entry. When leaving debug mode, the
//               processor jumps to `dpc`. The host may read/write this register
//               whilst in Debug Mode.
#define RVCSR_DPC_OFFSET _u(0x000007b1)
#define RVCSR_DPC_BITS   _u(0xfffffffe)
#define RVCSR_DPC_RESET  _u(0x00000000)
#define RVCSR_DPC_MSB    _u(31)
#define RVCSR_DPC_LSB    _u(1)
#define RVCSR_DPC_ACCESS "RW"
// =============================================================================
// Register    : RVCSR_MCYCLE
// Description : Machine-mode cycle counter, low half
//               Counts up once per cycle, when `mcountinhibit.cy` is 0.
//               Disabled by default to save power.
#define RVCSR_MCYCLE_OFFSET _u(0x00000b00)
#define RVCSR_MCYCLE_BITS   _u(0xffffffff)
#define RVCSR_MCYCLE_RESET  _u(0x00000000)
#define RVCSR_MCYCLE_MSB    _u(31)
#define RVCSR_MCYCLE_LSB    _u(0)
#define RVCSR_MCYCLE_ACCESS "RW"
// =============================================================================
// Register    : RVCSR_MINSTRET
// Description : Machine-mode instruction retire counter, low half
//               Counts up once per instruction, when `mcountinhibit.ir` is 0.
//               Disabled by default to save power.
#define RVCSR_MINSTRET_OFFSET _u(0x00000b02)
#define RVCSR_MINSTRET_BITS   _u(0xffffffff)
#define RVCSR_MINSTRET_RESET  _u(0x00000000)
#define RVCSR_MINSTRET_MSB    _u(31)
#define RVCSR_MINSTRET_LSB    _u(0)
#define RVCSR_MINSTRET_ACCESS "RW"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER3
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER3_OFFSET _u(0x00000b03)
#define RVCSR_MHPMCOUNTER3_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER3_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER3_MSB    _u(31)
#define RVCSR_MHPMCOUNTER3_LSB    _u(0)
#define RVCSR_MHPMCOUNTER3_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER4
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER4_OFFSET _u(0x00000b04)
#define RVCSR_MHPMCOUNTER4_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER4_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER4_MSB    _u(31)
#define RVCSR_MHPMCOUNTER4_LSB    _u(0)
#define RVCSR_MHPMCOUNTER4_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER5
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER5_OFFSET _u(0x00000b05)
#define RVCSR_MHPMCOUNTER5_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER5_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER5_MSB    _u(31)
#define RVCSR_MHPMCOUNTER5_LSB    _u(0)
#define RVCSR_MHPMCOUNTER5_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER6
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER6_OFFSET _u(0x00000b06)
#define RVCSR_MHPMCOUNTER6_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER6_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER6_MSB    _u(31)
#define RVCSR_MHPMCOUNTER6_LSB    _u(0)
#define RVCSR_MHPMCOUNTER6_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER7
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER7_OFFSET _u(0x00000b07)
#define RVCSR_MHPMCOUNTER7_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER7_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER7_MSB    _u(31)
#define RVCSR_MHPMCOUNTER7_LSB    _u(0)
#define RVCSR_MHPMCOUNTER7_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER8
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER8_OFFSET _u(0x00000b08)
#define RVCSR_MHPMCOUNTER8_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER8_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER8_MSB    _u(31)
#define RVCSR_MHPMCOUNTER8_LSB    _u(0)
#define RVCSR_MHPMCOUNTER8_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER9
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER9_OFFSET _u(0x00000b09)
#define RVCSR_MHPMCOUNTER9_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER9_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER9_MSB    _u(31)
#define RVCSR_MHPMCOUNTER9_LSB    _u(0)
#define RVCSR_MHPMCOUNTER9_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER10
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER10_OFFSET _u(0x00000b0a)
#define RVCSR_MHPMCOUNTER10_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER10_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER10_MSB    _u(31)
#define RVCSR_MHPMCOUNTER10_LSB    _u(0)
#define RVCSR_MHPMCOUNTER10_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER11
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER11_OFFSET _u(0x00000b0b)
#define RVCSR_MHPMCOUNTER11_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER11_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER11_MSB    _u(31)
#define RVCSR_MHPMCOUNTER11_LSB    _u(0)
#define RVCSR_MHPMCOUNTER11_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER12
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER12_OFFSET _u(0x00000b0c)
#define RVCSR_MHPMCOUNTER12_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER12_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER12_MSB    _u(31)
#define RVCSR_MHPMCOUNTER12_LSB    _u(0)
#define RVCSR_MHPMCOUNTER12_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER13
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER13_OFFSET _u(0x00000b0d)
#define RVCSR_MHPMCOUNTER13_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER13_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER13_MSB    _u(31)
#define RVCSR_MHPMCOUNTER13_LSB    _u(0)
#define RVCSR_MHPMCOUNTER13_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER14
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER14_OFFSET _u(0x00000b0e)
#define RVCSR_MHPMCOUNTER14_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER14_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER14_MSB    _u(31)
#define RVCSR_MHPMCOUNTER14_LSB    _u(0)
#define RVCSR_MHPMCOUNTER14_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER15
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER15_OFFSET _u(0x00000b0f)
#define RVCSR_MHPMCOUNTER15_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER15_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER15_MSB    _u(31)
#define RVCSR_MHPMCOUNTER15_LSB    _u(0)
#define RVCSR_MHPMCOUNTER15_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER16
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER16_OFFSET _u(0x00000b10)
#define RVCSR_MHPMCOUNTER16_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER16_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER16_MSB    _u(31)
#define RVCSR_MHPMCOUNTER16_LSB    _u(0)
#define RVCSR_MHPMCOUNTER16_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER17
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER17_OFFSET _u(0x00000b11)
#define RVCSR_MHPMCOUNTER17_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER17_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER17_MSB    _u(31)
#define RVCSR_MHPMCOUNTER17_LSB    _u(0)
#define RVCSR_MHPMCOUNTER17_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER18
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER18_OFFSET _u(0x00000b12)
#define RVCSR_MHPMCOUNTER18_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER18_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER18_MSB    _u(31)
#define RVCSR_MHPMCOUNTER18_LSB    _u(0)
#define RVCSR_MHPMCOUNTER18_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER19
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER19_OFFSET _u(0x00000b13)
#define RVCSR_MHPMCOUNTER19_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER19_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER19_MSB    _u(31)
#define RVCSR_MHPMCOUNTER19_LSB    _u(0)
#define RVCSR_MHPMCOUNTER19_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER20
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER20_OFFSET _u(0x00000b14)
#define RVCSR_MHPMCOUNTER20_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER20_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER20_MSB    _u(31)
#define RVCSR_MHPMCOUNTER20_LSB    _u(0)
#define RVCSR_MHPMCOUNTER20_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER21
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER21_OFFSET _u(0x00000b15)
#define RVCSR_MHPMCOUNTER21_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER21_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER21_MSB    _u(31)
#define RVCSR_MHPMCOUNTER21_LSB    _u(0)
#define RVCSR_MHPMCOUNTER21_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER22
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER22_OFFSET _u(0x00000b16)
#define RVCSR_MHPMCOUNTER22_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER22_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER22_MSB    _u(31)
#define RVCSR_MHPMCOUNTER22_LSB    _u(0)
#define RVCSR_MHPMCOUNTER22_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER23
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER23_OFFSET _u(0x00000b17)
#define RVCSR_MHPMCOUNTER23_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER23_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER23_MSB    _u(31)
#define RVCSR_MHPMCOUNTER23_LSB    _u(0)
#define RVCSR_MHPMCOUNTER23_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER24
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER24_OFFSET _u(0x00000b18)
#define RVCSR_MHPMCOUNTER24_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER24_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER24_MSB    _u(31)
#define RVCSR_MHPMCOUNTER24_LSB    _u(0)
#define RVCSR_MHPMCOUNTER24_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER25
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER25_OFFSET _u(0x00000b19)
#define RVCSR_MHPMCOUNTER25_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER25_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER25_MSB    _u(31)
#define RVCSR_MHPMCOUNTER25_LSB    _u(0)
#define RVCSR_MHPMCOUNTER25_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER26
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER26_OFFSET _u(0x00000b1a)
#define RVCSR_MHPMCOUNTER26_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER26_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER26_MSB    _u(31)
#define RVCSR_MHPMCOUNTER26_LSB    _u(0)
#define RVCSR_MHPMCOUNTER26_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER27
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER27_OFFSET _u(0x00000b1b)
#define RVCSR_MHPMCOUNTER27_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER27_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER27_MSB    _u(31)
#define RVCSR_MHPMCOUNTER27_LSB    _u(0)
#define RVCSR_MHPMCOUNTER27_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER28
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER28_OFFSET _u(0x00000b1c)
#define RVCSR_MHPMCOUNTER28_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER28_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER28_MSB    _u(31)
#define RVCSR_MHPMCOUNTER28_LSB    _u(0)
#define RVCSR_MHPMCOUNTER28_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER29
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER29_OFFSET _u(0x00000b1d)
#define RVCSR_MHPMCOUNTER29_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER29_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER29_MSB    _u(31)
#define RVCSR_MHPMCOUNTER29_LSB    _u(0)
#define RVCSR_MHPMCOUNTER29_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER30
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER30_OFFSET _u(0x00000b1e)
#define RVCSR_MHPMCOUNTER30_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER30_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER30_MSB    _u(31)
#define RVCSR_MHPMCOUNTER30_LSB    _u(0)
#define RVCSR_MHPMCOUNTER30_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER31
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER31_OFFSET _u(0x00000b1f)
#define RVCSR_MHPMCOUNTER31_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER31_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER31_MSB    _u(31)
#define RVCSR_MHPMCOUNTER31_LSB    _u(0)
#define RVCSR_MHPMCOUNTER31_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MCYCLEH
// Description : Machine-mode cycle counter, high half
//               Counts up once per 1 << 32 cycles, when `mcountinhibit.cy` is
//               0. Disabled by default to save power.
#define RVCSR_MCYCLEH_OFFSET _u(0x00000b80)
#define RVCSR_MCYCLEH_BITS   _u(0xffffffff)
#define RVCSR_MCYCLEH_RESET  _u(0x00000000)
#define RVCSR_MCYCLEH_MSB    _u(31)
#define RVCSR_MCYCLEH_LSB    _u(0)
#define RVCSR_MCYCLEH_ACCESS "RW"
// =============================================================================
// Register    : RVCSR_MINSTRETH
// Description : Machine-mode instruction retire counter, low half
//               Counts up once per 1 << 32 instructions, when
//               `mcountinhibit.ir` is 0. Disabled by default to save power.
#define RVCSR_MINSTRETH_OFFSET _u(0x00000b82)
#define RVCSR_MINSTRETH_BITS   _u(0xffffffff)
#define RVCSR_MINSTRETH_RESET  _u(0x00000000)
#define RVCSR_MINSTRETH_MSB    _u(31)
#define RVCSR_MINSTRETH_LSB    _u(0)
#define RVCSR_MINSTRETH_ACCESS "RW"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER3H
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER3H_OFFSET _u(0x00000b83)
#define RVCSR_MHPMCOUNTER3H_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER3H_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER3H_MSB    _u(31)
#define RVCSR_MHPMCOUNTER3H_LSB    _u(0)
#define RVCSR_MHPMCOUNTER3H_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER4H
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER4H_OFFSET _u(0x00000b84)
#define RVCSR_MHPMCOUNTER4H_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER4H_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER4H_MSB    _u(31)
#define RVCSR_MHPMCOUNTER4H_LSB    _u(0)
#define RVCSR_MHPMCOUNTER4H_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER5H
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER5H_OFFSET _u(0x00000b85)
#define RVCSR_MHPMCOUNTER5H_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER5H_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER5H_MSB    _u(31)
#define RVCSR_MHPMCOUNTER5H_LSB    _u(0)
#define RVCSR_MHPMCOUNTER5H_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER6H
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER6H_OFFSET _u(0x00000b86)
#define RVCSR_MHPMCOUNTER6H_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER6H_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER6H_MSB    _u(31)
#define RVCSR_MHPMCOUNTER6H_LSB    _u(0)
#define RVCSR_MHPMCOUNTER6H_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER7H
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER7H_OFFSET _u(0x00000b87)
#define RVCSR_MHPMCOUNTER7H_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER7H_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER7H_MSB    _u(31)
#define RVCSR_MHPMCOUNTER7H_LSB    _u(0)
#define RVCSR_MHPMCOUNTER7H_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER8H
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER8H_OFFSET _u(0x00000b88)
#define RVCSR_MHPMCOUNTER8H_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER8H_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER8H_MSB    _u(31)
#define RVCSR_MHPMCOUNTER8H_LSB    _u(0)
#define RVCSR_MHPMCOUNTER8H_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER9H
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER9H_OFFSET _u(0x00000b89)
#define RVCSR_MHPMCOUNTER9H_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER9H_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER9H_MSB    _u(31)
#define RVCSR_MHPMCOUNTER9H_LSB    _u(0)
#define RVCSR_MHPMCOUNTER9H_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER10H
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER10H_OFFSET _u(0x00000b8a)
#define RVCSR_MHPMCOUNTER10H_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER10H_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER10H_MSB    _u(31)
#define RVCSR_MHPMCOUNTER10H_LSB    _u(0)
#define RVCSR_MHPMCOUNTER10H_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER11H
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER11H_OFFSET _u(0x00000b8b)
#define RVCSR_MHPMCOUNTER11H_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER11H_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER11H_MSB    _u(31)
#define RVCSR_MHPMCOUNTER11H_LSB    _u(0)
#define RVCSR_MHPMCOUNTER11H_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER12H
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER12H_OFFSET _u(0x00000b8c)
#define RVCSR_MHPMCOUNTER12H_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER12H_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER12H_MSB    _u(31)
#define RVCSR_MHPMCOUNTER12H_LSB    _u(0)
#define RVCSR_MHPMCOUNTER12H_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER13H
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER13H_OFFSET _u(0x00000b8d)
#define RVCSR_MHPMCOUNTER13H_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER13H_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER13H_MSB    _u(31)
#define RVCSR_MHPMCOUNTER13H_LSB    _u(0)
#define RVCSR_MHPMCOUNTER13H_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER14H
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER14H_OFFSET _u(0x00000b8e)
#define RVCSR_MHPMCOUNTER14H_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER14H_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER14H_MSB    _u(31)
#define RVCSR_MHPMCOUNTER14H_LSB    _u(0)
#define RVCSR_MHPMCOUNTER14H_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER15H
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER15H_OFFSET _u(0x00000b8f)
#define RVCSR_MHPMCOUNTER15H_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER15H_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER15H_MSB    _u(31)
#define RVCSR_MHPMCOUNTER15H_LSB    _u(0)
#define RVCSR_MHPMCOUNTER15H_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER16H
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER16H_OFFSET _u(0x00000b90)
#define RVCSR_MHPMCOUNTER16H_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER16H_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER16H_MSB    _u(31)
#define RVCSR_MHPMCOUNTER16H_LSB    _u(0)
#define RVCSR_MHPMCOUNTER16H_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER17H
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER17H_OFFSET _u(0x00000b91)
#define RVCSR_MHPMCOUNTER17H_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER17H_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER17H_MSB    _u(31)
#define RVCSR_MHPMCOUNTER17H_LSB    _u(0)
#define RVCSR_MHPMCOUNTER17H_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER18H
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER18H_OFFSET _u(0x00000b92)
#define RVCSR_MHPMCOUNTER18H_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER18H_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER18H_MSB    _u(31)
#define RVCSR_MHPMCOUNTER18H_LSB    _u(0)
#define RVCSR_MHPMCOUNTER18H_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER19H
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER19H_OFFSET _u(0x00000b93)
#define RVCSR_MHPMCOUNTER19H_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER19H_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER19H_MSB    _u(31)
#define RVCSR_MHPMCOUNTER19H_LSB    _u(0)
#define RVCSR_MHPMCOUNTER19H_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER20H
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER20H_OFFSET _u(0x00000b94)
#define RVCSR_MHPMCOUNTER20H_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER20H_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER20H_MSB    _u(31)
#define RVCSR_MHPMCOUNTER20H_LSB    _u(0)
#define RVCSR_MHPMCOUNTER20H_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER21H
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER21H_OFFSET _u(0x00000b95)
#define RVCSR_MHPMCOUNTER21H_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER21H_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER21H_MSB    _u(31)
#define RVCSR_MHPMCOUNTER21H_LSB    _u(0)
#define RVCSR_MHPMCOUNTER21H_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER22H
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER22H_OFFSET _u(0x00000b96)
#define RVCSR_MHPMCOUNTER22H_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER22H_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER22H_MSB    _u(31)
#define RVCSR_MHPMCOUNTER22H_LSB    _u(0)
#define RVCSR_MHPMCOUNTER22H_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER23H
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER23H_OFFSET _u(0x00000b97)
#define RVCSR_MHPMCOUNTER23H_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER23H_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER23H_MSB    _u(31)
#define RVCSR_MHPMCOUNTER23H_LSB    _u(0)
#define RVCSR_MHPMCOUNTER23H_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER24H
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER24H_OFFSET _u(0x00000b98)
#define RVCSR_MHPMCOUNTER24H_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER24H_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER24H_MSB    _u(31)
#define RVCSR_MHPMCOUNTER24H_LSB    _u(0)
#define RVCSR_MHPMCOUNTER24H_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER25H
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER25H_OFFSET _u(0x00000b99)
#define RVCSR_MHPMCOUNTER25H_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER25H_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER25H_MSB    _u(31)
#define RVCSR_MHPMCOUNTER25H_LSB    _u(0)
#define RVCSR_MHPMCOUNTER25H_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER26H
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER26H_OFFSET _u(0x00000b9a)
#define RVCSR_MHPMCOUNTER26H_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER26H_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER26H_MSB    _u(31)
#define RVCSR_MHPMCOUNTER26H_LSB    _u(0)
#define RVCSR_MHPMCOUNTER26H_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER27H
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER27H_OFFSET _u(0x00000b9b)
#define RVCSR_MHPMCOUNTER27H_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER27H_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER27H_MSB    _u(31)
#define RVCSR_MHPMCOUNTER27H_LSB    _u(0)
#define RVCSR_MHPMCOUNTER27H_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER28H
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER28H_OFFSET _u(0x00000b9c)
#define RVCSR_MHPMCOUNTER28H_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER28H_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER28H_MSB    _u(31)
#define RVCSR_MHPMCOUNTER28H_LSB    _u(0)
#define RVCSR_MHPMCOUNTER28H_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER29H
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER29H_OFFSET _u(0x00000b9d)
#define RVCSR_MHPMCOUNTER29H_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER29H_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER29H_MSB    _u(31)
#define RVCSR_MHPMCOUNTER29H_LSB    _u(0)
#define RVCSR_MHPMCOUNTER29H_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER30H
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER30H_OFFSET _u(0x00000b9e)
#define RVCSR_MHPMCOUNTER30H_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER30H_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER30H_MSB    _u(31)
#define RVCSR_MHPMCOUNTER30H_LSB    _u(0)
#define RVCSR_MHPMCOUNTER30H_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHPMCOUNTER31H
// Description : Extended performance counter, hardwired to 0.
#define RVCSR_MHPMCOUNTER31H_OFFSET _u(0x00000b9f)
#define RVCSR_MHPMCOUNTER31H_BITS   _u(0xffffffff)
#define RVCSR_MHPMCOUNTER31H_RESET  _u(0x00000000)
#define RVCSR_MHPMCOUNTER31H_MSB    _u(31)
#define RVCSR_MHPMCOUNTER31H_LSB    _u(0)
#define RVCSR_MHPMCOUNTER31H_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_PMPCFGM0
// Description : PMP M-mode configuration. One bit per PMP region. Setting a bit
//               makes the corresponding region apply to M-mode (like the
//               `pmpcfg.L` bit) but does not lock the region.
//
//               PMP is useful for non-security-related purposes, such as stack
//               guarding and peripheral emulation. This extension allows M-mode
//               to freely use any currently unlocked regions for its own
//               purposes, without the inconvenience of having to lock them.
//
//               Note that this does not grant any new capabilities to M-mode,
//               since in the base standard it is already possible to apply
//               unlocked regions to M-mode by locking them. In general, PMP
//               regions should be locked in ascending region number order so
//               they can't be subsequently overridden by currently unlocked
//               regions.
//
//               Note also that this is not the same as the rule locking bypass
//               bit in the ePMP extension, which does not permit locked and
//               unlocked M-mode regions to coexist.
//
//               This is a Hazard3 custom CSR.
#define RVCSR_PMPCFGM0_OFFSET _u(0x00000bd0)
#define RVCSR_PMPCFGM0_BITS   _u(0x0000ffff)
#define RVCSR_PMPCFGM0_RESET  _u(0x00000000)
#define RVCSR_PMPCFGM0_MSB    _u(15)
#define RVCSR_PMPCFGM0_LSB    _u(0)
#define RVCSR_PMPCFGM0_ACCESS "RW"
// =============================================================================
// Register    : RVCSR_MEIEA
// Description : External interrupt enable array.
//
//               The array contains a read-write bit for each external interrupt
//               request: a `1` bit indicates that interrupt is currently
//               enabled. At reset, all external interrupts are disabled.
//
//               If enabled, an external interrupt can cause assertion of the
//               standard RISC-V machine external interrupt pending flag
//               (`mip.meip`), and therefore cause the processor to enter the
//               external interrupt vector. See `meipa`.
//
//               There are up to 512 external interrupts. The upper half of this
//               register contains a 16-bit window into the full 512-bit vector.
//               The window is indexed by the 5 LSBs of the write data.
#define RVCSR_MEIEA_OFFSET _u(0x00000be0)
#define RVCSR_MEIEA_BITS   _u(0xffff001f)
#define RVCSR_MEIEA_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : RVCSR_MEIEA_WINDOW
// Description : 16-bit read/write window into the external interrupt enable
//               array
#define RVCSR_MEIEA_WINDOW_RESET  _u(0x0000)
#define RVCSR_MEIEA_WINDOW_BITS   _u(0xffff0000)
#define RVCSR_MEIEA_WINDOW_MSB    _u(31)
#define RVCSR_MEIEA_WINDOW_LSB    _u(16)
#define RVCSR_MEIEA_WINDOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_MEIEA_INDEX
// Description : Write-only self-clearing field (no value is stored) used to
//               control which window of the array appears in `window`.
#define RVCSR_MEIEA_INDEX_RESET  _u(0x00)
#define RVCSR_MEIEA_INDEX_BITS   _u(0x0000001f)
#define RVCSR_MEIEA_INDEX_MSB    _u(4)
#define RVCSR_MEIEA_INDEX_LSB    _u(0)
#define RVCSR_MEIEA_INDEX_ACCESS "WO"
// =============================================================================
// Register    : RVCSR_MEIPA
// Description : External interrupt pending array
//
//               Contains a read-only bit for each external interrupt request.
//               Similarly to `meiea`, this register is a window into an array
//               of up to 512 external interrupt flags. The status appears in
//               the upper 16 bits of the value read from `meipa`, and the lower
//               5 bits of the value _written_ by the same CSR instruction (or 0
//               if no write takes place) select a 16-bit window of the full
//               interrupt pending array.
//
//               A `1` bit indicates that interrupt is currently asserted. IRQs
//               are assumed to be level-sensitive, and the relevant `meipa` bit
//               is cleared by servicing the requestor so that it deasserts its
//               interrupt request.
//
//               When any interrupt of sufficient priority is both set in
//               `meipa` and enabled in `meiea`, the standard RISC-V external
//               interrupt pending bit `mip.meip` is asserted. In other words,
//               `meipa` is filtered by `meiea` to generate the standard
//               `mip.meip` flag.
#define RVCSR_MEIPA_OFFSET _u(0x00000be1)
#define RVCSR_MEIPA_BITS   _u(0xffff001f)
#define RVCSR_MEIPA_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : RVCSR_MEIPA_WINDOW
// Description : 16-bit read-only window into the external interrupt pending
//               array
#define RVCSR_MEIPA_WINDOW_RESET  "-"
#define RVCSR_MEIPA_WINDOW_BITS   _u(0xffff0000)
#define RVCSR_MEIPA_WINDOW_MSB    _u(31)
#define RVCSR_MEIPA_WINDOW_LSB    _u(16)
#define RVCSR_MEIPA_WINDOW_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RVCSR_MEIPA_INDEX
// Description : Write-only, self-clearing field (no value is stored) used to
//               control which window of the array appears in `window`.
#define RVCSR_MEIPA_INDEX_RESET  _u(0x00)
#define RVCSR_MEIPA_INDEX_BITS   _u(0x0000001f)
#define RVCSR_MEIPA_INDEX_MSB    _u(4)
#define RVCSR_MEIPA_INDEX_LSB    _u(0)
#define RVCSR_MEIPA_INDEX_ACCESS "WO"
// =============================================================================
// Register    : RVCSR_MEIFA
// Description : External interrupt force array
//
//               Contains a read-write bit for every interrupt request. Writing
//               a 1 to a bit in the interrupt force array causes the
//               corresponding bit to become pending in `meipa`. Software can
//               use this feature to manually trigger a particular interrupt.
//
//               There are no restrictions on using `meifa` inside of an
//               interrupt. The more useful case here is to schedule some lower-
//               priority handler from within a high-priority interrupt, so that
//               it will execute before the core returns to the foreground code.
//               Implementers may wish to reserve some external IRQs with their
//               external inputs tied to 0 for this purpose.
//
//               Bits can be cleared by software, and are cleared automatically
//               by hardware upon a read of `meinext` which returns the
//               corresponding IRQ number in `meinext.irq` with `mienext.noirq`
//               clear (no matter whether `meinext.update` is written).
//
//               `meifa` implements the same array window indexing scheme as
//               `meiea` and `meipa`.
#define RVCSR_MEIFA_OFFSET _u(0x00000be2)
#define RVCSR_MEIFA_BITS   _u(0xffff001f)
#define RVCSR_MEIFA_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : RVCSR_MEIFA_WINDOW
// Description : 16-bit read/write window into the external interrupt force
//               array
#define RVCSR_MEIFA_WINDOW_RESET  _u(0x0000)
#define RVCSR_MEIFA_WINDOW_BITS   _u(0xffff0000)
#define RVCSR_MEIFA_WINDOW_MSB    _u(31)
#define RVCSR_MEIFA_WINDOW_LSB    _u(16)
#define RVCSR_MEIFA_WINDOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_MEIFA_INDEX
// Description : Write-only, self-clearing field (no value is stored) used to
//               control which window of the array appears in `window`.
#define RVCSR_MEIFA_INDEX_RESET  _u(0x00)
#define RVCSR_MEIFA_INDEX_BITS   _u(0x0000001f)
#define RVCSR_MEIFA_INDEX_MSB    _u(4)
#define RVCSR_MEIFA_INDEX_LSB    _u(0)
#define RVCSR_MEIFA_INDEX_ACCESS "WO"
// =============================================================================
// Register    : RVCSR_MEIPRA
// Description : External interrupt priority array
//
//               Each interrupt has an (up to) 4-bit priority value associated
//               with it, and each access to this register reads and/or writes a
//               16-bit window containing four such priority values. When less
//               than 16 priority levels are available, the LSBs of the priority
//               fields are hardwired to 0.
//
//               When an interrupt's priority is lower than the current
//               preemption priority `meicontext.preempt`, it is treated as not
//               being pending for the purposes of `mip.meip`. The pending bit
//               in `meipa` will still assert, but the machine external
//               interrupt pending bit `mip.meip` will not, so the processor
//               will ignore this interrupt. See `meicontext`.
#define RVCSR_MEIPRA_OFFSET _u(0x00000be3)
#define RVCSR_MEIPRA_BITS   _u(0xffff001f)
#define RVCSR_MEIPRA_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : RVCSR_MEIPRA_WINDOW
// Description : 16-bit read/write window into the external interrupt priority
//               array, containing four 4-bit priority values.
#define RVCSR_MEIPRA_WINDOW_RESET  _u(0x0000)
#define RVCSR_MEIPRA_WINDOW_BITS   _u(0xffff0000)
#define RVCSR_MEIPRA_WINDOW_MSB    _u(31)
#define RVCSR_MEIPRA_WINDOW_LSB    _u(16)
#define RVCSR_MEIPRA_WINDOW_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_MEIPRA_INDEX
// Description : Write-only, self-clearing field (no value is stored) used to
//               control which window of the array appears in `window`.
#define RVCSR_MEIPRA_INDEX_RESET  _u(0x00)
#define RVCSR_MEIPRA_INDEX_BITS   _u(0x0000001f)
#define RVCSR_MEIPRA_INDEX_MSB    _u(4)
#define RVCSR_MEIPRA_INDEX_LSB    _u(0)
#define RVCSR_MEIPRA_INDEX_ACCESS "WO"
// =============================================================================
// Register    : RVCSR_MEINEXT
// Description : Get next external interrupt
//
//               Contains the index of the highest-priority external interrupt
//               which is both asserted in `meipa` and enabled in `meiea`, left-
//               shifted by 2 so that it can be used to index an array of 32-bit
//               function pointers. If there is no such interrupt, the MSB is
//               set.
//
//               When multiple interrupts of the same priority are both pending
//               and enabled, the lowest-numbered wins. Interrupts with priority
//               less than `meicontext.ppreempt` -- the _previous_ preemption
//               priority -- are treated as though they are not pending. This is
//               to ensure that a preempting interrupt frame does not service
//               interrupts which may be in progress in the frame that was
//               preempted.
#define RVCSR_MEINEXT_OFFSET _u(0x00000be4)
#define RVCSR_MEINEXT_BITS   _u(0x800007fd)
#define RVCSR_MEINEXT_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : RVCSR_MEINEXT_NOIRQ
// Description : Set when there is no external interrupt which is enabled,
//               pending, and has priority greater than or equal to
//               `meicontext.ppreempt`. Can be efficiently tested with a `bltz`
//               or `bgez` instruction.
#define RVCSR_MEINEXT_NOIRQ_RESET  _u(0x0)
#define RVCSR_MEINEXT_NOIRQ_BITS   _u(0x80000000)
#define RVCSR_MEINEXT_NOIRQ_MSB    _u(31)
#define RVCSR_MEINEXT_NOIRQ_LSB    _u(31)
#define RVCSR_MEINEXT_NOIRQ_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RVCSR_MEINEXT_IRQ
// Description : Index of the highest-priority active external interrupt. Zero
//               when no external interrupts with sufficient priority are both
//               pending and enabled.
#define RVCSR_MEINEXT_IRQ_RESET  _u(0x000)
#define RVCSR_MEINEXT_IRQ_BITS   _u(0x000007fc)
#define RVCSR_MEINEXT_IRQ_MSB    _u(10)
#define RVCSR_MEINEXT_IRQ_LSB    _u(2)
#define RVCSR_MEINEXT_IRQ_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RVCSR_MEINEXT_UPDATE
// Description : Writing 1 (self-clearing) causes hardware to update
//               `meicontext` according to the IRQ number and preemption
//               priority of the interrupt indicated in `noirq`/`irq`. This
//               should be done in a single atomic operation, i.e. `csrrsi a0,
//               meinext, 0x1`.
#define RVCSR_MEINEXT_UPDATE_RESET  _u(0x0)
#define RVCSR_MEINEXT_UPDATE_BITS   _u(0x00000001)
#define RVCSR_MEINEXT_UPDATE_MSB    _u(0)
#define RVCSR_MEINEXT_UPDATE_LSB    _u(0)
#define RVCSR_MEINEXT_UPDATE_ACCESS "SC"
// =============================================================================
// Register    : RVCSR_MEICONTEXT
// Description : External interrupt context register
//
//               Configures the priority level for interrupt preemption, and
//               helps software track which interrupt it is currently in. The
//               latter is useful when a common interrupt service routine
//               handles interrupt requests from multiple instances of the same
//               peripheral.
//
//               A three-level stack of preemption priorities is maintained in
//               the `preempt`, `ppreempt` and `pppreempt` fields. The priority
//               stack is saved when hardware enters the external interrupt
//               vector, and restored by an `mret` instruction if
//               `meicontext.mreteirq` is set.
//
//               The top entry of the priority stack, `preempt`, is used by
//               hardware to ensure that only higher-priority interrupts can
//               preempt the current interrupt. The next entry, `ppreempt`, is
//               used to avoid servicing interrupts which may already be in
//               progress in a frame that was preempted. The third entry,
//               `pppreempt`, has no hardware effect, but ensures that `preempt`
//               and `ppreempt` can be correctly saved/restored across arbitrary
//               levels of preemption.
#define RVCSR_MEICONTEXT_OFFSET _u(0x00000be5)
#define RVCSR_MEICONTEXT_BITS   _u(0xff1f9fff)
#define RVCSR_MEICONTEXT_RESET  _u(0x00008000)
// -----------------------------------------------------------------------------
// Field       : RVCSR_MEICONTEXT_PPPREEMPT
// Description : Previous `ppreempt`. Set to `ppreempt` on priority save, set to
//               zero on priority restore.  Has no hardware effect, but ensures
//               that when `meicontext` is saved/restored correctly, `preempt`
//               and `ppreempt` stack correctly through arbitrarily many
//               preemption frames.
#define RVCSR_MEICONTEXT_PPPREEMPT_RESET  _u(0x0)
#define RVCSR_MEICONTEXT_PPPREEMPT_BITS   _u(0xf0000000)
#define RVCSR_MEICONTEXT_PPPREEMPT_MSB    _u(31)
#define RVCSR_MEICONTEXT_PPPREEMPT_LSB    _u(28)
#define RVCSR_MEICONTEXT_PPPREEMPT_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_MEICONTEXT_PPREEMPT
// Description : Previous `preempt`. Set to `preempt` on priority save, restored
//               to to `pppreempt` on priority restore.
//
//               IRQs of lower priority than `ppreempt` are not visible in
//               `meinext`, so that a preemptee is not re-taken in the
//               preempting frame.
#define RVCSR_MEICONTEXT_PPREEMPT_RESET  _u(0x0)
#define RVCSR_MEICONTEXT_PPREEMPT_BITS   _u(0x0f000000)
#define RVCSR_MEICONTEXT_PPREEMPT_MSB    _u(27)
#define RVCSR_MEICONTEXT_PPREEMPT_LSB    _u(24)
#define RVCSR_MEICONTEXT_PPREEMPT_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_MEICONTEXT_PREEMPT
// Description : Minimum interrupt priority to preempt the current interrupt.
//               Interrupts with lower priority than `preempt` do not cause the
//               core to transfer to an interrupt handler. Updated by hardware
//               when when `meinext.update` is written, or when hardware enters
//               the external interrupt vector.
//
//               If an interrupt is present in `meinext` when this field is
//               updated, then `preempt` is set to one level greater than that
//               interrupt's priority. Otherwise, `ppreempt` is set to one level
//               greater than the maximum interrupt priority, disabling
//               preemption.
#define RVCSR_MEICONTEXT_PREEMPT_RESET  _u(0x00)
#define RVCSR_MEICONTEXT_PREEMPT_BITS   _u(0x001f0000)
#define RVCSR_MEICONTEXT_PREEMPT_MSB    _u(20)
#define RVCSR_MEICONTEXT_PREEMPT_LSB    _u(16)
#define RVCSR_MEICONTEXT_PREEMPT_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_MEICONTEXT_NOIRQ
// Description : Not in interrupt (read/write). Set to 1 at reset. Set to
//               `meinext.noirq` when `meinext.update` is written. No hardware
//               effect.
#define RVCSR_MEICONTEXT_NOIRQ_RESET  _u(0x1)
#define RVCSR_MEICONTEXT_NOIRQ_BITS   _u(0x00008000)
#define RVCSR_MEICONTEXT_NOIRQ_MSB    _u(15)
#define RVCSR_MEICONTEXT_NOIRQ_LSB    _u(15)
#define RVCSR_MEICONTEXT_NOIRQ_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_MEICONTEXT_IRQ
// Description : Current IRQ number (read/write). Set to `meinext.irq` when
//               `meinext.update` is written. No hardware effect.
#define RVCSR_MEICONTEXT_IRQ_RESET  _u(0x000)
#define RVCSR_MEICONTEXT_IRQ_BITS   _u(0x00001ff0)
#define RVCSR_MEICONTEXT_IRQ_MSB    _u(12)
#define RVCSR_MEICONTEXT_IRQ_LSB    _u(4)
#define RVCSR_MEICONTEXT_IRQ_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_MEICONTEXT_MTIESAVE
// Description : Reads as the current value of `mie.mtie`, if `clearts` is set
//               by the same CSR access instruction. Otherwise reads as 0.
//               Writes are ORed into `mie.mtie`.
#define RVCSR_MEICONTEXT_MTIESAVE_RESET  _u(0x0)
#define RVCSR_MEICONTEXT_MTIESAVE_BITS   _u(0x00000008)
#define RVCSR_MEICONTEXT_MTIESAVE_MSB    _u(3)
#define RVCSR_MEICONTEXT_MTIESAVE_LSB    _u(3)
#define RVCSR_MEICONTEXT_MTIESAVE_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RVCSR_MEICONTEXT_MSIESAVE
// Description : Reads as the current value of `mie.msie`, if `clearts` is set
//               by the same CSR access instruction. Otherwise reads as 0.
//               Writes are ORed into `mie.msie`.
#define RVCSR_MEICONTEXT_MSIESAVE_RESET  _u(0x0)
#define RVCSR_MEICONTEXT_MSIESAVE_BITS   _u(0x00000004)
#define RVCSR_MEICONTEXT_MSIESAVE_MSB    _u(2)
#define RVCSR_MEICONTEXT_MSIESAVE_LSB    _u(2)
#define RVCSR_MEICONTEXT_MSIESAVE_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RVCSR_MEICONTEXT_CLEARTS
// Description : Write-1 self-clearing field. Writing 1 will clear `mie.mtie`
//               and `mie.msie`, and present their prior values in the
//               `mtiesave` and `msiesave` of this register. This makes it safe
//               to re-enable IRQs (via `mstatus.mie`) without the possibility
//               of being preempted by the standard timer and soft interrupt
//               handlers, which may not be aware of Hazard3's interrupt
//               hardware.
//
//               The clear due to `clearts` takes precedence over the set due to
//               `mtiesave`/`msiesave`, although it would be unusual for
//               software to write both on the same cycle.
#define RVCSR_MEICONTEXT_CLEARTS_RESET  _u(0x0)
#define RVCSR_MEICONTEXT_CLEARTS_BITS   _u(0x00000002)
#define RVCSR_MEICONTEXT_CLEARTS_MSB    _u(1)
#define RVCSR_MEICONTEXT_CLEARTS_LSB    _u(1)
#define RVCSR_MEICONTEXT_CLEARTS_ACCESS "SC"
// -----------------------------------------------------------------------------
// Field       : RVCSR_MEICONTEXT_MRETEIRQ
// Description : If 1, enable restore of the preemption priority stack on
//               `mret`. This bit is set on entering the external interrupt
//               vector, cleared by `mret`, and cleared upon taking any trap
//               other than an external interrupt.
//
//               Provided `meicontext` is saved on entry to the external
//               interrupt vector (before enabling preemption), is restored
//               before exiting, and the standard software/timer IRQs are
//               prevented from preempting (e.g. by using `clearts`), this flag
//               allows the hardware to safely manage the preemption priority
//               stack even when an external interrupt handler may take
//               exceptions.
#define RVCSR_MEICONTEXT_MRETEIRQ_RESET  _u(0x0)
#define RVCSR_MEICONTEXT_MRETEIRQ_BITS   _u(0x00000001)
#define RVCSR_MEICONTEXT_MRETEIRQ_MSB    _u(0)
#define RVCSR_MEICONTEXT_MRETEIRQ_LSB    _u(0)
#define RVCSR_MEICONTEXT_MRETEIRQ_ACCESS "RW"
// =============================================================================
// Register    : RVCSR_MSLEEP
// Description : M-mode sleep control register
#define RVCSR_MSLEEP_OFFSET _u(0x00000bf0)
#define RVCSR_MSLEEP_BITS   _u(0x00000007)
#define RVCSR_MSLEEP_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : RVCSR_MSLEEP_SLEEPONBLOCK
// Description : Enter the deep sleep state configured by
//               msleep.deepsleep/msleep.powerdown on a `h3.block` instruction,
//               as well as a standard `wfi`. If this bit is clear, a `h3.block`
//               is always implemented as a simple pipeline stall.
#define RVCSR_MSLEEP_SLEEPONBLOCK_RESET  _u(0x0)
#define RVCSR_MSLEEP_SLEEPONBLOCK_BITS   _u(0x00000004)
#define RVCSR_MSLEEP_SLEEPONBLOCK_MSB    _u(2)
#define RVCSR_MSLEEP_SLEEPONBLOCK_LSB    _u(2)
#define RVCSR_MSLEEP_SLEEPONBLOCK_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_MSLEEP_POWERDOWN
// Description : Release the external power request when going to sleep. The
//               function of this is platform-defined -- it may do nothing, it
//               may do something simple like clock-gating the fabric, or it may
//               be tied to some complex system-level power controller.
//
//               When waking, the processor reasserts its external power-up
//               request, and will not fetch any instructions until the request
//               is acknowledged. This may add considerable latency to the
//               wakeup.
#define RVCSR_MSLEEP_POWERDOWN_RESET  _u(0x0)
#define RVCSR_MSLEEP_POWERDOWN_BITS   _u(0x00000002)
#define RVCSR_MSLEEP_POWERDOWN_MSB    _u(1)
#define RVCSR_MSLEEP_POWERDOWN_LSB    _u(1)
#define RVCSR_MSLEEP_POWERDOWN_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RVCSR_MSLEEP_DEEPSLEEP
// Description : Deassert the processor clock enable when entering the sleep
//               state. If a clock gate is instantiated, this allows most of the
//               processor (everything except the power state machine and the
//               interrupt and halt input registers) to be clock gated whilst
//               asleep, which may reduce the sleep current. This adds one cycle
//               to the wakeup latency.
#define RVCSR_MSLEEP_DEEPSLEEP_RESET  _u(0x0)
#define RVCSR_MSLEEP_DEEPSLEEP_BITS   _u(0x00000001)
#define RVCSR_MSLEEP_DEEPSLEEP_MSB    _u(0)
#define RVCSR_MSLEEP_DEEPSLEEP_LSB    _u(0)
#define RVCSR_MSLEEP_DEEPSLEEP_ACCESS "RW"
// =============================================================================
// Register    : RVCSR_DMDATA0
// Description : The Debug Module's DATA0 register is mapped into Hazard3's CSR
//               space so that the Debug Module can exchange data with the core
//               by executing CSR access instructions (this is used to implement
//               the Abstract Access Register command). Only accessible in Debug
//               Mode.
#define RVCSR_DMDATA0_OFFSET _u(0x00000bff)
#define RVCSR_DMDATA0_BITS   _u(0xffffffff)
#define RVCSR_DMDATA0_RESET  _u(0x00000000)
#define RVCSR_DMDATA0_MSB    _u(31)
#define RVCSR_DMDATA0_LSB    _u(0)
#define RVCSR_DMDATA0_ACCESS "RW"
// =============================================================================
// Register    : RVCSR_CYCLE
// Description : Read-only U-mode alias of mcycle, accessible when
//               `mcounteren.cy` is set
#define RVCSR_CYCLE_OFFSET _u(0x00000c00)
#define RVCSR_CYCLE_BITS   _u(0xffffffff)
#define RVCSR_CYCLE_RESET  _u(0x00000000)
#define RVCSR_CYCLE_MSB    _u(31)
#define RVCSR_CYCLE_LSB    _u(0)
#define RVCSR_CYCLE_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_INSTRET
// Description : Read-only U-mode alias of minstret, accessible when
//               `mcounteren.ir` is set
#define RVCSR_INSTRET_OFFSET _u(0x00000c02)
#define RVCSR_INSTRET_BITS   _u(0xffffffff)
#define RVCSR_INSTRET_RESET  _u(0x00000000)
#define RVCSR_INSTRET_MSB    _u(31)
#define RVCSR_INSTRET_LSB    _u(0)
#define RVCSR_INSTRET_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_CYCLEH
// Description : Read-only U-mode alias of mcycleh, accessible when
//               `mcounteren.cy` is set
#define RVCSR_CYCLEH_OFFSET _u(0x00000c80)
#define RVCSR_CYCLEH_BITS   _u(0xffffffff)
#define RVCSR_CYCLEH_RESET  _u(0x00000000)
#define RVCSR_CYCLEH_MSB    _u(31)
#define RVCSR_CYCLEH_LSB    _u(0)
#define RVCSR_CYCLEH_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_INSTRETH
// Description : Read-only U-mode alias of minstreth, accessible when
//               `mcounteren.ir` is set
#define RVCSR_INSTRETH_OFFSET _u(0x00000c82)
#define RVCSR_INSTRETH_BITS   _u(0xffffffff)
#define RVCSR_INSTRETH_RESET  _u(0x00000000)
#define RVCSR_INSTRETH_MSB    _u(31)
#define RVCSR_INSTRETH_LSB    _u(0)
#define RVCSR_INSTRETH_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MVENDORID
// Description : Vendor ID
#define RVCSR_MVENDORID_OFFSET _u(0x00000f11)
#define RVCSR_MVENDORID_BITS   _u(0xffffffff)
#define RVCSR_MVENDORID_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : RVCSR_MVENDORID_BANK
#define RVCSR_MVENDORID_BANK_RESET  "-"
#define RVCSR_MVENDORID_BANK_BITS   _u(0xffffff80)
#define RVCSR_MVENDORID_BANK_MSB    _u(31)
#define RVCSR_MVENDORID_BANK_LSB    _u(7)
#define RVCSR_MVENDORID_BANK_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RVCSR_MVENDORID_OFFSET
#define RVCSR_MVENDORID_OFFSET_RESET  "-"
#define RVCSR_MVENDORID_OFFSET_BITS   _u(0x0000007f)
#define RVCSR_MVENDORID_OFFSET_MSB    _u(6)
#define RVCSR_MVENDORID_OFFSET_LSB    _u(0)
#define RVCSR_MVENDORID_OFFSET_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MARCHID
// Description : Architecture ID (Hazard3)
#define RVCSR_MARCHID_OFFSET _u(0x00000f12)
#define RVCSR_MARCHID_BITS   _u(0xffffffff)
#define RVCSR_MARCHID_RESET  _u(0x0000001b)
#define RVCSR_MARCHID_MSB    _u(31)
#define RVCSR_MARCHID_LSB    _u(0)
#define RVCSR_MARCHID_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MIMPID
// Description : Implementation ID
#define RVCSR_MIMPID_OFFSET _u(0x00000f13)
#define RVCSR_MIMPID_BITS   _u(0xffffffff)
#define RVCSR_MIMPID_RESET  "-"
#define RVCSR_MIMPID_MSB    _u(31)
#define RVCSR_MIMPID_LSB    _u(0)
#define RVCSR_MIMPID_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MHARTID
// Description : Hardware thread ID
//               On RP2350, core 0 has a hart ID of 0, and core 1 has a hart ID
//               of 1.
#define RVCSR_MHARTID_OFFSET _u(0x00000f14)
#define RVCSR_MHARTID_BITS   _u(0xffffffff)
#define RVCSR_MHARTID_RESET  "-"
#define RVCSR_MHARTID_MSB    _u(31)
#define RVCSR_MHARTID_LSB    _u(0)
#define RVCSR_MHARTID_ACCESS "RO"
// =============================================================================
// Register    : RVCSR_MCONFIGPTR
// Description : Pointer to configuration data structure (hardwired to 0)
#define RVCSR_MCONFIGPTR_OFFSET _u(0x00000f15)
#define RVCSR_MCONFIGPTR_BITS   _u(0xffffffff)
#define RVCSR_MCONFIGPTR_RESET  _u(0x00000000)
#define RVCSR_MCONFIGPTR_MSB    _u(31)
#define RVCSR_MCONFIGPTR_LSB    _u(0)
#define RVCSR_MCONFIGPTR_ACCESS "RO"
// =============================================================================
#endif // _HARDWARE_REGS_RVCSR_H

