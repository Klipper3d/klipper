/**
 * \file
 *
 * \brief Component description for SPI
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

#ifndef _SAM4L_SPI_COMPONENT_
#define _SAM4L_SPI_COMPONENT_

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR SPI */
/* ========================================================================== */
/** \addtogroup SAM4L_SPI Serial Peripheral Interface */
/*@{*/

#define REV_SPI                     0x211

/* -------- SPI_CR : (SPI Offset: 0x00) ( /W 32) Control Register -------- */
#define SPI_CR_OFFSET               0x00         /**< \brief (SPI_CR offset) Control Register */
#define SPI_CR_RESETVALUE           0x00000000   /**< \brief (SPI_CR reset_value) Control Register */

#define SPI_CR_SPIEN_Pos            0            /**< \brief (SPI_CR) SPI Enable */
#define SPI_CR_SPIEN                (0x1u << SPI_CR_SPIEN_Pos)
#define   SPI_CR_SPIEN_0            (0x0u <<  0) /**< \brief (SPI_CR) No effect. */
#define   SPI_CR_SPIEN_1            (0x1u <<  0) /**< \brief (SPI_CR) Enables the SPI to transfer and receive data. */
#define SPI_CR_SPIDIS_Pos           1            /**< \brief (SPI_CR) SPI Disable */
#define SPI_CR_SPIDIS               (0x1u << SPI_CR_SPIDIS_Pos)
#define   SPI_CR_SPIDIS_0           (0x0u <<  1) /**< \brief (SPI_CR) No effect. */
#define   SPI_CR_SPIDIS_1           (0x1u <<  1) /**< \brief (SPI_CR) Disables the SPI.All pins are set in input mode and no data is received or transmitted.If a transfer is in progress, the transfer is finished before the SPI is disabled.If both SPIEN and SPIDIS are equal to one when the control register is written, the SPI is disabled. */
#define SPI_CR_SWRST_Pos            7            /**< \brief (SPI_CR) SPI Software Reset */
#define SPI_CR_SWRST                (0x1u << SPI_CR_SWRST_Pos)
#define   SPI_CR_SWRST_0            (0x0u <<  7) /**< \brief (SPI_CR) No effect. */
#define   SPI_CR_SWRST_1            (0x1u <<  7) /**< \brief (SPI_CR) Reset the SPI. A software-triggered hardware reset of the SPI interface is performed. */
#define SPI_CR_FLUSHFIFO_Pos        8            /**< \brief (SPI_CR) Flush FIFO command */
#define SPI_CR_FLUSHFIFO            (0x1u << SPI_CR_FLUSHFIFO_Pos)
#define SPI_CR_LASTXFER_Pos         24           /**< \brief (SPI_CR) Last Transfer */
#define SPI_CR_LASTXFER             (0x1u << SPI_CR_LASTXFER_Pos)
#define   SPI_CR_LASTXFER_0         (0x0u << 24) /**< \brief (SPI_CR) No effect. */
#define   SPI_CR_LASTXFER_1         (0x1u << 24) /**< \brief (SPI_CR) The current NPCS will be deasserted after the character written in TD has been transferred. When CSAAT is set, thisallows to close the communication with the current serial peripheral by raising the corresponding NPCS line as soon as TDtransfer has completed. */
#define SPI_CR_MASK                 0x01000183u  /**< \brief (SPI_CR) MASK Register */

/* -------- SPI_MR : (SPI Offset: 0x04) (R/W 32) Mode Register -------- */
#define SPI_MR_OFFSET               0x04         /**< \brief (SPI_MR offset) Mode Register */
#define SPI_MR_RESETVALUE           0x00000000   /**< \brief (SPI_MR reset_value) Mode Register */

#define SPI_MR_MSTR_Pos             0            /**< \brief (SPI_MR) Master/Slave Mode */
#define SPI_MR_MSTR                 (0x1u << SPI_MR_MSTR_Pos)
#define   SPI_MR_MSTR_0             (0x0u <<  0) /**< \brief (SPI_MR) SPI is in Slave mode. */
#define   SPI_MR_MSTR_1             (0x1u <<  0) /**< \brief (SPI_MR) SPI is in Master mode. */
#define SPI_MR_PS_Pos               1            /**< \brief (SPI_MR) Peripheral Select */
#define SPI_MR_PS                   (0x1u << SPI_MR_PS_Pos)
#define   SPI_MR_PS_0               (0x0u <<  1) /**< \brief (SPI_MR) Fixed Peripheral Select. */
#define   SPI_MR_PS_1               (0x1u <<  1) /**< \brief (SPI_MR) Variable Peripheral Select. */
#define SPI_MR_PCSDEC_Pos           2            /**< \brief (SPI_MR) Chip Select Decode */
#define SPI_MR_PCSDEC               (0x1u << SPI_MR_PCSDEC_Pos)
#define   SPI_MR_PCSDEC_0           (0x0u <<  2) /**< \brief (SPI_MR) The chip selects are directly connected to a peripheral device. */
#define   SPI_MR_PCSDEC_1           (0x1u <<  2) /**< \brief (SPI_MR) The four chip select lines are connected to a 4- to 16-bit decoder.When PCSDEC equals one, up to 15 Chip Select signals can be generated with the four lines using an external 4- to 16-bitdecoder. The Chip Select Registers define the characteristics of the 16 chip selects according to the following rules:CSR0 defines peripheral chip select signals 0 to 3.CSR1 defines peripheral chip select signals 4 to 7.CSR2 defines peripheral chip select signals 8 to 11.CSR3 defines peripheral chip select signals 12 to 15. */
#define SPI_MR_MODFDIS_Pos          4            /**< \brief (SPI_MR) Mode Fault Detection */
#define SPI_MR_MODFDIS              (0x1u << SPI_MR_MODFDIS_Pos)
#define   SPI_MR_MODFDIS_0          (0x0u <<  4) /**< \brief (SPI_MR) Mode fault detection is enabled. */
#define   SPI_MR_MODFDIS_1          (0x1u <<  4) /**< \brief (SPI_MR) Mode fault detection is disabled. */
#define SPI_MR_WDRBT_Pos            5            /**< \brief (SPI_MR) wait data read before transfer */
#define SPI_MR_WDRBT                (0x1u << SPI_MR_WDRBT_Pos)
#define SPI_MR_RXFIFOEN_Pos         6            /**< \brief (SPI_MR) FIFO in Reception Enable */
#define SPI_MR_RXFIFOEN             (0x1u << SPI_MR_RXFIFOEN_Pos)
#define SPI_MR_LLB_Pos              7            /**< \brief (SPI_MR) Local Loopback Enable */
#define SPI_MR_LLB                  (0x1u << SPI_MR_LLB_Pos)
#define   SPI_MR_LLB_0              (0x0u <<  7) /**< \brief (SPI_MR) Local loopback path disabled. */
#define   SPI_MR_LLB_1              (0x1u <<  7) /**< \brief (SPI_MR) Local loopback path enabled.LLB controls the local loopback on the data serializer for testing in Master Mode only. */
#define SPI_MR_PCS_Pos              16           /**< \brief (SPI_MR) Peripheral Chip Select */
#define SPI_MR_PCS_Msk              (0xFu << SPI_MR_PCS_Pos)
#define SPI_MR_PCS(value)           ((SPI_MR_PCS_Msk & ((value) << SPI_MR_PCS_Pos)))
#define SPI_MR_DLYBCS_Pos           24           /**< \brief (SPI_MR) Delay Between Chip Selects */
#define SPI_MR_DLYBCS_Msk           (0xFFu << SPI_MR_DLYBCS_Pos)
#define SPI_MR_DLYBCS(value)        ((SPI_MR_DLYBCS_Msk & ((value) << SPI_MR_DLYBCS_Pos)))
#define SPI_MR_MASK                 0xFF0F00F7u  /**< \brief (SPI_MR) MASK Register */

/* -------- SPI_RDR : (SPI Offset: 0x08) (R/  32) Receive Data Register -------- */
#define SPI_RDR_OFFSET              0x08         /**< \brief (SPI_RDR offset) Receive Data Register */
#define SPI_RDR_RESETVALUE          0x00000000   /**< \brief (SPI_RDR reset_value) Receive Data Register */

#define SPI_RDR_RD_Pos              0            /**< \brief (SPI_RDR) Receive Data */
#define SPI_RDR_RD_Msk              (0xFFFFu << SPI_RDR_RD_Pos)
#define SPI_RDR_RD(value)           ((SPI_RDR_RD_Msk & ((value) << SPI_RDR_RD_Pos)))
#define SPI_RDR_PCS_Pos             16           /**< \brief (SPI_RDR) Peripheral Chip Select */
#define SPI_RDR_PCS_Msk             (0xFu << SPI_RDR_PCS_Pos)
#define SPI_RDR_PCS(value)          ((SPI_RDR_PCS_Msk & ((value) << SPI_RDR_PCS_Pos)))
#define SPI_RDR_MASK                0x000FFFFFu  /**< \brief (SPI_RDR) MASK Register */

/* -------- SPI_TDR : (SPI Offset: 0x0C) ( /W 32) Transmit Data Register -------- */
#define SPI_TDR_OFFSET              0x0C         /**< \brief (SPI_TDR offset) Transmit Data Register */
#define SPI_TDR_RESETVALUE          0x00000000   /**< \brief (SPI_TDR reset_value) Transmit Data Register */

#define SPI_TDR_TD_Pos              0            /**< \brief (SPI_TDR) Transmit Data */
#define SPI_TDR_TD_Msk              (0xFFFFu << SPI_TDR_TD_Pos)
#define SPI_TDR_TD(value)           ((SPI_TDR_TD_Msk & ((value) << SPI_TDR_TD_Pos)))
#define SPI_TDR_PCS_Pos             16           /**< \brief (SPI_TDR) Peripheral Chip Select */
#define SPI_TDR_PCS_Msk             (0xFu << SPI_TDR_PCS_Pos)
#define SPI_TDR_PCS(value)          ((SPI_TDR_PCS_Msk & ((value) << SPI_TDR_PCS_Pos)))
#define SPI_TDR_LASTXFER_Pos        24           /**< \brief (SPI_TDR) Last Transfer */
#define SPI_TDR_LASTXFER            (0x1u << SPI_TDR_LASTXFER_Pos)
#define   SPI_TDR_LASTXFER_0        (0x0u << 24) /**< \brief (SPI_TDR) No effect. */
#define   SPI_TDR_LASTXFER_1        (0x1u << 24) /**< \brief (SPI_TDR) The current NPCS will be deasserted after the character written in TD has been transferred. When CSAAT is set, thisallows to close the communication with the current serial peripheral by raising the corresponding NPCS line as soon as TDtransfer has completed. */
#define SPI_TDR_MASK                0x010FFFFFu  /**< \brief (SPI_TDR) MASK Register */

/* -------- SPI_SR : (SPI Offset: 0x10) (R/  32) Status Register -------- */
#define SPI_SR_OFFSET               0x10         /**< \brief (SPI_SR offset) Status Register */
#define SPI_SR_RESETVALUE           0x000000F0   /**< \brief (SPI_SR reset_value) Status Register */

#define SPI_SR_RDRF_Pos             0            /**< \brief (SPI_SR) Receive Data Register Full */
#define SPI_SR_RDRF                 (0x1u << SPI_SR_RDRF_Pos)
#define   SPI_SR_RDRF_0             (0x0u <<  0) /**< \brief (SPI_SR) No data has been received since the last read of RDR */
#define   SPI_SR_RDRF_1             (0x1u <<  0) /**< \brief (SPI_SR) Data has been received and the received data has been transferred from the serializer to RDR since the last readof RDR. */
#define SPI_SR_TDRE_Pos             1            /**< \brief (SPI_SR) Transmit Data Register Empty */
#define SPI_SR_TDRE                 (0x1u << SPI_SR_TDRE_Pos)
#define   SPI_SR_TDRE_0             (0x0u <<  1) /**< \brief (SPI_SR) Data has been written to TDR and not yet transferred to the serializer. */
#define   SPI_SR_TDRE_1             (0x1u <<  1) /**< \brief (SPI_SR) The last data written in the Transmit Data Register has been transferred to the serializer.TDRE equals zero when the SPI is disabled or at reset. The SPI enable command sets this bit to one. */
#define SPI_SR_MODF_Pos             2            /**< \brief (SPI_SR) Mode Fault Error */
#define SPI_SR_MODF                 (0x1u << SPI_SR_MODF_Pos)
#define   SPI_SR_MODF_0             (0x0u <<  2) /**< \brief (SPI_SR) No Mode Fault has been detected since the last read of SR. */
#define   SPI_SR_MODF_1             (0x1u <<  2) /**< \brief (SPI_SR) A Mode Fault occurred since the last read of the SR. */
#define SPI_SR_OVRES_Pos            3            /**< \brief (SPI_SR) Overrun Error Status */
#define SPI_SR_OVRES                (0x1u << SPI_SR_OVRES_Pos)
#define   SPI_SR_OVRES_0            (0x0u <<  3) /**< \brief (SPI_SR) No overrun has been detected since the last read of SR. */
#define   SPI_SR_OVRES_1            (0x1u <<  3) /**< \brief (SPI_SR) An overrun has occurred since the last read of SR. */
#define SPI_SR_ENDRX_Pos            4            /**< \brief (SPI_SR) End of RX buffer */
#define SPI_SR_ENDRX                (0x1u << SPI_SR_ENDRX_Pos)
#define   SPI_SR_ENDRX_0            (0x0u <<  4) /**< \brief (SPI_SR) The Receive Counter Register has not reached 0 since the last write in RCR or RNCR. */
#define   SPI_SR_ENDRX_1            (0x1u <<  4) /**< \brief (SPI_SR) The Receive Counter Register has reached 0 since the last write in RCR or RNCR. */
#define SPI_SR_ENDTX_Pos            5            /**< \brief (SPI_SR) End of TX buffer */
#define SPI_SR_ENDTX                (0x1u << SPI_SR_ENDTX_Pos)
#define   SPI_SR_ENDTX_0            (0x0u <<  5) /**< \brief (SPI_SR) The Transmit Counter Register has not reached 0 since the last write in TCR or TNCR. */
#define   SPI_SR_ENDTX_1            (0x1u <<  5) /**< \brief (SPI_SR) The Transmit Counter Register has reached 0 since the last write in TCR or TNCR. */
#define SPI_SR_RXBUFF_Pos           6            /**< \brief (SPI_SR) RX Buffer Full */
#define SPI_SR_RXBUFF               (0x1u << SPI_SR_RXBUFF_Pos)
#define   SPI_SR_RXBUFF_0           (0x0u <<  6) /**< \brief (SPI_SR) RCR or RNCR has a value other than 0. */
#define   SPI_SR_RXBUFF_1           (0x1u <<  6) /**< \brief (SPI_SR) Both RCR and RNCR has a value of 0. */
#define SPI_SR_TXBUFE_Pos           7            /**< \brief (SPI_SR) TX Buffer Empty */
#define SPI_SR_TXBUFE               (0x1u << SPI_SR_TXBUFE_Pos)
#define   SPI_SR_TXBUFE_0           (0x0u <<  7) /**< \brief (SPI_SR) TCR or TNCR has a value other than 0. */
#define   SPI_SR_TXBUFE_1           (0x1u <<  7) /**< \brief (SPI_SR) Both TCR and TNCR has a value of 0. */
#define SPI_SR_NSSR_Pos             8            /**< \brief (SPI_SR) NSS Rising */
#define SPI_SR_NSSR                 (0x1u << SPI_SR_NSSR_Pos)
#define   SPI_SR_NSSR_0             (0x0u <<  8) /**< \brief (SPI_SR) No rising edge detected on NSS pin since last read. */
#define   SPI_SR_NSSR_1             (0x1u <<  8) /**< \brief (SPI_SR) A rising edge occurred on NSS pin since last read. */
#define SPI_SR_TXEMPTY_Pos          9            /**< \brief (SPI_SR) Transmission Registers Empty */
#define SPI_SR_TXEMPTY              (0x1u << SPI_SR_TXEMPTY_Pos)
#define   SPI_SR_TXEMPTY_0          (0x0u <<  9) /**< \brief (SPI_SR) As soon as data is written in TDR. */
#define   SPI_SR_TXEMPTY_1          (0x1u <<  9) /**< \brief (SPI_SR) TDR and internal shifter are empty. If a transfer delay has been defined, TXEMPTY is set after the completion ofsuch delay. */
#define SPI_SR_UNDES_Pos            10           /**< \brief (SPI_SR) Underrun Error Status (Slave Mode Only) */
#define SPI_SR_UNDES                (0x1u << SPI_SR_UNDES_Pos)
#define SPI_SR_SPIENS_Pos           16           /**< \brief (SPI_SR) SPI Enable Status */
#define SPI_SR_SPIENS               (0x1u << SPI_SR_SPIENS_Pos)
#define   SPI_SR_SPIENS_0           (0x0u << 16) /**< \brief (SPI_SR) SPI is disabled. */
#define   SPI_SR_SPIENS_1           (0x1u << 16) /**< \brief (SPI_SR) SPI is enabled. */
#define SPI_SR_MASK                 0x000107FFu  /**< \brief (SPI_SR) MASK Register */

/* -------- SPI_IER : (SPI Offset: 0x14) ( /W 32) Interrupt Enable Register -------- */
#define SPI_IER_OFFSET              0x14         /**< \brief (SPI_IER offset) Interrupt Enable Register */
#define SPI_IER_RESETVALUE          0x00000000   /**< \brief (SPI_IER reset_value) Interrupt Enable Register */

#define SPI_IER_RDRF_Pos            0            /**< \brief (SPI_IER) Receive Data Register Full Interrupt Enable */
#define SPI_IER_RDRF                (0x1u << SPI_IER_RDRF_Pos)
#define   SPI_IER_RDRF_0            (0x0u <<  0) /**< \brief (SPI_IER) No effect. */
#define   SPI_IER_RDRF_1            (0x1u <<  0) /**< \brief (SPI_IER) Enables the corresponding interrupt. */
#define SPI_IER_TDRE_Pos            1            /**< \brief (SPI_IER) Transmit Data Register Empty Interrupt Enable */
#define SPI_IER_TDRE                (0x1u << SPI_IER_TDRE_Pos)
#define   SPI_IER_TDRE_0            (0x0u <<  1) /**< \brief (SPI_IER) No effect. */
#define   SPI_IER_TDRE_1            (0x1u <<  1) /**< \brief (SPI_IER) Enables the corresponding interrupt. */
#define SPI_IER_MODF_Pos            2            /**< \brief (SPI_IER) Mode Fault Error Interrupt Enable */
#define SPI_IER_MODF                (0x1u << SPI_IER_MODF_Pos)
#define   SPI_IER_MODF_0            (0x0u <<  2) /**< \brief (SPI_IER) No effect. */
#define   SPI_IER_MODF_1            (0x1u <<  2) /**< \brief (SPI_IER) Enables the corresponding interrupt. */
#define SPI_IER_OVRES_Pos           3            /**< \brief (SPI_IER) Overrun Error Interrupt Enable */
#define SPI_IER_OVRES               (0x1u << SPI_IER_OVRES_Pos)
#define   SPI_IER_OVRES_0           (0x0u <<  3) /**< \brief (SPI_IER) No effect. */
#define   SPI_IER_OVRES_1           (0x1u <<  3) /**< \brief (SPI_IER) Enables the corresponding interrupt. */
#define SPI_IER_ENDRX_Pos           4            /**< \brief (SPI_IER) End of Receive Buffer Interrupt Enable */
#define SPI_IER_ENDRX               (0x1u << SPI_IER_ENDRX_Pos)
#define   SPI_IER_ENDRX_0           (0x0u <<  4) /**< \brief (SPI_IER) No effect. */
#define   SPI_IER_ENDRX_1           (0x1u <<  4) /**< \brief (SPI_IER) Enables the corresponding interrupt. */
#define SPI_IER_ENDTX_Pos           5            /**< \brief (SPI_IER) End of Transmit Buffer Interrupt Enable */
#define SPI_IER_ENDTX               (0x1u << SPI_IER_ENDTX_Pos)
#define   SPI_IER_ENDTX_0           (0x0u <<  5) /**< \brief (SPI_IER) No effect. */
#define   SPI_IER_ENDTX_1           (0x1u <<  5) /**< \brief (SPI_IER) Enables the corresponding interrupt. */
#define SPI_IER_RXBUFF_Pos          6            /**< \brief (SPI_IER) Receive Buffer Full Interrupt Enable */
#define SPI_IER_RXBUFF              (0x1u << SPI_IER_RXBUFF_Pos)
#define   SPI_IER_RXBUFF_0          (0x0u <<  6) /**< \brief (SPI_IER) No effect. */
#define   SPI_IER_RXBUFF_1          (0x1u <<  6) /**< \brief (SPI_IER) Enables the corresponding interrupt. */
#define SPI_IER_TXBUFE_Pos          7            /**< \brief (SPI_IER) Transmit Buffer Empty Interrupt Enable */
#define SPI_IER_TXBUFE              (0x1u << SPI_IER_TXBUFE_Pos)
#define   SPI_IER_TXBUFE_0          (0x0u <<  7) /**< \brief (SPI_IER) No effect. */
#define   SPI_IER_TXBUFE_1          (0x1u <<  7) /**< \brief (SPI_IER) Enables the corresponding interrupt. */
#define SPI_IER_NSSR_Pos            8            /**< \brief (SPI_IER) NSS Rising Interrupt Enable */
#define SPI_IER_NSSR                (0x1u << SPI_IER_NSSR_Pos)
#define   SPI_IER_NSSR_0            (0x0u <<  8) /**< \brief (SPI_IER) No effect. */
#define   SPI_IER_NSSR_1            (0x1u <<  8) /**< \brief (SPI_IER) Enables the corresponding interrupt. */
#define SPI_IER_TXEMPTY_Pos         9            /**< \brief (SPI_IER) Transmission Registers Empty Enable */
#define SPI_IER_TXEMPTY             (0x1u << SPI_IER_TXEMPTY_Pos)
#define   SPI_IER_TXEMPTY_0         (0x0u <<  9) /**< \brief (SPI_IER) No effect. */
#define   SPI_IER_TXEMPTY_1         (0x1u <<  9) /**< \brief (SPI_IER) Enables the corresponding interrupt. */
#define SPI_IER_UNDES_Pos           10           /**< \brief (SPI_IER) Underrun Error Interrupt Enable */
#define SPI_IER_UNDES               (0x1u << SPI_IER_UNDES_Pos)
#define SPI_IER_MASK                0x000007FFu  /**< \brief (SPI_IER) MASK Register */

/* -------- SPI_IDR : (SPI Offset: 0x18) ( /W 32) Interrupt Disable Register -------- */
#define SPI_IDR_OFFSET              0x18         /**< \brief (SPI_IDR offset) Interrupt Disable Register */
#define SPI_IDR_RESETVALUE          0x00000000   /**< \brief (SPI_IDR reset_value) Interrupt Disable Register */

#define SPI_IDR_RDRF_Pos            0            /**< \brief (SPI_IDR) Receive Data Register Full Interrupt Disable */
#define SPI_IDR_RDRF                (0x1u << SPI_IDR_RDRF_Pos)
#define   SPI_IDR_RDRF_0            (0x0u <<  0) /**< \brief (SPI_IDR) No effect. */
#define   SPI_IDR_RDRF_1            (0x1u <<  0) /**< \brief (SPI_IDR) Disables the corresponding interrupt. */
#define SPI_IDR_TDRE_Pos            1            /**< \brief (SPI_IDR) Transmit Data Register Empty Interrupt Disable */
#define SPI_IDR_TDRE                (0x1u << SPI_IDR_TDRE_Pos)
#define   SPI_IDR_TDRE_0            (0x0u <<  1) /**< \brief (SPI_IDR) No effect. */
#define   SPI_IDR_TDRE_1            (0x1u <<  1) /**< \brief (SPI_IDR) Disables the corresponding interrupt. */
#define SPI_IDR_MODF_Pos            2            /**< \brief (SPI_IDR) Mode Fault Error Interrupt Disable */
#define SPI_IDR_MODF                (0x1u << SPI_IDR_MODF_Pos)
#define   SPI_IDR_MODF_0            (0x0u <<  2) /**< \brief (SPI_IDR) No effect. */
#define   SPI_IDR_MODF_1            (0x1u <<  2) /**< \brief (SPI_IDR) Disables the corresponding interrupt. */
#define SPI_IDR_OVRES_Pos           3            /**< \brief (SPI_IDR) Overrun Error Interrupt Disable */
#define SPI_IDR_OVRES               (0x1u << SPI_IDR_OVRES_Pos)
#define   SPI_IDR_OVRES_0           (0x0u <<  3) /**< \brief (SPI_IDR) No effect. */
#define   SPI_IDR_OVRES_1           (0x1u <<  3) /**< \brief (SPI_IDR) Disables the corresponding interrupt. */
#define SPI_IDR_ENDRX_Pos           4            /**< \brief (SPI_IDR) End of Receive Buffer Interrupt Disable */
#define SPI_IDR_ENDRX               (0x1u << SPI_IDR_ENDRX_Pos)
#define   SPI_IDR_ENDRX_0           (0x0u <<  4) /**< \brief (SPI_IDR) No effect. */
#define   SPI_IDR_ENDRX_1           (0x1u <<  4) /**< \brief (SPI_IDR) Disables the corresponding interrupt. */
#define SPI_IDR_ENDTX_Pos           5            /**< \brief (SPI_IDR) End of Transmit Buffer Interrupt Disable */
#define SPI_IDR_ENDTX               (0x1u << SPI_IDR_ENDTX_Pos)
#define   SPI_IDR_ENDTX_0           (0x0u <<  5) /**< \brief (SPI_IDR) No effect. */
#define   SPI_IDR_ENDTX_1           (0x1u <<  5) /**< \brief (SPI_IDR) Disables the corresponding interrupt. */
#define SPI_IDR_RXBUFF_Pos          6            /**< \brief (SPI_IDR) Receive Buffer Full Interrupt Disable */
#define SPI_IDR_RXBUFF              (0x1u << SPI_IDR_RXBUFF_Pos)
#define   SPI_IDR_RXBUFF_0          (0x0u <<  6) /**< \brief (SPI_IDR) No effect. */
#define   SPI_IDR_RXBUFF_1          (0x1u <<  6) /**< \brief (SPI_IDR) Disables the corresponding interrupt. */
#define SPI_IDR_TXBUFE_Pos          7            /**< \brief (SPI_IDR) Transmit Buffer Empty Interrupt Disable */
#define SPI_IDR_TXBUFE              (0x1u << SPI_IDR_TXBUFE_Pos)
#define   SPI_IDR_TXBUFE_0          (0x0u <<  7) /**< \brief (SPI_IDR) No effect. */
#define   SPI_IDR_TXBUFE_1          (0x1u <<  7) /**< \brief (SPI_IDR) Disables the corresponding interrupt. */
#define SPI_IDR_NSSR_Pos            8            /**< \brief (SPI_IDR) NSS Rising Interrupt Disable */
#define SPI_IDR_NSSR                (0x1u << SPI_IDR_NSSR_Pos)
#define   SPI_IDR_NSSR_0            (0x0u <<  8) /**< \brief (SPI_IDR) No effect. */
#define   SPI_IDR_NSSR_1            (0x1u <<  8) /**< \brief (SPI_IDR) Disables the corresponding interrupt. */
#define SPI_IDR_TXEMPTY_Pos         9            /**< \brief (SPI_IDR) Transmission Registers Empty Disable */
#define SPI_IDR_TXEMPTY             (0x1u << SPI_IDR_TXEMPTY_Pos)
#define   SPI_IDR_TXEMPTY_0         (0x0u <<  9) /**< \brief (SPI_IDR) No effect. */
#define   SPI_IDR_TXEMPTY_1         (0x1u <<  9) /**< \brief (SPI_IDR) Disables the corresponding interrupt. */
#define SPI_IDR_UNDES_Pos           10           /**< \brief (SPI_IDR) Underrun Error Interrupt Disable */
#define SPI_IDR_UNDES               (0x1u << SPI_IDR_UNDES_Pos)
#define SPI_IDR_MASK                0x000007FFu  /**< \brief (SPI_IDR) MASK Register */

/* -------- SPI_IMR : (SPI Offset: 0x1C) (R/  32) Interrupt Mask Register -------- */
#define SPI_IMR_OFFSET              0x1C         /**< \brief (SPI_IMR offset) Interrupt Mask Register */
#define SPI_IMR_RESETVALUE          0x00000000   /**< \brief (SPI_IMR reset_value) Interrupt Mask Register */

#define SPI_IMR_RDRF_Pos            0            /**< \brief (SPI_IMR) Receive Data Register Full Interrupt Mask */
#define SPI_IMR_RDRF                (0x1u << SPI_IMR_RDRF_Pos)
#define   SPI_IMR_RDRF_0            (0x0u <<  0) /**< \brief (SPI_IMR) The corresponding interrupt is not enabled. */
#define   SPI_IMR_RDRF_1            (0x1u <<  0) /**< \brief (SPI_IMR) The corresponding interrupt is enabled. */
#define SPI_IMR_TDRE_Pos            1            /**< \brief (SPI_IMR) Transmit Data Register Empty Interrupt Mask */
#define SPI_IMR_TDRE                (0x1u << SPI_IMR_TDRE_Pos)
#define   SPI_IMR_TDRE_0            (0x0u <<  1) /**< \brief (SPI_IMR) The corresponding interrupt is not enabled. */
#define   SPI_IMR_TDRE_1            (0x1u <<  1) /**< \brief (SPI_IMR) The corresponding interrupt is enabled. */
#define SPI_IMR_MODF_Pos            2            /**< \brief (SPI_IMR) Mode Fault Error Interrupt Mask */
#define SPI_IMR_MODF                (0x1u << SPI_IMR_MODF_Pos)
#define   SPI_IMR_MODF_0            (0x0u <<  2) /**< \brief (SPI_IMR) The corresponding interrupt is not enabled. */
#define   SPI_IMR_MODF_1            (0x1u <<  2) /**< \brief (SPI_IMR) The corresponding interrupt is enabled. */
#define SPI_IMR_OVRES_Pos           3            /**< \brief (SPI_IMR) Overrun Error Interrupt Mask */
#define SPI_IMR_OVRES               (0x1u << SPI_IMR_OVRES_Pos)
#define   SPI_IMR_OVRES_0           (0x0u <<  3) /**< \brief (SPI_IMR) The corresponding interrupt is not enabled. */
#define   SPI_IMR_OVRES_1           (0x1u <<  3) /**< \brief (SPI_IMR) The corresponding interrupt is enabled. */
#define SPI_IMR_ENDRX_Pos           4            /**< \brief (SPI_IMR) End of Receive Buffer Interrupt Mask */
#define SPI_IMR_ENDRX               (0x1u << SPI_IMR_ENDRX_Pos)
#define   SPI_IMR_ENDRX_0           (0x0u <<  4) /**< \brief (SPI_IMR) The corresponding interrupt is not enabled. */
#define   SPI_IMR_ENDRX_1           (0x1u <<  4) /**< \brief (SPI_IMR) The corresponding interrupt is enabled. */
#define SPI_IMR_ENDTX_Pos           5            /**< \brief (SPI_IMR) End of Transmit Buffer Interrupt Mask */
#define SPI_IMR_ENDTX               (0x1u << SPI_IMR_ENDTX_Pos)
#define   SPI_IMR_ENDTX_0           (0x0u <<  5) /**< \brief (SPI_IMR) The corresponding interrupt is not enabled. */
#define   SPI_IMR_ENDTX_1           (0x1u <<  5) /**< \brief (SPI_IMR) The corresponding interrupt is enabled. */
#define SPI_IMR_RXBUFF_Pos          6            /**< \brief (SPI_IMR) Receive Buffer Full Interrupt Mask */
#define SPI_IMR_RXBUFF              (0x1u << SPI_IMR_RXBUFF_Pos)
#define   SPI_IMR_RXBUFF_0          (0x0u <<  6) /**< \brief (SPI_IMR) The corresponding interrupt is not enabled. */
#define   SPI_IMR_RXBUFF_1          (0x1u <<  6) /**< \brief (SPI_IMR) The corresponding interrupt is enabled. */
#define SPI_IMR_TXBUFE_Pos          7            /**< \brief (SPI_IMR) Transmit Buffer Empty Interrupt Mask */
#define SPI_IMR_TXBUFE              (0x1u << SPI_IMR_TXBUFE_Pos)
#define   SPI_IMR_TXBUFE_0          (0x0u <<  7) /**< \brief (SPI_IMR) The corresponding interrupt is not enabled. */
#define   SPI_IMR_TXBUFE_1          (0x1u <<  7) /**< \brief (SPI_IMR) The corresponding interrupt is enabled. */
#define SPI_IMR_NSSR_Pos            8            /**< \brief (SPI_IMR) NSS Rising Interrupt Mask */
#define SPI_IMR_NSSR                (0x1u << SPI_IMR_NSSR_Pos)
#define   SPI_IMR_NSSR_0            (0x0u <<  8) /**< \brief (SPI_IMR) The corresponding interrupt is not enabled. */
#define   SPI_IMR_NSSR_1            (0x1u <<  8) /**< \brief (SPI_IMR) The corresponding interrupt is enabled. */
#define SPI_IMR_TXEMPTY_Pos         9            /**< \brief (SPI_IMR) Transmission Registers Empty Mask */
#define SPI_IMR_TXEMPTY             (0x1u << SPI_IMR_TXEMPTY_Pos)
#define   SPI_IMR_TXEMPTY_0         (0x0u <<  9) /**< \brief (SPI_IMR) The corresponding interrupt is not enabled. */
#define   SPI_IMR_TXEMPTY_1         (0x1u <<  9) /**< \brief (SPI_IMR) The corresponding interrupt is enabled. */
#define SPI_IMR_UNDES_Pos           10           /**< \brief (SPI_IMR) Underrun Error Interrupt Mask */
#define SPI_IMR_UNDES               (0x1u << SPI_IMR_UNDES_Pos)
#define SPI_IMR_MASK                0x000007FFu  /**< \brief (SPI_IMR) MASK Register */

/* -------- SPI_CSR : (SPI Offset: 0x30) (R/W 32) Chip Select Register -------- */
#define SPI_CSR_OFFSET              0x30         /**< \brief (SPI_CSR offset) Chip Select Register */
#define SPI_CSR_RESETVALUE          0x00000000   /**< \brief (SPI_CSR reset_value) Chip Select Register */

#define SPI_CSR_CPOL_Pos            0            /**< \brief (SPI_CSR) Clock Polarity */
#define SPI_CSR_CPOL                (0x1u << SPI_CSR_CPOL_Pos)
#define   SPI_CSR_CPOL_0            (0x0u <<  0) /**< \brief (SPI_CSR) The inactive state value of SPCK is logic level zero. */
#define   SPI_CSR_CPOL_1            (0x1u <<  0) /**< \brief (SPI_CSR) The inactive state value of SPCK is logic level one.CPOL is used to determine the inactive state value of the serial clock (SPCK). It is used with NCPHA to produce therequired clock/data relationship between master and slave devices. */
#define SPI_CSR_NCPHA_Pos           1            /**< \brief (SPI_CSR) Clock Phase */
#define SPI_CSR_NCPHA               (0x1u << SPI_CSR_NCPHA_Pos)
#define   SPI_CSR_NCPHA_0           (0x0u <<  1) /**< \brief (SPI_CSR) Data is changed on the leading edge of SPCK and captured on the following edge of SPCK. */
#define   SPI_CSR_NCPHA_1           (0x1u <<  1) /**< \brief (SPI_CSR) Data is captured on the leading edge of SPCK and changed on the following edge of SPCK.NCPHA determines which edge of SPCK causes data to change and which edge causes data to be captured. NCPHA isused with CPOL to produce the required clock/data relationship between master and slave devices. */
#define SPI_CSR_CSNAAT_Pos          2            /**< \brief (SPI_CSR) Chip Select Not Active After Transfer */
#define SPI_CSR_CSNAAT              (0x1u << SPI_CSR_CSNAAT_Pos)
#define SPI_CSR_CSAAT_Pos           3            /**< \brief (SPI_CSR) Chip Select Active After Transfer */
#define SPI_CSR_CSAAT               (0x1u << SPI_CSR_CSAAT_Pos)
#define   SPI_CSR_CSAAT_0           (0x0u <<  3) /**< \brief (SPI_CSR) The Peripheral Chip Select Line rises as soon as the last transfer is achieved. */
#define   SPI_CSR_CSAAT_1           (0x1u <<  3) /**< \brief (SPI_CSR) The Peripheral Chip Select does not rise after the last transfer is achieved. It remains active until a new transfer isrequested on a different chip select. */
#define SPI_CSR_BITS_Pos            4            /**< \brief (SPI_CSR) Bits Per Transfer */
#define SPI_CSR_BITS_Msk            (0xFu << SPI_CSR_BITS_Pos)
#define SPI_CSR_BITS(value)         ((SPI_CSR_BITS_Msk & ((value) << SPI_CSR_BITS_Pos)))
#define   SPI_CSR_BITS_8_BPT        (0x0u <<  4) /**< \brief (SPI_CSR) 8 bits per transfer */
#define   SPI_CSR_BITS_9_BPT        (0x1u <<  4) /**< \brief (SPI_CSR) 9 bits per transfer */
#define   SPI_CSR_BITS_10_BPT       (0x2u <<  4) /**< \brief (SPI_CSR) 10 bits per transfer */
#define   SPI_CSR_BITS_11_BPT       (0x3u <<  4) /**< \brief (SPI_CSR) 11 bits per transfer */
#define   SPI_CSR_BITS_12_BPT       (0x4u <<  4) /**< \brief (SPI_CSR) 12 bits per transfer */
#define   SPI_CSR_BITS_13_BPT       (0x5u <<  4) /**< \brief (SPI_CSR) 13 bits per transfer */
#define   SPI_CSR_BITS_14_BPT       (0x6u <<  4) /**< \brief (SPI_CSR) 14 bits per transfer */
#define   SPI_CSR_BITS_15_BPT       (0x7u <<  4) /**< \brief (SPI_CSR) 15 bits per transfer */
#define   SPI_CSR_BITS_16_BPT       (0x8u <<  4) /**< \brief (SPI_CSR) 16 bits per transfer */
#define SPI_CSR_SCBR_Pos            8            /**< \brief (SPI_CSR) Serial Clock Baud Rate */
#define SPI_CSR_SCBR_Msk            (0xFFu << SPI_CSR_SCBR_Pos)
#define SPI_CSR_SCBR(value)         ((SPI_CSR_SCBR_Msk & ((value) << SPI_CSR_SCBR_Pos)))
#define SPI_CSR_DLYBS_Pos           16           /**< \brief (SPI_CSR) Delay Before SPCK */
#define SPI_CSR_DLYBS_Msk           (0xFFu << SPI_CSR_DLYBS_Pos)
#define SPI_CSR_DLYBS(value)        ((SPI_CSR_DLYBS_Msk & ((value) << SPI_CSR_DLYBS_Pos)))
#define SPI_CSR_DLYBCT_Pos          24           /**< \brief (SPI_CSR) Delay Between Consecutive Transfers */
#define SPI_CSR_DLYBCT_Msk          (0xFFu << SPI_CSR_DLYBCT_Pos)
#define SPI_CSR_DLYBCT(value)       ((SPI_CSR_DLYBCT_Msk & ((value) << SPI_CSR_DLYBCT_Pos)))
#define SPI_CSR_MASK                0xFFFFFFFFu  /**< \brief (SPI_CSR) MASK Register */

/* -------- SPI_WPCR : (SPI Offset: 0xE4) (R/W 32) Write Protection control Register -------- */
#define SPI_WPCR_OFFSET             0xE4         /**< \brief (SPI_WPCR offset) Write Protection control Register */
#define SPI_WPCR_RESETVALUE         0x00000000   /**< \brief (SPI_WPCR reset_value) Write Protection control Register */

#define SPI_WPCR_WPEN_Pos           0            /**< \brief (SPI_WPCR) Write Protection Enable */
#define SPI_WPCR_WPEN               (0x1u << SPI_WPCR_WPEN_Pos)
#define SPI_WPCR_WPKEY_Pos          8            /**< \brief (SPI_WPCR) Write Protection Key Password */
#define SPI_WPCR_WPKEY_Msk          (0xFFFFFFu << SPI_WPCR_WPKEY_Pos)
#define SPI_WPCR_WPKEY(value)       ((SPI_WPCR_WPKEY_Msk & ((value) << SPI_WPCR_WPKEY_Pos)))
#define   SPI_WPCR_WPKEY_VALUE      (0x535049u <<  8) /**< \brief (SPI_WPCR) SPI Write Protection Key Password */
#define SPI_WPCR_MASK               0xFFFFFF01u  /**< \brief (SPI_WPCR) MASK Register */

/* -------- SPI_WPSR : (SPI Offset: 0xE8) (R/  32) Write Protection status Register -------- */
#define SPI_WPSR_OFFSET             0xE8         /**< \brief (SPI_WPSR offset) Write Protection status Register */
#define SPI_WPSR_RESETVALUE         0x00000000   /**< \brief (SPI_WPSR reset_value) Write Protection status Register */

#define SPI_WPSR_WPVS_Pos           0            /**< \brief (SPI_WPSR) Write Protection Violation Status */
#define SPI_WPSR_WPVS_Msk           (0x7u << SPI_WPSR_WPVS_Pos)
#define SPI_WPSR_WPVS(value)        ((SPI_WPSR_WPVS_Msk & ((value) << SPI_WPSR_WPVS_Pos)))
#define   SPI_WPSR_WPVS_WRITE_WITH_WP (0x1u <<  0) /**< \brief (SPI_WPSR) The Write Protection has blocked a Write access to a protected register (since the last read). */
#define   SPI_WPSR_WPVS_SWRST_WITH_WP (0x2u <<  0) /**< \brief (SPI_WPSR) Software Reset has been performed while Write Protection was enabled (since the last read or since the last write access on MR, IER, IDR or CSRx). */
#define   SPI_WPSR_WPVS_UNEXPECTED_WRITE (0x4u <<  0) /**< \brief (SPI_WPSR) Write accesses have been detected on MR (while a chip select was active) or on CSRi (while the Chip Select "i" was active) since the last read. */
#define SPI_WPSR_WPVSRC_Pos         8            /**< \brief (SPI_WPSR) Write Protection Violation Source */
#define SPI_WPSR_WPVSRC_Msk         (0xFFu << SPI_WPSR_WPVSRC_Pos)
#define SPI_WPSR_WPVSRC(value)      ((SPI_WPSR_WPVSRC_Msk & ((value) << SPI_WPSR_WPVSRC_Pos)))
#define SPI_WPSR_MASK               0x0000FF07u  /**< \brief (SPI_WPSR) MASK Register */

/* -------- SPI_FEATURES : (SPI Offset: 0xF8) (R/  32) Features Register -------- */
#define SPI_FEATURES_OFFSET         0xF8         /**< \brief (SPI_FEATURES offset) Features Register */

#define SPI_FEATURES_NCS_Pos        0            /**< \brief (SPI_FEATURES) Number of Chip Selects */
#define SPI_FEATURES_NCS_Msk        (0xFu << SPI_FEATURES_NCS_Pos)
#define SPI_FEATURES_NCS(value)     ((SPI_FEATURES_NCS_Msk & ((value) << SPI_FEATURES_NCS_Pos)))
#define SPI_FEATURES_PCONF_Pos      4            /**< \brief (SPI_FEATURES) Polarity is Configurable */
#define SPI_FEATURES_PCONF          (0x1u << SPI_FEATURES_PCONF_Pos)
#define SPI_FEATURES_PPNCONF_Pos    5            /**< \brief (SPI_FEATURES) Polarity is Positive if Polarity is not Configurable */
#define SPI_FEATURES_PPNCONF        (0x1u << SPI_FEATURES_PPNCONF_Pos)
#define SPI_FEATURES_PHCONF_Pos     6            /**< \brief (SPI_FEATURES) Phase is Configurable */
#define SPI_FEATURES_PHCONF         (0x1u << SPI_FEATURES_PHCONF_Pos)
#define SPI_FEATURES_PHZNCONF_Pos   7            /**< \brief (SPI_FEATURES) Phase is Zero if Phase is not Configurable */
#define SPI_FEATURES_PHZNCONF       (0x1u << SPI_FEATURES_PHZNCONF_Pos)
#define SPI_FEATURES_LENCONF_Pos    8            /**< \brief (SPI_FEATURES) Character Length is Configurable */
#define SPI_FEATURES_LENCONF        (0x1u << SPI_FEATURES_LENCONF_Pos)
#define SPI_FEATURES_LENNCONF_Pos   9            /**< \brief (SPI_FEATURES) Character Length if not Configurable */
#define SPI_FEATURES_LENNCONF_Msk   (0x7Fu << SPI_FEATURES_LENNCONF_Pos)
#define SPI_FEATURES_LENNCONF(value) ((SPI_FEATURES_LENNCONF_Msk & ((value) << SPI_FEATURES_LENNCONF_Pos)))
#define SPI_FEATURES_EXTDEC_Pos     16           /**< \brief (SPI_FEATURES) External Decoder is True */
#define SPI_FEATURES_EXTDEC         (0x1u << SPI_FEATURES_EXTDEC_Pos)
#define SPI_FEATURES_CSNAATIMPL_Pos 17           /**< \brief (SPI_FEATURES) CSNAAT Features are Implemented */
#define SPI_FEATURES_CSNAATIMPL     (0x1u << SPI_FEATURES_CSNAATIMPL_Pos)
#define SPI_FEATURES_BRPBHSB_Pos    18           /**< \brief (SPI_FEATURES) Bridge Type is PB to HSB */
#define SPI_FEATURES_BRPBHSB        (0x1u << SPI_FEATURES_BRPBHSB_Pos)
#define SPI_FEATURES_FIFORIMPL_Pos  19           /**< \brief (SPI_FEATURES) FIFO in Reception is Implemented */
#define SPI_FEATURES_FIFORIMPL      (0x1u << SPI_FEATURES_FIFORIMPL_Pos)
#define SPI_FEATURES_SWPIMPL_Pos    20           /**< \brief (SPI_FEATURES) Spurious Write Protection is Implemented */
#define SPI_FEATURES_SWPIMPL        (0x1u << SPI_FEATURES_SWPIMPL_Pos)
#define SPI_FEATURES_MASK           0x001FFFFFu  /**< \brief (SPI_FEATURES) MASK Register */

/* -------- SPI_VERSION : (SPI Offset: 0xFC) (R/  32) Version Register -------- */
#define SPI_VERSION_OFFSET          0xFC         /**< \brief (SPI_VERSION offset) Version Register */
#define SPI_VERSION_RESETVALUE      0x00000211   /**< \brief (SPI_VERSION reset_value) Version Register */

#define SPI_VERSION_VERSION_Pos     0            /**< \brief (SPI_VERSION) Version */
#define SPI_VERSION_VERSION_Msk     (0xFFFu << SPI_VERSION_VERSION_Pos)
#define SPI_VERSION_VERSION(value)  ((SPI_VERSION_VERSION_Msk & ((value) << SPI_VERSION_VERSION_Pos)))
#define SPI_VERSION_MFN_Pos         16           /**< \brief (SPI_VERSION) mfn */
#define SPI_VERSION_MFN_Msk         (0x7u << SPI_VERSION_MFN_Pos)
#define SPI_VERSION_MFN(value)      ((SPI_VERSION_MFN_Msk & ((value) << SPI_VERSION_MFN_Pos)))
#define SPI_VERSION_MASK            0x00070FFFu  /**< \brief (SPI_VERSION) MASK Register */

/** \brief SPI hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
  WoReg   SPI_CR;             /**< \brief (SPI Offset: 0x00) Control Register */
  RwReg   SPI_MR;             /**< \brief (SPI Offset: 0x04) Mode Register */
  RoReg   SPI_RDR;            /**< \brief (SPI Offset: 0x08) Receive Data Register */
  WoReg   SPI_TDR;            /**< \brief (SPI Offset: 0x0C) Transmit Data Register */
  RoReg   SPI_SR;             /**< \brief (SPI Offset: 0x10) Status Register */
  WoReg   SPI_IER;            /**< \brief (SPI Offset: 0x14) Interrupt Enable Register */
  WoReg   SPI_IDR;            /**< \brief (SPI Offset: 0x18) Interrupt Disable Register */
  RoReg   SPI_IMR;            /**< \brief (SPI Offset: 0x1C) Interrupt Mask Register */
  RoReg8  Reserved1[0x10];
  RwReg   SPI_CSR[4];         /**< \brief (SPI Offset: 0x30) Chip Select Register */
  RoReg8  Reserved2[0xA4];
  RwReg   SPI_WPCR;           /**< \brief (SPI Offset: 0xE4) Write Protection control Register */
  RoReg   SPI_WPSR;           /**< \brief (SPI Offset: 0xE8) Write Protection status Register */
  RoReg8  Reserved3[0xC];
  RoReg   SPI_FEATURES;       /**< \brief (SPI Offset: 0xF8) Features Register */
  RoReg   SPI_VERSION;        /**< \brief (SPI Offset: 0xFC) Version Register */
} Spi;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/*@}*/

#endif /* _SAM4L_SPI_COMPONENT_ */
