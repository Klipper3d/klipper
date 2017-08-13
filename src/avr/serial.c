// AVR serial port code.
//
// Copyright (C) 2016,2017  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <avr/interrupt.h> // USART0_RX_vect
#include <string.h> // memmove
#include "autoconf.h" // CONFIG_SERIAL_BAUD
#include "board/io.h" // readb
#include "board/misc.h" // console_sendf
#include "command.h" // DECL_CONSTANT
#include "irq.h" // irq_save
#include "pgm.h" // READP
#include "sched.h" // DECL_INIT

static char receive_buf[192];
static uint8_t receive_pos;
static char transmit_buf[96];
static uint8_t transmit_pos, transmit_max;


/****************************************************************
 * Serial hardware
 ****************************************************************/

DECL_CONSTANT(SERIAL_BAUD, CONFIG_SERIAL_BAUD);

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
    uint8_t data = UDR0;
    if (data == MESSAGE_SYNC)
        sched_wake_tasks();
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

// Enable tx interrupts
static void
enable_tx_irq(void)
{
    UCSR0B |= 1<<UDRIE0;
}


/****************************************************************
 * Console access functions
 ****************************************************************/

// Remove from the receive buffer the given number of bytes
static void
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
            sched_wake_tasks();
        }
        irqstatus_t flag = irq_save();
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

// Process any incoming commands
void
console_task(void)
{
    uint8_t pop_count, rpos = readb(&receive_pos);
    int8_t ret = command_find_block(receive_buf, rpos, &pop_count);
    if (ret > 0)
        command_dispatch(receive_buf, pop_count);
    if (ret)
        console_pop_input(pop_count);
}
DECL_TASK(console_task);

// Encode and transmit a "response" message
void
console_sendf(const struct command_encoder *ce, va_list args)
{
    // Verify space for message
    uint8_t tpos = readb(&transmit_pos), tmax = readb(&transmit_max);
    if (tpos >= tmax) {
        tpos = tmax = 0;
        writeb(&transmit_max, 0);
        writeb(&transmit_pos, 0);
    }
    uint8_t max_size = READP(ce->max_size);
    if (tmax + max_size > sizeof(transmit_buf)) {
        if (tmax + max_size - tpos > sizeof(transmit_buf))
            // Not enough space for message
            return;
        // Disable TX irq and move buffer
        writeb(&transmit_max, 0);
        tpos = readb(&transmit_pos);
        tmax -= tpos;
        memmove(&transmit_buf[0], &transmit_buf[tpos], tmax);
        writeb(&transmit_pos, 0);
        writeb(&transmit_max, tmax);
        enable_tx_irq();
    }

    // Generate message
    char *buf = &transmit_buf[tmax];
    uint8_t msglen = command_encodef(buf, ce, args);
    command_add_frame(buf, msglen);

    // Start message transmit
    writeb(&transmit_max, tmax + msglen);
    enable_tx_irq();
}
