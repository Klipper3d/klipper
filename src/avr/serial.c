// AVR serial port code.
//
// Copyright (C) 2016-2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <avr/interrupt.h> // USART0_RX_vect
#include "autoconf.h" // CONFIG_SERIAL_BAUD
#include "board/serial_irq.h" // serial_rx_byte
#include "sched.h" // DECL_INIT

// Define serial port registers on AT90USB1286
#if !defined(UCSR0A) && defined(UCSR1A)
#define UCSR0A UCSR1A
#define UCSR0B UCSR1B
#define UCSR0C UCSR1C
#define UBRR0 UBRR1
#define UDR0 UDR1
#define UCSZ01 UCSZ11
#define UCSZ00 UCSZ10
#define U2X0 U2X1
#define RXEN0 RXEN1
#define TXEN0 TXEN1
#define RXCIE0 RXCIE1
#define UDRIE0 UDRIE1
#define USART0_RX_vect USART1_RX_vect
#define USART0_UDRE_vect USART1_UDRE_vect
#endif

// Define serial port registers on atmega168 / atmega328
#if defined(USART_RX_vect)
#define USART0_RX_vect USART_RX_vect
#define USART0_UDRE_vect USART_UDRE_vect
#endif

void
serial_init(void)
{
    UCSR0A = CONFIG_SERIAL_BAUD_U2X ? (1<<U2X0) : 0;
    uint32_t cm = CONFIG_SERIAL_BAUD_U2X ? 8 : 16;
    UBRR0 = DIV_ROUND_CLOSEST(CONFIG_CLOCK_FREQ, cm * CONFIG_SERIAL_BAUD) - 1UL;
    UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);
    UCSR0B = (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0) | (1<<UDRIE0);
}
DECL_INIT(serial_init);

// Rx interrupt - data available to be read.
ISR(USART0_RX_vect)
{
    serial_rx_byte(UDR0);
}

// Tx interrupt - data can be written to serial.
ISR(USART0_UDRE_vect)
{
    uint8_t data;
    int ret = serial_get_tx_byte(&data);
    if (ret)
        UCSR0B &= ~(1<<UDRIE0);
    else
        UDR0 = data;
}

// Enable tx interrupts
void
serial_enable_tx_irq(void)
{
    UCSR0B |= 1<<UDRIE0;
}
