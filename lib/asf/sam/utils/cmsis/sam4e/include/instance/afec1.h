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

#ifndef _SAM4E_AFEC1_INSTANCE_
#define _SAM4E_AFEC1_INSTANCE_

/* ========== Register definition for AFEC1 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_AFEC1_CR                       (0x400B4000U) /**< \brief (AFEC1) AFEC Control Register */
  #define REG_AFEC1_MR                       (0x400B4004U) /**< \brief (AFEC1) AFEC Mode Register */
  #define REG_AFEC1_EMR                      (0x400B4008U) /**< \brief (AFEC1) AFEC Extended Mode Register */
  #define REG_AFEC1_SEQ1R                    (0x400B400CU) /**< \brief (AFEC1) AFEC Channel Sequence 1 Register */
  #define REG_AFEC1_SEQ2R                    (0x400B4010U) /**< \brief (AFEC1) AFEC Channel Sequence 2 Register */
  #define REG_AFEC1_CHER                     (0x400B4014U) /**< \brief (AFEC1) AFEC Channel Enable Register */
  #define REG_AFEC1_CHDR                     (0x400B4018U) /**< \brief (AFEC1) AFEC Channel Disable Register */
  #define REG_AFEC1_CHSR                     (0x400B401CU) /**< \brief (AFEC1) AFEC Channel Status Register */
  #define REG_AFEC1_LCDR                     (0x400B4020U) /**< \brief (AFEC1) AFEC Last Converted Data Register */
  #define REG_AFEC1_IER                      (0x400B4024U) /**< \brief (AFEC1) AFEC Interrupt Enable Register */
  #define REG_AFEC1_IDR                      (0x400B4028U) /**< \brief (AFEC1) AFEC Interrupt Disable Register */
  #define REG_AFEC1_IMR                      (0x400B402CU) /**< \brief (AFEC1) AFEC Interrupt Mask Register */
  #define REG_AFEC1_ISR                      (0x400B4030U) /**< \brief (AFEC1) AFEC Interrupt Status Register */
  #define REG_AFEC1_OVER                     (0x400B404CU) /**< \brief (AFEC1) AFEC Overrun Status Register */
  #define REG_AFEC1_CWR                      (0x400B4050U) /**< \brief (AFEC1) AFEC Compare Window Register */
  #define REG_AFEC1_CGR                      (0x400B4054U) /**< \brief (AFEC1) AFEC Channel Gain Register */
  #define REG_AFEC1_CDOR                     (0x400B405CU) /**< \brief (AFEC1) AFEC Channel Calibration DC Offset Register */
  #define REG_AFEC1_DIFFR                    (0x400B4060U) /**< \brief (AFEC1) AFEC Channel Differential Register */
  #define REG_AFEC1_CSELR                    (0x400B4064U) /**< \brief (AFEC1) AFEC Channel Register Selection */
  #define REG_AFEC1_CDR                      (0x400B4068U) /**< \brief (AFEC1) AFEC Channel Data Register */
  #define REG_AFEC1_COCR                     (0x400B406CU) /**< \brief (AFEC1) AFEC Channel Offset Compensation Register */
  #define REG_AFEC1_TEMPMR                   (0x400B4070U) /**< \brief (AFEC1) AFEC Temperature Sensor Mode Register */
  #define REG_AFEC1_TEMPCWR                  (0x400B4074U) /**< \brief (AFEC1) AFEC Temperature Compare Window Register */
  #define REG_AFEC1_ACR                      (0x400B4094U) /**< \brief (AFEC1) AFEC Analog Control Register */
  #define REG_AFEC1_COSR                     (0x400B40D0U) /**< \brief (AFEC1) AFEC Correction Select Register */
  #define REG_AFEC1_CVR                      (0x400B40D4U) /**< \brief (AFEC1) AFEC Correction Values Register */
  #define REG_AFEC1_CECR                     (0x400B40D8U) /**< \brief (AFEC1) AFEC Channel Error Correction Register */
  #define REG_AFEC1_WPMR                     (0x400B40E4U) /**< \brief (AFEC1) AFEC Write Protection Mode Register */
  #define REG_AFEC1_WPSR                     (0x400B40E8U) /**< \brief (AFEC1) AFEC Write Protection Status Register */
  #define REG_AFEC1_RPR                      (0x400B4100U) /**< \brief (AFEC1) Receive Pointer Register */
  #define REG_AFEC1_RCR                      (0x400B4104U) /**< \brief (AFEC1) Receive Counter Register */
  #define REG_AFEC1_RNPR                     (0x400B4110U) /**< \brief (AFEC1) Receive Next Pointer Register */
  #define REG_AFEC1_RNCR                     (0x400B4114U) /**< \brief (AFEC1) Receive Next Counter Register */
  #define REG_AFEC1_PTCR                     (0x400B4120U) /**< \brief (AFEC1) Transfer Control Register */
  #define REG_AFEC1_PTSR                     (0x400B4124U) /**< \brief (AFEC1) Transfer Status Register */
#else
  #define REG_AFEC1_CR      (*(__O  uint32_t*)0x400B4000U) /**< \brief (AFEC1) AFEC Control Register */
  #define REG_AFEC1_MR      (*(__IO uint32_t*)0x400B4004U) /**< \brief (AFEC1) AFEC Mode Register */
  #define REG_AFEC1_EMR     (*(__IO uint32_t*)0x400B4008U) /**< \brief (AFEC1) AFEC Extended Mode Register */
  #define REG_AFEC1_SEQ1R   (*(__IO uint32_t*)0x400B400CU) /**< \brief (AFEC1) AFEC Channel Sequence 1 Register */
  #define REG_AFEC1_SEQ2R   (*(__IO uint32_t*)0x400B4010U) /**< \brief (AFEC1) AFEC Channel Sequence 2 Register */
  #define REG_AFEC1_CHER    (*(__O  uint32_t*)0x400B4014U) /**< \brief (AFEC1) AFEC Channel Enable Register */
  #define REG_AFEC1_CHDR    (*(__O  uint32_t*)0x400B4018U) /**< \brief (AFEC1) AFEC Channel Disable Register */
  #define REG_AFEC1_CHSR    (*(__I  uint32_t*)0x400B401CU) /**< \brief (AFEC1) AFEC Channel Status Register */
  #define REG_AFEC1_LCDR    (*(__I  uint32_t*)0x400B4020U) /**< \brief (AFEC1) AFEC Last Converted Data Register */
  #define REG_AFEC1_IER     (*(__O  uint32_t*)0x400B4024U) /**< \brief (AFEC1) AFEC Interrupt Enable Register */
  #define REG_AFEC1_IDR     (*(__O  uint32_t*)0x400B4028U) /**< \brief (AFEC1) AFEC Interrupt Disable Register */
  #define REG_AFEC1_IMR     (*(__I  uint32_t*)0x400B402CU) /**< \brief (AFEC1) AFEC Interrupt Mask Register */
  #define REG_AFEC1_ISR     (*(__I  uint32_t*)0x400B4030U) /**< \brief (AFEC1) AFEC Interrupt Status Register */
  #define REG_AFEC1_OVER    (*(__I  uint32_t*)0x400B404CU) /**< \brief (AFEC1) AFEC Overrun Status Register */
  #define REG_AFEC1_CWR     (*(__IO uint32_t*)0x400B4050U) /**< \brief (AFEC1) AFEC Compare Window Register */
  #define REG_AFEC1_CGR     (*(__IO uint32_t*)0x400B4054U) /**< \brief (AFEC1) AFEC Channel Gain Register */
  #define REG_AFEC1_CDOR    (*(__IO uint32_t*)0x400B405CU) /**< \brief (AFEC1) AFEC Channel Calibration DC Offset Register */
  #define REG_AFEC1_DIFFR   (*(__IO uint32_t*)0x400B4060U) /**< \brief (AFEC1) AFEC Channel Differential Register */
  #define REG_AFEC1_CSELR   (*(__IO uint32_t*)0x400B4064U) /**< \brief (AFEC1) AFEC Channel Register Selection */
  #define REG_AFEC1_CDR     (*(__I  uint32_t*)0x400B4068U) /**< \brief (AFEC1) AFEC Channel Data Register */
  #define REG_AFEC1_COCR    (*(__IO uint32_t*)0x400B406CU) /**< \brief (AFEC1) AFEC Channel Offset Compensation Register */
  #define REG_AFEC1_TEMPMR  (*(__IO uint32_t*)0x400B4070U) /**< \brief (AFEC1) AFEC Temperature Sensor Mode Register */
  #define REG_AFEC1_TEMPCWR (*(__IO uint32_t*)0x400B4074U) /**< \brief (AFEC1) AFEC Temperature Compare Window Register */
  #define REG_AFEC1_ACR     (*(__IO uint32_t*)0x400B4094U) /**< \brief (AFEC1) AFEC Analog Control Register */
  #define REG_AFEC1_COSR    (*(__IO uint32_t*)0x400B40D0U) /**< \brief (AFEC1) AFEC Correction Select Register */
  #define REG_AFEC1_CVR     (*(__IO uint32_t*)0x400B40D4U) /**< \brief (AFEC1) AFEC Correction Values Register */
  #define REG_AFEC1_CECR    (*(__IO uint32_t*)0x400B40D8U) /**< \brief (AFEC1) AFEC Channel Error Correction Register */
  #define REG_AFEC1_WPMR    (*(__IO uint32_t*)0x400B40E4U) /**< \brief (AFEC1) AFEC Write Protection Mode Register */
  #define REG_AFEC1_WPSR    (*(__I  uint32_t*)0x400B40E8U) /**< \brief (AFEC1) AFEC Write Protection Status Register */
  #define REG_AFEC1_RPR     (*(__IO uint32_t*)0x400B4100U) /**< \brief (AFEC1) Receive Pointer Register */
  #define REG_AFEC1_RCR     (*(__IO uint32_t*)0x400B4104U) /**< \brief (AFEC1) Receive Counter Register */
  #define REG_AFEC1_RNPR    (*(__IO uint32_t*)0x400B4110U) /**< \brief (AFEC1) Receive Next Pointer Register */
  #define REG_AFEC1_RNCR    (*(__IO uint32_t*)0x400B4114U) /**< \brief (AFEC1) Receive Next Counter Register */
  #define REG_AFEC1_PTCR    (*(__O  uint32_t*)0x400B4120U) /**< \brief (AFEC1) Transfer Control Register */
  #define REG_AFEC1_PTSR    (*(__I  uint32_t*)0x400B4124U) /**< \brief (AFEC1) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAM4E_AFEC1_INSTANCE_ */
