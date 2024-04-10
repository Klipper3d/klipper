/*
 *  ADC functions on STM32F0
 *
 *  Copyright (C) 2019 Eug Krashtan <eug.krashtan@gmail.com>
 *  This file may be distributed under the terms of the GNU GPLv3 license.
 *
 */

#include <stdbool.h> // bool
#include "board/gpio.h" // gpio_adc_setup
#include "board/io.h" // readb
#include "stm32f0xx_hal.h"
#include "internal.h" // GPIO
#include "compiler.h" // ARRAY_SIZE
#include "command.h" // shutdown
#include "sched.h" // sched_shutdown

DECL_CONSTANT("ADC_MAX", 4095);

#define ADC_DELAY (240 * 8)
static bool adc_busy;
static uint8_t adc_current_channel;
ADC_HandleTypeDef hadc;

static const uint8_t adc_pins[] = {
    GPIO('A', 0), GPIO('A', 1),
#if !(CONFIG_SERIAL)
    GPIO('A', 2), GPIO('A', 3),
#endif
    GPIO('A', 4), GPIO('A', 5), GPIO('A', 6), GPIO('A', 7),
    GPIO('B', 1),
    // Fake ADC on this GPIO (returns tempsensor value)
    GPIO('B', 8)
};

static const uint32_t adc_channels[] = {
    ADC_CHANNEL_0,
    ADC_CHANNEL_1,
#if !(CONFIG_SERIAL)
    ADC_CHANNEL_2,
    ADC_CHANNEL_3,
#endif
    ADC_CHANNEL_4,
    ADC_CHANNEL_5,
    ADC_CHANNEL_6,
    ADC_CHANNEL_7,
    ADC_CHANNEL_9,
    // For tests
    ADC_CHANNEL_TEMPSENSOR
};

struct gpio_adc
gpio_adc_setup(uint8_t pin)
{
    // Find pin in adc_pins table
    int chan;
    ADC_ChannelConfTypeDef sConfig = {0};
    for (chan=0; ; chan++) {
        if (chan >= ARRAY_SIZE(adc_pins))
            shutdown("Not a valid ADC pin");
        if (adc_pins[chan] == pin) {
            sConfig.Channel = adc_channels[chan];
            break;
        }
    }

    /**ADC GPIO Configuration
    */
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = 1 << (pin % 16);;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init((pin<8)?GPIOA:GPIOB, &GPIO_InitStruct);
    /**Configure for the selected ADC regular channel to be converted.
    */
    sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
    sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
    HAL_ADC_ConfigChannel(&hadc, &sConfig);
    HAL_ADCEx_Calibration_Start(&hadc);
    return (struct gpio_adc){ .pin = adc_channels[chan] };
}

// Try to sample a value. Returns zero if sample ready, otherwise
// returns the number of clock ticks the caller should wait before
// retrying this function.
uint32_t
gpio_adc_sample(struct gpio_adc g)
{
    /* ADC not busy, start conversion */
    if (!readb(&adc_busy)) {
        HAL_ADC_Start(&hadc);
        adc_busy = true;
        adc_current_channel = g.pin;
        return ADC_DELAY;
    /* ADC finished conversion for this channel */
    } else if (HAL_ADC_PollForConversion(&hadc, 100) == HAL_OK) {
        __HAL_ADC_CLEAR_FLAG(&hadc, ADC_FLAG_EOS);
        adc_busy = false;
        return 0;
    }
    /* Wants to sample another channel, or not finished yet */
    return ADC_DELAY;
}

// Read a value; use only after gpio_adc_sample() returns zero
uint16_t
gpio_adc_read(struct gpio_adc g)
{
    return HAL_ADC_GetValue(&hadc);
}

// Cancel a sample that may have been started with gpio_adc_sample()
void
gpio_adc_cancel_sample(struct gpio_adc g)
{
    if (readb(&adc_busy) && readl(&adc_current_channel) == g.pin) {
        adc_busy = false;
        HAL_ADC_Stop(&hadc);
    }
}

/**
  * @brief ADC Initialization Function
  * @param None
  * @retval None
  */
void AdcInit(void)
{
    /**Configure the global features of the ADC (Clock,
     * Resolution, Data Alignment and number of conversion)
    */
    hadc.Instance = ADC1;
    hadc.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
    hadc.Init.Resolution = ADC_RESOLUTION_12B;
    hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
    hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    hadc.Init.LowPowerAutoWait = DISABLE;
    hadc.Init.LowPowerAutoPowerOff = DISABLE;
    hadc.Init.ContinuousConvMode = DISABLE;
    hadc.Init.DiscontinuousConvMode = DISABLE;
    hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc.Init.DMAContinuousRequests = DISABLE;
    hadc.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
    HAL_ADC_Init(&hadc);
}
DECL_INIT(AdcInit);


/**
* @brief ADC MSP Initialization
* @param hadc: ADC handle pointer
* @retval None
*/
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
    if(hadc->Instance==ADC1)
    {
        /* Peripheral clock enable */
        __HAL_RCC_ADC1_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
    }
}
