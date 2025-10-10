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
    GPIO('B', 0), GPIO('B', 1),
#if CONFIG_MACH_STM32F0
    GPIO('C', 0), GPIO('C', 1),
    GPIO('C', 2), GPIO('C', 3), GPIO('C', 4), GPIO('C', 5),
    ADC_TEMPERATURE_PIN
#elif CONFIG_MACH_STM32G0
    GPIO('B', 2), GPIO('B', 10),
    ADC_TEMPERATURE_PIN, 0x00, 0x00,
    GPIO('B', 11), GPIO('B', 12), GPIO('C', 4), GPIO('C', 5),
#endif
};

// Setup and calibrate ADC on stm32f0 chips
static void
stm32f0_adc_setup(void)
{
#if CONFIG_MACH_STM32F0
    #define CR_FLAGS 0
    ADC_TypeDef *adc = ADC1;
    // 100: 41.5 ADC clock cycles
    adc->SMPR = 4 << ADC_SMPR_SMP_Pos;
#endif
}

// Setup and calibrate ADC on stm32g0 chips
static void
stm32g0_adc_setup(void)
{
#if CONFIG_MACH_STM32G0
    #define CR_FLAGS ADC_CR_ADVREGEN
    ADC_TypeDef *adc = ADC1;
    // 101: 39.5 ADC clock cycles
    adc->SMPR = 5 << ADC_SMPR_SMP1_Pos;
    adc->CFGR2 = 2 << ADC_CFGR2_CKMODE_Pos; // 16Mhz

    // Enable voltage regulator
    adc->CR = CR_FLAGS;
    uint32_t end = timer_read_time() + timer_from_us(20);
    while (timer_is_before(timer_read_time(), end))
        ;
#endif
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

    // Enable the ADC
    if (!is_enabled_pclock(ADC1_BASE)) {
        enable_pclock(ADC1_BASE);
        if (CONFIG_MACH_STM32F0)
            stm32f0_adc_setup();
        else if (CONFIG_MACH_STM32G0)
            stm32g0_adc_setup();

        // Start calibration and wait for completion
        ADC_TypeDef *adc = ADC1;
        adc->CR = CR_FLAGS | ADC_CR_ADCAL;
        while (adc->CR & ADC_CR_ADCAL)
            ;
        // Enable ADC
        adc->ISR = ADC_ISR_ADRDY;
        adc->ISR; // Dummy read to make sure write is flushed
        adc->CR = CR_FLAGS | ADC_CR_ADEN;
        while (!(adc->ISR & ADC_ISR_ADRDY))
            ;
    }

    if (pin == ADC_TEMPERATURE_PIN)
        ADC1_COMMON->CCR = ADC_CCR_TSEN;
    else
        gpio_peripheral(pin, GPIO_ANALOG, 0);

    return (struct gpio_adc){ .chan = 1 << chan };
}

// Try to sample a value. Returns zero if sample ready, otherwise
// returns the number of clock ticks the caller should wait before
// retrying this function.
uint32_t
gpio_adc_sample(struct gpio_adc g)
{
    ADC_TypeDef *adc = ADC1;
    if (adc->CR & ADC_CR_ADSTART)
        goto need_delay;
    if (adc->ISR & ADC_ISR_EOC) {
        if (adc->CHSELR == g.chan)
            return 0;
        goto need_delay;
    }
#if CONFIG_MACH_STM32G0
    if (adc->CHSELR != g.chan) {
        adc->ISR = ADC_ISR_CCRDY;
        adc->CHSELR = g.chan;
        while (!(adc->ISR & ADC_ISR_CCRDY))
            ;
    }
#else
    adc->CHSELR = g.chan;
#endif
    adc->CR = CR_FLAGS | ADC_CR_ADSTART;

need_delay:
    return timer_from_us(10);
}

// Read a value; use only after gpio_adc_sample() returns zero
uint16_t
gpio_adc_read(struct gpio_adc g)
{
    ADC_TypeDef *adc = ADC1;
    return adc->DR;
}

// Cancel a sample that may have been started with gpio_adc_sample()
void
gpio_adc_cancel_sample(struct gpio_adc g)
{
    ADC_TypeDef *adc = ADC1;
    irqstatus_t flag = irq_save();
    if (adc->CHSELR == g.chan) {
        if (adc->CR & ADC_CR_ADSTART)
            adc->CR = CR_FLAGS | ADC_CR_ADSTP;
        if (adc->ISR & ADC_ISR_EOC)
            gpio_adc_read(g);
    }
    irq_restore(flag);
}
