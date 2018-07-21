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

#ifndef _SAMG55_MEM2MEM_INSTANCE_
#define _SAMG55_MEM2MEM_INSTANCE_

/* ========== Register definition for MEM2MEM peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_MEM2MEM_THR                   (0x40028000U) /**< \brief (MEM2MEM) Memory to Memory Transfer Holding Register */
  #define REG_MEM2MEM_MR                    (0x40028004U) /**< \brief (MEM2MEM) Memory to Memory Mode Register */
  #define REG_MEM2MEM_IER                   (0x40028008U) /**< \brief (MEM2MEM) Memory to Memory Interrupt Enable Register */
  #define REG_MEM2MEM_IDR                   (0x4002800CU) /**< \brief (MEM2MEM) Memory to Memory Interrupt Disable Register */
  #define REG_MEM2MEM_IMR                   (0x40028010U) /**< \brief (MEM2MEM) Memory to Memory Interrupt Mask Register */
  #define REG_MEM2MEM_ISR                   (0x40028014U) /**< \brief (MEM2MEM) Memory to Memory Interrupt Status Register */
  #define REG_MEM2MEM_RPR                   (0x40028100U) /**< \brief (MEM2MEM) Receive Pointer Register */
  #define REG_MEM2MEM_RCR                   (0x40028104U) /**< \brief (MEM2MEM) Receive Counter Register */
  #define REG_MEM2MEM_TPR                   (0x40028108U) /**< \brief (MEM2MEM) Transmit Pointer Register */
  #define REG_MEM2MEM_TCR                   (0x4002810CU) /**< \brief (MEM2MEM) Transmit Counter Register */
  #define REG_MEM2MEM_RNPR                  (0x40028110U) /**< \brief (MEM2MEM) Receive Next Pointer Register */
  #define REG_MEM2MEM_RNCR                  (0x40028114U) /**< \brief (MEM2MEM) Receive Next Counter Register */
  #define REG_MEM2MEM_TNPR                  (0x40028118U) /**< \brief (MEM2MEM) Transmit Next Pointer Register */
  #define REG_MEM2MEM_TNCR                  (0x4002811CU) /**< \brief (MEM2MEM) Transmit Next Counter Register */
  #define REG_MEM2MEM_PTCR                  (0x40028120U) /**< \brief (MEM2MEM) Transfer Control Register */
  #define REG_MEM2MEM_PTSR                  (0x40028124U) /**< \brief (MEM2MEM) Transfer Status Register */
#else
  #define REG_MEM2MEM_THR  (*(__IO uint32_t*)0x40028000U) /**< \brief (MEM2MEM) Memory to Memory Transfer Holding Register */
  #define REG_MEM2MEM_MR   (*(__IO uint32_t*)0x40028004U) /**< \brief (MEM2MEM) Memory to Memory Mode Register */
  #define REG_MEM2MEM_IER  (*(__O  uint32_t*)0x40028008U) /**< \brief (MEM2MEM) Memory to Memory Interrupt Enable Register */
  #define REG_MEM2MEM_IDR  (*(__O  uint32_t*)0x4002800CU) /**< \brief (MEM2MEM) Memory to Memory Interrupt Disable Register */
  #define REG_MEM2MEM_IMR  (*(__I  uint32_t*)0x40028010U) /**< \brief (MEM2MEM) Memory to Memory Interrupt Mask Register */
  #define REG_MEM2MEM_ISR  (*(__I  uint32_t*)0x40028014U) /**< \brief (MEM2MEM) Memory to Memory Interrupt Status Register */
  #define REG_MEM2MEM_RPR  (*(__IO uint32_t*)0x40028100U) /**< \brief (MEM2MEM) Receive Pointer Register */
  #define REG_MEM2MEM_RCR  (*(__IO uint32_t*)0x40028104U) /**< \brief (MEM2MEM) Receive Counter Register */
  #define REG_MEM2MEM_TPR  (*(__IO uint32_t*)0x40028108U) /**< \brief (MEM2MEM) Transmit Pointer Register */
  #define REG_MEM2MEM_TCR  (*(__IO uint32_t*)0x4002810CU) /**< \brief (MEM2MEM) Transmit Counter Register */
  #define REG_MEM2MEM_RNPR (*(__IO uint32_t*)0x40028110U) /**< \brief (MEM2MEM) Receive Next Pointer Register */
  #define REG_MEM2MEM_RNCR (*(__IO uint32_t*)0x40028114U) /**< \brief (MEM2MEM) Receive Next Counter Register */
  #define REG_MEM2MEM_TNPR (*(__IO uint32_t*)0x40028118U) /**< \brief (MEM2MEM) Transmit Next Pointer Register */
  #define REG_MEM2MEM_TNCR (*(__IO uint32_t*)0x4002811CU) /**< \brief (MEM2MEM) Transmit Next Counter Register */
  #define REG_MEM2MEM_PTCR (*(__O  uint32_t*)0x40028120U) /**< \brief (MEM2MEM) Transfer Control Register */
  #define REG_MEM2MEM_PTSR (*(__I  uint32_t*)0x40028124U) /**< \brief (MEM2MEM) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAMG55_MEM2MEM_INSTANCE_ */
