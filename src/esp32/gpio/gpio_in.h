#pragma once

#include "gpio/gpio.h"

struct gpio_in gpio_in_setup(gpio_num_t pin, int_fast8_t pull_up);
void gpio_in_reset(struct gpio_in gpio, int_fast8_t pull_up);
uint_fast8_t gpio_in_read(struct gpio_in gpio);
