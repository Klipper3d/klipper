/**
 * \file
 *
 * \brief Component description for DACC
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

#ifndef _SAM4L_DACC_COMPONENT_
#define _SAM4L_DACC_COMPONENT_

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR DACC */
/* ========================================================================== */
/** \addtogroup SAM4L_DACC DAC Controller */
/*@{*/

#define REV_DACC                    0x111

/* -------- DACC_CR : (DACC Offset: 0x00) ( /W 32) Control Register -------- */
#define DACC_CR_OFFSET              0x00         /**< \brief (DACC_CR offset) Control Register */
#define DACC_CR_RESETVALUE          0x00000000   /**< \brief (DACC_CR reset_value) Control Register */

#define DACC_CR_SWRST_Pos           0            /**< \brief (DACC_CR) Software Reset */
#define DACC_CR_SWRST               (0x1u << DACC_CR_SWRST_Pos)
#define DACC_CR_MASK                0x00000001u  /**< \brief (DACC_CR) MASK Register */

/* -------- DACC_MR : (DACC Offset: 0x04) (R/W 32) Mode Register -------- */
#define DACC_MR_OFFSET              0x04         /**< \brief (DACC_MR offset) Mode Register */
#define DACC_MR_RESETVALUE          0x00000000   /**< \brief (DACC_MR reset_value) Mode Register */

#define DACC_MR_TRGEN_Pos           0            /**< \brief (DACC_MR) Trigger Enable */
#define DACC_MR_TRGEN               (0x1u << DACC_MR_TRGEN_Pos)
#define DACC_MR_TRGSEL_Pos          1            /**< \brief (DACC_MR) Trigger Selection */
#define DACC_MR_TRGSEL_Msk          (0x7u << DACC_MR_TRGSEL_Pos)
#define DACC_MR_TRGSEL(value)       ((DACC_MR_TRGSEL_Msk & ((value) << DACC_MR_TRGSEL_Pos)))
#define DACC_MR_DACEN_Pos           4            /**< \brief (DACC_MR) DAC Enable */
#define DACC_MR_DACEN               (0x1u << DACC_MR_DACEN_Pos)
#define DACC_MR_WORD_Pos            5            /**< \brief (DACC_MR) Word Transfer */
#define DACC_MR_WORD                (0x1u << DACC_MR_WORD_Pos)
#define DACC_MR_STARTUP_Pos         8            /**< \brief (DACC_MR) Startup Time Selection */
#define DACC_MR_STARTUP_Msk         (0xFFu << DACC_MR_STARTUP_Pos)
#define DACC_MR_STARTUP(value)      ((DACC_MR_STARTUP_Msk & ((value) << DACC_MR_STARTUP_Pos)))
#define DACC_MR_CLKDIV_Pos          16           /**< \brief (DACC_MR) Clock Divider for Internal Trigger */
#define DACC_MR_CLKDIV_Msk          (0xFFFFu << DACC_MR_CLKDIV_Pos)
#define DACC_MR_CLKDIV(value)       ((DACC_MR_CLKDIV_Msk & ((value) << DACC_MR_CLKDIV_Pos)))
#define DACC_MR_MASK                0xFFFFFF3Fu  /**< \brief (DACC_MR) MASK Register */

/* -------- DACC_CDR : (DACC Offset: 0x08) ( /W 32) Conversion Data Register -------- */
#define DACC_CDR_OFFSET             0x08         /**< \brief (DACC_CDR offset) Conversion Data Register */
#define DACC_CDR_RESETVALUE         0x00000000   /**< \brief (DACC_CDR reset_value) Conversion Data Register */

#define DACC_CDR_DATA_Pos           0            /**< \brief (DACC_CDR) Data to Convert */
#define DACC_CDR_DATA_Msk           (0xFFFFFFFFu << DACC_CDR_DATA_Pos)
#define DACC_CDR_DATA(value)        ((DACC_CDR_DATA_Msk & ((value) << DACC_CDR_DATA_Pos)))
#define DACC_CDR_MASK               0xFFFFFFFFu  /**< \brief (DACC_CDR) MASK Register */

/* -------- DACC_IER : (DACC Offset: 0x0C) ( /W 32) Interrupt Enable Register -------- */
#define DACC_IER_OFFSET             0x0C         /**< \brief (DACC_IER offset) Interrupt Enable Register */
#define DACC_IER_RESETVALUE         0x00000000   /**< \brief (DACC_IER reset_value) Interrupt Enable Register */

#define DACC_IER_TXRDY_Pos          0            /**< \brief (DACC_IER) Transmit Ready Interrupt Enable */
#define DACC_IER_TXRDY              (0x1u << DACC_IER_TXRDY_Pos)
#define DACC_IER_MASK               0x00000001u  /**< \brief (DACC_IER) MASK Register */

/* -------- DACC_IDR : (DACC Offset: 0x10) ( /W 32) Interrupt Disable Register -------- */
#define DACC_IDR_OFFSET             0x10         /**< \brief (DACC_IDR offset) Interrupt Disable Register */

#define DACC_IDR_TXRDY_Pos          0            /**< \brief (DACC_IDR) Transmit Ready Interrupt Disable */
#define DACC_IDR_TXRDY              (0x1u << DACC_IDR_TXRDY_Pos)
#define DACC_IDR_MASK               0x00000001u  /**< \brief (DACC_IDR) MASK Register */

/* -------- DACC_IMR : (DACC Offset: 0x14) (R/  32) Interrupt Mask Register -------- */
#define DACC_IMR_OFFSET             0x14         /**< \brief (DACC_IMR offset) Interrupt Mask Register */
#define DACC_IMR_RESETVALUE         0x00000000   /**< \brief (DACC_IMR reset_value) Interrupt Mask Register */

#define DACC_IMR_TXRDY_Pos          0            /**< \brief (DACC_IMR) Transmit Ready Interrupt Mask */
#define DACC_IMR_TXRDY              (0x1u << DACC_IMR_TXRDY_Pos)
#define DACC_IMR_MASK               0x00000001u  /**< \brief (DACC_IMR) MASK Register */

/* -------- DACC_ISR : (DACC Offset: 0x18) (R/  32) Interrupt Status Register -------- */
#define DACC_ISR_OFFSET             0x18         /**< \brief (DACC_ISR offset) Interrupt Status Register */
#define DACC_ISR_RESETVALUE         0x00000000   /**< \brief (DACC_ISR reset_value) Interrupt Status Register */

#define DACC_ISR_TXRDY_Pos          0            /**< \brief (DACC_ISR) Transmit Ready Interrupt Status */
#define DACC_ISR_TXRDY              (0x1u << DACC_ISR_TXRDY_Pos)
#define DACC_ISR_MASK               0x00000001u  /**< \brief (DACC_ISR) MASK Register */

/* -------- DACC_WPMR : (DACC Offset: 0xE4) (R/W 32) Write Protect Mode Register -------- */
#define DACC_WPMR_OFFSET            0xE4         /**< \brief (DACC_WPMR offset) Write Protect Mode Register */
#define DACC_WPMR_RESETVALUE        0x00000000   /**< \brief (DACC_WPMR reset_value) Write Protect Mode Register */

#define DACC_WPMR_WPEN_Pos          0            /**< \brief (DACC_WPMR) Write Protect Enable */
#define DACC_WPMR_WPEN              (0x1u << DACC_WPMR_WPEN_Pos)
#define DACC_WPMR_WPKEY_Pos         8            /**< \brief (DACC_WPMR) Write Protect Key */
#define DACC_WPMR_WPKEY_Msk         (0xFFFFFFu << DACC_WPMR_WPKEY_Pos)
#define DACC_WPMR_WPKEY(value)      ((DACC_WPMR_WPKEY_Msk & ((value) << DACC_WPMR_WPKEY_Pos)))
#define DACC_WPMR_MASK              0xFFFFFF01u  /**< \brief (DACC_WPMR) MASK Register */

/* -------- DACC_WPSR : (DACC Offset: 0xE8) (R/  32) Write Protect Status Register -------- */
#define DACC_WPSR_OFFSET            0xE8         /**< \brief (DACC_WPSR offset) Write Protect Status Register */
#define DACC_WPSR_RESETVALUE        0x00000000   /**< \brief (DACC_WPSR reset_value) Write Protect Status Register */

#define DACC_WPSR_WPROTERR_Pos      0            /**< \brief (DACC_WPSR) Write Protection Error */
#define DACC_WPSR_WPROTERR          (0x1u << DACC_WPSR_WPROTERR_Pos)
#define DACC_WPSR_WPROTADDR_Pos     8            /**< \brief (DACC_WPSR) Write Protection Error Address */
#define DACC_WPSR_WPROTADDR_Msk     (0xFFu << DACC_WPSR_WPROTADDR_Pos)
#define DACC_WPSR_WPROTADDR(value)  ((DACC_WPSR_WPROTADDR_Msk & ((value) << DACC_WPSR_WPROTADDR_Pos)))
#define DACC_WPSR_MASK              0x0000FF01u  /**< \brief (DACC_WPSR) MASK Register */

/* -------- DACC_VERSION : (DACC Offset: 0xFC) (R/  32) Version Register -------- */
#define DACC_VERSION_OFFSET         0xFC         /**< \brief (DACC_VERSION offset) Version Register */
#define DACC_VERSION_RESETVALUE     0x00000111   /**< \brief (DACC_VERSION reset_value) Version Register */

#define DACC_VERSION_VERSION_Pos    0            /**< \brief (DACC_VERSION) Version Number */
#define DACC_VERSION_VERSION_Msk    (0xFFFu << DACC_VERSION_VERSION_Pos)
#define DACC_VERSION_VERSION(value) ((DACC_VERSION_VERSION_Msk & ((value) << DACC_VERSION_VERSION_Pos)))
#define DACC_VERSION_VARIANT_Pos    16           /**< \brief (DACC_VERSION) Variant Number */
#define DACC_VERSION_VARIANT_Msk    (0x7u << DACC_VERSION_VARIANT_Pos)
#define DACC_VERSION_VARIANT(value) ((DACC_VERSION_VARIANT_Msk & ((value) << DACC_VERSION_VARIANT_Pos)))
#define DACC_VERSION_MASK           0x00070FFFu  /**< \brief (DACC_VERSION) MASK Register */

/** \brief DACC hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
  WoReg   DACC_CR;            /**< \brief (DACC Offset: 0x00) Control Register */
  RwReg   DACC_MR;            /**< \brief (DACC Offset: 0x04) Mode Register */
  WoReg   DACC_CDR;           /**< \brief (DACC Offset: 0x08) Conversion Data Register */
  WoReg   DACC_IER;           /**< \brief (DACC Offset: 0x0C) Interrupt Enable Register */
  WoReg   DACC_IDR;           /**< \brief (DACC Offset: 0x10) Interrupt Disable Register */
  RoReg   DACC_IMR;           /**< \brief (DACC Offset: 0x14) Interrupt Mask Register */
  RoReg   DACC_ISR;           /**< \brief (DACC Offset: 0x18) Interrupt Status Register */
  RoReg8  Reserved1[0xC8];
  RwReg   DACC_WPMR;          /**< \brief (DACC Offset: 0xE4) Write Protect Mode Register */
  RoReg   DACC_WPSR;          /**< \brief (DACC Offset: 0xE8) Write Protect Status Register */
  RoReg8  Reserved2[0x10];
  RoReg   DACC_VERSION;       /**< \brief (DACC Offset: 0xFC) Version Register */
} Dacc;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/*@}*/

#endif /* _SAM4L_DACC_COMPONENT_ */
