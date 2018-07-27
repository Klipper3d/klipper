/**
 * \file
 *
 * \brief Component description for ACIFC
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

#ifndef _SAM4L_ACIFC_COMPONENT_
#define _SAM4L_ACIFC_COMPONENT_

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR ACIFC */
/* ========================================================================== */
/** \addtogroup SAM4L_ACIFC Analog Comparator Interface */
/*@{*/

#define REV_ACIFC                   0x101

/* -------- ACIFC_CTRL : (ACIFC Offset: 0x00) (R/W 32) Control Register -------- */
#define ACIFC_CTRL_OFFSET           0x00         /**< \brief (ACIFC_CTRL offset) Control Register */
#define ACIFC_CTRL_RESETVALUE       0x00000000   /**< \brief (ACIFC_CTRL reset_value) Control Register */

#define ACIFC_CTRL_EN_Pos           0            /**< \brief (ACIFC_CTRL) ACIFC Enable */
#define ACIFC_CTRL_EN               (0x1u << ACIFC_CTRL_EN_Pos)
#define ACIFC_CTRL_EVENTEN_Pos      1            /**< \brief (ACIFC_CTRL) Peripheral Event Trigger Enable */
#define ACIFC_CTRL_EVENTEN          (0x1u << ACIFC_CTRL_EVENTEN_Pos)
#define ACIFC_CTRL_USTART_Pos       4            /**< \brief (ACIFC_CTRL) User Start Single Comparison */
#define ACIFC_CTRL_USTART           (0x1u << ACIFC_CTRL_USTART_Pos)
#define ACIFC_CTRL_ESTART_Pos       5            /**< \brief (ACIFC_CTRL) Peripheral Event Start Single Comparison */
#define ACIFC_CTRL_ESTART           (0x1u << ACIFC_CTRL_ESTART_Pos)
#define ACIFC_CTRL_ACTEST_Pos       7            /**< \brief (ACIFC_CTRL) Analog Comparator Test Mode */
#define ACIFC_CTRL_ACTEST           (0x1u << ACIFC_CTRL_ACTEST_Pos)
#define ACIFC_CTRL_MASK             0x000000B3u  /**< \brief (ACIFC_CTRL) MASK Register */

/* -------- ACIFC_SR : (ACIFC Offset: 0x04) (R/  32) Status Register -------- */
#define ACIFC_SR_OFFSET             0x04         /**< \brief (ACIFC_SR offset) Status Register */
#define ACIFC_SR_RESETVALUE         0x00000000   /**< \brief (ACIFC_SR reset_value) Status Register */

#define ACIFC_SR_ACCS0_Pos          0            /**< \brief (ACIFC_SR) AC0 Current Comparison Status */
#define ACIFC_SR_ACCS0              (0x1u << ACIFC_SR_ACCS0_Pos)
#define ACIFC_SR_ACRDY0_Pos         1            /**< \brief (ACIFC_SR) AC0 Ready */
#define ACIFC_SR_ACRDY0             (0x1u << ACIFC_SR_ACRDY0_Pos)
#define ACIFC_SR_ACCS1_Pos          2            /**< \brief (ACIFC_SR) AC1 Current Comparison Status */
#define ACIFC_SR_ACCS1              (0x1u << ACIFC_SR_ACCS1_Pos)
#define ACIFC_SR_ACRDY1_Pos         3            /**< \brief (ACIFC_SR) AC1 Ready */
#define ACIFC_SR_ACRDY1             (0x1u << ACIFC_SR_ACRDY1_Pos)
#define ACIFC_SR_ACCS2_Pos          4            /**< \brief (ACIFC_SR) AC2 Current Comparison Status */
#define ACIFC_SR_ACCS2              (0x1u << ACIFC_SR_ACCS2_Pos)
#define ACIFC_SR_ACRDY2_Pos         5            /**< \brief (ACIFC_SR) AC2 Ready */
#define ACIFC_SR_ACRDY2             (0x1u << ACIFC_SR_ACRDY2_Pos)
#define ACIFC_SR_ACCS3_Pos          6            /**< \brief (ACIFC_SR) AC3 Current Comparison Status */
#define ACIFC_SR_ACCS3              (0x1u << ACIFC_SR_ACCS3_Pos)
#define ACIFC_SR_ACRDY3_Pos         7            /**< \brief (ACIFC_SR) AC3 Ready */
#define ACIFC_SR_ACRDY3             (0x1u << ACIFC_SR_ACRDY3_Pos)
#define ACIFC_SR_ACCS4_Pos          8            /**< \brief (ACIFC_SR) AC4 Current Comparison Status */
#define ACIFC_SR_ACCS4              (0x1u << ACIFC_SR_ACCS4_Pos)
#define ACIFC_SR_ACRDY4_Pos         9            /**< \brief (ACIFC_SR) AC4 Ready */
#define ACIFC_SR_ACRDY4             (0x1u << ACIFC_SR_ACRDY4_Pos)
#define ACIFC_SR_ACCS5_Pos          10           /**< \brief (ACIFC_SR) AC5 Current Comparison Status */
#define ACIFC_SR_ACCS5              (0x1u << ACIFC_SR_ACCS5_Pos)
#define ACIFC_SR_ACRDY5_Pos         11           /**< \brief (ACIFC_SR) AC5 Ready */
#define ACIFC_SR_ACRDY5             (0x1u << ACIFC_SR_ACRDY5_Pos)
#define ACIFC_SR_ACCS6_Pos          12           /**< \brief (ACIFC_SR) AC6 Current Comparison Status */
#define ACIFC_SR_ACCS6              (0x1u << ACIFC_SR_ACCS6_Pos)
#define ACIFC_SR_ACRDY6_Pos         13           /**< \brief (ACIFC_SR) AC6 Ready */
#define ACIFC_SR_ACRDY6             (0x1u << ACIFC_SR_ACRDY6_Pos)
#define ACIFC_SR_ACCS7_Pos          14           /**< \brief (ACIFC_SR) AC7 Current Comparison Status */
#define ACIFC_SR_ACCS7              (0x1u << ACIFC_SR_ACCS7_Pos)
#define ACIFC_SR_ACRDY7_Pos         15           /**< \brief (ACIFC_SR) AC7 Ready */
#define ACIFC_SR_ACRDY7             (0x1u << ACIFC_SR_ACRDY7_Pos)
#define ACIFC_SR_WFCS0_Pos          24           /**< \brief (ACIFC_SR) Window0 Mode Current Status */
#define ACIFC_SR_WFCS0              (0x1u << ACIFC_SR_WFCS0_Pos)
#define ACIFC_SR_WFCS1_Pos          25           /**< \brief (ACIFC_SR) Window1 Mode Current Status */
#define ACIFC_SR_WFCS1              (0x1u << ACIFC_SR_WFCS1_Pos)
#define ACIFC_SR_WFCS2_Pos          26           /**< \brief (ACIFC_SR) Window2 Mode Current Status */
#define ACIFC_SR_WFCS2              (0x1u << ACIFC_SR_WFCS2_Pos)
#define ACIFC_SR_WFCS3_Pos          27           /**< \brief (ACIFC_SR) Window3 Mode Current Status */
#define ACIFC_SR_WFCS3              (0x1u << ACIFC_SR_WFCS3_Pos)
#define ACIFC_SR_MASK               0x0F00FFFFu  /**< \brief (ACIFC_SR) MASK Register */

/* -------- ACIFC_IER : (ACIFC Offset: 0x10) ( /W 32) Interrupt Enable Register -------- */
#define ACIFC_IER_OFFSET            0x10         /**< \brief (ACIFC_IER offset) Interrupt Enable Register */
#define ACIFC_IER_RESETVALUE        0x00000000   /**< \brief (ACIFC_IER reset_value) Interrupt Enable Register */

#define ACIFC_IER_ACINT0_Pos        0            /**< \brief (ACIFC_IER) AC0 Interrupt Enable */
#define ACIFC_IER_ACINT0            (0x1u << ACIFC_IER_ACINT0_Pos)
#define ACIFC_IER_SUTINT0_Pos       1            /**< \brief (ACIFC_IER) AC0 Startup Time Interrupt Enable */
#define ACIFC_IER_SUTINT0           (0x1u << ACIFC_IER_SUTINT0_Pos)
#define ACIFC_IER_ACINT1_Pos        2            /**< \brief (ACIFC_IER) AC1 Interrupt Enable */
#define ACIFC_IER_ACINT1            (0x1u << ACIFC_IER_ACINT1_Pos)
#define ACIFC_IER_SUTINT1_Pos       3            /**< \brief (ACIFC_IER) AC1 Startup Time Interrupt Enable */
#define ACIFC_IER_SUTINT1           (0x1u << ACIFC_IER_SUTINT1_Pos)
#define ACIFC_IER_ACINT2_Pos        4            /**< \brief (ACIFC_IER) AC2 Interrupt Enable */
#define ACIFC_IER_ACINT2            (0x1u << ACIFC_IER_ACINT2_Pos)
#define ACIFC_IER_SUTINT2_Pos       5            /**< \brief (ACIFC_IER) AC2 Startup Time Interrupt Enable */
#define ACIFC_IER_SUTINT2           (0x1u << ACIFC_IER_SUTINT2_Pos)
#define ACIFC_IER_ACINT3_Pos        6            /**< \brief (ACIFC_IER) AC3 Interrupt Enable */
#define ACIFC_IER_ACINT3            (0x1u << ACIFC_IER_ACINT3_Pos)
#define ACIFC_IER_SUTINT3_Pos       7            /**< \brief (ACIFC_IER) AC3 Startup Time Interrupt Enable */
#define ACIFC_IER_SUTINT3           (0x1u << ACIFC_IER_SUTINT3_Pos)
#define ACIFC_IER_ACINT4_Pos        8            /**< \brief (ACIFC_IER) AC4 Interrupt Enable */
#define ACIFC_IER_ACINT4            (0x1u << ACIFC_IER_ACINT4_Pos)
#define ACIFC_IER_SUTINT4_Pos       9            /**< \brief (ACIFC_IER) AC4 Startup Time Interrupt Enable */
#define ACIFC_IER_SUTINT4           (0x1u << ACIFC_IER_SUTINT4_Pos)
#define ACIFC_IER_ACINT5_Pos        10           /**< \brief (ACIFC_IER) AC5 Interrupt Enable */
#define ACIFC_IER_ACINT5            (0x1u << ACIFC_IER_ACINT5_Pos)
#define ACIFC_IER_SUTINT5_Pos       11           /**< \brief (ACIFC_IER) AC5 Startup Time Interrupt Enable */
#define ACIFC_IER_SUTINT5           (0x1u << ACIFC_IER_SUTINT5_Pos)
#define ACIFC_IER_ACINT6_Pos        12           /**< \brief (ACIFC_IER) AC6 Interrupt Enable */
#define ACIFC_IER_ACINT6            (0x1u << ACIFC_IER_ACINT6_Pos)
#define ACIFC_IER_SUTINT6_Pos       13           /**< \brief (ACIFC_IER) AC6 Startup Time Interrupt Enable */
#define ACIFC_IER_SUTINT6           (0x1u << ACIFC_IER_SUTINT6_Pos)
#define ACIFC_IER_ACINT7_Pos        14           /**< \brief (ACIFC_IER) AC7 Interrupt Enable */
#define ACIFC_IER_ACINT7            (0x1u << ACIFC_IER_ACINT7_Pos)
#define ACIFC_IER_SUTINT7_Pos       15           /**< \brief (ACIFC_IER) AC7 Startup Time Interrupt Enable */
#define ACIFC_IER_SUTINT7           (0x1u << ACIFC_IER_SUTINT7_Pos)
#define ACIFC_IER_WFINT0_Pos        24           /**< \brief (ACIFC_IER) Window0 Mode Interrupt Enable */
#define ACIFC_IER_WFINT0            (0x1u << ACIFC_IER_WFINT0_Pos)
#define ACIFC_IER_WFINT1_Pos        25           /**< \brief (ACIFC_IER) Window1 Mode Interrupt Enable */
#define ACIFC_IER_WFINT1            (0x1u << ACIFC_IER_WFINT1_Pos)
#define ACIFC_IER_WFINT2_Pos        26           /**< \brief (ACIFC_IER) Window2 Mode Interrupt Enable */
#define ACIFC_IER_WFINT2            (0x1u << ACIFC_IER_WFINT2_Pos)
#define ACIFC_IER_WFINT3_Pos        27           /**< \brief (ACIFC_IER) Window3 Mode Interrupt Enable */
#define ACIFC_IER_WFINT3            (0x1u << ACIFC_IER_WFINT3_Pos)
#define ACIFC_IER_MASK              0x0F00FFFFu  /**< \brief (ACIFC_IER) MASK Register */

/* -------- ACIFC_IDR : (ACIFC Offset: 0x14) ( /W 32) Interrupt Disable Register -------- */
#define ACIFC_IDR_OFFSET            0x14         /**< \brief (ACIFC_IDR offset) Interrupt Disable Register */
#define ACIFC_IDR_RESETVALUE        0x00000000   /**< \brief (ACIFC_IDR reset_value) Interrupt Disable Register */

#define ACIFC_IDR_ACINT0_Pos        0            /**< \brief (ACIFC_IDR) AC0 Interrupt Disable */
#define ACIFC_IDR_ACINT0            (0x1u << ACIFC_IDR_ACINT0_Pos)
#define ACIFC_IDR_SUTINT0_Pos       1            /**< \brief (ACIFC_IDR) AC0 Startup Time Interrupt Disable */
#define ACIFC_IDR_SUTINT0           (0x1u << ACIFC_IDR_SUTINT0_Pos)
#define ACIFC_IDR_ACINT1_Pos        2            /**< \brief (ACIFC_IDR) AC1 Interrupt Disable */
#define ACIFC_IDR_ACINT1            (0x1u << ACIFC_IDR_ACINT1_Pos)
#define ACIFC_IDR_SUTINT1_Pos       3            /**< \brief (ACIFC_IDR) AC1 Startup Time Interrupt Disable */
#define ACIFC_IDR_SUTINT1           (0x1u << ACIFC_IDR_SUTINT1_Pos)
#define ACIFC_IDR_ACINT2_Pos        4            /**< \brief (ACIFC_IDR) AC2 Interrupt Disable */
#define ACIFC_IDR_ACINT2            (0x1u << ACIFC_IDR_ACINT2_Pos)
#define ACIFC_IDR_SUTINT2_Pos       5            /**< \brief (ACIFC_IDR) AC2 Startup Time Interrupt Disable */
#define ACIFC_IDR_SUTINT2           (0x1u << ACIFC_IDR_SUTINT2_Pos)
#define ACIFC_IDR_ACINT3_Pos        6            /**< \brief (ACIFC_IDR) AC3 Interrupt Disable */
#define ACIFC_IDR_ACINT3            (0x1u << ACIFC_IDR_ACINT3_Pos)
#define ACIFC_IDR_SUTINT3_Pos       7            /**< \brief (ACIFC_IDR) AC3 Startup Time Interrupt Disable */
#define ACIFC_IDR_SUTINT3           (0x1u << ACIFC_IDR_SUTINT3_Pos)
#define ACIFC_IDR_ACINT4_Pos        8            /**< \brief (ACIFC_IDR) AC4 Interrupt Disable */
#define ACIFC_IDR_ACINT4            (0x1u << ACIFC_IDR_ACINT4_Pos)
#define ACIFC_IDR_SUTINT4_Pos       9            /**< \brief (ACIFC_IDR) AC4 Startup Time Interrupt Disable */
#define ACIFC_IDR_SUTINT4           (0x1u << ACIFC_IDR_SUTINT4_Pos)
#define ACIFC_IDR_ACINT5_Pos        10           /**< \brief (ACIFC_IDR) AC5 Interrupt Disable */
#define ACIFC_IDR_ACINT5            (0x1u << ACIFC_IDR_ACINT5_Pos)
#define ACIFC_IDR_SUTINT5_Pos       11           /**< \brief (ACIFC_IDR) AC5 Startup Time Interrupt Disable */
#define ACIFC_IDR_SUTINT5           (0x1u << ACIFC_IDR_SUTINT5_Pos)
#define ACIFC_IDR_ACINT6_Pos        12           /**< \brief (ACIFC_IDR) AC6 Interrupt Disable */
#define ACIFC_IDR_ACINT6            (0x1u << ACIFC_IDR_ACINT6_Pos)
#define ACIFC_IDR_SUTINT6_Pos       13           /**< \brief (ACIFC_IDR) AC6 Startup Time Interrupt Disable */
#define ACIFC_IDR_SUTINT6           (0x1u << ACIFC_IDR_SUTINT6_Pos)
#define ACIFC_IDR_ACINT7_Pos        14           /**< \brief (ACIFC_IDR) AC7 Interrupt Disable */
#define ACIFC_IDR_ACINT7            (0x1u << ACIFC_IDR_ACINT7_Pos)
#define ACIFC_IDR_SUTINT7_Pos       15           /**< \brief (ACIFC_IDR) AC7 Startup Time Interrupt Disable */
#define ACIFC_IDR_SUTINT7           (0x1u << ACIFC_IDR_SUTINT7_Pos)
#define ACIFC_IDR_WFINT0_Pos        24           /**< \brief (ACIFC_IDR) Window0 Mode Interrupt Disable */
#define ACIFC_IDR_WFINT0            (0x1u << ACIFC_IDR_WFINT0_Pos)
#define ACIFC_IDR_WFINT1_Pos        25           /**< \brief (ACIFC_IDR) Window1 Mode Interrupt Disable */
#define ACIFC_IDR_WFINT1            (0x1u << ACIFC_IDR_WFINT1_Pos)
#define ACIFC_IDR_WFINT2_Pos        26           /**< \brief (ACIFC_IDR) Window2 Mode Interrupt Disable */
#define ACIFC_IDR_WFINT2            (0x1u << ACIFC_IDR_WFINT2_Pos)
#define ACIFC_IDR_WFINT3_Pos        27           /**< \brief (ACIFC_IDR) Window3 Mode Interrupt Disable */
#define ACIFC_IDR_WFINT3            (0x1u << ACIFC_IDR_WFINT3_Pos)
#define ACIFC_IDR_MASK              0x0F00FFFFu  /**< \brief (ACIFC_IDR) MASK Register */

/* -------- ACIFC_IMR : (ACIFC Offset: 0x18) (R/  32) Interrupt Mask Register -------- */
#define ACIFC_IMR_OFFSET            0x18         /**< \brief (ACIFC_IMR offset) Interrupt Mask Register */
#define ACIFC_IMR_RESETVALUE        0x00000000   /**< \brief (ACIFC_IMR reset_value) Interrupt Mask Register */

#define ACIFC_IMR_ACINT0_Pos        0            /**< \brief (ACIFC_IMR) AC0 Interrupt Mask */
#define ACIFC_IMR_ACINT0            (0x1u << ACIFC_IMR_ACINT0_Pos)
#define ACIFC_IMR_SUTINT0_Pos       1            /**< \brief (ACIFC_IMR) AC0 Startup Time Interrupt Mask */
#define ACIFC_IMR_SUTINT0           (0x1u << ACIFC_IMR_SUTINT0_Pos)
#define ACIFC_IMR_ACINT1_Pos        2            /**< \brief (ACIFC_IMR) AC1 Interrupt Mask */
#define ACIFC_IMR_ACINT1            (0x1u << ACIFC_IMR_ACINT1_Pos)
#define ACIFC_IMR_SUTINT1_Pos       3            /**< \brief (ACIFC_IMR) AC1 Startup Time Interrupt Mask */
#define ACIFC_IMR_SUTINT1           (0x1u << ACIFC_IMR_SUTINT1_Pos)
#define ACIFC_IMR_ACINT2_Pos        4            /**< \brief (ACIFC_IMR) AC2 Interrupt Mask */
#define ACIFC_IMR_ACINT2            (0x1u << ACIFC_IMR_ACINT2_Pos)
#define ACIFC_IMR_SUTINT2_Pos       5            /**< \brief (ACIFC_IMR) AC2 Startup Time Interrupt Mask */
#define ACIFC_IMR_SUTINT2           (0x1u << ACIFC_IMR_SUTINT2_Pos)
#define ACIFC_IMR_ACINT3_Pos        6            /**< \brief (ACIFC_IMR) AC3 Interrupt Mask */
#define ACIFC_IMR_ACINT3            (0x1u << ACIFC_IMR_ACINT3_Pos)
#define ACIFC_IMR_SUTINT3_Pos       7            /**< \brief (ACIFC_IMR) AC3 Startup Time Interrupt Mask */
#define ACIFC_IMR_SUTINT3           (0x1u << ACIFC_IMR_SUTINT3_Pos)
#define ACIFC_IMR_ACINT4_Pos        8            /**< \brief (ACIFC_IMR) AC4 Interrupt Mask */
#define ACIFC_IMR_ACINT4            (0x1u << ACIFC_IMR_ACINT4_Pos)
#define ACIFC_IMR_SUTINT4_Pos       9            /**< \brief (ACIFC_IMR) AC4 Startup Time Interrupt Mask */
#define ACIFC_IMR_SUTINT4           (0x1u << ACIFC_IMR_SUTINT4_Pos)
#define ACIFC_IMR_ACINT5_Pos        10           /**< \brief (ACIFC_IMR) AC5 Interrupt Mask */
#define ACIFC_IMR_ACINT5            (0x1u << ACIFC_IMR_ACINT5_Pos)
#define ACIFC_IMR_SUTINT5_Pos       11           /**< \brief (ACIFC_IMR) AC5 Startup Time Interrupt Mask */
#define ACIFC_IMR_SUTINT5           (0x1u << ACIFC_IMR_SUTINT5_Pos)
#define ACIFC_IMR_ACINT6_Pos        12           /**< \brief (ACIFC_IMR) AC6 Interrupt Mask */
#define ACIFC_IMR_ACINT6            (0x1u << ACIFC_IMR_ACINT6_Pos)
#define ACIFC_IMR_SUTINT6_Pos       13           /**< \brief (ACIFC_IMR) AC6 Startup Time Interrupt Mask */
#define ACIFC_IMR_SUTINT6           (0x1u << ACIFC_IMR_SUTINT6_Pos)
#define ACIFC_IMR_ACINT7_Pos        14           /**< \brief (ACIFC_IMR) AC7 Interrupt Mask */
#define ACIFC_IMR_ACINT7            (0x1u << ACIFC_IMR_ACINT7_Pos)
#define ACIFC_IMR_SUTINT7_Pos       15           /**< \brief (ACIFC_IMR) AC7 Startup Time Interrupt Mask */
#define ACIFC_IMR_SUTINT7           (0x1u << ACIFC_IMR_SUTINT7_Pos)
#define ACIFC_IMR_WFINT0_Pos        24           /**< \brief (ACIFC_IMR) Window0 Mode Interrupt Mask */
#define ACIFC_IMR_WFINT0            (0x1u << ACIFC_IMR_WFINT0_Pos)
#define ACIFC_IMR_WFINT1_Pos        25           /**< \brief (ACIFC_IMR) Window1 Mode Interrupt Mask */
#define ACIFC_IMR_WFINT1            (0x1u << ACIFC_IMR_WFINT1_Pos)
#define ACIFC_IMR_WFINT2_Pos        26           /**< \brief (ACIFC_IMR) Window2 Mode Interrupt Mask */
#define ACIFC_IMR_WFINT2            (0x1u << ACIFC_IMR_WFINT2_Pos)
#define ACIFC_IMR_WFINT3_Pos        27           /**< \brief (ACIFC_IMR) Window3 Mode Interrupt Mask */
#define ACIFC_IMR_WFINT3            (0x1u << ACIFC_IMR_WFINT3_Pos)
#define ACIFC_IMR_MASK              0x0F00FFFFu  /**< \brief (ACIFC_IMR) MASK Register */

/* -------- ACIFC_ISR : (ACIFC Offset: 0x1C) (R/  32) Interrupt Status Register -------- */
#define ACIFC_ISR_OFFSET            0x1C         /**< \brief (ACIFC_ISR offset) Interrupt Status Register */
#define ACIFC_ISR_RESETVALUE        0x00000000   /**< \brief (ACIFC_ISR reset_value) Interrupt Status Register */

#define ACIFC_ISR_ACINT0_Pos        0            /**< \brief (ACIFC_ISR) AC0 Interrupt Status */
#define ACIFC_ISR_ACINT0            (0x1u << ACIFC_ISR_ACINT0_Pos)
#define ACIFC_ISR_SUTINT0_Pos       1            /**< \brief (ACIFC_ISR) AC0 Startup Time Interrupt Status */
#define ACIFC_ISR_SUTINT0           (0x1u << ACIFC_ISR_SUTINT0_Pos)
#define ACIFC_ISR_ACINT1_Pos        2            /**< \brief (ACIFC_ISR) AC1 Interrupt Status */
#define ACIFC_ISR_ACINT1            (0x1u << ACIFC_ISR_ACINT1_Pos)
#define ACIFC_ISR_SUTINT1_Pos       3            /**< \brief (ACIFC_ISR) AC1 Startup Time Interrupt Status */
#define ACIFC_ISR_SUTINT1           (0x1u << ACIFC_ISR_SUTINT1_Pos)
#define ACIFC_ISR_ACINT2_Pos        4            /**< \brief (ACIFC_ISR) AC2 Interrupt Status */
#define ACIFC_ISR_ACINT2            (0x1u << ACIFC_ISR_ACINT2_Pos)
#define ACIFC_ISR_SUTINT2_Pos       5            /**< \brief (ACIFC_ISR) AC2 Startup Time Interrupt Status */
#define ACIFC_ISR_SUTINT2           (0x1u << ACIFC_ISR_SUTINT2_Pos)
#define ACIFC_ISR_ACINT3_Pos        6            /**< \brief (ACIFC_ISR) AC3 Interrupt Status */
#define ACIFC_ISR_ACINT3            (0x1u << ACIFC_ISR_ACINT3_Pos)
#define ACIFC_ISR_SUTINT3_Pos       7            /**< \brief (ACIFC_ISR) AC3 Startup Time Interrupt Status */
#define ACIFC_ISR_SUTINT3           (0x1u << ACIFC_ISR_SUTINT3_Pos)
#define ACIFC_ISR_ACINT4_Pos        8            /**< \brief (ACIFC_ISR) AC4 Interrupt Status */
#define ACIFC_ISR_ACINT4            (0x1u << ACIFC_ISR_ACINT4_Pos)
#define ACIFC_ISR_SUTINT4_Pos       9            /**< \brief (ACIFC_ISR) AC4 Startup Time Interrupt Status */
#define ACIFC_ISR_SUTINT4           (0x1u << ACIFC_ISR_SUTINT4_Pos)
#define ACIFC_ISR_ACINT5_Pos        10           /**< \brief (ACIFC_ISR) AC5 Interrupt Status */
#define ACIFC_ISR_ACINT5            (0x1u << ACIFC_ISR_ACINT5_Pos)
#define ACIFC_ISR_SUTINT5_Pos       11           /**< \brief (ACIFC_ISR) AC5 Startup Time Interrupt Status */
#define ACIFC_ISR_SUTINT5           (0x1u << ACIFC_ISR_SUTINT5_Pos)
#define ACIFC_ISR_ACINT6_Pos        12           /**< \brief (ACIFC_ISR) AC6 Interrupt Status */
#define ACIFC_ISR_ACINT6            (0x1u << ACIFC_ISR_ACINT6_Pos)
#define ACIFC_ISR_SUTINT6_Pos       13           /**< \brief (ACIFC_ISR) AC6 Startup Time Interrupt Status */
#define ACIFC_ISR_SUTINT6           (0x1u << ACIFC_ISR_SUTINT6_Pos)
#define ACIFC_ISR_ACINT7_Pos        14           /**< \brief (ACIFC_ISR) AC7 Interrupt Status */
#define ACIFC_ISR_ACINT7            (0x1u << ACIFC_ISR_ACINT7_Pos)
#define ACIFC_ISR_SUTINT7_Pos       15           /**< \brief (ACIFC_ISR) AC7 Startup Time Interrupt Status */
#define ACIFC_ISR_SUTINT7           (0x1u << ACIFC_ISR_SUTINT7_Pos)
#define ACIFC_ISR_WFINT0_Pos        24           /**< \brief (ACIFC_ISR) Window0 Mode Interrupt Status */
#define ACIFC_ISR_WFINT0            (0x1u << ACIFC_ISR_WFINT0_Pos)
#define ACIFC_ISR_WFINT1_Pos        25           /**< \brief (ACIFC_ISR) Window1 Mode Interrupt Status */
#define ACIFC_ISR_WFINT1            (0x1u << ACIFC_ISR_WFINT1_Pos)
#define ACIFC_ISR_WFINT2_Pos        26           /**< \brief (ACIFC_ISR) Window2 Mode Interrupt Status */
#define ACIFC_ISR_WFINT2            (0x1u << ACIFC_ISR_WFINT2_Pos)
#define ACIFC_ISR_WFINT3_Pos        27           /**< \brief (ACIFC_ISR) Window3 Mode Interrupt Status */
#define ACIFC_ISR_WFINT3            (0x1u << ACIFC_ISR_WFINT3_Pos)
#define ACIFC_ISR_MASK              0x0F00FFFFu  /**< \brief (ACIFC_ISR) MASK Register */

/* -------- ACIFC_ICR : (ACIFC Offset: 0x20) ( /W 32) Interrupt Status Clear Register -------- */
#define ACIFC_ICR_OFFSET            0x20         /**< \brief (ACIFC_ICR offset) Interrupt Status Clear Register */
#define ACIFC_ICR_RESETVALUE        0x00000000   /**< \brief (ACIFC_ICR reset_value) Interrupt Status Clear Register */

#define ACIFC_ICR_ACINT0_Pos        0            /**< \brief (ACIFC_ICR) AC0 Interrupt Status Clear */
#define ACIFC_ICR_ACINT0            (0x1u << ACIFC_ICR_ACINT0_Pos)
#define ACIFC_ICR_SUTINT0_Pos       1            /**< \brief (ACIFC_ICR) AC0 Startup Time Interrupt Status Clear */
#define ACIFC_ICR_SUTINT0           (0x1u << ACIFC_ICR_SUTINT0_Pos)
#define ACIFC_ICR_ACINT1_Pos        2            /**< \brief (ACIFC_ICR) AC1 Interrupt Status Clear */
#define ACIFC_ICR_ACINT1            (0x1u << ACIFC_ICR_ACINT1_Pos)
#define ACIFC_ICR_SUTINT1_Pos       3            /**< \brief (ACIFC_ICR) AC1 Startup Time Interrupt Status Clear */
#define ACIFC_ICR_SUTINT1           (0x1u << ACIFC_ICR_SUTINT1_Pos)
#define ACIFC_ICR_ACINT2_Pos        4            /**< \brief (ACIFC_ICR) AC2 Interrupt Status Clear */
#define ACIFC_ICR_ACINT2            (0x1u << ACIFC_ICR_ACINT2_Pos)
#define ACIFC_ICR_SUTINT2_Pos       5            /**< \brief (ACIFC_ICR) AC2 Startup Time Interrupt Status Clear */
#define ACIFC_ICR_SUTINT2           (0x1u << ACIFC_ICR_SUTINT2_Pos)
#define ACIFC_ICR_ACINT3_Pos        6            /**< \brief (ACIFC_ICR) AC3 Interrupt Status Clear */
#define ACIFC_ICR_ACINT3            (0x1u << ACIFC_ICR_ACINT3_Pos)
#define ACIFC_ICR_SUTINT3_Pos       7            /**< \brief (ACIFC_ICR) AC3 Startup Time Interrupt Status Clear */
#define ACIFC_ICR_SUTINT3           (0x1u << ACIFC_ICR_SUTINT3_Pos)
#define ACIFC_ICR_ACINT4_Pos        8            /**< \brief (ACIFC_ICR) AC4 Interrupt Status Clear */
#define ACIFC_ICR_ACINT4            (0x1u << ACIFC_ICR_ACINT4_Pos)
#define ACIFC_ICR_SUTINT4_Pos       9            /**< \brief (ACIFC_ICR) AC4 Startup Time Interrupt Status Clear */
#define ACIFC_ICR_SUTINT4           (0x1u << ACIFC_ICR_SUTINT4_Pos)
#define ACIFC_ICR_ACINT5_Pos        10           /**< \brief (ACIFC_ICR) AC5 Interrupt Status Clear */
#define ACIFC_ICR_ACINT5            (0x1u << ACIFC_ICR_ACINT5_Pos)
#define ACIFC_ICR_SUTINT5_Pos       11           /**< \brief (ACIFC_ICR) AC5 Startup Time Interrupt Status Clear */
#define ACIFC_ICR_SUTINT5           (0x1u << ACIFC_ICR_SUTINT5_Pos)
#define ACIFC_ICR_ACINT6_Pos        12           /**< \brief (ACIFC_ICR) AC6 Interrupt Status Clear */
#define ACIFC_ICR_ACINT6            (0x1u << ACIFC_ICR_ACINT6_Pos)
#define ACIFC_ICR_SUTINT6_Pos       13           /**< \brief (ACIFC_ICR) AC6 Startup Time Interrupt Status Clear */
#define ACIFC_ICR_SUTINT6           (0x1u << ACIFC_ICR_SUTINT6_Pos)
#define ACIFC_ICR_ACINT7_Pos        14           /**< \brief (ACIFC_ICR) AC7 Interrupt Status Clear */
#define ACIFC_ICR_ACINT7            (0x1u << ACIFC_ICR_ACINT7_Pos)
#define ACIFC_ICR_SUTINT7_Pos       15           /**< \brief (ACIFC_ICR) AC7 Startup Time Interrupt Status Clear */
#define ACIFC_ICR_SUTINT7           (0x1u << ACIFC_ICR_SUTINT7_Pos)
#define ACIFC_ICR_WFINT0_Pos        24           /**< \brief (ACIFC_ICR) Window0 Mode Interrupt Status Clear */
#define ACIFC_ICR_WFINT0            (0x1u << ACIFC_ICR_WFINT0_Pos)
#define ACIFC_ICR_WFINT1_Pos        25           /**< \brief (ACIFC_ICR) Window1 Mode Interrupt Status Clear */
#define ACIFC_ICR_WFINT1            (0x1u << ACIFC_ICR_WFINT1_Pos)
#define ACIFC_ICR_WFINT2_Pos        26           /**< \brief (ACIFC_ICR) Window2 Mode Interrupt Status Clear */
#define ACIFC_ICR_WFINT2            (0x1u << ACIFC_ICR_WFINT2_Pos)
#define ACIFC_ICR_WFINT3_Pos        27           /**< \brief (ACIFC_ICR) Window3 Mode Interrupt Status Clear */
#define ACIFC_ICR_WFINT3            (0x1u << ACIFC_ICR_WFINT3_Pos)
#define ACIFC_ICR_MASK              0x0F00FFFFu  /**< \brief (ACIFC_ICR) MASK Register */

/* -------- ACIFC_TR : (ACIFC Offset: 0x24) (R/W 32) Test Register -------- */
#define ACIFC_TR_OFFSET             0x24         /**< \brief (ACIFC_TR offset) Test Register */
#define ACIFC_TR_RESETVALUE         0x00000000   /**< \brief (ACIFC_TR reset_value) Test Register */

#define ACIFC_TR_ACTEST0_Pos        0            /**< \brief (ACIFC_TR) AC0 Output Override Value */
#define ACIFC_TR_ACTEST0            (0x1u << ACIFC_TR_ACTEST0_Pos)
#define ACIFC_TR_ACTEST1_Pos        1            /**< \brief (ACIFC_TR) AC1 Output Override Value */
#define ACIFC_TR_ACTEST1            (0x1u << ACIFC_TR_ACTEST1_Pos)
#define ACIFC_TR_ACTEST2_Pos        2            /**< \brief (ACIFC_TR) AC2 Output Override Value */
#define ACIFC_TR_ACTEST2            (0x1u << ACIFC_TR_ACTEST2_Pos)
#define ACIFC_TR_ACTEST3_Pos        3            /**< \brief (ACIFC_TR) AC3 Output Override Value */
#define ACIFC_TR_ACTEST3            (0x1u << ACIFC_TR_ACTEST3_Pos)
#define ACIFC_TR_ACTEST4_Pos        4            /**< \brief (ACIFC_TR) AC4 Output Override Value */
#define ACIFC_TR_ACTEST4            (0x1u << ACIFC_TR_ACTEST4_Pos)
#define ACIFC_TR_ACTEST5_Pos        5            /**< \brief (ACIFC_TR) AC5 Output Override Value */
#define ACIFC_TR_ACTEST5            (0x1u << ACIFC_TR_ACTEST5_Pos)
#define ACIFC_TR_ACTEST6_Pos        6            /**< \brief (ACIFC_TR) AC6 Output Override Value */
#define ACIFC_TR_ACTEST6            (0x1u << ACIFC_TR_ACTEST6_Pos)
#define ACIFC_TR_ACTEST7_Pos        7            /**< \brief (ACIFC_TR) AC7 Output Override Value */
#define ACIFC_TR_ACTEST7            (0x1u << ACIFC_TR_ACTEST7_Pos)
#define ACIFC_TR_MASK               0x000000FFu  /**< \brief (ACIFC_TR) MASK Register */

/* -------- ACIFC_PARAMETER : (ACIFC Offset: 0x30) (R/  32) Parameter Register -------- */
#define ACIFC_PARAMETER_OFFSET      0x30         /**< \brief (ACIFC_PARAMETER offset) Parameter Register */

#define ACIFC_PARAMETER_ACIMPL0_Pos 0            /**< \brief (ACIFC_PARAMETER) Analog Comparator 0 Implemented */
#define ACIFC_PARAMETER_ACIMPL0     (0x1u << ACIFC_PARAMETER_ACIMPL0_Pos)
#define ACIFC_PARAMETER_ACIMPL1_Pos 1            /**< \brief (ACIFC_PARAMETER) Analog Comparator 1 Implemented */
#define ACIFC_PARAMETER_ACIMPL1     (0x1u << ACIFC_PARAMETER_ACIMPL1_Pos)
#define ACIFC_PARAMETER_ACIMPL2_Pos 2            /**< \brief (ACIFC_PARAMETER) Analog Comparator 2 Implemented */
#define ACIFC_PARAMETER_ACIMPL2     (0x1u << ACIFC_PARAMETER_ACIMPL2_Pos)
#define ACIFC_PARAMETER_ACIMPL3_Pos 3            /**< \brief (ACIFC_PARAMETER) Analog Comparator 3 Implemented */
#define ACIFC_PARAMETER_ACIMPL3     (0x1u << ACIFC_PARAMETER_ACIMPL3_Pos)
#define ACIFC_PARAMETER_ACIMPL4_Pos 4            /**< \brief (ACIFC_PARAMETER) Analog Comparator 4 Implemented */
#define ACIFC_PARAMETER_ACIMPL4     (0x1u << ACIFC_PARAMETER_ACIMPL4_Pos)
#define ACIFC_PARAMETER_ACIMPL5_Pos 5            /**< \brief (ACIFC_PARAMETER) Analog Comparator 5 Implemented */
#define ACIFC_PARAMETER_ACIMPL5     (0x1u << ACIFC_PARAMETER_ACIMPL5_Pos)
#define ACIFC_PARAMETER_ACIMPL6_Pos 6            /**< \brief (ACIFC_PARAMETER) Analog Comparator 6 Implemented */
#define ACIFC_PARAMETER_ACIMPL6     (0x1u << ACIFC_PARAMETER_ACIMPL6_Pos)
#define ACIFC_PARAMETER_ACIMPL7_Pos 7            /**< \brief (ACIFC_PARAMETER) Analog Comparator 7 Implemented */
#define ACIFC_PARAMETER_ACIMPL7     (0x1u << ACIFC_PARAMETER_ACIMPL7_Pos)
#define ACIFC_PARAMETER_WIMPL0_Pos  16           /**< \brief (ACIFC_PARAMETER) Window0 Mode  Implemented */
#define ACIFC_PARAMETER_WIMPL0      (0x1u << ACIFC_PARAMETER_WIMPL0_Pos)
#define ACIFC_PARAMETER_WIMPL1_Pos  17           /**< \brief (ACIFC_PARAMETER) Window1 Mode  Implemented */
#define ACIFC_PARAMETER_WIMPL1      (0x1u << ACIFC_PARAMETER_WIMPL1_Pos)
#define ACIFC_PARAMETER_WIMPL2_Pos  18           /**< \brief (ACIFC_PARAMETER) Window2 Mode  Implemented */
#define ACIFC_PARAMETER_WIMPL2      (0x1u << ACIFC_PARAMETER_WIMPL2_Pos)
#define ACIFC_PARAMETER_WIMPL3_Pos  19           /**< \brief (ACIFC_PARAMETER) Window3 Mode  Implemented */
#define ACIFC_PARAMETER_WIMPL3      (0x1u << ACIFC_PARAMETER_WIMPL3_Pos)
#define ACIFC_PARAMETER_MASK        0x000F00FFu  /**< \brief (ACIFC_PARAMETER) MASK Register */

/* -------- ACIFC_VERSION : (ACIFC Offset: 0x34) (R/  32) Version Register -------- */
#define ACIFC_VERSION_OFFSET        0x34         /**< \brief (ACIFC_VERSION offset) Version Register */
#define ACIFC_VERSION_RESETVALUE    0x00000101   /**< \brief (ACIFC_VERSION reset_value) Version Register */

#define ACIFC_VERSION_VERSION_Pos   0            /**< \brief (ACIFC_VERSION) Version Number */
#define ACIFC_VERSION_VERSION_Msk   (0xFFFu << ACIFC_VERSION_VERSION_Pos)
#define ACIFC_VERSION_VERSION(value) ((ACIFC_VERSION_VERSION_Msk & ((value) << ACIFC_VERSION_VERSION_Pos)))
#define ACIFC_VERSION_VARIANT_Pos   16           /**< \brief (ACIFC_VERSION) Variant Number */
#define ACIFC_VERSION_VARIANT_Msk   (0xFu << ACIFC_VERSION_VARIANT_Pos)
#define ACIFC_VERSION_VARIANT(value) ((ACIFC_VERSION_VARIANT_Msk & ((value) << ACIFC_VERSION_VARIANT_Pos)))
#define ACIFC_VERSION_MASK          0x000F0FFFu  /**< \brief (ACIFC_VERSION) MASK Register */

/* -------- ACIFC_CONFW : (ACIFC Offset: 0x80) (R/W 32) CONFW Window configuration Register -------- */
#define ACIFC_CONFW_OFFSET          0x80         /**< \brief (ACIFC_CONFW offset) Window configuration Register */
#define ACIFC_CONFW_RESETVALUE      0x00000000   /**< \brief (ACIFC_CONFW reset_value) Window configuration Register */

#define ACIFC_CONFW_WIS_Pos         0            /**< \brief (ACIFC_CONFW) Window Mode Interrupt Settings */
#define ACIFC_CONFW_WIS_Msk         (0x7u << ACIFC_CONFW_WIS_Pos)
#define ACIFC_CONFW_WIS(value)      ((ACIFC_CONFW_WIS_Msk & ((value) << ACIFC_CONFW_WIS_Pos)))
#define ACIFC_CONFW_WEVSRC_Pos      8            /**< \brief (ACIFC_CONFW) Peripheral Event Sourse Selection for Window Mode */
#define ACIFC_CONFW_WEVSRC_Msk      (0x7u << ACIFC_CONFW_WEVSRC_Pos)
#define ACIFC_CONFW_WEVSRC(value)   ((ACIFC_CONFW_WEVSRC_Msk & ((value) << ACIFC_CONFW_WEVSRC_Pos)))
#define ACIFC_CONFW_WEVEN_Pos       11           /**< \brief (ACIFC_CONFW) Window Peripheral Event Enable */
#define ACIFC_CONFW_WEVEN           (0x1u << ACIFC_CONFW_WEVEN_Pos)
#define ACIFC_CONFW_WFEN_Pos        16           /**< \brief (ACIFC_CONFW) Window Mode Enable */
#define ACIFC_CONFW_WFEN            (0x1u << ACIFC_CONFW_WFEN_Pos)
#define ACIFC_CONFW_MASK            0x00010F07u  /**< \brief (ACIFC_CONFW) MASK Register */

/* -------- ACIFC_CONF : (ACIFC Offset: 0xD0) (R/W 32) CONF AC Configuration Register -------- */
#define ACIFC_CONF_OFFSET           0xD0         /**< \brief (ACIFC_CONF offset) AC Configuration Register */
#define ACIFC_CONF_RESETVALUE       0x00000000   /**< \brief (ACIFC_CONF reset_value) AC Configuration Register */

#define ACIFC_CONF_IS_Pos           0            /**< \brief (ACIFC_CONF) Interupt Settings */
#define ACIFC_CONF_IS_Msk           (0x3u << ACIFC_CONF_IS_Pos)
#define ACIFC_CONF_IS(value)        ((ACIFC_CONF_IS_Msk & ((value) << ACIFC_CONF_IS_Pos)))
#define ACIFC_CONF_MODE_Pos         4            /**< \brief (ACIFC_CONF) Analog Comparator Mode */
#define ACIFC_CONF_MODE_Msk         (0x3u << ACIFC_CONF_MODE_Pos)
#define ACIFC_CONF_MODE(value)      ((ACIFC_CONF_MODE_Msk & ((value) << ACIFC_CONF_MODE_Pos)))
#define ACIFC_CONF_INSELN_Pos       8            /**< \brief (ACIFC_CONF) Negative Input Select */
#define ACIFC_CONF_INSELN_Msk       (0x3u << ACIFC_CONF_INSELN_Pos)
#define ACIFC_CONF_INSELN(value)    ((ACIFC_CONF_INSELN_Msk & ((value) << ACIFC_CONF_INSELN_Pos)))
#define ACIFC_CONF_EVENN_Pos        16           /**< \brief (ACIFC_CONF) Peripheral Event Enable Negative */
#define ACIFC_CONF_EVENN            (0x1u << ACIFC_CONF_EVENN_Pos)
#define ACIFC_CONF_EVENP_Pos        17           /**< \brief (ACIFC_CONF) Peripheral Event Enable Positive */
#define ACIFC_CONF_EVENP            (0x1u << ACIFC_CONF_EVENP_Pos)
#define ACIFC_CONF_HYS_Pos          24           /**< \brief (ACIFC_CONF) Hysteresis Voltage Value */
#define ACIFC_CONF_HYS_Msk          (0x3u << ACIFC_CONF_HYS_Pos)
#define ACIFC_CONF_HYS(value)       ((ACIFC_CONF_HYS_Msk & ((value) << ACIFC_CONF_HYS_Pos)))
#define ACIFC_CONF_FAST_Pos         26           /**< \brief (ACIFC_CONF) Fast Mode Enable */
#define ACIFC_CONF_FAST             (0x1u << ACIFC_CONF_FAST_Pos)
#define ACIFC_CONF_ALWAYSON_Pos     27           /**< \brief (ACIFC_CONF) Always On */
#define ACIFC_CONF_ALWAYSON         (0x1u << ACIFC_CONF_ALWAYSON_Pos)
#define ACIFC_CONF_MASK             0x0F030333u  /**< \brief (ACIFC_CONF) MASK Register */

/** \brief AcifcConf hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
  RwReg   ACIFC_CONF;         /**< \brief (ACIFC Offset: 0x00) AC Configuration Register */
} AcifcConf;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/** \brief AcifcConfw hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
  RwReg   ACIFC_CONFW;        /**< \brief (ACIFC Offset: 0x00) Window configuration Register */
} AcifcConfw;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/** \brief ACIFC hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
  RwReg   ACIFC_CTRL;         /**< \brief (ACIFC Offset: 0x00) Control Register */
  RoReg   ACIFC_SR;           /**< \brief (ACIFC Offset: 0x04) Status Register */
  RoReg8  Reserved1[0x8];
  WoReg   ACIFC_IER;          /**< \brief (ACIFC Offset: 0x10) Interrupt Enable Register */
  WoReg   ACIFC_IDR;          /**< \brief (ACIFC Offset: 0x14) Interrupt Disable Register */
  RoReg   ACIFC_IMR;          /**< \brief (ACIFC Offset: 0x18) Interrupt Mask Register */
  RoReg   ACIFC_ISR;          /**< \brief (ACIFC Offset: 0x1C) Interrupt Status Register */
  WoReg   ACIFC_ICR;          /**< \brief (ACIFC Offset: 0x20) Interrupt Status Clear Register */
  RwReg   ACIFC_TR;           /**< \brief (ACIFC Offset: 0x24) Test Register */
  RoReg8  Reserved2[0x8];
  RoReg   ACIFC_PARAMETER;    /**< \brief (ACIFC Offset: 0x30) Parameter Register */
  RoReg   ACIFC_VERSION;      /**< \brief (ACIFC Offset: 0x34) Version Register */
  RoReg8  Reserved3[0x48];
  AcifcConfw ACIFC_CONFW[4];     /**< \brief (ACIFC Offset: 0x80) AcifcConfw groups */
  RoReg8  Reserved4[0x40];
  AcifcConf ACIFC_CONF[8];      /**< \brief (ACIFC Offset: 0xD0) AcifcConf groups */
} Acifc;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/*@}*/

#endif /* _SAM4L_ACIFC_COMPONENT_ */
