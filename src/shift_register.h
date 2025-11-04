// Support for 74HC595 shift registers
//
// Copyright (C) 2023  <Your Name>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#ifndef __SHIFT_REGISTER_H
#define __SHIFT_REGISTER_H

#include <stdint.h> // uint8_t
#include "board/gpio.h" // struct gpio_out

struct shift_register {
    struct gpio_out data_pin;
    struct gpio_out clock_pin;
    struct gpio_out latch_pin;
    uint8_t *state;
    uint8_t num_registers;
};

struct shift_register_pin {
    struct shift_register *sr;
    uint8_t pin;
};

struct shift_register *shift_register_setup(uint8_t oid, uint8_t data_pin, 
                                           uint8_t clock_pin, uint8_t latch_pin, 
                                           uint8_t num_registers);
struct shift_register *shift_register_oid_lookup(uint8_t oid);
void shift_register_update(struct shift_register *sr);
void shift_register_set_pin(struct shift_register *sr, uint8_t pin, uint8_t value);
uint8_t shift_register_get_pin(struct shift_register *sr, uint8_t pin);

void
command_config_shift_register(uint32_t *args);

#endif // shift_register.h