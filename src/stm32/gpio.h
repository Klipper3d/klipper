#ifndef __STM32_GPIO_H
#define __STM32_GPIO_H

#include <stdint.h> // uint32_t

struct gpio_out {
    void *regs;
    uint32_t bit;
};
struct gpio_out gpio_out_setup(uint32_t pin, uint32_t val);
void gpio_out_reset(struct gpio_out g, uint32_t val);
void gpio_out_toggle_noirq(struct gpio_out g);
void gpio_out_toggle(struct gpio_out g);
void gpio_out_write(struct gpio_out g, uint32_t val);

struct gpio_in {
    void *regs;
    uint32_t bit;
};
struct gpio_in gpio_in_setup(uint32_t pin, int32_t pull_up);
void gpio_in_reset(struct gpio_in g, int32_t pull_up);
uint8_t gpio_in_read(struct gpio_in g);

struct gpio_pwm {
  void *reg;
};
struct gpio_pwm gpio_pwm_setup(uint8_t pin, uint32_t cycle_time, uint8_t val);
void gpio_pwm_write(struct gpio_pwm g, uint32_t val);

struct gpio_adc {
    void *adc;
    uint32_t chan;
};
struct gpio_adc gpio_adc_setup(uint32_t pin);
uint32_t gpio_adc_sample(struct gpio_adc g);
uint16_t gpio_adc_read(struct gpio_adc g);
void gpio_adc_cancel_sample(struct gpio_adc g);

struct spi_config {
    void *spi;
    uint32_t spi_cr1;
};
struct spi_config spi_setup(uint32_t bus, uint8_t mode, uint32_t rate);
void spi_prepare(struct spi_config config);
void spi_transfer(struct spi_config config, uint8_t receive_data
                  , uint8_t len, uint8_t *data);

struct i2c_config {
    void *i2c;
    uint8_t addr;
};

struct i2c_config i2c_setup(uint32_t bus, uint32_t rate, uint8_t addr);
void i2c_write(struct i2c_config config, uint8_t write_len, uint8_t *write);
void i2c_read(struct i2c_config config, uint8_t reg_len, uint8_t *reg
              , uint8_t read_len, uint8_t *read);

struct sdio_config {
    void *sdio;
};
struct sdio_config sdio_setup(uint32_t bus);
void sdio_send_cmd(struct sdio_config sdio, uint8_t cmd, uint32_t argument, uint8_t wait);
uint8_t sdio_send_command(struct sdio_config sdio_config, uint8_t cmd, uint32_t argument, uint8_t wait, uint8_t *response_data, uint8_t *response_data_len);
uint8_t sdio_prepare_data_transfer(struct sdio_config sdio_config, uint8_t read, uint32_t numblocks, uint32_t blocksize);
uint8_t sdio_read_data(struct sdio_config sdio_config, uint8_t *data, uint32_t numblocks, uint32_t blocksize);
uint8_t sdio_write_data(struct sdio_config sdio_config, uint8_t *data, uint32_t numblocks, uint32_t blocksize);
void sdio_switch_bus_width_and_speed(struct sdio_config sdio_config, uint8_t width, uint8_t clkdiv);

#endif // gpio.h
