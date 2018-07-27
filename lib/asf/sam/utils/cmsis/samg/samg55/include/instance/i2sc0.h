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

#ifndef _SAMG55_I2SC0_INSTANCE_
#define _SAMG55_I2SC0_INSTANCE_

/* ========== Register definition for I2SC0 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_I2SC0_CR                    (0x40000000U) /**< \brief (I2SC0) Control Register */
  #define REG_I2SC0_MR                    (0x40000004U) /**< \brief (I2SC0) Mode Register */
  #define REG_I2SC0_SR                    (0x40000008U) /**< \brief (I2SC0) Status Register */
  #define REG_I2SC0_SCR                   (0x4000000CU) /**< \brief (I2SC0) Status Clear Register */
  #define REG_I2SC0_SSR                   (0x40000010U) /**< \brief (I2SC0) Status Set Register */
  #define REG_I2SC0_IER                   (0x40000014U) /**< \brief (I2SC0) Interrupt Enable Register */
  #define REG_I2SC0_IDR                   (0x40000018U) /**< \brief (I2SC0) Interrupt Disable Register */
  #define REG_I2SC0_IMR                   (0x4000001CU) /**< \brief (I2SC0) Interrupt Mask Register */
  #define REG_I2SC0_RHR                   (0x40000020U) /**< \brief (I2SC0) Receiver Holding Register */
  #define REG_I2SC0_THR                   (0x40000024U) /**< \brief (I2SC0) Transmitter Holding Register */
  #define REG_I2SC0_RPR                   (0x40000100U) /**< \brief (I2SC0) Receive Pointer Register */
  #define REG_I2SC0_RCR                   (0x40000104U) /**< \brief (I2SC0) Receive Counter Register */
  #define REG_I2SC0_TPR                   (0x40000108U) /**< \brief (I2SC0) Transmit Pointer Register */
  #define REG_I2SC0_TCR                   (0x4000010CU) /**< \brief (I2SC0) Transmit Counter Register */
  #define REG_I2SC0_RNPR                  (0x40000110U) /**< \brief (I2SC0) Receive Next Pointer Register */
  #define REG_I2SC0_RNCR                  (0x40000114U) /**< \brief (I2SC0) Receive Next Counter Register */
  #define REG_I2SC0_TNPR                  (0x40000118U) /**< \brief (I2SC0) Transmit Next Pointer Register */
  #define REG_I2SC0_TNCR                  (0x4000011CU) /**< \brief (I2SC0) Transmit Next Counter Register */
  #define REG_I2SC0_PTCR                  (0x40000120U) /**< \brief (I2SC0) Transfer Control Register */
  #define REG_I2SC0_PTSR                  (0x40000124U) /**< \brief (I2SC0) Transfer Status Register */
#else
  #define REG_I2SC0_CR   (*(__O  uint32_t*)0x40000000U) /**< \brief (I2SC0) Control Register */
  #define REG_I2SC0_MR   (*(__IO uint32_t*)0x40000004U) /**< \brief (I2SC0) Mode Register */
  #define REG_I2SC0_SR   (*(__I  uint32_t*)0x40000008U) /**< \brief (I2SC0) Status Register */
  #define REG_I2SC0_SCR  (*(__O  uint32_t*)0x4000000CU) /**< \brief (I2SC0) Status Clear Register */
  #define REG_I2SC0_SSR  (*(__O  uint32_t*)0x40000010U) /**< \brief (I2SC0) Status Set Register */
  #define REG_I2SC0_IER  (*(__O  uint32_t*)0x40000014U) /**< \brief (I2SC0) Interrupt Enable Register */
  #define REG_I2SC0_IDR  (*(__O  uint32_t*)0x40000018U) /**< \brief (I2SC0) Interrupt Disable Register */
  #define REG_I2SC0_IMR  (*(__I  uint32_t*)0x4000001CU) /**< \brief (I2SC0) Interrupt Mask Register */
  #define REG_I2SC0_RHR  (*(__I  uint32_t*)0x40000020U) /**< \brief (I2SC0) Receiver Holding Register */
  #define REG_I2SC0_THR  (*(__O  uint32_t*)0x40000024U) /**< \brief (I2SC0) Transmitter Holding Register */
  #define REG_I2SC0_RPR  (*(__IO uint32_t*)0x40000100U) /**< \brief (I2SC0) Receive Pointer Register */
  #define REG_I2SC0_RCR  (*(__IO uint32_t*)0x40000104U) /**< \brief (I2SC0) Receive Counter Register */
  #define REG_I2SC0_TPR  (*(__IO uint32_t*)0x40000108U) /**< \brief (I2SC0) Transmit Pointer Register */
  #define REG_I2SC0_TCR  (*(__IO uint32_t*)0x4000010CU) /**< \brief (I2SC0) Transmit Counter Register */
  #define REG_I2SC0_RNPR (*(__IO uint32_t*)0x40000110U) /**< \brief (I2SC0) Receive Next Pointer Register */
  #define REG_I2SC0_RNCR (*(__IO uint32_t*)0x40000114U) /**< \brief (I2SC0) Receive Next Counter Register */
  #define REG_I2SC0_TNPR (*(__IO uint32_t*)0x40000118U) /**< \brief (I2SC0) Transmit Next Pointer Register */
  #define REG_I2SC0_TNCR (*(__IO uint32_t*)0x4000011CU) /**< \brief (I2SC0) Transmit Next Counter Register */
  #define REG_I2SC0_PTCR (*(__O  uint32_t*)0x40000120U) /**< \brief (I2SC0) Transfer Control Register */
  #define REG_I2SC0_PTSR (*(__I  uint32_t*)0x40000124U) /**< \brief (I2SC0) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAMG55_I2SC0_INSTANCE_ */
