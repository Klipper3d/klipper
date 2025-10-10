#ifndef __SDIOCMDS_H
#define __SDIOCMDS_H

#include <stdint.h> // uint8_t

struct sdiodev_s *sdiodev_oid_lookup(uint8_t oid);

#endif // sdiocmds.h
