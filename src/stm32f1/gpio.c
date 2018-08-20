// GPIO functions on STM32F1
//
// Copyright (C) 2018 Grigori Goronzy <greg@kinoho.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stdint.h> // uint32_t
#include <stdbool.h>
#include "autoconf.h" // CONFIG_CLOCK_FREQ
#include "command.h" // shutdown
#include "compiler.h" // ARRAY_SIZE
#include "gpio.h" // gpio_out_setup
#include "stm32f1xx.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_adc.h"
#include "stm32f1xx_ll_spi.h"
#include "sched.h" // sched_shutdown
#include "board/irq.h"
#include "board/io.h"


/****************************************************************
 * Pin mappings
 ****************************************************************/

#define GPIO(PORT, NUM) (((PORT)-'A') * 16 + (NUM))
#define GPIO2PORT(PIN) ((PIN) / 16)

static GPIO_TypeDef *const digital_regs[] = {
    GPIOA, GPIOB, GPIOC, GPIOD, GPIOE
};

static uint32_t const digital_pins[] = {
    LL_GPIO_PIN_0,
    LL_GPIO_PIN_1,
    LL_GPIO_PIN_2,
    LL_GPIO_PIN_3,
    LL_GPIO_PIN_4,
    LL_GPIO_PIN_5,
    LL_GPIO_PIN_6,
    LL_GPIO_PIN_7,
    LL_GPIO_PIN_8,
    LL_GPIO_PIN_9,
    LL_GPIO_PIN_10,
    LL_GPIO_PIN_11,
    LL_GPIO_PIN_12,
    LL_GPIO_PIN_13,
    LL_GPIO_PIN_14,
    LL_GPIO_PIN_15,
};

/****************************************************************
 * General Purpose Input Output (GPIO) pins
 ****************************************************************/

struct gpio_out
gpio_out_setup(uint8_t pin, uint8_t val)
{
    if (GPIO2PORT(pin) >= ARRAY_SIZE(digital_regs))
        goto fail;
    GPIO_TypeDef *regs = digital_regs[GPIO2PORT(pin)];
    uint32_t bit = digital_pins[pin % 16];
    struct gpio_out g = { .regs=regs, .bit=bit };
    gpio_out_reset(g, val);
    return g;
fail:
    shutdown("Not an output pin");
}

void
gpio_out_reset(struct gpio_out g, uint8_t val)
{
    irqstatus_t flag = irq_save();
    if (val)
        LL_GPIO_SetOutputPin(g.regs, g.bit);
    else
        LL_GPIO_ResetOutputPin(g.regs, g.bit);
    LL_GPIO_SetPinMode(g.regs, g.bit, LL_GPIO_MODE_OUTPUT);
    irq_restore(flag);
}

void
gpio_out_toggle_noirq(struct gpio_out g)
{
    LL_GPIO_TogglePin(g.regs, g.bit);
}

void
gpio_out_toggle(struct gpio_out g)
{
    irqstatus_t flag = irq_save();
    gpio_out_toggle_noirq(g);
    irq_restore(flag);
}

void
gpio_out_write(struct gpio_out g, uint8_t val)
{
    if (val)
        LL_GPIO_SetOutputPin(g.regs, g.bit);
    else
        LL_GPIO_ResetOutputPin(g.regs, g.bit);
}


struct gpio_in
gpio_in_setup(uint8_t pin, int8_t pull_up)
{
    if (GPIO2PORT(pin) >= ARRAY_SIZE(digital_regs))
        goto fail;
    GPIO_TypeDef *regs = digital_regs[GPIO2PORT(pin)];
    uint32_t bit = digital_pins[pin % 16];
    struct gpio_in g = { .regs = regs, .bit = bit };
    gpio_in_reset(g, pull_up);
    return g;
fail:
    shutdown("Not an input pin");
}

void
gpio_in_reset(struct gpio_in g, int8_t pull_up)
{
    irqstatus_t flag = irq_save();
    if (pull_up) {
        LL_GPIO_SetPinMode(g.regs, g.bit, LL_GPIO_MODE_INPUT);
        LL_GPIO_SetPinPull(g.regs, g.bit, LL_GPIO_PULL_UP);
    } else {
        LL_GPIO_SetPinMode(g.regs, g.bit, LL_GPIO_MODE_FLOATING);
    }
    irq_restore(flag);
}

uint8_t
gpio_in_read(struct gpio_in g)
{
    return LL_GPIO_IsInputPinSet(g.regs, g.bit);
}


/****************************************************************
 * Analog to Digital Converter (ADC) pins
 ****************************************************************/

DECL_CONSTANT(ADC_MAX, 4095);

#define ADC_DELAY (240 * 8)

static bool adc_busy;
static uint32_t adc_current_channel;

static const uint8_t adc_pins[] = {
    GPIO('A', 0), GPIO('A', 1), GPIO('A', 2), GPIO('A', 3),
    GPIO('A', 4), GPIO('A', 5), GPIO('A', 6), GPIO('A', 7),
    GPIO('B', 0), GPIO('B', 1), GPIO('C', 0), GPIO('C', 1),
    GPIO('C', 2), GPIO('C', 3), GPIO('C', 4), GPIO('C', 5)
};

static const uint32_t adc_channels[] = {
    LL_ADC_CHANNEL_0,
    LL_ADC_CHANNEL_1,
    LL_ADC_CHANNEL_2,
    LL_ADC_CHANNEL_3,
    LL_ADC_CHANNEL_4,
    LL_ADC_CHANNEL_5,
    LL_ADC_CHANNEL_6,
    LL_ADC_CHANNEL_7,
    LL_ADC_CHANNEL_8,
    LL_ADC_CHANNEL_9,
    LL_ADC_CHANNEL_10,
    LL_ADC_CHANNEL_11,
    LL_ADC_CHANNEL_12,
    LL_ADC_CHANNEL_13,
    LL_ADC_CHANNEL_14,
    LL_ADC_CHANNEL_15,
};

struct gpio_adc
gpio_adc_setup(uint8_t pin)
{
    // Find pin in adc_pins table
    int chan;
    for (chan=0; ; chan++) {
        if (chan >= ARRAY_SIZE(adc_pins))
            shutdown("Not a valid ADC pin");
        if (adc_pins[chan] == pin)
            break;
    }

    GPIO_TypeDef *regs = digital_regs[GPIO2PORT(pin)];
    uint32_t bit = digital_pins[pin % 16];
    LL_GPIO_SetPinMode(regs, bit, LL_GPIO_MODE_ANALOG);

    return (struct gpio_adc){ .bit = adc_channels[chan] };
}

// Try to sample a value. Returns zero if sample ready, otherwise
// returns the number of clock ticks the caller should wait before
// retrying this function.
uint32_t
gpio_adc_sample(struct gpio_adc g)
{
    /* ADC not busy, start conversion */
    if (!readb(&adc_busy)) {
        LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, g.bit);
        LL_ADC_SetChannelSamplingTime(ADC1, g.bit, LL_ADC_SAMPLINGTIME_239CYCLES_5);
        LL_ADC_REG_StartConversionSWStart(ADC1);
        adc_busy = true;
        adc_current_channel = g.bit;
        return ADC_DELAY;
    /* ADC finished conversion for this channel */
    } else if (LL_ADC_IsActiveFlag_EOS(ADC1) &&
               readl(&adc_current_channel) == g.bit) {
        LL_ADC_ClearFlag_EOS(ADC1);
        adc_busy = false;
        return 0;
    }
    /* Wants to sample another channel, or not finished yet */
    return ADC_DELAY;
}

// Read a value; use only after gpio_adc_sample() returns zero
uint16_t
gpio_adc_read(struct gpio_adc g)
{
    return LL_ADC_REG_ReadConversionData12(ADC1);
}

// Cancel a sample that may have been started with gpio_adc_sample()
void
gpio_adc_cancel_sample(struct gpio_adc g)
{
    if (readb(&adc_busy) && readl(&adc_current_channel) == g.bit) {
        adc_busy = false;
        LL_ADC_ClearFlag_EOS(ADC1);
    }
}

/****************************************************************
 * Serial Peripheral Interface (SPI) pins
 ****************************************************************/

void spi_set_mode(SPI_TypeDef *spi, uint8_t mode)
{
    switch (mode) {
    case 0:
        LL_SPI_SetClockPolarity(spi, LL_SPI_POLARITY_LOW);
        LL_SPI_SetClockPhase(spi, LL_SPI_PHASE_1EDGE);
        break;
    case 1:
        LL_SPI_SetClockPolarity(spi, LL_SPI_POLARITY_LOW);
        LL_SPI_SetClockPhase(spi, LL_SPI_PHASE_2EDGE);
        break;
    case 2:
        LL_SPI_SetClockPolarity(spi, LL_SPI_POLARITY_HIGH);
        LL_SPI_SetClockPhase(spi, LL_SPI_PHASE_1EDGE);
        break;
    case 3:
        LL_SPI_SetClockPolarity(spi, LL_SPI_POLARITY_HIGH);
        LL_SPI_SetClockPhase(spi, LL_SPI_PHASE_2EDGE);
        break;
    default:
        shutdown("Invalid SPI mode");
    }
}

void spi_set_baudrate(SPI_TypeDef *spi, uint32_t rate)
{
    const uint32_t pclk = __LL_RCC_CALC_PCLK1_FREQ(SystemCoreClock, LL_RCC_GetAPB1Prescaler());
    const uint32_t prescaler = pclk / rate;

    uint32_t setting = LL_SPI_BAUDRATEPRESCALER_DIV256;
    if (prescaler <= 2)
        setting = LL_SPI_BAUDRATEPRESCALER_DIV2;
    else if (prescaler <= 4)
        setting = LL_SPI_BAUDRATEPRESCALER_DIV4;
    else if (prescaler <= 8)
        setting = LL_SPI_BAUDRATEPRESCALER_DIV8;
    else if (prescaler <= 16)
        setting = LL_SPI_BAUDRATEPRESCALER_DIV16;
    else if (prescaler <= 32)
        setting = LL_SPI_BAUDRATEPRESCALER_DIV32;
    else if (prescaler <= 64)
        setting = LL_SPI_BAUDRATEPRESCALER_DIV64;
    else if (prescaler <= 128)
        setting = LL_SPI_BAUDRATEPRESCALER_DIV128;

    LL_SPI_SetBaudRatePrescaler(spi, setting);
}

void spi_init_pins(void)
{
    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_13, LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_14, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_15, LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_13, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_14, LL_GPIO_PULL_UP);
    LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_15, LL_GPIO_OUTPUT_PUSHPULL);
}

struct spi_config
spi_setup(uint32_t bus, uint8_t mode, uint32_t rate)
{
    struct spi_config config;
    config.config = *SPI2;

    if (bus > 0 || !rate)
        shutdown("Invalid spi_setup parameters");

    spi_init_pins();
    spi_set_mode(&config.config, mode);
    spi_set_baudrate(&config.config, rate);

    return config;
}

void
spi_prepare(struct spi_config config)
{
    *SPI2 = config.config;
    LL_SPI_Enable(SPI2);
}

void
spi_transfer(struct spi_config config, uint8_t receive_data,
             uint8_t len, uint8_t *data)
{
    while (len--) {
        LL_SPI_TransmitData8(SPI2, *data);
        while (!LL_SPI_IsActiveFlag_TXE(SPI2));
        if (receive_data) {
            while (!LL_SPI_IsActiveFlag_RXNE(SPI2));
            *data = LL_SPI_ReceiveData8(SPI2);
        }
        data++;
    }

    while (LL_SPI_IsActiveFlag_BSY(SPI2));
    LL_SPI_Disable(SPI2);
}
