/**
 * \file
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

#ifndef _SAMG55_TWI_COMPONENT_
#define _SAMG55_TWI_COMPONENT_

/* ============================================================================= */
/**  SOFTWARE API DEFINITION FOR Two wire Interface */
/* ============================================================================= */
/** \addtogroup SAMG55_TWI Two wire Interface */
/*@{*/

#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
/** \brief Twi hardware registers */
typedef struct {
  __O  uint32_t TWI_CR;        /**< \brief (Twi Offset: 0x000) TWI Control Register */
  __IO uint32_t TWI_MMR;       /**< \brief (Twi Offset: 0x004) TWI Master Mode Register */
  __IO uint32_t TWI_SMR;       /**< \brief (Twi Offset: 0x008) TWI Slave Mode Register */
  __IO uint32_t TWI_IADR;      /**< \brief (Twi Offset: 0x00C) TWI Internal Address Register */
  __IO uint32_t TWI_CWGR;      /**< \brief (Twi Offset: 0x010) TWI Clock Waveform Generator Register */
  __I  uint32_t Reserved1[3];
  __I  uint32_t TWI_SR;        /**< \brief (Twi Offset: 0x020) TWI Status Register */
  __O  uint32_t TWI_IER;       /**< \brief (Twi Offset: 0x024) TWI Interrupt Enable Register */
  __O  uint32_t TWI_IDR;       /**< \brief (Twi Offset: 0x028) TWI Interrupt Disable Register */
  __I  uint32_t TWI_IMR;       /**< \brief (Twi Offset: 0x02C) TWI Interrupt Mask Register */
  __I  uint32_t TWI_RHR;       /**< \brief (Twi Offset: 0x030) TWI Receive Holding Register */
  __O  uint32_t TWI_THR;       /**< \brief (Twi Offset: 0x034) TWI Transmit Holding Register */
  __IO uint32_t TWI_SMBTR;     /**< \brief (Twi Offset: 0x038) TWI SMBus Timing Register */
  __I  uint32_t Reserved2[1];
  __IO uint32_t TWI_ACR;       /**< \brief (Twi Offset: 0x040) TWI Alternative Command Register */
  __IO uint32_t TWI_FILTR;     /**< \brief (Twi Offset: 0x044) TWI Filter Register */
  __I  uint32_t Reserved3[1];
  __IO uint32_t TWI_SWMR;      /**< \brief (Twi Offset: 0x04C) TWI SleepWalking Matching Register */
  __I  uint32_t Reserved4[37];
  __IO uint32_t TWI_WPMR;      /**< \brief (Twi Offset: 0x0E4) TWI Write Protection Mode Register */
  __I  uint32_t TWI_WPSR;      /**< \brief (Twi Offset: 0x0E8) TWI Write Protection Status Register */
  __I  uint32_t Reserved5[5];
  __IO uint32_t TWI_RPR;       /**< \brief (Twi Offset: 0x100) Receive Pointer Register */
  __IO uint32_t TWI_RCR;       /**< \brief (Twi Offset: 0x104) Receive Counter Register */
  __IO uint32_t TWI_TPR;       /**< \brief (Twi Offset: 0x108) Transmit Pointer Register */
  __IO uint32_t TWI_TCR;       /**< \brief (Twi Offset: 0x10C) Transmit Counter Register */
  __IO uint32_t TWI_RNPR;      /**< \brief (Twi Offset: 0x110) Receive Next Pointer Register */
  __IO uint32_t TWI_RNCR;      /**< \brief (Twi Offset: 0x114) Receive Next Counter Register */
  __IO uint32_t TWI_TNPR;      /**< \brief (Twi Offset: 0x118) Transmit Next Pointer Register */
  __IO uint32_t TWI_TNCR;      /**< \brief (Twi Offset: 0x11C) Transmit Next Counter Register */
  __O  uint32_t TWI_PTCR;      /**< \brief (Twi Offset: 0x120) Transfer Control Register */
  __I  uint32_t TWI_PTSR;      /**< \brief (Twi Offset: 0x124) Transfer Status Register */
} Twi;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */
/* -------- TWI_CR : (TWI Offset: 0x000) TWI Control Register -------- */
#define TWI_CR_START (0x1u << 0) /**< \brief (TWI_CR) Send a START Condition */
#define TWI_CR_STOP (0x1u << 1) /**< \brief (TWI_CR) Send a STOP Condition */
#define TWI_CR_MSEN (0x1u << 2) /**< \brief (TWI_CR) TWI Master Mode Enabled */
#define TWI_CR_MSDIS (0x1u << 3) /**< \brief (TWI_CR) TWI Master Mode Disabled */
#define TWI_CR_SVEN (0x1u << 4) /**< \brief (TWI_CR) TWI Slave Mode Enabled */
#define TWI_CR_SVDIS (0x1u << 5) /**< \brief (TWI_CR) TWI Slave Mode Disabled */
#define TWI_CR_QUICK (0x1u << 6) /**< \brief (TWI_CR) SMBUS Quick Command */
#define TWI_CR_SWRST (0x1u << 7) /**< \brief (TWI_CR) Software Reset */
#define TWI_CR_HSEN (0x1u << 8) /**< \brief (TWI_CR) TWI High-Speed Mode Enabled */
#define TWI_CR_HSDIS (0x1u << 9) /**< \brief (TWI_CR) TWI High-Speed Mode Disabled */
#define TWI_CR_SMBEN (0x1u << 10) /**< \brief (TWI_CR) SMBus Mode Enabled */
#define TWI_CR_SMBDIS (0x1u << 11) /**< \brief (TWI_CR) SMBus Mode Disabled */
#define TWI_CR_PECEN (0x1u << 12) /**< \brief (TWI_CR) Packet Error Checking Enable */
#define TWI_CR_PECDIS (0x1u << 13) /**< \brief (TWI_CR) Packet Error Checking Disable */
#define TWI_CR_PECRQ (0x1u << 14) /**< \brief (TWI_CR) PEC Request */
#define TWI_CR_CLEAR (0x1u << 15) /**< \brief (TWI_CR) Bus CLEAR Command */
#define TWI_CR_ACMEN (0x1u << 16) /**< \brief (TWI_CR) Alternative Command Mode Enable */
#define TWI_CR_ACMDIS (0x1u << 17) /**< \brief (TWI_CR) Alternative Command Mode Disable */
#define TWI_CR_THRCLR (0x1u << 24) /**< \brief (TWI_CR) Transmit Holding Register Clear */
#define TWI_CR_LOCKCLR (0x1u << 26) /**< \brief (TWI_CR) Lock Clear */
/* -------- TWI_MMR : (TWI Offset: 0x004) TWI Master Mode Register -------- */
#define TWI_MMR_IADRSZ_Pos 8
#define TWI_MMR_IADRSZ_Msk (0x3u << TWI_MMR_IADRSZ_Pos) /**< \brief (TWI_MMR) Internal Device Address Size */
#define   TWI_MMR_IADRSZ_NONE (0x0u << 8) /**< \brief (TWI_MMR) No internal device address */
#define   TWI_MMR_IADRSZ_1_BYTE (0x1u << 8) /**< \brief (TWI_MMR) One-byte internal device address */
#define   TWI_MMR_IADRSZ_2_BYTE (0x2u << 8) /**< \brief (TWI_MMR) Two-byte internal device address */
#define   TWI_MMR_IADRSZ_3_BYTE (0x3u << 8) /**< \brief (TWI_MMR) Three-byte internal device address */
#define TWI_MMR_MREAD (0x1u << 12) /**< \brief (TWI_MMR) Master Read Direction */
#define TWI_MMR_DADR_Pos 16
#define TWI_MMR_DADR_Msk (0x7fu << TWI_MMR_DADR_Pos) /**< \brief (TWI_MMR) Device Address */
#define TWI_MMR_DADR(value) ((TWI_MMR_DADR_Msk & ((value) << TWI_MMR_DADR_Pos)))
/* -------- TWI_SMR : (TWI Offset: 0x008) TWI Slave Mode Register -------- */
#define TWI_SMR_NACKEN (0x1u << 0) /**< \brief (TWI_SMR) Slave Receiver Data Phase NACK Enable */
#define TWI_SMR_SMDA (0x1u << 2) /**< \brief (TWI_SMR) SMBus Default Address */
#define TWI_SMR_SMHH (0x1u << 3) /**< \brief (TWI_SMR) SMBus Host Header */
#define TWI_SMR_SCLWSDIS (0x1u << 6) /**< \brief (TWI_SMR) Clock Wait State Disable */
#define TWI_SMR_MASK_Pos 8
#define TWI_SMR_MASK_Msk (0x7fu << TWI_SMR_MASK_Pos) /**< \brief (TWI_SMR) Slave Address Mask */
#define TWI_SMR_MASK(value) ((TWI_SMR_MASK_Msk & ((value) << TWI_SMR_MASK_Pos)))
#define TWI_SMR_SADR_Pos 16
#define TWI_SMR_SADR_Msk (0x7fu << TWI_SMR_SADR_Pos) /**< \brief (TWI_SMR) Slave Address */
#define TWI_SMR_SADR(value) ((TWI_SMR_SADR_Msk & ((value) << TWI_SMR_SADR_Pos)))
#define TWI_SMR_SADR1EN (0x1u << 28) /**< \brief (TWI_SMR) Slave Address 1 Enable */
#define TWI_SMR_SADR2EN (0x1u << 29) /**< \brief (TWI_SMR) Slave Address 2 Enable */
#define TWI_SMR_SADR3EN (0x1u << 30) /**< \brief (TWI_SMR) Slave Address 3 Enable */
#define TWI_SMR_DATAMEN (0x1u << 31) /**< \brief (TWI_SMR) Data Matching Enable */
/* -------- TWI_IADR : (TWI Offset: 0x00C) TWI Internal Address Register -------- */
#define TWI_IADR_IADR_Pos 0
#define TWI_IADR_IADR_Msk (0xffffffu << TWI_IADR_IADR_Pos) /**< \brief (TWI_IADR) Internal Address */
#define TWI_IADR_IADR(value) ((TWI_IADR_IADR_Msk & ((value) << TWI_IADR_IADR_Pos)))
/* -------- TWI_CWGR : (TWI Offset: 0x010) TWI Clock Waveform Generator Register -------- */
#define TWI_CWGR_CLDIV_Pos 0
#define TWI_CWGR_CLDIV_Msk (0xffu << TWI_CWGR_CLDIV_Pos) /**< \brief (TWI_CWGR) Clock Low Divider */
#define TWI_CWGR_CLDIV(value) ((TWI_CWGR_CLDIV_Msk & ((value) << TWI_CWGR_CLDIV_Pos)))
#define TWI_CWGR_CHDIV_Pos 8
#define TWI_CWGR_CHDIV_Msk (0xffu << TWI_CWGR_CHDIV_Pos) /**< \brief (TWI_CWGR) Clock High Divider */
#define TWI_CWGR_CHDIV(value) ((TWI_CWGR_CHDIV_Msk & ((value) << TWI_CWGR_CHDIV_Pos)))
#define TWI_CWGR_CKDIV_Pos 16
#define TWI_CWGR_CKDIV_Msk (0x7u << TWI_CWGR_CKDIV_Pos) /**< \brief (TWI_CWGR) Clock Divider */
#define TWI_CWGR_CKDIV(value) ((TWI_CWGR_CKDIV_Msk & ((value) << TWI_CWGR_CKDIV_Pos)))
#define TWI_CWGR_BRSRCCLK (0x1u << 20) /**< \brief (TWI_CWGR) Bit Rate Source Clock */
#define   TWI_CWGR_BRSRCCLK_PERIPH_CLK (0x0u << 20) /**< \brief (TWI_CWGR) The peripheral clock is the source clock for the bit rate generation. */
#define   TWI_CWGR_BRSRCCLK_PMC_PCK (0x1u << 20) /**< \brief (TWI_CWGR) PMC PCKx is the source clock for the bit rate generation, thus the bit rate can be independent of the core/peripheral clock. */
#define TWI_CWGR_HOLD_Pos 24
#define TWI_CWGR_HOLD_Msk (0x1fu << TWI_CWGR_HOLD_Pos) /**< \brief (TWI_CWGR) TWD Hold Time Versus TWCK Falling */
#define TWI_CWGR_HOLD(value) ((TWI_CWGR_HOLD_Msk & ((value) << TWI_CWGR_HOLD_Pos)))
/* -------- TWI_SR : (TWI Offset: 0x020) TWI Status Register -------- */
#define TWI_SR_TXCOMP (0x1u << 0) /**< \brief (TWI_SR) Transmission Completed (automatically set / reset) */
#define TWI_SR_RXRDY (0x1u << 1) /**< \brief (TWI_SR) Receive Holding Register Ready (automatically set / reset) */
#define TWI_SR_TXRDY (0x1u << 2) /**< \brief (TWI_SR) Transmit Holding Register Ready (automatically set / reset) */
#define TWI_SR_SVREAD (0x1u << 3) /**< \brief (TWI_SR) Slave Read (automatically set / reset) */
#define TWI_SR_SVACC (0x1u << 4) /**< \brief (TWI_SR) Slave Access (automatically set / reset) */
#define TWI_SR_GACC (0x1u << 5) /**< \brief (TWI_SR) General Call Access (clear on read) */
#define TWI_SR_OVRE (0x1u << 6) /**< \brief (TWI_SR) Overrun Error (clear on read) */
#define TWI_SR_UNRE (0x1u << 7) /**< \brief (TWI_SR) Underrun Error (clear on read) */
#define TWI_SR_NACK (0x1u << 8) /**< \brief (TWI_SR) Not Acknowledged (clear on read) */
#define TWI_SR_ARBLST (0x1u << 9) /**< \brief (TWI_SR) Arbitration Lost (clear on read) */
#define TWI_SR_SCLWS (0x1u << 10) /**< \brief (TWI_SR) Clock Wait State (automatically set / reset) */
#define TWI_SR_EOSACC (0x1u << 11) /**< \brief (TWI_SR) End Of Slave Access (clear on read) */
#define TWI_SR_ENDRX (0x1u << 12) /**< \brief (TWI_SR) End of RX Buffer */
#define TWI_SR_ENDTX (0x1u << 13) /**< \brief (TWI_SR) End of TX Buffer */
#define TWI_SR_RXBUFF (0x1u << 14) /**< \brief (TWI_SR) RX Buffer Full */
#define TWI_SR_TXBUFE (0x1u << 15) /**< \brief (TWI_SR) TX Buffer Empty */
#define TWI_SR_MCACK (0x1u << 16) /**< \brief (TWI_SR) Master Code Acknowledge */
#define TWI_SR_TOUT (0x1u << 18) /**< \brief (TWI_SR) Timeout Error */
#define TWI_SR_PECERR (0x1u << 19) /**< \brief (TWI_SR) PEC Error */
#define TWI_SR_SMBDAM (0x1u << 20) /**< \brief (TWI_SR) SMBus Default Address Match */
#define TWI_SR_SMBHHM (0x1u << 21) /**< \brief (TWI_SR) SMBus Host Header Address Match */
#define TWI_SR_LOCK (0x1u << 23) /**< \brief (TWI_SR) TWI Lock due to Frame Errors */
#define TWI_SR_SCL (0x1u << 24) /**< \brief (TWI_SR) SCL line value */
#define TWI_SR_SDA (0x1u << 25) /**< \brief (TWI_SR) SDA line value */
/* -------- TWI_IER : (TWI Offset: 0x024) TWI Interrupt Enable Register -------- */
#define TWI_IER_TXCOMP (0x1u << 0) /**< \brief (TWI_IER) Transmission Completed Interrupt Enable */
#define TWI_IER_RXRDY (0x1u << 1) /**< \brief (TWI_IER) Receive Holding Register Ready Interrupt Enable */
#define TWI_IER_TXRDY (0x1u << 2) /**< \brief (TWI_IER) Transmit Holding Register Ready Interrupt Enable */
#define TWI_IER_SVACC (0x1u << 4) /**< \brief (TWI_IER) Slave Access Interrupt Enable */
#define TWI_IER_GACC (0x1u << 5) /**< \brief (TWI_IER) General Call Access Interrupt Enable */
#define TWI_IER_OVRE (0x1u << 6) /**< \brief (TWI_IER) Overrun Error Interrupt Enable */
#define TWI_IER_UNRE (0x1u << 7) /**< \brief (TWI_IER) Underrun Error Interrupt Enable */
#define TWI_IER_NACK (0x1u << 8) /**< \brief (TWI_IER) Not Acknowledge Interrupt Enable */
#define TWI_IER_ARBLST (0x1u << 9) /**< \brief (TWI_IER) Arbitration Lost Interrupt Enable */
#define TWI_IER_SCL_WS (0x1u << 10) /**< \brief (TWI_IER) Clock Wait State Interrupt Enable */
#define TWI_IER_EOSACC (0x1u << 11) /**< \brief (TWI_IER) End Of Slave Access Interrupt Enable */
#define TWI_IER_ENDRX (0x1u << 12) /**< \brief (TWI_IER) End of Receive Buffer Interrupt Enable */
#define TWI_IER_ENDTX (0x1u << 13) /**< \brief (TWI_IER) End of Transmit Buffer Interrupt Enable */
#define TWI_IER_RXBUFF (0x1u << 14) /**< \brief (TWI_IER) Receive Buffer Full Interrupt Enable */
#define TWI_IER_TXBUFE (0x1u << 15) /**< \brief (TWI_IER) Transmit Buffer Empty Interrupt Enable */
#define TWI_IER_MCACK (0x1u << 16) /**< \brief (TWI_IER) Master Code Acknowledge Interrupt Enable */
#define TWI_IER_TOUT (0x1u << 18) /**< \brief (TWI_IER) Timeout Error Interrupt Enable */
#define TWI_IER_PECERR (0x1u << 19) /**< \brief (TWI_IER) PEC Error Interrupt Enable */
#define TWI_IER_SMBDAM (0x1u << 20) /**< \brief (TWI_IER) SMBus Default Address Match Interrupt Enable */
#define TWI_IER_SMBHHM (0x1u << 21) /**< \brief (TWI_IER) SMBus Host Header Address Match Interrupt Enable */
/* -------- TWI_IDR : (TWI Offset: 0x028) TWI Interrupt Disable Register -------- */
#define TWI_IDR_TXCOMP (0x1u << 0) /**< \brief (TWI_IDR) Transmission Completed Interrupt Disable */
#define TWI_IDR_RXRDY (0x1u << 1) /**< \brief (TWI_IDR) Receive Holding Register Ready Interrupt Disable */
#define TWI_IDR_TXRDY (0x1u << 2) /**< \brief (TWI_IDR) Transmit Holding Register Ready Interrupt Disable */
#define TWI_IDR_SVACC (0x1u << 4) /**< \brief (TWI_IDR) Slave Access Interrupt Disable */
#define TWI_IDR_GACC (0x1u << 5) /**< \brief (TWI_IDR) General Call Access Interrupt Disable */
#define TWI_IDR_OVRE (0x1u << 6) /**< \brief (TWI_IDR) Overrun Error Interrupt Disable */
#define TWI_IDR_UNRE (0x1u << 7) /**< \brief (TWI_IDR) Underrun Error Interrupt Disable */
#define TWI_IDR_NACK (0x1u << 8) /**< \brief (TWI_IDR) Not Acknowledge Interrupt Disable */
#define TWI_IDR_ARBLST (0x1u << 9) /**< \brief (TWI_IDR) Arbitration Lost Interrupt Disable */
#define TWI_IDR_SCL_WS (0x1u << 10) /**< \brief (TWI_IDR) Clock Wait State Interrupt Disable */
#define TWI_IDR_EOSACC (0x1u << 11) /**< \brief (TWI_IDR) End Of Slave Access Interrupt Disable */
#define TWI_IDR_ENDRX (0x1u << 12) /**< \brief (TWI_IDR) End of Receive Buffer Interrupt Disable */
#define TWI_IDR_ENDTX (0x1u << 13) /**< \brief (TWI_IDR) End of Transmit Buffer Interrupt Disable */
#define TWI_IDR_RXBUFF (0x1u << 14) /**< \brief (TWI_IDR) Receive Buffer Full Interrupt Disable */
#define TWI_IDR_TXBUFE (0x1u << 15) /**< \brief (TWI_IDR) Transmit Buffer Empty Interrupt Disable */
#define TWI_IDR_MCACK (0x1u << 16) /**< \brief (TWI_IDR) Master Code Acknowledge Interrupt Disable */
#define TWI_IDR_TOUT (0x1u << 18) /**< \brief (TWI_IDR) Timeout Error Interrupt Disable */
#define TWI_IDR_PECERR (0x1u << 19) /**< \brief (TWI_IDR) PEC Error Interrupt Disable */
#define TWI_IDR_SMBDAM (0x1u << 20) /**< \brief (TWI_IDR) SMBus Default Address Match Interrupt Disable */
#define TWI_IDR_SMBHHM (0x1u << 21) /**< \brief (TWI_IDR) SMBus Host Header Address Match Interrupt Disable */
/* -------- TWI_IMR : (TWI Offset: 0x02C) TWI Interrupt Mask Register -------- */
#define TWI_IMR_TXCOMP (0x1u << 0) /**< \brief (TWI_IMR) Transmission Completed Interrupt Mask */
#define TWI_IMR_RXRDY (0x1u << 1) /**< \brief (TWI_IMR) Receive Holding Register Ready Interrupt Mask */
#define TWI_IMR_TXRDY (0x1u << 2) /**< \brief (TWI_IMR) Transmit Holding Register Ready Interrupt Mask */
#define TWI_IMR_SVACC (0x1u << 4) /**< \brief (TWI_IMR) Slave Access Interrupt Mask */
#define TWI_IMR_GACC (0x1u << 5) /**< \brief (TWI_IMR) General Call Access Interrupt Mask */
#define TWI_IMR_OVRE (0x1u << 6) /**< \brief (TWI_IMR) Overrun Error Interrupt Mask */
#define TWI_IMR_UNRE (0x1u << 7) /**< \brief (TWI_IMR) Underrun Error Interrupt Mask */
#define TWI_IMR_NACK (0x1u << 8) /**< \brief (TWI_IMR) Not Acknowledge Interrupt Mask */
#define TWI_IMR_ARBLST (0x1u << 9) /**< \brief (TWI_IMR) Arbitration Lost Interrupt Mask */
#define TWI_IMR_SCL_WS (0x1u << 10) /**< \brief (TWI_IMR) Clock Wait State Interrupt Mask */
#define TWI_IMR_EOSACC (0x1u << 11) /**< \brief (TWI_IMR) End Of Slave Access Interrupt Mask */
#define TWI_IMR_ENDRX (0x1u << 12) /**< \brief (TWI_IMR) End of Receive Buffer Interrupt Mask */
#define TWI_IMR_ENDTX (0x1u << 13) /**< \brief (TWI_IMR) End of Transmit Buffer Interrupt Mask */
#define TWI_IMR_RXBUFF (0x1u << 14) /**< \brief (TWI_IMR) Receive Buffer Full Interrupt Mask */
#define TWI_IMR_TXBUFE (0x1u << 15) /**< \brief (TWI_IMR) Transmit Buffer Empty Interrupt Mask */
#define TWI_IMR_MCACK (0x1u << 16) /**< \brief (TWI_IMR) Master Code Acknowledge Interrupt Mask */
#define TWI_IMR_TOUT (0x1u << 18) /**< \brief (TWI_IMR) Timeout Error Interrupt Mask */
#define TWI_IMR_PECERR (0x1u << 19) /**< \brief (TWI_IMR) PEC Error Interrupt Mask */
#define TWI_IMR_SMBDAM (0x1u << 20) /**< \brief (TWI_IMR) SMBus Default Address Match Interrupt Mask */
#define TWI_IMR_SMBHHM (0x1u << 21) /**< \brief (TWI_IMR) SMBus Host Header Address Match Interrupt Mask */
/* -------- TWI_RHR : (TWI Offset: 0x030) TWI Receive Holding Register -------- */
#define TWI_RHR_RXDATA_Pos 0
#define TWI_RHR_RXDATA_Msk (0xffu << TWI_RHR_RXDATA_Pos) /**< \brief (TWI_RHR) Master or Slave Receive Holding Data */
/* -------- TWI_THR : (TWI Offset: 0x034) TWI Transmit Holding Register -------- */
#define TWI_THR_TXDATA_Pos 0
#define TWI_THR_TXDATA_Msk (0xffu << TWI_THR_TXDATA_Pos) /**< \brief (TWI_THR) Master or Slave Transmit Holding Data */
#define TWI_THR_TXDATA(value) ((TWI_THR_TXDATA_Msk & ((value) << TWI_THR_TXDATA_Pos)))
/* -------- TWI_SMBTR : (TWI Offset: 0x038) TWI SMBus Timing Register -------- */
#define TWI_SMBTR_PRESC_Pos 0
#define TWI_SMBTR_PRESC_Msk (0xfu << TWI_SMBTR_PRESC_Pos) /**< \brief (TWI_SMBTR) SMBus Clock Prescaler */
#define TWI_SMBTR_PRESC(value) ((TWI_SMBTR_PRESC_Msk & ((value) << TWI_SMBTR_PRESC_Pos)))
#define TWI_SMBTR_TLOWS_Pos 8
#define TWI_SMBTR_TLOWS_Msk (0xffu << TWI_SMBTR_TLOWS_Pos) /**< \brief (TWI_SMBTR) Slave Clock Stretch Maximum Cycles */
#define TWI_SMBTR_TLOWS(value) ((TWI_SMBTR_TLOWS_Msk & ((value) << TWI_SMBTR_TLOWS_Pos)))
#define TWI_SMBTR_TLOWM_Pos 16
#define TWI_SMBTR_TLOWM_Msk (0xffu << TWI_SMBTR_TLOWM_Pos) /**< \brief (TWI_SMBTR) Master Clock Stretch Maximum Cycles */
#define TWI_SMBTR_TLOWM(value) ((TWI_SMBTR_TLOWM_Msk & ((value) << TWI_SMBTR_TLOWM_Pos)))
#define TWI_SMBTR_THMAX_Pos 24
#define TWI_SMBTR_THMAX_Msk (0xffu << TWI_SMBTR_THMAX_Pos) /**< \brief (TWI_SMBTR) Clock High Maximum Cycles */
#define TWI_SMBTR_THMAX(value) ((TWI_SMBTR_THMAX_Msk & ((value) << TWI_SMBTR_THMAX_Pos)))
/* -------- TWI_ACR : (TWI Offset: 0x040) TWI Alternative Command Register -------- */
#define TWI_ACR_DATAL_Pos 0
#define TWI_ACR_DATAL_Msk (0xffu << TWI_ACR_DATAL_Pos) /**< \brief (TWI_ACR) Data Length */
#define TWI_ACR_DATAL(value) ((TWI_ACR_DATAL_Msk & ((value) << TWI_ACR_DATAL_Pos)))
#define TWI_ACR_DIR (0x1u << 8) /**< \brief (TWI_ACR) Transfer Direction */
#define TWI_ACR_PEC (0x1u << 9) /**< \brief (TWI_ACR) PEC Request (SMBus Mode only) */
#define TWI_ACR_NDATAL_Pos 16
#define TWI_ACR_NDATAL_Msk (0xffu << TWI_ACR_NDATAL_Pos) /**< \brief (TWI_ACR) Next Data Length */
#define TWI_ACR_NDATAL(value) ((TWI_ACR_NDATAL_Msk & ((value) << TWI_ACR_NDATAL_Pos)))
#define TWI_ACR_NDIR (0x1u << 24) /**< \brief (TWI_ACR) Next Transfer Direction */
#define TWI_ACR_NPEC (0x1u << 25) /**< \brief (TWI_ACR) Next PEC Request (SMBus Mode only) */
/* -------- TWI_FILTR : (TWI Offset: 0x044) TWI Filter Register -------- */
#define TWI_FILTR_FILT (0x1u << 0) /**< \brief (TWI_FILTR) RX Digital Filter */
#define TWI_FILTR_PADFEN (0x1u << 1) /**< \brief (TWI_FILTR) PAD Filter Enable */
#define TWI_FILTR_PADFCFG (0x1u << 2) /**< \brief (TWI_FILTR) PAD Filter Config */
#define TWI_FILTR_THRES_Pos 8
#define TWI_FILTR_THRES_Msk (0x7u << TWI_FILTR_THRES_Pos) /**< \brief (TWI_FILTR) Digital Filter Threshold */
#define TWI_FILTR_THRES(value) ((TWI_FILTR_THRES_Msk & ((value) << TWI_FILTR_THRES_Pos)))
/* -------- TWI_SWMR : (TWI Offset: 0x04C) TWI SleepWalking Matching Register -------- */
#define TWI_SWMR_SADR1_Pos 0
#define TWI_SWMR_SADR1_Msk (0x7fu << TWI_SWMR_SADR1_Pos) /**< \brief (TWI_SWMR) Slave Address 1 */
#define TWI_SWMR_SADR1(value) ((TWI_SWMR_SADR1_Msk & ((value) << TWI_SWMR_SADR1_Pos)))
#define TWI_SWMR_SADR2_Pos 8
#define TWI_SWMR_SADR2_Msk (0x7fu << TWI_SWMR_SADR2_Pos) /**< \brief (TWI_SWMR) Slave Address 2 */
#define TWI_SWMR_SADR2(value) ((TWI_SWMR_SADR2_Msk & ((value) << TWI_SWMR_SADR2_Pos)))
#define TWI_SWMR_SADR3_Pos 16
#define TWI_SWMR_SADR3_Msk (0x7fu << TWI_SWMR_SADR3_Pos) /**< \brief (TWI_SWMR) Slave Address 3 */
#define TWI_SWMR_SADR3(value) ((TWI_SWMR_SADR3_Msk & ((value) << TWI_SWMR_SADR3_Pos)))
#define TWI_SWMR_DATAM_Pos 24
#define TWI_SWMR_DATAM_Msk (0xffu << TWI_SWMR_DATAM_Pos) /**< \brief (TWI_SWMR) Data Match */
#define TWI_SWMR_DATAM(value) ((TWI_SWMR_DATAM_Msk & ((value) << TWI_SWMR_DATAM_Pos)))
/* -------- TWI_WPMR : (TWI Offset: 0x0E4) TWI Write Protection Mode Register -------- */
#define TWI_WPMR_WPEN (0x1u << 0) /**< \brief (TWI_WPMR) Write Protection Enable */
#define TWI_WPMR_WPKEY_Pos 8
#define TWI_WPMR_WPKEY_Msk (0xffffffu << TWI_WPMR_WPKEY_Pos) /**< \brief (TWI_WPMR) Write Protection Key */
#define   TWI_WPMR_WPKEY_PASSWD (0x545749u << 8) /**< \brief (TWI_WPMR) Writing any other value in this field aborts the write operation of the WPEN bit.Always reads as 0 */
/* -------- TWI_WPSR : (TWI Offset: 0x0E8) TWI Write Protection Status Register -------- */
#define TWI_WPSR_WPVS (0x1u << 0) /**< \brief (TWI_WPSR) Write Protect Violation Status */
#define TWI_WPSR_WPVSRC_Pos 8
#define TWI_WPSR_WPVSRC_Msk (0xffffffu << TWI_WPSR_WPVSRC_Pos) /**< \brief (TWI_WPSR) Write Protection Violation Source */
/* -------- TWI_RPR : (TWI Offset: 0x100) Receive Pointer Register -------- */
#define TWI_RPR_RXPTR_Pos 0
#define TWI_RPR_RXPTR_Msk (0xffffffffu << TWI_RPR_RXPTR_Pos) /**< \brief (TWI_RPR) Receive Pointer Register */
#define TWI_RPR_RXPTR(value) ((TWI_RPR_RXPTR_Msk & ((value) << TWI_RPR_RXPTR_Pos)))
/* -------- TWI_RCR : (TWI Offset: 0x104) Receive Counter Register -------- */
#define TWI_RCR_RXCTR_Pos 0
#define TWI_RCR_RXCTR_Msk (0xffffu << TWI_RCR_RXCTR_Pos) /**< \brief (TWI_RCR) Receive Counter Register */
#define TWI_RCR_RXCTR(value) ((TWI_RCR_RXCTR_Msk & ((value) << TWI_RCR_RXCTR_Pos)))
/* -------- TWI_TPR : (TWI Offset: 0x108) Transmit Pointer Register -------- */
#define TWI_TPR_TXPTR_Pos 0
#define TWI_TPR_TXPTR_Msk (0xffffffffu << TWI_TPR_TXPTR_Pos) /**< \brief (TWI_TPR) Transmit Counter Register */
#define TWI_TPR_TXPTR(value) ((TWI_TPR_TXPTR_Msk & ((value) << TWI_TPR_TXPTR_Pos)))
/* -------- TWI_TCR : (TWI Offset: 0x10C) Transmit Counter Register -------- */
#define TWI_TCR_TXCTR_Pos 0
#define TWI_TCR_TXCTR_Msk (0xffffu << TWI_TCR_TXCTR_Pos) /**< \brief (TWI_TCR) Transmit Counter Register */
#define TWI_TCR_TXCTR(value) ((TWI_TCR_TXCTR_Msk & ((value) << TWI_TCR_TXCTR_Pos)))
/* -------- TWI_RNPR : (TWI Offset: 0x110) Receive Next Pointer Register -------- */
#define TWI_RNPR_RXNPTR_Pos 0
#define TWI_RNPR_RXNPTR_Msk (0xffffffffu << TWI_RNPR_RXNPTR_Pos) /**< \brief (TWI_RNPR) Receive Next Pointer */
#define TWI_RNPR_RXNPTR(value) ((TWI_RNPR_RXNPTR_Msk & ((value) << TWI_RNPR_RXNPTR_Pos)))
/* -------- TWI_RNCR : (TWI Offset: 0x114) Receive Next Counter Register -------- */
#define TWI_RNCR_RXNCTR_Pos 0
#define TWI_RNCR_RXNCTR_Msk (0xffffu << TWI_RNCR_RXNCTR_Pos) /**< \brief (TWI_RNCR) Receive Next Counter */
#define TWI_RNCR_RXNCTR(value) ((TWI_RNCR_RXNCTR_Msk & ((value) << TWI_RNCR_RXNCTR_Pos)))
/* -------- TWI_TNPR : (TWI Offset: 0x118) Transmit Next Pointer Register -------- */
#define TWI_TNPR_TXNPTR_Pos 0
#define TWI_TNPR_TXNPTR_Msk (0xffffffffu << TWI_TNPR_TXNPTR_Pos) /**< \brief (TWI_TNPR) Transmit Next Pointer */
#define TWI_TNPR_TXNPTR(value) ((TWI_TNPR_TXNPTR_Msk & ((value) << TWI_TNPR_TXNPTR_Pos)))
/* -------- TWI_TNCR : (TWI Offset: 0x11C) Transmit Next Counter Register -------- */
#define TWI_TNCR_TXNCTR_Pos 0
#define TWI_TNCR_TXNCTR_Msk (0xffffu << TWI_TNCR_TXNCTR_Pos) /**< \brief (TWI_TNCR) Transmit Counter Next */
#define TWI_TNCR_TXNCTR(value) ((TWI_TNCR_TXNCTR_Msk & ((value) << TWI_TNCR_TXNCTR_Pos)))
/* -------- TWI_PTCR : (TWI Offset: 0x120) Transfer Control Register -------- */
#define TWI_PTCR_RXTEN (0x1u << 0) /**< \brief (TWI_PTCR) Receiver Transfer Enable */
#define TWI_PTCR_RXTDIS (0x1u << 1) /**< \brief (TWI_PTCR) Receiver Transfer Disable */
#define TWI_PTCR_TXTEN (0x1u << 8) /**< \brief (TWI_PTCR) Transmitter Transfer Enable */
#define TWI_PTCR_TXTDIS (0x1u << 9) /**< \brief (TWI_PTCR) Transmitter Transfer Disable */
#define TWI_PTCR_RXCBEN (0x1u << 16) /**< \brief (TWI_PTCR) Receiver Circular Buffer Enable */
#define TWI_PTCR_RXCBDIS (0x1u << 17) /**< \brief (TWI_PTCR) Receiver Circular Buffer Disable */
#define TWI_PTCR_TXCBEN (0x1u << 18) /**< \brief (TWI_PTCR) Transmitter Circular Buffer Enable */
#define TWI_PTCR_TXCBDIS (0x1u << 19) /**< \brief (TWI_PTCR) Transmitter Circular Buffer Disable */
#define TWI_PTCR_ERRCLR (0x1u << 24) /**< \brief (TWI_PTCR) Transfer Bus Error Clear */
/* -------- TWI_PTSR : (TWI Offset: 0x124) Transfer Status Register -------- */
#define TWI_PTSR_RXTEN (0x1u << 0) /**< \brief (TWI_PTSR) Receiver Transfer Enable */
#define TWI_PTSR_TXTEN (0x1u << 8) /**< \brief (TWI_PTSR) Transmitter Transfer Enable */
#define TWI_PTSR_RXCBEN (0x1u << 16) /**< \brief (TWI_PTSR) Receiver Transfer Enable */
#define TWI_PTSR_TXCBEN (0x1u << 18) /**< \brief (TWI_PTSR) Transmitter Transfer Enable */
#define TWI_PTSR_ERR (0x1u << 24) /**< \brief (TWI_PTSR) Transfer Bus Error (clear on read) */

/*@}*/


#endif /* _SAMG55_TWI_COMPONENT_ */
