/*****************************************************************************
 * Copyright (c) 2019, Nations Technologies Inc.
 *
 * All rights reserved.
 * ****************************************************************************
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Nations' name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY NATIONS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL NATIONS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ****************************************************************************/

#ifndef ____N32G45x_ADC____
#define ____N32G45x_ADC____

#define __IO volatile

typedef struct
{
  uint32_t WorkMode;
  uint32_t MultiChEn;
  uint32_t ContinueConvEn;
  uint32_t ExtTrigSelect;
  uint32_t DatAlign;
  uint8_t ChsNumber;
} ADC_InitType;

typedef struct
{
  __IO uint32_t STS;
  __IO uint32_t CTRL1;
  __IO uint32_t CTRL2;
  __IO uint32_t SAMPT1;
  __IO uint32_t SAMPT2;
  __IO uint32_t JOFFSET1;
  __IO uint32_t JOFFSET2;
  __IO uint32_t JOFFSET3;
  __IO uint32_t JOFFSET4;
  __IO uint32_t WDGHIGH;
  __IO uint32_t WDGLOW;
  __IO uint32_t RSEQ1;
  __IO uint32_t RSEQ2;
  __IO uint32_t RSEQ3;
  __IO uint32_t JSEQ;
  __IO uint32_t JDAT1;
  __IO uint32_t JDAT2;
  __IO uint32_t JDAT3;
  __IO uint32_t JDAT4;
  __IO uint32_t DAT;
  __IO uint32_t DIFSEL;
  __IO uint32_t CALFACT;
  __IO uint32_t CTRL3;
  __IO uint32_t SAMPT3;
} ADC_Module;

#define NS_ADC1_BASE ((uint32_t)0x40020800)
#define NS_ADC2_BASE ((uint32_t)0x40020c00)
#define NS_ADC3_BASE ((uint32_t)0x40021800)
#define NS_ADC4_BASE ((uint32_t)0x40021c00)

#define NS_ADC1 ((ADC_Module *)NS_ADC1_BASE)
#define NS_ADC2 ((ADC_Module *)NS_ADC2_BASE)
#define NS_ADC3 ((ADC_Module *)NS_ADC3_BASE)
#define NS_ADC4 ((ADC_Module *)NS_ADC4_BASE)

#define ADC_RCC_BASE ((uint32_t)0x40021000)
#define ADC_RCC_CTRL *((uint32_t *)(ADC_RCC_BASE + 0x00))
#define ADC_RCC_CFG *((uint32_t *)(ADC_RCC_BASE + 0x04))
#define ADC_RCC_CLKINT *((uint32_t *)(ADC_RCC_BASE + 0x08))
#define ADC_RCC_APB2PRST *((uint32_t *)(ADC_RCC_BASE + 0x0c))
#define ADC_RCC_APB1PRST *((uint32_t *)(ADC_RCC_BASE + 0x10))
#define ADC_RCC_AHBPCLKEN *((uint32_t *)(ADC_RCC_BASE + 0x14))
#define ADC_RCC_APB2PCLKEN *((uint32_t *)(ADC_RCC_BASE + 0x18))
#define ADC_RCC_APB1PCLKEN *((uint32_t *)(ADC_RCC_BASE + 0x1c))
#define ADC_RCC_BDCTRL *((uint32_t *)(ADC_RCC_BASE + 0x20))
#define ADC_RCC_CTRLSTS *((uint32_t *)(ADC_RCC_BASE + 0x24))
#define ADC_RCC_AHBPRST *((uint32_t *)(ADC_RCC_BASE + 0x28))
#define ADC_RCC_CFG2 *((uint32_t *)(ADC_RCC_BASE + 0x2c))
#define ADC_RCC_CFG3 *((uint32_t *)(ADC_RCC_BASE + 0x30))

/* CFG2 register bit mask */
#define CFG2_TIM18CLKSEL_SET_MASK ((uint32_t)0x20000000)
#define CFG2_TIM18CLKSEL_RESET_MASK ((uint32_t)0xDFFFFFFF)
#define CFG2_RNGCPRES_SET_MASK ((uint32_t)0x1F000000)
#define CFG2_RNGCPRES_RESET_MASK ((uint32_t)0xE0FFFFFF)
#define CFG2_ADC1MSEL_SET_MASK ((uint32_t)0x00000400)
#define CFG2_ADC1MSEL_RESET_MASK ((uint32_t)0xFFFFFBFF)
#define CFG2_ADC1MPRES_SET_MASK ((uint32_t)0x0000F800)
#define CFG2_ADC1MPRES_RESET_MASK ((uint32_t)0xFFFF07FF)
#define CFG2_ADCPLLPRES_SET_MASK ((uint32_t)0x000001F0)
#define CFG2_ADCPLLPRES_RESET_MASK ((uint32_t)0xFFFFFE0F)
#define CFG2_ADCHPRES_SET_MASK ((uint32_t)0x0000000F)
#define CFG2_ADCHPRES_RESET_MASK ((uint32_t)0xFFFFFFF0)

#define RCC_ADCPLLCLK_DISABLE ((uint32_t)0xFFFFFEFF)
#define RCC_ADCPLLCLK_DIV1 ((uint32_t)0x00000100)
#define RCC_ADCPLLCLK_DIV2 ((uint32_t)0x00000110)
#define RCC_ADCPLLCLK_DIV4 ((uint32_t)0x00000120)
#define RCC_ADCPLLCLK_DIV6 ((uint32_t)0x00000130)
#define RCC_ADCPLLCLK_DIV8 ((uint32_t)0x00000140)
#define RCC_ADCPLLCLK_DIV10 ((uint32_t)0x00000150)
#define RCC_ADCPLLCLK_DIV12 ((uint32_t)0x00000160)
#define RCC_ADCPLLCLK_DIV16 ((uint32_t)0x00000170)
#define RCC_ADCPLLCLK_DIV32 ((uint32_t)0x00000180)
#define RCC_ADCPLLCLK_DIV64 ((uint32_t)0x00000190)
#define RCC_ADCPLLCLK_DIV128 ((uint32_t)0x000001A0)
#define RCC_ADCPLLCLK_DIV256 ((uint32_t)0x000001B0)
#define RCC_ADCPLLCLK_DIV_OTHERS ((uint32_t)0x000001C0)

#define RCC_ADCHCLK_DIV1 ((uint32_t)0x00000000)
#define RCC_ADCHCLK_DIV2 ((uint32_t)0x00000001)
#define RCC_ADCHCLK_DIV4 ((uint32_t)0x00000002)
#define RCC_ADCHCLK_DIV6 ((uint32_t)0x00000003)
#define RCC_ADCHCLK_DIV8 ((uint32_t)0x00000004)
#define RCC_ADCHCLK_DIV10 ((uint32_t)0x00000005)
#define RCC_ADCHCLK_DIV12 ((uint32_t)0x00000006)
#define RCC_ADCHCLK_DIV16 ((uint32_t)0x00000007)
#define RCC_ADCHCLK_DIV32 ((uint32_t)0x00000008)
#define RCC_ADCHCLK_DIV_OTHERS ((uint32_t)0x00000008)

#define RCC_ADC1MCLK_SRC_HSI ((uint32_t)0x00000000)
#define RCC_ADC1MCLK_SRC_HSE ((uint32_t)0x00000400)

#define RCC_ADC1MCLK_DIV1  ((uint32_t)0x00000000)
#define RCC_ADC1MCLK_DIV2  ((uint32_t)0x00000800)
#define RCC_ADC1MCLK_DIV3  ((uint32_t)0x00001000)
#define RCC_ADC1MCLK_DIV4  ((uint32_t)0x00001800)
#define RCC_ADC1MCLK_DIV5  ((uint32_t)0x00002000)
#define RCC_ADC1MCLK_DIV6  ((uint32_t)0x00002800)
#define RCC_ADC1MCLK_DIV7  ((uint32_t)0x00003000)
#define RCC_ADC1MCLK_DIV8  ((uint32_t)0x00003800)
#define RCC_ADC1MCLK_DIV9  ((uint32_t)0x00004000)
#define RCC_ADC1MCLK_DIV10 ((uint32_t)0x00004800)
#define RCC_ADC1MCLK_DIV11 ((uint32_t)0x00005000)
#define RCC_ADC1MCLK_DIV12 ((uint32_t)0x00005800)
#define RCC_ADC1MCLK_DIV13 ((uint32_t)0x00006000)
#define RCC_ADC1MCLK_DIV14 ((uint32_t)0x00006800)
#define RCC_ADC1MCLK_DIV15 ((uint32_t)0x00007000)
#define RCC_ADC1MCLK_DIV16 ((uint32_t)0x00007800)
#define RCC_ADC1MCLK_DIV17 ((uint32_t)0x00008000)
#define RCC_ADC1MCLK_DIV18 ((uint32_t)0x00008800)
#define RCC_ADC1MCLK_DIV19 ((uint32_t)0x00009000)
#define RCC_ADC1MCLK_DIV20 ((uint32_t)0x00009800)
#define RCC_ADC1MCLK_DIV21 ((uint32_t)0x0000A000)
#define RCC_ADC1MCLK_DIV22 ((uint32_t)0x0000A800)
#define RCC_ADC1MCLK_DIV23 ((uint32_t)0x0000B000)
#define RCC_ADC1MCLK_DIV24 ((uint32_t)0x0000B800)
#define RCC_ADC1MCLK_DIV25 ((uint32_t)0x0000C000)
#define RCC_ADC1MCLK_DIV26 ((uint32_t)0x0000C800)
#define RCC_ADC1MCLK_DIV27 ((uint32_t)0x0000D000)
#define RCC_ADC1MCLK_DIV28 ((uint32_t)0x0000D800)
#define RCC_ADC1MCLK_DIV29 ((uint32_t)0x0000E000)
#define RCC_ADC1MCLK_DIV30 ((uint32_t)0x0000E800)
#define RCC_ADC1MCLK_DIV31 ((uint32_t)0x0000F000)
#define RCC_ADC1MCLK_DIV32 ((uint32_t)0x0000F800)

#define RCC_AHB_PERIPH_ADC1 ((uint32_t)0x00001000)
#define RCC_AHB_PERIPH_ADC2 ((uint32_t)0x00002000)
#define RCC_AHB_PERIPH_ADC3 ((uint32_t)0x00004000)
#define RCC_AHB_PERIPH_ADC4 ((uint32_t)0x00008000)

#define SAMPT1_SMP_SET ((uint32_t)0x00000007)
#define SAMPT2_SMP_SET ((uint32_t)0x00000007)

#define SQR4_SEQ_SET ((uint32_t)0x0000001F)
#define SQR3_SEQ_SET ((uint32_t)0x0000001F)
#define SQR2_SEQ_SET ((uint32_t)0x0000001F)
#define SQR1_SEQ_SET ((uint32_t)0x0000001F)

#define CTRL1_CLR_MASK ((uint32_t)0xFFF0FEFF)
#define RSEQ1_CLR_MASK ((uint32_t)0xFF0FFFFF)
#define CTRL2_CLR_MASK ((uint32_t)0xFFF1F7FD)

#define ADC_CH_0 ((uint8_t)0x00)
#define ADC_CH_1 ((uint8_t)0x01)
#define ADC_CH_2 ((uint8_t)0x02)
#define ADC_CH_3 ((uint8_t)0x03)
#define ADC_CH_4 ((uint8_t)0x04)
#define ADC_CH_5 ((uint8_t)0x05)
#define ADC_CH_6 ((uint8_t)0x06)
#define ADC_CH_7 ((uint8_t)0x07)
#define ADC_CH_8 ((uint8_t)0x08)
#define ADC_CH_9 ((uint8_t)0x09)
#define ADC_CH_10 ((uint8_t)0x0A)
#define ADC_CH_11 ((uint8_t)0x0B)
#define ADC_CH_12 ((uint8_t)0x0C)
#define ADC_CH_13 ((uint8_t)0x0D)
#define ADC_CH_14 ((uint8_t)0x0E)
#define ADC_CH_15 ((uint8_t)0x0F)
#define ADC_CH_16 ((uint8_t)0x10)
#define ADC_CH_17 ((uint8_t)0x11)
#define ADC_CH_18 ((uint8_t)0x12)

#define ADC_WORKMODE_INDEPENDENT ((uint32_t)0x00000000)
#define ADC_WORKMODE_REG_INJECT_SIMULT ((uint32_t)0x00010000)
#define ADC_WORKMODE_REG_SIMULT_ALTER_TRIG ((uint32_t)0x00020000)
#define ADC_WORKMODE_INJ_SIMULT_FAST_INTERL ((uint32_t)0x00030000)
#define ADC_WORKMODE_INJ_SIMULT_SLOW_INTERL ((uint32_t)0x00040000)
#define ADC_WORKMODE_INJ_SIMULT ((uint32_t)0x00050000)
#define ADC_WORKMODE_REG_SIMULT ((uint32_t)0x00060000)
#define ADC_WORKMODE_FAST_INTERL ((uint32_t)0x00070000)
#define ADC_WORKMODE_SLOW_INTERL ((uint32_t)0x00080000)
#define ADC_WORKMODE_ALTER_TRIG ((uint32_t)0x00090000)

#define ADC_EXT_TRIGCONV_T1_CC1                                               \
  ((uint32_t)0x00000000) /*!< For ADC1 and ADC2 */
#define ADC_EXT_TRIGCONV_T1_CC2                                               \
  ((uint32_t)0x00020000) /*!< For ADC1 and ADC2 */
#define ADC_EXT_TRIGCONV_T2_CC2                                               \
  ((uint32_t)0x00060000) /*!< For ADC1 and ADC2 */
#define ADC_EXT_TRIGCONV_T1_CC3                                               \
  ((uint32_t)0x00040000) /*!< For ADC1, ADC2 , ADC3 and ADC4 */
#define ADC_EXT_TRIGCONV_NONE                                                 \
  ((uint32_t)0x000E0000) /*!< For ADC1, ADC2 , ADC3 and ADC4 */

#define ADC_DAT_ALIGN_R ((uint32_t)0x00000000)
#define ADC_DAT_ALIGN_L ((uint32_t)0x00000800)

#define ADC_FLAG_RDY ((uint8_t)0x20)
#define ADC_FLAG_PD_RDY ((uint8_t)0x40)

#define CTRL2_AD_ON_SET ((uint32_t)0x00000001)
#define CTRL2_AD_ON_RESET ((uint32_t)0xFFFFFFFE)

#define CTRL2_CAL_SET ((uint32_t)0x00000004)

/* ADC Software start mask */
#define CTRL2_EXT_TRIG_SWSTART_SET ((uint32_t)0x00500000)
#define CTRL2_EXT_TRIG_SWSTART_RESET ((uint32_t)0xFFAFFFFF)

#define ADC_SAMP_TIME_1CYCLES5 ((uint8_t)0x00)
#define ADC_SAMP_TIME_7CYCLES5 ((uint8_t)0x01)
#define ADC_SAMP_TIME_13CYCLES5 ((uint8_t)0x02)
#define ADC_SAMP_TIME_28CYCLES5 ((uint8_t)0x03)
#define ADC_SAMP_TIME_41CYCLES5 ((uint8_t)0x04)
#define ADC_SAMP_TIME_55CYCLES5 ((uint8_t)0x05)
#define ADC_SAMP_TIME_71CYCLES5 ((uint8_t)0x06)
#define ADC_SAMP_TIME_239CYCLES5 ((uint8_t)0x07)

#define ADC_FLAG_AWDG ((uint8_t)0x01)
#define ADC_FLAG_ENDC ((uint8_t)0x02)
#define ADC_FLAG_JENDC ((uint8_t)0x04)
#define ADC_FLAG_JSTR ((uint8_t)0x08)
#define ADC_FLAG_STR ((uint8_t)0x10)
#define ADC_FLAG_EOC_ANY ((uint8_t)0x20)
#define ADC_FLAG_JEOC_ANY ((uint8_t)0x40)

#define ADC_STS_AWDG ((uint8_t)0x01) /*!< Analog watchdog flag */
#define ADC_STS_ENDC ((uint8_t)0x02) /*!< End of conversion */
#define ADC_STS_JENDC                                                         \
  ((uint8_t)0x04)                    /*!< Injected channel end of conversion */
#define ADC_STS_JSTR ((uint8_t)0x08) /*!< Injected channel Start flag */
#define ADC_STS_STR ((uint8_t)0x10)  /*!< Regular channel Start flag */
#define ADC_STS_ENDCA ((uint8_t)0x20) /*!< Any end of conversion */
#define ADC_STS_JENDCA                                                        \
  ((uint8_t)0x40) /*!< Any injected channel end of conversion */

/* ADC DMA mask */
#define CTRL2_DMA_SET ((uint32_t)0x00000100)
#define CTRL2_DMA_RESET ((uint32_t)0xFFFFFEFF)

#define CTRL2_TSVREFE_SET ((uint32_t)0x00800000)
#define CTRL2_TSVREFE_RESET ((uint32_t)0xFF7FFFFF)
#define VREF1P2_CTRL (*(uint32_t *)(0x40001800 + 0x20))
/*******************  Bit definition for ADC_CTRL2 register
 * ********************/
#define ADC_CTRL2_ON ((uint32_t)0x00000001)    /*!< A/D Converter ON / OFF */
#define ADC_CTRL2_CTU ((uint32_t)0x00000002)   /*!< Continuous Conversion */
#define ADC_CTRL2_ENCAL ((uint32_t)0x00000004) /*!< A/D Calibration */
#define ADC_CTRL2_ENDMA                                                       \
  ((uint32_t)0x00000100)                      /*!< Direct Memory access mode */
#define ADC_CTRL2_ALIG ((uint32_t)0x00000800) /*!< Data Alignment */

#define ADC_CTRL2_EXTJSEL                                                     \
  ((uint32_t)0x00007000) /*!< INJ_EXT_SEL[2:0] bits (External event select    \
                            for injected group) */
#define ADC_CTRL2_EXTJSEL_0 ((uint32_t)0x00001000) /*!< Bit 0 */
#define ADC_CTRL2_EXTJSEL_1 ((uint32_t)0x00002000) /*!< Bit 1 */
#define ADC_CTRL2_EXTJSEL_2 ((uint32_t)0x00004000) /*!< Bit 2 */

#define ADC_CTRL2_EXTJTRIG                                                    \
  ((uint32_t)0x00008000) /*!< External Trigger Conversion mode for injected   \
                            channels */

#define ADC_CTRL2_EXTRSEL                                                     \
  ((uint32_t)0x000E0000) /*!< EXTSEL[2:0] bits (External Event Select for     \
                            regular group) */
#define ADC_CTRL2_EXTRSEL_0 ((uint32_t)0x00020000) /*!< Bit 0 */
#define ADC_CTRL2_EXTRSEL_1 ((uint32_t)0x00040000) /*!< Bit 1 */
#define ADC_CTRL2_EXTRSEL_2 ((uint32_t)0x00080000) /*!< Bit 2 */

#define ADC_CTRL2_EXTRTRIG                                                    \
  ((uint32_t)0x00100000) /*!< External Trigger Conversion mode for regular    \
                            channels */
#define ADC_CTRL2_SWSTRJCH                                                    \
  ((uint32_t)0x00200000) /*!< Start Conversion of injected channels */
#define ADC_CTRL2_SWSTRRCH                                                    \
  ((uint32_t)0x00400000) /*!< Start Conversion of regular channels */
#define ADC_CTRL2_TEMPEN                                                      \
  ((uint32_t)0x00800000) /*!< Temperature Sensor and VREFINT Enable */

#define ADC_CTRL3_VABTMEN_MSK ((uint32_t)0x01L << 11)
#define ADC_CTRL3_DPWMOD_MSK ((uint32_t)0x01L << 10)
#define ADC_CTRL3_JENDCAIEN_MSK ((uint32_t)0x01L << 9)
#define ADC_CTRL3_ENDCAIEN_MSK ((uint32_t)0x01L << 8)
#define ADC_CTRL3_BPCAL_MSK ((uint32_t)0x01L << 7)
#define ADC_CTRL3_PDRDY_MSK ((uint32_t)0x01L << 6)
#define ADC_CTRL3_RDY_MSK ((uint32_t)0x01L << 5)
#define ADC_CTRL3_CKMOD_MSK ((uint32_t)0x01L << 4)
#define ADC_CTRL3_CALALD_MSK ((uint32_t)0x01L << 3)
#define ADC_CTRL3_CALDIF_MSK ((uint32_t)0x01L << 2)
#define ADC_CTRL3_RES_MSK ((uint32_t)0x03L << 0)

void ADC_Init(ADC_Module* ADCx, ADC_InitType* ADC_InitStruct);
void ADC_ConfigRegularChannel(ADC_Module* ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime);
#endif