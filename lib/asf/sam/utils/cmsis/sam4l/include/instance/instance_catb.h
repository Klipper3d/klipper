/**
 * \file
 *
 * \brief Instance description for CATB
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

#ifndef _SAM4L_CATB_INSTANCE_
#define _SAM4L_CATB_INSTANCE_

/* ========== Register definition for CATB peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_CATB_CR                (0x40070000U) /**< \brief (CATB) Control Register */
#define REG_CATB_CNTCR             (0x40070004U) /**< \brief (CATB) Counter Control Register */
#define REG_CATB_IDLE              (0x40070008U) /**< \brief (CATB) Sensor Idle Level */
#define REG_CATB_LEVEL             (0x4007000CU) /**< \brief (CATB) Sensor Relative Level */
#define REG_CATB_RAW               (0x40070010U) /**< \brief (CATB) Sensor Raw Value */
#define REG_CATB_TIMING            (0x40070014U) /**< \brief (CATB) Filter Timing Register */
#define REG_CATB_THRESH            (0x40070018U) /**< \brief (CATB) Threshold Register */
#define REG_CATB_PINSEL            (0x4007001CU) /**< \brief (CATB) Pin Selection Register */
#define REG_CATB_DMA               (0x40070020U) /**< \brief (CATB) Direct Memory Access Register */
#define REG_CATB_ISR               (0x40070024U) /**< \brief (CATB) Interrupt Status Register */
#define REG_CATB_IER               (0x40070028U) /**< \brief (CATB) Interrupt Enable Register */
#define REG_CATB_IDR               (0x4007002CU) /**< \brief (CATB) Interrupt Disable Register */
#define REG_CATB_IMR               (0x40070030U) /**< \brief (CATB) Interrupt Mask Register */
#define REG_CATB_SCR               (0x40070034U) /**< \brief (CATB) Status Clear Register */
#define REG_CATB_INTCH0            (0x40070040U) /**< \brief (CATB) In-Touch Status Register 0 */
#define REG_CATB_INTCHCLR0         (0x40070050U) /**< \brief (CATB) In-Touch Status Clear Register 0 */
#define REG_CATB_OUTTCH0           (0x40070060U) /**< \brief (CATB) Out-of-Touch Status Register 0 */
#define REG_CATB_OUTTCHCLR0        (0x40070070U) /**< \brief (CATB) Out-of-Touch Status Clear Register 0 */
#define REG_CATB_PARAMETER         (0x400700F8U) /**< \brief (CATB) Parameter Register */
#define REG_CATB_VERSION           (0x400700FCU) /**< \brief (CATB) Version Register */
#else
#define REG_CATB_CR                (*(RwReg  *)0x40070000U) /**< \brief (CATB) Control Register */
#define REG_CATB_CNTCR             (*(RwReg  *)0x40070004U) /**< \brief (CATB) Counter Control Register */
#define REG_CATB_IDLE              (*(RwReg  *)0x40070008U) /**< \brief (CATB) Sensor Idle Level */
#define REG_CATB_LEVEL             (*(RoReg  *)0x4007000CU) /**< \brief (CATB) Sensor Relative Level */
#define REG_CATB_RAW               (*(RoReg  *)0x40070010U) /**< \brief (CATB) Sensor Raw Value */
#define REG_CATB_TIMING            (*(RwReg  *)0x40070014U) /**< \brief (CATB) Filter Timing Register */
#define REG_CATB_THRESH            (*(RwReg  *)0x40070018U) /**< \brief (CATB) Threshold Register */
#define REG_CATB_PINSEL            (*(RwReg  *)0x4007001CU) /**< \brief (CATB) Pin Selection Register */
#define REG_CATB_DMA               (*(RwReg  *)0x40070020U) /**< \brief (CATB) Direct Memory Access Register */
#define REG_CATB_ISR               (*(RoReg  *)0x40070024U) /**< \brief (CATB) Interrupt Status Register */
#define REG_CATB_IER               (*(WoReg  *)0x40070028U) /**< \brief (CATB) Interrupt Enable Register */
#define REG_CATB_IDR               (*(WoReg  *)0x4007002CU) /**< \brief (CATB) Interrupt Disable Register */
#define REG_CATB_IMR               (*(RoReg  *)0x40070030U) /**< \brief (CATB) Interrupt Mask Register */
#define REG_CATB_SCR               (*(WoReg  *)0x40070034U) /**< \brief (CATB) Status Clear Register */
#define REG_CATB_INTCH0            (*(RoReg  *)0x40070040U) /**< \brief (CATB) In-Touch Status Register 0 */
#define REG_CATB_INTCHCLR0         (*(WoReg  *)0x40070050U) /**< \brief (CATB) In-Touch Status Clear Register 0 */
#define REG_CATB_OUTTCH0           (*(RoReg  *)0x40070060U) /**< \brief (CATB) Out-of-Touch Status Register 0 */
#define REG_CATB_OUTTCHCLR0        (*(WoReg  *)0x40070070U) /**< \brief (CATB) Out-of-Touch Status Clear Register 0 */
#define REG_CATB_PARAMETER         (*(RoReg  *)0x400700F8U) /**< \brief (CATB) Parameter Register */
#define REG_CATB_VERSION           (*(RoReg  *)0x400700FCU) /**< \brief (CATB) Version Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/* ========== Instance parameters for CATB peripheral ========== */
#define CATB_GCLK_NUM               3
#define CATB_PDCA_ID_RX             12
#define CATB_PDCA_ID_TX             30
#define CATB_SENSORS_MSB            31
#define CATB_STATUS_REG_NUMBER      1

#endif /* _SAM4L_CATB_INSTANCE_ */
