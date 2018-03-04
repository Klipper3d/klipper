#ifndef __SPI_H_
#define __SPI_H_

#ifdef __arm__
typedef uint32_t SPI_t;
#else
typedef uint16_t SPI_t;
#endif

#define SPI_READ_CMD (0x00)

// Config
// Init must be called only once from main()
void    spi_init(void);
// SPI config per device
SPI_t   spi_get_config(uint8_t const mode, uint32_t const speed);
// Call set before start transmission with device
void    spi_set_config(SPI_t const config);

// Transfer
void    spi_transfer_len(char *data, uint8_t len);
uint8_t spi_transfer(uint8_t const data, uint8_t const last);


extern SPI_t spi_basic_config;

#endif // __SPI_H_
