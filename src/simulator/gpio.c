// GPIO functions on simulator.
//
// Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/gpio.h" // gpio_out_write

struct gpio_out gpio_out_setup(uint8_t pin, uint8_t val) {
    return (struct gpio_out){.pin=pin};
}
void gpio_out_reset(struct gpio_out g, uint8_t val) {
}
void gpio_out_toggle_noirq(struct gpio_out g) {
}
void gpio_out_toggle(struct gpio_out g) {
}
void gpio_out_write(struct gpio_out g, uint8_t val) {
}
struct gpio_in gpio_in_setup(uint8_t pin, int8_t pull_up) {
    return (struct gpio_in){.pin=pin};
}
void gpio_in_reset(struct gpio_in g, int8_t pull_up) {
}
uint8_t gpio_in_read(struct gpio_in g) {
    return 0;
}
struct gpio_pwm gpio_pwm_setup(uint8_t pin, uint32_t cycle_time, uint8_t val) {
    return (struct gpio_pwm){.pin=pin};
}
void gpio_pwm_write(struct gpio_pwm g, uint8_t val) {
}
struct gpio_adc gpio_adc_setup(uint8_t pin) {
    return (struct gpio_adc){.pin=pin};
}
uint32_t gpio_adc_sample(struct gpio_adc g) {
    return 0;
}
uint16_t gpio_adc_read(struct gpio_adc g) {
    return 0;
}
void gpio_adc_cancel_sample(struct gpio_adc g) {
}

struct spi_config
spi_setup(uint32_t bus, uint8_t mode, uint32_t rate)
{
    return (struct spi_config){ };
}
void
spi_prepare(struct spi_config config)
{
}
void
spi_transfer(struct spi_config config, uint8_t receive_data
             , uint8_t len, uint8_t *data)
{
}
