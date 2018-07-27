/**
 * \file
 *
 * \brief Component description for FREQM
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

#ifndef _SAM4L_FREQM_COMPONENT_
#define _SAM4L_FREQM_COMPONENT_

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR FREQM */
/* ========================================================================== */
/** \addtogroup SAM4L_FREQM Frequency Meter */
/*@{*/

#define REV_FREQM                   0x311

/* -------- FREQM_CTRL : (FREQM Offset: 0x000) ( /W 32) Control register -------- */
#define FREQM_CTRL_OFFSET           0x000        /**< \brief (FREQM_CTRL offset) Control register */
#define FREQM_CTRL_RESETVALUE       0x00000000   /**< \brief (FREQM_CTRL reset_value) Control register */

#define FREQM_CTRL_START_Pos        0            /**< \brief (FREQM_CTRL) Start frequency measurement */
#define FREQM_CTRL_START            (0x1u << FREQM_CTRL_START_Pos)
#define FREQM_CTRL_MASK             0x00000001u  /**< \brief (FREQM_CTRL) MASK Register */

/* -------- FREQM_MODE : (FREQM Offset: 0x004) (R/W 32) Mode  register -------- */
#define FREQM_MODE_OFFSET           0x004        /**< \brief (FREQM_MODE offset) Mode  register */
#define FREQM_MODE_RESETVALUE       0x00000000   /**< \brief (FREQM_MODE reset_value) Mode  register */

#define FREQM_MODE_REFSEL_Pos       0            /**< \brief (FREQM_MODE) Reference Clock Selection */
#define FREQM_MODE_REFSEL_Msk       (0x3u << FREQM_MODE_REFSEL_Pos)
#define FREQM_MODE_REFSEL(value)    ((FREQM_MODE_REFSEL_Msk & ((value) << FREQM_MODE_REFSEL_Pos)))
#define FREQM_MODE_REFNUM_Pos       8            /**< \brief (FREQM_MODE) Number of Reference CLock Cycles */
#define FREQM_MODE_REFNUM_Msk       (0xFFu << FREQM_MODE_REFNUM_Pos)
#define FREQM_MODE_REFNUM(value)    ((FREQM_MODE_REFNUM_Msk & ((value) << FREQM_MODE_REFNUM_Pos)))
#define FREQM_MODE_CLKSEL_Pos       16           /**< \brief (FREQM_MODE) Clock Source Selection */
#define FREQM_MODE_CLKSEL_Msk       (0x1Fu << FREQM_MODE_CLKSEL_Pos)
#define FREQM_MODE_CLKSEL(value)    ((FREQM_MODE_CLKSEL_Msk & ((value) << FREQM_MODE_CLKSEL_Pos)))
#define FREQM_MODE_REFCEN_Pos       31           /**< \brief (FREQM_MODE) Reference Clock Enable */
#define FREQM_MODE_REFCEN           (0x1u << FREQM_MODE_REFCEN_Pos)
#define FREQM_MODE_MASK             0x801FFF03u  /**< \brief (FREQM_MODE) MASK Register */

/* -------- FREQM_STATUS : (FREQM Offset: 0x008) (R/  32) Status  register -------- */
#define FREQM_STATUS_OFFSET         0x008        /**< \brief (FREQM_STATUS offset) Status  register */
#define FREQM_STATUS_RESETVALUE     0x00000000   /**< \brief (FREQM_STATUS reset_value) Status  register */

#define FREQM_STATUS_BUSY_Pos       0            /**< \brief (FREQM_STATUS) Frequency measurement on-going */
#define FREQM_STATUS_BUSY           (0x1u << FREQM_STATUS_BUSY_Pos)
#define FREQM_STATUS_RCLKBUSY_Pos   1            /**< \brief (FREQM_STATUS) Reference Clock busy */
#define FREQM_STATUS_RCLKBUSY       (0x1u << FREQM_STATUS_RCLKBUSY_Pos)
#define FREQM_STATUS_MASK           0x00000003u  /**< \brief (FREQM_STATUS) MASK Register */

/* -------- FREQM_VALUE : (FREQM Offset: 0x00C) (R/  32) Value register -------- */
#define FREQM_VALUE_OFFSET          0x00C        /**< \brief (FREQM_VALUE offset) Value register */
#define FREQM_VALUE_RESETVALUE      0x00000000   /**< \brief (FREQM_VALUE reset_value) Value register */

#define FREQM_VALUE_VALUE_Pos       0            /**< \brief (FREQM_VALUE) Measured frequency */
#define FREQM_VALUE_VALUE_Msk       (0xFFFFFFu << FREQM_VALUE_VALUE_Pos)
#define FREQM_VALUE_VALUE(value)    ((FREQM_VALUE_VALUE_Msk & ((value) << FREQM_VALUE_VALUE_Pos)))
#define FREQM_VALUE_MASK            0x00FFFFFFu  /**< \brief (FREQM_VALUE) MASK Register */

/* -------- FREQM_IER : (FREQM Offset: 0x010) ( /W 32) Interrupt Enable Register -------- */
#define FREQM_IER_OFFSET            0x010        /**< \brief (FREQM_IER offset) Interrupt Enable Register */
#define FREQM_IER_RESETVALUE        0x00000000   /**< \brief (FREQM_IER reset_value) Interrupt Enable Register */

#define FREQM_IER_DONE_Pos          0            /**< \brief (FREQM_IER) Frequency measurment done */
#define FREQM_IER_DONE              (0x1u << FREQM_IER_DONE_Pos)
#define FREQM_IER_RCLKRDY_Pos       1            /**< \brief (FREQM_IER) Reference Clock ready */
#define FREQM_IER_RCLKRDY           (0x1u << FREQM_IER_RCLKRDY_Pos)
#define FREQM_IER_MASK              0x00000003u  /**< \brief (FREQM_IER) MASK Register */

/* -------- FREQM_IDR : (FREQM Offset: 0x014) ( /W 32) Interrupt Diable Register -------- */
#define FREQM_IDR_OFFSET            0x014        /**< \brief (FREQM_IDR offset) Interrupt Diable Register */
#define FREQM_IDR_RESETVALUE        0x00000000   /**< \brief (FREQM_IDR reset_value) Interrupt Diable Register */

#define FREQM_IDR_DONE_Pos          0            /**< \brief (FREQM_IDR) Frequency measurment done */
#define FREQM_IDR_DONE              (0x1u << FREQM_IDR_DONE_Pos)
#define FREQM_IDR_RCLKRDY_Pos       1            /**< \brief (FREQM_IDR) Reference Clock ready */
#define FREQM_IDR_RCLKRDY           (0x1u << FREQM_IDR_RCLKRDY_Pos)
#define FREQM_IDR_MASK              0x00000003u  /**< \brief (FREQM_IDR) MASK Register */

/* -------- FREQM_IMR : (FREQM Offset: 0x018) (R/  32) Interrupt Mask Register -------- */
#define FREQM_IMR_OFFSET            0x018        /**< \brief (FREQM_IMR offset) Interrupt Mask Register */
#define FREQM_IMR_RESETVALUE        0x00000000   /**< \brief (FREQM_IMR reset_value) Interrupt Mask Register */

#define FREQM_IMR_DONE_Pos          0            /**< \brief (FREQM_IMR) Frequency measurment done */
#define FREQM_IMR_DONE              (0x1u << FREQM_IMR_DONE_Pos)
#define FREQM_IMR_RCLKRDY_Pos       1            /**< \brief (FREQM_IMR) Reference Clock ready */
#define FREQM_IMR_RCLKRDY           (0x1u << FREQM_IMR_RCLKRDY_Pos)
#define FREQM_IMR_MASK              0x00000003u  /**< \brief (FREQM_IMR) MASK Register */

/* -------- FREQM_ISR : (FREQM Offset: 0x01C) (R/  32) Interrupt Status Register -------- */
#define FREQM_ISR_OFFSET            0x01C        /**< \brief (FREQM_ISR offset) Interrupt Status Register */
#define FREQM_ISR_RESETVALUE        0x00000000   /**< \brief (FREQM_ISR reset_value) Interrupt Status Register */

#define FREQM_ISR_DONE_Pos          0            /**< \brief (FREQM_ISR) Frequency measurment done */
#define FREQM_ISR_DONE              (0x1u << FREQM_ISR_DONE_Pos)
#define FREQM_ISR_RCLKRDY_Pos       1            /**< \brief (FREQM_ISR) Reference Clock ready */
#define FREQM_ISR_RCLKRDY           (0x1u << FREQM_ISR_RCLKRDY_Pos)
#define FREQM_ISR_MASK              0x00000003u  /**< \brief (FREQM_ISR) MASK Register */

/* -------- FREQM_ICR : (FREQM Offset: 0x020) ( /W 32) Interrupt Clear Register -------- */
#define FREQM_ICR_OFFSET            0x020        /**< \brief (FREQM_ICR offset) Interrupt Clear Register */
#define FREQM_ICR_RESETVALUE        0x00000000   /**< \brief (FREQM_ICR reset_value) Interrupt Clear Register */

#define FREQM_ICR_DONE_Pos          0            /**< \brief (FREQM_ICR) Frequency measurment done */
#define FREQM_ICR_DONE              (0x1u << FREQM_ICR_DONE_Pos)
#define FREQM_ICR_RCLKRDY_Pos       1            /**< \brief (FREQM_ICR) Reference Clock ready */
#define FREQM_ICR_RCLKRDY           (0x1u << FREQM_ICR_RCLKRDY_Pos)
#define FREQM_ICR_MASK              0x00000003u  /**< \brief (FREQM_ICR) MASK Register */

/* -------- FREQM_VERSION : (FREQM Offset: 0x3FC) (R/  32) Version Register -------- */
#define FREQM_VERSION_OFFSET        0x3FC        /**< \brief (FREQM_VERSION offset) Version Register */
#define FREQM_VERSION_RESETVALUE    0x00000311   /**< \brief (FREQM_VERSION reset_value) Version Register */

#define FREQM_VERSION_VERSION_Pos   0            /**< \brief (FREQM_VERSION) Version number */
#define FREQM_VERSION_VERSION_Msk   (0xFFFu << FREQM_VERSION_VERSION_Pos)
#define FREQM_VERSION_VERSION(value) ((FREQM_VERSION_VERSION_Msk & ((value) << FREQM_VERSION_VERSION_Pos)))
#define FREQM_VERSION_VARIANT_Pos   16           /**< \brief (FREQM_VERSION) Variant number */
#define FREQM_VERSION_VARIANT_Msk   (0xFu << FREQM_VERSION_VARIANT_Pos)
#define FREQM_VERSION_VARIANT(value) ((FREQM_VERSION_VARIANT_Msk & ((value) << FREQM_VERSION_VARIANT_Pos)))
#define FREQM_VERSION_MASK          0x000F0FFFu  /**< \brief (FREQM_VERSION) MASK Register */

/** \brief FREQM hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
  WoReg   FREQM_CTRL;         /**< \brief (FREQM Offset: 0x000) Control register */
  RwReg   FREQM_MODE;         /**< \brief (FREQM Offset: 0x004) Mode  register */
  RoReg   FREQM_STATUS;       /**< \brief (FREQM Offset: 0x008) Status  register */
  RoReg   FREQM_VALUE;        /**< \brief (FREQM Offset: 0x00C) Value register */
  WoReg   FREQM_IER;          /**< \brief (FREQM Offset: 0x010) Interrupt Enable Register */
  WoReg   FREQM_IDR;          /**< \brief (FREQM Offset: 0x014) Interrupt Diable Register */
  RoReg   FREQM_IMR;          /**< \brief (FREQM Offset: 0x018) Interrupt Mask Register */
  RoReg   FREQM_ISR;          /**< \brief (FREQM Offset: 0x01C) Interrupt Status Register */
  WoReg   FREQM_ICR;          /**< \brief (FREQM Offset: 0x020) Interrupt Clear Register */
  RoReg8  Reserved1[0x3D8];
  RoReg   FREQM_VERSION;      /**< \brief (FREQM Offset: 0x3FC) Version Register */
} Freqm;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/*@}*/

#endif /* _SAM4L_FREQM_COMPONENT_ */
