#ifndef __STM32_I8080_FSMC_H
#define __STM32_I8080_FSMC_H

#include <stdint.h> // uint32_t

void enable_i8080_fsmc(uint32_t cs_pin, uint32_t rs_pin, uint8_t dstime);
void i8080_fsmc_wr_reg(uint16_t cmd);
void i8080_fsmc_wr_data(uint16_t data);
uint16_t i8080_fsmc_rd_data(void);
void i8080_fsmc_rd_multi_data(uint16_t cmd, uint16_t *pdata, uint32_t cnt);

#endif // stm32_i8080_fsmc.h
