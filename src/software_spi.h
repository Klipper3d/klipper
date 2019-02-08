#ifndef __SOFTWARE_SPI_H
#define __SOFTWARE_SPI_H

static const uint8_t PIN_LOW = 0;
static const uint8_t PIN_HIGH = 1;

struct software_spi_pins {
    uint8_t slave_select; //cs
    uint8_t sysclock; // sck/clk
    uint8_t mosi; //do
    uint8_t miso; //di
};

struct software_spi_config {
    struct software_spi_pins pins;
    uint8_t mode;
    uint8_t shutdown_msg_len;
    uint8_t shutdown_msg[];
};

void
software_spi_transfer(struct software_spi_config *config, uint8_t receive_data
, uint8_t len, uint8_t *data);

void software_spi_setup(struct software_spi_config spi_config);

void software_spi_shutdown(struct software_spi_config spi_config);

#endif // software_spi.h
