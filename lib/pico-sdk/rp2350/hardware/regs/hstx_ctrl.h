// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
// =============================================================================
// Register block : HSTX_CTRL
// Version        : 0
// Bus type       : apb
// Description    : Control interface to HSTX. For FIFO write access and status,
//                  see the HSTX_FIFO register block.
// =============================================================================
#ifndef _HARDWARE_REGS_HSTX_CTRL_H
#define _HARDWARE_REGS_HSTX_CTRL_H
// =============================================================================
// Register    : HSTX_CTRL_CSR
#define HSTX_CTRL_CSR_OFFSET _u(0x00000000)
#define HSTX_CTRL_CSR_BITS   _u(0xff1f1f73)
#define HSTX_CTRL_CSR_RESET  _u(0x10050600)
// -----------------------------------------------------------------------------
// Field       : HSTX_CTRL_CSR_CLKDIV
// Description : Clock period of the generated clock, measured in HSTX clock
//               cycles. Can be odd or even. The generated clock advances only
//               on cycles where the shift register shifts.
//
//               For example, a clkdiv of 5 would generate a complete output
//               clock period for every 5 HSTX clocks (or every 10 half-clocks).
//
//               A CLKDIV value of 0 is mapped to a period of 16 HSTX clock
//               cycles.
#define HSTX_CTRL_CSR_CLKDIV_RESET  _u(0x1)
#define HSTX_CTRL_CSR_CLKDIV_BITS   _u(0xf0000000)
#define HSTX_CTRL_CSR_CLKDIV_MSB    _u(31)
#define HSTX_CTRL_CSR_CLKDIV_LSB    _u(28)
#define HSTX_CTRL_CSR_CLKDIV_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : HSTX_CTRL_CSR_CLKPHASE
// Description : Set the initial phase of the generated clock.
//
//               A CLKPHASE of 0 means the clock is initially low, and the first
//               rising edge occurs after one half period of the generated clock
//               (i.e. CLKDIV/2 cycles of clk_hstx). Incrementing CLKPHASE by 1
//               will advance the initial clock phase by one half clk_hstx
//               period. For example, if CLKDIV=2 and CLKPHASE=1:
//
//               * The clock will be initially low
//
//               * The first rising edge will be 0.5 clk_hstx cycles after
//               asserting first data
//
//               * The first falling edge will be 1.5 clk_hstx cycles after
//               asserting first data
//
//               This configuration would be suitable for serialising at a bit
//               rate of clk_hstx with a centre-aligned DDR clock.
//
//               When the HSTX is halted by clearing CSR_EN, the clock generator
//               will return to its initial phase as configured by the CLKPHASE
//               field.
//
//               Note CLKPHASE must be strictly less than double the value of
//               CLKDIV (one full period), else its operation is undefined.
#define HSTX_CTRL_CSR_CLKPHASE_RESET  _u(0x0)
#define HSTX_CTRL_CSR_CLKPHASE_BITS   _u(0x0f000000)
#define HSTX_CTRL_CSR_CLKPHASE_MSB    _u(27)
#define HSTX_CTRL_CSR_CLKPHASE_LSB    _u(24)
#define HSTX_CTRL_CSR_CLKPHASE_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : HSTX_CTRL_CSR_N_SHIFTS
// Description : Number of times to shift the shift register before refilling it
//               from the FIFO. (A count of how many times it has been shifted,
//               *not* the total shift distance.)
//
//               A register value of 0 means shift 32 times.
#define HSTX_CTRL_CSR_N_SHIFTS_RESET  _u(0x05)
#define HSTX_CTRL_CSR_N_SHIFTS_BITS   _u(0x001f0000)
#define HSTX_CTRL_CSR_N_SHIFTS_MSB    _u(20)
#define HSTX_CTRL_CSR_N_SHIFTS_LSB    _u(16)
#define HSTX_CTRL_CSR_N_SHIFTS_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : HSTX_CTRL_CSR_SHIFT
// Description : How many bits to right-rotate the shift register by each cycle.
//
//               The use of a rotate rather than a shift allows left shifts to
//               be emulated, by subtracting the left-shift amount from 32. It
//               also allows data to be repeated, when the product of SHIFT and
//               N_SHIFTS is greater than 32.
#define HSTX_CTRL_CSR_SHIFT_RESET  _u(0x06)
#define HSTX_CTRL_CSR_SHIFT_BITS   _u(0x00001f00)
#define HSTX_CTRL_CSR_SHIFT_MSB    _u(12)
#define HSTX_CTRL_CSR_SHIFT_LSB    _u(8)
#define HSTX_CTRL_CSR_SHIFT_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : HSTX_CTRL_CSR_COUPLED_SEL
// Description : Select which PIO to use for coupled mode operation.
#define HSTX_CTRL_CSR_COUPLED_SEL_RESET  _u(0x0)
#define HSTX_CTRL_CSR_COUPLED_SEL_BITS   _u(0x00000060)
#define HSTX_CTRL_CSR_COUPLED_SEL_MSB    _u(6)
#define HSTX_CTRL_CSR_COUPLED_SEL_LSB    _u(5)
#define HSTX_CTRL_CSR_COUPLED_SEL_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : HSTX_CTRL_CSR_COUPLED_MODE
// Description : Enable the PIO-to-HSTX 1:1 connection. The HSTX must be clocked
//               *directly* from the system clock (not just from some other
//               clock source of the same frequency) for this synchronous
//               interface to function correctly.
//
//               When COUPLED_MODE is set, BITx_SEL_P and SEL_N indices 24
//               through 31 will select bits from the 8-bit PIO-to-HSTX path,
//               rather than shifter bits. Indices of 0 through 23 will still
//               index the shift register as normal.
//
//               The PIO outputs connected to the PIO-to-HSTX bus are those same
//               outputs that would appear on the HSTX-capable pins if those
//               pins' FUNCSELs were set to PIO instead of HSTX.
//
//               For example, if HSTX is on GPIOs 12 through 19, then PIO
//               outputs 12 through 19 are connected to the HSTX when coupled
//               mode is engaged.
#define HSTX_CTRL_CSR_COUPLED_MODE_RESET  _u(0x0)
#define HSTX_CTRL_CSR_COUPLED_MODE_BITS   _u(0x00000010)
#define HSTX_CTRL_CSR_COUPLED_MODE_MSB    _u(4)
#define HSTX_CTRL_CSR_COUPLED_MODE_LSB    _u(4)
#define HSTX_CTRL_CSR_COUPLED_MODE_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : HSTX_CTRL_CSR_EXPAND_EN
// Description : Enable the command expander. When 0, raw FIFO data is passed
//               directly to the output shift register. When 1, the command
//               expander can perform simple operations such as run length
//               decoding on data between the FIFO and the shift register.
//
//               Do not change CXPD_EN whilst EN is set. It's safe to set
//               CXPD_EN simultaneously with setting EN.
#define HSTX_CTRL_CSR_EXPAND_EN_RESET  _u(0x0)
#define HSTX_CTRL_CSR_EXPAND_EN_BITS   _u(0x00000002)
#define HSTX_CTRL_CSR_EXPAND_EN_MSB    _u(1)
#define HSTX_CTRL_CSR_EXPAND_EN_LSB    _u(1)
#define HSTX_CTRL_CSR_EXPAND_EN_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : HSTX_CTRL_CSR_EN
// Description : When EN is 1, the HSTX will shift out data as it appears in the
//               FIFO. As long as there is data, the HSTX shift register will
//               shift once per clock cycle, and the frequency of popping from
//               the FIFO is determined by the ratio of SHIFT and SHIFT_THRESH.
//
//               When EN is 0, the FIFO is not popped. The shift counter and
//               clock generator are also reset to their initial state for as
//               long as EN is low. Note the initial phase of the clock
//               generator can be configured by the CLKPHASE field.
//
//               Once the HSTX is enabled again, and data is pushed to the FIFO,
//               the generated clock's first rising edge will be one half-period
//               after the first data is launched.
#define HSTX_CTRL_CSR_EN_RESET  _u(0x0)
#define HSTX_CTRL_CSR_EN_BITS   _u(0x00000001)
#define HSTX_CTRL_CSR_EN_MSB    _u(0)
#define HSTX_CTRL_CSR_EN_LSB    _u(0)
#define HSTX_CTRL_CSR_EN_ACCESS "RW"
// =============================================================================
// Register    : HSTX_CTRL_BIT0
// Description : Data control register for output bit 0
#define HSTX_CTRL_BIT0_OFFSET _u(0x00000004)
#define HSTX_CTRL_BIT0_BITS   _u(0x00031f1f)
#define HSTX_CTRL_BIT0_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : HSTX_CTRL_BIT0_CLK
// Description : Connect this output to the generated clock, rather than the
//               data shift register. SEL_P and SEL_N are ignored if this bit is
//               set, but INV can still be set to generate an antiphase clock.
#define HSTX_CTRL_BIT0_CLK_RESET  _u(0x0)
#define HSTX_CTRL_BIT0_CLK_BITS   _u(0x00020000)
#define HSTX_CTRL_BIT0_CLK_MSB    _u(17)
#define HSTX_CTRL_BIT0_CLK_LSB    _u(17)
#define HSTX_CTRL_BIT0_CLK_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : HSTX_CTRL_BIT0_INV
// Description : Invert this data output (logical NOT)
#define HSTX_CTRL_BIT0_INV_RESET  _u(0x0)
#define HSTX_CTRL_BIT0_INV_BITS   _u(0x00010000)
#define HSTX_CTRL_BIT0_INV_MSB    _u(16)
#define HSTX_CTRL_BIT0_INV_LSB    _u(16)
#define HSTX_CTRL_BIT0_INV_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : HSTX_CTRL_BIT0_SEL_N
// Description : Shift register data bit select for the second half of the HSTX
//               clock cycle
#define HSTX_CTRL_BIT0_SEL_N_RESET  _u(0x00)
#define HSTX_CTRL_BIT0_SEL_N_BITS   _u(0x00001f00)
#define HSTX_CTRL_BIT0_SEL_N_MSB    _u(12)
#define HSTX_CTRL_BIT0_SEL_N_LSB    _u(8)
#define HSTX_CTRL_BIT0_SEL_N_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : HSTX_CTRL_BIT0_SEL_P
// Description : Shift register data bit select for the first half of the HSTX
//               clock cycle
#define HSTX_CTRL_BIT0_SEL_P_RESET  _u(0x00)
#define HSTX_CTRL_BIT0_SEL_P_BITS   _u(0x0000001f)
#define HSTX_CTRL_BIT0_SEL_P_MSB    _u(4)
#define HSTX_CTRL_BIT0_SEL_P_LSB    _u(0)
#define HSTX_CTRL_BIT0_SEL_P_ACCESS "RW"
// =============================================================================
// Register    : HSTX_CTRL_BIT1
// Description : Data control register for output bit 1
#define HSTX_CTRL_BIT1_OFFSET _u(0x00000008)
#define HSTX_CTRL_BIT1_BITS   _u(0x00031f1f)
#define HSTX_CTRL_BIT1_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : HSTX_CTRL_BIT1_CLK
// Description : Connect this output to the generated clock, rather than the
//               data shift register. SEL_P and SEL_N are ignored if this bit is
//               set, but INV can still be set to generate an antiphase clock.
#define HSTX_CTRL_BIT1_CLK_RESET  _u(0x0)
#define HSTX_CTRL_BIT1_CLK_BITS   _u(0x00020000)
#define HSTX_CTRL_BIT1_CLK_MSB    _u(17)
#define HSTX_CTRL_BIT1_CLK_LSB    _u(17)
#define HSTX_CTRL_BIT1_CLK_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : HSTX_CTRL_BIT1_INV
// Description : Invert this data output (logical NOT)
#define HSTX_CTRL_BIT1_INV_RESET  _u(0x0)
#define HSTX_CTRL_BIT1_INV_BITS   _u(0x00010000)
#define HSTX_CTRL_BIT1_INV_MSB    _u(16)
#define HSTX_CTRL_BIT1_INV_LSB    _u(16)
#define HSTX_CTRL_BIT1_INV_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : HSTX_CTRL_BIT1_SEL_N
// Description : Shift register data bit select for the second half of the HSTX
//               clock cycle
#define HSTX_CTRL_BIT1_SEL_N_RESET  _u(0x00)
#define HSTX_CTRL_BIT1_SEL_N_BITS   _u(0x00001f00)
#define HSTX_CTRL_BIT1_SEL_N_MSB    _u(12)
#define HSTX_CTRL_BIT1_SEL_N_LSB    _u(8)
#define HSTX_CTRL_BIT1_SEL_N_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : HSTX_CTRL_BIT1_SEL_P
// Description : Shift register data bit select for the first half of the HSTX
//               clock cycle
#define HSTX_CTRL_BIT1_SEL_P_RESET  _u(0x00)
#define HSTX_CTRL_BIT1_SEL_P_BITS   _u(0x0000001f)
#define HSTX_CTRL_BIT1_SEL_P_MSB    _u(4)
#define HSTX_CTRL_BIT1_SEL_P_LSB    _u(0)
#define HSTX_CTRL_BIT1_SEL_P_ACCESS "RW"
// =============================================================================
// Register    : HSTX_CTRL_BIT2
// Description : Data control register for output bit 2
#define HSTX_CTRL_BIT2_OFFSET _u(0x0000000c)
#define HSTX_CTRL_BIT2_BITS   _u(0x00031f1f)
#define HSTX_CTRL_BIT2_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : HSTX_CTRL_BIT2_CLK
// Description : Connect this output to the generated clock, rather than the
//               data shift register. SEL_P and SEL_N are ignored if this bit is
//               set, but INV can still be set to generate an antiphase clock.
#define HSTX_CTRL_BIT2_CLK_RESET  _u(0x0)
#define HSTX_CTRL_BIT2_CLK_BITS   _u(0x00020000)
#define HSTX_CTRL_BIT2_CLK_MSB    _u(17)
#define HSTX_CTRL_BIT2_CLK_LSB    _u(17)
#define HSTX_CTRL_BIT2_CLK_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : HSTX_CTRL_BIT2_INV
// Description : Invert this data output (logical NOT)
#define HSTX_CTRL_BIT2_INV_RESET  _u(0x0)
#define HSTX_CTRL_BIT2_INV_BITS   _u(0x00010000)
#define HSTX_CTRL_BIT2_INV_MSB    _u(16)
#define HSTX_CTRL_BIT2_INV_LSB    _u(16)
#define HSTX_CTRL_BIT2_INV_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : HSTX_CTRL_BIT2_SEL_N
// Description : Shift register data bit select for the second half of the HSTX
//               clock cycle
#define HSTX_CTRL_BIT2_SEL_N_RESET  _u(0x00)
#define HSTX_CTRL_BIT2_SEL_N_BITS   _u(0x00001f00)
#define HSTX_CTRL_BIT2_SEL_N_MSB    _u(12)
#define HSTX_CTRL_BIT2_SEL_N_LSB    _u(8)
#define HSTX_CTRL_BIT2_SEL_N_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : HSTX_CTRL_BIT2_SEL_P
// Description : Shift register data bit select for the first half of the HSTX
//               clock cycle
#define HSTX_CTRL_BIT2_SEL_P_RESET  _u(0x00)
#define HSTX_CTRL_BIT2_SEL_P_BITS   _u(0x0000001f)
#define HSTX_CTRL_BIT2_SEL_P_MSB    _u(4)
#define HSTX_CTRL_BIT2_SEL_P_LSB    _u(0)
#define HSTX_CTRL_BIT2_SEL_P_ACCESS "RW"
// =============================================================================
// Register    : HSTX_CTRL_BIT3
// Description : Data control register for output bit 3
#define HSTX_CTRL_BIT3_OFFSET _u(0x00000010)
#define HSTX_CTRL_BIT3_BITS   _u(0x00031f1f)
#define HSTX_CTRL_BIT3_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : HSTX_CTRL_BIT3_CLK
// Description : Connect this output to the generated clock, rather than the
//               data shift register. SEL_P and SEL_N are ignored if this bit is
//               set, but INV can still be set to generate an antiphase clock.
#define HSTX_CTRL_BIT3_CLK_RESET  _u(0x0)
#define HSTX_CTRL_BIT3_CLK_BITS   _u(0x00020000)
#define HSTX_CTRL_BIT3_CLK_MSB    _u(17)
#define HSTX_CTRL_BIT3_CLK_LSB    _u(17)
#define HSTX_CTRL_BIT3_CLK_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : HSTX_CTRL_BIT3_INV
// Description : Invert this data output (logical NOT)
#define HSTX_CTRL_BIT3_INV_RESET  _u(0x0)
#define HSTX_CTRL_BIT3_INV_BITS   _u(0x00010000)
#define HSTX_CTRL_BIT3_INV_MSB    _u(16)
#define HSTX_CTRL_BIT3_INV_LSB    _u(16)
#define HSTX_CTRL_BIT3_INV_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : HSTX_CTRL_BIT3_SEL_N
// Description : Shift register data bit select for the second half of the HSTX
//               clock cycle
#define HSTX_CTRL_BIT3_SEL_N_RESET  _u(0x00)
#define HSTX_CTRL_BIT3_SEL_N_BITS   _u(0x00001f00)
#define HSTX_CTRL_BIT3_SEL_N_MSB    _u(12)
#define HSTX_CTRL_BIT3_SEL_N_LSB    _u(8)
#define HSTX_CTRL_BIT3_SEL_N_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : HSTX_CTRL_BIT3_SEL_P
// Description : Shift register data bit select for the first half of the HSTX
//               clock cycle
#define HSTX_CTRL_BIT3_SEL_P_RESET  _u(0x00)
#define HSTX_CTRL_BIT3_SEL_P_BITS   _u(0x0000001f)
#define HSTX_CTRL_BIT3_SEL_P_MSB    _u(4)
#define HSTX_CTRL_BIT3_SEL_P_LSB    _u(0)
#define HSTX_CTRL_BIT3_SEL_P_ACCESS "RW"
// =============================================================================
// Register    : HSTX_CTRL_BIT4
// Description : Data control register for output bit 4
#define HSTX_CTRL_BIT4_OFFSET _u(0x00000014)
#define HSTX_CTRL_BIT4_BITS   _u(0x00031f1f)
#define HSTX_CTRL_BIT4_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : HSTX_CTRL_BIT4_CLK
// Description : Connect this output to the generated clock, rather than the
//               data shift register. SEL_P and SEL_N are ignored if this bit is
//               set, but INV can still be set to generate an antiphase clock.
#define HSTX_CTRL_BIT4_CLK_RESET  _u(0x0)
#define HSTX_CTRL_BIT4_CLK_BITS   _u(0x00020000)
#define HSTX_CTRL_BIT4_CLK_MSB    _u(17)
#define HSTX_CTRL_BIT4_CLK_LSB    _u(17)
#define HSTX_CTRL_BIT4_CLK_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : HSTX_CTRL_BIT4_INV
// Description : Invert this data output (logical NOT)
#define HSTX_CTRL_BIT4_INV_RESET  _u(0x0)
#define HSTX_CTRL_BIT4_INV_BITS   _u(0x00010000)
#define HSTX_CTRL_BIT4_INV_MSB    _u(16)
#define HSTX_CTRL_BIT4_INV_LSB    _u(16)
#define HSTX_CTRL_BIT4_INV_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : HSTX_CTRL_BIT4_SEL_N
// Description : Shift register data bit select for the second half of the HSTX
//               clock cycle
#define HSTX_CTRL_BIT4_SEL_N_RESET  _u(0x00)
#define HSTX_CTRL_BIT4_SEL_N_BITS   _u(0x00001f00)
#define HSTX_CTRL_BIT4_SEL_N_MSB    _u(12)
#define HSTX_CTRL_BIT4_SEL_N_LSB    _u(8)
#define HSTX_CTRL_BIT4_SEL_N_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : HSTX_CTRL_BIT4_SEL_P
// Description : Shift register data bit select for the first half of the HSTX
//               clock cycle
#define HSTX_CTRL_BIT4_SEL_P_RESET  _u(0x00)
#define HSTX_CTRL_BIT4_SEL_P_BITS   _u(0x0000001f)
#define HSTX_CTRL_BIT4_SEL_P_MSB    _u(4)
#define HSTX_CTRL_BIT4_SEL_P_LSB    _u(0)
#define HSTX_CTRL_BIT4_SEL_P_ACCESS "RW"
// =============================================================================
// Register    : HSTX_CTRL_BIT5
// Description : Data control register for output bit 5
#define HSTX_CTRL_BIT5_OFFSET _u(0x00000018)
#define HSTX_CTRL_BIT5_BITS   _u(0x00031f1f)
#define HSTX_CTRL_BIT5_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : HSTX_CTRL_BIT5_CLK
// Description : Connect this output to the generated clock, rather than the
//               data shift register. SEL_P and SEL_N are ignored if this bit is
//               set, but INV can still be set to generate an antiphase clock.
#define HSTX_CTRL_BIT5_CLK_RESET  _u(0x0)
#define HSTX_CTRL_BIT5_CLK_BITS   _u(0x00020000)
#define HSTX_CTRL_BIT5_CLK_MSB    _u(17)
#define HSTX_CTRL_BIT5_CLK_LSB    _u(17)
#define HSTX_CTRL_BIT5_CLK_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : HSTX_CTRL_BIT5_INV
// Description : Invert this data output (logical NOT)
#define HSTX_CTRL_BIT5_INV_RESET  _u(0x0)
#define HSTX_CTRL_BIT5_INV_BITS   _u(0x00010000)
#define HSTX_CTRL_BIT5_INV_MSB    _u(16)
#define HSTX_CTRL_BIT5_INV_LSB    _u(16)
#define HSTX_CTRL_BIT5_INV_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : HSTX_CTRL_BIT5_SEL_N
// Description : Shift register data bit select for the second half of the HSTX
//               clock cycle
#define HSTX_CTRL_BIT5_SEL_N_RESET  _u(0x00)
#define HSTX_CTRL_BIT5_SEL_N_BITS   _u(0x00001f00)
#define HSTX_CTRL_BIT5_SEL_N_MSB    _u(12)
#define HSTX_CTRL_BIT5_SEL_N_LSB    _u(8)
#define HSTX_CTRL_BIT5_SEL_N_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : HSTX_CTRL_BIT5_SEL_P
// Description : Shift register data bit select for the first half of the HSTX
//               clock cycle
#define HSTX_CTRL_BIT5_SEL_P_RESET  _u(0x00)
#define HSTX_CTRL_BIT5_SEL_P_BITS   _u(0x0000001f)
#define HSTX_CTRL_BIT5_SEL_P_MSB    _u(4)
#define HSTX_CTRL_BIT5_SEL_P_LSB    _u(0)
#define HSTX_CTRL_BIT5_SEL_P_ACCESS "RW"
// =============================================================================
// Register    : HSTX_CTRL_BIT6
// Description : Data control register for output bit 6
#define HSTX_CTRL_BIT6_OFFSET _u(0x0000001c)
#define HSTX_CTRL_BIT6_BITS   _u(0x00031f1f)
#define HSTX_CTRL_BIT6_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : HSTX_CTRL_BIT6_CLK
// Description : Connect this output to the generated clock, rather than the
//               data shift register. SEL_P and SEL_N are ignored if this bit is
//               set, but INV can still be set to generate an antiphase clock.
#define HSTX_CTRL_BIT6_CLK_RESET  _u(0x0)
#define HSTX_CTRL_BIT6_CLK_BITS   _u(0x00020000)
#define HSTX_CTRL_BIT6_CLK_MSB    _u(17)
#define HSTX_CTRL_BIT6_CLK_LSB    _u(17)
#define HSTX_CTRL_BIT6_CLK_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : HSTX_CTRL_BIT6_INV
// Description : Invert this data output (logical NOT)
#define HSTX_CTRL_BIT6_INV_RESET  _u(0x0)
#define HSTX_CTRL_BIT6_INV_BITS   _u(0x00010000)
#define HSTX_CTRL_BIT6_INV_MSB    _u(16)
#define HSTX_CTRL_BIT6_INV_LSB    _u(16)
#define HSTX_CTRL_BIT6_INV_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : HSTX_CTRL_BIT6_SEL_N
// Description : Shift register data bit select for the second half of the HSTX
//               clock cycle
#define HSTX_CTRL_BIT6_SEL_N_RESET  _u(0x00)
#define HSTX_CTRL_BIT6_SEL_N_BITS   _u(0x00001f00)
#define HSTX_CTRL_BIT6_SEL_N_MSB    _u(12)
#define HSTX_CTRL_BIT6_SEL_N_LSB    _u(8)
#define HSTX_CTRL_BIT6_SEL_N_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : HSTX_CTRL_BIT6_SEL_P
// Description : Shift register data bit select for the first half of the HSTX
//               clock cycle
#define HSTX_CTRL_BIT6_SEL_P_RESET  _u(0x00)
#define HSTX_CTRL_BIT6_SEL_P_BITS   _u(0x0000001f)
#define HSTX_CTRL_BIT6_SEL_P_MSB    _u(4)
#define HSTX_CTRL_BIT6_SEL_P_LSB    _u(0)
#define HSTX_CTRL_BIT6_SEL_P_ACCESS "RW"
// =============================================================================
// Register    : HSTX_CTRL_BIT7
// Description : Data control register for output bit 7
#define HSTX_CTRL_BIT7_OFFSET _u(0x00000020)
#define HSTX_CTRL_BIT7_BITS   _u(0x00031f1f)
#define HSTX_CTRL_BIT7_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : HSTX_CTRL_BIT7_CLK
// Description : Connect this output to the generated clock, rather than the
//               data shift register. SEL_P and SEL_N are ignored if this bit is
//               set, but INV can still be set to generate an antiphase clock.
#define HSTX_CTRL_BIT7_CLK_RESET  _u(0x0)
#define HSTX_CTRL_BIT7_CLK_BITS   _u(0x00020000)
#define HSTX_CTRL_BIT7_CLK_MSB    _u(17)
#define HSTX_CTRL_BIT7_CLK_LSB    _u(17)
#define HSTX_CTRL_BIT7_CLK_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : HSTX_CTRL_BIT7_INV
// Description : Invert this data output (logical NOT)
#define HSTX_CTRL_BIT7_INV_RESET  _u(0x0)
#define HSTX_CTRL_BIT7_INV_BITS   _u(0x00010000)
#define HSTX_CTRL_BIT7_INV_MSB    _u(16)
#define HSTX_CTRL_BIT7_INV_LSB    _u(16)
#define HSTX_CTRL_BIT7_INV_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : HSTX_CTRL_BIT7_SEL_N
// Description : Shift register data bit select for the second half of the HSTX
//               clock cycle
#define HSTX_CTRL_BIT7_SEL_N_RESET  _u(0x00)
#define HSTX_CTRL_BIT7_SEL_N_BITS   _u(0x00001f00)
#define HSTX_CTRL_BIT7_SEL_N_MSB    _u(12)
#define HSTX_CTRL_BIT7_SEL_N_LSB    _u(8)
#define HSTX_CTRL_BIT7_SEL_N_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : HSTX_CTRL_BIT7_SEL_P
// Description : Shift register data bit select for the first half of the HSTX
//               clock cycle
#define HSTX_CTRL_BIT7_SEL_P_RESET  _u(0x00)
#define HSTX_CTRL_BIT7_SEL_P_BITS   _u(0x0000001f)
#define HSTX_CTRL_BIT7_SEL_P_MSB    _u(4)
#define HSTX_CTRL_BIT7_SEL_P_LSB    _u(0)
#define HSTX_CTRL_BIT7_SEL_P_ACCESS "RW"
// =============================================================================
// Register    : HSTX_CTRL_EXPAND_SHIFT
// Description : Configure the optional shifter inside the command expander
#define HSTX_CTRL_EXPAND_SHIFT_OFFSET _u(0x00000024)
#define HSTX_CTRL_EXPAND_SHIFT_BITS   _u(0x1f1f1f1f)
#define HSTX_CTRL_EXPAND_SHIFT_RESET  _u(0x01000100)
// -----------------------------------------------------------------------------
// Field       : HSTX_CTRL_EXPAND_SHIFT_ENC_N_SHIFTS
// Description : Number of times to consume from the shift register before
//               refilling it from the FIFO, when the current command is an
//               encoded data command (e.g. TMDS). A register value of 0 means
//               shift 32 times.
#define HSTX_CTRL_EXPAND_SHIFT_ENC_N_SHIFTS_RESET  _u(0x01)
#define HSTX_CTRL_EXPAND_SHIFT_ENC_N_SHIFTS_BITS   _u(0x1f000000)
#define HSTX_CTRL_EXPAND_SHIFT_ENC_N_SHIFTS_MSB    _u(28)
#define HSTX_CTRL_EXPAND_SHIFT_ENC_N_SHIFTS_LSB    _u(24)
#define HSTX_CTRL_EXPAND_SHIFT_ENC_N_SHIFTS_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : HSTX_CTRL_EXPAND_SHIFT_ENC_SHIFT
// Description : How many bits to right-rotate the shift register by each time
//               data is pushed to the output shifter, when the current command
//               is an encoded data command (e.g. TMDS).
#define HSTX_CTRL_EXPAND_SHIFT_ENC_SHIFT_RESET  _u(0x00)
#define HSTX_CTRL_EXPAND_SHIFT_ENC_SHIFT_BITS   _u(0x001f0000)
#define HSTX_CTRL_EXPAND_SHIFT_ENC_SHIFT_MSB    _u(20)
#define HSTX_CTRL_EXPAND_SHIFT_ENC_SHIFT_LSB    _u(16)
#define HSTX_CTRL_EXPAND_SHIFT_ENC_SHIFT_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : HSTX_CTRL_EXPAND_SHIFT_RAW_N_SHIFTS
// Description : Number of times to consume from the shift register before
//               refilling it from the FIFO, when the current command is a raw
//               data command. A register value of 0 means shift 32 times.
#define HSTX_CTRL_EXPAND_SHIFT_RAW_N_SHIFTS_RESET  _u(0x01)
#define HSTX_CTRL_EXPAND_SHIFT_RAW_N_SHIFTS_BITS   _u(0x00001f00)
#define HSTX_CTRL_EXPAND_SHIFT_RAW_N_SHIFTS_MSB    _u(12)
#define HSTX_CTRL_EXPAND_SHIFT_RAW_N_SHIFTS_LSB    _u(8)
#define HSTX_CTRL_EXPAND_SHIFT_RAW_N_SHIFTS_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : HSTX_CTRL_EXPAND_SHIFT_RAW_SHIFT
// Description : How many bits to right-rotate the shift register by each time
//               data is pushed to the output shifter, when the current command
//               is a raw data command.
#define HSTX_CTRL_EXPAND_SHIFT_RAW_SHIFT_RESET  _u(0x00)
#define HSTX_CTRL_EXPAND_SHIFT_RAW_SHIFT_BITS   _u(0x0000001f)
#define HSTX_CTRL_EXPAND_SHIFT_RAW_SHIFT_MSB    _u(4)
#define HSTX_CTRL_EXPAND_SHIFT_RAW_SHIFT_LSB    _u(0)
#define HSTX_CTRL_EXPAND_SHIFT_RAW_SHIFT_ACCESS "RW"
// =============================================================================
// Register    : HSTX_CTRL_EXPAND_TMDS
// Description : Configure the optional TMDS encoder inside the command expander
#define HSTX_CTRL_EXPAND_TMDS_OFFSET _u(0x00000028)
#define HSTX_CTRL_EXPAND_TMDS_BITS   _u(0x00ffffff)
#define HSTX_CTRL_EXPAND_TMDS_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : HSTX_CTRL_EXPAND_TMDS_L2_NBITS
// Description : Number of valid data bits for the lane 2 TMDS encoder, starting
//               from bit 7 of the rotated data. Field values of 0 -> 7 encode
//               counts of 1 -> 8 bits.
#define HSTX_CTRL_EXPAND_TMDS_L2_NBITS_RESET  _u(0x0)
#define HSTX_CTRL_EXPAND_TMDS_L2_NBITS_BITS   _u(0x00e00000)
#define HSTX_CTRL_EXPAND_TMDS_L2_NBITS_MSB    _u(23)
#define HSTX_CTRL_EXPAND_TMDS_L2_NBITS_LSB    _u(21)
#define HSTX_CTRL_EXPAND_TMDS_L2_NBITS_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : HSTX_CTRL_EXPAND_TMDS_L2_ROT
// Description : Right-rotate applied to the current shifter data before the
//               lane 2 TMDS encoder.
#define HSTX_CTRL_EXPAND_TMDS_L2_ROT_RESET  _u(0x00)
#define HSTX_CTRL_EXPAND_TMDS_L2_ROT_BITS   _u(0x001f0000)
#define HSTX_CTRL_EXPAND_TMDS_L2_ROT_MSB    _u(20)
#define HSTX_CTRL_EXPAND_TMDS_L2_ROT_LSB    _u(16)
#define HSTX_CTRL_EXPAND_TMDS_L2_ROT_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : HSTX_CTRL_EXPAND_TMDS_L1_NBITS
// Description : Number of valid data bits for the lane 1 TMDS encoder, starting
//               from bit 7 of the rotated data. Field values of 0 -> 7 encode
//               counts of 1 -> 8 bits.
#define HSTX_CTRL_EXPAND_TMDS_L1_NBITS_RESET  _u(0x0)
#define HSTX_CTRL_EXPAND_TMDS_L1_NBITS_BITS   _u(0x0000e000)
#define HSTX_CTRL_EXPAND_TMDS_L1_NBITS_MSB    _u(15)
#define HSTX_CTRL_EXPAND_TMDS_L1_NBITS_LSB    _u(13)
#define HSTX_CTRL_EXPAND_TMDS_L1_NBITS_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : HSTX_CTRL_EXPAND_TMDS_L1_ROT
// Description : Right-rotate applied to the current shifter data before the
//               lane 1 TMDS encoder.
#define HSTX_CTRL_EXPAND_TMDS_L1_ROT_RESET  _u(0x00)
#define HSTX_CTRL_EXPAND_TMDS_L1_ROT_BITS   _u(0x00001f00)
#define HSTX_CTRL_EXPAND_TMDS_L1_ROT_MSB    _u(12)
#define HSTX_CTRL_EXPAND_TMDS_L1_ROT_LSB    _u(8)
#define HSTX_CTRL_EXPAND_TMDS_L1_ROT_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : HSTX_CTRL_EXPAND_TMDS_L0_NBITS
// Description : Number of valid data bits for the lane 0 TMDS encoder, starting
//               from bit 7 of the rotated data. Field values of 0 -> 7 encode
//               counts of 1 -> 8 bits.
#define HSTX_CTRL_EXPAND_TMDS_L0_NBITS_RESET  _u(0x0)
#define HSTX_CTRL_EXPAND_TMDS_L0_NBITS_BITS   _u(0x000000e0)
#define HSTX_CTRL_EXPAND_TMDS_L0_NBITS_MSB    _u(7)
#define HSTX_CTRL_EXPAND_TMDS_L0_NBITS_LSB    _u(5)
#define HSTX_CTRL_EXPAND_TMDS_L0_NBITS_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : HSTX_CTRL_EXPAND_TMDS_L0_ROT
// Description : Right-rotate applied to the current shifter data before the
//               lane 0 TMDS encoder.
#define HSTX_CTRL_EXPAND_TMDS_L0_ROT_RESET  _u(0x00)
#define HSTX_CTRL_EXPAND_TMDS_L0_ROT_BITS   _u(0x0000001f)
#define HSTX_CTRL_EXPAND_TMDS_L0_ROT_MSB    _u(4)
#define HSTX_CTRL_EXPAND_TMDS_L0_ROT_LSB    _u(0)
#define HSTX_CTRL_EXPAND_TMDS_L0_ROT_ACCESS "RW"
// =============================================================================
#endif // _HARDWARE_REGS_HSTX_CTRL_H

