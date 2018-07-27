/**
 * \file
 *
 * \brief Instance description for FREQM
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

#ifndef _SAM4L_FREQM_INSTANCE_
#define _SAM4L_FREQM_INSTANCE_

/* ========== Register definition for FREQM peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_FREQM_CTRL             (0x400E0C00U) /**< \brief (FREQM) Control register */
#define REG_FREQM_MODE             (0x400E0C04U) /**< \brief (FREQM) Mode  register */
#define REG_FREQM_STATUS           (0x400E0C08U) /**< \brief (FREQM) Status  register */
#define REG_FREQM_VALUE            (0x400E0C0CU) /**< \brief (FREQM) Value register */
#define REG_FREQM_IER              (0x400E0C10U) /**< \brief (FREQM) Interrupt Enable Register */
#define REG_FREQM_IDR              (0x400E0C14U) /**< \brief (FREQM) Interrupt Diable Register */
#define REG_FREQM_IMR              (0x400E0C18U) /**< \brief (FREQM) Interrupt Mask Register */
#define REG_FREQM_ISR              (0x400E0C1CU) /**< \brief (FREQM) Interrupt Status Register */
#define REG_FREQM_ICR              (0x400E0C20U) /**< \brief (FREQM) Interrupt Clear Register */
#define REG_FREQM_VERSION          (0x400E0FFCU) /**< \brief (FREQM) Version Register */
#else
#define REG_FREQM_CTRL             (*(WoReg  *)0x400E0C00U) /**< \brief (FREQM) Control register */
#define REG_FREQM_MODE             (*(RwReg  *)0x400E0C04U) /**< \brief (FREQM) Mode  register */
#define REG_FREQM_STATUS           (*(RoReg  *)0x400E0C08U) /**< \brief (FREQM) Status  register */
#define REG_FREQM_VALUE            (*(RoReg  *)0x400E0C0CU) /**< \brief (FREQM) Value register */
#define REG_FREQM_IER              (*(WoReg  *)0x400E0C10U) /**< \brief (FREQM) Interrupt Enable Register */
#define REG_FREQM_IDR              (*(WoReg  *)0x400E0C14U) /**< \brief (FREQM) Interrupt Diable Register */
#define REG_FREQM_IMR              (*(RoReg  *)0x400E0C18U) /**< \brief (FREQM) Interrupt Mask Register */
#define REG_FREQM_ISR              (*(RoReg  *)0x400E0C1CU) /**< \brief (FREQM) Interrupt Status Register */
#define REG_FREQM_ICR              (*(WoReg  *)0x400E0C20U) /**< \brief (FREQM) Interrupt Clear Register */
#define REG_FREQM_VERSION          (*(RoReg  *)0x400E0FFCU) /**< \brief (FREQM) Version Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/* ========== Instance parameters for FREQM peripheral ========== */
#define FREQM_CPU                   0
#define FREQM_CRIPOSC               10
#define FREQM_DFLL0                 9
#define FREQM_FLO_JITTER            27
#define FREQM_GENCLK0               11
#define FREQM_GENCLK1               12
#define FREQM_GENCLK2               13
#define FREQM_GENCLK3               14
#define FREQM_GENCLK4               15
#define FREQM_GENCLK5               16
#define FREQM_GENCLK6               17
#define FREQM_GENCLK7               18
#define FREQM_GENCLK8               19
#define FREQM_GENCLK9               20
#define FREQM_GENCLK10              21
#define FREQM_HSB                   1
#define FREQM_NUM_CLK               28
#define FREQM_NUM_REF_CLK           4
#define FREQM_OSC0                  6
#define FREQM_OSC32                 7
#define FREQM_PBA                   2
#define FREQM_PBB                   3
#define FREQM_PBC                   4
#define FREQM_PLL0                  26
#define FREQM_RCFAST                24
#define FREQM_RCOSC                 8
#define FREQM_RC1M                  25
#define FREQM_RC32K                 7
#define FREQM_RC80M                 23
#define FREQM_REFSEL_BITS           2
#define FREQM_REF_OSC32             1
#define FREQM_REF_RCOSC             0

#endif /* _SAM4L_FREQM_INSTANCE_ */
