/**
 * \file
 *
 * \brief Instance description for PARC
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

#ifndef _SAM4L_PARC_INSTANCE_
#define _SAM4L_PARC_INSTANCE_

/* ========== Register definition for PARC peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_PARC_CFG               (0x4006C000U) /**< \brief (PARC) Configuration Register */
#define REG_PARC_CR                (0x4006C004U) /**< \brief (PARC) Control Register */
#define REG_PARC_IER               (0x4006C008U) /**< \brief (PARC) Interrupt Enable Register */
#define REG_PARC_IDR               (0x4006C00CU) /**< \brief (PARC) Interrupt Disable Register */
#define REG_PARC_IMR               (0x4006C010U) /**< \brief (PARC) Interrupt Mask Register */
#define REG_PARC_SR                (0x4006C014U) /**< \brief (PARC) Status Register */
#define REG_PARC_ICR               (0x4006C018U) /**< \brief (PARC) Interrupt Status Clear Register */
#define REG_PARC_RHR               (0x4006C01CU) /**< \brief (PARC) Receive Holding Register */
#define REG_PARC_VERSION           (0x4006C020U) /**< \brief (PARC) Version Register */
#else
#define REG_PARC_CFG               (*(RwReg  *)0x4006C000U) /**< \brief (PARC) Configuration Register */
#define REG_PARC_CR                (*(RwReg  *)0x4006C004U) /**< \brief (PARC) Control Register */
#define REG_PARC_IER               (*(WoReg  *)0x4006C008U) /**< \brief (PARC) Interrupt Enable Register */
#define REG_PARC_IDR               (*(WoReg  *)0x4006C00CU) /**< \brief (PARC) Interrupt Disable Register */
#define REG_PARC_IMR               (*(RoReg  *)0x4006C010U) /**< \brief (PARC) Interrupt Mask Register */
#define REG_PARC_SR                (*(RoReg  *)0x4006C014U) /**< \brief (PARC) Status Register */
#define REG_PARC_ICR               (*(WoReg  *)0x4006C018U) /**< \brief (PARC) Interrupt Status Clear Register */
#define REG_PARC_RHR               (*(RoReg  *)0x4006C01CU) /**< \brief (PARC) Receive Holding Register */
#define REG_PARC_VERSION           (*(RoReg  *)0x4006C020U) /**< \brief (PARC) Version Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/* ========== Instance parameters for PARC peripheral ========== */
#define PARC_PDCA_ID_RX             16

#endif /* _SAM4L_PARC_INSTANCE_ */
