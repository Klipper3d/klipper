/**
 * \file
 *
 * \brief Instance description for ABDACB
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

#ifndef _SAM4L_ABDACB_INSTANCE_
#define _SAM4L_ABDACB_INSTANCE_

/* ========== Register definition for ABDACB peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_ABDACB_CR              (0x40064000U) /**< \brief (ABDACB) Control Register */
#define REG_ABDACB_SDR0            (0x40064004U) /**< \brief (ABDACB) Sample Data Register 0 */
#define REG_ABDACB_SDR1            (0x40064008U) /**< \brief (ABDACB) Sample Data Register 1 */
#define REG_ABDACB_VCR0            (0x4006400CU) /**< \brief (ABDACB) Volume Control Register 0 */
#define REG_ABDACB_VCR1            (0x40064010U) /**< \brief (ABDACB) Volume Control Register 1 */
#define REG_ABDACB_IER             (0x40064014U) /**< \brief (ABDACB) Interrupt Enable Register */
#define REG_ABDACB_IDR             (0x40064018U) /**< \brief (ABDACB) Interupt Disable Register */
#define REG_ABDACB_IMR             (0x4006401CU) /**< \brief (ABDACB) Interrupt Mask Register */
#define REG_ABDACB_SR              (0x40064020U) /**< \brief (ABDACB) Status Register */
#define REG_ABDACB_SCR             (0x40064024U) /**< \brief (ABDACB) Status Clear Register */
#define REG_ABDACB_PARAMETER       (0x40064028U) /**< \brief (ABDACB) Parameter Register */
#define REG_ABDACB_VERSION         (0x4006402CU) /**< \brief (ABDACB) Version Register */
#else
#define REG_ABDACB_CR              (*(RwReg  *)0x40064000U) /**< \brief (ABDACB) Control Register */
#define REG_ABDACB_SDR0            (*(RwReg  *)0x40064004U) /**< \brief (ABDACB) Sample Data Register 0 */
#define REG_ABDACB_SDR1            (*(RwReg  *)0x40064008U) /**< \brief (ABDACB) Sample Data Register 1 */
#define REG_ABDACB_VCR0            (*(RwReg  *)0x4006400CU) /**< \brief (ABDACB) Volume Control Register 0 */
#define REG_ABDACB_VCR1            (*(RwReg  *)0x40064010U) /**< \brief (ABDACB) Volume Control Register 1 */
#define REG_ABDACB_IER             (*(WoReg  *)0x40064014U) /**< \brief (ABDACB) Interrupt Enable Register */
#define REG_ABDACB_IDR             (*(WoReg  *)0x40064018U) /**< \brief (ABDACB) Interupt Disable Register */
#define REG_ABDACB_IMR             (*(RoReg  *)0x4006401CU) /**< \brief (ABDACB) Interrupt Mask Register */
#define REG_ABDACB_SR              (*(RoReg  *)0x40064020U) /**< \brief (ABDACB) Status Register */
#define REG_ABDACB_SCR             (*(WoReg  *)0x40064024U) /**< \brief (ABDACB) Status Clear Register */
#define REG_ABDACB_PARAMETER       (*(RoReg  *)0x40064028U) /**< \brief (ABDACB) Parameter Register */
#define REG_ABDACB_VERSION         (*(RoReg  *)0x4006402CU) /**< \brief (ABDACB) Version Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/* ========== Instance parameters for ABDACB peripheral ========== */
#define ABDACB_GCLK_NUM             6
#define ABDACB_PDCA_ID_TX_CH0       31
#define ABDACB_PDCA_ID_TX_CH1       32

#endif /* _SAM4L_ABDACB_INSTANCE_ */
