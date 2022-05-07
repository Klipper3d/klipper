// Hardware PWM support on HC32F460
//
// Copyright (C) 2022  Steven Gotthardt <gotthardt@gmail.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h"
#include "board/irq.h"  // irq_save
#include "command.h"    // shutdown
#include "board/gpio.h" // gpio_pwm_write
#include "internal.h"   // GPIO
#include "sched.h"      // sched_shutdown

// library
#include "hc32f460_timera.h"
#include "hc32f460_pwc.h"
#include "hc32f460_gpio.h"


#define MAX_PWM ((1 << 16) - 1)
DECL_CONSTANT("PWM_MAX", MAX_PWM);

// timer A (general purpose) has 6 units and each has 8 PWM
// M4_TMRA_TypeDef* timer;

struct gpio_pwm_info {
    uint8_t gpio;
    uint8_t unit;   // 6 units in Timer A
    en_timera_channel_t chan;
};

// Timer A (general purpose) is only timer used
// These are for pin function 4 only
// Some PWM come out on more than 1 pin
// 64pin package
static const struct gpio_pwm_info pwm_mapping[] = {
    {GPIO('A', 0), 2, TimeraCh1},
    {GPIO('A', 1), 2, TimeraCh2},
    {GPIO('A', 2), 2, TimeraCh3},
    {GPIO('A', 3), 2, TimeraCh4},
    {GPIO('A', 5), 2, TimeraCh1},
    {GPIO('A', 8), 1, TimeraCh1},
    {GPIO('A', 9), 1, TimeraCh2},
    {GPIO('A',10), 1, TimeraCh3},
    {GPIO('A',11), 1, TimeraCh4},
    {GPIO('A',13), 2, TimeraCh5},
    {GPIO('A',14), 2, TimeraCh6},
    {GPIO('A',15), 2, TimeraCh1},
    {GPIO('B', 0), 1, TimeraCh6},
    {GPIO('B', 1), 1, TimeraCh7},
    {GPIO('B', 2), 1, TimeraCh8},
    {GPIO('B', 3), 2, TimeraCh2},
    {GPIO('B', 4), 3, TimeraCh1},
    {GPIO('B', 5), 3, TimeraCh2},
    {GPIO('B', 6), 4, TimeraCh1},
    {GPIO('B', 7), 4, TimeraCh2},
    {GPIO('B', 8), 4, TimeraCh3},
    {GPIO('B', 9), 4, TimeraCh4},
    {GPIO('B',10), 2, TimeraCh3},
    {GPIO('B',12), 1, TimeraCh8},
    {GPIO('B',13), 1, TimeraCh5},
    {GPIO('B',14), 1, TimeraCh6},
    {GPIO('B',15), 1, TimeraCh7},
    {GPIO('C', 0), 2, TimeraCh5},
    {GPIO('C', 1), 2, TimeraCh6},
    {GPIO('C', 2), 2, TimeraCh7},
    {GPIO('C', 3), 2, TimeraCh8},
    {GPIO('C', 6), 3, TimeraCh1},
    {GPIO('C', 7), 3, TimeraCh2},
    {GPIO('C', 8), 3, TimeraCh3},
    {GPIO('C', 9), 3, TimeraCh4},
    {GPIO('C',10), 2, TimeraCh7},
    {GPIO('C',11), 2, TimeraCh8},
    {GPIO('C',13), 4, TimeraCh8},
    {GPIO('C',14), 4, TimeraCh5},
    {GPIO('C',15), 4, TimeraCh6},
    {GPIO('D', 2), 2, TimeraCh4},
};


struct gpio_pwm
gpio_pwm_setup(uint8_t gpio, uint32_t cycle_time, uint8_t val)
{
    // Find gpio pin in pwm_regs table
    const struct gpio_pwm_info* p = pwm_mapping;
    for (;; p++) {
        if (p >= &pwm_mapping[ARRAY_SIZE(pwm_mapping)])
            shutdown("Not a valid PWM pin");
        if (p->gpio == gpio)
            break;
    }

    // select registers - could make it programmatic
    // All TimerA power control bits are in PWC_FCG2
    M4_TMRA_TypeDef *timerA;
    switch (p->unit)
    {
        default:
        case 1:
            timerA = M4_TMRA1;
            PWC_Fcg2PeriphClockCmd(PWC_FCG2_PERIPH_TIMA1, Enable);
            break;
        case 2:
            timerA = M4_TMRA2;
            PWC_Fcg2PeriphClockCmd(PWC_FCG2_PERIPH_TIMA2, Enable);
            break;
        case 3:
            timerA = M4_TMRA3;
            PWC_Fcg2PeriphClockCmd(PWC_FCG2_PERIPH_TIMA3, Enable);
            break;
        case 4:
            timerA = M4_TMRA4;
            PWC_Fcg2PeriphClockCmd(PWC_FCG2_PERIPH_TIMA4, Enable);
            break;
        case 5:
            timerA = M4_TMRA5;
            PWC_Fcg2PeriphClockCmd(PWC_FCG2_PERIPH_TIMA5, Enable);
            break;
        case 6:
            timerA = M4_TMRA6;
            PWC_Fcg2PeriphClockCmd(PWC_FCG2_PERIPH_TIMA6, Enable);
            break;
    }

    // set the function - only using #4 (Func_Tima0)
    PORT_SetFunc(GPIO2PORT(gpio), GPIO2PIN(gpio), Func_Tima0, Disable);

    /* Configuration timera unit 1 base structure */
    stc_timera_base_init_t stcTimeraInit;
    stcTimeraInit.enClkDiv = TimeraPclkDiv128;  // 128 chosen - use below
    stcTimeraInit.enCntMode = TimeraCountModeTriangularWave;
    stcTimeraInit.enCntDir = TimeraCountDirUp;
    stcTimeraInit.enSyncStartupEn = Disable;
    stcTimeraInit.u16PeriodVal = cycle_time / 2U / 128U / 2;
    TIMERA_BaseInit(timerA, &stcTimeraInit);

    /* Configuration timera unit 1 compare structure */
    stc_timera_compare_init_t stcTimerCompareInit;
    stcTimerCompareInit.u16CompareVal = stcTimeraInit.u16PeriodVal * 4u / 5u;
    stcTimerCompareInit.enStartCountOutput = TimeraCountStartOutputHigh;
    stcTimerCompareInit.enStopCountOutput = TimeraCountStopOutputHigh;
    stcTimerCompareInit.enCompareMatchOutput = TimeraCompareMatchOutputReverse;
    stcTimerCompareInit.enPeriodMatchOutput = TimeraPeriodMatchOutputKeep;
    stcTimerCompareInit.enSpecifyOutput = TimeraSpecifyOutputInvalid;
    stcTimerCompareInit.enCacheEn = Disable;
    stcTimerCompareInit.enTriangularTroughTransEn = Enable;
    stcTimerCompareInit.enTriangularCrestTransEn = Disable;
    stcTimerCompareInit.u16CompareCacheVal = stcTimerCompareInit.u16CompareVal;
    TIMERA_CompareInit(timerA, p->chan, &stcTimerCompareInit);
    TIMERA_CompareCmd(timerA, p->chan, Enable);

    // default setup - all disabled
    stc_timera_hw_startup_config_t stcTimeraHwConfig = { 0 };
    TIMERA_HwStartupConfig(timerA, &stcTimeraHwConfig);

    return (struct gpio_pwm){.timer = timerA, .chan = p->chan};
}


void
gpio_pwm_write(struct gpio_pwm g, uint32_t val)
{
    TIMERA_SetCompareValue(g.timer, g.chan, (uint16_t)val);
}
