/* ---------------------------------------------------------------------------- */
/*                  Atmel Microcontroller Software Support                      */
/*                       SAM Software Package License                           */
/* ---------------------------------------------------------------------------- */
/* Copyright (c) %copyright_year%, Atmel Corporation                                        */
/*                                                                              */
/* All rights reserved.                                                         */
/*                                                                              */
/* Redistribution and use in source and binary forms, with or without           */
/* modification, are permitted provided that the following condition is met:    */
/*                                                                              */
/* - Redistributions of source code must retain the above copyright notice,     */
/* this list of conditions and the disclaimer below.                            */
/*                                                                              */
/* Atmel's name may not be used to endorse or promote products derived from     */
/* this software without specific prior written permission.                     */
/*                                                                              */
/* DISCLAIMER:  THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR   */
/* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE   */
/* DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,      */
/* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT */
/* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,  */
/* OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF    */
/* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING         */
/* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, */
/* EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                           */
/* ---------------------------------------------------------------------------- */

#ifndef _SAM4E_AFEC_COMPONENT_
#define _SAM4E_AFEC_COMPONENT_

/* ============================================================================= */
/**  SOFTWARE API DEFINITION FOR Analog-Front-End Controller */
/* ============================================================================= */
/** \addtogroup SAM4E_AFEC Analog-Front-End Controller */
/*@{*/

#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
/** \brief Afec hardware registers */
typedef struct {
  WoReg AFE_CR;        /**< \brief (Afec Offset: 0x00) Control Register */
  RwReg AFE_MR;        /**< \brief (Afec Offset: 0x04) Mode Register */
  RwReg AFE_EMR;       /**< \brief (Afec Offset: 0x08) Extended Mode Register */
  RwReg AFE_SEQ1R;     /**< \brief (Afec Offset: 0x0C) Channel Sequence 1 Register */
  RwReg AFE_SEQ2R;     /**< \brief (Afec Offset: 0x10) Channel Sequence 2 Register */
  WoReg AFE_CHER;      /**< \brief (Afec Offset: 0x14) Channel Enable Register */
  WoReg AFE_CHDR;      /**< \brief (Afec Offset: 0x18) Channel Disable Register */
  RoReg AFE_CHSR;      /**< \brief (Afec Offset: 0x1C) Channel Status Register */
  RoReg AFE_LCDR;      /**< \brief (Afec Offset: 0x20) Last Converted Data Register */
  WoReg AFE_IER;       /**< \brief (Afec Offset: 0x24) Interrupt Enable Register */
  WoReg AFE_IDR;       /**< \brief (Afec Offset: 0x28) Interrupt Disable Register */
  RoReg AFE_IMR;       /**< \brief (Afec Offset: 0x2C) Interrupt Mask Register */
  RoReg AFE_ISR;       /**< \brief (Afec Offset: 0x30) Interrupt Status Register */
  RoReg Reserved1[6];
  RoReg AFE_OVER;      /**< \brief (Afec Offset: 0x4C) Overrun Status Register */
  RwReg AFE_CWR;       /**< \brief (Afec Offset: 0x50) Compare Window Register */
  RwReg AFE_CGR;       /**< \brief (Afec Offset: 0x54) Channel Gain Register */
  RoReg Reserved2[1];
  RwReg AFE_CDOR;      /**< \brief (Afec Offset: 0x5C) Channel DC Offset Register */
  RwReg AFE_DIFFR;     /**< \brief (Afec Offset: 0x60) Channel Differential Register */
  RwReg AFE_CSELR;     /**< \brief (Afec Offset: 0x64) Channel Register Selection */
  RoReg AFE_CDR;       /**< \brief (Afec Offset: 0x68) Channel Data Register */
  RwReg AFE_COCR;      /**< \brief (Afec Offset: 0x6C) Channel Offset Compensation Register */
  RwReg AFE_TEMPMR;    /**< \brief (Afec Offset: 0x70) Temperature Sensor Mode Register */
  RwReg AFE_TEMPCWR;   /**< \brief (Afec Offset: 0x74) Temperature Compare Window Register */
  RoReg Reserved3[7];
  RwReg AFE_ACR;       /**< \brief (Afec Offset: 0x94) Analog Control Register */
  RoReg Reserved4[19];
  RwReg AFE_WPMR;      /**< \brief (Afec Offset: 0xE4) Write Protect Mode Register */
  RoReg AFE_WPSR;      /**< \brief (Afec Offset: 0xE8) Write Protect Status Register */
  RoReg Reserved5[5];
  RwReg AFE_RPR;       /**< \brief (Afec Offset: 0x100) Receive Pointer Register */
  RwReg AFE_RCR;       /**< \brief (Afec Offset: 0x104) Receive Counter Register */
  RoReg Reserved6[2];
  RwReg AFE_RNPR;      /**< \brief (Afec Offset: 0x110) Receive Next Pointer Register */
  RwReg AFE_RNCR;      /**< \brief (Afec Offset: 0x114) Receive Next Counter Register */
  RoReg Reserved7[2];
  WoReg AFE_PTCR;      /**< \brief (Afec Offset: 0x120) Transfer Control Register */
  RoReg AFE_PTSR;      /**< \brief (Afec Offset: 0x124) Transfer Status Register */
} Afec;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */
/* -------- AFE_CR : (AFEC Offset: 0x00) Control Register -------- */
#define AFE_CR_SWRST (0x1u << 0) /**< \brief (AFE_CR) Software Reset */
#define AFE_CR_START (0x1u << 1) /**< \brief (AFE_CR) Start Conversion */
#define AFE_CR_AUTOCAL (0x1u << 3) /**< \brief (AFE_CR) Automatic Calibration of AFE */
/* -------- AFE_MR : (AFEC Offset: 0x04) Mode Register -------- */
#define AFE_MR_TRGEN (0x1u << 0) /**< \brief (AFE_MR) Trigger Enable */
#define   AFE_MR_TRGEN_DIS (0x0u << 0) /**< \brief (AFE_MR) Hardware triggers are disabled. Starting a conversion is only possible by software. */
#define   AFE_MR_TRGEN_EN (0x1u << 0) /**< \brief (AFE_MR) Hardware trigger selected by TRGSEL field is enabled. */
#define AFE_MR_TRGSEL_Pos 1
#define AFE_MR_TRGSEL_Msk (0x7u << AFE_MR_TRGSEL_Pos) /**< \brief (AFE_MR) Trigger Selection */
#define   AFE_MR_TRGSEL_AFE_TRIG0 (0x0u << 1) /**< \brief (AFE_MR) ADTRG pin */
#define   AFE_MR_TRGSEL_AFE_TRIG1 (0x1u << 1) /**< \brief (AFE_MR) TIO Output of the Timer Counter Channel 0 */
#define   AFE_MR_TRGSEL_AFE_TRIG2 (0x2u << 1) /**< \brief (AFE_MR) TIO Output of the Timer Counter Channel 1 */
#define   AFE_MR_TRGSEL_AFE_TRIG3 (0x3u << 1) /**< \brief (AFE_MR) TIO Output of the Timer Counter Channel 2 */
#define   AFE_MR_TRGSEL_AFE_TRIG4 (0x4u << 1) /**< \brief (AFE_MR) PWM Event Line 0 */
#define   AFE_MR_TRGSEL_AFE_TRIG5 (0x5u << 1) /**< \brief (AFE_MR) PWM Event Line 1 */
#define AFE_MR_SLEEP (0x1u << 5) /**< \brief (AFE_MR) Sleep Mode */
#define   AFE_MR_SLEEP_NORMAL (0x0u << 5) /**< \brief (AFE_MR) Normal Mode: The AFE Core and reference voltage circuitry are kept ON between conversions */
#define   AFE_MR_SLEEP_SLEEP (0x1u << 5) /**< \brief (AFE_MR) Sleep Mode: The AFE Core and reference voltage circuitry are OFF between conversions */
#define AFE_MR_FWUP (0x1u << 6) /**< \brief (AFE_MR) Fast Wake Up */
#define   AFE_MR_FWUP_OFF (0x0u << 6) /**< \brief (AFE_MR) Normal Sleep Mode: The sleep mode is defined by the SLEEP bit */
#define   AFE_MR_FWUP_ON (0x1u << 6) /**< \brief (AFE_MR) Fast Wake Up Sleep Mode: The Voltage reference is ON between conversions and AFE Core is OFF */
#define AFE_MR_FREERUN (0x1u << 7) /**< \brief (AFE_MR) Free Run Mode */
#define   AFE_MR_FREERUN_OFF (0x0u << 7) /**< \brief (AFE_MR) Normal Mode */
#define   AFE_MR_FREERUN_ON (0x1u << 7) /**< \brief (AFE_MR) Free Run Mode: Never wait for any trigger. */
#define AFE_MR_PRESCAL_Pos 8
#define AFE_MR_PRESCAL_Msk (0xffu << AFE_MR_PRESCAL_Pos) /**< \brief (AFE_MR) Prescaler Rate Selection */
#define AFE_MR_PRESCAL(value) ((AFE_MR_PRESCAL_Msk & ((value) << AFE_MR_PRESCAL_Pos)))
#define AFE_MR_STARTUP_Pos 16
#define AFE_MR_STARTUP_Msk (0xfu << AFE_MR_STARTUP_Pos) /**< \brief (AFE_MR) Start Up Time */
#define   AFE_MR_STARTUP_SUT0 (0x0u << 16) /**< \brief (AFE_MR) 0 periods of AFEClock */
#define   AFE_MR_STARTUP_SUT8 (0x1u << 16) /**< \brief (AFE_MR) 8 periods of AFEClock */
#define   AFE_MR_STARTUP_SUT16 (0x2u << 16) /**< \brief (AFE_MR) 16 periods of AFEClock */
#define   AFE_MR_STARTUP_SUT24 (0x3u << 16) /**< \brief (AFE_MR) 24 periods of AFEClock */
#define   AFE_MR_STARTUP_SUT64 (0x4u << 16) /**< \brief (AFE_MR) 64 periods of AFEClock */
#define   AFE_MR_STARTUP_SUT80 (0x5u << 16) /**< \brief (AFE_MR) 80 periods of AFEClock */
#define   AFE_MR_STARTUP_SUT96 (0x6u << 16) /**< \brief (AFE_MR) 96 periods of AFEClock */
#define   AFE_MR_STARTUP_SUT112 (0x7u << 16) /**< \brief (AFE_MR) 112 periods of AFEClock */
#define   AFE_MR_STARTUP_SUT512 (0x8u << 16) /**< \brief (AFE_MR) 512 periods of AFEClock */
#define   AFE_MR_STARTUP_SUT576 (0x9u << 16) /**< \brief (AFE_MR) 576 periods of AFEClock */
#define   AFE_MR_STARTUP_SUT640 (0xAu << 16) /**< \brief (AFE_MR) 640 periods of AFEClock */
#define   AFE_MR_STARTUP_SUT704 (0xBu << 16) /**< \brief (AFE_MR) 704 periods of AFEClock */
#define   AFE_MR_STARTUP_SUT768 (0xCu << 16) /**< \brief (AFE_MR) 768 periods of AFEClock */
#define   AFE_MR_STARTUP_SUT832 (0xDu << 16) /**< \brief (AFE_MR) 832 periods of AFEClock */
#define   AFE_MR_STARTUP_SUT896 (0xEu << 16) /**< \brief (AFE_MR) 896 periods of AFEClock */
#define   AFE_MR_STARTUP_SUT960 (0xFu << 16) /**< \brief (AFE_MR) 960 periods of AFEClock */
#define AFE_MR_SETTLING_Pos 20
#define AFE_MR_SETTLING_Msk (0x3u << AFE_MR_SETTLING_Pos) /**< \brief (AFE_MR) Analog Settling Time */
#define   AFE_MR_SETTLING_AST3 (0x0u << 20) /**< \brief (AFE_MR) 3 periods of AFEClock */
#define   AFE_MR_SETTLING_AST5 (0x1u << 20) /**< \brief (AFE_MR) 5 periods of AFEClock */
#define   AFE_MR_SETTLING_AST9 (0x2u << 20) /**< \brief (AFE_MR) 9 periods of AFEClock */
#define   AFE_MR_SETTLING_AST17 (0x3u << 20) /**< \brief (AFE_MR) 17 periods of AFEClock */
#define AFE_MR_ANACH (0x1u << 23) /**< \brief (AFE_MR) Analog Change */
#define   AFE_MR_ANACH_NONE (0x0u << 23) /**< \brief (AFE_MR) No analog change on channel switching: DIFF0, GAIN0 and OFF0 are used for all channels */
#define   AFE_MR_ANACH_ALLOWED (0x1u << 23) /**< \brief (AFE_MR) Allows different analog settings for each channel. See AFE_CGR and AFE_CDOR Registers */
#define AFE_MR_TRACKTIM_Pos 24
#define AFE_MR_TRACKTIM_Msk (0xfu << AFE_MR_TRACKTIM_Pos) /**< \brief (AFE_MR) Tracking Time */
#define AFE_MR_TRACKTIM(value) ((AFE_MR_TRACKTIM_Msk & ((value) << AFE_MR_TRACKTIM_Pos)))
#define AFE_MR_TRANSFER_Pos 28
#define AFE_MR_TRANSFER_Msk (0x3u << AFE_MR_TRANSFER_Pos) /**< \brief (AFE_MR) Transfer Period */
#define AFE_MR_TRANSFER(value) ((AFE_MR_TRANSFER_Msk & ((value) << AFE_MR_TRANSFER_Pos)))
#define AFE_MR_USEQ (0x1u << 31) /**< \brief (AFE_MR) Use Sequence Enable */
#define   AFE_MR_USEQ_NUM_ORDER (0x0u << 31) /**< \brief (AFE_MR) Normal Mode: The controller converts channels in a simple numeric order. */
#define   AFE_MR_USEQ_REG_ORDER (0x1u << 31) /**< \brief (AFE_MR) User Sequence Mode: The sequence respects what is defined in AFE_SEQR1 and AFE_SEQR2 registers. */
/* -------- AFE_EMR : (AFEC Offset: 0x08) Extended Mode Register -------- */
#define AFE_EMR_CMPMODE_Pos 0
#define AFE_EMR_CMPMODE_Msk (0x3u << AFE_EMR_CMPMODE_Pos) /**< \brief (AFE_EMR) Comparison Mode */
#define   AFE_EMR_CMPMODE_LOW (0x0u << 0) /**< \brief (AFE_EMR) Generates an event when the converted data is lower than the low threshold of the window. */
#define   AFE_EMR_CMPMODE_HIGH (0x1u << 0) /**< \brief (AFE_EMR) Generates an event when the converted data is higher than the high threshold of the window. */
#define   AFE_EMR_CMPMODE_IN (0x2u << 0) /**< \brief (AFE_EMR) Generates an event when the converted data is in the comparison window. */
#define   AFE_EMR_CMPMODE_OUT (0x3u << 0) /**< \brief (AFE_EMR) Generates an event when the converted data is out of the comparison window. */
#define AFE_EMR_CMPSEL_Pos 3
#define AFE_EMR_CMPSEL_Msk (0x1fu << AFE_EMR_CMPSEL_Pos) /**< \brief (AFE_EMR) Comparison Selected Channel */
#define AFE_EMR_CMPSEL(value) ((AFE_EMR_CMPSEL_Msk & ((value) << AFE_EMR_CMPSEL_Pos)))
#define AFE_EMR_CMPALL (0x1u << 9) /**< \brief (AFE_EMR) Compare All Channels */
#define AFE_EMR_CMPFILTER_Pos 12
#define AFE_EMR_CMPFILTER_Msk (0x3u << AFE_EMR_CMPFILTER_Pos) /**< \brief (AFE_EMR) Compare Event Filtering */
#define AFE_EMR_CMPFILTER(value) ((AFE_EMR_CMPFILTER_Msk & ((value) << AFE_EMR_CMPFILTER_Pos)))
#define AFE_EMR_RES_Pos 16
#define AFE_EMR_RES_Msk (0x7u << AFE_EMR_RES_Pos) /**< \brief (AFE_EMR) Resolution */
#define   AFE_EMR_RES_NO_AVERAGE (0x0u << 16) /**< \brief (AFE_EMR) 12-bit resolution, AFE sample rate is maximum (no averaging). */
#define   AFE_EMR_RES_LOW_RES (0x1u << 16) /**< \brief (AFE_EMR) 10-bit resolution, AFE sample rate is maximum (no averaging). */
#define   AFE_EMR_RES_OSR4 (0x2u << 16) /**< \brief (AFE_EMR) 13-bit resolution, AFE sample rate divided by 4 (averaging). */
#define   AFE_EMR_RES_OSR16 (0x3u << 16) /**< \brief (AFE_EMR) 14-bit resolution, AFE sample rate divided by 16 (averaging). */
#define   AFE_EMR_RES_OSR64 (0x4u << 16) /**< \brief (AFE_EMR) 15-bit resolution, AFE sample rate divided by 64 (averaging). */
#define   AFE_EMR_RES_OSR256 (0x5u << 16) /**< \brief (AFE_EMR) 16-bit resolution, AFE sample rate divided by 256 (averaging). */
#define AFE_EMR_TAG (0x1u << 24) /**< \brief (AFE_EMR) TAG of AFE_LDCR register */
#define AFE_EMR_STM (0x1u << 25) /**< \brief (AFE_EMR) Single Trigger Mode */
/* -------- AFE_SEQ1R : (AFEC Offset: 0x0C) Channel Sequence 1 Register -------- */
#define AFE_SEQ1R_USCH0_Pos 0
#define AFE_SEQ1R_USCH0_Msk (0xfu << AFE_SEQ1R_USCH0_Pos) /**< \brief (AFE_SEQ1R) User Sequence Number 0 */
#define AFE_SEQ1R_USCH0(value) ((AFE_SEQ1R_USCH0_Msk & ((value) << AFE_SEQ1R_USCH0_Pos)))
#define AFE_SEQ1R_USCH1_Pos 4
#define AFE_SEQ1R_USCH1_Msk (0xfu << AFE_SEQ1R_USCH1_Pos) /**< \brief (AFE_SEQ1R) User Sequence Number 1 */
#define AFE_SEQ1R_USCH1(value) ((AFE_SEQ1R_USCH1_Msk & ((value) << AFE_SEQ1R_USCH1_Pos)))
#define AFE_SEQ1R_USCH2_Pos 8
#define AFE_SEQ1R_USCH2_Msk (0xfu << AFE_SEQ1R_USCH2_Pos) /**< \brief (AFE_SEQ1R) User Sequence Number 2 */
#define AFE_SEQ1R_USCH2(value) ((AFE_SEQ1R_USCH2_Msk & ((value) << AFE_SEQ1R_USCH2_Pos)))
#define AFE_SEQ1R_USCH3_Pos 12
#define AFE_SEQ1R_USCH3_Msk (0xfu << AFE_SEQ1R_USCH3_Pos) /**< \brief (AFE_SEQ1R) User Sequence Number 3 */
#define AFE_SEQ1R_USCH3(value) ((AFE_SEQ1R_USCH3_Msk & ((value) << AFE_SEQ1R_USCH3_Pos)))
#define AFE_SEQ1R_USCH4_Pos 16
#define AFE_SEQ1R_USCH4_Msk (0xfu << AFE_SEQ1R_USCH4_Pos) /**< \brief (AFE_SEQ1R) User Sequence Number 4 */
#define AFE_SEQ1R_USCH4(value) ((AFE_SEQ1R_USCH4_Msk & ((value) << AFE_SEQ1R_USCH4_Pos)))
#define AFE_SEQ1R_USCH5_Pos 20
#define AFE_SEQ1R_USCH5_Msk (0xfu << AFE_SEQ1R_USCH5_Pos) /**< \brief (AFE_SEQ1R) User Sequence Number 5 */
#define AFE_SEQ1R_USCH5(value) ((AFE_SEQ1R_USCH5_Msk & ((value) << AFE_SEQ1R_USCH5_Pos)))
#define AFE_SEQ1R_USCH6_Pos 24
#define AFE_SEQ1R_USCH6_Msk (0xfu << AFE_SEQ1R_USCH6_Pos) /**< \brief (AFE_SEQ1R) User Sequence Number 6 */
#define AFE_SEQ1R_USCH6(value) ((AFE_SEQ1R_USCH6_Msk & ((value) << AFE_SEQ1R_USCH6_Pos)))
#define AFE_SEQ1R_USCH7_Pos 28
#define AFE_SEQ1R_USCH7_Msk (0xfu << AFE_SEQ1R_USCH7_Pos) /**< \brief (AFE_SEQ1R) User Sequence Number 7 */
#define AFE_SEQ1R_USCH7(value) ((AFE_SEQ1R_USCH7_Msk & ((value) << AFE_SEQ1R_USCH7_Pos)))
/* -------- AFE_SEQ2R : (AFEC Offset: 0x10) Channel Sequence 2 Register -------- */
#define AFE_SEQ2R_USCH8_Pos 0
#define AFE_SEQ2R_USCH8_Msk (0xfu << AFE_SEQ2R_USCH8_Pos) /**< \brief (AFE_SEQ2R) User Sequence Number 8 */
#define AFE_SEQ2R_USCH8(value) ((AFE_SEQ2R_USCH8_Msk & ((value) << AFE_SEQ2R_USCH8_Pos)))
#define AFE_SEQ2R_USCH9_Pos 4
#define AFE_SEQ2R_USCH9_Msk (0xfu << AFE_SEQ2R_USCH9_Pos) /**< \brief (AFE_SEQ2R) User Sequence Number 9 */
#define AFE_SEQ2R_USCH9(value) ((AFE_SEQ2R_USCH9_Msk & ((value) << AFE_SEQ2R_USCH9_Pos)))
#define AFE_SEQ2R_USCH10_Pos 8
#define AFE_SEQ2R_USCH10_Msk (0xfu << AFE_SEQ2R_USCH10_Pos) /**< \brief (AFE_SEQ2R) User Sequence Number 10 */
#define AFE_SEQ2R_USCH10(value) ((AFE_SEQ2R_USCH10_Msk & ((value) << AFE_SEQ2R_USCH10_Pos)))
#define AFE_SEQ2R_USCH11_Pos 12
#define AFE_SEQ2R_USCH11_Msk (0xfu << AFE_SEQ2R_USCH11_Pos) /**< \brief (AFE_SEQ2R) User Sequence Number 11 */
#define AFE_SEQ2R_USCH11(value) ((AFE_SEQ2R_USCH11_Msk & ((value) << AFE_SEQ2R_USCH11_Pos)))
#define AFE_SEQ2R_USCH12_Pos 16
#define AFE_SEQ2R_USCH12_Msk (0xfu << AFE_SEQ2R_USCH12_Pos) /**< \brief (AFE_SEQ2R) User Sequence Number 12 */
#define AFE_SEQ2R_USCH12(value) ((AFE_SEQ2R_USCH12_Msk & ((value) << AFE_SEQ2R_USCH12_Pos)))
#define AFE_SEQ2R_USCH13_Pos 20
#define AFE_SEQ2R_USCH13_Msk (0xfu << AFE_SEQ2R_USCH13_Pos) /**< \brief (AFE_SEQ2R) User Sequence Number 13 */
#define AFE_SEQ2R_USCH13(value) ((AFE_SEQ2R_USCH13_Msk & ((value) << AFE_SEQ2R_USCH13_Pos)))
#define AFE_SEQ2R_USCH14_Pos 24
#define AFE_SEQ2R_USCH14_Msk (0xfu << AFE_SEQ2R_USCH14_Pos) /**< \brief (AFE_SEQ2R) User Sequence Number 14 */
#define AFE_SEQ2R_USCH14(value) ((AFE_SEQ2R_USCH14_Msk & ((value) << AFE_SEQ2R_USCH14_Pos)))
#define AFE_SEQ2R_USCH15_Pos 28
#define AFE_SEQ2R_USCH15_Msk (0xfu << AFE_SEQ2R_USCH15_Pos) /**< \brief (AFE_SEQ2R) User Sequence Number 15 */
#define AFE_SEQ2R_USCH15(value) ((AFE_SEQ2R_USCH15_Msk & ((value) << AFE_SEQ2R_USCH15_Pos)))
/* -------- AFE_CHER : (AFEC Offset: 0x14) Channel Enable Register -------- */
#define AFE_CHER_CH0 (0x1u << 0) /**< \brief (AFE_CHER) Channel 0 Enable */
#define AFE_CHER_CH1 (0x1u << 1) /**< \brief (AFE_CHER) Channel 1 Enable */
#define AFE_CHER_CH2 (0x1u << 2) /**< \brief (AFE_CHER) Channel 2 Enable */
#define AFE_CHER_CH3 (0x1u << 3) /**< \brief (AFE_CHER) Channel 3 Enable */
#define AFE_CHER_CH4 (0x1u << 4) /**< \brief (AFE_CHER) Channel 4 Enable */
#define AFE_CHER_CH5 (0x1u << 5) /**< \brief (AFE_CHER) Channel 5 Enable */
#define AFE_CHER_CH6 (0x1u << 6) /**< \brief (AFE_CHER) Channel 6 Enable */
#define AFE_CHER_CH7 (0x1u << 7) /**< \brief (AFE_CHER) Channel 7 Enable */
#define AFE_CHER_CH8 (0x1u << 8) /**< \brief (AFE_CHER) Channel 8 Enable */
#define AFE_CHER_CH9 (0x1u << 9) /**< \brief (AFE_CHER) Channel 9 Enable */
#define AFE_CHER_CH10 (0x1u << 10) /**< \brief (AFE_CHER) Channel 10 Enable */
#define AFE_CHER_CH11 (0x1u << 11) /**< \brief (AFE_CHER) Channel 11 Enable */
#define AFE_CHER_CH12 (0x1u << 12) /**< \brief (AFE_CHER) Channel 12 Enable */
#define AFE_CHER_CH13 (0x1u << 13) /**< \brief (AFE_CHER) Channel 13 Enable */
#define AFE_CHER_CH14 (0x1u << 14) /**< \brief (AFE_CHER) Channel 14 Enable */
#define AFE_CHER_CH15 (0x1u << 15) /**< \brief (AFE_CHER) Channel 15 Enable */
#define AFE_CHER_CH16 (0x1u << 16) /**< \brief (AFE_CHER) Channel 16 Enable */
#define AFE_CHER_DIFF17 (0x1u << 17) /**< \brief (AFE_CHER)  */
#define AFE_CHER_CH18 (0x1u << 18) /**< \brief (AFE_CHER) Channel 18 Enable */
#define AFE_CHER_CH19 (0x1u << 19) /**< \brief (AFE_CHER) Channel 19 Enable */
#define AFE_CHER_CH20 (0x1u << 20) /**< \brief (AFE_CHER) Channel 20 Enable */
#define AFE_CHER_CH21 (0x1u << 21) /**< \brief (AFE_CHER) Channel 21 Enable */
#define AFE_CHER_CH22 (0x1u << 22) /**< \brief (AFE_CHER) Channel 22 Enable */
#define AFE_CHER_CH23 (0x1u << 23) /**< \brief (AFE_CHER) Channel 23 Enable */
/* -------- AFE_CHDR : (AFEC Offset: 0x18) Channel Disable Register -------- */
#define AFE_CHDR_CH0 (0x1u << 0) /**< \brief (AFE_CHDR) Channel 0 Disable */
#define AFE_CHDR_CH1 (0x1u << 1) /**< \brief (AFE_CHDR) Channel 1 Disable */
#define AFE_CHDR_CH2 (0x1u << 2) /**< \brief (AFE_CHDR) Channel 2 Disable */
#define AFE_CHDR_CH3 (0x1u << 3) /**< \brief (AFE_CHDR) Channel 3 Disable */
#define AFE_CHDR_CH4 (0x1u << 4) /**< \brief (AFE_CHDR) Channel 4 Disable */
#define AFE_CHDR_CH5 (0x1u << 5) /**< \brief (AFE_CHDR) Channel 5 Disable */
#define AFE_CHDR_CH6 (0x1u << 6) /**< \brief (AFE_CHDR) Channel 6 Disable */
#define AFE_CHDR_CH7 (0x1u << 7) /**< \brief (AFE_CHDR) Channel 7 Disable */
#define AFE_CHDR_CH8 (0x1u << 8) /**< \brief (AFE_CHDR) Channel 8 Disable */
#define AFE_CHDR_CH9 (0x1u << 9) /**< \brief (AFE_CHDR) Channel 9 Disable */
#define AFE_CHDR_CH10 (0x1u << 10) /**< \brief (AFE_CHDR) Channel 10 Disable */
#define AFE_CHDR_CH11 (0x1u << 11) /**< \brief (AFE_CHDR) Channel 11 Disable */
#define AFE_CHDR_CH12 (0x1u << 12) /**< \brief (AFE_CHDR) Channel 12 Disable */
#define AFE_CHDR_CH13 (0x1u << 13) /**< \brief (AFE_CHDR) Channel 13 Disable */
#define AFE_CHDR_CH14 (0x1u << 14) /**< \brief (AFE_CHDR) Channel 14 Disable */
#define AFE_CHDR_CH15 (0x1u << 15) /**< \brief (AFE_CHDR) Channel 15 Disable */
#define AFE_CHDR_CH16 (0x1u << 16) /**< \brief (AFE_CHDR) Channel 16 Disable */
#define AFE_CHDR_DIFF17 (0x1u << 17) /**< \brief (AFE_CHDR)  */
#define AFE_CHDR_CH18 (0x1u << 18) /**< \brief (AFE_CHDR) Channel 18 Disable */
#define AFE_CHDR_CH19 (0x1u << 19) /**< \brief (AFE_CHDR) Channel 19 Disable */
#define AFE_CHDR_CH20 (0x1u << 20) /**< \brief (AFE_CHDR) Channel 20 Disable */
#define AFE_CHDR_CH21 (0x1u << 21) /**< \brief (AFE_CHDR) Channel 21 Disable */
#define AFE_CHDR_CH22 (0x1u << 22) /**< \brief (AFE_CHDR) Channel 22 Disable */
#define AFE_CHDR_CH23 (0x1u << 23) /**< \brief (AFE_CHDR) Channel 23 Disable */
/* -------- AFE_CHSR : (AFEC Offset: 0x1C) Channel Status Register -------- */
#define AFE_CHSR_CH0 (0x1u << 0) /**< \brief (AFE_CHSR) Channel 0 Status */
#define AFE_CHSR_CH1 (0x1u << 1) /**< \brief (AFE_CHSR) Channel 1 Status */
#define AFE_CHSR_CH2 (0x1u << 2) /**< \brief (AFE_CHSR) Channel 2 Status */
#define AFE_CHSR_CH3 (0x1u << 3) /**< \brief (AFE_CHSR) Channel 3 Status */
#define AFE_CHSR_CH4 (0x1u << 4) /**< \brief (AFE_CHSR) Channel 4 Status */
#define AFE_CHSR_CH5 (0x1u << 5) /**< \brief (AFE_CHSR) Channel 5 Status */
#define AFE_CHSR_CH6 (0x1u << 6) /**< \brief (AFE_CHSR) Channel 6 Status */
#define AFE_CHSR_CH7 (0x1u << 7) /**< \brief (AFE_CHSR) Channel 7 Status */
#define AFE_CHSR_CH8 (0x1u << 8) /**< \brief (AFE_CHSR) Channel 8 Status */
#define AFE_CHSR_CH9 (0x1u << 9) /**< \brief (AFE_CHSR) Channel 9 Status */
#define AFE_CHSR_CH10 (0x1u << 10) /**< \brief (AFE_CHSR) Channel 10 Status */
#define AFE_CHSR_CH11 (0x1u << 11) /**< \brief (AFE_CHSR) Channel 11 Status */
#define AFE_CHSR_CH12 (0x1u << 12) /**< \brief (AFE_CHSR) Channel 12 Status */
#define AFE_CHSR_CH13 (0x1u << 13) /**< \brief (AFE_CHSR) Channel 13 Status */
#define AFE_CHSR_CH14 (0x1u << 14) /**< \brief (AFE_CHSR) Channel 14 Status */
#define AFE_CHSR_CH15 (0x1u << 15) /**< \brief (AFE_CHSR) Channel 15 Status */
#define AFE_CHSR_CH16 (0x1u << 16) /**< \brief (AFE_CHSR) Channel 16 Status */
#define AFE_CHSR_DIFF17 (0x1u << 17) /**< \brief (AFE_CHSR)  */
#define AFE_CHSR_CH18 (0x1u << 18) /**< \brief (AFE_CHSR) Channel 18 Status */
#define AFE_CHSR_CH19 (0x1u << 19) /**< \brief (AFE_CHSR) Channel 19 Status */
#define AFE_CHSR_CH20 (0x1u << 20) /**< \brief (AFE_CHSR) Channel 20 Status */
#define AFE_CHSR_CH21 (0x1u << 21) /**< \brief (AFE_CHSR) Channel 21 Status */
#define AFE_CHSR_CH22 (0x1u << 22) /**< \brief (AFE_CHSR) Channel 22 Status */
#define AFE_CHSR_CH23 (0x1u << 23) /**< \brief (AFE_CHSR) Channel 23 Status */
/* -------- AFE_LCDR : (AFEC Offset: 0x20) Last Converted Data Register -------- */
#define AFE_LCDR_LDATA_Pos 0
#define AFE_LCDR_LDATA_Msk (0xffffu << AFE_LCDR_LDATA_Pos) /**< \brief (AFE_LCDR) Last Data Converted */
#define AFE_LCDR_CHNB_Pos 24
#define AFE_LCDR_CHNB_Msk (0xfu << AFE_LCDR_CHNB_Pos) /**< \brief (AFE_LCDR) Channel Number */
/* -------- AFE_IER : (AFEC Offset: 0x24) Interrupt Enable Register -------- */
#define AFE_IER_EOC0 (0x1u << 0) /**< \brief (AFE_IER) End of Conversion Interrupt Enable 0 */
#define AFE_IER_EOC1 (0x1u << 1) /**< \brief (AFE_IER) End of Conversion Interrupt Enable 1 */
#define AFE_IER_EOC2 (0x1u << 2) /**< \brief (AFE_IER) End of Conversion Interrupt Enable 2 */
#define AFE_IER_EOC3 (0x1u << 3) /**< \brief (AFE_IER) End of Conversion Interrupt Enable 3 */
#define AFE_IER_EOC4 (0x1u << 4) /**< \brief (AFE_IER) End of Conversion Interrupt Enable 4 */
#define AFE_IER_EOC5 (0x1u << 5) /**< \brief (AFE_IER) End of Conversion Interrupt Enable 5 */
#define AFE_IER_EOC6 (0x1u << 6) /**< \brief (AFE_IER) End of Conversion Interrupt Enable 6 */
#define AFE_IER_EOC7 (0x1u << 7) /**< \brief (AFE_IER) End of Conversion Interrupt Enable 7 */
#define AFE_IER_EOC8 (0x1u << 8) /**< \brief (AFE_IER) End of Conversion Interrupt Enable 8 */
#define AFE_IER_EOC9 (0x1u << 9) /**< \brief (AFE_IER) End of Conversion Interrupt Enable 9 */
#define AFE_IER_EOC10 (0x1u << 10) /**< \brief (AFE_IER) End of Conversion Interrupt Enable 10 */
#define AFE_IER_EOC11 (0x1u << 11) /**< \brief (AFE_IER) End of Conversion Interrupt Enable 11 */
#define AFE_IER_EOC12 (0x1u << 12) /**< \brief (AFE_IER) End of Conversion Interrupt Enable 12 */
#define AFE_IER_EOC13 (0x1u << 13) /**< \brief (AFE_IER) End of Conversion Interrupt Enable 13 */
#define AFE_IER_EOC14 (0x1u << 14) /**< \brief (AFE_IER) End of Conversion Interrupt Enable 14 */
#define AFE_IER_EOC15 (0x1u << 15) /**< \brief (AFE_IER) End of Conversion Interrupt Enable 15 */
#define AFE_IER_EOC16 (0x1u << 16) /**< \brief (AFE_IER) End of Conversion Interrupt Enable 16 */
#define AFE_IER_EOC17 (0x1u << 17) /**< \brief (AFE_IER) End of Conversion Interrupt Enable 17 */
#define AFE_IER_EOC18 (0x1u << 18) /**< \brief (AFE_IER) End of Conversion Interrupt Enable 18 */
#define AFE_IER_EOC19 (0x1u << 19) /**< \brief (AFE_IER) End of Conversion Interrupt Enable 19 */
#define AFE_IER_EOC20 (0x1u << 20) /**< \brief (AFE_IER) End of Conversion Interrupt Enable 20 */
#define AFE_IER_EOC21 (0x1u << 21) /**< \brief (AFE_IER) End of Conversion Interrupt Enable 21 */
#define AFE_IER_EOC22 (0x1u << 22) /**< \brief (AFE_IER) End of Conversion Interrupt Enable 22 */
#define AFE_IER_EOC23 (0x1u << 23) /**< \brief (AFE_IER) End of Conversion Interrupt Enable 23 */
#define AFE_IER_DRDY (0x1u << 24) /**< \brief (AFE_IER) Data Ready Interrupt Enable */
#define AFE_IER_GOVRE (0x1u << 25) /**< \brief (AFE_IER) General Overrun Error Interrupt Enable */
#define AFE_IER_COMPE (0x1u << 26) /**< \brief (AFE_IER) Comparison Event Interrupt Enable+ */
#define AFE_IER_ENDRX (0x1u << 27) /**< \brief (AFE_IER) End of Receive Buffer Interrupt Enable */
#define AFE_IER_RXBUFF (0x1u << 28) /**< \brief (AFE_IER) Receive Buffer Full Interrupt Enable */
#define AFE_IER_TEMPCHG (0x1u << 30) /**< \brief (AFE_IER) Temperature Change Interrupt Enable */
#define AFE_IER_EOCAL (0x1u << 31) /**< \brief (AFE_IER) End of Calibration Sequence Interrupt Enable */
/* -------- AFE_IDR : (AFEC Offset: 0x28) Interrupt Disable Register -------- */
#define AFE_IDR_EOC0 (0x1u << 0) /**< \brief (AFE_IDR) End of Conversion Interrupt Disable 0 */
#define AFE_IDR_EOC1 (0x1u << 1) /**< \brief (AFE_IDR) End of Conversion Interrupt Disable 1 */
#define AFE_IDR_EOC2 (0x1u << 2) /**< \brief (AFE_IDR) End of Conversion Interrupt Disable 2 */
#define AFE_IDR_EOC3 (0x1u << 3) /**< \brief (AFE_IDR) End of Conversion Interrupt Disable 3 */
#define AFE_IDR_EOC4 (0x1u << 4) /**< \brief (AFE_IDR) End of Conversion Interrupt Disable 4 */
#define AFE_IDR_EOC5 (0x1u << 5) /**< \brief (AFE_IDR) End of Conversion Interrupt Disable 5 */
#define AFE_IDR_EOC6 (0x1u << 6) /**< \brief (AFE_IDR) End of Conversion Interrupt Disable 6 */
#define AFE_IDR_EOC7 (0x1u << 7) /**< \brief (AFE_IDR) End of Conversion Interrupt Disable 7 */
#define AFE_IDR_EOC8 (0x1u << 8) /**< \brief (AFE_IDR) End of Conversion Interrupt Disable 8 */
#define AFE_IDR_EOC9 (0x1u << 9) /**< \brief (AFE_IDR) End of Conversion Interrupt Disable 9 */
#define AFE_IDR_EOC10 (0x1u << 10) /**< \brief (AFE_IDR) End of Conversion Interrupt Disable 10 */
#define AFE_IDR_EOC11 (0x1u << 11) /**< \brief (AFE_IDR) End of Conversion Interrupt Disable 11 */
#define AFE_IDR_EOC12 (0x1u << 12) /**< \brief (AFE_IDR) End of Conversion Interrupt Disable 12 */
#define AFE_IDR_EOC13 (0x1u << 13) /**< \brief (AFE_IDR) End of Conversion Interrupt Disable 13 */
#define AFE_IDR_EOC14 (0x1u << 14) /**< \brief (AFE_IDR) End of Conversion Interrupt Disable 14 */
#define AFE_IDR_EOC15 (0x1u << 15) /**< \brief (AFE_IDR) End of Conversion Interrupt Disable 15 */
#define AFE_IDR_EOC16 (0x1u << 16) /**< \brief (AFE_IDR) End of Conversion Interrupt Disable 16 */
#define AFE_IDR_EOC17 (0x1u << 17) /**< \brief (AFE_IDR) End of Conversion Interrupt Disable 17 */
#define AFE_IDR_EOC18 (0x1u << 18) /**< \brief (AFE_IDR) End of Conversion Interrupt Disable 18 */
#define AFE_IDR_EOC19 (0x1u << 19) /**< \brief (AFE_IDR) End of Conversion Interrupt Disable 19 */
#define AFE_IDR_EOC20 (0x1u << 20) /**< \brief (AFE_IDR) End of Conversion Interrupt Disable 20 */
#define AFE_IDR_EOC21 (0x1u << 21) /**< \brief (AFE_IDR) End of Conversion Interrupt Disable 21 */
#define AFE_IDR_EOC22 (0x1u << 22) /**< \brief (AFE_IDR) End of Conversion Interrupt Disable 22 */
#define AFE_IDR_EOC23 (0x1u << 23) /**< \brief (AFE_IDR) End of Conversion Interrupt Disable 23 */
#define AFE_IDR_DRDY (0x1u << 24) /**< \brief (AFE_IDR) Data Ready Interrupt Disable */
#define AFE_IDR_GOVRE (0x1u << 25) /**< \brief (AFE_IDR) General Overrun Error Interrupt Disable */
#define AFE_IDR_COMPE (0x1u << 26) /**< \brief (AFE_IDR) Comparison Event Interrupt Disable */
#define AFE_IDR_ENDRX (0x1u << 27) /**< \brief (AFE_IDR) End of Receive Buffer Interrupt Disable */
#define AFE_IDR_RXBUFF (0x1u << 28) /**< \brief (AFE_IDR) Receive Buffer Full Interrupt Disable */
#define AFE_IDR_TEMPCHG (0x1u << 30) /**< \brief (AFE_IDR) Temperature Change Interrupt Disable */
#define AFE_IDR_EOCAL (0x1u << 31) /**< \brief (AFE_IDR) End of Calibration Sequence Interrupt Disable */
/* -------- AFE_IMR : (AFEC Offset: 0x2C) Interrupt Mask Register -------- */
#define AFE_IMR_EOC0 (0x1u << 0) /**< \brief (AFE_IMR) End of Conversion Interrupt Mask 0 */
#define AFE_IMR_EOC1 (0x1u << 1) /**< \brief (AFE_IMR) End of Conversion Interrupt Mask 1 */
#define AFE_IMR_EOC2 (0x1u << 2) /**< \brief (AFE_IMR) End of Conversion Interrupt Mask 2 */
#define AFE_IMR_EOC3 (0x1u << 3) /**< \brief (AFE_IMR) End of Conversion Interrupt Mask 3 */
#define AFE_IMR_EOC4 (0x1u << 4) /**< \brief (AFE_IMR) End of Conversion Interrupt Mask 4 */
#define AFE_IMR_EOC5 (0x1u << 5) /**< \brief (AFE_IMR) End of Conversion Interrupt Mask 5 */
#define AFE_IMR_EOC6 (0x1u << 6) /**< \brief (AFE_IMR) End of Conversion Interrupt Mask 6 */
#define AFE_IMR_EOC7 (0x1u << 7) /**< \brief (AFE_IMR) End of Conversion Interrupt Mask 7 */
#define AFE_IMR_EOC8 (0x1u << 8) /**< \brief (AFE_IMR) End of Conversion Interrupt Mask 8 */
#define AFE_IMR_EOC9 (0x1u << 9) /**< \brief (AFE_IMR) End of Conversion Interrupt Mask 9 */
#define AFE_IMR_EOC10 (0x1u << 10) /**< \brief (AFE_IMR) End of Conversion Interrupt Mask 10 */
#define AFE_IMR_EOC11 (0x1u << 11) /**< \brief (AFE_IMR) End of Conversion Interrupt Mask 11 */
#define AFE_IMR_EOC12 (0x1u << 12) /**< \brief (AFE_IMR) End of Conversion Interrupt Mask 12 */
#define AFE_IMR_EOC13 (0x1u << 13) /**< \brief (AFE_IMR) End of Conversion Interrupt Mask 13 */
#define AFE_IMR_EOC14 (0x1u << 14) /**< \brief (AFE_IMR) End of Conversion Interrupt Mask 14 */
#define AFE_IMR_EOC15 (0x1u << 15) /**< \brief (AFE_IMR) End of Conversion Interrupt Mask 15 */
#define AFE_IMR_EOC16 (0x1u << 16) /**< \brief (AFE_IMR) End of Conversion Interrupt Mask 16 */
#define AFE_IMR_EOC17 (0x1u << 17) /**< \brief (AFE_IMR) End of Conversion Interrupt Mask 17 */
#define AFE_IMR_EOC18 (0x1u << 18) /**< \brief (AFE_IMR) End of Conversion Interrupt Mask 18 */
#define AFE_IMR_EOC19 (0x1u << 19) /**< \brief (AFE_IMR) End of Conversion Interrupt Mask 19 */
#define AFE_IMR_EOC20 (0x1u << 20) /**< \brief (AFE_IMR) End of Conversion Interrupt Mask 20 */
#define AFE_IMR_EOC21 (0x1u << 21) /**< \brief (AFE_IMR) End of Conversion Interrupt Mask 21 */
#define AFE_IMR_EOC22 (0x1u << 22) /**< \brief (AFE_IMR) End of Conversion Interrupt Mask 22 */
#define AFE_IMR_EOC23 (0x1u << 23) /**< \brief (AFE_IMR) End of Conversion Interrupt Mask 23 */
#define AFE_IMR_DRDY (0x1u << 24) /**< \brief (AFE_IMR) Data Ready Interrupt Mask */
#define AFE_IMR_GOVRE (0x1u << 25) /**< \brief (AFE_IMR) General Overrun Error Interrupt Mask */
#define AFE_IMR_COMPE (0x1u << 26) /**< \brief (AFE_IMR) Comparison Event Interrupt Mask */
#define AFE_IMR_ENDRX (0x1u << 27) /**< \brief (AFE_IMR) End of Receive Buffer Interrupt Mask */
#define AFE_IMR_RXBUFF (0x1u << 28) /**< \brief (AFE_IMR) Receive Buffer Full Interrupt Mask */
#define AFE_IMR_TEMPCHG (0x1u << 30) /**< \brief (AFE_IMR) Temperature Change Interrupt Mask */
#define AFE_IMR_EOCAL (0x1u << 31) /**< \brief (AFE_IMR) End of Calibration Sequence Interrupt Mask */
/* -------- AFE_ISR : (AFEC Offset: 0x30) Interrupt Status Register -------- */
#define AFE_ISR_EOC0 (0x1u << 0) /**< \brief (AFE_ISR) End of Conversion 0 */
#define AFE_ISR_EOC1 (0x1u << 1) /**< \brief (AFE_ISR) End of Conversion 1 */
#define AFE_ISR_EOC2 (0x1u << 2) /**< \brief (AFE_ISR) End of Conversion 2 */
#define AFE_ISR_EOC3 (0x1u << 3) /**< \brief (AFE_ISR) End of Conversion 3 */
#define AFE_ISR_EOC4 (0x1u << 4) /**< \brief (AFE_ISR) End of Conversion 4 */
#define AFE_ISR_EOC5 (0x1u << 5) /**< \brief (AFE_ISR) End of Conversion 5 */
#define AFE_ISR_EOC6 (0x1u << 6) /**< \brief (AFE_ISR) End of Conversion 6 */
#define AFE_ISR_EOC7 (0x1u << 7) /**< \brief (AFE_ISR) End of Conversion 7 */
#define AFE_ISR_EOC8 (0x1u << 8) /**< \brief (AFE_ISR) End of Conversion 8 */
#define AFE_ISR_EOC9 (0x1u << 9) /**< \brief (AFE_ISR) End of Conversion 9 */
#define AFE_ISR_EOC10 (0x1u << 10) /**< \brief (AFE_ISR) End of Conversion 10 */
#define AFE_ISR_EOC11 (0x1u << 11) /**< \brief (AFE_ISR) End of Conversion 11 */
#define AFE_ISR_EOC12 (0x1u << 12) /**< \brief (AFE_ISR) End of Conversion 12 */
#define AFE_ISR_EOC13 (0x1u << 13) /**< \brief (AFE_ISR) End of Conversion 13 */
#define AFE_ISR_EOC14 (0x1u << 14) /**< \brief (AFE_ISR) End of Conversion 14 */
#define AFE_ISR_EOC15 (0x1u << 15) /**< \brief (AFE_ISR) End of Conversion 15 */
#define AFE_ISR_EOC16 (0x1u << 16) /**< \brief (AFE_ISR) End of Conversion 16 */
#define AFE_ISR_EOC17 (0x1u << 17) /**< \brief (AFE_ISR) End of Conversion 17 */
#define AFE_ISR_EOC18 (0x1u << 18) /**< \brief (AFE_ISR) End of Conversion 18 */
#define AFE_ISR_EOC19 (0x1u << 19) /**< \brief (AFE_ISR) End of Conversion 19 */
#define AFE_ISR_EOC20 (0x1u << 20) /**< \brief (AFE_ISR) End of Conversion 20 */
#define AFE_ISR_EOC21 (0x1u << 21) /**< \brief (AFE_ISR) End of Conversion 21 */
#define AFE_ISR_EOC22 (0x1u << 22) /**< \brief (AFE_ISR) End of Conversion 22 */
#define AFE_ISR_EOC23 (0x1u << 23) /**< \brief (AFE_ISR) End of Conversion 23 */
#define AFE_ISR_DRDY (0x1u << 24) /**< \brief (AFE_ISR) Data Ready */
#define AFE_ISR_GOVRE (0x1u << 25) /**< \brief (AFE_ISR) General Overrun Error */
#define AFE_ISR_COMPE (0x1u << 26) /**< \brief (AFE_ISR) Comparison Error */
#define AFE_ISR_ENDRX (0x1u << 27) /**< \brief (AFE_ISR) End of RX Buffer */
#define AFE_ISR_RXBUFF (0x1u << 28) /**< \brief (AFE_ISR) RX Buffer Full */
#define AFE_ISR_TEMPCHG (0x1u << 30) /**< \brief (AFE_ISR) Temperature Change */
#define AFE_ISR_EOCAL (0x1u << 31) /**< \brief (AFE_ISR) End of Calibration Sequence */
/* -------- AFE_OVER : (AFEC Offset: 0x4C) Overrun Status Register -------- */
#define AFE_OVER_OVRE0 (0x1u << 0) /**< \brief (AFE_OVER) Overrun Error 0 */
#define AFE_OVER_OVRE1 (0x1u << 1) /**< \brief (AFE_OVER) Overrun Error 1 */
#define AFE_OVER_OVRE2 (0x1u << 2) /**< \brief (AFE_OVER) Overrun Error 2 */
#define AFE_OVER_OVRE3 (0x1u << 3) /**< \brief (AFE_OVER) Overrun Error 3 */
#define AFE_OVER_OVRE4 (0x1u << 4) /**< \brief (AFE_OVER) Overrun Error 4 */
#define AFE_OVER_OVRE5 (0x1u << 5) /**< \brief (AFE_OVER) Overrun Error 5 */
#define AFE_OVER_OVRE6 (0x1u << 6) /**< \brief (AFE_OVER) Overrun Error 6 */
#define AFE_OVER_OVRE7 (0x1u << 7) /**< \brief (AFE_OVER) Overrun Error 7 */
#define AFE_OVER_OVRE8 (0x1u << 8) /**< \brief (AFE_OVER) Overrun Error 8 */
#define AFE_OVER_OVRE9 (0x1u << 9) /**< \brief (AFE_OVER) Overrun Error 9 */
#define AFE_OVER_OVRE10 (0x1u << 10) /**< \brief (AFE_OVER) Overrun Error 10 */
#define AFE_OVER_OVRE11 (0x1u << 11) /**< \brief (AFE_OVER) Overrun Error 11 */
#define AFE_OVER_OVRE12 (0x1u << 12) /**< \brief (AFE_OVER) Overrun Error 12 */
#define AFE_OVER_OVRE13 (0x1u << 13) /**< \brief (AFE_OVER) Overrun Error 13 */
#define AFE_OVER_OVRE14 (0x1u << 14) /**< \brief (AFE_OVER) Overrun Error 14 */
#define AFE_OVER_OVRE15 (0x1u << 15) /**< \brief (AFE_OVER) Overrun Error 15 */
#define AFE_OVER_OVRE16_Pos 16
#define AFE_OVER_OVRE16_Msk (0x3u << AFE_OVER_OVRE16_Pos) /**< \brief (AFE_OVER) Overrun Error 16 */
#define AFE_OVER_OVRE17 (0x1u << 18) /**< \brief (AFE_OVER) Overrun Error 17 */
#define AFE_OVER_OVRE18 (0x1u << 19) /**< \brief (AFE_OVER) Overrun Error 18 */
#define AFE_OVER_OVRE19 (0x1u << 20) /**< \brief (AFE_OVER) Overrun Error 19 */
#define AFE_OVER_OVRE20 (0x1u << 21) /**< \brief (AFE_OVER) Overrun Error 20 */
#define AFE_OVER_OVRE21 (0x1u << 22) /**< \brief (AFE_OVER) Overrun Error 21 */
#define AFE_OVER_OVRE22 (0x1u << 23) /**< \brief (AFE_OVER) Overrun Error 22 */
/* -------- AFE_CWR : (AFEC Offset: 0x50) Compare Window Register -------- */
#define AFE_CWR_LOWTHRES_Pos 0
#define AFE_CWR_LOWTHRES_Msk (0xfffu << AFE_CWR_LOWTHRES_Pos) /**< \brief (AFE_CWR) Low Threshold */
#define AFE_CWR_LOWTHRES(value) ((AFE_CWR_LOWTHRES_Msk & ((value) << AFE_CWR_LOWTHRES_Pos)))
#define AFE_CWR_HIGHTHRES_Pos 16
#define AFE_CWR_HIGHTHRES_Msk (0xfffu << AFE_CWR_HIGHTHRES_Pos) /**< \brief (AFE_CWR) High Threshold */
#define AFE_CWR_HIGHTHRES(value) ((AFE_CWR_HIGHTHRES_Msk & ((value) << AFE_CWR_HIGHTHRES_Pos)))
/* -------- AFE_CGR : (AFEC Offset: 0x54) Channel Gain Register -------- */
#define AFE_CGR_GAIN0_Pos 0
#define AFE_CGR_GAIN0_Msk (0x3u << AFE_CGR_GAIN0_Pos) /**< \brief (AFE_CGR) Gain for channel 0 */
#define AFE_CGR_GAIN0(value) ((AFE_CGR_GAIN0_Msk & ((value) << AFE_CGR_GAIN0_Pos)))
#define AFE_CGR_GAIN1_Pos 2
#define AFE_CGR_GAIN1_Msk (0x3u << AFE_CGR_GAIN1_Pos) /**< \brief (AFE_CGR) Gain for channel 1 */
#define AFE_CGR_GAIN1(value) ((AFE_CGR_GAIN1_Msk & ((value) << AFE_CGR_GAIN1_Pos)))
#define AFE_CGR_GAIN2_Pos 4
#define AFE_CGR_GAIN2_Msk (0x3u << AFE_CGR_GAIN2_Pos) /**< \brief (AFE_CGR) Gain for channel 2 */
#define AFE_CGR_GAIN2(value) ((AFE_CGR_GAIN2_Msk & ((value) << AFE_CGR_GAIN2_Pos)))
#define AFE_CGR_GAIN3_Pos 6
#define AFE_CGR_GAIN3_Msk (0x3u << AFE_CGR_GAIN3_Pos) /**< \brief (AFE_CGR) Gain for channel 3 */
#define AFE_CGR_GAIN3(value) ((AFE_CGR_GAIN3_Msk & ((value) << AFE_CGR_GAIN3_Pos)))
#define AFE_CGR_GAIN4_Pos 8
#define AFE_CGR_GAIN4_Msk (0x3u << AFE_CGR_GAIN4_Pos) /**< \brief (AFE_CGR) Gain for channel 4 */
#define AFE_CGR_GAIN4(value) ((AFE_CGR_GAIN4_Msk & ((value) << AFE_CGR_GAIN4_Pos)))
#define AFE_CGR_GAIN5_Pos 10
#define AFE_CGR_GAIN5_Msk (0x3u << AFE_CGR_GAIN5_Pos) /**< \brief (AFE_CGR) Gain for channel 5 */
#define AFE_CGR_GAIN5(value) ((AFE_CGR_GAIN5_Msk & ((value) << AFE_CGR_GAIN5_Pos)))
#define AFE_CGR_GAIN6_Pos 12
#define AFE_CGR_GAIN6_Msk (0x3u << AFE_CGR_GAIN6_Pos) /**< \brief (AFE_CGR) Gain for channel 6 */
#define AFE_CGR_GAIN6(value) ((AFE_CGR_GAIN6_Msk & ((value) << AFE_CGR_GAIN6_Pos)))
#define AFE_CGR_GAIN7_Pos 14
#define AFE_CGR_GAIN7_Msk (0x3u << AFE_CGR_GAIN7_Pos) /**< \brief (AFE_CGR) Gain for channel 7 */
#define AFE_CGR_GAIN7(value) ((AFE_CGR_GAIN7_Msk & ((value) << AFE_CGR_GAIN7_Pos)))
#define AFE_CGR_GAIN8_Pos 16
#define AFE_CGR_GAIN8_Msk (0x3u << AFE_CGR_GAIN8_Pos) /**< \brief (AFE_CGR) Gain for channel 8 */
#define AFE_CGR_GAIN8(value) ((AFE_CGR_GAIN8_Msk & ((value) << AFE_CGR_GAIN8_Pos)))
#define AFE_CGR_GAIN9_Pos 18
#define AFE_CGR_GAIN9_Msk (0x3u << AFE_CGR_GAIN9_Pos) /**< \brief (AFE_CGR) Gain for channel 9 */
#define AFE_CGR_GAIN9(value) ((AFE_CGR_GAIN9_Msk & ((value) << AFE_CGR_GAIN9_Pos)))
#define AFE_CGR_GAIN10_Pos 20
#define AFE_CGR_GAIN10_Msk (0x3u << AFE_CGR_GAIN10_Pos) /**< \brief (AFE_CGR) Gain for channel 10 */
#define AFE_CGR_GAIN10(value) ((AFE_CGR_GAIN10_Msk & ((value) << AFE_CGR_GAIN10_Pos)))
#define AFE_CGR_GAIN11_Pos 22
#define AFE_CGR_GAIN11_Msk (0x3u << AFE_CGR_GAIN11_Pos) /**< \brief (AFE_CGR) Gain for channel 11 */
#define AFE_CGR_GAIN11(value) ((AFE_CGR_GAIN11_Msk & ((value) << AFE_CGR_GAIN11_Pos)))
#define AFE_CGR_GAIN12_Pos 24
#define AFE_CGR_GAIN12_Msk (0x3u << AFE_CGR_GAIN12_Pos) /**< \brief (AFE_CGR) Gain for channel 12 */
#define AFE_CGR_GAIN12(value) ((AFE_CGR_GAIN12_Msk & ((value) << AFE_CGR_GAIN12_Pos)))
#define AFE_CGR_GAIN13_Pos 26
#define AFE_CGR_GAIN13_Msk (0x3u << AFE_CGR_GAIN13_Pos) /**< \brief (AFE_CGR) Gain for channel 13 */
#define AFE_CGR_GAIN13(value) ((AFE_CGR_GAIN13_Msk & ((value) << AFE_CGR_GAIN13_Pos)))
#define AFE_CGR_GAIN14_Pos 28
#define AFE_CGR_GAIN14_Msk (0x3u << AFE_CGR_GAIN14_Pos) /**< \brief (AFE_CGR) Gain for channel 14 */
#define AFE_CGR_GAIN14(value) ((AFE_CGR_GAIN14_Msk & ((value) << AFE_CGR_GAIN14_Pos)))
#define AFE_CGR_GAIN15_Pos 30
#define AFE_CGR_GAIN15_Msk (0x3u << AFE_CGR_GAIN15_Pos) /**< \brief (AFE_CGR) Gain for channel 15 */
#define AFE_CGR_GAIN15(value) ((AFE_CGR_GAIN15_Msk & ((value) << AFE_CGR_GAIN15_Pos)))
/* -------- AFE_CDOR : (AFEC Offset: 0x5C) Channel DC Offset Register -------- */
#define AFE_CDOR_OFF0 (0x1u << 0) /**< \brief (AFE_CDOR) Offset for channel 0 */
#define AFE_CDOR_OFF1 (0x1u << 1) /**< \brief (AFE_CDOR) Offset for channel 1 */
#define AFE_CDOR_OFF2 (0x1u << 2) /**< \brief (AFE_CDOR) Offset for channel 2 */
#define AFE_CDOR_OFF3 (0x1u << 3) /**< \brief (AFE_CDOR) Offset for channel 3 */
#define AFE_CDOR_OFF4 (0x1u << 4) /**< \brief (AFE_CDOR) Offset for channel 4 */
#define AFE_CDOR_OFF5 (0x1u << 5) /**< \brief (AFE_CDOR) Offset for channel 5 */
#define AFE_CDOR_OFF6 (0x1u << 6) /**< \brief (AFE_CDOR) Offset for channel 6 */
#define AFE_CDOR_OFF7 (0x1u << 7) /**< \brief (AFE_CDOR) Offset for channel 7 */
#define AFE_CDOR_OFF8 (0x1u << 8) /**< \brief (AFE_CDOR) Offset for channel 8 */
#define AFE_CDOR_OFF9 (0x1u << 9) /**< \brief (AFE_CDOR) Offset for channel 9 */
#define AFE_CDOR_OFF10 (0x1u << 10) /**< \brief (AFE_CDOR) Offset for channel 10 */
#define AFE_CDOR_OFF11 (0x1u << 11) /**< \brief (AFE_CDOR) Offset for channel 11 */
#define AFE_CDOR_OFF12 (0x1u << 12) /**< \brief (AFE_CDOR) Offset for channel 12 */
#define AFE_CDOR_OFF13 (0x1u << 13) /**< \brief (AFE_CDOR) Offset for channel 13 */
#define AFE_CDOR_OFF14 (0x1u << 14) /**< \brief (AFE_CDOR) Offset for channel 14 */
#define AFE_CDOR_OFF15 (0x1u << 15) /**< \brief (AFE_CDOR) Offset for channel 15 */
#define AFE_CDOR_OVRE16 (0x1u << 16) /**< \brief (AFE_CDOR)  */
#define AFE_CDOR_OVRE17 (0x1u << 17) /**< \brief (AFE_CDOR)  */
#define AFE_CDOR_OFF18 (0x1u << 18) /**< \brief (AFE_CDOR) Offset for channel 18 */
#define AFE_CDOR_OFF19 (0x1u << 19) /**< \brief (AFE_CDOR) Offset for channel 19 */
#define AFE_CDOR_OFF20 (0x1u << 20) /**< \brief (AFE_CDOR) Offset for channel 20 */
#define AFE_CDOR_OFF21 (0x1u << 21) /**< \brief (AFE_CDOR) Offset for channel 21 */
#define AFE_CDOR_OFF22 (0x1u << 22) /**< \brief (AFE_CDOR) Offset for channel 22 */
#define AFE_CDOR_OFF23 (0x1u << 23) /**< \brief (AFE_CDOR) Offset for channel 23 */
/* -------- AFE_DIFFR : (AFEC Offset: 0x60) Channel Differential Register -------- */
#define AFE_DIFFR_DIFF0 (0x1u << 0) /**< \brief (AFE_DIFFR) Differential inputs for channel 0 */
#define AFE_DIFFR_DIFF1 (0x1u << 1) /**< \brief (AFE_DIFFR) Differential inputs for channel 1 */
#define AFE_DIFFR_DIFF2 (0x1u << 2) /**< \brief (AFE_DIFFR) Differential inputs for channel 2 */
#define AFE_DIFFR_DIFF3 (0x1u << 3) /**< \brief (AFE_DIFFR) Differential inputs for channel 3 */
#define AFE_DIFFR_DIFF4 (0x1u << 4) /**< \brief (AFE_DIFFR) Differential inputs for channel 4 */
#define AFE_DIFFR_DIFF5 (0x1u << 5) /**< \brief (AFE_DIFFR) Differential inputs for channel 5 */
#define AFE_DIFFR_DIFF6 (0x1u << 6) /**< \brief (AFE_DIFFR) Differential inputs for channel 6 */
#define AFE_DIFFR_DIFF7 (0x1u << 7) /**< \brief (AFE_DIFFR) Differential inputs for channel 7 */
#define AFE_DIFFR_DIFF8 (0x1u << 8) /**< \brief (AFE_DIFFR) Differential inputs for channel 8 */
#define AFE_DIFFR_DIFF9 (0x1u << 9) /**< \brief (AFE_DIFFR) Differential inputs for channel 9 */
#define AFE_DIFFR_DIFF10 (0x1u << 10) /**< \brief (AFE_DIFFR) Differential inputs for channel 10 */
#define AFE_DIFFR_DIFF11 (0x1u << 11) /**< \brief (AFE_DIFFR) Differential inputs for channel 11 */
#define AFE_DIFFR_DIFF12 (0x1u << 12) /**< \brief (AFE_DIFFR) Differential inputs for channel 12 */
#define AFE_DIFFR_DIFF13 (0x1u << 13) /**< \brief (AFE_DIFFR) Differential inputs for channel 13 */
#define AFE_DIFFR_DIFF14 (0x1u << 14) /**< \brief (AFE_DIFFR) Differential inputs for channel 14 */
#define AFE_DIFFR_DIFF15 (0x1u << 15) /**< \brief (AFE_DIFFR) Differential inputs for channel 15 */
#define AFE_DIFFR_DIFF16 (0x1u << 16) /**< \brief (AFE_DIFFR) Differential inputs for channel 16 */
#define AFE_DIFFR_DIFF17 (0x1u << 17) /**< \brief (AFE_DIFFR) Differential inputs for channel 17 */
#define AFE_DIFFR_DIFF18 (0x1u << 18) /**< \brief (AFE_DIFFR) Differential inputs for channel 18 */
#define AFE_DIFFR_DIFF19 (0x1u << 19) /**< \brief (AFE_DIFFR) Differential inputs for channel 19 */
#define AFE_DIFFR_DIFF20 (0x1u << 20) /**< \brief (AFE_DIFFR) Differential inputs for channel 20 */
#define AFE_DIFFR_DIFF21 (0x1u << 21) /**< \brief (AFE_DIFFR) Differential inputs for channel 21 */
#define AFE_DIFFR_DIFF22 (0x1u << 22) /**< \brief (AFE_DIFFR) Differential inputs for channel 22 */
#define AFE_DIFFR_DIFF23 (0x1u << 23) /**< \brief (AFE_DIFFR) Differential inputs for channel 23 */
/* -------- AFE_CSELR : (AFEC Offset: 0x64) Channel Register Selection -------- */
#define AFE_CSELR_CSEL_Pos 0
#define AFE_CSELR_CSEL_Msk (0xfu << AFE_CSELR_CSEL_Pos) /**< \brief (AFE_CSELR) Channel Selection */
/* -------- AFE_CDR : (AFEC Offset: 0x68) Channel Data Register -------- */
#define AFE_CDR_DATA_Pos 0
#define AFE_CDR_DATA_Msk (0xfffu << AFE_CDR_DATA_Pos) /**< \brief (AFE_CDR) Converted Data */
/* -------- AFE_COCR : (AFEC Offset: 0x6C) Channel Offset Compensation Register -------- */
#define AFE_COCR_AOFF_Pos 0
#define AFE_COCR_AOFF_Msk (0xfffu << AFE_COCR_AOFF_Pos) /**< \brief (AFE_COCR) Analog Offset */
/* -------- AFE_TEMPMR : (AFEC Offset: 0x70) Temperature Sensor Mode Register -------- */
#define AFE_TEMPMR_RTCT (0x1u << 0) /**< \brief (AFE_TEMPMR) Temperature Sensor RTC Trigger mode */
#define AFE_TEMPMR_TEMPCMPMOD_Pos 4
#define AFE_TEMPMR_TEMPCMPMOD_Msk (0x3u << AFE_TEMPMR_TEMPCMPMOD_Pos) /**< \brief (AFE_TEMPMR) Temperature Comparison Mode */
#define   AFE_TEMPMR_TEMPCMPMOD_LOW (0x0u << 4) /**< \brief (AFE_TEMPMR) Generates an event when the converted data is lower than the low threshold of the window. */
#define   AFE_TEMPMR_TEMPCMPMOD_HIGH (0x1u << 4) /**< \brief (AFE_TEMPMR) Generates an event when the converted data is higher than the high threshold of the window. */
#define   AFE_TEMPMR_TEMPCMPMOD_IN (0x2u << 4) /**< \brief (AFE_TEMPMR) Generates an event when the converted data is in the comparison window. */
#define   AFE_TEMPMR_TEMPCMPMOD_OUT (0x3u << 4) /**< \brief (AFE_TEMPMR) Generates an event when the converted data is out of the comparison window. */
/* -------- AFE_TEMPCWR : (AFEC Offset: 0x74) Temperature Compare Window Register -------- */
#define AFE_TEMPCWR_TLOWTHRES_Pos 0
#define AFE_TEMPCWR_TLOWTHRES_Msk (0xffffu << AFE_TEMPCWR_TLOWTHRES_Pos) /**< \brief (AFE_TEMPCWR) Temperature Low Threshold */
#define AFE_TEMPCWR_TLOWTHRES(value) ((AFE_TEMPCWR_TLOWTHRES_Msk & ((value) << AFE_TEMPCWR_TLOWTHRES_Pos)))
#define AFE_TEMPCWR_THIGHTHRES_Pos 16
#define AFE_TEMPCWR_THIGHTHRES_Msk (0xffffu << AFE_TEMPCWR_THIGHTHRES_Pos) /**< \brief (AFE_TEMPCWR) Temperature High Threshold */
#define AFE_TEMPCWR_THIGHTHRES(value) ((AFE_TEMPCWR_THIGHTHRES_Msk & ((value) << AFE_TEMPCWR_THIGHTHRES_Pos)))
/* -------- AFE_ACR : (AFEC Offset: 0x94) Analog Control Register -------- */
#define AFE_ACR_IBCTL_Pos 8
#define AFE_ACR_IBCTL_Msk (0x3u << AFE_ACR_IBCTL_Pos) /**< \brief (AFE_ACR) AFE Bias Current Control */
#define AFE_ACR_IBCTL(value) ((AFE_ACR_IBCTL_Msk & ((value) << AFE_ACR_IBCTL_Pos)))
/* -------- AFE_WPMR : (AFEC Offset: 0xE4) Write Protect Mode Register -------- */
#define AFE_WPMR_WPEN (0x1u << 0) /**< \brief (AFE_WPMR) Write Protect Enable */
#define AFE_WPMR_WPKEY_Pos 8
#define AFE_WPMR_WPKEY_Msk (0xffffffu << AFE_WPMR_WPKEY_Pos) /**< \brief (AFE_WPMR) Write Protect KEY */
#define AFE_WPMR_WPKEY(value) ((AFE_WPMR_WPKEY_Msk & ((value) << AFE_WPMR_WPKEY_Pos)))
/* -------- AFE_WPSR : (AFEC Offset: 0xE8) Write Protect Status Register -------- */
#define AFE_WPSR_WPVS (0x1u << 0) /**< \brief (AFE_WPSR) Write Protect Violation Status */
#define AFE_WPSR_WPVSRC_Pos 8
#define AFE_WPSR_WPVSRC_Msk (0xffffu << AFE_WPSR_WPVSRC_Pos) /**< \brief (AFE_WPSR) Write Protect Violation Source */
/* -------- AFE_RPR : (AFEC Offset: 0x100) Receive Pointer Register -------- */
#define AFE_RPR_RXPTR_Pos 0
#define AFE_RPR_RXPTR_Msk (0xffffffffu << AFE_RPR_RXPTR_Pos) /**< \brief (AFE_RPR) Receive Pointer Register */
#define AFE_RPR_RXPTR(value) ((AFE_RPR_RXPTR_Msk & ((value) << AFE_RPR_RXPTR_Pos)))
/* -------- AFE_RCR : (AFEC Offset: 0x104) Receive Counter Register -------- */
#define AFE_RCR_RXCTR_Pos 0
#define AFE_RCR_RXCTR_Msk (0xffffu << AFE_RCR_RXCTR_Pos) /**< \brief (AFE_RCR) Receive Counter Register */
#define AFE_RCR_RXCTR(value) ((AFE_RCR_RXCTR_Msk & ((value) << AFE_RCR_RXCTR_Pos)))
/* -------- AFE_RNPR : (AFEC Offset: 0x110) Receive Next Pointer Register -------- */
#define AFE_RNPR_RXNPTR_Pos 0
#define AFE_RNPR_RXNPTR_Msk (0xffffffffu << AFE_RNPR_RXNPTR_Pos) /**< \brief (AFE_RNPR) Receive Next Pointer */
#define AFE_RNPR_RXNPTR(value) ((AFE_RNPR_RXNPTR_Msk & ((value) << AFE_RNPR_RXNPTR_Pos)))
/* -------- AFE_RNCR : (AFEC Offset: 0x114) Receive Next Counter Register -------- */
#define AFE_RNCR_RXNCTR_Pos 0
#define AFE_RNCR_RXNCTR_Msk (0xffffu << AFE_RNCR_RXNCTR_Pos) /**< \brief (AFE_RNCR) Receive Next Counter */
#define AFE_RNCR_RXNCTR(value) ((AFE_RNCR_RXNCTR_Msk & ((value) << AFE_RNCR_RXNCTR_Pos)))
/* -------- AFE_PTCR : (AFEC Offset: 0x120) Transfer Control Register -------- */
#define AFE_PTCR_RXTEN (0x1u << 0) /**< \brief (AFE_PTCR) Receiver Transfer Enable */
#define AFE_PTCR_RXTDIS (0x1u << 1) /**< \brief (AFE_PTCR) Receiver Transfer Disable */
#define AFE_PTCR_TXTEN (0x1u << 8) /**< \brief (AFE_PTCR) Transmitter Transfer Enable */
#define AFE_PTCR_TXTDIS (0x1u << 9) /**< \brief (AFE_PTCR) Transmitter Transfer Disable */
/* -------- AFE_PTSR : (AFEC Offset: 0x124) Transfer Status Register -------- */
#define AFE_PTSR_RXTEN (0x1u << 0) /**< \brief (AFE_PTSR) Receiver Transfer Enable */
#define AFE_PTSR_TXTEN (0x1u << 8) /**< \brief (AFE_PTSR) Transmitter Transfer Enable */

/*@}*/


#endif /* _SAM4E_AFEC_COMPONENT_ */
