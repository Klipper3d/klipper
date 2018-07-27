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

#ifndef _SAMG55_USART4_INSTANCE_
#define _SAMG55_USART4_INSTANCE_

/* ========== Register definition for USART4 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_USART4_CR                      (0x4001C200U) /**< \brief (USART4) USART Control Register */
  #define REG_USART4_MR                      (0x4001C204U) /**< \brief (USART4) USART Mode Register */
  #define REG_USART4_IER                     (0x4001C208U) /**< \brief (USART4) USART Interrupt Enable Register */
  #define REG_USART4_IDR                     (0x4001C20CU) /**< \brief (USART4) USART Interrupt Disable Register */
  #define REG_USART4_IMR                     (0x4001C210U) /**< \brief (USART4) USART Interrupt Mask Register */
  #define REG_USART4_CSR                     (0x4001C214U) /**< \brief (USART4) USART Channel Status Register */
  #define REG_USART4_RHR                     (0x4001C218U) /**< \brief (USART4) USART Receive Holding Register */
  #define REG_USART4_THR                     (0x4001C21CU) /**< \brief (USART4) USART Transmit Holding Register */
  #define REG_USART4_BRGR                    (0x4001C220U) /**< \brief (USART4) USART Baud Rate Generator Register */
  #define REG_USART4_RTOR                    (0x4001C224U) /**< \brief (USART4) USART Receiver Time-out Register */
  #define REG_USART4_TTGR                    (0x4001C228U) /**< \brief (USART4) USART Transmitter Timeguard Register */
  #define REG_USART4_FIDI                    (0x4001C240U) /**< \brief (USART4) USART FI DI Ratio Register */
  #define REG_USART4_NER                     (0x4001C244U) /**< \brief (USART4) USART Number of Errors Register */
  #define REG_USART4_LINMR                   (0x4001C254U) /**< \brief (USART4) USART LIN Mode Register */
  #define REG_USART4_LINIR                   (0x4001C258U) /**< \brief (USART4) USART LIN Identifier Register */
  #define REG_USART4_LINBRR                  (0x4001C25CU) /**< \brief (USART4) USART LIN Baud Rate Register */
  #define REG_USART4_CMPR                    (0x4001C290U) /**< \brief (USART4) USART Comparison Register */
  #define REG_USART4_WPMR                    (0x4001C2E4U) /**< \brief (USART4) USART Write Protection Mode Register */
  #define REG_USART4_WPSR                    (0x4001C2E8U) /**< \brief (USART4) USART Write Protection Status Register */
  #define REG_USART4_RPR                     (0x4001C300U) /**< \brief (USART4) Receive Pointer Register */
  #define REG_USART4_RCR                     (0x4001C304U) /**< \brief (USART4) Receive Counter Register */
  #define REG_USART4_TPR                     (0x4001C308U) /**< \brief (USART4) Transmit Pointer Register */
  #define REG_USART4_TCR                     (0x4001C30CU) /**< \brief (USART4) Transmit Counter Register */
  #define REG_USART4_RNPR                    (0x4001C310U) /**< \brief (USART4) Receive Next Pointer Register */
  #define REG_USART4_RNCR                    (0x4001C314U) /**< \brief (USART4) Receive Next Counter Register */
  #define REG_USART4_TNPR                    (0x4001C318U) /**< \brief (USART4) Transmit Next Pointer Register */
  #define REG_USART4_TNCR                    (0x4001C31CU) /**< \brief (USART4) Transmit Next Counter Register */
  #define REG_USART4_PTCR                    (0x4001C320U) /**< \brief (USART4) Transfer Control Register */
  #define REG_USART4_PTSR                    (0x4001C324U) /**< \brief (USART4) Transfer Status Register */
#else
  #define REG_USART4_CR     (*(__O  uint32_t*)0x4001C200U) /**< \brief (USART4) USART Control Register */
  #define REG_USART4_MR     (*(__IO uint32_t*)0x4001C204U) /**< \brief (USART4) USART Mode Register */
  #define REG_USART4_IER    (*(__O  uint32_t*)0x4001C208U) /**< \brief (USART4) USART Interrupt Enable Register */
  #define REG_USART4_IDR    (*(__O  uint32_t*)0x4001C20CU) /**< \brief (USART4) USART Interrupt Disable Register */
  #define REG_USART4_IMR    (*(__I  uint32_t*)0x4001C210U) /**< \brief (USART4) USART Interrupt Mask Register */
  #define REG_USART4_CSR    (*(__I  uint32_t*)0x4001C214U) /**< \brief (USART4) USART Channel Status Register */
  #define REG_USART4_RHR    (*(__I  uint32_t*)0x4001C218U) /**< \brief (USART4) USART Receive Holding Register */
  #define REG_USART4_THR    (*(__O  uint32_t*)0x4001C21CU) /**< \brief (USART4) USART Transmit Holding Register */
  #define REG_USART4_BRGR   (*(__IO uint32_t*)0x4001C220U) /**< \brief (USART4) USART Baud Rate Generator Register */
  #define REG_USART4_RTOR   (*(__IO uint32_t*)0x4001C224U) /**< \brief (USART4) USART Receiver Time-out Register */
  #define REG_USART4_TTGR   (*(__IO uint32_t*)0x4001C228U) /**< \brief (USART4) USART Transmitter Timeguard Register */
  #define REG_USART4_FIDI   (*(__IO uint32_t*)0x4001C240U) /**< \brief (USART4) USART FI DI Ratio Register */
  #define REG_USART4_NER    (*(__I  uint32_t*)0x4001C244U) /**< \brief (USART4) USART Number of Errors Register */
  #define REG_USART4_LINMR  (*(__IO uint32_t*)0x4001C254U) /**< \brief (USART4) USART LIN Mode Register */
  #define REG_USART4_LINIR  (*(__IO uint32_t*)0x4001C258U) /**< \brief (USART4) USART LIN Identifier Register */
  #define REG_USART4_LINBRR (*(__I  uint32_t*)0x4001C25CU) /**< \brief (USART4) USART LIN Baud Rate Register */
  #define REG_USART4_CMPR   (*(__IO uint32_t*)0x4001C290U) /**< \brief (USART4) USART Comparison Register */
  #define REG_USART4_WPMR   (*(__IO uint32_t*)0x4001C2E4U) /**< \brief (USART4) USART Write Protection Mode Register */
  #define REG_USART4_WPSR   (*(__I  uint32_t*)0x4001C2E8U) /**< \brief (USART4) USART Write Protection Status Register */
  #define REG_USART4_RPR    (*(__IO uint32_t*)0x4001C300U) /**< \brief (USART4) Receive Pointer Register */
  #define REG_USART4_RCR    (*(__IO uint32_t*)0x4001C304U) /**< \brief (USART4) Receive Counter Register */
  #define REG_USART4_TPR    (*(__IO uint32_t*)0x4001C308U) /**< \brief (USART4) Transmit Pointer Register */
  #define REG_USART4_TCR    (*(__IO uint32_t*)0x4001C30CU) /**< \brief (USART4) Transmit Counter Register */
  #define REG_USART4_RNPR   (*(__IO uint32_t*)0x4001C310U) /**< \brief (USART4) Receive Next Pointer Register */
  #define REG_USART4_RNCR   (*(__IO uint32_t*)0x4001C314U) /**< \brief (USART4) Receive Next Counter Register */
  #define REG_USART4_TNPR   (*(__IO uint32_t*)0x4001C318U) /**< \brief (USART4) Transmit Next Pointer Register */
  #define REG_USART4_TNCR   (*(__IO uint32_t*)0x4001C31CU) /**< \brief (USART4) Transmit Next Counter Register */
  #define REG_USART4_PTCR   (*(__O  uint32_t*)0x4001C320U) /**< \brief (USART4) Transfer Control Register */
  #define REG_USART4_PTSR   (*(__I  uint32_t*)0x4001C324U) /**< \brief (USART4) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAMG55_USART4_INSTANCE_ */
