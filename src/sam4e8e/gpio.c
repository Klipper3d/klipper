// SAM4e8e GPIO port
//
// Copyright (C) 2018  Florian Heilmann <Florian.Heilmann@gmx.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/irq.h" // irq_save
#include "command.h" // shutdown
#include "gpio.h" // gpio_out_setup
#include "internal.h" // gpio_peripheral
#include "sam4e.h" // Pio
#include "sched.h" // sched_shutdown

static Pio * const digital_regs[] = {
    PIOA, PIOB, PIOC, PIOD, PIOE
};


/****************************************************************
 * Pin multiplexing
 ****************************************************************/

void
gpio_peripheral(uint32_t gpio, char ptype, int32_t pull_up)
{
    uint32_t bank = GPIO2PORT(gpio), bit = GPIO2BIT(gpio);
    Pio *regs = digital_regs[bank];
    regs ->PIO_IDR = bit;

    // Enable peripheral for pin
    uint32_t sr;

    switch (ptype) {
    case 'A':
        sr = regs->PIO_ABCDSR[0];
        regs->PIO_ABCDSR[0] &= (~bit & sr);
        sr = regs->PIO_ABCDSR[1];
        regs->PIO_ABCDSR[1] &= (~bit & sr);
        break;
    case 'B':
        sr = regs->PIO_ABCDSR[0];
        regs->PIO_ABCDSR[0] = (bit | sr);
        sr = regs->PIO_ABCDSR[1];
        regs->PIO_ABCDSR[1] &= (~bit & sr);
        break;
    case 'C':
        sr = regs->PIO_ABCDSR[0];
        regs->PIO_ABCDSR[0] &= (~bit & sr);
        sr = regs->PIO_ABCDSR[1];
        regs->PIO_ABCDSR[1] = (bit | sr);
        break;
    case 'D':
        sr = regs->PIO_ABCDSR[0];
        regs->PIO_ABCDSR[0] = (bit | sr);
        sr = regs->PIO_ABCDSR[1];
        regs->PIO_ABCDSR[1] = (bit | sr);
        break;
    }

    // Disable pin in IO controller
    regs->PIO_PDR = bit;

    // Set pullup
    if (pull_up > 0) {
        regs->PIO_PUER = bit;
    } else {
        regs->PIO_PUDR = bit;
    }
}


/****************************************************************
 * General Purpose Input Output (GPIO) pins
 ****************************************************************/

struct gpio_out
gpio_out_setup(uint8_t pin, uint8_t val)
{
    if (GPIO2PORT(pin) >= ARRAY_SIZE(digital_regs))
        goto fail;
    uint32_t port = GPIO2PORT(pin);
    Pio *regs = digital_regs[port];
    uint32_t bank_id = ID_PIOA + port;
    if ((PMC->PMC_PCSR0 & (1u << bank_id)) == 0) {
        PMC->PMC_PCER0 = 1 << bank_id;
    }
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
    Pio *regs = digital_regs[port];
    uint32_t bank_id = ID_PIOA + port;
    if ((PMC->PMC_PCSR0 & (1u << bank_id)) == 0) {
        PMC->PMC_PCER0 = 1 << bank_id;
    }
    struct gpio_in g = { .regs=regs, .bit=GPIO2BIT(pin) };
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
    regs->PIO_IDR = g.bit;
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
