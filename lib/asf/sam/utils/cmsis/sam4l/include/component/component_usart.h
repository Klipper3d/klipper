/**
 * \file
 *
 * \brief Component description for USART
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

#ifndef _SAM4L_USART_COMPONENT_
#define _SAM4L_USART_COMPONENT_

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR USART */
/* ========================================================================== */
/** \addtogroup SAM4L_USART Universal Synchronous Asynchronous Receiver Transmitter */
/*@{*/

#define REV_USART                   0x6021

/* -------- US_CR : (USART Offset: 0x00) ( /W 32) Control Register -------- */
#define US_CR_OFFSET                0x00         /**< \brief (US_CR offset) Control Register */

// LIN mode
#define US_CR_LIN_RSTRX_Pos         2            /**< \brief (US_CR_LIN) Reset Receiver */
#define US_CR_LIN_RSTRX             (0x1u << US_CR_LIN_RSTRX_Pos)
#define   US_CR_LIN_RSTRX_0         (0x0u <<  2) /**< \brief (US_CR_LIN) No effect */
#define   US_CR_LIN_RSTRX_1         (0x1u <<  2) /**< \brief (US_CR_LIN) Resets the receiver */
#define US_CR_LIN_RSTTX_Pos         3            /**< \brief (US_CR_LIN) Reset Transmitter */
#define US_CR_LIN_RSTTX             (0x1u << US_CR_LIN_RSTTX_Pos)
#define   US_CR_LIN_RSTTX_0         (0x0u <<  3) /**< \brief (US_CR_LIN) No effect */
#define   US_CR_LIN_RSTTX_1         (0x1u <<  3) /**< \brief (US_CR_LIN) Resets the transmitter */
#define US_CR_LIN_RXEN_Pos          4            /**< \brief (US_CR_LIN) Receiver Enable */
#define US_CR_LIN_RXEN              (0x1u << US_CR_LIN_RXEN_Pos)
#define   US_CR_LIN_RXEN_0          (0x0u <<  4) /**< \brief (US_CR_LIN) No effect */
#define   US_CR_LIN_RXEN_1          (0x1u <<  4) /**< \brief (US_CR_LIN) Enables the receiver, if RXDIS is 0 */
#define US_CR_LIN_RXDIS_Pos         5            /**< \brief (US_CR_LIN) Receiver Disable */
#define US_CR_LIN_RXDIS             (0x1u << US_CR_LIN_RXDIS_Pos)
#define   US_CR_LIN_RXDIS_0         (0x0u <<  5) /**< \brief (US_CR_LIN) No effect */
#define   US_CR_LIN_RXDIS_1         (0x1u <<  5) /**< \brief (US_CR_LIN) Disables the receiver */
#define US_CR_LIN_TXEN_Pos          6            /**< \brief (US_CR_LIN) Transmitter Enable */
#define US_CR_LIN_TXEN              (0x1u << US_CR_LIN_TXEN_Pos)
#define   US_CR_LIN_TXEN_0          (0x0u <<  6) /**< \brief (US_CR_LIN) No effect */
#define   US_CR_LIN_TXEN_1          (0x1u <<  6) /**< \brief (US_CR_LIN) Enables the transmitter if TXDIS is 0 */
#define US_CR_LIN_TXDIS_Pos         7            /**< \brief (US_CR_LIN) Transmitter Disable */
#define US_CR_LIN_TXDIS             (0x1u << US_CR_LIN_TXDIS_Pos)
#define   US_CR_LIN_TXDIS_0         (0x0u <<  7) /**< \brief (US_CR_LIN) No effect */
#define   US_CR_LIN_TXDIS_1         (0x1u <<  7) /**< \brief (US_CR_LIN) Disables the transmitter */
#define US_CR_LIN_RSTSTA_Pos        8            /**< \brief (US_CR_LIN) Reset Status Bits */
#define US_CR_LIN_RSTSTA            (0x1u << US_CR_LIN_RSTSTA_Pos)
#define   US_CR_LIN_RSTSTA_0        (0x0u <<  8) /**< \brief (US_CR_LIN) No effect */
#define   US_CR_LIN_RSTSTA_1        (0x1u <<  8) /**< \brief (US_CR_LIN) Resets the status bits PARE, FRAME, OVRE and RXBRK in the CSR */
#define US_CR_LIN_STTBRK_Pos        9            /**< \brief (US_CR_LIN) Start Break */
#define US_CR_LIN_STTBRK            (0x1u << US_CR_LIN_STTBRK_Pos)
#define   US_CR_LIN_STTBRK_0        (0x0u <<  9) /**< \brief (US_CR_LIN) No effect */
#define   US_CR_LIN_STTBRK_1        (0x1u <<  9) /**< \brief (US_CR_LIN) Starts transmission of a break after the characters present in THR and the Transmit Shift Register have been transmitted. No effect if a break is already being transmitted */
#define US_CR_LIN_STPBRK_Pos        10           /**< \brief (US_CR_LIN) Stop Break */
#define US_CR_LIN_STPBRK            (0x1u << US_CR_LIN_STPBRK_Pos)
#define   US_CR_LIN_STPBRK_0        (0x0u << 10) /**< \brief (US_CR_LIN) No effect */
#define   US_CR_LIN_STPBRK_1        (0x1u << 10) /**< \brief (US_CR_LIN) Stops transmission of the break after a minimum of one character length and transmits a high level during 12-bit periods.No effect if no break is being transmitted */
#define US_CR_LIN_STTTO_Pos         11           /**< \brief (US_CR_LIN) Start Time-out */
#define US_CR_LIN_STTTO             (0x1u << US_CR_LIN_STTTO_Pos)
#define   US_CR_LIN_STTTO_0         (0x0u << 11) /**< \brief (US_CR_LIN) No effect */
#define   US_CR_LIN_STTTO_1         (0x1u << 11) /**< \brief (US_CR_LIN) Starts waiting for a character before clocking the time-out counter */
#define US_CR_LIN_SENDA_Pos         12           /**< \brief (US_CR_LIN) Send Address */
#define US_CR_LIN_SENDA             (0x1u << US_CR_LIN_SENDA_Pos)
#define   US_CR_LIN_SENDA_0         (0x0u << 12) /**< \brief (US_CR_LIN) No effect */
#define   US_CR_LIN_SENDA_1         (0x1u << 12) /**< \brief (US_CR_LIN) In Multi-drop Mode only, the next character written to the THR is sent with the address bit set */
#define US_CR_LIN_RSTIT_Pos         13           /**< \brief (US_CR_LIN) Reset Iterations */
#define US_CR_LIN_RSTIT             (0x1u << US_CR_LIN_RSTIT_Pos)
#define   US_CR_LIN_RSTIT_0         (0x0u << 13) /**< \brief (US_CR_LIN) No effect */
#define   US_CR_LIN_RSTIT_1         (0x1u << 13) /**< \brief (US_CR_LIN) Resets ITERATION in CSR. No effect if the ISO7816 is not enabled */
#define US_CR_LIN_RSTNACK_Pos       14           /**< \brief (US_CR_LIN) Reset Non Acknowledge */
#define US_CR_LIN_RSTNACK           (0x1u << US_CR_LIN_RSTNACK_Pos)
#define   US_CR_LIN_RSTNACK_0       (0x0u << 14) /**< \brief (US_CR_LIN) No effect */
#define   US_CR_LIN_RSTNACK_1       (0x1u << 14) /**< \brief (US_CR_LIN) Resets NACK in CSR */
#define US_CR_LIN_RETTO_Pos         15           /**< \brief (US_CR_LIN) Rearm Time-out */
#define US_CR_LIN_RETTO             (0x1u << US_CR_LIN_RETTO_Pos)
#define   US_CR_LIN_RETTO_0         (0x0u << 15) /**< \brief (US_CR_LIN) No effect */
#define   US_CR_LIN_RETTO_1         (0x1u << 15) /**< \brief (US_CR_LIN) Restart Time-out */
#define US_CR_LIN_DTREN_Pos         16           /**< \brief (US_CR_LIN) Data Terminal Ready Enable */
#define US_CR_LIN_DTREN             (0x1u << US_CR_LIN_DTREN_Pos)
#define   US_CR_LIN_DTREN_0         (0x0u << 16) /**< \brief (US_CR_LIN) No effect */
#define   US_CR_LIN_DTREN_1         (0x1u << 16) /**< \brief (US_CR_LIN) Drives the pin DTR at 0 */
#define US_CR_LIN_DTRDIS_Pos        17           /**< \brief (US_CR_LIN) Data Terminal Ready Disable */
#define US_CR_LIN_DTRDIS            (0x1u << US_CR_LIN_DTRDIS_Pos)
#define   US_CR_LIN_DTRDIS_0        (0x0u << 17) /**< \brief (US_CR_LIN) No effect */
#define   US_CR_LIN_DTRDIS_1        (0x1u << 17) /**< \brief (US_CR_LIN) Drives the pin DTR to 1 */
#define US_CR_LIN_RTSEN_Pos         18           /**< \brief (US_CR_LIN) Request to Send Enable */
#define US_CR_LIN_RTSEN             (0x1u << US_CR_LIN_RTSEN_Pos)
#define   US_CR_LIN_RTSEN_0         (0x0u << 18) /**< \brief (US_CR_LIN) No effect */
#define   US_CR_LIN_RTSEN_1         (0x1u << 18) /**< \brief (US_CR_LIN) Drives the pin RTS to 0 */
#define US_CR_LIN_RTSDIS_Pos        19           /**< \brief (US_CR_LIN) Request to Send Disable */
#define US_CR_LIN_RTSDIS            (0x1u << US_CR_LIN_RTSDIS_Pos)
#define   US_CR_LIN_RTSDIS_0        (0x0u << 19) /**< \brief (US_CR_LIN) No effect */
#define   US_CR_LIN_RTSDIS_1        (0x1u << 19) /**< \brief (US_CR_LIN) Drives the pin RTS to 1 */
#define US_CR_LIN_LINABT_Pos        20           /**< \brief (US_CR_LIN) Abort the current LIN transmission */
#define US_CR_LIN_LINABT            (0x1u << US_CR_LIN_LINABT_Pos)
#define US_CR_LIN_LINWKUP_Pos       21           /**< \brief (US_CR_LIN) Sends a wakeup signal on the LIN bus */
#define US_CR_LIN_LINWKUP           (0x1u << US_CR_LIN_LINWKUP_Pos)
#define US_CR_LIN_MASK              0x003FFFFCu  /**< \brief (US_CR_LIN) MASK Register */

// SPI_MASTER mode
#define US_CR_SPI_MASTER_RSTRX_Pos  2            /**< \brief (US_CR_SPI_MASTER) Reset Receiver */
#define US_CR_SPI_MASTER_RSTRX      (0x1u << US_CR_SPI_MASTER_RSTRX_Pos)
#define   US_CR_SPI_MASTER_RSTRX_0  (0x0u <<  2) /**< \brief (US_CR_SPI_MASTER) No effect */
#define   US_CR_SPI_MASTER_RSTRX_1  (0x1u <<  2) /**< \brief (US_CR_SPI_MASTER) Resets the receiver */
#define US_CR_SPI_MASTER_RSTTX_Pos  3            /**< \brief (US_CR_SPI_MASTER) Reset Transmitter */
#define US_CR_SPI_MASTER_RSTTX      (0x1u << US_CR_SPI_MASTER_RSTTX_Pos)
#define   US_CR_SPI_MASTER_RSTTX_0  (0x0u <<  3) /**< \brief (US_CR_SPI_MASTER) No effect */
#define   US_CR_SPI_MASTER_RSTTX_1  (0x1u <<  3) /**< \brief (US_CR_SPI_MASTER) Resets the transmitter */
#define US_CR_SPI_MASTER_RXEN_Pos   4            /**< \brief (US_CR_SPI_MASTER) Receiver Enable */
#define US_CR_SPI_MASTER_RXEN       (0x1u << US_CR_SPI_MASTER_RXEN_Pos)
#define   US_CR_SPI_MASTER_RXEN_0   (0x0u <<  4) /**< \brief (US_CR_SPI_MASTER) No effect */
#define   US_CR_SPI_MASTER_RXEN_1   (0x1u <<  4) /**< \brief (US_CR_SPI_MASTER) Enables the receiver, if RXDIS is 0 */
#define US_CR_SPI_MASTER_RXDIS_Pos  5            /**< \brief (US_CR_SPI_MASTER) Receiver Disable */
#define US_CR_SPI_MASTER_RXDIS      (0x1u << US_CR_SPI_MASTER_RXDIS_Pos)
#define   US_CR_SPI_MASTER_RXDIS_0  (0x0u <<  5) /**< \brief (US_CR_SPI_MASTER) No effect */
#define   US_CR_SPI_MASTER_RXDIS_1  (0x1u <<  5) /**< \brief (US_CR_SPI_MASTER) Disables the receiver */
#define US_CR_SPI_MASTER_TXEN_Pos   6            /**< \brief (US_CR_SPI_MASTER) Transmitter Enable */
#define US_CR_SPI_MASTER_TXEN       (0x1u << US_CR_SPI_MASTER_TXEN_Pos)
#define   US_CR_SPI_MASTER_TXEN_0   (0x0u <<  6) /**< \brief (US_CR_SPI_MASTER) No effect */
#define   US_CR_SPI_MASTER_TXEN_1   (0x1u <<  6) /**< \brief (US_CR_SPI_MASTER) Enables the transmitter if TXDIS is 0 */
#define US_CR_SPI_MASTER_TXDIS_Pos  7            /**< \brief (US_CR_SPI_MASTER) Transmitter Disable */
#define US_CR_SPI_MASTER_TXDIS      (0x1u << US_CR_SPI_MASTER_TXDIS_Pos)
#define   US_CR_SPI_MASTER_TXDIS_0  (0x0u <<  7) /**< \brief (US_CR_SPI_MASTER) No effect */
#define   US_CR_SPI_MASTER_TXDIS_1  (0x1u <<  7) /**< \brief (US_CR_SPI_MASTER) Disables the transmitter */
#define US_CR_SPI_MASTER_RSTSTA_Pos 8            /**< \brief (US_CR_SPI_MASTER) Reset Status Bits */
#define US_CR_SPI_MASTER_RSTSTA     (0x1u << US_CR_SPI_MASTER_RSTSTA_Pos)
#define   US_CR_SPI_MASTER_RSTSTA_0 (0x0u <<  8) /**< \brief (US_CR_SPI_MASTER) No effect */
#define   US_CR_SPI_MASTER_RSTSTA_1 (0x1u <<  8) /**< \brief (US_CR_SPI_MASTER) Resets the status bits PARE, FRAME, OVRE and RXBRK in the CSR */
#define US_CR_SPI_MASTER_STTBRK_Pos 9            /**< \brief (US_CR_SPI_MASTER) Start Break */
#define US_CR_SPI_MASTER_STTBRK     (0x1u << US_CR_SPI_MASTER_STTBRK_Pos)
#define   US_CR_SPI_MASTER_STTBRK_0 (0x0u <<  9) /**< \brief (US_CR_SPI_MASTER) No effect */
#define   US_CR_SPI_MASTER_STTBRK_1 (0x1u <<  9) /**< \brief (US_CR_SPI_MASTER) Starts transmission of a break after the characters present in THR and the Transmit Shift Register have been transmitted. No effect if a break is already being transmitted */
#define US_CR_SPI_MASTER_STPBRK_Pos 10           /**< \brief (US_CR_SPI_MASTER) Stop Break */
#define US_CR_SPI_MASTER_STPBRK     (0x1u << US_CR_SPI_MASTER_STPBRK_Pos)
#define   US_CR_SPI_MASTER_STPBRK_0 (0x0u << 10) /**< \brief (US_CR_SPI_MASTER) No effect */
#define   US_CR_SPI_MASTER_STPBRK_1 (0x1u << 10) /**< \brief (US_CR_SPI_MASTER) Stops transmission of the break after a minimum of one character length and transmits a high level during 12-bit periods.No effect if no break is being transmitted */
#define US_CR_SPI_MASTER_STTTO_Pos  11           /**< \brief (US_CR_SPI_MASTER) Start Time-out */
#define US_CR_SPI_MASTER_STTTO      (0x1u << US_CR_SPI_MASTER_STTTO_Pos)
#define   US_CR_SPI_MASTER_STTTO_0  (0x0u << 11) /**< \brief (US_CR_SPI_MASTER) No effect */
#define   US_CR_SPI_MASTER_STTTO_1  (0x1u << 11) /**< \brief (US_CR_SPI_MASTER) Starts waiting for a character before clocking the time-out counter */
#define US_CR_SPI_MASTER_SENDA_Pos  12           /**< \brief (US_CR_SPI_MASTER) Send Address */
#define US_CR_SPI_MASTER_SENDA      (0x1u << US_CR_SPI_MASTER_SENDA_Pos)
#define   US_CR_SPI_MASTER_SENDA_0  (0x0u << 12) /**< \brief (US_CR_SPI_MASTER) No effect */
#define   US_CR_SPI_MASTER_SENDA_1  (0x1u << 12) /**< \brief (US_CR_SPI_MASTER) In Multi-drop Mode only, the next character written to the THR is sent with the address bit set */
#define US_CR_SPI_MASTER_RSTIT_Pos  13           /**< \brief (US_CR_SPI_MASTER) Reset Iterations */
#define US_CR_SPI_MASTER_RSTIT      (0x1u << US_CR_SPI_MASTER_RSTIT_Pos)
#define   US_CR_SPI_MASTER_RSTIT_0  (0x0u << 13) /**< \brief (US_CR_SPI_MASTER) No effect */
#define   US_CR_SPI_MASTER_RSTIT_1  (0x1u << 13) /**< \brief (US_CR_SPI_MASTER) Resets ITERATION in CSR. No effect if the ISO7816 is not enabled */
#define US_CR_SPI_MASTER_RSTNACK_Pos 14           /**< \brief (US_CR_SPI_MASTER) Reset Non Acknowledge */
#define US_CR_SPI_MASTER_RSTNACK    (0x1u << US_CR_SPI_MASTER_RSTNACK_Pos)
#define   US_CR_SPI_MASTER_RSTNACK_0 (0x0u << 14) /**< \brief (US_CR_SPI_MASTER) No effect */
#define   US_CR_SPI_MASTER_RSTNACK_1 (0x1u << 14) /**< \brief (US_CR_SPI_MASTER) Resets NACK in CSR */
#define US_CR_SPI_MASTER_RETTO_Pos  15           /**< \brief (US_CR_SPI_MASTER) Rearm Time-out */
#define US_CR_SPI_MASTER_RETTO      (0x1u << US_CR_SPI_MASTER_RETTO_Pos)
#define   US_CR_SPI_MASTER_RETTO_0  (0x0u << 15) /**< \brief (US_CR_SPI_MASTER) No effect */
#define   US_CR_SPI_MASTER_RETTO_1  (0x1u << 15) /**< \brief (US_CR_SPI_MASTER) Restart Time-out */
#define US_CR_SPI_MASTER_DTREN_Pos  16           /**< \brief (US_CR_SPI_MASTER) Data Terminal Ready Enable */
#define US_CR_SPI_MASTER_DTREN      (0x1u << US_CR_SPI_MASTER_DTREN_Pos)
#define   US_CR_SPI_MASTER_DTREN_0  (0x0u << 16) /**< \brief (US_CR_SPI_MASTER) No effect */
#define   US_CR_SPI_MASTER_DTREN_1  (0x1u << 16) /**< \brief (US_CR_SPI_MASTER) Drives the pin DTR at 0 */
#define US_CR_SPI_MASTER_DTRDIS_Pos 17           /**< \brief (US_CR_SPI_MASTER) Data Terminal Ready Disable */
#define US_CR_SPI_MASTER_DTRDIS     (0x1u << US_CR_SPI_MASTER_DTRDIS_Pos)
#define   US_CR_SPI_MASTER_DTRDIS_0 (0x0u << 17) /**< \brief (US_CR_SPI_MASTER) No effect */
#define   US_CR_SPI_MASTER_DTRDIS_1 (0x1u << 17) /**< \brief (US_CR_SPI_MASTER) Drives the pin DTR to 1 */
#define US_CR_SPI_MASTER_FCS_Pos    18           /**< \brief (US_CR_SPI_MASTER) Force SPI Chip Select */
#define US_CR_SPI_MASTER_FCS        (0x1u << US_CR_SPI_MASTER_FCS_Pos)
#define   US_CR_SPI_MASTER_FCS_0    (0x0u << 18) /**< \brief (US_CR_SPI_MASTER) No effect */
#define   US_CR_SPI_MASTER_FCS_1    (0x1u << 18) /**< \brief (US_CR_SPI_MASTER) Forces the Slave Select Line NSS (RTS pin) to 0, even if USART is no transmitting, in order to address SPI slave devices supporting the CSAAT Mode (Chip Select Active After Transfer) */
#define US_CR_SPI_MASTER_RCS_Pos    19           /**< \brief (US_CR_SPI_MASTER) Release SPI Chip Select */
#define US_CR_SPI_MASTER_RCS        (0x1u << US_CR_SPI_MASTER_RCS_Pos)
#define   US_CR_SPI_MASTER_RCS_0    (0x0u << 19) /**< \brief (US_CR_SPI_MASTER) No effect */
#define   US_CR_SPI_MASTER_RCS_1    (0x1u << 19) /**< \brief (US_CR_SPI_MASTER) Releases the Slave Select Line NSS (RTS pin) */
#define US_CR_SPI_MASTER_MASK       0x000FFFFCu  /**< \brief (US_CR_SPI_MASTER) MASK Register */

// USART mode
#define US_CR_USART_RSTRX_Pos       2            /**< \brief (US_CR_USART) Reset Receiver */
#define US_CR_USART_RSTRX           (0x1u << US_CR_USART_RSTRX_Pos)
#define   US_CR_USART_RSTRX_0       (0x0u <<  2) /**< \brief (US_CR_USART) No effect */
#define   US_CR_USART_RSTRX_1       (0x1u <<  2) /**< \brief (US_CR_USART) Resets the receiver */
#define US_CR_USART_RSTTX_Pos       3            /**< \brief (US_CR_USART) Reset Transmitter */
#define US_CR_USART_RSTTX           (0x1u << US_CR_USART_RSTTX_Pos)
#define   US_CR_USART_RSTTX_0       (0x0u <<  3) /**< \brief (US_CR_USART) No effect */
#define   US_CR_USART_RSTTX_1       (0x1u <<  3) /**< \brief (US_CR_USART) Resets the transmitter */
#define US_CR_USART_RXEN_Pos        4            /**< \brief (US_CR_USART) Receiver Enable */
#define US_CR_USART_RXEN            (0x1u << US_CR_USART_RXEN_Pos)
#define   US_CR_USART_RXEN_0        (0x0u <<  4) /**< \brief (US_CR_USART) No effect */
#define   US_CR_USART_RXEN_1        (0x1u <<  4) /**< \brief (US_CR_USART) Enables the receiver, if RXDIS is 0 */
#define US_CR_USART_RXDIS_Pos       5            /**< \brief (US_CR_USART) Receiver Disable */
#define US_CR_USART_RXDIS           (0x1u << US_CR_USART_RXDIS_Pos)
#define   US_CR_USART_RXDIS_0       (0x0u <<  5) /**< \brief (US_CR_USART) No effect */
#define   US_CR_USART_RXDIS_1       (0x1u <<  5) /**< \brief (US_CR_USART) Disables the receiver */
#define US_CR_USART_TXEN_Pos        6            /**< \brief (US_CR_USART) Transmitter Enable */
#define US_CR_USART_TXEN            (0x1u << US_CR_USART_TXEN_Pos)
#define   US_CR_USART_TXEN_0        (0x0u <<  6) /**< \brief (US_CR_USART) No effect */
#define   US_CR_USART_TXEN_1        (0x1u <<  6) /**< \brief (US_CR_USART) Enables the transmitter if TXDIS is 0 */
#define US_CR_USART_TXDIS_Pos       7            /**< \brief (US_CR_USART) Transmitter Disable */
#define US_CR_USART_TXDIS           (0x1u << US_CR_USART_TXDIS_Pos)
#define   US_CR_USART_TXDIS_0       (0x0u <<  7) /**< \brief (US_CR_USART) No effect */
#define   US_CR_USART_TXDIS_1       (0x1u <<  7) /**< \brief (US_CR_USART) Disables the transmitter */
#define US_CR_USART_RSTSTA_Pos      8            /**< \brief (US_CR_USART) Reset Status Bits */
#define US_CR_USART_RSTSTA          (0x1u << US_CR_USART_RSTSTA_Pos)
#define   US_CR_USART_RSTSTA_0      (0x0u <<  8) /**< \brief (US_CR_USART) No effect */
#define   US_CR_USART_RSTSTA_1      (0x1u <<  8) /**< \brief (US_CR_USART) Resets the status bits PARE, FRAME, OVRE and RXBRK in the CSR */
#define US_CR_USART_STTBRK_Pos      9            /**< \brief (US_CR_USART) Start Break */
#define US_CR_USART_STTBRK          (0x1u << US_CR_USART_STTBRK_Pos)
#define   US_CR_USART_STTBRK_0      (0x0u <<  9) /**< \brief (US_CR_USART) No effect */
#define   US_CR_USART_STTBRK_1      (0x1u <<  9) /**< \brief (US_CR_USART) Starts transmission of a break after the characters present in THR and the Transmit Shift Register have been transmitted. No effect if a break is already being transmitted */
#define US_CR_USART_STPBRK_Pos      10           /**< \brief (US_CR_USART) Stop Break */
#define US_CR_USART_STPBRK          (0x1u << US_CR_USART_STPBRK_Pos)
#define   US_CR_USART_STPBRK_0      (0x0u << 10) /**< \brief (US_CR_USART) No effect */
#define   US_CR_USART_STPBRK_1      (0x1u << 10) /**< \brief (US_CR_USART) Stops transmission of the break after a minimum of one character length and transmits a high level during 12-bit periods.No effect if no break is being transmitted */
#define US_CR_USART_STTTO_Pos       11           /**< \brief (US_CR_USART) Start Time-out */
#define US_CR_USART_STTTO           (0x1u << US_CR_USART_STTTO_Pos)
#define   US_CR_USART_STTTO_0       (0x0u << 11) /**< \brief (US_CR_USART) No effect */
#define   US_CR_USART_STTTO_1       (0x1u << 11) /**< \brief (US_CR_USART) Starts waiting for a character before clocking the time-out counter */
#define US_CR_USART_SENDA_Pos       12           /**< \brief (US_CR_USART) Send Address */
#define US_CR_USART_SENDA           (0x1u << US_CR_USART_SENDA_Pos)
#define   US_CR_USART_SENDA_0       (0x0u << 12) /**< \brief (US_CR_USART) No effect */
#define   US_CR_USART_SENDA_1       (0x1u << 12) /**< \brief (US_CR_USART) In Multi-drop Mode only, the next character written to the THR is sent with the address bit set */
#define US_CR_USART_RSTIT_Pos       13           /**< \brief (US_CR_USART) Reset Iterations */
#define US_CR_USART_RSTIT           (0x1u << US_CR_USART_RSTIT_Pos)
#define   US_CR_USART_RSTIT_0       (0x0u << 13) /**< \brief (US_CR_USART) No effect */
#define   US_CR_USART_RSTIT_1       (0x1u << 13) /**< \brief (US_CR_USART) Resets ITERATION in CSR. No effect if the ISO7816 is not enabled */
#define US_CR_USART_RSTNACK_Pos     14           /**< \brief (US_CR_USART) Reset Non Acknowledge */
#define US_CR_USART_RSTNACK         (0x1u << US_CR_USART_RSTNACK_Pos)
#define   US_CR_USART_RSTNACK_0     (0x0u << 14) /**< \brief (US_CR_USART) No effect */
#define   US_CR_USART_RSTNACK_1     (0x1u << 14) /**< \brief (US_CR_USART) Resets NACK in CSR */
#define US_CR_USART_RETTO_Pos       15           /**< \brief (US_CR_USART) Rearm Time-out */
#define US_CR_USART_RETTO           (0x1u << US_CR_USART_RETTO_Pos)
#define   US_CR_USART_RETTO_0       (0x0u << 15) /**< \brief (US_CR_USART) No effect */
#define   US_CR_USART_RETTO_1       (0x1u << 15) /**< \brief (US_CR_USART) Restart Time-out */
#define US_CR_USART_DTREN_Pos       16           /**< \brief (US_CR_USART) Data Terminal Ready Enable */
#define US_CR_USART_DTREN           (0x1u << US_CR_USART_DTREN_Pos)
#define   US_CR_USART_DTREN_0       (0x0u << 16) /**< \brief (US_CR_USART) No effect */
#define   US_CR_USART_DTREN_1       (0x1u << 16) /**< \brief (US_CR_USART) Drives the pin DTR at 0 */
#define US_CR_USART_DTRDIS_Pos      17           /**< \brief (US_CR_USART) Data Terminal Ready Disable */
#define US_CR_USART_DTRDIS          (0x1u << US_CR_USART_DTRDIS_Pos)
#define   US_CR_USART_DTRDIS_0      (0x0u << 17) /**< \brief (US_CR_USART) No effect */
#define   US_CR_USART_DTRDIS_1      (0x1u << 17) /**< \brief (US_CR_USART) Drives the pin DTR to 1 */
#define US_CR_USART_RTSEN_Pos       18           /**< \brief (US_CR_USART) Request to Send Enable */
#define US_CR_USART_RTSEN           (0x1u << US_CR_USART_RTSEN_Pos)
#define   US_CR_USART_RTSEN_0       (0x0u << 18) /**< \brief (US_CR_USART) No effect */
#define   US_CR_USART_RTSEN_1       (0x1u << 18) /**< \brief (US_CR_USART) Drives the pin RTS to 0 */
#define US_CR_USART_RTSDIS_Pos      19           /**< \brief (US_CR_USART) Request to Send Disable */
#define US_CR_USART_RTSDIS          (0x1u << US_CR_USART_RTSDIS_Pos)
#define   US_CR_USART_RTSDIS_0      (0x0u << 19) /**< \brief (US_CR_USART) No effect */
#define   US_CR_USART_RTSDIS_1      (0x1u << 19) /**< \brief (US_CR_USART) Drives the pin RTS to 1 */
#define US_CR_USART_MASK            0x000FFFFCu  /**< \brief (US_CR_USART) MASK Register */

// Any mode
#define US_CR_RSTRX_Pos             2            /**< \brief (US_CR) Reset Receiver */
#define US_CR_RSTRX                 (0x1u << US_CR_RSTRX_Pos)
#define   US_CR_RSTRX_0             (0x0u <<  2) /**< \brief (US_CR) No effect */
#define   US_CR_RSTRX_1             (0x1u <<  2) /**< \brief (US_CR) Resets the receiver */
#define US_CR_RSTTX_Pos             3            /**< \brief (US_CR) Reset Transmitter */
#define US_CR_RSTTX                 (0x1u << US_CR_RSTTX_Pos)
#define   US_CR_RSTTX_0             (0x0u <<  3) /**< \brief (US_CR) No effect */
#define   US_CR_RSTTX_1             (0x1u <<  3) /**< \brief (US_CR) Resets the transmitter */
#define US_CR_RXEN_Pos              4            /**< \brief (US_CR) Receiver Enable */
#define US_CR_RXEN                  (0x1u << US_CR_RXEN_Pos)
#define   US_CR_RXEN_0              (0x0u <<  4) /**< \brief (US_CR) No effect */
#define   US_CR_RXEN_1              (0x1u <<  4) /**< \brief (US_CR) Enables the receiver, if RXDIS is 0 */
#define US_CR_RXDIS_Pos             5            /**< \brief (US_CR) Receiver Disable */
#define US_CR_RXDIS                 (0x1u << US_CR_RXDIS_Pos)
#define   US_CR_RXDIS_0             (0x0u <<  5) /**< \brief (US_CR) No effect */
#define   US_CR_RXDIS_1             (0x1u <<  5) /**< \brief (US_CR) Disables the receiver */
#define US_CR_TXEN_Pos              6            /**< \brief (US_CR) Transmitter Enable */
#define US_CR_TXEN                  (0x1u << US_CR_TXEN_Pos)
#define   US_CR_TXEN_0              (0x0u <<  6) /**< \brief (US_CR) No effect */
#define   US_CR_TXEN_1              (0x1u <<  6) /**< \brief (US_CR) Enables the transmitter if TXDIS is 0 */
#define US_CR_TXDIS_Pos             7            /**< \brief (US_CR) Transmitter Disable */
#define US_CR_TXDIS                 (0x1u << US_CR_TXDIS_Pos)
#define   US_CR_TXDIS_0             (0x0u <<  7) /**< \brief (US_CR) No effect */
#define   US_CR_TXDIS_1             (0x1u <<  7) /**< \brief (US_CR) Disables the transmitter */
#define US_CR_RSTSTA_Pos            8            /**< \brief (US_CR) Reset Status Bits */
#define US_CR_RSTSTA                (0x1u << US_CR_RSTSTA_Pos)
#define   US_CR_RSTSTA_0            (0x0u <<  8) /**< \brief (US_CR) No effect */
#define   US_CR_RSTSTA_1            (0x1u <<  8) /**< \brief (US_CR) Resets the status bits PARE, FRAME, OVRE and RXBRK in the CSR */
#define US_CR_STTBRK_Pos            9            /**< \brief (US_CR) Start Break */
#define US_CR_STTBRK                (0x1u << US_CR_STTBRK_Pos)
#define   US_CR_STTBRK_0            (0x0u <<  9) /**< \brief (US_CR) No effect */
#define   US_CR_STTBRK_1            (0x1u <<  9) /**< \brief (US_CR) Starts transmission of a break after the characters present in THR and the Transmit Shift Register have been transmitted. No effect if a break is already being transmitted */
#define US_CR_STPBRK_Pos            10           /**< \brief (US_CR) Stop Break */
#define US_CR_STPBRK                (0x1u << US_CR_STPBRK_Pos)
#define   US_CR_STPBRK_0            (0x0u << 10) /**< \brief (US_CR) No effect */
#define   US_CR_STPBRK_1            (0x1u << 10) /**< \brief (US_CR) Stops transmission of the break after a minimum of one character length and transmits a high level during 12-bit periods.No effect if no break is being transmitted */
#define US_CR_STTTO_Pos             11           /**< \brief (US_CR) Start Time-out */
#define US_CR_STTTO                 (0x1u << US_CR_STTTO_Pos)
#define   US_CR_STTTO_0             (0x0u << 11) /**< \brief (US_CR) No effect */
#define   US_CR_STTTO_1             (0x1u << 11) /**< \brief (US_CR) Starts waiting for a character before clocking the time-out counter */
#define US_CR_SENDA_Pos             12           /**< \brief (US_CR) Send Address */
#define US_CR_SENDA                 (0x1u << US_CR_SENDA_Pos)
#define   US_CR_SENDA_0             (0x0u << 12) /**< \brief (US_CR) No effect */
#define   US_CR_SENDA_1             (0x1u << 12) /**< \brief (US_CR) In Multi-drop Mode only, the next character written to the THR is sent with the address bit set */
#define US_CR_RSTIT_Pos             13           /**< \brief (US_CR) Reset Iterations */
#define US_CR_RSTIT                 (0x1u << US_CR_RSTIT_Pos)
#define   US_CR_RSTIT_0             (0x0u << 13) /**< \brief (US_CR) No effect */
#define   US_CR_RSTIT_1             (0x1u << 13) /**< \brief (US_CR) Resets ITERATION in CSR. No effect if the ISO7816 is not enabled */
#define US_CR_RSTNACK_Pos           14           /**< \brief (US_CR) Reset Non Acknowledge */
#define US_CR_RSTNACK               (0x1u << US_CR_RSTNACK_Pos)
#define   US_CR_RSTNACK_0           (0x0u << 14) /**< \brief (US_CR) No effect */
#define   US_CR_RSTNACK_1           (0x1u << 14) /**< \brief (US_CR) Resets NACK in CSR */
#define US_CR_RETTO_Pos             15           /**< \brief (US_CR) Rearm Time-out */
#define US_CR_RETTO                 (0x1u << US_CR_RETTO_Pos)
#define   US_CR_RETTO_0             (0x0u << 15) /**< \brief (US_CR) No effect */
#define   US_CR_RETTO_1             (0x1u << 15) /**< \brief (US_CR) Restart Time-out */
#define US_CR_DTREN_Pos             16           /**< \brief (US_CR) Data Terminal Ready Enable */
#define US_CR_DTREN                 (0x1u << US_CR_DTREN_Pos)
#define   US_CR_DTREN_0             (0x0u << 16) /**< \brief (US_CR) No effect */
#define   US_CR_DTREN_1             (0x1u << 16) /**< \brief (US_CR) Drives the pin DTR at 0 */
#define US_CR_DTRDIS_Pos            17           /**< \brief (US_CR) Data Terminal Ready Disable */
#define US_CR_DTRDIS                (0x1u << US_CR_DTRDIS_Pos)
#define   US_CR_DTRDIS_0            (0x0u << 17) /**< \brief (US_CR) No effect */
#define   US_CR_DTRDIS_1            (0x1u << 17) /**< \brief (US_CR) Drives the pin DTR to 1 */
#define US_CR_RTSEN_Pos             18           /**< \brief (US_CR) Request to Send Enable */
#define US_CR_RTSEN                 (0x1u << US_CR_RTSEN_Pos)
#define   US_CR_RTSEN_0             (0x0u << 18) /**< \brief (US_CR) No effect */
#define   US_CR_RTSEN_1             (0x1u << 18) /**< \brief (US_CR) Drives the pin RTS to 0 */
#define US_CR_FCS_Pos               18           /**< \brief (US_CR) Force SPI Chip Select */
#define US_CR_FCS                   (0x1u << US_CR_FCS_Pos)
#define   US_CR_FCS_0               (0x0u << 18) /**< \brief (US_CR) No effect */
#define   US_CR_FCS_1               (0x1u << 18) /**< \brief (US_CR) Forces the Slave Select Line NSS (RTS pin) to 0, even if USART is no transmitting, in order to address SPI slave devices supporting the CSAAT Mode (Chip Select Active After Transfer) */
#define US_CR_RTSEN_Pos             18           /**< \brief (US_CR) Request to Send Enable */
#define US_CR_RTSEN                 (0x1u << US_CR_RTSEN_Pos)
#define   US_CR_RTSEN_0             (0x0u << 18) /**< \brief (US_CR) No effect */
#define   US_CR_RTSEN_1             (0x1u << 18) /**< \brief (US_CR) Drives the pin RTS to 0 */
#define US_CR_RTSDIS_Pos            19           /**< \brief (US_CR) Request to Send Disable */
#define US_CR_RTSDIS                (0x1u << US_CR_RTSDIS_Pos)
#define   US_CR_RTSDIS_0            (0x0u << 19) /**< \brief (US_CR) No effect */
#define   US_CR_RTSDIS_1            (0x1u << 19) /**< \brief (US_CR) Drives the pin RTS to 1 */
#define US_CR_RCS_Pos               19           /**< \brief (US_CR) Release SPI Chip Select */
#define US_CR_RCS                   (0x1u << US_CR_RCS_Pos)
#define   US_CR_RCS_0               (0x0u << 19) /**< \brief (US_CR) No effect */
#define   US_CR_RCS_1               (0x1u << 19) /**< \brief (US_CR) Releases the Slave Select Line NSS (RTS pin) */
#define US_CR_RTSDIS_Pos            19           /**< \brief (US_CR) Request to Send Disable */
#define US_CR_RTSDIS                (0x1u << US_CR_RTSDIS_Pos)
#define   US_CR_RTSDIS_0            (0x0u << 19) /**< \brief (US_CR) No effect */
#define   US_CR_RTSDIS_1            (0x1u << 19) /**< \brief (US_CR) Drives the pin RTS to 1 */
#define US_CR_LINABT_Pos            20           /**< \brief (US_CR) Abort the current LIN transmission */
#define US_CR_LINABT                (0x1u << US_CR_LINABT_Pos)
#define US_CR_LINWKUP_Pos           21           /**< \brief (US_CR) Sends a wakeup signal on the LIN bus */
#define US_CR_LINWKUP               (0x1u << US_CR_LINWKUP_Pos)
#define US_CR_MASK                  0x003FFFFCu  /**< \brief (US_CR) MASK Register */

/* -------- US_MR : (USART Offset: 0x04) (R/W 32) Mode Register -------- */
#define US_MR_OFFSET                0x04         /**< \brief (US_MR offset) Mode Register */
#define US_MR_RESETVALUE            0x00000000   /**< \brief (US_MR reset_value) Mode Register */

// SPI mode
#define US_MR_SPI_MODE_Pos          0            /**< \brief (US_MR_SPI) Usart Mode */
#define US_MR_SPI_MODE_Msk          (0xFu << US_MR_SPI_MODE_Pos)
#define US_MR_SPI_MODE(value)       ((US_MR_SPI_MODE_Msk & ((value) << US_MR_SPI_MODE_Pos)))
#define   US_MR_SPI_MODE_NORMAL     (0x0u <<  0) /**< \brief (US_MR_SPI) Normal */
#define   US_MR_SPI_MODE_RS485      (0x1u <<  0) /**< \brief (US_MR_SPI) RS485 */
#define   US_MR_SPI_MODE_HARDWARE   (0x2u <<  0) /**< \brief (US_MR_SPI) Hardware Handshaking */
#define   US_MR_SPI_MODE_MODEM      (0x3u <<  0) /**< \brief (US_MR_SPI) Modem */
#define   US_MR_SPI_MODE_ISO7816_T0 (0x4u <<  0) /**< \brief (US_MR_SPI) IS07816 Protocol: T = 0 */
#define   US_MR_SPI_MODE_ISO7816_T1 (0x6u <<  0) /**< \brief (US_MR_SPI) IS07816 Protocol: T = 1 */
#define   US_MR_SPI_MODE_IRDA       (0x8u <<  0) /**< \brief (US_MR_SPI) IrDA */
#define   US_MR_SPI_MODE_LIN_MASTER (0xAu <<  0) /**< \brief (US_MR_SPI) LIN Master */
#define   US_MR_SPI_MODE_LIN_SLAVE  (0xBu <<  0) /**< \brief (US_MR_SPI) LIN Slave */
#define   US_MR_SPI_MODE_SPI_MASTER (0xEu <<  0) /**< \brief (US_MR_SPI) SPI Master */
#define   US_MR_SPI_MODE_SPI_SLAVE  (0xFu <<  0) /**< \brief (US_MR_SPI) SPI Slave */
#define US_MR_SPI_USCLKS_Pos        4            /**< \brief (US_MR_SPI) Clock Selection */
#define US_MR_SPI_USCLKS_Msk        (0x3u << US_MR_SPI_USCLKS_Pos)
#define US_MR_SPI_USCLKS(value)     ((US_MR_SPI_USCLKS_Msk & ((value) << US_MR_SPI_USCLKS_Pos)))
#define   US_MR_SPI_USCLKS_MCK      (0x0u <<  4) /**< \brief (US_MR_SPI) MCK */
#define   US_MR_SPI_USCLKS_MCK_DIV  (0x1u <<  4) /**< \brief (US_MR_SPI) MCK / DIV */
#define   US_MR_SPI_USCLKS_2        (0x2u <<  4) /**< \brief (US_MR_SPI) Reserved */
#define   US_MR_SPI_USCLKS_SCK      (0x3u <<  4) /**< \brief (US_MR_SPI) SCK */
#define US_MR_SPI_CHRL_Pos          6            /**< \brief (US_MR_SPI) Character Length. */
#define US_MR_SPI_CHRL_Msk          (0x3u << US_MR_SPI_CHRL_Pos)
#define US_MR_SPI_CHRL(value)       ((US_MR_SPI_CHRL_Msk & ((value) << US_MR_SPI_CHRL_Pos)))
#define   US_MR_SPI_CHRL_5          (0x0u <<  6) /**< \brief (US_MR_SPI) 5 bits */
#define   US_MR_SPI_CHRL_6          (0x1u <<  6) /**< \brief (US_MR_SPI) 6 bits */
#define   US_MR_SPI_CHRL_7          (0x2u <<  6) /**< \brief (US_MR_SPI) 7 bits */
#define   US_MR_SPI_CHRL_8          (0x3u <<  6) /**< \brief (US_MR_SPI) 8 bits */
#define US_MR_SPI_CPHA_Pos          8            /**< \brief (US_MR_SPI) SPI CLock Phase */
#define US_MR_SPI_CPHA              (0x1u << US_MR_SPI_CPHA_Pos)
#define   US_MR_SPI_CPHA_0          (0x0u <<  8) /**< \brief (US_MR_SPI) Data is changed on the leading edge of SPCK and captured on the following edge of SPCK */
#define   US_MR_SPI_CPHA_1          (0x1u <<  8) /**< \brief (US_MR_SPI) Data is captured on the leading edge of SPCK and changed on the following edge of SPCK */
#define US_MR_SPI_PAR_Pos           9            /**< \brief (US_MR_SPI) Parity Type */
#define US_MR_SPI_PAR_Msk           (0x7u << US_MR_SPI_PAR_Pos)
#define US_MR_SPI_PAR(value)        ((US_MR_SPI_PAR_Msk & ((value) << US_MR_SPI_PAR_Pos)))
#define   US_MR_SPI_PAR_EVEN        (0x0u <<  9) /**< \brief (US_MR_SPI) Even parity */
#define   US_MR_SPI_PAR_ODD         (0x1u <<  9) /**< \brief (US_MR_SPI) Odd parity */
#define   US_MR_SPI_PAR_SPACE       (0x2u <<  9) /**< \brief (US_MR_SPI) Parity forced to 0 (Space) */
#define   US_MR_SPI_PAR_MARK        (0x3u <<  9) /**< \brief (US_MR_SPI) Parity forced to 1 (Mark) */
#define   US_MR_SPI_PAR_NONE        (0x4u <<  9) /**< \brief (US_MR_SPI) No Parity */
#define   US_MR_SPI_PAR_5           (0x5u <<  9) /**< \brief (US_MR_SPI) No Parity */
#define   US_MR_SPI_PAR_MULTI       (0x6u <<  9) /**< \brief (US_MR_SPI) Multi-drop mode */
#define   US_MR_SPI_PAR_7           (0x7u <<  9) /**< \brief (US_MR_SPI) Multi-drop mode */
#define US_MR_SPI_NBSTOP_Pos        12           /**< \brief (US_MR_SPI) Number of Stop Bits */
#define US_MR_SPI_NBSTOP_Msk        (0x3u << US_MR_SPI_NBSTOP_Pos)
#define US_MR_SPI_NBSTOP(value)     ((US_MR_SPI_NBSTOP_Msk & ((value) << US_MR_SPI_NBSTOP_Pos)))
#define   US_MR_SPI_NBSTOP_1        (0x0u << 12) /**< \brief (US_MR_SPI) 1 stop bit */
#define   US_MR_SPI_NBSTOP_1_5      (0x1u << 12) /**< \brief (US_MR_SPI) 1.5 stop bits (Only valid if SYNC=0) */
#define   US_MR_SPI_NBSTOP_2        (0x2u << 12) /**< \brief (US_MR_SPI) 2 stop bits */
#define   US_MR_SPI_NBSTOP_3        (0x3u << 12) /**< \brief (US_MR_SPI) Reserved */
#define US_MR_SPI_CHMODE_Pos        14           /**< \brief (US_MR_SPI) Channel Mode */
#define US_MR_SPI_CHMODE_Msk        (0x3u << US_MR_SPI_CHMODE_Pos)
#define US_MR_SPI_CHMODE(value)     ((US_MR_SPI_CHMODE_Msk & ((value) << US_MR_SPI_CHMODE_Pos)))
#define   US_MR_SPI_CHMODE_NORMAL   (0x0u << 14) /**< \brief (US_MR_SPI) Normal Mode */
#define   US_MR_SPI_CHMODE_ECHO     (0x1u << 14) /**< \brief (US_MR_SPI) Automatic Echo. Receiver input is connected to the TXD pin */
#define   US_MR_SPI_CHMODE_LOCAL_LOOP (0x2u << 14) /**< \brief (US_MR_SPI) Local Loopback. Transmitter output is connected to the Receiver Input */
#define   US_MR_SPI_CHMODE_REMOTE_LOOP (0x3u << 14) /**< \brief (US_MR_SPI) Remote Loopback. RXD pin is internally connected to the TXD pin */
#define US_MR_SPI_CPOL_Pos          16           /**< \brief (US_MR_SPI) SPI Clock Polarity */
#define US_MR_SPI_CPOL              (0x1u << US_MR_SPI_CPOL_Pos)
#define   US_MR_SPI_CPOL_ZERO       (0x0u << 16) /**< \brief (US_MR_SPI) The inactive state value of SPCK is logic level zero */
#define   US_MR_SPI_CPOL_ONE        (0x1u << 16) /**< \brief (US_MR_SPI) The inactive state value of SPCK is logic level one */
#define US_MR_SPI_MODE9_Pos         17           /**< \brief (US_MR_SPI) 9-bit Character Length */
#define US_MR_SPI_MODE9             (0x1u << US_MR_SPI_MODE9_Pos)
#define   US_MR_SPI_MODE9_0         (0x0u << 17) /**< \brief (US_MR_SPI) CHRL defines character length */
#define   US_MR_SPI_MODE9_1         (0x1u << 17) /**< \brief (US_MR_SPI) 9-bit character length */
#define US_MR_SPI_CLKO_Pos          18           /**< \brief (US_MR_SPI) Clock Output Select */
#define US_MR_SPI_CLKO              (0x1u << US_MR_SPI_CLKO_Pos)
#define   US_MR_SPI_CLKO_0          (0x0u << 18) /**< \brief (US_MR_SPI) The USART does not drive the SCK pin */
#define   US_MR_SPI_CLKO_1          (0x1u << 18) /**< \brief (US_MR_SPI) The USART drives the SCK pin if USCLKS does not select the external clock SCK */
#define US_MR_SPI_OVER_Pos          19           /**< \brief (US_MR_SPI) Oversampling Mode */
#define US_MR_SPI_OVER              (0x1u << US_MR_SPI_OVER_Pos)
#define   US_MR_SPI_OVER_X16        (0x0u << 19) /**< \brief (US_MR_SPI) 16x Oversampling */
#define   US_MR_SPI_OVER_X8         (0x1u << 19) /**< \brief (US_MR_SPI) 8x Oversampling */
#define US_MR_SPI_INACK_Pos         20           /**< \brief (US_MR_SPI) Inhibit Non Acknowledge */
#define US_MR_SPI_INACK             (0x1u << US_MR_SPI_INACK_Pos)
#define   US_MR_SPI_INACK_0         (0x0u << 20) /**< \brief (US_MR_SPI) The NACK is generated */
#define   US_MR_SPI_INACK_1         (0x1u << 20) /**< \brief (US_MR_SPI) The NACK is not generated */
#define US_MR_SPI_DSNACK_Pos        21           /**< \brief (US_MR_SPI) Disable Successive NACK */
#define US_MR_SPI_DSNACK            (0x1u << US_MR_SPI_DSNACK_Pos)
#define   US_MR_SPI_DSNACK_0        (0x0u << 21) /**< \brief (US_MR_SPI) NACK is sent on the ISO line as soon as a parity error occurs in the received character (unless INACK is set) */
#define   US_MR_SPI_DSNACK_1        (0x1u << 21) /**< \brief (US_MR_SPI) Successive parity errors are counted up to the value specified in the MAX_ITERATION field. These parity errors generatea NACK on the ISO line. As soon as this value is reached, no additional NACK is sent on the ISO line. The flag ITERATION is asserted */
#define US_MR_SPI_INVDATA_Pos       23           /**< \brief (US_MR_SPI) Inverted data */
#define US_MR_SPI_INVDATA           (0x1u << US_MR_SPI_INVDATA_Pos)
#define US_MR_SPI_MAX_ITERATION_Pos 24           /**< \brief (US_MR_SPI) Max interation */
#define US_MR_SPI_MAX_ITERATION_Msk (0x7u << US_MR_SPI_MAX_ITERATION_Pos)
#define US_MR_SPI_MAX_ITERATION(value) ((US_MR_SPI_MAX_ITERATION_Msk & ((value) << US_MR_SPI_MAX_ITERATION_Pos)))
#define US_MR_SPI_FILTER_Pos        28           /**< \brief (US_MR_SPI) Infrared Receive Line Filter */
#define US_MR_SPI_FILTER            (0x1u << US_MR_SPI_FILTER_Pos)
#define   US_MR_SPI_FILTER_0        (0x0u << 28) /**< \brief (US_MR_SPI) The USART does not filter the receive line */
#define   US_MR_SPI_FILTER_1        (0x1u << 28) /**< \brief (US_MR_SPI) The USART filters the receive line using a three-sample filter (1/16-bit clock) (2 over 3 majority) */
#define US_MR_SPI_MASK              0x17BFFFFFu  /**< \brief (US_MR_SPI) MASK Register */

// USART mode
#define US_MR_USART_MODE_Pos        0            /**< \brief (US_MR_USART) Usart Mode */
#define US_MR_USART_MODE_Msk        (0xFu << US_MR_USART_MODE_Pos)
#define US_MR_USART_MODE(value)     ((US_MR_USART_MODE_Msk & ((value) << US_MR_USART_MODE_Pos)))
#define   US_MR_USART_MODE_NORMAL   (0x0u <<  0) /**< \brief (US_MR_USART) Normal */
#define   US_MR_USART_MODE_RS485    (0x1u <<  0) /**< \brief (US_MR_USART) RS485 */
#define   US_MR_USART_MODE_HARDWARE (0x2u <<  0) /**< \brief (US_MR_USART) Hardware Handshaking */
#define   US_MR_USART_MODE_MODEM    (0x3u <<  0) /**< \brief (US_MR_USART) Modem */
#define   US_MR_USART_MODE_ISO7816_T0 (0x4u <<  0) /**< \brief (US_MR_USART) IS07816 Protocol: T = 0 */
#define   US_MR_USART_MODE_ISO7816_T1 (0x6u <<  0) /**< \brief (US_MR_USART) IS07816 Protocol: T = 1 */
#define   US_MR_USART_MODE_IRDA     (0x8u <<  0) /**< \brief (US_MR_USART) IrDA */
#define   US_MR_USART_MODE_LIN_MASTER (0xAu <<  0) /**< \brief (US_MR_USART) LIN Master */
#define   US_MR_USART_MODE_LIN_SLAVE (0xBu <<  0) /**< \brief (US_MR_USART) LIN Slave */
#define   US_MR_USART_MODE_SPI_MASTER (0xEu <<  0) /**< \brief (US_MR_USART) SPI Master */
#define   US_MR_USART_MODE_SPI_SLAVE (0xFu <<  0) /**< \brief (US_MR_USART) SPI Slave */
#define US_MR_USART_USCLKS_Pos      4            /**< \brief (US_MR_USART) Clock Selection */
#define US_MR_USART_USCLKS_Msk      (0x3u << US_MR_USART_USCLKS_Pos)
#define US_MR_USART_USCLKS(value)   ((US_MR_USART_USCLKS_Msk & ((value) << US_MR_USART_USCLKS_Pos)))
#define   US_MR_USART_USCLKS_MCK    (0x0u <<  4) /**< \brief (US_MR_USART) MCK */
#define   US_MR_USART_USCLKS_MCK_DIV (0x1u <<  4) /**< \brief (US_MR_USART) MCK / DIV */
#define   US_MR_USART_USCLKS_2      (0x2u <<  4) /**< \brief (US_MR_USART) Reserved */
#define   US_MR_USART_USCLKS_SCK    (0x3u <<  4) /**< \brief (US_MR_USART) SCK */
#define US_MR_USART_CHRL_Pos        6            /**< \brief (US_MR_USART) Character Length. */
#define US_MR_USART_CHRL_Msk        (0x3u << US_MR_USART_CHRL_Pos)
#define US_MR_USART_CHRL(value)     ((US_MR_USART_CHRL_Msk & ((value) << US_MR_USART_CHRL_Pos)))
#define   US_MR_USART_CHRL_5        (0x0u <<  6) /**< \brief (US_MR_USART) 5 bits */
#define   US_MR_USART_CHRL_6        (0x1u <<  6) /**< \brief (US_MR_USART) 6 bits */
#define   US_MR_USART_CHRL_7        (0x2u <<  6) /**< \brief (US_MR_USART) 7 bits */
#define   US_MR_USART_CHRL_8        (0x3u <<  6) /**< \brief (US_MR_USART) 8 bits */
#define US_MR_USART_SYNC_Pos        8            /**< \brief (US_MR_USART) Synchronous Mode Select */
#define US_MR_USART_SYNC            (0x1u << US_MR_USART_SYNC_Pos)
#define   US_MR_USART_SYNC_0        (0x0u <<  8) /**< \brief (US_MR_USART) USART operates in Synchronous Mode */
#define   US_MR_USART_SYNC_1        (0x1u <<  8) /**< \brief (US_MR_USART) USART operates in Asynchronous Mode */
#define US_MR_USART_PAR_Pos         9            /**< \brief (US_MR_USART) Parity Type */
#define US_MR_USART_PAR_Msk         (0x7u << US_MR_USART_PAR_Pos)
#define US_MR_USART_PAR(value)      ((US_MR_USART_PAR_Msk & ((value) << US_MR_USART_PAR_Pos)))
#define   US_MR_USART_PAR_EVEN      (0x0u <<  9) /**< \brief (US_MR_USART) Even parity */
#define   US_MR_USART_PAR_ODD       (0x1u <<  9) /**< \brief (US_MR_USART) Odd parity */
#define   US_MR_USART_PAR_SPACE     (0x2u <<  9) /**< \brief (US_MR_USART) Parity forced to 0 (Space) */
#define   US_MR_USART_PAR_MARK      (0x3u <<  9) /**< \brief (US_MR_USART) Parity forced to 1 (Mark) */
#define   US_MR_USART_PAR_NONE      (0x4u <<  9) /**< \brief (US_MR_USART) No Parity */
#define   US_MR_USART_PAR_5         (0x5u <<  9) /**< \brief (US_MR_USART) No Parity */
#define   US_MR_USART_PAR_MULTI     (0x6u <<  9) /**< \brief (US_MR_USART) Multi-drop mode */
#define   US_MR_USART_PAR_7         (0x7u <<  9) /**< \brief (US_MR_USART) Multi-drop mode */
#define US_MR_USART_NBSTOP_Pos      12           /**< \brief (US_MR_USART) Number of Stop Bits */
#define US_MR_USART_NBSTOP_Msk      (0x3u << US_MR_USART_NBSTOP_Pos)
#define US_MR_USART_NBSTOP(value)   ((US_MR_USART_NBSTOP_Msk & ((value) << US_MR_USART_NBSTOP_Pos)))
#define   US_MR_USART_NBSTOP_1      (0x0u << 12) /**< \brief (US_MR_USART) 1 stop bit */
#define   US_MR_USART_NBSTOP_1_5    (0x1u << 12) /**< \brief (US_MR_USART) 1.5 stop bits (Only valid if SYNC=0) */
#define   US_MR_USART_NBSTOP_2      (0x2u << 12) /**< \brief (US_MR_USART) 2 stop bits */
#define   US_MR_USART_NBSTOP_3      (0x3u << 12) /**< \brief (US_MR_USART) Reserved */
#define US_MR_USART_CHMODE_Pos      14           /**< \brief (US_MR_USART) Channel Mode */
#define US_MR_USART_CHMODE_Msk      (0x3u << US_MR_USART_CHMODE_Pos)
#define US_MR_USART_CHMODE(value)   ((US_MR_USART_CHMODE_Msk & ((value) << US_MR_USART_CHMODE_Pos)))
#define   US_MR_USART_CHMODE_NORMAL (0x0u << 14) /**< \brief (US_MR_USART) Normal Mode */
#define   US_MR_USART_CHMODE_ECHO   (0x1u << 14) /**< \brief (US_MR_USART) Automatic Echo. Receiver input is connected to the TXD pin */
#define   US_MR_USART_CHMODE_LOCAL_LOOP (0x2u << 14) /**< \brief (US_MR_USART) Local Loopback. Transmitter output is connected to the Receiver Input */
#define   US_MR_USART_CHMODE_REMOTE_LOOP (0x3u << 14) /**< \brief (US_MR_USART) Remote Loopback. RXD pin is internally connected to the TXD pin */
#define US_MR_USART_MSBF_Pos        16           /**< \brief (US_MR_USART) Bit Order */
#define US_MR_USART_MSBF            (0x1u << US_MR_USART_MSBF_Pos)
#define   US_MR_USART_MSBF_LSBF     (0x0u << 16) /**< \brief (US_MR_USART) Least Significant Bit first */
#define   US_MR_USART_MSBF_MSBF     (0x1u << 16) /**< \brief (US_MR_USART) Most Significant Bit first */
#define US_MR_USART_MODE9_Pos       17           /**< \brief (US_MR_USART) 9-bit Character Length */
#define US_MR_USART_MODE9           (0x1u << US_MR_USART_MODE9_Pos)
#define   US_MR_USART_MODE9_0       (0x0u << 17) /**< \brief (US_MR_USART) CHRL defines character length */
#define   US_MR_USART_MODE9_1       (0x1u << 17) /**< \brief (US_MR_USART) 9-bit character length */
#define US_MR_USART_CLKO_Pos        18           /**< \brief (US_MR_USART) Clock Output Select */
#define US_MR_USART_CLKO            (0x1u << US_MR_USART_CLKO_Pos)
#define   US_MR_USART_CLKO_0        (0x0u << 18) /**< \brief (US_MR_USART) The USART does not drive the SCK pin */
#define   US_MR_USART_CLKO_1        (0x1u << 18) /**< \brief (US_MR_USART) The USART drives the SCK pin if USCLKS does not select the external clock SCK */
#define US_MR_USART_OVER_Pos        19           /**< \brief (US_MR_USART) Oversampling Mode */
#define US_MR_USART_OVER            (0x1u << US_MR_USART_OVER_Pos)
#define   US_MR_USART_OVER_X16      (0x0u << 19) /**< \brief (US_MR_USART) 16x Oversampling */
#define   US_MR_USART_OVER_X8       (0x1u << 19) /**< \brief (US_MR_USART) 8x Oversampling */
#define US_MR_USART_INACK_Pos       20           /**< \brief (US_MR_USART) Inhibit Non Acknowledge */
#define US_MR_USART_INACK           (0x1u << US_MR_USART_INACK_Pos)
#define   US_MR_USART_INACK_0       (0x0u << 20) /**< \brief (US_MR_USART) The NACK is generated */
#define   US_MR_USART_INACK_1       (0x1u << 20) /**< \brief (US_MR_USART) The NACK is not generated */
#define US_MR_USART_DSNACK_Pos      21           /**< \brief (US_MR_USART) Disable Successive NACK */
#define US_MR_USART_DSNACK          (0x1u << US_MR_USART_DSNACK_Pos)
#define   US_MR_USART_DSNACK_0      (0x0u << 21) /**< \brief (US_MR_USART) NACK is sent on the ISO line as soon as a parity error occurs in the received character (unless INACK is set) */
#define   US_MR_USART_DSNACK_1      (0x1u << 21) /**< \brief (US_MR_USART) Successive parity errors are counted up to the value specified in the MAX_ITERATION field. These parity errors generatea NACK on the ISO line. As soon as this value is reached, no additional NACK is sent on the ISO line. The flag ITERATION is asserted */
#define US_MR_USART_VAR_SYNC_Pos    22           /**< \brief (US_MR_USART) Variable synchronization of command/data sync Start Frame Delimiter */
#define US_MR_USART_VAR_SYNC        (0x1u << US_MR_USART_VAR_SYNC_Pos)
#define   US_MR_USART_VAR_SYNC_0    (0x0u << 22) /**< \brief (US_MR_USART) User defined configuration of command or data sync field depending on SYNC value */
#define   US_MR_USART_VAR_SYNC_1    (0x1u << 22) /**< \brief (US_MR_USART) The sync field is updated when a character is written into THR register */
#define US_MR_USART_INVDATA_Pos     23           /**< \brief (US_MR_USART) Inverted data */
#define US_MR_USART_INVDATA         (0x1u << US_MR_USART_INVDATA_Pos)
#define US_MR_USART_MAX_ITERATION_Pos 24           /**< \brief (US_MR_USART) Max interation */
#define US_MR_USART_MAX_ITERATION_Msk (0x7u << US_MR_USART_MAX_ITERATION_Pos)
#define US_MR_USART_MAX_ITERATION(value) ((US_MR_USART_MAX_ITERATION_Msk & ((value) << US_MR_USART_MAX_ITERATION_Pos)))
#define US_MR_USART_FILTER_Pos      28           /**< \brief (US_MR_USART) Infrared Receive Line Filter */
#define US_MR_USART_FILTER          (0x1u << US_MR_USART_FILTER_Pos)
#define   US_MR_USART_FILTER_0      (0x0u << 28) /**< \brief (US_MR_USART) The USART does not filter the receive line */
#define   US_MR_USART_FILTER_1      (0x1u << 28) /**< \brief (US_MR_USART) The USART filters the receive line using a three-sample filter (1/16-bit clock) (2 over 3 majority) */
#define US_MR_USART_MAN_Pos         29           /**< \brief (US_MR_USART) Manchester Encoder/Decoder Enable */
#define US_MR_USART_MAN             (0x1u << US_MR_USART_MAN_Pos)
#define   US_MR_USART_MAN_0         (0x0u << 29) /**< \brief (US_MR_USART) Manchester Encoder/Decoder is disabled */
#define   US_MR_USART_MAN_1         (0x1u << 29) /**< \brief (US_MR_USART) Manchester Encoder/Decoder is enabled */
#define US_MR_USART_MODSYNC_Pos     30           /**< \brief (US_MR_USART) Manchester Synchronization Mode */
#define US_MR_USART_MODSYNC         (0x1u << US_MR_USART_MODSYNC_Pos)
#define   US_MR_USART_MODSYNC_0     (0x0u << 30) /**< \brief (US_MR_USART) The Manchester Start bit is a 0 to 1 transition */
#define   US_MR_USART_MODSYNC_1     (0x1u << 30) /**< \brief (US_MR_USART) The Manchester Start bit is a 1 to 0 transition */
#define US_MR_USART_ONEBIT_Pos      31           /**< \brief (US_MR_USART) Start Frame Delimiter selector */
#define US_MR_USART_ONEBIT          (0x1u << US_MR_USART_ONEBIT_Pos)
#define   US_MR_USART_ONEBIT_0      (0x0u << 31) /**< \brief (US_MR_USART) Start Frame delimiter is COMMAND or DATA SYNC */
#define   US_MR_USART_ONEBIT_1      (0x1u << 31) /**< \brief (US_MR_USART) Start Frame delimiter is One Bit */
#define US_MR_USART_MASK            0xF7FFFFFFu  /**< \brief (US_MR_USART) MASK Register */

// Any mode
#define US_MR_MODE_Pos              0            /**< \brief (US_MR) Usart Mode */
#define US_MR_MODE_Msk              (0xFu << US_MR_MODE_Pos)
#define US_MR_MODE(value)           ((US_MR_MODE_Msk & ((value) << US_MR_MODE_Pos)))
#define   US_MR_MODE_NORMAL         (0x0u <<  0) /**< \brief (US_MR) Normal */
#define   US_MR_MODE_RS485          (0x1u <<  0) /**< \brief (US_MR) RS485 */
#define   US_MR_MODE_HARDWARE       (0x2u <<  0) /**< \brief (US_MR) Hardware Handshaking */
#define   US_MR_MODE_MODEM          (0x3u <<  0) /**< \brief (US_MR) Modem */
#define   US_MR_MODE_ISO7816_T0     (0x4u <<  0) /**< \brief (US_MR) IS07816 Protocol: T = 0 */
#define   US_MR_MODE_ISO7816_T1     (0x6u <<  0) /**< \brief (US_MR) IS07816 Protocol: T = 1 */
#define   US_MR_MODE_IRDA           (0x8u <<  0) /**< \brief (US_MR) IrDA */
#define   US_MR_MODE_LIN_MASTER     (0xAu <<  0) /**< \brief (US_MR) LIN Master */
#define   US_MR_MODE_LIN_SLAVE      (0xBu <<  0) /**< \brief (US_MR) LIN Slave */
#define   US_MR_MODE_SPI_MASTER     (0xEu <<  0) /**< \brief (US_MR) SPI Master */
#define   US_MR_MODE_SPI_SLAVE      (0xFu <<  0) /**< \brief (US_MR) SPI Slave */
#define US_MR_USCLKS_Pos            4            /**< \brief (US_MR) Clock Selection */
#define US_MR_USCLKS_Msk            (0x3u << US_MR_USCLKS_Pos)
#define US_MR_USCLKS(value)         ((US_MR_USCLKS_Msk & ((value) << US_MR_USCLKS_Pos)))
#define   US_MR_USCLKS_MCK          (0x0u <<  4) /**< \brief (US_MR) MCK */
#define   US_MR_USCLKS_MCK_DIV      (0x1u <<  4) /**< \brief (US_MR) MCK / DIV */
#define   US_MR_USCLKS_2            (0x2u <<  4) /**< \brief (US_MR) Reserved */
#define   US_MR_USCLKS_SCK          (0x3u <<  4) /**< \brief (US_MR) SCK */
#define US_MR_CHRL_Pos              6            /**< \brief (US_MR) Character Length. */
#define US_MR_CHRL_Msk              (0x3u << US_MR_CHRL_Pos)
#define US_MR_CHRL(value)           ((US_MR_CHRL_Msk & ((value) << US_MR_CHRL_Pos)))
#define   US_MR_CHRL_5              (0x0u <<  6) /**< \brief (US_MR) 5 bits */
#define   US_MR_CHRL_6              (0x1u <<  6) /**< \brief (US_MR) 6 bits */
#define   US_MR_CHRL_7              (0x2u <<  6) /**< \brief (US_MR) 7 bits */
#define   US_MR_CHRL_8              (0x3u <<  6) /**< \brief (US_MR) 8 bits */
#define US_MR_CPHA_Pos              8            /**< \brief (US_MR) SPI CLock Phase */
#define US_MR_CPHA                  (0x1u << US_MR_CPHA_Pos)
#define   US_MR_CPHA_0              (0x0u <<  8) /**< \brief (US_MR) Data is changed on the leading edge of SPCK and captured on the following edge of SPCK */
#define   US_MR_CPHA_1              (0x1u <<  8) /**< \brief (US_MR) Data is captured on the leading edge of SPCK and changed on the following edge of SPCK */
#define US_MR_SYNC_Pos              8            /**< \brief (US_MR) Synchronous Mode Select */
#define US_MR_SYNC                  (0x1u << US_MR_SYNC_Pos)
#define   US_MR_SYNC_0              (0x0u <<  8) /**< \brief (US_MR) USART operates in Synchronous Mode */
#define   US_MR_SYNC_1              (0x1u <<  8) /**< \brief (US_MR) USART operates in Asynchronous Mode */
#define US_MR_PAR_Pos               9            /**< \brief (US_MR) Parity Type */
#define US_MR_PAR_Msk               (0x7u << US_MR_PAR_Pos)
#define US_MR_PAR(value)            ((US_MR_PAR_Msk & ((value) << US_MR_PAR_Pos)))
#define   US_MR_PAR_EVEN            (0x0u <<  9) /**< \brief (US_MR) Even parity */
#define   US_MR_PAR_ODD             (0x1u <<  9) /**< \brief (US_MR) Odd parity */
#define   US_MR_PAR_SPACE           (0x2u <<  9) /**< \brief (US_MR) Parity forced to 0 (Space) */
#define   US_MR_PAR_MARK            (0x3u <<  9) /**< \brief (US_MR) Parity forced to 1 (Mark) */
#define   US_MR_PAR_NONE            (0x4u <<  9) /**< \brief (US_MR) No Parity */
#define   US_MR_PAR_5               (0x5u <<  9) /**< \brief (US_MR) No Parity */
#define   US_MR_PAR_MULTI           (0x6u <<  9) /**< \brief (US_MR) Multi-drop mode */
#define   US_MR_PAR_7               (0x7u <<  9) /**< \brief (US_MR) Multi-drop mode */
#define US_MR_NBSTOP_Pos            12           /**< \brief (US_MR) Number of Stop Bits */
#define US_MR_NBSTOP_Msk            (0x3u << US_MR_NBSTOP_Pos)
#define US_MR_NBSTOP(value)         ((US_MR_NBSTOP_Msk & ((value) << US_MR_NBSTOP_Pos)))
#define   US_MR_NBSTOP_1            (0x0u << 12) /**< \brief (US_MR) 1 stop bit */
#define   US_MR_NBSTOP_1_5          (0x1u << 12) /**< \brief (US_MR) 1.5 stop bits (Only valid if SYNC=0) */
#define   US_MR_NBSTOP_2            (0x2u << 12) /**< \brief (US_MR) 2 stop bits */
#define   US_MR_NBSTOP_3            (0x3u << 12) /**< \brief (US_MR) Reserved */
#define US_MR_CHMODE_Pos            14           /**< \brief (US_MR) Channel Mode */
#define US_MR_CHMODE_Msk            (0x3u << US_MR_CHMODE_Pos)
#define US_MR_CHMODE(value)         ((US_MR_CHMODE_Msk & ((value) << US_MR_CHMODE_Pos)))
#define   US_MR_CHMODE_NORMAL       (0x0u << 14) /**< \brief (US_MR) Normal Mode */
#define   US_MR_CHMODE_ECHO         (0x1u << 14) /**< \brief (US_MR) Automatic Echo. Receiver input is connected to the TXD pin */
#define   US_MR_CHMODE_LOCAL_LOOP   (0x2u << 14) /**< \brief (US_MR) Local Loopback. Transmitter output is connected to the Receiver Input */
#define   US_MR_CHMODE_REMOTE_LOOP  (0x3u << 14) /**< \brief (US_MR) Remote Loopback. RXD pin is internally connected to the TXD pin */
#define US_MR_CPOL_Pos              16           /**< \brief (US_MR) SPI Clock Polarity */
#define US_MR_CPOL                  (0x1u << US_MR_CPOL_Pos)
#define   US_MR_CPOL_ZERO           (0x0u << 16) /**< \brief (US_MR) The inactive state value of SPCK is logic level zero */
#define   US_MR_CPOL_ONE            (0x1u << 16) /**< \brief (US_MR) The inactive state value of SPCK is logic level one */
#define US_MR_MSBF_Pos              16           /**< \brief (US_MR) Bit Order */
#define US_MR_MSBF                  (0x1u << US_MR_MSBF_Pos)
#define   US_MR_MSBF_LSBF           (0x0u << 16) /**< \brief (US_MR) Least Significant Bit first */
#define   US_MR_MSBF_MSBF           (0x1u << 16) /**< \brief (US_MR) Most Significant Bit first */
#define US_MR_MODE9_Pos             17           /**< \brief (US_MR) 9-bit Character Length */
#define US_MR_MODE9                 (0x1u << US_MR_MODE9_Pos)
#define   US_MR_MODE9_0             (0x0u << 17) /**< \brief (US_MR) CHRL defines character length */
#define   US_MR_MODE9_1             (0x1u << 17) /**< \brief (US_MR) 9-bit character length */
#define US_MR_CLKO_Pos              18           /**< \brief (US_MR) Clock Output Select */
#define US_MR_CLKO                  (0x1u << US_MR_CLKO_Pos)
#define   US_MR_CLKO_0              (0x0u << 18) /**< \brief (US_MR) The USART does not drive the SCK pin */
#define   US_MR_CLKO_1              (0x1u << 18) /**< \brief (US_MR) The USART drives the SCK pin if USCLKS does not select the external clock SCK */
#define US_MR_OVER_Pos              19           /**< \brief (US_MR) Oversampling Mode */
#define US_MR_OVER                  (0x1u << US_MR_OVER_Pos)
#define   US_MR_OVER_X16            (0x0u << 19) /**< \brief (US_MR) 16x Oversampling */
#define   US_MR_OVER_X8             (0x1u << 19) /**< \brief (US_MR) 8x Oversampling */
#define US_MR_INACK_Pos             20           /**< \brief (US_MR) Inhibit Non Acknowledge */
#define US_MR_INACK                 (0x1u << US_MR_INACK_Pos)
#define   US_MR_INACK_0             (0x0u << 20) /**< \brief (US_MR) The NACK is generated */
#define   US_MR_INACK_1             (0x1u << 20) /**< \brief (US_MR) The NACK is not generated */
#define US_MR_DSNACK_Pos            21           /**< \brief (US_MR) Disable Successive NACK */
#define US_MR_DSNACK                (0x1u << US_MR_DSNACK_Pos)
#define   US_MR_DSNACK_0            (0x0u << 21) /**< \brief (US_MR) NACK is sent on the ISO line as soon as a parity error occurs in the received character (unless INACK is set) */
#define   US_MR_DSNACK_1            (0x1u << 21) /**< \brief (US_MR) Successive parity errors are counted up to the value specified in the MAX_ITERATION field. These parity errors generatea NACK on the ISO line. As soon as this value is reached, no additional NACK is sent on the ISO line. The flag ITERATION is asserted */
#define US_MR_VAR_SYNC_Pos          22           /**< \brief (US_MR) Variable synchronization of command/data sync Start Frame Delimiter */
#define US_MR_VAR_SYNC              (0x1u << US_MR_VAR_SYNC_Pos)
#define   US_MR_VAR_SYNC_0          (0x0u << 22) /**< \brief (US_MR) User defined configuration of command or data sync field depending on SYNC value */
#define   US_MR_VAR_SYNC_1          (0x1u << 22) /**< \brief (US_MR) The sync field is updated when a character is written into THR register */
#define US_MR_INVDATA_Pos           23           /**< \brief (US_MR) Inverted data */
#define US_MR_INVDATA               (0x1u << US_MR_INVDATA_Pos)
#define US_MR_MAX_ITERATION_Pos     24           /**< \brief (US_MR) Max interation */
#define US_MR_MAX_ITERATION_Msk     (0x7u << US_MR_MAX_ITERATION_Pos)
#define US_MR_MAX_ITERATION(value)  ((US_MR_MAX_ITERATION_Msk & ((value) << US_MR_MAX_ITERATION_Pos)))
#define US_MR_FILTER_Pos            28           /**< \brief (US_MR) Infrared Receive Line Filter */
#define US_MR_FILTER                (0x1u << US_MR_FILTER_Pos)
#define   US_MR_FILTER_0            (0x0u << 28) /**< \brief (US_MR) The USART does not filter the receive line */
#define   US_MR_FILTER_1            (0x1u << 28) /**< \brief (US_MR) The USART filters the receive line using a three-sample filter (1/16-bit clock) (2 over 3 majority) */
#define US_MR_MAN_Pos               29           /**< \brief (US_MR) Manchester Encoder/Decoder Enable */
#define US_MR_MAN                   (0x1u << US_MR_MAN_Pos)
#define   US_MR_MAN_0               (0x0u << 29) /**< \brief (US_MR) Manchester Encoder/Decoder is disabled */
#define   US_MR_MAN_1               (0x1u << 29) /**< \brief (US_MR) Manchester Encoder/Decoder is enabled */
#define US_MR_MODSYNC_Pos           30           /**< \brief (US_MR) Manchester Synchronization Mode */
#define US_MR_MODSYNC               (0x1u << US_MR_MODSYNC_Pos)
#define   US_MR_MODSYNC_0           (0x0u << 30) /**< \brief (US_MR) The Manchester Start bit is a 0 to 1 transition */
#define   US_MR_MODSYNC_1           (0x1u << 30) /**< \brief (US_MR) The Manchester Start bit is a 1 to 0 transition */
#define US_MR_ONEBIT_Pos            31           /**< \brief (US_MR) Start Frame Delimiter selector */
#define US_MR_ONEBIT                (0x1u << US_MR_ONEBIT_Pos)
#define   US_MR_ONEBIT_0            (0x0u << 31) /**< \brief (US_MR) Start Frame delimiter is COMMAND or DATA SYNC */
#define   US_MR_ONEBIT_1            (0x1u << 31) /**< \brief (US_MR) Start Frame delimiter is One Bit */
#define US_MR_MASK                  0xF7FFFFFFu  /**< \brief (US_MR) MASK Register */

/* -------- US_IER : (USART Offset: 0x08) ( /W 32) Interrupt Enable Register -------- */
#define US_IER_OFFSET               0x08         /**< \brief (US_IER offset) Interrupt Enable Register */
#define US_IER_RESETVALUE           0x00000000   /**< \brief (US_IER reset_value) Interrupt Enable Register */

// LIN mode
#define US_IER_LIN_RXRDY_Pos        0            /**< \brief (US_IER_LIN) Receiver Ready Interrupt Enable */
#define US_IER_LIN_RXRDY            (0x1u << US_IER_LIN_RXRDY_Pos)
#define   US_IER_LIN_RXRDY_0        (0x0u <<  0) /**< \brief (US_IER_LIN) No Effect */
#define   US_IER_LIN_RXRDY_1        (0x1u <<  0) /**< \brief (US_IER_LIN) Enables the interrupt */
#define US_IER_LIN_TXRDY_Pos        1            /**< \brief (US_IER_LIN) Transmitter Ready Interrupt Enable */
#define US_IER_LIN_TXRDY            (0x1u << US_IER_LIN_TXRDY_Pos)
#define   US_IER_LIN_TXRDY_0        (0x0u <<  1) /**< \brief (US_IER_LIN) No Effect */
#define   US_IER_LIN_TXRDY_1        (0x1u <<  1) /**< \brief (US_IER_LIN) Enables the interrupt */
#define US_IER_LIN_RXBRK_Pos        2            /**< \brief (US_IER_LIN) Receiver Break Interrupt Enable */
#define US_IER_LIN_RXBRK            (0x1u << US_IER_LIN_RXBRK_Pos)
#define   US_IER_LIN_RXBRK_0        (0x0u <<  2) /**< \brief (US_IER_LIN) No Effect */
#define   US_IER_LIN_RXBRK_1        (0x1u <<  2) /**< \brief (US_IER_LIN) Enables the interrupt */
#define US_IER_LIN_ENDRX_Pos        3            /**< \brief (US_IER_LIN) End of Receive Transfer Interrupt Enable */
#define US_IER_LIN_ENDRX            (0x1u << US_IER_LIN_ENDRX_Pos)
#define   US_IER_LIN_ENDRX_0        (0x0u <<  3) /**< \brief (US_IER_LIN) No Effect */
#define   US_IER_LIN_ENDRX_1        (0x1u <<  3) /**< \brief (US_IER_LIN) Enables the interrupt */
#define US_IER_LIN_ENDTX_Pos        4            /**< \brief (US_IER_LIN) End of Transmit Interrupt Enable */
#define US_IER_LIN_ENDTX            (0x1u << US_IER_LIN_ENDTX_Pos)
#define   US_IER_LIN_ENDTX_0        (0x0u <<  4) /**< \brief (US_IER_LIN) No Effect */
#define   US_IER_LIN_ENDTX_1        (0x1u <<  4) /**< \brief (US_IER_LIN) Enables the interrupt */
#define US_IER_LIN_OVRE_Pos         5            /**< \brief (US_IER_LIN) Overrun Error Interrupt Enable */
#define US_IER_LIN_OVRE             (0x1u << US_IER_LIN_OVRE_Pos)
#define   US_IER_LIN_OVRE_0         (0x0u <<  5) /**< \brief (US_IER_LIN) No Effect */
#define   US_IER_LIN_OVRE_1         (0x1u <<  5) /**< \brief (US_IER_LIN) Enables the interrupt */
#define US_IER_LIN_FRAME_Pos        6            /**< \brief (US_IER_LIN) Framing Error Interrupt Enable */
#define US_IER_LIN_FRAME            (0x1u << US_IER_LIN_FRAME_Pos)
#define   US_IER_LIN_FRAME_0        (0x0u <<  6) /**< \brief (US_IER_LIN) No Effect */
#define   US_IER_LIN_FRAME_1        (0x1u <<  6) /**< \brief (US_IER_LIN) Enables the interrupt */
#define US_IER_LIN_PARE_Pos         7            /**< \brief (US_IER_LIN) Parity Error Interrupt Enable */
#define US_IER_LIN_PARE             (0x1u << US_IER_LIN_PARE_Pos)
#define   US_IER_LIN_PARE_0         (0x0u <<  7) /**< \brief (US_IER_LIN) No Effect */
#define   US_IER_LIN_PARE_1         (0x1u <<  7) /**< \brief (US_IER_LIN) Enables the interrupt */
#define US_IER_LIN_TIMEOUT_Pos      8            /**< \brief (US_IER_LIN) Time-out Interrupt Enable */
#define US_IER_LIN_TIMEOUT          (0x1u << US_IER_LIN_TIMEOUT_Pos)
#define   US_IER_LIN_TIMEOUT_0      (0x0u <<  8) /**< \brief (US_IER_LIN) No Effect */
#define   US_IER_LIN_TIMEOUT_1      (0x1u <<  8) /**< \brief (US_IER_LIN) Enables the interrupt */
#define US_IER_LIN_TXEMPTY_Pos      9            /**< \brief (US_IER_LIN) Transmitter Empty Interrupt Enable */
#define US_IER_LIN_TXEMPTY          (0x1u << US_IER_LIN_TXEMPTY_Pos)
#define   US_IER_LIN_TXEMPTY_0      (0x0u <<  9) /**< \brief (US_IER_LIN) No Effect */
#define   US_IER_LIN_TXEMPTY_1      (0x1u <<  9) /**< \brief (US_IER_LIN) Enables the interrupt */
#define US_IER_LIN_ITER_Pos         10           /**< \brief (US_IER_LIN) Iteration Interrupt Enable */
#define US_IER_LIN_ITER             (0x1u << US_IER_LIN_ITER_Pos)
#define   US_IER_LIN_ITER_0         (0x0u << 10) /**< \brief (US_IER_LIN) No Effect */
#define   US_IER_LIN_ITER_1         (0x1u << 10) /**< \brief (US_IER_LIN) Enables the interrupt */
#define US_IER_LIN_TXBUFE_Pos       11           /**< \brief (US_IER_LIN) Buffer Empty Interrupt Enable */
#define US_IER_LIN_TXBUFE           (0x1u << US_IER_LIN_TXBUFE_Pos)
#define   US_IER_LIN_TXBUFE_0       (0x0u << 11) /**< \brief (US_IER_LIN) No Effect */
#define   US_IER_LIN_TXBUFE_1       (0x1u << 11) /**< \brief (US_IER_LIN) Enables the interrupt */
#define US_IER_LIN_RXBUFF_Pos       12           /**< \brief (US_IER_LIN) Buffer Full Interrupt Enable */
#define US_IER_LIN_RXBUFF           (0x1u << US_IER_LIN_RXBUFF_Pos)
#define   US_IER_LIN_RXBUFF_0       (0x0u << 12) /**< \brief (US_IER_LIN) No Effect */
#define   US_IER_LIN_RXBUFF_1       (0x1u << 12) /**< \brief (US_IER_LIN) Enables the interrupt */
#define US_IER_LIN_NACK_Pos         13           /**< \brief (US_IER_LIN) Non Acknowledge  or LIN Break Sent or LIN Break Received Interrupt Enable */
#define US_IER_LIN_NACK             (0x1u << US_IER_LIN_NACK_Pos)
#define   US_IER_LIN_NACK_0         (0x0u << 13) /**< \brief (US_IER_LIN) No Effect */
#define   US_IER_LIN_NACK_1         (0x1u << 13) /**< \brief (US_IER_LIN) Enables the interrupt */
#define US_IER_LIN_LINID_Pos        14           /**< \brief (US_IER_LIN) LIN Identifier Sent or LIN Identifier Received Interrupt Enable */
#define US_IER_LIN_LINID            (0x1u << US_IER_LIN_LINID_Pos)
#define US_IER_LIN_LINTC_Pos        15           /**< \brief (US_IER_LIN) LIN Transfer Conpleted Interrupt Enable */
#define US_IER_LIN_LINTC            (0x1u << US_IER_LIN_LINTC_Pos)
#define US_IER_LIN_RIIC_Pos         16           /**< \brief (US_IER_LIN) Ring Indicator Input Change Enable */
#define US_IER_LIN_RIIC             (0x1u << US_IER_LIN_RIIC_Pos)
#define   US_IER_LIN_RIIC_0         (0x0u << 16) /**< \brief (US_IER_LIN) No Effect */
#define   US_IER_LIN_RIIC_1         (0x1u << 16) /**< \brief (US_IER_LIN) Enables the interrupt */
#define US_IER_LIN_DSRIC_Pos        17           /**< \brief (US_IER_LIN) Data Set Ready Input Change Enable */
#define US_IER_LIN_DSRIC            (0x1u << US_IER_LIN_DSRIC_Pos)
#define   US_IER_LIN_DSRIC_0        (0x0u << 17) /**< \brief (US_IER_LIN) No Effect */
#define   US_IER_LIN_DSRIC_1        (0x1u << 17) /**< \brief (US_IER_LIN) Enables the interrupt */
#define US_IER_LIN_DCDIC_Pos        18           /**< \brief (US_IER_LIN) Data Carrier Detect Input Change Interrupt Enable */
#define US_IER_LIN_DCDIC            (0x1u << US_IER_LIN_DCDIC_Pos)
#define   US_IER_LIN_DCDIC_0        (0x0u << 18) /**< \brief (US_IER_LIN) No Effect */
#define   US_IER_LIN_DCDIC_1        (0x1u << 18) /**< \brief (US_IER_LIN) Enables the interrupt */
#define US_IER_LIN_CTSIC_Pos        19           /**< \brief (US_IER_LIN) Clear to Send Input Change Interrupt Enable */
#define US_IER_LIN_CTSIC            (0x1u << US_IER_LIN_CTSIC_Pos)
#define   US_IER_LIN_CTSIC_0        (0x0u << 19) /**< \brief (US_IER_LIN) No Effect */
#define   US_IER_LIN_CTSIC_1        (0x1u << 19) /**< \brief (US_IER_LIN) Enables the interrupt */
#define US_IER_LIN_LINBE_Pos        25           /**< \brief (US_IER_LIN) LIN Bus Error Interrupt Enable */
#define US_IER_LIN_LINBE            (0x1u << US_IER_LIN_LINBE_Pos)
#define US_IER_LIN_LINISFE_Pos      26           /**< \brief (US_IER_LIN) LIN Inconsistent Synch Field Error Interrupt Enable */
#define US_IER_LIN_LINISFE          (0x1u << US_IER_LIN_LINISFE_Pos)
#define US_IER_LIN_LINIPE_Pos       27           /**< \brief (US_IER_LIN) LIN Identifier Parity Interrupt Enable */
#define US_IER_LIN_LINIPE           (0x1u << US_IER_LIN_LINIPE_Pos)
#define US_IER_LIN_LINCE_Pos        28           /**< \brief (US_IER_LIN) LIN Checksum Error Interrupt Enable */
#define US_IER_LIN_LINCE            (0x1u << US_IER_LIN_LINCE_Pos)
#define US_IER_LIN_LINSNRE_Pos      29           /**< \brief (US_IER_LIN) LIN Slave Not Responding Error Interrupt Enable */
#define US_IER_LIN_LINSNRE          (0x1u << US_IER_LIN_LINSNRE_Pos)
#define US_IER_LIN_LINSTE_Pos       30           /**< \brief (US_IER_LIN) LIN Synch Tolerance Error Interrupt Enable */
#define US_IER_LIN_LINSTE           (0x1u << US_IER_LIN_LINSTE_Pos)
#define   US_IER_LIN_LINSTE_0       (0x0u << 30) /**< \brief (US_IER_LIN) No Effect */
#define   US_IER_LIN_LINSTE_1       (0x1u << 30) /**< \brief (US_IER_LIN) Enables the interrupt */
#define US_IER_LIN_LINHTE_Pos       31           /**< \brief (US_IER_LIN) LIN Header Timeout Error Interrupt Enable */
#define US_IER_LIN_LINHTE           (0x1u << US_IER_LIN_LINHTE_Pos)
#define   US_IER_LIN_LINHTE_0       (0x0u << 31) /**< \brief (US_IER_LIN) No Effect */
#define   US_IER_LIN_LINHTE_1       (0x1u << 31) /**< \brief (US_IER_LIN) Enables the interrupt */
#define US_IER_LIN_MASK             0xFE0FFFFFu  /**< \brief (US_IER_LIN) MASK Register */

// SPI_SLAVE mode
#define US_IER_SPI_SLAVE_RXRDY_Pos  0            /**< \brief (US_IER_SPI_SLAVE) Receiver Ready Interrupt Enable */
#define US_IER_SPI_SLAVE_RXRDY      (0x1u << US_IER_SPI_SLAVE_RXRDY_Pos)
#define   US_IER_SPI_SLAVE_RXRDY_0  (0x0u <<  0) /**< \brief (US_IER_SPI_SLAVE) No Effect */
#define   US_IER_SPI_SLAVE_RXRDY_1  (0x1u <<  0) /**< \brief (US_IER_SPI_SLAVE) Enables the interrupt */
#define US_IER_SPI_SLAVE_TXRDY_Pos  1            /**< \brief (US_IER_SPI_SLAVE) Transmitter Ready Interrupt Enable */
#define US_IER_SPI_SLAVE_TXRDY      (0x1u << US_IER_SPI_SLAVE_TXRDY_Pos)
#define   US_IER_SPI_SLAVE_TXRDY_0  (0x0u <<  1) /**< \brief (US_IER_SPI_SLAVE) No Effect */
#define   US_IER_SPI_SLAVE_TXRDY_1  (0x1u <<  1) /**< \brief (US_IER_SPI_SLAVE) Enables the interrupt */
#define US_IER_SPI_SLAVE_RXBRK_Pos  2            /**< \brief (US_IER_SPI_SLAVE) Receiver Break Interrupt Enable */
#define US_IER_SPI_SLAVE_RXBRK      (0x1u << US_IER_SPI_SLAVE_RXBRK_Pos)
#define   US_IER_SPI_SLAVE_RXBRK_0  (0x0u <<  2) /**< \brief (US_IER_SPI_SLAVE) No Effect */
#define   US_IER_SPI_SLAVE_RXBRK_1  (0x1u <<  2) /**< \brief (US_IER_SPI_SLAVE) Enables the interrupt */
#define US_IER_SPI_SLAVE_ENDRX_Pos  3            /**< \brief (US_IER_SPI_SLAVE) End of Receive Transfer Interrupt Enable */
#define US_IER_SPI_SLAVE_ENDRX      (0x1u << US_IER_SPI_SLAVE_ENDRX_Pos)
#define   US_IER_SPI_SLAVE_ENDRX_0  (0x0u <<  3) /**< \brief (US_IER_SPI_SLAVE) No Effect */
#define   US_IER_SPI_SLAVE_ENDRX_1  (0x1u <<  3) /**< \brief (US_IER_SPI_SLAVE) Enables the interrupt */
#define US_IER_SPI_SLAVE_ENDTX_Pos  4            /**< \brief (US_IER_SPI_SLAVE) End of Transmit Interrupt Enable */
#define US_IER_SPI_SLAVE_ENDTX      (0x1u << US_IER_SPI_SLAVE_ENDTX_Pos)
#define   US_IER_SPI_SLAVE_ENDTX_0  (0x0u <<  4) /**< \brief (US_IER_SPI_SLAVE) No Effect */
#define   US_IER_SPI_SLAVE_ENDTX_1  (0x1u <<  4) /**< \brief (US_IER_SPI_SLAVE) Enables the interrupt */
#define US_IER_SPI_SLAVE_OVRE_Pos   5            /**< \brief (US_IER_SPI_SLAVE) Overrun Error Interrupt Enable */
#define US_IER_SPI_SLAVE_OVRE       (0x1u << US_IER_SPI_SLAVE_OVRE_Pos)
#define   US_IER_SPI_SLAVE_OVRE_0   (0x0u <<  5) /**< \brief (US_IER_SPI_SLAVE) No Effect */
#define   US_IER_SPI_SLAVE_OVRE_1   (0x1u <<  5) /**< \brief (US_IER_SPI_SLAVE) Enables the interrupt */
#define US_IER_SPI_SLAVE_FRAME_Pos  6            /**< \brief (US_IER_SPI_SLAVE) Framing Error Interrupt Enable */
#define US_IER_SPI_SLAVE_FRAME      (0x1u << US_IER_SPI_SLAVE_FRAME_Pos)
#define   US_IER_SPI_SLAVE_FRAME_0  (0x0u <<  6) /**< \brief (US_IER_SPI_SLAVE) No Effect */
#define   US_IER_SPI_SLAVE_FRAME_1  (0x1u <<  6) /**< \brief (US_IER_SPI_SLAVE) Enables the interrupt */
#define US_IER_SPI_SLAVE_PARE_Pos   7            /**< \brief (US_IER_SPI_SLAVE) Parity Error Interrupt Enable */
#define US_IER_SPI_SLAVE_PARE       (0x1u << US_IER_SPI_SLAVE_PARE_Pos)
#define   US_IER_SPI_SLAVE_PARE_0   (0x0u <<  7) /**< \brief (US_IER_SPI_SLAVE) No Effect */
#define   US_IER_SPI_SLAVE_PARE_1   (0x1u <<  7) /**< \brief (US_IER_SPI_SLAVE) Enables the interrupt */
#define US_IER_SPI_SLAVE_TIMEOUT_Pos 8            /**< \brief (US_IER_SPI_SLAVE) Time-out Interrupt Enable */
#define US_IER_SPI_SLAVE_TIMEOUT    (0x1u << US_IER_SPI_SLAVE_TIMEOUT_Pos)
#define   US_IER_SPI_SLAVE_TIMEOUT_0 (0x0u <<  8) /**< \brief (US_IER_SPI_SLAVE) No Effect */
#define   US_IER_SPI_SLAVE_TIMEOUT_1 (0x1u <<  8) /**< \brief (US_IER_SPI_SLAVE) Enables the interrupt */
#define US_IER_SPI_SLAVE_TXEMPTY_Pos 9            /**< \brief (US_IER_SPI_SLAVE) Transmitter Empty Interrupt Enable */
#define US_IER_SPI_SLAVE_TXEMPTY    (0x1u << US_IER_SPI_SLAVE_TXEMPTY_Pos)
#define   US_IER_SPI_SLAVE_TXEMPTY_0 (0x0u <<  9) /**< \brief (US_IER_SPI_SLAVE) No Effect */
#define   US_IER_SPI_SLAVE_TXEMPTY_1 (0x1u <<  9) /**< \brief (US_IER_SPI_SLAVE) Enables the interrupt */
#define US_IER_SPI_SLAVE_UNRE_Pos   10           /**< \brief (US_IER_SPI_SLAVE) SPI Underrun Error Interrupt Enable */
#define US_IER_SPI_SLAVE_UNRE       (0x1u << US_IER_SPI_SLAVE_UNRE_Pos)
#define   US_IER_SPI_SLAVE_UNRE_0   (0x0u << 10) /**< \brief (US_IER_SPI_SLAVE) No Effect */
#define   US_IER_SPI_SLAVE_UNRE_1   (0x1u << 10) /**< \brief (US_IER_SPI_SLAVE) Enables the interrupt */
#define US_IER_SPI_SLAVE_TXBUFE_Pos 11           /**< \brief (US_IER_SPI_SLAVE) Buffer Empty Interrupt Enable */
#define US_IER_SPI_SLAVE_TXBUFE     (0x1u << US_IER_SPI_SLAVE_TXBUFE_Pos)
#define   US_IER_SPI_SLAVE_TXBUFE_0 (0x0u << 11) /**< \brief (US_IER_SPI_SLAVE) No Effect */
#define   US_IER_SPI_SLAVE_TXBUFE_1 (0x1u << 11) /**< \brief (US_IER_SPI_SLAVE) Enables the interrupt */
#define US_IER_SPI_SLAVE_RXBUFF_Pos 12           /**< \brief (US_IER_SPI_SLAVE) Buffer Full Interrupt Enable */
#define US_IER_SPI_SLAVE_RXBUFF     (0x1u << US_IER_SPI_SLAVE_RXBUFF_Pos)
#define   US_IER_SPI_SLAVE_RXBUFF_0 (0x0u << 12) /**< \brief (US_IER_SPI_SLAVE) No Effect */
#define   US_IER_SPI_SLAVE_RXBUFF_1 (0x1u << 12) /**< \brief (US_IER_SPI_SLAVE) Enables the interrupt */
#define US_IER_SPI_SLAVE_NACK_Pos   13           /**< \brief (US_IER_SPI_SLAVE) Non Acknowledge Interrupt Enable */
#define US_IER_SPI_SLAVE_NACK       (0x1u << US_IER_SPI_SLAVE_NACK_Pos)
#define   US_IER_SPI_SLAVE_NACK_0   (0x0u << 13) /**< \brief (US_IER_SPI_SLAVE) No Effect */
#define   US_IER_SPI_SLAVE_NACK_1   (0x1u << 13) /**< \brief (US_IER_SPI_SLAVE) Enables the interrupt */
#define US_IER_SPI_SLAVE_RIIC_Pos   16           /**< \brief (US_IER_SPI_SLAVE) Ring Indicator Input Change Enable */
#define US_IER_SPI_SLAVE_RIIC       (0x1u << US_IER_SPI_SLAVE_RIIC_Pos)
#define   US_IER_SPI_SLAVE_RIIC_0   (0x0u << 16) /**< \brief (US_IER_SPI_SLAVE) No Effect */
#define   US_IER_SPI_SLAVE_RIIC_1   (0x1u << 16) /**< \brief (US_IER_SPI_SLAVE) Enables the interrupt */
#define US_IER_SPI_SLAVE_DSRIC_Pos  17           /**< \brief (US_IER_SPI_SLAVE) Data Set Ready Input Change Enable */
#define US_IER_SPI_SLAVE_DSRIC      (0x1u << US_IER_SPI_SLAVE_DSRIC_Pos)
#define   US_IER_SPI_SLAVE_DSRIC_0  (0x0u << 17) /**< \brief (US_IER_SPI_SLAVE) No Effect */
#define   US_IER_SPI_SLAVE_DSRIC_1  (0x1u << 17) /**< \brief (US_IER_SPI_SLAVE) Enables the interrupt */
#define US_IER_SPI_SLAVE_DCDIC_Pos  18           /**< \brief (US_IER_SPI_SLAVE) Data Carrier Detect Input Change Interrupt Enable */
#define US_IER_SPI_SLAVE_DCDIC      (0x1u << US_IER_SPI_SLAVE_DCDIC_Pos)
#define   US_IER_SPI_SLAVE_DCDIC_0  (0x0u << 18) /**< \brief (US_IER_SPI_SLAVE) No Effect */
#define   US_IER_SPI_SLAVE_DCDIC_1  (0x1u << 18) /**< \brief (US_IER_SPI_SLAVE) Enables the interrupt */
#define US_IER_SPI_SLAVE_CTSIC_Pos  19           /**< \brief (US_IER_SPI_SLAVE) Clear to Send Input Change Interrupt Enable */
#define US_IER_SPI_SLAVE_CTSIC      (0x1u << US_IER_SPI_SLAVE_CTSIC_Pos)
#define   US_IER_SPI_SLAVE_CTSIC_0  (0x0u << 19) /**< \brief (US_IER_SPI_SLAVE) No Effect */
#define   US_IER_SPI_SLAVE_CTSIC_1  (0x1u << 19) /**< \brief (US_IER_SPI_SLAVE) Enables the interrupt */
#define US_IER_SPI_SLAVE_MASK       0x000F3FFFu  /**< \brief (US_IER_SPI_SLAVE) MASK Register */

// USART mode
#define US_IER_USART_RXRDY_Pos      0            /**< \brief (US_IER_USART) Receiver Ready Interrupt Enable */
#define US_IER_USART_RXRDY          (0x1u << US_IER_USART_RXRDY_Pos)
#define   US_IER_USART_RXRDY_0      (0x0u <<  0) /**< \brief (US_IER_USART) No Effect */
#define   US_IER_USART_RXRDY_1      (0x1u <<  0) /**< \brief (US_IER_USART) Enables the interrupt */
#define US_IER_USART_TXRDY_Pos      1            /**< \brief (US_IER_USART) Transmitter Ready Interrupt Enable */
#define US_IER_USART_TXRDY          (0x1u << US_IER_USART_TXRDY_Pos)
#define   US_IER_USART_TXRDY_0      (0x0u <<  1) /**< \brief (US_IER_USART) No Effect */
#define   US_IER_USART_TXRDY_1      (0x1u <<  1) /**< \brief (US_IER_USART) Enables the interrupt */
#define US_IER_USART_RXBRK_Pos      2            /**< \brief (US_IER_USART) Receiver Break Interrupt Enable */
#define US_IER_USART_RXBRK          (0x1u << US_IER_USART_RXBRK_Pos)
#define   US_IER_USART_RXBRK_0      (0x0u <<  2) /**< \brief (US_IER_USART) No Effect */
#define   US_IER_USART_RXBRK_1      (0x1u <<  2) /**< \brief (US_IER_USART) Enables the interrupt */
#define US_IER_USART_ENDRX_Pos      3            /**< \brief (US_IER_USART) End of Receive Transfer Interrupt Enable */
#define US_IER_USART_ENDRX          (0x1u << US_IER_USART_ENDRX_Pos)
#define   US_IER_USART_ENDRX_0      (0x0u <<  3) /**< \brief (US_IER_USART) No Effect */
#define   US_IER_USART_ENDRX_1      (0x1u <<  3) /**< \brief (US_IER_USART) Enables the interrupt */
#define US_IER_USART_ENDTX_Pos      4            /**< \brief (US_IER_USART) End of Transmit Interrupt Enable */
#define US_IER_USART_ENDTX          (0x1u << US_IER_USART_ENDTX_Pos)
#define   US_IER_USART_ENDTX_0      (0x0u <<  4) /**< \brief (US_IER_USART) No Effect */
#define   US_IER_USART_ENDTX_1      (0x1u <<  4) /**< \brief (US_IER_USART) Enables the interrupt */
#define US_IER_USART_OVRE_Pos       5            /**< \brief (US_IER_USART) Overrun Error Interrupt Enable */
#define US_IER_USART_OVRE           (0x1u << US_IER_USART_OVRE_Pos)
#define   US_IER_USART_OVRE_0       (0x0u <<  5) /**< \brief (US_IER_USART) No Effect */
#define   US_IER_USART_OVRE_1       (0x1u <<  5) /**< \brief (US_IER_USART) Enables the interrupt */
#define US_IER_USART_FRAME_Pos      6            /**< \brief (US_IER_USART) Framing Error Interrupt Enable */
#define US_IER_USART_FRAME          (0x1u << US_IER_USART_FRAME_Pos)
#define   US_IER_USART_FRAME_0      (0x0u <<  6) /**< \brief (US_IER_USART) No Effect */
#define   US_IER_USART_FRAME_1      (0x1u <<  6) /**< \brief (US_IER_USART) Enables the interrupt */
#define US_IER_USART_PARE_Pos       7            /**< \brief (US_IER_USART) Parity Error Interrupt Enable */
#define US_IER_USART_PARE           (0x1u << US_IER_USART_PARE_Pos)
#define   US_IER_USART_PARE_0       (0x0u <<  7) /**< \brief (US_IER_USART) No Effect */
#define   US_IER_USART_PARE_1       (0x1u <<  7) /**< \brief (US_IER_USART) Enables the interrupt */
#define US_IER_USART_TIMEOUT_Pos    8            /**< \brief (US_IER_USART) Time-out Interrupt Enable */
#define US_IER_USART_TIMEOUT        (0x1u << US_IER_USART_TIMEOUT_Pos)
#define   US_IER_USART_TIMEOUT_0    (0x0u <<  8) /**< \brief (US_IER_USART) No Effect */
#define   US_IER_USART_TIMEOUT_1    (0x1u <<  8) /**< \brief (US_IER_USART) Enables the interrupt */
#define US_IER_USART_TXEMPTY_Pos    9            /**< \brief (US_IER_USART) Transmitter Empty Interrupt Enable */
#define US_IER_USART_TXEMPTY        (0x1u << US_IER_USART_TXEMPTY_Pos)
#define   US_IER_USART_TXEMPTY_0    (0x0u <<  9) /**< \brief (US_IER_USART) No Effect */
#define   US_IER_USART_TXEMPTY_1    (0x1u <<  9) /**< \brief (US_IER_USART) Enables the interrupt */
#define US_IER_USART_ITER_Pos       10           /**< \brief (US_IER_USART) Iteration Interrupt Enable */
#define US_IER_USART_ITER           (0x1u << US_IER_USART_ITER_Pos)
#define   US_IER_USART_ITER_0       (0x0u << 10) /**< \brief (US_IER_USART) No Effect */
#define   US_IER_USART_ITER_1       (0x1u << 10) /**< \brief (US_IER_USART) Enables the interrupt */
#define US_IER_USART_TXBUFE_Pos     11           /**< \brief (US_IER_USART) Buffer Empty Interrupt Enable */
#define US_IER_USART_TXBUFE         (0x1u << US_IER_USART_TXBUFE_Pos)
#define   US_IER_USART_TXBUFE_0     (0x0u << 11) /**< \brief (US_IER_USART) No Effect */
#define   US_IER_USART_TXBUFE_1     (0x1u << 11) /**< \brief (US_IER_USART) Enables the interrupt */
#define US_IER_USART_RXBUFF_Pos     12           /**< \brief (US_IER_USART) Buffer Full Interrupt Enable */
#define US_IER_USART_RXBUFF         (0x1u << US_IER_USART_RXBUFF_Pos)
#define   US_IER_USART_RXBUFF_0     (0x0u << 12) /**< \brief (US_IER_USART) No Effect */
#define   US_IER_USART_RXBUFF_1     (0x1u << 12) /**< \brief (US_IER_USART) Enables the interrupt */
#define US_IER_USART_NACK_Pos       13           /**< \brief (US_IER_USART) Non Acknowledge Interrupt Enable */
#define US_IER_USART_NACK           (0x1u << US_IER_USART_NACK_Pos)
#define   US_IER_USART_NACK_0       (0x0u << 13) /**< \brief (US_IER_USART) No Effect */
#define   US_IER_USART_NACK_1       (0x1u << 13) /**< \brief (US_IER_USART) Enables the interrupt */
#define US_IER_USART_RIIC_Pos       16           /**< \brief (US_IER_USART) Ring Indicator Input Change Enable */
#define US_IER_USART_RIIC           (0x1u << US_IER_USART_RIIC_Pos)
#define   US_IER_USART_RIIC_0       (0x0u << 16) /**< \brief (US_IER_USART) No Effect */
#define   US_IER_USART_RIIC_1       (0x1u << 16) /**< \brief (US_IER_USART) Enables the interrupt */
#define US_IER_USART_DSRIC_Pos      17           /**< \brief (US_IER_USART) Data Set Ready Input Change Enable */
#define US_IER_USART_DSRIC          (0x1u << US_IER_USART_DSRIC_Pos)
#define   US_IER_USART_DSRIC_0      (0x0u << 17) /**< \brief (US_IER_USART) No Effect */
#define   US_IER_USART_DSRIC_1      (0x1u << 17) /**< \brief (US_IER_USART) Enables the interrupt */
#define US_IER_USART_DCDIC_Pos      18           /**< \brief (US_IER_USART) Data Carrier Detect Input Change Interrupt Enable */
#define US_IER_USART_DCDIC          (0x1u << US_IER_USART_DCDIC_Pos)
#define   US_IER_USART_DCDIC_0      (0x0u << 18) /**< \brief (US_IER_USART) No Effect */
#define   US_IER_USART_DCDIC_1      (0x1u << 18) /**< \brief (US_IER_USART) Enables the interrupt */
#define US_IER_USART_CTSIC_Pos      19           /**< \brief (US_IER_USART) Clear to Send Input Change Interrupt Enable */
#define US_IER_USART_CTSIC          (0x1u << US_IER_USART_CTSIC_Pos)
#define   US_IER_USART_CTSIC_0      (0x0u << 19) /**< \brief (US_IER_USART) No Effect */
#define   US_IER_USART_CTSIC_1      (0x1u << 19) /**< \brief (US_IER_USART) Enables the interrupt */
#define US_IER_USART_MANE_Pos       20           /**< \brief (US_IER_USART) Manchester Error Interrupt Enable */
#define US_IER_USART_MANE           (0x1u << US_IER_USART_MANE_Pos)
#define US_IER_USART_MANEA_Pos      24           /**< \brief (US_IER_USART) Manchester Error Interrupt Enable */
#define US_IER_USART_MANEA          (0x1u << US_IER_USART_MANEA_Pos)
#define   US_IER_USART_MANEA_0      (0x0u << 24) /**< \brief (US_IER_USART) No effect */
#define   US_IER_USART_MANEA_1      (0x1u << 24) /**< \brief (US_IER_USART) Enables the interrupt */
#define US_IER_USART_MASK           0x011F3FFFu  /**< \brief (US_IER_USART) MASK Register */

// Any mode
#define US_IER_RXRDY_Pos            0            /**< \brief (US_IER) Receiver Ready Interrupt Enable */
#define US_IER_RXRDY                (0x1u << US_IER_RXRDY_Pos)
#define   US_IER_RXRDY_0            (0x0u <<  0) /**< \brief (US_IER) No Effect */
#define   US_IER_RXRDY_1            (0x1u <<  0) /**< \brief (US_IER) Enables the interrupt */
#define US_IER_TXRDY_Pos            1            /**< \brief (US_IER) Transmitter Ready Interrupt Enable */
#define US_IER_TXRDY                (0x1u << US_IER_TXRDY_Pos)
#define   US_IER_TXRDY_0            (0x0u <<  1) /**< \brief (US_IER) No Effect */
#define   US_IER_TXRDY_1            (0x1u <<  1) /**< \brief (US_IER) Enables the interrupt */
#define US_IER_RXBRK_Pos            2            /**< \brief (US_IER) Receiver Break Interrupt Enable */
#define US_IER_RXBRK                (0x1u << US_IER_RXBRK_Pos)
#define   US_IER_RXBRK_0            (0x0u <<  2) /**< \brief (US_IER) No Effect */
#define   US_IER_RXBRK_1            (0x1u <<  2) /**< \brief (US_IER) Enables the interrupt */
#define US_IER_ENDRX_Pos            3            /**< \brief (US_IER) End of Receive Transfer Interrupt Enable */
#define US_IER_ENDRX                (0x1u << US_IER_ENDRX_Pos)
#define   US_IER_ENDRX_0            (0x0u <<  3) /**< \brief (US_IER) No Effect */
#define   US_IER_ENDRX_1            (0x1u <<  3) /**< \brief (US_IER) Enables the interrupt */
#define US_IER_ENDTX_Pos            4            /**< \brief (US_IER) End of Transmit Interrupt Enable */
#define US_IER_ENDTX                (0x1u << US_IER_ENDTX_Pos)
#define   US_IER_ENDTX_0            (0x0u <<  4) /**< \brief (US_IER) No Effect */
#define   US_IER_ENDTX_1            (0x1u <<  4) /**< \brief (US_IER) Enables the interrupt */
#define US_IER_OVRE_Pos             5            /**< \brief (US_IER) Overrun Error Interrupt Enable */
#define US_IER_OVRE                 (0x1u << US_IER_OVRE_Pos)
#define   US_IER_OVRE_0             (0x0u <<  5) /**< \brief (US_IER) No Effect */
#define   US_IER_OVRE_1             (0x1u <<  5) /**< \brief (US_IER) Enables the interrupt */
#define US_IER_FRAME_Pos            6            /**< \brief (US_IER) Framing Error Interrupt Enable */
#define US_IER_FRAME                (0x1u << US_IER_FRAME_Pos)
#define   US_IER_FRAME_0            (0x0u <<  6) /**< \brief (US_IER) No Effect */
#define   US_IER_FRAME_1            (0x1u <<  6) /**< \brief (US_IER) Enables the interrupt */
#define US_IER_PARE_Pos             7            /**< \brief (US_IER) Parity Error Interrupt Enable */
#define US_IER_PARE                 (0x1u << US_IER_PARE_Pos)
#define   US_IER_PARE_0             (0x0u <<  7) /**< \brief (US_IER) No Effect */
#define   US_IER_PARE_1             (0x1u <<  7) /**< \brief (US_IER) Enables the interrupt */
#define US_IER_TIMEOUT_Pos          8            /**< \brief (US_IER) Time-out Interrupt Enable */
#define US_IER_TIMEOUT              (0x1u << US_IER_TIMEOUT_Pos)
#define   US_IER_TIMEOUT_0          (0x0u <<  8) /**< \brief (US_IER) No Effect */
#define   US_IER_TIMEOUT_1          (0x1u <<  8) /**< \brief (US_IER) Enables the interrupt */
#define US_IER_TXEMPTY_Pos          9            /**< \brief (US_IER) Transmitter Empty Interrupt Enable */
#define US_IER_TXEMPTY              (0x1u << US_IER_TXEMPTY_Pos)
#define   US_IER_TXEMPTY_0          (0x0u <<  9) /**< \brief (US_IER) No Effect */
#define   US_IER_TXEMPTY_1          (0x1u <<  9) /**< \brief (US_IER) Enables the interrupt */
#define US_IER_ITER_Pos             10           /**< \brief (US_IER) Iteration Interrupt Enable */
#define US_IER_ITER                 (0x1u << US_IER_ITER_Pos)
#define   US_IER_ITER_0             (0x0u << 10) /**< \brief (US_IER) No Effect */
#define   US_IER_ITER_1             (0x1u << 10) /**< \brief (US_IER) Enables the interrupt */
#define US_IER_UNRE_Pos             10           /**< \brief (US_IER) SPI Underrun Error Interrupt Enable */
#define US_IER_UNRE                 (0x1u << US_IER_UNRE_Pos)
#define   US_IER_UNRE_0             (0x0u << 10) /**< \brief (US_IER) No Effect */
#define   US_IER_UNRE_1             (0x1u << 10) /**< \brief (US_IER) Enables the interrupt */
#define US_IER_ITER_Pos             10           /**< \brief (US_IER) Iteration Interrupt Enable */
#define US_IER_ITER                 (0x1u << US_IER_ITER_Pos)
#define   US_IER_ITER_0             (0x0u << 10) /**< \brief (US_IER) No Effect */
#define   US_IER_ITER_1             (0x1u << 10) /**< \brief (US_IER) Enables the interrupt */
#define US_IER_TXBUFE_Pos           11           /**< \brief (US_IER) Buffer Empty Interrupt Enable */
#define US_IER_TXBUFE               (0x1u << US_IER_TXBUFE_Pos)
#define   US_IER_TXBUFE_0           (0x0u << 11) /**< \brief (US_IER) No Effect */
#define   US_IER_TXBUFE_1           (0x1u << 11) /**< \brief (US_IER) Enables the interrupt */
#define US_IER_RXBUFF_Pos           12           /**< \brief (US_IER) Buffer Full Interrupt Enable */
#define US_IER_RXBUFF               (0x1u << US_IER_RXBUFF_Pos)
#define   US_IER_RXBUFF_0           (0x0u << 12) /**< \brief (US_IER) No Effect */
#define   US_IER_RXBUFF_1           (0x1u << 12) /**< \brief (US_IER) Enables the interrupt */
#define US_IER_NACK_Pos             13           /**< \brief (US_IER) Non Acknowledge  or LIN Break Sent or LIN Break Received Interrupt Enable */
#define US_IER_NACK                 (0x1u << US_IER_NACK_Pos)
#define   US_IER_NACK_0             (0x0u << 13) /**< \brief (US_IER) No Effect */
#define   US_IER_NACK_1             (0x1u << 13) /**< \brief (US_IER) Enables the interrupt */
#define US_IER_LINID_Pos            14           /**< \brief (US_IER) LIN Identifier Sent or LIN Identifier Received Interrupt Enable */
#define US_IER_LINID                (0x1u << US_IER_LINID_Pos)
#define US_IER_LINTC_Pos            15           /**< \brief (US_IER) LIN Transfer Conpleted Interrupt Enable */
#define US_IER_LINTC                (0x1u << US_IER_LINTC_Pos)
#define US_IER_RIIC_Pos             16           /**< \brief (US_IER) Ring Indicator Input Change Enable */
#define US_IER_RIIC                 (0x1u << US_IER_RIIC_Pos)
#define   US_IER_RIIC_0             (0x0u << 16) /**< \brief (US_IER) No Effect */
#define   US_IER_RIIC_1             (0x1u << 16) /**< \brief (US_IER) Enables the interrupt */
#define US_IER_DSRIC_Pos            17           /**< \brief (US_IER) Data Set Ready Input Change Enable */
#define US_IER_DSRIC                (0x1u << US_IER_DSRIC_Pos)
#define   US_IER_DSRIC_0            (0x0u << 17) /**< \brief (US_IER) No Effect */
#define   US_IER_DSRIC_1            (0x1u << 17) /**< \brief (US_IER) Enables the interrupt */
#define US_IER_DCDIC_Pos            18           /**< \brief (US_IER) Data Carrier Detect Input Change Interrupt Enable */
#define US_IER_DCDIC                (0x1u << US_IER_DCDIC_Pos)
#define   US_IER_DCDIC_0            (0x0u << 18) /**< \brief (US_IER) No Effect */
#define   US_IER_DCDIC_1            (0x1u << 18) /**< \brief (US_IER) Enables the interrupt */
#define US_IER_CTSIC_Pos            19           /**< \brief (US_IER) Clear to Send Input Change Interrupt Enable */
#define US_IER_CTSIC                (0x1u << US_IER_CTSIC_Pos)
#define   US_IER_CTSIC_0            (0x0u << 19) /**< \brief (US_IER) No Effect */
#define   US_IER_CTSIC_1            (0x1u << 19) /**< \brief (US_IER) Enables the interrupt */
#define US_IER_MANE_Pos             20           /**< \brief (US_IER) Manchester Error Interrupt Enable */
#define US_IER_MANE                 (0x1u << US_IER_MANE_Pos)
#define US_IER_MANEA_Pos            24           /**< \brief (US_IER) Manchester Error Interrupt Enable */
#define US_IER_MANEA                (0x1u << US_IER_MANEA_Pos)
#define   US_IER_MANEA_0            (0x0u << 24) /**< \brief (US_IER) No effect */
#define   US_IER_MANEA_1            (0x1u << 24) /**< \brief (US_IER) Enables the interrupt */
#define US_IER_LINBE_Pos            25           /**< \brief (US_IER) LIN Bus Error Interrupt Enable */
#define US_IER_LINBE                (0x1u << US_IER_LINBE_Pos)
#define US_IER_LINISFE_Pos          26           /**< \brief (US_IER) LIN Inconsistent Synch Field Error Interrupt Enable */
#define US_IER_LINISFE              (0x1u << US_IER_LINISFE_Pos)
#define US_IER_LINIPE_Pos           27           /**< \brief (US_IER) LIN Identifier Parity Interrupt Enable */
#define US_IER_LINIPE               (0x1u << US_IER_LINIPE_Pos)
#define US_IER_LINCE_Pos            28           /**< \brief (US_IER) LIN Checksum Error Interrupt Enable */
#define US_IER_LINCE                (0x1u << US_IER_LINCE_Pos)
#define US_IER_LINSNRE_Pos          29           /**< \brief (US_IER) LIN Slave Not Responding Error Interrupt Enable */
#define US_IER_LINSNRE              (0x1u << US_IER_LINSNRE_Pos)
#define US_IER_LINSTE_Pos           30           /**< \brief (US_IER) LIN Synch Tolerance Error Interrupt Enable */
#define US_IER_LINSTE               (0x1u << US_IER_LINSTE_Pos)
#define   US_IER_LINSTE_0           (0x0u << 30) /**< \brief (US_IER) No Effect */
#define   US_IER_LINSTE_1           (0x1u << 30) /**< \brief (US_IER) Enables the interrupt */
#define US_IER_LINHTE_Pos           31           /**< \brief (US_IER) LIN Header Timeout Error Interrupt Enable */
#define US_IER_LINHTE               (0x1u << US_IER_LINHTE_Pos)
#define   US_IER_LINHTE_0           (0x0u << 31) /**< \brief (US_IER) No Effect */
#define   US_IER_LINHTE_1           (0x1u << 31) /**< \brief (US_IER) Enables the interrupt */
#define US_IER_MASK                 0xFF1FFFFFu  /**< \brief (US_IER) MASK Register */

/* -------- US_IDR : (USART Offset: 0x0C) ( /W 32) Interrupt Disable Register -------- */
#define US_IDR_OFFSET               0x0C         /**< \brief (US_IDR offset) Interrupt Disable Register */
#define US_IDR_RESETVALUE           0x00000000   /**< \brief (US_IDR reset_value) Interrupt Disable Register */

// LIN mode
#define US_IDR_LIN_RXRDY_Pos        0            /**< \brief (US_IDR_LIN) Receiver Ready Interrupt Disable */
#define US_IDR_LIN_RXRDY            (0x1u << US_IDR_LIN_RXRDY_Pos)
#define   US_IDR_LIN_RXRDY_0        (0x0u <<  0) /**< \brief (US_IDR_LIN) No Effect */
#define   US_IDR_LIN_RXRDY_1        (0x1u <<  0) /**< \brief (US_IDR_LIN) Disables the interrupt */
#define US_IDR_LIN_TXRDY_Pos        1            /**< \brief (US_IDR_LIN) Transmitter Ready Interrupt Disable */
#define US_IDR_LIN_TXRDY            (0x1u << US_IDR_LIN_TXRDY_Pos)
#define   US_IDR_LIN_TXRDY_0        (0x0u <<  1) /**< \brief (US_IDR_LIN) No Effect */
#define   US_IDR_LIN_TXRDY_1        (0x1u <<  1) /**< \brief (US_IDR_LIN) Disables the interrupt */
#define US_IDR_LIN_RXBRK_Pos        2            /**< \brief (US_IDR_LIN) Receiver Break Interrupt Disable */
#define US_IDR_LIN_RXBRK            (0x1u << US_IDR_LIN_RXBRK_Pos)
#define   US_IDR_LIN_RXBRK_0        (0x0u <<  2) /**< \brief (US_IDR_LIN) No Effect */
#define   US_IDR_LIN_RXBRK_1        (0x1u <<  2) /**< \brief (US_IDR_LIN) Disables the interrupt */
#define US_IDR_LIN_ENDRX_Pos        3            /**< \brief (US_IDR_LIN) End of Receive Transfer Interrupt Disable */
#define US_IDR_LIN_ENDRX            (0x1u << US_IDR_LIN_ENDRX_Pos)
#define   US_IDR_LIN_ENDRX_0        (0x0u <<  3) /**< \brief (US_IDR_LIN) No Effect */
#define   US_IDR_LIN_ENDRX_1        (0x1u <<  3) /**< \brief (US_IDR_LIN) Disables the interrupt */
#define US_IDR_LIN_ENDTX_Pos        4            /**< \brief (US_IDR_LIN) End of Transmit Interrupt Disable */
#define US_IDR_LIN_ENDTX            (0x1u << US_IDR_LIN_ENDTX_Pos)
#define   US_IDR_LIN_ENDTX_0        (0x0u <<  4) /**< \brief (US_IDR_LIN) No Effect */
#define   US_IDR_LIN_ENDTX_1        (0x1u <<  4) /**< \brief (US_IDR_LIN) Disables the interrupt */
#define US_IDR_LIN_OVRE_Pos         5            /**< \brief (US_IDR_LIN) Overrun Error Interrupt Disable */
#define US_IDR_LIN_OVRE             (0x1u << US_IDR_LIN_OVRE_Pos)
#define   US_IDR_LIN_OVRE_0         (0x0u <<  5) /**< \brief (US_IDR_LIN) No Effect */
#define   US_IDR_LIN_OVRE_1         (0x1u <<  5) /**< \brief (US_IDR_LIN) Disables the interrupt */
#define US_IDR_LIN_FRAME_Pos        6            /**< \brief (US_IDR_LIN) Framing Error Interrupt Disable */
#define US_IDR_LIN_FRAME            (0x1u << US_IDR_LIN_FRAME_Pos)
#define   US_IDR_LIN_FRAME_0        (0x0u <<  6) /**< \brief (US_IDR_LIN) No Effect */
#define   US_IDR_LIN_FRAME_1        (0x1u <<  6) /**< \brief (US_IDR_LIN) Disables the interrupt */
#define US_IDR_LIN_PARE_Pos         7            /**< \brief (US_IDR_LIN) Parity Error Interrupt Disable */
#define US_IDR_LIN_PARE             (0x1u << US_IDR_LIN_PARE_Pos)
#define   US_IDR_LIN_PARE_0         (0x0u <<  7) /**< \brief (US_IDR_LIN) No Effect */
#define   US_IDR_LIN_PARE_1         (0x1u <<  7) /**< \brief (US_IDR_LIN) Disables the interrupt */
#define US_IDR_LIN_TIMEOUT_Pos      8            /**< \brief (US_IDR_LIN) Time-out Interrupt Disable */
#define US_IDR_LIN_TIMEOUT          (0x1u << US_IDR_LIN_TIMEOUT_Pos)
#define   US_IDR_LIN_TIMEOUT_0      (0x0u <<  8) /**< \brief (US_IDR_LIN) No Effect */
#define   US_IDR_LIN_TIMEOUT_1      (0x1u <<  8) /**< \brief (US_IDR_LIN) Disables the interrupt */
#define US_IDR_LIN_TXEMPTY_Pos      9            /**< \brief (US_IDR_LIN) Transmitter Empty Interrupt Disable */
#define US_IDR_LIN_TXEMPTY          (0x1u << US_IDR_LIN_TXEMPTY_Pos)
#define   US_IDR_LIN_TXEMPTY_0      (0x0u <<  9) /**< \brief (US_IDR_LIN) No Effect */
#define   US_IDR_LIN_TXEMPTY_1      (0x1u <<  9) /**< \brief (US_IDR_LIN) Disables the interrupt */
#define US_IDR_LIN_ITER_Pos         10           /**< \brief (US_IDR_LIN) Iteration Interrupt Disable */
#define US_IDR_LIN_ITER             (0x1u << US_IDR_LIN_ITER_Pos)
#define   US_IDR_LIN_ITER_0         (0x0u << 10) /**< \brief (US_IDR_LIN) No Effect */
#define   US_IDR_LIN_ITER_1         (0x1u << 10) /**< \brief (US_IDR_LIN) Disables the interrupt */
#define US_IDR_LIN_TXBUFE_Pos       11           /**< \brief (US_IDR_LIN) Buffer Empty Interrupt Disable */
#define US_IDR_LIN_TXBUFE           (0x1u << US_IDR_LIN_TXBUFE_Pos)
#define   US_IDR_LIN_TXBUFE_0       (0x0u << 11) /**< \brief (US_IDR_LIN) No Effect */
#define   US_IDR_LIN_TXBUFE_1       (0x1u << 11) /**< \brief (US_IDR_LIN) Disables the interrupt */
#define US_IDR_LIN_RXBUFF_Pos       12           /**< \brief (US_IDR_LIN) Buffer Full Interrupt Disable */
#define US_IDR_LIN_RXBUFF           (0x1u << US_IDR_LIN_RXBUFF_Pos)
#define   US_IDR_LIN_RXBUFF_0       (0x0u << 12) /**< \brief (US_IDR_LIN) No Effect */
#define   US_IDR_LIN_RXBUFF_1       (0x1u << 12) /**< \brief (US_IDR_LIN) Disables the interrupt */
#define US_IDR_LIN_NACK_Pos         13           /**< \brief (US_IDR_LIN) Non Acknowledge  or LIN Break Sent or LIN Break Received Interrupt Disable */
#define US_IDR_LIN_NACK             (0x1u << US_IDR_LIN_NACK_Pos)
#define   US_IDR_LIN_NACK_0         (0x0u << 13) /**< \brief (US_IDR_LIN) No Effect */
#define   US_IDR_LIN_NACK_1         (0x1u << 13) /**< \brief (US_IDR_LIN) Disables the interrupt */
#define US_IDR_LIN_LINID_Pos        14           /**< \brief (US_IDR_LIN) LIN Identifier Sent or LIN Identifier Received Interrupt Disable */
#define US_IDR_LIN_LINID            (0x1u << US_IDR_LIN_LINID_Pos)
#define US_IDR_LIN_LINTC_Pos        15           /**< \brief (US_IDR_LIN) LIN Transfer Conpleted Interrupt Disable */
#define US_IDR_LIN_LINTC            (0x1u << US_IDR_LIN_LINTC_Pos)
#define US_IDR_LIN_RIIC_Pos         16           /**< \brief (US_IDR_LIN) Ring Indicator Input Change Disable */
#define US_IDR_LIN_RIIC             (0x1u << US_IDR_LIN_RIIC_Pos)
#define   US_IDR_LIN_RIIC_0         (0x0u << 16) /**< \brief (US_IDR_LIN) No Effect */
#define   US_IDR_LIN_RIIC_1         (0x1u << 16) /**< \brief (US_IDR_LIN) Disables the interrupt */
#define US_IDR_LIN_DSRIC_Pos        17           /**< \brief (US_IDR_LIN) Data Set Ready Input Change Disable */
#define US_IDR_LIN_DSRIC            (0x1u << US_IDR_LIN_DSRIC_Pos)
#define   US_IDR_LIN_DSRIC_0        (0x0u << 17) /**< \brief (US_IDR_LIN) No Effect */
#define   US_IDR_LIN_DSRIC_1        (0x1u << 17) /**< \brief (US_IDR_LIN) Disables the interrupt */
#define US_IDR_LIN_DCDIC_Pos        18           /**< \brief (US_IDR_LIN) Data Carrier Detect Input Change Interrupt Disable */
#define US_IDR_LIN_DCDIC            (0x1u << US_IDR_LIN_DCDIC_Pos)
#define   US_IDR_LIN_DCDIC_0        (0x0u << 18) /**< \brief (US_IDR_LIN) No Effect */
#define   US_IDR_LIN_DCDIC_1        (0x1u << 18) /**< \brief (US_IDR_LIN) Disables the interrupt */
#define US_IDR_LIN_CTSIC_Pos        19           /**< \brief (US_IDR_LIN) Clear to Send Input Change Interrupt Disable */
#define US_IDR_LIN_CTSIC            (0x1u << US_IDR_LIN_CTSIC_Pos)
#define   US_IDR_LIN_CTSIC_0        (0x0u << 19) /**< \brief (US_IDR_LIN) No Effect */
#define   US_IDR_LIN_CTSIC_1        (0x1u << 19) /**< \brief (US_IDR_LIN) Disables the interrupt */
#define US_IDR_LIN_LINBE_Pos        25           /**< \brief (US_IDR_LIN) LIN Bus Error Interrupt Disable */
#define US_IDR_LIN_LINBE            (0x1u << US_IDR_LIN_LINBE_Pos)
#define US_IDR_LIN_LINISFE_Pos      26           /**< \brief (US_IDR_LIN) LIN Inconsistent Synch Field Error Interrupt Disable */
#define US_IDR_LIN_LINISFE          (0x1u << US_IDR_LIN_LINISFE_Pos)
#define US_IDR_LIN_LINIPE_Pos       27           /**< \brief (US_IDR_LIN) LIN Identifier Parity Interrupt Disable */
#define US_IDR_LIN_LINIPE           (0x1u << US_IDR_LIN_LINIPE_Pos)
#define US_IDR_LIN_LINCE_Pos        28           /**< \brief (US_IDR_LIN) LIN Checksum Error Interrupt Disable */
#define US_IDR_LIN_LINCE            (0x1u << US_IDR_LIN_LINCE_Pos)
#define US_IDR_LIN_LINSNRE_Pos      29           /**< \brief (US_IDR_LIN) LIN Slave Not Responding Error Interrupt Disable */
#define US_IDR_LIN_LINSNRE          (0x1u << US_IDR_LIN_LINSNRE_Pos)
#define US_IDR_LIN_LINSTE_Pos       30           /**< \brief (US_IDR_LIN) LIN Synch Tolerance Error Interrupt Disable */
#define US_IDR_LIN_LINSTE           (0x1u << US_IDR_LIN_LINSTE_Pos)
#define   US_IDR_LIN_LINSTE_0       (0x0u << 30) /**< \brief (US_IDR_LIN) No Effect */
#define   US_IDR_LIN_LINSTE_1       (0x1u << 30) /**< \brief (US_IDR_LIN) Disables the interrupt */
#define US_IDR_LIN_LINHTE_Pos       31           /**< \brief (US_IDR_LIN) LIN Header Timeout Error Interrupt Disable */
#define US_IDR_LIN_LINHTE           (0x1u << US_IDR_LIN_LINHTE_Pos)
#define   US_IDR_LIN_LINHTE_0       (0x0u << 31) /**< \brief (US_IDR_LIN) No Effect */
#define   US_IDR_LIN_LINHTE_1       (0x1u << 31) /**< \brief (US_IDR_LIN) Disables the interrupt */
#define US_IDR_LIN_MASK             0xFE0FFFFFu  /**< \brief (US_IDR_LIN) MASK Register */

// SPI_SLAVE mode
#define US_IDR_SPI_SLAVE_RXRDY_Pos  0            /**< \brief (US_IDR_SPI_SLAVE) Receiver Ready Interrupt Disable */
#define US_IDR_SPI_SLAVE_RXRDY      (0x1u << US_IDR_SPI_SLAVE_RXRDY_Pos)
#define   US_IDR_SPI_SLAVE_RXRDY_0  (0x0u <<  0) /**< \brief (US_IDR_SPI_SLAVE) No Effect */
#define   US_IDR_SPI_SLAVE_RXRDY_1  (0x1u <<  0) /**< \brief (US_IDR_SPI_SLAVE) Disables the interrupt */
#define US_IDR_SPI_SLAVE_TXRDY_Pos  1            /**< \brief (US_IDR_SPI_SLAVE) Transmitter Ready Interrupt Disable */
#define US_IDR_SPI_SLAVE_TXRDY      (0x1u << US_IDR_SPI_SLAVE_TXRDY_Pos)
#define   US_IDR_SPI_SLAVE_TXRDY_0  (0x0u <<  1) /**< \brief (US_IDR_SPI_SLAVE) No Effect */
#define   US_IDR_SPI_SLAVE_TXRDY_1  (0x1u <<  1) /**< \brief (US_IDR_SPI_SLAVE) Disables the interrupt */
#define US_IDR_SPI_SLAVE_RXBRK_Pos  2            /**< \brief (US_IDR_SPI_SLAVE) Receiver Break Interrupt Disable */
#define US_IDR_SPI_SLAVE_RXBRK      (0x1u << US_IDR_SPI_SLAVE_RXBRK_Pos)
#define   US_IDR_SPI_SLAVE_RXBRK_0  (0x0u <<  2) /**< \brief (US_IDR_SPI_SLAVE) No Effect */
#define   US_IDR_SPI_SLAVE_RXBRK_1  (0x1u <<  2) /**< \brief (US_IDR_SPI_SLAVE) Disables the interrupt */
#define US_IDR_SPI_SLAVE_ENDRX_Pos  3            /**< \brief (US_IDR_SPI_SLAVE) End of Receive Transfer Interrupt Disable */
#define US_IDR_SPI_SLAVE_ENDRX      (0x1u << US_IDR_SPI_SLAVE_ENDRX_Pos)
#define   US_IDR_SPI_SLAVE_ENDRX_0  (0x0u <<  3) /**< \brief (US_IDR_SPI_SLAVE) No Effect */
#define   US_IDR_SPI_SLAVE_ENDRX_1  (0x1u <<  3) /**< \brief (US_IDR_SPI_SLAVE) Disables the interrupt */
#define US_IDR_SPI_SLAVE_ENDTX_Pos  4            /**< \brief (US_IDR_SPI_SLAVE) End of Transmit Interrupt Disable */
#define US_IDR_SPI_SLAVE_ENDTX      (0x1u << US_IDR_SPI_SLAVE_ENDTX_Pos)
#define   US_IDR_SPI_SLAVE_ENDTX_0  (0x0u <<  4) /**< \brief (US_IDR_SPI_SLAVE) No Effect */
#define   US_IDR_SPI_SLAVE_ENDTX_1  (0x1u <<  4) /**< \brief (US_IDR_SPI_SLAVE) Disables the interrupt */
#define US_IDR_SPI_SLAVE_OVRE_Pos   5            /**< \brief (US_IDR_SPI_SLAVE) Overrun Error Interrupt Disable */
#define US_IDR_SPI_SLAVE_OVRE       (0x1u << US_IDR_SPI_SLAVE_OVRE_Pos)
#define   US_IDR_SPI_SLAVE_OVRE_0   (0x0u <<  5) /**< \brief (US_IDR_SPI_SLAVE) No Effect */
#define   US_IDR_SPI_SLAVE_OVRE_1   (0x1u <<  5) /**< \brief (US_IDR_SPI_SLAVE) Disables the interrupt */
#define US_IDR_SPI_SLAVE_FRAME_Pos  6            /**< \brief (US_IDR_SPI_SLAVE) Framing Error Interrupt Disable */
#define US_IDR_SPI_SLAVE_FRAME      (0x1u << US_IDR_SPI_SLAVE_FRAME_Pos)
#define   US_IDR_SPI_SLAVE_FRAME_0  (0x0u <<  6) /**< \brief (US_IDR_SPI_SLAVE) No Effect */
#define   US_IDR_SPI_SLAVE_FRAME_1  (0x1u <<  6) /**< \brief (US_IDR_SPI_SLAVE) Disables the interrupt */
#define US_IDR_SPI_SLAVE_PARE_Pos   7            /**< \brief (US_IDR_SPI_SLAVE) Parity Error Interrupt Disable */
#define US_IDR_SPI_SLAVE_PARE       (0x1u << US_IDR_SPI_SLAVE_PARE_Pos)
#define   US_IDR_SPI_SLAVE_PARE_0   (0x0u <<  7) /**< \brief (US_IDR_SPI_SLAVE) No Effect */
#define   US_IDR_SPI_SLAVE_PARE_1   (0x1u <<  7) /**< \brief (US_IDR_SPI_SLAVE) Disables the interrupt */
#define US_IDR_SPI_SLAVE_TIMEOUT_Pos 8            /**< \brief (US_IDR_SPI_SLAVE) Time-out Interrupt Disable */
#define US_IDR_SPI_SLAVE_TIMEOUT    (0x1u << US_IDR_SPI_SLAVE_TIMEOUT_Pos)
#define   US_IDR_SPI_SLAVE_TIMEOUT_0 (0x0u <<  8) /**< \brief (US_IDR_SPI_SLAVE) No Effect */
#define   US_IDR_SPI_SLAVE_TIMEOUT_1 (0x1u <<  8) /**< \brief (US_IDR_SPI_SLAVE) Disables the interrupt */
#define US_IDR_SPI_SLAVE_TXEMPTY_Pos 9            /**< \brief (US_IDR_SPI_SLAVE) Transmitter Empty Interrupt Disable */
#define US_IDR_SPI_SLAVE_TXEMPTY    (0x1u << US_IDR_SPI_SLAVE_TXEMPTY_Pos)
#define   US_IDR_SPI_SLAVE_TXEMPTY_0 (0x0u <<  9) /**< \brief (US_IDR_SPI_SLAVE) No Effect */
#define   US_IDR_SPI_SLAVE_TXEMPTY_1 (0x1u <<  9) /**< \brief (US_IDR_SPI_SLAVE) Disables the interrupt */
#define US_IDR_SPI_SLAVE_UNRE_Pos   10           /**< \brief (US_IDR_SPI_SLAVE) SPI Underrun Error Interrupt Disable */
#define US_IDR_SPI_SLAVE_UNRE       (0x1u << US_IDR_SPI_SLAVE_UNRE_Pos)
#define   US_IDR_SPI_SLAVE_UNRE_0   (0x0u << 10) /**< \brief (US_IDR_SPI_SLAVE) No Effect */
#define   US_IDR_SPI_SLAVE_UNRE_1   (0x1u << 10) /**< \brief (US_IDR_SPI_SLAVE) Disables the interrupt */
#define US_IDR_SPI_SLAVE_TXBUFE_Pos 11           /**< \brief (US_IDR_SPI_SLAVE) Buffer Empty Interrupt Disable */
#define US_IDR_SPI_SLAVE_TXBUFE     (0x1u << US_IDR_SPI_SLAVE_TXBUFE_Pos)
#define   US_IDR_SPI_SLAVE_TXBUFE_0 (0x0u << 11) /**< \brief (US_IDR_SPI_SLAVE) No Effect */
#define   US_IDR_SPI_SLAVE_TXBUFE_1 (0x1u << 11) /**< \brief (US_IDR_SPI_SLAVE) Disables the interrupt */
#define US_IDR_SPI_SLAVE_RXBUFF_Pos 12           /**< \brief (US_IDR_SPI_SLAVE) Buffer Full Interrupt Disable */
#define US_IDR_SPI_SLAVE_RXBUFF     (0x1u << US_IDR_SPI_SLAVE_RXBUFF_Pos)
#define   US_IDR_SPI_SLAVE_RXBUFF_0 (0x0u << 12) /**< \brief (US_IDR_SPI_SLAVE) No Effect */
#define   US_IDR_SPI_SLAVE_RXBUFF_1 (0x1u << 12) /**< \brief (US_IDR_SPI_SLAVE) Disables the interrupt */
#define US_IDR_SPI_SLAVE_NACK_Pos   13           /**< \brief (US_IDR_SPI_SLAVE) Non Acknowledge Interrupt Disable */
#define US_IDR_SPI_SLAVE_NACK       (0x1u << US_IDR_SPI_SLAVE_NACK_Pos)
#define   US_IDR_SPI_SLAVE_NACK_0   (0x0u << 13) /**< \brief (US_IDR_SPI_SLAVE) No Effect */
#define   US_IDR_SPI_SLAVE_NACK_1   (0x1u << 13) /**< \brief (US_IDR_SPI_SLAVE) Disables the interrupt */
#define US_IDR_SPI_SLAVE_RIIC_Pos   16           /**< \brief (US_IDR_SPI_SLAVE) Ring Indicator Input Change Disable */
#define US_IDR_SPI_SLAVE_RIIC       (0x1u << US_IDR_SPI_SLAVE_RIIC_Pos)
#define   US_IDR_SPI_SLAVE_RIIC_0   (0x0u << 16) /**< \brief (US_IDR_SPI_SLAVE) No Effect */
#define   US_IDR_SPI_SLAVE_RIIC_1   (0x1u << 16) /**< \brief (US_IDR_SPI_SLAVE) Disables the interrupt */
#define US_IDR_SPI_SLAVE_DSRIC_Pos  17           /**< \brief (US_IDR_SPI_SLAVE) Data Set Ready Input Change Disable */
#define US_IDR_SPI_SLAVE_DSRIC      (0x1u << US_IDR_SPI_SLAVE_DSRIC_Pos)
#define   US_IDR_SPI_SLAVE_DSRIC_0  (0x0u << 17) /**< \brief (US_IDR_SPI_SLAVE) No Effect */
#define   US_IDR_SPI_SLAVE_DSRIC_1  (0x1u << 17) /**< \brief (US_IDR_SPI_SLAVE) Disables the interrupt */
#define US_IDR_SPI_SLAVE_DCDIC_Pos  18           /**< \brief (US_IDR_SPI_SLAVE) Data Carrier Detect Input Change Interrupt Disable */
#define US_IDR_SPI_SLAVE_DCDIC      (0x1u << US_IDR_SPI_SLAVE_DCDIC_Pos)
#define   US_IDR_SPI_SLAVE_DCDIC_0  (0x0u << 18) /**< \brief (US_IDR_SPI_SLAVE) No Effect */
#define   US_IDR_SPI_SLAVE_DCDIC_1  (0x1u << 18) /**< \brief (US_IDR_SPI_SLAVE) Disables the interrupt */
#define US_IDR_SPI_SLAVE_CTSIC_Pos  19           /**< \brief (US_IDR_SPI_SLAVE) Clear to Send Input Change Interrupt Disable */
#define US_IDR_SPI_SLAVE_CTSIC      (0x1u << US_IDR_SPI_SLAVE_CTSIC_Pos)
#define   US_IDR_SPI_SLAVE_CTSIC_0  (0x0u << 19) /**< \brief (US_IDR_SPI_SLAVE) No Effect */
#define   US_IDR_SPI_SLAVE_CTSIC_1  (0x1u << 19) /**< \brief (US_IDR_SPI_SLAVE) Disables the interrupt */
#define US_IDR_SPI_SLAVE_MASK       0x000F3FFFu  /**< \brief (US_IDR_SPI_SLAVE) MASK Register */

// USART mode
#define US_IDR_USART_RXRDY_Pos      0            /**< \brief (US_IDR_USART) Receiver Ready Interrupt Disable */
#define US_IDR_USART_RXRDY          (0x1u << US_IDR_USART_RXRDY_Pos)
#define   US_IDR_USART_RXRDY_0      (0x0u <<  0) /**< \brief (US_IDR_USART) No Effect */
#define   US_IDR_USART_RXRDY_1      (0x1u <<  0) /**< \brief (US_IDR_USART) Disables the interrupt */
#define US_IDR_USART_TXRDY_Pos      1            /**< \brief (US_IDR_USART) Transmitter Ready Interrupt Disable */
#define US_IDR_USART_TXRDY          (0x1u << US_IDR_USART_TXRDY_Pos)
#define   US_IDR_USART_TXRDY_0      (0x0u <<  1) /**< \brief (US_IDR_USART) No Effect */
#define   US_IDR_USART_TXRDY_1      (0x1u <<  1) /**< \brief (US_IDR_USART) Disables the interrupt */
#define US_IDR_USART_RXBRK_Pos      2            /**< \brief (US_IDR_USART) Receiver Break Interrupt Disable */
#define US_IDR_USART_RXBRK          (0x1u << US_IDR_USART_RXBRK_Pos)
#define   US_IDR_USART_RXBRK_0      (0x0u <<  2) /**< \brief (US_IDR_USART) No Effect */
#define   US_IDR_USART_RXBRK_1      (0x1u <<  2) /**< \brief (US_IDR_USART) Disables the interrupt */
#define US_IDR_USART_ENDRX_Pos      3            /**< \brief (US_IDR_USART) End of Receive Transfer Interrupt Disable */
#define US_IDR_USART_ENDRX          (0x1u << US_IDR_USART_ENDRX_Pos)
#define   US_IDR_USART_ENDRX_0      (0x0u <<  3) /**< \brief (US_IDR_USART) No Effect */
#define   US_IDR_USART_ENDRX_1      (0x1u <<  3) /**< \brief (US_IDR_USART) Disables the interrupt */
#define US_IDR_USART_ENDTX_Pos      4            /**< \brief (US_IDR_USART) End of Transmit Interrupt Disable */
#define US_IDR_USART_ENDTX          (0x1u << US_IDR_USART_ENDTX_Pos)
#define   US_IDR_USART_ENDTX_0      (0x0u <<  4) /**< \brief (US_IDR_USART) No Effect */
#define   US_IDR_USART_ENDTX_1      (0x1u <<  4) /**< \brief (US_IDR_USART) Disables the interrupt */
#define US_IDR_USART_OVRE_Pos       5            /**< \brief (US_IDR_USART) Overrun Error Interrupt Disable */
#define US_IDR_USART_OVRE           (0x1u << US_IDR_USART_OVRE_Pos)
#define   US_IDR_USART_OVRE_0       (0x0u <<  5) /**< \brief (US_IDR_USART) No Effect */
#define   US_IDR_USART_OVRE_1       (0x1u <<  5) /**< \brief (US_IDR_USART) Disables the interrupt */
#define US_IDR_USART_FRAME_Pos      6            /**< \brief (US_IDR_USART) Framing Error Interrupt Disable */
#define US_IDR_USART_FRAME          (0x1u << US_IDR_USART_FRAME_Pos)
#define   US_IDR_USART_FRAME_0      (0x0u <<  6) /**< \brief (US_IDR_USART) No Effect */
#define   US_IDR_USART_FRAME_1      (0x1u <<  6) /**< \brief (US_IDR_USART) Disables the interrupt */
#define US_IDR_USART_PARE_Pos       7            /**< \brief (US_IDR_USART) Parity Error Interrupt Disable */
#define US_IDR_USART_PARE           (0x1u << US_IDR_USART_PARE_Pos)
#define   US_IDR_USART_PARE_0       (0x0u <<  7) /**< \brief (US_IDR_USART) No Effect */
#define   US_IDR_USART_PARE_1       (0x1u <<  7) /**< \brief (US_IDR_USART) Disables the interrupt */
#define US_IDR_USART_TIMEOUT_Pos    8            /**< \brief (US_IDR_USART) Time-out Interrupt Disable */
#define US_IDR_USART_TIMEOUT        (0x1u << US_IDR_USART_TIMEOUT_Pos)
#define   US_IDR_USART_TIMEOUT_0    (0x0u <<  8) /**< \brief (US_IDR_USART) No Effect */
#define   US_IDR_USART_TIMEOUT_1    (0x1u <<  8) /**< \brief (US_IDR_USART) Disables the interrupt */
#define US_IDR_USART_TXEMPTY_Pos    9            /**< \brief (US_IDR_USART) Transmitter Empty Interrupt Disable */
#define US_IDR_USART_TXEMPTY        (0x1u << US_IDR_USART_TXEMPTY_Pos)
#define   US_IDR_USART_TXEMPTY_0    (0x0u <<  9) /**< \brief (US_IDR_USART) No Effect */
#define   US_IDR_USART_TXEMPTY_1    (0x1u <<  9) /**< \brief (US_IDR_USART) Disables the interrupt */
#define US_IDR_USART_ITER_Pos       10           /**< \brief (US_IDR_USART) Iteration Interrupt Disable */
#define US_IDR_USART_ITER           (0x1u << US_IDR_USART_ITER_Pos)
#define   US_IDR_USART_ITER_0       (0x0u << 10) /**< \brief (US_IDR_USART) No Effect */
#define   US_IDR_USART_ITER_1       (0x1u << 10) /**< \brief (US_IDR_USART) Disables the interrupt */
#define US_IDR_USART_TXBUFE_Pos     11           /**< \brief (US_IDR_USART) Buffer Empty Interrupt Disable */
#define US_IDR_USART_TXBUFE         (0x1u << US_IDR_USART_TXBUFE_Pos)
#define   US_IDR_USART_TXBUFE_0     (0x0u << 11) /**< \brief (US_IDR_USART) No Effect */
#define   US_IDR_USART_TXBUFE_1     (0x1u << 11) /**< \brief (US_IDR_USART) Disables the interrupt */
#define US_IDR_USART_RXBUFF_Pos     12           /**< \brief (US_IDR_USART) Buffer Full Interrupt Disable */
#define US_IDR_USART_RXBUFF         (0x1u << US_IDR_USART_RXBUFF_Pos)
#define   US_IDR_USART_RXBUFF_0     (0x0u << 12) /**< \brief (US_IDR_USART) No Effect */
#define   US_IDR_USART_RXBUFF_1     (0x1u << 12) /**< \brief (US_IDR_USART) Disables the interrupt */
#define US_IDR_USART_NACK_Pos       13           /**< \brief (US_IDR_USART) Non Acknowledge Interrupt Disable */
#define US_IDR_USART_NACK           (0x1u << US_IDR_USART_NACK_Pos)
#define   US_IDR_USART_NACK_0       (0x0u << 13) /**< \brief (US_IDR_USART) No Effect */
#define   US_IDR_USART_NACK_1       (0x1u << 13) /**< \brief (US_IDR_USART) Disables the interrupt */
#define US_IDR_USART_RIIC_Pos       16           /**< \brief (US_IDR_USART) Ring Indicator Input Change Disable */
#define US_IDR_USART_RIIC           (0x1u << US_IDR_USART_RIIC_Pos)
#define   US_IDR_USART_RIIC_0       (0x0u << 16) /**< \brief (US_IDR_USART) No Effect */
#define   US_IDR_USART_RIIC_1       (0x1u << 16) /**< \brief (US_IDR_USART) Disables the interrupt */
#define US_IDR_USART_DSRIC_Pos      17           /**< \brief (US_IDR_USART) Data Set Ready Input Change Disable */
#define US_IDR_USART_DSRIC          (0x1u << US_IDR_USART_DSRIC_Pos)
#define   US_IDR_USART_DSRIC_0      (0x0u << 17) /**< \brief (US_IDR_USART) No Effect */
#define   US_IDR_USART_DSRIC_1      (0x1u << 17) /**< \brief (US_IDR_USART) Disables the interrupt */
#define US_IDR_USART_DCDIC_Pos      18           /**< \brief (US_IDR_USART) Data Carrier Detect Input Change Interrupt Disable */
#define US_IDR_USART_DCDIC          (0x1u << US_IDR_USART_DCDIC_Pos)
#define   US_IDR_USART_DCDIC_0      (0x0u << 18) /**< \brief (US_IDR_USART) No Effect */
#define   US_IDR_USART_DCDIC_1      (0x1u << 18) /**< \brief (US_IDR_USART) Disables the interrupt */
#define US_IDR_USART_CTSIC_Pos      19           /**< \brief (US_IDR_USART) Clear to Send Input Change Interrupt Disable */
#define US_IDR_USART_CTSIC          (0x1u << US_IDR_USART_CTSIC_Pos)
#define   US_IDR_USART_CTSIC_0      (0x0u << 19) /**< \brief (US_IDR_USART) No Effect */
#define   US_IDR_USART_CTSIC_1      (0x1u << 19) /**< \brief (US_IDR_USART) Disables the interrupt */
#define US_IDR_USART_MANE_Pos       20           /**< \brief (US_IDR_USART) Manchester Error Interrupt Disable */
#define US_IDR_USART_MANE           (0x1u << US_IDR_USART_MANE_Pos)
#define US_IDR_USART_MANEA_Pos      24           /**< \brief (US_IDR_USART) Manchester Error Interrupt Disable */
#define US_IDR_USART_MANEA          (0x1u << US_IDR_USART_MANEA_Pos)
#define   US_IDR_USART_MANEA_0      (0x0u << 24) /**< \brief (US_IDR_USART) No effect */
#define   US_IDR_USART_MANEA_1      (0x1u << 24) /**< \brief (US_IDR_USART) Disables the corresponding interrupt */
#define US_IDR_USART_MASK           0x011F3FFFu  /**< \brief (US_IDR_USART) MASK Register */

// Any mode
#define US_IDR_RXRDY_Pos            0            /**< \brief (US_IDR) Receiver Ready Interrupt Disable */
#define US_IDR_RXRDY                (0x1u << US_IDR_RXRDY_Pos)
#define   US_IDR_RXRDY_0            (0x0u <<  0) /**< \brief (US_IDR) No Effect */
#define   US_IDR_RXRDY_1            (0x1u <<  0) /**< \brief (US_IDR) Disables the interrupt */
#define US_IDR_TXRDY_Pos            1            /**< \brief (US_IDR) Transmitter Ready Interrupt Disable */
#define US_IDR_TXRDY                (0x1u << US_IDR_TXRDY_Pos)
#define   US_IDR_TXRDY_0            (0x0u <<  1) /**< \brief (US_IDR) No Effect */
#define   US_IDR_TXRDY_1            (0x1u <<  1) /**< \brief (US_IDR) Disables the interrupt */
#define US_IDR_RXBRK_Pos            2            /**< \brief (US_IDR) Receiver Break Interrupt Disable */
#define US_IDR_RXBRK                (0x1u << US_IDR_RXBRK_Pos)
#define   US_IDR_RXBRK_0            (0x0u <<  2) /**< \brief (US_IDR) No Effect */
#define   US_IDR_RXBRK_1            (0x1u <<  2) /**< \brief (US_IDR) Disables the interrupt */
#define US_IDR_ENDRX_Pos            3            /**< \brief (US_IDR) End of Receive Transfer Interrupt Disable */
#define US_IDR_ENDRX                (0x1u << US_IDR_ENDRX_Pos)
#define   US_IDR_ENDRX_0            (0x0u <<  3) /**< \brief (US_IDR) No Effect */
#define   US_IDR_ENDRX_1            (0x1u <<  3) /**< \brief (US_IDR) Disables the interrupt */
#define US_IDR_ENDTX_Pos            4            /**< \brief (US_IDR) End of Transmit Interrupt Disable */
#define US_IDR_ENDTX                (0x1u << US_IDR_ENDTX_Pos)
#define   US_IDR_ENDTX_0            (0x0u <<  4) /**< \brief (US_IDR) No Effect */
#define   US_IDR_ENDTX_1            (0x1u <<  4) /**< \brief (US_IDR) Disables the interrupt */
#define US_IDR_OVRE_Pos             5            /**< \brief (US_IDR) Overrun Error Interrupt Disable */
#define US_IDR_OVRE                 (0x1u << US_IDR_OVRE_Pos)
#define   US_IDR_OVRE_0             (0x0u <<  5) /**< \brief (US_IDR) No Effect */
#define   US_IDR_OVRE_1             (0x1u <<  5) /**< \brief (US_IDR) Disables the interrupt */
#define US_IDR_FRAME_Pos            6            /**< \brief (US_IDR) Framing Error Interrupt Disable */
#define US_IDR_FRAME                (0x1u << US_IDR_FRAME_Pos)
#define   US_IDR_FRAME_0            (0x0u <<  6) /**< \brief (US_IDR) No Effect */
#define   US_IDR_FRAME_1            (0x1u <<  6) /**< \brief (US_IDR) Disables the interrupt */
#define US_IDR_PARE_Pos             7            /**< \brief (US_IDR) Parity Error Interrupt Disable */
#define US_IDR_PARE                 (0x1u << US_IDR_PARE_Pos)
#define   US_IDR_PARE_0             (0x0u <<  7) /**< \brief (US_IDR) No Effect */
#define   US_IDR_PARE_1             (0x1u <<  7) /**< \brief (US_IDR) Disables the interrupt */
#define US_IDR_TIMEOUT_Pos          8            /**< \brief (US_IDR) Time-out Interrupt Disable */
#define US_IDR_TIMEOUT              (0x1u << US_IDR_TIMEOUT_Pos)
#define   US_IDR_TIMEOUT_0          (0x0u <<  8) /**< \brief (US_IDR) No Effect */
#define   US_IDR_TIMEOUT_1          (0x1u <<  8) /**< \brief (US_IDR) Disables the interrupt */
#define US_IDR_TXEMPTY_Pos          9            /**< \brief (US_IDR) Transmitter Empty Interrupt Disable */
#define US_IDR_TXEMPTY              (0x1u << US_IDR_TXEMPTY_Pos)
#define   US_IDR_TXEMPTY_0          (0x0u <<  9) /**< \brief (US_IDR) No Effect */
#define   US_IDR_TXEMPTY_1          (0x1u <<  9) /**< \brief (US_IDR) Disables the interrupt */
#define US_IDR_ITER_Pos             10           /**< \brief (US_IDR) Iteration Interrupt Disable */
#define US_IDR_ITER                 (0x1u << US_IDR_ITER_Pos)
#define   US_IDR_ITER_0             (0x0u << 10) /**< \brief (US_IDR) No Effect */
#define   US_IDR_ITER_1             (0x1u << 10) /**< \brief (US_IDR) Disables the interrupt */
#define US_IDR_UNRE_Pos             10           /**< \brief (US_IDR) SPI Underrun Error Interrupt Disable */
#define US_IDR_UNRE                 (0x1u << US_IDR_UNRE_Pos)
#define   US_IDR_UNRE_0             (0x0u << 10) /**< \brief (US_IDR) No Effect */
#define   US_IDR_UNRE_1             (0x1u << 10) /**< \brief (US_IDR) Disables the interrupt */
#define US_IDR_ITER_Pos             10           /**< \brief (US_IDR) Iteration Interrupt Disable */
#define US_IDR_ITER                 (0x1u << US_IDR_ITER_Pos)
#define   US_IDR_ITER_0             (0x0u << 10) /**< \brief (US_IDR) No Effect */
#define   US_IDR_ITER_1             (0x1u << 10) /**< \brief (US_IDR) Disables the interrupt */
#define US_IDR_TXBUFE_Pos           11           /**< \brief (US_IDR) Buffer Empty Interrupt Disable */
#define US_IDR_TXBUFE               (0x1u << US_IDR_TXBUFE_Pos)
#define   US_IDR_TXBUFE_0           (0x0u << 11) /**< \brief (US_IDR) No Effect */
#define   US_IDR_TXBUFE_1           (0x1u << 11) /**< \brief (US_IDR) Disables the interrupt */
#define US_IDR_RXBUFF_Pos           12           /**< \brief (US_IDR) Buffer Full Interrupt Disable */
#define US_IDR_RXBUFF               (0x1u << US_IDR_RXBUFF_Pos)
#define   US_IDR_RXBUFF_0           (0x0u << 12) /**< \brief (US_IDR) No Effect */
#define   US_IDR_RXBUFF_1           (0x1u << 12) /**< \brief (US_IDR) Disables the interrupt */
#define US_IDR_NACK_Pos             13           /**< \brief (US_IDR) Non Acknowledge  or LIN Break Sent or LIN Break Received Interrupt Disable */
#define US_IDR_NACK                 (0x1u << US_IDR_NACK_Pos)
#define   US_IDR_NACK_0             (0x0u << 13) /**< \brief (US_IDR) No Effect */
#define   US_IDR_NACK_1             (0x1u << 13) /**< \brief (US_IDR) Disables the interrupt */
#define US_IDR_LINID_Pos            14           /**< \brief (US_IDR) LIN Identifier Sent or LIN Identifier Received Interrupt Disable */
#define US_IDR_LINID                (0x1u << US_IDR_LINID_Pos)
#define US_IDR_LINTC_Pos            15           /**< \brief (US_IDR) LIN Transfer Conpleted Interrupt Disable */
#define US_IDR_LINTC                (0x1u << US_IDR_LINTC_Pos)
#define US_IDR_RIIC_Pos             16           /**< \brief (US_IDR) Ring Indicator Input Change Disable */
#define US_IDR_RIIC                 (0x1u << US_IDR_RIIC_Pos)
#define   US_IDR_RIIC_0             (0x0u << 16) /**< \brief (US_IDR) No Effect */
#define   US_IDR_RIIC_1             (0x1u << 16) /**< \brief (US_IDR) Disables the interrupt */
#define US_IDR_DSRIC_Pos            17           /**< \brief (US_IDR) Data Set Ready Input Change Disable */
#define US_IDR_DSRIC                (0x1u << US_IDR_DSRIC_Pos)
#define   US_IDR_DSRIC_0            (0x0u << 17) /**< \brief (US_IDR) No Effect */
#define   US_IDR_DSRIC_1            (0x1u << 17) /**< \brief (US_IDR) Disables the interrupt */
#define US_IDR_DCDIC_Pos            18           /**< \brief (US_IDR) Data Carrier Detect Input Change Interrupt Disable */
#define US_IDR_DCDIC                (0x1u << US_IDR_DCDIC_Pos)
#define   US_IDR_DCDIC_0            (0x0u << 18) /**< \brief (US_IDR) No Effect */
#define   US_IDR_DCDIC_1            (0x1u << 18) /**< \brief (US_IDR) Disables the interrupt */
#define US_IDR_CTSIC_Pos            19           /**< \brief (US_IDR) Clear to Send Input Change Interrupt Disable */
#define US_IDR_CTSIC                (0x1u << US_IDR_CTSIC_Pos)
#define   US_IDR_CTSIC_0            (0x0u << 19) /**< \brief (US_IDR) No Effect */
#define   US_IDR_CTSIC_1            (0x1u << 19) /**< \brief (US_IDR) Disables the interrupt */
#define US_IDR_MANE_Pos             20           /**< \brief (US_IDR) Manchester Error Interrupt Disable */
#define US_IDR_MANE                 (0x1u << US_IDR_MANE_Pos)
#define US_IDR_MANEA_Pos            24           /**< \brief (US_IDR) Manchester Error Interrupt Disable */
#define US_IDR_MANEA                (0x1u << US_IDR_MANEA_Pos)
#define   US_IDR_MANEA_0            (0x0u << 24) /**< \brief (US_IDR) No effect */
#define   US_IDR_MANEA_1            (0x1u << 24) /**< \brief (US_IDR) Disables the corresponding interrupt */
#define US_IDR_LINBE_Pos            25           /**< \brief (US_IDR) LIN Bus Error Interrupt Disable */
#define US_IDR_LINBE                (0x1u << US_IDR_LINBE_Pos)
#define US_IDR_LINISFE_Pos          26           /**< \brief (US_IDR) LIN Inconsistent Synch Field Error Interrupt Disable */
#define US_IDR_LINISFE              (0x1u << US_IDR_LINISFE_Pos)
#define US_IDR_LINIPE_Pos           27           /**< \brief (US_IDR) LIN Identifier Parity Interrupt Disable */
#define US_IDR_LINIPE               (0x1u << US_IDR_LINIPE_Pos)
#define US_IDR_LINCE_Pos            28           /**< \brief (US_IDR) LIN Checksum Error Interrupt Disable */
#define US_IDR_LINCE                (0x1u << US_IDR_LINCE_Pos)
#define US_IDR_LINSNRE_Pos          29           /**< \brief (US_IDR) LIN Slave Not Responding Error Interrupt Disable */
#define US_IDR_LINSNRE              (0x1u << US_IDR_LINSNRE_Pos)
#define US_IDR_LINSTE_Pos           30           /**< \brief (US_IDR) LIN Synch Tolerance Error Interrupt Disable */
#define US_IDR_LINSTE               (0x1u << US_IDR_LINSTE_Pos)
#define   US_IDR_LINSTE_0           (0x0u << 30) /**< \brief (US_IDR) No Effect */
#define   US_IDR_LINSTE_1           (0x1u << 30) /**< \brief (US_IDR) Disables the interrupt */
#define US_IDR_LINHTE_Pos           31           /**< \brief (US_IDR) LIN Header Timeout Error Interrupt Disable */
#define US_IDR_LINHTE               (0x1u << US_IDR_LINHTE_Pos)
#define   US_IDR_LINHTE_0           (0x0u << 31) /**< \brief (US_IDR) No Effect */
#define   US_IDR_LINHTE_1           (0x1u << 31) /**< \brief (US_IDR) Disables the interrupt */
#define US_IDR_MASK                 0xFF1FFFFFu  /**< \brief (US_IDR) MASK Register */

/* -------- US_IMR : (USART Offset: 0x10) (R/  32) Interrupt Mask Register -------- */
#define US_IMR_OFFSET               0x10         /**< \brief (US_IMR offset) Interrupt Mask Register */
#define US_IMR_RESETVALUE           0x00000000   /**< \brief (US_IMR reset_value) Interrupt Mask Register */

// LIN mode
#define US_IMR_LIN_RXRDY_Pos        0            /**< \brief (US_IMR_LIN) RXRDY Interrupt Mask */
#define US_IMR_LIN_RXRDY            (0x1u << US_IMR_LIN_RXRDY_Pos)
#define   US_IMR_LIN_RXRDY_0        (0x0u <<  0) /**< \brief (US_IMR_LIN) The interrupt is disabled */
#define   US_IMR_LIN_RXRDY_1        (0x1u <<  0) /**< \brief (US_IMR_LIN) The interrupt is enabled */
#define US_IMR_LIN_TXRDY_Pos        1            /**< \brief (US_IMR_LIN) TXRDY Interrupt Mask */
#define US_IMR_LIN_TXRDY            (0x1u << US_IMR_LIN_TXRDY_Pos)
#define   US_IMR_LIN_TXRDY_0        (0x0u <<  1) /**< \brief (US_IMR_LIN) The interrupt is disabled */
#define   US_IMR_LIN_TXRDY_1        (0x1u <<  1) /**< \brief (US_IMR_LIN) The interrupt is enabled */
#define US_IMR_LIN_RXBRK_Pos        2            /**< \brief (US_IMR_LIN) Receiver Break Interrupt Mask */
#define US_IMR_LIN_RXBRK            (0x1u << US_IMR_LIN_RXBRK_Pos)
#define   US_IMR_LIN_RXBRK_0        (0x0u <<  2) /**< \brief (US_IMR_LIN) The interrupt is disabled */
#define   US_IMR_LIN_RXBRK_1        (0x1u <<  2) /**< \brief (US_IMR_LIN) The interrupt is enabled */
#define US_IMR_LIN_ENDRX_Pos        3            /**< \brief (US_IMR_LIN) End of Receive Transfer Interrupt Mask */
#define US_IMR_LIN_ENDRX            (0x1u << US_IMR_LIN_ENDRX_Pos)
#define   US_IMR_LIN_ENDRX_0        (0x0u <<  3) /**< \brief (US_IMR_LIN) The interrupt is disabled */
#define   US_IMR_LIN_ENDRX_1        (0x1u <<  3) /**< \brief (US_IMR_LIN) The interrupt is enabled */
#define US_IMR_LIN_ENDTX_Pos        4            /**< \brief (US_IMR_LIN) End of Transmit Interrupt Mask */
#define US_IMR_LIN_ENDTX            (0x1u << US_IMR_LIN_ENDTX_Pos)
#define   US_IMR_LIN_ENDTX_0        (0x0u <<  4) /**< \brief (US_IMR_LIN) The interrupt is disabled */
#define   US_IMR_LIN_ENDTX_1        (0x1u <<  4) /**< \brief (US_IMR_LIN) The interrupt is enabled */
#define US_IMR_LIN_OVRE_Pos         5            /**< \brief (US_IMR_LIN) Overrun Error Interrupt Mask */
#define US_IMR_LIN_OVRE             (0x1u << US_IMR_LIN_OVRE_Pos)
#define   US_IMR_LIN_OVRE_0         (0x0u <<  5) /**< \brief (US_IMR_LIN) The interrupt is disabled */
#define   US_IMR_LIN_OVRE_1         (0x1u <<  5) /**< \brief (US_IMR_LIN) The interrupt is enabled */
#define US_IMR_LIN_FRAME_Pos        6            /**< \brief (US_IMR_LIN) Framing Error Interrupt Mask */
#define US_IMR_LIN_FRAME            (0x1u << US_IMR_LIN_FRAME_Pos)
#define   US_IMR_LIN_FRAME_0        (0x0u <<  6) /**< \brief (US_IMR_LIN) The interrupt is disabled */
#define   US_IMR_LIN_FRAME_1        (0x1u <<  6) /**< \brief (US_IMR_LIN) The interrupt is enabled */
#define US_IMR_LIN_PARE_Pos         7            /**< \brief (US_IMR_LIN) Parity Error Interrupt Mask */
#define US_IMR_LIN_PARE             (0x1u << US_IMR_LIN_PARE_Pos)
#define   US_IMR_LIN_PARE_0         (0x0u <<  7) /**< \brief (US_IMR_LIN) The interrupt is disabled */
#define   US_IMR_LIN_PARE_1         (0x1u <<  7) /**< \brief (US_IMR_LIN) The interrupt is enabled */
#define US_IMR_LIN_TIMEOUT_Pos      8            /**< \brief (US_IMR_LIN) Time-out Interrupt Mask */
#define US_IMR_LIN_TIMEOUT          (0x1u << US_IMR_LIN_TIMEOUT_Pos)
#define   US_IMR_LIN_TIMEOUT_0      (0x0u <<  8) /**< \brief (US_IMR_LIN) The interrupt is disabled */
#define   US_IMR_LIN_TIMEOUT_1      (0x1u <<  8) /**< \brief (US_IMR_LIN) The interrupt is enabled */
#define US_IMR_LIN_TXEMPTY_Pos      9            /**< \brief (US_IMR_LIN) TXEMPTY Interrupt Mask */
#define US_IMR_LIN_TXEMPTY          (0x1u << US_IMR_LIN_TXEMPTY_Pos)
#define   US_IMR_LIN_TXEMPTY_0      (0x0u <<  9) /**< \brief (US_IMR_LIN) The interrupt is disabled */
#define   US_IMR_LIN_TXEMPTY_1      (0x1u <<  9) /**< \brief (US_IMR_LIN) The interrupt is enabled */
#define US_IMR_LIN_ITER_Pos         10           /**< \brief (US_IMR_LIN) Iteration Interrupt Mask */
#define US_IMR_LIN_ITER             (0x1u << US_IMR_LIN_ITER_Pos)
#define   US_IMR_LIN_ITER_0         (0x0u << 10) /**< \brief (US_IMR_LIN) The interrupt is disabled */
#define   US_IMR_LIN_ITER_1         (0x1u << 10) /**< \brief (US_IMR_LIN) The interrupt is enabled */
#define US_IMR_LIN_TXBUFE_Pos       11           /**< \brief (US_IMR_LIN) Buffer Empty Interrupt Mask */
#define US_IMR_LIN_TXBUFE           (0x1u << US_IMR_LIN_TXBUFE_Pos)
#define   US_IMR_LIN_TXBUFE_0       (0x0u << 11) /**< \brief (US_IMR_LIN) The interrupt is disabled */
#define   US_IMR_LIN_TXBUFE_1       (0x1u << 11) /**< \brief (US_IMR_LIN) The interrupt is enabled */
#define US_IMR_LIN_RXBUFF_Pos       12           /**< \brief (US_IMR_LIN) Buffer Full Interrupt Mask */
#define US_IMR_LIN_RXBUFF           (0x1u << US_IMR_LIN_RXBUFF_Pos)
#define   US_IMR_LIN_RXBUFF_0       (0x0u << 12) /**< \brief (US_IMR_LIN) The interrupt is disabled */
#define   US_IMR_LIN_RXBUFF_1       (0x1u << 12) /**< \brief (US_IMR_LIN) The interrupt is enabled */
#define US_IMR_LIN_NACK_Pos         13           /**< \brief (US_IMR_LIN) Non Acknowledge  or LIN Break Sent or LIN Break Received Interrupt Mask */
#define US_IMR_LIN_NACK             (0x1u << US_IMR_LIN_NACK_Pos)
#define   US_IMR_LIN_NACK_0         (0x0u << 13) /**< \brief (US_IMR_LIN) The interrupt is disabled */
#define   US_IMR_LIN_NACK_1         (0x1u << 13) /**< \brief (US_IMR_LIN) The interrupt is enabled */
#define US_IMR_LIN_LINID_Pos        14           /**< \brief (US_IMR_LIN) LIN Identifier Sent or LIN Received Interrupt Mask */
#define US_IMR_LIN_LINID            (0x1u << US_IMR_LIN_LINID_Pos)
#define US_IMR_LIN_LINTC_Pos        15           /**< \brief (US_IMR_LIN) LIN Transfer Conpleted Interrupt Mask */
#define US_IMR_LIN_LINTC            (0x1u << US_IMR_LIN_LINTC_Pos)
#define US_IMR_LIN_RIIC_Pos         16           /**< \brief (US_IMR_LIN) Ring Indicator Input Change Mask */
#define US_IMR_LIN_RIIC             (0x1u << US_IMR_LIN_RIIC_Pos)
#define   US_IMR_LIN_RIIC_0         (0x0u << 16) /**< \brief (US_IMR_LIN) The interrupt is disabled */
#define   US_IMR_LIN_RIIC_1         (0x1u << 16) /**< \brief (US_IMR_LIN) The interrupt is enabled */
#define US_IMR_LIN_DSRIC_Pos        17           /**< \brief (US_IMR_LIN) Data Set Ready Input Change Mask */
#define US_IMR_LIN_DSRIC            (0x1u << US_IMR_LIN_DSRIC_Pos)
#define   US_IMR_LIN_DSRIC_0        (0x0u << 17) /**< \brief (US_IMR_LIN) The interrupt is disabled */
#define   US_IMR_LIN_DSRIC_1        (0x1u << 17) /**< \brief (US_IMR_LIN) The interrupt is enabled */
#define US_IMR_LIN_DCDIC_Pos        18           /**< \brief (US_IMR_LIN) Data Carrier Detect Input Change Interrupt Mask */
#define US_IMR_LIN_DCDIC            (0x1u << US_IMR_LIN_DCDIC_Pos)
#define   US_IMR_LIN_DCDIC_0        (0x0u << 18) /**< \brief (US_IMR_LIN) The interrupt is disabled */
#define   US_IMR_LIN_DCDIC_1        (0x1u << 18) /**< \brief (US_IMR_LIN) The interrupt is enabled */
#define US_IMR_LIN_CTSIC_Pos        19           /**< \brief (US_IMR_LIN) Clear to Send Input Change Interrupt Mask */
#define US_IMR_LIN_CTSIC            (0x1u << US_IMR_LIN_CTSIC_Pos)
#define   US_IMR_LIN_CTSIC_0        (0x0u << 19) /**< \brief (US_IMR_LIN) The interrupt is disabled */
#define   US_IMR_LIN_CTSIC_1        (0x1u << 19) /**< \brief (US_IMR_LIN) The interrupt is enabled */
#define US_IMR_LIN_LINBE_Pos        25           /**< \brief (US_IMR_LIN) LIN Bus Error Interrupt Mask */
#define US_IMR_LIN_LINBE            (0x1u << US_IMR_LIN_LINBE_Pos)
#define US_IMR_LIN_LINISFE_Pos      26           /**< \brief (US_IMR_LIN) LIN Inconsistent Synch Field Error Interrupt Mask */
#define US_IMR_LIN_LINISFE          (0x1u << US_IMR_LIN_LINISFE_Pos)
#define US_IMR_LIN_LINIPE_Pos       27           /**< \brief (US_IMR_LIN) LIN Identifier Parity Interrupt Mask */
#define US_IMR_LIN_LINIPE           (0x1u << US_IMR_LIN_LINIPE_Pos)
#define US_IMR_LIN_LINCE_Pos        28           /**< \brief (US_IMR_LIN) LIN Checksum Error Interrupt Mask */
#define US_IMR_LIN_LINCE            (0x1u << US_IMR_LIN_LINCE_Pos)
#define US_IMR_LIN_LINSNRE_Pos      29           /**< \brief (US_IMR_LIN) LIN Slave Not Responding Error Interrupt Mask */
#define US_IMR_LIN_LINSNRE          (0x1u << US_IMR_LIN_LINSNRE_Pos)
#define US_IMR_LIN_LINSTE_Pos       30           /**< \brief (US_IMR_LIN) LIN Synch Tolerance Error Interrupt Mask */
#define US_IMR_LIN_LINSTE           (0x1u << US_IMR_LIN_LINSTE_Pos)
#define   US_IMR_LIN_LINSTE_0       (0x0u << 30) /**< \brief (US_IMR_LIN) The interrupt is disabled */
#define   US_IMR_LIN_LINSTE_1       (0x1u << 30) /**< \brief (US_IMR_LIN) The interrupt is enabled */
#define US_IMR_LIN_LINHTE_Pos       31           /**< \brief (US_IMR_LIN) LIN Header Timeout Error Interrupt Mask */
#define US_IMR_LIN_LINHTE           (0x1u << US_IMR_LIN_LINHTE_Pos)
#define   US_IMR_LIN_LINHTE_0       (0x0u << 31) /**< \brief (US_IMR_LIN) The interrupt is disabled */
#define   US_IMR_LIN_LINHTE_1       (0x1u << 31) /**< \brief (US_IMR_LIN) The interrupt is enabled */
#define US_IMR_LIN_MASK             0xFE0FFFFFu  /**< \brief (US_IMR_LIN) MASK Register */

// SPI_SLAVE mode
#define US_IMR_SPI_SLAVE_RXRDY_Pos  0            /**< \brief (US_IMR_SPI_SLAVE) RXRDY Interrupt Mask */
#define US_IMR_SPI_SLAVE_RXRDY      (0x1u << US_IMR_SPI_SLAVE_RXRDY_Pos)
#define   US_IMR_SPI_SLAVE_RXRDY_0  (0x0u <<  0) /**< \brief (US_IMR_SPI_SLAVE) The interrupt is disabled */
#define   US_IMR_SPI_SLAVE_RXRDY_1  (0x1u <<  0) /**< \brief (US_IMR_SPI_SLAVE) The interrupt is enabled */
#define US_IMR_SPI_SLAVE_TXRDY_Pos  1            /**< \brief (US_IMR_SPI_SLAVE) TXRDY Interrupt Mask */
#define US_IMR_SPI_SLAVE_TXRDY      (0x1u << US_IMR_SPI_SLAVE_TXRDY_Pos)
#define   US_IMR_SPI_SLAVE_TXRDY_0  (0x0u <<  1) /**< \brief (US_IMR_SPI_SLAVE) The interrupt is disabled */
#define   US_IMR_SPI_SLAVE_TXRDY_1  (0x1u <<  1) /**< \brief (US_IMR_SPI_SLAVE) The interrupt is enabled */
#define US_IMR_SPI_SLAVE_RXBRK_Pos  2            /**< \brief (US_IMR_SPI_SLAVE) Receiver Break Interrupt Mask */
#define US_IMR_SPI_SLAVE_RXBRK      (0x1u << US_IMR_SPI_SLAVE_RXBRK_Pos)
#define   US_IMR_SPI_SLAVE_RXBRK_0  (0x0u <<  2) /**< \brief (US_IMR_SPI_SLAVE) The interrupt is disabled */
#define   US_IMR_SPI_SLAVE_RXBRK_1  (0x1u <<  2) /**< \brief (US_IMR_SPI_SLAVE) The interrupt is enabled */
#define US_IMR_SPI_SLAVE_ENDRX_Pos  3            /**< \brief (US_IMR_SPI_SLAVE) End of Receive Transfer Interrupt Mask */
#define US_IMR_SPI_SLAVE_ENDRX      (0x1u << US_IMR_SPI_SLAVE_ENDRX_Pos)
#define   US_IMR_SPI_SLAVE_ENDRX_0  (0x0u <<  3) /**< \brief (US_IMR_SPI_SLAVE) The interrupt is disabled */
#define   US_IMR_SPI_SLAVE_ENDRX_1  (0x1u <<  3) /**< \brief (US_IMR_SPI_SLAVE) The interrupt is enabled */
#define US_IMR_SPI_SLAVE_ENDTX_Pos  4            /**< \brief (US_IMR_SPI_SLAVE) End of Transmit Interrupt Mask */
#define US_IMR_SPI_SLAVE_ENDTX      (0x1u << US_IMR_SPI_SLAVE_ENDTX_Pos)
#define   US_IMR_SPI_SLAVE_ENDTX_0  (0x0u <<  4) /**< \brief (US_IMR_SPI_SLAVE) The interrupt is disabled */
#define   US_IMR_SPI_SLAVE_ENDTX_1  (0x1u <<  4) /**< \brief (US_IMR_SPI_SLAVE) The interrupt is enabled */
#define US_IMR_SPI_SLAVE_OVRE_Pos   5            /**< \brief (US_IMR_SPI_SLAVE) Overrun Error Interrupt Mask */
#define US_IMR_SPI_SLAVE_OVRE       (0x1u << US_IMR_SPI_SLAVE_OVRE_Pos)
#define   US_IMR_SPI_SLAVE_OVRE_0   (0x0u <<  5) /**< \brief (US_IMR_SPI_SLAVE) The interrupt is disabled */
#define   US_IMR_SPI_SLAVE_OVRE_1   (0x1u <<  5) /**< \brief (US_IMR_SPI_SLAVE) The interrupt is enabled */
#define US_IMR_SPI_SLAVE_FRAME_Pos  6            /**< \brief (US_IMR_SPI_SLAVE) Framing Error Interrupt Mask */
#define US_IMR_SPI_SLAVE_FRAME      (0x1u << US_IMR_SPI_SLAVE_FRAME_Pos)
#define   US_IMR_SPI_SLAVE_FRAME_0  (0x0u <<  6) /**< \brief (US_IMR_SPI_SLAVE) The interrupt is disabled */
#define   US_IMR_SPI_SLAVE_FRAME_1  (0x1u <<  6) /**< \brief (US_IMR_SPI_SLAVE) The interrupt is enabled */
#define US_IMR_SPI_SLAVE_PARE_Pos   7            /**< \brief (US_IMR_SPI_SLAVE) Parity Error Interrupt Mask */
#define US_IMR_SPI_SLAVE_PARE       (0x1u << US_IMR_SPI_SLAVE_PARE_Pos)
#define   US_IMR_SPI_SLAVE_PARE_0   (0x0u <<  7) /**< \brief (US_IMR_SPI_SLAVE) The interrupt is disabled */
#define   US_IMR_SPI_SLAVE_PARE_1   (0x1u <<  7) /**< \brief (US_IMR_SPI_SLAVE) The interrupt is enabled */
#define US_IMR_SPI_SLAVE_TIMEOUT_Pos 8            /**< \brief (US_IMR_SPI_SLAVE) Time-out Interrupt Mask */
#define US_IMR_SPI_SLAVE_TIMEOUT    (0x1u << US_IMR_SPI_SLAVE_TIMEOUT_Pos)
#define   US_IMR_SPI_SLAVE_TIMEOUT_0 (0x0u <<  8) /**< \brief (US_IMR_SPI_SLAVE) The interrupt is disabled */
#define   US_IMR_SPI_SLAVE_TIMEOUT_1 (0x1u <<  8) /**< \brief (US_IMR_SPI_SLAVE) The interrupt is enabled */
#define US_IMR_SPI_SLAVE_TXEMPTY_Pos 9            /**< \brief (US_IMR_SPI_SLAVE) TXEMPTY Interrupt Mask */
#define US_IMR_SPI_SLAVE_TXEMPTY    (0x1u << US_IMR_SPI_SLAVE_TXEMPTY_Pos)
#define   US_IMR_SPI_SLAVE_TXEMPTY_0 (0x0u <<  9) /**< \brief (US_IMR_SPI_SLAVE) The interrupt is disabled */
#define   US_IMR_SPI_SLAVE_TXEMPTY_1 (0x1u <<  9) /**< \brief (US_IMR_SPI_SLAVE) The interrupt is enabled */
#define US_IMR_SPI_SLAVE_UNRE_Pos   10           /**< \brief (US_IMR_SPI_SLAVE) SPI Underrun Error Interrupt Mask */
#define US_IMR_SPI_SLAVE_UNRE       (0x1u << US_IMR_SPI_SLAVE_UNRE_Pos)
#define   US_IMR_SPI_SLAVE_UNRE_0   (0x0u << 10) /**< \brief (US_IMR_SPI_SLAVE) The interrupt is disabled */
#define   US_IMR_SPI_SLAVE_UNRE_1   (0x1u << 10) /**< \brief (US_IMR_SPI_SLAVE) The interrupt is enabled */
#define US_IMR_SPI_SLAVE_TXBUFE_Pos 11           /**< \brief (US_IMR_SPI_SLAVE) Buffer Empty Interrupt Mask */
#define US_IMR_SPI_SLAVE_TXBUFE     (0x1u << US_IMR_SPI_SLAVE_TXBUFE_Pos)
#define   US_IMR_SPI_SLAVE_TXBUFE_0 (0x0u << 11) /**< \brief (US_IMR_SPI_SLAVE) The interrupt is disabled */
#define   US_IMR_SPI_SLAVE_TXBUFE_1 (0x1u << 11) /**< \brief (US_IMR_SPI_SLAVE) The interrupt is enabled */
#define US_IMR_SPI_SLAVE_RXBUFF_Pos 12           /**< \brief (US_IMR_SPI_SLAVE) Buffer Full Interrupt Mask */
#define US_IMR_SPI_SLAVE_RXBUFF     (0x1u << US_IMR_SPI_SLAVE_RXBUFF_Pos)
#define   US_IMR_SPI_SLAVE_RXBUFF_0 (0x0u << 12) /**< \brief (US_IMR_SPI_SLAVE) The interrupt is disabled */
#define   US_IMR_SPI_SLAVE_RXBUFF_1 (0x1u << 12) /**< \brief (US_IMR_SPI_SLAVE) The interrupt is enabled */
#define US_IMR_SPI_SLAVE_NACK_Pos   13           /**< \brief (US_IMR_SPI_SLAVE) Non Acknowledge Interrupt Mask */
#define US_IMR_SPI_SLAVE_NACK       (0x1u << US_IMR_SPI_SLAVE_NACK_Pos)
#define   US_IMR_SPI_SLAVE_NACK_0   (0x0u << 13) /**< \brief (US_IMR_SPI_SLAVE) The interrupt is disabled */
#define   US_IMR_SPI_SLAVE_NACK_1   (0x1u << 13) /**< \brief (US_IMR_SPI_SLAVE) The interrupt is enabled */
#define US_IMR_SPI_SLAVE_RIIC_Pos   16           /**< \brief (US_IMR_SPI_SLAVE) Ring Indicator Input Change Mask */
#define US_IMR_SPI_SLAVE_RIIC       (0x1u << US_IMR_SPI_SLAVE_RIIC_Pos)
#define   US_IMR_SPI_SLAVE_RIIC_0   (0x0u << 16) /**< \brief (US_IMR_SPI_SLAVE) The interrupt is disabled */
#define   US_IMR_SPI_SLAVE_RIIC_1   (0x1u << 16) /**< \brief (US_IMR_SPI_SLAVE) The interrupt is enabled */
#define US_IMR_SPI_SLAVE_DSRIC_Pos  17           /**< \brief (US_IMR_SPI_SLAVE) Data Set Ready Input Change Mask */
#define US_IMR_SPI_SLAVE_DSRIC      (0x1u << US_IMR_SPI_SLAVE_DSRIC_Pos)
#define   US_IMR_SPI_SLAVE_DSRIC_0  (0x0u << 17) /**< \brief (US_IMR_SPI_SLAVE) The interrupt is disabled */
#define   US_IMR_SPI_SLAVE_DSRIC_1  (0x1u << 17) /**< \brief (US_IMR_SPI_SLAVE) The interrupt is enabled */
#define US_IMR_SPI_SLAVE_DCDIC_Pos  18           /**< \brief (US_IMR_SPI_SLAVE) Data Carrier Detect Input Change Interrupt Mask */
#define US_IMR_SPI_SLAVE_DCDIC      (0x1u << US_IMR_SPI_SLAVE_DCDIC_Pos)
#define   US_IMR_SPI_SLAVE_DCDIC_0  (0x0u << 18) /**< \brief (US_IMR_SPI_SLAVE) The interrupt is disabled */
#define   US_IMR_SPI_SLAVE_DCDIC_1  (0x1u << 18) /**< \brief (US_IMR_SPI_SLAVE) The interrupt is enabled */
#define US_IMR_SPI_SLAVE_CTSIC_Pos  19           /**< \brief (US_IMR_SPI_SLAVE) Clear to Send Input Change Interrupt Mask */
#define US_IMR_SPI_SLAVE_CTSIC      (0x1u << US_IMR_SPI_SLAVE_CTSIC_Pos)
#define   US_IMR_SPI_SLAVE_CTSIC_0  (0x0u << 19) /**< \brief (US_IMR_SPI_SLAVE) The interrupt is disabled */
#define   US_IMR_SPI_SLAVE_CTSIC_1  (0x1u << 19) /**< \brief (US_IMR_SPI_SLAVE) The interrupt is enabled */
#define US_IMR_SPI_SLAVE_MASK       0x000F3FFFu  /**< \brief (US_IMR_SPI_SLAVE) MASK Register */

// USART mode
#define US_IMR_USART_RXRDY_Pos      0            /**< \brief (US_IMR_USART) RXRDY Interrupt Mask */
#define US_IMR_USART_RXRDY          (0x1u << US_IMR_USART_RXRDY_Pos)
#define   US_IMR_USART_RXRDY_0      (0x0u <<  0) /**< \brief (US_IMR_USART) The interrupt is disabled */
#define   US_IMR_USART_RXRDY_1      (0x1u <<  0) /**< \brief (US_IMR_USART) The interrupt is enabled */
#define US_IMR_USART_TXRDY_Pos      1            /**< \brief (US_IMR_USART) TXRDY Interrupt Mask */
#define US_IMR_USART_TXRDY          (0x1u << US_IMR_USART_TXRDY_Pos)
#define   US_IMR_USART_TXRDY_0      (0x0u <<  1) /**< \brief (US_IMR_USART) The interrupt is disabled */
#define   US_IMR_USART_TXRDY_1      (0x1u <<  1) /**< \brief (US_IMR_USART) The interrupt is enabled */
#define US_IMR_USART_RXBRK_Pos      2            /**< \brief (US_IMR_USART) Receiver Break Interrupt Mask */
#define US_IMR_USART_RXBRK          (0x1u << US_IMR_USART_RXBRK_Pos)
#define   US_IMR_USART_RXBRK_0      (0x0u <<  2) /**< \brief (US_IMR_USART) The interrupt is disabled */
#define   US_IMR_USART_RXBRK_1      (0x1u <<  2) /**< \brief (US_IMR_USART) The interrupt is enabled */
#define US_IMR_USART_ENDRX_Pos      3            /**< \brief (US_IMR_USART) End of Receive Transfer Interrupt Mask */
#define US_IMR_USART_ENDRX          (0x1u << US_IMR_USART_ENDRX_Pos)
#define   US_IMR_USART_ENDRX_0      (0x0u <<  3) /**< \brief (US_IMR_USART) The interrupt is disabled */
#define   US_IMR_USART_ENDRX_1      (0x1u <<  3) /**< \brief (US_IMR_USART) The interrupt is enabled */
#define US_IMR_USART_ENDTX_Pos      4            /**< \brief (US_IMR_USART) End of Transmit Interrupt Mask */
#define US_IMR_USART_ENDTX          (0x1u << US_IMR_USART_ENDTX_Pos)
#define   US_IMR_USART_ENDTX_0      (0x0u <<  4) /**< \brief (US_IMR_USART) The interrupt is disabled */
#define   US_IMR_USART_ENDTX_1      (0x1u <<  4) /**< \brief (US_IMR_USART) The interrupt is enabled */
#define US_IMR_USART_OVRE_Pos       5            /**< \brief (US_IMR_USART) Overrun Error Interrupt Mask */
#define US_IMR_USART_OVRE           (0x1u << US_IMR_USART_OVRE_Pos)
#define   US_IMR_USART_OVRE_0       (0x0u <<  5) /**< \brief (US_IMR_USART) The interrupt is disabled */
#define   US_IMR_USART_OVRE_1       (0x1u <<  5) /**< \brief (US_IMR_USART) The interrupt is enabled */
#define US_IMR_USART_FRAME_Pos      6            /**< \brief (US_IMR_USART) Framing Error Interrupt Mask */
#define US_IMR_USART_FRAME          (0x1u << US_IMR_USART_FRAME_Pos)
#define   US_IMR_USART_FRAME_0      (0x0u <<  6) /**< \brief (US_IMR_USART) The interrupt is disabled */
#define   US_IMR_USART_FRAME_1      (0x1u <<  6) /**< \brief (US_IMR_USART) The interrupt is enabled */
#define US_IMR_USART_PARE_Pos       7            /**< \brief (US_IMR_USART) Parity Error Interrupt Mask */
#define US_IMR_USART_PARE           (0x1u << US_IMR_USART_PARE_Pos)
#define   US_IMR_USART_PARE_0       (0x0u <<  7) /**< \brief (US_IMR_USART) The interrupt is disabled */
#define   US_IMR_USART_PARE_1       (0x1u <<  7) /**< \brief (US_IMR_USART) The interrupt is enabled */
#define US_IMR_USART_TIMEOUT_Pos    8            /**< \brief (US_IMR_USART) Time-out Interrupt Mask */
#define US_IMR_USART_TIMEOUT        (0x1u << US_IMR_USART_TIMEOUT_Pos)
#define   US_IMR_USART_TIMEOUT_0    (0x0u <<  8) /**< \brief (US_IMR_USART) The interrupt is disabled */
#define   US_IMR_USART_TIMEOUT_1    (0x1u <<  8) /**< \brief (US_IMR_USART) The interrupt is enabled */
#define US_IMR_USART_TXEMPTY_Pos    9            /**< \brief (US_IMR_USART) TXEMPTY Interrupt Mask */
#define US_IMR_USART_TXEMPTY        (0x1u << US_IMR_USART_TXEMPTY_Pos)
#define   US_IMR_USART_TXEMPTY_0    (0x0u <<  9) /**< \brief (US_IMR_USART) The interrupt is disabled */
#define   US_IMR_USART_TXEMPTY_1    (0x1u <<  9) /**< \brief (US_IMR_USART) The interrupt is enabled */
#define US_IMR_USART_ITER_Pos       10           /**< \brief (US_IMR_USART) Iteration Interrupt Mask */
#define US_IMR_USART_ITER           (0x1u << US_IMR_USART_ITER_Pos)
#define   US_IMR_USART_ITER_0       (0x0u << 10) /**< \brief (US_IMR_USART) The interrupt is disabled */
#define   US_IMR_USART_ITER_1       (0x1u << 10) /**< \brief (US_IMR_USART) The interrupt is enabled */
#define US_IMR_USART_TXBUFE_Pos     11           /**< \brief (US_IMR_USART) Buffer Empty Interrupt Mask */
#define US_IMR_USART_TXBUFE         (0x1u << US_IMR_USART_TXBUFE_Pos)
#define   US_IMR_USART_TXBUFE_0     (0x0u << 11) /**< \brief (US_IMR_USART) The interrupt is disabled */
#define   US_IMR_USART_TXBUFE_1     (0x1u << 11) /**< \brief (US_IMR_USART) The interrupt is enabled */
#define US_IMR_USART_RXBUFF_Pos     12           /**< \brief (US_IMR_USART) Buffer Full Interrupt Mask */
#define US_IMR_USART_RXBUFF         (0x1u << US_IMR_USART_RXBUFF_Pos)
#define   US_IMR_USART_RXBUFF_0     (0x0u << 12) /**< \brief (US_IMR_USART) The interrupt is disabled */
#define   US_IMR_USART_RXBUFF_1     (0x1u << 12) /**< \brief (US_IMR_USART) The interrupt is enabled */
#define US_IMR_USART_NACK_Pos       13           /**< \brief (US_IMR_USART) Non Acknowledge Interrupt Mask */
#define US_IMR_USART_NACK           (0x1u << US_IMR_USART_NACK_Pos)
#define   US_IMR_USART_NACK_0       (0x0u << 13) /**< \brief (US_IMR_USART) The interrupt is disabled */
#define   US_IMR_USART_NACK_1       (0x1u << 13) /**< \brief (US_IMR_USART) The interrupt is enabled */
#define US_IMR_USART_RIIC_Pos       16           /**< \brief (US_IMR_USART) Ring Indicator Input Change Mask */
#define US_IMR_USART_RIIC           (0x1u << US_IMR_USART_RIIC_Pos)
#define   US_IMR_USART_RIIC_0       (0x0u << 16) /**< \brief (US_IMR_USART) The interrupt is disabled */
#define   US_IMR_USART_RIIC_1       (0x1u << 16) /**< \brief (US_IMR_USART) The interrupt is enabled */
#define US_IMR_USART_DSRIC_Pos      17           /**< \brief (US_IMR_USART) Data Set Ready Input Change Mask */
#define US_IMR_USART_DSRIC          (0x1u << US_IMR_USART_DSRIC_Pos)
#define   US_IMR_USART_DSRIC_0      (0x0u << 17) /**< \brief (US_IMR_USART) The interrupt is disabled */
#define   US_IMR_USART_DSRIC_1      (0x1u << 17) /**< \brief (US_IMR_USART) The interrupt is enabled */
#define US_IMR_USART_DCDIC_Pos      18           /**< \brief (US_IMR_USART) Data Carrier Detect Input Change Interrupt Mask */
#define US_IMR_USART_DCDIC          (0x1u << US_IMR_USART_DCDIC_Pos)
#define   US_IMR_USART_DCDIC_0      (0x0u << 18) /**< \brief (US_IMR_USART) The interrupt is disabled */
#define   US_IMR_USART_DCDIC_1      (0x1u << 18) /**< \brief (US_IMR_USART) The interrupt is enabled */
#define US_IMR_USART_CTSIC_Pos      19           /**< \brief (US_IMR_USART) Clear to Send Input Change Interrupt Mask */
#define US_IMR_USART_CTSIC          (0x1u << US_IMR_USART_CTSIC_Pos)
#define   US_IMR_USART_CTSIC_0      (0x0u << 19) /**< \brief (US_IMR_USART) The interrupt is disabled */
#define   US_IMR_USART_CTSIC_1      (0x1u << 19) /**< \brief (US_IMR_USART) The interrupt is enabled */
#define US_IMR_USART_MANE_Pos       20           /**< \brief (US_IMR_USART)  */
#define US_IMR_USART_MANE           (0x1u << US_IMR_USART_MANE_Pos)
#define US_IMR_USART_MANEA_Pos      24           /**< \brief (US_IMR_USART) Manchester Error Interrupt Mask */
#define US_IMR_USART_MANEA          (0x1u << US_IMR_USART_MANEA_Pos)
#define   US_IMR_USART_MANEA_0      (0x0u << 24) /**< \brief (US_IMR_USART) The interrupt is disabled */
#define   US_IMR_USART_MANEA_1      (0x1u << 24) /**< \brief (US_IMR_USART) The interrupt is enabled */
#define US_IMR_USART_MASK           0x011F3FFFu  /**< \brief (US_IMR_USART) MASK Register */

// Any mode
#define US_IMR_RXRDY_Pos            0            /**< \brief (US_IMR) RXRDY Interrupt Mask */
#define US_IMR_RXRDY                (0x1u << US_IMR_RXRDY_Pos)
#define   US_IMR_RXRDY_0            (0x0u <<  0) /**< \brief (US_IMR) The interrupt is disabled */
#define   US_IMR_RXRDY_1            (0x1u <<  0) /**< \brief (US_IMR) The interrupt is enabled */
#define US_IMR_TXRDY_Pos            1            /**< \brief (US_IMR) TXRDY Interrupt Mask */
#define US_IMR_TXRDY                (0x1u << US_IMR_TXRDY_Pos)
#define   US_IMR_TXRDY_0            (0x0u <<  1) /**< \brief (US_IMR) The interrupt is disabled */
#define   US_IMR_TXRDY_1            (0x1u <<  1) /**< \brief (US_IMR) The interrupt is enabled */
#define US_IMR_RXBRK_Pos            2            /**< \brief (US_IMR) Receiver Break Interrupt Mask */
#define US_IMR_RXBRK                (0x1u << US_IMR_RXBRK_Pos)
#define   US_IMR_RXBRK_0            (0x0u <<  2) /**< \brief (US_IMR) The interrupt is disabled */
#define   US_IMR_RXBRK_1            (0x1u <<  2) /**< \brief (US_IMR) The interrupt is enabled */
#define US_IMR_ENDRX_Pos            3            /**< \brief (US_IMR) End of Receive Transfer Interrupt Mask */
#define US_IMR_ENDRX                (0x1u << US_IMR_ENDRX_Pos)
#define   US_IMR_ENDRX_0            (0x0u <<  3) /**< \brief (US_IMR) The interrupt is disabled */
#define   US_IMR_ENDRX_1            (0x1u <<  3) /**< \brief (US_IMR) The interrupt is enabled */
#define US_IMR_ENDTX_Pos            4            /**< \brief (US_IMR) End of Transmit Interrupt Mask */
#define US_IMR_ENDTX                (0x1u << US_IMR_ENDTX_Pos)
#define   US_IMR_ENDTX_0            (0x0u <<  4) /**< \brief (US_IMR) The interrupt is disabled */
#define   US_IMR_ENDTX_1            (0x1u <<  4) /**< \brief (US_IMR) The interrupt is enabled */
#define US_IMR_OVRE_Pos             5            /**< \brief (US_IMR) Overrun Error Interrupt Mask */
#define US_IMR_OVRE                 (0x1u << US_IMR_OVRE_Pos)
#define   US_IMR_OVRE_0             (0x0u <<  5) /**< \brief (US_IMR) The interrupt is disabled */
#define   US_IMR_OVRE_1             (0x1u <<  5) /**< \brief (US_IMR) The interrupt is enabled */
#define US_IMR_FRAME_Pos            6            /**< \brief (US_IMR) Framing Error Interrupt Mask */
#define US_IMR_FRAME                (0x1u << US_IMR_FRAME_Pos)
#define   US_IMR_FRAME_0            (0x0u <<  6) /**< \brief (US_IMR) The interrupt is disabled */
#define   US_IMR_FRAME_1            (0x1u <<  6) /**< \brief (US_IMR) The interrupt is enabled */
#define US_IMR_PARE_Pos             7            /**< \brief (US_IMR) Parity Error Interrupt Mask */
#define US_IMR_PARE                 (0x1u << US_IMR_PARE_Pos)
#define   US_IMR_PARE_0             (0x0u <<  7) /**< \brief (US_IMR) The interrupt is disabled */
#define   US_IMR_PARE_1             (0x1u <<  7) /**< \brief (US_IMR) The interrupt is enabled */
#define US_IMR_TIMEOUT_Pos          8            /**< \brief (US_IMR) Time-out Interrupt Mask */
#define US_IMR_TIMEOUT              (0x1u << US_IMR_TIMEOUT_Pos)
#define   US_IMR_TIMEOUT_0          (0x0u <<  8) /**< \brief (US_IMR) The interrupt is disabled */
#define   US_IMR_TIMEOUT_1          (0x1u <<  8) /**< \brief (US_IMR) The interrupt is enabled */
#define US_IMR_TXEMPTY_Pos          9            /**< \brief (US_IMR) TXEMPTY Interrupt Mask */
#define US_IMR_TXEMPTY              (0x1u << US_IMR_TXEMPTY_Pos)
#define   US_IMR_TXEMPTY_0          (0x0u <<  9) /**< \brief (US_IMR) The interrupt is disabled */
#define   US_IMR_TXEMPTY_1          (0x1u <<  9) /**< \brief (US_IMR) The interrupt is enabled */
#define US_IMR_ITER_Pos             10           /**< \brief (US_IMR) Iteration Interrupt Mask */
#define US_IMR_ITER                 (0x1u << US_IMR_ITER_Pos)
#define   US_IMR_ITER_0             (0x0u << 10) /**< \brief (US_IMR) The interrupt is disabled */
#define   US_IMR_ITER_1             (0x1u << 10) /**< \brief (US_IMR) The interrupt is enabled */
#define US_IMR_UNRE_Pos             10           /**< \brief (US_IMR) SPI Underrun Error Interrupt Mask */
#define US_IMR_UNRE                 (0x1u << US_IMR_UNRE_Pos)
#define   US_IMR_UNRE_0             (0x0u << 10) /**< \brief (US_IMR) The interrupt is disabled */
#define   US_IMR_UNRE_1             (0x1u << 10) /**< \brief (US_IMR) The interrupt is enabled */
#define US_IMR_ITER_Pos             10           /**< \brief (US_IMR) Iteration Interrupt Mask */
#define US_IMR_ITER                 (0x1u << US_IMR_ITER_Pos)
#define   US_IMR_ITER_0             (0x0u << 10) /**< \brief (US_IMR) The interrupt is disabled */
#define   US_IMR_ITER_1             (0x1u << 10) /**< \brief (US_IMR) The interrupt is enabled */
#define US_IMR_TXBUFE_Pos           11           /**< \brief (US_IMR) Buffer Empty Interrupt Mask */
#define US_IMR_TXBUFE               (0x1u << US_IMR_TXBUFE_Pos)
#define   US_IMR_TXBUFE_0           (0x0u << 11) /**< \brief (US_IMR) The interrupt is disabled */
#define   US_IMR_TXBUFE_1           (0x1u << 11) /**< \brief (US_IMR) The interrupt is enabled */
#define US_IMR_RXBUFF_Pos           12           /**< \brief (US_IMR) Buffer Full Interrupt Mask */
#define US_IMR_RXBUFF               (0x1u << US_IMR_RXBUFF_Pos)
#define   US_IMR_RXBUFF_0           (0x0u << 12) /**< \brief (US_IMR) The interrupt is disabled */
#define   US_IMR_RXBUFF_1           (0x1u << 12) /**< \brief (US_IMR) The interrupt is enabled */
#define US_IMR_NACK_Pos             13           /**< \brief (US_IMR) Non Acknowledge  or LIN Break Sent or LIN Break Received Interrupt Mask */
#define US_IMR_NACK                 (0x1u << US_IMR_NACK_Pos)
#define   US_IMR_NACK_0             (0x0u << 13) /**< \brief (US_IMR) The interrupt is disabled */
#define   US_IMR_NACK_1             (0x1u << 13) /**< \brief (US_IMR) The interrupt is enabled */
#define US_IMR_LINID_Pos            14           /**< \brief (US_IMR) LIN Identifier Sent or LIN Received Interrupt Mask */
#define US_IMR_LINID                (0x1u << US_IMR_LINID_Pos)
#define US_IMR_LINTC_Pos            15           /**< \brief (US_IMR) LIN Transfer Conpleted Interrupt Mask */
#define US_IMR_LINTC                (0x1u << US_IMR_LINTC_Pos)
#define US_IMR_RIIC_Pos             16           /**< \brief (US_IMR) Ring Indicator Input Change Mask */
#define US_IMR_RIIC                 (0x1u << US_IMR_RIIC_Pos)
#define   US_IMR_RIIC_0             (0x0u << 16) /**< \brief (US_IMR) The interrupt is disabled */
#define   US_IMR_RIIC_1             (0x1u << 16) /**< \brief (US_IMR) The interrupt is enabled */
#define US_IMR_DSRIC_Pos            17           /**< \brief (US_IMR) Data Set Ready Input Change Mask */
#define US_IMR_DSRIC                (0x1u << US_IMR_DSRIC_Pos)
#define   US_IMR_DSRIC_0            (0x0u << 17) /**< \brief (US_IMR) The interrupt is disabled */
#define   US_IMR_DSRIC_1            (0x1u << 17) /**< \brief (US_IMR) The interrupt is enabled */
#define US_IMR_DCDIC_Pos            18           /**< \brief (US_IMR) Data Carrier Detect Input Change Interrupt Mask */
#define US_IMR_DCDIC                (0x1u << US_IMR_DCDIC_Pos)
#define   US_IMR_DCDIC_0            (0x0u << 18) /**< \brief (US_IMR) The interrupt is disabled */
#define   US_IMR_DCDIC_1            (0x1u << 18) /**< \brief (US_IMR) The interrupt is enabled */
#define US_IMR_CTSIC_Pos            19           /**< \brief (US_IMR) Clear to Send Input Change Interrupt Mask */
#define US_IMR_CTSIC                (0x1u << US_IMR_CTSIC_Pos)
#define   US_IMR_CTSIC_0            (0x0u << 19) /**< \brief (US_IMR) The interrupt is disabled */
#define   US_IMR_CTSIC_1            (0x1u << 19) /**< \brief (US_IMR) The interrupt is enabled */
#define US_IMR_MANE_Pos             20           /**< \brief (US_IMR)  */
#define US_IMR_MANE                 (0x1u << US_IMR_MANE_Pos)
#define US_IMR_MANEA_Pos            24           /**< \brief (US_IMR) Manchester Error Interrupt Mask */
#define US_IMR_MANEA                (0x1u << US_IMR_MANEA_Pos)
#define   US_IMR_MANEA_0            (0x0u << 24) /**< \brief (US_IMR) The interrupt is disabled */
#define   US_IMR_MANEA_1            (0x1u << 24) /**< \brief (US_IMR) The interrupt is enabled */
#define US_IMR_LINBE_Pos            25           /**< \brief (US_IMR) LIN Bus Error Interrupt Mask */
#define US_IMR_LINBE                (0x1u << US_IMR_LINBE_Pos)
#define US_IMR_LINISFE_Pos          26           /**< \brief (US_IMR) LIN Inconsistent Synch Field Error Interrupt Mask */
#define US_IMR_LINISFE              (0x1u << US_IMR_LINISFE_Pos)
#define US_IMR_LINIPE_Pos           27           /**< \brief (US_IMR) LIN Identifier Parity Interrupt Mask */
#define US_IMR_LINIPE               (0x1u << US_IMR_LINIPE_Pos)
#define US_IMR_LINCE_Pos            28           /**< \brief (US_IMR) LIN Checksum Error Interrupt Mask */
#define US_IMR_LINCE                (0x1u << US_IMR_LINCE_Pos)
#define US_IMR_LINSNRE_Pos          29           /**< \brief (US_IMR) LIN Slave Not Responding Error Interrupt Mask */
#define US_IMR_LINSNRE              (0x1u << US_IMR_LINSNRE_Pos)
#define US_IMR_LINSTE_Pos           30           /**< \brief (US_IMR) LIN Synch Tolerance Error Interrupt Mask */
#define US_IMR_LINSTE               (0x1u << US_IMR_LINSTE_Pos)
#define   US_IMR_LINSTE_0           (0x0u << 30) /**< \brief (US_IMR) The interrupt is disabled */
#define   US_IMR_LINSTE_1           (0x1u << 30) /**< \brief (US_IMR) The interrupt is enabled */
#define US_IMR_LINHTE_Pos           31           /**< \brief (US_IMR) LIN Header Timeout Error Interrupt Mask */
#define US_IMR_LINHTE               (0x1u << US_IMR_LINHTE_Pos)
#define   US_IMR_LINHTE_0           (0x0u << 31) /**< \brief (US_IMR) The interrupt is disabled */
#define   US_IMR_LINHTE_1           (0x1u << 31) /**< \brief (US_IMR) The interrupt is enabled */
#define US_IMR_MASK                 0xFF1FFFFFu  /**< \brief (US_IMR) MASK Register */

/* -------- US_CSR : (USART Offset: 0x14) (R/  32) Channel Status Register -------- */
#define US_CSR_OFFSET               0x14         /**< \brief (US_CSR offset) Channel Status Register */
#define US_CSR_RESETVALUE           0x00000000   /**< \brief (US_CSR reset_value) Channel Status Register */

// LIN mode
#define US_CSR_LIN_RXRDY_Pos        0            /**< \brief (US_CSR_LIN) Receiver Ready */
#define US_CSR_LIN_RXRDY            (0x1u << US_CSR_LIN_RXRDY_Pos)
#define   US_CSR_LIN_RXRDY_0        (0x0u <<  0) /**< \brief (US_CSR_LIN) No complete character has been received since the last read of RHR or the receiver is disabled. If characters werebeing received when the receiver was disabled, RXRDY changes to 1 when the receiver is enabled */
#define   US_CSR_LIN_RXRDY_1        (0x1u <<  0) /**< \brief (US_CSR_LIN) At least one complete character has been received and RHR has not yet been read */
#define US_CSR_LIN_TXRDY_Pos        1            /**< \brief (US_CSR_LIN) Transmitter Ready */
#define US_CSR_LIN_TXRDY            (0x1u << US_CSR_LIN_TXRDY_Pos)
#define   US_CSR_LIN_TXRDY_0        (0x0u <<  1) /**< \brief (US_CSR_LIN) A character is in the THR waiting to be transferred to the Transmit Shift Register, or an STTBRK command has been requested, or the transmitter is disabled. As soon as the transmitter is enabled, TXRDY becomes 1 */
#define   US_CSR_LIN_TXRDY_1        (0x1u <<  1) /**< \brief (US_CSR_LIN) There is no character in the THR */
#define US_CSR_LIN_RXBRK_Pos        2            /**< \brief (US_CSR_LIN) Break Received/End of Break */
#define US_CSR_LIN_RXBRK            (0x1u << US_CSR_LIN_RXBRK_Pos)
#define   US_CSR_LIN_RXBRK_0        (0x0u <<  2) /**< \brief (US_CSR_LIN) No Break received or End of Break detected since the last RSTSTA */
#define   US_CSR_LIN_RXBRK_1        (0x1u <<  2) /**< \brief (US_CSR_LIN) Break Received or End of Break detected since the last RSTSTA */
#define US_CSR_LIN_OVRE_Pos         5            /**< \brief (US_CSR_LIN) Overrun Error */
#define US_CSR_LIN_OVRE             (0x1u << US_CSR_LIN_OVRE_Pos)
#define   US_CSR_LIN_OVRE_0         (0x0u <<  5) /**< \brief (US_CSR_LIN) No overrun error has occurred since since the last RSTSTA */
#define   US_CSR_LIN_OVRE_1         (0x1u <<  5) /**< \brief (US_CSR_LIN) At least one overrun error has occurred since the last RSTSTA */
#define US_CSR_LIN_FRAME_Pos        6            /**< \brief (US_CSR_LIN) Framing Error */
#define US_CSR_LIN_FRAME            (0x1u << US_CSR_LIN_FRAME_Pos)
#define   US_CSR_LIN_FRAME_0        (0x0u <<  6) /**< \brief (US_CSR_LIN) No stop bit has been detected low since the last RSTSTA */
#define   US_CSR_LIN_FRAME_1        (0x1u <<  6) /**< \brief (US_CSR_LIN) At least one stop bit has been detected low since the last RSTSTA */
#define US_CSR_LIN_PARE_Pos         7            /**< \brief (US_CSR_LIN) Parity Error */
#define US_CSR_LIN_PARE             (0x1u << US_CSR_LIN_PARE_Pos)
#define   US_CSR_LIN_PARE_0         (0x0u <<  7) /**< \brief (US_CSR_LIN) No parity error has been detected since the last RSTSTA */
#define   US_CSR_LIN_PARE_1         (0x1u <<  7) /**< \brief (US_CSR_LIN) At least one parity error has been detected since the last RSTSTA */
#define US_CSR_LIN_TIMEOUT_Pos      8            /**< \brief (US_CSR_LIN) Receiver Time-out */
#define US_CSR_LIN_TIMEOUT          (0x1u << US_CSR_LIN_TIMEOUT_Pos)
#define   US_CSR_LIN_TIMEOUT_0      (0x0u <<  8) /**< \brief (US_CSR_LIN) There has not been a time-out since the last Start Time-out command or the Time-out Register is 0 */
#define   US_CSR_LIN_TIMEOUT_1      (0x1u <<  8) /**< \brief (US_CSR_LIN) There has been a time-out since the last Start Time-out command */
#define US_CSR_LIN_TXEMPTY_Pos      9            /**< \brief (US_CSR_LIN) Transmitter Empty */
#define US_CSR_LIN_TXEMPTY          (0x1u << US_CSR_LIN_TXEMPTY_Pos)
#define   US_CSR_LIN_TXEMPTY_0      (0x0u <<  9) /**< \brief (US_CSR_LIN) There are characters in either THR or the Transmit Shift Register, or the transmitter is disabled */
#define   US_CSR_LIN_TXEMPTY_1      (0x1u <<  9) /**< \brief (US_CSR_LIN) There is at least one character in either THR or the Transmit Shift Register */
#define US_CSR_LIN_ITER_Pos         10           /**< \brief (US_CSR_LIN) Max number of Repetitions Reached */
#define US_CSR_LIN_ITER             (0x1u << US_CSR_LIN_ITER_Pos)
#define   US_CSR_LIN_ITER_0         (0x0u << 10) /**< \brief (US_CSR_LIN) Maximum number of repetitions has not been reached since the last RSIT */
#define   US_CSR_LIN_ITER_1         (0x1u << 10) /**< \brief (US_CSR_LIN) Maximum number of repetitions has been reached since the last RSIT */
#define US_CSR_LIN_RXBUFF_Pos       12           /**< \brief (US_CSR_LIN) Reception Buffer Full */
#define US_CSR_LIN_RXBUFF           (0x1u << US_CSR_LIN_RXBUFF_Pos)
#define   US_CSR_LIN_RXBUFF_0       (0x0u << 12) /**< \brief (US_CSR_LIN) The signal Buffer Full from the Receive PDC channel is inactive */
#define   US_CSR_LIN_RXBUFF_1       (0x1u << 12) /**< \brief (US_CSR_LIN) The signal Buffer Full from the Receive PDC channel is active */
#define US_CSR_LIN_NACK_Pos         13           /**< \brief (US_CSR_LIN) Non Acknowledge or LIN Break Sent or LIN Break Received */
#define US_CSR_LIN_NACK             (0x1u << US_CSR_LIN_NACK_Pos)
#define   US_CSR_LIN_NACK_0         (0x0u << 13) /**< \brief (US_CSR_LIN) No Non Acknowledge has not been detected since the last RSTNACK */
#define   US_CSR_LIN_NACK_1         (0x1u << 13) /**< \brief (US_CSR_LIN) At least one Non Acknowledge has been detected since the last RSTNACK */
#define US_CSR_LIN_LINID_Pos        14           /**< \brief (US_CSR_LIN) LIN Identifier Sent or LIN Identifier Received */
#define US_CSR_LIN_LINID            (0x1u << US_CSR_LIN_LINID_Pos)
#define US_CSR_LIN_LINTC_Pos        15           /**< \brief (US_CSR_LIN) LIN Transfer Conpleted */
#define US_CSR_LIN_LINTC            (0x1u << US_CSR_LIN_LINTC_Pos)
#define US_CSR_LIN_RIIC_Pos         16           /**< \brief (US_CSR_LIN) Ring Indicator Input Change Flag */
#define US_CSR_LIN_RIIC             (0x1u << US_CSR_LIN_RIIC_Pos)
#define   US_CSR_LIN_RIIC_0         (0x0u << 16) /**< \brief (US_CSR_LIN) No input change has been detected on the RI pin since the last read of CSR */
#define   US_CSR_LIN_RIIC_1         (0x1u << 16) /**< \brief (US_CSR_LIN) At least one input change has been detected on the RI pin since the last read of CSR */
#define US_CSR_LIN_DSRIC_Pos        17           /**< \brief (US_CSR_LIN) Data Set Ready Input Change Flag */
#define US_CSR_LIN_DSRIC            (0x1u << US_CSR_LIN_DSRIC_Pos)
#define   US_CSR_LIN_DSRIC_0        (0x0u << 17) /**< \brief (US_CSR_LIN) No input change has been detected on the DSR pin since the last read of CSR */
#define   US_CSR_LIN_DSRIC_1        (0x1u << 17) /**< \brief (US_CSR_LIN) At least one input change has been detected on the DSR pin since the last read of CSR */
#define US_CSR_LIN_DCDIC_Pos        18           /**< \brief (US_CSR_LIN) Data Carrier Detect Input Change Flag */
#define US_CSR_LIN_DCDIC            (0x1u << US_CSR_LIN_DCDIC_Pos)
#define   US_CSR_LIN_DCDIC_0        (0x0u << 18) /**< \brief (US_CSR_LIN) No input change has been detected on the DCD pin since the last read of CSR */
#define   US_CSR_LIN_DCDIC_1        (0x1u << 18) /**< \brief (US_CSR_LIN) At least one input change has been detected on the DCD pin since the last read of CSR */
#define US_CSR_LIN_CTSIC_Pos        19           /**< \brief (US_CSR_LIN) Clear to Send Input Change Flag */
#define US_CSR_LIN_CTSIC            (0x1u << US_CSR_LIN_CTSIC_Pos)
#define   US_CSR_LIN_CTSIC_0        (0x0u << 19) /**< \brief (US_CSR_LIN) No input change has been detected on the CTS pin since the last read of CSR */
#define   US_CSR_LIN_CTSIC_1        (0x1u << 19) /**< \brief (US_CSR_LIN) At least one input change has been detected on the CTS pin since the last read of CSR */
#define US_CSR_LIN_RI_Pos           20           /**< \brief (US_CSR_LIN) Image of RI Input */
#define US_CSR_LIN_RI               (0x1u << US_CSR_LIN_RI_Pos)
#define   US_CSR_LIN_RI_0           (0x0u << 20) /**< \brief (US_CSR_LIN) RI is at 0 */
#define   US_CSR_LIN_RI_1           (0x1u << 20) /**< \brief (US_CSR_LIN) RI is at 1 */
#define US_CSR_LIN_DSR_Pos          21           /**< \brief (US_CSR_LIN) Image of DSR Input */
#define US_CSR_LIN_DSR              (0x1u << US_CSR_LIN_DSR_Pos)
#define   US_CSR_LIN_DSR_0          (0x0u << 21) /**< \brief (US_CSR_LIN) DSR is at 0 */
#define   US_CSR_LIN_DSR_1          (0x1u << 21) /**< \brief (US_CSR_LIN) DSR is at 1 */
#define US_CSR_LIN_DCD_Pos          22           /**< \brief (US_CSR_LIN) Image of DCD Input */
#define US_CSR_LIN_DCD              (0x1u << US_CSR_LIN_DCD_Pos)
#define   US_CSR_LIN_DCD_0          (0x0u << 22) /**< \brief (US_CSR_LIN) DCD is at 0 */
#define   US_CSR_LIN_DCD_1          (0x1u << 22) /**< \brief (US_CSR_LIN) DCD is at 1 */
#define US_CSR_LIN_LINBLS_Pos       23           /**< \brief (US_CSR_LIN) LIN Bus Line Status */
#define US_CSR_LIN_LINBLS           (0x1u << US_CSR_LIN_LINBLS_Pos)
#define   US_CSR_LIN_LINBLS_0       (0x0u << 23) /**< \brief (US_CSR_LIN) CTS is at 0 */
#define   US_CSR_LIN_LINBLS_1       (0x1u << 23) /**< \brief (US_CSR_LIN) CTS is at 1 */
#define US_CSR_LIN_LINBE_Pos        25           /**< \brief (US_CSR_LIN) LIN Bit Error */
#define US_CSR_LIN_LINBE            (0x1u << US_CSR_LIN_LINBE_Pos)
#define US_CSR_LIN_LINISFE_Pos      26           /**< \brief (US_CSR_LIN) LIN Inconsistent Synch Field Error */
#define US_CSR_LIN_LINISFE          (0x1u << US_CSR_LIN_LINISFE_Pos)
#define US_CSR_LIN_LINIPE_Pos       27           /**< \brief (US_CSR_LIN) LIN Identifier Parity Error */
#define US_CSR_LIN_LINIPE           (0x1u << US_CSR_LIN_LINIPE_Pos)
#define US_CSR_LIN_LINCE_Pos        28           /**< \brief (US_CSR_LIN) LIN Checksum Error */
#define US_CSR_LIN_LINCE            (0x1u << US_CSR_LIN_LINCE_Pos)
#define US_CSR_LIN_LINSNRE_Pos      29           /**< \brief (US_CSR_LIN) LIN Slave Not Responding Error */
#define US_CSR_LIN_LINSNRE          (0x1u << US_CSR_LIN_LINSNRE_Pos)
#define US_CSR_LIN_LINSTE_Pos       30           /**< \brief (US_CSR_LIN) LIN Synch Tolerance Error */
#define US_CSR_LIN_LINSTE           (0x1u << US_CSR_LIN_LINSTE_Pos)
#define   US_CSR_LIN_LINSTE_0       (0x0u << 30) /**< \brief (US_CSR_LIN) COMM_TX is at 0 */
#define   US_CSR_LIN_LINSTE_1       (0x1u << 30) /**< \brief (US_CSR_LIN) COMM_TX is at 1 */
#define US_CSR_LIN_LINHTE_Pos       31           /**< \brief (US_CSR_LIN) LIN Header Timeout Error */
#define US_CSR_LIN_LINHTE           (0x1u << US_CSR_LIN_LINHTE_Pos)
#define   US_CSR_LIN_LINHTE_0       (0x0u << 31) /**< \brief (US_CSR_LIN) COMM_RX is at 0 */
#define   US_CSR_LIN_LINHTE_1       (0x1u << 31) /**< \brief (US_CSR_LIN) COMM_RX is at 1 */
#define US_CSR_LIN_MASK             0xFEFFFFFFu  /**< \brief (US_CSR_LIN) MASK Register */

// SPI_SLAVE mode
#define US_CSR_SPI_SLAVE_RXRDY_Pos  0            /**< \brief (US_CSR_SPI_SLAVE) Receiver Ready */
#define US_CSR_SPI_SLAVE_RXRDY      (0x1u << US_CSR_SPI_SLAVE_RXRDY_Pos)
#define   US_CSR_SPI_SLAVE_RXRDY_0  (0x0u <<  0) /**< \brief (US_CSR_SPI_SLAVE) No complete character has been received since the last read of RHR or the receiver is disabled. If characters werebeing received when the receiver was disabled, RXRDY changes to 1 when the receiver is enabled */
#define   US_CSR_SPI_SLAVE_RXRDY_1  (0x1u <<  0) /**< \brief (US_CSR_SPI_SLAVE) At least one complete character has been received and RHR has not yet been read */
#define US_CSR_SPI_SLAVE_TXRDY_Pos  1            /**< \brief (US_CSR_SPI_SLAVE) Transmitter Ready */
#define US_CSR_SPI_SLAVE_TXRDY      (0x1u << US_CSR_SPI_SLAVE_TXRDY_Pos)
#define   US_CSR_SPI_SLAVE_TXRDY_0  (0x0u <<  1) /**< \brief (US_CSR_SPI_SLAVE) A character is in the THR waiting to be transferred to the Transmit Shift Register, or an STTBRK command has been requested, or the transmitter is disabled. As soon as the transmitter is enabled, TXRDY becomes 1 */
#define   US_CSR_SPI_SLAVE_TXRDY_1  (0x1u <<  1) /**< \brief (US_CSR_SPI_SLAVE) There is no character in the THR */
#define US_CSR_SPI_SLAVE_RXBRK_Pos  2            /**< \brief (US_CSR_SPI_SLAVE) Break Received/End of Break */
#define US_CSR_SPI_SLAVE_RXBRK      (0x1u << US_CSR_SPI_SLAVE_RXBRK_Pos)
#define   US_CSR_SPI_SLAVE_RXBRK_0  (0x0u <<  2) /**< \brief (US_CSR_SPI_SLAVE) No Break received or End of Break detected since the last RSTSTA */
#define   US_CSR_SPI_SLAVE_RXBRK_1  (0x1u <<  2) /**< \brief (US_CSR_SPI_SLAVE) Break Received or End of Break detected since the last RSTSTA */
#define US_CSR_SPI_SLAVE_OVRE_Pos   5            /**< \brief (US_CSR_SPI_SLAVE) Overrun Error */
#define US_CSR_SPI_SLAVE_OVRE       (0x1u << US_CSR_SPI_SLAVE_OVRE_Pos)
#define   US_CSR_SPI_SLAVE_OVRE_0   (0x0u <<  5) /**< \brief (US_CSR_SPI_SLAVE) No overrun error has occurred since since the last RSTSTA */
#define   US_CSR_SPI_SLAVE_OVRE_1   (0x1u <<  5) /**< \brief (US_CSR_SPI_SLAVE) At least one overrun error has occurred since the last RSTSTA */
#define US_CSR_SPI_SLAVE_FRAME_Pos  6            /**< \brief (US_CSR_SPI_SLAVE) Framing Error */
#define US_CSR_SPI_SLAVE_FRAME      (0x1u << US_CSR_SPI_SLAVE_FRAME_Pos)
#define   US_CSR_SPI_SLAVE_FRAME_0  (0x0u <<  6) /**< \brief (US_CSR_SPI_SLAVE) No stop bit has been detected low since the last RSTSTA */
#define   US_CSR_SPI_SLAVE_FRAME_1  (0x1u <<  6) /**< \brief (US_CSR_SPI_SLAVE) At least one stop bit has been detected low since the last RSTSTA */
#define US_CSR_SPI_SLAVE_PARE_Pos   7            /**< \brief (US_CSR_SPI_SLAVE) Parity Error */
#define US_CSR_SPI_SLAVE_PARE       (0x1u << US_CSR_SPI_SLAVE_PARE_Pos)
#define   US_CSR_SPI_SLAVE_PARE_0   (0x0u <<  7) /**< \brief (US_CSR_SPI_SLAVE) No parity error has been detected since the last RSTSTA */
#define   US_CSR_SPI_SLAVE_PARE_1   (0x1u <<  7) /**< \brief (US_CSR_SPI_SLAVE) At least one parity error has been detected since the last RSTSTA */
#define US_CSR_SPI_SLAVE_TIMEOUT_Pos 8            /**< \brief (US_CSR_SPI_SLAVE) Receiver Time-out */
#define US_CSR_SPI_SLAVE_TIMEOUT    (0x1u << US_CSR_SPI_SLAVE_TIMEOUT_Pos)
#define   US_CSR_SPI_SLAVE_TIMEOUT_0 (0x0u <<  8) /**< \brief (US_CSR_SPI_SLAVE) There has not been a time-out since the last Start Time-out command or the Time-out Register is 0 */
#define   US_CSR_SPI_SLAVE_TIMEOUT_1 (0x1u <<  8) /**< \brief (US_CSR_SPI_SLAVE) There has been a time-out since the last Start Time-out command */
#define US_CSR_SPI_SLAVE_TXEMPTY_Pos 9            /**< \brief (US_CSR_SPI_SLAVE) Transmitter Empty */
#define US_CSR_SPI_SLAVE_TXEMPTY    (0x1u << US_CSR_SPI_SLAVE_TXEMPTY_Pos)
#define   US_CSR_SPI_SLAVE_TXEMPTY_0 (0x0u <<  9) /**< \brief (US_CSR_SPI_SLAVE) There are characters in either THR or the Transmit Shift Register, or the transmitter is disabled */
#define   US_CSR_SPI_SLAVE_TXEMPTY_1 (0x1u <<  9) /**< \brief (US_CSR_SPI_SLAVE) There is at least one character in either THR or the Transmit Shift Register */
#define US_CSR_SPI_SLAVE_UNRE_Pos   10           /**< \brief (US_CSR_SPI_SLAVE) SPI Underrun Error */
#define US_CSR_SPI_SLAVE_UNRE       (0x1u << US_CSR_SPI_SLAVE_UNRE_Pos)
#define   US_CSR_SPI_SLAVE_UNRE_0   (0x0u << 10) /**< \brief (US_CSR_SPI_SLAVE) No SPI underrun error has occurred since the last RSTSTA */
#define   US_CSR_SPI_SLAVE_UNRE_1   (0x1u << 10) /**< \brief (US_CSR_SPI_SLAVE) At least one SPI underrun error has occurred since the last RSTSTA */
#define US_CSR_SPI_SLAVE_NACK_Pos   13           /**< \brief (US_CSR_SPI_SLAVE) Non Acknowledge */
#define US_CSR_SPI_SLAVE_RXBUFF_Pos 12           /**< \brief (US_CSR_SPI_SLAVE) Reception Buffer Full */
#define US_CSR_SPI_SLAVE_RXBUFF     (0x1u << US_CSR_SPI_SLAVE_RXBUFF_Pos)
#define   US_CSR_SPI_SLAVE_RXBUFF_0 (0x0u << 12) /**< \brief (US_CSR_SPI_SLAVE) The signal Buffer Full from the Receive PDC channel is inactive */
#define   US_CSR_SPI_SLAVE_RXBUFF_1 (0x1u << 12) /**< \brief (US_CSR_SPI_SLAVE) The signal Buffer Full from the Receive PDC channel is active */
#define US_CSR_SPI_SLAVE_NACK       (0x1u << US_CSR_SPI_SLAVE_NACK_Pos)
#define   US_CSR_SPI_SLAVE_NACK_0   (0x0u << 13) /**< \brief (US_CSR_SPI_SLAVE) No Non Acknowledge has not been detected since the last RSTNACK */
#define   US_CSR_SPI_SLAVE_NACK_1   (0x1u << 13) /**< \brief (US_CSR_SPI_SLAVE) At least one Non Acknowledge has been detected since the last RSTNACK */
#define US_CSR_SPI_SLAVE_RIIC_Pos   16           /**< \brief (US_CSR_SPI_SLAVE) Ring Indicator Input Change Flag */
#define US_CSR_SPI_SLAVE_RIIC       (0x1u << US_CSR_SPI_SLAVE_RIIC_Pos)
#define   US_CSR_SPI_SLAVE_RIIC_0   (0x0u << 16) /**< \brief (US_CSR_SPI_SLAVE) No input change has been detected on the RI pin since the last read of CSR */
#define   US_CSR_SPI_SLAVE_RIIC_1   (0x1u << 16) /**< \brief (US_CSR_SPI_SLAVE) At least one input change has been detected on the RI pin since the last read of CSR */
#define US_CSR_SPI_SLAVE_DSRIC_Pos  17           /**< \brief (US_CSR_SPI_SLAVE) Data Set Ready Input Change Flag */
#define US_CSR_SPI_SLAVE_DSRIC      (0x1u << US_CSR_SPI_SLAVE_DSRIC_Pos)
#define   US_CSR_SPI_SLAVE_DSRIC_0  (0x0u << 17) /**< \brief (US_CSR_SPI_SLAVE) No input change has been detected on the DSR pin since the last read of CSR */
#define   US_CSR_SPI_SLAVE_DSRIC_1  (0x1u << 17) /**< \brief (US_CSR_SPI_SLAVE) At least one input change has been detected on the DSR pin since the last read of CSR */
#define US_CSR_SPI_SLAVE_DCDIC_Pos  18           /**< \brief (US_CSR_SPI_SLAVE) Data Carrier Detect Input Change Flag */
#define US_CSR_SPI_SLAVE_DCDIC      (0x1u << US_CSR_SPI_SLAVE_DCDIC_Pos)
#define   US_CSR_SPI_SLAVE_DCDIC_0  (0x0u << 18) /**< \brief (US_CSR_SPI_SLAVE) No input change has been detected on the DCD pin since the last read of CSR */
#define   US_CSR_SPI_SLAVE_DCDIC_1  (0x1u << 18) /**< \brief (US_CSR_SPI_SLAVE) At least one input change has been detected on the DCD pin since the last read of CSR */
#define US_CSR_SPI_SLAVE_CTSIC_Pos  19           /**< \brief (US_CSR_SPI_SLAVE) Clear to Send Input Change Flag */
#define US_CSR_SPI_SLAVE_CTSIC      (0x1u << US_CSR_SPI_SLAVE_CTSIC_Pos)
#define   US_CSR_SPI_SLAVE_CTSIC_0  (0x0u << 19) /**< \brief (US_CSR_SPI_SLAVE) No input change has been detected on the CTS pin since the last read of CSR */
#define   US_CSR_SPI_SLAVE_CTSIC_1  (0x1u << 19) /**< \brief (US_CSR_SPI_SLAVE) At least one input change has been detected on the CTS pin since the last read of CSR */
#define US_CSR_SPI_SLAVE_RI_Pos     20           /**< \brief (US_CSR_SPI_SLAVE) Image of RI Input */
#define US_CSR_SPI_SLAVE_RI         (0x1u << US_CSR_SPI_SLAVE_RI_Pos)
#define   US_CSR_SPI_SLAVE_RI_0     (0x0u << 20) /**< \brief (US_CSR_SPI_SLAVE) RI is at 0 */
#define   US_CSR_SPI_SLAVE_RI_1     (0x1u << 20) /**< \brief (US_CSR_SPI_SLAVE) RI is at 1 */
#define US_CSR_SPI_SLAVE_DSR_Pos    21           /**< \brief (US_CSR_SPI_SLAVE) Image of DSR Input */
#define US_CSR_SPI_SLAVE_DSR        (0x1u << US_CSR_SPI_SLAVE_DSR_Pos)
#define   US_CSR_SPI_SLAVE_DSR_0    (0x0u << 21) /**< \brief (US_CSR_SPI_SLAVE) DSR is at 0 */
#define   US_CSR_SPI_SLAVE_DSR_1    (0x1u << 21) /**< \brief (US_CSR_SPI_SLAVE) DSR is at 1 */
#define US_CSR_SPI_SLAVE_DCD_Pos    22           /**< \brief (US_CSR_SPI_SLAVE) Image of DCD Input */
#define US_CSR_SPI_SLAVE_DCD        (0x1u << US_CSR_SPI_SLAVE_DCD_Pos)
#define   US_CSR_SPI_SLAVE_DCD_0    (0x0u << 22) /**< \brief (US_CSR_SPI_SLAVE) DCD is at 0 */
#define   US_CSR_SPI_SLAVE_DCD_1    (0x1u << 22) /**< \brief (US_CSR_SPI_SLAVE) DCD is at 1 */
#define US_CSR_SPI_SLAVE_CTS_Pos    23           /**< \brief (US_CSR_SPI_SLAVE) Image of CTS Input */
#define US_CSR_SPI_SLAVE_CTS        (0x1u << US_CSR_SPI_SLAVE_CTS_Pos)
#define   US_CSR_SPI_SLAVE_CTS_0    (0x0u << 23) /**< \brief (US_CSR_SPI_SLAVE) CTS is at 0 */
#define   US_CSR_SPI_SLAVE_CTS_1    (0x1u << 23) /**< \brief (US_CSR_SPI_SLAVE) CTS is at 1 */
#define US_CSR_SPI_SLAVE_MASK       0x00FF3FFFu  /**< \brief (US_CSR_SPI_SLAVE) MASK Register */

// USART mode
#define US_CSR_USART_RXRDY_Pos      0            /**< \brief (US_CSR_USART) Receiver Ready */
#define US_CSR_USART_RXRDY          (0x1u << US_CSR_USART_RXRDY_Pos)
#define   US_CSR_USART_RXRDY_0      (0x0u <<  0) /**< \brief (US_CSR_USART) No complete character has been received since the last read of RHR or the receiver is disabled. If characters werebeing received when the receiver was disabled, RXRDY changes to 1 when the receiver is enabled */
#define   US_CSR_USART_RXRDY_1      (0x1u <<  0) /**< \brief (US_CSR_USART) At least one complete character has been received and RHR has not yet been read */
#define US_CSR_USART_TXRDY_Pos      1            /**< \brief (US_CSR_USART) Transmitter Ready */
#define US_CSR_USART_TXRDY          (0x1u << US_CSR_USART_TXRDY_Pos)
#define   US_CSR_USART_TXRDY_0      (0x0u <<  1) /**< \brief (US_CSR_USART) A character is in the THR waiting to be transferred to the Transmit Shift Register, or an STTBRK command has been requested, or the transmitter is disabled. As soon as the transmitter is enabled, TXRDY becomes 1 */
#define   US_CSR_USART_TXRDY_1      (0x1u <<  1) /**< \brief (US_CSR_USART) There is no character in the THR */
#define US_CSR_USART_RXBRK_Pos      2            /**< \brief (US_CSR_USART) Break Received/End of Break */
#define US_CSR_USART_RXBRK          (0x1u << US_CSR_USART_RXBRK_Pos)
#define   US_CSR_USART_RXBRK_0      (0x0u <<  2) /**< \brief (US_CSR_USART) No Break received or End of Break detected since the last RSTSTA */
#define   US_CSR_USART_RXBRK_1      (0x1u <<  2) /**< \brief (US_CSR_USART) Break Received or End of Break detected since the last RSTSTA */
#define US_CSR_USART_OVRE_Pos       5            /**< \brief (US_CSR_USART) Overrun Error */
#define US_CSR_USART_OVRE           (0x1u << US_CSR_USART_OVRE_Pos)
#define   US_CSR_USART_OVRE_0       (0x0u <<  5) /**< \brief (US_CSR_USART) No overrun error has occurred since since the last RSTSTA */
#define   US_CSR_USART_OVRE_1       (0x1u <<  5) /**< \brief (US_CSR_USART) At least one overrun error has occurred since the last RSTSTA */
#define US_CSR_USART_FRAME_Pos      6            /**< \brief (US_CSR_USART) Framing Error */
#define US_CSR_USART_FRAME          (0x1u << US_CSR_USART_FRAME_Pos)
#define   US_CSR_USART_FRAME_0      (0x0u <<  6) /**< \brief (US_CSR_USART) No stop bit has been detected low since the last RSTSTA */
#define   US_CSR_USART_FRAME_1      (0x1u <<  6) /**< \brief (US_CSR_USART) At least one stop bit has been detected low since the last RSTSTA */
#define US_CSR_USART_PARE_Pos       7            /**< \brief (US_CSR_USART) Parity Error */
#define US_CSR_USART_PARE           (0x1u << US_CSR_USART_PARE_Pos)
#define   US_CSR_USART_PARE_0       (0x0u <<  7) /**< \brief (US_CSR_USART) No parity error has been detected since the last RSTSTA */
#define   US_CSR_USART_PARE_1       (0x1u <<  7) /**< \brief (US_CSR_USART) At least one parity error has been detected since the last RSTSTA */
#define US_CSR_USART_TIMEOUT_Pos    8            /**< \brief (US_CSR_USART) Receiver Time-out */
#define US_CSR_USART_TIMEOUT        (0x1u << US_CSR_USART_TIMEOUT_Pos)
#define   US_CSR_USART_TIMEOUT_0    (0x0u <<  8) /**< \brief (US_CSR_USART) There has not been a time-out since the last Start Time-out command or the Time-out Register is 0 */
#define   US_CSR_USART_TIMEOUT_1    (0x1u <<  8) /**< \brief (US_CSR_USART) There has been a time-out since the last Start Time-out command */
#define US_CSR_USART_TXEMPTY_Pos    9            /**< \brief (US_CSR_USART) Transmitter Empty */
#define US_CSR_USART_TXEMPTY        (0x1u << US_CSR_USART_TXEMPTY_Pos)
#define   US_CSR_USART_TXEMPTY_0    (0x0u <<  9) /**< \brief (US_CSR_USART) There are characters in either THR or the Transmit Shift Register, or the transmitter is disabled */
#define   US_CSR_USART_TXEMPTY_1    (0x1u <<  9) /**< \brief (US_CSR_USART) There is at least one character in either THR or the Transmit Shift Register */
#define US_CSR_USART_ITER_Pos       10           /**< \brief (US_CSR_USART) Max number of Repetitions Reached */
#define US_CSR_USART_ITER           (0x1u << US_CSR_USART_ITER_Pos)
#define   US_CSR_USART_ITER_0       (0x0u << 10) /**< \brief (US_CSR_USART) Maximum number of repetitions has not been reached since the last RSIT */
#define   US_CSR_USART_ITER_1       (0x1u << 10) /**< \brief (US_CSR_USART) Maximum number of repetitions has been reached since the last RSIT */
#define US_CSR_USART_RXBUFF_Pos     12           /**< \brief (US_CSR_USART) Reception Buffer Full */
#define US_CSR_USART_RXBUFF         (0x1u << US_CSR_USART_RXBUFF_Pos)
#define   US_CSR_USART_RXBUFF_0     (0x0u << 12) /**< \brief (US_CSR_USART) The signal Buffer Full from the Receive PDC channel is inactive */
#define   US_CSR_USART_RXBUFF_1     (0x1u << 12) /**< \brief (US_CSR_USART) The signal Buffer Full from the Receive PDC channel is active */
#define US_CSR_USART_NACK_Pos       13           /**< \brief (US_CSR_USART) Non Acknowledge */
#define US_CSR_USART_NACK           (0x1u << US_CSR_USART_NACK_Pos)
#define   US_CSR_USART_NACK_0       (0x0u << 13) /**< \brief (US_CSR_USART) No Non Acknowledge has not been detected since the last RSTNACK */
#define   US_CSR_USART_NACK_1       (0x1u << 13) /**< \brief (US_CSR_USART) At least one Non Acknowledge has been detected since the last RSTNACK */
#define US_CSR_USART_RIIC_Pos       16           /**< \brief (US_CSR_USART) Ring Indicator Input Change Flag */
#define US_CSR_USART_RIIC           (0x1u << US_CSR_USART_RIIC_Pos)
#define   US_CSR_USART_RIIC_0       (0x0u << 16) /**< \brief (US_CSR_USART) No input change has been detected on the RI pin since the last read of CSR */
#define   US_CSR_USART_RIIC_1       (0x1u << 16) /**< \brief (US_CSR_USART) At least one input change has been detected on the RI pin since the last read of CSR */
#define US_CSR_USART_DSRIC_Pos      17           /**< \brief (US_CSR_USART) Data Set Ready Input Change Flag */
#define US_CSR_USART_DSRIC          (0x1u << US_CSR_USART_DSRIC_Pos)
#define   US_CSR_USART_DSRIC_0      (0x0u << 17) /**< \brief (US_CSR_USART) No input change has been detected on the DSR pin since the last read of CSR */
#define   US_CSR_USART_DSRIC_1      (0x1u << 17) /**< \brief (US_CSR_USART) At least one input change has been detected on the DSR pin since the last read of CSR */
#define US_CSR_USART_DCDIC_Pos      18           /**< \brief (US_CSR_USART) Data Carrier Detect Input Change Flag */
#define US_CSR_USART_DCDIC          (0x1u << US_CSR_USART_DCDIC_Pos)
#define   US_CSR_USART_DCDIC_0      (0x0u << 18) /**< \brief (US_CSR_USART) No input change has been detected on the DCD pin since the last read of CSR */
#define   US_CSR_USART_DCDIC_1      (0x1u << 18) /**< \brief (US_CSR_USART) At least one input change has been detected on the DCD pin since the last read of CSR */
#define US_CSR_USART_CTSIC_Pos      19           /**< \brief (US_CSR_USART) Clear to Send Input Change Flag */
#define US_CSR_USART_CTSIC          (0x1u << US_CSR_USART_CTSIC_Pos)
#define   US_CSR_USART_CTSIC_0      (0x0u << 19) /**< \brief (US_CSR_USART) No input change has been detected on the CTS pin since the last read of CSR */
#define   US_CSR_USART_CTSIC_1      (0x1u << 19) /**< \brief (US_CSR_USART) At least one input change has been detected on the CTS pin since the last read of CSR */
#define US_CSR_USART_RI_Pos         20           /**< \brief (US_CSR_USART) Image of RI Input */
#define US_CSR_USART_RI             (0x1u << US_CSR_USART_RI_Pos)
#define   US_CSR_USART_RI_0         (0x0u << 20) /**< \brief (US_CSR_USART) RI is at 0 */
#define   US_CSR_USART_RI_1         (0x1u << 20) /**< \brief (US_CSR_USART) RI is at 1 */
#define US_CSR_USART_DSR_Pos        21           /**< \brief (US_CSR_USART) Image of DSR Input */
#define US_CSR_USART_DSR            (0x1u << US_CSR_USART_DSR_Pos)
#define   US_CSR_USART_DSR_0        (0x0u << 21) /**< \brief (US_CSR_USART) DSR is at 0 */
#define   US_CSR_USART_DSR_1        (0x1u << 21) /**< \brief (US_CSR_USART) DSR is at 1 */
#define US_CSR_USART_DCD_Pos        22           /**< \brief (US_CSR_USART) Image of DCD Input */
#define US_CSR_USART_DCD            (0x1u << US_CSR_USART_DCD_Pos)
#define   US_CSR_USART_DCD_0        (0x0u << 22) /**< \brief (US_CSR_USART) DCD is at 0 */
#define   US_CSR_USART_DCD_1        (0x1u << 22) /**< \brief (US_CSR_USART) DCD is at 1 */
#define US_CSR_USART_CTS_Pos        23           /**< \brief (US_CSR_USART) Image of CTS Input */
#define US_CSR_USART_CTS            (0x1u << US_CSR_USART_CTS_Pos)
#define   US_CSR_USART_CTS_0        (0x0u << 23) /**< \brief (US_CSR_USART) CTS is at 0 */
#define   US_CSR_USART_CTS_1        (0x1u << 23) /**< \brief (US_CSR_USART) CTS is at 1 */
#define US_CSR_USART_MANERR_Pos     24           /**< \brief (US_CSR_USART) Manchester Error */
#define US_CSR_USART_MANERR         (0x1u << US_CSR_USART_MANERR_Pos)
#define   US_CSR_USART_MANERR_0     (0x0u << 24) /**< \brief (US_CSR_USART) No Manchester error has been detected since the last RSTSTA */
#define   US_CSR_USART_MANERR_1     (0x1u << 24) /**< \brief (US_CSR_USART) At least one Manchester error has been detected since the last RSTSTA */
#define US_CSR_USART_MASK           0x01FF3FFFu  /**< \brief (US_CSR_USART) MASK Register */

// Any mode
#define US_CSR_RXRDY_Pos            0            /**< \brief (US_CSR) Receiver Ready */
#define US_CSR_RXRDY                (0x1u << US_CSR_RXRDY_Pos)
#define   US_CSR_RXRDY_0            (0x0u <<  0) /**< \brief (US_CSR) No complete character has been received since the last read of RHR or the receiver is disabled. If characters werebeing received when the receiver was disabled, RXRDY changes to 1 when the receiver is enabled */
#define   US_CSR_RXRDY_1            (0x1u <<  0) /**< \brief (US_CSR) At least one complete character has been received and RHR has not yet been read */
#define US_CSR_TXRDY_Pos            1            /**< \brief (US_CSR) Transmitter Ready */
#define US_CSR_TXRDY                (0x1u << US_CSR_TXRDY_Pos)
#define   US_CSR_TXRDY_0            (0x0u <<  1) /**< \brief (US_CSR) A character is in the THR waiting to be transferred to the Transmit Shift Register, or an STTBRK command has been requested, or the transmitter is disabled. As soon as the transmitter is enabled, TXRDY becomes 1 */
#define   US_CSR_TXRDY_1            (0x1u <<  1) /**< \brief (US_CSR) There is no character in the THR */
#define US_CSR_RXBRK_Pos            2            /**< \brief (US_CSR) Break Received/End of Break */
#define US_CSR_RXBRK                (0x1u << US_CSR_RXBRK_Pos)
#define   US_CSR_RXBRK_0            (0x0u <<  2) /**< \brief (US_CSR) No Break received or End of Break detected since the last RSTSTA */
#define   US_CSR_RXBRK_1            (0x1u <<  2) /**< \brief (US_CSR) Break Received or End of Break detected since the last RSTSTA */
#define US_CSR_OVRE_Pos             5            /**< \brief (US_CSR) Overrun Error */
#define US_CSR_OVRE                 (0x1u << US_CSR_OVRE_Pos)
#define   US_CSR_OVRE_0             (0x0u <<  5) /**< \brief (US_CSR) No overrun error has occurred since since the last RSTSTA */
#define   US_CSR_OVRE_1             (0x1u <<  5) /**< \brief (US_CSR) At least one overrun error has occurred since the last RSTSTA */
#define US_CSR_FRAME_Pos            6            /**< \brief (US_CSR) Framing Error */
#define US_CSR_FRAME                (0x1u << US_CSR_FRAME_Pos)
#define   US_CSR_FRAME_0            (0x0u <<  6) /**< \brief (US_CSR) No stop bit has been detected low since the last RSTSTA */
#define   US_CSR_FRAME_1            (0x1u <<  6) /**< \brief (US_CSR) At least one stop bit has been detected low since the last RSTSTA */
#define US_CSR_PARE_Pos             7            /**< \brief (US_CSR) Parity Error */
#define US_CSR_PARE                 (0x1u << US_CSR_PARE_Pos)
#define   US_CSR_PARE_0             (0x0u <<  7) /**< \brief (US_CSR) No parity error has been detected since the last RSTSTA */
#define   US_CSR_PARE_1             (0x1u <<  7) /**< \brief (US_CSR) At least one parity error has been detected since the last RSTSTA */
#define US_CSR_TIMEOUT_Pos          8            /**< \brief (US_CSR) Receiver Time-out */
#define US_CSR_TIMEOUT              (0x1u << US_CSR_TIMEOUT_Pos)
#define   US_CSR_TIMEOUT_0          (0x0u <<  8) /**< \brief (US_CSR) There has not been a time-out since the last Start Time-out command or the Time-out Register is 0 */
#define   US_CSR_TIMEOUT_1          (0x1u <<  8) /**< \brief (US_CSR) There has been a time-out since the last Start Time-out command */
#define US_CSR_TXEMPTY_Pos          9            /**< \brief (US_CSR) Transmitter Empty */
#define US_CSR_TXEMPTY              (0x1u << US_CSR_TXEMPTY_Pos)
#define   US_CSR_TXEMPTY_0          (0x0u <<  9) /**< \brief (US_CSR) There are characters in either THR or the Transmit Shift Register, or the transmitter is disabled */
#define   US_CSR_TXEMPTY_1          (0x1u <<  9) /**< \brief (US_CSR) There is at least one character in either THR or the Transmit Shift Register */
#define US_CSR_ITER_Pos             10           /**< \brief (US_CSR) Max number of Repetitions Reached */
#define US_CSR_ITER                 (0x1u << US_CSR_ITER_Pos)
#define   US_CSR_ITER_0             (0x0u << 10) /**< \brief (US_CSR) Maximum number of repetitions has not been reached since the last RSIT */
#define   US_CSR_ITER_1             (0x1u << 10) /**< \brief (US_CSR) Maximum number of repetitions has been reached since the last RSIT */
#define US_CSR_UNRE_Pos             10           /**< \brief (US_CSR) SPI Underrun Error */
#define US_CSR_UNRE                 (0x1u << US_CSR_UNRE_Pos)
#define   US_CSR_UNRE_0             (0x0u << 10) /**< \brief (US_CSR) No SPI underrun error has occurred since the last RSTSTA */
#define   US_CSR_UNRE_1             (0x1u << 10) /**< \brief (US_CSR) At least one SPI underrun error has occurred since the last RSTSTA */
#define US_CSR_ITER_Pos             10           /**< \brief (US_CSR) Max number of Repetitions Reached */
#define US_CSR_ITER                 (0x1u << US_CSR_ITER_Pos)
#define   US_CSR_ITER_0             (0x0u << 10) /**< \brief (US_CSR) Maximum number of repetitions has not been reached since the last RSIT */
#define   US_CSR_ITER_1             (0x1u << 10) /**< \brief (US_CSR) Maximum number of repetitions has been reached since the last RSIT */
#define US_CSR_RXBUFF_Pos           12           /**< \brief (US_CSR) Reception Buffer Full */
#define US_CSR_RXBUFF               (0x1u << US_CSR_RXBUFF_Pos)
#define   US_CSR_RXBUFF_0           (0x0u << 12) /**< \brief (US_CSR) The signal Buffer Full from the Receive PDC channel is inactive */
#define   US_CSR_RXBUFF_1           (0x1u << 12) /**< \brief (US_CSR) The signal Buffer Full from the Receive PDC channel is active */
#define US_CSR_NACK_Pos             13           /**< \brief (US_CSR) Non Acknowledge or LIN Break Sent or LIN Break Received */
#define US_CSR_NACK                 (0x1u << US_CSR_NACK_Pos)
#define   US_CSR_NACK_0             (0x0u << 13) /**< \brief (US_CSR) No Non Acknowledge has not been detected since the last RSTNACK */
#define   US_CSR_NACK_1             (0x1u << 13) /**< \brief (US_CSR) At least one Non Acknowledge has been detected since the last RSTNACK */
#define US_CSR_LINID_Pos            14           /**< \brief (US_CSR) LIN Identifier Sent or LIN Identifier Received */
#define US_CSR_LINID                (0x1u << US_CSR_LINID_Pos)
#define US_CSR_LINTC_Pos            15           /**< \brief (US_CSR) LIN Transfer Conpleted */
#define US_CSR_LINTC                (0x1u << US_CSR_LINTC_Pos)
#define US_CSR_RIIC_Pos             16           /**< \brief (US_CSR) Ring Indicator Input Change Flag */
#define US_CSR_RIIC                 (0x1u << US_CSR_RIIC_Pos)
#define   US_CSR_RIIC_0             (0x0u << 16) /**< \brief (US_CSR) No input change has been detected on the RI pin since the last read of CSR */
#define   US_CSR_RIIC_1             (0x1u << 16) /**< \brief (US_CSR) At least one input change has been detected on the RI pin since the last read of CSR */
#define US_CSR_DSRIC_Pos            17           /**< \brief (US_CSR) Data Set Ready Input Change Flag */
#define US_CSR_DSRIC                (0x1u << US_CSR_DSRIC_Pos)
#define   US_CSR_DSRIC_0            (0x0u << 17) /**< \brief (US_CSR) No input change has been detected on the DSR pin since the last read of CSR */
#define   US_CSR_DSRIC_1            (0x1u << 17) /**< \brief (US_CSR) At least one input change has been detected on the DSR pin since the last read of CSR */
#define US_CSR_DCDIC_Pos            18           /**< \brief (US_CSR) Data Carrier Detect Input Change Flag */
#define US_CSR_DCDIC                (0x1u << US_CSR_DCDIC_Pos)
#define   US_CSR_DCDIC_0            (0x0u << 18) /**< \brief (US_CSR) No input change has been detected on the DCD pin since the last read of CSR */
#define   US_CSR_DCDIC_1            (0x1u << 18) /**< \brief (US_CSR) At least one input change has been detected on the DCD pin since the last read of CSR */
#define US_CSR_CTSIC_Pos            19           /**< \brief (US_CSR) Clear to Send Input Change Flag */
#define US_CSR_CTSIC                (0x1u << US_CSR_CTSIC_Pos)
#define   US_CSR_CTSIC_0            (0x0u << 19) /**< \brief (US_CSR) No input change has been detected on the CTS pin since the last read of CSR */
#define   US_CSR_CTSIC_1            (0x1u << 19) /**< \brief (US_CSR) At least one input change has been detected on the CTS pin since the last read of CSR */
#define US_CSR_RI_Pos               20           /**< \brief (US_CSR) Image of RI Input */
#define US_CSR_RI                   (0x1u << US_CSR_RI_Pos)
#define   US_CSR_RI_0               (0x0u << 20) /**< \brief (US_CSR) RI is at 0 */
#define   US_CSR_RI_1               (0x1u << 20) /**< \brief (US_CSR) RI is at 1 */
#define US_CSR_DSR_Pos              21           /**< \brief (US_CSR) Image of DSR Input */
#define US_CSR_DSR                  (0x1u << US_CSR_DSR_Pos)
#define   US_CSR_DSR_0              (0x0u << 21) /**< \brief (US_CSR) DSR is at 0 */
#define   US_CSR_DSR_1              (0x1u << 21) /**< \brief (US_CSR) DSR is at 1 */
#define US_CSR_DCD_Pos              22           /**< \brief (US_CSR) Image of DCD Input */
#define US_CSR_DCD                  (0x1u << US_CSR_DCD_Pos)
#define   US_CSR_DCD_0              (0x0u << 22) /**< \brief (US_CSR) DCD is at 0 */
#define   US_CSR_DCD_1              (0x1u << 22) /**< \brief (US_CSR) DCD is at 1 */
#define US_CSR_LINBLS_Pos           23           /**< \brief (US_CSR) LIN Bus Line Status */
#define US_CSR_LINBLS               (0x1u << US_CSR_LINBLS_Pos)
#define   US_CSR_LINBLS_0           (0x0u << 23) /**< \brief (US_CSR) CTS is at 0 */
#define   US_CSR_LINBLS_1           (0x1u << 23) /**< \brief (US_CSR) CTS is at 1 */
#define US_CSR_CTS_Pos              23           /**< \brief (US_CSR) Image of CTS Input */
#define US_CSR_CTS                  (0x1u << US_CSR_CTS_Pos)
#define   US_CSR_CTS_0              (0x0u << 23) /**< \brief (US_CSR) CTS is at 0 */
#define   US_CSR_CTS_1              (0x1u << 23) /**< \brief (US_CSR) CTS is at 1 */
#define US_CSR_MANERR_Pos           24           /**< \brief (US_CSR) Manchester Error */
#define US_CSR_MANERR               (0x1u << US_CSR_MANERR_Pos)
#define   US_CSR_MANERR_0           (0x0u << 24) /**< \brief (US_CSR) No Manchester error has been detected since the last RSTSTA */
#define   US_CSR_MANERR_1           (0x1u << 24) /**< \brief (US_CSR) At least one Manchester error has been detected since the last RSTSTA */
#define US_CSR_LINBE_Pos            25           /**< \brief (US_CSR) LIN Bit Error */
#define US_CSR_LINBE                (0x1u << US_CSR_LINBE_Pos)
#define US_CSR_LINISFE_Pos          26           /**< \brief (US_CSR) LIN Inconsistent Synch Field Error */
#define US_CSR_LINISFE              (0x1u << US_CSR_LINISFE_Pos)
#define US_CSR_LINIPE_Pos           27           /**< \brief (US_CSR) LIN Identifier Parity Error */
#define US_CSR_LINIPE               (0x1u << US_CSR_LINIPE_Pos)
#define US_CSR_LINCE_Pos            28           /**< \brief (US_CSR) LIN Checksum Error */
#define US_CSR_LINCE                (0x1u << US_CSR_LINCE_Pos)
#define US_CSR_LINSNRE_Pos          29           /**< \brief (US_CSR) LIN Slave Not Responding Error */
#define US_CSR_LINSNRE              (0x1u << US_CSR_LINSNRE_Pos)
#define US_CSR_LINSTE_Pos           30           /**< \brief (US_CSR) LIN Synch Tolerance Error */
#define US_CSR_LINSTE               (0x1u << US_CSR_LINSTE_Pos)
#define   US_CSR_LINSTE_0           (0x0u << 30) /**< \brief (US_CSR) COMM_TX is at 0 */
#define   US_CSR_LINSTE_1           (0x1u << 30) /**< \brief (US_CSR) COMM_TX is at 1 */
#define US_CSR_LINHTE_Pos           31           /**< \brief (US_CSR) LIN Header Timeout Error */
#define US_CSR_LINHTE               (0x1u << US_CSR_LINHTE_Pos)
#define   US_CSR_LINHTE_0           (0x0u << 31) /**< \brief (US_CSR) COMM_RX is at 0 */
#define   US_CSR_LINHTE_1           (0x1u << 31) /**< \brief (US_CSR) COMM_RX is at 1 */
#define US_CSR_MASK                 0xFFFFFFFFu  /**< \brief (US_CSR) MASK Register */

/* -------- US_RHR : (USART Offset: 0x18) (R/  32) Receiver Holding Register -------- */
#define US_RHR_OFFSET               0x18         /**< \brief (US_RHR offset) Receiver Holding Register */
#define US_RHR_RESETVALUE           0x00000000   /**< \brief (US_RHR reset_value) Receiver Holding Register */

#define US_RHR_RXCHR_Pos            0            /**< \brief (US_RHR) Received Character */
#define US_RHR_RXCHR_Msk            (0x1FFu << US_RHR_RXCHR_Pos)
#define US_RHR_RXCHR(value)         ((US_RHR_RXCHR_Msk & ((value) << US_RHR_RXCHR_Pos)))
#define US_RHR_RXSYNH_Pos           15           /**< \brief (US_RHR) Received Sync */
#define US_RHR_RXSYNH               (0x1u << US_RHR_RXSYNH_Pos)
#define   US_RHR_RXSYNH_0           (0x0u << 15) /**< \brief (US_RHR) Last character received is a Data */
#define   US_RHR_RXSYNH_1           (0x1u << 15) /**< \brief (US_RHR) Last character received is a Command */
#define US_RHR_MASK                 0x000081FFu  /**< \brief (US_RHR) MASK Register */

/* -------- US_THR : (USART Offset: 0x1C) ( /W 32) Transmitter Holding Register -------- */
#define US_THR_OFFSET               0x1C         /**< \brief (US_THR offset) Transmitter Holding Register */
#define US_THR_RESETVALUE           0x00000000   /**< \brief (US_THR reset_value) Transmitter Holding Register */

#define US_THR_TXCHR_Pos            0            /**< \brief (US_THR) Character to be Transmitted */
#define US_THR_TXCHR_Msk            (0x1FFu << US_THR_TXCHR_Pos)
#define US_THR_TXCHR(value)         ((US_THR_TXCHR_Msk & ((value) << US_THR_TXCHR_Pos)))
#define US_THR_TXSYNH_Pos           15           /**< \brief (US_THR) Sync Field to be transmitted */
#define US_THR_TXSYNH               (0x1u << US_THR_TXSYNH_Pos)
#define   US_THR_TXSYNH_0           (0x0u << 15) /**< \brief (US_THR) The next character sent is encoded as a data. Start Frame Delimiter is DATA SYNC */
#define   US_THR_TXSYNH_1           (0x1u << 15) /**< \brief (US_THR) The next character sent is encoded as a command. Start Frame Delimiter is COMMAND SYNC */
#define US_THR_MASK                 0x000081FFu  /**< \brief (US_THR) MASK Register */

/* -------- US_BRGR : (USART Offset: 0x20) (R/W 32) Baud Rate Generator Register -------- */
#define US_BRGR_OFFSET              0x20         /**< \brief (US_BRGR offset) Baud Rate Generator Register */
#define US_BRGR_RESETVALUE          0x00000000   /**< \brief (US_BRGR reset_value) Baud Rate Generator Register */

#define US_BRGR_CD_Pos              0            /**< \brief (US_BRGR) Clock Divisor */
#define US_BRGR_CD_Msk              (0xFFFFu << US_BRGR_CD_Pos)
#define US_BRGR_CD(value)           ((US_BRGR_CD_Msk & ((value) << US_BRGR_CD_Pos)))
#define   US_BRGR_CD_DISABLE        (0x0u <<  0) /**< \brief (US_BRGR) Disables the clock */
#define   US_BRGR_CD_BYPASS         (0x1u <<  0) /**< \brief (US_BRGR) Clock Divisor Bypass */
#define   US_BRGR_CD_2              (0x2u <<  0) /**< \brief (US_BRGR) Baud Rate (Asynchronous Mode) = Selected Clock/(16 x CD) or (8 x CD); Baud Rate (Synchronous Mode) = Selected Clock/CD; */
#define US_BRGR_FP_Pos              16           /**< \brief (US_BRGR) Fractional Part */
#define US_BRGR_FP_Msk              (0x7u << US_BRGR_FP_Pos)
#define US_BRGR_FP(value)           ((US_BRGR_FP_Msk & ((value) << US_BRGR_FP_Pos)))
#define   US_BRGR_FP_0              (0x0u << 16) /**< \brief (US_BRGR) Fractional divider is disabled */
#define US_BRGR_MASK                0x0007FFFFu  /**< \brief (US_BRGR) MASK Register */

/* -------- US_RTOR : (USART Offset: 0x24) (R/W 32) Receiver Time-out Register -------- */
#define US_RTOR_OFFSET              0x24         /**< \brief (US_RTOR offset) Receiver Time-out Register */
#define US_RTOR_RESETVALUE          0x00000000   /**< \brief (US_RTOR reset_value) Receiver Time-out Register */

#define US_RTOR_TO_Pos              0            /**< \brief (US_RTOR) Time-out Value */
#define US_RTOR_TO_Msk              (0x1FFFFu << US_RTOR_TO_Pos)
#define US_RTOR_TO(value)           ((US_RTOR_TO_Msk & ((value) << US_RTOR_TO_Pos)))
#define   US_RTOR_TO_DISABLE        (0x0u <<  0) /**< \brief (US_RTOR) Disables the RX Time-out function */
#define US_RTOR_MASK                0x0001FFFFu  /**< \brief (US_RTOR) MASK Register */

/* -------- US_TTGR : (USART Offset: 0x28) (R/W 32) Transmitter Timeguard Register -------- */
#define US_TTGR_OFFSET              0x28         /**< \brief (US_TTGR offset) Transmitter Timeguard Register */
#define US_TTGR_RESETVALUE          0x00000000   /**< \brief (US_TTGR reset_value) Transmitter Timeguard Register */

#define US_TTGR_TG_Pos              0            /**< \brief (US_TTGR) Timeguard Value */
#define US_TTGR_TG_Msk              (0xFFu << US_TTGR_TG_Pos)
#define US_TTGR_TG(value)           ((US_TTGR_TG_Msk & ((value) << US_TTGR_TG_Pos)))
#define   US_TTGR_TG_DISABLE        (0x0u <<  0) /**< \brief (US_TTGR) Disables the TX Timeguard function. */
#define US_TTGR_MASK                0x000000FFu  /**< \brief (US_TTGR) MASK Register */

/* -------- US_FIDI : (USART Offset: 0x40) (R/W 32) FI DI Ratio Register -------- */
#define US_FIDI_OFFSET              0x40         /**< \brief (US_FIDI offset) FI DI Ratio Register */
#define US_FIDI_RESETVALUE          0x00000174   /**< \brief (US_FIDI reset_value) FI DI Ratio Register */

#define US_FIDI_FI_DI_RATIO_Pos     0            /**< \brief (US_FIDI) FI Over DI Ratio Value */
#define US_FIDI_FI_DI_RATIO_Msk     (0x7FFu << US_FIDI_FI_DI_RATIO_Pos)
#define US_FIDI_FI_DI_RATIO(value)  ((US_FIDI_FI_DI_RATIO_Msk & ((value) << US_FIDI_FI_DI_RATIO_Pos)))
#define   US_FIDI_FI_DI_RATIO_DISABLE (0x0u <<  0) /**< \brief (US_FIDI) Baud Rate = 0 */
#define US_FIDI_MASK                0x000007FFu  /**< \brief (US_FIDI) MASK Register */

/* -------- US_NER : (USART Offset: 0x44) (R/  32) Number of Errors Register -------- */
#define US_NER_OFFSET               0x44         /**< \brief (US_NER offset) Number of Errors Register */
#define US_NER_RESETVALUE           0x00000000   /**< \brief (US_NER reset_value) Number of Errors Register */

#define US_NER_NB_ERRORS_Pos        0            /**< \brief (US_NER) Error number during ISO7816 transfers */
#define US_NER_NB_ERRORS_Msk        (0xFFu << US_NER_NB_ERRORS_Pos)
#define US_NER_NB_ERRORS(value)     ((US_NER_NB_ERRORS_Msk & ((value) << US_NER_NB_ERRORS_Pos)))
#define US_NER_MASK                 0x000000FFu  /**< \brief (US_NER) MASK Register */

/* -------- US_IFR : (USART Offset: 0x4C) (R/W 32) IrDA Filter Register -------- */
#define US_IFR_OFFSET               0x4C         /**< \brief (US_IFR offset) IrDA Filter Register */
#define US_IFR_RESETVALUE           0x00000000   /**< \brief (US_IFR reset_value) IrDA Filter Register */

#define US_IFR_IRDA_FILTER_Pos      0            /**< \brief (US_IFR) Irda filter */
#define US_IFR_IRDA_FILTER_Msk      (0xFFu << US_IFR_IRDA_FILTER_Pos)
#define US_IFR_IRDA_FILTER(value)   ((US_IFR_IRDA_FILTER_Msk & ((value) << US_IFR_IRDA_FILTER_Pos)))
#define US_IFR_MASK                 0x000000FFu  /**< \brief (US_IFR) MASK Register */

/* -------- US_MAN : (USART Offset: 0x50) (R/W 32) Manchester Configuration Register -------- */
#define US_MAN_OFFSET               0x50         /**< \brief (US_MAN offset) Manchester Configuration Register */
#define US_MAN_RESETVALUE           0x30011004   /**< \brief (US_MAN reset_value) Manchester Configuration Register */

#define US_MAN_TX_PL_Pos            0            /**< \brief (US_MAN) Transmitter Preamble Length */
#define US_MAN_TX_PL_Msk            (0xFu << US_MAN_TX_PL_Pos)
#define US_MAN_TX_PL(value)         ((US_MAN_TX_PL_Msk & ((value) << US_MAN_TX_PL_Pos)))
#define   US_MAN_TX_PL_0            (0x0u <<  0) /**< \brief (US_MAN) The Transmitter Preamble pattern generation is disabled */
#define US_MAN_TX_PP_Pos            8            /**< \brief (US_MAN) Transmitter Preamble Pattern */
#define US_MAN_TX_PP_Msk            (0x3u << US_MAN_TX_PP_Pos)
#define US_MAN_TX_PP(value)         ((US_MAN_TX_PP_Msk & ((value) << US_MAN_TX_PP_Pos)))
#define   US_MAN_TX_PP_0            (0x0u <<  8) /**< \brief (US_MAN) ALL_ONE */
#define   US_MAN_TX_PP_1            (0x1u <<  8) /**< \brief (US_MAN) ALL_ZERO */
#define   US_MAN_TX_PP_2            (0x2u <<  8) /**< \brief (US_MAN) ZERO_ONE */
#define   US_MAN_TX_PP_3            (0x3u <<  8) /**< \brief (US_MAN) ONE_ZERO */
#define US_MAN_TX_MPOL_Pos          12           /**< \brief (US_MAN) Transmitter Manchester Polarity */
#define US_MAN_TX_MPOL              (0x1u << US_MAN_TX_MPOL_Pos)
#define   US_MAN_TX_MPOL_0          (0x0u << 12) /**< \brief (US_MAN) Logic Zero is coded as a zero-to-one transition, Logic One is coded as a one-to-zero transition */
#define   US_MAN_TX_MPOL_1          (0x1u << 12) /**< \brief (US_MAN) Logic Zero is coded as a one-to-zero transition, Logic One is coded as a zero-to-one transition */
#define US_MAN_RX_PL_Pos            16           /**< \brief (US_MAN) Receiver Preamble Length */
#define US_MAN_RX_PL_Msk            (0xFu << US_MAN_RX_PL_Pos)
#define US_MAN_RX_PL(value)         ((US_MAN_RX_PL_Msk & ((value) << US_MAN_RX_PL_Pos)))
#define   US_MAN_RX_PL_0            (0x0u << 16) /**< \brief (US_MAN) The receiver preamble pattern detection is disabled */
#define US_MAN_RX_PP_Pos            24           /**< \brief (US_MAN) Receiver Preamble Pattern detected */
#define US_MAN_RX_PP_Msk            (0x3u << US_MAN_RX_PP_Pos)
#define US_MAN_RX_PP(value)         ((US_MAN_RX_PP_Msk & ((value) << US_MAN_RX_PP_Pos)))
#define   US_MAN_RX_PP_0            (0x0u << 24) /**< \brief (US_MAN) ALL_ONE */
#define   US_MAN_RX_PP_1            (0x1u << 24) /**< \brief (US_MAN) ALL_ZERO */
#define   US_MAN_RX_PP_2            (0x2u << 24) /**< \brief (US_MAN) ZERO_ONE */
#define   US_MAN_RX_PP_3            (0x3u << 24) /**< \brief (US_MAN) ONE_ZERO */
#define US_MAN_RX_MPOL_Pos          28           /**< \brief (US_MAN) Receiver Manchester Polarity */
#define US_MAN_RX_MPOL              (0x1u << US_MAN_RX_MPOL_Pos)
#define   US_MAN_RX_MPOL_0          (0x0u << 28) /**< \brief (US_MAN) Logic Zero is coded as a zero-to-one transition, Logic One is coded as a one-to-zero transition */
#define   US_MAN_RX_MPOL_1          (0x1u << 28) /**< \brief (US_MAN) Logic Zero is coded as a one-to-zero transition, Logic One is coded as a zero-to-one transition */
#define US_MAN_DRIFT_Pos            30           /**< \brief (US_MAN) Drift compensation */
#define US_MAN_DRIFT                (0x1u << US_MAN_DRIFT_Pos)
#define   US_MAN_DRIFT_0            (0x0u << 30) /**< \brief (US_MAN) The USART can not recover from an important clock drift */
#define   US_MAN_DRIFT_1            (0x1u << 30) /**< \brief (US_MAN) The USART can recover from clock drift. The 16X clock mode must be enabled */
#define US_MAN_MASK                 0x530F130Fu  /**< \brief (US_MAN) MASK Register */

/* -------- US_LINMR : (USART Offset: 0x54) (R/W 32) LIN Mode Register -------- */
#define US_LINMR_OFFSET             0x54         /**< \brief (US_LINMR offset) LIN Mode Register */
#define US_LINMR_RESETVALUE         0x00000000   /**< \brief (US_LINMR reset_value) LIN Mode Register */

#define US_LINMR_NACT_Pos           0            /**< \brief (US_LINMR) LIN Node Action */
#define US_LINMR_NACT_Msk           (0x3u << US_LINMR_NACT_Pos)
#define US_LINMR_NACT(value)        ((US_LINMR_NACT_Msk & ((value) << US_LINMR_NACT_Pos)))
#define   US_LINMR_NACT_PUBLISH     (0x0u <<  0) /**< \brief (US_LINMR) The LIN Controller transmits the response */
#define   US_LINMR_NACT_SUBSCRIBE   (0x1u <<  0) /**< \brief (US_LINMR) The LIN Controller receives the response */
#define   US_LINMR_NACT_IGNORE      (0x2u <<  0) /**< \brief (US_LINMR) The LIN Controller doesn't transmit and doesn't receive the response */
#define US_LINMR_PARDIS_Pos         2            /**< \brief (US_LINMR) Parity Disable */
#define US_LINMR_PARDIS             (0x1u << US_LINMR_PARDIS_Pos)
#define US_LINMR_CHKDIS_Pos         3            /**< \brief (US_LINMR) Checksum Disable */
#define US_LINMR_CHKDIS             (0x1u << US_LINMR_CHKDIS_Pos)
#define US_LINMR_CHKTYP_Pos         4            /**< \brief (US_LINMR) Checksum Type */
#define US_LINMR_CHKTYP             (0x1u << US_LINMR_CHKTYP_Pos)
#define US_LINMR_DLM_Pos            5            /**< \brief (US_LINMR) Data Length Mode */
#define US_LINMR_DLM                (0x1u << US_LINMR_DLM_Pos)
#define US_LINMR_FSDIS_Pos          6            /**< \brief (US_LINMR) Frame Slot Mode Disable */
#define US_LINMR_FSDIS              (0x1u << US_LINMR_FSDIS_Pos)
#define US_LINMR_WKUPTYP_Pos        7            /**< \brief (US_LINMR) Wakeup Signal Type */
#define US_LINMR_WKUPTYP            (0x1u << US_LINMR_WKUPTYP_Pos)
#define US_LINMR_DLC_Pos            8            /**< \brief (US_LINMR) Data Length Control */
#define US_LINMR_DLC_Msk            (0xFFu << US_LINMR_DLC_Pos)
#define US_LINMR_DLC(value)         ((US_LINMR_DLC_Msk & ((value) << US_LINMR_DLC_Pos)))
#define US_LINMR_PDCM_Pos           16           /**< \brief (US_LINMR) PDC Mode */
#define US_LINMR_PDCM               (0x1u << US_LINMR_PDCM_Pos)
#define US_LINMR_SYNCDIS_Pos        17           /**< \brief (US_LINMR) Synchronization Disable */
#define US_LINMR_SYNCDIS            (0x1u << US_LINMR_SYNCDIS_Pos)
#define US_LINMR_MASK               0x0003FFFFu  /**< \brief (US_LINMR) MASK Register */

/* -------- US_LINIR : (USART Offset: 0x58) (R/W 32) LIN Identifier Register -------- */
#define US_LINIR_OFFSET             0x58         /**< \brief (US_LINIR offset) LIN Identifier Register */
#define US_LINIR_RESETVALUE         0x00000000   /**< \brief (US_LINIR reset_value) LIN Identifier Register */

#define US_LINIR_IDCHR_Pos          0            /**< \brief (US_LINIR) Identifier Character */
#define US_LINIR_IDCHR_Msk          (0xFFu << US_LINIR_IDCHR_Pos)
#define US_LINIR_IDCHR(value)       ((US_LINIR_IDCHR_Msk & ((value) << US_LINIR_IDCHR_Pos)))
#define US_LINIR_MASK               0x000000FFu  /**< \brief (US_LINIR) MASK Register */

/* -------- US_LINBRR : (USART Offset: 0x5C) (R/  32) LIN Baud Rate Register -------- */
#define US_LINBRR_OFFSET            0x5C         /**< \brief (US_LINBRR offset) LIN Baud Rate Register */
#define US_LINBRR_RESETVALUE        0x00000000   /**< \brief (US_LINBRR reset_value) LIN Baud Rate Register */

#define US_LINBRR_LINCD_Pos         0            /**< \brief (US_LINBRR) Clock Divider after Synchronization */
#define US_LINBRR_LINCD_Msk         (0xFFFFu << US_LINBRR_LINCD_Pos)
#define US_LINBRR_LINCD(value)      ((US_LINBRR_LINCD_Msk & ((value) << US_LINBRR_LINCD_Pos)))
#define US_LINBRR_LINFP_Pos         16           /**< \brief (US_LINBRR) Fractional Part after Synchronization */
#define US_LINBRR_LINFP_Msk         (0x7u << US_LINBRR_LINFP_Pos)
#define US_LINBRR_LINFP(value)      ((US_LINBRR_LINFP_Msk & ((value) << US_LINBRR_LINFP_Pos)))
#define US_LINBRR_MASK              0x0007FFFFu  /**< \brief (US_LINBRR) MASK Register */

/* -------- US_WPMR : (USART Offset: 0xE4) (R/W 32) Write Protect Mode Register -------- */
#define US_WPMR_OFFSET              0xE4         /**< \brief (US_WPMR offset) Write Protect Mode Register */
#define US_WPMR_RESETVALUE          0x00000000   /**< \brief (US_WPMR reset_value) Write Protect Mode Register */

#define US_WPMR_WPEN_Pos            0            /**< \brief (US_WPMR) Write Protect Enable */
#define US_WPMR_WPEN                (0x1u << US_WPMR_WPEN_Pos)
#define   US_WPMR_WPEN_0            (0x0u <<  0) /**< \brief (US_WPMR) Disables the Write Protect if WPKEY corresponds to 0x858365 ("USA" in ACII) */
#define   US_WPMR_WPEN_1            (0x1u <<  0) /**< \brief (US_WPMR) Enables the Write Protect if WPKEY corresponds to 0x858365 ("USA" in ACII) */
#define US_WPMR_WPKEY_Pos           8            /**< \brief (US_WPMR) Write Protect Key */
#define US_WPMR_WPKEY_Msk           (0xFFFFFFu << US_WPMR_WPKEY_Pos)
#define US_WPMR_WPKEY(value)        ((US_WPMR_WPKEY_Msk & ((value) << US_WPMR_WPKEY_Pos)))
#define US_WPMR_MASK                0xFFFFFF01u  /**< \brief (US_WPMR) MASK Register */

/* -------- US_WPSR : (USART Offset: 0xE8) (R/  32) Write Protect Status Register -------- */
#define US_WPSR_OFFSET              0xE8         /**< \brief (US_WPSR offset) Write Protect Status Register */
#define US_WPSR_RESETVALUE          0x00000000   /**< \brief (US_WPSR reset_value) Write Protect Status Register */

#define US_WPSR_WPV_Pos             0            /**< \brief (US_WPSR) Write Protect Violation Status */
#define US_WPSR_WPV                 (0x1u << US_WPSR_WPV_Pos)
#define   US_WPSR_WPV_0             (0x0u <<  0) /**< \brief (US_WPSR) No Write Protect Violation has occurred since the last read of the WPSR register */
#define   US_WPSR_WPV_1             (0x1u <<  0) /**< \brief (US_WPSR) A Write Protect Violation has occurred since the last read of the WPSR register. If this violation is an unauthorized attempt to write a protected register, the associated violation is reported into field WPVSRC */
#define US_WPSR_WPVSRC_Pos          8            /**< \brief (US_WPSR) Write Protect Violation Source */
#define US_WPSR_WPVSRC_Msk          (0xFFFFu << US_WPSR_WPVSRC_Pos)
#define US_WPSR_WPVSRC(value)       ((US_WPSR_WPVSRC_Msk & ((value) << US_WPSR_WPVSRC_Pos)))
#define US_WPSR_MASK                0x00FFFF01u  /**< \brief (US_WPSR) MASK Register */

/* -------- US_VERSION : (USART Offset: 0xFC) (R/  32) Version Register -------- */
#define US_VERSION_OFFSET           0xFC         /**< \brief (US_VERSION offset) Version Register */
#define US_VERSION_RESETVALUE       0x00000602   /**< \brief (US_VERSION reset_value) Version Register */

#define US_VERSION_VERSION_Pos      0            /**< \brief (US_VERSION)  */
#define US_VERSION_VERSION_Msk      (0xFFFu << US_VERSION_VERSION_Pos)
#define US_VERSION_VERSION(value)   ((US_VERSION_VERSION_Msk & ((value) << US_VERSION_VERSION_Pos)))
#define US_VERSION_MFN_Pos          16           /**< \brief (US_VERSION)  */
#define US_VERSION_MFN_Msk          (0xFu << US_VERSION_MFN_Pos)
#define US_VERSION_MFN(value)       ((US_VERSION_MFN_Msk & ((value) << US_VERSION_MFN_Pos)))
#define US_VERSION_MASK             0x000F0FFFu  /**< \brief (US_VERSION) MASK Register */

/** \brief USART hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
  WoReg   US_CR;              /**< \brief (USART Offset: 0x00) Control Register */
  RwReg   US_MR;              /**< \brief (USART Offset: 0x04) Mode Register */
  WoReg   US_IER;             /**< \brief (USART Offset: 0x08) Interrupt Enable Register */
  WoReg   US_IDR;             /**< \brief (USART Offset: 0x0C) Interrupt Disable Register */
  RoReg   US_IMR;             /**< \brief (USART Offset: 0x10) Interrupt Mask Register */
  RoReg   US_CSR;             /**< \brief (USART Offset: 0x14) Channel Status Register */
  RoReg   US_RHR;             /**< \brief (USART Offset: 0x18) Receiver Holding Register */
  WoReg   US_THR;             /**< \brief (USART Offset: 0x1C) Transmitter Holding Register */
  RwReg   US_BRGR;            /**< \brief (USART Offset: 0x20) Baud Rate Generator Register */
  RwReg   US_RTOR;            /**< \brief (USART Offset: 0x24) Receiver Time-out Register */
  RwReg   US_TTGR;            /**< \brief (USART Offset: 0x28) Transmitter Timeguard Register */
  RoReg8  Reserved1[0x14];
  RwReg   US_FIDI;            /**< \brief (USART Offset: 0x40) FI DI Ratio Register */
  RoReg   US_NER;             /**< \brief (USART Offset: 0x44) Number of Errors Register */
  RoReg8  Reserved2[0x4];
  RwReg   US_IFR;             /**< \brief (USART Offset: 0x4C) IrDA Filter Register */
  RwReg   US_MAN;             /**< \brief (USART Offset: 0x50) Manchester Configuration Register */
  RwReg   US_LINMR;           /**< \brief (USART Offset: 0x54) LIN Mode Register */
  RwReg   US_LINIR;           /**< \brief (USART Offset: 0x58) LIN Identifier Register */
  RoReg   US_LINBRR;          /**< \brief (USART Offset: 0x5C) LIN Baud Rate Register */
  RoReg8  Reserved3[0x84];
  RwReg   US_WPMR;            /**< \brief (USART Offset: 0xE4) Write Protect Mode Register */
  RoReg   US_WPSR;            /**< \brief (USART Offset: 0xE8) Write Protect Status Register */
  RoReg8  Reserved4[0x10];
  RoReg   US_VERSION;         /**< \brief (USART Offset: 0xFC) Version Register */
} Usart;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/*@}*/

#endif /* _SAM4L_USART_COMPONENT_ */
