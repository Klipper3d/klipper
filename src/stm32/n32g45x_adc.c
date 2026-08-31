// ADC functions on N32G45x
//
// Copyright (C) 2022-2023  Alexey Golyshin <stas2z@gmail.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h" // CONFIG_CLOCK_FREQ
#include "board/irq.h" // irq_save
#include "board/misc.h" // timer_from_us
#include "command.h" // shutdown
#include "compiler.h" // ARRAY_SIZE
#include "generic/armcm_timer.h" // udelay
#include "gpio.h" // gpio_adc_setup
#include "internal.h" // GPIO
#include "sched.h" // sched_shutdown
#include "n32g45x_adc.h" // ADC

#define ADC_INVALID_PIN 0xFF

DECL_CONSTANT("ADC_MAX", 4095);

#define ADC_TEMPERATURE_PIN 0xfe
DECL_ENUMERATION("pin", "ADC_TEMPERATURE", ADC_TEMPERATURE_PIN);

static const uint8_t adc_pins[] = {
    // ADC1
    ADC_INVALID_PIN, GPIO('A', 0), GPIO('A', 1), GPIO('A', 6),
    GPIO('A', 3), GPIO('F', 4), ADC_INVALID_PIN, ADC_INVALID_PIN,
    ADC_INVALID_PIN, ADC_INVALID_PIN, ADC_INVALID_PIN, ADC_INVALID_PIN,
    ADC_INVALID_PIN, ADC_INVALID_PIN, ADC_INVALID_PIN, ADC_INVALID_PIN,
    ADC_TEMPERATURE_PIN, ADC_INVALID_PIN, ADC_INVALID_PIN, ADC_INVALID_PIN,
    ADC_INVALID_PIN, ADC_INVALID_PIN, ADC_INVALID_PIN, ADC_INVALID_PIN,
    ADC_INVALID_PIN, ADC_INVALID_PIN, ADC_INVALID_PIN, ADC_INVALID_PIN,
    ADC_INVALID_PIN, ADC_INVALID_PIN, ADC_INVALID_PIN, ADC_INVALID_PIN,
    // ADC2
    ADC_INVALID_PIN, GPIO('A', 4), GPIO('A', 5), GPIO('B', 1),
    GPIO('A', 7), GPIO('C', 4), GPIO('C', 0), GPIO('C', 1),
    GPIO('C', 2), GPIO('C', 3), GPIO('F', 2), GPIO('A', 2),
    GPIO('C', 5), GPIO('B', 2), ADC_INVALID_PIN, ADC_INVALID_PIN,
    ADC_INVALID_PIN, ADC_INVALID_PIN, ADC_INVALID_PIN, ADC_INVALID_PIN,
    ADC_INVALID_PIN, ADC_INVALID_PIN, ADC_INVALID_PIN, ADC_INVALID_PIN,
    ADC_INVALID_PIN, ADC_INVALID_PIN, ADC_INVALID_PIN, ADC_INVALID_PIN,
    ADC_INVALID_PIN, ADC_INVALID_PIN, ADC_INVALID_PIN, ADC_INVALID_PIN,
#if CONFIG_MACH_N32G455 // ADC3/4 for G455 only
    // ADC3
    ADC_INVALID_PIN, GPIO('B', 11), GPIO('E', 9), GPIO('E', 13),
    GPIO('E', 12), GPIO('B', 13), GPIO('E', 8), GPIO('D', 10),
    GPIO('D', 11), GPIO('D', 12), GPIO('D', 13), GPIO('D', 14),
    GPIO('B', 0), GPIO('E', 7), GPIO('E', 10), GPIO('E', 11),
    ADC_INVALID_PIN, ADC_INVALID_PIN, ADC_INVALID_PIN, ADC_INVALID_PIN,
    ADC_INVALID_PIN, ADC_INVALID_PIN, ADC_INVALID_PIN, ADC_INVALID_PIN,
    ADC_INVALID_PIN, ADC_INVALID_PIN, ADC_INVALID_PIN, ADC_INVALID_PIN,
    ADC_INVALID_PIN, ADC_INVALID_PIN, ADC_INVALID_PIN, ADC_INVALID_PIN,
    // ADC4
    ADC_INVALID_PIN, GPIO('E', 14), GPIO('E', 15), GPIO('B', 12),
    GPIO('B', 14), GPIO('B', 15), ADC_INVALID_PIN, ADC_INVALID_PIN,
    ADC_INVALID_PIN, ADC_INVALID_PIN, ADC_INVALID_PIN, ADC_INVALID_PIN,
    GPIO('D', 8), GPIO('D', 9), ADC_INVALID_PIN, ADC_INVALID_PIN,
    ADC_INVALID_PIN, ADC_INVALID_PIN, ADC_INVALID_PIN, ADC_INVALID_PIN,
    ADC_INVALID_PIN, ADC_INVALID_PIN, ADC_INVALID_PIN, ADC_INVALID_PIN,
    ADC_INVALID_PIN, ADC_INVALID_PIN, ADC_INVALID_PIN, ADC_INVALID_PIN,
    ADC_INVALID_PIN, ADC_INVALID_PIN, ADC_INVALID_PIN, ADC_INVALID_PIN,
#endif
};

// The adc needs both a conversion clock (ADCHCLK, derived from HCLK)
// and a separate 1Mhz reference clock (ADC1M).  Neither has a usable
// reset default at the clock rates this chip runs at, so configure
// both before enabling an adc.
static void
adc_clock_setup(void)
{
    // Pick the smallest divisor that keeps ADCHCLK at or below 9Mhz
    static const uint8_t adchclk_divs[] = { 1, 2, 4, 6, 8, 10, 12, 16, 32 };
    uint32_t adchclk_pres = ARRAY_SIZE(adchclk_divs) - 1;
    uint32_t i;
    for (i = 0; i < ARRAY_SIZE(adchclk_divs); i++) {
        if (CONFIG_CLOCK_FREQ / adchclk_divs[i] <= 9000000) {
            adchclk_pres = i;
            break;
        }
    }
    // ADC1M is divided down from the same oscillator the pll uses: the
    // crystal when there is one, otherwise the 8Mhz internal oscillator
    uint32_t adc1m_src = (CONFIG_STM32_CLOCK_REF_INTERNAL
                          ? RCC_ADC1MCLK_SRC_HSI : RCC_ADC1MCLK_SRC_HSE);
    uint32_t adc1m_freq = (CONFIG_STM32_CLOCK_REF_INTERNAL
                           ? 8000000 : CONFIG_CLOCK_REF_FREQ);
    uint32_t adc1m_pres = (adc1m_freq / 1000000 - 1) << 11;

    uint32_t reg_temp = ADC_RCC_CFG2;
    // Run the adc from HCLK, leaving the pll sourced adc clock disabled
    reg_temp &= CFG2_ADCPLLPRES_RESET_MASK;
    reg_temp &= RCC_ADCPLLCLK_DISABLE;
    reg_temp &= CFG2_ADCHPRES_RESET_MASK;
    reg_temp |= adchclk_pres;
    reg_temp &= CFG2_ADC1MSEL_RESET_MASK;
    reg_temp &= CFG2_ADC1MPRES_RESET_MASK;
    reg_temp |= adc1m_src | adc1m_pres;
    ADC_RCC_CFG2 = reg_temp;
}

// Perform calibration
static void
adc_calibrate(ADC_Module *adc)
{
    // Match the adc to the HCLK sourced clock selected in adc_clock_setup()
    adc->CTRL3 &= ~ADC_CTRL3_CKMOD_MSK;
    // Wake the adc from power down mode and let its input settle
    adc->CTRL2 |= CTRL2_AD_ON_SET;
    udelay(10);
    while (!(adc->CTRL3 & ADC_FLAG_RDY))
        ;
    adc->CTRL3 &= (~ADC_CTRL3_BPCAL_MSK);
    udelay(10);
    adc->CTRL2 |= CTRL2_CAL_SET;
    while (adc->CTRL2 & CTRL2_CAL_SET)
        ;
}

struct gpio_adc
gpio_adc_setup(uint32_t pin)
{
    // Find pin in adc_pins table
    int chan;
    for (chan=0; ; chan++) {
        if (chan >= ARRAY_SIZE(adc_pins))
            shutdown("Not a valid ADC pin");
        if (adc_pins[chan] == pin)
            break;
    }

    // Determine which ADC block to use
    ADC_Module *adc;
    if ((chan >> 5) == 0)
        adc = NS_ADC1;
    if ((chan >> 5) == 1)
        adc = NS_ADC2;
    if ((chan >> 5) == 2)
        adc = NS_ADC3;
    if ((chan >> 5) == 3)
        adc = NS_ADC4;
    chan &= 0x1F;

    // Enable the ADC
    uint32_t reg_temp;
    reg_temp = ADC_RCC_AHBPCLKEN;
    reg_temp |= (RCC_AHB_PERIPH_ADC1 | RCC_AHB_PERIPH_ADC2 |
                RCC_AHB_PERIPH_ADC3 | RCC_AHB_PERIPH_ADC4);
    ADC_RCC_AHBPCLKEN = reg_temp;

    adc_clock_setup();

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

    return (struct gpio_adc){ .adc = adc, .chan = chan };
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
