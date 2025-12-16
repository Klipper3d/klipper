// GPIO functions on rp2040
//
// Copyright (C) 2021-2025  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <string.h> // ffs
#include "board/irq.h" // irq_save
#include "command.h" // shutdown
#include "gpio.h" // gpio_out_setup
#include "hardware/structs/iobank0.h" // iobank0_hw
#include "hardware/structs/padsbank0.h" // padsbank0_hw
#include "hardware/structs/sio.h" // sio_hw
#include "internal.h" // gpio_peripheral
#include "sched.h" // sched_shutdown


/****************************************************************
 * Pin mappings
 ****************************************************************/

#define NUM_GPIO (CONFIG_MACH_RP2040 ? 30 : 48)

DECL_ENUMERATION_RANGE("pin", "gpio0", 0, NUM_GPIO);

// Set the mode and extended function of a pin
void
gpio_peripheral(uint32_t gpio, int func, int pull_up)
{
    if (gpio >= NUM_GPIO)
        shutdown("Not a valid pin");
    padsbank0_hw->io[gpio] = (
        PADS_BANK0_GPIO0_IE_BITS
        | (PADS_BANK0_GPIO0_DRIVE_VALUE_4MA << PADS_BANK0_GPIO0_DRIVE_MSB)
        | (pull_up > 0 ? PADS_BANK0_GPIO0_PUE_BITS : 0)
        | (pull_up < 0 ? PADS_BANK0_GPIO0_PDE_BITS : 0));
    iobank0_hw->io[gpio].ctrl = func << IO_BANK0_GPIO0_CTRL_FUNCSEL_LSB;
}

// Convert a register and bit location back to an integer pin identifier
static int
mask_to_pin(void *sio, uint32_t mask)
{
    int pin = ffs(mask)-1;
    if (CONFIG_MACH_RP2350 && sio != (void*)sio_hw)
        pin += 32;
    return pin;
}


/****************************************************************
 * General Purpose Input Output (GPIO) pins
 ****************************************************************/

struct gpio_out
gpio_out_setup(uint8_t pin, uint8_t val)
{
    if (pin >= NUM_GPIO)
        shutdown("Not a valid pin");
    void *sio = (void*)sio_hw;
    if (CONFIG_MACH_RP2350 && pin >= 32) {
        pin -= 32;
        sio += 4;
    }
    struct gpio_out g = { .sio=sio, .bit=1<<pin };
    gpio_out_reset(g, val);
    return g;
}

void
gpio_out_reset(struct gpio_out g, uint8_t val)
{
    int pin = mask_to_pin(g.sio, g.bit);
    irqstatus_t flag = irq_save();
    gpio_out_write(g, val);
    sio_hw_t *sio = g.sio;
    sio->gpio_oe_set = g.bit;
    gpio_peripheral(pin, 5, 0);
    irq_restore(flag);
}

void
gpio_out_toggle_noirq(struct gpio_out g)
{
    sio_hw_t *sio = g.sio;
    sio->gpio_togl = g.bit;
}

void
gpio_out_toggle(struct gpio_out g)
{
    gpio_out_toggle_noirq(g);
}

void
gpio_out_write(struct gpio_out g, uint8_t val)
{
    sio_hw_t *sio = g.sio;
    if (val)
        sio->gpio_set = g.bit;
    else
        sio->gpio_clr = g.bit;
}


struct gpio_in
gpio_in_setup(uint8_t pin, int8_t pull_up)
{
    if (pin >= NUM_GPIO)
        shutdown("Not a valid pin");
    void *sio = (void*)sio_hw;
    if (CONFIG_MACH_RP2350 && pin >= 32) {
        pin -= 32;
        sio += 4;
    }
    struct gpio_in g = { .sio=sio, .bit=1<<pin };
    gpio_in_reset(g, pull_up);
    return g;
}

void
gpio_in_reset(struct gpio_in g, int8_t pull_up)
{
    int pin = mask_to_pin(g.sio, g.bit);
    irqstatus_t flag = irq_save();
    gpio_peripheral(pin, 5, pull_up);
    sio_hw_t *sio = g.sio;
    sio->gpio_oe_clr = g.bit;
    irq_restore(flag);
}

uint8_t
gpio_in_read(struct gpio_in g)
{
    sio_hw_t *sio = g.sio;
    return !!(sio->gpio_in & g.bit);
}
