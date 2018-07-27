/**
 * \file
 *
 * \brief Instance description for HFLASHC
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

#ifndef _SAM4L_HFLASHC_INSTANCE_
#define _SAM4L_HFLASHC_INSTANCE_

/* ========== Register definition for HFLASHC peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_HFLASHC_FCR            (0x400A0000U) /**< \brief (HFLASHC) Flash Controller Control Register */
#define REG_HFLASHC_FCMD           (0x400A0004U) /**< \brief (HFLASHC) Flash Controller Command Register */
#define REG_HFLASHC_FSR            (0x400A0008U) /**< \brief (HFLASHC) Flash Controller Status Register */
#define REG_HFLASHC_FPR            (0x400A000CU) /**< \brief (HFLASHC) Flash Controller Parameter Register */
#define REG_HFLASHC_VERSION        (0x400A0010U) /**< \brief (HFLASHC) Flash Controller Version Register */
#define REG_HFLASHC_FGPFRHI        (0x400A0014U) /**< \brief (HFLASHC) Flash Controller General Purpose Fuse Register High */
#define REG_HFLASHC_FGPFRLO        (0x400A0018U) /**< \brief (HFLASHC) Flash Controller General Purpose Fuse Register Low */
#else
#define REG_HFLASHC_FCR            (*(RwReg  *)0x400A0000U) /**< \brief (HFLASHC) Flash Controller Control Register */
#define REG_HFLASHC_FCMD           (*(RwReg  *)0x400A0004U) /**< \brief (HFLASHC) Flash Controller Command Register */
#define REG_HFLASHC_FSR            (*(RwReg  *)0x400A0008U) /**< \brief (HFLASHC) Flash Controller Status Register */
#define REG_HFLASHC_FPR            (*(RoReg  *)0x400A000CU) /**< \brief (HFLASHC) Flash Controller Parameter Register */
#define REG_HFLASHC_VERSION        (*(RoReg  *)0x400A0010U) /**< \brief (HFLASHC) Flash Controller Version Register */
#define REG_HFLASHC_FGPFRHI        (*(RwReg  *)0x400A0014U) /**< \brief (HFLASHC) Flash Controller General Purpose Fuse Register High */
#define REG_HFLASHC_FGPFRLO        (*(RwReg  *)0x400A0018U) /**< \brief (HFLASHC) Flash Controller General Purpose Fuse Register Low */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */


#endif /* _SAM4L_HFLASHC_INSTANCE_ */
