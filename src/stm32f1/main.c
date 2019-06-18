// Main starting point for STM32F103 boards.
//
// Copyright (C) 2018 Grigori Goronzy <greg@kinoho.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h"
#include "board/internal.h" // udelay
#include "board/misc.h" // timer_read_time
#include "command.h" // DECL_CONSTANT
#include "stm32f1xx.h"
#include "stm32f1xx_ll_system.h"
#include "stm32f1xx_ll_utils.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_iwdg.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_adc.h"
#include "stm32f1xx_ll_spi.h"
#include "sched.h" // sched_main

DECL_CONSTANT_STR("MCU", "stm32f103");


/****************************************************************
 * dynamic memory pool
 ****************************************************************/

static char dynmem_pool[15 * 1024];

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
 * watchdog handler
 ****************************************************************/

void
watchdog_reset(void)
{
    LL_IWDG_ReloadCounter(IWDG);
}
DECL_TASK(watchdog_reset);

void
watchdog_init(void)
{
    LL_IWDG_EnableWriteAccess(IWDG);
    /* IWDG timer is 40 KHz, configure to trigger every seconds */
    LL_IWDG_SetPrescaler(IWDG, LL_IWDG_PRESCALER_16);
    LL_IWDG_SetReloadCounter(IWDG, 2500);
    LL_IWDG_Enable(IWDG);

}
DECL_INIT(watchdog_init);


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
    LL_RCC_HSE_Enable();
    while (!LL_RCC_HSE_IsReady());
    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLL_MUL_9);
    LL_RCC_PLL_Disable();
    LL_RCC_PLL_Enable();
    while (!LL_RCC_PLL_IsReady());
    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);
    LL_RCC_SetADCClockSource(LL_RCC_ADC_CLKSRC_PCLK2_DIV_4);
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);
    LL_FLASH_EnablePrefetch();
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
    SystemCoreClockUpdate();
    LL_Init1msTick(SystemCoreClock);
}

void adc_config(void)
{
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1);
    /* ADC might be in deep sleep, needs to be woken up twice in that case */
    LL_ADC_Enable(ADC1);
    LL_mDelay(1);
    LL_ADC_Enable(ADC1);
    LL_mDelay(1);
    LL_ADC_StartCalibration(ADC1);
    while (LL_ADC_IsCalibrationOnGoing(ADC1));
    LL_ADC_SetSequencersScanMode(ADC1, LL_ADC_SEQ_SCAN_DISABLE);
    LL_ADC_REG_SetTriggerSource(ADC1, LL_ADC_REG_TRIG_SOFTWARE);
    LL_ADC_REG_SetSequencerLength(ADC1, LL_ADC_REG_SEQ_SCAN_DISABLE);
}

void spi_config(void)
{
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_SPI2);
}

void io_config(void)
{
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO);
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC);
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOD);
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOE);
    /* JTAG is normally not needed, but blocks ports like PB3, PB4 */
    LL_GPIO_AF_Remap_SWJ_NOJTAG();
    /* Likewise, we don't need PB3 for TRACESWO output */
    LL_DBGMCU_SetTracePinAssignment(LL_DBGMCU_TRACE_NONE);
}

// Main entry point
int
main(void)
{
    SystemInit();
    SCB->VTOR += CONFIG_FLASH_START;

    LL_Init1msTick(SystemCoreClock);
    clock_config();
    adc_config();
    io_config();
    spi_config();
    sched_main();
    return 0;
}
