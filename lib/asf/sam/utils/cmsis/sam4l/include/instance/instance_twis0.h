/**
 * \file
 *
 * \brief Instance description for TWIS0
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

#ifndef _SAM4L_TWIS0_INSTANCE_
#define _SAM4L_TWIS0_INSTANCE_

/* ========== Register definition for TWIS0 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_TWIS0_CR               (0x40018400U) /**< \brief (TWIS0) Control Register */
#define REG_TWIS0_NBYTES           (0x40018404U) /**< \brief (TWIS0) NBYTES Register */
#define REG_TWIS0_TR               (0x40018408U) /**< \brief (TWIS0) Timing Register */
#define REG_TWIS0_RHR              (0x4001840CU) /**< \brief (TWIS0) Receive Holding Register */
#define REG_TWIS0_THR              (0x40018410U) /**< \brief (TWIS0) Transmit Holding Register */
#define REG_TWIS0_PECR             (0x40018414U) /**< \brief (TWIS0) Packet Error Check Register */
#define REG_TWIS0_SR               (0x40018418U) /**< \brief (TWIS0) Status Register */
#define REG_TWIS0_IER              (0x4001841CU) /**< \brief (TWIS0) Interrupt Enable Register */
#define REG_TWIS0_IDR              (0x40018420U) /**< \brief (TWIS0) Interrupt Disable Register */
#define REG_TWIS0_IMR              (0x40018424U) /**< \brief (TWIS0) Interrupt Mask Register */
#define REG_TWIS0_SCR              (0x40018428U) /**< \brief (TWIS0) Status Clear Register */
#define REG_TWIS0_PR               (0x4001842CU) /**< \brief (TWIS0) Parameter Register */
#define REG_TWIS0_VR               (0x40018430U) /**< \brief (TWIS0) Version Register */
#define REG_TWIS0_HSTR             (0x40018434U) /**< \brief (TWIS0) HS-mode Timing Register */
#define REG_TWIS0_SRR              (0x40018438U) /**< \brief (TWIS0) Slew Rate Register */
#define REG_TWIS0_HSSRR            (0x4001843CU) /**< \brief (TWIS0) HS-mode Slew Rate Register */
#else
#define REG_TWIS0_CR               (*(RwReg  *)0x40018400U) /**< \brief (TWIS0) Control Register */
#define REG_TWIS0_NBYTES           (*(RwReg  *)0x40018404U) /**< \brief (TWIS0) NBYTES Register */
#define REG_TWIS0_TR               (*(RwReg  *)0x40018408U) /**< \brief (TWIS0) Timing Register */
#define REG_TWIS0_RHR              (*(RoReg  *)0x4001840CU) /**< \brief (TWIS0) Receive Holding Register */
#define REG_TWIS0_THR              (*(WoReg  *)0x40018410U) /**< \brief (TWIS0) Transmit Holding Register */
#define REG_TWIS0_PECR             (*(RoReg  *)0x40018414U) /**< \brief (TWIS0) Packet Error Check Register */
#define REG_TWIS0_SR               (*(RoReg  *)0x40018418U) /**< \brief (TWIS0) Status Register */
#define REG_TWIS0_IER              (*(WoReg  *)0x4001841CU) /**< \brief (TWIS0) Interrupt Enable Register */
#define REG_TWIS0_IDR              (*(WoReg  *)0x40018420U) /**< \brief (TWIS0) Interrupt Disable Register */
#define REG_TWIS0_IMR              (*(RoReg  *)0x40018424U) /**< \brief (TWIS0) Interrupt Mask Register */
#define REG_TWIS0_SCR              (*(WoReg  *)0x40018428U) /**< \brief (TWIS0) Status Clear Register */
#define REG_TWIS0_PR               (*(RoReg  *)0x4001842CU) /**< \brief (TWIS0) Parameter Register */
#define REG_TWIS0_VR               (*(RoReg  *)0x40018430U) /**< \brief (TWIS0) Version Register */
#define REG_TWIS0_HSTR             (*(RwReg  *)0x40018434U) /**< \brief (TWIS0) HS-mode Timing Register */
#define REG_TWIS0_SRR              (*(RwReg  *)0x40018438U) /**< \brief (TWIS0) Slew Rate Register */
#define REG_TWIS0_HSSRR            (*(RwReg  *)0x4001843CU) /**< \brief (TWIS0) HS-mode Slew Rate Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/* ========== Instance parameters for TWIS0 peripheral ========== */
#define TWIS0_PDCA_ID_RX            9
#define TWIS0_PDCA_ID_TX            27

#endif /* _SAM4L_TWIS0_INSTANCE_ */
