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

#ifndef _SAM4E_TC2_INSTANCE_
#define _SAM4E_TC2_INSTANCE_

/* ========== Register definition for TC2 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_TC2_CCR0           (0x40098000U) /**< \brief (TC2) Channel Control Register (channel = 0) */
#define REG_TC2_CMR0           (0x40098004U) /**< \brief (TC2) Channel Mode Register (channel = 0) */
#define REG_TC2_SMMR0          (0x40098008U) /**< \brief (TC2) Stepper Motor Mode Register (channel = 0) */
#define REG_TC2_RAB0           (0x4009800CU) /**< \brief (TC2) Register AB (channel = 0) */
#define REG_TC2_CV0            (0x40098010U) /**< \brief (TC2) Counter Value (channel = 0) */
#define REG_TC2_RA0            (0x40098014U) /**< \brief (TC2) Register A (channel = 0) */
#define REG_TC2_RB0            (0x40098018U) /**< \brief (TC2) Register B (channel = 0) */
#define REG_TC2_RC0            (0x4009801CU) /**< \brief (TC2) Register C (channel = 0) */
#define REG_TC2_SR0            (0x40098020U) /**< \brief (TC2) Status Register (channel = 0) */
#define REG_TC2_IER0           (0x40098024U) /**< \brief (TC2) Interrupt Enable Register (channel = 0) */
#define REG_TC2_IDR0           (0x40098028U) /**< \brief (TC2) Interrupt Disable Register (channel = 0) */
#define REG_TC2_IMR0           (0x4009802CU) /**< \brief (TC2) Interrupt Mask Register (channel = 0) */
#define REG_TC2_EMR0           (0x40098030U) /**< \brief (TC2) Extended Mode Register (channel = 0) */
#define REG_TC2_CCR1           (0x40098040U) /**< \brief (TC2) Channel Control Register (channel = 1) */
#define REG_TC2_CMR1           (0x40098044U) /**< \brief (TC2) Channel Mode Register (channel = 1) */
#define REG_TC2_SMMR1          (0x40098048U) /**< \brief (TC2) Stepper Motor Mode Register (channel = 1) */
#define REG_TC2_RAB1           (0x4009804CU) /**< \brief (TC2) Register AB (channel = 1) */
#define REG_TC2_CV1            (0x40098050U) /**< \brief (TC2) Counter Value (channel = 1) */
#define REG_TC2_RA1            (0x40098054U) /**< \brief (TC2) Register A (channel = 1) */
#define REG_TC2_RB1            (0x40098058U) /**< \brief (TC2) Register B (channel = 1) */
#define REG_TC2_RC1            (0x4009805CU) /**< \brief (TC2) Register C (channel = 1) */
#define REG_TC2_SR1            (0x40098060U) /**< \brief (TC2) Status Register (channel = 1) */
#define REG_TC2_IER1           (0x40098064U) /**< \brief (TC2) Interrupt Enable Register (channel = 1) */
#define REG_TC2_IDR1           (0x40098068U) /**< \brief (TC2) Interrupt Disable Register (channel = 1) */
#define REG_TC2_IMR1           (0x4009806CU) /**< \brief (TC2) Interrupt Mask Register (channel = 1) */
#define REG_TC2_EMR1           (0x40098070U) /**< \brief (TC2) Extended Mode Register (channel = 1) */
#define REG_TC2_CCR2           (0x40098080U) /**< \brief (TC2) Channel Control Register (channel = 2) */
#define REG_TC2_CMR2           (0x40098084U) /**< \brief (TC2) Channel Mode Register (channel = 2) */
#define REG_TC2_SMMR2          (0x40098088U) /**< \brief (TC2) Stepper Motor Mode Register (channel = 2) */
#define REG_TC2_RAB2           (0x4009808CU) /**< \brief (TC2) Register AB (channel = 2) */
#define REG_TC2_CV2            (0x40098090U) /**< \brief (TC2) Counter Value (channel = 2) */
#define REG_TC2_RA2            (0x40098094U) /**< \brief (TC2) Register A (channel = 2) */
#define REG_TC2_RB2            (0x40098098U) /**< \brief (TC2) Register B (channel = 2) */
#define REG_TC2_RC2            (0x4009809CU) /**< \brief (TC2) Register C (channel = 2) */
#define REG_TC2_SR2            (0x400980A0U) /**< \brief (TC2) Status Register (channel = 2) */
#define REG_TC2_IER2           (0x400980A4U) /**< \brief (TC2) Interrupt Enable Register (channel = 2) */
#define REG_TC2_IDR2           (0x400980A8U) /**< \brief (TC2) Interrupt Disable Register (channel = 2) */
#define REG_TC2_IMR2           (0x400980ACU) /**< \brief (TC2) Interrupt Mask Register (channel = 2) */
#define REG_TC2_EMR2           (0x400980B0U) /**< \brief (TC2) Extended Mode Register (channel = 2) */
#define REG_TC2_BCR            (0x400980C0U) /**< \brief (TC2) Block Control Register */
#define REG_TC2_BMR            (0x400980C4U) /**< \brief (TC2) Block Mode Register */
#define REG_TC2_QIER           (0x400980C8U) /**< \brief (TC2) QDEC Interrupt Enable Register */
#define REG_TC2_QIDR           (0x400980CCU) /**< \brief (TC2) QDEC Interrupt Disable Register */
#define REG_TC2_QIMR           (0x400980D0U) /**< \brief (TC2) QDEC Interrupt Mask Register */
#define REG_TC2_QISR           (0x400980D4U) /**< \brief (TC2) QDEC Interrupt Status Register */
#define REG_TC2_FMR            (0x400980D8U) /**< \brief (TC2) Fault Mode Register */
#define REG_TC2_WPMR           (0x400980E4U) /**< \brief (TC2) Write Protect Mode Register */
#else
#define REG_TC2_CCR0  (*(WoReg*)0x40098000U) /**< \brief (TC2) Channel Control Register (channel = 0) */
#define REG_TC2_CMR0  (*(RwReg*)0x40098004U) /**< \brief (TC2) Channel Mode Register (channel = 0) */
#define REG_TC2_SMMR0 (*(RwReg*)0x40098008U) /**< \brief (TC2) Stepper Motor Mode Register (channel = 0) */
#define REG_TC2_RAB0  (*(RoReg*)0x4009800CU) /**< \brief (TC2) Register AB (channel = 0) */
#define REG_TC2_CV0   (*(RoReg*)0x40098010U) /**< \brief (TC2) Counter Value (channel = 0) */
#define REG_TC2_RA0   (*(RwReg*)0x40098014U) /**< \brief (TC2) Register A (channel = 0) */
#define REG_TC2_RB0   (*(RwReg*)0x40098018U) /**< \brief (TC2) Register B (channel = 0) */
#define REG_TC2_RC0   (*(RwReg*)0x4009801CU) /**< \brief (TC2) Register C (channel = 0) */
#define REG_TC2_SR0   (*(RoReg*)0x40098020U) /**< \brief (TC2) Status Register (channel = 0) */
#define REG_TC2_IER0  (*(WoReg*)0x40098024U) /**< \brief (TC2) Interrupt Enable Register (channel = 0) */
#define REG_TC2_IDR0  (*(WoReg*)0x40098028U) /**< \brief (TC2) Interrupt Disable Register (channel = 0) */
#define REG_TC2_IMR0  (*(RoReg*)0x4009802CU) /**< \brief (TC2) Interrupt Mask Register (channel = 0) */
#define REG_TC2_EMR0  (*(RwReg*)0x40098030U) /**< \brief (TC2) Extended Mode Register (channel = 0) */
#define REG_TC2_CCR1  (*(WoReg*)0x40098040U) /**< \brief (TC2) Channel Control Register (channel = 1) */
#define REG_TC2_CMR1  (*(RwReg*)0x40098044U) /**< \brief (TC2) Channel Mode Register (channel = 1) */
#define REG_TC2_SMMR1 (*(RwReg*)0x40098048U) /**< \brief (TC2) Stepper Motor Mode Register (channel = 1) */
#define REG_TC2_RAB1  (*(RoReg*)0x4009804CU) /**< \brief (TC2) Register AB (channel = 1) */
#define REG_TC2_CV1   (*(RoReg*)0x40098050U) /**< \brief (TC2) Counter Value (channel = 1) */
#define REG_TC2_RA1   (*(RwReg*)0x40098054U) /**< \brief (TC2) Register A (channel = 1) */
#define REG_TC2_RB1   (*(RwReg*)0x40098058U) /**< \brief (TC2) Register B (channel = 1) */
#define REG_TC2_RC1   (*(RwReg*)0x4009805CU) /**< \brief (TC2) Register C (channel = 1) */
#define REG_TC2_SR1   (*(RoReg*)0x40098060U) /**< \brief (TC2) Status Register (channel = 1) */
#define REG_TC2_IER1  (*(WoReg*)0x40098064U) /**< \brief (TC2) Interrupt Enable Register (channel = 1) */
#define REG_TC2_IDR1  (*(WoReg*)0x40098068U) /**< \brief (TC2) Interrupt Disable Register (channel = 1) */
#define REG_TC2_IMR1  (*(RoReg*)0x4009806CU) /**< \brief (TC2) Interrupt Mask Register (channel = 1) */
#define REG_TC2_EMR1  (*(RwReg*)0x40098070U) /**< \brief (TC2) Extended Mode Register (channel = 1) */
#define REG_TC2_CCR2  (*(WoReg*)0x40098080U) /**< \brief (TC2) Channel Control Register (channel = 2) */
#define REG_TC2_CMR2  (*(RwReg*)0x40098084U) /**< \brief (TC2) Channel Mode Register (channel = 2) */
#define REG_TC2_SMMR2 (*(RwReg*)0x40098088U) /**< \brief (TC2) Stepper Motor Mode Register (channel = 2) */
#define REG_TC2_RAB2  (*(RoReg*)0x4009808CU) /**< \brief (TC2) Register AB (channel = 2) */
#define REG_TC2_CV2   (*(RoReg*)0x40098090U) /**< \brief (TC2) Counter Value (channel = 2) */
#define REG_TC2_RA2   (*(RwReg*)0x40098094U) /**< \brief (TC2) Register A (channel = 2) */
#define REG_TC2_RB2   (*(RwReg*)0x40098098U) /**< \brief (TC2) Register B (channel = 2) */
#define REG_TC2_RC2   (*(RwReg*)0x4009809CU) /**< \brief (TC2) Register C (channel = 2) */
#define REG_TC2_SR2   (*(RoReg*)0x400980A0U) /**< \brief (TC2) Status Register (channel = 2) */
#define REG_TC2_IER2  (*(WoReg*)0x400980A4U) /**< \brief (TC2) Interrupt Enable Register (channel = 2) */
#define REG_TC2_IDR2  (*(WoReg*)0x400980A8U) /**< \brief (TC2) Interrupt Disable Register (channel = 2) */
#define REG_TC2_IMR2  (*(RoReg*)0x400980ACU) /**< \brief (TC2) Interrupt Mask Register (channel = 2) */
#define REG_TC2_EMR2  (*(RwReg*)0x400980B0U) /**< \brief (TC2) Extended Mode Register (channel = 2) */
#define REG_TC2_BCR   (*(WoReg*)0x400980C0U) /**< \brief (TC2) Block Control Register */
#define REG_TC2_BMR   (*(RwReg*)0x400980C4U) /**< \brief (TC2) Block Mode Register */
#define REG_TC2_QIER  (*(WoReg*)0x400980C8U) /**< \brief (TC2) QDEC Interrupt Enable Register */
#define REG_TC2_QIDR  (*(WoReg*)0x400980CCU) /**< \brief (TC2) QDEC Interrupt Disable Register */
#define REG_TC2_QIMR  (*(RoReg*)0x400980D0U) /**< \brief (TC2) QDEC Interrupt Mask Register */
#define REG_TC2_QISR  (*(RoReg*)0x400980D4U) /**< \brief (TC2) QDEC Interrupt Status Register */
#define REG_TC2_FMR   (*(RwReg*)0x400980D8U) /**< \brief (TC2) Fault Mode Register */
#define REG_TC2_WPMR  (*(RwReg*)0x400980E4U) /**< \brief (TC2) Write Protect Mode Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAM4E_TC2_INSTANCE_ */
