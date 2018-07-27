/**
 * \file
 *
 * Copyright (c) 2013-2018 Microchip Technology Inc. and its subsidiaries.
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

#ifndef _SAMG53_USART_INSTANCE_
#define _SAMG53_USART_INSTANCE_

/* ========== Register definition for USART peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_USART_CR                    (0x40024000U) /**< \brief (USART) Control Register */
  #define REG_USART_MR                    (0x40024004U) /**< \brief (USART) Mode Register */
  #define REG_USART_IER                   (0x40024008U) /**< \brief (USART) Interrupt Enable Register */
  #define REG_USART_IDR                   (0x4002400CU) /**< \brief (USART) Interrupt Disable Register */
  #define REG_USART_IMR                   (0x40024010U) /**< \brief (USART) Interrupt Mask Register */
  #define REG_USART_CSR                   (0x40024014U) /**< \brief (USART) Channel Status Register */
  #define REG_USART_RHR                   (0x40024018U) /**< \brief (USART) Receive Holding Register */
  #define REG_USART_THR                   (0x4002401CU) /**< \brief (USART) Transmit Holding Register */
  #define REG_USART_BRGR                  (0x40024020U) /**< \brief (USART) Baud Rate Generator Register */
  #define REG_USART_RTOR                  (0x40024024U) /**< \brief (USART) Receiver Time-out Register */
  #define REG_USART_TTGR                  (0x40024028U) /**< \brief (USART) Transmitter Timeguard Register */
  #define REG_USART_FIDI                  (0x40024040U) /**< \brief (USART) FI DI Ratio Register */
  #define REG_USART_NER                   (0x40024044U) /**< \brief (USART) Number of Errors Register */
  #define REG_USART_IF                    (0x4002404CU) /**< \brief (USART) IrDA Filter Register */
  #define REG_USART_WPMR                  (0x400240E4U) /**< \brief (USART) Write Protection Mode Register */
  #define REG_USART_WPSR                  (0x400240E8U) /**< \brief (USART) Write Protection Status Register */
  #define REG_USART_RPR                   (0x40024100U) /**< \brief (USART) Receive Pointer Register */
  #define REG_USART_RCR                   (0x40024104U) /**< \brief (USART) Receive Counter Register */
  #define REG_USART_TPR                   (0x40024108U) /**< \brief (USART) Transmit Pointer Register */
  #define REG_USART_TCR                   (0x4002410CU) /**< \brief (USART) Transmit Counter Register */
  #define REG_USART_RNPR                  (0x40024110U) /**< \brief (USART) Receive Next Pointer Register */
  #define REG_USART_RNCR                  (0x40024114U) /**< \brief (USART) Receive Next Counter Register */
  #define REG_USART_TNPR                  (0x40024118U) /**< \brief (USART) Transmit Next Pointer Register */
  #define REG_USART_TNCR                  (0x4002411CU) /**< \brief (USART) Transmit Next Counter Register */
  #define REG_USART_PTCR                  (0x40024120U) /**< \brief (USART) Transfer Control Register */
  #define REG_USART_PTSR                  (0x40024124U) /**< \brief (USART) Transfer Status Register */
#else
  #define REG_USART_CR   (*(__O  uint32_t*)0x40024000U) /**< \brief (USART) Control Register */
  #define REG_USART_MR   (*(__IO uint32_t*)0x40024004U) /**< \brief (USART) Mode Register */
  #define REG_USART_IER  (*(__O  uint32_t*)0x40024008U) /**< \brief (USART) Interrupt Enable Register */
  #define REG_USART_IDR  (*(__O  uint32_t*)0x4002400CU) /**< \brief (USART) Interrupt Disable Register */
  #define REG_USART_IMR  (*(__I  uint32_t*)0x40024010U) /**< \brief (USART) Interrupt Mask Register */
  #define REG_USART_CSR  (*(__I  uint32_t*)0x40024014U) /**< \brief (USART) Channel Status Register */
  #define REG_USART_RHR  (*(__I  uint32_t*)0x40024018U) /**< \brief (USART) Receive Holding Register */
  #define REG_USART_THR  (*(__O  uint32_t*)0x4002401CU) /**< \brief (USART) Transmit Holding Register */
  #define REG_USART_BRGR (*(__IO uint32_t*)0x40024020U) /**< \brief (USART) Baud Rate Generator Register */
  #define REG_USART_RTOR (*(__IO uint32_t*)0x40024024U) /**< \brief (USART) Receiver Time-out Register */
  #define REG_USART_TTGR (*(__IO uint32_t*)0x40024028U) /**< \brief (USART) Transmitter Timeguard Register */
  #define REG_USART_FIDI (*(__IO uint32_t*)0x40024040U) /**< \brief (USART) FI DI Ratio Register */
  #define REG_USART_NER  (*(__I  uint32_t*)0x40024044U) /**< \brief (USART) Number of Errors Register */
  #define REG_USART_IF   (*(__IO uint32_t*)0x4002404CU) /**< \brief (USART) IrDA Filter Register */
  #define REG_USART_WPMR (*(__IO uint32_t*)0x400240E4U) /**< \brief (USART) Write Protection Mode Register */
  #define REG_USART_WPSR (*(__I  uint32_t*)0x400240E8U) /**< \brief (USART) Write Protection Status Register */
  #define REG_USART_RPR  (*(__IO uint32_t*)0x40024100U) /**< \brief (USART) Receive Pointer Register */
  #define REG_USART_RCR  (*(__IO uint32_t*)0x40024104U) /**< \brief (USART) Receive Counter Register */
  #define REG_USART_TPR  (*(__IO uint32_t*)0x40024108U) /**< \brief (USART) Transmit Pointer Register */
  #define REG_USART_TCR  (*(__IO uint32_t*)0x4002410CU) /**< \brief (USART) Transmit Counter Register */
  #define REG_USART_RNPR (*(__IO uint32_t*)0x40024110U) /**< \brief (USART) Receive Next Pointer Register */
  #define REG_USART_RNCR (*(__IO uint32_t*)0x40024114U) /**< \brief (USART) Receive Next Counter Register */
  #define REG_USART_TNPR (*(__IO uint32_t*)0x40024118U) /**< \brief (USART) Transmit Next Pointer Register */
  #define REG_USART_TNCR (*(__IO uint32_t*)0x4002411CU) /**< \brief (USART) Transmit Next Counter Register */
  #define REG_USART_PTCR (*(__O  uint32_t*)0x40024120U) /**< \brief (USART) Transfer Control Register */
  #define REG_USART_PTSR (*(__I  uint32_t*)0x40024124U) /**< \brief (USART) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAMG53_USART_INSTANCE_ */
