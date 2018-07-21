/**
 * \file
 *
 * \brief Component description for PARC
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

#ifndef _SAM4L_PARC_COMPONENT_
#define _SAM4L_PARC_COMPONENT_

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR PARC */
/* ========================================================================== */
/** \addtogroup SAM4L_PARC Parallel Capture */
/*@{*/

#define REV_PARC                    0x100

/* -------- PARC_CFG : (PARC Offset: 0x00) (R/W 32) Configuration Register -------- */
#define PARC_CFG_OFFSET             0x00         /**< \brief (PARC_CFG offset) Configuration Register */
#define PARC_CFG_RESETVALUE         0x00000000   /**< \brief (PARC_CFG reset_value) Configuration Register */

#define PARC_CFG_DSIZE_Pos          0            /**< \brief (PARC_CFG) Data Size */
#define PARC_CFG_DSIZE_Msk          (0x3u << PARC_CFG_DSIZE_Pos)
#define PARC_CFG_DSIZE(value)       ((PARC_CFG_DSIZE_Msk & ((value) << PARC_CFG_DSIZE_Pos)))
#define PARC_CFG_SMODE_Pos          2            /**< \brief (PARC_CFG) Sampling Mode */
#define PARC_CFG_SMODE_Msk          (0x3u << PARC_CFG_SMODE_Pos)
#define PARC_CFG_SMODE(value)       ((PARC_CFG_SMODE_Msk & ((value) << PARC_CFG_SMODE_Pos)))
#define PARC_CFG_EMODE_Pos          4            /**< \brief (PARC_CFG) Events Mode */
#define PARC_CFG_EMODE              (0x1u << PARC_CFG_EMODE_Pos)
#define PARC_CFG_EDGE_Pos           5            /**< \brief (PARC_CFG) Sampling Edge Select */
#define PARC_CFG_EDGE               (0x1u << PARC_CFG_EDGE_Pos)
#define PARC_CFG_HALF_Pos           6            /**< \brief (PARC_CFG) Half Capture */
#define PARC_CFG_HALF               (0x1u << PARC_CFG_HALF_Pos)
#define PARC_CFG_ODD_Pos            7            /**< \brief (PARC_CFG) Odd Capture */
#define PARC_CFG_ODD                (0x1u << PARC_CFG_ODD_Pos)
#define PARC_CFG_MASK               0x000000FFu  /**< \brief (PARC_CFG) MASK Register */

/* -------- PARC_CR : (PARC Offset: 0x04) (R/W 32) Control Register -------- */
#define PARC_CR_OFFSET              0x04         /**< \brief (PARC_CR offset) Control Register */
#define PARC_CR_RESETVALUE          0x00000000   /**< \brief (PARC_CR reset_value) Control Register */

#define PARC_CR_EN_Pos              0            /**< \brief (PARC_CR) Enable */
#define PARC_CR_EN                  (0x1u << PARC_CR_EN_Pos)
#define PARC_CR_DIS_Pos             1            /**< \brief (PARC_CR) Disable */
#define PARC_CR_DIS                 (0x1u << PARC_CR_DIS_Pos)
#define PARC_CR_START_Pos           2            /**< \brief (PARC_CR) Start Capture */
#define PARC_CR_START               (0x1u << PARC_CR_START_Pos)
#define PARC_CR_STOP_Pos            3            /**< \brief (PARC_CR) Stop Capture */
#define PARC_CR_STOP                (0x1u << PARC_CR_STOP_Pos)
#define PARC_CR_MASK                0x0000000Fu  /**< \brief (PARC_CR) MASK Register */

/* -------- PARC_IER : (PARC Offset: 0x08) ( /W 32) Interrupt Enable Register -------- */
#define PARC_IER_OFFSET             0x08         /**< \brief (PARC_IER offset) Interrupt Enable Register */
#define PARC_IER_RESETVALUE         0x00000000   /**< \brief (PARC_IER reset_value) Interrupt Enable Register */

#define PARC_IER_DRDY_Pos           2            /**< \brief (PARC_IER) Data Ready Interrupt Enable */
#define PARC_IER_DRDY               (0x1u << PARC_IER_DRDY_Pos)
#define PARC_IER_OVR_Pos            3            /**< \brief (PARC_IER) Overrun Interrupt Enable */
#define PARC_IER_OVR                (0x1u << PARC_IER_OVR_Pos)
#define PARC_IER_MASK               0x0000000Cu  /**< \brief (PARC_IER) MASK Register */

/* -------- PARC_IDR : (PARC Offset: 0x0C) ( /W 32) Interrupt Disable Register -------- */
#define PARC_IDR_OFFSET             0x0C         /**< \brief (PARC_IDR offset) Interrupt Disable Register */
#define PARC_IDR_RESETVALUE         0x00000000   /**< \brief (PARC_IDR reset_value) Interrupt Disable Register */

#define PARC_IDR_DRDY_Pos           2            /**< \brief (PARC_IDR) Data Ready Interrupt Disable */
#define PARC_IDR_DRDY               (0x1u << PARC_IDR_DRDY_Pos)
#define PARC_IDR_OVR_Pos            3            /**< \brief (PARC_IDR) Overrun Interrupt Disable */
#define PARC_IDR_OVR                (0x1u << PARC_IDR_OVR_Pos)
#define PARC_IDR_MASK               0x0000000Cu  /**< \brief (PARC_IDR) MASK Register */

/* -------- PARC_IMR : (PARC Offset: 0x10) (R/  32) Interrupt Mask Register -------- */
#define PARC_IMR_OFFSET             0x10         /**< \brief (PARC_IMR offset) Interrupt Mask Register */
#define PARC_IMR_RESETVALUE         0x00000000   /**< \brief (PARC_IMR reset_value) Interrupt Mask Register */

#define PARC_IMR_DRDY_Pos           2            /**< \brief (PARC_IMR) Data Ready Interrupt Mask */
#define PARC_IMR_DRDY               (0x1u << PARC_IMR_DRDY_Pos)
#define PARC_IMR_OVR_Pos            3            /**< \brief (PARC_IMR) Overrun Interrupt Mask */
#define PARC_IMR_OVR                (0x1u << PARC_IMR_OVR_Pos)
#define PARC_IMR_MASK               0x0000000Cu  /**< \brief (PARC_IMR) MASK Register */

/* -------- PARC_SR : (PARC Offset: 0x14) (R/  32) Status Register -------- */
#define PARC_SR_OFFSET              0x14         /**< \brief (PARC_SR offset) Status Register */
#define PARC_SR_RESETVALUE          0x00000000   /**< \brief (PARC_SR reset_value) Status Register */

#define PARC_SR_EN_Pos              0            /**< \brief (PARC_SR) Enable Status */
#define PARC_SR_EN                  (0x1u << PARC_SR_EN_Pos)
#define PARC_SR_CS_Pos              1            /**< \brief (PARC_SR) Capture Status */
#define PARC_SR_CS                  (0x1u << PARC_SR_CS_Pos)
#define PARC_SR_DRDY_Pos            2            /**< \brief (PARC_SR) Data Ready Interrupt Status */
#define PARC_SR_DRDY                (0x1u << PARC_SR_DRDY_Pos)
#define PARC_SR_OVR_Pos             3            /**< \brief (PARC_SR) Overrun Interrupt Status */
#define PARC_SR_OVR                 (0x1u << PARC_SR_OVR_Pos)
#define PARC_SR_MASK                0x0000000Fu  /**< \brief (PARC_SR) MASK Register */

/* -------- PARC_ICR : (PARC Offset: 0x18) ( /W 32) Interrupt Status Clear Register -------- */
#define PARC_ICR_OFFSET             0x18         /**< \brief (PARC_ICR offset) Interrupt Status Clear Register */
#define PARC_ICR_RESETVALUE         0x00000000   /**< \brief (PARC_ICR reset_value) Interrupt Status Clear Register */

#define PARC_ICR_DRDY_Pos           2            /**< \brief (PARC_ICR) Data Ready Interrupt Status Clear */
#define PARC_ICR_DRDY               (0x1u << PARC_ICR_DRDY_Pos)
#define PARC_ICR_OVR_Pos            3            /**< \brief (PARC_ICR) Overrun Interrupt Status Clear */
#define PARC_ICR_OVR                (0x1u << PARC_ICR_OVR_Pos)
#define PARC_ICR_MASK               0x0000000Cu  /**< \brief (PARC_ICR) MASK Register */

/* -------- PARC_RHR : (PARC Offset: 0x1C) (R/  32) Receive Holding Register -------- */
#define PARC_RHR_OFFSET             0x1C         /**< \brief (PARC_RHR offset) Receive Holding Register */
#define PARC_RHR_RESETVALUE         0x00000000   /**< \brief (PARC_RHR reset_value) Receive Holding Register */

#define PARC_RHR_CDATA_Pos          0            /**< \brief (PARC_RHR) Captured Data */
#define PARC_RHR_CDATA_Msk          (0xFFFFFFFFu << PARC_RHR_CDATA_Pos)
#define PARC_RHR_CDATA(value)       ((PARC_RHR_CDATA_Msk & ((value) << PARC_RHR_CDATA_Pos)))
#define PARC_RHR_MASK               0xFFFFFFFFu  /**< \brief (PARC_RHR) MASK Register */

/* -------- PARC_VERSION : (PARC Offset: 0x20) (R/  32) Version Register -------- */
#define PARC_VERSION_OFFSET         0x20         /**< \brief (PARC_VERSION offset) Version Register */
#define PARC_VERSION_RESETVALUE     0x00000100   /**< \brief (PARC_VERSION reset_value) Version Register */

#define PARC_VERSION_VERSION_Pos    0            /**< \brief (PARC_VERSION) Version Number */
#define PARC_VERSION_VERSION_Msk    (0xFFFu << PARC_VERSION_VERSION_Pos)
#define PARC_VERSION_VERSION(value) ((PARC_VERSION_VERSION_Msk & ((value) << PARC_VERSION_VERSION_Pos)))
#define PARC_VERSION_VARIANT_Pos    16           /**< \brief (PARC_VERSION) Variant Number */
#define PARC_VERSION_VARIANT_Msk    (0xFu << PARC_VERSION_VARIANT_Pos)
#define PARC_VERSION_VARIANT(value) ((PARC_VERSION_VARIANT_Msk & ((value) << PARC_VERSION_VARIANT_Pos)))
#define PARC_VERSION_MASK           0x000F0FFFu  /**< \brief (PARC_VERSION) MASK Register */

/** \brief PARC hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
  RwReg   PARC_CFG;           /**< \brief (PARC Offset: 0x00) Configuration Register */
  RwReg   PARC_CR;            /**< \brief (PARC Offset: 0x04) Control Register */
  WoReg   PARC_IER;           /**< \brief (PARC Offset: 0x08) Interrupt Enable Register */
  WoReg   PARC_IDR;           /**< \brief (PARC Offset: 0x0C) Interrupt Disable Register */
  RoReg   PARC_IMR;           /**< \brief (PARC Offset: 0x10) Interrupt Mask Register */
  RoReg   PARC_SR;            /**< \brief (PARC Offset: 0x14) Status Register */
  WoReg   PARC_ICR;           /**< \brief (PARC Offset: 0x18) Interrupt Status Clear Register */
  RoReg   PARC_RHR;           /**< \brief (PARC Offset: 0x1C) Receive Holding Register */
  RoReg   PARC_VERSION;       /**< \brief (PARC Offset: 0x20) Version Register */
} Parc;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/*@}*/

#endif /* _SAM4L_PARC_COMPONENT_ */
