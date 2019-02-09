#ifndef __SPI_SOFTWARE_H
#define __SPI_SOFTWARE_H

#include <stdint.h> // uint8_t

struct spi_software *spi_software_setup(
    uint8_t oid, uint8_t mode, uint32_t rate);
void spi_software_prepare(struct spi_software *ss);
void spi_software_transfer(struct spi_software *ss, uint8_t receive_data
                           , uint8_t len, uint8_t *data);

#endif // spi_software.h
