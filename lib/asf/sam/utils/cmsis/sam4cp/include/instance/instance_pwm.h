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

#ifndef _SAM4CP_PWM_INSTANCE_
#define _SAM4CP_PWM_INSTANCE_

/* ========== Register definition for PWM peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_PWM_MR                     (0x48008000U) /**< \brief (PWM) PWM Mode Register */
  #define REG_PWM_ENA                    (0x48008004U) /**< \brief (PWM) PWM Enable Register */
  #define REG_PWM_DIS                    (0x48008008U) /**< \brief (PWM) PWM Disable Register */
  #define REG_PWM_SR                     (0x4800800CU) /**< \brief (PWM) PWM Status Register */
  #define REG_PWM_IER                    (0x48008010U) /**< \brief (PWM) PWM Interrupt Enable Register */
  #define REG_PWM_IDR                    (0x48008014U) /**< \brief (PWM) PWM Interrupt Disable Register */
  #define REG_PWM_IMR                    (0x48008018U) /**< \brief (PWM) PWM Interrupt Mask Register */
  #define REG_PWM_ISR                    (0x4800801CU) /**< \brief (PWM) PWM Interrupt Status Register */
  #define REG_PWM_CMR0                   (0x48008200U) /**< \brief (PWM) PWM Channel Mode Register (ch_num = 0) */
  #define REG_PWM_CDTY0                  (0x48008204U) /**< \brief (PWM) PWM Channel Duty Cycle Register (ch_num = 0) */
  #define REG_PWM_CPRD0                  (0x48008208U) /**< \brief (PWM) PWM Channel Period Register (ch_num = 0) */
  #define REG_PWM_CCNT0                  (0x4800820CU) /**< \brief (PWM) PWM Channel Counter Register (ch_num = 0) */
  #define REG_PWM_CUPD0                  (0x48008210U) /**< \brief (PWM) PWM Channel Update Register (ch_num = 0) */
  #define REG_PWM_CMR1                   (0x48008220U) /**< \brief (PWM) PWM Channel Mode Register (ch_num = 1) */
  #define REG_PWM_CDTY1                  (0x48008224U) /**< \brief (PWM) PWM Channel Duty Cycle Register (ch_num = 1) */
  #define REG_PWM_CPRD1                  (0x48008228U) /**< \brief (PWM) PWM Channel Period Register (ch_num = 1) */
  #define REG_PWM_CCNT1                  (0x4800822CU) /**< \brief (PWM) PWM Channel Counter Register (ch_num = 1) */
  #define REG_PWM_CUPD1                  (0x48008230U) /**< \brief (PWM) PWM Channel Update Register (ch_num = 1) */
  #define REG_PWM_CMR2                   (0x48008240U) /**< \brief (PWM) PWM Channel Mode Register (ch_num = 2) */
  #define REG_PWM_CDTY2                  (0x48008244U) /**< \brief (PWM) PWM Channel Duty Cycle Register (ch_num = 2) */
  #define REG_PWM_CPRD2                  (0x48008248U) /**< \brief (PWM) PWM Channel Period Register (ch_num = 2) */
  #define REG_PWM_CCNT2                  (0x4800824CU) /**< \brief (PWM) PWM Channel Counter Register (ch_num = 2) */
  #define REG_PWM_CUPD2                  (0x48008250U) /**< \brief (PWM) PWM Channel Update Register (ch_num = 2) */
  #define REG_PWM_CMR3                   (0x48008260U) /**< \brief (PWM) PWM Channel Mode Register (ch_num = 3) */
  #define REG_PWM_CDTY3                  (0x48008264U) /**< \brief (PWM) PWM Channel Duty Cycle Register (ch_num = 3) */
  #define REG_PWM_CPRD3                  (0x48008268U) /**< \brief (PWM) PWM Channel Period Register (ch_num = 3) */
  #define REG_PWM_CCNT3                  (0x4800826CU) /**< \brief (PWM) PWM Channel Counter Register (ch_num = 3) */
  #define REG_PWM_CUPD3                  (0x48008270U) /**< \brief (PWM) PWM Channel Update Register (ch_num = 3) */
#else
  #define REG_PWM_MR    (*(__IO uint32_t*)0x48008000U) /**< \brief (PWM) PWM Mode Register */
  #define REG_PWM_ENA   (*(__O  uint32_t*)0x48008004U) /**< \brief (PWM) PWM Enable Register */
  #define REG_PWM_DIS   (*(__O  uint32_t*)0x48008008U) /**< \brief (PWM) PWM Disable Register */
  #define REG_PWM_SR    (*(__I  uint32_t*)0x4800800CU) /**< \brief (PWM) PWM Status Register */
  #define REG_PWM_IER   (*(__O  uint32_t*)0x48008010U) /**< \brief (PWM) PWM Interrupt Enable Register */
  #define REG_PWM_IDR   (*(__O  uint32_t*)0x48008014U) /**< \brief (PWM) PWM Interrupt Disable Register */
  #define REG_PWM_IMR   (*(__I  uint32_t*)0x48008018U) /**< \brief (PWM) PWM Interrupt Mask Register */
  #define REG_PWM_ISR   (*(__I  uint32_t*)0x4800801CU) /**< \brief (PWM) PWM Interrupt Status Register */
  #define REG_PWM_CMR0  (*(__IO uint32_t*)0x48008200U) /**< \brief (PWM) PWM Channel Mode Register (ch_num = 0) */
  #define REG_PWM_CDTY0 (*(__IO uint32_t*)0x48008204U) /**< \brief (PWM) PWM Channel Duty Cycle Register (ch_num = 0) */
  #define REG_PWM_CPRD0 (*(__IO uint32_t*)0x48008208U) /**< \brief (PWM) PWM Channel Period Register (ch_num = 0) */
  #define REG_PWM_CCNT0 (*(__I  uint32_t*)0x4800820CU) /**< \brief (PWM) PWM Channel Counter Register (ch_num = 0) */
  #define REG_PWM_CUPD0 (*(__O  uint32_t*)0x48008210U) /**< \brief (PWM) PWM Channel Update Register (ch_num = 0) */
  #define REG_PWM_CMR1  (*(__IO uint32_t*)0x48008220U) /**< \brief (PWM) PWM Channel Mode Register (ch_num = 1) */
  #define REG_PWM_CDTY1 (*(__IO uint32_t*)0x48008224U) /**< \brief (PWM) PWM Channel Duty Cycle Register (ch_num = 1) */
  #define REG_PWM_CPRD1 (*(__IO uint32_t*)0x48008228U) /**< \brief (PWM) PWM Channel Period Register (ch_num = 1) */
  #define REG_PWM_CCNT1 (*(__I  uint32_t*)0x4800822CU) /**< \brief (PWM) PWM Channel Counter Register (ch_num = 1) */
  #define REG_PWM_CUPD1 (*(__O  uint32_t*)0x48008230U) /**< \brief (PWM) PWM Channel Update Register (ch_num = 1) */
  #define REG_PWM_CMR2  (*(__IO uint32_t*)0x48008240U) /**< \brief (PWM) PWM Channel Mode Register (ch_num = 2) */
  #define REG_PWM_CDTY2 (*(__IO uint32_t*)0x48008244U) /**< \brief (PWM) PWM Channel Duty Cycle Register (ch_num = 2) */
  #define REG_PWM_CPRD2 (*(__IO uint32_t*)0x48008248U) /**< \brief (PWM) PWM Channel Period Register (ch_num = 2) */
  #define REG_PWM_CCNT2 (*(__I  uint32_t*)0x4800824CU) /**< \brief (PWM) PWM Channel Counter Register (ch_num = 2) */
  #define REG_PWM_CUPD2 (*(__O  uint32_t*)0x48008250U) /**< \brief (PWM) PWM Channel Update Register (ch_num = 2) */
  #define REG_PWM_CMR3  (*(__IO uint32_t*)0x48008260U) /**< \brief (PWM) PWM Channel Mode Register (ch_num = 3) */
  #define REG_PWM_CDTY3 (*(__IO uint32_t*)0x48008264U) /**< \brief (PWM) PWM Channel Duty Cycle Register (ch_num = 3) */
  #define REG_PWM_CPRD3 (*(__IO uint32_t*)0x48008268U) /**< \brief (PWM) PWM Channel Period Register (ch_num = 3) */
  #define REG_PWM_CCNT3 (*(__I  uint32_t*)0x4800826CU) /**< \brief (PWM) PWM Channel Counter Register (ch_num = 3) */
  #define REG_PWM_CUPD3 (*(__O  uint32_t*)0x48008270U) /**< \brief (PWM) PWM Channel Update Register (ch_num = 3) */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAM4CP_PWM_INSTANCE_ */
