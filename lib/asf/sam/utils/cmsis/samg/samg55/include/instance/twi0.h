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

#ifndef _SAMG55_TWI0_INSTANCE_
#define _SAMG55_TWI0_INSTANCE_

/* ========== Register definition for TWI0 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_TWI0_CR                     (0x4000C600U) /**< \brief (TWI0) TWI Control Register */
  #define REG_TWI0_MMR                    (0x4000C604U) /**< \brief (TWI0) TWI Master Mode Register */
  #define REG_TWI0_SMR                    (0x4000C608U) /**< \brief (TWI0) TWI Slave Mode Register */
  #define REG_TWI0_IADR                   (0x4000C60CU) /**< \brief (TWI0) TWI Internal Address Register */
  #define REG_TWI0_CWGR                   (0x4000C610U) /**< \brief (TWI0) TWI Clock Waveform Generator Register */
  #define REG_TWI0_SR                     (0x4000C620U) /**< \brief (TWI0) TWI Status Register */
  #define REG_TWI0_IER                    (0x4000C624U) /**< \brief (TWI0) TWI Interrupt Enable Register */
  #define REG_TWI0_IDR                    (0x4000C628U) /**< \brief (TWI0) TWI Interrupt Disable Register */
  #define REG_TWI0_IMR                    (0x4000C62CU) /**< \brief (TWI0) TWI Interrupt Mask Register */
  #define REG_TWI0_RHR                    (0x4000C630U) /**< \brief (TWI0) TWI Receive Holding Register */
  #define REG_TWI0_THR                    (0x4000C634U) /**< \brief (TWI0) TWI Transmit Holding Register */
  #define REG_TWI0_SMBTR                  (0x4000C638U) /**< \brief (TWI0) TWI SMBus Timing Register */
  #define REG_TWI0_ACR                    (0x4000C640U) /**< \brief (TWI0) TWI Alternative Command Register */
  #define REG_TWI0_FILTR                  (0x4000C644U) /**< \brief (TWI0) TWI Filter Register */
  #define REG_TWI0_SWMR                   (0x4000C64CU) /**< \brief (TWI0) TWI SleepWalking Matching Register */
  #define REG_TWI0_WPMR                   (0x4000C6E4U) /**< \brief (TWI0) TWI Write Protection Mode Register */
  #define REG_TWI0_WPSR                   (0x4000C6E8U) /**< \brief (TWI0) TWI Write Protection Status Register */
  #define REG_TWI0_RPR                    (0x4000C700U) /**< \brief (TWI0) Receive Pointer Register */
  #define REG_TWI0_RCR                    (0x4000C704U) /**< \brief (TWI0) Receive Counter Register */
  #define REG_TWI0_TPR                    (0x4000C708U) /**< \brief (TWI0) Transmit Pointer Register */
  #define REG_TWI0_TCR                    (0x4000C70CU) /**< \brief (TWI0) Transmit Counter Register */
  #define REG_TWI0_RNPR                   (0x4000C710U) /**< \brief (TWI0) Receive Next Pointer Register */
  #define REG_TWI0_RNCR                   (0x4000C714U) /**< \brief (TWI0) Receive Next Counter Register */
  #define REG_TWI0_TNPR                   (0x4000C718U) /**< \brief (TWI0) Transmit Next Pointer Register */
  #define REG_TWI0_TNCR                   (0x4000C71CU) /**< \brief (TWI0) Transmit Next Counter Register */
  #define REG_TWI0_PTCR                   (0x4000C720U) /**< \brief (TWI0) Transfer Control Register */
  #define REG_TWI0_PTSR                   (0x4000C724U) /**< \brief (TWI0) Transfer Status Register */
#else
  #define REG_TWI0_CR    (*(__O  uint32_t*)0x4000C600U) /**< \brief (TWI0) TWI Control Register */
  #define REG_TWI0_MMR   (*(__IO uint32_t*)0x4000C604U) /**< \brief (TWI0) TWI Master Mode Register */
  #define REG_TWI0_SMR   (*(__IO uint32_t*)0x4000C608U) /**< \brief (TWI0) TWI Slave Mode Register */
  #define REG_TWI0_IADR  (*(__IO uint32_t*)0x4000C60CU) /**< \brief (TWI0) TWI Internal Address Register */
  #define REG_TWI0_CWGR  (*(__IO uint32_t*)0x4000C610U) /**< \brief (TWI0) TWI Clock Waveform Generator Register */
  #define REG_TWI0_SR    (*(__I  uint32_t*)0x4000C620U) /**< \brief (TWI0) TWI Status Register */
  #define REG_TWI0_IER   (*(__O  uint32_t*)0x4000C624U) /**< \brief (TWI0) TWI Interrupt Enable Register */
  #define REG_TWI0_IDR   (*(__O  uint32_t*)0x4000C628U) /**< \brief (TWI0) TWI Interrupt Disable Register */
  #define REG_TWI0_IMR   (*(__I  uint32_t*)0x4000C62CU) /**< \brief (TWI0) TWI Interrupt Mask Register */
  #define REG_TWI0_RHR   (*(__I  uint32_t*)0x4000C630U) /**< \brief (TWI0) TWI Receive Holding Register */
  #define REG_TWI0_THR   (*(__O  uint32_t*)0x4000C634U) /**< \brief (TWI0) TWI Transmit Holding Register */
  #define REG_TWI0_SMBTR (*(__IO uint32_t*)0x4000C638U) /**< \brief (TWI0) TWI SMBus Timing Register */
  #define REG_TWI0_ACR   (*(__IO uint32_t*)0x4000C640U) /**< \brief (TWI0) TWI Alternative Command Register */
  #define REG_TWI0_FILTR (*(__IO uint32_t*)0x4000C644U) /**< \brief (TWI0) TWI Filter Register */
  #define REG_TWI0_SWMR  (*(__IO uint32_t*)0x4000C64CU) /**< \brief (TWI0) TWI SleepWalking Matching Register */
  #define REG_TWI0_WPMR  (*(__IO uint32_t*)0x4000C6E4U) /**< \brief (TWI0) TWI Write Protection Mode Register */
  #define REG_TWI0_WPSR  (*(__I  uint32_t*)0x4000C6E8U) /**< \brief (TWI0) TWI Write Protection Status Register */
  #define REG_TWI0_RPR   (*(__IO uint32_t*)0x4000C700U) /**< \brief (TWI0) Receive Pointer Register */
  #define REG_TWI0_RCR   (*(__IO uint32_t*)0x4000C704U) /**< \brief (TWI0) Receive Counter Register */
  #define REG_TWI0_TPR   (*(__IO uint32_t*)0x4000C708U) /**< \brief (TWI0) Transmit Pointer Register */
  #define REG_TWI0_TCR   (*(__IO uint32_t*)0x4000C70CU) /**< \brief (TWI0) Transmit Counter Register */
  #define REG_TWI0_RNPR  (*(__IO uint32_t*)0x4000C710U) /**< \brief (TWI0) Receive Next Pointer Register */
  #define REG_TWI0_RNCR  (*(__IO uint32_t*)0x4000C714U) /**< \brief (TWI0) Receive Next Counter Register */
  #define REG_TWI0_TNPR  (*(__IO uint32_t*)0x4000C718U) /**< \brief (TWI0) Transmit Next Pointer Register */
  #define REG_TWI0_TNCR  (*(__IO uint32_t*)0x4000C71CU) /**< \brief (TWI0) Transmit Next Counter Register */
  #define REG_TWI0_PTCR  (*(__O  uint32_t*)0x4000C720U) /**< \brief (TWI0) Transfer Control Register */
  #define REG_TWI0_PTSR  (*(__I  uint32_t*)0x4000C724U) /**< \brief (TWI0) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAMG55_TWI0_INSTANCE_ */
