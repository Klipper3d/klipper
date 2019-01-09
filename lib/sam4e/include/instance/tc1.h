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

#ifndef _SAM4E_TC1_INSTANCE_
#define _SAM4E_TC1_INSTANCE_

/* ========== Register definition for TC1 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_TC1_CCR0           (0x40094000U) /**< \brief (TC1) Channel Control Register (channel = 0) */
#define REG_TC1_CMR0           (0x40094004U) /**< \brief (TC1) Channel Mode Register (channel = 0) */
#define REG_TC1_SMMR0          (0x40094008U) /**< \brief (TC1) Stepper Motor Mode Register (channel = 0) */
#define REG_TC1_RAB0           (0x4009400CU) /**< \brief (TC1) Register AB (channel = 0) */
#define REG_TC1_CV0            (0x40094010U) /**< \brief (TC1) Counter Value (channel = 0) */
#define REG_TC1_RA0            (0x40094014U) /**< \brief (TC1) Register A (channel = 0) */
#define REG_TC1_RB0            (0x40094018U) /**< \brief (TC1) Register B (channel = 0) */
#define REG_TC1_RC0            (0x4009401CU) /**< \brief (TC1) Register C (channel = 0) */
#define REG_TC1_SR0            (0x40094020U) /**< \brief (TC1) Status Register (channel = 0) */
#define REG_TC1_IER0           (0x40094024U) /**< \brief (TC1) Interrupt Enable Register (channel = 0) */
#define REG_TC1_IDR0           (0x40094028U) /**< \brief (TC1) Interrupt Disable Register (channel = 0) */
#define REG_TC1_IMR0           (0x4009402CU) /**< \brief (TC1) Interrupt Mask Register (channel = 0) */
#define REG_TC1_EMR0           (0x40094030U) /**< \brief (TC1) Extended Mode Register (channel = 0) */
#define REG_TC1_CCR1           (0x40094040U) /**< \brief (TC1) Channel Control Register (channel = 1) */
#define REG_TC1_CMR1           (0x40094044U) /**< \brief (TC1) Channel Mode Register (channel = 1) */
#define REG_TC1_SMMR1          (0x40094048U) /**< \brief (TC1) Stepper Motor Mode Register (channel = 1) */
#define REG_TC1_RAB1           (0x4009404CU) /**< \brief (TC1) Register AB (channel = 1) */
#define REG_TC1_CV1            (0x40094050U) /**< \brief (TC1) Counter Value (channel = 1) */
#define REG_TC1_RA1            (0x40094054U) /**< \brief (TC1) Register A (channel = 1) */
#define REG_TC1_RB1            (0x40094058U) /**< \brief (TC1) Register B (channel = 1) */
#define REG_TC1_RC1            (0x4009405CU) /**< \brief (TC1) Register C (channel = 1) */
#define REG_TC1_SR1            (0x40094060U) /**< \brief (TC1) Status Register (channel = 1) */
#define REG_TC1_IER1           (0x40094064U) /**< \brief (TC1) Interrupt Enable Register (channel = 1) */
#define REG_TC1_IDR1           (0x40094068U) /**< \brief (TC1) Interrupt Disable Register (channel = 1) */
#define REG_TC1_IMR1           (0x4009406CU) /**< \brief (TC1) Interrupt Mask Register (channel = 1) */
#define REG_TC1_EMR1           (0x40094070U) /**< \brief (TC1) Extended Mode Register (channel = 1) */
#define REG_TC1_CCR2           (0x40094080U) /**< \brief (TC1) Channel Control Register (channel = 2) */
#define REG_TC1_CMR2           (0x40094084U) /**< \brief (TC1) Channel Mode Register (channel = 2) */
#define REG_TC1_SMMR2          (0x40094088U) /**< \brief (TC1) Stepper Motor Mode Register (channel = 2) */
#define REG_TC1_RAB2           (0x4009408CU) /**< \brief (TC1) Register AB (channel = 2) */
#define REG_TC1_CV2            (0x40094090U) /**< \brief (TC1) Counter Value (channel = 2) */
#define REG_TC1_RA2            (0x40094094U) /**< \brief (TC1) Register A (channel = 2) */
#define REG_TC1_RB2            (0x40094098U) /**< \brief (TC1) Register B (channel = 2) */
#define REG_TC1_RC2            (0x4009409CU) /**< \brief (TC1) Register C (channel = 2) */
#define REG_TC1_SR2            (0x400940A0U) /**< \brief (TC1) Status Register (channel = 2) */
#define REG_TC1_IER2           (0x400940A4U) /**< \brief (TC1) Interrupt Enable Register (channel = 2) */
#define REG_TC1_IDR2           (0x400940A8U) /**< \brief (TC1) Interrupt Disable Register (channel = 2) */
#define REG_TC1_IMR2           (0x400940ACU) /**< \brief (TC1) Interrupt Mask Register (channel = 2) */
#define REG_TC1_EMR2           (0x400940B0U) /**< \brief (TC1) Extended Mode Register (channel = 2) */
#define REG_TC1_BCR            (0x400940C0U) /**< \brief (TC1) Block Control Register */
#define REG_TC1_BMR            (0x400940C4U) /**< \brief (TC1) Block Mode Register */
#define REG_TC1_QIER           (0x400940C8U) /**< \brief (TC1) QDEC Interrupt Enable Register */
#define REG_TC1_QIDR           (0x400940CCU) /**< \brief (TC1) QDEC Interrupt Disable Register */
#define REG_TC1_QIMR           (0x400940D0U) /**< \brief (TC1) QDEC Interrupt Mask Register */
#define REG_TC1_QISR           (0x400940D4U) /**< \brief (TC1) QDEC Interrupt Status Register */
#define REG_TC1_FMR            (0x400940D8U) /**< \brief (TC1) Fault Mode Register */
#define REG_TC1_WPMR           (0x400940E4U) /**< \brief (TC1) Write Protect Mode Register */
#define REG_TC1_RPR0           (0x40094100U) /**< \brief (TC1) Receive Pointer Register (pdc = 0) */
#define REG_TC1_RCR0           (0x40094104U) /**< \brief (TC1) Receive Counter Register (pdc = 0) */
#define REG_TC1_RNPR0          (0x40094110U) /**< \brief (TC1) Receive Next Pointer Register (pdc = 0) */
#define REG_TC1_RNCR0          (0x40094114U) /**< \brief (TC1) Receive Next Counter Register (pdc = 0) */
#define REG_TC1_PTCR0          (0x40094120U) /**< \brief (TC1) Transfer Control Register (pdc = 0) */
#define REG_TC1_PTSR0          (0x40094124U) /**< \brief (TC1) Transfer Status Register (pdc = 0) */
#define REG_TC1_RPR1           (0x40094140U) /**< \brief (TC1) Receive Pointer Register (pdc = 1) */
#define REG_TC1_RCR1           (0x40094144U) /**< \brief (TC1) Receive Counter Register (pdc = 1) */
#define REG_TC1_RNPR1          (0x40094150U) /**< \brief (TC1) Receive Next Pointer Register (pdc = 1) */
#define REG_TC1_RNCR1          (0x40094154U) /**< \brief (TC1) Receive Next Counter Register (pdc = 1) */
#define REG_TC1_PTCR1          (0x40094160U) /**< \brief (TC1) Transfer Control Register (pdc = 1) */
#define REG_TC1_PTSR1          (0x40094164U) /**< \brief (TC1) Transfer Status Register (pdc = 1) */
#define REG_TC1_RPR2           (0x40094180U) /**< \brief (TC1) Receive Pointer Register (pdc = 2) */
#define REG_TC1_RCR2           (0x40094184U) /**< \brief (TC1) Receive Counter Register (pdc = 2) */
#define REG_TC1_RNPR2          (0x40094190U) /**< \brief (TC1) Receive Next Pointer Register (pdc = 2) */
#define REG_TC1_RNCR2          (0x40094194U) /**< \brief (TC1) Receive Next Counter Register (pdc = 2) */
#define REG_TC1_PTCR2          (0x400941A0U) /**< \brief (TC1) Transfer Control Register (pdc = 2) */
#define REG_TC1_PTSR2          (0x400941A4U) /**< \brief (TC1) Transfer Status Register (pdc = 2) */
#else
#define REG_TC1_CCR0  (*(WoReg*)0x40094000U) /**< \brief (TC1) Channel Control Register (channel = 0) */
#define REG_TC1_CMR0  (*(RwReg*)0x40094004U) /**< \brief (TC1) Channel Mode Register (channel = 0) */
#define REG_TC1_SMMR0 (*(RwReg*)0x40094008U) /**< \brief (TC1) Stepper Motor Mode Register (channel = 0) */
#define REG_TC1_RAB0  (*(RoReg*)0x4009400CU) /**< \brief (TC1) Register AB (channel = 0) */
#define REG_TC1_CV0   (*(RoReg*)0x40094010U) /**< \brief (TC1) Counter Value (channel = 0) */
#define REG_TC1_RA0   (*(RwReg*)0x40094014U) /**< \brief (TC1) Register A (channel = 0) */
#define REG_TC1_RB0   (*(RwReg*)0x40094018U) /**< \brief (TC1) Register B (channel = 0) */
#define REG_TC1_RC0   (*(RwReg*)0x4009401CU) /**< \brief (TC1) Register C (channel = 0) */
#define REG_TC1_SR0   (*(RoReg*)0x40094020U) /**< \brief (TC1) Status Register (channel = 0) */
#define REG_TC1_IER0  (*(WoReg*)0x40094024U) /**< \brief (TC1) Interrupt Enable Register (channel = 0) */
#define REG_TC1_IDR0  (*(WoReg*)0x40094028U) /**< \brief (TC1) Interrupt Disable Register (channel = 0) */
#define REG_TC1_IMR0  (*(RoReg*)0x4009402CU) /**< \brief (TC1) Interrupt Mask Register (channel = 0) */
#define REG_TC1_EMR0  (*(RwReg*)0x40094030U) /**< \brief (TC1) Extended Mode Register (channel = 0) */
#define REG_TC1_CCR1  (*(WoReg*)0x40094040U) /**< \brief (TC1) Channel Control Register (channel = 1) */
#define REG_TC1_CMR1  (*(RwReg*)0x40094044U) /**< \brief (TC1) Channel Mode Register (channel = 1) */
#define REG_TC1_SMMR1 (*(RwReg*)0x40094048U) /**< \brief (TC1) Stepper Motor Mode Register (channel = 1) */
#define REG_TC1_RAB1  (*(RoReg*)0x4009404CU) /**< \brief (TC1) Register AB (channel = 1) */
#define REG_TC1_CV1   (*(RoReg*)0x40094050U) /**< \brief (TC1) Counter Value (channel = 1) */
#define REG_TC1_RA1   (*(RwReg*)0x40094054U) /**< \brief (TC1) Register A (channel = 1) */
#define REG_TC1_RB1   (*(RwReg*)0x40094058U) /**< \brief (TC1) Register B (channel = 1) */
#define REG_TC1_RC1   (*(RwReg*)0x4009405CU) /**< \brief (TC1) Register C (channel = 1) */
#define REG_TC1_SR1   (*(RoReg*)0x40094060U) /**< \brief (TC1) Status Register (channel = 1) */
#define REG_TC1_IER1  (*(WoReg*)0x40094064U) /**< \brief (TC1) Interrupt Enable Register (channel = 1) */
#define REG_TC1_IDR1  (*(WoReg*)0x40094068U) /**< \brief (TC1) Interrupt Disable Register (channel = 1) */
#define REG_TC1_IMR1  (*(RoReg*)0x4009406CU) /**< \brief (TC1) Interrupt Mask Register (channel = 1) */
#define REG_TC1_EMR1  (*(RwReg*)0x40094070U) /**< \brief (TC1) Extended Mode Register (channel = 1) */
#define REG_TC1_CCR2  (*(WoReg*)0x40094080U) /**< \brief (TC1) Channel Control Register (channel = 2) */
#define REG_TC1_CMR2  (*(RwReg*)0x40094084U) /**< \brief (TC1) Channel Mode Register (channel = 2) */
#define REG_TC1_SMMR2 (*(RwReg*)0x40094088U) /**< \brief (TC1) Stepper Motor Mode Register (channel = 2) */
#define REG_TC1_RAB2  (*(RoReg*)0x4009408CU) /**< \brief (TC1) Register AB (channel = 2) */
#define REG_TC1_CV2   (*(RoReg*)0x40094090U) /**< \brief (TC1) Counter Value (channel = 2) */
#define REG_TC1_RA2   (*(RwReg*)0x40094094U) /**< \brief (TC1) Register A (channel = 2) */
#define REG_TC1_RB2   (*(RwReg*)0x40094098U) /**< \brief (TC1) Register B (channel = 2) */
#define REG_TC1_RC2   (*(RwReg*)0x4009409CU) /**< \brief (TC1) Register C (channel = 2) */
#define REG_TC1_SR2   (*(RoReg*)0x400940A0U) /**< \brief (TC1) Status Register (channel = 2) */
#define REG_TC1_IER2  (*(WoReg*)0x400940A4U) /**< \brief (TC1) Interrupt Enable Register (channel = 2) */
#define REG_TC1_IDR2  (*(WoReg*)0x400940A8U) /**< \brief (TC1) Interrupt Disable Register (channel = 2) */
#define REG_TC1_IMR2  (*(RoReg*)0x400940ACU) /**< \brief (TC1) Interrupt Mask Register (channel = 2) */
#define REG_TC1_EMR2  (*(RwReg*)0x400940B0U) /**< \brief (TC1) Extended Mode Register (channel = 2) */
#define REG_TC1_BCR   (*(WoReg*)0x400940C0U) /**< \brief (TC1) Block Control Register */
#define REG_TC1_BMR   (*(RwReg*)0x400940C4U) /**< \brief (TC1) Block Mode Register */
#define REG_TC1_QIER  (*(WoReg*)0x400940C8U) /**< \brief (TC1) QDEC Interrupt Enable Register */
#define REG_TC1_QIDR  (*(WoReg*)0x400940CCU) /**< \brief (TC1) QDEC Interrupt Disable Register */
#define REG_TC1_QIMR  (*(RoReg*)0x400940D0U) /**< \brief (TC1) QDEC Interrupt Mask Register */
#define REG_TC1_QISR  (*(RoReg*)0x400940D4U) /**< \brief (TC1) QDEC Interrupt Status Register */
#define REG_TC1_FMR   (*(RwReg*)0x400940D8U) /**< \brief (TC1) Fault Mode Register */
#define REG_TC1_WPMR  (*(RwReg*)0x400940E4U) /**< \brief (TC1) Write Protect Mode Register */
#define REG_TC1_RPR0  (*(RwReg*)0x40094100U) /**< \brief (TC1) Receive Pointer Register (pdc = 0) */
#define REG_TC1_RCR0  (*(RwReg*)0x40094104U) /**< \brief (TC1) Receive Counter Register (pdc = 0) */
#define REG_TC1_RNPR0 (*(RwReg*)0x40094110U) /**< \brief (TC1) Receive Next Pointer Register (pdc = 0) */
#define REG_TC1_RNCR0 (*(RwReg*)0x40094114U) /**< \brief (TC1) Receive Next Counter Register (pdc = 0) */
#define REG_TC1_PTCR0 (*(WoReg*)0x40094120U) /**< \brief (TC1) Transfer Control Register (pdc = 0) */
#define REG_TC1_PTSR0 (*(RoReg*)0x40094124U) /**< \brief (TC1) Transfer Status Register (pdc = 0) */
#define REG_TC1_RPR1  (*(RwReg*)0x40094140U) /**< \brief (TC1) Receive Pointer Register (pdc = 1) */
#define REG_TC1_RCR1  (*(RwReg*)0x40094144U) /**< \brief (TC1) Receive Counter Register (pdc = 1) */
#define REG_TC1_RNPR1 (*(RwReg*)0x40094150U) /**< \brief (TC1) Receive Next Pointer Register (pdc = 1) */
#define REG_TC1_RNCR1 (*(RwReg*)0x40094154U) /**< \brief (TC1) Receive Next Counter Register (pdc = 1) */
#define REG_TC1_PTCR1 (*(WoReg*)0x40094160U) /**< \brief (TC1) Transfer Control Register (pdc = 1) */
#define REG_TC1_PTSR1 (*(RoReg*)0x40094164U) /**< \brief (TC1) Transfer Status Register (pdc = 1) */
#define REG_TC1_RPR2  (*(RwReg*)0x40094180U) /**< \brief (TC1) Receive Pointer Register (pdc = 2) */
#define REG_TC1_RCR2  (*(RwReg*)0x40094184U) /**< \brief (TC1) Receive Counter Register (pdc = 2) */
#define REG_TC1_RNPR2 (*(RwReg*)0x40094190U) /**< \brief (TC1) Receive Next Pointer Register (pdc = 2) */
#define REG_TC1_RNCR2 (*(RwReg*)0x40094194U) /**< \brief (TC1) Receive Next Counter Register (pdc = 2) */
#define REG_TC1_PTCR2 (*(WoReg*)0x400941A0U) /**< \brief (TC1) Transfer Control Register (pdc = 2) */
#define REG_TC1_PTSR2 (*(RoReg*)0x400941A4U) /**< \brief (TC1) Transfer Status Register (pdc = 2) */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAM4E_TC1_INSTANCE_ */
