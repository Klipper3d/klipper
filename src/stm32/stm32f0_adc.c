// ADC functions on STM32
//
// Copyright (C) 2019-2020  Kevin O'Connor <kevin@koconnor.net>
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

DECL_CONSTANT("ADC_MAX", 4095);

#define ADC_TEMPERATURE_PIN 0xfe
DECL_ENUMERATION("pin", "ADC_TEMPERATURE", ADC_TEMPERATURE_PIN);

static const uint8_t adc_pins[] = {
    GPIO('A', 0), GPIO('A', 1), GPIO('A', 2), GPIO('A', 3),
    GPIO('A', 4), GPIO('A', 5), GPIO('A', 6), GPIO('A', 7),
    GPIO('B', 0), GPIO('B', 1), GPIO('C', 0), GPIO('C', 1),
    GPIO('C', 2), GPIO('C', 3), GPIO('C', 4), GPIO('C', 5),
    ADC_TEMPERATURE_PIN
};

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
    ADC_TypeDef *adc = ADC1;
    uint32_t adc_base = ADC1_BASE;

    // Enable the ADC
    if (!is_enabled_pclock(adc_base)) {
        enable_pclock(adc_base);

        // 100: 41.5 ADC clock cycles
        adc->SMPR |= (~ADC_SMPR_SMP_Msk | ADC_SMPR_SMP_2 );
        adc->CFGR2 |= ADC_CFGR2_CKMODE;
        adc->CFGR1 &= ~ADC_CFGR1_AUTOFF;
        adc->CFGR1 |= ADC_CFGR1_EXTSEL;

        // do not enable ADC before calibration
        adc->CR &= ~ADC_CR_ADEN;
        while (adc->CR & ADC_CR_ADEN)
            ;
        while (adc->CFGR1 & ADC_CFGR1_DMAEN)
            ;
        // start calibration and wait for completion
        adc->CR |= ADC_CR_ADCAL;
        while (adc->CR & ADC_CR_ADCAL)
            ;

        // if not enabled
        if (!(adc->CR & ADC_CR_ADEN)){
            adc->ISR |= ADC_ISR_ADRDY;
            adc->CR |= ADC_CR_ADEN;
            while (!(ADC1->ISR & ADC_ISR_ADRDY))
                ;
        }
    }

    if (pin == ADC_TEMPERATURE_PIN) {
        ADC1_COMMON->CCR = ADC_CCR_TSEN;
    } else {
        gpio_peripheral(pin, GPIO_ANALOG, 0);
    }

    return (struct gpio_adc){ .adc = adc, .chan = 1 << chan };
}

// Try to sample a value. Returns zero if sample ready, otherwise
// returns the number of clock ticks the caller should wait before
// retrying this function.
uint32_t
gpio_adc_sample(struct gpio_adc g)
{
    ADC_TypeDef *adc = g.adc;
    if ((adc->ISR & ADC_ISR_EOC) && (adc->CHSELR == g.chan)){
        return 0;
    }
    if (adc->CR & ADC_CR_ADSTART){
       goto need_delay;
    }
    adc->CHSELR = g.chan;
    adc->CR |= ADC_CR_ADSTART;

need_delay:
    return timer_from_us(10);
}

// Read a value; use only after gpio_adc_sample() returns zero
uint16_t
gpio_adc_read(struct gpio_adc g)
{
    ADC_TypeDef *adc = g.adc;
    adc->ISR &= ~ADC_ISR_EOSEQ;
    return adc->DR;
}

// Cancel a sample that may have been started with gpio_adc_sample()
void
gpio_adc_cancel_sample(struct gpio_adc g)
{
    ADC_TypeDef *adc = g.adc;
    irqstatus_t flag = irq_save();
    if (!(adc->ISR & ADC_ISR_EOC) && (adc->CHSELR == g.chan)){
        adc->CR |= ADC_CR_ADSTP;
    }
    irq_restore(flag);
}
