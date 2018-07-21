/**
 * \file
 *
 * \brief Instance description for PEVC
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

#ifndef _SAM4L_PEVC_INSTANCE_
#define _SAM4L_PEVC_INSTANCE_

/* ========== Register definition for PEVC peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_PEVC_CHSR              (0x400A6000U) /**< \brief (PEVC) Channel Status Register */
#define REG_PEVC_CHER              (0x400A6004U) /**< \brief (PEVC) Channel Enable Register */
#define REG_PEVC_CHDR              (0x400A6008U) /**< \brief (PEVC) Channel Disable Register */
#define REG_PEVC_SEV               (0x400A6010U) /**< \brief (PEVC) Software Event */
#define REG_PEVC_BUSY              (0x400A6014U) /**< \brief (PEVC) Channel / User Busy */
#define REG_PEVC_TRIER             (0x400A6020U) /**< \brief (PEVC) Trigger Interrupt Mask Enable Register */
#define REG_PEVC_TRIDR             (0x400A6024U) /**< \brief (PEVC) Trigger Interrupt Mask Disable Register */
#define REG_PEVC_TRIMR             (0x400A6028U) /**< \brief (PEVC) Trigger Interrupt Mask Register */
#define REG_PEVC_TRSR              (0x400A6030U) /**< \brief (PEVC) Trigger Status Register */
#define REG_PEVC_TRSCR             (0x400A6034U) /**< \brief (PEVC) Trigger Status Clear Register */
#define REG_PEVC_OVIER             (0x400A6040U) /**< \brief (PEVC) Overrun Interrupt Mask Enable Register */
#define REG_PEVC_OVIDR             (0x400A6044U) /**< \brief (PEVC) Overrun Interrupt Mask Disable Register */
#define REG_PEVC_OVIMR             (0x400A6048U) /**< \brief (PEVC) Overrun Interrupt Mask Register */
#define REG_PEVC_OVSR              (0x400A6050U) /**< \brief (PEVC) Overrun Status Register */
#define REG_PEVC_OVSCR             (0x400A6054U) /**< \brief (PEVC) Overrun Status Clear Register */
#define REG_PEVC_CHMX0             (0x400A6100U) /**< \brief (PEVC) Channel Multiplexer 0 */
#define REG_PEVC_CHMX1             (0x400A6104U) /**< \brief (PEVC) Channel Multiplexer 1 */
#define REG_PEVC_CHMX2             (0x400A6108U) /**< \brief (PEVC) Channel Multiplexer 2 */
#define REG_PEVC_CHMX3             (0x400A610CU) /**< \brief (PEVC) Channel Multiplexer 3 */
#define REG_PEVC_CHMX4             (0x400A6110U) /**< \brief (PEVC) Channel Multiplexer 4 */
#define REG_PEVC_CHMX5             (0x400A6114U) /**< \brief (PEVC) Channel Multiplexer 5 */
#define REG_PEVC_CHMX6             (0x400A6118U) /**< \brief (PEVC) Channel Multiplexer 6 */
#define REG_PEVC_CHMX7             (0x400A611CU) /**< \brief (PEVC) Channel Multiplexer 7 */
#define REG_PEVC_CHMX8             (0x400A6120U) /**< \brief (PEVC) Channel Multiplexer 8 */
#define REG_PEVC_CHMX9             (0x400A6124U) /**< \brief (PEVC) Channel Multiplexer 9 */
#define REG_PEVC_CHMX10            (0x400A6128U) /**< \brief (PEVC) Channel Multiplexer 10 */
#define REG_PEVC_CHMX11            (0x400A612CU) /**< \brief (PEVC) Channel Multiplexer 11 */
#define REG_PEVC_CHMX12            (0x400A6130U) /**< \brief (PEVC) Channel Multiplexer 12 */
#define REG_PEVC_CHMX13            (0x400A6134U) /**< \brief (PEVC) Channel Multiplexer 13 */
#define REG_PEVC_CHMX14            (0x400A6138U) /**< \brief (PEVC) Channel Multiplexer 14 */
#define REG_PEVC_CHMX15            (0x400A613CU) /**< \brief (PEVC) Channel Multiplexer 15 */
#define REG_PEVC_CHMX16            (0x400A6140U) /**< \brief (PEVC) Channel Multiplexer 16 */
#define REG_PEVC_CHMX17            (0x400A6144U) /**< \brief (PEVC) Channel Multiplexer 17 */
#define REG_PEVC_CHMX18            (0x400A6148U) /**< \brief (PEVC) Channel Multiplexer 18 */
#define REG_PEVC_EVS0              (0x400A6200U) /**< \brief (PEVC) Event Shaper 0 */
#define REG_PEVC_EVS1              (0x400A6204U) /**< \brief (PEVC) Event Shaper 1 */
#define REG_PEVC_EVS2              (0x400A6208U) /**< \brief (PEVC) Event Shaper 2 */
#define REG_PEVC_EVS3              (0x400A620CU) /**< \brief (PEVC) Event Shaper 3 */
#define REG_PEVC_EVS4              (0x400A6210U) /**< \brief (PEVC) Event Shaper 4 */
#define REG_PEVC_EVS5              (0x400A6214U) /**< \brief (PEVC) Event Shaper 5 */
#define REG_PEVC_EVS6              (0x400A6218U) /**< \brief (PEVC) Event Shaper 6 */
#define REG_PEVC_EVS7              (0x400A621CU) /**< \brief (PEVC) Event Shaper 7 */
#define REG_PEVC_EVS8              (0x400A6220U) /**< \brief (PEVC) Event Shaper 8 */
#define REG_PEVC_EVS9              (0x400A6224U) /**< \brief (PEVC) Event Shaper 9 */
#define REG_PEVC_EVS10             (0x400A6228U) /**< \brief (PEVC) Event Shaper 10 */
#define REG_PEVC_EVS11             (0x400A622CU) /**< \brief (PEVC) Event Shaper 11 */
#define REG_PEVC_EVS12             (0x400A6230U) /**< \brief (PEVC) Event Shaper 12 */
#define REG_PEVC_EVS13             (0x400A6234U) /**< \brief (PEVC) Event Shaper 13 */
#define REG_PEVC_EVS14             (0x400A6238U) /**< \brief (PEVC) Event Shaper 14 */
#define REG_PEVC_EVS15             (0x400A623CU) /**< \brief (PEVC) Event Shaper 15 */
#define REG_PEVC_EVS16             (0x400A6240U) /**< \brief (PEVC) Event Shaper 16 */
#define REG_PEVC_EVS17             (0x400A6244U) /**< \brief (PEVC) Event Shaper 17 */
#define REG_PEVC_EVS18             (0x400A6248U) /**< \brief (PEVC) Event Shaper 18 */
#define REG_PEVC_EVS19             (0x400A624CU) /**< \brief (PEVC) Event Shaper 19 */
#define REG_PEVC_EVS20             (0x400A6250U) /**< \brief (PEVC) Event Shaper 20 */
#define REG_PEVC_EVS21             (0x400A6254U) /**< \brief (PEVC) Event Shaper 21 */
#define REG_PEVC_EVS22             (0x400A6258U) /**< \brief (PEVC) Event Shaper 22 */
#define REG_PEVC_EVS23             (0x400A625CU) /**< \brief (PEVC) Event Shaper 23 */
#define REG_PEVC_EVS24             (0x400A6260U) /**< \brief (PEVC) Event Shaper 24 */
#define REG_PEVC_EVS25             (0x400A6264U) /**< \brief (PEVC) Event Shaper 25 */
#define REG_PEVC_EVS26             (0x400A6268U) /**< \brief (PEVC) Event Shaper 26 */
#define REG_PEVC_EVS27             (0x400A626CU) /**< \brief (PEVC) Event Shaper 27 */
#define REG_PEVC_EVS28             (0x400A6270U) /**< \brief (PEVC) Event Shaper 28 */
#define REG_PEVC_EVS29             (0x400A6274U) /**< \brief (PEVC) Event Shaper 29 */
#define REG_PEVC_EVS30             (0x400A6278U) /**< \brief (PEVC) Event Shaper 30 */
#define REG_PEVC_IGFDR             (0x400A6300U) /**< \brief (PEVC) Input Glitch Filter Divider Register */
#define REG_PEVC_PARAMETER         (0x400A63F8U) /**< \brief (PEVC) Parameter */
#define REG_PEVC_VERSION           (0x400A63FCU) /**< \brief (PEVC) Version */
#else
#define REG_PEVC_CHSR              (*(RoReg  *)0x400A6000U) /**< \brief (PEVC) Channel Status Register */
#define REG_PEVC_CHER              (*(WoReg  *)0x400A6004U) /**< \brief (PEVC) Channel Enable Register */
#define REG_PEVC_CHDR              (*(WoReg  *)0x400A6008U) /**< \brief (PEVC) Channel Disable Register */
#define REG_PEVC_SEV               (*(WoReg  *)0x400A6010U) /**< \brief (PEVC) Software Event */
#define REG_PEVC_BUSY              (*(RoReg  *)0x400A6014U) /**< \brief (PEVC) Channel / User Busy */
#define REG_PEVC_TRIER             (*(WoReg  *)0x400A6020U) /**< \brief (PEVC) Trigger Interrupt Mask Enable Register */
#define REG_PEVC_TRIDR             (*(WoReg  *)0x400A6024U) /**< \brief (PEVC) Trigger Interrupt Mask Disable Register */
#define REG_PEVC_TRIMR             (*(RoReg  *)0x400A6028U) /**< \brief (PEVC) Trigger Interrupt Mask Register */
#define REG_PEVC_TRSR              (*(RoReg  *)0x400A6030U) /**< \brief (PEVC) Trigger Status Register */
#define REG_PEVC_TRSCR             (*(WoReg  *)0x400A6034U) /**< \brief (PEVC) Trigger Status Clear Register */
#define REG_PEVC_OVIER             (*(WoReg  *)0x400A6040U) /**< \brief (PEVC) Overrun Interrupt Mask Enable Register */
#define REG_PEVC_OVIDR             (*(WoReg  *)0x400A6044U) /**< \brief (PEVC) Overrun Interrupt Mask Disable Register */
#define REG_PEVC_OVIMR             (*(RoReg  *)0x400A6048U) /**< \brief (PEVC) Overrun Interrupt Mask Register */
#define REG_PEVC_OVSR              (*(RoReg  *)0x400A6050U) /**< \brief (PEVC) Overrun Status Register */
#define REG_PEVC_OVSCR             (*(WoReg  *)0x400A6054U) /**< \brief (PEVC) Overrun Status Clear Register */
#define REG_PEVC_CHMX0             (*(RwReg  *)0x400A6100U) /**< \brief (PEVC) Channel Multiplexer 0 */
#define REG_PEVC_CHMX1             (*(RwReg  *)0x400A6104U) /**< \brief (PEVC) Channel Multiplexer 1 */
#define REG_PEVC_CHMX2             (*(RwReg  *)0x400A6108U) /**< \brief (PEVC) Channel Multiplexer 2 */
#define REG_PEVC_CHMX3             (*(RwReg  *)0x400A610CU) /**< \brief (PEVC) Channel Multiplexer 3 */
#define REG_PEVC_CHMX4             (*(RwReg  *)0x400A6110U) /**< \brief (PEVC) Channel Multiplexer 4 */
#define REG_PEVC_CHMX5             (*(RwReg  *)0x400A6114U) /**< \brief (PEVC) Channel Multiplexer 5 */
#define REG_PEVC_CHMX6             (*(RwReg  *)0x400A6118U) /**< \brief (PEVC) Channel Multiplexer 6 */
#define REG_PEVC_CHMX7             (*(RwReg  *)0x400A611CU) /**< \brief (PEVC) Channel Multiplexer 7 */
#define REG_PEVC_CHMX8             (*(RwReg  *)0x400A6120U) /**< \brief (PEVC) Channel Multiplexer 8 */
#define REG_PEVC_CHMX9             (*(RwReg  *)0x400A6124U) /**< \brief (PEVC) Channel Multiplexer 9 */
#define REG_PEVC_CHMX10            (*(RwReg  *)0x400A6128U) /**< \brief (PEVC) Channel Multiplexer 10 */
#define REG_PEVC_CHMX11            (*(RwReg  *)0x400A612CU) /**< \brief (PEVC) Channel Multiplexer 11 */
#define REG_PEVC_CHMX12            (*(RwReg  *)0x400A6130U) /**< \brief (PEVC) Channel Multiplexer 12 */
#define REG_PEVC_CHMX13            (*(RwReg  *)0x400A6134U) /**< \brief (PEVC) Channel Multiplexer 13 */
#define REG_PEVC_CHMX14            (*(RwReg  *)0x400A6138U) /**< \brief (PEVC) Channel Multiplexer 14 */
#define REG_PEVC_CHMX15            (*(RwReg  *)0x400A613CU) /**< \brief (PEVC) Channel Multiplexer 15 */
#define REG_PEVC_CHMX16            (*(RwReg  *)0x400A6140U) /**< \brief (PEVC) Channel Multiplexer 16 */
#define REG_PEVC_CHMX17            (*(RwReg  *)0x400A6144U) /**< \brief (PEVC) Channel Multiplexer 17 */
#define REG_PEVC_CHMX18            (*(RwReg  *)0x400A6148U) /**< \brief (PEVC) Channel Multiplexer 18 */
#define REG_PEVC_EVS0              (*(RwReg  *)0x400A6200U) /**< \brief (PEVC) Event Shaper 0 */
#define REG_PEVC_EVS1              (*(RwReg  *)0x400A6204U) /**< \brief (PEVC) Event Shaper 1 */
#define REG_PEVC_EVS2              (*(RwReg  *)0x400A6208U) /**< \brief (PEVC) Event Shaper 2 */
#define REG_PEVC_EVS3              (*(RwReg  *)0x400A620CU) /**< \brief (PEVC) Event Shaper 3 */
#define REG_PEVC_EVS4              (*(RwReg  *)0x400A6210U) /**< \brief (PEVC) Event Shaper 4 */
#define REG_PEVC_EVS5              (*(RwReg  *)0x400A6214U) /**< \brief (PEVC) Event Shaper 5 */
#define REG_PEVC_EVS6              (*(RwReg  *)0x400A6218U) /**< \brief (PEVC) Event Shaper 6 */
#define REG_PEVC_EVS7              (*(RwReg  *)0x400A621CU) /**< \brief (PEVC) Event Shaper 7 */
#define REG_PEVC_EVS8              (*(RwReg  *)0x400A6220U) /**< \brief (PEVC) Event Shaper 8 */
#define REG_PEVC_EVS9              (*(RwReg  *)0x400A6224U) /**< \brief (PEVC) Event Shaper 9 */
#define REG_PEVC_EVS10             (*(RwReg  *)0x400A6228U) /**< \brief (PEVC) Event Shaper 10 */
#define REG_PEVC_EVS11             (*(RwReg  *)0x400A622CU) /**< \brief (PEVC) Event Shaper 11 */
#define REG_PEVC_EVS12             (*(RwReg  *)0x400A6230U) /**< \brief (PEVC) Event Shaper 12 */
#define REG_PEVC_EVS13             (*(RwReg  *)0x400A6234U) /**< \brief (PEVC) Event Shaper 13 */
#define REG_PEVC_EVS14             (*(RwReg  *)0x400A6238U) /**< \brief (PEVC) Event Shaper 14 */
#define REG_PEVC_EVS15             (*(RwReg  *)0x400A623CU) /**< \brief (PEVC) Event Shaper 15 */
#define REG_PEVC_EVS16             (*(RwReg  *)0x400A6240U) /**< \brief (PEVC) Event Shaper 16 */
#define REG_PEVC_EVS17             (*(RwReg  *)0x400A6244U) /**< \brief (PEVC) Event Shaper 17 */
#define REG_PEVC_EVS18             (*(RwReg  *)0x400A6248U) /**< \brief (PEVC) Event Shaper 18 */
#define REG_PEVC_EVS19             (*(RwReg  *)0x400A624CU) /**< \brief (PEVC) Event Shaper 19 */
#define REG_PEVC_EVS20             (*(RwReg  *)0x400A6250U) /**< \brief (PEVC) Event Shaper 20 */
#define REG_PEVC_EVS21             (*(RwReg  *)0x400A6254U) /**< \brief (PEVC) Event Shaper 21 */
#define REG_PEVC_EVS22             (*(RwReg  *)0x400A6258U) /**< \brief (PEVC) Event Shaper 22 */
#define REG_PEVC_EVS23             (*(RwReg  *)0x400A625CU) /**< \brief (PEVC) Event Shaper 23 */
#define REG_PEVC_EVS24             (*(RwReg  *)0x400A6260U) /**< \brief (PEVC) Event Shaper 24 */
#define REG_PEVC_EVS25             (*(RwReg  *)0x400A6264U) /**< \brief (PEVC) Event Shaper 25 */
#define REG_PEVC_EVS26             (*(RwReg  *)0x400A6268U) /**< \brief (PEVC) Event Shaper 26 */
#define REG_PEVC_EVS27             (*(RwReg  *)0x400A626CU) /**< \brief (PEVC) Event Shaper 27 */
#define REG_PEVC_EVS28             (*(RwReg  *)0x400A6270U) /**< \brief (PEVC) Event Shaper 28 */
#define REG_PEVC_EVS29             (*(RwReg  *)0x400A6274U) /**< \brief (PEVC) Event Shaper 29 */
#define REG_PEVC_EVS30             (*(RwReg  *)0x400A6278U) /**< \brief (PEVC) Event Shaper 30 */
#define REG_PEVC_IGFDR             (*(RwReg  *)0x400A6300U) /**< \brief (PEVC) Input Glitch Filter Divider Register */
#define REG_PEVC_PARAMETER         (*(RoReg  *)0x400A63F8U) /**< \brief (PEVC) Parameter */
#define REG_PEVC_VERSION           (*(RoReg  *)0x400A63FCU) /**< \brief (PEVC) Version */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/* ========== Instance parameters for PEVC peripheral ========== */
#define PEVC_EVIN_BITS              31
#define PEVC_GCLK_NUM_0             8
#define PEVC_GCLK_NUM_1             9
#define PEVC_GCLK_NUM_LSB           8
#define PEVC_GCLK_NUM_MSB           9
#define PEVC_GCLK_NUM_SIZE          2
#define PEVC_PADS_BITS              4
#define PEVC_TRIGOUT_BITS           19

// GENERATORS
#define PEVC_ID_GEN_PAD_0          0 /* EVS IGF */
#define PEVC_ID_GEN_PAD_1          1 /* EVS IGF */
#define PEVC_ID_GEN_PAD_2          2 /* EVS IGF */
#define PEVC_ID_GEN_PAD_3          3 /* EVS IGF */
#define PEVC_ID_GEN_GCLK_0         4 /* EVS */
#define PEVC_ID_GEN_GCLK_1         5 /* EVS */
#define PEVC_ID_GEN_AST_0          6 /* EVS */
#define PEVC_ID_GEN_AST_1          7 /* EVS */
#define PEVC_ID_GEN_AST_2          8 /* EVS */
#define PEVC_ID_GEN_AST_3          9 /* EVS */
#define PEVC_ID_GEN_AST_4          10 /* EVS */
#define PEVC_ID_GEN_ACIFC_CHANNEL_POSITIVE_0 11
#define PEVC_ID_GEN_ACIFC_CHANNEL_POSITIVE_1 12
#define PEVC_ID_GEN_ACIFC_CHANNEL_POSITIVE_2 13
#define PEVC_ID_GEN_ACIFC_CHANNEL_POSITIVE_3 14
#define PEVC_ID_GEN_ACIFC_CHANNEL_NEGATIVE_0 15
#define PEVC_ID_GEN_ACIFC_CHANNEL_NEGATIVE_1 16
#define PEVC_ID_GEN_ACIFC_CHANNEL_NEGATIVE_2 17
#define PEVC_ID_GEN_ACIFC_CHANNEL_NEGATIVE_3 18
#define PEVC_ID_GEN_ACIFC_WINDOW_0 19
#define PEVC_ID_GEN_ACIFC_WINDOW_1 20
#define PEVC_ID_GEN_TC0_A0         21 /* EVS */
#define PEVC_ID_GEN_TC0_A1         22 /* EVS */
#define PEVC_ID_GEN_TC0_A2         23 /* EVS */
#define PEVC_ID_GEN_TC0_B0         24 /* EVS */
#define PEVC_ID_GEN_TC0_B1         25 /* EVS */
#define PEVC_ID_GEN_TC0_B2         26 /* EVS */
#define PEVC_ID_GEN_ADCIFE_WM      27
#define PEVC_ID_GEN_ADCIFE_EOC     28
#define PEVC_ID_GEN_VREGIFG_SSWRDY 29
#define PEVC_ID_GEN_PICOUART       30 /* EVS */

#define PEVC_CHMX0_EVMX_SIZE_IMPL 5
#define PEVC_CHMX0_EVMX_MASK_IMPL 0x0000001F

#define PEVC_EVS_IMPL 0b1000111111000000000011111111111
#define PEVC_IGF_IMPL 0b0000000000000000000000000001111

// USERS / CHANNELS
#define PEVC_ID_USER_PDCA_0        0
#define PEVC_ID_USER_PDCA_1        1
#define PEVC_ID_USER_PDCA_2        2
#define PEVC_ID_USER_PDCA_3        3
#define PEVC_ID_USER_ADCIFE_SOC    4
#define PEVC_ID_USER_DACC_CONV     5
#define PEVC_ID_USER_CATB          6
#define PEVC_ID_USER_TC1_A0        8
#define PEVC_ID_USER_TC1_A1        9
#define PEVC_ID_USER_TC1_A2        10
#define PEVC_ID_USER_TC1_B0        11
#define PEVC_ID_USER_TC1_B1        12
#define PEVC_ID_USER_TC1_B2        13
#define PEVC_ID_USER_ACIFC         14
#define PEVC_ID_USER_PARC_START    15
#define PEVC_ID_USER_PARC_STOP     16
#define PEVC_ID_USER_VREGIFG_SSWREQ 17
#define PEVC_ID_USER_VREGIFG_SSWDIS 18

#endif /* _SAM4L_PEVC_INSTANCE_ */
