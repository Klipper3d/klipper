#ifndef __SPI_H
#define __SPI_H

#include <stdint.h> // uint32_t

struct spidev_s {
    union {
        struct spi_config spi_config;
        struct spi_software *spi_software;
    };
    struct gpio_out pin;
    uint8_t flags;
};

enum {
    SF_HAVE_PIN = 1, SF_SOFTWARE = 2, SF_HARDWARE = 4, SF_CS_ACTIVE_HIGH = 8
};

#endif // __SPI_H
