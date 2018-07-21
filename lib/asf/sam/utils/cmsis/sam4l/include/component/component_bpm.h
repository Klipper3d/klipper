/**
 * \file
 *
 * \brief Component description for BPM
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

#ifndef _SAM4L_BPM_COMPONENT_
#define _SAM4L_BPM_COMPONENT_

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR BPM */
/* ========================================================================== */
/** \addtogroup SAM4L_BPM Backup Power Manager */
/*@{*/

#define REV_BPM                     0x120

/* -------- BPM_IER : (BPM Offset: 0x00) ( /W 32) Interrupt Enable Register -------- */
#define BPM_IER_OFFSET              0x00         /**< \brief (BPM_IER offset) Interrupt Enable Register */
#define BPM_IER_RESETVALUE          0x00000000   /**< \brief (BPM_IER reset_value) Interrupt Enable Register */

#define BPM_IER_PSOK_Pos            0            /**< \brief (BPM_IER) Power Scaling OK Interrupt Enable */
#define BPM_IER_PSOK                (0x1u << BPM_IER_PSOK_Pos)
#define BPM_IER_AE_Pos              31           /**< \brief (BPM_IER) Access Error Interrupt Enable */
#define BPM_IER_AE                  (0x1u << BPM_IER_AE_Pos)
#define BPM_IER_MASK                0x80000001u  /**< \brief (BPM_IER) MASK Register */

/* -------- BPM_IDR : (BPM Offset: 0x04) ( /W 32) Interrupt Disable Register -------- */
#define BPM_IDR_OFFSET              0x04         /**< \brief (BPM_IDR offset) Interrupt Disable Register */
#define BPM_IDR_RESETVALUE          0x00000000   /**< \brief (BPM_IDR reset_value) Interrupt Disable Register */

#define BPM_IDR_PSOK_Pos            0            /**< \brief (BPM_IDR) Power Scaling OK Interrupt Disable */
#define BPM_IDR_PSOK                (0x1u << BPM_IDR_PSOK_Pos)
#define BPM_IDR_AE_Pos              31           /**< \brief (BPM_IDR) Access Error Interrupt Disable */
#define BPM_IDR_AE                  (0x1u << BPM_IDR_AE_Pos)
#define BPM_IDR_MASK                0x80000001u  /**< \brief (BPM_IDR) MASK Register */

/* -------- BPM_IMR : (BPM Offset: 0x08) (R/  32) Interrupt Mask Register -------- */
#define BPM_IMR_OFFSET              0x08         /**< \brief (BPM_IMR offset) Interrupt Mask Register */
#define BPM_IMR_RESETVALUE          0x00000000   /**< \brief (BPM_IMR reset_value) Interrupt Mask Register */

#define BPM_IMR_PSOK_Pos            0            /**< \brief (BPM_IMR) Power Scaling OK Interrupt Mask */
#define BPM_IMR_PSOK                (0x1u << BPM_IMR_PSOK_Pos)
#define BPM_IMR_AE_Pos              31           /**< \brief (BPM_IMR) Access Error Interrupt Mask */
#define BPM_IMR_AE                  (0x1u << BPM_IMR_AE_Pos)
#define BPM_IMR_MASK                0x80000001u  /**< \brief (BPM_IMR) MASK Register */

/* -------- BPM_ISR : (BPM Offset: 0x0C) (R/  32) Interrupt Status Register -------- */
#define BPM_ISR_OFFSET              0x0C         /**< \brief (BPM_ISR offset) Interrupt Status Register */
#define BPM_ISR_RESETVALUE          0x00000000   /**< \brief (BPM_ISR reset_value) Interrupt Status Register */

#define BPM_ISR_PSOK_Pos            0            /**< \brief (BPM_ISR) Power Scaling OK Interrupt Status */
#define BPM_ISR_PSOK                (0x1u << BPM_ISR_PSOK_Pos)
#define BPM_ISR_AE_Pos              31           /**< \brief (BPM_ISR) Access Error Interrupt Status */
#define BPM_ISR_AE                  (0x1u << BPM_ISR_AE_Pos)
#define BPM_ISR_MASK                0x80000001u  /**< \brief (BPM_ISR) MASK Register */

/* -------- BPM_ICR : (BPM Offset: 0x10) ( /W 32) Interrupt Clear Register -------- */
#define BPM_ICR_OFFSET              0x10         /**< \brief (BPM_ICR offset) Interrupt Clear Register */
#define BPM_ICR_RESETVALUE          0x00000000   /**< \brief (BPM_ICR reset_value) Interrupt Clear Register */

#define BPM_ICR_PSOK_Pos            0            /**< \brief (BPM_ICR) Power Scaling OK Interrupt Status Clear */
#define BPM_ICR_PSOK                (0x1u << BPM_ICR_PSOK_Pos)
#define BPM_ICR_AE_Pos              31           /**< \brief (BPM_ICR) Access Error Interrupt Status Clear */
#define BPM_ICR_AE                  (0x1u << BPM_ICR_AE_Pos)
#define BPM_ICR_MASK                0x80000001u  /**< \brief (BPM_ICR) MASK Register */

/* -------- BPM_SR : (BPM Offset: 0x14) (R/  32) Status Register -------- */
#define BPM_SR_OFFSET               0x14         /**< \brief (BPM_SR offset) Status Register */
#define BPM_SR_RESETVALUE           0x00000000   /**< \brief (BPM_SR reset_value) Status Register */

#define BPM_SR_PSOK_Pos             0            /**< \brief (BPM_SR) Power Scaling OK Status */
#define BPM_SR_PSOK                 (0x1u << BPM_SR_PSOK_Pos)
#define BPM_SR_MASK                 0x00000001u  /**< \brief (BPM_SR) MASK Register */

/* -------- BPM_UNLOCK : (BPM Offset: 0x18) ( /W 32) Unlock Register -------- */
#define BPM_UNLOCK_OFFSET           0x18         /**< \brief (BPM_UNLOCK offset) Unlock Register */
#define BPM_UNLOCK_RESETVALUE       0x00000000   /**< \brief (BPM_UNLOCK reset_value) Unlock Register */

#define BPM_UNLOCK_ADDR_Pos         0            /**< \brief (BPM_UNLOCK) Unlock Address */
#define BPM_UNLOCK_ADDR_Msk         (0x3FFu << BPM_UNLOCK_ADDR_Pos)
#define BPM_UNLOCK_ADDR(value)      ((BPM_UNLOCK_ADDR_Msk & ((value) << BPM_UNLOCK_ADDR_Pos)))
#define BPM_UNLOCK_KEY_Pos          24           /**< \brief (BPM_UNLOCK) Unlock Key */
#define BPM_UNLOCK_KEY_Msk          (0xFFu << BPM_UNLOCK_KEY_Pos)
#define BPM_UNLOCK_KEY(value)       ((BPM_UNLOCK_KEY_Msk & ((value) << BPM_UNLOCK_KEY_Pos)))
#define BPM_UNLOCK_MASK             0xFF0003FFu  /**< \brief (BPM_UNLOCK) MASK Register */

/* -------- BPM_PMCON : (BPM Offset: 0x1C) (R/W 32) Power Mode Control Register -------- */
#define BPM_PMCON_OFFSET            0x1C         /**< \brief (BPM_PMCON offset) Power Mode Control Register */
#define BPM_PMCON_RESETVALUE        0x00000000   /**< \brief (BPM_PMCON reset_value) Power Mode Control Register */

#define BPM_PMCON_PS_Pos            0            /**< \brief (BPM_PMCON) Power Scaling Configuration Value */
#define BPM_PMCON_PS_Msk            (0x3u << BPM_PMCON_PS_Pos)
#define BPM_PMCON_PS(value)         ((BPM_PMCON_PS_Msk & ((value) << BPM_PMCON_PS_Pos)))
#define BPM_PMCON_PSCREQ_Pos        2            /**< \brief (BPM_PMCON) Power Scaling Change Request */
#define BPM_PMCON_PSCREQ            (0x1u << BPM_PMCON_PSCREQ_Pos)
#define BPM_PMCON_PSCM_Pos          3            /**< \brief (BPM_PMCON) Power Scaling Change Mode */
#define BPM_PMCON_PSCM              (0x1u << BPM_PMCON_PSCM_Pos)
#define BPM_PMCON_BKUP_Pos          8            /**< \brief (BPM_PMCON) BACKUP Mode */
#define BPM_PMCON_BKUP              (0x1u << BPM_PMCON_BKUP_Pos)
#define BPM_PMCON_RET_Pos           9            /**< \brief (BPM_PMCON) RETENTION Mode */
#define BPM_PMCON_RET               (0x1u << BPM_PMCON_RET_Pos)
#define BPM_PMCON_SLEEP_Pos         12           /**< \brief (BPM_PMCON) SLEEP mode Configuration */
#define BPM_PMCON_SLEEP_Msk         (0x3u << BPM_PMCON_SLEEP_Pos)
#define BPM_PMCON_SLEEP(value)      ((BPM_PMCON_SLEEP_Msk & ((value) << BPM_PMCON_SLEEP_Pos)))
#define BPM_PMCON_CK32S_Pos         16           /**< \brief (BPM_PMCON) 32Khz-1Khz Clock Source Selection */
#define BPM_PMCON_CK32S             (0x1u << BPM_PMCON_CK32S_Pos)
#define BPM_PMCON_FASTWKUP_Pos      24           /**< \brief (BPM_PMCON) Fast Wakeup */
#define BPM_PMCON_FASTWKUP          (0x1u << BPM_PMCON_FASTWKUP_Pos)
#define BPM_PMCON_MASK              0x0101330Fu  /**< \brief (BPM_PMCON) MASK Register */

/* -------- BPM_BKUPWCAUSE : (BPM Offset: 0x28) (R/  32) Backup Wake up Cause Register -------- */
#define BPM_BKUPWCAUSE_OFFSET       0x28         /**< \brief (BPM_BKUPWCAUSE offset) Backup Wake up Cause Register */
#define BPM_BKUPWCAUSE_RESETVALUE   0x00000000   /**< \brief (BPM_BKUPWCAUSE reset_value) Backup Wake up Cause Register */
#define BPM_BKUPWCAUSE_MASK         0xFFFFFFFFu  /**< \brief (BPM_BKUPWCAUSE) MASK Register */

/* -------- BPM_BKUPWEN : (BPM Offset: 0x2C) (R/W 32) Backup Wake up Enable Register -------- */
#define BPM_BKUPWEN_OFFSET          0x2C         /**< \brief (BPM_BKUPWEN offset) Backup Wake up Enable Register */
#define BPM_BKUPWEN_RESETVALUE      0x00000000   /**< \brief (BPM_BKUPWEN reset_value) Backup Wake up Enable Register */
#define BPM_BKUPWEN_MASK            0xFFFFFFFFu  /**< \brief (BPM_BKUPWEN) MASK Register */

/* -------- BPM_BKUPPMUX : (BPM Offset: 0x30) (R/W 32) Backup Pin Muxing Register -------- */
#define BPM_BKUPPMUX_OFFSET         0x30         /**< \brief (BPM_BKUPPMUX offset) Backup Pin Muxing Register */
#define BPM_BKUPPMUX_RESETVALUE     0x00000000   /**< \brief (BPM_BKUPPMUX reset_value) Backup Pin Muxing Register */

#define BPM_BKUPPMUX_BKUPPMUX_Pos   0            /**< \brief (BPM_BKUPPMUX) Backup Pin Muxing */
#define BPM_BKUPPMUX_BKUPPMUX_Msk   (0x1FFu << BPM_BKUPPMUX_BKUPPMUX_Pos)
#define BPM_BKUPPMUX_BKUPPMUX(value) ((BPM_BKUPPMUX_BKUPPMUX_Msk & ((value) << BPM_BKUPPMUX_BKUPPMUX_Pos)))
#define BPM_BKUPPMUX_MASK           0x000001FFu  /**< \brief (BPM_BKUPPMUX) MASK Register */

/* -------- BPM_IORET : (BPM Offset: 0x34) (R/W 32) Input Output Retention Register -------- */
#define BPM_IORET_OFFSET            0x34         /**< \brief (BPM_IORET offset) Input Output Retention Register */
#define BPM_IORET_RESETVALUE        0x00000000   /**< \brief (BPM_IORET reset_value) Input Output Retention Register */

#define BPM_IORET_RET_Pos           0            /**< \brief (BPM_IORET) Retention on I/O lines after waking up from the BACKUP mode */
#define BPM_IORET_RET               (0x1u << BPM_IORET_RET_Pos)
#define BPM_IORET_MASK              0x00000001u  /**< \brief (BPM_IORET) MASK Register */

/* -------- BPM_BPR : (BPM Offset: 0x40) (R/W 32) Bypass Register -------- */
#define BPM_BPR_OFFSET              0x40         /**< \brief (BPM_BPR offset) Bypass Register */
#define BPM_BPR_RESETVALUE          0x00000000   /**< \brief (BPM_BPR reset_value) Bypass Register */

#define BPM_BPR_RUNPSPB_Pos         0            /**< \brief (BPM_BPR) Run Mode Power Scaling Preset Bypass */
#define BPM_BPR_RUNPSPB             (0x1u << BPM_BPR_RUNPSPB_Pos)
#define BPM_BPR_PSMPSPB_Pos         1            /**< \brief (BPM_BPR) Power Save Mode Power Scaling Preset Bypass */
#define BPM_BPR_PSMPSPB             (0x1u << BPM_BPR_PSMPSPB_Pos)
#define BPM_BPR_SEQSTN_Pos          2            /**< \brief (BPM_BPR) Sequencial Startup from ULP (Active Low) */
#define BPM_BPR_SEQSTN              (0x1u << BPM_BPR_SEQSTN_Pos)
#define BPM_BPR_PSBTD_Pos           3            /**< \brief (BPM_BPR) Power Scaling Bias Timing Disable */
#define BPM_BPR_PSBTD               (0x1u << BPM_BPR_PSBTD_Pos)
#define BPM_BPR_PSHFD_Pos           4            /**< \brief (BPM_BPR) Power Scaling Halt Flash Until VREGOK Disable */
#define BPM_BPR_PSHFD               (0x1u << BPM_BPR_PSHFD_Pos)
#define BPM_BPR_DLYRSTD_Pos         5            /**< \brief (BPM_BPR) Delaying Reset Disable */
#define BPM_BPR_DLYRSTD             (0x1u << BPM_BPR_DLYRSTD_Pos)
#define BPM_BPR_BIASSEN_Pos         6            /**< \brief (BPM_BPR) Bias Switch Enable */
#define BPM_BPR_BIASSEN             (0x1u << BPM_BPR_BIASSEN_Pos)
#define BPM_BPR_LATSEN_Pos          7            /**< \brief (BPM_BPR) Latdel Switch Enable */
#define BPM_BPR_LATSEN              (0x1u << BPM_BPR_LATSEN_Pos)
#define BPM_BPR_BOD18CONT_Pos       8            /**< \brief (BPM_BPR) BOD18 in continuous mode not disabled in WAIT/RET/BACKUP modes */
#define BPM_BPR_BOD18CONT           (0x1u << BPM_BPR_BOD18CONT_Pos)
#define BPM_BPR_POBS_Pos            9            /**< \brief (BPM_BPR) Pico Uart Observability */
#define BPM_BPR_POBS                (0x1u << BPM_BPR_POBS_Pos)
#define BPM_BPR_FFFW_Pos            10           /**< \brief (BPM_BPR) Force Flash Fast Wakeup */
#define BPM_BPR_FFFW                (0x1u << BPM_BPR_FFFW_Pos)
#define BPM_BPR_FBRDYEN_Pos         11           /**< \brief (BPM_BPR) Flash Bias Ready Enable */
#define BPM_BPR_FBRDYEN             (0x1u << BPM_BPR_FBRDYEN_Pos)
#define BPM_BPR_FVREFSEN_Pos        12           /**< \brief (BPM_BPR) Flash Vref Switch Enable */
#define BPM_BPR_FVREFSEN            (0x1u << BPM_BPR_FVREFSEN_Pos)
#define BPM_BPR_MASK                0x00001FFFu  /**< \brief (BPM_BPR) MASK Register */

/* -------- BPM_FWRUNPS : (BPM Offset: 0x44) (R/  32) Factory Word Run PS Register -------- */
#define BPM_FWRUNPS_OFFSET          0x44         /**< \brief (BPM_FWRUNPS offset) Factory Word Run PS Register */

#define BPM_FWRUNPS_REGLEVEL_Pos    0            /**< \brief (BPM_FWRUNPS) Regulator Voltage Level */
#define BPM_FWRUNPS_REGLEVEL_Msk    (0xFu << BPM_FWRUNPS_REGLEVEL_Pos)
#define BPM_FWRUNPS_REGLEVEL(value) ((BPM_FWRUNPS_REGLEVEL_Msk & ((value) << BPM_FWRUNPS_REGLEVEL_Pos)))
#define BPM_FWRUNPS_REGTYPE_Pos     4            /**< \brief (BPM_FWRUNPS) Regulator Type */
#define BPM_FWRUNPS_REGTYPE_Msk     (0x3u << BPM_FWRUNPS_REGTYPE_Pos)
#define BPM_FWRUNPS_REGTYPE(value)  ((BPM_FWRUNPS_REGTYPE_Msk & ((value) << BPM_FWRUNPS_REGTYPE_Pos)))
#define   BPM_FWRUNPS_REGTYPE_NORMAL (0x0u <<  4) /**< \brief (BPM_FWRUNPS)  */
#define   BPM_FWRUNPS_REGTYPE_LP    (0x1u <<  4) /**< \brief (BPM_FWRUNPS)  */
#define   BPM_FWRUNPS_REGTYPE_XULP  (0x2u <<  4) /**< \brief (BPM_FWRUNPS)  */
#define BPM_FWRUNPS_REFTYPE_Pos     6            /**< \brief (BPM_FWRUNPS) Reference Type */
#define BPM_FWRUNPS_REFTYPE_Msk     (0x3u << BPM_FWRUNPS_REFTYPE_Pos)
#define BPM_FWRUNPS_REFTYPE(value)  ((BPM_FWRUNPS_REFTYPE_Msk & ((value) << BPM_FWRUNPS_REFTYPE_Pos)))
#define   BPM_FWRUNPS_REFTYPE_BOTH  (0x0u <<  6) /**< \brief (BPM_FWRUNPS)  */
#define   BPM_FWRUNPS_REFTYPE_BG    (0x1u <<  6) /**< \brief (BPM_FWRUNPS)  */
#define   BPM_FWRUNPS_REFTYPE_LPBG  (0x2u <<  6) /**< \brief (BPM_FWRUNPS)  */
#define   BPM_FWRUNPS_REFTYPE_INTERNAL (0x3u <<  6) /**< \brief (BPM_FWRUNPS)  */
#define BPM_FWRUNPS_FLASHLATDEL_Pos 8            /**< \brief (BPM_FWRUNPS) Flash Latch Delay Value */
#define BPM_FWRUNPS_FLASHLATDEL_Msk (0x1Fu << BPM_FWRUNPS_FLASHLATDEL_Pos)
#define BPM_FWRUNPS_FLASHLATDEL(value) ((BPM_FWRUNPS_FLASHLATDEL_Msk & ((value) << BPM_FWRUNPS_FLASHLATDEL_Pos)))
#define BPM_FWRUNPS_FLASHBIAS_Pos   13           /**< \brief (BPM_FWRUNPS) Flash Bias Value */
#define BPM_FWRUNPS_FLASHBIAS_Msk   (0xFu << BPM_FWRUNPS_FLASHBIAS_Pos)
#define BPM_FWRUNPS_FLASHBIAS(value) ((BPM_FWRUNPS_FLASHBIAS_Msk & ((value) << BPM_FWRUNPS_FLASHBIAS_Pos)))
#define BPM_FWRUNPS_FPPW_Pos        17           /**< \brief (BPM_FWRUNPS) Flash Pico Power Mode */
#define BPM_FWRUNPS_FPPW            (0x1u << BPM_FWRUNPS_FPPW_Pos)
#define BPM_FWRUNPS_RC115_Pos       18           /**< \brief (BPM_FWRUNPS) RC 115KHZ Calibration Value */
#define BPM_FWRUNPS_RC115_Msk       (0x7Fu << BPM_FWRUNPS_RC115_Pos)
#define BPM_FWRUNPS_RC115(value)    ((BPM_FWRUNPS_RC115_Msk & ((value) << BPM_FWRUNPS_RC115_Pos)))
#define BPM_FWRUNPS_RCFAST_Pos      25           /**< \brief (BPM_FWRUNPS) RCFAST Calibration Value */
#define BPM_FWRUNPS_RCFAST_Msk      (0x7Fu << BPM_FWRUNPS_RCFAST_Pos)
#define BPM_FWRUNPS_RCFAST(value)   ((BPM_FWRUNPS_RCFAST_Msk & ((value) << BPM_FWRUNPS_RCFAST_Pos)))
#define BPM_FWRUNPS_MASK            0xFFFFFFFFu  /**< \brief (BPM_FWRUNPS) MASK Register */

/* -------- BPM_FWPSAVEPS : (BPM Offset: 0x48) (R/  32) Factory Word Power Save PS Register -------- */
#define BPM_FWPSAVEPS_OFFSET        0x48         /**< \brief (BPM_FWPSAVEPS offset) Factory Word Power Save PS Register */

#define BPM_FWPSAVEPS_WREGLEVEL_Pos 0            /**< \brief (BPM_FWPSAVEPS) Wait mode Regulator Level */
#define BPM_FWPSAVEPS_WREGLEVEL_Msk (0xFu << BPM_FWPSAVEPS_WREGLEVEL_Pos)
#define BPM_FWPSAVEPS_WREGLEVEL(value) ((BPM_FWPSAVEPS_WREGLEVEL_Msk & ((value) << BPM_FWPSAVEPS_WREGLEVEL_Pos)))
#define BPM_FWPSAVEPS_WBIAS_Pos     4            /**< \brief (BPM_FWPSAVEPS) Bias in wait mode */
#define BPM_FWPSAVEPS_WBIAS_Msk     (0xFu << BPM_FWPSAVEPS_WBIAS_Pos)
#define BPM_FWPSAVEPS_WBIAS(value)  ((BPM_FWPSAVEPS_WBIAS_Msk & ((value) << BPM_FWPSAVEPS_WBIAS_Pos)))
#define BPM_FWPSAVEPS_WLATDEL_Pos   8            /**< \brief (BPM_FWPSAVEPS) Flash Latdel in wait mode */
#define BPM_FWPSAVEPS_WLATDEL_Msk   (0x1Fu << BPM_FWPSAVEPS_WLATDEL_Pos)
#define BPM_FWPSAVEPS_WLATDEL(value) ((BPM_FWPSAVEPS_WLATDEL_Msk & ((value) << BPM_FWPSAVEPS_WLATDEL_Pos)))
#define BPM_FWPSAVEPS_RREGLEVEL_Pos 13           /**< \brief (BPM_FWPSAVEPS) Retention mode Regulator Level */
#define BPM_FWPSAVEPS_RREGLEVEL_Msk (0xFu << BPM_FWPSAVEPS_RREGLEVEL_Pos)
#define BPM_FWPSAVEPS_RREGLEVEL(value) ((BPM_FWPSAVEPS_RREGLEVEL_Msk & ((value) << BPM_FWPSAVEPS_RREGLEVEL_Pos)))
#define BPM_FWPSAVEPS_RBIAS_Pos     17           /**< \brief (BPM_FWPSAVEPS) Bias in Retention mode */
#define BPM_FWPSAVEPS_RBIAS_Msk     (0xFu << BPM_FWPSAVEPS_RBIAS_Pos)
#define BPM_FWPSAVEPS_RBIAS(value)  ((BPM_FWPSAVEPS_RBIAS_Msk & ((value) << BPM_FWPSAVEPS_RBIAS_Pos)))
#define BPM_FWPSAVEPS_RLATDEL_Pos   21           /**< \brief (BPM_FWPSAVEPS) Flash Latdel in Retention mode */
#define BPM_FWPSAVEPS_RLATDEL_Msk   (0x1Fu << BPM_FWPSAVEPS_RLATDEL_Pos)
#define BPM_FWPSAVEPS_RLATDEL(value) ((BPM_FWPSAVEPS_RLATDEL_Msk & ((value) << BPM_FWPSAVEPS_RLATDEL_Pos)))
#define BPM_FWPSAVEPS_BREGLEVEL_Pos 26           /**< \brief (BPM_FWPSAVEPS) Backup mode Regulator Level */
#define BPM_FWPSAVEPS_BREGLEVEL_Msk (0xFu << BPM_FWPSAVEPS_BREGLEVEL_Pos)
#define BPM_FWPSAVEPS_BREGLEVEL(value) ((BPM_FWPSAVEPS_BREGLEVEL_Msk & ((value) << BPM_FWPSAVEPS_BREGLEVEL_Pos)))
#define BPM_FWPSAVEPS_POR18DIS_Pos  30           /**< \brief (BPM_FWPSAVEPS) POR 18 Disable */
#define BPM_FWPSAVEPS_POR18DIS      (0x1u << BPM_FWPSAVEPS_POR18DIS_Pos)
#define BPM_FWPSAVEPS_FWSAS_Pos     31           /**< \brief (BPM_FWPSAVEPS) Flash Wait State Automatic Switching */
#define BPM_FWPSAVEPS_FWSAS         (0x1u << BPM_FWPSAVEPS_FWSAS_Pos)
#define BPM_FWPSAVEPS_MASK          0xFFFFFFFFu  /**< \brief (BPM_FWPSAVEPS) MASK Register */

/* -------- BPM_VERSION : (BPM Offset: 0xFC) (R/  32) Version Register -------- */
#define BPM_VERSION_OFFSET          0xFC         /**< \brief (BPM_VERSION offset) Version Register */
#define BPM_VERSION_RESETVALUE      0x00000120   /**< \brief (BPM_VERSION reset_value) Version Register */

#define BPM_VERSION_VERSION_Pos     0            /**< \brief (BPM_VERSION) Version Number */
#define BPM_VERSION_VERSION_Msk     (0xFFFu << BPM_VERSION_VERSION_Pos)
#define BPM_VERSION_VERSION(value)  ((BPM_VERSION_VERSION_Msk & ((value) << BPM_VERSION_VERSION_Pos)))
#define BPM_VERSION_VARIANT_Pos     16           /**< \brief (BPM_VERSION) Variant Number */
#define BPM_VERSION_VARIANT_Msk     (0xFu << BPM_VERSION_VARIANT_Pos)
#define BPM_VERSION_VARIANT(value)  ((BPM_VERSION_VARIANT_Msk & ((value) << BPM_VERSION_VARIANT_Pos)))
#define BPM_VERSION_MASK            0x000F0FFFu  /**< \brief (BPM_VERSION) MASK Register */

/** \brief BPM hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
  WoReg   BPM_IER;            /**< \brief (BPM Offset: 0x00) Interrupt Enable Register */
  WoReg   BPM_IDR;            /**< \brief (BPM Offset: 0x04) Interrupt Disable Register */
  RoReg   BPM_IMR;            /**< \brief (BPM Offset: 0x08) Interrupt Mask Register */
  RoReg   BPM_ISR;            /**< \brief (BPM Offset: 0x0C) Interrupt Status Register */
  WoReg   BPM_ICR;            /**< \brief (BPM Offset: 0x10) Interrupt Clear Register */
  RoReg   BPM_SR;             /**< \brief (BPM Offset: 0x14) Status Register */
  WoReg   BPM_UNLOCK;         /**< \brief (BPM Offset: 0x18) Unlock Register */
  RwReg   BPM_PMCON;          /**< \brief (BPM Offset: 0x1C) Power Mode Control Register */
  RoReg8  Reserved1[0x8];
  RoReg   BPM_BKUPWCAUSE;     /**< \brief (BPM Offset: 0x28) Backup Wake up Cause Register */
  RwReg   BPM_BKUPWEN;        /**< \brief (BPM Offset: 0x2C) Backup Wake up Enable Register */
  RwReg   BPM_BKUPPMUX;       /**< \brief (BPM Offset: 0x30) Backup Pin Muxing Register */
  RwReg   BPM_IORET;          /**< \brief (BPM Offset: 0x34) Input Output Retention Register */
  RoReg8  Reserved2[0x8];
  RwReg   BPM_BPR;            /**< \brief (BPM Offset: 0x40) Bypass Register */
  RoReg   BPM_FWRUNPS;        /**< \brief (BPM Offset: 0x44) Factory Word Run PS Register */
  RoReg   BPM_FWPSAVEPS;      /**< \brief (BPM Offset: 0x48) Factory Word Power Save PS Register */
  RoReg8  Reserved3[0xB0];
  RoReg   BPM_VERSION;        /**< \brief (BPM Offset: 0xFC) Version Register */
} Bpm;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/*@}*/

#endif /* _SAM4L_BPM_COMPONENT_ */
