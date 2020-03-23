// GPIO functions on simulator.
//
// Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "gpio.h" // gpio_out_write
#include <stdio.h>
#include <gpiod.h>
#include <syslog.h>

static uint32_t vals[1000];

struct gpio_out gpio_out_setup(uint8_t pin, uint8_t val) {
    vals[pin] = val;
    gpiod_ctxless_set_value("gpiochip1", pin, val, 0, "ar100", NULL, NULL);
    return (struct gpio_out){.pin=pin};
}
void gpio_out_reset(struct gpio_out g, uint8_t val) {
  vals[g.pin] = val;
  gpiod_ctxless_set_value("gpiochip1", g.pin, val, 0, "ar100", NULL, NULL);
}
void gpio_out_toggle_noirq(struct gpio_out g) {
  gpio_out_write(g, vals[g.pin] ? 0 : 1);
}
void gpio_out_toggle(struct gpio_out g) {
  gpio_out_toggle_noirq(g);
}
void gpio_out_write(struct gpio_out g, uint8_t val) {
  gpiod_ctxless_set_value("gpiochip1", g.pin, val, 0, "ar100", NULL, NULL);
  vals[g.pin] = val;
}
struct gpio_in gpio_in_setup(uint8_t pin, int8_t pull_up) {
  return (struct gpio_in){.pin=pin};
}
void gpio_in_reset(struct gpio_in g, int8_t pull_up) {
  gpio_in_setup(g.pin, 0);
}
uint8_t gpio_in_read(struct gpio_in g) {
    return gpiod_ctxless_get_value("gpiochip0", g.pin, 0, "ar100");
}
