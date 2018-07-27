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

#ifndef _SAM4E_UART0_INSTANCE_
#define _SAM4E_UART0_INSTANCE_

/* ========== Register definition for UART0 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_UART0_CR                    (0x400E0600U) /**< \brief (UART0) Control Register */
  #define REG_UART0_MR                    (0x400E0604U) /**< \brief (UART0) Mode Register */
  #define REG_UART0_IER                   (0x400E0608U) /**< \brief (UART0) Interrupt Enable Register */
  #define REG_UART0_IDR                   (0x400E060CU) /**< \brief (UART0) Interrupt Disable Register */
  #define REG_UART0_IMR                   (0x400E0610U) /**< \brief (UART0) Interrupt Mask Register */
  #define REG_UART0_SR                    (0x400E0614U) /**< \brief (UART0) Status Register */
  #define REG_UART0_RHR                   (0x400E0618U) /**< \brief (UART0) Receive Holding Register */
  #define REG_UART0_THR                   (0x400E061CU) /**< \brief (UART0) Transmit Holding Register */
  #define REG_UART0_BRGR                  (0x400E0620U) /**< \brief (UART0) Baud Rate Generator Register */
  #define REG_UART0_WPMR                  (0x400E06E4U) /**< \brief (UART0) Write Protection Mode Register */
  #define REG_UART0_RPR                   (0x400E0700U) /**< \brief (UART0) Receive Pointer Register */
  #define REG_UART0_RCR                   (0x400E0704U) /**< \brief (UART0) Receive Counter Register */
  #define REG_UART0_TPR                   (0x400E0708U) /**< \brief (UART0) Transmit Pointer Register */
  #define REG_UART0_TCR                   (0x400E070CU) /**< \brief (UART0) Transmit Counter Register */
  #define REG_UART0_RNPR                  (0x400E0710U) /**< \brief (UART0) Receive Next Pointer Register */
  #define REG_UART0_RNCR                  (0x400E0714U) /**< \brief (UART0) Receive Next Counter Register */
  #define REG_UART0_TNPR                  (0x400E0718U) /**< \brief (UART0) Transmit Next Pointer Register */
  #define REG_UART0_TNCR                  (0x400E071CU) /**< \brief (UART0) Transmit Next Counter Register */
  #define REG_UART0_PTCR                  (0x400E0720U) /**< \brief (UART0) Transfer Control Register */
  #define REG_UART0_PTSR                  (0x400E0724U) /**< \brief (UART0) Transfer Status Register */
#else
  #define REG_UART0_CR   (*(__O  uint32_t*)0x400E0600U) /**< \brief (UART0) Control Register */
  #define REG_UART0_MR   (*(__IO uint32_t*)0x400E0604U) /**< \brief (UART0) Mode Register */
  #define REG_UART0_IER  (*(__O  uint32_t*)0x400E0608U) /**< \brief (UART0) Interrupt Enable Register */
  #define REG_UART0_IDR  (*(__O  uint32_t*)0x400E060CU) /**< \brief (UART0) Interrupt Disable Register */
  #define REG_UART0_IMR  (*(__I  uint32_t*)0x400E0610U) /**< \brief (UART0) Interrupt Mask Register */
  #define REG_UART0_SR   (*(__I  uint32_t*)0x400E0614U) /**< \brief (UART0) Status Register */
  #define REG_UART0_RHR  (*(__I  uint32_t*)0x400E0618U) /**< \brief (UART0) Receive Holding Register */
  #define REG_UART0_THR  (*(__O  uint32_t*)0x400E061CU) /**< \brief (UART0) Transmit Holding Register */
  #define REG_UART0_BRGR (*(__IO uint32_t*)0x400E0620U) /**< \brief (UART0) Baud Rate Generator Register */
  #define REG_UART0_WPMR (*(__IO uint32_t*)0x400E06E4U) /**< \brief (UART0) Write Protection Mode Register */
  #define REG_UART0_RPR  (*(__IO uint32_t*)0x400E0700U) /**< \brief (UART0) Receive Pointer Register */
  #define REG_UART0_RCR  (*(__IO uint32_t*)0x400E0704U) /**< \brief (UART0) Receive Counter Register */
  #define REG_UART0_TPR  (*(__IO uint32_t*)0x400E0708U) /**< \brief (UART0) Transmit Pointer Register */
  #define REG_UART0_TCR  (*(__IO uint32_t*)0x400E070CU) /**< \brief (UART0) Transmit Counter Register */
  #define REG_UART0_RNPR (*(__IO uint32_t*)0x400E0710U) /**< \brief (UART0) Receive Next Pointer Register */
  #define REG_UART0_RNCR (*(__IO uint32_t*)0x400E0714U) /**< \brief (UART0) Receive Next Counter Register */
  #define REG_UART0_TNPR (*(__IO uint32_t*)0x400E0718U) /**< \brief (UART0) Transmit Next Pointer Register */
  #define REG_UART0_TNCR (*(__IO uint32_t*)0x400E071CU) /**< \brief (UART0) Transmit Next Counter Register */
  #define REG_UART0_PTCR (*(__O  uint32_t*)0x400E0720U) /**< \brief (UART0) Transfer Control Register */
  #define REG_UART0_PTSR (*(__I  uint32_t*)0x400E0724U) /**< \brief (UART0) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAM4E_UART0_INSTANCE_ */
