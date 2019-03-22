// GPIO functions on sam3/sam4
//
// Copyright (C) 2016-2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/irq.h" // irq_save
#include "command.h" // shutdown
#include "compiler.h" // ARRAY_SIZE
#include "gpio.h" // gpio_out_setup
#include "internal.h" // gpio_peripheral
#include "sched.h" // sched_shutdown

DECL_ENUMERATION_RANGE("pin", "PA0", GPIO('A', 0), 32);
DECL_ENUMERATION_RANGE("pin", "PB0", GPIO('B', 0), 32);
#ifdef PIOC
DECL_ENUMERATION_RANGE("pin", "PC0", GPIO('C', 0), 32);
#endif
#ifdef PIOD
DECL_ENUMERATION_RANGE("pin", "PD0", GPIO('D', 0), 32);
#endif
#ifdef PIOE
DECL_ENUMERATION_RANGE("pin", "PE0", GPIO('E', 0), 32);
#endif

static Pio * const digital_regs[] = {
    PIOA, PIOB,
#ifdef PIOC
    PIOC,
#endif
#ifdef PIOD
    PIOD,
#endif
#ifdef PIOE
    PIOE,
#endif
};


/****************************************************************
 * Pin multiplexing
 ****************************************************************/

static void
set_pull_up(Pio *regs, uint32_t bit, int32_t pull_up)
{
#if CONFIG_MACH_SAM3X
    if (pull_up > 0)
        regs->PIO_PUER = bit;
    else
        regs->PIO_PUDR = bit;
#else
    if (pull_up > 0) {
        regs->PIO_PPDDR = bit;
        regs->PIO_PUER = bit;
    } else if (pull_up < 0) {
        regs->PIO_PUDR = bit;
        regs->PIO_PPDER = bit;
    } else {
        regs->PIO_PUDR = bit;
        regs->PIO_PPDDR = bit;
    }
#endif
}

void
gpio_peripheral(uint32_t gpio, char ptype, int32_t pull_up)
{
    uint32_t bank = GPIO2PORT(gpio), bit = GPIO2BIT(gpio), pt = ptype - 'A';
    Pio *regs = digital_regs[bank];

#if CONFIG_MACH_SAM3X
    regs->PIO_ABSR = (regs->PIO_ABSR & ~bit) | (pt & 0x01 ? bit : 0);
#else
    regs->PIO_ABCDSR[0] = (regs->PIO_ABCDSR[0] & ~bit) | (pt & 0x01 ? bit : 0);
    regs->PIO_ABCDSR[1] = (regs->PIO_ABCDSR[1] & ~bit) | (pt & 0x02 ? bit : 0);
#endif
    set_pull_up(regs, bit, pull_up);
    regs->PIO_PDR = bit;
}


/****************************************************************
 * General Purpose Input Output (GPIO) pins
 ****************************************************************/

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
    set_pull_up(regs, g.bit, 0);
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
    if (CONFIG_MACH_SAM3X && pull_up < 0)
        goto fail;
    uint32_t port = GPIO2PORT(pin);
    enable_pclock(ID_PIOA + port);
    struct gpio_in g = { .regs=digital_regs[port], .bit=GPIO2BIT(pin) };
    gpio_in_reset(g, pull_up);
    return g;
fail:
    shutdown("Not a valid input pin");
}

void
gpio_in_reset(struct gpio_in g, int8_t pull_up)
{
    Pio *regs = g.regs;
    irqstatus_t flag = irq_save();
    set_pull_up(regs, g.bit, pull_up);
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
