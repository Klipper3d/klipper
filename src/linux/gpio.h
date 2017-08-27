#ifndef __LINUX_GPIO_H
#define __LINUX_GPIO_H

#include <stdint.h> // uint8_t

struct gpio_adc {
    int fd;
};
struct gpio_adc gpio_adc_setup(uint8_t pin);
uint32_t gpio_adc_sample(struct gpio_adc g);
uint16_t gpio_adc_read(struct gpio_adc g);
void gpio_adc_cancel_sample(struct gpio_adc g);

#endif // gpio.h
