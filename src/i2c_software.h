#ifndef __I2C_SOFTWARE_H
#define __I2C_SOFTWARE_H

#include <stdint.h> // uint8_t
#include "sched.h" // struct timer

struct i2c_software *i2c_software_oid_lookup(uint8_t oid);
uint_fast8_t i2c_software_async(struct timer *timer);

#endif // i2c_software.h
