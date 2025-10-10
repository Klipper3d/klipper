/* ---------------------------------------------------------------------------- */
/*                  Atmel Microcontroller Software Support                      */
/*                       SAM Software Package License                           */
/* ---------------------------------------------------------------------------- */
/* Copyright (c) %copyright_year%, Atmel Corporation                                        */
/*                                                                              */
/* All rights reserved.                                                         */
/*                                                                              */
/* Redistribution and use in source and binary forms, with or without           */
/* modification, are permitted provided that the following condition is met:    */
/*                                                                              */
/* - Redistributions of source code must retain the above copyright notice,     */
/* this list of conditions and the disclaimer below.                            */
/*                                                                              */
/* Atmel's name may not be used to endorse or promote products derived from     */
/* this software without specific prior written permission.                     */
/*                                                                              */
/* DISCLAIMER:  THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR   */
/* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE   */
/* DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,      */
/* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT */
/* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,  */
/* OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF    */
/* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING         */
/* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, */
/* EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                           */
/* ---------------------------------------------------------------------------- */

#ifndef _SAM4E_CAN0_INSTANCE_
#define _SAM4E_CAN0_INSTANCE_

/* ========== Register definition for CAN0 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_CAN0_MR               (0x40010000U) /**< \brief (CAN0) Mode Register */
#define REG_CAN0_IER              (0x40010004U) /**< \brief (CAN0) Interrupt Enable Register */
#define REG_CAN0_IDR              (0x40010008U) /**< \brief (CAN0) Interrupt Disable Register */
#define REG_CAN0_IMR              (0x4001000CU) /**< \brief (CAN0) Interrupt Mask Register */
#define REG_CAN0_SR               (0x40010010U) /**< \brief (CAN0) Status Register */
#define REG_CAN0_BR               (0x40010014U) /**< \brief (CAN0) Baudrate Register */
#define REG_CAN0_TIM              (0x40010018U) /**< \brief (CAN0) Timer Register */
#define REG_CAN0_TIMESTP          (0x4001001CU) /**< \brief (CAN0) Timestamp Register */
#define REG_CAN0_ECR              (0x40010020U) /**< \brief (CAN0) Error Counter Register */
#define REG_CAN0_TCR              (0x40010024U) /**< \brief (CAN0) Transfer Command Register */
#define REG_CAN0_ACR              (0x40010028U) /**< \brief (CAN0) Abort Command Register */
#define REG_CAN0_WPMR             (0x400100E4U) /**< \brief (CAN0) Write Protect Mode Register */
#define REG_CAN0_WPSR             (0x400100E8U) /**< \brief (CAN0) Write Protect Status Register */
#define REG_CAN0_MMR0             (0x40010200U) /**< \brief (CAN0) Mailbox Mode Register (MB = 0) */
#define REG_CAN0_MAM0             (0x40010204U) /**< \brief (CAN0) Mailbox Acceptance Mask Register (MB = 0) */
#define REG_CAN0_MID0             (0x40010208U) /**< \brief (CAN0) Mailbox ID Register (MB = 0) */
#define REG_CAN0_MFID0            (0x4001020CU) /**< \brief (CAN0) Mailbox Family ID Register (MB = 0) */
#define REG_CAN0_MSR0             (0x40010210U) /**< \brief (CAN0) Mailbox Status Register (MB = 0) */
#define REG_CAN0_MDL0             (0x40010214U) /**< \brief (CAN0) Mailbox Data Low Register (MB = 0) */
#define REG_CAN0_MDH0             (0x40010218U) /**< \brief (CAN0) Mailbox Data High Register (MB = 0) */
#define REG_CAN0_MCR0             (0x4001021CU) /**< \brief (CAN0) Mailbox Control Register (MB = 0) */
#define REG_CAN0_MMR1             (0x40010220U) /**< \brief (CAN0) Mailbox Mode Register (MB = 1) */
#define REG_CAN0_MAM1             (0x40010224U) /**< \brief (CAN0) Mailbox Acceptance Mask Register (MB = 1) */
#define REG_CAN0_MID1             (0x40010228U) /**< \brief (CAN0) Mailbox ID Register (MB = 1) */
#define REG_CAN0_MFID1            (0x4001022CU) /**< \brief (CAN0) Mailbox Family ID Register (MB = 1) */
#define REG_CAN0_MSR1             (0x40010230U) /**< \brief (CAN0) Mailbox Status Register (MB = 1) */
#define REG_CAN0_MDL1             (0x40010234U) /**< \brief (CAN0) Mailbox Data Low Register (MB = 1) */
#define REG_CAN0_MDH1             (0x40010238U) /**< \brief (CAN0) Mailbox Data High Register (MB = 1) */
#define REG_CAN0_MCR1             (0x4001023CU) /**< \brief (CAN0) Mailbox Control Register (MB = 1) */
#define REG_CAN0_MMR2             (0x40010240U) /**< \brief (CAN0) Mailbox Mode Register (MB = 2) */
#define REG_CAN0_MAM2             (0x40010244U) /**< \brief (CAN0) Mailbox Acceptance Mask Register (MB = 2) */
#define REG_CAN0_MID2             (0x40010248U) /**< \brief (CAN0) Mailbox ID Register (MB = 2) */
#define REG_CAN0_MFID2            (0x4001024CU) /**< \brief (CAN0) Mailbox Family ID Register (MB = 2) */
#define REG_CAN0_MSR2             (0x40010250U) /**< \brief (CAN0) Mailbox Status Register (MB = 2) */
#define REG_CAN0_MDL2             (0x40010254U) /**< \brief (CAN0) Mailbox Data Low Register (MB = 2) */
#define REG_CAN0_MDH2             (0x40010258U) /**< \brief (CAN0) Mailbox Data High Register (MB = 2) */
#define REG_CAN0_MCR2             (0x4001025CU) /**< \brief (CAN0) Mailbox Control Register (MB = 2) */
#define REG_CAN0_MMR3             (0x40010260U) /**< \brief (CAN0) Mailbox Mode Register (MB = 3) */
#define REG_CAN0_MAM3             (0x40010264U) /**< \brief (CAN0) Mailbox Acceptance Mask Register (MB = 3) */
#define REG_CAN0_MID3             (0x40010268U) /**< \brief (CAN0) Mailbox ID Register (MB = 3) */
#define REG_CAN0_MFID3            (0x4001026CU) /**< \brief (CAN0) Mailbox Family ID Register (MB = 3) */
#define REG_CAN0_MSR3             (0x40010270U) /**< \brief (CAN0) Mailbox Status Register (MB = 3) */
#define REG_CAN0_MDL3             (0x40010274U) /**< \brief (CAN0) Mailbox Data Low Register (MB = 3) */
#define REG_CAN0_MDH3             (0x40010278U) /**< \brief (CAN0) Mailbox Data High Register (MB = 3) */
#define REG_CAN0_MCR3             (0x4001027CU) /**< \brief (CAN0) Mailbox Control Register (MB = 3) */
#define REG_CAN0_MMR4             (0x40010280U) /**< \brief (CAN0) Mailbox Mode Register (MB = 4) */
#define REG_CAN0_MAM4             (0x40010284U) /**< \brief (CAN0) Mailbox Acceptance Mask Register (MB = 4) */
#define REG_CAN0_MID4             (0x40010288U) /**< \brief (CAN0) Mailbox ID Register (MB = 4) */
#define REG_CAN0_MFID4            (0x4001028CU) /**< \brief (CAN0) Mailbox Family ID Register (MB = 4) */
#define REG_CAN0_MSR4             (0x40010290U) /**< \brief (CAN0) Mailbox Status Register (MB = 4) */
#define REG_CAN0_MDL4             (0x40010294U) /**< \brief (CAN0) Mailbox Data Low Register (MB = 4) */
#define REG_CAN0_MDH4             (0x40010298U) /**< \brief (CAN0) Mailbox Data High Register (MB = 4) */
#define REG_CAN0_MCR4             (0x4001029CU) /**< \brief (CAN0) Mailbox Control Register (MB = 4) */
#define REG_CAN0_MMR5             (0x400102A0U) /**< \brief (CAN0) Mailbox Mode Register (MB = 5) */
#define REG_CAN0_MAM5             (0x400102A4U) /**< \brief (CAN0) Mailbox Acceptance Mask Register (MB = 5) */
#define REG_CAN0_MID5             (0x400102A8U) /**< \brief (CAN0) Mailbox ID Register (MB = 5) */
#define REG_CAN0_MFID5            (0x400102ACU) /**< \brief (CAN0) Mailbox Family ID Register (MB = 5) */
#define REG_CAN0_MSR5             (0x400102B0U) /**< \brief (CAN0) Mailbox Status Register (MB = 5) */
#define REG_CAN0_MDL5             (0x400102B4U) /**< \brief (CAN0) Mailbox Data Low Register (MB = 5) */
#define REG_CAN0_MDH5             (0x400102B8U) /**< \brief (CAN0) Mailbox Data High Register (MB = 5) */
#define REG_CAN0_MCR5             (0x400102BCU) /**< \brief (CAN0) Mailbox Control Register (MB = 5) */
#define REG_CAN0_MMR6             (0x400102C0U) /**< \brief (CAN0) Mailbox Mode Register (MB = 6) */
#define REG_CAN0_MAM6             (0x400102C4U) /**< \brief (CAN0) Mailbox Acceptance Mask Register (MB = 6) */
#define REG_CAN0_MID6             (0x400102C8U) /**< \brief (CAN0) Mailbox ID Register (MB = 6) */
#define REG_CAN0_MFID6            (0x400102CCU) /**< \brief (CAN0) Mailbox Family ID Register (MB = 6) */
#define REG_CAN0_MSR6             (0x400102D0U) /**< \brief (CAN0) Mailbox Status Register (MB = 6) */
#define REG_CAN0_MDL6             (0x400102D4U) /**< \brief (CAN0) Mailbox Data Low Register (MB = 6) */
#define REG_CAN0_MDH6             (0x400102D8U) /**< \brief (CAN0) Mailbox Data High Register (MB = 6) */
#define REG_CAN0_MCR6             (0x400102DCU) /**< \brief (CAN0) Mailbox Control Register (MB = 6) */
#define REG_CAN0_MMR7             (0x400102E0U) /**< \brief (CAN0) Mailbox Mode Register (MB = 7) */
#define REG_CAN0_MAM7             (0x400102E4U) /**< \brief (CAN0) Mailbox Acceptance Mask Register (MB = 7) */
#define REG_CAN0_MID7             (0x400102E8U) /**< \brief (CAN0) Mailbox ID Register (MB = 7) */
#define REG_CAN0_MFID7            (0x400102ECU) /**< \brief (CAN0) Mailbox Family ID Register (MB = 7) */
#define REG_CAN0_MSR7             (0x400102F0U) /**< \brief (CAN0) Mailbox Status Register (MB = 7) */
#define REG_CAN0_MDL7             (0x400102F4U) /**< \brief (CAN0) Mailbox Data Low Register (MB = 7) */
#define REG_CAN0_MDH7             (0x400102F8U) /**< \brief (CAN0) Mailbox Data High Register (MB = 7) */
#define REG_CAN0_MCR7             (0x400102FCU) /**< \brief (CAN0) Mailbox Control Register (MB = 7) */
#else
#define REG_CAN0_MR      (*(RwReg*)0x40010000U) /**< \brief (CAN0) Mode Register */
#define REG_CAN0_IER     (*(WoReg*)0x40010004U) /**< \brief (CAN0) Interrupt Enable Register */
#define REG_CAN0_IDR     (*(WoReg*)0x40010008U) /**< \brief (CAN0) Interrupt Disable Register */
#define REG_CAN0_IMR     (*(RoReg*)0x4001000CU) /**< \brief (CAN0) Interrupt Mask Register */
#define REG_CAN0_SR      (*(RoReg*)0x40010010U) /**< \brief (CAN0) Status Register */
#define REG_CAN0_BR      (*(RwReg*)0x40010014U) /**< \brief (CAN0) Baudrate Register */
#define REG_CAN0_TIM     (*(RoReg*)0x40010018U) /**< \brief (CAN0) Timer Register */
#define REG_CAN0_TIMESTP (*(RoReg*)0x4001001CU) /**< \brief (CAN0) Timestamp Register */
#define REG_CAN0_ECR     (*(RoReg*)0x40010020U) /**< \brief (CAN0) Error Counter Register */
#define REG_CAN0_TCR     (*(WoReg*)0x40010024U) /**< \brief (CAN0) Transfer Command Register */
#define REG_CAN0_ACR     (*(WoReg*)0x40010028U) /**< \brief (CAN0) Abort Command Register */
#define REG_CAN0_WPMR    (*(RwReg*)0x400100E4U) /**< \brief (CAN0) Write Protect Mode Register */
#define REG_CAN0_WPSR    (*(RoReg*)0x400100E8U) /**< \brief (CAN0) Write Protect Status Register */
#define REG_CAN0_MMR0    (*(RwReg*)0x40010200U) /**< \brief (CAN0) Mailbox Mode Register (MB = 0) */
#define REG_CAN0_MAM0    (*(RwReg*)0x40010204U) /**< \brief (CAN0) Mailbox Acceptance Mask Register (MB = 0) */
#define REG_CAN0_MID0    (*(RwReg*)0x40010208U) /**< \brief (CAN0) Mailbox ID Register (MB = 0) */
#define REG_CAN0_MFID0   (*(RoReg*)0x4001020CU) /**< \brief (CAN0) Mailbox Family ID Register (MB = 0) */
#define REG_CAN0_MSR0    (*(RoReg*)0x40010210U) /**< \brief (CAN0) Mailbox Status Register (MB = 0) */
#define REG_CAN0_MDL0    (*(RwReg*)0x40010214U) /**< \brief (CAN0) Mailbox Data Low Register (MB = 0) */
#define REG_CAN0_MDH0    (*(RwReg*)0x40010218U) /**< \brief (CAN0) Mailbox Data High Register (MB = 0) */
#define REG_CAN0_MCR0    (*(WoReg*)0x4001021CU) /**< \brief (CAN0) Mailbox Control Register (MB = 0) */
#define REG_CAN0_MMR1    (*(RwReg*)0x40010220U) /**< \brief (CAN0) Mailbox Mode Register (MB = 1) */
#define REG_CAN0_MAM1    (*(RwReg*)0x40010224U) /**< \brief (CAN0) Mailbox Acceptance Mask Register (MB = 1) */
#define REG_CAN0_MID1    (*(RwReg*)0x40010228U) /**< \brief (CAN0) Mailbox ID Register (MB = 1) */
#define REG_CAN0_MFID1   (*(RoReg*)0x4001022CU) /**< \brief (CAN0) Mailbox Family ID Register (MB = 1) */
#define REG_CAN0_MSR1    (*(RoReg*)0x40010230U) /**< \brief (CAN0) Mailbox Status Register (MB = 1) */
#define REG_CAN0_MDL1    (*(RwReg*)0x40010234U) /**< \brief (CAN0) Mailbox Data Low Register (MB = 1) */
#define REG_CAN0_MDH1    (*(RwReg*)0x40010238U) /**< \brief (CAN0) Mailbox Data High Register (MB = 1) */
#define REG_CAN0_MCR1    (*(WoReg*)0x4001023CU) /**< \brief (CAN0) Mailbox Control Register (MB = 1) */
#define REG_CAN0_MMR2    (*(RwReg*)0x40010240U) /**< \brief (CAN0) Mailbox Mode Register (MB = 2) */
#define REG_CAN0_MAM2    (*(RwReg*)0x40010244U) /**< \brief (CAN0) Mailbox Acceptance Mask Register (MB = 2) */
#define REG_CAN0_MID2    (*(RwReg*)0x40010248U) /**< \brief (CAN0) Mailbox ID Register (MB = 2) */
#define REG_CAN0_MFID2   (*(RoReg*)0x4001024CU) /**< \brief (CAN0) Mailbox Family ID Register (MB = 2) */
#define REG_CAN0_MSR2    (*(RoReg*)0x40010250U) /**< \brief (CAN0) Mailbox Status Register (MB = 2) */
#define REG_CAN0_MDL2    (*(RwReg*)0x40010254U) /**< \brief (CAN0) Mailbox Data Low Register (MB = 2) */
#define REG_CAN0_MDH2    (*(RwReg*)0x40010258U) /**< \brief (CAN0) Mailbox Data High Register (MB = 2) */
#define REG_CAN0_MCR2    (*(WoReg*)0x4001025CU) /**< \brief (CAN0) Mailbox Control Register (MB = 2) */
#define REG_CAN0_MMR3    (*(RwReg*)0x40010260U) /**< \brief (CAN0) Mailbox Mode Register (MB = 3) */
#define REG_CAN0_MAM3    (*(RwReg*)0x40010264U) /**< \brief (CAN0) Mailbox Acceptance Mask Register (MB = 3) */
#define REG_CAN0_MID3    (*(RwReg*)0x40010268U) /**< \brief (CAN0) Mailbox ID Register (MB = 3) */
#define REG_CAN0_MFID3   (*(RoReg*)0x4001026CU) /**< \brief (CAN0) Mailbox Family ID Register (MB = 3) */
#define REG_CAN0_MSR3    (*(RoReg*)0x40010270U) /**< \brief (CAN0) Mailbox Status Register (MB = 3) */
#define REG_CAN0_MDL3    (*(RwReg*)0x40010274U) /**< \brief (CAN0) Mailbox Data Low Register (MB = 3) */
#define REG_CAN0_MDH3    (*(RwReg*)0x40010278U) /**< \brief (CAN0) Mailbox Data High Register (MB = 3) */
#define REG_CAN0_MCR3    (*(WoReg*)0x4001027CU) /**< \brief (CAN0) Mailbox Control Register (MB = 3) */
#define REG_CAN0_MMR4    (*(RwReg*)0x40010280U) /**< \brief (CAN0) Mailbox Mode Register (MB = 4) */
#define REG_CAN0_MAM4    (*(RwReg*)0x40010284U) /**< \brief (CAN0) Mailbox Acceptance Mask Register (MB = 4) */
#define REG_CAN0_MID4    (*(RwReg*)0x40010288U) /**< \brief (CAN0) Mailbox ID Register (MB = 4) */
#define REG_CAN0_MFID4   (*(RoReg*)0x4001028CU) /**< \brief (CAN0) Mailbox Family ID Register (MB = 4) */
#define REG_CAN0_MSR4    (*(RoReg*)0x40010290U) /**< \brief (CAN0) Mailbox Status Register (MB = 4) */
#define REG_CAN0_MDL4    (*(RwReg*)0x40010294U) /**< \brief (CAN0) Mailbox Data Low Register (MB = 4) */
#define REG_CAN0_MDH4    (*(RwReg*)0x40010298U) /**< \brief (CAN0) Mailbox Data High Register (MB = 4) */
#define REG_CAN0_MCR4    (*(WoReg*)0x4001029CU) /**< \brief (CAN0) Mailbox Control Register (MB = 4) */
#define REG_CAN0_MMR5    (*(RwReg*)0x400102A0U) /**< \brief (CAN0) Mailbox Mode Register (MB = 5) */
#define REG_CAN0_MAM5    (*(RwReg*)0x400102A4U) /**< \brief (CAN0) Mailbox Acceptance Mask Register (MB = 5) */
#define REG_CAN0_MID5    (*(RwReg*)0x400102A8U) /**< \brief (CAN0) Mailbox ID Register (MB = 5) */
#define REG_CAN0_MFID5   (*(RoReg*)0x400102ACU) /**< \brief (CAN0) Mailbox Family ID Register (MB = 5) */
#define REG_CAN0_MSR5    (*(RoReg*)0x400102B0U) /**< \brief (CAN0) Mailbox Status Register (MB = 5) */
#define REG_CAN0_MDL5    (*(RwReg*)0x400102B4U) /**< \brief (CAN0) Mailbox Data Low Register (MB = 5) */
#define REG_CAN0_MDH5    (*(RwReg*)0x400102B8U) /**< \brief (CAN0) Mailbox Data High Register (MB = 5) */
#define REG_CAN0_MCR5    (*(WoReg*)0x400102BCU) /**< \brief (CAN0) Mailbox Control Register (MB = 5) */
#define REG_CAN0_MMR6    (*(RwReg*)0x400102C0U) /**< \brief (CAN0) Mailbox Mode Register (MB = 6) */
#define REG_CAN0_MAM6    (*(RwReg*)0x400102C4U) /**< \brief (CAN0) Mailbox Acceptance Mask Register (MB = 6) */
#define REG_CAN0_MID6    (*(RwReg*)0x400102C8U) /**< \brief (CAN0) Mailbox ID Register (MB = 6) */
#define REG_CAN0_MFID6   (*(RoReg*)0x400102CCU) /**< \brief (CAN0) Mailbox Family ID Register (MB = 6) */
#define REG_CAN0_MSR6    (*(RoReg*)0x400102D0U) /**< \brief (CAN0) Mailbox Status Register (MB = 6) */
#define REG_CAN0_MDL6    (*(RwReg*)0x400102D4U) /**< \brief (CAN0) Mailbox Data Low Register (MB = 6) */
#define REG_CAN0_MDH6    (*(RwReg*)0x400102D8U) /**< \brief (CAN0) Mailbox Data High Register (MB = 6) */
#define REG_CAN0_MCR6    (*(WoReg*)0x400102DCU) /**< \brief (CAN0) Mailbox Control Register (MB = 6) */
#define REG_CAN0_MMR7    (*(RwReg*)0x400102E0U) /**< \brief (CAN0) Mailbox Mode Register (MB = 7) */
#define REG_CAN0_MAM7    (*(RwReg*)0x400102E4U) /**< \brief (CAN0) Mailbox Acceptance Mask Register (MB = 7) */
#define REG_CAN0_MID7    (*(RwReg*)0x400102E8U) /**< \brief (CAN0) Mailbox ID Register (MB = 7) */
#define REG_CAN0_MFID7   (*(RoReg*)0x400102ECU) /**< \brief (CAN0) Mailbox Family ID Register (MB = 7) */
#define REG_CAN0_MSR7    (*(RoReg*)0x400102F0U) /**< \brief (CAN0) Mailbox Status Register (MB = 7) */
#define REG_CAN0_MDL7    (*(RwReg*)0x400102F4U) /**< \brief (CAN0) Mailbox Data Low Register (MB = 7) */
#define REG_CAN0_MDH7    (*(RwReg*)0x400102F8U) /**< \brief (CAN0) Mailbox Data High Register (MB = 7) */
#define REG_CAN0_MCR7    (*(WoReg*)0x400102FCU) /**< \brief (CAN0) Mailbox Control Register (MB = 7) */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAM4E_CAN0_INSTANCE_ */
