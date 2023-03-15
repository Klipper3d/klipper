// I2C functions on AVR
//
// Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <avr/io.h> // TWCR
#include "autoconf.h" // CONFIG_CLOCK_FREQ
#include "board/misc.h" // timer_is_before
#include "command.h" // shutdown
#include "gpio.h" // i2c_setup
#include "internal.h" // GPIO
#include "sched.h" // sched_shutdown

DECL_ENUMERATION("i2c_bus", "twi", 0);

#if CONFIG_MACH_atmega168 || CONFIG_MACH_atmega328 || CONFIG_MACH_atmega328p
static const uint8_t SCL = GPIO('C', 5), SDA = GPIO('C', 4);
DECL_CONSTANT_STR("BUS_PINS_twi", "PC5,PC4");
#elif CONFIG_MACH_atmega644p || CONFIG_MACH_atmega1284p
static const uint8_t SCL = GPIO('C', 0), SDA = GPIO('C', 1);
DECL_CONSTANT_STR("BUS_PINS_twi", "PC0,PC1");
#elif CONFIG_MACH_at90usb1286 || CONFIG_MACH_at90usb646 \
      || CONFIG_MACH_atmega32u4 || CONFIG_MACH_atmega1280 \
      || CONFIG_MACH_atmega2560
static const uint8_t SCL = GPIO('D', 0), SDA = GPIO('D', 1);
DECL_CONSTANT_STR("BUS_PINS_twi", "PD0,PD1");
#endif

struct i2c_config
i2c_setup(uint32_t bus, uint32_t rate, uint8_t addr)
{
    if (bus)
        shutdown("Unsupported i2c bus");

    if (!(TWCR & (1<<TWEN))) {
        // Setup output pins and enable pullups
        gpio_out_setup(SDA, 1);
        gpio_out_setup(SCL, 1);

        // Set frequency avoiding pulling in integer divide
        TWSR = 0;
        if (rate >= 400000)
            TWBR = ((CONFIG_CLOCK_FREQ / 400000) - 16) / 2;
        else
            TWBR = ((CONFIG_CLOCK_FREQ / 100000) - 16) / 2;

        // Enable interface
        TWCR = (1<<TWEN);
    }
    return (struct i2c_config){ .addr=addr<<1 };
}

static void
i2c_wait(uint32_t timeout)
{
    for (;;) {
        if (TWCR & (1<<TWINT))
            break;
        if (!timer_is_before(timer_read_time(), timeout))
            shutdown("i2c timeout");
    }
}

static void
i2c_start(uint32_t timeout)
{
    TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWSTA);
    i2c_wait(timeout);
    uint32_t status = TWSR;
    if (status != 0x10 && status != 0x08)
        shutdown("Failed to send i2c start");
}

static void
i2c_send_byte(uint8_t b, uint32_t timeout)
{
    TWDR = b;
    TWCR = (1<<TWEN) | (1<<TWINT);
    i2c_wait(timeout);
}

static void
i2c_receive_byte(uint8_t *read, uint32_t timeout, uint8_t send_ack)
{
    TWCR = (1<<TWEN) | (1<<TWINT) | ((send_ack?1:0)<<TWEA);
    i2c_wait(timeout);
    *read = TWDR;
}

static void
i2c_stop(uint32_t timeout)
{
    TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWSTO);
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
    uint32_t timeout = timer_read_time() + timer_from_us(5000);
    i2c_start(timeout);
    i2c_send_byte(config.addr, timeout);
    while (reg_len--)
        i2c_send_byte(*reg++, timeout);
    i2c_start(timeout);
    i2c_send_byte(config.addr | 0x1, timeout);
    while (read_len--)
        i2c_receive_byte(read++, timeout, read_len);
    i2c_stop(timeout);
}
