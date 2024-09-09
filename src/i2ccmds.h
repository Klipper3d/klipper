#ifndef __I2CCMDS_H
#define __I2CCMDS_H

#include <inttypes.h>
#include "sched.h" // timer
#include "board/gpio.h" // i2c_config

enum {
    IF_SOFTWARE = 1,
    IF_ACTIVE = 1 << 2,
    IF_RW = 1 << 7,
    I2C_R = 1 << 7
};

struct i2cdev_s {
    struct timer timer;
    struct i2c_config i2c_config;
    struct i2c_software *i2c_software;
    uint8_t oid;
    uint8_t flags;
    // MSB is W/R ops W is 0, R is 1
    uint8_t data_len[8];
    uint8_t buf[32];
    uint8_t head;
    uint8_t tail;
    uint8_t size;
    uint8_t cur;

    // callback to return to
    uint_fast8_t (*callback)(struct timer *timer);
};

struct i2cdev_s *i2cdev_oid_lookup(uint8_t oid);
void i2cdev_set_software_bus(struct i2cdev_s *i2c, struct i2c_software *is);
void i2c_buf_write_b(struct i2cdev_s *i2c, uint8_t byte);
uint8_t i2c_buf_read_b(struct i2cdev_s *i2c);
void i2c_cmd_done(struct i2cdev_s *i2c);

#endif
