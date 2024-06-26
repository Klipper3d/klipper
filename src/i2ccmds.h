#ifndef __I2CCMDS_H
#define __I2CCMDS_H

#include <inttypes.h>
#include "board/gpio.h" // i2c_config

struct i2cdev_s {
    struct i2c_config i2c_config;
    struct i2c_software *i2c_software;
    uint8_t flags;
};

struct i2cdev_s *i2cdev_oid_lookup(uint8_t oid);
void i2cdev_set_software_bus(struct i2cdev_s *i2c, struct i2c_software *is);

#endif
