// GPIO with shift register support
//
// Copyright (C) 2023  <Your Name>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "gpio_sr.h" // self
#include "board/gpio.h" // gpio_out_setup
#include "board/irq.h" // irq_save
#include "command.h" // shutdown
#include "sched.h" // sched_shutdown
#include "shift_register.h" // shift_register_oid_lookup

// Setup a GPIO output pin, potentially on a shift register
struct gpio_out_extended
gpio_out_sr_setup(uint8_t pin, uint8_t val, uint8_t sr_oid)
{
    struct gpio_out_extended g;
    
    if (sr_oid == 0) {
        // Regular GPIO pin
        struct gpio_out regular = gpio_out_setup(pin, val);
        g.regs = regular.regs;
        g.bit = regular.bit;
        g.is_sr_pin = 0;
    } else {
        // Shift register pin
        struct shift_register *sr = shift_register_oid_lookup(sr_oid);
        if (!sr)
            shutdown("Invalid shift register OID");
        
        g.sr = sr;
        g.pin = pin;
        g.is_sr_pin = 1;
        
        // Set initial value
        shift_register_set_pin(sr, pin, val);
    }
    
    return g;
}

// Write a value to a GPIO output pin
void
gpio_out_sr_write(struct gpio_out_extended g, uint8_t val)
{
    if (g.is_sr_pin) {
        // Shift register pin
        shift_register_set_pin(g.sr, g.pin, val);
    } else {
        // Regular GPIO pin
        irqstatus_t flag = irq_save();
        struct gpio_out regular = { .regs = g.regs, .bit = g.bit };
        gpio_out_write(regular, val);
        irq_restore(flag);
    }
}

// Toggle a GPIO output pin
void
gpio_out_sr_toggle(struct gpio_out_extended g)
{
    if (g.is_sr_pin) {
        // Shift register pin
        uint8_t val = shift_register_get_pin(g.sr, g.pin);
        shift_register_set_pin(g.sr, g.pin, !val);
    } else {
        // Regular GPIO pin
        irqstatus_t flag = irq_save();
        struct gpio_out regular = { .regs = g.regs, .bit = g.bit };
        gpio_out_toggle(regular);
        irq_restore(flag);
    }
}

// Toggle a GPIO output pin without IRQ
void
gpio_out_sr_toggle_noirq(struct gpio_out_extended g)
{
    if (g.is_sr_pin) {
        // Shift register pin
        uint8_t val = shift_register_get_pin(g.sr, g.pin);
        shift_register_set_pin(g.sr, g.pin, !val);
    } else {
        // Regular GPIO pin
        struct gpio_out regular = { .regs = g.regs, .bit = g.bit };
        gpio_out_toggle_noirq(regular);
    }
}