#ifndef __I2CCMDS_H
#define __I2CCMDS_H

#include <inttypes.h>
#include "board/gpio.h" // i2c_config

// I2C ERROR Codes
enum {
    I2C_BUS_SUCCESS,
    I2C_BUS_NACK,
    I2C_BUS_TIMEOUT,
    I2C_BUS_START_NACK,
    I2C_BUS_START_READ_NACK,
};

struct i2cdev_s {
    union {
        struct i2c_config i2c_hw;
        struct i2c_software *i2c_sw;
    };
    uint8_t flags;
};

struct i2cdev_s *i2cdev_oid_lookup(uint8_t oid);
void i2cdev_set_software_bus(struct i2cdev_s *i2c, struct i2c_software *is);
int i2c_dev_read(struct i2cdev_s *i2c, uint8_t reg_len, uint8_t *reg
                  , uint8_t read_len, uint8_t *read);
int i2c_dev_write(struct i2cdev_s *i2c, uint8_t write_len, uint8_t *data);
void i2c_shutdown_on_err(int ret);

#endif
