// sam3x8e serial port
//
// Copyright (C) 2016-2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h" // CONFIG_SERIAL_BAUD
#include "board/gpio.h" // gpio_peripheral
#include "board/serial_irq.h" // serial_rx_data
#include "sam3x8e.h" // UART
#include "sched.h" // DECL_INIT

void
serial_init(void)
{
    gpio_peripheral('A', PIO_PA8A_URXD, 'A', 1);
    gpio_peripheral('A', PIO_PA9A_UTXD, 'A', 0);

    // Reset uart
    PMC->PMC_PCER0 = 1 << ID_UART;
    UART->UART_PTCR = UART_PTCR_RXTDIS | UART_PTCR_TXTDIS;
    UART->UART_CR = UART_CR_RSTRX | UART_CR_RSTTX | UART_CR_RXDIS | UART_CR_TXDIS;
    UART->UART_IDR = 0xFFFFFFFF;

    // Enable uart
    UART->UART_MR = (US_MR_CHRL_8_BIT | US_MR_NBSTOP_1_BIT | UART_MR_PAR_NO
                     | UART_MR_CHMODE_NORMAL);
    UART->UART_BRGR = SystemCoreClock / (16 * CONFIG_SERIAL_BAUD);
    UART->UART_IER = UART_IER_RXRDY;
    NVIC_EnableIRQ(UART_IRQn);
    NVIC_SetPriority(UART_IRQn, 0);
    UART->UART_CR = UART_CR_RXEN | UART_CR_TXEN;
}
DECL_INIT(serial_init);

void __visible
UART_Handler(void)
{
    uint32_t status = UART->UART_SR;
    if (status & UART_SR_RXRDY)
        serial_rx_byte(UART->UART_RHR);
    if (status & UART_SR_TXRDY) {
        uint8_t data;
        int ret = serial_get_tx_byte(&data);
        if (ret)
            UART->UART_IDR = UART_IDR_TXRDY;
        else
            UART->UART_THR = data;
    }
}

void
serial_enable_tx_irq(void)
{
    UART->UART_IER = UART_IDR_TXRDY;
}
