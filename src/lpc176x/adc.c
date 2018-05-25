#include "autoconf.h"
#include "board/irq.h" // irq_save
#include "command.h"   // shutdown
#include "sched.h"     // shutdown
#include "compiler.h"  // ARRAY_SIZE
#include "gpio.h"

#include <stdint.h>
#include <LPC17xx.h>
#include <lpc17xx_adc.h>
#include <lpc17xx_pinsel.h>
#include <lpc17xx_clkpwr.h>
#include <cmsis_nvic.h>

#include "pins_MKS.h"

/****************************************************************
 * Analog to Digital Converter (ADC) pins
 ****************************************************************/

/*
 * Analog Pins
 */
static const _gpio_peripheral_t adc_pins[8] = {
  // ADC0...7
  { 0, 23, PINSEL_FUNC_1 }, // ADC0
  { 0, 24, PINSEL_FUNC_1 }, // ADC1
  { 0, 25, PINSEL_FUNC_1 }, // ADC2
  { 0, 26, PINSEL_FUNC_1 }, // ADC3
  { 1, 30, PINSEL_FUNC_3 }, // ADC4
  { 1, 31, PINSEL_FUNC_3 }, // ADC5
  { 0,  3, PINSEL_FUNC_2 }, // ADC6
  { 0,  2, PINSEL_FUNC_2 }  // ADC7
};

//#define ADC_FREQ_MAX 5000000 // 5MHz (should be less than or equal to 13MHz)
#define ADC_FREQ_MAX 1000000 // 1MHz (should be less than or equal to 13MHz)
DECL_CONSTANT(ADC_MAX, 4095);

#define ENABLE_BURST_MODE 0 // Enable ADC IRQ
static int32_t _adc_data[8];
#if 0
static void gpio_adc_isr(void) {
    // Read status
    uint32_t stat = LPC_ADC->ADSTAT;
    //Scan channels for over-run or done and update array
    if (stat & 0x0101) _adc_data[0] = LPC_ADC->ADDR0;
    if (stat & 0x0202) _adc_data[1] = LPC_ADC->ADDR1;
    if (stat & 0x0404) _adc_data[2] = LPC_ADC->ADDR2;
    if (stat & 0x0808) _adc_data[3] = LPC_ADC->ADDR3;
    if (stat & 0x1010) _adc_data[4] = LPC_ADC->ADDR4;
    if (stat & 0x2020) _adc_data[5] = LPC_ADC->ADDR5;
    if (stat & 0x4040) _adc_data[6] = LPC_ADC->ADDR6;
    if (stat & 0x8080) _adc_data[7] = LPC_ADC->ADDR7;
}
#endif
void ADC_IRQHandler(void)
//void gpio_adc_isr(void)
{
    uint32_t const ADGDR   = LPC_ADC->ADGDR;
    uint32_t const channel = ADC_GDR_CH(ADGDR); // Extract Channel Number
    _adc_data[channel] = ADC_GDR_RESULT(ADGDR); // Extract Conversion Result
}

static uint32_t CFG_REG = 0;
static uint32_t CFG_CURRENT = 0;

void
gpio_adc_init(void) {
    /* Init ADC HW */
    uint32_t rate;

#if (ENABLE_BURST_MODE == 1)
    uint8_t iter;
    for (iter = 0; iter < 8; iter++) _adc_data[iter] = -1;
#endif

    NVIC_DisableIRQ(ADC_IRQn);
    NVIC_ClearPendingIRQ(ADC_IRQn);

    CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCAD, ENABLE);
    CLKPWR_SetPCLKDiv(CLKPWR_PCLKSEL_ADC, CLKPWR_PCLKSEL_CCLK_DIV_2);
    //Enable PDN bit = The A/D converter is operational.
    CFG_REG |= ADC_CR_PDN;

    // Set ADC clock frequency scale
    rate = (SystemCoreClock / (2 * ADC_FREQ_MAX)) - 1;
    CFG_REG |= ADC_CR_CLKDIV(rate);

#if (ENABLE_BURST_MODE == 1)
    // Enable burst mode (auto mode)
    CFG_REG |= ADC_CR_BURST;
    // Enable global interrupt
    LPC_ADC->ADINTEN = ADC_INTEN_GLOBAL;
    //NVIC_SetVector(ADC_IRQn, (uint32_t)&gpio_adc_isr);

    LPC_ADC->ADCR = CFG_REG;
#else
    // Disable interrupts
    LPC_ADC->ADINTEN = 0;
    //LPC_ADC->ADCR = CFG_REG;
#endif

    DEBUG_OUT("ADC configured\n");

#if (ENABLE_BURST_MODE == 1)
    NVIC_EnableIRQ(ADC_IRQn);
#endif
}
DECL_INIT(gpio_adc_init);

struct gpio_adc
gpio_adc_setup(uint8_t pin)
{
    uint8_t const in_port = GPIO2PORT(pin);
    uint8_t const in_pin  = GPIO2PIN(pin);
    // Find pin in adc_pins table

    uint32_t chan;
    for (chan=0; ; chan++) {
        if (chan >= ARRAY_SIZE(adc_pins))
            shutdown("Not a valid ADC pin");
        if (adc_pins[chan].port == in_port &&
            adc_pins[chan].pin == in_pin)
            break;
    }
    gpio_peripheral(&adc_pins[chan], 0);
#if (ENABLE_BURST_MODE == 1)
    // Enable ADC channel
    LPC_ADC->ADCR    |= ADC_CR_CH_SEL(chan);
    // Enable channel ISR
    LPC_ADC->ADINTEN |= ADC_INTEN_CH(chan);
#endif
    return (struct gpio_adc){ .channel = chan };
}

// Try to sample a value. Returns zero if sample ready, otherwise
// returns the number of clock ticks the caller should wait before
// retrying this function.
uint32_t
gpio_adc_sample(struct gpio_adc g)
{
#if (ENABLE_BURST_MODE == 1)
    if (_adc_data[g.channel] < 0) goto need_delay;
#else

    uint32_t const chsr = LPC_ADC->ADGDR; // read global status reg

    if ( /*! (LPC_ADC->ADCR & ADC_CR_START_MASK)*/ CFG_CURRENT == 0) {
        // Start sample
        LPC_ADC->ADCR = (CFG_REG |
                         ADC_CR_CH_SEL(g.channel) |
                         ADC_CR_START_NOW);
        CFG_CURRENT = g.channel;
        goto need_delay;
    }
    if (/*ADC_GDR_CH(chsr)*/ CFG_CURRENT != g.channel)
        // Sampling in progress on another channel
        goto need_delay;
    if ( ! (chsr & ADC_GDR_DONE_FLAG) )
        // Conversion still in progress
        goto need_delay;
#endif
    // Conversion ready
    return 0;
need_delay:
    //return (CONFIG_CLOCK_FREQ / 1000000); // ~1us
    //return (CONFIG_CLOCK_FREQ / (ADC_FREQ_MAX));
    //return 25; // ~1us
    return 500;
}

// Read a value; use only after gpio_adc_sample() returns zero
uint16_t
gpio_adc_read(struct gpio_adc g)
{
#if (ENABLE_BURST_MODE == 1)
    return _adc_data[g.channel];
#else
    gpio_adc_cancel_sample(g);
    return (uint16_t)ADC_GDR_RESULT( (&LPC_ADC->ADDR0)[g.channel] /*LPC_ADC->ADGDR*/ );
#endif
}

// Cancel a sample that may have been started with gpio_adc_sample()
void
gpio_adc_cancel_sample(struct gpio_adc g)
{
    //irqstatus_t flag = irq_save();
#if (ENABLE_BURST_MODE == 1)
    _adc_data[g.channel] = -1;
#else
    (void)g;
    //need to stop START bits before disable channel
    /*LPC_ADC->ADCR &= ~ADC_CR_START_MASK;
      LPC_ADC->ADCR &= ~ADC_CR_CH_SEL(g.channel);*/
    CFG_CURRENT = 0;
#endif
    //irq_restore(flag);
}

#if (ENABLE_BURST_MODE == 1)
// Disable ADC ISR
void
gpio_adc_shutdown(void)
{
    NVIC_DisableIRQ(ADC_IRQn);
}
DECL_SHUTDOWN(gpio_adc_shutdown);
#endif
