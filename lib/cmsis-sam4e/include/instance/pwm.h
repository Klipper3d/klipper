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

#ifndef _SAM4E_PWM_INSTANCE_
#define _SAM4E_PWM_INSTANCE_

/* ========== Register definition for PWM peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_PWM_CLK               (0x40000000U) /**< \brief (PWM) PWM Clock Register */
#define REG_PWM_ENA               (0x40000004U) /**< \brief (PWM) PWM Enable Register */
#define REG_PWM_DIS               (0x40000008U) /**< \brief (PWM) PWM Disable Register */
#define REG_PWM_SR                (0x4000000CU) /**< \brief (PWM) PWM Status Register */
#define REG_PWM_IER1              (0x40000010U) /**< \brief (PWM) PWM Interrupt Enable Register 1 */
#define REG_PWM_IDR1              (0x40000014U) /**< \brief (PWM) PWM Interrupt Disable Register 1 */
#define REG_PWM_IMR1              (0x40000018U) /**< \brief (PWM) PWM Interrupt Mask Register 1 */
#define REG_PWM_ISR1              (0x4000001CU) /**< \brief (PWM) PWM Interrupt Status Register 1 */
#define REG_PWM_SCM               (0x40000020U) /**< \brief (PWM) PWM Sync Channels Mode Register */
#define REG_PWM_SCUC              (0x40000028U) /**< \brief (PWM) PWM Sync Channels Update Control Register */
#define REG_PWM_SCUP              (0x4000002CU) /**< \brief (PWM) PWM Sync Channels Update Period Register */
#define REG_PWM_SCUPUPD           (0x40000030U) /**< \brief (PWM) PWM Sync Channels Update Period Update Register */
#define REG_PWM_IER2              (0x40000034U) /**< \brief (PWM) PWM Interrupt Enable Register 2 */
#define REG_PWM_IDR2              (0x40000038U) /**< \brief (PWM) PWM Interrupt Disable Register 2 */
#define REG_PWM_IMR2              (0x4000003CU) /**< \brief (PWM) PWM Interrupt Mask Register 2 */
#define REG_PWM_ISR2              (0x40000040U) /**< \brief (PWM) PWM Interrupt Status Register 2 */
#define REG_PWM_OOV               (0x40000044U) /**< \brief (PWM) PWM Output Override Value Register */
#define REG_PWM_OS                (0x40000048U) /**< \brief (PWM) PWM Output Selection Register */
#define REG_PWM_OSS               (0x4000004CU) /**< \brief (PWM) PWM Output Selection Set Register */
#define REG_PWM_OSC               (0x40000050U) /**< \brief (PWM) PWM Output Selection Clear Register */
#define REG_PWM_OSSUPD            (0x40000054U) /**< \brief (PWM) PWM Output Selection Set Update Register */
#define REG_PWM_OSCUPD            (0x40000058U) /**< \brief (PWM) PWM Output Selection Clear Update Register */
#define REG_PWM_FMR               (0x4000005CU) /**< \brief (PWM) PWM Fault Mode Register */
#define REG_PWM_FSR               (0x40000060U) /**< \brief (PWM) PWM Fault Status Register */
#define REG_PWM_FCR               (0x40000064U) /**< \brief (PWM) PWM Fault Clear Register */
#define REG_PWM_FPV1              (0x40000068U) /**< \brief (PWM) PWM Fault Protection Value Register 1 */
#define REG_PWM_FPE               (0x4000006CU) /**< \brief (PWM) PWM Fault Protection Enable Register */
#define REG_PWM_ELMR              (0x4000007CU) /**< \brief (PWM) PWM Event Line 0 Mode Register */
#define REG_PWM_SSPR              (0x400000A0U) /**< \brief (PWM) PWM Spread Spectrum Register */
#define REG_PWM_SSPUP             (0x400000A4U) /**< \brief (PWM) PWM Spread Spectrum Update Register */
#define REG_PWM_SMMR              (0x400000B0U) /**< \brief (PWM) PWM Stepper Motor Mode Register */
#define REG_PWM_FPV2              (0x400000C0U) /**< \brief (PWM) PWM Fault Protection Value 2 Register */
#define REG_PWM_WPCR              (0x400000E4U) /**< \brief (PWM) PWM Write Protect Control Register */
#define REG_PWM_WPSR              (0x400000E8U) /**< \brief (PWM) PWM Write Protect Status Register */
#define REG_PWM_TPR               (0x40000108U) /**< \brief (PWM) Transmit Pointer Register */
#define REG_PWM_TCR               (0x4000010CU) /**< \brief (PWM) Transmit Counter Register */
#define REG_PWM_TNPR              (0x40000118U) /**< \brief (PWM) Transmit Next Pointer Register */
#define REG_PWM_TNCR              (0x4000011CU) /**< \brief (PWM) Transmit Next Counter Register */
#define REG_PWM_PTCR              (0x40000120U) /**< \brief (PWM) Transfer Control Register */
#define REG_PWM_PTSR              (0x40000124U) /**< \brief (PWM) Transfer Status Register */
#define REG_PWM_CMPV0             (0x40000130U) /**< \brief (PWM) PWM Comparison 0 Value Register */
#define REG_PWM_CMPVUPD0          (0x40000134U) /**< \brief (PWM) PWM Comparison 0 Value Update Register */
#define REG_PWM_CMPM0             (0x40000138U) /**< \brief (PWM) PWM Comparison 0 Mode Register */
#define REG_PWM_CMPMUPD0          (0x4000013CU) /**< \brief (PWM) PWM Comparison 0 Mode Update Register */
#define REG_PWM_CMPV1             (0x40000140U) /**< \brief (PWM) PWM Comparison 1 Value Register */
#define REG_PWM_CMPVUPD1          (0x40000144U) /**< \brief (PWM) PWM Comparison 1 Value Update Register */
#define REG_PWM_CMPM1             (0x40000148U) /**< \brief (PWM) PWM Comparison 1 Mode Register */
#define REG_PWM_CMPMUPD1          (0x4000014CU) /**< \brief (PWM) PWM Comparison 1 Mode Update Register */
#define REG_PWM_CMPV2             (0x40000150U) /**< \brief (PWM) PWM Comparison 2 Value Register */
#define REG_PWM_CMPVUPD2          (0x40000154U) /**< \brief (PWM) PWM Comparison 2 Value Update Register */
#define REG_PWM_CMPM2             (0x40000158U) /**< \brief (PWM) PWM Comparison 2 Mode Register */
#define REG_PWM_CMPMUPD2          (0x4000015CU) /**< \brief (PWM) PWM Comparison 2 Mode Update Register */
#define REG_PWM_CMPV3             (0x40000160U) /**< \brief (PWM) PWM Comparison 3 Value Register */
#define REG_PWM_CMPVUPD3          (0x40000164U) /**< \brief (PWM) PWM Comparison 3 Value Update Register */
#define REG_PWM_CMPM3             (0x40000168U) /**< \brief (PWM) PWM Comparison 3 Mode Register */
#define REG_PWM_CMPMUPD3          (0x4000016CU) /**< \brief (PWM) PWM Comparison 3 Mode Update Register */
#define REG_PWM_CMPV4             (0x40000170U) /**< \brief (PWM) PWM Comparison 4 Value Register */
#define REG_PWM_CMPVUPD4          (0x40000174U) /**< \brief (PWM) PWM Comparison 4 Value Update Register */
#define REG_PWM_CMPM4             (0x40000178U) /**< \brief (PWM) PWM Comparison 4 Mode Register */
#define REG_PWM_CMPMUPD4          (0x4000017CU) /**< \brief (PWM) PWM Comparison 4 Mode Update Register */
#define REG_PWM_CMPV5             (0x40000180U) /**< \brief (PWM) PWM Comparison 5 Value Register */
#define REG_PWM_CMPVUPD5          (0x40000184U) /**< \brief (PWM) PWM Comparison 5 Value Update Register */
#define REG_PWM_CMPM5             (0x40000188U) /**< \brief (PWM) PWM Comparison 5 Mode Register */
#define REG_PWM_CMPMUPD5          (0x4000018CU) /**< \brief (PWM) PWM Comparison 5 Mode Update Register */
#define REG_PWM_CMPV6             (0x40000190U) /**< \brief (PWM) PWM Comparison 6 Value Register */
#define REG_PWM_CMPVUPD6          (0x40000194U) /**< \brief (PWM) PWM Comparison 6 Value Update Register */
#define REG_PWM_CMPM6             (0x40000198U) /**< \brief (PWM) PWM Comparison 6 Mode Register */
#define REG_PWM_CMPMUPD6          (0x4000019CU) /**< \brief (PWM) PWM Comparison 6 Mode Update Register */
#define REG_PWM_CMPV7             (0x400001A0U) /**< \brief (PWM) PWM Comparison 7 Value Register */
#define REG_PWM_CMPVUPD7          (0x400001A4U) /**< \brief (PWM) PWM Comparison 7 Value Update Register */
#define REG_PWM_CMPM7             (0x400001A8U) /**< \brief (PWM) PWM Comparison 7 Mode Register */
#define REG_PWM_CMPMUPD7          (0x400001ACU) /**< \brief (PWM) PWM Comparison 7 Mode Update Register */
#define REG_PWM_CMR0              (0x40000200U) /**< \brief (PWM) PWM Channel Mode Register (ch_num = 0) */
#define REG_PWM_CDTY0             (0x40000204U) /**< \brief (PWM) PWM Channel Duty Cycle Register (ch_num = 0) */
#define REG_PWM_CDTYUPD0          (0x40000208U) /**< \brief (PWM) PWM Channel Duty Cycle Update Register (ch_num = 0) */
#define REG_PWM_CPRD0             (0x4000020CU) /**< \brief (PWM) PWM Channel Period Register (ch_num = 0) */
#define REG_PWM_CPRDUPD0          (0x40000210U) /**< \brief (PWM) PWM Channel Period Update Register (ch_num = 0) */
#define REG_PWM_CCNT0             (0x40000214U) /**< \brief (PWM) PWM Channel Counter Register (ch_num = 0) */
#define REG_PWM_DT0               (0x40000218U) /**< \brief (PWM) PWM Channel Dead Time Register (ch_num = 0) */
#define REG_PWM_DTUPD0            (0x4000021CU) /**< \brief (PWM) PWM Channel Dead Time Update Register (ch_num = 0) */
#define REG_PWM_CMR1              (0x40000220U) /**< \brief (PWM) PWM Channel Mode Register (ch_num = 1) */
#define REG_PWM_CDTY1             (0x40000224U) /**< \brief (PWM) PWM Channel Duty Cycle Register (ch_num = 1) */
#define REG_PWM_CDTYUPD1          (0x40000228U) /**< \brief (PWM) PWM Channel Duty Cycle Update Register (ch_num = 1) */
#define REG_PWM_CPRD1             (0x4000022CU) /**< \brief (PWM) PWM Channel Period Register (ch_num = 1) */
#define REG_PWM_CPRDUPD1          (0x40000230U) /**< \brief (PWM) PWM Channel Period Update Register (ch_num = 1) */
#define REG_PWM_CCNT1             (0x40000234U) /**< \brief (PWM) PWM Channel Counter Register (ch_num = 1) */
#define REG_PWM_DT1               (0x40000238U) /**< \brief (PWM) PWM Channel Dead Time Register (ch_num = 1) */
#define REG_PWM_DTUPD1            (0x4000023CU) /**< \brief (PWM) PWM Channel Dead Time Update Register (ch_num = 1) */
#define REG_PWM_CMR2              (0x40000240U) /**< \brief (PWM) PWM Channel Mode Register (ch_num = 2) */
#define REG_PWM_CDTY2             (0x40000244U) /**< \brief (PWM) PWM Channel Duty Cycle Register (ch_num = 2) */
#define REG_PWM_CDTYUPD2          (0x40000248U) /**< \brief (PWM) PWM Channel Duty Cycle Update Register (ch_num = 2) */
#define REG_PWM_CPRD2             (0x4000024CU) /**< \brief (PWM) PWM Channel Period Register (ch_num = 2) */
#define REG_PWM_CPRDUPD2          (0x40000250U) /**< \brief (PWM) PWM Channel Period Update Register (ch_num = 2) */
#define REG_PWM_CCNT2             (0x40000254U) /**< \brief (PWM) PWM Channel Counter Register (ch_num = 2) */
#define REG_PWM_DT2               (0x40000258U) /**< \brief (PWM) PWM Channel Dead Time Register (ch_num = 2) */
#define REG_PWM_DTUPD2            (0x4000025CU) /**< \brief (PWM) PWM Channel Dead Time Update Register (ch_num = 2) */
#define REG_PWM_CMR3              (0x40000260U) /**< \brief (PWM) PWM Channel Mode Register (ch_num = 3) */
#define REG_PWM_CDTY3             (0x40000264U) /**< \brief (PWM) PWM Channel Duty Cycle Register (ch_num = 3) */
#define REG_PWM_CDTYUPD3          (0x40000268U) /**< \brief (PWM) PWM Channel Duty Cycle Update Register (ch_num = 3) */
#define REG_PWM_CPRD3             (0x4000026CU) /**< \brief (PWM) PWM Channel Period Register (ch_num = 3) */
#define REG_PWM_CPRDUPD3          (0x40000270U) /**< \brief (PWM) PWM Channel Period Update Register (ch_num = 3) */
#define REG_PWM_CCNT3             (0x40000274U) /**< \brief (PWM) PWM Channel Counter Register (ch_num = 3) */
#define REG_PWM_DT3               (0x40000278U) /**< \brief (PWM) PWM Channel Dead Time Register (ch_num = 3) */
#define REG_PWM_DTUPD3            (0x4000027CU) /**< \brief (PWM) PWM Channel Dead Time Update Register (ch_num = 3) */
#define REG_PWM_CMUPD0            (0x40000400U) /**< \brief (PWM) PWM Channel Mode Update Register (ch_num = 0) */
#define REG_PWM_CAE0              (0x40000404U) /**< \brief (PWM) PWM Channel Additional Edge Register (ch_num = 0) */
#define REG_PWM_CAEUPD0           (0x40000408U) /**< \brief (PWM) PWM Channel Additional Edge Update Register (ch_num = 0) */
#define REG_PWM_CMUPD1            (0x40000420U) /**< \brief (PWM) PWM Channel Mode Update Register (ch_num = 1) */
#define REG_PWM_CAE1              (0x40000424U) /**< \brief (PWM) PWM Channel Additional Edge Register (ch_num = 1) */
#define REG_PWM_CAEUPD1           (0x40000428U) /**< \brief (PWM) PWM Channel Additional Edge Update Register (ch_num = 1) */
#define REG_PWM_CMUPD2            (0x40000440U) /**< \brief (PWM) PWM Channel Mode Update Register (ch_num = 2) */
#define REG_PWM_CAE2              (0x40000444U) /**< \brief (PWM) PWM Channel Additional Edge Register (ch_num = 2) */
#define REG_PWM_CAEUPD2           (0x40000448U) /**< \brief (PWM) PWM Channel Additional Edge Update Register (ch_num = 2) */
#define REG_PWM_CMUPD3            (0x40000460U) /**< \brief (PWM) PWM Channel Mode Update Register (ch_num = 3) */
#define REG_PWM_CAE3              (0x40000464U) /**< \brief (PWM) PWM Channel Additional Edge Register (ch_num = 3) */
#define REG_PWM_CAEUPD3           (0x40000468U) /**< \brief (PWM) PWM Channel Additional Edge Update Register (ch_num = 3) */
#else
#define REG_PWM_CLK      (*(RwReg*)0x40000000U) /**< \brief (PWM) PWM Clock Register */
#define REG_PWM_ENA      (*(WoReg*)0x40000004U) /**< \brief (PWM) PWM Enable Register */
#define REG_PWM_DIS      (*(WoReg*)0x40000008U) /**< \brief (PWM) PWM Disable Register */
#define REG_PWM_SR       (*(RoReg*)0x4000000CU) /**< \brief (PWM) PWM Status Register */
#define REG_PWM_IER1     (*(WoReg*)0x40000010U) /**< \brief (PWM) PWM Interrupt Enable Register 1 */
#define REG_PWM_IDR1     (*(WoReg*)0x40000014U) /**< \brief (PWM) PWM Interrupt Disable Register 1 */
#define REG_PWM_IMR1     (*(RoReg*)0x40000018U) /**< \brief (PWM) PWM Interrupt Mask Register 1 */
#define REG_PWM_ISR1     (*(RoReg*)0x4000001CU) /**< \brief (PWM) PWM Interrupt Status Register 1 */
#define REG_PWM_SCM      (*(RwReg*)0x40000020U) /**< \brief (PWM) PWM Sync Channels Mode Register */
#define REG_PWM_SCUC     (*(RwReg*)0x40000028U) /**< \brief (PWM) PWM Sync Channels Update Control Register */
#define REG_PWM_SCUP     (*(RwReg*)0x4000002CU) /**< \brief (PWM) PWM Sync Channels Update Period Register */
#define REG_PWM_SCUPUPD  (*(WoReg*)0x40000030U) /**< \brief (PWM) PWM Sync Channels Update Period Update Register */
#define REG_PWM_IER2     (*(WoReg*)0x40000034U) /**< \brief (PWM) PWM Interrupt Enable Register 2 */
#define REG_PWM_IDR2     (*(WoReg*)0x40000038U) /**< \brief (PWM) PWM Interrupt Disable Register 2 */
#define REG_PWM_IMR2     (*(RoReg*)0x4000003CU) /**< \brief (PWM) PWM Interrupt Mask Register 2 */
#define REG_PWM_ISR2     (*(RoReg*)0x40000040U) /**< \brief (PWM) PWM Interrupt Status Register 2 */
#define REG_PWM_OOV      (*(RwReg*)0x40000044U) /**< \brief (PWM) PWM Output Override Value Register */
#define REG_PWM_OS       (*(RwReg*)0x40000048U) /**< \brief (PWM) PWM Output Selection Register */
#define REG_PWM_OSS      (*(WoReg*)0x4000004CU) /**< \brief (PWM) PWM Output Selection Set Register */
#define REG_PWM_OSC      (*(WoReg*)0x40000050U) /**< \brief (PWM) PWM Output Selection Clear Register */
#define REG_PWM_OSSUPD   (*(WoReg*)0x40000054U) /**< \brief (PWM) PWM Output Selection Set Update Register */
#define REG_PWM_OSCUPD   (*(WoReg*)0x40000058U) /**< \brief (PWM) PWM Output Selection Clear Update Register */
#define REG_PWM_FMR      (*(RwReg*)0x4000005CU) /**< \brief (PWM) PWM Fault Mode Register */
#define REG_PWM_FSR      (*(RoReg*)0x40000060U) /**< \brief (PWM) PWM Fault Status Register */
#define REG_PWM_FCR      (*(WoReg*)0x40000064U) /**< \brief (PWM) PWM Fault Clear Register */
#define REG_PWM_FPV1     (*(RwReg*)0x40000068U) /**< \brief (PWM) PWM Fault Protection Value Register 1 */
#define REG_PWM_FPE      (*(RwReg*)0x4000006CU) /**< \brief (PWM) PWM Fault Protection Enable Register */
#define REG_PWM_ELMR     (*(RwReg*)0x4000007CU) /**< \brief (PWM) PWM Event Line 0 Mode Register */
#define REG_PWM_SSPR     (*(RwReg*)0x400000A0U) /**< \brief (PWM) PWM Spread Spectrum Register */
#define REG_PWM_SSPUP    (*(WoReg*)0x400000A4U) /**< \brief (PWM) PWM Spread Spectrum Update Register */
#define REG_PWM_SMMR     (*(RwReg*)0x400000B0U) /**< \brief (PWM) PWM Stepper Motor Mode Register */
#define REG_PWM_FPV2     (*(RwReg*)0x400000C0U) /**< \brief (PWM) PWM Fault Protection Value 2 Register */
#define REG_PWM_WPCR     (*(WoReg*)0x400000E4U) /**< \brief (PWM) PWM Write Protect Control Register */
#define REG_PWM_WPSR     (*(RoReg*)0x400000E8U) /**< \brief (PWM) PWM Write Protect Status Register */
#define REG_PWM_TPR      (*(RwReg*)0x40000108U) /**< \brief (PWM) Transmit Pointer Register */
#define REG_PWM_TCR      (*(RwReg*)0x4000010CU) /**< \brief (PWM) Transmit Counter Register */
#define REG_PWM_TNPR     (*(RwReg*)0x40000118U) /**< \brief (PWM) Transmit Next Pointer Register */
#define REG_PWM_TNCR     (*(RwReg*)0x4000011CU) /**< \brief (PWM) Transmit Next Counter Register */
#define REG_PWM_PTCR     (*(WoReg*)0x40000120U) /**< \brief (PWM) Transfer Control Register */
#define REG_PWM_PTSR     (*(RoReg*)0x40000124U) /**< \brief (PWM) Transfer Status Register */
#define REG_PWM_CMPV0    (*(RwReg*)0x40000130U) /**< \brief (PWM) PWM Comparison 0 Value Register */
#define REG_PWM_CMPVUPD0 (*(WoReg*)0x40000134U) /**< \brief (PWM) PWM Comparison 0 Value Update Register */
#define REG_PWM_CMPM0    (*(RwReg*)0x40000138U) /**< \brief (PWM) PWM Comparison 0 Mode Register */
#define REG_PWM_CMPMUPD0 (*(WoReg*)0x4000013CU) /**< \brief (PWM) PWM Comparison 0 Mode Update Register */
#define REG_PWM_CMPV1    (*(RwReg*)0x40000140U) /**< \brief (PWM) PWM Comparison 1 Value Register */
#define REG_PWM_CMPVUPD1 (*(WoReg*)0x40000144U) /**< \brief (PWM) PWM Comparison 1 Value Update Register */
#define REG_PWM_CMPM1    (*(RwReg*)0x40000148U) /**< \brief (PWM) PWM Comparison 1 Mode Register */
#define REG_PWM_CMPMUPD1 (*(WoReg*)0x4000014CU) /**< \brief (PWM) PWM Comparison 1 Mode Update Register */
#define REG_PWM_CMPV2    (*(RwReg*)0x40000150U) /**< \brief (PWM) PWM Comparison 2 Value Register */
#define REG_PWM_CMPVUPD2 (*(WoReg*)0x40000154U) /**< \brief (PWM) PWM Comparison 2 Value Update Register */
#define REG_PWM_CMPM2    (*(RwReg*)0x40000158U) /**< \brief (PWM) PWM Comparison 2 Mode Register */
#define REG_PWM_CMPMUPD2 (*(WoReg*)0x4000015CU) /**< \brief (PWM) PWM Comparison 2 Mode Update Register */
#define REG_PWM_CMPV3    (*(RwReg*)0x40000160U) /**< \brief (PWM) PWM Comparison 3 Value Register */
#define REG_PWM_CMPVUPD3 (*(WoReg*)0x40000164U) /**< \brief (PWM) PWM Comparison 3 Value Update Register */
#define REG_PWM_CMPM3    (*(RwReg*)0x40000168U) /**< \brief (PWM) PWM Comparison 3 Mode Register */
#define REG_PWM_CMPMUPD3 (*(WoReg*)0x4000016CU) /**< \brief (PWM) PWM Comparison 3 Mode Update Register */
#define REG_PWM_CMPV4    (*(RwReg*)0x40000170U) /**< \brief (PWM) PWM Comparison 4 Value Register */
#define REG_PWM_CMPVUPD4 (*(WoReg*)0x40000174U) /**< \brief (PWM) PWM Comparison 4 Value Update Register */
#define REG_PWM_CMPM4    (*(RwReg*)0x40000178U) /**< \brief (PWM) PWM Comparison 4 Mode Register */
#define REG_PWM_CMPMUPD4 (*(WoReg*)0x4000017CU) /**< \brief (PWM) PWM Comparison 4 Mode Update Register */
#define REG_PWM_CMPV5    (*(RwReg*)0x40000180U) /**< \brief (PWM) PWM Comparison 5 Value Register */
#define REG_PWM_CMPVUPD5 (*(WoReg*)0x40000184U) /**< \brief (PWM) PWM Comparison 5 Value Update Register */
#define REG_PWM_CMPM5    (*(RwReg*)0x40000188U) /**< \brief (PWM) PWM Comparison 5 Mode Register */
#define REG_PWM_CMPMUPD5 (*(WoReg*)0x4000018CU) /**< \brief (PWM) PWM Comparison 5 Mode Update Register */
#define REG_PWM_CMPV6    (*(RwReg*)0x40000190U) /**< \brief (PWM) PWM Comparison 6 Value Register */
#define REG_PWM_CMPVUPD6 (*(WoReg*)0x40000194U) /**< \brief (PWM) PWM Comparison 6 Value Update Register */
#define REG_PWM_CMPM6    (*(RwReg*)0x40000198U) /**< \brief (PWM) PWM Comparison 6 Mode Register */
#define REG_PWM_CMPMUPD6 (*(WoReg*)0x4000019CU) /**< \brief (PWM) PWM Comparison 6 Mode Update Register */
#define REG_PWM_CMPV7    (*(RwReg*)0x400001A0U) /**< \brief (PWM) PWM Comparison 7 Value Register */
#define REG_PWM_CMPVUPD7 (*(WoReg*)0x400001A4U) /**< \brief (PWM) PWM Comparison 7 Value Update Register */
#define REG_PWM_CMPM7    (*(RwReg*)0x400001A8U) /**< \brief (PWM) PWM Comparison 7 Mode Register */
#define REG_PWM_CMPMUPD7 (*(WoReg*)0x400001ACU) /**< \brief (PWM) PWM Comparison 7 Mode Update Register */
#define REG_PWM_CMR0     (*(RwReg*)0x40000200U) /**< \brief (PWM) PWM Channel Mode Register (ch_num = 0) */
#define REG_PWM_CDTY0    (*(RwReg*)0x40000204U) /**< \brief (PWM) PWM Channel Duty Cycle Register (ch_num = 0) */
#define REG_PWM_CDTYUPD0 (*(WoReg*)0x40000208U) /**< \brief (PWM) PWM Channel Duty Cycle Update Register (ch_num = 0) */
#define REG_PWM_CPRD0    (*(RwReg*)0x4000020CU) /**< \brief (PWM) PWM Channel Period Register (ch_num = 0) */
#define REG_PWM_CPRDUPD0 (*(WoReg*)0x40000210U) /**< \brief (PWM) PWM Channel Period Update Register (ch_num = 0) */
#define REG_PWM_CCNT0    (*(RoReg*)0x40000214U) /**< \brief (PWM) PWM Channel Counter Register (ch_num = 0) */
#define REG_PWM_DT0      (*(RwReg*)0x40000218U) /**< \brief (PWM) PWM Channel Dead Time Register (ch_num = 0) */
#define REG_PWM_DTUPD0   (*(WoReg*)0x4000021CU) /**< \brief (PWM) PWM Channel Dead Time Update Register (ch_num = 0) */
#define REG_PWM_CMR1     (*(RwReg*)0x40000220U) /**< \brief (PWM) PWM Channel Mode Register (ch_num = 1) */
#define REG_PWM_CDTY1    (*(RwReg*)0x40000224U) /**< \brief (PWM) PWM Channel Duty Cycle Register (ch_num = 1) */
#define REG_PWM_CDTYUPD1 (*(WoReg*)0x40000228U) /**< \brief (PWM) PWM Channel Duty Cycle Update Register (ch_num = 1) */
#define REG_PWM_CPRD1    (*(RwReg*)0x4000022CU) /**< \brief (PWM) PWM Channel Period Register (ch_num = 1) */
#define REG_PWM_CPRDUPD1 (*(WoReg*)0x40000230U) /**< \brief (PWM) PWM Channel Period Update Register (ch_num = 1) */
#define REG_PWM_CCNT1    (*(RoReg*)0x40000234U) /**< \brief (PWM) PWM Channel Counter Register (ch_num = 1) */
#define REG_PWM_DT1      (*(RwReg*)0x40000238U) /**< \brief (PWM) PWM Channel Dead Time Register (ch_num = 1) */
#define REG_PWM_DTUPD1   (*(WoReg*)0x4000023CU) /**< \brief (PWM) PWM Channel Dead Time Update Register (ch_num = 1) */
#define REG_PWM_CMR2     (*(RwReg*)0x40000240U) /**< \brief (PWM) PWM Channel Mode Register (ch_num = 2) */
#define REG_PWM_CDTY2    (*(RwReg*)0x40000244U) /**< \brief (PWM) PWM Channel Duty Cycle Register (ch_num = 2) */
#define REG_PWM_CDTYUPD2 (*(WoReg*)0x40000248U) /**< \brief (PWM) PWM Channel Duty Cycle Update Register (ch_num = 2) */
#define REG_PWM_CPRD2    (*(RwReg*)0x4000024CU) /**< \brief (PWM) PWM Channel Period Register (ch_num = 2) */
#define REG_PWM_CPRDUPD2 (*(WoReg*)0x40000250U) /**< \brief (PWM) PWM Channel Period Update Register (ch_num = 2) */
#define REG_PWM_CCNT2    (*(RoReg*)0x40000254U) /**< \brief (PWM) PWM Channel Counter Register (ch_num = 2) */
#define REG_PWM_DT2      (*(RwReg*)0x40000258U) /**< \brief (PWM) PWM Channel Dead Time Register (ch_num = 2) */
#define REG_PWM_DTUPD2   (*(WoReg*)0x4000025CU) /**< \brief (PWM) PWM Channel Dead Time Update Register (ch_num = 2) */
#define REG_PWM_CMR3     (*(RwReg*)0x40000260U) /**< \brief (PWM) PWM Channel Mode Register (ch_num = 3) */
#define REG_PWM_CDTY3    (*(RwReg*)0x40000264U) /**< \brief (PWM) PWM Channel Duty Cycle Register (ch_num = 3) */
#define REG_PWM_CDTYUPD3 (*(WoReg*)0x40000268U) /**< \brief (PWM) PWM Channel Duty Cycle Update Register (ch_num = 3) */
#define REG_PWM_CPRD3    (*(RwReg*)0x4000026CU) /**< \brief (PWM) PWM Channel Period Register (ch_num = 3) */
#define REG_PWM_CPRDUPD3 (*(WoReg*)0x40000270U) /**< \brief (PWM) PWM Channel Period Update Register (ch_num = 3) */
#define REG_PWM_CCNT3    (*(RoReg*)0x40000274U) /**< \brief (PWM) PWM Channel Counter Register (ch_num = 3) */
#define REG_PWM_DT3      (*(RwReg*)0x40000278U) /**< \brief (PWM) PWM Channel Dead Time Register (ch_num = 3) */
#define REG_PWM_DTUPD3   (*(WoReg*)0x4000027CU) /**< \brief (PWM) PWM Channel Dead Time Update Register (ch_num = 3) */
#define REG_PWM_CMUPD0   (*(WoReg*)0x40000400U) /**< \brief (PWM) PWM Channel Mode Update Register (ch_num = 0) */
#define REG_PWM_CAE0     (*(RwReg*)0x40000404U) /**< \brief (PWM) PWM Channel Additional Edge Register (ch_num = 0) */
#define REG_PWM_CAEUPD0  (*(WoReg*)0x40000408U) /**< \brief (PWM) PWM Channel Additional Edge Update Register (ch_num = 0) */
#define REG_PWM_CMUPD1   (*(WoReg*)0x40000420U) /**< \brief (PWM) PWM Channel Mode Update Register (ch_num = 1) */
#define REG_PWM_CAE1     (*(RwReg*)0x40000424U) /**< \brief (PWM) PWM Channel Additional Edge Register (ch_num = 1) */
#define REG_PWM_CAEUPD1  (*(WoReg*)0x40000428U) /**< \brief (PWM) PWM Channel Additional Edge Update Register (ch_num = 1) */
#define REG_PWM_CMUPD2   (*(WoReg*)0x40000440U) /**< \brief (PWM) PWM Channel Mode Update Register (ch_num = 2) */
#define REG_PWM_CAE2     (*(RwReg*)0x40000444U) /**< \brief (PWM) PWM Channel Additional Edge Register (ch_num = 2) */
#define REG_PWM_CAEUPD2  (*(WoReg*)0x40000448U) /**< \brief (PWM) PWM Channel Additional Edge Update Register (ch_num = 2) */
#define REG_PWM_CMUPD3   (*(WoReg*)0x40000460U) /**< \brief (PWM) PWM Channel Mode Update Register (ch_num = 3) */
#define REG_PWM_CAE3     (*(RwReg*)0x40000464U) /**< \brief (PWM) PWM Channel Additional Edge Register (ch_num = 3) */
#define REG_PWM_CAEUPD3  (*(WoReg*)0x40000468U) /**< \brief (PWM) PWM Channel Additional Edge Update Register (ch_num = 3) */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAM4E_PWM_INSTANCE_ */
