// Analog to digital support
//
// Copyright (C) 2016-2020  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h" // CONFIG_CLOCK_FREQ
#include "board/irq.h" // irq_save
#include "command.h" // shutdown
#include "compiler.h" // ARRAY_SIZE
#include "gpio.h" // gpio_adc_setup
#include "internal.h" // GPIO
#include "sched.h" // sched_shutdown

#define ADC_TEMPERATURE_PIN 0xfe
DECL_ENUMERATION("pin", "ADC_TEMPERATURE", ADC_TEMPERATURE_PIN);

static const uint8_t adc_pins[] = {
#if CONFIG_MACH_SAM3X
    GPIO('A', 2), GPIO('A', 3), GPIO('A', 4), GPIO('A', 6),
    GPIO('A', 22), GPIO('A', 23), GPIO('A', 24), GPIO('A', 16),
    GPIO('B', 12), GPIO('B', 13), GPIO('B', 17), GPIO('B', 18),
    GPIO('B', 19), GPIO('B', 20), GPIO('B', 21), ADC_TEMPERATURE_PIN
#elif CONFIG_MACH_SAM4S
    GPIO('A', 17), GPIO('A', 18), GPIO('A', 19), GPIO('A', 20),
    GPIO('B', 0), GPIO('B', 1), GPIO('B', 2), GPIO('B', 3),
    GPIO('A', 21), GPIO('A', 22), GPIO('C', 13), GPIO('C', 15),
    GPIO('C', 12), GPIO('C', 29), GPIO('C', 30), ADC_TEMPERATURE_PIN
#endif
};

#define ADC_FREQ_MAX 20000000
DECL_CONSTANT("ADC_MAX", 4095);

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

    if (!is_enabled_pclock(ID_ADC)) {
        // Setup ADC
        enable_pclock(ID_ADC);
        uint32_t prescal = SystemCoreClock / (2 * ADC_FREQ_MAX) - 1;
        ADC->ADC_MR = (ADC_MR_PRESCAL(prescal)
                       | ADC_MR_STARTUP_SUT768
                       | ADC_MR_TRANSFER(1));
    }

    if (pin == ADC_TEMPERATURE_PIN) {
        // Enable temperature sensor
        ADC->ADC_ACR |= ADC_ACR_TSON;
    } else {
        // Place pin in input floating mode
        gpio_in_setup(pin, 0);
    }
    return (struct gpio_adc){ .chan = 1 << chan };
}

// Try to sample a value. Returns zero if sample ready, otherwise
// returns the number of clock ticks the caller should wait before
// retrying this function.
uint32_t
gpio_adc_sample(struct gpio_adc g)
{
    uint32_t chsr = ADC->ADC_CHSR & 0xffff;
    if (!chsr) {
        // Start sample
        ADC->ADC_CHER = g.chan;
        ADC->ADC_CR = ADC_CR_START;
        goto need_delay;
    }
    if (chsr != g.chan)
        // Sampling in progress on another channel
        goto need_delay;
    if (!(ADC->ADC_ISR & ADC_ISR_DRDY))
        // Conversion still in progress
        goto need_delay;
    // Conversion ready
    return 0;
need_delay:
    return ADC_FREQ_MAX * 1000ULL / CONFIG_CLOCK_FREQ;
}

// Read a value; use only after gpio_adc_sample() returns zero
uint16_t
gpio_adc_read(struct gpio_adc g)
{
    ADC->ADC_CHDR = g.chan;
    return ADC->ADC_LCDR;
}

// Cancel a sample that may have been started with gpio_adc_sample()
void
gpio_adc_cancel_sample(struct gpio_adc g)
{
    irqstatus_t flag = irq_save();
    if ((ADC->ADC_CHSR & 0xffff) == g.chan)
        gpio_adc_read(g);
    irq_restore(flag);
}
