#include "gpio/gpio.h"
#include "command.h"
#include "hal/gpio_ll.h"

// @todo está ok? ._. Num max o num -1?
DECL_ENUMERATION_RANGE("pin", "GPIO_NUM_0", GPIO_NUM_0, GPIO_NUM_MAX);
