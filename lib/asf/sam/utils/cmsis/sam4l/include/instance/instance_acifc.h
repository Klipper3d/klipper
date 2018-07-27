/**
 * \file
 *
 * \brief Instance description for ACIFC
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

#ifndef _SAM4L_ACIFC_INSTANCE_
#define _SAM4L_ACIFC_INSTANCE_

/* ========== Register definition for ACIFC peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_ACIFC_CTRL             (0x40040000U) /**< \brief (ACIFC) Control Register */
#define REG_ACIFC_SR               (0x40040004U) /**< \brief (ACIFC) Status Register */
#define REG_ACIFC_IER              (0x40040010U) /**< \brief (ACIFC) Interrupt Enable Register */
#define REG_ACIFC_IDR              (0x40040014U) /**< \brief (ACIFC) Interrupt Disable Register */
#define REG_ACIFC_IMR              (0x40040018U) /**< \brief (ACIFC) Interrupt Mask Register */
#define REG_ACIFC_ISR              (0x4004001CU) /**< \brief (ACIFC) Interrupt Status Register */
#define REG_ACIFC_ICR              (0x40040020U) /**< \brief (ACIFC) Interrupt Status Clear Register */
#define REG_ACIFC_TR               (0x40040024U) /**< \brief (ACIFC) Test Register */
#define REG_ACIFC_PARAMETER        (0x40040030U) /**< \brief (ACIFC) Parameter Register */
#define REG_ACIFC_VERSION          (0x40040034U) /**< \brief (ACIFC) Version Register */
#define REG_ACIFC_CONFW0           (0x40040080U) /**< \brief (ACIFC) Window configuration Register 0 */
#define REG_ACIFC_CONFW1           (0x40040084U) /**< \brief (ACIFC) Window configuration Register 1 */
#define REG_ACIFC_CONFW2           (0x40040088U) /**< \brief (ACIFC) Window configuration Register 2 */
#define REG_ACIFC_CONFW3           (0x4004008CU) /**< \brief (ACIFC) Window configuration Register 3 */
#define REG_ACIFC_CONF0            (0x400400D0U) /**< \brief (ACIFC) AC Configuration Register 0 */
#define REG_ACIFC_CONF1            (0x400400D4U) /**< \brief (ACIFC) AC Configuration Register 1 */
#define REG_ACIFC_CONF2            (0x400400D8U) /**< \brief (ACIFC) AC Configuration Register 2 */
#define REG_ACIFC_CONF3            (0x400400DCU) /**< \brief (ACIFC) AC Configuration Register 3 */
#define REG_ACIFC_CONF4            (0x400400E0U) /**< \brief (ACIFC) AC Configuration Register 4 */
#define REG_ACIFC_CONF5            (0x400400E4U) /**< \brief (ACIFC) AC Configuration Register 5 */
#define REG_ACIFC_CONF6            (0x400400E8U) /**< \brief (ACIFC) AC Configuration Register 6 */
#define REG_ACIFC_CONF7            (0x400400ECU) /**< \brief (ACIFC) AC Configuration Register 7 */
#else
#define REG_ACIFC_CTRL             (*(RwReg  *)0x40040000U) /**< \brief (ACIFC) Control Register */
#define REG_ACIFC_SR               (*(RoReg  *)0x40040004U) /**< \brief (ACIFC) Status Register */
#define REG_ACIFC_IER              (*(WoReg  *)0x40040010U) /**< \brief (ACIFC) Interrupt Enable Register */
#define REG_ACIFC_IDR              (*(WoReg  *)0x40040014U) /**< \brief (ACIFC) Interrupt Disable Register */
#define REG_ACIFC_IMR              (*(RoReg  *)0x40040018U) /**< \brief (ACIFC) Interrupt Mask Register */
#define REG_ACIFC_ISR              (*(RoReg  *)0x4004001CU) /**< \brief (ACIFC) Interrupt Status Register */
#define REG_ACIFC_ICR              (*(WoReg  *)0x40040020U) /**< \brief (ACIFC) Interrupt Status Clear Register */
#define REG_ACIFC_TR               (*(RwReg  *)0x40040024U) /**< \brief (ACIFC) Test Register */
#define REG_ACIFC_PARAMETER        (*(RoReg  *)0x40040030U) /**< \brief (ACIFC) Parameter Register */
#define REG_ACIFC_VERSION          (*(RoReg  *)0x40040034U) /**< \brief (ACIFC) Version Register */
#define REG_ACIFC_CONFW0           (*(RwReg  *)0x40040080U) /**< \brief (ACIFC) Window configuration Register 0 */
#define REG_ACIFC_CONFW1           (*(RwReg  *)0x40040084U) /**< \brief (ACIFC) Window configuration Register 1 */
#define REG_ACIFC_CONFW2           (*(RwReg  *)0x40040088U) /**< \brief (ACIFC) Window configuration Register 2 */
#define REG_ACIFC_CONFW3           (*(RwReg  *)0x4004008CU) /**< \brief (ACIFC) Window configuration Register 3 */
#define REG_ACIFC_CONF0            (*(RwReg  *)0x400400D0U) /**< \brief (ACIFC) AC Configuration Register 0 */
#define REG_ACIFC_CONF1            (*(RwReg  *)0x400400D4U) /**< \brief (ACIFC) AC Configuration Register 1 */
#define REG_ACIFC_CONF2            (*(RwReg  *)0x400400D8U) /**< \brief (ACIFC) AC Configuration Register 2 */
#define REG_ACIFC_CONF3            (*(RwReg  *)0x400400DCU) /**< \brief (ACIFC) AC Configuration Register 3 */
#define REG_ACIFC_CONF4            (*(RwReg  *)0x400400E0U) /**< \brief (ACIFC) AC Configuration Register 4 */
#define REG_ACIFC_CONF5            (*(RwReg  *)0x400400E4U) /**< \brief (ACIFC) AC Configuration Register 5 */
#define REG_ACIFC_CONF6            (*(RwReg  *)0x400400E8U) /**< \brief (ACIFC) AC Configuration Register 6 */
#define REG_ACIFC_CONF7            (*(RwReg  *)0x400400ECU) /**< \brief (ACIFC) AC Configuration Register 7 */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */


#endif /* _SAM4L_ACIFC_INSTANCE_ */
