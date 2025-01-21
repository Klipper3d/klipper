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

#ifndef _SAM4E_CAN1_INSTANCE_
#define _SAM4E_CAN1_INSTANCE_

/* ========== Register definition for CAN1 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_CAN1_MR               (0x40014000U) /**< \brief (CAN1) Mode Register */
#define REG_CAN1_IER              (0x40014004U) /**< \brief (CAN1) Interrupt Enable Register */
#define REG_CAN1_IDR              (0x40014008U) /**< \brief (CAN1) Interrupt Disable Register */
#define REG_CAN1_IMR              (0x4001400CU) /**< \brief (CAN1) Interrupt Mask Register */
#define REG_CAN1_SR               (0x40014010U) /**< \brief (CAN1) Status Register */
#define REG_CAN1_BR               (0x40014014U) /**< \brief (CAN1) Baudrate Register */
#define REG_CAN1_TIM              (0x40014018U) /**< \brief (CAN1) Timer Register */
#define REG_CAN1_TIMESTP          (0x4001401CU) /**< \brief (CAN1) Timestamp Register */
#define REG_CAN1_ECR              (0x40014020U) /**< \brief (CAN1) Error Counter Register */
#define REG_CAN1_TCR              (0x40014024U) /**< \brief (CAN1) Transfer Command Register */
#define REG_CAN1_ACR              (0x40014028U) /**< \brief (CAN1) Abort Command Register */
#define REG_CAN1_WPMR             (0x400140E4U) /**< \brief (CAN1) Write Protect Mode Register */
#define REG_CAN1_WPSR             (0x400140E8U) /**< \brief (CAN1) Write Protect Status Register */
#define REG_CAN1_MMR0             (0x40014200U) /**< \brief (CAN1) Mailbox Mode Register (MB = 0) */
#define REG_CAN1_MAM0             (0x40014204U) /**< \brief (CAN1) Mailbox Acceptance Mask Register (MB = 0) */
#define REG_CAN1_MID0             (0x40014208U) /**< \brief (CAN1) Mailbox ID Register (MB = 0) */
#define REG_CAN1_MFID0            (0x4001420CU) /**< \brief (CAN1) Mailbox Family ID Register (MB = 0) */
#define REG_CAN1_MSR0             (0x40014210U) /**< \brief (CAN1) Mailbox Status Register (MB = 0) */
#define REG_CAN1_MDL0             (0x40014214U) /**< \brief (CAN1) Mailbox Data Low Register (MB = 0) */
#define REG_CAN1_MDH0             (0x40014218U) /**< \brief (CAN1) Mailbox Data High Register (MB = 0) */
#define REG_CAN1_MCR0             (0x4001421CU) /**< \brief (CAN1) Mailbox Control Register (MB = 0) */
#define REG_CAN1_MMR1             (0x40014220U) /**< \brief (CAN1) Mailbox Mode Register (MB = 1) */
#define REG_CAN1_MAM1             (0x40014224U) /**< \brief (CAN1) Mailbox Acceptance Mask Register (MB = 1) */
#define REG_CAN1_MID1             (0x40014228U) /**< \brief (CAN1) Mailbox ID Register (MB = 1) */
#define REG_CAN1_MFID1            (0x4001422CU) /**< \brief (CAN1) Mailbox Family ID Register (MB = 1) */
#define REG_CAN1_MSR1             (0x40014230U) /**< \brief (CAN1) Mailbox Status Register (MB = 1) */
#define REG_CAN1_MDL1             (0x40014234U) /**< \brief (CAN1) Mailbox Data Low Register (MB = 1) */
#define REG_CAN1_MDH1             (0x40014238U) /**< \brief (CAN1) Mailbox Data High Register (MB = 1) */
#define REG_CAN1_MCR1             (0x4001423CU) /**< \brief (CAN1) Mailbox Control Register (MB = 1) */
#define REG_CAN1_MMR2             (0x40014240U) /**< \brief (CAN1) Mailbox Mode Register (MB = 2) */
#define REG_CAN1_MAM2             (0x40014244U) /**< \brief (CAN1) Mailbox Acceptance Mask Register (MB = 2) */
#define REG_CAN1_MID2             (0x40014248U) /**< \brief (CAN1) Mailbox ID Register (MB = 2) */
#define REG_CAN1_MFID2            (0x4001424CU) /**< \brief (CAN1) Mailbox Family ID Register (MB = 2) */
#define REG_CAN1_MSR2             (0x40014250U) /**< \brief (CAN1) Mailbox Status Register (MB = 2) */
#define REG_CAN1_MDL2             (0x40014254U) /**< \brief (CAN1) Mailbox Data Low Register (MB = 2) */
#define REG_CAN1_MDH2             (0x40014258U) /**< \brief (CAN1) Mailbox Data High Register (MB = 2) */
#define REG_CAN1_MCR2             (0x4001425CU) /**< \brief (CAN1) Mailbox Control Register (MB = 2) */
#define REG_CAN1_MMR3             (0x40014260U) /**< \brief (CAN1) Mailbox Mode Register (MB = 3) */
#define REG_CAN1_MAM3             (0x40014264U) /**< \brief (CAN1) Mailbox Acceptance Mask Register (MB = 3) */
#define REG_CAN1_MID3             (0x40014268U) /**< \brief (CAN1) Mailbox ID Register (MB = 3) */
#define REG_CAN1_MFID3            (0x4001426CU) /**< \brief (CAN1) Mailbox Family ID Register (MB = 3) */
#define REG_CAN1_MSR3             (0x40014270U) /**< \brief (CAN1) Mailbox Status Register (MB = 3) */
#define REG_CAN1_MDL3             (0x40014274U) /**< \brief (CAN1) Mailbox Data Low Register (MB = 3) */
#define REG_CAN1_MDH3             (0x40014278U) /**< \brief (CAN1) Mailbox Data High Register (MB = 3) */
#define REG_CAN1_MCR3             (0x4001427CU) /**< \brief (CAN1) Mailbox Control Register (MB = 3) */
#define REG_CAN1_MMR4             (0x40014280U) /**< \brief (CAN1) Mailbox Mode Register (MB = 4) */
#define REG_CAN1_MAM4             (0x40014284U) /**< \brief (CAN1) Mailbox Acceptance Mask Register (MB = 4) */
#define REG_CAN1_MID4             (0x40014288U) /**< \brief (CAN1) Mailbox ID Register (MB = 4) */
#define REG_CAN1_MFID4            (0x4001428CU) /**< \brief (CAN1) Mailbox Family ID Register (MB = 4) */
#define REG_CAN1_MSR4             (0x40014290U) /**< \brief (CAN1) Mailbox Status Register (MB = 4) */
#define REG_CAN1_MDL4             (0x40014294U) /**< \brief (CAN1) Mailbox Data Low Register (MB = 4) */
#define REG_CAN1_MDH4             (0x40014298U) /**< \brief (CAN1) Mailbox Data High Register (MB = 4) */
#define REG_CAN1_MCR4             (0x4001429CU) /**< \brief (CAN1) Mailbox Control Register (MB = 4) */
#define REG_CAN1_MMR5             (0x400142A0U) /**< \brief (CAN1) Mailbox Mode Register (MB = 5) */
#define REG_CAN1_MAM5             (0x400142A4U) /**< \brief (CAN1) Mailbox Acceptance Mask Register (MB = 5) */
#define REG_CAN1_MID5             (0x400142A8U) /**< \brief (CAN1) Mailbox ID Register (MB = 5) */
#define REG_CAN1_MFID5            (0x400142ACU) /**< \brief (CAN1) Mailbox Family ID Register (MB = 5) */
#define REG_CAN1_MSR5             (0x400142B0U) /**< \brief (CAN1) Mailbox Status Register (MB = 5) */
#define REG_CAN1_MDL5             (0x400142B4U) /**< \brief (CAN1) Mailbox Data Low Register (MB = 5) */
#define REG_CAN1_MDH5             (0x400142B8U) /**< \brief (CAN1) Mailbox Data High Register (MB = 5) */
#define REG_CAN1_MCR5             (0x400142BCU) /**< \brief (CAN1) Mailbox Control Register (MB = 5) */
#define REG_CAN1_MMR6             (0x400142C0U) /**< \brief (CAN1) Mailbox Mode Register (MB = 6) */
#define REG_CAN1_MAM6             (0x400142C4U) /**< \brief (CAN1) Mailbox Acceptance Mask Register (MB = 6) */
#define REG_CAN1_MID6             (0x400142C8U) /**< \brief (CAN1) Mailbox ID Register (MB = 6) */
#define REG_CAN1_MFID6            (0x400142CCU) /**< \brief (CAN1) Mailbox Family ID Register (MB = 6) */
#define REG_CAN1_MSR6             (0x400142D0U) /**< \brief (CAN1) Mailbox Status Register (MB = 6) */
#define REG_CAN1_MDL6             (0x400142D4U) /**< \brief (CAN1) Mailbox Data Low Register (MB = 6) */
#define REG_CAN1_MDH6             (0x400142D8U) /**< \brief (CAN1) Mailbox Data High Register (MB = 6) */
#define REG_CAN1_MCR6             (0x400142DCU) /**< \brief (CAN1) Mailbox Control Register (MB = 6) */
#define REG_CAN1_MMR7             (0x400142E0U) /**< \brief (CAN1) Mailbox Mode Register (MB = 7) */
#define REG_CAN1_MAM7             (0x400142E4U) /**< \brief (CAN1) Mailbox Acceptance Mask Register (MB = 7) */
#define REG_CAN1_MID7             (0x400142E8U) /**< \brief (CAN1) Mailbox ID Register (MB = 7) */
#define REG_CAN1_MFID7            (0x400142ECU) /**< \brief (CAN1) Mailbox Family ID Register (MB = 7) */
#define REG_CAN1_MSR7             (0x400142F0U) /**< \brief (CAN1) Mailbox Status Register (MB = 7) */
#define REG_CAN1_MDL7             (0x400142F4U) /**< \brief (CAN1) Mailbox Data Low Register (MB = 7) */
#define REG_CAN1_MDH7             (0x400142F8U) /**< \brief (CAN1) Mailbox Data High Register (MB = 7) */
#define REG_CAN1_MCR7             (0x400142FCU) /**< \brief (CAN1) Mailbox Control Register (MB = 7) */
#else
#define REG_CAN1_MR      (*(RwReg*)0x40014000U) /**< \brief (CAN1) Mode Register */
#define REG_CAN1_IER     (*(WoReg*)0x40014004U) /**< \brief (CAN1) Interrupt Enable Register */
#define REG_CAN1_IDR     (*(WoReg*)0x40014008U) /**< \brief (CAN1) Interrupt Disable Register */
#define REG_CAN1_IMR     (*(RoReg*)0x4001400CU) /**< \brief (CAN1) Interrupt Mask Register */
#define REG_CAN1_SR      (*(RoReg*)0x40014010U) /**< \brief (CAN1) Status Register */
#define REG_CAN1_BR      (*(RwReg*)0x40014014U) /**< \brief (CAN1) Baudrate Register */
#define REG_CAN1_TIM     (*(RoReg*)0x40014018U) /**< \brief (CAN1) Timer Register */
#define REG_CAN1_TIMESTP (*(RoReg*)0x4001401CU) /**< \brief (CAN1) Timestamp Register */
#define REG_CAN1_ECR     (*(RoReg*)0x40014020U) /**< \brief (CAN1) Error Counter Register */
#define REG_CAN1_TCR     (*(WoReg*)0x40014024U) /**< \brief (CAN1) Transfer Command Register */
#define REG_CAN1_ACR     (*(WoReg*)0x40014028U) /**< \brief (CAN1) Abort Command Register */
#define REG_CAN1_WPMR    (*(RwReg*)0x400140E4U) /**< \brief (CAN1) Write Protect Mode Register */
#define REG_CAN1_WPSR    (*(RoReg*)0x400140E8U) /**< \brief (CAN1) Write Protect Status Register */
#define REG_CAN1_MMR0    (*(RwReg*)0x40014200U) /**< \brief (CAN1) Mailbox Mode Register (MB = 0) */
#define REG_CAN1_MAM0    (*(RwReg*)0x40014204U) /**< \brief (CAN1) Mailbox Acceptance Mask Register (MB = 0) */
#define REG_CAN1_MID0    (*(RwReg*)0x40014208U) /**< \brief (CAN1) Mailbox ID Register (MB = 0) */
#define REG_CAN1_MFID0   (*(RoReg*)0x4001420CU) /**< \brief (CAN1) Mailbox Family ID Register (MB = 0) */
#define REG_CAN1_MSR0    (*(RoReg*)0x40014210U) /**< \brief (CAN1) Mailbox Status Register (MB = 0) */
#define REG_CAN1_MDL0    (*(RwReg*)0x40014214U) /**< \brief (CAN1) Mailbox Data Low Register (MB = 0) */
#define REG_CAN1_MDH0    (*(RwReg*)0x40014218U) /**< \brief (CAN1) Mailbox Data High Register (MB = 0) */
#define REG_CAN1_MCR0    (*(WoReg*)0x4001421CU) /**< \brief (CAN1) Mailbox Control Register (MB = 0) */
#define REG_CAN1_MMR1    (*(RwReg*)0x40014220U) /**< \brief (CAN1) Mailbox Mode Register (MB = 1) */
#define REG_CAN1_MAM1    (*(RwReg*)0x40014224U) /**< \brief (CAN1) Mailbox Acceptance Mask Register (MB = 1) */
#define REG_CAN1_MID1    (*(RwReg*)0x40014228U) /**< \brief (CAN1) Mailbox ID Register (MB = 1) */
#define REG_CAN1_MFID1   (*(RoReg*)0x4001422CU) /**< \brief (CAN1) Mailbox Family ID Register (MB = 1) */
#define REG_CAN1_MSR1    (*(RoReg*)0x40014230U) /**< \brief (CAN1) Mailbox Status Register (MB = 1) */
#define REG_CAN1_MDL1    (*(RwReg*)0x40014234U) /**< \brief (CAN1) Mailbox Data Low Register (MB = 1) */
#define REG_CAN1_MDH1    (*(RwReg*)0x40014238U) /**< \brief (CAN1) Mailbox Data High Register (MB = 1) */
#define REG_CAN1_MCR1    (*(WoReg*)0x4001423CU) /**< \brief (CAN1) Mailbox Control Register (MB = 1) */
#define REG_CAN1_MMR2    (*(RwReg*)0x40014240U) /**< \brief (CAN1) Mailbox Mode Register (MB = 2) */
#define REG_CAN1_MAM2    (*(RwReg*)0x40014244U) /**< \brief (CAN1) Mailbox Acceptance Mask Register (MB = 2) */
#define REG_CAN1_MID2    (*(RwReg*)0x40014248U) /**< \brief (CAN1) Mailbox ID Register (MB = 2) */
#define REG_CAN1_MFID2   (*(RoReg*)0x4001424CU) /**< \brief (CAN1) Mailbox Family ID Register (MB = 2) */
#define REG_CAN1_MSR2    (*(RoReg*)0x40014250U) /**< \brief (CAN1) Mailbox Status Register (MB = 2) */
#define REG_CAN1_MDL2    (*(RwReg*)0x40014254U) /**< \brief (CAN1) Mailbox Data Low Register (MB = 2) */
#define REG_CAN1_MDH2    (*(RwReg*)0x40014258U) /**< \brief (CAN1) Mailbox Data High Register (MB = 2) */
#define REG_CAN1_MCR2    (*(WoReg*)0x4001425CU) /**< \brief (CAN1) Mailbox Control Register (MB = 2) */
#define REG_CAN1_MMR3    (*(RwReg*)0x40014260U) /**< \brief (CAN1) Mailbox Mode Register (MB = 3) */
#define REG_CAN1_MAM3    (*(RwReg*)0x40014264U) /**< \brief (CAN1) Mailbox Acceptance Mask Register (MB = 3) */
#define REG_CAN1_MID3    (*(RwReg*)0x40014268U) /**< \brief (CAN1) Mailbox ID Register (MB = 3) */
#define REG_CAN1_MFID3   (*(RoReg*)0x4001426CU) /**< \brief (CAN1) Mailbox Family ID Register (MB = 3) */
#define REG_CAN1_MSR3    (*(RoReg*)0x40014270U) /**< \brief (CAN1) Mailbox Status Register (MB = 3) */
#define REG_CAN1_MDL3    (*(RwReg*)0x40014274U) /**< \brief (CAN1) Mailbox Data Low Register (MB = 3) */
#define REG_CAN1_MDH3    (*(RwReg*)0x40014278U) /**< \brief (CAN1) Mailbox Data High Register (MB = 3) */
#define REG_CAN1_MCR3    (*(WoReg*)0x4001427CU) /**< \brief (CAN1) Mailbox Control Register (MB = 3) */
#define REG_CAN1_MMR4    (*(RwReg*)0x40014280U) /**< \brief (CAN1) Mailbox Mode Register (MB = 4) */
#define REG_CAN1_MAM4    (*(RwReg*)0x40014284U) /**< \brief (CAN1) Mailbox Acceptance Mask Register (MB = 4) */
#define REG_CAN1_MID4    (*(RwReg*)0x40014288U) /**< \brief (CAN1) Mailbox ID Register (MB = 4) */
#define REG_CAN1_MFID4   (*(RoReg*)0x4001428CU) /**< \brief (CAN1) Mailbox Family ID Register (MB = 4) */
#define REG_CAN1_MSR4    (*(RoReg*)0x40014290U) /**< \brief (CAN1) Mailbox Status Register (MB = 4) */
#define REG_CAN1_MDL4    (*(RwReg*)0x40014294U) /**< \brief (CAN1) Mailbox Data Low Register (MB = 4) */
#define REG_CAN1_MDH4    (*(RwReg*)0x40014298U) /**< \brief (CAN1) Mailbox Data High Register (MB = 4) */
#define REG_CAN1_MCR4    (*(WoReg*)0x4001429CU) /**< \brief (CAN1) Mailbox Control Register (MB = 4) */
#define REG_CAN1_MMR5    (*(RwReg*)0x400142A0U) /**< \brief (CAN1) Mailbox Mode Register (MB = 5) */
#define REG_CAN1_MAM5    (*(RwReg*)0x400142A4U) /**< \brief (CAN1) Mailbox Acceptance Mask Register (MB = 5) */
#define REG_CAN1_MID5    (*(RwReg*)0x400142A8U) /**< \brief (CAN1) Mailbox ID Register (MB = 5) */
#define REG_CAN1_MFID5   (*(RoReg*)0x400142ACU) /**< \brief (CAN1) Mailbox Family ID Register (MB = 5) */
#define REG_CAN1_MSR5    (*(RoReg*)0x400142B0U) /**< \brief (CAN1) Mailbox Status Register (MB = 5) */
#define REG_CAN1_MDL5    (*(RwReg*)0x400142B4U) /**< \brief (CAN1) Mailbox Data Low Register (MB = 5) */
#define REG_CAN1_MDH5    (*(RwReg*)0x400142B8U) /**< \brief (CAN1) Mailbox Data High Register (MB = 5) */
#define REG_CAN1_MCR5    (*(WoReg*)0x400142BCU) /**< \brief (CAN1) Mailbox Control Register (MB = 5) */
#define REG_CAN1_MMR6    (*(RwReg*)0x400142C0U) /**< \brief (CAN1) Mailbox Mode Register (MB = 6) */
#define REG_CAN1_MAM6    (*(RwReg*)0x400142C4U) /**< \brief (CAN1) Mailbox Acceptance Mask Register (MB = 6) */
#define REG_CAN1_MID6    (*(RwReg*)0x400142C8U) /**< \brief (CAN1) Mailbox ID Register (MB = 6) */
#define REG_CAN1_MFID6   (*(RoReg*)0x400142CCU) /**< \brief (CAN1) Mailbox Family ID Register (MB = 6) */
#define REG_CAN1_MSR6    (*(RoReg*)0x400142D0U) /**< \brief (CAN1) Mailbox Status Register (MB = 6) */
#define REG_CAN1_MDL6    (*(RwReg*)0x400142D4U) /**< \brief (CAN1) Mailbox Data Low Register (MB = 6) */
#define REG_CAN1_MDH6    (*(RwReg*)0x400142D8U) /**< \brief (CAN1) Mailbox Data High Register (MB = 6) */
#define REG_CAN1_MCR6    (*(WoReg*)0x400142DCU) /**< \brief (CAN1) Mailbox Control Register (MB = 6) */
#define REG_CAN1_MMR7    (*(RwReg*)0x400142E0U) /**< \brief (CAN1) Mailbox Mode Register (MB = 7) */
#define REG_CAN1_MAM7    (*(RwReg*)0x400142E4U) /**< \brief (CAN1) Mailbox Acceptance Mask Register (MB = 7) */
#define REG_CAN1_MID7    (*(RwReg*)0x400142E8U) /**< \brief (CAN1) Mailbox ID Register (MB = 7) */
#define REG_CAN1_MFID7   (*(RoReg*)0x400142ECU) /**< \brief (CAN1) Mailbox Family ID Register (MB = 7) */
#define REG_CAN1_MSR7    (*(RoReg*)0x400142F0U) /**< \brief (CAN1) Mailbox Status Register (MB = 7) */
#define REG_CAN1_MDL7    (*(RwReg*)0x400142F4U) /**< \brief (CAN1) Mailbox Data Low Register (MB = 7) */
#define REG_CAN1_MDH7    (*(RwReg*)0x400142F8U) /**< \brief (CAN1) Mailbox Data High Register (MB = 7) */
#define REG_CAN1_MCR7    (*(WoReg*)0x400142FCU) /**< \brief (CAN1) Mailbox Control Register (MB = 7) */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAM4E_CAN1_INSTANCE_ */
