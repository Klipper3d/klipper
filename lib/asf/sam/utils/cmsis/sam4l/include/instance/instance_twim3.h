/**
 * \file
 *
 * \brief Instance description for TWIM3
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

#ifndef _SAM4L_TWIM3_INSTANCE_
#define _SAM4L_TWIM3_INSTANCE_

/* ========== Register definition for TWIM3 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_TWIM3_CR               (0x4007C000U) /**< \brief (TWIM3) Control Register */
#define REG_TWIM3_CWGR             (0x4007C004U) /**< \brief (TWIM3) Clock Waveform Generator Register */
#define REG_TWIM3_SMBTR            (0x4007C008U) /**< \brief (TWIM3) SMBus Timing Register */
#define REG_TWIM3_CMDR             (0x4007C00CU) /**< \brief (TWIM3) Command Register */
#define REG_TWIM3_NCMDR            (0x4007C010U) /**< \brief (TWIM3) Next Command Register */
#define REG_TWIM3_RHR              (0x4007C014U) /**< \brief (TWIM3) Receive Holding Register */
#define REG_TWIM3_THR              (0x4007C018U) /**< \brief (TWIM3) Transmit Holding Register */
#define REG_TWIM3_SR               (0x4007C01CU) /**< \brief (TWIM3) Status Register */
#define REG_TWIM3_IER              (0x4007C020U) /**< \brief (TWIM3) Interrupt Enable Register */
#define REG_TWIM3_IDR              (0x4007C024U) /**< \brief (TWIM3) Interrupt Disable Register */
#define REG_TWIM3_IMR              (0x4007C028U) /**< \brief (TWIM3) Interrupt Mask Register */
#define REG_TWIM3_SCR              (0x4007C02CU) /**< \brief (TWIM3) Status Clear Register */
#define REG_TWIM3_PR               (0x4007C030U) /**< \brief (TWIM3) Parameter Register */
#define REG_TWIM3_VR               (0x4007C034U) /**< \brief (TWIM3) Version Register */
#define REG_TWIM3_HSCWGR           (0x4007C038U) /**< \brief (TWIM3) HS-mode Clock Waveform Generator */
#define REG_TWIM3_SRR              (0x4007C03CU) /**< \brief (TWIM3) Slew Rate Register */
#define REG_TWIM3_HSSRR            (0x4007C040U) /**< \brief (TWIM3) HS-mode Slew Rate Register */
#else
#define REG_TWIM3_CR               (*(WoReg  *)0x4007C000U) /**< \brief (TWIM3) Control Register */
#define REG_TWIM3_CWGR             (*(RwReg  *)0x4007C004U) /**< \brief (TWIM3) Clock Waveform Generator Register */
#define REG_TWIM3_SMBTR            (*(RwReg  *)0x4007C008U) /**< \brief (TWIM3) SMBus Timing Register */
#define REG_TWIM3_CMDR             (*(RwReg  *)0x4007C00CU) /**< \brief (TWIM3) Command Register */
#define REG_TWIM3_NCMDR            (*(RwReg  *)0x4007C010U) /**< \brief (TWIM3) Next Command Register */
#define REG_TWIM3_RHR              (*(RoReg  *)0x4007C014U) /**< \brief (TWIM3) Receive Holding Register */
#define REG_TWIM3_THR              (*(WoReg  *)0x4007C018U) /**< \brief (TWIM3) Transmit Holding Register */
#define REG_TWIM3_SR               (*(RoReg  *)0x4007C01CU) /**< \brief (TWIM3) Status Register */
#define REG_TWIM3_IER              (*(WoReg  *)0x4007C020U) /**< \brief (TWIM3) Interrupt Enable Register */
#define REG_TWIM3_IDR              (*(WoReg  *)0x4007C024U) /**< \brief (TWIM3) Interrupt Disable Register */
#define REG_TWIM3_IMR              (*(RoReg  *)0x4007C028U) /**< \brief (TWIM3) Interrupt Mask Register */
#define REG_TWIM3_SCR              (*(WoReg  *)0x4007C02CU) /**< \brief (TWIM3) Status Clear Register */
#define REG_TWIM3_PR               (*(RoReg  *)0x4007C030U) /**< \brief (TWIM3) Parameter Register */
#define REG_TWIM3_VR               (*(RoReg  *)0x4007C034U) /**< \brief (TWIM3) Version Register */
#define REG_TWIM3_HSCWGR           (*(RwReg  *)0x4007C038U) /**< \brief (TWIM3) HS-mode Clock Waveform Generator */
#define REG_TWIM3_SRR              (*(RwReg  *)0x4007C03CU) /**< \brief (TWIM3) Slew Rate Register */
#define REG_TWIM3_HSSRR            (*(RwReg  *)0x4007C040U) /**< \brief (TWIM3) HS-mode Slew Rate Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/* ========== Instance parameters for TWIM3 peripheral ========== */
#define TWIM3_PDCA_ID_RX            8
#define TWIM3_PDCA_ID_TX            26

#endif /* _SAM4L_TWIM3_INSTANCE_ */
