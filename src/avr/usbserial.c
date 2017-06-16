// Wrappers for AVR usb serial.
//
// Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <avr/interrupt.h> // USART0_RX_vect
#include <string.h> // memmove
#include "../lib/pjrc_usb_serial/usb_serial.h"
#include "command.h" // command_dispatch
#include "pgm.h" // READP
#include "sched.h" // DECL_INIT

#define USBSERIAL_BUFFER_SIZE 64
static char receive_buf[USBSERIAL_BUFFER_SIZE];
static uint8_t receive_pos;
static char transmit_buf[USBSERIAL_BUFFER_SIZE];

void
usbserial_init(void)
{
    usb_init();
}
DECL_INIT(usbserial_init);

// Return a buffer (and length) containing any incoming messages
static char *
console_get_input(uint8_t *plen)
{
    for (;;) {
        if (receive_pos >= sizeof(receive_buf))
            break;
        int16_t ret = usb_serial_getchar();
        if (ret == -1)
            break;
        receive_buf[receive_pos++] = ret;
    }
    *plen = receive_pos;
    return receive_buf;
}

// Remove from the receive buffer the given number of bytes
static void
console_pop_input(uint8_t len)
{
    uint8_t needcopy = receive_pos - len;
    if (needcopy)
        memmove(receive_buf, &receive_buf[len], needcopy);
    receive_pos = needcopy;
}

// Process any incoming commands
void
console_task(void)
{
    uint8_t buf_len, pop_count;
    char *buf = console_get_input(&buf_len);
    int8_t ret = command_find_block(buf, buf_len, &pop_count);
    if (ret > 0)
        command_dispatch(buf, pop_count);
    if (ret)
        console_pop_input(pop_count);
}
DECL_TASK(console_task);

// Return an output buffer that the caller may fill with transmit messages
static char *
console_get_output(uint8_t len)
{
    if (len > sizeof(transmit_buf))
        return NULL;
    return transmit_buf;
}

// Accept the given number of bytes added to the transmit buffer
static void
console_push_output(uint8_t len)
{
    usb_serial_write((void*)transmit_buf, len);
    usb_serial_flush_output();
}

// Encode and transmit a "response" message
void
console_sendf(const struct command_encoder *ce, va_list args)
{
    uint8_t buf_len = READP(ce->max_size);
    char *buf = console_get_output(buf_len);
    if (!buf)
        return;
    uint8_t msglen = command_encodef(buf, buf_len, ce, args);
    command_add_frame(buf, msglen);
    console_push_output(msglen);
}
