/**
 * \file
 *
 * \brief Component description for AESA
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

#ifndef _SAM4L_AESA_COMPONENT_
#define _SAM4L_AESA_COMPONENT_

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR AESA */
/* ========================================================================== */
/** \addtogroup SAM4L_AESA Advanced Encryption Standard */
/*@{*/

#define REV_AESA                    0x102

/* -------- AESA_CTRL : (AESA Offset: 0x00) (R/W 32) Control Register -------- */
#define AESA_CTRL_OFFSET            0x00         /**< \brief (AESA_CTRL offset) Control Register */
#define AESA_CTRL_RESETVALUE        0x00000000   /**< \brief (AESA_CTRL reset_value) Control Register */

#define AESA_CTRL_ENABLE_Pos        0            /**< \brief (AESA_CTRL) Enable Module */
#define AESA_CTRL_ENABLE            (0x1u << AESA_CTRL_ENABLE_Pos)
#define AESA_CTRL_DKEYGEN_Pos       1            /**< \brief (AESA_CTRL) Decryption Key Generate */
#define AESA_CTRL_DKEYGEN           (0x1u << AESA_CTRL_DKEYGEN_Pos)
#define AESA_CTRL_NEWMSG_Pos        2            /**< \brief (AESA_CTRL) New Message */
#define AESA_CTRL_NEWMSG            (0x1u << AESA_CTRL_NEWMSG_Pos)
#define AESA_CTRL_SWRST_Pos         8            /**< \brief (AESA_CTRL) Software Reset */
#define AESA_CTRL_SWRST             (0x1u << AESA_CTRL_SWRST_Pos)
#define AESA_CTRL_MASK              0x00000107u  /**< \brief (AESA_CTRL) MASK Register */

/* -------- AESA_MODE : (AESA Offset: 0x04) (R/W 32) Mode Register -------- */
#define AESA_MODE_OFFSET            0x04         /**< \brief (AESA_MODE offset) Mode Register */
#define AESA_MODE_RESETVALUE        0x000F0000   /**< \brief (AESA_MODE reset_value) Mode Register */

#define AESA_MODE_ENCRYPT_Pos       0            /**< \brief (AESA_MODE) Encryption */
#define AESA_MODE_ENCRYPT           (0x1u << AESA_MODE_ENCRYPT_Pos)
#define AESA_MODE_KEYSIZE_Pos       1            /**< \brief (AESA_MODE) Key Size */
#define AESA_MODE_KEYSIZE_Msk       (0x3u << AESA_MODE_KEYSIZE_Pos)
#define AESA_MODE_KEYSIZE(value)    ((AESA_MODE_KEYSIZE_Msk & ((value) << AESA_MODE_KEYSIZE_Pos)))
#define AESA_MODE_DMA_Pos           3            /**< \brief (AESA_MODE) DMA Mode */
#define AESA_MODE_DMA               (0x1u << AESA_MODE_DMA_Pos)
#define AESA_MODE_OPMODE_Pos        4            /**< \brief (AESA_MODE) Confidentiality Mode of Operation */
#define AESA_MODE_OPMODE_Msk        (0x7u << AESA_MODE_OPMODE_Pos)
#define AESA_MODE_OPMODE(value)     ((AESA_MODE_OPMODE_Msk & ((value) << AESA_MODE_OPMODE_Pos)))
#define AESA_MODE_CFBS_Pos          8            /**< \brief (AESA_MODE) Cipher Feedback Data Segment Size */
#define AESA_MODE_CFBS_Msk          (0x7u << AESA_MODE_CFBS_Pos)
#define AESA_MODE_CFBS(value)       ((AESA_MODE_CFBS_Msk & ((value) << AESA_MODE_CFBS_Pos)))
#define AESA_MODE_CTYPE_Pos         16           /**< \brief (AESA_MODE) Countermeasure Type */
#define AESA_MODE_CTYPE_Msk         (0xFu << AESA_MODE_CTYPE_Pos)
#define AESA_MODE_CTYPE(value)      ((AESA_MODE_CTYPE_Msk & ((value) << AESA_MODE_CTYPE_Pos)))
#define AESA_MODE_MASK              0x000F077Fu  /**< \brief (AESA_MODE) MASK Register */

/* -------- AESA_DATABUFPTR : (AESA Offset: 0x08) (R/W 32) Data Buffer Pointer Register -------- */
#define AESA_DATABUFPTR_OFFSET      0x08         /**< \brief (AESA_DATABUFPTR offset) Data Buffer Pointer Register */
#define AESA_DATABUFPTR_RESETVALUE  0x00000000   /**< \brief (AESA_DATABUFPTR reset_value) Data Buffer Pointer Register */

#define AESA_DATABUFPTR_IDATAW_Pos  0            /**< \brief (AESA_DATABUFPTR) Input Data Word */
#define AESA_DATABUFPTR_IDATAW_Msk  (0x3u << AESA_DATABUFPTR_IDATAW_Pos)
#define AESA_DATABUFPTR_IDATAW(value) ((AESA_DATABUFPTR_IDATAW_Msk & ((value) << AESA_DATABUFPTR_IDATAW_Pos)))
#define AESA_DATABUFPTR_ODATAW_Pos  4            /**< \brief (AESA_DATABUFPTR) Output Data Word */
#define AESA_DATABUFPTR_ODATAW_Msk  (0x3u << AESA_DATABUFPTR_ODATAW_Pos)
#define AESA_DATABUFPTR_ODATAW(value) ((AESA_DATABUFPTR_ODATAW_Msk & ((value) << AESA_DATABUFPTR_ODATAW_Pos)))
#define AESA_DATABUFPTR_MASK        0x00000033u  /**< \brief (AESA_DATABUFPTR) MASK Register */

/* -------- AESA_SR : (AESA Offset: 0x0C) (R/  32) Status Register -------- */
#define AESA_SR_OFFSET              0x0C         /**< \brief (AESA_SR offset) Status Register */
#define AESA_SR_RESETVALUE          0x00010000   /**< \brief (AESA_SR reset_value) Status Register */

#define AESA_SR_ODATARDY_Pos        0            /**< \brief (AESA_SR) Output Data Ready */
#define AESA_SR_ODATARDY            (0x1u << AESA_SR_ODATARDY_Pos)
#define AESA_SR_IBUFRDY_Pos         16           /**< \brief (AESA_SR) Input Buffer Ready */
#define AESA_SR_IBUFRDY             (0x1u << AESA_SR_IBUFRDY_Pos)
#define AESA_SR_MASK                0x00010001u  /**< \brief (AESA_SR) MASK Register */

/* -------- AESA_IER : (AESA Offset: 0x10) ( /W 32) Interrupt Enable Register -------- */
#define AESA_IER_OFFSET             0x10         /**< \brief (AESA_IER offset) Interrupt Enable Register */
#define AESA_IER_RESETVALUE         0x00000000   /**< \brief (AESA_IER reset_value) Interrupt Enable Register */

#define AESA_IER_ODATARDY_Pos       0            /**< \brief (AESA_IER) Output Data Ready Interrupt Enable */
#define AESA_IER_ODATARDY           (0x1u << AESA_IER_ODATARDY_Pos)
#define AESA_IER_IBUFRDY_Pos        16           /**< \brief (AESA_IER) Input Buffer Ready Interrupt Enable */
#define AESA_IER_IBUFRDY            (0x1u << AESA_IER_IBUFRDY_Pos)
#define AESA_IER_MASK               0x00010001u  /**< \brief (AESA_IER) MASK Register */

/* -------- AESA_IDR : (AESA Offset: 0x14) ( /W 32) Interrupt Disable Register -------- */
#define AESA_IDR_OFFSET             0x14         /**< \brief (AESA_IDR offset) Interrupt Disable Register */
#define AESA_IDR_RESETVALUE         0x00000000   /**< \brief (AESA_IDR reset_value) Interrupt Disable Register */

#define AESA_IDR_ODATARDY_Pos       0            /**< \brief (AESA_IDR) Output Data Ready Interrupt Disable */
#define AESA_IDR_ODATARDY           (0x1u << AESA_IDR_ODATARDY_Pos)
#define AESA_IDR_IBUFRDY_Pos        16           /**< \brief (AESA_IDR) Input Buffer Ready Interrupt Disable */
#define AESA_IDR_IBUFRDY            (0x1u << AESA_IDR_IBUFRDY_Pos)
#define AESA_IDR_MASK               0x00010001u  /**< \brief (AESA_IDR) MASK Register */

/* -------- AESA_IMR : (AESA Offset: 0x18) (R/  32) Interrupt Mask Register -------- */
#define AESA_IMR_OFFSET             0x18         /**< \brief (AESA_IMR offset) Interrupt Mask Register */
#define AESA_IMR_RESETVALUE         0x00000000   /**< \brief (AESA_IMR reset_value) Interrupt Mask Register */

#define AESA_IMR_ODATARDY_Pos       0            /**< \brief (AESA_IMR) Output Data Ready Interrupt Mask */
#define AESA_IMR_ODATARDY           (0x1u << AESA_IMR_ODATARDY_Pos)
#define AESA_IMR_IBUFRDY_Pos        16           /**< \brief (AESA_IMR) Input Buffer Ready Interrupt Mask */
#define AESA_IMR_IBUFRDY            (0x1u << AESA_IMR_IBUFRDY_Pos)
#define AESA_IMR_MASK               0x00010001u  /**< \brief (AESA_IMR) MASK Register */

/* -------- AESA_KEY : (AESA Offset: 0x20) ( /W 32) KEY Key Register -------- */
#define AESA_KEY_OFFSET             0x20         /**< \brief (AESA_KEY offset) Key Register */
#define AESA_KEY_RESETVALUE         0x00000000   /**< \brief (AESA_KEY reset_value) Key Register */

#define AESA_KEY_KEY0_Pos           0            /**< \brief (AESA_KEY) Key Word 0 */
#define AESA_KEY_KEY0_Msk           (0xFFFFFFFFu << AESA_KEY_KEY0_Pos)
#define AESA_KEY_KEY0(value)        ((AESA_KEY_KEY0_Msk & ((value) << AESA_KEY_KEY0_Pos)))
#define AESA_KEY_MASK               0xFFFFFFFFu  /**< \brief (AESA_KEY) MASK Register */

/* -------- AESA_INITVECT : (AESA Offset: 0x40) ( /W 32) INITVECT Initialization Vector Register -------- */
#define AESA_INITVECT_OFFSET        0x40         /**< \brief (AESA_INITVECT offset) Initialization Vector Register */
#define AESA_INITVECT_RESETVALUE    0x00000000   /**< \brief (AESA_INITVECT reset_value) Initialization Vector Register */

#define AESA_INITVECT_INITVECT0_Pos 0            /**< \brief (AESA_INITVECT) Initialization Vector Word 0 */
#define AESA_INITVECT_INITVECT0_Msk (0xFFFFFFFFu << AESA_INITVECT_INITVECT0_Pos)
#define AESA_INITVECT_INITVECT0(value) ((AESA_INITVECT_INITVECT0_Msk & ((value) << AESA_INITVECT_INITVECT0_Pos)))
#define AESA_INITVECT_MASK          0xFFFFFFFFu  /**< \brief (AESA_INITVECT) MASK Register */

/* -------- AESA_IDATA : (AESA Offset: 0x50) ( /W 32) Input Data Register -------- */
#define AESA_IDATA_OFFSET           0x50         /**< \brief (AESA_IDATA offset) Input Data Register */
#define AESA_IDATA_RESETVALUE       0x00000000   /**< \brief (AESA_IDATA reset_value) Input Data Register */

#define AESA_IDATA_IDATA_Pos        0            /**< \brief (AESA_IDATA) Input Data */
#define AESA_IDATA_IDATA_Msk        (0xFFFFFFFFu << AESA_IDATA_IDATA_Pos)
#define AESA_IDATA_IDATA(value)     ((AESA_IDATA_IDATA_Msk & ((value) << AESA_IDATA_IDATA_Pos)))
#define AESA_IDATA_MASK             0xFFFFFFFFu  /**< \brief (AESA_IDATA) MASK Register */

/* -------- AESA_ODATA : (AESA Offset: 0x60) (R/  32) Output Data Register -------- */
#define AESA_ODATA_OFFSET           0x60         /**< \brief (AESA_ODATA offset) Output Data Register */
#define AESA_ODATA_RESETVALUE       0x00000000   /**< \brief (AESA_ODATA reset_value) Output Data Register */

#define AESA_ODATA_ODATA_Pos        0            /**< \brief (AESA_ODATA) Output Data */
#define AESA_ODATA_ODATA_Msk        (0xFFFFFFFFu << AESA_ODATA_ODATA_Pos)
#define AESA_ODATA_ODATA(value)     ((AESA_ODATA_ODATA_Msk & ((value) << AESA_ODATA_ODATA_Pos)))
#define AESA_ODATA_MASK             0xFFFFFFFFu  /**< \brief (AESA_ODATA) MASK Register */

/* -------- AESA_DRNGSEED : (AESA Offset: 0x70) ( /W 32) DRNG Seed Register -------- */
#define AESA_DRNGSEED_OFFSET        0x70         /**< \brief (AESA_DRNGSEED offset) DRNG Seed Register */
#define AESA_DRNGSEED_RESETVALUE    0x00000000   /**< \brief (AESA_DRNGSEED reset_value) DRNG Seed Register */

#define AESA_DRNGSEED_SEED_Pos      0            /**< \brief (AESA_DRNGSEED) DRNG Seed */
#define AESA_DRNGSEED_SEED_Msk      (0xFFFFFFFFu << AESA_DRNGSEED_SEED_Pos)
#define AESA_DRNGSEED_SEED(value)   ((AESA_DRNGSEED_SEED_Msk & ((value) << AESA_DRNGSEED_SEED_Pos)))
#define AESA_DRNGSEED_MASK          0xFFFFFFFFu  /**< \brief (AESA_DRNGSEED) MASK Register */

/* -------- AESA_PARAMETER : (AESA Offset: 0xF8) (R/  32) Parameter Register -------- */
#define AESA_PARAMETER_OFFSET       0xF8         /**< \brief (AESA_PARAMETER offset) Parameter Register */
#define AESA_PARAMETER_RESETVALUE   0x00000112   /**< \brief (AESA_PARAMETER reset_value) Parameter Register */

#define AESA_PARAMETER_KEYSIZE_Pos  0            /**< \brief (AESA_PARAMETER) Maximum Key Size */
#define AESA_PARAMETER_KEYSIZE_Msk  (0x3u << AESA_PARAMETER_KEYSIZE_Pos)
#define AESA_PARAMETER_KEYSIZE(value) ((AESA_PARAMETER_KEYSIZE_Msk & ((value) << AESA_PARAMETER_KEYSIZE_Pos)))
#define AESA_PARAMETER_OPMODE_Pos   2            /**< \brief (AESA_PARAMETER) Maximum Number of Confidentiality Modes of Operation */
#define AESA_PARAMETER_OPMODE_Msk   (0x7u << AESA_PARAMETER_OPMODE_Pos)
#define AESA_PARAMETER_OPMODE(value) ((AESA_PARAMETER_OPMODE_Msk & ((value) << AESA_PARAMETER_OPMODE_Pos)))
#define AESA_PARAMETER_CTRMEAS_Pos  8            /**< \brief (AESA_PARAMETER) Countermeasures */
#define AESA_PARAMETER_CTRMEAS      (0x1u << AESA_PARAMETER_CTRMEAS_Pos)
#define AESA_PARAMETER_MASK         0x0000011Fu  /**< \brief (AESA_PARAMETER) MASK Register */

/* -------- AESA_VERSION : (AESA Offset: 0xFC) (R/  32) Version Register -------- */
#define AESA_VERSION_OFFSET         0xFC         /**< \brief (AESA_VERSION offset) Version Register */
#define AESA_VERSION_RESETVALUE     0x00000102   /**< \brief (AESA_VERSION reset_value) Version Register */

#define AESA_VERSION_VERSION_Pos    0            /**< \brief (AESA_VERSION) Version Number */
#define AESA_VERSION_VERSION_Msk    (0xFFFu << AESA_VERSION_VERSION_Pos)
#define AESA_VERSION_VERSION(value) ((AESA_VERSION_VERSION_Msk & ((value) << AESA_VERSION_VERSION_Pos)))
#define AESA_VERSION_VARIANT_Pos    16           /**< \brief (AESA_VERSION) Variant Number */
#define AESA_VERSION_VARIANT_Msk    (0xFu << AESA_VERSION_VARIANT_Pos)
#define AESA_VERSION_VARIANT(value) ((AESA_VERSION_VARIANT_Msk & ((value) << AESA_VERSION_VARIANT_Pos)))
#define AESA_VERSION_MASK           0x000F0FFFu  /**< \brief (AESA_VERSION) MASK Register */

/** \brief AesaInitvect hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
  WoReg   AESA_INITVECT;      /**< \brief (AESA Offset: 0x00) Initialization Vector Register */
} AesaInitvect;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/** \brief AesaKey hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
  WoReg   AESA_KEY;           /**< \brief (AESA Offset: 0x00) Key Register */
} AesaKey;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/** \brief AESA hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
  RwReg   AESA_CTRL;          /**< \brief (AESA Offset: 0x00) Control Register */
  RwReg   AESA_MODE;          /**< \brief (AESA Offset: 0x04) Mode Register */
  RwReg   AESA_DATABUFPTR;    /**< \brief (AESA Offset: 0x08) Data Buffer Pointer Register */
  RoReg   AESA_SR;            /**< \brief (AESA Offset: 0x0C) Status Register */
  WoReg   AESA_IER;           /**< \brief (AESA Offset: 0x10) Interrupt Enable Register */
  WoReg   AESA_IDR;           /**< \brief (AESA Offset: 0x14) Interrupt Disable Register */
  RoReg   AESA_IMR;           /**< \brief (AESA Offset: 0x18) Interrupt Mask Register */
  RoReg8  Reserved1[0x4];
  AesaKey AESA_KEY[8];        /**< \brief (AESA Offset: 0x20) AesaKey groups */
  AesaInitvect AESA_INITVECT[4];   /**< \brief (AESA Offset: 0x40) AesaInitvect groups */
  WoReg   AESA_IDATA;         /**< \brief (AESA Offset: 0x50) Input Data Register */
  RoReg8  Reserved2[0xC];
  RoReg   AESA_ODATA;         /**< \brief (AESA Offset: 0x60) Output Data Register */
  RoReg8  Reserved3[0xC];
  WoReg   AESA_DRNGSEED;      /**< \brief (AESA Offset: 0x70) DRNG Seed Register */
  RoReg8  Reserved4[0x84];
  RoReg   AESA_PARAMETER;     /**< \brief (AESA Offset: 0xF8) Parameter Register */
  RoReg   AESA_VERSION;       /**< \brief (AESA Offset: 0xFC) Version Register */
} Aesa;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/*@}*/

#endif /* _SAM4L_AESA_COMPONENT_ */
