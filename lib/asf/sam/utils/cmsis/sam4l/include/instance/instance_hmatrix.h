/**
 * \file
 *
 * \brief Instance description for HMATRIX
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

#ifndef _SAM4L_HMATRIX_INSTANCE_
#define _SAM4L_HMATRIX_INSTANCE_

/* ========== Register definition for HMATRIX peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_HMATRIX_MCFG0          (0x400A1000U) /**< \brief (HMATRIX) Master Configuration Register 0 */
#define REG_HMATRIX_MCFG1          (0x400A1004U) /**< \brief (HMATRIX) Master Configuration Register 1 */
#define REG_HMATRIX_MCFG2          (0x400A1008U) /**< \brief (HMATRIX) Master Configuration Register 2 */
#define REG_HMATRIX_MCFG3          (0x400A100CU) /**< \brief (HMATRIX) Master Configuration Register 3 */
#define REG_HMATRIX_MCFG4          (0x400A1010U) /**< \brief (HMATRIX) Master Configuration Register 4 */
#define REG_HMATRIX_MCFG5          (0x400A1014U) /**< \brief (HMATRIX) Master Configuration Register 5 */
#define REG_HMATRIX_MCFG6          (0x400A1018U) /**< \brief (HMATRIX) Master Configuration Register 6 */
#define REG_HMATRIX_MCFG7          (0x400A101CU) /**< \brief (HMATRIX) Master Configuration Register 7 */
#define REG_HMATRIX_MCFG8          (0x400A1020U) /**< \brief (HMATRIX) Master Configuration Register 8 */
#define REG_HMATRIX_MCFG9          (0x400A1024U) /**< \brief (HMATRIX) Master Configuration Register 9 */
#define REG_HMATRIX_MCFG10         (0x400A1028U) /**< \brief (HMATRIX) Master Configuration Register 10 */
#define REG_HMATRIX_MCFG11         (0x400A102CU) /**< \brief (HMATRIX) Master Configuration Register 11 */
#define REG_HMATRIX_MCFG12         (0x400A1030U) /**< \brief (HMATRIX) Master Configuration Register 12 */
#define REG_HMATRIX_MCFG13         (0x400A1034U) /**< \brief (HMATRIX) Master Configuration Register 13 */
#define REG_HMATRIX_MCFG14         (0x400A1038U) /**< \brief (HMATRIX) Master Configuration Register 14 */
#define REG_HMATRIX_MCFG15         (0x400A103CU) /**< \brief (HMATRIX) Master Configuration Register 15 */
#define REG_HMATRIX_SCFG0          (0x400A1040U) /**< \brief (HMATRIX) Slave Configuration Register 0 */
#define REG_HMATRIX_SCFG1          (0x400A1044U) /**< \brief (HMATRIX) Slave Configuration Register 1 */
#define REG_HMATRIX_SCFG2          (0x400A1048U) /**< \brief (HMATRIX) Slave Configuration Register 2 */
#define REG_HMATRIX_SCFG3          (0x400A104CU) /**< \brief (HMATRIX) Slave Configuration Register 3 */
#define REG_HMATRIX_SCFG4          (0x400A1050U) /**< \brief (HMATRIX) Slave Configuration Register 4 */
#define REG_HMATRIX_SCFG5          (0x400A1054U) /**< \brief (HMATRIX) Slave Configuration Register 5 */
#define REG_HMATRIX_SCFG6          (0x400A1058U) /**< \brief (HMATRIX) Slave Configuration Register 6 */
#define REG_HMATRIX_SCFG7          (0x400A105CU) /**< \brief (HMATRIX) Slave Configuration Register 7 */
#define REG_HMATRIX_SCFG8          (0x400A1060U) /**< \brief (HMATRIX) Slave Configuration Register 8 */
#define REG_HMATRIX_SCFG9          (0x400A1064U) /**< \brief (HMATRIX) Slave Configuration Register 9 */
#define REG_HMATRIX_SCFG10         (0x400A1068U) /**< \brief (HMATRIX) Slave Configuration Register 10 */
#define REG_HMATRIX_SCFG11         (0x400A106CU) /**< \brief (HMATRIX) Slave Configuration Register 11 */
#define REG_HMATRIX_SCFG12         (0x400A1070U) /**< \brief (HMATRIX) Slave Configuration Register 12 */
#define REG_HMATRIX_SCFG13         (0x400A1074U) /**< \brief (HMATRIX) Slave Configuration Register 13 */
#define REG_HMATRIX_SCFG14         (0x400A1078U) /**< \brief (HMATRIX) Slave Configuration Register 14 */
#define REG_HMATRIX_SCFG15         (0x400A107CU) /**< \brief (HMATRIX) Slave Configuration Register 15 */
#define REG_HMATRIX_PRAS0          (0x400A1080U) /**< \brief (HMATRIX) Priority Register A for Slave 0 */
#define REG_HMATRIX_PRBS0          (0x400A1084U) /**< \brief (HMATRIX) Priority Register B for Slave 0 */
#define REG_HMATRIX_PRAS1          (0x400A1088U) /**< \brief (HMATRIX) Priority Register A for Slave 1 */
#define REG_HMATRIX_PRBS1          (0x400A108CU) /**< \brief (HMATRIX) Priority Register B for Slave 1 */
#define REG_HMATRIX_PRAS2          (0x400A1090U) /**< \brief (HMATRIX) Priority Register A for Slave 2 */
#define REG_HMATRIX_PRBS2          (0x400A1094U) /**< \brief (HMATRIX) Priority Register B for Slave 2 */
#define REG_HMATRIX_PRAS3          (0x400A1098U) /**< \brief (HMATRIX) Priority Register A for Slave 3 */
#define REG_HMATRIX_PRBS3          (0x400A109CU) /**< \brief (HMATRIX) Priority Register B for Slave 3 */
#define REG_HMATRIX_PRAS4          (0x400A10A0U) /**< \brief (HMATRIX) Priority Register A for Slave 4 */
#define REG_HMATRIX_PRBS4          (0x400A10A4U) /**< \brief (HMATRIX) Priority Register B for Slave 4 */
#define REG_HMATRIX_PRAS5          (0x400A10A8U) /**< \brief (HMATRIX) Priority Register A for Slave 5 */
#define REG_HMATRIX_PRBS5          (0x400A10ACU) /**< \brief (HMATRIX) Priority Register B for Slave 5 */
#define REG_HMATRIX_PRAS6          (0x400A10B0U) /**< \brief (HMATRIX) Priority Register A for Slave 6 */
#define REG_HMATRIX_PRBS6          (0x400A10B4U) /**< \brief (HMATRIX) Priority Register B for Slave 6 */
#define REG_HMATRIX_PRAS7          (0x400A10B8U) /**< \brief (HMATRIX) Priority Register A for Slave 7 */
#define REG_HMATRIX_PRBS7          (0x400A10BCU) /**< \brief (HMATRIX) Priority Register B for Slave 7 */
#define REG_HMATRIX_PRAS8          (0x400A10C0U) /**< \brief (HMATRIX) Priority Register A for Slave 8 */
#define REG_HMATRIX_PRBS8          (0x400A10C4U) /**< \brief (HMATRIX) Priority Register B for Slave 8 */
#define REG_HMATRIX_PRAS9          (0x400A10C8U) /**< \brief (HMATRIX) Priority Register A for Slave 9 */
#define REG_HMATRIX_PRBS9          (0x400A10CCU) /**< \brief (HMATRIX) Priority Register B for Slave 9 */
#define REG_HMATRIX_PRAS10         (0x400A10D0U) /**< \brief (HMATRIX) Priority Register A for Slave 10 */
#define REG_HMATRIX_PRBS10         (0x400A10D4U) /**< \brief (HMATRIX) Priority Register B for Slave 10 */
#define REG_HMATRIX_PRAS11         (0x400A10D8U) /**< \brief (HMATRIX) Priority Register A for Slave 11 */
#define REG_HMATRIX_PRBS11         (0x400A10DCU) /**< \brief (HMATRIX) Priority Register B for Slave 11 */
#define REG_HMATRIX_PRAS12         (0x400A10E0U) /**< \brief (HMATRIX) Priority Register A for Slave 12 */
#define REG_HMATRIX_PRBS12         (0x400A10E4U) /**< \brief (HMATRIX) Priority Register B for Slave 12 */
#define REG_HMATRIX_PRAS13         (0x400A10E8U) /**< \brief (HMATRIX) Priority Register A for Slave 13 */
#define REG_HMATRIX_PRBS13         (0x400A10ECU) /**< \brief (HMATRIX) Priority Register B for Slave 13 */
#define REG_HMATRIX_PRAS14         (0x400A10F0U) /**< \brief (HMATRIX) Priority Register A for Slave 14 */
#define REG_HMATRIX_PRBS14         (0x400A10F4U) /**< \brief (HMATRIX) Priority Register B for Slave 14 */
#define REG_HMATRIX_PRAS15         (0x400A10F8U) /**< \brief (HMATRIX) Priority Register A for Slave 15 */
#define REG_HMATRIX_PRBS15         (0x400A10FCU) /**< \brief (HMATRIX) Priority Register B for Slave 15 */
#define REG_HMATRIX_MRCR           (0x400A1100U) /**< \brief (HMATRIX) Master Remap Control Register */
#define REG_HMATRIX_SFR0           (0x400A1110U) /**< \brief (HMATRIX) Special Function Register 0 */
#define REG_HMATRIX_SFR1           (0x400A1114U) /**< \brief (HMATRIX) Special Function Register 1 */
#define REG_HMATRIX_SFR2           (0x400A1118U) /**< \brief (HMATRIX) Special Function Register 2 */
#define REG_HMATRIX_SFR3           (0x400A111CU) /**< \brief (HMATRIX) Special Function Register 3 */
#define REG_HMATRIX_SFR4           (0x400A1120U) /**< \brief (HMATRIX) Special Function Register 4 */
#define REG_HMATRIX_SFR5           (0x400A1124U) /**< \brief (HMATRIX) Special Function Register 5 */
#define REG_HMATRIX_SFR6           (0x400A1128U) /**< \brief (HMATRIX) Special Function Register 6 */
#define REG_HMATRIX_SFR7           (0x400A112CU) /**< \brief (HMATRIX) Special Function Register 7 */
#define REG_HMATRIX_SFR8           (0x400A1130U) /**< \brief (HMATRIX) Special Function Register 8 */
#define REG_HMATRIX_SFR9           (0x400A1134U) /**< \brief (HMATRIX) Special Function Register 9 */
#define REG_HMATRIX_SFR10          (0x400A1138U) /**< \brief (HMATRIX) Special Function Register 10 */
#define REG_HMATRIX_SFR11          (0x400A113CU) /**< \brief (HMATRIX) Special Function Register 11 */
#define REG_HMATRIX_SFR12          (0x400A1140U) /**< \brief (HMATRIX) Special Function Register 12 */
#define REG_HMATRIX_SFR13          (0x400A1144U) /**< \brief (HMATRIX) Special Function Register 13 */
#define REG_HMATRIX_SFR14          (0x400A1148U) /**< \brief (HMATRIX) Special Function Register 14 */
#define REG_HMATRIX_SFR15          (0x400A114CU) /**< \brief (HMATRIX) Special Function Register 15 */
#else
#define REG_HMATRIX_MCFG0          (*(RwReg  *)0x400A1000U) /**< \brief (HMATRIX) Master Configuration Register 0 */
#define REG_HMATRIX_MCFG1          (*(RwReg  *)0x400A1004U) /**< \brief (HMATRIX) Master Configuration Register 1 */
#define REG_HMATRIX_MCFG2          (*(RwReg  *)0x400A1008U) /**< \brief (HMATRIX) Master Configuration Register 2 */
#define REG_HMATRIX_MCFG3          (*(RwReg  *)0x400A100CU) /**< \brief (HMATRIX) Master Configuration Register 3 */
#define REG_HMATRIX_MCFG4          (*(RwReg  *)0x400A1010U) /**< \brief (HMATRIX) Master Configuration Register 4 */
#define REG_HMATRIX_MCFG5          (*(RwReg  *)0x400A1014U) /**< \brief (HMATRIX) Master Configuration Register 5 */
#define REG_HMATRIX_MCFG6          (*(RwReg  *)0x400A1018U) /**< \brief (HMATRIX) Master Configuration Register 6 */
#define REG_HMATRIX_MCFG7          (*(RwReg  *)0x400A101CU) /**< \brief (HMATRIX) Master Configuration Register 7 */
#define REG_HMATRIX_MCFG8          (*(RwReg  *)0x400A1020U) /**< \brief (HMATRIX) Master Configuration Register 8 */
#define REG_HMATRIX_MCFG9          (*(RwReg  *)0x400A1024U) /**< \brief (HMATRIX) Master Configuration Register 9 */
#define REG_HMATRIX_MCFG10         (*(RwReg  *)0x400A1028U) /**< \brief (HMATRIX) Master Configuration Register 10 */
#define REG_HMATRIX_MCFG11         (*(RwReg  *)0x400A102CU) /**< \brief (HMATRIX) Master Configuration Register 11 */
#define REG_HMATRIX_MCFG12         (*(RwReg  *)0x400A1030U) /**< \brief (HMATRIX) Master Configuration Register 12 */
#define REG_HMATRIX_MCFG13         (*(RwReg  *)0x400A1034U) /**< \brief (HMATRIX) Master Configuration Register 13 */
#define REG_HMATRIX_MCFG14         (*(RwReg  *)0x400A1038U) /**< \brief (HMATRIX) Master Configuration Register 14 */
#define REG_HMATRIX_MCFG15         (*(RwReg  *)0x400A103CU) /**< \brief (HMATRIX) Master Configuration Register 15 */
#define REG_HMATRIX_SCFG0          (*(RwReg  *)0x400A1040U) /**< \brief (HMATRIX) Slave Configuration Register 0 */
#define REG_HMATRIX_SCFG1          (*(RwReg  *)0x400A1044U) /**< \brief (HMATRIX) Slave Configuration Register 1 */
#define REG_HMATRIX_SCFG2          (*(RwReg  *)0x400A1048U) /**< \brief (HMATRIX) Slave Configuration Register 2 */
#define REG_HMATRIX_SCFG3          (*(RwReg  *)0x400A104CU) /**< \brief (HMATRIX) Slave Configuration Register 3 */
#define REG_HMATRIX_SCFG4          (*(RwReg  *)0x400A1050U) /**< \brief (HMATRIX) Slave Configuration Register 4 */
#define REG_HMATRIX_SCFG5          (*(RwReg  *)0x400A1054U) /**< \brief (HMATRIX) Slave Configuration Register 5 */
#define REG_HMATRIX_SCFG6          (*(RwReg  *)0x400A1058U) /**< \brief (HMATRIX) Slave Configuration Register 6 */
#define REG_HMATRIX_SCFG7          (*(RwReg  *)0x400A105CU) /**< \brief (HMATRIX) Slave Configuration Register 7 */
#define REG_HMATRIX_SCFG8          (*(RwReg  *)0x400A1060U) /**< \brief (HMATRIX) Slave Configuration Register 8 */
#define REG_HMATRIX_SCFG9          (*(RwReg  *)0x400A1064U) /**< \brief (HMATRIX) Slave Configuration Register 9 */
#define REG_HMATRIX_SCFG10         (*(RwReg  *)0x400A1068U) /**< \brief (HMATRIX) Slave Configuration Register 10 */
#define REG_HMATRIX_SCFG11         (*(RwReg  *)0x400A106CU) /**< \brief (HMATRIX) Slave Configuration Register 11 */
#define REG_HMATRIX_SCFG12         (*(RwReg  *)0x400A1070U) /**< \brief (HMATRIX) Slave Configuration Register 12 */
#define REG_HMATRIX_SCFG13         (*(RwReg  *)0x400A1074U) /**< \brief (HMATRIX) Slave Configuration Register 13 */
#define REG_HMATRIX_SCFG14         (*(RwReg  *)0x400A1078U) /**< \brief (HMATRIX) Slave Configuration Register 14 */
#define REG_HMATRIX_SCFG15         (*(RwReg  *)0x400A107CU) /**< \brief (HMATRIX) Slave Configuration Register 15 */
#define REG_HMATRIX_PRAS0          (*(RwReg  *)0x400A1080U) /**< \brief (HMATRIX) Priority Register A for Slave 0 */
#define REG_HMATRIX_PRBS0          (*(RwReg  *)0x400A1084U) /**< \brief (HMATRIX) Priority Register B for Slave 0 */
#define REG_HMATRIX_PRAS1          (*(RwReg  *)0x400A1088U) /**< \brief (HMATRIX) Priority Register A for Slave 1 */
#define REG_HMATRIX_PRBS1          (*(RwReg  *)0x400A108CU) /**< \brief (HMATRIX) Priority Register B for Slave 1 */
#define REG_HMATRIX_PRAS2          (*(RwReg  *)0x400A1090U) /**< \brief (HMATRIX) Priority Register A for Slave 2 */
#define REG_HMATRIX_PRBS2          (*(RwReg  *)0x400A1094U) /**< \brief (HMATRIX) Priority Register B for Slave 2 */
#define REG_HMATRIX_PRAS3          (*(RwReg  *)0x400A1098U) /**< \brief (HMATRIX) Priority Register A for Slave 3 */
#define REG_HMATRIX_PRBS3          (*(RwReg  *)0x400A109CU) /**< \brief (HMATRIX) Priority Register B for Slave 3 */
#define REG_HMATRIX_PRAS4          (*(RwReg  *)0x400A10A0U) /**< \brief (HMATRIX) Priority Register A for Slave 4 */
#define REG_HMATRIX_PRBS4          (*(RwReg  *)0x400A10A4U) /**< \brief (HMATRIX) Priority Register B for Slave 4 */
#define REG_HMATRIX_PRAS5          (*(RwReg  *)0x400A10A8U) /**< \brief (HMATRIX) Priority Register A for Slave 5 */
#define REG_HMATRIX_PRBS5          (*(RwReg  *)0x400A10ACU) /**< \brief (HMATRIX) Priority Register B for Slave 5 */
#define REG_HMATRIX_PRAS6          (*(RwReg  *)0x400A10B0U) /**< \brief (HMATRIX) Priority Register A for Slave 6 */
#define REG_HMATRIX_PRBS6          (*(RwReg  *)0x400A10B4U) /**< \brief (HMATRIX) Priority Register B for Slave 6 */
#define REG_HMATRIX_PRAS7          (*(RwReg  *)0x400A10B8U) /**< \brief (HMATRIX) Priority Register A for Slave 7 */
#define REG_HMATRIX_PRBS7          (*(RwReg  *)0x400A10BCU) /**< \brief (HMATRIX) Priority Register B for Slave 7 */
#define REG_HMATRIX_PRAS8          (*(RwReg  *)0x400A10C0U) /**< \brief (HMATRIX) Priority Register A for Slave 8 */
#define REG_HMATRIX_PRBS8          (*(RwReg  *)0x400A10C4U) /**< \brief (HMATRIX) Priority Register B for Slave 8 */
#define REG_HMATRIX_PRAS9          (*(RwReg  *)0x400A10C8U) /**< \brief (HMATRIX) Priority Register A for Slave 9 */
#define REG_HMATRIX_PRBS9          (*(RwReg  *)0x400A10CCU) /**< \brief (HMATRIX) Priority Register B for Slave 9 */
#define REG_HMATRIX_PRAS10         (*(RwReg  *)0x400A10D0U) /**< \brief (HMATRIX) Priority Register A for Slave 10 */
#define REG_HMATRIX_PRBS10         (*(RwReg  *)0x400A10D4U) /**< \brief (HMATRIX) Priority Register B for Slave 10 */
#define REG_HMATRIX_PRAS11         (*(RwReg  *)0x400A10D8U) /**< \brief (HMATRIX) Priority Register A for Slave 11 */
#define REG_HMATRIX_PRBS11         (*(RwReg  *)0x400A10DCU) /**< \brief (HMATRIX) Priority Register B for Slave 11 */
#define REG_HMATRIX_PRAS12         (*(RwReg  *)0x400A10E0U) /**< \brief (HMATRIX) Priority Register A for Slave 12 */
#define REG_HMATRIX_PRBS12         (*(RwReg  *)0x400A10E4U) /**< \brief (HMATRIX) Priority Register B for Slave 12 */
#define REG_HMATRIX_PRAS13         (*(RwReg  *)0x400A10E8U) /**< \brief (HMATRIX) Priority Register A for Slave 13 */
#define REG_HMATRIX_PRBS13         (*(RwReg  *)0x400A10ECU) /**< \brief (HMATRIX) Priority Register B for Slave 13 */
#define REG_HMATRIX_PRAS14         (*(RwReg  *)0x400A10F0U) /**< \brief (HMATRIX) Priority Register A for Slave 14 */
#define REG_HMATRIX_PRBS14         (*(RwReg  *)0x400A10F4U) /**< \brief (HMATRIX) Priority Register B for Slave 14 */
#define REG_HMATRIX_PRAS15         (*(RwReg  *)0x400A10F8U) /**< \brief (HMATRIX) Priority Register A for Slave 15 */
#define REG_HMATRIX_PRBS15         (*(RwReg  *)0x400A10FCU) /**< \brief (HMATRIX) Priority Register B for Slave 15 */
#define REG_HMATRIX_MRCR           (*(RwReg  *)0x400A1100U) /**< \brief (HMATRIX) Master Remap Control Register */
#define REG_HMATRIX_SFR0           (*(RwReg  *)0x400A1110U) /**< \brief (HMATRIX) Special Function Register 0 */
#define REG_HMATRIX_SFR1           (*(RwReg  *)0x400A1114U) /**< \brief (HMATRIX) Special Function Register 1 */
#define REG_HMATRIX_SFR2           (*(RwReg  *)0x400A1118U) /**< \brief (HMATRIX) Special Function Register 2 */
#define REG_HMATRIX_SFR3           (*(RwReg  *)0x400A111CU) /**< \brief (HMATRIX) Special Function Register 3 */
#define REG_HMATRIX_SFR4           (*(RwReg  *)0x400A1120U) /**< \brief (HMATRIX) Special Function Register 4 */
#define REG_HMATRIX_SFR5           (*(RwReg  *)0x400A1124U) /**< \brief (HMATRIX) Special Function Register 5 */
#define REG_HMATRIX_SFR6           (*(RwReg  *)0x400A1128U) /**< \brief (HMATRIX) Special Function Register 6 */
#define REG_HMATRIX_SFR7           (*(RwReg  *)0x400A112CU) /**< \brief (HMATRIX) Special Function Register 7 */
#define REG_HMATRIX_SFR8           (*(RwReg  *)0x400A1130U) /**< \brief (HMATRIX) Special Function Register 8 */
#define REG_HMATRIX_SFR9           (*(RwReg  *)0x400A1134U) /**< \brief (HMATRIX) Special Function Register 9 */
#define REG_HMATRIX_SFR10          (*(RwReg  *)0x400A1138U) /**< \brief (HMATRIX) Special Function Register 10 */
#define REG_HMATRIX_SFR11          (*(RwReg  *)0x400A113CU) /**< \brief (HMATRIX) Special Function Register 11 */
#define REG_HMATRIX_SFR12          (*(RwReg  *)0x400A1140U) /**< \brief (HMATRIX) Special Function Register 12 */
#define REG_HMATRIX_SFR13          (*(RwReg  *)0x400A1144U) /**< \brief (HMATRIX) Special Function Register 13 */
#define REG_HMATRIX_SFR14          (*(RwReg  *)0x400A1148U) /**< \brief (HMATRIX) Special Function Register 14 */
#define REG_HMATRIX_SFR15          (*(RwReg  *)0x400A114CU) /**< \brief (HMATRIX) Special Function Register 15 */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/* ========== Instance parameters for HMATRIX ========== */
#define HMATRIX_SLAVE_FLASH         0
#define HMATRIX_SLAVE_HTOP0         1
#define HMATRIX_SLAVE_HTOP1         2
#define HMATRIX_SLAVE_HTOP2         3
#define HMATRIX_SLAVE_HTOP3         4
#define HMATRIX_SLAVE_HRAMC0        5
#define HMATRIX_SLAVE_HRAMC1        6
#define HMATRIX_SLAVE_AESA          7
#define HMATRIX_SLAVE_NUM           8

#define HMATRIX_MASTER_CPU_IDCODE   0
#define HMATRIX_MASTER_CPU_SYS      1
#define HMATRIX_MASTER_SMAP         2
#define HMATRIX_MASTER_PDCA         3
#define HMATRIX_MASTER_USBC_MASTER  4
#define HMATRIX_MASTER_CRCCU        5
#define HMATRIX_MASTER_NUM          6

#endif /* _SAM4L_HMATRIX_INSTANCE_ */
