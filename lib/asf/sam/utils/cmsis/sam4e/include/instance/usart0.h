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

#ifndef _SAM4E_USART0_INSTANCE_
#define _SAM4E_USART0_INSTANCE_

/* ========== Register definition for USART0 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_USART0_CR                    (0x400A0000U) /**< \brief (USART0) Control Register */
  #define REG_USART0_MR                    (0x400A0004U) /**< \brief (USART0) Mode Register */
  #define REG_USART0_IER                   (0x400A0008U) /**< \brief (USART0) Interrupt Enable Register */
  #define REG_USART0_IDR                   (0x400A000CU) /**< \brief (USART0) Interrupt Disable Register */
  #define REG_USART0_IMR                   (0x400A0010U) /**< \brief (USART0) Interrupt Mask Register */
  #define REG_USART0_CSR                   (0x400A0014U) /**< \brief (USART0) Channel Status Register */
  #define REG_USART0_RHR                   (0x400A0018U) /**< \brief (USART0) Receive Holding Register */
  #define REG_USART0_THR                   (0x400A001CU) /**< \brief (USART0) Transmit Holding Register */
  #define REG_USART0_BRGR                  (0x400A0020U) /**< \brief (USART0) Baud Rate Generator Register */
  #define REG_USART0_RTOR                  (0x400A0024U) /**< \brief (USART0) Receiver Time-out Register */
  #define REG_USART0_TTGR                  (0x400A0028U) /**< \brief (USART0) Transmitter Timeguard Register */
  #define REG_USART0_FIDI                  (0x400A0040U) /**< \brief (USART0) FI DI Ratio Register */
  #define REG_USART0_NER                   (0x400A0044U) /**< \brief (USART0) Number of Errors Register */
  #define REG_USART0_IF                    (0x400A004CU) /**< \brief (USART0) IrDA Filter Register */
  #define REG_USART0_MAN                   (0x400A0050U) /**< \brief (USART0) Manchester Configuration Register */
  #define REG_USART0_WPMR                  (0x400A00E4U) /**< \brief (USART0) Write Protection Mode Register */
  #define REG_USART0_WPSR                  (0x400A00E8U) /**< \brief (USART0) Write Protection Status Register */
  #define REG_USART0_RPR                   (0x400A0100U) /**< \brief (USART0) Receive Pointer Register */
  #define REG_USART0_RCR                   (0x400A0104U) /**< \brief (USART0) Receive Counter Register */
  #define REG_USART0_TPR                   (0x400A0108U) /**< \brief (USART0) Transmit Pointer Register */
  #define REG_USART0_TCR                   (0x400A010CU) /**< \brief (USART0) Transmit Counter Register */
  #define REG_USART0_RNPR                  (0x400A0110U) /**< \brief (USART0) Receive Next Pointer Register */
  #define REG_USART0_RNCR                  (0x400A0114U) /**< \brief (USART0) Receive Next Counter Register */
  #define REG_USART0_TNPR                  (0x400A0118U) /**< \brief (USART0) Transmit Next Pointer Register */
  #define REG_USART0_TNCR                  (0x400A011CU) /**< \brief (USART0) Transmit Next Counter Register */
  #define REG_USART0_PTCR                  (0x400A0120U) /**< \brief (USART0) Transfer Control Register */
  #define REG_USART0_PTSR                  (0x400A0124U) /**< \brief (USART0) Transfer Status Register */
#else
  #define REG_USART0_CR   (*(__O  uint32_t*)0x400A0000U) /**< \brief (USART0) Control Register */
  #define REG_USART0_MR   (*(__IO uint32_t*)0x400A0004U) /**< \brief (USART0) Mode Register */
  #define REG_USART0_IER  (*(__O  uint32_t*)0x400A0008U) /**< \brief (USART0) Interrupt Enable Register */
  #define REG_USART0_IDR  (*(__O  uint32_t*)0x400A000CU) /**< \brief (USART0) Interrupt Disable Register */
  #define REG_USART0_IMR  (*(__I  uint32_t*)0x400A0010U) /**< \brief (USART0) Interrupt Mask Register */
  #define REG_USART0_CSR  (*(__I  uint32_t*)0x400A0014U) /**< \brief (USART0) Channel Status Register */
  #define REG_USART0_RHR  (*(__I  uint32_t*)0x400A0018U) /**< \brief (USART0) Receive Holding Register */
  #define REG_USART0_THR  (*(__O  uint32_t*)0x400A001CU) /**< \brief (USART0) Transmit Holding Register */
  #define REG_USART0_BRGR (*(__IO uint32_t*)0x400A0020U) /**< \brief (USART0) Baud Rate Generator Register */
  #define REG_USART0_RTOR (*(__IO uint32_t*)0x400A0024U) /**< \brief (USART0) Receiver Time-out Register */
  #define REG_USART0_TTGR (*(__IO uint32_t*)0x400A0028U) /**< \brief (USART0) Transmitter Timeguard Register */
  #define REG_USART0_FIDI (*(__IO uint32_t*)0x400A0040U) /**< \brief (USART0) FI DI Ratio Register */
  #define REG_USART0_NER  (*(__I  uint32_t*)0x400A0044U) /**< \brief (USART0) Number of Errors Register */
  #define REG_USART0_IF   (*(__IO uint32_t*)0x400A004CU) /**< \brief (USART0) IrDA Filter Register */
  #define REG_USART0_MAN  (*(__IO uint32_t*)0x400A0050U) /**< \brief (USART0) Manchester Configuration Register */
  #define REG_USART0_WPMR (*(__IO uint32_t*)0x400A00E4U) /**< \brief (USART0) Write Protection Mode Register */
  #define REG_USART0_WPSR (*(__I  uint32_t*)0x400A00E8U) /**< \brief (USART0) Write Protection Status Register */
  #define REG_USART0_RPR  (*(__IO uint32_t*)0x400A0100U) /**< \brief (USART0) Receive Pointer Register */
  #define REG_USART0_RCR  (*(__IO uint32_t*)0x400A0104U) /**< \brief (USART0) Receive Counter Register */
  #define REG_USART0_TPR  (*(__IO uint32_t*)0x400A0108U) /**< \brief (USART0) Transmit Pointer Register */
  #define REG_USART0_TCR  (*(__IO uint32_t*)0x400A010CU) /**< \brief (USART0) Transmit Counter Register */
  #define REG_USART0_RNPR (*(__IO uint32_t*)0x400A0110U) /**< \brief (USART0) Receive Next Pointer Register */
  #define REG_USART0_RNCR (*(__IO uint32_t*)0x400A0114U) /**< \brief (USART0) Receive Next Counter Register */
  #define REG_USART0_TNPR (*(__IO uint32_t*)0x400A0118U) /**< \brief (USART0) Transmit Next Pointer Register */
  #define REG_USART0_TNCR (*(__IO uint32_t*)0x400A011CU) /**< \brief (USART0) Transmit Next Counter Register */
  #define REG_USART0_PTCR (*(__O  uint32_t*)0x400A0120U) /**< \brief (USART0) Transfer Control Register */
  #define REG_USART0_PTSR (*(__I  uint32_t*)0x400A0124U) /**< \brief (USART0) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAM4E_USART0_INSTANCE_ */
