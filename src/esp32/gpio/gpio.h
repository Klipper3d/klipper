#pragma once

#include "soc/gpio_struct.h"
#include "soc/gpio_num.h"

extern volatile gpio_dev_t* gpio_dev;

struct gpio { gpio_num_t pin; };
#define gpio_in gpio
#define gpio_out gpio
