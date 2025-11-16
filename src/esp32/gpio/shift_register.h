#pragma once

#include "gpio/gpio.h"
#include "autoconf.h"
#include <stdbool.h>

#define SR_GPIO_BITMASK (1ul << 7)

void sr_write();
void gpio_sr_write(struct gpio gpio, bool val);

/**
 * This function should be optimized out if the feature is
 * disabled (CONFIG_HAVE_GPIO_SR == false)
 */
static inline bool __attribute__((always_inline)) gpio_is_sr(struct gpio gpio)
{
    return CONFIG_HAVE_GPIO_SR && gpio.pin & SR_GPIO_BITMASK;
}
