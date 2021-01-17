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

#ifndef _SAM4E_TC0_INSTANCE_
#define _SAM4E_TC0_INSTANCE_

/* ========== Register definition for TC0 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_TC0_CCR0           (0x40090000U) /**< \brief (TC0) Channel Control Register (channel = 0) */
#define REG_TC0_CMR0           (0x40090004U) /**< \brief (TC0) Channel Mode Register (channel = 0) */
#define REG_TC0_SMMR0          (0x40090008U) /**< \brief (TC0) Stepper Motor Mode Register (channel = 0) */
#define REG_TC0_RAB0           (0x4009000CU) /**< \brief (TC0) Register AB (channel = 0) */
#define REG_TC0_CV0            (0x40090010U) /**< \brief (TC0) Counter Value (channel = 0) */
#define REG_TC0_RA0            (0x40090014U) /**< \brief (TC0) Register A (channel = 0) */
#define REG_TC0_RB0            (0x40090018U) /**< \brief (TC0) Register B (channel = 0) */
#define REG_TC0_RC0            (0x4009001CU) /**< \brief (TC0) Register C (channel = 0) */
#define REG_TC0_SR0            (0x40090020U) /**< \brief (TC0) Status Register (channel = 0) */
#define REG_TC0_IER0           (0x40090024U) /**< \brief (TC0) Interrupt Enable Register (channel = 0) */
#define REG_TC0_IDR0           (0x40090028U) /**< \brief (TC0) Interrupt Disable Register (channel = 0) */
#define REG_TC0_IMR0           (0x4009002CU) /**< \brief (TC0) Interrupt Mask Register (channel = 0) */
#define REG_TC0_EMR0           (0x40090030U) /**< \brief (TC0) Extended Mode Register (channel = 0) */
#define REG_TC0_CCR1           (0x40090040U) /**< \brief (TC0) Channel Control Register (channel = 1) */
#define REG_TC0_CMR1           (0x40090044U) /**< \brief (TC0) Channel Mode Register (channel = 1) */
#define REG_TC0_SMMR1          (0x40090048U) /**< \brief (TC0) Stepper Motor Mode Register (channel = 1) */
#define REG_TC0_RAB1           (0x4009004CU) /**< \brief (TC0) Register AB (channel = 1) */
#define REG_TC0_CV1            (0x40090050U) /**< \brief (TC0) Counter Value (channel = 1) */
#define REG_TC0_RA1            (0x40090054U) /**< \brief (TC0) Register A (channel = 1) */
#define REG_TC0_RB1            (0x40090058U) /**< \brief (TC0) Register B (channel = 1) */
#define REG_TC0_RC1            (0x4009005CU) /**< \brief (TC0) Register C (channel = 1) */
#define REG_TC0_SR1            (0x40090060U) /**< \brief (TC0) Status Register (channel = 1) */
#define REG_TC0_IER1           (0x40090064U) /**< \brief (TC0) Interrupt Enable Register (channel = 1) */
#define REG_TC0_IDR1           (0x40090068U) /**< \brief (TC0) Interrupt Disable Register (channel = 1) */
#define REG_TC0_IMR1           (0x4009006CU) /**< \brief (TC0) Interrupt Mask Register (channel = 1) */
#define REG_TC0_EMR1           (0x40090070U) /**< \brief (TC0) Extended Mode Register (channel = 1) */
#define REG_TC0_CCR2           (0x40090080U) /**< \brief (TC0) Channel Control Register (channel = 2) */
#define REG_TC0_CMR2           (0x40090084U) /**< \brief (TC0) Channel Mode Register (channel = 2) */
#define REG_TC0_SMMR2          (0x40090088U) /**< \brief (TC0) Stepper Motor Mode Register (channel = 2) */
#define REG_TC0_RAB2           (0x4009008CU) /**< \brief (TC0) Register AB (channel = 2) */
#define REG_TC0_CV2            (0x40090090U) /**< \brief (TC0) Counter Value (channel = 2) */
#define REG_TC0_RA2            (0x40090094U) /**< \brief (TC0) Register A (channel = 2) */
#define REG_TC0_RB2            (0x40090098U) /**< \brief (TC0) Register B (channel = 2) */
#define REG_TC0_RC2            (0x4009009CU) /**< \brief (TC0) Register C (channel = 2) */
#define REG_TC0_SR2            (0x400900A0U) /**< \brief (TC0) Status Register (channel = 2) */
#define REG_TC0_IER2           (0x400900A4U) /**< \brief (TC0) Interrupt Enable Register (channel = 2) */
#define REG_TC0_IDR2           (0x400900A8U) /**< \brief (TC0) Interrupt Disable Register (channel = 2) */
#define REG_TC0_IMR2           (0x400900ACU) /**< \brief (TC0) Interrupt Mask Register (channel = 2) */
#define REG_TC0_EMR2           (0x400900B0U) /**< \brief (TC0) Extended Mode Register (channel = 2) */
#define REG_TC0_BCR            (0x400900C0U) /**< \brief (TC0) Block Control Register */
#define REG_TC0_BMR            (0x400900C4U) /**< \brief (TC0) Block Mode Register */
#define REG_TC0_QIER           (0x400900C8U) /**< \brief (TC0) QDEC Interrupt Enable Register */
#define REG_TC0_QIDR           (0x400900CCU) /**< \brief (TC0) QDEC Interrupt Disable Register */
#define REG_TC0_QIMR           (0x400900D0U) /**< \brief (TC0) QDEC Interrupt Mask Register */
#define REG_TC0_QISR           (0x400900D4U) /**< \brief (TC0) QDEC Interrupt Status Register */
#define REG_TC0_FMR            (0x400900D8U) /**< \brief (TC0) Fault Mode Register */
#define REG_TC0_WPMR           (0x400900E4U) /**< \brief (TC0) Write Protect Mode Register */
#define REG_TC0_RPR0           (0x40090100U) /**< \brief (TC0) Receive Pointer Register (pdc = 0) */
#define REG_TC0_RCR0           (0x40090104U) /**< \brief (TC0) Receive Counter Register (pdc = 0) */
#define REG_TC0_RNPR0          (0x40090110U) /**< \brief (TC0) Receive Next Pointer Register (pdc = 0) */
#define REG_TC0_RNCR0          (0x40090114U) /**< \brief (TC0) Receive Next Counter Register (pdc = 0) */
#define REG_TC0_PTCR0          (0x40090120U) /**< \brief (TC0) Transfer Control Register (pdc = 0) */
#define REG_TC0_PTSR0          (0x40090124U) /**< \brief (TC0) Transfer Status Register (pdc = 0) */
#define REG_TC0_RPR1           (0x40090140U) /**< \brief (TC0) Receive Pointer Register (pdc = 1) */
#define REG_TC0_RCR1           (0x40090144U) /**< \brief (TC0) Receive Counter Register (pdc = 1) */
#define REG_TC0_RNPR1          (0x40090150U) /**< \brief (TC0) Receive Next Pointer Register (pdc = 1) */
#define REG_TC0_RNCR1          (0x40090154U) /**< \brief (TC0) Receive Next Counter Register (pdc = 1) */
#define REG_TC0_PTCR1          (0x40090160U) /**< \brief (TC0) Transfer Control Register (pdc = 1) */
#define REG_TC0_PTSR1          (0x40090164U) /**< \brief (TC0) Transfer Status Register (pdc = 1) */
#define REG_TC0_RPR2           (0x40090180U) /**< \brief (TC0) Receive Pointer Register (pdc = 2) */
#define REG_TC0_RCR2           (0x40090184U) /**< \brief (TC0) Receive Counter Register (pdc = 2) */
#define REG_TC0_RNPR2          (0x40090190U) /**< \brief (TC0) Receive Next Pointer Register (pdc = 2) */
#define REG_TC0_RNCR2          (0x40090194U) /**< \brief (TC0) Receive Next Counter Register (pdc = 2) */
#define REG_TC0_PTCR2          (0x400901A0U) /**< \brief (TC0) Transfer Control Register (pdc = 2) */
#define REG_TC0_PTSR2          (0x400901A4U) /**< \brief (TC0) Transfer Status Register (pdc = 2) */
#else
#define REG_TC0_CCR0  (*(WoReg*)0x40090000U) /**< \brief (TC0) Channel Control Register (channel = 0) */
#define REG_TC0_CMR0  (*(RwReg*)0x40090004U) /**< \brief (TC0) Channel Mode Register (channel = 0) */
#define REG_TC0_SMMR0 (*(RwReg*)0x40090008U) /**< \brief (TC0) Stepper Motor Mode Register (channel = 0) */
#define REG_TC0_RAB0  (*(RoReg*)0x4009000CU) /**< \brief (TC0) Register AB (channel = 0) */
#define REG_TC0_CV0   (*(RoReg*)0x40090010U) /**< \brief (TC0) Counter Value (channel = 0) */
#define REG_TC0_RA0   (*(RwReg*)0x40090014U) /**< \brief (TC0) Register A (channel = 0) */
#define REG_TC0_RB0   (*(RwReg*)0x40090018U) /**< \brief (TC0) Register B (channel = 0) */
#define REG_TC0_RC0   (*(RwReg*)0x4009001CU) /**< \brief (TC0) Register C (channel = 0) */
#define REG_TC0_SR0   (*(RoReg*)0x40090020U) /**< \brief (TC0) Status Register (channel = 0) */
#define REG_TC0_IER0  (*(WoReg*)0x40090024U) /**< \brief (TC0) Interrupt Enable Register (channel = 0) */
#define REG_TC0_IDR0  (*(WoReg*)0x40090028U) /**< \brief (TC0) Interrupt Disable Register (channel = 0) */
#define REG_TC0_IMR0  (*(RoReg*)0x4009002CU) /**< \brief (TC0) Interrupt Mask Register (channel = 0) */
#define REG_TC0_EMR0  (*(RwReg*)0x40090030U) /**< \brief (TC0) Extended Mode Register (channel = 0) */
#define REG_TC0_CCR1  (*(WoReg*)0x40090040U) /**< \brief (TC0) Channel Control Register (channel = 1) */
#define REG_TC0_CMR1  (*(RwReg*)0x40090044U) /**< \brief (TC0) Channel Mode Register (channel = 1) */
#define REG_TC0_SMMR1 (*(RwReg*)0x40090048U) /**< \brief (TC0) Stepper Motor Mode Register (channel = 1) */
#define REG_TC0_RAB1  (*(RoReg*)0x4009004CU) /**< \brief (TC0) Register AB (channel = 1) */
#define REG_TC0_CV1   (*(RoReg*)0x40090050U) /**< \brief (TC0) Counter Value (channel = 1) */
#define REG_TC0_RA1   (*(RwReg*)0x40090054U) /**< \brief (TC0) Register A (channel = 1) */
#define REG_TC0_RB1   (*(RwReg*)0x40090058U) /**< \brief (TC0) Register B (channel = 1) */
#define REG_TC0_RC1   (*(RwReg*)0x4009005CU) /**< \brief (TC0) Register C (channel = 1) */
#define REG_TC0_SR1   (*(RoReg*)0x40090060U) /**< \brief (TC0) Status Register (channel = 1) */
#define REG_TC0_IER1  (*(WoReg*)0x40090064U) /**< \brief (TC0) Interrupt Enable Register (channel = 1) */
#define REG_TC0_IDR1  (*(WoReg*)0x40090068U) /**< \brief (TC0) Interrupt Disable Register (channel = 1) */
#define REG_TC0_IMR1  (*(RoReg*)0x4009006CU) /**< \brief (TC0) Interrupt Mask Register (channel = 1) */
#define REG_TC0_EMR1  (*(RwReg*)0x40090070U) /**< \brief (TC0) Extended Mode Register (channel = 1) */
#define REG_TC0_CCR2  (*(WoReg*)0x40090080U) /**< \brief (TC0) Channel Control Register (channel = 2) */
#define REG_TC0_CMR2  (*(RwReg*)0x40090084U) /**< \brief (TC0) Channel Mode Register (channel = 2) */
#define REG_TC0_SMMR2 (*(RwReg*)0x40090088U) /**< \brief (TC0) Stepper Motor Mode Register (channel = 2) */
#define REG_TC0_RAB2  (*(RoReg*)0x4009008CU) /**< \brief (TC0) Register AB (channel = 2) */
#define REG_TC0_CV2   (*(RoReg*)0x40090090U) /**< \brief (TC0) Counter Value (channel = 2) */
#define REG_TC0_RA2   (*(RwReg*)0x40090094U) /**< \brief (TC0) Register A (channel = 2) */
#define REG_TC0_RB2   (*(RwReg*)0x40090098U) /**< \brief (TC0) Register B (channel = 2) */
#define REG_TC0_RC2   (*(RwReg*)0x4009009CU) /**< \brief (TC0) Register C (channel = 2) */
#define REG_TC0_SR2   (*(RoReg*)0x400900A0U) /**< \brief (TC0) Status Register (channel = 2) */
#define REG_TC0_IER2  (*(WoReg*)0x400900A4U) /**< \brief (TC0) Interrupt Enable Register (channel = 2) */
#define REG_TC0_IDR2  (*(WoReg*)0x400900A8U) /**< \brief (TC0) Interrupt Disable Register (channel = 2) */
#define REG_TC0_IMR2  (*(RoReg*)0x400900ACU) /**< \brief (TC0) Interrupt Mask Register (channel = 2) */
#define REG_TC0_EMR2  (*(RwReg*)0x400900B0U) /**< \brief (TC0) Extended Mode Register (channel = 2) */
#define REG_TC0_BCR   (*(WoReg*)0x400900C0U) /**< \brief (TC0) Block Control Register */
#define REG_TC0_BMR   (*(RwReg*)0x400900C4U) /**< \brief (TC0) Block Mode Register */
#define REG_TC0_QIER  (*(WoReg*)0x400900C8U) /**< \brief (TC0) QDEC Interrupt Enable Register */
#define REG_TC0_QIDR  (*(WoReg*)0x400900CCU) /**< \brief (TC0) QDEC Interrupt Disable Register */
#define REG_TC0_QIMR  (*(RoReg*)0x400900D0U) /**< \brief (TC0) QDEC Interrupt Mask Register */
#define REG_TC0_QISR  (*(RoReg*)0x400900D4U) /**< \brief (TC0) QDEC Interrupt Status Register */
#define REG_TC0_FMR   (*(RwReg*)0x400900D8U) /**< \brief (TC0) Fault Mode Register */
#define REG_TC0_WPMR  (*(RwReg*)0x400900E4U) /**< \brief (TC0) Write Protect Mode Register */
#define REG_TC0_RPR0  (*(RwReg*)0x40090100U) /**< \brief (TC0) Receive Pointer Register (pdc = 0) */
#define REG_TC0_RCR0  (*(RwReg*)0x40090104U) /**< \brief (TC0) Receive Counter Register (pdc = 0) */
#define REG_TC0_RNPR0 (*(RwReg*)0x40090110U) /**< \brief (TC0) Receive Next Pointer Register (pdc = 0) */
#define REG_TC0_RNCR0 (*(RwReg*)0x40090114U) /**< \brief (TC0) Receive Next Counter Register (pdc = 0) */
#define REG_TC0_PTCR0 (*(WoReg*)0x40090120U) /**< \brief (TC0) Transfer Control Register (pdc = 0) */
#define REG_TC0_PTSR0 (*(RoReg*)0x40090124U) /**< \brief (TC0) Transfer Status Register (pdc = 0) */
#define REG_TC0_RPR1  (*(RwReg*)0x40090140U) /**< \brief (TC0) Receive Pointer Register (pdc = 1) */
#define REG_TC0_RCR1  (*(RwReg*)0x40090144U) /**< \brief (TC0) Receive Counter Register (pdc = 1) */
#define REG_TC0_RNPR1 (*(RwReg*)0x40090150U) /**< \brief (TC0) Receive Next Pointer Register (pdc = 1) */
#define REG_TC0_RNCR1 (*(RwReg*)0x40090154U) /**< \brief (TC0) Receive Next Counter Register (pdc = 1) */
#define REG_TC0_PTCR1 (*(WoReg*)0x40090160U) /**< \brief (TC0) Transfer Control Register (pdc = 1) */
#define REG_TC0_PTSR1 (*(RoReg*)0x40090164U) /**< \brief (TC0) Transfer Status Register (pdc = 1) */
#define REG_TC0_RPR2  (*(RwReg*)0x40090180U) /**< \brief (TC0) Receive Pointer Register (pdc = 2) */
#define REG_TC0_RCR2  (*(RwReg*)0x40090184U) /**< \brief (TC0) Receive Counter Register (pdc = 2) */
#define REG_TC0_RNPR2 (*(RwReg*)0x40090190U) /**< \brief (TC0) Receive Next Pointer Register (pdc = 2) */
#define REG_TC0_RNCR2 (*(RwReg*)0x40090194U) /**< \brief (TC0) Receive Next Counter Register (pdc = 2) */
#define REG_TC0_PTCR2 (*(WoReg*)0x400901A0U) /**< \brief (TC0) Transfer Control Register (pdc = 2) */
#define REG_TC0_PTSR2 (*(RoReg*)0x400901A4U) /**< \brief (TC0) Transfer Status Register (pdc = 2) */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAM4E_TC0_INSTANCE_ */
