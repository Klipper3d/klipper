// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
// =============================================================================
// Register block : TRNG
// Version        : 1
// Bus type       : apb
// Description    : ARM TrustZone RNG register block
// =============================================================================
#ifndef _HARDWARE_REGS_TRNG_H
#define _HARDWARE_REGS_TRNG_H
// =============================================================================
// Register    : TRNG_RNG_IMR
// Description : Interrupt masking.
#define TRNG_RNG_IMR_OFFSET _u(0x00000100)
#define TRNG_RNG_IMR_BITS   _u(0xffffffff)
#define TRNG_RNG_IMR_RESET  _u(0x0000000f)
// -----------------------------------------------------------------------------
// Field       : TRNG_RNG_IMR_RESERVED
// Description : RESERVED
#define TRNG_RNG_IMR_RESERVED_RESET  _u(0x0000000)
#define TRNG_RNG_IMR_RESERVED_BITS   _u(0xfffffff0)
#define TRNG_RNG_IMR_RESERVED_MSB    _u(31)
#define TRNG_RNG_IMR_RESERVED_LSB    _u(4)
#define TRNG_RNG_IMR_RESERVED_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : TRNG_RNG_IMR_VN_ERR_INT_MASK
// Description : 1'b1-mask interrupt, no interrupt will be generated. See
//               RNG_ISR for an explanation on this interrupt.
#define TRNG_RNG_IMR_VN_ERR_INT_MASK_RESET  _u(0x1)
#define TRNG_RNG_IMR_VN_ERR_INT_MASK_BITS   _u(0x00000008)
#define TRNG_RNG_IMR_VN_ERR_INT_MASK_MSB    _u(3)
#define TRNG_RNG_IMR_VN_ERR_INT_MASK_LSB    _u(3)
#define TRNG_RNG_IMR_VN_ERR_INT_MASK_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : TRNG_RNG_IMR_CRNGT_ERR_INT_MASK
// Description : 1'b1-mask interrupt, no interrupt will be generated. See
//               RNG_ISR for an explanation on this interrupt.
#define TRNG_RNG_IMR_CRNGT_ERR_INT_MASK_RESET  _u(0x1)
#define TRNG_RNG_IMR_CRNGT_ERR_INT_MASK_BITS   _u(0x00000004)
#define TRNG_RNG_IMR_CRNGT_ERR_INT_MASK_MSB    _u(2)
#define TRNG_RNG_IMR_CRNGT_ERR_INT_MASK_LSB    _u(2)
#define TRNG_RNG_IMR_CRNGT_ERR_INT_MASK_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : TRNG_RNG_IMR_AUTOCORR_ERR_INT_MASK
// Description : 1'b1-mask interrupt, no interrupt will be generated. See
//               RNG_ISR for an explanation on this interrupt.
#define TRNG_RNG_IMR_AUTOCORR_ERR_INT_MASK_RESET  _u(0x1)
#define TRNG_RNG_IMR_AUTOCORR_ERR_INT_MASK_BITS   _u(0x00000002)
#define TRNG_RNG_IMR_AUTOCORR_ERR_INT_MASK_MSB    _u(1)
#define TRNG_RNG_IMR_AUTOCORR_ERR_INT_MASK_LSB    _u(1)
#define TRNG_RNG_IMR_AUTOCORR_ERR_INT_MASK_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : TRNG_RNG_IMR_EHR_VALID_INT_MASK
// Description : 1'b1-mask interrupt, no interrupt will be generated. See
//               RNG_ISR for an explanation on this interrupt.
#define TRNG_RNG_IMR_EHR_VALID_INT_MASK_RESET  _u(0x1)
#define TRNG_RNG_IMR_EHR_VALID_INT_MASK_BITS   _u(0x00000001)
#define TRNG_RNG_IMR_EHR_VALID_INT_MASK_MSB    _u(0)
#define TRNG_RNG_IMR_EHR_VALID_INT_MASK_LSB    _u(0)
#define TRNG_RNG_IMR_EHR_VALID_INT_MASK_ACCESS "RW"
// =============================================================================
// Register    : TRNG_RNG_ISR
// Description : RNG status register. If corresponding RNG_IMR bit is unmasked,
//               an interrupt will be generated.
#define TRNG_RNG_ISR_OFFSET _u(0x00000104)
#define TRNG_RNG_ISR_BITS   _u(0xffffffff)
#define TRNG_RNG_ISR_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : TRNG_RNG_ISR_RESERVED
// Description : RESERVED
#define TRNG_RNG_ISR_RESERVED_RESET  _u(0x0000000)
#define TRNG_RNG_ISR_RESERVED_BITS   _u(0xfffffff0)
#define TRNG_RNG_ISR_RESERVED_MSB    _u(31)
#define TRNG_RNG_ISR_RESERVED_LSB    _u(4)
#define TRNG_RNG_ISR_RESERVED_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : TRNG_RNG_ISR_VN_ERR
// Description : 1'b1 indicates Von Neuman error. Error in von Neuman occurs if
//               32 consecutive collected bits are identical, ZERO or ONE.
#define TRNG_RNG_ISR_VN_ERR_RESET  _u(0x0)
#define TRNG_RNG_ISR_VN_ERR_BITS   _u(0x00000008)
#define TRNG_RNG_ISR_VN_ERR_MSB    _u(3)
#define TRNG_RNG_ISR_VN_ERR_LSB    _u(3)
#define TRNG_RNG_ISR_VN_ERR_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : TRNG_RNG_ISR_CRNGT_ERR
// Description : 1'b1 indicates CRNGT in the RNG test failed. Failure occurs
//               when two consecutive blocks of 16 collected bits are equal.
#define TRNG_RNG_ISR_CRNGT_ERR_RESET  _u(0x0)
#define TRNG_RNG_ISR_CRNGT_ERR_BITS   _u(0x00000004)
#define TRNG_RNG_ISR_CRNGT_ERR_MSB    _u(2)
#define TRNG_RNG_ISR_CRNGT_ERR_LSB    _u(2)
#define TRNG_RNG_ISR_CRNGT_ERR_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : TRNG_RNG_ISR_AUTOCORR_ERR
// Description : 1'b1 indicates Autocorrelation test failed four times in a row.
//               When set, RNG cease from functioning until next reset.
#define TRNG_RNG_ISR_AUTOCORR_ERR_RESET  _u(0x0)
#define TRNG_RNG_ISR_AUTOCORR_ERR_BITS   _u(0x00000002)
#define TRNG_RNG_ISR_AUTOCORR_ERR_MSB    _u(1)
#define TRNG_RNG_ISR_AUTOCORR_ERR_LSB    _u(1)
#define TRNG_RNG_ISR_AUTOCORR_ERR_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : TRNG_RNG_ISR_EHR_VALID
// Description : 1'b1 indicates that 192 bits have been collected in the RNG,
//               and are ready to be read.
#define TRNG_RNG_ISR_EHR_VALID_RESET  _u(0x0)
#define TRNG_RNG_ISR_EHR_VALID_BITS   _u(0x00000001)
#define TRNG_RNG_ISR_EHR_VALID_MSB    _u(0)
#define TRNG_RNG_ISR_EHR_VALID_LSB    _u(0)
#define TRNG_RNG_ISR_EHR_VALID_ACCESS "RO"
// =============================================================================
// Register    : TRNG_RNG_ICR
// Description : Interrupt/status bit clear Register.
#define TRNG_RNG_ICR_OFFSET _u(0x00000108)
#define TRNG_RNG_ICR_BITS   _u(0xffffffff)
#define TRNG_RNG_ICR_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : TRNG_RNG_ICR_RESERVED
// Description : RESERVED
#define TRNG_RNG_ICR_RESERVED_RESET  _u(0x0000000)
#define TRNG_RNG_ICR_RESERVED_BITS   _u(0xfffffff0)
#define TRNG_RNG_ICR_RESERVED_MSB    _u(31)
#define TRNG_RNG_ICR_RESERVED_LSB    _u(4)
#define TRNG_RNG_ICR_RESERVED_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : TRNG_RNG_ICR_VN_ERR
// Description : Write 1'b1 - clear corresponding bit in RNG_ISR.
#define TRNG_RNG_ICR_VN_ERR_RESET  _u(0x0)
#define TRNG_RNG_ICR_VN_ERR_BITS   _u(0x00000008)
#define TRNG_RNG_ICR_VN_ERR_MSB    _u(3)
#define TRNG_RNG_ICR_VN_ERR_LSB    _u(3)
#define TRNG_RNG_ICR_VN_ERR_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : TRNG_RNG_ICR_CRNGT_ERR
// Description : Write 1'b1 - clear corresponding bit in RNG_ISR.
#define TRNG_RNG_ICR_CRNGT_ERR_RESET  _u(0x0)
#define TRNG_RNG_ICR_CRNGT_ERR_BITS   _u(0x00000004)
#define TRNG_RNG_ICR_CRNGT_ERR_MSB    _u(2)
#define TRNG_RNG_ICR_CRNGT_ERR_LSB    _u(2)
#define TRNG_RNG_ICR_CRNGT_ERR_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : TRNG_RNG_ICR_AUTOCORR_ERR
// Description : Cannot be cleared by SW! Only RNG reset clears this bit.
#define TRNG_RNG_ICR_AUTOCORR_ERR_RESET  _u(0x0)
#define TRNG_RNG_ICR_AUTOCORR_ERR_BITS   _u(0x00000002)
#define TRNG_RNG_ICR_AUTOCORR_ERR_MSB    _u(1)
#define TRNG_RNG_ICR_AUTOCORR_ERR_LSB    _u(1)
#define TRNG_RNG_ICR_AUTOCORR_ERR_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : TRNG_RNG_ICR_EHR_VALID
// Description : Write 1'b1 - clear corresponding bit in RNG_ISR.
#define TRNG_RNG_ICR_EHR_VALID_RESET  _u(0x0)
#define TRNG_RNG_ICR_EHR_VALID_BITS   _u(0x00000001)
#define TRNG_RNG_ICR_EHR_VALID_MSB    _u(0)
#define TRNG_RNG_ICR_EHR_VALID_LSB    _u(0)
#define TRNG_RNG_ICR_EHR_VALID_ACCESS "RW"
// =============================================================================
// Register    : TRNG_TRNG_CONFIG
// Description : Selecting the inverter-chain length.
#define TRNG_TRNG_CONFIG_OFFSET _u(0x0000010c)
#define TRNG_TRNG_CONFIG_BITS   _u(0xffffffff)
#define TRNG_TRNG_CONFIG_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : TRNG_TRNG_CONFIG_RESERVED
// Description : RESERVED
#define TRNG_TRNG_CONFIG_RESERVED_RESET  _u(0x00000000)
#define TRNG_TRNG_CONFIG_RESERVED_BITS   _u(0xfffffffc)
#define TRNG_TRNG_CONFIG_RESERVED_MSB    _u(31)
#define TRNG_TRNG_CONFIG_RESERVED_LSB    _u(2)
#define TRNG_TRNG_CONFIG_RESERVED_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : TRNG_TRNG_CONFIG_RND_SRC_SEL
// Description : Selects the number of inverters (out of four possible
//               selections) in the ring oscillator (the entropy source).
#define TRNG_TRNG_CONFIG_RND_SRC_SEL_RESET  _u(0x0)
#define TRNG_TRNG_CONFIG_RND_SRC_SEL_BITS   _u(0x00000003)
#define TRNG_TRNG_CONFIG_RND_SRC_SEL_MSB    _u(1)
#define TRNG_TRNG_CONFIG_RND_SRC_SEL_LSB    _u(0)
#define TRNG_TRNG_CONFIG_RND_SRC_SEL_ACCESS "RW"
// =============================================================================
// Register    : TRNG_TRNG_VALID
// Description : 192 bit collection indication.
#define TRNG_TRNG_VALID_OFFSET _u(0x00000110)
#define TRNG_TRNG_VALID_BITS   _u(0xffffffff)
#define TRNG_TRNG_VALID_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : TRNG_TRNG_VALID_RESERVED
// Description : RESERVED
#define TRNG_TRNG_VALID_RESERVED_RESET  _u(0x00000000)
#define TRNG_TRNG_VALID_RESERVED_BITS   _u(0xfffffffe)
#define TRNG_TRNG_VALID_RESERVED_MSB    _u(31)
#define TRNG_TRNG_VALID_RESERVED_LSB    _u(1)
#define TRNG_TRNG_VALID_RESERVED_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : TRNG_TRNG_VALID_EHR_VALID
// Description : 1'b1 indicates that collection of bits in the RNG is completed,
//               and data can be read from EHR_DATA register.
#define TRNG_TRNG_VALID_EHR_VALID_RESET  _u(0x0)
#define TRNG_TRNG_VALID_EHR_VALID_BITS   _u(0x00000001)
#define TRNG_TRNG_VALID_EHR_VALID_MSB    _u(0)
#define TRNG_TRNG_VALID_EHR_VALID_LSB    _u(0)
#define TRNG_TRNG_VALID_EHR_VALID_ACCESS "RO"
// =============================================================================
// Register    : TRNG_EHR_DATA0
// Description : RNG collected bits.
//               Bits [31:0] of Entropy Holding Register (EHR) - RNG output
//               register
#define TRNG_EHR_DATA0_OFFSET _u(0x00000114)
#define TRNG_EHR_DATA0_BITS   _u(0xffffffff)
#define TRNG_EHR_DATA0_RESET  _u(0x00000000)
#define TRNG_EHR_DATA0_MSB    _u(31)
#define TRNG_EHR_DATA0_LSB    _u(0)
#define TRNG_EHR_DATA0_ACCESS "RO"
// =============================================================================
// Register    : TRNG_EHR_DATA1
// Description : RNG collected bits.
//               Bits [63:32] of Entropy Holding Register (EHR) - RNG output
//               register
#define TRNG_EHR_DATA1_OFFSET _u(0x00000118)
#define TRNG_EHR_DATA1_BITS   _u(0xffffffff)
#define TRNG_EHR_DATA1_RESET  _u(0x00000000)
#define TRNG_EHR_DATA1_MSB    _u(31)
#define TRNG_EHR_DATA1_LSB    _u(0)
#define TRNG_EHR_DATA1_ACCESS "RO"
// =============================================================================
// Register    : TRNG_EHR_DATA2
// Description : RNG collected bits.
//               Bits [95:64] of Entropy Holding Register (EHR) - RNG output
//               register
#define TRNG_EHR_DATA2_OFFSET _u(0x0000011c)
#define TRNG_EHR_DATA2_BITS   _u(0xffffffff)
#define TRNG_EHR_DATA2_RESET  _u(0x00000000)
#define TRNG_EHR_DATA2_MSB    _u(31)
#define TRNG_EHR_DATA2_LSB    _u(0)
#define TRNG_EHR_DATA2_ACCESS "RO"
// =============================================================================
// Register    : TRNG_EHR_DATA3
// Description : RNG collected bits.
//               Bits [127:96] of Entropy Holding Register (EHR) - RNG output
//               register
#define TRNG_EHR_DATA3_OFFSET _u(0x00000120)
#define TRNG_EHR_DATA3_BITS   _u(0xffffffff)
#define TRNG_EHR_DATA3_RESET  _u(0x00000000)
#define TRNG_EHR_DATA3_MSB    _u(31)
#define TRNG_EHR_DATA3_LSB    _u(0)
#define TRNG_EHR_DATA3_ACCESS "RO"
// =============================================================================
// Register    : TRNG_EHR_DATA4
// Description : RNG collected bits.
//               Bits [159:128] of Entropy Holding Register (EHR) - RNG output
//               register
#define TRNG_EHR_DATA4_OFFSET _u(0x00000124)
#define TRNG_EHR_DATA4_BITS   _u(0xffffffff)
#define TRNG_EHR_DATA4_RESET  _u(0x00000000)
#define TRNG_EHR_DATA4_MSB    _u(31)
#define TRNG_EHR_DATA4_LSB    _u(0)
#define TRNG_EHR_DATA4_ACCESS "RO"
// =============================================================================
// Register    : TRNG_EHR_DATA5
// Description : RNG collected bits.
//               Bits [191:160] of Entropy Holding Register (EHR) - RNG output
//               register
#define TRNG_EHR_DATA5_OFFSET _u(0x00000128)
#define TRNG_EHR_DATA5_BITS   _u(0xffffffff)
#define TRNG_EHR_DATA5_RESET  _u(0x00000000)
#define TRNG_EHR_DATA5_MSB    _u(31)
#define TRNG_EHR_DATA5_LSB    _u(0)
#define TRNG_EHR_DATA5_ACCESS "RO"
// =============================================================================
// Register    : TRNG_RND_SOURCE_ENABLE
// Description : Enable signal for the random source.
#define TRNG_RND_SOURCE_ENABLE_OFFSET _u(0x0000012c)
#define TRNG_RND_SOURCE_ENABLE_BITS   _u(0xffffffff)
#define TRNG_RND_SOURCE_ENABLE_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : TRNG_RND_SOURCE_ENABLE_RESERVED
// Description : RESERVED
#define TRNG_RND_SOURCE_ENABLE_RESERVED_RESET  _u(0x00000000)
#define TRNG_RND_SOURCE_ENABLE_RESERVED_BITS   _u(0xfffffffe)
#define TRNG_RND_SOURCE_ENABLE_RESERVED_MSB    _u(31)
#define TRNG_RND_SOURCE_ENABLE_RESERVED_LSB    _u(1)
#define TRNG_RND_SOURCE_ENABLE_RESERVED_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : TRNG_RND_SOURCE_ENABLE_RND_SRC_EN
// Description : * 1'b1 - entropy source is enabled. *1'b0 - entropy source is
//               disabled
#define TRNG_RND_SOURCE_ENABLE_RND_SRC_EN_RESET  _u(0x0)
#define TRNG_RND_SOURCE_ENABLE_RND_SRC_EN_BITS   _u(0x00000001)
#define TRNG_RND_SOURCE_ENABLE_RND_SRC_EN_MSB    _u(0)
#define TRNG_RND_SOURCE_ENABLE_RND_SRC_EN_LSB    _u(0)
#define TRNG_RND_SOURCE_ENABLE_RND_SRC_EN_ACCESS "RW"
// =============================================================================
// Register    : TRNG_SAMPLE_CNT1
// Description : Counts clocks between sampling of random bit.
#define TRNG_SAMPLE_CNT1_OFFSET _u(0x00000130)
#define TRNG_SAMPLE_CNT1_BITS   _u(0xffffffff)
#define TRNG_SAMPLE_CNT1_RESET  _u(0x0000ffff)
// -----------------------------------------------------------------------------
// Field       : TRNG_SAMPLE_CNT1_SAMPLE_CNTR1
// Description : Sets the number of rng_clk cycles between two consecutive ring
//               oscillator samples. Note! If the Von-Neuman is bypassed, the
//               minimum value for sample counter must not be less then decimal
//               seventeen
#define TRNG_SAMPLE_CNT1_SAMPLE_CNTR1_RESET  _u(0x0000ffff)
#define TRNG_SAMPLE_CNT1_SAMPLE_CNTR1_BITS   _u(0xffffffff)
#define TRNG_SAMPLE_CNT1_SAMPLE_CNTR1_MSB    _u(31)
#define TRNG_SAMPLE_CNT1_SAMPLE_CNTR1_LSB    _u(0)
#define TRNG_SAMPLE_CNT1_SAMPLE_CNTR1_ACCESS "RW"
// =============================================================================
// Register    : TRNG_AUTOCORR_STATISTIC
// Description : Statistic about Autocorrelation test activations.
#define TRNG_AUTOCORR_STATISTIC_OFFSET _u(0x00000134)
#define TRNG_AUTOCORR_STATISTIC_BITS   _u(0xffffffff)
#define TRNG_AUTOCORR_STATISTIC_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : TRNG_AUTOCORR_STATISTIC_RESERVED
// Description : RESERVED
#define TRNG_AUTOCORR_STATISTIC_RESERVED_RESET  _u(0x000)
#define TRNG_AUTOCORR_STATISTIC_RESERVED_BITS   _u(0xffc00000)
#define TRNG_AUTOCORR_STATISTIC_RESERVED_MSB    _u(31)
#define TRNG_AUTOCORR_STATISTIC_RESERVED_LSB    _u(22)
#define TRNG_AUTOCORR_STATISTIC_RESERVED_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : TRNG_AUTOCORR_STATISTIC_AUTOCORR_FAILS
// Description : Count each time an autocorrelation test fails. Any write to the
//               register reset the counter. Stop collecting statistic if one of
//               the counters reached the limit.
#define TRNG_AUTOCORR_STATISTIC_AUTOCORR_FAILS_RESET  _u(0x00)
#define TRNG_AUTOCORR_STATISTIC_AUTOCORR_FAILS_BITS   _u(0x003fc000)
#define TRNG_AUTOCORR_STATISTIC_AUTOCORR_FAILS_MSB    _u(21)
#define TRNG_AUTOCORR_STATISTIC_AUTOCORR_FAILS_LSB    _u(14)
#define TRNG_AUTOCORR_STATISTIC_AUTOCORR_FAILS_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : TRNG_AUTOCORR_STATISTIC_AUTOCORR_TRYS
// Description : Count each time an autocorrelation test starts. Any write to
//               the register reset the counter. Stop collecting statistic if
//               one of the counters reached the limit.
#define TRNG_AUTOCORR_STATISTIC_AUTOCORR_TRYS_RESET  _u(0x0000)
#define TRNG_AUTOCORR_STATISTIC_AUTOCORR_TRYS_BITS   _u(0x00003fff)
#define TRNG_AUTOCORR_STATISTIC_AUTOCORR_TRYS_MSB    _u(13)
#define TRNG_AUTOCORR_STATISTIC_AUTOCORR_TRYS_LSB    _u(0)
#define TRNG_AUTOCORR_STATISTIC_AUTOCORR_TRYS_ACCESS "RW"
// =============================================================================
// Register    : TRNG_TRNG_DEBUG_CONTROL
// Description : Debug register.
#define TRNG_TRNG_DEBUG_CONTROL_OFFSET _u(0x00000138)
#define TRNG_TRNG_DEBUG_CONTROL_BITS   _u(0x0000000f)
#define TRNG_TRNG_DEBUG_CONTROL_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : TRNG_TRNG_DEBUG_CONTROL_AUTO_CORRELATE_BYPASS
// Description : When set, the autocorrelation test in the TRNG module is
//               bypassed.
#define TRNG_TRNG_DEBUG_CONTROL_AUTO_CORRELATE_BYPASS_RESET  _u(0x0)
#define TRNG_TRNG_DEBUG_CONTROL_AUTO_CORRELATE_BYPASS_BITS   _u(0x00000008)
#define TRNG_TRNG_DEBUG_CONTROL_AUTO_CORRELATE_BYPASS_MSB    _u(3)
#define TRNG_TRNG_DEBUG_CONTROL_AUTO_CORRELATE_BYPASS_LSB    _u(3)
#define TRNG_TRNG_DEBUG_CONTROL_AUTO_CORRELATE_BYPASS_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : TRNG_TRNG_DEBUG_CONTROL_TRNG_CRNGT_BYPASS
// Description : When set, the CRNGT test in the RNG is bypassed.
#define TRNG_TRNG_DEBUG_CONTROL_TRNG_CRNGT_BYPASS_RESET  _u(0x0)
#define TRNG_TRNG_DEBUG_CONTROL_TRNG_CRNGT_BYPASS_BITS   _u(0x00000004)
#define TRNG_TRNG_DEBUG_CONTROL_TRNG_CRNGT_BYPASS_MSB    _u(2)
#define TRNG_TRNG_DEBUG_CONTROL_TRNG_CRNGT_BYPASS_LSB    _u(2)
#define TRNG_TRNG_DEBUG_CONTROL_TRNG_CRNGT_BYPASS_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : TRNG_TRNG_DEBUG_CONTROL_VNC_BYPASS
// Description : When set, the Von-Neuman balancer is bypassed (including the 32
//               consecutive bits test).
#define TRNG_TRNG_DEBUG_CONTROL_VNC_BYPASS_RESET  _u(0x0)
#define TRNG_TRNG_DEBUG_CONTROL_VNC_BYPASS_BITS   _u(0x00000002)
#define TRNG_TRNG_DEBUG_CONTROL_VNC_BYPASS_MSB    _u(1)
#define TRNG_TRNG_DEBUG_CONTROL_VNC_BYPASS_LSB    _u(1)
#define TRNG_TRNG_DEBUG_CONTROL_VNC_BYPASS_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : TRNG_TRNG_DEBUG_CONTROL_RESERVED
// Description : N/A
#define TRNG_TRNG_DEBUG_CONTROL_RESERVED_RESET  _u(0x0)
#define TRNG_TRNG_DEBUG_CONTROL_RESERVED_BITS   _u(0x00000001)
#define TRNG_TRNG_DEBUG_CONTROL_RESERVED_MSB    _u(0)
#define TRNG_TRNG_DEBUG_CONTROL_RESERVED_LSB    _u(0)
#define TRNG_TRNG_DEBUG_CONTROL_RESERVED_ACCESS "RO"
// =============================================================================
// Register    : TRNG_TRNG_SW_RESET
// Description : Generate internal SW reset within the RNG block.
#define TRNG_TRNG_SW_RESET_OFFSET _u(0x00000140)
#define TRNG_TRNG_SW_RESET_BITS   _u(0xffffffff)
#define TRNG_TRNG_SW_RESET_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : TRNG_TRNG_SW_RESET_RESERVED
// Description : RESERVED
#define TRNG_TRNG_SW_RESET_RESERVED_RESET  _u(0x00000000)
#define TRNG_TRNG_SW_RESET_RESERVED_BITS   _u(0xfffffffe)
#define TRNG_TRNG_SW_RESET_RESERVED_MSB    _u(31)
#define TRNG_TRNG_SW_RESET_RESERVED_LSB    _u(1)
#define TRNG_TRNG_SW_RESET_RESERVED_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : TRNG_TRNG_SW_RESET_TRNG_SW_RESET
// Description : Writing 1'b1 to this register causes an internal RNG reset.
#define TRNG_TRNG_SW_RESET_TRNG_SW_RESET_RESET  _u(0x0)
#define TRNG_TRNG_SW_RESET_TRNG_SW_RESET_BITS   _u(0x00000001)
#define TRNG_TRNG_SW_RESET_TRNG_SW_RESET_MSB    _u(0)
#define TRNG_TRNG_SW_RESET_TRNG_SW_RESET_LSB    _u(0)
#define TRNG_TRNG_SW_RESET_TRNG_SW_RESET_ACCESS "RW"
// =============================================================================
// Register    : TRNG_RNG_DEBUG_EN_INPUT
// Description : Enable the RNG debug mode
#define TRNG_RNG_DEBUG_EN_INPUT_OFFSET _u(0x000001b4)
#define TRNG_RNG_DEBUG_EN_INPUT_BITS   _u(0xffffffff)
#define TRNG_RNG_DEBUG_EN_INPUT_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : TRNG_RNG_DEBUG_EN_INPUT_RESERVED
// Description : RESERVED
#define TRNG_RNG_DEBUG_EN_INPUT_RESERVED_RESET  _u(0x00000000)
#define TRNG_RNG_DEBUG_EN_INPUT_RESERVED_BITS   _u(0xfffffffe)
#define TRNG_RNG_DEBUG_EN_INPUT_RESERVED_MSB    _u(31)
#define TRNG_RNG_DEBUG_EN_INPUT_RESERVED_LSB    _u(1)
#define TRNG_RNG_DEBUG_EN_INPUT_RESERVED_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : TRNG_RNG_DEBUG_EN_INPUT_RNG_DEBUG_EN
// Description : * 1'b1 - debug mode is enabled. *1'b0 - debug mode is disabled
#define TRNG_RNG_DEBUG_EN_INPUT_RNG_DEBUG_EN_RESET  _u(0x0)
#define TRNG_RNG_DEBUG_EN_INPUT_RNG_DEBUG_EN_BITS   _u(0x00000001)
#define TRNG_RNG_DEBUG_EN_INPUT_RNG_DEBUG_EN_MSB    _u(0)
#define TRNG_RNG_DEBUG_EN_INPUT_RNG_DEBUG_EN_LSB    _u(0)
#define TRNG_RNG_DEBUG_EN_INPUT_RNG_DEBUG_EN_ACCESS "RW"
// =============================================================================
// Register    : TRNG_TRNG_BUSY
// Description : RNG Busy indication.
#define TRNG_TRNG_BUSY_OFFSET _u(0x000001b8)
#define TRNG_TRNG_BUSY_BITS   _u(0xffffffff)
#define TRNG_TRNG_BUSY_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : TRNG_TRNG_BUSY_RESERVED
// Description : RESERVED
#define TRNG_TRNG_BUSY_RESERVED_RESET  _u(0x00000000)
#define TRNG_TRNG_BUSY_RESERVED_BITS   _u(0xfffffffe)
#define TRNG_TRNG_BUSY_RESERVED_MSB    _u(31)
#define TRNG_TRNG_BUSY_RESERVED_LSB    _u(1)
#define TRNG_TRNG_BUSY_RESERVED_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : TRNG_TRNG_BUSY_TRNG_BUSY
// Description : Reflects rng_busy status.
#define TRNG_TRNG_BUSY_TRNG_BUSY_RESET  _u(0x0)
#define TRNG_TRNG_BUSY_TRNG_BUSY_BITS   _u(0x00000001)
#define TRNG_TRNG_BUSY_TRNG_BUSY_MSB    _u(0)
#define TRNG_TRNG_BUSY_TRNG_BUSY_LSB    _u(0)
#define TRNG_TRNG_BUSY_TRNG_BUSY_ACCESS "RO"
// =============================================================================
// Register    : TRNG_RST_BITS_COUNTER
// Description : Reset the counter of collected bits in the RNG.
#define TRNG_RST_BITS_COUNTER_OFFSET _u(0x000001bc)
#define TRNG_RST_BITS_COUNTER_BITS   _u(0xffffffff)
#define TRNG_RST_BITS_COUNTER_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : TRNG_RST_BITS_COUNTER_RESERVED
// Description : RESERVED
#define TRNG_RST_BITS_COUNTER_RESERVED_RESET  _u(0x00000000)
#define TRNG_RST_BITS_COUNTER_RESERVED_BITS   _u(0xfffffffe)
#define TRNG_RST_BITS_COUNTER_RESERVED_MSB    _u(31)
#define TRNG_RST_BITS_COUNTER_RESERVED_LSB    _u(1)
#define TRNG_RST_BITS_COUNTER_RESERVED_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : TRNG_RST_BITS_COUNTER_RST_BITS_COUNTER
// Description : Writing any value to this address will reset the bits counter
//               and RNG valid registers. RND_SORCE_ENABLE register must be
//               unset in order for the reset to take place.
#define TRNG_RST_BITS_COUNTER_RST_BITS_COUNTER_RESET  _u(0x0)
#define TRNG_RST_BITS_COUNTER_RST_BITS_COUNTER_BITS   _u(0x00000001)
#define TRNG_RST_BITS_COUNTER_RST_BITS_COUNTER_MSB    _u(0)
#define TRNG_RST_BITS_COUNTER_RST_BITS_COUNTER_LSB    _u(0)
#define TRNG_RST_BITS_COUNTER_RST_BITS_COUNTER_ACCESS "RW"
// =============================================================================
// Register    : TRNG_RNG_VERSION
// Description : Displays the version settings of the TRNG.
#define TRNG_RNG_VERSION_OFFSET _u(0x000001c0)
#define TRNG_RNG_VERSION_BITS   _u(0xffffffff)
#define TRNG_RNG_VERSION_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : TRNG_RNG_VERSION_RESERVED
// Description : RESERVED
#define TRNG_RNG_VERSION_RESERVED_RESET  _u(0x000000)
#define TRNG_RNG_VERSION_RESERVED_BITS   _u(0xffffff00)
#define TRNG_RNG_VERSION_RESERVED_MSB    _u(31)
#define TRNG_RNG_VERSION_RESERVED_LSB    _u(8)
#define TRNG_RNG_VERSION_RESERVED_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : TRNG_RNG_VERSION_RNG_USE_5_SBOXES
// Description : * 1'b1 - 5 SBOX AES. *1'b0 - 20 SBOX AES
#define TRNG_RNG_VERSION_RNG_USE_5_SBOXES_RESET  _u(0x0)
#define TRNG_RNG_VERSION_RNG_USE_5_SBOXES_BITS   _u(0x00000080)
#define TRNG_RNG_VERSION_RNG_USE_5_SBOXES_MSB    _u(7)
#define TRNG_RNG_VERSION_RNG_USE_5_SBOXES_LSB    _u(7)
#define TRNG_RNG_VERSION_RNG_USE_5_SBOXES_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : TRNG_RNG_VERSION_RESEEDING_EXISTS
// Description : * 1'b1 - Exists. *1'b0 - Does not exist
#define TRNG_RNG_VERSION_RESEEDING_EXISTS_RESET  _u(0x0)
#define TRNG_RNG_VERSION_RESEEDING_EXISTS_BITS   _u(0x00000040)
#define TRNG_RNG_VERSION_RESEEDING_EXISTS_MSB    _u(6)
#define TRNG_RNG_VERSION_RESEEDING_EXISTS_LSB    _u(6)
#define TRNG_RNG_VERSION_RESEEDING_EXISTS_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : TRNG_RNG_VERSION_KAT_EXISTS
// Description : * 1'b1 - Exists. *1'b0 - Does not exist
#define TRNG_RNG_VERSION_KAT_EXISTS_RESET  _u(0x0)
#define TRNG_RNG_VERSION_KAT_EXISTS_BITS   _u(0x00000020)
#define TRNG_RNG_VERSION_KAT_EXISTS_MSB    _u(5)
#define TRNG_RNG_VERSION_KAT_EXISTS_LSB    _u(5)
#define TRNG_RNG_VERSION_KAT_EXISTS_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : TRNG_RNG_VERSION_PRNG_EXISTS
// Description : * 1'b1 - Exists. *1'b0 - Does not exist
#define TRNG_RNG_VERSION_PRNG_EXISTS_RESET  _u(0x0)
#define TRNG_RNG_VERSION_PRNG_EXISTS_BITS   _u(0x00000010)
#define TRNG_RNG_VERSION_PRNG_EXISTS_MSB    _u(4)
#define TRNG_RNG_VERSION_PRNG_EXISTS_LSB    _u(4)
#define TRNG_RNG_VERSION_PRNG_EXISTS_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : TRNG_RNG_VERSION_TRNG_TESTS_BYPASS_EN
// Description : * 1'b1 - Exists. *1'b0 - Does not exist
#define TRNG_RNG_VERSION_TRNG_TESTS_BYPASS_EN_RESET  _u(0x0)
#define TRNG_RNG_VERSION_TRNG_TESTS_BYPASS_EN_BITS   _u(0x00000008)
#define TRNG_RNG_VERSION_TRNG_TESTS_BYPASS_EN_MSB    _u(3)
#define TRNG_RNG_VERSION_TRNG_TESTS_BYPASS_EN_LSB    _u(3)
#define TRNG_RNG_VERSION_TRNG_TESTS_BYPASS_EN_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : TRNG_RNG_VERSION_AUTOCORR_EXISTS
// Description : * 1'b1 - Exists. *1'b0 - Does not exist
#define TRNG_RNG_VERSION_AUTOCORR_EXISTS_RESET  _u(0x0)
#define TRNG_RNG_VERSION_AUTOCORR_EXISTS_BITS   _u(0x00000004)
#define TRNG_RNG_VERSION_AUTOCORR_EXISTS_MSB    _u(2)
#define TRNG_RNG_VERSION_AUTOCORR_EXISTS_LSB    _u(2)
#define TRNG_RNG_VERSION_AUTOCORR_EXISTS_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : TRNG_RNG_VERSION_CRNGT_EXISTS
// Description : * 1'b1 - Exists. *1'b0 - Does not exist
#define TRNG_RNG_VERSION_CRNGT_EXISTS_RESET  _u(0x0)
#define TRNG_RNG_VERSION_CRNGT_EXISTS_BITS   _u(0x00000002)
#define TRNG_RNG_VERSION_CRNGT_EXISTS_MSB    _u(1)
#define TRNG_RNG_VERSION_CRNGT_EXISTS_LSB    _u(1)
#define TRNG_RNG_VERSION_CRNGT_EXISTS_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : TRNG_RNG_VERSION_EHR_WIDTH_192
// Description : * 1'b1 - 192-bit EHR. *1'b0 - 128-bit EHR
#define TRNG_RNG_VERSION_EHR_WIDTH_192_RESET  _u(0x0)
#define TRNG_RNG_VERSION_EHR_WIDTH_192_BITS   _u(0x00000001)
#define TRNG_RNG_VERSION_EHR_WIDTH_192_MSB    _u(0)
#define TRNG_RNG_VERSION_EHR_WIDTH_192_LSB    _u(0)
#define TRNG_RNG_VERSION_EHR_WIDTH_192_ACCESS "RO"
// =============================================================================
// Register    : TRNG_RNG_BIST_CNTR_0
// Description : Collected BIST results.
#define TRNG_RNG_BIST_CNTR_0_OFFSET _u(0x000001e0)
#define TRNG_RNG_BIST_CNTR_0_BITS   _u(0xffffffff)
#define TRNG_RNG_BIST_CNTR_0_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : TRNG_RNG_BIST_CNTR_0_RESERVED
// Description : RESERVED
#define TRNG_RNG_BIST_CNTR_0_RESERVED_RESET  _u(0x000)
#define TRNG_RNG_BIST_CNTR_0_RESERVED_BITS   _u(0xffc00000)
#define TRNG_RNG_BIST_CNTR_0_RESERVED_MSB    _u(31)
#define TRNG_RNG_BIST_CNTR_0_RESERVED_LSB    _u(22)
#define TRNG_RNG_BIST_CNTR_0_RESERVED_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : TRNG_RNG_BIST_CNTR_0_ROSC_CNTR_VAL
// Description : Reflects the results of RNG BIST counter.
#define TRNG_RNG_BIST_CNTR_0_ROSC_CNTR_VAL_RESET  _u(0x000000)
#define TRNG_RNG_BIST_CNTR_0_ROSC_CNTR_VAL_BITS   _u(0x003fffff)
#define TRNG_RNG_BIST_CNTR_0_ROSC_CNTR_VAL_MSB    _u(21)
#define TRNG_RNG_BIST_CNTR_0_ROSC_CNTR_VAL_LSB    _u(0)
#define TRNG_RNG_BIST_CNTR_0_ROSC_CNTR_VAL_ACCESS "RO"
// =============================================================================
// Register    : TRNG_RNG_BIST_CNTR_1
// Description : Collected BIST results.
#define TRNG_RNG_BIST_CNTR_1_OFFSET _u(0x000001e4)
#define TRNG_RNG_BIST_CNTR_1_BITS   _u(0xffffffff)
#define TRNG_RNG_BIST_CNTR_1_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : TRNG_RNG_BIST_CNTR_1_RESERVED
// Description : RESERVED
#define TRNG_RNG_BIST_CNTR_1_RESERVED_RESET  _u(0x000)
#define TRNG_RNG_BIST_CNTR_1_RESERVED_BITS   _u(0xffc00000)
#define TRNG_RNG_BIST_CNTR_1_RESERVED_MSB    _u(31)
#define TRNG_RNG_BIST_CNTR_1_RESERVED_LSB    _u(22)
#define TRNG_RNG_BIST_CNTR_1_RESERVED_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : TRNG_RNG_BIST_CNTR_1_ROSC_CNTR_VAL
// Description : Reflects the results of RNG BIST counter.
#define TRNG_RNG_BIST_CNTR_1_ROSC_CNTR_VAL_RESET  _u(0x000000)
#define TRNG_RNG_BIST_CNTR_1_ROSC_CNTR_VAL_BITS   _u(0x003fffff)
#define TRNG_RNG_BIST_CNTR_1_ROSC_CNTR_VAL_MSB    _u(21)
#define TRNG_RNG_BIST_CNTR_1_ROSC_CNTR_VAL_LSB    _u(0)
#define TRNG_RNG_BIST_CNTR_1_ROSC_CNTR_VAL_ACCESS "RO"
// =============================================================================
// Register    : TRNG_RNG_BIST_CNTR_2
// Description : Collected BIST results.
#define TRNG_RNG_BIST_CNTR_2_OFFSET _u(0x000001e8)
#define TRNG_RNG_BIST_CNTR_2_BITS   _u(0xffffffff)
#define TRNG_RNG_BIST_CNTR_2_RESET  _u(0x00000000)
// -----------------------------------------------------------------------------
// Field       : TRNG_RNG_BIST_CNTR_2_RESERVED
// Description : RESERVED
#define TRNG_RNG_BIST_CNTR_2_RESERVED_RESET  _u(0x000)
#define TRNG_RNG_BIST_CNTR_2_RESERVED_BITS   _u(0xffc00000)
#define TRNG_RNG_BIST_CNTR_2_RESERVED_MSB    _u(31)
#define TRNG_RNG_BIST_CNTR_2_RESERVED_LSB    _u(22)
#define TRNG_RNG_BIST_CNTR_2_RESERVED_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : TRNG_RNG_BIST_CNTR_2_ROSC_CNTR_VAL
// Description : Reflects the results of RNG BIST counter.
#define TRNG_RNG_BIST_CNTR_2_ROSC_CNTR_VAL_RESET  _u(0x000000)
#define TRNG_RNG_BIST_CNTR_2_ROSC_CNTR_VAL_BITS   _u(0x003fffff)
#define TRNG_RNG_BIST_CNTR_2_ROSC_CNTR_VAL_MSB    _u(21)
#define TRNG_RNG_BIST_CNTR_2_ROSC_CNTR_VAL_LSB    _u(0)
#define TRNG_RNG_BIST_CNTR_2_ROSC_CNTR_VAL_ACCESS "RO"
// =============================================================================
#endif // _HARDWARE_REGS_TRNG_H

