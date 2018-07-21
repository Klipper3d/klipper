/**
 * \file
 *
 * \brief Instance description for PDCA
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

#ifndef _SAM4L_PDCA_INSTANCE_
#define _SAM4L_PDCA_INSTANCE_

/* ========== Register definition for PDCA peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_PDCA_MAR0              (0x400A2000U) /**< \brief (PDCA) Memory Address Register 0 */
#define REG_PDCA_PSR0              (0x400A2004U) /**< \brief (PDCA) Peripheral Select Register 0 */
#define REG_PDCA_TCR0              (0x400A2008U) /**< \brief (PDCA) Transfer Counter Register 0 */
#define REG_PDCA_MARR0             (0x400A200CU) /**< \brief (PDCA) Memory Address Reload Register 0 */
#define REG_PDCA_TCRR0             (0x400A2010U) /**< \brief (PDCA) Transfer Counter Reload Register 0 */
#define REG_PDCA_CR0               (0x400A2014U) /**< \brief (PDCA) Control Register 0 */
#define REG_PDCA_MR0               (0x400A2018U) /**< \brief (PDCA) Mode Register 0 */
#define REG_PDCA_SR0               (0x400A201CU) /**< \brief (PDCA) Status Register 0 */
#define REG_PDCA_IER0              (0x400A2020U) /**< \brief (PDCA) Interrupt Enable Register 0 */
#define REG_PDCA_IDR0              (0x400A2024U) /**< \brief (PDCA) Interrupt Disable Register 0 */
#define REG_PDCA_IMR0              (0x400A2028U) /**< \brief (PDCA) Interrupt Mask Register 0 */
#define REG_PDCA_ISR0              (0x400A202CU) /**< \brief (PDCA) Interrupt Status Register 0 */
#define REG_PDCA_MAR1              (0x400A2040U) /**< \brief (PDCA) Memory Address Register 1 */
#define REG_PDCA_PSR1              (0x400A2044U) /**< \brief (PDCA) Peripheral Select Register 1 */
#define REG_PDCA_TCR1              (0x400A2048U) /**< \brief (PDCA) Transfer Counter Register 1 */
#define REG_PDCA_MARR1             (0x400A204CU) /**< \brief (PDCA) Memory Address Reload Register 1 */
#define REG_PDCA_TCRR1             (0x400A2050U) /**< \brief (PDCA) Transfer Counter Reload Register 1 */
#define REG_PDCA_CR1               (0x400A2054U) /**< \brief (PDCA) Control Register 1 */
#define REG_PDCA_MR1               (0x400A2058U) /**< \brief (PDCA) Mode Register 1 */
#define REG_PDCA_SR1               (0x400A205CU) /**< \brief (PDCA) Status Register 1 */
#define REG_PDCA_IER1              (0x400A2060U) /**< \brief (PDCA) Interrupt Enable Register 1 */
#define REG_PDCA_IDR1              (0x400A2064U) /**< \brief (PDCA) Interrupt Disable Register 1 */
#define REG_PDCA_IMR1              (0x400A2068U) /**< \brief (PDCA) Interrupt Mask Register 1 */
#define REG_PDCA_ISR1              (0x400A206CU) /**< \brief (PDCA) Interrupt Status Register 1 */
#define REG_PDCA_MAR2              (0x400A2080U) /**< \brief (PDCA) Memory Address Register 2 */
#define REG_PDCA_PSR2              (0x400A2084U) /**< \brief (PDCA) Peripheral Select Register 2 */
#define REG_PDCA_TCR2              (0x400A2088U) /**< \brief (PDCA) Transfer Counter Register 2 */
#define REG_PDCA_MARR2             (0x400A208CU) /**< \brief (PDCA) Memory Address Reload Register 2 */
#define REG_PDCA_TCRR2             (0x400A2090U) /**< \brief (PDCA) Transfer Counter Reload Register 2 */
#define REG_PDCA_CR2               (0x400A2094U) /**< \brief (PDCA) Control Register 2 */
#define REG_PDCA_MR2               (0x400A2098U) /**< \brief (PDCA) Mode Register 2 */
#define REG_PDCA_SR2               (0x400A209CU) /**< \brief (PDCA) Status Register 2 */
#define REG_PDCA_IER2              (0x400A20A0U) /**< \brief (PDCA) Interrupt Enable Register 2 */
#define REG_PDCA_IDR2              (0x400A20A4U) /**< \brief (PDCA) Interrupt Disable Register 2 */
#define REG_PDCA_IMR2              (0x400A20A8U) /**< \brief (PDCA) Interrupt Mask Register 2 */
#define REG_PDCA_ISR2              (0x400A20ACU) /**< \brief (PDCA) Interrupt Status Register 2 */
#define REG_PDCA_MAR3              (0x400A20C0U) /**< \brief (PDCA) Memory Address Register 3 */
#define REG_PDCA_PSR3              (0x400A20C4U) /**< \brief (PDCA) Peripheral Select Register 3 */
#define REG_PDCA_TCR3              (0x400A20C8U) /**< \brief (PDCA) Transfer Counter Register 3 */
#define REG_PDCA_MARR3             (0x400A20CCU) /**< \brief (PDCA) Memory Address Reload Register 3 */
#define REG_PDCA_TCRR3             (0x400A20D0U) /**< \brief (PDCA) Transfer Counter Reload Register 3 */
#define REG_PDCA_CR3               (0x400A20D4U) /**< \brief (PDCA) Control Register 3 */
#define REG_PDCA_MR3               (0x400A20D8U) /**< \brief (PDCA) Mode Register 3 */
#define REG_PDCA_SR3               (0x400A20DCU) /**< \brief (PDCA) Status Register 3 */
#define REG_PDCA_IER3              (0x400A20E0U) /**< \brief (PDCA) Interrupt Enable Register 3 */
#define REG_PDCA_IDR3              (0x400A20E4U) /**< \brief (PDCA) Interrupt Disable Register 3 */
#define REG_PDCA_IMR3              (0x400A20E8U) /**< \brief (PDCA) Interrupt Mask Register 3 */
#define REG_PDCA_ISR3              (0x400A20ECU) /**< \brief (PDCA) Interrupt Status Register 3 */
#define REG_PDCA_MAR4              (0x400A2100U) /**< \brief (PDCA) Memory Address Register 4 */
#define REG_PDCA_PSR4              (0x400A2104U) /**< \brief (PDCA) Peripheral Select Register 4 */
#define REG_PDCA_TCR4              (0x400A2108U) /**< \brief (PDCA) Transfer Counter Register 4 */
#define REG_PDCA_MARR4             (0x400A210CU) /**< \brief (PDCA) Memory Address Reload Register 4 */
#define REG_PDCA_TCRR4             (0x400A2110U) /**< \brief (PDCA) Transfer Counter Reload Register 4 */
#define REG_PDCA_CR4               (0x400A2114U) /**< \brief (PDCA) Control Register 4 */
#define REG_PDCA_MR4               (0x400A2118U) /**< \brief (PDCA) Mode Register 4 */
#define REG_PDCA_SR4               (0x400A211CU) /**< \brief (PDCA) Status Register 4 */
#define REG_PDCA_IER4              (0x400A2120U) /**< \brief (PDCA) Interrupt Enable Register 4 */
#define REG_PDCA_IDR4              (0x400A2124U) /**< \brief (PDCA) Interrupt Disable Register 4 */
#define REG_PDCA_IMR4              (0x400A2128U) /**< \brief (PDCA) Interrupt Mask Register 4 */
#define REG_PDCA_ISR4              (0x400A212CU) /**< \brief (PDCA) Interrupt Status Register 4 */
#define REG_PDCA_MAR5              (0x400A2140U) /**< \brief (PDCA) Memory Address Register 5 */
#define REG_PDCA_PSR5              (0x400A2144U) /**< \brief (PDCA) Peripheral Select Register 5 */
#define REG_PDCA_TCR5              (0x400A2148U) /**< \brief (PDCA) Transfer Counter Register 5 */
#define REG_PDCA_MARR5             (0x400A214CU) /**< \brief (PDCA) Memory Address Reload Register 5 */
#define REG_PDCA_TCRR5             (0x400A2150U) /**< \brief (PDCA) Transfer Counter Reload Register 5 */
#define REG_PDCA_CR5               (0x400A2154U) /**< \brief (PDCA) Control Register 5 */
#define REG_PDCA_MR5               (0x400A2158U) /**< \brief (PDCA) Mode Register 5 */
#define REG_PDCA_SR5               (0x400A215CU) /**< \brief (PDCA) Status Register 5 */
#define REG_PDCA_IER5              (0x400A2160U) /**< \brief (PDCA) Interrupt Enable Register 5 */
#define REG_PDCA_IDR5              (0x400A2164U) /**< \brief (PDCA) Interrupt Disable Register 5 */
#define REG_PDCA_IMR5              (0x400A2168U) /**< \brief (PDCA) Interrupt Mask Register 5 */
#define REG_PDCA_ISR5              (0x400A216CU) /**< \brief (PDCA) Interrupt Status Register 5 */
#define REG_PDCA_MAR6              (0x400A2180U) /**< \brief (PDCA) Memory Address Register 6 */
#define REG_PDCA_PSR6              (0x400A2184U) /**< \brief (PDCA) Peripheral Select Register 6 */
#define REG_PDCA_TCR6              (0x400A2188U) /**< \brief (PDCA) Transfer Counter Register 6 */
#define REG_PDCA_MARR6             (0x400A218CU) /**< \brief (PDCA) Memory Address Reload Register 6 */
#define REG_PDCA_TCRR6             (0x400A2190U) /**< \brief (PDCA) Transfer Counter Reload Register 6 */
#define REG_PDCA_CR6               (0x400A2194U) /**< \brief (PDCA) Control Register 6 */
#define REG_PDCA_MR6               (0x400A2198U) /**< \brief (PDCA) Mode Register 6 */
#define REG_PDCA_SR6               (0x400A219CU) /**< \brief (PDCA) Status Register 6 */
#define REG_PDCA_IER6              (0x400A21A0U) /**< \brief (PDCA) Interrupt Enable Register 6 */
#define REG_PDCA_IDR6              (0x400A21A4U) /**< \brief (PDCA) Interrupt Disable Register 6 */
#define REG_PDCA_IMR6              (0x400A21A8U) /**< \brief (PDCA) Interrupt Mask Register 6 */
#define REG_PDCA_ISR6              (0x400A21ACU) /**< \brief (PDCA) Interrupt Status Register 6 */
#define REG_PDCA_MAR7              (0x400A21C0U) /**< \brief (PDCA) Memory Address Register 7 */
#define REG_PDCA_PSR7              (0x400A21C4U) /**< \brief (PDCA) Peripheral Select Register 7 */
#define REG_PDCA_TCR7              (0x400A21C8U) /**< \brief (PDCA) Transfer Counter Register 7 */
#define REG_PDCA_MARR7             (0x400A21CCU) /**< \brief (PDCA) Memory Address Reload Register 7 */
#define REG_PDCA_TCRR7             (0x400A21D0U) /**< \brief (PDCA) Transfer Counter Reload Register 7 */
#define REG_PDCA_CR7               (0x400A21D4U) /**< \brief (PDCA) Control Register 7 */
#define REG_PDCA_MR7               (0x400A21D8U) /**< \brief (PDCA) Mode Register 7 */
#define REG_PDCA_SR7               (0x400A21DCU) /**< \brief (PDCA) Status Register 7 */
#define REG_PDCA_IER7              (0x400A21E0U) /**< \brief (PDCA) Interrupt Enable Register 7 */
#define REG_PDCA_IDR7              (0x400A21E4U) /**< \brief (PDCA) Interrupt Disable Register 7 */
#define REG_PDCA_IMR7              (0x400A21E8U) /**< \brief (PDCA) Interrupt Mask Register 7 */
#define REG_PDCA_ISR7              (0x400A21ECU) /**< \brief (PDCA) Interrupt Status Register 7 */
#define REG_PDCA_MAR8              (0x400A2200U) /**< \brief (PDCA) Memory Address Register 8 */
#define REG_PDCA_PSR8              (0x400A2204U) /**< \brief (PDCA) Peripheral Select Register 8 */
#define REG_PDCA_TCR8              (0x400A2208U) /**< \brief (PDCA) Transfer Counter Register 8 */
#define REG_PDCA_MARR8             (0x400A220CU) /**< \brief (PDCA) Memory Address Reload Register 8 */
#define REG_PDCA_TCRR8             (0x400A2210U) /**< \brief (PDCA) Transfer Counter Reload Register 8 */
#define REG_PDCA_CR8               (0x400A2214U) /**< \brief (PDCA) Control Register 8 */
#define REG_PDCA_MR8               (0x400A2218U) /**< \brief (PDCA) Mode Register 8 */
#define REG_PDCA_SR8               (0x400A221CU) /**< \brief (PDCA) Status Register 8 */
#define REG_PDCA_IER8              (0x400A2220U) /**< \brief (PDCA) Interrupt Enable Register 8 */
#define REG_PDCA_IDR8              (0x400A2224U) /**< \brief (PDCA) Interrupt Disable Register 8 */
#define REG_PDCA_IMR8              (0x400A2228U) /**< \brief (PDCA) Interrupt Mask Register 8 */
#define REG_PDCA_ISR8              (0x400A222CU) /**< \brief (PDCA) Interrupt Status Register 8 */
#define REG_PDCA_MAR9              (0x400A2240U) /**< \brief (PDCA) Memory Address Register 9 */
#define REG_PDCA_PSR9              (0x400A2244U) /**< \brief (PDCA) Peripheral Select Register 9 */
#define REG_PDCA_TCR9              (0x400A2248U) /**< \brief (PDCA) Transfer Counter Register 9 */
#define REG_PDCA_MARR9             (0x400A224CU) /**< \brief (PDCA) Memory Address Reload Register 9 */
#define REG_PDCA_TCRR9             (0x400A2250U) /**< \brief (PDCA) Transfer Counter Reload Register 9 */
#define REG_PDCA_CR9               (0x400A2254U) /**< \brief (PDCA) Control Register 9 */
#define REG_PDCA_MR9               (0x400A2258U) /**< \brief (PDCA) Mode Register 9 */
#define REG_PDCA_SR9               (0x400A225CU) /**< \brief (PDCA) Status Register 9 */
#define REG_PDCA_IER9              (0x400A2260U) /**< \brief (PDCA) Interrupt Enable Register 9 */
#define REG_PDCA_IDR9              (0x400A2264U) /**< \brief (PDCA) Interrupt Disable Register 9 */
#define REG_PDCA_IMR9              (0x400A2268U) /**< \brief (PDCA) Interrupt Mask Register 9 */
#define REG_PDCA_ISR9              (0x400A226CU) /**< \brief (PDCA) Interrupt Status Register 9 */
#define REG_PDCA_MAR10             (0x400A2280U) /**< \brief (PDCA) Memory Address Register 10 */
#define REG_PDCA_PSR10             (0x400A2284U) /**< \brief (PDCA) Peripheral Select Register 10 */
#define REG_PDCA_TCR10             (0x400A2288U) /**< \brief (PDCA) Transfer Counter Register 10 */
#define REG_PDCA_MARR10            (0x400A228CU) /**< \brief (PDCA) Memory Address Reload Register 10 */
#define REG_PDCA_TCRR10            (0x400A2290U) /**< \brief (PDCA) Transfer Counter Reload Register 10 */
#define REG_PDCA_CR10              (0x400A2294U) /**< \brief (PDCA) Control Register 10 */
#define REG_PDCA_MR10              (0x400A2298U) /**< \brief (PDCA) Mode Register 10 */
#define REG_PDCA_SR10              (0x400A229CU) /**< \brief (PDCA) Status Register 10 */
#define REG_PDCA_IER10             (0x400A22A0U) /**< \brief (PDCA) Interrupt Enable Register 10 */
#define REG_PDCA_IDR10             (0x400A22A4U) /**< \brief (PDCA) Interrupt Disable Register 10 */
#define REG_PDCA_IMR10             (0x400A22A8U) /**< \brief (PDCA) Interrupt Mask Register 10 */
#define REG_PDCA_ISR10             (0x400A22ACU) /**< \brief (PDCA) Interrupt Status Register 10 */
#define REG_PDCA_MAR11             (0x400A22C0U) /**< \brief (PDCA) Memory Address Register 11 */
#define REG_PDCA_PSR11             (0x400A22C4U) /**< \brief (PDCA) Peripheral Select Register 11 */
#define REG_PDCA_TCR11             (0x400A22C8U) /**< \brief (PDCA) Transfer Counter Register 11 */
#define REG_PDCA_MARR11            (0x400A22CCU) /**< \brief (PDCA) Memory Address Reload Register 11 */
#define REG_PDCA_TCRR11            (0x400A22D0U) /**< \brief (PDCA) Transfer Counter Reload Register 11 */
#define REG_PDCA_CR11              (0x400A22D4U) /**< \brief (PDCA) Control Register 11 */
#define REG_PDCA_MR11              (0x400A22D8U) /**< \brief (PDCA) Mode Register 11 */
#define REG_PDCA_SR11              (0x400A22DCU) /**< \brief (PDCA) Status Register 11 */
#define REG_PDCA_IER11             (0x400A22E0U) /**< \brief (PDCA) Interrupt Enable Register 11 */
#define REG_PDCA_IDR11             (0x400A22E4U) /**< \brief (PDCA) Interrupt Disable Register 11 */
#define REG_PDCA_IMR11             (0x400A22E8U) /**< \brief (PDCA) Interrupt Mask Register 11 */
#define REG_PDCA_ISR11             (0x400A22ECU) /**< \brief (PDCA) Interrupt Status Register 11 */
#define REG_PDCA_MAR12             (0x400A2300U) /**< \brief (PDCA) Memory Address Register 12 */
#define REG_PDCA_PSR12             (0x400A2304U) /**< \brief (PDCA) Peripheral Select Register 12 */
#define REG_PDCA_TCR12             (0x400A2308U) /**< \brief (PDCA) Transfer Counter Register 12 */
#define REG_PDCA_MARR12            (0x400A230CU) /**< \brief (PDCA) Memory Address Reload Register 12 */
#define REG_PDCA_TCRR12            (0x400A2310U) /**< \brief (PDCA) Transfer Counter Reload Register 12 */
#define REG_PDCA_CR12              (0x400A2314U) /**< \brief (PDCA) Control Register 12 */
#define REG_PDCA_MR12              (0x400A2318U) /**< \brief (PDCA) Mode Register 12 */
#define REG_PDCA_SR12              (0x400A231CU) /**< \brief (PDCA) Status Register 12 */
#define REG_PDCA_IER12             (0x400A2320U) /**< \brief (PDCA) Interrupt Enable Register 12 */
#define REG_PDCA_IDR12             (0x400A2324U) /**< \brief (PDCA) Interrupt Disable Register 12 */
#define REG_PDCA_IMR12             (0x400A2328U) /**< \brief (PDCA) Interrupt Mask Register 12 */
#define REG_PDCA_ISR12             (0x400A232CU) /**< \brief (PDCA) Interrupt Status Register 12 */
#define REG_PDCA_MAR13             (0x400A2340U) /**< \brief (PDCA) Memory Address Register 13 */
#define REG_PDCA_PSR13             (0x400A2344U) /**< \brief (PDCA) Peripheral Select Register 13 */
#define REG_PDCA_TCR13             (0x400A2348U) /**< \brief (PDCA) Transfer Counter Register 13 */
#define REG_PDCA_MARR13            (0x400A234CU) /**< \brief (PDCA) Memory Address Reload Register 13 */
#define REG_PDCA_TCRR13            (0x400A2350U) /**< \brief (PDCA) Transfer Counter Reload Register 13 */
#define REG_PDCA_CR13              (0x400A2354U) /**< \brief (PDCA) Control Register 13 */
#define REG_PDCA_MR13              (0x400A2358U) /**< \brief (PDCA) Mode Register 13 */
#define REG_PDCA_SR13              (0x400A235CU) /**< \brief (PDCA) Status Register 13 */
#define REG_PDCA_IER13             (0x400A2360U) /**< \brief (PDCA) Interrupt Enable Register 13 */
#define REG_PDCA_IDR13             (0x400A2364U) /**< \brief (PDCA) Interrupt Disable Register 13 */
#define REG_PDCA_IMR13             (0x400A2368U) /**< \brief (PDCA) Interrupt Mask Register 13 */
#define REG_PDCA_ISR13             (0x400A236CU) /**< \brief (PDCA) Interrupt Status Register 13 */
#define REG_PDCA_MAR14             (0x400A2380U) /**< \brief (PDCA) Memory Address Register 14 */
#define REG_PDCA_PSR14             (0x400A2384U) /**< \brief (PDCA) Peripheral Select Register 14 */
#define REG_PDCA_TCR14             (0x400A2388U) /**< \brief (PDCA) Transfer Counter Register 14 */
#define REG_PDCA_MARR14            (0x400A238CU) /**< \brief (PDCA) Memory Address Reload Register 14 */
#define REG_PDCA_TCRR14            (0x400A2390U) /**< \brief (PDCA) Transfer Counter Reload Register 14 */
#define REG_PDCA_CR14              (0x400A2394U) /**< \brief (PDCA) Control Register 14 */
#define REG_PDCA_MR14              (0x400A2398U) /**< \brief (PDCA) Mode Register 14 */
#define REG_PDCA_SR14              (0x400A239CU) /**< \brief (PDCA) Status Register 14 */
#define REG_PDCA_IER14             (0x400A23A0U) /**< \brief (PDCA) Interrupt Enable Register 14 */
#define REG_PDCA_IDR14             (0x400A23A4U) /**< \brief (PDCA) Interrupt Disable Register 14 */
#define REG_PDCA_IMR14             (0x400A23A8U) /**< \brief (PDCA) Interrupt Mask Register 14 */
#define REG_PDCA_ISR14             (0x400A23ACU) /**< \brief (PDCA) Interrupt Status Register 14 */
#define REG_PDCA_MAR15             (0x400A23C0U) /**< \brief (PDCA) Memory Address Register 15 */
#define REG_PDCA_PSR15             (0x400A23C4U) /**< \brief (PDCA) Peripheral Select Register 15 */
#define REG_PDCA_TCR15             (0x400A23C8U) /**< \brief (PDCA) Transfer Counter Register 15 */
#define REG_PDCA_MARR15            (0x400A23CCU) /**< \brief (PDCA) Memory Address Reload Register 15 */
#define REG_PDCA_TCRR15            (0x400A23D0U) /**< \brief (PDCA) Transfer Counter Reload Register 15 */
#define REG_PDCA_CR15              (0x400A23D4U) /**< \brief (PDCA) Control Register 15 */
#define REG_PDCA_MR15              (0x400A23D8U) /**< \brief (PDCA) Mode Register 15 */
#define REG_PDCA_SR15              (0x400A23DCU) /**< \brief (PDCA) Status Register 15 */
#define REG_PDCA_IER15             (0x400A23E0U) /**< \brief (PDCA) Interrupt Enable Register 15 */
#define REG_PDCA_IDR15             (0x400A23E4U) /**< \brief (PDCA) Interrupt Disable Register 15 */
#define REG_PDCA_IMR15             (0x400A23E8U) /**< \brief (PDCA) Interrupt Mask Register 15 */
#define REG_PDCA_ISR15             (0x400A23ECU) /**< \brief (PDCA) Interrupt Status Register 15 */
#define REG_PDCA_PCONTROL          (0x400A2800U) /**< \brief (PDCA) Performance Control Register */
#define REG_PDCA_PRDATA0           (0x400A2804U) /**< \brief (PDCA) Channel 0 Read Data Cycles */
#define REG_PDCA_PRSTALL0          (0x400A2808U) /**< \brief (PDCA) Channel 0 Read Stall Cycles */
#define REG_PDCA_PRLAT0            (0x400A280CU) /**< \brief (PDCA) Channel 0 Read Max Latency */
#define REG_PDCA_PWDATA0           (0x400A2810U) /**< \brief (PDCA) Channel 0 Write Data Cycles */
#define REG_PDCA_PWSTALL0          (0x400A2814U) /**< \brief (PDCA) Channel 0 Write Stall Cycles */
#define REG_PDCA_PWLAT0            (0x400A2818U) /**< \brief (PDCA) Channel0 Write Max Latency */
#define REG_PDCA_PRDATA1           (0x400A281CU) /**< \brief (PDCA) Channel 1 Read Data Cycles */
#define REG_PDCA_PRSTALL1          (0x400A2820U) /**< \brief (PDCA) Channel Read Stall Cycles */
#define REG_PDCA_PRLAT1            (0x400A2824U) /**< \brief (PDCA) Channel 1 Read Max Latency */
#define REG_PDCA_PWDATA1           (0x400A2828U) /**< \brief (PDCA) Channel 1 Write Data Cycles */
#define REG_PDCA_PWSTALL1          (0x400A282CU) /**< \brief (PDCA) Channel 1 Write stall Cycles */
#define REG_PDCA_PWLAT1            (0x400A2830U) /**< \brief (PDCA) Channel 1 Read Max Latency */
#define REG_PDCA_VERSION           (0x400A2834U) /**< \brief (PDCA) Version Register */
#else
#define REG_PDCA_MAR0              (*(RwReg  *)0x400A2000U) /**< \brief (PDCA) Memory Address Register 0 */
#define REG_PDCA_PSR0              (*(RwReg  *)0x400A2004U) /**< \brief (PDCA) Peripheral Select Register 0 */
#define REG_PDCA_TCR0              (*(RwReg  *)0x400A2008U) /**< \brief (PDCA) Transfer Counter Register 0 */
#define REG_PDCA_MARR0             (*(RwReg  *)0x400A200CU) /**< \brief (PDCA) Memory Address Reload Register 0 */
#define REG_PDCA_TCRR0             (*(RwReg  *)0x400A2010U) /**< \brief (PDCA) Transfer Counter Reload Register 0 */
#define REG_PDCA_CR0               (*(WoReg  *)0x400A2014U) /**< \brief (PDCA) Control Register 0 */
#define REG_PDCA_MR0               (*(RwReg  *)0x400A2018U) /**< \brief (PDCA) Mode Register 0 */
#define REG_PDCA_SR0               (*(RoReg  *)0x400A201CU) /**< \brief (PDCA) Status Register 0 */
#define REG_PDCA_IER0              (*(WoReg  *)0x400A2020U) /**< \brief (PDCA) Interrupt Enable Register 0 */
#define REG_PDCA_IDR0              (*(WoReg  *)0x400A2024U) /**< \brief (PDCA) Interrupt Disable Register 0 */
#define REG_PDCA_IMR0              (*(RoReg  *)0x400A2028U) /**< \brief (PDCA) Interrupt Mask Register 0 */
#define REG_PDCA_ISR0              (*(RoReg  *)0x400A202CU) /**< \brief (PDCA) Interrupt Status Register 0 */
#define REG_PDCA_MAR1              (*(RwReg  *)0x400A2040U) /**< \brief (PDCA) Memory Address Register 1 */
#define REG_PDCA_PSR1              (*(RwReg  *)0x400A2044U) /**< \brief (PDCA) Peripheral Select Register 1 */
#define REG_PDCA_TCR1              (*(RwReg  *)0x400A2048U) /**< \brief (PDCA) Transfer Counter Register 1 */
#define REG_PDCA_MARR1             (*(RwReg  *)0x400A204CU) /**< \brief (PDCA) Memory Address Reload Register 1 */
#define REG_PDCA_TCRR1             (*(RwReg  *)0x400A2050U) /**< \brief (PDCA) Transfer Counter Reload Register 1 */
#define REG_PDCA_CR1               (*(WoReg  *)0x400A2054U) /**< \brief (PDCA) Control Register 1 */
#define REG_PDCA_MR1               (*(RwReg  *)0x400A2058U) /**< \brief (PDCA) Mode Register 1 */
#define REG_PDCA_SR1               (*(RoReg  *)0x400A205CU) /**< \brief (PDCA) Status Register 1 */
#define REG_PDCA_IER1              (*(WoReg  *)0x400A2060U) /**< \brief (PDCA) Interrupt Enable Register 1 */
#define REG_PDCA_IDR1              (*(WoReg  *)0x400A2064U) /**< \brief (PDCA) Interrupt Disable Register 1 */
#define REG_PDCA_IMR1              (*(RoReg  *)0x400A2068U) /**< \brief (PDCA) Interrupt Mask Register 1 */
#define REG_PDCA_ISR1              (*(RoReg  *)0x400A206CU) /**< \brief (PDCA) Interrupt Status Register 1 */
#define REG_PDCA_MAR2              (*(RwReg  *)0x400A2080U) /**< \brief (PDCA) Memory Address Register 2 */
#define REG_PDCA_PSR2              (*(RwReg  *)0x400A2084U) /**< \brief (PDCA) Peripheral Select Register 2 */
#define REG_PDCA_TCR2              (*(RwReg  *)0x400A2088U) /**< \brief (PDCA) Transfer Counter Register 2 */
#define REG_PDCA_MARR2             (*(RwReg  *)0x400A208CU) /**< \brief (PDCA) Memory Address Reload Register 2 */
#define REG_PDCA_TCRR2             (*(RwReg  *)0x400A2090U) /**< \brief (PDCA) Transfer Counter Reload Register 2 */
#define REG_PDCA_CR2               (*(WoReg  *)0x400A2094U) /**< \brief (PDCA) Control Register 2 */
#define REG_PDCA_MR2               (*(RwReg  *)0x400A2098U) /**< \brief (PDCA) Mode Register 2 */
#define REG_PDCA_SR2               (*(RoReg  *)0x400A209CU) /**< \brief (PDCA) Status Register 2 */
#define REG_PDCA_IER2              (*(WoReg  *)0x400A20A0U) /**< \brief (PDCA) Interrupt Enable Register 2 */
#define REG_PDCA_IDR2              (*(WoReg  *)0x400A20A4U) /**< \brief (PDCA) Interrupt Disable Register 2 */
#define REG_PDCA_IMR2              (*(RoReg  *)0x400A20A8U) /**< \brief (PDCA) Interrupt Mask Register 2 */
#define REG_PDCA_ISR2              (*(RoReg  *)0x400A20ACU) /**< \brief (PDCA) Interrupt Status Register 2 */
#define REG_PDCA_MAR3              (*(RwReg  *)0x400A20C0U) /**< \brief (PDCA) Memory Address Register 3 */
#define REG_PDCA_PSR3              (*(RwReg  *)0x400A20C4U) /**< \brief (PDCA) Peripheral Select Register 3 */
#define REG_PDCA_TCR3              (*(RwReg  *)0x400A20C8U) /**< \brief (PDCA) Transfer Counter Register 3 */
#define REG_PDCA_MARR3             (*(RwReg  *)0x400A20CCU) /**< \brief (PDCA) Memory Address Reload Register 3 */
#define REG_PDCA_TCRR3             (*(RwReg  *)0x400A20D0U) /**< \brief (PDCA) Transfer Counter Reload Register 3 */
#define REG_PDCA_CR3               (*(WoReg  *)0x400A20D4U) /**< \brief (PDCA) Control Register 3 */
#define REG_PDCA_MR3               (*(RwReg  *)0x400A20D8U) /**< \brief (PDCA) Mode Register 3 */
#define REG_PDCA_SR3               (*(RoReg  *)0x400A20DCU) /**< \brief (PDCA) Status Register 3 */
#define REG_PDCA_IER3              (*(WoReg  *)0x400A20E0U) /**< \brief (PDCA) Interrupt Enable Register 3 */
#define REG_PDCA_IDR3              (*(WoReg  *)0x400A20E4U) /**< \brief (PDCA) Interrupt Disable Register 3 */
#define REG_PDCA_IMR3              (*(RoReg  *)0x400A20E8U) /**< \brief (PDCA) Interrupt Mask Register 3 */
#define REG_PDCA_ISR3              (*(RoReg  *)0x400A20ECU) /**< \brief (PDCA) Interrupt Status Register 3 */
#define REG_PDCA_MAR4              (*(RwReg  *)0x400A2100U) /**< \brief (PDCA) Memory Address Register 4 */
#define REG_PDCA_PSR4              (*(RwReg  *)0x400A2104U) /**< \brief (PDCA) Peripheral Select Register 4 */
#define REG_PDCA_TCR4              (*(RwReg  *)0x400A2108U) /**< \brief (PDCA) Transfer Counter Register 4 */
#define REG_PDCA_MARR4             (*(RwReg  *)0x400A210CU) /**< \brief (PDCA) Memory Address Reload Register 4 */
#define REG_PDCA_TCRR4             (*(RwReg  *)0x400A2110U) /**< \brief (PDCA) Transfer Counter Reload Register 4 */
#define REG_PDCA_CR4               (*(WoReg  *)0x400A2114U) /**< \brief (PDCA) Control Register 4 */
#define REG_PDCA_MR4               (*(RwReg  *)0x400A2118U) /**< \brief (PDCA) Mode Register 4 */
#define REG_PDCA_SR4               (*(RoReg  *)0x400A211CU) /**< \brief (PDCA) Status Register 4 */
#define REG_PDCA_IER4              (*(WoReg  *)0x400A2120U) /**< \brief (PDCA) Interrupt Enable Register 4 */
#define REG_PDCA_IDR4              (*(WoReg  *)0x400A2124U) /**< \brief (PDCA) Interrupt Disable Register 4 */
#define REG_PDCA_IMR4              (*(RoReg  *)0x400A2128U) /**< \brief (PDCA) Interrupt Mask Register 4 */
#define REG_PDCA_ISR4              (*(RoReg  *)0x400A212CU) /**< \brief (PDCA) Interrupt Status Register 4 */
#define REG_PDCA_MAR5              (*(RwReg  *)0x400A2140U) /**< \brief (PDCA) Memory Address Register 5 */
#define REG_PDCA_PSR5              (*(RwReg  *)0x400A2144U) /**< \brief (PDCA) Peripheral Select Register 5 */
#define REG_PDCA_TCR5              (*(RwReg  *)0x400A2148U) /**< \brief (PDCA) Transfer Counter Register 5 */
#define REG_PDCA_MARR5             (*(RwReg  *)0x400A214CU) /**< \brief (PDCA) Memory Address Reload Register 5 */
#define REG_PDCA_TCRR5             (*(RwReg  *)0x400A2150U) /**< \brief (PDCA) Transfer Counter Reload Register 5 */
#define REG_PDCA_CR5               (*(WoReg  *)0x400A2154U) /**< \brief (PDCA) Control Register 5 */
#define REG_PDCA_MR5               (*(RwReg  *)0x400A2158U) /**< \brief (PDCA) Mode Register 5 */
#define REG_PDCA_SR5               (*(RoReg  *)0x400A215CU) /**< \brief (PDCA) Status Register 5 */
#define REG_PDCA_IER5              (*(WoReg  *)0x400A2160U) /**< \brief (PDCA) Interrupt Enable Register 5 */
#define REG_PDCA_IDR5              (*(WoReg  *)0x400A2164U) /**< \brief (PDCA) Interrupt Disable Register 5 */
#define REG_PDCA_IMR5              (*(RoReg  *)0x400A2168U) /**< \brief (PDCA) Interrupt Mask Register 5 */
#define REG_PDCA_ISR5              (*(RoReg  *)0x400A216CU) /**< \brief (PDCA) Interrupt Status Register 5 */
#define REG_PDCA_MAR6              (*(RwReg  *)0x400A2180U) /**< \brief (PDCA) Memory Address Register 6 */
#define REG_PDCA_PSR6              (*(RwReg  *)0x400A2184U) /**< \brief (PDCA) Peripheral Select Register 6 */
#define REG_PDCA_TCR6              (*(RwReg  *)0x400A2188U) /**< \brief (PDCA) Transfer Counter Register 6 */
#define REG_PDCA_MARR6             (*(RwReg  *)0x400A218CU) /**< \brief (PDCA) Memory Address Reload Register 6 */
#define REG_PDCA_TCRR6             (*(RwReg  *)0x400A2190U) /**< \brief (PDCA) Transfer Counter Reload Register 6 */
#define REG_PDCA_CR6               (*(WoReg  *)0x400A2194U) /**< \brief (PDCA) Control Register 6 */
#define REG_PDCA_MR6               (*(RwReg  *)0x400A2198U) /**< \brief (PDCA) Mode Register 6 */
#define REG_PDCA_SR6               (*(RoReg  *)0x400A219CU) /**< \brief (PDCA) Status Register 6 */
#define REG_PDCA_IER6              (*(WoReg  *)0x400A21A0U) /**< \brief (PDCA) Interrupt Enable Register 6 */
#define REG_PDCA_IDR6              (*(WoReg  *)0x400A21A4U) /**< \brief (PDCA) Interrupt Disable Register 6 */
#define REG_PDCA_IMR6              (*(RoReg  *)0x400A21A8U) /**< \brief (PDCA) Interrupt Mask Register 6 */
#define REG_PDCA_ISR6              (*(RoReg  *)0x400A21ACU) /**< \brief (PDCA) Interrupt Status Register 6 */
#define REG_PDCA_MAR7              (*(RwReg  *)0x400A21C0U) /**< \brief (PDCA) Memory Address Register 7 */
#define REG_PDCA_PSR7              (*(RwReg  *)0x400A21C4U) /**< \brief (PDCA) Peripheral Select Register 7 */
#define REG_PDCA_TCR7              (*(RwReg  *)0x400A21C8U) /**< \brief (PDCA) Transfer Counter Register 7 */
#define REG_PDCA_MARR7             (*(RwReg  *)0x400A21CCU) /**< \brief (PDCA) Memory Address Reload Register 7 */
#define REG_PDCA_TCRR7             (*(RwReg  *)0x400A21D0U) /**< \brief (PDCA) Transfer Counter Reload Register 7 */
#define REG_PDCA_CR7               (*(WoReg  *)0x400A21D4U) /**< \brief (PDCA) Control Register 7 */
#define REG_PDCA_MR7               (*(RwReg  *)0x400A21D8U) /**< \brief (PDCA) Mode Register 7 */
#define REG_PDCA_SR7               (*(RoReg  *)0x400A21DCU) /**< \brief (PDCA) Status Register 7 */
#define REG_PDCA_IER7              (*(WoReg  *)0x400A21E0U) /**< \brief (PDCA) Interrupt Enable Register 7 */
#define REG_PDCA_IDR7              (*(WoReg  *)0x400A21E4U) /**< \brief (PDCA) Interrupt Disable Register 7 */
#define REG_PDCA_IMR7              (*(RoReg  *)0x400A21E8U) /**< \brief (PDCA) Interrupt Mask Register 7 */
#define REG_PDCA_ISR7              (*(RoReg  *)0x400A21ECU) /**< \brief (PDCA) Interrupt Status Register 7 */
#define REG_PDCA_MAR8              (*(RwReg  *)0x400A2200U) /**< \brief (PDCA) Memory Address Register 8 */
#define REG_PDCA_PSR8              (*(RwReg  *)0x400A2204U) /**< \brief (PDCA) Peripheral Select Register 8 */
#define REG_PDCA_TCR8              (*(RwReg  *)0x400A2208U) /**< \brief (PDCA) Transfer Counter Register 8 */
#define REG_PDCA_MARR8             (*(RwReg  *)0x400A220CU) /**< \brief (PDCA) Memory Address Reload Register 8 */
#define REG_PDCA_TCRR8             (*(RwReg  *)0x400A2210U) /**< \brief (PDCA) Transfer Counter Reload Register 8 */
#define REG_PDCA_CR8               (*(WoReg  *)0x400A2214U) /**< \brief (PDCA) Control Register 8 */
#define REG_PDCA_MR8               (*(RwReg  *)0x400A2218U) /**< \brief (PDCA) Mode Register 8 */
#define REG_PDCA_SR8               (*(RoReg  *)0x400A221CU) /**< \brief (PDCA) Status Register 8 */
#define REG_PDCA_IER8              (*(WoReg  *)0x400A2220U) /**< \brief (PDCA) Interrupt Enable Register 8 */
#define REG_PDCA_IDR8              (*(WoReg  *)0x400A2224U) /**< \brief (PDCA) Interrupt Disable Register 8 */
#define REG_PDCA_IMR8              (*(RoReg  *)0x400A2228U) /**< \brief (PDCA) Interrupt Mask Register 8 */
#define REG_PDCA_ISR8              (*(RoReg  *)0x400A222CU) /**< \brief (PDCA) Interrupt Status Register 8 */
#define REG_PDCA_MAR9              (*(RwReg  *)0x400A2240U) /**< \brief (PDCA) Memory Address Register 9 */
#define REG_PDCA_PSR9              (*(RwReg  *)0x400A2244U) /**< \brief (PDCA) Peripheral Select Register 9 */
#define REG_PDCA_TCR9              (*(RwReg  *)0x400A2248U) /**< \brief (PDCA) Transfer Counter Register 9 */
#define REG_PDCA_MARR9             (*(RwReg  *)0x400A224CU) /**< \brief (PDCA) Memory Address Reload Register 9 */
#define REG_PDCA_TCRR9             (*(RwReg  *)0x400A2250U) /**< \brief (PDCA) Transfer Counter Reload Register 9 */
#define REG_PDCA_CR9               (*(WoReg  *)0x400A2254U) /**< \brief (PDCA) Control Register 9 */
#define REG_PDCA_MR9               (*(RwReg  *)0x400A2258U) /**< \brief (PDCA) Mode Register 9 */
#define REG_PDCA_SR9               (*(RoReg  *)0x400A225CU) /**< \brief (PDCA) Status Register 9 */
#define REG_PDCA_IER9              (*(WoReg  *)0x400A2260U) /**< \brief (PDCA) Interrupt Enable Register 9 */
#define REG_PDCA_IDR9              (*(WoReg  *)0x400A2264U) /**< \brief (PDCA) Interrupt Disable Register 9 */
#define REG_PDCA_IMR9              (*(RoReg  *)0x400A2268U) /**< \brief (PDCA) Interrupt Mask Register 9 */
#define REG_PDCA_ISR9              (*(RoReg  *)0x400A226CU) /**< \brief (PDCA) Interrupt Status Register 9 */
#define REG_PDCA_MAR10             (*(RwReg  *)0x400A2280U) /**< \brief (PDCA) Memory Address Register 10 */
#define REG_PDCA_PSR10             (*(RwReg  *)0x400A2284U) /**< \brief (PDCA) Peripheral Select Register 10 */
#define REG_PDCA_TCR10             (*(RwReg  *)0x400A2288U) /**< \brief (PDCA) Transfer Counter Register 10 */
#define REG_PDCA_MARR10            (*(RwReg  *)0x400A228CU) /**< \brief (PDCA) Memory Address Reload Register 10 */
#define REG_PDCA_TCRR10            (*(RwReg  *)0x400A2290U) /**< \brief (PDCA) Transfer Counter Reload Register 10 */
#define REG_PDCA_CR10              (*(WoReg  *)0x400A2294U) /**< \brief (PDCA) Control Register 10 */
#define REG_PDCA_MR10              (*(RwReg  *)0x400A2298U) /**< \brief (PDCA) Mode Register 10 */
#define REG_PDCA_SR10              (*(RoReg  *)0x400A229CU) /**< \brief (PDCA) Status Register 10 */
#define REG_PDCA_IER10             (*(WoReg  *)0x400A22A0U) /**< \brief (PDCA) Interrupt Enable Register 10 */
#define REG_PDCA_IDR10             (*(WoReg  *)0x400A22A4U) /**< \brief (PDCA) Interrupt Disable Register 10 */
#define REG_PDCA_IMR10             (*(RoReg  *)0x400A22A8U) /**< \brief (PDCA) Interrupt Mask Register 10 */
#define REG_PDCA_ISR10             (*(RoReg  *)0x400A22ACU) /**< \brief (PDCA) Interrupt Status Register 10 */
#define REG_PDCA_MAR11             (*(RwReg  *)0x400A22C0U) /**< \brief (PDCA) Memory Address Register 11 */
#define REG_PDCA_PSR11             (*(RwReg  *)0x400A22C4U) /**< \brief (PDCA) Peripheral Select Register 11 */
#define REG_PDCA_TCR11             (*(RwReg  *)0x400A22C8U) /**< \brief (PDCA) Transfer Counter Register 11 */
#define REG_PDCA_MARR11            (*(RwReg  *)0x400A22CCU) /**< \brief (PDCA) Memory Address Reload Register 11 */
#define REG_PDCA_TCRR11            (*(RwReg  *)0x400A22D0U) /**< \brief (PDCA) Transfer Counter Reload Register 11 */
#define REG_PDCA_CR11              (*(WoReg  *)0x400A22D4U) /**< \brief (PDCA) Control Register 11 */
#define REG_PDCA_MR11              (*(RwReg  *)0x400A22D8U) /**< \brief (PDCA) Mode Register 11 */
#define REG_PDCA_SR11              (*(RoReg  *)0x400A22DCU) /**< \brief (PDCA) Status Register 11 */
#define REG_PDCA_IER11             (*(WoReg  *)0x400A22E0U) /**< \brief (PDCA) Interrupt Enable Register 11 */
#define REG_PDCA_IDR11             (*(WoReg  *)0x400A22E4U) /**< \brief (PDCA) Interrupt Disable Register 11 */
#define REG_PDCA_IMR11             (*(RoReg  *)0x400A22E8U) /**< \brief (PDCA) Interrupt Mask Register 11 */
#define REG_PDCA_ISR11             (*(RoReg  *)0x400A22ECU) /**< \brief (PDCA) Interrupt Status Register 11 */
#define REG_PDCA_MAR12             (*(RwReg  *)0x400A2300U) /**< \brief (PDCA) Memory Address Register 12 */
#define REG_PDCA_PSR12             (*(RwReg  *)0x400A2304U) /**< \brief (PDCA) Peripheral Select Register 12 */
#define REG_PDCA_TCR12             (*(RwReg  *)0x400A2308U) /**< \brief (PDCA) Transfer Counter Register 12 */
#define REG_PDCA_MARR12            (*(RwReg  *)0x400A230CU) /**< \brief (PDCA) Memory Address Reload Register 12 */
#define REG_PDCA_TCRR12            (*(RwReg  *)0x400A2310U) /**< \brief (PDCA) Transfer Counter Reload Register 12 */
#define REG_PDCA_CR12              (*(WoReg  *)0x400A2314U) /**< \brief (PDCA) Control Register 12 */
#define REG_PDCA_MR12              (*(RwReg  *)0x400A2318U) /**< \brief (PDCA) Mode Register 12 */
#define REG_PDCA_SR12              (*(RoReg  *)0x400A231CU) /**< \brief (PDCA) Status Register 12 */
#define REG_PDCA_IER12             (*(WoReg  *)0x400A2320U) /**< \brief (PDCA) Interrupt Enable Register 12 */
#define REG_PDCA_IDR12             (*(WoReg  *)0x400A2324U) /**< \brief (PDCA) Interrupt Disable Register 12 */
#define REG_PDCA_IMR12             (*(RoReg  *)0x400A2328U) /**< \brief (PDCA) Interrupt Mask Register 12 */
#define REG_PDCA_ISR12             (*(RoReg  *)0x400A232CU) /**< \brief (PDCA) Interrupt Status Register 12 */
#define REG_PDCA_MAR13             (*(RwReg  *)0x400A2340U) /**< \brief (PDCA) Memory Address Register 13 */
#define REG_PDCA_PSR13             (*(RwReg  *)0x400A2344U) /**< \brief (PDCA) Peripheral Select Register 13 */
#define REG_PDCA_TCR13             (*(RwReg  *)0x400A2348U) /**< \brief (PDCA) Transfer Counter Register 13 */
#define REG_PDCA_MARR13            (*(RwReg  *)0x400A234CU) /**< \brief (PDCA) Memory Address Reload Register 13 */
#define REG_PDCA_TCRR13            (*(RwReg  *)0x400A2350U) /**< \brief (PDCA) Transfer Counter Reload Register 13 */
#define REG_PDCA_CR13              (*(WoReg  *)0x400A2354U) /**< \brief (PDCA) Control Register 13 */
#define REG_PDCA_MR13              (*(RwReg  *)0x400A2358U) /**< \brief (PDCA) Mode Register 13 */
#define REG_PDCA_SR13              (*(RoReg  *)0x400A235CU) /**< \brief (PDCA) Status Register 13 */
#define REG_PDCA_IER13             (*(WoReg  *)0x400A2360U) /**< \brief (PDCA) Interrupt Enable Register 13 */
#define REG_PDCA_IDR13             (*(WoReg  *)0x400A2364U) /**< \brief (PDCA) Interrupt Disable Register 13 */
#define REG_PDCA_IMR13             (*(RoReg  *)0x400A2368U) /**< \brief (PDCA) Interrupt Mask Register 13 */
#define REG_PDCA_ISR13             (*(RoReg  *)0x400A236CU) /**< \brief (PDCA) Interrupt Status Register 13 */
#define REG_PDCA_MAR14             (*(RwReg  *)0x400A2380U) /**< \brief (PDCA) Memory Address Register 14 */
#define REG_PDCA_PSR14             (*(RwReg  *)0x400A2384U) /**< \brief (PDCA) Peripheral Select Register 14 */
#define REG_PDCA_TCR14             (*(RwReg  *)0x400A2388U) /**< \brief (PDCA) Transfer Counter Register 14 */
#define REG_PDCA_MARR14            (*(RwReg  *)0x400A238CU) /**< \brief (PDCA) Memory Address Reload Register 14 */
#define REG_PDCA_TCRR14            (*(RwReg  *)0x400A2390U) /**< \brief (PDCA) Transfer Counter Reload Register 14 */
#define REG_PDCA_CR14              (*(WoReg  *)0x400A2394U) /**< \brief (PDCA) Control Register 14 */
#define REG_PDCA_MR14              (*(RwReg  *)0x400A2398U) /**< \brief (PDCA) Mode Register 14 */
#define REG_PDCA_SR14              (*(RoReg  *)0x400A239CU) /**< \brief (PDCA) Status Register 14 */
#define REG_PDCA_IER14             (*(WoReg  *)0x400A23A0U) /**< \brief (PDCA) Interrupt Enable Register 14 */
#define REG_PDCA_IDR14             (*(WoReg  *)0x400A23A4U) /**< \brief (PDCA) Interrupt Disable Register 14 */
#define REG_PDCA_IMR14             (*(RoReg  *)0x400A23A8U) /**< \brief (PDCA) Interrupt Mask Register 14 */
#define REG_PDCA_ISR14             (*(RoReg  *)0x400A23ACU) /**< \brief (PDCA) Interrupt Status Register 14 */
#define REG_PDCA_MAR15             (*(RwReg  *)0x400A23C0U) /**< \brief (PDCA) Memory Address Register 15 */
#define REG_PDCA_PSR15             (*(RwReg  *)0x400A23C4U) /**< \brief (PDCA) Peripheral Select Register 15 */
#define REG_PDCA_TCR15             (*(RwReg  *)0x400A23C8U) /**< \brief (PDCA) Transfer Counter Register 15 */
#define REG_PDCA_MARR15            (*(RwReg  *)0x400A23CCU) /**< \brief (PDCA) Memory Address Reload Register 15 */
#define REG_PDCA_TCRR15            (*(RwReg  *)0x400A23D0U) /**< \brief (PDCA) Transfer Counter Reload Register 15 */
#define REG_PDCA_CR15              (*(WoReg  *)0x400A23D4U) /**< \brief (PDCA) Control Register 15 */
#define REG_PDCA_MR15              (*(RwReg  *)0x400A23D8U) /**< \brief (PDCA) Mode Register 15 */
#define REG_PDCA_SR15              (*(RoReg  *)0x400A23DCU) /**< \brief (PDCA) Status Register 15 */
#define REG_PDCA_IER15             (*(WoReg  *)0x400A23E0U) /**< \brief (PDCA) Interrupt Enable Register 15 */
#define REG_PDCA_IDR15             (*(WoReg  *)0x400A23E4U) /**< \brief (PDCA) Interrupt Disable Register 15 */
#define REG_PDCA_IMR15             (*(RoReg  *)0x400A23E8U) /**< \brief (PDCA) Interrupt Mask Register 15 */
#define REG_PDCA_ISR15             (*(RoReg  *)0x400A23ECU) /**< \brief (PDCA) Interrupt Status Register 15 */
#define REG_PDCA_PCONTROL          (*(RwReg  *)0x400A2800U) /**< \brief (PDCA) Performance Control Register */
#define REG_PDCA_PRDATA0           (*(RoReg  *)0x400A2804U) /**< \brief (PDCA) Channel 0 Read Data Cycles */
#define REG_PDCA_PRSTALL0          (*(RoReg  *)0x400A2808U) /**< \brief (PDCA) Channel 0 Read Stall Cycles */
#define REG_PDCA_PRLAT0            (*(RoReg  *)0x400A280CU) /**< \brief (PDCA) Channel 0 Read Max Latency */
#define REG_PDCA_PWDATA0           (*(RoReg  *)0x400A2810U) /**< \brief (PDCA) Channel 0 Write Data Cycles */
#define REG_PDCA_PWSTALL0          (*(RoReg  *)0x400A2814U) /**< \brief (PDCA) Channel 0 Write Stall Cycles */
#define REG_PDCA_PWLAT0            (*(RoReg  *)0x400A2818U) /**< \brief (PDCA) Channel0 Write Max Latency */
#define REG_PDCA_PRDATA1           (*(RoReg  *)0x400A281CU) /**< \brief (PDCA) Channel 1 Read Data Cycles */
#define REG_PDCA_PRSTALL1          (*(RoReg  *)0x400A2820U) /**< \brief (PDCA) Channel Read Stall Cycles */
#define REG_PDCA_PRLAT1            (*(RoReg  *)0x400A2824U) /**< \brief (PDCA) Channel 1 Read Max Latency */
#define REG_PDCA_PWDATA1           (*(RoReg  *)0x400A2828U) /**< \brief (PDCA) Channel 1 Write Data Cycles */
#define REG_PDCA_PWSTALL1          (*(RoReg  *)0x400A282CU) /**< \brief (PDCA) Channel 1 Write stall Cycles */
#define REG_PDCA_PWLAT1            (*(RoReg  *)0x400A2830U) /**< \brief (PDCA) Channel 1 Read Max Latency */
#define REG_PDCA_VERSION           (*(RoReg  *)0x400A2834U) /**< \brief (PDCA) Version Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/* ========== Instance parameters for PDCA peripheral ========== */
#define PDCA_CHANNEL_LENGTH         16
#define PDCA_CLK_AHB_ID             0
#define PDCA_MON_CH0_IMPL           0
#define PDCA_MON_CH1_IMPL           0

#endif /* _SAM4L_PDCA_INSTANCE_ */
