/**
 * \file
 *
 * \brief Instance description for TWIM1
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

#ifndef _SAM4L_TWIM1_INSTANCE_
#define _SAM4L_TWIM1_INSTANCE_

/* ========== Register definition for TWIM1 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_TWIM1_CR               (0x4001C000U) /**< \brief (TWIM1) Control Register */
#define REG_TWIM1_CWGR             (0x4001C004U) /**< \brief (TWIM1) Clock Waveform Generator Register */
#define REG_TWIM1_SMBTR            (0x4001C008U) /**< \brief (TWIM1) SMBus Timing Register */
#define REG_TWIM1_CMDR             (0x4001C00CU) /**< \brief (TWIM1) Command Register */
#define REG_TWIM1_NCMDR            (0x4001C010U) /**< \brief (TWIM1) Next Command Register */
#define REG_TWIM1_RHR              (0x4001C014U) /**< \brief (TWIM1) Receive Holding Register */
#define REG_TWIM1_THR              (0x4001C018U) /**< \brief (TWIM1) Transmit Holding Register */
#define REG_TWIM1_SR               (0x4001C01CU) /**< \brief (TWIM1) Status Register */
#define REG_TWIM1_IER              (0x4001C020U) /**< \brief (TWIM1) Interrupt Enable Register */
#define REG_TWIM1_IDR              (0x4001C024U) /**< \brief (TWIM1) Interrupt Disable Register */
#define REG_TWIM1_IMR              (0x4001C028U) /**< \brief (TWIM1) Interrupt Mask Register */
#define REG_TWIM1_SCR              (0x4001C02CU) /**< \brief (TWIM1) Status Clear Register */
#define REG_TWIM1_PR               (0x4001C030U) /**< \brief (TWIM1) Parameter Register */
#define REG_TWIM1_VR               (0x4001C034U) /**< \brief (TWIM1) Version Register */
#define REG_TWIM1_HSCWGR           (0x4001C038U) /**< \brief (TWIM1) HS-mode Clock Waveform Generator */
#define REG_TWIM1_SRR              (0x4001C03CU) /**< \brief (TWIM1) Slew Rate Register */
#define REG_TWIM1_HSSRR            (0x4001C040U) /**< \brief (TWIM1) HS-mode Slew Rate Register */
#else
#define REG_TWIM1_CR               (*(WoReg  *)0x4001C000U) /**< \brief (TWIM1) Control Register */
#define REG_TWIM1_CWGR             (*(RwReg  *)0x4001C004U) /**< \brief (TWIM1) Clock Waveform Generator Register */
#define REG_TWIM1_SMBTR            (*(RwReg  *)0x4001C008U) /**< \brief (TWIM1) SMBus Timing Register */
#define REG_TWIM1_CMDR             (*(RwReg  *)0x4001C00CU) /**< \brief (TWIM1) Command Register */
#define REG_TWIM1_NCMDR            (*(RwReg  *)0x4001C010U) /**< \brief (TWIM1) Next Command Register */
#define REG_TWIM1_RHR              (*(RoReg  *)0x4001C014U) /**< \brief (TWIM1) Receive Holding Register */
#define REG_TWIM1_THR              (*(WoReg  *)0x4001C018U) /**< \brief (TWIM1) Transmit Holding Register */
#define REG_TWIM1_SR               (*(RoReg  *)0x4001C01CU) /**< \brief (TWIM1) Status Register */
#define REG_TWIM1_IER              (*(WoReg  *)0x4001C020U) /**< \brief (TWIM1) Interrupt Enable Register */
#define REG_TWIM1_IDR              (*(WoReg  *)0x4001C024U) /**< \brief (TWIM1) Interrupt Disable Register */
#define REG_TWIM1_IMR              (*(RoReg  *)0x4001C028U) /**< \brief (TWIM1) Interrupt Mask Register */
#define REG_TWIM1_SCR              (*(WoReg  *)0x4001C02CU) /**< \brief (TWIM1) Status Clear Register */
#define REG_TWIM1_PR               (*(RoReg  *)0x4001C030U) /**< \brief (TWIM1) Parameter Register */
#define REG_TWIM1_VR               (*(RoReg  *)0x4001C034U) /**< \brief (TWIM1) Version Register */
#define REG_TWIM1_HSCWGR           (*(RwReg  *)0x4001C038U) /**< \brief (TWIM1) HS-mode Clock Waveform Generator */
#define REG_TWIM1_SRR              (*(RwReg  *)0x4001C03CU) /**< \brief (TWIM1) Slew Rate Register */
#define REG_TWIM1_HSSRR            (*(RwReg  *)0x4001C040U) /**< \brief (TWIM1) HS-mode Slew Rate Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/* ========== Instance parameters for TWIM1 peripheral ========== */
#define TWIM1_PDCA_ID_RX            6
#define TWIM1_PDCA_ID_TX            24

#endif /* _SAM4L_TWIM1_INSTANCE_ */
