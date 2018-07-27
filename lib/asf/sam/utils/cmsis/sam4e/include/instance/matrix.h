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

#ifndef _SAM4E_MATRIX_INSTANCE_
#define _SAM4E_MATRIX_INSTANCE_

/* ========== Register definition for MATRIX peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_MATRIX_MCFG                   (0x400E0200U) /**< \brief (MATRIX) Master Configuration Register */
  #define REG_MATRIX_SCFG                   (0x400E0240U) /**< \brief (MATRIX) Slave Configuration Register */
  #define REG_MATRIX_PRAS0                  (0x400E0280U) /**< \brief (MATRIX) Priority Register A for Slave 0 */
  #define REG_MATRIX_PRAS1                  (0x400E0288U) /**< \brief (MATRIX) Priority Register A for Slave 1 */
  #define REG_MATRIX_PRAS2                  (0x400E0290U) /**< \brief (MATRIX) Priority Register A for Slave 2 */
  #define REG_MATRIX_PRAS3                  (0x400E0298U) /**< \brief (MATRIX) Priority Register A for Slave 3 */
  #define REG_MATRIX_PRAS4                  (0x400E02A0U) /**< \brief (MATRIX) Priority Register A for Slave 4 */
  #define REG_MATRIX_PRAS5                  (0x400E02A8U) /**< \brief (MATRIX) Priority Register A for Slave 5 */
  #define REG_MATRIX_MRCR                   (0x400E0300U) /**< \brief (MATRIX) Master Remap Control Register */
  #define REG_CCFG_SYSIO                    (0x400E0314U) /**< \brief (MATRIX) System I/O Configuration Register */
  #define REG_CCFG_SMCNFCS                  (0x400E0324U) /**< \brief (MATRIX) SMC NAND Flash Chip Select Configuration Register */
  #define REG_MATRIX_WPMR                   (0x400E03E4U) /**< \brief (MATRIX) Write Protect Mode Register */
  #define REG_MATRIX_WPSR                   (0x400E03E8U) /**< \brief (MATRIX) Write Protect Status Register */
#else
  #define REG_MATRIX_MCFG  (*(__IO uint32_t*)0x400E0200U) /**< \brief (MATRIX) Master Configuration Register */
  #define REG_MATRIX_SCFG  (*(__IO uint32_t*)0x400E0240U) /**< \brief (MATRIX) Slave Configuration Register */
  #define REG_MATRIX_PRAS0 (*(__IO uint32_t*)0x400E0280U) /**< \brief (MATRIX) Priority Register A for Slave 0 */
  #define REG_MATRIX_PRAS1 (*(__IO uint32_t*)0x400E0288U) /**< \brief (MATRIX) Priority Register A for Slave 1 */
  #define REG_MATRIX_PRAS2 (*(__IO uint32_t*)0x400E0290U) /**< \brief (MATRIX) Priority Register A for Slave 2 */
  #define REG_MATRIX_PRAS3 (*(__IO uint32_t*)0x400E0298U) /**< \brief (MATRIX) Priority Register A for Slave 3 */
  #define REG_MATRIX_PRAS4 (*(__IO uint32_t*)0x400E02A0U) /**< \brief (MATRIX) Priority Register A for Slave 4 */
  #define REG_MATRIX_PRAS5 (*(__IO uint32_t*)0x400E02A8U) /**< \brief (MATRIX) Priority Register A for Slave 5 */
  #define REG_MATRIX_MRCR  (*(__IO uint32_t*)0x400E0300U) /**< \brief (MATRIX) Master Remap Control Register */
  #define REG_CCFG_SYSIO   (*(__IO uint32_t*)0x400E0314U) /**< \brief (MATRIX) System I/O Configuration Register */
  #define REG_CCFG_SMCNFCS (*(__IO uint32_t*)0x400E0324U) /**< \brief (MATRIX) SMC NAND Flash Chip Select Configuration Register */
  #define REG_MATRIX_WPMR  (*(__IO uint32_t*)0x400E03E4U) /**< \brief (MATRIX) Write Protect Mode Register */
  #define REG_MATRIX_WPSR  (*(__I  uint32_t*)0x400E03E8U) /**< \brief (MATRIX) Write Protect Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAM4E_MATRIX_INSTANCE_ */
