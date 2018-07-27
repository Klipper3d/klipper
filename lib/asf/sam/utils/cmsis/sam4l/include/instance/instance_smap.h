/**
 * \file
 *
 * \brief Instance description for SMAP
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

#ifndef _SAM4L_SMAP_INSTANCE_
#define _SAM4L_SMAP_INSTANCE_

/* ========== Register definition for SMAP peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_SMAP_CR                (0x400A3000U) /**< \brief (SMAP) Control Register */
#define REG_SMAP_SR                (0x400A3004U) /**< \brief (SMAP) Status Register */
#define REG_SMAP_SCR               (0x400A3008U) /**< \brief (SMAP) Status Clear Register */
#define REG_SMAP_ADDR              (0x400A300CU) /**< \brief (SMAP) Address Register */
#define REG_SMAP_LENGTH            (0x400A3010U) /**< \brief (SMAP) Length Register */
#define REG_SMAP_DATA              (0x400A3014U) /**< \brief (SMAP) Data Register */
#define REG_SMAP_VERSION           (0x400A3028U) /**< \brief (SMAP) VERSION register */
#define REG_SMAP_CIDR              (0x400A30F0U) /**< \brief (SMAP) Chip ID Register */
#define REG_SMAP_EXID              (0x400A30F4U) /**< \brief (SMAP) Chip ID Extension Register */
#define REG_SMAP_IDR               (0x400A30FCU) /**< \brief (SMAP) AP Identification register */
#else
#define REG_SMAP_CR                (*(WoReg  *)0x400A3000U) /**< \brief (SMAP) Control Register */
#define REG_SMAP_SR                (*(RoReg  *)0x400A3004U) /**< \brief (SMAP) Status Register */
#define REG_SMAP_SCR               (*(WoReg  *)0x400A3008U) /**< \brief (SMAP) Status Clear Register */
#define REG_SMAP_ADDR              (*(RwReg  *)0x400A300CU) /**< \brief (SMAP) Address Register */
#define REG_SMAP_LENGTH            (*(RwReg  *)0x400A3010U) /**< \brief (SMAP) Length Register */
#define REG_SMAP_DATA              (*(RwReg  *)0x400A3014U) /**< \brief (SMAP) Data Register */
#define REG_SMAP_VERSION           (*(RoReg  *)0x400A3028U) /**< \brief (SMAP) VERSION register */
#define REG_SMAP_CIDR              (*(RoReg  *)0x400A30F0U) /**< \brief (SMAP) Chip ID Register */
#define REG_SMAP_EXID              (*(RoReg  *)0x400A30F4U) /**< \brief (SMAP) Chip ID Extension Register */
#define REG_SMAP_IDR               (*(RoReg  *)0x400A30FCU) /**< \brief (SMAP) AP Identification register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */


#endif /* _SAM4L_SMAP_INSTANCE_ */
