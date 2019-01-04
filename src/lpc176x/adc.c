// Analog to digital support on lpc176x
//
// Copyright (C) 2018-2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "LPC17xx.h" // LPC_PINCON
#include "autoconf.h" // CONFIG_CLOCK_FREQ
#include "board/misc.h" // timer_from_us
#include "command.h" // shutdown
#include "gpio.h" // gpio_adc_setup
#include "internal.h" // gpio_peripheral
#include "sched.h" // sched_shutdown

static const uint8_t adc_pins[] = {
    GPIO(0, 23), GPIO(0, 24), GPIO(0, 25), GPIO(0, 26),
    GPIO(1, 30), GPIO(1, 31), GPIO(0, 3), GPIO(0, 2),
};

static const uint8_t adc_pin_funcs[] = {
    1, 1, 1, 1, 3, 3, 2, 2
};

#define ADC_FREQ_MAX 13000000
DECL_CONSTANT(ADC_MAX, 4095);

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

    uint32_t prescal = DIV_ROUND_UP(CONFIG_CLOCK_FREQ*4, ADC_FREQ_MAX) - 1;
    uint32_t adcr = (1<<21) | ((prescal & 0xff) << 8);
    if (!is_enabled_pclock(PCLK_ADC)) {
        // Power up ADC
        enable_pclock(PCLK_ADC);
        LPC_ADC->ADCR = adcr;
    }

    gpio_peripheral(pin, adc_pin_funcs[chan], 0);

    return (struct gpio_adc){ .cmd = adcr | (1 << chan) | (1 << 24) };
}

static uint32_t adc_status;

// Try to sample a value. Returns zero if sample ready, otherwise
// returns the number of clock ticks the caller should wait before
// retrying this function.
uint32_t
gpio_adc_sample(struct gpio_adc g)
{
    uint32_t status = adc_status;
    if (status == g.cmd) {
        // Sample already underway - check if it is ready
        uint32_t val = LPC_ADC->ADGDR;
        if (val & (1<<31))
            // Sample ready
            return 0;
        goto need_delay;
    }
    if (status)
        // ADC busy on some other channel
        goto need_delay;

    // Start new sample
    adc_status = g.cmd;
    LPC_ADC->ADCR = g.cmd;

need_delay:
    return (65 * DIV_ROUND_UP(CONFIG_CLOCK_FREQ*4, ADC_FREQ_MAX)
            + timer_from_us(10));
}

// Read a value; use only after gpio_adc_sample() returns zero
uint16_t
gpio_adc_read(struct gpio_adc g)
{
    adc_status = 0;
    return (LPC_ADC->ADGDR >> 4) & 0x0fff;
}

// Cancel a sample that may have been started with gpio_adc_sample()
void
gpio_adc_cancel_sample(struct gpio_adc g)
{
    if (adc_status == g.cmd)
        adc_status = 0;
}
