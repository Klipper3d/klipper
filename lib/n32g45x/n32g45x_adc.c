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

#include "stdint.h"
#include "n32g45x_adc.h"

void
ADC_Init (ADC_Module *NS_ADCx, ADC_InitType *ADC_InitStruct)
{
  uint32_t tmpreg1 = 0;
  uint8_t tmpreg2 = 0;

  /*---------------------------- ADCx CTRL1 Configuration -----------------*/
  /* Get the ADCx CTRL1 value */
  tmpreg1 = NS_ADCx->CTRL1;
  /* Clear DUALMOD and SCAN bits */
  tmpreg1 &= CTRL1_CLR_MASK;
  /* Configure ADCx: Dual mode and scan conversion mode */
  /* Set DUALMOD bits according to WorkMode value */
  /* Set SCAN bit according to MultiChEn value */
  tmpreg1 |= (uint32_t) (ADC_InitStruct->WorkMode
                         | ((uint32_t)ADC_InitStruct->MultiChEn << 8));
  /* Write to ADCx CTRL1 */
  NS_ADCx->CTRL1 = tmpreg1;

  /*---------------------------- ADCx CTRL2 Configuration -----------------*/
  /* Get the ADCx CTRL2 value */
  tmpreg1 = NS_ADCx->CTRL2;
  /* Clear CONT, ALIGN and EXTSEL bits */
  tmpreg1 &= CTRL2_CLR_MASK;
  /* Configure ADCx: external trigger event and continuous conversion mode */
  /* Set ALIGN bit according to DatAlign value */
  /* Set EXTSEL bits according to ExtTrigSelect value */
  /* Set CONT bit according to ContinueConvEn value */
  tmpreg1
      |= (uint32_t) (ADC_InitStruct->DatAlign | ADC_InitStruct->ExtTrigSelect
                     | ((uint32_t)ADC_InitStruct->ContinueConvEn << 1));
  /* Write to ADCx CTRL2 */
  NS_ADCx->CTRL2 = tmpreg1;

  /*---------------------------- ADCx RSEQ1 Configuration -----------------*/
  /* Get the ADCx RSEQ1 value */
  tmpreg1 = NS_ADCx->RSEQ1;
  /* Clear L bits */
  tmpreg1 &= RSEQ1_CLR_MASK;
  /* Configure ADCx: regular channel sequence length */
  /* Set L bits according to ChsNumber value */
  tmpreg2 |= (uint8_t) (ADC_InitStruct->ChsNumber - (uint8_t)1);
  tmpreg1 |= (uint32_t)tmpreg2 << 20;
  /* Write to ADCx RSEQ1 */
  NS_ADCx->RSEQ1 = tmpreg1;
}

void
ADC_ConfigRegularChannel (ADC_Module *NS_ADCx, uint8_t ADC_Channel,
                          uint8_t Rank, uint8_t ADC_SampleTime)
{
  uint32_t tmpreg1 = 0, tmpreg2 = 0;

  if (ADC_Channel == ADC_CH_18)
    {
      tmpreg1 = NS_ADCx->SAMPT3;
      tmpreg1 &= (~0x00000007);
      tmpreg1 |= ADC_SampleTime;
      NS_ADCx->SAMPT3 = tmpreg1;
    }
  else if (ADC_Channel > ADC_CH_9) /* if ADC_CH_10 ... ADC_CH_17 is selected */
    {
      /* Get the old register value */
      tmpreg1 = NS_ADCx->SAMPT1;
      /* Calculate the mask to clear */
      tmpreg2 = SAMPT1_SMP_SET << (3 * (ADC_Channel - 10));
      /* Clear the old channel sample time */
      tmpreg1 &= ~tmpreg2;
      /* Calculate the mask to set */
      tmpreg2 = (uint32_t)ADC_SampleTime << (3 * (ADC_Channel - 10));
      /* Set the new channel sample time */
      tmpreg1 |= tmpreg2;
      /* Store the new register value */
      NS_ADCx->SAMPT1 = tmpreg1;
    }
  else /* ADC_Channel include in ADC_Channel_[0..9] */
    {
      /* Get the old register value */
      tmpreg1 = NS_ADCx->SAMPT2;
      /* Calculate the mask to clear */
      tmpreg2 = SAMPT2_SMP_SET << (3 * ADC_Channel);
      /* Clear the old channel sample time */
      tmpreg1 &= ~tmpreg2;
      /* Calculate the mask to set */
      tmpreg2 = (uint32_t)ADC_SampleTime << (3 * ADC_Channel);
      /* Set the new channel sample time */
      tmpreg1 |= tmpreg2;
      /* Store the new register value */
      NS_ADCx->SAMPT2 = tmpreg1;
    }
  /* For Rank 1 to 6 */
  if (Rank < 7)
    {
      /* Get the old register value */
      tmpreg1 = NS_ADCx->RSEQ3;
      /* Calculate the mask to clear */
      tmpreg2 = SQR3_SEQ_SET << (5 * (Rank - 1));
      /* Clear the old SQx bits for the selected rank */
      tmpreg1 &= ~tmpreg2;
      /* Calculate the mask to set */
      tmpreg2 = (uint32_t)ADC_Channel << (5 * (Rank - 1));
      /* Set the SQx bits for the selected rank */
      tmpreg1 |= tmpreg2;
      /* Store the new register value */
      NS_ADCx->RSEQ3 = tmpreg1;
    }
  /* For Rank 7 to 12 */
  else if (Rank < 13)
    {
      /* Get the old register value */
      tmpreg1 = NS_ADCx->RSEQ2;
      /* Calculate the mask to clear */
      tmpreg2 = SQR2_SEQ_SET << (5 * (Rank - 7));
      /* Clear the old SQx bits for the selected rank */
      tmpreg1 &= ~tmpreg2;
      /* Calculate the mask to set */
      tmpreg2 = (uint32_t)ADC_Channel << (5 * (Rank - 7));
      /* Set the SQx bits for the selected rank */
      tmpreg1 |= tmpreg2;
      /* Store the new register value */
      NS_ADCx->RSEQ2 = tmpreg1;
    }
  /* For Rank 13 to 16 */
  else
    {
      /* Get the old register value */
      tmpreg1 = NS_ADCx->RSEQ1;
      /* Calculate the mask to clear */
      tmpreg2 = SQR1_SEQ_SET << (5 * (Rank - 13));
      /* Clear the old SQx bits for the selected rank */
      tmpreg1 &= ~tmpreg2;
      /* Calculate the mask to set */
      tmpreg2 = (uint32_t)ADC_Channel << (5 * (Rank - 13));
      /* Set the SQx bits for the selected rank */
      tmpreg1 |= tmpreg2;
      /* Store the new register value */
      NS_ADCx->RSEQ1 = tmpreg1;
    }
}
