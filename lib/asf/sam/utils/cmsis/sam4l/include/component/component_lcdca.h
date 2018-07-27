/**
 * \file
 *
 * \brief Component description for LCDCA
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

#ifndef _SAM4L_LCDCA_COMPONENT_
#define _SAM4L_LCDCA_COMPONENT_

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR LCDCA */
/* ========================================================================== */
/** \addtogroup SAM4L_LCDCA LCD Controller */
/*@{*/

#define REV_LCDCA                   0x100

/* -------- LCDCA_CR : (LCDCA Offset: 0x00) ( /W 32) Control Register -------- */
#define LCDCA_CR_OFFSET             0x00         /**< \brief (LCDCA_CR offset) Control Register */
#define LCDCA_CR_RESETVALUE         0x00000000   /**< \brief (LCDCA_CR reset_value) Control Register */

#define LCDCA_CR_DIS_Pos            0            /**< \brief (LCDCA_CR) Disable */
#define LCDCA_CR_DIS                (0x1u << LCDCA_CR_DIS_Pos)
#define LCDCA_CR_EN_Pos             1            /**< \brief (LCDCA_CR) Enable */
#define LCDCA_CR_EN                 (0x1u << LCDCA_CR_EN_Pos)
#define LCDCA_CR_FC0DIS_Pos         2            /**< \brief (LCDCA_CR) Frame Counter 0 Disable */
#define LCDCA_CR_FC0DIS             (0x1u << LCDCA_CR_FC0DIS_Pos)
#define LCDCA_CR_FC0EN_Pos          3            /**< \brief (LCDCA_CR) Frame Counter 0 Enable */
#define LCDCA_CR_FC0EN              (0x1u << LCDCA_CR_FC0EN_Pos)
#define LCDCA_CR_FC1DIS_Pos         4            /**< \brief (LCDCA_CR) Frame Counter 1 Disable */
#define LCDCA_CR_FC1DIS             (0x1u << LCDCA_CR_FC1DIS_Pos)
#define LCDCA_CR_FC1EN_Pos          5            /**< \brief (LCDCA_CR) Frame Counter 1 Enable */
#define LCDCA_CR_FC1EN              (0x1u << LCDCA_CR_FC1EN_Pos)
#define LCDCA_CR_FC2DIS_Pos         6            /**< \brief (LCDCA_CR) Frame Counter 2 Disable */
#define LCDCA_CR_FC2DIS             (0x1u << LCDCA_CR_FC2DIS_Pos)
#define LCDCA_CR_FC2EN_Pos          7            /**< \brief (LCDCA_CR) Frame Counter 2 Enable */
#define LCDCA_CR_FC2EN              (0x1u << LCDCA_CR_FC2EN_Pos)
#define LCDCA_CR_CDM_Pos            8            /**< \brief (LCDCA_CR) Clear Display Memory */
#define LCDCA_CR_CDM                (0x1u << LCDCA_CR_CDM_Pos)
#define LCDCA_CR_WDIS_Pos           9            /**< \brief (LCDCA_CR) Wake up Disable */
#define LCDCA_CR_WDIS               (0x1u << LCDCA_CR_WDIS_Pos)
#define LCDCA_CR_WEN_Pos            10           /**< \brief (LCDCA_CR) Wake up Enable */
#define LCDCA_CR_WEN                (0x1u << LCDCA_CR_WEN_Pos)
#define LCDCA_CR_BSTART_Pos         11           /**< \brief (LCDCA_CR) Blinking Start */
#define LCDCA_CR_BSTART             (0x1u << LCDCA_CR_BSTART_Pos)
#define LCDCA_CR_BSTOP_Pos          12           /**< \brief (LCDCA_CR) Blinking Stop */
#define LCDCA_CR_BSTOP              (0x1u << LCDCA_CR_BSTOP_Pos)
#define LCDCA_CR_CSTART_Pos         13           /**< \brief (LCDCA_CR) Circular Shift Start */
#define LCDCA_CR_CSTART             (0x1u << LCDCA_CR_CSTART_Pos)
#define LCDCA_CR_CSTOP_Pos          14           /**< \brief (LCDCA_CR) Circular Shift Stop */
#define LCDCA_CR_CSTOP              (0x1u << LCDCA_CR_CSTOP_Pos)
#define LCDCA_CR_MASK               0x00007FFFu  /**< \brief (LCDCA_CR) MASK Register */

/* -------- LCDCA_CFG : (LCDCA Offset: 0x04) (R/W 32) Configuration Register -------- */
#define LCDCA_CFG_OFFSET            0x04         /**< \brief (LCDCA_CFG offset) Configuration Register */
#define LCDCA_CFG_RESETVALUE        0x00000000   /**< \brief (LCDCA_CFG reset_value) Configuration Register */

#define LCDCA_CFG_XBIAS_Pos         0            /**< \brief (LCDCA_CFG) External Bias Generation */
#define LCDCA_CFG_XBIAS             (0x1u << LCDCA_CFG_XBIAS_Pos)
#define LCDCA_CFG_WMOD_Pos          1            /**< \brief (LCDCA_CFG) Waveform Mode */
#define LCDCA_CFG_WMOD              (0x1u << LCDCA_CFG_WMOD_Pos)
#define LCDCA_CFG_BLANK_Pos         2            /**< \brief (LCDCA_CFG) Blank LCD */
#define LCDCA_CFG_BLANK             (0x1u << LCDCA_CFG_BLANK_Pos)
#define LCDCA_CFG_LOCK_Pos          3            /**< \brief (LCDCA_CFG) Lock */
#define LCDCA_CFG_LOCK              (0x1u << LCDCA_CFG_LOCK_Pos)
#define LCDCA_CFG_DUTY_Pos          8            /**< \brief (LCDCA_CFG) Duty Select */
#define LCDCA_CFG_DUTY_Msk          (0x3u << LCDCA_CFG_DUTY_Pos)
#define LCDCA_CFG_DUTY(value)       ((LCDCA_CFG_DUTY_Msk & ((value) << LCDCA_CFG_DUTY_Pos)))
#define LCDCA_CFG_FCST_Pos          16           /**< \brief (LCDCA_CFG) Fine Contrast */
#define LCDCA_CFG_FCST_Msk          (0x3Fu << LCDCA_CFG_FCST_Pos)
#define LCDCA_CFG_FCST(value)       ((LCDCA_CFG_FCST_Msk & ((value) << LCDCA_CFG_FCST_Pos)))
#define LCDCA_CFG_NSU_Pos           24           /**< \brief (LCDCA_CFG) Number of Segment Terminals in Use */
#define LCDCA_CFG_NSU_Msk           (0x3Fu << LCDCA_CFG_NSU_Pos)
#define LCDCA_CFG_NSU(value)        ((LCDCA_CFG_NSU_Msk & ((value) << LCDCA_CFG_NSU_Pos)))
#define LCDCA_CFG_MASK              0x3F3F030Fu  /**< \brief (LCDCA_CFG) MASK Register */

/* -------- LCDCA_TIM : (LCDCA Offset: 0x08) (R/W 32) Timing Register -------- */
#define LCDCA_TIM_OFFSET            0x08         /**< \brief (LCDCA_TIM offset) Timing Register */
#define LCDCA_TIM_RESETVALUE        0x00000000   /**< \brief (LCDCA_TIM reset_value) Timing Register */

#define LCDCA_TIM_PRESC_Pos         0            /**< \brief (LCDCA_TIM) LCD Prescaler Select */
#define LCDCA_TIM_PRESC             (0x1u << LCDCA_TIM_PRESC_Pos)
#define LCDCA_TIM_CLKDIV_Pos        1            /**< \brief (LCDCA_TIM) LCD Clock Division */
#define LCDCA_TIM_CLKDIV_Msk        (0x7u << LCDCA_TIM_CLKDIV_Pos)
#define LCDCA_TIM_CLKDIV(value)     ((LCDCA_TIM_CLKDIV_Msk & ((value) << LCDCA_TIM_CLKDIV_Pos)))
#define LCDCA_TIM_FC0_Pos           8            /**< \brief (LCDCA_TIM) Frame Counter 0 */
#define LCDCA_TIM_FC0_Msk           (0x1Fu << LCDCA_TIM_FC0_Pos)
#define LCDCA_TIM_FC0(value)        ((LCDCA_TIM_FC0_Msk & ((value) << LCDCA_TIM_FC0_Pos)))
#define LCDCA_TIM_FC0PB_Pos         13           /**< \brief (LCDCA_TIM) Frame Counter 0 Prescaler Bypass */
#define LCDCA_TIM_FC0PB             (0x1u << LCDCA_TIM_FC0PB_Pos)
#define LCDCA_TIM_FC1_Pos           16           /**< \brief (LCDCA_TIM) Frame Counter 1 */
#define LCDCA_TIM_FC1_Msk           (0x1Fu << LCDCA_TIM_FC1_Pos)
#define LCDCA_TIM_FC1(value)        ((LCDCA_TIM_FC1_Msk & ((value) << LCDCA_TIM_FC1_Pos)))
#define LCDCA_TIM_FC2_Pos           24           /**< \brief (LCDCA_TIM) Frame Counter 2 */
#define LCDCA_TIM_FC2_Msk           (0x1Fu << LCDCA_TIM_FC2_Pos)
#define LCDCA_TIM_FC2(value)        ((LCDCA_TIM_FC2_Msk & ((value) << LCDCA_TIM_FC2_Pos)))
#define LCDCA_TIM_MASK              0x1F1F3F0Fu  /**< \brief (LCDCA_TIM) MASK Register */

/* -------- LCDCA_SR : (LCDCA Offset: 0x0C) (R/  32) Status Register -------- */
#define LCDCA_SR_OFFSET             0x0C         /**< \brief (LCDCA_SR offset) Status Register */
#define LCDCA_SR_RESETVALUE         0x00000000   /**< \brief (LCDCA_SR reset_value) Status Register */

#define LCDCA_SR_FC0R_Pos           0            /**< \brief (LCDCA_SR) Frame Counter 0 Rollover */
#define LCDCA_SR_FC0R               (0x1u << LCDCA_SR_FC0R_Pos)
#define LCDCA_SR_FC0S_Pos           1            /**< \brief (LCDCA_SR) Frame Counter 0 Status */
#define LCDCA_SR_FC0S               (0x1u << LCDCA_SR_FC0S_Pos)
#define LCDCA_SR_FC1S_Pos           2            /**< \brief (LCDCA_SR) Frame Counter 1 Status */
#define LCDCA_SR_FC1S               (0x1u << LCDCA_SR_FC1S_Pos)
#define LCDCA_SR_FC2S_Pos           3            /**< \brief (LCDCA_SR) Frame Counter 2 Status */
#define LCDCA_SR_FC2S               (0x1u << LCDCA_SR_FC2S_Pos)
#define LCDCA_SR_EN_Pos             4            /**< \brief (LCDCA_SR) LCDCA Status */
#define LCDCA_SR_EN                 (0x1u << LCDCA_SR_EN_Pos)
#define LCDCA_SR_WEN_Pos            5            /**< \brief (LCDCA_SR) Wake up Status */
#define LCDCA_SR_WEN                (0x1u << LCDCA_SR_WEN_Pos)
#define LCDCA_SR_BLKS_Pos           6            /**< \brief (LCDCA_SR) Blink Status */
#define LCDCA_SR_BLKS               (0x1u << LCDCA_SR_BLKS_Pos)
#define LCDCA_SR_CSRS_Pos           7            /**< \brief (LCDCA_SR) Circular Shift Register Status */
#define LCDCA_SR_CSRS               (0x1u << LCDCA_SR_CSRS_Pos)
#define LCDCA_SR_CPS_Pos            8            /**< \brief (LCDCA_SR) Charge Pump Status */
#define LCDCA_SR_CPS                (0x1u << LCDCA_SR_CPS_Pos)
#define LCDCA_SR_MASK               0x000001FFu  /**< \brief (LCDCA_SR) MASK Register */

/* -------- LCDCA_SCR : (LCDCA Offset: 0x10) ( /W 32) Status Clear Register -------- */
#define LCDCA_SCR_OFFSET            0x10         /**< \brief (LCDCA_SCR offset) Status Clear Register */
#define LCDCA_SCR_RESETVALUE        0x00000000   /**< \brief (LCDCA_SCR reset_value) Status Clear Register */

#define LCDCA_SCR_FC0R_Pos          0            /**< \brief (LCDCA_SCR) Frame Counter 0 Rollover */
#define LCDCA_SCR_FC0R              (0x1u << LCDCA_SCR_FC0R_Pos)
#define LCDCA_SCR_MASK              0x00000001u  /**< \brief (LCDCA_SCR) MASK Register */

/* -------- LCDCA_DRL0 : (LCDCA Offset: 0x14) (R/W 32) Data Register Low 0 -------- */
#define LCDCA_DRL0_OFFSET           0x14         /**< \brief (LCDCA_DRL0 offset) Data Register Low 0 */
#define LCDCA_DRL0_RESETVALUE       0x00000000   /**< \brief (LCDCA_DRL0 reset_value) Data Register Low 0 */

#define LCDCA_DRL0_DATA_Pos         0            /**< \brief (LCDCA_DRL0) Segments Value */
#define LCDCA_DRL0_DATA_Msk         (0xFFFFFFFFu << LCDCA_DRL0_DATA_Pos)
#define LCDCA_DRL0_DATA(value)      ((LCDCA_DRL0_DATA_Msk & ((value) << LCDCA_DRL0_DATA_Pos)))
#define LCDCA_DRL0_MASK             0xFFFFFFFFu  /**< \brief (LCDCA_DRL0) MASK Register */

/* -------- LCDCA_DRH0 : (LCDCA Offset: 0x18) (R/W 32) Data Register High 0 -------- */
#define LCDCA_DRH0_OFFSET           0x18         /**< \brief (LCDCA_DRH0 offset) Data Register High 0 */
#define LCDCA_DRH0_RESETVALUE       0x00000000   /**< \brief (LCDCA_DRH0 reset_value) Data Register High 0 */

#define LCDCA_DRH0_DATA_Pos         0            /**< \brief (LCDCA_DRH0) Segments Value */
#define LCDCA_DRH0_DATA_Msk         (0xFFu << LCDCA_DRH0_DATA_Pos)
#define LCDCA_DRH0_DATA(value)      ((LCDCA_DRH0_DATA_Msk & ((value) << LCDCA_DRH0_DATA_Pos)))
#define LCDCA_DRH0_MASK             0x000000FFu  /**< \brief (LCDCA_DRH0) MASK Register */

/* -------- LCDCA_DRL1 : (LCDCA Offset: 0x1C) (R/W 32) Data Register Low 1 -------- */
#define LCDCA_DRL1_OFFSET           0x1C         /**< \brief (LCDCA_DRL1 offset) Data Register Low 1 */
#define LCDCA_DRL1_RESETVALUE       0x00000000   /**< \brief (LCDCA_DRL1 reset_value) Data Register Low 1 */

#define LCDCA_DRL1_DATA_Pos         0            /**< \brief (LCDCA_DRL1) Segments Value */
#define LCDCA_DRL1_DATA_Msk         (0xFFFFFFFFu << LCDCA_DRL1_DATA_Pos)
#define LCDCA_DRL1_DATA(value)      ((LCDCA_DRL1_DATA_Msk & ((value) << LCDCA_DRL1_DATA_Pos)))
#define LCDCA_DRL1_MASK             0xFFFFFFFFu  /**< \brief (LCDCA_DRL1) MASK Register */

/* -------- LCDCA_DRH1 : (LCDCA Offset: 0x20) (R/W 32) Data Register High 1 -------- */
#define LCDCA_DRH1_OFFSET           0x20         /**< \brief (LCDCA_DRH1 offset) Data Register High 1 */
#define LCDCA_DRH1_RESETVALUE       0x00000000   /**< \brief (LCDCA_DRH1 reset_value) Data Register High 1 */

#define LCDCA_DRH1_DATA_Pos         0            /**< \brief (LCDCA_DRH1) Segments Value */
#define LCDCA_DRH1_DATA_Msk         (0xFFu << LCDCA_DRH1_DATA_Pos)
#define LCDCA_DRH1_DATA(value)      ((LCDCA_DRH1_DATA_Msk & ((value) << LCDCA_DRH1_DATA_Pos)))
#define LCDCA_DRH1_MASK             0x000000FFu  /**< \brief (LCDCA_DRH1) MASK Register */

/* -------- LCDCA_DRL2 : (LCDCA Offset: 0x24) (R/W 32) Data Register Low 2 -------- */
#define LCDCA_DRL2_OFFSET           0x24         /**< \brief (LCDCA_DRL2 offset) Data Register Low 2 */
#define LCDCA_DRL2_RESETVALUE       0x00000000   /**< \brief (LCDCA_DRL2 reset_value) Data Register Low 2 */

#define LCDCA_DRL2_DATA_Pos         0            /**< \brief (LCDCA_DRL2) Segments Value */
#define LCDCA_DRL2_DATA_Msk         (0xFFFFFFFFu << LCDCA_DRL2_DATA_Pos)
#define LCDCA_DRL2_DATA(value)      ((LCDCA_DRL2_DATA_Msk & ((value) << LCDCA_DRL2_DATA_Pos)))
#define LCDCA_DRL2_MASK             0xFFFFFFFFu  /**< \brief (LCDCA_DRL2) MASK Register */

/* -------- LCDCA_DRH2 : (LCDCA Offset: 0x28) (R/W 32) Data Register High 2 -------- */
#define LCDCA_DRH2_OFFSET           0x28         /**< \brief (LCDCA_DRH2 offset) Data Register High 2 */
#define LCDCA_DRH2_RESETVALUE       0x00000000   /**< \brief (LCDCA_DRH2 reset_value) Data Register High 2 */

#define LCDCA_DRH2_DATA_Pos         0            /**< \brief (LCDCA_DRH2) Segments Value */
#define LCDCA_DRH2_DATA_Msk         (0xFFu << LCDCA_DRH2_DATA_Pos)
#define LCDCA_DRH2_DATA(value)      ((LCDCA_DRH2_DATA_Msk & ((value) << LCDCA_DRH2_DATA_Pos)))
#define LCDCA_DRH2_MASK             0x000000FFu  /**< \brief (LCDCA_DRH2) MASK Register */

/* -------- LCDCA_DRL3 : (LCDCA Offset: 0x2C) (R/W 32) Data Register Low 3 -------- */
#define LCDCA_DRL3_OFFSET           0x2C         /**< \brief (LCDCA_DRL3 offset) Data Register Low 3 */
#define LCDCA_DRL3_RESETVALUE       0x00000000   /**< \brief (LCDCA_DRL3 reset_value) Data Register Low 3 */

#define LCDCA_DRL3_DATA_Pos         0            /**< \brief (LCDCA_DRL3) Segments Value */
#define LCDCA_DRL3_DATA_Msk         (0xFFFFFFFFu << LCDCA_DRL3_DATA_Pos)
#define LCDCA_DRL3_DATA(value)      ((LCDCA_DRL3_DATA_Msk & ((value) << LCDCA_DRL3_DATA_Pos)))
#define LCDCA_DRL3_MASK             0xFFFFFFFFu  /**< \brief (LCDCA_DRL3) MASK Register */

/* -------- LCDCA_DRH3 : (LCDCA Offset: 0x30) (R/W 32) Data Register High 3 -------- */
#define LCDCA_DRH3_OFFSET           0x30         /**< \brief (LCDCA_DRH3 offset) Data Register High 3 */
#define LCDCA_DRH3_RESETVALUE       0x00000000   /**< \brief (LCDCA_DRH3 reset_value) Data Register High 3 */

#define LCDCA_DRH3_DATA_Pos         0            /**< \brief (LCDCA_DRH3) Segments Value */
#define LCDCA_DRH3_DATA_Msk         (0xFFu << LCDCA_DRH3_DATA_Pos)
#define LCDCA_DRH3_DATA(value)      ((LCDCA_DRH3_DATA_Msk & ((value) << LCDCA_DRH3_DATA_Pos)))
#define LCDCA_DRH3_MASK             0x000000FFu  /**< \brief (LCDCA_DRH3) MASK Register */

/* -------- LCDCA_IADR : (LCDCA Offset: 0x34) ( /W 32) Indirect Access Data Register -------- */
#define LCDCA_IADR_OFFSET           0x34         /**< \brief (LCDCA_IADR offset) Indirect Access Data Register */
#define LCDCA_IADR_RESETVALUE       0x00000000   /**< \brief (LCDCA_IADR reset_value) Indirect Access Data Register */

#define LCDCA_IADR_DATA_Pos         0            /**< \brief (LCDCA_IADR) Segments Value */
#define LCDCA_IADR_DATA_Msk         (0xFFu << LCDCA_IADR_DATA_Pos)
#define LCDCA_IADR_DATA(value)      ((LCDCA_IADR_DATA_Msk & ((value) << LCDCA_IADR_DATA_Pos)))
#define LCDCA_IADR_DMASK_Pos        8            /**< \brief (LCDCA_IADR) Data Mask */
#define LCDCA_IADR_DMASK_Msk        (0xFFu << LCDCA_IADR_DMASK_Pos)
#define LCDCA_IADR_DMASK(value)     ((LCDCA_IADR_DMASK_Msk & ((value) << LCDCA_IADR_DMASK_Pos)))
#define LCDCA_IADR_OFF_Pos          16           /**< \brief (LCDCA_IADR) Byte Offset */
#define LCDCA_IADR_OFF_Msk          (0x1Fu << LCDCA_IADR_OFF_Pos)
#define LCDCA_IADR_OFF(value)       ((LCDCA_IADR_OFF_Msk & ((value) << LCDCA_IADR_OFF_Pos)))
#define LCDCA_IADR_MASK             0x001FFFFFu  /**< \brief (LCDCA_IADR) MASK Register */

/* -------- LCDCA_BCFG : (LCDCA Offset: 0x38) (R/W 32) Blink Configuration Register -------- */
#define LCDCA_BCFG_OFFSET           0x38         /**< \brief (LCDCA_BCFG offset) Blink Configuration Register */
#define LCDCA_BCFG_RESETVALUE       0x00000000   /**< \brief (LCDCA_BCFG reset_value) Blink Configuration Register */

#define LCDCA_BCFG_MODE_Pos         0            /**< \brief (LCDCA_BCFG) Blinking Mode */
#define LCDCA_BCFG_MODE             (0x1u << LCDCA_BCFG_MODE_Pos)
#define LCDCA_BCFG_FCS_Pos          1            /**< \brief (LCDCA_BCFG) Frame Counter Selection */
#define LCDCA_BCFG_FCS_Msk          (0x3u << LCDCA_BCFG_FCS_Pos)
#define LCDCA_BCFG_FCS(value)       ((LCDCA_BCFG_FCS_Msk & ((value) << LCDCA_BCFG_FCS_Pos)))
#define LCDCA_BCFG_BSS0_Pos         8            /**< \brief (LCDCA_BCFG) Blink Segment Selection 0 */
#define LCDCA_BCFG_BSS0_Msk         (0xFu << LCDCA_BCFG_BSS0_Pos)
#define LCDCA_BCFG_BSS0(value)      ((LCDCA_BCFG_BSS0_Msk & ((value) << LCDCA_BCFG_BSS0_Pos)))
#define LCDCA_BCFG_BSS1_Pos         12           /**< \brief (LCDCA_BCFG) Blink Segment Selection 1 */
#define LCDCA_BCFG_BSS1_Msk         (0xFu << LCDCA_BCFG_BSS1_Pos)
#define LCDCA_BCFG_BSS1(value)      ((LCDCA_BCFG_BSS1_Msk & ((value) << LCDCA_BCFG_BSS1_Pos)))
#define LCDCA_BCFG_MASK             0x0000FF07u  /**< \brief (LCDCA_BCFG) MASK Register */

/* -------- LCDCA_CSRCFG : (LCDCA Offset: 0x3C) (R/W 32) Circular Shift Register Configuration -------- */
#define LCDCA_CSRCFG_OFFSET         0x3C         /**< \brief (LCDCA_CSRCFG offset) Circular Shift Register Configuration */
#define LCDCA_CSRCFG_RESETVALUE     0x00000000   /**< \brief (LCDCA_CSRCFG reset_value) Circular Shift Register Configuration */

#define LCDCA_CSRCFG_DIR_Pos        0            /**< \brief (LCDCA_CSRCFG) Direction */
#define LCDCA_CSRCFG_DIR            (0x1u << LCDCA_CSRCFG_DIR_Pos)
#define LCDCA_CSRCFG_FCS_Pos        1            /**< \brief (LCDCA_CSRCFG) Frame Counter Selection */
#define LCDCA_CSRCFG_FCS_Msk        (0x3u << LCDCA_CSRCFG_FCS_Pos)
#define LCDCA_CSRCFG_FCS(value)     ((LCDCA_CSRCFG_FCS_Msk & ((value) << LCDCA_CSRCFG_FCS_Pos)))
#define LCDCA_CSRCFG_SIZE_Pos       3            /**< \brief (LCDCA_CSRCFG) Size */
#define LCDCA_CSRCFG_SIZE_Msk       (0x7u << LCDCA_CSRCFG_SIZE_Pos)
#define LCDCA_CSRCFG_SIZE(value)    ((LCDCA_CSRCFG_SIZE_Msk & ((value) << LCDCA_CSRCFG_SIZE_Pos)))
#define LCDCA_CSRCFG_DATA_Pos       8            /**< \brief (LCDCA_CSRCFG) Circular Shift Register Value */
#define LCDCA_CSRCFG_DATA_Msk       (0xFFu << LCDCA_CSRCFG_DATA_Pos)
#define LCDCA_CSRCFG_DATA(value)    ((LCDCA_CSRCFG_DATA_Msk & ((value) << LCDCA_CSRCFG_DATA_Pos)))
#define LCDCA_CSRCFG_MASK           0x0000FF3Fu  /**< \brief (LCDCA_CSRCFG) MASK Register */

/* -------- LCDCA_CMCFG : (LCDCA Offset: 0x40) (R/W 32) Character Mapping Configuration Register -------- */
#define LCDCA_CMCFG_OFFSET          0x40         /**< \brief (LCDCA_CMCFG offset) Character Mapping Configuration Register */
#define LCDCA_CMCFG_RESETVALUE      0x00000000   /**< \brief (LCDCA_CMCFG reset_value) Character Mapping Configuration Register */

#define LCDCA_CMCFG_DREV_Pos        0            /**< \brief (LCDCA_CMCFG) Digit Reverse Mode */
#define LCDCA_CMCFG_DREV            (0x1u << LCDCA_CMCFG_DREV_Pos)
#define LCDCA_CMCFG_TDG_Pos         1            /**< \brief (LCDCA_CMCFG) Type of Digit */
#define LCDCA_CMCFG_TDG_Msk         (0x3u << LCDCA_CMCFG_TDG_Pos)
#define LCDCA_CMCFG_TDG(value)      ((LCDCA_CMCFG_TDG_Msk & ((value) << LCDCA_CMCFG_TDG_Pos)))
#define LCDCA_CMCFG_STSEG_Pos       8            /**< \brief (LCDCA_CMCFG) Start Segment */
#define LCDCA_CMCFG_STSEG_Msk       (0x3Fu << LCDCA_CMCFG_STSEG_Pos)
#define LCDCA_CMCFG_STSEG(value)    ((LCDCA_CMCFG_STSEG_Msk & ((value) << LCDCA_CMCFG_STSEG_Pos)))
#define LCDCA_CMCFG_MASK            0x00003F07u  /**< \brief (LCDCA_CMCFG) MASK Register */

/* -------- LCDCA_CMDR : (LCDCA Offset: 0x44) ( /W 32) Character Mapping Data Register -------- */
#define LCDCA_CMDR_OFFSET           0x44         /**< \brief (LCDCA_CMDR offset) Character Mapping Data Register */
#define LCDCA_CMDR_RESETVALUE       0x00000000   /**< \brief (LCDCA_CMDR reset_value) Character Mapping Data Register */

#define LCDCA_CMDR_ASCII_Pos        0            /**< \brief (LCDCA_CMDR) ASCII Code */
#define LCDCA_CMDR_ASCII_Msk        (0x7Fu << LCDCA_CMDR_ASCII_Pos)
#define LCDCA_CMDR_ASCII(value)     ((LCDCA_CMDR_ASCII_Msk & ((value) << LCDCA_CMDR_ASCII_Pos)))
#define LCDCA_CMDR_MASK             0x0000007Fu  /**< \brief (LCDCA_CMDR) MASK Register */

/* -------- LCDCA_ACMCFG : (LCDCA Offset: 0x48) (R/W 32) Automated Character Mapping Configuration Register -------- */
#define LCDCA_ACMCFG_OFFSET         0x48         /**< \brief (LCDCA_ACMCFG offset) Automated Character Mapping Configuration Register */
#define LCDCA_ACMCFG_RESETVALUE     0x00000000   /**< \brief (LCDCA_ACMCFG reset_value) Automated Character Mapping Configuration Register */

#define LCDCA_ACMCFG_EN_Pos         0            /**< \brief (LCDCA_ACMCFG) Enable */
#define LCDCA_ACMCFG_EN             (0x1u << LCDCA_ACMCFG_EN_Pos)
#define LCDCA_ACMCFG_FCS_Pos        1            /**< \brief (LCDCA_ACMCFG) Frame Counter Selection */
#define LCDCA_ACMCFG_FCS_Msk        (0x3u << LCDCA_ACMCFG_FCS_Pos)
#define LCDCA_ACMCFG_FCS(value)     ((LCDCA_ACMCFG_FCS_Msk & ((value) << LCDCA_ACMCFG_FCS_Pos)))
#define LCDCA_ACMCFG_MODE_Pos       3            /**< \brief (LCDCA_ACMCFG) Mode (sequential or scrolling) */
#define LCDCA_ACMCFG_MODE           (0x1u << LCDCA_ACMCFG_MODE_Pos)
#define LCDCA_ACMCFG_DREV_Pos       4            /**< \brief (LCDCA_ACMCFG) Digit Reverse */
#define LCDCA_ACMCFG_DREV           (0x1u << LCDCA_ACMCFG_DREV_Pos)
#define LCDCA_ACMCFG_TDG_Pos        5            /**< \brief (LCDCA_ACMCFG) Type of Digit */
#define LCDCA_ACMCFG_TDG_Msk        (0x3u << LCDCA_ACMCFG_TDG_Pos)
#define LCDCA_ACMCFG_TDG(value)     ((LCDCA_ACMCFG_TDG_Msk & ((value) << LCDCA_ACMCFG_TDG_Pos)))
#define LCDCA_ACMCFG_STSEG_Pos      8            /**< \brief (LCDCA_ACMCFG) Start Segment */
#define LCDCA_ACMCFG_STSEG_Msk      (0x3Fu << LCDCA_ACMCFG_STSEG_Pos)
#define LCDCA_ACMCFG_STSEG(value)   ((LCDCA_ACMCFG_STSEG_Msk & ((value) << LCDCA_ACMCFG_STSEG_Pos)))
#define LCDCA_ACMCFG_STEPS_Pos      16           /**< \brief (LCDCA_ACMCFG) Scrolling Steps */
#define LCDCA_ACMCFG_STEPS_Msk      (0xFFu << LCDCA_ACMCFG_STEPS_Pos)
#define LCDCA_ACMCFG_STEPS(value)   ((LCDCA_ACMCFG_STEPS_Msk & ((value) << LCDCA_ACMCFG_STEPS_Pos)))
#define LCDCA_ACMCFG_DIGN_Pos       24           /**< \brief (LCDCA_ACMCFG) Digit Number */
#define LCDCA_ACMCFG_DIGN_Msk       (0xFu << LCDCA_ACMCFG_DIGN_Pos)
#define LCDCA_ACMCFG_DIGN(value)    ((LCDCA_ACMCFG_DIGN_Msk & ((value) << LCDCA_ACMCFG_DIGN_Pos)))
#define LCDCA_ACMCFG_MASK           0x0FFF3F7Fu  /**< \brief (LCDCA_ACMCFG) MASK Register */

/* -------- LCDCA_ACMDR : (LCDCA Offset: 0x4C) ( /W 32) Automated Character Mapping Data Register -------- */
#define LCDCA_ACMDR_OFFSET          0x4C         /**< \brief (LCDCA_ACMDR offset) Automated Character Mapping Data Register */
#define LCDCA_ACMDR_RESETVALUE      0x00000000   /**< \brief (LCDCA_ACMDR reset_value) Automated Character Mapping Data Register */

#define LCDCA_ACMDR_ASCII_Pos       0            /**< \brief (LCDCA_ACMDR) ASCII Code */
#define LCDCA_ACMDR_ASCII_Msk       (0x7Fu << LCDCA_ACMDR_ASCII_Pos)
#define LCDCA_ACMDR_ASCII(value)    ((LCDCA_ACMDR_ASCII_Msk & ((value) << LCDCA_ACMDR_ASCII_Pos)))
#define LCDCA_ACMDR_MASK            0x0000007Fu  /**< \brief (LCDCA_ACMDR) MASK Register */

/* -------- LCDCA_ABMCFG : (LCDCA Offset: 0x50) (R/W 32) Automated Bit Mapping Configuration Register -------- */
#define LCDCA_ABMCFG_OFFSET         0x50         /**< \brief (LCDCA_ABMCFG offset) Automated Bit Mapping Configuration Register */
#define LCDCA_ABMCFG_RESETVALUE     0x00000000   /**< \brief (LCDCA_ABMCFG reset_value) Automated Bit Mapping Configuration Register */

#define LCDCA_ABMCFG_EN_Pos         0            /**< \brief (LCDCA_ABMCFG) Enable */
#define LCDCA_ABMCFG_EN             (0x1u << LCDCA_ABMCFG_EN_Pos)
#define LCDCA_ABMCFG_FCS_Pos        1            /**< \brief (LCDCA_ABMCFG) Frame Counter Selection */
#define LCDCA_ABMCFG_FCS_Msk        (0x3u << LCDCA_ABMCFG_FCS_Pos)
#define LCDCA_ABMCFG_FCS(value)     ((LCDCA_ABMCFG_FCS_Msk & ((value) << LCDCA_ABMCFG_FCS_Pos)))
#define LCDCA_ABMCFG_SIZE_Pos       8            /**< \brief (LCDCA_ABMCFG) Size */
#define LCDCA_ABMCFG_SIZE_Msk       (0x1Fu << LCDCA_ABMCFG_SIZE_Pos)
#define LCDCA_ABMCFG_SIZE(value)    ((LCDCA_ABMCFG_SIZE_Msk & ((value) << LCDCA_ABMCFG_SIZE_Pos)))
#define LCDCA_ABMCFG_MASK           0x00001F07u  /**< \brief (LCDCA_ABMCFG) MASK Register */

/* -------- LCDCA_ABMDR : (LCDCA Offset: 0x54) ( /W 32) Automated Bit Mapping Data Register -------- */
#define LCDCA_ABMDR_OFFSET          0x54         /**< \brief (LCDCA_ABMDR offset) Automated Bit Mapping Data Register */
#define LCDCA_ABMDR_RESETVALUE      0x00000000   /**< \brief (LCDCA_ABMDR reset_value) Automated Bit Mapping Data Register */

#define LCDCA_ABMDR_DATA_Pos        0            /**< \brief (LCDCA_ABMDR) Segments Value */
#define LCDCA_ABMDR_DATA_Msk        (0xFFu << LCDCA_ABMDR_DATA_Pos)
#define LCDCA_ABMDR_DATA(value)     ((LCDCA_ABMDR_DATA_Msk & ((value) << LCDCA_ABMDR_DATA_Pos)))
#define LCDCA_ABMDR_DMASK_Pos       8            /**< \brief (LCDCA_ABMDR) Data Mask */
#define LCDCA_ABMDR_DMASK_Msk       (0xFFu << LCDCA_ABMDR_DMASK_Pos)
#define LCDCA_ABMDR_DMASK(value)    ((LCDCA_ABMDR_DMASK_Msk & ((value) << LCDCA_ABMDR_DMASK_Pos)))
#define LCDCA_ABMDR_OFF_Pos         16           /**< \brief (LCDCA_ABMDR) Byte Offset */
#define LCDCA_ABMDR_OFF_Msk         (0x1Fu << LCDCA_ABMDR_OFF_Pos)
#define LCDCA_ABMDR_OFF(value)      ((LCDCA_ABMDR_OFF_Msk & ((value) << LCDCA_ABMDR_OFF_Pos)))
#define LCDCA_ABMDR_MASK            0x001FFFFFu  /**< \brief (LCDCA_ABMDR) MASK Register */

/* -------- LCDCA_IER : (LCDCA Offset: 0x58) ( /W 32) Interrupt Enable Register -------- */
#define LCDCA_IER_OFFSET            0x58         /**< \brief (LCDCA_IER offset) Interrupt Enable Register */
#define LCDCA_IER_RESETVALUE        0x00000000   /**< \brief (LCDCA_IER reset_value) Interrupt Enable Register */

#define LCDCA_IER_FC0R_Pos          0            /**< \brief (LCDCA_IER) Frame Counter 0 Rollover */
#define LCDCA_IER_FC0R              (0x1u << LCDCA_IER_FC0R_Pos)
#define LCDCA_IER_MASK              0x00000001u  /**< \brief (LCDCA_IER) MASK Register */

/* -------- LCDCA_IDR : (LCDCA Offset: 0x5C) ( /W 32) Interrupt Disable Register -------- */
#define LCDCA_IDR_OFFSET            0x5C         /**< \brief (LCDCA_IDR offset) Interrupt Disable Register */
#define LCDCA_IDR_RESETVALUE        0x00000000   /**< \brief (LCDCA_IDR reset_value) Interrupt Disable Register */

#define LCDCA_IDR_FC0R_Pos          0            /**< \brief (LCDCA_IDR) Frame Counter 0 Rollover */
#define LCDCA_IDR_FC0R              (0x1u << LCDCA_IDR_FC0R_Pos)
#define LCDCA_IDR_MASK              0x00000001u  /**< \brief (LCDCA_IDR) MASK Register */

/* -------- LCDCA_IMR : (LCDCA Offset: 0x60) (R/  32) Interrupt Mask Register -------- */
#define LCDCA_IMR_OFFSET            0x60         /**< \brief (LCDCA_IMR offset) Interrupt Mask Register */
#define LCDCA_IMR_RESETVALUE        0x00000000   /**< \brief (LCDCA_IMR reset_value) Interrupt Mask Register */

#define LCDCA_IMR_FC0R_Pos          0            /**< \brief (LCDCA_IMR) Frame Counter 0 Rollover */
#define LCDCA_IMR_FC0R              (0x1u << LCDCA_IMR_FC0R_Pos)
#define LCDCA_IMR_MASK              0x00000001u  /**< \brief (LCDCA_IMR) MASK Register */

/* -------- LCDCA_VERSION : (LCDCA Offset: 0x64) (R/  32) Version Register -------- */
#define LCDCA_VERSION_OFFSET        0x64         /**< \brief (LCDCA_VERSION offset) Version Register */
#define LCDCA_VERSION_RESETVALUE    0x00000100   /**< \brief (LCDCA_VERSION reset_value) Version Register */

#define LCDCA_VERSION_VERSION_Pos   0            /**< \brief (LCDCA_VERSION) Version Number */
#define LCDCA_VERSION_VERSION_Msk   (0xFFFu << LCDCA_VERSION_VERSION_Pos)
#define LCDCA_VERSION_VERSION(value) ((LCDCA_VERSION_VERSION_Msk & ((value) << LCDCA_VERSION_VERSION_Pos)))
#define LCDCA_VERSION_VARIANT_Pos   16           /**< \brief (LCDCA_VERSION) Variant Number */
#define LCDCA_VERSION_VARIANT_Msk   (0xFu << LCDCA_VERSION_VARIANT_Pos)
#define LCDCA_VERSION_VARIANT(value) ((LCDCA_VERSION_VARIANT_Msk & ((value) << LCDCA_VERSION_VARIANT_Pos)))
#define LCDCA_VERSION_MASK          0x000F0FFFu  /**< \brief (LCDCA_VERSION) MASK Register */

/** \brief LCDCA hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
  WoReg   LCDCA_CR;           /**< \brief (LCDCA Offset: 0x00) Control Register */
  RwReg   LCDCA_CFG;          /**< \brief (LCDCA Offset: 0x04) Configuration Register */
  RwReg   LCDCA_TIM;          /**< \brief (LCDCA Offset: 0x08) Timing Register */
  RoReg   LCDCA_SR;           /**< \brief (LCDCA Offset: 0x0C) Status Register */
  WoReg   LCDCA_SCR;          /**< \brief (LCDCA Offset: 0x10) Status Clear Register */
  RwReg   LCDCA_DRL0;         /**< \brief (LCDCA Offset: 0x14) Data Register Low 0 */
  RwReg   LCDCA_DRH0;         /**< \brief (LCDCA Offset: 0x18) Data Register High 0 */
  RwReg   LCDCA_DRL1;         /**< \brief (LCDCA Offset: 0x1C) Data Register Low 1 */
  RwReg   LCDCA_DRH1;         /**< \brief (LCDCA Offset: 0x20) Data Register High 1 */
  RwReg   LCDCA_DRL2;         /**< \brief (LCDCA Offset: 0x24) Data Register Low 2 */
  RwReg   LCDCA_DRH2;         /**< \brief (LCDCA Offset: 0x28) Data Register High 2 */
  RwReg   LCDCA_DRL3;         /**< \brief (LCDCA Offset: 0x2C) Data Register Low 3 */
  RwReg   LCDCA_DRH3;         /**< \brief (LCDCA Offset: 0x30) Data Register High 3 */
  WoReg   LCDCA_IADR;         /**< \brief (LCDCA Offset: 0x34) Indirect Access Data Register */
  RwReg   LCDCA_BCFG;         /**< \brief (LCDCA Offset: 0x38) Blink Configuration Register */
  RwReg   LCDCA_CSRCFG;       /**< \brief (LCDCA Offset: 0x3C) Circular Shift Register Configuration */
  RwReg   LCDCA_CMCFG;        /**< \brief (LCDCA Offset: 0x40) Character Mapping Configuration Register */
  WoReg   LCDCA_CMDR;         /**< \brief (LCDCA Offset: 0x44) Character Mapping Data Register */
  RwReg   LCDCA_ACMCFG;       /**< \brief (LCDCA Offset: 0x48) Automated Character Mapping Configuration Register */
  WoReg   LCDCA_ACMDR;        /**< \brief (LCDCA Offset: 0x4C) Automated Character Mapping Data Register */
  RwReg   LCDCA_ABMCFG;       /**< \brief (LCDCA Offset: 0x50) Automated Bit Mapping Configuration Register */
  WoReg   LCDCA_ABMDR;        /**< \brief (LCDCA Offset: 0x54) Automated Bit Mapping Data Register */
  WoReg   LCDCA_IER;          /**< \brief (LCDCA Offset: 0x58) Interrupt Enable Register */
  WoReg   LCDCA_IDR;          /**< \brief (LCDCA Offset: 0x5C) Interrupt Disable Register */
  RoReg   LCDCA_IMR;          /**< \brief (LCDCA Offset: 0x60) Interrupt Mask Register */
  RoReg   LCDCA_VERSION;      /**< \brief (LCDCA Offset: 0x64) Version Register */
} Lcdca;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/*@}*/

#endif /* _SAM4L_LCDCA_COMPONENT_ */
