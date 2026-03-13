// Generic GPIO with shift register support
//
// Copyright (C) 2025  Hans-Albert Maritz <maritz.hans@gmail.com>
// Copyright (C) 2026  Michael Atzmüller <michael.atzmueller98@gmail.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#ifndef __GPIO_SR_H
#define __GPIO_SR_H

#include <stdint.h>        // uint8_t
#include "board/gpio.h"    // struct gpio_out
#include "shift_register.h" // struct shift_register

// Extended GPIO output structure that can handle both regular GPIO and
// shift register pins.  Stores the full opaque struct gpio_out so it
// works identically on every architecture.
struct gpio_out_extended {
    uint8_t is_sr_pin;
    struct gpio_out gpio;    // used when is_sr_pin == 0
    struct shift_register *sr; // used when is_sr_pin == 1
    uint8_t pin;             // bit index within the shift register
};

// Setup a GPIO output pin, potentially on a shift register.
// sr_oid == 0 means a regular MCU pin; sr_oid > 0 selects a shift register.
struct gpio_out_extended gpio_out_sr_setup(uint8_t pin, uint8_t val,
                                           uint8_t sr_oid);

// Write a value to an extended GPIO output pin
void gpio_out_sr_write(struct gpio_out_extended g, uint8_t val);

// Toggle an extended GPIO output pin (IRQ-safe)
void gpio_out_sr_toggle(struct gpio_out_extended g);

// Toggle an extended GPIO output pin (no IRQ save/restore)
void gpio_out_sr_toggle_noirq(struct gpio_out_extended g);

#endif // gpio_sr.h
