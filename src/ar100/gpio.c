// GPIO functions on simulator.
//
// Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/gpio.h" // gpio_out_write

struct gpio_out gpio_out_setup(uint8_t pin, uint8_t val) {
    return (struct gpio_out){.pin=pin};
}
void gpio_out_reset(struct gpio_out g, uint8_t val) {
}
void gpio_out_toggle_noirq(struct gpio_out g) {
}
void gpio_out_toggle(struct gpio_out g) {
}
void gpio_out_write(struct gpio_out g, uint8_t val) {
}
struct gpio_in gpio_in_setup(uint8_t pin, int8_t pull_up) {
    return (struct gpio_in){.pin=pin};
}
void gpio_in_reset(struct gpio_in g, int8_t pull_up) {
}
uint8_t gpio_in_read(struct gpio_in g) {
    return 0;
}