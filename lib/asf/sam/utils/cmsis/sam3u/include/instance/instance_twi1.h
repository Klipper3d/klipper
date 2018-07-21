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

#ifndef _SAM3U_TWI1_INSTANCE_
#define _SAM3U_TWI1_INSTANCE_

/* ========== Register definition for TWI1 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_TWI1_CR            (0x40088000U) /**< \brief (TWI1) Control Register */
#define REG_TWI1_MMR           (0x40088004U) /**< \brief (TWI1) Master Mode Register */
#define REG_TWI1_SMR           (0x40088008U) /**< \brief (TWI1) Slave Mode Register */
#define REG_TWI1_IADR          (0x4008800CU) /**< \brief (TWI1) Internal Address Register */
#define REG_TWI1_CWGR          (0x40088010U) /**< \brief (TWI1) Clock Waveform Generator Register */
#define REG_TWI1_SR            (0x40088020U) /**< \brief (TWI1) Status Register */
#define REG_TWI1_IER           (0x40088024U) /**< \brief (TWI1) Interrupt Enable Register */
#define REG_TWI1_IDR           (0x40088028U) /**< \brief (TWI1) Interrupt Disable Register */
#define REG_TWI1_IMR           (0x4008802CU) /**< \brief (TWI1) Interrupt Mask Register */
#define REG_TWI1_RHR           (0x40088030U) /**< \brief (TWI1) Receive Holding Register */
#define REG_TWI1_THR           (0x40088034U) /**< \brief (TWI1) Transmit Holding Register */
#define REG_TWI1_RPR           (0x40088100U) /**< \brief (TWI1) Receive Pointer Register */
#define REG_TWI1_RCR           (0x40088104U) /**< \brief (TWI1) Receive Counter Register */
#define REG_TWI1_TPR           (0x40088108U) /**< \brief (TWI1) Transmit Pointer Register */
#define REG_TWI1_TCR           (0x4008810CU) /**< \brief (TWI1) Transmit Counter Register */
#define REG_TWI1_RNPR          (0x40088110U) /**< \brief (TWI1) Receive Next Pointer Register */
#define REG_TWI1_RNCR          (0x40088114U) /**< \brief (TWI1) Receive Next Counter Register */
#define REG_TWI1_TNPR          (0x40088118U) /**< \brief (TWI1) Transmit Next Pointer Register */
#define REG_TWI1_TNCR          (0x4008811CU) /**< \brief (TWI1) Transmit Next Counter Register */
#define REG_TWI1_PTCR          (0x40088120U) /**< \brief (TWI1) Transfer Control Register */
#define REG_TWI1_PTSR          (0x40088124U) /**< \brief (TWI1) Transfer Status Register */
#else
#define REG_TWI1_CR   (*(WoReg*)0x40088000U) /**< \brief (TWI1) Control Register */
#define REG_TWI1_MMR  (*(RwReg*)0x40088004U) /**< \brief (TWI1) Master Mode Register */
#define REG_TWI1_SMR  (*(RwReg*)0x40088008U) /**< \brief (TWI1) Slave Mode Register */
#define REG_TWI1_IADR (*(RwReg*)0x4008800CU) /**< \brief (TWI1) Internal Address Register */
#define REG_TWI1_CWGR (*(RwReg*)0x40088010U) /**< \brief (TWI1) Clock Waveform Generator Register */
#define REG_TWI1_SR   (*(RoReg*)0x40088020U) /**< \brief (TWI1) Status Register */
#define REG_TWI1_IER  (*(WoReg*)0x40088024U) /**< \brief (TWI1) Interrupt Enable Register */
#define REG_TWI1_IDR  (*(WoReg*)0x40088028U) /**< \brief (TWI1) Interrupt Disable Register */
#define REG_TWI1_IMR  (*(RoReg*)0x4008802CU) /**< \brief (TWI1) Interrupt Mask Register */
#define REG_TWI1_RHR  (*(RoReg*)0x40088030U) /**< \brief (TWI1) Receive Holding Register */
#define REG_TWI1_THR  (*(WoReg*)0x40088034U) /**< \brief (TWI1) Transmit Holding Register */
#define REG_TWI1_RPR  (*(RwReg*)0x40088100U) /**< \brief (TWI1) Receive Pointer Register */
#define REG_TWI1_RCR  (*(RwReg*)0x40088104U) /**< \brief (TWI1) Receive Counter Register */
#define REG_TWI1_TPR  (*(RwReg*)0x40088108U) /**< \brief (TWI1) Transmit Pointer Register */
#define REG_TWI1_TCR  (*(RwReg*)0x4008810CU) /**< \brief (TWI1) Transmit Counter Register */
#define REG_TWI1_RNPR (*(RwReg*)0x40088110U) /**< \brief (TWI1) Receive Next Pointer Register */
#define REG_TWI1_RNCR (*(RwReg*)0x40088114U) /**< \brief (TWI1) Receive Next Counter Register */
#define REG_TWI1_TNPR (*(RwReg*)0x40088118U) /**< \brief (TWI1) Transmit Next Pointer Register */
#define REG_TWI1_TNCR (*(RwReg*)0x4008811CU) /**< \brief (TWI1) Transmit Next Counter Register */
#define REG_TWI1_PTCR (*(WoReg*)0x40088120U) /**< \brief (TWI1) Transfer Control Register */
#define REG_TWI1_PTSR (*(RoReg*)0x40088124U) /**< \brief (TWI1) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAM3U_TWI1_INSTANCE_ */
