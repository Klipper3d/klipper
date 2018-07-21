/**
 * \file
 *
 * \brief Instance description for GPIO
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

#ifndef _SAM4L_GPIO_INSTANCE_
#define _SAM4L_GPIO_INSTANCE_

/* ========== Register definition for GPIO peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_GPIO_GPER0             (0x400E1000U) /**< \brief (GPIO) GPIO Enable Register 0 */
#define REG_GPIO_GPERS0            (0x400E1004U) /**< \brief (GPIO) GPIO Enable Register - Set 0 */
#define REG_GPIO_GPERC0            (0x400E1008U) /**< \brief (GPIO) GPIO Enable Register - Clear 0 */
#define REG_GPIO_GPERT0            (0x400E100CU) /**< \brief (GPIO) GPIO Enable Register - Toggle 0 */
#define REG_GPIO_PMR00             (0x400E1010U) /**< \brief (GPIO) Peripheral Mux Register 0 0 */
#define REG_GPIO_PMR0S0            (0x400E1014U) /**< \brief (GPIO) Peripheral Mux Register 0 - Set 0 */
#define REG_GPIO_PMR0C0            (0x400E1018U) /**< \brief (GPIO) Peripheral Mux Register 0 - Clear 0 */
#define REG_GPIO_PMR0T0            (0x400E101CU) /**< \brief (GPIO) Peripheral Mux Register 0 - Toggle 0 */
#define REG_GPIO_PMR10             (0x400E1020U) /**< \brief (GPIO) Peripheral Mux Register 1 0 */
#define REG_GPIO_PMR1S0            (0x400E1024U) /**< \brief (GPIO) Peripheral Mux Register 1 - Set 0 */
#define REG_GPIO_PMR1C0            (0x400E1028U) /**< \brief (GPIO) Peripheral Mux Register 1 - Clear 0 */
#define REG_GPIO_PMR1T0            (0x400E102CU) /**< \brief (GPIO) Peripheral Mux Register 1 - Toggle 0 */
#define REG_GPIO_PMR20             (0x400E1030U) /**< \brief (GPIO) Peripheral Mux Register 2 0 */
#define REG_GPIO_PMR2S0            (0x400E1034U) /**< \brief (GPIO) Peripheral Mux Register 2 - Set 0 */
#define REG_GPIO_PMR2C0            (0x400E1038U) /**< \brief (GPIO) Peripheral Mux Register 2 - Clear 0 */
#define REG_GPIO_PMR2T0            (0x400E103CU) /**< \brief (GPIO) Peripheral Mux Register 2 - Toggle 0 */
#define REG_GPIO_ODER0             (0x400E1040U) /**< \brief (GPIO) Output Driver Enable Register 0 */
#define REG_GPIO_ODERS0            (0x400E1044U) /**< \brief (GPIO) Output Driver Enable Register - Set 0 */
#define REG_GPIO_ODERC0            (0x400E1048U) /**< \brief (GPIO) Output Driver Enable Register - Clear 0 */
#define REG_GPIO_ODERT0            (0x400E104CU) /**< \brief (GPIO) Output Driver Enable Register - Toggle 0 */
#define REG_GPIO_OVR0              (0x400E1050U) /**< \brief (GPIO) Output Value Register 0 */
#define REG_GPIO_OVRS0             (0x400E1054U) /**< \brief (GPIO) Output Value Register - Set 0 */
#define REG_GPIO_OVRC0             (0x400E1058U) /**< \brief (GPIO) Output Value Register - Clear 0 */
#define REG_GPIO_OVRT0             (0x400E105CU) /**< \brief (GPIO) Output Value Register - Toggle 0 */
#define REG_GPIO_PVR0              (0x400E1060U) /**< \brief (GPIO) Pin Value Register 0 */
#define REG_GPIO_PUER0             (0x400E1070U) /**< \brief (GPIO) Pull-up Enable Register 0 */
#define REG_GPIO_PUERS0            (0x400E1074U) /**< \brief (GPIO) Pull-up Enable Register - Set 0 */
#define REG_GPIO_PUERC0            (0x400E1078U) /**< \brief (GPIO) Pull-up Enable Register - Clear 0 */
#define REG_GPIO_PUERT0            (0x400E107CU) /**< \brief (GPIO) Pull-up Enable Register - Toggle 0 */
#define REG_GPIO_PDER0             (0x400E1080U) /**< \brief (GPIO) Pull-down Enable Register 0 */
#define REG_GPIO_PDERS0            (0x400E1084U) /**< \brief (GPIO) Pull-down Enable Register - Set 0 */
#define REG_GPIO_PDERC0            (0x400E1088U) /**< \brief (GPIO) Pull-down Enable Register - Clear 0 */
#define REG_GPIO_PDERT0            (0x400E108CU) /**< \brief (GPIO) Pull-down Enable Register - Toggle 0 */
#define REG_GPIO_IER0              (0x400E1090U) /**< \brief (GPIO) Interrupt Enable Register 0 */
#define REG_GPIO_IERS0             (0x400E1094U) /**< \brief (GPIO) Interrupt Enable Register - Set 0 */
#define REG_GPIO_IERC0             (0x400E1098U) /**< \brief (GPIO) Interrupt Enable Register - Clear 0 */
#define REG_GPIO_IERT0             (0x400E109CU) /**< \brief (GPIO) Interrupt Enable Register - Toggle 0 */
#define REG_GPIO_IMR00             (0x400E10A0U) /**< \brief (GPIO) Interrupt Mode Register 0 0 */
#define REG_GPIO_IMR0S0            (0x400E10A4U) /**< \brief (GPIO) Interrupt Mode Register 0 - Set 0 */
#define REG_GPIO_IMR0C0            (0x400E10A8U) /**< \brief (GPIO) Interrupt Mode Register 0 - Clear 0 */
#define REG_GPIO_IMR0T0            (0x400E10ACU) /**< \brief (GPIO) Interrupt Mode Register 0 - Toggle 0 */
#define REG_GPIO_IMR10             (0x400E10B0U) /**< \brief (GPIO) Interrupt Mode Register 1 0 */
#define REG_GPIO_IMR1S0            (0x400E10B4U) /**< \brief (GPIO) Interrupt Mode Register 1 - Set 0 */
#define REG_GPIO_IMR1C0            (0x400E10B8U) /**< \brief (GPIO) Interrupt Mode Register 1 - Clear 0 */
#define REG_GPIO_IMR1T0            (0x400E10BCU) /**< \brief (GPIO) Interrupt Mode Register 1 - Toggle 0 */
#define REG_GPIO_GFER0             (0x400E10C0U) /**< \brief (GPIO) Glitch Filter Enable Register 0 */
#define REG_GPIO_GFERS0            (0x400E10C4U) /**< \brief (GPIO) Glitch Filter Enable Register - Set 0 */
#define REG_GPIO_GFERC0            (0x400E10C8U) /**< \brief (GPIO) Glitch Filter Enable Register - Clear 0 */
#define REG_GPIO_GFERT0            (0x400E10CCU) /**< \brief (GPIO) Glitch Filter Enable Register - Toggle 0 */
#define REG_GPIO_IFR0              (0x400E10D0U) /**< \brief (GPIO) Interrupt Flag Register 0 */
#define REG_GPIO_IFRC0             (0x400E10D8U) /**< \brief (GPIO) Interrupt Flag Register - Clear 0 */
#define REG_GPIO_ODMER0            (0x400E10E0U) /**< \brief (GPIO) Open Drain Mode Register 0 */
#define REG_GPIO_ODMERS0           (0x400E10E4U) /**< \brief (GPIO) Open Drain Mode Register - Set 0 */
#define REG_GPIO_ODMERC0           (0x400E10E8U) /**< \brief (GPIO) Open Drain Mode Register - Clear 0 */
#define REG_GPIO_ODMERT0           (0x400E10ECU) /**< \brief (GPIO) Open Drain Mode Register - Toggle 0 */
#define REG_GPIO_ODCR00            (0x400E1100U) /**< \brief (GPIO) Output Driving Capability Register 0 0 */
#define REG_GPIO_ODCR0S0           (0x400E1104U) /**< \brief (GPIO) Output Driving Capability Register 0 - Set 0 */
#define REG_GPIO_ODCR0C0           (0x400E1108U) /**< \brief (GPIO) Output Driving Capability Register 0 - Clear 0 */
#define REG_GPIO_ODCR0T0           (0x400E110CU) /**< \brief (GPIO) Output Driving Capability Register 0 - Toggle 0 */
#define REG_GPIO_ODCR10            (0x400E1110U) /**< \brief (GPIO) Output Driving Capability Register 1 0 */
#define REG_GPIO_ODCR1S0           (0x400E1114U) /**< \brief (GPIO) Output Driving Capability Register 1 - Set 0 */
#define REG_GPIO_ODCR1C0           (0x400E1118U) /**< \brief (GPIO) Output Driving Capability Register 1 - Clear 0 */
#define REG_GPIO_ODCR1T0           (0x400E111CU) /**< \brief (GPIO) Output Driving Capability Register 1 - Toggle 0 */
#define REG_GPIO_OSRR00            (0x400E1130U) /**< \brief (GPIO) Output Slew Rate Register 0 0 */
#define REG_GPIO_OSRR0S0           (0x400E1134U) /**< \brief (GPIO) Output Slew Rate Register 0 - Set 0 */
#define REG_GPIO_OSRR0C0           (0x400E1138U) /**< \brief (GPIO) Output Slew Rate Register 0 - Clear 0 */
#define REG_GPIO_OSRR0T0           (0x400E113CU) /**< \brief (GPIO) Output Slew Rate Register 0 - Toggle 0 */
#define REG_GPIO_STER0             (0x400E1160U) /**< \brief (GPIO) Schmitt Trigger Enable Register 0 */
#define REG_GPIO_STERS0            (0x400E1164U) /**< \brief (GPIO) Schmitt Trigger Enable Register - Set 0 */
#define REG_GPIO_STERC0            (0x400E1168U) /**< \brief (GPIO) Schmitt Trigger Enable Register - Clear 0 */
#define REG_GPIO_STERT0            (0x400E116CU) /**< \brief (GPIO) Schmitt Trigger Enable Register - Toggle 0 */
#define REG_GPIO_EVER0             (0x400E1180U) /**< \brief (GPIO) Event Enable Register 0 */
#define REG_GPIO_EVERS0            (0x400E1184U) /**< \brief (GPIO) Event Enable Register - Set 0 */
#define REG_GPIO_EVERC0            (0x400E1188U) /**< \brief (GPIO) Event Enable Register - Clear 0 */
#define REG_GPIO_EVERT0            (0x400E118CU) /**< \brief (GPIO) Event Enable Register - Toggle 0 */
#define REG_GPIO_LOCK0             (0x400E11A0U) /**< \brief (GPIO) Lock Register 0 */
#define REG_GPIO_LOCKS0            (0x400E11A4U) /**< \brief (GPIO) Lock Register - Set 0 */
#define REG_GPIO_LOCKC0            (0x400E11A8U) /**< \brief (GPIO) Lock Register - Clear 0 */
#define REG_GPIO_LOCKT0            (0x400E11ACU) /**< \brief (GPIO) Lock Register - Toggle 0 */
#define REG_GPIO_UNLOCK0           (0x400E11E0U) /**< \brief (GPIO) Unlock Register 0 */
#define REG_GPIO_ASR0              (0x400E11E4U) /**< \brief (GPIO) Access Status Register 0 */
#define REG_GPIO_PARAMETER0        (0x400E11F8U) /**< \brief (GPIO) Parameter Register 0 */
#define REG_GPIO_VERSION0          (0x400E11FCU) /**< \brief (GPIO) Version Register 0 */
#define REG_GPIO_GPER1             (0x400E1200U) /**< \brief (GPIO) GPIO Enable Register 1 */
#define REG_GPIO_GPERS1            (0x400E1204U) /**< \brief (GPIO) GPIO Enable Register - Set 1 */
#define REG_GPIO_GPERC1            (0x400E1208U) /**< \brief (GPIO) GPIO Enable Register - Clear 1 */
#define REG_GPIO_GPERT1            (0x400E120CU) /**< \brief (GPIO) GPIO Enable Register - Toggle 1 */
#define REG_GPIO_PMR01             (0x400E1210U) /**< \brief (GPIO) Peripheral Mux Register 0 1 */
#define REG_GPIO_PMR0S1            (0x400E1214U) /**< \brief (GPIO) Peripheral Mux Register 0 - Set 1 */
#define REG_GPIO_PMR0C1            (0x400E1218U) /**< \brief (GPIO) Peripheral Mux Register 0 - Clear 1 */
#define REG_GPIO_PMR0T1            (0x400E121CU) /**< \brief (GPIO) Peripheral Mux Register 0 - Toggle 1 */
#define REG_GPIO_PMR11             (0x400E1220U) /**< \brief (GPIO) Peripheral Mux Register 1 1 */
#define REG_GPIO_PMR1S1            (0x400E1224U) /**< \brief (GPIO) Peripheral Mux Register 1 - Set 1 */
#define REG_GPIO_PMR1C1            (0x400E1228U) /**< \brief (GPIO) Peripheral Mux Register 1 - Clear 1 */
#define REG_GPIO_PMR1T1            (0x400E122CU) /**< \brief (GPIO) Peripheral Mux Register 1 - Toggle 1 */
#define REG_GPIO_PMR21             (0x400E1230U) /**< \brief (GPIO) Peripheral Mux Register 2 1 */
#define REG_GPIO_PMR2S1            (0x400E1234U) /**< \brief (GPIO) Peripheral Mux Register 2 - Set 1 */
#define REG_GPIO_PMR2C1            (0x400E1238U) /**< \brief (GPIO) Peripheral Mux Register 2 - Clear 1 */
#define REG_GPIO_PMR2T1            (0x400E123CU) /**< \brief (GPIO) Peripheral Mux Register 2 - Toggle 1 */
#define REG_GPIO_ODER1             (0x400E1240U) /**< \brief (GPIO) Output Driver Enable Register 1 */
#define REG_GPIO_ODERS1            (0x400E1244U) /**< \brief (GPIO) Output Driver Enable Register - Set 1 */
#define REG_GPIO_ODERC1            (0x400E1248U) /**< \brief (GPIO) Output Driver Enable Register - Clear 1 */
#define REG_GPIO_ODERT1            (0x400E124CU) /**< \brief (GPIO) Output Driver Enable Register - Toggle 1 */
#define REG_GPIO_OVR1              (0x400E1250U) /**< \brief (GPIO) Output Value Register 1 */
#define REG_GPIO_OVRS1             (0x400E1254U) /**< \brief (GPIO) Output Value Register - Set 1 */
#define REG_GPIO_OVRC1             (0x400E1258U) /**< \brief (GPIO) Output Value Register - Clear 1 */
#define REG_GPIO_OVRT1             (0x400E125CU) /**< \brief (GPIO) Output Value Register - Toggle 1 */
#define REG_GPIO_PVR1              (0x400E1260U) /**< \brief (GPIO) Pin Value Register 1 */
#define REG_GPIO_PUER1             (0x400E1270U) /**< \brief (GPIO) Pull-up Enable Register 1 */
#define REG_GPIO_PUERS1            (0x400E1274U) /**< \brief (GPIO) Pull-up Enable Register - Set 1 */
#define REG_GPIO_PUERC1            (0x400E1278U) /**< \brief (GPIO) Pull-up Enable Register - Clear 1 */
#define REG_GPIO_PUERT1            (0x400E127CU) /**< \brief (GPIO) Pull-up Enable Register - Toggle 1 */
#define REG_GPIO_PDER1             (0x400E1280U) /**< \brief (GPIO) Pull-down Enable Register 1 */
#define REG_GPIO_PDERS1            (0x400E1284U) /**< \brief (GPIO) Pull-down Enable Register - Set 1 */
#define REG_GPIO_PDERC1            (0x400E1288U) /**< \brief (GPIO) Pull-down Enable Register - Clear 1 */
#define REG_GPIO_PDERT1            (0x400E128CU) /**< \brief (GPIO) Pull-down Enable Register - Toggle 1 */
#define REG_GPIO_IER1              (0x400E1290U) /**< \brief (GPIO) Interrupt Enable Register 1 */
#define REG_GPIO_IERS1             (0x400E1294U) /**< \brief (GPIO) Interrupt Enable Register - Set 1 */
#define REG_GPIO_IERC1             (0x400E1298U) /**< \brief (GPIO) Interrupt Enable Register - Clear 1 */
#define REG_GPIO_IERT1             (0x400E129CU) /**< \brief (GPIO) Interrupt Enable Register - Toggle 1 */
#define REG_GPIO_IMR01             (0x400E12A0U) /**< \brief (GPIO) Interrupt Mode Register 0 1 */
#define REG_GPIO_IMR0S1            (0x400E12A4U) /**< \brief (GPIO) Interrupt Mode Register 0 - Set 1 */
#define REG_GPIO_IMR0C1            (0x400E12A8U) /**< \brief (GPIO) Interrupt Mode Register 0 - Clear 1 */
#define REG_GPIO_IMR0T1            (0x400E12ACU) /**< \brief (GPIO) Interrupt Mode Register 0 - Toggle 1 */
#define REG_GPIO_IMR11             (0x400E12B0U) /**< \brief (GPIO) Interrupt Mode Register 1 1 */
#define REG_GPIO_IMR1S1            (0x400E12B4U) /**< \brief (GPIO) Interrupt Mode Register 1 - Set 1 */
#define REG_GPIO_IMR1C1            (0x400E12B8U) /**< \brief (GPIO) Interrupt Mode Register 1 - Clear 1 */
#define REG_GPIO_IMR1T1            (0x400E12BCU) /**< \brief (GPIO) Interrupt Mode Register 1 - Toggle 1 */
#define REG_GPIO_GFER1             (0x400E12C0U) /**< \brief (GPIO) Glitch Filter Enable Register 1 */
#define REG_GPIO_GFERS1            (0x400E12C4U) /**< \brief (GPIO) Glitch Filter Enable Register - Set 1 */
#define REG_GPIO_GFERC1            (0x400E12C8U) /**< \brief (GPIO) Glitch Filter Enable Register - Clear 1 */
#define REG_GPIO_GFERT1            (0x400E12CCU) /**< \brief (GPIO) Glitch Filter Enable Register - Toggle 1 */
#define REG_GPIO_IFR1              (0x400E12D0U) /**< \brief (GPIO) Interrupt Flag Register 1 */
#define REG_GPIO_IFRC1             (0x400E12D8U) /**< \brief (GPIO) Interrupt Flag Register - Clear 1 */
#define REG_GPIO_ODMER1            (0x400E12E0U) /**< \brief (GPIO) Open Drain Mode Register 1 */
#define REG_GPIO_ODMERS1           (0x400E12E4U) /**< \brief (GPIO) Open Drain Mode Register - Set 1 */
#define REG_GPIO_ODMERC1           (0x400E12E8U) /**< \brief (GPIO) Open Drain Mode Register - Clear 1 */
#define REG_GPIO_ODMERT1           (0x400E12ECU) /**< \brief (GPIO) Open Drain Mode Register - Toggle 1 */
#define REG_GPIO_ODCR01            (0x400E1300U) /**< \brief (GPIO) Output Driving Capability Register 0 1 */
#define REG_GPIO_ODCR0S1           (0x400E1304U) /**< \brief (GPIO) Output Driving Capability Register 0 - Set 1 */
#define REG_GPIO_ODCR0C1           (0x400E1308U) /**< \brief (GPIO) Output Driving Capability Register 0 - Clear 1 */
#define REG_GPIO_ODCR0T1           (0x400E130CU) /**< \brief (GPIO) Output Driving Capability Register 0 - Toggle 1 */
#define REG_GPIO_ODCR11            (0x400E1310U) /**< \brief (GPIO) Output Driving Capability Register 1 1 */
#define REG_GPIO_ODCR1S1           (0x400E1314U) /**< \brief (GPIO) Output Driving Capability Register 1 - Set 1 */
#define REG_GPIO_ODCR1C1           (0x400E1318U) /**< \brief (GPIO) Output Driving Capability Register 1 - Clear 1 */
#define REG_GPIO_ODCR1T1           (0x400E131CU) /**< \brief (GPIO) Output Driving Capability Register 1 - Toggle 1 */
#define REG_GPIO_OSRR01            (0x400E1330U) /**< \brief (GPIO) Output Slew Rate Register 0 1 */
#define REG_GPIO_OSRR0S1           (0x400E1334U) /**< \brief (GPIO) Output Slew Rate Register 0 - Set 1 */
#define REG_GPIO_OSRR0C1           (0x400E1338U) /**< \brief (GPIO) Output Slew Rate Register 0 - Clear 1 */
#define REG_GPIO_OSRR0T1           (0x400E133CU) /**< \brief (GPIO) Output Slew Rate Register 0 - Toggle 1 */
#define REG_GPIO_STER1             (0x400E1360U) /**< \brief (GPIO) Schmitt Trigger Enable Register 1 */
#define REG_GPIO_STERS1            (0x400E1364U) /**< \brief (GPIO) Schmitt Trigger Enable Register - Set 1 */
#define REG_GPIO_STERC1            (0x400E1368U) /**< \brief (GPIO) Schmitt Trigger Enable Register - Clear 1 */
#define REG_GPIO_STERT1            (0x400E136CU) /**< \brief (GPIO) Schmitt Trigger Enable Register - Toggle 1 */
#define REG_GPIO_EVER1             (0x400E1380U) /**< \brief (GPIO) Event Enable Register 1 */
#define REG_GPIO_EVERS1            (0x400E1384U) /**< \brief (GPIO) Event Enable Register - Set 1 */
#define REG_GPIO_EVERC1            (0x400E1388U) /**< \brief (GPIO) Event Enable Register - Clear 1 */
#define REG_GPIO_EVERT1            (0x400E138CU) /**< \brief (GPIO) Event Enable Register - Toggle 1 */
#define REG_GPIO_LOCK1             (0x400E13A0U) /**< \brief (GPIO) Lock Register 1 */
#define REG_GPIO_LOCKS1            (0x400E13A4U) /**< \brief (GPIO) Lock Register - Set 1 */
#define REG_GPIO_LOCKC1            (0x400E13A8U) /**< \brief (GPIO) Lock Register - Clear 1 */
#define REG_GPIO_LOCKT1            (0x400E13ACU) /**< \brief (GPIO) Lock Register - Toggle 1 */
#define REG_GPIO_UNLOCK1           (0x400E13E0U) /**< \brief (GPIO) Unlock Register 1 */
#define REG_GPIO_ASR1              (0x400E13E4U) /**< \brief (GPIO) Access Status Register 1 */
#define REG_GPIO_PARAMETER1        (0x400E13F8U) /**< \brief (GPIO) Parameter Register 1 */
#define REG_GPIO_VERSION1          (0x400E13FCU) /**< \brief (GPIO) Version Register 1 */
#define REG_GPIO_GPER2             (0x400E1400U) /**< \brief (GPIO) GPIO Enable Register 2 */
#define REG_GPIO_GPERS2            (0x400E1404U) /**< \brief (GPIO) GPIO Enable Register - Set 2 */
#define REG_GPIO_GPERC2            (0x400E1408U) /**< \brief (GPIO) GPIO Enable Register - Clear 2 */
#define REG_GPIO_GPERT2            (0x400E140CU) /**< \brief (GPIO) GPIO Enable Register - Toggle 2 */
#define REG_GPIO_PMR02             (0x400E1410U) /**< \brief (GPIO) Peripheral Mux Register 0 2 */
#define REG_GPIO_PMR0S2            (0x400E1414U) /**< \brief (GPIO) Peripheral Mux Register 0 - Set 2 */
#define REG_GPIO_PMR0C2            (0x400E1418U) /**< \brief (GPIO) Peripheral Mux Register 0 - Clear 2 */
#define REG_GPIO_PMR0T2            (0x400E141CU) /**< \brief (GPIO) Peripheral Mux Register 0 - Toggle 2 */
#define REG_GPIO_PMR12             (0x400E1420U) /**< \brief (GPIO) Peripheral Mux Register 1 2 */
#define REG_GPIO_PMR1S2            (0x400E1424U) /**< \brief (GPIO) Peripheral Mux Register 1 - Set 2 */
#define REG_GPIO_PMR1C2            (0x400E1428U) /**< \brief (GPIO) Peripheral Mux Register 1 - Clear 2 */
#define REG_GPIO_PMR1T2            (0x400E142CU) /**< \brief (GPIO) Peripheral Mux Register 1 - Toggle 2 */
#define REG_GPIO_PMR22             (0x400E1430U) /**< \brief (GPIO) Peripheral Mux Register 2 2 */
#define REG_GPIO_PMR2S2            (0x400E1434U) /**< \brief (GPIO) Peripheral Mux Register 2 - Set 2 */
#define REG_GPIO_PMR2C2            (0x400E1438U) /**< \brief (GPIO) Peripheral Mux Register 2 - Clear 2 */
#define REG_GPIO_PMR2T2            (0x400E143CU) /**< \brief (GPIO) Peripheral Mux Register 2 - Toggle 2 */
#define REG_GPIO_ODER2             (0x400E1440U) /**< \brief (GPIO) Output Driver Enable Register 2 */
#define REG_GPIO_ODERS2            (0x400E1444U) /**< \brief (GPIO) Output Driver Enable Register - Set 2 */
#define REG_GPIO_ODERC2            (0x400E1448U) /**< \brief (GPIO) Output Driver Enable Register - Clear 2 */
#define REG_GPIO_ODERT2            (0x400E144CU) /**< \brief (GPIO) Output Driver Enable Register - Toggle 2 */
#define REG_GPIO_OVR2              (0x400E1450U) /**< \brief (GPIO) Output Value Register 2 */
#define REG_GPIO_OVRS2             (0x400E1454U) /**< \brief (GPIO) Output Value Register - Set 2 */
#define REG_GPIO_OVRC2             (0x400E1458U) /**< \brief (GPIO) Output Value Register - Clear 2 */
#define REG_GPIO_OVRT2             (0x400E145CU) /**< \brief (GPIO) Output Value Register - Toggle 2 */
#define REG_GPIO_PVR2              (0x400E1460U) /**< \brief (GPIO) Pin Value Register 2 */
#define REG_GPIO_PUER2             (0x400E1470U) /**< \brief (GPIO) Pull-up Enable Register 2 */
#define REG_GPIO_PUERS2            (0x400E1474U) /**< \brief (GPIO) Pull-up Enable Register - Set 2 */
#define REG_GPIO_PUERC2            (0x400E1478U) /**< \brief (GPIO) Pull-up Enable Register - Clear 2 */
#define REG_GPIO_PUERT2            (0x400E147CU) /**< \brief (GPIO) Pull-up Enable Register - Toggle 2 */
#define REG_GPIO_PDER2             (0x400E1480U) /**< \brief (GPIO) Pull-down Enable Register 2 */
#define REG_GPIO_PDERS2            (0x400E1484U) /**< \brief (GPIO) Pull-down Enable Register - Set 2 */
#define REG_GPIO_PDERC2            (0x400E1488U) /**< \brief (GPIO) Pull-down Enable Register - Clear 2 */
#define REG_GPIO_PDERT2            (0x400E148CU) /**< \brief (GPIO) Pull-down Enable Register - Toggle 2 */
#define REG_GPIO_IER2              (0x400E1490U) /**< \brief (GPIO) Interrupt Enable Register 2 */
#define REG_GPIO_IERS2             (0x400E1494U) /**< \brief (GPIO) Interrupt Enable Register - Set 2 */
#define REG_GPIO_IERC2             (0x400E1498U) /**< \brief (GPIO) Interrupt Enable Register - Clear 2 */
#define REG_GPIO_IERT2             (0x400E149CU) /**< \brief (GPIO) Interrupt Enable Register - Toggle 2 */
#define REG_GPIO_IMR02             (0x400E14A0U) /**< \brief (GPIO) Interrupt Mode Register 0 2 */
#define REG_GPIO_IMR0S2            (0x400E14A4U) /**< \brief (GPIO) Interrupt Mode Register 0 - Set 2 */
#define REG_GPIO_IMR0C2            (0x400E14A8U) /**< \brief (GPIO) Interrupt Mode Register 0 - Clear 2 */
#define REG_GPIO_IMR0T2            (0x400E14ACU) /**< \brief (GPIO) Interrupt Mode Register 0 - Toggle 2 */
#define REG_GPIO_IMR12             (0x400E14B0U) /**< \brief (GPIO) Interrupt Mode Register 1 2 */
#define REG_GPIO_IMR1S2            (0x400E14B4U) /**< \brief (GPIO) Interrupt Mode Register 1 - Set 2 */
#define REG_GPIO_IMR1C2            (0x400E14B8U) /**< \brief (GPIO) Interrupt Mode Register 1 - Clear 2 */
#define REG_GPIO_IMR1T2            (0x400E14BCU) /**< \brief (GPIO) Interrupt Mode Register 1 - Toggle 2 */
#define REG_GPIO_GFER2             (0x400E14C0U) /**< \brief (GPIO) Glitch Filter Enable Register 2 */
#define REG_GPIO_GFERS2            (0x400E14C4U) /**< \brief (GPIO) Glitch Filter Enable Register - Set 2 */
#define REG_GPIO_GFERC2            (0x400E14C8U) /**< \brief (GPIO) Glitch Filter Enable Register - Clear 2 */
#define REG_GPIO_GFERT2            (0x400E14CCU) /**< \brief (GPIO) Glitch Filter Enable Register - Toggle 2 */
#define REG_GPIO_IFR2              (0x400E14D0U) /**< \brief (GPIO) Interrupt Flag Register 2 */
#define REG_GPIO_IFRC2             (0x400E14D8U) /**< \brief (GPIO) Interrupt Flag Register - Clear 2 */
#define REG_GPIO_ODMER2            (0x400E14E0U) /**< \brief (GPIO) Open Drain Mode Register 2 */
#define REG_GPIO_ODMERS2           (0x400E14E4U) /**< \brief (GPIO) Open Drain Mode Register - Set 2 */
#define REG_GPIO_ODMERC2           (0x400E14E8U) /**< \brief (GPIO) Open Drain Mode Register - Clear 2 */
#define REG_GPIO_ODMERT2           (0x400E14ECU) /**< \brief (GPIO) Open Drain Mode Register - Toggle 2 */
#define REG_GPIO_ODCR02            (0x400E1500U) /**< \brief (GPIO) Output Driving Capability Register 0 2 */
#define REG_GPIO_ODCR0S2           (0x400E1504U) /**< \brief (GPIO) Output Driving Capability Register 0 - Set 2 */
#define REG_GPIO_ODCR0C2           (0x400E1508U) /**< \brief (GPIO) Output Driving Capability Register 0 - Clear 2 */
#define REG_GPIO_ODCR0T2           (0x400E150CU) /**< \brief (GPIO) Output Driving Capability Register 0 - Toggle 2 */
#define REG_GPIO_ODCR12            (0x400E1510U) /**< \brief (GPIO) Output Driving Capability Register 1 2 */
#define REG_GPIO_ODCR1S2           (0x400E1514U) /**< \brief (GPIO) Output Driving Capability Register 1 - Set 2 */
#define REG_GPIO_ODCR1C2           (0x400E1518U) /**< \brief (GPIO) Output Driving Capability Register 1 - Clear 2 */
#define REG_GPIO_ODCR1T2           (0x400E151CU) /**< \brief (GPIO) Output Driving Capability Register 1 - Toggle 2 */
#define REG_GPIO_OSRR02            (0x400E1530U) /**< \brief (GPIO) Output Slew Rate Register 0 2 */
#define REG_GPIO_OSRR0S2           (0x400E1534U) /**< \brief (GPIO) Output Slew Rate Register 0 - Set 2 */
#define REG_GPIO_OSRR0C2           (0x400E1538U) /**< \brief (GPIO) Output Slew Rate Register 0 - Clear 2 */
#define REG_GPIO_OSRR0T2           (0x400E153CU) /**< \brief (GPIO) Output Slew Rate Register 0 - Toggle 2 */
#define REG_GPIO_STER2             (0x400E1560U) /**< \brief (GPIO) Schmitt Trigger Enable Register 2 */
#define REG_GPIO_STERS2            (0x400E1564U) /**< \brief (GPIO) Schmitt Trigger Enable Register - Set 2 */
#define REG_GPIO_STERC2            (0x400E1568U) /**< \brief (GPIO) Schmitt Trigger Enable Register - Clear 2 */
#define REG_GPIO_STERT2            (0x400E156CU) /**< \brief (GPIO) Schmitt Trigger Enable Register - Toggle 2 */
#define REG_GPIO_EVER2             (0x400E1580U) /**< \brief (GPIO) Event Enable Register 2 */
#define REG_GPIO_EVERS2            (0x400E1584U) /**< \brief (GPIO) Event Enable Register - Set 2 */
#define REG_GPIO_EVERC2            (0x400E1588U) /**< \brief (GPIO) Event Enable Register - Clear 2 */
#define REG_GPIO_EVERT2            (0x400E158CU) /**< \brief (GPIO) Event Enable Register - Toggle 2 */
#define REG_GPIO_LOCK2             (0x400E15A0U) /**< \brief (GPIO) Lock Register 2 */
#define REG_GPIO_LOCKS2            (0x400E15A4U) /**< \brief (GPIO) Lock Register - Set 2 */
#define REG_GPIO_LOCKC2            (0x400E15A8U) /**< \brief (GPIO) Lock Register - Clear 2 */
#define REG_GPIO_LOCKT2            (0x400E15ACU) /**< \brief (GPIO) Lock Register - Toggle 2 */
#define REG_GPIO_UNLOCK2           (0x400E15E0U) /**< \brief (GPIO) Unlock Register 2 */
#define REG_GPIO_ASR2              (0x400E15E4U) /**< \brief (GPIO) Access Status Register 2 */
#define REG_GPIO_PARAMETER2        (0x400E15F8U) /**< \brief (GPIO) Parameter Register 2 */
#define REG_GPIO_VERSION2          (0x400E15FCU) /**< \brief (GPIO) Version Register 2 */
#else
#define REG_GPIO_GPER0             (*(RwReg  *)0x400E1000U) /**< \brief (GPIO) GPIO Enable Register 0 */
#define REG_GPIO_GPERS0            (*(WoReg  *)0x400E1004U) /**< \brief (GPIO) GPIO Enable Register - Set 0 */
#define REG_GPIO_GPERC0            (*(WoReg  *)0x400E1008U) /**< \brief (GPIO) GPIO Enable Register - Clear 0 */
#define REG_GPIO_GPERT0            (*(WoReg  *)0x400E100CU) /**< \brief (GPIO) GPIO Enable Register - Toggle 0 */
#define REG_GPIO_PMR00             (*(RwReg  *)0x400E1010U) /**< \brief (GPIO) Peripheral Mux Register 0 0 */
#define REG_GPIO_PMR0S0            (*(WoReg  *)0x400E1014U) /**< \brief (GPIO) Peripheral Mux Register 0 - Set 0 */
#define REG_GPIO_PMR0C0            (*(WoReg  *)0x400E1018U) /**< \brief (GPIO) Peripheral Mux Register 0 - Clear 0 */
#define REG_GPIO_PMR0T0            (*(WoReg  *)0x400E101CU) /**< \brief (GPIO) Peripheral Mux Register 0 - Toggle 0 */
#define REG_GPIO_PMR10             (*(RwReg  *)0x400E1020U) /**< \brief (GPIO) Peripheral Mux Register 1 0 */
#define REG_GPIO_PMR1S0            (*(WoReg  *)0x400E1024U) /**< \brief (GPIO) Peripheral Mux Register 1 - Set 0 */
#define REG_GPIO_PMR1C0            (*(WoReg  *)0x400E1028U) /**< \brief (GPIO) Peripheral Mux Register 1 - Clear 0 */
#define REG_GPIO_PMR1T0            (*(WoReg  *)0x400E102CU) /**< \brief (GPIO) Peripheral Mux Register 1 - Toggle 0 */
#define REG_GPIO_PMR20             (*(RwReg  *)0x400E1030U) /**< \brief (GPIO) Peripheral Mux Register 2 0 */
#define REG_GPIO_PMR2S0            (*(WoReg  *)0x400E1034U) /**< \brief (GPIO) Peripheral Mux Register 2 - Set 0 */
#define REG_GPIO_PMR2C0            (*(WoReg  *)0x400E1038U) /**< \brief (GPIO) Peripheral Mux Register 2 - Clear 0 */
#define REG_GPIO_PMR2T0            (*(WoReg  *)0x400E103CU) /**< \brief (GPIO) Peripheral Mux Register 2 - Toggle 0 */
#define REG_GPIO_ODER0             (*(RwReg  *)0x400E1040U) /**< \brief (GPIO) Output Driver Enable Register 0 */
#define REG_GPIO_ODERS0            (*(WoReg  *)0x400E1044U) /**< \brief (GPIO) Output Driver Enable Register - Set 0 */
#define REG_GPIO_ODERC0            (*(WoReg  *)0x400E1048U) /**< \brief (GPIO) Output Driver Enable Register - Clear 0 */
#define REG_GPIO_ODERT0            (*(WoReg  *)0x400E104CU) /**< \brief (GPIO) Output Driver Enable Register - Toggle 0 */
#define REG_GPIO_OVR0              (*(RwReg  *)0x400E1050U) /**< \brief (GPIO) Output Value Register 0 */
#define REG_GPIO_OVRS0             (*(WoReg  *)0x400E1054U) /**< \brief (GPIO) Output Value Register - Set 0 */
#define REG_GPIO_OVRC0             (*(WoReg  *)0x400E1058U) /**< \brief (GPIO) Output Value Register - Clear 0 */
#define REG_GPIO_OVRT0             (*(WoReg  *)0x400E105CU) /**< \brief (GPIO) Output Value Register - Toggle 0 */
#define REG_GPIO_PVR0              (*(RoReg  *)0x400E1060U) /**< \brief (GPIO) Pin Value Register 0 */
#define REG_GPIO_PUER0             (*(RwReg  *)0x400E1070U) /**< \brief (GPIO) Pull-up Enable Register 0 */
#define REG_GPIO_PUERS0            (*(WoReg  *)0x400E1074U) /**< \brief (GPIO) Pull-up Enable Register - Set 0 */
#define REG_GPIO_PUERC0            (*(WoReg  *)0x400E1078U) /**< \brief (GPIO) Pull-up Enable Register - Clear 0 */
#define REG_GPIO_PUERT0            (*(WoReg  *)0x400E107CU) /**< \brief (GPIO) Pull-up Enable Register - Toggle 0 */
#define REG_GPIO_PDER0             (*(RwReg  *)0x400E1080U) /**< \brief (GPIO) Pull-down Enable Register 0 */
#define REG_GPIO_PDERS0            (*(WoReg  *)0x400E1084U) /**< \brief (GPIO) Pull-down Enable Register - Set 0 */
#define REG_GPIO_PDERC0            (*(WoReg  *)0x400E1088U) /**< \brief (GPIO) Pull-down Enable Register - Clear 0 */
#define REG_GPIO_PDERT0            (*(WoReg  *)0x400E108CU) /**< \brief (GPIO) Pull-down Enable Register - Toggle 0 */
#define REG_GPIO_IER0              (*(RwReg  *)0x400E1090U) /**< \brief (GPIO) Interrupt Enable Register 0 */
#define REG_GPIO_IERS0             (*(WoReg  *)0x400E1094U) /**< \brief (GPIO) Interrupt Enable Register - Set 0 */
#define REG_GPIO_IERC0             (*(WoReg  *)0x400E1098U) /**< \brief (GPIO) Interrupt Enable Register - Clear 0 */
#define REG_GPIO_IERT0             (*(WoReg  *)0x400E109CU) /**< \brief (GPIO) Interrupt Enable Register - Toggle 0 */
#define REG_GPIO_IMR00             (*(RwReg  *)0x400E10A0U) /**< \brief (GPIO) Interrupt Mode Register 0 0 */
#define REG_GPIO_IMR0S0            (*(WoReg  *)0x400E10A4U) /**< \brief (GPIO) Interrupt Mode Register 0 - Set 0 */
#define REG_GPIO_IMR0C0            (*(WoReg  *)0x400E10A8U) /**< \brief (GPIO) Interrupt Mode Register 0 - Clear 0 */
#define REG_GPIO_IMR0T0            (*(WoReg  *)0x400E10ACU) /**< \brief (GPIO) Interrupt Mode Register 0 - Toggle 0 */
#define REG_GPIO_IMR10             (*(RwReg  *)0x400E10B0U) /**< \brief (GPIO) Interrupt Mode Register 1 0 */
#define REG_GPIO_IMR1S0            (*(WoReg  *)0x400E10B4U) /**< \brief (GPIO) Interrupt Mode Register 1 - Set 0 */
#define REG_GPIO_IMR1C0            (*(WoReg  *)0x400E10B8U) /**< \brief (GPIO) Interrupt Mode Register 1 - Clear 0 */
#define REG_GPIO_IMR1T0            (*(WoReg  *)0x400E10BCU) /**< \brief (GPIO) Interrupt Mode Register 1 - Toggle 0 */
#define REG_GPIO_GFER0             (*(RwReg  *)0x400E10C0U) /**< \brief (GPIO) Glitch Filter Enable Register 0 */
#define REG_GPIO_GFERS0            (*(WoReg  *)0x400E10C4U) /**< \brief (GPIO) Glitch Filter Enable Register - Set 0 */
#define REG_GPIO_GFERC0            (*(WoReg  *)0x400E10C8U) /**< \brief (GPIO) Glitch Filter Enable Register - Clear 0 */
#define REG_GPIO_GFERT0            (*(WoReg  *)0x400E10CCU) /**< \brief (GPIO) Glitch Filter Enable Register - Toggle 0 */
#define REG_GPIO_IFR0              (*(RoReg  *)0x400E10D0U) /**< \brief (GPIO) Interrupt Flag Register 0 */
#define REG_GPIO_IFRC0             (*(WoReg  *)0x400E10D8U) /**< \brief (GPIO) Interrupt Flag Register - Clear 0 */
#define REG_GPIO_ODMER0            (*(RwReg  *)0x400E10E0U) /**< \brief (GPIO) Open Drain Mode Register 0 */
#define REG_GPIO_ODMERS0           (*(WoReg  *)0x400E10E4U) /**< \brief (GPIO) Open Drain Mode Register - Set 0 */
#define REG_GPIO_ODMERC0           (*(WoReg  *)0x400E10E8U) /**< \brief (GPIO) Open Drain Mode Register - Clear 0 */
#define REG_GPIO_ODMERT0           (*(WoReg  *)0x400E10ECU) /**< \brief (GPIO) Open Drain Mode Register - Toggle 0 */
#define REG_GPIO_ODCR00            (*(RwReg  *)0x400E1100U) /**< \brief (GPIO) Output Driving Capability Register 0 0 */
#define REG_GPIO_ODCR0S0           (*(RwReg  *)0x400E1104U) /**< \brief (GPIO) Output Driving Capability Register 0 - Set 0 */
#define REG_GPIO_ODCR0C0           (*(RwReg  *)0x400E1108U) /**< \brief (GPIO) Output Driving Capability Register 0 - Clear 0 */
#define REG_GPIO_ODCR0T0           (*(RwReg  *)0x400E110CU) /**< \brief (GPIO) Output Driving Capability Register 0 - Toggle 0 */
#define REG_GPIO_ODCR10            (*(RwReg  *)0x400E1110U) /**< \brief (GPIO) Output Driving Capability Register 1 0 */
#define REG_GPIO_ODCR1S0           (*(RwReg  *)0x400E1114U) /**< \brief (GPIO) Output Driving Capability Register 1 - Set 0 */
#define REG_GPIO_ODCR1C0           (*(RwReg  *)0x400E1118U) /**< \brief (GPIO) Output Driving Capability Register 1 - Clear 0 */
#define REG_GPIO_ODCR1T0           (*(RwReg  *)0x400E111CU) /**< \brief (GPIO) Output Driving Capability Register 1 - Toggle 0 */
#define REG_GPIO_OSRR00            (*(RwReg  *)0x400E1130U) /**< \brief (GPIO) Output Slew Rate Register 0 0 */
#define REG_GPIO_OSRR0S0           (*(RwReg  *)0x400E1134U) /**< \brief (GPIO) Output Slew Rate Register 0 - Set 0 */
#define REG_GPIO_OSRR0C0           (*(RwReg  *)0x400E1138U) /**< \brief (GPIO) Output Slew Rate Register 0 - Clear 0 */
#define REG_GPIO_OSRR0T0           (*(RwReg  *)0x400E113CU) /**< \brief (GPIO) Output Slew Rate Register 0 - Toggle 0 */
#define REG_GPIO_STER0             (*(RwReg  *)0x400E1160U) /**< \brief (GPIO) Schmitt Trigger Enable Register 0 */
#define REG_GPIO_STERS0            (*(RwReg  *)0x400E1164U) /**< \brief (GPIO) Schmitt Trigger Enable Register - Set 0 */
#define REG_GPIO_STERC0            (*(RwReg  *)0x400E1168U) /**< \brief (GPIO) Schmitt Trigger Enable Register - Clear 0 */
#define REG_GPIO_STERT0            (*(RwReg  *)0x400E116CU) /**< \brief (GPIO) Schmitt Trigger Enable Register - Toggle 0 */
#define REG_GPIO_EVER0             (*(RwReg  *)0x400E1180U) /**< \brief (GPIO) Event Enable Register 0 */
#define REG_GPIO_EVERS0            (*(WoReg  *)0x400E1184U) /**< \brief (GPIO) Event Enable Register - Set 0 */
#define REG_GPIO_EVERC0            (*(WoReg  *)0x400E1188U) /**< \brief (GPIO) Event Enable Register - Clear 0 */
#define REG_GPIO_EVERT0            (*(WoReg  *)0x400E118CU) /**< \brief (GPIO) Event Enable Register - Toggle 0 */
#define REG_GPIO_LOCK0             (*(RwReg  *)0x400E11A0U) /**< \brief (GPIO) Lock Register 0 */
#define REG_GPIO_LOCKS0            (*(WoReg  *)0x400E11A4U) /**< \brief (GPIO) Lock Register - Set 0 */
#define REG_GPIO_LOCKC0            (*(WoReg  *)0x400E11A8U) /**< \brief (GPIO) Lock Register - Clear 0 */
#define REG_GPIO_LOCKT0            (*(WoReg  *)0x400E11ACU) /**< \brief (GPIO) Lock Register - Toggle 0 */
#define REG_GPIO_UNLOCK0           (*(WoReg  *)0x400E11E0U) /**< \brief (GPIO) Unlock Register 0 */
#define REG_GPIO_ASR0              (*(RwReg  *)0x400E11E4U) /**< \brief (GPIO) Access Status Register 0 */
#define REG_GPIO_PARAMETER0        (*(RoReg  *)0x400E11F8U) /**< \brief (GPIO) Parameter Register 0 */
#define REG_GPIO_VERSION0          (*(RoReg  *)0x400E11FCU) /**< \brief (GPIO) Version Register 0 */
#define REG_GPIO_GPER1             (*(RwReg  *)0x400E1200U) /**< \brief (GPIO) GPIO Enable Register 1 */
#define REG_GPIO_GPERS1            (*(WoReg  *)0x400E1204U) /**< \brief (GPIO) GPIO Enable Register - Set 1 */
#define REG_GPIO_GPERC1            (*(WoReg  *)0x400E1208U) /**< \brief (GPIO) GPIO Enable Register - Clear 1 */
#define REG_GPIO_GPERT1            (*(WoReg  *)0x400E120CU) /**< \brief (GPIO) GPIO Enable Register - Toggle 1 */
#define REG_GPIO_PMR01             (*(RwReg  *)0x400E1210U) /**< \brief (GPIO) Peripheral Mux Register 0 1 */
#define REG_GPIO_PMR0S1            (*(WoReg  *)0x400E1214U) /**< \brief (GPIO) Peripheral Mux Register 0 - Set 1 */
#define REG_GPIO_PMR0C1            (*(WoReg  *)0x400E1218U) /**< \brief (GPIO) Peripheral Mux Register 0 - Clear 1 */
#define REG_GPIO_PMR0T1            (*(WoReg  *)0x400E121CU) /**< \brief (GPIO) Peripheral Mux Register 0 - Toggle 1 */
#define REG_GPIO_PMR11             (*(RwReg  *)0x400E1220U) /**< \brief (GPIO) Peripheral Mux Register 1 1 */
#define REG_GPIO_PMR1S1            (*(WoReg  *)0x400E1224U) /**< \brief (GPIO) Peripheral Mux Register 1 - Set 1 */
#define REG_GPIO_PMR1C1            (*(WoReg  *)0x400E1228U) /**< \brief (GPIO) Peripheral Mux Register 1 - Clear 1 */
#define REG_GPIO_PMR1T1            (*(WoReg  *)0x400E122CU) /**< \brief (GPIO) Peripheral Mux Register 1 - Toggle 1 */
#define REG_GPIO_PMR21             (*(RwReg  *)0x400E1230U) /**< \brief (GPIO) Peripheral Mux Register 2 1 */
#define REG_GPIO_PMR2S1            (*(WoReg  *)0x400E1234U) /**< \brief (GPIO) Peripheral Mux Register 2 - Set 1 */
#define REG_GPIO_PMR2C1            (*(WoReg  *)0x400E1238U) /**< \brief (GPIO) Peripheral Mux Register 2 - Clear 1 */
#define REG_GPIO_PMR2T1            (*(WoReg  *)0x400E123CU) /**< \brief (GPIO) Peripheral Mux Register 2 - Toggle 1 */
#define REG_GPIO_ODER1             (*(RwReg  *)0x400E1240U) /**< \brief (GPIO) Output Driver Enable Register 1 */
#define REG_GPIO_ODERS1            (*(WoReg  *)0x400E1244U) /**< \brief (GPIO) Output Driver Enable Register - Set 1 */
#define REG_GPIO_ODERC1            (*(WoReg  *)0x400E1248U) /**< \brief (GPIO) Output Driver Enable Register - Clear 1 */
#define REG_GPIO_ODERT1            (*(WoReg  *)0x400E124CU) /**< \brief (GPIO) Output Driver Enable Register - Toggle 1 */
#define REG_GPIO_OVR1              (*(RwReg  *)0x400E1250U) /**< \brief (GPIO) Output Value Register 1 */
#define REG_GPIO_OVRS1             (*(WoReg  *)0x400E1254U) /**< \brief (GPIO) Output Value Register - Set 1 */
#define REG_GPIO_OVRC1             (*(WoReg  *)0x400E1258U) /**< \brief (GPIO) Output Value Register - Clear 1 */
#define REG_GPIO_OVRT1             (*(WoReg  *)0x400E125CU) /**< \brief (GPIO) Output Value Register - Toggle 1 */
#define REG_GPIO_PVR1              (*(RoReg  *)0x400E1260U) /**< \brief (GPIO) Pin Value Register 1 */
#define REG_GPIO_PUER1             (*(RwReg  *)0x400E1270U) /**< \brief (GPIO) Pull-up Enable Register 1 */
#define REG_GPIO_PUERS1            (*(WoReg  *)0x400E1274U) /**< \brief (GPIO) Pull-up Enable Register - Set 1 */
#define REG_GPIO_PUERC1            (*(WoReg  *)0x400E1278U) /**< \brief (GPIO) Pull-up Enable Register - Clear 1 */
#define REG_GPIO_PUERT1            (*(WoReg  *)0x400E127CU) /**< \brief (GPIO) Pull-up Enable Register - Toggle 1 */
#define REG_GPIO_PDER1             (*(RwReg  *)0x400E1280U) /**< \brief (GPIO) Pull-down Enable Register 1 */
#define REG_GPIO_PDERS1            (*(WoReg  *)0x400E1284U) /**< \brief (GPIO) Pull-down Enable Register - Set 1 */
#define REG_GPIO_PDERC1            (*(WoReg  *)0x400E1288U) /**< \brief (GPIO) Pull-down Enable Register - Clear 1 */
#define REG_GPIO_PDERT1            (*(WoReg  *)0x400E128CU) /**< \brief (GPIO) Pull-down Enable Register - Toggle 1 */
#define REG_GPIO_IER1              (*(RwReg  *)0x400E1290U) /**< \brief (GPIO) Interrupt Enable Register 1 */
#define REG_GPIO_IERS1             (*(WoReg  *)0x400E1294U) /**< \brief (GPIO) Interrupt Enable Register - Set 1 */
#define REG_GPIO_IERC1             (*(WoReg  *)0x400E1298U) /**< \brief (GPIO) Interrupt Enable Register - Clear 1 */
#define REG_GPIO_IERT1             (*(WoReg  *)0x400E129CU) /**< \brief (GPIO) Interrupt Enable Register - Toggle 1 */
#define REG_GPIO_IMR01             (*(RwReg  *)0x400E12A0U) /**< \brief (GPIO) Interrupt Mode Register 0 1 */
#define REG_GPIO_IMR0S1            (*(WoReg  *)0x400E12A4U) /**< \brief (GPIO) Interrupt Mode Register 0 - Set 1 */
#define REG_GPIO_IMR0C1            (*(WoReg  *)0x400E12A8U) /**< \brief (GPIO) Interrupt Mode Register 0 - Clear 1 */
#define REG_GPIO_IMR0T1            (*(WoReg  *)0x400E12ACU) /**< \brief (GPIO) Interrupt Mode Register 0 - Toggle 1 */
#define REG_GPIO_IMR11             (*(RwReg  *)0x400E12B0U) /**< \brief (GPIO) Interrupt Mode Register 1 1 */
#define REG_GPIO_IMR1S1            (*(WoReg  *)0x400E12B4U) /**< \brief (GPIO) Interrupt Mode Register 1 - Set 1 */
#define REG_GPIO_IMR1C1            (*(WoReg  *)0x400E12B8U) /**< \brief (GPIO) Interrupt Mode Register 1 - Clear 1 */
#define REG_GPIO_IMR1T1            (*(WoReg  *)0x400E12BCU) /**< \brief (GPIO) Interrupt Mode Register 1 - Toggle 1 */
#define REG_GPIO_GFER1             (*(RwReg  *)0x400E12C0U) /**< \brief (GPIO) Glitch Filter Enable Register 1 */
#define REG_GPIO_GFERS1            (*(WoReg  *)0x400E12C4U) /**< \brief (GPIO) Glitch Filter Enable Register - Set 1 */
#define REG_GPIO_GFERC1            (*(WoReg  *)0x400E12C8U) /**< \brief (GPIO) Glitch Filter Enable Register - Clear 1 */
#define REG_GPIO_GFERT1            (*(WoReg  *)0x400E12CCU) /**< \brief (GPIO) Glitch Filter Enable Register - Toggle 1 */
#define REG_GPIO_IFR1              (*(RoReg  *)0x400E12D0U) /**< \brief (GPIO) Interrupt Flag Register 1 */
#define REG_GPIO_IFRC1             (*(WoReg  *)0x400E12D8U) /**< \brief (GPIO) Interrupt Flag Register - Clear 1 */
#define REG_GPIO_ODMER1            (*(RwReg  *)0x400E12E0U) /**< \brief (GPIO) Open Drain Mode Register 1 */
#define REG_GPIO_ODMERS1           (*(WoReg  *)0x400E12E4U) /**< \brief (GPIO) Open Drain Mode Register - Set 1 */
#define REG_GPIO_ODMERC1           (*(WoReg  *)0x400E12E8U) /**< \brief (GPIO) Open Drain Mode Register - Clear 1 */
#define REG_GPIO_ODMERT1           (*(WoReg  *)0x400E12ECU) /**< \brief (GPIO) Open Drain Mode Register - Toggle 1 */
#define REG_GPIO_ODCR01            (*(RwReg  *)0x400E1300U) /**< \brief (GPIO) Output Driving Capability Register 0 1 */
#define REG_GPIO_ODCR0S1           (*(RwReg  *)0x400E1304U) /**< \brief (GPIO) Output Driving Capability Register 0 - Set 1 */
#define REG_GPIO_ODCR0C1           (*(RwReg  *)0x400E1308U) /**< \brief (GPIO) Output Driving Capability Register 0 - Clear 1 */
#define REG_GPIO_ODCR0T1           (*(RwReg  *)0x400E130CU) /**< \brief (GPIO) Output Driving Capability Register 0 - Toggle 1 */
#define REG_GPIO_ODCR11            (*(RwReg  *)0x400E1310U) /**< \brief (GPIO) Output Driving Capability Register 1 1 */
#define REG_GPIO_ODCR1S1           (*(RwReg  *)0x400E1314U) /**< \brief (GPIO) Output Driving Capability Register 1 - Set 1 */
#define REG_GPIO_ODCR1C1           (*(RwReg  *)0x400E1318U) /**< \brief (GPIO) Output Driving Capability Register 1 - Clear 1 */
#define REG_GPIO_ODCR1T1           (*(RwReg  *)0x400E131CU) /**< \brief (GPIO) Output Driving Capability Register 1 - Toggle 1 */
#define REG_GPIO_OSRR01            (*(RwReg  *)0x400E1330U) /**< \brief (GPIO) Output Slew Rate Register 0 1 */
#define REG_GPIO_OSRR0S1           (*(RwReg  *)0x400E1334U) /**< \brief (GPIO) Output Slew Rate Register 0 - Set 1 */
#define REG_GPIO_OSRR0C1           (*(RwReg  *)0x400E1338U) /**< \brief (GPIO) Output Slew Rate Register 0 - Clear 1 */
#define REG_GPIO_OSRR0T1           (*(RwReg  *)0x400E133CU) /**< \brief (GPIO) Output Slew Rate Register 0 - Toggle 1 */
#define REG_GPIO_STER1             (*(RwReg  *)0x400E1360U) /**< \brief (GPIO) Schmitt Trigger Enable Register 1 */
#define REG_GPIO_STERS1            (*(RwReg  *)0x400E1364U) /**< \brief (GPIO) Schmitt Trigger Enable Register - Set 1 */
#define REG_GPIO_STERC1            (*(RwReg  *)0x400E1368U) /**< \brief (GPIO) Schmitt Trigger Enable Register - Clear 1 */
#define REG_GPIO_STERT1            (*(RwReg  *)0x400E136CU) /**< \brief (GPIO) Schmitt Trigger Enable Register - Toggle 1 */
#define REG_GPIO_EVER1             (*(RwReg  *)0x400E1380U) /**< \brief (GPIO) Event Enable Register 1 */
#define REG_GPIO_EVERS1            (*(WoReg  *)0x400E1384U) /**< \brief (GPIO) Event Enable Register - Set 1 */
#define REG_GPIO_EVERC1            (*(WoReg  *)0x400E1388U) /**< \brief (GPIO) Event Enable Register - Clear 1 */
#define REG_GPIO_EVERT1            (*(WoReg  *)0x400E138CU) /**< \brief (GPIO) Event Enable Register - Toggle 1 */
#define REG_GPIO_LOCK1             (*(RwReg  *)0x400E13A0U) /**< \brief (GPIO) Lock Register 1 */
#define REG_GPIO_LOCKS1            (*(WoReg  *)0x400E13A4U) /**< \brief (GPIO) Lock Register - Set 1 */
#define REG_GPIO_LOCKC1            (*(WoReg  *)0x400E13A8U) /**< \brief (GPIO) Lock Register - Clear 1 */
#define REG_GPIO_LOCKT1            (*(WoReg  *)0x400E13ACU) /**< \brief (GPIO) Lock Register - Toggle 1 */
#define REG_GPIO_UNLOCK1           (*(WoReg  *)0x400E13E0U) /**< \brief (GPIO) Unlock Register 1 */
#define REG_GPIO_ASR1              (*(RwReg  *)0x400E13E4U) /**< \brief (GPIO) Access Status Register 1 */
#define REG_GPIO_PARAMETER1        (*(RoReg  *)0x400E13F8U) /**< \brief (GPIO) Parameter Register 1 */
#define REG_GPIO_VERSION1          (*(RoReg  *)0x400E13FCU) /**< \brief (GPIO) Version Register 1 */
#define REG_GPIO_GPER2             (*(RwReg  *)0x400E1400U) /**< \brief (GPIO) GPIO Enable Register 2 */
#define REG_GPIO_GPERS2            (*(WoReg  *)0x400E1404U) /**< \brief (GPIO) GPIO Enable Register - Set 2 */
#define REG_GPIO_GPERC2            (*(WoReg  *)0x400E1408U) /**< \brief (GPIO) GPIO Enable Register - Clear 2 */
#define REG_GPIO_GPERT2            (*(WoReg  *)0x400E140CU) /**< \brief (GPIO) GPIO Enable Register - Toggle 2 */
#define REG_GPIO_PMR02             (*(RwReg  *)0x400E1410U) /**< \brief (GPIO) Peripheral Mux Register 0 2 */
#define REG_GPIO_PMR0S2            (*(WoReg  *)0x400E1414U) /**< \brief (GPIO) Peripheral Mux Register 0 - Set 2 */
#define REG_GPIO_PMR0C2            (*(WoReg  *)0x400E1418U) /**< \brief (GPIO) Peripheral Mux Register 0 - Clear 2 */
#define REG_GPIO_PMR0T2            (*(WoReg  *)0x400E141CU) /**< \brief (GPIO) Peripheral Mux Register 0 - Toggle 2 */
#define REG_GPIO_PMR12             (*(RwReg  *)0x400E1420U) /**< \brief (GPIO) Peripheral Mux Register 1 2 */
#define REG_GPIO_PMR1S2            (*(WoReg  *)0x400E1424U) /**< \brief (GPIO) Peripheral Mux Register 1 - Set 2 */
#define REG_GPIO_PMR1C2            (*(WoReg  *)0x400E1428U) /**< \brief (GPIO) Peripheral Mux Register 1 - Clear 2 */
#define REG_GPIO_PMR1T2            (*(WoReg  *)0x400E142CU) /**< \brief (GPIO) Peripheral Mux Register 1 - Toggle 2 */
#define REG_GPIO_PMR22             (*(RwReg  *)0x400E1430U) /**< \brief (GPIO) Peripheral Mux Register 2 2 */
#define REG_GPIO_PMR2S2            (*(WoReg  *)0x400E1434U) /**< \brief (GPIO) Peripheral Mux Register 2 - Set 2 */
#define REG_GPIO_PMR2C2            (*(WoReg  *)0x400E1438U) /**< \brief (GPIO) Peripheral Mux Register 2 - Clear 2 */
#define REG_GPIO_PMR2T2            (*(WoReg  *)0x400E143CU) /**< \brief (GPIO) Peripheral Mux Register 2 - Toggle 2 */
#define REG_GPIO_ODER2             (*(RwReg  *)0x400E1440U) /**< \brief (GPIO) Output Driver Enable Register 2 */
#define REG_GPIO_ODERS2            (*(WoReg  *)0x400E1444U) /**< \brief (GPIO) Output Driver Enable Register - Set 2 */
#define REG_GPIO_ODERC2            (*(WoReg  *)0x400E1448U) /**< \brief (GPIO) Output Driver Enable Register - Clear 2 */
#define REG_GPIO_ODERT2            (*(WoReg  *)0x400E144CU) /**< \brief (GPIO) Output Driver Enable Register - Toggle 2 */
#define REG_GPIO_OVR2              (*(RwReg  *)0x400E1450U) /**< \brief (GPIO) Output Value Register 2 */
#define REG_GPIO_OVRS2             (*(WoReg  *)0x400E1454U) /**< \brief (GPIO) Output Value Register - Set 2 */
#define REG_GPIO_OVRC2             (*(WoReg  *)0x400E1458U) /**< \brief (GPIO) Output Value Register - Clear 2 */
#define REG_GPIO_OVRT2             (*(WoReg  *)0x400E145CU) /**< \brief (GPIO) Output Value Register - Toggle 2 */
#define REG_GPIO_PVR2              (*(RoReg  *)0x400E1460U) /**< \brief (GPIO) Pin Value Register 2 */
#define REG_GPIO_PUER2             (*(RwReg  *)0x400E1470U) /**< \brief (GPIO) Pull-up Enable Register 2 */
#define REG_GPIO_PUERS2            (*(WoReg  *)0x400E1474U) /**< \brief (GPIO) Pull-up Enable Register - Set 2 */
#define REG_GPIO_PUERC2            (*(WoReg  *)0x400E1478U) /**< \brief (GPIO) Pull-up Enable Register - Clear 2 */
#define REG_GPIO_PUERT2            (*(WoReg  *)0x400E147CU) /**< \brief (GPIO) Pull-up Enable Register - Toggle 2 */
#define REG_GPIO_PDER2             (*(RwReg  *)0x400E1480U) /**< \brief (GPIO) Pull-down Enable Register 2 */
#define REG_GPIO_PDERS2            (*(WoReg  *)0x400E1484U) /**< \brief (GPIO) Pull-down Enable Register - Set 2 */
#define REG_GPIO_PDERC2            (*(WoReg  *)0x400E1488U) /**< \brief (GPIO) Pull-down Enable Register - Clear 2 */
#define REG_GPIO_PDERT2            (*(WoReg  *)0x400E148CU) /**< \brief (GPIO) Pull-down Enable Register - Toggle 2 */
#define REG_GPIO_IER2              (*(RwReg  *)0x400E1490U) /**< \brief (GPIO) Interrupt Enable Register 2 */
#define REG_GPIO_IERS2             (*(WoReg  *)0x400E1494U) /**< \brief (GPIO) Interrupt Enable Register - Set 2 */
#define REG_GPIO_IERC2             (*(WoReg  *)0x400E1498U) /**< \brief (GPIO) Interrupt Enable Register - Clear 2 */
#define REG_GPIO_IERT2             (*(WoReg  *)0x400E149CU) /**< \brief (GPIO) Interrupt Enable Register - Toggle 2 */
#define REG_GPIO_IMR02             (*(RwReg  *)0x400E14A0U) /**< \brief (GPIO) Interrupt Mode Register 0 2 */
#define REG_GPIO_IMR0S2            (*(WoReg  *)0x400E14A4U) /**< \brief (GPIO) Interrupt Mode Register 0 - Set 2 */
#define REG_GPIO_IMR0C2            (*(WoReg  *)0x400E14A8U) /**< \brief (GPIO) Interrupt Mode Register 0 - Clear 2 */
#define REG_GPIO_IMR0T2            (*(WoReg  *)0x400E14ACU) /**< \brief (GPIO) Interrupt Mode Register 0 - Toggle 2 */
#define REG_GPIO_IMR12             (*(RwReg  *)0x400E14B0U) /**< \brief (GPIO) Interrupt Mode Register 1 2 */
#define REG_GPIO_IMR1S2            (*(WoReg  *)0x400E14B4U) /**< \brief (GPIO) Interrupt Mode Register 1 - Set 2 */
#define REG_GPIO_IMR1C2            (*(WoReg  *)0x400E14B8U) /**< \brief (GPIO) Interrupt Mode Register 1 - Clear 2 */
#define REG_GPIO_IMR1T2            (*(WoReg  *)0x400E14BCU) /**< \brief (GPIO) Interrupt Mode Register 1 - Toggle 2 */
#define REG_GPIO_GFER2             (*(RwReg  *)0x400E14C0U) /**< \brief (GPIO) Glitch Filter Enable Register 2 */
#define REG_GPIO_GFERS2            (*(WoReg  *)0x400E14C4U) /**< \brief (GPIO) Glitch Filter Enable Register - Set 2 */
#define REG_GPIO_GFERC2            (*(WoReg  *)0x400E14C8U) /**< \brief (GPIO) Glitch Filter Enable Register - Clear 2 */
#define REG_GPIO_GFERT2            (*(WoReg  *)0x400E14CCU) /**< \brief (GPIO) Glitch Filter Enable Register - Toggle 2 */
#define REG_GPIO_IFR2              (*(RoReg  *)0x400E14D0U) /**< \brief (GPIO) Interrupt Flag Register 2 */
#define REG_GPIO_IFRC2             (*(WoReg  *)0x400E14D8U) /**< \brief (GPIO) Interrupt Flag Register - Clear 2 */
#define REG_GPIO_ODMER2            (*(RwReg  *)0x400E14E0U) /**< \brief (GPIO) Open Drain Mode Register 2 */
#define REG_GPIO_ODMERS2           (*(WoReg  *)0x400E14E4U) /**< \brief (GPIO) Open Drain Mode Register - Set 2 */
#define REG_GPIO_ODMERC2           (*(WoReg  *)0x400E14E8U) /**< \brief (GPIO) Open Drain Mode Register - Clear 2 */
#define REG_GPIO_ODMERT2           (*(WoReg  *)0x400E14ECU) /**< \brief (GPIO) Open Drain Mode Register - Toggle 2 */
#define REG_GPIO_ODCR02            (*(RwReg  *)0x400E1500U) /**< \brief (GPIO) Output Driving Capability Register 0 2 */
#define REG_GPIO_ODCR0S2           (*(RwReg  *)0x400E1504U) /**< \brief (GPIO) Output Driving Capability Register 0 - Set 2 */
#define REG_GPIO_ODCR0C2           (*(RwReg  *)0x400E1508U) /**< \brief (GPIO) Output Driving Capability Register 0 - Clear 2 */
#define REG_GPIO_ODCR0T2           (*(RwReg  *)0x400E150CU) /**< \brief (GPIO) Output Driving Capability Register 0 - Toggle 2 */
#define REG_GPIO_ODCR12            (*(RwReg  *)0x400E1510U) /**< \brief (GPIO) Output Driving Capability Register 1 2 */
#define REG_GPIO_ODCR1S2           (*(RwReg  *)0x400E1514U) /**< \brief (GPIO) Output Driving Capability Register 1 - Set 2 */
#define REG_GPIO_ODCR1C2           (*(RwReg  *)0x400E1518U) /**< \brief (GPIO) Output Driving Capability Register 1 - Clear 2 */
#define REG_GPIO_ODCR1T2           (*(RwReg  *)0x400E151CU) /**< \brief (GPIO) Output Driving Capability Register 1 - Toggle 2 */
#define REG_GPIO_OSRR02            (*(RwReg  *)0x400E1530U) /**< \brief (GPIO) Output Slew Rate Register 0 2 */
#define REG_GPIO_OSRR0S2           (*(RwReg  *)0x400E1534U) /**< \brief (GPIO) Output Slew Rate Register 0 - Set 2 */
#define REG_GPIO_OSRR0C2           (*(RwReg  *)0x400E1538U) /**< \brief (GPIO) Output Slew Rate Register 0 - Clear 2 */
#define REG_GPIO_OSRR0T2           (*(RwReg  *)0x400E153CU) /**< \brief (GPIO) Output Slew Rate Register 0 - Toggle 2 */
#define REG_GPIO_STER2             (*(RwReg  *)0x400E1560U) /**< \brief (GPIO) Schmitt Trigger Enable Register 2 */
#define REG_GPIO_STERS2            (*(RwReg  *)0x400E1564U) /**< \brief (GPIO) Schmitt Trigger Enable Register - Set 2 */
#define REG_GPIO_STERC2            (*(RwReg  *)0x400E1568U) /**< \brief (GPIO) Schmitt Trigger Enable Register - Clear 2 */
#define REG_GPIO_STERT2            (*(RwReg  *)0x400E156CU) /**< \brief (GPIO) Schmitt Trigger Enable Register - Toggle 2 */
#define REG_GPIO_EVER2             (*(RwReg  *)0x400E1580U) /**< \brief (GPIO) Event Enable Register 2 */
#define REG_GPIO_EVERS2            (*(WoReg  *)0x400E1584U) /**< \brief (GPIO) Event Enable Register - Set 2 */
#define REG_GPIO_EVERC2            (*(WoReg  *)0x400E1588U) /**< \brief (GPIO) Event Enable Register - Clear 2 */
#define REG_GPIO_EVERT2            (*(WoReg  *)0x400E158CU) /**< \brief (GPIO) Event Enable Register - Toggle 2 */
#define REG_GPIO_LOCK2             (*(RwReg  *)0x400E15A0U) /**< \brief (GPIO) Lock Register 2 */
#define REG_GPIO_LOCKS2            (*(WoReg  *)0x400E15A4U) /**< \brief (GPIO) Lock Register - Set 2 */
#define REG_GPIO_LOCKC2            (*(WoReg  *)0x400E15A8U) /**< \brief (GPIO) Lock Register - Clear 2 */
#define REG_GPIO_LOCKT2            (*(WoReg  *)0x400E15ACU) /**< \brief (GPIO) Lock Register - Toggle 2 */
#define REG_GPIO_UNLOCK2           (*(WoReg  *)0x400E15E0U) /**< \brief (GPIO) Unlock Register 2 */
#define REG_GPIO_ASR2              (*(RwReg  *)0x400E15E4U) /**< \brief (GPIO) Access Status Register 2 */
#define REG_GPIO_PARAMETER2        (*(RoReg  *)0x400E15F8U) /**< \brief (GPIO) Parameter Register 2 */
#define REG_GPIO_VERSION2          (*(RoReg  *)0x400E15FCU) /**< \brief (GPIO) Version Register 2 */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/* ========== Instance parameters for GPIO peripheral ========== */
#define GPIO_GFER_DEFAULT_VAL       { 0x00000000, 0x00000000, 0x00000000 }
#define GPIO_GFER_IMPLEMENTED       { 0x00000000, 0x00000000, 0x00000000 }
#define GPIO_GFILTER_IMPLEMENTED    { 0x00000000, 0x00000000, 0x00000000 }
#define GPIO_GPER_DEFAULT_VAL       { 0xFFFFFFFF, 0x0000FFFF, 0xFFFFFFFF }
#define GPIO_GPER_IMPLEMENTED       { 0xFFFFFFFC, 0x0000FFFF, 0xFFFFFFFF }
#define GPIO_GPIO_IRQ_MSB           11
#define GPIO_GPIO_MAX_IRQ_MSB       11
#define GPIO_GPIO_PADDR_BITS        11
#define GPIO_GPIO_PINS_MSB          95
#define GPIO_IER_DEFAULT_VAL        { 0x00000000, 0x00000000, 0x00000000 }
#define GPIO_IER_IMPLEMENTED        { 0xFFFFFFFF, 0x0000FFFF, 0xFFFFFFFF }
#define GPIO_IFE_IMPLEMENTED        { 0xFFFFFFFF, 0x0000FFFF, 0xFFFFFFFF }
#define GPIO_IFR_IMPLEMENTED        { 0xFFFFFFFF, 0x0000FFFF, 0xFFFFFFFF }
#define GPIO_IMR0_DEFAULT_VAL       { 0x00000000, 0x00000000, 0x00000000 }
#define GPIO_IMR0_IMPLEMENTED       { 0xFFFFFFFF, 0x0000FFFF, 0xFFFFFFFF }
#define GPIO_IMR1_DEFAULT_VAL       { 0x00000000, 0x00000000, 0x00000000 }
#define GPIO_IMR1_IMPLEMENTED       { 0xFFFFFFFF, 0x0000FFFF, 0xFFFFFFFF }
#define GPIO_INPUT_SYNC_IMPLEMENTED { 0xFFFFFFFF, 0x0000FFFF, 0xFFFFFFFF }
#define GPIO_IRQS_PER_GROUP         8
#define GPIO_LOCK_DEFAULT_VAL       { 0x00000000, 0x00000000, 0x00000000 }
#define GPIO_LOCK_IMPLEMENTED       { 0x00000000, 0x00000000, 0x00000000 }
#define GPIO_NUMBER_OF_PINS         96
#define GPIO_ODCR0_DEFAULT_VAL      { 0x00000000, 0x00000000, 0x00000000 }
#define GPIO_ODCR0_IMPLEMENTED      { 0xFFFFFFFF, 0x0000FFFF, 0xFFFFFFFF }
#define GPIO_ODCR1_DEFAULT_VAL      { 0x00000000, 0x00000000, 0x00000000 }
#define GPIO_ODCR1_IMPLEMENTED      { 0x00000000, 0x00000000, 0x00000000 }
#define GPIO_ODER_DEFAULT_VAL       { 0x00000000, 0x00000000, 0x00000000 }
#define GPIO_ODER_IMPLEMENTED       { 0xFFFFFFFF, 0x0000FFFF, 0xFFFFFFFF }
#define GPIO_ODMER_DEFAULT_VAL      { 0x00000000, 0x00000000, 0x00000000 }
#define GPIO_ODMER_IMPLEMENTED      { 0x00000000, 0x00000000, 0x00000000 }
#define GPIO_OSRR0_DEFAULT_VAL      { 0xFFFFFFFF, 0x0000FFFF, 0xFFFFFFFF }
#define GPIO_OSRR0_IMPLEMENTED      { 0xFFFFFFFF, 0x0000FFFF, 0xFFFFFFFF }
#define GPIO_OVR_DEFAULT_VAL        { 0x00000000, 0x00000000, 0x00000000 }
#define GPIO_OVR_IMPLEMENTED        { 0xFFFFFFFF, 0x0000FFFF, 0xFFFFFFFF }
#define GPIO_PADDR_MSB              10
#define GPIO_PDATA_MSB              31
#define GPIO_PDER_DEFAULT_VAL       { 0x00000000, 0x00000000, 0x00000000 }
#define GPIO_PDER_IMPLEMENTED       { 0xFFFFFFFF, 0x0000FFFF, 0xFFFFFFFF }
#define GPIO_PMR0_DEFAULT_VAL       { 0x00000008, 0x00000000, 0x00000000 }
#define GPIO_PMR0_IMPLEMENTED       { 0xFFFFFFF4, 0x0000FFFF, 0xFFFFFFFF }
#define GPIO_PMR1_DEFAULT_VAL       { 0x00000000, 0x00000000, 0x00000000 }
#define GPIO_PMR1_IMPLEMENTED       { 0xFFFFFFF4, 0x0000FFFF, 0xFFFFFFFF }
#define GPIO_PMR2_DEFAULT_VAL       { 0x00000000, 0x00000000, 0x00000000 }
#define GPIO_PMR2_IMPLEMENTED       { 0xFFFFFFF4, 0x0000FFFF, 0xFFFFFFFF }
#define GPIO_PORT_LENGTH            3
#define GPIO_PUER_DEFAULT_VAL       { 0x00000000, 0x00000000, 0x00000000 }
#define GPIO_PUER_IMPLEMENTED       { 0xFFFFFFFF, 0x0000FFFF, 0xFFFFFFFF }
#define GPIO_STER_DEFAULT_VAL       { 0x00000004, 0x00000000, 0x00000000 }
#define GPIO_STER_IMPLEMENTED       { 0xFFFFFFFF, 0x0000FFFF, 0xFFFFFFFF }

#endif /* _SAM4L_GPIO_INSTANCE_ */
