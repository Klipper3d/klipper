/*
 * Main starting point for STM32F042 boards.
 *
 *  Copyright (C) 2019 Eug Krashtan <eug.krashtan@gmail.com>
 *  This file may be distributed under the terms of the GNU GPLv3 license.
 *
 */

#include "stm32f0xx_hal.h"
#include "autoconf.h"
#include "command.h" // DECL_CONSTANT
#include "board/misc.h" // timer_read_time
#include "sched.h" // sched_main
#include "internal.h"
#include "can.h"
#include "log.h"

DECL_CONSTANT_STR("MCU","stm32f042");

static IWDG_HandleTypeDef hiwdg;

/****************************************************************
 * dynamic memory pool
 ****************************************************************/

static char dynmem_pool[3 * 1024];

// Return the start of memory available for dynamic allocations
void *
dynmem_start(void)
{
    return dynmem_pool;
}

// Return the end of memory available for dynamic allocations
void *
dynmem_end(void)
{
    return &dynmem_pool[sizeof(dynmem_pool)];
}

/****************************************************************
 * misc functions
 ****************************************************************/

void
command_reset(uint32_t *args)
{
    NVIC_SystemReset();
}
DECL_COMMAND_FLAGS(command_reset, HF_IN_SHUTDOWN, "reset");

void clock_config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    /**Initializes the CPU, AHB and APB busses clocks
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI
            |RCC_OSCILLATORTYPE_HSI14|RCC_OSCILLATORTYPE_HSI48
            |RCC_OSCILLATORTYPE_LSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
    RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.HSI14CalibrationValue = 16;
    RCC_OscInitStruct.LSIState = RCC_LSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    /**Initializes the CPU, AHB and APB busses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI48;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);

    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1|RCC_PERIPHCLK_RTC;
    PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);
}

void
watchdog_reset(void)
{
    HAL_IWDG_Refresh(&hiwdg);
}
DECL_TASK(watchdog_reset);

void
watchdog_init(void)
{
    hiwdg.Instance = IWDG;
    hiwdg.Init.Prescaler = IWDG_PRESCALER_16;
    hiwdg.Init.Window = 4095;
    hiwdg.Init.Reload = 4095;
    //HAL_IWDG_Init(&hiwdg); ToDo enable after debug
}
DECL_INIT(watchdog_init);

// Main entry point
int
main(void)
{
    HAL_Init();

    clock_config();
    gpio_init();
#if (CONFIG_DEBUG_OUT)
    LogInit();
#endif
    sched_main();
    return 0;
}

void __attribute__((weak)) lprint(char *msg) {}
void __attribute__((weak)) lnprint(char *msg, size_t len) {}

/*
 * MSP init functions ( __weak replacement )
 */

/*
 * Initializes the Global MSP.
 */
void HAL_MspInit(void)
{
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_RCC_WWDG_CLK_ENABLE();
}
