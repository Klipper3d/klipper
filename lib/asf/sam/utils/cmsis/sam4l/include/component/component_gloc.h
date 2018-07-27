/**
 * \file
 *
 * \brief Component description for GLOC
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

#ifndef _SAM4L_GLOC_COMPONENT_
#define _SAM4L_GLOC_COMPONENT_

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR GLOC */
/* ========================================================================== */
/** \addtogroup SAM4L_GLOC Glue Logic Controller */
/*@{*/

#define REV_GLOC                    0x102

/* -------- GLOC_CR : (GLOC Offset: 0x00) (R/W 32) LUT Control Register -------- */
#define GLOC_CR_OFFSET              0x00         /**< \brief (GLOC_CR offset) Control Register */
#define GLOC_CR_RESETVALUE          0x00000000   /**< \brief (GLOC_CR reset_value) Control Register */

#define GLOC_CR_AEN_Pos             0            /**< \brief (GLOC_CR) Input mask */
#define GLOC_CR_AEN_Msk             (0xFu << GLOC_CR_AEN_Pos)
#define GLOC_CR_AEN(value)          ((GLOC_CR_AEN_Msk & ((value) << GLOC_CR_AEN_Pos)))
#define GLOC_CR_FILTEN_Pos          31           /**< \brief (GLOC_CR) Filter enable */
#define GLOC_CR_FILTEN              (0x1u << GLOC_CR_FILTEN_Pos)
#define GLOC_CR_MASK                0x8000000Fu  /**< \brief (GLOC_CR) MASK Register */

/* -------- GLOC_TRUTH : (GLOC Offset: 0x04) (R/W 32) LUT Truth Register -------- */
#define GLOC_TRUTH_OFFSET           0x04         /**< \brief (GLOC_TRUTH offset) Truth Register */
#define GLOC_TRUTH_RESETVALUE       0x00000000   /**< \brief (GLOC_TRUTH reset_value) Truth Register */

#define GLOC_TRUTH_TRUTH_Pos        0            /**< \brief (GLOC_TRUTH) Truth */
#define GLOC_TRUTH_TRUTH_Msk        (0xFFFFu << GLOC_TRUTH_TRUTH_Pos)
#define GLOC_TRUTH_TRUTH(value)     ((GLOC_TRUTH_TRUTH_Msk & ((value) << GLOC_TRUTH_TRUTH_Pos)))
#define GLOC_TRUTH_MASK             0x0000FFFFu  /**< \brief (GLOC_TRUTH) MASK Register */

/* -------- GLOC_PARAMETER : (GLOC Offset: 0x38) (R/  32) Parameter Register -------- */
#define GLOC_PARAMETER_OFFSET       0x38         /**< \brief (GLOC_PARAMETER offset) Parameter Register */

#define GLOC_PARAMETER_LUTS_Pos     0            /**< \brief (GLOC_PARAMETER) LUTs */
#define GLOC_PARAMETER_LUTS_Msk     (0xFFu << GLOC_PARAMETER_LUTS_Pos)
#define GLOC_PARAMETER_LUTS(value)  ((GLOC_PARAMETER_LUTS_Msk & ((value) << GLOC_PARAMETER_LUTS_Pos)))
#define GLOC_PARAMETER_MASK         0x000000FFu  /**< \brief (GLOC_PARAMETER) MASK Register */

/* -------- GLOC_VERSION : (GLOC Offset: 0x3C) (R/  32) Version Register -------- */
#define GLOC_VERSION_OFFSET         0x3C         /**< \brief (GLOC_VERSION offset) Version Register */
#define GLOC_VERSION_RESETVALUE     0x00000102   /**< \brief (GLOC_VERSION reset_value) Version Register */

#define GLOC_VERSION_VERSION_Pos    0            /**< \brief (GLOC_VERSION) Version */
#define GLOC_VERSION_VERSION_Msk    (0xFFFu << GLOC_VERSION_VERSION_Pos)
#define GLOC_VERSION_VERSION(value) ((GLOC_VERSION_VERSION_Msk & ((value) << GLOC_VERSION_VERSION_Pos)))
#define GLOC_VERSION_VARIANT_Pos    16           /**< \brief (GLOC_VERSION) Variant */
#define GLOC_VERSION_VARIANT_Msk    (0xFu << GLOC_VERSION_VARIANT_Pos)
#define GLOC_VERSION_VARIANT(value) ((GLOC_VERSION_VARIANT_Msk & ((value) << GLOC_VERSION_VARIANT_Pos)))
#define GLOC_VERSION_MASK           0x000F0FFFu  /**< \brief (GLOC_VERSION) MASK Register */

/** \brief GlocLut hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
  RwReg   GLOC_CR;            /**< \brief (GLOC Offset: 0x00) Control Register */
  RwReg   GLOC_TRUTH;         /**< \brief (GLOC Offset: 0x04) Truth Register */
} GlocLut;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/** \brief GLOC hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
  GlocLut GLOC_LUT[2];        /**< \brief (GLOC Offset: 0x00) GlocLut groups [LUTS] */
  RoReg8  Reserved1[0x28];
  RoReg   GLOC_PARAMETER;     /**< \brief (GLOC Offset: 0x38) Parameter Register */
  RoReg   GLOC_VERSION;       /**< \brief (GLOC Offset: 0x3C) Version Register */
} Gloc;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/*@}*/

#endif /* _SAM4L_GLOC_COMPONENT_ */
