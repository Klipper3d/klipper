/**
 * \file
 *
 * \brief Component description for ADCIFE
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

#ifndef _SAM4L_ADCIFE_COMPONENT_
#define _SAM4L_ADCIFE_COMPONENT_

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR ADCIFE */
/* ========================================================================== */
/** \addtogroup SAM4L_ADCIFE ADC controller interface */
/*@{*/

#define REV_ADCIFE                  0x100

/* -------- ADCIFE_CR : (ADCIFE Offset: 0x00) ( /W 32) Control Register -------- */
#define ADCIFE_CR_OFFSET            0x00         /**< \brief (ADCIFE_CR offset) Control Register */
#define ADCIFE_CR_RESETVALUE        0x00000000   /**< \brief (ADCIFE_CR reset_value) Control Register */

#define ADCIFE_CR_SWRST_Pos         0            /**< \brief (ADCIFE_CR) Software reset */
#define ADCIFE_CR_SWRST             (0x1u << ADCIFE_CR_SWRST_Pos)
#define ADCIFE_CR_TSTOP_Pos         1            /**< \brief (ADCIFE_CR) Internal timer stop bit */
#define ADCIFE_CR_TSTOP             (0x1u << ADCIFE_CR_TSTOP_Pos)
#define ADCIFE_CR_TSTART_Pos        2            /**< \brief (ADCIFE_CR) Internal timer start bit */
#define ADCIFE_CR_TSTART            (0x1u << ADCIFE_CR_TSTART_Pos)
#define ADCIFE_CR_STRIG_Pos         3            /**< \brief (ADCIFE_CR) Sequencer trigger */
#define ADCIFE_CR_STRIG             (0x1u << ADCIFE_CR_STRIG_Pos)
#define ADCIFE_CR_REFBUFEN_Pos      4            /**< \brief (ADCIFE_CR) Reference buffer enable */
#define ADCIFE_CR_REFBUFEN          (0x1u << ADCIFE_CR_REFBUFEN_Pos)
#define ADCIFE_CR_REFBUFDIS_Pos     5            /**< \brief (ADCIFE_CR) Reference buffer disable */
#define ADCIFE_CR_REFBUFDIS         (0x1u << ADCIFE_CR_REFBUFDIS_Pos)
#define ADCIFE_CR_EN_Pos            8            /**< \brief (ADCIFE_CR) ADCIFD enable */
#define ADCIFE_CR_EN                (0x1u << ADCIFE_CR_EN_Pos)
#define ADCIFE_CR_DIS_Pos           9            /**< \brief (ADCIFE_CR) ADCIFD disable */
#define ADCIFE_CR_DIS               (0x1u << ADCIFE_CR_DIS_Pos)
#define ADCIFE_CR_BGREQEN_Pos       10           /**< \brief (ADCIFE_CR) Bandgap buffer request enable */
#define ADCIFE_CR_BGREQEN           (0x1u << ADCIFE_CR_BGREQEN_Pos)
#define ADCIFE_CR_BGREQDIS_Pos      11           /**< \brief (ADCIFE_CR) Bandgap buffer request disable */
#define ADCIFE_CR_BGREQDIS          (0x1u << ADCIFE_CR_BGREQDIS_Pos)
#define ADCIFE_CR_MASK              0x00000F3Fu  /**< \brief (ADCIFE_CR) MASK Register */

/* -------- ADCIFE_CFG : (ADCIFE Offset: 0x04) (R/W 32) Configuration Register -------- */
#define ADCIFE_CFG_OFFSET           0x04         /**< \brief (ADCIFE_CFG offset) Configuration Register */
#define ADCIFE_CFG_RESETVALUE       0x00000000   /**< \brief (ADCIFE_CFG reset_value) Configuration Register */

#define ADCIFE_CFG_REFSEL_Pos       1            /**< \brief (ADCIFE_CFG) ADC Reference Selection */
#define ADCIFE_CFG_REFSEL_Msk       (0x7u << ADCIFE_CFG_REFSEL_Pos)
#define ADCIFE_CFG_REFSEL(value)    ((ADCIFE_CFG_REFSEL_Msk & ((value) << ADCIFE_CFG_REFSEL_Pos)))
#define ADCIFE_CFG_SPEED_Pos        4            /**< \brief (ADCIFE_CFG) ADC current reduction */
#define ADCIFE_CFG_SPEED_Msk        (0x3u << ADCIFE_CFG_SPEED_Pos)
#define ADCIFE_CFG_SPEED(value)     ((ADCIFE_CFG_SPEED_Msk & ((value) << ADCIFE_CFG_SPEED_Pos)))
#define ADCIFE_CFG_CLKSEL_Pos       6            /**< \brief (ADCIFE_CFG) Clock Selection for sequencer/ADC cell */
#define ADCIFE_CFG_CLKSEL           (0x1u << ADCIFE_CFG_CLKSEL_Pos)
#define ADCIFE_CFG_PRESCAL_Pos      8            /**< \brief (ADCIFE_CFG) Prescaler Rate Selection */
#define ADCIFE_CFG_PRESCAL_Msk      (0x7u << ADCIFE_CFG_PRESCAL_Pos)
#define ADCIFE_CFG_PRESCAL(value)   ((ADCIFE_CFG_PRESCAL_Msk & ((value) << ADCIFE_CFG_PRESCAL_Pos)))
#define ADCIFE_CFG_MASK             0x0000077Eu  /**< \brief (ADCIFE_CFG) MASK Register */

/* -------- ADCIFE_SR : (ADCIFE Offset: 0x08) (R/  32) Status Register -------- */
#define ADCIFE_SR_OFFSET            0x08         /**< \brief (ADCIFE_SR offset) Status Register */
#define ADCIFE_SR_RESETVALUE        0x00000000   /**< \brief (ADCIFE_SR reset_value) Status Register */

#define ADCIFE_SR_SEOC_Pos          0            /**< \brief (ADCIFE_SR) Sequencer end of conversion */
#define ADCIFE_SR_SEOC              (0x1u << ADCIFE_SR_SEOC_Pos)
#define ADCIFE_SR_LOVR_Pos          1            /**< \brief (ADCIFE_SR) Sequencer last converted value overrun */
#define ADCIFE_SR_LOVR              (0x1u << ADCIFE_SR_LOVR_Pos)
#define ADCIFE_SR_WM_Pos            2            /**< \brief (ADCIFE_SR) Window monitor */
#define ADCIFE_SR_WM                (0x1u << ADCIFE_SR_WM_Pos)
#define ADCIFE_SR_SMTRG_Pos         3            /**< \brief (ADCIFE_SR) Sequencer missed trigger event */
#define ADCIFE_SR_SMTRG             (0x1u << ADCIFE_SR_SMTRG_Pos)
#define ADCIFE_SR_SUTD_Pos          4            /**< \brief (ADCIFE_SR) Start-up time done */
#define ADCIFE_SR_SUTD              (0x1u << ADCIFE_SR_SUTD_Pos)
#define ADCIFE_SR_TTO_Pos           5            /**< \brief (ADCIFE_SR) Timer time-out */
#define ADCIFE_SR_TTO               (0x1u << ADCIFE_SR_TTO_Pos)
#define ADCIFE_SR_EN_Pos            24           /**< \brief (ADCIFE_SR) Enable Status */
#define ADCIFE_SR_EN                (0x1u << ADCIFE_SR_EN_Pos)
#define ADCIFE_SR_TBUSY_Pos         25           /**< \brief (ADCIFE_SR) Timer busy */
#define ADCIFE_SR_TBUSY             (0x1u << ADCIFE_SR_TBUSY_Pos)
#define ADCIFE_SR_SBUSY_Pos         26           /**< \brief (ADCIFE_SR) Sequencer busy */
#define ADCIFE_SR_SBUSY             (0x1u << ADCIFE_SR_SBUSY_Pos)
#define ADCIFE_SR_CBUSY_Pos         27           /**< \brief (ADCIFE_SR) Conversion busy */
#define ADCIFE_SR_CBUSY             (0x1u << ADCIFE_SR_CBUSY_Pos)
#define ADCIFE_SR_REFBUF_Pos        28           /**< \brief (ADCIFE_SR) Reference buffer status */
#define ADCIFE_SR_REFBUF            (0x1u << ADCIFE_SR_REFBUF_Pos)
#define ADCIFE_SR_MASK              0x1F00003Fu  /**< \brief (ADCIFE_SR) MASK Register */

/* -------- ADCIFE_SCR : (ADCIFE Offset: 0x0C) ( /W 32) Status Clear Register -------- */
#define ADCIFE_SCR_OFFSET           0x0C         /**< \brief (ADCIFE_SCR offset) Status Clear Register */
#define ADCIFE_SCR_RESETVALUE       0x00000000   /**< \brief (ADCIFE_SCR reset_value) Status Clear Register */

#define ADCIFE_SCR_SEOC_Pos         0            /**< \brief (ADCIFE_SCR) Sequencer end of conversion */
#define ADCIFE_SCR_SEOC             (0x1u << ADCIFE_SCR_SEOC_Pos)
#define ADCIFE_SCR_LOVR_Pos         1            /**< \brief (ADCIFE_SCR) Sequencer last converted value overrun */
#define ADCIFE_SCR_LOVR             (0x1u << ADCIFE_SCR_LOVR_Pos)
#define ADCIFE_SCR_WM_Pos           2            /**< \brief (ADCIFE_SCR) Window monitor */
#define ADCIFE_SCR_WM               (0x1u << ADCIFE_SCR_WM_Pos)
#define ADCIFE_SCR_SMTRG_Pos        3            /**< \brief (ADCIFE_SCR) Sequencer missed trigger event */
#define ADCIFE_SCR_SMTRG            (0x1u << ADCIFE_SCR_SMTRG_Pos)
#define ADCIFE_SCR_SUTD_Pos         4            /**< \brief (ADCIFE_SCR) Start-up time done */
#define ADCIFE_SCR_SUTD             (0x1u << ADCIFE_SCR_SUTD_Pos)
#define ADCIFE_SCR_TTO_Pos          5            /**< \brief (ADCIFE_SCR) Timer time-out */
#define ADCIFE_SCR_TTO              (0x1u << ADCIFE_SCR_TTO_Pos)
#define ADCIFE_SCR_MASK             0x0000003Fu  /**< \brief (ADCIFE_SCR) MASK Register */

/* -------- ADCIFE_RTS : (ADCIFE Offset: 0x10) (R/W 32) Resistive Touch Screen Register -------- */
#define ADCIFE_RTS_OFFSET           0x10         /**< \brief (ADCIFE_RTS offset) Resistive Touch Screen Register */
#define ADCIFE_RTS_RESETVALUE       0x00000000   /**< \brief (ADCIFE_RTS reset_value) Resistive Touch Screen Register */

#define ADCIFE_RTS_MASK             0x00000000u  /**< \brief (ADCIFE_RTS) MASK Register */

/* -------- ADCIFE_SEQCFG : (ADCIFE Offset: 0x14) (R/W 32) Sequencer Configuration Register -------- */
#define ADCIFE_SEQCFG_OFFSET        0x14         /**< \brief (ADCIFE_SEQCFG offset) Sequencer Configuration Register */
#define ADCIFE_SEQCFG_RESETVALUE    0x00000000   /**< \brief (ADCIFE_SEQCFG reset_value) Sequencer Configuration Register */

#define ADCIFE_SEQCFG_HWLA_Pos      0            /**< \brief (ADCIFE_SEQCFG) Half word left adjust */
#define ADCIFE_SEQCFG_HWLA          (0x1u << ADCIFE_SEQCFG_HWLA_Pos)
#define ADCIFE_SEQCFG_BIPOLAR_Pos   2            /**< \brief (ADCIFE_SEQCFG) Bipolar Mode */
#define ADCIFE_SEQCFG_BIPOLAR       (0x1u << ADCIFE_SEQCFG_BIPOLAR_Pos)
#define ADCIFE_SEQCFG_GAIN_Pos      4            /**< \brief (ADCIFE_SEQCFG) Gain factor */
#define ADCIFE_SEQCFG_GAIN_Msk      (0x7u << ADCIFE_SEQCFG_GAIN_Pos)
#define ADCIFE_SEQCFG_GAIN(value)   ((ADCIFE_SEQCFG_GAIN_Msk & ((value) << ADCIFE_SEQCFG_GAIN_Pos)))
#define ADCIFE_SEQCFG_GCOMP_Pos     7            /**< \brief (ADCIFE_SEQCFG) Gain Compensation */
#define ADCIFE_SEQCFG_GCOMP         (0x1u << ADCIFE_SEQCFG_GCOMP_Pos)
#define ADCIFE_SEQCFG_TRGSEL_Pos    8            /**< \brief (ADCIFE_SEQCFG) Trigger selection */
#define ADCIFE_SEQCFG_TRGSEL_Msk    (0x7u << ADCIFE_SEQCFG_TRGSEL_Pos)
#define ADCIFE_SEQCFG_TRGSEL(value) ((ADCIFE_SEQCFG_TRGSEL_Msk & ((value) << ADCIFE_SEQCFG_TRGSEL_Pos)))
#define ADCIFE_SEQCFG_RES_Pos       12           /**< \brief (ADCIFE_SEQCFG) Resolution */
#define ADCIFE_SEQCFG_RES           (0x1u << ADCIFE_SEQCFG_RES_Pos)
#define ADCIFE_SEQCFG_INTERNAL_Pos  14           /**< \brief (ADCIFE_SEQCFG) Internal Voltage Source Selection */
#define ADCIFE_SEQCFG_INTERNAL_Msk  (0x3u << ADCIFE_SEQCFG_INTERNAL_Pos)
#define ADCIFE_SEQCFG_INTERNAL(value) ((ADCIFE_SEQCFG_INTERNAL_Msk & ((value) << ADCIFE_SEQCFG_INTERNAL_Pos)))
#define ADCIFE_SEQCFG_MUXPOS_Pos    16           /**< \brief (ADCIFE_SEQCFG) MUX selection on Positive ADC input channel */
#define ADCIFE_SEQCFG_MUXPOS_Msk    (0xFu << ADCIFE_SEQCFG_MUXPOS_Pos)
#define ADCIFE_SEQCFG_MUXPOS(value) ((ADCIFE_SEQCFG_MUXPOS_Msk & ((value) << ADCIFE_SEQCFG_MUXPOS_Pos)))
#define ADCIFE_SEQCFG_MUXNEG_Pos    20           /**< \brief (ADCIFE_SEQCFG) MUX selection on Negative ADC input channel */
#define ADCIFE_SEQCFG_MUXNEG_Msk    (0x7u << ADCIFE_SEQCFG_MUXNEG_Pos)
#define ADCIFE_SEQCFG_MUXNEG(value) ((ADCIFE_SEQCFG_MUXNEG_Msk & ((value) << ADCIFE_SEQCFG_MUXNEG_Pos)))
#define ADCIFE_SEQCFG_ZOOMRANGE_Pos 28           /**< \brief (ADCIFE_SEQCFG) Zoom shift/unipolar reference source selection */
#define ADCIFE_SEQCFG_ZOOMRANGE_Msk (0x7u << ADCIFE_SEQCFG_ZOOMRANGE_Pos)
#define ADCIFE_SEQCFG_ZOOMRANGE(value) ((ADCIFE_SEQCFG_ZOOMRANGE_Msk & ((value) << ADCIFE_SEQCFG_ZOOMRANGE_Pos)))
#define ADCIFE_SEQCFG_MASK          0x707FD7F5u  /**< \brief (ADCIFE_SEQCFG) MASK Register */

/* -------- ADCIFE_CDMA : (ADCIFE Offset: 0x18) ( /W 32) Configuration Direct Memory Access Register -------- */
#define ADCIFE_CDMA_OFFSET          0x18         /**< \brief (ADCIFE_CDMA offset) Configuration Direct Memory Access Register */
#define ADCIFE_CDMA_RESETVALUE      0x00000000   /**< \brief (ADCIFE_CDMA reset_value) Configuration Direct Memory Access Register */

// FIRST_DMA_WORD mode
#define ADCIFE_CDMA_FIRST_DMA_WORD_HWLA_Pos 0            /**< \brief (ADCIFE_CDMA_FIRST_DMA_WORD)  */
#define ADCIFE_CDMA_FIRST_DMA_WORD_HWLA (0x1u << ADCIFE_CDMA_FIRST_DMA_WORD_HWLA_Pos)
#define ADCIFE_CDMA_FIRST_DMA_WORD_BIPOLAR_Pos 2            /**< \brief (ADCIFE_CDMA_FIRST_DMA_WORD)  */
#define ADCIFE_CDMA_FIRST_DMA_WORD_BIPOLAR (0x1u << ADCIFE_CDMA_FIRST_DMA_WORD_BIPOLAR_Pos)
#define ADCIFE_CDMA_FIRST_DMA_WORD_STRIG_Pos 3            /**< \brief (ADCIFE_CDMA_FIRST_DMA_WORD)  */
#define ADCIFE_CDMA_FIRST_DMA_WORD_STRIG (0x1u << ADCIFE_CDMA_FIRST_DMA_WORD_STRIG_Pos)
#define ADCIFE_CDMA_FIRST_DMA_WORD_GAIN_Pos 4            /**< \brief (ADCIFE_CDMA_FIRST_DMA_WORD)  */
#define ADCIFE_CDMA_FIRST_DMA_WORD_GAIN_Msk (0x7u << ADCIFE_CDMA_FIRST_DMA_WORD_GAIN_Pos)
#define ADCIFE_CDMA_FIRST_DMA_WORD_GAIN(value) ((ADCIFE_CDMA_FIRST_DMA_WORD_GAIN_Msk & ((value) << ADCIFE_CDMA_FIRST_DMA_WORD_GAIN_Pos)))
#define ADCIFE_CDMA_FIRST_DMA_WORD_GCOMP_Pos 7            /**< \brief (ADCIFE_CDMA_FIRST_DMA_WORD)  */
#define ADCIFE_CDMA_FIRST_DMA_WORD_GCOMP (0x1u << ADCIFE_CDMA_FIRST_DMA_WORD_GCOMP_Pos)
#define ADCIFE_CDMA_FIRST_DMA_WORD_ENSTUP_Pos 8            /**< \brief (ADCIFE_CDMA_FIRST_DMA_WORD)  */
#define ADCIFE_CDMA_FIRST_DMA_WORD_ENSTUP (0x1u << ADCIFE_CDMA_FIRST_DMA_WORD_ENSTUP_Pos)
#define ADCIFE_CDMA_FIRST_DMA_WORD_RES_Pos 12           /**< \brief (ADCIFE_CDMA_FIRST_DMA_WORD)  */
#define ADCIFE_CDMA_FIRST_DMA_WORD_RES (0x1u << ADCIFE_CDMA_FIRST_DMA_WORD_RES_Pos)
#define ADCIFE_CDMA_FIRST_DMA_WORD_TSS_Pos 13           /**< \brief (ADCIFE_CDMA_FIRST_DMA_WORD) Internal timer start or stop bit */
#define ADCIFE_CDMA_FIRST_DMA_WORD_TSS (0x1u << ADCIFE_CDMA_FIRST_DMA_WORD_TSS_Pos)
#define ADCIFE_CDMA_FIRST_DMA_WORD_INTERNAL_Pos 14           /**< \brief (ADCIFE_CDMA_FIRST_DMA_WORD)  */
#define ADCIFE_CDMA_FIRST_DMA_WORD_INTERNAL_Msk (0x3u << ADCIFE_CDMA_FIRST_DMA_WORD_INTERNAL_Pos)
#define ADCIFE_CDMA_FIRST_DMA_WORD_INTERNAL(value) ((ADCIFE_CDMA_FIRST_DMA_WORD_INTERNAL_Msk & ((value) << ADCIFE_CDMA_FIRST_DMA_WORD_INTERNAL_Pos)))
#define ADCIFE_CDMA_FIRST_DMA_WORD_MUXPOS_Pos 16           /**< \brief (ADCIFE_CDMA_FIRST_DMA_WORD)  */
#define ADCIFE_CDMA_FIRST_DMA_WORD_MUXPOS_Msk (0xFu << ADCIFE_CDMA_FIRST_DMA_WORD_MUXPOS_Pos)
#define ADCIFE_CDMA_FIRST_DMA_WORD_MUXPOS(value) ((ADCIFE_CDMA_FIRST_DMA_WORD_MUXPOS_Msk & ((value) << ADCIFE_CDMA_FIRST_DMA_WORD_MUXPOS_Pos)))
#define ADCIFE_CDMA_FIRST_DMA_WORD_MUXNEG_Pos 20           /**< \brief (ADCIFE_CDMA_FIRST_DMA_WORD)  */
#define ADCIFE_CDMA_FIRST_DMA_WORD_MUXNEG_Msk (0x7u << ADCIFE_CDMA_FIRST_DMA_WORD_MUXNEG_Pos)
#define ADCIFE_CDMA_FIRST_DMA_WORD_MUXNEG(value) ((ADCIFE_CDMA_FIRST_DMA_WORD_MUXNEG_Msk & ((value) << ADCIFE_CDMA_FIRST_DMA_WORD_MUXNEG_Pos)))
#define ADCIFE_CDMA_FIRST_DMA_WORD_ZOOMRANGE_Pos 28           /**< \brief (ADCIFE_CDMA_FIRST_DMA_WORD)  */
#define ADCIFE_CDMA_FIRST_DMA_WORD_ZOOMRANGE_Msk (0x7u << ADCIFE_CDMA_FIRST_DMA_WORD_ZOOMRANGE_Pos)
#define ADCIFE_CDMA_FIRST_DMA_WORD_ZOOMRANGE(value) ((ADCIFE_CDMA_FIRST_DMA_WORD_ZOOMRANGE_Msk & ((value) << ADCIFE_CDMA_FIRST_DMA_WORD_ZOOMRANGE_Pos)))
#define ADCIFE_CDMA_FIRST_DMA_WORD_DW_Pos 31           /**< \brief (ADCIFE_CDMA_FIRST_DMA_WORD) Double Word transmitting */
#define ADCIFE_CDMA_FIRST_DMA_WORD_DW (0x1u << ADCIFE_CDMA_FIRST_DMA_WORD_DW_Pos)
#define ADCIFE_CDMA_FIRST_DMA_WORD_MASK 0xF07FF1FDu  /**< \brief (ADCIFE_CDMA_FIRST_DMA_WORD) MASK Register */

// SECOND_DMA_WORD mode
#define ADCIFE_CDMA_SECOND_DMA_WORD_LT_Pos 0            /**< \brief (ADCIFE_CDMA_SECOND_DMA_WORD)  */
#define ADCIFE_CDMA_SECOND_DMA_WORD_LT_Msk (0xFFFu << ADCIFE_CDMA_SECOND_DMA_WORD_LT_Pos)
#define ADCIFE_CDMA_SECOND_DMA_WORD_LT(value) ((ADCIFE_CDMA_SECOND_DMA_WORD_LT_Msk & ((value) << ADCIFE_CDMA_SECOND_DMA_WORD_LT_Pos)))
#define ADCIFE_CDMA_SECOND_DMA_WORD_WM_Pos 12           /**< \brief (ADCIFE_CDMA_SECOND_DMA_WORD)  */
#define ADCIFE_CDMA_SECOND_DMA_WORD_WM_Msk (0x7u << ADCIFE_CDMA_SECOND_DMA_WORD_WM_Pos)
#define ADCIFE_CDMA_SECOND_DMA_WORD_WM(value) ((ADCIFE_CDMA_SECOND_DMA_WORD_WM_Msk & ((value) << ADCIFE_CDMA_SECOND_DMA_WORD_WM_Pos)))
#define ADCIFE_CDMA_SECOND_DMA_WORD_HT_Pos 16           /**< \brief (ADCIFE_CDMA_SECOND_DMA_WORD)  */
#define ADCIFE_CDMA_SECOND_DMA_WORD_HT_Msk (0xFFFu << ADCIFE_CDMA_SECOND_DMA_WORD_HT_Pos)
#define ADCIFE_CDMA_SECOND_DMA_WORD_HT(value) ((ADCIFE_CDMA_SECOND_DMA_WORD_HT_Msk & ((value) << ADCIFE_CDMA_SECOND_DMA_WORD_HT_Pos)))
#define ADCIFE_CDMA_SECOND_DMA_WORD_DW_Pos 31           /**< \brief (ADCIFE_CDMA_SECOND_DMA_WORD)  */
#define ADCIFE_CDMA_SECOND_DMA_WORD_DW (0x1u << ADCIFE_CDMA_SECOND_DMA_WORD_DW_Pos)
#define ADCIFE_CDMA_SECOND_DMA_WORD_MASK 0x8FFF7FFFu  /**< \brief (ADCIFE_CDMA_SECOND_DMA_WORD) MASK Register */

// Any mode
#define ADCIFE_CDMA_HWLA_Pos        0            /**< \brief (ADCIFE_CDMA)  */
#define ADCIFE_CDMA_HWLA            (0x1u << ADCIFE_CDMA_HWLA_Pos)
#define ADCIFE_CDMA_LT_Pos          0            /**< \brief (ADCIFE_CDMA)  */
#define ADCIFE_CDMA_LT_Msk          (0xFFFu << ADCIFE_CDMA_LT_Pos)
#define ADCIFE_CDMA_LT(value)       ((ADCIFE_CDMA_LT_Msk & ((value) << ADCIFE_CDMA_LT_Pos)))
#define ADCIFE_CDMA_BIPOLAR_Pos     2            /**< \brief (ADCIFE_CDMA)  */
#define ADCIFE_CDMA_BIPOLAR         (0x1u << ADCIFE_CDMA_BIPOLAR_Pos)
#define ADCIFE_CDMA_STRIG_Pos       3            /**< \brief (ADCIFE_CDMA)  */
#define ADCIFE_CDMA_STRIG           (0x1u << ADCIFE_CDMA_STRIG_Pos)
#define ADCIFE_CDMA_GAIN_Pos        4            /**< \brief (ADCIFE_CDMA)  */
#define ADCIFE_CDMA_GAIN_Msk        (0x7u << ADCIFE_CDMA_GAIN_Pos)
#define ADCIFE_CDMA_GAIN(value)     ((ADCIFE_CDMA_GAIN_Msk & ((value) << ADCIFE_CDMA_GAIN_Pos)))
#define ADCIFE_CDMA_GCOMP_Pos       7            /**< \brief (ADCIFE_CDMA)  */
#define ADCIFE_CDMA_GCOMP           (0x1u << ADCIFE_CDMA_GCOMP_Pos)
#define ADCIFE_CDMA_ENSTUP_Pos      8            /**< \brief (ADCIFE_CDMA)  */
#define ADCIFE_CDMA_ENSTUP          (0x1u << ADCIFE_CDMA_ENSTUP_Pos)
#define ADCIFE_CDMA_RES_Pos         12           /**< \brief (ADCIFE_CDMA)  */
#define ADCIFE_CDMA_RES             (0x1u << ADCIFE_CDMA_RES_Pos)
#define ADCIFE_CDMA_WM_Pos          12           /**< \brief (ADCIFE_CDMA)  */
#define ADCIFE_CDMA_WM_Msk          (0x7u << ADCIFE_CDMA_WM_Pos)
#define ADCIFE_CDMA_WM(value)       ((ADCIFE_CDMA_WM_Msk & ((value) << ADCIFE_CDMA_WM_Pos)))
#define ADCIFE_CDMA_TSS_Pos         13           /**< \brief (ADCIFE_CDMA) Internal timer start or stop bit */
#define ADCIFE_CDMA_TSS             (0x1u << ADCIFE_CDMA_TSS_Pos)
#define ADCIFE_CDMA_INTERNAL_Pos    14           /**< \brief (ADCIFE_CDMA)  */
#define ADCIFE_CDMA_INTERNAL_Msk    (0x3u << ADCIFE_CDMA_INTERNAL_Pos)
#define ADCIFE_CDMA_INTERNAL(value) ((ADCIFE_CDMA_INTERNAL_Msk & ((value) << ADCIFE_CDMA_INTERNAL_Pos)))
#define ADCIFE_CDMA_MUXPOS_Pos      16           /**< \brief (ADCIFE_CDMA)  */
#define ADCIFE_CDMA_MUXPOS_Msk      (0xFu << ADCIFE_CDMA_MUXPOS_Pos)
#define ADCIFE_CDMA_MUXPOS(value)   ((ADCIFE_CDMA_MUXPOS_Msk & ((value) << ADCIFE_CDMA_MUXPOS_Pos)))
#define ADCIFE_CDMA_HT_Pos          16           /**< \brief (ADCIFE_CDMA)  */
#define ADCIFE_CDMA_HT_Msk          (0xFFFu << ADCIFE_CDMA_HT_Pos)
#define ADCIFE_CDMA_HT(value)       ((ADCIFE_CDMA_HT_Msk & ((value) << ADCIFE_CDMA_HT_Pos)))
#define ADCIFE_CDMA_MUXNEG_Pos      20           /**< \brief (ADCIFE_CDMA)  */
#define ADCIFE_CDMA_MUXNEG_Msk      (0x7u << ADCIFE_CDMA_MUXNEG_Pos)
#define ADCIFE_CDMA_MUXNEG(value)   ((ADCIFE_CDMA_MUXNEG_Msk & ((value) << ADCIFE_CDMA_MUXNEG_Pos)))
#define ADCIFE_CDMA_ZOOMRANGE_Pos   28           /**< \brief (ADCIFE_CDMA)  */
#define ADCIFE_CDMA_ZOOMRANGE_Msk   (0x7u << ADCIFE_CDMA_ZOOMRANGE_Pos)
#define ADCIFE_CDMA_ZOOMRANGE(value) ((ADCIFE_CDMA_ZOOMRANGE_Msk & ((value) << ADCIFE_CDMA_ZOOMRANGE_Pos)))
#define ADCIFE_CDMA_DW_Pos          31           /**< \brief (ADCIFE_CDMA) Double Word transmitting */
#define ADCIFE_CDMA_DW              (0x1u << ADCIFE_CDMA_DW_Pos)
#define ADCIFE_CDMA_MASK            0xFFFFFFFFu  /**< \brief (ADCIFE_CDMA) MASK Register */

/* -------- ADCIFE_TIM : (ADCIFE Offset: 0x1C) (R/W 32) Timing Configuration Register -------- */
#define ADCIFE_TIM_OFFSET           0x1C         /**< \brief (ADCIFE_TIM offset) Timing Configuration Register */
#define ADCIFE_TIM_RESETVALUE       0x00000000   /**< \brief (ADCIFE_TIM reset_value) Timing Configuration Register */

#define ADCIFE_TIM_STARTUP_Pos      0            /**< \brief (ADCIFE_TIM) Startup time */
#define ADCIFE_TIM_STARTUP_Msk      (0x1Fu << ADCIFE_TIM_STARTUP_Pos)
#define ADCIFE_TIM_STARTUP(value)   ((ADCIFE_TIM_STARTUP_Msk & ((value) << ADCIFE_TIM_STARTUP_Pos)))
#define ADCIFE_TIM_ENSTUP_Pos       8            /**< \brief (ADCIFE_TIM) Enable Startup */
#define ADCIFE_TIM_ENSTUP           (0x1u << ADCIFE_TIM_ENSTUP_Pos)
#define ADCIFE_TIM_MASK             0x0000011Fu  /**< \brief (ADCIFE_TIM) MASK Register */

/* -------- ADCIFE_ITIMER : (ADCIFE Offset: 0x20) (R/W 32) Internal Timer Register -------- */
#define ADCIFE_ITIMER_OFFSET        0x20         /**< \brief (ADCIFE_ITIMER offset) Internal Timer Register */
#define ADCIFE_ITIMER_RESETVALUE    0x00000000   /**< \brief (ADCIFE_ITIMER reset_value) Internal Timer Register */

#define ADCIFE_ITIMER_ITMC_Pos      0            /**< \brief (ADCIFE_ITIMER) Internal timer max counter */
#define ADCIFE_ITIMER_ITMC_Msk      (0xFFFFu << ADCIFE_ITIMER_ITMC_Pos)
#define ADCIFE_ITIMER_ITMC(value)   ((ADCIFE_ITIMER_ITMC_Msk & ((value) << ADCIFE_ITIMER_ITMC_Pos)))
#define ADCIFE_ITIMER_MASK          0x0000FFFFu  /**< \brief (ADCIFE_ITIMER) MASK Register */

/* -------- ADCIFE_WCFG : (ADCIFE Offset: 0x24) (R/W 32) Window Monitor Configuration Register -------- */
#define ADCIFE_WCFG_OFFSET          0x24         /**< \brief (ADCIFE_WCFG offset) Window Monitor Configuration Register */
#define ADCIFE_WCFG_RESETVALUE      0x00000000   /**< \brief (ADCIFE_WCFG reset_value) Window Monitor Configuration Register */

#define ADCIFE_WCFG_WM_Pos          12           /**< \brief (ADCIFE_WCFG) Window Monitor Mode */
#define ADCIFE_WCFG_WM_Msk          (0x7u << ADCIFE_WCFG_WM_Pos)
#define ADCIFE_WCFG_WM(value)       ((ADCIFE_WCFG_WM_Msk & ((value) << ADCIFE_WCFG_WM_Pos)))
#define ADCIFE_WCFG_MASK            0x00007000u  /**< \brief (ADCIFE_WCFG) MASK Register */

/* -------- ADCIFE_WTH : (ADCIFE Offset: 0x28) (R/W 32) Window Monitor Threshold Configuration Register -------- */
#define ADCIFE_WTH_OFFSET           0x28         /**< \brief (ADCIFE_WTH offset) Window Monitor Threshold Configuration Register */
#define ADCIFE_WTH_RESETVALUE       0x00000000   /**< \brief (ADCIFE_WTH reset_value) Window Monitor Threshold Configuration Register */

#define ADCIFE_WTH_LT_Pos           0            /**< \brief (ADCIFE_WTH) Low threshold */
#define ADCIFE_WTH_LT_Msk           (0xFFFu << ADCIFE_WTH_LT_Pos)
#define ADCIFE_WTH_LT(value)        ((ADCIFE_WTH_LT_Msk & ((value) << ADCIFE_WTH_LT_Pos)))
#define ADCIFE_WTH_HT_Pos           16           /**< \brief (ADCIFE_WTH) High Threshold */
#define ADCIFE_WTH_HT_Msk           (0xFFFu << ADCIFE_WTH_HT_Pos)
#define ADCIFE_WTH_HT(value)        ((ADCIFE_WTH_HT_Msk & ((value) << ADCIFE_WTH_HT_Pos)))
#define ADCIFE_WTH_MASK             0x0FFF0FFFu  /**< \brief (ADCIFE_WTH) MASK Register */

/* -------- ADCIFE_LCV : (ADCIFE Offset: 0x2C) (R/  32) Sequencer Last Converted Value Register -------- */
#define ADCIFE_LCV_OFFSET           0x2C         /**< \brief (ADCIFE_LCV offset) Sequencer Last Converted Value Register */
#define ADCIFE_LCV_RESETVALUE       0x00000000   /**< \brief (ADCIFE_LCV reset_value) Sequencer Last Converted Value Register */

#define ADCIFE_LCV_LCV_Pos          0            /**< \brief (ADCIFE_LCV) Last converted value */
#define ADCIFE_LCV_LCV_Msk          (0xFFFFu << ADCIFE_LCV_LCV_Pos)
#define ADCIFE_LCV_LCV(value)       ((ADCIFE_LCV_LCV_Msk & ((value) << ADCIFE_LCV_LCV_Pos)))
#define ADCIFE_LCV_LCPC_Pos         16           /**< \brief (ADCIFE_LCV) Last converted positive channel */
#define ADCIFE_LCV_LCPC_Msk         (0xFu << ADCIFE_LCV_LCPC_Pos)
#define ADCIFE_LCV_LCPC(value)      ((ADCIFE_LCV_LCPC_Msk & ((value) << ADCIFE_LCV_LCPC_Pos)))
#define ADCIFE_LCV_LCNC_Pos         20           /**< \brief (ADCIFE_LCV) Last converted negative channel */
#define ADCIFE_LCV_LCNC_Msk         (0x7u << ADCIFE_LCV_LCNC_Pos)
#define ADCIFE_LCV_LCNC(value)      ((ADCIFE_LCV_LCNC_Msk & ((value) << ADCIFE_LCV_LCNC_Pos)))
#define ADCIFE_LCV_MASK             0x007FFFFFu  /**< \brief (ADCIFE_LCV) MASK Register */

/* -------- ADCIFE_IER : (ADCIFE Offset: 0x30) ( /W 32) Interrupt Enable Register -------- */
#define ADCIFE_IER_OFFSET           0x30         /**< \brief (ADCIFE_IER offset) Interrupt Enable Register */
#define ADCIFE_IER_RESETVALUE       0x00000000   /**< \brief (ADCIFE_IER reset_value) Interrupt Enable Register */

#define ADCIFE_IER_SEOC_Pos         0            /**< \brief (ADCIFE_IER) Sequencer end of conversion Interrupt Enable */
#define ADCIFE_IER_SEOC             (0x1u << ADCIFE_IER_SEOC_Pos)
#define ADCIFE_IER_LOVR_Pos         1            /**< \brief (ADCIFE_IER) Sequencer last converted value overrun Interrupt Enable */
#define ADCIFE_IER_LOVR             (0x1u << ADCIFE_IER_LOVR_Pos)
#define ADCIFE_IER_WM_Pos           2            /**< \brief (ADCIFE_IER) Window monitor Interrupt Enable */
#define ADCIFE_IER_WM               (0x1u << ADCIFE_IER_WM_Pos)
#define ADCIFE_IER_SMTRG_Pos        3            /**< \brief (ADCIFE_IER) Sequencer missed trigger event Interrupt Enable */
#define ADCIFE_IER_SMTRG            (0x1u << ADCIFE_IER_SMTRG_Pos)
#define ADCIFE_IER_TTO_Pos          5            /**< \brief (ADCIFE_IER) Timer time-out Interrupt Enable */
#define ADCIFE_IER_TTO              (0x1u << ADCIFE_IER_TTO_Pos)
#define ADCIFE_IER_MASK             0x0000002Fu  /**< \brief (ADCIFE_IER) MASK Register */

/* -------- ADCIFE_IDR : (ADCIFE Offset: 0x34) ( /W 32) Interrupt Disable Register -------- */
#define ADCIFE_IDR_OFFSET           0x34         /**< \brief (ADCIFE_IDR offset) Interrupt Disable Register */
#define ADCIFE_IDR_RESETVALUE       0x00000000   /**< \brief (ADCIFE_IDR reset_value) Interrupt Disable Register */

#define ADCIFE_IDR_SEOC_Pos         0            /**< \brief (ADCIFE_IDR) Sequencer end of conversion Interrupt Disable */
#define ADCIFE_IDR_SEOC             (0x1u << ADCIFE_IDR_SEOC_Pos)
#define ADCIFE_IDR_LOVR_Pos         1            /**< \brief (ADCIFE_IDR) Sequencer last converted value overrun Interrupt Disable */
#define ADCIFE_IDR_LOVR             (0x1u << ADCIFE_IDR_LOVR_Pos)
#define ADCIFE_IDR_WM_Pos           2            /**< \brief (ADCIFE_IDR) Window monitor Interrupt Disable */
#define ADCIFE_IDR_WM               (0x1u << ADCIFE_IDR_WM_Pos)
#define ADCIFE_IDR_SMTRG_Pos        3            /**< \brief (ADCIFE_IDR) Sequencer missed trigger event Interrupt Disable */
#define ADCIFE_IDR_SMTRG            (0x1u << ADCIFE_IDR_SMTRG_Pos)
#define ADCIFE_IDR_TTO_Pos          5            /**< \brief (ADCIFE_IDR) Timer time-out Interrupt Disable */
#define ADCIFE_IDR_TTO              (0x1u << ADCIFE_IDR_TTO_Pos)
#define ADCIFE_IDR_MASK             0x0000002Fu  /**< \brief (ADCIFE_IDR) MASK Register */

/* -------- ADCIFE_IMR : (ADCIFE Offset: 0x38) (R/  32) Interrupt Mask Register -------- */
#define ADCIFE_IMR_OFFSET           0x38         /**< \brief (ADCIFE_IMR offset) Interrupt Mask Register */
#define ADCIFE_IMR_RESETVALUE       0x00000000   /**< \brief (ADCIFE_IMR reset_value) Interrupt Mask Register */

#define ADCIFE_IMR_SEOC_Pos         0            /**< \brief (ADCIFE_IMR) Sequencer end of conversion Interrupt Mask */
#define ADCIFE_IMR_SEOC             (0x1u << ADCIFE_IMR_SEOC_Pos)
#define ADCIFE_IMR_LOVR_Pos         1            /**< \brief (ADCIFE_IMR) Sequencer last converted value overrun Interrupt Mask */
#define ADCIFE_IMR_LOVR             (0x1u << ADCIFE_IMR_LOVR_Pos)
#define ADCIFE_IMR_WM_Pos           2            /**< \brief (ADCIFE_IMR) Window monitor Interrupt Mask */
#define ADCIFE_IMR_WM               (0x1u << ADCIFE_IMR_WM_Pos)
#define ADCIFE_IMR_SMTRG_Pos        3            /**< \brief (ADCIFE_IMR) Sequencer missed trigger event Interrupt Mask */
#define ADCIFE_IMR_SMTRG            (0x1u << ADCIFE_IMR_SMTRG_Pos)
#define ADCIFE_IMR_TTO_Pos          5            /**< \brief (ADCIFE_IMR) Timer time-out Interrupt Mask */
#define ADCIFE_IMR_TTO              (0x1u << ADCIFE_IMR_TTO_Pos)
#define ADCIFE_IMR_MASK             0x0000002Fu  /**< \brief (ADCIFE_IMR) MASK Register */

/* -------- ADCIFE_CALIB : (ADCIFE Offset: 0x3C) (R/W 32) Calibration Register -------- */
#define ADCIFE_CALIB_OFFSET         0x3C         /**< \brief (ADCIFE_CALIB offset) Calibration Register */
#define ADCIFE_CALIB_RESETVALUE     0x00000000   /**< \brief (ADCIFE_CALIB reset_value) Calibration Register */

#define ADCIFE_CALIB_CALIB_Pos      0            /**< \brief (ADCIFE_CALIB) Calibration Value */
#define ADCIFE_CALIB_CALIB_Msk      (0xFFu << ADCIFE_CALIB_CALIB_Pos)
#define ADCIFE_CALIB_CALIB(value)   ((ADCIFE_CALIB_CALIB_Msk & ((value) << ADCIFE_CALIB_CALIB_Pos)))
#define ADCIFE_CALIB_BIASSEL_Pos    8            /**< \brief (ADCIFE_CALIB) Select bias mode */
#define ADCIFE_CALIB_BIASSEL        (0x1u << ADCIFE_CALIB_BIASSEL_Pos)
#define ADCIFE_CALIB_BIASCAL_Pos    12           /**< \brief (ADCIFE_CALIB) Bias Calibration */
#define ADCIFE_CALIB_BIASCAL_Msk    (0xFu << ADCIFE_CALIB_BIASCAL_Pos)
#define ADCIFE_CALIB_BIASCAL(value) ((ADCIFE_CALIB_BIASCAL_Msk & ((value) << ADCIFE_CALIB_BIASCAL_Pos)))
#define ADCIFE_CALIB_FCD_Pos        16           /**< \brief (ADCIFE_CALIB) Flash Calibration Done */
#define ADCIFE_CALIB_FCD            (0x1u << ADCIFE_CALIB_FCD_Pos)
#define ADCIFE_CALIB_MASK           0x0001F1FFu  /**< \brief (ADCIFE_CALIB) MASK Register */

/* -------- ADCIFE_VERSION : (ADCIFE Offset: 0x40) (R/  32) Version Register -------- */
#define ADCIFE_VERSION_OFFSET       0x40         /**< \brief (ADCIFE_VERSION offset) Version Register */
#define ADCIFE_VERSION_RESETVALUE   0x00000100   /**< \brief (ADCIFE_VERSION reset_value) Version Register */

#define ADCIFE_VERSION_VERSION_Pos  0            /**< \brief (ADCIFE_VERSION) Version number */
#define ADCIFE_VERSION_VERSION_Msk  (0xFFFu << ADCIFE_VERSION_VERSION_Pos)
#define ADCIFE_VERSION_VERSION(value) ((ADCIFE_VERSION_VERSION_Msk & ((value) << ADCIFE_VERSION_VERSION_Pos)))
#define ADCIFE_VERSION_VARIANT_Pos  16           /**< \brief (ADCIFE_VERSION) Variant number */
#define ADCIFE_VERSION_VARIANT_Msk  (0xFu << ADCIFE_VERSION_VARIANT_Pos)
#define ADCIFE_VERSION_VARIANT(value) ((ADCIFE_VERSION_VARIANT_Msk & ((value) << ADCIFE_VERSION_VARIANT_Pos)))
#define ADCIFE_VERSION_MASK         0x000F0FFFu  /**< \brief (ADCIFE_VERSION) MASK Register */

/* -------- ADCIFE_PARAMETER : (ADCIFE Offset: 0x44) (R/  32) Parameter Register -------- */
#define ADCIFE_PARAMETER_OFFSET     0x44         /**< \brief (ADCIFE_PARAMETER offset) Parameter Register */

#define ADCIFE_PARAMETER_N_Pos      0            /**< \brief (ADCIFE_PARAMETER) Number of channels */
#define ADCIFE_PARAMETER_N_Msk      (0xFFu << ADCIFE_PARAMETER_N_Pos)
#define ADCIFE_PARAMETER_N(value)   ((ADCIFE_PARAMETER_N_Msk & ((value) << ADCIFE_PARAMETER_N_Pos)))
#define ADCIFE_PARAMETER_MASK       0x000000FFu  /**< \brief (ADCIFE_PARAMETER) MASK Register */

/** \brief ADCIFE hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
  WoReg   ADCIFE_CR;          /**< \brief (ADCIFE Offset: 0x00) Control Register */
  RwReg   ADCIFE_CFG;         /**< \brief (ADCIFE Offset: 0x04) Configuration Register */
  RoReg   ADCIFE_SR;          /**< \brief (ADCIFE Offset: 0x08) Status Register */
  WoReg   ADCIFE_SCR;         /**< \brief (ADCIFE Offset: 0x0C) Status Clear Register */
  RwReg   ADCIFE_RTS;         /**< \brief (ADCIFE Offset: 0x10) Resistive Touch Screen Register */
  RwReg   ADCIFE_SEQCFG;      /**< \brief (ADCIFE Offset: 0x14) Sequencer Configuration Register */
  WoReg   ADCIFE_CDMA;        /**< \brief (ADCIFE Offset: 0x18) Configuration Direct Memory Access Register */
  RwReg   ADCIFE_TIM;         /**< \brief (ADCIFE Offset: 0x1C) Timing Configuration Register */
  RwReg   ADCIFE_ITIMER;      /**< \brief (ADCIFE Offset: 0x20) Internal Timer Register */
  RwReg   ADCIFE_WCFG;        /**< \brief (ADCIFE Offset: 0x24) Window Monitor Configuration Register */
  RwReg   ADCIFE_WTH;         /**< \brief (ADCIFE Offset: 0x28) Window Monitor Threshold Configuration Register */
  RoReg   ADCIFE_LCV;         /**< \brief (ADCIFE Offset: 0x2C) Sequencer Last Converted Value Register */
  WoReg   ADCIFE_IER;         /**< \brief (ADCIFE Offset: 0x30) Interrupt Enable Register */
  WoReg   ADCIFE_IDR;         /**< \brief (ADCIFE Offset: 0x34) Interrupt Disable Register */
  RoReg   ADCIFE_IMR;         /**< \brief (ADCIFE Offset: 0x38) Interrupt Mask Register */
  RwReg   ADCIFE_CALIB;       /**< \brief (ADCIFE Offset: 0x3C) Calibration Register */
  RoReg   ADCIFE_VERSION;     /**< \brief (ADCIFE Offset: 0x40) Version Register */
  RoReg   ADCIFE_PARAMETER;   /**< \brief (ADCIFE Offset: 0x44) Parameter Register */
} Adcife;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/*@}*/

#endif /* _SAM4L_ADCIFE_COMPONENT_ */
