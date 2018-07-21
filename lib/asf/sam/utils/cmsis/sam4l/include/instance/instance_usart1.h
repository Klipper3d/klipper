/**
 * \file
 *
 * \brief Instance description for USART1
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

#ifndef _SAM4L_USART1_INSTANCE_
#define _SAM4L_USART1_INSTANCE_

/* ========== Register definition for USART1 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_USART1_CR              (0x40028000U) /**< \brief (USART1) Control Register */
#define REG_USART1_MR              (0x40028004U) /**< \brief (USART1) Mode Register */
#define REG_USART1_IER             (0x40028008U) /**< \brief (USART1) Interrupt Enable Register */
#define REG_USART1_IDR             (0x4002800CU) /**< \brief (USART1) Interrupt Disable Register */
#define REG_USART1_IMR             (0x40028010U) /**< \brief (USART1) Interrupt Mask Register */
#define REG_USART1_CSR             (0x40028014U) /**< \brief (USART1) Channel Status Register */
#define REG_USART1_RHR             (0x40028018U) /**< \brief (USART1) Receiver Holding Register */
#define REG_USART1_THR             (0x4002801CU) /**< \brief (USART1) Transmitter Holding Register */
#define REG_USART1_BRGR            (0x40028020U) /**< \brief (USART1) Baud Rate Generator Register */
#define REG_USART1_RTOR            (0x40028024U) /**< \brief (USART1) Receiver Time-out Register */
#define REG_USART1_TTGR            (0x40028028U) /**< \brief (USART1) Transmitter Timeguard Register */
#define REG_USART1_FIDI            (0x40028040U) /**< \brief (USART1) FI DI Ratio Register */
#define REG_USART1_NER             (0x40028044U) /**< \brief (USART1) Number of Errors Register */
#define REG_USART1_IFR             (0x4002804CU) /**< \brief (USART1) IrDA Filter Register */
#define REG_USART1_MAN             (0x40028050U) /**< \brief (USART1) Manchester Configuration Register */
#define REG_USART1_LINMR           (0x40028054U) /**< \brief (USART1) LIN Mode Register */
#define REG_USART1_LINIR           (0x40028058U) /**< \brief (USART1) LIN Identifier Register */
#define REG_USART1_LINBRR          (0x4002805CU) /**< \brief (USART1) LIN Baud Rate Register */
#define REG_USART1_WPMR            (0x400280E4U) /**< \brief (USART1) Write Protect Mode Register */
#define REG_USART1_WPSR            (0x400280E8U) /**< \brief (USART1) Write Protect Status Register */
#define REG_USART1_VERSION         (0x400280FCU) /**< \brief (USART1) Version Register */
#else
#define REG_USART1_CR              (*(WoReg  *)0x40028000U) /**< \brief (USART1) Control Register */
#define REG_USART1_MR              (*(RwReg  *)0x40028004U) /**< \brief (USART1) Mode Register */
#define REG_USART1_IER             (*(WoReg  *)0x40028008U) /**< \brief (USART1) Interrupt Enable Register */
#define REG_USART1_IDR             (*(WoReg  *)0x4002800CU) /**< \brief (USART1) Interrupt Disable Register */
#define REG_USART1_IMR             (*(RoReg  *)0x40028010U) /**< \brief (USART1) Interrupt Mask Register */
#define REG_USART1_CSR             (*(RoReg  *)0x40028014U) /**< \brief (USART1) Channel Status Register */
#define REG_USART1_RHR             (*(RoReg  *)0x40028018U) /**< \brief (USART1) Receiver Holding Register */
#define REG_USART1_THR             (*(WoReg  *)0x4002801CU) /**< \brief (USART1) Transmitter Holding Register */
#define REG_USART1_BRGR            (*(RwReg  *)0x40028020U) /**< \brief (USART1) Baud Rate Generator Register */
#define REG_USART1_RTOR            (*(RwReg  *)0x40028024U) /**< \brief (USART1) Receiver Time-out Register */
#define REG_USART1_TTGR            (*(RwReg  *)0x40028028U) /**< \brief (USART1) Transmitter Timeguard Register */
#define REG_USART1_FIDI            (*(RwReg  *)0x40028040U) /**< \brief (USART1) FI DI Ratio Register */
#define REG_USART1_NER             (*(RoReg  *)0x40028044U) /**< \brief (USART1) Number of Errors Register */
#define REG_USART1_IFR             (*(RwReg  *)0x4002804CU) /**< \brief (USART1) IrDA Filter Register */
#define REG_USART1_MAN             (*(RwReg  *)0x40028050U) /**< \brief (USART1) Manchester Configuration Register */
#define REG_USART1_LINMR           (*(RwReg  *)0x40028054U) /**< \brief (USART1) LIN Mode Register */
#define REG_USART1_LINIR           (*(RwReg  *)0x40028058U) /**< \brief (USART1) LIN Identifier Register */
#define REG_USART1_LINBRR          (*(RoReg  *)0x4002805CU) /**< \brief (USART1) LIN Baud Rate Register */
#define REG_USART1_WPMR            (*(RwReg  *)0x400280E4U) /**< \brief (USART1) Write Protect Mode Register */
#define REG_USART1_WPSR            (*(RoReg  *)0x400280E8U) /**< \brief (USART1) Write Protect Status Register */
#define REG_USART1_VERSION         (*(RoReg  *)0x400280FCU) /**< \brief (USART1) Version Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/* ========== Instance parameters for USART1 peripheral ========== */
#define USART1_CLK_DIV              8
#define USART1_PDCA_ID_RX           1
#define USART1_PDCA_ID_TX           19

#endif /* _SAM4L_USART1_INSTANCE_ */
