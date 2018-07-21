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

#ifndef _SAM4CP_AES_INSTANCE_
#define _SAM4CP_AES_INSTANCE_

/* ========== Register definition for AES peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_AES_CR                       (0x40000000U) /**< \brief (AES) Control Register */
  #define REG_AES_MR                       (0x40000004U) /**< \brief (AES) Mode Register */
  #define REG_AES_IER                      (0x40000010U) /**< \brief (AES) Interrupt Enable Register */
  #define REG_AES_IDR                      (0x40000014U) /**< \brief (AES) Interrupt Disable Register */
  #define REG_AES_IMR                      (0x40000018U) /**< \brief (AES) Interrupt Mask Register */
  #define REG_AES_ISR                      (0x4000001CU) /**< \brief (AES) Interrupt Status Register */
  #define REG_AES_KEYWR                    (0x40000020U) /**< \brief (AES) Key Word Register */
  #define REG_AES_IDATAR                   (0x40000040U) /**< \brief (AES) Input Data Register */
  #define REG_AES_ODATAR                   (0x40000050U) /**< \brief (AES) Output Data Register */
  #define REG_AES_IVR                      (0x40000060U) /**< \brief (AES) Initialization Vector Register */
  #define REG_AES_AADLENR                  (0x40000070U) /**< \brief (AES) Additional Authenticated Data Length Register */
  #define REG_AES_CLENR                    (0x40000074U) /**< \brief (AES) Plaintext/Ciphertext Length Register */
  #define REG_AES_GHASHR                   (0x40000078U) /**< \brief (AES) GCM Intermediate Hash Word Register */
  #define REG_AES_TAGR                     (0x40000088U) /**< \brief (AES) GCM Authentication Tag Word Register */
  #define REG_AES_CTRR                     (0x40000098U) /**< \brief (AES) GCM Encryption Counter Value Register */
  #define REG_AES_GCMHR                    (0x4000009CU) /**< \brief (AES) GCM H World Register */
  #define REG_AES_RPR                      (0x40000100U) /**< \brief (AES) Receive Pointer Register */
  #define REG_AES_RCR                      (0x40000104U) /**< \brief (AES) Receive Counter Register */
  #define REG_AES_TPR                      (0x40000108U) /**< \brief (AES) Transmit Pointer Register */
  #define REG_AES_TCR                      (0x4000010CU) /**< \brief (AES) Transmit Counter Register */
  #define REG_AES_RNPR                     (0x40000110U) /**< \brief (AES) Receive Next Pointer Register */
  #define REG_AES_RNCR                     (0x40000114U) /**< \brief (AES) Receive Next Counter Register */
  #define REG_AES_TNPR                     (0x40000118U) /**< \brief (AES) Transmit Next Pointer Register */
  #define REG_AES_TNCR                     (0x4000011CU) /**< \brief (AES) Transmit Next Counter Register */
  #define REG_AES_PTCR                     (0x40000120U) /**< \brief (AES) Transfer Control Register */
  #define REG_AES_PTSR                     (0x40000124U) /**< \brief (AES) Transfer Status Register */
#else
  #define REG_AES_CR      (*(__O  uint32_t*)0x40000000U) /**< \brief (AES) Control Register */
  #define REG_AES_MR      (*(__IO uint32_t*)0x40000004U) /**< \brief (AES) Mode Register */
  #define REG_AES_IER     (*(__O  uint32_t*)0x40000010U) /**< \brief (AES) Interrupt Enable Register */
  #define REG_AES_IDR     (*(__O  uint32_t*)0x40000014U) /**< \brief (AES) Interrupt Disable Register */
  #define REG_AES_IMR     (*(__I  uint32_t*)0x40000018U) /**< \brief (AES) Interrupt Mask Register */
  #define REG_AES_ISR     (*(__I  uint32_t*)0x4000001CU) /**< \brief (AES) Interrupt Status Register */
  #define REG_AES_KEYWR   (*(__O  uint32_t*)0x40000020U) /**< \brief (AES) Key Word Register */
  #define REG_AES_IDATAR  (*(__O  uint32_t*)0x40000040U) /**< \brief (AES) Input Data Register */
  #define REG_AES_ODATAR  (*(__I  uint32_t*)0x40000050U) /**< \brief (AES) Output Data Register */
  #define REG_AES_IVR     (*(__O  uint32_t*)0x40000060U) /**< \brief (AES) Initialization Vector Register */
  #define REG_AES_AADLENR (*(__IO uint32_t*)0x40000070U) /**< \brief (AES) Additional Authenticated Data Length Register */
  #define REG_AES_CLENR   (*(__IO uint32_t*)0x40000074U) /**< \brief (AES) Plaintext/Ciphertext Length Register */
  #define REG_AES_GHASHR  (*(__IO uint32_t*)0x40000078U) /**< \brief (AES) GCM Intermediate Hash Word Register */
  #define REG_AES_TAGR    (*(__I  uint32_t*)0x40000088U) /**< \brief (AES) GCM Authentication Tag Word Register */
  #define REG_AES_CTRR    (*(__I  uint32_t*)0x40000098U) /**< \brief (AES) GCM Encryption Counter Value Register */
  #define REG_AES_GCMHR   (*(__IO uint32_t*)0x4000009CU) /**< \brief (AES) GCM H World Register */
  #define REG_AES_RPR     (*(__IO uint32_t*)0x40000100U) /**< \brief (AES) Receive Pointer Register */
  #define REG_AES_RCR     (*(__IO uint32_t*)0x40000104U) /**< \brief (AES) Receive Counter Register */
  #define REG_AES_TPR     (*(__IO uint32_t*)0x40000108U) /**< \brief (AES) Transmit Pointer Register */
  #define REG_AES_TCR     (*(__IO uint32_t*)0x4000010CU) /**< \brief (AES) Transmit Counter Register */
  #define REG_AES_RNPR    (*(__IO uint32_t*)0x40000110U) /**< \brief (AES) Receive Next Pointer Register */
  #define REG_AES_RNCR    (*(__IO uint32_t*)0x40000114U) /**< \brief (AES) Receive Next Counter Register */
  #define REG_AES_TNPR    (*(__IO uint32_t*)0x40000118U) /**< \brief (AES) Transmit Next Pointer Register */
  #define REG_AES_TNCR    (*(__IO uint32_t*)0x4000011CU) /**< \brief (AES) Transmit Next Counter Register */
  #define REG_AES_PTCR    (*(__O  uint32_t*)0x40000120U) /**< \brief (AES) Transfer Control Register */
  #define REG_AES_PTSR    (*(__I  uint32_t*)0x40000124U) /**< \brief (AES) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAM4CP_AES_INSTANCE_ */
