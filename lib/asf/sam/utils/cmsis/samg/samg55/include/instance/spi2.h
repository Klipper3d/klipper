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

#ifndef _SAMG55_SPI2_INSTANCE_
#define _SAMG55_SPI2_INSTANCE_

/* ========== Register definition for SPI2 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_SPI2_CR                    (0x40024400U) /**< \brief (SPI2) SPI Control Register */
  #define REG_SPI2_MR                    (0x40024404U) /**< \brief (SPI2) SPI Mode Register */
  #define REG_SPI2_RDR                   (0x40024408U) /**< \brief (SPI2) SPI Receive Data Register */
  #define REG_SPI2_TDR                   (0x4002440CU) /**< \brief (SPI2) SPI Transmit Data Register */
  #define REG_SPI2_SR                    (0x40024410U) /**< \brief (SPI2) SPI Status Register */
  #define REG_SPI2_IER                   (0x40024414U) /**< \brief (SPI2) SPI Interrupt Enable Register */
  #define REG_SPI2_IDR                   (0x40024418U) /**< \brief (SPI2) SPI Interrupt Disable Register */
  #define REG_SPI2_IMR                   (0x4002441CU) /**< \brief (SPI2) SPI Interrupt Mask Register */
  #define REG_SPI2_CSR                   (0x40024430U) /**< \brief (SPI2) SPI Chip Select Register */
  #define REG_SPI2_CMPR                  (0x40024448U) /**< \brief (SPI2) SPI Comparison Register */
  #define REG_SPI2_WPMR                  (0x400244E4U) /**< \brief (SPI2) SPI Write Protection Mode Register */
  #define REG_SPI2_WPSR                  (0x400244E8U) /**< \brief (SPI2) SPI Write Protection Status Register */
  #define REG_SPI2_RPR                   (0x40024500U) /**< \brief (SPI2) Receive Pointer Register */
  #define REG_SPI2_RCR                   (0x40024504U) /**< \brief (SPI2) Receive Counter Register */
  #define REG_SPI2_TPR                   (0x40024508U) /**< \brief (SPI2) Transmit Pointer Register */
  #define REG_SPI2_TCR                   (0x4002450CU) /**< \brief (SPI2) Transmit Counter Register */
  #define REG_SPI2_RNPR                  (0x40024510U) /**< \brief (SPI2) Receive Next Pointer Register */
  #define REG_SPI2_RNCR                  (0x40024514U) /**< \brief (SPI2) Receive Next Counter Register */
  #define REG_SPI2_TNPR                  (0x40024518U) /**< \brief (SPI2) Transmit Next Pointer Register */
  #define REG_SPI2_TNCR                  (0x4002451CU) /**< \brief (SPI2) Transmit Next Counter Register */
  #define REG_SPI2_PTCR                  (0x40024520U) /**< \brief (SPI2) Transfer Control Register */
  #define REG_SPI2_PTSR                  (0x40024524U) /**< \brief (SPI2) Transfer Status Register */
#else
  #define REG_SPI2_CR   (*(__O  uint32_t*)0x40024400U) /**< \brief (SPI2) SPI Control Register */
  #define REG_SPI2_MR   (*(__IO uint32_t*)0x40024404U) /**< \brief (SPI2) SPI Mode Register */
  #define REG_SPI2_RDR  (*(__I  uint32_t*)0x40024408U) /**< \brief (SPI2) SPI Receive Data Register */
  #define REG_SPI2_TDR  (*(__O  uint32_t*)0x4002440CU) /**< \brief (SPI2) SPI Transmit Data Register */
  #define REG_SPI2_SR   (*(__I  uint32_t*)0x40024410U) /**< \brief (SPI2) SPI Status Register */
  #define REG_SPI2_IER  (*(__O  uint32_t*)0x40024414U) /**< \brief (SPI2) SPI Interrupt Enable Register */
  #define REG_SPI2_IDR  (*(__O  uint32_t*)0x40024418U) /**< \brief (SPI2) SPI Interrupt Disable Register */
  #define REG_SPI2_IMR  (*(__I  uint32_t*)0x4002441CU) /**< \brief (SPI2) SPI Interrupt Mask Register */
  #define REG_SPI2_CSR  (*(__IO uint32_t*)0x40024430U) /**< \brief (SPI2) SPI Chip Select Register */
  #define REG_SPI2_CMPR (*(__IO uint32_t*)0x40024448U) /**< \brief (SPI2) SPI Comparison Register */
  #define REG_SPI2_WPMR (*(__IO uint32_t*)0x400244E4U) /**< \brief (SPI2) SPI Write Protection Mode Register */
  #define REG_SPI2_WPSR (*(__I  uint32_t*)0x400244E8U) /**< \brief (SPI2) SPI Write Protection Status Register */
  #define REG_SPI2_RPR  (*(__IO uint32_t*)0x40024500U) /**< \brief (SPI2) Receive Pointer Register */
  #define REG_SPI2_RCR  (*(__IO uint32_t*)0x40024504U) /**< \brief (SPI2) Receive Counter Register */
  #define REG_SPI2_TPR  (*(__IO uint32_t*)0x40024508U) /**< \brief (SPI2) Transmit Pointer Register */
  #define REG_SPI2_TCR  (*(__IO uint32_t*)0x4002450CU) /**< \brief (SPI2) Transmit Counter Register */
  #define REG_SPI2_RNPR (*(__IO uint32_t*)0x40024510U) /**< \brief (SPI2) Receive Next Pointer Register */
  #define REG_SPI2_RNCR (*(__IO uint32_t*)0x40024514U) /**< \brief (SPI2) Receive Next Counter Register */
  #define REG_SPI2_TNPR (*(__IO uint32_t*)0x40024518U) /**< \brief (SPI2) Transmit Next Pointer Register */
  #define REG_SPI2_TNCR (*(__IO uint32_t*)0x4002451CU) /**< \brief (SPI2) Transmit Next Counter Register */
  #define REG_SPI2_PTCR (*(__O  uint32_t*)0x40024520U) /**< \brief (SPI2) Transfer Control Register */
  #define REG_SPI2_PTSR (*(__I  uint32_t*)0x40024524U) /**< \brief (SPI2) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAMG55_SPI2_INSTANCE_ */
