/**
 * \file
 *
 * \brief Component description for ABDACB
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

#ifndef _SAM4L_ABDACB_COMPONENT_
#define _SAM4L_ABDACB_COMPONENT_

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR ABDACB */
/* ========================================================================== */
/** \addtogroup SAM4L_ABDACB Audio Bitstream DAC */
/*@{*/

#define REV_ABDACB                  0x100

/* -------- ABDACB_CR : (ABDACB Offset: 0x00) (R/W 32) Control Register -------- */
#define ABDACB_CR_OFFSET            0x00         /**< \brief (ABDACB_CR offset) Control Register */
#define ABDACB_CR_RESETVALUE        0x00000000   /**< \brief (ABDACB_CR reset_value) Control Register */

#define ABDACB_CR_EN_Pos            0            /**< \brief (ABDACB_CR) Enable */
#define ABDACB_CR_EN                (0x1u << ABDACB_CR_EN_Pos)
#define   ABDACB_CR_EN_0            (0x0u <<  0) /**< \brief (ABDACB_CR) Audio DAC is disabled */
#define   ABDACB_CR_EN_1            (0x1u <<  0) /**< \brief (ABDACB_CR) Audio DAC is enabled */
#define ABDACB_CR_SWAP_Pos          1            /**< \brief (ABDACB_CR) Swap Channels */
#define ABDACB_CR_SWAP              (0x1u << ABDACB_CR_SWAP_Pos)
#define   ABDACB_CR_SWAP_0          (0x0u <<  1) /**< \brief (ABDACB_CR) The CHANNEL0 and CHANNEL1 samples will not be swapped when writing the Audio DAC Sample Data Register (SDR) */
#define   ABDACB_CR_SWAP_1          (0x1u <<  1) /**< \brief (ABDACB_CR) The CHANNEL0 and CHANNEL1 samples will be swapped when writing the Audio DAC Sample Data Register (SDR) */
#define ABDACB_CR_ALTUPR_Pos        3            /**< \brief (ABDACB_CR) Alternative up-sampling ratio */
#define ABDACB_CR_ALTUPR            (0x1u << ABDACB_CR_ALTUPR_Pos)
#define ABDACB_CR_CMOC_Pos          4            /**< \brief (ABDACB_CR) Common mode offset control */
#define ABDACB_CR_CMOC              (0x1u << ABDACB_CR_CMOC_Pos)
#define ABDACB_CR_MONO_Pos          5            /**< \brief (ABDACB_CR) Mono mode */
#define ABDACB_CR_MONO              (0x1u << ABDACB_CR_MONO_Pos)
#define ABDACB_CR_SWRST_Pos         7            /**< \brief (ABDACB_CR) Software reset */
#define ABDACB_CR_SWRST             (0x1u << ABDACB_CR_SWRST_Pos)
#define ABDACB_CR_DATAFORMAT_Pos    16           /**< \brief (ABDACB_CR) Data word format */
#define ABDACB_CR_DATAFORMAT_Msk    (0x7u << ABDACB_CR_DATAFORMAT_Pos)
#define ABDACB_CR_DATAFORMAT(value) ((ABDACB_CR_DATAFORMAT_Msk & ((value) << ABDACB_CR_DATAFORMAT_Pos)))
#define ABDACB_CR_FS_Pos            24           /**< \brief (ABDACB_CR) Sampling frequency */
#define ABDACB_CR_FS_Msk            (0xFu << ABDACB_CR_FS_Pos)
#define ABDACB_CR_FS(value)         ((ABDACB_CR_FS_Msk & ((value) << ABDACB_CR_FS_Pos)))
#define ABDACB_CR_MASK              0x0F0700BBu  /**< \brief (ABDACB_CR) MASK Register */

/* -------- ABDACB_SDR0 : (ABDACB Offset: 0x04) (R/W 32) Sample Data Register 0 -------- */
#define ABDACB_SDR0_OFFSET          0x04         /**< \brief (ABDACB_SDR0 offset) Sample Data Register 0 */
#define ABDACB_SDR0_RESETVALUE      0x00000000   /**< \brief (ABDACB_SDR0 reset_value) Sample Data Register 0 */

#define ABDACB_SDR0_DATA_Pos        0            /**< \brief (ABDACB_SDR0) Sample Data */
#define ABDACB_SDR0_DATA_Msk        (0xFFFFFFFFu << ABDACB_SDR0_DATA_Pos)
#define ABDACB_SDR0_DATA(value)     ((ABDACB_SDR0_DATA_Msk & ((value) << ABDACB_SDR0_DATA_Pos)))
#define ABDACB_SDR0_MASK            0xFFFFFFFFu  /**< \brief (ABDACB_SDR0) MASK Register */

/* -------- ABDACB_SDR1 : (ABDACB Offset: 0x08) (R/W 32) Sample Data Register 1 -------- */
#define ABDACB_SDR1_OFFSET          0x08         /**< \brief (ABDACB_SDR1 offset) Sample Data Register 1 */
#define ABDACB_SDR1_RESETVALUE      0x00000000   /**< \brief (ABDACB_SDR1 reset_value) Sample Data Register 1 */

#define ABDACB_SDR1_DATA_Pos        0            /**< \brief (ABDACB_SDR1) Sample Data */
#define ABDACB_SDR1_DATA_Msk        (0xFFFFFFFFu << ABDACB_SDR1_DATA_Pos)
#define ABDACB_SDR1_DATA(value)     ((ABDACB_SDR1_DATA_Msk & ((value) << ABDACB_SDR1_DATA_Pos)))
#define ABDACB_SDR1_MASK            0xFFFFFFFFu  /**< \brief (ABDACB_SDR1) MASK Register */

/* -------- ABDACB_VCR0 : (ABDACB Offset: 0x0C) (R/W 32) Volume Control Register 0 -------- */
#define ABDACB_VCR0_OFFSET          0x0C         /**< \brief (ABDACB_VCR0 offset) Volume Control Register 0 */
#define ABDACB_VCR0_RESETVALUE      0x00000000   /**< \brief (ABDACB_VCR0 reset_value) Volume Control Register 0 */

#define ABDACB_VCR0_VOLUME_Pos      0            /**< \brief (ABDACB_VCR0) Volume Control */
#define ABDACB_VCR0_VOLUME_Msk      (0x7FFFu << ABDACB_VCR0_VOLUME_Pos)
#define ABDACB_VCR0_VOLUME(value)   ((ABDACB_VCR0_VOLUME_Msk & ((value) << ABDACB_VCR0_VOLUME_Pos)))
#define ABDACB_VCR0_MUTE_Pos        31           /**< \brief (ABDACB_VCR0) Mute */
#define ABDACB_VCR0_MUTE            (0x1u << ABDACB_VCR0_MUTE_Pos)
#define ABDACB_VCR0_MASK            0x80007FFFu  /**< \brief (ABDACB_VCR0) MASK Register */

/* -------- ABDACB_VCR1 : (ABDACB Offset: 0x10) (R/W 32) Volume Control Register 1 -------- */
#define ABDACB_VCR1_OFFSET          0x10         /**< \brief (ABDACB_VCR1 offset) Volume Control Register 1 */
#define ABDACB_VCR1_RESETVALUE      0x00000000   /**< \brief (ABDACB_VCR1 reset_value) Volume Control Register 1 */

#define ABDACB_VCR1_VOLUME_Pos      0            /**< \brief (ABDACB_VCR1) Volume Control */
#define ABDACB_VCR1_VOLUME_Msk      (0x7FFFu << ABDACB_VCR1_VOLUME_Pos)
#define ABDACB_VCR1_VOLUME(value)   ((ABDACB_VCR1_VOLUME_Msk & ((value) << ABDACB_VCR1_VOLUME_Pos)))
#define ABDACB_VCR1_MUTE_Pos        31           /**< \brief (ABDACB_VCR1) Mute */
#define ABDACB_VCR1_MUTE            (0x1u << ABDACB_VCR1_MUTE_Pos)
#define ABDACB_VCR1_MASK            0x80007FFFu  /**< \brief (ABDACB_VCR1) MASK Register */

/* -------- ABDACB_IER : (ABDACB Offset: 0x14) ( /W 32) Interrupt Enable Register -------- */
#define ABDACB_IER_OFFSET           0x14         /**< \brief (ABDACB_IER offset) Interrupt Enable Register */
#define ABDACB_IER_RESETVALUE       0x00000000   /**< \brief (ABDACB_IER reset_value) Interrupt Enable Register */

#define ABDACB_IER_TXRDY_Pos        1            /**< \brief (ABDACB_IER) Transmit Ready Interrupt Enable */
#define ABDACB_IER_TXRDY            (0x1u << ABDACB_IER_TXRDY_Pos)
#define   ABDACB_IER_TXRDY_0        (0x0u <<  1) /**< \brief (ABDACB_IER) No effect */
#define   ABDACB_IER_TXRDY_1        (0x1u <<  1) /**< \brief (ABDACB_IER) Enables the Audio DAC TX Ready interrupt */
#define ABDACB_IER_TXUR_Pos         2            /**< \brief (ABDACB_IER) Transmit Underrun Interrupt Enable */
#define ABDACB_IER_TXUR             (0x1u << ABDACB_IER_TXUR_Pos)
#define   ABDACB_IER_TXUR_0         (0x0u <<  2) /**< \brief (ABDACB_IER) No effect */
#define   ABDACB_IER_TXUR_1         (0x1u <<  2) /**< \brief (ABDACB_IER) Enables the Audio DAC Underrun interrupt */
#define ABDACB_IER_MASK             0x00000006u  /**< \brief (ABDACB_IER) MASK Register */

/* -------- ABDACB_IDR : (ABDACB Offset: 0x18) ( /W 32) Interupt Disable Register -------- */
#define ABDACB_IDR_OFFSET           0x18         /**< \brief (ABDACB_IDR offset) Interupt Disable Register */
#define ABDACB_IDR_RESETVALUE       0x00000000   /**< \brief (ABDACB_IDR reset_value) Interupt Disable Register */

#define ABDACB_IDR_TXRDY_Pos        1            /**< \brief (ABDACB_IDR) Transmit Ready Interrupt Disable */
#define ABDACB_IDR_TXRDY            (0x1u << ABDACB_IDR_TXRDY_Pos)
#define   ABDACB_IDR_TXRDY_0        (0x0u <<  1) /**< \brief (ABDACB_IDR) No effect */
#define   ABDACB_IDR_TXRDY_1        (0x1u <<  1) /**< \brief (ABDACB_IDR) Disable the Audio DAC TX Ready interrupt */
#define ABDACB_IDR_TXUR_Pos         2            /**< \brief (ABDACB_IDR) Transmit Underrun Interrupt Disable */
#define ABDACB_IDR_TXUR             (0x1u << ABDACB_IDR_TXUR_Pos)
#define   ABDACB_IDR_TXUR_0         (0x0u <<  2) /**< \brief (ABDACB_IDR) No effect */
#define   ABDACB_IDR_TXUR_1         (0x1u <<  2) /**< \brief (ABDACB_IDR) Disable the Audio DAC Underrun interrupt */
#define ABDACB_IDR_MASK             0x00000006u  /**< \brief (ABDACB_IDR) MASK Register */

/* -------- ABDACB_IMR : (ABDACB Offset: 0x1C) (R/  32) Interrupt Mask Register -------- */
#define ABDACB_IMR_OFFSET           0x1C         /**< \brief (ABDACB_IMR offset) Interrupt Mask Register */
#define ABDACB_IMR_RESETVALUE       0x00000000   /**< \brief (ABDACB_IMR reset_value) Interrupt Mask Register */

#define ABDACB_IMR_TXRDY_Pos        1            /**< \brief (ABDACB_IMR) Transmit Ready Interrupt Mask */
#define ABDACB_IMR_TXRDY            (0x1u << ABDACB_IMR_TXRDY_Pos)
#define   ABDACB_IMR_TXRDY_0        (0x0u <<  1) /**< \brief (ABDACB_IMR) The Audio DAC TX Ready interrupt is disabled */
#define   ABDACB_IMR_TXRDY_1        (0x1u <<  1) /**< \brief (ABDACB_IMR) The Audio DAC TX Ready interrupt is enabled */
#define ABDACB_IMR_TXUR_Pos         2            /**< \brief (ABDACB_IMR) Transmit Underrun Interrupt Mask */
#define ABDACB_IMR_TXUR             (0x1u << ABDACB_IMR_TXUR_Pos)
#define   ABDACB_IMR_TXUR_0         (0x0u <<  2) /**< \brief (ABDACB_IMR) The Audio DAC Underrun interrupt is disabled */
#define   ABDACB_IMR_TXUR_1         (0x1u <<  2) /**< \brief (ABDACB_IMR) The Audio DAC Underrun interrupt is enabled */
#define ABDACB_IMR_MASK             0x00000006u  /**< \brief (ABDACB_IMR) MASK Register */

/* -------- ABDACB_SR : (ABDACB Offset: 0x20) (R/  32) Status Register -------- */
#define ABDACB_SR_OFFSET            0x20         /**< \brief (ABDACB_SR offset) Status Register */
#define ABDACB_SR_RESETVALUE        0x00000000   /**< \brief (ABDACB_SR reset_value) Status Register */

#define ABDACB_SR_BUSY_Pos          0            /**< \brief (ABDACB_SR) ABDACB Busy */
#define ABDACB_SR_BUSY              (0x1u << ABDACB_SR_BUSY_Pos)
#define ABDACB_SR_TXRDY_Pos         1            /**< \brief (ABDACB_SR) Transmit Ready */
#define ABDACB_SR_TXRDY             (0x1u << ABDACB_SR_TXRDY_Pos)
#define   ABDACB_SR_TXRDY_0         (0x0u <<  1) /**< \brief (ABDACB_SR) No Audio DAC TX Ready has occured since the last time ISR was read or since reset */
#define   ABDACB_SR_TXRDY_1         (0x1u <<  1) /**< \brief (ABDACB_SR) At least one Audio DAC TX Ready has occured since the last time ISR was read or since reset */
#define ABDACB_SR_TXUR_Pos          2            /**< \brief (ABDACB_SR) Transmit Underrun */
#define ABDACB_SR_TXUR              (0x1u << ABDACB_SR_TXUR_Pos)
#define   ABDACB_SR_TXUR_0          (0x0u <<  2) /**< \brief (ABDACB_SR) No Audio DAC Underrun has occured since the last time ISR was read or since reset */
#define   ABDACB_SR_TXUR_1          (0x1u <<  2) /**< \brief (ABDACB_SR) At least one Audio DAC Underrun has occured since the last time ISR was read or since reset */
#define ABDACB_SR_MASK              0x00000007u  /**< \brief (ABDACB_SR) MASK Register */

/* -------- ABDACB_SCR : (ABDACB Offset: 0x24) ( /W 32) Status Clear Register -------- */
#define ABDACB_SCR_OFFSET           0x24         /**< \brief (ABDACB_SCR offset) Status Clear Register */
#define ABDACB_SCR_RESETVALUE       0x00000000   /**< \brief (ABDACB_SCR reset_value) Status Clear Register */

#define ABDACB_SCR_TXRDY_Pos        1            /**< \brief (ABDACB_SCR) Transmit Ready Interrupt Clear */
#define ABDACB_SCR_TXRDY            (0x1u << ABDACB_SCR_TXRDY_Pos)
#define   ABDACB_SCR_TXRDY_0        (0x0u <<  1) /**< \brief (ABDACB_SCR) No effect */
#define   ABDACB_SCR_TXRDY_1        (0x1u <<  1) /**< \brief (ABDACB_SCR) Clear the Audio DAC TX Ready interrupt */
#define ABDACB_SCR_TXUR_Pos         2            /**< \brief (ABDACB_SCR) Transmit Underrun Interrupt Clear */
#define ABDACB_SCR_TXUR             (0x1u << ABDACB_SCR_TXUR_Pos)
#define   ABDACB_SCR_TXUR_0         (0x0u <<  2) /**< \brief (ABDACB_SCR) No effect */
#define   ABDACB_SCR_TXUR_1         (0x1u <<  2) /**< \brief (ABDACB_SCR) Clear the Audio DAC Underrun interrupt */
#define ABDACB_SCR_MASK             0x00000006u  /**< \brief (ABDACB_SCR) MASK Register */

/* -------- ABDACB_PARAMETER : (ABDACB Offset: 0x28) (R/  32) Parameter Register -------- */
#define ABDACB_PARAMETER_OFFSET     0x28         /**< \brief (ABDACB_PARAMETER offset) Parameter Register */
#define ABDACB_PARAMETER_RESETVALUE 0x00000000   /**< \brief (ABDACB_PARAMETER reset_value) Parameter Register */
#define ABDACB_PARAMETER_MASK       0xFFFFFFFFu  /**< \brief (ABDACB_PARAMETER) MASK Register */

/* -------- ABDACB_VERSION : (ABDACB Offset: 0x2C) (R/  32) Version Register -------- */
#define ABDACB_VERSION_OFFSET       0x2C         /**< \brief (ABDACB_VERSION offset) Version Register */
#define ABDACB_VERSION_RESETVALUE   0x00000100   /**< \brief (ABDACB_VERSION reset_value) Version Register */

#define ABDACB_VERSION_VERSION_Pos  0            /**< \brief (ABDACB_VERSION) Version Number */
#define ABDACB_VERSION_VERSION_Msk  (0xFFFu << ABDACB_VERSION_VERSION_Pos)
#define ABDACB_VERSION_VERSION(value) ((ABDACB_VERSION_VERSION_Msk & ((value) << ABDACB_VERSION_VERSION_Pos)))
#define ABDACB_VERSION_VARIANT_Pos  16           /**< \brief (ABDACB_VERSION) Variant Number */
#define ABDACB_VERSION_VARIANT_Msk  (0xFu << ABDACB_VERSION_VARIANT_Pos)
#define ABDACB_VERSION_VARIANT(value) ((ABDACB_VERSION_VARIANT_Msk & ((value) << ABDACB_VERSION_VARIANT_Pos)))
#define ABDACB_VERSION_MASK         0x000F0FFFu  /**< \brief (ABDACB_VERSION) MASK Register */

/** \brief ABDACB hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
  RwReg   ABDACB_CR;          /**< \brief (ABDACB Offset: 0x00) Control Register */
  RwReg   ABDACB_SDR0;        /**< \brief (ABDACB Offset: 0x04) Sample Data Register 0 */
  RwReg   ABDACB_SDR1;        /**< \brief (ABDACB Offset: 0x08) Sample Data Register 1 */
  RwReg   ABDACB_VCR0;        /**< \brief (ABDACB Offset: 0x0C) Volume Control Register 0 */
  RwReg   ABDACB_VCR1;        /**< \brief (ABDACB Offset: 0x10) Volume Control Register 1 */
  WoReg   ABDACB_IER;         /**< \brief (ABDACB Offset: 0x14) Interrupt Enable Register */
  WoReg   ABDACB_IDR;         /**< \brief (ABDACB Offset: 0x18) Interupt Disable Register */
  RoReg   ABDACB_IMR;         /**< \brief (ABDACB Offset: 0x1C) Interrupt Mask Register */
  RoReg   ABDACB_SR;          /**< \brief (ABDACB Offset: 0x20) Status Register */
  WoReg   ABDACB_SCR;         /**< \brief (ABDACB Offset: 0x24) Status Clear Register */
  RoReg   ABDACB_PARAMETER;   /**< \brief (ABDACB Offset: 0x28) Parameter Register */
  RoReg   ABDACB_VERSION;     /**< \brief (ABDACB Offset: 0x2C) Version Register */
} Abdacb;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/*@}*/

#endif /* _SAM4L_ABDACB_COMPONENT_ */
