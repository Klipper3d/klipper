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

#ifndef _SAM4E_AES_INSTANCE_
#define _SAM4E_AES_INSTANCE_

/* ========== Register definition for AES peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_AES_CR                      (0x40004000U) /**< \brief (AES) Control Register */
  #define REG_AES_MR                      (0x40004004U) /**< \brief (AES) Mode Register */
  #define REG_AES_IER                     (0x40004010U) /**< \brief (AES) Interrupt Enable Register */
  #define REG_AES_IDR                     (0x40004014U) /**< \brief (AES) Interrupt Disable Register */
  #define REG_AES_IMR                     (0x40004018U) /**< \brief (AES) Interrupt Mask Register */
  #define REG_AES_ISR                     (0x4000401CU) /**< \brief (AES) Interrupt Status Register */
  #define REG_AES_KEYWR                   (0x40004020U) /**< \brief (AES) Key Word Register */
  #define REG_AES_IDATAR                  (0x40004040U) /**< \brief (AES) Input Data Register */
  #define REG_AES_ODATAR                  (0x40004050U) /**< \brief (AES) Output Data Register */
  #define REG_AES_IVR                     (0x40004060U) /**< \brief (AES) Initialization Vector Register */
#else
  #define REG_AES_CR     (*(__O  uint32_t*)0x40004000U) /**< \brief (AES) Control Register */
  #define REG_AES_MR     (*(__IO uint32_t*)0x40004004U) /**< \brief (AES) Mode Register */
  #define REG_AES_IER    (*(__O  uint32_t*)0x40004010U) /**< \brief (AES) Interrupt Enable Register */
  #define REG_AES_IDR    (*(__O  uint32_t*)0x40004014U) /**< \brief (AES) Interrupt Disable Register */
  #define REG_AES_IMR    (*(__I  uint32_t*)0x40004018U) /**< \brief (AES) Interrupt Mask Register */
  #define REG_AES_ISR    (*(__I  uint32_t*)0x4000401CU) /**< \brief (AES) Interrupt Status Register */
  #define REG_AES_KEYWR  (*(__O  uint32_t*)0x40004020U) /**< \brief (AES) Key Word Register */
  #define REG_AES_IDATAR (*(__O  uint32_t*)0x40004040U) /**< \brief (AES) Input Data Register */
  #define REG_AES_ODATAR (*(__I  uint32_t*)0x40004050U) /**< \brief (AES) Output Data Register */
  #define REG_AES_IVR    (*(__O  uint32_t*)0x40004060U) /**< \brief (AES) Initialization Vector Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAM4E_AES_INSTANCE_ */
