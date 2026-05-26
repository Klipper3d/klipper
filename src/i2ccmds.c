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

DECL_ENUMERATION("i2c_bus_status", "SUCCESS", I2C_BUS_SUCCESS);
DECL_ENUMERATION("i2c_bus_status", "NACK", I2C_BUS_NACK);
DECL_ENUMERATION("i2c_bus_status", "START_NACK", I2C_BUS_START_NACK);
DECL_ENUMERATION("i2c_bus_status", "START_READ_NACK", I2C_BUS_START_READ_NACK);
DECL_ENUMERATION("i2c_bus_status", "BUS_TIMEOUT", I2C_BUS_TIMEOUT);

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
    uint8_t addr = args[3] & 0x7f;
    struct i2cdev_s *i2c = i2cdev_oid_lookup(args[0]);
    i2c->i2c_hw = i2c_setup(args[1], args[2], addr);
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

void i2c_shutdown_on_err(int ret)
{
    switch (ret) {
    case I2C_BUS_NACK:
        shutdown("I2C NACK");
    case I2C_BUS_START_NACK:
        shutdown("I2C START NACK");
    case I2C_BUS_START_READ_NACK:
        shutdown("I2C START READ NACK");
    case I2C_BUS_TIMEOUT:
        shutdown("I2C Timeout");
    }
}

int i2c_dev_write(struct i2cdev_s *i2c, uint8_t write_len, uint8_t *data)
{
    uint_fast8_t flags = i2c->flags;
    if (CONFIG_WANT_SOFTWARE_I2C && flags & IF_SOFTWARE)
        return i2c_software_write(i2c->i2c_sw, write_len, data);
    else
        return i2c_write(i2c->i2c_hw, write_len, data);
}

int i2c_dev_read(struct i2cdev_s *i2c, uint8_t reg_len, uint8_t *reg
                  , uint8_t read_len, uint8_t *read)
{
    uint_fast8_t flags = i2c->flags;
    if (CONFIG_WANT_SOFTWARE_I2C && flags & IF_SOFTWARE)
        return i2c_software_read(i2c->i2c_sw, reg_len, reg, read_len, read);
    else
        return i2c_read(i2c->i2c_hw, reg_len, reg, read_len, read);
}

void command_i2c_transfer(uint32_t *args)
{
    uint8_t oid = args[0];
    struct i2cdev_s *i2c = oid_lookup(oid, command_config_i2c);
    uint8_t wlen = args[1];
    uint8_t *wdata = command_decode_ptr(args[2]);
    uint8_t rlen = args[3];
    uint8_t rdata[rlen];
    uint8_t nack;
    if (rlen > 0)
        nack = i2c_dev_read(i2c, wlen, wdata, rlen, rdata);
    else
        nack = i2c_dev_write(i2c, wlen, wdata);
    sendf("i2c_response oid=%c i2c_bus_status=%c response=%*s",
                        oid, nack, rlen, rdata);
}
DECL_COMMAND(command_i2c_transfer, "i2c_transfer oid=%c write=%*s read_len=%u");
