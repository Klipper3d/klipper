/**
 * \file
 *
 * \brief Instance description for GLOC
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

#ifndef _SAM4L_GLOC_INSTANCE_
#define _SAM4L_GLOC_INSTANCE_

/* ========== Register definition for GLOC peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_GLOC_CR0               (0x40060000U) /**< \brief (GLOC) Control Register 0 */
#define REG_GLOC_TRUTH0            (0x40060004U) /**< \brief (GLOC) Truth Register 0 */
#define REG_GLOC_CR1               (0x40060008U) /**< \brief (GLOC) Control Register 1 */
#define REG_GLOC_TRUTH1            (0x4006000CU) /**< \brief (GLOC) Truth Register 1 */
#define REG_GLOC_PARAMETER         (0x40060038U) /**< \brief (GLOC) Parameter Register */
#define REG_GLOC_VERSION           (0x4006003CU) /**< \brief (GLOC) Version Register */
#else
#define REG_GLOC_CR0               (*(RwReg  *)0x40060000U) /**< \brief (GLOC) Control Register 0 */
#define REG_GLOC_TRUTH0            (*(RwReg  *)0x40060004U) /**< \brief (GLOC) Truth Register 0 */
#define REG_GLOC_CR1               (*(RwReg  *)0x40060008U) /**< \brief (GLOC) Control Register 1 */
#define REG_GLOC_TRUTH1            (*(RwReg  *)0x4006000CU) /**< \brief (GLOC) Truth Register 1 */
#define REG_GLOC_PARAMETER         (*(RoReg  *)0x40060038U) /**< \brief (GLOC) Parameter Register */
#define REG_GLOC_VERSION           (*(RoReg  *)0x4006003CU) /**< \brief (GLOC) Version Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/* ========== Instance parameters for GLOC peripheral ========== */
#define GLOC_GCLK_NUM               5
#define GLOC_LUTS                   2

#endif /* _SAM4L_GLOC_INSTANCE_ */
