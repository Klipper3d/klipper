/**
 * \file
 *
 * \brief Instance description for USART3
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

#ifndef _SAM4L_USART3_INSTANCE_
#define _SAM4L_USART3_INSTANCE_

/* ========== Register definition for USART3 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_USART3_CR              (0x40030000U) /**< \brief (USART3) Control Register */
#define REG_USART3_MR              (0x40030004U) /**< \brief (USART3) Mode Register */
#define REG_USART3_IER             (0x40030008U) /**< \brief (USART3) Interrupt Enable Register */
#define REG_USART3_IDR             (0x4003000CU) /**< \brief (USART3) Interrupt Disable Register */
#define REG_USART3_IMR             (0x40030010U) /**< \brief (USART3) Interrupt Mask Register */
#define REG_USART3_CSR             (0x40030014U) /**< \brief (USART3) Channel Status Register */
#define REG_USART3_RHR             (0x40030018U) /**< \brief (USART3) Receiver Holding Register */
#define REG_USART3_THR             (0x4003001CU) /**< \brief (USART3) Transmitter Holding Register */
#define REG_USART3_BRGR            (0x40030020U) /**< \brief (USART3) Baud Rate Generator Register */
#define REG_USART3_RTOR            (0x40030024U) /**< \brief (USART3) Receiver Time-out Register */
#define REG_USART3_TTGR            (0x40030028U) /**< \brief (USART3) Transmitter Timeguard Register */
#define REG_USART3_FIDI            (0x40030040U) /**< \brief (USART3) FI DI Ratio Register */
#define REG_USART3_NER             (0x40030044U) /**< \brief (USART3) Number of Errors Register */
#define REG_USART3_IFR             (0x4003004CU) /**< \brief (USART3) IrDA Filter Register */
#define REG_USART3_MAN             (0x40030050U) /**< \brief (USART3) Manchester Configuration Register */
#define REG_USART3_LINMR           (0x40030054U) /**< \brief (USART3) LIN Mode Register */
#define REG_USART3_LINIR           (0x40030058U) /**< \brief (USART3) LIN Identifier Register */
#define REG_USART3_LINBRR          (0x4003005CU) /**< \brief (USART3) LIN Baud Rate Register */
#define REG_USART3_WPMR            (0x400300E4U) /**< \brief (USART3) Write Protect Mode Register */
#define REG_USART3_WPSR            (0x400300E8U) /**< \brief (USART3) Write Protect Status Register */
#define REG_USART3_VERSION         (0x400300FCU) /**< \brief (USART3) Version Register */
#else
#define REG_USART3_CR              (*(WoReg  *)0x40030000U) /**< \brief (USART3) Control Register */
#define REG_USART3_MR              (*(RwReg  *)0x40030004U) /**< \brief (USART3) Mode Register */
#define REG_USART3_IER             (*(WoReg  *)0x40030008U) /**< \brief (USART3) Interrupt Enable Register */
#define REG_USART3_IDR             (*(WoReg  *)0x4003000CU) /**< \brief (USART3) Interrupt Disable Register */
#define REG_USART3_IMR             (*(RoReg  *)0x40030010U) /**< \brief (USART3) Interrupt Mask Register */
#define REG_USART3_CSR             (*(RoReg  *)0x40030014U) /**< \brief (USART3) Channel Status Register */
#define REG_USART3_RHR             (*(RoReg  *)0x40030018U) /**< \brief (USART3) Receiver Holding Register */
#define REG_USART3_THR             (*(WoReg  *)0x4003001CU) /**< \brief (USART3) Transmitter Holding Register */
#define REG_USART3_BRGR            (*(RwReg  *)0x40030020U) /**< \brief (USART3) Baud Rate Generator Register */
#define REG_USART3_RTOR            (*(RwReg  *)0x40030024U) /**< \brief (USART3) Receiver Time-out Register */
#define REG_USART3_TTGR            (*(RwReg  *)0x40030028U) /**< \brief (USART3) Transmitter Timeguard Register */
#define REG_USART3_FIDI            (*(RwReg  *)0x40030040U) /**< \brief (USART3) FI DI Ratio Register */
#define REG_USART3_NER             (*(RoReg  *)0x40030044U) /**< \brief (USART3) Number of Errors Register */
#define REG_USART3_IFR             (*(RwReg  *)0x4003004CU) /**< \brief (USART3) IrDA Filter Register */
#define REG_USART3_MAN             (*(RwReg  *)0x40030050U) /**< \brief (USART3) Manchester Configuration Register */
#define REG_USART3_LINMR           (*(RwReg  *)0x40030054U) /**< \brief (USART3) LIN Mode Register */
#define REG_USART3_LINIR           (*(RwReg  *)0x40030058U) /**< \brief (USART3) LIN Identifier Register */
#define REG_USART3_LINBRR          (*(RoReg  *)0x4003005CU) /**< \brief (USART3) LIN Baud Rate Register */
#define REG_USART3_WPMR            (*(RwReg  *)0x400300E4U) /**< \brief (USART3) Write Protect Mode Register */
#define REG_USART3_WPSR            (*(RoReg  *)0x400300E8U) /**< \brief (USART3) Write Protect Status Register */
#define REG_USART3_VERSION         (*(RoReg  *)0x400300FCU) /**< \brief (USART3) Version Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/* ========== Instance parameters for USART3 peripheral ========== */
#define USART3_CLK_DIV              8
#define USART3_PDCA_ID_RX           3
#define USART3_PDCA_ID_TX           21

#endif /* _SAM4L_USART3_INSTANCE_ */
