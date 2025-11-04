// GPIO with shift register support
//
// Copyright (C) 2023  <Your Name>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#ifndef __GPIO_SR_H
#define __GPIO_SR_H

#include <stdint.h> // uint8_t
#include "shift_register.h" // struct shift_register

// Extended GPIO output structure that can handle both regular GPIO and shift register pins
struct gpio_out_extended {
    union {
        struct {
            void *regs;
            uint8_t bit;
        };
        struct {
            struct shift_register *sr;
            uint8_t pin;
            uint8_t is_sr_pin;
        };
    };
};

// Setup a GPIO output pin, potentially on a shift register
struct gpio_out_extended gpio_out_sr_setup(uint8_t pin, uint8_t val, uint8_t sr_oid);

// Write a value to a GPIO output pin
void gpio_out_sr_write(struct gpio_out_extended g, uint8_t val);

// Toggle a GPIO output pin
void gpio_out_sr_toggle(struct gpio_out_extended g);

// Toggle a GPIO output pin without IRQ
void gpio_out_sr_toggle_noirq(struct gpio_out_extended g);

#endif // gpio_sr.h