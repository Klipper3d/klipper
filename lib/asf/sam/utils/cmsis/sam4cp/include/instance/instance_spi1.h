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

#ifndef _SAM4CP_SPI1_INSTANCE_
#define _SAM4CP_SPI1_INSTANCE_

/* ========== Register definition for SPI1 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_SPI1_CR                    (0x48000000U) /**< \brief (SPI1) Control Register */
  #define REG_SPI1_MR                    (0x48000004U) /**< \brief (SPI1) Mode Register */
  #define REG_SPI1_RDR                   (0x48000008U) /**< \brief (SPI1) Receive Data Register */
  #define REG_SPI1_TDR                   (0x4800000CU) /**< \brief (SPI1) Transmit Data Register */
  #define REG_SPI1_SR                    (0x48000010U) /**< \brief (SPI1) Status Register */
  #define REG_SPI1_IER                   (0x48000014U) /**< \brief (SPI1) Interrupt Enable Register */
  #define REG_SPI1_IDR                   (0x48000018U) /**< \brief (SPI1) Interrupt Disable Register */
  #define REG_SPI1_IMR                   (0x4800001CU) /**< \brief (SPI1) Interrupt Mask Register */
  #define REG_SPI1_CSR                   (0x48000030U) /**< \brief (SPI1) Chip Select Register */
  #define REG_SPI1_WPMR                  (0x480000E4U) /**< \brief (SPI1) Write Protection Control Register */
  #define REG_SPI1_WPSR                  (0x480000E8U) /**< \brief (SPI1) Write Protection Status Register */
  #define REG_SPI1_RPR                   (0x48000100U) /**< \brief (SPI1) Receive Pointer Register */
  #define REG_SPI1_RCR                   (0x48000104U) /**< \brief (SPI1) Receive Counter Register */
  #define REG_SPI1_TPR                   (0x48000108U) /**< \brief (SPI1) Transmit Pointer Register */
  #define REG_SPI1_TCR                   (0x4800010CU) /**< \brief (SPI1) Transmit Counter Register */
  #define REG_SPI1_RNPR                  (0x48000110U) /**< \brief (SPI1) Receive Next Pointer Register */
  #define REG_SPI1_RNCR                  (0x48000114U) /**< \brief (SPI1) Receive Next Counter Register */
  #define REG_SPI1_TNPR                  (0x48000118U) /**< \brief (SPI1) Transmit Next Pointer Register */
  #define REG_SPI1_TNCR                  (0x4800011CU) /**< \brief (SPI1) Transmit Next Counter Register */
  #define REG_SPI1_PTCR                  (0x48000120U) /**< \brief (SPI1) Transfer Control Register */
  #define REG_SPI1_PTSR                  (0x48000124U) /**< \brief (SPI1) Transfer Status Register */
#else
  #define REG_SPI1_CR   (*(__O  uint32_t*)0x48000000U) /**< \brief (SPI1) Control Register */
  #define REG_SPI1_MR   (*(__IO uint32_t*)0x48000004U) /**< \brief (SPI1) Mode Register */
  #define REG_SPI1_RDR  (*(__I  uint32_t*)0x48000008U) /**< \brief (SPI1) Receive Data Register */
  #define REG_SPI1_TDR  (*(__O  uint32_t*)0x4800000CU) /**< \brief (SPI1) Transmit Data Register */
  #define REG_SPI1_SR   (*(__I  uint32_t*)0x48000010U) /**< \brief (SPI1) Status Register */
  #define REG_SPI1_IER  (*(__O  uint32_t*)0x48000014U) /**< \brief (SPI1) Interrupt Enable Register */
  #define REG_SPI1_IDR  (*(__O  uint32_t*)0x48000018U) /**< \brief (SPI1) Interrupt Disable Register */
  #define REG_SPI1_IMR  (*(__I  uint32_t*)0x4800001CU) /**< \brief (SPI1) Interrupt Mask Register */
  #define REG_SPI1_CSR  (*(__IO uint32_t*)0x48000030U) /**< \brief (SPI1) Chip Select Register */
  #define REG_SPI1_WPMR (*(__IO uint32_t*)0x480000E4U) /**< \brief (SPI1) Write Protection Control Register */
  #define REG_SPI1_WPSR (*(__I  uint32_t*)0x480000E8U) /**< \brief (SPI1) Write Protection Status Register */
  #define REG_SPI1_RPR  (*(__IO uint32_t*)0x48000100U) /**< \brief (SPI1) Receive Pointer Register */
  #define REG_SPI1_RCR  (*(__IO uint32_t*)0x48000104U) /**< \brief (SPI1) Receive Counter Register */
  #define REG_SPI1_TPR  (*(__IO uint32_t*)0x48000108U) /**< \brief (SPI1) Transmit Pointer Register */
  #define REG_SPI1_TCR  (*(__IO uint32_t*)0x4800010CU) /**< \brief (SPI1) Transmit Counter Register */
  #define REG_SPI1_RNPR (*(__IO uint32_t*)0x48000110U) /**< \brief (SPI1) Receive Next Pointer Register */
  #define REG_SPI1_RNCR (*(__IO uint32_t*)0x48000114U) /**< \brief (SPI1) Receive Next Counter Register */
  #define REG_SPI1_TNPR (*(__IO uint32_t*)0x48000118U) /**< \brief (SPI1) Transmit Next Pointer Register */
  #define REG_SPI1_TNCR (*(__IO uint32_t*)0x4800011CU) /**< \brief (SPI1) Transmit Next Counter Register */
  #define REG_SPI1_PTCR (*(__O  uint32_t*)0x48000120U) /**< \brief (SPI1) Transfer Control Register */
  #define REG_SPI1_PTSR (*(__I  uint32_t*)0x48000124U) /**< \brief (SPI1) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAM4CP_SPI1_INSTANCE_ */
