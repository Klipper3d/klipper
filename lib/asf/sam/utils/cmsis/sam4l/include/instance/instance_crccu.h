/**
 * \file
 *
 * \brief Instance description for CRCCU
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

#ifndef _SAM4L_CRCCU_INSTANCE_
#define _SAM4L_CRCCU_INSTANCE_

/* ========== Register definition for CRCCU peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_CRCCU_DSCR             (0x400A4000U) /**< \brief (CRCCU) Descriptor Base Register */
#define REG_CRCCU_DMAEN            (0x400A4008U) /**< \brief (CRCCU) DMA Enable Register */
#define REG_CRCCU_DMADIS           (0x400A400CU) /**< \brief (CRCCU) DMA Disable Register */
#define REG_CRCCU_DMASR            (0x400A4010U) /**< \brief (CRCCU) DMA Status Register */
#define REG_CRCCU_DMAIER           (0x400A4014U) /**< \brief (CRCCU) DMA Interrupt Enable Register */
#define REG_CRCCU_DMAIDR           (0x400A4018U) /**< \brief (CRCCU) DMA Interrupt Disable Register */
#define REG_CRCCU_DMAIMR           (0x400A401CU) /**< \brief (CRCCU) DMA Interrupt Mask Register */
#define REG_CRCCU_DMAISR           (0x400A4020U) /**< \brief (CRCCU) DMA Interrupt Status Register */
#define REG_CRCCU_CR               (0x400A4034U) /**< \brief (CRCCU) Control Register */
#define REG_CRCCU_MR               (0x400A4038U) /**< \brief (CRCCU) Mode Register */
#define REG_CRCCU_SR               (0x400A403CU) /**< \brief (CRCCU) Status Register */
#define REG_CRCCU_IER              (0x400A4040U) /**< \brief (CRCCU) Interrupt Enable Register */
#define REG_CRCCU_IDR              (0x400A4044U) /**< \brief (CRCCU) Interrupt Disable Register */
#define REG_CRCCU_IMR              (0x400A4048U) /**< \brief (CRCCU) Interrupt Mask Register */
#define REG_CRCCU_ISR              (0x400A404CU) /**< \brief (CRCCU) Interrupt Status Register */
#define REG_CRCCU_VERSION          (0x400A40FCU) /**< \brief (CRCCU) Version Register */
#else
#define REG_CRCCU_DSCR             (*(RwReg  *)0x400A4000U) /**< \brief (CRCCU) Descriptor Base Register */
#define REG_CRCCU_DMAEN            (*(WoReg  *)0x400A4008U) /**< \brief (CRCCU) DMA Enable Register */
#define REG_CRCCU_DMADIS           (*(WoReg  *)0x400A400CU) /**< \brief (CRCCU) DMA Disable Register */
#define REG_CRCCU_DMASR            (*(RoReg  *)0x400A4010U) /**< \brief (CRCCU) DMA Status Register */
#define REG_CRCCU_DMAIER           (*(WoReg  *)0x400A4014U) /**< \brief (CRCCU) DMA Interrupt Enable Register */
#define REG_CRCCU_DMAIDR           (*(WoReg  *)0x400A4018U) /**< \brief (CRCCU) DMA Interrupt Disable Register */
#define REG_CRCCU_DMAIMR           (*(RoReg  *)0x400A401CU) /**< \brief (CRCCU) DMA Interrupt Mask Register */
#define REG_CRCCU_DMAISR           (*(RoReg  *)0x400A4020U) /**< \brief (CRCCU) DMA Interrupt Status Register */
#define REG_CRCCU_CR               (*(WoReg  *)0x400A4034U) /**< \brief (CRCCU) Control Register */
#define REG_CRCCU_MR               (*(RwReg  *)0x400A4038U) /**< \brief (CRCCU) Mode Register */
#define REG_CRCCU_SR               (*(RoReg  *)0x400A403CU) /**< \brief (CRCCU) Status Register */
#define REG_CRCCU_IER              (*(WoReg  *)0x400A4040U) /**< \brief (CRCCU) Interrupt Enable Register */
#define REG_CRCCU_IDR              (*(WoReg  *)0x400A4044U) /**< \brief (CRCCU) Interrupt Disable Register */
#define REG_CRCCU_IMR              (*(RoReg  *)0x400A4048U) /**< \brief (CRCCU) Interrupt Mask Register */
#define REG_CRCCU_ISR              (*(RoReg  *)0x400A404CU) /**< \brief (CRCCU) Interrupt Status Register */
#define REG_CRCCU_VERSION          (*(RoReg  *)0x400A40FCU) /**< \brief (CRCCU) Version Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */


#endif /* _SAM4L_CRCCU_INSTANCE_ */
