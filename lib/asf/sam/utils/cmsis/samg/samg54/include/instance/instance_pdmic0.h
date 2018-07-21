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

#ifndef _SAMG54_PDMIC0_INSTANCE_
#define _SAMG54_PDMIC0_INSTANCE_

/* ========== Register definition for PDMIC0 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_PDMIC0_CR                     (0x4002C000U) /**< \brief (PDMIC0) Control Register */
  #define REG_PDMIC0_MR                     (0x4002C004U) /**< \brief (PDMIC0) Mode Register */
  #define REG_PDMIC0_CDR                    (0x4002C014U) /**< \brief (PDMIC0) Converted Data Register */
  #define REG_PDMIC0_IER                    (0x4002C018U) /**< \brief (PDMIC0) Interrupt Enable Register */
  #define REG_PDMIC0_IDR                    (0x4002C01CU) /**< \brief (PDMIC0) Interrupt Disable Register */
  #define REG_PDMIC0_IMR                    (0x4002C020U) /**< \brief (PDMIC0) Interrupt Mask Register */
  #define REG_PDMIC0_ISR                    (0x4002C024U) /**< \brief (PDMIC0) Interrupt Status Register */
  #define REG_PDMIC0_DSPR0                  (0x4002C058U) /**< \brief (PDMIC0) DSP Configuration Register 0 */
  #define REG_PDMIC0_DSPR1                  (0x4002C05CU) /**< \brief (PDMIC0) DSP Configuration Register 1 */
  #define REG_PDMIC0_WPMR                   (0x4002C0E4U) /**< \brief (PDMIC0) Write Protection Mode Register */
  #define REG_PDMIC0_WPSR                   (0x4002C0E8U) /**< \brief (PDMIC0) Write Protection Status Register */
  #define REG_PDMIC0_RPR                    (0x4002C100U) /**< \brief (PDMIC0) Receive Pointer Register */
  #define REG_PDMIC0_RCR                    (0x4002C104U) /**< \brief (PDMIC0) Receive Counter Register */
  #define REG_PDMIC0_RNPR                   (0x4002C110U) /**< \brief (PDMIC0) Receive Next Pointer Register */
  #define REG_PDMIC0_RNCR                   (0x4002C114U) /**< \brief (PDMIC0) Receive Next Counter Register */
  #define REG_PDMIC0_PTCR                   (0x4002C120U) /**< \brief (PDMIC0) Transfer Control Register */
  #define REG_PDMIC0_PTSR                   (0x4002C124U) /**< \brief (PDMIC0) Transfer Status Register */
#else
  #define REG_PDMIC0_CR    (*(__IO uint32_t*)0x4002C000U) /**< \brief (PDMIC0) Control Register */
  #define REG_PDMIC0_MR    (*(__IO uint32_t*)0x4002C004U) /**< \brief (PDMIC0) Mode Register */
  #define REG_PDMIC0_CDR   (*(__I  uint32_t*)0x4002C014U) /**< \brief (PDMIC0) Converted Data Register */
  #define REG_PDMIC0_IER   (*(__O  uint32_t*)0x4002C018U) /**< \brief (PDMIC0) Interrupt Enable Register */
  #define REG_PDMIC0_IDR   (*(__O  uint32_t*)0x4002C01CU) /**< \brief (PDMIC0) Interrupt Disable Register */
  #define REG_PDMIC0_IMR   (*(__I  uint32_t*)0x4002C020U) /**< \brief (PDMIC0) Interrupt Mask Register */
  #define REG_PDMIC0_ISR   (*(__I  uint32_t*)0x4002C024U) /**< \brief (PDMIC0) Interrupt Status Register */
  #define REG_PDMIC0_DSPR0 (*(__IO uint32_t*)0x4002C058U) /**< \brief (PDMIC0) DSP Configuration Register 0 */
  #define REG_PDMIC0_DSPR1 (*(__IO uint32_t*)0x4002C05CU) /**< \brief (PDMIC0) DSP Configuration Register 1 */
  #define REG_PDMIC0_WPMR  (*(__IO uint32_t*)0x4002C0E4U) /**< \brief (PDMIC0) Write Protection Mode Register */
  #define REG_PDMIC0_WPSR  (*(__I  uint32_t*)0x4002C0E8U) /**< \brief (PDMIC0) Write Protection Status Register */
  #define REG_PDMIC0_RPR   (*(__IO uint32_t*)0x4002C100U) /**< \brief (PDMIC0) Receive Pointer Register */
  #define REG_PDMIC0_RCR   (*(__IO uint32_t*)0x4002C104U) /**< \brief (PDMIC0) Receive Counter Register */
  #define REG_PDMIC0_RNPR  (*(__IO uint32_t*)0x4002C110U) /**< \brief (PDMIC0) Receive Next Pointer Register */
  #define REG_PDMIC0_RNCR  (*(__IO uint32_t*)0x4002C114U) /**< \brief (PDMIC0) Receive Next Counter Register */
  #define REG_PDMIC0_PTCR  (*(__O  uint32_t*)0x4002C120U) /**< \brief (PDMIC0) Transfer Control Register */
  #define REG_PDMIC0_PTSR  (*(__I  uint32_t*)0x4002C124U) /**< \brief (PDMIC0) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAMG54_PDMIC0_INSTANCE_ */
