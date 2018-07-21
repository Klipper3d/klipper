/**
 * \file
 *
 * \brief Instance description for PICOUART
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

#ifndef _SAM4L_PICOUART_INSTANCE_
#define _SAM4L_PICOUART_INSTANCE_

/* ========== Register definition for PICOUART peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_PICOUART_CR            (0x400F1400U) /**< \brief (PICOUART) Control Register */
#define REG_PICOUART_CFG           (0x400F1404U) /**< \brief (PICOUART) Configuration Register */
#define REG_PICOUART_SR            (0x400F1408U) /**< \brief (PICOUART) Status Register */
#define REG_PICOUART_RHR           (0x400F140CU) /**< \brief (PICOUART) Receive Holding Register */
#define REG_PICOUART_VERSION       (0x400F1420U) /**< \brief (PICOUART) Version Register */
#else
#define REG_PICOUART_CR            (*(WoReg  *)0x400F1400U) /**< \brief (PICOUART) Control Register */
#define REG_PICOUART_CFG           (*(RwReg  *)0x400F1404U) /**< \brief (PICOUART) Configuration Register */
#define REG_PICOUART_SR            (*(RoReg  *)0x400F1408U) /**< \brief (PICOUART) Status Register */
#define REG_PICOUART_RHR           (*(RoReg  *)0x400F140CU) /**< \brief (PICOUART) Receive Holding Register */
#define REG_PICOUART_VERSION       (*(RoReg  *)0x400F1420U) /**< \brief (PICOUART) Version Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */


#endif /* _SAM4L_PICOUART_INSTANCE_ */
