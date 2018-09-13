// GPIO functions on sam3x8e
//
// Copyright (C) 2016,2017  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stdint.h> // uint32_t
#include "autoconf.h" // CONFIG_CLOCK_FREQ
#include "board/irq.h" // irq_save
#include "command.h" // shutdown
#include "compiler.h" // ARRAY_SIZE
#include "gpio.h" // gpio_out_setup
#include "sam3x8e.h" // Pio
#include "sched.h" // sched_shutdown


/****************************************************************
 * Pin mappings
 ****************************************************************/

#define GPIO(PORT, NUM) (((PORT)-'A') * 32 + (NUM))
#define GPIO2PORT(PIN) ((PIN) / 32)
#define GPIO2BIT(PIN) (1<<((PIN) % 32))

static Pio * const digital_regs[] = {
    PIOA, PIOB, PIOC, PIOD
};


/****************************************************************
 * General Purpose Input Output (GPIO) pins
 ****************************************************************/

void
gpio_peripheral(char bank, uint32_t bit, char ptype, uint32_t pull_up)
{
    Pio *regs = digital_regs[bank - 'A'];
    if (ptype == 'A')
        regs->PIO_ABSR &= ~bit;
    else
        regs->PIO_ABSR |= bit;
    if (pull_up)
        regs->PIO_PUER = bit;
    else
        regs->PIO_PUDR = bit;
    regs->PIO_PDR = bit;
}


struct gpio_out
gpio_out_setup(uint8_t pin, uint8_t val)
{
    if (GPIO2PORT(pin) >= ARRAY_SIZE(digital_regs))
        goto fail;
    Pio *regs = digital_regs[GPIO2PORT(pin)];
    struct gpio_out g = { .regs=regs, .bit=GPIO2BIT(pin) };
    gpio_out_reset(g, val);
    return g;
fail:
    shutdown("Not an output pin");
}

void
gpio_out_reset(struct gpio_out g, uint8_t val)
{
    Pio *regs = g.regs;
    irqstatus_t flag = irq_save();
    if (val)
        regs->PIO_SODR = g.bit;
    else
        regs->PIO_CODR = g.bit;
    regs->PIO_OER = g.bit;
    regs->PIO_OWER = g.bit;
    regs->PIO_PER = g.bit;
    regs->PIO_PUDR = g.bit;
    irq_restore(flag);
}

void
gpio_out_toggle_noirq(struct gpio_out g)
{
    Pio *regs = g.regs;
    regs->PIO_ODSR ^= g.bit;
}

void
gpio_out_toggle(struct gpio_out g)
{
    irqstatus_t flag = irq_save();
    gpio_out_toggle_noirq(g);
    irq_restore(flag);
}

void
gpio_out_write(struct gpio_out g, uint8_t val)
{
    Pio *regs = g.regs;
    if (val)
        regs->PIO_SODR = g.bit;
    else
        regs->PIO_CODR = g.bit;
}


struct gpio_in
gpio_in_setup(uint8_t pin, int8_t pull_up)
{
    if (GPIO2PORT(pin) >= ARRAY_SIZE(digital_regs))
        goto fail;
    uint32_t port = GPIO2PORT(pin);
    PMC->PMC_PCER0 = 1 << (ID_PIOA + port);
    struct gpio_in g = { .regs=digital_regs[port], .bit=GPIO2BIT(pin) };
    gpio_in_reset(g, pull_up);
    return g;
fail:
    shutdown("Not an input pin");
}

void
gpio_in_reset(struct gpio_in g, int8_t pull_up)
{
    Pio *regs = g.regs;
    irqstatus_t flag = irq_save();
    if (pull_up)
        regs->PIO_PUER = g.bit;
    else
        regs->PIO_PUDR = g.bit;
    regs->PIO_ODR = g.bit;
    regs->PIO_PER = g.bit;
    irq_restore(flag);
}

uint8_t
gpio_in_read(struct gpio_in g)
{
    Pio *regs = g.regs;
    return !!(regs->PIO_PDSR & g.bit);
}


/****************************************************************
 * Analog to Digital Converter (ADC) pins
 ****************************************************************/

static const uint8_t adc_pins[] = {
    GPIO('A', 2), GPIO('A', 3), GPIO('A', 4), GPIO('A', 6),
    GPIO('A', 22), GPIO('A', 23), GPIO('A', 24), GPIO('A', 16),
    GPIO('B', 12), GPIO('B', 13), GPIO('B', 17), GPIO('B', 18),
    GPIO('B', 19), GPIO('B', 20)
};

#define ADC_FREQ_MAX 20000000
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

    if (!(PMC->PMC_PCSR1 & (1 << (ID_ADC-32)))) {
        // Setup ADC
        PMC->PMC_PCER1 = 1 << (ID_ADC-32);
        uint32_t prescal = SystemCoreClock / (2 * ADC_FREQ_MAX) - 1;
        ADC->ADC_MR = (ADC_MR_PRESCAL(prescal)
                       | ADC_MR_STARTUP_SUT768
                       | ADC_MR_TRANSFER(1));
    }
    return (struct gpio_adc){ .bit = 1 << chan };
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
        ADC->ADC_CHER = g.bit;
        ADC->ADC_CR = ADC_CR_START;
        goto need_delay;
    }
    if (chsr != g.bit)
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
    ADC->ADC_CHDR = g.bit;
    return ADC->ADC_LCDR;
}

// Cancel a sample that may have been started with gpio_adc_sample()
void
gpio_adc_cancel_sample(struct gpio_adc g)
{
    irqstatus_t flag = irq_save();
    if ((ADC->ADC_CHSR & 0xffff) == g.bit)
        gpio_adc_read(g);
    irq_restore(flag);
}
