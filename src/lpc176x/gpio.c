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
#include "sched.h" // sched_shutdown

#include <LPC17xx.h>
#include <lpc17xx_pinsel.h>
#include <lpc17xx_gpio.h>
#include <lpc17xx_clkpwr.h>

#ifndef _BV
#define _BV(_b) (1 << (_b))
#endif


/****************************************************************
 * Pin mappings
 ****************************************************************/
#define NUM_PORTS      5
#define MAX_PIN_VALUE  (NUM_PORTS * 32)
/*
static LPC_GPIO_TypeDef * const digital_regs[] = {
    LPC_GPIO0, // 'A.xx' --> 0.0 .. 0.31
    LPC_GPIO1, // 'B.xx' --> 1.0 .. 1.31
    LPC_GPIO2, // 'C.xx' --> 2.0 .. 2.31
    LPC_GPIO3, // 'D.xx' --> 3.0 .. 3.31
    LPC_GPIO4  // 'E.xx' --> 4.0 .. 4.31
};
*/

/****************************************************************
 * General Purpose Input Output (GPIO) pins
 ****************************************************************/

void
gpio_peripheral(_gpio_peripheral_t const * const ptr,
                uint8_t const pull_up)
{
    PINSEL_CFG_Type cfg;
    cfg.Portnum   = ptr->port; // PINSEL_PORT_0 ... 4
    cfg.Pinnum    = ptr->pin;
    cfg.Funcnum   = ptr->func; // PINSEL_FUNC_0 ... 3
    if (pull_up)
        cfg.Pinmode = PINSEL_PINMODE_PULLUP;
    else
        cfg.Pinmode = PINSEL_PINMODE_PULLDOWN;
    cfg.OpenDrain = PINSEL_PINMODE_NORMAL;
    PINSEL_ConfigPin(&cfg);
}


struct gpio_out
gpio_out_setup(uint8_t pin, uint8_t val)
{
    if (pin >= MAX_PIN_VALUE)
        goto fail;
    uint32_t const port_idx = GPIO2PORT(pin);
    uint32_t const pin_idx = GPIO2PIN(pin);
    uint32_t const bit = _BV(pin_idx);
    LPC_GPIO_TypeDef * const regs = &LPC_GPIO0[port_idx];
    PINSEL_CFG_Type cfg;
    cfg.Portnum   = port_idx;
    cfg.Pinnum    = pin_idx;
    cfg.Funcnum   = PINSEL_FUNC_0; // GPIO
    cfg.Pinmode   = PINSEL_PINMODE_PULLDOWN;
    cfg.OpenDrain = PINSEL_PINMODE_NORMAL;

    irqstatus_t flag = irq_save();
    PINSEL_ConfigPin(&cfg);
    regs->FIODIR |= bit; // Mark as output
    irq_restore(flag);
    if (val)
        regs->FIOSET = bit;
    else
        regs->FIOCLR = bit;
    return (struct gpio_out){ .regs=regs, .bit=bit };
fail:
    shutdown("Not an output pin");
}

void
gpio_out_toggle(struct gpio_out g)
{
    LPC_GPIO_TypeDef * const regs = g.regs;
    if (regs->FIOPIN & g.bit)
        regs->FIOCLR = g.bit;
    else
        regs->FIOSET = g.bit;
}

void
gpio_out_write(struct gpio_out g, uint8_t val)
{
    LPC_GPIO_TypeDef * const regs = g.regs;
    if (val)
        regs->FIOSET = g.bit;
    else
        regs->FIOCLR = g.bit;
}


struct gpio_in
gpio_in_setup(uint8_t pin, int8_t pull_up)
{
    if (pin >= MAX_PIN_VALUE)
        goto fail;
    uint32_t const port_idx = GPIO2PORT(pin);
    uint32_t const pin_idx = GPIO2PIN(pin);
    LPC_GPIO_TypeDef * const regs = &LPC_GPIO0[port_idx];
    PINSEL_CFG_Type cfg;
    cfg.Portnum   = port_idx;
    cfg.Pinnum    = pin_idx;
    cfg.Funcnum   = PINSEL_FUNC_0; // GPIO
    if (pull_up)
        cfg.Pinmode = PINSEL_PINMODE_PULLUP;
    else
        cfg.Pinmode = PINSEL_PINMODE_PULLDOWN;
    cfg.OpenDrain = PINSEL_PINMODE_NORMAL;

    irqstatus_t flag = irq_save();
    PINSEL_ConfigPin(&cfg);
    regs->FIODIR &= ~(_BV(pin_idx)); // mark as input
    irq_restore(flag);
    return (struct gpio_in){ .regs=regs, .bit=_BV(pin_idx) };
fail:
    shutdown("Not an input pin");
}

uint8_t
gpio_in_read(struct gpio_in g)
{
    LPC_GPIO_TypeDef * const regs = g.regs;
    return !!(regs->FIOPIN & g.bit);
}
