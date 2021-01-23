// Commands for sending messages on an I2C bus
//
// Copyright (C) 2018  Florian Heilmann <Florian.Heilmann@gmx.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <string.h> // memcpy
#include "basecmd.h" //oid_alloc
#include "command.h"  //sendf
#include "sched.h" //DECL_COMMAND
#include "board/gpio.h" //i2c_write/read/setup

struct i2cdev_s {
    struct i2c_config i2c_config;
};

void
command_config_i2c(uint32_t *args)
{
    uint8_t addr = args[3] & 0x7f;
    struct i2cdev_s *i2c = oid_alloc(args[0], command_config_i2c
                                     , sizeof(*i2c));
    i2c->i2c_config = i2c_setup(args[1], args[2], addr);
}
DECL_COMMAND(command_config_i2c,
             "config_i2c oid=%c i2c_bus=%u rate=%u address=%u");

void
command_i2c_write(uint32_t *args)
{
    uint8_t oid = args[0];
    struct i2cdev_s *i2c = oid_lookup(oid, command_config_i2c);
    uint8_t data_len = args[1];
    uint8_t *data = command_decode_ptr(args[2]);
    i2c_write(i2c->i2c_config, data_len, data);
}
DECL_COMMAND(command_i2c_write, "i2c_write oid=%c data=%*s");

void
command_i2c_read(uint32_t * args)
{
    uint8_t oid = args[0];
    struct i2cdev_s *i2c = oid_lookup(oid, command_config_i2c);
    uint8_t reg_len = args[1];
    uint8_t *reg = command_decode_ptr(args[2]);
    uint8_t data_len = args[3];
    uint8_t data[data_len];
    i2c_read(i2c->i2c_config, reg_len, reg, data_len, data);
    sendf("i2c_read_response oid=%c response=%*s", oid, data_len, data);
}
DECL_COMMAND(command_i2c_read, "i2c_read oid=%c reg=%*s read_len=%u");

void
command_i2c_modify_bits(uint32_t *args)
{
    uint8_t oid = args[0];
    struct i2cdev_s *i2c = oid_lookup(oid, command_config_i2c);
    uint8_t reg_len = args[1];
    uint8_t *reg = command_decode_ptr(args[2]);
    uint32_t clear_set_len = args[3];
    if (clear_set_len % 2 != 0)
        shutdown("i2c_modify_bits: Odd number of bits!");
    uint8_t data_len = clear_set_len/2;
    uint8_t *clear_set = command_decode_ptr(args[4]);
    uint8_t receive_data[reg_len + data_len];
    memcpy(receive_data, reg, reg_len);
    i2c_read(i2c->i2c_config, reg_len, reg, data_len, receive_data + reg_len);
    for (int i = 0; i < data_len; i++) {
        receive_data[reg_len + i] &= ~clear_set[i];
        receive_data[reg_len + i] |= clear_set[data_len + i];
    }
    i2c_write(i2c->i2c_config, reg_len + data_len, receive_data);
}
DECL_COMMAND(command_i2c_modify_bits,
             "i2c_modify_bits oid=%c reg=%*s clear_set_bits=%*s");
