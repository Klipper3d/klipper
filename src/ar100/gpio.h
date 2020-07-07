// Copyright (C) 2020  Elias Bakken <elias@iagent.no>
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stdint.h>

struct gpio_out {
  uint8_t pin;
  uint8_t val;
};

void gpio_out_write(struct gpio_out pin, uint8_t val);
struct gpio_out gpio_out_setup(uint8_t pin, uint8_t val);
void gpio_out_toggle_noirq(struct gpio_out pin);