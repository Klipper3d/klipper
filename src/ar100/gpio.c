// GPIO functions on simulator.
//
// Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "command.h" // shutdown
#include "gpio.h" // gpio_out_write

struct gpio_out gpio_out_setup(uint8_t pin, uint8_t val) {
    if(pin > 8)
      goto fail;
    // TODO: add setup for pins > 8
    uint32_t cfg_reg = PE_CFG0;
    uint8_t bit_pos = 1 << (pin*4);
    write_reg(cfg_reg, bit_pos);
    struct gpio_out out = {.reg=PE_DATA, .pin=pin, .val=val};
    gpio_out_write(out, val);

    return out;
fail:
    shutdown("Not an output pin");
}
void gpio_out_reset(struct gpio_out g, uint8_t val) {
}
void gpio_out_toggle_noirq(struct gpio_out g) {
}
void gpio_out_toggle(struct gpio_out g) {
}
void gpio_out_write(struct gpio_out g, uint8_t val) {
  write_reg(g.reg, val);
}
struct gpio_in gpio_in_setup(uint8_t pin, int8_t pull_up) {
    return (struct gpio_in){.pin=pin};
}
void gpio_in_reset(struct gpio_in g, int8_t pull_up) {
}
uint8_t gpio_in_read(struct gpio_in g) {
    return 0;
}