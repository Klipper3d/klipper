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

#ifndef _SAM4CM_IPC0_INSTANCE_
#define _SAM4CM_IPC0_INSTANCE_

/* ========== Register definition for IPC0 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_IPC0_ISCR                 (0x4004C000U) /**< \brief (IPC0) Interrupt Set Command Register */
  #define REG_IPC0_ICCR                 (0x4004C004U) /**< \brief (IPC0) Interrupt Clear Command Register */
  #define REG_IPC0_IPR                  (0x4004C008U) /**< \brief (IPC0) Interrupt Pending Register */
  #define REG_IPC0_IECR                 (0x4004C00CU) /**< \brief (IPC0) Interrupt Enable Command Register */
  #define REG_IPC0_IDCR                 (0x4004C010U) /**< \brief (IPC0) Interrupt Disable Command Register */
  #define REG_IPC0_IMR                  (0x4004C014U) /**< \brief (IPC0) Interrupt Mask Register */
  #define REG_IPC0_ISR                  (0x4004C018U) /**< \brief (IPC0) Interrupt Status Register */
#else
  #define REG_IPC0_ISCR (*(__O uint32_t*)0x4004C000U) /**< \brief (IPC0) Interrupt Set Command Register */
  #define REG_IPC0_ICCR (*(__O uint32_t*)0x4004C004U) /**< \brief (IPC0) Interrupt Clear Command Register */
  #define REG_IPC0_IPR  (*(__I uint32_t*)0x4004C008U) /**< \brief (IPC0) Interrupt Pending Register */
  #define REG_IPC0_IECR (*(__O uint32_t*)0x4004C00CU) /**< \brief (IPC0) Interrupt Enable Command Register */
  #define REG_IPC0_IDCR (*(__O uint32_t*)0x4004C010U) /**< \brief (IPC0) Interrupt Disable Command Register */
  #define REG_IPC0_IMR  (*(__I uint32_t*)0x4004C014U) /**< \brief (IPC0) Interrupt Mask Register */
  #define REG_IPC0_ISR  (*(__I uint32_t*)0x4004C018U) /**< \brief (IPC0) Interrupt Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAM4CM_IPC0_INSTANCE_ */
