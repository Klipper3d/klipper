#ifndef __SPICMDS_H
#define __SPICMDS_H

#include <stdint.h> // uint8_t

struct spidev_s *spidev_oid_lookup(uint8_t oid);
struct spi_software;
void spidev_set_software_bus(struct spidev_s *spi, struct spi_software *ss);
void spidev_transfer(struct spidev_s *spi, uint8_t receive_data
                     , uint8_t data_len, uint8_t *data);

#endif // spicmds.h
