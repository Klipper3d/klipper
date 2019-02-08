#ifndef __HARDWARE_SPI_H
#define __HARDWARE_SPI_H
#include <stdint.h> // uint8_t

struct spidev_s {
    struct spi_config spi_config;
    struct gpio_out pin;
    uint8_t flags;
    uint8_t shutdown_msg_len;
    uint8_t shutdown_msg[];
};

enum {
    SF_HAVE_PIN = 1,
};

struct spidev_s *spidev_oid_lookup(uint8_t oid);

void hardware_spi_transfer(struct spidev_s *spi, uint8_t receive_data
        , uint8_t data_len, uint8_t *data);

hardware_spi_shutdown(spidev_s *spi);

#endif // hardware_spi.h
