/**
 * \file
 *
 * \brief Instance description for TWIM2
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

#ifndef _SAM4L_TWIM2_INSTANCE_
#define _SAM4L_TWIM2_INSTANCE_

/* ========== Register definition for TWIM2 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_TWIM2_CR               (0x40078000U) /**< \brief (TWIM2) Control Register */
#define REG_TWIM2_CWGR             (0x40078004U) /**< \brief (TWIM2) Clock Waveform Generator Register */
#define REG_TWIM2_SMBTR            (0x40078008U) /**< \brief (TWIM2) SMBus Timing Register */
#define REG_TWIM2_CMDR             (0x4007800CU) /**< \brief (TWIM2) Command Register */
#define REG_TWIM2_NCMDR            (0x40078010U) /**< \brief (TWIM2) Next Command Register */
#define REG_TWIM2_RHR              (0x40078014U) /**< \brief (TWIM2) Receive Holding Register */
#define REG_TWIM2_THR              (0x40078018U) /**< \brief (TWIM2) Transmit Holding Register */
#define REG_TWIM2_SR               (0x4007801CU) /**< \brief (TWIM2) Status Register */
#define REG_TWIM2_IER              (0x40078020U) /**< \brief (TWIM2) Interrupt Enable Register */
#define REG_TWIM2_IDR              (0x40078024U) /**< \brief (TWIM2) Interrupt Disable Register */
#define REG_TWIM2_IMR              (0x40078028U) /**< \brief (TWIM2) Interrupt Mask Register */
#define REG_TWIM2_SCR              (0x4007802CU) /**< \brief (TWIM2) Status Clear Register */
#define REG_TWIM2_PR               (0x40078030U) /**< \brief (TWIM2) Parameter Register */
#define REG_TWIM2_VR               (0x40078034U) /**< \brief (TWIM2) Version Register */
#define REG_TWIM2_HSCWGR           (0x40078038U) /**< \brief (TWIM2) HS-mode Clock Waveform Generator */
#define REG_TWIM2_SRR              (0x4007803CU) /**< \brief (TWIM2) Slew Rate Register */
#define REG_TWIM2_HSSRR            (0x40078040U) /**< \brief (TWIM2) HS-mode Slew Rate Register */
#else
#define REG_TWIM2_CR               (*(WoReg  *)0x40078000U) /**< \brief (TWIM2) Control Register */
#define REG_TWIM2_CWGR             (*(RwReg  *)0x40078004U) /**< \brief (TWIM2) Clock Waveform Generator Register */
#define REG_TWIM2_SMBTR            (*(RwReg  *)0x40078008U) /**< \brief (TWIM2) SMBus Timing Register */
#define REG_TWIM2_CMDR             (*(RwReg  *)0x4007800CU) /**< \brief (TWIM2) Command Register */
#define REG_TWIM2_NCMDR            (*(RwReg  *)0x40078010U) /**< \brief (TWIM2) Next Command Register */
#define REG_TWIM2_RHR              (*(RoReg  *)0x40078014U) /**< \brief (TWIM2) Receive Holding Register */
#define REG_TWIM2_THR              (*(WoReg  *)0x40078018U) /**< \brief (TWIM2) Transmit Holding Register */
#define REG_TWIM2_SR               (*(RoReg  *)0x4007801CU) /**< \brief (TWIM2) Status Register */
#define REG_TWIM2_IER              (*(WoReg  *)0x40078020U) /**< \brief (TWIM2) Interrupt Enable Register */
#define REG_TWIM2_IDR              (*(WoReg  *)0x40078024U) /**< \brief (TWIM2) Interrupt Disable Register */
#define REG_TWIM2_IMR              (*(RoReg  *)0x40078028U) /**< \brief (TWIM2) Interrupt Mask Register */
#define REG_TWIM2_SCR              (*(WoReg  *)0x4007802CU) /**< \brief (TWIM2) Status Clear Register */
#define REG_TWIM2_PR               (*(RoReg  *)0x40078030U) /**< \brief (TWIM2) Parameter Register */
#define REG_TWIM2_VR               (*(RoReg  *)0x40078034U) /**< \brief (TWIM2) Version Register */
#define REG_TWIM2_HSCWGR           (*(RwReg  *)0x40078038U) /**< \brief (TWIM2) HS-mode Clock Waveform Generator */
#define REG_TWIM2_SRR              (*(RwReg  *)0x4007803CU) /**< \brief (TWIM2) Slew Rate Register */
#define REG_TWIM2_HSSRR            (*(RwReg  *)0x40078040U) /**< \brief (TWIM2) HS-mode Slew Rate Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/* ========== Instance parameters for TWIM2 peripheral ========== */
#define TWIM2_PDCA_ID_RX            7
#define TWIM2_PDCA_ID_TX            25

#endif /* _SAM4L_TWIM2_INSTANCE_ */
