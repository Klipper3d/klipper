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

#ifndef _SAMG55_SPI7_INSTANCE_
#define _SAMG55_SPI7_INSTANCE_

/* ========== Register definition for SPI7 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_SPI7_CR                    (0x40034400U) /**< \brief (SPI7) SPI Control Register */
  #define REG_SPI7_MR                    (0x40034404U) /**< \brief (SPI7) SPI Mode Register */
  #define REG_SPI7_RDR                   (0x40034408U) /**< \brief (SPI7) SPI Receive Data Register */
  #define REG_SPI7_TDR                   (0x4003440CU) /**< \brief (SPI7) SPI Transmit Data Register */
  #define REG_SPI7_SR                    (0x40034410U) /**< \brief (SPI7) SPI Status Register */
  #define REG_SPI7_IER                   (0x40034414U) /**< \brief (SPI7) SPI Interrupt Enable Register */
  #define REG_SPI7_IDR                   (0x40034418U) /**< \brief (SPI7) SPI Interrupt Disable Register */
  #define REG_SPI7_IMR                   (0x4003441CU) /**< \brief (SPI7) SPI Interrupt Mask Register */
  #define REG_SPI7_CSR                   (0x40034430U) /**< \brief (SPI7) SPI Chip Select Register */
  #define REG_SPI7_CMPR                  (0x40034448U) /**< \brief (SPI7) SPI Comparison Register */
  #define REG_SPI7_WPMR                  (0x400344E4U) /**< \brief (SPI7) SPI Write Protection Mode Register */
  #define REG_SPI7_WPSR                  (0x400344E8U) /**< \brief (SPI7) SPI Write Protection Status Register */
  #define REG_SPI7_RPR                   (0x40034500U) /**< \brief (SPI7) Receive Pointer Register */
  #define REG_SPI7_RCR                   (0x40034504U) /**< \brief (SPI7) Receive Counter Register */
  #define REG_SPI7_TPR                   (0x40034508U) /**< \brief (SPI7) Transmit Pointer Register */
  #define REG_SPI7_TCR                   (0x4003450CU) /**< \brief (SPI7) Transmit Counter Register */
  #define REG_SPI7_RNPR                  (0x40034510U) /**< \brief (SPI7) Receive Next Pointer Register */
  #define REG_SPI7_RNCR                  (0x40034514U) /**< \brief (SPI7) Receive Next Counter Register */
  #define REG_SPI7_TNPR                  (0x40034518U) /**< \brief (SPI7) Transmit Next Pointer Register */
  #define REG_SPI7_TNCR                  (0x4003451CU) /**< \brief (SPI7) Transmit Next Counter Register */
  #define REG_SPI7_PTCR                  (0x40034520U) /**< \brief (SPI7) Transfer Control Register */
  #define REG_SPI7_PTSR                  (0x40034524U) /**< \brief (SPI7) Transfer Status Register */
#else
  #define REG_SPI7_CR   (*(__O  uint32_t*)0x40034400U) /**< \brief (SPI7) SPI Control Register */
  #define REG_SPI7_MR   (*(__IO uint32_t*)0x40034404U) /**< \brief (SPI7) SPI Mode Register */
  #define REG_SPI7_RDR  (*(__I  uint32_t*)0x40034408U) /**< \brief (SPI7) SPI Receive Data Register */
  #define REG_SPI7_TDR  (*(__O  uint32_t*)0x4003440CU) /**< \brief (SPI7) SPI Transmit Data Register */
  #define REG_SPI7_SR   (*(__I  uint32_t*)0x40034410U) /**< \brief (SPI7) SPI Status Register */
  #define REG_SPI7_IER  (*(__O  uint32_t*)0x40034414U) /**< \brief (SPI7) SPI Interrupt Enable Register */
  #define REG_SPI7_IDR  (*(__O  uint32_t*)0x40034418U) /**< \brief (SPI7) SPI Interrupt Disable Register */
  #define REG_SPI7_IMR  (*(__I  uint32_t*)0x4003441CU) /**< \brief (SPI7) SPI Interrupt Mask Register */
  #define REG_SPI7_CSR  (*(__IO uint32_t*)0x40034430U) /**< \brief (SPI7) SPI Chip Select Register */
  #define REG_SPI7_CMPR (*(__IO uint32_t*)0x40034448U) /**< \brief (SPI7) SPI Comparison Register */
  #define REG_SPI7_WPMR (*(__IO uint32_t*)0x400344E4U) /**< \brief (SPI7) SPI Write Protection Mode Register */
  #define REG_SPI7_WPSR (*(__I  uint32_t*)0x400344E8U) /**< \brief (SPI7) SPI Write Protection Status Register */
  #define REG_SPI7_RPR  (*(__IO uint32_t*)0x40034500U) /**< \brief (SPI7) Receive Pointer Register */
  #define REG_SPI7_RCR  (*(__IO uint32_t*)0x40034504U) /**< \brief (SPI7) Receive Counter Register */
  #define REG_SPI7_TPR  (*(__IO uint32_t*)0x40034508U) /**< \brief (SPI7) Transmit Pointer Register */
  #define REG_SPI7_TCR  (*(__IO uint32_t*)0x4003450CU) /**< \brief (SPI7) Transmit Counter Register */
  #define REG_SPI7_RNPR (*(__IO uint32_t*)0x40034510U) /**< \brief (SPI7) Receive Next Pointer Register */
  #define REG_SPI7_RNCR (*(__IO uint32_t*)0x40034514U) /**< \brief (SPI7) Receive Next Counter Register */
  #define REG_SPI7_TNPR (*(__IO uint32_t*)0x40034518U) /**< \brief (SPI7) Transmit Next Pointer Register */
  #define REG_SPI7_TNCR (*(__IO uint32_t*)0x4003451CU) /**< \brief (SPI7) Transmit Next Counter Register */
  #define REG_SPI7_PTCR (*(__O  uint32_t*)0x40034520U) /**< \brief (SPI7) Transfer Control Register */
  #define REG_SPI7_PTSR (*(__I  uint32_t*)0x40034524U) /**< \brief (SPI7) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAMG55_SPI7_INSTANCE_ */
