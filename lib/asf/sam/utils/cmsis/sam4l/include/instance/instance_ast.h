/**
 * \file
 *
 * \brief Instance description for AST
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

#ifndef _SAM4L_AST_INSTANCE_
#define _SAM4L_AST_INSTANCE_

/* ========== Register definition for AST peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_AST_CR                 (0x400F0800U) /**< \brief (AST) Control Register */
#define REG_AST_CV                 (0x400F0804U) /**< \brief (AST) Counter Value */
#define REG_AST_SR                 (0x400F0808U) /**< \brief (AST) Status Register */
#define REG_AST_SCR                (0x400F080CU) /**< \brief (AST) Status Clear Register */
#define REG_AST_IER                (0x400F0810U) /**< \brief (AST) Interrupt Enable Register */
#define REG_AST_IDR                (0x400F0814U) /**< \brief (AST) Interrupt Disable Register */
#define REG_AST_IMR                (0x400F0818U) /**< \brief (AST) Interrupt Mask Register */
#define REG_AST_WER                (0x400F081CU) /**< \brief (AST) Wake Enable Register */
#define REG_AST_AR0                (0x400F0820U) /**< \brief (AST) Alarm Register 0 */
#define REG_AST_AR1                (0x400F0824U) /**< \brief (AST) Alarm Register 1 */
#define REG_AST_PIR0               (0x400F0830U) /**< \brief (AST) Periodic Interval Register 0 */
#define REG_AST_PIR1               (0x400F0834U) /**< \brief (AST) Periodic Interval Register 1 */
#define REG_AST_CLOCK              (0x400F0840U) /**< \brief (AST) Clock Control Register */
#define REG_AST_DTR                (0x400F0844U) /**< \brief (AST) Digital Tuner Register */
#define REG_AST_EVE                (0x400F0848U) /**< \brief (AST) Event Enable Register */
#define REG_AST_EVD                (0x400F084CU) /**< \brief (AST) Event Disable Register */
#define REG_AST_EVM                (0x400F0850U) /**< \brief (AST) Event Mask Register */
#define REG_AST_CALV               (0x400F0854U) /**< \brief (AST) Calendar Value */
#define REG_AST_PARAMETER          (0x400F08F0U) /**< \brief (AST) Parameter Register */
#define REG_AST_VERSION            (0x400F08FCU) /**< \brief (AST) Version Register */
#else
#define REG_AST_CR                 (*(RwReg  *)0x400F0800U) /**< \brief (AST) Control Register */
#define REG_AST_CV                 (*(RwReg  *)0x400F0804U) /**< \brief (AST) Counter Value */
#define REG_AST_SR                 (*(RoReg  *)0x400F0808U) /**< \brief (AST) Status Register */
#define REG_AST_SCR                (*(WoReg  *)0x400F080CU) /**< \brief (AST) Status Clear Register */
#define REG_AST_IER                (*(WoReg  *)0x400F0810U) /**< \brief (AST) Interrupt Enable Register */
#define REG_AST_IDR                (*(WoReg  *)0x400F0814U) /**< \brief (AST) Interrupt Disable Register */
#define REG_AST_IMR                (*(RoReg  *)0x400F0818U) /**< \brief (AST) Interrupt Mask Register */
#define REG_AST_WER                (*(RwReg  *)0x400F081CU) /**< \brief (AST) Wake Enable Register */
#define REG_AST_AR0                (*(RwReg  *)0x400F0820U) /**< \brief (AST) Alarm Register 0 */
#define REG_AST_AR1                (*(RwReg  *)0x400F0824U) /**< \brief (AST) Alarm Register 1 */
#define REG_AST_PIR0               (*(RwReg  *)0x400F0830U) /**< \brief (AST) Periodic Interval Register 0 */
#define REG_AST_PIR1               (*(RwReg  *)0x400F0834U) /**< \brief (AST) Periodic Interval Register 1 */
#define REG_AST_CLOCK              (*(RwReg  *)0x400F0840U) /**< \brief (AST) Clock Control Register */
#define REG_AST_DTR                (*(RwReg  *)0x400F0844U) /**< \brief (AST) Digital Tuner Register */
#define REG_AST_EVE                (*(WoReg  *)0x400F0848U) /**< \brief (AST) Event Enable Register */
#define REG_AST_EVD                (*(WoReg  *)0x400F084CU) /**< \brief (AST) Event Disable Register */
#define REG_AST_EVM                (*(RoReg  *)0x400F0850U) /**< \brief (AST) Event Mask Register */
#define REG_AST_CALV               (*(RwReg  *)0x400F0854U) /**< \brief (AST) Calendar Value */
#define REG_AST_PARAMETER          (*(RoReg  *)0x400F08F0U) /**< \brief (AST) Parameter Register */
#define REG_AST_VERSION            (*(RoReg  *)0x400F08FCU) /**< \brief (AST) Version Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/* ========== Instance parameters for AST peripheral ========== */
#define AST_CLK1K                   4
#define AST_CLK32                   1
#define AST_GCLK_NUM                2
#define AST_GENCLK                  3
#define AST_PB                      2
#define AST_RCOSC                   0

#endif /* _SAM4L_AST_INSTANCE_ */
