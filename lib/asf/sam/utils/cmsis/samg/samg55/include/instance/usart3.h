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

#ifndef _SAMG55_USART3_INSTANCE_
#define _SAMG55_USART3_INSTANCE_

/* ========== Register definition for USART3 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_USART3_CR                      (0x40018200U) /**< \brief (USART3) USART Control Register */
  #define REG_USART3_MR                      (0x40018204U) /**< \brief (USART3) USART Mode Register */
  #define REG_USART3_IER                     (0x40018208U) /**< \brief (USART3) USART Interrupt Enable Register */
  #define REG_USART3_IDR                     (0x4001820CU) /**< \brief (USART3) USART Interrupt Disable Register */
  #define REG_USART3_IMR                     (0x40018210U) /**< \brief (USART3) USART Interrupt Mask Register */
  #define REG_USART3_CSR                     (0x40018214U) /**< \brief (USART3) USART Channel Status Register */
  #define REG_USART3_RHR                     (0x40018218U) /**< \brief (USART3) USART Receive Holding Register */
  #define REG_USART3_THR                     (0x4001821CU) /**< \brief (USART3) USART Transmit Holding Register */
  #define REG_USART3_BRGR                    (0x40018220U) /**< \brief (USART3) USART Baud Rate Generator Register */
  #define REG_USART3_RTOR                    (0x40018224U) /**< \brief (USART3) USART Receiver Time-out Register */
  #define REG_USART3_TTGR                    (0x40018228U) /**< \brief (USART3) USART Transmitter Timeguard Register */
  #define REG_USART3_FIDI                    (0x40018240U) /**< \brief (USART3) USART FI DI Ratio Register */
  #define REG_USART3_NER                     (0x40018244U) /**< \brief (USART3) USART Number of Errors Register */
  #define REG_USART3_LINMR                   (0x40018254U) /**< \brief (USART3) USART LIN Mode Register */
  #define REG_USART3_LINIR                   (0x40018258U) /**< \brief (USART3) USART LIN Identifier Register */
  #define REG_USART3_LINBRR                  (0x4001825CU) /**< \brief (USART3) USART LIN Baud Rate Register */
  #define REG_USART3_CMPR                    (0x40018290U) /**< \brief (USART3) USART Comparison Register */
  #define REG_USART3_WPMR                    (0x400182E4U) /**< \brief (USART3) USART Write Protection Mode Register */
  #define REG_USART3_WPSR                    (0x400182E8U) /**< \brief (USART3) USART Write Protection Status Register */
  #define REG_USART3_RPR                     (0x40018300U) /**< \brief (USART3) Receive Pointer Register */
  #define REG_USART3_RCR                     (0x40018304U) /**< \brief (USART3) Receive Counter Register */
  #define REG_USART3_TPR                     (0x40018308U) /**< \brief (USART3) Transmit Pointer Register */
  #define REG_USART3_TCR                     (0x4001830CU) /**< \brief (USART3) Transmit Counter Register */
  #define REG_USART3_RNPR                    (0x40018310U) /**< \brief (USART3) Receive Next Pointer Register */
  #define REG_USART3_RNCR                    (0x40018314U) /**< \brief (USART3) Receive Next Counter Register */
  #define REG_USART3_TNPR                    (0x40018318U) /**< \brief (USART3) Transmit Next Pointer Register */
  #define REG_USART3_TNCR                    (0x4001831CU) /**< \brief (USART3) Transmit Next Counter Register */
  #define REG_USART3_PTCR                    (0x40018320U) /**< \brief (USART3) Transfer Control Register */
  #define REG_USART3_PTSR                    (0x40018324U) /**< \brief (USART3) Transfer Status Register */
#else
  #define REG_USART3_CR     (*(__O  uint32_t*)0x40018200U) /**< \brief (USART3) USART Control Register */
  #define REG_USART3_MR     (*(__IO uint32_t*)0x40018204U) /**< \brief (USART3) USART Mode Register */
  #define REG_USART3_IER    (*(__O  uint32_t*)0x40018208U) /**< \brief (USART3) USART Interrupt Enable Register */
  #define REG_USART3_IDR    (*(__O  uint32_t*)0x4001820CU) /**< \brief (USART3) USART Interrupt Disable Register */
  #define REG_USART3_IMR    (*(__I  uint32_t*)0x40018210U) /**< \brief (USART3) USART Interrupt Mask Register */
  #define REG_USART3_CSR    (*(__I  uint32_t*)0x40018214U) /**< \brief (USART3) USART Channel Status Register */
  #define REG_USART3_RHR    (*(__I  uint32_t*)0x40018218U) /**< \brief (USART3) USART Receive Holding Register */
  #define REG_USART3_THR    (*(__O  uint32_t*)0x4001821CU) /**< \brief (USART3) USART Transmit Holding Register */
  #define REG_USART3_BRGR   (*(__IO uint32_t*)0x40018220U) /**< \brief (USART3) USART Baud Rate Generator Register */
  #define REG_USART3_RTOR   (*(__IO uint32_t*)0x40018224U) /**< \brief (USART3) USART Receiver Time-out Register */
  #define REG_USART3_TTGR   (*(__IO uint32_t*)0x40018228U) /**< \brief (USART3) USART Transmitter Timeguard Register */
  #define REG_USART3_FIDI   (*(__IO uint32_t*)0x40018240U) /**< \brief (USART3) USART FI DI Ratio Register */
  #define REG_USART3_NER    (*(__I  uint32_t*)0x40018244U) /**< \brief (USART3) USART Number of Errors Register */
  #define REG_USART3_LINMR  (*(__IO uint32_t*)0x40018254U) /**< \brief (USART3) USART LIN Mode Register */
  #define REG_USART3_LINIR  (*(__IO uint32_t*)0x40018258U) /**< \brief (USART3) USART LIN Identifier Register */
  #define REG_USART3_LINBRR (*(__I  uint32_t*)0x4001825CU) /**< \brief (USART3) USART LIN Baud Rate Register */
  #define REG_USART3_CMPR   (*(__IO uint32_t*)0x40018290U) /**< \brief (USART3) USART Comparison Register */
  #define REG_USART3_WPMR   (*(__IO uint32_t*)0x400182E4U) /**< \brief (USART3) USART Write Protection Mode Register */
  #define REG_USART3_WPSR   (*(__I  uint32_t*)0x400182E8U) /**< \brief (USART3) USART Write Protection Status Register */
  #define REG_USART3_RPR    (*(__IO uint32_t*)0x40018300U) /**< \brief (USART3) Receive Pointer Register */
  #define REG_USART3_RCR    (*(__IO uint32_t*)0x40018304U) /**< \brief (USART3) Receive Counter Register */
  #define REG_USART3_TPR    (*(__IO uint32_t*)0x40018308U) /**< \brief (USART3) Transmit Pointer Register */
  #define REG_USART3_TCR    (*(__IO uint32_t*)0x4001830CU) /**< \brief (USART3) Transmit Counter Register */
  #define REG_USART3_RNPR   (*(__IO uint32_t*)0x40018310U) /**< \brief (USART3) Receive Next Pointer Register */
  #define REG_USART3_RNCR   (*(__IO uint32_t*)0x40018314U) /**< \brief (USART3) Receive Next Counter Register */
  #define REG_USART3_TNPR   (*(__IO uint32_t*)0x40018318U) /**< \brief (USART3) Transmit Next Pointer Register */
  #define REG_USART3_TNCR   (*(__IO uint32_t*)0x4001831CU) /**< \brief (USART3) Transmit Next Counter Register */
  #define REG_USART3_PTCR   (*(__O  uint32_t*)0x40018320U) /**< \brief (USART3) Transfer Control Register */
  #define REG_USART3_PTSR   (*(__I  uint32_t*)0x40018324U) /**< \brief (USART3) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAMG55_USART3_INSTANCE_ */
