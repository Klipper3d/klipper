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

#ifndef _SAM4E_HSMCI_INSTANCE_
#define _SAM4E_HSMCI_INSTANCE_

/* ========== Register definition for HSMCI peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_HSMCI_CR                     (0x40080000U) /**< \brief (HSMCI) Control Register */
  #define REG_HSMCI_MR                     (0x40080004U) /**< \brief (HSMCI) Mode Register */
  #define REG_HSMCI_DTOR                   (0x40080008U) /**< \brief (HSMCI) Data Timeout Register */
  #define REG_HSMCI_SDCR                   (0x4008000CU) /**< \brief (HSMCI) SD/SDIO Card Register */
  #define REG_HSMCI_ARGR                   (0x40080010U) /**< \brief (HSMCI) Argument Register */
  #define REG_HSMCI_CMDR                   (0x40080014U) /**< \brief (HSMCI) Command Register */
  #define REG_HSMCI_BLKR                   (0x40080018U) /**< \brief (HSMCI) Block Register */
  #define REG_HSMCI_CSTOR                  (0x4008001CU) /**< \brief (HSMCI) Completion Signal Timeout Register */
  #define REG_HSMCI_RSPR                   (0x40080020U) /**< \brief (HSMCI) Response Register */
  #define REG_HSMCI_RDR                    (0x40080030U) /**< \brief (HSMCI) Receive Data Register */
  #define REG_HSMCI_TDR                    (0x40080034U) /**< \brief (HSMCI) Transmit Data Register */
  #define REG_HSMCI_SR                     (0x40080040U) /**< \brief (HSMCI) Status Register */
  #define REG_HSMCI_IER                    (0x40080044U) /**< \brief (HSMCI) Interrupt Enable Register */
  #define REG_HSMCI_IDR                    (0x40080048U) /**< \brief (HSMCI) Interrupt Disable Register */
  #define REG_HSMCI_IMR                    (0x4008004CU) /**< \brief (HSMCI) Interrupt Mask Register */
  #define REG_HSMCI_CFG                    (0x40080054U) /**< \brief (HSMCI) Configuration Register */
  #define REG_HSMCI_WPMR                   (0x400800E4U) /**< \brief (HSMCI) Write Protection Mode Register */
  #define REG_HSMCI_WPSR                   (0x400800E8U) /**< \brief (HSMCI) Write Protection Status Register */
  #define REG_HSMCI_RPR                    (0x40080100U) /**< \brief (HSMCI) Receive Pointer Register */
  #define REG_HSMCI_RCR                    (0x40080104U) /**< \brief (HSMCI) Receive Counter Register */
  #define REG_HSMCI_TPR                    (0x40080108U) /**< \brief (HSMCI) Transmit Pointer Register */
  #define REG_HSMCI_TCR                    (0x4008010CU) /**< \brief (HSMCI) Transmit Counter Register */
  #define REG_HSMCI_RNPR                   (0x40080110U) /**< \brief (HSMCI) Receive Next Pointer Register */
  #define REG_HSMCI_RNCR                   (0x40080114U) /**< \brief (HSMCI) Receive Next Counter Register */
  #define REG_HSMCI_TNPR                   (0x40080118U) /**< \brief (HSMCI) Transmit Next Pointer Register */
  #define REG_HSMCI_TNCR                   (0x4008011CU) /**< \brief (HSMCI) Transmit Next Counter Register */
  #define REG_HSMCI_PTCR                   (0x40080120U) /**< \brief (HSMCI) Transfer Control Register */
  #define REG_HSMCI_PTSR                   (0x40080124U) /**< \brief (HSMCI) Transfer Status Register */
  #define REG_HSMCI_FIFO                   (0x40080200U) /**< \brief (HSMCI) FIFO Memory Aperture0 */
#else
  #define REG_HSMCI_CR    (*(__O  uint32_t*)0x40080000U) /**< \brief (HSMCI) Control Register */
  #define REG_HSMCI_MR    (*(__IO uint32_t*)0x40080004U) /**< \brief (HSMCI) Mode Register */
  #define REG_HSMCI_DTOR  (*(__IO uint32_t*)0x40080008U) /**< \brief (HSMCI) Data Timeout Register */
  #define REG_HSMCI_SDCR  (*(__IO uint32_t*)0x4008000CU) /**< \brief (HSMCI) SD/SDIO Card Register */
  #define REG_HSMCI_ARGR  (*(__IO uint32_t*)0x40080010U) /**< \brief (HSMCI) Argument Register */
  #define REG_HSMCI_CMDR  (*(__O  uint32_t*)0x40080014U) /**< \brief (HSMCI) Command Register */
  #define REG_HSMCI_BLKR  (*(__IO uint32_t*)0x40080018U) /**< \brief (HSMCI) Block Register */
  #define REG_HSMCI_CSTOR (*(__IO uint32_t*)0x4008001CU) /**< \brief (HSMCI) Completion Signal Timeout Register */
  #define REG_HSMCI_RSPR  (*(__I  uint32_t*)0x40080020U) /**< \brief (HSMCI) Response Register */
  #define REG_HSMCI_RDR   (*(__I  uint32_t*)0x40080030U) /**< \brief (HSMCI) Receive Data Register */
  #define REG_HSMCI_TDR   (*(__O  uint32_t*)0x40080034U) /**< \brief (HSMCI) Transmit Data Register */
  #define REG_HSMCI_SR    (*(__I  uint32_t*)0x40080040U) /**< \brief (HSMCI) Status Register */
  #define REG_HSMCI_IER   (*(__O  uint32_t*)0x40080044U) /**< \brief (HSMCI) Interrupt Enable Register */
  #define REG_HSMCI_IDR   (*(__O  uint32_t*)0x40080048U) /**< \brief (HSMCI) Interrupt Disable Register */
  #define REG_HSMCI_IMR   (*(__I  uint32_t*)0x4008004CU) /**< \brief (HSMCI) Interrupt Mask Register */
  #define REG_HSMCI_CFG   (*(__IO uint32_t*)0x40080054U) /**< \brief (HSMCI) Configuration Register */
  #define REG_HSMCI_WPMR  (*(__IO uint32_t*)0x400800E4U) /**< \brief (HSMCI) Write Protection Mode Register */
  #define REG_HSMCI_WPSR  (*(__I  uint32_t*)0x400800E8U) /**< \brief (HSMCI) Write Protection Status Register */
  #define REG_HSMCI_RPR   (*(__IO uint32_t*)0x40080100U) /**< \brief (HSMCI) Receive Pointer Register */
  #define REG_HSMCI_RCR   (*(__IO uint32_t*)0x40080104U) /**< \brief (HSMCI) Receive Counter Register */
  #define REG_HSMCI_TPR   (*(__IO uint32_t*)0x40080108U) /**< \brief (HSMCI) Transmit Pointer Register */
  #define REG_HSMCI_TCR   (*(__IO uint32_t*)0x4008010CU) /**< \brief (HSMCI) Transmit Counter Register */
  #define REG_HSMCI_RNPR  (*(__IO uint32_t*)0x40080110U) /**< \brief (HSMCI) Receive Next Pointer Register */
  #define REG_HSMCI_RNCR  (*(__IO uint32_t*)0x40080114U) /**< \brief (HSMCI) Receive Next Counter Register */
  #define REG_HSMCI_TNPR  (*(__IO uint32_t*)0x40080118U) /**< \brief (HSMCI) Transmit Next Pointer Register */
  #define REG_HSMCI_TNCR  (*(__IO uint32_t*)0x4008011CU) /**< \brief (HSMCI) Transmit Next Counter Register */
  #define REG_HSMCI_PTCR  (*(__O  uint32_t*)0x40080120U) /**< \brief (HSMCI) Transfer Control Register */
  #define REG_HSMCI_PTSR  (*(__I  uint32_t*)0x40080124U) /**< \brief (HSMCI) Transfer Status Register */
  #define REG_HSMCI_FIFO  (*(__IO uint32_t*)0x40080200U) /**< \brief (HSMCI) FIFO Memory Aperture0 */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAM4E_HSMCI_INSTANCE_ */
