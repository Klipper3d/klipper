#include "gpio/gpio.h"
#include "command.h"
#include "hal/gpio_ll.h"

volatile gpio_dev_t* gpio_dev = NULL;

void gpio_init()
{
    gpio_dev = GPIO_LL_GET_HW(GPIO_PORT_0);
}
DECL_INIT(gpio_init);




// @todo está ok? ._. Num max o num -1?
DECL_ENUMERATION_RANGE("pin", "GPIO_NUM_0", GPIO_NUM_0, GPIO_NUM_MAX);
