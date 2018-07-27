/**
 * \file
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

#ifndef _SAM3N_UART1_INSTANCE_
#define _SAM3N_UART1_INSTANCE_

/* ========== Register definition for UART1 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_UART1_CR            (0x400E0800U) /**< \brief (UART1) Control Register */
#define REG_UART1_MR            (0x400E0804U) /**< \brief (UART1) Mode Register */
#define REG_UART1_IER           (0x400E0808U) /**< \brief (UART1) Interrupt Enable Register */
#define REG_UART1_IDR           (0x400E080CU) /**< \brief (UART1) Interrupt Disable Register */
#define REG_UART1_IMR           (0x400E0810U) /**< \brief (UART1) Interrupt Mask Register */
#define REG_UART1_SR            (0x400E0814U) /**< \brief (UART1) Status Register */
#define REG_UART1_RHR           (0x400E0818U) /**< \brief (UART1) Receive Holding Register */
#define REG_UART1_THR           (0x400E081CU) /**< \brief (UART1) Transmit Holding Register */
#define REG_UART1_BRGR          (0x400E0820U) /**< \brief (UART1) Baud Rate Generator Register */
#else
#define REG_UART1_CR   (*(WoReg*)0x400E0800U) /**< \brief (UART1) Control Register */
#define REG_UART1_MR   (*(RwReg*)0x400E0804U) /**< \brief (UART1) Mode Register */
#define REG_UART1_IER  (*(WoReg*)0x400E0808U) /**< \brief (UART1) Interrupt Enable Register */
#define REG_UART1_IDR  (*(WoReg*)0x400E080CU) /**< \brief (UART1) Interrupt Disable Register */
#define REG_UART1_IMR  (*(RoReg*)0x400E0810U) /**< \brief (UART1) Interrupt Mask Register */
#define REG_UART1_SR   (*(RoReg*)0x400E0814U) /**< \brief (UART1) Status Register */
#define REG_UART1_RHR  (*(RoReg*)0x400E0818U) /**< \brief (UART1) Receive Holding Register */
#define REG_UART1_THR  (*(WoReg*)0x400E081CU) /**< \brief (UART1) Transmit Holding Register */
#define REG_UART1_BRGR (*(RwReg*)0x400E0820U) /**< \brief (UART1) Baud Rate Generator Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAM3N_UART1_INSTANCE_ */
