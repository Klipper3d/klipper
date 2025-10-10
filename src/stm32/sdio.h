#ifndef __STM32_SDIO_H
#define __STM32_SDIO_H

#include <stdint.h> // uint32_t

struct sdio_config {
    void *sdio;
};
struct sdio_config sdio_setup(uint32_t bus);
void sdio_send_cmd(struct sdio_config sdio, uint8_t cmd, uint32_t argument
                   , uint8_t wait);
uint8_t sdio_send_command(struct sdio_config sdio_config, uint8_t cmd
                          , uint32_t argument, uint8_t wait
                          , uint8_t *response_data
                          , uint8_t *response_data_len);
uint8_t sdio_prepare_data_transfer(struct sdio_config sdio_config, uint8_t read
                                   , uint32_t numblocks, uint32_t blocksize
                                   , uint32_t timeout);
uint8_t sdio_read_data(struct sdio_config sdio_config, uint8_t *data
                       , uint32_t numblocks, uint32_t blocksize);
uint8_t sdio_write_data(struct sdio_config sdio_config, uint8_t *data
                        , uint32_t numblocks, uint32_t blocksize);
void sdio_set_speed(struct sdio_config sdio_config, uint32_t speed);

#endif // sdio.h
