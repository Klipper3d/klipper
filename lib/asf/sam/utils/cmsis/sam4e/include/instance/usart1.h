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

#ifndef _SAM4E_USART1_INSTANCE_
#define _SAM4E_USART1_INSTANCE_

/* ========== Register definition for USART1 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_USART1_CR                    (0x400A4000U) /**< \brief (USART1) Control Register */
  #define REG_USART1_MR                    (0x400A4004U) /**< \brief (USART1) Mode Register */
  #define REG_USART1_IER                   (0x400A4008U) /**< \brief (USART1) Interrupt Enable Register */
  #define REG_USART1_IDR                   (0x400A400CU) /**< \brief (USART1) Interrupt Disable Register */
  #define REG_USART1_IMR                   (0x400A4010U) /**< \brief (USART1) Interrupt Mask Register */
  #define REG_USART1_CSR                   (0x400A4014U) /**< \brief (USART1) Channel Status Register */
  #define REG_USART1_RHR                   (0x400A4018U) /**< \brief (USART1) Receive Holding Register */
  #define REG_USART1_THR                   (0x400A401CU) /**< \brief (USART1) Transmit Holding Register */
  #define REG_USART1_BRGR                  (0x400A4020U) /**< \brief (USART1) Baud Rate Generator Register */
  #define REG_USART1_RTOR                  (0x400A4024U) /**< \brief (USART1) Receiver Time-out Register */
  #define REG_USART1_TTGR                  (0x400A4028U) /**< \brief (USART1) Transmitter Timeguard Register */
  #define REG_USART1_FIDI                  (0x400A4040U) /**< \brief (USART1) FI DI Ratio Register */
  #define REG_USART1_NER                   (0x400A4044U) /**< \brief (USART1) Number of Errors Register */
  #define REG_USART1_IF                    (0x400A404CU) /**< \brief (USART1) IrDA Filter Register */
  #define REG_USART1_MAN                   (0x400A4050U) /**< \brief (USART1) Manchester Configuration Register */
  #define REG_USART1_WPMR                  (0x400A40E4U) /**< \brief (USART1) Write Protection Mode Register */
  #define REG_USART1_WPSR                  (0x400A40E8U) /**< \brief (USART1) Write Protection Status Register */
  #define REG_USART1_RPR                   (0x400A4100U) /**< \brief (USART1) Receive Pointer Register */
  #define REG_USART1_RCR                   (0x400A4104U) /**< \brief (USART1) Receive Counter Register */
  #define REG_USART1_TPR                   (0x400A4108U) /**< \brief (USART1) Transmit Pointer Register */
  #define REG_USART1_TCR                   (0x400A410CU) /**< \brief (USART1) Transmit Counter Register */
  #define REG_USART1_RNPR                  (0x400A4110U) /**< \brief (USART1) Receive Next Pointer Register */
  #define REG_USART1_RNCR                  (0x400A4114U) /**< \brief (USART1) Receive Next Counter Register */
  #define REG_USART1_TNPR                  (0x400A4118U) /**< \brief (USART1) Transmit Next Pointer Register */
  #define REG_USART1_TNCR                  (0x400A411CU) /**< \brief (USART1) Transmit Next Counter Register */
  #define REG_USART1_PTCR                  (0x400A4120U) /**< \brief (USART1) Transfer Control Register */
  #define REG_USART1_PTSR                  (0x400A4124U) /**< \brief (USART1) Transfer Status Register */
#else
  #define REG_USART1_CR   (*(__O  uint32_t*)0x400A4000U) /**< \brief (USART1) Control Register */
  #define REG_USART1_MR   (*(__IO uint32_t*)0x400A4004U) /**< \brief (USART1) Mode Register */
  #define REG_USART1_IER  (*(__O  uint32_t*)0x400A4008U) /**< \brief (USART1) Interrupt Enable Register */
  #define REG_USART1_IDR  (*(__O  uint32_t*)0x400A400CU) /**< \brief (USART1) Interrupt Disable Register */
  #define REG_USART1_IMR  (*(__I  uint32_t*)0x400A4010U) /**< \brief (USART1) Interrupt Mask Register */
  #define REG_USART1_CSR  (*(__I  uint32_t*)0x400A4014U) /**< \brief (USART1) Channel Status Register */
  #define REG_USART1_RHR  (*(__I  uint32_t*)0x400A4018U) /**< \brief (USART1) Receive Holding Register */
  #define REG_USART1_THR  (*(__O  uint32_t*)0x400A401CU) /**< \brief (USART1) Transmit Holding Register */
  #define REG_USART1_BRGR (*(__IO uint32_t*)0x400A4020U) /**< \brief (USART1) Baud Rate Generator Register */
  #define REG_USART1_RTOR (*(__IO uint32_t*)0x400A4024U) /**< \brief (USART1) Receiver Time-out Register */
  #define REG_USART1_TTGR (*(__IO uint32_t*)0x400A4028U) /**< \brief (USART1) Transmitter Timeguard Register */
  #define REG_USART1_FIDI (*(__IO uint32_t*)0x400A4040U) /**< \brief (USART1) FI DI Ratio Register */
  #define REG_USART1_NER  (*(__I  uint32_t*)0x400A4044U) /**< \brief (USART1) Number of Errors Register */
  #define REG_USART1_IF   (*(__IO uint32_t*)0x400A404CU) /**< \brief (USART1) IrDA Filter Register */
  #define REG_USART1_MAN  (*(__IO uint32_t*)0x400A4050U) /**< \brief (USART1) Manchester Configuration Register */
  #define REG_USART1_WPMR (*(__IO uint32_t*)0x400A40E4U) /**< \brief (USART1) Write Protection Mode Register */
  #define REG_USART1_WPSR (*(__I  uint32_t*)0x400A40E8U) /**< \brief (USART1) Write Protection Status Register */
  #define REG_USART1_RPR  (*(__IO uint32_t*)0x400A4100U) /**< \brief (USART1) Receive Pointer Register */
  #define REG_USART1_RCR  (*(__IO uint32_t*)0x400A4104U) /**< \brief (USART1) Receive Counter Register */
  #define REG_USART1_TPR  (*(__IO uint32_t*)0x400A4108U) /**< \brief (USART1) Transmit Pointer Register */
  #define REG_USART1_TCR  (*(__IO uint32_t*)0x400A410CU) /**< \brief (USART1) Transmit Counter Register */
  #define REG_USART1_RNPR (*(__IO uint32_t*)0x400A4110U) /**< \brief (USART1) Receive Next Pointer Register */
  #define REG_USART1_RNCR (*(__IO uint32_t*)0x400A4114U) /**< \brief (USART1) Receive Next Counter Register */
  #define REG_USART1_TNPR (*(__IO uint32_t*)0x400A4118U) /**< \brief (USART1) Transmit Next Pointer Register */
  #define REG_USART1_TNCR (*(__IO uint32_t*)0x400A411CU) /**< \brief (USART1) Transmit Next Counter Register */
  #define REG_USART1_PTCR (*(__O  uint32_t*)0x400A4120U) /**< \brief (USART1) Transfer Control Register */
  #define REG_USART1_PTSR (*(__I  uint32_t*)0x400A4124U) /**< \brief (USART1) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAM4E_USART1_INSTANCE_ */
