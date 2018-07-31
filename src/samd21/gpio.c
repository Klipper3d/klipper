// samd21 gpio functions
//
// Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/irq.h" // irq_save
#include "command.h" // shutdown
#include "gpio.h" // gpio_out_setup
#include "internal.h" // gpio_peripheral
#include "samd21.h" // PORT
#include "sched.h" // sched_shutdown


/****************************************************************
 * Pin multiplexing
 ****************************************************************/

void
gpio_peripheral(char bank, uint32_t bit, char ptype, uint32_t pull_up)
{
    int group = bank == 'A' ? 0 : 1;
    PortGroup *pg = &PORT->Group[group];
    if (ptype) {
        volatile uint8_t *pmux = &pg->PMUX[bit/2].reg;
        uint8_t shift = (bit & 1) ? 4 : 0, mask = ~(0xf << shift);
        *pmux = (*pmux & mask) | ((ptype - 'A') << shift);
    }
    pg->PINCFG[bit].reg = ((ptype ? PORT_PINCFG_PMUXEN : 0)
                           | (pull_up ? PORT_PINCFG_PULLEN : 0));
}


/****************************************************************
 * General Purpose Input Output (GPIO) pins
 ****************************************************************/

#define GPIO(PORT, NUM) (((PORT)-'A') * 32 + (NUM))
#define GPIO2PORT(PIN) ((PIN) / 32)
#define GPIO2NUM(PIN) ((PIN) % 32)
#define GPIO2BIT(PIN) (1<<((PIN) % 32))
#define NUM_PORT 2

struct gpio_out
gpio_out_setup(uint8_t pin, uint8_t val)
{
    if (GPIO2PORT(pin) >= NUM_PORT)
        goto fail;
    PortGroup *pg = &PORT->Group[GPIO2PORT(pin)];
    uint32_t bit = GPIO2BIT(pin);
    irqstatus_t flag = irq_save();
    if (val)
        pg->OUTSET.reg = bit;
    else
        pg->OUTCLR.reg = bit;
    pg->DIRSET.reg = bit;
    pg->PINCFG[GPIO2NUM(pin)].reg = 0;
    irq_restore(flag);
    return (struct gpio_out){ .regs=pg, .bit=bit };
fail:
    shutdown("Not an output pin");
}

void
gpio_out_toggle_noirq(struct gpio_out g)
{
    PortGroup *pg = g.regs;
    pg->OUTTGL.reg = g.bit;
}

void
gpio_out_toggle(struct gpio_out g)
{
    gpio_out_toggle_noirq(g);
}

void
gpio_out_write(struct gpio_out g, uint8_t val)
{
    PortGroup *pg = g.regs;
    if (val)
        pg->OUTSET.reg = g.bit;
    else
        pg->OUTCLR.reg = g.bit;
}


struct gpio_in
gpio_in_setup(uint8_t pin, int8_t pull_up)
{
    if (GPIO2PORT(pin) >= NUM_PORT)
        goto fail;
    PortGroup *pg = &PORT->Group[GPIO2PORT(pin)];
    uint32_t bit = GPIO2BIT(pin);
    irqstatus_t flag = irq_save();
    pg->PINCFG[GPIO2NUM(pin)].reg = pull_up > 0 ? PORT_PINCFG_PULLEN : 0;
    pg->DIRCLR.reg = bit;
    irq_restore(flag);
    return (struct gpio_in){ .regs=pg, .bit=bit };
fail:
    shutdown("Not an input pin");
}

uint8_t
gpio_in_read(struct gpio_in g)
{
    PortGroup *pg = g.regs;
    return !!(pg->IN.reg & g.bit);
}
