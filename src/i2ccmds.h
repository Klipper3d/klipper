#ifndef __I2CCMDS_H
#define __I2CCMDS_H

#include <inttypes.h>
#include "board/gpio.h" // i2c_config

struct i2cdev_s {
    struct i2c_config i2c_config;
};

struct i2cdev_s *i2cdev_oid_lookup(uint8_t oid);

#endif
