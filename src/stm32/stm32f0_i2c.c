// I2C functions on stm32f0
//
// Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/misc.h" // timer_is_before
#include "command.h" // shutdown
#include "gpio.h" // i2c_setup
#include "internal.h" // GPIO
#include "sched.h" // sched_shutdown

struct i2c_info {
    I2C_TypeDef *i2c;
    uint8_t scl_pin, sda_pin, function;
};

DECL_ENUMERATION("i2c_bus", "i2c1", 0);
DECL_CONSTANT_STR("BUS_PINS_i2c1", "PB6,PB7");
DECL_ENUMERATION("i2c_bus", "i2c1a", 1);
DECL_CONSTANT_STR("BUS_PINS_i2c1a", "PF1,PF0");

static const struct i2c_info i2c_bus[] = {
    { I2C1, GPIO('B', 6), GPIO('B', 7), GPIO_FUNCTION(1) },
    { I2C1, GPIO('F', 1), GPIO('F', 0), GPIO_FUNCTION(1) },
};

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
        gpio_peripheral(ii->scl_pin, ii->function | GPIO_OPEN_DRAIN, 1);
        gpio_peripheral(ii->sda_pin, ii->function | GPIO_OPEN_DRAIN, 1);

        // Set 100Khz frequency and enable
        i2c->TIMINGR = ((0xB << I2C_TIMINGR_PRESC_Pos)
                        | (0x13 << I2C_TIMINGR_SCLL_Pos)
                        | (0xF << I2C_TIMINGR_SCLH_Pos)
                        | (0x2 << I2C_TIMINGR_SDADEL_Pos)
                        | (0x4 << I2C_TIMINGR_SCLDEL_Pos));
        i2c->CR1 = I2C_CR1_PE;
    }

    return (struct i2c_config){ .i2c=i2c, .addr=addr<<1 };
}

static uint32_t
i2c_wait(I2C_TypeDef *i2c, uint32_t set, uint32_t timeout)
{
    for (;;) {
        uint32_t isr = i2c->ISR;
        if (isr & set)
            return isr;
        if (!timer_is_before(timer_read_time(), timeout))
            shutdown("i2c timeout");
    }
}

void
i2c_write(struct i2c_config config, uint8_t write_len, uint8_t *write)
{
    I2C_TypeDef *i2c = config.i2c;
    uint32_t timeout = timer_read_time() + timer_from_us(5000);

    // Send start and address
    i2c->CR2 = (I2C_CR2_START | config.addr | (write_len << I2C_CR2_NBYTES_Pos)
                | I2C_CR2_AUTOEND);
    while (write_len--) {
        i2c_wait(i2c, I2C_ISR_TXIS, timeout);
        i2c->TXDR = *write++;
    }
    i2c_wait(i2c, I2C_ISR_TXE, timeout);
}

void
i2c_read(struct i2c_config config, uint8_t reg_len, uint8_t *reg
         , uint8_t read_len, uint8_t *read)
{
    shutdown("i2c_read not supported on stm32");
}
