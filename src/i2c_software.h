#ifndef __I2C_SOFTWARE_H
#define __I2C_SOFTWARE_H

#include <stdint.h> // uint8_t

struct i2c_software *i2c_software_oid_lookup(uint8_t oid);
void i2c_software_write(struct i2c_software *sw_i2c
                        , uint8_t write_len, uint8_t *write);
void i2c_software_read(struct i2c_software *sw_i2c
                       , uint8_t reg_len, uint8_t *reg
                       , uint8_t read_len, uint8_t *read);

#endif // i2c_software.h
