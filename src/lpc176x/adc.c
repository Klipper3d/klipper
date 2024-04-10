// Analog to digital support on lpc176x
//
// Copyright (C) 2018-2021  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/armcm_boot.h" // armcm_enable_irq
#include "board/irq.h" // irq_save
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
DECL_CONSTANT("ADC_MAX", 4095);

// The lpc176x adc is extremely noisy. Implement a 5 entry median
// filter to weed out obviously incorrect readings.
static struct {
    uint32_t adcr;
    uint16_t pos;
    uint16_t chan;
    uint16_t samples[5];
} adc_status;

enum { ADC_DONE=0x0100 };

// ADC hardware irq handler
void
ADC_IRQHandler(void)
{
    uint32_t pos = adc_status.pos, chan = adc_status.chan & 0xff;
    uint32_t result = (&LPC_ADC->ADDR0)[chan];
    if (pos >= ARRAY_SIZE(adc_status.samples))
        // All samples complete
        return;
    if (pos >= ARRAY_SIZE(adc_status.samples) - 2)
        // Turn off burst mode
        LPC_ADC->ADCR = adc_status.adcr | (1 << chan);
    adc_status.samples[pos++] = (result >> 4) & 0x0fff;
    adc_status.pos = pos;
}

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

    if (!is_enabled_pclock(PCLK_ADC)) {
        // Power up ADC
        enable_pclock(PCLK_ADC);
        uint32_t pclk = get_pclock_frequency(PCLK_ADC);
        uint32_t prescal = DIV_ROUND_UP(pclk, ADC_FREQ_MAX) - 1;
        LPC_ADC->ADCR = adc_status.adcr = (1<<21) | ((prescal & 0xff) << 8);
        LPC_ADC->ADINTEN = 0xff;
        adc_status.chan = ADC_DONE;
        armcm_enable_irq(ADC_IRQHandler, ADC_IRQn, 0);
    }

    gpio_peripheral(pin, adc_pin_funcs[chan], 0);

    return (struct gpio_adc){ .chan = chan };
}

// Try to sample a value. Returns zero if sample ready, otherwise
// returns the number of clock ticks the caller should wait before
// retrying this function.
uint32_t
gpio_adc_sample(struct gpio_adc g)
{
    uint32_t chan = adc_status.chan;
    if (chan == g.chan) {
        // Sample already underway - check if it is ready
        if (adc_status.pos >= ARRAY_SIZE(adc_status.samples))
            // Sample ready
            return 0;
        goto need_delay;
    }
    if (!(chan & ADC_DONE))
        // ADC busy on some other channel
        goto need_delay;

    // Start new sample
    adc_status.pos = 0;
    adc_status.chan = g.chan;
    LPC_ADC->ADCR = adc_status.adcr | (1 << g.chan) | (1<<16);

need_delay: ;
    uint32_t pclk = get_pclock_frequency(PCLK_ADC);
    return ((64 * DIV_ROUND_UP(pclk, ADC_FREQ_MAX)
             * ARRAY_SIZE(adc_status.samples)) / 4 + timer_from_us(10));
}

#define ORDER(r1, r2) do {                                      \
        if (r1 > r2) { uint32_t t = r1; r1 = r2; r2 = t; }      \
    } while (0)

// Read a value; use only after gpio_adc_sample() returns zero
uint16_t
gpio_adc_read(struct gpio_adc g)
{
    adc_status.chan |= ADC_DONE;
    // The lpc176x adc has a defect that causes random reports near
    // 0xfff. Work around that with a 5 sample median filter.
    uint16_t *p = adc_status.samples;
    uint32_t v0 = p[0], v4 = p[1], v1 = p[2], v3 = p[3], v2 = p[4];
    ORDER(v0, v4);
    ORDER(v1, v3);
    ORDER(v0, v1);
    ORDER(v3, v4);
    ORDER(v1, v3);
    ORDER(v1, v2);
    ORDER(v2, v3);
    if (v3 >= 0xff0 || v4 >= 0xff0) {
        ORDER(v0, v1);
        if (v2 >= 0xff0)
            // At least 3 reports are clearly bogus - return the minimum sample
            return v0;
        // 1 or 2 bogus reports - return the median of the minimum 3 samples
        return v1;
    }
    // Return the median of the 5 samples
    return v2;
}

// Cancel a sample that may have been started with gpio_adc_sample()
void
gpio_adc_cancel_sample(struct gpio_adc g)
{
    uint32_t chan = adc_status.chan;
    if (chan != g.chan)
        return;
    irqstatus_t flag = irq_save();
    LPC_ADC->ADCR = adc_status.adcr;
    adc_status.chan = chan | ADC_DONE;
    adc_status.pos = ARRAY_SIZE(adc_status.samples);
    (&LPC_ADC->ADDR0)[chan & 0xff];
    irq_restore(flag);
}
