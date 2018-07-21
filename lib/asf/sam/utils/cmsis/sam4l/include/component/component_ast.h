/**
 * \file
 *
 * \brief Component description for AST
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

#ifndef _SAM4L_AST_COMPONENT_
#define _SAM4L_AST_COMPONENT_

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR AST */
/* ========================================================================== */
/** \addtogroup SAM4L_AST Asynchronous Timer */
/*@{*/

#define REV_AST                     0x311

/* -------- AST_CR : (AST Offset: 0x00) (R/W 32) Control Register -------- */
#define AST_CR_OFFSET               0x00         /**< \brief (AST_CR offset) Control Register */
#define AST_CR_RESETVALUE           0x00000000   /**< \brief (AST_CR reset_value) Control Register */

#define AST_CR_EN_Pos               0            /**< \brief (AST_CR) Enable */
#define AST_CR_EN                   (0x1u << AST_CR_EN_Pos)
#define   AST_CR_EN_0               (0x0u <<  0) /**< \brief (AST_CR) The AST is disabled. */
#define   AST_CR_EN_1               (0x1u <<  0) /**< \brief (AST_CR) The AST is enabled */
#define AST_CR_PCLR_Pos             1            /**< \brief (AST_CR) Prescaler Clear */
#define AST_CR_PCLR                 (0x1u << AST_CR_PCLR_Pos)
#define AST_CR_CAL_Pos              2            /**< \brief (AST_CR) Calendar mode */
#define AST_CR_CAL                  (0x1u << AST_CR_CAL_Pos)
#define AST_CR_CA0_Pos              8            /**< \brief (AST_CR) Clear on Alarm 0 */
#define AST_CR_CA0                  (0x1u << AST_CR_CA0_Pos)
#define AST_CR_CA1_Pos              9            /**< \brief (AST_CR) Clear on Alarm 1 */
#define AST_CR_CA1                  (0x1u << AST_CR_CA1_Pos)
#define AST_CR_PSEL_Pos             16           /**< \brief (AST_CR) Prescaler Select */
#define AST_CR_PSEL_Msk             (0x1Fu << AST_CR_PSEL_Pos)
#define AST_CR_PSEL(value)          ((AST_CR_PSEL_Msk & ((value) << AST_CR_PSEL_Pos)))
#define AST_CR_MASK                 0x001F0307u  /**< \brief (AST_CR) MASK Register */

/* -------- AST_CV : (AST Offset: 0x04) (R/W 32) Counter Value -------- */
#define AST_CV_OFFSET               0x04         /**< \brief (AST_CV offset) Counter Value */
#define AST_CV_RESETVALUE           0x00000000   /**< \brief (AST_CV reset_value) Counter Value */

#define AST_CV_VALUE_Pos            0            /**< \brief (AST_CV) AST Value */
#define AST_CV_VALUE_Msk            (0xFFFFFFFFu << AST_CV_VALUE_Pos)
#define AST_CV_VALUE(value)         ((AST_CV_VALUE_Msk & ((value) << AST_CV_VALUE_Pos)))
#define AST_CV_MASK                 0xFFFFFFFFu  /**< \brief (AST_CV) MASK Register */

/* -------- AST_SR : (AST Offset: 0x08) (R/  32) Status Register -------- */
#define AST_SR_OFFSET               0x08         /**< \brief (AST_SR offset) Status Register */
#define AST_SR_RESETVALUE           0x00000000   /**< \brief (AST_SR reset_value) Status Register */

#define AST_SR_OVF_Pos              0            /**< \brief (AST_SR) Overflow */
#define AST_SR_OVF                  (0x1u << AST_SR_OVF_Pos)
#define AST_SR_ALARM0_Pos           8            /**< \brief (AST_SR) Alarm 0 */
#define AST_SR_ALARM0               (0x1u << AST_SR_ALARM0_Pos)
#define AST_SR_ALARM1_Pos           9            /**< \brief (AST_SR) Alarm 1 */
#define AST_SR_ALARM1               (0x1u << AST_SR_ALARM1_Pos)
#define AST_SR_PER0_Pos             16           /**< \brief (AST_SR) Periodic 0 */
#define AST_SR_PER0                 (0x1u << AST_SR_PER0_Pos)
#define AST_SR_PER1_Pos             17           /**< \brief (AST_SR) Periodic 1 */
#define AST_SR_PER1                 (0x1u << AST_SR_PER1_Pos)
#define AST_SR_BUSY_Pos             24           /**< \brief (AST_SR) AST Busy */
#define AST_SR_BUSY                 (0x1u << AST_SR_BUSY_Pos)
#define   AST_SR_BUSY_0             (0x0u << 24) /**< \brief (AST_SR) The AST accepts writes to CV, WER, DTR, SCR, AR, PIR and CR */
#define   AST_SR_BUSY_1             (0x1u << 24) /**< \brief (AST_SR) The AST is busy and will discard writes to CV, WER, DTR, SCR, AR, PIR and CR */
#define AST_SR_READY_Pos            25           /**< \brief (AST_SR) AST Ready */
#define AST_SR_READY                (0x1u << AST_SR_READY_Pos)
#define AST_SR_CLKBUSY_Pos          28           /**< \brief (AST_SR) Clock Busy */
#define AST_SR_CLKBUSY              (0x1u << AST_SR_CLKBUSY_Pos)
#define   AST_SR_CLKBUSY_0          (0x0u << 28) /**< \brief (AST_SR) The clock is ready and can be changed */
#define   AST_SR_CLKBUSY_1          (0x1u << 28) /**< \brief (AST_SR) CEN has been written and the clock is busy */
#define AST_SR_CLKRDY_Pos           29           /**< \brief (AST_SR) Clock Ready */
#define AST_SR_CLKRDY               (0x1u << AST_SR_CLKRDY_Pos)
#define AST_SR_MASK                 0x33030301u  /**< \brief (AST_SR) MASK Register */

/* -------- AST_SCR : (AST Offset: 0x0C) ( /W 32) Status Clear Register -------- */
#define AST_SCR_OFFSET              0x0C         /**< \brief (AST_SCR offset) Status Clear Register */
#define AST_SCR_RESETVALUE          0x00000000   /**< \brief (AST_SCR reset_value) Status Clear Register */

#define AST_SCR_OVF_Pos             0            /**< \brief (AST_SCR) Overflow */
#define AST_SCR_OVF                 (0x1u << AST_SCR_OVF_Pos)
#define AST_SCR_ALARM0_Pos          8            /**< \brief (AST_SCR) Alarm 0 */
#define AST_SCR_ALARM0              (0x1u << AST_SCR_ALARM0_Pos)
#define AST_SCR_ALARM1_Pos          9            /**< \brief (AST_SCR) Alarm 1 */
#define AST_SCR_ALARM1              (0x1u << AST_SCR_ALARM1_Pos)
#define AST_SCR_PER0_Pos            16           /**< \brief (AST_SCR) Periodic 0 */
#define AST_SCR_PER0                (0x1u << AST_SCR_PER0_Pos)
#define AST_SCR_PER1_Pos            17           /**< \brief (AST_SCR) Periodic 1 */
#define AST_SCR_PER1                (0x1u << AST_SCR_PER1_Pos)
#define AST_SCR_READY_Pos           25           /**< \brief (AST_SCR) AST Ready */
#define AST_SCR_READY               (0x1u << AST_SCR_READY_Pos)
#define AST_SCR_CLKRDY_Pos          29           /**< \brief (AST_SCR) Clock Ready */
#define AST_SCR_CLKRDY              (0x1u << AST_SCR_CLKRDY_Pos)
#define AST_SCR_MASK                0x22030301u  /**< \brief (AST_SCR) MASK Register */

/* -------- AST_IER : (AST Offset: 0x10) ( /W 32) Interrupt Enable Register -------- */
#define AST_IER_OFFSET              0x10         /**< \brief (AST_IER offset) Interrupt Enable Register */
#define AST_IER_RESETVALUE          0x00000000   /**< \brief (AST_IER reset_value) Interrupt Enable Register */

#define AST_IER_OVF_Pos             0            /**< \brief (AST_IER) Overflow */
#define AST_IER_OVF                 (0x1u << AST_IER_OVF_Pos)
#define   AST_IER_OVF_0             (0x0u <<  0) /**< \brief (AST_IER) No effect */
#define   AST_IER_OVF_1             (0x1u <<  0) /**< \brief (AST_IER) Enable Interrupt. */
#define AST_IER_ALARM0_Pos          8            /**< \brief (AST_IER) Alarm 0 */
#define AST_IER_ALARM0              (0x1u << AST_IER_ALARM0_Pos)
#define   AST_IER_ALARM0_0          (0x0u <<  8) /**< \brief (AST_IER) No effect */
#define   AST_IER_ALARM0_1          (0x1u <<  8) /**< \brief (AST_IER) Enable interrupt */
#define AST_IER_ALARM1_Pos          9            /**< \brief (AST_IER) Alarm 1 */
#define AST_IER_ALARM1              (0x1u << AST_IER_ALARM1_Pos)
#define   AST_IER_ALARM1_0          (0x0u <<  9) /**< \brief (AST_IER) No effect */
#define   AST_IER_ALARM1_1          (0x1u <<  9) /**< \brief (AST_IER) Enable interrupt */
#define AST_IER_PER0_Pos            16           /**< \brief (AST_IER) Periodic 0 */
#define AST_IER_PER0                (0x1u << AST_IER_PER0_Pos)
#define   AST_IER_PER0_0            (0x0u << 16) /**< \brief (AST_IER) No effect */
#define   AST_IER_PER0_1            (0x1u << 16) /**< \brief (AST_IER) Enable interrupt */
#define AST_IER_PER1_Pos            17           /**< \brief (AST_IER) Periodic 1 */
#define AST_IER_PER1                (0x1u << AST_IER_PER1_Pos)
#define   AST_IER_PER1_0            (0x0u << 17) /**< \brief (AST_IER) No effect */
#define   AST_IER_PER1_1            (0x1u << 17) /**< \brief (AST_IER) Enable interrupt */
#define AST_IER_READY_Pos           25           /**< \brief (AST_IER) AST Ready */
#define AST_IER_READY               (0x1u << AST_IER_READY_Pos)
#define   AST_IER_READY_0           (0x0u << 25) /**< \brief (AST_IER) No effect */
#define   AST_IER_READY_1           (0x1u << 25) /**< \brief (AST_IER) Enable interrupt */
#define AST_IER_CLKRDY_Pos          29           /**< \brief (AST_IER) Clock Ready */
#define AST_IER_CLKRDY              (0x1u << AST_IER_CLKRDY_Pos)
#define   AST_IER_CLKRDY_0          (0x0u << 29) /**< \brief (AST_IER) No effect */
#define   AST_IER_CLKRDY_1          (0x1u << 29) /**< \brief (AST_IER) Enable interrupt */
#define AST_IER_MASK                0x22030301u  /**< \brief (AST_IER) MASK Register */

/* -------- AST_IDR : (AST Offset: 0x14) ( /W 32) Interrupt Disable Register -------- */
#define AST_IDR_OFFSET              0x14         /**< \brief (AST_IDR offset) Interrupt Disable Register */
#define AST_IDR_RESETVALUE          0x00000000   /**< \brief (AST_IDR reset_value) Interrupt Disable Register */

#define AST_IDR_OVF_Pos             0            /**< \brief (AST_IDR) Overflow */
#define AST_IDR_OVF                 (0x1u << AST_IDR_OVF_Pos)
#define   AST_IDR_OVF_0             (0x0u <<  0) /**< \brief (AST_IDR) No effect */
#define   AST_IDR_OVF_1             (0x1u <<  0) /**< \brief (AST_IDR) Disable Interrupt. */
#define AST_IDR_ALARM0_Pos          8            /**< \brief (AST_IDR) Alarm 0 */
#define AST_IDR_ALARM0              (0x1u << AST_IDR_ALARM0_Pos)
#define   AST_IDR_ALARM0_0          (0x0u <<  8) /**< \brief (AST_IDR) No effect */
#define   AST_IDR_ALARM0_1          (0x1u <<  8) /**< \brief (AST_IDR) Disable interrupt */
#define AST_IDR_ALARM1_Pos          9            /**< \brief (AST_IDR) Alarm 1 */
#define AST_IDR_ALARM1              (0x1u << AST_IDR_ALARM1_Pos)
#define   AST_IDR_ALARM1_0          (0x0u <<  9) /**< \brief (AST_IDR) No effect */
#define   AST_IDR_ALARM1_1          (0x1u <<  9) /**< \brief (AST_IDR) Disable interrupt */
#define AST_IDR_PER0_Pos            16           /**< \brief (AST_IDR) Periodic 0 */
#define AST_IDR_PER0                (0x1u << AST_IDR_PER0_Pos)
#define   AST_IDR_PER0_0            (0x0u << 16) /**< \brief (AST_IDR) No effet */
#define   AST_IDR_PER0_1            (0x1u << 16) /**< \brief (AST_IDR) Disalbe interrupt */
#define AST_IDR_PER1_Pos            17           /**< \brief (AST_IDR) Periodic 1 */
#define AST_IDR_PER1                (0x1u << AST_IDR_PER1_Pos)
#define   AST_IDR_PER1_0            (0x0u << 17) /**< \brief (AST_IDR) No effect */
#define   AST_IDR_PER1_1            (0x1u << 17) /**< \brief (AST_IDR) Disable interrupt */
#define AST_IDR_READY_Pos           25           /**< \brief (AST_IDR) AST Ready */
#define AST_IDR_READY               (0x1u << AST_IDR_READY_Pos)
#define   AST_IDR_READY_0           (0x0u << 25) /**< \brief (AST_IDR) No effect */
#define   AST_IDR_READY_1           (0x1u << 25) /**< \brief (AST_IDR) Disable interrupt */
#define AST_IDR_CLKRDY_Pos          29           /**< \brief (AST_IDR) Clock Ready */
#define AST_IDR_CLKRDY              (0x1u << AST_IDR_CLKRDY_Pos)
#define   AST_IDR_CLKRDY_0          (0x0u << 29) /**< \brief (AST_IDR) No effect */
#define   AST_IDR_CLKRDY_1          (0x1u << 29) /**< \brief (AST_IDR) Disable interrupt */
#define AST_IDR_MASK                0x22030301u  /**< \brief (AST_IDR) MASK Register */

/* -------- AST_IMR : (AST Offset: 0x18) (R/  32) Interrupt Mask Register -------- */
#define AST_IMR_OFFSET              0x18         /**< \brief (AST_IMR offset) Interrupt Mask Register */
#define AST_IMR_RESETVALUE          0x00000000   /**< \brief (AST_IMR reset_value) Interrupt Mask Register */

#define AST_IMR_OVF_Pos             0            /**< \brief (AST_IMR) Overflow */
#define AST_IMR_OVF                 (0x1u << AST_IMR_OVF_Pos)
#define   AST_IMR_OVF_0             (0x0u <<  0) /**< \brief (AST_IMR) Interrupt is disabled */
#define   AST_IMR_OVF_1             (0x1u <<  0) /**< \brief (AST_IMR) Interrupt is enabled. */
#define AST_IMR_ALARM0_Pos          8            /**< \brief (AST_IMR) Alarm 0 */
#define AST_IMR_ALARM0              (0x1u << AST_IMR_ALARM0_Pos)
#define   AST_IMR_ALARM0_0          (0x0u <<  8) /**< \brief (AST_IMR) Interupt is disabled */
#define   AST_IMR_ALARM0_1          (0x1u <<  8) /**< \brief (AST_IMR) Interrupt is enabled */
#define AST_IMR_ALARM1_Pos          9            /**< \brief (AST_IMR) Alarm 1 */
#define AST_IMR_ALARM1              (0x1u << AST_IMR_ALARM1_Pos)
#define   AST_IMR_ALARM1_0          (0x0u <<  9) /**< \brief (AST_IMR) Interrupt is disabled */
#define   AST_IMR_ALARM1_1          (0x1u <<  9) /**< \brief (AST_IMR) Interrupt is enabled */
#define AST_IMR_PER0_Pos            16           /**< \brief (AST_IMR) Periodic 0 */
#define AST_IMR_PER0                (0x1u << AST_IMR_PER0_Pos)
#define   AST_IMR_PER0_0            (0x0u << 16) /**< \brief (AST_IMR) Interrupt is disabled */
#define   AST_IMR_PER0_1            (0x1u << 16) /**< \brief (AST_IMR) Interrupt is enabled */
#define AST_IMR_PER1_Pos            17           /**< \brief (AST_IMR) Periodic 1 */
#define AST_IMR_PER1                (0x1u << AST_IMR_PER1_Pos)
#define   AST_IMR_PER1_0            (0x0u << 17) /**< \brief (AST_IMR) Interrupt is disabled */
#define   AST_IMR_PER1_1            (0x1u << 17) /**< \brief (AST_IMR) Interrupt is enabled */
#define AST_IMR_READY_Pos           25           /**< \brief (AST_IMR) AST Ready */
#define AST_IMR_READY               (0x1u << AST_IMR_READY_Pos)
#define   AST_IMR_READY_0           (0x0u << 25) /**< \brief (AST_IMR) Interrupt is disabled */
#define   AST_IMR_READY_1           (0x1u << 25) /**< \brief (AST_IMR) Interrupt is enabled */
#define AST_IMR_CLKRDY_Pos          29           /**< \brief (AST_IMR) Clock Ready */
#define AST_IMR_CLKRDY              (0x1u << AST_IMR_CLKRDY_Pos)
#define   AST_IMR_CLKRDY_0          (0x0u << 29) /**< \brief (AST_IMR) Interrupt is disabled */
#define   AST_IMR_CLKRDY_1          (0x1u << 29) /**< \brief (AST_IMR) Interrupt is enabled */
#define AST_IMR_MASK                0x22030301u  /**< \brief (AST_IMR) MASK Register */

/* -------- AST_WER : (AST Offset: 0x1C) (R/W 32) Wake Enable Register -------- */
#define AST_WER_OFFSET              0x1C         /**< \brief (AST_WER offset) Wake Enable Register */
#define AST_WER_RESETVALUE          0x00000000   /**< \brief (AST_WER reset_value) Wake Enable Register */

#define AST_WER_OVF_Pos             0            /**< \brief (AST_WER) Overflow */
#define AST_WER_OVF                 (0x1u << AST_WER_OVF_Pos)
#define   AST_WER_OVF_0             (0x0u <<  0) /**< \brief (AST_WER) The corresponing event will not wake up the CPU from sleep mode */
#define   AST_WER_OVF_1             (0x1u <<  0) /**< \brief (AST_WER) The corresponding event will wake up the CPU from sleep mode */
#define AST_WER_ALARM0_Pos          8            /**< \brief (AST_WER) Alarm 0 */
#define AST_WER_ALARM0              (0x1u << AST_WER_ALARM0_Pos)
#define   AST_WER_ALARM0_0          (0x0u <<  8) /**< \brief (AST_WER) The corresponing event will not wake up the CPU from sleep mode */
#define   AST_WER_ALARM0_1          (0x1u <<  8) /**< \brief (AST_WER) The corresponding event will wake up the CPU from sleep mode */
#define AST_WER_ALARM1_Pos          9            /**< \brief (AST_WER) Alarm 1 */
#define AST_WER_ALARM1              (0x1u << AST_WER_ALARM1_Pos)
#define   AST_WER_ALARM1_0          (0x0u <<  9) /**< \brief (AST_WER) The corresponing event will not wake up the CPU from sleep mode */
#define   AST_WER_ALARM1_1          (0x1u <<  9) /**< \brief (AST_WER) The corresponding event will wake up the CPU from sleep mode */
#define AST_WER_PER0_Pos            16           /**< \brief (AST_WER) Periodic 0 */
#define AST_WER_PER0                (0x1u << AST_WER_PER0_Pos)
#define   AST_WER_PER0_0            (0x0u << 16) /**< \brief (AST_WER) The corresponing event will not wake up the CPU from sleep mode */
#define   AST_WER_PER0_1            (0x1u << 16) /**< \brief (AST_WER) The corresponding event will wake up the CPU from sleep mode */
#define AST_WER_PER1_Pos            17           /**< \brief (AST_WER) Periodic 1 */
#define AST_WER_PER1                (0x1u << AST_WER_PER1_Pos)
#define   AST_WER_PER1_0            (0x0u << 17) /**< \brief (AST_WER) The corresponing event will not wake up the CPU from sleep mode */
#define   AST_WER_PER1_1            (0x1u << 17) /**< \brief (AST_WER) The corresponding event will wake up the CPU from sleep mode */
#define AST_WER_MASK                0x00030301u  /**< \brief (AST_WER) MASK Register */

/* -------- AST_AR0 : (AST Offset: 0x20) (R/W 32) Alarm Register 0 -------- */
#define AST_AR0_OFFSET              0x20         /**< \brief (AST_AR0 offset) Alarm Register 0 */
#define AST_AR0_RESETVALUE          0x00000000   /**< \brief (AST_AR0 reset_value) Alarm Register 0 */

#define AST_AR0_VALUE_Pos           0            /**< \brief (AST_AR0) Alarm Value */
#define AST_AR0_VALUE_Msk           (0xFFFFFFFFu << AST_AR0_VALUE_Pos)
#define AST_AR0_VALUE(value)        ((AST_AR0_VALUE_Msk & ((value) << AST_AR0_VALUE_Pos)))
#define AST_AR0_MASK                0xFFFFFFFFu  /**< \brief (AST_AR0) MASK Register */

/* -------- AST_AR1 : (AST Offset: 0x24) (R/W 32) Alarm Register 1 -------- */
#define AST_AR1_OFFSET              0x24         /**< \brief (AST_AR1 offset) Alarm Register 1 */
#define AST_AR1_RESETVALUE          0x00000000   /**< \brief (AST_AR1 reset_value) Alarm Register 1 */

#define AST_AR1_VALUE_Pos           0            /**< \brief (AST_AR1) Alarm Value */
#define AST_AR1_VALUE_Msk           (0xFFFFFFFFu << AST_AR1_VALUE_Pos)
#define AST_AR1_VALUE(value)        ((AST_AR1_VALUE_Msk & ((value) << AST_AR1_VALUE_Pos)))
#define AST_AR1_MASK                0xFFFFFFFFu  /**< \brief (AST_AR1) MASK Register */

/* -------- AST_PIR0 : (AST Offset: 0x30) (R/W 32) Periodic Interval Register 0 -------- */
#define AST_PIR0_OFFSET             0x30         /**< \brief (AST_PIR0 offset) Periodic Interval Register 0 */
#define AST_PIR0_RESETVALUE         0x00000000   /**< \brief (AST_PIR0 reset_value) Periodic Interval Register 0 */

#define AST_PIR0_INSEL_Pos          0            /**< \brief (AST_PIR0) Interval Select */
#define AST_PIR0_INSEL_Msk          (0x1Fu << AST_PIR0_INSEL_Pos)
#define AST_PIR0_INSEL(value)       ((AST_PIR0_INSEL_Msk & ((value) << AST_PIR0_INSEL_Pos)))
#define AST_PIR0_MASK               0x0000001Fu  /**< \brief (AST_PIR0) MASK Register */

/* -------- AST_PIR1 : (AST Offset: 0x34) (R/W 32) Periodic Interval Register 1 -------- */
#define AST_PIR1_OFFSET             0x34         /**< \brief (AST_PIR1 offset) Periodic Interval Register 1 */
#define AST_PIR1_RESETVALUE         0x00000000   /**< \brief (AST_PIR1 reset_value) Periodic Interval Register 1 */

#define AST_PIR1_INSEL_Pos          0            /**< \brief (AST_PIR1) Interval Select */
#define AST_PIR1_INSEL_Msk          (0x1Fu << AST_PIR1_INSEL_Pos)
#define AST_PIR1_INSEL(value)       ((AST_PIR1_INSEL_Msk & ((value) << AST_PIR1_INSEL_Pos)))
#define AST_PIR1_MASK               0x0000001Fu  /**< \brief (AST_PIR1) MASK Register */

/* -------- AST_CLOCK : (AST Offset: 0x40) (R/W 32) Clock Control Register -------- */
#define AST_CLOCK_OFFSET            0x40         /**< \brief (AST_CLOCK offset) Clock Control Register */
#define AST_CLOCK_RESETVALUE        0x00000000   /**< \brief (AST_CLOCK reset_value) Clock Control Register */

#define AST_CLOCK_CEN_Pos           0            /**< \brief (AST_CLOCK) Clock Enable */
#define AST_CLOCK_CEN               (0x1u << AST_CLOCK_CEN_Pos)
#define   AST_CLOCK_CEN_0           (0x0u <<  0) /**< \brief (AST_CLOCK) The clock is disabled */
#define   AST_CLOCK_CEN_1           (0x1u <<  0) /**< \brief (AST_CLOCK) The clock is enabled */
#define AST_CLOCK_CSSEL_Pos         8            /**< \brief (AST_CLOCK) Clock Source Selection */
#define AST_CLOCK_CSSEL_Msk         (0x7u << AST_CLOCK_CSSEL_Pos)
#define AST_CLOCK_CSSEL(value)      ((AST_CLOCK_CSSEL_Msk & ((value) << AST_CLOCK_CSSEL_Pos)))
#define   AST_CLOCK_CSSEL_SLOWCLOCK (0x0u <<  8) /**< \brief (AST_CLOCK) Slow clock */
#define   AST_CLOCK_CSSEL_32KHZCLK  (0x1u <<  8) /**< \brief (AST_CLOCK) 32 kHz clock */
#define   AST_CLOCK_CSSEL_PBCLOCK   (0x2u <<  8) /**< \brief (AST_CLOCK) PB clock */
#define   AST_CLOCK_CSSEL_GCLK      (0x3u <<  8) /**< \brief (AST_CLOCK) Generic clock */
#define   AST_CLOCK_CSSEL_1KHZCLK   (0x4u <<  8) /**< \brief (AST_CLOCK) 1kHz clock from 32 kHz oscillator */
#define AST_CLOCK_MASK              0x00000701u  /**< \brief (AST_CLOCK) MASK Register */

/* -------- AST_DTR : (AST Offset: 0x44) (R/W 32) Digital Tuner Register -------- */
#define AST_DTR_OFFSET              0x44         /**< \brief (AST_DTR offset) Digital Tuner Register */
#define AST_DTR_RESETVALUE          0x00000000   /**< \brief (AST_DTR reset_value) Digital Tuner Register */

#define AST_DTR_EXP_Pos             0            /**< \brief (AST_DTR) EXP */
#define AST_DTR_EXP_Msk             (0x1Fu << AST_DTR_EXP_Pos)
#define AST_DTR_EXP(value)          ((AST_DTR_EXP_Msk & ((value) << AST_DTR_EXP_Pos)))
#define AST_DTR_ADD_Pos             5            /**< \brief (AST_DTR) ADD */
#define AST_DTR_ADD                 (0x1u << AST_DTR_ADD_Pos)
#define AST_DTR_VALUE_Pos           8            /**< \brief (AST_DTR) VALUE */
#define AST_DTR_VALUE_Msk           (0xFFu << AST_DTR_VALUE_Pos)
#define AST_DTR_VALUE(value)        ((AST_DTR_VALUE_Msk & ((value) << AST_DTR_VALUE_Pos)))
#define AST_DTR_MASK                0x0000FF3Fu  /**< \brief (AST_DTR) MASK Register */

/* -------- AST_EVE : (AST Offset: 0x48) ( /W 32) Event Enable Register -------- */
#define AST_EVE_OFFSET              0x48         /**< \brief (AST_EVE offset) Event Enable Register */
#define AST_EVE_RESETVALUE          0x00000000   /**< \brief (AST_EVE reset_value) Event Enable Register */

#define AST_EVE_OVF_Pos             0            /**< \brief (AST_EVE) Overflow */
#define AST_EVE_OVF                 (0x1u << AST_EVE_OVF_Pos)
#define AST_EVE_ALARM0_Pos          8            /**< \brief (AST_EVE) Alarm 0 */
#define AST_EVE_ALARM0              (0x1u << AST_EVE_ALARM0_Pos)
#define AST_EVE_ALARM1_Pos          9            /**< \brief (AST_EVE) Alarm 1 */
#define AST_EVE_ALARM1              (0x1u << AST_EVE_ALARM1_Pos)
#define AST_EVE_PER0_Pos            16           /**< \brief (AST_EVE) Perioidc 0 */
#define AST_EVE_PER0                (0x1u << AST_EVE_PER0_Pos)
#define AST_EVE_PER1_Pos            17           /**< \brief (AST_EVE) Periodic 1 */
#define AST_EVE_PER1                (0x1u << AST_EVE_PER1_Pos)
#define AST_EVE_MASK                0x00030301u  /**< \brief (AST_EVE) MASK Register */

/* -------- AST_EVD : (AST Offset: 0x4C) ( /W 32) Event Disable Register -------- */
#define AST_EVD_OFFSET              0x4C         /**< \brief (AST_EVD offset) Event Disable Register */
#define AST_EVD_RESETVALUE          0x00000000   /**< \brief (AST_EVD reset_value) Event Disable Register */

#define AST_EVD_OVF_Pos             0            /**< \brief (AST_EVD) Overflow */
#define AST_EVD_OVF                 (0x1u << AST_EVD_OVF_Pos)
#define AST_EVD_ALARM0_Pos          8            /**< \brief (AST_EVD) Alarm 0 */
#define AST_EVD_ALARM0              (0x1u << AST_EVD_ALARM0_Pos)
#define AST_EVD_ALARM1_Pos          9            /**< \brief (AST_EVD) Alarm 1 */
#define AST_EVD_ALARM1              (0x1u << AST_EVD_ALARM1_Pos)
#define AST_EVD_PER0_Pos            16           /**< \brief (AST_EVD) Perioidc 0 */
#define AST_EVD_PER0                (0x1u << AST_EVD_PER0_Pos)
#define AST_EVD_PER1_Pos            17           /**< \brief (AST_EVD) Periodic 1 */
#define AST_EVD_PER1                (0x1u << AST_EVD_PER1_Pos)
#define AST_EVD_MASK                0x00030301u  /**< \brief (AST_EVD) MASK Register */

/* -------- AST_EVM : (AST Offset: 0x50) (R/  32) Event Mask Register -------- */
#define AST_EVM_OFFSET              0x50         /**< \brief (AST_EVM offset) Event Mask Register */
#define AST_EVM_RESETVALUE          0x00000000   /**< \brief (AST_EVM reset_value) Event Mask Register */

#define AST_EVM_OVF_Pos             0            /**< \brief (AST_EVM) Overflow */
#define AST_EVM_OVF                 (0x1u << AST_EVM_OVF_Pos)
#define AST_EVM_ALARM0_Pos          8            /**< \brief (AST_EVM) Alarm 0 */
#define AST_EVM_ALARM0              (0x1u << AST_EVM_ALARM0_Pos)
#define AST_EVM_ALARM1_Pos          9            /**< \brief (AST_EVM) Alarm 1 */
#define AST_EVM_ALARM1              (0x1u << AST_EVM_ALARM1_Pos)
#define AST_EVM_PER0_Pos            16           /**< \brief (AST_EVM) Perioidc 0 */
#define AST_EVM_PER0                (0x1u << AST_EVM_PER0_Pos)
#define AST_EVM_PER1_Pos            17           /**< \brief (AST_EVM) Periodic 1 */
#define AST_EVM_PER1                (0x1u << AST_EVM_PER1_Pos)
#define AST_EVM_MASK                0x00030301u  /**< \brief (AST_EVM) MASK Register */

/* -------- AST_CALV : (AST Offset: 0x54) (R/W 32) Calendar Value -------- */
#define AST_CALV_OFFSET             0x54         /**< \brief (AST_CALV offset) Calendar Value */
#define AST_CALV_RESETVALUE         0x00000000   /**< \brief (AST_CALV reset_value) Calendar Value */

#define AST_CALV_SEC_Pos            0            /**< \brief (AST_CALV) Second */
#define AST_CALV_SEC_Msk            (0x3Fu << AST_CALV_SEC_Pos)
#define AST_CALV_SEC(value)         ((AST_CALV_SEC_Msk & ((value) << AST_CALV_SEC_Pos)))
#define AST_CALV_MIN_Pos            6            /**< \brief (AST_CALV) Minute */
#define AST_CALV_MIN_Msk            (0x3Fu << AST_CALV_MIN_Pos)
#define AST_CALV_MIN(value)         ((AST_CALV_MIN_Msk & ((value) << AST_CALV_MIN_Pos)))
#define AST_CALV_HOUR_Pos           12           /**< \brief (AST_CALV) Hour */
#define AST_CALV_HOUR_Msk           (0x1Fu << AST_CALV_HOUR_Pos)
#define AST_CALV_HOUR(value)        ((AST_CALV_HOUR_Msk & ((value) << AST_CALV_HOUR_Pos)))
#define AST_CALV_DAY_Pos            17           /**< \brief (AST_CALV) Day */
#define AST_CALV_DAY_Msk            (0x1Fu << AST_CALV_DAY_Pos)
#define AST_CALV_DAY(value)         ((AST_CALV_DAY_Msk & ((value) << AST_CALV_DAY_Pos)))
#define AST_CALV_MONTH_Pos          22           /**< \brief (AST_CALV) Month */
#define AST_CALV_MONTH_Msk          (0xFu << AST_CALV_MONTH_Pos)
#define AST_CALV_MONTH(value)       ((AST_CALV_MONTH_Msk & ((value) << AST_CALV_MONTH_Pos)))
#define AST_CALV_YEAR_Pos           26           /**< \brief (AST_CALV) Year */
#define AST_CALV_YEAR_Msk           (0x3Fu << AST_CALV_YEAR_Pos)
#define AST_CALV_YEAR(value)        ((AST_CALV_YEAR_Msk & ((value) << AST_CALV_YEAR_Pos)))
#define AST_CALV_MASK               0xFFFFFFFFu  /**< \brief (AST_CALV) MASK Register */

/* -------- AST_PARAMETER : (AST Offset: 0xF0) (R/  32) Parameter Register -------- */
#define AST_PARAMETER_OFFSET        0xF0         /**< \brief (AST_PARAMETER offset) Parameter Register */

#define AST_PARAMETER_DT_Pos        0            /**< \brief (AST_PARAMETER) Digital Tuner */
#define AST_PARAMETER_DT            (0x1u << AST_PARAMETER_DT_Pos)
#define   AST_PARAMETER_DT_OFF      (0x0u <<  0) /**< \brief (AST_PARAMETER) Digital tuner off */
#define   AST_PARAMETER_DT_ON       (0x1u <<  0) /**< \brief (AST_PARAMETER) Digital tuner on */
#define AST_PARAMETER_DTEXPWA_Pos   1            /**< \brief (AST_PARAMETER) Digital Tuner Exponent Writeable */
#define AST_PARAMETER_DTEXPWA       (0x1u << AST_PARAMETER_DTEXPWA_Pos)
#define   AST_PARAMETER_DTEXPWA_0   (0x0u <<  1) /**< \brief (AST_PARAMETER) Digital tuner exponent is a constant value. Writes to EXP bitfield in DTR will be discarded. */
#define   AST_PARAMETER_DTEXPWA_1   (0x1u <<  1) /**< \brief (AST_PARAMETER) Digital tuner exponent is chosen by writing to EXP bitfield in DTR */
#define AST_PARAMETER_DTEXPVALUE_Pos 2            /**< \brief (AST_PARAMETER) Digital Tuner Exponent Value */
#define AST_PARAMETER_DTEXPVALUE_Msk (0x1Fu << AST_PARAMETER_DTEXPVALUE_Pos)
#define AST_PARAMETER_DTEXPVALUE(value) ((AST_PARAMETER_DTEXPVALUE_Msk & ((value) << AST_PARAMETER_DTEXPVALUE_Pos)))
#define AST_PARAMETER_NUMAR_Pos     8            /**< \brief (AST_PARAMETER) Number of alarm comparators */
#define AST_PARAMETER_NUMAR_Msk     (0x3u << AST_PARAMETER_NUMAR_Pos)
#define AST_PARAMETER_NUMAR(value)  ((AST_PARAMETER_NUMAR_Msk & ((value) << AST_PARAMETER_NUMAR_Pos)))
#define   AST_PARAMETER_NUMAR_ZERO  (0x0u <<  8) /**< \brief (AST_PARAMETER) No alarm comparators */
#define   AST_PARAMETER_NUMAR_ONE   (0x1u <<  8) /**< \brief (AST_PARAMETER) One alarm comparator */
#define   AST_PARAMETER_NUMAR_TWO   (0x2u <<  8) /**< \brief (AST_PARAMETER) Two alarm comparators */
#define AST_PARAMETER_NUMPIR_Pos    12           /**< \brief (AST_PARAMETER) Number of periodic comparators */
#define AST_PARAMETER_NUMPIR        (0x1u << AST_PARAMETER_NUMPIR_Pos)
#define   AST_PARAMETER_NUMPIR_ONE  (0x0u << 12) /**< \brief (AST_PARAMETER) One periodic comparator */
#define   AST_PARAMETER_NUMPIR_TWO  (0x1u << 12) /**< \brief (AST_PARAMETER) Two periodic comparators */
#define AST_PARAMETER_PIR0WA_Pos    14           /**< \brief (AST_PARAMETER) Periodic Interval 0 Writeable */
#define AST_PARAMETER_PIR0WA        (0x1u << AST_PARAMETER_PIR0WA_Pos)
#define   AST_PARAMETER_PIR0WA_0    (0x0u << 14) /**< \brief (AST_PARAMETER) Periodic alarm prescaler 0 tapping is a constant value. Writes to INSEL bitfield in PIR0 will be discarded. */
#define   AST_PARAMETER_PIR0WA_1    (0x1u << 14) /**< \brief (AST_PARAMETER) Periodic alarm prescaler 0 tapping is chosen by writing to INSEL bitfield in PIR0 */
#define AST_PARAMETER_PIR1WA_Pos    15           /**< \brief (AST_PARAMETER) Periodic Interval 1 Writeable */
#define AST_PARAMETER_PIR1WA        (0x1u << AST_PARAMETER_PIR1WA_Pos)
#define   AST_PARAMETER_PIR1WA_0    (0x0u << 15) /**< \brief (AST_PARAMETER) Writes to PIR1 will be discarded */
#define   AST_PARAMETER_PIR1WA_1    (0x1u << 15) /**< \brief (AST_PARAMETER) PIR1 can be written */
#define AST_PARAMETER_PER0VALUE_Pos 16           /**< \brief (AST_PARAMETER) Periodic Interval 0 Value */
#define AST_PARAMETER_PER0VALUE_Msk (0x1Fu << AST_PARAMETER_PER0VALUE_Pos)
#define AST_PARAMETER_PER0VALUE(value) ((AST_PARAMETER_PER0VALUE_Msk & ((value) << AST_PARAMETER_PER0VALUE_Pos)))
#define AST_PARAMETER_PER1VALUE_Pos 24           /**< \brief (AST_PARAMETER) Periodic Interval 1 Value */
#define AST_PARAMETER_PER1VALUE_Msk (0x1Fu << AST_PARAMETER_PER1VALUE_Pos)
#define AST_PARAMETER_PER1VALUE(value) ((AST_PARAMETER_PER1VALUE_Msk & ((value) << AST_PARAMETER_PER1VALUE_Pos)))
#define AST_PARAMETER_MASK          0x1F1FD37Fu  /**< \brief (AST_PARAMETER) MASK Register */

/* -------- AST_VERSION : (AST Offset: 0xFC) (R/  32) Version Register -------- */
#define AST_VERSION_OFFSET          0xFC         /**< \brief (AST_VERSION offset) Version Register */
#define AST_VERSION_RESETVALUE      0x00000311   /**< \brief (AST_VERSION reset_value) Version Register */

#define AST_VERSION_VERSION_Pos     0            /**< \brief (AST_VERSION) Version Number */
#define AST_VERSION_VERSION_Msk     (0xFFFu << AST_VERSION_VERSION_Pos)
#define AST_VERSION_VERSION(value)  ((AST_VERSION_VERSION_Msk & ((value) << AST_VERSION_VERSION_Pos)))
#define AST_VERSION_VARIANT_Pos     16           /**< \brief (AST_VERSION) Variant Number */
#define AST_VERSION_VARIANT_Msk     (0xFu << AST_VERSION_VARIANT_Pos)
#define AST_VERSION_VARIANT(value)  ((AST_VERSION_VARIANT_Msk & ((value) << AST_VERSION_VARIANT_Pos)))
#define AST_VERSION_MASK            0x000F0FFFu  /**< \brief (AST_VERSION) MASK Register */

/** \brief AST hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
  RwReg   AST_CR;             /**< \brief (AST Offset: 0x00) Control Register */
  RwReg   AST_CV;             /**< \brief (AST Offset: 0x04) Counter Value */
  RoReg   AST_SR;             /**< \brief (AST Offset: 0x08) Status Register */
  WoReg   AST_SCR;            /**< \brief (AST Offset: 0x0C) Status Clear Register */
  WoReg   AST_IER;            /**< \brief (AST Offset: 0x10) Interrupt Enable Register */
  WoReg   AST_IDR;            /**< \brief (AST Offset: 0x14) Interrupt Disable Register */
  RoReg   AST_IMR;            /**< \brief (AST Offset: 0x18) Interrupt Mask Register */
  RwReg   AST_WER;            /**< \brief (AST Offset: 0x1C) Wake Enable Register */
  RwReg   AST_AR0;            /**< \brief (AST Offset: 0x20) Alarm Register 0 */
  RwReg   AST_AR1;            /**< \brief (AST Offset: 0x24) Alarm Register 1 */
  RoReg8  Reserved1[0x8];
  RwReg   AST_PIR0;           /**< \brief (AST Offset: 0x30) Periodic Interval Register 0 */
  RwReg   AST_PIR1;           /**< \brief (AST Offset: 0x34) Periodic Interval Register 1 */
  RoReg8  Reserved2[0x8];
  RwReg   AST_CLOCK;          /**< \brief (AST Offset: 0x40) Clock Control Register */
  RwReg   AST_DTR;            /**< \brief (AST Offset: 0x44) Digital Tuner Register */
  WoReg   AST_EVE;            /**< \brief (AST Offset: 0x48) Event Enable Register */
  WoReg   AST_EVD;            /**< \brief (AST Offset: 0x4C) Event Disable Register */
  RoReg   AST_EVM;            /**< \brief (AST Offset: 0x50) Event Mask Register */
  RwReg   AST_CALV;           /**< \brief (AST Offset: 0x54) Calendar Value */
  RoReg8  Reserved3[0x98];
  RoReg   AST_PARAMETER;      /**< \brief (AST Offset: 0xF0) Parameter Register */
  RoReg8  Reserved4[0x8];
  RoReg   AST_VERSION;        /**< \brief (AST Offset: 0xFC) Version Register */
} Ast;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/*@}*/

#endif /* _SAM4L_AST_COMPONENT_ */
