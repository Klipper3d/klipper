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

#ifndef _SAM4E_CMCC_INSTANCE_
#define _SAM4E_CMCC_INSTANCE_

/* ========== Register definition for CMCC peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_CMCC_TYPE                    (0x400C4000U) /**< \brief (CMCC) Cache Type Register */
  #define REG_CMCC_CFG                     (0x400C4004U) /**< \brief (CMCC) Cache Configuration Register */
  #define REG_CMCC_CTRL                    (0x400C4008U) /**< \brief (CMCC) Cache Control Register */
  #define REG_CMCC_SR                      (0x400C400CU) /**< \brief (CMCC) Cache Status Register */
  #define REG_CMCC_MAINT0                  (0x400C4020U) /**< \brief (CMCC) Cache Maintenance Register 0 */
  #define REG_CMCC_MAINT1                  (0x400C4024U) /**< \brief (CMCC) Cache Maintenance Register 1 */
  #define REG_CMCC_MCFG                    (0x400C4028U) /**< \brief (CMCC) Cache Monitor Configuration Register */
  #define REG_CMCC_MEN                     (0x400C402CU) /**< \brief (CMCC) Cache Monitor Enable Register */
  #define REG_CMCC_MCTRL                   (0x400C4030U) /**< \brief (CMCC) Cache Monitor Control Register */
  #define REG_CMCC_MSR                     (0x400C4034U) /**< \brief (CMCC) Cache Monitor Status Register */
#else
  #define REG_CMCC_TYPE   (*(__I  uint32_t*)0x400C4000U) /**< \brief (CMCC) Cache Type Register */
  #define REG_CMCC_CFG    (*(__IO uint32_t*)0x400C4004U) /**< \brief (CMCC) Cache Configuration Register */
  #define REG_CMCC_CTRL   (*(__O  uint32_t*)0x400C4008U) /**< \brief (CMCC) Cache Control Register */
  #define REG_CMCC_SR     (*(__I  uint32_t*)0x400C400CU) /**< \brief (CMCC) Cache Status Register */
  #define REG_CMCC_MAINT0 (*(__O  uint32_t*)0x400C4020U) /**< \brief (CMCC) Cache Maintenance Register 0 */
  #define REG_CMCC_MAINT1 (*(__O  uint32_t*)0x400C4024U) /**< \brief (CMCC) Cache Maintenance Register 1 */
  #define REG_CMCC_MCFG   (*(__IO uint32_t*)0x400C4028U) /**< \brief (CMCC) Cache Monitor Configuration Register */
  #define REG_CMCC_MEN    (*(__IO uint32_t*)0x400C402CU) /**< \brief (CMCC) Cache Monitor Enable Register */
  #define REG_CMCC_MCTRL  (*(__O  uint32_t*)0x400C4030U) /**< \brief (CMCC) Cache Monitor Control Register */
  #define REG_CMCC_MSR    (*(__I  uint32_t*)0x400C4034U) /**< \brief (CMCC) Cache Monitor Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAM4E_CMCC_INSTANCE_ */
