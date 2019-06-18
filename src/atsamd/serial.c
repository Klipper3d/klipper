// samd21 serial port
//
// Copyright (C) 2018-2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/serial_irq.h" // serial_rx_data
#include "command.h" // DECL_CONSTANT_STR
#include "internal.h" // enable_pclock
#include "sched.h" // DECL_INIT

DECL_CONSTANT_STR("RESERVE_PINS_serial", "PA11,PA10");

void
serial_init(void)
{
    // Enable serial clock
    enable_pclock(SERCOM0_GCLK_ID_CORE, ID_SERCOM0);
    // Enable pins
    gpio_peripheral(GPIO('A', 11), 'C', 0);
    gpio_peripheral(GPIO('A', 10), 'C', 0);
    // Configure serial
    SercomUsart *su = &SERCOM0->USART;
    su->CTRLA.reg = 0;
    uint32_t areg = (SERCOM_USART_CTRLA_MODE(1)
                     | SERCOM_USART_CTRLA_DORD
                     | SERCOM_USART_CTRLA_SAMPR(1)
                     | SERCOM_USART_CTRLA_RXPO(3)
                     | SERCOM_USART_CTRLA_TXPO(1));
    su->CTRLA.reg = areg;
    su->CTRLB.reg = SERCOM_USART_CTRLB_RXEN | SERCOM_USART_CTRLB_TXEN;
    uint32_t freq = get_pclock_frequency(SERCOM0_GCLK_ID_CORE);
    uint32_t baud8 = freq / (2 * CONFIG_SERIAL_BAUD);
    su->BAUD.reg = (SERCOM_USART_BAUD_FRAC_BAUD(baud8 / 8)
                    | SERCOM_USART_BAUD_FRAC_FP(baud8 % 8));
    // enable irqs
    su->INTENSET.reg = SERCOM_USART_INTENSET_RXC;
    su->CTRLA.reg = areg | SERCOM_USART_CTRLA_ENABLE;
#if CONFIG_MACH_SAMD21
    NVIC_SetPriority(SERCOM0_IRQn, 0);
    NVIC_EnableIRQ(SERCOM0_IRQn);
#elif CONFIG_MACH_SAMD51
    NVIC_SetPriority(SERCOM0_0_IRQn, 0);
    NVIC_SetPriority(SERCOM0_1_IRQn, 0);
    NVIC_SetPriority(SERCOM0_2_IRQn, 0);
    NVIC_SetPriority(SERCOM0_3_IRQn, 0);
    NVIC_EnableIRQ(SERCOM0_0_IRQn);
    NVIC_EnableIRQ(SERCOM0_1_IRQn);
    NVIC_EnableIRQ(SERCOM0_2_IRQn);
    NVIC_EnableIRQ(SERCOM0_3_IRQn);
#endif
}
DECL_INIT(serial_init);

void
serial_enable_tx_irq(void)
{
    SERCOM0->USART.INTENSET.reg = SERCOM_USART_INTENSET_DRE;
}

void __visible
SERCOM0_Handler(void)
{
    uint32_t status = SERCOM0->USART.INTFLAG.reg;
    if (status & SERCOM_USART_INTFLAG_RXC)
        serial_rx_byte(SERCOM0->USART.DATA.reg);
    if (status & SERCOM_USART_INTFLAG_DRE) {
        uint8_t data;
        int ret = serial_get_tx_byte(&data);
        if (ret)
            SERCOM0->USART.INTENCLR.reg = SERCOM_USART_INTENSET_DRE;
        else
            SERCOM0->USART.DATA.reg = data;
    }
}

// Aliases for irq handeler on SAMD51
void SERCOM0_0_Handler(void)
    __visible __attribute__((alias("SERCOM0_Handler")));
void SERCOM0_1_Handler(void)
    __visible __attribute__((alias("SERCOM0_Handler")));
void SERCOM0_2_Handler(void)
    __visible __attribute__((alias("SERCOM0_Handler")));
void SERCOM0_3_Handler(void)
    __visible __attribute__((alias("SERCOM0_Handler")));
