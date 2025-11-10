#pragma once

#include "gpio/gpio.h"

struct gpio_out gpio_out_setup(gpio_num_t pin, uint_fast8_t val);
void gpio_out_reset(struct gpio_out gpio, uint_fast8_t val);
void gpio_out_toggle_noirq(struct gpio_out gpio);
void gpio_out_toggle(struct gpio_out gpio);
void gpio_out_write(struct gpio_out gpio, uint_fast8_t val);
