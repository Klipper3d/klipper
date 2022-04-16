#ifndef __I2CCMDS_H
#define __I2CCMDS_H

#include <inttypes.h>

struct i2cdev_s *
i2cdev_oid_lookup(uint8_t oid);

#endif