// GPIO functions on STM32F1
//
// Copyright (C) 2018 Grigori Goronzy <greg@kinoho.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/irq.h" // irq_save
#include "command.h" // shutdown
#include "compiler.h" // ARRAY_SIZE
#include "gpio.h" // gpio_out_setup
#include "internal.h" // GPIO
#include "stm32f1xx_ll_gpio.h" // LL_GPIO_SetPinMode
#include "sched.h" // sched_shutdown


/****************************************************************
 * Pin mappings
 ****************************************************************/

DECL_ENUMERATION_RANGE("pin", "PA0", GPIO('A', 0), 16);
DECL_ENUMERATION_RANGE("pin", "PB0", GPIO('B', 0), 16);
DECL_ENUMERATION_RANGE("pin", "PC0", GPIO('C', 0), 16);
DECL_ENUMERATION_RANGE("pin", "PD0", GPIO('D', 0), 16);
DECL_ENUMERATION_RANGE("pin", "PE0", GPIO('E', 0), 16);

GPIO_TypeDef *const digital_regs[] = {
    GPIOA, GPIOB, GPIOC, GPIOD, GPIOE
};

uint32_t const digital_pins[] = {
    LL_GPIO_PIN_0,
    LL_GPIO_PIN_1,
    LL_GPIO_PIN_2,
    LL_GPIO_PIN_3,
    LL_GPIO_PIN_4,
    LL_GPIO_PIN_5,
    LL_GPIO_PIN_6,
    LL_GPIO_PIN_7,
    LL_GPIO_PIN_8,
    LL_GPIO_PIN_9,
    LL_GPIO_PIN_10,
    LL_GPIO_PIN_11,
    LL_GPIO_PIN_12,
    LL_GPIO_PIN_13,
    LL_GPIO_PIN_14,
    LL_GPIO_PIN_15,
};

/****************************************************************
 * General Purpose Input Output (GPIO) pins
 ****************************************************************/

struct gpio_out
gpio_out_setup(uint8_t pin, uint8_t val)
{
    if (GPIO2PORT(pin) >= ARRAY_SIZE(digital_regs))
        goto fail;
    GPIO_TypeDef *regs = digital_regs[GPIO2PORT(pin)];
    uint32_t bit = digital_pins[pin % 16];
    struct gpio_out g = { .regs=regs, .bit=bit };
    gpio_out_reset(g, val);
    return g;
fail:
    shutdown("Not an output pin");
}

void
gpio_out_reset(struct gpio_out g, uint8_t val)
{
    irqstatus_t flag = irq_save();
    if (val)
        LL_GPIO_SetOutputPin(g.regs, g.bit);
    else
        LL_GPIO_ResetOutputPin(g.regs, g.bit);
    LL_GPIO_SetPinMode(g.regs, g.bit, LL_GPIO_MODE_OUTPUT);
    irq_restore(flag);
}

void
gpio_out_toggle_noirq(struct gpio_out g)
{
    LL_GPIO_TogglePin(g.regs, g.bit);
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
    if (val)
        LL_GPIO_SetOutputPin(g.regs, g.bit);
    else
        LL_GPIO_ResetOutputPin(g.regs, g.bit);
}


struct gpio_in
gpio_in_setup(uint8_t pin, int8_t pull_up)
{
    if (GPIO2PORT(pin) >= ARRAY_SIZE(digital_regs))
        goto fail;
    GPIO_TypeDef *regs = digital_regs[GPIO2PORT(pin)];
    uint32_t bit = digital_pins[pin % 16];
    struct gpio_in g = { .regs = regs, .bit = bit };
    gpio_in_reset(g, pull_up);
    return g;
fail:
    shutdown("Not an input pin");
}

void
gpio_in_reset(struct gpio_in g, int8_t pull_up)
{
    irqstatus_t flag = irq_save();
    if (pull_up) {
        LL_GPIO_SetPinMode(g.regs, g.bit, LL_GPIO_MODE_INPUT);
        uint32_t p = pull_up > 0 ? LL_GPIO_PULL_UP : LL_GPIO_PULL_DOWN;
        LL_GPIO_SetPinPull(g.regs, g.bit, p);
    } else {
        LL_GPIO_SetPinMode(g.regs, g.bit, LL_GPIO_MODE_FLOATING);
    }
    irq_restore(flag);
}

uint8_t
gpio_in_read(struct gpio_in g)
{
    return LL_GPIO_IsInputPinSet(g.regs, g.bit);
}
