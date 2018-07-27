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

#ifndef _SAM3XA_SDRAMC_INSTANCE_
#define _SAM3XA_SDRAMC_INSTANCE_

/* ========== Register definition for SDRAMC peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_SDRAMC_MR            (0x400E0200U) /**< \brief (SDRAMC) SDRAMC Mode Register */
#define REG_SDRAMC_TR            (0x400E0204U) /**< \brief (SDRAMC) SDRAMC Refresh Timer Register */
#define REG_SDRAMC_CR            (0x400E0208U) /**< \brief (SDRAMC) SDRAMC Configuration Register */
#define REG_SDRAMC_LPR           (0x400E0210U) /**< \brief (SDRAMC) SDRAMC Low Power Register */
#define REG_SDRAMC_IER           (0x400E0214U) /**< \brief (SDRAMC) SDRAMC Interrupt Enable Register */
#define REG_SDRAMC_IDR           (0x400E0218U) /**< \brief (SDRAMC) SDRAMC Interrupt Disable Register */
#define REG_SDRAMC_IMR           (0x400E021CU) /**< \brief (SDRAMC) SDRAMC Interrupt Mask Register */
#define REG_SDRAMC_ISR           (0x400E0220U) /**< \brief (SDRAMC) SDRAMC Interrupt Status Register */
#define REG_SDRAMC_MDR           (0x400E0224U) /**< \brief (SDRAMC) SDRAMC Memory Device Register */
#define REG_SDRAMC_CR1           (0x400E0228U) /**< \brief (SDRAMC) SDRAMC Configuration Register 1 */
#define REG_SDRAMC_OCMS          (0x400E022CU) /**< \brief (SDRAMC) SDRAMC OCMS Register 1 */
#else
#define REG_SDRAMC_MR   (*(RwReg*)0x400E0200U) /**< \brief (SDRAMC) SDRAMC Mode Register */
#define REG_SDRAMC_TR   (*(RwReg*)0x400E0204U) /**< \brief (SDRAMC) SDRAMC Refresh Timer Register */
#define REG_SDRAMC_CR   (*(RwReg*)0x400E0208U) /**< \brief (SDRAMC) SDRAMC Configuration Register */
#define REG_SDRAMC_LPR  (*(RwReg*)0x400E0210U) /**< \brief (SDRAMC) SDRAMC Low Power Register */
#define REG_SDRAMC_IER  (*(WoReg*)0x400E0214U) /**< \brief (SDRAMC) SDRAMC Interrupt Enable Register */
#define REG_SDRAMC_IDR  (*(WoReg*)0x400E0218U) /**< \brief (SDRAMC) SDRAMC Interrupt Disable Register */
#define REG_SDRAMC_IMR  (*(RoReg*)0x400E021CU) /**< \brief (SDRAMC) SDRAMC Interrupt Mask Register */
#define REG_SDRAMC_ISR  (*(RoReg*)0x400E0220U) /**< \brief (SDRAMC) SDRAMC Interrupt Status Register */
#define REG_SDRAMC_MDR  (*(RwReg*)0x400E0224U) /**< \brief (SDRAMC) SDRAMC Memory Device Register */
#define REG_SDRAMC_CR1  (*(RwReg*)0x400E0228U) /**< \brief (SDRAMC) SDRAMC Configuration Register 1 */
#define REG_SDRAMC_OCMS (*(RwReg*)0x400E022CU) /**< \brief (SDRAMC) SDRAMC OCMS Register 1 */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAM3XA_SDRAMC_INSTANCE_ */
