/*
 *  i2c support on STM32F0
 *
 *  Copyright (C) 2019 Eug Krashtan <eug.krashtan@gmail.com>
 *  This file may be distributed under the terms of the GNU GPLv3 license.
 *
 */

#include "stm32f0xx_hal.h"
#include "command.h" // shutdown
#include "board/gpio.h" // i2c_setup
#include "sched.h" // sched_shutdown
#include "command.h" // shutdown
#include "board/misc.h" // timer_is_before

I2C_HandleTypeDef hi2c1;

struct i2c_config i2c_setup(uint32_t bus, uint32_t rate, uint8_t addr)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    /**I2C1 GPIO Configuration
    PF0-OSC_IN     ------> I2C1_SDA
    PF1-OSC_OUT     ------> I2C1_SCL
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_I2C1;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    hi2c1.Instance = I2C1;
    hi2c1.Init.Timing = 0x2000090E;
    hi2c1.Init.OwnAddress1 = addr;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2 = 0;
    hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    HAL_I2C_Init(&hi2c1);
    /** Configure Analogue filter
    */
    HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE);
    /** Configure Digital filter
    */
    HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0);
    return (struct i2c_config){ .addr = addr };
}

static void
i2c_to_check(uint32_t timeout)
{
    if (!timer_is_before(timer_read_time(), timeout))
        shutdown("i2c timeout");
}

void i2c_write(struct i2c_config config, uint8_t write_len, uint8_t *write)
{
    uint32_t timeout = timer_read_time() + timer_from_us(5000);
    while(HAL_I2C_Master_Transmit(&hi2c1, config.addr, write,
            (uint16_t)write_len, (uint32_t)1000)!= HAL_OK) {
         if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF) {
             shutdown("Buffer error");
         }
         i2c_to_check(timeout);
     }
}

void i2c_read(struct i2c_config config, uint8_t reg_len, uint8_t *reg
              , uint8_t read_len, uint8_t *read)
{
    uint32_t timeout = timer_read_time() + timer_from_us(5000);
    i2c_write(config, reg_len, reg);
    while(HAL_I2C_Master_Receive(&hi2c1, config.addr, read,
            (uint16_t)read_len, (uint32_t)1000)!= HAL_OK) {
         if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF) {
             shutdown("Buffer error");
         }
         i2c_to_check(timeout);
     }
}

/**
* @brief I2C MSP Initialization
* @param hi2c: I2C handle pointer
* @retval None
*/
void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c)
{
    if(hi2c->Instance==I2C1)
    {
        /* Peripheral clock enable */
        __HAL_RCC_GPIOF_CLK_ENABLE();
        __HAL_RCC_I2C1_CLK_ENABLE();
    }
}
