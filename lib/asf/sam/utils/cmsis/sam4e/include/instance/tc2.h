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

#ifndef _SAM4E_TC2_INSTANCE_
#define _SAM4E_TC2_INSTANCE_

/* ========== Register definition for TC2 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_TC2_CCR0                   (0x40098000U) /**< \brief (TC2) Channel Control Register (channel = 0) */
  #define REG_TC2_CMR0                   (0x40098004U) /**< \brief (TC2) Channel Mode Register (channel = 0) */
  #define REG_TC2_SMMR0                  (0x40098008U) /**< \brief (TC2) Stepper Motor Mode Register (channel = 0) */
  #define REG_TC2_RAB0                   (0x4009800CU) /**< \brief (TC2) Register AB (channel = 0) */
  #define REG_TC2_CV0                    (0x40098010U) /**< \brief (TC2) Counter Value (channel = 0) */
  #define REG_TC2_RA0                    (0x40098014U) /**< \brief (TC2) Register A (channel = 0) */
  #define REG_TC2_RB0                    (0x40098018U) /**< \brief (TC2) Register B (channel = 0) */
  #define REG_TC2_RC0                    (0x4009801CU) /**< \brief (TC2) Register C (channel = 0) */
  #define REG_TC2_SR0                    (0x40098020U) /**< \brief (TC2) Status Register (channel = 0) */
  #define REG_TC2_IER0                   (0x40098024U) /**< \brief (TC2) Interrupt Enable Register (channel = 0) */
  #define REG_TC2_IDR0                   (0x40098028U) /**< \brief (TC2) Interrupt Disable Register (channel = 0) */
  #define REG_TC2_IMR0                   (0x4009802CU) /**< \brief (TC2) Interrupt Mask Register (channel = 0) */
  #define REG_TC2_EMR0                   (0x40098030U) /**< \brief (TC2) Extended Mode Register (channel = 0) */
  #define REG_TC2_CCR1                   (0x40098040U) /**< \brief (TC2) Channel Control Register (channel = 1) */
  #define REG_TC2_CMR1                   (0x40098044U) /**< \brief (TC2) Channel Mode Register (channel = 1) */
  #define REG_TC2_SMMR1                  (0x40098048U) /**< \brief (TC2) Stepper Motor Mode Register (channel = 1) */
  #define REG_TC2_RAB1                   (0x4009804CU) /**< \brief (TC2) Register AB (channel = 1) */
  #define REG_TC2_CV1                    (0x40098050U) /**< \brief (TC2) Counter Value (channel = 1) */
  #define REG_TC2_RA1                    (0x40098054U) /**< \brief (TC2) Register A (channel = 1) */
  #define REG_TC2_RB1                    (0x40098058U) /**< \brief (TC2) Register B (channel = 1) */
  #define REG_TC2_RC1                    (0x4009805CU) /**< \brief (TC2) Register C (channel = 1) */
  #define REG_TC2_SR1                    (0x40098060U) /**< \brief (TC2) Status Register (channel = 1) */
  #define REG_TC2_IER1                   (0x40098064U) /**< \brief (TC2) Interrupt Enable Register (channel = 1) */
  #define REG_TC2_IDR1                   (0x40098068U) /**< \brief (TC2) Interrupt Disable Register (channel = 1) */
  #define REG_TC2_IMR1                   (0x4009806CU) /**< \brief (TC2) Interrupt Mask Register (channel = 1) */
  #define REG_TC2_EMR1                   (0x40098070U) /**< \brief (TC2) Extended Mode Register (channel = 1) */
  #define REG_TC2_CCR2                   (0x40098080U) /**< \brief (TC2) Channel Control Register (channel = 2) */
  #define REG_TC2_CMR2                   (0x40098084U) /**< \brief (TC2) Channel Mode Register (channel = 2) */
  #define REG_TC2_SMMR2                  (0x40098088U) /**< \brief (TC2) Stepper Motor Mode Register (channel = 2) */
  #define REG_TC2_RAB2                   (0x4009808CU) /**< \brief (TC2) Register AB (channel = 2) */
  #define REG_TC2_CV2                    (0x40098090U) /**< \brief (TC2) Counter Value (channel = 2) */
  #define REG_TC2_RA2                    (0x40098094U) /**< \brief (TC2) Register A (channel = 2) */
  #define REG_TC2_RB2                    (0x40098098U) /**< \brief (TC2) Register B (channel = 2) */
  #define REG_TC2_RC2                    (0x4009809CU) /**< \brief (TC2) Register C (channel = 2) */
  #define REG_TC2_SR2                    (0x400980A0U) /**< \brief (TC2) Status Register (channel = 2) */
  #define REG_TC2_IER2                   (0x400980A4U) /**< \brief (TC2) Interrupt Enable Register (channel = 2) */
  #define REG_TC2_IDR2                   (0x400980A8U) /**< \brief (TC2) Interrupt Disable Register (channel = 2) */
  #define REG_TC2_IMR2                   (0x400980ACU) /**< \brief (TC2) Interrupt Mask Register (channel = 2) */
  #define REG_TC2_EMR2                   (0x400980B0U) /**< \brief (TC2) Extended Mode Register (channel = 2) */
  #define REG_TC2_BCR                    (0x400980C0U) /**< \brief (TC2) Block Control Register */
  #define REG_TC2_BMR                    (0x400980C4U) /**< \brief (TC2) Block Mode Register */
  #define REG_TC2_QIER                   (0x400980C8U) /**< \brief (TC2) QDEC Interrupt Enable Register */
  #define REG_TC2_QIDR                   (0x400980CCU) /**< \brief (TC2) QDEC Interrupt Disable Register */
  #define REG_TC2_QIMR                   (0x400980D0U) /**< \brief (TC2) QDEC Interrupt Mask Register */
  #define REG_TC2_QISR                   (0x400980D4U) /**< \brief (TC2) QDEC Interrupt Status Register */
  #define REG_TC2_FMR                    (0x400980D8U) /**< \brief (TC2) Fault Mode Register */
  #define REG_TC2_WPMR                   (0x400980E4U) /**< \brief (TC2) Write Protection Mode Register */
#else
  #define REG_TC2_CCR0  (*(__O  uint32_t*)0x40098000U) /**< \brief (TC2) Channel Control Register (channel = 0) */
  #define REG_TC2_CMR0  (*(__IO uint32_t*)0x40098004U) /**< \brief (TC2) Channel Mode Register (channel = 0) */
  #define REG_TC2_SMMR0 (*(__IO uint32_t*)0x40098008U) /**< \brief (TC2) Stepper Motor Mode Register (channel = 0) */
  #define REG_TC2_RAB0  (*(__I  uint32_t*)0x4009800CU) /**< \brief (TC2) Register AB (channel = 0) */
  #define REG_TC2_CV0   (*(__I  uint32_t*)0x40098010U) /**< \brief (TC2) Counter Value (channel = 0) */
  #define REG_TC2_RA0   (*(__IO uint32_t*)0x40098014U) /**< \brief (TC2) Register A (channel = 0) */
  #define REG_TC2_RB0   (*(__IO uint32_t*)0x40098018U) /**< \brief (TC2) Register B (channel = 0) */
  #define REG_TC2_RC0   (*(__IO uint32_t*)0x4009801CU) /**< \brief (TC2) Register C (channel = 0) */
  #define REG_TC2_SR0   (*(__I  uint32_t*)0x40098020U) /**< \brief (TC2) Status Register (channel = 0) */
  #define REG_TC2_IER0  (*(__O  uint32_t*)0x40098024U) /**< \brief (TC2) Interrupt Enable Register (channel = 0) */
  #define REG_TC2_IDR0  (*(__O  uint32_t*)0x40098028U) /**< \brief (TC2) Interrupt Disable Register (channel = 0) */
  #define REG_TC2_IMR0  (*(__I  uint32_t*)0x4009802CU) /**< \brief (TC2) Interrupt Mask Register (channel = 0) */
  #define REG_TC2_EMR0  (*(__IO uint32_t*)0x40098030U) /**< \brief (TC2) Extended Mode Register (channel = 0) */
  #define REG_TC2_CCR1  (*(__O  uint32_t*)0x40098040U) /**< \brief (TC2) Channel Control Register (channel = 1) */
  #define REG_TC2_CMR1  (*(__IO uint32_t*)0x40098044U) /**< \brief (TC2) Channel Mode Register (channel = 1) */
  #define REG_TC2_SMMR1 (*(__IO uint32_t*)0x40098048U) /**< \brief (TC2) Stepper Motor Mode Register (channel = 1) */
  #define REG_TC2_RAB1  (*(__I  uint32_t*)0x4009804CU) /**< \brief (TC2) Register AB (channel = 1) */
  #define REG_TC2_CV1   (*(__I  uint32_t*)0x40098050U) /**< \brief (TC2) Counter Value (channel = 1) */
  #define REG_TC2_RA1   (*(__IO uint32_t*)0x40098054U) /**< \brief (TC2) Register A (channel = 1) */
  #define REG_TC2_RB1   (*(__IO uint32_t*)0x40098058U) /**< \brief (TC2) Register B (channel = 1) */
  #define REG_TC2_RC1   (*(__IO uint32_t*)0x4009805CU) /**< \brief (TC2) Register C (channel = 1) */
  #define REG_TC2_SR1   (*(__I  uint32_t*)0x40098060U) /**< \brief (TC2) Status Register (channel = 1) */
  #define REG_TC2_IER1  (*(__O  uint32_t*)0x40098064U) /**< \brief (TC2) Interrupt Enable Register (channel = 1) */
  #define REG_TC2_IDR1  (*(__O  uint32_t*)0x40098068U) /**< \brief (TC2) Interrupt Disable Register (channel = 1) */
  #define REG_TC2_IMR1  (*(__I  uint32_t*)0x4009806CU) /**< \brief (TC2) Interrupt Mask Register (channel = 1) */
  #define REG_TC2_EMR1  (*(__IO uint32_t*)0x40098070U) /**< \brief (TC2) Extended Mode Register (channel = 1) */
  #define REG_TC2_CCR2  (*(__O  uint32_t*)0x40098080U) /**< \brief (TC2) Channel Control Register (channel = 2) */
  #define REG_TC2_CMR2  (*(__IO uint32_t*)0x40098084U) /**< \brief (TC2) Channel Mode Register (channel = 2) */
  #define REG_TC2_SMMR2 (*(__IO uint32_t*)0x40098088U) /**< \brief (TC2) Stepper Motor Mode Register (channel = 2) */
  #define REG_TC2_RAB2  (*(__I  uint32_t*)0x4009808CU) /**< \brief (TC2) Register AB (channel = 2) */
  #define REG_TC2_CV2   (*(__I  uint32_t*)0x40098090U) /**< \brief (TC2) Counter Value (channel = 2) */
  #define REG_TC2_RA2   (*(__IO uint32_t*)0x40098094U) /**< \brief (TC2) Register A (channel = 2) */
  #define REG_TC2_RB2   (*(__IO uint32_t*)0x40098098U) /**< \brief (TC2) Register B (channel = 2) */
  #define REG_TC2_RC2   (*(__IO uint32_t*)0x4009809CU) /**< \brief (TC2) Register C (channel = 2) */
  #define REG_TC2_SR2   (*(__I  uint32_t*)0x400980A0U) /**< \brief (TC2) Status Register (channel = 2) */
  #define REG_TC2_IER2  (*(__O  uint32_t*)0x400980A4U) /**< \brief (TC2) Interrupt Enable Register (channel = 2) */
  #define REG_TC2_IDR2  (*(__O  uint32_t*)0x400980A8U) /**< \brief (TC2) Interrupt Disable Register (channel = 2) */
  #define REG_TC2_IMR2  (*(__I  uint32_t*)0x400980ACU) /**< \brief (TC2) Interrupt Mask Register (channel = 2) */
  #define REG_TC2_EMR2  (*(__IO uint32_t*)0x400980B0U) /**< \brief (TC2) Extended Mode Register (channel = 2) */
  #define REG_TC2_BCR   (*(__O  uint32_t*)0x400980C0U) /**< \brief (TC2) Block Control Register */
  #define REG_TC2_BMR   (*(__IO uint32_t*)0x400980C4U) /**< \brief (TC2) Block Mode Register */
  #define REG_TC2_QIER  (*(__O  uint32_t*)0x400980C8U) /**< \brief (TC2) QDEC Interrupt Enable Register */
  #define REG_TC2_QIDR  (*(__O  uint32_t*)0x400980CCU) /**< \brief (TC2) QDEC Interrupt Disable Register */
  #define REG_TC2_QIMR  (*(__I  uint32_t*)0x400980D0U) /**< \brief (TC2) QDEC Interrupt Mask Register */
  #define REG_TC2_QISR  (*(__I  uint32_t*)0x400980D4U) /**< \brief (TC2) QDEC Interrupt Status Register */
  #define REG_TC2_FMR   (*(__IO uint32_t*)0x400980D8U) /**< \brief (TC2) Fault Mode Register */
  #define REG_TC2_WPMR  (*(__IO uint32_t*)0x400980E4U) /**< \brief (TC2) Write Protection Mode Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAM4E_TC2_INSTANCE_ */
