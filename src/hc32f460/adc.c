// ADC functions on Huada HC32F460
//
// Copyright (C) 2022  Steven Gotthardt <gotthardt@gmail.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "generic/misc.h"   // timer_from_us
#include "command.h"        // shutdown
#include "board/gpio.h"     // gpio_adc_setup
#include "board/internal.h" // GPIO
#include "sched.h"          // sched_shutdown

// library
#include "hc32f460_adc.h"
#include "hc32f460_pwc.h"
#include "hc32f460_gpio.h"

#define ADC_RESOLUTION_12BIT        (12u)
#define ADC_RESOLUTION_10BIT        (10u)
#define ADC_RESOLUTION_8BIT         (8u)

#define ADC1_RESOLUTION             (ADC_RESOLUTION_12BIT)
#define ADC1_PRECISION              (1ul << ADC1_RESOLUTION)

#if ADC1_RESOLUTION == ADC_RESOLUTION_12BIT
#define AdcResolution   AdcResolution_12Bit
#elif ADC1_RESOLUTION == ADC_RESOLUTION_10BIT
#define AdcResolution   AdcResolution_10Bit
#else
#define AdcResolution   AdcResolution_8Bit
#endif


/* Timeout value definitions. Found in example code */
#define TIMEOUT_VAL     (30u)

DECL_CONSTANT("ADC_MAX", ADC1_PRECISION-1);

// These pins can be used for ADC
static const uint8_t adc_gpio[] = {
    GPIO('A', 0),   // Chan 0
    GPIO('A', 1),   // Chan 1
    GPIO('A', 2),   // Chan 2
    GPIO('A', 3),   // Chan 3
    GPIO('A', 4),   // Chan 4
    GPIO('A', 5),   // Chan 5
    GPIO('A', 6),   // Chan 6
    GPIO('A', 7),   // Chan 7
    GPIO('B', 0),   // Chan 8
    GPIO('B', 1),   // Chan 9
    GPIO('C', 0),   // Chan 10  // TBed  on TriGorilla
    GPIO('C', 1),   // Chan 11  // THead on TriGorilla
    GPIO('C', 2),   // Chan 12
    GPIO('C', 3),   // Chan 13
    GPIO('C', 4),   // Chan 14  // TBed  on aquilla
    GPIO('C', 5),   // Chan 15  // THead on aquilla
};


struct gpio_adc
gpio_adc_setup(uint32_t gpio)
{
    // validate pin in adc_pins table
    int chan;
    for (chan=0; ; chan++)
    {
        if (chan >= ARRAY_SIZE(adc_gpio))
        {
            shutdown("Not a valid ADC pin");
        }
        if (adc_gpio[chan] == gpio)
        {
            break;
        }
    }

    // set as analog
    gpio_peripheral(gpio, Pin_Mode_Ana, 0);

    uint8_t sampleTime[ARRAY_SIZE(adc_gpio)] = { TIMEOUT_VAL };   // all chans
    stc_adc_ch_cfg_t stcAdcChan;
    stcAdcChan.u32Channel   = 1 << chan;
    stcAdcChan.u8Sequence   = ADC_SEQ_A;    // all conversions are in SEQ A
    stcAdcChan.pu8SampTime  = sampleTime;
    ADC_AddAdcChannel(M4_ADC1, &stcAdcChan);

    return (struct gpio_adc){ .chan = chan };
}


// Try to sample a value. Returns zero if sample ready, otherwise
// returns the number of clock ticks the caller should wait before
// retrying this function.
uint32_t
gpio_adc_sample(struct gpio_adc g)
{
    // true if the sequence is finished
    if (ADC_GetEocFlag(M4_ADC1, ADC_SEQ_A))
    {
         // all conversions are done - clear the flag
        ADC_ClrEocFlag(M4_ADC1, ADC_SEQ_A);
        return 0;
    }
    else if (M4_ADC1->STR & 1)
    {
        // running but not done yet
        return timer_from_us(TIMEOUT_VAL/2);
    }
    else
    {
        // not running - so start
        ADC_StartConvert(M4_ADC1);
    }

    return timer_from_us(TIMEOUT_VAL);
}


// Read a value; use only after gpio_adc_sample() returns zero
uint16_t
gpio_adc_read(struct gpio_adc g)
{
    // return the one we want...
    return ADC_GetValue(M4_ADC1, g.chan);
}


// Cancel a sample that may have been started with gpio_adc_sample()
void
gpio_adc_cancel_sample(struct gpio_adc g)
{
    ADC_StopConvert(M4_ADC1);
}


// The clocks are already set by the loader.
// There is ADC1 and ADC2. Sequences do all channels at once.
void
adc_init(void)
{
    //  PCLK2 (ADC clock) is 'divide by 4', Max ADC clock is 60MHz
    stc_adc_init_t stcAdcInit = {0};
    stcAdcInit.enResolution = AdcResolution;  // see define above
    stcAdcInit.enDataAlign  = AdcDataAlign_Right;
    stcAdcInit.enAutoClear  = AdcClren_Disable;
    stcAdcInit.enScanMode   = AdcMode_SAOnce;

    // power-on ADC
    PWC_Fcg3PeriphClockCmd(PWC_FCG3_PERIPH_ADC1, Enable);

    // only using ADC1
    ADC_Init(M4_ADC1, &stcAdcInit);
}

DECL_INIT(adc_init);
