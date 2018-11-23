// GPIO functions on lpc176x
//
// Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <string.h> // ffs
#include "LPC17xx.h" // LPC_PINCON
#include "autoconf.h" // CONFIG_CLOCK_FREQ
#include "board/irq.h" // irq_save
#include "board/misc.h" // timer_from_us
#include "command.h" // shutdown
#include "gpio.h" // gpio_out_setup
#include "internal.h" // gpio_peripheral
#include "sched.h" // sched_shutdown


/****************************************************************
 * Pin mappings
 ****************************************************************/

#define GPIO(PORT, NUM) ((PORT) * 32 + (NUM))
#define GPIO2PORT(PIN) ((PIN) / 32)
#define GPIO2BIT(PIN) (1<<((PIN) % 32))

static LPC_GPIO_TypeDef * const digital_regs[] = {
    LPC_GPIO0, LPC_GPIO1, LPC_GPIO2, LPC_GPIO3, LPC_GPIO4
};


/****************************************************************
 * General Purpose Input Output (GPIO) pins
 ****************************************************************/

// Enable a peripheral clock
void
enable_peripheral_clock(uint32_t pclk)
{
    LPC_SC->PCONP |= 1<<pclk;
    if (pclk < 16) {
        uint32_t shift = pclk * 2;
        LPC_SC->PCLKSEL0 = (LPC_SC->PCLKSEL0 & ~(0x3<<shift)) | (0x1<<shift);
    } else {
        uint32_t shift = (pclk - 16) * 2;
        LPC_SC->PCLKSEL1 = (LPC_SC->PCLKSEL1 & ~(0x3<<shift)) | (0x1<<shift);
    }
}

// Set the mode and extended function of a pin
void
gpio_peripheral(int bank, int pin, int func, int pullup)
{
    uint32_t bank_pos = bank * 2, pin_pos = pin * 2;
    if (pin_pos >= 32) {
        pin_pos -= 32;
        bank_pos++;
    }
    uint32_t sel_bits = (func & 0x03) << pin_pos, mask = ~(0x03 << pin_pos);
    uint32_t mode_bits = (pullup ? 0x00 : 0x02) << pin_pos;
    volatile uint32_t *pinsel = &LPC_PINCON->PINSEL0;
    volatile uint32_t *pinmode = &LPC_PINCON->PINMODE0;
    irqstatus_t flag = irq_save();
    pinsel[bank_pos] = (pinsel[bank_pos] & mask) | sel_bits;
    pinmode[bank_pos] = (pinmode[bank_pos] & mask) | mode_bits;
    irq_restore(flag);
}

// Convert a register and bit location back to an integer pin identifier
static int
regs_to_pin(LPC_GPIO_TypeDef *regs, uint32_t bit)
{
    int i;
    for (i=0; i<ARRAY_SIZE(digital_regs); i++)
        if (digital_regs[i] == regs)
            return GPIO(i, ffs(bit)-1);
    return 0;
}

struct gpio_out
gpio_out_setup(uint8_t pin, uint8_t val)
{
    if (GPIO2PORT(pin) >= ARRAY_SIZE(digital_regs))
        goto fail;
    LPC_GPIO_TypeDef *regs = digital_regs[GPIO2PORT(pin)];
    struct gpio_out g = { .regs=regs, .bit=GPIO2BIT(pin) };
    gpio_out_reset(g, val);
    return g;
fail:
    shutdown("Not an output pin");
}

void
gpio_out_reset(struct gpio_out g, uint8_t val)
{
    LPC_GPIO_TypeDef *regs = g.regs;
    int pin = regs_to_pin(regs, g.bit);
    irqstatus_t flag = irq_save();
    if (val)
        regs->FIOSET = g.bit;
    else
        regs->FIOCLR = g.bit;
    regs->FIODIR |= g.bit;
    gpio_peripheral(GPIO2PORT(pin), pin % 32, 0, 0);
    irq_restore(flag);
}

void
gpio_out_toggle_noirq(struct gpio_out g)
{
    LPC_GPIO_TypeDef *regs = g.regs;
    regs->FIOPIN = regs->FIOSET ^ g.bit;
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
    LPC_GPIO_TypeDef *regs = g.regs;
    if (val)
        regs->FIOSET = g.bit;
    else
        regs->FIOCLR = g.bit;
}


struct gpio_in
gpio_in_setup(uint8_t pin, int8_t pull_up)
{
    if (GPIO2PORT(pin) >= ARRAY_SIZE(digital_regs))
        goto fail;
    LPC_GPIO_TypeDef *regs = digital_regs[GPIO2PORT(pin)];
    struct gpio_in g = { .regs=regs, .bit=GPIO2BIT(pin) };
    gpio_in_reset(g, pull_up);
    return g;
fail:
    shutdown("Not an input pin");
}

void
gpio_in_reset(struct gpio_in g, int8_t pull_up)
{
    LPC_GPIO_TypeDef *regs = g.regs;
    int pin = regs_to_pin(regs, g.bit);
    irqstatus_t flag = irq_save();
    gpio_peripheral(GPIO2PORT(pin), pin % 32, 0, pull_up);
    regs->FIODIR &= ~g.bit;
    irq_restore(flag);
}

uint8_t
gpio_in_read(struct gpio_in g)
{
    LPC_GPIO_TypeDef *regs = g.regs;
    return !!(regs->FIOPIN & g.bit);
}


/****************************************************************
 * Analog to Digital Converter (ADC) pins
 ****************************************************************/

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
    if (!(LPC_SC->PCONP & (1<<PCLK_ADC))) {
        // Power up ADC
        enable_peripheral_clock(PCLK_ADC);
        LPC_ADC->ADCR = adcr;
    }

    gpio_peripheral(GPIO2PORT(pin), pin % 32, adc_pin_funcs[chan], 0);

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
