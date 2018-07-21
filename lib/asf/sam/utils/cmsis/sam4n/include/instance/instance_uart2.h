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

#ifndef _SAM4N_UART2_INSTANCE_
#define _SAM4N_UART2_INSTANCE_

/* ========== Register definition for UART2 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_UART2_CR            (0x40044000U) /**< \brief (UART2) Control Register */
#define REG_UART2_MR            (0x40044004U) /**< \brief (UART2) Mode Register */
#define REG_UART2_IER           (0x40044008U) /**< \brief (UART2) Interrupt Enable Register */
#define REG_UART2_IDR           (0x4004400CU) /**< \brief (UART2) Interrupt Disable Register */
#define REG_UART2_IMR           (0x40044010U) /**< \brief (UART2) Interrupt Mask Register */
#define REG_UART2_SR            (0x40044014U) /**< \brief (UART2) Status Register */
#define REG_UART2_RHR           (0x40044018U) /**< \brief (UART2) Receive Holding Register */
#define REG_UART2_THR           (0x4004401CU) /**< \brief (UART2) Transmit Holding Register */
#define REG_UART2_BRGR          (0x40044020U) /**< \brief (UART2) Baud Rate Generator Register */
#define REG_UART2_RPR           (0x40044100U) /**< \brief (UART2) Receive Pointer Register */
#define REG_UART2_RCR           (0x40044104U) /**< \brief (UART2) Receive Counter Register */
#define REG_UART2_TPR           (0x40044108U) /**< \brief (UART2) Transmit Pointer Register */
#define REG_UART2_TCR           (0x4004410CU) /**< \brief (UART2) Transmit Counter Register */
#define REG_UART2_RNPR          (0x40044110U) /**< \brief (UART2) Receive Next Pointer Register */
#define REG_UART2_RNCR          (0x40044114U) /**< \brief (UART2) Receive Next Counter Register */
#define REG_UART2_TNPR          (0x40044118U) /**< \brief (UART2) Transmit Next Pointer Register */
#define REG_UART2_TNCR          (0x4004411CU) /**< \brief (UART2) Transmit Next Counter Register */
#define REG_UART2_PTCR          (0x40044120U) /**< \brief (UART2) Transfer Control Register */
#define REG_UART2_PTSR          (0x40044124U) /**< \brief (UART2) Transfer Status Register */
#else
#define REG_UART2_CR   (*(WoReg*)0x40044000U) /**< \brief (UART2) Control Register */
#define REG_UART2_MR   (*(RwReg*)0x40044004U) /**< \brief (UART2) Mode Register */
#define REG_UART2_IER  (*(WoReg*)0x40044008U) /**< \brief (UART2) Interrupt Enable Register */
#define REG_UART2_IDR  (*(WoReg*)0x4004400CU) /**< \brief (UART2) Interrupt Disable Register */
#define REG_UART2_IMR  (*(RoReg*)0x40044010U) /**< \brief (UART2) Interrupt Mask Register */
#define REG_UART2_SR   (*(RoReg*)0x40044014U) /**< \brief (UART2) Status Register */
#define REG_UART2_RHR  (*(RoReg*)0x40044018U) /**< \brief (UART2) Receive Holding Register */
#define REG_UART2_THR  (*(WoReg*)0x4004401CU) /**< \brief (UART2) Transmit Holding Register */
#define REG_UART2_BRGR (*(RwReg*)0x40044020U) /**< \brief (UART2) Baud Rate Generator Register */
#define REG_UART2_RPR  (*(RwReg*)0x40044100U) /**< \brief (UART2) Receive Pointer Register */
#define REG_UART2_RCR  (*(RwReg*)0x40044104U) /**< \brief (UART2) Receive Counter Register */
#define REG_UART2_TPR  (*(RwReg*)0x40044108U) /**< \brief (UART2) Transmit Pointer Register */
#define REG_UART2_TCR  (*(RwReg*)0x4004410CU) /**< \brief (UART2) Transmit Counter Register */
#define REG_UART2_RNPR (*(RwReg*)0x40044110U) /**< \brief (UART2) Receive Next Pointer Register */
#define REG_UART2_RNCR (*(RwReg*)0x40044114U) /**< \brief (UART2) Receive Next Counter Register */
#define REG_UART2_TNPR (*(RwReg*)0x40044118U) /**< \brief (UART2) Transmit Next Pointer Register */
#define REG_UART2_TNCR (*(RwReg*)0x4004411CU) /**< \brief (UART2) Transmit Next Counter Register */
#define REG_UART2_PTCR (*(WoReg*)0x40044120U) /**< \brief (UART2) Transfer Control Register */
#define REG_UART2_PTSR (*(RoReg*)0x40044124U) /**< \brief (UART2) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAM4N_UART2_INSTANCE_ */
