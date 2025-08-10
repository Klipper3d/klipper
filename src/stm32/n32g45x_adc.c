// ADC functions on N32G45x
//
// Copyright (C) 2022-2023  Alexey Golyshin <stas2z@gmail.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/irq.h" // irq_save
#include "board/misc.h" // timer_from_us
#include "command.h" // shutdown
#include "compiler.h" // ARRAY_SIZE
#include "generic/armcm_timer.h" // udelay
#include "gpio.h" // gpio_adc_setup
#include "internal.h" // GPIO
#include "sched.h" // sched_shutdown
#include "n32g45x_adc.h" // ADC

DECL_CONSTANT("ADC_MAX", 4095);

#define ADC_TEMPERATURE_PIN 0xfe
DECL_ENUMERATION("pin", "ADC_TEMPERATURE", ADC_TEMPERATURE_PIN);

struct adc_pin_info {
    uint8_t adc_num;
    uint8_t channel;
};

static const struct adc_pin_info adc_pin_map[] = {
    [GPIO('A', 0)] = {1, 1},   // PA0 -> ADC1_IN1
    [GPIO('A', 1)] = {1, 2},   // PA1 -> ADC1_IN2
    [GPIO('A', 2)] = {2, 11},  // PA2 -> ADC2_IN11
    [GPIO('A', 3)] = {1, 4},   // PA3 -> ADC1_IN4
    [GPIO('A', 4)] = {2, 1},   // PA4 -> ADC2_IN1
    [GPIO('A', 5)] = {2, 2},   // PA5 -> ADC2_IN2
    [GPIO('A', 6)] = {1, 3},   // PA6 -> ADC1_IN3
    [GPIO('A', 7)] = {2, 4},   // PA7 -> ADC2_IN4

#if CONFIG_MACH_N32G455
    [GPIO('B', 0)] = {3, 12},  // PB0 -> ADC3_IN12
#endif
    [GPIO('B', 1)] = {2, 3},   // PB1 -> ADC2_IN3
    [GPIO('B', 2)] = {2, 13},  // PB2 -> ADC2_IN13
#if CONFIG_MACH_N32G455
    [GPIO('B', 11)] = {3, 1},  // PB11 -> ADC3_IN1
    [GPIO('B', 12)] = {4, 3},  // PB12 -> ADC4_IN3
    [GPIO('B', 13)] = {3, 5},  // PB13 -> ADC3_IN5
    [GPIO('B', 14)] = {4, 4},  // PB14 -> ADC4_IN4
    [GPIO('B', 15)] = {4, 5},  // PB15 -> ADC4_IN5
#endif

    [GPIO('C', 0)] = {2, 6},   // PC0 -> ADC2_IN6
    [GPIO('C', 1)] = {2, 7},   // PC1 -> ADC2_IN7
    [GPIO('C', 2)] = {2, 8},   // PC2 -> ADC2_IN8
    [GPIO('C', 3)] = {2, 9},   // PC3 -> ADC2_IN9
    [GPIO('C', 4)] = {2, 5},   // PC4 -> ADC2_IN5
    [GPIO('C', 5)] = {2, 12},  // PC5 -> ADC2_IN12

#if CONFIG_MACH_N32G455
    [GPIO('D', 8)] = {4, 12},  // PD8 -> ADC4_IN12
    [GPIO('D', 9)] = {4, 13},  // PD9 -> ADC4_IN13
    [GPIO('D', 10)] = {3, 7},  // PD10 -> ADC3_IN7
    [GPIO('D', 11)] = {3, 8},  // PD11 -> ADC3_IN8
    [GPIO('D', 12)] = {3, 9},  // PD12 -> ADC3_IN9
    [GPIO('D', 13)] = {3, 10}, // PD13 -> ADC3_IN10
    [GPIO('D', 14)] = {3, 11}, // PD14 -> ADC3_IN11

    [GPIO('E', 7)] = {3, 13},  // PE7 -> ADC3_IN13
    [GPIO('E', 8)] = {3, 6},   // PE8 -> ADC3_IN6
    [GPIO('E', 9)] = {3, 2},   // PE9 -> ADC3_IN2
    [GPIO('E', 10)] = {3, 14}, // PE10 -> ADC3_IN14
    [GPIO('E', 11)] = {3, 15}, // PE11 -> ADC3_IN15
    [GPIO('E', 12)] = {3, 4},  // PE12 -> ADC3_IN4
    [GPIO('E', 13)] = {3, 3},  // PE13 -> ADC3_IN3
    [GPIO('E', 14)] = {4, 1},  // PE14 -> ADC4_IN1
    [GPIO('E', 15)] = {4, 2},  // PE15 -> ADC4_IN2
#endif

    [GPIO('F', 2)] = {2, 10},  // PF2 -> ADC2_IN10
    [GPIO('F', 4)] = {1, 5},   // PF4 -> ADC1_IN5

    [ADC_TEMPERATURE_PIN] = {1, 16},
};

// Perform calibration
static void
adc_calibrate(ADC_Module *adc)
{
    adc->CTRL2 = CTRL2_AD_ON_SET;
    while (!(adc->CTRL3 & ADC_FLAG_RDY))
        ;
    adc->CTRL3 &= (~ADC_CTRL3_BPCAL_MSK);
    udelay(10);
    adc->CTRL2 = CTRL2_AD_ON_SET | CTRL2_CAL_SET;
    while (adc->CTRL2 & CTRL2_CAL_SET)
        ;
}

struct gpio_adc
gpio_adc_setup(uint32_t pin)
{
    if (pin >= ARRAY_SIZE(adc_pin_map)) {
        shutdown("ADC pin out of range");
    }

    struct adc_pin_info pin_info = adc_pin_map[pin];

    if (pin_info.adc_num == 0) {
        shutdown("Not a valid ADC pin");
    }

    ADC_Module *adc;
    switch (pin_info.adc_num) {
        case 1: adc = NS_ADC1; break;
        case 2: adc = NS_ADC2; break;
#if CONFIG_MACH_N32G455
        case 3: adc = NS_ADC3; break;
        case 4: adc = NS_ADC4; break;
#endif
        default: shutdown("Invalid ADC number");
    }

    // Enable the ADC
    uint32_t reg_temp;
    reg_temp = ADC_RCC_AHBPCLKEN;
#if CONFIG_MACH_N32G455
    reg_temp |= (RCC_AHB_PERIPH_ADC1 | RCC_AHB_PERIPH_ADC2 |
                RCC_AHB_PERIPH_ADC3 | RCC_AHB_PERIPH_ADC4);
#else
    reg_temp |= (RCC_AHB_PERIPH_ADC1 | RCC_AHB_PERIPH_ADC2);
#endif
    ADC_RCC_AHBPCLKEN = reg_temp;

    reg_temp = ADC_RCC_CFG2;
    reg_temp &= CFG2_ADCPLLPRES_RESET_MASK;
    reg_temp |= RCC_ADCPLLCLK_DIV1;
    reg_temp &= RCC_ADCPLLCLK_DISABLE;
    ADC_RCC_CFG2 = reg_temp;

    reg_temp = ADC_RCC_CFG2;
    reg_temp &= CFG2_ADCHPRES_RESET_MASK;
    reg_temp |= RCC_ADCHCLK_DIV16;
    ADC_RCC_CFG2 = reg_temp;

    ADC_InitType ADC_InitStructure;
    ADC_InitStructure.WorkMode       = ADC_WORKMODE_INDEPENDENT;
    ADC_InitStructure.MultiChEn      = 0;
    ADC_InitStructure.ContinueConvEn = 0;
    ADC_InitStructure.ExtTrigSelect  = ADC_EXT_TRIGCONV_NONE;
    ADC_InitStructure.DatAlign       = ADC_DAT_ALIGN_R;
    ADC_InitStructure.ChsNumber      = 1;
    ADC_Init(adc, &ADC_InitStructure);

    adc_calibrate(adc);

    if (pin == ADC_TEMPERATURE_PIN) {
        NS_ADC1->CTRL2 |= CTRL2_TSVREFE_SET;
        VREF1P2_CTRL |= (1<<10);
    } else {
        gpio_peripheral(pin, GPIO_ANALOG, 0);
    }

    return (struct gpio_adc){ .adc = adc, .chan = pin_info.channel };
}

// Try to sample a value. Returns zero if sample ready, otherwise
// returns the number of clock ticks the caller should wait before
// retrying this function.
uint32_t
gpio_adc_sample(struct gpio_adc g)
{
    ADC_Module *adc = g.adc;
    uint32_t sr = adc->STS;
    if (sr & ADC_STS_STR) {
        if (!(sr & ADC_STS_ENDC) || adc->RSEQ3 != g.chan)
            // Conversion still in progress or busy on another channel
            goto need_delay;
        // Conversion ready
        return 0;
    }
    // ADC timing: clock=4Mhz, Tconv=12.5, Tsamp=41.5, total=13.500us
    ADC_ConfigRegularChannel(adc, g.chan, 1, ADC_SAMP_TIME_41CYCLES5);
    adc->CTRL2 |= CTRL2_AD_ON_SET;
    adc->CTRL2 |= CTRL2_EXT_TRIG_SWSTART_SET;

need_delay:
    return timer_from_us(20);
}

// Read a value; use only after gpio_adc_sample() returns zero
uint16_t
gpio_adc_read(struct gpio_adc g)
{
    ADC_Module *adc = g.adc;
    adc->STS &= ~ADC_STS_ENDC;
    adc->STS &= ~ADC_STS_STR;
    adc->CTRL2 &= CTRL2_EXT_TRIG_SWSTART_RESET;
    uint16_t result = adc->DAT;
    return result;
}

// Cancel a sample that may have been started with gpio_adc_sample()
void
gpio_adc_cancel_sample(struct gpio_adc g)
{
    ADC_Module *adc = g.adc;
    irqstatus_t flag = irq_save();
    if (adc->STS & ADC_STS_STR)
        gpio_adc_read(g);
    irq_restore(flag);
}
