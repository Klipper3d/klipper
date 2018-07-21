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

#ifndef _SAMG55_CRCCU_INSTANCE_
#define _SAMG55_CRCCU_INSTANCE_

/* ========== Register definition for CRCCU peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_CRCCU_DSCR                     (0x40048000U) /**< \brief (CRCCU) CRCCU Descriptor Base Register */
  #define REG_CRCCU_DMA_EN                   (0x40048008U) /**< \brief (CRCCU) CRCCU DMA Enable Register */
  #define REG_CRCCU_DMA_DIS                  (0x4004800CU) /**< \brief (CRCCU) CRCCU DMA Disable Register */
  #define REG_CRCCU_DMA_SR                   (0x40048010U) /**< \brief (CRCCU) CRCCU DMA Status Register */
  #define REG_CRCCU_DMA_IER                  (0x40048014U) /**< \brief (CRCCU) CRCCU DMA Interrupt Enable Register */
  #define REG_CRCCU_DMA_IDR                  (0x40048018U) /**< \brief (CRCCU) CRCCU DMA Interrupt Disable Register */
  #define REG_CRCCU_DMA_IMR                  (0x4004801CU) /**< \brief (CRCCU) CRCCU DMA Interrupt Mask Register */
  #define REG_CRCCU_DMA_ISR                  (0x40048020U) /**< \brief (CRCCU) CRCCU DMA Interrupt Status Register */
  #define REG_CRCCU_CR                       (0x40048034U) /**< \brief (CRCCU) CRCCU Control Register */
  #define REG_CRCCU_MR                       (0x40048038U) /**< \brief (CRCCU) CRCCU Mode Register */
  #define REG_CRCCU_SR                       (0x4004803CU) /**< \brief (CRCCU) CRCCU Status Register */
  #define REG_CRCCU_IER                      (0x40048040U) /**< \brief (CRCCU) CRCCU Interrupt Enable Register */
  #define REG_CRCCU_IDR                      (0x40048044U) /**< \brief (CRCCU) CRCCU Interrupt Disable Register */
  #define REG_CRCCU_IMR                      (0x40048048U) /**< \brief (CRCCU) CRCCU Interrupt Mask Register */
  #define REG_CRCCU_ISR                      (0x4004804CU) /**< \brief (CRCCU) CRCCU Interrupt Status Register */
#else
  #define REG_CRCCU_DSCR    (*(__IO uint32_t*)0x40048000U) /**< \brief (CRCCU) CRCCU Descriptor Base Register */
  #define REG_CRCCU_DMA_EN  (*(__O  uint32_t*)0x40048008U) /**< \brief (CRCCU) CRCCU DMA Enable Register */
  #define REG_CRCCU_DMA_DIS (*(__O  uint32_t*)0x4004800CU) /**< \brief (CRCCU) CRCCU DMA Disable Register */
  #define REG_CRCCU_DMA_SR  (*(__I  uint32_t*)0x40048010U) /**< \brief (CRCCU) CRCCU DMA Status Register */
  #define REG_CRCCU_DMA_IER (*(__O  uint32_t*)0x40048014U) /**< \brief (CRCCU) CRCCU DMA Interrupt Enable Register */
  #define REG_CRCCU_DMA_IDR (*(__O  uint32_t*)0x40048018U) /**< \brief (CRCCU) CRCCU DMA Interrupt Disable Register */
  #define REG_CRCCU_DMA_IMR (*(__I  uint32_t*)0x4004801CU) /**< \brief (CRCCU) CRCCU DMA Interrupt Mask Register */
  #define REG_CRCCU_DMA_ISR (*(__I  uint32_t*)0x40048020U) /**< \brief (CRCCU) CRCCU DMA Interrupt Status Register */
  #define REG_CRCCU_CR      (*(__O  uint32_t*)0x40048034U) /**< \brief (CRCCU) CRCCU Control Register */
  #define REG_CRCCU_MR      (*(__IO uint32_t*)0x40048038U) /**< \brief (CRCCU) CRCCU Mode Register */
  #define REG_CRCCU_SR      (*(__I  uint32_t*)0x4004803CU) /**< \brief (CRCCU) CRCCU Status Register */
  #define REG_CRCCU_IER     (*(__O  uint32_t*)0x40048040U) /**< \brief (CRCCU) CRCCU Interrupt Enable Register */
  #define REG_CRCCU_IDR     (*(__O  uint32_t*)0x40048044U) /**< \brief (CRCCU) CRCCU Interrupt Disable Register */
  #define REG_CRCCU_IMR     (*(__I  uint32_t*)0x40048048U) /**< \brief (CRCCU) CRCCU Interrupt Mask Register */
  #define REG_CRCCU_ISR     (*(__I  uint32_t*)0x4004804CU) /**< \brief (CRCCU) CRCCU Interrupt Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAMG55_CRCCU_INSTANCE_ */
