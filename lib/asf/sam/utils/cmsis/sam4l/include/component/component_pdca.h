/**
 * \file
 *
 * \brief Component description for PDCA
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

#ifndef _SAM4L_PDCA_COMPONENT_
#define _SAM4L_PDCA_COMPONENT_

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR PDCA */
/* ========================================================================== */
/** \addtogroup SAM4L_PDCA Peripheral DMA Controller */
/*@{*/

#define REV_PDCA                    0x124

/* -------- PDCA_MAR : (PDCA Offset: 0x000) (R/W 32) channel Memory Address Register -------- */
#define PDCA_MAR_OFFSET             0x000        /**< \brief (PDCA_MAR offset) Memory Address Register */
#define PDCA_MAR_RESETVALUE         0x00000000   /**< \brief (PDCA_MAR reset_value) Memory Address Register */

#define PDCA_MAR_MADDR_Pos          0            /**< \brief (PDCA_MAR) Memory Address */
#define PDCA_MAR_MADDR_Msk          (0xFFFFFFFFu << PDCA_MAR_MADDR_Pos)
#define PDCA_MAR_MADDR(value)       ((PDCA_MAR_MADDR_Msk & ((value) << PDCA_MAR_MADDR_Pos)))
#define PDCA_MAR_MASK               0xFFFFFFFFu  /**< \brief (PDCA_MAR) MASK Register */

/* -------- PDCA_PSR : (PDCA Offset: 0x004) (R/W 32) channel Peripheral Select Register -------- */
#define PDCA_PSR_OFFSET             0x004        /**< \brief (PDCA_PSR offset) Peripheral Select Register */

#define PDCA_PSR_PID_Pos            0            /**< \brief (PDCA_PSR) Peripheral Identifier */
#define PDCA_PSR_PID_Msk            (0xFFu << PDCA_PSR_PID_Pos)
#define PDCA_PSR_PID(value)         ((PDCA_PSR_PID_Msk & ((value) << PDCA_PSR_PID_Pos)))
#define PDCA_PSR_MASK               0x000000FFu  /**< \brief (PDCA_PSR) MASK Register */

/* -------- PDCA_TCR : (PDCA Offset: 0x008) (R/W 32) channel Transfer Counter Register -------- */
#define PDCA_TCR_OFFSET             0x008        /**< \brief (PDCA_TCR offset) Transfer Counter Register */
#define PDCA_TCR_RESETVALUE         0x00000000   /**< \brief (PDCA_TCR reset_value) Transfer Counter Register */

#define PDCA_TCR_TCV_Pos            0            /**< \brief (PDCA_TCR) Transfer Counter Value */
#define PDCA_TCR_TCV_Msk            (0xFFFFu << PDCA_TCR_TCV_Pos)
#define PDCA_TCR_TCV(value)         ((PDCA_TCR_TCV_Msk & ((value) << PDCA_TCR_TCV_Pos)))
#define PDCA_TCR_MASK               0x0000FFFFu  /**< \brief (PDCA_TCR) MASK Register */

/* -------- PDCA_MARR : (PDCA Offset: 0x00C) (R/W 32) channel Memory Address Reload Register -------- */
#define PDCA_MARR_OFFSET            0x00C        /**< \brief (PDCA_MARR offset) Memory Address Reload Register */
#define PDCA_MARR_RESETVALUE        0x00000000   /**< \brief (PDCA_MARR reset_value) Memory Address Reload Register */

#define PDCA_MARR_MARV_Pos          0            /**< \brief (PDCA_MARR) Memory Address Reload Value */
#define PDCA_MARR_MARV_Msk          (0xFFFFFFFFu << PDCA_MARR_MARV_Pos)
#define PDCA_MARR_MARV(value)       ((PDCA_MARR_MARV_Msk & ((value) << PDCA_MARR_MARV_Pos)))
#define PDCA_MARR_MASK              0xFFFFFFFFu  /**< \brief (PDCA_MARR) MASK Register */

/* -------- PDCA_TCRR : (PDCA Offset: 0x010) (R/W 32) channel Transfer Counter Reload Register -------- */
#define PDCA_TCRR_OFFSET            0x010        /**< \brief (PDCA_TCRR offset) Transfer Counter Reload Register */
#define PDCA_TCRR_RESETVALUE        0x00000000   /**< \brief (PDCA_TCRR reset_value) Transfer Counter Reload Register */

#define PDCA_TCRR_TCRV_Pos          0            /**< \brief (PDCA_TCRR) Transfer Counter Reload Value */
#define PDCA_TCRR_TCRV_Msk          (0xFFFFu << PDCA_TCRR_TCRV_Pos)
#define PDCA_TCRR_TCRV(value)       ((PDCA_TCRR_TCRV_Msk & ((value) << PDCA_TCRR_TCRV_Pos)))
#define PDCA_TCRR_MASK              0x0000FFFFu  /**< \brief (PDCA_TCRR) MASK Register */

/* -------- PDCA_CR : (PDCA Offset: 0x014) ( /W 32) channel Control Register -------- */
#define PDCA_CR_OFFSET              0x014        /**< \brief (PDCA_CR offset) Control Register */
#define PDCA_CR_RESETVALUE          0x00000000   /**< \brief (PDCA_CR reset_value) Control Register */

#define PDCA_CR_TEN_Pos             0            /**< \brief (PDCA_CR) Transfer Enable */
#define PDCA_CR_TEN                 (0x1u << PDCA_CR_TEN_Pos)
#define PDCA_CR_TDIS_Pos            1            /**< \brief (PDCA_CR) Transfer Disable */
#define PDCA_CR_TDIS                (0x1u << PDCA_CR_TDIS_Pos)
#define PDCA_CR_ECLR_Pos            8            /**< \brief (PDCA_CR) Error Clear */
#define PDCA_CR_ECLR                (0x1u << PDCA_CR_ECLR_Pos)
#define PDCA_CR_MASK                0x00000103u  /**< \brief (PDCA_CR) MASK Register */

/* -------- PDCA_MR : (PDCA Offset: 0x018) (R/W 32) channel Mode Register -------- */
#define PDCA_MR_OFFSET              0x018        /**< \brief (PDCA_MR offset) Mode Register */
#define PDCA_MR_RESETVALUE          0x00000000   /**< \brief (PDCA_MR reset_value) Mode Register */

#define PDCA_MR_SIZE_Pos            0            /**< \brief (PDCA_MR) Transfer size */
#define PDCA_MR_SIZE_Msk            (0x3u << PDCA_MR_SIZE_Pos)
#define PDCA_MR_SIZE(value)         ((PDCA_MR_SIZE_Msk & ((value) << PDCA_MR_SIZE_Pos)))
#define   PDCA_MR_SIZE_BYTE         (0x0u <<  0) /**< \brief (PDCA_MR)  */
#define   PDCA_MR_SIZE_HALF_WORD    (0x1u <<  0) /**< \brief (PDCA_MR)  */
#define   PDCA_MR_SIZE_WORD         (0x2u <<  0) /**< \brief (PDCA_MR)  */
#define PDCA_MR_ETRIG_Pos           2            /**< \brief (PDCA_MR) Event trigger */
#define PDCA_MR_ETRIG               (0x1u << PDCA_MR_ETRIG_Pos)
#define PDCA_MR_RING_Pos            3            /**< \brief (PDCA_MR) Ring Buffer */
#define PDCA_MR_RING                (0x1u << PDCA_MR_RING_Pos)
#define PDCA_MR_MASK                0x0000000Fu  /**< \brief (PDCA_MR) MASK Register */

/* -------- PDCA_SR : (PDCA Offset: 0x01C) (R/  32) channel Status Register -------- */
#define PDCA_SR_OFFSET              0x01C        /**< \brief (PDCA_SR offset) Status Register */
#define PDCA_SR_RESETVALUE          0x00000000   /**< \brief (PDCA_SR reset_value) Status Register */

#define PDCA_SR_TEN_Pos             0            /**< \brief (PDCA_SR) Transfer Enabled */
#define PDCA_SR_TEN                 (0x1u << PDCA_SR_TEN_Pos)
#define PDCA_SR_MASK                0x00000001u  /**< \brief (PDCA_SR) MASK Register */

/* -------- PDCA_IER : (PDCA Offset: 0x020) ( /W 32) channel Interrupt Enable Register -------- */
#define PDCA_IER_OFFSET             0x020        /**< \brief (PDCA_IER offset) Interrupt Enable Register */
#define PDCA_IER_RESETVALUE         0x00000000   /**< \brief (PDCA_IER reset_value) Interrupt Enable Register */

#define PDCA_IER_RCZ_Pos            0            /**< \brief (PDCA_IER) Reload Counter Zero */
#define PDCA_IER_RCZ                (0x1u << PDCA_IER_RCZ_Pos)
#define PDCA_IER_TRC_Pos            1            /**< \brief (PDCA_IER) Transfer Complete */
#define PDCA_IER_TRC                (0x1u << PDCA_IER_TRC_Pos)
#define PDCA_IER_TERR_Pos           2            /**< \brief (PDCA_IER) Transfer Error */
#define PDCA_IER_TERR               (0x1u << PDCA_IER_TERR_Pos)
#define PDCA_IER_MASK               0x00000007u  /**< \brief (PDCA_IER) MASK Register */

/* -------- PDCA_IDR : (PDCA Offset: 0x024) ( /W 32) channel Interrupt Disable Register -------- */
#define PDCA_IDR_OFFSET             0x024        /**< \brief (PDCA_IDR offset) Interrupt Disable Register */
#define PDCA_IDR_RESETVALUE         0x00000000   /**< \brief (PDCA_IDR reset_value) Interrupt Disable Register */

#define PDCA_IDR_RCZ_Pos            0            /**< \brief (PDCA_IDR) Reload Counter Zero */
#define PDCA_IDR_RCZ                (0x1u << PDCA_IDR_RCZ_Pos)
#define PDCA_IDR_TRC_Pos            1            /**< \brief (PDCA_IDR) Transfer Complete */
#define PDCA_IDR_TRC                (0x1u << PDCA_IDR_TRC_Pos)
#define PDCA_IDR_TERR_Pos           2            /**< \brief (PDCA_IDR) Transfer Error */
#define PDCA_IDR_TERR               (0x1u << PDCA_IDR_TERR_Pos)
#define PDCA_IDR_MASK               0x00000007u  /**< \brief (PDCA_IDR) MASK Register */

/* -------- PDCA_IMR : (PDCA Offset: 0x028) (R/  32) channel Interrupt Mask Register -------- */
#define PDCA_IMR_OFFSET             0x028        /**< \brief (PDCA_IMR offset) Interrupt Mask Register */
#define PDCA_IMR_RESETVALUE         0x00000000   /**< \brief (PDCA_IMR reset_value) Interrupt Mask Register */

#define PDCA_IMR_RCZ_Pos            0            /**< \brief (PDCA_IMR) Reload Counter Zero */
#define PDCA_IMR_RCZ                (0x1u << PDCA_IMR_RCZ_Pos)
#define PDCA_IMR_TRC_Pos            1            /**< \brief (PDCA_IMR) Transfer Complete */
#define PDCA_IMR_TRC                (0x1u << PDCA_IMR_TRC_Pos)
#define PDCA_IMR_TERR_Pos           2            /**< \brief (PDCA_IMR) Transfer Error */
#define PDCA_IMR_TERR               (0x1u << PDCA_IMR_TERR_Pos)
#define PDCA_IMR_MASK               0x00000007u  /**< \brief (PDCA_IMR) MASK Register */

/* -------- PDCA_ISR : (PDCA Offset: 0x02C) (R/  32) channel Interrupt Status Register -------- */
#define PDCA_ISR_OFFSET             0x02C        /**< \brief (PDCA_ISR offset) Interrupt Status Register */
#define PDCA_ISR_RESETVALUE         0x00000000   /**< \brief (PDCA_ISR reset_value) Interrupt Status Register */

#define PDCA_ISR_RCZ_Pos            0            /**< \brief (PDCA_ISR) Reload Counter Zero */
#define PDCA_ISR_RCZ                (0x1u << PDCA_ISR_RCZ_Pos)
#define PDCA_ISR_TRC_Pos            1            /**< \brief (PDCA_ISR) Transfer Complete */
#define PDCA_ISR_TRC                (0x1u << PDCA_ISR_TRC_Pos)
#define PDCA_ISR_TERR_Pos           2            /**< \brief (PDCA_ISR) Transfer Error */
#define PDCA_ISR_TERR               (0x1u << PDCA_ISR_TERR_Pos)
#define PDCA_ISR_MASK               0x00000007u  /**< \brief (PDCA_ISR) MASK Register */

/* -------- PDCA_PCONTROL : (PDCA Offset: 0x800) (R/W 32) Performance Control Register -------- */
#define PDCA_PCONTROL_OFFSET        0x800        /**< \brief (PDCA_PCONTROL offset) Performance Control Register */
#define PDCA_PCONTROL_RESETVALUE    0x00000000   /**< \brief (PDCA_PCONTROL reset_value) Performance Control Register */

#define PDCA_PCONTROL_CH0EN_Pos     0            /**< \brief (PDCA_PCONTROL) Channel 0 Enabled */
#define PDCA_PCONTROL_CH0EN         (0x1u << PDCA_PCONTROL_CH0EN_Pos)
#define PDCA_PCONTROL_CH1EN_Pos     1            /**< \brief (PDCA_PCONTROL) Channel 1 Enabled. */
#define PDCA_PCONTROL_CH1EN         (0x1u << PDCA_PCONTROL_CH1EN_Pos)
#define PDCA_PCONTROL_CH0OF_Pos     4            /**< \brief (PDCA_PCONTROL) Channel 0 Overflow Freeze */
#define PDCA_PCONTROL_CH0OF         (0x1u << PDCA_PCONTROL_CH0OF_Pos)
#define PDCA_PCONTROL_CH1OF_Pos     5            /**< \brief (PDCA_PCONTROL) Channel 1 overflow freeze */
#define PDCA_PCONTROL_CH1OF         (0x1u << PDCA_PCONTROL_CH1OF_Pos)
#define PDCA_PCONTROL_CH0RES_Pos    8            /**< \brief (PDCA_PCONTROL) Channel 0 counter reset */
#define PDCA_PCONTROL_CH0RES        (0x1u << PDCA_PCONTROL_CH0RES_Pos)
#define PDCA_PCONTROL_CH1RES_Pos    9            /**< \brief (PDCA_PCONTROL) Channel 1 counter reset */
#define PDCA_PCONTROL_CH1RES        (0x1u << PDCA_PCONTROL_CH1RES_Pos)
#define PDCA_PCONTROL_MON0CH_Pos    16           /**< \brief (PDCA_PCONTROL) PDCA Channel to monitor with counter 0 */
#define PDCA_PCONTROL_MON0CH_Msk    (0x3Fu << PDCA_PCONTROL_MON0CH_Pos)
#define PDCA_PCONTROL_MON0CH(value) ((PDCA_PCONTROL_MON0CH_Msk & ((value) << PDCA_PCONTROL_MON0CH_Pos)))
#define PDCA_PCONTROL_MON1CH_Pos    24           /**< \brief (PDCA_PCONTROL) PDCA Channel to monitor with counter 1 */
#define PDCA_PCONTROL_MON1CH_Msk    (0x3Fu << PDCA_PCONTROL_MON1CH_Pos)
#define PDCA_PCONTROL_MON1CH(value) ((PDCA_PCONTROL_MON1CH_Msk & ((value) << PDCA_PCONTROL_MON1CH_Pos)))
#define PDCA_PCONTROL_MASK          0x3F3F0333u  /**< \brief (PDCA_PCONTROL) MASK Register */

/* -------- PDCA_PRDATA0 : (PDCA Offset: 0x804) (R/  32) Channel 0 Read Data Cycles -------- */
#define PDCA_PRDATA0_OFFSET         0x804        /**< \brief (PDCA_PRDATA0 offset) Channel 0 Read Data Cycles */
#define PDCA_PRDATA0_RESETVALUE     0x00000000   /**< \brief (PDCA_PRDATA0 reset_value) Channel 0 Read Data Cycles */

#define PDCA_PRDATA0_DATA_Pos       0            /**< \brief (PDCA_PRDATA0) Data Cycles Counted Since Last reset */
#define PDCA_PRDATA0_DATA_Msk       (0xFFFFFFFFu << PDCA_PRDATA0_DATA_Pos)
#define PDCA_PRDATA0_DATA(value)    ((PDCA_PRDATA0_DATA_Msk & ((value) << PDCA_PRDATA0_DATA_Pos)))
#define PDCA_PRDATA0_MASK           0xFFFFFFFFu  /**< \brief (PDCA_PRDATA0) MASK Register */

/* -------- PDCA_PRSTALL0 : (PDCA Offset: 0x808) (R/  32) Channel 0 Read Stall Cycles -------- */
#define PDCA_PRSTALL0_OFFSET        0x808        /**< \brief (PDCA_PRSTALL0 offset) Channel 0 Read Stall Cycles */
#define PDCA_PRSTALL0_RESETVALUE    0x00000000   /**< \brief (PDCA_PRSTALL0 reset_value) Channel 0 Read Stall Cycles */

#define PDCA_PRSTALL0_STALL_Pos     0            /**< \brief (PDCA_PRSTALL0) Stall Cycles counted since last reset */
#define PDCA_PRSTALL0_STALL_Msk     (0xFFFFFFFFu << PDCA_PRSTALL0_STALL_Pos)
#define PDCA_PRSTALL0_STALL(value)  ((PDCA_PRSTALL0_STALL_Msk & ((value) << PDCA_PRSTALL0_STALL_Pos)))
#define PDCA_PRSTALL0_MASK          0xFFFFFFFFu  /**< \brief (PDCA_PRSTALL0) MASK Register */

/* -------- PDCA_PRLAT0 : (PDCA Offset: 0x80C) (R/  32) Channel 0 Read Max Latency -------- */
#define PDCA_PRLAT0_OFFSET          0x80C        /**< \brief (PDCA_PRLAT0 offset) Channel 0 Read Max Latency */
#define PDCA_PRLAT0_RESETVALUE      0x00000000   /**< \brief (PDCA_PRLAT0 reset_value) Channel 0 Read Max Latency */

#define PDCA_PRLAT0_LAT_Pos         0            /**< \brief (PDCA_PRLAT0) Maximum Transfer Initiation cycles counted since last reset */
#define PDCA_PRLAT0_LAT_Msk         (0xFFFFu << PDCA_PRLAT0_LAT_Pos)
#define PDCA_PRLAT0_LAT(value)      ((PDCA_PRLAT0_LAT_Msk & ((value) << PDCA_PRLAT0_LAT_Pos)))
#define PDCA_PRLAT0_MASK            0x0000FFFFu  /**< \brief (PDCA_PRLAT0) MASK Register */

/* -------- PDCA_PWDATA0 : (PDCA Offset: 0x810) (R/  32) Channel 0 Write Data Cycles -------- */
#define PDCA_PWDATA0_OFFSET         0x810        /**< \brief (PDCA_PWDATA0 offset) Channel 0 Write Data Cycles */
#define PDCA_PWDATA0_RESETVALUE     0x00000000   /**< \brief (PDCA_PWDATA0 reset_value) Channel 0 Write Data Cycles */

#define PDCA_PWDATA0_DATA_Pos       0            /**< \brief (PDCA_PWDATA0) Data Cycles Counted since last Reset */
#define PDCA_PWDATA0_DATA_Msk       (0xFFFFFFFFu << PDCA_PWDATA0_DATA_Pos)
#define PDCA_PWDATA0_DATA(value)    ((PDCA_PWDATA0_DATA_Msk & ((value) << PDCA_PWDATA0_DATA_Pos)))
#define PDCA_PWDATA0_MASK           0xFFFFFFFFu  /**< \brief (PDCA_PWDATA0) MASK Register */

/* -------- PDCA_PWSTALL0 : (PDCA Offset: 0x814) (R/  32) Channel 0 Write Stall Cycles -------- */
#define PDCA_PWSTALL0_OFFSET        0x814        /**< \brief (PDCA_PWSTALL0 offset) Channel 0 Write Stall Cycles */
#define PDCA_PWSTALL0_RESETVALUE    0x00000000   /**< \brief (PDCA_PWSTALL0 reset_value) Channel 0 Write Stall Cycles */

#define PDCA_PWSTALL0_STALL_Pos     0            /**< \brief (PDCA_PWSTALL0) Stall cycles counted since last reset */
#define PDCA_PWSTALL0_STALL_Msk     (0xFFFFFFFFu << PDCA_PWSTALL0_STALL_Pos)
#define PDCA_PWSTALL0_STALL(value)  ((PDCA_PWSTALL0_STALL_Msk & ((value) << PDCA_PWSTALL0_STALL_Pos)))
#define PDCA_PWSTALL0_MASK          0xFFFFFFFFu  /**< \brief (PDCA_PWSTALL0) MASK Register */

/* -------- PDCA_PWLAT0 : (PDCA Offset: 0x818) (R/  32) Channel0 Write Max Latency -------- */
#define PDCA_PWLAT0_OFFSET          0x818        /**< \brief (PDCA_PWLAT0 offset) Channel0 Write Max Latency */
#define PDCA_PWLAT0_RESETVALUE      0x00000000   /**< \brief (PDCA_PWLAT0 reset_value) Channel0 Write Max Latency */

#define PDCA_PWLAT0_LAT_Pos         0            /**< \brief (PDCA_PWLAT0) Maximum transfer initiation cycles counted since last reset */
#define PDCA_PWLAT0_LAT_Msk         (0xFFFFu << PDCA_PWLAT0_LAT_Pos)
#define PDCA_PWLAT0_LAT(value)      ((PDCA_PWLAT0_LAT_Msk & ((value) << PDCA_PWLAT0_LAT_Pos)))
#define PDCA_PWLAT0_MASK            0x0000FFFFu  /**< \brief (PDCA_PWLAT0) MASK Register */

/* -------- PDCA_PRDATA1 : (PDCA Offset: 0x81C) (R/  32) Channel 1 Read Data Cycles -------- */
#define PDCA_PRDATA1_OFFSET         0x81C        /**< \brief (PDCA_PRDATA1 offset) Channel 1 Read Data Cycles */
#define PDCA_PRDATA1_RESETVALUE     0x00000000   /**< \brief (PDCA_PRDATA1 reset_value) Channel 1 Read Data Cycles */

#define PDCA_PRDATA1_DATA_Pos       0            /**< \brief (PDCA_PRDATA1) Data Cycles Counted Since Last reset */
#define PDCA_PRDATA1_DATA_Msk       (0xFFFFFFFFu << PDCA_PRDATA1_DATA_Pos)
#define PDCA_PRDATA1_DATA(value)    ((PDCA_PRDATA1_DATA_Msk & ((value) << PDCA_PRDATA1_DATA_Pos)))
#define PDCA_PRDATA1_MASK           0xFFFFFFFFu  /**< \brief (PDCA_PRDATA1) MASK Register */

/* -------- PDCA_PRSTALL1 : (PDCA Offset: 0x820) (R/  32) Channel Read Stall Cycles -------- */
#define PDCA_PRSTALL1_OFFSET        0x820        /**< \brief (PDCA_PRSTALL1 offset) Channel Read Stall Cycles */
#define PDCA_PRSTALL1_RESETVALUE    0x00000000   /**< \brief (PDCA_PRSTALL1 reset_value) Channel Read Stall Cycles */

#define PDCA_PRSTALL1_STALL_Pos     0            /**< \brief (PDCA_PRSTALL1) Stall Cycles Counted since last reset */
#define PDCA_PRSTALL1_STALL_Msk     (0xFFFFFFFFu << PDCA_PRSTALL1_STALL_Pos)
#define PDCA_PRSTALL1_STALL(value)  ((PDCA_PRSTALL1_STALL_Msk & ((value) << PDCA_PRSTALL1_STALL_Pos)))
#define PDCA_PRSTALL1_MASK          0xFFFFFFFFu  /**< \brief (PDCA_PRSTALL1) MASK Register */

/* -------- PDCA_PRLAT1 : (PDCA Offset: 0x824) (R/  32) Channel 1 Read Max Latency -------- */
#define PDCA_PRLAT1_OFFSET          0x824        /**< \brief (PDCA_PRLAT1 offset) Channel 1 Read Max Latency */
#define PDCA_PRLAT1_RESETVALUE      0x00000000   /**< \brief (PDCA_PRLAT1 reset_value) Channel 1 Read Max Latency */

#define PDCA_PRLAT1_LAT_Pos         0            /**< \brief (PDCA_PRLAT1) Maximum Transfer initiation cycles counted since last reset */
#define PDCA_PRLAT1_LAT_Msk         (0xFFFFu << PDCA_PRLAT1_LAT_Pos)
#define PDCA_PRLAT1_LAT(value)      ((PDCA_PRLAT1_LAT_Msk & ((value) << PDCA_PRLAT1_LAT_Pos)))
#define PDCA_PRLAT1_MASK            0x0000FFFFu  /**< \brief (PDCA_PRLAT1) MASK Register */

/* -------- PDCA_PWDATA1 : (PDCA Offset: 0x828) (R/  32) Channel 1 Write Data Cycles -------- */
#define PDCA_PWDATA1_OFFSET         0x828        /**< \brief (PDCA_PWDATA1 offset) Channel 1 Write Data Cycles */
#define PDCA_PWDATA1_RESETVALUE     0x00000000   /**< \brief (PDCA_PWDATA1 reset_value) Channel 1 Write Data Cycles */

#define PDCA_PWDATA1_DATA_Pos       0            /**< \brief (PDCA_PWDATA1) Data cycles Counted Since last reset */
#define PDCA_PWDATA1_DATA_Msk       (0xFFFFFFFFu << PDCA_PWDATA1_DATA_Pos)
#define PDCA_PWDATA1_DATA(value)    ((PDCA_PWDATA1_DATA_Msk & ((value) << PDCA_PWDATA1_DATA_Pos)))
#define PDCA_PWDATA1_MASK           0xFFFFFFFFu  /**< \brief (PDCA_PWDATA1) MASK Register */

/* -------- PDCA_PWSTALL1 : (PDCA Offset: 0x82C) (R/  32) Channel 1 Write stall Cycles -------- */
#define PDCA_PWSTALL1_OFFSET        0x82C        /**< \brief (PDCA_PWSTALL1 offset) Channel 1 Write stall Cycles */
#define PDCA_PWSTALL1_RESETVALUE    0x00000000   /**< \brief (PDCA_PWSTALL1 reset_value) Channel 1 Write stall Cycles */

#define PDCA_PWSTALL1_STALL_Pos     0            /**< \brief (PDCA_PWSTALL1) Stall cycles counted since last reset */
#define PDCA_PWSTALL1_STALL_Msk     (0xFFFFFFFFu << PDCA_PWSTALL1_STALL_Pos)
#define PDCA_PWSTALL1_STALL(value)  ((PDCA_PWSTALL1_STALL_Msk & ((value) << PDCA_PWSTALL1_STALL_Pos)))
#define PDCA_PWSTALL1_MASK          0xFFFFFFFFu  /**< \brief (PDCA_PWSTALL1) MASK Register */

/* -------- PDCA_PWLAT1 : (PDCA Offset: 0x830) (R/  32) Channel 1 Read Max Latency -------- */
#define PDCA_PWLAT1_OFFSET          0x830        /**< \brief (PDCA_PWLAT1 offset) Channel 1 Read Max Latency */
#define PDCA_PWLAT1_RESETVALUE      0x00000000   /**< \brief (PDCA_PWLAT1 reset_value) Channel 1 Read Max Latency */

#define PDCA_PWLAT1_LAT_Pos         0            /**< \brief (PDCA_PWLAT1) Maximum transfer initiation cycles counted since last reset */
#define PDCA_PWLAT1_LAT_Msk         (0xFFFFu << PDCA_PWLAT1_LAT_Pos)
#define PDCA_PWLAT1_LAT(value)      ((PDCA_PWLAT1_LAT_Msk & ((value) << PDCA_PWLAT1_LAT_Pos)))
#define PDCA_PWLAT1_MASK            0x0000FFFFu  /**< \brief (PDCA_PWLAT1) MASK Register */

/* -------- PDCA_VERSION : (PDCA Offset: 0x834) (R/  32) Version Register -------- */
#define PDCA_VERSION_OFFSET         0x834        /**< \brief (PDCA_VERSION offset) Version Register */
#define PDCA_VERSION_RESETVALUE     0x00000124   /**< \brief (PDCA_VERSION reset_value) Version Register */

#define PDCA_VERSION_VERSION_Pos    0            /**< \brief (PDCA_VERSION) Version Number */
#define PDCA_VERSION_VERSION_Msk    (0xFFFu << PDCA_VERSION_VERSION_Pos)
#define PDCA_VERSION_VERSION(value) ((PDCA_VERSION_VERSION_Msk & ((value) << PDCA_VERSION_VERSION_Pos)))
#define PDCA_VERSION_VARIANT_Pos    16           /**< \brief (PDCA_VERSION) Variant Number */
#define PDCA_VERSION_VARIANT_Msk    (0xFu << PDCA_VERSION_VARIANT_Pos)
#define PDCA_VERSION_VARIANT(value) ((PDCA_VERSION_VARIANT_Msk & ((value) << PDCA_VERSION_VARIANT_Pos)))
#define PDCA_VERSION_MASK           0x000F0FFFu  /**< \brief (PDCA_VERSION) MASK Register */

/** \brief PdcaChannel hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
  RwReg   PDCA_MAR;           /**< \brief (PDCA Offset: 0x000) Memory Address Register */
  RwReg   PDCA_PSR;           /**< \brief (PDCA Offset: 0x004) Peripheral Select Register */
  RwReg   PDCA_TCR;           /**< \brief (PDCA Offset: 0x008) Transfer Counter Register */
  RwReg   PDCA_MARR;          /**< \brief (PDCA Offset: 0x00C) Memory Address Reload Register */
  RwReg   PDCA_TCRR;          /**< \brief (PDCA Offset: 0x010) Transfer Counter Reload Register */
  WoReg   PDCA_CR;            /**< \brief (PDCA Offset: 0x014) Control Register */
  RwReg   PDCA_MR;            /**< \brief (PDCA Offset: 0x018) Mode Register */
  RoReg   PDCA_SR;            /**< \brief (PDCA Offset: 0x01C) Status Register */
  WoReg   PDCA_IER;           /**< \brief (PDCA Offset: 0x020) Interrupt Enable Register */
  WoReg   PDCA_IDR;           /**< \brief (PDCA Offset: 0x024) Interrupt Disable Register */
  RoReg   PDCA_IMR;           /**< \brief (PDCA Offset: 0x028) Interrupt Mask Register */
  RoReg   PDCA_ISR;           /**< \brief (PDCA Offset: 0x02C) Interrupt Status Register */
  RoReg8  Reserved1[0x10];
} PdcaChannel;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/** \brief PDCA hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
  PdcaChannel PDCA_CHANNEL[16];   /**< \brief (PDCA Offset: 0x000) PdcaChannel groups [CHANNEL_LENGTH] */
  RoReg8  Reserved1[0x400];
  RwReg   PDCA_PCONTROL;      /**< \brief (PDCA Offset: 0x800) Performance Control Register */
  RoReg   PDCA_PRDATA0;       /**< \brief (PDCA Offset: 0x804) Channel 0 Read Data Cycles */
  RoReg   PDCA_PRSTALL0;      /**< \brief (PDCA Offset: 0x808) Channel 0 Read Stall Cycles */
  RoReg   PDCA_PRLAT0;        /**< \brief (PDCA Offset: 0x80C) Channel 0 Read Max Latency */
  RoReg   PDCA_PWDATA0;       /**< \brief (PDCA Offset: 0x810) Channel 0 Write Data Cycles */
  RoReg   PDCA_PWSTALL0;      /**< \brief (PDCA Offset: 0x814) Channel 0 Write Stall Cycles */
  RoReg   PDCA_PWLAT0;        /**< \brief (PDCA Offset: 0x818) Channel0 Write Max Latency */
  RoReg   PDCA_PRDATA1;       /**< \brief (PDCA Offset: 0x81C) Channel 1 Read Data Cycles */
  RoReg   PDCA_PRSTALL1;      /**< \brief (PDCA Offset: 0x820) Channel Read Stall Cycles */
  RoReg   PDCA_PRLAT1;        /**< \brief (PDCA Offset: 0x824) Channel 1 Read Max Latency */
  RoReg   PDCA_PWDATA1;       /**< \brief (PDCA Offset: 0x828) Channel 1 Write Data Cycles */
  RoReg   PDCA_PWSTALL1;      /**< \brief (PDCA Offset: 0x82C) Channel 1 Write stall Cycles */
  RoReg   PDCA_PWLAT1;        /**< \brief (PDCA Offset: 0x830) Channel 1 Read Max Latency */
  RoReg   PDCA_VERSION;       /**< \brief (PDCA Offset: 0x834) Version Register */
} Pdca;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/*@}*/

#endif /* _SAM4L_PDCA_COMPONENT_ */
