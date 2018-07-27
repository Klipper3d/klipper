/**
 * \file
 *
 * \brief Component description for CRCCU
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

#ifndef _SAM4L_CRCCU_COMPONENT_
#define _SAM4L_CRCCU_COMPONENT_

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR CRCCU */
/* ========================================================================== */
/** \addtogroup SAM4L_CRCCU CRC Calculation Unit */
/*@{*/

#define REV_CRCCU                   0x202

/* -------- CRCCU_DSCR : (CRCCU Offset: 0x00) (R/W 32) Descriptor Base Register -------- */
#define CRCCU_DSCR_OFFSET           0x00         /**< \brief (CRCCU_DSCR offset) Descriptor Base Register */
#define CRCCU_DSCR_RESETVALUE       0x00000000   /**< \brief (CRCCU_DSCR reset_value) Descriptor Base Register */

#define CRCCU_DSCR_DSCR_Pos         9            /**< \brief (CRCCU_DSCR) Description Base Address */
#define CRCCU_DSCR_DSCR_Msk         (0x7FFFFFu << CRCCU_DSCR_DSCR_Pos)
#define CRCCU_DSCR_DSCR(value)      ((CRCCU_DSCR_DSCR_Msk & ((value) << CRCCU_DSCR_DSCR_Pos)))
#define CRCCU_DSCR_MASK             0xFFFFFE00u  /**< \brief (CRCCU_DSCR) MASK Register */

/* -------- CRCCU_DMAEN : (CRCCU Offset: 0x08) ( /W 32) DMA Enable Register -------- */
#define CRCCU_DMAEN_OFFSET          0x08         /**< \brief (CRCCU_DMAEN offset) DMA Enable Register */
#define CRCCU_DMAEN_RESETVALUE      0x00000000   /**< \brief (CRCCU_DMAEN reset_value) DMA Enable Register */

#define CRCCU_DMAEN_DMAEN_Pos       0            /**< \brief (CRCCU_DMAEN) DMA Enable */
#define CRCCU_DMAEN_DMAEN           (0x1u << CRCCU_DMAEN_DMAEN_Pos)
#define CRCCU_DMAEN_MASK            0x00000001u  /**< \brief (CRCCU_DMAEN) MASK Register */

/* -------- CRCCU_DMADIS : (CRCCU Offset: 0x0C) ( /W 32) DMA Disable Register -------- */
#define CRCCU_DMADIS_OFFSET         0x0C         /**< \brief (CRCCU_DMADIS offset) DMA Disable Register */
#define CRCCU_DMADIS_RESETVALUE     0x00000000   /**< \brief (CRCCU_DMADIS reset_value) DMA Disable Register */

#define CRCCU_DMADIS_DMADIS_Pos     0            /**< \brief (CRCCU_DMADIS) DMA Disable */
#define CRCCU_DMADIS_DMADIS         (0x1u << CRCCU_DMADIS_DMADIS_Pos)
#define CRCCU_DMADIS_MASK           0x00000001u  /**< \brief (CRCCU_DMADIS) MASK Register */

/* -------- CRCCU_DMASR : (CRCCU Offset: 0x10) (R/  32) DMA Status Register -------- */
#define CRCCU_DMASR_OFFSET          0x10         /**< \brief (CRCCU_DMASR offset) DMA Status Register */
#define CRCCU_DMASR_RESETVALUE      0x00000000   /**< \brief (CRCCU_DMASR reset_value) DMA Status Register */

#define CRCCU_DMASR_DMASR_Pos       0            /**< \brief (CRCCU_DMASR) DMA Channel Status */
#define CRCCU_DMASR_DMASR           (0x1u << CRCCU_DMASR_DMASR_Pos)
#define CRCCU_DMASR_MASK            0x00000001u  /**< \brief (CRCCU_DMASR) MASK Register */

/* -------- CRCCU_DMAIER : (CRCCU Offset: 0x14) ( /W 32) DMA Interrupt Enable Register -------- */
#define CRCCU_DMAIER_OFFSET         0x14         /**< \brief (CRCCU_DMAIER offset) DMA Interrupt Enable Register */
#define CRCCU_DMAIER_RESETVALUE     0x00000000   /**< \brief (CRCCU_DMAIER reset_value) DMA Interrupt Enable Register */

#define CRCCU_DMAIER_DMAIER_Pos     0            /**< \brief (CRCCU_DMAIER) DMA Interrupt Enable */
#define CRCCU_DMAIER_DMAIER         (0x1u << CRCCU_DMAIER_DMAIER_Pos)
#define CRCCU_DMAIER_MASK           0x00000001u  /**< \brief (CRCCU_DMAIER) MASK Register */

/* -------- CRCCU_DMAIDR : (CRCCU Offset: 0x18) ( /W 32) DMA Interrupt Disable Register -------- */
#define CRCCU_DMAIDR_OFFSET         0x18         /**< \brief (CRCCU_DMAIDR offset) DMA Interrupt Disable Register */
#define CRCCU_DMAIDR_RESETVALUE     0x00000000   /**< \brief (CRCCU_DMAIDR reset_value) DMA Interrupt Disable Register */

#define CRCCU_DMAIDR_DMAIDR_Pos     0            /**< \brief (CRCCU_DMAIDR) DMA Interrupt Disable */
#define CRCCU_DMAIDR_DMAIDR         (0x1u << CRCCU_DMAIDR_DMAIDR_Pos)
#define CRCCU_DMAIDR_MASK           0x00000001u  /**< \brief (CRCCU_DMAIDR) MASK Register */

/* -------- CRCCU_DMAIMR : (CRCCU Offset: 0x1C) (R/  32) DMA Interrupt Mask Register -------- */
#define CRCCU_DMAIMR_OFFSET         0x1C         /**< \brief (CRCCU_DMAIMR offset) DMA Interrupt Mask Register */
#define CRCCU_DMAIMR_RESETVALUE     0x00000000   /**< \brief (CRCCU_DMAIMR reset_value) DMA Interrupt Mask Register */

#define CRCCU_DMAIMR_DMAIMR_Pos     0            /**< \brief (CRCCU_DMAIMR) DMA Interrupt Mask */
#define CRCCU_DMAIMR_DMAIMR         (0x1u << CRCCU_DMAIMR_DMAIMR_Pos)
#define CRCCU_DMAIMR_MASK           0x00000001u  /**< \brief (CRCCU_DMAIMR) MASK Register */

/* -------- CRCCU_DMAISR : (CRCCU Offset: 0x20) (R/  32) DMA Interrupt Status Register -------- */
#define CRCCU_DMAISR_OFFSET         0x20         /**< \brief (CRCCU_DMAISR offset) DMA Interrupt Status Register */
#define CRCCU_DMAISR_RESETVALUE     0x00000000   /**< \brief (CRCCU_DMAISR reset_value) DMA Interrupt Status Register */

#define CRCCU_DMAISR_DMAISR_Pos     0            /**< \brief (CRCCU_DMAISR) DMA Interrupt Status */
#define CRCCU_DMAISR_DMAISR         (0x1u << CRCCU_DMAISR_DMAISR_Pos)
#define CRCCU_DMAISR_MASK           0x00000001u  /**< \brief (CRCCU_DMAISR) MASK Register */

/* -------- CRCCU_CR : (CRCCU Offset: 0x34) ( /W 32) Control Register -------- */
#define CRCCU_CR_OFFSET             0x34         /**< \brief (CRCCU_CR offset) Control Register */
#define CRCCU_CR_RESETVALUE         0x00000000   /**< \brief (CRCCU_CR reset_value) Control Register */

#define CRCCU_CR_RESET_Pos          0            /**< \brief (CRCCU_CR) Reset CRCComputation */
#define CRCCU_CR_RESET              (0x1u << CRCCU_CR_RESET_Pos)
#define CRCCU_CR_MASK               0x00000001u  /**< \brief (CRCCU_CR) MASK Register */

/* -------- CRCCU_MR : (CRCCU Offset: 0x38) (R/W 32) Mode Register -------- */
#define CRCCU_MR_OFFSET             0x38         /**< \brief (CRCCU_MR offset) Mode Register */
#define CRCCU_MR_RESETVALUE         0x00000000   /**< \brief (CRCCU_MR reset_value) Mode Register */

#define CRCCU_MR_ENABLE_Pos         0            /**< \brief (CRCCU_MR) CRC Computation Enable */
#define CRCCU_MR_ENABLE             (0x1u << CRCCU_MR_ENABLE_Pos)
#define CRCCU_MR_COMPARE_Pos        1            /**< \brief (CRCCU_MR) CRC Compare */
#define CRCCU_MR_COMPARE            (0x1u << CRCCU_MR_COMPARE_Pos)
#define CRCCU_MR_PTYPE_Pos          2            /**< \brief (CRCCU_MR) Polynomial Type */
#define CRCCU_MR_PTYPE_Msk          (0x3u << CRCCU_MR_PTYPE_Pos)
#define CRCCU_MR_PTYPE(value)       ((CRCCU_MR_PTYPE_Msk & ((value) << CRCCU_MR_PTYPE_Pos)))
#define   CRCCU_MR_PTYPE_CCITT8023  (0x0u <<  2) /**< \brief (CRCCU_MR)  */
#define   CRCCU_MR_PTYPE_CASTAGNOLI (0x1u <<  2) /**< \brief (CRCCU_MR)  */
#define   CRCCU_MR_PTYPE_CCITT16    (0x2u <<  2) /**< \brief (CRCCU_MR)  */
#define CRCCU_MR_DIVIDER_Pos        4            /**< \brief (CRCCU_MR) Bandwidth Divider */
#define CRCCU_MR_DIVIDER_Msk        (0xFu << CRCCU_MR_DIVIDER_Pos)
#define CRCCU_MR_DIVIDER(value)     ((CRCCU_MR_DIVIDER_Msk & ((value) << CRCCU_MR_DIVIDER_Pos)))
#define CRCCU_MR_MASK               0x000000FFu  /**< \brief (CRCCU_MR) MASK Register */

/* -------- CRCCU_SR : (CRCCU Offset: 0x3C) (R/  32) Status Register -------- */
#define CRCCU_SR_OFFSET             0x3C         /**< \brief (CRCCU_SR offset) Status Register */
#define CRCCU_SR_RESETVALUE         0xFFFFFFFF   /**< \brief (CRCCU_SR reset_value) Status Register */

#define CRCCU_SR_CRC_Pos            0            /**< \brief (CRCCU_SR) Cyclic Redundancy Check Value */
#define CRCCU_SR_CRC_Msk            (0xFFFFFFFFu << CRCCU_SR_CRC_Pos)
#define CRCCU_SR_CRC(value)         ((CRCCU_SR_CRC_Msk & ((value) << CRCCU_SR_CRC_Pos)))
#define CRCCU_SR_MASK               0xFFFFFFFFu  /**< \brief (CRCCU_SR) MASK Register */

/* -------- CRCCU_IER : (CRCCU Offset: 0x40) ( /W 32) Interrupt Enable Register -------- */
#define CRCCU_IER_OFFSET            0x40         /**< \brief (CRCCU_IER offset) Interrupt Enable Register */
#define CRCCU_IER_RESETVALUE        0x00000000   /**< \brief (CRCCU_IER reset_value) Interrupt Enable Register */

#define CRCCU_IER_ERRIER_Pos        0            /**< \brief (CRCCU_IER) CRC Error Interrupt Enable */
#define CRCCU_IER_ERRIER            (0x1u << CRCCU_IER_ERRIER_Pos)
#define CRCCU_IER_MASK              0x00000001u  /**< \brief (CRCCU_IER) MASK Register */

/* -------- CRCCU_IDR : (CRCCU Offset: 0x44) ( /W 32) Interrupt Disable Register -------- */
#define CRCCU_IDR_OFFSET            0x44         /**< \brief (CRCCU_IDR offset) Interrupt Disable Register */
#define CRCCU_IDR_RESETVALUE        0x00000000   /**< \brief (CRCCU_IDR reset_value) Interrupt Disable Register */

#define CRCCU_IDR_ERRIDR_Pos        0            /**< \brief (CRCCU_IDR) CRC Error Interrupt Disable */
#define CRCCU_IDR_ERRIDR            (0x1u << CRCCU_IDR_ERRIDR_Pos)
#define CRCCU_IDR_MASK              0x00000001u  /**< \brief (CRCCU_IDR) MASK Register */

/* -------- CRCCU_IMR : (CRCCU Offset: 0x48) (R/  32) Interrupt Mask Register -------- */
#define CRCCU_IMR_OFFSET            0x48         /**< \brief (CRCCU_IMR offset) Interrupt Mask Register */
#define CRCCU_IMR_RESETVALUE        0x00000000   /**< \brief (CRCCU_IMR reset_value) Interrupt Mask Register */

#define CRCCU_IMR_ERRIMR_Pos        0            /**< \brief (CRCCU_IMR) CRC Error Interrupt Mask */
#define CRCCU_IMR_ERRIMR            (0x1u << CRCCU_IMR_ERRIMR_Pos)
#define CRCCU_IMR_MASK              0x00000001u  /**< \brief (CRCCU_IMR) MASK Register */

/* -------- CRCCU_ISR : (CRCCU Offset: 0x4C) (R/  32) Interrupt Status Register -------- */
#define CRCCU_ISR_OFFSET            0x4C         /**< \brief (CRCCU_ISR offset) Interrupt Status Register */
#define CRCCU_ISR_RESETVALUE        0x00000000   /**< \brief (CRCCU_ISR reset_value) Interrupt Status Register */

#define CRCCU_ISR_ERRISR_Pos        0            /**< \brief (CRCCU_ISR) CRC Error Interrupt Status */
#define CRCCU_ISR_ERRISR            (0x1u << CRCCU_ISR_ERRISR_Pos)
#define CRCCU_ISR_MASK              0x00000001u  /**< \brief (CRCCU_ISR) MASK Register */

/* -------- CRCCU_VERSION : (CRCCU Offset: 0xFC) (R/  32) Version Register -------- */
#define CRCCU_VERSION_OFFSET        0xFC         /**< \brief (CRCCU_VERSION offset) Version Register */
#define CRCCU_VERSION_RESETVALUE    0x00000202   /**< \brief (CRCCU_VERSION reset_value) Version Register */

#define CRCCU_VERSION_VERSION_Pos   0            /**< \brief (CRCCU_VERSION) Version Number */
#define CRCCU_VERSION_VERSION_Msk   (0xFFFu << CRCCU_VERSION_VERSION_Pos)
#define CRCCU_VERSION_VERSION(value) ((CRCCU_VERSION_VERSION_Msk & ((value) << CRCCU_VERSION_VERSION_Pos)))
#define CRCCU_VERSION_VARIANT_Pos   16           /**< \brief (CRCCU_VERSION) Variant Number */
#define CRCCU_VERSION_VARIANT_Msk   (0xFu << CRCCU_VERSION_VARIANT_Pos)
#define CRCCU_VERSION_VARIANT(value) ((CRCCU_VERSION_VARIANT_Msk & ((value) << CRCCU_VERSION_VARIANT_Pos)))
#define CRCCU_VERSION_MASK          0x000F0FFFu  /**< \brief (CRCCU_VERSION) MASK Register */

/** \brief CRCCU hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
  RwReg   CRCCU_DSCR;         /**< \brief (CRCCU Offset: 0x00) Descriptor Base Register */
  RoReg8  Reserved1[0x4];
  WoReg   CRCCU_DMAEN;        /**< \brief (CRCCU Offset: 0x08) DMA Enable Register */
  WoReg   CRCCU_DMADIS;       /**< \brief (CRCCU Offset: 0x0C) DMA Disable Register */
  RoReg   CRCCU_DMASR;        /**< \brief (CRCCU Offset: 0x10) DMA Status Register */
  WoReg   CRCCU_DMAIER;       /**< \brief (CRCCU Offset: 0x14) DMA Interrupt Enable Register */
  WoReg   CRCCU_DMAIDR;       /**< \brief (CRCCU Offset: 0x18) DMA Interrupt Disable Register */
  RoReg   CRCCU_DMAIMR;       /**< \brief (CRCCU Offset: 0x1C) DMA Interrupt Mask Register */
  RoReg   CRCCU_DMAISR;       /**< \brief (CRCCU Offset: 0x20) DMA Interrupt Status Register */
  RoReg8  Reserved2[0x10];
  WoReg   CRCCU_CR;           /**< \brief (CRCCU Offset: 0x34) Control Register */
  RwReg   CRCCU_MR;           /**< \brief (CRCCU Offset: 0x38) Mode Register */
  RoReg   CRCCU_SR;           /**< \brief (CRCCU Offset: 0x3C) Status Register */
  WoReg   CRCCU_IER;          /**< \brief (CRCCU Offset: 0x40) Interrupt Enable Register */
  WoReg   CRCCU_IDR;          /**< \brief (CRCCU Offset: 0x44) Interrupt Disable Register */
  RoReg   CRCCU_IMR;          /**< \brief (CRCCU Offset: 0x48) Interrupt Mask Register */
  RoReg   CRCCU_ISR;          /**< \brief (CRCCU Offset: 0x4C) Interrupt Status Register */
  RoReg8  Reserved3[0xAC];
  RoReg   CRCCU_VERSION;      /**< \brief (CRCCU Offset: 0xFC) Version Register */
} Crccu;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/*@}*/

#endif /* _SAM4L_CRCCU_COMPONENT_ */
