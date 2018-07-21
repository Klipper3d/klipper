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

#ifndef _SAM4CM_SLCDC_INSTANCE_
#define _SAM4CM_SLCDC_INSTANCE_

/* ========== Register definition for SLCDC peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_SLCDC_CR                      (0x4003C000U) /**< \brief (SLCDC) SLCDC Control Register */
  #define REG_SLCDC_MR                      (0x4003C004U) /**< \brief (SLCDC) SLCDC Mode Register */
  #define REG_SLCDC_FRR                     (0x4003C008U) /**< \brief (SLCDC) SLCDC Frame Rate Register */
  #define REG_SLCDC_DR                      (0x4003C00CU) /**< \brief (SLCDC) SLCDC Display Register */
  #define REG_SLCDC_SR                      (0x4003C010U) /**< \brief (SLCDC) SLCDC Status Register */
  #define REG_SLCDC_IER                     (0x4003C020U) /**< \brief (SLCDC) SLCDC Interrupt Enable Register */
  #define REG_SLCDC_IDR                     (0x4003C024U) /**< \brief (SLCDC) SLCDC Interrupt Disable Register */
  #define REG_SLCDC_IMR                     (0x4003C028U) /**< \brief (SLCDC) SLCDC Interrupt Mask Register */
  #define REG_SLCDC_ISR                     (0x4003C02CU) /**< \brief (SLCDC) SLCDC Interrupt Status Register */
  #define REG_SLCDC_SMR0                    (0x4003C030U) /**< \brief (SLCDC) SLCDC Segment Map Register 0 */
  #define REG_SLCDC_SMR1                    (0x4003C034U) /**< \brief (SLCDC) SLCDC Segment Map Register 1 */
  #define REG_SLCDC_WPMR                    (0x4003C0E4U) /**< \brief (SLCDC) SLCDC Write Protect Mode Register */
  #define REG_SLCDC_WPSR                    (0x4003C0E8U) /**< \brief (SLCDC) SLCDC Write Protect Status Register */
  #define REG_SLCDC_LMEMR0                  (0x4003C200U) /**< \brief (SLCDC) SLCDC LSB Memory Register (com = 0) */
  #define REG_SLCDC_MMEMR0                  (0x4003C204U) /**< \brief (SLCDC) SLCDC MSB Memory Register (com = 0) */
  #define REG_SLCDC_LMEMR1                  (0x4003C208U) /**< \brief (SLCDC) SLCDC LSB Memory Register (com = 1) */
  #define REG_SLCDC_MMEMR1                  (0x4003C20CU) /**< \brief (SLCDC) SLCDC MSB Memory Register (com = 1) */
  #define REG_SLCDC_LMEMR2                  (0x4003C210U) /**< \brief (SLCDC) SLCDC LSB Memory Register (com = 2) */
  #define REG_SLCDC_MMEMR2                  (0x4003C214U) /**< \brief (SLCDC) SLCDC MSB Memory Register (com = 2) */
  #define REG_SLCDC_LMEMR3                  (0x4003C218U) /**< \brief (SLCDC) SLCDC LSB Memory Register (com = 3) */
  #define REG_SLCDC_MMEMR3                  (0x4003C21CU) /**< \brief (SLCDC) SLCDC MSB Memory Register (com = 3) */
  #define REG_SLCDC_LMEMR4                  (0x4003C220U) /**< \brief (SLCDC) SLCDC LSB Memory Register (com = 4) */
  #define REG_SLCDC_MMEMR4                  (0x4003C224U) /**< \brief (SLCDC) SLCDC MSB Memory Register (com = 4) */
  #define REG_SLCDC_LMEMR5                  (0x4003C228U) /**< \brief (SLCDC) SLCDC LSB Memory Register (com = 5) */
  #define REG_SLCDC_MMEMR5                  (0x4003C22CU) /**< \brief (SLCDC) SLCDC MSB Memory Register (com = 5) */
#else
  #define REG_SLCDC_CR     (*(__O  uint32_t*)0x4003C000U) /**< \brief (SLCDC) SLCDC Control Register */
  #define REG_SLCDC_MR     (*(__IO uint32_t*)0x4003C004U) /**< \brief (SLCDC) SLCDC Mode Register */
  #define REG_SLCDC_FRR    (*(__IO uint32_t*)0x4003C008U) /**< \brief (SLCDC) SLCDC Frame Rate Register */
  #define REG_SLCDC_DR     (*(__IO uint32_t*)0x4003C00CU) /**< \brief (SLCDC) SLCDC Display Register */
  #define REG_SLCDC_SR     (*(__I  uint32_t*)0x4003C010U) /**< \brief (SLCDC) SLCDC Status Register */
  #define REG_SLCDC_IER    (*(__O  uint32_t*)0x4003C020U) /**< \brief (SLCDC) SLCDC Interrupt Enable Register */
  #define REG_SLCDC_IDR    (*(__O  uint32_t*)0x4003C024U) /**< \brief (SLCDC) SLCDC Interrupt Disable Register */
  #define REG_SLCDC_IMR    (*(__O  uint32_t*)0x4003C028U) /**< \brief (SLCDC) SLCDC Interrupt Mask Register */
  #define REG_SLCDC_ISR    (*(__I  uint32_t*)0x4003C02CU) /**< \brief (SLCDC) SLCDC Interrupt Status Register */
  #define REG_SLCDC_SMR0   (*(__IO uint32_t*)0x4003C030U) /**< \brief (SLCDC) SLCDC Segment Map Register 0 */
  #define REG_SLCDC_SMR1   (*(__IO uint32_t*)0x4003C034U) /**< \brief (SLCDC) SLCDC Segment Map Register 1 */
  #define REG_SLCDC_WPMR   (*(__IO uint32_t*)0x4003C0E4U) /**< \brief (SLCDC) SLCDC Write Protect Mode Register */
  #define REG_SLCDC_WPSR   (*(__I  uint32_t*)0x4003C0E8U) /**< \brief (SLCDC) SLCDC Write Protect Status Register */
  #define REG_SLCDC_LMEMR0 (*(__IO uint32_t*)0x4003C200U) /**< \brief (SLCDC) SLCDC LSB Memory Register (com = 0) */
  #define REG_SLCDC_MMEMR0 (*(__IO uint32_t*)0x4003C204U) /**< \brief (SLCDC) SLCDC MSB Memory Register (com = 0) */
  #define REG_SLCDC_LMEMR1 (*(__IO uint32_t*)0x4003C208U) /**< \brief (SLCDC) SLCDC LSB Memory Register (com = 1) */
  #define REG_SLCDC_MMEMR1 (*(__IO uint32_t*)0x4003C20CU) /**< \brief (SLCDC) SLCDC MSB Memory Register (com = 1) */
  #define REG_SLCDC_LMEMR2 (*(__IO uint32_t*)0x4003C210U) /**< \brief (SLCDC) SLCDC LSB Memory Register (com = 2) */
  #define REG_SLCDC_MMEMR2 (*(__IO uint32_t*)0x4003C214U) /**< \brief (SLCDC) SLCDC MSB Memory Register (com = 2) */
  #define REG_SLCDC_LMEMR3 (*(__IO uint32_t*)0x4003C218U) /**< \brief (SLCDC) SLCDC LSB Memory Register (com = 3) */
  #define REG_SLCDC_MMEMR3 (*(__IO uint32_t*)0x4003C21CU) /**< \brief (SLCDC) SLCDC MSB Memory Register (com = 3) */
  #define REG_SLCDC_LMEMR4 (*(__IO uint32_t*)0x4003C220U) /**< \brief (SLCDC) SLCDC LSB Memory Register (com = 4) */
  #define REG_SLCDC_MMEMR4 (*(__IO uint32_t*)0x4003C224U) /**< \brief (SLCDC) SLCDC MSB Memory Register (com = 4) */
  #define REG_SLCDC_LMEMR5 (*(__IO uint32_t*)0x4003C228U) /**< \brief (SLCDC) SLCDC LSB Memory Register (com = 5) */
  #define REG_SLCDC_MMEMR5 (*(__IO uint32_t*)0x4003C22CU) /**< \brief (SLCDC) SLCDC MSB Memory Register (com = 5) */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAM4CM_SLCDC_INSTANCE_ */
