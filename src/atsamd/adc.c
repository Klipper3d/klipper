// Analog to Digital Converter support
//
// Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "command.h" // shutdown
#include "gpio.h" // gpio_adc_read
#include "internal.h" // GPIO
#include "sched.h" // sched_shutdown

static const uint8_t adc_pins[] = {
    GPIO('A', 2), GPIO('A', 3), GPIO('B', 8), GPIO('B', 9), GPIO('A', 4),
    GPIO('A', 5), GPIO('A', 6), GPIO('A', 7), GPIO('B', 0), GPIO('B', 1),
    GPIO('B', 2), GPIO('B', 3), GPIO('B', 4), GPIO('B', 5), GPIO('B', 6),
    GPIO('B', 7), GPIO('A', 8), GPIO('A', 9), GPIO('A', 10), GPIO('A', 11)
};

DECL_CONSTANT(ADC_MAX, 4095);

static void
adc_init(void)
{
    static uint8_t have_run_init;
    if (have_run_init)
        return;
    have_run_init = 1;

    // Enable adc clock
    enable_pclock(ADC_GCLK_ID, ID_ADC);

    // Load calibraiton info
    uint32_t v = *((uint32_t*)ADC_FUSES_BIASCAL_ADDR);
    uint32_t bias = (v & ADC_FUSES_BIASCAL_Msk) >> ADC_FUSES_BIASCAL_Pos;
    v = *((uint32_t*)ADC_FUSES_LINEARITY_0_ADDR);
    uint32_t li0 = (v & ADC_FUSES_LINEARITY_0_Msk) >> ADC_FUSES_LINEARITY_0_Pos;
    v = *((uint32_t*)ADC_FUSES_LINEARITY_1_ADDR);
    uint32_t li5 = (v & ADC_FUSES_LINEARITY_1_Msk) >> ADC_FUSES_LINEARITY_1_Pos;
    uint32_t lin = li0 | (li5 << 5);
    ADC->CALIB.reg = ADC_CALIB_BIAS_CAL(bias) | ADC_CALIB_LINEARITY_CAL(lin);

    // Setup and enable adc
    ADC->REFCTRL.reg = ADC_REFCTRL_REFSEL_INTVCC1;
    ADC->CTRLB.reg = ADC_CTRLB_PRESCALER_DIV128;
    ADC->SAMPCTRL.reg = 63;
    ADC->CTRLA.reg = ADC_CTRLA_ENABLE;
}

struct gpio_adc
gpio_adc_setup(uint8_t pin)
{
    // Find pin in adc_pins table
    uint8_t chan;
    for (chan=0; ; chan++) {
        if (chan >= ARRAY_SIZE(adc_pins))
            shutdown("Not a valid ADC pin");
        if (adc_pins[chan] == pin)
            break;
    }

    // Enable ADC
    adc_init();

    // Set pin in ADC mode
    gpio_peripheral(pin, 'B', 0);

    return (struct gpio_adc){ chan };
}

enum { ADC_DUMMY=0xff };
static uint8_t last_analog_read = ADC_DUMMY;

// Try to sample a value. Returns zero if sample ready, otherwise
// returns the number of clock ticks the caller should wait before
// retrying this function.
uint32_t
gpio_adc_sample(struct gpio_adc g)
{
    if (last_analog_read == g.chan) {
        if (ADC->INTFLAG.reg & ADC_INTFLAG_RESRDY)
            // Sample now ready
            return 0;
        // ADC is still busy
        goto need_delay;
    }
    if (last_analog_read != ADC_DUMMY)
        // Sample on another channel in progress
        goto need_delay;
    last_analog_read = g.chan;

    // Set the channel to sample
    ADC->INPUTCTRL.reg = (ADC_INPUTCTRL_MUXPOS(g.chan)
                          | ADC_INPUTCTRL_MUXNEG_GND | ADC_INPUTCTRL_GAIN_DIV2);

    // Start the sample
    ADC->SWTRIG.reg = ADC_SWTRIG_START;

    // Schedule next attempt after sample is likely to be complete
need_delay:
    return 42 * 128 + 200; // 42 == 1 + (63+1)/2 + 1 + 12/2 + 1.5
}

// Read a value; use only after gpio_adc_sample() returns zero
uint16_t
gpio_adc_read(struct gpio_adc g)
{
    last_analog_read = ADC_DUMMY;
    return ADC->RESULT.reg;
}

// Cancel a sample that may have been started with gpio_adc_sample()
void
gpio_adc_cancel_sample(struct gpio_adc g)
{
    if (last_analog_read == g.chan) {
        ADC->SWTRIG.reg = ADC_SWTRIG_FLUSH;
        ADC->INTFLAG.reg = ADC_INTFLAG_RESRDY;
        last_analog_read = ADC_DUMMY;
    }
}
