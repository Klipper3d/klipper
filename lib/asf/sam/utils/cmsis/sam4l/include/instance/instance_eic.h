/**
 * \file
 *
 * \brief Instance description for EIC
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

#ifndef _SAM4L_EIC_INSTANCE_
#define _SAM4L_EIC_INSTANCE_

/* ========== Register definition for EIC peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_EIC_IER                (0x400F1000U) /**< \brief (EIC) Interrupt Enable Register */
#define REG_EIC_IDR                (0x400F1004U) /**< \brief (EIC) Interrupt Disable Register */
#define REG_EIC_IMR                (0x400F1008U) /**< \brief (EIC) Interrupt Mask Register */
#define REG_EIC_ISR                (0x400F100CU) /**< \brief (EIC) Interrupt Status Register */
#define REG_EIC_ICR                (0x400F1010U) /**< \brief (EIC) Interrupt Clear Register */
#define REG_EIC_MODE               (0x400F1014U) /**< \brief (EIC) Mode Register */
#define REG_EIC_EDGE               (0x400F1018U) /**< \brief (EIC) Edge Register */
#define REG_EIC_LEVEL              (0x400F101CU) /**< \brief (EIC) Level Register */
#define REG_EIC_FILTER             (0x400F1020U) /**< \brief (EIC) Filter Register */
#define REG_EIC_ASYNC              (0x400F1028U) /**< \brief (EIC) Asynchronous Register */
#define REG_EIC_EN                 (0x400F1030U) /**< \brief (EIC) Enable Register */
#define REG_EIC_DIS                (0x400F1034U) /**< \brief (EIC) Disable Register */
#define REG_EIC_CTRL               (0x400F1038U) /**< \brief (EIC) Control Register */
#define REG_EIC_VERSION            (0x400F13FCU) /**< \brief (EIC) Version Register */
#else
#define REG_EIC_IER                (*(WoReg  *)0x400F1000U) /**< \brief (EIC) Interrupt Enable Register */
#define REG_EIC_IDR                (*(WoReg  *)0x400F1004U) /**< \brief (EIC) Interrupt Disable Register */
#define REG_EIC_IMR                (*(RoReg  *)0x400F1008U) /**< \brief (EIC) Interrupt Mask Register */
#define REG_EIC_ISR                (*(RoReg  *)0x400F100CU) /**< \brief (EIC) Interrupt Status Register */
#define REG_EIC_ICR                (*(WoReg  *)0x400F1010U) /**< \brief (EIC) Interrupt Clear Register */
#define REG_EIC_MODE               (*(RwReg  *)0x400F1014U) /**< \brief (EIC) Mode Register */
#define REG_EIC_EDGE               (*(RwReg  *)0x400F1018U) /**< \brief (EIC) Edge Register */
#define REG_EIC_LEVEL              (*(RwReg  *)0x400F101CU) /**< \brief (EIC) Level Register */
#define REG_EIC_FILTER             (*(RwReg  *)0x400F1020U) /**< \brief (EIC) Filter Register */
#define REG_EIC_ASYNC              (*(RwReg  *)0x400F1028U) /**< \brief (EIC) Asynchronous Register */
#define REG_EIC_EN                 (*(WoReg  *)0x400F1030U) /**< \brief (EIC) Enable Register */
#define REG_EIC_DIS                (*(WoReg  *)0x400F1034U) /**< \brief (EIC) Disable Register */
#define REG_EIC_CTRL               (*(RoReg  *)0x400F1038U) /**< \brief (EIC) Control Register */
#define REG_EIC_VERSION            (*(RoReg  *)0x400F13FCU) /**< \brief (EIC) Version Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/* ========== Instance parameters for EIC peripheral ========== */
#define EIC_STD_NUM                 8
#define EIC_EIC_EDGE_IRQ            0
#define EIC_EIC_FALLING_EDGE        0
#define EIC_EIC_FILTER_OFF          0
#define EIC_EIC_FILTER_ON           1
#define EIC_EIC_HIGH_LEVEL          1
#define EIC_EIC_LEVEL_IRQ           1
#define EIC_EIC_LOW_LEVEL           0
#define EIC_EIC_RISING_EDGE         1
#define EIC_EIC_SYNC                0
#define EIC_EIC_USE_ASYNC           1

#endif /* _SAM4L_EIC_INSTANCE_ */
