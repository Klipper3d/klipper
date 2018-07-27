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

#ifndef _SAM4CM_SMC1_INSTANCE_
#define _SAM4CM_SMC1_INSTANCE_

/* ========== Register definition for SMC1 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_SMC1_SETUP0                  (0x4801C000U) /**< \brief (SMC1) SMC Setup Register (CS_number = 0) */
  #define REG_SMC1_PULSE0                  (0x4801C004U) /**< \brief (SMC1) SMC Pulse Register (CS_number = 0) */
  #define REG_SMC1_CYCLE0                  (0x4801C008U) /**< \brief (SMC1) SMC Cycle Register (CS_number = 0) */
  #define REG_SMC1_MODE0                   (0x4801C00CU) /**< \brief (SMC1) SMC Mode Register (CS_number = 0) */
  #define REG_SMC1_SETUP1                  (0x4801C010U) /**< \brief (SMC1) SMC Setup Register (CS_number = 1) */
  #define REG_SMC1_PULSE1                  (0x4801C014U) /**< \brief (SMC1) SMC Pulse Register (CS_number = 1) */
  #define REG_SMC1_CYCLE1                  (0x4801C018U) /**< \brief (SMC1) SMC Cycle Register (CS_number = 1) */
  #define REG_SMC1_MODE1                   (0x4801C01CU) /**< \brief (SMC1) SMC Mode Register (CS_number = 1) */
  #define REG_SMC1_SETUP2                  (0x4801C020U) /**< \brief (SMC1) SMC Setup Register (CS_number = 2) */
  #define REG_SMC1_PULSE2                  (0x4801C024U) /**< \brief (SMC1) SMC Pulse Register (CS_number = 2) */
  #define REG_SMC1_CYCLE2                  (0x4801C028U) /**< \brief (SMC1) SMC Cycle Register (CS_number = 2) */
  #define REG_SMC1_MODE2                   (0x4801C02CU) /**< \brief (SMC1) SMC Mode Register (CS_number = 2) */
  #define REG_SMC1_SETUP3                  (0x4801C030U) /**< \brief (SMC1) SMC Setup Register (CS_number = 3) */
  #define REG_SMC1_PULSE3                  (0x4801C034U) /**< \brief (SMC1) SMC Pulse Register (CS_number = 3) */
  #define REG_SMC1_CYCLE3                  (0x4801C038U) /**< \brief (SMC1) SMC Cycle Register (CS_number = 3) */
  #define REG_SMC1_MODE3                   (0x4801C03CU) /**< \brief (SMC1) SMC Mode Register (CS_number = 3) */
  #define REG_SMC1_OCMS                    (0x4801C080U) /**< \brief (SMC1) SMC OCMS MODE Register */
  #define REG_SMC1_KEY1                    (0x4801C084U) /**< \brief (SMC1) SMC OCMS KEY1 Register */
  #define REG_SMC1_KEY2                    (0x4801C088U) /**< \brief (SMC1) SMC OCMS KEY2 Register */
  #define REG_SMC1_WPMR                    (0x4801C0E4U) /**< \brief (SMC1) SMC Write Protect Mode Register */
  #define REG_SMC1_WPSR                    (0x4801C0E8U) /**< \brief (SMC1) SMC Write Protect Status Register */
#else
  #define REG_SMC1_SETUP0 (*(__IO uint32_t*)0x4801C000U) /**< \brief (SMC1) SMC Setup Register (CS_number = 0) */
  #define REG_SMC1_PULSE0 (*(__IO uint32_t*)0x4801C004U) /**< \brief (SMC1) SMC Pulse Register (CS_number = 0) */
  #define REG_SMC1_CYCLE0 (*(__IO uint32_t*)0x4801C008U) /**< \brief (SMC1) SMC Cycle Register (CS_number = 0) */
  #define REG_SMC1_MODE0  (*(__IO uint32_t*)0x4801C00CU) /**< \brief (SMC1) SMC Mode Register (CS_number = 0) */
  #define REG_SMC1_SETUP1 (*(__IO uint32_t*)0x4801C010U) /**< \brief (SMC1) SMC Setup Register (CS_number = 1) */
  #define REG_SMC1_PULSE1 (*(__IO uint32_t*)0x4801C014U) /**< \brief (SMC1) SMC Pulse Register (CS_number = 1) */
  #define REG_SMC1_CYCLE1 (*(__IO uint32_t*)0x4801C018U) /**< \brief (SMC1) SMC Cycle Register (CS_number = 1) */
  #define REG_SMC1_MODE1  (*(__IO uint32_t*)0x4801C01CU) /**< \brief (SMC1) SMC Mode Register (CS_number = 1) */
  #define REG_SMC1_SETUP2 (*(__IO uint32_t*)0x4801C020U) /**< \brief (SMC1) SMC Setup Register (CS_number = 2) */
  #define REG_SMC1_PULSE2 (*(__IO uint32_t*)0x4801C024U) /**< \brief (SMC1) SMC Pulse Register (CS_number = 2) */
  #define REG_SMC1_CYCLE2 (*(__IO uint32_t*)0x4801C028U) /**< \brief (SMC1) SMC Cycle Register (CS_number = 2) */
  #define REG_SMC1_MODE2  (*(__IO uint32_t*)0x4801C02CU) /**< \brief (SMC1) SMC Mode Register (CS_number = 2) */
  #define REG_SMC1_SETUP3 (*(__IO uint32_t*)0x4801C030U) /**< \brief (SMC1) SMC Setup Register (CS_number = 3) */
  #define REG_SMC1_PULSE3 (*(__IO uint32_t*)0x4801C034U) /**< \brief (SMC1) SMC Pulse Register (CS_number = 3) */
  #define REG_SMC1_CYCLE3 (*(__IO uint32_t*)0x4801C038U) /**< \brief (SMC1) SMC Cycle Register (CS_number = 3) */
  #define REG_SMC1_MODE3  (*(__IO uint32_t*)0x4801C03CU) /**< \brief (SMC1) SMC Mode Register (CS_number = 3) */
  #define REG_SMC1_OCMS   (*(__IO uint32_t*)0x4801C080U) /**< \brief (SMC1) SMC OCMS MODE Register */
  #define REG_SMC1_KEY1   (*(__O  uint32_t*)0x4801C084U) /**< \brief (SMC1) SMC OCMS KEY1 Register */
  #define REG_SMC1_KEY2   (*(__O  uint32_t*)0x4801C088U) /**< \brief (SMC1) SMC OCMS KEY2 Register */
  #define REG_SMC1_WPMR   (*(__IO uint32_t*)0x4801C0E4U) /**< \brief (SMC1) SMC Write Protect Mode Register */
  #define REG_SMC1_WPSR   (*(__I  uint32_t*)0x4801C0E8U) /**< \brief (SMC1) SMC Write Protect Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAM4CM_SMC1_INSTANCE_ */
