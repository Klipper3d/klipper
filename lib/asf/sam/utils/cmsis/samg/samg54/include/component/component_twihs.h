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

#ifndef _SAMG54_TWIHS_COMPONENT_
#define _SAMG54_TWIHS_COMPONENT_

/* ============================================================================= */
/**  SOFTWARE API DEFINITION FOR Two-wire Interface High Speed */
/* ============================================================================= */
/** \addtogroup SAMG54_TWIHS Two-wire Interface High Speed */
/*@{*/

#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
/** \brief Twihs hardware registers */
typedef struct {
  __O  uint32_t TWIHS_CR;      /**< \brief (Twihs Offset: 0x00) Control Register */
  __IO uint32_t TWIHS_MMR;     /**< \brief (Twihs Offset: 0x04) Master Mode Register */
  __IO uint32_t TWIHS_SMR;     /**< \brief (Twihs Offset: 0x08) Slave Mode Register */
  __IO uint32_t TWIHS_IADR;    /**< \brief (Twihs Offset: 0x0C) Internal Address Register */
  __IO uint32_t TWIHS_CWGR;    /**< \brief (Twihs Offset: 0x10) Clock Waveform Generator Register */
  __I  uint32_t Reserved1[3];
  __I  uint32_t TWIHS_SR;      /**< \brief (Twihs Offset: 0x20) Status Register */
  __O  uint32_t TWIHS_IER;     /**< \brief (Twihs Offset: 0x24) Interrupt Enable Register */
  __O  uint32_t TWIHS_IDR;     /**< \brief (Twihs Offset: 0x28) Interrupt Disable Register */
  __I  uint32_t TWIHS_IMR;     /**< \brief (Twihs Offset: 0x2C) Interrupt Mask Register */
  __I  uint32_t TWIHS_RHR;     /**< \brief (Twihs Offset: 0x30) Receive Holding Register */
  __O  uint32_t TWIHS_THR;     /**< \brief (Twihs Offset: 0x34) Transmit Holding Register */
  __IO uint32_t TWIHS_SMBTR;   /**< \brief (Twihs Offset: 0x38) SMBus Timing Register */
  __I  uint32_t Reserved2[1];
  __IO uint32_t TWIHS_ACR;     /**< \brief (Twihs Offset: 0x40) Alternative Command Register */
  __IO uint32_t TWIHS_FILTR;   /**< \brief (Twihs Offset: 0x44) Filter Register */
  __I  uint32_t Reserved3[1];
  __IO uint32_t TWIHS_SWMR;    /**< \brief (Twihs Offset: 0x4C) SleepWalking Matching Register */
  __I  uint32_t Reserved4[37];
  __IO uint32_t TWIHS_WPMR;    /**< \brief (Twihs Offset: 0xE4) Protection Mode Register */
  __I  uint32_t TWIHS_WPSR;    /**< \brief (Twihs Offset: 0xE8) Protection Status Register */
  __I  uint32_t Reserved5[5];
  __IO uint32_t TWIHS_RPR;     /**< \brief (Twihs Offset: 0x100) Receive Pointer Register */
  __IO uint32_t TWIHS_RCR;     /**< \brief (Twihs Offset: 0x104) Receive Counter Register */
  __IO uint32_t TWIHS_TPR;     /**< \brief (Twihs Offset: 0x108) Transmit Pointer Register */
  __IO uint32_t TWIHS_TCR;     /**< \brief (Twihs Offset: 0x10C) Transmit Counter Register */
  __IO uint32_t TWIHS_RNPR;    /**< \brief (Twihs Offset: 0x110) Receive Next Pointer Register */
  __IO uint32_t TWIHS_RNCR;    /**< \brief (Twihs Offset: 0x114) Receive Next Counter Register */
  __IO uint32_t TWIHS_TNPR;    /**< \brief (Twihs Offset: 0x118) Transmit Next Pointer Register */
  __IO uint32_t TWIHS_TNCR;    /**< \brief (Twihs Offset: 0x11C) Transmit Next Counter Register */
  __O  uint32_t TWIHS_PTCR;    /**< \brief (Twihs Offset: 0x120) Transfer Control Register */
  __I  uint32_t TWIHS_PTSR;    /**< \brief (Twihs Offset: 0x124) Transfer Status Register */
} Twihs;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */
/* -------- TWIHS_CR : (TWIHS Offset: 0x00) Control Register -------- */
#define TWIHS_CR_START (0x1u << 0) /**< \brief (TWIHS_CR) Send a START Condition */
#define TWIHS_CR_STOP (0x1u << 1) /**< \brief (TWIHS_CR) Send a STOP Condition */
#define TWIHS_CR_MSEN (0x1u << 2) /**< \brief (TWIHS_CR) TWI Master Mode Enabled */
#define TWIHS_CR_MSDIS (0x1u << 3) /**< \brief (TWIHS_CR) TWI Master Mode Disabled */
#define TWIHS_CR_SVEN (0x1u << 4) /**< \brief (TWIHS_CR) TWI Slave Mode Enabled */
#define TWIHS_CR_SVDIS (0x1u << 5) /**< \brief (TWIHS_CR) TWI Slave Mode Disabled */
#define TWIHS_CR_QUICK (0x1u << 6) /**< \brief (TWIHS_CR) SMBUS Quick Command */
#define TWIHS_CR_SWRST (0x1u << 7) /**< \brief (TWIHS_CR) Software Reset */
#define TWIHS_CR_HSEN (0x1u << 8) /**< \brief (TWIHS_CR) TWI High-Speed Mode Enabled */
#define TWIHS_CR_HSDIS (0x1u << 9) /**< \brief (TWIHS_CR) TWI High-Speed Mode Disabled */
#define TWIHS_CR_SMBEN (0x1u << 10) /**< \brief (TWIHS_CR) SMBus Mode Enabled */
#define TWIHS_CR_SMBDIS (0x1u << 11) /**< \brief (TWIHS_CR) SMBus Mode Disabled */
#define TWIHS_CR_PECEN (0x1u << 12) /**< \brief (TWIHS_CR) Packet Error Checking Enable */
#define TWIHS_CR_PECDIS (0x1u << 13) /**< \brief (TWIHS_CR) Packet Error Checking Disable */
#define TWIHS_CR_PECRQ (0x1u << 14) /**< \brief (TWIHS_CR) PEC Request */
#define TWIHS_CR_CLEAR (0x1u << 15) /**< \brief (TWIHS_CR) Bus CLEAR Command */
#define TWIHS_CR_ACMEN (0x1u << 16) /**< \brief (TWIHS_CR) Alternative Command Mode Enable */
#define TWIHS_CR_ACMDIS (0x1u << 17) /**< \brief (TWIHS_CR) Alternative Command Mode Disable */
/* -------- TWIHS_MMR : (TWIHS Offset: 0x04) Master Mode Register -------- */
#define TWIHS_MMR_IADRSZ_Pos 8
#define TWIHS_MMR_IADRSZ_Msk (0x3u << TWIHS_MMR_IADRSZ_Pos) /**< \brief (TWIHS_MMR) Internal Device Address Size */
#define   TWIHS_MMR_IADRSZ_NONE (0x0u << 8) /**< \brief (TWIHS_MMR) No internal device address */
#define   TWIHS_MMR_IADRSZ_1_BYTE (0x1u << 8) /**< \brief (TWIHS_MMR) One-byte internal device address */
#define   TWIHS_MMR_IADRSZ_2_BYTE (0x2u << 8) /**< \brief (TWIHS_MMR) Two-byte internal device address */
#define   TWIHS_MMR_IADRSZ_3_BYTE (0x3u << 8) /**< \brief (TWIHS_MMR) Three-byte internal device address */
#define TWIHS_MMR_MREAD (0x1u << 12) /**< \brief (TWIHS_MMR) Master Read Direction */
#define TWIHS_MMR_DADR_Pos 16
#define TWIHS_MMR_DADR_Msk (0x7fu << TWIHS_MMR_DADR_Pos) /**< \brief (TWIHS_MMR) Device Address */
#define TWIHS_MMR_DADR(value) ((TWIHS_MMR_DADR_Msk & ((value) << TWIHS_MMR_DADR_Pos)))
/* -------- TWIHS_SMR : (TWIHS Offset: 0x08) Slave Mode Register -------- */
#define TWIHS_SMR_NACKEN (0x1u << 0) /**< \brief (TWIHS_SMR) Slave Receiver Data Phase NACK enable */
#define TWIHS_SMR_SMDA (0x1u << 2) /**< \brief (TWIHS_SMR) SMBus Default Address */
#define TWIHS_SMR_SMHH (0x1u << 3) /**< \brief (TWIHS_SMR) SMBus Host Header */
#define TWIHS_SMR_SCLWSDIS (0x1u << 6) /**< \brief (TWIHS_SMR) Clock Wait State Disable */
#define TWIHS_SMR_MASK_Pos 8
#define TWIHS_SMR_MASK_Msk (0x7fu << TWIHS_SMR_MASK_Pos) /**< \brief (TWIHS_SMR) Slave Address Mask */
#define TWIHS_SMR_MASK(value) ((TWIHS_SMR_MASK_Msk & ((value) << TWIHS_SMR_MASK_Pos)))
#define TWIHS_SMR_SADR_Pos 16
#define TWIHS_SMR_SADR_Msk (0x7fu << TWIHS_SMR_SADR_Pos) /**< \brief (TWIHS_SMR) Slave Address */
#define TWIHS_SMR_SADR(value) ((TWIHS_SMR_SADR_Msk & ((value) << TWIHS_SMR_SADR_Pos)))
#define TWIHS_SMR_SADR1EN (0x1u << 28) /**< \brief (TWIHS_SMR) Slave Address 1 Enable */
#define TWIHS_SMR_SADR2EN (0x1u << 29) /**< \brief (TWIHS_SMR) Slave Address 2 Enable */
#define TWIHS_SMR_SADR3EN (0x1u << 30) /**< \brief (TWIHS_SMR) Slave Address 3 Enable */
#define TWIHS_SMR_DATAMEN (0x1u << 31) /**< \brief (TWIHS_SMR) Data Matching Enable */
/* -------- TWIHS_IADR : (TWIHS Offset: 0x0C) Internal Address Register -------- */
#define TWIHS_IADR_IADR_Pos 0
#define TWIHS_IADR_IADR_Msk (0xffffffu << TWIHS_IADR_IADR_Pos) /**< \brief (TWIHS_IADR) Internal Address */
#define TWIHS_IADR_IADR(value) ((TWIHS_IADR_IADR_Msk & ((value) << TWIHS_IADR_IADR_Pos)))
/* -------- TWIHS_CWGR : (TWIHS Offset: 0x10) Clock Waveform Generator Register -------- */
#define TWIHS_CWGR_CLDIV_Pos 0
#define TWIHS_CWGR_CLDIV_Msk (0xffu << TWIHS_CWGR_CLDIV_Pos) /**< \brief (TWIHS_CWGR) Clock Low Divider */
#define TWIHS_CWGR_CLDIV(value) ((TWIHS_CWGR_CLDIV_Msk & ((value) << TWIHS_CWGR_CLDIV_Pos)))
#define TWIHS_CWGR_CHDIV_Pos 8
#define TWIHS_CWGR_CHDIV_Msk (0xffu << TWIHS_CWGR_CHDIV_Pos) /**< \brief (TWIHS_CWGR) Clock High Divider */
#define TWIHS_CWGR_CHDIV(value) ((TWIHS_CWGR_CHDIV_Msk & ((value) << TWIHS_CWGR_CHDIV_Pos)))
#define TWIHS_CWGR_CKDIV_Pos 16
#define TWIHS_CWGR_CKDIV_Msk (0x7u << TWIHS_CWGR_CKDIV_Pos) /**< \brief (TWIHS_CWGR) Clock Divider */
#define TWIHS_CWGR_CKDIV(value) ((TWIHS_CWGR_CKDIV_Msk & ((value) << TWIHS_CWGR_CKDIV_Pos)))
#define TWIHS_CWGR_HOLD_Pos 24
#define TWIHS_CWGR_HOLD_Msk (0x1fu << TWIHS_CWGR_HOLD_Pos) /**< \brief (TWIHS_CWGR) TWD Hold Time Versus TWCK Falling */
#define TWIHS_CWGR_HOLD(value) ((TWIHS_CWGR_HOLD_Msk & ((value) << TWIHS_CWGR_HOLD_Pos)))
/* -------- TWIHS_SR : (TWIHS Offset: 0x20) Status Register -------- */
#define TWIHS_SR_TXCOMP (0x1u << 0) /**< \brief (TWIHS_SR) Transmission Completed (automatically set / reset) */
#define TWIHS_SR_RXRDY (0x1u << 1) /**< \brief (TWIHS_SR) Receive Holding Register Ready (automatically set / reset) */
#define TWIHS_SR_TXRDY (0x1u << 2) /**< \brief (TWIHS_SR) Transmit Holding Register Ready (automatically set / reset) */
#define TWIHS_SR_SVREAD (0x1u << 3) /**< \brief (TWIHS_SR) Slave Read (automatically set / reset) */
#define TWIHS_SR_SVACC (0x1u << 4) /**< \brief (TWIHS_SR) Slave Access (automatically set / reset) */
#define TWIHS_SR_GACC (0x1u << 5) /**< \brief (TWIHS_SR) General Call Access (clear on read) */
#define TWIHS_SR_OVRE (0x1u << 6) /**< \brief (TWIHS_SR) Overrun Error (clear on read) */
#define TWIHS_SR_UNRE (0x1u << 7) /**< \brief (TWIHS_SR) Underrun Error (clear on read) */
#define TWIHS_SR_NACK (0x1u << 8) /**< \brief (TWIHS_SR) Not Acknowledged (clear on read) */
#define TWIHS_SR_ARBLST (0x1u << 9) /**< \brief (TWIHS_SR) Arbitration Lost (clear on read) */
#define TWIHS_SR_SCLWS (0x1u << 10) /**< \brief (TWIHS_SR) Clock Wait State (automatically set / reset) */
#define TWIHS_SR_EOSACC (0x1u << 11) /**< \brief (TWIHS_SR) End Of Slave Access (clear on read) */
#define TWIHS_SR_ENDRX (0x1u << 12) /**< \brief (TWIHS_SR) End of RX Buffer */
#define TWIHS_SR_ENDTX (0x1u << 13) /**< \brief (TWIHS_SR) End of TX Buffer */
#define TWIHS_SR_RXBUFF (0x1u << 14) /**< \brief (TWIHS_SR) RX Buffer Full */
#define TWIHS_SR_TXBUFE (0x1u << 15) /**< \brief (TWIHS_SR) TX Buffer Empty */
#define TWIHS_SR_MCACK (0x1u << 16) /**< \brief (TWIHS_SR) Master Code Acknowledge */
#define TWIHS_SR_TOUT (0x1u << 18) /**< \brief (TWIHS_SR) Timeout Error */
#define TWIHS_SR_PECERR (0x1u << 19) /**< \brief (TWIHS_SR) PEC Error */
#define TWIHS_SR_SMBDAM (0x1u << 20) /**< \brief (TWIHS_SR) SMBus Default Address Match */
#define TWIHS_SR_SMBHHM (0x1u << 21) /**< \brief (TWIHS_SR) SMBus Host Header Address Match */
#define TWIHS_SR_SCL (0x1u << 24) /**< \brief (TWIHS_SR) SCL line value */
#define TWIHS_SR_SDA (0x1u << 25) /**< \brief (TWIHS_SR) SDA line value */
/* -------- TWIHS_IER : (TWIHS Offset: 0x24) Interrupt Enable Register -------- */
#define TWIHS_IER_TXCOMP (0x1u << 0) /**< \brief (TWIHS_IER) Transmission Completed Interrupt Enable */
#define TWIHS_IER_RXRDY (0x1u << 1) /**< \brief (TWIHS_IER) Receive Holding Register Ready Interrupt Enable */
#define TWIHS_IER_TXRDY (0x1u << 2) /**< \brief (TWIHS_IER) Transmit Holding Register Ready Interrupt Enable */
#define TWIHS_IER_SVACC (0x1u << 4) /**< \brief (TWIHS_IER) Slave Access Interrupt Enable */
#define TWIHS_IER_GACC (0x1u << 5) /**< \brief (TWIHS_IER) General Call Access Interrupt Enable */
#define TWIHS_IER_OVRE (0x1u << 6) /**< \brief (TWIHS_IER) Overrun Error Interrupt Enable */
#define TWIHS_IER_UNRE (0x1u << 7) /**< \brief (TWIHS_IER) Underrun Error Interrupt Enable */
#define TWIHS_IER_NACK (0x1u << 8) /**< \brief (TWIHS_IER) Not Acknowledge Interrupt Enable */
#define TWIHS_IER_ARBLST (0x1u << 9) /**< \brief (TWIHS_IER) Arbitration Lost Interrupt Enable */
#define TWIHS_IER_SCL_WS (0x1u << 10) /**< \brief (TWIHS_IER) Clock Wait State Interrupt Enable */
#define TWIHS_IER_EOSACC (0x1u << 11) /**< \brief (TWIHS_IER) End Of Slave Access Interrupt Enable */
#define TWIHS_IER_ENDRX (0x1u << 12) /**< \brief (TWIHS_IER) End of Receive Buffer Interrupt Enable */
#define TWIHS_IER_ENDTX (0x1u << 13) /**< \brief (TWIHS_IER) End of Transmit Buffer Interrupt Enable */
#define TWIHS_IER_RXBUFF (0x1u << 14) /**< \brief (TWIHS_IER) Receive Buffer Full Interrupt Enable */
#define TWIHS_IER_TXBUFE (0x1u << 15) /**< \brief (TWIHS_IER) Transmit Buffer Empty Interrupt Enable */
#define TWIHS_IER_MCACK (0x1u << 16) /**< \brief (TWIHS_IER) Master Code Acknowledge Interrupt Enable */
#define TWIHS_IER_TOUT (0x1u << 18) /**< \brief (TWIHS_IER) Timeout Error Interrupt Enable */
#define TWIHS_IER_PECERR (0x1u << 19) /**< \brief (TWIHS_IER) PEC Error Interrupt Enable */
#define TWIHS_IER_SMBDAM (0x1u << 20) /**< \brief (TWIHS_IER) SMBus Default Address Match Interrupt Enable */
#define TWIHS_IER_SMBHHM (0x1u << 21) /**< \brief (TWIHS_IER) SMBus Host Header Address Match Interrupt Enable */
/* -------- TWIHS_IDR : (TWIHS Offset: 0x28) Interrupt Disable Register -------- */
#define TWIHS_IDR_TXCOMP (0x1u << 0) /**< \brief (TWIHS_IDR) Transmission Completed Interrupt Disable */
#define TWIHS_IDR_RXRDY (0x1u << 1) /**< \brief (TWIHS_IDR) Receive Holding Register Ready Interrupt Disable */
#define TWIHS_IDR_TXRDY (0x1u << 2) /**< \brief (TWIHS_IDR) Transmit Holding Register Ready Interrupt Disable */
#define TWIHS_IDR_SVACC (0x1u << 4) /**< \brief (TWIHS_IDR) Slave Access Interrupt Disable */
#define TWIHS_IDR_GACC (0x1u << 5) /**< \brief (TWIHS_IDR) General Call Access Interrupt Disable */
#define TWIHS_IDR_OVRE (0x1u << 6) /**< \brief (TWIHS_IDR) Overrun Error Interrupt Disable */
#define TWIHS_IDR_UNRE (0x1u << 7) /**< \brief (TWIHS_IDR) Underrun Error Interrupt Disable */
#define TWIHS_IDR_NACK (0x1u << 8) /**< \brief (TWIHS_IDR) Not Acknowledge Interrupt Disable */
#define TWIHS_IDR_ARBLST (0x1u << 9) /**< \brief (TWIHS_IDR) Arbitration Lost Interrupt Disable */
#define TWIHS_IDR_SCL_WS (0x1u << 10) /**< \brief (TWIHS_IDR) Clock Wait State Interrupt Disable */
#define TWIHS_IDR_EOSACC (0x1u << 11) /**< \brief (TWIHS_IDR) End Of Slave Access Interrupt Disable */
#define TWIHS_IDR_ENDRX (0x1u << 12) /**< \brief (TWIHS_IDR) End of Receive Buffer Interrupt Disable */
#define TWIHS_IDR_ENDTX (0x1u << 13) /**< \brief (TWIHS_IDR) End of Transmit Buffer Interrupt Disable */
#define TWIHS_IDR_RXBUFF (0x1u << 14) /**< \brief (TWIHS_IDR) Receive Buffer Full Interrupt Disable */
#define TWIHS_IDR_TXBUFE (0x1u << 15) /**< \brief (TWIHS_IDR) Transmit Buffer Empty Interrupt Disable */
#define TWIHS_IDR_MCACK (0x1u << 16) /**< \brief (TWIHS_IDR) Master Code Acknowledge Interrupt Disable */
#define TWIHS_IDR_TOUT (0x1u << 18) /**< \brief (TWIHS_IDR) Timeout Error Interrupt Disable */
#define TWIHS_IDR_PECERR (0x1u << 19) /**< \brief (TWIHS_IDR) PEC Error Interrupt Disable */
#define TWIHS_IDR_SMBDAM (0x1u << 20) /**< \brief (TWIHS_IDR) SMBus Default Address Match Interrupt Disable */
#define TWIHS_IDR_SMBHHM (0x1u << 21) /**< \brief (TWIHS_IDR) SMBus Host Header Address Match Interrupt Disable */
/* -------- TWIHS_IMR : (TWIHS Offset: 0x2C) Interrupt Mask Register -------- */
#define TWIHS_IMR_TXCOMP (0x1u << 0) /**< \brief (TWIHS_IMR) Transmission Completed Interrupt Mask */
#define TWIHS_IMR_RXRDY (0x1u << 1) /**< \brief (TWIHS_IMR) Receive Holding Register Ready Interrupt Mask */
#define TWIHS_IMR_TXRDY (0x1u << 2) /**< \brief (TWIHS_IMR) Transmit Holding Register Ready Interrupt Mask */
#define TWIHS_IMR_SVACC (0x1u << 4) /**< \brief (TWIHS_IMR) Slave Access Interrupt Mask */
#define TWIHS_IMR_GACC (0x1u << 5) /**< \brief (TWIHS_IMR) General Call Access Interrupt Mask */
#define TWIHS_IMR_OVRE (0x1u << 6) /**< \brief (TWIHS_IMR) Overrun Error Interrupt Mask */
#define TWIHS_IMR_UNRE (0x1u << 7) /**< \brief (TWIHS_IMR) Underrun Error Interrupt Mask */
#define TWIHS_IMR_NACK (0x1u << 8) /**< \brief (TWIHS_IMR) Not Acknowledge Interrupt Mask */
#define TWIHS_IMR_ARBLST (0x1u << 9) /**< \brief (TWIHS_IMR) Arbitration Lost Interrupt Mask */
#define TWIHS_IMR_SCL_WS (0x1u << 10) /**< \brief (TWIHS_IMR) Clock Wait State Interrupt Mask */
#define TWIHS_IMR_EOSACC (0x1u << 11) /**< \brief (TWIHS_IMR) End Of Slave Access Interrupt Mask */
#define TWIHS_IMR_ENDRX (0x1u << 12) /**< \brief (TWIHS_IMR) End of Receive Buffer Interrupt Mask */
#define TWIHS_IMR_ENDTX (0x1u << 13) /**< \brief (TWIHS_IMR) End of Transmit Buffer Interrupt Mask */
#define TWIHS_IMR_RXBUFF (0x1u << 14) /**< \brief (TWIHS_IMR) Receive Buffer Full Interrupt Mask */
#define TWIHS_IMR_TXBUFE (0x1u << 15) /**< \brief (TWIHS_IMR) Transmit Buffer Empty Interrupt Mask */
#define TWIHS_IMR_MCACK (0x1u << 16) /**< \brief (TWIHS_IMR) Master Code Acknowledge Interrupt Mask */
#define TWIHS_IMR_TOUT (0x1u << 18) /**< \brief (TWIHS_IMR) Timeout Error Interrupt Mask */
#define TWIHS_IMR_PECERR (0x1u << 19) /**< \brief (TWIHS_IMR) PEC Error Interrupt Mask */
#define TWIHS_IMR_SMBDAM (0x1u << 20) /**< \brief (TWIHS_IMR) SMBus Default Address Match Interrupt Mask */
#define TWIHS_IMR_SMBHHM (0x1u << 21) /**< \brief (TWIHS_IMR) SMBus Host Header Address Match Interrupt Mask */
/* -------- TWIHS_RHR : (TWIHS Offset: 0x30) Receive Holding Register -------- */
#define TWIHS_RHR_RXDATA_Pos 0
#define TWIHS_RHR_RXDATA_Msk (0xffu << TWIHS_RHR_RXDATA_Pos) /**< \brief (TWIHS_RHR) Master or Slave Receive Holding Data */
/* -------- TWIHS_THR : (TWIHS Offset: 0x34) Transmit Holding Register -------- */
#define TWIHS_THR_TXDATA_Pos 0
#define TWIHS_THR_TXDATA_Msk (0xffu << TWIHS_THR_TXDATA_Pos) /**< \brief (TWIHS_THR) Master or Slave Transmit Holding Data */
#define TWIHS_THR_TXDATA(value) ((TWIHS_THR_TXDATA_Msk & ((value) << TWIHS_THR_TXDATA_Pos)))
/* -------- TWIHS_SMBTR : (TWIHS Offset: 0x38) SMBus Timing Register -------- */
#define TWIHS_SMBTR_PRESC_Pos 0
#define TWIHS_SMBTR_PRESC_Msk (0xfu << TWIHS_SMBTR_PRESC_Pos) /**< \brief (TWIHS_SMBTR) SMBus Clock Prescaler */
#define TWIHS_SMBTR_PRESC(value) ((TWIHS_SMBTR_PRESC_Msk & ((value) << TWIHS_SMBTR_PRESC_Pos)))
#define TWIHS_SMBTR_TLOWS_Pos 8
#define TWIHS_SMBTR_TLOWS_Msk (0xffu << TWIHS_SMBTR_TLOWS_Pos) /**< \brief (TWIHS_SMBTR) Slave Clock Stretch Maximum Cycles */
#define TWIHS_SMBTR_TLOWS(value) ((TWIHS_SMBTR_TLOWS_Msk & ((value) << TWIHS_SMBTR_TLOWS_Pos)))
#define TWIHS_SMBTR_TLOWM_Pos 16
#define TWIHS_SMBTR_TLOWM_Msk (0xffu << TWIHS_SMBTR_TLOWM_Pos) /**< \brief (TWIHS_SMBTR) Master Clock Stretch Maximum Cycles */
#define TWIHS_SMBTR_TLOWM(value) ((TWIHS_SMBTR_TLOWM_Msk & ((value) << TWIHS_SMBTR_TLOWM_Pos)))
#define TWIHS_SMBTR_THMAX_Pos 24
#define TWIHS_SMBTR_THMAX_Msk (0xffu << TWIHS_SMBTR_THMAX_Pos) /**< \brief (TWIHS_SMBTR) Clock High Maximum Cycles */
#define TWIHS_SMBTR_THMAX(value) ((TWIHS_SMBTR_THMAX_Msk & ((value) << TWIHS_SMBTR_THMAX_Pos)))
/* -------- TWIHS_ACR : (TWIHS Offset: 0x40) Alternative Command Register -------- */
#define TWIHS_ACR_DATAL_Pos 0
#define TWIHS_ACR_DATAL_Msk (0xffu << TWIHS_ACR_DATAL_Pos) /**< \brief (TWIHS_ACR) Data Length */
#define TWIHS_ACR_DATAL(value) ((TWIHS_ACR_DATAL_Msk & ((value) << TWIHS_ACR_DATAL_Pos)))
#define TWIHS_ACR_DIR (0x1u << 8) /**< \brief (TWIHS_ACR) Transfer Direction */
#define TWIHS_ACR_PEC (0x1u << 9) /**< \brief (TWIHS_ACR) PEC Request (SMBus Mode only) */
#define TWIHS_ACR_NDATAL_Pos 16
#define TWIHS_ACR_NDATAL_Msk (0xffu << TWIHS_ACR_NDATAL_Pos) /**< \brief (TWIHS_ACR) Next Data Length */
#define TWIHS_ACR_NDATAL(value) ((TWIHS_ACR_NDATAL_Msk & ((value) << TWIHS_ACR_NDATAL_Pos)))
#define TWIHS_ACR_NDIR (0x1u << 24) /**< \brief (TWIHS_ACR) Next Transfer Direction */
#define TWIHS_ACR_NPEC (0x1u << 25) /**< \brief (TWIHS_ACR) Next PEC Request (SMBus Mode only) */
/* -------- TWIHS_FILTR : (TWIHS Offset: 0x44) Filter Register -------- */
#define TWIHS_FILTR_FILT (0x1u << 0) /**< \brief (TWIHS_FILTR) RX Digital Filter */
#define TWIHS_FILTR_PADFEN (0x1u << 1) /**< \brief (TWIHS_FILTR) PAD Filter Enable */
#define TWIHS_FILTR_PADFCFG (0x1u << 2) /**< \brief (TWIHS_FILTR) PAD Filter Config */
#define TWIHS_FILTR_THRES_Pos 8
#define TWIHS_FILTR_THRES_Msk (0x7u << TWIHS_FILTR_THRES_Pos) /**< \brief (TWIHS_FILTR) Digital Filter Threshold */
#define TWIHS_FILTR_THRES(value) ((TWIHS_FILTR_THRES_Msk & ((value) << TWIHS_FILTR_THRES_Pos)))
/* -------- TWIHS_SWMR : (TWIHS Offset: 0x4C) SleepWalking Matching Register -------- */
#define TWIHS_SWMR_SADR1_Pos 0
#define TWIHS_SWMR_SADR1_Msk (0x7fu << TWIHS_SWMR_SADR1_Pos) /**< \brief (TWIHS_SWMR) Slave Address 1 */
#define TWIHS_SWMR_SADR1(value) ((TWIHS_SWMR_SADR1_Msk & ((value) << TWIHS_SWMR_SADR1_Pos)))
#define TWIHS_SWMR_SADR2_Pos 8
#define TWIHS_SWMR_SADR2_Msk (0x7fu << TWIHS_SWMR_SADR2_Pos) /**< \brief (TWIHS_SWMR) Slave Address 2 */
#define TWIHS_SWMR_SADR2(value) ((TWIHS_SWMR_SADR2_Msk & ((value) << TWIHS_SWMR_SADR2_Pos)))
#define TWIHS_SWMR_SADR3_Pos 16
#define TWIHS_SWMR_SADR3_Msk (0x7fu << TWIHS_SWMR_SADR3_Pos) /**< \brief (TWIHS_SWMR) Slave Address 3 */
#define TWIHS_SWMR_SADR3(value) ((TWIHS_SWMR_SADR3_Msk & ((value) << TWIHS_SWMR_SADR3_Pos)))
#define TWIHS_SWMR_DATAM_Pos 24
#define TWIHS_SWMR_DATAM_Msk (0xffu << TWIHS_SWMR_DATAM_Pos) /**< \brief (TWIHS_SWMR) Data Match */
#define TWIHS_SWMR_DATAM(value) ((TWIHS_SWMR_DATAM_Msk & ((value) << TWIHS_SWMR_DATAM_Pos)))
/* -------- TWIHS_WPMR : (TWIHS Offset: 0xE4) Protection Mode Register -------- */
#define TWIHS_WPMR_WPEN (0x1u << 0) /**< \brief (TWIHS_WPMR) Write Protection Enable */
#define TWIHS_WPMR_WPKEY_Pos 8
#define TWIHS_WPMR_WPKEY_Msk (0xffffffu << TWIHS_WPMR_WPKEY_Pos) /**< \brief (TWIHS_WPMR) Write Protection Key */
#define   TWIHS_WPMR_WPKEY_PASSWD (0x545749u << 8) /**< \brief (TWIHS_WPMR) Writing any other value in this field aborts the write operation of the WPEN bit.Always reads as 0 */
/* -------- TWIHS_WPSR : (TWIHS Offset: 0xE8) Protection Status Register -------- */
#define TWIHS_WPSR_WPVS (0x1u << 0) /**< \brief (TWIHS_WPSR) Write Protect Violation Status */
#define TWIHS_WPSR_WPVSRC_Pos 8
#define TWIHS_WPSR_WPVSRC_Msk (0xffffffu << TWIHS_WPSR_WPVSRC_Pos) /**< \brief (TWIHS_WPSR) Write Protection Violation Source */
/* -------- TWIHS_RPR : (TWIHS Offset: 0x100) Receive Pointer Register -------- */
#define TWIHS_RPR_RXPTR_Pos 0
#define TWIHS_RPR_RXPTR_Msk (0xffffffffu << TWIHS_RPR_RXPTR_Pos) /**< \brief (TWIHS_RPR) Receive Pointer Register */
#define TWIHS_RPR_RXPTR(value) ((TWIHS_RPR_RXPTR_Msk & ((value) << TWIHS_RPR_RXPTR_Pos)))
/* -------- TWIHS_RCR : (TWIHS Offset: 0x104) Receive Counter Register -------- */
#define TWIHS_RCR_RXCTR_Pos 0
#define TWIHS_RCR_RXCTR_Msk (0xffffu << TWIHS_RCR_RXCTR_Pos) /**< \brief (TWIHS_RCR) Receive Counter Register */
#define TWIHS_RCR_RXCTR(value) ((TWIHS_RCR_RXCTR_Msk & ((value) << TWIHS_RCR_RXCTR_Pos)))
/* -------- TWIHS_TPR : (TWIHS Offset: 0x108) Transmit Pointer Register -------- */
#define TWIHS_TPR_TXPTR_Pos 0
#define TWIHS_TPR_TXPTR_Msk (0xffffffffu << TWIHS_TPR_TXPTR_Pos) /**< \brief (TWIHS_TPR) Transmit Counter Register */
#define TWIHS_TPR_TXPTR(value) ((TWIHS_TPR_TXPTR_Msk & ((value) << TWIHS_TPR_TXPTR_Pos)))
/* -------- TWIHS_TCR : (TWIHS Offset: 0x10C) Transmit Counter Register -------- */
#define TWIHS_TCR_TXCTR_Pos 0
#define TWIHS_TCR_TXCTR_Msk (0xffffu << TWIHS_TCR_TXCTR_Pos) /**< \brief (TWIHS_TCR) Transmit Counter Register */
#define TWIHS_TCR_TXCTR(value) ((TWIHS_TCR_TXCTR_Msk & ((value) << TWIHS_TCR_TXCTR_Pos)))
/* -------- TWIHS_RNPR : (TWIHS Offset: 0x110) Receive Next Pointer Register -------- */
#define TWIHS_RNPR_RXNPTR_Pos 0
#define TWIHS_RNPR_RXNPTR_Msk (0xffffffffu << TWIHS_RNPR_RXNPTR_Pos) /**< \brief (TWIHS_RNPR) Receive Next Pointer */
#define TWIHS_RNPR_RXNPTR(value) ((TWIHS_RNPR_RXNPTR_Msk & ((value) << TWIHS_RNPR_RXNPTR_Pos)))
/* -------- TWIHS_RNCR : (TWIHS Offset: 0x114) Receive Next Counter Register -------- */
#define TWIHS_RNCR_RXNCTR_Pos 0
#define TWIHS_RNCR_RXNCTR_Msk (0xffffu << TWIHS_RNCR_RXNCTR_Pos) /**< \brief (TWIHS_RNCR) Receive Next Counter */
#define TWIHS_RNCR_RXNCTR(value) ((TWIHS_RNCR_RXNCTR_Msk & ((value) << TWIHS_RNCR_RXNCTR_Pos)))
/* -------- TWIHS_TNPR : (TWIHS Offset: 0x118) Transmit Next Pointer Register -------- */
#define TWIHS_TNPR_TXNPTR_Pos 0
#define TWIHS_TNPR_TXNPTR_Msk (0xffffffffu << TWIHS_TNPR_TXNPTR_Pos) /**< \brief (TWIHS_TNPR) Transmit Next Pointer */
#define TWIHS_TNPR_TXNPTR(value) ((TWIHS_TNPR_TXNPTR_Msk & ((value) << TWIHS_TNPR_TXNPTR_Pos)))
/* -------- TWIHS_TNCR : (TWIHS Offset: 0x11C) Transmit Next Counter Register -------- */
#define TWIHS_TNCR_TXNCTR_Pos 0
#define TWIHS_TNCR_TXNCTR_Msk (0xffffu << TWIHS_TNCR_TXNCTR_Pos) /**< \brief (TWIHS_TNCR) Transmit Counter Next */
#define TWIHS_TNCR_TXNCTR(value) ((TWIHS_TNCR_TXNCTR_Msk & ((value) << TWIHS_TNCR_TXNCTR_Pos)))
/* -------- TWIHS_PTCR : (TWIHS Offset: 0x120) Transfer Control Register -------- */
#define TWIHS_PTCR_RXTEN (0x1u << 0) /**< \brief (TWIHS_PTCR) Receiver Transfer Enable */
#define TWIHS_PTCR_RXTDIS (0x1u << 1) /**< \brief (TWIHS_PTCR) Receiver Transfer Disable */
#define TWIHS_PTCR_TXTEN (0x1u << 8) /**< \brief (TWIHS_PTCR) Transmitter Transfer Enable */
#define TWIHS_PTCR_TXTDIS (0x1u << 9) /**< \brief (TWIHS_PTCR) Transmitter Transfer Disable */
#define TWIHS_PTCR_RXCBEN (0x1u << 16) /**< \brief (TWIHS_PTCR) Receiver Circular Buffer Enable */
#define TWIHS_PTCR_RXCBDIS (0x1u << 17) /**< \brief (TWIHS_PTCR) Receiver Circular Buffer Disable */
#define TWIHS_PTCR_TXCBEN (0x1u << 18) /**< \brief (TWIHS_PTCR) Transmitter Circular Buffer Enable */
#define TWIHS_PTCR_TXCBDIS (0x1u << 19) /**< \brief (TWIHS_PTCR) Transmitter Circular Buffer Disable */
#define TWIHS_PTCR_ERRCLR (0x1u << 24) /**< \brief (TWIHS_PTCR) Transfer Bus Error Clear */
/* -------- TWIHS_PTSR : (TWIHS Offset: 0x124) Transfer Status Register -------- */
#define TWIHS_PTSR_RXTEN (0x1u << 0) /**< \brief (TWIHS_PTSR) Receiver Transfer Enable */
#define TWIHS_PTSR_TXTEN (0x1u << 8) /**< \brief (TWIHS_PTSR) Transmitter Transfer Enable */
#define TWIHS_PTSR_RXCBEN (0x1u << 16) /**< \brief (TWIHS_PTSR) Receiver Transfer Enable */
#define TWIHS_PTSR_TXCBEN (0x1u << 18) /**< \brief (TWIHS_PTSR) Transmitter Transfer Enable */
#define TWIHS_PTSR_ERR (0x1u << 24) /**< \brief (TWIHS_PTSR) Transfer Bus Error */

/*@}*/


#endif /* _SAMG54_TWIHS_COMPONENT_ */
