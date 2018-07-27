/**
 * \file
 *
 * \brief Instance description for BPM
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

#ifndef _SAM4L_BPM_INSTANCE_
#define _SAM4L_BPM_INSTANCE_

/* ========== Register definition for BPM peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_BPM_IER                (0x400F0000U) /**< \brief (BPM) Interrupt Enable Register */
#define REG_BPM_IDR                (0x400F0004U) /**< \brief (BPM) Interrupt Disable Register */
#define REG_BPM_IMR                (0x400F0008U) /**< \brief (BPM) Interrupt Mask Register */
#define REG_BPM_ISR                (0x400F000CU) /**< \brief (BPM) Interrupt Status Register */
#define REG_BPM_ICR                (0x400F0010U) /**< \brief (BPM) Interrupt Clear Register */
#define REG_BPM_SR                 (0x400F0014U) /**< \brief (BPM) Status Register */
#define REG_BPM_UNLOCK             (0x400F0018U) /**< \brief (BPM) Unlock Register */
#define REG_BPM_PMCON              (0x400F001CU) /**< \brief (BPM) Power Mode Control Register */
#define REG_BPM_BKUPWCAUSE         (0x400F0028U) /**< \brief (BPM) Backup Wake up Cause Register */
#define REG_BPM_BKUPWEN            (0x400F002CU) /**< \brief (BPM) Backup Wake up Enable Register */
#define REG_BPM_BKUPPMUX           (0x400F0030U) /**< \brief (BPM) Backup Pin Muxing Register */
#define REG_BPM_IORET              (0x400F0034U) /**< \brief (BPM) Input Output Retention Register */
#define REG_BPM_BPR                (0x400F0040U) /**< \brief (BPM) Bypass Register */
#define REG_BPM_FWRUNPS            (0x400F0044U) /**< \brief (BPM) Factory Word Run PS Register */
#define REG_BPM_FWPSAVEPS          (0x400F0048U) /**< \brief (BPM) Factory Word Power Save PS Register */
#define REG_BPM_VERSION            (0x400F00FCU) /**< \brief (BPM) Version Register */
#else
#define REG_BPM_IER                (*(WoReg  *)0x400F0000U) /**< \brief (BPM) Interrupt Enable Register */
#define REG_BPM_IDR                (*(WoReg  *)0x400F0004U) /**< \brief (BPM) Interrupt Disable Register */
#define REG_BPM_IMR                (*(RoReg  *)0x400F0008U) /**< \brief (BPM) Interrupt Mask Register */
#define REG_BPM_ISR                (*(RoReg  *)0x400F000CU) /**< \brief (BPM) Interrupt Status Register */
#define REG_BPM_ICR                (*(WoReg  *)0x400F0010U) /**< \brief (BPM) Interrupt Clear Register */
#define REG_BPM_SR                 (*(RoReg  *)0x400F0014U) /**< \brief (BPM) Status Register */
#define REG_BPM_UNLOCK             (*(WoReg  *)0x400F0018U) /**< \brief (BPM) Unlock Register */
#define REG_BPM_PMCON              (*(RwReg  *)0x400F001CU) /**< \brief (BPM) Power Mode Control Register */
#define REG_BPM_BKUPWCAUSE         (*(RoReg  *)0x400F0028U) /**< \brief (BPM) Backup Wake up Cause Register */
#define REG_BPM_BKUPWEN            (*(RwReg  *)0x400F002CU) /**< \brief (BPM) Backup Wake up Enable Register */
#define REG_BPM_BKUPPMUX           (*(RwReg  *)0x400F0030U) /**< \brief (BPM) Backup Pin Muxing Register */
#define REG_BPM_IORET              (*(RwReg  *)0x400F0034U) /**< \brief (BPM) Input Output Retention Register */
#define REG_BPM_BPR                (*(RwReg  *)0x400F0040U) /**< \brief (BPM) Bypass Register */
#define REG_BPM_FWRUNPS            (*(RoReg  *)0x400F0044U) /**< \brief (BPM) Factory Word Run PS Register */
#define REG_BPM_FWPSAVEPS          (*(RoReg  *)0x400F0048U) /**< \brief (BPM) Factory Word Power Save PS Register */
#define REG_BPM_VERSION            (*(RoReg  *)0x400F00FCU) /**< \brief (BPM) Version Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/* ========== Instance parameters for BPM peripheral ========== */
#define BPM_BKUPPMUX_MSB            9
#define BPM_BKUPWEN_AST             1
#define BPM_BKUPWEN_BOD18           4
#define BPM_BKUPWEN_BOD33           3
#define BPM_BKUPWEN_EIC             0
#define BPM_BKUPWEN_MSB             5
#define BPM_BKUPWEN_PICOUART        5
#define BPM_BKUPWEN_WDT             2

#endif /* _SAM4L_BPM_INSTANCE_ */
