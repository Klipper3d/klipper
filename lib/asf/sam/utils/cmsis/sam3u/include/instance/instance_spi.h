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

#ifndef _SAM3U_SPI_INSTANCE_
#define _SAM3U_SPI_INSTANCE_

/* ========== Register definition for SPI peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_SPI_CR              (0x40008000U) /**< \brief (SPI) Control Register */
#define REG_SPI_MR              (0x40008004U) /**< \brief (SPI) Mode Register */
#define REG_SPI_RDR             (0x40008008U) /**< \brief (SPI) Receive Data Register */
#define REG_SPI_TDR             (0x4000800CU) /**< \brief (SPI) Transmit Data Register */
#define REG_SPI_SR              (0x40008010U) /**< \brief (SPI) Status Register */
#define REG_SPI_IER             (0x40008014U) /**< \brief (SPI) Interrupt Enable Register */
#define REG_SPI_IDR             (0x40008018U) /**< \brief (SPI) Interrupt Disable Register */
#define REG_SPI_IMR             (0x4000801CU) /**< \brief (SPI) Interrupt Mask Register */
#define REG_SPI_CSR             (0x40008030U) /**< \brief (SPI) Chip Select Register */
#define REG_SPI_WPMR            (0x400080E4U) /**< \brief (SPI) Write Protection Control Register */
#define REG_SPI_WPSR            (0x400080E8U) /**< \brief (SPI) Write Protection Status Register */
#else
#define REG_SPI_CR     (*(WoReg*)0x40008000U) /**< \brief (SPI) Control Register */
#define REG_SPI_MR     (*(RwReg*)0x40008004U) /**< \brief (SPI) Mode Register */
#define REG_SPI_RDR    (*(RoReg*)0x40008008U) /**< \brief (SPI) Receive Data Register */
#define REG_SPI_TDR    (*(WoReg*)0x4000800CU) /**< \brief (SPI) Transmit Data Register */
#define REG_SPI_SR     (*(RoReg*)0x40008010U) /**< \brief (SPI) Status Register */
#define REG_SPI_IER    (*(WoReg*)0x40008014U) /**< \brief (SPI) Interrupt Enable Register */
#define REG_SPI_IDR    (*(WoReg*)0x40008018U) /**< \brief (SPI) Interrupt Disable Register */
#define REG_SPI_IMR    (*(RoReg*)0x4000801CU) /**< \brief (SPI) Interrupt Mask Register */
#define REG_SPI_CSR    (*(RwReg*)0x40008030U) /**< \brief (SPI) Chip Select Register */
#define REG_SPI_WPMR   (*(RwReg*)0x400080E4U) /**< \brief (SPI) Write Protection Control Register */
#define REG_SPI_WPSR   (*(RoReg*)0x400080E8U) /**< \brief (SPI) Write Protection Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAM3U_SPI_INSTANCE_ */
