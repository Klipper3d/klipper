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

#ifndef _SAM4E_DMAC_INSTANCE_
#define _SAM4E_DMAC_INSTANCE_

/* ========== Register definition for DMAC peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_DMAC_GCFG                    (0x400C0000U) /**< \brief (DMAC) DMAC Global Configuration Register */
  #define REG_DMAC_EN                      (0x400C0004U) /**< \brief (DMAC) DMAC Enable Register */
  #define REG_DMAC_SREQ                    (0x400C0008U) /**< \brief (DMAC) DMAC Software Single Request Register */
  #define REG_DMAC_CREQ                    (0x400C000CU) /**< \brief (DMAC) DMAC Software Chunk Transfer Request Register */
  #define REG_DMAC_LAST                    (0x400C0010U) /**< \brief (DMAC) DMAC Software Last Transfer Flag Register */
  #define REG_DMAC_EBCIER                  (0x400C0018U) /**< \brief (DMAC) DMAC Error, Chained Buffer Transfer Completed Interrupt and Buffer Transfer Completed Interrupt Enable register. */
  #define REG_DMAC_EBCIDR                  (0x400C001CU) /**< \brief (DMAC) DMAC Error, Chained Buffer Transfer Completed Interrupt and Buffer Transfer Completed Interrupt Disable register. */
  #define REG_DMAC_EBCIMR                  (0x400C0020U) /**< \brief (DMAC) DMAC Error, Chained Buffer Transfer Completed Interrupt and Buffer transfer completed Mask Register. */
  #define REG_DMAC_EBCISR                  (0x400C0024U) /**< \brief (DMAC) DMAC Error, Chained Buffer Transfer Completed Interrupt and Buffer transfer completed Status Register. */
  #define REG_DMAC_CHER                    (0x400C0028U) /**< \brief (DMAC) DMAC Channel Handler Enable Register */
  #define REG_DMAC_CHDR                    (0x400C002CU) /**< \brief (DMAC) DMAC Channel Handler Disable Register */
  #define REG_DMAC_CHSR                    (0x400C0030U) /**< \brief (DMAC) DMAC Channel Handler Status Register */
  #define REG_DMAC_SADDR0                  (0x400C003CU) /**< \brief (DMAC) DMAC Channel Source Address Register (ch_num = 0) */
  #define REG_DMAC_DADDR0                  (0x400C0040U) /**< \brief (DMAC) DMAC Channel Destination Address Register (ch_num = 0) */
  #define REG_DMAC_DSCR0                   (0x400C0044U) /**< \brief (DMAC) DMAC Channel Descriptor Address Register (ch_num = 0) */
  #define REG_DMAC_CTRLA0                  (0x400C0048U) /**< \brief (DMAC) DMAC Channel Control A Register (ch_num = 0) */
  #define REG_DMAC_CTRLB0                  (0x400C004CU) /**< \brief (DMAC) DMAC Channel Control B Register (ch_num = 0) */
  #define REG_DMAC_CFG0                    (0x400C0050U) /**< \brief (DMAC) DMAC Channel Configuration Register (ch_num = 0) */
  #define REG_DMAC_SADDR1                  (0x400C0064U) /**< \brief (DMAC) DMAC Channel Source Address Register (ch_num = 1) */
  #define REG_DMAC_DADDR1                  (0x400C0068U) /**< \brief (DMAC) DMAC Channel Destination Address Register (ch_num = 1) */
  #define REG_DMAC_DSCR1                   (0x400C006CU) /**< \brief (DMAC) DMAC Channel Descriptor Address Register (ch_num = 1) */
  #define REG_DMAC_CTRLA1                  (0x400C0070U) /**< \brief (DMAC) DMAC Channel Control A Register (ch_num = 1) */
  #define REG_DMAC_CTRLB1                  (0x400C0074U) /**< \brief (DMAC) DMAC Channel Control B Register (ch_num = 1) */
  #define REG_DMAC_CFG1                    (0x400C0078U) /**< \brief (DMAC) DMAC Channel Configuration Register (ch_num = 1) */
  #define REG_DMAC_SADDR2                  (0x400C008CU) /**< \brief (DMAC) DMAC Channel Source Address Register (ch_num = 2) */
  #define REG_DMAC_DADDR2                  (0x400C0090U) /**< \brief (DMAC) DMAC Channel Destination Address Register (ch_num = 2) */
  #define REG_DMAC_DSCR2                   (0x400C0094U) /**< \brief (DMAC) DMAC Channel Descriptor Address Register (ch_num = 2) */
  #define REG_DMAC_CTRLA2                  (0x400C0098U) /**< \brief (DMAC) DMAC Channel Control A Register (ch_num = 2) */
  #define REG_DMAC_CTRLB2                  (0x400C009CU) /**< \brief (DMAC) DMAC Channel Control B Register (ch_num = 2) */
  #define REG_DMAC_CFG2                    (0x400C00A0U) /**< \brief (DMAC) DMAC Channel Configuration Register (ch_num = 2) */
  #define REG_DMAC_SADDR3                  (0x400C00B4U) /**< \brief (DMAC) DMAC Channel Source Address Register (ch_num = 3) */
  #define REG_DMAC_DADDR3                  (0x400C00B8U) /**< \brief (DMAC) DMAC Channel Destination Address Register (ch_num = 3) */
  #define REG_DMAC_DSCR3                   (0x400C00BCU) /**< \brief (DMAC) DMAC Channel Descriptor Address Register (ch_num = 3) */
  #define REG_DMAC_CTRLA3                  (0x400C00C0U) /**< \brief (DMAC) DMAC Channel Control A Register (ch_num = 3) */
  #define REG_DMAC_CTRLB3                  (0x400C00C4U) /**< \brief (DMAC) DMAC Channel Control B Register (ch_num = 3) */
  #define REG_DMAC_CFG3                    (0x400C00C8U) /**< \brief (DMAC) DMAC Channel Configuration Register (ch_num = 3) */
  #define REG_DMAC_WPMR                    (0x400C01E4U) /**< \brief (DMAC) DMAC Write Protect Mode Register */
  #define REG_DMAC_WPSR                    (0x400C01E8U) /**< \brief (DMAC) DMAC Write Protect Status Register */
#else
  #define REG_DMAC_GCFG   (*(__IO uint32_t*)0x400C0000U) /**< \brief (DMAC) DMAC Global Configuration Register */
  #define REG_DMAC_EN     (*(__IO uint32_t*)0x400C0004U) /**< \brief (DMAC) DMAC Enable Register */
  #define REG_DMAC_SREQ   (*(__IO uint32_t*)0x400C0008U) /**< \brief (DMAC) DMAC Software Single Request Register */
  #define REG_DMAC_CREQ   (*(__IO uint32_t*)0x400C000CU) /**< \brief (DMAC) DMAC Software Chunk Transfer Request Register */
  #define REG_DMAC_LAST   (*(__IO uint32_t*)0x400C0010U) /**< \brief (DMAC) DMAC Software Last Transfer Flag Register */
  #define REG_DMAC_EBCIER (*(__O  uint32_t*)0x400C0018U) /**< \brief (DMAC) DMAC Error, Chained Buffer Transfer Completed Interrupt and Buffer Transfer Completed Interrupt Enable register. */
  #define REG_DMAC_EBCIDR (*(__O  uint32_t*)0x400C001CU) /**< \brief (DMAC) DMAC Error, Chained Buffer Transfer Completed Interrupt and Buffer Transfer Completed Interrupt Disable register. */
  #define REG_DMAC_EBCIMR (*(__I  uint32_t*)0x400C0020U) /**< \brief (DMAC) DMAC Error, Chained Buffer Transfer Completed Interrupt and Buffer transfer completed Mask Register. */
  #define REG_DMAC_EBCISR (*(__I  uint32_t*)0x400C0024U) /**< \brief (DMAC) DMAC Error, Chained Buffer Transfer Completed Interrupt and Buffer transfer completed Status Register. */
  #define REG_DMAC_CHER   (*(__O  uint32_t*)0x400C0028U) /**< \brief (DMAC) DMAC Channel Handler Enable Register */
  #define REG_DMAC_CHDR   (*(__O  uint32_t*)0x400C002CU) /**< \brief (DMAC) DMAC Channel Handler Disable Register */
  #define REG_DMAC_CHSR   (*(__I  uint32_t*)0x400C0030U) /**< \brief (DMAC) DMAC Channel Handler Status Register */
  #define REG_DMAC_SADDR0 (*(__IO uint32_t*)0x400C003CU) /**< \brief (DMAC) DMAC Channel Source Address Register (ch_num = 0) */
  #define REG_DMAC_DADDR0 (*(__IO uint32_t*)0x400C0040U) /**< \brief (DMAC) DMAC Channel Destination Address Register (ch_num = 0) */
  #define REG_DMAC_DSCR0  (*(__IO uint32_t*)0x400C0044U) /**< \brief (DMAC) DMAC Channel Descriptor Address Register (ch_num = 0) */
  #define REG_DMAC_CTRLA0 (*(__IO uint32_t*)0x400C0048U) /**< \brief (DMAC) DMAC Channel Control A Register (ch_num = 0) */
  #define REG_DMAC_CTRLB0 (*(__IO uint32_t*)0x400C004CU) /**< \brief (DMAC) DMAC Channel Control B Register (ch_num = 0) */
  #define REG_DMAC_CFG0   (*(__IO uint32_t*)0x400C0050U) /**< \brief (DMAC) DMAC Channel Configuration Register (ch_num = 0) */
  #define REG_DMAC_SADDR1 (*(__IO uint32_t*)0x400C0064U) /**< \brief (DMAC) DMAC Channel Source Address Register (ch_num = 1) */
  #define REG_DMAC_DADDR1 (*(__IO uint32_t*)0x400C0068U) /**< \brief (DMAC) DMAC Channel Destination Address Register (ch_num = 1) */
  #define REG_DMAC_DSCR1  (*(__IO uint32_t*)0x400C006CU) /**< \brief (DMAC) DMAC Channel Descriptor Address Register (ch_num = 1) */
  #define REG_DMAC_CTRLA1 (*(__IO uint32_t*)0x400C0070U) /**< \brief (DMAC) DMAC Channel Control A Register (ch_num = 1) */
  #define REG_DMAC_CTRLB1 (*(__IO uint32_t*)0x400C0074U) /**< \brief (DMAC) DMAC Channel Control B Register (ch_num = 1) */
  #define REG_DMAC_CFG1   (*(__IO uint32_t*)0x400C0078U) /**< \brief (DMAC) DMAC Channel Configuration Register (ch_num = 1) */
  #define REG_DMAC_SADDR2 (*(__IO uint32_t*)0x400C008CU) /**< \brief (DMAC) DMAC Channel Source Address Register (ch_num = 2) */
  #define REG_DMAC_DADDR2 (*(__IO uint32_t*)0x400C0090U) /**< \brief (DMAC) DMAC Channel Destination Address Register (ch_num = 2) */
  #define REG_DMAC_DSCR2  (*(__IO uint32_t*)0x400C0094U) /**< \brief (DMAC) DMAC Channel Descriptor Address Register (ch_num = 2) */
  #define REG_DMAC_CTRLA2 (*(__IO uint32_t*)0x400C0098U) /**< \brief (DMAC) DMAC Channel Control A Register (ch_num = 2) */
  #define REG_DMAC_CTRLB2 (*(__IO uint32_t*)0x400C009CU) /**< \brief (DMAC) DMAC Channel Control B Register (ch_num = 2) */
  #define REG_DMAC_CFG2   (*(__IO uint32_t*)0x400C00A0U) /**< \brief (DMAC) DMAC Channel Configuration Register (ch_num = 2) */
  #define REG_DMAC_SADDR3 (*(__IO uint32_t*)0x400C00B4U) /**< \brief (DMAC) DMAC Channel Source Address Register (ch_num = 3) */
  #define REG_DMAC_DADDR3 (*(__IO uint32_t*)0x400C00B8U) /**< \brief (DMAC) DMAC Channel Destination Address Register (ch_num = 3) */
  #define REG_DMAC_DSCR3  (*(__IO uint32_t*)0x400C00BCU) /**< \brief (DMAC) DMAC Channel Descriptor Address Register (ch_num = 3) */
  #define REG_DMAC_CTRLA3 (*(__IO uint32_t*)0x400C00C0U) /**< \brief (DMAC) DMAC Channel Control A Register (ch_num = 3) */
  #define REG_DMAC_CTRLB3 (*(__IO uint32_t*)0x400C00C4U) /**< \brief (DMAC) DMAC Channel Control B Register (ch_num = 3) */
  #define REG_DMAC_CFG3   (*(__IO uint32_t*)0x400C00C8U) /**< \brief (DMAC) DMAC Channel Configuration Register (ch_num = 3) */
  #define REG_DMAC_WPMR   (*(__IO uint32_t*)0x400C01E4U) /**< \brief (DMAC) DMAC Write Protect Mode Register */
  #define REG_DMAC_WPSR   (*(__I  uint32_t*)0x400C01E8U) /**< \brief (DMAC) DMAC Write Protect Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAM4E_DMAC_INSTANCE_ */
