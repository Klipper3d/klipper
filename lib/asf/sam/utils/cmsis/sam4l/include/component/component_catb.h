/**
 * \file
 *
 * \brief Component description for CATB
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

#ifndef _SAM4L_CATB_COMPONENT_
#define _SAM4L_CATB_COMPONENT_

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR CATB */
/* ========================================================================== */
/** \addtogroup SAM4L_CATB Capacitive Touch Module B */
/*@{*/

#define REV_CATB                    0x100

/* -------- CATB_CR : (CATB Offset: 0x00) (R/W 32) Control Register -------- */
#define CATB_CR_OFFSET              0x00         /**< \brief (CATB_CR offset) Control Register */
#define CATB_CR_RESETVALUE          0x00000000   /**< \brief (CATB_CR reset_value) Control Register */

#define CATB_CR_EN_Pos              0            /**< \brief (CATB_CR) Module Enable */
#define CATB_CR_EN                  (0x1u << CATB_CR_EN_Pos)
#define CATB_CR_RUN_Pos             1            /**< \brief (CATB_CR) Start Operation */
#define CATB_CR_RUN                 (0x1u << CATB_CR_RUN_Pos)
#define CATB_CR_IIDLE_Pos           2            /**< \brief (CATB_CR) Initialize Idle Value */
#define CATB_CR_IIDLE               (0x1u << CATB_CR_IIDLE_Pos)
#define CATB_CR_ETRIG_Pos           3            /**< \brief (CATB_CR) Event Triggered Operation */
#define CATB_CR_ETRIG               (0x1u << CATB_CR_ETRIG_Pos)
#define CATB_CR_INTRES_Pos          4            /**< \brief (CATB_CR) Internal Resistors */
#define CATB_CR_INTRES              (0x1u << CATB_CR_INTRES_Pos)
#define CATB_CR_CKSEL_Pos           5            /**< \brief (CATB_CR) Clock Select */
#define CATB_CR_CKSEL               (0x1u << CATB_CR_CKSEL_Pos)
#define CATB_CR_DIFF_Pos            6            /**< \brief (CATB_CR) Differential Mode */
#define CATB_CR_DIFF                (0x1u << CATB_CR_DIFF_Pos)
#define CATB_CR_DMAEN_Pos           7            /**< \brief (CATB_CR) DMA Enable */
#define CATB_CR_DMAEN               (0x1u << CATB_CR_DMAEN_Pos)
#define CATB_CR_ESAMPLES_Pos        8            /**< \brief (CATB_CR) Number of Event Samples */
#define CATB_CR_ESAMPLES_Msk        (0x7Fu << CATB_CR_ESAMPLES_Pos)
#define CATB_CR_ESAMPLES(value)     ((CATB_CR_ESAMPLES_Msk & ((value) << CATB_CR_ESAMPLES_Pos)))
#define CATB_CR_CHARGET_Pos         16           /**< \brief (CATB_CR) Charge Time */
#define CATB_CR_CHARGET_Msk         (0xFu << CATB_CR_CHARGET_Pos)
#define CATB_CR_CHARGET(value)      ((CATB_CR_CHARGET_Msk & ((value) << CATB_CR_CHARGET_Pos)))
#define CATB_CR_SWRST_Pos           31           /**< \brief (CATB_CR) Software Reset */
#define CATB_CR_SWRST               (0x1u << CATB_CR_SWRST_Pos)
#define CATB_CR_MASK                0x800F7FFFu  /**< \brief (CATB_CR) MASK Register */

/* -------- CATB_CNTCR : (CATB Offset: 0x04) (R/W 32) Counter Control Register -------- */
#define CATB_CNTCR_OFFSET           0x04         /**< \brief (CATB_CNTCR offset) Counter Control Register */
#define CATB_CNTCR_RESETVALUE       0x00000000   /**< \brief (CATB_CNTCR reset_value) Counter Control Register */

#define CATB_CNTCR_TOP_Pos          0            /**< \brief (CATB_CNTCR) Counter Top Value */
#define CATB_CNTCR_TOP_Msk          (0xFFFFFFu << CATB_CNTCR_TOP_Pos)
#define CATB_CNTCR_TOP(value)       ((CATB_CNTCR_TOP_Msk & ((value) << CATB_CNTCR_TOP_Pos)))
#define CATB_CNTCR_SPREAD_Pos       24           /**< \brief (CATB_CNTCR) Spread Spectrum */
#define CATB_CNTCR_SPREAD_Msk       (0xFu << CATB_CNTCR_SPREAD_Pos)
#define CATB_CNTCR_SPREAD(value)    ((CATB_CNTCR_SPREAD_Msk & ((value) << CATB_CNTCR_SPREAD_Pos)))
#define CATB_CNTCR_REPEAT_Pos       28           /**< \brief (CATB_CNTCR) Repeat Measurements */
#define CATB_CNTCR_REPEAT_Msk       (0x7u << CATB_CNTCR_REPEAT_Pos)
#define CATB_CNTCR_REPEAT(value)    ((CATB_CNTCR_REPEAT_Msk & ((value) << CATB_CNTCR_REPEAT_Pos)))
#define CATB_CNTCR_MASK             0x7FFFFFFFu  /**< \brief (CATB_CNTCR) MASK Register */

/* -------- CATB_IDLE : (CATB Offset: 0x08) (R/W 32) Sensor Idle Level -------- */
#define CATB_IDLE_OFFSET            0x08         /**< \brief (CATB_IDLE offset) Sensor Idle Level */
#define CATB_IDLE_RESETVALUE        0x00000000   /**< \brief (CATB_IDLE reset_value) Sensor Idle Level */

#define CATB_IDLE_FIDLE_Pos         0            /**< \brief (CATB_IDLE) Fractional Sensor Idle */
#define CATB_IDLE_FIDLE_Msk         (0xFFFu << CATB_IDLE_FIDLE_Pos)
#define CATB_IDLE_FIDLE(value)      ((CATB_IDLE_FIDLE_Msk & ((value) << CATB_IDLE_FIDLE_Pos)))
#define CATB_IDLE_RIDLE_Pos         12           /**< \brief (CATB_IDLE) Integer Sensor Idle */
#define CATB_IDLE_RIDLE_Msk         (0xFFFFu << CATB_IDLE_RIDLE_Pos)
#define CATB_IDLE_RIDLE(value)      ((CATB_IDLE_RIDLE_Msk & ((value) << CATB_IDLE_RIDLE_Pos)))
#define CATB_IDLE_MASK              0x0FFFFFFFu  /**< \brief (CATB_IDLE) MASK Register */

/* -------- CATB_LEVEL : (CATB Offset: 0x0C) (R/  32) Sensor Relative Level -------- */
#define CATB_LEVEL_OFFSET           0x0C         /**< \brief (CATB_LEVEL offset) Sensor Relative Level */
#define CATB_LEVEL_RESETVALUE       0x00000000   /**< \brief (CATB_LEVEL reset_value) Sensor Relative Level */

#define CATB_LEVEL_FLEVEL_Pos       0            /**< \brief (CATB_LEVEL) Fractional Sensor Level */
#define CATB_LEVEL_FLEVEL_Msk       (0xFFFu << CATB_LEVEL_FLEVEL_Pos)
#define CATB_LEVEL_FLEVEL(value)    ((CATB_LEVEL_FLEVEL_Msk & ((value) << CATB_LEVEL_FLEVEL_Pos)))
#define CATB_LEVEL_RLEVEL_Pos       12           /**< \brief (CATB_LEVEL) Integer Sensor Level */
#define CATB_LEVEL_RLEVEL_Msk       (0xFFu << CATB_LEVEL_RLEVEL_Pos)
#define CATB_LEVEL_RLEVEL(value)    ((CATB_LEVEL_RLEVEL_Msk & ((value) << CATB_LEVEL_RLEVEL_Pos)))
#define CATB_LEVEL_MASK             0x000FFFFFu  /**< \brief (CATB_LEVEL) MASK Register */

/* -------- CATB_RAW : (CATB Offset: 0x10) (R/  32) Sensor Raw Value -------- */
#define CATB_RAW_OFFSET             0x10         /**< \brief (CATB_RAW offset) Sensor Raw Value */
#define CATB_RAW_RESETVALUE         0x00000000   /**< \brief (CATB_RAW reset_value) Sensor Raw Value */

#define CATB_RAW_RAWA_Pos           16           /**< \brief (CATB_RAW) Current Sensor Raw Value */
#define CATB_RAW_RAWA_Msk           (0xFFu << CATB_RAW_RAWA_Pos)
#define CATB_RAW_RAWA(value)        ((CATB_RAW_RAWA_Msk & ((value) << CATB_RAW_RAWA_Pos)))
#define CATB_RAW_RAWB_Pos           24           /**< \brief (CATB_RAW) Last Sensor Raw Value */
#define CATB_RAW_RAWB_Msk           (0xFFu << CATB_RAW_RAWB_Pos)
#define CATB_RAW_RAWB(value)        ((CATB_RAW_RAWB_Msk & ((value) << CATB_RAW_RAWB_Pos)))
#define CATB_RAW_MASK               0xFFFF0000u  /**< \brief (CATB_RAW) MASK Register */

/* -------- CATB_TIMING : (CATB Offset: 0x14) (R/W 32) Filter Timing Register -------- */
#define CATB_TIMING_OFFSET          0x14         /**< \brief (CATB_TIMING offset) Filter Timing Register */
#define CATB_TIMING_RESETVALUE      0x00000000   /**< \brief (CATB_TIMING reset_value) Filter Timing Register */

#define CATB_TIMING_TLEVEL_Pos      0            /**< \brief (CATB_TIMING) Relative Level Smoothing */
#define CATB_TIMING_TLEVEL_Msk      (0xFFFu << CATB_TIMING_TLEVEL_Pos)
#define CATB_TIMING_TLEVEL(value)   ((CATB_TIMING_TLEVEL_Msk & ((value) << CATB_TIMING_TLEVEL_Pos)))
#define CATB_TIMING_TIDLE_Pos       16           /**< \brief (CATB_TIMING) Idle Smoothening */
#define CATB_TIMING_TIDLE_Msk       (0xFFFu << CATB_TIMING_TIDLE_Pos)
#define CATB_TIMING_TIDLE(value)    ((CATB_TIMING_TIDLE_Msk & ((value) << CATB_TIMING_TIDLE_Pos)))
#define CATB_TIMING_MASK            0x0FFF0FFFu  /**< \brief (CATB_TIMING) MASK Register */

/* -------- CATB_THRESH : (CATB Offset: 0x18) (R/W 32) Threshold Register -------- */
#define CATB_THRESH_OFFSET          0x18         /**< \brief (CATB_THRESH offset) Threshold Register */
#define CATB_THRESH_RESETVALUE      0x00000000   /**< \brief (CATB_THRESH reset_value) Threshold Register */

#define CATB_THRESH_FTHRESH_Pos     0            /**< \brief (CATB_THRESH) Fractional part of Threshold Value */
#define CATB_THRESH_FTHRESH_Msk     (0xFFFu << CATB_THRESH_FTHRESH_Pos)
#define CATB_THRESH_FTHRESH(value)  ((CATB_THRESH_FTHRESH_Msk & ((value) << CATB_THRESH_FTHRESH_Pos)))
#define CATB_THRESH_RTHRESH_Pos     12           /**< \brief (CATB_THRESH) Rational part of Threshold Value */
#define CATB_THRESH_RTHRESH_Msk     (0xFFu << CATB_THRESH_RTHRESH_Pos)
#define CATB_THRESH_RTHRESH(value)  ((CATB_THRESH_RTHRESH_Msk & ((value) << CATB_THRESH_RTHRESH_Pos)))
#define CATB_THRESH_DIR_Pos         23           /**< \brief (CATB_THRESH) Threshold Direction */
#define CATB_THRESH_DIR             (0x1u << CATB_THRESH_DIR_Pos)
#define CATB_THRESH_LENGTH_Pos      24           /**< \brief (CATB_THRESH) Threshold Length */
#define CATB_THRESH_LENGTH_Msk      (0x1Fu << CATB_THRESH_LENGTH_Pos)
#define CATB_THRESH_LENGTH(value)   ((CATB_THRESH_LENGTH_Msk & ((value) << CATB_THRESH_LENGTH_Pos)))
#define CATB_THRESH_MASK            0x1F8FFFFFu  /**< \brief (CATB_THRESH) MASK Register */

/* -------- CATB_PINSEL : (CATB Offset: 0x1C) (R/W 32) Pin Selection Register -------- */
#define CATB_PINSEL_OFFSET          0x1C         /**< \brief (CATB_PINSEL offset) Pin Selection Register */
#define CATB_PINSEL_RESETVALUE      0x00000000   /**< \brief (CATB_PINSEL reset_value) Pin Selection Register */

#define CATB_PINSEL_PINSEL_Pos      0            /**< \brief (CATB_PINSEL) Pin Select */
#define CATB_PINSEL_PINSEL_Msk      (0xFFu << CATB_PINSEL_PINSEL_Pos)
#define CATB_PINSEL_PINSEL(value)   ((CATB_PINSEL_PINSEL_Msk & ((value) << CATB_PINSEL_PINSEL_Pos)))
#define CATB_PINSEL_MASK            0x000000FFu  /**< \brief (CATB_PINSEL) MASK Register */

/* -------- CATB_DMA : (CATB Offset: 0x20) (R/W 32) Direct Memory Access Register -------- */
#define CATB_DMA_OFFSET             0x20         /**< \brief (CATB_DMA offset) Direct Memory Access Register */
#define CATB_DMA_RESETVALUE         0x00000000   /**< \brief (CATB_DMA reset_value) Direct Memory Access Register */

#define CATB_DMA_DMA_Pos            0            /**< \brief (CATB_DMA) Direct Memory Access */
#define CATB_DMA_DMA_Msk            (0xFFFFFFFFu << CATB_DMA_DMA_Pos)
#define CATB_DMA_DMA(value)         ((CATB_DMA_DMA_Msk & ((value) << CATB_DMA_DMA_Pos)))
#define CATB_DMA_MASK               0xFFFFFFFFu  /**< \brief (CATB_DMA) MASK Register */

/* -------- CATB_ISR : (CATB Offset: 0x24) (R/  32) Interrupt Status Register -------- */
#define CATB_ISR_OFFSET             0x24         /**< \brief (CATB_ISR offset) Interrupt Status Register */
#define CATB_ISR_RESETVALUE         0x00000000   /**< \brief (CATB_ISR reset_value) Interrupt Status Register */

#define CATB_ISR_SAMPLE_Pos         0            /**< \brief (CATB_ISR) Sample Ready Interrupt Status */
#define CATB_ISR_SAMPLE             (0x1u << CATB_ISR_SAMPLE_Pos)
#define CATB_ISR_INTCH_Pos          1            /**< \brief (CATB_ISR) In-touch Interrupt Status */
#define CATB_ISR_INTCH              (0x1u << CATB_ISR_INTCH_Pos)
#define CATB_ISR_OUTTCH_Pos         2            /**< \brief (CATB_ISR) Out-of-Touch Interrupt Status */
#define CATB_ISR_OUTTCH             (0x1u << CATB_ISR_OUTTCH_Pos)
#define CATB_ISR_MASK               0x00000007u  /**< \brief (CATB_ISR) MASK Register */

/* -------- CATB_IER : (CATB Offset: 0x28) ( /W 32) Interrupt Enable Register -------- */
#define CATB_IER_OFFSET             0x28         /**< \brief (CATB_IER offset) Interrupt Enable Register */
#define CATB_IER_RESETVALUE         0x00000000   /**< \brief (CATB_IER reset_value) Interrupt Enable Register */

#define CATB_IER_SAMPLE_Pos         0            /**< \brief (CATB_IER) Sample Ready Interrupt Enable */
#define CATB_IER_SAMPLE             (0x1u << CATB_IER_SAMPLE_Pos)
#define CATB_IER_INTCH_Pos          1            /**< \brief (CATB_IER) In-touch Interrupt Enable */
#define CATB_IER_INTCH              (0x1u << CATB_IER_INTCH_Pos)
#define CATB_IER_OUTTCH_Pos         2            /**< \brief (CATB_IER) Out-of-Touch Interrupt Enable */
#define CATB_IER_OUTTCH             (0x1u << CATB_IER_OUTTCH_Pos)
#define CATB_IER_MASK               0x00000007u  /**< \brief (CATB_IER) MASK Register */

/* -------- CATB_IDR : (CATB Offset: 0x2C) ( /W 32) Interrupt Disable Register -------- */
#define CATB_IDR_OFFSET             0x2C         /**< \brief (CATB_IDR offset) Interrupt Disable Register */
#define CATB_IDR_RESETVALUE         0x00000000   /**< \brief (CATB_IDR reset_value) Interrupt Disable Register */

#define CATB_IDR_SAMPLE_Pos         0            /**< \brief (CATB_IDR) Sample Ready Interrupt Disable */
#define CATB_IDR_SAMPLE             (0x1u << CATB_IDR_SAMPLE_Pos)
#define CATB_IDR_INTCH_Pos          1            /**< \brief (CATB_IDR) In-touch Interrupt Disable */
#define CATB_IDR_INTCH              (0x1u << CATB_IDR_INTCH_Pos)
#define CATB_IDR_OUTTCH_Pos         2            /**< \brief (CATB_IDR) Out-of-Touch Interrupt Disable */
#define CATB_IDR_OUTTCH             (0x1u << CATB_IDR_OUTTCH_Pos)
#define CATB_IDR_MASK               0x00000007u  /**< \brief (CATB_IDR) MASK Register */

/* -------- CATB_IMR : (CATB Offset: 0x30) (R/  32) Interrupt Mask Register -------- */
#define CATB_IMR_OFFSET             0x30         /**< \brief (CATB_IMR offset) Interrupt Mask Register */
#define CATB_IMR_RESETVALUE         0x00000000   /**< \brief (CATB_IMR reset_value) Interrupt Mask Register */

#define CATB_IMR_SAMPLE_Pos         0            /**< \brief (CATB_IMR) Sample Ready Interrupt Mask */
#define CATB_IMR_SAMPLE             (0x1u << CATB_IMR_SAMPLE_Pos)
#define CATB_IMR_INTCH_Pos          1            /**< \brief (CATB_IMR) In-touch Interrupt Mask */
#define CATB_IMR_INTCH              (0x1u << CATB_IMR_INTCH_Pos)
#define CATB_IMR_OUTTCH_Pos         2            /**< \brief (CATB_IMR) Out-of-Touch Interrupt Mask */
#define CATB_IMR_OUTTCH             (0x1u << CATB_IMR_OUTTCH_Pos)
#define CATB_IMR_MASK               0x00000007u  /**< \brief (CATB_IMR) MASK Register */

/* -------- CATB_SCR : (CATB Offset: 0x34) ( /W 32) Status Clear Register -------- */
#define CATB_SCR_OFFSET             0x34         /**< \brief (CATB_SCR offset) Status Clear Register */
#define CATB_SCR_RESETVALUE         0x00000000   /**< \brief (CATB_SCR reset_value) Status Clear Register */

#define CATB_SCR_SAMPLE_Pos         0            /**< \brief (CATB_SCR) Sample Ready */
#define CATB_SCR_SAMPLE             (0x1u << CATB_SCR_SAMPLE_Pos)
#define CATB_SCR_INTCH_Pos          1            /**< \brief (CATB_SCR) In-touch */
#define CATB_SCR_INTCH              (0x1u << CATB_SCR_INTCH_Pos)
#define CATB_SCR_OUTTCH_Pos         2            /**< \brief (CATB_SCR) Out-of-Touch */
#define CATB_SCR_OUTTCH             (0x1u << CATB_SCR_OUTTCH_Pos)
#define CATB_SCR_MASK               0x00000007u  /**< \brief (CATB_SCR) MASK Register */

/* -------- CATB_INTCH : (CATB Offset: 0x40) (R/  32) INTCH In-Touch Status Register -------- */
#define CATB_INTCH_OFFSET           0x40         /**< \brief (CATB_INTCH offset) In-Touch Status Register */
#define CATB_INTCH_RESETVALUE       0x00000000   /**< \brief (CATB_INTCH reset_value) In-Touch Status Register */

#define CATB_INTCH_INTCH_Pos        0            /**< \brief (CATB_INTCH) In-Touch */
#define CATB_INTCH_INTCH_Msk        (0xFFFFFFFFu << CATB_INTCH_INTCH_Pos)
#define CATB_INTCH_INTCH(value)     ((CATB_INTCH_INTCH_Msk & ((value) << CATB_INTCH_INTCH_Pos)))
#define CATB_INTCH_MASK             0xFFFFFFFFu  /**< \brief (CATB_INTCH) MASK Register */

/* -------- CATB_INTCHCLR : (CATB Offset: 0x50) ( /W 32) INTCHCLR In-Touch Status Clear Register -------- */
#define CATB_INTCHCLR_OFFSET        0x50         /**< \brief (CATB_INTCHCLR offset) In-Touch Status Clear Register */
#define CATB_INTCHCLR_RESETVALUE    0x00000000   /**< \brief (CATB_INTCHCLR reset_value) In-Touch Status Clear Register */

#define CATB_INTCHCLR_INTCHCLR_Pos  0            /**< \brief (CATB_INTCHCLR) In-Touch Clear */
#define CATB_INTCHCLR_INTCHCLR_Msk  (0xFFFFFFFFu << CATB_INTCHCLR_INTCHCLR_Pos)
#define CATB_INTCHCLR_INTCHCLR(value) ((CATB_INTCHCLR_INTCHCLR_Msk & ((value) << CATB_INTCHCLR_INTCHCLR_Pos)))
#define CATB_INTCHCLR_MASK          0xFFFFFFFFu  /**< \brief (CATB_INTCHCLR) MASK Register */

/* -------- CATB_OUTTCH : (CATB Offset: 0x60) (R/  32) OUTTCH Out-of-Touch Status Register -------- */
#define CATB_OUTTCH_OFFSET          0x60         /**< \brief (CATB_OUTTCH offset) Out-of-Touch Status Register */
#define CATB_OUTTCH_RESETVALUE      0x00000000   /**< \brief (CATB_OUTTCH reset_value) Out-of-Touch Status Register */

#define CATB_OUTTCH_OUTTCH_Pos      0            /**< \brief (CATB_OUTTCH) Out-of-Touch */
#define CATB_OUTTCH_OUTTCH_Msk      (0xFFFFFFFFu << CATB_OUTTCH_OUTTCH_Pos)
#define CATB_OUTTCH_OUTTCH(value)   ((CATB_OUTTCH_OUTTCH_Msk & ((value) << CATB_OUTTCH_OUTTCH_Pos)))
#define CATB_OUTTCH_MASK            0xFFFFFFFFu  /**< \brief (CATB_OUTTCH) MASK Register */

/* -------- CATB_OUTTCHCLR : (CATB Offset: 0x70) ( /W 32) OUTTCHCLR Out-of-Touch Status Clear Register -------- */
#define CATB_OUTTCHCLR_OFFSET       0x70         /**< \brief (CATB_OUTTCHCLR offset) Out-of-Touch Status Clear Register */
#define CATB_OUTTCHCLR_RESETVALUE   0x00000000   /**< \brief (CATB_OUTTCHCLR reset_value) Out-of-Touch Status Clear Register */

#define CATB_OUTTCHCLR_OUTTCHCLR_Pos 0            /**< \brief (CATB_OUTTCHCLR) Out of Touch */
#define CATB_OUTTCHCLR_OUTTCHCLR_Msk (0xFFFFFFFFu << CATB_OUTTCHCLR_OUTTCHCLR_Pos)
#define CATB_OUTTCHCLR_OUTTCHCLR(value) ((CATB_OUTTCHCLR_OUTTCHCLR_Msk & ((value) << CATB_OUTTCHCLR_OUTTCHCLR_Pos)))
#define CATB_OUTTCHCLR_MASK         0xFFFFFFFFu  /**< \brief (CATB_OUTTCHCLR) MASK Register */

/* -------- CATB_PARAMETER : (CATB Offset: 0xF8) (R/  32) Parameter Register -------- */
#define CATB_PARAMETER_OFFSET       0xF8         /**< \brief (CATB_PARAMETER offset) Parameter Register */

#define CATB_PARAMETER_NPINS_Pos    0            /**< \brief (CATB_PARAMETER) Number of Pins */
#define CATB_PARAMETER_NPINS_Msk    (0xFFu << CATB_PARAMETER_NPINS_Pos)
#define CATB_PARAMETER_NPINS(value) ((CATB_PARAMETER_NPINS_Msk & ((value) << CATB_PARAMETER_NPINS_Pos)))
#define CATB_PARAMETER_NSTATUS_Pos  8            /**< \brief (CATB_PARAMETER) Number of Status bits */
#define CATB_PARAMETER_NSTATUS_Msk  (0xFFu << CATB_PARAMETER_NSTATUS_Pos)
#define CATB_PARAMETER_NSTATUS(value) ((CATB_PARAMETER_NSTATUS_Msk & ((value) << CATB_PARAMETER_NSTATUS_Pos)))
#define CATB_PARAMETER_FRACTIONAL_Pos 16           /**< \brief (CATB_PARAMETER) Number of Fractional bits */
#define CATB_PARAMETER_FRACTIONAL_Msk (0xFu << CATB_PARAMETER_FRACTIONAL_Pos)
#define CATB_PARAMETER_FRACTIONAL(value) ((CATB_PARAMETER_FRACTIONAL_Msk & ((value) << CATB_PARAMETER_FRACTIONAL_Pos)))
#define CATB_PARAMETER_MASK         0x000FFFFFu  /**< \brief (CATB_PARAMETER) MASK Register */

/* -------- CATB_VERSION : (CATB Offset: 0xFC) (R/  32) Version Register -------- */
#define CATB_VERSION_OFFSET         0xFC         /**< \brief (CATB_VERSION offset) Version Register */
#define CATB_VERSION_RESETVALUE     0x00000100   /**< \brief (CATB_VERSION reset_value) Version Register */

#define CATB_VERSION_VERSION_Pos    0            /**< \brief (CATB_VERSION) Version number */
#define CATB_VERSION_VERSION_Msk    (0xFFFu << CATB_VERSION_VERSION_Pos)
#define CATB_VERSION_VERSION(value) ((CATB_VERSION_VERSION_Msk & ((value) << CATB_VERSION_VERSION_Pos)))
#define CATB_VERSION_VARIANT_Pos    16           /**< \brief (CATB_VERSION) Variant number */
#define CATB_VERSION_VARIANT_Msk    (0xFu << CATB_VERSION_VARIANT_Pos)
#define CATB_VERSION_VARIANT(value) ((CATB_VERSION_VARIANT_Msk & ((value) << CATB_VERSION_VARIANT_Pos)))
#define CATB_VERSION_MASK           0x000F0FFFu  /**< \brief (CATB_VERSION) MASK Register */

/** \brief CatbIntch hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
  RoReg   CATB_INTCH;         /**< \brief (CATB Offset: 0x00) In-Touch Status Register */
} CatbIntch;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/** \brief CatbIntchclr hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
  WoReg   CATB_INTCHCLR;      /**< \brief (CATB Offset: 0x00) In-Touch Status Clear Register */
} CatbIntchclr;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/** \brief CatbOuttch hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
  RoReg   CATB_OUTTCH;        /**< \brief (CATB Offset: 0x00) Out-of-Touch Status Register */
} CatbOuttch;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/** \brief CatbOuttchclr hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
  WoReg   CATB_OUTTCHCLR;     /**< \brief (CATB Offset: 0x00) Out-of-Touch Status Clear Register */
} CatbOuttchclr;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/** \brief CATB hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
  RwReg   CATB_CR;            /**< \brief (CATB Offset: 0x00) Control Register */
  RwReg   CATB_CNTCR;         /**< \brief (CATB Offset: 0x04) Counter Control Register */
  RwReg   CATB_IDLE;          /**< \brief (CATB Offset: 0x08) Sensor Idle Level */
  RoReg   CATB_LEVEL;         /**< \brief (CATB Offset: 0x0C) Sensor Relative Level */
  RoReg   CATB_RAW;           /**< \brief (CATB Offset: 0x10) Sensor Raw Value */
  RwReg   CATB_TIMING;        /**< \brief (CATB Offset: 0x14) Filter Timing Register */
  RwReg   CATB_THRESH;        /**< \brief (CATB Offset: 0x18) Threshold Register */
  RwReg   CATB_PINSEL;        /**< \brief (CATB Offset: 0x1C) Pin Selection Register */
  RwReg   CATB_DMA;           /**< \brief (CATB Offset: 0x20) Direct Memory Access Register */
  RoReg   CATB_ISR;           /**< \brief (CATB Offset: 0x24) Interrupt Status Register */
  WoReg   CATB_IER;           /**< \brief (CATB Offset: 0x28) Interrupt Enable Register */
  WoReg   CATB_IDR;           /**< \brief (CATB Offset: 0x2C) Interrupt Disable Register */
  RoReg   CATB_IMR;           /**< \brief (CATB Offset: 0x30) Interrupt Mask Register */
  WoReg   CATB_SCR;           /**< \brief (CATB Offset: 0x34) Status Clear Register */
  RoReg8  Reserved1[0x8];
  CatbIntch CATB_INTCH[1];      /**< \brief (CATB Offset: 0x40) CatbIntch groups [STATUS_REG_NUMBER] */
  RoReg8  Reserved2[0xC];
  CatbIntchclr CATB_INTCHCLR[1];   /**< \brief (CATB Offset: 0x50) CatbIntchclr groups [STATUS_REG_NUMBER] */
  RoReg8  Reserved3[0xC];
  CatbOuttch CATB_OUTTCH[1];     /**< \brief (CATB Offset: 0x60) CatbOuttch groups [STATUS_REG_NUMBER] */
  RoReg8  Reserved4[0xC];
  CatbOuttchclr CATB_OUTTCHCLR[1];  /**< \brief (CATB Offset: 0x70) CatbOuttchclr groups [STATUS_REG_NUMBER] */
  RoReg8  Reserved5[0x84];
  RoReg   CATB_PARAMETER;     /**< \brief (CATB Offset: 0xF8) Parameter Register */
  RoReg   CATB_VERSION;       /**< \brief (CATB Offset: 0xFC) Version Register */
} Catb;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/*@}*/

#endif /* _SAM4L_CATB_COMPONENT_ */
