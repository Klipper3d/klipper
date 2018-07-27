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

#ifndef _SAMG55_USART0_INSTANCE_
#define _SAMG55_USART0_INSTANCE_

/* ========== Register definition for USART0 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_USART0_CR                      (0x4000C200U) /**< \brief (USART0) USART Control Register */
  #define REG_USART0_MR                      (0x4000C204U) /**< \brief (USART0) USART Mode Register */
  #define REG_USART0_IER                     (0x4000C208U) /**< \brief (USART0) USART Interrupt Enable Register */
  #define REG_USART0_IDR                     (0x4000C20CU) /**< \brief (USART0) USART Interrupt Disable Register */
  #define REG_USART0_IMR                     (0x4000C210U) /**< \brief (USART0) USART Interrupt Mask Register */
  #define REG_USART0_CSR                     (0x4000C214U) /**< \brief (USART0) USART Channel Status Register */
  #define REG_USART0_RHR                     (0x4000C218U) /**< \brief (USART0) USART Receive Holding Register */
  #define REG_USART0_THR                     (0x4000C21CU) /**< \brief (USART0) USART Transmit Holding Register */
  #define REG_USART0_BRGR                    (0x4000C220U) /**< \brief (USART0) USART Baud Rate Generator Register */
  #define REG_USART0_RTOR                    (0x4000C224U) /**< \brief (USART0) USART Receiver Time-out Register */
  #define REG_USART0_TTGR                    (0x4000C228U) /**< \brief (USART0) USART Transmitter Timeguard Register */
  #define REG_USART0_FIDI                    (0x4000C240U) /**< \brief (USART0) USART FI DI Ratio Register */
  #define REG_USART0_NER                     (0x4000C244U) /**< \brief (USART0) USART Number of Errors Register */
  #define REG_USART0_LINMR                   (0x4000C254U) /**< \brief (USART0) USART LIN Mode Register */
  #define REG_USART0_LINIR                   (0x4000C258U) /**< \brief (USART0) USART LIN Identifier Register */
  #define REG_USART0_LINBRR                  (0x4000C25CU) /**< \brief (USART0) USART LIN Baud Rate Register */
  #define REG_USART0_CMPR                    (0x4000C290U) /**< \brief (USART0) USART Comparison Register */
  #define REG_USART0_WPMR                    (0x4000C2E4U) /**< \brief (USART0) USART Write Protection Mode Register */
  #define REG_USART0_WPSR                    (0x4000C2E8U) /**< \brief (USART0) USART Write Protection Status Register */
  #define REG_USART0_RPR                     (0x4000C300U) /**< \brief (USART0) Receive Pointer Register */
  #define REG_USART0_RCR                     (0x4000C304U) /**< \brief (USART0) Receive Counter Register */
  #define REG_USART0_TPR                     (0x4000C308U) /**< \brief (USART0) Transmit Pointer Register */
  #define REG_USART0_TCR                     (0x4000C30CU) /**< \brief (USART0) Transmit Counter Register */
  #define REG_USART0_RNPR                    (0x4000C310U) /**< \brief (USART0) Receive Next Pointer Register */
  #define REG_USART0_RNCR                    (0x4000C314U) /**< \brief (USART0) Receive Next Counter Register */
  #define REG_USART0_TNPR                    (0x4000C318U) /**< \brief (USART0) Transmit Next Pointer Register */
  #define REG_USART0_TNCR                    (0x4000C31CU) /**< \brief (USART0) Transmit Next Counter Register */
  #define REG_USART0_PTCR                    (0x4000C320U) /**< \brief (USART0) Transfer Control Register */
  #define REG_USART0_PTSR                    (0x4000C324U) /**< \brief (USART0) Transfer Status Register */
#else
  #define REG_USART0_CR     (*(__O  uint32_t*)0x4000C200U) /**< \brief (USART0) USART Control Register */
  #define REG_USART0_MR     (*(__IO uint32_t*)0x4000C204U) /**< \brief (USART0) USART Mode Register */
  #define REG_USART0_IER    (*(__O  uint32_t*)0x4000C208U) /**< \brief (USART0) USART Interrupt Enable Register */
  #define REG_USART0_IDR    (*(__O  uint32_t*)0x4000C20CU) /**< \brief (USART0) USART Interrupt Disable Register */
  #define REG_USART0_IMR    (*(__I  uint32_t*)0x4000C210U) /**< \brief (USART0) USART Interrupt Mask Register */
  #define REG_USART0_CSR    (*(__I  uint32_t*)0x4000C214U) /**< \brief (USART0) USART Channel Status Register */
  #define REG_USART0_RHR    (*(__I  uint32_t*)0x4000C218U) /**< \brief (USART0) USART Receive Holding Register */
  #define REG_USART0_THR    (*(__O  uint32_t*)0x4000C21CU) /**< \brief (USART0) USART Transmit Holding Register */
  #define REG_USART0_BRGR   (*(__IO uint32_t*)0x4000C220U) /**< \brief (USART0) USART Baud Rate Generator Register */
  #define REG_USART0_RTOR   (*(__IO uint32_t*)0x4000C224U) /**< \brief (USART0) USART Receiver Time-out Register */
  #define REG_USART0_TTGR   (*(__IO uint32_t*)0x4000C228U) /**< \brief (USART0) USART Transmitter Timeguard Register */
  #define REG_USART0_FIDI   (*(__IO uint32_t*)0x4000C240U) /**< \brief (USART0) USART FI DI Ratio Register */
  #define REG_USART0_NER    (*(__I  uint32_t*)0x4000C244U) /**< \brief (USART0) USART Number of Errors Register */
  #define REG_USART0_LINMR  (*(__IO uint32_t*)0x4000C254U) /**< \brief (USART0) USART LIN Mode Register */
  #define REG_USART0_LINIR  (*(__IO uint32_t*)0x4000C258U) /**< \brief (USART0) USART LIN Identifier Register */
  #define REG_USART0_LINBRR (*(__I  uint32_t*)0x4000C25CU) /**< \brief (USART0) USART LIN Baud Rate Register */
  #define REG_USART0_CMPR   (*(__IO uint32_t*)0x4000C290U) /**< \brief (USART0) USART Comparison Register */
  #define REG_USART0_WPMR   (*(__IO uint32_t*)0x4000C2E4U) /**< \brief (USART0) USART Write Protection Mode Register */
  #define REG_USART0_WPSR   (*(__I  uint32_t*)0x4000C2E8U) /**< \brief (USART0) USART Write Protection Status Register */
  #define REG_USART0_RPR    (*(__IO uint32_t*)0x4000C300U) /**< \brief (USART0) Receive Pointer Register */
  #define REG_USART0_RCR    (*(__IO uint32_t*)0x4000C304U) /**< \brief (USART0) Receive Counter Register */
  #define REG_USART0_TPR    (*(__IO uint32_t*)0x4000C308U) /**< \brief (USART0) Transmit Pointer Register */
  #define REG_USART0_TCR    (*(__IO uint32_t*)0x4000C30CU) /**< \brief (USART0) Transmit Counter Register */
  #define REG_USART0_RNPR   (*(__IO uint32_t*)0x4000C310U) /**< \brief (USART0) Receive Next Pointer Register */
  #define REG_USART0_RNCR   (*(__IO uint32_t*)0x4000C314U) /**< \brief (USART0) Receive Next Counter Register */
  #define REG_USART0_TNPR   (*(__IO uint32_t*)0x4000C318U) /**< \brief (USART0) Transmit Next Pointer Register */
  #define REG_USART0_TNCR   (*(__IO uint32_t*)0x4000C31CU) /**< \brief (USART0) Transmit Next Counter Register */
  #define REG_USART0_PTCR   (*(__O  uint32_t*)0x4000C320U) /**< \brief (USART0) Transfer Control Register */
  #define REG_USART0_PTSR   (*(__I  uint32_t*)0x4000C324U) /**< \brief (USART0) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAMG55_USART0_INSTANCE_ */
