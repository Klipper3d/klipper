// Copyright (C) 2020  Elias Bakken <elias@iagent.no>
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stdint.h>
#define R_PIO_BASE  0x01F02C00
#define PIO_BASE    0x01C20800

enum pin_func {
  PIO_INPUT,
  PIO_OUTPUT,
  PIO_ALT1,
  PIO_ALT2,
  PIO_ALT3,
  PIO_ALT4,
  PIO_ALT5,
  PIO_DISABLE
};

struct gpio_mux {
  uint8_t pin;
  uint32_t reg;
};

struct gpio_out {
  uint8_t pin;
  uint32_t reg;
};

struct gpio_in {
  uint8_t pin;
  uint32_t reg;
};

struct gpio_mux gpio_mux_setup(uint8_t pin, enum pin_func func);

struct gpio_out gpio_out_setup(uint8_t pin, uint8_t val);
void gpio_out_write(struct gpio_out pin, uint8_t val);
void gpio_out_toggle_noirq(struct gpio_out pin);

struct gpio_in gpio_in_setup(uint8_t pin, uint8_t val);
uint8_t gpio_in_read(struct gpio_in pin);
