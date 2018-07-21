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

#ifndef _SAMG55_PDMIC1_INSTANCE_
#define _SAMG55_PDMIC1_INSTANCE_

/* ========== Register definition for PDMIC1 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_PDMIC1_CR                     (0x40030000U) /**< \brief (PDMIC1) Control Register */
  #define REG_PDMIC1_MR                     (0x40030004U) /**< \brief (PDMIC1) Mode Register */
  #define REG_PDMIC1_CDR                    (0x40030014U) /**< \brief (PDMIC1) Converted Data Register */
  #define REG_PDMIC1_IER                    (0x40030018U) /**< \brief (PDMIC1) Interrupt Enable Register */
  #define REG_PDMIC1_IDR                    (0x4003001CU) /**< \brief (PDMIC1) Interrupt Disable Register */
  #define REG_PDMIC1_IMR                    (0x40030020U) /**< \brief (PDMIC1) Interrupt Mask Register */
  #define REG_PDMIC1_ISR                    (0x40030024U) /**< \brief (PDMIC1) Interrupt Status Register */
  #define REG_PDMIC1_DSPR0                  (0x40030058U) /**< \brief (PDMIC1) DSP Configuration Register 0 */
  #define REG_PDMIC1_DSPR1                  (0x4003005CU) /**< \brief (PDMIC1) DSP Configuration Register 1 */
  #define REG_PDMIC1_WPMR                   (0x400300E4U) /**< \brief (PDMIC1) Write Protection Mode Register */
  #define REG_PDMIC1_WPSR                   (0x400300E8U) /**< \brief (PDMIC1) Write Protection Status Register */
  #define REG_PDMIC1_RPR                    (0x40030100U) /**< \brief (PDMIC1) Receive Pointer Register */
  #define REG_PDMIC1_RCR                    (0x40030104U) /**< \brief (PDMIC1) Receive Counter Register */
  #define REG_PDMIC1_RNPR                   (0x40030110U) /**< \brief (PDMIC1) Receive Next Pointer Register */
  #define REG_PDMIC1_RNCR                   (0x40030114U) /**< \brief (PDMIC1) Receive Next Counter Register */
  #define REG_PDMIC1_PTCR                   (0x40030120U) /**< \brief (PDMIC1) Transfer Control Register */
  #define REG_PDMIC1_PTSR                   (0x40030124U) /**< \brief (PDMIC1) Transfer Status Register */
#else
  #define REG_PDMIC1_CR    (*(__IO uint32_t*)0x40030000U) /**< \brief (PDMIC1) Control Register */
  #define REG_PDMIC1_MR    (*(__IO uint32_t*)0x40030004U) /**< \brief (PDMIC1) Mode Register */
  #define REG_PDMIC1_CDR   (*(__I  uint32_t*)0x40030014U) /**< \brief (PDMIC1) Converted Data Register */
  #define REG_PDMIC1_IER   (*(__O  uint32_t*)0x40030018U) /**< \brief (PDMIC1) Interrupt Enable Register */
  #define REG_PDMIC1_IDR   (*(__O  uint32_t*)0x4003001CU) /**< \brief (PDMIC1) Interrupt Disable Register */
  #define REG_PDMIC1_IMR   (*(__I  uint32_t*)0x40030020U) /**< \brief (PDMIC1) Interrupt Mask Register */
  #define REG_PDMIC1_ISR   (*(__I  uint32_t*)0x40030024U) /**< \brief (PDMIC1) Interrupt Status Register */
  #define REG_PDMIC1_DSPR0 (*(__IO uint32_t*)0x40030058U) /**< \brief (PDMIC1) DSP Configuration Register 0 */
  #define REG_PDMIC1_DSPR1 (*(__IO uint32_t*)0x4003005CU) /**< \brief (PDMIC1) DSP Configuration Register 1 */
  #define REG_PDMIC1_WPMR  (*(__IO uint32_t*)0x400300E4U) /**< \brief (PDMIC1) Write Protection Mode Register */
  #define REG_PDMIC1_WPSR  (*(__I  uint32_t*)0x400300E8U) /**< \brief (PDMIC1) Write Protection Status Register */
  #define REG_PDMIC1_RPR   (*(__IO uint32_t*)0x40030100U) /**< \brief (PDMIC1) Receive Pointer Register */
  #define REG_PDMIC1_RCR   (*(__IO uint32_t*)0x40030104U) /**< \brief (PDMIC1) Receive Counter Register */
  #define REG_PDMIC1_RNPR  (*(__IO uint32_t*)0x40030110U) /**< \brief (PDMIC1) Receive Next Pointer Register */
  #define REG_PDMIC1_RNCR  (*(__IO uint32_t*)0x40030114U) /**< \brief (PDMIC1) Receive Next Counter Register */
  #define REG_PDMIC1_PTCR  (*(__O  uint32_t*)0x40030120U) /**< \brief (PDMIC1) Transfer Control Register */
  #define REG_PDMIC1_PTSR  (*(__I  uint32_t*)0x40030124U) /**< \brief (PDMIC1) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAMG55_PDMIC1_INSTANCE_ */
