/**
 * \file
 *
 * \brief Instance description for USART2
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

#ifndef _SAM4L_USART2_INSTANCE_
#define _SAM4L_USART2_INSTANCE_

/* ========== Register definition for USART2 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_USART2_CR              (0x4002C000U) /**< \brief (USART2) Control Register */
#define REG_USART2_MR              (0x4002C004U) /**< \brief (USART2) Mode Register */
#define REG_USART2_IER             (0x4002C008U) /**< \brief (USART2) Interrupt Enable Register */
#define REG_USART2_IDR             (0x4002C00CU) /**< \brief (USART2) Interrupt Disable Register */
#define REG_USART2_IMR             (0x4002C010U) /**< \brief (USART2) Interrupt Mask Register */
#define REG_USART2_CSR             (0x4002C014U) /**< \brief (USART2) Channel Status Register */
#define REG_USART2_RHR             (0x4002C018U) /**< \brief (USART2) Receiver Holding Register */
#define REG_USART2_THR             (0x4002C01CU) /**< \brief (USART2) Transmitter Holding Register */
#define REG_USART2_BRGR            (0x4002C020U) /**< \brief (USART2) Baud Rate Generator Register */
#define REG_USART2_RTOR            (0x4002C024U) /**< \brief (USART2) Receiver Time-out Register */
#define REG_USART2_TTGR            (0x4002C028U) /**< \brief (USART2) Transmitter Timeguard Register */
#define REG_USART2_FIDI            (0x4002C040U) /**< \brief (USART2) FI DI Ratio Register */
#define REG_USART2_NER             (0x4002C044U) /**< \brief (USART2) Number of Errors Register */
#define REG_USART2_IFR             (0x4002C04CU) /**< \brief (USART2) IrDA Filter Register */
#define REG_USART2_MAN             (0x4002C050U) /**< \brief (USART2) Manchester Configuration Register */
#define REG_USART2_LINMR           (0x4002C054U) /**< \brief (USART2) LIN Mode Register */
#define REG_USART2_LINIR           (0x4002C058U) /**< \brief (USART2) LIN Identifier Register */
#define REG_USART2_LINBRR          (0x4002C05CU) /**< \brief (USART2) LIN Baud Rate Register */
#define REG_USART2_WPMR            (0x4002C0E4U) /**< \brief (USART2) Write Protect Mode Register */
#define REG_USART2_WPSR            (0x4002C0E8U) /**< \brief (USART2) Write Protect Status Register */
#define REG_USART2_VERSION         (0x4002C0FCU) /**< \brief (USART2) Version Register */
#else
#define REG_USART2_CR              (*(WoReg  *)0x4002C000U) /**< \brief (USART2) Control Register */
#define REG_USART2_MR              (*(RwReg  *)0x4002C004U) /**< \brief (USART2) Mode Register */
#define REG_USART2_IER             (*(WoReg  *)0x4002C008U) /**< \brief (USART2) Interrupt Enable Register */
#define REG_USART2_IDR             (*(WoReg  *)0x4002C00CU) /**< \brief (USART2) Interrupt Disable Register */
#define REG_USART2_IMR             (*(RoReg  *)0x4002C010U) /**< \brief (USART2) Interrupt Mask Register */
#define REG_USART2_CSR             (*(RoReg  *)0x4002C014U) /**< \brief (USART2) Channel Status Register */
#define REG_USART2_RHR             (*(RoReg  *)0x4002C018U) /**< \brief (USART2) Receiver Holding Register */
#define REG_USART2_THR             (*(WoReg  *)0x4002C01CU) /**< \brief (USART2) Transmitter Holding Register */
#define REG_USART2_BRGR            (*(RwReg  *)0x4002C020U) /**< \brief (USART2) Baud Rate Generator Register */
#define REG_USART2_RTOR            (*(RwReg  *)0x4002C024U) /**< \brief (USART2) Receiver Time-out Register */
#define REG_USART2_TTGR            (*(RwReg  *)0x4002C028U) /**< \brief (USART2) Transmitter Timeguard Register */
#define REG_USART2_FIDI            (*(RwReg  *)0x4002C040U) /**< \brief (USART2) FI DI Ratio Register */
#define REG_USART2_NER             (*(RoReg  *)0x4002C044U) /**< \brief (USART2) Number of Errors Register */
#define REG_USART2_IFR             (*(RwReg  *)0x4002C04CU) /**< \brief (USART2) IrDA Filter Register */
#define REG_USART2_MAN             (*(RwReg  *)0x4002C050U) /**< \brief (USART2) Manchester Configuration Register */
#define REG_USART2_LINMR           (*(RwReg  *)0x4002C054U) /**< \brief (USART2) LIN Mode Register */
#define REG_USART2_LINIR           (*(RwReg  *)0x4002C058U) /**< \brief (USART2) LIN Identifier Register */
#define REG_USART2_LINBRR          (*(RoReg  *)0x4002C05CU) /**< \brief (USART2) LIN Baud Rate Register */
#define REG_USART2_WPMR            (*(RwReg  *)0x4002C0E4U) /**< \brief (USART2) Write Protect Mode Register */
#define REG_USART2_WPSR            (*(RoReg  *)0x4002C0E8U) /**< \brief (USART2) Write Protect Status Register */
#define REG_USART2_VERSION         (*(RoReg  *)0x4002C0FCU) /**< \brief (USART2) Version Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/* ========== Instance parameters for USART2 peripheral ========== */
#define USART2_CLK_DIV              8
#define USART2_PDCA_ID_RX           2
#define USART2_PDCA_ID_TX           20

#endif /* _SAM4L_USART2_INSTANCE_ */
