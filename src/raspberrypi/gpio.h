#ifndef RASPBERRYPI_GPIO_H_
#define RASPBERRYPI_GPIO_H_

#include <stdint.h> // uint8_t

int gpio_initialise(void);

struct gpio_out {
    uint32_t pin;
};
struct gpio_out gpio_out_setup(uint32_t pin, uint8_t val);
void gpio_out_reset(struct gpio_out g, uint8_t val);
void gpio_out_toggle_noirq(struct gpio_out g);
void gpio_out_toggle(struct gpio_out g);
void gpio_out_write(struct gpio_out g, uint8_t val);

struct gpio_in {
    uint32_t pin;
};
struct gpio_in gpio_in_setup(uint32_t pin, int8_t pull_up);
void gpio_in_reset(struct gpio_in g, int8_t pull_up);
uint8_t gpio_in_read(struct gpio_in g);

#endif // RASPBERRYPI_GPIO_H_
