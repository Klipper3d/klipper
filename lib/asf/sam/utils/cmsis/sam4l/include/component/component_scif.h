/**
 * \file
 *
 * \brief Component description for SCIF
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

#ifndef _SAM4L_SCIF_COMPONENT_
#define _SAM4L_SCIF_COMPONENT_

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR SCIF */
/* ========================================================================== */
/** \addtogroup SAM4L_SCIF System Control Interface */
/*@{*/

#define REV_SCIF                    0x130

/* -------- SCIF_IER : (SCIF Offset: 0x000) ( /W 32) Interrupt Enable Register -------- */
#define SCIF_IER_OFFSET             0x000        /**< \brief (SCIF_IER offset) Interrupt Enable Register */
#define SCIF_IER_RESETVALUE         0x00000000   /**< \brief (SCIF_IER reset_value) Interrupt Enable Register */

#define SCIF_IER_OSC0RDY_Pos        0            /**< \brief (SCIF_IER)  */
#define SCIF_IER_OSC0RDY            (0x1u << SCIF_IER_OSC0RDY_Pos)
#define SCIF_IER_DFLL0LOCKC_Pos     1            /**< \brief (SCIF_IER)  */
#define SCIF_IER_DFLL0LOCKC         (0x1u << SCIF_IER_DFLL0LOCKC_Pos)
#define SCIF_IER_DFLL0LOCKF_Pos     2            /**< \brief (SCIF_IER)  */
#define SCIF_IER_DFLL0LOCKF         (0x1u << SCIF_IER_DFLL0LOCKF_Pos)
#define SCIF_IER_DFLL0RDY_Pos       3            /**< \brief (SCIF_IER)  */
#define SCIF_IER_DFLL0RDY           (0x1u << SCIF_IER_DFLL0RDY_Pos)
#define SCIF_IER_DFLL0RCS_Pos       4            /**< \brief (SCIF_IER)  */
#define SCIF_IER_DFLL0RCS           (0x1u << SCIF_IER_DFLL0RCS_Pos)
#define SCIF_IER_DFLL0OOB_Pos       5            /**< \brief (SCIF_IER)  */
#define SCIF_IER_DFLL0OOB           (0x1u << SCIF_IER_DFLL0OOB_Pos)
#define SCIF_IER_PLL0LOCK_Pos       6            /**< \brief (SCIF_IER)  */
#define SCIF_IER_PLL0LOCK           (0x1u << SCIF_IER_PLL0LOCK_Pos)
#define SCIF_IER_PLL0LOCKLOST_Pos   7            /**< \brief (SCIF_IER)  */
#define SCIF_IER_PLL0LOCKLOST       (0x1u << SCIF_IER_PLL0LOCKLOST_Pos)
#define SCIF_IER_RCFASTLOCK_Pos     13           /**< \brief (SCIF_IER)  */
#define SCIF_IER_RCFASTLOCK         (0x1u << SCIF_IER_RCFASTLOCK_Pos)
#define SCIF_IER_RCFASTLOCKLOST_Pos 14           /**< \brief (SCIF_IER)  */
#define SCIF_IER_RCFASTLOCKLOST     (0x1u << SCIF_IER_RCFASTLOCKLOST_Pos)
#define SCIF_IER_AE_Pos             31           /**< \brief (SCIF_IER)  */
#define SCIF_IER_AE                 (0x1u << SCIF_IER_AE_Pos)
#define SCIF_IER_MASK               0x800060FFu  /**< \brief (SCIF_IER) MASK Register */

/* -------- SCIF_IDR : (SCIF Offset: 0x004) ( /W 32) Interrupt Disable Register -------- */
#define SCIF_IDR_OFFSET             0x004        /**< \brief (SCIF_IDR offset) Interrupt Disable Register */
#define SCIF_IDR_RESETVALUE         0x00000000   /**< \brief (SCIF_IDR reset_value) Interrupt Disable Register */

#define SCIF_IDR_OSC0RDY_Pos        0            /**< \brief (SCIF_IDR)  */
#define SCIF_IDR_OSC0RDY            (0x1u << SCIF_IDR_OSC0RDY_Pos)
#define SCIF_IDR_DFLL0LOCKC_Pos     1            /**< \brief (SCIF_IDR)  */
#define SCIF_IDR_DFLL0LOCKC         (0x1u << SCIF_IDR_DFLL0LOCKC_Pos)
#define SCIF_IDR_DFLL0LOCKF_Pos     2            /**< \brief (SCIF_IDR)  */
#define SCIF_IDR_DFLL0LOCKF         (0x1u << SCIF_IDR_DFLL0LOCKF_Pos)
#define SCIF_IDR_DFLL0RDY_Pos       3            /**< \brief (SCIF_IDR)  */
#define SCIF_IDR_DFLL0RDY           (0x1u << SCIF_IDR_DFLL0RDY_Pos)
#define SCIF_IDR_DFLL0RCS_Pos       4            /**< \brief (SCIF_IDR)  */
#define SCIF_IDR_DFLL0RCS           (0x1u << SCIF_IDR_DFLL0RCS_Pos)
#define SCIF_IDR_DFLL0OOB_Pos       5            /**< \brief (SCIF_IDR)  */
#define SCIF_IDR_DFLL0OOB           (0x1u << SCIF_IDR_DFLL0OOB_Pos)
#define SCIF_IDR_PLL0LOCK_Pos       6            /**< \brief (SCIF_IDR)  */
#define SCIF_IDR_PLL0LOCK           (0x1u << SCIF_IDR_PLL0LOCK_Pos)
#define SCIF_IDR_PLL0LOCKLOST_Pos   7            /**< \brief (SCIF_IDR)  */
#define SCIF_IDR_PLL0LOCKLOST       (0x1u << SCIF_IDR_PLL0LOCKLOST_Pos)
#define SCIF_IDR_RCFASTLOCK_Pos     13           /**< \brief (SCIF_IDR)  */
#define SCIF_IDR_RCFASTLOCK         (0x1u << SCIF_IDR_RCFASTLOCK_Pos)
#define SCIF_IDR_RCFASTLOCKLOST_Pos 14           /**< \brief (SCIF_IDR)  */
#define SCIF_IDR_RCFASTLOCKLOST     (0x1u << SCIF_IDR_RCFASTLOCKLOST_Pos)
#define SCIF_IDR_AE_Pos             31           /**< \brief (SCIF_IDR)  */
#define SCIF_IDR_AE                 (0x1u << SCIF_IDR_AE_Pos)
#define SCIF_IDR_MASK               0x800060FFu  /**< \brief (SCIF_IDR) MASK Register */

/* -------- SCIF_IMR : (SCIF Offset: 0x008) (R/  32) Interrupt Mask Register -------- */
#define SCIF_IMR_OFFSET             0x008        /**< \brief (SCIF_IMR offset) Interrupt Mask Register */
#define SCIF_IMR_RESETVALUE         0x00000000   /**< \brief (SCIF_IMR reset_value) Interrupt Mask Register */

#define SCIF_IMR_OSC0RDY_Pos        0            /**< \brief (SCIF_IMR)  */
#define SCIF_IMR_OSC0RDY            (0x1u << SCIF_IMR_OSC0RDY_Pos)
#define SCIF_IMR_DFLL0LOCKC_Pos     1            /**< \brief (SCIF_IMR)  */
#define SCIF_IMR_DFLL0LOCKC         (0x1u << SCIF_IMR_DFLL0LOCKC_Pos)
#define SCIF_IMR_DFLL0LOCKF_Pos     2            /**< \brief (SCIF_IMR)  */
#define SCIF_IMR_DFLL0LOCKF         (0x1u << SCIF_IMR_DFLL0LOCKF_Pos)
#define SCIF_IMR_DFLL0RDY_Pos       3            /**< \brief (SCIF_IMR)  */
#define SCIF_IMR_DFLL0RDY           (0x1u << SCIF_IMR_DFLL0RDY_Pos)
#define SCIF_IMR_DFLL0RCS_Pos       4            /**< \brief (SCIF_IMR)  */
#define SCIF_IMR_DFLL0RCS           (0x1u << SCIF_IMR_DFLL0RCS_Pos)
#define SCIF_IMR_DFLL0OOB_Pos       5            /**< \brief (SCIF_IMR)  */
#define SCIF_IMR_DFLL0OOB           (0x1u << SCIF_IMR_DFLL0OOB_Pos)
#define SCIF_IMR_PLL0LOCK_Pos       6            /**< \brief (SCIF_IMR)  */
#define SCIF_IMR_PLL0LOCK           (0x1u << SCIF_IMR_PLL0LOCK_Pos)
#define SCIF_IMR_PLL0LOCKLOST_Pos   7            /**< \brief (SCIF_IMR)  */
#define SCIF_IMR_PLL0LOCKLOST       (0x1u << SCIF_IMR_PLL0LOCKLOST_Pos)
#define SCIF_IMR_RCFASTLOCK_Pos     13           /**< \brief (SCIF_IMR)  */
#define SCIF_IMR_RCFASTLOCK         (0x1u << SCIF_IMR_RCFASTLOCK_Pos)
#define SCIF_IMR_RCFASTLOCKLOST_Pos 14           /**< \brief (SCIF_IMR)  */
#define SCIF_IMR_RCFASTLOCKLOST     (0x1u << SCIF_IMR_RCFASTLOCKLOST_Pos)
#define SCIF_IMR_AE_Pos             31           /**< \brief (SCIF_IMR)  */
#define SCIF_IMR_AE                 (0x1u << SCIF_IMR_AE_Pos)
#define SCIF_IMR_MASK               0x800060FFu  /**< \brief (SCIF_IMR) MASK Register */

/* -------- SCIF_ISR : (SCIF Offset: 0x00C) (R/  32) Interrupt Status Register -------- */
#define SCIF_ISR_OFFSET             0x00C        /**< \brief (SCIF_ISR offset) Interrupt Status Register */
#define SCIF_ISR_RESETVALUE         0x00000000   /**< \brief (SCIF_ISR reset_value) Interrupt Status Register */

#define SCIF_ISR_OSC0RDY_Pos        0            /**< \brief (SCIF_ISR)  */
#define SCIF_ISR_OSC0RDY            (0x1u << SCIF_ISR_OSC0RDY_Pos)
#define SCIF_ISR_DFLL0LOCKC_Pos     1            /**< \brief (SCIF_ISR)  */
#define SCIF_ISR_DFLL0LOCKC         (0x1u << SCIF_ISR_DFLL0LOCKC_Pos)
#define SCIF_ISR_DFLL0LOCKF_Pos     2            /**< \brief (SCIF_ISR)  */
#define SCIF_ISR_DFLL0LOCKF         (0x1u << SCIF_ISR_DFLL0LOCKF_Pos)
#define SCIF_ISR_DFLL0RDY_Pos       3            /**< \brief (SCIF_ISR)  */
#define SCIF_ISR_DFLL0RDY           (0x1u << SCIF_ISR_DFLL0RDY_Pos)
#define SCIF_ISR_DFLL0RCS_Pos       4            /**< \brief (SCIF_ISR)  */
#define SCIF_ISR_DFLL0RCS           (0x1u << SCIF_ISR_DFLL0RCS_Pos)
#define SCIF_ISR_DFLL0OOB_Pos       5            /**< \brief (SCIF_ISR)  */
#define SCIF_ISR_DFLL0OOB           (0x1u << SCIF_ISR_DFLL0OOB_Pos)
#define SCIF_ISR_PLL0LOCK_Pos       6            /**< \brief (SCIF_ISR)  */
#define SCIF_ISR_PLL0LOCK           (0x1u << SCIF_ISR_PLL0LOCK_Pos)
#define SCIF_ISR_PLL0LOCKLOST_Pos   7            /**< \brief (SCIF_ISR)  */
#define SCIF_ISR_PLL0LOCKLOST       (0x1u << SCIF_ISR_PLL0LOCKLOST_Pos)
#define SCIF_ISR_RCFASTLOCK_Pos     13           /**< \brief (SCIF_ISR)  */
#define SCIF_ISR_RCFASTLOCK         (0x1u << SCIF_ISR_RCFASTLOCK_Pos)
#define SCIF_ISR_RCFASTLOCKLOST_Pos 14           /**< \brief (SCIF_ISR)  */
#define SCIF_ISR_RCFASTLOCKLOST     (0x1u << SCIF_ISR_RCFASTLOCKLOST_Pos)
#define SCIF_ISR_AE_Pos             31           /**< \brief (SCIF_ISR)  */
#define SCIF_ISR_AE                 (0x1u << SCIF_ISR_AE_Pos)
#define SCIF_ISR_MASK               0x800060FFu  /**< \brief (SCIF_ISR) MASK Register */

/* -------- SCIF_ICR : (SCIF Offset: 0x010) ( /W 32) Interrupt Clear Register -------- */
#define SCIF_ICR_OFFSET             0x010        /**< \brief (SCIF_ICR offset) Interrupt Clear Register */
#define SCIF_ICR_RESETVALUE         0x00000000   /**< \brief (SCIF_ICR reset_value) Interrupt Clear Register */

#define SCIF_ICR_OSC0RDY_Pos        0            /**< \brief (SCIF_ICR)  */
#define SCIF_ICR_OSC0RDY            (0x1u << SCIF_ICR_OSC0RDY_Pos)
#define SCIF_ICR_DFLL0LOCKC_Pos     1            /**< \brief (SCIF_ICR)  */
#define SCIF_ICR_DFLL0LOCKC         (0x1u << SCIF_ICR_DFLL0LOCKC_Pos)
#define SCIF_ICR_DFLL0LOCKF_Pos     2            /**< \brief (SCIF_ICR)  */
#define SCIF_ICR_DFLL0LOCKF         (0x1u << SCIF_ICR_DFLL0LOCKF_Pos)
#define SCIF_ICR_DFLL0RDY_Pos       3            /**< \brief (SCIF_ICR)  */
#define SCIF_ICR_DFLL0RDY           (0x1u << SCIF_ICR_DFLL0RDY_Pos)
#define SCIF_ICR_DFLL0RCS_Pos       4            /**< \brief (SCIF_ICR)  */
#define SCIF_ICR_DFLL0RCS           (0x1u << SCIF_ICR_DFLL0RCS_Pos)
#define SCIF_ICR_DFLL0OOB_Pos       5            /**< \brief (SCIF_ICR)  */
#define SCIF_ICR_DFLL0OOB           (0x1u << SCIF_ICR_DFLL0OOB_Pos)
#define SCIF_ICR_PLL0LOCK_Pos       6            /**< \brief (SCIF_ICR)  */
#define SCIF_ICR_PLL0LOCK           (0x1u << SCIF_ICR_PLL0LOCK_Pos)
#define SCIF_ICR_PLL0LOCKLOST_Pos   7            /**< \brief (SCIF_ICR)  */
#define SCIF_ICR_PLL0LOCKLOST       (0x1u << SCIF_ICR_PLL0LOCKLOST_Pos)
#define SCIF_ICR_RCFASTLOCK_Pos     13           /**< \brief (SCIF_ICR)  */
#define SCIF_ICR_RCFASTLOCK         (0x1u << SCIF_ICR_RCFASTLOCK_Pos)
#define SCIF_ICR_RCFASTLOCKLOST_Pos 14           /**< \brief (SCIF_ICR)  */
#define SCIF_ICR_RCFASTLOCKLOST     (0x1u << SCIF_ICR_RCFASTLOCKLOST_Pos)
#define SCIF_ICR_AE_Pos             31           /**< \brief (SCIF_ICR)  */
#define SCIF_ICR_AE                 (0x1u << SCIF_ICR_AE_Pos)
#define SCIF_ICR_MASK               0x800060FFu  /**< \brief (SCIF_ICR) MASK Register */

/* -------- SCIF_PCLKSR : (SCIF Offset: 0x014) (R/  32) Power and Clocks Status Register -------- */
#define SCIF_PCLKSR_OFFSET          0x014        /**< \brief (SCIF_PCLKSR offset) Power and Clocks Status Register */
#define SCIF_PCLKSR_RESETVALUE      0x00000000   /**< \brief (SCIF_PCLKSR reset_value) Power and Clocks Status Register */

#define SCIF_PCLKSR_OSC0RDY_Pos     0            /**< \brief (SCIF_PCLKSR) OSC0 Ready */
#define SCIF_PCLKSR_OSC0RDY         (0x1u << SCIF_PCLKSR_OSC0RDY_Pos)
#define SCIF_PCLKSR_DFLL0LOCKC_Pos  1            /**< \brief (SCIF_PCLKSR) DFLL0 Locked on Coarse Value */
#define SCIF_PCLKSR_DFLL0LOCKC      (0x1u << SCIF_PCLKSR_DFLL0LOCKC_Pos)
#define SCIF_PCLKSR_DFLL0LOCKF_Pos  2            /**< \brief (SCIF_PCLKSR) DFLL0 Locked on Fine Value */
#define SCIF_PCLKSR_DFLL0LOCKF      (0x1u << SCIF_PCLKSR_DFLL0LOCKF_Pos)
#define SCIF_PCLKSR_DFLL0RDY_Pos    3            /**< \brief (SCIF_PCLKSR) DFLL0 Synchronization Ready */
#define SCIF_PCLKSR_DFLL0RDY        (0x1u << SCIF_PCLKSR_DFLL0RDY_Pos)
#define SCIF_PCLKSR_DFLL0RCS_Pos    4            /**< \brief (SCIF_PCLKSR) DFLL0 Reference Clock Stopped */
#define SCIF_PCLKSR_DFLL0RCS        (0x1u << SCIF_PCLKSR_DFLL0RCS_Pos)
#define SCIF_PCLKSR_DFLL0OOB_Pos    5            /**< \brief (SCIF_PCLKSR) DFLL0 Track Out Of Bounds */
#define SCIF_PCLKSR_DFLL0OOB        (0x1u << SCIF_PCLKSR_DFLL0OOB_Pos)
#define SCIF_PCLKSR_PLL0LOCK_Pos    6            /**< \brief (SCIF_PCLKSR) PLL0 Locked on Accurate value */
#define SCIF_PCLKSR_PLL0LOCK        (0x1u << SCIF_PCLKSR_PLL0LOCK_Pos)
#define SCIF_PCLKSR_PLL0LOCKLOST_Pos 7            /**< \brief (SCIF_PCLKSR) PLL0 lock lost value */
#define SCIF_PCLKSR_PLL0LOCKLOST    (0x1u << SCIF_PCLKSR_PLL0LOCKLOST_Pos)
#define SCIF_PCLKSR_RCFASTLOCK_Pos  13           /**< \brief (SCIF_PCLKSR) RCFAST Locked on Accurate value */
#define SCIF_PCLKSR_RCFASTLOCK      (0x1u << SCIF_PCLKSR_RCFASTLOCK_Pos)
#define SCIF_PCLKSR_RCFASTLOCKLOST_Pos 14           /**< \brief (SCIF_PCLKSR) RCFAST lock lost value */
#define SCIF_PCLKSR_RCFASTLOCKLOST  (0x1u << SCIF_PCLKSR_RCFASTLOCKLOST_Pos)
#define SCIF_PCLKSR_MASK            0x000060FFu  /**< \brief (SCIF_PCLKSR) MASK Register */

/* -------- SCIF_UNLOCK : (SCIF Offset: 0x018) ( /W 32) Unlock Register -------- */
#define SCIF_UNLOCK_OFFSET          0x018        /**< \brief (SCIF_UNLOCK offset) Unlock Register */
#define SCIF_UNLOCK_RESETVALUE      0x00000000   /**< \brief (SCIF_UNLOCK reset_value) Unlock Register */

#define SCIF_UNLOCK_ADDR_Pos        0            /**< \brief (SCIF_UNLOCK) Unlock Address */
#define SCIF_UNLOCK_ADDR_Msk        (0x3FFu << SCIF_UNLOCK_ADDR_Pos)
#define SCIF_UNLOCK_ADDR(value)     ((SCIF_UNLOCK_ADDR_Msk & ((value) << SCIF_UNLOCK_ADDR_Pos)))
#define SCIF_UNLOCK_KEY_Pos         24           /**< \brief (SCIF_UNLOCK) Unlock Key */
#define SCIF_UNLOCK_KEY_Msk         (0xFFu << SCIF_UNLOCK_KEY_Pos)
#define SCIF_UNLOCK_KEY(value)      ((SCIF_UNLOCK_KEY_Msk & ((value) << SCIF_UNLOCK_KEY_Pos)))
#define SCIF_UNLOCK_MASK            0xFF0003FFu  /**< \brief (SCIF_UNLOCK) MASK Register */

/* -------- SCIF_CSCR : (SCIF Offset: 0x01C) (R/W 32) Chip Specific Configuration Register -------- */
#define SCIF_CSCR_OFFSET            0x01C        /**< \brief (SCIF_CSCR offset) Chip Specific Configuration Register */
#define SCIF_CSCR_RESETVALUE        0x00000000   /**< \brief (SCIF_CSCR reset_value) Chip Specific Configuration Register */
#define SCIF_CSCR_MASK              0xFFFFFFFFu  /**< \brief (SCIF_CSCR) MASK Register */

/* -------- SCIF_OSCCTRL0 : (SCIF Offset: 0x020) (R/W 32) Oscillator Control Register -------- */
#define SCIF_OSCCTRL0_OFFSET        0x020        /**< \brief (SCIF_OSCCTRL0 offset) Oscillator Control Register */
#define SCIF_OSCCTRL0_RESETVALUE    0x00000000   /**< \brief (SCIF_OSCCTRL0 reset_value) Oscillator Control Register */

#define SCIF_OSCCTRL0_MODE_Pos      0            /**< \brief (SCIF_OSCCTRL0) Oscillator Mode */
#define SCIF_OSCCTRL0_MODE          (0x1u << SCIF_OSCCTRL0_MODE_Pos)
#define SCIF_OSCCTRL0_GAIN_Pos      1            /**< \brief (SCIF_OSCCTRL0) Gain */
#define SCIF_OSCCTRL0_GAIN_Msk      (0x3u << SCIF_OSCCTRL0_GAIN_Pos)
#define SCIF_OSCCTRL0_GAIN(value)   ((SCIF_OSCCTRL0_GAIN_Msk & ((value) << SCIF_OSCCTRL0_GAIN_Pos)))
#define SCIF_OSCCTRL0_AGC_Pos       3            /**< \brief (SCIF_OSCCTRL0) Automatic Gain Control */
#define SCIF_OSCCTRL0_AGC           (0x1u << SCIF_OSCCTRL0_AGC_Pos)
#define SCIF_OSCCTRL0_STARTUP_Pos   8            /**< \brief (SCIF_OSCCTRL0) Oscillator Start-up Time */
#define SCIF_OSCCTRL0_STARTUP_Msk   (0xFu << SCIF_OSCCTRL0_STARTUP_Pos)
#define SCIF_OSCCTRL0_STARTUP(value) ((SCIF_OSCCTRL0_STARTUP_Msk & ((value) << SCIF_OSCCTRL0_STARTUP_Pos)))
#define SCIF_OSCCTRL0_OSCEN_Pos     16           /**< \brief (SCIF_OSCCTRL0) Oscillator Enable */
#define SCIF_OSCCTRL0_OSCEN         (0x1u << SCIF_OSCCTRL0_OSCEN_Pos)
#define SCIF_OSCCTRL0_MASK          0x00010F0Fu  /**< \brief (SCIF_OSCCTRL0) MASK Register */

/* -------- SCIF_PLL : (SCIF Offset: 0x024) (R/W 32) pll PLL0 Control Register -------- */
#define SCIF_PLL_OFFSET             0x024        /**< \brief (SCIF_PLL offset) PLL0 Control Register */
#define SCIF_PLL_RESETVALUE         0x00000000   /**< \brief (SCIF_PLL reset_value) PLL0 Control Register */

#define SCIF_PLL_PLLEN_Pos          0            /**< \brief (SCIF_PLL) PLL Enable */
#define SCIF_PLL_PLLEN              (0x1u << SCIF_PLL_PLLEN_Pos)
#define SCIF_PLL_PLLOSC_Pos         1            /**< \brief (SCIF_PLL) PLL Oscillator Select */
#define SCIF_PLL_PLLOSC_Msk         (0x3u << SCIF_PLL_PLLOSC_Pos)
#define SCIF_PLL_PLLOSC(value)      ((SCIF_PLL_PLLOSC_Msk & ((value) << SCIF_PLL_PLLOSC_Pos)))
#define SCIF_PLL_PLLOPT_Pos         3            /**< \brief (SCIF_PLL) PLL Option */
#define SCIF_PLL_PLLOPT_Msk         (0x7u << SCIF_PLL_PLLOPT_Pos)
#define SCIF_PLL_PLLOPT(value)      ((SCIF_PLL_PLLOPT_Msk & ((value) << SCIF_PLL_PLLOPT_Pos)))
#define SCIF_PLL_PLLDIV_Pos         8            /**< \brief (SCIF_PLL) PLL Division Factor */
#define SCIF_PLL_PLLDIV_Msk         (0xFu << SCIF_PLL_PLLDIV_Pos)
#define SCIF_PLL_PLLDIV(value)      ((SCIF_PLL_PLLDIV_Msk & ((value) << SCIF_PLL_PLLDIV_Pos)))
#define SCIF_PLL_PLLMUL_Pos         16           /**< \brief (SCIF_PLL) PLL Multiply Factor */
#define SCIF_PLL_PLLMUL_Msk         (0xFu << SCIF_PLL_PLLMUL_Pos)
#define SCIF_PLL_PLLMUL(value)      ((SCIF_PLL_PLLMUL_Msk & ((value) << SCIF_PLL_PLLMUL_Pos)))
#define SCIF_PLL_PLLCOUNT_Pos       24           /**< \brief (SCIF_PLL) PLL Count */
#define SCIF_PLL_PLLCOUNT_Msk       (0x3Fu << SCIF_PLL_PLLCOUNT_Pos)
#define SCIF_PLL_PLLCOUNT(value)    ((SCIF_PLL_PLLCOUNT_Msk & ((value) << SCIF_PLL_PLLCOUNT_Pos)))
#define SCIF_PLL_MASK               0x3F0F0F3Fu  /**< \brief (SCIF_PLL) MASK Register */

/* -------- SCIF_DFLL0CONF : (SCIF Offset: 0x028) (R/W 32) DFLL0 Config Register -------- */
#define SCIF_DFLL0CONF_OFFSET       0x028        /**< \brief (SCIF_DFLL0CONF offset) DFLL0 Config Register */
#define SCIF_DFLL0CONF_RESETVALUE   0x00000000   /**< \brief (SCIF_DFLL0CONF reset_value) DFLL0 Config Register */

#define SCIF_DFLL0CONF_EN_Pos       0            /**< \brief (SCIF_DFLL0CONF) Enable */
#define SCIF_DFLL0CONF_EN           (0x1u << SCIF_DFLL0CONF_EN_Pos)
#define SCIF_DFLL0CONF_MODE_Pos     1            /**< \brief (SCIF_DFLL0CONF) Mode Selection */
#define SCIF_DFLL0CONF_MODE         (0x1u << SCIF_DFLL0CONF_MODE_Pos)
#define SCIF_DFLL0CONF_STABLE_Pos   2            /**< \brief (SCIF_DFLL0CONF) Stable DFLL Frequency */
#define SCIF_DFLL0CONF_STABLE       (0x1u << SCIF_DFLL0CONF_STABLE_Pos)
#define SCIF_DFLL0CONF_LLAW_Pos     3            /**< \brief (SCIF_DFLL0CONF) Lose Lock After Wake */
#define SCIF_DFLL0CONF_LLAW         (0x1u << SCIF_DFLL0CONF_LLAW_Pos)
#define SCIF_DFLL0CONF_CCDIS_Pos    5            /**< \brief (SCIF_DFLL0CONF) Chill Cycle Disable */
#define SCIF_DFLL0CONF_CCDIS        (0x1u << SCIF_DFLL0CONF_CCDIS_Pos)
#define SCIF_DFLL0CONF_QLDIS_Pos    6            /**< \brief (SCIF_DFLL0CONF) Quick Lock Disable */
#define SCIF_DFLL0CONF_QLDIS        (0x1u << SCIF_DFLL0CONF_QLDIS_Pos)
#define SCIF_DFLL0CONF_RANGE_Pos    16           /**< \brief (SCIF_DFLL0CONF) Range Value */
#define SCIF_DFLL0CONF_RANGE_Msk    (0x3u << SCIF_DFLL0CONF_RANGE_Pos)
#define SCIF_DFLL0CONF_RANGE(value) ((SCIF_DFLL0CONF_RANGE_Msk & ((value) << SCIF_DFLL0CONF_RANGE_Pos)))
#define SCIF_DFLL0CONF_FCD_Pos      23           /**< \brief (SCIF_DFLL0CONF) Fuse Calibration Done */
#define SCIF_DFLL0CONF_FCD          (0x1u << SCIF_DFLL0CONF_FCD_Pos)
#define SCIF_DFLL0CONF_CALIB_Pos    24           /**< \brief (SCIF_DFLL0CONF) Calibration Value */
#define SCIF_DFLL0CONF_CALIB_Msk    (0xFu << SCIF_DFLL0CONF_CALIB_Pos)
#define SCIF_DFLL0CONF_CALIB(value) ((SCIF_DFLL0CONF_CALIB_Msk & ((value) << SCIF_DFLL0CONF_CALIB_Pos)))
#define SCIF_DFLL0CONF_MASK         0x0F83006Fu  /**< \brief (SCIF_DFLL0CONF) MASK Register */

/* -------- SCIF_DFLL0VAL : (SCIF Offset: 0x02C) (R/W 32) DFLL Value Register -------- */
#define SCIF_DFLL0VAL_OFFSET        0x02C        /**< \brief (SCIF_DFLL0VAL offset) DFLL Value Register */
#define SCIF_DFLL0VAL_RESETVALUE    0x00000000   /**< \brief (SCIF_DFLL0VAL reset_value) DFLL Value Register */

#define SCIF_DFLL0VAL_FINE_Pos      0            /**< \brief (SCIF_DFLL0VAL) Fine Value */
#define SCIF_DFLL0VAL_FINE_Msk      (0xFFu << SCIF_DFLL0VAL_FINE_Pos)
#define SCIF_DFLL0VAL_FINE(value)   ((SCIF_DFLL0VAL_FINE_Msk & ((value) << SCIF_DFLL0VAL_FINE_Pos)))
#define SCIF_DFLL0VAL_COARSE_Pos    16           /**< \brief (SCIF_DFLL0VAL) Coarse Value */
#define SCIF_DFLL0VAL_COARSE_Msk    (0x1Fu << SCIF_DFLL0VAL_COARSE_Pos)
#define SCIF_DFLL0VAL_COARSE(value) ((SCIF_DFLL0VAL_COARSE_Msk & ((value) << SCIF_DFLL0VAL_COARSE_Pos)))
#define SCIF_DFLL0VAL_MASK          0x001F00FFu  /**< \brief (SCIF_DFLL0VAL) MASK Register */

/* -------- SCIF_DFLL0MUL : (SCIF Offset: 0x030) (R/W 32) DFLL0 Multiplier Register -------- */
#define SCIF_DFLL0MUL_OFFSET        0x030        /**< \brief (SCIF_DFLL0MUL offset) DFLL0 Multiplier Register */
#define SCIF_DFLL0MUL_RESETVALUE    0x00000000   /**< \brief (SCIF_DFLL0MUL reset_value) DFLL0 Multiplier Register */

#define SCIF_DFLL0MUL_MUL_Pos       0            /**< \brief (SCIF_DFLL0MUL) DFLL Multiply Factor */
#define SCIF_DFLL0MUL_MUL_Msk       (0xFFFFu << SCIF_DFLL0MUL_MUL_Pos)
#define SCIF_DFLL0MUL_MUL(value)    ((SCIF_DFLL0MUL_MUL_Msk & ((value) << SCIF_DFLL0MUL_MUL_Pos)))
#define SCIF_DFLL0MUL_MASK          0x0000FFFFu  /**< \brief (SCIF_DFLL0MUL) MASK Register */

/* -------- SCIF_DFLL0STEP : (SCIF Offset: 0x034) (R/W 32) DFLL0 Step Register -------- */
#define SCIF_DFLL0STEP_OFFSET       0x034        /**< \brief (SCIF_DFLL0STEP offset) DFLL0 Step Register */
#define SCIF_DFLL0STEP_RESETVALUE   0x00000000   /**< \brief (SCIF_DFLL0STEP reset_value) DFLL0 Step Register */

#define SCIF_DFLL0STEP_FSTEP_Pos    0            /**< \brief (SCIF_DFLL0STEP) Fine Maximum Step */
#define SCIF_DFLL0STEP_FSTEP_Msk    (0xFFu << SCIF_DFLL0STEP_FSTEP_Pos)
#define SCIF_DFLL0STEP_FSTEP(value) ((SCIF_DFLL0STEP_FSTEP_Msk & ((value) << SCIF_DFLL0STEP_FSTEP_Pos)))
#define SCIF_DFLL0STEP_CSTEP_Pos    16           /**< \brief (SCIF_DFLL0STEP) Coarse Maximum Step */
#define SCIF_DFLL0STEP_CSTEP_Msk    (0x1Fu << SCIF_DFLL0STEP_CSTEP_Pos)
#define SCIF_DFLL0STEP_CSTEP(value) ((SCIF_DFLL0STEP_CSTEP_Msk & ((value) << SCIF_DFLL0STEP_CSTEP_Pos)))
#define SCIF_DFLL0STEP_MASK         0x001F00FFu  /**< \brief (SCIF_DFLL0STEP) MASK Register */

/* -------- SCIF_DFLL0SSG : (SCIF Offset: 0x038) (R/W 32) DFLL0 Spread Spectrum Generator Control Register -------- */
#define SCIF_DFLL0SSG_OFFSET        0x038        /**< \brief (SCIF_DFLL0SSG offset) DFLL0 Spread Spectrum Generator Control Register */
#define SCIF_DFLL0SSG_RESETVALUE    0x00000000   /**< \brief (SCIF_DFLL0SSG reset_value) DFLL0 Spread Spectrum Generator Control Register */

#define SCIF_DFLL0SSG_EN_Pos        0            /**< \brief (SCIF_DFLL0SSG) Enable */
#define SCIF_DFLL0SSG_EN            (0x1u << SCIF_DFLL0SSG_EN_Pos)
#define SCIF_DFLL0SSG_PRBS_Pos      1            /**< \brief (SCIF_DFLL0SSG) Pseudo Random Bit Sequence */
#define SCIF_DFLL0SSG_PRBS          (0x1u << SCIF_DFLL0SSG_PRBS_Pos)
#define SCIF_DFLL0SSG_AMPLITUDE_Pos 8            /**< \brief (SCIF_DFLL0SSG) SSG Amplitude */
#define SCIF_DFLL0SSG_AMPLITUDE_Msk (0x1Fu << SCIF_DFLL0SSG_AMPLITUDE_Pos)
#define SCIF_DFLL0SSG_AMPLITUDE(value) ((SCIF_DFLL0SSG_AMPLITUDE_Msk & ((value) << SCIF_DFLL0SSG_AMPLITUDE_Pos)))
#define SCIF_DFLL0SSG_STEPSIZE_Pos  16           /**< \brief (SCIF_DFLL0SSG) SSG Step Size */
#define SCIF_DFLL0SSG_STEPSIZE_Msk  (0x1Fu << SCIF_DFLL0SSG_STEPSIZE_Pos)
#define SCIF_DFLL0SSG_STEPSIZE(value) ((SCIF_DFLL0SSG_STEPSIZE_Msk & ((value) << SCIF_DFLL0SSG_STEPSIZE_Pos)))
#define SCIF_DFLL0SSG_MASK          0x001F1F03u  /**< \brief (SCIF_DFLL0SSG) MASK Register */

/* -------- SCIF_DFLL0RATIO : (SCIF Offset: 0x03C) (R/  32) DFLL0 Ratio Registe -------- */
#define SCIF_DFLL0RATIO_OFFSET      0x03C        /**< \brief (SCIF_DFLL0RATIO offset) DFLL0 Ratio Registe */
#define SCIF_DFLL0RATIO_RESETVALUE  0x00000000   /**< \brief (SCIF_DFLL0RATIO reset_value) DFLL0 Ratio Registe */

#define SCIF_DFLL0RATIO_RATIODIFF_Pos 0            /**< \brief (SCIF_DFLL0RATIO) Multiplication Ratio Difference */
#define SCIF_DFLL0RATIO_RATIODIFF_Msk (0xFFFFu << SCIF_DFLL0RATIO_RATIODIFF_Pos)
#define SCIF_DFLL0RATIO_RATIODIFF(value) ((SCIF_DFLL0RATIO_RATIODIFF_Msk & ((value) << SCIF_DFLL0RATIO_RATIODIFF_Pos)))
#define SCIF_DFLL0RATIO_MASK        0x0000FFFFu  /**< \brief (SCIF_DFLL0RATIO) MASK Register */

/* -------- SCIF_DFLL0SYNC : (SCIF Offset: 0x040) ( /W 32) DFLL0 Synchronization Register -------- */
#define SCIF_DFLL0SYNC_OFFSET       0x040        /**< \brief (SCIF_DFLL0SYNC offset) DFLL0 Synchronization Register */
#define SCIF_DFLL0SYNC_RESETVALUE   0x00000000   /**< \brief (SCIF_DFLL0SYNC reset_value) DFLL0 Synchronization Register */

#define SCIF_DFLL0SYNC_SYNC_Pos     0            /**< \brief (SCIF_DFLL0SYNC) Synchronization */
#define SCIF_DFLL0SYNC_SYNC         (0x1u << SCIF_DFLL0SYNC_SYNC_Pos)
#define SCIF_DFLL0SYNC_MASK         0x00000001u  /**< \brief (SCIF_DFLL0SYNC) MASK Register */

/* -------- SCIF_RCCR : (SCIF Offset: 0x044) (R/W 32) System RC Oscillator Calibration Register -------- */
#define SCIF_RCCR_OFFSET            0x044        /**< \brief (SCIF_RCCR offset) System RC Oscillator Calibration Register */

#define SCIF_RCCR_CALIB_Pos         0            /**< \brief (SCIF_RCCR) Calibration Value */
#define SCIF_RCCR_CALIB_Msk         (0x3FFu << SCIF_RCCR_CALIB_Pos)
#define SCIF_RCCR_CALIB(value)      ((SCIF_RCCR_CALIB_Msk & ((value) << SCIF_RCCR_CALIB_Pos)))
#define SCIF_RCCR_FCD_Pos           16           /**< \brief (SCIF_RCCR) Flash Calibration Done */
#define SCIF_RCCR_FCD               (0x1u << SCIF_RCCR_FCD_Pos)
#define SCIF_RCCR_MASK              0x000103FFu  /**< \brief (SCIF_RCCR) MASK Register */

/* -------- SCIF_RCFASTCFG : (SCIF Offset: 0x048) (R/W 32) 4/8/12 MHz RC Oscillator Configuration Register -------- */
#define SCIF_RCFASTCFG_OFFSET       0x048        /**< \brief (SCIF_RCFASTCFG offset) 4/8/12 MHz RC Oscillator Configuration Register */
#define SCIF_RCFASTCFG_RESETVALUE   0x00000000   /**< \brief (SCIF_RCFASTCFG reset_value) 4/8/12 MHz RC Oscillator Configuration Register */

#define SCIF_RCFASTCFG_EN_Pos       0            /**< \brief (SCIF_RCFASTCFG) Oscillator Enable */
#define SCIF_RCFASTCFG_EN           (0x1u << SCIF_RCFASTCFG_EN_Pos)
#define SCIF_RCFASTCFG_TUNEEN_Pos   1            /**< \brief (SCIF_RCFASTCFG) Tuner Enable */
#define SCIF_RCFASTCFG_TUNEEN       (0x1u << SCIF_RCFASTCFG_TUNEEN_Pos)
#define SCIF_RCFASTCFG_JITMODE_Pos  2            /**< \brief (SCIF_RCFASTCFG) Jitter Mode */
#define SCIF_RCFASTCFG_JITMODE      (0x1u << SCIF_RCFASTCFG_JITMODE_Pos)
#define SCIF_RCFASTCFG_NBPERIODS_Pos 4            /**< \brief (SCIF_RCFASTCFG) Number of 32kHz Periods */
#define SCIF_RCFASTCFG_NBPERIODS_Msk (0x7u << SCIF_RCFASTCFG_NBPERIODS_Pos)
#define SCIF_RCFASTCFG_NBPERIODS(value) ((SCIF_RCFASTCFG_NBPERIODS_Msk & ((value) << SCIF_RCFASTCFG_NBPERIODS_Pos)))
#define SCIF_RCFASTCFG_FCD_Pos      7            /**< \brief (SCIF_RCFASTCFG) RCFAST Fuse Calibration Done */
#define SCIF_RCFASTCFG_FCD          (0x1u << SCIF_RCFASTCFG_FCD_Pos)
#define SCIF_RCFASTCFG_FRANGE_Pos   8            /**< \brief (SCIF_RCFASTCFG) Frequency Range */
#define SCIF_RCFASTCFG_FRANGE_Msk   (0x3u << SCIF_RCFASTCFG_FRANGE_Pos)
#define SCIF_RCFASTCFG_FRANGE(value) ((SCIF_RCFASTCFG_FRANGE_Msk & ((value) << SCIF_RCFASTCFG_FRANGE_Pos)))
#define SCIF_RCFASTCFG_LOCKMARGIN_Pos 12           /**< \brief (SCIF_RCFASTCFG) Accepted Count Error for Lock */
#define SCIF_RCFASTCFG_LOCKMARGIN_Msk (0xFu << SCIF_RCFASTCFG_LOCKMARGIN_Pos)
#define SCIF_RCFASTCFG_LOCKMARGIN(value) ((SCIF_RCFASTCFG_LOCKMARGIN_Msk & ((value) << SCIF_RCFASTCFG_LOCKMARGIN_Pos)))
#define SCIF_RCFASTCFG_CALIB_Pos    16           /**< \brief (SCIF_RCFASTCFG) Oscillator Calibration Value */
#define SCIF_RCFASTCFG_CALIB_Msk    (0x7Fu << SCIF_RCFASTCFG_CALIB_Pos)
#define SCIF_RCFASTCFG_CALIB(value) ((SCIF_RCFASTCFG_CALIB_Msk & ((value) << SCIF_RCFASTCFG_CALIB_Pos)))
#define SCIF_RCFASTCFG_MASK         0x007FF3F7u  /**< \brief (SCIF_RCFASTCFG) MASK Register */

/* -------- SCIF_RCFASTSR : (SCIF Offset: 0x04C) (R/W 32) 4/8/12 MHz RC Oscillator Status Register -------- */
#define SCIF_RCFASTSR_OFFSET        0x04C        /**< \brief (SCIF_RCFASTSR offset) 4/8/12 MHz RC Oscillator Status Register */
#define SCIF_RCFASTSR_RESETVALUE    0x00000000   /**< \brief (SCIF_RCFASTSR reset_value) 4/8/12 MHz RC Oscillator Status Register */

#define SCIF_RCFASTSR_CURTRIM_Pos   0            /**< \brief (SCIF_RCFASTSR) Current Trim Value */
#define SCIF_RCFASTSR_CURTRIM_Msk   (0x7Fu << SCIF_RCFASTSR_CURTRIM_Pos)
#define SCIF_RCFASTSR_CURTRIM(value) ((SCIF_RCFASTSR_CURTRIM_Msk & ((value) << SCIF_RCFASTSR_CURTRIM_Pos)))
#define SCIF_RCFASTSR_CNTERR_Pos    16           /**< \brief (SCIF_RCFASTSR) Current Count Error */
#define SCIF_RCFASTSR_CNTERR_Msk    (0x1Fu << SCIF_RCFASTSR_CNTERR_Pos)
#define SCIF_RCFASTSR_CNTERR(value) ((SCIF_RCFASTSR_CNTERR_Msk & ((value) << SCIF_RCFASTSR_CNTERR_Pos)))
#define SCIF_RCFASTSR_SIGN_Pos      21           /**< \brief (SCIF_RCFASTSR) Sign of Current Count Error */
#define SCIF_RCFASTSR_SIGN          (0x1u << SCIF_RCFASTSR_SIGN_Pos)
#define SCIF_RCFASTSR_LOCK_Pos      24           /**< \brief (SCIF_RCFASTSR) Lock */
#define SCIF_RCFASTSR_LOCK          (0x1u << SCIF_RCFASTSR_LOCK_Pos)
#define SCIF_RCFASTSR_LOCKLOST_Pos  25           /**< \brief (SCIF_RCFASTSR) Lock Lost */
#define SCIF_RCFASTSR_LOCKLOST      (0x1u << SCIF_RCFASTSR_LOCKLOST_Pos)
#define SCIF_RCFASTSR_UPDATED_Pos   31           /**< \brief (SCIF_RCFASTSR) Current Trim Value Updated */
#define SCIF_RCFASTSR_UPDATED       (0x1u << SCIF_RCFASTSR_UPDATED_Pos)
#define SCIF_RCFASTSR_MASK          0x833F007Fu  /**< \brief (SCIF_RCFASTSR) MASK Register */

/* -------- SCIF_RC80MCR : (SCIF Offset: 0x050) (R/W 32) 80 MHz RC Oscillator Register -------- */
#define SCIF_RC80MCR_OFFSET         0x050        /**< \brief (SCIF_RC80MCR offset) 80 MHz RC Oscillator Register */
#define SCIF_RC80MCR_RESETVALUE     0x00000000   /**< \brief (SCIF_RC80MCR reset_value) 80 MHz RC Oscillator Register */

#define SCIF_RC80MCR_EN_Pos         0            /**< \brief (SCIF_RC80MCR) Enable */
#define SCIF_RC80MCR_EN             (0x1u << SCIF_RC80MCR_EN_Pos)
#define SCIF_RC80MCR_FCD_Pos        7            /**< \brief (SCIF_RC80MCR) Flash Calibration Done */
#define SCIF_RC80MCR_FCD            (0x1u << SCIF_RC80MCR_FCD_Pos)
#define SCIF_RC80MCR_CALIB_Pos      16           /**< \brief (SCIF_RC80MCR) Calibration Value */
#define SCIF_RC80MCR_CALIB_Msk      (0x3u << SCIF_RC80MCR_CALIB_Pos)
#define SCIF_RC80MCR_CALIB(value)   ((SCIF_RC80MCR_CALIB_Msk & ((value) << SCIF_RC80MCR_CALIB_Pos)))
#define SCIF_RC80MCR_MASK           0x00030081u  /**< \brief (SCIF_RC80MCR) MASK Register */

/* -------- SCIF_HRPCR : (SCIF Offset: 0x064) (R/W 32) High Resolution Prescaler Control Register -------- */
#define SCIF_HRPCR_OFFSET           0x064        /**< \brief (SCIF_HRPCR offset) High Resolution Prescaler Control Register */

#define SCIF_HRPCR_HRPEN_Pos        0            /**< \brief (SCIF_HRPCR) High Resolution Prescaler Enable */
#define SCIF_HRPCR_HRPEN            (0x1u << SCIF_HRPCR_HRPEN_Pos)
#define SCIF_HRPCR_CKSEL_Pos        1            /**< \brief (SCIF_HRPCR) Clock Input Selection */
#define SCIF_HRPCR_CKSEL_Msk        (0x7u << SCIF_HRPCR_CKSEL_Pos)
#define SCIF_HRPCR_CKSEL(value)     ((SCIF_HRPCR_CKSEL_Msk & ((value) << SCIF_HRPCR_CKSEL_Pos)))
#define SCIF_HRPCR_HRCOUNT_Pos      8            /**< \brief (SCIF_HRPCR) High Resolution Counter */
#define SCIF_HRPCR_HRCOUNT_Msk      (0xFFFFFFu << SCIF_HRPCR_HRCOUNT_Pos)
#define SCIF_HRPCR_HRCOUNT(value)   ((SCIF_HRPCR_HRCOUNT_Msk & ((value) << SCIF_HRPCR_HRCOUNT_Pos)))
#define SCIF_HRPCR_MASK             0xFFFFFF0Fu  /**< \brief (SCIF_HRPCR) MASK Register */

/* -------- SCIF_FPCR : (SCIF Offset: 0x068) (R/W 32) Fractional Prescaler Control Register -------- */
#define SCIF_FPCR_OFFSET            0x068        /**< \brief (SCIF_FPCR offset) Fractional Prescaler Control Register */

#define SCIF_FPCR_FPEN_Pos          0            /**< \brief (SCIF_FPCR) High Resolution Prescaler Enable */
#define SCIF_FPCR_FPEN              (0x1u << SCIF_FPCR_FPEN_Pos)
#define SCIF_FPCR_CKSEL_Pos         1            /**< \brief (SCIF_FPCR) Clock Input Selection */
#define SCIF_FPCR_CKSEL_Msk         (0x7u << SCIF_FPCR_CKSEL_Pos)
#define SCIF_FPCR_CKSEL(value)      ((SCIF_FPCR_CKSEL_Msk & ((value) << SCIF_FPCR_CKSEL_Pos)))
#define SCIF_FPCR_MASK              0x0000000Fu  /**< \brief (SCIF_FPCR) MASK Register */

/* -------- SCIF_FPMUL : (SCIF Offset: 0x06C) (R/W 32) Fractional Prescaler Multiplier Register -------- */
#define SCIF_FPMUL_OFFSET           0x06C        /**< \brief (SCIF_FPMUL offset) Fractional Prescaler Multiplier Register */

#define SCIF_FPMUL_FPMUL_Pos        0            /**< \brief (SCIF_FPMUL) Fractional Prescaler Multiplication Factor */
#define SCIF_FPMUL_FPMUL_Msk        (0xFFFFu << SCIF_FPMUL_FPMUL_Pos)
#define SCIF_FPMUL_FPMUL(value)     ((SCIF_FPMUL_FPMUL_Msk & ((value) << SCIF_FPMUL_FPMUL_Pos)))
#define SCIF_FPMUL_MASK             0x0000FFFFu  /**< \brief (SCIF_FPMUL) MASK Register */

/* -------- SCIF_FPDIV : (SCIF Offset: 0x070) (R/W 32) Fractional Prescaler DIVIDER Register -------- */
#define SCIF_FPDIV_OFFSET           0x070        /**< \brief (SCIF_FPDIV offset) Fractional Prescaler DIVIDER Register */

#define SCIF_FPDIV_FPDIV_Pos        0            /**< \brief (SCIF_FPDIV) Fractional Prescaler Division Factor */
#define SCIF_FPDIV_FPDIV_Msk        (0xFFFFu << SCIF_FPDIV_FPDIV_Pos)
#define SCIF_FPDIV_FPDIV(value)     ((SCIF_FPDIV_FPDIV_Msk & ((value) << SCIF_FPDIV_FPDIV_Pos)))
#define SCIF_FPDIV_MASK             0x0000FFFFu  /**< \brief (SCIF_FPDIV) MASK Register */

/* -------- SCIF_GCCTRL : (SCIF Offset: 0x074) (R/W 32) gcctrl Generic Clock Control -------- */
#define SCIF_GCCTRL_OFFSET          0x074        /**< \brief (SCIF_GCCTRL offset) Generic Clock Control */
#define SCIF_GCCTRL_RESETVALUE      0x00000000   /**< \brief (SCIF_GCCTRL reset_value) Generic Clock Control */

#define SCIF_GCCTRL_CEN_Pos         0            /**< \brief (SCIF_GCCTRL) Clock Enable */
#define SCIF_GCCTRL_CEN             (0x1u << SCIF_GCCTRL_CEN_Pos)
#define SCIF_GCCTRL_DIVEN_Pos       1            /**< \brief (SCIF_GCCTRL) Divide Enable */
#define SCIF_GCCTRL_DIVEN           (0x1u << SCIF_GCCTRL_DIVEN_Pos)
#define SCIF_GCCTRL_OSCSEL_Pos      8            /**< \brief (SCIF_GCCTRL) Clock Select */
#define SCIF_GCCTRL_OSCSEL_Msk      (0x1Fu << SCIF_GCCTRL_OSCSEL_Pos)
#define SCIF_GCCTRL_OSCSEL(value)   ((SCIF_GCCTRL_OSCSEL_Msk & ((value) << SCIF_GCCTRL_OSCSEL_Pos)))
#define SCIF_GCCTRL_DIV_Pos         16           /**< \brief (SCIF_GCCTRL) Division Factor */
#define SCIF_GCCTRL_DIV_Msk         (0xFFFFu << SCIF_GCCTRL_DIV_Pos)
#define SCIF_GCCTRL_DIV(value)      ((SCIF_GCCTRL_DIV_Msk & ((value) << SCIF_GCCTRL_DIV_Pos)))
#define SCIF_GCCTRL_MASK            0xFFFF1F03u  /**< \brief (SCIF_GCCTRL) MASK Register */

/* -------- SCIF_RCFASTVERSION : (SCIF Offset: 0x3D8) (R/  32) 4/8/12 MHz RC Oscillator Version Register -------- */
#define SCIF_RCFASTVERSION_OFFSET   0x3D8        /**< \brief (SCIF_RCFASTVERSION offset) 4/8/12 MHz RC Oscillator Version Register */

#define SCIF_RCFASTVERSION_VERSION_Pos 0            /**< \brief (SCIF_RCFASTVERSION) Version number */
#define SCIF_RCFASTVERSION_VERSION_Msk (0xFFFu << SCIF_RCFASTVERSION_VERSION_Pos)
#define SCIF_RCFASTVERSION_VERSION(value) ((SCIF_RCFASTVERSION_VERSION_Msk & ((value) << SCIF_RCFASTVERSION_VERSION_Pos)))
#define SCIF_RCFASTVERSION_VARIANT_Pos 16           /**< \brief (SCIF_RCFASTVERSION) Variant number */
#define SCIF_RCFASTVERSION_VARIANT_Msk (0xFu << SCIF_RCFASTVERSION_VARIANT_Pos)
#define SCIF_RCFASTVERSION_VARIANT(value) ((SCIF_RCFASTVERSION_VARIANT_Msk & ((value) << SCIF_RCFASTVERSION_VARIANT_Pos)))
#define SCIF_RCFASTVERSION_MASK     0x000F0FFFu  /**< \brief (SCIF_RCFASTVERSION) MASK Register */

/* -------- SCIF_GCLKPRESCVERSION : (SCIF Offset: 0x3DC) (R/  32) Generic Clock Prescaler Version Register -------- */
#define SCIF_GCLKPRESCVERSION_OFFSET 0x3DC        /**< \brief (SCIF_GCLKPRESCVERSION offset) Generic Clock Prescaler Version Register */

#define SCIF_GCLKPRESCVERSION_VERSION_Pos 0            /**< \brief (SCIF_GCLKPRESCVERSION) Version number */
#define SCIF_GCLKPRESCVERSION_VERSION_Msk (0xFFFu << SCIF_GCLKPRESCVERSION_VERSION_Pos)
#define SCIF_GCLKPRESCVERSION_VERSION(value) ((SCIF_GCLKPRESCVERSION_VERSION_Msk & ((value) << SCIF_GCLKPRESCVERSION_VERSION_Pos)))
#define SCIF_GCLKPRESCVERSION_VARIANT_Pos 16           /**< \brief (SCIF_GCLKPRESCVERSION) Variant number */
#define SCIF_GCLKPRESCVERSION_VARIANT_Msk (0xFu << SCIF_GCLKPRESCVERSION_VARIANT_Pos)
#define SCIF_GCLKPRESCVERSION_VARIANT(value) ((SCIF_GCLKPRESCVERSION_VARIANT_Msk & ((value) << SCIF_GCLKPRESCVERSION_VARIANT_Pos)))
#define SCIF_GCLKPRESCVERSION_MASK  0x000F0FFFu  /**< \brief (SCIF_GCLKPRESCVERSION) MASK Register */

/* -------- SCIF_PLLIFAVERSION : (SCIF Offset: 0x3E0) (R/  32) PLL Version Register -------- */
#define SCIF_PLLIFAVERSION_OFFSET   0x3E0        /**< \brief (SCIF_PLLIFAVERSION offset) PLL Version Register */

#define SCIF_PLLIFAVERSION_VERSION_Pos 0            /**< \brief (SCIF_PLLIFAVERSION) Version number */
#define SCIF_PLLIFAVERSION_VERSION_Msk (0xFFFu << SCIF_PLLIFAVERSION_VERSION_Pos)
#define SCIF_PLLIFAVERSION_VERSION(value) ((SCIF_PLLIFAVERSION_VERSION_Msk & ((value) << SCIF_PLLIFAVERSION_VERSION_Pos)))
#define SCIF_PLLIFAVERSION_VARIANT_Pos 16           /**< \brief (SCIF_PLLIFAVERSION) Variant nubmer */
#define SCIF_PLLIFAVERSION_VARIANT_Msk (0xFu << SCIF_PLLIFAVERSION_VARIANT_Pos)
#define SCIF_PLLIFAVERSION_VARIANT(value) ((SCIF_PLLIFAVERSION_VARIANT_Msk & ((value) << SCIF_PLLIFAVERSION_VARIANT_Pos)))
#define SCIF_PLLIFAVERSION_MASK     0x000F0FFFu  /**< \brief (SCIF_PLLIFAVERSION) MASK Register */

/* -------- SCIF_OSCIFAVERSION : (SCIF Offset: 0x3E4) (R/  32) Oscillator 0 Version Register -------- */
#define SCIF_OSCIFAVERSION_OFFSET   0x3E4        /**< \brief (SCIF_OSCIFAVERSION offset) Oscillator 0 Version Register */

#define SCIF_OSCIFAVERSION_VERSION_Pos 0            /**< \brief (SCIF_OSCIFAVERSION) Version number */
#define SCIF_OSCIFAVERSION_VERSION_Msk (0xFFFu << SCIF_OSCIFAVERSION_VERSION_Pos)
#define SCIF_OSCIFAVERSION_VERSION(value) ((SCIF_OSCIFAVERSION_VERSION_Msk & ((value) << SCIF_OSCIFAVERSION_VERSION_Pos)))
#define SCIF_OSCIFAVERSION_VARIANT_Pos 16           /**< \brief (SCIF_OSCIFAVERSION) Variant nubmer */
#define SCIF_OSCIFAVERSION_VARIANT_Msk (0xFu << SCIF_OSCIFAVERSION_VARIANT_Pos)
#define SCIF_OSCIFAVERSION_VARIANT(value) ((SCIF_OSCIFAVERSION_VARIANT_Msk & ((value) << SCIF_OSCIFAVERSION_VARIANT_Pos)))
#define SCIF_OSCIFAVERSION_MASK     0x000F0FFFu  /**< \brief (SCIF_OSCIFAVERSION) MASK Register */

/* -------- SCIF_DFLLIFBVERSION : (SCIF Offset: 0x3E8) (R/  32) DFLL Version Register -------- */
#define SCIF_DFLLIFBVERSION_OFFSET  0x3E8        /**< \brief (SCIF_DFLLIFBVERSION offset) DFLL Version Register */

#define SCIF_DFLLIFBVERSION_VERSION_Pos 0            /**< \brief (SCIF_DFLLIFBVERSION) Version number */
#define SCIF_DFLLIFBVERSION_VERSION_Msk (0xFFFu << SCIF_DFLLIFBVERSION_VERSION_Pos)
#define SCIF_DFLLIFBVERSION_VERSION(value) ((SCIF_DFLLIFBVERSION_VERSION_Msk & ((value) << SCIF_DFLLIFBVERSION_VERSION_Pos)))
#define SCIF_DFLLIFBVERSION_VARIANT_Pos 16           /**< \brief (SCIF_DFLLIFBVERSION) Variant number */
#define SCIF_DFLLIFBVERSION_VARIANT_Msk (0xFu << SCIF_DFLLIFBVERSION_VARIANT_Pos)
#define SCIF_DFLLIFBVERSION_VARIANT(value) ((SCIF_DFLLIFBVERSION_VARIANT_Msk & ((value) << SCIF_DFLLIFBVERSION_VARIANT_Pos)))
#define SCIF_DFLLIFBVERSION_MASK    0x000F0FFFu  /**< \brief (SCIF_DFLLIFBVERSION) MASK Register */

/* -------- SCIF_RCOSCIFAVERSION : (SCIF Offset: 0x3EC) (R/  32) System RC Oscillator Version Register -------- */
#define SCIF_RCOSCIFAVERSION_OFFSET 0x3EC        /**< \brief (SCIF_RCOSCIFAVERSION offset) System RC Oscillator Version Register */

#define SCIF_RCOSCIFAVERSION_VERSION_Pos 0            /**< \brief (SCIF_RCOSCIFAVERSION) Version number */
#define SCIF_RCOSCIFAVERSION_VERSION_Msk (0xFFFu << SCIF_RCOSCIFAVERSION_VERSION_Pos)
#define SCIF_RCOSCIFAVERSION_VERSION(value) ((SCIF_RCOSCIFAVERSION_VERSION_Msk & ((value) << SCIF_RCOSCIFAVERSION_VERSION_Pos)))
#define SCIF_RCOSCIFAVERSION_VARIANT_Pos 16           /**< \brief (SCIF_RCOSCIFAVERSION) Variant number */
#define SCIF_RCOSCIFAVERSION_VARIANT_Msk (0xFu << SCIF_RCOSCIFAVERSION_VARIANT_Pos)
#define SCIF_RCOSCIFAVERSION_VARIANT(value) ((SCIF_RCOSCIFAVERSION_VARIANT_Msk & ((value) << SCIF_RCOSCIFAVERSION_VARIANT_Pos)))
#define SCIF_RCOSCIFAVERSION_MASK   0x000F0FFFu  /**< \brief (SCIF_RCOSCIFAVERSION) MASK Register */

/* -------- SCIF_FLOVERSION : (SCIF Offset: 0x3F0) (R/  32) Frequency Locked Oscillator Version Register -------- */
#define SCIF_FLOVERSION_OFFSET      0x3F0        /**< \brief (SCIF_FLOVERSION offset) Frequency Locked Oscillator Version Register */

#define SCIF_FLOVERSION_VERSION_Pos 0            /**< \brief (SCIF_FLOVERSION) Version number */
#define SCIF_FLOVERSION_VERSION_Msk (0xFFFu << SCIF_FLOVERSION_VERSION_Pos)
#define SCIF_FLOVERSION_VERSION(value) ((SCIF_FLOVERSION_VERSION_Msk & ((value) << SCIF_FLOVERSION_VERSION_Pos)))
#define SCIF_FLOVERSION_VARIANT_Pos 16           /**< \brief (SCIF_FLOVERSION) Variant number */
#define SCIF_FLOVERSION_VARIANT_Msk (0xFu << SCIF_FLOVERSION_VARIANT_Pos)
#define SCIF_FLOVERSION_VARIANT(value) ((SCIF_FLOVERSION_VARIANT_Msk & ((value) << SCIF_FLOVERSION_VARIANT_Pos)))
#define SCIF_FLOVERSION_MASK        0x000F0FFFu  /**< \brief (SCIF_FLOVERSION) MASK Register */

/* -------- SCIF_RC80MVERSION : (SCIF Offset: 0x3F4) (R/  32) 80MHz RC Oscillator Version Register -------- */
#define SCIF_RC80MVERSION_OFFSET    0x3F4        /**< \brief (SCIF_RC80MVERSION offset) 80MHz RC Oscillator Version Register */

#define SCIF_RC80MVERSION_VERSION_Pos 0            /**< \brief (SCIF_RC80MVERSION) Version number */
#define SCIF_RC80MVERSION_VERSION_Msk (0xFFFu << SCIF_RC80MVERSION_VERSION_Pos)
#define SCIF_RC80MVERSION_VERSION(value) ((SCIF_RC80MVERSION_VERSION_Msk & ((value) << SCIF_RC80MVERSION_VERSION_Pos)))
#define SCIF_RC80MVERSION_VARIANT_Pos 16           /**< \brief (SCIF_RC80MVERSION) Variant number */
#define SCIF_RC80MVERSION_VARIANT_Msk (0xFu << SCIF_RC80MVERSION_VARIANT_Pos)
#define SCIF_RC80MVERSION_VARIANT(value) ((SCIF_RC80MVERSION_VARIANT_Msk & ((value) << SCIF_RC80MVERSION_VARIANT_Pos)))
#define SCIF_RC80MVERSION_MASK      0x000F0FFFu  /**< \brief (SCIF_RC80MVERSION) MASK Register */

/* -------- SCIF_GCLKIFVERSION : (SCIF Offset: 0x3F8) (R/  32) Generic Clock Version Register -------- */
#define SCIF_GCLKIFVERSION_OFFSET   0x3F8        /**< \brief (SCIF_GCLKIFVERSION offset) Generic Clock Version Register */

#define SCIF_GCLKIFVERSION_VERSION_Pos 0            /**< \brief (SCIF_GCLKIFVERSION) Version number */
#define SCIF_GCLKIFVERSION_VERSION_Msk (0xFFFu << SCIF_GCLKIFVERSION_VERSION_Pos)
#define SCIF_GCLKIFVERSION_VERSION(value) ((SCIF_GCLKIFVERSION_VERSION_Msk & ((value) << SCIF_GCLKIFVERSION_VERSION_Pos)))
#define SCIF_GCLKIFVERSION_VARIANT_Pos 16           /**< \brief (SCIF_GCLKIFVERSION) Variant number */
#define SCIF_GCLKIFVERSION_VARIANT_Msk (0xFu << SCIF_GCLKIFVERSION_VARIANT_Pos)
#define SCIF_GCLKIFVERSION_VARIANT(value) ((SCIF_GCLKIFVERSION_VARIANT_Msk & ((value) << SCIF_GCLKIFVERSION_VARIANT_Pos)))
#define SCIF_GCLKIFVERSION_MASK     0x000F0FFFu  /**< \brief (SCIF_GCLKIFVERSION) MASK Register */

/* -------- SCIF_VERSION : (SCIF Offset: 0x3FC) (R/  32) SCIF Version Register -------- */
#define SCIF_VERSION_OFFSET         0x3FC        /**< \brief (SCIF_VERSION offset) SCIF Version Register */
#define SCIF_VERSION_RESETVALUE     0x00000130   /**< \brief (SCIF_VERSION reset_value) SCIF Version Register */

#define SCIF_VERSION_VERSION_Pos    0            /**< \brief (SCIF_VERSION) Version number */
#define SCIF_VERSION_VERSION_Msk    (0xFFFu << SCIF_VERSION_VERSION_Pos)
#define SCIF_VERSION_VERSION(value) ((SCIF_VERSION_VERSION_Msk & ((value) << SCIF_VERSION_VERSION_Pos)))
#define SCIF_VERSION_VARIANT_Pos    16           /**< \brief (SCIF_VERSION) Variant number */
#define SCIF_VERSION_VARIANT_Msk    (0xFu << SCIF_VERSION_VARIANT_Pos)
#define SCIF_VERSION_VARIANT(value) ((SCIF_VERSION_VARIANT_Msk & ((value) << SCIF_VERSION_VARIANT_Pos)))
#define SCIF_VERSION_MASK           0x000F0FFFu  /**< \brief (SCIF_VERSION) MASK Register */

/** \brief ScifGcctrl hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
  RwReg   SCIF_GCCTRL;        /**< \brief (SCIF Offset: 0x000) Generic Clock Control */
} ScifGcctrl;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/** \brief ScifPll hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
  RwReg   SCIF_PLL;           /**< \brief (SCIF Offset: 0x000) PLL0 Control Register */
} ScifPll;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/** \brief SCIF hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
  WoReg   SCIF_IER;           /**< \brief (SCIF Offset: 0x000) Interrupt Enable Register */
  WoReg   SCIF_IDR;           /**< \brief (SCIF Offset: 0x004) Interrupt Disable Register */
  RoReg   SCIF_IMR;           /**< \brief (SCIF Offset: 0x008) Interrupt Mask Register */
  RoReg   SCIF_ISR;           /**< \brief (SCIF Offset: 0x00C) Interrupt Status Register */
  WoReg   SCIF_ICR;           /**< \brief (SCIF Offset: 0x010) Interrupt Clear Register */
  RoReg   SCIF_PCLKSR;        /**< \brief (SCIF Offset: 0x014) Power and Clocks Status Register */
  WoReg   SCIF_UNLOCK;        /**< \brief (SCIF Offset: 0x018) Unlock Register */
  RwReg   SCIF_CSCR;          /**< \brief (SCIF Offset: 0x01C) Chip Specific Configuration Register */
  RwReg   SCIF_OSCCTRL0;      /**< \brief (SCIF Offset: 0x020) Oscillator Control Register */
  ScifPll SCIF_PLL[1];        /**< \brief (SCIF Offset: 0x024) ScifPll groups */
  RwReg   SCIF_DFLL0CONF;     /**< \brief (SCIF Offset: 0x028) DFLL0 Config Register */
  RwReg   SCIF_DFLL0VAL;      /**< \brief (SCIF Offset: 0x02C) DFLL Value Register */
  RwReg   SCIF_DFLL0MUL;      /**< \brief (SCIF Offset: 0x030) DFLL0 Multiplier Register */
  RwReg   SCIF_DFLL0STEP;     /**< \brief (SCIF Offset: 0x034) DFLL0 Step Register */
  RwReg   SCIF_DFLL0SSG;      /**< \brief (SCIF Offset: 0x038) DFLL0 Spread Spectrum Generator Control Register */
  RoReg   SCIF_DFLL0RATIO;    /**< \brief (SCIF Offset: 0x03C) DFLL0 Ratio Registe */
  WoReg   SCIF_DFLL0SYNC;     /**< \brief (SCIF Offset: 0x040) DFLL0 Synchronization Register */
  RwReg   SCIF_RCCR;          /**< \brief (SCIF Offset: 0x044) System RC Oscillator Calibration Register */
  RwReg   SCIF_RCFASTCFG;     /**< \brief (SCIF Offset: 0x048) 4/8/12 MHz RC Oscillator Configuration Register */
  RwReg   SCIF_RCFASTSR;      /**< \brief (SCIF Offset: 0x04C) 4/8/12 MHz RC Oscillator Status Register */
  RwReg   SCIF_RC80MCR;       /**< \brief (SCIF Offset: 0x050) 80 MHz RC Oscillator Register */
  RoReg8  Reserved1[0x10];
  RwReg   SCIF_HRPCR;         /**< \brief (SCIF Offset: 0x064) High Resolution Prescaler Control Register */
  RwReg   SCIF_FPCR;          /**< \brief (SCIF Offset: 0x068) Fractional Prescaler Control Register */
  RwReg   SCIF_FPMUL;         /**< \brief (SCIF Offset: 0x06C) Fractional Prescaler Multiplier Register */
  RwReg   SCIF_FPDIV;         /**< \brief (SCIF Offset: 0x070) Fractional Prescaler DIVIDER Register */
  ScifGcctrl SCIF_GCCTRL[12];    /**< \brief (SCIF Offset: 0x074) ScifGcctrl groups */
  RoReg8  Reserved2[0x334];
  RoReg   SCIF_RCFASTVERSION; /**< \brief (SCIF Offset: 0x3D8) 4/8/12 MHz RC Oscillator Version Register */
  RoReg   SCIF_GCLKPRESCVERSION; /**< \brief (SCIF Offset: 0x3DC) Generic Clock Prescaler Version Register */
  RoReg   SCIF_PLLIFAVERSION; /**< \brief (SCIF Offset: 0x3E0) PLL Version Register */
  RoReg   SCIF_OSCIFAVERSION; /**< \brief (SCIF Offset: 0x3E4) Oscillator 0 Version Register */
  RoReg   SCIF_DFLLIFBVERSION; /**< \brief (SCIF Offset: 0x3E8) DFLL Version Register */
  RoReg   SCIF_RCOSCIFAVERSION; /**< \brief (SCIF Offset: 0x3EC) System RC Oscillator Version Register */
  RoReg   SCIF_FLOVERSION;    /**< \brief (SCIF Offset: 0x3F0) Frequency Locked Oscillator Version Register */
  RoReg   SCIF_RC80MVERSION;  /**< \brief (SCIF Offset: 0x3F4) 80MHz RC Oscillator Version Register */
  RoReg   SCIF_GCLKIFVERSION; /**< \brief (SCIF Offset: 0x3F8) Generic Clock Version Register */
  RoReg   SCIF_VERSION;       /**< \brief (SCIF Offset: 0x3FC) SCIF Version Register */
} Scif;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/*@}*/

#endif /* _SAM4L_SCIF_COMPONENT_ */
