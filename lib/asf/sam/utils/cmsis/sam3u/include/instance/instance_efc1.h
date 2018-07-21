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

#ifndef _SAM3U_EFC1_INSTANCE_
#define _SAM3U_EFC1_INSTANCE_

/* ========== Register definition for EFC1 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_EFC1_FMR           (0x400E0A00U) /**< \brief (EFC1) EEFC Flash Mode Register */
#define REG_EFC1_FCR           (0x400E0A04U) /**< \brief (EFC1) EEFC Flash Command Register */
#define REG_EFC1_FSR           (0x400E0A08U) /**< \brief (EFC1) EEFC Flash Status Register */
#define REG_EFC1_FRR           (0x400E0A0CU) /**< \brief (EFC1) EEFC Flash Result Register */
#else
#define REG_EFC1_FMR  (*(RwReg*)0x400E0A00U) /**< \brief (EFC1) EEFC Flash Mode Register */
#define REG_EFC1_FCR  (*(WoReg*)0x400E0A04U) /**< \brief (EFC1) EEFC Flash Command Register */
#define REG_EFC1_FSR  (*(RoReg*)0x400E0A08U) /**< \brief (EFC1) EEFC Flash Status Register */
#define REG_EFC1_FRR  (*(RoReg*)0x400E0A0CU) /**< \brief (EFC1) EEFC Flash Result Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAM3U_EFC1_INSTANCE_ */
