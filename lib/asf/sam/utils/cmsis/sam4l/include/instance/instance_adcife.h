/**
 * \file
 *
 * \brief Instance description for ADCIFE
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

#ifndef _SAM4L_ADCIFE_INSTANCE_
#define _SAM4L_ADCIFE_INSTANCE_

/* ========== Register definition for ADCIFE peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_ADCIFE_CR              (0x40038000U) /**< \brief (ADCIFE) Control Register */
#define REG_ADCIFE_CFG             (0x40038004U) /**< \brief (ADCIFE) Configuration Register */
#define REG_ADCIFE_SR              (0x40038008U) /**< \brief (ADCIFE) Status Register */
#define REG_ADCIFE_SCR             (0x4003800CU) /**< \brief (ADCIFE) Status Clear Register */
#define REG_ADCIFE_RTS             (0x40038010U) /**< \brief (ADCIFE) Resistive Touch Screen Register */
#define REG_ADCIFE_SEQCFG          (0x40038014U) /**< \brief (ADCIFE) Sequencer Configuration Register */
#define REG_ADCIFE_CDMA            (0x40038018U) /**< \brief (ADCIFE) Configuration Direct Memory Access Register */
#define REG_ADCIFE_TIM             (0x4003801CU) /**< \brief (ADCIFE) Timing Configuration Register */
#define REG_ADCIFE_ITIMER          (0x40038020U) /**< \brief (ADCIFE) Internal Timer Register */
#define REG_ADCIFE_WCFG            (0x40038024U) /**< \brief (ADCIFE) Window Monitor Configuration Register */
#define REG_ADCIFE_WTH             (0x40038028U) /**< \brief (ADCIFE) Window Monitor Threshold Configuration Register */
#define REG_ADCIFE_LCV             (0x4003802CU) /**< \brief (ADCIFE) Sequencer Last Converted Value Register */
#define REG_ADCIFE_IER             (0x40038030U) /**< \brief (ADCIFE) Interrupt Enable Register */
#define REG_ADCIFE_IDR             (0x40038034U) /**< \brief (ADCIFE) Interrupt Disable Register */
#define REG_ADCIFE_IMR             (0x40038038U) /**< \brief (ADCIFE) Interrupt Mask Register */
#define REG_ADCIFE_CALIB           (0x4003803CU) /**< \brief (ADCIFE) Calibration Register */
#define REG_ADCIFE_VERSION         (0x40038040U) /**< \brief (ADCIFE) Version Register */
#define REG_ADCIFE_PARAMETER       (0x40038044U) /**< \brief (ADCIFE) Parameter Register */
#else
#define REG_ADCIFE_CR              (*(WoReg  *)0x40038000U) /**< \brief (ADCIFE) Control Register */
#define REG_ADCIFE_CFG             (*(RwReg  *)0x40038004U) /**< \brief (ADCIFE) Configuration Register */
#define REG_ADCIFE_SR              (*(RoReg  *)0x40038008U) /**< \brief (ADCIFE) Status Register */
#define REG_ADCIFE_SCR             (*(WoReg  *)0x4003800CU) /**< \brief (ADCIFE) Status Clear Register */
#define REG_ADCIFE_RTS             (*(RwReg  *)0x40038010U) /**< \brief (ADCIFE) Resistive Touch Screen Register */
#define REG_ADCIFE_SEQCFG          (*(RwReg  *)0x40038014U) /**< \brief (ADCIFE) Sequencer Configuration Register */
#define REG_ADCIFE_CDMA            (*(WoReg  *)0x40038018U) /**< \brief (ADCIFE) Configuration Direct Memory Access Register */
#define REG_ADCIFE_TIM             (*(RwReg  *)0x4003801CU) /**< \brief (ADCIFE) Timing Configuration Register */
#define REG_ADCIFE_ITIMER          (*(RwReg  *)0x40038020U) /**< \brief (ADCIFE) Internal Timer Register */
#define REG_ADCIFE_WCFG            (*(RwReg  *)0x40038024U) /**< \brief (ADCIFE) Window Monitor Configuration Register */
#define REG_ADCIFE_WTH             (*(RwReg  *)0x40038028U) /**< \brief (ADCIFE) Window Monitor Threshold Configuration Register */
#define REG_ADCIFE_LCV             (*(RoReg  *)0x4003802CU) /**< \brief (ADCIFE) Sequencer Last Converted Value Register */
#define REG_ADCIFE_IER             (*(WoReg  *)0x40038030U) /**< \brief (ADCIFE) Interrupt Enable Register */
#define REG_ADCIFE_IDR             (*(WoReg  *)0x40038034U) /**< \brief (ADCIFE) Interrupt Disable Register */
#define REG_ADCIFE_IMR             (*(RoReg  *)0x40038038U) /**< \brief (ADCIFE) Interrupt Mask Register */
#define REG_ADCIFE_CALIB           (*(RwReg  *)0x4003803CU) /**< \brief (ADCIFE) Calibration Register */
#define REG_ADCIFE_VERSION         (*(RoReg  *)0x40038040U) /**< \brief (ADCIFE) Version Register */
#define REG_ADCIFE_PARAMETER       (*(RoReg  *)0x40038044U) /**< \brief (ADCIFE) Parameter Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/* ========== Instance parameters for ADCIFE peripheral ========== */
#define ADCIFE_CHANNELS             16
#define ADCIFE_CHANNEL_MSB          15
#define ADCIFE_EXT_TRIGGERS_MSB     0
#define ADCIFE_GCLK_NUM             10
#define ADCIFE_PDCA_ID_RX           11
#define ADCIFE_PDCA_ID_TX           29
#define ADCIFE_REG_RX               LCV
#define ADCIFE_REG_TX               CDMA

#endif /* _SAM4L_ADCIFE_INSTANCE_ */
