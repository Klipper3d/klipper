// I2C functions on lpc176x
//
// Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "LPC17xx.h" // LPC_I2C1
#include "board/misc.h" // timer_is_before
#include "command.h" // DECL_COMMAND
#include "gpio.h" // i2c_setup
#include "internal.h" // gpio_peripheral
#include "sched.h" // sched_shutdown

DECL_ENUMERATION("i2c_bus", "i2c1", 0);
DECL_CONSTANT_STR("BUS_PINS_i2c1", "P0.1,P0.0");

// i2c connection status flags
enum {
    IF_START = 1<<5, IF_STOP = 1<<4, IF_IRQ = 1<<3, IF_ACK = 1<<2, IF_ENA = 1<<6
};

static void
i2c_init(void)
{
    static int have_run_init;
    if (have_run_init)
        return;
    have_run_init = 1;

    // Init i2c bus 1 pins
    gpio_peripheral(GPIO(0, 1), 3, 0);
    gpio_peripheral(GPIO(0, 0), 3, 0);

    // Set 100Khz frequency
    enable_pclock(PCLK_I2C1);
    uint32_t pclk = SystemCoreClock, pulse = pclk / (100000 * 2);
    LPC_I2C1->I2SCLL = pulse;
    LPC_I2C1->I2SCLH = pulse;

    // Enable interface
    LPC_I2C1->I2CONCLR = IF_START | IF_IRQ | IF_ACK | IF_ENA;
    LPC_I2C1->I2CONSET = IF_ENA;
}

struct i2c_config
i2c_setup(uint32_t bus, uint32_t rate, uint8_t addr)
{
    if (bus)
        shutdown("Unsupported i2c bus");
    i2c_init();
    return (struct i2c_config){ .addr=addr<<1 };
}

static void
i2c_wait(uint32_t bit, uint32_t timeout)
{
    for (;;) {
        uint32_t flags = LPC_I2C1->I2CONSET;
        if (flags & bit)
            break;
        if (!timer_is_before(timer_read_time(), timeout))
            shutdown("i2c timeout");
    }
}

static void
i2c_start(uint32_t timeout)
{
    LPC_I2C1->I2CONCLR = IF_ACK | IF_IRQ | IF_START;
    LPC_I2C1->I2CONSET = IF_ACK | IF_START;
    i2c_wait(IF_IRQ, timeout);
    uint32_t status = LPC_I2C1->I2STAT;
    if (status != 0x10 && status != 0x08)
        shutdown("Failed to send i2c start");
    LPC_I2C1->I2CONCLR = IF_START;
}

static uint32_t
i2c_send_byte(uint8_t b, uint32_t timeout)
{
    LPC_I2C1->I2DAT = b;
    LPC_I2C1->I2CONCLR = IF_IRQ;
    i2c_wait(IF_IRQ, timeout);
    return LPC_I2C1->I2STAT;
}

static void
i2c_stop(uint32_t timeout)
{
    LPC_I2C1->I2CONSET = IF_STOP;
    LPC_I2C1->I2CONCLR = IF_IRQ;
    i2c_wait(IF_STOP, timeout);
}

void
i2c_write(struct i2c_config config, uint8_t write_len, uint8_t *write)
{
    uint32_t timeout = timer_read_time() + timer_from_us(5000);

    i2c_start(timeout);
    i2c_send_byte(config.addr, timeout);
    while (write_len--)
        i2c_send_byte(*write++, timeout);
    i2c_stop(timeout);
}

void
i2c_read(struct i2c_config config, uint8_t reg_len, uint8_t *reg
         , uint8_t read_len, uint8_t *read)
{
    shutdown("i2c_read not supported on lpc176x");
}
