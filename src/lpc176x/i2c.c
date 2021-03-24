// I2C functions on lpc176x
//
// Copyright (C) 2018-2021  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/misc.h" // timer_is_before
#include "command.h" // DECL_COMMAND
#include "gpio.h" // i2c_setup
#include "internal.h" // gpio_peripheral
#include "sched.h" // sched_shutdown

struct i2c_info {
    LPC_I2C_TypeDef *i2c;
    uint8_t scl_pin, sda_pin, function, pclk;
};

DECL_ENUMERATION("i2c_bus", "i2c1", 0);
DECL_ENUMERATION("i2c_bus", "i2c1a", 1);
DECL_ENUMERATION("i2c_bus", "i2c0", 2);
DECL_ENUMERATION("i2c_bus", "i2c2", 3);
DECL_CONSTANT_STR("BUS_PINS_i2c1", "P0.1,P0.0");
DECL_CONSTANT_STR("BUS_PINS_i2c1a", "P0.20,P0.19");
DECL_CONSTANT_STR("BUS_PINS_i2c0", "P0.28,P0.27");
DECL_CONSTANT_STR("BUS_PINS_i2c2", "P0.11,P0.10");

static const struct i2c_info i2c_bus[] = {
    { LPC_I2C1, GPIO(0, 1), GPIO(0, 0), 3, PCLK_I2C1 },
    { LPC_I2C1, GPIO(0, 20), GPIO(0, 19), 3, PCLK_I2C1 },
    { LPC_I2C0, GPIO(0, 28), GPIO(0, 27), 1, PCLK_I2C0 },
    { LPC_I2C2, GPIO(0, 11), GPIO(0, 10), 2, PCLK_I2C2 },
};

// i2c connection status flags
enum {
    IF_START = 1<<5, IF_STOP = 1<<4, IF_IRQ = 1<<3, IF_ACK = 1<<2, IF_ENA = 1<<6
};

struct i2c_config
i2c_setup(uint32_t bus, uint32_t rate, uint8_t addr)
{
    if (bus >= ARRAY_SIZE(i2c_bus))
        shutdown("Invalid i2c bus");

    const struct i2c_info *info = &i2c_bus[bus];
    LPC_I2C_TypeDef *i2c = info->i2c;
    static uint8_t have_run_init;
    if (!(have_run_init & (1 << bus))) {
        have_run_init |= 1 << bus;

        // Init pins
        gpio_peripheral(info->scl_pin, info->function, 0);
        gpio_peripheral(info->sda_pin, info->function, 0);

        // Set 100Khz frequency
        enable_pclock(info->pclk);
        uint32_t pclk = get_pclock_frequency(info->pclk);
        uint32_t pulse = pclk / (100000 * 2);
        i2c->I2SCLL = pulse;
        i2c->I2SCLH = pulse;

        // Enable interface
        i2c->I2CONCLR = IF_START | IF_IRQ | IF_ACK | IF_ENA;
        i2c->I2CONSET = IF_ENA;
    }

    return (struct i2c_config){ .i2c=i2c, .addr=addr<<1 };
}

static void
i2c_wait(LPC_I2C_TypeDef *i2c, uint32_t bit, uint32_t timeout)
{
    for (;;) {
        uint32_t flags = i2c->I2CONSET;
        if (flags & bit)
            break;
        if (!timer_is_before(timer_read_time(), timeout))
            shutdown("i2c timeout");
    }
}

static void
i2c_start(LPC_I2C_TypeDef *i2c, uint32_t timeout)
{
    i2c->I2CONCLR = IF_ACK | IF_IRQ | IF_START;
    i2c->I2CONSET = IF_ACK | IF_START;
    i2c_wait(i2c, IF_IRQ, timeout);
    uint32_t status = i2c->I2STAT;
    if (status != 0x10 && status != 0x08)
        shutdown("Failed to send i2c start");
    i2c->I2CONCLR = IF_START;
}

static uint32_t
i2c_send_byte(LPC_I2C_TypeDef *i2c, uint8_t b, uint32_t timeout)
{
    i2c->I2DAT = b;
    i2c->I2CONCLR = IF_IRQ;
    i2c_wait(i2c, IF_IRQ, timeout);
    return i2c->I2STAT;
}

static void
i2c_stop(LPC_I2C_TypeDef *i2c, uint32_t timeout)
{
    i2c->I2CONSET = IF_STOP;
    i2c->I2CONCLR = IF_IRQ;
    i2c_wait(i2c, IF_STOP, timeout);
}

void
i2c_write(struct i2c_config config, uint8_t write_len, uint8_t *write)
{
    LPC_I2C_TypeDef *i2c = config.i2c;
    uint32_t timeout = timer_read_time() + timer_from_us(5000);

    i2c_start(i2c, timeout);
    i2c_send_byte(i2c, config.addr, timeout);
    while (write_len--)
        i2c_send_byte(i2c, *write++, timeout);
    i2c_stop(i2c, timeout);
}

void
i2c_read(struct i2c_config config, uint8_t reg_len, uint8_t *reg
         , uint8_t read_len, uint8_t *read)
{
    shutdown("i2c_read not supported on lpc176x");
}
