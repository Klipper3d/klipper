/**
 * \file
 *
 * \brief Component description for TC
 *
 * Copyright (c) 2014-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#ifndef _SAM4L_TC_COMPONENT_
#define _SAM4L_TC_COMPONENT_

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR TC */
/* ========================================================================== */
/** \addtogroup SAM4L_TC Timer/Counter */
/*@{*/

#define REV_TC                      0x402

/* -------- TC_CCR : (TC Offset: 0x00) ( /W 32) channel Channel Control Register Channel -------- */
#define TC_CCR_OFFSET               0x00         /**< \brief (TC_CCR offset) Channel Control Register Channel */
#define TC_CCR_RESETVALUE           0x00000000   /**< \brief (TC_CCR reset_value) Channel Control Register Channel */

#define TC_CCR_CLKEN_Pos            0            /**< \brief (TC_CCR) Counter Clock Enable Command */
#define TC_CCR_CLKEN                (0x1u << TC_CCR_CLKEN_Pos)
#define   TC_CCR_CLKEN_0            (0x0u <<  0) /**< \brief (TC_CCR) No effect. */
#define   TC_CCR_CLKEN_1            (0x1u <<  0) /**< \brief (TC_CCR) Enables the clock if CLKDIS is not 1. */
#define TC_CCR_CLKDIS_Pos           1            /**< \brief (TC_CCR) Counter Clock Disable Command */
#define TC_CCR_CLKDIS               (0x1u << TC_CCR_CLKDIS_Pos)
#define   TC_CCR_CLKDIS_0           (0x0u <<  1) /**< \brief (TC_CCR) No effect. */
#define   TC_CCR_CLKDIS_1           (0x1u <<  1) /**< \brief (TC_CCR) Disables the clock. */
#define TC_CCR_SWTRG_Pos            2            /**< \brief (TC_CCR) Software Trigger Command */
#define TC_CCR_SWTRG                (0x1u << TC_CCR_SWTRG_Pos)
#define   TC_CCR_SWTRG_0            (0x0u <<  2) /**< \brief (TC_CCR) No effect. */
#define   TC_CCR_SWTRG_1            (0x1u <<  2) /**< \brief (TC_CCR) A software trigger is performed:the counter is reset and clock is started. */
#define TC_CCR_MASK                 0x00000007u  /**< \brief (TC_CCR) MASK Register */

/* -------- TC_CMR : (TC Offset: 0x04) (R/W 32) channel Channel Mode Register Channel -------- */
#define TC_CMR_OFFSET               0x04         /**< \brief (TC_CMR offset) Channel Mode Register Channel */
#define TC_CMR_RESETVALUE           0x00000000   /**< \brief (TC_CMR reset_value) Channel Mode Register Channel */

// CAPTURE mode
#define TC_CMR_CAPTURE_TCCLKS_Pos   0            /**< \brief (TC_CMR_CAPTURE) Clock Selection */
#define TC_CMR_CAPTURE_TCCLKS_Msk   (0x7u << TC_CMR_CAPTURE_TCCLKS_Pos)
#define TC_CMR_CAPTURE_TCCLKS(value) ((TC_CMR_CAPTURE_TCCLKS_Msk & ((value) << TC_CMR_CAPTURE_TCCLKS_Pos)))
#define   TC_CMR_CAPTURE_TCCLKS_TIMER_CLOCK1 (0x0u <<  0) /**< \brief (TC_CMR_CAPTURE) TIMER_CLOCK1 */
#define   TC_CMR_CAPTURE_TCCLKS_TIMER_CLOCK2 (0x1u <<  0) /**< \brief (TC_CMR_CAPTURE) TIMER_CLOCK2 */
#define   TC_CMR_CAPTURE_TCCLKS_TIMER_CLOCK3 (0x2u <<  0) /**< \brief (TC_CMR_CAPTURE) TIMER_CLOCK3 */
#define   TC_CMR_CAPTURE_TCCLKS_TIMER_CLOCK4 (0x3u <<  0) /**< \brief (TC_CMR_CAPTURE) TIMER_CLOCK4 */
#define   TC_CMR_CAPTURE_TCCLKS_TIMER_CLOCK5 (0x4u <<  0) /**< \brief (TC_CMR_CAPTURE) TIMER_CLOCK5 */
#define   TC_CMR_CAPTURE_TCCLKS_XC0 (0x5u <<  0) /**< \brief (TC_CMR_CAPTURE) XC0 */
#define   TC_CMR_CAPTURE_TCCLKS_XC1 (0x6u <<  0) /**< \brief (TC_CMR_CAPTURE) XC1 */
#define   TC_CMR_CAPTURE_TCCLKS_XC2 (0x7u <<  0) /**< \brief (TC_CMR_CAPTURE) XC2 */
#define TC_CMR_CAPTURE_CLKI_Pos     3            /**< \brief (TC_CMR_CAPTURE) Clock Invert */
#define TC_CMR_CAPTURE_CLKI         (0x1u << TC_CMR_CAPTURE_CLKI_Pos)
#define   TC_CMR_CAPTURE_CLKI_0     (0x0u <<  3) /**< \brief (TC_CMR_CAPTURE) Counter is incremented on rising edge of the clock. */
#define   TC_CMR_CAPTURE_CLKI_1     (0x1u <<  3) /**< \brief (TC_CMR_CAPTURE) Counter is incremented on falling edge of the clock. */
#define TC_CMR_CAPTURE_BURST_Pos    4            /**< \brief (TC_CMR_CAPTURE) Burst Signal Selection */
#define TC_CMR_CAPTURE_BURST_Msk    (0x3u << TC_CMR_CAPTURE_BURST_Pos)
#define TC_CMR_CAPTURE_BURST(value) ((TC_CMR_CAPTURE_BURST_Msk & ((value) << TC_CMR_CAPTURE_BURST_Pos)))
#define   TC_CMR_CAPTURE_BURST_NOT_GATED (0x0u <<  4) /**< \brief (TC_CMR_CAPTURE) The clock is not gated by an external signal. */
#define   TC_CMR_CAPTURE_BURST_CLK_AND_XC0 (0x1u <<  4) /**< \brief (TC_CMR_CAPTURE) XC0 is ANDed with the selected clock. */
#define   TC_CMR_CAPTURE_BURST_CLK_AND_XC1 (0x2u <<  4) /**< \brief (TC_CMR_CAPTURE) XC1 is ANDed with the selected clock. */
#define   TC_CMR_CAPTURE_BURST_CLK_AND_XC2 (0x3u <<  4) /**< \brief (TC_CMR_CAPTURE) XC2 is ANDed with the selected clock. */
#define TC_CMR_CAPTURE_LDBSTOP_Pos  6            /**< \brief (TC_CMR_CAPTURE) Counter Clock Stopped with RB Loading */
#define TC_CMR_CAPTURE_LDBSTOP      (0x1u << TC_CMR_CAPTURE_LDBSTOP_Pos)
#define   TC_CMR_CAPTURE_LDBSTOP_0  (0x0u <<  6) /**< \brief (TC_CMR_CAPTURE) Counter clock is not stopped when RB loading occurs. */
#define   TC_CMR_CAPTURE_LDBSTOP_1  (0x1u <<  6) /**< \brief (TC_CMR_CAPTURE) Counter clock is stopped when RB loading occurs. */
#define TC_CMR_CAPTURE_LDBDIS_Pos   7            /**< \brief (TC_CMR_CAPTURE) Counter Clock Disable with RB Loading */
#define TC_CMR_CAPTURE_LDBDIS       (0x1u << TC_CMR_CAPTURE_LDBDIS_Pos)
#define   TC_CMR_CAPTURE_LDBDIS_0   (0x0u <<  7) /**< \brief (TC_CMR_CAPTURE) Counter clock is not disabled when RB loading occurs. */
#define   TC_CMR_CAPTURE_LDBDIS_1   (0x1u <<  7) /**< \brief (TC_CMR_CAPTURE) Counter clock is disabled when RB loading occurs. */
#define TC_CMR_CAPTURE_ETRGEDG_Pos  8            /**< \brief (TC_CMR_CAPTURE) External Trigger Edge Selection */
#define TC_CMR_CAPTURE_ETRGEDG_Msk  (0x3u << TC_CMR_CAPTURE_ETRGEDG_Pos)
#define TC_CMR_CAPTURE_ETRGEDG(value) ((TC_CMR_CAPTURE_ETRGEDG_Msk & ((value) << TC_CMR_CAPTURE_ETRGEDG_Pos)))
#define   TC_CMR_CAPTURE_ETRGEDG_NO_EDGE (0x0u <<  8) /**< \brief (TC_CMR_CAPTURE) none */
#define   TC_CMR_CAPTURE_ETRGEDG_POS_EDGE (0x1u <<  8) /**< \brief (TC_CMR_CAPTURE) rising edge */
#define   TC_CMR_CAPTURE_ETRGEDG_NEG_EDGE (0x2u <<  8) /**< \brief (TC_CMR_CAPTURE) falling edge */
#define   TC_CMR_CAPTURE_ETRGEDG_BOTH_EDGES (0x3u <<  8) /**< \brief (TC_CMR_CAPTURE) each edge */
#define TC_CMR_CAPTURE_ABETRG_Pos   10           /**< \brief (TC_CMR_CAPTURE) TIOA or TIOB External Trigger Selection */
#define TC_CMR_CAPTURE_ABETRG       (0x1u << TC_CMR_CAPTURE_ABETRG_Pos)
#define   TC_CMR_CAPTURE_ABETRG_0   (0x0u << 10) /**< \brief (TC_CMR_CAPTURE) TIOB is used as an external trigger. */
#define   TC_CMR_CAPTURE_ABETRG_1   (0x1u << 10) /**< \brief (TC_CMR_CAPTURE) TIOA is used as an external trigger. */
#define TC_CMR_CAPTURE_CPCTRG_Pos   14           /**< \brief (TC_CMR_CAPTURE) RC Compare Trigger Enable */
#define TC_CMR_CAPTURE_CPCTRG       (0x1u << TC_CMR_CAPTURE_CPCTRG_Pos)
#define   TC_CMR_CAPTURE_CPCTRG_0   (0x0u << 14) /**< \brief (TC_CMR_CAPTURE) RC Compare has no effect on the counter and its clock. */
#define   TC_CMR_CAPTURE_CPCTRG_1   (0x1u << 14) /**< \brief (TC_CMR_CAPTURE) RC Compare resets the counter and starts the counter clock. */
#define TC_CMR_CAPTURE_WAVE_Pos     15           /**< \brief (TC_CMR_CAPTURE) Wave */
#define TC_CMR_CAPTURE_WAVE         (0x1u << TC_CMR_CAPTURE_WAVE_Pos)
#define   TC_CMR_CAPTURE_WAVE_0     (0x0u << 15) /**< \brief (TC_CMR_CAPTURE) Capture Mode is enabled. */
#define   TC_CMR_CAPTURE_WAVE_1     (0x1u << 15) /**< \brief (TC_CMR_CAPTURE) Capture Mode is disabled (Waveform Mode is enabled). */
#define TC_CMR_CAPTURE_LDRA_Pos     16           /**< \brief (TC_CMR_CAPTURE) RA Loading Selection */
#define TC_CMR_CAPTURE_LDRA_Msk     (0x3u << TC_CMR_CAPTURE_LDRA_Pos)
#define TC_CMR_CAPTURE_LDRA(value)  ((TC_CMR_CAPTURE_LDRA_Msk & ((value) << TC_CMR_CAPTURE_LDRA_Pos)))
#define   TC_CMR_CAPTURE_LDRA_NO_EDGE (0x0u << 16) /**< \brief (TC_CMR_CAPTURE) none */
#define   TC_CMR_CAPTURE_LDRA_POS_EDGE_TIOA (0x1u << 16) /**< \brief (TC_CMR_CAPTURE) rising edge of TIOA */
#define   TC_CMR_CAPTURE_LDRA_NEG_EDGE_TIOA (0x2u << 16) /**< \brief (TC_CMR_CAPTURE) falling edge of TIOA */
#define   TC_CMR_CAPTURE_LDRA_BOTH_EDGES_TIOA (0x3u << 16) /**< \brief (TC_CMR_CAPTURE) each edge of TIOA */
#define TC_CMR_CAPTURE_LDRB_Pos     18           /**< \brief (TC_CMR_CAPTURE) RB Loading Selection */
#define TC_CMR_CAPTURE_LDRB_Msk     (0x3u << TC_CMR_CAPTURE_LDRB_Pos)
#define TC_CMR_CAPTURE_LDRB(value)  ((TC_CMR_CAPTURE_LDRB_Msk & ((value) << TC_CMR_CAPTURE_LDRB_Pos)))
#define   TC_CMR_CAPTURE_LDRB_NO_EDGE (0x0u << 18) /**< \brief (TC_CMR_CAPTURE) none */
#define   TC_CMR_CAPTURE_LDRB_POS_EDGE_TIOA (0x1u << 18) /**< \brief (TC_CMR_CAPTURE) rising edge of TIOA */
#define   TC_CMR_CAPTURE_LDRB_NEG_EDGE_TIOA (0x2u << 18) /**< \brief (TC_CMR_CAPTURE) falling edge of TIOA */
#define   TC_CMR_CAPTURE_LDRB_BOTH_EDGES_TIOA (0x3u << 18) /**< \brief (TC_CMR_CAPTURE) each edge of TIOA */
#define TC_CMR_CAPTURE_MASK         0x000FC7FFu  /**< \brief (TC_CMR_CAPTURE) MASK Register */

// WAVEFORM mode
#define TC_CMR_WAVEFORM_TCCLKS_Pos  0            /**< \brief (TC_CMR_WAVEFORM) Clock Selection */
#define TC_CMR_WAVEFORM_TCCLKS_Msk  (0x7u << TC_CMR_WAVEFORM_TCCLKS_Pos)
#define TC_CMR_WAVEFORM_TCCLKS(value) ((TC_CMR_WAVEFORM_TCCLKS_Msk & ((value) << TC_CMR_WAVEFORM_TCCLKS_Pos)))
#define   TC_CMR_WAVEFORM_TCCLKS_TIMER_DIV1_CLOCK (0x0u <<  0) /**< \brief (TC_CMR_WAVEFORM) TIMER_DIV1_CLOCK */
#define   TC_CMR_WAVEFORM_TCCLKS_TIMER_DIV2_CLOCK (0x1u <<  0) /**< \brief (TC_CMR_WAVEFORM) TIMER_DIV2_CLOCK */
#define   TC_CMR_WAVEFORM_TCCLKS_TIMER_DIV3_CLOCK (0x2u <<  0) /**< \brief (TC_CMR_WAVEFORM) TIMER_DIV3_CLOCK */
#define   TC_CMR_WAVEFORM_TCCLKS_TIMER_DIV4_CLOCK (0x3u <<  0) /**< \brief (TC_CMR_WAVEFORM) TIMER_DIV4_CLOCK */
#define   TC_CMR_WAVEFORM_TCCLKS_TIMER_DIV5_CLOCK (0x4u <<  0) /**< \brief (TC_CMR_WAVEFORM) TIMER_DIV5_CLOCK */
#define   TC_CMR_WAVEFORM_TCCLKS_XC0 (0x5u <<  0) /**< \brief (TC_CMR_WAVEFORM) XC0 */
#define   TC_CMR_WAVEFORM_TCCLKS_XC1 (0x6u <<  0) /**< \brief (TC_CMR_WAVEFORM) XC1 */
#define   TC_CMR_WAVEFORM_TCCLKS_XC2 (0x7u <<  0) /**< \brief (TC_CMR_WAVEFORM) XC2 */
#define TC_CMR_WAVEFORM_CLKI_Pos    3            /**< \brief (TC_CMR_WAVEFORM) Clock Invert */
#define TC_CMR_WAVEFORM_CLKI        (0x1u << TC_CMR_WAVEFORM_CLKI_Pos)
#define   TC_CMR_WAVEFORM_CLKI_0    (0x0u <<  3) /**< \brief (TC_CMR_WAVEFORM) Counter is incremented on rising edge of the clock. */
#define   TC_CMR_WAVEFORM_CLKI_1    (0x1u <<  3) /**< \brief (TC_CMR_WAVEFORM) Counter is incremented on falling edge of the clock. */
#define TC_CMR_WAVEFORM_BURST_Pos   4            /**< \brief (TC_CMR_WAVEFORM) Burst Signal Selection */
#define TC_CMR_WAVEFORM_BURST_Msk   (0x3u << TC_CMR_WAVEFORM_BURST_Pos)
#define TC_CMR_WAVEFORM_BURST(value) ((TC_CMR_WAVEFORM_BURST_Msk & ((value) << TC_CMR_WAVEFORM_BURST_Pos)))
#define   TC_CMR_WAVEFORM_BURST_NOT_GATED (0x0u <<  4) /**< \brief (TC_CMR_WAVEFORM) The clock is not gated by an external signal. */
#define   TC_CMR_WAVEFORM_BURST_CLK_AND_XC0 (0x1u <<  4) /**< \brief (TC_CMR_WAVEFORM) XC0 is ANDed with the selected clock. */
#define   TC_CMR_WAVEFORM_BURST_CLK_AND_XC1 (0x2u <<  4) /**< \brief (TC_CMR_WAVEFORM) XC1 is ANDed with the selected clock. */
#define   TC_CMR_WAVEFORM_BURST_CLK_AND_XC2 (0x3u <<  4) /**< \brief (TC_CMR_WAVEFORM) XC2 is ANDed with the selected clock. */
#define TC_CMR_WAVEFORM_CPCSTOP_Pos 6            /**< \brief (TC_CMR_WAVEFORM) Counter Clock Stopped with RC Compare */
#define TC_CMR_WAVEFORM_CPCSTOP     (0x1u << TC_CMR_WAVEFORM_CPCSTOP_Pos)
#define   TC_CMR_WAVEFORM_CPCSTOP_0 (0x0u <<  6) /**< \brief (TC_CMR_WAVEFORM) Counter clock is not stopped when counter reaches RC. */
#define   TC_CMR_WAVEFORM_CPCSTOP_1 (0x1u <<  6) /**< \brief (TC_CMR_WAVEFORM) Counter clock is stopped when counter reaches RC. */
#define TC_CMR_WAVEFORM_CPCDIS_Pos  7            /**< \brief (TC_CMR_WAVEFORM) Counter Clock Disable with RC Compare */
#define TC_CMR_WAVEFORM_CPCDIS      (0x1u << TC_CMR_WAVEFORM_CPCDIS_Pos)
#define   TC_CMR_WAVEFORM_CPCDIS_0  (0x0u <<  7) /**< \brief (TC_CMR_WAVEFORM) Counter clock is not disabled when counter reaches RC. */
#define   TC_CMR_WAVEFORM_CPCDIS_1  (0x1u <<  7) /**< \brief (TC_CMR_WAVEFORM) Counter clock is disabled when counter reaches RC. */
#define TC_CMR_WAVEFORM_EEVTEDG_Pos 8            /**< \brief (TC_CMR_WAVEFORM) External Event Edge Selection */
#define TC_CMR_WAVEFORM_EEVTEDG_Msk (0x3u << TC_CMR_WAVEFORM_EEVTEDG_Pos)
#define TC_CMR_WAVEFORM_EEVTEDG(value) ((TC_CMR_WAVEFORM_EEVTEDG_Msk & ((value) << TC_CMR_WAVEFORM_EEVTEDG_Pos)))
#define   TC_CMR_WAVEFORM_EEVTEDG_NO_EDGE (0x0u <<  8) /**< \brief (TC_CMR_WAVEFORM) none */
#define   TC_CMR_WAVEFORM_EEVTEDG_POS_EDGE (0x1u <<  8) /**< \brief (TC_CMR_WAVEFORM) rising edge */
#define   TC_CMR_WAVEFORM_EEVTEDG_NEG_EDGE (0x2u <<  8) /**< \brief (TC_CMR_WAVEFORM) falling edge */
#define   TC_CMR_WAVEFORM_EEVTEDG_BOTH_EDGES (0x3u <<  8) /**< \brief (TC_CMR_WAVEFORM) each edge */
#define TC_CMR_WAVEFORM_EEVT_Pos    10           /**< \brief (TC_CMR_WAVEFORM) External Event Selection */
#define TC_CMR_WAVEFORM_EEVT_Msk    (0x3u << TC_CMR_WAVEFORM_EEVT_Pos)
#define TC_CMR_WAVEFORM_EEVT(value) ((TC_CMR_WAVEFORM_EEVT_Msk & ((value) << TC_CMR_WAVEFORM_EEVT_Pos)))
#define   TC_CMR_WAVEFORM_EEVT_TIOB_INPUT (0x0u << 10) /**< \brief (TC_CMR_WAVEFORM) TIOB input. If TIOB is chosen as the external event signal, it is configured as an input and no longer generates waveforms. */
#define   TC_CMR_WAVEFORM_EEVT_XC0_OUTPUT (0x1u << 10) /**< \brief (TC_CMR_WAVEFORM) XC0 output */
#define   TC_CMR_WAVEFORM_EEVT_XC1_OUTPUT (0x2u << 10) /**< \brief (TC_CMR_WAVEFORM) XC1 output */
#define   TC_CMR_WAVEFORM_EEVT_XC2_OUTPUT (0x3u << 10) /**< \brief (TC_CMR_WAVEFORM) XC2 output */
#define TC_CMR_WAVEFORM_ENETRG_Pos  12           /**< \brief (TC_CMR_WAVEFORM) External Event Trigger Enable */
#define TC_CMR_WAVEFORM_ENETRG      (0x1u << TC_CMR_WAVEFORM_ENETRG_Pos)
#define   TC_CMR_WAVEFORM_ENETRG_0  (0x0u << 12) /**< \brief (TC_CMR_WAVEFORM) The external event has no effect on the counter and its clock. In this case, the selected external event only controls the TIOA output. */
#define   TC_CMR_WAVEFORM_ENETRG_1  (0x1u << 12) /**< \brief (TC_CMR_WAVEFORM) The external event resets the counter and starts the counter clock. */
#define TC_CMR_WAVEFORM_WAVSEL_Pos  13           /**< \brief (TC_CMR_WAVEFORM) Waveform Selection */
#define TC_CMR_WAVEFORM_WAVSEL_Msk  (0x3u << TC_CMR_WAVEFORM_WAVSEL_Pos)
#define TC_CMR_WAVEFORM_WAVSEL(value) ((TC_CMR_WAVEFORM_WAVSEL_Msk & ((value) << TC_CMR_WAVEFORM_WAVSEL_Pos)))
#define   TC_CMR_WAVEFORM_WAVSEL_UP_NO_AUTO (0x0u << 13) /**< \brief (TC_CMR_WAVEFORM) UP mode without automatic trigger on RC Compare */
#define   TC_CMR_WAVEFORM_WAVSEL_UPDOWN_NO_AUTO (0x1u << 13) /**< \brief (TC_CMR_WAVEFORM) UPDOWN mode without automatic trigger on RC Compare */
#define   TC_CMR_WAVEFORM_WAVSEL_UP_AUTO (0x2u << 13) /**< \brief (TC_CMR_WAVEFORM) UP mode with automatic trigger on RC Compare */
#define   TC_CMR_WAVEFORM_WAVSEL_UPDOWN_AUTO (0x3u << 13) /**< \brief (TC_CMR_WAVEFORM) UPDOWN mode with automatic trigger on RC Compare */
#define TC_CMR_WAVEFORM_WAVE_Pos    15           /**< \brief (TC_CMR_WAVEFORM) WAVE */
#define TC_CMR_WAVEFORM_WAVE        (0x1u << TC_CMR_WAVEFORM_WAVE_Pos)
#define   TC_CMR_WAVEFORM_WAVE_0    (0x0u << 15) /**< \brief (TC_CMR_WAVEFORM) Waveform Mode is disabled (Capture Mode is enabled). */
#define   TC_CMR_WAVEFORM_WAVE_1    (0x1u << 15) /**< \brief (TC_CMR_WAVEFORM) Waveform Mode is enabled. */
#define TC_CMR_WAVEFORM_ACPA_Pos    16           /**< \brief (TC_CMR_WAVEFORM) RA Compare Effect on TIOA */
#define TC_CMR_WAVEFORM_ACPA_Msk    (0x3u << TC_CMR_WAVEFORM_ACPA_Pos)
#define TC_CMR_WAVEFORM_ACPA(value) ((TC_CMR_WAVEFORM_ACPA_Msk & ((value) << TC_CMR_WAVEFORM_ACPA_Pos)))
#define   TC_CMR_WAVEFORM_ACPA_NONE (0x0u << 16) /**< \brief (TC_CMR_WAVEFORM) none */
#define   TC_CMR_WAVEFORM_ACPA_SET  (0x1u << 16) /**< \brief (TC_CMR_WAVEFORM) set */
#define   TC_CMR_WAVEFORM_ACPA_CLEAR (0x2u << 16) /**< \brief (TC_CMR_WAVEFORM) clear */
#define   TC_CMR_WAVEFORM_ACPA_TOGGLE (0x3u << 16) /**< \brief (TC_CMR_WAVEFORM) toggle */
#define TC_CMR_WAVEFORM_ACPC_Pos    18           /**< \brief (TC_CMR_WAVEFORM) RC Compare Effect on TIOA */
#define TC_CMR_WAVEFORM_ACPC_Msk    (0x3u << TC_CMR_WAVEFORM_ACPC_Pos)
#define TC_CMR_WAVEFORM_ACPC(value) ((TC_CMR_WAVEFORM_ACPC_Msk & ((value) << TC_CMR_WAVEFORM_ACPC_Pos)))
#define   TC_CMR_WAVEFORM_ACPC_NONE (0x0u << 18) /**< \brief (TC_CMR_WAVEFORM) none */
#define   TC_CMR_WAVEFORM_ACPC_SET  (0x1u << 18) /**< \brief (TC_CMR_WAVEFORM) set */
#define   TC_CMR_WAVEFORM_ACPC_CLEAR (0x2u << 18) /**< \brief (TC_CMR_WAVEFORM) clear */
#define   TC_CMR_WAVEFORM_ACPC_TOGGLE (0x3u << 18) /**< \brief (TC_CMR_WAVEFORM) toggle */
#define TC_CMR_WAVEFORM_AEEVT_Pos   20           /**< \brief (TC_CMR_WAVEFORM) External Event Effect on TIOA */
#define TC_CMR_WAVEFORM_AEEVT_Msk   (0x3u << TC_CMR_WAVEFORM_AEEVT_Pos)
#define TC_CMR_WAVEFORM_AEEVT(value) ((TC_CMR_WAVEFORM_AEEVT_Msk & ((value) << TC_CMR_WAVEFORM_AEEVT_Pos)))
#define   TC_CMR_WAVEFORM_AEEVT_NONE (0x0u << 20) /**< \brief (TC_CMR_WAVEFORM) none */
#define   TC_CMR_WAVEFORM_AEEVT_SET (0x1u << 20) /**< \brief (TC_CMR_WAVEFORM) set */
#define   TC_CMR_WAVEFORM_AEEVT_CLEAR (0x2u << 20) /**< \brief (TC_CMR_WAVEFORM) clear */
#define   TC_CMR_WAVEFORM_AEEVT_TOGGLE (0x3u << 20) /**< \brief (TC_CMR_WAVEFORM) toggle */
#define TC_CMR_WAVEFORM_ASWTRG_Pos  22           /**< \brief (TC_CMR_WAVEFORM) Software Trigger Effect on TIOA */
#define TC_CMR_WAVEFORM_ASWTRG_Msk  (0x3u << TC_CMR_WAVEFORM_ASWTRG_Pos)
#define TC_CMR_WAVEFORM_ASWTRG(value) ((TC_CMR_WAVEFORM_ASWTRG_Msk & ((value) << TC_CMR_WAVEFORM_ASWTRG_Pos)))
#define   TC_CMR_WAVEFORM_ASWTRG_NONE (0x0u << 22) /**< \brief (TC_CMR_WAVEFORM) none */
#define   TC_CMR_WAVEFORM_ASWTRG_SET (0x1u << 22) /**< \brief (TC_CMR_WAVEFORM) set */
#define   TC_CMR_WAVEFORM_ASWTRG_CLEAR (0x2u << 22) /**< \brief (TC_CMR_WAVEFORM) clear */
#define   TC_CMR_WAVEFORM_ASWTRG_TOGGLE (0x3u << 22) /**< \brief (TC_CMR_WAVEFORM) toggle */
#define TC_CMR_WAVEFORM_BCPB_Pos    24           /**< \brief (TC_CMR_WAVEFORM) RB Compare Effect on TIOB */
#define TC_CMR_WAVEFORM_BCPB_Msk    (0x3u << TC_CMR_WAVEFORM_BCPB_Pos)
#define TC_CMR_WAVEFORM_BCPB(value) ((TC_CMR_WAVEFORM_BCPB_Msk & ((value) << TC_CMR_WAVEFORM_BCPB_Pos)))
#define   TC_CMR_WAVEFORM_BCPB_NONE (0x0u << 24) /**< \brief (TC_CMR_WAVEFORM) none */
#define   TC_CMR_WAVEFORM_BCPB_SET  (0x1u << 24) /**< \brief (TC_CMR_WAVEFORM) set */
#define   TC_CMR_WAVEFORM_BCPB_CLEAR (0x2u << 24) /**< \brief (TC_CMR_WAVEFORM) clear */
#define   TC_CMR_WAVEFORM_BCPB_TOGGLE (0x3u << 24) /**< \brief (TC_CMR_WAVEFORM) toggle */
#define TC_CMR_WAVEFORM_BCPC_Pos    26           /**< \brief (TC_CMR_WAVEFORM) RC Compare Effect on TIOB */
#define TC_CMR_WAVEFORM_BCPC_Msk    (0x3u << TC_CMR_WAVEFORM_BCPC_Pos)
#define TC_CMR_WAVEFORM_BCPC(value) ((TC_CMR_WAVEFORM_BCPC_Msk & ((value) << TC_CMR_WAVEFORM_BCPC_Pos)))
#define   TC_CMR_WAVEFORM_BCPC_NONE (0x0u << 26) /**< \brief (TC_CMR_WAVEFORM) none */
#define   TC_CMR_WAVEFORM_BCPC_SET  (0x1u << 26) /**< \brief (TC_CMR_WAVEFORM) set */
#define   TC_CMR_WAVEFORM_BCPC_CLEAR (0x2u << 26) /**< \brief (TC_CMR_WAVEFORM) clear */
#define   TC_CMR_WAVEFORM_BCPC_TOGGLE (0x3u << 26) /**< \brief (TC_CMR_WAVEFORM) toggle */
#define TC_CMR_WAVEFORM_BEEVT_Pos   28           /**< \brief (TC_CMR_WAVEFORM) External Event Effect on TIOB */
#define TC_CMR_WAVEFORM_BEEVT_Msk   (0x3u << TC_CMR_WAVEFORM_BEEVT_Pos)
#define TC_CMR_WAVEFORM_BEEVT(value) ((TC_CMR_WAVEFORM_BEEVT_Msk & ((value) << TC_CMR_WAVEFORM_BEEVT_Pos)))
#define   TC_CMR_WAVEFORM_BEEVT_NONE (0x0u << 28) /**< \brief (TC_CMR_WAVEFORM) none */
#define   TC_CMR_WAVEFORM_BEEVT_SET (0x1u << 28) /**< \brief (TC_CMR_WAVEFORM) set */
#define   TC_CMR_WAVEFORM_BEEVT_CLEAR (0x2u << 28) /**< \brief (TC_CMR_WAVEFORM) clear */
#define   TC_CMR_WAVEFORM_BEEVT_TOGGLE (0x3u << 28) /**< \brief (TC_CMR_WAVEFORM) toggle */
#define TC_CMR_WAVEFORM_BSWTRG_Pos  30           /**< \brief (TC_CMR_WAVEFORM) Software Trigger Effect on TIOB */
#define TC_CMR_WAVEFORM_BSWTRG_Msk  (0x3u << TC_CMR_WAVEFORM_BSWTRG_Pos)
#define TC_CMR_WAVEFORM_BSWTRG(value) ((TC_CMR_WAVEFORM_BSWTRG_Msk & ((value) << TC_CMR_WAVEFORM_BSWTRG_Pos)))
#define   TC_CMR_WAVEFORM_BSWTRG_NONE (0x0u << 30) /**< \brief (TC_CMR_WAVEFORM) none */
#define   TC_CMR_WAVEFORM_BSWTRG_SET (0x1u << 30) /**< \brief (TC_CMR_WAVEFORM) set */
#define   TC_CMR_WAVEFORM_BSWTRG_CLEAR (0x2u << 30) /**< \brief (TC_CMR_WAVEFORM) clear */
#define   TC_CMR_WAVEFORM_BSWTRG_TOGGLE (0x3u << 30) /**< \brief (TC_CMR_WAVEFORM) toggle */
#define TC_CMR_WAVEFORM_MASK        0xFFFFFFFFu  /**< \brief (TC_CMR_WAVEFORM) MASK Register */

// Any mode
#define TC_CMR_TCCLKS_Pos           0            /**< \brief (TC_CMR) Clock Selection */
#define TC_CMR_TCCLKS_Msk           (0x7u << TC_CMR_TCCLKS_Pos)
#define TC_CMR_TCCLKS(value)        ((TC_CMR_TCCLKS_Msk & ((value) << TC_CMR_TCCLKS_Pos)))
#define   TC_CMR_TCCLKS_TIMER_CLOCK1 (0x0u <<  0) /**< \brief (TC_CMR) TIMER_CLOCK1 */
#define   TC_CMR_TCCLKS_TIMER_CLOCK2 (0x1u <<  0) /**< \brief (TC_CMR) TIMER_CLOCK2 */
#define   TC_CMR_TCCLKS_TIMER_CLOCK3 (0x2u <<  0) /**< \brief (TC_CMR) TIMER_CLOCK3 */
#define   TC_CMR_TCCLKS_TIMER_CLOCK4 (0x3u <<  0) /**< \brief (TC_CMR) TIMER_CLOCK4 */
#define   TC_CMR_TCCLKS_TIMER_CLOCK5 (0x4u <<  0) /**< \brief (TC_CMR) TIMER_CLOCK5 */
#define   TC_CMR_TCCLKS_XC0         (0x5u <<  0) /**< \brief (TC_CMR) XC0 */
#define   TC_CMR_TCCLKS_XC1         (0x6u <<  0) /**< \brief (TC_CMR) XC1 */
#define   TC_CMR_TCCLKS_XC2         (0x7u <<  0) /**< \brief (TC_CMR) XC2 */
#define TC_CMR_CLKI_Pos             3            /**< \brief (TC_CMR) Clock Invert */
#define TC_CMR_CLKI                 (0x1u << TC_CMR_CLKI_Pos)
#define   TC_CMR_CLKI_0             (0x0u <<  3) /**< \brief (TC_CMR) Counter is incremented on rising edge of the clock. */
#define   TC_CMR_CLKI_1             (0x1u <<  3) /**< \brief (TC_CMR) Counter is incremented on falling edge of the clock. */
#define TC_CMR_BURST_Pos            4            /**< \brief (TC_CMR) Burst Signal Selection */
#define TC_CMR_BURST_Msk            (0x3u << TC_CMR_BURST_Pos)
#define TC_CMR_BURST(value)         ((TC_CMR_BURST_Msk & ((value) << TC_CMR_BURST_Pos)))
#define   TC_CMR_BURST_NOT_GATED    (0x0u <<  4) /**< \brief (TC_CMR) The clock is not gated by an external signal. */
#define   TC_CMR_BURST_CLK_AND_XC0  (0x1u <<  4) /**< \brief (TC_CMR) XC0 is ANDed with the selected clock. */
#define   TC_CMR_BURST_CLK_AND_XC1  (0x2u <<  4) /**< \brief (TC_CMR) XC1 is ANDed with the selected clock. */
#define   TC_CMR_BURST_CLK_AND_XC2  (0x3u <<  4) /**< \brief (TC_CMR) XC2 is ANDed with the selected clock. */
#define TC_CMR_LDBSTOP_Pos          6            /**< \brief (TC_CMR) Counter Clock Stopped with RB Loading */
#define TC_CMR_LDBSTOP              (0x1u << TC_CMR_LDBSTOP_Pos)
#define   TC_CMR_LDBSTOP_0          (0x0u <<  6) /**< \brief (TC_CMR) Counter clock is not stopped when RB loading occurs. */
#define   TC_CMR_LDBSTOP_1          (0x1u <<  6) /**< \brief (TC_CMR) Counter clock is stopped when RB loading occurs. */
#define TC_CMR_CPCSTOP_Pos          6            /**< \brief (TC_CMR) Counter Clock Stopped with RC Compare */
#define TC_CMR_CPCSTOP              (0x1u << TC_CMR_CPCSTOP_Pos)
#define   TC_CMR_CPCSTOP_0          (0x0u <<  6) /**< \brief (TC_CMR) Counter clock is not stopped when counter reaches RC. */
#define   TC_CMR_CPCSTOP_1          (0x1u <<  6) /**< \brief (TC_CMR) Counter clock is stopped when counter reaches RC. */
#define TC_CMR_LDBDIS_Pos           7            /**< \brief (TC_CMR) Counter Clock Disable with RB Loading */
#define TC_CMR_LDBDIS               (0x1u << TC_CMR_LDBDIS_Pos)
#define   TC_CMR_LDBDIS_0           (0x0u <<  7) /**< \brief (TC_CMR) Counter clock is not disabled when RB loading occurs. */
#define   TC_CMR_LDBDIS_1           (0x1u <<  7) /**< \brief (TC_CMR) Counter clock is disabled when RB loading occurs. */
#define TC_CMR_CPCDIS_Pos           7            /**< \brief (TC_CMR) Counter Clock Disable with RC Compare */
#define TC_CMR_CPCDIS               (0x1u << TC_CMR_CPCDIS_Pos)
#define   TC_CMR_CPCDIS_0           (0x0u <<  7) /**< \brief (TC_CMR) Counter clock is not disabled when counter reaches RC. */
#define   TC_CMR_CPCDIS_1           (0x1u <<  7) /**< \brief (TC_CMR) Counter clock is disabled when counter reaches RC. */
#define TC_CMR_ETRGEDG_Pos          8            /**< \brief (TC_CMR) External Trigger Edge Selection */
#define TC_CMR_ETRGEDG_Msk          (0x3u << TC_CMR_ETRGEDG_Pos)
#define TC_CMR_ETRGEDG(value)       ((TC_CMR_ETRGEDG_Msk & ((value) << TC_CMR_ETRGEDG_Pos)))
#define   TC_CMR_ETRGEDG_NO_EDGE    (0x0u <<  8) /**< \brief (TC_CMR) none */
#define   TC_CMR_ETRGEDG_POS_EDGE   (0x1u <<  8) /**< \brief (TC_CMR) rising edge */
#define   TC_CMR_ETRGEDG_NEG_EDGE   (0x2u <<  8) /**< \brief (TC_CMR) falling edge */
#define   TC_CMR_ETRGEDG_BOTH_EDGES (0x3u <<  8) /**< \brief (TC_CMR) each edge */
#define TC_CMR_EEVTEDG_Pos          8            /**< \brief (TC_CMR) External Event Edge Selection */
#define TC_CMR_EEVTEDG_Msk          (0x3u << TC_CMR_EEVTEDG_Pos)
#define TC_CMR_EEVTEDG(value)       ((TC_CMR_EEVTEDG_Msk & ((value) << TC_CMR_EEVTEDG_Pos)))
#define   TC_CMR_EEVTEDG_NO_EDGE    (0x0u <<  8) /**< \brief (TC_CMR) none */
#define   TC_CMR_EEVTEDG_POS_EDGE   (0x1u <<  8) /**< \brief (TC_CMR) rising edge */
#define   TC_CMR_EEVTEDG_NEG_EDGE   (0x2u <<  8) /**< \brief (TC_CMR) falling edge */
#define   TC_CMR_EEVTEDG_BOTH_EDGES (0x3u <<  8) /**< \brief (TC_CMR) each edge */
#define TC_CMR_ABETRG_Pos           10           /**< \brief (TC_CMR) TIOA or TIOB External Trigger Selection */
#define TC_CMR_ABETRG               (0x1u << TC_CMR_ABETRG_Pos)
#define   TC_CMR_ABETRG_0           (0x0u << 10) /**< \brief (TC_CMR) TIOB is used as an external trigger. */
#define   TC_CMR_ABETRG_1           (0x1u << 10) /**< \brief (TC_CMR) TIOA is used as an external trigger. */
#define TC_CMR_EEVT_Pos             10           /**< \brief (TC_CMR) External Event Selection */
#define TC_CMR_EEVT_Msk             (0x3u << TC_CMR_EEVT_Pos)
#define TC_CMR_EEVT(value)          ((TC_CMR_EEVT_Msk & ((value) << TC_CMR_EEVT_Pos)))
#define   TC_CMR_EEVT_TIOB_INPUT    (0x0u << 10) /**< \brief (TC_CMR) TIOB input. If TIOB is chosen as the external event signal, it is configured as an input and no longer generates waveforms. */
#define   TC_CMR_EEVT_XC0_OUTPUT    (0x1u << 10) /**< \brief (TC_CMR) XC0 output */
#define   TC_CMR_EEVT_XC1_OUTPUT    (0x2u << 10) /**< \brief (TC_CMR) XC1 output */
#define   TC_CMR_EEVT_XC2_OUTPUT    (0x3u << 10) /**< \brief (TC_CMR) XC2 output */
#define TC_CMR_ENETRG_Pos           12           /**< \brief (TC_CMR) External Event Trigger Enable */
#define TC_CMR_ENETRG               (0x1u << TC_CMR_ENETRG_Pos)
#define   TC_CMR_ENETRG_0           (0x0u << 12) /**< \brief (TC_CMR) The external event has no effect on the counter and its clock. In this case, the selected external event only controls the TIOA output. */
#define   TC_CMR_ENETRG_1           (0x1u << 12) /**< \brief (TC_CMR) The external event resets the counter and starts the counter clock. */
#define TC_CMR_WAVSEL_Pos           13           /**< \brief (TC_CMR) Waveform Selection */
#define TC_CMR_WAVSEL_Msk           (0x3u << TC_CMR_WAVSEL_Pos)
#define TC_CMR_WAVSEL(value)        ((TC_CMR_WAVSEL_Msk & ((value) << TC_CMR_WAVSEL_Pos)))
#define   TC_CMR_WAVSEL_UP_NO_AUTO  (0x0u << 13) /**< \brief (TC_CMR) UP mode without automatic trigger on RC Compare */
#define   TC_CMR_WAVSEL_UPDOWN_NO_AUTO (0x1u << 13) /**< \brief (TC_CMR) UPDOWN mode without automatic trigger on RC Compare */
#define   TC_CMR_WAVSEL_UP_AUTO     (0x2u << 13) /**< \brief (TC_CMR) UP mode with automatic trigger on RC Compare */
#define   TC_CMR_WAVSEL_UPDOWN_AUTO (0x3u << 13) /**< \brief (TC_CMR) UPDOWN mode with automatic trigger on RC Compare */
#define TC_CMR_CPCTRG_Pos           14           /**< \brief (TC_CMR) RC Compare Trigger Enable */
#define TC_CMR_CPCTRG               (0x1u << TC_CMR_CPCTRG_Pos)
#define   TC_CMR_CPCTRG_0           (0x0u << 14) /**< \brief (TC_CMR) RC Compare has no effect on the counter and its clock. */
#define   TC_CMR_CPCTRG_1           (0x1u << 14) /**< \brief (TC_CMR) RC Compare resets the counter and starts the counter clock. */
#define TC_CMR_WAVE_Pos             15           /**< \brief (TC_CMR) Wave */
#define TC_CMR_WAVE                 (0x1u << TC_CMR_WAVE_Pos)
#define   TC_CMR_WAVE_0             (0x0u << 15) /**< \brief (TC_CMR) Capture Mode is enabled. */
#define   TC_CMR_WAVE_1             (0x1u << 15) /**< \brief (TC_CMR) Capture Mode is disabled (Waveform Mode is enabled). */
#define TC_CMR_LDRA_Pos             16           /**< \brief (TC_CMR) RA Loading Selection */
#define TC_CMR_LDRA_Msk             (0x3u << TC_CMR_LDRA_Pos)
#define TC_CMR_LDRA(value)          ((TC_CMR_LDRA_Msk & ((value) << TC_CMR_LDRA_Pos)))
#define   TC_CMR_LDRA_NO_EDGE       (0x0u << 16) /**< \brief (TC_CMR) none */
#define   TC_CMR_LDRA_POS_EDGE_TIOA (0x1u << 16) /**< \brief (TC_CMR) rising edge of TIOA */
#define   TC_CMR_LDRA_NEG_EDGE_TIOA (0x2u << 16) /**< \brief (TC_CMR) falling edge of TIOA */
#define   TC_CMR_LDRA_BOTH_EDGES_TIOA (0x3u << 16) /**< \brief (TC_CMR) each edge of TIOA */
#define TC_CMR_ACPA_Pos             16           /**< \brief (TC_CMR) RA Compare Effect on TIOA */
#define TC_CMR_ACPA_Msk             (0x3u << TC_CMR_ACPA_Pos)
#define TC_CMR_ACPA(value)          ((TC_CMR_ACPA_Msk & ((value) << TC_CMR_ACPA_Pos)))
#define   TC_CMR_ACPA_NONE          (0x0u << 16) /**< \brief (TC_CMR) none */
#define   TC_CMR_ACPA_SET           (0x1u << 16) /**< \brief (TC_CMR) set */
#define   TC_CMR_ACPA_CLEAR         (0x2u << 16) /**< \brief (TC_CMR) clear */
#define   TC_CMR_ACPA_TOGGLE        (0x3u << 16) /**< \brief (TC_CMR) toggle */
#define TC_CMR_LDRB_Pos             18           /**< \brief (TC_CMR) RB Loading Selection */
#define TC_CMR_LDRB_Msk             (0x3u << TC_CMR_LDRB_Pos)
#define TC_CMR_LDRB(value)          ((TC_CMR_LDRB_Msk & ((value) << TC_CMR_LDRB_Pos)))
#define   TC_CMR_LDRB_NO_EDGE       (0x0u << 18) /**< \brief (TC_CMR) none */
#define   TC_CMR_LDRB_POS_EDGE_TIOA (0x1u << 18) /**< \brief (TC_CMR) rising edge of TIOA */
#define   TC_CMR_LDRB_NEG_EDGE_TIOA (0x2u << 18) /**< \brief (TC_CMR) falling edge of TIOA */
#define   TC_CMR_LDRB_BOTH_EDGES_TIOA (0x3u << 18) /**< \brief (TC_CMR) each edge of TIOA */
#define TC_CMR_ACPC_Pos             18           /**< \brief (TC_CMR) RC Compare Effect on TIOA */
#define TC_CMR_ACPC_Msk             (0x3u << TC_CMR_ACPC_Pos)
#define TC_CMR_ACPC(value)          ((TC_CMR_ACPC_Msk & ((value) << TC_CMR_ACPC_Pos)))
#define   TC_CMR_ACPC_NONE          (0x0u << 18) /**< \brief (TC_CMR) none */
#define   TC_CMR_ACPC_SET           (0x1u << 18) /**< \brief (TC_CMR) set */
#define   TC_CMR_ACPC_CLEAR         (0x2u << 18) /**< \brief (TC_CMR) clear */
#define   TC_CMR_ACPC_TOGGLE        (0x3u << 18) /**< \brief (TC_CMR) toggle */
#define TC_CMR_AEEVT_Pos            20           /**< \brief (TC_CMR) External Event Effect on TIOA */
#define TC_CMR_AEEVT_Msk            (0x3u << TC_CMR_AEEVT_Pos)
#define TC_CMR_AEEVT(value)         ((TC_CMR_AEEVT_Msk & ((value) << TC_CMR_AEEVT_Pos)))
#define   TC_CMR_AEEVT_NONE         (0x0u << 20) /**< \brief (TC_CMR) none */
#define   TC_CMR_AEEVT_SET          (0x1u << 20) /**< \brief (TC_CMR) set */
#define   TC_CMR_AEEVT_CLEAR        (0x2u << 20) /**< \brief (TC_CMR) clear */
#define   TC_CMR_AEEVT_TOGGLE       (0x3u << 20) /**< \brief (TC_CMR) toggle */
#define TC_CMR_ASWTRG_Pos           22           /**< \brief (TC_CMR) Software Trigger Effect on TIOA */
#define TC_CMR_ASWTRG_Msk           (0x3u << TC_CMR_ASWTRG_Pos)
#define TC_CMR_ASWTRG(value)        ((TC_CMR_ASWTRG_Msk & ((value) << TC_CMR_ASWTRG_Pos)))
#define   TC_CMR_ASWTRG_NONE        (0x0u << 22) /**< \brief (TC_CMR) none */
#define   TC_CMR_ASWTRG_SET         (0x1u << 22) /**< \brief (TC_CMR) set */
#define   TC_CMR_ASWTRG_CLEAR       (0x2u << 22) /**< \brief (TC_CMR) clear */
#define   TC_CMR_ASWTRG_TOGGLE      (0x3u << 22) /**< \brief (TC_CMR) toggle */
#define TC_CMR_BCPB_Pos             24           /**< \brief (TC_CMR) RB Compare Effect on TIOB */
#define TC_CMR_BCPB_Msk             (0x3u << TC_CMR_BCPB_Pos)
#define TC_CMR_BCPB(value)          ((TC_CMR_BCPB_Msk & ((value) << TC_CMR_BCPB_Pos)))
#define   TC_CMR_BCPB_NONE          (0x0u << 24) /**< \brief (TC_CMR) none */
#define   TC_CMR_BCPB_SET           (0x1u << 24) /**< \brief (TC_CMR) set */
#define   TC_CMR_BCPB_CLEAR         (0x2u << 24) /**< \brief (TC_CMR) clear */
#define   TC_CMR_BCPB_TOGGLE        (0x3u << 24) /**< \brief (TC_CMR) toggle */
#define TC_CMR_BCPC_Pos             26           /**< \brief (TC_CMR) RC Compare Effect on TIOB */
#define TC_CMR_BCPC_Msk             (0x3u << TC_CMR_BCPC_Pos)
#define TC_CMR_BCPC(value)          ((TC_CMR_BCPC_Msk & ((value) << TC_CMR_BCPC_Pos)))
#define   TC_CMR_BCPC_NONE          (0x0u << 26) /**< \brief (TC_CMR) none */
#define   TC_CMR_BCPC_SET           (0x1u << 26) /**< \brief (TC_CMR) set */
#define   TC_CMR_BCPC_CLEAR         (0x2u << 26) /**< \brief (TC_CMR) clear */
#define   TC_CMR_BCPC_TOGGLE        (0x3u << 26) /**< \brief (TC_CMR) toggle */
#define TC_CMR_BEEVT_Pos            28           /**< \brief (TC_CMR) External Event Effect on TIOB */
#define TC_CMR_BEEVT_Msk            (0x3u << TC_CMR_BEEVT_Pos)
#define TC_CMR_BEEVT(value)         ((TC_CMR_BEEVT_Msk & ((value) << TC_CMR_BEEVT_Pos)))
#define   TC_CMR_BEEVT_NONE         (0x0u << 28) /**< \brief (TC_CMR) none */
#define   TC_CMR_BEEVT_SET          (0x1u << 28) /**< \brief (TC_CMR) set */
#define   TC_CMR_BEEVT_CLEAR        (0x2u << 28) /**< \brief (TC_CMR) clear */
#define   TC_CMR_BEEVT_TOGGLE       (0x3u << 28) /**< \brief (TC_CMR) toggle */
#define TC_CMR_BSWTRG_Pos           30           /**< \brief (TC_CMR) Software Trigger Effect on TIOB */
#define TC_CMR_BSWTRG_Msk           (0x3u << TC_CMR_BSWTRG_Pos)
#define TC_CMR_BSWTRG(value)        ((TC_CMR_BSWTRG_Msk & ((value) << TC_CMR_BSWTRG_Pos)))
#define   TC_CMR_BSWTRG_NONE        (0x0u << 30) /**< \brief (TC_CMR) none */
#define   TC_CMR_BSWTRG_SET         (0x1u << 30) /**< \brief (TC_CMR) set */
#define   TC_CMR_BSWTRG_CLEAR       (0x2u << 30) /**< \brief (TC_CMR) clear */
#define   TC_CMR_BSWTRG_TOGGLE      (0x3u << 30) /**< \brief (TC_CMR) toggle */
#define TC_CMR_MASK                 0xFFFFFFFFu  /**< \brief (TC_CMR) MASK Register */

/* -------- TC_SMMR : (TC Offset: 0x08) (R/W 32) channel Stepper Motor Mode Register -------- */
#define TC_SMMR_OFFSET              0x08         /**< \brief (TC_SMMR offset) Stepper Motor Mode Register */
#define TC_SMMR_RESETVALUE          0x00000000   /**< \brief (TC_SMMR reset_value) Stepper Motor Mode Register */

#define TC_SMMR_GCEN_Pos            0            /**< \brief (TC_SMMR) Gray Count Enable */
#define TC_SMMR_GCEN                (0x1u << TC_SMMR_GCEN_Pos)
#define TC_SMMR_DOWN_Pos            1            /**< \brief (TC_SMMR) Down Count */
#define TC_SMMR_DOWN                (0x1u << TC_SMMR_DOWN_Pos)
#define TC_SMMR_MASK                0x00000003u  /**< \brief (TC_SMMR) MASK Register */

/* -------- TC_CV : (TC Offset: 0x10) (R/  32) channel Counter Value Channel -------- */
#define TC_CV_OFFSET                0x10         /**< \brief (TC_CV offset) Counter Value Channel */
#define TC_CV_RESETVALUE            0x00000000   /**< \brief (TC_CV reset_value) Counter Value Channel */

#define TC_CV_CV_Pos                0            /**< \brief (TC_CV) Counter Value */
#define TC_CV_CV_Msk                (0xFFFFu << TC_CV_CV_Pos)
#define TC_CV_CV(value)             ((TC_CV_CV_Msk & ((value) << TC_CV_CV_Pos)))
#define TC_CV_MASK                  0x0000FFFFu  /**< \brief (TC_CV) MASK Register */

/* -------- TC_RA : (TC Offset: 0x14) (R/W 32) channel Register A Channel -------- */
#define TC_RA_OFFSET                0x14         /**< \brief (TC_RA offset) Register A Channel */
#define TC_RA_RESETVALUE            0x00000000   /**< \brief (TC_RA reset_value) Register A Channel */

#define TC_RA_RA_Pos                0            /**< \brief (TC_RA) Register A */
#define TC_RA_RA_Msk                (0xFFFFu << TC_RA_RA_Pos)
#define TC_RA_RA(value)             ((TC_RA_RA_Msk & ((value) << TC_RA_RA_Pos)))
#define TC_RA_MASK                  0x0000FFFFu  /**< \brief (TC_RA) MASK Register */

/* -------- TC_RB : (TC Offset: 0x18) (R/W 32) channel Register B Channel -------- */
#define TC_RB_OFFSET                0x18         /**< \brief (TC_RB offset) Register B Channel */
#define TC_RB_RESETVALUE            0x00000000   /**< \brief (TC_RB reset_value) Register B Channel */

#define TC_RB_RB_Pos                0            /**< \brief (TC_RB) Register B */
#define TC_RB_RB_Msk                (0xFFFFu << TC_RB_RB_Pos)
#define TC_RB_RB(value)             ((TC_RB_RB_Msk & ((value) << TC_RB_RB_Pos)))
#define TC_RB_MASK                  0x0000FFFFu  /**< \brief (TC_RB) MASK Register */

/* -------- TC_RC : (TC Offset: 0x1C) (R/W 32) channel Register C Channel -------- */
#define TC_RC_OFFSET                0x1C         /**< \brief (TC_RC offset) Register C Channel */
#define TC_RC_RESETVALUE            0x00000000   /**< \brief (TC_RC reset_value) Register C Channel */

#define TC_RC_RC_Pos                0            /**< \brief (TC_RC) Register C */
#define TC_RC_RC_Msk                (0xFFFFu << TC_RC_RC_Pos)
#define TC_RC_RC(value)             ((TC_RC_RC_Msk & ((value) << TC_RC_RC_Pos)))
#define TC_RC_MASK                  0x0000FFFFu  /**< \brief (TC_RC) MASK Register */

/* -------- TC_SR : (TC Offset: 0x20) (R/  32) channel Status Register Channel -------- */
#define TC_SR_OFFSET                0x20         /**< \brief (TC_SR offset) Status Register Channel */
#define TC_SR_RESETVALUE            0x00000000   /**< \brief (TC_SR reset_value) Status Register Channel */

#define TC_SR_COVFS_Pos             0            /**< \brief (TC_SR) Counter Overflow Status */
#define TC_SR_COVFS                 (0x1u << TC_SR_COVFS_Pos)
#define   TC_SR_COVFS_0             (0x0u <<  0) /**< \brief (TC_SR) No counter overflow has occurred since the last read of the Status Register. */
#define   TC_SR_COVFS_1             (0x1u <<  0) /**< \brief (TC_SR) A counter overflow has occurred since the last read of the Status Register. */
#define TC_SR_LOVRS_Pos             1            /**< \brief (TC_SR) Load Overrun Status */
#define TC_SR_LOVRS                 (0x1u << TC_SR_LOVRS_Pos)
#define   TC_SR_LOVRS_0             (0x0u <<  1) /**< \brief (TC_SR) Load overrun has not occurred since the last read of the Status Register or WAVE:1. */
#define   TC_SR_LOVRS_1             (0x1u <<  1) /**< \brief (TC_SR) RA or RB have been loaded at least twice without any read of the corresponding register since the last read of the StatusRegister, if WAVE:0. */
#define TC_SR_CPAS_Pos              2            /**< \brief (TC_SR) RA Compare Status */
#define TC_SR_CPAS                  (0x1u << TC_SR_CPAS_Pos)
#define   TC_SR_CPAS_0              (0x0u <<  2) /**< \brief (TC_SR) RA Compare has not occurred since the last read of the Status Register or WAVE:0. */
#define   TC_SR_CPAS_1              (0x1u <<  2) /**< \brief (TC_SR) RA Compare has occurred since the last read of the Status Register, if WAVE:1. */
#define TC_SR_CPBS_Pos              3            /**< \brief (TC_SR) RB Compare Status */
#define TC_SR_CPBS                  (0x1u << TC_SR_CPBS_Pos)
#define   TC_SR_CPBS_0              (0x0u <<  3) /**< \brief (TC_SR) RB Compare has not occurred since the last read of the Status Register or WAVE:0. */
#define   TC_SR_CPBS_1              (0x1u <<  3) /**< \brief (TC_SR) RB Compare has occurred since the last read of the Status Register, if WAVE:1. */
#define TC_SR_CPCS_Pos              4            /**< \brief (TC_SR) RC Compare Status */
#define TC_SR_CPCS                  (0x1u << TC_SR_CPCS_Pos)
#define   TC_SR_CPCS_0              (0x0u <<  4) /**< \brief (TC_SR) RC Compare has not occurred since the last read of the Status Register. */
#define   TC_SR_CPCS_1              (0x1u <<  4) /**< \brief (TC_SR) RC Compare has occurred since the last read of the Status Register. */
#define TC_SR_LDRAS_Pos             5            /**< \brief (TC_SR) RA Loading Status */
#define TC_SR_LDRAS                 (0x1u << TC_SR_LDRAS_Pos)
#define   TC_SR_LDRAS_0             (0x0u <<  5) /**< \brief (TC_SR) RA Load has not occurred since the last read of the Status Register or WAVE:1. */
#define   TC_SR_LDRAS_1             (0x1u <<  5) /**< \brief (TC_SR) RA Load has occurred since the last read of the Status Register, if WAVE:0. */
#define TC_SR_LDRBS_Pos             6            /**< \brief (TC_SR) RB Loading Status */
#define TC_SR_LDRBS                 (0x1u << TC_SR_LDRBS_Pos)
#define   TC_SR_LDRBS_0             (0x0u <<  6) /**< \brief (TC_SR) RB Load has not occurred since the last read of the Status Register or WAVE:1. */
#define   TC_SR_LDRBS_1             (0x1u <<  6) /**< \brief (TC_SR) RB Load has occurred since the last read of the Status Register, if WAVE:0. */
#define TC_SR_ETRGS_Pos             7            /**< \brief (TC_SR) External Trigger Status */
#define TC_SR_ETRGS                 (0x1u << TC_SR_ETRGS_Pos)
#define   TC_SR_ETRGS_0             (0x0u <<  7) /**< \brief (TC_SR) External trigger has not occurred since the last read of the Status Register. */
#define   TC_SR_ETRGS_1             (0x1u <<  7) /**< \brief (TC_SR) External trigger has occurred since the last read of the Status Register. */
#define TC_SR_CLKSTA_Pos            16           /**< \brief (TC_SR) Clock Enabling Status */
#define TC_SR_CLKSTA                (0x1u << TC_SR_CLKSTA_Pos)
#define   TC_SR_CLKSTA_0            (0x0u << 16) /**< \brief (TC_SR) Clock is disabled. */
#define   TC_SR_CLKSTA_1            (0x1u << 16) /**< \brief (TC_SR) Clock is enabled. */
#define TC_SR_MTIOA_Pos             17           /**< \brief (TC_SR) TIOA Mirror */
#define TC_SR_MTIOA                 (0x1u << TC_SR_MTIOA_Pos)
#define   TC_SR_MTIOA_0             (0x0u << 17) /**< \brief (TC_SR) TIOA is low. If WAVE:0, this means that TIOA pin is low. If WAVE:1, this means that TIOA is driven low. */
#define   TC_SR_MTIOA_1             (0x1u << 17) /**< \brief (TC_SR) TIOA is high. If WAVE:0, this means that TIOA pin is high. If WAVE:1, this means that TIOA is driven high. */
#define TC_SR_MTIOB_Pos             18           /**< \brief (TC_SR) TIOB Mirror */
#define TC_SR_MTIOB                 (0x1u << TC_SR_MTIOB_Pos)
#define   TC_SR_MTIOB_0             (0x0u << 18) /**< \brief (TC_SR) TIOB is low. If WAVE:0, this means that TIOB pin is low. If WAVE:1, this means that TIOB is driven low. */
#define   TC_SR_MTIOB_1             (0x1u << 18) /**< \brief (TC_SR) TIOB is high. If WAVE:0, this means that TIOB pin is high. If WAVE:1, this means that TIOB is driven high. */
#define TC_SR_MASK                  0x000700FFu  /**< \brief (TC_SR) MASK Register */

/* -------- TC_IER : (TC Offset: 0x24) ( /W 32) channel Interrupt Enable Register Channel -------- */
#define TC_IER_OFFSET               0x24         /**< \brief (TC_IER offset) Interrupt Enable Register Channel */
#define TC_IER_RESETVALUE           0x00000000   /**< \brief (TC_IER reset_value) Interrupt Enable Register Channel */

#define TC_IER_COVFS_Pos            0            /**< \brief (TC_IER) Counter Overflow */
#define TC_IER_COVFS                (0x1u << TC_IER_COVFS_Pos)
#define   TC_IER_COVFS_0            (0x0u <<  0) /**< \brief (TC_IER) No effect. */
#define   TC_IER_COVFS_1            (0x1u <<  0) /**< \brief (TC_IER) Enables the Counter Overflow Interrupt. */
#define TC_IER_LOVRS_Pos            1            /**< \brief (TC_IER) Load Overrun */
#define TC_IER_LOVRS                (0x1u << TC_IER_LOVRS_Pos)
#define   TC_IER_LOVRS_0            (0x0u <<  1) /**< \brief (TC_IER) No effect. */
#define   TC_IER_LOVRS_1            (0x1u <<  1) /**< \brief (TC_IER) Enables the Load Overrun Interrupt. */
#define TC_IER_CPAS_Pos             2            /**< \brief (TC_IER) RA Compare */
#define TC_IER_CPAS                 (0x1u << TC_IER_CPAS_Pos)
#define   TC_IER_CPAS_0             (0x0u <<  2) /**< \brief (TC_IER) No effect. */
#define   TC_IER_CPAS_1             (0x1u <<  2) /**< \brief (TC_IER) Enables the RA Compare Interrupt. */
#define TC_IER_CPBS_Pos             3            /**< \brief (TC_IER) RB Compare */
#define TC_IER_CPBS                 (0x1u << TC_IER_CPBS_Pos)
#define   TC_IER_CPBS_0             (0x0u <<  3) /**< \brief (TC_IER) No effect. */
#define   TC_IER_CPBS_1             (0x1u <<  3) /**< \brief (TC_IER) Enables the RB Compare Interrupt. */
#define TC_IER_CPCS_Pos             4            /**< \brief (TC_IER) RC Compare */
#define TC_IER_CPCS                 (0x1u << TC_IER_CPCS_Pos)
#define   TC_IER_CPCS_0             (0x0u <<  4) /**< \brief (TC_IER) No effect. */
#define   TC_IER_CPCS_1             (0x1u <<  4) /**< \brief (TC_IER) Enables the RC Compare Interrupt. */
#define TC_IER_LDRAS_Pos            5            /**< \brief (TC_IER) RA Loading */
#define TC_IER_LDRAS                (0x1u << TC_IER_LDRAS_Pos)
#define   TC_IER_LDRAS_0            (0x0u <<  5) /**< \brief (TC_IER) No effect. */
#define   TC_IER_LDRAS_1            (0x1u <<  5) /**< \brief (TC_IER) Enables the RA Load Interrupt. */
#define TC_IER_LDRBS_Pos            6            /**< \brief (TC_IER) RB Loading */
#define TC_IER_LDRBS                (0x1u << TC_IER_LDRBS_Pos)
#define   TC_IER_LDRBS_0            (0x0u <<  6) /**< \brief (TC_IER) No effect. */
#define   TC_IER_LDRBS_1            (0x1u <<  6) /**< \brief (TC_IER) Enables the RB Load Interrupt. */
#define TC_IER_ETRGS_Pos            7            /**< \brief (TC_IER) External Trigger */
#define TC_IER_ETRGS                (0x1u << TC_IER_ETRGS_Pos)
#define   TC_IER_ETRGS_0            (0x0u <<  7) /**< \brief (TC_IER) No effect. */
#define   TC_IER_ETRGS_1            (0x1u <<  7) /**< \brief (TC_IER) Enables the External Trigger Interrupt. */
#define TC_IER_MASK                 0x000000FFu  /**< \brief (TC_IER) MASK Register */

/* -------- TC_IDR : (TC Offset: 0x28) ( /W 32) channel Interrupt Disable Register Channel -------- */
#define TC_IDR_OFFSET               0x28         /**< \brief (TC_IDR offset) Interrupt Disable Register Channel */
#define TC_IDR_RESETVALUE           0x00000000   /**< \brief (TC_IDR reset_value) Interrupt Disable Register Channel */

#define TC_IDR_COVFS_Pos            0            /**< \brief (TC_IDR) Counter Overflow */
#define TC_IDR_COVFS                (0x1u << TC_IDR_COVFS_Pos)
#define   TC_IDR_COVFS_0            (0x0u <<  0) /**< \brief (TC_IDR) No effect. */
#define   TC_IDR_COVFS_1            (0x1u <<  0) /**< \brief (TC_IDR) Disables the Counter Overflow Interrupt. */
#define TC_IDR_LOVRS_Pos            1            /**< \brief (TC_IDR) Load Overrun */
#define TC_IDR_LOVRS                (0x1u << TC_IDR_LOVRS_Pos)
#define   TC_IDR_LOVRS_0            (0x0u <<  1) /**< \brief (TC_IDR) No effect. */
#define   TC_IDR_LOVRS_1            (0x1u <<  1) /**< \brief (TC_IDR) Disables the Load Overrun Interrupt (if WAVE:0). */
#define TC_IDR_CPAS_Pos             2            /**< \brief (TC_IDR) RA Compare */
#define TC_IDR_CPAS                 (0x1u << TC_IDR_CPAS_Pos)
#define   TC_IDR_CPAS_0             (0x0u <<  2) /**< \brief (TC_IDR) No effect. */
#define   TC_IDR_CPAS_1             (0x1u <<  2) /**< \brief (TC_IDR) Disables the RA Compare Interrupt (if WAVE:1). */
#define TC_IDR_CPBS_Pos             3            /**< \brief (TC_IDR) RB Compare */
#define TC_IDR_CPBS                 (0x1u << TC_IDR_CPBS_Pos)
#define   TC_IDR_CPBS_0             (0x0u <<  3) /**< \brief (TC_IDR) No effect. */
#define   TC_IDR_CPBS_1             (0x1u <<  3) /**< \brief (TC_IDR) Disables the RB Compare Interrupt (if WAVE:1). */
#define TC_IDR_CPCS_Pos             4            /**< \brief (TC_IDR) RC Compare */
#define TC_IDR_CPCS                 (0x1u << TC_IDR_CPCS_Pos)
#define   TC_IDR_CPCS_0             (0x0u <<  4) /**< \brief (TC_IDR) No effect. */
#define   TC_IDR_CPCS_1             (0x1u <<  4) /**< \brief (TC_IDR) Disables the RC Compare Interrupt. */
#define TC_IDR_LDRAS_Pos            5            /**< \brief (TC_IDR) RA Loading */
#define TC_IDR_LDRAS                (0x1u << TC_IDR_LDRAS_Pos)
#define   TC_IDR_LDRAS_0            (0x0u <<  5) /**< \brief (TC_IDR) No effect. */
#define   TC_IDR_LDRAS_1            (0x1u <<  5) /**< \brief (TC_IDR) Disables the RA Load Interrupt (if WAVE:0). */
#define TC_IDR_LDRBS_Pos            6            /**< \brief (TC_IDR) RB Loading */
#define TC_IDR_LDRBS                (0x1u << TC_IDR_LDRBS_Pos)
#define   TC_IDR_LDRBS_0            (0x0u <<  6) /**< \brief (TC_IDR) No effect. */
#define   TC_IDR_LDRBS_1            (0x1u <<  6) /**< \brief (TC_IDR) Disables the RB Load Interrupt (if WAVE:0). */
#define TC_IDR_ETRGS_Pos            7            /**< \brief (TC_IDR) External Trigger */
#define TC_IDR_ETRGS                (0x1u << TC_IDR_ETRGS_Pos)
#define   TC_IDR_ETRGS_0            (0x0u <<  7) /**< \brief (TC_IDR) No effect. */
#define   TC_IDR_ETRGS_1            (0x1u <<  7) /**< \brief (TC_IDR) Disables the External Trigger Interrupt. */
#define TC_IDR_MASK                 0x000000FFu  /**< \brief (TC_IDR) MASK Register */

/* -------- TC_IMR : (TC Offset: 0x2C) (R/  32) channel Interrupt Mask Register Channel -------- */
#define TC_IMR_OFFSET               0x2C         /**< \brief (TC_IMR offset) Interrupt Mask Register Channel */
#define TC_IMR_RESETVALUE           0x00000000   /**< \brief (TC_IMR reset_value) Interrupt Mask Register Channel */

#define TC_IMR_COVFS_Pos            0            /**< \brief (TC_IMR) Counter Overflow */
#define TC_IMR_COVFS                (0x1u << TC_IMR_COVFS_Pos)
#define   TC_IMR_COVFS_0            (0x0u <<  0) /**< \brief (TC_IMR) The Counter Overflow Interrupt is disabled. */
#define   TC_IMR_COVFS_1            (0x1u <<  0) /**< \brief (TC_IMR) The Counter Overflow Interrupt is enabled. */
#define TC_IMR_LOVRS_Pos            1            /**< \brief (TC_IMR) Load Overrun */
#define TC_IMR_LOVRS                (0x1u << TC_IMR_LOVRS_Pos)
#define   TC_IMR_LOVRS_0            (0x0u <<  1) /**< \brief (TC_IMR) The Load Overrun Interrupt is disabled. */
#define   TC_IMR_LOVRS_1            (0x1u <<  1) /**< \brief (TC_IMR) The Load Overrun Interrupt is enabled. */
#define TC_IMR_CPAS_Pos             2            /**< \brief (TC_IMR) RA Compare */
#define TC_IMR_CPAS                 (0x1u << TC_IMR_CPAS_Pos)
#define   TC_IMR_CPAS_0             (0x0u <<  2) /**< \brief (TC_IMR) The RA Compare Interrupt is disabled. */
#define   TC_IMR_CPAS_1             (0x1u <<  2) /**< \brief (TC_IMR) The RA Compare Interrupt is enabled. */
#define TC_IMR_CPBS_Pos             3            /**< \brief (TC_IMR) RB Compare */
#define TC_IMR_CPBS                 (0x1u << TC_IMR_CPBS_Pos)
#define   TC_IMR_CPBS_0             (0x0u <<  3) /**< \brief (TC_IMR) The RB Compare Interrupt is disabled. */
#define   TC_IMR_CPBS_1             (0x1u <<  3) /**< \brief (TC_IMR) The RB Compare Interrupt is enabled. */
#define TC_IMR_CPCS_Pos             4            /**< \brief (TC_IMR) RC Compare */
#define TC_IMR_CPCS                 (0x1u << TC_IMR_CPCS_Pos)
#define   TC_IMR_CPCS_0             (0x0u <<  4) /**< \brief (TC_IMR) The RC Compare Interrupt is disabled. */
#define   TC_IMR_CPCS_1             (0x1u <<  4) /**< \brief (TC_IMR) The RC Compare Interrupt is enabled. */
#define TC_IMR_LDRAS_Pos            5            /**< \brief (TC_IMR) RA Loading */
#define TC_IMR_LDRAS                (0x1u << TC_IMR_LDRAS_Pos)
#define   TC_IMR_LDRAS_0            (0x0u <<  5) /**< \brief (TC_IMR) The Load RA Interrupt is disabled. */
#define   TC_IMR_LDRAS_1            (0x1u <<  5) /**< \brief (TC_IMR) The Load RA Interrupt is enabled. */
#define TC_IMR_LDRBS_Pos            6            /**< \brief (TC_IMR) RB Loading */
#define TC_IMR_LDRBS                (0x1u << TC_IMR_LDRBS_Pos)
#define   TC_IMR_LDRBS_0            (0x0u <<  6) /**< \brief (TC_IMR) The Load RB Interrupt is disabled. */
#define   TC_IMR_LDRBS_1            (0x1u <<  6) /**< \brief (TC_IMR) The Load RB Interrupt is enabled. */
#define TC_IMR_ETRGS_Pos            7            /**< \brief (TC_IMR) External Trigger */
#define TC_IMR_ETRGS                (0x1u << TC_IMR_ETRGS_Pos)
#define   TC_IMR_ETRGS_0            (0x0u <<  7) /**< \brief (TC_IMR) The External Trigger Interrupt is disabled. */
#define   TC_IMR_ETRGS_1            (0x1u <<  7) /**< \brief (TC_IMR) The External Trigger Interrupt is enabled. */
#define TC_IMR_MASK                 0x000000FFu  /**< \brief (TC_IMR) MASK Register */

/* -------- TC_BCR : (TC Offset: 0xC0) ( /W 32) TC Block Control Register -------- */
#define TC_BCR_OFFSET               0xC0         /**< \brief (TC_BCR offset) TC Block Control Register */
#define TC_BCR_RESETVALUE           0x00000000   /**< \brief (TC_BCR reset_value) TC Block Control Register */

#define TC_BCR_SYNC_Pos             0            /**< \brief (TC_BCR) Synchro Command */
#define TC_BCR_SYNC                 (0x1u << TC_BCR_SYNC_Pos)
#define   TC_BCR_SYNC_0             (0x0u <<  0) /**< \brief (TC_BCR) No effect. */
#define   TC_BCR_SYNC_1             (0x1u <<  0) /**< \brief (TC_BCR) Asserts the SYNC signal which generates a software trigger simultaneously for each of the channels. */
#define TC_BCR_MASK                 0x00000001u  /**< \brief (TC_BCR) MASK Register */

/* -------- TC_BMR : (TC Offset: 0xC4) (R/W 32) TC Block Mode Register -------- */
#define TC_BMR_OFFSET               0xC4         /**< \brief (TC_BMR offset) TC Block Mode Register */
#define TC_BMR_RESETVALUE           0x00000000   /**< \brief (TC_BMR reset_value) TC Block Mode Register */

#define TC_BMR_TC0XC0S_Pos          0            /**< \brief (TC_BMR) External Clock Signal 0 Selection */
#define TC_BMR_TC0XC0S_Msk          (0x3u << TC_BMR_TC0XC0S_Pos)
#define TC_BMR_TC0XC0S(value)       ((TC_BMR_TC0XC0S_Msk & ((value) << TC_BMR_TC0XC0S_Pos)))
#define   TC_BMR_TC0XC0S_TCLK0      (0x0u <<  0) /**< \brief (TC_BMR) Select TCLK0 as clock signal 0. */
#define   TC_BMR_TC0XC0S_NO_CLK     (0x1u <<  0) /**< \brief (TC_BMR) Select no clock as clock signal 0. */
#define   TC_BMR_TC0XC0S_TIOA1      (0x2u <<  0) /**< \brief (TC_BMR) Select TIOA1 as clock signal 0. */
#define   TC_BMR_TC0XC0S_TIOA2      (0x3u <<  0) /**< \brief (TC_BMR) Select TIOA2 as clock signal 0. */
#define TC_BMR_TC1XC1S_Pos          2            /**< \brief (TC_BMR) External Clock Signal 1 Selection */
#define TC_BMR_TC1XC1S_Msk          (0x3u << TC_BMR_TC1XC1S_Pos)
#define TC_BMR_TC1XC1S(value)       ((TC_BMR_TC1XC1S_Msk & ((value) << TC_BMR_TC1XC1S_Pos)))
#define   TC_BMR_TC1XC1S_TCLK1      (0x0u <<  2) /**< \brief (TC_BMR) Select TCLK1 as clock signal 1. */
#define   TC_BMR_TC1XC1S_NO_CLK     (0x1u <<  2) /**< \brief (TC_BMR) Select no clock as clock signal 1. */
#define   TC_BMR_TC1XC1S_TIOA0      (0x2u <<  2) /**< \brief (TC_BMR) Select TIOA0 as clock signal 1. */
#define   TC_BMR_TC1XC1S_TIOA2      (0x3u <<  2) /**< \brief (TC_BMR) Select TIOA2 as clock signal 1. */
#define TC_BMR_TC2XC2S_Pos          4            /**< \brief (TC_BMR) External Clock Signal 2 Selection */
#define TC_BMR_TC2XC2S_Msk          (0x3u << TC_BMR_TC2XC2S_Pos)
#define TC_BMR_TC2XC2S(value)       ((TC_BMR_TC2XC2S_Msk & ((value) << TC_BMR_TC2XC2S_Pos)))
#define   TC_BMR_TC2XC2S_TCLK2      (0x0u <<  4) /**< \brief (TC_BMR) Select TCLK2 as clock signal 2. */
#define   TC_BMR_TC2XC2S_NO_CLK     (0x1u <<  4) /**< \brief (TC_BMR) Select no clock as clock signal 2. */
#define   TC_BMR_TC2XC2S_TIOA0      (0x2u <<  4) /**< \brief (TC_BMR) Select TIOA0 as clock signal 2. */
#define   TC_BMR_TC2XC2S_TIOA1      (0x3u <<  4) /**< \brief (TC_BMR) Select TIOA1 as clock signal 2. */
#define TC_BMR_MASK                 0x0000003Fu  /**< \brief (TC_BMR) MASK Register */

/* -------- TC_WPMR : (TC Offset: 0xE4) (R/W 32) Write Protect Mode Register -------- */
#define TC_WPMR_OFFSET              0xE4         /**< \brief (TC_WPMR offset) Write Protect Mode Register */
#define TC_WPMR_RESETVALUE          0x00000000   /**< \brief (TC_WPMR reset_value) Write Protect Mode Register */

#define TC_WPMR_WPEN_Pos            0            /**< \brief (TC_WPMR) Write Protect Enable */
#define TC_WPMR_WPEN                (0x1u << TC_WPMR_WPEN_Pos)
#define TC_WPMR_WPKEY_Pos           8            /**< \brief (TC_WPMR) Write Protect Key */
#define TC_WPMR_WPKEY_Msk           (0xFFFFFFu << TC_WPMR_WPKEY_Pos)
#define TC_WPMR_WPKEY(value)        ((TC_WPMR_WPKEY_Msk & ((value) << TC_WPMR_WPKEY_Pos)))
#define TC_WPMR_MASK                0xFFFFFF01u  /**< \brief (TC_WPMR) MASK Register */

/* -------- TC_FEATURES : (TC Offset: 0xF8) (R/  32) Features Register -------- */
#define TC_FEATURES_OFFSET          0xF8         /**< \brief (TC_FEATURES offset) Features Register */

#define TC_FEATURES_CTRSIZE_Pos     0            /**< \brief (TC_FEATURES) Counter Size */
#define TC_FEATURES_CTRSIZE_Msk     (0xFFu << TC_FEATURES_CTRSIZE_Pos)
#define TC_FEATURES_CTRSIZE(value)  ((TC_FEATURES_CTRSIZE_Msk & ((value) << TC_FEATURES_CTRSIZE_Pos)))
#define TC_FEATURES_UPDNIMPL_Pos    8            /**< \brief (TC_FEATURES) Up Down is Implemented */
#define TC_FEATURES_UPDNIMPL        (0x1u << TC_FEATURES_UPDNIMPL_Pos)
#define TC_FEATURES_BRPBHSB_Pos     9            /**< \brief (TC_FEATURES) Bridge Type is PB to HSB */
#define TC_FEATURES_BRPBHSB         (0x1u << TC_FEATURES_BRPBHSB_Pos)
#define TC_FEATURES_MASK            0x000003FFu  /**< \brief (TC_FEATURES) MASK Register */

/* -------- TC_VERSION : (TC Offset: 0xFC) (R/  32) Version Register -------- */
#define TC_VERSION_OFFSET           0xFC         /**< \brief (TC_VERSION offset) Version Register */
#define TC_VERSION_RESETVALUE       0x00000402   /**< \brief (TC_VERSION reset_value) Version Register */

#define TC_VERSION_VERSION_Pos      0            /**< \brief (TC_VERSION) Reserved. Value subject to change. No functionality associated. This is the Atmel internal version of the macrocell. */
#define TC_VERSION_VERSION_Msk      (0xFFFu << TC_VERSION_VERSION_Pos)
#define TC_VERSION_VERSION(value)   ((TC_VERSION_VERSION_Msk & ((value) << TC_VERSION_VERSION_Pos)))
#define TC_VERSION_VARIANT_Pos      16           /**< \brief (TC_VERSION) Reserved.  Value subject to change.  No functionality associated. */
#define TC_VERSION_VARIANT_Msk      (0xFu << TC_VERSION_VARIANT_Pos)
#define TC_VERSION_VARIANT(value)   ((TC_VERSION_VARIANT_Msk & ((value) << TC_VERSION_VARIANT_Pos)))
#define TC_VERSION_MASK             0x000F0FFFu  /**< \brief (TC_VERSION) MASK Register */

/** \brief TcChannel hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
  WoReg   TC_CCR;             /**< \brief (TC Offset: 0x00) Channel Control Register Channel */
  RwReg   TC_CMR;             /**< \brief (TC Offset: 0x04) Channel Mode Register Channel */
  RwReg   TC_SMMR;            /**< \brief (TC Offset: 0x08) Stepper Motor Mode Register */
  RoReg8  Reserved1[0x4];
  RoReg   TC_CV;              /**< \brief (TC Offset: 0x10) Counter Value Channel */
  RwReg   TC_RA;              /**< \brief (TC Offset: 0x14) Register A Channel */
  RwReg   TC_RB;              /**< \brief (TC Offset: 0x18) Register B Channel */
  RwReg   TC_RC;              /**< \brief (TC Offset: 0x1C) Register C Channel */
  RoReg   TC_SR;              /**< \brief (TC Offset: 0x20) Status Register Channel */
  WoReg   TC_IER;             /**< \brief (TC Offset: 0x24) Interrupt Enable Register Channel */
  WoReg   TC_IDR;             /**< \brief (TC Offset: 0x28) Interrupt Disable Register Channel */
  RoReg   TC_IMR;             /**< \brief (TC Offset: 0x2C) Interrupt Mask Register Channel */
  RoReg8  Reserved2[0x10];
} TcChannel;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/** \brief TC hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
  TcChannel TC_CHANNEL[3];      /**< \brief (TC Offset: 0x00) TcChannel groups */
  WoReg   TC_BCR;             /**< \brief (TC Offset: 0xC0) TC Block Control Register */
  RwReg   TC_BMR;             /**< \brief (TC Offset: 0xC4) TC Block Mode Register */
  RoReg8  Reserved1[0x1C];
  RwReg   TC_WPMR;            /**< \brief (TC Offset: 0xE4) Write Protect Mode Register */
  RoReg8  Reserved2[0x10];
  RoReg   TC_FEATURES;        /**< \brief (TC Offset: 0xF8) Features Register */
  RoReg   TC_VERSION;         /**< \brief (TC Offset: 0xFC) Version Register */
} Tc;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/*@}*/

#endif /* _SAM4L_TC_COMPONENT_ */
