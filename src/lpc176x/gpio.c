// GPIO functions on lpc176x
//
// Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "LPC17xx.h" // LPC_PINCON
#include "board/irq.h" // irq_save
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


struct gpio_out
gpio_out_setup(uint8_t pin, uint8_t val)
{
    if (GPIO2PORT(pin) >= ARRAY_SIZE(digital_regs))
        goto fail;
    LPC_GPIO_TypeDef *regs = digital_regs[GPIO2PORT(pin)];
    uint32_t bit = GPIO2BIT(pin);
    irqstatus_t flag = irq_save();
    if (val)
        regs->FIOSET = bit;
    else
        regs->FIOCLR = bit;
    regs->FIODIR |= bit;
    irq_restore(flag);
    return (struct gpio_out){ .regs=regs, .bit=bit };
fail:
    shutdown("Not an output pin");
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
    uint32_t port = GPIO2PORT(pin);
    LPC_GPIO_TypeDef *regs = digital_regs[port];
    uint32_t bit = GPIO2BIT(pin);
    irqstatus_t flag = irq_save();
    gpio_peripheral(port, pin % 32, 0, pull_up);
    regs->FIODIR &= ~bit;
    irq_restore(flag);
    return (struct gpio_in){ .regs=regs, .bit=bit };
fail:
    shutdown("Not an input pin");
}

uint8_t
gpio_in_read(struct gpio_in g)
{
    LPC_GPIO_TypeDef *regs = g.regs;
    return !!(regs->FIOPIN & g.bit);
}
