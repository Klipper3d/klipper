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

#ifndef _SAM4E_SMC_INSTANCE_
#define _SAM4E_SMC_INSTANCE_

/* ========== Register definition for SMC peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_SMC_SETUP0                  (0x40060000U) /**< \brief (SMC) SMC Setup Register (CS_number = 0) */
  #define REG_SMC_PULSE0                  (0x40060004U) /**< \brief (SMC) SMC Pulse Register (CS_number = 0) */
  #define REG_SMC_CYCLE0                  (0x40060008U) /**< \brief (SMC) SMC Cycle Register (CS_number = 0) */
  #define REG_SMC_MODE0                   (0x4006000CU) /**< \brief (SMC) SMC Mode Register (CS_number = 0) */
  #define REG_SMC_SETUP1                  (0x40060010U) /**< \brief (SMC) SMC Setup Register (CS_number = 1) */
  #define REG_SMC_PULSE1                  (0x40060014U) /**< \brief (SMC) SMC Pulse Register (CS_number = 1) */
  #define REG_SMC_CYCLE1                  (0x40060018U) /**< \brief (SMC) SMC Cycle Register (CS_number = 1) */
  #define REG_SMC_MODE1                   (0x4006001CU) /**< \brief (SMC) SMC Mode Register (CS_number = 1) */
  #define REG_SMC_SETUP2                  (0x40060020U) /**< \brief (SMC) SMC Setup Register (CS_number = 2) */
  #define REG_SMC_PULSE2                  (0x40060024U) /**< \brief (SMC) SMC Pulse Register (CS_number = 2) */
  #define REG_SMC_CYCLE2                  (0x40060028U) /**< \brief (SMC) SMC Cycle Register (CS_number = 2) */
  #define REG_SMC_MODE2                   (0x4006002CU) /**< \brief (SMC) SMC Mode Register (CS_number = 2) */
  #define REG_SMC_SETUP3                  (0x40060030U) /**< \brief (SMC) SMC Setup Register (CS_number = 3) */
  #define REG_SMC_PULSE3                  (0x40060034U) /**< \brief (SMC) SMC Pulse Register (CS_number = 3) */
  #define REG_SMC_CYCLE3                  (0x40060038U) /**< \brief (SMC) SMC Cycle Register (CS_number = 3) */
  #define REG_SMC_MODE3                   (0x4006003CU) /**< \brief (SMC) SMC Mode Register (CS_number = 3) */
  #define REG_SMC_OCMS                    (0x40060080U) /**< \brief (SMC) SMC OCMS MODE Register */
  #define REG_SMC_KEY1                    (0x40060084U) /**< \brief (SMC) SMC OCMS KEY1 Register */
  #define REG_SMC_KEY2                    (0x40060088U) /**< \brief (SMC) SMC OCMS KEY2 Register */
  #define REG_SMC_WPMR                    (0x400600E4U) /**< \brief (SMC) SMC Write Protect Mode Register */
  #define REG_SMC_WPSR                    (0x400600E8U) /**< \brief (SMC) SMC Write Protect Status Register */
#else
  #define REG_SMC_SETUP0 (*(__IO uint32_t*)0x40060000U) /**< \brief (SMC) SMC Setup Register (CS_number = 0) */
  #define REG_SMC_PULSE0 (*(__IO uint32_t*)0x40060004U) /**< \brief (SMC) SMC Pulse Register (CS_number = 0) */
  #define REG_SMC_CYCLE0 (*(__IO uint32_t*)0x40060008U) /**< \brief (SMC) SMC Cycle Register (CS_number = 0) */
  #define REG_SMC_MODE0  (*(__IO uint32_t*)0x4006000CU) /**< \brief (SMC) SMC Mode Register (CS_number = 0) */
  #define REG_SMC_SETUP1 (*(__IO uint32_t*)0x40060010U) /**< \brief (SMC) SMC Setup Register (CS_number = 1) */
  #define REG_SMC_PULSE1 (*(__IO uint32_t*)0x40060014U) /**< \brief (SMC) SMC Pulse Register (CS_number = 1) */
  #define REG_SMC_CYCLE1 (*(__IO uint32_t*)0x40060018U) /**< \brief (SMC) SMC Cycle Register (CS_number = 1) */
  #define REG_SMC_MODE1  (*(__IO uint32_t*)0x4006001CU) /**< \brief (SMC) SMC Mode Register (CS_number = 1) */
  #define REG_SMC_SETUP2 (*(__IO uint32_t*)0x40060020U) /**< \brief (SMC) SMC Setup Register (CS_number = 2) */
  #define REG_SMC_PULSE2 (*(__IO uint32_t*)0x40060024U) /**< \brief (SMC) SMC Pulse Register (CS_number = 2) */
  #define REG_SMC_CYCLE2 (*(__IO uint32_t*)0x40060028U) /**< \brief (SMC) SMC Cycle Register (CS_number = 2) */
  #define REG_SMC_MODE2  (*(__IO uint32_t*)0x4006002CU) /**< \brief (SMC) SMC Mode Register (CS_number = 2) */
  #define REG_SMC_SETUP3 (*(__IO uint32_t*)0x40060030U) /**< \brief (SMC) SMC Setup Register (CS_number = 3) */
  #define REG_SMC_PULSE3 (*(__IO uint32_t*)0x40060034U) /**< \brief (SMC) SMC Pulse Register (CS_number = 3) */
  #define REG_SMC_CYCLE3 (*(__IO uint32_t*)0x40060038U) /**< \brief (SMC) SMC Cycle Register (CS_number = 3) */
  #define REG_SMC_MODE3  (*(__IO uint32_t*)0x4006003CU) /**< \brief (SMC) SMC Mode Register (CS_number = 3) */
  #define REG_SMC_OCMS   (*(__IO uint32_t*)0x40060080U) /**< \brief (SMC) SMC OCMS MODE Register */
  #define REG_SMC_KEY1   (*(__O  uint32_t*)0x40060084U) /**< \brief (SMC) SMC OCMS KEY1 Register */
  #define REG_SMC_KEY2   (*(__O  uint32_t*)0x40060088U) /**< \brief (SMC) SMC OCMS KEY2 Register */
  #define REG_SMC_WPMR   (*(__IO uint32_t*)0x400600E4U) /**< \brief (SMC) SMC Write Protect Mode Register */
  #define REG_SMC_WPSR   (*(__I  uint32_t*)0x400600E8U) /**< \brief (SMC) SMC Write Protect Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAM4E_SMC_INSTANCE_ */
