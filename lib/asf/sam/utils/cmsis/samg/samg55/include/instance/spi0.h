/**
 * \file
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

#ifndef _SAMG55_SPI0_INSTANCE_
#define _SAMG55_SPI0_INSTANCE_

/* ========== Register definition for SPI0 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_SPI0_CR                    (0x4000C400U) /**< \brief (SPI0) SPI Control Register */
  #define REG_SPI0_MR                    (0x4000C404U) /**< \brief (SPI0) SPI Mode Register */
  #define REG_SPI0_RDR                   (0x4000C408U) /**< \brief (SPI0) SPI Receive Data Register */
  #define REG_SPI0_TDR                   (0x4000C40CU) /**< \brief (SPI0) SPI Transmit Data Register */
  #define REG_SPI0_SR                    (0x4000C410U) /**< \brief (SPI0) SPI Status Register */
  #define REG_SPI0_IER                   (0x4000C414U) /**< \brief (SPI0) SPI Interrupt Enable Register */
  #define REG_SPI0_IDR                   (0x4000C418U) /**< \brief (SPI0) SPI Interrupt Disable Register */
  #define REG_SPI0_IMR                   (0x4000C41CU) /**< \brief (SPI0) SPI Interrupt Mask Register */
  #define REG_SPI0_CSR                   (0x4000C430U) /**< \brief (SPI0) SPI Chip Select Register */
  #define REG_SPI0_CMPR                  (0x4000C448U) /**< \brief (SPI0) SPI Comparison Register */
  #define REG_SPI0_WPMR                  (0x4000C4E4U) /**< \brief (SPI0) SPI Write Protection Mode Register */
  #define REG_SPI0_WPSR                  (0x4000C4E8U) /**< \brief (SPI0) SPI Write Protection Status Register */
  #define REG_SPI0_RPR                   (0x4000C500U) /**< \brief (SPI0) Receive Pointer Register */
  #define REG_SPI0_RCR                   (0x4000C504U) /**< \brief (SPI0) Receive Counter Register */
  #define REG_SPI0_TPR                   (0x4000C508U) /**< \brief (SPI0) Transmit Pointer Register */
  #define REG_SPI0_TCR                   (0x4000C50CU) /**< \brief (SPI0) Transmit Counter Register */
  #define REG_SPI0_RNPR                  (0x4000C510U) /**< \brief (SPI0) Receive Next Pointer Register */
  #define REG_SPI0_RNCR                  (0x4000C514U) /**< \brief (SPI0) Receive Next Counter Register */
  #define REG_SPI0_TNPR                  (0x4000C518U) /**< \brief (SPI0) Transmit Next Pointer Register */
  #define REG_SPI0_TNCR                  (0x4000C51CU) /**< \brief (SPI0) Transmit Next Counter Register */
  #define REG_SPI0_PTCR                  (0x4000C520U) /**< \brief (SPI0) Transfer Control Register */
  #define REG_SPI0_PTSR                  (0x4000C524U) /**< \brief (SPI0) Transfer Status Register */
#else
  #define REG_SPI0_CR   (*(__O  uint32_t*)0x4000C400U) /**< \brief (SPI0) SPI Control Register */
  #define REG_SPI0_MR   (*(__IO uint32_t*)0x4000C404U) /**< \brief (SPI0) SPI Mode Register */
  #define REG_SPI0_RDR  (*(__I  uint32_t*)0x4000C408U) /**< \brief (SPI0) SPI Receive Data Register */
  #define REG_SPI0_TDR  (*(__O  uint32_t*)0x4000C40CU) /**< \brief (SPI0) SPI Transmit Data Register */
  #define REG_SPI0_SR   (*(__I  uint32_t*)0x4000C410U) /**< \brief (SPI0) SPI Status Register */
  #define REG_SPI0_IER  (*(__O  uint32_t*)0x4000C414U) /**< \brief (SPI0) SPI Interrupt Enable Register */
  #define REG_SPI0_IDR  (*(__O  uint32_t*)0x4000C418U) /**< \brief (SPI0) SPI Interrupt Disable Register */
  #define REG_SPI0_IMR  (*(__I  uint32_t*)0x4000C41CU) /**< \brief (SPI0) SPI Interrupt Mask Register */
  #define REG_SPI0_CSR  (*(__IO uint32_t*)0x4000C430U) /**< \brief (SPI0) SPI Chip Select Register */
  #define REG_SPI0_CMPR (*(__IO uint32_t*)0x4000C448U) /**< \brief (SPI0) SPI Comparison Register */
  #define REG_SPI0_WPMR (*(__IO uint32_t*)0x4000C4E4U) /**< \brief (SPI0) SPI Write Protection Mode Register */
  #define REG_SPI0_WPSR (*(__I  uint32_t*)0x4000C4E8U) /**< \brief (SPI0) SPI Write Protection Status Register */
  #define REG_SPI0_RPR  (*(__IO uint32_t*)0x4000C500U) /**< \brief (SPI0) Receive Pointer Register */
  #define REG_SPI0_RCR  (*(__IO uint32_t*)0x4000C504U) /**< \brief (SPI0) Receive Counter Register */
  #define REG_SPI0_TPR  (*(__IO uint32_t*)0x4000C508U) /**< \brief (SPI0) Transmit Pointer Register */
  #define REG_SPI0_TCR  (*(__IO uint32_t*)0x4000C50CU) /**< \brief (SPI0) Transmit Counter Register */
  #define REG_SPI0_RNPR (*(__IO uint32_t*)0x4000C510U) /**< \brief (SPI0) Receive Next Pointer Register */
  #define REG_SPI0_RNCR (*(__IO uint32_t*)0x4000C514U) /**< \brief (SPI0) Receive Next Counter Register */
  #define REG_SPI0_TNPR (*(__IO uint32_t*)0x4000C518U) /**< \brief (SPI0) Transmit Next Pointer Register */
  #define REG_SPI0_TNCR (*(__IO uint32_t*)0x4000C51CU) /**< \brief (SPI0) Transmit Next Counter Register */
  #define REG_SPI0_PTCR (*(__O  uint32_t*)0x4000C520U) /**< \brief (SPI0) Transfer Control Register */
  #define REG_SPI0_PTSR (*(__I  uint32_t*)0x4000C524U) /**< \brief (SPI0) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAMG55_SPI0_INSTANCE_ */
