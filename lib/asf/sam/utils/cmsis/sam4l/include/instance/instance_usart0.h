/**
 * \file
 *
 * \brief Instance description for USART0
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

#ifndef _SAM4L_USART0_INSTANCE_
#define _SAM4L_USART0_INSTANCE_

/* ========== Register definition for USART0 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_USART0_CR              (0x40024000U) /**< \brief (USART0) Control Register */
#define REG_USART0_MR              (0x40024004U) /**< \brief (USART0) Mode Register */
#define REG_USART0_IER             (0x40024008U) /**< \brief (USART0) Interrupt Enable Register */
#define REG_USART0_IDR             (0x4002400CU) /**< \brief (USART0) Interrupt Disable Register */
#define REG_USART0_IMR             (0x40024010U) /**< \brief (USART0) Interrupt Mask Register */
#define REG_USART0_CSR             (0x40024014U) /**< \brief (USART0) Channel Status Register */
#define REG_USART0_RHR             (0x40024018U) /**< \brief (USART0) Receiver Holding Register */
#define REG_USART0_THR             (0x4002401CU) /**< \brief (USART0) Transmitter Holding Register */
#define REG_USART0_BRGR            (0x40024020U) /**< \brief (USART0) Baud Rate Generator Register */
#define REG_USART0_RTOR            (0x40024024U) /**< \brief (USART0) Receiver Time-out Register */
#define REG_USART0_TTGR            (0x40024028U) /**< \brief (USART0) Transmitter Timeguard Register */
#define REG_USART0_FIDI            (0x40024040U) /**< \brief (USART0) FI DI Ratio Register */
#define REG_USART0_NER             (0x40024044U) /**< \brief (USART0) Number of Errors Register */
#define REG_USART0_IFR             (0x4002404CU) /**< \brief (USART0) IrDA Filter Register */
#define REG_USART0_MAN             (0x40024050U) /**< \brief (USART0) Manchester Configuration Register */
#define REG_USART0_LINMR           (0x40024054U) /**< \brief (USART0) LIN Mode Register */
#define REG_USART0_LINIR           (0x40024058U) /**< \brief (USART0) LIN Identifier Register */
#define REG_USART0_LINBRR          (0x4002405CU) /**< \brief (USART0) LIN Baud Rate Register */
#define REG_USART0_WPMR            (0x400240E4U) /**< \brief (USART0) Write Protect Mode Register */
#define REG_USART0_WPSR            (0x400240E8U) /**< \brief (USART0) Write Protect Status Register */
#define REG_USART0_VERSION         (0x400240FCU) /**< \brief (USART0) Version Register */
#else
#define REG_USART0_CR              (*(WoReg  *)0x40024000U) /**< \brief (USART0) Control Register */
#define REG_USART0_MR              (*(RwReg  *)0x40024004U) /**< \brief (USART0) Mode Register */
#define REG_USART0_IER             (*(WoReg  *)0x40024008U) /**< \brief (USART0) Interrupt Enable Register */
#define REG_USART0_IDR             (*(WoReg  *)0x4002400CU) /**< \brief (USART0) Interrupt Disable Register */
#define REG_USART0_IMR             (*(RoReg  *)0x40024010U) /**< \brief (USART0) Interrupt Mask Register */
#define REG_USART0_CSR             (*(RoReg  *)0x40024014U) /**< \brief (USART0) Channel Status Register */
#define REG_USART0_RHR             (*(RoReg  *)0x40024018U) /**< \brief (USART0) Receiver Holding Register */
#define REG_USART0_THR             (*(WoReg  *)0x4002401CU) /**< \brief (USART0) Transmitter Holding Register */
#define REG_USART0_BRGR            (*(RwReg  *)0x40024020U) /**< \brief (USART0) Baud Rate Generator Register */
#define REG_USART0_RTOR            (*(RwReg  *)0x40024024U) /**< \brief (USART0) Receiver Time-out Register */
#define REG_USART0_TTGR            (*(RwReg  *)0x40024028U) /**< \brief (USART0) Transmitter Timeguard Register */
#define REG_USART0_FIDI            (*(RwReg  *)0x40024040U) /**< \brief (USART0) FI DI Ratio Register */
#define REG_USART0_NER             (*(RoReg  *)0x40024044U) /**< \brief (USART0) Number of Errors Register */
#define REG_USART0_IFR             (*(RwReg  *)0x4002404CU) /**< \brief (USART0) IrDA Filter Register */
#define REG_USART0_MAN             (*(RwReg  *)0x40024050U) /**< \brief (USART0) Manchester Configuration Register */
#define REG_USART0_LINMR           (*(RwReg  *)0x40024054U) /**< \brief (USART0) LIN Mode Register */
#define REG_USART0_LINIR           (*(RwReg  *)0x40024058U) /**< \brief (USART0) LIN Identifier Register */
#define REG_USART0_LINBRR          (*(RoReg  *)0x4002405CU) /**< \brief (USART0) LIN Baud Rate Register */
#define REG_USART0_WPMR            (*(RwReg  *)0x400240E4U) /**< \brief (USART0) Write Protect Mode Register */
#define REG_USART0_WPSR            (*(RoReg  *)0x400240E8U) /**< \brief (USART0) Write Protect Status Register */
#define REG_USART0_VERSION         (*(RoReg  *)0x400240FCU) /**< \brief (USART0) Version Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/* ========== Instance parameters for USART0 peripheral ========== */
#define USART0_CLK_DIV              8
#define USART0_PDCA_ID_RX           0
#define USART0_PDCA_ID_TX           18

#endif /* _SAM4L_USART0_INSTANCE_ */
