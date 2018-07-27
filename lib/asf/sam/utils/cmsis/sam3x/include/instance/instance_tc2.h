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

#ifndef _SAM3XA_TC2_INSTANCE_
#define _SAM3XA_TC2_INSTANCE_

/* ========== Register definition for TC2 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_TC2_CCR0           (0x40088000U) /**< \brief (TC2) Channel Control Register (channel = 0) */
#define REG_TC2_CMR0           (0x40088004U) /**< \brief (TC2) Channel Mode Register (channel = 0) */
#define REG_TC2_SMMR0          (0x40088008U) /**< \brief (TC2) Stepper Motor Mode Register (channel = 0) */
#define REG_TC2_CV0            (0x40088010U) /**< \brief (TC2) Counter Value (channel = 0) */
#define REG_TC2_RA0            (0x40088014U) /**< \brief (TC2) Register A (channel = 0) */
#define REG_TC2_RB0            (0x40088018U) /**< \brief (TC2) Register B (channel = 0) */
#define REG_TC2_RC0            (0x4008801CU) /**< \brief (TC2) Register C (channel = 0) */
#define REG_TC2_SR0            (0x40088020U) /**< \brief (TC2) Status Register (channel = 0) */
#define REG_TC2_IER0           (0x40088024U) /**< \brief (TC2) Interrupt Enable Register (channel = 0) */
#define REG_TC2_IDR0           (0x40088028U) /**< \brief (TC2) Interrupt Disable Register (channel = 0) */
#define REG_TC2_IMR0           (0x4008802CU) /**< \brief (TC2) Interrupt Mask Register (channel = 0) */
#define REG_TC2_CCR1           (0x40088040U) /**< \brief (TC2) Channel Control Register (channel = 1) */
#define REG_TC2_CMR1           (0x40088044U) /**< \brief (TC2) Channel Mode Register (channel = 1) */
#define REG_TC2_SMMR1          (0x40088048U) /**< \brief (TC2) Stepper Motor Mode Register (channel = 1) */
#define REG_TC2_CV1            (0x40088050U) /**< \brief (TC2) Counter Value (channel = 1) */
#define REG_TC2_RA1            (0x40088054U) /**< \brief (TC2) Register A (channel = 1) */
#define REG_TC2_RB1            (0x40088058U) /**< \brief (TC2) Register B (channel = 1) */
#define REG_TC2_RC1            (0x4008805CU) /**< \brief (TC2) Register C (channel = 1) */
#define REG_TC2_SR1            (0x40088060U) /**< \brief (TC2) Status Register (channel = 1) */
#define REG_TC2_IER1           (0x40088064U) /**< \brief (TC2) Interrupt Enable Register (channel = 1) */
#define REG_TC2_IDR1           (0x40088068U) /**< \brief (TC2) Interrupt Disable Register (channel = 1) */
#define REG_TC2_IMR1           (0x4008806CU) /**< \brief (TC2) Interrupt Mask Register (channel = 1) */
#define REG_TC2_CCR2           (0x40088080U) /**< \brief (TC2) Channel Control Register (channel = 2) */
#define REG_TC2_CMR2           (0x40088084U) /**< \brief (TC2) Channel Mode Register (channel = 2) */
#define REG_TC2_SMMR2          (0x40088088U) /**< \brief (TC2) Stepper Motor Mode Register (channel = 2) */
#define REG_TC2_CV2            (0x40088090U) /**< \brief (TC2) Counter Value (channel = 2) */
#define REG_TC2_RA2            (0x40088094U) /**< \brief (TC2) Register A (channel = 2) */
#define REG_TC2_RB2            (0x40088098U) /**< \brief (TC2) Register B (channel = 2) */
#define REG_TC2_RC2            (0x4008809CU) /**< \brief (TC2) Register C (channel = 2) */
#define REG_TC2_SR2            (0x400880A0U) /**< \brief (TC2) Status Register (channel = 2) */
#define REG_TC2_IER2           (0x400880A4U) /**< \brief (TC2) Interrupt Enable Register (channel = 2) */
#define REG_TC2_IDR2           (0x400880A8U) /**< \brief (TC2) Interrupt Disable Register (channel = 2) */
#define REG_TC2_IMR2           (0x400880ACU) /**< \brief (TC2) Interrupt Mask Register (channel = 2) */
#define REG_TC2_BCR            (0x400880C0U) /**< \brief (TC2) Block Control Register */
#define REG_TC2_BMR            (0x400880C4U) /**< \brief (TC2) Block Mode Register */
#define REG_TC2_QIER           (0x400880C8U) /**< \brief (TC2) QDEC Interrupt Enable Register */
#define REG_TC2_QIDR           (0x400880CCU) /**< \brief (TC2) QDEC Interrupt Disable Register */
#define REG_TC2_QIMR           (0x400880D0U) /**< \brief (TC2) QDEC Interrupt Mask Register */
#define REG_TC2_QISR           (0x400880D4U) /**< \brief (TC2) QDEC Interrupt Status Register */
#define REG_TC2_FMR            (0x400880D8U) /**< \brief (TC2) Fault Mode Register */
#define REG_TC2_WPMR           (0x400880E4U) /**< \brief (TC2) Write Protect Mode Register */
#else
#define REG_TC2_CCR0  (*(WoReg*)0x40088000U) /**< \brief (TC2) Channel Control Register (channel = 0) */
#define REG_TC2_CMR0  (*(RwReg*)0x40088004U) /**< \brief (TC2) Channel Mode Register (channel = 0) */
#define REG_TC2_SMMR0 (*(RwReg*)0x40088008U) /**< \brief (TC2) Stepper Motor Mode Register (channel = 0) */
#define REG_TC2_CV0   (*(RoReg*)0x40088010U) /**< \brief (TC2) Counter Value (channel = 0) */
#define REG_TC2_RA0   (*(RwReg*)0x40088014U) /**< \brief (TC2) Register A (channel = 0) */
#define REG_TC2_RB0   (*(RwReg*)0x40088018U) /**< \brief (TC2) Register B (channel = 0) */
#define REG_TC2_RC0   (*(RwReg*)0x4008801CU) /**< \brief (TC2) Register C (channel = 0) */
#define REG_TC2_SR0   (*(RoReg*)0x40088020U) /**< \brief (TC2) Status Register (channel = 0) */
#define REG_TC2_IER0  (*(WoReg*)0x40088024U) /**< \brief (TC2) Interrupt Enable Register (channel = 0) */
#define REG_TC2_IDR0  (*(WoReg*)0x40088028U) /**< \brief (TC2) Interrupt Disable Register (channel = 0) */
#define REG_TC2_IMR0  (*(RoReg*)0x4008802CU) /**< \brief (TC2) Interrupt Mask Register (channel = 0) */
#define REG_TC2_CCR1  (*(WoReg*)0x40088040U) /**< \brief (TC2) Channel Control Register (channel = 1) */
#define REG_TC2_CMR1  (*(RwReg*)0x40088044U) /**< \brief (TC2) Channel Mode Register (channel = 1) */
#define REG_TC2_SMMR1 (*(RwReg*)0x40088048U) /**< \brief (TC2) Stepper Motor Mode Register (channel = 1) */
#define REG_TC2_CV1   (*(RoReg*)0x40088050U) /**< \brief (TC2) Counter Value (channel = 1) */
#define REG_TC2_RA1   (*(RwReg*)0x40088054U) /**< \brief (TC2) Register A (channel = 1) */
#define REG_TC2_RB1   (*(RwReg*)0x40088058U) /**< \brief (TC2) Register B (channel = 1) */
#define REG_TC2_RC1   (*(RwReg*)0x4008805CU) /**< \brief (TC2) Register C (channel = 1) */
#define REG_TC2_SR1   (*(RoReg*)0x40088060U) /**< \brief (TC2) Status Register (channel = 1) */
#define REG_TC2_IER1  (*(WoReg*)0x40088064U) /**< \brief (TC2) Interrupt Enable Register (channel = 1) */
#define REG_TC2_IDR1  (*(WoReg*)0x40088068U) /**< \brief (TC2) Interrupt Disable Register (channel = 1) */
#define REG_TC2_IMR1  (*(RoReg*)0x4008806CU) /**< \brief (TC2) Interrupt Mask Register (channel = 1) */
#define REG_TC2_CCR2  (*(WoReg*)0x40088080U) /**< \brief (TC2) Channel Control Register (channel = 2) */
#define REG_TC2_CMR2  (*(RwReg*)0x40088084U) /**< \brief (TC2) Channel Mode Register (channel = 2) */
#define REG_TC2_SMMR2 (*(RwReg*)0x40088088U) /**< \brief (TC2) Stepper Motor Mode Register (channel = 2) */
#define REG_TC2_CV2   (*(RoReg*)0x40088090U) /**< \brief (TC2) Counter Value (channel = 2) */
#define REG_TC2_RA2   (*(RwReg*)0x40088094U) /**< \brief (TC2) Register A (channel = 2) */
#define REG_TC2_RB2   (*(RwReg*)0x40088098U) /**< \brief (TC2) Register B (channel = 2) */
#define REG_TC2_RC2   (*(RwReg*)0x4008809CU) /**< \brief (TC2) Register C (channel = 2) */
#define REG_TC2_SR2   (*(RoReg*)0x400880A0U) /**< \brief (TC2) Status Register (channel = 2) */
#define REG_TC2_IER2  (*(WoReg*)0x400880A4U) /**< \brief (TC2) Interrupt Enable Register (channel = 2) */
#define REG_TC2_IDR2  (*(WoReg*)0x400880A8U) /**< \brief (TC2) Interrupt Disable Register (channel = 2) */
#define REG_TC2_IMR2  (*(RoReg*)0x400880ACU) /**< \brief (TC2) Interrupt Mask Register (channel = 2) */
#define REG_TC2_BCR   (*(WoReg*)0x400880C0U) /**< \brief (TC2) Block Control Register */
#define REG_TC2_BMR   (*(RwReg*)0x400880C4U) /**< \brief (TC2) Block Mode Register */
#define REG_TC2_QIER  (*(WoReg*)0x400880C8U) /**< \brief (TC2) QDEC Interrupt Enable Register */
#define REG_TC2_QIDR  (*(WoReg*)0x400880CCU) /**< \brief (TC2) QDEC Interrupt Disable Register */
#define REG_TC2_QIMR  (*(RoReg*)0x400880D0U) /**< \brief (TC2) QDEC Interrupt Mask Register */
#define REG_TC2_QISR  (*(RoReg*)0x400880D4U) /**< \brief (TC2) QDEC Interrupt Status Register */
#define REG_TC2_FMR   (*(RwReg*)0x400880D8U) /**< \brief (TC2) Fault Mode Register */
#define REG_TC2_WPMR  (*(RwReg*)0x400880E4U) /**< \brief (TC2) Write Protect Mode Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAM3XA_TC2_INSTANCE_ */
