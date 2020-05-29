// Analog to digital conversion (ADC) code on PRU
//
// Copyright (C) 2017  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/io.h" // readl
#include "command.h" // shutdown
#include "compiler.h" // ARRAY_SIZE
#include "gpio.h" // gpio_adc_setup
#include "internal.h" // ADC
#include "sched.h" // sched_shutdown


/****************************************************************
 * Analog to Digital Converter (ADC) pins
 ****************************************************************/

DECL_CONSTANT("ADC_MAX", 4095);

static void
adc_full_reset(void)
{
    static uint8_t have_done_reset;
    if (have_done_reset)
        return;

    // Disable ADC
    ADC->ctrl = (1<<2);
    barrier();
    // Clear registers
    ADC->irqstatus = 0xffffffff;
    ADC->irqenable_clr = 0xffffffff;
    ADC->dmaenable_clr = 0xffffffff;
    ADC->adc_clkdiv = 0;
    ADC->stepenable = 0;
    ADC->idleconfig = 0;
    int i;
    for (i=0; i<8; i++) {
        ADC->step[i].config = i<<19;
        ADC->step[i].delay = 0;
    }
    // Enable ADC
    writel(&ADC->ctrl, 0x07);
    // Drain fifo
    while (readl(&ADC->fifo0count))
        readl(&ADC->fifo0data);

    if (!readl(&ADC->ctrl))
        shutdown("ADC module not enabled");
    have_done_reset = 1;
}

DECL_ENUMERATION_RANGE("pin", "AIN0", 4 * 32, 8);

struct gpio_adc
gpio_adc_setup(uint8_t pin)
{
    uint8_t chan = pin - 4 * 32;
    if (chan >= 8)
        shutdown("Not an adc channel");
    adc_full_reset();
    return (struct gpio_adc){ .chan = chan };
}

enum { ADC_DUMMY=0xff };
static uint8_t last_analog_read = ADC_DUMMY;
static uint16_t last_analog_sample;

// Try to sample a value. Returns zero if sample ready, otherwise
// returns the number of clock ticks the caller should wait before
// retrying this function.
uint32_t
gpio_adc_sample(struct gpio_adc g)
{
    uint8_t last = last_analog_read;
    if (last == ADC_DUMMY) {
        // Start sample
        last_analog_read = g.chan;
        writel(&ADC->stepenable, 1 << (g.chan + 1));
        goto need_delay;
    }
    if (last == g.chan) {
        // Check if sample ready
        while (readl(&ADC->fifo0count)) {
            uint32_t sample = readl(&ADC->fifo0data);
            if (sample >> 16 == g.chan) {
                last_analog_read = ADC_DUMMY;
                last_analog_sample = sample;
                return 0;
            }
        }
    }
need_delay:
    return 160;
}

// Read a value; use only after gpio_adc_sample() returns zero
uint16_t
gpio_adc_read(struct gpio_adc g)
{
    return last_analog_sample;
}

// Cancel a sample that may have been started with gpio_adc_sample()
void
gpio_adc_cancel_sample(struct gpio_adc g)
{
    if (last_analog_read == g.chan)
        last_analog_read = ADC_DUMMY;
}
