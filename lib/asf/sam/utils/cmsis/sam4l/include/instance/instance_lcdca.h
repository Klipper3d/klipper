/**
 * \file
 *
 * \brief Instance description for LCDCA
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

#ifndef _SAM4L_LCDCA_INSTANCE_
#define _SAM4L_LCDCA_INSTANCE_

/* ========== Register definition for LCDCA peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_LCDCA_CR               (0x40080000U) /**< \brief (LCDCA) Control Register */
#define REG_LCDCA_CFG              (0x40080004U) /**< \brief (LCDCA) Configuration Register */
#define REG_LCDCA_TIM              (0x40080008U) /**< \brief (LCDCA) Timing Register */
#define REG_LCDCA_SR               (0x4008000CU) /**< \brief (LCDCA) Status Register */
#define REG_LCDCA_SCR              (0x40080010U) /**< \brief (LCDCA) Status Clear Register */
#define REG_LCDCA_DRL0             (0x40080014U) /**< \brief (LCDCA) Data Register Low 0 */
#define REG_LCDCA_DRH0             (0x40080018U) /**< \brief (LCDCA) Data Register High 0 */
#define REG_LCDCA_DRL1             (0x4008001CU) /**< \brief (LCDCA) Data Register Low 1 */
#define REG_LCDCA_DRH1             (0x40080020U) /**< \brief (LCDCA) Data Register High 1 */
#define REG_LCDCA_DRL2             (0x40080024U) /**< \brief (LCDCA) Data Register Low 2 */
#define REG_LCDCA_DRH2             (0x40080028U) /**< \brief (LCDCA) Data Register High 2 */
#define REG_LCDCA_DRL3             (0x4008002CU) /**< \brief (LCDCA) Data Register Low 3 */
#define REG_LCDCA_DRH3             (0x40080030U) /**< \brief (LCDCA) Data Register High 3 */
#define REG_LCDCA_IADR             (0x40080034U) /**< \brief (LCDCA) Indirect Access Data Register */
#define REG_LCDCA_BCFG             (0x40080038U) /**< \brief (LCDCA) Blink Configuration Register */
#define REG_LCDCA_CSRCFG           (0x4008003CU) /**< \brief (LCDCA) Circular Shift Register Configuration */
#define REG_LCDCA_CMCFG            (0x40080040U) /**< \brief (LCDCA) Character Mapping Configuration Register */
#define REG_LCDCA_CMDR             (0x40080044U) /**< \brief (LCDCA) Character Mapping Data Register */
#define REG_LCDCA_ACMCFG           (0x40080048U) /**< \brief (LCDCA) Automated Character Mapping Configuration Register */
#define REG_LCDCA_ACMDR            (0x4008004CU) /**< \brief (LCDCA) Automated Character Mapping Data Register */
#define REG_LCDCA_ABMCFG           (0x40080050U) /**< \brief (LCDCA) Automated Bit Mapping Configuration Register */
#define REG_LCDCA_ABMDR            (0x40080054U) /**< \brief (LCDCA) Automated Bit Mapping Data Register */
#define REG_LCDCA_IER              (0x40080058U) /**< \brief (LCDCA) Interrupt Enable Register */
#define REG_LCDCA_IDR              (0x4008005CU) /**< \brief (LCDCA) Interrupt Disable Register */
#define REG_LCDCA_IMR              (0x40080060U) /**< \brief (LCDCA) Interrupt Mask Register */
#define REG_LCDCA_VERSION          (0x40080064U) /**< \brief (LCDCA) Version Register */
#else
#define REG_LCDCA_CR               (*(WoReg  *)0x40080000U) /**< \brief (LCDCA) Control Register */
#define REG_LCDCA_CFG              (*(RwReg  *)0x40080004U) /**< \brief (LCDCA) Configuration Register */
#define REG_LCDCA_TIM              (*(RwReg  *)0x40080008U) /**< \brief (LCDCA) Timing Register */
#define REG_LCDCA_SR               (*(RoReg  *)0x4008000CU) /**< \brief (LCDCA) Status Register */
#define REG_LCDCA_SCR              (*(WoReg  *)0x40080010U) /**< \brief (LCDCA) Status Clear Register */
#define REG_LCDCA_DRL0             (*(RwReg  *)0x40080014U) /**< \brief (LCDCA) Data Register Low 0 */
#define REG_LCDCA_DRH0             (*(RwReg  *)0x40080018U) /**< \brief (LCDCA) Data Register High 0 */
#define REG_LCDCA_DRL1             (*(RwReg  *)0x4008001CU) /**< \brief (LCDCA) Data Register Low 1 */
#define REG_LCDCA_DRH1             (*(RwReg  *)0x40080020U) /**< \brief (LCDCA) Data Register High 1 */
#define REG_LCDCA_DRL2             (*(RwReg  *)0x40080024U) /**< \brief (LCDCA) Data Register Low 2 */
#define REG_LCDCA_DRH2             (*(RwReg  *)0x40080028U) /**< \brief (LCDCA) Data Register High 2 */
#define REG_LCDCA_DRL3             (*(RwReg  *)0x4008002CU) /**< \brief (LCDCA) Data Register Low 3 */
#define REG_LCDCA_DRH3             (*(RwReg  *)0x40080030U) /**< \brief (LCDCA) Data Register High 3 */
#define REG_LCDCA_IADR             (*(WoReg  *)0x40080034U) /**< \brief (LCDCA) Indirect Access Data Register */
#define REG_LCDCA_BCFG             (*(RwReg  *)0x40080038U) /**< \brief (LCDCA) Blink Configuration Register */
#define REG_LCDCA_CSRCFG           (*(RwReg  *)0x4008003CU) /**< \brief (LCDCA) Circular Shift Register Configuration */
#define REG_LCDCA_CMCFG            (*(RwReg  *)0x40080040U) /**< \brief (LCDCA) Character Mapping Configuration Register */
#define REG_LCDCA_CMDR             (*(WoReg  *)0x40080044U) /**< \brief (LCDCA) Character Mapping Data Register */
#define REG_LCDCA_ACMCFG           (*(RwReg  *)0x40080048U) /**< \brief (LCDCA) Automated Character Mapping Configuration Register */
#define REG_LCDCA_ACMDR            (*(WoReg  *)0x4008004CU) /**< \brief (LCDCA) Automated Character Mapping Data Register */
#define REG_LCDCA_ABMCFG           (*(RwReg  *)0x40080050U) /**< \brief (LCDCA) Automated Bit Mapping Configuration Register */
#define REG_LCDCA_ABMDR            (*(WoReg  *)0x40080054U) /**< \brief (LCDCA) Automated Bit Mapping Data Register */
#define REG_LCDCA_IER              (*(WoReg  *)0x40080058U) /**< \brief (LCDCA) Interrupt Enable Register */
#define REG_LCDCA_IDR              (*(WoReg  *)0x4008005CU) /**< \brief (LCDCA) Interrupt Disable Register */
#define REG_LCDCA_IMR              (*(RoReg  *)0x40080060U) /**< \brief (LCDCA) Interrupt Mask Register */
#define REG_LCDCA_VERSION          (*(RoReg  *)0x40080064U) /**< \brief (LCDCA) Version Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/* ========== Instance parameters for LCDCA peripheral ========== */
#define LCDCA_PDCA_ID_TX_ABM        38
#define LCDCA_PDCA_ID_TX_ACM        37

#endif /* _SAM4L_LCDCA_INSTANCE_ */
