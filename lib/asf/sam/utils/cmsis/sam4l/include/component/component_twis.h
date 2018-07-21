/**
 * \file
 *
 * \brief Component description for TWIS
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

#ifndef _SAM4L_TWIS_COMPONENT_
#define _SAM4L_TWIS_COMPONENT_

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR TWIS */
/* ========================================================================== */
/** \addtogroup SAM4L_TWIS Two-wire Slave Interface */
/*@{*/

#define REV_TWIS                    0x140

/* -------- TWIS_CR : (TWIS Offset: 0x00) (R/W 32) Control Register -------- */
#define TWIS_CR_OFFSET              0x00         /**< \brief (TWIS_CR offset) Control Register */
#define TWIS_CR_RESETVALUE          0x00000000   /**< \brief (TWIS_CR reset_value) Control Register */

#define TWIS_CR_SEN_Pos             0            /**< \brief (TWIS_CR) Slave Enable */
#define TWIS_CR_SEN                 (0x1u << TWIS_CR_SEN_Pos)
#define TWIS_CR_SMEN_Pos            1            /**< \brief (TWIS_CR) SMBus Mode Enable */
#define TWIS_CR_SMEN                (0x1u << TWIS_CR_SMEN_Pos)
#define TWIS_CR_SMATCH_Pos          2            /**< \brief (TWIS_CR) Slave Address Match */
#define TWIS_CR_SMATCH              (0x1u << TWIS_CR_SMATCH_Pos)
#define TWIS_CR_GCMATCH_Pos         3            /**< \brief (TWIS_CR) General Call Address Match */
#define TWIS_CR_GCMATCH             (0x1u << TWIS_CR_GCMATCH_Pos)
#define TWIS_CR_STREN_Pos           4            /**< \brief (TWIS_CR) Clock Stretch Enable */
#define TWIS_CR_STREN               (0x1u << TWIS_CR_STREN_Pos)
#define TWIS_CR_SWRST_Pos           7            /**< \brief (TWIS_CR) Software Reset */
#define TWIS_CR_SWRST               (0x1u << TWIS_CR_SWRST_Pos)
#define TWIS_CR_SMBALERT_Pos        8            /**< \brief (TWIS_CR) SMBus Alert */
#define TWIS_CR_SMBALERT            (0x1u << TWIS_CR_SMBALERT_Pos)
#define TWIS_CR_SMDA_Pos            9            /**< \brief (TWIS_CR) SMBus Default Address */
#define TWIS_CR_SMDA                (0x1u << TWIS_CR_SMDA_Pos)
#define TWIS_CR_SMHH_Pos            10           /**< \brief (TWIS_CR) SMBus Host Header */
#define TWIS_CR_SMHH                (0x1u << TWIS_CR_SMHH_Pos)
#define TWIS_CR_PECEN_Pos           11           /**< \brief (TWIS_CR) Packet Error Checking Enable */
#define TWIS_CR_PECEN               (0x1u << TWIS_CR_PECEN_Pos)
#define TWIS_CR_ACK_Pos             12           /**< \brief (TWIS_CR) Slave Receiver Data Phase ACK Value */
#define TWIS_CR_ACK                 (0x1u << TWIS_CR_ACK_Pos)
#define TWIS_CR_CUP_Pos             13           /**< \brief (TWIS_CR) NBYTES Count Up */
#define TWIS_CR_CUP                 (0x1u << TWIS_CR_CUP_Pos)
#define TWIS_CR_SOAM_Pos            14           /**< \brief (TWIS_CR) Stretch Clock on Address Match */
#define TWIS_CR_SOAM                (0x1u << TWIS_CR_SOAM_Pos)
#define TWIS_CR_SODR_Pos            15           /**< \brief (TWIS_CR) Stretch Clock on Data Byte Reception */
#define TWIS_CR_SODR                (0x1u << TWIS_CR_SODR_Pos)
#define TWIS_CR_ADR_Pos             16           /**< \brief (TWIS_CR) Slave Address */
#define TWIS_CR_ADR_Msk             (0x3FFu << TWIS_CR_ADR_Pos)
#define TWIS_CR_ADR(value)          ((TWIS_CR_ADR_Msk & ((value) << TWIS_CR_ADR_Pos)))
#define TWIS_CR_TENBIT_Pos          26           /**< \brief (TWIS_CR) Ten Bit Address Match */
#define TWIS_CR_TENBIT              (0x1u << TWIS_CR_TENBIT_Pos)
#define TWIS_CR_BRIDGE_Pos          27           /**< \brief (TWIS_CR) Bridge Control Enable */
#define TWIS_CR_BRIDGE              (0x1u << TWIS_CR_BRIDGE_Pos)
#define TWIS_CR_MASK                0x0FFFFF9Fu  /**< \brief (TWIS_CR) MASK Register */

/* -------- TWIS_NBYTES : (TWIS Offset: 0x04) (R/W 32) NBYTES Register -------- */
#define TWIS_NBYTES_OFFSET          0x04         /**< \brief (TWIS_NBYTES offset) NBYTES Register */
#define TWIS_NBYTES_RESETVALUE      0x00000000   /**< \brief (TWIS_NBYTES reset_value) NBYTES Register */

#define TWIS_NBYTES_NBYTES_Pos      0            /**< \brief (TWIS_NBYTES) Number of Bytes to Transfer */
#define TWIS_NBYTES_NBYTES_Msk      (0xFFu << TWIS_NBYTES_NBYTES_Pos)
#define TWIS_NBYTES_NBYTES(value)   ((TWIS_NBYTES_NBYTES_Msk & ((value) << TWIS_NBYTES_NBYTES_Pos)))
#define TWIS_NBYTES_MASK            0x000000FFu  /**< \brief (TWIS_NBYTES) MASK Register */

/* -------- TWIS_TR : (TWIS Offset: 0x08) (R/W 32) Timing Register -------- */
#define TWIS_TR_OFFSET              0x08         /**< \brief (TWIS_TR offset) Timing Register */
#define TWIS_TR_RESETVALUE          0x00000000   /**< \brief (TWIS_TR reset_value) Timing Register */

#define TWIS_TR_TLOWS_Pos           0            /**< \brief (TWIS_TR) SMBus Tlow:sext Cycles */
#define TWIS_TR_TLOWS_Msk           (0xFFu << TWIS_TR_TLOWS_Pos)
#define TWIS_TR_TLOWS(value)        ((TWIS_TR_TLOWS_Msk & ((value) << TWIS_TR_TLOWS_Pos)))
#define TWIS_TR_TTOUT_Pos           8            /**< \brief (TWIS_TR) SMBus Ttimeout Cycles */
#define TWIS_TR_TTOUT_Msk           (0xFFu << TWIS_TR_TTOUT_Pos)
#define TWIS_TR_TTOUT(value)        ((TWIS_TR_TTOUT_Msk & ((value) << TWIS_TR_TTOUT_Pos)))
#define TWIS_TR_SUDAT_Pos           16           /**< \brief (TWIS_TR) Data Setup Cycles */
#define TWIS_TR_SUDAT_Msk           (0xFFu << TWIS_TR_SUDAT_Pos)
#define TWIS_TR_SUDAT(value)        ((TWIS_TR_SUDAT_Msk & ((value) << TWIS_TR_SUDAT_Pos)))
#define TWIS_TR_EXP_Pos             28           /**< \brief (TWIS_TR) Clock Prescaler */
#define TWIS_TR_EXP_Msk             (0xFu << TWIS_TR_EXP_Pos)
#define TWIS_TR_EXP(value)          ((TWIS_TR_EXP_Msk & ((value) << TWIS_TR_EXP_Pos)))
#define TWIS_TR_MASK                0xF0FFFFFFu  /**< \brief (TWIS_TR) MASK Register */

/* -------- TWIS_RHR : (TWIS Offset: 0x0C) (R/  32) Receive Holding Register -------- */
#define TWIS_RHR_OFFSET             0x0C         /**< \brief (TWIS_RHR offset) Receive Holding Register */
#define TWIS_RHR_RESETVALUE         0x00000000   /**< \brief (TWIS_RHR reset_value) Receive Holding Register */

#define TWIS_RHR_RXDATA_Pos         0            /**< \brief (TWIS_RHR) Received Data Byte */
#define TWIS_RHR_RXDATA_Msk         (0xFFu << TWIS_RHR_RXDATA_Pos)
#define TWIS_RHR_RXDATA(value)      ((TWIS_RHR_RXDATA_Msk & ((value) << TWIS_RHR_RXDATA_Pos)))
#define TWIS_RHR_MASK               0x000000FFu  /**< \brief (TWIS_RHR) MASK Register */

/* -------- TWIS_THR : (TWIS Offset: 0x10) ( /W 32) Transmit Holding Register -------- */
#define TWIS_THR_OFFSET             0x10         /**< \brief (TWIS_THR offset) Transmit Holding Register */
#define TWIS_THR_RESETVALUE         0x00000000   /**< \brief (TWIS_THR reset_value) Transmit Holding Register */

#define TWIS_THR_TXDATA_Pos         0            /**< \brief (TWIS_THR) Data Byte to Transmit */
#define TWIS_THR_TXDATA_Msk         (0xFFu << TWIS_THR_TXDATA_Pos)
#define TWIS_THR_TXDATA(value)      ((TWIS_THR_TXDATA_Msk & ((value) << TWIS_THR_TXDATA_Pos)))
#define TWIS_THR_MASK               0x000000FFu  /**< \brief (TWIS_THR) MASK Register */

/* -------- TWIS_PECR : (TWIS Offset: 0x14) (R/  32) Packet Error Check Register -------- */
#define TWIS_PECR_OFFSET            0x14         /**< \brief (TWIS_PECR offset) Packet Error Check Register */
#define TWIS_PECR_RESETVALUE        0x00000000   /**< \brief (TWIS_PECR reset_value) Packet Error Check Register */

#define TWIS_PECR_PEC_Pos           0            /**< \brief (TWIS_PECR) Calculated PEC Value */
#define TWIS_PECR_PEC_Msk           (0xFFu << TWIS_PECR_PEC_Pos)
#define TWIS_PECR_PEC(value)        ((TWIS_PECR_PEC_Msk & ((value) << TWIS_PECR_PEC_Pos)))
#define TWIS_PECR_MASK              0x000000FFu  /**< \brief (TWIS_PECR) MASK Register */

/* -------- TWIS_SR : (TWIS Offset: 0x18) (R/  32) Status Register -------- */
#define TWIS_SR_OFFSET              0x18         /**< \brief (TWIS_SR offset) Status Register */
#define TWIS_SR_RESETVALUE          0x00000002   /**< \brief (TWIS_SR reset_value) Status Register */

#define TWIS_SR_RXRDY_Pos           0            /**< \brief (TWIS_SR) RX Buffer Ready */
#define TWIS_SR_RXRDY               (0x1u << TWIS_SR_RXRDY_Pos)
#define TWIS_SR_TXRDY_Pos           1            /**< \brief (TWIS_SR) TX Buffer Ready */
#define TWIS_SR_TXRDY               (0x1u << TWIS_SR_TXRDY_Pos)
#define TWIS_SR_SEN_Pos             2            /**< \brief (TWIS_SR) Slave Enabled */
#define TWIS_SR_SEN                 (0x1u << TWIS_SR_SEN_Pos)
#define TWIS_SR_TCOMP_Pos           3            /**< \brief (TWIS_SR) Transmission Complete */
#define TWIS_SR_TCOMP               (0x1u << TWIS_SR_TCOMP_Pos)
#define TWIS_SR_TRA_Pos             5            /**< \brief (TWIS_SR) Transmitter Mode */
#define TWIS_SR_TRA                 (0x1u << TWIS_SR_TRA_Pos)
#define TWIS_SR_URUN_Pos            6            /**< \brief (TWIS_SR) Underrun */
#define TWIS_SR_URUN                (0x1u << TWIS_SR_URUN_Pos)
#define TWIS_SR_ORUN_Pos            7            /**< \brief (TWIS_SR) Overrun */
#define TWIS_SR_ORUN                (0x1u << TWIS_SR_ORUN_Pos)
#define TWIS_SR_NAK_Pos             8            /**< \brief (TWIS_SR) NAK Received */
#define TWIS_SR_NAK                 (0x1u << TWIS_SR_NAK_Pos)
#define TWIS_SR_SMBTOUT_Pos         12           /**< \brief (TWIS_SR) SMBus Timeout */
#define TWIS_SR_SMBTOUT             (0x1u << TWIS_SR_SMBTOUT_Pos)
#define TWIS_SR_SMBPECERR_Pos       13           /**< \brief (TWIS_SR) SMBus PEC Error */
#define TWIS_SR_SMBPECERR           (0x1u << TWIS_SR_SMBPECERR_Pos)
#define TWIS_SR_BUSERR_Pos          14           /**< \brief (TWIS_SR) Bus Error */
#define TWIS_SR_BUSERR              (0x1u << TWIS_SR_BUSERR_Pos)
#define TWIS_SR_SAM_Pos             16           /**< \brief (TWIS_SR) Slave Address Match */
#define TWIS_SR_SAM                 (0x1u << TWIS_SR_SAM_Pos)
#define TWIS_SR_GCM_Pos             17           /**< \brief (TWIS_SR) General Call Match */
#define TWIS_SR_GCM                 (0x1u << TWIS_SR_GCM_Pos)
#define TWIS_SR_SMBALERTM_Pos       18           /**< \brief (TWIS_SR) SMBus Alert Response Address Match */
#define TWIS_SR_SMBALERTM           (0x1u << TWIS_SR_SMBALERTM_Pos)
#define TWIS_SR_SMBHHM_Pos          19           /**< \brief (TWIS_SR) SMBus Host Header Address Match */
#define TWIS_SR_SMBHHM              (0x1u << TWIS_SR_SMBHHM_Pos)
#define TWIS_SR_SMBDAM_Pos          20           /**< \brief (TWIS_SR) SMBus Default Address Match */
#define TWIS_SR_SMBDAM              (0x1u << TWIS_SR_SMBDAM_Pos)
#define TWIS_SR_STO_Pos             21           /**< \brief (TWIS_SR) Stop Received */
#define TWIS_SR_STO                 (0x1u << TWIS_SR_STO_Pos)
#define TWIS_SR_REP_Pos             22           /**< \brief (TWIS_SR) Repeated Start Received */
#define TWIS_SR_REP                 (0x1u << TWIS_SR_REP_Pos)
#define TWIS_SR_BTF_Pos             23           /**< \brief (TWIS_SR) Byte Transfer Finished */
#define TWIS_SR_BTF                 (0x1u << TWIS_SR_BTF_Pos)
#define TWIS_SR_MASK                0x00FF71EFu  /**< \brief (TWIS_SR) MASK Register */

/* -------- TWIS_IER : (TWIS Offset: 0x1C) ( /W 32) Interrupt Enable Register -------- */
#define TWIS_IER_OFFSET             0x1C         /**< \brief (TWIS_IER offset) Interrupt Enable Register */
#define TWIS_IER_RESETVALUE         0x00000000   /**< \brief (TWIS_IER reset_value) Interrupt Enable Register */

#define TWIS_IER_RXRDY_Pos          0            /**< \brief (TWIS_IER) RX Buffer Ready */
#define TWIS_IER_RXRDY              (0x1u << TWIS_IER_RXRDY_Pos)
#define TWIS_IER_TXRDY_Pos          1            /**< \brief (TWIS_IER) TX Buffer Ready */
#define TWIS_IER_TXRDY              (0x1u << TWIS_IER_TXRDY_Pos)
#define TWIS_IER_TCOMP_Pos          3            /**< \brief (TWIS_IER) Transmission Complete */
#define TWIS_IER_TCOMP              (0x1u << TWIS_IER_TCOMP_Pos)
#define TWIS_IER_URUN_Pos           6            /**< \brief (TWIS_IER) Underrun */
#define TWIS_IER_URUN               (0x1u << TWIS_IER_URUN_Pos)
#define TWIS_IER_ORUN_Pos           7            /**< \brief (TWIS_IER) Overrun */
#define TWIS_IER_ORUN               (0x1u << TWIS_IER_ORUN_Pos)
#define TWIS_IER_NAK_Pos            8            /**< \brief (TWIS_IER) NAK Received */
#define TWIS_IER_NAK                (0x1u << TWIS_IER_NAK_Pos)
#define TWIS_IER_SMBTOUT_Pos        12           /**< \brief (TWIS_IER) SMBus Timeout */
#define TWIS_IER_SMBTOUT            (0x1u << TWIS_IER_SMBTOUT_Pos)
#define TWIS_IER_SMBPECERR_Pos      13           /**< \brief (TWIS_IER) SMBus PEC Error */
#define TWIS_IER_SMBPECERR          (0x1u << TWIS_IER_SMBPECERR_Pos)
#define TWIS_IER_BUSERR_Pos         14           /**< \brief (TWIS_IER) Bus Error */
#define TWIS_IER_BUSERR             (0x1u << TWIS_IER_BUSERR_Pos)
#define TWIS_IER_SAM_Pos            16           /**< \brief (TWIS_IER) Slave Address Match */
#define TWIS_IER_SAM                (0x1u << TWIS_IER_SAM_Pos)
#define TWIS_IER_GCM_Pos            17           /**< \brief (TWIS_IER) General Call Match */
#define TWIS_IER_GCM                (0x1u << TWIS_IER_GCM_Pos)
#define TWIS_IER_SMBALERTM_Pos      18           /**< \brief (TWIS_IER) SMBus Alert Response Address Match */
#define TWIS_IER_SMBALERTM          (0x1u << TWIS_IER_SMBALERTM_Pos)
#define TWIS_IER_SMBHHM_Pos         19           /**< \brief (TWIS_IER) SMBus Host Header Address Match */
#define TWIS_IER_SMBHHM             (0x1u << TWIS_IER_SMBHHM_Pos)
#define TWIS_IER_SMBDAM_Pos         20           /**< \brief (TWIS_IER) SMBus Default Address Match */
#define TWIS_IER_SMBDAM             (0x1u << TWIS_IER_SMBDAM_Pos)
#define TWIS_IER_STO_Pos            21           /**< \brief (TWIS_IER) Stop Received */
#define TWIS_IER_STO                (0x1u << TWIS_IER_STO_Pos)
#define TWIS_IER_REP_Pos            22           /**< \brief (TWIS_IER) Repeated Start Received */
#define TWIS_IER_REP                (0x1u << TWIS_IER_REP_Pos)
#define TWIS_IER_BTF_Pos            23           /**< \brief (TWIS_IER) Byte Transfer Finished */
#define TWIS_IER_BTF                (0x1u << TWIS_IER_BTF_Pos)
#define TWIS_IER_MASK               0x00FF71CBu  /**< \brief (TWIS_IER) MASK Register */

/* -------- TWIS_IDR : (TWIS Offset: 0x20) ( /W 32) Interrupt Disable Register -------- */
#define TWIS_IDR_OFFSET             0x20         /**< \brief (TWIS_IDR offset) Interrupt Disable Register */
#define TWIS_IDR_RESETVALUE         0x00000000   /**< \brief (TWIS_IDR reset_value) Interrupt Disable Register */

#define TWIS_IDR_RXRDY_Pos          0            /**< \brief (TWIS_IDR) RX Buffer Ready */
#define TWIS_IDR_RXRDY              (0x1u << TWIS_IDR_RXRDY_Pos)
#define TWIS_IDR_TXRDY_Pos          1            /**< \brief (TWIS_IDR) TX Buffer Ready */
#define TWIS_IDR_TXRDY              (0x1u << TWIS_IDR_TXRDY_Pos)
#define TWIS_IDR_TCOMP_Pos          3            /**< \brief (TWIS_IDR) Transmission Complete */
#define TWIS_IDR_TCOMP              (0x1u << TWIS_IDR_TCOMP_Pos)
#define TWIS_IDR_URUN_Pos           6            /**< \brief (TWIS_IDR) Underrun */
#define TWIS_IDR_URUN               (0x1u << TWIS_IDR_URUN_Pos)
#define TWIS_IDR_ORUN_Pos           7            /**< \brief (TWIS_IDR) Overrun */
#define TWIS_IDR_ORUN               (0x1u << TWIS_IDR_ORUN_Pos)
#define TWIS_IDR_NAK_Pos            8            /**< \brief (TWIS_IDR) NAK Received */
#define TWIS_IDR_NAK                (0x1u << TWIS_IDR_NAK_Pos)
#define TWIS_IDR_SMBTOUT_Pos        12           /**< \brief (TWIS_IDR) SMBus Timeout */
#define TWIS_IDR_SMBTOUT            (0x1u << TWIS_IDR_SMBTOUT_Pos)
#define TWIS_IDR_SMBPECERR_Pos      13           /**< \brief (TWIS_IDR) SMBus PEC Error */
#define TWIS_IDR_SMBPECERR          (0x1u << TWIS_IDR_SMBPECERR_Pos)
#define TWIS_IDR_BUSERR_Pos         14           /**< \brief (TWIS_IDR) Bus Error */
#define TWIS_IDR_BUSERR             (0x1u << TWIS_IDR_BUSERR_Pos)
#define TWIS_IDR_SAM_Pos            16           /**< \brief (TWIS_IDR) Slave Address Match */
#define TWIS_IDR_SAM                (0x1u << TWIS_IDR_SAM_Pos)
#define TWIS_IDR_GCM_Pos            17           /**< \brief (TWIS_IDR) General Call Match */
#define TWIS_IDR_GCM                (0x1u << TWIS_IDR_GCM_Pos)
#define TWIS_IDR_SMBALERTM_Pos      18           /**< \brief (TWIS_IDR) SMBus Alert Response Address Match */
#define TWIS_IDR_SMBALERTM          (0x1u << TWIS_IDR_SMBALERTM_Pos)
#define TWIS_IDR_SMBHHM_Pos         19           /**< \brief (TWIS_IDR) SMBus Host Header Address Match */
#define TWIS_IDR_SMBHHM             (0x1u << TWIS_IDR_SMBHHM_Pos)
#define TWIS_IDR_SMBDAM_Pos         20           /**< \brief (TWIS_IDR) SMBus Default Address Match */
#define TWIS_IDR_SMBDAM             (0x1u << TWIS_IDR_SMBDAM_Pos)
#define TWIS_IDR_STO_Pos            21           /**< \brief (TWIS_IDR) Stop Received */
#define TWIS_IDR_STO                (0x1u << TWIS_IDR_STO_Pos)
#define TWIS_IDR_REP_Pos            22           /**< \brief (TWIS_IDR) Repeated Start Received */
#define TWIS_IDR_REP                (0x1u << TWIS_IDR_REP_Pos)
#define TWIS_IDR_BTF_Pos            23           /**< \brief (TWIS_IDR) Byte Transfer Finished */
#define TWIS_IDR_BTF                (0x1u << TWIS_IDR_BTF_Pos)
#define TWIS_IDR_MASK               0x00FF71CBu  /**< \brief (TWIS_IDR) MASK Register */

/* -------- TWIS_IMR : (TWIS Offset: 0x24) (R/  32) Interrupt Mask Register -------- */
#define TWIS_IMR_OFFSET             0x24         /**< \brief (TWIS_IMR offset) Interrupt Mask Register */
#define TWIS_IMR_RESETVALUE         0x00000000   /**< \brief (TWIS_IMR reset_value) Interrupt Mask Register */

#define TWIS_IMR_RXRDY_Pos          0            /**< \brief (TWIS_IMR) RX Buffer Ready */
#define TWIS_IMR_RXRDY              (0x1u << TWIS_IMR_RXRDY_Pos)
#define TWIS_IMR_TXRDY_Pos          1            /**< \brief (TWIS_IMR) TX Buffer Ready */
#define TWIS_IMR_TXRDY              (0x1u << TWIS_IMR_TXRDY_Pos)
#define TWIS_IMR_TCOMP_Pos          3            /**< \brief (TWIS_IMR) Transmission Complete */
#define TWIS_IMR_TCOMP              (0x1u << TWIS_IMR_TCOMP_Pos)
#define TWIS_IMR_URUN_Pos           6            /**< \brief (TWIS_IMR) Underrun */
#define TWIS_IMR_URUN               (0x1u << TWIS_IMR_URUN_Pos)
#define TWIS_IMR_ORUN_Pos           7            /**< \brief (TWIS_IMR) Overrun */
#define TWIS_IMR_ORUN               (0x1u << TWIS_IMR_ORUN_Pos)
#define TWIS_IMR_NAK_Pos            8            /**< \brief (TWIS_IMR) NAK Received */
#define TWIS_IMR_NAK                (0x1u << TWIS_IMR_NAK_Pos)
#define TWIS_IMR_SMBTOUT_Pos        12           /**< \brief (TWIS_IMR) SMBus Timeout */
#define TWIS_IMR_SMBTOUT            (0x1u << TWIS_IMR_SMBTOUT_Pos)
#define TWIS_IMR_SMBPECERR_Pos      13           /**< \brief (TWIS_IMR) SMBus PEC Error */
#define TWIS_IMR_SMBPECERR          (0x1u << TWIS_IMR_SMBPECERR_Pos)
#define TWIS_IMR_BUSERR_Pos         14           /**< \brief (TWIS_IMR) Bus Error */
#define TWIS_IMR_BUSERR             (0x1u << TWIS_IMR_BUSERR_Pos)
#define TWIS_IMR_SAM_Pos            16           /**< \brief (TWIS_IMR) Slave Address Match */
#define TWIS_IMR_SAM                (0x1u << TWIS_IMR_SAM_Pos)
#define TWIS_IMR_GCM_Pos            17           /**< \brief (TWIS_IMR) General Call Match */
#define TWIS_IMR_GCM                (0x1u << TWIS_IMR_GCM_Pos)
#define TWIS_IMR_SMBALERTM_Pos      18           /**< \brief (TWIS_IMR) SMBus Alert Response Address Match */
#define TWIS_IMR_SMBALERTM          (0x1u << TWIS_IMR_SMBALERTM_Pos)
#define TWIS_IMR_SMBHHM_Pos         19           /**< \brief (TWIS_IMR) SMBus Host Header Address Match */
#define TWIS_IMR_SMBHHM             (0x1u << TWIS_IMR_SMBHHM_Pos)
#define TWIS_IMR_SMBDAM_Pos         20           /**< \brief (TWIS_IMR) SMBus Default Address Match */
#define TWIS_IMR_SMBDAM             (0x1u << TWIS_IMR_SMBDAM_Pos)
#define TWIS_IMR_STO_Pos            21           /**< \brief (TWIS_IMR) Stop Received */
#define TWIS_IMR_STO                (0x1u << TWIS_IMR_STO_Pos)
#define TWIS_IMR_REP_Pos            22           /**< \brief (TWIS_IMR) Repeated Start Received */
#define TWIS_IMR_REP                (0x1u << TWIS_IMR_REP_Pos)
#define TWIS_IMR_BTF_Pos            23           /**< \brief (TWIS_IMR) Byte Transfer Finished */
#define TWIS_IMR_BTF                (0x1u << TWIS_IMR_BTF_Pos)
#define TWIS_IMR_MASK               0x00FF71CBu  /**< \brief (TWIS_IMR) MASK Register */

/* -------- TWIS_SCR : (TWIS Offset: 0x28) ( /W 32) Status Clear Register -------- */
#define TWIS_SCR_OFFSET             0x28         /**< \brief (TWIS_SCR offset) Status Clear Register */
#define TWIS_SCR_RESETVALUE         0x00000000   /**< \brief (TWIS_SCR reset_value) Status Clear Register */

#define TWIS_SCR_TCOMP_Pos          3            /**< \brief (TWIS_SCR) Transmission Complete */
#define TWIS_SCR_TCOMP              (0x1u << TWIS_SCR_TCOMP_Pos)
#define TWIS_SCR_URUN_Pos           6            /**< \brief (TWIS_SCR) Underrun */
#define TWIS_SCR_URUN               (0x1u << TWIS_SCR_URUN_Pos)
#define TWIS_SCR_ORUN_Pos           7            /**< \brief (TWIS_SCR) Overrun */
#define TWIS_SCR_ORUN               (0x1u << TWIS_SCR_ORUN_Pos)
#define TWIS_SCR_NAK_Pos            8            /**< \brief (TWIS_SCR) NAK Received */
#define TWIS_SCR_NAK                (0x1u << TWIS_SCR_NAK_Pos)
#define TWIS_SCR_SMBTOUT_Pos        12           /**< \brief (TWIS_SCR) SMBus Timeout */
#define TWIS_SCR_SMBTOUT            (0x1u << TWIS_SCR_SMBTOUT_Pos)
#define TWIS_SCR_SMBPECERR_Pos      13           /**< \brief (TWIS_SCR) SMBus PEC Error */
#define TWIS_SCR_SMBPECERR          (0x1u << TWIS_SCR_SMBPECERR_Pos)
#define TWIS_SCR_BUSERR_Pos         14           /**< \brief (TWIS_SCR) Bus Error */
#define TWIS_SCR_BUSERR             (0x1u << TWIS_SCR_BUSERR_Pos)
#define TWIS_SCR_SAM_Pos            16           /**< \brief (TWIS_SCR) Slave Address Match */
#define TWIS_SCR_SAM                (0x1u << TWIS_SCR_SAM_Pos)
#define TWIS_SCR_GCM_Pos            17           /**< \brief (TWIS_SCR) General Call Match */
#define TWIS_SCR_GCM                (0x1u << TWIS_SCR_GCM_Pos)
#define TWIS_SCR_SMBALERTM_Pos      18           /**< \brief (TWIS_SCR) SMBus Alert Response Address Match */
#define TWIS_SCR_SMBALERTM          (0x1u << TWIS_SCR_SMBALERTM_Pos)
#define TWIS_SCR_SMBHHM_Pos         19           /**< \brief (TWIS_SCR) SMBus Host Header Address Match */
#define TWIS_SCR_SMBHHM             (0x1u << TWIS_SCR_SMBHHM_Pos)
#define TWIS_SCR_SMBDAM_Pos         20           /**< \brief (TWIS_SCR) SMBus Default Address Match */
#define TWIS_SCR_SMBDAM             (0x1u << TWIS_SCR_SMBDAM_Pos)
#define TWIS_SCR_STO_Pos            21           /**< \brief (TWIS_SCR) Stop Received */
#define TWIS_SCR_STO                (0x1u << TWIS_SCR_STO_Pos)
#define TWIS_SCR_REP_Pos            22           /**< \brief (TWIS_SCR) Repeated Start Received */
#define TWIS_SCR_REP                (0x1u << TWIS_SCR_REP_Pos)
#define TWIS_SCR_BTF_Pos            23           /**< \brief (TWIS_SCR) Byte Transfer Finished */
#define TWIS_SCR_BTF                (0x1u << TWIS_SCR_BTF_Pos)
#define TWIS_SCR_MASK               0x00FF71C8u  /**< \brief (TWIS_SCR) MASK Register */

/* -------- TWIS_PR : (TWIS Offset: 0x2C) (R/  32) Parameter Register -------- */
#define TWIS_PR_OFFSET              0x2C         /**< \brief (TWIS_PR offset) Parameter Register */
#define TWIS_PR_RESETVALUE          0x00000001   /**< \brief (TWIS_PR reset_value) Parameter Register */

#define TWIS_PR_HS_Pos              0            /**< \brief (TWIS_PR) HS-mode */
#define TWIS_PR_HS                  (0x1u << TWIS_PR_HS_Pos)
#define TWIS_PR_MASK                0x00000001u  /**< \brief (TWIS_PR) MASK Register */

/* -------- TWIS_VR : (TWIS Offset: 0x30) (R/  32) Version Register -------- */
#define TWIS_VR_OFFSET              0x30         /**< \brief (TWIS_VR offset) Version Register */
#define TWIS_VR_RESETVALUE          0x00000140   /**< \brief (TWIS_VR reset_value) Version Register */

#define TWIS_VR_VERSION_Pos         0            /**< \brief (TWIS_VR) Version Number */
#define TWIS_VR_VERSION_Msk         (0xFFFu << TWIS_VR_VERSION_Pos)
#define TWIS_VR_VERSION(value)      ((TWIS_VR_VERSION_Msk & ((value) << TWIS_VR_VERSION_Pos)))
#define TWIS_VR_VARIANT_Pos         16           /**< \brief (TWIS_VR) Variant Number */
#define TWIS_VR_VARIANT_Msk         (0xFu << TWIS_VR_VARIANT_Pos)
#define TWIS_VR_VARIANT(value)      ((TWIS_VR_VARIANT_Msk & ((value) << TWIS_VR_VARIANT_Pos)))
#define TWIS_VR_MASK                0x000F0FFFu  /**< \brief (TWIS_VR) MASK Register */

/* -------- TWIS_HSTR : (TWIS Offset: 0x34) (R/W 32) HS-mode Timing Register -------- */
#define TWIS_HSTR_OFFSET            0x34         /**< \brief (TWIS_HSTR offset) HS-mode Timing Register */
#define TWIS_HSTR_RESETVALUE        0x00000000   /**< \brief (TWIS_HSTR reset_value) HS-mode Timing Register */

#define TWIS_HSTR_HDDAT_Pos         16           /**< \brief (TWIS_HSTR) Data Hold Cycles */
#define TWIS_HSTR_HDDAT_Msk         (0xFFu << TWIS_HSTR_HDDAT_Pos)
#define TWIS_HSTR_HDDAT(value)      ((TWIS_HSTR_HDDAT_Msk & ((value) << TWIS_HSTR_HDDAT_Pos)))
#define TWIS_HSTR_MASK              0x00FF0000u  /**< \brief (TWIS_HSTR) MASK Register */

/* -------- TWIS_SRR : (TWIS Offset: 0x38) (R/W 32) Slew Rate Register -------- */
#define TWIS_SRR_OFFSET             0x38         /**< \brief (TWIS_SRR offset) Slew Rate Register */
#define TWIS_SRR_RESETVALUE         0x00000000   /**< \brief (TWIS_SRR reset_value) Slew Rate Register */

#define TWIS_SRR_DADRIVEL_Pos       0            /**< \brief (TWIS_SRR) Data Drive Strength LOW */
#define TWIS_SRR_DADRIVEL_Msk       (0x7u << TWIS_SRR_DADRIVEL_Pos)
#define TWIS_SRR_DADRIVEL(value)    ((TWIS_SRR_DADRIVEL_Msk & ((value) << TWIS_SRR_DADRIVEL_Pos)))
#define TWIS_SRR_DASLEW_Pos         8            /**< \brief (TWIS_SRR) Data Slew Limit */
#define TWIS_SRR_DASLEW_Msk         (0x3u << TWIS_SRR_DASLEW_Pos)
#define TWIS_SRR_DASLEW(value)      ((TWIS_SRR_DASLEW_Msk & ((value) << TWIS_SRR_DASLEW_Pos)))
#define TWIS_SRR_FILTER_Pos         28           /**< \brief (TWIS_SRR) Input Spike Filter Control */
#define TWIS_SRR_FILTER_Msk         (0x3u << TWIS_SRR_FILTER_Pos)
#define TWIS_SRR_FILTER(value)      ((TWIS_SRR_FILTER_Msk & ((value) << TWIS_SRR_FILTER_Pos)))
#define TWIS_SRR_MASK               0x30000307u  /**< \brief (TWIS_SRR) MASK Register */

/* -------- TWIS_HSSRR : (TWIS Offset: 0x3C) (R/W 32) HS-mode Slew Rate Register -------- */
#define TWIS_HSSRR_OFFSET           0x3C         /**< \brief (TWIS_HSSRR offset) HS-mode Slew Rate Register */
#define TWIS_HSSRR_RESETVALUE       0x00000000   /**< \brief (TWIS_HSSRR reset_value) HS-mode Slew Rate Register */

#define TWIS_HSSRR_DADRIVEL_Pos     0            /**< \brief (TWIS_HSSRR) Data Drive Strength LOW */
#define TWIS_HSSRR_DADRIVEL_Msk     (0x7u << TWIS_HSSRR_DADRIVEL_Pos)
#define TWIS_HSSRR_DADRIVEL(value)  ((TWIS_HSSRR_DADRIVEL_Msk & ((value) << TWIS_HSSRR_DADRIVEL_Pos)))
#define TWIS_HSSRR_DASLEW_Pos       8            /**< \brief (TWIS_HSSRR) Data Slew Limit */
#define TWIS_HSSRR_DASLEW_Msk       (0x3u << TWIS_HSSRR_DASLEW_Pos)
#define TWIS_HSSRR_DASLEW(value)    ((TWIS_HSSRR_DASLEW_Msk & ((value) << TWIS_HSSRR_DASLEW_Pos)))
#define TWIS_HSSRR_FILTER_Pos       28           /**< \brief (TWIS_HSSRR) Input Spike Filter Control */
#define TWIS_HSSRR_FILTER_Msk       (0x3u << TWIS_HSSRR_FILTER_Pos)
#define TWIS_HSSRR_FILTER(value)    ((TWIS_HSSRR_FILTER_Msk & ((value) << TWIS_HSSRR_FILTER_Pos)))
#define TWIS_HSSRR_MASK             0x30000307u  /**< \brief (TWIS_HSSRR) MASK Register */

/** \brief TWIS hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
  RwReg   TWIS_CR;            /**< \brief (TWIS Offset: 0x00) Control Register */
  RwReg   TWIS_NBYTES;        /**< \brief (TWIS Offset: 0x04) NBYTES Register */
  RwReg   TWIS_TR;            /**< \brief (TWIS Offset: 0x08) Timing Register */
  RoReg   TWIS_RHR;           /**< \brief (TWIS Offset: 0x0C) Receive Holding Register */
  WoReg   TWIS_THR;           /**< \brief (TWIS Offset: 0x10) Transmit Holding Register */
  RoReg   TWIS_PECR;          /**< \brief (TWIS Offset: 0x14) Packet Error Check Register */
  RoReg   TWIS_SR;            /**< \brief (TWIS Offset: 0x18) Status Register */
  WoReg   TWIS_IER;           /**< \brief (TWIS Offset: 0x1C) Interrupt Enable Register */
  WoReg   TWIS_IDR;           /**< \brief (TWIS Offset: 0x20) Interrupt Disable Register */
  RoReg   TWIS_IMR;           /**< \brief (TWIS Offset: 0x24) Interrupt Mask Register */
  WoReg   TWIS_SCR;           /**< \brief (TWIS Offset: 0x28) Status Clear Register */
  RoReg   TWIS_PR;            /**< \brief (TWIS Offset: 0x2C) Parameter Register */
  RoReg   TWIS_VR;            /**< \brief (TWIS Offset: 0x30) Version Register */
  RwReg   TWIS_HSTR;          /**< \brief (TWIS Offset: 0x34) HS-mode Timing Register */
  RwReg   TWIS_SRR;           /**< \brief (TWIS Offset: 0x38) Slew Rate Register */
  RwReg   TWIS_HSSRR;         /**< \brief (TWIS Offset: 0x3C) HS-mode Slew Rate Register */
} Twis;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/*@}*/

#endif /* _SAM4L_TWIS_COMPONENT_ */
