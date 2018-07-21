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
#ifndef _SAM4C_USART3_INSTANCE_
#define _SAM4C_USART3_INSTANCE_

/* ========== Register definition for USART3 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_USART3_CR                    (0x40030000U) /**< \brief (USART3) Control Register */
  #define REG_USART3_MR                    (0x40030004U) /**< \brief (USART3) Mode Register */
  #define REG_USART3_IER                   (0x40030008U) /**< \brief (USART3) Interrupt Enable Register */
  #define REG_USART3_IDR                   (0x4003000CU) /**< \brief (USART3) Interrupt Disable Register */
  #define REG_USART3_IMR                   (0x40030010U) /**< \brief (USART3) Interrupt Mask Register */
  #define REG_USART3_CSR                   (0x40030014U) /**< \brief (USART3) Channel Status Register */
  #define REG_USART3_RHR                   (0x40030018U) /**< \brief (USART3) Receiver Holding Register */
  #define REG_USART3_THR                   (0x4003001CU) /**< \brief (USART3) Transmitter Holding Register */
  #define REG_USART3_BRGR                  (0x40030020U) /**< \brief (USART3) Baud Rate Generator Register */
  #define REG_USART3_RTOR                  (0x40030024U) /**< \brief (USART3) Receiver Time-out Register */
  #define REG_USART3_TTGR                  (0x40030028U) /**< \brief (USART3) Transmitter Timeguard Register */
  #define REG_USART3_FIDI                  (0x40030040U) /**< \brief (USART3) FI DI Ratio Register */
  #define REG_USART3_NER                   (0x40030044U) /**< \brief (USART3) Number of Errors Register */
  #define REG_USART3_IF                    (0x4003004CU) /**< \brief (USART3) IrDA Filter Register */
  #define REG_USART3_MAN                   (0x40030050U) /**< \brief (USART3) Manchester Encoder Decoder Register */
  #define REG_USART3_WPMR                  (0x400300E4U) /**< \brief (USART3) Write Protect Mode Register */
  #define REG_USART3_WPSR                  (0x400300E8U) /**< \brief (USART3) Write Protect Status Register */
  #define REG_USART3_RPR                   (0x40030100U) /**< \brief (USART3) Receive Pointer Register */
  #define REG_USART3_RCR                   (0x40030104U) /**< \brief (USART3) Receive Counter Register */
  #define REG_USART3_TPR                   (0x40030108U) /**< \brief (USART3) Transmit Pointer Register */
  #define REG_USART3_TCR                   (0x4003010CU) /**< \brief (USART3) Transmit Counter Register */
  #define REG_USART3_RNPR                  (0x40030110U) /**< \brief (USART3) Receive Next Pointer Register */
  #define REG_USART3_RNCR                  (0x40030114U) /**< \brief (USART3) Receive Next Counter Register */
  #define REG_USART3_TNPR                  (0x40030118U) /**< \brief (USART3) Transmit Next Pointer Register */
  #define REG_USART3_TNCR                  (0x4003011CU) /**< \brief (USART3) Transmit Next Counter Register */
  #define REG_USART3_PTCR                  (0x40030120U) /**< \brief (USART3) Transfer Control Register */
  #define REG_USART3_PTSR                  (0x40030124U) /**< \brief (USART3) Transfer Status Register */
#else
  #define REG_USART3_CR   (*(__O  uint32_t*)0x40030000U) /**< \brief (USART3) Control Register */
  #define REG_USART3_MR   (*(__IO uint32_t*)0x40030004U) /**< \brief (USART3) Mode Register */
  #define REG_USART3_IER  (*(__O  uint32_t*)0x40030008U) /**< \brief (USART3) Interrupt Enable Register */
  #define REG_USART3_IDR  (*(__O  uint32_t*)0x4003000CU) /**< \brief (USART3) Interrupt Disable Register */
  #define REG_USART3_IMR  (*(__I  uint32_t*)0x40030010U) /**< \brief (USART3) Interrupt Mask Register */
  #define REG_USART3_CSR  (*(__I  uint32_t*)0x40030014U) /**< \brief (USART3) Channel Status Register */
  #define REG_USART3_RHR  (*(__I  uint32_t*)0x40030018U) /**< \brief (USART3) Receiver Holding Register */
  #define REG_USART3_THR  (*(__O  uint32_t*)0x4003001CU) /**< \brief (USART3) Transmitter Holding Register */
  #define REG_USART3_BRGR (*(__IO uint32_t*)0x40030020U) /**< \brief (USART3) Baud Rate Generator Register */
  #define REG_USART3_RTOR (*(__IO uint32_t*)0x40030024U) /**< \brief (USART3) Receiver Time-out Register */
  #define REG_USART3_TTGR (*(__IO uint32_t*)0x40030028U) /**< \brief (USART3) Transmitter Timeguard Register */
  #define REG_USART3_FIDI (*(__IO uint32_t*)0x40030040U) /**< \brief (USART3) FI DI Ratio Register */
  #define REG_USART3_NER  (*(__I  uint32_t*)0x40030044U) /**< \brief (USART3) Number of Errors Register */
  #define REG_USART3_IF   (*(__IO uint32_t*)0x4003004CU) /**< \brief (USART3) IrDA Filter Register */
  #define REG_USART3_MAN  (*(__IO uint32_t*)0x40030050U) /**< \brief (USART3) Manchester Encoder Decoder Register */
  #define REG_USART3_WPMR (*(__IO uint32_t*)0x400300E4U) /**< \brief (USART3) Write Protect Mode Register */
  #define REG_USART3_WPSR (*(__I  uint32_t*)0x400300E8U) /**< \brief (USART3) Write Protect Status Register */
  #define REG_USART3_RPR  (*(__IO uint32_t*)0x40030100U) /**< \brief (USART3) Receive Pointer Register */
  #define REG_USART3_RCR  (*(__IO uint32_t*)0x40030104U) /**< \brief (USART3) Receive Counter Register */
  #define REG_USART3_TPR  (*(__IO uint32_t*)0x40030108U) /**< \brief (USART3) Transmit Pointer Register */
  #define REG_USART3_TCR  (*(__IO uint32_t*)0x4003010CU) /**< \brief (USART3) Transmit Counter Register */
  #define REG_USART3_RNPR (*(__IO uint32_t*)0x40030110U) /**< \brief (USART3) Receive Next Pointer Register */
  #define REG_USART3_RNCR (*(__IO uint32_t*)0x40030114U) /**< \brief (USART3) Receive Next Counter Register */
  #define REG_USART3_TNPR (*(__IO uint32_t*)0x40030118U) /**< \brief (USART3) Transmit Next Pointer Register */
  #define REG_USART3_TNCR (*(__IO uint32_t*)0x4003011CU) /**< \brief (USART3) Transmit Next Counter Register */
  #define REG_USART3_PTCR (*(__O  uint32_t*)0x40030120U) /**< \brief (USART3) Transfer Control Register */
  #define REG_USART3_PTSR (*(__I  uint32_t*)0x40030124U) /**< \brief (USART3) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAM4C_USART3_INSTANCE_ */
