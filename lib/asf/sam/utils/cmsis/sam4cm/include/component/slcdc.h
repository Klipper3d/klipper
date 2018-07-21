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

#ifndef _SAM4CM_SLCDC_COMPONENT_
#define _SAM4CM_SLCDC_COMPONENT_

/* ============================================================================= */
/**  SOFTWARE API DEFINITION FOR Segment LCD Controller */
/* ============================================================================= */
/** \addtogroup SAM4CM_SLCDC Segment LCD Controller */
/*@{*/

#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
/** \brief SlcdcCom hardware registers */
typedef struct {
  __IO uint32_t SLCDC_LMEMR; /**< \brief (SlcdcCom Offset: 0x0) SLCDC LSB Memory Register */
  __IO uint32_t SLCDC_MMEMR; /**< \brief (SlcdcCom Offset: 0x4) SLCDC MSB Memory Register */
} SlcdcCom;
/** \brief Slcdc hardware registers */
#define SLCDCCOM_NUMBER 6
typedef struct {
  __O  uint32_t SLCDC_CR;                   /**< \brief (Slcdc Offset: 0x0) SLCDC Control Register */
  __IO uint32_t SLCDC_MR;                   /**< \brief (Slcdc Offset: 0x4) SLCDC Mode Register */
  __IO uint32_t SLCDC_FRR;                  /**< \brief (Slcdc Offset: 0x8) SLCDC Frame Rate Register */
  __IO uint32_t SLCDC_DR;                   /**< \brief (Slcdc Offset: 0xC) SLCDC Display Register */
  __I  uint32_t SLCDC_SR;                   /**< \brief (Slcdc Offset: 0x10) SLCDC Status Register */
  __I  uint32_t Reserved1[3];
  __O  uint32_t SLCDC_IER;                  /**< \brief (Slcdc Offset: 0x20) SLCDC Interrupt Enable Register */
  __O  uint32_t SLCDC_IDR;                  /**< \brief (Slcdc Offset: 0x24) SLCDC Interrupt Disable Register */
  __O  uint32_t SLCDC_IMR;                  /**< \brief (Slcdc Offset: 0x28) SLCDC Interrupt Mask Register */
  __I  uint32_t SLCDC_ISR;                  /**< \brief (Slcdc Offset: 0x2C) SLCDC Interrupt Status Register */
  __IO uint32_t SLCDC_SMR0;                 /**< \brief (Slcdc Offset: 0x30) SLCDC Segment Map Register 0 */
  __IO uint32_t SLCDC_SMR1;                 /**< \brief (Slcdc Offset: 0x34) SLCDC Segment Map Register 1 */
  __I  uint32_t Reserved2[43];
  __IO uint32_t SLCDC_WPMR;                 /**< \brief (Slcdc Offset: 0xE4) SLCDC Write Protect Mode Register */
  __I  uint32_t SLCDC_WPSR;                 /**< \brief (Slcdc Offset: 0xE8) SLCDC Write Protect Status Register */
  __I  uint32_t Reserved3[69];
       SlcdcCom SLCDC_COM[SLCDCCOM_NUMBER]; /**< \brief (Slcdc Offset: 0x200) com = 0 .. 5 */
} Slcdc;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */
/* -------- SLCDC_CR : (SLCDC Offset: 0x0) SLCDC Control Register -------- */
#define SLCDC_CR_LCDEN (0x1u << 0) /**< \brief (SLCDC_CR) Enable the LCDC */
#define SLCDC_CR_LCDDIS (0x1u << 1) /**< \brief (SLCDC_CR) Disable LCDC */
#define SLCDC_CR_SWRST (0x1u << 3) /**< \brief (SLCDC_CR) Software Reset */
/* -------- SLCDC_MR : (SLCDC Offset: 0x4) SLCDC Mode Register -------- */
#define SLCDC_MR_COMSEL_Pos 0
#define SLCDC_MR_COMSEL_Msk (0x7u << SLCDC_MR_COMSEL_Pos) /**< \brief (SLCDC_MR) Selection of the Number of Commons */
#define   SLCDC_MR_COMSEL_COM_0 (0x0u << 0) /**< \brief (SLCDC_MR) COM0 is driven by SLCDC, COM1:5 are driven by digital function */
#define   SLCDC_MR_COMSEL_COM_0TO1 (0x1u << 0) /**< \brief (SLCDC_MR) COM0:1 are driven by SLCDC, COM2:5 are driven by digital function */
#define   SLCDC_MR_COMSEL_COM_0TO2 (0x2u << 0) /**< \brief (SLCDC_MR) COM0:2 are driven by SLCDC, COM3:5 are driven by digital function */
#define   SLCDC_MR_COMSEL_COM_0TO3 (0x3u << 0) /**< \brief (SLCDC_MR) COM0:3 are driven by SLCDC, COM4:5 are driven by digital function */
#define   SLCDC_MR_COMSEL_COM_0TO4 (0x4u << 0) /**< \brief (SLCDC_MR) COM0:4 are driven by SLCDC, COM5 is driven by digital function */
#define   SLCDC_MR_COMSEL_COM_0TO5 (0x5u << 0) /**< \brief (SLCDC_MR) COM0:5 are driven by SLCDC, No COM pin driven by digital function */
#define SLCDC_MR_SEGSEL_Pos 8
#define SLCDC_MR_SEGSEL_Msk (0x3fu << SLCDC_MR_SEGSEL_Pos) /**< \brief (SLCDC_MR) Selection of the Number of Segments */
#define SLCDC_MR_SEGSEL(value) ((SLCDC_MR_SEGSEL_Msk & ((value) << SLCDC_MR_SEGSEL_Pos)))
#define SLCDC_MR_BUFTIME_Pos 16
#define SLCDC_MR_BUFTIME_Msk (0xfu << SLCDC_MR_BUFTIME_Pos) /**< \brief (SLCDC_MR) Buffer On-Time */
#define   SLCDC_MR_BUFTIME_OFF (0x0u << 16) /**< \brief (SLCDC_MR) Nominal drive time is 0% of SCLK period */
#define   SLCDC_MR_BUFTIME_X2_SCLK_PERIOD (0x1u << 16) /**< \brief (SLCDC_MR) Nominal drive time is 2 periods of SCLK clock */
#define   SLCDC_MR_BUFTIME_X4_SCLK_PERIOD (0x2u << 16) /**< \brief (SLCDC_MR) Nominal drive time is 4 periods of SCLK clock */
#define   SLCDC_MR_BUFTIME_X8_SCLK_PERIOD (0x3u << 16) /**< \brief (SLCDC_MR) Nominal drive time is 8 periods of SCLK clock */
#define   SLCDC_MR_BUFTIME_X16_SCLK_PERIOD (0x4u << 16) /**< \brief (SLCDC_MR) Nominal drive time is 16 periods of SCLK clock */
#define   SLCDC_MR_BUFTIME_X32_SCLK_PERIOD (0x5u << 16) /**< \brief (SLCDC_MR) Nominal drive time is 32 periods of SCLK clock */
#define   SLCDC_MR_BUFTIME_X64_SCLK_PERIOD (0x6u << 16) /**< \brief (SLCDC_MR) Nominal drive time is 64 periods of SCLK clock */
#define   SLCDC_MR_BUFTIME_X128_SCLK_PERIOD (0x7u << 16) /**< \brief (SLCDC_MR) Nominal drive time is 128 periods of SCLK clock */
#define   SLCDC_MR_BUFTIME_PERCENT_50 (0x8u << 16) /**< \brief (SLCDC_MR) Nominal drive time is 50% of SCLK period */
#define   SLCDC_MR_BUFTIME_PERCENT_100 (0x9u << 16) /**< \brief (SLCDC_MR) Nominal drive time is 100% of SCLK period */
#define SLCDC_MR_BIAS_Pos 20
#define SLCDC_MR_BIAS_Msk (0x3u << SLCDC_MR_BIAS_Pos) /**< \brief (SLCDC_MR) LCD Display Configuration */
#define   SLCDC_MR_BIAS_STATIC (0x0u << 20) /**< \brief (SLCDC_MR) static */
#define   SLCDC_MR_BIAS_BIAS_1_2 (0x1u << 20) /**< \brief (SLCDC_MR) bias 1/2 */
#define   SLCDC_MR_BIAS_BIAS_1_3 (0x2u << 20) /**< \brief (SLCDC_MR) bias 1/3 */
#define SLCDC_MR_LPMODE (0x1u << 24) /**< \brief (SLCDC_MR) Low Power Mode (Taken into account from the next begin of frame.) */
/* -------- SLCDC_FRR : (SLCDC Offset: 0x8) SLCDC Frame Rate Register -------- */
#define SLCDC_FRR_PRESC_Pos 0
#define SLCDC_FRR_PRESC_Msk (0x7u << SLCDC_FRR_PRESC_Pos) /**< \brief (SLCDC_FRR) Clock Prescaler */
#define   SLCDC_FRR_PRESC_SCLK_DIV8 (0x0u << 0) /**< \brief (SLCDC_FRR) slow clock is divided by 8 */
#define   SLCDC_FRR_PRESC_SCLK_DIV16 (0x1u << 0) /**< \brief (SLCDC_FRR) slow clock is divided by 16 */
#define   SLCDC_FRR_PRESC_SCLK_DIV32 (0x2u << 0) /**< \brief (SLCDC_FRR) slow clock is divided by 32 */
#define   SLCDC_FRR_PRESC_SCLK_DIV64 (0x3u << 0) /**< \brief (SLCDC_FRR) slow clock is divided by 64 */
#define   SLCDC_FRR_PRESC_SCLK_DIV128 (0x4u << 0) /**< \brief (SLCDC_FRR) slow clock is divided by 128 */
#define   SLCDC_FRR_PRESC_SCLK_DIV256 (0x5u << 0) /**< \brief (SLCDC_FRR) slow clock is divided by 256 */
#define   SLCDC_FRR_PRESC_SCLK_DIV512 (0x6u << 0) /**< \brief (SLCDC_FRR) slow clock is divided by 512 */
#define   SLCDC_FRR_PRESC_SCLK_DIV1024 (0x7u << 0) /**< \brief (SLCDC_FRR) slow clock is divided by 1024 */
#define SLCDC_FRR_DIV_Pos 8
#define SLCDC_FRR_DIV_Msk (0x7u << SLCDC_FRR_DIV_Pos) /**< \brief (SLCDC_FRR) Clock Division */
#define   SLCDC_FRR_DIV_PRESC_CLK_DIV1 (0x0u << 8) /**< \brief (SLCDC_FRR) clock output from prescaler is divided by 1 */
#define   SLCDC_FRR_DIV_PRESC_CLK_DIV2 (0x1u << 8) /**< \brief (SLCDC_FRR) clock output from prescaler is divided by 2 */
#define   SLCDC_FRR_DIV_PRESC_CLK_DIV3 (0x2u << 8) /**< \brief (SLCDC_FRR) clock output from prescaler is divided by 3 */
#define   SLCDC_FRR_DIV_PRESC_CLK_DIV4 (0x3u << 8) /**< \brief (SLCDC_FRR) clock output from prescaler is divided by 4 */
#define   SLCDC_FRR_DIV_PRESC_CLK_DIV5 (0x4u << 8) /**< \brief (SLCDC_FRR) clock output from prescaler is divided by 5 */
#define   SLCDC_FRR_DIV_PRESC_CLK_DIV6 (0x5u << 8) /**< \brief (SLCDC_FRR) clock output from prescaler is divided by 6 */
#define   SLCDC_FRR_DIV_PRESC_CLK_DIV7 (0x6u << 8) /**< \brief (SLCDC_FRR) clock output from prescaler is divided by 7 */
#define   SLCDC_FRR_DIV_PRESC_CLK_DIV8 (0x7u << 8) /**< \brief (SLCDC_FRR) clock output from prescaler is divided by 8 */
/* -------- SLCDC_DR : (SLCDC Offset: 0xC) SLCDC Display Register -------- */
#define SLCDC_DR_DISPMODE_Pos 0
#define SLCDC_DR_DISPMODE_Msk (0x7u << SLCDC_DR_DISPMODE_Pos) /**< \brief (SLCDC_DR) Display Mode Register */
#define   SLCDC_DR_DISPMODE_NORMAL (0x0u << 0) /**< \brief (SLCDC_DR) Normal Mode:  Latched data are displayed. */
#define   SLCDC_DR_DISPMODE_FORCE_OFF (0x1u << 0) /**< \brief (SLCDC_DR) Force Off Mode:  All pixels are invisible. (The SLCDC memory is unchanged.) */
#define   SLCDC_DR_DISPMODE_FORCE_ON (0x2u << 0) /**< \brief (SLCDC_DR) Force On Mode All pixels are visible. (The SLCDC memory is unchanged.) */
#define   SLCDC_DR_DISPMODE_BLINKING (0x3u << 0) /**< \brief (SLCDC_DR) Blinking Mode: All pixels are alternately turned off to the predefined state in SLCDC memory at LCDBLKFREQ frequency. (The SLCDC memory is unchanged.) */
#define   SLCDC_DR_DISPMODE_INVERTED (0x4u << 0) /**< \brief (SLCDC_DR) Inverted Mode: All pixels are set in the inverted state as defined in SLCDC memory. (The SLCDC memory is unchanged.) */
#define   SLCDC_DR_DISPMODE_INVERTED_BLINK (0x5u << 0) /**< \brief (SLCDC_DR) Inverted Blinking Mode: All pixels are alternately turned off to the predefined opposite state in SLCDC memory at LCDBLKFREQ frequency. (The SLCDC memory is unchanged.) */
#define   SLCDC_DR_DISPMODE_USER_BUFFER_LOAD (0x6u << 0) /**< \brief (SLCDC_DR) User Buffer Only Load Mode: Blocks the automatic transfer from User Buffer to Display Buffer. */
#define   SLCDC_DR_DISPMODE_BUFFERS_SWAP (0x7u << 0) /**< \brief (SLCDC_DR) Buffer Swap Mode: All pixels are alternatively assigned to the state defined in the User Buffer, then to the state defined in the Display Buffer at LCDBLKFREQ frequency. */
#define SLCDC_DR_LCDBLKFREQ_Pos 8
#define SLCDC_DR_LCDBLKFREQ_Msk (0xffu << SLCDC_DR_LCDBLKFREQ_Pos) /**< \brief (SLCDC_DR) LCD Blinking Frequency Selection */
#define SLCDC_DR_LCDBLKFREQ(value) ((SLCDC_DR_LCDBLKFREQ_Msk & ((value) << SLCDC_DR_LCDBLKFREQ_Pos)))
/* -------- SLCDC_SR : (SLCDC Offset: 0x10) SLCDC Status Register -------- */
#define SLCDC_SR_ENA (0x1u << 0) /**< \brief (SLCDC_SR) Enable Status (Automatically Set/Reset) */
/* -------- SLCDC_IER : (SLCDC Offset: 0x20) SLCDC Interrupt Enable Register -------- */
#define SLCDC_IER_ENDFRAME (0x1u << 0) /**< \brief (SLCDC_IER) End of Frame Interrupt Enable */
#define SLCDC_IER_DIS (0x1u << 2) /**< \brief (SLCDC_IER) Disable Interrupt Enable */
/* -------- SLCDC_IDR : (SLCDC Offset: 0x24) SLCDC Interrupt Disable Register -------- */
#define SLCDC_IDR_ENDFRAME (0x1u << 0) /**< \brief (SLCDC_IDR) End of Frame Interrupt Disable */
#define SLCDC_IDR_DIS (0x1u << 2) /**< \brief (SLCDC_IDR) Disable Interrupt Disable */
/* -------- SLCDC_IMR : (SLCDC Offset: 0x28) SLCDC Interrupt Mask Register -------- */
#define SLCDC_IMR_ENDFRAME (0x1u << 0) /**< \brief (SLCDC_IMR) End of Frame Interrupt Mask */
#define SLCDC_IMR_DIS (0x1u << 2) /**< \brief (SLCDC_IMR) Disable Interrupt Mask */
/* -------- SLCDC_ISR : (SLCDC Offset: 0x2C) SLCDC Interrupt Status Register -------- */
#define SLCDC_ISR_ENDFRAME (0x1u << 0) /**< \brief (SLCDC_ISR) End of Frame Interrupt Mask */
#define SLCDC_ISR_DIS (0x1u << 2) /**< \brief (SLCDC_ISR) Disable Interrupt Mask */
/* -------- SLCDC_SMR0 : (SLCDC Offset: 0x30) SLCDC Segment Map Register 0 -------- */
#define SLCDC_SMR0_LCD0 (0x1u << 0) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O pin */
#define SLCDC_SMR0_LCD1 (0x1u << 1) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O pin */
#define SLCDC_SMR0_LCD2 (0x1u << 2) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O pin */
#define SLCDC_SMR0_LCD3 (0x1u << 3) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O pin */
#define SLCDC_SMR0_LCD4 (0x1u << 4) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O pin */
#define SLCDC_SMR0_LCD5 (0x1u << 5) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O pin */
#define SLCDC_SMR0_LCD6 (0x1u << 6) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O pin */
#define SLCDC_SMR0_LCD7 (0x1u << 7) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O pin */
#define SLCDC_SMR0_LCD8 (0x1u << 8) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O pin */
#define SLCDC_SMR0_LCD9 (0x1u << 9) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O pin */
#define SLCDC_SMR0_LCD10 (0x1u << 10) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O pin */
#define SLCDC_SMR0_LCD11 (0x1u << 11) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O pin */
#define SLCDC_SMR0_LCD12 (0x1u << 12) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O pin */
#define SLCDC_SMR0_LCD13 (0x1u << 13) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O pin */
#define SLCDC_SMR0_LCD14 (0x1u << 14) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O pin */
#define SLCDC_SMR0_LCD15 (0x1u << 15) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O pin */
#define SLCDC_SMR0_LCD16 (0x1u << 16) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O pin */
#define SLCDC_SMR0_LCD17 (0x1u << 17) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O pin */
#define SLCDC_SMR0_LCD18 (0x1u << 18) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O pin */
#define SLCDC_SMR0_LCD19 (0x1u << 19) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O pin */
#define SLCDC_SMR0_LCD20 (0x1u << 20) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O pin */
#define SLCDC_SMR0_LCD21 (0x1u << 21) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O pin */
#define SLCDC_SMR0_LCD22 (0x1u << 22) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O pin */
#define SLCDC_SMR0_LCD23 (0x1u << 23) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O pin */
#define SLCDC_SMR0_LCD24 (0x1u << 24) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O pin */
#define SLCDC_SMR0_LCD25 (0x1u << 25) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O pin */
#define SLCDC_SMR0_LCD26 (0x1u << 26) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O pin */
#define SLCDC_SMR0_LCD27 (0x1u << 27) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O pin */
#define SLCDC_SMR0_LCD28 (0x1u << 28) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O pin */
#define SLCDC_SMR0_LCD29 (0x1u << 29) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O pin */
#define SLCDC_SMR0_LCD30 (0x1u << 30) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O pin */
#define SLCDC_SMR0_LCD31 (0x1u << 31) /**< \brief (SLCDC_SMR0) LCD Segment Mapped on SEGx I/O pin */
/* -------- SLCDC_SMR1 : (SLCDC Offset: 0x34) SLCDC Segment Map Register 1 -------- */
#define SLCDC_SMR1_LCD32 (0x1u << 0) /**< \brief (SLCDC_SMR1) LCD Segment Mapped on SEGx I/O pin */
#define SLCDC_SMR1_LCD33 (0x1u << 1) /**< \brief (SLCDC_SMR1) LCD Segment Mapped on SEGx I/O pin */
#define SLCDC_SMR1_LCD34 (0x1u << 2) /**< \brief (SLCDC_SMR1) LCD Segment Mapped on SEGx I/O pin */
#define SLCDC_SMR1_LCD35 (0x1u << 3) /**< \brief (SLCDC_SMR1) LCD Segment Mapped on SEGx I/O pin */
#define SLCDC_SMR1_LCD36 (0x1u << 4) /**< \brief (SLCDC_SMR1) LCD Segment Mapped on SEGx I/O pin */
#define SLCDC_SMR1_LCD37 (0x1u << 5) /**< \brief (SLCDC_SMR1) LCD Segment Mapped on SEGx I/O pin */
#define SLCDC_SMR1_LCD38 (0x1u << 6) /**< \brief (SLCDC_SMR1) LCD Segment Mapped on SEGx I/O pin */
#define SLCDC_SMR1_LCD39 (0x1u << 7) /**< \brief (SLCDC_SMR1) LCD Segment Mapped on SEGx I/O pin */
#define SLCDC_SMR1_LCD40 (0x1u << 8) /**< \brief (SLCDC_SMR1) LCD Segment Mapped on SEGx I/O pin */
#define SLCDC_SMR1_LCD41 (0x1u << 9) /**< \brief (SLCDC_SMR1) LCD Segment Mapped on SEGx I/O pin */
#define SLCDC_SMR1_LCD42 (0x1u << 10) /**< \brief (SLCDC_SMR1) LCD Segment Mapped on SEGx I/O pin */
#define SLCDC_SMR1_LCD43 (0x1u << 11) /**< \brief (SLCDC_SMR1) LCD Segment Mapped on SEGx I/O pin */
#define SLCDC_SMR1_LCD44 (0x1u << 12) /**< \brief (SLCDC_SMR1) LCD Segment Mapped on SEGx I/O pin */
#define SLCDC_SMR1_LCD45 (0x1u << 13) /**< \brief (SLCDC_SMR1) LCD Segment Mapped on SEGx I/O pin */
#define SLCDC_SMR1_LCD46 (0x1u << 14) /**< \brief (SLCDC_SMR1) LCD Segment Mapped on SEGx I/O pin */
#define SLCDC_SMR1_LCD47 (0x1u << 15) /**< \brief (SLCDC_SMR1) LCD Segment Mapped on SEGx I/O pin */
#define SLCDC_SMR1_LCD48 (0x1u << 16) /**< \brief (SLCDC_SMR1) LCD Segment Mapped on SEGx I/O pin */
#define SLCDC_SMR1_LCD49 (0x1u << 17) /**< \brief (SLCDC_SMR1) LCD Segment Mapped on SEGx I/O pin */
/* -------- SLCDC_WPMR : (SLCDC Offset: 0xE4) SLCDC Write Protect Mode Register -------- */
#define SLCDC_WPMR_WPEN (0x1u << 0) /**< \brief (SLCDC_WPMR) Write Protect Enable */
#define SLCDC_WPMR_WPKEY_Pos 8
#define SLCDC_WPMR_WPKEY_Msk (0xffffffu << SLCDC_WPMR_WPKEY_Pos) /**< \brief (SLCDC_WPMR) Write Protect KEY */
#define SLCDC_WPMR_WPKEY(value) ((SLCDC_WPMR_WPKEY_Msk & ((value) << SLCDC_WPMR_WPKEY_Pos)))
/* -------- SLCDC_WPSR : (SLCDC Offset: 0xE8) SLCDC Write Protect Status Register -------- */
#define SLCDC_WPSR_WPVS (0x1u << 0) /**< \brief (SLCDC_WPSR) Write Protect Violation Status */
#define SLCDC_WPSR_WPVSRC_Pos 8
#define SLCDC_WPSR_WPVSRC_Msk (0xffffu << SLCDC_WPSR_WPVSRC_Pos) /**< \brief (SLCDC_WPSR) Write Protect Violation Source */
/* -------- SLCDC_LMEMR : (SLCDC Offset: N/A) SLCDC LSB Memory Register -------- */
#define SLCDC_LMEMR_LPIXEL_Pos 0
#define SLCDC_LMEMR_LPIXEL_Msk (0xffffffffu << SLCDC_LMEMR_LPIXEL_Pos) /**< \brief (SLCDC_LMEMR) LSB Pixels pattern associated to COMx terminal */
#define SLCDC_LMEMR_LPIXEL(value) ((SLCDC_LMEMR_LPIXEL_Msk & ((value) << SLCDC_LMEMR_LPIXEL_Pos)))
/* -------- SLCDC_MMEMR : (SLCDC Offset: N/A) SLCDC MSB Memory Register -------- */
#define SLCDC_MMEMR_MPIXEL_Pos 0
#define SLCDC_MMEMR_MPIXEL_Msk (0xffffffffu << SLCDC_MMEMR_MPIXEL_Pos) /**< \brief (SLCDC_MMEMR) MSB Pixels pattern associated to COMx terminal */
#define SLCDC_MMEMR_MPIXEL(value) ((SLCDC_MMEMR_MPIXEL_Msk & ((value) << SLCDC_MMEMR_MPIXEL_Pos)))

/*@}*/


#endif /* _SAM4CM_SLCDC_COMPONENT_ */
