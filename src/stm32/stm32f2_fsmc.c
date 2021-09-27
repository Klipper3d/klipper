// STM32F2 FSMC support
//
// Copyright (C) 2021  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "command.h" // shutdown
#include "sched.h"
#include "board/internal.h" // GPIO
#include "stm32f2xx_fsmc.h"

#define RCC_AHB3Periph_FSMC               ((uint32_t)0x00000001)

#define FSMC_FUNCTION  GPIO_FUNCTION(12) // Alternative function mapping number

typedef struct
{
    volatile uint16_t reg;
    volatile uint16_t data;
} i8080_access_struct;

i8080_access_struct *i8080_access;

DECL_CONSTANT_STR("RESERVE_PINS_FSMC", "PD0,PD1,PD8,PD9,PD10,PD14,PD15,PE7," \
                  "PE8,PE9,PE10,PE11,PE12,PE13,PE14,PE15,PD4,PD5");

void enable_i8080_fsmc(uint32_t cs_pin, uint32_t rs_pin, uint8_t dstime)
{
    FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
    FSMC_NORSRAMTimingInitTypeDef  readWriteTiming,writeTiming;

    // Configure STM32F2 FSMC data pins ; D0 - D15
    gpio_peripheral(GPIO('D', 0), FSMC_FUNCTION, 0);
    gpio_peripheral(GPIO('D', 1), FSMC_FUNCTION, 0);
    gpio_peripheral(GPIO('D', 8), FSMC_FUNCTION, 0);
    gpio_peripheral(GPIO('D', 9), FSMC_FUNCTION, 0);
    gpio_peripheral(GPIO('D', 10), FSMC_FUNCTION, 0);
    gpio_peripheral(GPIO('D', 14), FSMC_FUNCTION, 0);
    gpio_peripheral(GPIO('D', 15), FSMC_FUNCTION, 0);
    gpio_peripheral(GPIO('E', 7), FSMC_FUNCTION, 0);
    gpio_peripheral(GPIO('E', 8), FSMC_FUNCTION, 0);
    gpio_peripheral(GPIO('E', 9), FSMC_FUNCTION, 0);
    gpio_peripheral(GPIO('E', 10), FSMC_FUNCTION, 0);
    gpio_peripheral(GPIO('E', 11), FSMC_FUNCTION, 0);
    gpio_peripheral(GPIO('E', 12), FSMC_FUNCTION, 0);
    gpio_peripheral(GPIO('E', 13), FSMC_FUNCTION, 0);
    gpio_peripheral(GPIO('E', 14), FSMC_FUNCTION, 0);
    gpio_peripheral(GPIO('E', 15), FSMC_FUNCTION, 0);

    // Confiure STM32F2 FSMC control pins
    // FSMC_NOE :LCD-RD
    gpio_peripheral(GPIO('D', 4), FSMC_FUNCTION, 0);
    // FSMC_NWE :LCD-WR
    gpio_peripheral(GPIO('D', 5), FSMC_FUNCTION, 0);
    // FSMC_Ax :LCD-RS
    gpio_peripheral(rs_pin, FSMC_FUNCTION, 0);
    // FSMC_NEx :LCD-CS
    gpio_peripheral(cs_pin, FSMC_FUNCTION, 0);

    // start FSMC clock
    RCC->AHB3ENR |= RCC_AHB3Periph_FSMC;

    // Address setup time (ADDSET) is 2 HCLK 1 / 36M = 27ns
    readWriteTiming.FSMC_AddressSetupTime = 0x01;
    readWriteTiming.FSMC_AddressHoldTime = 0x00;
    readWriteTiming.FSMC_DataSetupTime = 0x0f;
    readWriteTiming.FSMC_BusTurnAroundDuration = 0x00;
    readWriteTiming.FSMC_CLKDivision = 0x00;
    readWriteTiming.FSMC_DataLatency = 0x00;
    readWriteTiming.FSMC_AccessMode = FSMC_AccessMode_A;

    // Address setup time (ADDSET) is 1 HCLK
    writeTiming.FSMC_AddressSetupTime = 0x00;
    writeTiming.FSMC_AddressHoldTime = 0x00;
    // Data save time from parameter
    writeTiming.FSMC_DataSetupTime = dstime;
    writeTiming.FSMC_BusTurnAroundDuration = 0x00;
    writeTiming.FSMC_CLKDivision = 0x00;
    writeTiming.FSMC_DataLatency = 0x00;
    writeTiming.FSMC_AccessMode = FSMC_AccessMode_A;

    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
    FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;

    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = \
        FSMC_BurstAccessMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = \
        FSMC_WaitSignalPolarity_Low;
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = \
        FSMC_AsynchronousWait_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = \
        FSMC_WaitSignalActive_BeforeWaitState;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;

    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable;

    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &readWriteTiming;
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &writeTiming;

    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);
    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);

    if( (rs_pin == GPIO('E', 2)) && (cs_pin == GPIO('D', 7)) )
        i8080_access =
            ((i8080_access_struct *)((uint32_t)(0x60000000 | 0x00FFFFFE)));
    else
        shutdown("stm32f2_fsmc: only RS on PE2 and CS on PD7 is supported.");
}

void
i8080_fsmc_wr_reg(uint16_t cmd)
{
    i8080_access->reg = cmd;
}

void
i8080_fsmc_wr_data(uint16_t data)
{
    i8080_access->data = data;
}

uint16_t
i8080_fsmc_rd_data(void)
{
    uint16_t ram;
    ram = i8080_access->data;
    return ram;
}

void
i8080_fsmc_rd_multi_data(uint16_t cmd, uint16_t *pdata, uint32_t cnt)
{
    i8080_fsmc_wr_reg(cmd);
    while(cnt--) {
        *pdata = i8080_fsmc_rd_data();
        pdata++;
    }
}
