/**
 * \file
 *
 * \brief Instance description for TWIS1
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

#ifndef _SAM4L_TWIS1_INSTANCE_
#define _SAM4L_TWIS1_INSTANCE_

/* ========== Register definition for TWIS1 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_TWIS1_CR               (0x4001C400U) /**< \brief (TWIS1) Control Register */
#define REG_TWIS1_NBYTES           (0x4001C404U) /**< \brief (TWIS1) NBYTES Register */
#define REG_TWIS1_TR               (0x4001C408U) /**< \brief (TWIS1) Timing Register */
#define REG_TWIS1_RHR              (0x4001C40CU) /**< \brief (TWIS1) Receive Holding Register */
#define REG_TWIS1_THR              (0x4001C410U) /**< \brief (TWIS1) Transmit Holding Register */
#define REG_TWIS1_PECR             (0x4001C414U) /**< \brief (TWIS1) Packet Error Check Register */
#define REG_TWIS1_SR               (0x4001C418U) /**< \brief (TWIS1) Status Register */
#define REG_TWIS1_IER              (0x4001C41CU) /**< \brief (TWIS1) Interrupt Enable Register */
#define REG_TWIS1_IDR              (0x4001C420U) /**< \brief (TWIS1) Interrupt Disable Register */
#define REG_TWIS1_IMR              (0x4001C424U) /**< \brief (TWIS1) Interrupt Mask Register */
#define REG_TWIS1_SCR              (0x4001C428U) /**< \brief (TWIS1) Status Clear Register */
#define REG_TWIS1_PR               (0x4001C42CU) /**< \brief (TWIS1) Parameter Register */
#define REG_TWIS1_VR               (0x4001C430U) /**< \brief (TWIS1) Version Register */
#define REG_TWIS1_HSTR             (0x4001C434U) /**< \brief (TWIS1) HS-mode Timing Register */
#define REG_TWIS1_SRR              (0x4001C438U) /**< \brief (TWIS1) Slew Rate Register */
#define REG_TWIS1_HSSRR            (0x4001C43CU) /**< \brief (TWIS1) HS-mode Slew Rate Register */
#else
#define REG_TWIS1_CR               (*(RwReg  *)0x4001C400U) /**< \brief (TWIS1) Control Register */
#define REG_TWIS1_NBYTES           (*(RwReg  *)0x4001C404U) /**< \brief (TWIS1) NBYTES Register */
#define REG_TWIS1_TR               (*(RwReg  *)0x4001C408U) /**< \brief (TWIS1) Timing Register */
#define REG_TWIS1_RHR              (*(RoReg  *)0x4001C40CU) /**< \brief (TWIS1) Receive Holding Register */
#define REG_TWIS1_THR              (*(WoReg  *)0x4001C410U) /**< \brief (TWIS1) Transmit Holding Register */
#define REG_TWIS1_PECR             (*(RoReg  *)0x4001C414U) /**< \brief (TWIS1) Packet Error Check Register */
#define REG_TWIS1_SR               (*(RoReg  *)0x4001C418U) /**< \brief (TWIS1) Status Register */
#define REG_TWIS1_IER              (*(WoReg  *)0x4001C41CU) /**< \brief (TWIS1) Interrupt Enable Register */
#define REG_TWIS1_IDR              (*(WoReg  *)0x4001C420U) /**< \brief (TWIS1) Interrupt Disable Register */
#define REG_TWIS1_IMR              (*(RoReg  *)0x4001C424U) /**< \brief (TWIS1) Interrupt Mask Register */
#define REG_TWIS1_SCR              (*(WoReg  *)0x4001C428U) /**< \brief (TWIS1) Status Clear Register */
#define REG_TWIS1_PR               (*(RoReg  *)0x4001C42CU) /**< \brief (TWIS1) Parameter Register */
#define REG_TWIS1_VR               (*(RoReg  *)0x4001C430U) /**< \brief (TWIS1) Version Register */
#define REG_TWIS1_HSTR             (*(RwReg  *)0x4001C434U) /**< \brief (TWIS1) HS-mode Timing Register */
#define REG_TWIS1_SRR              (*(RwReg  *)0x4001C438U) /**< \brief (TWIS1) Slew Rate Register */
#define REG_TWIS1_HSSRR            (*(RwReg  *)0x4001C43CU) /**< \brief (TWIS1) HS-mode Slew Rate Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/* ========== Instance parameters for TWIS1 peripheral ========== */
#define TWIS1_PDCA_ID_RX            10
#define TWIS1_PDCA_ID_TX            28

#endif /* _SAM4L_TWIS1_INSTANCE_ */
