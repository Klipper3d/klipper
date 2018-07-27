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

#ifndef _SAM4E_CAN0_INSTANCE_
#define _SAM4E_CAN0_INSTANCE_

/* ========== Register definition for CAN0 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_CAN0_MR                       (0x40010000U) /**< \brief (CAN0) Mode Register */
  #define REG_CAN0_IER                      (0x40010004U) /**< \brief (CAN0) Interrupt Enable Register */
  #define REG_CAN0_IDR                      (0x40010008U) /**< \brief (CAN0) Interrupt Disable Register */
  #define REG_CAN0_IMR                      (0x4001000CU) /**< \brief (CAN0) Interrupt Mask Register */
  #define REG_CAN0_SR                       (0x40010010U) /**< \brief (CAN0) Status Register */
  #define REG_CAN0_BR                       (0x40010014U) /**< \brief (CAN0) Baudrate Register */
  #define REG_CAN0_TIM                      (0x40010018U) /**< \brief (CAN0) Timer Register */
  #define REG_CAN0_TIMESTP                  (0x4001001CU) /**< \brief (CAN0) Timestamp Register */
  #define REG_CAN0_ECR                      (0x40010020U) /**< \brief (CAN0) Error Counter Register */
  #define REG_CAN0_TCR                      (0x40010024U) /**< \brief (CAN0) Transfer Command Register */
  #define REG_CAN0_ACR                      (0x40010028U) /**< \brief (CAN0) Abort Command Register */
  #define REG_CAN0_WPMR                     (0x400100E4U) /**< \brief (CAN0) Write Protect Mode Register */
  #define REG_CAN0_WPSR                     (0x400100E8U) /**< \brief (CAN0) Write Protect Status Register */
  #define REG_CAN0_MMR0                     (0x40010200U) /**< \brief (CAN0) Mailbox Mode Register (MB = 0) */
  #define REG_CAN0_MAM0                     (0x40010204U) /**< \brief (CAN0) Mailbox Acceptance Mask Register (MB = 0) */
  #define REG_CAN0_MID0                     (0x40010208U) /**< \brief (CAN0) Mailbox ID Register (MB = 0) */
  #define REG_CAN0_MFID0                    (0x4001020CU) /**< \brief (CAN0) Mailbox Family ID Register (MB = 0) */
  #define REG_CAN0_MSR0                     (0x40010210U) /**< \brief (CAN0) Mailbox Status Register (MB = 0) */
  #define REG_CAN0_MDL0                     (0x40010214U) /**< \brief (CAN0) Mailbox Data Low Register (MB = 0) */
  #define REG_CAN0_MDH0                     (0x40010218U) /**< \brief (CAN0) Mailbox Data High Register (MB = 0) */
  #define REG_CAN0_MCR0                     (0x4001021CU) /**< \brief (CAN0) Mailbox Control Register (MB = 0) */
  #define REG_CAN0_MMR1                     (0x40010220U) /**< \brief (CAN0) Mailbox Mode Register (MB = 1) */
  #define REG_CAN0_MAM1                     (0x40010224U) /**< \brief (CAN0) Mailbox Acceptance Mask Register (MB = 1) */
  #define REG_CAN0_MID1                     (0x40010228U) /**< \brief (CAN0) Mailbox ID Register (MB = 1) */
  #define REG_CAN0_MFID1                    (0x4001022CU) /**< \brief (CAN0) Mailbox Family ID Register (MB = 1) */
  #define REG_CAN0_MSR1                     (0x40010230U) /**< \brief (CAN0) Mailbox Status Register (MB = 1) */
  #define REG_CAN0_MDL1                     (0x40010234U) /**< \brief (CAN0) Mailbox Data Low Register (MB = 1) */
  #define REG_CAN0_MDH1                     (0x40010238U) /**< \brief (CAN0) Mailbox Data High Register (MB = 1) */
  #define REG_CAN0_MCR1                     (0x4001023CU) /**< \brief (CAN0) Mailbox Control Register (MB = 1) */
  #define REG_CAN0_MMR2                     (0x40010240U) /**< \brief (CAN0) Mailbox Mode Register (MB = 2) */
  #define REG_CAN0_MAM2                     (0x40010244U) /**< \brief (CAN0) Mailbox Acceptance Mask Register (MB = 2) */
  #define REG_CAN0_MID2                     (0x40010248U) /**< \brief (CAN0) Mailbox ID Register (MB = 2) */
  #define REG_CAN0_MFID2                    (0x4001024CU) /**< \brief (CAN0) Mailbox Family ID Register (MB = 2) */
  #define REG_CAN0_MSR2                     (0x40010250U) /**< \brief (CAN0) Mailbox Status Register (MB = 2) */
  #define REG_CAN0_MDL2                     (0x40010254U) /**< \brief (CAN0) Mailbox Data Low Register (MB = 2) */
  #define REG_CAN0_MDH2                     (0x40010258U) /**< \brief (CAN0) Mailbox Data High Register (MB = 2) */
  #define REG_CAN0_MCR2                     (0x4001025CU) /**< \brief (CAN0) Mailbox Control Register (MB = 2) */
  #define REG_CAN0_MMR3                     (0x40010260U) /**< \brief (CAN0) Mailbox Mode Register (MB = 3) */
  #define REG_CAN0_MAM3                     (0x40010264U) /**< \brief (CAN0) Mailbox Acceptance Mask Register (MB = 3) */
  #define REG_CAN0_MID3                     (0x40010268U) /**< \brief (CAN0) Mailbox ID Register (MB = 3) */
  #define REG_CAN0_MFID3                    (0x4001026CU) /**< \brief (CAN0) Mailbox Family ID Register (MB = 3) */
  #define REG_CAN0_MSR3                     (0x40010270U) /**< \brief (CAN0) Mailbox Status Register (MB = 3) */
  #define REG_CAN0_MDL3                     (0x40010274U) /**< \brief (CAN0) Mailbox Data Low Register (MB = 3) */
  #define REG_CAN0_MDH3                     (0x40010278U) /**< \brief (CAN0) Mailbox Data High Register (MB = 3) */
  #define REG_CAN0_MCR3                     (0x4001027CU) /**< \brief (CAN0) Mailbox Control Register (MB = 3) */
  #define REG_CAN0_MMR4                     (0x40010280U) /**< \brief (CAN0) Mailbox Mode Register (MB = 4) */
  #define REG_CAN0_MAM4                     (0x40010284U) /**< \brief (CAN0) Mailbox Acceptance Mask Register (MB = 4) */
  #define REG_CAN0_MID4                     (0x40010288U) /**< \brief (CAN0) Mailbox ID Register (MB = 4) */
  #define REG_CAN0_MFID4                    (0x4001028CU) /**< \brief (CAN0) Mailbox Family ID Register (MB = 4) */
  #define REG_CAN0_MSR4                     (0x40010290U) /**< \brief (CAN0) Mailbox Status Register (MB = 4) */
  #define REG_CAN0_MDL4                     (0x40010294U) /**< \brief (CAN0) Mailbox Data Low Register (MB = 4) */
  #define REG_CAN0_MDH4                     (0x40010298U) /**< \brief (CAN0) Mailbox Data High Register (MB = 4) */
  #define REG_CAN0_MCR4                     (0x4001029CU) /**< \brief (CAN0) Mailbox Control Register (MB = 4) */
  #define REG_CAN0_MMR5                     (0x400102A0U) /**< \brief (CAN0) Mailbox Mode Register (MB = 5) */
  #define REG_CAN0_MAM5                     (0x400102A4U) /**< \brief (CAN0) Mailbox Acceptance Mask Register (MB = 5) */
  #define REG_CAN0_MID5                     (0x400102A8U) /**< \brief (CAN0) Mailbox ID Register (MB = 5) */
  #define REG_CAN0_MFID5                    (0x400102ACU) /**< \brief (CAN0) Mailbox Family ID Register (MB = 5) */
  #define REG_CAN0_MSR5                     (0x400102B0U) /**< \brief (CAN0) Mailbox Status Register (MB = 5) */
  #define REG_CAN0_MDL5                     (0x400102B4U) /**< \brief (CAN0) Mailbox Data Low Register (MB = 5) */
  #define REG_CAN0_MDH5                     (0x400102B8U) /**< \brief (CAN0) Mailbox Data High Register (MB = 5) */
  #define REG_CAN0_MCR5                     (0x400102BCU) /**< \brief (CAN0) Mailbox Control Register (MB = 5) */
  #define REG_CAN0_MMR6                     (0x400102C0U) /**< \brief (CAN0) Mailbox Mode Register (MB = 6) */
  #define REG_CAN0_MAM6                     (0x400102C4U) /**< \brief (CAN0) Mailbox Acceptance Mask Register (MB = 6) */
  #define REG_CAN0_MID6                     (0x400102C8U) /**< \brief (CAN0) Mailbox ID Register (MB = 6) */
  #define REG_CAN0_MFID6                    (0x400102CCU) /**< \brief (CAN0) Mailbox Family ID Register (MB = 6) */
  #define REG_CAN0_MSR6                     (0x400102D0U) /**< \brief (CAN0) Mailbox Status Register (MB = 6) */
  #define REG_CAN0_MDL6                     (0x400102D4U) /**< \brief (CAN0) Mailbox Data Low Register (MB = 6) */
  #define REG_CAN0_MDH6                     (0x400102D8U) /**< \brief (CAN0) Mailbox Data High Register (MB = 6) */
  #define REG_CAN0_MCR6                     (0x400102DCU) /**< \brief (CAN0) Mailbox Control Register (MB = 6) */
  #define REG_CAN0_MMR7                     (0x400102E0U) /**< \brief (CAN0) Mailbox Mode Register (MB = 7) */
  #define REG_CAN0_MAM7                     (0x400102E4U) /**< \brief (CAN0) Mailbox Acceptance Mask Register (MB = 7) */
  #define REG_CAN0_MID7                     (0x400102E8U) /**< \brief (CAN0) Mailbox ID Register (MB = 7) */
  #define REG_CAN0_MFID7                    (0x400102ECU) /**< \brief (CAN0) Mailbox Family ID Register (MB = 7) */
  #define REG_CAN0_MSR7                     (0x400102F0U) /**< \brief (CAN0) Mailbox Status Register (MB = 7) */
  #define REG_CAN0_MDL7                     (0x400102F4U) /**< \brief (CAN0) Mailbox Data Low Register (MB = 7) */
  #define REG_CAN0_MDH7                     (0x400102F8U) /**< \brief (CAN0) Mailbox Data High Register (MB = 7) */
  #define REG_CAN0_MCR7                     (0x400102FCU) /**< \brief (CAN0) Mailbox Control Register (MB = 7) */
#else
  #define REG_CAN0_MR      (*(__IO uint32_t*)0x40010000U) /**< \brief (CAN0) Mode Register */
  #define REG_CAN0_IER     (*(__O  uint32_t*)0x40010004U) /**< \brief (CAN0) Interrupt Enable Register */
  #define REG_CAN0_IDR     (*(__O  uint32_t*)0x40010008U) /**< \brief (CAN0) Interrupt Disable Register */
  #define REG_CAN0_IMR     (*(__I  uint32_t*)0x4001000CU) /**< \brief (CAN0) Interrupt Mask Register */
  #define REG_CAN0_SR      (*(__I  uint32_t*)0x40010010U) /**< \brief (CAN0) Status Register */
  #define REG_CAN0_BR      (*(__IO uint32_t*)0x40010014U) /**< \brief (CAN0) Baudrate Register */
  #define REG_CAN0_TIM     (*(__I  uint32_t*)0x40010018U) /**< \brief (CAN0) Timer Register */
  #define REG_CAN0_TIMESTP (*(__I  uint32_t*)0x4001001CU) /**< \brief (CAN0) Timestamp Register */
  #define REG_CAN0_ECR     (*(__I  uint32_t*)0x40010020U) /**< \brief (CAN0) Error Counter Register */
  #define REG_CAN0_TCR     (*(__O  uint32_t*)0x40010024U) /**< \brief (CAN0) Transfer Command Register */
  #define REG_CAN0_ACR     (*(__O  uint32_t*)0x40010028U) /**< \brief (CAN0) Abort Command Register */
  #define REG_CAN0_WPMR    (*(__IO uint32_t*)0x400100E4U) /**< \brief (CAN0) Write Protect Mode Register */
  #define REG_CAN0_WPSR    (*(__I  uint32_t*)0x400100E8U) /**< \brief (CAN0) Write Protect Status Register */
  #define REG_CAN0_MMR0    (*(__IO uint32_t*)0x40010200U) /**< \brief (CAN0) Mailbox Mode Register (MB = 0) */
  #define REG_CAN0_MAM0    (*(__IO uint32_t*)0x40010204U) /**< \brief (CAN0) Mailbox Acceptance Mask Register (MB = 0) */
  #define REG_CAN0_MID0    (*(__IO uint32_t*)0x40010208U) /**< \brief (CAN0) Mailbox ID Register (MB = 0) */
  #define REG_CAN0_MFID0   (*(__I  uint32_t*)0x4001020CU) /**< \brief (CAN0) Mailbox Family ID Register (MB = 0) */
  #define REG_CAN0_MSR0    (*(__I  uint32_t*)0x40010210U) /**< \brief (CAN0) Mailbox Status Register (MB = 0) */
  #define REG_CAN0_MDL0    (*(__IO uint32_t*)0x40010214U) /**< \brief (CAN0) Mailbox Data Low Register (MB = 0) */
  #define REG_CAN0_MDH0    (*(__IO uint32_t*)0x40010218U) /**< \brief (CAN0) Mailbox Data High Register (MB = 0) */
  #define REG_CAN0_MCR0    (*(__O  uint32_t*)0x4001021CU) /**< \brief (CAN0) Mailbox Control Register (MB = 0) */
  #define REG_CAN0_MMR1    (*(__IO uint32_t*)0x40010220U) /**< \brief (CAN0) Mailbox Mode Register (MB = 1) */
  #define REG_CAN0_MAM1    (*(__IO uint32_t*)0x40010224U) /**< \brief (CAN0) Mailbox Acceptance Mask Register (MB = 1) */
  #define REG_CAN0_MID1    (*(__IO uint32_t*)0x40010228U) /**< \brief (CAN0) Mailbox ID Register (MB = 1) */
  #define REG_CAN0_MFID1   (*(__I  uint32_t*)0x4001022CU) /**< \brief (CAN0) Mailbox Family ID Register (MB = 1) */
  #define REG_CAN0_MSR1    (*(__I  uint32_t*)0x40010230U) /**< \brief (CAN0) Mailbox Status Register (MB = 1) */
  #define REG_CAN0_MDL1    (*(__IO uint32_t*)0x40010234U) /**< \brief (CAN0) Mailbox Data Low Register (MB = 1) */
  #define REG_CAN0_MDH1    (*(__IO uint32_t*)0x40010238U) /**< \brief (CAN0) Mailbox Data High Register (MB = 1) */
  #define REG_CAN0_MCR1    (*(__O  uint32_t*)0x4001023CU) /**< \brief (CAN0) Mailbox Control Register (MB = 1) */
  #define REG_CAN0_MMR2    (*(__IO uint32_t*)0x40010240U) /**< \brief (CAN0) Mailbox Mode Register (MB = 2) */
  #define REG_CAN0_MAM2    (*(__IO uint32_t*)0x40010244U) /**< \brief (CAN0) Mailbox Acceptance Mask Register (MB = 2) */
  #define REG_CAN0_MID2    (*(__IO uint32_t*)0x40010248U) /**< \brief (CAN0) Mailbox ID Register (MB = 2) */
  #define REG_CAN0_MFID2   (*(__I  uint32_t*)0x4001024CU) /**< \brief (CAN0) Mailbox Family ID Register (MB = 2) */
  #define REG_CAN0_MSR2    (*(__I  uint32_t*)0x40010250U) /**< \brief (CAN0) Mailbox Status Register (MB = 2) */
  #define REG_CAN0_MDL2    (*(__IO uint32_t*)0x40010254U) /**< \brief (CAN0) Mailbox Data Low Register (MB = 2) */
  #define REG_CAN0_MDH2    (*(__IO uint32_t*)0x40010258U) /**< \brief (CAN0) Mailbox Data High Register (MB = 2) */
  #define REG_CAN0_MCR2    (*(__O  uint32_t*)0x4001025CU) /**< \brief (CAN0) Mailbox Control Register (MB = 2) */
  #define REG_CAN0_MMR3    (*(__IO uint32_t*)0x40010260U) /**< \brief (CAN0) Mailbox Mode Register (MB = 3) */
  #define REG_CAN0_MAM3    (*(__IO uint32_t*)0x40010264U) /**< \brief (CAN0) Mailbox Acceptance Mask Register (MB = 3) */
  #define REG_CAN0_MID3    (*(__IO uint32_t*)0x40010268U) /**< \brief (CAN0) Mailbox ID Register (MB = 3) */
  #define REG_CAN0_MFID3   (*(__I  uint32_t*)0x4001026CU) /**< \brief (CAN0) Mailbox Family ID Register (MB = 3) */
  #define REG_CAN0_MSR3    (*(__I  uint32_t*)0x40010270U) /**< \brief (CAN0) Mailbox Status Register (MB = 3) */
  #define REG_CAN0_MDL3    (*(__IO uint32_t*)0x40010274U) /**< \brief (CAN0) Mailbox Data Low Register (MB = 3) */
  #define REG_CAN0_MDH3    (*(__IO uint32_t*)0x40010278U) /**< \brief (CAN0) Mailbox Data High Register (MB = 3) */
  #define REG_CAN0_MCR3    (*(__O  uint32_t*)0x4001027CU) /**< \brief (CAN0) Mailbox Control Register (MB = 3) */
  #define REG_CAN0_MMR4    (*(__IO uint32_t*)0x40010280U) /**< \brief (CAN0) Mailbox Mode Register (MB = 4) */
  #define REG_CAN0_MAM4    (*(__IO uint32_t*)0x40010284U) /**< \brief (CAN0) Mailbox Acceptance Mask Register (MB = 4) */
  #define REG_CAN0_MID4    (*(__IO uint32_t*)0x40010288U) /**< \brief (CAN0) Mailbox ID Register (MB = 4) */
  #define REG_CAN0_MFID4   (*(__I  uint32_t*)0x4001028CU) /**< \brief (CAN0) Mailbox Family ID Register (MB = 4) */
  #define REG_CAN0_MSR4    (*(__I  uint32_t*)0x40010290U) /**< \brief (CAN0) Mailbox Status Register (MB = 4) */
  #define REG_CAN0_MDL4    (*(__IO uint32_t*)0x40010294U) /**< \brief (CAN0) Mailbox Data Low Register (MB = 4) */
  #define REG_CAN0_MDH4    (*(__IO uint32_t*)0x40010298U) /**< \brief (CAN0) Mailbox Data High Register (MB = 4) */
  #define REG_CAN0_MCR4    (*(__O  uint32_t*)0x4001029CU) /**< \brief (CAN0) Mailbox Control Register (MB = 4) */
  #define REG_CAN0_MMR5    (*(__IO uint32_t*)0x400102A0U) /**< \brief (CAN0) Mailbox Mode Register (MB = 5) */
  #define REG_CAN0_MAM5    (*(__IO uint32_t*)0x400102A4U) /**< \brief (CAN0) Mailbox Acceptance Mask Register (MB = 5) */
  #define REG_CAN0_MID5    (*(__IO uint32_t*)0x400102A8U) /**< \brief (CAN0) Mailbox ID Register (MB = 5) */
  #define REG_CAN0_MFID5   (*(__I  uint32_t*)0x400102ACU) /**< \brief (CAN0) Mailbox Family ID Register (MB = 5) */
  #define REG_CAN0_MSR5    (*(__I  uint32_t*)0x400102B0U) /**< \brief (CAN0) Mailbox Status Register (MB = 5) */
  #define REG_CAN0_MDL5    (*(__IO uint32_t*)0x400102B4U) /**< \brief (CAN0) Mailbox Data Low Register (MB = 5) */
  #define REG_CAN0_MDH5    (*(__IO uint32_t*)0x400102B8U) /**< \brief (CAN0) Mailbox Data High Register (MB = 5) */
  #define REG_CAN0_MCR5    (*(__O  uint32_t*)0x400102BCU) /**< \brief (CAN0) Mailbox Control Register (MB = 5) */
  #define REG_CAN0_MMR6    (*(__IO uint32_t*)0x400102C0U) /**< \brief (CAN0) Mailbox Mode Register (MB = 6) */
  #define REG_CAN0_MAM6    (*(__IO uint32_t*)0x400102C4U) /**< \brief (CAN0) Mailbox Acceptance Mask Register (MB = 6) */
  #define REG_CAN0_MID6    (*(__IO uint32_t*)0x400102C8U) /**< \brief (CAN0) Mailbox ID Register (MB = 6) */
  #define REG_CAN0_MFID6   (*(__I  uint32_t*)0x400102CCU) /**< \brief (CAN0) Mailbox Family ID Register (MB = 6) */
  #define REG_CAN0_MSR6    (*(__I  uint32_t*)0x400102D0U) /**< \brief (CAN0) Mailbox Status Register (MB = 6) */
  #define REG_CAN0_MDL6    (*(__IO uint32_t*)0x400102D4U) /**< \brief (CAN0) Mailbox Data Low Register (MB = 6) */
  #define REG_CAN0_MDH6    (*(__IO uint32_t*)0x400102D8U) /**< \brief (CAN0) Mailbox Data High Register (MB = 6) */
  #define REG_CAN0_MCR6    (*(__O  uint32_t*)0x400102DCU) /**< \brief (CAN0) Mailbox Control Register (MB = 6) */
  #define REG_CAN0_MMR7    (*(__IO uint32_t*)0x400102E0U) /**< \brief (CAN0) Mailbox Mode Register (MB = 7) */
  #define REG_CAN0_MAM7    (*(__IO uint32_t*)0x400102E4U) /**< \brief (CAN0) Mailbox Acceptance Mask Register (MB = 7) */
  #define REG_CAN0_MID7    (*(__IO uint32_t*)0x400102E8U) /**< \brief (CAN0) Mailbox ID Register (MB = 7) */
  #define REG_CAN0_MFID7   (*(__I  uint32_t*)0x400102ECU) /**< \brief (CAN0) Mailbox Family ID Register (MB = 7) */
  #define REG_CAN0_MSR7    (*(__I  uint32_t*)0x400102F0U) /**< \brief (CAN0) Mailbox Status Register (MB = 7) */
  #define REG_CAN0_MDL7    (*(__IO uint32_t*)0x400102F4U) /**< \brief (CAN0) Mailbox Data Low Register (MB = 7) */
  #define REG_CAN0_MDH7    (*(__IO uint32_t*)0x400102F8U) /**< \brief (CAN0) Mailbox Data High Register (MB = 7) */
  #define REG_CAN0_MCR7    (*(__O  uint32_t*)0x400102FCU) /**< \brief (CAN0) Mailbox Control Register (MB = 7) */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAM4E_CAN0_INSTANCE_ */
