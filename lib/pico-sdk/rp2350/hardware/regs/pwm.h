// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
// =============================================================================
// Register block : PWM
// Version        : 1
// Bus type       : apb
// Description    : Simple PWM
// =============================================================================
#ifndef _HARDWARE_REGS_PWM_H
#define _HARDWARE_REGS_PWM_H
// =============================================================================
// Register    : PWM_CH0_CSR
// Description : Control and status register
#define PWM_CH0_CSR_OFFSET _u(0x00000000)
#define PWM_CH0_CSR_BITS   _u(0x000000ff)
#define PWM_CH0_CSR_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : PWM_CH0_CSR_PH_ADV
// Description : Advance the phase of the counter by 1 count, while it is
//               running.
//               Self-clearing. Write a 1, and poll until low. Counter must be
//               running
//               at less than full speed (div_int + div_frac / 16 > 1)
#define PWM_CH0_CSR_PH_ADV_RESET  _u(0x0)
#define PWM_CH0_CSR_PH_ADV_BITS   _u(0x00000080)
#define PWM_CH0_CSR_PH_ADV_MSB    _u(7)
#define PWM_CH0_CSR_PH_ADV_LSB    _u(7)
#define PWM_CH0_CSR_PH_ADV_ACCESS "SC"
// -----------------------------------------------------------------------------
// Field       : PWM_CH0_CSR_PH_RET
// Description : Retard the phase of the counter by 1 count, while it is
//               running.
//               Self-clearing. Write a 1, and poll until low. Counter must be
//               running.
#define PWM_CH0_CSR_PH_RET_RESET  _u(0x0)
#define PWM_CH0_CSR_PH_RET_BITS   _u(0x00000040)
#define PWM_CH0_CSR_PH_RET_MSB    _u(6)
#define PWM_CH0_CSR_PH_RET_LSB    _u(6)
#define PWM_CH0_CSR_PH_RET_ACCESS "SC"
// -----------------------------------------------------------------------------
// Field       : PWM_CH0_CSR_DIVMODE
//               0x0 -> Free-running counting at rate dictated by fractional divider
//               0x1 -> Fractional divider operation is gated by the PWM B pin.
//               0x2 -> Counter advances with each rising edge of the PWM B pin.
//               0x3 -> Counter advances with each falling edge of the PWM B pin.
#define PWM_CH0_CSR_DIVMODE_RESET  _u(0x0)
#define PWM_CH0_CSR_DIVMODE_BITS   _u(0x00000030)
#define PWM_CH0_CSR_DIVMODE_MSB    _u(5)
#define PWM_CH0_CSR_DIVMODE_LSB    _u(4)
#define PWM_CH0_CSR_DIVMODE_ACCESS "RW"
#define PWM_CH0_CSR_DIVMODE_VALUE_DIV _u(0x0)
#define PWM_CH0_CSR_DIVMODE_VALUE_LEVEL _u(0x1)
#define PWM_CH0_CSR_DIVMODE_VALUE_RISE _u(0x2)
#define PWM_CH0_CSR_DIVMODE_VALUE_FALL _u(0x3)
// -----------------------------------------------------------------------------
// Field       : PWM_CH0_CSR_B_INV
// Description : Invert output B
#define PWM_CH0_CSR_B_INV_RESET  _u(0x0)
#define PWM_CH0_CSR_B_INV_BITS   _u(0x00000008)
#define PWM_CH0_CSR_B_INV_MSB    _u(3)
#define PWM_CH0_CSR_B_INV_LSB    _u(3)
#define PWM_CH0_CSR_B_INV_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH0_CSR_A_INV
// Description : Invert output A
#define PWM_CH0_CSR_A_INV_RESET  _u(0x0)
#define PWM_CH0_CSR_A_INV_BITS   _u(0x00000004)
#define PWM_CH0_CSR_A_INV_MSB    _u(2)
#define PWM_CH0_CSR_A_INV_LSB    _u(2)
#define PWM_CH0_CSR_A_INV_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH0_CSR_PH_CORRECT
// Description : 1: Enable phase-correct modulation. 0: Trailing-edge
#define PWM_CH0_CSR_PH_CORRECT_RESET  _u(0x0)
#define PWM_CH0_CSR_PH_CORRECT_BITS   _u(0x00000002)
#define PWM_CH0_CSR_PH_CORRECT_MSB    _u(1)
#define PWM_CH0_CSR_PH_CORRECT_LSB    _u(1)
#define PWM_CH0_CSR_PH_CORRECT_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH0_CSR_EN
// Description : Enable the PWM channel.
#define PWM_CH0_CSR_EN_RESET  _u(0x0)
#define PWM_CH0_CSR_EN_BITS   _u(0x00000001)
#define PWM_CH0_CSR_EN_MSB    _u(0)
#define PWM_CH0_CSR_EN_LSB    _u(0)
#define PWM_CH0_CSR_EN_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH0_DIV
// Description : INT and FRAC form a fixed-point fractional number.
//               Counting rate is system clock frequency divided by this number.
//               Fractional division uses simple 1st-order sigma-delta.
#define PWM_CH0_DIV_OFFSET _u(0x00000004)
#define PWM_CH0_DIV_BITS   _u(0x00000fff)
#define PWM_CH0_DIV_RESET  _u(0x00000010)
// -----------------------------------------------------------------------------
// Field       : PWM_CH0_DIV_INT
#define PWM_CH0_DIV_INT_RESET  _u(0x01)
#define PWM_CH0_DIV_INT_BITS   _u(0x00000ff0)
#define PWM_CH0_DIV_INT_MSB    _u(11)
#define PWM_CH0_DIV_INT_LSB    _u(4)
#define PWM_CH0_DIV_INT_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH0_DIV_FRAC
#define PWM_CH0_DIV_FRAC_RESET  _u(0x0)
#define PWM_CH0_DIV_FRAC_BITS   _u(0x0000000f)
#define PWM_CH0_DIV_FRAC_MSB    _u(3)
#define PWM_CH0_DIV_FRAC_LSB    _u(0)
#define PWM_CH0_DIV_FRAC_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH0_CTR
// Description : Direct access to the PWM counter
#define PWM_CH0_CTR_OFFSET _u(0x00000008)
#define PWM_CH0_CTR_BITS   _u(0x0000ffff)
#define PWM_CH0_CTR_RESET  _u(0x00000000)
#define PWM_CH0_CTR_MSB    _u(15)
#define PWM_CH0_CTR_LSB    _u(0)
#define PWM_CH0_CTR_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH0_CC
// Description : Counter compare values
#define PWM_CH0_CC_OFFSET _u(0x0000000c)
#define PWM_CH0_CC_BITS   _u(0xffffffff)
#define PWM_CH0_CC_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : PWM_CH0_CC_B
#define PWM_CH0_CC_B_RESET  _u(0x0000)
#define PWM_CH0_CC_B_BITS   _u(0xffff0000)
#define PWM_CH0_CC_B_MSB    _u(31)
#define PWM_CH0_CC_B_LSB    _u(16)
#define PWM_CH0_CC_B_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH0_CC_A
#define PWM_CH0_CC_A_RESET  _u(0x0000)
#define PWM_CH0_CC_A_BITS   _u(0x0000ffff)
#define PWM_CH0_CC_A_MSB    _u(15)
#define PWM_CH0_CC_A_LSB    _u(0)
#define PWM_CH0_CC_A_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH0_TOP
// Description : Counter wrap value
#define PWM_CH0_TOP_OFFSET _u(0x00000010)
#define PWM_CH0_TOP_BITS   _u(0x0000ffff)
#define PWM_CH0_TOP_RESET  _u(0x0000ffff)
#define PWM_CH0_TOP_MSB    _u(15)
#define PWM_CH0_TOP_LSB    _u(0)
#define PWM_CH0_TOP_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH1_CSR
// Description : Control and status register
#define PWM_CH1_CSR_OFFSET _u(0x00000014)
#define PWM_CH1_CSR_BITS   _u(0x000000ff)
#define PWM_CH1_CSR_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : PWM_CH1_CSR_PH_ADV
// Description : Advance the phase of the counter by 1 count, while it is
//               running.
//               Self-clearing. Write a 1, and poll until low. Counter must be
//               running
//               at less than full speed (div_int + div_frac / 16 > 1)
#define PWM_CH1_CSR_PH_ADV_RESET  _u(0x0)
#define PWM_CH1_CSR_PH_ADV_BITS   _u(0x00000080)
#define PWM_CH1_CSR_PH_ADV_MSB    _u(7)
#define PWM_CH1_CSR_PH_ADV_LSB    _u(7)
#define PWM_CH1_CSR_PH_ADV_ACCESS "SC"
// -----------------------------------------------------------------------------
// Field       : PWM_CH1_CSR_PH_RET
// Description : Retard the phase of the counter by 1 count, while it is
//               running.
//               Self-clearing. Write a 1, and poll until low. Counter must be
//               running.
#define PWM_CH1_CSR_PH_RET_RESET  _u(0x0)
#define PWM_CH1_CSR_PH_RET_BITS   _u(0x00000040)
#define PWM_CH1_CSR_PH_RET_MSB    _u(6)
#define PWM_CH1_CSR_PH_RET_LSB    _u(6)
#define PWM_CH1_CSR_PH_RET_ACCESS "SC"
// -----------------------------------------------------------------------------
// Field       : PWM_CH1_CSR_DIVMODE
//               0x0 -> Free-running counting at rate dictated by fractional divider
//               0x1 -> Fractional divider operation is gated by the PWM B pin.
//               0x2 -> Counter advances with each rising edge of the PWM B pin.
//               0x3 -> Counter advances with each falling edge of the PWM B pin.
#define PWM_CH1_CSR_DIVMODE_RESET  _u(0x0)
#define PWM_CH1_CSR_DIVMODE_BITS   _u(0x00000030)
#define PWM_CH1_CSR_DIVMODE_MSB    _u(5)
#define PWM_CH1_CSR_DIVMODE_LSB    _u(4)
#define PWM_CH1_CSR_DIVMODE_ACCESS "RW"
#define PWM_CH1_CSR_DIVMODE_VALUE_DIV _u(0x0)
#define PWM_CH1_CSR_DIVMODE_VALUE_LEVEL _u(0x1)
#define PWM_CH1_CSR_DIVMODE_VALUE_RISE _u(0x2)
#define PWM_CH1_CSR_DIVMODE_VALUE_FALL _u(0x3)
// -----------------------------------------------------------------------------
// Field       : PWM_CH1_CSR_B_INV
// Description : Invert output B
#define PWM_CH1_CSR_B_INV_RESET  _u(0x0)
#define PWM_CH1_CSR_B_INV_BITS   _u(0x00000008)
#define PWM_CH1_CSR_B_INV_MSB    _u(3)
#define PWM_CH1_CSR_B_INV_LSB    _u(3)
#define PWM_CH1_CSR_B_INV_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH1_CSR_A_INV
// Description : Invert output A
#define PWM_CH1_CSR_A_INV_RESET  _u(0x0)
#define PWM_CH1_CSR_A_INV_BITS   _u(0x00000004)
#define PWM_CH1_CSR_A_INV_MSB    _u(2)
#define PWM_CH1_CSR_A_INV_LSB    _u(2)
#define PWM_CH1_CSR_A_INV_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH1_CSR_PH_CORRECT
// Description : 1: Enable phase-correct modulation. 0: Trailing-edge
#define PWM_CH1_CSR_PH_CORRECT_RESET  _u(0x0)
#define PWM_CH1_CSR_PH_CORRECT_BITS   _u(0x00000002)
#define PWM_CH1_CSR_PH_CORRECT_MSB    _u(1)
#define PWM_CH1_CSR_PH_CORRECT_LSB    _u(1)
#define PWM_CH1_CSR_PH_CORRECT_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH1_CSR_EN
// Description : Enable the PWM channel.
#define PWM_CH1_CSR_EN_RESET  _u(0x0)
#define PWM_CH1_CSR_EN_BITS   _u(0x00000001)
#define PWM_CH1_CSR_EN_MSB    _u(0)
#define PWM_CH1_CSR_EN_LSB    _u(0)
#define PWM_CH1_CSR_EN_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH1_DIV
// Description : INT and FRAC form a fixed-point fractional number.
//               Counting rate is system clock frequency divided by this number.
//               Fractional division uses simple 1st-order sigma-delta.
#define PWM_CH1_DIV_OFFSET _u(0x00000018)
#define PWM_CH1_DIV_BITS   _u(0x00000fff)
#define PWM_CH1_DIV_RESET  _u(0x00000010)
// -----------------------------------------------------------------------------
// Field       : PWM_CH1_DIV_INT
#define PWM_CH1_DIV_INT_RESET  _u(0x01)
#define PWM_CH1_DIV_INT_BITS   _u(0x00000ff0)
#define PWM_CH1_DIV_INT_MSB    _u(11)
#define PWM_CH1_DIV_INT_LSB    _u(4)
#define PWM_CH1_DIV_INT_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH1_DIV_FRAC
#define PWM_CH1_DIV_FRAC_RESET  _u(0x0)
#define PWM_CH1_DIV_FRAC_BITS   _u(0x0000000f)
#define PWM_CH1_DIV_FRAC_MSB    _u(3)
#define PWM_CH1_DIV_FRAC_LSB    _u(0)
#define PWM_CH1_DIV_FRAC_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH1_CTR
// Description : Direct access to the PWM counter
#define PWM_CH1_CTR_OFFSET _u(0x0000001c)
#define PWM_CH1_CTR_BITS   _u(0x0000ffff)
#define PWM_CH1_CTR_RESET  _u(0x00000000)
#define PWM_CH1_CTR_MSB    _u(15)
#define PWM_CH1_CTR_LSB    _u(0)
#define PWM_CH1_CTR_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH1_CC
// Description : Counter compare values
#define PWM_CH1_CC_OFFSET _u(0x00000020)
#define PWM_CH1_CC_BITS   _u(0xffffffff)
#define PWM_CH1_CC_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : PWM_CH1_CC_B
#define PWM_CH1_CC_B_RESET  _u(0x0000)
#define PWM_CH1_CC_B_BITS   _u(0xffff0000)
#define PWM_CH1_CC_B_MSB    _u(31)
#define PWM_CH1_CC_B_LSB    _u(16)
#define PWM_CH1_CC_B_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH1_CC_A
#define PWM_CH1_CC_A_RESET  _u(0x0000)
#define PWM_CH1_CC_A_BITS   _u(0x0000ffff)
#define PWM_CH1_CC_A_MSB    _u(15)
#define PWM_CH1_CC_A_LSB    _u(0)
#define PWM_CH1_CC_A_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH1_TOP
// Description : Counter wrap value
#define PWM_CH1_TOP_OFFSET _u(0x00000024)
#define PWM_CH1_TOP_BITS   _u(0x0000ffff)
#define PWM_CH1_TOP_RESET  _u(0x0000ffff)
#define PWM_CH1_TOP_MSB    _u(15)
#define PWM_CH1_TOP_LSB    _u(0)
#define PWM_CH1_TOP_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH2_CSR
// Description : Control and status register
#define PWM_CH2_CSR_OFFSET _u(0x00000028)
#define PWM_CH2_CSR_BITS   _u(0x000000ff)
#define PWM_CH2_CSR_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : PWM_CH2_CSR_PH_ADV
// Description : Advance the phase of the counter by 1 count, while it is
//               running.
//               Self-clearing. Write a 1, and poll until low. Counter must be
//               running
//               at less than full speed (div_int + div_frac / 16 > 1)
#define PWM_CH2_CSR_PH_ADV_RESET  _u(0x0)
#define PWM_CH2_CSR_PH_ADV_BITS   _u(0x00000080)
#define PWM_CH2_CSR_PH_ADV_MSB    _u(7)
#define PWM_CH2_CSR_PH_ADV_LSB    _u(7)
#define PWM_CH2_CSR_PH_ADV_ACCESS "SC"
// -----------------------------------------------------------------------------
// Field       : PWM_CH2_CSR_PH_RET
// Description : Retard the phase of the counter by 1 count, while it is
//               running.
//               Self-clearing. Write a 1, and poll until low. Counter must be
//               running.
#define PWM_CH2_CSR_PH_RET_RESET  _u(0x0)
#define PWM_CH2_CSR_PH_RET_BITS   _u(0x00000040)
#define PWM_CH2_CSR_PH_RET_MSB    _u(6)
#define PWM_CH2_CSR_PH_RET_LSB    _u(6)
#define PWM_CH2_CSR_PH_RET_ACCESS "SC"
// -----------------------------------------------------------------------------
// Field       : PWM_CH2_CSR_DIVMODE
//               0x0 -> Free-running counting at rate dictated by fractional divider
//               0x1 -> Fractional divider operation is gated by the PWM B pin.
//               0x2 -> Counter advances with each rising edge of the PWM B pin.
//               0x3 -> Counter advances with each falling edge of the PWM B pin.
#define PWM_CH2_CSR_DIVMODE_RESET  _u(0x0)
#define PWM_CH2_CSR_DIVMODE_BITS   _u(0x00000030)
#define PWM_CH2_CSR_DIVMODE_MSB    _u(5)
#define PWM_CH2_CSR_DIVMODE_LSB    _u(4)
#define PWM_CH2_CSR_DIVMODE_ACCESS "RW"
#define PWM_CH2_CSR_DIVMODE_VALUE_DIV _u(0x0)
#define PWM_CH2_CSR_DIVMODE_VALUE_LEVEL _u(0x1)
#define PWM_CH2_CSR_DIVMODE_VALUE_RISE _u(0x2)
#define PWM_CH2_CSR_DIVMODE_VALUE_FALL _u(0x3)
// -----------------------------------------------------------------------------
// Field       : PWM_CH2_CSR_B_INV
// Description : Invert output B
#define PWM_CH2_CSR_B_INV_RESET  _u(0x0)
#define PWM_CH2_CSR_B_INV_BITS   _u(0x00000008)
#define PWM_CH2_CSR_B_INV_MSB    _u(3)
#define PWM_CH2_CSR_B_INV_LSB    _u(3)
#define PWM_CH2_CSR_B_INV_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH2_CSR_A_INV
// Description : Invert output A
#define PWM_CH2_CSR_A_INV_RESET  _u(0x0)
#define PWM_CH2_CSR_A_INV_BITS   _u(0x00000004)
#define PWM_CH2_CSR_A_INV_MSB    _u(2)
#define PWM_CH2_CSR_A_INV_LSB    _u(2)
#define PWM_CH2_CSR_A_INV_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH2_CSR_PH_CORRECT
// Description : 1: Enable phase-correct modulation. 0: Trailing-edge
#define PWM_CH2_CSR_PH_CORRECT_RESET  _u(0x0)
#define PWM_CH2_CSR_PH_CORRECT_BITS   _u(0x00000002)
#define PWM_CH2_CSR_PH_CORRECT_MSB    _u(1)
#define PWM_CH2_CSR_PH_CORRECT_LSB    _u(1)
#define PWM_CH2_CSR_PH_CORRECT_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH2_CSR_EN
// Description : Enable the PWM channel.
#define PWM_CH2_CSR_EN_RESET  _u(0x0)
#define PWM_CH2_CSR_EN_BITS   _u(0x00000001)
#define PWM_CH2_CSR_EN_MSB    _u(0)
#define PWM_CH2_CSR_EN_LSB    _u(0)
#define PWM_CH2_CSR_EN_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH2_DIV
// Description : INT and FRAC form a fixed-point fractional number.
//               Counting rate is system clock frequency divided by this number.
//               Fractional division uses simple 1st-order sigma-delta.
#define PWM_CH2_DIV_OFFSET _u(0x0000002c)
#define PWM_CH2_DIV_BITS   _u(0x00000fff)
#define PWM_CH2_DIV_RESET  _u(0x00000010)
// -----------------------------------------------------------------------------
// Field       : PWM_CH2_DIV_INT
#define PWM_CH2_DIV_INT_RESET  _u(0x01)
#define PWM_CH2_DIV_INT_BITS   _u(0x00000ff0)
#define PWM_CH2_DIV_INT_MSB    _u(11)
#define PWM_CH2_DIV_INT_LSB    _u(4)
#define PWM_CH2_DIV_INT_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH2_DIV_FRAC
#define PWM_CH2_DIV_FRAC_RESET  _u(0x0)
#define PWM_CH2_DIV_FRAC_BITS   _u(0x0000000f)
#define PWM_CH2_DIV_FRAC_MSB    _u(3)
#define PWM_CH2_DIV_FRAC_LSB    _u(0)
#define PWM_CH2_DIV_FRAC_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH2_CTR
// Description : Direct access to the PWM counter
#define PWM_CH2_CTR_OFFSET _u(0x00000030)
#define PWM_CH2_CTR_BITS   _u(0x0000ffff)
#define PWM_CH2_CTR_RESET  _u(0x00000000)
#define PWM_CH2_CTR_MSB    _u(15)
#define PWM_CH2_CTR_LSB    _u(0)
#define PWM_CH2_CTR_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH2_CC
// Description : Counter compare values
#define PWM_CH2_CC_OFFSET _u(0x00000034)
#define PWM_CH2_CC_BITS   _u(0xffffffff)
#define PWM_CH2_CC_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : PWM_CH2_CC_B
#define PWM_CH2_CC_B_RESET  _u(0x0000)
#define PWM_CH2_CC_B_BITS   _u(0xffff0000)
#define PWM_CH2_CC_B_MSB    _u(31)
#define PWM_CH2_CC_B_LSB    _u(16)
#define PWM_CH2_CC_B_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH2_CC_A
#define PWM_CH2_CC_A_RESET  _u(0x0000)
#define PWM_CH2_CC_A_BITS   _u(0x0000ffff)
#define PWM_CH2_CC_A_MSB    _u(15)
#define PWM_CH2_CC_A_LSB    _u(0)
#define PWM_CH2_CC_A_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH2_TOP
// Description : Counter wrap value
#define PWM_CH2_TOP_OFFSET _u(0x00000038)
#define PWM_CH2_TOP_BITS   _u(0x0000ffff)
#define PWM_CH2_TOP_RESET  _u(0x0000ffff)
#define PWM_CH2_TOP_MSB    _u(15)
#define PWM_CH2_TOP_LSB    _u(0)
#define PWM_CH2_TOP_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH3_CSR
// Description : Control and status register
#define PWM_CH3_CSR_OFFSET _u(0x0000003c)
#define PWM_CH3_CSR_BITS   _u(0x000000ff)
#define PWM_CH3_CSR_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : PWM_CH3_CSR_PH_ADV
// Description : Advance the phase of the counter by 1 count, while it is
//               running.
//               Self-clearing. Write a 1, and poll until low. Counter must be
//               running
//               at less than full speed (div_int + div_frac / 16 > 1)
#define PWM_CH3_CSR_PH_ADV_RESET  _u(0x0)
#define PWM_CH3_CSR_PH_ADV_BITS   _u(0x00000080)
#define PWM_CH3_CSR_PH_ADV_MSB    _u(7)
#define PWM_CH3_CSR_PH_ADV_LSB    _u(7)
#define PWM_CH3_CSR_PH_ADV_ACCESS "SC"
// -----------------------------------------------------------------------------
// Field       : PWM_CH3_CSR_PH_RET
// Description : Retard the phase of the counter by 1 count, while it is
//               running.
//               Self-clearing. Write a 1, and poll until low. Counter must be
//               running.
#define PWM_CH3_CSR_PH_RET_RESET  _u(0x0)
#define PWM_CH3_CSR_PH_RET_BITS   _u(0x00000040)
#define PWM_CH3_CSR_PH_RET_MSB    _u(6)
#define PWM_CH3_CSR_PH_RET_LSB    _u(6)
#define PWM_CH3_CSR_PH_RET_ACCESS "SC"
// -----------------------------------------------------------------------------
// Field       : PWM_CH3_CSR_DIVMODE
//               0x0 -> Free-running counting at rate dictated by fractional divider
//               0x1 -> Fractional divider operation is gated by the PWM B pin.
//               0x2 -> Counter advances with each rising edge of the PWM B pin.
//               0x3 -> Counter advances with each falling edge of the PWM B pin.
#define PWM_CH3_CSR_DIVMODE_RESET  _u(0x0)
#define PWM_CH3_CSR_DIVMODE_BITS   _u(0x00000030)
#define PWM_CH3_CSR_DIVMODE_MSB    _u(5)
#define PWM_CH3_CSR_DIVMODE_LSB    _u(4)
#define PWM_CH3_CSR_DIVMODE_ACCESS "RW"
#define PWM_CH3_CSR_DIVMODE_VALUE_DIV _u(0x0)
#define PWM_CH3_CSR_DIVMODE_VALUE_LEVEL _u(0x1)
#define PWM_CH3_CSR_DIVMODE_VALUE_RISE _u(0x2)
#define PWM_CH3_CSR_DIVMODE_VALUE_FALL _u(0x3)
// -----------------------------------------------------------------------------
// Field       : PWM_CH3_CSR_B_INV
// Description : Invert output B
#define PWM_CH3_CSR_B_INV_RESET  _u(0x0)
#define PWM_CH3_CSR_B_INV_BITS   _u(0x00000008)
#define PWM_CH3_CSR_B_INV_MSB    _u(3)
#define PWM_CH3_CSR_B_INV_LSB    _u(3)
#define PWM_CH3_CSR_B_INV_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH3_CSR_A_INV
// Description : Invert output A
#define PWM_CH3_CSR_A_INV_RESET  _u(0x0)
#define PWM_CH3_CSR_A_INV_BITS   _u(0x00000004)
#define PWM_CH3_CSR_A_INV_MSB    _u(2)
#define PWM_CH3_CSR_A_INV_LSB    _u(2)
#define PWM_CH3_CSR_A_INV_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH3_CSR_PH_CORRECT
// Description : 1: Enable phase-correct modulation. 0: Trailing-edge
#define PWM_CH3_CSR_PH_CORRECT_RESET  _u(0x0)
#define PWM_CH3_CSR_PH_CORRECT_BITS   _u(0x00000002)
#define PWM_CH3_CSR_PH_CORRECT_MSB    _u(1)
#define PWM_CH3_CSR_PH_CORRECT_LSB    _u(1)
#define PWM_CH3_CSR_PH_CORRECT_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH3_CSR_EN
// Description : Enable the PWM channel.
#define PWM_CH3_CSR_EN_RESET  _u(0x0)
#define PWM_CH3_CSR_EN_BITS   _u(0x00000001)
#define PWM_CH3_CSR_EN_MSB    _u(0)
#define PWM_CH3_CSR_EN_LSB    _u(0)
#define PWM_CH3_CSR_EN_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH3_DIV
// Description : INT and FRAC form a fixed-point fractional number.
//               Counting rate is system clock frequency divided by this number.
//               Fractional division uses simple 1st-order sigma-delta.
#define PWM_CH3_DIV_OFFSET _u(0x00000040)
#define PWM_CH3_DIV_BITS   _u(0x00000fff)
#define PWM_CH3_DIV_RESET  _u(0x00000010)
// -----------------------------------------------------------------------------
// Field       : PWM_CH3_DIV_INT
#define PWM_CH3_DIV_INT_RESET  _u(0x01)
#define PWM_CH3_DIV_INT_BITS   _u(0x00000ff0)
#define PWM_CH3_DIV_INT_MSB    _u(11)
#define PWM_CH3_DIV_INT_LSB    _u(4)
#define PWM_CH3_DIV_INT_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH3_DIV_FRAC
#define PWM_CH3_DIV_FRAC_RESET  _u(0x0)
#define PWM_CH3_DIV_FRAC_BITS   _u(0x0000000f)
#define PWM_CH3_DIV_FRAC_MSB    _u(3)
#define PWM_CH3_DIV_FRAC_LSB    _u(0)
#define PWM_CH3_DIV_FRAC_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH3_CTR
// Description : Direct access to the PWM counter
#define PWM_CH3_CTR_OFFSET _u(0x00000044)
#define PWM_CH3_CTR_BITS   _u(0x0000ffff)
#define PWM_CH3_CTR_RESET  _u(0x00000000)
#define PWM_CH3_CTR_MSB    _u(15)
#define PWM_CH3_CTR_LSB    _u(0)
#define PWM_CH3_CTR_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH3_CC
// Description : Counter compare values
#define PWM_CH3_CC_OFFSET _u(0x00000048)
#define PWM_CH3_CC_BITS   _u(0xffffffff)
#define PWM_CH3_CC_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : PWM_CH3_CC_B
#define PWM_CH3_CC_B_RESET  _u(0x0000)
#define PWM_CH3_CC_B_BITS   _u(0xffff0000)
#define PWM_CH3_CC_B_MSB    _u(31)
#define PWM_CH3_CC_B_LSB    _u(16)
#define PWM_CH3_CC_B_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH3_CC_A
#define PWM_CH3_CC_A_RESET  _u(0x0000)
#define PWM_CH3_CC_A_BITS   _u(0x0000ffff)
#define PWM_CH3_CC_A_MSB    _u(15)
#define PWM_CH3_CC_A_LSB    _u(0)
#define PWM_CH3_CC_A_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH3_TOP
// Description : Counter wrap value
#define PWM_CH3_TOP_OFFSET _u(0x0000004c)
#define PWM_CH3_TOP_BITS   _u(0x0000ffff)
#define PWM_CH3_TOP_RESET  _u(0x0000ffff)
#define PWM_CH3_TOP_MSB    _u(15)
#define PWM_CH3_TOP_LSB    _u(0)
#define PWM_CH3_TOP_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH4_CSR
// Description : Control and status register
#define PWM_CH4_CSR_OFFSET _u(0x00000050)
#define PWM_CH4_CSR_BITS   _u(0x000000ff)
#define PWM_CH4_CSR_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : PWM_CH4_CSR_PH_ADV
// Description : Advance the phase of the counter by 1 count, while it is
//               running.
//               Self-clearing. Write a 1, and poll until low. Counter must be
//               running
//               at less than full speed (div_int + div_frac / 16 > 1)
#define PWM_CH4_CSR_PH_ADV_RESET  _u(0x0)
#define PWM_CH4_CSR_PH_ADV_BITS   _u(0x00000080)
#define PWM_CH4_CSR_PH_ADV_MSB    _u(7)
#define PWM_CH4_CSR_PH_ADV_LSB    _u(7)
#define PWM_CH4_CSR_PH_ADV_ACCESS "SC"
// -----------------------------------------------------------------------------
// Field       : PWM_CH4_CSR_PH_RET
// Description : Retard the phase of the counter by 1 count, while it is
//               running.
//               Self-clearing. Write a 1, and poll until low. Counter must be
//               running.
#define PWM_CH4_CSR_PH_RET_RESET  _u(0x0)
#define PWM_CH4_CSR_PH_RET_BITS   _u(0x00000040)
#define PWM_CH4_CSR_PH_RET_MSB    _u(6)
#define PWM_CH4_CSR_PH_RET_LSB    _u(6)
#define PWM_CH4_CSR_PH_RET_ACCESS "SC"
// -----------------------------------------------------------------------------
// Field       : PWM_CH4_CSR_DIVMODE
//               0x0 -> Free-running counting at rate dictated by fractional divider
//               0x1 -> Fractional divider operation is gated by the PWM B pin.
//               0x2 -> Counter advances with each rising edge of the PWM B pin.
//               0x3 -> Counter advances with each falling edge of the PWM B pin.
#define PWM_CH4_CSR_DIVMODE_RESET  _u(0x0)
#define PWM_CH4_CSR_DIVMODE_BITS   _u(0x00000030)
#define PWM_CH4_CSR_DIVMODE_MSB    _u(5)
#define PWM_CH4_CSR_DIVMODE_LSB    _u(4)
#define PWM_CH4_CSR_DIVMODE_ACCESS "RW"
#define PWM_CH4_CSR_DIVMODE_VALUE_DIV _u(0x0)
#define PWM_CH4_CSR_DIVMODE_VALUE_LEVEL _u(0x1)
#define PWM_CH4_CSR_DIVMODE_VALUE_RISE _u(0x2)
#define PWM_CH4_CSR_DIVMODE_VALUE_FALL _u(0x3)
// -----------------------------------------------------------------------------
// Field       : PWM_CH4_CSR_B_INV
// Description : Invert output B
#define PWM_CH4_CSR_B_INV_RESET  _u(0x0)
#define PWM_CH4_CSR_B_INV_BITS   _u(0x00000008)
#define PWM_CH4_CSR_B_INV_MSB    _u(3)
#define PWM_CH4_CSR_B_INV_LSB    _u(3)
#define PWM_CH4_CSR_B_INV_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH4_CSR_A_INV
// Description : Invert output A
#define PWM_CH4_CSR_A_INV_RESET  _u(0x0)
#define PWM_CH4_CSR_A_INV_BITS   _u(0x00000004)
#define PWM_CH4_CSR_A_INV_MSB    _u(2)
#define PWM_CH4_CSR_A_INV_LSB    _u(2)
#define PWM_CH4_CSR_A_INV_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH4_CSR_PH_CORRECT
// Description : 1: Enable phase-correct modulation. 0: Trailing-edge
#define PWM_CH4_CSR_PH_CORRECT_RESET  _u(0x0)
#define PWM_CH4_CSR_PH_CORRECT_BITS   _u(0x00000002)
#define PWM_CH4_CSR_PH_CORRECT_MSB    _u(1)
#define PWM_CH4_CSR_PH_CORRECT_LSB    _u(1)
#define PWM_CH4_CSR_PH_CORRECT_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH4_CSR_EN
// Description : Enable the PWM channel.
#define PWM_CH4_CSR_EN_RESET  _u(0x0)
#define PWM_CH4_CSR_EN_BITS   _u(0x00000001)
#define PWM_CH4_CSR_EN_MSB    _u(0)
#define PWM_CH4_CSR_EN_LSB    _u(0)
#define PWM_CH4_CSR_EN_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH4_DIV
// Description : INT and FRAC form a fixed-point fractional number.
//               Counting rate is system clock frequency divided by this number.
//               Fractional division uses simple 1st-order sigma-delta.
#define PWM_CH4_DIV_OFFSET _u(0x00000054)
#define PWM_CH4_DIV_BITS   _u(0x00000fff)
#define PWM_CH4_DIV_RESET  _u(0x00000010)
// -----------------------------------------------------------------------------
// Field       : PWM_CH4_DIV_INT
#define PWM_CH4_DIV_INT_RESET  _u(0x01)
#define PWM_CH4_DIV_INT_BITS   _u(0x00000ff0)
#define PWM_CH4_DIV_INT_MSB    _u(11)
#define PWM_CH4_DIV_INT_LSB    _u(4)
#define PWM_CH4_DIV_INT_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH4_DIV_FRAC
#define PWM_CH4_DIV_FRAC_RESET  _u(0x0)
#define PWM_CH4_DIV_FRAC_BITS   _u(0x0000000f)
#define PWM_CH4_DIV_FRAC_MSB    _u(3)
#define PWM_CH4_DIV_FRAC_LSB    _u(0)
#define PWM_CH4_DIV_FRAC_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH4_CTR
// Description : Direct access to the PWM counter
#define PWM_CH4_CTR_OFFSET _u(0x00000058)
#define PWM_CH4_CTR_BITS   _u(0x0000ffff)
#define PWM_CH4_CTR_RESET  _u(0x00000000)
#define PWM_CH4_CTR_MSB    _u(15)
#define PWM_CH4_CTR_LSB    _u(0)
#define PWM_CH4_CTR_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH4_CC
// Description : Counter compare values
#define PWM_CH4_CC_OFFSET _u(0x0000005c)
#define PWM_CH4_CC_BITS   _u(0xffffffff)
#define PWM_CH4_CC_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : PWM_CH4_CC_B
#define PWM_CH4_CC_B_RESET  _u(0x0000)
#define PWM_CH4_CC_B_BITS   _u(0xffff0000)
#define PWM_CH4_CC_B_MSB    _u(31)
#define PWM_CH4_CC_B_LSB    _u(16)
#define PWM_CH4_CC_B_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH4_CC_A
#define PWM_CH4_CC_A_RESET  _u(0x0000)
#define PWM_CH4_CC_A_BITS   _u(0x0000ffff)
#define PWM_CH4_CC_A_MSB    _u(15)
#define PWM_CH4_CC_A_LSB    _u(0)
#define PWM_CH4_CC_A_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH4_TOP
// Description : Counter wrap value
#define PWM_CH4_TOP_OFFSET _u(0x00000060)
#define PWM_CH4_TOP_BITS   _u(0x0000ffff)
#define PWM_CH4_TOP_RESET  _u(0x0000ffff)
#define PWM_CH4_TOP_MSB    _u(15)
#define PWM_CH4_TOP_LSB    _u(0)
#define PWM_CH4_TOP_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH5_CSR
// Description : Control and status register
#define PWM_CH5_CSR_OFFSET _u(0x00000064)
#define PWM_CH5_CSR_BITS   _u(0x000000ff)
#define PWM_CH5_CSR_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : PWM_CH5_CSR_PH_ADV
// Description : Advance the phase of the counter by 1 count, while it is
//               running.
//               Self-clearing. Write a 1, and poll until low. Counter must be
//               running
//               at less than full speed (div_int + div_frac / 16 > 1)
#define PWM_CH5_CSR_PH_ADV_RESET  _u(0x0)
#define PWM_CH5_CSR_PH_ADV_BITS   _u(0x00000080)
#define PWM_CH5_CSR_PH_ADV_MSB    _u(7)
#define PWM_CH5_CSR_PH_ADV_LSB    _u(7)
#define PWM_CH5_CSR_PH_ADV_ACCESS "SC"
// -----------------------------------------------------------------------------
// Field       : PWM_CH5_CSR_PH_RET
// Description : Retard the phase of the counter by 1 count, while it is
//               running.
//               Self-clearing. Write a 1, and poll until low. Counter must be
//               running.
#define PWM_CH5_CSR_PH_RET_RESET  _u(0x0)
#define PWM_CH5_CSR_PH_RET_BITS   _u(0x00000040)
#define PWM_CH5_CSR_PH_RET_MSB    _u(6)
#define PWM_CH5_CSR_PH_RET_LSB    _u(6)
#define PWM_CH5_CSR_PH_RET_ACCESS "SC"
// -----------------------------------------------------------------------------
// Field       : PWM_CH5_CSR_DIVMODE
//               0x0 -> Free-running counting at rate dictated by fractional divider
//               0x1 -> Fractional divider operation is gated by the PWM B pin.
//               0x2 -> Counter advances with each rising edge of the PWM B pin.
//               0x3 -> Counter advances with each falling edge of the PWM B pin.
#define PWM_CH5_CSR_DIVMODE_RESET  _u(0x0)
#define PWM_CH5_CSR_DIVMODE_BITS   _u(0x00000030)
#define PWM_CH5_CSR_DIVMODE_MSB    _u(5)
#define PWM_CH5_CSR_DIVMODE_LSB    _u(4)
#define PWM_CH5_CSR_DIVMODE_ACCESS "RW"
#define PWM_CH5_CSR_DIVMODE_VALUE_DIV _u(0x0)
#define PWM_CH5_CSR_DIVMODE_VALUE_LEVEL _u(0x1)
#define PWM_CH5_CSR_DIVMODE_VALUE_RISE _u(0x2)
#define PWM_CH5_CSR_DIVMODE_VALUE_FALL _u(0x3)
// -----------------------------------------------------------------------------
// Field       : PWM_CH5_CSR_B_INV
// Description : Invert output B
#define PWM_CH5_CSR_B_INV_RESET  _u(0x0)
#define PWM_CH5_CSR_B_INV_BITS   _u(0x00000008)
#define PWM_CH5_CSR_B_INV_MSB    _u(3)
#define PWM_CH5_CSR_B_INV_LSB    _u(3)
#define PWM_CH5_CSR_B_INV_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH5_CSR_A_INV
// Description : Invert output A
#define PWM_CH5_CSR_A_INV_RESET  _u(0x0)
#define PWM_CH5_CSR_A_INV_BITS   _u(0x00000004)
#define PWM_CH5_CSR_A_INV_MSB    _u(2)
#define PWM_CH5_CSR_A_INV_LSB    _u(2)
#define PWM_CH5_CSR_A_INV_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH5_CSR_PH_CORRECT
// Description : 1: Enable phase-correct modulation. 0: Trailing-edge
#define PWM_CH5_CSR_PH_CORRECT_RESET  _u(0x0)
#define PWM_CH5_CSR_PH_CORRECT_BITS   _u(0x00000002)
#define PWM_CH5_CSR_PH_CORRECT_MSB    _u(1)
#define PWM_CH5_CSR_PH_CORRECT_LSB    _u(1)
#define PWM_CH5_CSR_PH_CORRECT_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH5_CSR_EN
// Description : Enable the PWM channel.
#define PWM_CH5_CSR_EN_RESET  _u(0x0)
#define PWM_CH5_CSR_EN_BITS   _u(0x00000001)
#define PWM_CH5_CSR_EN_MSB    _u(0)
#define PWM_CH5_CSR_EN_LSB    _u(0)
#define PWM_CH5_CSR_EN_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH5_DIV
// Description : INT and FRAC form a fixed-point fractional number.
//               Counting rate is system clock frequency divided by this number.
//               Fractional division uses simple 1st-order sigma-delta.
#define PWM_CH5_DIV_OFFSET _u(0x00000068)
#define PWM_CH5_DIV_BITS   _u(0x00000fff)
#define PWM_CH5_DIV_RESET  _u(0x00000010)
// -----------------------------------------------------------------------------
// Field       : PWM_CH5_DIV_INT
#define PWM_CH5_DIV_INT_RESET  _u(0x01)
#define PWM_CH5_DIV_INT_BITS   _u(0x00000ff0)
#define PWM_CH5_DIV_INT_MSB    _u(11)
#define PWM_CH5_DIV_INT_LSB    _u(4)
#define PWM_CH5_DIV_INT_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH5_DIV_FRAC
#define PWM_CH5_DIV_FRAC_RESET  _u(0x0)
#define PWM_CH5_DIV_FRAC_BITS   _u(0x0000000f)
#define PWM_CH5_DIV_FRAC_MSB    _u(3)
#define PWM_CH5_DIV_FRAC_LSB    _u(0)
#define PWM_CH5_DIV_FRAC_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH5_CTR
// Description : Direct access to the PWM counter
#define PWM_CH5_CTR_OFFSET _u(0x0000006c)
#define PWM_CH5_CTR_BITS   _u(0x0000ffff)
#define PWM_CH5_CTR_RESET  _u(0x00000000)
#define PWM_CH5_CTR_MSB    _u(15)
#define PWM_CH5_CTR_LSB    _u(0)
#define PWM_CH5_CTR_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH5_CC
// Description : Counter compare values
#define PWM_CH5_CC_OFFSET _u(0x00000070)
#define PWM_CH5_CC_BITS   _u(0xffffffff)
#define PWM_CH5_CC_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : PWM_CH5_CC_B
#define PWM_CH5_CC_B_RESET  _u(0x0000)
#define PWM_CH5_CC_B_BITS   _u(0xffff0000)
#define PWM_CH5_CC_B_MSB    _u(31)
#define PWM_CH5_CC_B_LSB    _u(16)
#define PWM_CH5_CC_B_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH5_CC_A
#define PWM_CH5_CC_A_RESET  _u(0x0000)
#define PWM_CH5_CC_A_BITS   _u(0x0000ffff)
#define PWM_CH5_CC_A_MSB    _u(15)
#define PWM_CH5_CC_A_LSB    _u(0)
#define PWM_CH5_CC_A_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH5_TOP
// Description : Counter wrap value
#define PWM_CH5_TOP_OFFSET _u(0x00000074)
#define PWM_CH5_TOP_BITS   _u(0x0000ffff)
#define PWM_CH5_TOP_RESET  _u(0x0000ffff)
#define PWM_CH5_TOP_MSB    _u(15)
#define PWM_CH5_TOP_LSB    _u(0)
#define PWM_CH5_TOP_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH6_CSR
// Description : Control and status register
#define PWM_CH6_CSR_OFFSET _u(0x00000078)
#define PWM_CH6_CSR_BITS   _u(0x000000ff)
#define PWM_CH6_CSR_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : PWM_CH6_CSR_PH_ADV
// Description : Advance the phase of the counter by 1 count, while it is
//               running.
//               Self-clearing. Write a 1, and poll until low. Counter must be
//               running
//               at less than full speed (div_int + div_frac / 16 > 1)
#define PWM_CH6_CSR_PH_ADV_RESET  _u(0x0)
#define PWM_CH6_CSR_PH_ADV_BITS   _u(0x00000080)
#define PWM_CH6_CSR_PH_ADV_MSB    _u(7)
#define PWM_CH6_CSR_PH_ADV_LSB    _u(7)
#define PWM_CH6_CSR_PH_ADV_ACCESS "SC"
// -----------------------------------------------------------------------------
// Field       : PWM_CH6_CSR_PH_RET
// Description : Retard the phase of the counter by 1 count, while it is
//               running.
//               Self-clearing. Write a 1, and poll until low. Counter must be
//               running.
#define PWM_CH6_CSR_PH_RET_RESET  _u(0x0)
#define PWM_CH6_CSR_PH_RET_BITS   _u(0x00000040)
#define PWM_CH6_CSR_PH_RET_MSB    _u(6)
#define PWM_CH6_CSR_PH_RET_LSB    _u(6)
#define PWM_CH6_CSR_PH_RET_ACCESS "SC"
// -----------------------------------------------------------------------------
// Field       : PWM_CH6_CSR_DIVMODE
//               0x0 -> Free-running counting at rate dictated by fractional divider
//               0x1 -> Fractional divider operation is gated by the PWM B pin.
//               0x2 -> Counter advances with each rising edge of the PWM B pin.
//               0x3 -> Counter advances with each falling edge of the PWM B pin.
#define PWM_CH6_CSR_DIVMODE_RESET  _u(0x0)
#define PWM_CH6_CSR_DIVMODE_BITS   _u(0x00000030)
#define PWM_CH6_CSR_DIVMODE_MSB    _u(5)
#define PWM_CH6_CSR_DIVMODE_LSB    _u(4)
#define PWM_CH6_CSR_DIVMODE_ACCESS "RW"
#define PWM_CH6_CSR_DIVMODE_VALUE_DIV _u(0x0)
#define PWM_CH6_CSR_DIVMODE_VALUE_LEVEL _u(0x1)
#define PWM_CH6_CSR_DIVMODE_VALUE_RISE _u(0x2)
#define PWM_CH6_CSR_DIVMODE_VALUE_FALL _u(0x3)
// -----------------------------------------------------------------------------
// Field       : PWM_CH6_CSR_B_INV
// Description : Invert output B
#define PWM_CH6_CSR_B_INV_RESET  _u(0x0)
#define PWM_CH6_CSR_B_INV_BITS   _u(0x00000008)
#define PWM_CH6_CSR_B_INV_MSB    _u(3)
#define PWM_CH6_CSR_B_INV_LSB    _u(3)
#define PWM_CH6_CSR_B_INV_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH6_CSR_A_INV
// Description : Invert output A
#define PWM_CH6_CSR_A_INV_RESET  _u(0x0)
#define PWM_CH6_CSR_A_INV_BITS   _u(0x00000004)
#define PWM_CH6_CSR_A_INV_MSB    _u(2)
#define PWM_CH6_CSR_A_INV_LSB    _u(2)
#define PWM_CH6_CSR_A_INV_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH6_CSR_PH_CORRECT
// Description : 1: Enable phase-correct modulation. 0: Trailing-edge
#define PWM_CH6_CSR_PH_CORRECT_RESET  _u(0x0)
#define PWM_CH6_CSR_PH_CORRECT_BITS   _u(0x00000002)
#define PWM_CH6_CSR_PH_CORRECT_MSB    _u(1)
#define PWM_CH6_CSR_PH_CORRECT_LSB    _u(1)
#define PWM_CH6_CSR_PH_CORRECT_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH6_CSR_EN
// Description : Enable the PWM channel.
#define PWM_CH6_CSR_EN_RESET  _u(0x0)
#define PWM_CH6_CSR_EN_BITS   _u(0x00000001)
#define PWM_CH6_CSR_EN_MSB    _u(0)
#define PWM_CH6_CSR_EN_LSB    _u(0)
#define PWM_CH6_CSR_EN_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH6_DIV
// Description : INT and FRAC form a fixed-point fractional number.
//               Counting rate is system clock frequency divided by this number.
//               Fractional division uses simple 1st-order sigma-delta.
#define PWM_CH6_DIV_OFFSET _u(0x0000007c)
#define PWM_CH6_DIV_BITS   _u(0x00000fff)
#define PWM_CH6_DIV_RESET  _u(0x00000010)
// -----------------------------------------------------------------------------
// Field       : PWM_CH6_DIV_INT
#define PWM_CH6_DIV_INT_RESET  _u(0x01)
#define PWM_CH6_DIV_INT_BITS   _u(0x00000ff0)
#define PWM_CH6_DIV_INT_MSB    _u(11)
#define PWM_CH6_DIV_INT_LSB    _u(4)
#define PWM_CH6_DIV_INT_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH6_DIV_FRAC
#define PWM_CH6_DIV_FRAC_RESET  _u(0x0)
#define PWM_CH6_DIV_FRAC_BITS   _u(0x0000000f)
#define PWM_CH6_DIV_FRAC_MSB    _u(3)
#define PWM_CH6_DIV_FRAC_LSB    _u(0)
#define PWM_CH6_DIV_FRAC_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH6_CTR
// Description : Direct access to the PWM counter
#define PWM_CH6_CTR_OFFSET _u(0x00000080)
#define PWM_CH6_CTR_BITS   _u(0x0000ffff)
#define PWM_CH6_CTR_RESET  _u(0x00000000)
#define PWM_CH6_CTR_MSB    _u(15)
#define PWM_CH6_CTR_LSB    _u(0)
#define PWM_CH6_CTR_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH6_CC
// Description : Counter compare values
#define PWM_CH6_CC_OFFSET _u(0x00000084)
#define PWM_CH6_CC_BITS   _u(0xffffffff)
#define PWM_CH6_CC_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : PWM_CH6_CC_B
#define PWM_CH6_CC_B_RESET  _u(0x0000)
#define PWM_CH6_CC_B_BITS   _u(0xffff0000)
#define PWM_CH6_CC_B_MSB    _u(31)
#define PWM_CH6_CC_B_LSB    _u(16)
#define PWM_CH6_CC_B_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH6_CC_A
#define PWM_CH6_CC_A_RESET  _u(0x0000)
#define PWM_CH6_CC_A_BITS   _u(0x0000ffff)
#define PWM_CH6_CC_A_MSB    _u(15)
#define PWM_CH6_CC_A_LSB    _u(0)
#define PWM_CH6_CC_A_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH6_TOP
// Description : Counter wrap value
#define PWM_CH6_TOP_OFFSET _u(0x00000088)
#define PWM_CH6_TOP_BITS   _u(0x0000ffff)
#define PWM_CH6_TOP_RESET  _u(0x0000ffff)
#define PWM_CH6_TOP_MSB    _u(15)
#define PWM_CH6_TOP_LSB    _u(0)
#define PWM_CH6_TOP_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH7_CSR
// Description : Control and status register
#define PWM_CH7_CSR_OFFSET _u(0x0000008c)
#define PWM_CH7_CSR_BITS   _u(0x000000ff)
#define PWM_CH7_CSR_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : PWM_CH7_CSR_PH_ADV
// Description : Advance the phase of the counter by 1 count, while it is
//               running.
//               Self-clearing. Write a 1, and poll until low. Counter must be
//               running
//               at less than full speed (div_int + div_frac / 16 > 1)
#define PWM_CH7_CSR_PH_ADV_RESET  _u(0x0)
#define PWM_CH7_CSR_PH_ADV_BITS   _u(0x00000080)
#define PWM_CH7_CSR_PH_ADV_MSB    _u(7)
#define PWM_CH7_CSR_PH_ADV_LSB    _u(7)
#define PWM_CH7_CSR_PH_ADV_ACCESS "SC"
// -----------------------------------------------------------------------------
// Field       : PWM_CH7_CSR_PH_RET
// Description : Retard the phase of the counter by 1 count, while it is
//               running.
//               Self-clearing. Write a 1, and poll until low. Counter must be
//               running.
#define PWM_CH7_CSR_PH_RET_RESET  _u(0x0)
#define PWM_CH7_CSR_PH_RET_BITS   _u(0x00000040)
#define PWM_CH7_CSR_PH_RET_MSB    _u(6)
#define PWM_CH7_CSR_PH_RET_LSB    _u(6)
#define PWM_CH7_CSR_PH_RET_ACCESS "SC"
// -----------------------------------------------------------------------------
// Field       : PWM_CH7_CSR_DIVMODE
//               0x0 -> Free-running counting at rate dictated by fractional divider
//               0x1 -> Fractional divider operation is gated by the PWM B pin.
//               0x2 -> Counter advances with each rising edge of the PWM B pin.
//               0x3 -> Counter advances with each falling edge of the PWM B pin.
#define PWM_CH7_CSR_DIVMODE_RESET  _u(0x0)
#define PWM_CH7_CSR_DIVMODE_BITS   _u(0x00000030)
#define PWM_CH7_CSR_DIVMODE_MSB    _u(5)
#define PWM_CH7_CSR_DIVMODE_LSB    _u(4)
#define PWM_CH7_CSR_DIVMODE_ACCESS "RW"
#define PWM_CH7_CSR_DIVMODE_VALUE_DIV _u(0x0)
#define PWM_CH7_CSR_DIVMODE_VALUE_LEVEL _u(0x1)
#define PWM_CH7_CSR_DIVMODE_VALUE_RISE _u(0x2)
#define PWM_CH7_CSR_DIVMODE_VALUE_FALL _u(0x3)
// -----------------------------------------------------------------------------
// Field       : PWM_CH7_CSR_B_INV
// Description : Invert output B
#define PWM_CH7_CSR_B_INV_RESET  _u(0x0)
#define PWM_CH7_CSR_B_INV_BITS   _u(0x00000008)
#define PWM_CH7_CSR_B_INV_MSB    _u(3)
#define PWM_CH7_CSR_B_INV_LSB    _u(3)
#define PWM_CH7_CSR_B_INV_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH7_CSR_A_INV
// Description : Invert output A
#define PWM_CH7_CSR_A_INV_RESET  _u(0x0)
#define PWM_CH7_CSR_A_INV_BITS   _u(0x00000004)
#define PWM_CH7_CSR_A_INV_MSB    _u(2)
#define PWM_CH7_CSR_A_INV_LSB    _u(2)
#define PWM_CH7_CSR_A_INV_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH7_CSR_PH_CORRECT
// Description : 1: Enable phase-correct modulation. 0: Trailing-edge
#define PWM_CH7_CSR_PH_CORRECT_RESET  _u(0x0)
#define PWM_CH7_CSR_PH_CORRECT_BITS   _u(0x00000002)
#define PWM_CH7_CSR_PH_CORRECT_MSB    _u(1)
#define PWM_CH7_CSR_PH_CORRECT_LSB    _u(1)
#define PWM_CH7_CSR_PH_CORRECT_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH7_CSR_EN
// Description : Enable the PWM channel.
#define PWM_CH7_CSR_EN_RESET  _u(0x0)
#define PWM_CH7_CSR_EN_BITS   _u(0x00000001)
#define PWM_CH7_CSR_EN_MSB    _u(0)
#define PWM_CH7_CSR_EN_LSB    _u(0)
#define PWM_CH7_CSR_EN_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH7_DIV
// Description : INT and FRAC form a fixed-point fractional number.
//               Counting rate is system clock frequency divided by this number.
//               Fractional division uses simple 1st-order sigma-delta.
#define PWM_CH7_DIV_OFFSET _u(0x00000090)
#define PWM_CH7_DIV_BITS   _u(0x00000fff)
#define PWM_CH7_DIV_RESET  _u(0x00000010)
// -----------------------------------------------------------------------------
// Field       : PWM_CH7_DIV_INT
#define PWM_CH7_DIV_INT_RESET  _u(0x01)
#define PWM_CH7_DIV_INT_BITS   _u(0x00000ff0)
#define PWM_CH7_DIV_INT_MSB    _u(11)
#define PWM_CH7_DIV_INT_LSB    _u(4)
#define PWM_CH7_DIV_INT_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH7_DIV_FRAC
#define PWM_CH7_DIV_FRAC_RESET  _u(0x0)
#define PWM_CH7_DIV_FRAC_BITS   _u(0x0000000f)
#define PWM_CH7_DIV_FRAC_MSB    _u(3)
#define PWM_CH7_DIV_FRAC_LSB    _u(0)
#define PWM_CH7_DIV_FRAC_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH7_CTR
// Description : Direct access to the PWM counter
#define PWM_CH7_CTR_OFFSET _u(0x00000094)
#define PWM_CH7_CTR_BITS   _u(0x0000ffff)
#define PWM_CH7_CTR_RESET  _u(0x00000000)
#define PWM_CH7_CTR_MSB    _u(15)
#define PWM_CH7_CTR_LSB    _u(0)
#define PWM_CH7_CTR_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH7_CC
// Description : Counter compare values
#define PWM_CH7_CC_OFFSET _u(0x00000098)
#define PWM_CH7_CC_BITS   _u(0xffffffff)
#define PWM_CH7_CC_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : PWM_CH7_CC_B
#define PWM_CH7_CC_B_RESET  _u(0x0000)
#define PWM_CH7_CC_B_BITS   _u(0xffff0000)
#define PWM_CH7_CC_B_MSB    _u(31)
#define PWM_CH7_CC_B_LSB    _u(16)
#define PWM_CH7_CC_B_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH7_CC_A
#define PWM_CH7_CC_A_RESET  _u(0x0000)
#define PWM_CH7_CC_A_BITS   _u(0x0000ffff)
#define PWM_CH7_CC_A_MSB    _u(15)
#define PWM_CH7_CC_A_LSB    _u(0)
#define PWM_CH7_CC_A_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH7_TOP
// Description : Counter wrap value
#define PWM_CH7_TOP_OFFSET _u(0x0000009c)
#define PWM_CH7_TOP_BITS   _u(0x0000ffff)
#define PWM_CH7_TOP_RESET  _u(0x0000ffff)
#define PWM_CH7_TOP_MSB    _u(15)
#define PWM_CH7_TOP_LSB    _u(0)
#define PWM_CH7_TOP_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH8_CSR
// Description : Control and status register
#define PWM_CH8_CSR_OFFSET _u(0x000000a0)
#define PWM_CH8_CSR_BITS   _u(0x000000ff)
#define PWM_CH8_CSR_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : PWM_CH8_CSR_PH_ADV
// Description : Advance the phase of the counter by 1 count, while it is
//               running.
//               Self-clearing. Write a 1, and poll until low. Counter must be
//               running
//               at less than full speed (div_int + div_frac / 16 > 1)
#define PWM_CH8_CSR_PH_ADV_RESET  _u(0x0)
#define PWM_CH8_CSR_PH_ADV_BITS   _u(0x00000080)
#define PWM_CH8_CSR_PH_ADV_MSB    _u(7)
#define PWM_CH8_CSR_PH_ADV_LSB    _u(7)
#define PWM_CH8_CSR_PH_ADV_ACCESS "SC"
// -----------------------------------------------------------------------------
// Field       : PWM_CH8_CSR_PH_RET
// Description : Retard the phase of the counter by 1 count, while it is
//               running.
//               Self-clearing. Write a 1, and poll until low. Counter must be
//               running.
#define PWM_CH8_CSR_PH_RET_RESET  _u(0x0)
#define PWM_CH8_CSR_PH_RET_BITS   _u(0x00000040)
#define PWM_CH8_CSR_PH_RET_MSB    _u(6)
#define PWM_CH8_CSR_PH_RET_LSB    _u(6)
#define PWM_CH8_CSR_PH_RET_ACCESS "SC"
// -----------------------------------------------------------------------------
// Field       : PWM_CH8_CSR_DIVMODE
//               0x0 -> Free-running counting at rate dictated by fractional divider
//               0x1 -> Fractional divider operation is gated by the PWM B pin.
//               0x2 -> Counter advances with each rising edge of the PWM B pin.
//               0x3 -> Counter advances with each falling edge of the PWM B pin.
#define PWM_CH8_CSR_DIVMODE_RESET  _u(0x0)
#define PWM_CH8_CSR_DIVMODE_BITS   _u(0x00000030)
#define PWM_CH8_CSR_DIVMODE_MSB    _u(5)
#define PWM_CH8_CSR_DIVMODE_LSB    _u(4)
#define PWM_CH8_CSR_DIVMODE_ACCESS "RW"
#define PWM_CH8_CSR_DIVMODE_VALUE_DIV _u(0x0)
#define PWM_CH8_CSR_DIVMODE_VALUE_LEVEL _u(0x1)
#define PWM_CH8_CSR_DIVMODE_VALUE_RISE _u(0x2)
#define PWM_CH8_CSR_DIVMODE_VALUE_FALL _u(0x3)
// -----------------------------------------------------------------------------
// Field       : PWM_CH8_CSR_B_INV
// Description : Invert output B
#define PWM_CH8_CSR_B_INV_RESET  _u(0x0)
#define PWM_CH8_CSR_B_INV_BITS   _u(0x00000008)
#define PWM_CH8_CSR_B_INV_MSB    _u(3)
#define PWM_CH8_CSR_B_INV_LSB    _u(3)
#define PWM_CH8_CSR_B_INV_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH8_CSR_A_INV
// Description : Invert output A
#define PWM_CH8_CSR_A_INV_RESET  _u(0x0)
#define PWM_CH8_CSR_A_INV_BITS   _u(0x00000004)
#define PWM_CH8_CSR_A_INV_MSB    _u(2)
#define PWM_CH8_CSR_A_INV_LSB    _u(2)
#define PWM_CH8_CSR_A_INV_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH8_CSR_PH_CORRECT
// Description : 1: Enable phase-correct modulation. 0: Trailing-edge
#define PWM_CH8_CSR_PH_CORRECT_RESET  _u(0x0)
#define PWM_CH8_CSR_PH_CORRECT_BITS   _u(0x00000002)
#define PWM_CH8_CSR_PH_CORRECT_MSB    _u(1)
#define PWM_CH8_CSR_PH_CORRECT_LSB    _u(1)
#define PWM_CH8_CSR_PH_CORRECT_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH8_CSR_EN
// Description : Enable the PWM channel.
#define PWM_CH8_CSR_EN_RESET  _u(0x0)
#define PWM_CH8_CSR_EN_BITS   _u(0x00000001)
#define PWM_CH8_CSR_EN_MSB    _u(0)
#define PWM_CH8_CSR_EN_LSB    _u(0)
#define PWM_CH8_CSR_EN_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH8_DIV
// Description : INT and FRAC form a fixed-point fractional number.
//               Counting rate is system clock frequency divided by this number.
//               Fractional division uses simple 1st-order sigma-delta.
#define PWM_CH8_DIV_OFFSET _u(0x000000a4)
#define PWM_CH8_DIV_BITS   _u(0x00000fff)
#define PWM_CH8_DIV_RESET  _u(0x00000010)
// -----------------------------------------------------------------------------
// Field       : PWM_CH8_DIV_INT
#define PWM_CH8_DIV_INT_RESET  _u(0x01)
#define PWM_CH8_DIV_INT_BITS   _u(0x00000ff0)
#define PWM_CH8_DIV_INT_MSB    _u(11)
#define PWM_CH8_DIV_INT_LSB    _u(4)
#define PWM_CH8_DIV_INT_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH8_DIV_FRAC
#define PWM_CH8_DIV_FRAC_RESET  _u(0x0)
#define PWM_CH8_DIV_FRAC_BITS   _u(0x0000000f)
#define PWM_CH8_DIV_FRAC_MSB    _u(3)
#define PWM_CH8_DIV_FRAC_LSB    _u(0)
#define PWM_CH8_DIV_FRAC_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH8_CTR
// Description : Direct access to the PWM counter
#define PWM_CH8_CTR_OFFSET _u(0x000000a8)
#define PWM_CH8_CTR_BITS   _u(0x0000ffff)
#define PWM_CH8_CTR_RESET  _u(0x00000000)
#define PWM_CH8_CTR_MSB    _u(15)
#define PWM_CH8_CTR_LSB    _u(0)
#define PWM_CH8_CTR_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH8_CC
// Description : Counter compare values
#define PWM_CH8_CC_OFFSET _u(0x000000ac)
#define PWM_CH8_CC_BITS   _u(0xffffffff)
#define PWM_CH8_CC_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : PWM_CH8_CC_B
#define PWM_CH8_CC_B_RESET  _u(0x0000)
#define PWM_CH8_CC_B_BITS   _u(0xffff0000)
#define PWM_CH8_CC_B_MSB    _u(31)
#define PWM_CH8_CC_B_LSB    _u(16)
#define PWM_CH8_CC_B_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH8_CC_A
#define PWM_CH8_CC_A_RESET  _u(0x0000)
#define PWM_CH8_CC_A_BITS   _u(0x0000ffff)
#define PWM_CH8_CC_A_MSB    _u(15)
#define PWM_CH8_CC_A_LSB    _u(0)
#define PWM_CH8_CC_A_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH8_TOP
// Description : Counter wrap value
#define PWM_CH8_TOP_OFFSET _u(0x000000b0)
#define PWM_CH8_TOP_BITS   _u(0x0000ffff)
#define PWM_CH8_TOP_RESET  _u(0x0000ffff)
#define PWM_CH8_TOP_MSB    _u(15)
#define PWM_CH8_TOP_LSB    _u(0)
#define PWM_CH8_TOP_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH9_CSR
// Description : Control and status register
#define PWM_CH9_CSR_OFFSET _u(0x000000b4)
#define PWM_CH9_CSR_BITS   _u(0x000000ff)
#define PWM_CH9_CSR_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : PWM_CH9_CSR_PH_ADV
// Description : Advance the phase of the counter by 1 count, while it is
//               running.
//               Self-clearing. Write a 1, and poll until low. Counter must be
//               running
//               at less than full speed (div_int + div_frac / 16 > 1)
#define PWM_CH9_CSR_PH_ADV_RESET  _u(0x0)
#define PWM_CH9_CSR_PH_ADV_BITS   _u(0x00000080)
#define PWM_CH9_CSR_PH_ADV_MSB    _u(7)
#define PWM_CH9_CSR_PH_ADV_LSB    _u(7)
#define PWM_CH9_CSR_PH_ADV_ACCESS "SC"
// -----------------------------------------------------------------------------
// Field       : PWM_CH9_CSR_PH_RET
// Description : Retard the phase of the counter by 1 count, while it is
//               running.
//               Self-clearing. Write a 1, and poll until low. Counter must be
//               running.
#define PWM_CH9_CSR_PH_RET_RESET  _u(0x0)
#define PWM_CH9_CSR_PH_RET_BITS   _u(0x00000040)
#define PWM_CH9_CSR_PH_RET_MSB    _u(6)
#define PWM_CH9_CSR_PH_RET_LSB    _u(6)
#define PWM_CH9_CSR_PH_RET_ACCESS "SC"
// -----------------------------------------------------------------------------
// Field       : PWM_CH9_CSR_DIVMODE
//               0x0 -> Free-running counting at rate dictated by fractional divider
//               0x1 -> Fractional divider operation is gated by the PWM B pin.
//               0x2 -> Counter advances with each rising edge of the PWM B pin.
//               0x3 -> Counter advances with each falling edge of the PWM B pin.
#define PWM_CH9_CSR_DIVMODE_RESET  _u(0x0)
#define PWM_CH9_CSR_DIVMODE_BITS   _u(0x00000030)
#define PWM_CH9_CSR_DIVMODE_MSB    _u(5)
#define PWM_CH9_CSR_DIVMODE_LSB    _u(4)
#define PWM_CH9_CSR_DIVMODE_ACCESS "RW"
#define PWM_CH9_CSR_DIVMODE_VALUE_DIV _u(0x0)
#define PWM_CH9_CSR_DIVMODE_VALUE_LEVEL _u(0x1)
#define PWM_CH9_CSR_DIVMODE_VALUE_RISE _u(0x2)
#define PWM_CH9_CSR_DIVMODE_VALUE_FALL _u(0x3)
// -----------------------------------------------------------------------------
// Field       : PWM_CH9_CSR_B_INV
// Description : Invert output B
#define PWM_CH9_CSR_B_INV_RESET  _u(0x0)
#define PWM_CH9_CSR_B_INV_BITS   _u(0x00000008)
#define PWM_CH9_CSR_B_INV_MSB    _u(3)
#define PWM_CH9_CSR_B_INV_LSB    _u(3)
#define PWM_CH9_CSR_B_INV_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH9_CSR_A_INV
// Description : Invert output A
#define PWM_CH9_CSR_A_INV_RESET  _u(0x0)
#define PWM_CH9_CSR_A_INV_BITS   _u(0x00000004)
#define PWM_CH9_CSR_A_INV_MSB    _u(2)
#define PWM_CH9_CSR_A_INV_LSB    _u(2)
#define PWM_CH9_CSR_A_INV_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH9_CSR_PH_CORRECT
// Description : 1: Enable phase-correct modulation. 0: Trailing-edge
#define PWM_CH9_CSR_PH_CORRECT_RESET  _u(0x0)
#define PWM_CH9_CSR_PH_CORRECT_BITS   _u(0x00000002)
#define PWM_CH9_CSR_PH_CORRECT_MSB    _u(1)
#define PWM_CH9_CSR_PH_CORRECT_LSB    _u(1)
#define PWM_CH9_CSR_PH_CORRECT_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH9_CSR_EN
// Description : Enable the PWM channel.
#define PWM_CH9_CSR_EN_RESET  _u(0x0)
#define PWM_CH9_CSR_EN_BITS   _u(0x00000001)
#define PWM_CH9_CSR_EN_MSB    _u(0)
#define PWM_CH9_CSR_EN_LSB    _u(0)
#define PWM_CH9_CSR_EN_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH9_DIV
// Description : INT and FRAC form a fixed-point fractional number.
//               Counting rate is system clock frequency divided by this number.
//               Fractional division uses simple 1st-order sigma-delta.
#define PWM_CH9_DIV_OFFSET _u(0x000000b8)
#define PWM_CH9_DIV_BITS   _u(0x00000fff)
#define PWM_CH9_DIV_RESET  _u(0x00000010)
// -----------------------------------------------------------------------------
// Field       : PWM_CH9_DIV_INT
#define PWM_CH9_DIV_INT_RESET  _u(0x01)
#define PWM_CH9_DIV_INT_BITS   _u(0x00000ff0)
#define PWM_CH9_DIV_INT_MSB    _u(11)
#define PWM_CH9_DIV_INT_LSB    _u(4)
#define PWM_CH9_DIV_INT_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH9_DIV_FRAC
#define PWM_CH9_DIV_FRAC_RESET  _u(0x0)
#define PWM_CH9_DIV_FRAC_BITS   _u(0x0000000f)
#define PWM_CH9_DIV_FRAC_MSB    _u(3)
#define PWM_CH9_DIV_FRAC_LSB    _u(0)
#define PWM_CH9_DIV_FRAC_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH9_CTR
// Description : Direct access to the PWM counter
#define PWM_CH9_CTR_OFFSET _u(0x000000bc)
#define PWM_CH9_CTR_BITS   _u(0x0000ffff)
#define PWM_CH9_CTR_RESET  _u(0x00000000)
#define PWM_CH9_CTR_MSB    _u(15)
#define PWM_CH9_CTR_LSB    _u(0)
#define PWM_CH9_CTR_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH9_CC
// Description : Counter compare values
#define PWM_CH9_CC_OFFSET _u(0x000000c0)
#define PWM_CH9_CC_BITS   _u(0xffffffff)
#define PWM_CH9_CC_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : PWM_CH9_CC_B
#define PWM_CH9_CC_B_RESET  _u(0x0000)
#define PWM_CH9_CC_B_BITS   _u(0xffff0000)
#define PWM_CH9_CC_B_MSB    _u(31)
#define PWM_CH9_CC_B_LSB    _u(16)
#define PWM_CH9_CC_B_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH9_CC_A
#define PWM_CH9_CC_A_RESET  _u(0x0000)
#define PWM_CH9_CC_A_BITS   _u(0x0000ffff)
#define PWM_CH9_CC_A_MSB    _u(15)
#define PWM_CH9_CC_A_LSB    _u(0)
#define PWM_CH9_CC_A_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH9_TOP
// Description : Counter wrap value
#define PWM_CH9_TOP_OFFSET _u(0x000000c4)
#define PWM_CH9_TOP_BITS   _u(0x0000ffff)
#define PWM_CH9_TOP_RESET  _u(0x0000ffff)
#define PWM_CH9_TOP_MSB    _u(15)
#define PWM_CH9_TOP_LSB    _u(0)
#define PWM_CH9_TOP_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH10_CSR
// Description : Control and status register
#define PWM_CH10_CSR_OFFSET _u(0x000000c8)
#define PWM_CH10_CSR_BITS   _u(0x000000ff)
#define PWM_CH10_CSR_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : PWM_CH10_CSR_PH_ADV
// Description : Advance the phase of the counter by 1 count, while it is
//               running.
//               Self-clearing. Write a 1, and poll until low. Counter must be
//               running
//               at less than full speed (div_int + div_frac / 16 > 1)
#define PWM_CH10_CSR_PH_ADV_RESET  _u(0x0)
#define PWM_CH10_CSR_PH_ADV_BITS   _u(0x00000080)
#define PWM_CH10_CSR_PH_ADV_MSB    _u(7)
#define PWM_CH10_CSR_PH_ADV_LSB    _u(7)
#define PWM_CH10_CSR_PH_ADV_ACCESS "SC"
// -----------------------------------------------------------------------------
// Field       : PWM_CH10_CSR_PH_RET
// Description : Retard the phase of the counter by 1 count, while it is
//               running.
//               Self-clearing. Write a 1, and poll until low. Counter must be
//               running.
#define PWM_CH10_CSR_PH_RET_RESET  _u(0x0)
#define PWM_CH10_CSR_PH_RET_BITS   _u(0x00000040)
#define PWM_CH10_CSR_PH_RET_MSB    _u(6)
#define PWM_CH10_CSR_PH_RET_LSB    _u(6)
#define PWM_CH10_CSR_PH_RET_ACCESS "SC"
// -----------------------------------------------------------------------------
// Field       : PWM_CH10_CSR_DIVMODE
//               0x0 -> Free-running counting at rate dictated by fractional divider
//               0x1 -> Fractional divider operation is gated by the PWM B pin.
//               0x2 -> Counter advances with each rising edge of the PWM B pin.
//               0x3 -> Counter advances with each falling edge of the PWM B pin.
#define PWM_CH10_CSR_DIVMODE_RESET  _u(0x0)
#define PWM_CH10_CSR_DIVMODE_BITS   _u(0x00000030)
#define PWM_CH10_CSR_DIVMODE_MSB    _u(5)
#define PWM_CH10_CSR_DIVMODE_LSB    _u(4)
#define PWM_CH10_CSR_DIVMODE_ACCESS "RW"
#define PWM_CH10_CSR_DIVMODE_VALUE_DIV _u(0x0)
#define PWM_CH10_CSR_DIVMODE_VALUE_LEVEL _u(0x1)
#define PWM_CH10_CSR_DIVMODE_VALUE_RISE _u(0x2)
#define PWM_CH10_CSR_DIVMODE_VALUE_FALL _u(0x3)
// -----------------------------------------------------------------------------
// Field       : PWM_CH10_CSR_B_INV
// Description : Invert output B
#define PWM_CH10_CSR_B_INV_RESET  _u(0x0)
#define PWM_CH10_CSR_B_INV_BITS   _u(0x00000008)
#define PWM_CH10_CSR_B_INV_MSB    _u(3)
#define PWM_CH10_CSR_B_INV_LSB    _u(3)
#define PWM_CH10_CSR_B_INV_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH10_CSR_A_INV
// Description : Invert output A
#define PWM_CH10_CSR_A_INV_RESET  _u(0x0)
#define PWM_CH10_CSR_A_INV_BITS   _u(0x00000004)
#define PWM_CH10_CSR_A_INV_MSB    _u(2)
#define PWM_CH10_CSR_A_INV_LSB    _u(2)
#define PWM_CH10_CSR_A_INV_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH10_CSR_PH_CORRECT
// Description : 1: Enable phase-correct modulation. 0: Trailing-edge
#define PWM_CH10_CSR_PH_CORRECT_RESET  _u(0x0)
#define PWM_CH10_CSR_PH_CORRECT_BITS   _u(0x00000002)
#define PWM_CH10_CSR_PH_CORRECT_MSB    _u(1)
#define PWM_CH10_CSR_PH_CORRECT_LSB    _u(1)
#define PWM_CH10_CSR_PH_CORRECT_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH10_CSR_EN
// Description : Enable the PWM channel.
#define PWM_CH10_CSR_EN_RESET  _u(0x0)
#define PWM_CH10_CSR_EN_BITS   _u(0x00000001)
#define PWM_CH10_CSR_EN_MSB    _u(0)
#define PWM_CH10_CSR_EN_LSB    _u(0)
#define PWM_CH10_CSR_EN_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH10_DIV
// Description : INT and FRAC form a fixed-point fractional number.
//               Counting rate is system clock frequency divided by this number.
//               Fractional division uses simple 1st-order sigma-delta.
#define PWM_CH10_DIV_OFFSET _u(0x000000cc)
#define PWM_CH10_DIV_BITS   _u(0x00000fff)
#define PWM_CH10_DIV_RESET  _u(0x00000010)
// -----------------------------------------------------------------------------
// Field       : PWM_CH10_DIV_INT
#define PWM_CH10_DIV_INT_RESET  _u(0x01)
#define PWM_CH10_DIV_INT_BITS   _u(0x00000ff0)
#define PWM_CH10_DIV_INT_MSB    _u(11)
#define PWM_CH10_DIV_INT_LSB    _u(4)
#define PWM_CH10_DIV_INT_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH10_DIV_FRAC
#define PWM_CH10_DIV_FRAC_RESET  _u(0x0)
#define PWM_CH10_DIV_FRAC_BITS   _u(0x0000000f)
#define PWM_CH10_DIV_FRAC_MSB    _u(3)
#define PWM_CH10_DIV_FRAC_LSB    _u(0)
#define PWM_CH10_DIV_FRAC_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH10_CTR
// Description : Direct access to the PWM counter
#define PWM_CH10_CTR_OFFSET _u(0x000000d0)
#define PWM_CH10_CTR_BITS   _u(0x0000ffff)
#define PWM_CH10_CTR_RESET  _u(0x00000000)
#define PWM_CH10_CTR_MSB    _u(15)
#define PWM_CH10_CTR_LSB    _u(0)
#define PWM_CH10_CTR_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH10_CC
// Description : Counter compare values
#define PWM_CH10_CC_OFFSET _u(0x000000d4)
#define PWM_CH10_CC_BITS   _u(0xffffffff)
#define PWM_CH10_CC_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : PWM_CH10_CC_B
#define PWM_CH10_CC_B_RESET  _u(0x0000)
#define PWM_CH10_CC_B_BITS   _u(0xffff0000)
#define PWM_CH10_CC_B_MSB    _u(31)
#define PWM_CH10_CC_B_LSB    _u(16)
#define PWM_CH10_CC_B_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH10_CC_A
#define PWM_CH10_CC_A_RESET  _u(0x0000)
#define PWM_CH10_CC_A_BITS   _u(0x0000ffff)
#define PWM_CH10_CC_A_MSB    _u(15)
#define PWM_CH10_CC_A_LSB    _u(0)
#define PWM_CH10_CC_A_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH10_TOP
// Description : Counter wrap value
#define PWM_CH10_TOP_OFFSET _u(0x000000d8)
#define PWM_CH10_TOP_BITS   _u(0x0000ffff)
#define PWM_CH10_TOP_RESET  _u(0x0000ffff)
#define PWM_CH10_TOP_MSB    _u(15)
#define PWM_CH10_TOP_LSB    _u(0)
#define PWM_CH10_TOP_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH11_CSR
// Description : Control and status register
#define PWM_CH11_CSR_OFFSET _u(0x000000dc)
#define PWM_CH11_CSR_BITS   _u(0x000000ff)
#define PWM_CH11_CSR_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : PWM_CH11_CSR_PH_ADV
// Description : Advance the phase of the counter by 1 count, while it is
//               running.
//               Self-clearing. Write a 1, and poll until low. Counter must be
//               running
//               at less than full speed (div_int + div_frac / 16 > 1)
#define PWM_CH11_CSR_PH_ADV_RESET  _u(0x0)
#define PWM_CH11_CSR_PH_ADV_BITS   _u(0x00000080)
#define PWM_CH11_CSR_PH_ADV_MSB    _u(7)
#define PWM_CH11_CSR_PH_ADV_LSB    _u(7)
#define PWM_CH11_CSR_PH_ADV_ACCESS "SC"
// -----------------------------------------------------------------------------
// Field       : PWM_CH11_CSR_PH_RET
// Description : Retard the phase of the counter by 1 count, while it is
//               running.
//               Self-clearing. Write a 1, and poll until low. Counter must be
//               running.
#define PWM_CH11_CSR_PH_RET_RESET  _u(0x0)
#define PWM_CH11_CSR_PH_RET_BITS   _u(0x00000040)
#define PWM_CH11_CSR_PH_RET_MSB    _u(6)
#define PWM_CH11_CSR_PH_RET_LSB    _u(6)
#define PWM_CH11_CSR_PH_RET_ACCESS "SC"
// -----------------------------------------------------------------------------
// Field       : PWM_CH11_CSR_DIVMODE
//               0x0 -> Free-running counting at rate dictated by fractional divider
//               0x1 -> Fractional divider operation is gated by the PWM B pin.
//               0x2 -> Counter advances with each rising edge of the PWM B pin.
//               0x3 -> Counter advances with each falling edge of the PWM B pin.
#define PWM_CH11_CSR_DIVMODE_RESET  _u(0x0)
#define PWM_CH11_CSR_DIVMODE_BITS   _u(0x00000030)
#define PWM_CH11_CSR_DIVMODE_MSB    _u(5)
#define PWM_CH11_CSR_DIVMODE_LSB    _u(4)
#define PWM_CH11_CSR_DIVMODE_ACCESS "RW"
#define PWM_CH11_CSR_DIVMODE_VALUE_DIV _u(0x0)
#define PWM_CH11_CSR_DIVMODE_VALUE_LEVEL _u(0x1)
#define PWM_CH11_CSR_DIVMODE_VALUE_RISE _u(0x2)
#define PWM_CH11_CSR_DIVMODE_VALUE_FALL _u(0x3)
// -----------------------------------------------------------------------------
// Field       : PWM_CH11_CSR_B_INV
// Description : Invert output B
#define PWM_CH11_CSR_B_INV_RESET  _u(0x0)
#define PWM_CH11_CSR_B_INV_BITS   _u(0x00000008)
#define PWM_CH11_CSR_B_INV_MSB    _u(3)
#define PWM_CH11_CSR_B_INV_LSB    _u(3)
#define PWM_CH11_CSR_B_INV_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH11_CSR_A_INV
// Description : Invert output A
#define PWM_CH11_CSR_A_INV_RESET  _u(0x0)
#define PWM_CH11_CSR_A_INV_BITS   _u(0x00000004)
#define PWM_CH11_CSR_A_INV_MSB    _u(2)
#define PWM_CH11_CSR_A_INV_LSB    _u(2)
#define PWM_CH11_CSR_A_INV_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH11_CSR_PH_CORRECT
// Description : 1: Enable phase-correct modulation. 0: Trailing-edge
#define PWM_CH11_CSR_PH_CORRECT_RESET  _u(0x0)
#define PWM_CH11_CSR_PH_CORRECT_BITS   _u(0x00000002)
#define PWM_CH11_CSR_PH_CORRECT_MSB    _u(1)
#define PWM_CH11_CSR_PH_CORRECT_LSB    _u(1)
#define PWM_CH11_CSR_PH_CORRECT_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH11_CSR_EN
// Description : Enable the PWM channel.
#define PWM_CH11_CSR_EN_RESET  _u(0x0)
#define PWM_CH11_CSR_EN_BITS   _u(0x00000001)
#define PWM_CH11_CSR_EN_MSB    _u(0)
#define PWM_CH11_CSR_EN_LSB    _u(0)
#define PWM_CH11_CSR_EN_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH11_DIV
// Description : INT and FRAC form a fixed-point fractional number.
//               Counting rate is system clock frequency divided by this number.
//               Fractional division uses simple 1st-order sigma-delta.
#define PWM_CH11_DIV_OFFSET _u(0x000000e0)
#define PWM_CH11_DIV_BITS   _u(0x00000fff)
#define PWM_CH11_DIV_RESET  _u(0x00000010)
// -----------------------------------------------------------------------------
// Field       : PWM_CH11_DIV_INT
#define PWM_CH11_DIV_INT_RESET  _u(0x01)
#define PWM_CH11_DIV_INT_BITS   _u(0x00000ff0)
#define PWM_CH11_DIV_INT_MSB    _u(11)
#define PWM_CH11_DIV_INT_LSB    _u(4)
#define PWM_CH11_DIV_INT_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH11_DIV_FRAC
#define PWM_CH11_DIV_FRAC_RESET  _u(0x0)
#define PWM_CH11_DIV_FRAC_BITS   _u(0x0000000f)
#define PWM_CH11_DIV_FRAC_MSB    _u(3)
#define PWM_CH11_DIV_FRAC_LSB    _u(0)
#define PWM_CH11_DIV_FRAC_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH11_CTR
// Description : Direct access to the PWM counter
#define PWM_CH11_CTR_OFFSET _u(0x000000e4)
#define PWM_CH11_CTR_BITS   _u(0x0000ffff)
#define PWM_CH11_CTR_RESET  _u(0x00000000)
#define PWM_CH11_CTR_MSB    _u(15)
#define PWM_CH11_CTR_LSB    _u(0)
#define PWM_CH11_CTR_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH11_CC
// Description : Counter compare values
#define PWM_CH11_CC_OFFSET _u(0x000000e8)
#define PWM_CH11_CC_BITS   _u(0xffffffff)
#define PWM_CH11_CC_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : PWM_CH11_CC_B
#define PWM_CH11_CC_B_RESET  _u(0x0000)
#define PWM_CH11_CC_B_BITS   _u(0xffff0000)
#define PWM_CH11_CC_B_MSB    _u(31)
#define PWM_CH11_CC_B_LSB    _u(16)
#define PWM_CH11_CC_B_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_CH11_CC_A
#define PWM_CH11_CC_A_RESET  _u(0x0000)
#define PWM_CH11_CC_A_BITS   _u(0x0000ffff)
#define PWM_CH11_CC_A_MSB    _u(15)
#define PWM_CH11_CC_A_LSB    _u(0)
#define PWM_CH11_CC_A_ACCESS "RW"
// =============================================================================
// Register    : PWM_CH11_TOP
// Description : Counter wrap value
#define PWM_CH11_TOP_OFFSET _u(0x000000ec)
#define PWM_CH11_TOP_BITS   _u(0x0000ffff)
#define PWM_CH11_TOP_RESET  _u(0x0000ffff)
#define PWM_CH11_TOP_MSB    _u(15)
#define PWM_CH11_TOP_LSB    _u(0)
#define PWM_CH11_TOP_ACCESS "RW"
// =============================================================================
// Register    : PWM_EN
// Description : This register aliases the CSR_EN bits for all channels.
//               Writing to this register allows multiple channels to be enabled
//               or disabled simultaneously, so they can run in perfect sync.
//               For each channel, there is only one physical EN register bit,
//               which can be accessed through here or CHx_CSR.
#define PWM_EN_OFFSET _u(0x000000f0)
#define PWM_EN_BITS   _u(0x00000fff)
#define PWM_EN_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : PWM_EN_CH11
#define PWM_EN_CH11_RESET  _u(0x0)
#define PWM_EN_CH11_BITS   _u(0x00000800)
#define PWM_EN_CH11_MSB    _u(11)
#define PWM_EN_CH11_LSB    _u(11)
#define PWM_EN_CH11_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_EN_CH10
#define PWM_EN_CH10_RESET  _u(0x0)
#define PWM_EN_CH10_BITS   _u(0x00000400)
#define PWM_EN_CH10_MSB    _u(10)
#define PWM_EN_CH10_LSB    _u(10)
#define PWM_EN_CH10_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_EN_CH9
#define PWM_EN_CH9_RESET  _u(0x0)
#define PWM_EN_CH9_BITS   _u(0x00000200)
#define PWM_EN_CH9_MSB    _u(9)
#define PWM_EN_CH9_LSB    _u(9)
#define PWM_EN_CH9_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_EN_CH8
#define PWM_EN_CH8_RESET  _u(0x0)
#define PWM_EN_CH8_BITS   _u(0x00000100)
#define PWM_EN_CH8_MSB    _u(8)
#define PWM_EN_CH8_LSB    _u(8)
#define PWM_EN_CH8_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_EN_CH7
#define PWM_EN_CH7_RESET  _u(0x0)
#define PWM_EN_CH7_BITS   _u(0x00000080)
#define PWM_EN_CH7_MSB    _u(7)
#define PWM_EN_CH7_LSB    _u(7)
#define PWM_EN_CH7_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_EN_CH6
#define PWM_EN_CH6_RESET  _u(0x0)
#define PWM_EN_CH6_BITS   _u(0x00000040)
#define PWM_EN_CH6_MSB    _u(6)
#define PWM_EN_CH6_LSB    _u(6)
#define PWM_EN_CH6_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_EN_CH5
#define PWM_EN_CH5_RESET  _u(0x0)
#define PWM_EN_CH5_BITS   _u(0x00000020)
#define PWM_EN_CH5_MSB    _u(5)
#define PWM_EN_CH5_LSB    _u(5)
#define PWM_EN_CH5_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_EN_CH4
#define PWM_EN_CH4_RESET  _u(0x0)
#define PWM_EN_CH4_BITS   _u(0x00000010)
#define PWM_EN_CH4_MSB    _u(4)
#define PWM_EN_CH4_LSB    _u(4)
#define PWM_EN_CH4_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_EN_CH3
#define PWM_EN_CH3_RESET  _u(0x0)
#define PWM_EN_CH3_BITS   _u(0x00000008)
#define PWM_EN_CH3_MSB    _u(3)
#define PWM_EN_CH3_LSB    _u(3)
#define PWM_EN_CH3_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_EN_CH2
#define PWM_EN_CH2_RESET  _u(0x0)
#define PWM_EN_CH2_BITS   _u(0x00000004)
#define PWM_EN_CH2_MSB    _u(2)
#define PWM_EN_CH2_LSB    _u(2)
#define PWM_EN_CH2_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_EN_CH1
#define PWM_EN_CH1_RESET  _u(0x0)
#define PWM_EN_CH1_BITS   _u(0x00000002)
#define PWM_EN_CH1_MSB    _u(1)
#define PWM_EN_CH1_LSB    _u(1)
#define PWM_EN_CH1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_EN_CH0
#define PWM_EN_CH0_RESET  _u(0x0)
#define PWM_EN_CH0_BITS   _u(0x00000001)
#define PWM_EN_CH0_MSB    _u(0)
#define PWM_EN_CH0_LSB    _u(0)
#define PWM_EN_CH0_ACCESS "RW"
// =============================================================================
// Register    : PWM_INTR
// Description : Raw Interrupts
#define PWM_INTR_OFFSET _u(0x000000f4)
#define PWM_INTR_BITS   _u(0x00000fff)
#define PWM_INTR_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : PWM_INTR_CH11
#define PWM_INTR_CH11_RESET  _u(0x0)
#define PWM_INTR_CH11_BITS   _u(0x00000800)
#define PWM_INTR_CH11_MSB    _u(11)
#define PWM_INTR_CH11_LSB    _u(11)
#define PWM_INTR_CH11_ACCESS "WC"
// -----------------------------------------------------------------------------
// Field       : PWM_INTR_CH10
#define PWM_INTR_CH10_RESET  _u(0x0)
#define PWM_INTR_CH10_BITS   _u(0x00000400)
#define PWM_INTR_CH10_MSB    _u(10)
#define PWM_INTR_CH10_LSB    _u(10)
#define PWM_INTR_CH10_ACCESS "WC"
// -----------------------------------------------------------------------------
// Field       : PWM_INTR_CH9
#define PWM_INTR_CH9_RESET  _u(0x0)
#define PWM_INTR_CH9_BITS   _u(0x00000200)
#define PWM_INTR_CH9_MSB    _u(9)
#define PWM_INTR_CH9_LSB    _u(9)
#define PWM_INTR_CH9_ACCESS "WC"
// -----------------------------------------------------------------------------
// Field       : PWM_INTR_CH8
#define PWM_INTR_CH8_RESET  _u(0x0)
#define PWM_INTR_CH8_BITS   _u(0x00000100)
#define PWM_INTR_CH8_MSB    _u(8)
#define PWM_INTR_CH8_LSB    _u(8)
#define PWM_INTR_CH8_ACCESS "WC"
// -----------------------------------------------------------------------------
// Field       : PWM_INTR_CH7
#define PWM_INTR_CH7_RESET  _u(0x0)
#define PWM_INTR_CH7_BITS   _u(0x00000080)
#define PWM_INTR_CH7_MSB    _u(7)
#define PWM_INTR_CH7_LSB    _u(7)
#define PWM_INTR_CH7_ACCESS "WC"
// -----------------------------------------------------------------------------
// Field       : PWM_INTR_CH6
#define PWM_INTR_CH6_RESET  _u(0x0)
#define PWM_INTR_CH6_BITS   _u(0x00000040)
#define PWM_INTR_CH6_MSB    _u(6)
#define PWM_INTR_CH6_LSB    _u(6)
#define PWM_INTR_CH6_ACCESS "WC"
// -----------------------------------------------------------------------------
// Field       : PWM_INTR_CH5
#define PWM_INTR_CH5_RESET  _u(0x0)
#define PWM_INTR_CH5_BITS   _u(0x00000020)
#define PWM_INTR_CH5_MSB    _u(5)
#define PWM_INTR_CH5_LSB    _u(5)
#define PWM_INTR_CH5_ACCESS "WC"
// -----------------------------------------------------------------------------
// Field       : PWM_INTR_CH4
#define PWM_INTR_CH4_RESET  _u(0x0)
#define PWM_INTR_CH4_BITS   _u(0x00000010)
#define PWM_INTR_CH4_MSB    _u(4)
#define PWM_INTR_CH4_LSB    _u(4)
#define PWM_INTR_CH4_ACCESS "WC"
// -----------------------------------------------------------------------------
// Field       : PWM_INTR_CH3
#define PWM_INTR_CH3_RESET  _u(0x0)
#define PWM_INTR_CH3_BITS   _u(0x00000008)
#define PWM_INTR_CH3_MSB    _u(3)
#define PWM_INTR_CH3_LSB    _u(3)
#define PWM_INTR_CH3_ACCESS "WC"
// -----------------------------------------------------------------------------
// Field       : PWM_INTR_CH2
#define PWM_INTR_CH2_RESET  _u(0x0)
#define PWM_INTR_CH2_BITS   _u(0x00000004)
#define PWM_INTR_CH2_MSB    _u(2)
#define PWM_INTR_CH2_LSB    _u(2)
#define PWM_INTR_CH2_ACCESS "WC"
// -----------------------------------------------------------------------------
// Field       : PWM_INTR_CH1
#define PWM_INTR_CH1_RESET  _u(0x0)
#define PWM_INTR_CH1_BITS   _u(0x00000002)
#define PWM_INTR_CH1_MSB    _u(1)
#define PWM_INTR_CH1_LSB    _u(1)
#define PWM_INTR_CH1_ACCESS "WC"
// -----------------------------------------------------------------------------
// Field       : PWM_INTR_CH0
#define PWM_INTR_CH0_RESET  _u(0x0)
#define PWM_INTR_CH0_BITS   _u(0x00000001)
#define PWM_INTR_CH0_MSB    _u(0)
#define PWM_INTR_CH0_LSB    _u(0)
#define PWM_INTR_CH0_ACCESS "WC"
// =============================================================================
// Register    : PWM_IRQ0_INTE
// Description : Interrupt Enable for irq0
#define PWM_IRQ0_INTE_OFFSET _u(0x000000f8)
#define PWM_IRQ0_INTE_BITS   _u(0x00000fff)
#define PWM_IRQ0_INTE_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ0_INTE_CH11
#define PWM_IRQ0_INTE_CH11_RESET  _u(0x0)
#define PWM_IRQ0_INTE_CH11_BITS   _u(0x00000800)
#define PWM_IRQ0_INTE_CH11_MSB    _u(11)
#define PWM_IRQ0_INTE_CH11_LSB    _u(11)
#define PWM_IRQ0_INTE_CH11_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ0_INTE_CH10
#define PWM_IRQ0_INTE_CH10_RESET  _u(0x0)
#define PWM_IRQ0_INTE_CH10_BITS   _u(0x00000400)
#define PWM_IRQ0_INTE_CH10_MSB    _u(10)
#define PWM_IRQ0_INTE_CH10_LSB    _u(10)
#define PWM_IRQ0_INTE_CH10_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ0_INTE_CH9
#define PWM_IRQ0_INTE_CH9_RESET  _u(0x0)
#define PWM_IRQ0_INTE_CH9_BITS   _u(0x00000200)
#define PWM_IRQ0_INTE_CH9_MSB    _u(9)
#define PWM_IRQ0_INTE_CH9_LSB    _u(9)
#define PWM_IRQ0_INTE_CH9_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ0_INTE_CH8
#define PWM_IRQ0_INTE_CH8_RESET  _u(0x0)
#define PWM_IRQ0_INTE_CH8_BITS   _u(0x00000100)
#define PWM_IRQ0_INTE_CH8_MSB    _u(8)
#define PWM_IRQ0_INTE_CH8_LSB    _u(8)
#define PWM_IRQ0_INTE_CH8_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ0_INTE_CH7
#define PWM_IRQ0_INTE_CH7_RESET  _u(0x0)
#define PWM_IRQ0_INTE_CH7_BITS   _u(0x00000080)
#define PWM_IRQ0_INTE_CH7_MSB    _u(7)
#define PWM_IRQ0_INTE_CH7_LSB    _u(7)
#define PWM_IRQ0_INTE_CH7_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ0_INTE_CH6
#define PWM_IRQ0_INTE_CH6_RESET  _u(0x0)
#define PWM_IRQ0_INTE_CH6_BITS   _u(0x00000040)
#define PWM_IRQ0_INTE_CH6_MSB    _u(6)
#define PWM_IRQ0_INTE_CH6_LSB    _u(6)
#define PWM_IRQ0_INTE_CH6_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ0_INTE_CH5
#define PWM_IRQ0_INTE_CH5_RESET  _u(0x0)
#define PWM_IRQ0_INTE_CH5_BITS   _u(0x00000020)
#define PWM_IRQ0_INTE_CH5_MSB    _u(5)
#define PWM_IRQ0_INTE_CH5_LSB    _u(5)
#define PWM_IRQ0_INTE_CH5_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ0_INTE_CH4
#define PWM_IRQ0_INTE_CH4_RESET  _u(0x0)
#define PWM_IRQ0_INTE_CH4_BITS   _u(0x00000010)
#define PWM_IRQ0_INTE_CH4_MSB    _u(4)
#define PWM_IRQ0_INTE_CH4_LSB    _u(4)
#define PWM_IRQ0_INTE_CH4_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ0_INTE_CH3
#define PWM_IRQ0_INTE_CH3_RESET  _u(0x0)
#define PWM_IRQ0_INTE_CH3_BITS   _u(0x00000008)
#define PWM_IRQ0_INTE_CH3_MSB    _u(3)
#define PWM_IRQ0_INTE_CH3_LSB    _u(3)
#define PWM_IRQ0_INTE_CH3_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ0_INTE_CH2
#define PWM_IRQ0_INTE_CH2_RESET  _u(0x0)
#define PWM_IRQ0_INTE_CH2_BITS   _u(0x00000004)
#define PWM_IRQ0_INTE_CH2_MSB    _u(2)
#define PWM_IRQ0_INTE_CH2_LSB    _u(2)
#define PWM_IRQ0_INTE_CH2_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ0_INTE_CH1
#define PWM_IRQ0_INTE_CH1_RESET  _u(0x0)
#define PWM_IRQ0_INTE_CH1_BITS   _u(0x00000002)
#define PWM_IRQ0_INTE_CH1_MSB    _u(1)
#define PWM_IRQ0_INTE_CH1_LSB    _u(1)
#define PWM_IRQ0_INTE_CH1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ0_INTE_CH0
#define PWM_IRQ0_INTE_CH0_RESET  _u(0x0)
#define PWM_IRQ0_INTE_CH0_BITS   _u(0x00000001)
#define PWM_IRQ0_INTE_CH0_MSB    _u(0)
#define PWM_IRQ0_INTE_CH0_LSB    _u(0)
#define PWM_IRQ0_INTE_CH0_ACCESS "RW"
// =============================================================================
// Register    : PWM_IRQ0_INTF
// Description : Interrupt Force for irq0
#define PWM_IRQ0_INTF_OFFSET _u(0x000000fc)
#define PWM_IRQ0_INTF_BITS   _u(0x00000fff)
#define PWM_IRQ0_INTF_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ0_INTF_CH11
#define PWM_IRQ0_INTF_CH11_RESET  _u(0x0)
#define PWM_IRQ0_INTF_CH11_BITS   _u(0x00000800)
#define PWM_IRQ0_INTF_CH11_MSB    _u(11)
#define PWM_IRQ0_INTF_CH11_LSB    _u(11)
#define PWM_IRQ0_INTF_CH11_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ0_INTF_CH10
#define PWM_IRQ0_INTF_CH10_RESET  _u(0x0)
#define PWM_IRQ0_INTF_CH10_BITS   _u(0x00000400)
#define PWM_IRQ0_INTF_CH10_MSB    _u(10)
#define PWM_IRQ0_INTF_CH10_LSB    _u(10)
#define PWM_IRQ0_INTF_CH10_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ0_INTF_CH9
#define PWM_IRQ0_INTF_CH9_RESET  _u(0x0)
#define PWM_IRQ0_INTF_CH9_BITS   _u(0x00000200)
#define PWM_IRQ0_INTF_CH9_MSB    _u(9)
#define PWM_IRQ0_INTF_CH9_LSB    _u(9)
#define PWM_IRQ0_INTF_CH9_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ0_INTF_CH8
#define PWM_IRQ0_INTF_CH8_RESET  _u(0x0)
#define PWM_IRQ0_INTF_CH8_BITS   _u(0x00000100)
#define PWM_IRQ0_INTF_CH8_MSB    _u(8)
#define PWM_IRQ0_INTF_CH8_LSB    _u(8)
#define PWM_IRQ0_INTF_CH8_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ0_INTF_CH7
#define PWM_IRQ0_INTF_CH7_RESET  _u(0x0)
#define PWM_IRQ0_INTF_CH7_BITS   _u(0x00000080)
#define PWM_IRQ0_INTF_CH7_MSB    _u(7)
#define PWM_IRQ0_INTF_CH7_LSB    _u(7)
#define PWM_IRQ0_INTF_CH7_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ0_INTF_CH6
#define PWM_IRQ0_INTF_CH6_RESET  _u(0x0)
#define PWM_IRQ0_INTF_CH6_BITS   _u(0x00000040)
#define PWM_IRQ0_INTF_CH6_MSB    _u(6)
#define PWM_IRQ0_INTF_CH6_LSB    _u(6)
#define PWM_IRQ0_INTF_CH6_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ0_INTF_CH5
#define PWM_IRQ0_INTF_CH5_RESET  _u(0x0)
#define PWM_IRQ0_INTF_CH5_BITS   _u(0x00000020)
#define PWM_IRQ0_INTF_CH5_MSB    _u(5)
#define PWM_IRQ0_INTF_CH5_LSB    _u(5)
#define PWM_IRQ0_INTF_CH5_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ0_INTF_CH4
#define PWM_IRQ0_INTF_CH4_RESET  _u(0x0)
#define PWM_IRQ0_INTF_CH4_BITS   _u(0x00000010)
#define PWM_IRQ0_INTF_CH4_MSB    _u(4)
#define PWM_IRQ0_INTF_CH4_LSB    _u(4)
#define PWM_IRQ0_INTF_CH4_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ0_INTF_CH3
#define PWM_IRQ0_INTF_CH3_RESET  _u(0x0)
#define PWM_IRQ0_INTF_CH3_BITS   _u(0x00000008)
#define PWM_IRQ0_INTF_CH3_MSB    _u(3)
#define PWM_IRQ0_INTF_CH3_LSB    _u(3)
#define PWM_IRQ0_INTF_CH3_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ0_INTF_CH2
#define PWM_IRQ0_INTF_CH2_RESET  _u(0x0)
#define PWM_IRQ0_INTF_CH2_BITS   _u(0x00000004)
#define PWM_IRQ0_INTF_CH2_MSB    _u(2)
#define PWM_IRQ0_INTF_CH2_LSB    _u(2)
#define PWM_IRQ0_INTF_CH2_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ0_INTF_CH1
#define PWM_IRQ0_INTF_CH1_RESET  _u(0x0)
#define PWM_IRQ0_INTF_CH1_BITS   _u(0x00000002)
#define PWM_IRQ0_INTF_CH1_MSB    _u(1)
#define PWM_IRQ0_INTF_CH1_LSB    _u(1)
#define PWM_IRQ0_INTF_CH1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ0_INTF_CH0
#define PWM_IRQ0_INTF_CH0_RESET  _u(0x0)
#define PWM_IRQ0_INTF_CH0_BITS   _u(0x00000001)
#define PWM_IRQ0_INTF_CH0_MSB    _u(0)
#define PWM_IRQ0_INTF_CH0_LSB    _u(0)
#define PWM_IRQ0_INTF_CH0_ACCESS "RW"
// =============================================================================
// Register    : PWM_IRQ0_INTS
// Description : Interrupt status after masking & forcing for irq0
#define PWM_IRQ0_INTS_OFFSET _u(0x00000100)
#define PWM_IRQ0_INTS_BITS   _u(0x00000fff)
#define PWM_IRQ0_INTS_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ0_INTS_CH11
#define PWM_IRQ0_INTS_CH11_RESET  _u(0x0)
#define PWM_IRQ0_INTS_CH11_BITS   _u(0x00000800)
#define PWM_IRQ0_INTS_CH11_MSB    _u(11)
#define PWM_IRQ0_INTS_CH11_LSB    _u(11)
#define PWM_IRQ0_INTS_CH11_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ0_INTS_CH10
#define PWM_IRQ0_INTS_CH10_RESET  _u(0x0)
#define PWM_IRQ0_INTS_CH10_BITS   _u(0x00000400)
#define PWM_IRQ0_INTS_CH10_MSB    _u(10)
#define PWM_IRQ0_INTS_CH10_LSB    _u(10)
#define PWM_IRQ0_INTS_CH10_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ0_INTS_CH9
#define PWM_IRQ0_INTS_CH9_RESET  _u(0x0)
#define PWM_IRQ0_INTS_CH9_BITS   _u(0x00000200)
#define PWM_IRQ0_INTS_CH9_MSB    _u(9)
#define PWM_IRQ0_INTS_CH9_LSB    _u(9)
#define PWM_IRQ0_INTS_CH9_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ0_INTS_CH8
#define PWM_IRQ0_INTS_CH8_RESET  _u(0x0)
#define PWM_IRQ0_INTS_CH8_BITS   _u(0x00000100)
#define PWM_IRQ0_INTS_CH8_MSB    _u(8)
#define PWM_IRQ0_INTS_CH8_LSB    _u(8)
#define PWM_IRQ0_INTS_CH8_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ0_INTS_CH7
#define PWM_IRQ0_INTS_CH7_RESET  _u(0x0)
#define PWM_IRQ0_INTS_CH7_BITS   _u(0x00000080)
#define PWM_IRQ0_INTS_CH7_MSB    _u(7)
#define PWM_IRQ0_INTS_CH7_LSB    _u(7)
#define PWM_IRQ0_INTS_CH7_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ0_INTS_CH6
#define PWM_IRQ0_INTS_CH6_RESET  _u(0x0)
#define PWM_IRQ0_INTS_CH6_BITS   _u(0x00000040)
#define PWM_IRQ0_INTS_CH6_MSB    _u(6)
#define PWM_IRQ0_INTS_CH6_LSB    _u(6)
#define PWM_IRQ0_INTS_CH6_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ0_INTS_CH5
#define PWM_IRQ0_INTS_CH5_RESET  _u(0x0)
#define PWM_IRQ0_INTS_CH5_BITS   _u(0x00000020)
#define PWM_IRQ0_INTS_CH5_MSB    _u(5)
#define PWM_IRQ0_INTS_CH5_LSB    _u(5)
#define PWM_IRQ0_INTS_CH5_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ0_INTS_CH4
#define PWM_IRQ0_INTS_CH4_RESET  _u(0x0)
#define PWM_IRQ0_INTS_CH4_BITS   _u(0x00000010)
#define PWM_IRQ0_INTS_CH4_MSB    _u(4)
#define PWM_IRQ0_INTS_CH4_LSB    _u(4)
#define PWM_IRQ0_INTS_CH4_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ0_INTS_CH3
#define PWM_IRQ0_INTS_CH3_RESET  _u(0x0)
#define PWM_IRQ0_INTS_CH3_BITS   _u(0x00000008)
#define PWM_IRQ0_INTS_CH3_MSB    _u(3)
#define PWM_IRQ0_INTS_CH3_LSB    _u(3)
#define PWM_IRQ0_INTS_CH3_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ0_INTS_CH2
#define PWM_IRQ0_INTS_CH2_RESET  _u(0x0)
#define PWM_IRQ0_INTS_CH2_BITS   _u(0x00000004)
#define PWM_IRQ0_INTS_CH2_MSB    _u(2)
#define PWM_IRQ0_INTS_CH2_LSB    _u(2)
#define PWM_IRQ0_INTS_CH2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ0_INTS_CH1
#define PWM_IRQ0_INTS_CH1_RESET  _u(0x0)
#define PWM_IRQ0_INTS_CH1_BITS   _u(0x00000002)
#define PWM_IRQ0_INTS_CH1_MSB    _u(1)
#define PWM_IRQ0_INTS_CH1_LSB    _u(1)
#define PWM_IRQ0_INTS_CH1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ0_INTS_CH0
#define PWM_IRQ0_INTS_CH0_RESET  _u(0x0)
#define PWM_IRQ0_INTS_CH0_BITS   _u(0x00000001)
#define PWM_IRQ0_INTS_CH0_MSB    _u(0)
#define PWM_IRQ0_INTS_CH0_LSB    _u(0)
#define PWM_IRQ0_INTS_CH0_ACCESS "RO"
// =============================================================================
// Register    : PWM_IRQ1_INTE
// Description : Interrupt Enable for irq1
#define PWM_IRQ1_INTE_OFFSET _u(0x00000104)
#define PWM_IRQ1_INTE_BITS   _u(0x00000fff)
#define PWM_IRQ1_INTE_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ1_INTE_CH11
#define PWM_IRQ1_INTE_CH11_RESET  _u(0x0)
#define PWM_IRQ1_INTE_CH11_BITS   _u(0x00000800)
#define PWM_IRQ1_INTE_CH11_MSB    _u(11)
#define PWM_IRQ1_INTE_CH11_LSB    _u(11)
#define PWM_IRQ1_INTE_CH11_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ1_INTE_CH10
#define PWM_IRQ1_INTE_CH10_RESET  _u(0x0)
#define PWM_IRQ1_INTE_CH10_BITS   _u(0x00000400)
#define PWM_IRQ1_INTE_CH10_MSB    _u(10)
#define PWM_IRQ1_INTE_CH10_LSB    _u(10)
#define PWM_IRQ1_INTE_CH10_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ1_INTE_CH9
#define PWM_IRQ1_INTE_CH9_RESET  _u(0x0)
#define PWM_IRQ1_INTE_CH9_BITS   _u(0x00000200)
#define PWM_IRQ1_INTE_CH9_MSB    _u(9)
#define PWM_IRQ1_INTE_CH9_LSB    _u(9)
#define PWM_IRQ1_INTE_CH9_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ1_INTE_CH8
#define PWM_IRQ1_INTE_CH8_RESET  _u(0x0)
#define PWM_IRQ1_INTE_CH8_BITS   _u(0x00000100)
#define PWM_IRQ1_INTE_CH8_MSB    _u(8)
#define PWM_IRQ1_INTE_CH8_LSB    _u(8)
#define PWM_IRQ1_INTE_CH8_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ1_INTE_CH7
#define PWM_IRQ1_INTE_CH7_RESET  _u(0x0)
#define PWM_IRQ1_INTE_CH7_BITS   _u(0x00000080)
#define PWM_IRQ1_INTE_CH7_MSB    _u(7)
#define PWM_IRQ1_INTE_CH7_LSB    _u(7)
#define PWM_IRQ1_INTE_CH7_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ1_INTE_CH6
#define PWM_IRQ1_INTE_CH6_RESET  _u(0x0)
#define PWM_IRQ1_INTE_CH6_BITS   _u(0x00000040)
#define PWM_IRQ1_INTE_CH6_MSB    _u(6)
#define PWM_IRQ1_INTE_CH6_LSB    _u(6)
#define PWM_IRQ1_INTE_CH6_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ1_INTE_CH5
#define PWM_IRQ1_INTE_CH5_RESET  _u(0x0)
#define PWM_IRQ1_INTE_CH5_BITS   _u(0x00000020)
#define PWM_IRQ1_INTE_CH5_MSB    _u(5)
#define PWM_IRQ1_INTE_CH5_LSB    _u(5)
#define PWM_IRQ1_INTE_CH5_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ1_INTE_CH4
#define PWM_IRQ1_INTE_CH4_RESET  _u(0x0)
#define PWM_IRQ1_INTE_CH4_BITS   _u(0x00000010)
#define PWM_IRQ1_INTE_CH4_MSB    _u(4)
#define PWM_IRQ1_INTE_CH4_LSB    _u(4)
#define PWM_IRQ1_INTE_CH4_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ1_INTE_CH3
#define PWM_IRQ1_INTE_CH3_RESET  _u(0x0)
#define PWM_IRQ1_INTE_CH3_BITS   _u(0x00000008)
#define PWM_IRQ1_INTE_CH3_MSB    _u(3)
#define PWM_IRQ1_INTE_CH3_LSB    _u(3)
#define PWM_IRQ1_INTE_CH3_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ1_INTE_CH2
#define PWM_IRQ1_INTE_CH2_RESET  _u(0x0)
#define PWM_IRQ1_INTE_CH2_BITS   _u(0x00000004)
#define PWM_IRQ1_INTE_CH2_MSB    _u(2)
#define PWM_IRQ1_INTE_CH2_LSB    _u(2)
#define PWM_IRQ1_INTE_CH2_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ1_INTE_CH1
#define PWM_IRQ1_INTE_CH1_RESET  _u(0x0)
#define PWM_IRQ1_INTE_CH1_BITS   _u(0x00000002)
#define PWM_IRQ1_INTE_CH1_MSB    _u(1)
#define PWM_IRQ1_INTE_CH1_LSB    _u(1)
#define PWM_IRQ1_INTE_CH1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ1_INTE_CH0
#define PWM_IRQ1_INTE_CH0_RESET  _u(0x0)
#define PWM_IRQ1_INTE_CH0_BITS   _u(0x00000001)
#define PWM_IRQ1_INTE_CH0_MSB    _u(0)
#define PWM_IRQ1_INTE_CH0_LSB    _u(0)
#define PWM_IRQ1_INTE_CH0_ACCESS "RW"
// =============================================================================
// Register    : PWM_IRQ1_INTF
// Description : Interrupt Force for irq1
#define PWM_IRQ1_INTF_OFFSET _u(0x00000108)
#define PWM_IRQ1_INTF_BITS   _u(0x00000fff)
#define PWM_IRQ1_INTF_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ1_INTF_CH11
#define PWM_IRQ1_INTF_CH11_RESET  _u(0x0)
#define PWM_IRQ1_INTF_CH11_BITS   _u(0x00000800)
#define PWM_IRQ1_INTF_CH11_MSB    _u(11)
#define PWM_IRQ1_INTF_CH11_LSB    _u(11)
#define PWM_IRQ1_INTF_CH11_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ1_INTF_CH10
#define PWM_IRQ1_INTF_CH10_RESET  _u(0x0)
#define PWM_IRQ1_INTF_CH10_BITS   _u(0x00000400)
#define PWM_IRQ1_INTF_CH10_MSB    _u(10)
#define PWM_IRQ1_INTF_CH10_LSB    _u(10)
#define PWM_IRQ1_INTF_CH10_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ1_INTF_CH9
#define PWM_IRQ1_INTF_CH9_RESET  _u(0x0)
#define PWM_IRQ1_INTF_CH9_BITS   _u(0x00000200)
#define PWM_IRQ1_INTF_CH9_MSB    _u(9)
#define PWM_IRQ1_INTF_CH9_LSB    _u(9)
#define PWM_IRQ1_INTF_CH9_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ1_INTF_CH8
#define PWM_IRQ1_INTF_CH8_RESET  _u(0x0)
#define PWM_IRQ1_INTF_CH8_BITS   _u(0x00000100)
#define PWM_IRQ1_INTF_CH8_MSB    _u(8)
#define PWM_IRQ1_INTF_CH8_LSB    _u(8)
#define PWM_IRQ1_INTF_CH8_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ1_INTF_CH7
#define PWM_IRQ1_INTF_CH7_RESET  _u(0x0)
#define PWM_IRQ1_INTF_CH7_BITS   _u(0x00000080)
#define PWM_IRQ1_INTF_CH7_MSB    _u(7)
#define PWM_IRQ1_INTF_CH7_LSB    _u(7)
#define PWM_IRQ1_INTF_CH7_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ1_INTF_CH6
#define PWM_IRQ1_INTF_CH6_RESET  _u(0x0)
#define PWM_IRQ1_INTF_CH6_BITS   _u(0x00000040)
#define PWM_IRQ1_INTF_CH6_MSB    _u(6)
#define PWM_IRQ1_INTF_CH6_LSB    _u(6)
#define PWM_IRQ1_INTF_CH6_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ1_INTF_CH5
#define PWM_IRQ1_INTF_CH5_RESET  _u(0x0)
#define PWM_IRQ1_INTF_CH5_BITS   _u(0x00000020)
#define PWM_IRQ1_INTF_CH5_MSB    _u(5)
#define PWM_IRQ1_INTF_CH5_LSB    _u(5)
#define PWM_IRQ1_INTF_CH5_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ1_INTF_CH4
#define PWM_IRQ1_INTF_CH4_RESET  _u(0x0)
#define PWM_IRQ1_INTF_CH4_BITS   _u(0x00000010)
#define PWM_IRQ1_INTF_CH4_MSB    _u(4)
#define PWM_IRQ1_INTF_CH4_LSB    _u(4)
#define PWM_IRQ1_INTF_CH4_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ1_INTF_CH3
#define PWM_IRQ1_INTF_CH3_RESET  _u(0x0)
#define PWM_IRQ1_INTF_CH3_BITS   _u(0x00000008)
#define PWM_IRQ1_INTF_CH3_MSB    _u(3)
#define PWM_IRQ1_INTF_CH3_LSB    _u(3)
#define PWM_IRQ1_INTF_CH3_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ1_INTF_CH2
#define PWM_IRQ1_INTF_CH2_RESET  _u(0x0)
#define PWM_IRQ1_INTF_CH2_BITS   _u(0x00000004)
#define PWM_IRQ1_INTF_CH2_MSB    _u(2)
#define PWM_IRQ1_INTF_CH2_LSB    _u(2)
#define PWM_IRQ1_INTF_CH2_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ1_INTF_CH1
#define PWM_IRQ1_INTF_CH1_RESET  _u(0x0)
#define PWM_IRQ1_INTF_CH1_BITS   _u(0x00000002)
#define PWM_IRQ1_INTF_CH1_MSB    _u(1)
#define PWM_IRQ1_INTF_CH1_LSB    _u(1)
#define PWM_IRQ1_INTF_CH1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ1_INTF_CH0
#define PWM_IRQ1_INTF_CH0_RESET  _u(0x0)
#define PWM_IRQ1_INTF_CH0_BITS   _u(0x00000001)
#define PWM_IRQ1_INTF_CH0_MSB    _u(0)
#define PWM_IRQ1_INTF_CH0_LSB    _u(0)
#define PWM_IRQ1_INTF_CH0_ACCESS "RW"
// =============================================================================
// Register    : PWM_IRQ1_INTS
// Description : Interrupt status after masking & forcing for irq1
#define PWM_IRQ1_INTS_OFFSET _u(0x0000010c)
#define PWM_IRQ1_INTS_BITS   _u(0x00000fff)
#define PWM_IRQ1_INTS_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ1_INTS_CH11
#define PWM_IRQ1_INTS_CH11_RESET  _u(0x0)
#define PWM_IRQ1_INTS_CH11_BITS   _u(0x00000800)
#define PWM_IRQ1_INTS_CH11_MSB    _u(11)
#define PWM_IRQ1_INTS_CH11_LSB    _u(11)
#define PWM_IRQ1_INTS_CH11_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ1_INTS_CH10
#define PWM_IRQ1_INTS_CH10_RESET  _u(0x0)
#define PWM_IRQ1_INTS_CH10_BITS   _u(0x00000400)
#define PWM_IRQ1_INTS_CH10_MSB    _u(10)
#define PWM_IRQ1_INTS_CH10_LSB    _u(10)
#define PWM_IRQ1_INTS_CH10_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ1_INTS_CH9
#define PWM_IRQ1_INTS_CH9_RESET  _u(0x0)
#define PWM_IRQ1_INTS_CH9_BITS   _u(0x00000200)
#define PWM_IRQ1_INTS_CH9_MSB    _u(9)
#define PWM_IRQ1_INTS_CH9_LSB    _u(9)
#define PWM_IRQ1_INTS_CH9_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ1_INTS_CH8
#define PWM_IRQ1_INTS_CH8_RESET  _u(0x0)
#define PWM_IRQ1_INTS_CH8_BITS   _u(0x00000100)
#define PWM_IRQ1_INTS_CH8_MSB    _u(8)
#define PWM_IRQ1_INTS_CH8_LSB    _u(8)
#define PWM_IRQ1_INTS_CH8_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ1_INTS_CH7
#define PWM_IRQ1_INTS_CH7_RESET  _u(0x0)
#define PWM_IRQ1_INTS_CH7_BITS   _u(0x00000080)
#define PWM_IRQ1_INTS_CH7_MSB    _u(7)
#define PWM_IRQ1_INTS_CH7_LSB    _u(7)
#define PWM_IRQ1_INTS_CH7_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ1_INTS_CH6
#define PWM_IRQ1_INTS_CH6_RESET  _u(0x0)
#define PWM_IRQ1_INTS_CH6_BITS   _u(0x00000040)
#define PWM_IRQ1_INTS_CH6_MSB    _u(6)
#define PWM_IRQ1_INTS_CH6_LSB    _u(6)
#define PWM_IRQ1_INTS_CH6_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ1_INTS_CH5
#define PWM_IRQ1_INTS_CH5_RESET  _u(0x0)
#define PWM_IRQ1_INTS_CH5_BITS   _u(0x00000020)
#define PWM_IRQ1_INTS_CH5_MSB    _u(5)
#define PWM_IRQ1_INTS_CH5_LSB    _u(5)
#define PWM_IRQ1_INTS_CH5_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ1_INTS_CH4
#define PWM_IRQ1_INTS_CH4_RESET  _u(0x0)
#define PWM_IRQ1_INTS_CH4_BITS   _u(0x00000010)
#define PWM_IRQ1_INTS_CH4_MSB    _u(4)
#define PWM_IRQ1_INTS_CH4_LSB    _u(4)
#define PWM_IRQ1_INTS_CH4_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ1_INTS_CH3
#define PWM_IRQ1_INTS_CH3_RESET  _u(0x0)
#define PWM_IRQ1_INTS_CH3_BITS   _u(0x00000008)
#define PWM_IRQ1_INTS_CH3_MSB    _u(3)
#define PWM_IRQ1_INTS_CH3_LSB    _u(3)
#define PWM_IRQ1_INTS_CH3_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ1_INTS_CH2
#define PWM_IRQ1_INTS_CH2_RESET  _u(0x0)
#define PWM_IRQ1_INTS_CH2_BITS   _u(0x00000004)
#define PWM_IRQ1_INTS_CH2_MSB    _u(2)
#define PWM_IRQ1_INTS_CH2_LSB    _u(2)
#define PWM_IRQ1_INTS_CH2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ1_INTS_CH1
#define PWM_IRQ1_INTS_CH1_RESET  _u(0x0)
#define PWM_IRQ1_INTS_CH1_BITS   _u(0x00000002)
#define PWM_IRQ1_INTS_CH1_MSB    _u(1)
#define PWM_IRQ1_INTS_CH1_LSB    _u(1)
#define PWM_IRQ1_INTS_CH1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : PWM_IRQ1_INTS_CH0
#define PWM_IRQ1_INTS_CH0_RESET  _u(0x0)
#define PWM_IRQ1_INTS_CH0_BITS   _u(0x00000001)
#define PWM_IRQ1_INTS_CH0_MSB    _u(0)
#define PWM_IRQ1_INTS_CH0_LSB    _u(0)
#define PWM_IRQ1_INTS_CH0_ACCESS "RO"
// =============================================================================
#endif // _HARDWARE_REGS_PWM_H

