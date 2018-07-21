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

#ifndef _SAMG55_TWI6_INSTANCE_
#define _SAMG55_TWI6_INSTANCE_

/* ========== Register definition for TWI6 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_TWI6_CR                     (0x40040600U) /**< \brief (TWI6) TWI Control Register */
  #define REG_TWI6_MMR                    (0x40040604U) /**< \brief (TWI6) TWI Master Mode Register */
  #define REG_TWI6_SMR                    (0x40040608U) /**< \brief (TWI6) TWI Slave Mode Register */
  #define REG_TWI6_IADR                   (0x4004060CU) /**< \brief (TWI6) TWI Internal Address Register */
  #define REG_TWI6_CWGR                   (0x40040610U) /**< \brief (TWI6) TWI Clock Waveform Generator Register */
  #define REG_TWI6_SR                     (0x40040620U) /**< \brief (TWI6) TWI Status Register */
  #define REG_TWI6_IER                    (0x40040624U) /**< \brief (TWI6) TWI Interrupt Enable Register */
  #define REG_TWI6_IDR                    (0x40040628U) /**< \brief (TWI6) TWI Interrupt Disable Register */
  #define REG_TWI6_IMR                    (0x4004062CU) /**< \brief (TWI6) TWI Interrupt Mask Register */
  #define REG_TWI6_RHR                    (0x40040630U) /**< \brief (TWI6) TWI Receive Holding Register */
  #define REG_TWI6_THR                    (0x40040634U) /**< \brief (TWI6) TWI Transmit Holding Register */
  #define REG_TWI6_SMBTR                  (0x40040638U) /**< \brief (TWI6) TWI SMBus Timing Register */
  #define REG_TWI6_ACR                    (0x40040640U) /**< \brief (TWI6) TWI Alternative Command Register */
  #define REG_TWI6_FILTR                  (0x40040644U) /**< \brief (TWI6) TWI Filter Register */
  #define REG_TWI6_SWMR                   (0x4004064CU) /**< \brief (TWI6) TWI SleepWalking Matching Register */
  #define REG_TWI6_WPMR                   (0x400406E4U) /**< \brief (TWI6) TWI Write Protection Mode Register */
  #define REG_TWI6_WPSR                   (0x400406E8U) /**< \brief (TWI6) TWI Write Protection Status Register */
  #define REG_TWI6_RPR                    (0x40040700U) /**< \brief (TWI6) Receive Pointer Register */
  #define REG_TWI6_RCR                    (0x40040704U) /**< \brief (TWI6) Receive Counter Register */
  #define REG_TWI6_TPR                    (0x40040708U) /**< \brief (TWI6) Transmit Pointer Register */
  #define REG_TWI6_TCR                    (0x4004070CU) /**< \brief (TWI6) Transmit Counter Register */
  #define REG_TWI6_RNPR                   (0x40040710U) /**< \brief (TWI6) Receive Next Pointer Register */
  #define REG_TWI6_RNCR                   (0x40040714U) /**< \brief (TWI6) Receive Next Counter Register */
  #define REG_TWI6_TNPR                   (0x40040718U) /**< \brief (TWI6) Transmit Next Pointer Register */
  #define REG_TWI6_TNCR                   (0x4004071CU) /**< \brief (TWI6) Transmit Next Counter Register */
  #define REG_TWI6_PTCR                   (0x40040720U) /**< \brief (TWI6) Transfer Control Register */
  #define REG_TWI6_PTSR                   (0x40040724U) /**< \brief (TWI6) Transfer Status Register */
#else
  #define REG_TWI6_CR    (*(__O  uint32_t*)0x40040600U) /**< \brief (TWI6) TWI Control Register */
  #define REG_TWI6_MMR   (*(__IO uint32_t*)0x40040604U) /**< \brief (TWI6) TWI Master Mode Register */
  #define REG_TWI6_SMR   (*(__IO uint32_t*)0x40040608U) /**< \brief (TWI6) TWI Slave Mode Register */
  #define REG_TWI6_IADR  (*(__IO uint32_t*)0x4004060CU) /**< \brief (TWI6) TWI Internal Address Register */
  #define REG_TWI6_CWGR  (*(__IO uint32_t*)0x40040610U) /**< \brief (TWI6) TWI Clock Waveform Generator Register */
  #define REG_TWI6_SR    (*(__I  uint32_t*)0x40040620U) /**< \brief (TWI6) TWI Status Register */
  #define REG_TWI6_IER   (*(__O  uint32_t*)0x40040624U) /**< \brief (TWI6) TWI Interrupt Enable Register */
  #define REG_TWI6_IDR   (*(__O  uint32_t*)0x40040628U) /**< \brief (TWI6) TWI Interrupt Disable Register */
  #define REG_TWI6_IMR   (*(__I  uint32_t*)0x4004062CU) /**< \brief (TWI6) TWI Interrupt Mask Register */
  #define REG_TWI6_RHR   (*(__I  uint32_t*)0x40040630U) /**< \brief (TWI6) TWI Receive Holding Register */
  #define REG_TWI6_THR   (*(__O  uint32_t*)0x40040634U) /**< \brief (TWI6) TWI Transmit Holding Register */
  #define REG_TWI6_SMBTR (*(__IO uint32_t*)0x40040638U) /**< \brief (TWI6) TWI SMBus Timing Register */
  #define REG_TWI6_ACR   (*(__IO uint32_t*)0x40040640U) /**< \brief (TWI6) TWI Alternative Command Register */
  #define REG_TWI6_FILTR (*(__IO uint32_t*)0x40040644U) /**< \brief (TWI6) TWI Filter Register */
  #define REG_TWI6_SWMR  (*(__IO uint32_t*)0x4004064CU) /**< \brief (TWI6) TWI SleepWalking Matching Register */
  #define REG_TWI6_WPMR  (*(__IO uint32_t*)0x400406E4U) /**< \brief (TWI6) TWI Write Protection Mode Register */
  #define REG_TWI6_WPSR  (*(__I  uint32_t*)0x400406E8U) /**< \brief (TWI6) TWI Write Protection Status Register */
  #define REG_TWI6_RPR   (*(__IO uint32_t*)0x40040700U) /**< \brief (TWI6) Receive Pointer Register */
  #define REG_TWI6_RCR   (*(__IO uint32_t*)0x40040704U) /**< \brief (TWI6) Receive Counter Register */
  #define REG_TWI6_TPR   (*(__IO uint32_t*)0x40040708U) /**< \brief (TWI6) Transmit Pointer Register */
  #define REG_TWI6_TCR   (*(__IO uint32_t*)0x4004070CU) /**< \brief (TWI6) Transmit Counter Register */
  #define REG_TWI6_RNPR  (*(__IO uint32_t*)0x40040710U) /**< \brief (TWI6) Receive Next Pointer Register */
  #define REG_TWI6_RNCR  (*(__IO uint32_t*)0x40040714U) /**< \brief (TWI6) Receive Next Counter Register */
  #define REG_TWI6_TNPR  (*(__IO uint32_t*)0x40040718U) /**< \brief (TWI6) Transmit Next Pointer Register */
  #define REG_TWI6_TNCR  (*(__IO uint32_t*)0x4004071CU) /**< \brief (TWI6) Transmit Next Counter Register */
  #define REG_TWI6_PTCR  (*(__O  uint32_t*)0x40040720U) /**< \brief (TWI6) Transfer Control Register */
  #define REG_TWI6_PTSR  (*(__I  uint32_t*)0x40040724U) /**< \brief (TWI6) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAMG55_TWI6_INSTANCE_ */
