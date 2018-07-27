/**
 * \file
 *
 * \brief Component description for IISC
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

#ifndef _SAM4L_IISC_COMPONENT_
#define _SAM4L_IISC_COMPONENT_

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR IISC */
/* ========================================================================== */
/** \addtogroup SAM4L_IISC Inter-IC Sound (I2S) Controller */
/*@{*/

#define REV_IISC                    0x100

/* -------- IISC_CR : (IISC Offset: 0x00) ( /W 32) Control Register -------- */
#define IISC_CR_OFFSET              0x00         /**< \brief (IISC_CR offset) Control Register */

#define IISC_CR_RXEN_Pos            0            /**< \brief (IISC_CR) Receive Enable */
#define IISC_CR_RXEN                (0x1u << IISC_CR_RXEN_Pos)
#define   IISC_CR_RXEN_OFF          (0x0u <<  0) /**< \brief (IISC_CR) No effect */
#define   IISC_CR_RXEN_ON           (0x1u <<  0) /**< \brief (IISC_CR) Enables Data Receive if RXDIS is not set */
#define IISC_CR_RXDIS_Pos           1            /**< \brief (IISC_CR) Receive Disable */
#define IISC_CR_RXDIS               (0x1u << IISC_CR_RXDIS_Pos)
#define   IISC_CR_RXDIS_OFF         (0x0u <<  1) /**< \brief (IISC_CR) No effect */
#define   IISC_CR_RXDIS_ON          (0x1u <<  1) /**< \brief (IISC_CR) Disables Data Receive */
#define IISC_CR_CKEN_Pos            2            /**< \brief (IISC_CR) Clocks Enable */
#define IISC_CR_CKEN                (0x1u << IISC_CR_CKEN_Pos)
#define   IISC_CR_CKEN_OFF          (0x0u <<  2) /**< \brief (IISC_CR) No effect */
#define   IISC_CR_CKEN_ON           (0x1u <<  2) /**< \brief (IISC_CR) Enables clocks if CKDIS is not set */
#define IISC_CR_CKDIS_Pos           3            /**< \brief (IISC_CR) Clocks Disable */
#define IISC_CR_CKDIS               (0x1u << IISC_CR_CKDIS_Pos)
#define   IISC_CR_CKDIS_OFF         (0x0u <<  3) /**< \brief (IISC_CR) No effect */
#define   IISC_CR_CKDIS_ON          (0x1u <<  3) /**< \brief (IISC_CR) Disables clocks */
#define IISC_CR_TXEN_Pos            4            /**< \brief (IISC_CR) Transmit Enable */
#define IISC_CR_TXEN                (0x1u << IISC_CR_TXEN_Pos)
#define   IISC_CR_TXEN_OFF          (0x0u <<  4) /**< \brief (IISC_CR) No effect */
#define   IISC_CR_TXEN_ON           (0x1u <<  4) /**< \brief (IISC_CR) Enables Data Transmit if TXDIS is not set */
#define IISC_CR_TXDIS_Pos           5            /**< \brief (IISC_CR) Transmit Disable */
#define IISC_CR_TXDIS               (0x1u << IISC_CR_TXDIS_Pos)
#define   IISC_CR_TXDIS_OFF         (0x0u <<  5) /**< \brief (IISC_CR) No effect */
#define   IISC_CR_TXDIS_ON          (0x1u <<  5) /**< \brief (IISC_CR) Disables Data Transmit */
#define IISC_CR_SWRST_Pos           7            /**< \brief (IISC_CR) Software Reset */
#define IISC_CR_SWRST               (0x1u << IISC_CR_SWRST_Pos)
#define   IISC_CR_SWRST_OFF         (0x0u <<  7) /**< \brief (IISC_CR) No effect */
#define   IISC_CR_SWRST_ON          (0x1u <<  7) /**< \brief (IISC_CR) Performs a software reset. Has priority on any other bit in CR */
#define IISC_CR_MASK                0x000000BFu  /**< \brief (IISC_CR) MASK Register */

/* -------- IISC_MR : (IISC Offset: 0x04) (R/W 32) Mode Register -------- */
#define IISC_MR_OFFSET              0x04         /**< \brief (IISC_MR offset) Mode Register */
#define IISC_MR_RESETVALUE          0x00000000   /**< \brief (IISC_MR reset_value) Mode Register */

#define IISC_MR_MODE_Pos            0            /**< \brief (IISC_MR) Master/Slave/Controller Mode */
#define IISC_MR_MODE                (0x1u << IISC_MR_MODE_Pos)
#define   IISC_MR_MODE_SLAVE        (0x0u <<  0) /**< \brief (IISC_MR) Slave mode (only serial data handled, clocks received from external master or controller) */
#define   IISC_MR_MODE_MASTER       (0x1u <<  0) /**< \brief (IISC_MR) Master mode (clocks generated and output by IISC, serial data handled if CR.RXEN and/or CR.TXEN written to 1) */
#define IISC_MR_DATALENGTH_Pos      2            /**< \brief (IISC_MR) Data Word Length */
#define IISC_MR_DATALENGTH_Msk      (0x7u << IISC_MR_DATALENGTH_Pos)
#define IISC_MR_DATALENGTH(value)   ((IISC_MR_DATALENGTH_Msk & ((value) << IISC_MR_DATALENGTH_Pos)))
#define   IISC_MR_DATALENGTH_32     (0x0u <<  2) /**< \brief (IISC_MR) 32 bits */
#define   IISC_MR_DATALENGTH_24     (0x1u <<  2) /**< \brief (IISC_MR) 24 bits */
#define   IISC_MR_DATALENGTH_20     (0x2u <<  2) /**< \brief (IISC_MR) 20 bits */
#define   IISC_MR_DATALENGTH_18     (0x3u <<  2) /**< \brief (IISC_MR) 18 bits */
#define   IISC_MR_DATALENGTH_16     (0x4u <<  2) /**< \brief (IISC_MR) 16 bits */
#define   IISC_MR_DATALENGTH_16C    (0x5u <<  2) /**< \brief (IISC_MR) 16 bits compact stereo */
#define   IISC_MR_DATALENGTH_8      (0x6u <<  2) /**< \brief (IISC_MR) 8 bits */
#define   IISC_MR_DATALENGTH_8C     (0x7u <<  2) /**< \brief (IISC_MR) 8 bits compact stereo */
#define IISC_MR_RXMONO_Pos          8            /**< \brief (IISC_MR) Receiver Mono */
#define IISC_MR_RXMONO              (0x1u << IISC_MR_RXMONO_Pos)
#define   IISC_MR_RXMONO_STEREO     (0x0u <<  8) /**< \brief (IISC_MR) Normal mode */
#define   IISC_MR_RXMONO_MONO       (0x1u <<  8) /**< \brief (IISC_MR) Left channel data is duplicated to right channel */
#define IISC_MR_RXDMA_Pos           9            /**< \brief (IISC_MR) Single or Multiple DMA Channels for Receiver */
#define IISC_MR_RXDMA               (0x1u << IISC_MR_RXDMA_Pos)
#define   IISC_MR_RXDMA_SINGLE      (0x0u <<  9) /**< \brief (IISC_MR) Single DMA channel */
#define   IISC_MR_RXDMA_MULTIPLE    (0x1u <<  9) /**< \brief (IISC_MR) One DMA channel per data channel */
#define IISC_MR_RXLOOP_Pos          10           /**< \brief (IISC_MR) Loop-back Test Mode */
#define IISC_MR_RXLOOP              (0x1u << IISC_MR_RXLOOP_Pos)
#define   IISC_MR_RXLOOP_OFF        (0x0u << 10) /**< \brief (IISC_MR) Normal mode */
#define   IISC_MR_RXLOOP_ON         (0x1u << 10) /**< \brief (IISC_MR) ISDO internally connected to ISDI */
#define IISC_MR_TXMONO_Pos          12           /**< \brief (IISC_MR) Transmitter Mono */
#define IISC_MR_TXMONO              (0x1u << IISC_MR_TXMONO_Pos)
#define   IISC_MR_TXMONO_STEREO     (0x0u << 12) /**< \brief (IISC_MR) Normal mode */
#define   IISC_MR_TXMONO_MONO       (0x1u << 12) /**< \brief (IISC_MR) Left channel data is duplicated to right channel */
#define IISC_MR_TXDMA_Pos           13           /**< \brief (IISC_MR) Single or Multiple DMA Channels for Transmitter */
#define IISC_MR_TXDMA               (0x1u << IISC_MR_TXDMA_Pos)
#define   IISC_MR_TXDMA_SINGLE      (0x0u << 13) /**< \brief (IISC_MR) Single DMA channel */
#define   IISC_MR_TXDMA_MULTIPLE    (0x1u << 13) /**< \brief (IISC_MR) One DMA channel per data channel */
#define IISC_MR_TXSAME_Pos          14           /**< \brief (IISC_MR) Transmit Data when Underrun */
#define IISC_MR_TXSAME              (0x1u << IISC_MR_TXSAME_Pos)
#define   IISC_MR_TXSAME_ZERO       (0x0u << 14) /**< \brief (IISC_MR) Zero data transmitted in case of underrun */
#define   IISC_MR_TXSAME_SAME       (0x1u << 14) /**< \brief (IISC_MR) Last data transmitted in case of underrun */
#define IISC_MR_IMCKFS_Pos          24           /**< \brief (IISC_MR) Master Clock to fs Ratio */
#define IISC_MR_IMCKFS_Msk          (0x3Fu << IISC_MR_IMCKFS_Pos)
#define IISC_MR_IMCKFS(value)       ((IISC_MR_IMCKFS_Msk & ((value) << IISC_MR_IMCKFS_Pos)))
#define   IISC_MR_IMCKFS_16         (0x0u << 24) /**< \brief (IISC_MR) 16 fs */
#define   IISC_MR_IMCKFS_32         (0x1u << 24) /**< \brief (IISC_MR) 32 fs */
#define   IISC_MR_IMCKFS_64         (0x3u << 24) /**< \brief (IISC_MR) 64 fs */
#define   IISC_MR_IMCKFS_128        (0x7u << 24) /**< \brief (IISC_MR) 128 fs */
#define   IISC_MR_IMCKFS_256        (0xFu << 24) /**< \brief (IISC_MR) 256 fs */
#define   IISC_MR_IMCKFS_384        (0x17u << 24) /**< \brief (IISC_MR) 384 fs */
#define   IISC_MR_IMCKFS_512        (0x1Fu << 24) /**< \brief (IISC_MR) 512 fs */
#define   IISC_MR_IMCKFS_768        (0x2Fu << 24) /**< \brief (IISC_MR) 768 fs */
#define   IISC_MR_IMCKFS_1024       (0x3Fu << 24) /**< \brief (IISC_MR) 1024 fs */
#define IISC_MR_IMCKMODE_Pos        30           /**< \brief (IISC_MR) Master Clock Mode */
#define IISC_MR_IMCKMODE            (0x1u << IISC_MR_IMCKMODE_Pos)
#define   IISC_MR_IMCKMODE_NO_IMCK  (0x0u << 30) /**< \brief (IISC_MR) No IMCK generated */
#define   IISC_MR_IMCKMODE_IMCK     (0x1u << 30) /**< \brief (IISC_MR) IMCK generated */
#define IISC_MR_IWS24_Pos           31           /**< \brief (IISC_MR) IWS Data Slot Width */
#define IISC_MR_IWS24               (0x1u << IISC_MR_IWS24_Pos)
#define   IISC_MR_IWS24_32          (0x0u << 31) /**< \brief (IISC_MR) IWS Data Slot is 32-bit wide for DATALENGTH=18/20/24-bit */
#define   IISC_MR_IWS24_24          (0x1u << 31) /**< \brief (IISC_MR) IWS Data Slot is 24-bit wide for DATALENGTH=18/20/24-bit */
#define IISC_MR_MASK                0xFF00771Du  /**< \brief (IISC_MR) MASK Register */

/* -------- IISC_SR : (IISC Offset: 0x08) (R/  32) Status Register -------- */
#define IISC_SR_OFFSET              0x08         /**< \brief (IISC_SR offset) Status Register */
#define IISC_SR_RESETVALUE          0x00000000   /**< \brief (IISC_SR reset_value) Status Register */

#define IISC_SR_RXEN_Pos            0            /**< \brief (IISC_SR) Receive Enable */
#define IISC_SR_RXEN                (0x1u << IISC_SR_RXEN_Pos)
#define   IISC_SR_RXEN_OFF          (0x0u <<  0) /**< \brief (IISC_SR) Receiver is effectively disabled, following a CR.RXDIS or CR.SWRST request */
#define   IISC_SR_RXEN_ON           (0x1u <<  0) /**< \brief (IISC_SR) Receiver is effectively enabled, following a CR.RXEN request */
#define IISC_SR_RXRDY_Pos           1            /**< \brief (IISC_SR) Receive Ready */
#define IISC_SR_RXRDY               (0x1u << IISC_SR_RXRDY_Pos)
#define   IISC_SR_RXRDY_EMPTY       (0x0u <<  1) /**< \brief (IISC_SR) The register RHR is empty and can't be read */
#define   IISC_SR_RXRDY_FULL        (0x1u <<  1) /**< \brief (IISC_SR) The register RHR is full and is ready to be read */
#define IISC_SR_RXOR_Pos            2            /**< \brief (IISC_SR) Receive Overrun */
#define IISC_SR_RXOR                (0x1u << IISC_SR_RXOR_Pos)
#define   IISC_SR_RXOR_NO           (0x0u <<  2) /**< \brief (IISC_SR) No overrun */
#define   IISC_SR_RXOR_YES          (0x1u <<  2) /**< \brief (IISC_SR) The previous received data has not been read. This data is lost */
#define IISC_SR_TXEN_Pos            4            /**< \brief (IISC_SR) Transmit Enable */
#define IISC_SR_TXEN                (0x1u << IISC_SR_TXEN_Pos)
#define   IISC_SR_TXEN_OFF          (0x0u <<  4) /**< \brief (IISC_SR) Transmitter is effectively disabled, following a CR.TXDIS or CR.SWRST request */
#define   IISC_SR_TXEN_ON           (0x1u <<  4) /**< \brief (IISC_SR) Transmitter is effectively enabled, following a CR.TXEN request */
#define IISC_SR_TXRDY_Pos           5            /**< \brief (IISC_SR) Transmit Ready */
#define IISC_SR_TXRDY               (0x1u << IISC_SR_TXRDY_Pos)
#define   IISC_SR_TXRDY_FULL        (0x0u <<  5) /**< \brief (IISC_SR) The register THR is full and can't be written */
#define   IISC_SR_TXRDY_EMPTY       (0x1u <<  5) /**< \brief (IISC_SR) The register THR is empty and is ready to be written */
#define IISC_SR_TXUR_Pos            6            /**< \brief (IISC_SR) Transmit Underrun */
#define IISC_SR_TXUR                (0x1u << IISC_SR_TXUR_Pos)
#define   IISC_SR_TXUR_NO           (0x0u <<  6) /**< \brief (IISC_SR) No underrun */
#define   IISC_SR_TXUR_YES          (0x1u <<  6) /**< \brief (IISC_SR) The last bit of the last data written to the register THR has been set. Until the next write to THR, data will be sent according to MR.TXSAME field */
#define IISC_SR_RXORCH_Pos          8            /**< \brief (IISC_SR) Receive Overrun Channels */
#define IISC_SR_RXORCH_Msk          (0x3u << IISC_SR_RXORCH_Pos)
#define IISC_SR_RXORCH(value)       ((IISC_SR_RXORCH_Msk & ((value) << IISC_SR_RXORCH_Pos)))
#define   IISC_SR_RXORCH_LEFT       (0x0u <<  8) /**< \brief (IISC_SR) Overrun first occurred on left channel */
#define   IISC_SR_RXORCH_RIGHT      (0x1u <<  8) /**< \brief (IISC_SR) Overrun first occurred on right channel */
#define IISC_SR_TXURCH_Pos          20           /**< \brief (IISC_SR) Transmit Underrun Channels */
#define IISC_SR_TXURCH_Msk          (0x3u << IISC_SR_TXURCH_Pos)
#define IISC_SR_TXURCH(value)       ((IISC_SR_TXURCH_Msk & ((value) << IISC_SR_TXURCH_Pos)))
#define   IISC_SR_TXURCH_LEFT       (0x0u << 20) /**< \brief (IISC_SR) Underrun first occurred on left channel */
#define   IISC_SR_TXURCH_RIGHT      (0x1u << 20) /**< \brief (IISC_SR) Underrun first occurred on right channel */
#define IISC_SR_MASK                0x00300377u  /**< \brief (IISC_SR) MASK Register */

/* -------- IISC_SCR : (IISC Offset: 0x0C) ( /W 32) Status Clear Register -------- */
#define IISC_SCR_OFFSET             0x0C         /**< \brief (IISC_SCR offset) Status Clear Register */

#define IISC_SCR_RXOR_Pos           2            /**< \brief (IISC_SCR) Receive Overrun */
#define IISC_SCR_RXOR               (0x1u << IISC_SCR_RXOR_Pos)
#define   IISC_SCR_RXOR_NO          (0x0u <<  2) /**< \brief (IISC_SCR) No effect */
#define   IISC_SCR_RXOR_CLEAR       (0x1u <<  2) /**< \brief (IISC_SCR) Clears the corresponding SR bit */
#define IISC_SCR_TXUR_Pos           6            /**< \brief (IISC_SCR) Transmit Underrun */
#define IISC_SCR_TXUR               (0x1u << IISC_SCR_TXUR_Pos)
#define   IISC_SCR_TXUR_NO          (0x0u <<  6) /**< \brief (IISC_SCR) No effect */
#define   IISC_SCR_TXUR_CLEAR       (0x1u <<  6) /**< \brief (IISC_SCR) Clears the corresponding SR bit */
#define IISC_SCR_RXORCH_Pos         8            /**< \brief (IISC_SCR) Receive Overrun Channels */
#define IISC_SCR_RXORCH_Msk         (0x3u << IISC_SCR_RXORCH_Pos)
#define IISC_SCR_RXORCH(value)      ((IISC_SCR_RXORCH_Msk & ((value) << IISC_SCR_RXORCH_Pos)))
#define IISC_SCR_TXURCH_Pos         20           /**< \brief (IISC_SCR) Transmit Underrun Channels */
#define IISC_SCR_TXURCH_Msk         (0x3u << IISC_SCR_TXURCH_Pos)
#define IISC_SCR_TXURCH(value)      ((IISC_SCR_TXURCH_Msk & ((value) << IISC_SCR_TXURCH_Pos)))
#define IISC_SCR_MASK               0x00300344u  /**< \brief (IISC_SCR) MASK Register */

/* -------- IISC_SSR : (IISC Offset: 0x10) ( /W 32) Status Set Register -------- */
#define IISC_SSR_OFFSET             0x10         /**< \brief (IISC_SSR offset) Status Set Register */

#define IISC_SSR_RXOR_Pos           2            /**< \brief (IISC_SSR) Receive Overrun */
#define IISC_SSR_RXOR               (0x1u << IISC_SSR_RXOR_Pos)
#define   IISC_SSR_RXOR_NO          (0x0u <<  2) /**< \brief (IISC_SSR) No effect */
#define   IISC_SSR_RXOR_SET         (0x1u <<  2) /**< \brief (IISC_SSR) Sets corresponding SR bit */
#define IISC_SSR_TXUR_Pos           6            /**< \brief (IISC_SSR) Transmit Underrun */
#define IISC_SSR_TXUR               (0x1u << IISC_SSR_TXUR_Pos)
#define   IISC_SSR_TXUR_NO          (0x0u <<  6) /**< \brief (IISC_SSR) No effect */
#define   IISC_SSR_TXUR_SET         (0x1u <<  6) /**< \brief (IISC_SSR) Sets corresponding SR bit */
#define IISC_SSR_RXORCH_Pos         8            /**< \brief (IISC_SSR) Receive Overrun Channels */
#define IISC_SSR_RXORCH_Msk         (0x3u << IISC_SSR_RXORCH_Pos)
#define IISC_SSR_RXORCH(value)      ((IISC_SSR_RXORCH_Msk & ((value) << IISC_SSR_RXORCH_Pos)))
#define IISC_SSR_TXURCH_Pos         20           /**< \brief (IISC_SSR) Transmit Underrun Channels */
#define IISC_SSR_TXURCH_Msk         (0x3u << IISC_SSR_TXURCH_Pos)
#define IISC_SSR_TXURCH(value)      ((IISC_SSR_TXURCH_Msk & ((value) << IISC_SSR_TXURCH_Pos)))
#define IISC_SSR_MASK               0x00300344u  /**< \brief (IISC_SSR) MASK Register */

/* -------- IISC_IER : (IISC Offset: 0x14) ( /W 32) Interrupt Enable Register -------- */
#define IISC_IER_OFFSET             0x14         /**< \brief (IISC_IER offset) Interrupt Enable Register */

#define IISC_IER_RXRDY_Pos          1            /**< \brief (IISC_IER) Receiver Ready Interrupt Enable */
#define IISC_IER_RXRDY              (0x1u << IISC_IER_RXRDY_Pos)
#define   IISC_IER_RXRDY_OFF        (0x0u <<  1) /**< \brief (IISC_IER) No effect */
#define   IISC_IER_RXRDY_ON         (0x1u <<  1) /**< \brief (IISC_IER) Enables the corresponding interrupt */
#define IISC_IER_RXOR_Pos           2            /**< \brief (IISC_IER) Receive Overrun Interrupt Enable */
#define IISC_IER_RXOR               (0x1u << IISC_IER_RXOR_Pos)
#define   IISC_IER_RXOR_OFF         (0x0u <<  2) /**< \brief (IISC_IER) No effect */
#define   IISC_IER_RXOR_ON          (0x1u <<  2) /**< \brief (IISC_IER) Enables the corresponding interrupt */
#define IISC_IER_TXRDY_Pos          5            /**< \brief (IISC_IER) Transmit Ready Interrupt Enable */
#define IISC_IER_TXRDY              (0x1u << IISC_IER_TXRDY_Pos)
#define   IISC_IER_TXRDY_OFF        (0x0u <<  5) /**< \brief (IISC_IER) No effect */
#define   IISC_IER_TXRDY_ON         (0x1u <<  5) /**< \brief (IISC_IER) Enables the corresponding interrupt */
#define IISC_IER_TXUR_Pos           6            /**< \brief (IISC_IER) Transmit Underrun Interrupt Enable */
#define IISC_IER_TXUR               (0x1u << IISC_IER_TXUR_Pos)
#define   IISC_IER_TXUR_OFF         (0x0u <<  6) /**< \brief (IISC_IER) No effect */
#define   IISC_IER_TXUR_ON          (0x1u <<  6) /**< \brief (IISC_IER) Enables the corresponding interrupt */
#define IISC_IER_MASK               0x00000066u  /**< \brief (IISC_IER) MASK Register */

/* -------- IISC_IDR : (IISC Offset: 0x18) ( /W 32) Interrupt Disable Register -------- */
#define IISC_IDR_OFFSET             0x18         /**< \brief (IISC_IDR offset) Interrupt Disable Register */

#define IISC_IDR_RXRDY_Pos          1            /**< \brief (IISC_IDR) Receive Ready Interrupt Disable */
#define IISC_IDR_RXRDY              (0x1u << IISC_IDR_RXRDY_Pos)
#define   IISC_IDR_RXRDY_OFF        (0x0u <<  1) /**< \brief (IISC_IDR) No effect */
#define   IISC_IDR_RXRDY_ON         (0x1u <<  1) /**< \brief (IISC_IDR) Disables the corresponding interrupt */
#define IISC_IDR_RXOR_Pos           2            /**< \brief (IISC_IDR) Receive Overrun Interrupt Disable */
#define IISC_IDR_RXOR               (0x1u << IISC_IDR_RXOR_Pos)
#define   IISC_IDR_RXOR_OFF         (0x0u <<  2) /**< \brief (IISC_IDR) No effect */
#define   IISC_IDR_RXOR_ON          (0x1u <<  2) /**< \brief (IISC_IDR) Disables the corresponding interrupt */
#define IISC_IDR_TXRDY_Pos          5            /**< \brief (IISC_IDR) Transmit Ready Interrupt Disable */
#define IISC_IDR_TXRDY              (0x1u << IISC_IDR_TXRDY_Pos)
#define   IISC_IDR_TXRDY_OFF        (0x0u <<  5) /**< \brief (IISC_IDR) No effect */
#define   IISC_IDR_TXRDY_ON         (0x1u <<  5) /**< \brief (IISC_IDR) Disables the corresponding interrupt */
#define IISC_IDR_TXUR_Pos           6            /**< \brief (IISC_IDR) Transmit Underrun Interrupt Disable */
#define IISC_IDR_TXUR               (0x1u << IISC_IDR_TXUR_Pos)
#define   IISC_IDR_TXUR_OFF         (0x0u <<  6) /**< \brief (IISC_IDR) No effect */
#define   IISC_IDR_TXUR_ON          (0x1u <<  6) /**< \brief (IISC_IDR) Disables the corresponding interrupt */
#define IISC_IDR_MASK               0x00000066u  /**< \brief (IISC_IDR) MASK Register */

/* -------- IISC_IMR : (IISC Offset: 0x1C) (R/  32) Interrupt Mask Register -------- */
#define IISC_IMR_OFFSET             0x1C         /**< \brief (IISC_IMR offset) Interrupt Mask Register */
#define IISC_IMR_RESETVALUE         0x00000000   /**< \brief (IISC_IMR reset_value) Interrupt Mask Register */

#define IISC_IMR_RXRDY_Pos          1            /**< \brief (IISC_IMR) Receive Ready Interrupt Mask */
#define IISC_IMR_RXRDY              (0x1u << IISC_IMR_RXRDY_Pos)
#define   IISC_IMR_RXRDY_DISABLED   (0x0u <<  1) /**< \brief (IISC_IMR) The corresponding interrupt is disabled */
#define   IISC_IMR_RXRDY_ENABLED    (0x1u <<  1) /**< \brief (IISC_IMR) The corresponding interrupt is enabled */
#define IISC_IMR_RXOR_Pos           2            /**< \brief (IISC_IMR) Receive Overrun Interrupt Mask */
#define IISC_IMR_RXOR               (0x1u << IISC_IMR_RXOR_Pos)
#define   IISC_IMR_RXOR_DISABLED    (0x0u <<  2) /**< \brief (IISC_IMR) The corresponding interrupt is disabled */
#define   IISC_IMR_RXOR_ENABLED     (0x1u <<  2) /**< \brief (IISC_IMR) The corresponding interrupt is enabled */
#define IISC_IMR_TXRDY_Pos          5            /**< \brief (IISC_IMR) Transmit Ready Interrupt Mask */
#define IISC_IMR_TXRDY              (0x1u << IISC_IMR_TXRDY_Pos)
#define   IISC_IMR_TXRDY_DISABLED   (0x0u <<  5) /**< \brief (IISC_IMR) The corresponding interrupt is disabled */
#define   IISC_IMR_TXRDY_ENABLED    (0x1u <<  5) /**< \brief (IISC_IMR) The corresponding interrupt is enabled */
#define IISC_IMR_TXUR_Pos           6            /**< \brief (IISC_IMR) Transmit Underrun Interrupt Mask */
#define IISC_IMR_TXUR               (0x1u << IISC_IMR_TXUR_Pos)
#define   IISC_IMR_TXUR_DISABLED    (0x0u <<  6) /**< \brief (IISC_IMR) The corresponding interrupt is disabled */
#define   IISC_IMR_TXUR_ENABLED     (0x1u <<  6) /**< \brief (IISC_IMR) The corresponding interrupt is enabled */
#define IISC_IMR_MASK               0x00000066u  /**< \brief (IISC_IMR) MASK Register */

/* -------- IISC_RHR : (IISC Offset: 0x20) (R/  32) Receive Holding Register -------- */
#define IISC_RHR_OFFSET             0x20         /**< \brief (IISC_RHR offset) Receive Holding Register */
#define IISC_RHR_RESETVALUE         0x00000000   /**< \brief (IISC_RHR reset_value) Receive Holding Register */

#define IISC_RHR_RDAT_Pos           0            /**< \brief (IISC_RHR) Receive Data */
#define IISC_RHR_RDAT_Msk           (0xFFFFFFFFu << IISC_RHR_RDAT_Pos)
#define IISC_RHR_RDAT(value)        ((IISC_RHR_RDAT_Msk & ((value) << IISC_RHR_RDAT_Pos)))
#define IISC_RHR_MASK               0xFFFFFFFFu  /**< \brief (IISC_RHR) MASK Register */

/* -------- IISC_THR : (IISC Offset: 0x24) ( /W 32) Transmit Holding Register -------- */
#define IISC_THR_OFFSET             0x24         /**< \brief (IISC_THR offset) Transmit Holding Register */

#define IISC_THR_TDAT_Pos           0            /**< \brief (IISC_THR) Transmit Data */
#define IISC_THR_TDAT_Msk           (0xFFFFFFFFu << IISC_THR_TDAT_Pos)
#define IISC_THR_TDAT(value)        ((IISC_THR_TDAT_Msk & ((value) << IISC_THR_TDAT_Pos)))
#define IISC_THR_MASK               0xFFFFFFFFu  /**< \brief (IISC_THR) MASK Register */

/* -------- IISC_VERSION : (IISC Offset: 0x28) (R/  32) Version Register -------- */
#define IISC_VERSION_OFFSET         0x28         /**< \brief (IISC_VERSION offset) Version Register */
#define IISC_VERSION_RESETVALUE     0x00000100   /**< \brief (IISC_VERSION reset_value) Version Register */

#define IISC_VERSION_VERSION_Pos    0            /**< \brief (IISC_VERSION) Reserved. Value subject to change. No functionality associated. This is the Atmel internal version of the macrocell. */
#define IISC_VERSION_VERSION_Msk    (0xFFFu << IISC_VERSION_VERSION_Pos)
#define IISC_VERSION_VERSION(value) ((IISC_VERSION_VERSION_Msk & ((value) << IISC_VERSION_VERSION_Pos)))
#define IISC_VERSION_VARIANT_Pos    16           /**< \brief (IISC_VERSION) Reserved. Value subject to change. No functionality associated. */
#define IISC_VERSION_VARIANT_Msk    (0xFu << IISC_VERSION_VARIANT_Pos)
#define IISC_VERSION_VARIANT(value) ((IISC_VERSION_VARIANT_Msk & ((value) << IISC_VERSION_VARIANT_Pos)))
#define IISC_VERSION_MASK           0x000F0FFFu  /**< \brief (IISC_VERSION) MASK Register */

/* -------- IISC_PARAMETER : (IISC Offset: 0x2C) (R/  32) Parameter Register -------- */
#define IISC_PARAMETER_OFFSET       0x2C         /**< \brief (IISC_PARAMETER offset) Parameter Register */
#define IISC_PARAMETER_RESETVALUE   0x00010000   /**< \brief (IISC_PARAMETER reset_value) Parameter Register */

#define IISC_PARAMETER_FORMAT_Pos   7            /**< \brief (IISC_PARAMETER) Data protocol format */
#define IISC_PARAMETER_FORMAT       (0x1u << IISC_PARAMETER_FORMAT_Pos)
#define   IISC_PARAMETER_FORMAT_I2S (0x0u <<  7) /**< \brief (IISC_PARAMETER) I2S format, stereo with IWS low for left channel */
#define IISC_PARAMETER_NBCHAN_Pos   16           /**< \brief (IISC_PARAMETER) Maximum number of channels - 1 */
#define IISC_PARAMETER_NBCHAN_Msk   (0x1Fu << IISC_PARAMETER_NBCHAN_Pos)
#define IISC_PARAMETER_NBCHAN(value) ((IISC_PARAMETER_NBCHAN_Msk & ((value) << IISC_PARAMETER_NBCHAN_Pos)))
#define IISC_PARAMETER_MASK         0x001F0080u  /**< \brief (IISC_PARAMETER) MASK Register */

/** \brief IISC hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
  WoReg   IISC_CR;            /**< \brief (IISC Offset: 0x00) Control Register */
  RwReg   IISC_MR;            /**< \brief (IISC Offset: 0x04) Mode Register */
  RoReg   IISC_SR;            /**< \brief (IISC Offset: 0x08) Status Register */
  WoReg   IISC_SCR;           /**< \brief (IISC Offset: 0x0C) Status Clear Register */
  WoReg   IISC_SSR;           /**< \brief (IISC Offset: 0x10) Status Set Register */
  WoReg   IISC_IER;           /**< \brief (IISC Offset: 0x14) Interrupt Enable Register */
  WoReg   IISC_IDR;           /**< \brief (IISC Offset: 0x18) Interrupt Disable Register */
  RoReg   IISC_IMR;           /**< \brief (IISC Offset: 0x1C) Interrupt Mask Register */
  RoReg   IISC_RHR;           /**< \brief (IISC Offset: 0x20) Receive Holding Register */
  WoReg   IISC_THR;           /**< \brief (IISC Offset: 0x24) Transmit Holding Register */
  RoReg   IISC_VERSION;       /**< \brief (IISC Offset: 0x28) Version Register */
  RoReg   IISC_PARAMETER;     /**< \brief (IISC Offset: 0x2C) Parameter Register */
} Iisc;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/*@}*/

#endif /* _SAM4L_IISC_COMPONENT_ */
