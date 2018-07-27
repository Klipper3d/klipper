/**
 * \file
 *
 * \brief Component description for TRNG
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

#ifndef _SAM4L_TRNG_COMPONENT_
#define _SAM4L_TRNG_COMPONENT_

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR TRNG */
/* ========================================================================== */
/** \addtogroup SAM4L_TRNG True Random Number Generator */
/*@{*/

#define REV_TRNG                    0x103

/* -------- TRNG_CR : (TRNG Offset: 0x00) ( /W 32) Control Register -------- */
#define TRNG_CR_OFFSET              0x00         /**< \brief (TRNG_CR offset) Control Register */
#define TRNG_CR_RESETVALUE          0x00000000   /**< \brief (TRNG_CR reset_value) Control Register */

#define TRNG_CR_ENABLE_Pos          0            /**< \brief (TRNG_CR) Enables the TRNG to provide random values */
#define TRNG_CR_ENABLE              (0x1u << TRNG_CR_ENABLE_Pos)
#define TRNG_CR_KEY_Pos             8            /**< \brief (TRNG_CR) Security Key */
#define TRNG_CR_KEY_Msk             (0xFFFFFFu << TRNG_CR_KEY_Pos)
#define TRNG_CR_KEY(value)          ((TRNG_CR_KEY_Msk & ((value) << TRNG_CR_KEY_Pos)))
#define TRNG_CR_MASK                0xFFFFFF01u  /**< \brief (TRNG_CR) MASK Register */

/* -------- TRNG_IER : (TRNG Offset: 0x10) ( /W 32) Interrupt Enable Register -------- */
#define TRNG_IER_OFFSET             0x10         /**< \brief (TRNG_IER offset) Interrupt Enable Register */
#define TRNG_IER_RESETVALUE         0x00000000   /**< \brief (TRNG_IER reset_value) Interrupt Enable Register */

#define TRNG_IER_DATRDY_Pos         0            /**< \brief (TRNG_IER) Data Ready Interrupt Enable */
#define TRNG_IER_DATRDY             (0x1u << TRNG_IER_DATRDY_Pos)
#define TRNG_IER_MASK               0x00000001u  /**< \brief (TRNG_IER) MASK Register */

/* -------- TRNG_IDR : (TRNG Offset: 0x14) ( /W 32) Interrupt Disable Register -------- */
#define TRNG_IDR_OFFSET             0x14         /**< \brief (TRNG_IDR offset) Interrupt Disable Register */
#define TRNG_IDR_RESETVALUE         0x00000000   /**< \brief (TRNG_IDR reset_value) Interrupt Disable Register */

#define TRNG_IDR_DATRDY_Pos         0            /**< \brief (TRNG_IDR) Data Ready Interrupt Disable */
#define TRNG_IDR_DATRDY             (0x1u << TRNG_IDR_DATRDY_Pos)
#define TRNG_IDR_MASK               0x00000001u  /**< \brief (TRNG_IDR) MASK Register */

/* -------- TRNG_IMR : (TRNG Offset: 0x18) (R/  32) Interrupt Mask Register -------- */
#define TRNG_IMR_OFFSET             0x18         /**< \brief (TRNG_IMR offset) Interrupt Mask Register */
#define TRNG_IMR_RESETVALUE         0x00000000   /**< \brief (TRNG_IMR reset_value) Interrupt Mask Register */

#define TRNG_IMR_DATRDY_Pos         0            /**< \brief (TRNG_IMR) Data Ready Interrupt Mask */
#define TRNG_IMR_DATRDY             (0x1u << TRNG_IMR_DATRDY_Pos)
#define TRNG_IMR_MASK               0x00000001u  /**< \brief (TRNG_IMR) MASK Register */

/* -------- TRNG_ISR : (TRNG Offset: 0x1C) (R/  32) Interrupt Status Register -------- */
#define TRNG_ISR_OFFSET             0x1C         /**< \brief (TRNG_ISR offset) Interrupt Status Register */
#define TRNG_ISR_RESETVALUE         0x00000000   /**< \brief (TRNG_ISR reset_value) Interrupt Status Register */

#define TRNG_ISR_DATRDY_Pos         0            /**< \brief (TRNG_ISR) Data Ready Interrupt Status */
#define TRNG_ISR_DATRDY             (0x1u << TRNG_ISR_DATRDY_Pos)
#define TRNG_ISR_MASK               0x00000001u  /**< \brief (TRNG_ISR) MASK Register */

/* -------- TRNG_ODATA : (TRNG Offset: 0x50) (R/  32) Output Data Register -------- */
#define TRNG_ODATA_OFFSET           0x50         /**< \brief (TRNG_ODATA offset) Output Data Register */
#define TRNG_ODATA_RESETVALUE       0x00000000   /**< \brief (TRNG_ODATA reset_value) Output Data Register */

#define TRNG_ODATA_ODATA_Pos        0            /**< \brief (TRNG_ODATA) Output Data */
#define TRNG_ODATA_ODATA            (0x1u << TRNG_ODATA_ODATA_Pos)
#define TRNG_ODATA_MASK             0x00000001u  /**< \brief (TRNG_ODATA) MASK Register */

/* -------- TRNG_VERSION : (TRNG Offset: 0xFC) (R/  32) Version Register -------- */
#define TRNG_VERSION_OFFSET         0xFC         /**< \brief (TRNG_VERSION offset) Version Register */
#define TRNG_VERSION_RESETVALUE     0x00000103   /**< \brief (TRNG_VERSION reset_value) Version Register */

#define TRNG_VERSION_VERSION_Pos    0            /**< \brief (TRNG_VERSION) Version Number */
#define TRNG_VERSION_VERSION_Msk    (0xFFFu << TRNG_VERSION_VERSION_Pos)
#define TRNG_VERSION_VERSION(value) ((TRNG_VERSION_VERSION_Msk & ((value) << TRNG_VERSION_VERSION_Pos)))
#define TRNG_VERSION_VARIANT_Pos    16           /**< \brief (TRNG_VERSION) Variant Number */
#define TRNG_VERSION_VARIANT_Msk    (0x7u << TRNG_VERSION_VARIANT_Pos)
#define TRNG_VERSION_VARIANT(value) ((TRNG_VERSION_VARIANT_Msk & ((value) << TRNG_VERSION_VARIANT_Pos)))
#define TRNG_VERSION_MASK           0x00070FFFu  /**< \brief (TRNG_VERSION) MASK Register */

/** \brief TRNG hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
  WoReg   TRNG_CR;            /**< \brief (TRNG Offset: 0x00) Control Register */
  RoReg8  Reserved1[0xC];
  WoReg   TRNG_IER;           /**< \brief (TRNG Offset: 0x10) Interrupt Enable Register */
  WoReg   TRNG_IDR;           /**< \brief (TRNG Offset: 0x14) Interrupt Disable Register */
  RoReg   TRNG_IMR;           /**< \brief (TRNG Offset: 0x18) Interrupt Mask Register */
  RoReg   TRNG_ISR;           /**< \brief (TRNG Offset: 0x1C) Interrupt Status Register */
  RoReg8  Reserved2[0x30];
  RoReg   TRNG_ODATA;         /**< \brief (TRNG Offset: 0x50) Output Data Register */
  RoReg8  Reserved3[0xA8];
  RoReg   TRNG_VERSION;       /**< \brief (TRNG Offset: 0xFC) Version Register */
} Trng;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/*@}*/

#endif /* _SAM4L_TRNG_COMPONENT_ */
