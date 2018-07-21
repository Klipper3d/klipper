/**
 * \file
 *
 * \brief Component description for SMAP
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

#ifndef _SAM4L_SMAP_COMPONENT_
#define _SAM4L_SMAP_COMPONENT_

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR SMAP */
/* ========================================================================== */
/** \addtogroup SAM4L_SMAP System Manager Access Port */
/*@{*/

#define REV_SMAP                    0x100

/* -------- SMAP_CR : (SMAP Offset: 0x00) ( /W 32) Control Register -------- */
#define SMAP_CR_OFFSET              0x00         /**< \brief (SMAP_CR offset) Control Register */
#define SMAP_CR_RESETVALUE          0x00000000   /**< \brief (SMAP_CR reset_value) Control Register */

#define SMAP_CR_EN_Pos              0            /**< \brief (SMAP_CR) Enable */
#define SMAP_CR_EN                  (0x1u << SMAP_CR_EN_Pos)
#define SMAP_CR_DIS_Pos             1            /**< \brief (SMAP_CR) Disable */
#define SMAP_CR_DIS                 (0x1u << SMAP_CR_DIS_Pos)
#define SMAP_CR_CRC_Pos             2            /**< \brief (SMAP_CR) User Page Read */
#define SMAP_CR_CRC                 (0x1u << SMAP_CR_CRC_Pos)
#define SMAP_CR_FSPR_Pos            3            /**< \brief (SMAP_CR) Flash Supplementary Page Read */
#define SMAP_CR_FSPR                (0x1u << SMAP_CR_FSPR_Pos)
#define SMAP_CR_CE_Pos              4            /**< \brief (SMAP_CR) Chip Erase */
#define SMAP_CR_CE                  (0x1u << SMAP_CR_CE_Pos)
#define SMAP_CR_MASK                0x0000001Fu  /**< \brief (SMAP_CR) MASK Register */

/* -------- SMAP_SR : (SMAP Offset: 0x04) (R/  32) Status Register -------- */
#define SMAP_SR_OFFSET              0x04         /**< \brief (SMAP_SR offset) Status Register */
#define SMAP_SR_RESETVALUE          0x00000000   /**< \brief (SMAP_SR reset_value) Status Register */

#define SMAP_SR_DONE_Pos            0            /**< \brief (SMAP_SR) Operation done */
#define SMAP_SR_DONE                (0x1u << SMAP_SR_DONE_Pos)
#define SMAP_SR_HCR_Pos             1            /**< \brief (SMAP_SR) Hold Core reset */
#define SMAP_SR_HCR                 (0x1u << SMAP_SR_HCR_Pos)
#define SMAP_SR_BERR_Pos            2            /**< \brief (SMAP_SR) Bus error */
#define SMAP_SR_BERR                (0x1u << SMAP_SR_BERR_Pos)
#define SMAP_SR_FAIL_Pos            3            /**< \brief (SMAP_SR) Failure */
#define SMAP_SR_FAIL                (0x1u << SMAP_SR_FAIL_Pos)
#define SMAP_SR_LCK_Pos             4            /**< \brief (SMAP_SR) Lock */
#define SMAP_SR_LCK                 (0x1u << SMAP_SR_LCK_Pos)
#define SMAP_SR_EN_Pos              8            /**< \brief (SMAP_SR) Enabled */
#define SMAP_SR_EN                  (0x1u << SMAP_SR_EN_Pos)
#define SMAP_SR_PROT_Pos            9            /**< \brief (SMAP_SR) Protected */
#define SMAP_SR_PROT                (0x1u << SMAP_SR_PROT_Pos)
#define SMAP_SR_DBGP_Pos            10           /**< \brief (SMAP_SR) Debugger Present */
#define SMAP_SR_DBGP                (0x1u << SMAP_SR_DBGP_Pos)
#define SMAP_SR_STATE_Pos           24           /**< \brief (SMAP_SR) State */
#define SMAP_SR_STATE_Msk           (0x7u << SMAP_SR_STATE_Pos)
#define SMAP_SR_STATE(value)        ((SMAP_SR_STATE_Msk & ((value) << SMAP_SR_STATE_Pos)))
#define SMAP_SR_MASK                0x0700071Fu  /**< \brief (SMAP_SR) MASK Register */

/* -------- SMAP_SCR : (SMAP Offset: 0x08) ( /W 32) Status Clear Register -------- */
#define SMAP_SCR_OFFSET             0x08         /**< \brief (SMAP_SCR offset) Status Clear Register */
#define SMAP_SCR_RESETVALUE         0x00000000   /**< \brief (SMAP_SCR reset_value) Status Clear Register */

#define SMAP_SCR_DONE_Pos           0            /**< \brief (SMAP_SCR) Done */
#define SMAP_SCR_DONE               (0x1u << SMAP_SCR_DONE_Pos)
#define SMAP_SCR_HCR_Pos            1            /**< \brief (SMAP_SCR) Hold Core Register */
#define SMAP_SCR_HCR                (0x1u << SMAP_SCR_HCR_Pos)
#define SMAP_SCR_BERR_Pos           2            /**< \brief (SMAP_SCR) Bus error */
#define SMAP_SCR_BERR               (0x1u << SMAP_SCR_BERR_Pos)
#define SMAP_SCR_FAIL_Pos           3            /**< \brief (SMAP_SCR) Failure */
#define SMAP_SCR_FAIL               (0x1u << SMAP_SCR_FAIL_Pos)
#define SMAP_SCR_LCK_Pos            4            /**< \brief (SMAP_SCR) Lock error */
#define SMAP_SCR_LCK                (0x1u << SMAP_SCR_LCK_Pos)
#define SMAP_SCR_MASK               0x0000001Fu  /**< \brief (SMAP_SCR) MASK Register */

/* -------- SMAP_ADDR : (SMAP Offset: 0x0C) (R/W 32) Address Register -------- */
#define SMAP_ADDR_OFFSET            0x0C         /**< \brief (SMAP_ADDR offset) Address Register */
#define SMAP_ADDR_RESETVALUE        0x00000000   /**< \brief (SMAP_ADDR reset_value) Address Register */

#define SMAP_ADDR_ADDR_Pos          2            /**< \brief (SMAP_ADDR) Address Value */
#define SMAP_ADDR_ADDR_Msk          (0x3FFFFFFFu << SMAP_ADDR_ADDR_Pos)
#define SMAP_ADDR_ADDR(value)       ((SMAP_ADDR_ADDR_Msk & ((value) << SMAP_ADDR_ADDR_Pos)))
#define SMAP_ADDR_MASK              0xFFFFFFFCu  /**< \brief (SMAP_ADDR) MASK Register */

/* -------- SMAP_LENGTH : (SMAP Offset: 0x10) (R/W 32) Length Register -------- */
#define SMAP_LENGTH_OFFSET          0x10         /**< \brief (SMAP_LENGTH offset) Length Register */
#define SMAP_LENGTH_RESETVALUE      0x00000000   /**< \brief (SMAP_LENGTH reset_value) Length Register */

#define SMAP_LENGTH_LENGTH_Pos      2            /**< \brief (SMAP_LENGTH) Length Register */
#define SMAP_LENGTH_LENGTH_Msk      (0x3FFFFFFFu << SMAP_LENGTH_LENGTH_Pos)
#define SMAP_LENGTH_LENGTH(value)   ((SMAP_LENGTH_LENGTH_Msk & ((value) << SMAP_LENGTH_LENGTH_Pos)))
#define SMAP_LENGTH_MASK            0xFFFFFFFCu  /**< \brief (SMAP_LENGTH) MASK Register */

/* -------- SMAP_DATA : (SMAP Offset: 0x14) (R/W 32) Data Register -------- */
#define SMAP_DATA_OFFSET            0x14         /**< \brief (SMAP_DATA offset) Data Register */
#define SMAP_DATA_RESETVALUE        0x00000000   /**< \brief (SMAP_DATA reset_value) Data Register */

#define SMAP_DATA_DATA_Pos          0            /**< \brief (SMAP_DATA) Generic data register */
#define SMAP_DATA_DATA_Msk          (0xFFFFFFFFu << SMAP_DATA_DATA_Pos)
#define SMAP_DATA_DATA(value)       ((SMAP_DATA_DATA_Msk & ((value) << SMAP_DATA_DATA_Pos)))
#define SMAP_DATA_MASK              0xFFFFFFFFu  /**< \brief (SMAP_DATA) MASK Register */

/* -------- SMAP_VERSION : (SMAP Offset: 0x28) (R/  32) VERSION register -------- */
#define SMAP_VERSION_OFFSET         0x28         /**< \brief (SMAP_VERSION offset) VERSION register */
#define SMAP_VERSION_RESETVALUE     0x00000100   /**< \brief (SMAP_VERSION reset_value) VERSION register */

#define SMAP_VERSION_VERSION_Pos    0            /**< \brief (SMAP_VERSION) Version number */
#define SMAP_VERSION_VERSION_Msk    (0xFFFu << SMAP_VERSION_VERSION_Pos)
#define SMAP_VERSION_VERSION(value) ((SMAP_VERSION_VERSION_Msk & ((value) << SMAP_VERSION_VERSION_Pos)))
#define SMAP_VERSION_VARIANT_Pos    16           /**< \brief (SMAP_VERSION) Variant number */
#define SMAP_VERSION_VARIANT_Msk    (0xFu << SMAP_VERSION_VARIANT_Pos)
#define SMAP_VERSION_VARIANT(value) ((SMAP_VERSION_VARIANT_Msk & ((value) << SMAP_VERSION_VARIANT_Pos)))
#define SMAP_VERSION_MASK           0x000F0FFFu  /**< \brief (SMAP_VERSION) MASK Register */

/* -------- SMAP_CIDR : (SMAP Offset: 0xF0) (R/  32) Chip ID Register -------- */
#define SMAP_CIDR_OFFSET            0xF0         /**< \brief (SMAP_CIDR offset) Chip ID Register */

#define SMAP_CIDR_VERSION_Pos       0            /**< \brief (SMAP_CIDR) Version of the Device */
#define SMAP_CIDR_VERSION_Msk       (0x1Fu << SMAP_CIDR_VERSION_Pos)
#define SMAP_CIDR_VERSION(value)    ((SMAP_CIDR_VERSION_Msk & ((value) << SMAP_CIDR_VERSION_Pos)))
#define SMAP_CIDR_EPROC_Pos         5            /**< \brief (SMAP_CIDR) Embedded Processor */
#define SMAP_CIDR_EPROC_Msk         (0x7u << SMAP_CIDR_EPROC_Pos)
#define SMAP_CIDR_EPROC(value)      ((SMAP_CIDR_EPROC_Msk & ((value) << SMAP_CIDR_EPROC_Pos)))
#define SMAP_CIDR_NVPSIZ_Pos        8            /**< \brief (SMAP_CIDR) Nonvolatile Program Memory Size */
#define SMAP_CIDR_NVPSIZ_Msk        (0xFu << SMAP_CIDR_NVPSIZ_Pos)
#define SMAP_CIDR_NVPSIZ(value)     ((SMAP_CIDR_NVPSIZ_Msk & ((value) << SMAP_CIDR_NVPSIZ_Pos)))
#define SMAP_CIDR_NVPSIZ2_Pos       12           /**< \brief (SMAP_CIDR) Second Nonvolatile Program Memory Size */
#define SMAP_CIDR_NVPSIZ2_Msk       (0xFu << SMAP_CIDR_NVPSIZ2_Pos)
#define SMAP_CIDR_NVPSIZ2(value)    ((SMAP_CIDR_NVPSIZ2_Msk & ((value) << SMAP_CIDR_NVPSIZ2_Pos)))
#define SMAP_CIDR_SRAMSIZ_Pos       16           /**< \brief (SMAP_CIDR) Internal SRAM Size */
#define SMAP_CIDR_SRAMSIZ_Msk       (0x1Fu << SMAP_CIDR_SRAMSIZ_Pos)
#define SMAP_CIDR_SRAMSIZ(value)    ((SMAP_CIDR_SRAMSIZ_Msk & ((value) << SMAP_CIDR_SRAMSIZ_Pos)))
#define SMAP_CIDR_ARCH_Pos          21           /**< \brief (SMAP_CIDR) Architecture Identifier */
#define SMAP_CIDR_ARCH_Msk          (0x7Fu << SMAP_CIDR_ARCH_Pos)
#define SMAP_CIDR_ARCH(value)       ((SMAP_CIDR_ARCH_Msk & ((value) << SMAP_CIDR_ARCH_Pos)))
#define SMAP_CIDR_NVPTYP_Pos        28           /**< \brief (SMAP_CIDR) Nonvolatile Program Memory Type */
#define SMAP_CIDR_NVPTYP_Msk        (0x7u << SMAP_CIDR_NVPTYP_Pos)
#define SMAP_CIDR_NVPTYP(value)     ((SMAP_CIDR_NVPTYP_Msk & ((value) << SMAP_CIDR_NVPTYP_Pos)))
#define SMAP_CIDR_EXT_Pos           31           /**< \brief (SMAP_CIDR) Extension Flag */
#define SMAP_CIDR_EXT               (0x1u << SMAP_CIDR_EXT_Pos)
#define SMAP_CIDR_MASK              0xFFFFFFFFu  /**< \brief (SMAP_CIDR) MASK Register */

/* -------- SMAP_EXID : (SMAP Offset: 0xF4) (R/  32) Chip ID Extension Register -------- */
#define SMAP_EXID_OFFSET            0xF4         /**< \brief (SMAP_EXID offset) Chip ID Extension Register */

#define SMAP_EXID_EXID_Pos          0            /**< \brief (SMAP_EXID) Chip ID Extension */
#define SMAP_EXID_EXID_Msk          (0xFFFFFFFFu << SMAP_EXID_EXID_Pos)
#define SMAP_EXID_EXID(value)       ((SMAP_EXID_EXID_Msk & ((value) << SMAP_EXID_EXID_Pos)))
#define SMAP_EXID_MASK              0xFFFFFFFFu  /**< \brief (SMAP_EXID) MASK Register */

/* -------- SMAP_IDR : (SMAP Offset: 0xFC) (R/  32) AP Identification register -------- */
#define SMAP_IDR_OFFSET             0xFC         /**< \brief (SMAP_IDR offset) AP Identification register */
#define SMAP_IDR_RESETVALUE         0x003E0000   /**< \brief (SMAP_IDR reset_value) AP Identification register */

#define SMAP_IDR_APIDV_Pos          0            /**< \brief (SMAP_IDR) AP Identification Variant */
#define SMAP_IDR_APIDV_Msk          (0xFu << SMAP_IDR_APIDV_Pos)
#define SMAP_IDR_APIDV(value)       ((SMAP_IDR_APIDV_Msk & ((value) << SMAP_IDR_APIDV_Pos)))
#define SMAP_IDR_APID_Pos           4            /**< \brief (SMAP_IDR) AP Identification */
#define SMAP_IDR_APID_Msk           (0xFu << SMAP_IDR_APID_Pos)
#define SMAP_IDR_APID(value)        ((SMAP_IDR_APID_Msk & ((value) << SMAP_IDR_APID_Pos)))
#define SMAP_IDR_CLSS_Pos           16           /**< \brief (SMAP_IDR) Class */
#define SMAP_IDR_CLSS               (0x1u << SMAP_IDR_CLSS_Pos)
#define SMAP_IDR_IC_Pos             17           /**< \brief (SMAP_IDR) JEP-106 Identity Code */
#define SMAP_IDR_IC_Msk             (0x7Fu << SMAP_IDR_IC_Pos)
#define SMAP_IDR_IC(value)          ((SMAP_IDR_IC_Msk & ((value) << SMAP_IDR_IC_Pos)))
#define SMAP_IDR_CC_Pos             24           /**< \brief (SMAP_IDR) JEP-106 Continuation Code */
#define SMAP_IDR_CC_Msk             (0xFu << SMAP_IDR_CC_Pos)
#define SMAP_IDR_CC(value)          ((SMAP_IDR_CC_Msk & ((value) << SMAP_IDR_CC_Pos)))
#define SMAP_IDR_REVISION_Pos       28           /**< \brief (SMAP_IDR) Revision */
#define SMAP_IDR_REVISION_Msk       (0xFu << SMAP_IDR_REVISION_Pos)
#define SMAP_IDR_REVISION(value)    ((SMAP_IDR_REVISION_Msk & ((value) << SMAP_IDR_REVISION_Pos)))
#define SMAP_IDR_MASK               0xFFFF00FFu  /**< \brief (SMAP_IDR) MASK Register */

/** \brief SMAP hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
  WoReg   SMAP_CR;            /**< \brief (SMAP Offset: 0x00) Control Register */
  RoReg   SMAP_SR;            /**< \brief (SMAP Offset: 0x04) Status Register */
  WoReg   SMAP_SCR;           /**< \brief (SMAP Offset: 0x08) Status Clear Register */
  RwReg   SMAP_ADDR;          /**< \brief (SMAP Offset: 0x0C) Address Register */
  RwReg   SMAP_LENGTH;        /**< \brief (SMAP Offset: 0x10) Length Register */
  RwReg   SMAP_DATA;          /**< \brief (SMAP Offset: 0x14) Data Register */
  RoReg8  Reserved1[0x10];
  RoReg   SMAP_VERSION;       /**< \brief (SMAP Offset: 0x28) VERSION register */
  RoReg8  Reserved2[0xC4];
  RoReg   SMAP_CIDR;          /**< \brief (SMAP Offset: 0xF0) Chip ID Register */
  RoReg   SMAP_EXID;          /**< \brief (SMAP Offset: 0xF4) Chip ID Extension Register */
  RoReg8  Reserved3[0x4];
  RoReg   SMAP_IDR;           /**< \brief (SMAP Offset: 0xFC) AP Identification register */
} Smap;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/*@}*/

#endif /* _SAM4L_SMAP_COMPONENT_ */
