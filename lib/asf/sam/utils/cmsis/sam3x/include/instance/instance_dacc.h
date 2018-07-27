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

#ifndef _SAM3XA_DACC_INSTANCE_
#define _SAM3XA_DACC_INSTANCE_

/* ========== Register definition for DACC peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_DACC_CR            (0x400C8000U) /**< \brief (DACC) Control Register */
#define REG_DACC_MR            (0x400C8004U) /**< \brief (DACC) Mode Register */
#define REG_DACC_CHER          (0x400C8010U) /**< \brief (DACC) Channel Enable Register */
#define REG_DACC_CHDR          (0x400C8014U) /**< \brief (DACC) Channel Disable Register */
#define REG_DACC_CHSR          (0x400C8018U) /**< \brief (DACC) Channel Status Register */
#define REG_DACC_CDR           (0x400C8020U) /**< \brief (DACC) Conversion Data Register */
#define REG_DACC_IER           (0x400C8024U) /**< \brief (DACC) Interrupt Enable Register */
#define REG_DACC_IDR           (0x400C8028U) /**< \brief (DACC) Interrupt Disable Register */
#define REG_DACC_IMR           (0x400C802CU) /**< \brief (DACC) Interrupt Mask Register */
#define REG_DACC_ISR           (0x400C8030U) /**< \brief (DACC) Interrupt Status Register */
#define REG_DACC_ACR           (0x400C8094U) /**< \brief (DACC) Analog Current Register */
#define REG_DACC_WPMR          (0x400C80E4U) /**< \brief (DACC) Write Protect Mode register */
#define REG_DACC_WPSR          (0x400C80E8U) /**< \brief (DACC) Write Protect Status register */
#define REG_DACC_TPR           (0x400C8108U) /**< \brief (DACC) Transmit Pointer Register */
#define REG_DACC_TCR           (0x400C810CU) /**< \brief (DACC) Transmit Counter Register */
#define REG_DACC_TNPR          (0x400C8118U) /**< \brief (DACC) Transmit Next Pointer Register */
#define REG_DACC_TNCR          (0x400C811CU) /**< \brief (DACC) Transmit Next Counter Register */
#define REG_DACC_PTCR          (0x400C8120U) /**< \brief (DACC) Transfer Control Register */
#define REG_DACC_PTSR          (0x400C8124U) /**< \brief (DACC) Transfer Status Register */
#else
#define REG_DACC_CR   (*(WoReg*)0x400C8000U) /**< \brief (DACC) Control Register */
#define REG_DACC_MR   (*(RwReg*)0x400C8004U) /**< \brief (DACC) Mode Register */
#define REG_DACC_CHER (*(WoReg*)0x400C8010U) /**< \brief (DACC) Channel Enable Register */
#define REG_DACC_CHDR (*(WoReg*)0x400C8014U) /**< \brief (DACC) Channel Disable Register */
#define REG_DACC_CHSR (*(RoReg*)0x400C8018U) /**< \brief (DACC) Channel Status Register */
#define REG_DACC_CDR  (*(WoReg*)0x400C8020U) /**< \brief (DACC) Conversion Data Register */
#define REG_DACC_IER  (*(WoReg*)0x400C8024U) /**< \brief (DACC) Interrupt Enable Register */
#define REG_DACC_IDR  (*(WoReg*)0x400C8028U) /**< \brief (DACC) Interrupt Disable Register */
#define REG_DACC_IMR  (*(RoReg*)0x400C802CU) /**< \brief (DACC) Interrupt Mask Register */
#define REG_DACC_ISR  (*(RoReg*)0x400C8030U) /**< \brief (DACC) Interrupt Status Register */
#define REG_DACC_ACR  (*(RwReg*)0x400C8094U) /**< \brief (DACC) Analog Current Register */
#define REG_DACC_WPMR (*(RwReg*)0x400C80E4U) /**< \brief (DACC) Write Protect Mode register */
#define REG_DACC_WPSR (*(RoReg*)0x400C80E8U) /**< \brief (DACC) Write Protect Status register */
#define REG_DACC_TPR  (*(RwReg*)0x400C8108U) /**< \brief (DACC) Transmit Pointer Register */
#define REG_DACC_TCR  (*(RwReg*)0x400C810CU) /**< \brief (DACC) Transmit Counter Register */
#define REG_DACC_TNPR (*(RwReg*)0x400C8118U) /**< \brief (DACC) Transmit Next Pointer Register */
#define REG_DACC_TNCR (*(RwReg*)0x400C811CU) /**< \brief (DACC) Transmit Next Counter Register */
#define REG_DACC_PTCR (*(WoReg*)0x400C8120U) /**< \brief (DACC) Transfer Control Register */
#define REG_DACC_PTSR (*(RoReg*)0x400C8124U) /**< \brief (DACC) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAM3XA_DACC_INSTANCE_ */
