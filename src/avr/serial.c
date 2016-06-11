// AVR serial port code.
//
// Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <avr/interrupt.h> // USART0_RX_vect
#include <string.h> // memmove
#include "autoconf.h" // CONFIG_SERIAL_BAUD
#include "board/io.h" // readb
#include "board/misc.h" // console_get_input
#include "sched.h" // DECL_INIT
#include "irq.h" // irq_save

#define SERIAL_BUFFER_SIZE 96
static char receive_buf[SERIAL_BUFFER_SIZE];
static uint8_t receive_pos;
static char transmit_buf[SERIAL_BUFFER_SIZE];
static uint8_t transmit_pos, transmit_max;


/****************************************************************
 * Serial hardware
 ****************************************************************/

static void
serial_init(void)
{
    if (CONFIG_SERIAL_BAUD_U2X) {
        UCSR0A = 1<<U2X0;
        UBRR0 = DIV_ROUND_CLOSEST(F_CPU, 8UL * CONFIG_SERIAL_BAUD) - 1UL;
    } else {
        UCSR0A = 0;
        UBRR0 = DIV_ROUND_CLOSEST(F_CPU, 16UL * CONFIG_SERIAL_BAUD) - 1UL;
    }

    UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);
    UCSR0B = (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0) | (1<<UDRIE0);
}
DECL_INIT(serial_init);

#ifdef USART_RX_vect
#define USART0_RX_vect USART_RX_vect
#define USART0_UDRE_vect USART_UDRE_vect
#endif

// Rx interrupt - data available to be read.
ISR(USART0_RX_vect)
{
    uint8_t data = UDR0;
    if (receive_pos >= sizeof(receive_buf))
        // Serial overflow - ignore it as crc error will force retransmit
        return;
    receive_buf[receive_pos++] = data;
}

// Tx interrupt - data can be written to serial.
ISR(USART0_UDRE_vect)
{
    if (transmit_pos >= transmit_max)
        UCSR0B &= ~(1<<UDRIE0);
    else
        UDR0 = transmit_buf[transmit_pos++];
}


/****************************************************************
 * Console access functions
 ****************************************************************/

// Return a buffer (and length) containing any incoming messages
char *
console_get_input(uint8_t *plen)
{
    *plen = readb(&receive_pos);
    return receive_buf;
}

// Remove from the receive buffer the given number of bytes
void
console_pop_input(uint8_t len)
{
    uint8_t copied = 0;
    for (;;) {
        uint8_t rpos = readb(&receive_pos);
        uint8_t needcopy = rpos - len;
        if (needcopy) {
            memmove(&receive_buf[copied], &receive_buf[copied + len]
                    , needcopy - copied);
            copied = needcopy;
        }
        uint8_t flag = irq_save();
        if (rpos != readb(&receive_pos)) {
            // Raced with irq handler - retry
            irq_restore(flag);
            continue;
        }
        receive_pos = needcopy;
        irq_restore(flag);
        break;
    }
}

// Return an output buffer that the caller may fill with transmit messages
char *
console_get_output(uint8_t len)
{
    uint8_t tpos = readb(&transmit_pos), tmax = readb(&transmit_max);
    if (tpos == tmax) {
        tpos = tmax = 0;
        writeb(&transmit_max, 0);
        writeb(&transmit_pos, 0);
    }
    if (tmax + len <= sizeof(transmit_buf))
        return &transmit_buf[tmax];
    if (tmax - tpos + len > sizeof(transmit_buf))
        return NULL;
    // Disable TX irq and move buffer
    writeb(&transmit_max, 0);
    barrier();
    tpos = readb(&transmit_pos);
    tmax -= tpos;
    memmove(&transmit_buf[0], &transmit_buf[tpos], tmax);
    writeb(&transmit_pos, 0);
    barrier();
    writeb(&transmit_max, tmax);
    UCSR0B |= 1<<UDRIE0;
    return &transmit_buf[tmax];
}

// Accept the given number of bytes added to the transmit buffer
void
console_push_output(uint8_t len)
{
    writeb(&transmit_max, readb(&transmit_max) + len);
    // enable TX interrupt
    UCSR0B |= 1<<UDRIE0;
}
