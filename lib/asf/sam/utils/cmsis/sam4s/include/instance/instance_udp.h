/**
 * \file
 *
 * Copyright (c) 2012-2018 Microchip Technology Inc. and its subsidiaries.
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

#ifndef _SAM4S_UDP_INSTANCE_
#define _SAM4S_UDP_INSTANCE_

/* ========== Register definition for UDP peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_UDP_FRM_NUM                   (0x40034000U) /**< \brief (UDP) Frame Number Register */
  #define REG_UDP_GLB_STAT                  (0x40034004U) /**< \brief (UDP) Global State Register */
  #define REG_UDP_FADDR                     (0x40034008U) /**< \brief (UDP) Function Address Register */
  #define REG_UDP_IER                       (0x40034010U) /**< \brief (UDP) Interrupt Enable Register */
  #define REG_UDP_IDR                       (0x40034014U) /**< \brief (UDP) Interrupt Disable Register */
  #define REG_UDP_IMR                       (0x40034018U) /**< \brief (UDP) Interrupt Mask Register */
  #define REG_UDP_ISR                       (0x4003401CU) /**< \brief (UDP) Interrupt Status Register */
  #define REG_UDP_ICR                       (0x40034020U) /**< \brief (UDP) Interrupt Clear Register */
  #define REG_UDP_RST_EP                    (0x40034028U) /**< \brief (UDP) Reset Endpoint Register */
  #define REG_UDP_CSR                       (0x40034030U) /**< \brief (UDP) Endpoint Control and Status Register */
  #define REG_UDP_FDR                       (0x40034050U) /**< \brief (UDP) Endpoint FIFO Data Register */
  #define REG_UDP_TXVC                      (0x40034074U) /**< \brief (UDP) Transceiver Control Register */
#else
  #define REG_UDP_FRM_NUM  (*(__I  uint32_t*)0x40034000U) /**< \brief (UDP) Frame Number Register */
  #define REG_UDP_GLB_STAT (*(__IO uint32_t*)0x40034004U) /**< \brief (UDP) Global State Register */
  #define REG_UDP_FADDR    (*(__IO uint32_t*)0x40034008U) /**< \brief (UDP) Function Address Register */
  #define REG_UDP_IER      (*(__O  uint32_t*)0x40034010U) /**< \brief (UDP) Interrupt Enable Register */
  #define REG_UDP_IDR      (*(__O  uint32_t*)0x40034014U) /**< \brief (UDP) Interrupt Disable Register */
  #define REG_UDP_IMR      (*(__I  uint32_t*)0x40034018U) /**< \brief (UDP) Interrupt Mask Register */
  #define REG_UDP_ISR      (*(__I  uint32_t*)0x4003401CU) /**< \brief (UDP) Interrupt Status Register */
  #define REG_UDP_ICR      (*(__O  uint32_t*)0x40034020U) /**< \brief (UDP) Interrupt Clear Register */
  #define REG_UDP_RST_EP   (*(__IO uint32_t*)0x40034028U) /**< \brief (UDP) Reset Endpoint Register */
  #define REG_UDP_CSR      (*(__IO uint32_t*)0x40034030U) /**< \brief (UDP) Endpoint Control and Status Register */
  #define REG_UDP_FDR      (*(__IO uint32_t*)0x40034050U) /**< \brief (UDP) Endpoint FIFO Data Register */
  #define REG_UDP_TXVC     (*(__IO uint32_t*)0x40034074U) /**< \brief (UDP) Transceiver Control Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAM4S_UDP_INSTANCE_ */
