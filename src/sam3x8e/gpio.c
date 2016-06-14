// GPIO functions on sam3x8e
//
// Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stdint.h> // uint32_t
#include "command.h" // shutdown
#include "compiler.h" // ARRAY_SIZE
#include "gpio.h" // gpio_out_setup
#include "irq.h" // irq_save
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
 * gpio functions
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
    uint32_t bit = GPIO2BIT(pin);
    irqstatus_t flag = irq_save();
    if (val)
        regs->PIO_SODR = bit;
    else
        regs->PIO_CODR = bit;
    regs->PIO_OER = bit;
    regs->PIO_OWER = bit;
    regs->PIO_PER = bit;
    irq_restore(flag);
    return (struct gpio_out){ .regs=regs, .bit=bit };
fail:
    shutdown("Not an output pin");
}

void
gpio_out_toggle(struct gpio_out g)
{
    Pio *regs = g.regs;
    regs->PIO_ODSR ^= g.bit;
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
    uint32_t bit = GPIO2BIT(pin);
    irqstatus_t flag = irq_save();
    PMC->PMC_PCER0 = 1 << (ID_PIOA + port);
    if (pull_up)
        regs->PIO_PUER = bit;
    else
        regs->PIO_PUDR = bit;
    regs->PIO_ODR = bit;
    regs->PIO_PER = bit;
    irq_restore(flag);
    return (struct gpio_in){ .regs=regs, .bit=bit };
fail:
    shutdown("Not an output pin");
}

uint8_t
gpio_in_read(struct gpio_in g)
{
    Pio *regs = g.regs;
    return !!(regs->PIO_PDSR & g.bit);
}
