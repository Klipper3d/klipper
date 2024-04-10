// I2C functions on stm32
//
// Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h" // CONFIG_MACH_STM32F1
#include "board/misc.h" // timer_is_before
#include "command.h" // shutdown
#include "gpio.h" // i2c_setup
#include "internal.h" // GPIO
#include "sched.h" // sched_shutdown

struct i2c_info {
    I2C_TypeDef *i2c;
    uint8_t scl_pin, sda_pin;
};

DECL_ENUMERATION_RANGE("i2c_bus", "i2c1", 0, 2);
DECL_CONSTANT_STR("BUS_PINS_i2c1", "PB6,PB7");
DECL_CONSTANT_STR("BUS_PINS_i2c2", "PB10,PB11");

static const struct i2c_info i2c_bus[] = {
    { I2C1, GPIO('B', 6), GPIO('B', 7) },
    { I2C2, GPIO('B', 10), GPIO('B', 11) },
};

// Work around stm32 errata causing busy bit to be stuck
static void
i2c_busy_errata(uint8_t scl_pin, uint8_t sda_pin)
{
    if (! CONFIG_MACH_STM32F1)
        return;
    gpio_peripheral(scl_pin, GPIO_OUTPUT | GPIO_OPEN_DRAIN, 1);
    gpio_peripheral(sda_pin, GPIO_OUTPUT | GPIO_OPEN_DRAIN, 1);
    gpio_peripheral(sda_pin, GPIO_OUTPUT | GPIO_OPEN_DRAIN, -1);
    gpio_peripheral(scl_pin, GPIO_OUTPUT | GPIO_OPEN_DRAIN, -1);
    gpio_peripheral(scl_pin, GPIO_OUTPUT | GPIO_OPEN_DRAIN, 1);
    gpio_peripheral(sda_pin, GPIO_OUTPUT | GPIO_OPEN_DRAIN, 1);
}

struct i2c_config
i2c_setup(uint32_t bus, uint32_t rate, uint8_t addr)
{
    // Lookup requested i2c bus
    if (bus >= ARRAY_SIZE(i2c_bus))
        shutdown("Unsupported i2c bus");
    const struct i2c_info *ii = &i2c_bus[bus];
    I2C_TypeDef *i2c = ii->i2c;

    if (!is_enabled_pclock((uint32_t)i2c)) {
        // Enable i2c clock and gpio
        enable_pclock((uint32_t)i2c);
        i2c_busy_errata(ii->scl_pin, ii->sda_pin);
        gpio_peripheral(ii->scl_pin, GPIO_FUNCTION(4) | GPIO_OPEN_DRAIN, 1);
        gpio_peripheral(ii->sda_pin, GPIO_FUNCTION(4) | GPIO_OPEN_DRAIN, 1);
        i2c->CR1 = I2C_CR1_SWRST;
        i2c->CR1 = 0;

        // Set 100Khz frequency and enable
        uint32_t pclk = get_pclock_frequency((uint32_t)i2c);
        i2c->CR2 = pclk / 1000000;
        i2c->CCR = pclk / 100000 / 2;
        i2c->TRISE = (pclk / 1000000) + 1;
        i2c->CR1 = I2C_CR1_PE;
    }

    return (struct i2c_config){ .i2c=i2c, .addr=addr<<1 };
}

static uint32_t
i2c_wait(I2C_TypeDef *i2c, uint32_t set, uint32_t clear, uint32_t timeout)
{
    for (;;) {
        uint32_t sr1 = i2c->SR1;
        if ((sr1 & set) == set && (sr1 & clear) == 0)
            return sr1;
        if (!timer_is_before(timer_read_time(), timeout))
            shutdown("i2c timeout");
    }
}

static void
i2c_start(I2C_TypeDef *i2c, uint8_t addr, uint32_t timeout)
{
    i2c->CR1 = I2C_CR1_START | I2C_CR1_PE;
    i2c_wait(i2c, I2C_SR1_SB, 0, timeout);
    i2c->DR = addr;
    i2c_wait(i2c, I2C_SR1_ADDR, 0, timeout);
    uint32_t sr2 = i2c->SR2;
    if (!(sr2 & I2C_SR2_MSL))
        shutdown("Failed to send i2c addr");
}

static void
i2c_send_byte(I2C_TypeDef *i2c, uint8_t b, uint32_t timeout)
{
    i2c->DR = b;
    i2c_wait(i2c, I2C_SR1_TXE, 0, timeout);
}

static void
i2c_stop(I2C_TypeDef *i2c, uint32_t timeout)
{
    i2c->CR1 = I2C_CR1_STOP | I2C_CR1_PE;
    i2c_wait(i2c, 0, I2C_SR1_TXE, timeout);
}

void
i2c_write(struct i2c_config config, uint8_t write_len, uint8_t *write)
{
    I2C_TypeDef *i2c = config.i2c;
    uint32_t timeout = timer_read_time() + timer_from_us(5000);

    i2c_start(i2c, config.addr, timeout);
    while (write_len--)
        i2c_send_byte(i2c, *write++, timeout);
    i2c_stop(i2c, timeout);
}

void
i2c_read(struct i2c_config config, uint8_t reg_len, uint8_t *reg
         , uint8_t read_len, uint8_t *read)
{
    shutdown("i2c_read not supported on stm32");
}
