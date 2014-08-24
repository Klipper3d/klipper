// AVR serial port code.
//
// Copyright (C) 2014  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU LGPLv3 license.

#include <avr/interrupt.h> // USART0_RX_vect
#include <stdint.h>
#include "autoconf.h" // CONFIG_SERIAL_BAUD
#include "sched.h" // DECL_INIT
#include "serial.h" // serial_push


/****************************************************************
 * Simple ring buffer
 ****************************************************************/

#define SERIAL_BUFFER_SIZE 64

struct ring_buffer {
    char buf[SERIAL_BUFFER_SIZE];
    uint8_t head;
    uint8_t tail;
};

static uint8_t
buf_canpush(struct ring_buffer *buf)
{
    return (uint8_t)(buf->tail - buf->head - 1) % SERIAL_BUFFER_SIZE;
}

static void
buf_push(struct ring_buffer *buf, char c)
{
    buf->buf[buf->head] = c;
    buf->head = (uint8_t)(buf->head + 1) % SERIAL_BUFFER_SIZE;
}

static uint8_t
buf_canpop(struct ring_buffer *buf)
{
    return (uint8_t)(buf->head - buf->tail) % SERIAL_BUFFER_SIZE;
}

static char
buf_pop(struct ring_buffer *buf)
{
    char c = buf->buf[buf->tail];
    buf->tail = (uint8_t)(buf->tail + 1) % SERIAL_BUFFER_SIZE;
    return c;
}


/****************************************************************
 * Serial hardware
 ****************************************************************/

static void
serial_init(void)
{
    if (CONFIG_SERIAL_BAUD > 38401) {
        UCSR0A = 1<<U2X0;
        UBRR0 = ((F_CPU / 8) / CONFIG_SERIAL_BAUD) - 0.5;
    } else {
        UCSR0A = 0;
        UBRR0 = ((F_CPU / 16) / CONFIG_SERIAL_BAUD) - 0.5;
    }

    UCSR0B = (1<<RXEN0) | (1<<TXEN0);
    UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);

    UCSR0B |= (1<<RXCIE0) | (1<<UDRIE0);
}
DECL_INIT(serial_init);

static struct ring_buffer rxbuf, txbuf;

#ifdef USART_RX_vect
#define USART0_RX_vect USART_RX_vect
#define USART0_UDRE_vect USART_UDRE_vect
#endif

// Rx interrupt - data available to be read.
ISR(USART0_RX_vect)
{
    if (buf_canpush(&rxbuf))
        buf_push(&rxbuf, UDR0);
    // XXX - turn off irq if full?
}

// Tx interrupt - data can be written to serial.
ISR(USART0_UDRE_vect)
{
    if (buf_canpop(&txbuf))
        UDR0 = buf_pop(&txbuf);
    else
        UCSR0B &= ~(1<<UDRIE0);
}

uint8_t
serial_canpop(void)
{
    return buf_canpop(&rxbuf);
}

char
serial_pop(void)
{
    if (buf_canpop(&rxbuf))
        return buf_pop(&rxbuf);
    return 0;
}

void
serial_push(char data)
{
    if (!buf_canpush(&txbuf)) {
        if (!(SREG & (1<<SREG_I)))
            return;
        // Irqs are on, so can wait for space.
        while (!buf_canpush(&txbuf))
            ;
    }
    // XXX - not irq safe if serial_push can be called from irq and task
    buf_push(&txbuf, data);
    // enable TX interrupt
    UCSR0B |= 1<<UDRIE0;
}
