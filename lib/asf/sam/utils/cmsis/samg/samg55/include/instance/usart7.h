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

#ifndef _SAMG55_USART7_INSTANCE_
#define _SAMG55_USART7_INSTANCE_

/* ========== Register definition for USART7 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_USART7_CR                      (0x40034200U) /**< \brief (USART7) USART Control Register */
  #define REG_USART7_MR                      (0x40034204U) /**< \brief (USART7) USART Mode Register */
  #define REG_USART7_IER                     (0x40034208U) /**< \brief (USART7) USART Interrupt Enable Register */
  #define REG_USART7_IDR                     (0x4003420CU) /**< \brief (USART7) USART Interrupt Disable Register */
  #define REG_USART7_IMR                     (0x40034210U) /**< \brief (USART7) USART Interrupt Mask Register */
  #define REG_USART7_CSR                     (0x40034214U) /**< \brief (USART7) USART Channel Status Register */
  #define REG_USART7_RHR                     (0x40034218U) /**< \brief (USART7) USART Receive Holding Register */
  #define REG_USART7_THR                     (0x4003421CU) /**< \brief (USART7) USART Transmit Holding Register */
  #define REG_USART7_BRGR                    (0x40034220U) /**< \brief (USART7) USART Baud Rate Generator Register */
  #define REG_USART7_RTOR                    (0x40034224U) /**< \brief (USART7) USART Receiver Time-out Register */
  #define REG_USART7_TTGR                    (0x40034228U) /**< \brief (USART7) USART Transmitter Timeguard Register */
  #define REG_USART7_FIDI                    (0x40034240U) /**< \brief (USART7) USART FI DI Ratio Register */
  #define REG_USART7_NER                     (0x40034244U) /**< \brief (USART7) USART Number of Errors Register */
  #define REG_USART7_LINMR                   (0x40034254U) /**< \brief (USART7) USART LIN Mode Register */
  #define REG_USART7_LINIR                   (0x40034258U) /**< \brief (USART7) USART LIN Identifier Register */
  #define REG_USART7_LINBRR                  (0x4003425CU) /**< \brief (USART7) USART LIN Baud Rate Register */
  #define REG_USART7_CMPR                    (0x40034290U) /**< \brief (USART7) USART Comparison Register */
  #define REG_USART7_WPMR                    (0x400342E4U) /**< \brief (USART7) USART Write Protection Mode Register */
  #define REG_USART7_WPSR                    (0x400342E8U) /**< \brief (USART7) USART Write Protection Status Register */
  #define REG_USART7_RPR                     (0x40034300U) /**< \brief (USART7) Receive Pointer Register */
  #define REG_USART7_RCR                     (0x40034304U) /**< \brief (USART7) Receive Counter Register */
  #define REG_USART7_TPR                     (0x40034308U) /**< \brief (USART7) Transmit Pointer Register */
  #define REG_USART7_TCR                     (0x4003430CU) /**< \brief (USART7) Transmit Counter Register */
  #define REG_USART7_RNPR                    (0x40034310U) /**< \brief (USART7) Receive Next Pointer Register */
  #define REG_USART7_RNCR                    (0x40034314U) /**< \brief (USART7) Receive Next Counter Register */
  #define REG_USART7_TNPR                    (0x40034318U) /**< \brief (USART7) Transmit Next Pointer Register */
  #define REG_USART7_TNCR                    (0x4003431CU) /**< \brief (USART7) Transmit Next Counter Register */
  #define REG_USART7_PTCR                    (0x40034320U) /**< \brief (USART7) Transfer Control Register */
  #define REG_USART7_PTSR                    (0x40034324U) /**< \brief (USART7) Transfer Status Register */
#else
  #define REG_USART7_CR     (*(__O  uint32_t*)0x40034200U) /**< \brief (USART7) USART Control Register */
  #define REG_USART7_MR     (*(__IO uint32_t*)0x40034204U) /**< \brief (USART7) USART Mode Register */
  #define REG_USART7_IER    (*(__O  uint32_t*)0x40034208U) /**< \brief (USART7) USART Interrupt Enable Register */
  #define REG_USART7_IDR    (*(__O  uint32_t*)0x4003420CU) /**< \brief (USART7) USART Interrupt Disable Register */
  #define REG_USART7_IMR    (*(__I  uint32_t*)0x40034210U) /**< \brief (USART7) USART Interrupt Mask Register */
  #define REG_USART7_CSR    (*(__I  uint32_t*)0x40034214U) /**< \brief (USART7) USART Channel Status Register */
  #define REG_USART7_RHR    (*(__I  uint32_t*)0x40034218U) /**< \brief (USART7) USART Receive Holding Register */
  #define REG_USART7_THR    (*(__O  uint32_t*)0x4003421CU) /**< \brief (USART7) USART Transmit Holding Register */
  #define REG_USART7_BRGR   (*(__IO uint32_t*)0x40034220U) /**< \brief (USART7) USART Baud Rate Generator Register */
  #define REG_USART7_RTOR   (*(__IO uint32_t*)0x40034224U) /**< \brief (USART7) USART Receiver Time-out Register */
  #define REG_USART7_TTGR   (*(__IO uint32_t*)0x40034228U) /**< \brief (USART7) USART Transmitter Timeguard Register */
  #define REG_USART7_FIDI   (*(__IO uint32_t*)0x40034240U) /**< \brief (USART7) USART FI DI Ratio Register */
  #define REG_USART7_NER    (*(__I  uint32_t*)0x40034244U) /**< \brief (USART7) USART Number of Errors Register */
  #define REG_USART7_LINMR  (*(__IO uint32_t*)0x40034254U) /**< \brief (USART7) USART LIN Mode Register */
  #define REG_USART7_LINIR  (*(__IO uint32_t*)0x40034258U) /**< \brief (USART7) USART LIN Identifier Register */
  #define REG_USART7_LINBRR (*(__I  uint32_t*)0x4003425CU) /**< \brief (USART7) USART LIN Baud Rate Register */
  #define REG_USART7_CMPR   (*(__IO uint32_t*)0x40034290U) /**< \brief (USART7) USART Comparison Register */
  #define REG_USART7_WPMR   (*(__IO uint32_t*)0x400342E4U) /**< \brief (USART7) USART Write Protection Mode Register */
  #define REG_USART7_WPSR   (*(__I  uint32_t*)0x400342E8U) /**< \brief (USART7) USART Write Protection Status Register */
  #define REG_USART7_RPR    (*(__IO uint32_t*)0x40034300U) /**< \brief (USART7) Receive Pointer Register */
  #define REG_USART7_RCR    (*(__IO uint32_t*)0x40034304U) /**< \brief (USART7) Receive Counter Register */
  #define REG_USART7_TPR    (*(__IO uint32_t*)0x40034308U) /**< \brief (USART7) Transmit Pointer Register */
  #define REG_USART7_TCR    (*(__IO uint32_t*)0x4003430CU) /**< \brief (USART7) Transmit Counter Register */
  #define REG_USART7_RNPR   (*(__IO uint32_t*)0x40034310U) /**< \brief (USART7) Receive Next Pointer Register */
  #define REG_USART7_RNCR   (*(__IO uint32_t*)0x40034314U) /**< \brief (USART7) Receive Next Counter Register */
  #define REG_USART7_TNPR   (*(__IO uint32_t*)0x40034318U) /**< \brief (USART7) Transmit Next Pointer Register */
  #define REG_USART7_TNCR   (*(__IO uint32_t*)0x4003431CU) /**< \brief (USART7) Transmit Next Counter Register */
  #define REG_USART7_PTCR   (*(__O  uint32_t*)0x40034320U) /**< \brief (USART7) Transfer Control Register */
  #define REG_USART7_PTSR   (*(__I  uint32_t*)0x40034324U) /**< \brief (USART7) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAMG55_USART7_INSTANCE_ */
