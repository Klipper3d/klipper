// samd21 serial port
//
// Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/serial_irq.h" // serial_rx_data
#include "internal.h" // enable_pclock
#include "samd21.h" // SERCOM0
#include "sched.h" // DECL_INIT

void
serial_init(void)
{
    // Enable serial clock
    enable_pclock(SERCOM0_GCLK_ID_CORE, ID_SERCOM0);
    // Enable pins
    gpio_peripheral(GPIO('A', 10), 'C', 0);
    gpio_peripheral(GPIO('A', 11), 'C', 0);
    // Configure serial
    SercomUsart *su = &SERCOM0->USART;
    su->CTRLA.reg = 0;
    uint32_t areg = (SERCOM_USART_CTRLA_MODE_USART_INT_CLK
                     | SERCOM_USART_CTRLA_DORD
                     | SERCOM_USART_CTRLA_SAMPR(1)
                     | SERCOM_USART_CTRLA_TXPO(1)
                     | SERCOM_USART_CTRLA_RXPO(3));
    su->CTRLA.reg = areg;
    su->CTRLB.reg = SERCOM_USART_CTRLB_TXEN | SERCOM_USART_CTRLB_RXEN;
    uint32_t freq = get_pclock_frequency(SERCOM0_GCLK_ID_CORE);
    uint32_t baud8 = freq / (2 * CONFIG_SERIAL_BAUD);
    su->BAUD.reg = (SERCOM_USART_BAUD_FRAC_BAUD(baud8 / 8)
                    | SERCOM_USART_BAUD_FRAC_FP(baud8 % 8));
    NVIC_SetPriority(SERCOM0_IRQn, 0);
    NVIC_EnableIRQ(SERCOM0_IRQn);
    su->INTENSET.reg = SERCOM_USART_INTENSET_RXC;
    su->CTRLA.reg = areg | SERCOM_USART_CTRLA_ENABLE;
}
DECL_INIT(serial_init);

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

void
serial_enable_tx_irq(void)
{
    SERCOM0->USART.INTENSET.reg = SERCOM_USART_INTENSET_DRE;
}
