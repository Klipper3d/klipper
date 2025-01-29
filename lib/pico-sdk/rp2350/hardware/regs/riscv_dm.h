// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
// =============================================================================
// Register block : RISCV_DM
// Version        : 1
// Bus type       : apb
// Description    : RISC-V Debug module registers (Hazard3 subset only). Read-
//                  only information fields (such as dmstatus.version) are
//                  listed here with the values they have on the RP2350
//                  instantiation of Hazard3.
// =============================================================================
#ifndef _HARDWARE_REGS_RISCV_DM_H
#define _HARDWARE_REGS_RISCV_DM_H
// =============================================================================
// Register    : RISCV_DM_DATA0
// Description : data0 through data11 are basic read/write registers that may be
//               read or changed by abstract commands. abstractcs.datacount
//               indicates how many of them are implemented, starting at data0,
//               counting up.
//
//               Accessing these registers while an abstract command is
//               executing causes abstractcs.cmderr to be set to 1 (busy) if it
//               is 0.
//
//               Attempts to write them while abstractcs.busy is set does not
//               change their value.
//
//               The values in these registers may not be preserved after an
//               abstract command is executed. The only guarantees on their
//               contents are the ones offered by the command in question. If
//               the command fails, no assumptions can be made about the
//               contents of these registers.
//
//               (Note: Hazard3 implements data0 only.)
#define RISCV_DM_DATA0_OFFSET _u(0x00000010)
#define RISCV_DM_DATA0_BITS   _u(0xffffffff)
#define RISCV_DM_DATA0_RESET  _u(0x00000000)
#define RISCV_DM_DATA0_MSB    _u(31)
#define RISCV_DM_DATA0_LSB    _u(0)
#define RISCV_DM_DATA0_ACCESS "RW"
// =============================================================================
// Register    : RISCV_DM_DMCONTROL
// Description : This register controls the overall Debug Module as well as the
//               currently selected harts, as defined in hasel.
#define RISCV_DM_DMCONTROL_OFFSET _u(0x00000040)
#define RISCV_DM_DMCONTROL_BITS   _u(0xf7ffffcf)
#define RISCV_DM_DMCONTROL_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_DMCONTROL_HALTREQ
// Description : Writing 0 clears the halt request bit for all currently
//               selected harts. This may cancel outstanding halt requests for
//               those harts.
//
//               Writing 1 sets the halt request bit for all currently selected
//               harts. Running harts will halt whenever their halt request bit
//               is set.
//
//               Writes apply to the new value of hartsel and hasel.
#define RISCV_DM_DMCONTROL_HALTREQ_RESET  _u(0x0)
#define RISCV_DM_DMCONTROL_HALTREQ_BITS   _u(0x80000000)
#define RISCV_DM_DMCONTROL_HALTREQ_MSB    _u(31)
#define RISCV_DM_DMCONTROL_HALTREQ_LSB    _u(31)
#define RISCV_DM_DMCONTROL_HALTREQ_ACCESS "WO"
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_DMCONTROL_RESUMEREQ
// Description : Writing 1 causes the currently selected harts to resume once,
//               if they are halted when the write occurs. It also clears the
//               resume ack bit for those harts.
//
//               resumereq is ignored if haltreq is set.
//
//               Writes apply to the new value of hartsel and hasel.
#define RISCV_DM_DMCONTROL_RESUMEREQ_RESET  _u(0x0)
#define RISCV_DM_DMCONTROL_RESUMEREQ_BITS   _u(0x40000000)
#define RISCV_DM_DMCONTROL_RESUMEREQ_MSB    _u(30)
#define RISCV_DM_DMCONTROL_RESUMEREQ_LSB    _u(30)
#define RISCV_DM_DMCONTROL_RESUMEREQ_ACCESS "SC"
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_DMCONTROL_HARTRESET
// Description : This optional field writes the reset bit for all the currently
//               selected harts. To perform a reset the debugger writes 1, and
//               then writes 0 to deassert the reset signal.
//
//               While this bit is 1, the debugger must not change which harts
//               are selected.
//
//               Writes apply to the new value of hartsel and hasel.
//
//               (The exact behaviour of this field is implementation-defined:
//               on RP2350 it (triggers a local reset of the selected core(s)
//               only.)
#define RISCV_DM_DMCONTROL_HARTRESET_RESET  _u(0x0)
#define RISCV_DM_DMCONTROL_HARTRESET_BITS   _u(0x20000000)
#define RISCV_DM_DMCONTROL_HARTRESET_MSB    _u(29)
#define RISCV_DM_DMCONTROL_HARTRESET_LSB    _u(29)
#define RISCV_DM_DMCONTROL_HARTRESET_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_DMCONTROL_ACKHAVERESET
// Description : 0: No effect.
//
//               1: Clears havereset for any selected harts.
//
//               Writes apply to the new value of hartsel and hasel.
#define RISCV_DM_DMCONTROL_ACKHAVERESET_RESET  _u(0x0)
#define RISCV_DM_DMCONTROL_ACKHAVERESET_BITS   _u(0x10000000)
#define RISCV_DM_DMCONTROL_ACKHAVERESET_MSB    _u(28)
#define RISCV_DM_DMCONTROL_ACKHAVERESET_LSB    _u(28)
#define RISCV_DM_DMCONTROL_ACKHAVERESET_ACCESS "SC"
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_DMCONTROL_HASEL
// Description : Selects the definition of currently selected harts.
//
//               0: There is a single currently selected hart, that is selected
//               by hartsel.
//
//               1: There may be multiple currently selected harts – the hart
//               selected by hartsel, plus those selected by the hart array mask
//               register.
//
//               Hazard3 does support the hart array mask.
#define RISCV_DM_DMCONTROL_HASEL_RESET  _u(0x0)
#define RISCV_DM_DMCONTROL_HASEL_BITS   _u(0x04000000)
#define RISCV_DM_DMCONTROL_HASEL_MSB    _u(26)
#define RISCV_DM_DMCONTROL_HASEL_LSB    _u(26)
#define RISCV_DM_DMCONTROL_HASEL_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_DMCONTROL_HARTSELLO
// Description : The low 10 bits of hartsel: the DM-specific index of the hart
//               to select. This hart is always part of the currently selected
//               harts.
//
//               On RP2350, since there are only two cores (with one hart each),
//               only the least-significant bit is writable. The others are tied
//               to 0.
#define RISCV_DM_DMCONTROL_HARTSELLO_RESET  _u(0x000)
#define RISCV_DM_DMCONTROL_HARTSELLO_BITS   _u(0x03ff0000)
#define RISCV_DM_DMCONTROL_HARTSELLO_MSB    _u(25)
#define RISCV_DM_DMCONTROL_HARTSELLO_LSB    _u(16)
#define RISCV_DM_DMCONTROL_HARTSELLO_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_DMCONTROL_HARTSELHI
// Description : The high 10 bits of hartsel: the DM-specific index of the hart
//               to select. This hart is always part of the currently selected
//               harts.
//
//               On Hazard3 this field is always tied to all-zeroes.
#define RISCV_DM_DMCONTROL_HARTSELHI_RESET  _u(0x000)
#define RISCV_DM_DMCONTROL_HARTSELHI_BITS   _u(0x0000ffc0)
#define RISCV_DM_DMCONTROL_HARTSELHI_MSB    _u(15)
#define RISCV_DM_DMCONTROL_HARTSELHI_LSB    _u(6)
#define RISCV_DM_DMCONTROL_HARTSELHI_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_DMCONTROL_SETRESETHALTREQ
// Description : This optional field writes the halt-on-reset request bit for
//               all currently selected harts, unless clrresethaltreq is
//               simultaneously set to 1.
//
//               When set to 1, each selected hart will halt upon the next
//               deassertion of its reset. The halt-on-reset request bit is not
//               automatically cleared. The debugger must write to
//               clrresethaltreq to clear it.
//
//               Writes apply to the new value of hartsel and hasel.
#define RISCV_DM_DMCONTROL_SETRESETHALTREQ_RESET  _u(0x0)
#define RISCV_DM_DMCONTROL_SETRESETHALTREQ_BITS   _u(0x00000008)
#define RISCV_DM_DMCONTROL_SETRESETHALTREQ_MSB    _u(3)
#define RISCV_DM_DMCONTROL_SETRESETHALTREQ_LSB    _u(3)
#define RISCV_DM_DMCONTROL_SETRESETHALTREQ_ACCESS "SC"
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_DMCONTROL_CLRRESETHALTREQ
// Description : This optional field clears the halt-on-reset request bit for
//               all currently selected harts.
//
//               Writes apply to the new value of hartsel and hasel.
#define RISCV_DM_DMCONTROL_CLRRESETHALTREQ_RESET  _u(0x0)
#define RISCV_DM_DMCONTROL_CLRRESETHALTREQ_BITS   _u(0x00000004)
#define RISCV_DM_DMCONTROL_CLRRESETHALTREQ_MSB    _u(2)
#define RISCV_DM_DMCONTROL_CLRRESETHALTREQ_LSB    _u(2)
#define RISCV_DM_DMCONTROL_CLRRESETHALTREQ_ACCESS "SC"
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_DMCONTROL_NDMRESET
// Description : This bit controls the reset signal from the DM to the rest of
//               the system. The signal should reset every part of the system,
//               including every hart, except for the DM and any logic required
//               to access the DM. To perform a system reset the debugger writes
//               1, and then writes 0 to deassert the reset.
//
//               On RP2350 this performs a cold reset, the equivalent of a
//               watchdog reset with all WDSEL bits set. This includes both
//               cores and all peripherals.
#define RISCV_DM_DMCONTROL_NDMRESET_RESET  _u(0x0)
#define RISCV_DM_DMCONTROL_NDMRESET_BITS   _u(0x00000002)
#define RISCV_DM_DMCONTROL_NDMRESET_MSB    _u(1)
#define RISCV_DM_DMCONTROL_NDMRESET_LSB    _u(1)
#define RISCV_DM_DMCONTROL_NDMRESET_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_DMCONTROL_DMACTIVE
// Description : This bit serves as a reset signal for the Debug Module itself.
//
//               0: The module’s state, including authentication mechanism,
//               takes its reset values (the dmactive bit is the only bit which
//               can be written to something other than its reset value).
//
//               1: The module functions normally.
//
//               No other mechanism should exist that may result in resetting
//               the Debug Module after power up, with the possible (but not
//               recommended) exception of a global reset signal that resets the
//               entire platform.
//
//               (On RP2350, the Debug Module is reset by a power-on reset, a
//               brownout reset, the RUN pin, and a rescue reset.)
//
//               A debugger may pulse this bit low to get the Debug Module into
//               a known state.
#define RISCV_DM_DMCONTROL_DMACTIVE_RESET  _u(0x0)
#define RISCV_DM_DMCONTROL_DMACTIVE_BITS   _u(0x00000001)
#define RISCV_DM_DMCONTROL_DMACTIVE_MSB    _u(0)
#define RISCV_DM_DMCONTROL_DMACTIVE_LSB    _u(0)
#define RISCV_DM_DMCONTROL_DMACTIVE_ACCESS "RW"
// =============================================================================
// Register    : RISCV_DM_DMSTATUS
// Description : This register reports status for the overall Debug Module as
//               well as the currently selected harts, as defined in hasel. Its
//               address will not change in the future, because it contains
//               version.
//
//               This entire register is read-only.
#define RISCV_DM_DMSTATUS_OFFSET _u(0x00000044)
#define RISCV_DM_DMSTATUS_BITS   _u(0x004fffff)
#define RISCV_DM_DMSTATUS_RESET  _u(0x004000a2)
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_DMSTATUS_IMPEBREAK
// Description : If 1, then there is an implicit ebreak instruction at the non-
//               existent word immediately after the Program Buffer. This saves
//               the debugger from having to write the ebreak itself, and allows
//               the Program Buffer to be one word smaller.
#define RISCV_DM_DMSTATUS_IMPEBREAK_RESET  _u(0x1)
#define RISCV_DM_DMSTATUS_IMPEBREAK_BITS   _u(0x00400000)
#define RISCV_DM_DMSTATUS_IMPEBREAK_MSB    _u(22)
#define RISCV_DM_DMSTATUS_IMPEBREAK_LSB    _u(22)
#define RISCV_DM_DMSTATUS_IMPEBREAK_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_DMSTATUS_ALLHAVERESET
// Description : This field is 1 when all currently selected harts have been
//               reset and reset has not been acknowledged for any of them.
#define RISCV_DM_DMSTATUS_ALLHAVERESET_RESET  _u(0x0)
#define RISCV_DM_DMSTATUS_ALLHAVERESET_BITS   _u(0x00080000)
#define RISCV_DM_DMSTATUS_ALLHAVERESET_MSB    _u(19)
#define RISCV_DM_DMSTATUS_ALLHAVERESET_LSB    _u(19)
#define RISCV_DM_DMSTATUS_ALLHAVERESET_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_DMSTATUS_ANYHAVERESET
// Description : This field is 1 when at least one currently selected hart has
//               been reset and reset has not been acknowledged for that hart.
#define RISCV_DM_DMSTATUS_ANYHAVERESET_RESET  _u(0x0)
#define RISCV_DM_DMSTATUS_ANYHAVERESET_BITS   _u(0x00040000)
#define RISCV_DM_DMSTATUS_ANYHAVERESET_MSB    _u(18)
#define RISCV_DM_DMSTATUS_ANYHAVERESET_LSB    _u(18)
#define RISCV_DM_DMSTATUS_ANYHAVERESET_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_DMSTATUS_ALLRESUMEACK
// Description : This field is 1 when all currently selected harts have
//               acknowledged their last resume request.
#define RISCV_DM_DMSTATUS_ALLRESUMEACK_RESET  _u(0x0)
#define RISCV_DM_DMSTATUS_ALLRESUMEACK_BITS   _u(0x00020000)
#define RISCV_DM_DMSTATUS_ALLRESUMEACK_MSB    _u(17)
#define RISCV_DM_DMSTATUS_ALLRESUMEACK_LSB    _u(17)
#define RISCV_DM_DMSTATUS_ALLRESUMEACK_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_DMSTATUS_ANYRESUMEACK
// Description : This field is 1 when any currently selected hart has
//               acknowledged its last resume request.
#define RISCV_DM_DMSTATUS_ANYRESUMEACK_RESET  _u(0x0)
#define RISCV_DM_DMSTATUS_ANYRESUMEACK_BITS   _u(0x00010000)
#define RISCV_DM_DMSTATUS_ANYRESUMEACK_MSB    _u(16)
#define RISCV_DM_DMSTATUS_ANYRESUMEACK_LSB    _u(16)
#define RISCV_DM_DMSTATUS_ANYRESUMEACK_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_DMSTATUS_ALLNONEXISTENT
// Description : This field is 1 when all currently selected harts do not exist
//               on this platform.
#define RISCV_DM_DMSTATUS_ALLNONEXISTENT_RESET  "-"
#define RISCV_DM_DMSTATUS_ALLNONEXISTENT_BITS   _u(0x00008000)
#define RISCV_DM_DMSTATUS_ALLNONEXISTENT_MSB    _u(15)
#define RISCV_DM_DMSTATUS_ALLNONEXISTENT_LSB    _u(15)
#define RISCV_DM_DMSTATUS_ALLNONEXISTENT_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_DMSTATUS_ANYNONEXISTENT
// Description : This field is 1 when any currently selected hart does not exist
//               in this platform.
#define RISCV_DM_DMSTATUS_ANYNONEXISTENT_RESET  "-"
#define RISCV_DM_DMSTATUS_ANYNONEXISTENT_BITS   _u(0x00004000)
#define RISCV_DM_DMSTATUS_ANYNONEXISTENT_MSB    _u(14)
#define RISCV_DM_DMSTATUS_ANYNONEXISTENT_LSB    _u(14)
#define RISCV_DM_DMSTATUS_ANYNONEXISTENT_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_DMSTATUS_ALLUNAVAIL
// Description : This field is 1 when all currently selected harts are
//               unavailable.
#define RISCV_DM_DMSTATUS_ALLUNAVAIL_RESET  "-"
#define RISCV_DM_DMSTATUS_ALLUNAVAIL_BITS   _u(0x00002000)
#define RISCV_DM_DMSTATUS_ALLUNAVAIL_MSB    _u(13)
#define RISCV_DM_DMSTATUS_ALLUNAVAIL_LSB    _u(13)
#define RISCV_DM_DMSTATUS_ALLUNAVAIL_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_DMSTATUS_ANYUNAVAIL
// Description : This field is 1 when any currently selected hart is
//               unavailable.
#define RISCV_DM_DMSTATUS_ANYUNAVAIL_RESET  "-"
#define RISCV_DM_DMSTATUS_ANYUNAVAIL_BITS   _u(0x00001000)
#define RISCV_DM_DMSTATUS_ANYUNAVAIL_MSB    _u(12)
#define RISCV_DM_DMSTATUS_ANYUNAVAIL_LSB    _u(12)
#define RISCV_DM_DMSTATUS_ANYUNAVAIL_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_DMSTATUS_ALLRUNNING
// Description : This field is 1 when all currently selected harts are running.
#define RISCV_DM_DMSTATUS_ALLRUNNING_RESET  _u(0x0)
#define RISCV_DM_DMSTATUS_ALLRUNNING_BITS   _u(0x00000800)
#define RISCV_DM_DMSTATUS_ALLRUNNING_MSB    _u(11)
#define RISCV_DM_DMSTATUS_ALLRUNNING_LSB    _u(11)
#define RISCV_DM_DMSTATUS_ALLRUNNING_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_DMSTATUS_ANYRUNNING
// Description : This field is 1 when any currently selected hart is running.
#define RISCV_DM_DMSTATUS_ANYRUNNING_RESET  _u(0x0)
#define RISCV_DM_DMSTATUS_ANYRUNNING_BITS   _u(0x00000400)
#define RISCV_DM_DMSTATUS_ANYRUNNING_MSB    _u(10)
#define RISCV_DM_DMSTATUS_ANYRUNNING_LSB    _u(10)
#define RISCV_DM_DMSTATUS_ANYRUNNING_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_DMSTATUS_ALLHALTED
// Description : This field is 1 when all currently selected harts are halted.
#define RISCV_DM_DMSTATUS_ALLHALTED_RESET  _u(0x0)
#define RISCV_DM_DMSTATUS_ALLHALTED_BITS   _u(0x00000200)
#define RISCV_DM_DMSTATUS_ALLHALTED_MSB    _u(9)
#define RISCV_DM_DMSTATUS_ALLHALTED_LSB    _u(9)
#define RISCV_DM_DMSTATUS_ALLHALTED_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_DMSTATUS_ANYHALTED
// Description : This field is 1 when any currently selected hart is halted.
#define RISCV_DM_DMSTATUS_ANYHALTED_RESET  _u(0x0)
#define RISCV_DM_DMSTATUS_ANYHALTED_BITS   _u(0x00000100)
#define RISCV_DM_DMSTATUS_ANYHALTED_MSB    _u(8)
#define RISCV_DM_DMSTATUS_ANYHALTED_LSB    _u(8)
#define RISCV_DM_DMSTATUS_ANYHALTED_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_DMSTATUS_AUTHENTICATED
// Description : 0: Authentication is required before using the DM.
//
//               1: The authentication check has passed.
//
//               On components that don’t implement authentication, this bit
//               must be preset as 1. (Note: the version of Hazard3 on RP2350
//               does not implement authentication.)
#define RISCV_DM_DMSTATUS_AUTHENTICATED_RESET  _u(0x1)
#define RISCV_DM_DMSTATUS_AUTHENTICATED_BITS   _u(0x00000080)
#define RISCV_DM_DMSTATUS_AUTHENTICATED_MSB    _u(7)
#define RISCV_DM_DMSTATUS_AUTHENTICATED_LSB    _u(7)
#define RISCV_DM_DMSTATUS_AUTHENTICATED_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_DMSTATUS_AUTHBUSY
// Description : 0: The authentication module is ready to process the next
//               read/write to authdata.
//
//               1: The authentication module is busy. Accessing authdata
//               results in unspecified behavior. authbusy only becomes set in
//               immediate response to an access to authdata.
#define RISCV_DM_DMSTATUS_AUTHBUSY_RESET  _u(0x0)
#define RISCV_DM_DMSTATUS_AUTHBUSY_BITS   _u(0x00000040)
#define RISCV_DM_DMSTATUS_AUTHBUSY_MSB    _u(6)
#define RISCV_DM_DMSTATUS_AUTHBUSY_LSB    _u(6)
#define RISCV_DM_DMSTATUS_AUTHBUSY_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_DMSTATUS_HASRESETHALTREQ
// Description : 1 if this Debug Module supports halt-on-reset functionality
//               controllable by the setresethaltreq and clrresethaltreq bits. 0
//               otherwise.
#define RISCV_DM_DMSTATUS_HASRESETHALTREQ_RESET  _u(0x1)
#define RISCV_DM_DMSTATUS_HASRESETHALTREQ_BITS   _u(0x00000020)
#define RISCV_DM_DMSTATUS_HASRESETHALTREQ_MSB    _u(5)
#define RISCV_DM_DMSTATUS_HASRESETHALTREQ_LSB    _u(5)
#define RISCV_DM_DMSTATUS_HASRESETHALTREQ_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_DMSTATUS_CONFSTPTRVALID
// Description : 0: confstrptr0–confstrptr3 hold information which is not
//               relevant to the configuration string.
//
//               1: confstrptr0–confstrptr3 hold the address of the
//               configuration string.
#define RISCV_DM_DMSTATUS_CONFSTPTRVALID_RESET  _u(0x0)
#define RISCV_DM_DMSTATUS_CONFSTPTRVALID_BITS   _u(0x00000010)
#define RISCV_DM_DMSTATUS_CONFSTPTRVALID_MSB    _u(4)
#define RISCV_DM_DMSTATUS_CONFSTPTRVALID_LSB    _u(4)
#define RISCV_DM_DMSTATUS_CONFSTPTRVALID_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_DMSTATUS_VERSION
// Description : 0: There is no Debug Module present. 1: There is a Debug Module
//               and it conforms to version 0.11 of the RISC-V debug
//               specification.
//
//               2: There is a Debug Module and it conforms to version 0.13 of
//               the RISC-V debug specification.
//
//               15: There is a Debug Module but it does not con- form to any
//               available version of the RISC-V debug spec.
#define RISCV_DM_DMSTATUS_VERSION_RESET  _u(0x2)
#define RISCV_DM_DMSTATUS_VERSION_BITS   _u(0x0000000f)
#define RISCV_DM_DMSTATUS_VERSION_MSB    _u(3)
#define RISCV_DM_DMSTATUS_VERSION_LSB    _u(0)
#define RISCV_DM_DMSTATUS_VERSION_ACCESS "RO"
// =============================================================================
// Register    : RISCV_DM_HARTINFO
// Description : This register gives information about the hart currently
//               selected by hartsel.
//
//               This entire register is read-only.
#define RISCV_DM_HARTINFO_OFFSET _u(0x00000048)
#define RISCV_DM_HARTINFO_BITS   _u(0x00f1ffff)
#define RISCV_DM_HARTINFO_RESET  _u(0x00001bff)
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_HARTINFO_NSCRATCH
// Description : Number of dscratch registers available for the debugger to use
//               during program buffer execution, starting from dscratch0. The
//               debugger can make no assumptions about the contents of these
//               registers between commands.
#define RISCV_DM_HARTINFO_NSCRATCH_RESET  _u(0x0)
#define RISCV_DM_HARTINFO_NSCRATCH_BITS   _u(0x00f00000)
#define RISCV_DM_HARTINFO_NSCRATCH_MSB    _u(23)
#define RISCV_DM_HARTINFO_NSCRATCH_LSB    _u(20)
#define RISCV_DM_HARTINFO_NSCRATCH_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_HARTINFO_DATAACCESS
// Description : 0: The data registers are shadowed in the hart by CSRs. Each
//               CSR is DXLEN bits in size, and corresponds to a single
//               argument.
//
//               1: The data registers are shadowed in the hart’s memory map.
//               Each register takes up 4 bytes in the memory map.
#define RISCV_DM_HARTINFO_DATAACCESS_RESET  _u(0x0)
#define RISCV_DM_HARTINFO_DATAACCESS_BITS   _u(0x00010000)
#define RISCV_DM_HARTINFO_DATAACCESS_MSB    _u(16)
#define RISCV_DM_HARTINFO_DATAACCESS_LSB    _u(16)
#define RISCV_DM_HARTINFO_DATAACCESS_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_HARTINFO_DATASIZE
// Description : If dataaccess is 0: Number of CSRs dedicated to shadowing the
//               data registers.
//
//               If dataaccess is 1: Number of 32-bit words in the memory map
//               dedicated to shadowing the data registers.
#define RISCV_DM_HARTINFO_DATASIZE_RESET  _u(0x1)
#define RISCV_DM_HARTINFO_DATASIZE_BITS   _u(0x0000f000)
#define RISCV_DM_HARTINFO_DATASIZE_MSB    _u(15)
#define RISCV_DM_HARTINFO_DATASIZE_LSB    _u(12)
#define RISCV_DM_HARTINFO_DATASIZE_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_HARTINFO_DATAADDR
// Description : If dataaccess is 0: The number of the first CSR dedicated to
//               shadowing the data registers.
//
//               If dataaccess is 1: Signed address of RAM where the data
//               registers are shadowed, to be used to access relative to zero
//
//               On Hazard3 this indicates the single data register mapped as
//               dmdata0. There is actually only a single shared register,
//               internal to the Debug Module, and mirrored in each core's CSR
//               space.
#define RISCV_DM_HARTINFO_DATAADDR_RESET  _u(0xbff)
#define RISCV_DM_HARTINFO_DATAADDR_BITS   _u(0x00000fff)
#define RISCV_DM_HARTINFO_DATAADDR_MSB    _u(11)
#define RISCV_DM_HARTINFO_DATAADDR_LSB    _u(0)
#define RISCV_DM_HARTINFO_DATAADDR_ACCESS "RW"
// =============================================================================
// Register    : RISCV_DM_HALTSUM1
// Description : Each bit in this read-only register indicates whether any of a
//               group of harts is halted or not. Unavailable/nonexistent harts
//               are not considered to be halted.
//
//               Each bit in haltsum1 is an OR reduction of 32 bits' worth of
//               haltsum0. On RP2350, only the LSB is implemented.
#define RISCV_DM_HALTSUM1_OFFSET _u(0x0000004c)
#define RISCV_DM_HALTSUM1_BITS   _u(0x00000001)
#define RISCV_DM_HALTSUM1_RESET  _u(0x00000000)
#define RISCV_DM_HALTSUM1_MSB    _u(0)
#define RISCV_DM_HALTSUM1_LSB    _u(0)
#define RISCV_DM_HALTSUM1_ACCESS "RO"
// =============================================================================
// Register    : RISCV_DM_HAWINDOWSEL
// Description : This register selects which of the 32-bit portion of the hart
//               array mask register is accessible in hawindow.
#define RISCV_DM_HAWINDOWSEL_OFFSET _u(0x00000050)
#define RISCV_DM_HAWINDOWSEL_BITS   _u(0x00007fff)
#define RISCV_DM_HAWINDOWSEL_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_HAWINDOWSEL_HAWINDOWSEL
// Description : On Hazard3 this register is entirely hardwired to 0.
#define RISCV_DM_HAWINDOWSEL_HAWINDOWSEL_RESET  _u(0x0000)
#define RISCV_DM_HAWINDOWSEL_HAWINDOWSEL_BITS   _u(0x00007fff)
#define RISCV_DM_HAWINDOWSEL_HAWINDOWSEL_MSB    _u(14)
#define RISCV_DM_HAWINDOWSEL_HAWINDOWSEL_LSB    _u(0)
#define RISCV_DM_HAWINDOWSEL_HAWINDOWSEL_ACCESS "RW"
// =============================================================================
// Register    : RISCV_DM_HAWINDOW
// Description : This register provides R/W access to a 32-bit portion of the
//               hart array mask register. The position of the window is
//               determined by hawindowsel. I.e. bit 0 refers to hart
//               hawindowsel ∗ 32, while bit 31 refers to hart hawindowsel ∗ 32
//               + 31.
//
//               On RP2350 only the two least-significant bits of this register
//               are implemented, since there are only two cores. This is still
//               useful to run/halt/reset both cores exactly simultaneously.
#define RISCV_DM_HAWINDOW_OFFSET _u(0x00000054)
#define RISCV_DM_HAWINDOW_BITS   _u(0x00000003)
#define RISCV_DM_HAWINDOW_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_HAWINDOW_MASKDATA
#define RISCV_DM_HAWINDOW_MASKDATA_RESET  _u(0x0)
#define RISCV_DM_HAWINDOW_MASKDATA_BITS   _u(0x00000003)
#define RISCV_DM_HAWINDOW_MASKDATA_MSB    _u(1)
#define RISCV_DM_HAWINDOW_MASKDATA_LSB    _u(0)
#define RISCV_DM_HAWINDOW_MASKDATA_ACCESS "RW"
// =============================================================================
// Register    : RISCV_DM_ABSTRACTS
// Description : Abstract Control and Status. Writing this register while an
//               abstract command is executing causes cmderr to be set to 1
//               (busy) if it is 0.
#define RISCV_DM_ABSTRACTS_OFFSET _u(0x00000058)
#define RISCV_DM_ABSTRACTS_BITS   _u(0x1f00170f)
#define RISCV_DM_ABSTRACTS_RESET  _u(0x02000001)
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_ABSTRACTS_PROGBUFSIZE
// Description : Size of the Program Buffer, in 32-bit words.
#define RISCV_DM_ABSTRACTS_PROGBUFSIZE_RESET  _u(0x02)
#define RISCV_DM_ABSTRACTS_PROGBUFSIZE_BITS   _u(0x1f000000)
#define RISCV_DM_ABSTRACTS_PROGBUFSIZE_MSB    _u(28)
#define RISCV_DM_ABSTRACTS_PROGBUFSIZE_LSB    _u(24)
#define RISCV_DM_ABSTRACTS_PROGBUFSIZE_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_ABSTRACTS_BUSY
// Description : 1: An abstract command is currently being executed.
//
//               This bit is set as soon as command is written, and is not
//               cleared until that command has completed.
#define RISCV_DM_ABSTRACTS_BUSY_RESET  _u(0x0)
#define RISCV_DM_ABSTRACTS_BUSY_BITS   _u(0x00001000)
#define RISCV_DM_ABSTRACTS_BUSY_MSB    _u(12)
#define RISCV_DM_ABSTRACTS_BUSY_LSB    _u(12)
#define RISCV_DM_ABSTRACTS_BUSY_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_ABSTRACTS_CMDERR
// Description : Gets set if an abstract command fails. The bits in this field
//               remain set until they are cleared by writing 1 to them. No
//               abstract command is started until the value is reset to 0.
//
//               This field only contains a valid value if busy is 0.
//
//               0 (none): No error.
//
//               1 (busy): An abstract command was executing while command,
//               abstractcs, or abstractauto was written, or when one of the
//               data or progbuf registers was read or written. This status is
//               only written if cmderr contains 0.
//
//               2 (not supported): The requested command is not supported,
//               regardless of whether the hart is running or not.
//
//               3 (exception): An exception occurred while executing the
//               command (e.g. while executing the Program Buffer).
//
//               4 (halt/resume): The abstract command couldn’t execute because
//               the hart wasn’t in the required state (running/halted), or
//               unavailable.
//
//               5 (bus): The abstract command failed due to a bus error (e.g.
//               alignment, access size, or timeout).
//
//               7 (other): The command failed for another reason.
//
//               Note: Hazard3 does not set values 5 or 7. Load/store
//               instructions in the program buffer raise an exception when they
//               encounter a bus fault, setting cmderr=3.
#define RISCV_DM_ABSTRACTS_CMDERR_RESET  _u(0x0)
#define RISCV_DM_ABSTRACTS_CMDERR_BITS   _u(0x00000700)
#define RISCV_DM_ABSTRACTS_CMDERR_MSB    _u(10)
#define RISCV_DM_ABSTRACTS_CMDERR_LSB    _u(8)
#define RISCV_DM_ABSTRACTS_CMDERR_ACCESS "WC"
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_ABSTRACTS_DATACOUNT
// Description : Number of data registers that are implemented as part of the
//               abstract command interface.
#define RISCV_DM_ABSTRACTS_DATACOUNT_RESET  _u(0x1)
#define RISCV_DM_ABSTRACTS_DATACOUNT_BITS   _u(0x0000000f)
#define RISCV_DM_ABSTRACTS_DATACOUNT_MSB    _u(3)
#define RISCV_DM_ABSTRACTS_DATACOUNT_LSB    _u(0)
#define RISCV_DM_ABSTRACTS_DATACOUNT_ACCESS "RO"
// =============================================================================
// Register    : RISCV_DM_COMMAND
// Description : Writes to this register cause the corresponding abstract
//               command to be executed.
//
//               Writing this register while an abstract command is executing
//               causes cmderr to be set to 1 (busy) if it is 0.
//
//               If cmderr is non-zero, writes to this register are ignored.
#define RISCV_DM_COMMAND_OFFSET _u(0x0000005c)
#define RISCV_DM_COMMAND_BITS   _u(0xff7fffff)
#define RISCV_DM_COMMAND_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_COMMAND_CMDTYPE
// Description : On Hazard3 this field must be 0 (Access Register)
#define RISCV_DM_COMMAND_CMDTYPE_RESET  _u(0x00)
#define RISCV_DM_COMMAND_CMDTYPE_BITS   _u(0xff000000)
#define RISCV_DM_COMMAND_CMDTYPE_MSB    _u(31)
#define RISCV_DM_COMMAND_CMDTYPE_LSB    _u(24)
#define RISCV_DM_COMMAND_CMDTYPE_ACCESS "WO"
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_COMMAND_AARSIZE
// Description : On Hazard3 this field must be 2 (32-bit register access)
#define RISCV_DM_COMMAND_AARSIZE_RESET  _u(0x0)
#define RISCV_DM_COMMAND_AARSIZE_BITS   _u(0x00700000)
#define RISCV_DM_COMMAND_AARSIZE_MSB    _u(22)
#define RISCV_DM_COMMAND_AARSIZE_LSB    _u(20)
#define RISCV_DM_COMMAND_AARSIZE_ACCESS "WO"
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_COMMAND_AARPOSTINCREMENT
// Description : On Hazard3 this field must be 0 (no post-increment of regno)
#define RISCV_DM_COMMAND_AARPOSTINCREMENT_RESET  _u(0x0)
#define RISCV_DM_COMMAND_AARPOSTINCREMENT_BITS   _u(0x00080000)
#define RISCV_DM_COMMAND_AARPOSTINCREMENT_MSB    _u(19)
#define RISCV_DM_COMMAND_AARPOSTINCREMENT_LSB    _u(19)
#define RISCV_DM_COMMAND_AARPOSTINCREMENT_ACCESS "WO"
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_COMMAND_POSTEXEC
// Description : 0: No effect.
//
//               1: Execute the program in the Program Buffer exactly once after
//               performing the transfer, if any.
#define RISCV_DM_COMMAND_POSTEXEC_RESET  _u(0x0)
#define RISCV_DM_COMMAND_POSTEXEC_BITS   _u(0x00040000)
#define RISCV_DM_COMMAND_POSTEXEC_MSB    _u(18)
#define RISCV_DM_COMMAND_POSTEXEC_LSB    _u(18)
#define RISCV_DM_COMMAND_POSTEXEC_ACCESS "WO"
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_COMMAND_TRANSFER
// Description : 0: Don’t do the operation specified by write.
//
//               1: Do the operation specified by write.
//
//               This bit can be used to just execute the Program Buffer without
//               having to worry about placing valid values into aarsize or
//               regno.
#define RISCV_DM_COMMAND_TRANSFER_RESET  _u(0x0)
#define RISCV_DM_COMMAND_TRANSFER_BITS   _u(0x00020000)
#define RISCV_DM_COMMAND_TRANSFER_MSB    _u(17)
#define RISCV_DM_COMMAND_TRANSFER_LSB    _u(17)
#define RISCV_DM_COMMAND_TRANSFER_ACCESS "WO"
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_COMMAND_WRITE
// Description : When transfer is set:
//
//               0: Copy data from the specified register into data0.
//
//               1: Copy data from data0 into the specified register.
#define RISCV_DM_COMMAND_WRITE_RESET  _u(0x0)
#define RISCV_DM_COMMAND_WRITE_BITS   _u(0x00010000)
#define RISCV_DM_COMMAND_WRITE_MSB    _u(16)
#define RISCV_DM_COMMAND_WRITE_LSB    _u(16)
#define RISCV_DM_COMMAND_WRITE_ACCESS "WO"
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_COMMAND_REGNO
// Description : Number of the register to access.
//
//               On Hazard3 this must be in the range 0x1000 through 0x101f
//               inclusive, referring to GPRs x0 through x31.
#define RISCV_DM_COMMAND_REGNO_RESET  _u(0x0000)
#define RISCV_DM_COMMAND_REGNO_BITS   _u(0x0000ffff)
#define RISCV_DM_COMMAND_REGNO_MSB    _u(15)
#define RISCV_DM_COMMAND_REGNO_LSB    _u(0)
#define RISCV_DM_COMMAND_REGNO_ACCESS "WO"
// =============================================================================
// Register    : RISCV_DM_ABSTRACTAUTO
// Description : Abstract Command Autoexec. Writing this register while an
//               abstract command is executing causes cmderr to be set to 1
//               (busy) if it is 0.
#define RISCV_DM_ABSTRACTAUTO_OFFSET _u(0x00000060)
#define RISCV_DM_ABSTRACTAUTO_BITS   _u(0xffff8fff)
#define RISCV_DM_ABSTRACTAUTO_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_ABSTRACTAUTO_AUTOEXECPROGBUF
// Description : When a bit in this field is 1, read or write accesses to the
//               corresponding progbuf word cause the command in command to be
//               executed again.
//
//               Hazard3 implements only the two least-significant bits of this
//               field (for the two-entry progbuf)
#define RISCV_DM_ABSTRACTAUTO_AUTOEXECPROGBUF_RESET  _u(0x00000)
#define RISCV_DM_ABSTRACTAUTO_AUTOEXECPROGBUF_BITS   _u(0xffff8000)
#define RISCV_DM_ABSTRACTAUTO_AUTOEXECPROGBUF_MSB    _u(31)
#define RISCV_DM_ABSTRACTAUTO_AUTOEXECPROGBUF_LSB    _u(15)
#define RISCV_DM_ABSTRACTAUTO_AUTOEXECPROGBUF_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_ABSTRACTAUTO_AUTOEXEDDATA
// Description : When a bit in this field is 1, read or write accesses to the
//               corresponding data word cause the command in command to be
//               executed again.
//
//               Hazard3 implements only the least-significant bit of this
//               field.
#define RISCV_DM_ABSTRACTAUTO_AUTOEXEDDATA_RESET  _u(0x000)
#define RISCV_DM_ABSTRACTAUTO_AUTOEXEDDATA_BITS   _u(0x00000fff)
#define RISCV_DM_ABSTRACTAUTO_AUTOEXEDDATA_MSB    _u(11)
#define RISCV_DM_ABSTRACTAUTO_AUTOEXEDDATA_LSB    _u(0)
#define RISCV_DM_ABSTRACTAUTO_AUTOEXEDDATA_ACCESS "RW"
// =============================================================================
// Register    : RISCV_DM_NEXTDM
// Description : If there is more than one DM accessible on this DMI, this
//               register contains the base address of thenext one in the chain,
//               or 0 if this is the last one in the chain.
#define RISCV_DM_NEXTDM_OFFSET _u(0x00000074)
#define RISCV_DM_NEXTDM_BITS   _u(0xffffffff)
#define RISCV_DM_NEXTDM_RESET  _u(0x00000000)
#define RISCV_DM_NEXTDM_MSB    _u(31)
#define RISCV_DM_NEXTDM_LSB    _u(0)
#define RISCV_DM_NEXTDM_ACCESS "RO"
// =============================================================================
// Register    : RISCV_DM_PROGBUF0
// Description : progbuf0 through progbuf15 provide read/write access to the
//               program buffer. abstractcs.progbufsize indicates how many of
//               them are implemented starting at progbuf0, counting up.
//
//               (Hazard3 implements a 2-word program buffer.)
#define RISCV_DM_PROGBUF0_OFFSET _u(0x00000080)
#define RISCV_DM_PROGBUF0_BITS   _u(0xffffffff)
#define RISCV_DM_PROGBUF0_RESET  _u(0x00000000)
#define RISCV_DM_PROGBUF0_MSB    _u(31)
#define RISCV_DM_PROGBUF0_LSB    _u(0)
#define RISCV_DM_PROGBUF0_ACCESS "RW"
// =============================================================================
// Register    : RISCV_DM_PROGBUF1
// Description : progbuf0 through progbuf15 provide read/write access to the
//               program buffer. abstractcs.progbufsize indicates how many of
//               them are implemented starting at progbuf0, counting up.
//
//               (Hazard3 implements a 2-word program buffer.)
#define RISCV_DM_PROGBUF1_OFFSET _u(0x00000084)
#define RISCV_DM_PROGBUF1_BITS   _u(0xffffffff)
#define RISCV_DM_PROGBUF1_RESET  _u(0x00000000)
#define RISCV_DM_PROGBUF1_MSB    _u(31)
#define RISCV_DM_PROGBUF1_LSB    _u(0)
#define RISCV_DM_PROGBUF1_ACCESS "RW"
// =============================================================================
// Register    : RISCV_DM_SBCS
// Description : System Bus Access Control and Status
#define RISCV_DM_SBCS_OFFSET _u(0x000000e0)
#define RISCV_DM_SBCS_BITS   _u(0xe07fffff)
#define RISCV_DM_SBCS_RESET  _u(0x20000407)
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_SBCS_SBVERSION
// Description : 1: The System Bus interface conforms to version 0.13.2 of the
//               RISC-V debug spec.
#define RISCV_DM_SBCS_SBVERSION_RESET  _u(0x1)
#define RISCV_DM_SBCS_SBVERSION_BITS   _u(0xe0000000)
#define RISCV_DM_SBCS_SBVERSION_MSB    _u(31)
#define RISCV_DM_SBCS_SBVERSION_LSB    _u(29)
#define RISCV_DM_SBCS_SBVERSION_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_SBCS_SBBUSYERROR
// Description : Set when the debugger attempts to read data while a read is in
//               progress, or when the debugger initiates a new access while one
//               is already in progress (while sbbusy is set). It remains set
//               until it’s explicitly cleared by the debugger.
//
//               While this field is set, no more system bus accesses can be
//               initiated by the Debug Module.
#define RISCV_DM_SBCS_SBBUSYERROR_RESET  _u(0x0)
#define RISCV_DM_SBCS_SBBUSYERROR_BITS   _u(0x00400000)
#define RISCV_DM_SBCS_SBBUSYERROR_MSB    _u(22)
#define RISCV_DM_SBCS_SBBUSYERROR_LSB    _u(22)
#define RISCV_DM_SBCS_SBBUSYERROR_ACCESS "WC"
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_SBCS_SBBUSY
// Description : When 1, indicates the system bus master is busy. (Whether the
//               system bus itself is busy is related, but not the same thing.)
//               This bit goes high immediately when a read or write is
//               requested for any reason, and does not go low until the access
//               is fully completed.
//
//               Writes to sbcs while sbbusy is high result in undefined
//               behavior. A debugger must not write to sbcs until it reads
//               sbbusy as 0.
#define RISCV_DM_SBCS_SBBUSY_RESET  _u(0x0)
#define RISCV_DM_SBCS_SBBUSY_BITS   _u(0x00200000)
#define RISCV_DM_SBCS_SBBUSY_MSB    _u(21)
#define RISCV_DM_SBCS_SBBUSY_LSB    _u(21)
#define RISCV_DM_SBCS_SBBUSY_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_SBCS_SBREADONADDR
// Description : When 1, every write to sbaddress0 automatically triggers a
//               system bus read at the new address.
#define RISCV_DM_SBCS_SBREADONADDR_RESET  _u(0x0)
#define RISCV_DM_SBCS_SBREADONADDR_BITS   _u(0x00100000)
#define RISCV_DM_SBCS_SBREADONADDR_MSB    _u(20)
#define RISCV_DM_SBCS_SBREADONADDR_LSB    _u(20)
#define RISCV_DM_SBCS_SBREADONADDR_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_SBCS_SBACCESS
// Description : Select the access size to use for system bus accesses.
//
//               0: 8-bit
//
//               1: 16-bit
//
//               2: 32-bit
//
//               3: 64-bit
//
//               4: 128-bit
//
//               If sbaccess has an unsupported value when the DM starts a bus
//               access, the access is not per formed and sberror is set to 4.
//               (On Hazard3 the supported values are 8-bit, 16-bit and 32-bit.)
#define RISCV_DM_SBCS_SBACCESS_RESET  _u(0x0)
#define RISCV_DM_SBCS_SBACCESS_BITS   _u(0x000e0000)
#define RISCV_DM_SBCS_SBACCESS_MSB    _u(19)
#define RISCV_DM_SBCS_SBACCESS_LSB    _u(17)
#define RISCV_DM_SBCS_SBACCESS_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_SBCS_SBAUTOINCREMENT
// Description : When 1, sbaddress is incremented by the access size (in bytes)
//               selected in sbaccess after every system bus access.
#define RISCV_DM_SBCS_SBAUTOINCREMENT_RESET  _u(0x0)
#define RISCV_DM_SBCS_SBAUTOINCREMENT_BITS   _u(0x00010000)
#define RISCV_DM_SBCS_SBAUTOINCREMENT_MSB    _u(16)
#define RISCV_DM_SBCS_SBAUTOINCREMENT_LSB    _u(16)
#define RISCV_DM_SBCS_SBAUTOINCREMENT_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_SBCS_SBREADONDATA
// Description : When 1, every read from sbdata0 automatically triggers a system
//               bus read at the (possibly auto- incremented) address.
#define RISCV_DM_SBCS_SBREADONDATA_RESET  _u(0x0)
#define RISCV_DM_SBCS_SBREADONDATA_BITS   _u(0x00008000)
#define RISCV_DM_SBCS_SBREADONDATA_MSB    _u(15)
#define RISCV_DM_SBCS_SBREADONDATA_LSB    _u(15)
#define RISCV_DM_SBCS_SBREADONDATA_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_SBCS_SBERROR
// Description : When the Debug Module’s system bus master encounters an error,
//               this field gets set. The bits in this field remain set until
//               they are cleared by writing 1 to them. While this field is non-
//               zero, no more system bus accesses can be initiated by the Debug
//               Module.
//
//               An implementation may report “Other” (7) for any error
//               condition. (Hazard3 does not use this value for any errors.)
//
//               0: There was no bus error.
//
//               1: There was a timeout.
//
//               2: A bad address was accessed.
//
//               3: There was an alignment error.
//
//               4: An access of unsupported size was requested.
//
//               7: Other.
//
//               Hazard3 raises an alignment error for any non-naturally-aligned
//               bus transfer which would otherwise be a valid transfer.
#define RISCV_DM_SBCS_SBERROR_RESET  _u(0x0)
#define RISCV_DM_SBCS_SBERROR_BITS   _u(0x00007000)
#define RISCV_DM_SBCS_SBERROR_MSB    _u(14)
#define RISCV_DM_SBCS_SBERROR_LSB    _u(12)
#define RISCV_DM_SBCS_SBERROR_ACCESS "WC"
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_SBCS_SBASIZE
// Description : Width of system bus addresses in bits. (0 indicates there is no
//               bus access support.)
#define RISCV_DM_SBCS_SBASIZE_RESET  _u(0x20)
#define RISCV_DM_SBCS_SBASIZE_BITS   _u(0x00000fe0)
#define RISCV_DM_SBCS_SBASIZE_MSB    _u(11)
#define RISCV_DM_SBCS_SBASIZE_LSB    _u(5)
#define RISCV_DM_SBCS_SBASIZE_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_SBCS_SBACCESS128
// Description : 1 when 128-bit system bus accesses are supported.
#define RISCV_DM_SBCS_SBACCESS128_RESET  _u(0x0)
#define RISCV_DM_SBCS_SBACCESS128_BITS   _u(0x00000010)
#define RISCV_DM_SBCS_SBACCESS128_MSB    _u(4)
#define RISCV_DM_SBCS_SBACCESS128_LSB    _u(4)
#define RISCV_DM_SBCS_SBACCESS128_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_SBCS_SBACCESS64
// Description : 1 when 64-bit system bus accesses are supported.
#define RISCV_DM_SBCS_SBACCESS64_RESET  _u(0x0)
#define RISCV_DM_SBCS_SBACCESS64_BITS   _u(0x00000008)
#define RISCV_DM_SBCS_SBACCESS64_MSB    _u(3)
#define RISCV_DM_SBCS_SBACCESS64_LSB    _u(3)
#define RISCV_DM_SBCS_SBACCESS64_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_SBCS_SBACCESS32
// Description : 1 when 32-bit system bus accesses are supported.
#define RISCV_DM_SBCS_SBACCESS32_RESET  _u(0x1)
#define RISCV_DM_SBCS_SBACCESS32_BITS   _u(0x00000004)
#define RISCV_DM_SBCS_SBACCESS32_MSB    _u(2)
#define RISCV_DM_SBCS_SBACCESS32_LSB    _u(2)
#define RISCV_DM_SBCS_SBACCESS32_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_SBCS_SBACCESS16
// Description : 1 when 16-bit system bus accesses are supported.
#define RISCV_DM_SBCS_SBACCESS16_RESET  _u(0x1)
#define RISCV_DM_SBCS_SBACCESS16_BITS   _u(0x00000002)
#define RISCV_DM_SBCS_SBACCESS16_MSB    _u(1)
#define RISCV_DM_SBCS_SBACCESS16_LSB    _u(1)
#define RISCV_DM_SBCS_SBACCESS16_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_SBCS_SBACCESS8
// Description : 1 when 8-bit system bus accesses are supported.
#define RISCV_DM_SBCS_SBACCESS8_RESET  _u(0x1)
#define RISCV_DM_SBCS_SBACCESS8_BITS   _u(0x00000001)
#define RISCV_DM_SBCS_SBACCESS8_MSB    _u(0)
#define RISCV_DM_SBCS_SBACCESS8_LSB    _u(0)
#define RISCV_DM_SBCS_SBACCESS8_ACCESS "RO"
// =============================================================================
// Register    : RISCV_DM_SBADDRESS0
// Description : System Bus Address 31:0
//
//               When the system bus master is busy, writes to this register
//               will set sbbusyerror and don’t do anything else.
//
//               If sberror is 0, sbbusyerror is 0, and sbreadonaddr is set then
//               writes to this register start the following:
//
//               1. Set sbbusy.
//
//               2. Perform a bus read from the new value of sbaddress.
//
//               3. If the read succeeded and sbautoincrement is set, increment
//               sbaddress.
//
//               4. Clear sbbusy.
#define RISCV_DM_SBADDRESS0_OFFSET _u(0x000000e4)
#define RISCV_DM_SBADDRESS0_BITS   _u(0xffffffff)
#define RISCV_DM_SBADDRESS0_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_SBADDRESS0_ADDRESS
// Description : Accesses bits 31:0 of the physical address in sbaddress.
#define RISCV_DM_SBADDRESS0_ADDRESS_RESET  _u(0x00000000)
#define RISCV_DM_SBADDRESS0_ADDRESS_BITS   _u(0xffffffff)
#define RISCV_DM_SBADDRESS0_ADDRESS_MSB    _u(31)
#define RISCV_DM_SBADDRESS0_ADDRESS_LSB    _u(0)
#define RISCV_DM_SBADDRESS0_ADDRESS_ACCESS "RW"
// =============================================================================
// Register    : RISCV_DM_SBDATA0
// Description : System Bus Data 31:0
//
//               Any successful system bus read updates sbdata. If the width of
//               the read access is less than the width of sbdata, the contents
//               of the remaining high bits may take on any value.
//
//               If sberror or sbbusyerror both aren’t 0 then accesses do
//               nothing.
//
//               If the bus master is busy then accesses set sbbusyerror, and
//               don’t do anything else. Writes to this register start the
//               following:
//
//               1. Set sbbusy.
//
//               2. Perform a bus write of the new value of sbdata to sbaddress.
//
//               3. If the write succeeded and sbautoincrement is set, increment
//               sbaddress.
//
//               4. Clear sbbusy.
//
//               Reads from this register start the following:
//
//               1. “Return” the data.
//
//               2. Set sbbusy.
//
//               3. If sbreadondata is set, perform a system bus read from the
//               address contained in sbaddress, placing the result in sbdata.
//
//               4. If the read was successful, and sbautoincrement is set,
//               increment sbaddress.
//
//               5. Clear sbbusy.
#define RISCV_DM_SBDATA0_OFFSET _u(0x000000f0)
#define RISCV_DM_SBDATA0_BITS   _u(0xffffffff)
#define RISCV_DM_SBDATA0_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : RISCV_DM_SBDATA0_DATA
#define RISCV_DM_SBDATA0_DATA_RESET  _u(0x00000000)
#define RISCV_DM_SBDATA0_DATA_BITS   _u(0xffffffff)
#define RISCV_DM_SBDATA0_DATA_MSB    _u(31)
#define RISCV_DM_SBDATA0_DATA_LSB    _u(0)
#define RISCV_DM_SBDATA0_DATA_ACCESS "RW"
// =============================================================================
// Register    : RISCV_DM_HALTSUM0
// Description : Each bit in this read-only register indicates whether one
//               specific hart is halted or not. Unavailable/nonexistent harts
//               are not considered to be halted.
//
//               On RP2350, only the two LSBs of this register are implemented,
//               one for each core/hart.
//
//               This entire register is read-only.
#define RISCV_DM_HALTSUM0_OFFSET _u(0x00000100)
#define RISCV_DM_HALTSUM0_BITS   _u(0xffffffff)
#define RISCV_DM_HALTSUM0_RESET  _u(0x00000000)
#define RISCV_DM_HALTSUM0_MSB    _u(31)
#define RISCV_DM_HALTSUM0_LSB    _u(0)
#define RISCV_DM_HALTSUM0_ACCESS "RO"
// =============================================================================
#endif // _HARDWARE_REGS_RISCV_DM_H

