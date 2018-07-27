/**
 * \file
 *
 * Copyright (c) 2015-2018 Microchip Technology Inc. and its subsidiaries.
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

#ifndef _SAMV71_MLB_INSTANCE_
#define _SAMV71_MLB_INSTANCE_

/* ========== Register definition for MLB peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_MLB_MLBC0                  (0x40068000U) /**< \brief (MLB) MediaLB Control 0 Register */
  #define REG_MLB_MS0                    (0x4006800CU) /**< \brief (MLB) MediaLB Channel Status 0 Register */
  #define REG_MLB_MS1                    (0x40068014U) /**< \brief (MLB) MediaLB Channel Status1 Register */
  #define REG_MLB_MSS                    (0x40068020U) /**< \brief (MLB) MediaLB System Status Register */
  #define REG_MLB_MSD                    (0x40068024U) /**< \brief (MLB) MediaLB System Data Register */
  #define REG_MLB_MIEN                   (0x4006802CU) /**< \brief (MLB) MediaLB Interrupt Enable Register */
  #define REG_MLB_MLBC1                  (0x4006803CU) /**< \brief (MLB) MediaLB Control 1 Register */
  #define REG_MLB_HCTL                   (0x40068080U) /**< \brief (MLB) HBI Control Register */
  #define REG_MLB_HCMR                   (0x40068088U) /**< \brief (MLB) HBI Channel Mask 0 Register */
  #define REG_MLB_HCER                   (0x40068090U) /**< \brief (MLB) HBI Channel Error 0 Register */
  #define REG_MLB_HCBR                   (0x40068098U) /**< \brief (MLB) HBI Channel Busy 0 Register */
  #define REG_MLB_MDAT                   (0x400680C0U) /**< \brief (MLB) MIF Data 0 Register */
  #define REG_MLB_MDWE                   (0x400680D0U) /**< \brief (MLB) MIF Data Write Enable 0 Register */
  #define REG_MLB_MCTL                   (0x400680E0U) /**< \brief (MLB) MIF Control Register */
  #define REG_MLB_MADR                   (0x400680E4U) /**< \brief (MLB) MIF Address Register */
  #define REG_MLB_ACTL                   (0x400683C0U) /**< \brief (MLB) AHB Control Register */
  #define REG_MLB_ACSR                   (0x400683D0U) /**< \brief (MLB) AHB Channel Status 0 Register */
  #define REG_MLB_ACMR                   (0x400683D8U) /**< \brief (MLB) AHB Channel Mask 0 Register */
#else
  #define REG_MLB_MLBC0 (*(__IO uint32_t*)0x40068000U) /**< \brief (MLB) MediaLB Control 0 Register */
  #define REG_MLB_MS0   (*(__IO uint32_t*)0x4006800CU) /**< \brief (MLB) MediaLB Channel Status 0 Register */
  #define REG_MLB_MS1   (*(__IO uint32_t*)0x40068014U) /**< \brief (MLB) MediaLB Channel Status1 Register */
  #define REG_MLB_MSS   (*(__IO uint32_t*)0x40068020U) /**< \brief (MLB) MediaLB System Status Register */
  #define REG_MLB_MSD   (*(__I  uint32_t*)0x40068024U) /**< \brief (MLB) MediaLB System Data Register */
  #define REG_MLB_MIEN  (*(__IO uint32_t*)0x4006802CU) /**< \brief (MLB) MediaLB Interrupt Enable Register */
  #define REG_MLB_MLBC1 (*(__IO uint32_t*)0x4006803CU) /**< \brief (MLB) MediaLB Control 1 Register */
  #define REG_MLB_HCTL  (*(__IO uint32_t*)0x40068080U) /**< \brief (MLB) HBI Control Register */
  #define REG_MLB_HCMR  (*(__IO uint32_t*)0x40068088U) /**< \brief (MLB) HBI Channel Mask 0 Register */
  #define REG_MLB_HCER  (*(__I  uint32_t*)0x40068090U) /**< \brief (MLB) HBI Channel Error 0 Register */
  #define REG_MLB_HCBR  (*(__I  uint32_t*)0x40068098U) /**< \brief (MLB) HBI Channel Busy 0 Register */
  #define REG_MLB_MDAT  (*(__IO uint32_t*)0x400680C0U) /**< \brief (MLB) MIF Data 0 Register */
  #define REG_MLB_MDWE  (*(__IO uint32_t*)0x400680D0U) /**< \brief (MLB) MIF Data Write Enable 0 Register */
  #define REG_MLB_MCTL  (*(__IO uint32_t*)0x400680E0U) /**< \brief (MLB) MIF Control Register */
  #define REG_MLB_MADR  (*(__IO uint32_t*)0x400680E4U) /**< \brief (MLB) MIF Address Register */
  #define REG_MLB_ACTL  (*(__IO uint32_t*)0x400683C0U) /**< \brief (MLB) AHB Control Register */
  #define REG_MLB_ACSR  (*(__IO uint32_t*)0x400683D0U) /**< \brief (MLB) AHB Channel Status 0 Register */
  #define REG_MLB_ACMR  (*(__IO uint32_t*)0x400683D8U) /**< \brief (MLB) AHB Channel Mask 0 Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAMV71_MLB_INSTANCE_ */
