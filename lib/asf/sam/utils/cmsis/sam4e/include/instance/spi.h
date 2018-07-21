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

#ifndef _SAM4E_SPI_INSTANCE_
#define _SAM4E_SPI_INSTANCE_

/* ========== Register definition for SPI peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_SPI_CR                    (0x40088000U) /**< \brief (SPI) Control Register */
  #define REG_SPI_MR                    (0x40088004U) /**< \brief (SPI) Mode Register */
  #define REG_SPI_RDR                   (0x40088008U) /**< \brief (SPI) Receive Data Register */
  #define REG_SPI_TDR                   (0x4008800CU) /**< \brief (SPI) Transmit Data Register */
  #define REG_SPI_SR                    (0x40088010U) /**< \brief (SPI) Status Register */
  #define REG_SPI_IER                   (0x40088014U) /**< \brief (SPI) Interrupt Enable Register */
  #define REG_SPI_IDR                   (0x40088018U) /**< \brief (SPI) Interrupt Disable Register */
  #define REG_SPI_IMR                   (0x4008801CU) /**< \brief (SPI) Interrupt Mask Register */
  #define REG_SPI_CSR                   (0x40088030U) /**< \brief (SPI) Chip Select Register */
  #define REG_SPI_WPMR                  (0x400880E4U) /**< \brief (SPI) Write Protection Mode Register */
  #define REG_SPI_WPSR                  (0x400880E8U) /**< \brief (SPI) Write Protection Status Register */
  #define REG_SPI_RPR                   (0x40088100U) /**< \brief (SPI) Receive Pointer Register */
  #define REG_SPI_RCR                   (0x40088104U) /**< \brief (SPI) Receive Counter Register */
  #define REG_SPI_TPR                   (0x40088108U) /**< \brief (SPI) Transmit Pointer Register */
  #define REG_SPI_TCR                   (0x4008810CU) /**< \brief (SPI) Transmit Counter Register */
  #define REG_SPI_RNPR                  (0x40088110U) /**< \brief (SPI) Receive Next Pointer Register */
  #define REG_SPI_RNCR                  (0x40088114U) /**< \brief (SPI) Receive Next Counter Register */
  #define REG_SPI_TNPR                  (0x40088118U) /**< \brief (SPI) Transmit Next Pointer Register */
  #define REG_SPI_TNCR                  (0x4008811CU) /**< \brief (SPI) Transmit Next Counter Register */
  #define REG_SPI_PTCR                  (0x40088120U) /**< \brief (SPI) Transfer Control Register */
  #define REG_SPI_PTSR                  (0x40088124U) /**< \brief (SPI) Transfer Status Register */
#else
  #define REG_SPI_CR   (*(__O  uint32_t*)0x40088000U) /**< \brief (SPI) Control Register */
  #define REG_SPI_MR   (*(__IO uint32_t*)0x40088004U) /**< \brief (SPI) Mode Register */
  #define REG_SPI_RDR  (*(__I  uint32_t*)0x40088008U) /**< \brief (SPI) Receive Data Register */
  #define REG_SPI_TDR  (*(__O  uint32_t*)0x4008800CU) /**< \brief (SPI) Transmit Data Register */
  #define REG_SPI_SR   (*(__I  uint32_t*)0x40088010U) /**< \brief (SPI) Status Register */
  #define REG_SPI_IER  (*(__O  uint32_t*)0x40088014U) /**< \brief (SPI) Interrupt Enable Register */
  #define REG_SPI_IDR  (*(__O  uint32_t*)0x40088018U) /**< \brief (SPI) Interrupt Disable Register */
  #define REG_SPI_IMR  (*(__I  uint32_t*)0x4008801CU) /**< \brief (SPI) Interrupt Mask Register */
  #define REG_SPI_CSR  (*(__IO uint32_t*)0x40088030U) /**< \brief (SPI) Chip Select Register */
  #define REG_SPI_WPMR (*(__IO uint32_t*)0x400880E4U) /**< \brief (SPI) Write Protection Mode Register */
  #define REG_SPI_WPSR (*(__I  uint32_t*)0x400880E8U) /**< \brief (SPI) Write Protection Status Register */
  #define REG_SPI_RPR  (*(__IO uint32_t*)0x40088100U) /**< \brief (SPI) Receive Pointer Register */
  #define REG_SPI_RCR  (*(__IO uint32_t*)0x40088104U) /**< \brief (SPI) Receive Counter Register */
  #define REG_SPI_TPR  (*(__IO uint32_t*)0x40088108U) /**< \brief (SPI) Transmit Pointer Register */
  #define REG_SPI_TCR  (*(__IO uint32_t*)0x4008810CU) /**< \brief (SPI) Transmit Counter Register */
  #define REG_SPI_RNPR (*(__IO uint32_t*)0x40088110U) /**< \brief (SPI) Receive Next Pointer Register */
  #define REG_SPI_RNCR (*(__IO uint32_t*)0x40088114U) /**< \brief (SPI) Receive Next Counter Register */
  #define REG_SPI_TNPR (*(__IO uint32_t*)0x40088118U) /**< \brief (SPI) Transmit Next Pointer Register */
  #define REG_SPI_TNCR (*(__IO uint32_t*)0x4008811CU) /**< \brief (SPI) Transmit Next Counter Register */
  #define REG_SPI_PTCR (*(__O  uint32_t*)0x40088120U) /**< \brief (SPI) Transfer Control Register */
  #define REG_SPI_PTSR (*(__I  uint32_t*)0x40088124U) /**< \brief (SPI) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAM4E_SPI_INSTANCE_ */
