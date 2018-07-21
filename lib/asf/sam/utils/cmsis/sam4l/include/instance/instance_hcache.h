/**
 * \file
 *
 * \brief Instance description for HCACHE
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

#ifndef _SAM4L_HCACHE_INSTANCE_
#define _SAM4L_HCACHE_INSTANCE_

/* ========== Register definition for HCACHE peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_HCACHE_CTRL            (0x400A0408U) /**< \brief (HCACHE) Control Register */
#define REG_HCACHE_SR              (0x400A040CU) /**< \brief (HCACHE) Status Register */
#define REG_HCACHE_MAINT0          (0x400A0420U) /**< \brief (HCACHE) Maintenance Register 0 */
#define REG_HCACHE_MAINT1          (0x400A0424U) /**< \brief (HCACHE) Maintenance Register 1 */
#define REG_HCACHE_MCFG            (0x400A0428U) /**< \brief (HCACHE) Monitor Configuration Register */
#define REG_HCACHE_MEN             (0x400A042CU) /**< \brief (HCACHE) Monitor Enable Register */
#define REG_HCACHE_MCTRL           (0x400A0430U) /**< \brief (HCACHE) Monitor Control Register */
#define REG_HCACHE_MSR             (0x400A0434U) /**< \brief (HCACHE) Monitor Status Register */
#define REG_HCACHE_VERSION         (0x400A04FCU) /**< \brief (HCACHE) Version Register */
#else
#define REG_HCACHE_CTRL            (*(WoReg  *)0x400A0408U) /**< \brief (HCACHE) Control Register */
#define REG_HCACHE_SR              (*(RwReg  *)0x400A040CU) /**< \brief (HCACHE) Status Register */
#define REG_HCACHE_MAINT0          (*(WoReg  *)0x400A0420U) /**< \brief (HCACHE) Maintenance Register 0 */
#define REG_HCACHE_MAINT1          (*(WoReg  *)0x400A0424U) /**< \brief (HCACHE) Maintenance Register 1 */
#define REG_HCACHE_MCFG            (*(RwReg  *)0x400A0428U) /**< \brief (HCACHE) Monitor Configuration Register */
#define REG_HCACHE_MEN             (*(RwReg  *)0x400A042CU) /**< \brief (HCACHE) Monitor Enable Register */
#define REG_HCACHE_MCTRL           (*(WoReg  *)0x400A0430U) /**< \brief (HCACHE) Monitor Control Register */
#define REG_HCACHE_MSR             (*(RoReg  *)0x400A0434U) /**< \brief (HCACHE) Monitor Status Register */
#define REG_HCACHE_VERSION         (*(RoReg  *)0x400A04FCU) /**< \brief (HCACHE) Version Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */


#endif /* _SAM4L_HCACHE_INSTANCE_ */
