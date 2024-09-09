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
#include "i2ccmds.h" // struct i2cdev_s

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
    uint8_t us = 23;
    if (bus)
        shutdown("Unsupported i2c bus");

    if (!(TWCR & (1<<TWEN))) {
        // Setup output pins and enable pullups
        gpio_out_setup(SDA, 1);
        gpio_out_setup(SCL, 1);

        // Set frequency avoiding pulling in integer divide
        TWSR = 0;
        if (rate >= 400000) {
            TWBR = ((CONFIG_CLOCK_FREQ / 400000) - 16) / 2;
        } else {
            TWBR = ((CONFIG_CLOCK_FREQ / 100000) - 16) / 2;
            us = 90;
        }
        // Enable interface
        TWCR = (1<<TWEN);
    }
    return (struct i2c_config){.addr = addr << 1, .pause = timer_from_us(us)};
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

static uint_fast8_t i2c_async_read(struct timer *timer);

static uint_fast8_t i2c_async_read_wait(struct timer *timer)
{
    struct i2cdev_s *i2c_slv = container_of(timer, struct i2cdev_s, timer);
    struct i2c_config *config = &i2c_slv->i2c_config;

    if (TWCR & (1 << TWINT)) {
        i2c_slv->buf[i2c_slv->cur] = TWDR;
        i2c_slv->cur++;
        if (i2c_slv->data_len[0] == 0) {
            TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWSTO);
            timer->func = i2c_slv->callback;
        }
        return i2c_async_read(timer);
    }

    timer->waketime = timer_read_time() + config->pause;
    return SF_RESCHEDULE;
}

static uint_fast8_t i2c_async_read(struct timer *timer)
{
    struct i2cdev_s *i2c_slv = container_of(timer, struct i2cdev_s, timer);
    struct i2c_config *config = &i2c_slv->i2c_config;

    i2c_slv->data_len[0]--;
    TWCR = (1<<TWEN) | (1<<TWINT) | ((i2c_slv->data_len[0] ? 1 : 0)<<TWEA);

    timer->func = i2c_async_read_wait;
    timer->waketime = timer_read_time() + config->pause;
    return SF_RESCHEDULE;
}

static uint_fast8_t i2c_async_read_start_wait(struct timer *timer)
{
    struct i2cdev_s *i2c_slv = container_of(timer, struct i2cdev_s, timer);
    struct i2c_config *config = &i2c_slv->i2c_config;
    if (TWCR & (1 << TWINT)) {
        timer->func = i2c_async_read;
        return i2c_async_read(timer);
    }

    timer->waketime = timer_read_time() + config->pause;
    return SF_RESCHEDULE;
}

static uint_fast8_t i2c_async_start(struct timer *timer);

static uint_fast8_t i2c_async_write_end(struct timer *timer)
{
    struct i2cdev_s *i2c_slv = container_of(timer, struct i2cdev_s, timer);
    struct i2c_config *config = &i2c_slv->i2c_config;
    if (TWCR & (1 << TWINT)) {
        if (i2c_slv->data_len[0] & I2C_R) {
            return i2c_async_start(timer);
        }
        TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWSTO);
        timer->func = i2c_async;
    }

    timer->waketime = timer_read_time() + config->pause;
    return SF_RESCHEDULE;
}

static uint_fast8_t i2c_async_write(struct timer *timer)
{
    struct i2cdev_s *i2c_slv = container_of(timer, struct i2cdev_s, timer);
    struct i2c_config *config = &i2c_slv->i2c_config;
    if (TWCR & (1 << TWINT)) {
        TWDR = i2c_buf_read_b(i2c_slv);
        TWCR = (1 << TWEN) | (1 << TWINT);
        i2c_slv->data_len[0]--;
        if (i2c_slv->data_len[0] == 0) {
            i2c_cmd_done(i2c_slv);
            timer->func = i2c_async_write_end;
        }
    }

    timer->waketime = timer_read_time() + config->pause;
    return SF_RESCHEDULE;
}

static uint_fast8_t i2c_async_start_wait(struct timer *timer)
{
    struct i2cdev_s *i2c_slv = container_of(timer, struct i2cdev_s, timer);
    struct i2c_config *config = &i2c_slv->i2c_config;
    if (TWCR & (1 << TWINT)) {
        uint32_t status = TWSR;
        if (status != 0x10 && status != 0x08)
            shutdown("Failed to send i2c start");

        if (i2c_slv->data_len[0] & I2C_R) {
            TWDR = config->addr | 0x1;
            i2c_slv->data_len[0] &= ~(I2C_R);
            i2c_slv->cur = i2c_slv->tail;
            timer->func = i2c_async_read_start_wait;
        } else {
            TWDR = config->addr;
            timer->func = i2c_async_write;
        }
        TWCR = (1 << TWEN) | (1 << TWINT);
    }

    timer->waketime = timer_read_time() + config->pause;
    return SF_RESCHEDULE;
}

static uint_fast8_t i2c_async_start(struct timer *timer)
{
    struct i2cdev_s *i2c_slv = container_of(timer, struct i2cdev_s, timer);
    struct i2c_config *config = &i2c_slv->i2c_config;
    TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWSTA);
    timer->func = i2c_async_start_wait;
    timer->waketime = timer_read_time() + config->pause;
    return SF_RESCHEDULE;
}

uint_fast8_t i2c_async(struct timer *timer)
{
    struct i2cdev_s *i2c_slv = container_of(timer, struct i2cdev_s, timer);

    // write register + read
    if (i2c_slv->data_len[0] || i2c_slv->data_len[1] & I2C_R) {
        if (i2c_slv->data_len[0] == 0) {
            // cleanup empty write, start read
            i2c_cmd_done(i2c_slv);
        }

        return i2c_async_start(timer);
    }

    i2c_slv->flags &= ~IF_ACTIVE;
    return SF_DONE;
}
