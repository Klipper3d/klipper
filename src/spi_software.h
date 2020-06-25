#ifndef __SPI_SOFTWARE_H
#define __SPI_SOFTWARE_H

#include <stdint.h> // uint8_t

struct spi_software *spi_software_oid_lookup(uint8_t oid);
void spi_software_prepare(struct spi_software *ss);
void spi_software_transfer(struct spi_software *ss, uint8_t receive_data
                           , uint8_t len, uint8_t *data);

#endif // spi_software.h
