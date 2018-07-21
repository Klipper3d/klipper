/**
 * \file
 *
 * \brief Instance description for IISC
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

#ifndef _SAM4L_IISC_INSTANCE_
#define _SAM4L_IISC_INSTANCE_

/* ========== Register definition for IISC peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_IISC_CR                (0x40004000U) /**< \brief (IISC) Control Register */
#define REG_IISC_MR                (0x40004004U) /**< \brief (IISC) Mode Register */
#define REG_IISC_SR                (0x40004008U) /**< \brief (IISC) Status Register */
#define REG_IISC_SCR               (0x4000400CU) /**< \brief (IISC) Status Clear Register */
#define REG_IISC_SSR               (0x40004010U) /**< \brief (IISC) Status Set Register */
#define REG_IISC_IER               (0x40004014U) /**< \brief (IISC) Interrupt Enable Register */
#define REG_IISC_IDR               (0x40004018U) /**< \brief (IISC) Interrupt Disable Register */
#define REG_IISC_IMR               (0x4000401CU) /**< \brief (IISC) Interrupt Mask Register */
#define REG_IISC_RHR               (0x40004020U) /**< \brief (IISC) Receive Holding Register */
#define REG_IISC_THR               (0x40004024U) /**< \brief (IISC) Transmit Holding Register */
#define REG_IISC_VERSION           (0x40004028U) /**< \brief (IISC) Version Register */
#define REG_IISC_PARAMETER         (0x4000402CU) /**< \brief (IISC) Parameter Register */
#else
#define REG_IISC_CR                (*(WoReg  *)0x40004000U) /**< \brief (IISC) Control Register */
#define REG_IISC_MR                (*(RwReg  *)0x40004004U) /**< \brief (IISC) Mode Register */
#define REG_IISC_SR                (*(RoReg  *)0x40004008U) /**< \brief (IISC) Status Register */
#define REG_IISC_SCR               (*(WoReg  *)0x4000400CU) /**< \brief (IISC) Status Clear Register */
#define REG_IISC_SSR               (*(WoReg  *)0x40004010U) /**< \brief (IISC) Status Set Register */
#define REG_IISC_IER               (*(WoReg  *)0x40004014U) /**< \brief (IISC) Interrupt Enable Register */
#define REG_IISC_IDR               (*(WoReg  *)0x40004018U) /**< \brief (IISC) Interrupt Disable Register */
#define REG_IISC_IMR               (*(RoReg  *)0x4000401CU) /**< \brief (IISC) Interrupt Mask Register */
#define REG_IISC_RHR               (*(RoReg  *)0x40004020U) /**< \brief (IISC) Receive Holding Register */
#define REG_IISC_THR               (*(WoReg  *)0x40004024U) /**< \brief (IISC) Transmit Holding Register */
#define REG_IISC_VERSION           (*(RoReg  *)0x40004028U) /**< \brief (IISC) Version Register */
#define REG_IISC_PARAMETER         (*(RoReg  *)0x4000402CU) /**< \brief (IISC) Parameter Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/* ========== Instance parameters for IISC peripheral ========== */
#define IISC_GCLK_NUM               6
#define IISC_PDCA_ID_RX             14
#define IISC_PDCA_ID_RX_1           15
#define IISC_PDCA_ID_TX             33
#define IISC_PDCA_ID_TX_1           34

#endif /* _SAM4L_IISC_INSTANCE_ */
