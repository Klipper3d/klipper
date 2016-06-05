#ifndef __GENERIC_GPIO_H
#define __GENERIC_GPIO_H

#include <stdint.h>

struct gpio_out {
    uint8_t pin;
};
struct gpio_out gpio_out_setup(uint8_t pin, uint8_t val);
void gpio_out_toggle(struct gpio_out g);
void gpio_out_write(struct gpio_out g, uint8_t val);

struct gpio_in {
    uint8_t pin;
};
struct gpio_in gpio_in_setup(uint8_t pin, int8_t pull_up);
uint8_t gpio_in_read(struct gpio_in g);

struct gpio_pwm {
    uint8_t pin;
};
struct gpio_pwm gpio_pwm_setup(uint8_t pin, uint32_t cycle_time, uint8_t val);
void gpio_pwm_write(struct gpio_pwm g, uint8_t val);

struct gpio_adc {
    uint8_t pin;
};
struct gpio_adc gpio_adc_setup(uint8_t pin);
uint32_t gpio_adc_sample_time(void);
uint8_t gpio_adc_sample(struct gpio_adc g);
void gpio_adc_clear_sample(struct gpio_adc g);
uint16_t gpio_adc_read(struct gpio_adc g);

void spi_config(void);
void spi_transfer(char *data, uint8_t len);

#endif // gpio.h
