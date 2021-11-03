// STM32F2 FSMC support
//
// Copyright (C) 2021  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "command.h" // shutdown
#include "sched.h"
#include "board/internal.h" // GPIO
#include "stm32f2xx.h"

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
    /* FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure; */
    /* FSMC_NORSRAMTimingInitTypeDef  readWriteTiming,writeTiming; */

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

#define FSMC_Bank1_NORSRAM1 ((uint32_t)0x00)

    /* Bank1 NOR/SRAM control register configuration */
    FSMC_Bank1->BTCR[FSMC_Bank1_NORSRAM1] =
        (uint32_t)0x08 /*FSMC_MemoryType_NOR */ |
        0x10 /*FSMC_MemoryDataWidth_16b*/ |
        0x1000 /*FSMC_WriteOperation_Enable*/ |
        0x4000 /*FSMC_ExtendedMode_Enable*/;

#define BCR_FACCEN_SET         ((uint32_t)0x40)
    FSMC_Bank1->BTCR[FSMC_Bank1_NORSRAM1] |= (uint32_t)BCR_FACCEN_SET;

    /* Bank1 NOR/SRAM timing register configuration */
    FSMC_Bank1->BTCR[FSMC_Bank1_NORSRAM1+1] =
        (uint32_t)0x01 /*FSMC_AddressSetupTime*/ |
        (0x00 /*FSMC_AddressHoldTime*/ << 4) |
        (0x0f /*FSMC_DataSetupTime*/ << 8) |
        (0x00 /*FSMC_BusTurnAroundDuration*/ << 16) |
        (0x00 /*FSMC_CLKDivision*/ << 20) |
        (0x00 /*FSMC_DataLatency*/ << 24) |
        0x00 /*FSMC_AccessMode_A*/;

    /* Bank1 NOR/SRAM timing register for write configuration,
       if extended mode is used */
    FSMC_Bank1E->BWTR[FSMC_Bank1_NORSRAM1] =
        (uint32_t)0x00 /*FSMC_AddressSetupTime*/ |
        (0x00 /*FSMC_AddressHoldTime*/ << 4 )|
        (dstime /*FSMC_DataSetupTime*/ << 8) |
        (0x00 /*FSMC_CLKDivision*/ << 20) |
        (0x00 /*FSMC_DataLatency*/ << 24) |
        0x00 /*FSMC_AccessMode_A*/;

    FSMC_Bank1->BTCR[FSMC_Bank1_NORSRAM1] |= ((uint32_t)0x01) /*BCR_MBKEN_SET*/;

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
