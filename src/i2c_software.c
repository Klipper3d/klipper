// Software I2C emulation
//
// Copyright (C) 2023-2025  Kevin O'Connor <kevin@koconnor.net>
// Copyright (C) 2023  Alan.Ma <tech@biqu3d.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <string.h> // memcpy
#include "autoconf.h" // CONFIG_CLOCK_FREQ
#include "board/gpio.h" // gpio_out_setup
#include "board/internal.h" // gpio_peripheral
#include "board/misc.h" // timer_read_time
#include "basecmd.h" // oid_alloc
#include "command.h" // DECL_COMMAND
#include "i2ccmds.h" // i2cdev_set_software_bus

struct i2c_software {
    struct gpio_out scl_out, sda_out;
    struct gpio_in scl_in, sda_in;
    uint8_t addr;
    uint32_t ticks;
};

void
command_i2c_set_sw_bus(uint32_t *args)
{
    struct i2cdev_s *i2c = i2cdev_oid_lookup(args[0]);
    struct i2c_software *is = alloc_chunk(sizeof(*is));
    is->ticks = args[3];
    is->addr = (args[4] & 0x7f) << 1; // address format shifted
    is->scl_in = gpio_in_setup(args[1], 1);
    is->scl_out = gpio_out_setup(args[1], 1);
    is->sda_in = gpio_in_setup(args[2], 1);
    is->sda_out = gpio_out_setup(args[2], 1);
    i2cdev_set_software_bus(i2c, is);
}
DECL_COMMAND(command_i2c_set_sw_bus,
             "i2c_set_sw_bus oid=%c scl_pin=%u sda_pin=%u"
             " pulse_ticks=%u address=%u");

// The AVR micro-controllers require specialized timing
#if CONFIG_MACH_AVR

#define i2c_delay(ticks) (void)(ticks)

#else

static void
i2c_delay(uint32_t ticks)
{
    uint32_t end = timer_read_time() + ticks;
    while (timer_is_before(timer_read_time(), end))
        ;
}

#endif

static void
i2c_software_send_ack(struct i2c_software *is, const uint8_t ack)
{
    if (!ack) {
        gpio_out_reset(is->sda_out, 0);
    }
    i2c_delay(is->ticks);
    gpio_in_reset(is->scl_in, 1);
    i2c_delay(is->ticks);
    gpio_out_reset(is->scl_out, 0);
}

static uint8_t
i2c_software_read_ack(struct i2c_software *is, uint_fast8_t state)
{
    uint8_t nack = 0;
    if (state == 0)
        gpio_in_reset(is->sda_in, 1);
    i2c_delay(is->ticks);
    gpio_in_reset(is->scl_in, 1);
    nack = gpio_in_read(is->sda_in);
    i2c_delay(is->ticks);
    gpio_out_reset(is->scl_out, 0);
    return nack;
}

static int
i2c_software_send_byte(struct i2c_software *is, uint8_t b)
{
    uint_fast8_t state = 2;
    for (uint_fast8_t i = 0; i < 8; i++) {
        if (b & 0x80) {
            if (state != 1)
                gpio_in_reset(is->sda_in, 1);
            state = 1;
        } else {
            if (state > 0)
                gpio_out_reset(is->sda_out, 0);
            state = 0;
        }
        b <<= 1;
        i2c_delay(is->ticks);
        gpio_in_reset(is->scl_in, 1);
        i2c_delay(is->ticks);
        gpio_out_reset(is->scl_out, 0);
    }

    if (i2c_software_read_ack(is, state)) {
        return I2C_BUS_NACK;
    }

    return I2C_BUS_SUCCESS;
}

static uint8_t
i2c_software_read_byte(struct i2c_software *is, uint8_t remaining)
{
    uint8_t b = 0;
    gpio_in_reset(is->sda_in, 1);
    for (uint_fast8_t i = 0; i < 8; i++) {
        i2c_delay(is->ticks);
        gpio_in_reset(is->scl_in, 1);
        i2c_delay(is->ticks);
        b <<= 1;
        b |= gpio_in_read(is->sda_in);
        gpio_out_reset(is->scl_out, 0);
    }
    i2c_software_send_ack(is, remaining == 0);
    return b;
}

static int
i2c_software_start(struct i2c_software *is, uint8_t addr)
{
    int ret;
    i2c_delay(is->ticks);
    gpio_in_reset(is->sda_in, 1);
    gpio_in_reset(is->scl_in, 1);
    i2c_delay(is->ticks);
    gpio_out_reset(is->sda_out, 0);
    i2c_delay(is->ticks);
    gpio_out_reset(is->scl_out, 0);

    ret = i2c_software_send_byte(is, addr);
    if (ret == I2C_BUS_NACK)
        return I2C_BUS_START_NACK;
    return ret;
}

static void
i2c_software_stop(struct i2c_software *is)
{
    gpio_out_reset(is->sda_out, 0);
    i2c_delay(is->ticks);
    gpio_in_reset(is->scl_in, 1);
    i2c_delay(is->ticks);
    gpio_in_reset(is->sda_in, 1);
}

int
i2c_software_write(struct i2c_software *is, uint8_t write_len, uint8_t *write)
{
    int ret = i2c_software_start(is, is->addr);
    if (ret != I2C_BUS_SUCCESS)
        goto out;

    while (write_len--) {
        ret = i2c_software_send_byte(is, *write++);
        if (ret != I2C_BUS_SUCCESS)
            break;
    }

out:
    i2c_software_stop(is);
    return ret;
}

int
i2c_software_read(struct i2c_software *is, uint8_t reg_len, uint8_t *reg
                  , uint8_t read_len, uint8_t *read)
{
    int ret;
    if (reg_len) {
        // write the register
        ret = i2c_software_start(is, is->addr);
        if (ret != I2C_BUS_SUCCESS)
            goto out;
        while(reg_len--) {
            ret = i2c_software_send_byte(is, *reg++);
            if (ret != I2C_BUS_SUCCESS)
                goto out;
        }

    }
    // start/re-start and read data
    ret = i2c_software_start(is, is->addr | 0x01);
    if (ret != I2C_BUS_SUCCESS) {
        ret = I2C_BUS_START_READ_NACK;
        goto out;
    }
    while(read_len--) {
        *read = i2c_software_read_byte(is, read_len);
        read++;
    }
out:
    i2c_software_stop(is);
    return ret;
}
