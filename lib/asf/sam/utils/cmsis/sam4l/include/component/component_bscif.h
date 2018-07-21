/**
 * \file
 *
 * \brief Component description for BSCIF
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

#ifndef _SAM4L_BSCIF_COMPONENT_
#define _SAM4L_BSCIF_COMPONENT_

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR BSCIF */
/* ========================================================================== */
/** \addtogroup SAM4L_BSCIF Backup System Control Interface */
/*@{*/

#define REV_BSCIF                   0x100

/* -------- BSCIF_IER : (BSCIF Offset: 0x000) ( /W 32) Interrupt Enable Register -------- */
#define BSCIF_IER_OFFSET            0x000        /**< \brief (BSCIF_IER offset) Interrupt Enable Register */
#define BSCIF_IER_RESETVALUE        0x00000000   /**< \brief (BSCIF_IER reset_value) Interrupt Enable Register */

#define BSCIF_IER_OSC32RDY_Pos      0            /**< \brief (BSCIF_IER)  */
#define BSCIF_IER_OSC32RDY          (0x1u << BSCIF_IER_OSC32RDY_Pos)
#define BSCIF_IER_RC32KRDY_Pos      1            /**< \brief (BSCIF_IER)  */
#define BSCIF_IER_RC32KRDY          (0x1u << BSCIF_IER_RC32KRDY_Pos)
#define BSCIF_IER_RC32KLOCK_Pos     2            /**< \brief (BSCIF_IER)  */
#define BSCIF_IER_RC32KLOCK         (0x1u << BSCIF_IER_RC32KLOCK_Pos)
#define BSCIF_IER_RC32KREFE_Pos     3            /**< \brief (BSCIF_IER)  */
#define BSCIF_IER_RC32KREFE         (0x1u << BSCIF_IER_RC32KREFE_Pos)
#define BSCIF_IER_RC32KSAT_Pos      4            /**< \brief (BSCIF_IER)  */
#define BSCIF_IER_RC32KSAT          (0x1u << BSCIF_IER_RC32KSAT_Pos)
#define BSCIF_IER_BOD33DET_Pos      5            /**< \brief (BSCIF_IER) BOD33 Detected */
#define BSCIF_IER_BOD33DET          (0x1u << BSCIF_IER_BOD33DET_Pos)
#define BSCIF_IER_BOD18DET_Pos      6            /**< \brief (BSCIF_IER) BOD18 Detected */
#define BSCIF_IER_BOD18DET          (0x1u << BSCIF_IER_BOD18DET_Pos)
#define BSCIF_IER_BOD33SYNRDY_Pos   7            /**< \brief (BSCIF_IER) BOD33 Synchronization Ready */
#define BSCIF_IER_BOD33SYNRDY       (0x1u << BSCIF_IER_BOD33SYNRDY_Pos)
#define BSCIF_IER_BOD18SYNRDY_Pos   8            /**< \brief (BSCIF_IER) BOD18 Synchronization Ready */
#define BSCIF_IER_BOD18SYNRDY       (0x1u << BSCIF_IER_BOD18SYNRDY_Pos)
#define BSCIF_IER_SSWRDY_Pos        9            /**< \brief (BSCIF_IER) VREG Stop Switching Ready */
#define BSCIF_IER_SSWRDY            (0x1u << BSCIF_IER_SSWRDY_Pos)
#define BSCIF_IER_VREGOK_Pos        10           /**< \brief (BSCIF_IER) Main VREG OK */
#define BSCIF_IER_VREGOK            (0x1u << BSCIF_IER_VREGOK_Pos)
#define BSCIF_IER_LPBGRDY_Pos       12           /**< \brief (BSCIF_IER) Low Power Bandgap Voltage Reference Ready */
#define BSCIF_IER_LPBGRDY           (0x1u << BSCIF_IER_LPBGRDY_Pos)
#define BSCIF_IER_AE_Pos            31           /**< \brief (BSCIF_IER) Access Error */
#define BSCIF_IER_AE                (0x1u << BSCIF_IER_AE_Pos)
#define BSCIF_IER_MASK              0x800017FFu  /**< \brief (BSCIF_IER) MASK Register */

/* -------- BSCIF_IDR : (BSCIF Offset: 0x004) ( /W 32) Interrupt Disable Register -------- */
#define BSCIF_IDR_OFFSET            0x004        /**< \brief (BSCIF_IDR offset) Interrupt Disable Register */
#define BSCIF_IDR_RESETVALUE        0x00000000   /**< \brief (BSCIF_IDR reset_value) Interrupt Disable Register */

#define BSCIF_IDR_OSC32RDY_Pos      0            /**< \brief (BSCIF_IDR)  */
#define BSCIF_IDR_OSC32RDY          (0x1u << BSCIF_IDR_OSC32RDY_Pos)
#define BSCIF_IDR_RC32KRDY_Pos      1            /**< \brief (BSCIF_IDR)  */
#define BSCIF_IDR_RC32KRDY          (0x1u << BSCIF_IDR_RC32KRDY_Pos)
#define BSCIF_IDR_RC32KLOCK_Pos     2            /**< \brief (BSCIF_IDR)  */
#define BSCIF_IDR_RC32KLOCK         (0x1u << BSCIF_IDR_RC32KLOCK_Pos)
#define BSCIF_IDR_RC32KREFE_Pos     3            /**< \brief (BSCIF_IDR)  */
#define BSCIF_IDR_RC32KREFE         (0x1u << BSCIF_IDR_RC32KREFE_Pos)
#define BSCIF_IDR_RC32KSAT_Pos      4            /**< \brief (BSCIF_IDR)  */
#define BSCIF_IDR_RC32KSAT          (0x1u << BSCIF_IDR_RC32KSAT_Pos)
#define BSCIF_IDR_BOD33DET_Pos      5            /**< \brief (BSCIF_IDR) BOD33 Detected */
#define BSCIF_IDR_BOD33DET          (0x1u << BSCIF_IDR_BOD33DET_Pos)
#define BSCIF_IDR_BOD18DET_Pos      6            /**< \brief (BSCIF_IDR) BOD18 Detected */
#define BSCIF_IDR_BOD18DET          (0x1u << BSCIF_IDR_BOD18DET_Pos)
#define BSCIF_IDR_BOD33SYNRDY_Pos   7            /**< \brief (BSCIF_IDR) BOD33 Synchronization Ready */
#define BSCIF_IDR_BOD33SYNRDY       (0x1u << BSCIF_IDR_BOD33SYNRDY_Pos)
#define BSCIF_IDR_BOD18SYNRDY_Pos   8            /**< \brief (BSCIF_IDR) BOD18 Synchronization Ready */
#define BSCIF_IDR_BOD18SYNRDY       (0x1u << BSCIF_IDR_BOD18SYNRDY_Pos)
#define BSCIF_IDR_SSWRDY_Pos        9            /**< \brief (BSCIF_IDR) VREG Stop Switching Ready */
#define BSCIF_IDR_SSWRDY            (0x1u << BSCIF_IDR_SSWRDY_Pos)
#define BSCIF_IDR_VREGOK_Pos        10           /**< \brief (BSCIF_IDR) Mai n VREG OK */
#define BSCIF_IDR_VREGOK            (0x1u << BSCIF_IDR_VREGOK_Pos)
#define BSCIF_IDR_LPBGRDY_Pos       12           /**< \brief (BSCIF_IDR) Low Power Bandgap Voltage Reference Ready */
#define BSCIF_IDR_LPBGRDY           (0x1u << BSCIF_IDR_LPBGRDY_Pos)
#define BSCIF_IDR_AE_Pos            31           /**< \brief (BSCIF_IDR) Access Error */
#define BSCIF_IDR_AE                (0x1u << BSCIF_IDR_AE_Pos)
#define BSCIF_IDR_MASK              0x800017FFu  /**< \brief (BSCIF_IDR) MASK Register */

/* -------- BSCIF_IMR : (BSCIF Offset: 0x008) (R/  32) Interrupt Mask Register -------- */
#define BSCIF_IMR_OFFSET            0x008        /**< \brief (BSCIF_IMR offset) Interrupt Mask Register */
#define BSCIF_IMR_RESETVALUE        0x00000000   /**< \brief (BSCIF_IMR reset_value) Interrupt Mask Register */

#define BSCIF_IMR_OSC32RDY_Pos      0            /**< \brief (BSCIF_IMR)  */
#define BSCIF_IMR_OSC32RDY          (0x1u << BSCIF_IMR_OSC32RDY_Pos)
#define BSCIF_IMR_RC32KRDY_Pos      1            /**< \brief (BSCIF_IMR)  */
#define BSCIF_IMR_RC32KRDY          (0x1u << BSCIF_IMR_RC32KRDY_Pos)
#define BSCIF_IMR_RC32KLOCK_Pos     2            /**< \brief (BSCIF_IMR)  */
#define BSCIF_IMR_RC32KLOCK         (0x1u << BSCIF_IMR_RC32KLOCK_Pos)
#define BSCIF_IMR_RC32KREFE_Pos     3            /**< \brief (BSCIF_IMR)  */
#define BSCIF_IMR_RC32KREFE         (0x1u << BSCIF_IMR_RC32KREFE_Pos)
#define BSCIF_IMR_RC32KSAT_Pos      4            /**< \brief (BSCIF_IMR)  */
#define BSCIF_IMR_RC32KSAT          (0x1u << BSCIF_IMR_RC32KSAT_Pos)
#define BSCIF_IMR_BOD33DET_Pos      5            /**< \brief (BSCIF_IMR) BOD33 Detected */
#define BSCIF_IMR_BOD33DET          (0x1u << BSCIF_IMR_BOD33DET_Pos)
#define BSCIF_IMR_BOD18DET_Pos      6            /**< \brief (BSCIF_IMR) BOD18 Detected */
#define BSCIF_IMR_BOD18DET          (0x1u << BSCIF_IMR_BOD18DET_Pos)
#define BSCIF_IMR_BOD33SYNRDY_Pos   7            /**< \brief (BSCIF_IMR) BOD33 Synchronization Ready */
#define BSCIF_IMR_BOD33SYNRDY       (0x1u << BSCIF_IMR_BOD33SYNRDY_Pos)
#define BSCIF_IMR_BOD18SYNRDY_Pos   8            /**< \brief (BSCIF_IMR) BOD18 Synchronization Ready */
#define BSCIF_IMR_BOD18SYNRDY       (0x1u << BSCIF_IMR_BOD18SYNRDY_Pos)
#define BSCIF_IMR_SSWRDY_Pos        9            /**< \brief (BSCIF_IMR) VREG Stop Switching Ready */
#define BSCIF_IMR_SSWRDY            (0x1u << BSCIF_IMR_SSWRDY_Pos)
#define BSCIF_IMR_VREGOK_Pos        10           /**< \brief (BSCIF_IMR) Main VREG OK */
#define BSCIF_IMR_VREGOK            (0x1u << BSCIF_IMR_VREGOK_Pos)
#define BSCIF_IMR_LPBGRDY_Pos       12           /**< \brief (BSCIF_IMR) Low Power Bandgap Voltage Reference Ready */
#define BSCIF_IMR_LPBGRDY           (0x1u << BSCIF_IMR_LPBGRDY_Pos)
#define BSCIF_IMR_AE_Pos            31           /**< \brief (BSCIF_IMR) Access Error */
#define BSCIF_IMR_AE                (0x1u << BSCIF_IMR_AE_Pos)
#define BSCIF_IMR_MASK              0x800017FFu  /**< \brief (BSCIF_IMR) MASK Register */

/* -------- BSCIF_ISR : (BSCIF Offset: 0x00C) (R/  32) Interrupt Status Register -------- */
#define BSCIF_ISR_OFFSET            0x00C        /**< \brief (BSCIF_ISR offset) Interrupt Status Register */
#define BSCIF_ISR_RESETVALUE        0x00000000   /**< \brief (BSCIF_ISR reset_value) Interrupt Status Register */

#define BSCIF_ISR_OSC32RDY_Pos      0            /**< \brief (BSCIF_ISR)  */
#define BSCIF_ISR_OSC32RDY          (0x1u << BSCIF_ISR_OSC32RDY_Pos)
#define BSCIF_ISR_RC32KRDY_Pos      1            /**< \brief (BSCIF_ISR)  */
#define BSCIF_ISR_RC32KRDY          (0x1u << BSCIF_ISR_RC32KRDY_Pos)
#define BSCIF_ISR_RC32KLOCK_Pos     2            /**< \brief (BSCIF_ISR)  */
#define BSCIF_ISR_RC32KLOCK         (0x1u << BSCIF_ISR_RC32KLOCK_Pos)
#define BSCIF_ISR_RC32KREFE_Pos     3            /**< \brief (BSCIF_ISR)  */
#define BSCIF_ISR_RC32KREFE         (0x1u << BSCIF_ISR_RC32KREFE_Pos)
#define BSCIF_ISR_RC32KSAT_Pos      4            /**< \brief (BSCIF_ISR)  */
#define BSCIF_ISR_RC32KSAT          (0x1u << BSCIF_ISR_RC32KSAT_Pos)
#define BSCIF_ISR_BOD33DET_Pos      5            /**< \brief (BSCIF_ISR) BOD33 Detected */
#define BSCIF_ISR_BOD33DET          (0x1u << BSCIF_ISR_BOD33DET_Pos)
#define BSCIF_ISR_BOD18DET_Pos      6            /**< \brief (BSCIF_ISR) BOD18 Detected */
#define BSCIF_ISR_BOD18DET          (0x1u << BSCIF_ISR_BOD18DET_Pos)
#define BSCIF_ISR_BOD33SYNRDY_Pos   7            /**< \brief (BSCIF_ISR) BOD33 Synchronization Ready */
#define BSCIF_ISR_BOD33SYNRDY       (0x1u << BSCIF_ISR_BOD33SYNRDY_Pos)
#define BSCIF_ISR_BOD18SYNRDY_Pos   8            /**< \brief (BSCIF_ISR) BOD18 Synchronization Ready */
#define BSCIF_ISR_BOD18SYNRDY       (0x1u << BSCIF_ISR_BOD18SYNRDY_Pos)
#define BSCIF_ISR_SSWRDY_Pos        9            /**< \brief (BSCIF_ISR) VREG Stop Switching Ready */
#define BSCIF_ISR_SSWRDY            (0x1u << BSCIF_ISR_SSWRDY_Pos)
#define BSCIF_ISR_VREGOK_Pos        10           /**< \brief (BSCIF_ISR) Main VREG OK */
#define BSCIF_ISR_VREGOK            (0x1u << BSCIF_ISR_VREGOK_Pos)
#define BSCIF_ISR_LPBGRDY_Pos       12           /**< \brief (BSCIF_ISR) Low Power Bandgap Voltage Reference Ready */
#define BSCIF_ISR_LPBGRDY           (0x1u << BSCIF_ISR_LPBGRDY_Pos)
#define BSCIF_ISR_AE_Pos            31           /**< \brief (BSCIF_ISR) Access Error */
#define BSCIF_ISR_AE                (0x1u << BSCIF_ISR_AE_Pos)
#define BSCIF_ISR_MASK              0x800017FFu  /**< \brief (BSCIF_ISR) MASK Register */

/* -------- BSCIF_ICR : (BSCIF Offset: 0x010) ( /W 32) Interrupt Clear Register -------- */
#define BSCIF_ICR_OFFSET            0x010        /**< \brief (BSCIF_ICR offset) Interrupt Clear Register */
#define BSCIF_ICR_RESETVALUE        0x00000000   /**< \brief (BSCIF_ICR reset_value) Interrupt Clear Register */

#define BSCIF_ICR_OSC32RDY_Pos      0            /**< \brief (BSCIF_ICR)  */
#define BSCIF_ICR_OSC32RDY          (0x1u << BSCIF_ICR_OSC32RDY_Pos)
#define BSCIF_ICR_RC32KRDY_Pos      1            /**< \brief (BSCIF_ICR)  */
#define BSCIF_ICR_RC32KRDY          (0x1u << BSCIF_ICR_RC32KRDY_Pos)
#define BSCIF_ICR_RC32KLOCK_Pos     2            /**< \brief (BSCIF_ICR)  */
#define BSCIF_ICR_RC32KLOCK         (0x1u << BSCIF_ICR_RC32KLOCK_Pos)
#define BSCIF_ICR_RC32KREFE_Pos     3            /**< \brief (BSCIF_ICR)  */
#define BSCIF_ICR_RC32KREFE         (0x1u << BSCIF_ICR_RC32KREFE_Pos)
#define BSCIF_ICR_RC32KSAT_Pos      4            /**< \brief (BSCIF_ICR)  */
#define BSCIF_ICR_RC32KSAT          (0x1u << BSCIF_ICR_RC32KSAT_Pos)
#define BSCIF_ICR_BOD33DET_Pos      5            /**< \brief (BSCIF_ICR) BOD33 Detected */
#define BSCIF_ICR_BOD33DET          (0x1u << BSCIF_ICR_BOD33DET_Pos)
#define BSCIF_ICR_BOD18DET_Pos      6            /**< \brief (BSCIF_ICR) BOD18 Detected */
#define BSCIF_ICR_BOD18DET          (0x1u << BSCIF_ICR_BOD18DET_Pos)
#define BSCIF_ICR_BOD33SYNRDY_Pos   7            /**< \brief (BSCIF_ICR) BOD33 Synchronization Ready */
#define BSCIF_ICR_BOD33SYNRDY       (0x1u << BSCIF_ICR_BOD33SYNRDY_Pos)
#define BSCIF_ICR_BOD18SYNRDY_Pos   8            /**< \brief (BSCIF_ICR) BOD18 Synchronization Ready */
#define BSCIF_ICR_BOD18SYNRDY       (0x1u << BSCIF_ICR_BOD18SYNRDY_Pos)
#define BSCIF_ICR_SSWRDY_Pos        9            /**< \brief (BSCIF_ICR) VREG Stop Switching Ready */
#define BSCIF_ICR_SSWRDY            (0x1u << BSCIF_ICR_SSWRDY_Pos)
#define BSCIF_ICR_VREGOK_Pos        10           /**< \brief (BSCIF_ICR) Main VREG OK */
#define BSCIF_ICR_VREGOK            (0x1u << BSCIF_ICR_VREGOK_Pos)
#define BSCIF_ICR_LPBGRDY_Pos       12           /**< \brief (BSCIF_ICR) Low Power Bandgap Voltage Reference Ready */
#define BSCIF_ICR_LPBGRDY           (0x1u << BSCIF_ICR_LPBGRDY_Pos)
#define BSCIF_ICR_AE_Pos            31           /**< \brief (BSCIF_ICR) Access Error */
#define BSCIF_ICR_AE                (0x1u << BSCIF_ICR_AE_Pos)
#define BSCIF_ICR_MASK              0x800017FFu  /**< \brief (BSCIF_ICR) MASK Register */

/* -------- BSCIF_PCLKSR : (BSCIF Offset: 0x014) (R/  32) Power and Clocks Status Register -------- */
#define BSCIF_PCLKSR_OFFSET         0x014        /**< \brief (BSCIF_PCLKSR offset) Power and Clocks Status Register */
#define BSCIF_PCLKSR_RESETVALUE     0x00000000   /**< \brief (BSCIF_PCLKSR reset_value) Power and Clocks Status Register */

#define BSCIF_PCLKSR_OSC32RDY_Pos   0            /**< \brief (BSCIF_PCLKSR)  */
#define BSCIF_PCLKSR_OSC32RDY       (0x1u << BSCIF_PCLKSR_OSC32RDY_Pos)
#define BSCIF_PCLKSR_RC32KRDY_Pos   1            /**< \brief (BSCIF_PCLKSR)  */
#define BSCIF_PCLKSR_RC32KRDY       (0x1u << BSCIF_PCLKSR_RC32KRDY_Pos)
#define BSCIF_PCLKSR_RC32KLOCK_Pos  2            /**< \brief (BSCIF_PCLKSR)  */
#define BSCIF_PCLKSR_RC32KLOCK      (0x1u << BSCIF_PCLKSR_RC32KLOCK_Pos)
#define BSCIF_PCLKSR_RC32KREFE_Pos  3            /**< \brief (BSCIF_PCLKSR)  */
#define BSCIF_PCLKSR_RC32KREFE      (0x1u << BSCIF_PCLKSR_RC32KREFE_Pos)
#define BSCIF_PCLKSR_RC32KSAT_Pos   4            /**< \brief (BSCIF_PCLKSR)  */
#define BSCIF_PCLKSR_RC32KSAT       (0x1u << BSCIF_PCLKSR_RC32KSAT_Pos)
#define BSCIF_PCLKSR_BOD33DET_Pos   5            /**< \brief (BSCIF_PCLKSR) BOD33 Detected */
#define BSCIF_PCLKSR_BOD33DET       (0x1u << BSCIF_PCLKSR_BOD33DET_Pos)
#define BSCIF_PCLKSR_BOD18DET_Pos   6            /**< \brief (BSCIF_PCLKSR) BOD18 Detected */
#define BSCIF_PCLKSR_BOD18DET       (0x1u << BSCIF_PCLKSR_BOD18DET_Pos)
#define BSCIF_PCLKSR_BOD33SYNRDY_Pos 7            /**< \brief (BSCIF_PCLKSR) BOD33 Synchronization Ready */
#define BSCIF_PCLKSR_BOD33SYNRDY    (0x1u << BSCIF_PCLKSR_BOD33SYNRDY_Pos)
#define BSCIF_PCLKSR_BOD18SYNRDY_Pos 8            /**< \brief (BSCIF_PCLKSR) BOD18 Synchronization Ready */
#define BSCIF_PCLKSR_BOD18SYNRDY    (0x1u << BSCIF_PCLKSR_BOD18SYNRDY_Pos)
#define BSCIF_PCLKSR_SSWRDY_Pos     9            /**< \brief (BSCIF_PCLKSR) VREG Stop Switching Ready */
#define BSCIF_PCLKSR_SSWRDY         (0x1u << BSCIF_PCLKSR_SSWRDY_Pos)
#define BSCIF_PCLKSR_VREGOK_Pos     10           /**< \brief (BSCIF_PCLKSR) Main VREG OK */
#define BSCIF_PCLKSR_VREGOK         (0x1u << BSCIF_PCLKSR_VREGOK_Pos)
#define BSCIF_PCLKSR_RC1MRDY_Pos    11           /**< \brief (BSCIF_PCLKSR) RC 1MHz Oscillator Ready */
#define BSCIF_PCLKSR_RC1MRDY        (0x1u << BSCIF_PCLKSR_RC1MRDY_Pos)
#define BSCIF_PCLKSR_LPBGRDY_Pos    12           /**< \brief (BSCIF_PCLKSR) Low Power Bandgap Voltage Reference Ready */
#define BSCIF_PCLKSR_LPBGRDY        (0x1u << BSCIF_PCLKSR_LPBGRDY_Pos)
#define BSCIF_PCLKSR_MASK           0x00001FFFu  /**< \brief (BSCIF_PCLKSR) MASK Register */

/* -------- BSCIF_UNLOCK : (BSCIF Offset: 0x018) ( /W 32) Unlock Register -------- */
#define BSCIF_UNLOCK_OFFSET         0x018        /**< \brief (BSCIF_UNLOCK offset) Unlock Register */
#define BSCIF_UNLOCK_RESETVALUE     0x00000000   /**< \brief (BSCIF_UNLOCK reset_value) Unlock Register */

#define BSCIF_UNLOCK_ADDR_Pos       0            /**< \brief (BSCIF_UNLOCK) Unlock Address */
#define BSCIF_UNLOCK_ADDR_Msk       (0x3FFu << BSCIF_UNLOCK_ADDR_Pos)
#define BSCIF_UNLOCK_ADDR(value)    ((BSCIF_UNLOCK_ADDR_Msk & ((value) << BSCIF_UNLOCK_ADDR_Pos)))
#define BSCIF_UNLOCK_KEY_Pos        24           /**< \brief (BSCIF_UNLOCK) Unlock Key */
#define BSCIF_UNLOCK_KEY_Msk        (0xFFu << BSCIF_UNLOCK_KEY_Pos)
#define BSCIF_UNLOCK_KEY(value)     ((BSCIF_UNLOCK_KEY_Msk & ((value) << BSCIF_UNLOCK_KEY_Pos)))
#define   BSCIF_UNLOCK_KEY_VALID    (0xAAu << 24) /**< \brief (BSCIF_UNLOCK) Valid Key to Unlock register */
#define BSCIF_UNLOCK_MASK           0xFF0003FFu  /**< \brief (BSCIF_UNLOCK) MASK Register */

/* -------- BSCIF_CSCR : (BSCIF Offset: 0x01C) (R/W 32) Chip Specific Configuration Register -------- */
#define BSCIF_CSCR_OFFSET           0x01C        /**< \brief (BSCIF_CSCR offset) Chip Specific Configuration Register */
#define BSCIF_CSCR_RESETVALUE       0x00000000   /**< \brief (BSCIF_CSCR reset_value) Chip Specific Configuration Register */

#define BSCIF_CSCR_MASK             0x00000000u  /**< \brief (BSCIF_CSCR) MASK Register */

/* -------- BSCIF_OSCCTRL32 : (BSCIF Offset: 0x020) (R/W 32) Oscillator 32 Control Register -------- */
#define BSCIF_OSCCTRL32_OFFSET      0x020        /**< \brief (BSCIF_OSCCTRL32 offset) Oscillator 32 Control Register */
#define BSCIF_OSCCTRL32_RESETVALUE  0x00000004   /**< \brief (BSCIF_OSCCTRL32 reset_value) Oscillator 32 Control Register */

#define BSCIF_OSCCTRL32_OSC32EN_Pos 0            /**< \brief (BSCIF_OSCCTRL32) 32 KHz Oscillator Enable */
#define BSCIF_OSCCTRL32_OSC32EN     (0x1u << BSCIF_OSCCTRL32_OSC32EN_Pos)
#define BSCIF_OSCCTRL32_PINSEL_Pos  1            /**< \brief (BSCIF_OSCCTRL32) Pins Select */
#define BSCIF_OSCCTRL32_PINSEL      (0x1u << BSCIF_OSCCTRL32_PINSEL_Pos)
#define BSCIF_OSCCTRL32_EN32K_Pos   2            /**< \brief (BSCIF_OSCCTRL32) 32 KHz output Enable */
#define BSCIF_OSCCTRL32_EN32K       (0x1u << BSCIF_OSCCTRL32_EN32K_Pos)
#define BSCIF_OSCCTRL32_EN1K_Pos    3            /**< \brief (BSCIF_OSCCTRL32) 1 KHz output Enable */
#define BSCIF_OSCCTRL32_EN1K        (0x1u << BSCIF_OSCCTRL32_EN1K_Pos)
#define BSCIF_OSCCTRL32_MODE_Pos    8            /**< \brief (BSCIF_OSCCTRL32) Oscillator Mode */
#define BSCIF_OSCCTRL32_MODE_Msk    (0x7u << BSCIF_OSCCTRL32_MODE_Pos)
#define BSCIF_OSCCTRL32_MODE(value) ((BSCIF_OSCCTRL32_MODE_Msk & ((value) << BSCIF_OSCCTRL32_MODE_Pos)))
#define BSCIF_OSCCTRL32_SELCURR_Pos 12           /**< \brief (BSCIF_OSCCTRL32) Current selection */
#define BSCIF_OSCCTRL32_SELCURR_Msk (0xFu << BSCIF_OSCCTRL32_SELCURR_Pos)
#define BSCIF_OSCCTRL32_SELCURR(value) ((BSCIF_OSCCTRL32_SELCURR_Msk & ((value) << BSCIF_OSCCTRL32_SELCURR_Pos)))
#define BSCIF_OSCCTRL32_STARTUP_Pos 16           /**< \brief (BSCIF_OSCCTRL32) Oscillator Start-up Time */
#define BSCIF_OSCCTRL32_STARTUP_Msk (0x7u << BSCIF_OSCCTRL32_STARTUP_Pos)
#define BSCIF_OSCCTRL32_STARTUP(value) ((BSCIF_OSCCTRL32_STARTUP_Msk & ((value) << BSCIF_OSCCTRL32_STARTUP_Pos)))
#define BSCIF_OSCCTRL32_MASK        0x0007F70Fu  /**< \brief (BSCIF_OSCCTRL32) MASK Register */

/* -------- BSCIF_RC32KCR : (BSCIF Offset: 0x024) (R/W 32) 32 kHz RC Oscillator Control Register -------- */
#define BSCIF_RC32KCR_OFFSET        0x024        /**< \brief (BSCIF_RC32KCR offset) 32 kHz RC Oscillator Control Register */
#define BSCIF_RC32KCR_RESETVALUE    0x00000000   /**< \brief (BSCIF_RC32KCR reset_value) 32 kHz RC Oscillator Control Register */

#define BSCIF_RC32KCR_EN_Pos        0            /**< \brief (BSCIF_RC32KCR) Enable as Generic clock source */
#define BSCIF_RC32KCR_EN            (0x1u << BSCIF_RC32KCR_EN_Pos)
#define BSCIF_RC32KCR_TCEN_Pos      1            /**< \brief (BSCIF_RC32KCR) Temperature Compensation Enable */
#define BSCIF_RC32KCR_TCEN          (0x1u << BSCIF_RC32KCR_TCEN_Pos)
#define BSCIF_RC32KCR_EN32K_Pos     2            /**< \brief (BSCIF_RC32KCR) Enable 32 KHz output */
#define BSCIF_RC32KCR_EN32K         (0x1u << BSCIF_RC32KCR_EN32K_Pos)
#define BSCIF_RC32KCR_EN1K_Pos      3            /**< \brief (BSCIF_RC32KCR) Enable 1 kHz output */
#define BSCIF_RC32KCR_EN1K          (0x1u << BSCIF_RC32KCR_EN1K_Pos)
#define BSCIF_RC32KCR_MODE_Pos      4            /**< \brief (BSCIF_RC32KCR) Mode Selection */
#define BSCIF_RC32KCR_MODE          (0x1u << BSCIF_RC32KCR_MODE_Pos)
#define BSCIF_RC32KCR_REF_Pos       5            /**< \brief (BSCIF_RC32KCR) Reference select */
#define BSCIF_RC32KCR_REF           (0x1u << BSCIF_RC32KCR_REF_Pos)
#define BSCIF_RC32KCR_FCD_Pos       7            /**< \brief (BSCIF_RC32KCR) Flash calibration done */
#define BSCIF_RC32KCR_FCD           (0x1u << BSCIF_RC32KCR_FCD_Pos)
#define BSCIF_RC32KCR_MASK          0x000000BFu  /**< \brief (BSCIF_RC32KCR) MASK Register */

/* -------- BSCIF_RC32KTUNE : (BSCIF Offset: 0x028) (R/W 32) 32kHz RC Oscillator Tuning Register -------- */
#define BSCIF_RC32KTUNE_OFFSET      0x028        /**< \brief (BSCIF_RC32KTUNE offset) 32kHz RC Oscillator Tuning Register */
#define BSCIF_RC32KTUNE_RESETVALUE  0x00000000   /**< \brief (BSCIF_RC32KTUNE reset_value) 32kHz RC Oscillator Tuning Register */

#define BSCIF_RC32KTUNE_FINE_Pos    0            /**< \brief (BSCIF_RC32KTUNE) Fine value */
#define BSCIF_RC32KTUNE_FINE_Msk    (0x3Fu << BSCIF_RC32KTUNE_FINE_Pos)
#define BSCIF_RC32KTUNE_FINE(value) ((BSCIF_RC32KTUNE_FINE_Msk & ((value) << BSCIF_RC32KTUNE_FINE_Pos)))
#define BSCIF_RC32KTUNE_COARSE_Pos  16           /**< \brief (BSCIF_RC32KTUNE) Coarse Value */
#define BSCIF_RC32KTUNE_COARSE_Msk  (0x7Fu << BSCIF_RC32KTUNE_COARSE_Pos)
#define BSCIF_RC32KTUNE_COARSE(value) ((BSCIF_RC32KTUNE_COARSE_Msk & ((value) << BSCIF_RC32KTUNE_COARSE_Pos)))
#define BSCIF_RC32KTUNE_MASK        0x007F003Fu  /**< \brief (BSCIF_RC32KTUNE) MASK Register */

/* -------- BSCIF_BOD33CTRL : (BSCIF Offset: 0x02C) (R/W 32) BOD33 Control Register -------- */
#define BSCIF_BOD33CTRL_OFFSET      0x02C        /**< \brief (BSCIF_BOD33CTRL offset) BOD33 Control Register */

#define BSCIF_BOD33CTRL_EN_Pos      0            /**< \brief (BSCIF_BOD33CTRL) Enable */
#define BSCIF_BOD33CTRL_EN          (0x1u << BSCIF_BOD33CTRL_EN_Pos)
#define BSCIF_BOD33CTRL_HYST_Pos    1            /**< \brief (BSCIF_BOD33CTRL) BOD Hysteresis */
#define BSCIF_BOD33CTRL_HYST        (0x1u << BSCIF_BOD33CTRL_HYST_Pos)
#define BSCIF_BOD33CTRL_ACTION_Pos  8            /**< \brief (BSCIF_BOD33CTRL) Action */
#define BSCIF_BOD33CTRL_ACTION_Msk  (0x3u << BSCIF_BOD33CTRL_ACTION_Pos)
#define BSCIF_BOD33CTRL_ACTION(value) ((BSCIF_BOD33CTRL_ACTION_Msk & ((value) << BSCIF_BOD33CTRL_ACTION_Pos)))
#define BSCIF_BOD33CTRL_MODE_Pos    16           /**< \brief (BSCIF_BOD33CTRL) Operation modes */
#define BSCIF_BOD33CTRL_MODE        (0x1u << BSCIF_BOD33CTRL_MODE_Pos)
#define BSCIF_BOD33CTRL_FCD_Pos     30           /**< \brief (BSCIF_BOD33CTRL) BOD Fuse Calibration Done */
#define BSCIF_BOD33CTRL_FCD         (0x1u << BSCIF_BOD33CTRL_FCD_Pos)
#define BSCIF_BOD33CTRL_SFV_Pos     31           /**< \brief (BSCIF_BOD33CTRL) BOD Control Register Store Final Value */
#define BSCIF_BOD33CTRL_SFV         (0x1u << BSCIF_BOD33CTRL_SFV_Pos)
#define BSCIF_BOD33CTRL_MASK        0xC0010303u  /**< \brief (BSCIF_BOD33CTRL) MASK Register */

/* -------- BSCIF_BOD33LEVEL : (BSCIF Offset: 0x030) (R/W 32) BOD33 Level Register -------- */
#define BSCIF_BOD33LEVEL_OFFSET     0x030        /**< \brief (BSCIF_BOD33LEVEL offset) BOD33 Level Register */
#define BSCIF_BOD33LEVEL_RESETVALUE 0x00000000   /**< \brief (BSCIF_BOD33LEVEL reset_value) BOD33 Level Register */

#define BSCIF_BOD33LEVEL_VAL_Pos    0            /**< \brief (BSCIF_BOD33LEVEL) BOD Value */
#define BSCIF_BOD33LEVEL_VAL_Msk    (0x3Fu << BSCIF_BOD33LEVEL_VAL_Pos)
#define BSCIF_BOD33LEVEL_VAL(value) ((BSCIF_BOD33LEVEL_VAL_Msk & ((value) << BSCIF_BOD33LEVEL_VAL_Pos)))
#define BSCIF_BOD33LEVEL_MASK       0x0000003Fu  /**< \brief (BSCIF_BOD33LEVEL) MASK Register */

/* -------- BSCIF_BOD33SAMPLING : (BSCIF Offset: 0x034) (R/W 32) BOD33 Sampling Control Register -------- */
#define BSCIF_BOD33SAMPLING_OFFSET  0x034        /**< \brief (BSCIF_BOD33SAMPLING offset) BOD33 Sampling Control Register */
#define BSCIF_BOD33SAMPLING_RESETVALUE 0x00000000   /**< \brief (BSCIF_BOD33SAMPLING reset_value) BOD33 Sampling Control Register */

#define BSCIF_BOD33SAMPLING_CEN_Pos 0            /**< \brief (BSCIF_BOD33SAMPLING) Clock Enable */
#define BSCIF_BOD33SAMPLING_CEN     (0x1u << BSCIF_BOD33SAMPLING_CEN_Pos)
#define BSCIF_BOD33SAMPLING_CSSEL_Pos 1            /**< \brief (BSCIF_BOD33SAMPLING) Clock Source Select */
#define BSCIF_BOD33SAMPLING_CSSEL   (0x1u << BSCIF_BOD33SAMPLING_CSSEL_Pos)
#define BSCIF_BOD33SAMPLING_PSEL_Pos 8            /**< \brief (BSCIF_BOD33SAMPLING) Prescaler Select */
#define BSCIF_BOD33SAMPLING_PSEL_Msk (0xFu << BSCIF_BOD33SAMPLING_PSEL_Pos)
#define BSCIF_BOD33SAMPLING_PSEL(value) ((BSCIF_BOD33SAMPLING_PSEL_Msk & ((value) << BSCIF_BOD33SAMPLING_PSEL_Pos)))
#define BSCIF_BOD33SAMPLING_MASK    0x00000F03u  /**< \brief (BSCIF_BOD33SAMPLING) MASK Register */

/* -------- BSCIF_BOD18CTRL : (BSCIF Offset: 0x038) (R/W 32) BOD18 Control Register -------- */
#define BSCIF_BOD18CTRL_OFFSET      0x038        /**< \brief (BSCIF_BOD18CTRL offset) BOD18 Control Register */
#define BSCIF_BOD18CTRL_RESETVALUE  0x00000000   /**< \brief (BSCIF_BOD18CTRL reset_value) BOD18 Control Register */

#define BSCIF_BOD18CTRL_EN_Pos      0            /**< \brief (BSCIF_BOD18CTRL) Enable */
#define BSCIF_BOD18CTRL_EN          (0x1u << BSCIF_BOD18CTRL_EN_Pos)
#define BSCIF_BOD18CTRL_HYST_Pos    1            /**< \brief (BSCIF_BOD18CTRL) BOD Hysteresis */
#define BSCIF_BOD18CTRL_HYST        (0x1u << BSCIF_BOD18CTRL_HYST_Pos)
#define BSCIF_BOD18CTRL_ACTION_Pos  8            /**< \brief (BSCIF_BOD18CTRL) Action */
#define BSCIF_BOD18CTRL_ACTION_Msk  (0x3u << BSCIF_BOD18CTRL_ACTION_Pos)
#define BSCIF_BOD18CTRL_ACTION(value) ((BSCIF_BOD18CTRL_ACTION_Msk & ((value) << BSCIF_BOD18CTRL_ACTION_Pos)))
#define BSCIF_BOD18CTRL_MODE_Pos    16           /**< \brief (BSCIF_BOD18CTRL) Operation modes */
#define BSCIF_BOD18CTRL_MODE        (0x1u << BSCIF_BOD18CTRL_MODE_Pos)
#define BSCIF_BOD18CTRL_FCD_Pos     30           /**< \brief (BSCIF_BOD18CTRL) BOD Fuse Calibration Done */
#define BSCIF_BOD18CTRL_FCD         (0x1u << BSCIF_BOD18CTRL_FCD_Pos)
#define BSCIF_BOD18CTRL_SFV_Pos     31           /**< \brief (BSCIF_BOD18CTRL) BOD Control Register Store Final Value */
#define BSCIF_BOD18CTRL_SFV         (0x1u << BSCIF_BOD18CTRL_SFV_Pos)
#define BSCIF_BOD18CTRL_MASK        0xC0010303u  /**< \brief (BSCIF_BOD18CTRL) MASK Register */

/* -------- BSCIF_BOD18LEVEL : (BSCIF Offset: 0x03C) (R/W 32) BOD18 Level Register -------- */
#define BSCIF_BOD18LEVEL_OFFSET     0x03C        /**< \brief (BSCIF_BOD18LEVEL offset) BOD18 Level Register */
#define BSCIF_BOD18LEVEL_RESETVALUE 0x00000000   /**< \brief (BSCIF_BOD18LEVEL reset_value) BOD18 Level Register */

#define BSCIF_BOD18LEVEL_VAL_Pos    0            /**< \brief (BSCIF_BOD18LEVEL) BOD Value */
#define BSCIF_BOD18LEVEL_VAL_Msk    (0x3Fu << BSCIF_BOD18LEVEL_VAL_Pos)
#define BSCIF_BOD18LEVEL_VAL(value) ((BSCIF_BOD18LEVEL_VAL_Msk & ((value) << BSCIF_BOD18LEVEL_VAL_Pos)))
#define BSCIF_BOD18LEVEL_RANGE_Pos  31           /**< \brief (BSCIF_BOD18LEVEL) BOD Threshold Range */
#define BSCIF_BOD18LEVEL_RANGE      (0x1u << BSCIF_BOD18LEVEL_RANGE_Pos)
#define BSCIF_BOD18LEVEL_MASK       0x8000003Fu  /**< \brief (BSCIF_BOD18LEVEL) MASK Register */

/* -------- BSCIF_VREGCR : (BSCIF Offset: 0x044) (R/W 32) Voltage Regulator Configuration Register -------- */
#define BSCIF_VREGCR_OFFSET         0x044        /**< \brief (BSCIF_VREGCR offset) Voltage Regulator Configuration Register */
#define BSCIF_VREGCR_RESETVALUE     0x00000000   /**< \brief (BSCIF_VREGCR reset_value) Voltage Regulator Configuration Register */

#define BSCIF_VREGCR_DIS_Pos        0            /**< \brief (BSCIF_VREGCR) Voltage Regulator disable */
#define BSCIF_VREGCR_DIS            (0x1u << BSCIF_VREGCR_DIS_Pos)
#define BSCIF_VREGCR_SSG_Pos        8            /**< \brief (BSCIF_VREGCR) Spread Spectrum Generator Enable */
#define BSCIF_VREGCR_SSG            (0x1u << BSCIF_VREGCR_SSG_Pos)
#define BSCIF_VREGCR_SSW_Pos        9            /**< \brief (BSCIF_VREGCR) Stop Switching */
#define BSCIF_VREGCR_SSW            (0x1u << BSCIF_VREGCR_SSW_Pos)
#define BSCIF_VREGCR_SSWEVT_Pos     10           /**< \brief (BSCIF_VREGCR) Stop Switching On Event Enable */
#define BSCIF_VREGCR_SSWEVT         (0x1u << BSCIF_VREGCR_SSWEVT_Pos)
#define BSCIF_VREGCR_SFV_Pos        31           /**< \brief (BSCIF_VREGCR) Store Final Value */
#define BSCIF_VREGCR_SFV            (0x1u << BSCIF_VREGCR_SFV_Pos)
#define BSCIF_VREGCR_MASK           0x80000701u  /**< \brief (BSCIF_VREGCR) MASK Register */

/* -------- BSCIF_VREGNCSR : (BSCIF Offset: 0x04C) (R/W 32) Normal Mode Control and Status Register -------- */
#define BSCIF_VREGNCSR_OFFSET       0x04C        /**< \brief (BSCIF_VREGNCSR offset) Normal Mode Control and Status Register */
#define BSCIF_VREGNCSR_RESETVALUE   0x00000000   /**< \brief (BSCIF_VREGNCSR reset_value) Normal Mode Control and Status Register */

#define BSCIF_VREGNCSR_MASK         0x00000000u  /**< \brief (BSCIF_VREGNCSR) MASK Register */

/* -------- BSCIF_VREGLPCSR : (BSCIF Offset: 0x050) (R/W 32) LP Mode Control and Status Register -------- */
#define BSCIF_VREGLPCSR_OFFSET      0x050        /**< \brief (BSCIF_VREGLPCSR offset) LP Mode Control and Status Register */
#define BSCIF_VREGLPCSR_RESETVALUE  0x00000000   /**< \brief (BSCIF_VREGLPCSR reset_value) LP Mode Control and Status Register */

#define BSCIF_VREGLPCSR_MASK        0x00000000u  /**< \brief (BSCIF_VREGLPCSR) MASK Register */

/* -------- BSCIF_RC1MCR : (BSCIF Offset: 0x058) (R/W 32) 1MHz RC Clock Configuration Register -------- */
#define BSCIF_RC1MCR_OFFSET         0x058        /**< \brief (BSCIF_RC1MCR offset) 1MHz RC Clock Configuration Register */
#define BSCIF_RC1MCR_RESETVALUE     0x00000F00   /**< \brief (BSCIF_RC1MCR reset_value) 1MHz RC Clock Configuration Register */

#define BSCIF_RC1MCR_CLKOE_Pos      0            /**< \brief (BSCIF_RC1MCR) 1MHz RC Osc Clock Output Enable */
#define BSCIF_RC1MCR_CLKOE          (0x1u << BSCIF_RC1MCR_CLKOE_Pos)
#define BSCIF_RC1MCR_FCD_Pos        7            /**< \brief (BSCIF_RC1MCR) Flash Calibration Done */
#define BSCIF_RC1MCR_FCD            (0x1u << BSCIF_RC1MCR_FCD_Pos)
#define BSCIF_RC1MCR_CLKCAL_Pos     8            /**< \brief (BSCIF_RC1MCR) 1MHz RC Osc Calibration */
#define BSCIF_RC1MCR_CLKCAL_Msk     (0x1Fu << BSCIF_RC1MCR_CLKCAL_Pos)
#define BSCIF_RC1MCR_CLKCAL(value)  ((BSCIF_RC1MCR_CLKCAL_Msk & ((value) << BSCIF_RC1MCR_CLKCAL_Pos)))
#define BSCIF_RC1MCR_MASK           0x00001F81u  /**< \brief (BSCIF_RC1MCR) MASK Register */

/* -------- BSCIF_BGCR : (BSCIF Offset: 0x05C) (R/W 32) Bandgap Calibration Register -------- */
#define BSCIF_BGCR_OFFSET           0x05C        /**< \brief (BSCIF_BGCR offset) Bandgap Calibration Register */
#define BSCIF_BGCR_RESETVALUE       0x00000000   /**< \brief (BSCIF_BGCR reset_value) Bandgap Calibration Register */

#define BSCIF_BGCR_MASK             0x00000000u  /**< \brief (BSCIF_BGCR) MASK Register */

/* -------- BSCIF_BGCTRL : (BSCIF Offset: 0x060) (R/W 32) Bandgap Control Register -------- */
#define BSCIF_BGCTRL_OFFSET         0x060        /**< \brief (BSCIF_BGCTRL offset) Bandgap Control Register */
#define BSCIF_BGCTRL_RESETVALUE     0x00000000   /**< \brief (BSCIF_BGCTRL reset_value) Bandgap Control Register */

#define BSCIF_BGCTRL_ADCISEL_Pos    0            /**< \brief (BSCIF_BGCTRL) ADC Input Selection */
#define BSCIF_BGCTRL_ADCISEL_Msk    (0x3u << BSCIF_BGCTRL_ADCISEL_Pos)
#define BSCIF_BGCTRL_ADCISEL(value) ((BSCIF_BGCTRL_ADCISEL_Msk & ((value) << BSCIF_BGCTRL_ADCISEL_Pos)))
#define   BSCIF_BGCTRL_ADCISEL_DIS  (0x0u <<  0) /**< \brief (BSCIF_BGCTRL)  */
#define   BSCIF_BGCTRL_ADCISEL_VTEMP (0x1u <<  0) /**< \brief (BSCIF_BGCTRL)  */
#define   BSCIF_BGCTRL_ADCISEL_VREF (0x2u <<  0) /**< \brief (BSCIF_BGCTRL)  */
#define BSCIF_BGCTRL_TSEN_Pos       8            /**< \brief (BSCIF_BGCTRL) Temperature Sensor Enable */
#define BSCIF_BGCTRL_TSEN           (0x1u << BSCIF_BGCTRL_TSEN_Pos)
#define BSCIF_BGCTRL_MASK           0x00000103u  /**< \brief (BSCIF_BGCTRL) MASK Register */

/* -------- BSCIF_BGSR : (BSCIF Offset: 0x064) (R/  32) Bandgap Status Register -------- */
#define BSCIF_BGSR_OFFSET           0x064        /**< \brief (BSCIF_BGSR offset) Bandgap Status Register */
#define BSCIF_BGSR_RESETVALUE       0x00000000   /**< \brief (BSCIF_BGSR reset_value) Bandgap Status Register */

#define BSCIF_BGSR_BGBUFRDY_Pos     0            /**< \brief (BSCIF_BGSR) Bandgap Buffer Ready */
#define BSCIF_BGSR_BGBUFRDY_Msk     (0xFFu << BSCIF_BGSR_BGBUFRDY_Pos)
#define BSCIF_BGSR_BGBUFRDY(value)  ((BSCIF_BGSR_BGBUFRDY_Msk & ((value) << BSCIF_BGSR_BGBUFRDY_Pos)))
#define   BSCIF_BGSR_BGBUFRDY_FLASH (0x1u <<  0) /**< \brief (BSCIF_BGSR)  */
#define   BSCIF_BGSR_BGBUFRDY_PLL   (0x2u <<  0) /**< \brief (BSCIF_BGSR)  */
#define   BSCIF_BGSR_BGBUFRDY_VREG  (0x4u <<  0) /**< \brief (BSCIF_BGSR)  */
#define   BSCIF_BGSR_BGBUFRDY_BUFRR (0x8u <<  0) /**< \brief (BSCIF_BGSR)  */
#define   BSCIF_BGSR_BGBUFRDY_ADC   (0x10u <<  0) /**< \brief (BSCIF_BGSR)  */
#define   BSCIF_BGSR_BGBUFRDY_LCD   (0x20u <<  0) /**< \brief (BSCIF_BGSR)  */
#define BSCIF_BGSR_BGRDY_Pos        16           /**< \brief (BSCIF_BGSR) Bandgap Voltage Reference Ready */
#define BSCIF_BGSR_BGRDY            (0x1u << BSCIF_BGSR_BGRDY_Pos)
#define BSCIF_BGSR_LPBGRDY_Pos      17           /**< \brief (BSCIF_BGSR) Low Power Bandgap Voltage Reference Ready */
#define BSCIF_BGSR_LPBGRDY          (0x1u << BSCIF_BGSR_LPBGRDY_Pos)
#define BSCIF_BGSR_VREF_Pos         18           /**< \brief (BSCIF_BGSR) Voltage Reference Used by the System */
#define BSCIF_BGSR_VREF_Msk         (0x3u << BSCIF_BGSR_VREF_Pos)
#define BSCIF_BGSR_VREF(value)      ((BSCIF_BGSR_VREF_Msk & ((value) << BSCIF_BGSR_VREF_Pos)))
#define BSCIF_BGSR_MASK             0x000F00FFu  /**< \brief (BSCIF_BGSR) MASK Register */

/* -------- BSCIF_BR : (BSCIF Offset: 0x078) (R/W 32) br Backup Register -------- */
#define BSCIF_BR_OFFSET             0x078        /**< \brief (BSCIF_BR offset) Backup Register */
#define BSCIF_BR_RESETVALUE         0x00000000   /**< \brief (BSCIF_BR reset_value) Backup Register */
#define BSCIF_BR_MASK               0xFFFFFFFFu  /**< \brief (BSCIF_BR) MASK Register */

/* -------- BSCIF_BRIFBVERSION : (BSCIF Offset: 0x3E4) (R/  32) Backup Register Interface Version Register -------- */
#define BSCIF_BRIFBVERSION_OFFSET   0x3E4        /**< \brief (BSCIF_BRIFBVERSION offset) Backup Register Interface Version Register */
#define BSCIF_BRIFBVERSION_RESETVALUE 0x00000100   /**< \brief (BSCIF_BRIFBVERSION reset_value) Backup Register Interface Version Register */

#define BSCIF_BRIFBVERSION_VERSION_Pos 0            /**< \brief (BSCIF_BRIFBVERSION) Version Number */
#define BSCIF_BRIFBVERSION_VERSION_Msk (0xFFFu << BSCIF_BRIFBVERSION_VERSION_Pos)
#define BSCIF_BRIFBVERSION_VERSION(value) ((BSCIF_BRIFBVERSION_VERSION_Msk & ((value) << BSCIF_BRIFBVERSION_VERSION_Pos)))
#define BSCIF_BRIFBVERSION_VARIANT_Pos 16           /**< \brief (BSCIF_BRIFBVERSION) Variant Number */
#define BSCIF_BRIFBVERSION_VARIANT_Msk (0xFu << BSCIF_BRIFBVERSION_VARIANT_Pos)
#define BSCIF_BRIFBVERSION_VARIANT(value) ((BSCIF_BRIFBVERSION_VARIANT_Msk & ((value) << BSCIF_BRIFBVERSION_VARIANT_Pos)))
#define BSCIF_BRIFBVERSION_MASK     0x000F0FFFu  /**< \brief (BSCIF_BRIFBVERSION) MASK Register */

/* -------- BSCIF_BGREFIFBVERSION : (BSCIF Offset: 0x3E8) (R/  32) BGREFIFB Version Register -------- */
#define BSCIF_BGREFIFBVERSION_OFFSET 0x3E8        /**< \brief (BSCIF_BGREFIFBVERSION offset) BGREFIFB Version Register */
#define BSCIF_BGREFIFBVERSION_RESETVALUE 0x00000110   /**< \brief (BSCIF_BGREFIFBVERSION reset_value) BGREFIFB Version Register */

#define BSCIF_BGREFIFBVERSION_VERSION_Pos 0            /**< \brief (BSCIF_BGREFIFBVERSION) Version Number */
#define BSCIF_BGREFIFBVERSION_VERSION_Msk (0xFFFu << BSCIF_BGREFIFBVERSION_VERSION_Pos)
#define BSCIF_BGREFIFBVERSION_VERSION(value) ((BSCIF_BGREFIFBVERSION_VERSION_Msk & ((value) << BSCIF_BGREFIFBVERSION_VERSION_Pos)))
#define BSCIF_BGREFIFBVERSION_VARIANT_Pos 16           /**< \brief (BSCIF_BGREFIFBVERSION) Variant Number */
#define BSCIF_BGREFIFBVERSION_VARIANT_Msk (0xFu << BSCIF_BGREFIFBVERSION_VARIANT_Pos)
#define BSCIF_BGREFIFBVERSION_VARIANT(value) ((BSCIF_BGREFIFBVERSION_VARIANT_Msk & ((value) << BSCIF_BGREFIFBVERSION_VARIANT_Pos)))
#define BSCIF_BGREFIFBVERSION_MASK  0x000F0FFFu  /**< \brief (BSCIF_BGREFIFBVERSION) MASK Register */

/* -------- BSCIF_VREGIFGVERSION : (BSCIF Offset: 0x3EC) (R/  32) VREGIFA Version Register -------- */
#define BSCIF_VREGIFGVERSION_OFFSET 0x3EC        /**< \brief (BSCIF_VREGIFGVERSION offset) VREGIFA Version Register */
#define BSCIF_VREGIFGVERSION_RESETVALUE 0x00000110   /**< \brief (BSCIF_VREGIFGVERSION reset_value) VREGIFA Version Register */

#define BSCIF_VREGIFGVERSION_VERSION_Pos 0            /**< \brief (BSCIF_VREGIFGVERSION) Version Number */
#define BSCIF_VREGIFGVERSION_VERSION_Msk (0xFFFu << BSCIF_VREGIFGVERSION_VERSION_Pos)
#define BSCIF_VREGIFGVERSION_VERSION(value) ((BSCIF_VREGIFGVERSION_VERSION_Msk & ((value) << BSCIF_VREGIFGVERSION_VERSION_Pos)))
#define BSCIF_VREGIFGVERSION_VARIANT_Pos 16           /**< \brief (BSCIF_VREGIFGVERSION) Variant Number */
#define BSCIF_VREGIFGVERSION_VARIANT_Msk (0xFu << BSCIF_VREGIFGVERSION_VARIANT_Pos)
#define BSCIF_VREGIFGVERSION_VARIANT(value) ((BSCIF_VREGIFGVERSION_VARIANT_Msk & ((value) << BSCIF_VREGIFGVERSION_VARIANT_Pos)))
#define BSCIF_VREGIFGVERSION_MASK   0x000F0FFFu  /**< \brief (BSCIF_VREGIFGVERSION) MASK Register */

/* -------- BSCIF_BODIFCVERSION : (BSCIF Offset: 0x3F0) (R/  32) BODIFC Version Register -------- */
#define BSCIF_BODIFCVERSION_OFFSET  0x3F0        /**< \brief (BSCIF_BODIFCVERSION offset) BODIFC Version Register */
#define BSCIF_BODIFCVERSION_RESETVALUE 0x00000110   /**< \brief (BSCIF_BODIFCVERSION reset_value) BODIFC Version Register */

#define BSCIF_BODIFCVERSION_VERSION_Pos 0            /**< \brief (BSCIF_BODIFCVERSION) Version Number */
#define BSCIF_BODIFCVERSION_VERSION_Msk (0xFFFu << BSCIF_BODIFCVERSION_VERSION_Pos)
#define BSCIF_BODIFCVERSION_VERSION(value) ((BSCIF_BODIFCVERSION_VERSION_Msk & ((value) << BSCIF_BODIFCVERSION_VERSION_Pos)))
#define BSCIF_BODIFCVERSION_VARIANT_Pos 16           /**< \brief (BSCIF_BODIFCVERSION) Variant Number */
#define BSCIF_BODIFCVERSION_VARIANT_Msk (0xFu << BSCIF_BODIFCVERSION_VARIANT_Pos)
#define BSCIF_BODIFCVERSION_VARIANT(value) ((BSCIF_BODIFCVERSION_VARIANT_Msk & ((value) << BSCIF_BODIFCVERSION_VARIANT_Pos)))
#define BSCIF_BODIFCVERSION_MASK    0x000F0FFFu  /**< \brief (BSCIF_BODIFCVERSION) MASK Register */

/* -------- BSCIF_RC32KIFBVERSION : (BSCIF Offset: 0x3F4) (R/  32) 32 kHz RC Oscillator Version Register -------- */
#define BSCIF_RC32KIFBVERSION_OFFSET 0x3F4        /**< \brief (BSCIF_RC32KIFBVERSION offset) 32 kHz RC Oscillator Version Register */
#define BSCIF_RC32KIFBVERSION_RESETVALUE 0x00000100   /**< \brief (BSCIF_RC32KIFBVERSION reset_value) 32 kHz RC Oscillator Version Register */

#define BSCIF_RC32KIFBVERSION_VERSION_Pos 0            /**< \brief (BSCIF_RC32KIFBVERSION) Version number */
#define BSCIF_RC32KIFBVERSION_VERSION_Msk (0xFFFu << BSCIF_RC32KIFBVERSION_VERSION_Pos)
#define BSCIF_RC32KIFBVERSION_VERSION(value) ((BSCIF_RC32KIFBVERSION_VERSION_Msk & ((value) << BSCIF_RC32KIFBVERSION_VERSION_Pos)))
#define BSCIF_RC32KIFBVERSION_VARIANT_Pos 16           /**< \brief (BSCIF_RC32KIFBVERSION) Variant number */
#define BSCIF_RC32KIFBVERSION_VARIANT_Msk (0xFu << BSCIF_RC32KIFBVERSION_VARIANT_Pos)
#define BSCIF_RC32KIFBVERSION_VARIANT(value) ((BSCIF_RC32KIFBVERSION_VARIANT_Msk & ((value) << BSCIF_RC32KIFBVERSION_VARIANT_Pos)))
#define BSCIF_RC32KIFBVERSION_MASK  0x000F0FFFu  /**< \brief (BSCIF_RC32KIFBVERSION) MASK Register */

/* -------- BSCIF_OSC32IFAVERSION : (BSCIF Offset: 0x3F8) (R/  32) 32 KHz Oscillator Version Register -------- */
#define BSCIF_OSC32IFAVERSION_OFFSET 0x3F8        /**< \brief (BSCIF_OSC32IFAVERSION offset) 32 KHz Oscillator Version Register */
#define BSCIF_OSC32IFAVERSION_RESETVALUE 0x00000200   /**< \brief (BSCIF_OSC32IFAVERSION reset_value) 32 KHz Oscillator Version Register */

#define BSCIF_OSC32IFAVERSION_VERSION_Pos 0            /**< \brief (BSCIF_OSC32IFAVERSION) Version number */
#define BSCIF_OSC32IFAVERSION_VERSION_Msk (0xFFFu << BSCIF_OSC32IFAVERSION_VERSION_Pos)
#define BSCIF_OSC32IFAVERSION_VERSION(value) ((BSCIF_OSC32IFAVERSION_VERSION_Msk & ((value) << BSCIF_OSC32IFAVERSION_VERSION_Pos)))
#define BSCIF_OSC32IFAVERSION_VARIANT_Pos 16           /**< \brief (BSCIF_OSC32IFAVERSION) Variant nubmer */
#define BSCIF_OSC32IFAVERSION_VARIANT_Msk (0xFu << BSCIF_OSC32IFAVERSION_VARIANT_Pos)
#define BSCIF_OSC32IFAVERSION_VARIANT(value) ((BSCIF_OSC32IFAVERSION_VARIANT_Msk & ((value) << BSCIF_OSC32IFAVERSION_VARIANT_Pos)))
#define BSCIF_OSC32IFAVERSION_MASK  0x000F0FFFu  /**< \brief (BSCIF_OSC32IFAVERSION) MASK Register */

/* -------- BSCIF_VERSION : (BSCIF Offset: 0x3FC) (R/  32) BSCIF Version Register -------- */
#define BSCIF_VERSION_OFFSET        0x3FC        /**< \brief (BSCIF_VERSION offset) BSCIF Version Register */
#define BSCIF_VERSION_RESETVALUE    0x00000100   /**< \brief (BSCIF_VERSION reset_value) BSCIF Version Register */

#define BSCIF_VERSION_VERSION_Pos   0            /**< \brief (BSCIF_VERSION) Version Number */
#define BSCIF_VERSION_VERSION_Msk   (0xFFFu << BSCIF_VERSION_VERSION_Pos)
#define BSCIF_VERSION_VERSION(value) ((BSCIF_VERSION_VERSION_Msk & ((value) << BSCIF_VERSION_VERSION_Pos)))
#define BSCIF_VERSION_VARIANT_Pos   16           /**< \brief (BSCIF_VERSION) Variant Number */
#define BSCIF_VERSION_VARIANT_Msk   (0xFu << BSCIF_VERSION_VARIANT_Pos)
#define BSCIF_VERSION_VARIANT(value) ((BSCIF_VERSION_VARIANT_Msk & ((value) << BSCIF_VERSION_VARIANT_Pos)))
#define BSCIF_VERSION_MASK          0x000F0FFFu  /**< \brief (BSCIF_VERSION) MASK Register */

/** \brief BscifBr hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
  RwReg   BSCIF_BR;           /**< \brief (BSCIF Offset: 0x000) Backup Register */
} BscifBr;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/** \brief BSCIF hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
  WoReg   BSCIF_IER;          /**< \brief (BSCIF Offset: 0x000) Interrupt Enable Register */
  WoReg   BSCIF_IDR;          /**< \brief (BSCIF Offset: 0x004) Interrupt Disable Register */
  RoReg   BSCIF_IMR;          /**< \brief (BSCIF Offset: 0x008) Interrupt Mask Register */
  RoReg   BSCIF_ISR;          /**< \brief (BSCIF Offset: 0x00C) Interrupt Status Register */
  WoReg   BSCIF_ICR;          /**< \brief (BSCIF Offset: 0x010) Interrupt Clear Register */
  RoReg   BSCIF_PCLKSR;       /**< \brief (BSCIF Offset: 0x014) Power and Clocks Status Register */
  WoReg   BSCIF_UNLOCK;       /**< \brief (BSCIF Offset: 0x018) Unlock Register */
  RwReg   BSCIF_CSCR;         /**< \brief (BSCIF Offset: 0x01C) Chip Specific Configuration Register */
  RwReg   BSCIF_OSCCTRL32;    /**< \brief (BSCIF Offset: 0x020) Oscillator 32 Control Register */
  RwReg   BSCIF_RC32KCR;      /**< \brief (BSCIF Offset: 0x024) 32 kHz RC Oscillator Control Register */
  RwReg   BSCIF_RC32KTUNE;    /**< \brief (BSCIF Offset: 0x028) 32kHz RC Oscillator Tuning Register */
  RwReg   BSCIF_BOD33CTRL;    /**< \brief (BSCIF Offset: 0x02C) BOD33 Control Register */
  RwReg   BSCIF_BOD33LEVEL;   /**< \brief (BSCIF Offset: 0x030) BOD33 Level Register */
  RwReg   BSCIF_BOD33SAMPLING; /**< \brief (BSCIF Offset: 0x034) BOD33 Sampling Control Register */
  RwReg   BSCIF_BOD18CTRL;    /**< \brief (BSCIF Offset: 0x038) BOD18 Control Register */
  RwReg   BSCIF_BOD18LEVEL;   /**< \brief (BSCIF Offset: 0x03C) BOD18 Level Register */
  RoReg8  Reserved1[0x4];
  RwReg   BSCIF_VREGCR;       /**< \brief (BSCIF Offset: 0x044) Voltage Regulator Configuration Register */
  RoReg8  Reserved2[0x4];
  RwReg   BSCIF_VREGNCSR;     /**< \brief (BSCIF Offset: 0x04C) Normal Mode Control and Status Register */
  RwReg   BSCIF_VREGLPCSR;    /**< \brief (BSCIF Offset: 0x050) LP Mode Control and Status Register */
  RoReg8  Reserved3[0x4];
  RwReg   BSCIF_RC1MCR;       /**< \brief (BSCIF Offset: 0x058) 1MHz RC Clock Configuration Register */
  RwReg   BSCIF_BGCR;         /**< \brief (BSCIF Offset: 0x05C) Bandgap Calibration Register */
  RwReg   BSCIF_BGCTRL;       /**< \brief (BSCIF Offset: 0x060) Bandgap Control Register */
  RoReg   BSCIF_BGSR;         /**< \brief (BSCIF Offset: 0x064) Bandgap Status Register */
  RoReg8  Reserved4[0x10];
  BscifBr BSCIF_BR[4];        /**< \brief (BSCIF Offset: 0x078) BscifBr groups */
  RoReg8  Reserved5[0x35C];
  RoReg   BSCIF_BRIFBVERSION; /**< \brief (BSCIF Offset: 0x3E4) Backup Register Interface Version Register */
  RoReg   BSCIF_BGREFIFBVERSION; /**< \brief (BSCIF Offset: 0x3E8) BGREFIFB Version Register */
  RoReg   BSCIF_VREGIFGVERSION; /**< \brief (BSCIF Offset: 0x3EC) VREGIFA Version Register */
  RoReg   BSCIF_BODIFCVERSION; /**< \brief (BSCIF Offset: 0x3F0) BODIFC Version Register */
  RoReg   BSCIF_RC32KIFBVERSION; /**< \brief (BSCIF Offset: 0x3F4) 32 kHz RC Oscillator Version Register */
  RoReg   BSCIF_OSC32IFAVERSION; /**< \brief (BSCIF Offset: 0x3F8) 32 KHz Oscillator Version Register */
  RoReg   BSCIF_VERSION;      /**< \brief (BSCIF Offset: 0x3FC) BSCIF Version Register */
} Bscif;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/*@}*/

#endif /* _SAM4L_BSCIF_COMPONENT_ */
