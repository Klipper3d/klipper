// ADC functions on STM32F4
//
// Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/irq.h" // irq_save
#include "board/misc.h" // timer_from_us
#include "command.h" // shutdown
#include "compiler.h" // ARRAY_SIZE
#include "gpio.h" // gpio_adc_setup
#include "internal.h" // GPIO
#include "sched.h" // sched_shutdown

DECL_CONSTANT("ADC_MAX", 4095);

static const uint8_t adc_pins[] = {
    GPIO('A', 0), GPIO('A', 1), GPIO('A', 2), GPIO('A', 3),
    GPIO('A', 4), GPIO('A', 5), GPIO('A', 6), GPIO('A', 7),
    GPIO('B', 0), GPIO('B', 1), GPIO('C', 0), GPIO('C', 1),
    GPIO('C', 2), GPIO('C', 3), GPIO('C', 4), GPIO('C', 5)
};

struct gpio_adc
gpio_adc_setup(uint32_t pin)
{
    // Find pin in adc_pins table
    int chan;
    for (chan=0; ; chan++) {
        if (chan >= ARRAY_SIZE(adc_pins))
            shutdown("Not a valid ADC pin");
        if (adc_pins[chan] == pin)
            break;
    }

    // Enable the ADC
    if (!is_enabled_pclock(ADC1_BASE)) {
        enable_pclock(ADC1_BASE);
        uint32_t aticks = 3; // 56 adc cycles
        ADC1->SMPR1 = (aticks | (aticks << 3) | (aticks << 6) | (aticks << 9)
                       | (aticks << 12) | (aticks << 15) | (aticks << 18)
                       | (aticks << 21) | (aticks << 24));
        ADC1->SMPR2 = (aticks | (aticks << 3) | (aticks << 6) | (aticks << 9)
                       | (aticks << 12) | (aticks << 15) | (aticks << 18)
                       | (aticks << 21) | (aticks << 24) | (aticks << 27));
        ADC1->CR2 = ADC_CR2_ADON;
    }

    gpio_peripheral(pin, GPIO_ANALOG, 0);

    return (struct gpio_adc){ .chan = chan };
}

// Try to sample a value. Returns zero if sample ready, otherwise
// returns the number of clock ticks the caller should wait before
// retrying this function.
uint32_t
gpio_adc_sample(struct gpio_adc g)
{
    uint32_t sr = ADC1->SR;
    if (sr & ADC_SR_STRT) {
        if (!(sr & ADC_SR_EOC) || ADC1->SQR3 != g.chan)
            // Conversion still in progress or busy on another channel
            goto need_delay;
        // Conversion ready
        return 0;
    }
    // Start sample
    ADC1->SQR3 = g.chan;
    ADC1->CR2 = ADC_CR2_SWSTART | ADC_CR2_ADON;
need_delay:
    return timer_from_us(4);
}

// Read a value; use only after gpio_adc_sample() returns zero
uint16_t
gpio_adc_read(struct gpio_adc g)
{
    ADC1->SR = ~ADC_SR_STRT;
    return ADC1->DR;
}

// Cancel a sample that may have been started with gpio_adc_sample()
void
gpio_adc_cancel_sample(struct gpio_adc g)
{
    irqstatus_t flag = irq_save();
    if (ADC1->SR & ADC_SR_STRT && ADC1->SQR3 == g.chan)
        gpio_adc_read(g);
    irq_restore(flag);
}
