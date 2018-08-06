// SAM4e8e serial port
//
// Copyright (C) 2018  Florian Heilmann <Florian.Heilmann@gmx.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

// CMSIS
#include "sam.h" // UART

// Klipper
#include "autoconf.h" // CONFIG_SERIAL_BAUD
#include "board/gpio.h" // gpio_peripheral
#include "board/serial_irq.h" // serial_rx_data
#include "sched.h" // DECL_INIT

void
serial_init(void)
{
    gpio_set_peripheral('A', PIO_PA9A_URXD0, 'A', 1);
    gpio_set_peripheral('A', PIO_PA10A_UTXD0, 'A', 0);

    // Reset uart
    PMC->PMC_PCER0 = 1 << ID_UART0;
    UART0->UART_PTCR = UART_PTCR_RXTDIS | UART_PTCR_TXTDIS;
    UART0->UART_CR = UART_CR_RSTRX | UART_CR_RSTTX | UART_CR_RXDIS | UART_CR_TXDIS;
    UART0->UART_IDR = 0xFFFFFFFF;

    // Enable uart
    UART0->UART_MR = (US_MR_CHRL_8_BIT | US_MR_NBSTOP_1_BIT | UART_MR_PAR_NO
                     | UART_MR_CHMODE_NORMAL);
    UART0->UART_BRGR = SystemCoreClock / (16 * CONFIG_SERIAL_BAUD);
    UART0->UART_IER = UART_IER_RXRDY;
    NVIC_EnableIRQ(UART0_IRQn);
    NVIC_SetPriority(UART0_IRQn, 0);
    UART0->UART_CR = UART_CR_RXEN | UART_CR_TXEN;
}
DECL_INIT(serial_init);

void __visible
UART0_Handler(void)
{
    uint32_t status = UART0->UART_SR;
    if (status & UART_SR_RXRDY)
        serial_rx_byte(UART0->UART_RHR);
    if (status & UART_SR_TXRDY) {
        uint8_t data;
        int ret = serial_get_tx_byte(&data);
        if (ret)
            UART0->UART_IDR = UART_IDR_TXRDY;
        else
            UART0->UART_THR = data;
    }
}

void
serial_enable_tx_irq(void)
{
    UART0->UART_IER = UART_IDR_TXRDY;
}
