#ifndef __SPI_H_
#define __SPI_H_

struct spi_config {
#ifdef __arm__
    uint32_t cfg;
#else
    uint16_t cfg;
#endif
};

// Config
// Init must be called only once from main()
void spi_init(void);
// SPI config per device
struct spi_config spi_get_config(uint8_t const mode,
                                 uint32_t const speed);
// Call set before start transmission with device
uint8_t spi_set_config(struct spi_config const config);
// Release reserved SPI peripheral
void spi_set_ready(void);

// Transfer
void    spi_transfer_len(char *data, uint8_t len);
uint8_t spi_transfer(uint8_t const data);
uint8_t spi_read_rdy(void);

extern struct spi_config spi_basic_config;

#endif // __SPI_H_
