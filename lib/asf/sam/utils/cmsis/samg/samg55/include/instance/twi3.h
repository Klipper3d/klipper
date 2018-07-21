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

#ifndef _SAMG55_TWI3_INSTANCE_
#define _SAMG55_TWI3_INSTANCE_

/* ========== Register definition for TWI3 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_TWI3_CR                     (0x40018600U) /**< \brief (TWI3) TWI Control Register */
  #define REG_TWI3_MMR                    (0x40018604U) /**< \brief (TWI3) TWI Master Mode Register */
  #define REG_TWI3_SMR                    (0x40018608U) /**< \brief (TWI3) TWI Slave Mode Register */
  #define REG_TWI3_IADR                   (0x4001860CU) /**< \brief (TWI3) TWI Internal Address Register */
  #define REG_TWI3_CWGR                   (0x40018610U) /**< \brief (TWI3) TWI Clock Waveform Generator Register */
  #define REG_TWI3_SR                     (0x40018620U) /**< \brief (TWI3) TWI Status Register */
  #define REG_TWI3_IER                    (0x40018624U) /**< \brief (TWI3) TWI Interrupt Enable Register */
  #define REG_TWI3_IDR                    (0x40018628U) /**< \brief (TWI3) TWI Interrupt Disable Register */
  #define REG_TWI3_IMR                    (0x4001862CU) /**< \brief (TWI3) TWI Interrupt Mask Register */
  #define REG_TWI3_RHR                    (0x40018630U) /**< \brief (TWI3) TWI Receive Holding Register */
  #define REG_TWI3_THR                    (0x40018634U) /**< \brief (TWI3) TWI Transmit Holding Register */
  #define REG_TWI3_SMBTR                  (0x40018638U) /**< \brief (TWI3) TWI SMBus Timing Register */
  #define REG_TWI3_ACR                    (0x40018640U) /**< \brief (TWI3) TWI Alternative Command Register */
  #define REG_TWI3_FILTR                  (0x40018644U) /**< \brief (TWI3) TWI Filter Register */
  #define REG_TWI3_SWMR                   (0x4001864CU) /**< \brief (TWI3) TWI SleepWalking Matching Register */
  #define REG_TWI3_WPMR                   (0x400186E4U) /**< \brief (TWI3) TWI Write Protection Mode Register */
  #define REG_TWI3_WPSR                   (0x400186E8U) /**< \brief (TWI3) TWI Write Protection Status Register */
  #define REG_TWI3_RPR                    (0x40018700U) /**< \brief (TWI3) Receive Pointer Register */
  #define REG_TWI3_RCR                    (0x40018704U) /**< \brief (TWI3) Receive Counter Register */
  #define REG_TWI3_TPR                    (0x40018708U) /**< \brief (TWI3) Transmit Pointer Register */
  #define REG_TWI3_TCR                    (0x4001870CU) /**< \brief (TWI3) Transmit Counter Register */
  #define REG_TWI3_RNPR                   (0x40018710U) /**< \brief (TWI3) Receive Next Pointer Register */
  #define REG_TWI3_RNCR                   (0x40018714U) /**< \brief (TWI3) Receive Next Counter Register */
  #define REG_TWI3_TNPR                   (0x40018718U) /**< \brief (TWI3) Transmit Next Pointer Register */
  #define REG_TWI3_TNCR                   (0x4001871CU) /**< \brief (TWI3) Transmit Next Counter Register */
  #define REG_TWI3_PTCR                   (0x40018720U) /**< \brief (TWI3) Transfer Control Register */
  #define REG_TWI3_PTSR                   (0x40018724U) /**< \brief (TWI3) Transfer Status Register */
#else
  #define REG_TWI3_CR    (*(__O  uint32_t*)0x40018600U) /**< \brief (TWI3) TWI Control Register */
  #define REG_TWI3_MMR   (*(__IO uint32_t*)0x40018604U) /**< \brief (TWI3) TWI Master Mode Register */
  #define REG_TWI3_SMR   (*(__IO uint32_t*)0x40018608U) /**< \brief (TWI3) TWI Slave Mode Register */
  #define REG_TWI3_IADR  (*(__IO uint32_t*)0x4001860CU) /**< \brief (TWI3) TWI Internal Address Register */
  #define REG_TWI3_CWGR  (*(__IO uint32_t*)0x40018610U) /**< \brief (TWI3) TWI Clock Waveform Generator Register */
  #define REG_TWI3_SR    (*(__I  uint32_t*)0x40018620U) /**< \brief (TWI3) TWI Status Register */
  #define REG_TWI3_IER   (*(__O  uint32_t*)0x40018624U) /**< \brief (TWI3) TWI Interrupt Enable Register */
  #define REG_TWI3_IDR   (*(__O  uint32_t*)0x40018628U) /**< \brief (TWI3) TWI Interrupt Disable Register */
  #define REG_TWI3_IMR   (*(__I  uint32_t*)0x4001862CU) /**< \brief (TWI3) TWI Interrupt Mask Register */
  #define REG_TWI3_RHR   (*(__I  uint32_t*)0x40018630U) /**< \brief (TWI3) TWI Receive Holding Register */
  #define REG_TWI3_THR   (*(__O  uint32_t*)0x40018634U) /**< \brief (TWI3) TWI Transmit Holding Register */
  #define REG_TWI3_SMBTR (*(__IO uint32_t*)0x40018638U) /**< \brief (TWI3) TWI SMBus Timing Register */
  #define REG_TWI3_ACR   (*(__IO uint32_t*)0x40018640U) /**< \brief (TWI3) TWI Alternative Command Register */
  #define REG_TWI3_FILTR (*(__IO uint32_t*)0x40018644U) /**< \brief (TWI3) TWI Filter Register */
  #define REG_TWI3_SWMR  (*(__IO uint32_t*)0x4001864CU) /**< \brief (TWI3) TWI SleepWalking Matching Register */
  #define REG_TWI3_WPMR  (*(__IO uint32_t*)0x400186E4U) /**< \brief (TWI3) TWI Write Protection Mode Register */
  #define REG_TWI3_WPSR  (*(__I  uint32_t*)0x400186E8U) /**< \brief (TWI3) TWI Write Protection Status Register */
  #define REG_TWI3_RPR   (*(__IO uint32_t*)0x40018700U) /**< \brief (TWI3) Receive Pointer Register */
  #define REG_TWI3_RCR   (*(__IO uint32_t*)0x40018704U) /**< \brief (TWI3) Receive Counter Register */
  #define REG_TWI3_TPR   (*(__IO uint32_t*)0x40018708U) /**< \brief (TWI3) Transmit Pointer Register */
  #define REG_TWI3_TCR   (*(__IO uint32_t*)0x4001870CU) /**< \brief (TWI3) Transmit Counter Register */
  #define REG_TWI3_RNPR  (*(__IO uint32_t*)0x40018710U) /**< \brief (TWI3) Receive Next Pointer Register */
  #define REG_TWI3_RNCR  (*(__IO uint32_t*)0x40018714U) /**< \brief (TWI3) Receive Next Counter Register */
  #define REG_TWI3_TNPR  (*(__IO uint32_t*)0x40018718U) /**< \brief (TWI3) Transmit Next Pointer Register */
  #define REG_TWI3_TNCR  (*(__IO uint32_t*)0x4001871CU) /**< \brief (TWI3) Transmit Next Counter Register */
  #define REG_TWI3_PTCR  (*(__O  uint32_t*)0x40018720U) /**< \brief (TWI3) Transfer Control Register */
  #define REG_TWI3_PTSR  (*(__I  uint32_t*)0x40018724U) /**< \brief (TWI3) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAMG55_TWI3_INSTANCE_ */
