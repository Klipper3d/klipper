#ifndef __SAM4E8E_GPIO_H
#define __SAM4E8E_GPIO_H

#include <stdint.h>

struct gpio_out {
    uint8_t pin;
    void *regs;
    uint32_t bit;
};

void gpio_set_peripheral(char bank, uint32_t bit, char ptype, uint32_t pull_up);
struct gpio_out gpio_out_setup(uint8_t pin, uint8_t val);
void gpio_out_reset(struct gpio_out g, uint8_t val);
void gpio_out_toggle_noirq(struct gpio_out g);
void gpio_out_toggle(struct gpio_out g);
void gpio_out_write(struct gpio_out g, uint8_t val);

struct gpio_in {
    uint8_t pin;
    void *regs;
    uint32_t bit;
};

struct gpio_in gpio_in_setup(uint8_t pin, int8_t pull_up);
void gpio_in_reset(struct gpio_in g, int8_t pull_up);
uint8_t gpio_in_read(struct gpio_in g);

struct gpio_adc {
    uint8_t pin;
    void *afec;
    uint32_t chan;
};

struct gpio_adc gpio_pin_to_afec(uint8_t pin);
struct gpio_adc gpio_adc_setup(uint8_t pin);
uint32_t gpio_adc_sample(struct gpio_adc g);
uint16_t gpio_adc_read(struct gpio_adc g);
void gpio_adc_cancel_sample(struct gpio_adc g);

struct spi_config {
    void *sspi;
    uint32_t cfg;
};
struct spi_config spi_setup(uint32_t bus, uint8_t mode, uint32_t rate);
void spi_transfer(struct spi_config config, uint8_t receive_data
                  , uint8_t len, uint8_t *data);
void spi_prepare(struct spi_config config);

struct i2c_config {
    void *twi;
    uint8_t addr;
};

struct i2c_config i2c_setup(uint32_t bus, uint32_t rate, uint8_t addr);
void i2c_write(struct i2c_config config,
                uint8_t write_len, uint8_t *write);
void i2c_read(struct i2c_config config,
              uint8_t reg_len, uint8_t *reg,
              uint8_t read_len, uint8_t *read);
#endif // gpio.h
