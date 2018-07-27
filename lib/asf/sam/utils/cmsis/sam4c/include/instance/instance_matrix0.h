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
#ifndef _SAM4C_MATRIX0_INSTANCE_
#define _SAM4C_MATRIX0_INSTANCE_

/* ========== Register definition for MATRIX0 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_MATRIX0_MCFG                        (0x400E0200U) /**< \brief (MATRIX0) Master Configuration Register */
  #define REG_MATRIX0_SCFG                        (0x400E0240U) /**< \brief (MATRIX0) Slave Configuration Register */
  #define REG_MATRIX0_PRAS0                       (0x400E0280U) /**< \brief (MATRIX0) Priority Register A for Slave 0 */
  #define REG_MATRIX0_PRAS1                       (0x400E0288U) /**< \brief (MATRIX0) Priority Register A for Slave 1 */
  #define REG_MATRIX0_PRAS2                       (0x400E0290U) /**< \brief (MATRIX0) Priority Register A for Slave 2 */
  #define REG_MATRIX0_PRAS3                       (0x400E0298U) /**< \brief (MATRIX0) Priority Register A for Slave 3 */
  #define REG_MATRIX0_PRAS4                       (0x400E02A0U) /**< \brief (MATRIX0) Priority Register A for Slave 4 */
  #define REG_MATRIX0_PRAS5                       (0x400E02A8U) /**< \brief (MATRIX0) Priority Register A for Slave 5 */
  #define REG_MATRIX0_PRAS6                       (0x400E02B0U) /**< \brief (MATRIX0) Priority Register A for Slave 6 */
  #define REG_MATRIX0_PRAS7                       (0x400E02B8U) /**< \brief (MATRIX0) Priority Register A for Slave 7 */
  #define REG_MATRIX0_PRAS8                       (0x400E02BCU) /**< \brief (MATRIX0) Priority Register A for Slave 8 */
  #define REG_MATRIX0_SYSIO                       (0x400E0314U) /**< \brief (MATRIX0) System I/O Configuration Register */
  #define REG_MATRIX0_SMCNFCS                     (0x400E031CU) /**< \brief (MATRIX0) SMC Nand Flash Chip Select Configuration Register */
  #define REG_MATRIX0_CORE_DEBUG                  (0x400E0328U) /**< \brief (MATRIX0) Core Debug Configuration Register */
  #define REG_MATRIX0_WPMR                        (0x400E03E4U) /**< \brief (MATRIX0) Write Protection Mode Register */
  #define REG_MATRIX0_WPSR                        (0x400E03E8U) /**< \brief (MATRIX0) Write Protection Status Register */
#else
  #define REG_MATRIX0_MCFG       (*(__IO uint32_t*)0x400E0200U) /**< \brief (MATRIX0) Master Configuration Register */
  #define REG_MATRIX0_SCFG       (*(__IO uint32_t*)0x400E0240U) /**< \brief (MATRIX0) Slave Configuration Register */
  #define REG_MATRIX0_PRAS0      (*(__IO uint32_t*)0x400E0280U) /**< \brief (MATRIX0) Priority Register A for Slave 0 */
  #define REG_MATRIX0_PRAS1      (*(__IO uint32_t*)0x400E0288U) /**< \brief (MATRIX0) Priority Register A for Slave 1 */
  #define REG_MATRIX0_PRAS2      (*(__IO uint32_t*)0x400E0290U) /**< \brief (MATRIX0) Priority Register A for Slave 2 */
  #define REG_MATRIX0_PRAS3      (*(__IO uint32_t*)0x400E0298U) /**< \brief (MATRIX0) Priority Register A for Slave 3 */
  #define REG_MATRIX0_PRAS4      (*(__IO uint32_t*)0x400E02A0U) /**< \brief (MATRIX0) Priority Register A for Slave 4 */
  #define REG_MATRIX0_PRAS5      (*(__IO uint32_t*)0x400E02A8U) /**< \brief (MATRIX0) Priority Register A for Slave 5 */
  #define REG_MATRIX0_PRAS6      (*(__IO uint32_t*)0x400E02B0U) /**< \brief (MATRIX0) Priority Register A for Slave 6 */
  #define REG_MATRIX0_PRAS7      (*(__IO uint32_t*)0x400E02B8U) /**< \brief (MATRIX0) Priority Register A for Slave 7 */
  #define REG_MATRIX0_PRAS8      (*(__IO uint32_t*)0x400E02BCU) /**< \brief (MATRIX0) Priority Register A for Slave 8 */
  #define REG_MATRIX0_SYSIO      (*(__IO uint32_t*)0x400E0314U) /**< \brief (MATRIX0) System I/O Configuration Register */
  #define REG_MATRIX0_SMCNFCS    (*(__IO uint32_t*)0x400E031CU) /**< \brief (MATRIX0) SMC Nand Flash Chip Select Configuration Register */
  #define REG_MATRIX0_CORE_DEBUG (*(__IO uint32_t*)0x400E0328U) /**< \brief (MATRIX0) Core Debug Configuration Register */
  #define REG_MATRIX0_WPMR       (*(__IO uint32_t*)0x400E03E4U) /**< \brief (MATRIX0) Write Protection Mode Register */
  #define REG_MATRIX0_WPSR       (*(__I  uint32_t*)0x400E03E8U) /**< \brief (MATRIX0) Write Protection Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAM4C_MATRIX0_INSTANCE_ */
