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

#ifndef _SAMG55_USART1_INSTANCE_
#define _SAMG55_USART1_INSTANCE_

/* ========== Register definition for USART1 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_USART1_CR                      (0x40020200U) /**< \brief (USART1) USART Control Register */
  #define REG_USART1_MR                      (0x40020204U) /**< \brief (USART1) USART Mode Register */
  #define REG_USART1_IER                     (0x40020208U) /**< \brief (USART1) USART Interrupt Enable Register */
  #define REG_USART1_IDR                     (0x4002020CU) /**< \brief (USART1) USART Interrupt Disable Register */
  #define REG_USART1_IMR                     (0x40020210U) /**< \brief (USART1) USART Interrupt Mask Register */
  #define REG_USART1_CSR                     (0x40020214U) /**< \brief (USART1) USART Channel Status Register */
  #define REG_USART1_RHR                     (0x40020218U) /**< \brief (USART1) USART Receive Holding Register */
  #define REG_USART1_THR                     (0x4002021CU) /**< \brief (USART1) USART Transmit Holding Register */
  #define REG_USART1_BRGR                    (0x40020220U) /**< \brief (USART1) USART Baud Rate Generator Register */
  #define REG_USART1_RTOR                    (0x40020224U) /**< \brief (USART1) USART Receiver Time-out Register */
  #define REG_USART1_TTGR                    (0x40020228U) /**< \brief (USART1) USART Transmitter Timeguard Register */
  #define REG_USART1_FIDI                    (0x40020240U) /**< \brief (USART1) USART FI DI Ratio Register */
  #define REG_USART1_NER                     (0x40020244U) /**< \brief (USART1) USART Number of Errors Register */
  #define REG_USART1_LINMR                   (0x40020254U) /**< \brief (USART1) USART LIN Mode Register */
  #define REG_USART1_LINIR                   (0x40020258U) /**< \brief (USART1) USART LIN Identifier Register */
  #define REG_USART1_LINBRR                  (0x4002025CU) /**< \brief (USART1) USART LIN Baud Rate Register */
  #define REG_USART1_CMPR                    (0x40020290U) /**< \brief (USART1) USART Comparison Register */
  #define REG_USART1_WPMR                    (0x400202E4U) /**< \brief (USART1) USART Write Protection Mode Register */
  #define REG_USART1_WPSR                    (0x400202E8U) /**< \brief (USART1) USART Write Protection Status Register */
  #define REG_USART1_RPR                     (0x40020300U) /**< \brief (USART1) Receive Pointer Register */
  #define REG_USART1_RCR                     (0x40020304U) /**< \brief (USART1) Receive Counter Register */
  #define REG_USART1_TPR                     (0x40020308U) /**< \brief (USART1) Transmit Pointer Register */
  #define REG_USART1_TCR                     (0x4002030CU) /**< \brief (USART1) Transmit Counter Register */
  #define REG_USART1_RNPR                    (0x40020310U) /**< \brief (USART1) Receive Next Pointer Register */
  #define REG_USART1_RNCR                    (0x40020314U) /**< \brief (USART1) Receive Next Counter Register */
  #define REG_USART1_TNPR                    (0x40020318U) /**< \brief (USART1) Transmit Next Pointer Register */
  #define REG_USART1_TNCR                    (0x4002031CU) /**< \brief (USART1) Transmit Next Counter Register */
  #define REG_USART1_PTCR                    (0x40020320U) /**< \brief (USART1) Transfer Control Register */
  #define REG_USART1_PTSR                    (0x40020324U) /**< \brief (USART1) Transfer Status Register */
#else
  #define REG_USART1_CR     (*(__O  uint32_t*)0x40020200U) /**< \brief (USART1) USART Control Register */
  #define REG_USART1_MR     (*(__IO uint32_t*)0x40020204U) /**< \brief (USART1) USART Mode Register */
  #define REG_USART1_IER    (*(__O  uint32_t*)0x40020208U) /**< \brief (USART1) USART Interrupt Enable Register */
  #define REG_USART1_IDR    (*(__O  uint32_t*)0x4002020CU) /**< \brief (USART1) USART Interrupt Disable Register */
  #define REG_USART1_IMR    (*(__I  uint32_t*)0x40020210U) /**< \brief (USART1) USART Interrupt Mask Register */
  #define REG_USART1_CSR    (*(__I  uint32_t*)0x40020214U) /**< \brief (USART1) USART Channel Status Register */
  #define REG_USART1_RHR    (*(__I  uint32_t*)0x40020218U) /**< \brief (USART1) USART Receive Holding Register */
  #define REG_USART1_THR    (*(__O  uint32_t*)0x4002021CU) /**< \brief (USART1) USART Transmit Holding Register */
  #define REG_USART1_BRGR   (*(__IO uint32_t*)0x40020220U) /**< \brief (USART1) USART Baud Rate Generator Register */
  #define REG_USART1_RTOR   (*(__IO uint32_t*)0x40020224U) /**< \brief (USART1) USART Receiver Time-out Register */
  #define REG_USART1_TTGR   (*(__IO uint32_t*)0x40020228U) /**< \brief (USART1) USART Transmitter Timeguard Register */
  #define REG_USART1_FIDI   (*(__IO uint32_t*)0x40020240U) /**< \brief (USART1) USART FI DI Ratio Register */
  #define REG_USART1_NER    (*(__I  uint32_t*)0x40020244U) /**< \brief (USART1) USART Number of Errors Register */
  #define REG_USART1_LINMR  (*(__IO uint32_t*)0x40020254U) /**< \brief (USART1) USART LIN Mode Register */
  #define REG_USART1_LINIR  (*(__IO uint32_t*)0x40020258U) /**< \brief (USART1) USART LIN Identifier Register */
  #define REG_USART1_LINBRR (*(__I  uint32_t*)0x4002025CU) /**< \brief (USART1) USART LIN Baud Rate Register */
  #define REG_USART1_CMPR   (*(__IO uint32_t*)0x40020290U) /**< \brief (USART1) USART Comparison Register */
  #define REG_USART1_WPMR   (*(__IO uint32_t*)0x400202E4U) /**< \brief (USART1) USART Write Protection Mode Register */
  #define REG_USART1_WPSR   (*(__I  uint32_t*)0x400202E8U) /**< \brief (USART1) USART Write Protection Status Register */
  #define REG_USART1_RPR    (*(__IO uint32_t*)0x40020300U) /**< \brief (USART1) Receive Pointer Register */
  #define REG_USART1_RCR    (*(__IO uint32_t*)0x40020304U) /**< \brief (USART1) Receive Counter Register */
  #define REG_USART1_TPR    (*(__IO uint32_t*)0x40020308U) /**< \brief (USART1) Transmit Pointer Register */
  #define REG_USART1_TCR    (*(__IO uint32_t*)0x4002030CU) /**< \brief (USART1) Transmit Counter Register */
  #define REG_USART1_RNPR   (*(__IO uint32_t*)0x40020310U) /**< \brief (USART1) Receive Next Pointer Register */
  #define REG_USART1_RNCR   (*(__IO uint32_t*)0x40020314U) /**< \brief (USART1) Receive Next Counter Register */
  #define REG_USART1_TNPR   (*(__IO uint32_t*)0x40020318U) /**< \brief (USART1) Transmit Next Pointer Register */
  #define REG_USART1_TNCR   (*(__IO uint32_t*)0x4002031CU) /**< \brief (USART1) Transmit Next Counter Register */
  #define REG_USART1_PTCR   (*(__O  uint32_t*)0x40020320U) /**< \brief (USART1) Transfer Control Register */
  #define REG_USART1_PTSR   (*(__I  uint32_t*)0x40020324U) /**< \brief (USART1) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAMG55_USART1_INSTANCE_ */
