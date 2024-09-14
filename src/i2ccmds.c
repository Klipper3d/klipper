// Commands for sending messages on an I2C bus
//
// Copyright (C) 2018  Florian Heilmann <Florian.Heilmann@gmx.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <string.h> // memcpy
#include "autoconf.h" // CONFIG_WANT_SOFTWARE_I2C
#include "basecmd.h" //oid_alloc
#include "command.h"  //sendf
#include "sched.h" //DECL_COMMAND
#include "board/gpio.h" //i2c_write/read/setup
#include "i2c_software.h" // i2c_software_setup
#include "i2ccmds.h"

enum {
    IF_SOFTWARE = 1, IF_HARDWARE = 2
};

enum {
    I2C_DEV_OK = 0,
    I2C_DEV_NACK = 1,
    I2C_DEV_TIMEOUT = 2,
    I2C_DEV_START_NACK = 3,
    I2C_DEV_START_READ_NACK = 4,
};

void
command_config_i2c(uint32_t *args)
{
    struct i2cdev_s *i2c = oid_alloc(args[0], command_config_i2c
                                     , sizeof(*i2c));
}
DECL_COMMAND(command_config_i2c, "config_i2c oid=%c");

struct i2cdev_s *
i2cdev_oid_lookup(uint8_t oid)
{
    return oid_lookup(oid, command_config_i2c);
}

void
command_i2c_set_bus(uint32_t *args)
{
    struct i2cdev_s *i2c = i2cdev_oid_lookup(args[0]);
    i2c->i2c_hw = i2c_setup(args[1], args[2]);
    i2c->addr = args[3] & 0x7f;
    i2c->flags |= IF_HARDWARE;
}
DECL_COMMAND(command_i2c_set_bus,
             "i2c_set_bus oid=%c i2c_bus=%u rate=%u address=%u");

void
i2cdev_set_software_bus(struct i2cdev_s *i2c, struct i2c_software *is)
{
    i2c->i2c_sw = is;
    i2c->flags |= IF_SOFTWARE;
}

static int i2c_dev_remap_ret(int ret)
{
    switch (ret) {
    case I2C_BUS_SUCCESS:
        ret = I2C_DEV_OK;
        break;
    case I2C_BUS_NACK:
        ret = I2C_DEV_NACK;
        break;
    case I2C_BUS_START_NACK:
        ret = I2C_DEV_START_NACK;
        break;
    case I2C_BUS_START_READ_NACK:
        ret = I2C_DEV_START_READ_NACK;
        break;
    case I2C_BUS_TIMEOUT:
        ret = I2C_DEV_TIMEOUT;
        break;
    }
    return ret;
}

int i2c_dev_write(struct i2cdev_s *i2c, uint8_t write_len, uint8_t *data)
{
    uint_fast8_t flags = i2c->flags;
    if (CONFIG_WANT_SOFTWARE_I2C && flags & IF_SOFTWARE)
        return i2c_software_write(i2c->i2c_sw, i2c->addr, write_len, data);
    else
        return i2c_write(i2c->i2c_hw, i2c->addr, write_len, data);
}

void command_i2c_write(uint32_t *args)
{
    uint8_t oid = args[0];
    struct i2cdev_s *i2c = oid_lookup(oid, command_config_i2c);
    uint8_t check = args[1];
    uint8_t data_len = args[2];
    uint8_t *data = command_decode_ptr(args[3]);
    int ret = i2c_dev_write(i2c, data_len, data);
    ret = i2c_dev_remap_ret(ret);
    if (check) {
        sendf("i2c_write_response oid=%c status=%c", oid, ret);
        return;
    }

    switch (ret) {
    case I2C_DEV_NACK:
        shutdown("I2C NACK");
    case I2C_DEV_START_NACK:
        shutdown("I2C START NACK");
    case I2C_DEV_START_READ_NACK:
        shutdown("I2C START READ NACK");
    case I2C_DEV_TIMEOUT:
        shutdown("I2C TIMEOUT");
    }
}
DECL_COMMAND(command_i2c_write, "i2c_write oid=%c check=%c data=%*s");

int i2c_dev_read(struct i2cdev_s *i2c, uint8_t reg_len, uint8_t *reg
                 , uint8_t read_len, uint8_t *read)
{
    uint_fast8_t flags = i2c->flags;
    uint8_t addr = i2c->addr;
    if (CONFIG_WANT_SOFTWARE_I2C && flags & IF_SOFTWARE)
        return i2c_software_read(i2c->i2c_sw, addr, reg_len, reg
                                 , read_len, read);
    else
        return i2c_read(i2c->i2c_hw, addr, reg_len, reg, read_len, read);
}

void command_i2c_read(uint32_t *args)
{
    uint8_t oid = args[0];
    struct i2cdev_s *i2c = oid_lookup(oid, command_config_i2c);
    uint8_t reg_len = args[1];
    uint8_t *reg = command_decode_ptr(args[2]);
    uint8_t data_len = args[3];
    uint8_t data[data_len];
    int ret = i2c_dev_read(i2c, reg_len, reg, data_len, data);
    ret = i2c_dev_remap_ret(ret);
    sendf("i2c_read_response oid=%c status=%c response=%*s", oid, ret
          , data_len, data);
}
DECL_COMMAND(command_i2c_read, "i2c_read oid=%c reg=%*s read_len=%u");
