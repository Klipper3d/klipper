#include "gpio/gpio_out.h"
#include "command.h"
#include "driver/gpio.h"
#include "hal/gpio_ll.h"

struct gpio_out gpio_out_setup(gpio_num_t pin, uint_fast8_t val)
{
    if (pin >= GPIO_NUM_MAX) {
        shutdown("Output pin outside of range");
    }

    struct gpio_out gpio = { .pin = pin };
    gpio_out_reset(gpio, val);
    return gpio;
}

void gpio_out_reset(struct gpio_out gpio, uint_fast8_t val)
{
    // We also need to keep the input enabled to be able to read it when toggling
    gpio_pullup_dis(gpio.pin);
    gpio_ll_input_enable(GPIO_LL_GET_HW(GPIO_PORT_0), gpio.pin);

    gpio_ll_set_level(GPIO_LL_GET_HW(GPIO_PORT_0), gpio.pin, val);
    gpio_ll_output_enable(GPIO_LL_GET_HW(GPIO_PORT_0), gpio.pin);
}

void gpio_out_toggle_noirq(struct gpio_out gpio)
{
    gpio_ll_set_level(GPIO_LL_GET_HW(GPIO_PORT_0), gpio.pin, !gpio_ll_get_level(GPIO_LL_GET_HW(GPIO_PORT_0), gpio.pin));
}

void gpio_out_toggle(struct gpio_out gpio)
{
    gpio_out_toggle_noirq(gpio);
}

void gpio_out_write(struct gpio_out gpio, uint_fast8_t val)
{
    gpio_ll_set_level(GPIO_LL_GET_HW(GPIO_PORT_0), gpio.pin, val);
}
