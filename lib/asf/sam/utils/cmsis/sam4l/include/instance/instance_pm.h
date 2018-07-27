/**
 * \file
 *
 * \brief Instance description for PM
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

#ifndef _SAM4L_PM_INSTANCE_
#define _SAM4L_PM_INSTANCE_

/* ========== Register definition for PM peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_PM_MCCTRL              (0x400E0000U) /**< \brief (PM) Main Clock Control */
#define REG_PM_CPUSEL              (0x400E0004U) /**< \brief (PM) CPU Clock Select */
#define REG_PM_PBASEL              (0x400E000CU) /**< \brief (PM) PBA Clock Select */
#define REG_PM_PBBSEL              (0x400E0010U) /**< \brief (PM) PBB Clock Select */
#define REG_PM_PBCSEL              (0x400E0014U) /**< \brief (PM) PBC Clock Select */
#define REG_PM_PBDSEL              (0x400E0018U) /**< \brief (PM) PBD Clock Select */
#define REG_PM_CPUMASK             (0x400E0020U) /**< \brief (PM) CPU Mask */
#define REG_PM_HSBMASK             (0x400E0024U) /**< \brief (PM) HSB Mask */
#define REG_PM_PBAMASK             (0x400E0028U) /**< \brief (PM) PBA Mask */
#define REG_PM_PBBMASK             (0x400E002CU) /**< \brief (PM) PBB Mask */
#define REG_PM_PBCMASK             (0x400E0030U) /**< \brief (PM) PBC Mask */
#define REG_PM_PBDMASK             (0x400E0034U) /**< \brief (PM) PBD Mask */
#define REG_PM_PBADIVMASK          (0x400E0040U) /**< \brief (PM) PBA Divided Clock Mask */
#define REG_PM_CFDCTRL             (0x400E0054U) /**< \brief (PM) Clock Failure Detector Control */
#define REG_PM_UNLOCK              (0x400E0058U) /**< \brief (PM) Unlock Register */
#define REG_PM_IER                 (0x400E00C0U) /**< \brief (PM) Interrupt Enable Register */
#define REG_PM_IDR                 (0x400E00C4U) /**< \brief (PM) Interrupt Disable Register */
#define REG_PM_IMR                 (0x400E00C8U) /**< \brief (PM) Interrupt Mask Register */
#define REG_PM_ISR                 (0x400E00CCU) /**< \brief (PM) Interrupt Status Register */
#define REG_PM_ICR                 (0x400E00D0U) /**< \brief (PM) Interrupt Clear Register */
#define REG_PM_SR                  (0x400E00D4U) /**< \brief (PM) Status Register */
#define REG_PM_PPCR                (0x400E0160U) /**< \brief (PM) Peripheral Power Control Register */
#define REG_PM_RCAUSE              (0x400E0180U) /**< \brief (PM) Reset Cause Register */
#define REG_PM_WCAUSE              (0x400E0184U) /**< \brief (PM) Wake Cause Register */
#define REG_PM_AWEN                (0x400E0188U) /**< \brief (PM) Asynchronous Wake Enable */
#define REG_PM_OBS                 (0x400E0190U) /**< \brief (PM) Obsvervability */
#define REG_PM_FASTSLEEP           (0x400E0194U) /**< \brief (PM) Fast Sleep Register */
#define REG_PM_CONFIG              (0x400E03F8U) /**< \brief (PM) Configuration Register */
#define REG_PM_VERSION             (0x400E03FCU) /**< \brief (PM) Version Register */
#else
#define REG_PM_MCCTRL              (*(RwReg  *)0x400E0000U) /**< \brief (PM) Main Clock Control */
#define REG_PM_CPUSEL              (*(RwReg  *)0x400E0004U) /**< \brief (PM) CPU Clock Select */
#define REG_PM_PBASEL              (*(RwReg  *)0x400E000CU) /**< \brief (PM) PBA Clock Select */
#define REG_PM_PBBSEL              (*(RwReg  *)0x400E0010U) /**< \brief (PM) PBB Clock Select */
#define REG_PM_PBCSEL              (*(RwReg  *)0x400E0014U) /**< \brief (PM) PBC Clock Select */
#define REG_PM_PBDSEL              (*(RwReg  *)0x400E0018U) /**< \brief (PM) PBD Clock Select */
#define REG_PM_CPUMASK             (*(RwReg  *)0x400E0020U) /**< \brief (PM) CPU Mask */
#define REG_PM_HSBMASK             (*(RwReg  *)0x400E0024U) /**< \brief (PM) HSB Mask */
#define REG_PM_PBAMASK             (*(RwReg  *)0x400E0028U) /**< \brief (PM) PBA Mask */
#define REG_PM_PBBMASK             (*(RwReg  *)0x400E002CU) /**< \brief (PM) PBB Mask */
#define REG_PM_PBCMASK             (*(RwReg  *)0x400E0030U) /**< \brief (PM) PBC Mask */
#define REG_PM_PBDMASK             (*(RwReg  *)0x400E0034U) /**< \brief (PM) PBD Mask */
#define REG_PM_PBADIVMASK          (*(RwReg  *)0x400E0040U) /**< \brief (PM) PBA Divided Clock Mask */
#define REG_PM_CFDCTRL             (*(RwReg  *)0x400E0054U) /**< \brief (PM) Clock Failure Detector Control */
#define REG_PM_UNLOCK              (*(WoReg  *)0x400E0058U) /**< \brief (PM) Unlock Register */
#define REG_PM_IER                 (*(WoReg  *)0x400E00C0U) /**< \brief (PM) Interrupt Enable Register */
#define REG_PM_IDR                 (*(WoReg  *)0x400E00C4U) /**< \brief (PM) Interrupt Disable Register */
#define REG_PM_IMR                 (*(RoReg  *)0x400E00C8U) /**< \brief (PM) Interrupt Mask Register */
#define REG_PM_ISR                 (*(RoReg  *)0x400E00CCU) /**< \brief (PM) Interrupt Status Register */
#define REG_PM_ICR                 (*(WoReg  *)0x400E00D0U) /**< \brief (PM) Interrupt Clear Register */
#define REG_PM_SR                  (*(RoReg  *)0x400E00D4U) /**< \brief (PM) Status Register */
#define REG_PM_PPCR                (*(RwReg  *)0x400E0160U) /**< \brief (PM) Peripheral Power Control Register */
#define REG_PM_RCAUSE              (*(RoReg  *)0x400E0180U) /**< \brief (PM) Reset Cause Register */
#define REG_PM_WCAUSE              (*(RoReg  *)0x400E0184U) /**< \brief (PM) Wake Cause Register */
#define REG_PM_AWEN                (*(RwReg  *)0x400E0188U) /**< \brief (PM) Asynchronous Wake Enable */
#define REG_PM_OBS                 (*(RwReg  *)0x400E0190U) /**< \brief (PM) Obsvervability */
#define REG_PM_FASTSLEEP           (*(RwReg  *)0x400E0194U) /**< \brief (PM) Fast Sleep Register */
#define REG_PM_CONFIG              (*(RoReg  *)0x400E03F8U) /**< \brief (PM) Configuration Register */
#define REG_PM_VERSION             (*(RoReg  *)0x400E03FCU) /**< \brief (PM) Version Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/* ========== Instance parameters for PM peripheral ========== */
#define PM_AWEN_LCDCA               7
#define PM_AWEN_PICOUART            6
#define PM_AWEN_TWIS0               0
#define PM_AWEN_TWIS1               1
#define PM_AWEN_USBC                2
#define PM_MCCTRL_MCSEL_DFLL0       3
#define PM_MCCTRL_MCSEL_FLO         7
#define PM_MCCTRL_MCSEL_OSC0        1
#define PM_MCCTRL_MCSEL_PLL0        2
#define PM_MCCTRL_MCSEL_RCFAST      5
#define PM_MCCTRL_MCSEL_RC1M        6
#define PM_MCCTRL_MCSEL_RC80M       4
#define PM_MCCTRL_MCSEL_SLOW        0
#define PM_PM_CLK_APB_NUM           4
#define PM_PPCR_FLASH_WAIT_BGREF_MASK 512
#define PM_PPCR_FLASH_WAIT_BOD18_MASK 1024
#define PM_PM_SMODE_DEEPSTOP        4
#define PM_PM_SMODE_FROZEN          1
#define PM_PM_SMODE_IDLE            0
#define PM_PM_SMODE_SHUTDOWN        6
#define PM_PM_SMODE_STANDBY         2
#define PM_PM_SMODE_STATIC          5
#define PM_PM_SMODE_STOP            3

#endif /* _SAM4L_PM_INSTANCE_ */
