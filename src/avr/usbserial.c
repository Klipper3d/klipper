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

// Check for new incoming data
static void
console_check_input(void)
{
    for (;;) {
        if (receive_pos >= sizeof(receive_buf))
            break;
        int16_t ret = usb_serial_getchar();
        if (ret == -1)
            break;
        receive_buf[receive_pos++] = ret;
    }
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
    console_check_input();
    uint8_t pop_count;
    int8_t ret = command_find_block(receive_buf, receive_pos, &pop_count);
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
    // Generate message
    uint8_t max_size = READP(ce->max_size);
    if (max_size > sizeof(transmit_buf))
        return;
    uint8_t msglen = command_encodef(transmit_buf, max_size, ce, args);
    command_add_frame(transmit_buf, msglen);

    // Transmit message
    usb_serial_write((void*)transmit_buf, msglen);
    usb_serial_flush_output();
}
