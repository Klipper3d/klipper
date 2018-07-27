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
#ifndef _SAM4C_AES_COMPONENT_
#define _SAM4C_AES_COMPONENT_

/* ============================================================================= */
/**  SOFTWARE API DEFINITION FOR Advanced Encryption Standard */
/* ============================================================================= */
/** \addtogroup SAM4C_AES Advanced Encryption Standard */
/*@{*/

#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
/** \brief Aes hardware registers */
typedef struct {
  __O  uint32_t AES_CR;        /**< \brief (Aes Offset: 0x00) Control Register */
  __IO uint32_t AES_MR;        /**< \brief (Aes Offset: 0x04) Mode Register */
  __I  uint32_t Reserved1[2];
  __O  uint32_t AES_IER;       /**< \brief (Aes Offset: 0x10) Interrupt Enable Register */
  __O  uint32_t AES_IDR;       /**< \brief (Aes Offset: 0x14) Interrupt Disable Register */
  __I  uint32_t AES_IMR;       /**< \brief (Aes Offset: 0x18) Interrupt Mask Register */
  __I  uint32_t AES_ISR;       /**< \brief (Aes Offset: 0x1C) Interrupt Status Register */
  __O  uint32_t AES_KEYWR[8];  /**< \brief (Aes Offset: 0x20) Key Word Register */
  __O  uint32_t AES_IDATAR[4]; /**< \brief (Aes Offset: 0x40) Input Data Register */
  __I  uint32_t AES_ODATAR[4]; /**< \brief (Aes Offset: 0x50) Output Data Register */
  __O  uint32_t AES_IVR[4];    /**< \brief (Aes Offset: 0x60) Initialization Vector Register */
  __IO uint32_t AES_AADLENR;   /**< \brief (Aes Offset: 0x70) Additional Authenticated Data Length Register */
  __IO uint32_t AES_CLENR;     /**< \brief (Aes Offset: 0x74) Plaintext/Ciphertext Length Register */
  __IO uint32_t AES_GHASHR[4]; /**< \brief (Aes Offset: 0x78) GCM Intermediate Hash Word Register */
  __I  uint32_t AES_TAGR[4];   /**< \brief (Aes Offset: 0x88) GCM Authentication Tag Word Register */
  __I  uint32_t AES_CTRR;      /**< \brief (Aes Offset: 0x98) GCM Encryption Counter Value Register */
  __IO uint32_t AES_GCMHR[4];  /**< \brief (Aes Offset: 0x9C) GCM H World Register */
  __I  uint32_t Reserved2[21];
  __IO uint32_t AES_RPR;       /**< \brief (Aes Offset: 0x100) Receive Pointer Register */
  __IO uint32_t AES_RCR;       /**< \brief (Aes Offset: 0x104) Receive Counter Register */
  __IO uint32_t AES_TPR;       /**< \brief (Aes Offset: 0x108) Transmit Pointer Register */
  __IO uint32_t AES_TCR;       /**< \brief (Aes Offset: 0x10C) Transmit Counter Register */
  __IO uint32_t AES_RNPR;      /**< \brief (Aes Offset: 0x110) Receive Next Pointer Register */
  __IO uint32_t AES_RNCR;      /**< \brief (Aes Offset: 0x114) Receive Next Counter Register */
  __IO uint32_t AES_TNPR;      /**< \brief (Aes Offset: 0x118) Transmit Next Pointer Register */
  __IO uint32_t AES_TNCR;      /**< \brief (Aes Offset: 0x11C) Transmit Next Counter Register */
  __O  uint32_t AES_PTCR;      /**< \brief (Aes Offset: 0x120) Transfer Control Register */
  __I  uint32_t AES_PTSR;      /**< \brief (Aes Offset: 0x124) Transfer Status Register */
} Aes;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */
/* -------- AES_CR : (AES Offset: 0x00) Control Register -------- */
#define AES_CR_START (0x1u << 0) /**< \brief (AES_CR) Start Processing */
#define AES_CR_SWRST (0x1u << 8) /**< \brief (AES_CR) Software Reset */
/* -------- AES_MR : (AES Offset: 0x04) Mode Register -------- */
#define AES_MR_CIPHER (0x1u << 0) /**< \brief (AES_MR) Processing Mode */
#define AES_MR_GTAGEN (0x1u << 1) /**< \brief (AES_MR) GCM Automatic Tag Generation Enable */
#define AES_MR_DUALBUFF (0x1u << 3) /**< \brief (AES_MR) Dual Input Buffer */
#define   AES_MR_DUALBUFF_INACTIVE (0x0u << 3) /**< \brief (AES_MR) AES_IDATARx cannot be written during processing of previous block. */
#define   AES_MR_DUALBUFF_ACTIVE (0x1u << 3) /**< \brief (AES_MR) AES_IDATARx can be written during processing of previous block when SMOD = 0x2. It speeds up the overall runtime of large files. */
#define AES_MR_PROCDLY_Pos 4
#define AES_MR_PROCDLY_Msk (0xfu << AES_MR_PROCDLY_Pos) /**< \brief (AES_MR) Processing Delay */
#define AES_MR_PROCDLY(value) ((AES_MR_PROCDLY_Msk & ((value) << AES_MR_PROCDLY_Pos)))
#define AES_MR_SMOD_Pos 8
#define AES_MR_SMOD_Msk (0x3u << AES_MR_SMOD_Pos) /**< \brief (AES_MR) Start Mode */
#define   AES_MR_SMOD_MANUAL_START (0x0u << 8) /**< \brief (AES_MR) Manual Mode */
#define   AES_MR_SMOD_AUTO_START (0x1u << 8) /**< \brief (AES_MR) Auto Mode */
#define   AES_MR_SMOD_IDATAR0_START (0x2u << 8) /**< \brief (AES_MR) AES_IDATAR0 access only Auto Mode */
#define AES_MR_KEYSIZE_Pos 10
#define AES_MR_KEYSIZE_Msk (0x3u << AES_MR_KEYSIZE_Pos) /**< \brief (AES_MR) Key Size */
#define   AES_MR_KEYSIZE_AES128 (0x0u << 10) /**< \brief (AES_MR) AES Key Size is 128 bits */
#define   AES_MR_KEYSIZE_AES192 (0x1u << 10) /**< \brief (AES_MR) AES Key Size is 192 bits */
#define   AES_MR_KEYSIZE_AES256 (0x2u << 10) /**< \brief (AES_MR) AES Key Size is 256 bits */
#define AES_MR_OPMOD_Pos 12
#define AES_MR_OPMOD_Msk (0x7u << AES_MR_OPMOD_Pos) /**< \brief (AES_MR) Operation Mode */
#define   AES_MR_OPMOD_ECB (0x0u << 12) /**< \brief (AES_MR) ECB: Electronic Code Book mode */
#define   AES_MR_OPMOD_CBC (0x1u << 12) /**< \brief (AES_MR) CBC: Cipher Block Chaining mode */
#define   AES_MR_OPMOD_OFB (0x2u << 12) /**< \brief (AES_MR) OFB: Output Feedback mode */
#define   AES_MR_OPMOD_CFB (0x3u << 12) /**< \brief (AES_MR) CFB: Cipher Feedback mode */
#define   AES_MR_OPMOD_CTR (0x4u << 12) /**< \brief (AES_MR) CTR: Counter mode (16-bit internal counter) */
#define   AES_MR_OPMOD_GCM (0x5u << 12) /**< \brief (AES_MR) GCM: Galois Counter mode */
#define AES_MR_LOD (0x1u << 15) /**< \brief (AES_MR) Last Output Data Mode */
#define AES_MR_CFBS_Pos 16
#define AES_MR_CFBS_Msk (0x7u << AES_MR_CFBS_Pos) /**< \brief (AES_MR) Cipher Feedback Data Size */
#define   AES_MR_CFBS_SIZE_128BIT (0x0u << 16) /**< \brief (AES_MR) 128-bit */
#define   AES_MR_CFBS_SIZE_64BIT (0x1u << 16) /**< \brief (AES_MR) 64-bit */
#define   AES_MR_CFBS_SIZE_32BIT (0x2u << 16) /**< \brief (AES_MR) 32-bit */
#define   AES_MR_CFBS_SIZE_16BIT (0x3u << 16) /**< \brief (AES_MR) 16-bit */
#define   AES_MR_CFBS_SIZE_8BIT (0x4u << 16) /**< \brief (AES_MR) 8-bit */
#define AES_MR_CKEY_Pos 20
#define AES_MR_CKEY_Msk (0xfu << AES_MR_CKEY_Pos) /**< \brief (AES_MR) Key */
#define   AES_MR_CKEY_PASSWD (0xEu << 20) /**< \brief (AES_MR) This field must be written with 0xE the first time that AES_MR is programmed. For subsequent programming of the AES_MR, any value can be written, including that of 0xE.Always reads as 0. */
/* -------- AES_IER : (AES Offset: 0x10) Interrupt Enable Register -------- */
#define AES_IER_DATRDY (0x1u << 0) /**< \brief (AES_IER) Data Ready Interrupt Enable */
#define AES_IER_ENDRX (0x1u << 1) /**< \brief (AES_IER) End of Receive Buffer Interrupt Enable */
#define AES_IER_ENDTX (0x1u << 2) /**< \brief (AES_IER) End of Transmit Buffer Interrupt Enable */
#define AES_IER_RXBUFF (0x1u << 3) /**< \brief (AES_IER) Receive Buffer Full Interrupt Enable */
#define AES_IER_TXBUFE (0x1u << 4) /**< \brief (AES_IER) Transmit Buffer Empty Interrupt Enable */
#define AES_IER_URAD (0x1u << 8) /**< \brief (AES_IER) Unspecified Register Access Detection Interrupt Enable */
/* -------- AES_IDR : (AES Offset: 0x14) Interrupt Disable Register -------- */
#define AES_IDR_DATRDY (0x1u << 0) /**< \brief (AES_IDR) Data Ready Interrupt Disable */
#define AES_IDR_ENDRX (0x1u << 1) /**< \brief (AES_IDR) End of Receive Buffer Interrupt Disable */
#define AES_IDR_ENDTX (0x1u << 2) /**< \brief (AES_IDR) End of Transmit Buffer Interrupt Disable */
#define AES_IDR_RXBUFF (0x1u << 3) /**< \brief (AES_IDR) Receive Buffer Full Interrupt Disable */
#define AES_IDR_TXBUFE (0x1u << 4) /**< \brief (AES_IDR) Transmit Buffer Empty Interrupt Disable */
#define AES_IDR_URAD (0x1u << 8) /**< \brief (AES_IDR) Unspecified Register Access Detection Interrupt Disable */
/* -------- AES_IMR : (AES Offset: 0x18) Interrupt Mask Register -------- */
#define AES_IMR_DATRDY (0x1u << 0) /**< \brief (AES_IMR) Data Ready Interrupt Mask */
#define AES_IMR_ENDRX (0x1u << 1) /**< \brief (AES_IMR) End of Receive Buffer Interrupt Mask */
#define AES_IMR_ENDTX (0x1u << 2) /**< \brief (AES_IMR) End of Transmit Buffer Interrupt Mask */
#define AES_IMR_RXBUFF (0x1u << 3) /**< \brief (AES_IMR) Receive Buffer Full Interrupt Mask */
#define AES_IMR_TXBUFE (0x1u << 4) /**< \brief (AES_IMR) Transmit Buffer Empty Interrupt Mask */
#define AES_IMR_URAD (0x1u << 8) /**< \brief (AES_IMR) Unspecified Register Access Detection Interrupt Mask */
/* -------- AES_ISR : (AES Offset: 0x1C) Interrupt Status Register -------- */
#define AES_ISR_DATRDY (0x1u << 0) /**< \brief (AES_ISR) Data Ready */
#define AES_ISR_ENDRX (0x1u << 1) /**< \brief (AES_ISR) End of RX Buffer */
#define AES_ISR_ENDTX (0x1u << 2) /**< \brief (AES_ISR) End of TX Buffer */
#define AES_ISR_RXBUFF (0x1u << 3) /**< \brief (AES_ISR) RX Buffer Full */
#define AES_ISR_TXBUFE (0x1u << 4) /**< \brief (AES_ISR) TX Buffer Empty */
#define AES_ISR_URAD (0x1u << 8) /**< \brief (AES_ISR) Unspecified Register Access Detection Status */
#define AES_ISR_URAT_Pos 12
#define AES_ISR_URAT_Msk (0xfu << AES_ISR_URAT_Pos) /**< \brief (AES_ISR) Unspecified Register Access: */
#define   AES_ISR_URAT_IDR_WR_PROCESSING (0x0u << 12) /**< \brief (AES_ISR) Input Data Register written during the data processing when SMOD = 0x2 mode. */
#define   AES_ISR_URAT_ODR_RD_PROCESSING (0x1u << 12) /**< \brief (AES_ISR) Output Data Register read during the data processing. */
#define   AES_ISR_URAT_MR_WR_PROCESSING (0x2u << 12) /**< \brief (AES_ISR) Mode Register written during the data processing. */
#define   AES_ISR_URAT_ODR_RD_SUBKGEN (0x3u << 12) /**< \brief (AES_ISR) Output Data Register read during the sub-keys generation. */
#define   AES_ISR_URAT_MR_WR_SUBKGEN (0x4u << 12) /**< \brief (AES_ISR) Mode Register written during the sub-keys generation. */
#define   AES_ISR_URAT_WOR_RD_ACCESS (0x5u << 12) /**< \brief (AES_ISR) Write-only register read access. */
#define AES_ISR_TAGRDY (0x1u << 16) /**< \brief (AES_ISR) GCM Tag Ready */
/* -------- AES_KEYWR[8] : (AES Offset: 0x20) Key Word Register -------- */
#define AES_KEYWR_KEYW_Pos 0
#define AES_KEYWR_KEYW_Msk (0xffffffffu << AES_KEYWR_KEYW_Pos) /**< \brief (AES_KEYWR[8]) Key Word */
#define AES_KEYWR_KEYW(value) ((AES_KEYWR_KEYW_Msk & ((value) << AES_KEYWR_KEYW_Pos)))
/* -------- AES_IDATAR[4] : (AES Offset: 0x40) Input Data Register -------- */
#define AES_IDATAR_IDATA_Pos 0
#define AES_IDATAR_IDATA_Msk (0xffffffffu << AES_IDATAR_IDATA_Pos) /**< \brief (AES_IDATAR[4]) Input Data Word */
#define AES_IDATAR_IDATA(value) ((AES_IDATAR_IDATA_Msk & ((value) << AES_IDATAR_IDATA_Pos)))
/* -------- AES_ODATAR[4] : (AES Offset: 0x50) Output Data Register -------- */
#define AES_ODATAR_ODATA_Pos 0
#define AES_ODATAR_ODATA_Msk (0xffffffffu << AES_ODATAR_ODATA_Pos) /**< \brief (AES_ODATAR[4]) Output Data */
/* -------- AES_IVR[4] : (AES Offset: 0x60) Initialization Vector Register -------- */
#define AES_IVR_IV_Pos 0
#define AES_IVR_IV_Msk (0xffffffffu << AES_IVR_IV_Pos) /**< \brief (AES_IVR[4]) Initialization Vector */
#define AES_IVR_IV(value) ((AES_IVR_IV_Msk & ((value) << AES_IVR_IV_Pos)))
/* -------- AES_AADLENR : (AES Offset: 0x70) Additional Authenticated Data Length Register -------- */
#define AES_AADLENR_AADLEN_Pos 0
#define AES_AADLENR_AADLEN_Msk (0xffffffffu << AES_AADLENR_AADLEN_Pos) /**< \brief (AES_AADLENR) AAD Length */
#define AES_AADLENR_AADLEN(value) ((AES_AADLENR_AADLEN_Msk & ((value) << AES_AADLENR_AADLEN_Pos)))
/* -------- AES_CLENR : (AES Offset: 0x74) Plaintext/Ciphertext Length Register -------- */
#define AES_CLENR_CLEN_Pos 0
#define AES_CLENR_CLEN_Msk (0xffffffffu << AES_CLENR_CLEN_Pos) /**< \brief (AES_CLENR) Plaintext/Ciphertext Length */
#define AES_CLENR_CLEN(value) ((AES_CLENR_CLEN_Msk & ((value) << AES_CLENR_CLEN_Pos)))
/* -------- AES_GHASHR[4] : (AES Offset: 0x78) GCM Intermediate Hash Word Register -------- */
#define AES_GHASHR_GHASH_Pos 0
#define AES_GHASHR_GHASH_Msk (0xffffffffu << AES_GHASHR_GHASH_Pos) /**< \brief (AES_GHASHR[4]) Intermediate GCM Hash Word x */
#define AES_GHASHR_GHASH(value) ((AES_GHASHR_GHASH_Msk & ((value) << AES_GHASHR_GHASH_Pos)))
/* -------- AES_TAGR[4] : (AES Offset: 0x88) GCM Authentication Tag Word Register -------- */
#define AES_TAGR_TAG_Pos 0
#define AES_TAGR_TAG_Msk (0xffffffffu << AES_TAGR_TAG_Pos) /**< \brief (AES_TAGR[4]) GCM Authentication Tag x */
/* -------- AES_CTRR : (AES Offset: 0x98) GCM Encryption Counter Value Register -------- */
#define AES_CTRR_CTR_Pos 0
#define AES_CTRR_CTR_Msk (0xffffffffu << AES_CTRR_CTR_Pos) /**< \brief (AES_CTRR) GCM Encryption Counter */
/* -------- AES_GCMHR[4] : (AES Offset: 0x9C) GCM H World Register -------- */
#define AES_GCMHR_H_Pos 0
#define AES_GCMHR_H_Msk (0xffffffffu << AES_GCMHR_H_Pos) /**< \brief (AES_GCMHR[4]) GCM H word x */
#define AES_GCMHR_H(value) ((AES_GCMHR_H_Msk & ((value) << AES_GCMHR_H_Pos)))
/* -------- AES_RPR : (AES Offset: 0x100) Receive Pointer Register -------- */
#define AES_RPR_RXPTR_Pos 0
#define AES_RPR_RXPTR_Msk (0xffffffffu << AES_RPR_RXPTR_Pos) /**< \brief (AES_RPR) Receive Pointer Register */
#define AES_RPR_RXPTR(value) ((AES_RPR_RXPTR_Msk & ((value) << AES_RPR_RXPTR_Pos)))
/* -------- AES_RCR : (AES Offset: 0x104) Receive Counter Register -------- */
#define AES_RCR_RXCTR_Pos 0
#define AES_RCR_RXCTR_Msk (0xffffu << AES_RCR_RXCTR_Pos) /**< \brief (AES_RCR) Receive Counter Register */
#define AES_RCR_RXCTR(value) ((AES_RCR_RXCTR_Msk & ((value) << AES_RCR_RXCTR_Pos)))
/* -------- AES_TPR : (AES Offset: 0x108) Transmit Pointer Register -------- */
#define AES_TPR_TXPTR_Pos 0
#define AES_TPR_TXPTR_Msk (0xffffffffu << AES_TPR_TXPTR_Pos) /**< \brief (AES_TPR) Transmit Counter Register */
#define AES_TPR_TXPTR(value) ((AES_TPR_TXPTR_Msk & ((value) << AES_TPR_TXPTR_Pos)))
/* -------- AES_TCR : (AES Offset: 0x10C) Transmit Counter Register -------- */
#define AES_TCR_TXCTR_Pos 0
#define AES_TCR_TXCTR_Msk (0xffffu << AES_TCR_TXCTR_Pos) /**< \brief (AES_TCR) Transmit Counter Register */
#define AES_TCR_TXCTR(value) ((AES_TCR_TXCTR_Msk & ((value) << AES_TCR_TXCTR_Pos)))
/* -------- AES_RNPR : (AES Offset: 0x110) Receive Next Pointer Register -------- */
#define AES_RNPR_RXNPTR_Pos 0
#define AES_RNPR_RXNPTR_Msk (0xffffffffu << AES_RNPR_RXNPTR_Pos) /**< \brief (AES_RNPR) Receive Next Pointer */
#define AES_RNPR_RXNPTR(value) ((AES_RNPR_RXNPTR_Msk & ((value) << AES_RNPR_RXNPTR_Pos)))
/* -------- AES_RNCR : (AES Offset: 0x114) Receive Next Counter Register -------- */
#define AES_RNCR_RXNCTR_Pos 0
#define AES_RNCR_RXNCTR_Msk (0xffffu << AES_RNCR_RXNCTR_Pos) /**< \brief (AES_RNCR) Receive Next Counter */
#define AES_RNCR_RXNCTR(value) ((AES_RNCR_RXNCTR_Msk & ((value) << AES_RNCR_RXNCTR_Pos)))
/* -------- AES_TNPR : (AES Offset: 0x118) Transmit Next Pointer Register -------- */
#define AES_TNPR_TXNPTR_Pos 0
#define AES_TNPR_TXNPTR_Msk (0xffffffffu << AES_TNPR_TXNPTR_Pos) /**< \brief (AES_TNPR) Transmit Next Pointer */
#define AES_TNPR_TXNPTR(value) ((AES_TNPR_TXNPTR_Msk & ((value) << AES_TNPR_TXNPTR_Pos)))
/* -------- AES_TNCR : (AES Offset: 0x11C) Transmit Next Counter Register -------- */
#define AES_TNCR_TXNCTR_Pos 0
#define AES_TNCR_TXNCTR_Msk (0xffffu << AES_TNCR_TXNCTR_Pos) /**< \brief (AES_TNCR) Transmit Counter Next */
#define AES_TNCR_TXNCTR(value) ((AES_TNCR_TXNCTR_Msk & ((value) << AES_TNCR_TXNCTR_Pos)))
/* -------- AES_PTCR : (AES Offset: 0x120) Transfer Control Register -------- */
#define AES_PTCR_RXTEN (0x1u << 0) /**< \brief (AES_PTCR) Receiver Transfer Enable */
#define AES_PTCR_RXTDIS (0x1u << 1) /**< \brief (AES_PTCR) Receiver Transfer Disable */
#define AES_PTCR_TXTEN (0x1u << 8) /**< \brief (AES_PTCR) Transmitter Transfer Enable */
#define AES_PTCR_TXTDIS (0x1u << 9) /**< \brief (AES_PTCR) Transmitter Transfer Disable */
/* -------- AES_PTSR : (AES Offset: 0x124) Transfer Status Register -------- */
#define AES_PTSR_RXTEN (0x1u << 0) /**< \brief (AES_PTSR) Receiver Transfer Enable */
#define AES_PTSR_TXTEN (0x1u << 8) /**< \brief (AES_PTSR) Transmitter Transfer Enable */

/*@}*/


#endif /* _SAM4C_AES_COMPONENT_ */
