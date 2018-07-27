/**
 * \file
 *
 * \brief Instance description for AESA
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

#ifndef _SAM4L_AESA_INSTANCE_
#define _SAM4L_AESA_INSTANCE_

/* ========== Register definition for AESA peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_AESA_CTRL              (0x400B0000U) /**< \brief (AESA) Control Register */
#define REG_AESA_MODE              (0x400B0004U) /**< \brief (AESA) Mode Register */
#define REG_AESA_DATABUFPTR        (0x400B0008U) /**< \brief (AESA) Data Buffer Pointer Register */
#define REG_AESA_SR                (0x400B000CU) /**< \brief (AESA) Status Register */
#define REG_AESA_IER               (0x400B0010U) /**< \brief (AESA) Interrupt Enable Register */
#define REG_AESA_IDR               (0x400B0014U) /**< \brief (AESA) Interrupt Disable Register */
#define REG_AESA_IMR               (0x400B0018U) /**< \brief (AESA) Interrupt Mask Register */
#define REG_AESA_KEY0              (0x400B0020U) /**< \brief (AESA) Key Register 0 */
#define REG_AESA_KEY1              (0x400B0024U) /**< \brief (AESA) Key Register 1 */
#define REG_AESA_KEY2              (0x400B0028U) /**< \brief (AESA) Key Register 2 */
#define REG_AESA_KEY3              (0x400B002CU) /**< \brief (AESA) Key Register 3 */
#define REG_AESA_KEY4              (0x400B0030U) /**< \brief (AESA) Key Register 4 */
#define REG_AESA_KEY5              (0x400B0034U) /**< \brief (AESA) Key Register 5 */
#define REG_AESA_KEY6              (0x400B0038U) /**< \brief (AESA) Key Register 6 */
#define REG_AESA_KEY7              (0x400B003CU) /**< \brief (AESA) Key Register 7 */
#define REG_AESA_INITVECT0         (0x400B0040U) /**< \brief (AESA) Initialization Vector Register 0 */
#define REG_AESA_INITVECT1         (0x400B0044U) /**< \brief (AESA) Initialization Vector Register 1 */
#define REG_AESA_INITVECT2         (0x400B0048U) /**< \brief (AESA) Initialization Vector Register 2 */
#define REG_AESA_INITVECT3         (0x400B004CU) /**< \brief (AESA) Initialization Vector Register 3 */
#define REG_AESA_IDATA             (0x400B0050U) /**< \brief (AESA) Input Data Register */
#define REG_AESA_ODATA             (0x400B0060U) /**< \brief (AESA) Output Data Register */
#define REG_AESA_DRNGSEED          (0x400B0070U) /**< \brief (AESA) DRNG Seed Register */
#define REG_AESA_PARAMETER         (0x400B00F8U) /**< \brief (AESA) Parameter Register */
#define REG_AESA_VERSION           (0x400B00FCU) /**< \brief (AESA) Version Register */
#else
#define REG_AESA_CTRL              (*(RwReg  *)0x400B0000U) /**< \brief (AESA) Control Register */
#define REG_AESA_MODE              (*(RwReg  *)0x400B0004U) /**< \brief (AESA) Mode Register */
#define REG_AESA_DATABUFPTR        (*(RwReg  *)0x400B0008U) /**< \brief (AESA) Data Buffer Pointer Register */
#define REG_AESA_SR                (*(RoReg  *)0x400B000CU) /**< \brief (AESA) Status Register */
#define REG_AESA_IER               (*(WoReg  *)0x400B0010U) /**< \brief (AESA) Interrupt Enable Register */
#define REG_AESA_IDR               (*(WoReg  *)0x400B0014U) /**< \brief (AESA) Interrupt Disable Register */
#define REG_AESA_IMR               (*(RoReg  *)0x400B0018U) /**< \brief (AESA) Interrupt Mask Register */
#define REG_AESA_KEY0              (*(WoReg  *)0x400B0020U) /**< \brief (AESA) Key Register 0 */
#define REG_AESA_KEY1              (*(WoReg  *)0x400B0024U) /**< \brief (AESA) Key Register 1 */
#define REG_AESA_KEY2              (*(WoReg  *)0x400B0028U) /**< \brief (AESA) Key Register 2 */
#define REG_AESA_KEY3              (*(WoReg  *)0x400B002CU) /**< \brief (AESA) Key Register 3 */
#define REG_AESA_KEY4              (*(WoReg  *)0x400B0030U) /**< \brief (AESA) Key Register 4 */
#define REG_AESA_KEY5              (*(WoReg  *)0x400B0034U) /**< \brief (AESA) Key Register 5 */
#define REG_AESA_KEY6              (*(WoReg  *)0x400B0038U) /**< \brief (AESA) Key Register 6 */
#define REG_AESA_KEY7              (*(WoReg  *)0x400B003CU) /**< \brief (AESA) Key Register 7 */
#define REG_AESA_INITVECT0         (*(WoReg  *)0x400B0040U) /**< \brief (AESA) Initialization Vector Register 0 */
#define REG_AESA_INITVECT1         (*(WoReg  *)0x400B0044U) /**< \brief (AESA) Initialization Vector Register 1 */
#define REG_AESA_INITVECT2         (*(WoReg  *)0x400B0048U) /**< \brief (AESA) Initialization Vector Register 2 */
#define REG_AESA_INITVECT3         (*(WoReg  *)0x400B004CU) /**< \brief (AESA) Initialization Vector Register 3 */
#define REG_AESA_IDATA             (*(WoReg  *)0x400B0050U) /**< \brief (AESA) Input Data Register */
#define REG_AESA_ODATA             (*(RoReg  *)0x400B0060U) /**< \brief (AESA) Output Data Register */
#define REG_AESA_DRNGSEED          (*(WoReg  *)0x400B0070U) /**< \brief (AESA) DRNG Seed Register */
#define REG_AESA_PARAMETER         (*(RoReg  *)0x400B00F8U) /**< \brief (AESA) Parameter Register */
#define REG_AESA_VERSION           (*(RoReg  *)0x400B00FCU) /**< \brief (AESA) Version Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/* ========== Instance parameters for AESA peripheral ========== */
#define AESA_DMAC_ID_RX             
#define AESA_DMAC_ID_TX             
#define AESA_GCLK_NUM               4
#define AESA_PDCA_ID_RX             17
#define AESA_PDCA_ID_TX             36

#endif /* _SAM4L_AESA_INSTANCE_ */
