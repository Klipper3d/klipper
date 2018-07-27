/**
 * \file
 *
 * \brief Instance description for TWIM0
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

#ifndef _SAM4L_TWIM0_INSTANCE_
#define _SAM4L_TWIM0_INSTANCE_

/* ========== Register definition for TWIM0 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_TWIM0_CR               (0x40018000U) /**< \brief (TWIM0) Control Register */
#define REG_TWIM0_CWGR             (0x40018004U) /**< \brief (TWIM0) Clock Waveform Generator Register */
#define REG_TWIM0_SMBTR            (0x40018008U) /**< \brief (TWIM0) SMBus Timing Register */
#define REG_TWIM0_CMDR             (0x4001800CU) /**< \brief (TWIM0) Command Register */
#define REG_TWIM0_NCMDR            (0x40018010U) /**< \brief (TWIM0) Next Command Register */
#define REG_TWIM0_RHR              (0x40018014U) /**< \brief (TWIM0) Receive Holding Register */
#define REG_TWIM0_THR              (0x40018018U) /**< \brief (TWIM0) Transmit Holding Register */
#define REG_TWIM0_SR               (0x4001801CU) /**< \brief (TWIM0) Status Register */
#define REG_TWIM0_IER              (0x40018020U) /**< \brief (TWIM0) Interrupt Enable Register */
#define REG_TWIM0_IDR              (0x40018024U) /**< \brief (TWIM0) Interrupt Disable Register */
#define REG_TWIM0_IMR              (0x40018028U) /**< \brief (TWIM0) Interrupt Mask Register */
#define REG_TWIM0_SCR              (0x4001802CU) /**< \brief (TWIM0) Status Clear Register */
#define REG_TWIM0_PR               (0x40018030U) /**< \brief (TWIM0) Parameter Register */
#define REG_TWIM0_VR               (0x40018034U) /**< \brief (TWIM0) Version Register */
#define REG_TWIM0_HSCWGR           (0x40018038U) /**< \brief (TWIM0) HS-mode Clock Waveform Generator */
#define REG_TWIM0_SRR              (0x4001803CU) /**< \brief (TWIM0) Slew Rate Register */
#define REG_TWIM0_HSSRR            (0x40018040U) /**< \brief (TWIM0) HS-mode Slew Rate Register */
#else
#define REG_TWIM0_CR               (*(WoReg  *)0x40018000U) /**< \brief (TWIM0) Control Register */
#define REG_TWIM0_CWGR             (*(RwReg  *)0x40018004U) /**< \brief (TWIM0) Clock Waveform Generator Register */
#define REG_TWIM0_SMBTR            (*(RwReg  *)0x40018008U) /**< \brief (TWIM0) SMBus Timing Register */
#define REG_TWIM0_CMDR             (*(RwReg  *)0x4001800CU) /**< \brief (TWIM0) Command Register */
#define REG_TWIM0_NCMDR            (*(RwReg  *)0x40018010U) /**< \brief (TWIM0) Next Command Register */
#define REG_TWIM0_RHR              (*(RoReg  *)0x40018014U) /**< \brief (TWIM0) Receive Holding Register */
#define REG_TWIM0_THR              (*(WoReg  *)0x40018018U) /**< \brief (TWIM0) Transmit Holding Register */
#define REG_TWIM0_SR               (*(RoReg  *)0x4001801CU) /**< \brief (TWIM0) Status Register */
#define REG_TWIM0_IER              (*(WoReg  *)0x40018020U) /**< \brief (TWIM0) Interrupt Enable Register */
#define REG_TWIM0_IDR              (*(WoReg  *)0x40018024U) /**< \brief (TWIM0) Interrupt Disable Register */
#define REG_TWIM0_IMR              (*(RoReg  *)0x40018028U) /**< \brief (TWIM0) Interrupt Mask Register */
#define REG_TWIM0_SCR              (*(WoReg  *)0x4001802CU) /**< \brief (TWIM0) Status Clear Register */
#define REG_TWIM0_PR               (*(RoReg  *)0x40018030U) /**< \brief (TWIM0) Parameter Register */
#define REG_TWIM0_VR               (*(RoReg  *)0x40018034U) /**< \brief (TWIM0) Version Register */
#define REG_TWIM0_HSCWGR           (*(RwReg  *)0x40018038U) /**< \brief (TWIM0) HS-mode Clock Waveform Generator */
#define REG_TWIM0_SRR              (*(RwReg  *)0x4001803CU) /**< \brief (TWIM0) Slew Rate Register */
#define REG_TWIM0_HSSRR            (*(RwReg  *)0x40018040U) /**< \brief (TWIM0) HS-mode Slew Rate Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/* ========== Instance parameters for TWIM0 peripheral ========== */
#define TWIM0_PDCA_ID_RX            5
#define TWIM0_PDCA_ID_TX            23

#endif /* _SAM4L_TWIM0_INSTANCE_ */
