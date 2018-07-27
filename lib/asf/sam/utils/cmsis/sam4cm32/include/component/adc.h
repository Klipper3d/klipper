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

#ifndef _SAM4CM_ADC_COMPONENT_
#define _SAM4CM_ADC_COMPONENT_

/* ============================================================================= */
/**  SOFTWARE API DEFINITION FOR Analog-to-Digital Converter */
/* ============================================================================= */
/** \addtogroup SAM4CM_ADC Analog-to-Digital Converter */
/*@{*/

#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
/** \brief Adc hardware registers */
typedef struct {
  __O  uint32_t ADC_CR;        /**< \brief (Adc Offset: 0x00) Control Register */
  __IO uint32_t ADC_MR;        /**< \brief (Adc Offset: 0x04) Mode Register */
  __IO uint32_t ADC_SEQR1;     /**< \brief (Adc Offset: 0x08) Channel Sequence 1 Register */
  __I  uint32_t Reserved1[1];
  __O  uint32_t ADC_CHER;      /**< \brief (Adc Offset: 0x10) Channel Enable Register */
  __O  uint32_t ADC_CHDR;      /**< \brief (Adc Offset: 0x14) Channel Disable Register */
  __I  uint32_t ADC_CHSR;      /**< \brief (Adc Offset: 0x18) Channel Status Register */
  __I  uint32_t Reserved2[1];
  __I  uint32_t ADC_LCDR;      /**< \brief (Adc Offset: 0x20) Last Converted Data Register */
  __O  uint32_t ADC_IER;       /**< \brief (Adc Offset: 0x24) Interrupt Enable Register */
  __O  uint32_t ADC_IDR;       /**< \brief (Adc Offset: 0x28) Interrupt Disable Register */
  __I  uint32_t ADC_IMR;       /**< \brief (Adc Offset: 0x2C) Interrupt Mask Register */
  __I  uint32_t ADC_ISR;       /**< \brief (Adc Offset: 0x30) Interrupt Status Register */
  __IO uint32_t ADC_TEMPMR;    /**< \brief (Adc Offset: 0x34) Temperature Sensor Mode Register */
  __IO uint32_t ADC_TEMPCWR;   /**< \brief (Adc Offset: 0x38) Temperature Compare Window Register */
  __I  uint32_t ADC_OVER;      /**< \brief (Adc Offset: 0x3C) Overrun Status Register */
  __IO uint32_t ADC_EMR;       /**< \brief (Adc Offset: 0x40) Extended Mode Register */
  __IO uint32_t ADC_CWR;       /**< \brief (Adc Offset: 0x44) Compare Window Register */
  __I  uint32_t Reserved3[2];
  __I  uint32_t ADC_CDR[8];    /**< \brief (Adc Offset: 0x50) Channel Data Register */
  __I  uint32_t Reserved4[9];
  __IO uint32_t ADC_ACR;       /**< \brief (Adc Offset: 0x94) Analog Control Register */
  __I  uint32_t Reserved5[19];
  __IO uint32_t ADC_WPMR;      /**< \brief (Adc Offset: 0xE4) Write Protection Mode Register */
  __I  uint32_t ADC_WPSR;      /**< \brief (Adc Offset: 0xE8) Write Protection Status Register */
  __I  uint32_t Reserved6[5];
  __IO uint32_t ADC_RPR;       /**< \brief (Adc Offset: 0x100) Receive Pointer Register */
  __IO uint32_t ADC_RCR;       /**< \brief (Adc Offset: 0x104) Receive Counter Register */
  __I  uint32_t Reserved7[2];
  __IO uint32_t ADC_RNPR;      /**< \brief (Adc Offset: 0x110) Receive Next Pointer Register */
  __IO uint32_t ADC_RNCR;      /**< \brief (Adc Offset: 0x114) Receive Next Counter Register */
  __I  uint32_t Reserved8[2];
  __O  uint32_t ADC_PTCR;      /**< \brief (Adc Offset: 0x120) Transfer Control Register */
  __I  uint32_t ADC_PTSR;      /**< \brief (Adc Offset: 0x124) Transfer Status Register */
} Adc;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */
/* -------- ADC_CR : (ADC Offset: 0x00) Control Register -------- */
#define ADC_CR_SWRST (0x1u << 0) /**< \brief (ADC_CR) Software Reset */
#define ADC_CR_START (0x1u << 1) /**< \brief (ADC_CR) Start Conversion */
/* -------- ADC_MR : (ADC Offset: 0x04) Mode Register -------- */
#define ADC_MR_TRGEN (0x1u << 0) /**< \brief (ADC_MR) Trigger Enable */
#define   ADC_MR_TRGEN_DIS (0x0u << 0) /**< \brief (ADC_MR) Hardware triggers are disabled. Starting a conversion is only possible by software. */
#define   ADC_MR_TRGEN_EN (0x1u << 0) /**< \brief (ADC_MR) Hardware trigger selected by TRGSEL field is enabled. */
#define ADC_MR_TRGSEL_Pos 1
#define ADC_MR_TRGSEL_Msk (0x7u << ADC_MR_TRGSEL_Pos) /**< \brief (ADC_MR) Trigger Selection */
#define   ADC_MR_TRGSEL_ADC_TRIG0 (0x0u << 1) /**< \brief (ADC_MR) - */
#define   ADC_MR_TRGSEL_ADC_TRIG1 (0x1u << 1) /**< \brief (ADC_MR) Timer Counter Channel 0 Output */
#define   ADC_MR_TRGSEL_ADC_TRIG2 (0x2u << 1) /**< \brief (ADC_MR) Timer Counter Channel  1 Output */
#define   ADC_MR_TRGSEL_ADC_TRIG3 (0x3u << 1) /**< \brief (ADC_MR) Timer Counter Channel  2 Output */
#define   ADC_MR_TRGSEL_ADC_TRIG4 (0x4u << 1) /**< \brief (ADC_MR) Timer Counter Channel  3 Output */
#define   ADC_MR_TRGSEL_ADC_TRIG5 (0x5u << 1) /**< \brief (ADC_MR) Timer Counter Channel  4 Output */
#define   ADC_MR_TRGSEL_ADC_TRIG6 (0x6u << 1) /**< \brief (ADC_MR) Timer Counter Channel  5 Output */
#define ADC_MR_LOWRES (0x1u << 4) /**< \brief (ADC_MR) Resolution */
#define   ADC_MR_LOWRES_BITS_10 (0x0u << 4) /**< \brief (ADC_MR) 10-bit resolution. For higher resolution by averaging, please refer to Section 8.15 "ADC Extended Mode Register" */
#define   ADC_MR_LOWRES_BITS_8 (0x1u << 4) /**< \brief (ADC_MR) 8-bit resolution */
#define ADC_MR_SLEEP (0x1u << 5) /**< \brief (ADC_MR) Sleep Mode */
#define   ADC_MR_SLEEP_NORMAL (0x0u << 5) /**< \brief (ADC_MR) Normal Mode: The ADC core and reference voltage circuitry are kept ON between conversions */
#define   ADC_MR_SLEEP_SLEEP (0x1u << 5) /**< \brief (ADC_MR) Sleep Mode: The ADC core and reference voltage circuitry are OFF between conversions */
#define ADC_MR_FREERUN (0x1u << 7) /**< \brief (ADC_MR) Free Run Mode */
#define   ADC_MR_FREERUN_OFF (0x0u << 7) /**< \brief (ADC_MR) Normal Mode */
#define   ADC_MR_FREERUN_ON (0x1u << 7) /**< \brief (ADC_MR) Free Run Mode: Never wait for any trigger. */
#define ADC_MR_PRESCAL_Pos 8
#define ADC_MR_PRESCAL_Msk (0xffu << ADC_MR_PRESCAL_Pos) /**< \brief (ADC_MR) Prescaler Rate Selection */
#define ADC_MR_PRESCAL(value) ((ADC_MR_PRESCAL_Msk & ((value) << ADC_MR_PRESCAL_Pos)))
#define ADC_MR_STARTUP_Pos 16
#define ADC_MR_STARTUP_Msk (0xfu << ADC_MR_STARTUP_Pos) /**< \brief (ADC_MR) Start Up Time */
#define   ADC_MR_STARTUP_SUT0 (0x0u << 16) /**< \brief (ADC_MR) 0 periods of ADCClock */
#define   ADC_MR_STARTUP_SUT8 (0x1u << 16) /**< \brief (ADC_MR) 8 periods of ADCClock */
#define   ADC_MR_STARTUP_SUT16 (0x2u << 16) /**< \brief (ADC_MR) 16 periods of ADCClock */
#define   ADC_MR_STARTUP_SUT24 (0x3u << 16) /**< \brief (ADC_MR) 24 periods of ADCClock */
#define   ADC_MR_STARTUP_SUT64 (0x4u << 16) /**< \brief (ADC_MR) 64 periods of ADCClock */
#define   ADC_MR_STARTUP_SUT80 (0x5u << 16) /**< \brief (ADC_MR) 80 periods of ADCClock */
#define   ADC_MR_STARTUP_SUT96 (0x6u << 16) /**< \brief (ADC_MR) 96 periods of ADCClock */
#define   ADC_MR_STARTUP_SUT112 (0x7u << 16) /**< \brief (ADC_MR) 112 periods of ADCClock */
#define   ADC_MR_STARTUP_SUT512 (0x8u << 16) /**< \brief (ADC_MR) 512 periods of ADCClock */
#define   ADC_MR_STARTUP_SUT576 (0x9u << 16) /**< \brief (ADC_MR) 576 periods of ADCClock */
#define   ADC_MR_STARTUP_SUT640 (0xAu << 16) /**< \brief (ADC_MR) 640 periods of ADCClock */
#define   ADC_MR_STARTUP_SUT704 (0xBu << 16) /**< \brief (ADC_MR) 704 periods of ADCClock */
#define   ADC_MR_STARTUP_SUT768 (0xCu << 16) /**< \brief (ADC_MR) 768 periods of ADCClock */
#define   ADC_MR_STARTUP_SUT832 (0xDu << 16) /**< \brief (ADC_MR) 832 periods of ADCClock */
#define   ADC_MR_STARTUP_SUT896 (0xEu << 16) /**< \brief (ADC_MR) 896 periods of ADCClock */
#define   ADC_MR_STARTUP_SUT960 (0xFu << 16) /**< \brief (ADC_MR) 960 periods of ADCClock */
#define ADC_MR_TRACKTIM_Pos 24
#define ADC_MR_TRACKTIM_Msk (0xfu << ADC_MR_TRACKTIM_Pos) /**< \brief (ADC_MR) Tracking Time */
#define ADC_MR_TRACKTIM(value) ((ADC_MR_TRACKTIM_Msk & ((value) << ADC_MR_TRACKTIM_Pos)))
#define ADC_MR_USEQ (0x1u << 31) /**< \brief (ADC_MR) User Sequence Enable */
#define   ADC_MR_USEQ_NUM_ORDER (0x0u << 31) /**< \brief (ADC_MR) Normal Mode: The controller converts channels in a simple numeric order depending only on the channel index. */
#define   ADC_MR_USEQ_REG_ORDER (0x1u << 31) /**< \brief (ADC_MR) User Sequence Mode: The sequence respects what is defined in ADC_SEQR1 and can be used to convert the same channel several times. */
/* -------- ADC_SEQR1 : (ADC Offset: 0x08) Channel Sequence 1 Register -------- */
#define ADC_SEQR1_USCH1_Pos 0
#define ADC_SEQR1_USCH1_Msk (0xfu << ADC_SEQR1_USCH1_Pos) /**< \brief (ADC_SEQR1) User Sequence Number 1 */
#define ADC_SEQR1_USCH1(value) ((ADC_SEQR1_USCH1_Msk & ((value) << ADC_SEQR1_USCH1_Pos)))
#define ADC_SEQR1_USCH2_Pos 4
#define ADC_SEQR1_USCH2_Msk (0xfu << ADC_SEQR1_USCH2_Pos) /**< \brief (ADC_SEQR1) User Sequence Number 2 */
#define ADC_SEQR1_USCH2(value) ((ADC_SEQR1_USCH2_Msk & ((value) << ADC_SEQR1_USCH2_Pos)))
#define ADC_SEQR1_USCH3_Pos 8
#define ADC_SEQR1_USCH3_Msk (0xfu << ADC_SEQR1_USCH3_Pos) /**< \brief (ADC_SEQR1) User Sequence Number 3 */
#define ADC_SEQR1_USCH3(value) ((ADC_SEQR1_USCH3_Msk & ((value) << ADC_SEQR1_USCH3_Pos)))
#define ADC_SEQR1_USCH4_Pos 12
#define ADC_SEQR1_USCH4_Msk (0xfu << ADC_SEQR1_USCH4_Pos) /**< \brief (ADC_SEQR1) User Sequence Number 4 */
#define ADC_SEQR1_USCH4(value) ((ADC_SEQR1_USCH4_Msk & ((value) << ADC_SEQR1_USCH4_Pos)))
#define ADC_SEQR1_USCH5_Pos 16
#define ADC_SEQR1_USCH5_Msk (0xfu << ADC_SEQR1_USCH5_Pos) /**< \brief (ADC_SEQR1) User Sequence Number 5 */
#define ADC_SEQR1_USCH5(value) ((ADC_SEQR1_USCH5_Msk & ((value) << ADC_SEQR1_USCH5_Pos)))
#define ADC_SEQR1_USCH6_Pos 20
#define ADC_SEQR1_USCH6_Msk (0xfu << ADC_SEQR1_USCH6_Pos) /**< \brief (ADC_SEQR1) User Sequence Number 6 */
#define ADC_SEQR1_USCH6(value) ((ADC_SEQR1_USCH6_Msk & ((value) << ADC_SEQR1_USCH6_Pos)))
/* -------- ADC_CHER : (ADC Offset: 0x10) Channel Enable Register -------- */
#define ADC_CHER_CH0 (0x1u << 0) /**< \brief (ADC_CHER) Channel 0 Enable */
#define ADC_CHER_CH1 (0x1u << 1) /**< \brief (ADC_CHER) Channel 1 Enable */
#define ADC_CHER_CH2 (0x1u << 2) /**< \brief (ADC_CHER) Channel 2 Enable */
#define ADC_CHER_CH3 (0x1u << 3) /**< \brief (ADC_CHER) Channel 3 Enable */
#define ADC_CHER_CH6 (0x1u << 6) /**< \brief (ADC_CHER) Channel 6 Enable */
#define ADC_CHER_CH7 (0x1u << 7) /**< \brief (ADC_CHER) Channel 7 Enable */
/* -------- ADC_CHDR : (ADC Offset: 0x14) Channel Disable Register -------- */
#define ADC_CHDR_CH0 (0x1u << 0) /**< \brief (ADC_CHDR) Channel 0 Disable */
#define ADC_CHDR_CH1 (0x1u << 1) /**< \brief (ADC_CHDR) Channel 1 Disable */
#define ADC_CHDR_CH2 (0x1u << 2) /**< \brief (ADC_CHDR) Channel 2 Disable */
#define ADC_CHDR_CH3 (0x1u << 3) /**< \brief (ADC_CHDR) Channel 3 Disable */
#define ADC_CHDR_CH6 (0x1u << 6) /**< \brief (ADC_CHDR) Channel 6 Disable */
#define ADC_CHDR_CH7 (0x1u << 7) /**< \brief (ADC_CHDR) Channel 7 Disable */
/* -------- ADC_CHSR : (ADC Offset: 0x18) Channel Status Register -------- */
#define ADC_CHSR_CH0 (0x1u << 0) /**< \brief (ADC_CHSR) Channel 0 Status */
#define ADC_CHSR_CH1 (0x1u << 1) /**< \brief (ADC_CHSR) Channel 1 Status */
#define ADC_CHSR_CH2 (0x1u << 2) /**< \brief (ADC_CHSR) Channel 2 Status */
#define ADC_CHSR_CH3 (0x1u << 3) /**< \brief (ADC_CHSR) Channel 3 Status */
#define ADC_CHSR_CH6 (0x1u << 6) /**< \brief (ADC_CHSR) Channel 6 Status */
#define ADC_CHSR_CH7 (0x1u << 7) /**< \brief (ADC_CHSR) Channel 7 Status */
/* -------- ADC_LCDR : (ADC Offset: 0x20) Last Converted Data Register -------- */
#define ADC_LCDR_LDATA_Pos 0
#define ADC_LCDR_LDATA_Msk (0xfffu << ADC_LCDR_LDATA_Pos) /**< \brief (ADC_LCDR) Last Data Converted */
#define ADC_LCDR_CHNB_Pos 12
#define ADC_LCDR_CHNB_Msk (0xfu << ADC_LCDR_CHNB_Pos) /**< \brief (ADC_LCDR) Channel Number */
/* -------- ADC_IER : (ADC Offset: 0x24) Interrupt Enable Register -------- */
#define ADC_IER_EOC0 (0x1u << 0) /**< \brief (ADC_IER) End of Conversion Interrupt Enable 0 */
#define ADC_IER_EOC1 (0x1u << 1) /**< \brief (ADC_IER) End of Conversion Interrupt Enable 1 */
#define ADC_IER_EOC2 (0x1u << 2) /**< \brief (ADC_IER) End of Conversion Interrupt Enable 2 */
#define ADC_IER_EOC3 (0x1u << 3) /**< \brief (ADC_IER) End of Conversion Interrupt Enable 3 */
#define ADC_IER_EOC6 (0x1u << 6) /**< \brief (ADC_IER) End of Conversion Interrupt Enable 6 */
#define ADC_IER_EOC7 (0x1u << 7) /**< \brief (ADC_IER) End of Conversion Interrupt Enable 7 */
#define ADC_IER_TEMPCHG (0x1u << 19) /**< \brief (ADC_IER) Temperature Change Interrupt Enable */
#define ADC_IER_DRDY (0x1u << 24) /**< \brief (ADC_IER) Data Ready Interrupt Enable */
#define ADC_IER_GOVRE (0x1u << 25) /**< \brief (ADC_IER) General Overrun Error Interrupt Enable */
#define ADC_IER_COMPE (0x1u << 26) /**< \brief (ADC_IER) Comparison Event Interrupt Enable */
#define ADC_IER_ENDRX (0x1u << 27) /**< \brief (ADC_IER) End of Receive Buffer Interrupt Enable */
#define ADC_IER_RXBUFF (0x1u << 28) /**< \brief (ADC_IER) Receive Buffer Full Interrupt Enable */
/* -------- ADC_IDR : (ADC Offset: 0x28) Interrupt Disable Register -------- */
#define ADC_IDR_EOC0 (0x1u << 0) /**< \brief (ADC_IDR) End of Conversion Interrupt Disable 0 */
#define ADC_IDR_EOC1 (0x1u << 1) /**< \brief (ADC_IDR) End of Conversion Interrupt Disable 1 */
#define ADC_IDR_EOC2 (0x1u << 2) /**< \brief (ADC_IDR) End of Conversion Interrupt Disable 2 */
#define ADC_IDR_EOC3 (0x1u << 3) /**< \brief (ADC_IDR) End of Conversion Interrupt Disable 3 */
#define ADC_IDR_EOC6 (0x1u << 6) /**< \brief (ADC_IDR) End of Conversion Interrupt Disable 6 */
#define ADC_IDR_EOC7 (0x1u << 7) /**< \brief (ADC_IDR) End of Conversion Interrupt Disable 7 */
#define ADC_IDR_TEMPCHG (0x1u << 19) /**< \brief (ADC_IDR) Temperature Change Interrupt Disable */
#define ADC_IDR_DRDY (0x1u << 24) /**< \brief (ADC_IDR) Data Ready Interrupt Disable */
#define ADC_IDR_GOVRE (0x1u << 25) /**< \brief (ADC_IDR) General Overrun Error Interrupt Disable */
#define ADC_IDR_COMPE (0x1u << 26) /**< \brief (ADC_IDR) Comparison Event Interrupt Disable */
#define ADC_IDR_ENDRX (0x1u << 27) /**< \brief (ADC_IDR) End of Receive Buffer Interrupt Disable */
#define ADC_IDR_RXBUFF (0x1u << 28) /**< \brief (ADC_IDR) Receive Buffer Full Interrupt Disable */
/* -------- ADC_IMR : (ADC Offset: 0x2C) Interrupt Mask Register -------- */
#define ADC_IMR_EOC0 (0x1u << 0) /**< \brief (ADC_IMR) End of Conversion Interrupt Mask 0 */
#define ADC_IMR_EOC1 (0x1u << 1) /**< \brief (ADC_IMR) End of Conversion Interrupt Mask 1 */
#define ADC_IMR_EOC2 (0x1u << 2) /**< \brief (ADC_IMR) End of Conversion Interrupt Mask 2 */
#define ADC_IMR_EOC3 (0x1u << 3) /**< \brief (ADC_IMR) End of Conversion Interrupt Mask 3 */
#define ADC_IMR_EOC6 (0x1u << 6) /**< \brief (ADC_IMR) End of Conversion Interrupt Mask 6 */
#define ADC_IMR_EOC7 (0x1u << 7) /**< \brief (ADC_IMR) End of Conversion Interrupt Mask 7 */
#define ADC_IMR_TEMPCHG (0x1u << 19) /**< \brief (ADC_IMR) Temperature Change Interrupt Mask */
#define ADC_IMR_DRDY (0x1u << 24) /**< \brief (ADC_IMR) Data Ready Interrupt Mask */
#define ADC_IMR_GOVRE (0x1u << 25) /**< \brief (ADC_IMR) General Overrun Error Interrupt Mask */
#define ADC_IMR_COMPE (0x1u << 26) /**< \brief (ADC_IMR) Comparison Event Interrupt Mask */
#define ADC_IMR_ENDRX (0x1u << 27) /**< \brief (ADC_IMR) End of Receive Buffer Interrupt Mask */
#define ADC_IMR_RXBUFF (0x1u << 28) /**< \brief (ADC_IMR) Receive Buffer Full Interrupt Mask */
/* -------- ADC_ISR : (ADC Offset: 0x30) Interrupt Status Register -------- */
#define ADC_ISR_EOC0 (0x1u << 0) /**< \brief (ADC_ISR) End of Conversion 0 */
#define ADC_ISR_EOC1 (0x1u << 1) /**< \brief (ADC_ISR) End of Conversion 1 */
#define ADC_ISR_EOC2 (0x1u << 2) /**< \brief (ADC_ISR) End of Conversion 2 */
#define ADC_ISR_EOC3 (0x1u << 3) /**< \brief (ADC_ISR) End of Conversion 3 */
#define ADC_ISR_EOC6 (0x1u << 6) /**< \brief (ADC_ISR) End of Conversion 6 */
#define ADC_ISR_EOC7 (0x1u << 7) /**< \brief (ADC_ISR) End of Conversion 7 */
#define ADC_ISR_TEMPCHG (0x1u << 19) /**< \brief (ADC_ISR) Temperature Change */
#define ADC_ISR_DRDY (0x1u << 24) /**< \brief (ADC_ISR) Data Ready */
#define ADC_ISR_GOVRE (0x1u << 25) /**< \brief (ADC_ISR) General Overrun Error */
#define ADC_ISR_COMPE (0x1u << 26) /**< \brief (ADC_ISR) Comparison Error */
#define ADC_ISR_ENDRX (0x1u << 27) /**< \brief (ADC_ISR) End of RX Buffer */
#define ADC_ISR_RXBUFF (0x1u << 28) /**< \brief (ADC_ISR) RX Buffer Full */
/* -------- ADC_TEMPMR : (ADC Offset: 0x34) Temperature Sensor Mode Register -------- */
#define ADC_TEMPMR_TEMPON (0x1u << 0) /**< \brief (ADC_TEMPMR) Temperature Sensor ON */
#define ADC_TEMPMR_TEMPCMPMOD_Pos 4
#define ADC_TEMPMR_TEMPCMPMOD_Msk (0x3u << ADC_TEMPMR_TEMPCMPMOD_Pos) /**< \brief (ADC_TEMPMR) Temperature Comparison Mode */
#define   ADC_TEMPMR_TEMPCMPMOD_LOW (0x0u << 4) /**< \brief (ADC_TEMPMR) Generates an event when the converted data is lower than the low threshold of the window. */
#define   ADC_TEMPMR_TEMPCMPMOD_HIGH (0x1u << 4) /**< \brief (ADC_TEMPMR) Generates an event when the converted data is higher than the high threshold of the window. */
#define   ADC_TEMPMR_TEMPCMPMOD_IN (0x2u << 4) /**< \brief (ADC_TEMPMR) Generates an event when the converted data is in the comparison window. */
#define   ADC_TEMPMR_TEMPCMPMOD_OUT (0x3u << 4) /**< \brief (ADC_TEMPMR) Generates an event when the converted data is out of the comparison window. */
/* -------- ADC_TEMPCWR : (ADC Offset: 0x38) Temperature Compare Window Register -------- */
#define ADC_TEMPCWR_TLOWTHRES_Pos 0
#define ADC_TEMPCWR_TLOWTHRES_Msk (0xfffu << ADC_TEMPCWR_TLOWTHRES_Pos) /**< \brief (ADC_TEMPCWR) Temperature Low Threshold */
#define ADC_TEMPCWR_TLOWTHRES(value) ((ADC_TEMPCWR_TLOWTHRES_Msk & ((value) << ADC_TEMPCWR_TLOWTHRES_Pos)))
#define ADC_TEMPCWR_THIGHTHRES_Pos 16
#define ADC_TEMPCWR_THIGHTHRES_Msk (0xfffu << ADC_TEMPCWR_THIGHTHRES_Pos) /**< \brief (ADC_TEMPCWR) Temperature High Threshold */
#define ADC_TEMPCWR_THIGHTHRES(value) ((ADC_TEMPCWR_THIGHTHRES_Msk & ((value) << ADC_TEMPCWR_THIGHTHRES_Pos)))
/* -------- ADC_OVER : (ADC Offset: 0x3C) Overrun Status Register -------- */
#define ADC_OVER_OVRE0 (0x1u << 0) /**< \brief (ADC_OVER) Overrun Error 0 */
#define ADC_OVER_OVRE1 (0x1u << 1) /**< \brief (ADC_OVER) Overrun Error 1 */
#define ADC_OVER_OVRE2 (0x1u << 2) /**< \brief (ADC_OVER) Overrun Error 2 */
#define ADC_OVER_OVRE3 (0x1u << 3) /**< \brief (ADC_OVER) Overrun Error 3 */
#define ADC_OVER_OVRE6 (0x1u << 6) /**< \brief (ADC_OVER) Overrun Error 6 */
#define ADC_OVER_OVRE7 (0x1u << 7) /**< \brief (ADC_OVER) Overrun Error 7 */
/* -------- ADC_EMR : (ADC Offset: 0x40) Extended Mode Register -------- */
#define ADC_EMR_CMPMODE_Pos 0
#define ADC_EMR_CMPMODE_Msk (0x3u << ADC_EMR_CMPMODE_Pos) /**< \brief (ADC_EMR) Comparison Mode */
#define   ADC_EMR_CMPMODE_LOW (0x0u << 0) /**< \brief (ADC_EMR) Generates an event when the converted data is lower than the low threshold of the window. */
#define   ADC_EMR_CMPMODE_HIGH (0x1u << 0) /**< \brief (ADC_EMR) Generates an event when the converted data is higher than the high threshold of the window. */
#define   ADC_EMR_CMPMODE_IN (0x2u << 0) /**< \brief (ADC_EMR) Generates an event when the converted data is in the comparison window. */
#define   ADC_EMR_CMPMODE_OUT (0x3u << 0) /**< \brief (ADC_EMR) Generates an event when the converted data is out of the comparison window. */
#define ADC_EMR_CMPSEL_Pos 4
#define ADC_EMR_CMPSEL_Msk (0xfu << ADC_EMR_CMPSEL_Pos) /**< \brief (ADC_EMR) Comparison Selected Channel */
#define ADC_EMR_CMPSEL(value) ((ADC_EMR_CMPSEL_Msk & ((value) << ADC_EMR_CMPSEL_Pos)))
#define ADC_EMR_CMPALL (0x1u << 9) /**< \brief (ADC_EMR) Compare All Channels */
#define ADC_EMR_CMPFILTER_Pos 12
#define ADC_EMR_CMPFILTER_Msk (0x3u << ADC_EMR_CMPFILTER_Pos) /**< \brief (ADC_EMR) Compare Event Filtering */
#define ADC_EMR_CMPFILTER(value) ((ADC_EMR_CMPFILTER_Msk & ((value) << ADC_EMR_CMPFILTER_Pos)))
#define ADC_EMR_OSR_Pos 16
#define ADC_EMR_OSR_Msk (0x3u << ADC_EMR_OSR_Pos) /**< \brief (ADC_EMR) Over Sampling Rate */
#define   ADC_EMR_OSR_NO_AVERAGE (0x0u << 16) /**< \brief (ADC_EMR) No averaging. ADC sample rate is maximum. */
#define   ADC_EMR_OSR_OSR4 (0x1u << 16) /**< \brief (ADC_EMR) 1-bit enhanced resolution by averaging. ADC sample rate divided by 4. */
#define   ADC_EMR_OSR_OSR16 (0x2u << 16) /**< \brief (ADC_EMR) 2-bit enhanced resolution by averaging. ADC sample rate divided by 16. */
#define ADC_EMR_ASTE (0x1u << 20) /**< \brief (ADC_EMR) Averaging on Single Trigger Event */
#define   ADC_EMR_ASTE_MULTI_TRIG_AVERAGE (0x0u << 20) /**< \brief (ADC_EMR) The average requests several trigger events. */
#define   ADC_EMR_ASTE_SINGLE_TRIG_AVERAGE (0x1u << 20) /**< \brief (ADC_EMR) The average requests only one trigger event. */
#define ADC_EMR_TAG (0x1u << 24) /**< \brief (ADC_EMR) TAG of the ADC_LDCR register */
/* -------- ADC_CWR : (ADC Offset: 0x44) Compare Window Register -------- */
#define ADC_CWR_LOWTHRES_Pos 0
#define ADC_CWR_LOWTHRES_Msk (0xfffu << ADC_CWR_LOWTHRES_Pos) /**< \brief (ADC_CWR) Low Threshold */
#define ADC_CWR_LOWTHRES(value) ((ADC_CWR_LOWTHRES_Msk & ((value) << ADC_CWR_LOWTHRES_Pos)))
#define ADC_CWR_HIGHTHRES_Pos 16
#define ADC_CWR_HIGHTHRES_Msk (0xfffu << ADC_CWR_HIGHTHRES_Pos) /**< \brief (ADC_CWR) High Threshold */
#define ADC_CWR_HIGHTHRES(value) ((ADC_CWR_HIGHTHRES_Msk & ((value) << ADC_CWR_HIGHTHRES_Pos)))
/* -------- ADC_CDR[8] : (ADC Offset: 0x50) Channel Data Register -------- */
#define ADC_CDR_DATA_Pos 0
#define ADC_CDR_DATA_Msk (0xfffu << ADC_CDR_DATA_Pos) /**< \brief (ADC_CDR[8]) Converted Data */
/* -------- ADC_ACR : (ADC Offset: 0x94) Analog Control Register -------- */
#define ADC_ACR_IRVCE (0x1u << 2) /**< \brief (ADC_ACR) Internal Reference Voltage Change Enable */
#define   ADC_ACR_IRVCE_STUCK_AT_DEFAULT (0x0u << 2) /**< \brief (ADC_ACR) The internal reference voltage is stuck at the default value (see the Electrical Characteristics for further details). */
#define   ADC_ACR_IRVCE_SELECTION (0x1u << 2) /**< \brief (ADC_ACR) The internal reference voltage is defined by field IRVS. */
#define ADC_ACR_IRVS_Pos 3
#define ADC_ACR_IRVS_Msk (0xfu << ADC_ACR_IRVS_Pos) /**< \brief (ADC_ACR) Internal Reference Voltage Selection */
#define ADC_ACR_IRVS(value) ((ADC_ACR_IRVS_Msk & ((value) << ADC_ACR_IRVS_Pos)))
#define ADC_ACR_FORCEREF (0x1u << 19) /**< \brief (ADC_ACR) Force Internal Reference Voltage */
#define ADC_ACR_ONREF (0x1u << 20) /**< \brief (ADC_ACR) Internal Voltage Reference ON */
/* -------- ADC_WPMR : (ADC Offset: 0xE4) Write Protection Mode Register -------- */
#define ADC_WPMR_WPEN (0x1u << 0) /**< \brief (ADC_WPMR) Write Protect Enable */
#define ADC_WPMR_WPKEY_Pos 8
#define ADC_WPMR_WPKEY_Msk (0xffffffu << ADC_WPMR_WPKEY_Pos) /**< \brief (ADC_WPMR) Write Protect Key */
#define   ADC_WPMR_WPKEY_PASSWD (0x414443u << 8) /**< \brief (ADC_WPMR) Writing any other value in this field aborts the write operation of the WPEN bit.Always reads as 0 */
/* -------- ADC_WPSR : (ADC Offset: 0xE8) Write Protection Status Register -------- */
#define ADC_WPSR_WPVS (0x1u << 0) /**< \brief (ADC_WPSR) Write Protection Violation Status */
#define ADC_WPSR_WPVSRC_Pos 8
#define ADC_WPSR_WPVSRC_Msk (0xffffu << ADC_WPSR_WPVSRC_Pos) /**< \brief (ADC_WPSR) Write Protection Violation Source */
/* -------- ADC_RPR : (ADC Offset: 0x100) Receive Pointer Register -------- */
#define ADC_RPR_RXPTR_Pos 0
#define ADC_RPR_RXPTR_Msk (0xffffffffu << ADC_RPR_RXPTR_Pos) /**< \brief (ADC_RPR) Receive Pointer Register */
#define ADC_RPR_RXPTR(value) ((ADC_RPR_RXPTR_Msk & ((value) << ADC_RPR_RXPTR_Pos)))
/* -------- ADC_RCR : (ADC Offset: 0x104) Receive Counter Register -------- */
#define ADC_RCR_RXCTR_Pos 0
#define ADC_RCR_RXCTR_Msk (0xffffu << ADC_RCR_RXCTR_Pos) /**< \brief (ADC_RCR) Receive Counter Register */
#define ADC_RCR_RXCTR(value) ((ADC_RCR_RXCTR_Msk & ((value) << ADC_RCR_RXCTR_Pos)))
/* -------- ADC_RNPR : (ADC Offset: 0x110) Receive Next Pointer Register -------- */
#define ADC_RNPR_RXNPTR_Pos 0
#define ADC_RNPR_RXNPTR_Msk (0xffffffffu << ADC_RNPR_RXNPTR_Pos) /**< \brief (ADC_RNPR) Receive Next Pointer */
#define ADC_RNPR_RXNPTR(value) ((ADC_RNPR_RXNPTR_Msk & ((value) << ADC_RNPR_RXNPTR_Pos)))
/* -------- ADC_RNCR : (ADC Offset: 0x114) Receive Next Counter Register -------- */
#define ADC_RNCR_RXNCTR_Pos 0
#define ADC_RNCR_RXNCTR_Msk (0xffffu << ADC_RNCR_RXNCTR_Pos) /**< \brief (ADC_RNCR) Receive Next Counter */
#define ADC_RNCR_RXNCTR(value) ((ADC_RNCR_RXNCTR_Msk & ((value) << ADC_RNCR_RXNCTR_Pos)))
/* -------- ADC_PTCR : (ADC Offset: 0x120) Transfer Control Register -------- */
#define ADC_PTCR_RXTEN (0x1u << 0) /**< \brief (ADC_PTCR) Receiver Transfer Enable */
#define ADC_PTCR_RXTDIS (0x1u << 1) /**< \brief (ADC_PTCR) Receiver Transfer Disable */
#define ADC_PTCR_TXTEN (0x1u << 8) /**< \brief (ADC_PTCR) Transmitter Transfer Enable */
#define ADC_PTCR_TXTDIS (0x1u << 9) /**< \brief (ADC_PTCR) Transmitter Transfer Disable */
/* -------- ADC_PTSR : (ADC Offset: 0x124) Transfer Status Register -------- */
#define ADC_PTSR_RXTEN (0x1u << 0) /**< \brief (ADC_PTSR) Receiver Transfer Enable */
#define ADC_PTSR_TXTEN (0x1u << 8) /**< \brief (ADC_PTSR) Transmitter Transfer Enable */

/*@}*/


#endif /* _SAM4CM_ADC_COMPONENT_ */
