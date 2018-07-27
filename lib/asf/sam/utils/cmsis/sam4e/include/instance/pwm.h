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

#ifndef _SAM4E_PWM_INSTANCE_
#define _SAM4E_PWM_INSTANCE_

/* ========== Register definition for PWM peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_PWM_CLK                       (0x40000000U) /**< \brief (PWM) PWM Clock Register */
  #define REG_PWM_ENA                       (0x40000004U) /**< \brief (PWM) PWM Enable Register */
  #define REG_PWM_DIS                       (0x40000008U) /**< \brief (PWM) PWM Disable Register */
  #define REG_PWM_SR                        (0x4000000CU) /**< \brief (PWM) PWM Status Register */
  #define REG_PWM_IER1                      (0x40000010U) /**< \brief (PWM) PWM Interrupt Enable Register 1 */
  #define REG_PWM_IDR1                      (0x40000014U) /**< \brief (PWM) PWM Interrupt Disable Register 1 */
  #define REG_PWM_IMR1                      (0x40000018U) /**< \brief (PWM) PWM Interrupt Mask Register 1 */
  #define REG_PWM_ISR1                      (0x4000001CU) /**< \brief (PWM) PWM Interrupt Status Register 1 */
  #define REG_PWM_SCM                       (0x40000020U) /**< \brief (PWM) PWM Sync Channels Mode Register */
  #define REG_PWM_DMAR                      (0x40000024U) /**< \brief (PWM) PWM DMA Register */
  #define REG_PWM_SCUC                      (0x40000028U) /**< \brief (PWM) PWM Sync Channels Update Control Register */
  #define REG_PWM_SCUP                      (0x4000002CU) /**< \brief (PWM) PWM Sync Channels Update Period Register */
  #define REG_PWM_SCUPUPD                   (0x40000030U) /**< \brief (PWM) PWM Sync Channels Update Period Update Register */
  #define REG_PWM_IER2                      (0x40000034U) /**< \brief (PWM) PWM Interrupt Enable Register 2 */
  #define REG_PWM_IDR2                      (0x40000038U) /**< \brief (PWM) PWM Interrupt Disable Register 2 */
  #define REG_PWM_IMR2                      (0x4000003CU) /**< \brief (PWM) PWM Interrupt Mask Register 2 */
  #define REG_PWM_ISR2                      (0x40000040U) /**< \brief (PWM) PWM Interrupt Status Register 2 */
  #define REG_PWM_OOV                       (0x40000044U) /**< \brief (PWM) PWM Output Override Value Register */
  #define REG_PWM_OS                        (0x40000048U) /**< \brief (PWM) PWM Output Selection Register */
  #define REG_PWM_OSS                       (0x4000004CU) /**< \brief (PWM) PWM Output Selection Set Register */
  #define REG_PWM_OSC                       (0x40000050U) /**< \brief (PWM) PWM Output Selection Clear Register */
  #define REG_PWM_OSSUPD                    (0x40000054U) /**< \brief (PWM) PWM Output Selection Set Update Register */
  #define REG_PWM_OSCUPD                    (0x40000058U) /**< \brief (PWM) PWM Output Selection Clear Update Register */
  #define REG_PWM_FMR                       (0x4000005CU) /**< \brief (PWM) PWM Fault Mode Register */
  #define REG_PWM_FSR                       (0x40000060U) /**< \brief (PWM) PWM Fault Status Register */
  #define REG_PWM_FCR                       (0x40000064U) /**< \brief (PWM) PWM Fault Clear Register */
#define REG_PWM_FPV1              (0x40000068U) /**< \brief (PWM) PWM Fault Protection Value Register 1 */
  #define REG_PWM_FPE                       (0x4000006CU) /**< \brief (PWM) PWM Fault Protection Enable Register */
  #define REG_PWM_ELMR                      (0x4000007CU) /**< \brief (PWM) PWM Event Line 0 Mode Register */
  #define REG_PWM_SSPR                      (0x400000A0U) /**< \brief (PWM) PWM Spread Spectrum Register */
  #define REG_PWM_SSPUP                     (0x400000A4U) /**< \brief (PWM) PWM Spread Spectrum Update Register */
  #define REG_PWM_SMMR                      (0x400000B0U) /**< \brief (PWM) PWM Stepper Motor Mode Register */
  #define REG_PWM_FPV2                      (0x400000C0U) /**< \brief (PWM) PWM Fault Protection Value 2 Register */
  #define REG_PWM_WPCR                      (0x400000E4U) /**< \brief (PWM) PWM Write Protection Control Register */
  #define REG_PWM_WPSR                      (0x400000E8U) /**< \brief (PWM) PWM Write Protection Status Register */
  #define REG_PWM_TPR                       (0x40000108U) /**< \brief (PWM) Transmit Pointer Register */
  #define REG_PWM_TCR                       (0x4000010CU) /**< \brief (PWM) Transmit Counter Register */
  #define REG_PWM_TNPR                      (0x40000118U) /**< \brief (PWM) Transmit Next Pointer Register */
  #define REG_PWM_TNCR                      (0x4000011CU) /**< \brief (PWM) Transmit Next Counter Register */
  #define REG_PWM_PTCR                      (0x40000120U) /**< \brief (PWM) Transfer Control Register */
  #define REG_PWM_PTSR                      (0x40000124U) /**< \brief (PWM) Transfer Status Register */
  #define REG_PWM_CMPV0                     (0x40000130U) /**< \brief (PWM) PWM Comparison 0 Value Register */
  #define REG_PWM_CMPVUPD0                  (0x40000134U) /**< \brief (PWM) PWM Comparison 0 Value Update Register */
  #define REG_PWM_CMPM0                     (0x40000138U) /**< \brief (PWM) PWM Comparison 0 Mode Register */
  #define REG_PWM_CMPMUPD0                  (0x4000013CU) /**< \brief (PWM) PWM Comparison 0 Mode Update Register */
  #define REG_PWM_CMPV1                     (0x40000140U) /**< \brief (PWM) PWM Comparison 1 Value Register */
  #define REG_PWM_CMPVUPD1                  (0x40000144U) /**< \brief (PWM) PWM Comparison 1 Value Update Register */
  #define REG_PWM_CMPM1                     (0x40000148U) /**< \brief (PWM) PWM Comparison 1 Mode Register */
  #define REG_PWM_CMPMUPD1                  (0x4000014CU) /**< \brief (PWM) PWM Comparison 1 Mode Update Register */
  #define REG_PWM_CMPV2                     (0x40000150U) /**< \brief (PWM) PWM Comparison 2 Value Register */
  #define REG_PWM_CMPVUPD2                  (0x40000154U) /**< \brief (PWM) PWM Comparison 2 Value Update Register */
  #define REG_PWM_CMPM2                     (0x40000158U) /**< \brief (PWM) PWM Comparison 2 Mode Register */
  #define REG_PWM_CMPMUPD2                  (0x4000015CU) /**< \brief (PWM) PWM Comparison 2 Mode Update Register */
  #define REG_PWM_CMPV3                     (0x40000160U) /**< \brief (PWM) PWM Comparison 3 Value Register */
  #define REG_PWM_CMPVUPD3                  (0x40000164U) /**< \brief (PWM) PWM Comparison 3 Value Update Register */
  #define REG_PWM_CMPM3                     (0x40000168U) /**< \brief (PWM) PWM Comparison 3 Mode Register */
  #define REG_PWM_CMPMUPD3                  (0x4000016CU) /**< \brief (PWM) PWM Comparison 3 Mode Update Register */
  #define REG_PWM_CMPV4                     (0x40000170U) /**< \brief (PWM) PWM Comparison 4 Value Register */
  #define REG_PWM_CMPVUPD4                  (0x40000174U) /**< \brief (PWM) PWM Comparison 4 Value Update Register */
  #define REG_PWM_CMPM4                     (0x40000178U) /**< \brief (PWM) PWM Comparison 4 Mode Register */
  #define REG_PWM_CMPMUPD4                  (0x4000017CU) /**< \brief (PWM) PWM Comparison 4 Mode Update Register */
  #define REG_PWM_CMPV5                     (0x40000180U) /**< \brief (PWM) PWM Comparison 5 Value Register */
  #define REG_PWM_CMPVUPD5                  (0x40000184U) /**< \brief (PWM) PWM Comparison 5 Value Update Register */
  #define REG_PWM_CMPM5                     (0x40000188U) /**< \brief (PWM) PWM Comparison 5 Mode Register */
  #define REG_PWM_CMPMUPD5                  (0x4000018CU) /**< \brief (PWM) PWM Comparison 5 Mode Update Register */
  #define REG_PWM_CMPV6                     (0x40000190U) /**< \brief (PWM) PWM Comparison 6 Value Register */
  #define REG_PWM_CMPVUPD6                  (0x40000194U) /**< \brief (PWM) PWM Comparison 6 Value Update Register */
  #define REG_PWM_CMPM6                     (0x40000198U) /**< \brief (PWM) PWM Comparison 6 Mode Register */
  #define REG_PWM_CMPMUPD6                  (0x4000019CU) /**< \brief (PWM) PWM Comparison 6 Mode Update Register */
  #define REG_PWM_CMPV7                     (0x400001A0U) /**< \brief (PWM) PWM Comparison 7 Value Register */
  #define REG_PWM_CMPVUPD7                  (0x400001A4U) /**< \brief (PWM) PWM Comparison 7 Value Update Register */
  #define REG_PWM_CMPM7                     (0x400001A8U) /**< \brief (PWM) PWM Comparison 7 Mode Register */
  #define REG_PWM_CMPMUPD7                  (0x400001ACU) /**< \brief (PWM) PWM Comparison 7 Mode Update Register */
  #define REG_PWM_CMR0                      (0x40000200U) /**< \brief (PWM) PWM Channel Mode Register (ch_num = 0) */
  #define REG_PWM_CDTY0                     (0x40000204U) /**< \brief (PWM) PWM Channel Duty Cycle Register (ch_num = 0) */
  #define REG_PWM_CDTYUPD0                  (0x40000208U) /**< \brief (PWM) PWM Channel Duty Cycle Update Register (ch_num = 0) */
  #define REG_PWM_CPRD0                     (0x4000020CU) /**< \brief (PWM) PWM Channel Period Register (ch_num = 0) */
  #define REG_PWM_CPRDUPD0                  (0x40000210U) /**< \brief (PWM) PWM Channel Period Update Register (ch_num = 0) */
  #define REG_PWM_CCNT0                     (0x40000214U) /**< \brief (PWM) PWM Channel Counter Register (ch_num = 0) */
  #define REG_PWM_DT0                       (0x40000218U) /**< \brief (PWM) PWM Channel Dead Time Register (ch_num = 0) */
  #define REG_PWM_DTUPD0                    (0x4000021CU) /**< \brief (PWM) PWM Channel Dead Time Update Register (ch_num = 0) */
  #define REG_PWM_CMR1                      (0x40000220U) /**< \brief (PWM) PWM Channel Mode Register (ch_num = 1) */
  #define REG_PWM_CDTY1                     (0x40000224U) /**< \brief (PWM) PWM Channel Duty Cycle Register (ch_num = 1) */
  #define REG_PWM_CDTYUPD1                  (0x40000228U) /**< \brief (PWM) PWM Channel Duty Cycle Update Register (ch_num = 1) */
  #define REG_PWM_CPRD1                     (0x4000022CU) /**< \brief (PWM) PWM Channel Period Register (ch_num = 1) */
  #define REG_PWM_CPRDUPD1                  (0x40000230U) /**< \brief (PWM) PWM Channel Period Update Register (ch_num = 1) */
  #define REG_PWM_CCNT1                     (0x40000234U) /**< \brief (PWM) PWM Channel Counter Register (ch_num = 1) */
  #define REG_PWM_DT1                       (0x40000238U) /**< \brief (PWM) PWM Channel Dead Time Register (ch_num = 1) */
  #define REG_PWM_DTUPD1                    (0x4000023CU) /**< \brief (PWM) PWM Channel Dead Time Update Register (ch_num = 1) */
  #define REG_PWM_CMR2                      (0x40000240U) /**< \brief (PWM) PWM Channel Mode Register (ch_num = 2) */
  #define REG_PWM_CDTY2                     (0x40000244U) /**< \brief (PWM) PWM Channel Duty Cycle Register (ch_num = 2) */
  #define REG_PWM_CDTYUPD2                  (0x40000248U) /**< \brief (PWM) PWM Channel Duty Cycle Update Register (ch_num = 2) */
  #define REG_PWM_CPRD2                     (0x4000024CU) /**< \brief (PWM) PWM Channel Period Register (ch_num = 2) */
  #define REG_PWM_CPRDUPD2                  (0x40000250U) /**< \brief (PWM) PWM Channel Period Update Register (ch_num = 2) */
  #define REG_PWM_CCNT2                     (0x40000254U) /**< \brief (PWM) PWM Channel Counter Register (ch_num = 2) */
  #define REG_PWM_DT2                       (0x40000258U) /**< \brief (PWM) PWM Channel Dead Time Register (ch_num = 2) */
  #define REG_PWM_DTUPD2                    (0x4000025CU) /**< \brief (PWM) PWM Channel Dead Time Update Register (ch_num = 2) */
  #define REG_PWM_CMR3                      (0x40000260U) /**< \brief (PWM) PWM Channel Mode Register (ch_num = 3) */
  #define REG_PWM_CDTY3                     (0x40000264U) /**< \brief (PWM) PWM Channel Duty Cycle Register (ch_num = 3) */
  #define REG_PWM_CDTYUPD3                  (0x40000268U) /**< \brief (PWM) PWM Channel Duty Cycle Update Register (ch_num = 3) */
  #define REG_PWM_CPRD3                     (0x4000026CU) /**< \brief (PWM) PWM Channel Period Register (ch_num = 3) */
  #define REG_PWM_CPRDUPD3                  (0x40000270U) /**< \brief (PWM) PWM Channel Period Update Register (ch_num = 3) */
  #define REG_PWM_CCNT3                     (0x40000274U) /**< \brief (PWM) PWM Channel Counter Register (ch_num = 3) */
  #define REG_PWM_DT3                       (0x40000278U) /**< \brief (PWM) PWM Channel Dead Time Register (ch_num = 3) */
  #define REG_PWM_DTUPD3                    (0x4000027CU) /**< \brief (PWM) PWM Channel Dead Time Update Register (ch_num = 3) */
#else
  #define REG_PWM_CLK      (*(__IO uint32_t*)0x40000000U) /**< \brief (PWM) PWM Clock Register */
  #define REG_PWM_ENA      (*(__O  uint32_t*)0x40000004U) /**< \brief (PWM) PWM Enable Register */
  #define REG_PWM_DIS      (*(__O  uint32_t*)0x40000008U) /**< \brief (PWM) PWM Disable Register */
  #define REG_PWM_SR       (*(__I  uint32_t*)0x4000000CU) /**< \brief (PWM) PWM Status Register */
  #define REG_PWM_IER1     (*(__O  uint32_t*)0x40000010U) /**< \brief (PWM) PWM Interrupt Enable Register 1 */
  #define REG_PWM_IDR1     (*(__O  uint32_t*)0x40000014U) /**< \brief (PWM) PWM Interrupt Disable Register 1 */
  #define REG_PWM_IMR1     (*(__I  uint32_t*)0x40000018U) /**< \brief (PWM) PWM Interrupt Mask Register 1 */
  #define REG_PWM_ISR1     (*(__I  uint32_t*)0x4000001CU) /**< \brief (PWM) PWM Interrupt Status Register 1 */
  #define REG_PWM_SCM      (*(__IO uint32_t*)0x40000020U) /**< \brief (PWM) PWM Sync Channels Mode Register */
  #define REG_PWM_DMAR     (*(__O  uint32_t*)0x40000024U) /**< \brief (PWM) PWM DMA Register */
  #define REG_PWM_SCUC     (*(__IO uint32_t*)0x40000028U) /**< \brief (PWM) PWM Sync Channels Update Control Register */
  #define REG_PWM_SCUP     (*(__IO uint32_t*)0x4000002CU) /**< \brief (PWM) PWM Sync Channels Update Period Register */
  #define REG_PWM_SCUPUPD  (*(__O  uint32_t*)0x40000030U) /**< \brief (PWM) PWM Sync Channels Update Period Update Register */
  #define REG_PWM_IER2     (*(__O  uint32_t*)0x40000034U) /**< \brief (PWM) PWM Interrupt Enable Register 2 */
  #define REG_PWM_IDR2     (*(__O  uint32_t*)0x40000038U) /**< \brief (PWM) PWM Interrupt Disable Register 2 */
  #define REG_PWM_IMR2     (*(__I  uint32_t*)0x4000003CU) /**< \brief (PWM) PWM Interrupt Mask Register 2 */
  #define REG_PWM_ISR2     (*(__I  uint32_t*)0x40000040U) /**< \brief (PWM) PWM Interrupt Status Register 2 */
  #define REG_PWM_OOV      (*(__IO uint32_t*)0x40000044U) /**< \brief (PWM) PWM Output Override Value Register */
  #define REG_PWM_OS       (*(__IO uint32_t*)0x40000048U) /**< \brief (PWM) PWM Output Selection Register */
  #define REG_PWM_OSS      (*(__O  uint32_t*)0x4000004CU) /**< \brief (PWM) PWM Output Selection Set Register */
  #define REG_PWM_OSC      (*(__O  uint32_t*)0x40000050U) /**< \brief (PWM) PWM Output Selection Clear Register */
  #define REG_PWM_OSSUPD   (*(__O  uint32_t*)0x40000054U) /**< \brief (PWM) PWM Output Selection Set Update Register */
  #define REG_PWM_OSCUPD   (*(__O  uint32_t*)0x40000058U) /**< \brief (PWM) PWM Output Selection Clear Update Register */
  #define REG_PWM_FMR      (*(__IO uint32_t*)0x4000005CU) /**< \brief (PWM) PWM Fault Mode Register */
  #define REG_PWM_FSR      (*(__I  uint32_t*)0x40000060U) /**< \brief (PWM) PWM Fault Status Register */
  #define REG_PWM_FCR      (*(__O  uint32_t*)0x40000064U) /**< \brief (PWM) PWM Fault Clear Register */
#define REG_PWM_FPV1     (*(RwReg*)0x40000068U) /**< \brief (PWM) PWM Fault Protection Value Register 1 */
  #define REG_PWM_FPE      (*(__IO uint32_t*)0x4000006CU) /**< \brief (PWM) PWM Fault Protection Enable Register */
  #define REG_PWM_ELMR     (*(__IO uint32_t*)0x4000007CU) /**< \brief (PWM) PWM Event Line 0 Mode Register */
  #define REG_PWM_SSPR     (*(__IO uint32_t*)0x400000A0U) /**< \brief (PWM) PWM Spread Spectrum Register */
  #define REG_PWM_SSPUP    (*(__O  uint32_t*)0x400000A4U) /**< \brief (PWM) PWM Spread Spectrum Update Register */
  #define REG_PWM_SMMR     (*(__IO uint32_t*)0x400000B0U) /**< \brief (PWM) PWM Stepper Motor Mode Register */
  #define REG_PWM_FPV2     (*(__IO uint32_t*)0x400000C0U) /**< \brief (PWM) PWM Fault Protection Value 2 Register */
  #define REG_PWM_WPCR     (*(__O  uint32_t*)0x400000E4U) /**< \brief (PWM) PWM Write Protection Control Register */
  #define REG_PWM_WPSR     (*(__I  uint32_t*)0x400000E8U) /**< \brief (PWM) PWM Write Protection Status Register */
  #define REG_PWM_TPR      (*(__IO uint32_t*)0x40000108U) /**< \brief (PWM) Transmit Pointer Register */
  #define REG_PWM_TCR      (*(__IO uint32_t*)0x4000010CU) /**< \brief (PWM) Transmit Counter Register */
  #define REG_PWM_TNPR     (*(__IO uint32_t*)0x40000118U) /**< \brief (PWM) Transmit Next Pointer Register */
  #define REG_PWM_TNCR     (*(__IO uint32_t*)0x4000011CU) /**< \brief (PWM) Transmit Next Counter Register */
  #define REG_PWM_PTCR     (*(__O  uint32_t*)0x40000120U) /**< \brief (PWM) Transfer Control Register */
  #define REG_PWM_PTSR     (*(__I  uint32_t*)0x40000124U) /**< \brief (PWM) Transfer Status Register */
  #define REG_PWM_CMPV0    (*(__IO uint32_t*)0x40000130U) /**< \brief (PWM) PWM Comparison 0 Value Register */
  #define REG_PWM_CMPVUPD0 (*(__O  uint32_t*)0x40000134U) /**< \brief (PWM) PWM Comparison 0 Value Update Register */
  #define REG_PWM_CMPM0    (*(__IO uint32_t*)0x40000138U) /**< \brief (PWM) PWM Comparison 0 Mode Register */
  #define REG_PWM_CMPMUPD0 (*(__O  uint32_t*)0x4000013CU) /**< \brief (PWM) PWM Comparison 0 Mode Update Register */
  #define REG_PWM_CMPV1    (*(__IO uint32_t*)0x40000140U) /**< \brief (PWM) PWM Comparison 1 Value Register */
  #define REG_PWM_CMPVUPD1 (*(__O  uint32_t*)0x40000144U) /**< \brief (PWM) PWM Comparison 1 Value Update Register */
  #define REG_PWM_CMPM1    (*(__IO uint32_t*)0x40000148U) /**< \brief (PWM) PWM Comparison 1 Mode Register */
  #define REG_PWM_CMPMUPD1 (*(__O  uint32_t*)0x4000014CU) /**< \brief (PWM) PWM Comparison 1 Mode Update Register */
  #define REG_PWM_CMPV2    (*(__IO uint32_t*)0x40000150U) /**< \brief (PWM) PWM Comparison 2 Value Register */
  #define REG_PWM_CMPVUPD2 (*(__O  uint32_t*)0x40000154U) /**< \brief (PWM) PWM Comparison 2 Value Update Register */
  #define REG_PWM_CMPM2    (*(__IO uint32_t*)0x40000158U) /**< \brief (PWM) PWM Comparison 2 Mode Register */
  #define REG_PWM_CMPMUPD2 (*(__O  uint32_t*)0x4000015CU) /**< \brief (PWM) PWM Comparison 2 Mode Update Register */
  #define REG_PWM_CMPV3    (*(__IO uint32_t*)0x40000160U) /**< \brief (PWM) PWM Comparison 3 Value Register */
  #define REG_PWM_CMPVUPD3 (*(__O  uint32_t*)0x40000164U) /**< \brief (PWM) PWM Comparison 3 Value Update Register */
  #define REG_PWM_CMPM3    (*(__IO uint32_t*)0x40000168U) /**< \brief (PWM) PWM Comparison 3 Mode Register */
  #define REG_PWM_CMPMUPD3 (*(__O  uint32_t*)0x4000016CU) /**< \brief (PWM) PWM Comparison 3 Mode Update Register */
  #define REG_PWM_CMPV4    (*(__IO uint32_t*)0x40000170U) /**< \brief (PWM) PWM Comparison 4 Value Register */
  #define REG_PWM_CMPVUPD4 (*(__O  uint32_t*)0x40000174U) /**< \brief (PWM) PWM Comparison 4 Value Update Register */
  #define REG_PWM_CMPM4    (*(__IO uint32_t*)0x40000178U) /**< \brief (PWM) PWM Comparison 4 Mode Register */
  #define REG_PWM_CMPMUPD4 (*(__O  uint32_t*)0x4000017CU) /**< \brief (PWM) PWM Comparison 4 Mode Update Register */
  #define REG_PWM_CMPV5    (*(__IO uint32_t*)0x40000180U) /**< \brief (PWM) PWM Comparison 5 Value Register */
  #define REG_PWM_CMPVUPD5 (*(__O  uint32_t*)0x40000184U) /**< \brief (PWM) PWM Comparison 5 Value Update Register */
  #define REG_PWM_CMPM5    (*(__IO uint32_t*)0x40000188U) /**< \brief (PWM) PWM Comparison 5 Mode Register */
  #define REG_PWM_CMPMUPD5 (*(__O  uint32_t*)0x4000018CU) /**< \brief (PWM) PWM Comparison 5 Mode Update Register */
  #define REG_PWM_CMPV6    (*(__IO uint32_t*)0x40000190U) /**< \brief (PWM) PWM Comparison 6 Value Register */
  #define REG_PWM_CMPVUPD6 (*(__O  uint32_t*)0x40000194U) /**< \brief (PWM) PWM Comparison 6 Value Update Register */
  #define REG_PWM_CMPM6    (*(__IO uint32_t*)0x40000198U) /**< \brief (PWM) PWM Comparison 6 Mode Register */
  #define REG_PWM_CMPMUPD6 (*(__O  uint32_t*)0x4000019CU) /**< \brief (PWM) PWM Comparison 6 Mode Update Register */
  #define REG_PWM_CMPV7    (*(__IO uint32_t*)0x400001A0U) /**< \brief (PWM) PWM Comparison 7 Value Register */
  #define REG_PWM_CMPVUPD7 (*(__O  uint32_t*)0x400001A4U) /**< \brief (PWM) PWM Comparison 7 Value Update Register */
  #define REG_PWM_CMPM7    (*(__IO uint32_t*)0x400001A8U) /**< \brief (PWM) PWM Comparison 7 Mode Register */
  #define REG_PWM_CMPMUPD7 (*(__O  uint32_t*)0x400001ACU) /**< \brief (PWM) PWM Comparison 7 Mode Update Register */
  #define REG_PWM_CMR0     (*(__IO uint32_t*)0x40000200U) /**< \brief (PWM) PWM Channel Mode Register (ch_num = 0) */
  #define REG_PWM_CDTY0    (*(__IO uint32_t*)0x40000204U) /**< \brief (PWM) PWM Channel Duty Cycle Register (ch_num = 0) */
  #define REG_PWM_CDTYUPD0 (*(__O  uint32_t*)0x40000208U) /**< \brief (PWM) PWM Channel Duty Cycle Update Register (ch_num = 0) */
  #define REG_PWM_CPRD0    (*(__IO uint32_t*)0x4000020CU) /**< \brief (PWM) PWM Channel Period Register (ch_num = 0) */
  #define REG_PWM_CPRDUPD0 (*(__O  uint32_t*)0x40000210U) /**< \brief (PWM) PWM Channel Period Update Register (ch_num = 0) */
  #define REG_PWM_CCNT0    (*(__I  uint32_t*)0x40000214U) /**< \brief (PWM) PWM Channel Counter Register (ch_num = 0) */
  #define REG_PWM_DT0      (*(__IO uint32_t*)0x40000218U) /**< \brief (PWM) PWM Channel Dead Time Register (ch_num = 0) */
  #define REG_PWM_DTUPD0   (*(__O  uint32_t*)0x4000021CU) /**< \brief (PWM) PWM Channel Dead Time Update Register (ch_num = 0) */
  #define REG_PWM_CMR1     (*(__IO uint32_t*)0x40000220U) /**< \brief (PWM) PWM Channel Mode Register (ch_num = 1) */
  #define REG_PWM_CDTY1    (*(__IO uint32_t*)0x40000224U) /**< \brief (PWM) PWM Channel Duty Cycle Register (ch_num = 1) */
  #define REG_PWM_CDTYUPD1 (*(__O  uint32_t*)0x40000228U) /**< \brief (PWM) PWM Channel Duty Cycle Update Register (ch_num = 1) */
  #define REG_PWM_CPRD1    (*(__IO uint32_t*)0x4000022CU) /**< \brief (PWM) PWM Channel Period Register (ch_num = 1) */
  #define REG_PWM_CPRDUPD1 (*(__O  uint32_t*)0x40000230U) /**< \brief (PWM) PWM Channel Period Update Register (ch_num = 1) */
  #define REG_PWM_CCNT1    (*(__I  uint32_t*)0x40000234U) /**< \brief (PWM) PWM Channel Counter Register (ch_num = 1) */
  #define REG_PWM_DT1      (*(__IO uint32_t*)0x40000238U) /**< \brief (PWM) PWM Channel Dead Time Register (ch_num = 1) */
  #define REG_PWM_DTUPD1   (*(__O  uint32_t*)0x4000023CU) /**< \brief (PWM) PWM Channel Dead Time Update Register (ch_num = 1) */
  #define REG_PWM_CMR2     (*(__IO uint32_t*)0x40000240U) /**< \brief (PWM) PWM Channel Mode Register (ch_num = 2) */
  #define REG_PWM_CDTY2    (*(__IO uint32_t*)0x40000244U) /**< \brief (PWM) PWM Channel Duty Cycle Register (ch_num = 2) */
  #define REG_PWM_CDTYUPD2 (*(__O  uint32_t*)0x40000248U) /**< \brief (PWM) PWM Channel Duty Cycle Update Register (ch_num = 2) */
  #define REG_PWM_CPRD2    (*(__IO uint32_t*)0x4000024CU) /**< \brief (PWM) PWM Channel Period Register (ch_num = 2) */
  #define REG_PWM_CPRDUPD2 (*(__O  uint32_t*)0x40000250U) /**< \brief (PWM) PWM Channel Period Update Register (ch_num = 2) */
  #define REG_PWM_CCNT2    (*(__I  uint32_t*)0x40000254U) /**< \brief (PWM) PWM Channel Counter Register (ch_num = 2) */
  #define REG_PWM_DT2      (*(__IO uint32_t*)0x40000258U) /**< \brief (PWM) PWM Channel Dead Time Register (ch_num = 2) */
  #define REG_PWM_DTUPD2   (*(__O  uint32_t*)0x4000025CU) /**< \brief (PWM) PWM Channel Dead Time Update Register (ch_num = 2) */
  #define REG_PWM_CMR3     (*(__IO uint32_t*)0x40000260U) /**< \brief (PWM) PWM Channel Mode Register (ch_num = 3) */
  #define REG_PWM_CDTY3    (*(__IO uint32_t*)0x40000264U) /**< \brief (PWM) PWM Channel Duty Cycle Register (ch_num = 3) */
  #define REG_PWM_CDTYUPD3 (*(__O  uint32_t*)0x40000268U) /**< \brief (PWM) PWM Channel Duty Cycle Update Register (ch_num = 3) */
  #define REG_PWM_CPRD3    (*(__IO uint32_t*)0x4000026CU) /**< \brief (PWM) PWM Channel Period Register (ch_num = 3) */
  #define REG_PWM_CPRDUPD3 (*(__O  uint32_t*)0x40000270U) /**< \brief (PWM) PWM Channel Period Update Register (ch_num = 3) */
  #define REG_PWM_CCNT3    (*(__I  uint32_t*)0x40000274U) /**< \brief (PWM) PWM Channel Counter Register (ch_num = 3) */
  #define REG_PWM_DT3      (*(__IO uint32_t*)0x40000278U) /**< \brief (PWM) PWM Channel Dead Time Register (ch_num = 3) */
  #define REG_PWM_DTUPD3   (*(__O  uint32_t*)0x4000027CU) /**< \brief (PWM) PWM Channel Dead Time Update Register (ch_num = 3) */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAM4E_PWM_INSTANCE_ */
