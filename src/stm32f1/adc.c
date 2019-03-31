// ADC functions on STM32F1
//
// Copyright (C) 2018 Grigori Goronzy <greg@kinoho.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stdbool.h> // bool
#include "board/io.h" // readb
#include "command.h" // shutdown
#include "compiler.h" // ARRAY_SIZE
#include "gpio.h" // gpio_adc_setup
#include "internal.h" // GPIO
#include "stm32f1xx_ll_adc.h" // LL_ADC_REG_ReadConversionData12
#include "stm32f1xx_ll_gpio.h" // LL_GPIO_SetPinMode
#include "sched.h" // sched_shutdown

DECL_CONSTANT("ADC_MAX", 4095);

#define ADC_DELAY (240 * 8)

static bool adc_busy;
static uint32_t adc_current_channel;

static const uint8_t adc_pins[] = {
    GPIO('A', 0), GPIO('A', 1), GPIO('A', 2), GPIO('A', 3),
    GPIO('A', 4), GPIO('A', 5), GPIO('A', 6), GPIO('A', 7),
    GPIO('B', 0), GPIO('B', 1), GPIO('C', 0), GPIO('C', 1),
    GPIO('C', 2), GPIO('C', 3), GPIO('C', 4), GPIO('C', 5)
};

static const uint32_t adc_channels[] = {
    LL_ADC_CHANNEL_0,
    LL_ADC_CHANNEL_1,
    LL_ADC_CHANNEL_2,
    LL_ADC_CHANNEL_3,
    LL_ADC_CHANNEL_4,
    LL_ADC_CHANNEL_5,
    LL_ADC_CHANNEL_6,
    LL_ADC_CHANNEL_7,
    LL_ADC_CHANNEL_8,
    LL_ADC_CHANNEL_9,
    LL_ADC_CHANNEL_10,
    LL_ADC_CHANNEL_11,
    LL_ADC_CHANNEL_12,
    LL_ADC_CHANNEL_13,
    LL_ADC_CHANNEL_14,
    LL_ADC_CHANNEL_15,
};

struct gpio_adc
gpio_adc_setup(uint8_t pin)
{
    // Find pin in adc_pins table
    int chan;
    for (chan=0; ; chan++) {
        if (chan >= ARRAY_SIZE(adc_pins))
            shutdown("Not a valid ADC pin");
        if (adc_pins[chan] == pin)
            break;
    }

    GPIO_TypeDef *regs = digital_regs[GPIO2PORT(pin)];
    uint32_t bit = digital_pins[pin % 16];
    LL_GPIO_SetPinMode(regs, bit, LL_GPIO_MODE_ANALOG);

    return (struct gpio_adc){ .bit = adc_channels[chan] };
}

// Try to sample a value. Returns zero if sample ready, otherwise
// returns the number of clock ticks the caller should wait before
// retrying this function.
uint32_t
gpio_adc_sample(struct gpio_adc g)
{
    /* ADC not busy, start conversion */
    if (!readb(&adc_busy)) {
        LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, g.bit);
        LL_ADC_SetChannelSamplingTime(ADC1, g.bit
                                      , LL_ADC_SAMPLINGTIME_41CYCLES_5);
        LL_ADC_REG_StartConversionSWStart(ADC1);
        adc_busy = true;
        adc_current_channel = g.bit;
        return ADC_DELAY;
    /* ADC finished conversion for this channel */
    } else if (LL_ADC_IsActiveFlag_EOS(ADC1) &&
               readl(&adc_current_channel) == g.bit) {
        LL_ADC_ClearFlag_EOS(ADC1);
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
    return LL_ADC_REG_ReadConversionData12(ADC1);
}

// Cancel a sample that may have been started with gpio_adc_sample()
void
gpio_adc_cancel_sample(struct gpio_adc g)
{
    if (readb(&adc_busy) && readl(&adc_current_channel) == g.bit) {
        adc_busy = false;
        LL_ADC_ClearFlag_EOS(ADC1);
    }
}
