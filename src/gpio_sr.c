// Generic GPIO with shift register support
//
// Copyright (C) 2023  <Your Name>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "gpio_sr.h"           // self
#include "board/gpio.h"        // gpio_out_setup, gpio_out_write, gpio_out_toggle
#include "board/irq.h"         // irq_save, irq_restore
#include "command.h"           // shutdown
#include "sched.h"             // sched_shutdown
#include "shift_register.h"    // shift_register_oid_lookup, shift_register_set_pin

// Setup a GPIO output pin, potentially on a shift register
struct gpio_out_extended
gpio_out_sr_setup(uint8_t pin, uint8_t val, uint8_t sr_oid)
{
    struct gpio_out_extended g;

    if (sr_oid == 0) {
        // Regular MCU GPIO pin
        g.is_sr_pin = 0;
        g.gpio = gpio_out_setup(pin, val);
    } else {
        // Shift register pin
        struct shift_register *sr = shift_register_oid_lookup(sr_oid);
        if (!sr)
            shutdown("Invalid shift register OID");
        g.is_sr_pin = 1;
        g.sr  = sr;
        g.pin = pin;
        shift_register_set_pin(sr, pin, val);
    }

    return g;
}

// Write a value to an extended GPIO output pin
void
gpio_out_sr_write(struct gpio_out_extended g, uint8_t val)
{
    if (g.is_sr_pin) {
        shift_register_set_pin(g.sr, g.pin, val);
    } else {
        irqstatus_t flag = irq_save();
        gpio_out_write(g.gpio, val);
        irq_restore(flag);
    }
}

// Toggle an extended GPIO output pin (IRQ-safe)
void
gpio_out_sr_toggle(struct gpio_out_extended g)
{
    if (g.is_sr_pin) {
        uint8_t val = shift_register_get_pin(g.sr, g.pin);
        shift_register_set_pin(g.sr, g.pin, !val);
    } else {
        irqstatus_t flag = irq_save();
        gpio_out_toggle(g.gpio);
        irq_restore(flag);
    }
}

// Toggle an extended GPIO output pin without saving/restoring IRQ state
void
gpio_out_sr_toggle_noirq(struct gpio_out_extended g)
{
    if (g.is_sr_pin) {
        uint8_t val = shift_register_get_pin(g.sr, g.pin);
        shift_register_set_pin(g.sr, g.pin, !val);
    } else {
        gpio_out_toggle_noirq(g.gpio);
    }
}
