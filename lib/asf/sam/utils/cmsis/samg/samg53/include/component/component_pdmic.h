/**
 * \file
 *
 * Copyright (c) 2013-2018 Microchip Technology Inc. and its subsidiaries.
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

#ifndef _SAMG53_PDMIC_COMPONENT_
#define _SAMG53_PDMIC_COMPONENT_

/* ============================================================================= */
/**  SOFTWARE API DEFINITION FOR Pulse Density Modulation Interface Controller */
/* ============================================================================= */
/** \addtogroup SAMG53_PDMIC Pulse Density Modulation Interface Controller */
/*@{*/

#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
/** \brief Pdmic hardware registers */
typedef struct {
  __IO uint32_t PDMIC_CR;      /**< \brief (Pdmic Offset: 0x00) Control Register */
  __IO uint32_t PDMIC_MR;      /**< \brief (Pdmic Offset: 0x04) Mode Register */
  __I  uint32_t Reserved1[3];
  __I  uint32_t PDMIC_CDR;     /**< \brief (Pdmic Offset: 0x14) Converted Data Register */
  __O  uint32_t PDMIC_IER;     /**< \brief (Pdmic Offset: 0x18) Interrupt Enable Register */
  __O  uint32_t PDMIC_IDR;     /**< \brief (Pdmic Offset: 0x1C) Interrupt Disable Register */
  __I  uint32_t PDMIC_IMR;     /**< \brief (Pdmic Offset: 0x20) Interrupt Mask Register */
  __I  uint32_t PDMIC_ISR;     /**< \brief (Pdmic Offset: 0x24) Interrupt Status Register */
  __I  uint32_t Reserved2[12];
  __IO uint32_t PDMIC_DSPR0;   /**< \brief (Pdmic Offset: 0x58) DSP Configuration Register 0 */
  __IO uint32_t PDMIC_DSPR1;   /**< \brief (Pdmic Offset: 0x5C) DSP Configuration Register 1 */
  __I  uint32_t Reserved3[33];
  __IO uint32_t PDMIC_WPMR;    /**< \brief (Pdmic Offset: 0xE4) Write Protection Mode Register */
  __I  uint32_t PDMIC_WPSR;    /**< \brief (Pdmic Offset: 0xE8) Write Protection Status Register */
  __I  uint32_t Reserved4[5];
  __IO uint32_t PDMIC_RPR;     /**< \brief (Pdmic Offset: 0x100) Receive Pointer Register */
  __IO uint32_t PDMIC_RCR;     /**< \brief (Pdmic Offset: 0x104) Receive Counter Register */
  __I  uint32_t Reserved5[2];
  __IO uint32_t PDMIC_RNPR;    /**< \brief (Pdmic Offset: 0x110) Receive Next Pointer Register */
  __IO uint32_t PDMIC_RNCR;    /**< \brief (Pdmic Offset: 0x114) Receive Next Counter Register */
  __I  uint32_t Reserved6[2];
  __O  uint32_t PDMIC_PTCR;    /**< \brief (Pdmic Offset: 0x120) Transfer Control Register */
  __I  uint32_t PDMIC_PTSR;    /**< \brief (Pdmic Offset: 0x124) Transfer Status Register */
} Pdmic;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */
/* -------- PDMIC_CR : (PDMIC Offset: 0x00) Control Register -------- */
#define PDMIC_CR_SWRST (0x1u << 0) /**< \brief (PDMIC_CR) Software Reset */
#define PDMIC_CR_ENPDM (0x1u << 4) /**< \brief (PDMIC_CR) Enable PDM */
/* -------- PDMIC_MR : (PDMIC Offset: 0x04) Mode Register -------- */
#define PDMIC_MR_PRESCAL_Pos 8
#define PDMIC_MR_PRESCAL_Msk (0x7fu << PDMIC_MR_PRESCAL_Pos) /**< \brief (PDMIC_MR) Prescaler Rate Selection */
#define PDMIC_MR_PRESCAL(value) ((PDMIC_MR_PRESCAL_Msk & ((value) << PDMIC_MR_PRESCAL_Pos)))
/* -------- PDMIC_CDR : (PDMIC Offset: 0x14) Converted Data Register -------- */
#define PDMIC_CDR_DATA_Pos 0
#define PDMIC_CDR_DATA_Msk (0xffffffffu << PDMIC_CDR_DATA_Pos) /**< \brief (PDMIC_CDR) Data Converted */
/* -------- PDMIC_IER : (PDMIC Offset: 0x18) Interrupt Enable Register -------- */
#define PDMIC_IER_DRDY (0x1u << 24) /**< \brief (PDMIC_IER) Data Ready Interrupt Enable */
#define PDMIC_IER_OVRE (0x1u << 25) /**< \brief (PDMIC_IER) Overrun Error Interrupt Enable */
#define PDMIC_IER_ENDRX (0x1u << 27) /**< \brief (PDMIC_IER) End of Receive Buffer Interrupt Enable */
#define PDMIC_IER_RXBUFF (0x1u << 28) /**< \brief (PDMIC_IER) Receive Buffer Full Interrupt Enable */
/* -------- PDMIC_IDR : (PDMIC Offset: 0x1C) Interrupt Disable Register -------- */
#define PDMIC_IDR_DRDY (0x1u << 24) /**< \brief (PDMIC_IDR) Data Ready Interrupt Disable */
#define PDMIC_IDR_OVRE (0x1u << 25) /**< \brief (PDMIC_IDR) General Overrun Error Interrupt Disable */
#define PDMIC_IDR_ENDRX (0x1u << 27) /**< \brief (PDMIC_IDR) End of Receive Buffer Interrupt Disable */
#define PDMIC_IDR_RXBUFF (0x1u << 28) /**< \brief (PDMIC_IDR) Receive Buffer Full Interrupt Disable */
/* -------- PDMIC_IMR : (PDMIC Offset: 0x20) Interrupt Mask Register -------- */
#define PDMIC_IMR_DRDY (0x1u << 24) /**< \brief (PDMIC_IMR) Data Ready Interrupt Mask */
#define PDMIC_IMR_OVRE (0x1u << 25) /**< \brief (PDMIC_IMR) General Overrun Error Interrupt Mask */
#define PDMIC_IMR_ENDRX (0x1u << 27) /**< \brief (PDMIC_IMR) End of Receive Buffer Interrupt Mask */
#define PDMIC_IMR_RXBUFF (0x1u << 28) /**< \brief (PDMIC_IMR) Receive Buffer Full Interrupt Mask */
/* -------- PDMIC_ISR : (PDMIC Offset: 0x24) Interrupt Status Register -------- */
#define PDMIC_ISR_FIFOCNT_Pos 16
#define PDMIC_ISR_FIFOCNT_Msk (0xffu << PDMIC_ISR_FIFOCNT_Pos) /**< \brief (PDMIC_ISR) FIFO Count */
#define PDMIC_ISR_DRDY (0x1u << 24) /**< \brief (PDMIC_ISR) Data Ready */
#define PDMIC_ISR_OVRE (0x1u << 25) /**< \brief (PDMIC_ISR) Overrun Error */
#define PDMIC_ISR_ENDRX (0x1u << 27) /**< \brief (PDMIC_ISR) End of RX Buffer */
#define PDMIC_ISR_RXBUFF (0x1u << 28) /**< \brief (PDMIC_ISR) RX Buffer Full */
/* -------- PDMIC_DSPR0 : (PDMIC Offset: 0x58) DSP Configuration Register 0 -------- */
#define PDMIC_DSPR0_HPFBYP (0x1u << 1) /**< \brief (PDMIC_DSPR0) High-Pass Filter Bypass */
#define PDMIC_DSPR0_SINBYP (0x1u << 2) /**< \brief (PDMIC_DSPR0) SINCC Filter Bypass */
#define PDMIC_DSPR0_SIZE (0x1u << 3) /**< \brief (PDMIC_DSPR0) Data Size */
#define PDMIC_DSPR0_OSR_Pos 4
#define PDMIC_DSPR0_OSR_Msk (0x7u << PDMIC_DSPR0_OSR_Pos) /**< \brief (PDMIC_DSPR0) Oversampling Ratio */
#define   PDMIC_DSPR0_OSR_128 (0x0u << 4) /**< \brief (PDMIC_DSPR0) Oversampling ratio is 128 */
#define   PDMIC_DSPR0_OSR_64 (0x1u << 4) /**< \brief (PDMIC_DSPR0) Oversampling ratio is 64 */
#define PDMIC_DSPR0_SCALE_Pos 8
#define PDMIC_DSPR0_SCALE_Msk (0xfu << PDMIC_DSPR0_SCALE_Pos) /**< \brief (PDMIC_DSPR0) Data Scale */
#define PDMIC_DSPR0_SCALE(value) ((PDMIC_DSPR0_SCALE_Msk & ((value) << PDMIC_DSPR0_SCALE_Pos)))
#define PDMIC_DSPR0_SHIFT_Pos 12
#define PDMIC_DSPR0_SHIFT_Msk (0xfu << PDMIC_DSPR0_SHIFT_Pos) /**< \brief (PDMIC_DSPR0) Data Shift */
#define PDMIC_DSPR0_SHIFT(value) ((PDMIC_DSPR0_SHIFT_Msk & ((value) << PDMIC_DSPR0_SHIFT_Pos)))
/* -------- PDMIC_DSPR1 : (PDMIC Offset: 0x5C) DSP Configuration Register 1 -------- */
#define PDMIC_DSPR1_DGAIN_Pos 0
#define PDMIC_DSPR1_DGAIN_Msk (0x7fffu << PDMIC_DSPR1_DGAIN_Pos) /**< \brief (PDMIC_DSPR1) Gain Correction */
#define PDMIC_DSPR1_DGAIN(value) ((PDMIC_DSPR1_DGAIN_Msk & ((value) << PDMIC_DSPR1_DGAIN_Pos)))
#define PDMIC_DSPR1_OFFSET_Pos 16
#define PDMIC_DSPR1_OFFSET_Msk (0xffffu << PDMIC_DSPR1_OFFSET_Pos) /**< \brief (PDMIC_DSPR1) Offset Correction */
#define PDMIC_DSPR1_OFFSET(value) ((PDMIC_DSPR1_OFFSET_Msk & ((value) << PDMIC_DSPR1_OFFSET_Pos)))
/* -------- PDMIC_WPMR : (PDMIC Offset: 0xE4) Write Protection Mode Register -------- */
#define PDMIC_WPMR_WPEN (0x1u << 0) /**< \brief (PDMIC_WPMR) Write Protection Enable */
#define PDMIC_WPMR_WPKEY_Pos 8
#define PDMIC_WPMR_WPKEY_Msk (0xffffffu << PDMIC_WPMR_WPKEY_Pos) /**< \brief (PDMIC_WPMR) Write Protect KEY */
#define   PDMIC_WPMR_WPKEY_PASSWD (0x414443u << 8) /**< \brief (PDMIC_WPMR) Writing any other value in this field aborts the write operation of the WPEN bit.Always reads as 0. */
/* -------- PDMIC_WPSR : (PDMIC Offset: 0xE8) Write Protection Status Register -------- */
#define PDMIC_WPSR_WPVS (0x1u << 0) /**< \brief (PDMIC_WPSR) Write Protection Violation Status */
#define PDMIC_WPSR_WPVSRC_Pos 8
#define PDMIC_WPSR_WPVSRC_Msk (0xffffu << PDMIC_WPSR_WPVSRC_Pos) /**< \brief (PDMIC_WPSR) Write Protection Violation Source */
/* -------- PDMIC_RPR : (PDMIC Offset: 0x100) Receive Pointer Register -------- */
#define PDMIC_RPR_RXPTR_Pos 0
#define PDMIC_RPR_RXPTR_Msk (0xffffffffu << PDMIC_RPR_RXPTR_Pos) /**< \brief (PDMIC_RPR) Receive Pointer Register */
#define PDMIC_RPR_RXPTR(value) ((PDMIC_RPR_RXPTR_Msk & ((value) << PDMIC_RPR_RXPTR_Pos)))
/* -------- PDMIC_RCR : (PDMIC Offset: 0x104) Receive Counter Register -------- */
#define PDMIC_RCR_RXCTR_Pos 0
#define PDMIC_RCR_RXCTR_Msk (0xffffu << PDMIC_RCR_RXCTR_Pos) /**< \brief (PDMIC_RCR) Receive Counter Register */
#define PDMIC_RCR_RXCTR(value) ((PDMIC_RCR_RXCTR_Msk & ((value) << PDMIC_RCR_RXCTR_Pos)))
/* -------- PDMIC_RNPR : (PDMIC Offset: 0x110) Receive Next Pointer Register -------- */
#define PDMIC_RNPR_RXNPTR_Pos 0
#define PDMIC_RNPR_RXNPTR_Msk (0xffffffffu << PDMIC_RNPR_RXNPTR_Pos) /**< \brief (PDMIC_RNPR) Receive Next Pointer */
#define PDMIC_RNPR_RXNPTR(value) ((PDMIC_RNPR_RXNPTR_Msk & ((value) << PDMIC_RNPR_RXNPTR_Pos)))
/* -------- PDMIC_RNCR : (PDMIC Offset: 0x114) Receive Next Counter Register -------- */
#define PDMIC_RNCR_RXNCTR_Pos 0
#define PDMIC_RNCR_RXNCTR_Msk (0xffffu << PDMIC_RNCR_RXNCTR_Pos) /**< \brief (PDMIC_RNCR) Receive Next Counter */
#define PDMIC_RNCR_RXNCTR(value) ((PDMIC_RNCR_RXNCTR_Msk & ((value) << PDMIC_RNCR_RXNCTR_Pos)))
/* -------- PDMIC_PTCR : (PDMIC Offset: 0x120) Transfer Control Register -------- */
#define PDMIC_PTCR_RXTEN (0x1u << 0) /**< \brief (PDMIC_PTCR) Receiver Transfer Enable */
#define PDMIC_PTCR_RXTDIS (0x1u << 1) /**< \brief (PDMIC_PTCR) Receiver Transfer Disable */
#define PDMIC_PTCR_TXTEN (0x1u << 8) /**< \brief (PDMIC_PTCR) Transmitter Transfer Enable */
#define PDMIC_PTCR_TXTDIS (0x1u << 9) /**< \brief (PDMIC_PTCR) Transmitter Transfer Disable */
#define PDMIC_PTCR_RXCBEN (0x1u << 16) /**< \brief (PDMIC_PTCR) Receiver Circular Buffer Enable */
#define PDMIC_PTCR_RXCBDIS (0x1u << 17) /**< \brief (PDMIC_PTCR) Receiver Circular Buffer Disable */
#define PDMIC_PTCR_TXCBEN (0x1u << 18) /**< \brief (PDMIC_PTCR) Transmitter Circular Buffer Enable */
#define PDMIC_PTCR_TXCBDIS (0x1u << 19) /**< \brief (PDMIC_PTCR) Transmitter Circular Buffer Disable */
#define PDMIC_PTCR_ERRCLR (0x1u << 24) /**< \brief (PDMIC_PTCR) Transfer Bus Error Clear */
/* -------- PDMIC_PTSR : (PDMIC Offset: 0x124) Transfer Status Register -------- */
#define PDMIC_PTSR_RXTEN (0x1u << 0) /**< \brief (PDMIC_PTSR) Receiver Transfer Enable */
#define PDMIC_PTSR_TXTEN (0x1u << 8) /**< \brief (PDMIC_PTSR) Transmitter Transfer Enable */
#define PDMIC_PTSR_RXCBEN (0x1u << 16) /**< \brief (PDMIC_PTSR) Receiver Transfer Enable */
#define PDMIC_PTSR_TXCBEN (0x1u << 18) /**< \brief (PDMIC_PTSR) Transmitter Transfer Enable */
#define PDMIC_PTSR_ERR (0x1u << 24) /**< \brief (PDMIC_PTSR) Transfer Bus Error */

/*@}*/


#endif /* _SAMG53_PDMIC_COMPONENT_ */
