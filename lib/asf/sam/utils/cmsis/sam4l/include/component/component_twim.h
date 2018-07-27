/**
 * \file
 *
 * \brief Component description for TWIM
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

#ifndef _SAM4L_TWIM_COMPONENT_
#define _SAM4L_TWIM_COMPONENT_

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR TWIM */
/* ========================================================================== */
/** \addtogroup SAM4L_TWIM Two-wire Master Interface */
/*@{*/

#define REV_TWIM                    0x120

/* -------- TWIM_CR : (TWIM Offset: 0x00) ( /W 32) Control Register -------- */
#define TWIM_CR_OFFSET              0x00         /**< \brief (TWIM_CR offset) Control Register */
#define TWIM_CR_RESETVALUE          0x00000000   /**< \brief (TWIM_CR reset_value) Control Register */

#define TWIM_CR_MEN_Pos             0            /**< \brief (TWIM_CR) Master Enable */
#define TWIM_CR_MEN                 (0x1u << TWIM_CR_MEN_Pos)
#define TWIM_CR_MDIS_Pos            1            /**< \brief (TWIM_CR) Master Disable */
#define TWIM_CR_MDIS                (0x1u << TWIM_CR_MDIS_Pos)
#define TWIM_CR_SMEN_Pos            4            /**< \brief (TWIM_CR) SMBus Enable */
#define TWIM_CR_SMEN                (0x1u << TWIM_CR_SMEN_Pos)
#define TWIM_CR_SMDIS_Pos           5            /**< \brief (TWIM_CR) SMBus Disable */
#define TWIM_CR_SMDIS               (0x1u << TWIM_CR_SMDIS_Pos)
#define TWIM_CR_SWRST_Pos           7            /**< \brief (TWIM_CR) Software Reset */
#define TWIM_CR_SWRST               (0x1u << TWIM_CR_SWRST_Pos)
#define TWIM_CR_STOP_Pos            8            /**< \brief (TWIM_CR) Stop the current transfer */
#define TWIM_CR_STOP                (0x1u << TWIM_CR_STOP_Pos)
#define TWIM_CR_MASK                0x000001B3u  /**< \brief (TWIM_CR) MASK Register */

/* -------- TWIM_CWGR : (TWIM Offset: 0x04) (R/W 32) Clock Waveform Generator Register -------- */
#define TWIM_CWGR_OFFSET            0x04         /**< \brief (TWIM_CWGR offset) Clock Waveform Generator Register */
#define TWIM_CWGR_RESETVALUE        0x00000000   /**< \brief (TWIM_CWGR reset_value) Clock Waveform Generator Register */

#define TWIM_CWGR_LOW_Pos           0            /**< \brief (TWIM_CWGR) Clock Low Cycles */
#define TWIM_CWGR_LOW_Msk           (0xFFu << TWIM_CWGR_LOW_Pos)
#define TWIM_CWGR_LOW(value)        ((TWIM_CWGR_LOW_Msk & ((value) << TWIM_CWGR_LOW_Pos)))
#define TWIM_CWGR_HIGH_Pos          8            /**< \brief (TWIM_CWGR) Clock High Cycles */
#define TWIM_CWGR_HIGH_Msk          (0xFFu << TWIM_CWGR_HIGH_Pos)
#define TWIM_CWGR_HIGH(value)       ((TWIM_CWGR_HIGH_Msk & ((value) << TWIM_CWGR_HIGH_Pos)))
#define TWIM_CWGR_STASTO_Pos        16           /**< \brief (TWIM_CWGR) START and STOP Cycles */
#define TWIM_CWGR_STASTO_Msk        (0xFFu << TWIM_CWGR_STASTO_Pos)
#define TWIM_CWGR_STASTO(value)     ((TWIM_CWGR_STASTO_Msk & ((value) << TWIM_CWGR_STASTO_Pos)))
#define TWIM_CWGR_DATA_Pos          24           /**< \brief (TWIM_CWGR) Data Setup and Hold Cycles */
#define TWIM_CWGR_DATA_Msk          (0xFu << TWIM_CWGR_DATA_Pos)
#define TWIM_CWGR_DATA(value)       ((TWIM_CWGR_DATA_Msk & ((value) << TWIM_CWGR_DATA_Pos)))
#define TWIM_CWGR_EXP_Pos           28           /**< \brief (TWIM_CWGR) Clock Prescaler */
#define TWIM_CWGR_EXP_Msk           (0x7u << TWIM_CWGR_EXP_Pos)
#define TWIM_CWGR_EXP(value)        ((TWIM_CWGR_EXP_Msk & ((value) << TWIM_CWGR_EXP_Pos)))
#define TWIM_CWGR_MASK              0x7FFFFFFFu  /**< \brief (TWIM_CWGR) MASK Register */

/* -------- TWIM_SMBTR : (TWIM Offset: 0x08) (R/W 32) SMBus Timing Register -------- */
#define TWIM_SMBTR_OFFSET           0x08         /**< \brief (TWIM_SMBTR offset) SMBus Timing Register */
#define TWIM_SMBTR_RESETVALUE       0x00000000   /**< \brief (TWIM_SMBTR reset_value) SMBus Timing Register */

#define TWIM_SMBTR_TLOWS_Pos        0            /**< \brief (TWIM_SMBTR) Slave Clock stretch maximum cycles */
#define TWIM_SMBTR_TLOWS_Msk        (0xFFu << TWIM_SMBTR_TLOWS_Pos)
#define TWIM_SMBTR_TLOWS(value)     ((TWIM_SMBTR_TLOWS_Msk & ((value) << TWIM_SMBTR_TLOWS_Pos)))
#define TWIM_SMBTR_TLOWM_Pos        8            /**< \brief (TWIM_SMBTR) Master Clock stretch maximum cycles */
#define TWIM_SMBTR_TLOWM_Msk        (0xFFu << TWIM_SMBTR_TLOWM_Pos)
#define TWIM_SMBTR_TLOWM(value)     ((TWIM_SMBTR_TLOWM_Msk & ((value) << TWIM_SMBTR_TLOWM_Pos)))
#define TWIM_SMBTR_THMAX_Pos        16           /**< \brief (TWIM_SMBTR) Clock High maximum cycles */
#define TWIM_SMBTR_THMAX_Msk        (0xFFu << TWIM_SMBTR_THMAX_Pos)
#define TWIM_SMBTR_THMAX(value)     ((TWIM_SMBTR_THMAX_Msk & ((value) << TWIM_SMBTR_THMAX_Pos)))
#define TWIM_SMBTR_EXP_Pos          28           /**< \brief (TWIM_SMBTR) SMBus Timeout Clock prescaler */
#define TWIM_SMBTR_EXP_Msk          (0xFu << TWIM_SMBTR_EXP_Pos)
#define TWIM_SMBTR_EXP(value)       ((TWIM_SMBTR_EXP_Msk & ((value) << TWIM_SMBTR_EXP_Pos)))
#define TWIM_SMBTR_MASK             0xF0FFFFFFu  /**< \brief (TWIM_SMBTR) MASK Register */

/* -------- TWIM_CMDR : (TWIM Offset: 0x0C) (R/W 32) Command Register -------- */
#define TWIM_CMDR_OFFSET            0x0C         /**< \brief (TWIM_CMDR offset) Command Register */
#define TWIM_CMDR_RESETVALUE        0x00000000   /**< \brief (TWIM_CMDR reset_value) Command Register */

#define TWIM_CMDR_READ_Pos          0            /**< \brief (TWIM_CMDR) Transfer Direction */
#define TWIM_CMDR_READ              (0x1u << TWIM_CMDR_READ_Pos)
#define TWIM_CMDR_SADR_Pos          1            /**< \brief (TWIM_CMDR) Slave Address */
#define TWIM_CMDR_SADR_Msk          (0x3FFu << TWIM_CMDR_SADR_Pos)
#define TWIM_CMDR_SADR(value)       ((TWIM_CMDR_SADR_Msk & ((value) << TWIM_CMDR_SADR_Pos)))
#define TWIM_CMDR_TENBIT_Pos        11           /**< \brief (TWIM_CMDR) Ten Bit Addressing Mode */
#define TWIM_CMDR_TENBIT            (0x1u << TWIM_CMDR_TENBIT_Pos)
#define TWIM_CMDR_REPSAME_Pos       12           /**< \brief (TWIM_CMDR) Transfer is to same address as previous address */
#define TWIM_CMDR_REPSAME           (0x1u << TWIM_CMDR_REPSAME_Pos)
#define TWIM_CMDR_START_Pos         13           /**< \brief (TWIM_CMDR) Send START condition */
#define TWIM_CMDR_START             (0x1u << TWIM_CMDR_START_Pos)
#define TWIM_CMDR_STOP_Pos          14           /**< \brief (TWIM_CMDR) Send STOP condition */
#define TWIM_CMDR_STOP              (0x1u << TWIM_CMDR_STOP_Pos)
#define TWIM_CMDR_VALID_Pos         15           /**< \brief (TWIM_CMDR) CMDR Valid */
#define TWIM_CMDR_VALID             (0x1u << TWIM_CMDR_VALID_Pos)
#define TWIM_CMDR_NBYTES_Pos        16           /**< \brief (TWIM_CMDR) Number of data bytes in transfer */
#define TWIM_CMDR_NBYTES_Msk        (0xFFu << TWIM_CMDR_NBYTES_Pos)
#define TWIM_CMDR_NBYTES(value)     ((TWIM_CMDR_NBYTES_Msk & ((value) << TWIM_CMDR_NBYTES_Pos)))
#define TWIM_CMDR_PECEN_Pos         24           /**< \brief (TWIM_CMDR) Packet Error Checking Enable */
#define TWIM_CMDR_PECEN             (0x1u << TWIM_CMDR_PECEN_Pos)
#define TWIM_CMDR_ACKLAST_Pos       25           /**< \brief (TWIM_CMDR) ACK Last Master RX Byte */
#define TWIM_CMDR_ACKLAST           (0x1u << TWIM_CMDR_ACKLAST_Pos)
#define TWIM_CMDR_HS_Pos            26           /**< \brief (TWIM_CMDR) HS-mode */
#define TWIM_CMDR_HS                (0x1u << TWIM_CMDR_HS_Pos)
#define TWIM_CMDR_HSMCODE_Pos       28           /**< \brief (TWIM_CMDR) HS-mode Master Code */
#define TWIM_CMDR_HSMCODE_Msk       (0x7u << TWIM_CMDR_HSMCODE_Pos)
#define TWIM_CMDR_HSMCODE(value)    ((TWIM_CMDR_HSMCODE_Msk & ((value) << TWIM_CMDR_HSMCODE_Pos)))
#define TWIM_CMDR_MASK              0x77FFFFFFu  /**< \brief (TWIM_CMDR) MASK Register */

/* -------- TWIM_NCMDR : (TWIM Offset: 0x10) (R/W 32) Next Command Register -------- */
#define TWIM_NCMDR_OFFSET           0x10         /**< \brief (TWIM_NCMDR offset) Next Command Register */
#define TWIM_NCMDR_RESETVALUE       0x00000000   /**< \brief (TWIM_NCMDR reset_value) Next Command Register */

#define TWIM_NCMDR_READ_Pos         0            /**< \brief (TWIM_NCMDR) Transfer Direction */
#define TWIM_NCMDR_READ             (0x1u << TWIM_NCMDR_READ_Pos)
#define TWIM_NCMDR_SADR_Pos         1            /**< \brief (TWIM_NCMDR) Slave Address */
#define TWIM_NCMDR_SADR_Msk         (0x3FFu << TWIM_NCMDR_SADR_Pos)
#define TWIM_NCMDR_SADR(value)      ((TWIM_NCMDR_SADR_Msk & ((value) << TWIM_NCMDR_SADR_Pos)))
#define TWIM_NCMDR_TENBIT_Pos       11           /**< \brief (TWIM_NCMDR) Ten Bit Addressing Mode */
#define TWIM_NCMDR_TENBIT           (0x1u << TWIM_NCMDR_TENBIT_Pos)
#define TWIM_NCMDR_REPSAME_Pos      12           /**< \brief (TWIM_NCMDR) Transfer is to same address as previous address */
#define TWIM_NCMDR_REPSAME          (0x1u << TWIM_NCMDR_REPSAME_Pos)
#define TWIM_NCMDR_START_Pos        13           /**< \brief (TWIM_NCMDR) Send START condition */
#define TWIM_NCMDR_START            (0x1u << TWIM_NCMDR_START_Pos)
#define TWIM_NCMDR_STOP_Pos         14           /**< \brief (TWIM_NCMDR) Send STOP condition */
#define TWIM_NCMDR_STOP             (0x1u << TWIM_NCMDR_STOP_Pos)
#define TWIM_NCMDR_VALID_Pos        15           /**< \brief (TWIM_NCMDR) CMDR Valid */
#define TWIM_NCMDR_VALID            (0x1u << TWIM_NCMDR_VALID_Pos)
#define TWIM_NCMDR_NBYTES_Pos       16           /**< \brief (TWIM_NCMDR) Number of data bytes in transfer */
#define TWIM_NCMDR_NBYTES_Msk       (0xFFu << TWIM_NCMDR_NBYTES_Pos)
#define TWIM_NCMDR_NBYTES(value)    ((TWIM_NCMDR_NBYTES_Msk & ((value) << TWIM_NCMDR_NBYTES_Pos)))
#define TWIM_NCMDR_PECEN_Pos        24           /**< \brief (TWIM_NCMDR) Packet Error Checking Enable */
#define TWIM_NCMDR_PECEN            (0x1u << TWIM_NCMDR_PECEN_Pos)
#define TWIM_NCMDR_ACKLAST_Pos      25           /**< \brief (TWIM_NCMDR) ACK Last Master RX Byte */
#define TWIM_NCMDR_ACKLAST          (0x1u << TWIM_NCMDR_ACKLAST_Pos)
#define TWIM_NCMDR_HS_Pos           26           /**< \brief (TWIM_NCMDR) HS-mode */
#define TWIM_NCMDR_HS               (0x1u << TWIM_NCMDR_HS_Pos)
#define TWIM_NCMDR_HSMCODE_Pos      28           /**< \brief (TWIM_NCMDR) HS-mode Master Code */
#define TWIM_NCMDR_HSMCODE_Msk      (0x7u << TWIM_NCMDR_HSMCODE_Pos)
#define TWIM_NCMDR_HSMCODE(value)   ((TWIM_NCMDR_HSMCODE_Msk & ((value) << TWIM_NCMDR_HSMCODE_Pos)))
#define TWIM_NCMDR_MASK             0x77FFFFFFu  /**< \brief (TWIM_NCMDR) MASK Register */

/* -------- TWIM_RHR : (TWIM Offset: 0x14) (R/  32) Receive Holding Register -------- */
#define TWIM_RHR_OFFSET             0x14         /**< \brief (TWIM_RHR offset) Receive Holding Register */
#define TWIM_RHR_RESETVALUE         0x00000000   /**< \brief (TWIM_RHR reset_value) Receive Holding Register */

#define TWIM_RHR_RXDATA_Pos         0            /**< \brief (TWIM_RHR) Received Data */
#define TWIM_RHR_RXDATA_Msk         (0xFFu << TWIM_RHR_RXDATA_Pos)
#define TWIM_RHR_RXDATA(value)      ((TWIM_RHR_RXDATA_Msk & ((value) << TWIM_RHR_RXDATA_Pos)))
#define TWIM_RHR_MASK               0x000000FFu  /**< \brief (TWIM_RHR) MASK Register */

/* -------- TWIM_THR : (TWIM Offset: 0x18) ( /W 32) Transmit Holding Register -------- */
#define TWIM_THR_OFFSET             0x18         /**< \brief (TWIM_THR offset) Transmit Holding Register */
#define TWIM_THR_RESETVALUE         0x00000000   /**< \brief (TWIM_THR reset_value) Transmit Holding Register */

#define TWIM_THR_TXDATA_Pos         0            /**< \brief (TWIM_THR) Data to Transmit */
#define TWIM_THR_TXDATA_Msk         (0xFFu << TWIM_THR_TXDATA_Pos)
#define TWIM_THR_TXDATA(value)      ((TWIM_THR_TXDATA_Msk & ((value) << TWIM_THR_TXDATA_Pos)))
#define TWIM_THR_MASK               0x000000FFu  /**< \brief (TWIM_THR) MASK Register */

/* -------- TWIM_SR : (TWIM Offset: 0x1C) (R/  32) Status Register -------- */
#define TWIM_SR_OFFSET              0x1C         /**< \brief (TWIM_SR offset) Status Register */
#define TWIM_SR_RESETVALUE          0x00000002   /**< \brief (TWIM_SR reset_value) Status Register */

#define TWIM_SR_RXRDY_Pos           0            /**< \brief (TWIM_SR) RHR Data Ready */
#define TWIM_SR_RXRDY               (0x1u << TWIM_SR_RXRDY_Pos)
#define TWIM_SR_TXRDY_Pos           1            /**< \brief (TWIM_SR) THR Data Ready */
#define TWIM_SR_TXRDY               (0x1u << TWIM_SR_TXRDY_Pos)
#define TWIM_SR_CRDY_Pos            2            /**< \brief (TWIM_SR) Ready for More Commands */
#define TWIM_SR_CRDY                (0x1u << TWIM_SR_CRDY_Pos)
#define TWIM_SR_CCOMP_Pos           3            /**< \brief (TWIM_SR) Command Complete */
#define TWIM_SR_CCOMP               (0x1u << TWIM_SR_CCOMP_Pos)
#define TWIM_SR_IDLE_Pos            4            /**< \brief (TWIM_SR) Master Interface is Idle */
#define TWIM_SR_IDLE                (0x1u << TWIM_SR_IDLE_Pos)
#define TWIM_SR_BUSFREE_Pos         5            /**< \brief (TWIM_SR) Two-wire Bus is Free */
#define TWIM_SR_BUSFREE             (0x1u << TWIM_SR_BUSFREE_Pos)
#define TWIM_SR_ANAK_Pos            8            /**< \brief (TWIM_SR) NAK in Address Phase Received */
#define TWIM_SR_ANAK                (0x1u << TWIM_SR_ANAK_Pos)
#define TWIM_SR_DNAK_Pos            9            /**< \brief (TWIM_SR) NAK in Data Phase Received */
#define TWIM_SR_DNAK                (0x1u << TWIM_SR_DNAK_Pos)
#define TWIM_SR_ARBLST_Pos          10           /**< \brief (TWIM_SR) Arbitration Lost */
#define TWIM_SR_ARBLST              (0x1u << TWIM_SR_ARBLST_Pos)
#define TWIM_SR_SMBALERT_Pos        11           /**< \brief (TWIM_SR) SMBus Alert */
#define TWIM_SR_SMBALERT            (0x1u << TWIM_SR_SMBALERT_Pos)
#define TWIM_SR_TOUT_Pos            12           /**< \brief (TWIM_SR) Timeout */
#define TWIM_SR_TOUT                (0x1u << TWIM_SR_TOUT_Pos)
#define TWIM_SR_PECERR_Pos          13           /**< \brief (TWIM_SR) PEC Error */
#define TWIM_SR_PECERR              (0x1u << TWIM_SR_PECERR_Pos)
#define TWIM_SR_STOP_Pos            14           /**< \brief (TWIM_SR) Stop Request Accepted */
#define TWIM_SR_STOP                (0x1u << TWIM_SR_STOP_Pos)
#define TWIM_SR_MENB_Pos            16           /**< \brief (TWIM_SR) Master Interface Enable */
#define TWIM_SR_MENB                (0x1u << TWIM_SR_MENB_Pos)
#define TWIM_SR_HSMCACK_Pos         17           /**< \brief (TWIM_SR) ACK in HS-mode Master Code Phase Received */
#define TWIM_SR_HSMCACK             (0x1u << TWIM_SR_HSMCACK_Pos)
#define TWIM_SR_MASK                0x00037F3Fu  /**< \brief (TWIM_SR) MASK Register */

/* -------- TWIM_IER : (TWIM Offset: 0x20) ( /W 32) Interrupt Enable Register -------- */
#define TWIM_IER_OFFSET             0x20         /**< \brief (TWIM_IER offset) Interrupt Enable Register */
#define TWIM_IER_RESETVALUE         0x00000000   /**< \brief (TWIM_IER reset_value) Interrupt Enable Register */

#define TWIM_IER_RXRDY_Pos          0            /**< \brief (TWIM_IER) RHR Data Ready */
#define TWIM_IER_RXRDY              (0x1u << TWIM_IER_RXRDY_Pos)
#define TWIM_IER_TXRDY_Pos          1            /**< \brief (TWIM_IER) THR Data Ready */
#define TWIM_IER_TXRDY              (0x1u << TWIM_IER_TXRDY_Pos)
#define TWIM_IER_CRDY_Pos           2            /**< \brief (TWIM_IER) Ready for More Commands */
#define TWIM_IER_CRDY               (0x1u << TWIM_IER_CRDY_Pos)
#define TWIM_IER_CCOMP_Pos          3            /**< \brief (TWIM_IER) Command Complete */
#define TWIM_IER_CCOMP              (0x1u << TWIM_IER_CCOMP_Pos)
#define TWIM_IER_IDLE_Pos           4            /**< \brief (TWIM_IER) Master Interface is Idle */
#define TWIM_IER_IDLE               (0x1u << TWIM_IER_IDLE_Pos)
#define TWIM_IER_BUSFREE_Pos        5            /**< \brief (TWIM_IER) Two-wire Bus is Free */
#define TWIM_IER_BUSFREE            (0x1u << TWIM_IER_BUSFREE_Pos)
#define TWIM_IER_ANAK_Pos           8            /**< \brief (TWIM_IER) NAK in Address Phase Received */
#define TWIM_IER_ANAK               (0x1u << TWIM_IER_ANAK_Pos)
#define TWIM_IER_DNAK_Pos           9            /**< \brief (TWIM_IER) NAK in Data Phase Received */
#define TWIM_IER_DNAK               (0x1u << TWIM_IER_DNAK_Pos)
#define TWIM_IER_ARBLST_Pos         10           /**< \brief (TWIM_IER) Arbitration Lost */
#define TWIM_IER_ARBLST             (0x1u << TWIM_IER_ARBLST_Pos)
#define TWIM_IER_SMBALERT_Pos       11           /**< \brief (TWIM_IER) SMBus Alert */
#define TWIM_IER_SMBALERT           (0x1u << TWIM_IER_SMBALERT_Pos)
#define TWIM_IER_TOUT_Pos           12           /**< \brief (TWIM_IER) Timeout */
#define TWIM_IER_TOUT               (0x1u << TWIM_IER_TOUT_Pos)
#define TWIM_IER_PECERR_Pos         13           /**< \brief (TWIM_IER) PEC Error */
#define TWIM_IER_PECERR             (0x1u << TWIM_IER_PECERR_Pos)
#define TWIM_IER_STOP_Pos           14           /**< \brief (TWIM_IER) Stop Request Accepted */
#define TWIM_IER_STOP               (0x1u << TWIM_IER_STOP_Pos)
#define TWIM_IER_HSMCACK_Pos        17           /**< \brief (TWIM_IER) ACK in HS-mode Master Code Phase Received */
#define TWIM_IER_HSMCACK            (0x1u << TWIM_IER_HSMCACK_Pos)
#define TWIM_IER_MASK               0x00027F3Fu  /**< \brief (TWIM_IER) MASK Register */

/* -------- TWIM_IDR : (TWIM Offset: 0x24) ( /W 32) Interrupt Disable Register -------- */
#define TWIM_IDR_OFFSET             0x24         /**< \brief (TWIM_IDR offset) Interrupt Disable Register */
#define TWIM_IDR_RESETVALUE         0x00000000   /**< \brief (TWIM_IDR reset_value) Interrupt Disable Register */

#define TWIM_IDR_RXRDY_Pos          0            /**< \brief (TWIM_IDR) RHR Data Ready */
#define TWIM_IDR_RXRDY              (0x1u << TWIM_IDR_RXRDY_Pos)
#define TWIM_IDR_TXRDY_Pos          1            /**< \brief (TWIM_IDR) THR Data Ready */
#define TWIM_IDR_TXRDY              (0x1u << TWIM_IDR_TXRDY_Pos)
#define TWIM_IDR_CRDY_Pos           2            /**< \brief (TWIM_IDR) Ready for More Commands */
#define TWIM_IDR_CRDY               (0x1u << TWIM_IDR_CRDY_Pos)
#define TWIM_IDR_CCOMP_Pos          3            /**< \brief (TWIM_IDR) Command Complete */
#define TWIM_IDR_CCOMP              (0x1u << TWIM_IDR_CCOMP_Pos)
#define TWIM_IDR_IDLE_Pos           4            /**< \brief (TWIM_IDR) Master Interface is Idle */
#define TWIM_IDR_IDLE               (0x1u << TWIM_IDR_IDLE_Pos)
#define TWIM_IDR_BUSFREE_Pos        5            /**< \brief (TWIM_IDR) Two-wire Bus is Free */
#define TWIM_IDR_BUSFREE            (0x1u << TWIM_IDR_BUSFREE_Pos)
#define TWIM_IDR_ANAK_Pos           8            /**< \brief (TWIM_IDR) NAK in Address Phase Received */
#define TWIM_IDR_ANAK               (0x1u << TWIM_IDR_ANAK_Pos)
#define TWIM_IDR_DNAK_Pos           9            /**< \brief (TWIM_IDR) NAK in Data Phase Received */
#define TWIM_IDR_DNAK               (0x1u << TWIM_IDR_DNAK_Pos)
#define TWIM_IDR_ARBLST_Pos         10           /**< \brief (TWIM_IDR) Arbitration Lost */
#define TWIM_IDR_ARBLST             (0x1u << TWIM_IDR_ARBLST_Pos)
#define TWIM_IDR_SMBALERT_Pos       11           /**< \brief (TWIM_IDR) SMBus Alert */
#define TWIM_IDR_SMBALERT           (0x1u << TWIM_IDR_SMBALERT_Pos)
#define TWIM_IDR_TOUT_Pos           12           /**< \brief (TWIM_IDR) Timeout */
#define TWIM_IDR_TOUT               (0x1u << TWIM_IDR_TOUT_Pos)
#define TWIM_IDR_PECERR_Pos         13           /**< \brief (TWIM_IDR) PEC Error */
#define TWIM_IDR_PECERR             (0x1u << TWIM_IDR_PECERR_Pos)
#define TWIM_IDR_STOP_Pos           14           /**< \brief (TWIM_IDR) Stop Request Accepted */
#define TWIM_IDR_STOP               (0x1u << TWIM_IDR_STOP_Pos)
#define TWIM_IDR_HSMCACK_Pos        17           /**< \brief (TWIM_IDR) ACK in HS-mode Master Code Phase Received */
#define TWIM_IDR_HSMCACK            (0x1u << TWIM_IDR_HSMCACK_Pos)
#define TWIM_IDR_MASK               0x00027F3Fu  /**< \brief (TWIM_IDR) MASK Register */

/* -------- TWIM_IMR : (TWIM Offset: 0x28) (R/  32) Interrupt Mask Register -------- */
#define TWIM_IMR_OFFSET             0x28         /**< \brief (TWIM_IMR offset) Interrupt Mask Register */
#define TWIM_IMR_RESETVALUE         0x00000000   /**< \brief (TWIM_IMR reset_value) Interrupt Mask Register */

#define TWIM_IMR_RXRDY_Pos          0            /**< \brief (TWIM_IMR) RHR Data Ready */
#define TWIM_IMR_RXRDY              (0x1u << TWIM_IMR_RXRDY_Pos)
#define TWIM_IMR_TXRDY_Pos          1            /**< \brief (TWIM_IMR) THR Data Ready */
#define TWIM_IMR_TXRDY              (0x1u << TWIM_IMR_TXRDY_Pos)
#define TWIM_IMR_CRDY_Pos           2            /**< \brief (TWIM_IMR) Ready for More Commands */
#define TWIM_IMR_CRDY               (0x1u << TWIM_IMR_CRDY_Pos)
#define TWIM_IMR_CCOMP_Pos          3            /**< \brief (TWIM_IMR) Command Complete */
#define TWIM_IMR_CCOMP              (0x1u << TWIM_IMR_CCOMP_Pos)
#define TWIM_IMR_IDLE_Pos           4            /**< \brief (TWIM_IMR) Master Interface is Idle */
#define TWIM_IMR_IDLE               (0x1u << TWIM_IMR_IDLE_Pos)
#define TWIM_IMR_BUSFREE_Pos        5            /**< \brief (TWIM_IMR) Two-wire Bus is Free */
#define TWIM_IMR_BUSFREE            (0x1u << TWIM_IMR_BUSFREE_Pos)
#define TWIM_IMR_ANAK_Pos           8            /**< \brief (TWIM_IMR) NAK in Address Phase Received */
#define TWIM_IMR_ANAK               (0x1u << TWIM_IMR_ANAK_Pos)
#define TWIM_IMR_DNAK_Pos           9            /**< \brief (TWIM_IMR) NAK in Data Phase Received */
#define TWIM_IMR_DNAK               (0x1u << TWIM_IMR_DNAK_Pos)
#define TWIM_IMR_ARBLST_Pos         10           /**< \brief (TWIM_IMR) Arbitration Lost */
#define TWIM_IMR_ARBLST             (0x1u << TWIM_IMR_ARBLST_Pos)
#define TWIM_IMR_SMBALERT_Pos       11           /**< \brief (TWIM_IMR) SMBus Alert */
#define TWIM_IMR_SMBALERT           (0x1u << TWIM_IMR_SMBALERT_Pos)
#define TWIM_IMR_TOUT_Pos           12           /**< \brief (TWIM_IMR) Timeout */
#define TWIM_IMR_TOUT               (0x1u << TWIM_IMR_TOUT_Pos)
#define TWIM_IMR_PECERR_Pos         13           /**< \brief (TWIM_IMR) PEC Error */
#define TWIM_IMR_PECERR             (0x1u << TWIM_IMR_PECERR_Pos)
#define TWIM_IMR_STOP_Pos           14           /**< \brief (TWIM_IMR) Stop Request Accepted */
#define TWIM_IMR_STOP               (0x1u << TWIM_IMR_STOP_Pos)
#define TWIM_IMR_HSMCACK_Pos        17           /**< \brief (TWIM_IMR) ACK in HS-mode Master Code Phase Received */
#define TWIM_IMR_HSMCACK            (0x1u << TWIM_IMR_HSMCACK_Pos)
#define TWIM_IMR_MASK               0x00027F3Fu  /**< \brief (TWIM_IMR) MASK Register */

/* -------- TWIM_SCR : (TWIM Offset: 0x2C) ( /W 32) Status Clear Register -------- */
#define TWIM_SCR_OFFSET             0x2C         /**< \brief (TWIM_SCR offset) Status Clear Register */
#define TWIM_SCR_RESETVALUE         0x00000000   /**< \brief (TWIM_SCR reset_value) Status Clear Register */

#define TWIM_SCR_CCOMP_Pos          3            /**< \brief (TWIM_SCR) Command Complete */
#define TWIM_SCR_CCOMP              (0x1u << TWIM_SCR_CCOMP_Pos)
#define TWIM_SCR_ANAK_Pos           8            /**< \brief (TWIM_SCR) NAK in Address Phase Received */
#define TWIM_SCR_ANAK               (0x1u << TWIM_SCR_ANAK_Pos)
#define TWIM_SCR_DNAK_Pos           9            /**< \brief (TWIM_SCR) NAK in Data Phase Received */
#define TWIM_SCR_DNAK               (0x1u << TWIM_SCR_DNAK_Pos)
#define TWIM_SCR_ARBLST_Pos         10           /**< \brief (TWIM_SCR) Arbitration Lost */
#define TWIM_SCR_ARBLST             (0x1u << TWIM_SCR_ARBLST_Pos)
#define TWIM_SCR_SMBALERT_Pos       11           /**< \brief (TWIM_SCR) SMBus Alert */
#define TWIM_SCR_SMBALERT           (0x1u << TWIM_SCR_SMBALERT_Pos)
#define TWIM_SCR_TOUT_Pos           12           /**< \brief (TWIM_SCR) Timeout */
#define TWIM_SCR_TOUT               (0x1u << TWIM_SCR_TOUT_Pos)
#define TWIM_SCR_PECERR_Pos         13           /**< \brief (TWIM_SCR) PEC Error */
#define TWIM_SCR_PECERR             (0x1u << TWIM_SCR_PECERR_Pos)
#define TWIM_SCR_STOP_Pos           14           /**< \brief (TWIM_SCR) Stop Request Accepted */
#define TWIM_SCR_STOP               (0x1u << TWIM_SCR_STOP_Pos)
#define TWIM_SCR_HSMCACK_Pos        17           /**< \brief (TWIM_SCR) ACK in HS-mode Master Code Phase Received */
#define TWIM_SCR_HSMCACK            (0x1u << TWIM_SCR_HSMCACK_Pos)
#define TWIM_SCR_MASK               0x00027F08u  /**< \brief (TWIM_SCR) MASK Register */

/* -------- TWIM_PR : (TWIM Offset: 0x30) (R/  32) Parameter Register -------- */
#define TWIM_PR_OFFSET              0x30         /**< \brief (TWIM_PR offset) Parameter Register */
#define TWIM_PR_RESETVALUE          0x00000001   /**< \brief (TWIM_PR reset_value) Parameter Register */

#define TWIM_PR_HS_Pos              0            /**< \brief (TWIM_PR) HS-mode */
#define TWIM_PR_HS                  (0x1u << TWIM_PR_HS_Pos)
#define TWIM_PR_MASK                0x00000001u  /**< \brief (TWIM_PR) MASK Register */

/* -------- TWIM_VR : (TWIM Offset: 0x34) (R/  32) Version Register -------- */
#define TWIM_VR_OFFSET              0x34         /**< \brief (TWIM_VR offset) Version Register */
#define TWIM_VR_RESETVALUE          0x00000120   /**< \brief (TWIM_VR reset_value) Version Register */

#define TWIM_VR_VERSION_Pos         0            /**< \brief (TWIM_VR) Version number */
#define TWIM_VR_VERSION_Msk         (0xFFFu << TWIM_VR_VERSION_Pos)
#define TWIM_VR_VERSION(value)      ((TWIM_VR_VERSION_Msk & ((value) << TWIM_VR_VERSION_Pos)))
#define TWIM_VR_VARIANT_Pos         16           /**< \brief (TWIM_VR) Variant number */
#define TWIM_VR_VARIANT_Msk         (0xFu << TWIM_VR_VARIANT_Pos)
#define TWIM_VR_VARIANT(value)      ((TWIM_VR_VARIANT_Msk & ((value) << TWIM_VR_VARIANT_Pos)))
#define TWIM_VR_MASK                0x000F0FFFu  /**< \brief (TWIM_VR) MASK Register */

/* -------- TWIM_HSCWGR : (TWIM Offset: 0x38) (R/W 32) HS-mode Clock Waveform Generator -------- */
#define TWIM_HSCWGR_OFFSET          0x38         /**< \brief (TWIM_HSCWGR offset) HS-mode Clock Waveform Generator */
#define TWIM_HSCWGR_RESETVALUE      0x00000000   /**< \brief (TWIM_HSCWGR reset_value) HS-mode Clock Waveform Generator */

#define TWIM_HSCWGR_LOW_Pos         0            /**< \brief (TWIM_HSCWGR) Clock Low Cycles */
#define TWIM_HSCWGR_LOW_Msk         (0xFFu << TWIM_HSCWGR_LOW_Pos)
#define TWIM_HSCWGR_LOW(value)      ((TWIM_HSCWGR_LOW_Msk & ((value) << TWIM_HSCWGR_LOW_Pos)))
#define TWIM_HSCWGR_HIGH_Pos        8            /**< \brief (TWIM_HSCWGR) Clock High Cycles */
#define TWIM_HSCWGR_HIGH_Msk        (0xFFu << TWIM_HSCWGR_HIGH_Pos)
#define TWIM_HSCWGR_HIGH(value)     ((TWIM_HSCWGR_HIGH_Msk & ((value) << TWIM_HSCWGR_HIGH_Pos)))
#define TWIM_HSCWGR_STASTO_Pos      16           /**< \brief (TWIM_HSCWGR) START and STOP Cycles */
#define TWIM_HSCWGR_STASTO_Msk      (0xFFu << TWIM_HSCWGR_STASTO_Pos)
#define TWIM_HSCWGR_STASTO(value)   ((TWIM_HSCWGR_STASTO_Msk & ((value) << TWIM_HSCWGR_STASTO_Pos)))
#define TWIM_HSCWGR_DATA_Pos        24           /**< \brief (TWIM_HSCWGR) Data Setup and Hold Cycles */
#define TWIM_HSCWGR_DATA_Msk        (0xFu << TWIM_HSCWGR_DATA_Pos)
#define TWIM_HSCWGR_DATA(value)     ((TWIM_HSCWGR_DATA_Msk & ((value) << TWIM_HSCWGR_DATA_Pos)))
#define TWIM_HSCWGR_EXP_Pos         28           /**< \brief (TWIM_HSCWGR) Clock Prescaler */
#define TWIM_HSCWGR_EXP_Msk         (0x7u << TWIM_HSCWGR_EXP_Pos)
#define TWIM_HSCWGR_EXP(value)      ((TWIM_HSCWGR_EXP_Msk & ((value) << TWIM_HSCWGR_EXP_Pos)))
#define TWIM_HSCWGR_MASK            0x7FFFFFFFu  /**< \brief (TWIM_HSCWGR) MASK Register */

/* -------- TWIM_SRR : (TWIM Offset: 0x3C) (R/W 32) Slew Rate Register -------- */
#define TWIM_SRR_OFFSET             0x3C         /**< \brief (TWIM_SRR offset) Slew Rate Register */
#define TWIM_SRR_RESETVALUE         0x00000000   /**< \brief (TWIM_SRR reset_value) Slew Rate Register */

#define TWIM_SRR_DADRIVEL_Pos       0            /**< \brief (TWIM_SRR) Data Drive Strength LOW */
#define TWIM_SRR_DADRIVEL_Msk       (0x7u << TWIM_SRR_DADRIVEL_Pos)
#define TWIM_SRR_DADRIVEL(value)    ((TWIM_SRR_DADRIVEL_Msk & ((value) << TWIM_SRR_DADRIVEL_Pos)))
#define TWIM_SRR_DASLEW_Pos         8            /**< \brief (TWIM_SRR) Data Slew Limit */
#define TWIM_SRR_DASLEW_Msk         (0x3u << TWIM_SRR_DASLEW_Pos)
#define TWIM_SRR_DASLEW(value)      ((TWIM_SRR_DASLEW_Msk & ((value) << TWIM_SRR_DASLEW_Pos)))
#define TWIM_SRR_CLDRIVEL_Pos       16           /**< \brief (TWIM_SRR) Clock Drive Strength LOW */
#define TWIM_SRR_CLDRIVEL_Msk       (0x7u << TWIM_SRR_CLDRIVEL_Pos)
#define TWIM_SRR_CLDRIVEL(value)    ((TWIM_SRR_CLDRIVEL_Msk & ((value) << TWIM_SRR_CLDRIVEL_Pos)))
#define TWIM_SRR_CLSLEW_Pos         24           /**< \brief (TWIM_SRR) Clock Slew Limit */
#define TWIM_SRR_CLSLEW_Msk         (0x3u << TWIM_SRR_CLSLEW_Pos)
#define TWIM_SRR_CLSLEW(value)      ((TWIM_SRR_CLSLEW_Msk & ((value) << TWIM_SRR_CLSLEW_Pos)))
#define TWIM_SRR_FILTER_Pos         28           /**< \brief (TWIM_SRR) Input Spike Filter Control */
#define TWIM_SRR_FILTER_Msk         (0x3u << TWIM_SRR_FILTER_Pos)
#define TWIM_SRR_FILTER(value)      ((TWIM_SRR_FILTER_Msk & ((value) << TWIM_SRR_FILTER_Pos)))
#define TWIM_SRR_MASK               0x33070307u  /**< \brief (TWIM_SRR) MASK Register */

/* -------- TWIM_HSSRR : (TWIM Offset: 0x40) (R/W 32) HS-mode Slew Rate Register -------- */
#define TWIM_HSSRR_OFFSET           0x40         /**< \brief (TWIM_HSSRR offset) HS-mode Slew Rate Register */
#define TWIM_HSSRR_RESETVALUE       0x00000000   /**< \brief (TWIM_HSSRR reset_value) HS-mode Slew Rate Register */

#define TWIM_HSSRR_DADRIVEL_Pos     0            /**< \brief (TWIM_HSSRR) Data Drive Strength LOW */
#define TWIM_HSSRR_DADRIVEL_Msk     (0x7u << TWIM_HSSRR_DADRIVEL_Pos)
#define TWIM_HSSRR_DADRIVEL(value)  ((TWIM_HSSRR_DADRIVEL_Msk & ((value) << TWIM_HSSRR_DADRIVEL_Pos)))
#define TWIM_HSSRR_DASLEW_Pos       8            /**< \brief (TWIM_HSSRR) Data Slew Limit */
#define TWIM_HSSRR_DASLEW_Msk       (0x3u << TWIM_HSSRR_DASLEW_Pos)
#define TWIM_HSSRR_DASLEW(value)    ((TWIM_HSSRR_DASLEW_Msk & ((value) << TWIM_HSSRR_DASLEW_Pos)))
#define TWIM_HSSRR_CLDRIVEL_Pos     16           /**< \brief (TWIM_HSSRR) Clock Drive Strength LOW */
#define TWIM_HSSRR_CLDRIVEL_Msk     (0x7u << TWIM_HSSRR_CLDRIVEL_Pos)
#define TWIM_HSSRR_CLDRIVEL(value)  ((TWIM_HSSRR_CLDRIVEL_Msk & ((value) << TWIM_HSSRR_CLDRIVEL_Pos)))
#define TWIM_HSSRR_CLDRIVEH_Pos     20           /**< \brief (TWIM_HSSRR) Clock Drive Strength HIGH */
#define TWIM_HSSRR_CLDRIVEH_Msk     (0x3u << TWIM_HSSRR_CLDRIVEH_Pos)
#define TWIM_HSSRR_CLDRIVEH(value)  ((TWIM_HSSRR_CLDRIVEH_Msk & ((value) << TWIM_HSSRR_CLDRIVEH_Pos)))
#define TWIM_HSSRR_CLSLEW_Pos       24           /**< \brief (TWIM_HSSRR) Clock Slew Limit */
#define TWIM_HSSRR_CLSLEW_Msk       (0x3u << TWIM_HSSRR_CLSLEW_Pos)
#define TWIM_HSSRR_CLSLEW(value)    ((TWIM_HSSRR_CLSLEW_Msk & ((value) << TWIM_HSSRR_CLSLEW_Pos)))
#define TWIM_HSSRR_FILTER_Pos       28           /**< \brief (TWIM_HSSRR) Input Spike Filter Control */
#define TWIM_HSSRR_FILTER_Msk       (0x3u << TWIM_HSSRR_FILTER_Pos)
#define TWIM_HSSRR_FILTER(value)    ((TWIM_HSSRR_FILTER_Msk & ((value) << TWIM_HSSRR_FILTER_Pos)))
#define TWIM_HSSRR_MASK             0x33370307u  /**< \brief (TWIM_HSSRR) MASK Register */

/** \brief TWIM hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
  WoReg   TWIM_CR;            /**< \brief (TWIM Offset: 0x00) Control Register */
  RwReg   TWIM_CWGR;          /**< \brief (TWIM Offset: 0x04) Clock Waveform Generator Register */
  RwReg   TWIM_SMBTR;         /**< \brief (TWIM Offset: 0x08) SMBus Timing Register */
  RwReg   TWIM_CMDR;          /**< \brief (TWIM Offset: 0x0C) Command Register */
  RwReg   TWIM_NCMDR;         /**< \brief (TWIM Offset: 0x10) Next Command Register */
  RoReg   TWIM_RHR;           /**< \brief (TWIM Offset: 0x14) Receive Holding Register */
  WoReg   TWIM_THR;           /**< \brief (TWIM Offset: 0x18) Transmit Holding Register */
  RoReg   TWIM_SR;            /**< \brief (TWIM Offset: 0x1C) Status Register */
  WoReg   TWIM_IER;           /**< \brief (TWIM Offset: 0x20) Interrupt Enable Register */
  WoReg   TWIM_IDR;           /**< \brief (TWIM Offset: 0x24) Interrupt Disable Register */
  RoReg   TWIM_IMR;           /**< \brief (TWIM Offset: 0x28) Interrupt Mask Register */
  WoReg   TWIM_SCR;           /**< \brief (TWIM Offset: 0x2C) Status Clear Register */
  RoReg   TWIM_PR;            /**< \brief (TWIM Offset: 0x30) Parameter Register */
  RoReg   TWIM_VR;            /**< \brief (TWIM Offset: 0x34) Version Register */
  RwReg   TWIM_HSCWGR;        /**< \brief (TWIM Offset: 0x38) HS-mode Clock Waveform Generator */
  RwReg   TWIM_SRR;           /**< \brief (TWIM Offset: 0x3C) Slew Rate Register */
  RwReg   TWIM_HSSRR;         /**< \brief (TWIM Offset: 0x40) HS-mode Slew Rate Register */
} Twim;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/*@}*/

#endif /* _SAM4L_TWIM_COMPONENT_ */
