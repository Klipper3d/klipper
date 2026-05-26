// Analog to digital (ADC) on stm32h7 and similar chips
//
// Copyright (C) 2020 Konstantin Vogel <konstantin.vogel@gmx.net>
// Copyright (C) 2022  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/irq.h" // irq_save
#include "board/misc.h" // timer_from_us
#include "command.h" // shutdown
#include "gpio.h" // gpio_adc_setup
#include "internal.h" // GPIO
#include "sched.h" // sched_shutdown

#define ADC_INVALID_PIN 0xFF

#define ADC_TEMPERATURE_PIN 0xfe
DECL_ENUMERATION("pin", "ADC_TEMPERATURE", ADC_TEMPERATURE_PIN);

DECL_CONSTANT("ADC_MAX", 4095);

#define ADCIN_BANK_SIZE 20

// GPIOs like A0_C are not covered!
// This always gives the pin connected to the positive channel
static const uint8_t adc_pins[] = {
#if CONFIG_MACH_STM32H7
    // ADC1
    ADC_INVALID_PIN, // PA0_C  ADC12_INP0
    ADC_INVALID_PIN, // PA1_C  ADC12_INP1
    GPIO('F', 11), //           ADC1_INP2
    GPIO('A', 6),  //          ADC12_INP3
    GPIO('C', 4),  //          ADC12_INP4
    GPIO('B', 1),  //          ADC12_INP5
    GPIO('F', 12), //           ADC1_INP6
    GPIO('A', 7),  //          ADC12_INP7
    GPIO('C', 5),  //          ADC12_INP8
    GPIO('B', 0),  //          ADC12_INP9
    GPIO('C', 0),  //        ADC123_INP10
    GPIO('C', 1),  //        ADC123_INP11
    GPIO('C', 2),  //        ADC123_INP12
    GPIO('C', 3),  //         ADC12_INP13
    GPIO('A', 2),  //         ADC12_INP14
    GPIO('A', 3),  //         ADC12_INP15
    GPIO('A', 0),  //          ADC1_INP16
    GPIO('A', 1),  //          ADC1_INP17
    GPIO('A', 4),  //         ADC12_INP18
    GPIO('A', 5),  //         ADC12_INP19
    // ADC2
    ADC_INVALID_PIN, // PA0_C  ADC12_INP0
    ADC_INVALID_PIN, // PA1_C  ADC12_INP1
    GPIO('F', 13), //           ADC2_INP2
    GPIO('A', 6),  //          ADC12_INP3
    GPIO('C', 4),  //          ADC12_INP4
    GPIO('B', 1),  //          ADC12_INP5
    GPIO('F', 14), //           ADC2_INP6
    GPIO('A', 7),  //          ADC12_INP7
    GPIO('C', 5),  //          ADC12_INP8
    GPIO('B', 0),  //          ADC12_INP9
    GPIO('C', 0),  //        ADC123_INP10
    GPIO('C', 1),  //        ADC123_INP11
    GPIO('C', 2),  //        ADC123_INP12
    GPIO('C', 3),  //         ADC12_INP13
    GPIO('A', 2),  //         ADC12_INP14
    GPIO('A', 3),  //         ADC12_INP15
    ADC_INVALID_PIN,  //         dac_out1
    ADC_INVALID_PIN,  //         dac_out2
    GPIO('A', 4),  //         ADC12_INP18
    GPIO('A', 5),  //         ADC12_INP19
    // ADC3
    ADC_INVALID_PIN, // PC2_C   ADC3_INP0
    ADC_INVALID_PIN, // PC3_C   ADC3_INP1
    GPIO('F', 9) , //           ADC3_INP2
    GPIO('F', 7),  //           ADC3_INP3
    GPIO('F', 5),  //           ADC3_INP4
    GPIO('F', 3),  //           ADC3_INP5
    GPIO('F', 10), //           ADC3_INP6
    GPIO('F', 8),  //           ADC3_INP7
    GPIO('F', 6),  //           ADC3_INP8
    GPIO('F', 4),  //           ADC3_INP9
    GPIO('C', 0),  //        ADC123_INP10
    GPIO('C', 1),  //        ADC123_INP11
    GPIO('C', 2),  //        ADC123_INP12
    GPIO('H', 2),  //          ADC3_INP13
    GPIO('H', 3),  //          ADC3_INP14
    GPIO('H', 4),  //          ADC3_INP15
    GPIO('H', 5),  //          ADC3_INP16
  #if CONFIG_MACH_STM32H723
    ADC_TEMPERATURE_PIN,
    ADC_INVALID_PIN,
  #else
    ADC_INVALID_PIN, //            Vbat/4
    ADC_TEMPERATURE_PIN,//         VSENSE
  #endif
    ADC_INVALID_PIN, //           VREFINT
#elif CONFIG_MACH_STM32G4
    ADC_INVALID_PIN,        // [0] vssa
    GPIO('A', 0),           // [1]
    GPIO('A', 1),           // [2]
    GPIO('A', 2),           // [3]
    GPIO('A', 3),           // [4]
    GPIO('B', 14),          // [5]
    GPIO('C', 0),           // [6]
    GPIO('C', 1),           // [7]
    GPIO('C', 2),           // [8]
    GPIO('C', 3),           // [9]
    GPIO('F', 0),           // [10]
    GPIO('B', 12),          // [11]
    GPIO('B', 1),           // [12]
    ADC_INVALID_PIN,        // [13] opamp
    GPIO('B', 11),          // [14]
    GPIO('B', 0),           // [15]
    ADC_TEMPERATURE_PIN,    // [16] vtemp
    ADC_INVALID_PIN,        // [17] vbat/3
    ADC_INVALID_PIN,        // [18] vref
    ADC_INVALID_PIN,
    ADC_INVALID_PIN,        // [0] vssa       ADC 2
    GPIO('A', 0),           // [1]
    GPIO('A', 1),           // [2]
    GPIO('A', 6),           // [3]
    GPIO('A', 7),           // [4]
    GPIO('C', 4),           // [5]
    GPIO('C', 0),           // [6]
    GPIO('C', 1),           // [7]
    GPIO('C', 2),           // [8]
    GPIO('C', 3),           // [9]
    GPIO('F', 1),           // [10]
    GPIO('C', 5),           // [11]
    GPIO('B', 2),           // [12]
    GPIO('A', 5),           // [13]
    GPIO('B', 11),          // [14]
    GPIO('B', 15),          // [15]
    ADC_INVALID_PIN,        // [16] opamp
    GPIO('A', 4),           // [17]
    ADC_INVALID_PIN,        // [18] opamp
#else // stm32l4
    ADC_INVALID_PIN,        // vref
    GPIO('C', 0),           // ADC12_IN1 .. 16
    GPIO('C', 1),
    GPIO('C', 2),
    GPIO('C', 3),
    GPIO('A', 0),
    GPIO('A', 1),
    GPIO('A', 2),
    GPIO('A', 3),
    GPIO('A', 4),
    GPIO('A', 5),
    GPIO('A', 6),
    GPIO('A', 7),
    GPIO('C', 4),
    GPIO('C', 5),
    GPIO('B', 0),
    GPIO('B', 1),
    ADC_TEMPERATURE_PIN,    // temp
    ADC_INVALID_PIN,        // vbat
#endif
};

// ADC timing
#define ADC_CKMODE 0b11
#define ADC_ATICKS 0b110
#define ADC_ATICKS_H723_ADC3 0b111
// 400Mhz stm32h7: clock=25Mhz, Tsamp=387.5, Tconv=394, total=15.76us
// 520Mhz stm32h723: clock=32.5Mhz, Tsamp=387.5, Tconv=394, total=12.12us
// 520Mhz stm32h723 adc3: clock=65Mhz, Tsamp=640.5, Tconv=653, total=10.05us
// 80Mhz stm32l4: clock=20Mhz, Tsamp=247.5, Tconv=260, total=13.0us
// 170Mhz stm32g4: clock=42.5Mhz, Tsamp=247.5, Tconv=260, total=6.118us

// Handle register name differences between chips
#if CONFIG_MACH_STM32H723
  #define PCSEL PCSEL_RES0
#elif CONFIG_MACH_STM32G4
  #define ADC_CCR_TSEN ADC_CCR_VSENSESEL
#endif

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

    // Determine which ADC block to use and enable its clock
    ADC_TypeDef *adc;
    ADC_Common_TypeDef *adc_common;
#ifdef ADC3
    if (chan >= 2 * ADCIN_BANK_SIZE) {
        chan -= 2 * ADCIN_BANK_SIZE;
        adc = ADC3;
#if CONFIG_MACH_STM32G4
        adc_common = ADC345_COMMON;
#else
        adc_common = ADC3_COMMON;
#endif
    } else
#endif
#ifdef ADC2
    if (chan >= ADCIN_BANK_SIZE) {
        chan -= ADCIN_BANK_SIZE;
        adc = ADC2;
        adc_common = ADC12_COMMON;
    } else
#endif
    {
        adc = ADC1;
        adc_common = ADC12_COMMON;
    }
    if (!is_enabled_pclock((uint32_t)adc_common))
        enable_pclock((uint32_t)adc_common);
    MODIFY_REG(adc_common->CCR, ADC_CCR_CKMODE_Msk,
               ADC_CKMODE << ADC_CCR_CKMODE_Pos);

    // Enable the ADC
    if (!(adc->CR & ADC_CR_ADEN)) {
        // Switch on voltage regulator and wait for it to stabilize
        uint32_t cr = ADC_CR_ADVREGEN;
        adc->CR = cr;
        uint32_t end = timer_read_time() + timer_from_us(20);
        while (timer_is_before(timer_read_time(), end))
            ;

        // Setup chip specific flags
        uint32_t aticks = ADC_ATICKS;
#if CONFIG_MACH_STM32H7
        if (CONFIG_MACH_STM32H723 && adc == ADC3) {
            aticks = ADC_ATICKS_H723_ADC3;
        } else {
            // Use linear calibration on stm32h7
            cr |= ADC_CR_ADCALLIN;
            // Set boost mode on stm32h7
            uint32_t boost = CONFIG_CLOCK_FREQ > 400000000 ? 0b11 : 0b10;
            cr |= boost << ADC_CR_BOOST_Pos;
            // Set 12bit samples on the stm32h7
            adc->CFGR = ADC_CFGR_JQDIS | (0b110 << ADC_CFGR_RES_Pos);
        }
#endif

        // Perform adc calibration
        adc->CR = cr | ADC_CR_ADCAL;
        while (adc->CR & ADC_CR_ADCAL)
            ;

        // Enable ADC
        adc->ISR = ADC_ISR_ADRDY;
        adc->ISR; // Dummy read to make sure write is flushed
        while (!(adc->CR & ADC_CR_ADEN))
            adc->CR |= ADC_CR_ADEN;
        while (!(adc->ISR & ADC_ISR_ADRDY))
            ;

        // Set ADC clock cycles sample time for every channel
        uint32_t av = (aticks           | (aticks << 3)  | (aticks << 6)
                       | (aticks << 9)  | (aticks << 12) | (aticks << 15)
                       | (aticks << 18) | (aticks << 21) | (aticks << 24)
                       | (aticks << 27));
        adc->SMPR1 = av;
        adc->SMPR2 = av;
    }

    if (pin == ADC_TEMPERATURE_PIN) {
        adc_common->CCR |= ADC_CCR_TSEN;
    } else {
        gpio_peripheral(pin, GPIO_ANALOG, 0);
    }

    // Setup preselect (connect) channel on stm32h7
#if CONFIG_MACH_STM32H7
    adc->PCSEL |= (1 << chan);
#endif
    return (struct gpio_adc){ .adc = adc, .chan = chan };
}

// Try to sample a value. Returns zero if sample ready, otherwise
// returns the number of clock ticks the caller should wait before
// retrying this function.
uint32_t
gpio_adc_sample(struct gpio_adc g)
{
    ADC_TypeDef *adc = g.adc;
    uint32_t cr = adc->CR;
    if (cr & ADC_CR_ADSTART)
        goto need_delay;
    if (adc->ISR & ADC_ISR_EOC) {
        if (adc->SQR1 == (g.chan << ADC_SQR1_SQ1_Pos))
            return 0;
        goto need_delay;
    }
    // Start sample
    adc->SQR1 = (g.chan << ADC_SQR1_SQ1_Pos);
    adc->CR = cr | ADC_CR_ADSTART;
need_delay:
    return timer_from_us(20);
}

// Read a value; use only after gpio_adc_sample() returns zero
uint16_t
gpio_adc_read(struct gpio_adc g)
{
    ADC_TypeDef *adc = g.adc;
    return adc->DR;
}

// Cancel a sample that may have been started with gpio_adc_sample()
void
gpio_adc_cancel_sample(struct gpio_adc g)
{
    ADC_TypeDef *adc = g.adc;
    irqstatus_t flag = irq_save();
    if (adc->SQR1 == (g.chan << ADC_SQR1_SQ1_Pos)) {
        uint32_t cr = adc->CR;
        if (cr & ADC_CR_ADSTART)
            adc->CR = (cr & ~ADC_CR_ADSTART) | ADC_CR_ADSTP;
        if (adc->ISR & ADC_ISR_EOC)
            gpio_adc_read(g);
    }
    irq_restore(flag);
}
