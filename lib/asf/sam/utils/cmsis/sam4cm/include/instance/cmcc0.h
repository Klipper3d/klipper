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

#ifndef _SAM4CM_CMCC0_INSTANCE_
#define _SAM4CM_CMCC0_INSTANCE_

/* ========== Register definition for CMCC0 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_CMCC0_TYPE                    (0x4007C000U) /**< \brief (CMCC0) Cache Type Register */
  #define REG_CMCC0_CTRL                    (0x4007C008U) /**< \brief (CMCC0) Cache Control Register */
  #define REG_CMCC0_SR                      (0x4007C00CU) /**< \brief (CMCC0) Cache Status Register */
  #define REG_CMCC0_MAINT0                  (0x4007C020U) /**< \brief (CMCC0) Cache Maintenance Register 0 */
  #define REG_CMCC0_MAINT1                  (0x4007C024U) /**< \brief (CMCC0) Cache Maintenance Register 1 */
  #define REG_CMCC0_MCFG                    (0x4007C028U) /**< \brief (CMCC0) Cache Monitor Configuration Register */
  #define REG_CMCC0_MEN                     (0x4007C02CU) /**< \brief (CMCC0) Cache Monitor Enable Register */
  #define REG_CMCC0_MCTRL                   (0x4007C030U) /**< \brief (CMCC0) Cache Monitor Control Register */
  #define REG_CMCC0_MSR                     (0x4007C034U) /**< \brief (CMCC0) Cache Monitor Status Register */
#else
  #define REG_CMCC0_TYPE   (*(__I  uint32_t*)0x4007C000U) /**< \brief (CMCC0) Cache Type Register */
  #define REG_CMCC0_CTRL   (*(__O  uint32_t*)0x4007C008U) /**< \brief (CMCC0) Cache Control Register */
  #define REG_CMCC0_SR     (*(__I  uint32_t*)0x4007C00CU) /**< \brief (CMCC0) Cache Status Register */
  #define REG_CMCC0_MAINT0 (*(__O  uint32_t*)0x4007C020U) /**< \brief (CMCC0) Cache Maintenance Register 0 */
  #define REG_CMCC0_MAINT1 (*(__O  uint32_t*)0x4007C024U) /**< \brief (CMCC0) Cache Maintenance Register 1 */
  #define REG_CMCC0_MCFG   (*(__IO uint32_t*)0x4007C028U) /**< \brief (CMCC0) Cache Monitor Configuration Register */
  #define REG_CMCC0_MEN    (*(__IO uint32_t*)0x4007C02CU) /**< \brief (CMCC0) Cache Monitor Enable Register */
  #define REG_CMCC0_MCTRL  (*(__O  uint32_t*)0x4007C030U) /**< \brief (CMCC0) Cache Monitor Control Register */
  #define REG_CMCC0_MSR    (*(__I  uint32_t*)0x4007C034U) /**< \brief (CMCC0) Cache Monitor Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAM4CM_CMCC0_INSTANCE_ */
